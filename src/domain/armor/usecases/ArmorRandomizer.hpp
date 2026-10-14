#pragma once

#include "domain/armor/entities/Armor.hpp"
#include "domain/config/innerconfig.hpp"
#include "domain/file/entities/PE2File.hpp"
#include "domain/game/entities/Stage.hpp"
#include "domain/randomizer/usecases/IFeatureRandomizer.hpp"
#include "utils/FileWorks/FileWorks.hpp"
#include "utils/Logger.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <random>
#include <vector>

namespace usecase::armors {
class ArmorRandomizer : public IFeatureRandomizer {
  public:
    ArmorRandomizer(std::shared_ptr<PE2Logger> _logger, std::mt19937 _gen, std::string _temp_dir_name,
                    domain::inner_config_t _config)
        : config(_config), logger(_logger), temp_dir_name(_temp_dir_name), gen(_gen) {};
    pe2_randomizer_context randomize(pe2_randomizer_context);

  private:
    domain::inner_config_t config;
    std::shared_ptr<PE2Logger> logger;
    std::string temp_dir_name;
    std::mt19937 gen;

    std::vector<char> build_armor_stats_descriptor(int16_t, uint8_t, uint8_t, uint32_t);
};
} // namespace usecase::armors
