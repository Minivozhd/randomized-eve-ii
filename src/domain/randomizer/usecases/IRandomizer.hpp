#pragma once

#include "domain/config/innerconfig.hpp"

using namespace domain;

namespace usecase {
class IRandomizer {
  public:
    virtual void randomize(domain::inner_config_t config) = 0;
};
}; // namespace usecase
