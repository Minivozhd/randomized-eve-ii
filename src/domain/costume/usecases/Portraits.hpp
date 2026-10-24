#pragma once
#include "domain/costume/entities/Costume.hpp"
#include "resources/EmbeddedResources.hpp"
#include <filesystem>
#include <iostream>
#include <string>
namespace domain::costume::portraits {
// Uses the already selected costume: no additional RNG draw.
std::string assetKey(pe2_costume_id_t costume);
void applyDelta(pe2::resources::Bytes patch, const std::filesystem::path &stage0);
void applyNameLayout(std::iostream &gameplay);
void apply(pe2_costume_id_t costume, const std::filesystem::path &temp);
} // namespace domain::costume::portraits
