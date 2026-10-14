#pragma once

#include "domain/config/innerconfig.hpp"
#include "domain/file/entities/PE2File.hpp"
#include "domain/game/entities/Stage.hpp"
#include "domain/magic/entities/Magic.hpp"
#include "domain/randomizer/entities/Strategy.hpp"
#include "domain/randomizer/usecases/IFeatureRandomizer.hpp"
#include "utils/FileWorks/FileWorks.hpp"
#include "utils/Logger.hpp"
#include "utils/VectorWorks/VectorWorks.hpp"
#include <cstring>

namespace usecase::magic {
class MagicRandomizer : public IFeatureRandomizer {
  public:
    MagicRandomizer(std::shared_ptr<PE2Logger> _logger, std::mt19937 _gen, std::string _temp_dir_name,
                    domain::inner_config_t _config)
        : logger(_logger), gen(_gen), temp_dir_name(_temp_dir_name), config(_config) {};
    pe2_randomizer_context randomize(pe2_randomizer_context);

  private:
    void randomizeStartingMagic();

    std::shared_ptr<PE2Logger> logger;
    std::mt19937 gen;
    std::string temp_dir_name;
    domain::inner_config_t config;
};
}; // namespace usecase::magic
