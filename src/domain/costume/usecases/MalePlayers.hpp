#pragma once
#include "domain/costume/entities/Costume.hpp"
#include "resources/EmbeddedResources.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace domain::costume::male {
std::string assetKey(pe2_costume_id_t costume);
void applyDeltas(const std::vector<pe2::resources::Bytes> &patches,
                 const std::vector<std::filesystem::path> &targets);
void apply(pe2_costume_id_t costume, const std::filesystem::path &temp);
} // namespace domain::costume::male
