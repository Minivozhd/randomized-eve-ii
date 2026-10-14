#pragma once

#include "domain/randomizer/usecases/RandomizerContext.hpp"

namespace usecase {
class IFeatureRandomizer {
  public:
    virtual pe2_randomizer_context randomize(pe2_randomizer_context) = 0;
    virtual ~IFeatureRandomizer() = default;
};
}; // namespace usecase
