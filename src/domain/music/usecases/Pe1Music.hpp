#pragma once
#include "domain/music/entities/Music.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace domain::music::pe1 {
struct Track {
    const char *name;
    pe2_music_type_t mood;
    uint32_t source, phase, id, offset, length, before, after, deltaHash, slotBytes;
};
const std::vector<Track> &tracks();
std::vector<uint8_t> pool(pe2_music_type_t mood);
std::filesystem::path packagePath();
void validatePackage(const std::filesystem::path &package = packagePath());
void applyVolumeBoost(const std::filesystem::path &workdir);
void apply(const std::filesystem::path &workdir, const std::filesystem::path &package = packagePath());
} // namespace domain::music::pe1
