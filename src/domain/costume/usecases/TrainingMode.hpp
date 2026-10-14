#pragma once
#include <array>
#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace domain::costume::training {
// USA gameplay predicates already guard stage 1 / area 20. Model slot 4
// was also the retail training marker. The room explicitly sets sceneEvent
// to 2 for training (warp 6), and 1 for ordinary entry (warp 5).
// Read that marker without changing model selection or allocating a hook.
struct Predicate {
    uint32_t address;
    uint8_t baseRegister;
};
inline constexpr std::array<Predicate, 21> predicates{
    {{0x800a15a4, 6}, {0x800a16ac, 5}, {0x800a1d18, 6}, {0x800a2024, 6},  {0x800a217c, 16}, {0x800a227c, 6},
     {0x800a23b4, 6}, {0x800a24fc, 6}, {0x800a260c, 7}, {0x800a2744, 5},  {0x800a29f0, 6},  {0x800a3004, 18},
     {0x800a3480, 4}, {0x800a39ec, 6}, {0x800a3ee4, 4}, {0x800a4290, 17}, {0x800a4314, 4},  {0x800a7494, 4},
     {0x800a74ec, 4}, {0x800a7b54, 5}, {0x800a7be4, 6}}};
inline uint32_t read(std::fstream &file, size_t offset) {
    std::array<uint8_t, 4> b{};
    file.seekg(offset);
    if (!file.read(reinterpret_cast<char *>(b.data()), 4))
        throw std::runtime_error("Truncated training-mode predicate");
    return uint32_t(b[0]) | uint32_t(b[1]) << 8 | uint32_t(b[2]) << 16 | uint32_t(b[3]) << 24;
}
inline void write(std::fstream &file, size_t offset, uint32_t word) {
    const std::array<uint8_t, 4> b{uint8_t(word), uint8_t(word >> 8), uint8_t(word >> 16),
                                   uint8_t(word >> 24)};
    file.seekp(offset);
    if (!file.write(reinterpret_cast<const char *>(b.data()), 4))
        throw std::runtime_error("Cannot patch training-mode predicate");
}
inline void apply(std::fstream &file) {
    for (auto p : predicates) {
        const auto offset = p.address - 0x80093800;
        const auto load = 0x90020026u | (uint32_t(p.baseRegister) << 21);
        if (read(file, offset) != load || read(file, offset + 4) != 0 ||
            read(file, offset + 8) != 0x38420004 ||
            read(file, offset + 12) != (p.address == 0x800a74ec ? 0x03e00008u : 0x2c420001u))
            throw std::runtime_error("Unsupported USA training-mode predicate at " +
                                     std::to_string(p.address));
    }
    for (auto p : predicates) {
        // gMcSaveData.state.sceneEvent (8007272d) - gPlayerStatus (80073b88).
        write(file, p.address - 0x80093800, 0x9002eba5u | (uint32_t(p.baseRegister) << 21));
        write(file, p.address - 0x80093800 + 8, 0x38420002);
    }
}
} // namespace domain::costume::training
