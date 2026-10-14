#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace patches::title {
#include "patches/title_screen/generated.inc"

inline constexpr size_t kChunkOffset = 0x7c000;
inline constexpr size_t kCapacity = 0x5ff0;
inline constexpr std::array<uint8_t, 16> kChunkHeader = {
    0x05, 0x01, 0x00, 0x08, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static_assert(sizeof(kBackground) <= kCapacity);

inline uint64_t hashBackground(const std::vector<uint8_t> &bytes) {
    uint64_t hash = 0xcbf29ce484222325ULL;
    for (uint8_t byte : bytes)
        hash = (hash ^ byte) * 0x100000001b3ULL;
    return hash;
}

// A fixed-size data replacement: no code, VRAM, CD extent or chunk-header changes.
// Call only on the extracted working CDF, never the user's original disc image.
inline void applyBackground(const std::filesystem::path &stage0) {
    std::fstream file(stage0, std::ios::binary | std::ios::in | std::ios::out);
    if (!file)
        throw std::runtime_error("Title background: cannot open " + stage0.string());
    std::array<uint8_t, 16> header{};
    std::vector<uint8_t> current(kCapacity);
    file.seekg(kChunkOffset);
    file.read(reinterpret_cast<char *>(header.data()), header.size());
    file.read(reinterpret_cast<char *>(current.data()), current.size());
    if (!file || header != kChunkHeader)
        throw std::runtime_error("Title background: unsupported or truncated USA STAGE0 title chunk");
    const uint64_t hash = hashBackground(current);
    if (hash == kPatchedHash)
        return;
    if (hash != kOriginalHash)
        throw std::runtime_error("Title background: original USA title resource does not match");
    std::fill(current.begin(), current.end(), 0);
    std::copy(std::begin(kBackground), std::end(kBackground), current.begin());
    file.seekp(kChunkOffset + header.size());
    file.write(reinterpret_cast<const char *>(current.data()), current.size());
    file.flush();
    if (!file)
        throw std::runtime_error("Title background: failed to write working STAGE0.CDF");
}
} // namespace patches::title
