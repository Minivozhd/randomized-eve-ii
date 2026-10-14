#pragma once

#include "domain/armor/usecases/ArmorRandomizer.hpp"
#include "domain/config/innerconfig.hpp"
#include "domain/costume/usecases/CostumeRandomizer.hpp"
#include "domain/enemy/usecases/EnemyRandomizer.hpp"
#include "domain/enhancer/enemy/Enemy.hpp"
#include "domain/game/entities/Stage.hpp"
#include "domain/item/usecases/ItemRandomizer.hpp"
#include "domain/magic/usecases/MagicRandomizer.hpp"
#include "domain/music/entities/Music.hpp"
#include "domain/music/usecases/MusicRandomizer.hpp"
#include "domain/randomizer/usecases/IFeatureRandomizer.hpp"
#include "domain/randomizer/usecases/IRandomizer.hpp"
#include "utils/Logger.hpp"
#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <stdio.h>
#include <string>
#include <unordered_set>
#include <vector>

#define ACROPOLIS_ENCOUNTERS_START 0x136c
#define ACROPOLIS_ENCOUNTERS_END 0x242c
#define DRYFIELD_DAY_ENCOUNTERS_START 0x13f4
#define DRYFIELD_DAY_ENCOUNTERS_END 0x2394
#define DRYFIELD_NIGHT_ENCOUNTERS_START 0x12f8
#define DRYFIELD_NIGHT_ENCOUNTERS_END 0x3758
#define ENCOUNTER_RECORD_SIZE 0x10
#define NEO_ARK_ENCOUNTERS_START 0x1580
#define NEO_ARK_ENCOUNTERS_END 0x2e40
#define POOL_ENEMY_TRESCHOLD 9
#define MAX_ENEMIES 13
#define MAX_BIG_ENEMIES 4
#define ROOM_INFO_FILE_LOAD_ADDRESS 0x8017d5c0
#define LEVEL_INFO_FILE_LOAD_ADDRESS 0x80179950
#define ITEM_COST_START_ADDRESS 0x7a040
#define ITEMS_COUNT 0xaf

#define AMBUSH_PROBABILITY 0.5

namespace usecase {
class Randomizer : public IRandomizer {
  public:
    Randomizer(std::shared_ptr<PE2Logger>);
    void randomize(domain::inner_config_t) override;

  private:
    void enhanceEnemiesAndBosses();
    void oneTimeAmmoBoxPickups();
    void disableAmmoBoxes(std::string filename, std::vector<size_t> addresses, bool acropolis);
    void enableMagicElements();
    void activeTimeBattle();
    void updateColorScheme();
    std::vector<std::shared_ptr<IFeatureRandomizer>> getRandomizers();
    void applyMods();
    void increaseIFrames();
    void skipWhiteScreen();
    void skipWinPose();
    void disableTurning();
    void enableInventoryDuringBattles();

    const std::string temp_dir;
    domain::inner_config_t config;
    std::mt19937 gen;

    std::shared_ptr<PE2Logger> _logger;
};
} // namespace usecase
