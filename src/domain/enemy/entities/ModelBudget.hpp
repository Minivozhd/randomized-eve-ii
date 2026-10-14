#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <map>
#include <stdexcept>

namespace domain::enemy::models {
// USA TmdSource envelopes: sum of BOTH primitive-buffer halves of every
// distinct model in the actor package (including weapons/death fragments).
// Shared model families are deliberately counted per actor in the heap budget.
// This bounds the bootstrap model buffers; repeated late-AI effects and the
// primary task heap remain separate runtime-loader test obligations.
struct Cost {
    uint8_t id;
    uint32_t bytes;
    uint8_t allocations;
};
inline constexpr std::array<Cost, 33> costs{
    {{0x01, 51648, 5}, {0x03, 56272, 7}, {0x04, 40656, 7}, {0x06, 65472, 10}, {0x07, 13304, 2},
     {0x08, 13304, 2}, {0x0a, 62416, 5}, {0x0b, 53472, 4}, {0x0c, 11968, 1},  {0x0d, 50896, 4},
     {0x0f, 14224, 5}, {0x10, 22832, 4}, {0x12, 50416, 4}, {0x13, 44080, 2},  {0x14, 43824, 2},
     {0x15, 3408, 1},  {0x16, 87808, 1}, {0x17, 43592, 3}, {0x18, 8336, 1},   {0x19, 17328, 4},
     {0x1a, 15088, 2}, {0x25, 5648, 3},  {0x26, 6200, 6},  {0x27, 87808, 1},  {0x28, 11248, 1},
     {0x2c, 35496, 4}, {0x2e, 13400, 2}, {0x31, 53472, 4}, {0x37, 15088, 2},  {0x38, 44680, 3},
     {0x39, 44448, 4}, {0x46, 36584, 5}, {0x48, 28592, 5}}};
inline uint32_t actorBytes(uint8_t id) {
    if (id == 0xff)
        return 0; // resource-only dummy, no model
    for (auto c : costs)
        if (c.id == id)
            return c.bytes + 32u * c.allocations;
    throw std::runtime_error("Missing enemy model budget metadata: " + std::to_string(id));
}
// Largest retail body + all four hand models: 54520 bytes. Reserve 64 KiB
// for player model variants, 8 KiB for the largest weapon family (7336),
// plus allocator blocks. This is not a claim to cover all scene/AI heaps.
inline constexpr uint32_t playerAndWeaponReserve = 0x10000 + 0x2000 + 8 * 32;
inline uint64_t required(const std::map<uint8_t, size_t> &counts) {
    uint64_t bytes = playerAndWeaponReserve;
    for (auto [id, n] : counts)
        bytes += uint64_t(actorBytes(id)) * n;
    return bytes;
}
inline bool fits(uint32_t capacity, const std::map<uint8_t, size_t> &counts) {
    return required(counts) <= capacity;
}
// Preserve the selected species. Selection already guarantees one of each
// fits; reduce excessive multiplicities before recording actors or rewards.
inline void fit(uint32_t capacity, std::map<uint8_t, size_t> &counts) {
    while (!fits(capacity, counts)) {
        auto chosen = counts.end();
        for (auto it = counts.begin(); it != counts.end(); ++it)
            if (it->second > 1 &&
                (chosen == counts.end() || uint64_t(actorBytes(it->first)) * it->second >
                                               uint64_t(actorBytes(chosen->first)) * chosen->second))
                chosen = it;
        if (chosen == counts.end())
            throw std::runtime_error("Selected enemy models exceed room auxiliary heap");
        --chosen->second;
    }
}
inline uint32_t roomCapacity(std::fstream &map, unsigned stage, unsigned room) {
    // Gfx_ImageSlotTables (Mem_ConfigureAuxHeap); map overlay base 80179950.
    constexpr std::array<uint32_t, 6> offsets{0, 0x6f8, 0xc4, 0x54, 0x1f0, 0x468};
    constexpr std::array<unsigned, 6> rooms{0, 22, 39, 39, 50, 34};
    if (!stage || stage >= offsets.size() || room >= rooms[stage])
        throw std::runtime_error("Invalid room model-budget index");
    std::array<unsigned char, 8> b{};
    map.clear();
    map.seekg(offsets[stage] + room * 8);
    if (!map.read(reinterpret_cast<char *>(b.data()), b.size()))
        throw std::runtime_error("Truncated room model-budget table");
    auto word = [&](unsigned off) {
        return uint32_t(b[off]) | (uint32_t(b[off + 1]) << 8) | (uint32_t(b[off + 2]) << 16) |
               (uint32_t(b[off + 3]) << 24);
    };
    const uint32_t base = word(0), extent = word(4);
    if (base < 0x80179950 || base >= 0x801d7000 || extent < 0x10000 || uint64_t(base) + extent != 0x801d7000)
        throw std::runtime_error("Unsupported USA room auxiliary heap bounds");
    return extent - 0x10000;
}
} // namespace domain::enemy::models
