#include "domain/armor/usecases/ArmorRandomizer.hpp"
#include "domain/armor/entities/ArmorAssets.hpp"
#include "domain/armor/entities/ArmorLabels.hpp"
#include "resources/EmbeddedResources.hpp"

using namespace domain::armor;
using namespace domain::game;
using namespace domain::file;

pe2_randomizer_context usecase::armors::ArmorRandomizer::randomize(pe2_randomizer_context context) {
    std::fstream game_lib_file(temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                                   FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                        FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED);
        logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " + temp_dir_name +
                                 getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                                 FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED + ".");
    }

    std::fstream stage0_file(temp_dir_name + getNameFromStage(STAGE0) + FORMAT_CDF,
                             std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!stage0_file) {
        logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE0) + FORMAT_CDF);
        logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " + temp_dir_name +
                                 getNameFromStage(STAGE0) + FORMAT_CDF + ".");
    }

    std::fstream slus_file(temp_dir_name + getNameFromStage(SLUS) + FORMAT_CDF,
                           std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!slus_file) {
        logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(SLUS) + FORMAT_CDF);
        logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " + temp_dir_name +
                                 getNameFromStage(SLUS) + FORMAT_CDF + ".");
    }

    std::vector<std::vector<char>> armor_records{};
    std::vector<std::string> armor_labels{};

    std::map<domain::item::pe2_item_rarity_t, std::vector<size_t>> dropped_armors_map{
        {domain::item::COMMON, std::vector<size_t>{}},
        {domain::item::RARE, std::vector<size_t>{}},
        {domain::item::LEGENDARY, std::vector<size_t>{}},
    };

    // 1. Find out what armors should be changed
    for (uint8_t armor_id = 0x60; armor_id <= 0x6d; armor_id++) {
        pe2_armor_slot_t armor_slot = armor_slots_map[armor_id];
        std::uniform_real_distribution<> dis(0.0, 1.0);
        double prob = dis(gen);
        if (prob < armor_prob_map[armor_slot.rarity]) {
            size_t random_index = 0;
            do {
                std::uniform_int_distribution<> dis(0, armors_pool[armor_slot.rarity].size() - 1);
                random_index = dis(gen);
            } while (std::find(dropped_armors_map[armor_slot.rarity].begin(),
                               dropped_armors_map[armor_slot.rarity].end(),
                               random_index) != dropped_armors_map[armor_slot.rarity].end());
            pe2_armor_t chosen_armor = armors_pool[armor_slot.rarity][random_index];
            dropped_armors_map[armor_slot.rarity].push_back(random_index);

            const auto palette_buffer = pe2::resources::copy("game/armor/" + chosen_armor.image_file +
                                                             FORMAT_PALETTE + FORMAT_COMPRESSED);
            const auto texture_buffer = pe2::resources::copy("game/armor/" + chosen_armor.image_file +
                                                             FORMAT_TEXTURE + FORMAT_COMPRESSED);
            try {
                validateArmorImage(texture_buffer, palette_buffer);
            } catch (const std::runtime_error &error) {
                throw std::runtime_error("Invalid armor image " + chosen_armor.image_file + ": " +
                                         error.what());
            }

            if (texture_buffer.size() >=
                armor_slot.special_features_text_offset - 0x20 - armor_slot.texture_offset) {
                logger->log("[ERR] [Randomizer] [ArmorRandomizer] Don't have enough size for armor " +
                                chosen_armor.image_file + " texture. Had ",
                            armor_slot.special_features_text_offset - 0x20 - armor_slot.texture_offset,
                            " but needed ", texture_buffer.size(), ".");
                armor_records.push_back(build_armor_stats_descriptor(
                    armor_slot.default_hp_bonus, armor_slot.default_mp_bonus, armor_slot.default_attachments,
                    armor_slot.default_status_effects));
                armor_labels.push_back(armor_slot.default_name);
                armor_labels.push_back(armor_slot.default_description);
                continue;
            }

            const size_t palette_capacity = armor_slot.texture_offset - armor_slot.palette_offset - 0x20;
            if (palette_buffer.size() > palette_capacity)
                throw std::runtime_error("Armor palette exceeds its slot: " + chosen_armor.image_file);
            utils::file_works::writeTwoBytesLittleEndian(
                game_lib_file,
                chosen_armor.cost * config.shop_sost_multiplier >= 65535
                    ? 65535
                    : chosen_armor.cost * config.shop_sost_multiplier,
                armor_slot.cost_offset);
            stage0_file.seekp(armor_slot.texture_offset);
            stage0_file.write(texture_buffer.data(), texture_buffer.size());
            std::vector<char> tex_zeroes(armor_slot.special_features_text_offset - armor_slot.texture_offset -
                                             0x10 - texture_buffer.size(),
                                         0);
            stage0_file.write(tex_zeroes.data(), tex_zeroes.size());

            stage0_file.seekp(armor_slot.palette_offset);
            stage0_file.write(palette_buffer.data(), palette_buffer.size());
            std::vector<char> pal_zeroes(palette_capacity - palette_buffer.size(), 0);
            stage0_file.write(pal_zeroes.data(), pal_zeroes.size());

            std::string special_features =
                "\x0d\x0a\x0d\x0a\x0d\x0a\x0d\x0a\x0d\x0a\x0d\x0a\x0d\x0a\x0d\x0a\x0d\x0a\x20";

            bool is_first = true;
            for (size_t feature = 1; feature <= MP_RECOVERY; feature = feature << 1) {
                if ((feature & (chosen_armor.status_effects)) != 0) {
                    if (is_first) {
                        is_first = false;
                    } else {
                        special_features += "\x2e\x0d\x0a\x20";
                    }
                    special_features += special_features_map[feature];
                }
            }
            if ((chosen_armor.status_effects) != 0) {
                special_features +=
                    "\x2e\x0d\x0a\x5c\x5a\x0d\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
            } else {
                special_features +=
                    "\x0a\x0d\x0a\x5c\x5a\x0d\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
                    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
            }
            stage0_file.seekp(armor_slot.special_features_text_offset);
            stage0_file.write(special_features.c_str(), special_features.size());
            char zeroes[] = {'\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
                             '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
                             '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
                             '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
                             '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'};
            stage0_file.write(zeroes, sizeof(zeroes));

            armor_records.push_back(build_armor_stats_descriptor(chosen_armor.hp_bonus, chosen_armor.mp_bonus,
                                                                 chosen_armor.attachments,
                                                                 chosen_armor.status_effects));
            armor_labels.push_back(chosen_armor.name);
            armor_labels.push_back(chosen_armor.description);
        } else {
            armor_records.push_back(build_armor_stats_descriptor(
                armor_slot.default_hp_bonus, armor_slot.default_mp_bonus, armor_slot.default_attachments,
                armor_slot.default_status_effects));
            armor_labels.push_back(armor_slot.default_name);
            armor_labels.push_back(armor_slot.default_description);
        }
    }

    utils::file_works::writeVectorsToFile(game_lib_file, ARMOR_STATS_LIST_OFFSET, armor_records);

    // 2. Patch game to have new abilities stats working

    for (const auto &entry : new_armor_patch_map) {
        game_lib_file.seekp(entry.first);
        if (!game_lib_file.write(reinterpret_cast<const char *>(entry.second.data()), entry.second.size())) {
            logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error: failed to write data to the file");
            logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error code : () ", std::strerror(errno), " )");
            throw std::runtime_error("[ArmorRandomizer] Error: failed to write data to the file. ");
        }
    }

    for (const auto &entry : new_armor_patch_map_slus) {
        slus_file.seekp(entry.first);
        if (!slus_file.write(reinterpret_cast<const char *>(entry.second.data()), entry.second.size())) {
            logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error: failed to write data to the slus file");
            logger->log("[ERR] [Randomizer] [ArmorRandomizer] Error code : () ", std::strerror(errno), " )");
            throw std::runtime_error("[ArmorRandomizer] Error: failed to write data to the slus file. ");
        }
    }

    // 3. Patching text labels

    const size_t capacity = ARMOR_LABELS_LIST_END_OFFSET - ARMOR_LABELS_LIST_START_OFFSET;
    std::vector<char> labels;
    std::vector<uint32_t> addresses;
    for (size_t i = 0; i < armor_labels.size(); i += 2) {
        addresses.push_back(0x80093800 + ARMOR_LABELS_LIST_START_OFFSET + labels.size());
        const auto record = armorLabelRecord(armor_labels[i], armor_labels[i + 1], i / 2 == 5);
        labels.insert(labels.end(), record.begin(), record.end());
    }
    if (labels.size() > capacity)
        throw std::runtime_error("Armor descriptions exceed the original text pool");
    // Validate the entire pool before writing any label or pointer. Never
    // truncate one description and underflow the capacity on the next name.
    labels.resize(capacity, 0);
    game_lib_file.seekp(ARMOR_LABELS_LIST_START_OFFSET);
    if (!game_lib_file.write(labels.data(), labels.size()))
        throw std::runtime_error("Cannot write armor descriptions");
    for (size_t i = 0; i < addresses.size(); ++i)
        utils::file_works::writeFourBytesLittleEndian(game_lib_file, addresses[i],
                                                      ARMOR_LABELS_ADDRESSES_OFFSET + i * 8);

    game_lib_file.close();

    return context;
}

std::vector<char> usecase::armors::ArmorRandomizer::build_armor_stats_descriptor(int16_t hp_bonus,
                                                                                 uint8_t mp_bonus,
                                                                                 uint8_t attachments,
                                                                                 uint32_t stats) {
    return std::vector<char>{(char)((stats & 0xff000000) >> 24),
                             (char)((stats & 0x00ff0000) >> 16),
                             (char)((stats & 0x0000ff00) >> 8),
                             (char)((stats & 0x000000ff)),
                             (char)(hp_bonus >= 0 ? hp_bonus : -hp_bonus),
                             (char)attachments,
                             (char)mp_bonus,
                             (char)(hp_bonus >= 0 ? 0 : 1)};
}
