#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace domain::enemy {
// Aligned gap after the 0x75da-byte shared effects/routes asset, before the
// cloak golem stats at 0x7cd0. The old 0x11be8 address overlaps Knight clip 25.
inline constexpr size_t dummyTaskOffset = 0x75e0;
inline void validateCloakResources(const std::filesystem::path &directory) {
    auto read = [](const auto &path) {
        std::ifstream in(path, std::ios::binary);
        if (!in)
            throw std::runtime_error("Missing cloak golem resource: " + path.string());
        return std::vector<unsigned char>(std::istreambuf_iterator<char>(in), {});
    };
    const auto source = read(directory / "16_knight_golem_slot_1.pe2enm.decompr");
    const auto relocated = read(directory / "dummy_monster.pe2e.decompr");
    if (source.size() < 0x223cc || relocated.size() < 0x123d4)
        throw std::runtime_error("Incomplete cloak golem resources");
    auto word = [](const auto &b, size_t p) {
        return uint32_t(b[p]) | uint32_t(b[p + 1]) << 8 | uint32_t(b[p + 2]) << 16 | uint32_t(b[p + 3]) << 24;
    };
    // All keyframes, indices, descriptors and non-pointer fields must survive.
    // Only pointers into the moved tail change; model/code stay in slot 1.
    for (size_t pos = 0x17dec; pos < 0x223cc; pos += 4) {
        auto expected = word(source, pos);
        if (expected >= 0x80149c0c && expected < 0x801541ec)
            expected += 0x20000;
        if (word(relocated, pos - 0x10000) != expected)
            throw std::runtime_error("Cloak golem relocation lost data or retained an old pointer at " +
                                     std::to_string(pos));
    }
    if (word(relocated, 0x12368) != 0x00600001 || word(relocated, 0x1236c) != 0x80138340 ||
        word(relocated, 0x12370) != 0x8013dbd4)
        throw std::runtime_error("Invalid relocated Knight startup descriptor");
}
// ENEMY_DUMMY_DATA exposes this TaskDesc even though the custom resource
// package no longer contains the original actor. A zero callback crashes the
// retail task walker on its first frame (BUG-012).
inline void finalizeDummyResources(const std::filesystem::path &path) {
    constexpr size_t offset = dummyTaskOffset;
    constexpr size_t capacity = 0x80179950 - 0x80161e20;
    // TaskDesc: flags/priority, callback, metadata. No body or enemy AI.
    constexpr std::array<unsigned char, 12> descriptor{0, 0, 0x60, 0, 0xc4, 0xde, 0x02, 0x80, 0, 0, 0, 0};
    std::ifstream in(path, std::ios::binary);
    if (!in)
        throw std::runtime_error("Cannot read dummy enemy resources");
    std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(in)), {});
    if (bytes.empty() || bytes.size() > capacity)
        throw std::runtime_error("Dummy enemy resources exceed their RAM slot");
    bytes.resize(std::max(bytes.size(), offset + descriptor.size()), 0);
    const auto begin = bytes.begin() + offset;
    if (!std::all_of(begin, begin + descriptor.size(), [](auto b) { return b == 0; }) &&
        !std::equal(descriptor.begin(), descriptor.end(), begin))
        throw std::runtime_error("Dummy TaskDesc overlaps resource data");
    std::copy(descriptor.begin(), descriptor.end(), begin);
    in.close();
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out.write(reinterpret_cast<const char *>(bytes.data()), bytes.size()))
        throw std::runtime_error("Cannot write dummy enemy TaskDesc");
}
} // namespace domain::enemy
