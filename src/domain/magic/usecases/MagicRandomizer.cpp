#include "domain/magic/usecases/MagicRandomizer.hpp"

using namespace domain::magic;
using namespace domain::game;
using namespace domain::file;

pe2_randomizer_context usecase::magic::MagicRandomizer::randomize(pe2_randomizer_context context) {
    randomizeStartingMagic();
    return context;
}

void usecase::magic::MagicRandomizer::randomizeStartingMagic() {
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

    // 1. choose random starting spell
    uint8_t random_magic = 0;
    uint8_t random_magic_level = 1;

    std::uniform_int_distribution<size_t> dist_only_first_magic(0, first_circle_magic.size() - 1);
    std::uniform_int_distribution<size_t> dist_level(1, 3);
    std::uniform_int_distribution<size_t> dist_any_circle(0, all_cricles_magic.size() - 1);
    switch (config.spell_randomization_strategy) {
    case RANDO_STRATEGY_MAGIC_FIRST_CIRCLE_RANDOM:
        random_magic = (uint8_t)first_circle_magic[dist_only_first_magic(gen)];
        break;
    case RANDO_STRATEGY_MAGIC_FULL_RANDOM:
        random_magic_level = (uint8_t)all_cricles_magic[dist_level(gen)];
        // fallthrough
    case RANDO_STRATEGY_MAGIC_STARTING:
        random_magic = (uint8_t)dist_any_circle(gen);
        break;
    default:;
    }
    if (config.spell_randomization_strategy == RANDO_STRATEGY_MAGIC_FIXED) {
        random_magic = config.starting_spell;
        random_magic_level = config.starting_spell_level + 1;
    }

    // 2. choose random starting spell
    if (config.spell_randomization_strategy != RANDO_STRATEGY_MAGIC_UNRANDOMIZED) {
        utils::file_works::writeByte(base_file, random_magic_level, 0x249c0);
        utils::file_works::writeByte(base_file, 0x50 + random_magic, 0x249c8);
        utils::file_works::writeByte(base_file, random_magic_level, 0x26f34);
        utils::file_works::writeByte(base_file, random_magic, 0x27230);
        utils::file_works::writeByte(base_file, 0x13 - random_magic, 0x27230 + 4);
        utils::file_works::writeByte(base_file, 0x63, 0x27230 + 6);
        utils::file_works::writeByte(base_file, 0xd - random_magic, 0x27230 + 8);
        utils::file_works::writeByte(base_file, 0xf0 + random_magic, 0x26f38);
        utils::file_works::writeByte(base_file, random_magic, 0x26f40);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0xa0c0001d, 0x26f44);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0xa0c20023, 0x26f48);
        // Reset selected magic in magic menu to the randomized magic on every room loading
        utils::file_works::writeFourBytesLittleEndian(base_file, 0xac4053f0, 0x47c10);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0xa6000002, 0x47c14);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0x24030000 + random_magic, 0x47c18);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0xa203f823, 0x47c1c);
        // Reset selected magic in magic menu after using flare or pepper spray
        utils::file_works::writeFourBytesLittleEndian(base_file, 0x14400003, 0xfbfc);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0x24060000 + random_magic, 0xfc04);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0xa086000b, 0xfc08);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0x8082000b, 0xfc0c);
        utils::file_works::writeFourBytesLittleEndian(base_file, 0x90a30000, 0xfc1c);
    } else {
        random_magic = 0;
        random_magic_level = 1;
    }

    // 3. update magic costs
    for (size_t i = 0; i < magic_vector.size(); i++) {
        if (random_magic == magic_vector[i].name && random_magic_level == magic_vector[i].level) {
            utils::file_works::writeTwoBytesLittleEndian(base_file, 0x00, magic_vector[i].cost_address);
            utils::file_works::writeByte(base_file, 0x0, magic_vector[i].mana_bonus_address);
        } else if (config.spell_cost_multiplier != 1.0) {
            utils::file_works::writeTwoBytesLittleEndian(
                base_file, magic_vector[i].default_cost * config.spell_cost_multiplier,
                magic_vector[i].cost_address);
            utils::file_works::writeByte(base_file, magic_vector[i].default_mana_bonus,
                                         magic_vector[i].mana_bonus_address);
        } else if (magic_vector[i].name == PYROKINESIS && magic_vector[i].level == 1) {
            utils::file_works::writeTwoBytesLittleEndian(
                base_file, magic_vector[i].default_cost * config.spell_cost_multiplier,
                magic_vector[i].cost_address);
            utils::file_works::writeByte(base_file, magic_vector[i].default_mana_bonus,
                                         magic_vector[i].mana_bonus_address);
        }
    }
}
