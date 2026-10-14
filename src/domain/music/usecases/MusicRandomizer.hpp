#pragma once

#include "domain/config/innerconfig.hpp"
#include "domain/file/entities/PE2File.hpp"
#include "domain/game/entities/Stage.hpp"
#include "domain/music/entities/Music.hpp"
#include "domain/randomizer/usecases/IFeatureRandomizer.hpp"
#include "utils/FileWorks/FileWorks.hpp"
#include "utils/Logger.hpp"
#include "utils/VectorWorks/VectorWorks.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

namespace usecase::music {

/*
    MUSIC RANDOMIZER CLASS

    Implementation of the IFeatureRandomizer base class.
    Randomizes all music in the game (during gameplay). Simply iterates
    through all globally set music entries and rolls the dice within
    fixed number of tracks related to the "mood" of the music entry.

    atrributes:

    LOGGER - thread safe logger for info and error messages
    GEN - Random Generator Engine for forwarding seed
    TEMP_DIR_NAME - relative or full path to the temp file directory
    STAGE_0_DIR_NAME - sub directory in TEMP_DIR_NAME for stage0 files

    public methods:

    RANDOMIZE - randomizes music
*/

class MusicRandomizer : public usecase::IFeatureRandomizer {
  public:
    MusicRandomizer(std::shared_ptr<PE2Logger> _logger, std::mt19937 _gen, std::string _temp_dir_name,
                    domain::inner_config_t _config)
        : config(_config), logger(_logger), gen(_gen), temp_dir_name(_temp_dir_name) {};
    pe2_randomizer_context randomize(pe2_randomizer_context);

  private:
    void randomizeMusicInStage(std::string, std::vector<domain::music::pe2_music_entry_t>);

    domain::inner_config_t config;
    std::shared_ptr<PE2Logger> logger;
    std::mt19937 gen;
    std::string temp_dir_name;
};
} // namespace usecase::music
