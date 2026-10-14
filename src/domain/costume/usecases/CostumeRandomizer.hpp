#pragma once

#include "domain/config/innerconfig.hpp"
#include "domain/costume/entities/Costume.hpp"
#include "domain/file/entities/PE2File.hpp"
#include "domain/game/entities/Room.hpp"
#include "domain/game/entities/Stage.hpp"
#include "domain/randomizer/usecases/IFeatureRandomizer.hpp"
#include "utils/FileWorks/FileWorks.hpp"
#include "utils/Logger.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <map>
#include <random>

#define ADDITIONAL_COSTUME_TEXTURE_OFFSET 0x462048

namespace usecase::costumes {
class CostumeRandomizer : public IFeatureRandomizer {
  public:
    CostumeRandomizer(std::shared_ptr<PE2Logger> _logger, std::mt19937 _gen, std::string _temp_dir_name,
                      domain::inner_config_t _config)
        : config(_config), logger(_logger), temp_dir_name(_temp_dir_name), gen(_gen) {};
    pe2_randomizer_context randomize(pe2_randomizer_context);

  private:
    friend struct CostumeRandomizerTestAccess;
    domain::inner_config_t config;
    std::shared_ptr<PE2Logger> logger;
    std::string temp_dir_name;
    std::mt19937 gen;

    void chooseCostumeSlot(domain::costume::pe2_costume_t &, pe2_randomizer_context);
    void substituteCostumeModel(domain::costume::pe2_costume_t &);
};
} // namespace usecase::costumes
