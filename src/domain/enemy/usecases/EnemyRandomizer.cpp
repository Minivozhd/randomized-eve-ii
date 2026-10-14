#include "domain/enemy/usecases/EnemyRandomizer.hpp"
#include "domain/enemy/entities/DummyResources.hpp"
#include "domain/enemy/entities/EnemyCounts.hpp"
#include "domain/enemy/entities/GolemSounds.hpp"
#include "resources/EmbeddedResources.hpp"

pe2_randomizer_context usecase::enemies::EnemyRandomizer::randomize(pe2_randomizer_context context) {
    std::filesystem::remove(temp_dir_name + domain::game::getNameFromStage(domain::game::STAGE0) +
                            "/enemies/dummy_monster.pe2e.decompr");
    pe2::resources::write("binary/dummy_monster.pe2e.decompr",
                          temp_dir_name + domain::game::getNameFromStage(domain::game::STAGE0) +
                              "/enemies/dummy_monster.pe2e.decompr");
    pe2::resources::write("binary/golem_sound_data.pe2e",
                          temp_dir_name + domain::game::getNameFromStage(domain::game::STAGE0) +
                              "/enemies/golem_sound_data.pe2e");
    std::fstream dummy_monster_file(temp_dir_name + domain::game::getNameFromStage(domain::game::STAGE0) +
                                        "/enemies/dummy_monster.pe2e.decompr",
                                    std::ios::in | std::ios::out | std::ios::binary);
    std::ofstream slender_additional_data_file(temp_dir_name +
                                                   domain::game::getNameFromStage(domain::game::STAGE0) +
                                                   "/enemies/slender_additional_data.pe2e",
                                               std::ios::out | std::ios::trunc | std::ios::binary);
    std::ofstream purple_golem_additional_data_file(temp_dir_name +
                                                        domain::game::getNameFromStage(domain::game::STAGE0) +
                                                        "/enemies/knight_golem_additional_data.pe2e",
                                                    std::ios::out | std::ios::trunc | std::ios::binary);
    slender_additional_data_file.close();
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, (uint32_t)0x0,
                                                  current_slender_walking_route_addresses_offset);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, (uint32_t)0x0,
                                                  current_slender_walking_route_addresses_offset + 4);
    current_slender_walking_route_addresses_offset += 8;
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, (uint32_t)0x0,
                                                  current_purple_golem_spawn_zone_addresses_offset);
    current_purple_golem_spawn_zone_addresses_offset += 4;
    dummy_monster_file.close();

    soundCatalog.loadEnemies(temp_dir_name + "STAGE0.CDF");
    updateEnemiesStats();

    randomizeEnemiesInStage(
        context, domain::game::STAGE1, domain::game::AKROPOLIS_LEVEL,
        domain::encounter::encounters_per_level_map[domain::game::AKROPOLIS_LEVEL],
        domain::encounter::encounters_segment_per_level_map[domain::game::AKROPOLIS_LEVEL]);
    randomizeEnemiesInStage(
        context, domain::game::STAGE2, domain::game::DRYFIELD_DAY_LEVEL,
        domain::encounter::encounters_per_level_map[domain::game::DRYFIELD_DAY_LEVEL],
        domain::encounter::encounters_segment_per_level_map[domain::game::DRYFIELD_DAY_LEVEL]);
    randomizeEnemiesInStage(
        context, domain::game::STAGE3, domain::game::DRYFIELD_NIGHT_LEVEL,
        domain::encounter::encounters_per_level_map[domain::game::DRYFIELD_NIGHT_LEVEL],
        domain::encounter::encounters_segment_per_level_map[domain::game::DRYFIELD_NIGHT_LEVEL]);
    randomizeEnemiesInStage(
        context, domain::game::STAGE4, domain::game::MINES_AND_SHELTER_LEVEL,
        domain::encounter::encounters_per_level_map[domain::game::MINES_AND_SHELTER_LEVEL],
        domain::encounter::encounters_segment_per_level_map[domain::game::MINES_AND_SHELTER_LEVEL]);
    randomizeEnemiesInStage(
        context, domain::game::STAGE5, domain::game::NEOARK_AND_SHELTER_LEVEL,
        domain::encounter::encounters_per_level_map[domain::game::NEOARK_AND_SHELTER_LEVEL],
        domain::encounter::encounters_segment_per_level_map[domain::game::NEOARK_AND_SHELTER_LEVEL]);
    updateSpecialEnemiesCode();

    if (config.flags.cloak_golem_mult) {
        allowOneSlotGolems();
    }
    domain::enemy::finalizeDummyResources(temp_dir_name + "STAGE0/enemies/dummy_monster.pe2e.decompr");
    if (config.flags.cloak_golem_mult)
        domain::enemy::validateCloakResources(std::filesystem::path(temp_dir_name) / "STAGE0/enemies");
    return context;
}

void usecase::enemies::EnemyRandomizer::updateConsequentFileOffset(
    const std::vector<domain::file::pe2file_t> &files, std::fstream &level_file,
    domain::encounter::pe2_room_encounters_t &room, domain::game::pe2_stage_name_t stage, size_t offset) {
    for (const auto &f : files) {
        if (f.filename != getNameFromRoom(room.name))
            continue;
        std::fstream file(temp_dir_name + getNameFromStage(stage) + FORMAT_CDF,
                          std::ios::binary | std::ios::in | std::ios::out);
        if (!file)
            throw std::runtime_error("Cannot open room archive for relocation");
        auto readWord = [](const std::array<uint8_t, 16> &h, size_t i) {
            return uint32_t(h[i]) | (uint32_t(h[i + 1]) << 8) | (uint32_t(h[i + 2]) << 16) |
                   (uint32_t(h[i + 3]) << 24);
        };
        file.seekg(0, std::ios::end);
        const auto archive_size = static_cast<size_t>(file.tellg());
        size_t position = f.file_offset;
        bool first = true;
        while (true) {
            std::array<uint8_t, 16> header{};
            file.seekg(position);
            if (!file.read(reinterpret_cast<char *>(header.data()), header.size()))
                throw std::runtime_error("Truncated room archive header");
            const size_t sectors = readWord(header, 4);
            if (!sectors || sectors > (archive_size - position) / 0x800)
                throw std::runtime_error("Invalid room archive sector count");
            // +4 is the ON-DISC sector count, not the decompressed size.
            // Never enlarge it unless the following disk chunks are moved.
            // Relocate RAM destinations, including the fixed CAP directory.
            if (!first) {
                const uint32_t address = readWord(header, 8);
                if (address >= ROOM_INFO_FILE_LOAD_ADDRESS && address < 0x80200000)
                    utils::file_works::writeFourBytesLittleEndian(file, address + offset, position + 8);
            }
            if (!first && header[0] == 4) {
                // A CAP resource bundle has exactly 50 directory entries in
                // its first sector. Both RAM destination fields relocate.
                for (size_t i = 0; i < 50; ++i) {
                    std::array<uint8_t, 16> entry{};
                    const auto entry_pos = position + 16 + i * 16;
                    file.seekg(entry_pos);
                    if (!file.read(reinterpret_cast<char *>(entry.data()), entry.size()))
                        throw std::runtime_error("Truncated resource bundle directory");
                    for (size_t field : {8u, 12u}) {
                        const auto address = readWord(entry, field);
                        if (address >= ROOM_INFO_FILE_LOAD_ADDRESS && address < 0x80200000)
                            utils::file_works::writeFourBytesLittleEndian(file, address + offset,
                                                                          entry_pos + field);
                    }
                }
            }
            if (header[1] == 0xff)
                break;
            position += sectors * 0x800;
            first = false;
        }
        break;
    }

    if (room.additional_file_end_pointer != 0x0) {
        level_file.seekg(room.additional_file_end_pointer);

        std::vector<char> buffer(4);
        level_file.read(buffer.data(), 4);
        uint32_t num = *reinterpret_cast<int32_t *>(&buffer[0]);
        num += offset;
        std::memcpy(&buffer[0], &num, sizeof(num));
        level_file.seekp(room.additional_file_end_pointer);
        level_file.write(reinterpret_cast<char *>(buffer.data()), 4);
    }
}

void usecase::enemies::EnemyRandomizer::updateSpecialEnemiesCode() {

    // 1. Update Slenderman code
    std::fstream outfile_slender_slot_1(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                            "/enemies/03_slender_slot_1.pe2enm.decompr",
                                        std::ios::in | std::ios::out | std::ios::binary);
    std::fstream outfile_slender_slot_2(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                            "/enemies/03_slender_slot_2.pe2enm.decompr",
                                        std::ios::in | std::ios::out | std::ios::binary);

    // 1.1. Add activation descriptors
    outfile_slender_slot_1.seekp(0x163b8);
    outfile_slender_slot_2.seekp(0x163b8);
    std::fstream infile(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                            "/enemies/slender_additional_data.pe2e",
                        std::ios::in | std::ios::out | std::ios::binary);
    const int CHUNK_SIZE = 16;
    std::vector<char> buffer(CHUNK_SIZE);
    while (!infile.eof()) {
        infile.read(reinterpret_cast<char *>(buffer.data()), CHUNK_SIZE);
        outfile_slender_slot_1.write(reinterpret_cast<char *>(buffer.data()), CHUNK_SIZE);
        outfile_slender_slot_2.write(reinterpret_cast<char *>(buffer.data()), CHUNK_SIZE);
    }
    utils::file_works::writeFourBytesLittleEndian(outfile_slender_slot_1, 0x00000000, 0);
    utils::file_works::writeFourBytesLittleEndian(outfile_slender_slot_2, 0x00000000, 0);
    utils::file_works::writeFourBytesLittleEndian(outfile_slender_slot_1, 0x00000000, 0);
    utils::file_works::writeFourBytesLittleEndian(outfile_slender_slot_2, 0x00000000, 0);
    infile.close();

    // 1.2. Update slenderman code to follow activation descriptors in the end of file
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_1, 0x8015, 0x9c0);
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_1, 0x81d8, 0x9e4);
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_1, 0x81d8, 0xa00);
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_2, 0x8016, 0x9c0);
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_2, 0x01d8, 0x9e4);
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_2, 0x01d8, 0xa00);

    // 1.3. Enable Slenderman music
    std::fstream outfile_level(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                   "/01_akropolis.pe2lvl.decompr",
                               std::ios::in | std::ios::out | std::ios::binary);
    utils::file_works::writeFourBytesLittleEndian(outfile_level, 0x02460245, 0x29c8);
    utils::file_works::writeTwoBytesLittleEndian(outfile_level, 0x0000, 0x29cc);

    // 1.4. Set Slenderman walking route points
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_1, 0x8016, 0x9f8);
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_1, 0x7360, 0x9fc);
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_2, 0x8016, 0x9f8);
    utils::file_works::writeTwoBytesLittleEndian(outfile_slender_slot_2, 0x7360, 0x9fc);

    // 2. Knight Golem

    // 2.1. Add activation descriptors
    std::fstream outfile2(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                              "/enemies/16_knight_golem_slot_1.pe2enm.decompr",
                          std::ios::in | std::ios::out | std::ios::binary);
    utils::file_works::writeTwoBytesLittleEndian(outfile2, 0x41ec, 0x57d8);
    outfile2.seekp(0x223cc);
    std::fstream infile2(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                             "/enemies/knight_golem_additional_data.pe2e",
                         std::ios::in | std::ios::out | std::ios::binary);
    while (!infile2.eof()) {
        infile2.read(reinterpret_cast<char *>(buffer.data()), CHUNK_SIZE);
        outfile2.write(reinterpret_cast<char *>(buffer.data()), CHUNK_SIZE);
    }
    utils::file_works::writeFourBytesLittleEndian(outfile2, 0x00000000, 0);
    utils::file_works::writeFourBytesLittleEndian(outfile2, 0x00000000, 0);
    infile2.close();

    // 2.2. Add Knight golems spawn areas
    utils::file_works::writeTwoBytesLittleEndian(outfile2, 0x8017, 0x57d0);
    utils::file_works::writeTwoBytesLittleEndian(outfile2, 0x8600, 0x57d4);

    outfile2.close();

    // 3. Bishop Golem

    // 3.1. Add activation descriptors
    std::fstream outfile3(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                              "/enemies/27_bishop_golem_slot_1.pe2enm.decompr",
                          std::ios::in | std::ios::out | std::ios::binary);
    utils::file_works::writeTwoBytesLittleEndian(outfile3, 0x4144, 0x57d8);
    outfile3.seekp(0x22324);
    std::fstream infile3(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                             "/enemies/knight_golem_additional_data.pe2e",
                         std::ios::in | std::ios::out | std::ios::binary);
    while (!infile3.eof()) {
        infile3.read(reinterpret_cast<char *>(buffer.data()), CHUNK_SIZE);
        outfile3.write(reinterpret_cast<char *>(buffer.data()), CHUNK_SIZE);
    }
    utils::file_works::writeFourBytesLittleEndian(outfile3, 0x00000000, 0);
    utils::file_works::writeFourBytesLittleEndian(outfile3, 0x00000000, 0);

    // 3.2. Add Bishop golems spawn areas
    utils::file_works::writeTwoBytesLittleEndian(outfile3, 0x8017, 0x57d0);
    utils::file_works::writeTwoBytesLittleEndian(outfile3, 0x8600, 0x57d4);

    outfile3.close();

    // 4. Diver
    std::fstream outfile4(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                              "/enemies/04_diver.pe2enm.decompr",
                          std::ios::in | std::ios::out | std::ios::binary);

    // Exact bounded write; the old EOF loop repeated two bytes after the table.
    domain::enemy::audio::patchGolemSounds(temp_dir_name + "STAGE0/enemies");

    std::string filename = temp_dir_name + getNameFromStage(domain::game::STAGE0) + "/00_base.pe2lvl.decompr";
    std::fstream outfile5(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!outfile5.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    // Golem effects
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x80162730, 0x7e314);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x80161e20, 0x7e320);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x801631b0, 0x7e32C);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x3c030006, 0x56550);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x34630293, 0x56554);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0xac435758, 0x56558);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x20630001, 0x5655c);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0xac43572c, 0x56560);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x20630001, 0x56564);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0xac405730, 0x56568);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0xac435750, 0x5656c);
    // Slender effects
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x80163e20, 0x7e248);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x80164b68, 0x7e254);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x80164f00, 0x7e260);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x2063ffed, 0x5657c);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0xac435728, 0x56580);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x20630001, 0x56584);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0xac435744, 0x56588);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0x20630001, 0x5658c);
    utils::file_works::writeFourBytesLittleEndian(outfile5, 0xac43573c, 0x56590);
}

size_t usecase::enemies::EnemyRandomizer::calculatePoolSize(
    std::string &level_filename, std::vector<domain::encounter::pe2_room_encounters_t> &room_encounters,
    domain::encounter::pe2_encounters_list_segment_t segment) {
    size_t enemy_pool_size = (segment.to - segment.from) / ENEMY_RECORD_DESCRIPTOR_SIZE;
    logger->log("[INF] [Randomizer] [EnemyRandomizer] Enemy pool size : ", enemy_pool_size);

    std::ifstream infile(level_filename, std::ios::binary);

    if (!infile.is_open()) {
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Couldn't open file while calculating Pool Size : ",
                    level_filename);
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[EnemyRandomizer] couldn't open file while calculating Pool Size.");
    }

    for (size_t i = 0; i < room_encounters.size(); i++) {
        size_t offset = room_encounters[i].encounters_offset;

        std::vector<char> buffer(ENEMY_RECORD_DESCRIPTOR_SIZE);
        for (size_t j = 0; j < room_encounters[i].encounters.size(); j++) {
            int enemies = 0;

            if (room_encounters[i].encounters[j].id >= 0x07 && room_encounters[i].encounters[j].id < 0x0a) {
                continue;
            }

            if (room_encounters[i].encounters[j].mask != domain::encounter::UNRANDOMIZED) {
                if (room_encounters[i].encounters[j].mask ==
                        domain::encounter::FORBID_MULTI_ENEMY_ENC_AND_SLOT_3 ||
                    room_encounters[i].encounters[j].mask == domain::encounter::FORBID_MULTI_ENEMY_ENC) {
                    if (enemy_pool_size >= 3) {
                        enemy_pool_size -= 3;
                    } else {
                        enemy_pool_size = 0;
                    }
                } else {
                    if (enemy_pool_size >= 4) {
                        enemy_pool_size -= 4;
                    } else {
                        enemy_pool_size = 0;
                    }
                }
                logger->log(
                    "[INF] [Randomizer] [EnemyRandomizer] Enemy pool size domain::encounter::RANDOMIZED (",
                    getNameFromRoom(room_encounters[i].name), ") #",
                    (size_t)room_encounters[i].encounters[j].id, " : ", enemy_pool_size);
                continue;
            }

            while (true) {
                infile.seekg((size_t)offset, std::ios::beg);
                if (!infile) {
                    logger->log("[ERR] [Randomizer] [EnemyRandomizer] Couldn't go to offset : ", offset,
                                " for the file : ", level_filename);
                }
                infile.read(buffer.data(), (size_t)ENEMY_RECORD_DESCRIPTOR_SIZE);
                std::streamsize bytes_read = infile.gcount();
                if (!infile) {
                    if (infile.eof()) {
                        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Couldn't read ",
                                    (size_t)ENEMY_RECORD_DESCRIPTOR_SIZE, "bytes at offset ", offset,
                                    " from the file : ", level_filename,
                                    " : eof! actually read: ", bytes_read);
                    } else {
                        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Couldn't read ",
                                    (size_t)ENEMY_RECORD_DESCRIPTOR_SIZE, "bytes at offset ", offset,
                                    " from the file : ", level_filename, " : read error occured!");
                    }
                }
                offset += ENEMY_RECORD_DESCRIPTOR_SIZE;
                enemies++;
                logger->log(
                    "[INF] [Randomizer] [EnemyRandomizer] Enemy record domain::encounter::RANDOMIZED (",
                    getNameFromRoom(room_encounters[i].name), ") #",
                    (size_t)room_encounters[i].encounters[j].id, " : ", (size_t)buffer[0], " , ", offset);
                if (buffer[0] == (char)0xff) {
                    break;
                }
            }

            enemy_pool_size -= enemies;
            logger->log(
                "[INF] [Randomizer] [EnemyRandomizer] Enemy pool size domain::encounter::UNRANDOMIZED (",
                getNameFromRoom(room_encounters[i].name), ") #", (size_t)room_encounters[i].encounters[j].id,
                " : ", enemy_pool_size);
        }
    }

    logger->log("[INF] [Randomizer] [EnemyRandomizer] Enemy pool size : ", enemy_pool_size);

    return enemy_pool_size;
}

size_t usecase::enemies::EnemyRandomizer::rewriteMaskedEncounter(
    std::fstream &file, std::fstream &room_file, std::vector<std::vector<char>> &descriptors,
    domain::encounter::pe2_room_encounters_t room, size_t i, size_t offset) {
    std::vector<char> encounter_record(ENEMY_RECORD_DESCRIPTOR_SIZE);
    file.seekg(room.encounters_offset, std::ios::beg);
    for (size_t k = 0; k < i; k++) {
        while (true) {
            file.read(encounter_record.data(), ENEMY_RECORD_DESCRIPTOR_SIZE);
            if (encounter_record[0] == (char)0xFF) {
                break;
            }
        }
    }
    room_file.seekg(room.encounters[i].enemy_resources_desciptors_list_offset, std::ios::beg);
    char cur_monster_id = 0x0;
    room_file.read(&cur_monster_id, 1);
    room_file.seekg(offset, std::ios::beg);
    size_t old_offset = offset;
    while (room.encounters[i].need_to_skip_encoutners) {
        file.read(encounter_record.data(), ENEMY_RECORD_DESCRIPTOR_SIZE);
        old_offset += ENEMY_RECORD_DESCRIPTOR_SIZE;
        if (encounter_record[0] == cur_monster_id) {
            old_offset -= ENEMY_RECORD_DESCRIPTOR_SIZE;
            file.seekg(old_offset, std::ios::beg);
            break;
        }
    }
    while (true) {
        file.read(encounter_record.data(), ENEMY_RECORD_DESCRIPTOR_SIZE);
        descriptors.push_back(encounter_record);
        offset += ENEMY_RECORD_DESCRIPTOR_SIZE;
        if (encounter_record[0] == (char)0xFF) {
            break;
        }
    }
    return offset;
}

std::map<uint32_t, uint8_t> usecase::enemies::EnemyRandomizer::generateRandomEnemyIds(
    size_t pool_size, domain::encounter::pe2_room_encounters_t room,
    domain::encounter::pe2_encounter_t encounter,
    const std::function<uint8_t(const std::vector<uint8_t> &, int)> &choose) {
    auto pick = [&](const auto &pool, int fallback) {
        return choose ? choose(pool, fallback) : utils::vector_works::getRandomElement(gen, pool, fallback);
    };
    // Build vector of available enemies in this encounter due to restrictions and customized options

    // First add normal enemies
    std::vector<uint8_t> result(common_enemies_by_size_map.at(ENEMY_SIZE_NORMAL).begin(),
                                common_enemies_by_size_map.at(ENEMY_SIZE_NORMAL).end());
    result = utils::vector_works::concatVector(result, common_enemies_by_size_map.at(ENEMY_SIZE_SMALLER));
    // Than add special enemies if allow them
    if (encounter.mask != domain::encounter::FORBID_SLOT_3_ENEMIES &&
        encounter.mask != domain::encounter::FORBID_MULTI_ENEMY_ENC_AND_SLOT_3) {
        result = utils::vector_works::concatVector(result, special_enemies);
    }

    // Than add small enemies if we have enough pool size for generating adequate amount
    if (pool_size > POOL_ENEMY_THRESCHOLD) {
        result = utils::vector_works::concatVector(result, common_enemies_by_size_map.at(ENEMY_SIZE_SMALL));
    }

    // Add additional enemies and remove forbidden for this rooom only
    result = utils::vector_works::concatVector(result, room.allowed_additional_enemies);
    result = utils::vector_works::substractVector(result, room.forbidden_enemies);

    // Intersect with chosen by player
    result = utils::vector_works::intersecVector(result, config.randomized_enemies_pool);

    auto removeUnsafeFirst = [&](std::vector<uint8_t> &candidates) {
        std::erase_if(candidates, [&](uint8_t id) {
            auto enemy = enemies_map.at(id);
            if (config.flags.cloak_golem_mult && (id == KNIGHT_GOLEM_ID || id == BISHOP_GOLEM_ID))
                enemy = one_slot_cloak_golems_map.at(id);
            return !soundFits({{enemy.available_files.begin()->first, id}}) ||
                   !modelsFit({{enemy.available_files.begin()->first, id}});
        });
    };
    removeUnsafeFirst(result);
    uint8_t first_enemy_id = NO_ENEMY_ID;
    if (result.size() > 0) {
        first_enemy_id = pick(result, 0x01);
        result = utils::vector_works::substractVector(result, std::vector<uint8_t>({first_enemy_id}));
    } else {
        // We can't spawn any enemy in this room, may be we can spawn little ones?
        result = std::vector<uint8_t>(config.randomized_enemies_pool);
        result = utils::vector_works::substractVector(result, room.forbidden_enemies);
        if (encounter.mask != domain::encounter::FORBID_SLOT_3_ENEMIES) {
            result = utils::vector_works::concatVector(result, special_enemies);
        }
        result = utils::vector_works::intersecVector(result, config.randomized_enemies_pool);
        removeUnsafeFirst(result);
        if (result.empty())
            throw std::runtime_error("No selected enemy fits the room's resource/audio restrictions: " +
                                     getNameFromRoom(room.name));
        first_enemy_id = pick(result, 0x01);
        result = utils::vector_works::substractVector(result, std::vector<uint8_t>({first_enemy_id}));
    }

    // Let's remove cloak golems because they will not be possible here by any chance
    result = utils::vector_works::substractVector(result, {KNIGHT_GOLEM_ID, BISHOP_GOLEM_ID});

    // Let's calculate how much space do we have for additional enemies and mark available slots with
    // NO_ENEMY_ID
    pe2_enemy_t first_enemy = enemies_map.at(first_enemy_id);
    if (config.flags.cloak_golem_mult && (first_enemy.id == 0x16 || first_enemy.id == 0x27)) {
        first_enemy = one_slot_cloak_golems_map.at(first_enemy.id);
    }
    std::map<uint32_t, uint8_t> encounter_enemies = {};
    bool already_have_dummy_monster = false;
    if (first_enemy.available_files.find(ENEMY_SLOT_1_ADDRESS) != first_enemy.available_files.end()) {
        encounter_enemies[ENEMY_SLOT_1_ADDRESS] = first_enemy_id;
        logger->log("[INF] [Randomizer] [EnemyRandomizer] - enemy ", getEnemyName(first_enemy_id),
                    " is using slot 1");
        if (first_enemy.file_size == ENEMY_FILE_1_SLOT &&
            encounter.mask != domain::encounter::FORBID_SLOT_2_ENEMIES) {
            encounter_enemies[ENEMY_SLOT_2_ADDRESS] = NO_ENEMY_ID;
            logger->log("[INF] [Randomizer] [EnemyRandomizer] - we have slot 2 available");
        }
        if (first_enemy.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES &&
            std::find(room.allowed_additional_enemies.begin(), room.allowed_additional_enemies.end(),
                      first_enemy_id) == room.allowed_additional_enemies.end()) {
            encounter_enemies[ENEMY_SLOT_3_ADDRESS] = DUMMY_MONSTER_ID;
            already_have_dummy_monster = true;
            logger->log("[INF] [Randomizer] [EnemyRandomizer] - we use slot 3 for dummy data");
        } else if (encounter.mask != domain::encounter::FORBID_SLOT_3_ENEMIES &&
                   encounter.mask != domain::encounter::FORBID_MULTI_ENEMY_ENC_AND_SLOT_3) {
            encounter_enemies[ENEMY_SLOT_3_ADDRESS] = NO_ENEMY_ID;
            logger->log("[INF] [Randomizer] [EnemyRandomizer] - we have slot 3 available");
        }
    } else if (first_enemy.available_files.find(ENEMY_SLOT_2_ADDRESS) != first_enemy.available_files.end()) {
        encounter_enemies[ENEMY_SLOT_1_ADDRESS] = NO_ENEMY_ID;
        logger->log("[INF] [Randomizer] [EnemyRandomizer] - we have slot 1 available");
        encounter_enemies[ENEMY_SLOT_2_ADDRESS] = first_enemy_id;
        logger->log("[INF] [Randomizer] [EnemyRandomizer] - enemy ", getEnemyName(first_enemy_id),
                    " is using slot 2");
        if (first_enemy.file_size == ENEMY_FILE_1_SLOT) {
            if (first_enemy.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES &&
                std::find(room.allowed_additional_enemies.begin(), room.allowed_additional_enemies.end(),
                          first_enemy_id) == room.allowed_additional_enemies.end()) {
                encounter_enemies[ENEMY_SLOT_3_ADDRESS] = DUMMY_MONSTER_ID;
                already_have_dummy_monster = true;
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - we use slot 3 for dummy data");
            } else if (encounter.mask != domain::encounter::FORBID_SLOT_3_ENEMIES &&
                       encounter.mask != domain::encounter::FORBID_MULTI_ENEMY_ENC_AND_SLOT_3) {
                encounter_enemies[ENEMY_SLOT_3_ADDRESS] = NO_ENEMY_ID;
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - we have slot 3 available");
            }
        }
    } else if (first_enemy.available_files.find(ENEMY_SLOT_3_ADDRESS) != first_enemy.available_files.end()) {
        encounter_enemies[ENEMY_SLOT_1_ADDRESS] = NO_ENEMY_ID;
        logger->log("[INF] [Randomizer] [EnemyRandomizer] - we have slot 1 available");
        encounter_enemies[ENEMY_SLOT_2_ADDRESS] = NO_ENEMY_ID;
        logger->log("[INF] [Randomizer] [EnemyRandomizer] - we have slot 2 available");
        encounter_enemies[ENEMY_SLOT_3_ADDRESS] = first_enemy_id;
        logger->log("[INF] [Randomizer] [EnemyRandomizer] - enemy ", getEnemyName(first_enemy_id),
                    " is using slot 3");
    } else {
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] - no available files were found for enemy: ",
                    getEnemyName(first_enemy_id));
        throw std::runtime_error(
            "EnemyRandomizer] - no available files were found for enemy for a certain enemy.");
    }

    // Check if we can add second enemy to encounter
    if (result.size() > 0 && encounter.mask != domain::encounter::FORBID_MULTI_ENEMY_ENC &&
        (encounter.mask != domain::encounter::FORBID_ALL_SLOTS_ENC ||
         first_enemy.res_type != ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES) &&
        encounter.mask != domain::encounter::FORBID_MULTI_ENEMY_ENC_AND_SLOT_3) {
        std::uniform_real_distribution<> dis(0.0, 1.0);
        double prob = dis(gen);
        if (prob < MULTI_ENIMIES_ENCOUNTER_PROBABILITY) {
            // First if we have occupied the second slot or we load dummy monster we remove all 2slot enemies
            if (encounter_enemies.find(ENEMY_SLOT_2_ADDRESS) != encounter_enemies.end()) {
                if (encounter_enemies[ENEMY_SLOT_2_ADDRESS] != NO_ENEMY_ID || already_have_dummy_monster ||
                    first_enemy.file_size > ENEMY_FILE_1_SLOT ||
                    encounter.mask == domain::encounter::FORBID_SLOT_3_ENEMIES) {
                    result = utils::vector_works::substractVector(
                        result, enemies_by_file_size.at(ENEMY_FILE_2_SLOTS));
                }
            }
            // If enemy occupies 2-slots we forbid special enemies because we have no space for dummy monster
            // then
            if (first_enemy.file_size > ENEMY_FILE_1_SLOT ||
                encounter.mask == domain::encounter::FORBID_SLOT_3_ENEMIES) {
                result = utils::vector_works::substractVector(result, special_enemies);
            }
            uint32_t available_slot = ENEMY_NO_SLOT;
            for (auto &&entry : encounter_enemies) {
                if (entry.second == NO_ENEMY_ID) {
                    available_slot = entry.first;
                    break;
                }
            }
            std::string slot_name = "slot 1";
            if (available_slot == ENEMY_SLOT_2_ADDRESS) {
                slot_name = "slot 2";
            } else if (available_slot == ENEMY_SLOT_3_ADDRESS) {
                slot_name = "slot 3";
            }
            logger->log("[INF] [Randomizer] [EnemyRandomizer] - available slot is ", slot_name);
            if (available_slot != ENEMY_NO_SLOT) {
                result = utils::vector_works::intersecVector(result, enemy_ids_by_slot_map[available_slot]);
                std::erase_if(result, [&](uint8_t id) {
                    auto proposed = encounter_enemies;
                    proposed[available_slot] = id;
                    return !soundFits(proposed) || !modelsFit(proposed);
                });
                // No we finally can add second enemy type to encounter
                if (result.size() > 0) {
                    uint8_t second_enemy_id = pick(result, 0x0);
                    encounter_enemies[available_slot] = second_enemy_id;
                    pe2_enemy_t second_enemy = enemies_map.at(second_enemy_id);
                    if (second_enemy.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES &&
                        !already_have_dummy_monster &&
                        std::find(room.allowed_additional_enemies.begin(),
                                  room.allowed_additional_enemies.end(),
                                  second_enemy_id) == room.allowed_additional_enemies.end()) {
                        encounter_enemies[ENEMY_SLOT_3_ADDRESS] = DUMMY_MONSTER_ID;
                    }
                }
            }
        }
    }

    // Remove all slots entry that are unused

    for (auto it = encounter_enemies.begin(); it != encounter_enemies.end();) {
        if (it->second == NO_ENEMY_ID) {
            it = encounter_enemies.erase(it);
        } else {
            ++it;
        }
    }

    if (!soundFits(encounter_enemies))
        throw std::runtime_error("Enemy sound budget exceeded");
    if (!modelsFit(encounter_enemies))
        throw std::runtime_error("Enemy model budget exceeded");
    logger->log("[INF] [Randomizer] [EnemyRandomizer] - we have chosen ", encounter_enemies.size(),
                " enemies/enemy for this encounter. It is/they are:");
    for (auto &&i : encounter_enemies) {
        logger->log("[INF] [Randomizer] [EnemyRandomizer] - ", int(i.second), " - ", getEnemyName(i.second));
    }

    return encounter_enemies;
}

const usecase::enemies::EnemyRandomizer::SpawnZones &
usecase::enemies::EnemyRandomizer::cachedSpawningZones(std::map<pe2_enemy_movement_t, SpawnZones> &cache,
                                                       const domain::encounter::pe2_room_encounters_t &room,
                                                       pe2_enemy_t enemy) {
    auto [it, inserted] = cache.try_emplace(enemy.move_type);
    if (inserted)
        it->second.fromWaterToGround =
            prepareSpawningZones(it->second.allowed, it->second.forbidden, room, enemy);
    return it->second;
}

bool usecase::enemies::EnemyRandomizer::prepareSpawningZones(
    std::vector<utils::geometry_works::Zone> &zones, std::vector<utils::geometry_works::Zone> &dead_zones,
    domain::encounter::pe2_room_encounters_t room, pe2_enemy_t enemy) {
    for (utils::geometry_works::Zone zone : room.environment.spawn_zones) {
        zone.polygon = utils::geometry_works::reorderVerticesClockwise(zone.polygon);
        zones.push_back(zone);
    }

    for (utils::geometry_works::Zone zone : room.environment.dead_zones) {
        zone.polygon = utils::geometry_works::reorderVerticesClockwise(zone.polygon);
        dead_zones.push_back(zone);
    }

    if (enemy.move_type == ENEMY_MOVE_AERIAL) {
        for (utils::geometry_works::Zone zone : room.environment.spawn_aerial_zones) {
            zone.polygon = utils::geometry_works::reorderVerticesClockwise(zone.polygon);
            zones.push_back(zone);
        }
    }

    if (enemy.move_type == ENEMY_MOVE_WATER || enemy.move_type == ENEMY_MOVE_AERIAL) {
        for (utils::geometry_works::Zone zone : room.environment.spawn_water_zones) {
            zone.polygon = utils::geometry_works::reorderVerticesClockwise(zone.polygon);
            zones.push_back(zone);
        }
    }

    // Retain this flag while the encounter format still distinguishes these cases.
    bool from_water_to_ground = false;

    if (enemy.move_type == ENEMY_MOVE_WATER && room.environment.spawn_water_zones.size() > 0) {
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        float prob = dist(gen);
        if (prob > 0.8) {
            from_water_to_ground = true;
        } else {
            zones = {};
            for (utils::geometry_works::Zone zone : room.environment.spawn_water_zones) {
                zone.polygon = utils::geometry_works::reorderVerticesClockwise(zone.polygon);
                zones.push_back(zone);
            }
        }
    } else {
        from_water_to_ground = true;
    }

    if (room.environment.separated_zones) {
        std::uniform_int_distribution<int> dist(0, zones.size() - 1);
        int random_index = dist(gen);
        zones = {zones[random_index]};
    }

    return from_water_to_ground;
}

std::vector<char> usecase::enemies::EnemyRandomizer::buildEnemyEncounterRecordDescriptor(
    utils::geometry_works::Circle &spawn_point, pe2_enemy_t enemy, bool &from_water_to_ground,
    bool &ambush_flag, bool &ambush_was_chosen, uint16_t ceiling_height, size_t enemy_i,
    bool first_enemy_was_a_big_textured_guy, bool has_cloak_enemy) {
    // calculating random angle for spawn point
    std::uniform_int_distribution<int> dist(std::numeric_limits<char>::min(),
                                            std::numeric_limits<char>::max());
    char random_angle = static_cast<char>(dist(gen));

    // calculating texture offset in VRAM
    uint32_t texture_offset = first_enemy_was_a_big_textured_guy ? 0x00030100 : 0x00020000;
    while (enemy_i > 0) {
        enemy_i--;
        // if (enemy.texture_size == ENEMY_TEX_NORMAL) {
        //     texture_offset += 0x00020200;
        // } else {
        //     texture_offset += 0x00030300;
        // }
        texture_offset += 0x00020200;
        if (enemy.texture_size == ENEMY_TEX_BIG && !first_enemy_was_a_big_textured_guy) {
            texture_offset += 0x00010100;
        }
    }

    // If enemy is flying set another height coordinate:
    if (enemy.move_type == ENEMY_MOVE_AERIAL) {
        spawn_point.center.z = spawn_point.center.z - 0x800;
    }

    // Generate first parameter
    uint8_t first_param = 0x00;
    if (enemy.first_param.size() > 0) {
        float base = 0.0;
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        float random_float = dist(gen);
        for (size_t l = 0; l < enemy.first_param.size(); ++l) {
            base += enemy.first_param[l].first;
            if (random_float <= base) {
                first_param = enemy.first_param[l].second;
                break;
            }
        }
    }

    // Generate second parameter
    uint8_t second_param = 0;
    if (enemy.move_type == ENEMY_MOVE_WATER && !from_water_to_ground) {
        // For divers only
        std::uniform_int_distribution<int> dist(1, 4);
        second_param = dist(gen);
    } else {
        // Basic second parameter
        if (enemy.second_param.size() > 0) {
            float base = 0.0;
            std::uniform_real_distribution<float> dist(0.0f, 1.0f);
            float random_float = dist(gen);
            for (size_t l = 0; l < enemy.second_param.size(); ++l) {
                base += enemy.second_param[l].first;
                if (random_float <= base) {
                    second_param = enemy.second_param[l].second;
                    break;
                }
            }
            // Ambush second parameter
        } else if (enemy.ambush_type != ENEMY_NO_AMBUSH) {
            if (!ambush_was_chosen) {
                std::uniform_real_distribution<float> dist(0.0f, 1.0f);
                float random_float = dist(gen);
                if (random_float < ENEMY_AMBUSH_PROBABILITY) {
                    ambush_flag = true;
                    second_param = enemy.ambush_value;
                }
                ambush_was_chosen = true;
            } else {
                if (ambush_flag) {
                    second_param = enemy.ambush_value + 1;
                }
            }
        }
    }

    // For failed chaser state 1:

    if (enemy.id == 0x2c && second_param == 1) {
        spawn_point.center.z = spawn_point.center.z - 0xa00;
    }

    if (enemy.id == 0x03) {
        second_param = current_slender_state % 2;
        current_slender_state++;
    }

    if (ambush_flag && enemy.ambush_type == ENEMY_AERIAL_AMBUSH) {
        spawn_point.center.z =
            spawn_point.center.z -
            (enemy.move_type == ENEMY_MOVE_AERIAL ? (ceiling_height - 0x800) : ceiling_height);
    }

    if (config.flags.cloak_golem_mult && has_cloak_enemy) {
        if (enemy.id == 0x27 || enemy.id == 0x16) {
            texture_offset = 0x0005ff00;
        } else {
            texture_offset = 0x00020000;
        }
    }

    pe2_enemy_record_desc_t current_descriptor{
        .id = enemy.id,
        .param1 = first_param,
        .param2 = second_param,
        .spawning_point_x = (uint16_t)spawn_point.center.x,
        .spawning_point_y = (uint16_t)spawn_point.center.y,
        .spawning_point_z = (uint16_t)spawn_point.center.z,
        .angle = (uint16_t)(random_angle << 0x8),
        .texture_vram_offset = texture_offset,
    };

    return recordDescriptorToBytesVector(current_descriptor);
}

void usecase::enemies::EnemyRandomizer::createWalkingPathForSlender(
    std::vector<utils::geometry_works::Zone> &zones, std::vector<utils::geometry_works::Zone> &dead_zones,
    uint8_t room_id, uint8_t stage_id) {
    if (current_slender_walking_route_offset + 32 > 0x5540 ||
        current_slender_walking_route_addresses_offset + 8 > 0x5a00 || current_slender_path_id == 255)
        throw std::runtime_error("Brain Stinger routes exceed shared resource region");
    std::vector<utils::geometry_works::Circle> spawn_points =
        utils::geometry_works::arrangeSpawnPoints(4, 512, zones, dead_zones, gen, 100);

    std::fstream dummy_monster_file(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                        "/enemies/dummy_monster.pe2e.decompr",
                                    std::ios::in | std::ios::out | std::ios::binary);
    std::fstream slender_additional_data(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                             "/enemies/slender_additional_data.pe2e",
                                         std::ios::in | std::ios::out | std::ios::binary);

    utils::file_works::writeFourBytesLittleEndian(
        slender_additional_data, ((uint32_t)(stage_id) << 16) + (uint32_t)(current_slender_path_id),
        current_slender_additional_data_address);
    utils::file_works::writeFourBytesLittleEndian(slender_additional_data,
                                                  ((uint32_t)(2) << 16) + (uint32_t)(room_id),
                                                  current_slender_additional_data_address + 4);
    current_slender_additional_data_address += 8;
    current_slender_path_id++;

    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file,
                                                  0x80161e20 + current_slender_walking_route_offset,
                                                  current_slender_walking_route_addresses_offset);
    current_slender_walking_route_addresses_offset += 4;
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file,
                                                  0x80161e20 + current_slender_walking_route_offset + 0x10,
                                                  current_slender_walking_route_addresses_offset);
    current_slender_walking_route_addresses_offset += 4;

    for (auto sp : spawn_points) {
        utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, (uint32_t)sp.center.x,
                                                      current_slender_walking_route_offset);
        utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, (uint32_t)sp.center.y,
                                                      current_slender_walking_route_offset + 4);
        current_slender_walking_route_offset += 8;
    }

    for (size_t i = 0; i < 4 - spawn_points.size(); i++) {
        utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, (uint32_t)0x0,
                                                      current_slender_walking_route_offset);
        utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, (uint32_t)0x0,
                                                      current_slender_walking_route_offset + 4);
        current_slender_walking_route_offset += 8;
    }
}

void usecase::enemies::EnemyRandomizer::createSpawnZoneForPurpleGolem(
    std::vector<utils::geometry_works::Zone> &zones, std::vector<utils::geometry_works::Zone> &dead_zones,
    uint8_t room_id, uint8_t stage_id) {
    if (current_purple_golem_spawn_zone_offset + 16 > 0x67e0 ||
        current_purple_golem_spawn_zone_addresses_offset + 4 > domain::enemy::dummyTaskOffset ||
        current_purple_golem_spawn_zone_id == 255)
        throw std::runtime_error("Cloak Golem routes exceed shared resource region");
    std::fstream dummy_monster_file(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                        "/enemies/dummy_monster.pe2e.decompr",
                                    std::ios::in | std::ios::out | std::ios::binary);
    std::fstream golem_additional_data(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                           "/enemies/knight_golem_additional_data.pe2e",
                                       std::ios::out | std::ios::app | std::ios::binary);
    utils::file_works::writeFourBytesLittleEndian(
        golem_additional_data,
        ((uint32_t)(stage_id) << 16) + (uint32_t)((current_purple_golem_spawn_zone_id) << 0), 0);
    utils::file_works::writeFourBytesLittleEndian(golem_additional_data,
                                                  ((uint32_t)(1) << 16) + (uint32_t)((room_id) << 0), 0);
    current_purple_golem_spawn_zone_id++;

    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file,
                                                  0x80161e20 + current_purple_golem_spawn_zone_offset,
                                                  current_purple_golem_spawn_zone_addresses_offset);
    current_purple_golem_spawn_zone_addresses_offset += 4;

    std::uniform_int_distribution<> dist(0, zones.size() - 1);
    std::uniform_int_distribution<uint16_t> angle_dist(0, 65535);
    int random_index = dist(gen);
    int random_index_2 = random_index;
    if (zones.size() > 1) {
        while (random_index_2 != random_index) {
            random_index_2 = dist(gen);
        }
    } else {
        random_index = 0;
    }
    utils::geometry_works::Zone zone_to_choose = zones[random_index];
    utils::geometry_works::Zone zone_to_choose_2 = zones[random_index_2];

    std::vector<utils::geometry_works::Circle> spawn_points =
        utils::geometry_works::arrangeSpawnPoints(1, 256, {zone_to_choose}, dead_zones, gen, 100);
    if (spawn_points.size() == 0) {
        spawn_points.push_back(utils::geometry_works::Circle(utils::geometry_works::Point(), 512));
    }

    utils::geometry_works::Rectangle spawn_rect =
        utils::geometry_works::findLongestRectangleInPolygons(zones, dead_zones);

    int16_t min_x = spawn_rect.topLeft.x;
    int16_t max_x = spawn_rect.topLeft.x + spawn_rect.width;
    int16_t min_y = spawn_rect.topLeft.y;
    int16_t max_y = spawn_rect.topLeft.y + spawn_rect.length;

    bool horizontal = true;
    if (spawn_rect.length > spawn_rect.width) {
        horizontal = false;
    }
    utils::file_works::writeTwoBytesLittleEndian(dummy_monster_file, (uint16_t)0x0001,
                                                 current_purple_golem_spawn_zone_offset);
    current_purple_golem_spawn_zone_offset += 2;
    utils::file_works::writeTwoBytesLittleEndian(dummy_monster_file, horizontal ? 0x0400 : 0x0000,
                                                 current_purple_golem_spawn_zone_offset);
    current_purple_golem_spawn_zone_offset += 2;
    utils::file_works::writeTwoBytesLittleEndian(
        dummy_monster_file, (uint16_t)spawn_rect.topLeft.x + (horizontal ? 0 : spawn_rect.width / 2),
        current_purple_golem_spawn_zone_offset);
    current_purple_golem_spawn_zone_offset += 2;
    utils::file_works::writeTwoBytesLittleEndian(
        dummy_monster_file, (uint16_t)spawn_rect.topLeft.y + (horizontal ? spawn_rect.length / 2 : 0),
        current_purple_golem_spawn_zone_offset);
    current_purple_golem_spawn_zone_offset += 2;
    utils::file_works::writeTwoBytesLittleEndian(dummy_monster_file,
                                                 (uint16_t)min_x + (horizontal ? 0x0900 : 0),
                                                 current_purple_golem_spawn_zone_offset);
    current_purple_golem_spawn_zone_offset += 2;
    utils::file_works::writeTwoBytesLittleEndian(dummy_monster_file, (uint16_t)max_y,
                                                 current_purple_golem_spawn_zone_offset);
    current_purple_golem_spawn_zone_offset += 2;
    utils::file_works::writeTwoBytesLittleEndian(dummy_monster_file, (uint16_t)max_x,
                                                 current_purple_golem_spawn_zone_offset);
    current_purple_golem_spawn_zone_offset += 2;
    utils::file_works::writeTwoBytesLittleEndian(dummy_monster_file,
                                                 (uint16_t)min_y + (!horizontal ? 0x0900 : 0),
                                                 current_purple_golem_spawn_zone_offset);
    current_purple_golem_spawn_zone_offset += 2;
}

// Counts returned here are actual actors, with a separate slot for the dummy
// and the terminator. Never borrow the records reserved for later encounters.
void usecase::enemies::EnemyRandomizer::fitEnemyCounts(std::map<uint32_t, uint8_t> &slots,
                                                       std::map<uint8_t, size_t> &counts,
                                                       const domain::encounter::pe2_room_encounters_t &room,
                                                       const domain::encounter::pe2_encounter_t &encounter,
                                                       size_t budget) {
    auto needsDummy = [&](uint8_t id) {
        return enemies_map.at(id).res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES &&
               encounter.mask != domain::encounter::FORBID_SLOT_3_ENEMIES &&
               std::find(room.allowed_additional_enemies.begin(), room.allowed_additional_enemies.end(),
                         id) == room.allowed_additional_enemies.end();
    };
    // The legacy count generator includes a replaceable special-enemy actor.
    for (auto &[id, count] : counts) {
        if (!count)
            throw std::runtime_error("Empty randomized enemy count");
        if (needsDummy(id)) {
            if (count > 1)
                --count;
            break;
        }
    }
    auto records = [&]() {
        size_t result = 1; // terminator
        bool dummy = false;
        for (const auto &[id, count] : counts) {
            result += count;
            dummy |= needsDummy(id);
        }
        return result + size_t(dummy);
    };
    while (records() > budget) {
        auto largest = std::max_element(counts.begin(), counts.end(),
                                        [](const auto &a, const auto &b) { return a.second < b.second; });
        if (largest == counts.end())
            throw std::runtime_error("No enemy fits encounter record budget");
        if (largest->second > 1)
            --largest->second;
        else if (counts.size() > 1)
            counts.erase(std::prev(counts.end()));
        else
            throw std::runtime_error("Enemy and resources do not fit encounter record budget");
    }
    bool dummy = false;
    for (const auto &[id, count] : counts)
        dummy |= needsDummy(id);
    for (auto it = slots.begin(); it != slots.end();) {
        if (it->second == DUMMY_MONSTER_ID ? !dummy : counts.count(it->second) == 0)
            it = slots.erase(it);
        else
            ++it;
    }
}

void usecase::enemies::EnemyRandomizer::randomizeEnemiesInRoom(
    std::fstream &level_file, std::fstream &room_file, domain::encounter::pe2_room_encounters_t room,
    std::vector<std::vector<char>> &descriptors,
    std::vector<domain::encounter::pe2_enc_reward_descriptor_t> &rewards, size_t &offset,
    size_t &enemy_pool_size, bool internal, int stage_num) {
    logger->log("[INF] [Randomizer] [EnemyRandomizer] starting randomizing encounters for room ",
                getNameFromRoom(room.name));
    bool room_file_end_flag = true;
    bool first_slender = true;
    bool first_cloak_golem = true;
    size_t next_file_bias = 0x0;
    std::vector<size_t> reserved(room.encounters.size());
    std::vector<std::vector<std::vector<char>>> preserved(room.encounters.size());
    auto isAlias = [](uint8_t id) { return (id >= 0x07 && id < 0x09) || (id >= 0x17 && id < 0x19); };
    size_t record_end = 0;
    if (internal) {
        descriptors = {};
        // Internal encounters_end is the address of the LAST 16-byte record
        // (the original terminator), not the first byte of the following data.
        if (!room.encounters.empty()) {
            if (room.encounters_end < room.encounters_offset ||
                (room.encounters_end - room.encounters_offset) % ENEMY_RECORD_DESCRIPTOR_SIZE)
                throw std::runtime_error("Invalid internal encounter bounds");
            record_end = room.encounters_end + ENEMY_RECORD_DESCRIPTOR_SIZE;
            room_file.seekg(0, std::ios::end);
            if (record_end > static_cast<size_t>(room_file.tellg()))
                throw std::runtime_error("Internal encounter bounds exceed room file");
            enemy_pool_size = (record_end - room.encounters_offset) / ENEMY_RECORD_DESCRIPTOR_SIZE;
        } else
            enemy_pool_size = 0;
        for (size_t i = 0; i < room.encounters.size(); ++i) {
            const auto &encounter = room.encounters[i];
            if (isAlias(encounter.id))
                continue;
            if (encounter.mask == domain::encounter::UNRANDOMIZED) {
                // Snapshot by the original pointer before rewriting any pointer.
                unsigned char address[4];
                room_file.seekg(encounter.enemy_record_descriptor_list_addr_offset);
                if (!room_file.read(reinterpret_cast<char *>(address), 4))
                    throw std::runtime_error("Cannot read original encounter pointer");
                uint32_t pointer = uint32_t(address[0]) | uint32_t(address[1]) << 8 |
                                   uint32_t(address[2]) << 16 | uint32_t(address[3]) << 24;
                if (pointer < ROOM_INFO_FILE_LOAD_ADDRESS)
                    throw std::runtime_error("Invalid original encounter pointer");
                size_t source = pointer - ROOM_INFO_FILE_LOAD_ADDRESS;
                while (true) {
                    if (source < room.encounters_offset ||
                        source > record_end - ENEMY_RECORD_DESCRIPTOR_SIZE ||
                        (source - room.encounters_offset) % ENEMY_RECORD_DESCRIPTOR_SIZE)
                        throw std::runtime_error("Original encounter exceeds room record bounds");
                    std::vector<char> record(ENEMY_RECORD_DESCRIPTOR_SIZE);
                    room_file.seekg(source);
                    if (!room_file.read(record.data(), record.size()))
                        throw std::runtime_error("Truncated original encounter");
                    preserved[i].push_back(record);
                    if (static_cast<uint8_t>(record[0]) == 0xff)
                        break;
                    source += ENEMY_RECORD_DESCRIPTOR_SIZE;
                }
                reserved[i] = preserved[i].size();
            } else {
                // One actor + optional dummy + terminator. More types/actors
                // can only consume the genuinely unreserved room capacity.
                reserved[i] = (encounter.mask == domain::encounter::FORBID_SLOT_3_ENEMIES ||
                               encounter.mask == domain::encounter::FORBID_MULTI_ENEMY_ENC_AND_SLOT_3)
                                  ? 2
                                  : 3;
            }
            if (reserved[i] > enemy_pool_size)
                throw std::runtime_error("Room has insufficient encounter record capacity");
            enemy_pool_size -= reserved[i];
        }
    }
    for (size_t i = 0; i < room.encounters.size(); i++) {
        if ((room.encounters[i].id >= 0x07 && room.encounters[i].id < 0x09) ||
            (room.encounters[i].id >= 0x17 && room.encounters[i].id < 0x19)) {
            // make bounty encounters same as OG
            room_file.seekg(room.encounters[i - 1].mask != domain::encounter::UNRANDOMIZED
                                ? room.encounters[i - 1].enemy_record_descriptor_list_addr_offset
                                : room.encounters[i - 2].enemy_record_descriptor_list_addr_offset);
            char buffer[8];
            room_file.read(buffer, 8);
            room_file.seekp(room.encounters[i].enemy_record_descriptor_list_addr_offset);
            room_file.write(buffer, 8);
            continue;
        }
        logger->log("[INF] [Randomizer] [EnemyRandomizer] - ", getNameFromRoom(room.name),
                    " - randomizing encounter #", i + 1);
        const size_t descriptor_start = descriptors.size();
        const size_t record_budget = internal ? reserved[i] + enemy_pool_size : 0;
        // Write new absolute address to the current randomized encounter (that is going to be randomized
        // soon)
        utils::file_works::writeFourBytesLittleEndian(
            room_file,
            internal ? ROOM_INFO_FILE_LOAD_ADDRESS + offset : LEVEL_INFO_FILE_LOAD_ADDRESS + offset,
            room.encounters[i].enemy_record_descriptor_list_addr_offset);
        // 4. Check if this encounter is masked (if it is -1, than this encounter is not randomized)
        if (room.encounters[i].mask == domain::encounter::UNRANDOMIZED) {
            logger->log("[INF] [Randomizer] [EnemyRandomizer] - this encounter is unrandomized. Just "
                        "rewriting original stuff...");
            // TODO: Need to check if it works in the case, when the masked encounter is overwritten by the
            // previous encounter. 4a. If is masked just rewrite it without changes
            if (internal) {
                descriptors.insert(descriptors.end(), preserved[i].begin(), preserved[i].end());
                offset += preserved[i].size() * ENEMY_RECORD_DESCRIPTOR_SIZE;
            } else
                offset = rewriteMaskedEncounter(level_file, room_file, descriptors, room, i, offset);
            if (room.encounters[i].force_reward_value != 0) {
                rewards.push_back(domain::encounter::pe2_enc_reward_descriptor_t{
                    .encounter_id = room.encounters[i].id,
                    .room_id = room.id,
                    .stage_id = (uint8_t)stage_num,
                    .items_list{},
                    .value = room.encounters[i].force_reward_value});
            }
        } else {
            // 4b. Else, we need to randomize stuff

            // 4b.1. Let's choose enemies for this encounter
            std::map<uint32_t, uint8_t> enemy_slots =
                generateRandomEnemyIds(enemy_pool_size, room, room.encounters[i]);
            logger->log("[INF] [Randomizer] [EnemyRandomizer] - enemy ids are defined.");

            // 4b.2. Let's count how many enemies of each type we should get
            size_t count_pool = enemy_pool_size;
            std::map<uint8_t, size_t> enemy_count_map = generateRandomEnemyCount(
                enemy_slots, internal ? count_pool : enemy_pool_size, room.encounters[i].encounter_value);
            if (internal)
                fitEnemyCounts(enemy_slots, enemy_count_map, room, room.encounters[i], record_budget);
            if (roomModelCapacity)
                domain::enemy::models::fit(roomModelCapacity, enemy_count_map);
            logger->log("[INF] [Randomizer] [EnemyRandomizer] - we have decided to spawn: ");
            size_t real_value = 0;
            // Let's calculate real dropped value
            for (auto it = enemy_count_map.begin(); it != enemy_count_map.end();) {
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - x", it->second, " ",
                            getEnemyName(it->first));
                pe2_enemy_t enemy = enemies_map.at(it->first);
                size_t enemy_value = enemy.value;
                size_t enemy_num = it->second;
                if (enemy.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES &&
                    room.encounters[i].mask != domain::encounter::FORBID_SLOT_3_ENEMIES) {
                    if (!internal && enemy_num > 1) {
                        enemy_num--;
                    }
                    enemy_value *= 1.4;
                }
                real_value += enemy_value * enemy_num;
                it++;
            }
            rewards.push_back(domain::encounter::pe2_enc_reward_descriptor_t{
                .encounter_id = room.encounters[i].id,
                .room_id = room.id,
                .stage_id = (uint8_t)stage_num,
                .items_list{},
                .value = real_value > room.encounters[i].encounter_value
                             ? real_value - room.encounters[i].encounter_value
                             : 0});

            // 4b.3. Let's generate spawn points for this encounter
            size_t current_enemy_i = 0;
            bool no_special_enemies = true;
            bool first_enemy_was_a_big_textured_guy = false;
            std::map<pe2_enemy_movement_t, SpawnZones> allowed_zones;
            bool from_water_to_ground = false;
            bool has_cloak_enemies = false;
            for (auto &&entry : enemy_count_map) {
                pe2_enemy_t enemy = enemies_map.at(entry.first);
                if (config.flags.cloak_golem_mult && (enemy.id == 0x16 || enemy.id == 0x27)) {
                    enemy = one_slot_cloak_golems_map.at(enemy.id);
                }
                if (config.flags.cloak_golem_mult) {
                    has_cloak_enemies = has_cloak_enemies || enemy.id == 0x16 || enemy.id == 0x27;
                }
                if (room.forbid_ambush_encounters && enemy.ambush_type == ENEMY_AERIAL_AMBUSH) {
                    enemy.ambush_type = ENEMY_NO_AMBUSH;
                }
                const auto &spawnZones = cachedSpawningZones(allowed_zones, room, enemy);
                auto zones = spawnZones.allowed;
                auto dead_zones = spawnZones.forbidden;
                from_water_to_ground = spawnZones.fromWaterToGround;
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - spawning zones have been prepared.");

                // Remove one special enemy because we will substitute it with a dummy monster with additional
                // resources later
                if (no_special_enemies && enemy.res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES &&
                    room.encounters[i].mask != domain::encounter::FORBID_SLOT_3_ENEMIES) {
                    if (std::find(room.allowed_additional_enemies.begin(),
                                  room.allowed_additional_enemies.end(),
                                  enemy.id) == room.allowed_additional_enemies.end()) {
                        logger->log("[INF] [Randomizer] [EnemyRandomizer] - we found an enemy that needs "
                                    "additional resources.");
                        no_special_enemies = false;
                        if (!internal && entry.second > 1) {
                            entry.second--;
                        }
                    }
                }
                std::vector<utils::geometry_works::Circle> spawn_points =
                    utils::geometry_works::arrangeSpawnPoints(
                        entry.second, enemy.radius, zones, dead_zones, gen,
                        100 // How much retries before forcing ignoring self collision
                    );
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - spawn points for ",
                            getEnemyName(enemy.id), " were calculated.");

                if (spawn_points.begin() == spawn_points.end()) {
                    logger->log("[ERR] [Randomizer] - spawn points list is empty!");
                    // Trying to at least spawn one enemy at 0 0 0
                    spawn_points.push_back(
                        utils::geometry_works::Circle(utils::geometry_works::Point(0, 0, 0), 255));
                }

                // 4b.4. Let's write these enemy descriptors as many times as needed
                bool ambush_flag = false;
                bool ambush_was_chosen = false;
                for (size_t k = 0; k < spawn_points.size(); k++) {
                    std::vector<char> enemy_record = buildEnemyEncounterRecordDescriptor(
                        spawn_points[k], enemy, from_water_to_ground, ambush_flag, ambush_was_chosen,
                        room.environment.ceiling_height, current_enemy_i, first_enemy_was_a_big_textured_guy,
                        has_cloak_enemies);
                    descriptors.push_back(enemy_record);
                    offset += ENEMY_RECORD_DESCRIPTOR_SIZE;
                }
                if (enemy.texture_size == ENEMY_TEX_BIG) {
                    first_enemy_was_a_big_textured_guy = true;
                }
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - enemy encounter record descriptors were "
                            "created.");
                // 4b.5 Apply specific pathways and spawn zones for Slenders and Golems
                if (enemy.id == 0x03 && room.encounters[i].mask != domain::encounter::FORBID_SLOT_3_ENEMIES) {
                    if (first_slender) {
                        createWalkingPathForSlender(zones, dead_zones, room.id, stage_num);
                        first_slender = false;
                        logger->log("[INF] [Randomizer] [EnemyRandomizer] - special brain stingers code were "
                                    "modified for this encounter to work.");
                    }
                }
                if ((enemy.id == 0x16 || enemy.id == 0x27) &&
                    room.encounters[i].mask != domain::encounter::FORBID_SLOT_3_ENEMIES) {
                    if (first_cloak_golem) {
                        createSpawnZoneForPurpleGolem(zones, dead_zones, room.id, stage_num);
                        first_cloak_golem = false;
                        logger->log("[INF] [Randomizer] [EnemyRandomizer] - special cloak golems code were "
                                    "modified for this encounter to work.");
                    }
                }
                current_enemy_i++;
            }
            // 4b.6. Add dummy monster descriptor if we need to load special resources
            if (!no_special_enemies) {
                descriptors.push_back(recordDescriptorToBytesVector({.id = 0x09}));
                offset += ENEMY_RECORD_DESCRIPTOR_SIZE;
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - the dummy monster descriptor has been "
                            "written to a record descriptors vector.");
            }
            current_enemy_i++;
            descriptors.push_back(ENEMY_RECORD_DESCRIPTOR_LIST_DELIMITER);
            offset += ENEMY_RECORD_DESCRIPTOR_SIZE;
            // 4b.7. Let's update resource descriptors in room file
            // First let's find out if we need to extend the file
            std::vector<std::vector<char>> resource_descs{};
            for (auto &&entry : enemy_slots) {
                if (entry.second != NO_ENEMY_ID && entry.second != DUMMY_MONSTER_ID) {
                    pe2_enemy_t enemy = enemies_map.at(entry.second);
                    if (config.flags.cloak_golem_mult && (enemy.id == 0x16 || enemy.id == 0x27)) {
                        enemy = one_slot_cloak_golems_map.at(enemy.id);
                    }
                    pe2_enemy_resource_desc_t resource_desc{
                        .id = enemy.id,
                        .file_id = enemy.file_id,
                        .slot_val = enemy.available_files[entry.first].first,
                        .file_table_addr = enemy.available_files[entry.first].second,
                    };
                    resource_descs.push_back(resourceDescriptorToBytesVector(resource_desc));
                } else if (entry.second == DUMMY_MONSTER_ID) {
                    // We should add dummy monster data descriptor BEFORE the actual enemies data descriptors
                    // in order to prevent it from overriding the texture of relevant enmemies by butterfly
                    // texture (it will be loaded anyway)
                    resource_descs.insert(resource_descs.begin(), ENEMY_DUMMY_DATA);
                }
            }
            logger->log("[INF] [Randomizer] [EnemyRandomizer] - enemy data descriptors were created.");
            resource_descs.push_back(ENEMY_RESOURCE_DESCRIPTOR_LIST_DELIMITER);
            if (enemy_slots.size() > 1) {
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - we have to write data descriptors to the "
                            "end of the file, because they won't fit original place.");
                // ok, if we have to extend the file, than lets find the point at the end of the file that is
                // suitable for that
                room_file.seekg(0, std::ios::end);
                size_t room_file_offset = room_file.tellg();
                // making sure that we ar writing to a place that is multiple of 16
                // TODO: change 16 to 4?
                if (room_file_end_flag) {
                    room_file.seekp(room_file_offset);
                    room_file_end_flag = false;
                    while (room_file_offset % 0x10 != 0) {
                        room_file_offset++;
                        next_file_bias++;
                        char zero = 0x00;
                        room_file.write(reinterpret_cast<char *>(&zero), 1);
                    }
                }
                logger->log(
                    "[INF] [Randomizer] [EnemyRandomizer] - we have found the proper end of the file.");
                // updating new address to resources descriptors for this encounter
                utils::file_works::writeFourBytesLittleEndian(
                    room_file, ROOM_INFO_FILE_LOAD_ADDRESS + room_file_offset,
                    room.encounters[i].enemy_record_descriptor_list_addr_offset + 4);
                logger->log(
                    "[INF] [Randomizer] [EnemyRandomizer] - new data descriptors address was written.");

                // TODO: update header (out of this cycle)
                // TODO: update consequent headers (out of this cycle)
                for (auto it = enemy_slots.begin(); it != enemy_slots.end();) {
                    next_file_bias += 12;
                    it++;
                }
                next_file_bias += 12;

                utils::file_works::writeVectorsToFile(room_file, room_file_offset, resource_descs);
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - data descriptors were written.");
            } else {
                // or just write resource as it is
                utils::file_works::writeVectorsToFile(
                    room_file, room.encounters[i].enemy_resources_desciptors_list_offset, resource_descs);
                logger->log("[INF] [Randomizer] [EnemyRandomizer] - luckily we can just write data "
                            "descriptors where they were originally.");
            }
        }
        if (internal) {
            const size_t used = descriptors.size() - descriptor_start;
            if (used > record_budget)
                throw std::runtime_error("Encounter exceeds reserved record capacity");
            enemy_pool_size = record_budget - used;
        }
    }

    if (internal && !room.encounters.empty() &&
        (offset > record_end ||
         descriptors.size() > (record_end - room.encounters_offset) / ENEMY_RECORD_DESCRIPTOR_SIZE))
        throw std::runtime_error("Generated encounters exceed room record bounds");

    if (next_file_bias > 0) {
        size_t next_file_bias_normilized = 0;
        while (next_file_bias_normilized < next_file_bias) {
            next_file_bias_normilized += 32;
        }
        switch (stage_num) {
        case domain::game::STAGE1:
            updateConsequentFileOffset(domain::file::stage1_files_info, level_file, room,
                                       domain::game::STAGE1, next_file_bias_normilized);
            break;
        case domain::game::STAGE2:
            updateConsequentFileOffset(domain::file::stage2_files_info, level_file, room,
                                       domain::game::STAGE2, next_file_bias_normilized);
            break;
        case domain::game::STAGE3:
            updateConsequentFileOffset(domain::file::stage3_files_info, level_file, room,
                                       domain::game::STAGE3, next_file_bias_normilized);
            break;
        case domain::game::STAGE4:
            updateConsequentFileOffset(domain::file::stage4_files_info, level_file, room,
                                       domain::game::STAGE4, next_file_bias_normilized);
            break;
        case domain::game::STAGE5:
            updateConsequentFileOffset(domain::file::stage5_files_info, level_file, room,
                                       domain::game::STAGE5, next_file_bias_normilized);
            break;
        default:
            break;
        }
    }

    // Write encounter record descriptors to room file if they should be there
    if (internal) {
        utils::file_works::writeVectorsToFile(room_file, room.encounters_offset, descriptors);
    }

    // 5. Apply custom cheats for rooms
    for (const auto &pair : room.file_patch_map) {
        utils::file_works::writeByte(room_file, pair.second, pair.first);
    }
}

std::map<uint8_t, size_t>
usecase::enemies::EnemyRandomizer::generateRandomEnemyCount(std::map<uint32_t, uint8_t> &enemy_slots,
                                                            size_t &pool_size, size_t value) {
    std::uniform_real_distribution dis(0.575, 1.725);
    value *= dis(gen);
    const auto actorTypes = std::count_if(enemy_slots.begin(), enemy_slots.end(), [](const auto &entry) {
        return entry.second != NO_ENEMY_ID && entry.second != DUMMY_MONSTER_ID;
    });
    double share = 0;
    if (actorTypes > 1) {
        std::uniform_real_distribution<> split(0.0, 1.0);
        share = split(gen);
    }
    return domain::enemy::calculateEnemyCounts(enemy_slots, pool_size, value, share);
}

void usecase::enemies::EnemyRandomizer::updateEnemiesStats() {
    std::string level_name = getNameFromLevel(domain::game::BASE_LEVEL);
    std::string level_filename = temp_dir_name + getNameFromStage(domain::game::STAGE0) + "/" + level_name +
                                 FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED;

    std::fstream level_file(level_filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!level_file) {
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error opening input file: ", level_filename);
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[EnemyRandomizer] couldn't open level file file for level " + level_name +
                                 ".");
    }

    if (config.enemy_hp != 100) {
        level_file.seekg(ENEMY_HP_MULTIPLICATOR_ADDRESS);
        uint16_t enemy_hp_mode_mults[5];

        level_file.read(reinterpret_cast<char *>(enemy_hp_mode_mults), sizeof(enemy_hp_mode_mults));

        for (size_t i = 0; i < 5; ++i) {
            enemy_hp_mode_mults[i] = static_cast<uint16_t>(enemy_hp_mode_mults[i] * 100 / config.enemy_hp);
        }

        level_file.seekp(ENEMY_HP_MULTIPLICATOR_ADDRESS);

        level_file.write(reinterpret_cast<char *>(enemy_hp_mode_mults), sizeof(enemy_hp_mode_mults));
    }

    if (config.enemy_damage != 100) {
        for (size_t j = 0; j < 4; j++) {
            level_file.seekg(ENEMY_DMG_MULTIPLICATOR_ADDRESS + j * 20);
            uint16_t enemy_dmg_mode_mults[5];

            level_file.read(reinterpret_cast<char *>(enemy_dmg_mode_mults), sizeof(enemy_dmg_mode_mults));

            for (size_t i = 0; i < 5; ++i) {
                enemy_dmg_mode_mults[i] =
                    static_cast<uint16_t>(enemy_dmg_mode_mults[i] * config.enemy_damage / 100);
            }

            level_file.seekp(ENEMY_DMG_MULTIPLICATOR_ADDRESS + j * 20);

            level_file.write(reinterpret_cast<char *>(enemy_dmg_mode_mults), sizeof(enemy_dmg_mode_mults));
        }
    }
}

void usecase::enemies::EnemyRandomizer::allowOneSlotGolems() {
    std::fstream dummy_monster_file(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                        "/enemies/dummy_monster.pe2e.decompr",
                                    std::ios::in | std::ios::out | std::ios::binary);

    if (!dummy_monster_file.is_open()) {
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Couldn't open dummy monster file : ",
                    temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                        "/enemies/dummy_monster.pe2e.decompr");
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[EnemyRandomizer] couldn't open dummy monster file.");
    }

    // Copy the complete live tail before relocating its pointers. Writing only
    // pointers into a zero-expanded file leaves TaskDesc, poses and keyframes
    // absent; a mixed Knight encounter then dispatches callback zero (BUG-018).
    std::ifstream source(temp_dir_name + "STAGE0/enemies/16_knight_golem_slot_1.pe2enm.decompr",
                         std::ios::binary);
    constexpr size_t first = 0x80149c0c - 0x80131e20;
    constexpr size_t count = 0x801541ec - 0x80149c0c;
    std::vector<char> tail(count);
    source.seekg(first);
    if (!source.read(tail.data(), tail.size()))
        throw std::runtime_error("Truncated Knight animation resources");
    dummy_monster_file.seekp(0x80169c0c - 0x80161e20);
    if (!dummy_monster_file.write(tail.data(), tail.size()))
        throw std::runtime_error("Cannot relocate Knight animation resources");

    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173bec, 0x80173bfc - 0x80161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173fd0, 0x00174148 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173ffc, 0x0017414c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8017404c, 0x00174150 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8017409c, 0x00174154 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80174100, 0x00174158 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80171994, 0x00174160 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80171f28, 0x00174164 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80172988, 0x00174168 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173bc4, 0x0017416c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016ac5c, 0x001741c8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b130, 0x001741cc - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b440, 0x001741d0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b61c, 0x001741d4 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016c564, 0x001741d8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016caf8, 0x001741dc - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cdc0, 0x001741e0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cf9c, 0x001741e4 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x801705c4, 0x001741e8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80170ff4, 0x00171994 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8017196c, 0x00171998 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x801705ec, 0x001719a0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x801707c0, 0x001719ac - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80171c2c, 0x00171f28 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80171f00, 0x00171f2c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x801719bc, 0x00171f34 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80171a34, 0x00171f40 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80172460, 0x00172988 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80172960, 0x0017298c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80171f50, 0x00172994 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8017204c, 0x001729a0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x801732e8, 0x00173bc4 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173b9c, 0x00173bc8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x801729b0, 0x00173bd0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80172b54, 0x00173bdc - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016a434, 0x0016ac5c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016ac34, 0x0016ac60 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80169c0c, 0x0016ac68 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80169d80, 0x0016ac74 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016aeb4, 0x0016b130 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b108, 0x0016b134 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016ac84, 0x0016b13c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016acf0, 0x0016b148 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b298, 0x0016b440 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b418, 0x0016b444 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b158, 0x0016b44c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b194, 0x0016b458 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b4c4, 0x0016b61c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b5f4, 0x0016b620 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b468, 0x0016b628 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b480, 0x0016b634 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016bdbc, 0x0016c564 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016c53c, 0x0016c568 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b644, 0x0016c570 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016b794, 0x0016c57c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016c7f4, 0x0016caf8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cad0, 0x0016cafc - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016c58c, 0x0016cb04 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016c5f8, 0x0016cb10 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cc3c, 0x0016cdc0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cd98, 0x0016cdc4 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cb20, 0x0016cdcc - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cb5c, 0x0016cdd8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016ce44, 0x0016cf9c - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cf74, 0x0016cfa0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cde8, 0x0016cfa8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016ce00, 0x0016cfb4 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016eb50, 0x001705c4 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8017059c, 0x001705c8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016cfc4, 0x001705d0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x8016d474, 0x001705dc - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173cc8, 0x00173fac - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173d48, 0x00173fb0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173d98, 0x00173fb4 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173db8, 0x00173fb8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173e08, 0x00173fbc - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173e38, 0x00173fc0 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173eb8, 0x00173fc4 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173ef8, 0x00173fc8 - 0x161e20);
    utils::file_works::writeFourBytesLittleEndian(dummy_monster_file, 0x80173f48, 0x00173fcc - 0x161e20);

    std::ifstream routes(temp_dir_name + "STAGE0/enemies/knight_golem_additional_data.pe2e",
                         std::ios::binary);
    if (!routes)
        throw std::runtime_error("Missing Knight spawn routes");
    std::vector<char> routeData((std::istreambuf_iterator<char>(routes)), {});
    if (routeData.size() % 8 || 0x123cc + routeData.size() + 8 > 0x17b30)
        throw std::runtime_error("Knight spawn routes exceed their RAM region");
    // Explicit sentinel, including the no-Knight/no-Bishop encounter case.
    routeData.resize(routeData.size() + 8, 0);
    dummy_monster_file.seekp(0x123cc);
    if (!dummy_monster_file.write(routeData.data(), routeData.size()))
        throw std::runtime_error("Cannot write Knight spawn routes");

    utils::file_works::writeVectorsToFile(
        dummy_monster_file, 0x7CD0,
        {{'\x0A', '\x00', '\x0A', '\x00', '\x2D', '\x00', '\x02', '\x00', '\x3A', '\x00', '\x02', '\x00',
          '\xE7', '\x03', '\x00', '\x00', '\xF0', '\x9A', '\x16', '\x80', '\x20', '\x03', '\x90', '\x01',
          '\xC4', '\x09', '\x07', '\x64', '\x14', '\x00', '\x00', '\x00', '\x06', '\x00', '\x0B', '\x00',
          '\x10', '\x00', '\x10', '\x00', '\x04', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
          '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00',
          '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x01', '\x00', '\x01', '\x00', '\x01', '\x00',
          '\x01', '\x00', '\x01', '\x00', '\x0A', '\x00', '\x0F', '\x00', '\x14', '\x00', '\x14', '\x00',
          '\x19', '\x00', '\x1E', '\x00', '\x28', '\x00', '\x32', '\x00', '\x19', '\x00', '\x1E', '\x00',
          '\x28', '\x00', '\x32', '\x00', '\x19', '\x00', '\x1E', '\x00', '\x28', '\x00', '\x32', '\x00',
          '\x01', '\x00', '\x01', '\x00', '\x01', '\x00', '\x01', '\x00', '\x02', '\x00', '\x02', '\x00',
          '\x02', '\x00', '\x02', '\x00', '\x02', '\x00', '\x02', '\x00', '\x02', '\x00', '\x02', '\x00',
          '\x02', '\x00', '\x02', '\x00', '\x02', '\x00', '\x02', '\x00', '\x01', '\x00', '\x04', '\x00',
          '\x20', '\x00', '\x05', '\x00', '\x02', '\x00', '\x05', '\x00', '\x15', '\x00', '\x06', '\x00',
          '\x03', '\x00', '\x04', '\x00', '\x23', '\x00', '\x03', '\x00', '\x04', '\x00', '\x03', '\x00',
          '\x0F', '\x00', '\x03', '\x00', '\x05', '\x00', '\x04', '\x00', '\x0E', '\x00', '\x08', '\x00',
          '\x06', '\x00', '\x04', '\x00', '\x13', '\x00', '\x05', '\x00', '\x07', '\x00', '\x05', '\x00',
          '\x05', '\x00', '\x02', '\x00', '\x08', '\x00', '\x05', '\x00', '\x10', '\x00', '\x04', '\x00',
          '\x00', '\x00', '\x00', '\x00'}});

    dummy_monster_file.flush();
    dummy_monster_file.close();

    std::fstream knight_golem_file(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                       "/enemies/16_knight_golem_slot_1.pe2enm.decompr",
                                   std::ios::in | std::ios::out | std::ios::binary);

    if (!knight_golem_file.is_open()) {
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Couldn't open dummy monster file : ",
                    temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                        "/enemies/16_knight_golem_slot_1.pe2enm.decompr");
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[EnemyRandomizer] couldn't open knight golem file.");
    }

    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00132a24 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00132a74 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x001333a0 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00133464 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00133524 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00133654 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00133684 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x001337d8 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00133890 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00133998 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00133b6c - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00133fe8 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x001346a0 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x00136264 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x001374a8 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x001375a0 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x001375d4 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x0013767c - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x001377b8 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(knight_golem_file, 0x8017, 0x57b4);

    utils::file_works::writeFourBytesLittleEndian(knight_golem_file, 0x240bffe0, 0x51A0);
    utils::file_works::writeFourBytesLittleEndian(knight_golem_file, 0x25a30004, 0x526C);

    knight_golem_file.flush();
    knight_golem_file.close();

    std::fstream bishop_golem_file(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                       "/enemies/27_bishop_golem_slot_1.pe2enm.decompr",
                                   std::ios::in | std::ios::out | std::ios::binary);

    if (!bishop_golem_file.is_open()) {
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Couldn't open dummy monster file : ",
                    temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                        "/enemies/27_bishop_golem_slot_1.pe2enm.decompr");
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[EnemyRandomizer] couldn't open bishop golem file.");
    }

    utils::file_works::writeFourBytesLittleEndian(bishop_golem_file, 0x240bffe0, 0x00136fc0 - 0x131e20);
    utils::file_works::writeFourBytesLittleEndian(bishop_golem_file, 0x25a30004, 0x0013708c - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x9d00, 0x00137688 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x9d18, 0x001333a4 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x9d18, 0x00133468 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x9d18, 0x00133658 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x9d18, 0x001337dc - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x9d18, 0x00133894 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x9d2c, 0x001339a0 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x9d34, 0x00136268 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x9d50, 0x001374ac - 0x131e20);

    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x41ec, 0x001375f8 - 0x131e20);
    utils::file_works::writeTwoBytesLittleEndian(bishop_golem_file, 0x8017, 0x001375d4 - 0x131e20);

    utils::file_works::writeVectorsToFile(
        bishop_golem_file, 0x00149d00 - 0x131e20,
        {{'\x00', '\x00', '\x00', '\x00', '\xD0', '\x3F', '\x17', '\x80', '\xFC', '\x3F', '\x17', '\x80',
          '\x4C', '\x40', '\x17', '\x80', '\x9C', '\x40', '\x17', '\x80', '\x00', '\x41', '\x17', '\x80',
          '\x00', '\x00', '\x00', '\x00', '\x94', '\x19', '\x17', '\x80', '\x28', '\x1F', '\x17', '\x80',
          '\x88', '\x29', '\x17', '\x80', '\xC4', '\x3B', '\x17', '\x80', '\x00', '\x00', '\x00', '\x00',
          '\x2C', '\x01', '\x01', '\x00', '\x00', '\x00', '\x01', '\x00', '\x02', '\x00', '\x03', '\x00',
          '\x00', '\x00', '\x01', '\x00', '\x04', '\x00', '\x05', '\x00', '\x01', '\x00', '\x60', '\x00',
          '\x44', '\x83', '\x13', '\x80', '\xD8', '\xDB', '\x13', '\x80', '\x00', '\x00', '\x00', '\x00',
          '\x6C', '\xE2', '\x13', '\x80', '\x24', '\xE7', '\x13', '\x80', '\x30', '\xEF', '\x13', '\x80',
          '\x68', '\xFB', '\x13', '\x80', '\x1C', '\x0C', '\x14', '\x80', '\xC8', '\x17', '\x14', '\x80',
          '\x40', '\x58', '\x14', '\x80', '\x64', '\x5D', '\x14', '\x80', '\xAC', '\x67', '\x14', '\x80',
          '\x9C', '\x74', '\x14', '\x80', '\xD8', '\x8D', '\x14', '\x80', '\xE8', '\x9B', '\x14', '\x80',
          '\x5C', '\xAC', '\x16', '\x80', '\x30', '\xB1', '\x16', '\x80', '\x40', '\xB4', '\x16', '\x80',
          '\x1C', '\xB6', '\x16', '\x80', '\x64', '\xC5', '\x16', '\x80', '\xF8', '\xCA', '\x16', '\x80',
          '\xC0', '\xCD', '\x16', '\x80', '\x9C', '\xCF', '\x16', '\x80', '\xC4', '\x05', '\x17', '\x80',
          '\x01', '\x00', '\x01', '\x00'}});

    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x001375a0 - 0x131e20,
                                          {{'\x17', '\x80', '\x03', '\x3c'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x001375a8 - 0x131e20,
                                          {{'\x00', '\x9b', '\x63', '\x24'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x001377bc - 0x131e20,
                                          {{'\x17', '\x80', '\x04', '\x3c'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x001377c4 - 0x131e20,
                                          {{'\xf0', '\x9a', '\x84', '\x24'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x00133b6c - 0x131e20,
                                          {{'\x17', '\x80', '\x04', '\x3c'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x00133b78 - 0x131e20,
                                          {{'\x1c', '\x9b', '\x84', '\x24'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x00133524 - 0x131e20,
                                          {{'\x17', '\x80', '\x03', '\x3c'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x00133530 - 0x131e20,
                                          {{'\x10', '\x9b', '\x63', '\x24'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x00132a74 - 0x131e20,
                                          {{'\x17', '\x80', '\x03', '\x3c'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x00132a80 - 0x131e20,
                                          {{'\x3c', '\x9b', '\x63', '\x24'}});

    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x001346a0 - 0x131e20,
                                          {{'\x17', '\x80', '\x04', '\x3c', '\xf0', '\x9a', '\x84', '\x24'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x00133fe8 - 0x131e20,
                                          {{'\x17', '\x80', '\x04', '\x3c', '\xf0', '\x9a', '\x84', '\x24'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x00133684 - 0x131e20,
                                          {{'\x17', '\x80', '\x04', '\x3c', '\xf0', '\x9a', '\x84', '\x24'}});
    utils::file_works::writeVectorsToFile(bishop_golem_file, 0x00132a24 - 0x131e20,
                                          {{'\x17', '\x80', '\x03', '\x3c', '\x5c', '\x9b', '\x63', '\x24'}});

    bishop_golem_file.flush();
    bishop_golem_file.close();

    std::fstream game_lib_file(temp_dir_name + getNameFromStage(domain::game::STAGE0) +
                                   "/00_base.pe2lvl.decompr",
                               std::ios::in | std::ios::out | std::ios::binary);

    if (!game_lib_file.is_open()) {
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Couldn't open dummy monster file : ",
                    temp_dir_name + getNameFromStage(domain::game::STAGE0) + "/00_base.pe2lvl.decompr");
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[EnemyRandomizer] couldn't open game lib file.");
    }

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24020139, 0x00099e74 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24020139, 0x0009a1c8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24020139, 0x0009bb50 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x24020139, 0x0009cd08 - 0x93800);

    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x288200a0, 0x0009aef8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x288200a0, 0x0009b25c - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x288200a0, 0x0009b8e8 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x288200a0, 0x0009b9e4 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x288200a0, 0x0009bae0 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x288200a0, 0x0009c73c - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x288200a0, 0x0009c838 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x288200a0, 0x0009c934 - 0x93800);
    utils::file_works::writeFourBytesLittleEndian(game_lib_file, 0x288200a0, 0x0009ca84 - 0x93800);

    game_lib_file.flush();
    game_lib_file.close();
}

bool usecase::enemies::EnemyRandomizer::soundFits(const std::map<uint32_t, uint8_t> &slots) const {
    if (!checkSoundBudget)
        return true; // Synthetic room-writer fixtures only.
    domain::enemy::audio::Banks banks;
    for (auto [slot, id] : slots) {
        if (id == NO_ENEMY_ID || id == DUMMY_MONSTER_ID)
            continue;
        auto enemy = enemies_map.at(id);
        if (config.flags.cloak_golem_mult && (id == KNIGHT_GOLEM_ID || id == BISHOP_GOLEM_ID))
            enemy = one_slot_cloak_golems_map.at(id);
        domain::enemy::audio::merge(banks,
                                    soundCatalog.enemy(enemy.file_id, enemy.available_files.at(slot).first));
    }
    return domain::enemy::audio::fits(roomSoundBanks, banks);
}

void usecase::enemies::EnemyRandomizer::randomizeEnemiesInStage(
    pe2_randomizer_context &context, domain::game::pe2_stage_name_t stage,
    domain::game::pe2_level_name_t level,
    std::vector<domain::encounter::pe2_room_encounters_t> &room_encounters,
    domain::encounter::pe2_encounters_list_segment_t segment) {
    const auto roomAudio =
        domain::enemy::audio::Catalog::loadRooms(temp_dir_name + getNameFromStage(stage) + ".CDF");
    checkSoundBudget = true;
    size_t enemy_pool_size;
    size_t offset = segment.from;
    std::string level_name = getNameFromLevel(level);
    std::string level_filename = temp_dir_name + getNameFromStage(domain::game::STAGE0) + "/" + level_name +
                                 FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED;
    std::vector<std::vector<char>>
        descriptors_to_write{}; // vector of encounter records that will be written to file

    // 1. First need to count enemies pool

    logger->log("[INF] [Randomizer] [EnemyRandomizer] starting randomizing new level: ",
                getNameFromStage(stage));

    // If encounter records are stored in level info file let's count the enemies pool size available
    if (!segment.internal) {
        enemy_pool_size = calculatePoolSize(level_filename, room_encounters, segment);
        logger->log("[INF] [Randomizer] [EnemyRandomizer] enemy pool size for level ", level_name, " = ",
                    enemy_pool_size);
    }

    // 2. Then For each room let's randomize encounter
    std::fstream level_file(level_filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!level_file) {
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error opening input file: ", level_filename);
        logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error code : () ", std::strerror(errno), " )");
        throw std::runtime_error("[EnemyRandomizer] couldn't open level file file for level " + level_name +
                                 ".");
    }

    // Now we are doing only for NRS modes
    domain::encounter::pe2_enc_rewards_segment_t rewards_segment =
        domain::encounter::rewards_segment_map_by_level.at(level).first;
    domain::encounter::pe2_enc_rewards_segment_t rewards_segment_bounty =
        domain::encounter::rewards_segment_map_by_level.at(level).second;
    std::vector<domain::encounter::pe2_enc_reward_descriptor_t> encounter_rewards{};

    // std::vector<domain::encounter::pe2_room_encounters_t> room_encounters_shuffled = room_encounters;
    // std::shuffle(room_encounters_shuffled.begin(), room_encounters_shuffled.end(), gen);
    // 3. Iterate through every room that is supported in randomizer:
    for (size_t i = 0; i < room_encounters.size(); i++) {
        std::string room_filename = temp_dir_name + getNameFromStage(stage) + "/" +
                                    getNameFromRoom(room_encounters[i].name) + FORMAT_ROOM_FILE +
                                    FORMAT_DECOMPRESSED;
        std::fstream room_file(room_filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!room_file) {
            logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error opening input file: ", room_filename);
            logger->log("[ERR] [Randomizer] [EnemyRandomizer] Error code : () ", std::strerror(errno), " )");
            throw std::runtime_error("[EnemyRandomizer] couldn't open room file file for room " +
                                     room_filename + ".");
        }
        // If encounters list is inside room files (domain::game::STAGE4) let's update the offset
        if (segment.internal) {
            offset = room_encounters[i].encounters_offset;
            // If encounters segment is separated in minisemgents in room files (domain::game::STAGE4) let's
            // create new pool_size value for every room manualy Internal capacity and unchanged lists are
            // reserved from the original room data by randomizeEnemiesInRoom.
            enemy_pool_size = 0;
            descriptors_to_write = {};
        }
        const bool randomized =
            std::any_of(room_encounters[i].encounters.begin(), room_encounters[i].encounters.end(),
                        [](const auto &enc) { return enc.mask != domain::encounter::UNRANDOMIZED; });
        roomSoundBanks = randomized ? roomAudio.at(room_encounters[i].id) : domain::enemy::audio::Banks{};
        roomModelCapacity =
            randomized ? domain::enemy::models::roomCapacity(level_file, stage, room_encounters[i].id) : 0;
        logger->log("[INF] [Audio] room ", getNameFromRoom(room_encounters[i].name),
                    " enemy SPU capacity: ", domain::enemy::audio::enemyCapacity(roomSoundBanks));
        randomizeEnemiesInRoom(level_file, room_file, room_encounters[i], descriptors_to_write,
                               encounter_rewards, offset, enemy_pool_size, segment.internal, stage);
    }
    std::sort(encounter_rewards.begin(), encounter_rewards.end(), domain::encounter::compareByValue);
    if (rewards_segment.num < encounter_rewards.size()) {
        // Erase all elements after the ith element
        encounter_rewards.erase(encounter_rewards.begin() + rewards_segment.num, encounter_rewards.end());
    }

    auto encounter_rewards_bounty(encounter_rewards);
    if (rewards_segment_bounty.num < encounter_rewards_bounty.size()) {
        encounter_rewards_bounty.erase(encounter_rewards_bounty.begin() + rewards_segment_bounty.num,
                                       encounter_rewards_bounty.end());
    }

    logger->log("[INF] [Randomizer] [EnemyRandomizer] Reward descriptors for level ",
                getNameFromLevel(level));
    for (auto it = encounter_rewards.begin(); it != encounter_rewards.end(); it++) {
        logger->log("[INF] [Randomizer] [EnemyRandomizer] - ", (int)it->encounter_id, " - ", (int)it->room_id,
                    " - ", (int)it->stage_id, " - ", (int)it->value);
    }
    context.reward_slots.insert({level, encounter_rewards});
    context.bounty_reward_slots.insert({level, encounter_rewards_bounty});

    // Write encounter record descriptors to level file if they should be there
    if (!segment.internal) {
        utils::file_works::writeVectorsToFile(level_file, segment.from, descriptors_to_write);
    }
}

// Candidate selection uses one actor of each species, then count fitting uses
// the same resource envelope for the final multiplicities.
bool usecase::enemies::EnemyRandomizer::modelsFit(const std::map<uint32_t, uint8_t> &slots) const {
    if (!roomModelCapacity)
        return true; // isolated legacy test fixtures
    std::map<uint8_t, size_t> counts;
    for (auto [slot, id] : slots)
        if (id != NO_ENEMY_ID && id != DUMMY_MONSTER_ID)
            counts[id] = 1;
    return domain::enemy::models::fits(roomModelCapacity, counts);
}
