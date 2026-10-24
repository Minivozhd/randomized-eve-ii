#include "domain/randomizer/usecases/Randomizer.hpp"
#include "domain/magic/usecases/NoMagic.hpp"
#include "utils/RuntimePaths.hpp"

// TODO: error handling and move all constants to headers
// TODO: remove all magic numbers (everywhere)
// TODO: add custom structs for descriptors

usecase::Randomizer::Randomizer(std::shared_ptr<PE2Logger> logger)
    : temp_dir(utils::paths::workDirectory()), _logger(logger) {};

void usecase::Randomizer::enhanceEnemiesAndBosses() {
    if (config.flags.enhanced_bosses || config.bosses_damage != 100 || config.bosses_hp != 100) {
        for (auto &&boss_enh_key_val : boss_enhancements) {
            auto boss_enh = boss_enh_key_val.second;

            std::fstream boss_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                       boss_enh.filename + FORMAT_ENEMY_FILE + FORMAT_DECOMPRESSED,
                                   std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
            if (!boss_file) {
                throw std::runtime_error("[CostumeRandomizer] couldn't open level file file for level " +
                                         temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) +
                                         "/" + boss_enh.filename + FORMAT_ENEMY_FILE + FORMAT_DECOMPRESSED +
                                         ".");
            }

            if (config.flags.enhanced_bosses) {
                for (auto &&patch : boss_enh.command_to_change) {
                    utils::file_works::writeFourBytesLittleEndian(boss_file, patch.second, patch.first);
                }
            }

            if (config.bosses_hp != 100) {
                uint16_t read_val;
                boss_file.seekg(boss_enh.hp_offset);
                boss_file.read(reinterpret_cast<char *>(&read_val), sizeof(read_val));
                size_t calculated_val = (size_t)read_val * config.bosses_hp / 100;
                calculated_val = calculated_val > 65535 ? 65535 : calculated_val;
                utils::file_works::writeTwoBytesLittleEndian(boss_file, (uint16_t)calculated_val,
                                                             boss_enh.hp_offset);
            }

            if (config.bosses_damage != 100) {
                for (auto &&damage_offset : boss_enh.damage_offsets) {
                    uint16_t read_val;
                    boss_file.seekg(damage_offset);
                    boss_file.read(reinterpret_cast<char *>(&read_val), sizeof(read_val));
                    size_t calculated_val = (size_t)read_val * config.bosses_damage / 100;
                    calculated_val = calculated_val > 65535 ? 65535 : calculated_val;
                    utils::file_works::writeTwoBytesLittleEndian(boss_file, (uint16_t)calculated_val,
                                                                 damage_offset);
                }
            }
        }
    }
}

void usecase::Randomizer::oneTimeAmmoBoxPickups() {
    std::fstream game_lib_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                   getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                   FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        throw std::runtime_error("[CostumeRandomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    utils::file_works::writeByte(game_lib_file, 0x08, 0x23020);
}

// TODO: move to the other class because this is not randomzier
void usecase::Randomizer::enableMagicElements() {
    std::fstream game_lib_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                   getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                   FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        // logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error opening input file: ", temp_dir +
        // getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
        // getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED);
        // logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[CostumeRandomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    // Item casts enter at 800A3520 through queuedIndex. Skip both ways of
    // opening the ordinary PE wheel (pad and scripted flag). NOPing only the
    // actor load left v0=80110000, treating unrelated resource data as an actor;
    // a wheel cast could then consume Gp_SelItemRec, including an equipped gun.
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x10000089, 0xfaa0);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0xfaa4);
    // Allow cancelling an item cast before release.
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x3ae0c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x3aed0);

    // disables menu p.energy
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x13d04);

    // remove mana cost
    // for (size_t offset = 0x801a0; offset <= 0x803d0; offset += 0x10) {
    //     utils::file_works::writeByte(game_lib_file, 0x00, offset);
    // }

    // update spell names
    utils::file_works::writeVectorsToFile(
        game_lib_file, 0xb36, {{'P', 'y', 'r', 'o', 'k', 'i', 'n', 'e', 's', 'i', 's', '1', ' '}});
    utils::file_works::writeVectorsToFile(
        game_lib_file, 0xb66, {{'P', 'y', 'r', 'o', 'k', 'i', 'n', 'e', 's', 'i', 's', '2', ' '}});
    utils::file_works::writeVectorsToFile(
        game_lib_file, 0xb9e, {{'P', 'y', 'r', 'o', 'k', 'i', 'n', 'e', 's', 'i', 's', '3', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xbda,
                                          {{'C', 'o', 'm', 'b', 'u', 's', 't', 'i', 'o', 'n', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xbfe,
                                          {{'C', 'o', 'm', 'b', 'u', 's', 't', 'i', 'o', 'n', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xc36,
                                          {{'C', 'o', 'm', 'b', 'u', 's', 't', 'i', 'o', 'n', '3', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xc6a,
                                          {{'I', 'n', 'f', 'e', 'r', 'n', 'o', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xc9e,
                                          {{'I', 'n', 'f', 'e', 'r', 'n', 'o', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xcd6,
                                          {{'I', 'n', 'f', 'e', 'r', 'n', 'o', '3', ' '}});

    utils::file_works::writeVectorsToFile(game_lib_file, 0xd0e,
                                          {{'N', 'e', 'c', 'r', 'o', 's', 'i', 's', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xd36,
                                          {{'N', 'e', 'c', 'r', 'o', 's', 'i', 's', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xd72,
                                          {{'N', 'e', 'c', 'r', 'o', 's', 'i', 's', '3', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xdae, {{'P', 'l', 'a', 's', 'm', 'a', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xde6, {{'P', 'l', 'a', 's', 'm', 'a', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xe1e, {{'P', 'l', 'a', 's', 'm', 'a', '3', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xe5a,
                                          {{'A', 'p', 'o', 'b', 'i', 'o', 's', 'i', 's', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xe92,
                                          {{'A', 'p', 'o', 'b', 'i', 'o', 's', 'i', 's', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xece,
                                          {{'A', 'p', 'o', 'b', 'i', 'o', 's', 'i', 's', '3', ' '}});

    utils::file_works::writeVectorsToFile(game_lib_file, 0xf0e,
                                          {{'M', 'e', 't', 'a', 'b', 'o', 'l', 'i', 's', 'm', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xf42,
                                          {{'M', 'e', 't', 'a', 'b', 'o', 'l', 'i', 's', 'm', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xf72,
                                          {{'M', 'e', 't', 'a', 'b', 'o', 'l', 'i', 's', 'm', '3', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xfa6,
                                          {{'H', 'e', 'a', 'l', 'i', 'n', 'g', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xfc6,
                                          {{'H', 'e', 'a', 'l', 'i', 'n', 'g', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0xfe2,
                                          {{'H', 'e', 'a', 'l', 'i', 'n', 'g', '3', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x1002,
                                          {{'L', 'i', 'f', 'e', 'd', 'r', 'a', 'i', 'n', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x1032,
                                          {{'L', 'i', 'f', 'e', 'd', 'r', 'a', 'i', 'n', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x1062,
                                          {{'L', 'i', 'f', 'e', 'd', 'r', 'a', 'i', 'n', '3', ' '}});

    utils::file_works::writeVectorsToFile(game_lib_file, 0x109a,
                                          {{'A', 'n', 't', 'i', 'b', 'o', 'd', 'y', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x10c2,
                                          {{'A', 'n', 't', 'i', 'b', 'o', 'd', 'y', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x10ea,
                                          {{'A', 'n', 't', 'i', 'b', 'o', 'd', 'y', '3', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x111a,
                                          {{'E', 'n', 'e', 'r', 'g', 'y', 's', 'h', 'o', 't', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x115a,
                                          {{'E', 'n', 'e', 'r', 'g', 'y', 's', 'h', 'o', 't', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x1192,
                                          {{'E', 'n', 'e', 'r', 'g', 'y', 's', 'h', 'o', 't', '3', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x11d2,
                                          {{'E', 'n', 'e', 'r', 'g', 'y', 'b', 'a', 'l', 'l', '1', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x120e,
                                          {{'E', 'n', 'e', 'r', 'g', 'y', 'b', 'a', 'l', 'l', '2', ' '}});
    utils::file_works::writeVectorsToFile(game_lib_file, 0x124a,
                                          {{'E', 'n', 'e', 'r', 'g', 'y', 'b', 'a', 'l', 'l', '3', ' '}});

    // update spell names addresses (??? might be problems with that because the addresses are not multiple of
    // 4)
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094336, 0x7a0b4);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094366, 0x7a0bc);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009439e, 0x7a0c4);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800943da, 0x7a0cc);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800943fe, 0x7a0d4);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094436, 0x7a0dc);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009446a, 0x7a0e4);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009449e, 0x7a0ec);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800944d6, 0x7a0f4);

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009450e, 0x7a0fc);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094536, 0x7a104);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094572, 0x7a10c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800945ae, 0x7a114);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800945e6, 0x7a11c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009461e, 0x7a124);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009465a, 0x7a12c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094692, 0x7a134);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800946ce, 0x7a13c);

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009470e, 0x7a144);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094742, 0x7a14c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094772, 0x7a154);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800947a6, 0x7a15c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800947c6, 0x7a164);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800947e2, 0x7a16c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094802, 0x7a174);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094832, 0x7a17c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094862, 0x7a184);

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009489a, 0x7a18c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800948c2, 0x7a194);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800948ea, 0x7a19c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009491a, 0x7a1a4);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8009495a, 0x7a1ac);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094992, 0x7a1b4);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800949d2, 0x7a1bc);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094a0e, 0x7a1c4);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80094a4a, 0x7a1cc);

    // Remove lv.1 label
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x2f21c);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x30c88);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x337f0);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x3a220);

    // Make them available for attachments
    for (size_t offset = 0x7a0b3; offset <= 0x7a1cb; offset += 0x08) {
        utils::file_works::writeByte(game_lib_file, 0x08, offset);
    }

    // Disable magic usage out of combat
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x38cdc);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x3c2b4);

    for (size_t offset = 0x79bdc; offset <= 0x79c68; offset += 0x4) {
        utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, offset);
    }

    // Make them usable from the attachemnts during combat
    for (size_t offset = 0x3b74; offset <= 0x3c00; offset += 0x4) {
        utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800CC1D0, offset);
    }

    for (size_t offset = 0x3a6c; offset <= 0x3af8; offset += 0x4) {
        utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800CC15C, offset);
    }

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x2063FFFF, 0x10148);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x10154);

    utils::file_works::writeVectorsToFile(
        game_lib_file, 0x3895C,
        {{
            '\x03', '\x00', '\x02', '\x24', '\x1A', '\x00', '\x82', '\x02', '\x12', '\x20', '\x00', '\x00',
            '\xFC', '\xFF', '\x84', '\x24', '\x1A', '\x00', '\x82', '\x02', '\x10', '\x18', '\x00', '\x00',
            '\x12', '\x30', '\x00', '\x00', '\x07', '\x80', '\x05', '\x3C', '\xB3', '\x29', '\xA5', '\x24',
            '\x20', '\x28', '\xC5', '\x00', '\x09', '\x00', '\x60', '\x10', '\x00', '\x00', '\x00', '\x00',
            '\x21', '\x20', '\x03', '\x00', '\x01', '\x00', '\x84', '\x20', '\x00', '\x00', '\xA4', '\xA0',
            '\x12', '\x20', '\x00', '\x00', '\x6E', '\x9F', '\x02', '\x0C', '\xFC', '\xFF', '\x84', '\x24',
            '\x25', '\x58', '\x03', '\x08', '\x00', '\x00', '\x00', '\x00', '\xFE', '\xFF', '\x44', '\x24',
            '\x00', '\x00', '\xA4', '\xA0', '\x12', '\x20', '\x00', '\x00', '\x6E', '\x9F', '\x02', '\x0C',
            '\xFC', '\xFF', '\x84', '\x24', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
            '\x25', '\x58', '\x03', '\x08', '\x00', '\x00', '\x00', '\x00',
        }});

    utils::file_works::writeVectorsToFile(
        game_lib_file, 0x389D0,
        {{
            '\x07', '\x80', '\x02', '\x3C', '\xA4', '\x3B', '\x43', '\x84', '\x00', '\x00', '\x00', '\x00',
            '\x03', '\x00', '\x64', '\x28', '\x02', '\x00', '\x80', '\x10', '\x00', '\x00', '\x00', '\x00',
            '\xC4', '\x58', '\x03', '\x08', '\x07', '\x80', '\x02', '\x3C', '\xAD', '\x3B', '\x43', '\x80',
            '\x00', '\x00', '\x00', '\x00', '\x10', '\x00', '\x62', '\x30', '\x02', '\x00', '\x40', '\x14',
            '\x00', '\x00', '\x00', '\x00', '\xC3', '\x58', '\x03', '\x08', '\x00', '\x00', '\x00', '\x00',
            '\xC4', '\x58', '\x03', '\x08', '\x00', '\x00', '\x00', '\x00',
        }});

    // Makes items dissapear after applying
    utils::file_works::writeByte(game_lib_file, 0x00, 0xfec4);

    // Fix flare, flash & spray
    utils::file_works::writeByte(game_lib_file, 0xd6, 0x42890);
    utils::file_works::writeByte(game_lib_file, 0x12, 0x428b0);

    // Makes healing magic disabled when having max hp
    utils::file_works::writeVectorsToFile(
        game_lib_file, 0x38A14,
        {{
            '\x07', '\x80', '\x02', '\x3C', '\xA4', '\x3B', '\x43', '\x84', '\x00', '\x00', '\x00', '\x00',
            '\x03', '\x00', '\x64', '\x28', '\x02', '\x00', '\x80', '\x10', '\x00', '\x00', '\x00', '\x00',
            '\xC4', '\x58', '\x03', '\x08', '\x07', '\x80', '\x02', '\x3C', '\xA0', '\x3B', '\x43', '\x84',
            '\xA2', '\x3B', '\x44', '\x84', '\x00', '\x00', '\x00', '\x00', '\x02', '\x00', '\x83', '\x14',
            '\x00', '\x00', '\x00', '\x00', '\xC4', '\x58', '\x03', '\x08', '\x07', '\x80', '\x02', '\x3C',
            '\xA4', '\x3B', '\x43', '\x84', '\x00', '\x00', '\x00', '\x00', '\x03', '\x00', '\x64', '\x28',
            '\x02', '\x00', '\x80', '\x10', '\x00', '\x00', '\x00', '\x00', '\xC4', '\x58', '\x03', '\x08',
            '\x07', '\x80', '\x02', '\x3C', '\xAD', '\x3B', '\x43', '\x80', '\x00', '\x00', '\x00', '\x00',
            '\x80', '\x00', '\x62', '\x30', '\x02', '\x00', '\x40', '\x10', '\x00', '\x00', '\x00', '\x00',
            '\xC4', '\x58', '\x03', '\x08', '\x07', '\x80', '\x02', '\x3C', '\xAD', '\x3B', '\x43', '\x80',
            '\x00', '\x00', '\x00', '\x00', '\x10', '\x00', '\x62', '\x30', '\x02', '\x00', '\x40', '\x14',
            '\x00', '\x00', '\x00', '\x00', '\xC3', '\x58', '\x03', '\x08', '\xC4', '\x58', '\x03', '\x08',
            '\x00', '\x00', '\x00', '\x00',
        }});

    for (size_t offset = 0x3BBC; offset <= 0x3C00; offset += 4) {
        utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800CC24C, offset);
    }

    for (size_t offset = 0x3BC8; offset <= 0x3BD0; offset += 4) {
        utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x800CC214, offset);
    }

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0xE66C);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x14600005, 0xE670);
}

void usecase::Randomizer::activeTimeBattle() {
    std::fstream game_lib_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                   getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                   FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        throw std::runtime_error("[Randomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    std::fstream slus_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::SLUS) + FORMAT_CDF,
                           std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!slus_file) {
        throw std::runtime_error("[Randomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00005452, 0x9388C - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x0000544C, 0x93890 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24060000, 0xA2224 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24060008, 0xA223C - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x34060004, 0xA2374 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3042001A, 0xA31EC - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3C068011, 0xA33D0 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x90820006, 0xA33D4 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x9083000A, 0xA33D8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x304200EF, 0xA33DC - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x38630001, 0xA33E0 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA0820006, 0xA33E4 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3C028011, 0xA33E8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA083000A, 0xA33EC - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3C078009, 0xA33C4 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24060080, 0xA33C8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0xA33CC - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA0E02EAF, 0xA3400 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x10440067, 0xAD790 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x104400FA, 0xD9E84 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24020008, 0xE9674 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3C038011, 0xE9678 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA4625718, 0xE967C - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA4624D08, 0xE9680 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x90634C12, 0xE9684 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24020001, 0xE9688 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x14620004, 0xE968C - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3C028011, 0xE9690 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x90435718, 0xE9694 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24020008, 0xE9698 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x10620008, 0xE969C - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3C038011, 0xE96A0 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3C028009, 0xE96A4 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x80422E4F, 0xE96A8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24030000, 0xE96AC - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x0062102A, 0xE96B0 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x18400007, 0xE96B4 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA0804C12, 0xE96C8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3C028009, 0xE96E4 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x90432EAF, 0xE96E8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24020000, 0xE96EC - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x10430007, 0xE96F0 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA0804C12, 0xE9704 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA440570A, 0xE970C - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x14E00005, 0x10942C - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x14400003, 0x109434 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x30620005, 0x109438 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3C078011, 0x1093F0 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x90E74C12, 0x1093F8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x3062A005, 0x10973C - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x30628004, 0x109754 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00020301, 0x10FBEC - 0x93800);

    utils::file_works::writeFourBytesLittleEndian(slus_file, 0x3063500F, 0x2C770 - 0xF800);
    utils::file_works::writeFourBytesLittleEndian(slus_file, 0x3042500F, 0x2C774 - 0xF800);
    utils::file_works::writeFourBytesLittleEndian(slus_file, 0x3042500F, 0x2C7BC - 0xF800);
    utils::file_works::writeFourBytesLittleEndian(slus_file, 0x24020009, 0x2C7AC - 0xF800);
    utils::file_works::writeFourBytesLittleEndian(slus_file, 0x2C420010, 0x2C7A4 - 0xF800);

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8c45ed64, 0x000a336c - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x8ca6001c, 0x000a33c4 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x000a33c8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xa0c009cf, 0x000a33cc - 0x93800);
}

void usecase::Randomizer::updateColorScheme() {
    std::fstream slus_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::SLUS) + FORMAT_CDF,
                           std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!slus_file) {
        throw std::runtime_error("[Randomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    std::fstream game_lib_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                   getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                   FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        throw std::runtime_error("[Randomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    // main color
    uint8_t main_color_g = (uint8_t)((size_t)config.hud_color_g * 0xb8 / 255);
    uint8_t main_color_b = (uint8_t)((size_t)config.hud_color_b * 0xdf / 255);

    utils::file_works::writeByte(slus_file, main_color_b, 0x350A4);
    utils::file_works::writeByte(slus_file, config.hud_color_r, 0x35178);
    utils::file_works::writeByte(slus_file, main_color_g, 0x35179);

    utils::file_works::writeByte(game_lib_file, main_color_b, 0x12F68);
    utils::file_works::writeByte(game_lib_file, config.hud_color_r, 0x12FF0);
    utils::file_works::writeByte(game_lib_file, main_color_g, 0x12FF1);

    utils::file_works::writeByte(game_lib_file, main_color_b, 0x13004);
    utils::file_works::writeByte(game_lib_file, config.hud_color_r, 0x1300C);
    utils::file_works::writeByte(game_lib_file, main_color_g, 0x1300D);

    utils::file_works::writeByte(game_lib_file, config.hud_color_b, 0x51EA8);
    utils::file_works::writeByte(game_lib_file, config.hud_color_r, 0x51E7C);
    utils::file_works::writeByte(game_lib_file, config.hud_color_g, 0x51E7D);

    // first gradient
    uint8_t gradient_color_1_b = (uint8_t)(main_color_b / 2);
    uint8_t gradient_color_1_g = (uint8_t)(main_color_g / 2);
    uint8_t gradient_color_1_r = (uint8_t)(config.hud_color_r / 2);

    utils::file_works::writeByte(slus_file, gradient_color_1_b, 0x3517C);
    utils::file_works::writeByte(slus_file, gradient_color_1_r, 0x35180);
    utils::file_works::writeByte(slus_file, gradient_color_1_g, 0x35181);

    // second gradient
    uint8_t gradient_color_2_b = gradient_color_1_b > 0x20 ? gradient_color_1_b - 0x20 : 0;
    uint8_t gradient_color_2_g = gradient_color_1_g > 0x20 ? gradient_color_1_g - 0x20 : 0;
    uint8_t gradient_color_2_r = gradient_color_1_r > 0x20 ? gradient_color_1_r - 0x20 : 0;

    utils::file_works::writeByte(slus_file, gradient_color_2_b, 0x35184);
    utils::file_works::writeByte(slus_file, gradient_color_2_r, 0x35188);
    utils::file_works::writeByte(slus_file, gradient_color_2_g, 0x35189);

    // third gradient
    uint8_t gradient_color_3_b = gradient_color_2_b > 0x20 ? gradient_color_2_b - 0x20 : 0;
    uint8_t gradient_color_3_g = gradient_color_2_g > 0x20 ? gradient_color_2_g - 0x20 : 0;
    uint8_t gradient_color_3_r = gradient_color_2_r > 0x20 ? gradient_color_2_r - 0x20 : 0;

    utils::file_works::writeByte(slus_file, gradient_color_3_b, 0x3518C);
    utils::file_works::writeByte(slus_file, gradient_color_3_r, 0x35190);
    utils::file_works::writeByte(slus_file, gradient_color_3_g, 0x35191);

    // fourth gradient
    uint8_t gradient_color_4_b = (uint8_t)(main_color_b / 3);
    uint8_t gradient_color_4_g = (uint8_t)(main_color_g / 3);
    uint8_t gradient_color_4_r = (uint8_t)(config.hud_color_r / 3);

    utils::file_works::writeByte(game_lib_file, gradient_color_4_b, 0x12FF4);
    utils::file_works::writeByte(game_lib_file, gradient_color_4_r, 0x12FF8);
    utils::file_works::writeByte(game_lib_file, gradient_color_4_g, 0x12FF9);

    utils::file_works::writeByte(game_lib_file, gradient_color_4_b, 0x12FFC);
    utils::file_works::writeByte(game_lib_file, gradient_color_4_r, 0x13000);
    utils::file_works::writeByte(game_lib_file, gradient_color_4_g, 0x13001);

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA4830014, 0x51EB8);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xA483001C, 0x51EBC);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x51EDC);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x51EE0);

    utils::file_works::writeTwoBytesLittleEndian(game_lib_file, 0x3C0A, 0x52264);
}

// TODO: move to the other class because this is not randomzier
void usecase::Randomizer::disableAmmoBoxes(std::string filename, std::vector<size_t> addresses,
                                           bool acropolis = false) {
    (void)acropolis;
    std::fstream outfile(filename, std::ios::in | std::ios::out | std::ios::binary);
    for (size_t addr : addresses) {
        // utils::file_works::writeTwoBytesLittleEndian(outfile, acropolis ? 0x10a0 : 0x0907, addr);
        utils::file_works::writeFourBytesLittleEndian(outfile, 0x00060000, addr + 0x2);
    }
}

void usecase::Randomizer::applyMods() {
    switch (config.ammo_boxes_option) {
    case 1: // No ammo boxes
        disableAmmoBoxes(temp_dir + getStageFileName(domain::file::LEVEL_G) + "/01_akropolis.pe2lvl.decompr",
                         {0x24ce}, true);
        disableAmmoBoxes(temp_dir + getStageFileName(domain::file::LEVEL_G) + "/00_base.pe2lvl.decompr",
                         {0x80daa, /*0x8105a*/});
        disableAmmoBoxes(temp_dir + getStageFileName(domain::file::LEVEL_G) +
                             "/04_mines_and_shelter.pe2lvl.decompr",
                         {/*0x1d8a, */ 0x1dda, 0x1e5a, 0x1eba, 0x1eca, 0x1eda, 0x1fca, 0x1fda, 0x1fea, 0x1ffa,
                          0x200a, 0x20da, 0x212a, 0x213a});
        disableAmmoBoxes(temp_dir + getStageFileName(domain::file::LEVEL_G) +
                             "/05_neo_ark_and_shelter.pe2lvl.decompr",
                         {0x2f02, 0x2f12, 0x2f22, 0x3042});
        break;
    case 2: // One time ammo boxes
        oneTimeAmmoBoxPickups();
        break;
    }
    if (config.flags.enable_map_in_fight) {
        std::string filename = temp_dir + getStageFileName(domain::file::LEVEL_G) + "/00_base.pe2lvl.decompr";
        std::fstream outfile(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!outfile.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }
        utils::file_works::writeByte(outfile, 0x43, 0x106f8);
    }
    if (config.flags.enable_special_modes) {
        std::fstream outfile(temp_dir + "STAGE1/19_shooting_gallery.pe2rml.decompr",
                             std::ios::in | std::ios::out | std::ios::binary);

        utils::file_works::writeFourBytesLittleEndian(outfile, 0x00000000, 0x15ac);
        utils::file_works::writeFourBytesLittleEndian(outfile, 0x00000000, 0x2624);
    }
    if (!config.no_magic && config.flags.magic_pickup_mode) {
        enableMagicElements();
    }
    if (config.flags.enhanced_bosses || config.bosses_damage != 100 || config.bosses_hp) {
        enhanceEnemiesAndBosses();
    }
    if (!config.no_magic && config.flags.active_time_battle && !config.flags.magic_pickup_mode) {
        activeTimeBattle();
    }
    if (config.flags.enable_hud_coloring) {
        updateColorScheme();
    }

    if (config.flags.increased_inv) {
        increaseIFrames();
    }

    if (config.flags.no_white_screen) {
        skipWhiteScreen();
    }

    if (config.flags.no_win_pose) {
        skipWinPose();
    }

    if (config.flags.no_turn) {
        disableTurning();
    }

    if (config.flags.inventory_mode) {
        enableInventoryDuringBattles();
    }
    if (config.no_magic) {
        std::fstream gameplay(temp_dir + "STAGE0/00_base.pe2lvl.decompr",
                              std::ios::in | std::ios::out | std::ios::binary);
        if (!gameplay)
            throw std::runtime_error("Cannot open gameplay for No magic");
        domain::magic::no_magic::apply(gameplay);
    }
}

void usecase::Randomizer::increaseIFrames() {
    std::fstream game_lib_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                   getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                   FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        throw std::runtime_error("[Randomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    utils::file_works::writeVectorsToFile(
        game_lib_file, 0x1b584,
        {{
            '\x06', '\x80', '\x08', '\x3C', '\x64', '\xED', '\x08', '\x8D', '\x20', '\x00', '\x09', '\x24',
            '\x18', '\x00', '\x08', '\x8D', '\x00', '\x00', '\x00', '\x00', '\xAA', '\xF9', '\x09', '\xA1',
            '\x00', '\x00', '\x00', '\x00', '\xDE', '\x4C', '\x40', '\xA0', '\xDD', '\x4C', '\x40', '\xA0',
            '\xD2', '\x4C', '\x40', '\xA4', '\xD0', '\x4C', '\x40', '\xA4', '\xDC', '\x4C', '\x40', '\xA0',
            '\xD8', '\x4C', '\x40', '\xA0', '\xD9', '\x4C', '\x40', '\xA0', '\xD6', '\x4C', '\x40', '\xA4',
        }});

    utils::file_works::writeVectorsToFile(
        game_lib_file, 0x1aad4,
        {{
            '\x06', '\x80', '\x08', '\x3C', '\x64', '\xED', '\x08', '\x8D', '\x20', '\x00', '\x09', '\x24',
            '\x18', '\x00', '\x08', '\x8D', '\x00', '\x00', '\x00', '\x00', '\xAA', '\xF9', '\x09', '\xA1',
            '\xF8', '\x4C', '\x40', '\xA0', '\xD9', '\x4C', '\x40', '\xA0', '\xD8', '\x4C', '\x40', '\xA0',
            '\xD2', '\x4C', '\x40', '\xA4', '\xDB', '\x4C', '\x40', '\xA0', '\xDA', '\x4C', '\x40', '\xA0',
        }});
}

void usecase::Randomizer::skipWhiteScreen() {
    std::fstream game_lib_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                   getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                   FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        throw std::runtime_error("[Randomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x10ccc);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x10790);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x10798);
}

void usecase::Randomizer::skipWinPose() {
    std::fstream game_lib_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                   getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                   FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        throw std::runtime_error("[Randomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x75110);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x74F38);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x750b8);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x75134);
}

void usecase::Randomizer::disableTurning() {
    std::fstream game_lib_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                   getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                   FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        throw std::runtime_error("[Randomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x47598);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x475B0);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x00000000, 0x707F4);
}

void usecase::Randomizer::enableInventoryDuringBattles() {
    std::fstream game_lib_file(temp_dir + getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                   getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                   FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        throw std::runtime_error("[Randomizer] couldn't open level file file for level " + temp_dir +
                                 getNameFromStage(domain::game::pe2_stage_name_t::STAGE0) + "/" +
                                 getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    utils::file_works::writeByte(game_lib_file, 0x42, 0x000a3ef8 - 0x93800);
    utils::file_works::writeByte(game_lib_file, 0x41, 0x000a3e90 - 0x93800);
}

std::vector<std::shared_ptr<usecase::IFeatureRandomizer>> usecase::Randomizer::getRandomizers() {
    std::vector<std::shared_ptr<usecase::IFeatureRandomizer>> feature_randomizers{};

    if (config.pe1_music || config.flags.randomize_music) {
        feature_randomizers.push_back(
            std::make_shared<usecase::music::MusicRandomizer>(_logger, gen, temp_dir, config));
    }
    if (config.flags.randomize_enemies) {
        feature_randomizers.push_back(
            std::make_shared<usecase::enemies::EnemyRandomizer>(_logger, gen, temp_dir, config));
    }
    if (config.flags.randomize_items || config.flags.randomize_shops) {
        feature_randomizers.push_back(
            std::make_shared<usecase::items::ItemRandomizer>(_logger, gen, temp_dir, config));
    }
    if (config.player_model_option) {
        feature_randomizers.push_back(
            std::make_shared<usecase::costumes::CostumeRandomizer>(_logger, gen, temp_dir, config));
    }
    if (config.flags.additional_armors) {
        feature_randomizers.push_back(
            std::make_shared<usecase::armors::ArmorRandomizer>(_logger, gen, temp_dir, config));
    }
    if (!config.no_magic && config.flags.randomize_magic) {
        feature_randomizers.push_back(
            std::make_shared<usecase::magic::MagicRandomizer>(_logger, gen, temp_dir, config));
    }

    return feature_randomizers;
}

void usecase::Randomizer::randomize(domain::inner_config_t _config) {
    // TODO: adaptive file lists
    _logger->log("[INF] [Randomizer] Entering randomize function()...");
    config = _config;
    if (config.no_magic) {
        config.flags.randomize_magic = false;
        config.flags.magic_pickup_mode = false;
        config.flags.active_time_battle = false;
    }

    // set fixed random seed
    gen = std::mt19937(config.randomize_seed);
    // setting up context
    pe2_randomizer_context context{
        .reward_slots{},
        .selected_weapon = 0x83,
    };

    _logger->log("[INF] [Randomizer] Setting random seed to ", config.randomize_seed);

    auto feature_randomizers = getRandomizers();
    for (auto &feature_randomizer : feature_randomizers) {
        context = feature_randomizer->randomize(context);
    }

    applyMods();
}