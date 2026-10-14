// Local, bounded-memory enumeration of the production selector. No ROM assets
// are distributed with this tool. Outputs TSV, not one ROM per combination.
#include "domain/enemy/entities/DummyResources.hpp"
#include "domain/enemy/entities/EnemyCounts.hpp"
#include "domain/enemy/usecases/EnemyRandomizer.hpp"
#include "enemy_matrix_counts.hpp"
#include "utils/lzss/Compressor.hpp"
#include <cmath>
#include <iostream>
#include <set>
using namespace domain::enemy;
namespace usecase::enemies {
struct EnemyRandomizerTestAccess {
    static void prepare(EnemyRandomizer &r) {
        r.allowOneSlotGolems();
        finalizeDummyResources(r.temp_dir_name + "STAGE0/enemies/dummy_monster.pe2e.decompr");
        validateCloakResources(std::filesystem::path(r.temp_dir_name) / "STAGE0/enemies");
    }
    static void prepareComplete(EnemyRandomizer &r, bool cloak) {
        r.updateSpecialEnemiesCode();
        if (cloak)
            r.allowOneSlotGolems();
        finalizeDummyResources(r.temp_dir_name + "STAGE0/enemies/dummy_monster.pe2e.decompr");
        if (cloak)
            validateCloakResources(std::filesystem::path(r.temp_dir_name) / "STAGE0/enemies");
    }
    struct Branch {
        std::vector<uint8_t> options;
    };
    static void enumerate(EnemyRandomizer &r, size_t budget,
                          const domain::encounter::pe2_room_encounters_t &room,
                          const domain::encounter::pe2_encounter_t &enc,
                          const std::function<void(const std::map<uint32_t, uint8_t> &)> &emit,
                          std::vector<uint8_t> tape = {}) {
        size_t cursor = 0;
        try {
            const auto result = r.generateRandomEnemyIds(budget, room, enc, [&](const auto &candidates, int) {
                if (cursor == tape.size())
                    throw Branch{candidates};
                auto id = tape[cursor++];
                if (std::find(candidates.begin(), candidates.end(), id) == candidates.end())
                    throw std::runtime_error("Nondeterministic selection tree");
                return id;
            });
            emit(result);
        } catch (const Branch &b) {
            for (auto id : b.options) {
                auto next = tape;
                next.push_back(id);
                enumerate(r, budget, room, enc, emit, next);
            }
        }
    }
    // All allowed subset pools reduce to singleton/pair witnesses: production
    // chooses at most two distinct species. A witness preserves the preferred
    // pool vs fallback decision and every second-species exclusion.
    static void exhaustive(EnemyRandomizer &r, const std::filesystem::path &root) {
        r.soundCatalog.loadEnemies(root / "STAGE0.CDF");
        r.checkSoundBudget = true;
        const auto all = r.config.randomized_enemies_pool;
        std::map<std::tuple<std::map<uint32_t, uint8_t>, size_t, uint32_t>, size_t> countKeys;
        size_t nextKey = 0;
        for (auto [level, rooms] : domain::encounter::encounters_per_level_map) {
            const auto sounds = audio::Catalog::loadRooms(root / ("STAGE" + std::to_string(level) + ".CDF"));
            for (const auto &room : rooms)
                for (const auto &enc : room.encounters) {
                    if (enc.mask == domain::encounter::UNRANDOMIZED)
                        continue;
                    if (const char *x = std::getenv("PE2_MATRIX_STAGE"); x && std::stoi(x) != int(level))
                        continue;
                    if (const char *x = std::getenv("PE2_MATRIX_ROOM"); x && std::stoi(x) != int(room.id))
                        continue;
                    r.roomSoundBanks = sounds.at(room.id);
                    std::fstream map(
                        root / ("STAGE0/" + domain::game::getNameFromLevel(level) + ".pe2lvl.decompr"),
                        std::ios::in | std::ios::binary);
                    r.roomModelCapacity = models::roomCapacity(map, int(level), room.id);
                    std::set<std::map<uint32_t, uint8_t>> plans;
                    for (size_t a = 0; a < all.size(); ++a)
                        for (size_t b = a; b < all.size(); ++b) {
                            r.config.randomized_enemies_pool = {all[a]};
                            if (a != b)
                                r.config.randomized_enemies_pool.push_back(all[b]);
                            for (size_t pool : {size_t(0), size_t(10)}) {
                                try {
                                    enumerate(r, pool, room, enc,
                                              [&](const auto &plan) { plans.insert(plan); });
                                } catch (const std::runtime_error &e) {
                                    if (std::string(e.what()).find("No selected enemy fits") != 0)
                                        throw;
                                }
                            }
                        }
                    // Record-budget fitting can discard a species without moving
                    // the survivor to its usual first-choice file slot. Cover those
                    // residual slot assignments too, including their dummy lifetime.
                    const auto selectedPlans = plans;
                    for (const auto &original : selectedPlans)
                        for (auto [slot, id] : original) {
                            if (id == DUMMY_MONSTER_ID)
                                continue;
                            std::map<uint32_t, uint8_t> single{{slot, id}};
                            if (enemies_map.at(id).res_type == ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES &&
                                original.count(ENEMY_SLOT_3_ADDRESS) &&
                                original.at(ENEMY_SLOT_3_ADDRESS) == DUMMY_MONSTER_ID)
                                single[ENEMY_SLOT_3_ADDRESS] = DUMMY_MONSTER_ID;
                            plans.insert(single);
                        }
                    for (const auto &plan : plans) {
                        auto key = std::make_tuple(plan, enc.encounter_value, r.roomModelCapacity);
                        auto found = countKeys.find(key);
                        size_t index;
                        if (found == countKeys.end()) {
                            index = nextKey++;
                            countKeys[key] = index;
                            std::set<std::map<uint8_t, size_t>> counts;
                            // Explore all count-changing integer partition thresholds.
                            // +/-2 protects C++ double-to-size_t truncation at decimal
                            // share boundaries; 12 is the production descriptor cap.
                            for (double shareCase : {0.0, 0.5, 0.9}) {
                                auto values =
                                    matrix_tests::scaledCountValues(plan, enc.encounter_value, shareCase);
                                for (auto value : values)
                                    for (size_t pool = 0; pool <= 13; ++pool) {
                                        auto remaining = pool;
                                        auto actual = calculateEnemyCounts(plan, remaining, value, shareCase);
                                        // Match the production dummy's replaceable actor.
                                        for (auto &[id, n] : actual)
                                            if (enemies_map.at(id).res_type ==
                                                    ENEMY_RES_TYPE_NEEDS_ADDITIONAL_RESOURCES &&
                                                plan.count(ENEMY_SLOT_3_ADDRESS) &&
                                                plan.at(ENEMY_SLOT_3_ADDRESS) == DUMMY_MONSTER_ID) {
                                                if (n > 1)
                                                    --n;
                                                break;
                                            }
                                        if (std::all_of(actual.begin(), actual.end(),
                                                        [](auto p) { return p.second > 0; })) {
                                            models::fit(r.roomModelCapacity, actual);
                                            counts.insert(actual);
                                        }
                                    }
                            }
                            std::cout << "counts\t" << r.config.flags.cloak_golem_mult << '\t' << index;
                            for (auto &c : counts) {
                                std::cout << '\t';
                                bool first = true;
                                for (auto [id, n] : c) {
                                    if (!first)
                                        std::cout << ',';
                                    first = false;
                                    std::cout << std::hex << int(id) << std::dec << ':' << n;
                                }
                            }
                            std::cout << '\n';
                        } else
                            index = found->second;
                        std::cout << "plan2\t" << r.config.flags.cloak_golem_mult << '\t' << level << '\t'
                                  << int(room.id) << '\t' << int(enc.id) << '@'
                                  << enc.enemy_record_descriptor_list_addr_offset << '\t' << index;
                        for (auto [slot, id] : plan)
                            std::cout << '\t' << std::hex << slot << ':' << int(id) << std::dec;
                        std::cout << '\n';
                    }
                    // Diagnostic trace is expendable; enumeration never accumulates
                    // a log proportional to the full Cartesian product.
                    std::filesystem::resize_file(root / "enemy-matrix.log", 0);
                }
        }
        r.config.randomized_enemies_pool = all;
    }
    static void matrix(EnemyRandomizer &r, const std::filesystem::path &root) {
        r.soundCatalog.loadEnemies(root / "STAGE0.CDF");
        r.checkSoundBudget = true;
        for (auto [level, rooms] : domain::encounter::encounters_per_level_map) {
            const auto sounds = audio::Catalog::loadRooms(root / ("STAGE" + std::to_string(level) + ".CDF"));
            for (const auto &room : rooms) {
                if (room.encounters.empty())
                    continue;
                for (const auto &enc : room.encounters) {
                    if (enc.mask == domain::encounter::UNRANDOMIZED)
                        continue;
                    r.roomSoundBanks = sounds.at(room.id);
                    std::fstream map(
                        root / ("STAGE0/" + domain::game::getNameFromLevel(level) + ".pe2lvl.decompr"),
                        std::ios::in | std::ios::binary);
                    r.roomModelCapacity = models::roomCapacity(map, int(level), room.id);
                    std::set<std::map<uint32_t, uint8_t>> unique;
                    for (size_t budget : {size_t(9), size_t(10)})
                        enumerate(r, budget, room, enc, [&](const auto &plan) { unique.insert(plan); });
                    for (const auto &plan : unique) {
                        std::cout << "plan\t" << r.config.flags.cloak_golem_mult << '\t' << level << '\t'
                                  << int(room.id) << '\t' << int(enc.id) << '@'
                                  << enc.enemy_record_descriptor_list_addr_offset;
                        for (auto [slot, id] : plan)
                            std::cout << '\t' << std::hex << slot << ':' << int(id) << std::dec;
                        std::cout << '\n';
                    }
                }
            }
        }
    }
};
} // namespace usecase::enemies
int main(int argc, char **argv) {
    try {
        if (argc != 3)
            throw std::runtime_error("Usage: pe2_enemy_matrix --prepare|--matrix|--metadata WORK_ROOT");
        auto log = PE2Logger::getInstance((std::filesystem::path(argv[2]) / "enemy-matrix.log").string());
        domain::inner_config_t config{};
        for (auto [id, e] : enemies_map)
            config.randomized_enemies_pool.push_back(id);
        if (std::string(argv[1]) == "--parameters") {
            for (auto [id, e] : enemies_map) {
                std::cout << int(id) << '\t' << int(e.texture_size) << '\t' << int(e.move_type) << '\t'
                          << int(e.ambush_type) << '\t' << int(e.ambush_value) << '\t';
                for (auto p : e.first_param)
                    if (p.first > 0)
                        std::cout << int(p.second) << ',';
                std::cout << '\t';
                for (auto p : e.second_param)
                    if (p.first > 0)
                        std::cout << int(p.second) << ',';
                std::cout << '\n';
            }
            return 0;
        }
        if (std::string(argv[1]) == "--rooms") {
            for (auto [level, rooms] : domain::encounter::encounters_per_level_map)
                for (const auto &room : rooms)
                    for (const auto &enc : room.encounters) {
                        if (enc.mask == domain::encounter::UNRANDOMIZED)
                            continue;
                        std::cout << level << '\t' << int(room.id) << '\t' << getNameFromRoom(room.name)
                                  << '\t' << int(enc.id) << '\t'
                                  << enc.enemy_record_descriptor_list_addr_offset << '\n';
                    }
            return 0;
        }
        if (std::string(argv[1]) == "--prepare-mode0" || std::string(argv[1]) == "--prepare-mode1") {
            usecase::enemies::EnemyRandomizer r(log, std::mt19937(0), std::string(argv[2]) + "/", config);
            usecase::enemies::EnemyRandomizerTestAccess::prepareComplete(r, std::string(argv[1]) ==
                                                                                "--prepare-mode1");
            std::atomic<bool> cancel(false);
            utils::lzss::Compressor compressor(cancel, log);
            for (const auto &entry :
                 std::filesystem::recursive_directory_iterator(std::filesystem::path(argv[2]) / "STAGE0"))
                if (entry.path().extension() == ".decompr")
                    compressor.compress(entry.path().string(), entry.path().string() + ".lzss");
            return 0;
        }
        if (std::string(argv[1]) == "--prepare") {
            usecase::enemies::EnemyRandomizer r(log, std::mt19937(0), std::string(argv[2]) + "/", config);
            usecase::enemies::EnemyRandomizerTestAccess::prepare(r);
            return 0;
        }
        for (bool cloak : {false, true}) {
            config.flags.cloak_golem_mult = cloak;
            if (std::string(argv[1]) == "--metadata") {
                for (auto [id, e] : enemies_map) {
                    if (cloak && one_slot_cloak_golems_map.count(id))
                        e = one_slot_cloak_golems_map.at(id);
                    for (auto [slot, f] : e.available_files)
                        std::cout << "actor\t" << cloak << '\t' << std::hex << int(id) << '\t' << slot << '\t'
                                  << f.second << std::dec << '\t'
                                  << (f.first + 1) * 100000 + int(e.file_id) * 100 << '\n';
                }
            } else if (std::string(argv[1]) == "--exhaustive") {
                usecase::enemies::EnemyRandomizer r(log, std::mt19937(0), std::string(argv[2]) + "/", config);
                usecase::enemies::EnemyRandomizerTestAccess::exhaustive(r, argv[2]);
            } else if (std::string(argv[1]) == "--matrix") {
                usecase::enemies::EnemyRandomizer r(log, std::mt19937(0), std::string(argv[2]) + "/", config);
                usecase::enemies::EnemyRandomizerTestAccess::matrix(r, argv[2]);
            } else
                throw std::runtime_error("Unknown matrix command");
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
