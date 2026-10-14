#include "domain/item/usecases/ItemRandomizer.hpp"

using namespace domain::item;
using namespace domain::game;
using namespace domain::file;
using namespace domain::encounter;

namespace {
// Retail's IMPOSSIBLE tier denotes bonus items. The randomizer exposes the
// enabled bonus weapons in its Legendary bucket, not a fourth random tier.
bool needsStartingAmmo(pe2_item_tag_t tag) {
    return tag == PISTOL || tag == SHOTGUN || tag == MAGNUM || tag == GRENADE || tag == RIFLE;
}
std::vector<uint8_t> permittedAmmo(uint8_t weapon, const std::vector<uint8_t> &pool) {
    std::vector<uint8_t> result;
    const auto tag = items_map.at(weapon).main_tag;
    if (!needsStartingAmmo(tag))
        return result;
    for (const auto &entry : weapon_ammo_allowance_map.at(tag))
        result = utils::vector_works::unionVector(result, entry.second);
    return utils::vector_works::intersecVector(result, pool);
}
std::vector<uint8_t> permittedStartingWeapons(const std::vector<uint8_t> &weapons,
                                              const std::vector<uint8_t> &pool) {
    auto allowed = utils::vector_works::intersecVector(weapons, pool);
    allowed.erase(std::remove_if(allowed.begin(), allowed.end(),
                                 [&](uint8_t id) {
                                     return needsStartingAmmo(items_map.at(id).main_tag) &&
                                            permittedAmmo(id, pool).empty();
                                 }),
                  allowed.end());
    return allowed;
}
pe2_item_rarity_t poolRarity(pe2_item_rarity_t rarity) {
    return rarity == IMPOSSIBLE ? LEGENDARY : rarity;
}
} // namespace

pe2_randomizer_context usecase::items::ItemRandomizer::randomize(pe2_randomizer_context context) {
    if (config.flags.randomize_items) {
        randomizeStartingWeaponAndArmor(context);
        randomizeStartingItems();
        logger->log("[INF] [Randomizer] [Item Randomizer] Starting item randomization...");
        randomizePickups(AKROPOLIS_LEVEL, acropolis_pickup_items_map, false, false);
        logger->log("[INF] [Randomizer] [Item Randomizer] Pickups are randomized for akropolis level.");
        randomizePickups(AKROPOLIS_LEVEL, acropolis_shooting_range_rewards_map, true, false);
        logger->log("[INF] [Randomizer] [Item Randomizer] Pickups are randomized for mist shooting range.");
        randomizeRewards(AKROPOLIS_LEVEL, rewards_segment_map_by_level[AKROPOLIS_LEVEL].first,
                         context.reward_slots[AKROPOLIS_LEVEL]);
        randomizeRewards(AKROPOLIS_LEVEL, rewards_segment_map_by_level[AKROPOLIS_LEVEL].second,
                         context.bounty_reward_slots[AKROPOLIS_LEVEL]);
        logger->log("[INF] [Randomizer] [Item Randomizer] Rewards are randomized for mist akropolis level.");
        updateAkropolisScript();
        randomizePickups(BASE_LEVEL, dryfield_motel_room_6_box, false, true);
        randomizePickups(BASE_LEVEL, dryfield_day_items_pickup_dict, false, false);
        randomizeRewards(DRYFIELD_DAY_LEVEL, rewards_segment_map_by_level[DRYFIELD_DAY_LEVEL].first,
                         context.reward_slots[DRYFIELD_DAY_LEVEL]);
        randomizeRewards(DRYFIELD_DAY_LEVEL, rewards_segment_map_by_level[DRYFIELD_DAY_LEVEL].second,
                         context.bounty_reward_slots[DRYFIELD_DAY_LEVEL]);
        randomizePickups(BASE_LEVEL, dryfield_night_items_pickup_dict, false, false);
        randomizeRewards(DRYFIELD_NIGHT_LEVEL, rewards_segment_map_by_level[DRYFIELD_NIGHT_LEVEL].first,
                         context.reward_slots[DRYFIELD_NIGHT_LEVEL]);
        randomizeRewards(DRYFIELD_NIGHT_LEVEL, rewards_segment_map_by_level[DRYFIELD_NIGHT_LEVEL].second,
                         context.bounty_reward_slots[DRYFIELD_NIGHT_LEVEL]);
        randomizePickups(BASE_LEVEL, mines_and_shelter_sterilization_room_box, false, true);
        randomizePickups(MINES_AND_SHELTER_LEVEL, mines_and_shelter_pickup_items_dict, false, false);
        randomizePickups(BASE_LEVEL, dryfield_night_return_items_pickup_dict, false, false);
        randomizeRewards(MINES_AND_SHELTER_LEVEL, rewards_segment_map_by_level[MINES_AND_SHELTER_LEVEL].first,
                         context.reward_slots[MINES_AND_SHELTER_LEVEL]);
        randomizeRewards(MINES_AND_SHELTER_LEVEL,
                         rewards_segment_map_by_level[MINES_AND_SHELTER_LEVEL].second,
                         context.bounty_reward_slots[MINES_AND_SHELTER_LEVEL]);
        randomizePickups(NEOARK_AND_SHELTER_LEVEL, neo_ark_and_shelter_pickup_items_dict, false, false);
        randomizeRewards(NEOARK_AND_SHELTER_LEVEL,
                         rewards_segment_map_by_level[NEOARK_AND_SHELTER_LEVEL].first,
                         context.reward_slots[NEOARK_AND_SHELTER_LEVEL]);
        randomizeRewards(NEOARK_AND_SHELTER_LEVEL,
                         rewards_segment_map_by_level[NEOARK_AND_SHELTER_LEVEL].second,
                         context.bounty_reward_slots[NEOARK_AND_SHELTER_LEVEL]);
    }
    if (config.flags.randomize_shops) {
        randomizeShops();
        updateShopCost();
    }
    return context;
}

uint8_t usecase::items::ItemRandomizer::randomizeItem(std::vector<pe2_item_type_t> forbidden_types,
                                                      pe2_item_pickup_t pickup_type, bool forbid_weapons) {
    // Build only eligible pools before rolling. Empty categories and rarities
    // must never fall back to shop permissions or to hard-coded pistol ammo.
    std::map<pe2_item_type_t, std::map<pe2_item_rarity_t, std::vector<uint8_t>>> pools;
    std::vector<pe2_item_type_t> types;
    std::vector<double> type_weights;
    for (auto type : {HP_RECOVERY, MP_RECOVERY, AMMO, MISC, UNIQUE}) {
        if (std::find(forbidden_types.begin(), forbidden_types.end(), type) != forbidden_types.end())
            continue;
        for (auto rarity : {COMMON, RARE, LEGENDARY}) {
            auto ids = item_map_by_type.at(type).at(rarity);
            if (type == UNIQUE)
                ids = utils::vector_works::unionVector(ids, available_unique_items.at(rarity));
            if (type == AMMO)
                ids = utils::vector_works::unionVector(ids, available_ammo_items.at(rarity));
            ids = utils::vector_works::intersecVector(ids, config.randomized_items_drop_pool);
            if (config.flags.magic_pickup_mode && type == MISC)
                ids = utils::vector_works::unionVector(ids, magic.at(rarity));
            if (forbid_weapons && type == UNIQUE)
                ids = utils::vector_works::substractVector(ids, weapons_ids);
            if (!ids.empty())
                pools[type][rarity] = std::move(ids);
        }
        if (pools.count(type)) {
            types.push_back(type);
            type_weights.push_back(std::max(0.0, config.item_classes_distribution.at(type)));
        }
    }
    if (types.empty())
        throw std::runtime_error("No eligible items remain in the drop pool. Enable more items or change the "
                                 "starting weapon/ammo options.");
    std::discrete_distribution<size_t> type_roll(type_weights.begin(), type_weights.end());
    const auto type = types[type_roll(gen)];
    const auto &probabilities = pickup_type == REWARD          ? reward_item_rarity_probability_map
                                : pickup_type == PICKUP_SECRET ? secret_item_rarity_probability_map
                                                               : item_rarity_probability_map;
    std::vector<pe2_item_rarity_t> rarities;
    std::vector<double> rarity_weights;
    for (const auto &entry : pools.at(type)) {
        rarities.push_back(entry.first);
        rarity_weights.push_back(probabilities.at(entry.first));
    }
    std::discrete_distribution<size_t> rarity_roll(rarity_weights.begin(), rarity_weights.end());
    const auto rarity = rarities[rarity_roll(gen)];
    const auto &candidates = pools.at(type).at(rarity);
    const uint8_t item_id = candidates[std::uniform_int_distribution<size_t>(0, candidates.size() - 1)(gen)];

    pe2_item_t item = items_map[item_id];

    // 8. Let's added new items to pool if needed
    if (item.main_tag != NONE &&
        weapon_ammo_allowance_map.find(item.main_tag) != weapon_ammo_allowance_map.end()) {
        for (auto it = weapon_ammo_allowance_map.at(item.main_tag).begin();
             it != weapon_ammo_allowance_map.at(item.main_tag).end(); it++) {
            available_ammo_items.at(it->first) =
                utils::vector_works::unionVector(available_ammo_items.at(it->first), it->second);
        }
    }

    if (item.additional_tag != NONE &&
        weapon_attachment_allowance_map.find(item.additional_tag) != weapon_attachment_allowance_map.end()) {
        for (auto it = weapon_attachment_allowance_map.at(item.additional_tag).begin();
             it != weapon_attachment_allowance_map.at(item.additional_tag).end(); it++) {
            available_unique_items.at(it->first) =
                utils::vector_works::unionVector(available_unique_items.at(it->first), it->second);
        }
    }

    // 9. Remove item if it was unique
    if (item.type == UNIQUE) {
        available_unique_items.at(rarity) =
            utils::vector_works::substractVector(available_unique_items.at(rarity), {item_id});
        // 10. Update best items vector
        if (best_dropped_unique_items.size() < 3) {
            best_dropped_unique_items.push_back({item.id, item.value});
        } else {
            best_dropped_unique_items[2] = {item.id, item.value};
        }
        std::sort(best_dropped_unique_items.begin(), best_dropped_unique_items.end(),
                  [](const std::pair<uint8_t, size_t> &a, const std::pair<uint8_t, size_t> &b) {
                      return a.second > b.second;
                  });
    }

    return item_id;
}

void usecase::items::ItemRandomizer::randomizePickups(
    pe2_level_name_t level, std::vector<std::pair<size_t, pe2_item_pickup_t>> offsets, bool forbid_weapons,
    bool is_box) {
    std::string level_name = getNameFromLevel(level);
    std::string level_filename =
        temp_dir_name + getNameFromStage(STAGE0) + "/" + level_name + FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED;

    std::fstream level_file(level_filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!level_file) {
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error opening input file: ", level_filename);
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " + level_name +
                                 ".");
    }
    if (randomized_box_levels.insert(level).second) {
        std::vector<uint8_t> candidates;
        if (config.ammo_boxes_rando_option == 2)
            candidates = all_ammo;
        else
            for (const auto &entry : available_ammo_items)
                candidates = utils::vector_works::unionVector(candidates, entry.second);
        candidates = utils::vector_works::intersecVector(candidates, config.randomized_items_drop_pool);
        for (auto offset : ammo_boxes[level]) {
            if (config.ammo_boxes_rando_option == 0) {
                level_file.seekg(offset);
                const int original = level_file.get();
                if (original < 0)
                    throw std::runtime_error("Truncated ammo box descriptor");
                if (std::find(config.randomized_items_drop_pool.begin(),
                              config.randomized_items_drop_pool.end(),
                              original) != config.randomized_items_drop_pool.end())
                    continue;
            }
            if (candidates.empty()) {
                // Same disabled-box descriptor used by the No ammo boxes mod.
                utils::file_works::writeFourBytesLittleEndian(level_file, 0x00060000, offset + 2);
            } else {
                const auto id =
                    candidates[std::uniform_int_distribution<size_t>(0, candidates.size() - 1)(gen)];
                utils::file_works::writeByte(level_file, id, offset);
            }
        }
    }
    for (auto offset : offsets) {
        uint8_t item_id = randomizeItem({}, offset.second, forbid_weapons);
        utils::file_works::writeByte(level_file, item_id, offset.first);
        if (is_box) {
            pe2_item_t item = items_map[item_id];
            utils::file_works::writeTwoBytesLittleEndian(level_file, item.type == AMMO ? 0xffff : 0x01,
                                                         offset.first + 0x8);
        }
        // Make Doug's box appear
        if (level == BASE_LEVEL && offset.first == 0x810fa) {
            std::fstream dryfield_level_file(temp_dir_name + getNameFromStage(STAGE0) + "/" +
                                                 getNameFromLevel(DRYFIELD_DAY_LEVEL) + FORMAT_LEVEL_FILE +
                                                 FORMAT_DECOMPRESSED,
                                             std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
            if (!dryfield_level_file) {
                logger->log("[ERR] [Randomizer] [ItemRandomizer] Error opening input file: ",
                            temp_dir_name + getNameFromStage(STAGE0) + "/" +
                                getNameFromLevel(DRYFIELD_DAY_LEVEL) + FORMAT_LEVEL_FILE +
                                FORMAT_DECOMPRESSED);
                logger->log("[ERR] [Randomizer] [ItemRandomizer] Error code : () ", std::strerror(errno),
                            " )");
                throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " +
                                         temp_dir_name + getNameFromStage(STAGE0) + "/" +
                                         getNameFromLevel(DRYFIELD_DAY_LEVEL) + FORMAT_LEVEL_FILE +
                                         FORMAT_DECOMPRESSED + ".");
            }
            utils::file_works::writeByte(dryfield_level_file, item_id, 0xbf4);
            dryfield_level_file.close();
            std::fstream dryfield_night_level_file(
                temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(DRYFIELD_NIGHT_LEVEL) +
                    FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED,
                std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
            if (!dryfield_night_level_file) {
                logger->log("[ERR] [Randomizer] [ItemRandomizer] Error opening input file: ",
                            temp_dir_name + getNameFromStage(STAGE0) + "/" +
                                getNameFromLevel(DRYFIELD_NIGHT_LEVEL) + FORMAT_LEVEL_FILE +
                                FORMAT_DECOMPRESSED);
                logger->log("[ERR] [Randomizer] [ItemRandomizer] Error code : () ", std::strerror(errno),
                            " )");
                throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " +
                                         temp_dir_name + getNameFromStage(STAGE0) + "/" +
                                         getNameFromLevel(DRYFIELD_NIGHT_LEVEL) + FORMAT_LEVEL_FILE +
                                         FORMAT_DECOMPRESSED + ".");
            }
            utils::file_works::writeByte(dryfield_night_level_file, item_id, 0xadc);
            dryfield_night_level_file.close();
        }
        // Make Glutton box appear
        if (level == MINES_AND_SHELTER_LEVEL && offset.first == 0x211a) {
            utils::file_works::writeByte(level_file, item_id, 0xfc8);
        }
    }
}

void usecase::items::ItemRandomizer::randomizeRewards(pe2_level_name_t level,
                                                      pe2_enc_rewards_segment_t reward_segment,
                                                      std::vector<pe2_enc_reward_descriptor_t> rewards) {
    logger->log("[INF] [Randomizer] [Item Randomizer] Entering reward randomization function.");
    std::string level_name = getNameFromLevel(level);
    std::string level_filename =
        temp_dir_name + getNameFromStage(STAGE0) + "/" + level_name + FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED;

    std::fstream level_file(level_filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!level_file) {
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error opening input file: ", level_filename);
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " + level_name +
                                 ".");
    }
    if (rewards.empty()) {
        for (size_t i = 0; i < reward_segment.num; ++i) {
            uint8_t row[12]{};
            level_file.seekg(reward_segment.starting_offset + i * sizeof(row));
            if (!level_file.read(reinterpret_cast<char *>(row), sizeof(row)))
                throw std::runtime_error("Cannot read reward table for " + level_name);
            if (row[0] == 0xff)
                break;
            pe2_enc_reward_descriptor_t reward;
            reward.encounter_id = row[1];
            reward.room_id = row[2];
            reward.stage_id = row[3];
            rewards.push_back(reward);
        }
    }
    std::vector<std::vector<char>> reward_descriptors_bytes{};
    if (rewards.size() > reward_segment.num) {
        throw std::runtime_error("Reward list exceeds the reserved table for " + level_name);
    }
    for (auto reward : rewards) {
        logger->log("[INF] [Randomizer] [Randomizer] - ", (int)reward.encounter_id, " - ",
                    (int)reward.room_id, " - ", (int)reward.stage_id, " - ", (int)reward.value);
        // 1. calculate how many items will be randomized
        int items_num = 1;
        if (reward.value > 200) {
            items_num++;
        }
        if (reward.value > 400) {
            items_num++;
        }
        reward.items_list = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
        for (size_t i = 0; i < (size_t)items_num; i++) {
            logger->log("[INF] [Randomizer] [ItemRandomizer] Choosing item # ", i + 1);
            uint8_t item_id = randomizeItem({}, REWARD,
                                            (reward_segment.mode & domain::game::MODE_BOUNTY) ||
                                                (reward_segment.mode & domain::game::MODE_NIGTHMARE));
            reward.items_list[i].first = item_id;
        }
        // 2. Let's randomize Medicine Wheel fo this encounter sometimes
        std::uniform_real_distribution dis(0.0, 1.0);
        double prob = dis(gen);
        if (prob < MEDICINE_WHEEL_RPOBABILITY) {
            logger->log("[INF] [Randomizer] [ItemRandomizer] Choosing Medicine wheel");
            uint8_t item_id = randomizeItem({}, REWARD, true);
            reward.items_list[3].first = item_id;
        }
        reward_descriptors_bytes.push_back(rewardToByteVector(reward));
    }

    // The game scans until the 0xffffffff key, not until our vector ends.
    // Remove unused retail rows so a shorter replacement cannot expose old
    // rewards (including items explicitly excluded by the user).
    while (reward_descriptors_bytes.size() < reward_segment.num) {
        std::vector<char> terminator(12, 0);
        std::fill_n(terminator.begin(), 4, static_cast<char>(0xff));
        reward_descriptors_bytes.push_back(std::move(terminator));
    }
    utils::file_works::writeVectorsToFile(level_file, reward_segment.starting_offset,
                                          reward_descriptors_bytes);
}

void usecase::items::ItemRandomizer::randomizeStartingWeaponAndArmor(pe2_randomizer_context &context) {
    std::fstream base_file(temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                               FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED,
                           std::ios::in | std::ios::out | std::ios::binary);
    if (!base_file) {
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                        FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED);
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " +
                                 getNameFromLevel(BASE_LEVEL) + ".");
    }
    if (config.armor_randomization_strategy != RANDO_STRATEGY_UNRANDOMIZED) {
        uint8_t armor_id = 0x0;
        switch (config.armor_randomization_strategy) {
        case RANDO_STRATEGY_FIXED:
            armor_id = config.starting_armor;
            break;
        case RANDO_STRATEGY_STARTING:
            armor_id = utils::vector_works::getRandomElement(gen, starting_armors, 0x60);
            break;
        case RANDO_STRATEGY_FULL_RANDOM:
            armor_id = utils::vector_works::getRandomElement(gen, all_armors, 0x60);
            break;
        default:
            break;
        }
        utils::file_works::writeByte(base_file, armor_id, 0x24610);
        utils::file_works::writeByte(base_file, armor_id, 0x249f0);
        utils::file_works::writeByte(base_file, armor_id, 0x24a00);
    }
    if (config.dryfield_armor_randomization_strategy != RANDO_STRATEGY_UNRANDOMIZED) {
        uint8_t armor_id = 0x63;
        switch (config.dryfield_armor_randomization_strategy) {
        case RANDO_STRATEGY_FIXED:
            armor_id = config.dryfield_armor;
            break;
        case RANDO_STRATEGY_STARTING:
            armor_id = utils::vector_works::getRandomElement(gen, starting_armors, 0x63);
            break;
        case RANDO_STRATEGY_FULL_RANDOM:
            armor_id = utils::vector_works::getRandomElement(gen, all_armors, 0x63);
            break;
        default:
            break;
        }
        utils::file_works::writeByte(base_file, armor_id, 0x246d4);
        utils::file_works::writeByte(base_file, armor_id, 0x246e0);
    }
    if (config.weapon_randomization_strategy == RANDO_STRATEGY_UNRANDOMIZED) {
        available_ammo_items = weapon_ammo_allowance_map.at(PISTOL);
    }
    if (config.weapon_randomization_strategy != RANDO_STRATEGY_UNRANDOMIZED) {
        uint8_t weapon_id = 0x81;
        switch (config.weapon_randomization_strategy) {
        case RANDO_STRATEGY_FIXED:
            weapon_id = config.starting_weapon;
            break;
        case RANDO_STRATEGY_STARTING: {
            auto allowed = permittedStartingWeapons(starting_weapons, config.randomized_items_drop_pool);
            if (allowed.empty())
                throw std::runtime_error("No allowed starting weapons. Enable one in the drop pool or select "
                                         "a fixed starting weapon.");
            weapon_id = utils::vector_works::getRandomElement(gen, allowed, 0);
        } break;
        case RANDO_STRATEGY_FULL_RANDOM: {
            auto allowed = permittedStartingWeapons(all_weapons, config.randomized_items_drop_pool);
            if (allowed.empty())
                throw std::runtime_error("No allowed starting weapons. Enable one in the drop pool or select "
                                         "a fixed starting weapon.");
            weapon_id = utils::vector_works::getRandomElement(gen, allowed, 0);
        } break;
        default:
            break;
        }
        utils::file_works::writeByte(base_file, weapon_id, 0x245f0);
        utils::file_works::writeByte(base_file, weapon_id, 0x2472c);
        utils::file_works::writeByte(base_file, weapon_id, 0x24758);
        utils::file_works::writeByte(base_file, weapon_id, 0x24a54);
        utils::file_works::writeByte(base_file, weapon_id, 0x27310);
        utils::file_works::writeByte(base_file, weapon_id, 0x27344);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0x00000000, 0x27304);
        context.selected_weapon = weapon_id;

        pe2_item_t weapon = items_map[weapon_id];

        // Keep the retail gallery whitelist. Inserting an arbitrary starting
        // weapon here made MM1/Grenade Pistol selectable in unsupported trials.
        available_unique_items[weapon.rarity] =
            utils::vector_works::substractVector(available_unique_items[weapon.rarity], {weapon.id});
        uint8_t ammo_id = 0xa0;
        uint16_t ammo_ammount = 0x64;
        switch (weapon.main_tag) {
        case PISTOL:
            ammo_ammount = config.flags.full_ammo ? 500 : 100;
            ammo_id = 0xa0;
            break;
        case SHOTGUN:
            ammo_ammount = config.flags.full_ammo ? 200 : 50;
            ammo_id = 0xac;
            break;
        case MAGNUM:
            ammo_ammount = config.flags.full_ammo ? 500 : 100;
            ammo_id = 0xa6;
            break;
        case GRENADE:
            ammo_ammount = config.flags.full_ammo ? 100 : 20;
            ammo_id = 0xa9;
            break;
        case RIFLE:
            ammo_ammount = config.flags.full_ammo ? 800 : 240;
            ammo_id = 0xaf;
            break;
        default:
            break;
        }
        if (needsStartingAmmo(weapon.main_tag)) {
            const auto allowed = permittedAmmo(weapon_id, config.randomized_items_drop_pool);
            if (allowed.empty())
                throw std::runtime_error("Starting weapon has no enabled ammunition in the drop pool. Enable "
                                         "compatible ammo or change the starting weapon.");
            if (std::find(allowed.begin(), allowed.end(), ammo_id) == allowed.end())
                ammo_id = allowed.front();
        } else {
            // The retail starting script also grants an ammo stack for melee/
            // rechargeable weapons. Keep that grant inside the configured pool.
            auto allowed = utils::vector_works::intersecVector(all_ammo, config.randomized_items_drop_pool);
            if (allowed.empty())
                throw std::runtime_error("The starting loadout requires an enabled ammo stack. Enable at "
                                         "least one ammunition type in the drop pool.");
            ammo_id = allowed.front();
        }
        utils::file_works::writeByte(base_file, ammo_id, 0x24608);
        utils::file_works::writeByte(base_file, ammo_id, 0x24744);
        utils::file_works::writeByte(base_file, ammo_id, 0x2475c);
        utils::file_works::writeByte(base_file, ammo_id, 0x24a38);
        utils::file_works::writeByte(base_file, ammo_id, 0x27348);
        utils::file_works::writeTwoBytesLittleEndian(base_file, ammo_ammount, 0x24748);
        utils::file_works::writeTwoBytesLittleEndian(base_file, ammo_ammount, 0x24a3c);
        utils::file_works::writeTwoBytesLittleEndian(base_file, 0x00000000, 0x2731c);

        // Let's added new items to pool if needed
        pe2_item_t item = items_map[weapon_id];
        if (item.main_tag != NONE &&
            weapon_ammo_allowance_map.find(item.main_tag) != weapon_ammo_allowance_map.end()) {
            for (auto it = weapon_ammo_allowance_map.at(item.main_tag).begin();
                 it != weapon_ammo_allowance_map.at(item.main_tag).end(); it++) {
                available_ammo_items.at(it->first) =
                    utils::vector_works::unionVector(available_ammo_items.at(it->first), it->second);
            }
        }

        if (item.additional_tag != NONE && weapon_attachment_allowance_map.find(item.additional_tag) !=
                                               weapon_attachment_allowance_map.end()) {
            for (auto it = weapon_attachment_allowance_map.at(item.additional_tag).begin();
                 it != weapon_attachment_allowance_map.at(item.additional_tag).end(); it++) {
                available_unique_items.at(it->first) =
                    utils::vector_works::unionVector(available_unique_items.at(it->first), it->second);
            }
        }

        // Remove item if it was unique
        if (item.type == UNIQUE) {
            auto &bucket = available_unique_items.at(poolRarity(item.rarity));
            bucket = utils::vector_works::substractVector(bucket, {item.id});
            // 10. Update best items vector
            if (best_dropped_unique_items.size() < 3) {
                best_dropped_unique_items.push_back({item.id, item.value});
            } else {
                best_dropped_unique_items[2] = {item.id, item.value};
            }
            std::sort(best_dropped_unique_items.begin(), best_dropped_unique_items.end(),
                      [](const std::pair<uint8_t, size_t> &a, const std::pair<uint8_t, size_t> &b) {
                          return a.second > b.second;
                      });
        }
    }
}

void usecase::items::ItemRandomizer::randomizeStartingItems() {
    std::fstream base_file(temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                               FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED,
                           std::ios::in | std::ios::out | std::ios::binary);
    if (!base_file) {
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                        FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED);
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " +
                                 getNameFromLevel(BASE_LEVEL) + ".");
    }
    std::vector<uint8_t> starting_item_options =
        utils::vector_works::intersecVector(starting_rare_items_id, config.randomized_items_drop_pool);

    if (config.flags.magic_pickup_mode) {
        starting_item_options.insert(starting_item_options.begin(), starting_magic.begin(),
                                     starting_magic.end());
    }

    uint8_t random_item_id = starting_item_options.empty()
                                 ? randomizeItem({}, PICKUP_DEFAULT, false)
                                 : utils::vector_works::getRandomElement(gen, starting_item_options, 0);
    utils::file_works::writeByte(base_file, random_item_id, 0x24a78);
    starting_item_options =
        utils::vector_works::intersecVector(starting_unique_items, config.randomized_items_drop_pool);
    random_item_id = starting_item_options.empty()
                         ? randomizeItem({}, PICKUP_DEFAULT, false)
                         : utils::vector_works::getRandomElement(gen, starting_item_options, 0);
    utils::file_works::writeByte(base_file, random_item_id, 0x24704);
    utils::file_works::writeByte(base_file, random_item_id, 0x24a28);
    utils::file_works::writeByte(base_file, random_item_id, 0x24618);
}

void usecase::items::ItemRandomizer::randomizeShops() {
    for (size_t i = 0; i < shops_vector.size(); i++) {
        std::vector<uint8_t> items{};
        // Randomize Weapons
        std::vector<uint8_t> weapons =
            getShopRandomItemsList(utils::vector_works::intersecVector(all_weapons_and_attachments,
                                                                       config.randomized_items_shop_pool),
                                   shops_vector[i].weapon, false);
        items = utils::vector_works::concatVector(items, weapons);
        // Randomize Ammo
        std::vector<uint8_t> possible_ammo{};
        for (size_t j = 0; j < weapons.size(); j++) {
            if (weapons[j] != 0x00 && weapons[j] != 0xFF && weapons[j] != 0xFE) {
                pe2_item_t item = items_map[weapons[j]];
                possible_ammo = utils::vector_works::unionVector(
                    possible_ammo, weapon_ammo_allowance_map.at(item.main_tag).at(COMMON));
                possible_ammo = utils::vector_works::unionVector(
                    possible_ammo, weapon_ammo_allowance_map.at(item.main_tag).at(RARE));
                possible_ammo = utils::vector_works::unionVector(
                    possible_ammo, weapon_ammo_allowance_map.at(item.main_tag).at(LEGENDARY));
            }
        }
        const auto allowed_ammo =
            utils::vector_works::intersecVector(all_ammo, config.randomized_items_shop_pool);
        possible_ammo = utils::vector_works::intersecVector(possible_ammo, allowed_ammo);
        auto extra_ammo = utils::vector_works::substractVector(allowed_ammo, possible_ammo);
        std::shuffle(extra_ammo.begin(), extra_ammo.end(), gen);
        for (auto id : extra_ammo) {
            if (possible_ammo.size() >= shops_vector[i].ammo)
                break;
            possible_ammo.push_back(id);
        }
        std::vector<uint8_t> ammo =
            getShopRandomItemsList(possible_ammo, shops_vector[i].ammo, shops_vector[i].has_charger);
        items = utils::vector_works::concatVector(items, ammo);
        // Randomize Armor
        std::vector<uint8_t> armors = getShopRandomItemsList(
            utils::vector_works::intersecVector(all_armors, config.randomized_items_shop_pool),
            shops_vector[i].armor, false);
        items = utils::vector_works::concatVector(items, armors);
        // Randomize Items
        std::vector<uint8_t> possible_misc_items =
            utils::vector_works::intersecVector(all_misc_items, config.randomized_items_shop_pool);
        if (config.flags.magic_pickup_mode) {
            possible_misc_items.insert(possible_misc_items.begin(), all_magic.begin(), all_magic.end());
        }
        std::vector<uint8_t> miscs = getShopRandomItemsList(possible_misc_items, shops_vector[i].item, false);
        items = utils::vector_works::concatVector(items, miscs);
        for (size_t j = 0; j < shops_vector[i].location.size(); j++) {
            std::fstream base_file(temp_dir_name + shops_vector[i].location[j] + FORMAT_ROOM_FILE +
                                       FORMAT_DECOMPRESSED,
                                   std::ios::in | std::ios::out | std::ios::binary);
            if (!base_file) {
                logger->log("[ERR] [Randomizer] [ItemRandomizer] Error opening input file: ",
                            temp_dir_name + shops_vector[i].location[j] + FORMAT_LEVEL_FILE +
                                FORMAT_DECOMPRESSED);
                logger->log("[ERR] [Randomizer] [ItemRandomizer] Error code : () ", std::strerror(errno),
                            " )");
                throw std::runtime_error("[ItemRandomizer] couldn't open room file file for room " +
                                         temp_dir_name + shops_vector[i].location[j] + FORMAT_LEVEL_FILE +
                                         FORMAT_DECOMPRESSED + ".");
            }
            if (!items.empty()) {
                base_file.seekp(shops_vector[i].offset[j]);
                base_file.write(reinterpret_cast<const char *>(items.data()), items.size());
            }
            base_file.close();
        }
    }
}

std::vector<uint8_t> usecase::items::ItemRandomizer::getShopRandomItemsList(std::vector<uint8_t> vector,
                                                                            size_t items_num,
                                                                            bool recharger_flag) {
    size_t list_len_in_bytes = 0;
    std::vector<uint8_t> result{};
    std::shuffle(vector.begin(), vector.end(), gen);
    for (size_t i = 0; i < items_num; i++) {
        uint8_t item_id = i < vector.size() ? vector[i] : 0x00;
        result.push_back(item_id);
        result.push_back(0x00);
        list_len_in_bytes += 2;
    }
    if (recharger_flag) {
        result.push_back(0xFE);
        result.push_back(0xFF);
        list_len_in_bytes += 2;
    }
    result.push_back(0xFF);
    result.push_back(0xFF);
    list_len_in_bytes += 2;

    if (list_len_in_bytes % 4 != 0) {
        result.push_back(0x00);
        result.push_back(0x00);
    }
    return result;
}

void usecase::items::ItemRandomizer::updateAkropolisScript() {
    std::fstream base_file(temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                               FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED,
                           std::ios::in | std::ios::out | std::ios::binary);
    if (!base_file) {
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                        FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED);
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " +
                                 getNameFromLevel(BASE_LEVEL) + ".");
    }

    std::vector<size_t> offsets = {
        0x245d0, 0x245d8,
        0x245b0, // this is for weapon with possible attachments randomization
    };

    // Check if we have anything that potentially can have attachments: mp5a5, m4a1, p08
    int attachments_num = 1;
    for (size_t i = 0; i < best_dropped_unique_items.size(); i++) {
        switch (best_dropped_unique_items[i].first) {
        case 0x8f:
            attachments_num = 0xc;
            break;
        case 0x9d:
            attachments_num = 0x3;
            break;
        }
        // A restricted pool can leave fewer than three unique rewards.
        // Only reorder into the attachment slot when that slot exists.
        if (i != 2 && best_dropped_unique_items.size() > 2) {
            std::swap(best_dropped_unique_items[i], best_dropped_unique_items[2]);
            break;
        }
    }

    for (size_t i = 0; i < best_dropped_unique_items.size(); i++) {
        pe2_item_t item = items_map[best_dropped_unique_items[i].first];
        // Return this to rando as it is now again available
        available_unique_items.at(poolRarity(item.rarity)).push_back(item.id);
        if (i != 2) {
            utils::file_works::writeByte(base_file, best_dropped_unique_items[i].first, offsets[i]);
        } else {
            utils::file_works::writeByte(base_file, 0x100 - best_dropped_unique_items[i].first, offsets[i]);
            utils::file_works::writeByte(base_file, attachments_num, offsets[i] + 0x8);
        }
    }
}

void usecase::items::ItemRandomizer::updateShopCost() {
    std::fstream base_file(temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                               FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED,
                           std::ios::in | std::ios::out | std::ios::binary);
    if (!base_file) {
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                        FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED);
        logger->log("[ERR] [Randomizer] [ItemRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[ItemRandomizer] couldn't open level file file for level " +
                                 getNameFromLevel(BASE_LEVEL) + ".");
    }
    for (size_t i = 0; i < shop_items.size(); i++) {
        uint16_t value = config.shop_costs[i];
        base_file.seekp(ITEM_COST_START_ADDRESS + (shop_items[i] - 1) * 8, std::ios::beg);
        base_file.write(reinterpret_cast<const char *>(&value), sizeof(value));
        base_file.flush();
    }
    for (size_t i = 0; i < 0xb0; i++) {
        uint16_t value;
        base_file.seekp(ITEM_COST_START_ADDRESS + i * 8, std::ios::beg);
        base_file.read(reinterpret_cast<char *>(&value), sizeof(value));

        if (new_costs.find(ITEM_COST_START_ADDRESS + i * 8) != new_costs.end()) {
            value = new_costs[ITEM_COST_START_ADDRESS + i * 8];
        }
        base_file.seekp(ITEM_COST_START_ADDRESS + i * 8, std::ios::beg);
        if ((int)value * config.shop_sost_multiplier <= MAXIMUM_SHOP_COST) {
            value *= config.shop_sost_multiplier;
        } else {
            value = MAXIMUM_SHOP_COST;
        }

        base_file.write(reinterpret_cast<const char *>(&value), sizeof(value));
        base_file.flush();
    }
    base_file.close();
}
