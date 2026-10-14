#pragma once

#include "domain/config/innerconfig.hpp"
#include "domain/encounter/entities/Encounter.hpp"
#include "domain/encounter/entities/Reward.hpp"
#include "domain/enemy/entities/Enemy.hpp"
#include "domain/enemy/entities/ModelBudget.hpp"
#include "domain/enemy/entities/SoundBudget.hpp"
#include "domain/file/entities/PE2File.hpp"
#include "domain/game/entities/Stage.hpp"
#include "domain/randomizer/usecases/IFeatureRandomizer.hpp"
#include "utils/FileWorks/FileWorks.hpp"
#include "utils/GeometryWorks/GeometryWorks.hpp"
#include "utils/Logger.hpp"
#include "utils/VectorWorks/VectorWorks.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <random>
#include <vector>

#define POOL_ENEMY_THRESCHOLD 9
#define MAX_ENEMY_DESCRIPTORS 12
#define NORMAL_ENEMY_DESCRIPTOR_WEIGHT 3
#define SMALLER_ENEMY_DESCRIPTOR_WEIGHT 2
#define SMALL_ENEMY_DESCRIPTOR_WEIGHT 1
#define MULTI_ENIMIES_ENCOUNTER_PROBABILITY 1.0
#define ENEMY_AMBUSH_PROBABILITY 0.5

namespace usecase::enemies {
class EnemyRandomizer : public IFeatureRandomizer {
  public:
    EnemyRandomizer(std::shared_ptr<PE2Logger> _logger, std::mt19937 _gen, std::string _temp_dir_name,
                    domain::inner_config_t _config)
        : config(_config), logger(_logger), temp_dir_name(_temp_dir_name), gen(_gen) {};
    pe2_randomizer_context randomize(pe2_randomizer_context);

  private:
    friend struct EnemyRandomizerTestAccess;
    void updateSpecialEnemiesCode();
    bool soundFits(const std::map<uint32_t, uint8_t> &) const;
    domain::enemy::audio::Catalog soundCatalog;
    domain::enemy::audio::Banks roomSoundBanks;
    bool checkSoundBudget = false;
    uint32_t roomModelCapacity = 0;
    bool modelsFit(const std::map<uint32_t, uint8_t> &) const;
    void updateConsequentFileOffset(const std::vector<domain::file::pe2file_t> &, std::fstream &,
                                    domain::encounter::pe2_room_encounters_t &,
                                    domain::game::pe2_stage_name_t, size_t);
    size_t calculatePoolSize(std::string &, std::vector<domain::encounter::pe2_room_encounters_t> &,
                             domain::encounter::pe2_encounters_list_segment_t);
    size_t rewriteMaskedEncounter(std::fstream &, std::fstream &, std::vector<std::vector<char>> &,
                                  domain::encounter::pe2_room_encounters_t, size_t, size_t);
    void randomizeEnemiesInRoom(std::fstream &, std::fstream &, domain::encounter::pe2_room_encounters_t,
                                std::vector<std::vector<char>> &,
                                std::vector<domain::encounter::pe2_enc_reward_descriptor_t> &, size_t &,
                                size_t &, bool, int);
    std::map<uint8_t, size_t> generateRandomEnemyCount(std::map<uint32_t, uint8_t> &, size_t &, size_t);
    void fitEnemyCounts(std::map<uint32_t, uint8_t> &, std::map<uint8_t, size_t> &,
                        const domain::encounter::pe2_room_encounters_t &,
                        const domain::encounter::pe2_encounter_t &, size_t);
    std::map<uint32_t, uint8_t>
    generateRandomEnemyIds(size_t, domain::encounter::pe2_room_encounters_t,
                           domain::encounter::pe2_encounter_t,
                           const std::function<uint8_t(const std::vector<uint8_t> &, int)> &choose = {});
    void randomizeEnemiesInStage(pe2_randomizer_context &, domain::game::pe2_stage_name_t,
                                 domain::game::pe2_level_name_t,
                                 std::vector<domain::encounter::pe2_room_encounters_t> &,
                                 domain::encounter::pe2_encounters_list_segment_t);
    struct SpawnZones {
        std::vector<utils::geometry_works::Zone> allowed, forbidden;
        bool fromWaterToGround = false;
    };
    const SpawnZones &cachedSpawningZones(std::map<pe2_enemy_movement_t, SpawnZones> &,
                                          const domain::encounter::pe2_room_encounters_t &, pe2_enemy_t);
    bool prepareSpawningZones(std::vector<utils::geometry_works::Zone> &,
                              std::vector<utils::geometry_works::Zone> &,
                              domain::encounter::pe2_room_encounters_t, pe2_enemy_t);
    void createWalkingPathForSlender(std::vector<utils::geometry_works::Zone> &,
                                     std::vector<utils::geometry_works::Zone> &, uint8_t, uint8_t);
    void createSpawnZoneForPurpleGolem(std::vector<utils::geometry_works::Zone> &,
                                       std::vector<utils::geometry_works::Zone> &, uint8_t, uint8_t);
    std::vector<char> buildEnemyEncounterRecordDescriptor(utils::geometry_works::Circle &, pe2_enemy_t,
                                                          bool &, bool &, bool &, uint16_t, size_t, bool,
                                                          bool);
    void updateEnemiesStats();
    void allowOneSlotGolems();

    domain::inner_config_t config;
    std::shared_ptr<PE2Logger> logger;
    std::string temp_dir_name;
    std::mt19937 gen;

    const size_t initial_slender_walking_route_offset = 0x4760;
    const size_t slender_walking_route_addresses_offset = 0xde0;
    size_t current_slender_walking_route_offset = initial_slender_walking_route_offset;
    size_t current_slender_walking_route_addresses_offset =
        initial_slender_walking_route_offset + slender_walking_route_addresses_offset;
    uint8_t current_slender_state = 0x0;
    size_t current_slender_additional_data_address = 0x0;
    uint8_t current_slender_path_id = 0x1;

    const size_t initial_purple_golem_spawn_zone_offset = 0x5a00;
    const size_t purple_golem_spawn_zone_addresses_offset = 0xde0;
    uint8_t current_purple_golem_spawn_zone_id = 0x1;
    size_t current_purple_golem_spawn_zone_offset = initial_purple_golem_spawn_zone_offset;
    size_t current_purple_golem_spawn_zone_addresses_offset =
        initial_purple_golem_spawn_zone_offset + purple_golem_spawn_zone_addresses_offset;
};
} // namespace usecase::enemies
