#include "domain/costume/usecases/CostumeRandomizer.hpp"
#include "domain/costume/usecases/MalePlayers.hpp"
#include "domain/costume/usecases/Portraits.hpp"
#include "domain/costume/usecases/TrainingMode.hpp"
#include "resources/EmbeddedResources.hpp"

using namespace domain::costume;
using namespace domain::game;
using namespace domain::file;

pe2_randomizer_context usecase::costumes::CostumeRandomizer::randomize(pe2_randomizer_context context) {
    pe2_costume_t costume;
    switch (config.player_model_option) {
    case 1: {
        std::uniform_int_distribution<size_t> dist(0, pe2_costumes_map.size() - 1);
        size_t randomIndex = dist(gen);
        auto it = pe2_costumes_map.begin();
        std::advance(it, randomIndex);
        costume = it->second;
    } break;
    case 2:
        costume = pe2_costumes_map[PE2_COSTUME_SHOULDER_HOLSTER_WITH_EAR_PROTECTION];
        break;
    case 3:
        costume = pe2_costumes_map[PE2_COSTUME_SHOULDER_HOLSTER];
        break;
    case 4:
        costume = pe2_costumes_map[PE2_COSTUME_LEATHER_JACKET];
        break;
    case 5:
        costume = pe2_costumes_map[PE2_COSTUME_ASSAULT_SUIT];
        break;
    case 6:
        costume = pe2_costumes_map[PE2_COSTUME_DC2_REGINA_DEFAULT];
        break;
    case 7:
        costume = pe2_costumes_map[PE2_COSTUME_DC2_REGINA_LIGHTWEIGHT_ARMOR];
        break;
    case 8:
        costume = pe2_costumes_map[PE2_COSTUME_JUDIE_MIST_GARAGE];
        break;
    case 9:
        costume = pe2_costumes_map[PE2_COSTUME_JUDIE_MILITARY_CAMP];
        break;
    case 10:
        costume = pe2_costumes_map[PE2_COSTUME_CAFETERIA_WOMAN];
        break;
    case 11:
        costume = pe2_costumes_map[PE2_COSTUME_TOILET_WOMAN];
        break;
    case 12:
        costume = pe2_costumes_map[PE2_COSTUME_RE1_JILL];
        break;
    case 13:
        costume = pe2_costumes_map.at(PE2_COSTUME_KYLE);
        break;
    case 14:
        costume = pe2_costumes_map.at(PE2_COSTUME_RUPERT);
        break;
    case 15:
        costume = pe2_costumes_map.at(PE2_COSTUME_DOUGLAS);
        break;
    case 16:
        costume = pe2_costumes_map.at(PE2_COSTUME_POLICE);
        break;
    case 17:
        costume = pe2_costumes_map.at(PE2_COSTUME_GENERAL);
        break;
    case 18:
        costume = pe2_costumes_map.at(PE2_COSTUME_PRESIDENT);
        break;
    case 19:
        costume = pe2_costumes_map.at(PE2_COSTUME_BALDWIN);
        break;
    case 20:
        costume = pe2_costumes_map.at(PE2_COSTUME_SOLDIER);
        break;
    case 21:
        costume = pe2_costumes_map.at(PE2_COSTUME_PIERCE);
        break;
    default:
        return context;
    }

    portraits::apply(costume.id, temp_dir_name);
    logger->log("[INF] [Player model] menu portrait=", portraits::assetKey(costume.id));
    male::apply(costume.id, temp_dir_name);
    chooseCostumeSlot(costume, context);
    if (costume.custom) {
        substituteCostumeModel(costume);
    }

    return context;
}

void usecase::costumes::CostumeRandomizer::chooseCostumeSlot(pe2_costume_t &costume,
                                                             pe2_randomizer_context context) {
    (void)context;
    std::fstream slus_file(temp_dir_name + getNameFromStage(SLUS) + FORMAT_CDF,
                           std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!slus_file) {
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(SLUS) + FORMAT_CDF);
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[CostumeRandomizer] couldn't open level file file for level " +
                                 temp_dir_name + getNameFromStage(SLUS) + FORMAT_CDF + ".");
    }

    std::fstream game_lib_file(temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                                   FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED,
                               std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!game_lib_file) {
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE0) + "/" + getNameFromLevel(BASE_LEVEL) +
                        FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED);
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[CostumeRandomizer] couldn't open level file file for level " +
                                 temp_dir_name + getNameFromStage(STAGE0) + "/" +
                                 getNameFromLevel(BASE_LEVEL) + FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED +
                                 ".");
    }

    std::fstream shooting_range_room_file(temp_dir_name + getNameFromStage(STAGE1) + "/" +
                                              getNameFromRoom(ROOM_AKROPOLIS_SHOOTING_GALLERY) +
                                              FORMAT_ROOM_FILE + FORMAT_DECOMPRESSED,
                                          std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!shooting_range_room_file) {
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE1) + "/" +
                        getNameFromRoom(ROOM_AKROPOLIS_SHOOTING_GALLERY) + FORMAT_ROOM_FILE +
                        FORMAT_DECOMPRESSED);
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[CostumeRandomizer] couldn't open level file file for level " +
                                 temp_dir_name + temp_dir_name + getNameFromStage(STAGE1) + "/" +
                                 getNameFromRoom(ROOM_AKROPOLIS_SHOOTING_GALLERY) + FORMAT_ROOM_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    std::fstream mist_garage_room_file(temp_dir_name + getNameFromStage(STAGE1) + "/" +
                                           getNameFromRoom(ROOM_AKROPOLIS_MIST_PARKING) + FORMAT_ROOM_FILE +
                                           FORMAT_DECOMPRESSED,
                                       std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!mist_garage_room_file) {
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE1) + "/" +
                        getNameFromRoom(ROOM_AKROPOLIS_MIST_PARKING) + FORMAT_ROOM_FILE +
                        FORMAT_DECOMPRESSED);
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[CostumeRandomizer] couldn't open level file file for level " +
                                 temp_dir_name + temp_dir_name + getNameFromStage(STAGE1) + "/" +
                                 getNameFromRoom(ROOM_AKROPOLIS_MIST_PARKING) + FORMAT_ROOM_FILE +
                                 FORMAT_DECOMPRESSED + ".");
    }

    std::fstream pierce_file(temp_dir_name + getNameFromStage(STAGE0) + "/enemies/00_pierce" +
                                 FORMAT_ENEMY_FILE + FORMAT_DECOMPRESSED,
                             std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!pierce_file) {
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE0) + "/enemies/00_pierce" + FORMAT_ENEMY_FILE +
                        FORMAT_DECOMPRESSED);
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[CostumeRandomizer] couldn't open level file file for level " +
                                 temp_dir_name + getNameFromStage(STAGE0) + "/enemies/00_pierce" +
                                 FORMAT_ENEMY_FILE + FORMAT_DECOMPRESSED + ".");
    }

    // Training behavior must follow the room's mode, not the selected model.
    domain::costume::training::apply(game_lib_file);
    utils::file_works::writeByte(shooting_range_room_file, (uint8_t)costume.slot,
                                 0x29d4); // shooting range not changing costume
    utils::file_works::writeByte(shooting_range_room_file, 0x05,
                                 0x2a18); // shooting range not changing costume
    utils::file_works::writeFourBytesLittleEndian(shooting_range_room_file, 0xa08215de,
                                                  0x2a1c); // shooting range not changing costume
    utils::file_works::writeByte(slus_file, (uint8_t)costume.slot, 0x3ccf4);     // Start game costume
    utils::file_works::writeByte(game_lib_file, (uint8_t)costume.slot, 0x28cc4); // Akropolis approach costume
    utils::file_works::writeFourBytesLittleEndian(mist_garage_room_file, 0xa04215de,
                                                  0x6f84); // MIST garage disable function
    utils::file_works::writeByte(game_lib_file, (uint8_t)costume.slot, 0x246cc); // Baldwin's Cabinet costume
    utils::file_works::writeByte(game_lib_file, 0x05, 0x28cec);                  // Dryfield costume
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0xa06215de, 0x28cf8); // Update animations
    utils::file_works::writeByte(pierce_file, (uint8_t)costume.slot, 0x90c);
}

void usecase::costumes::CostumeRandomizer::substituteCostumeModel(pe2_costume_t &costume) {
    std::fstream output(temp_dir_name + getNameFromStage(STAGE0) + FORMAT_CDF,
                        std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
    if (!output) {
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error opening input file: ",
                    temp_dir_name + getNameFromStage(STAGE0) + FORMAT_CDF);
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[CostumeRandomizer] couldn't open level file file for level " +
                                 temp_dir_name + getNameFromStage(STAGE0) + FORMAT_CDF + ".");
    }

    auto input = pe2::resources::stream(pe2::resources::get("binary/data.bin"));

    input.seekg(costume.data_file_offset, std::ios::beg);
    std::vector<char> buffer(costume.data_file_size);
    if (!input.read(buffer.data(), costume.data_file_size)) {
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error reading input file at offset: ",
                    costume.data_file_offset);
        throw std::runtime_error("[CostumeRandomizer] couldn't read from data file at offset " +
                                 std::to_string(costume.data_file_offset));
    }

    output.seekp(costume.stage0_offset, std::ios::beg);
    if (!output.write(buffer.data(), costume.data_file_size)) {
        logger->log("[ERR] [Randomizer] [CostumeRandomizer] Error writing to output file at offset: ",
                    costume.stage0_offset);
        throw std::runtime_error("[CostumeRandomizer] couldn't write to level file at offset " +
                                 std::to_string(costume.stage0_offset));
    }
}
