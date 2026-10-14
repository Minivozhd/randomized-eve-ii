#pragma once
#include <cstdint>
#include <istream>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

namespace utils::binary {
inline std::uint32_t fnv1a(std::span<const std::uint8_t> bytes) {
    std::uint32_t value = 2166136261u;
    for (auto byte : bytes)
        value = (value ^ byte) * 16777619u;
    return value;
}
inline std::uint32_t readLittleEndianWord(std::istream &input, std::string_view context) {
    std::uint8_t bytes[4];
    if (!input.read(reinterpret_cast<char *>(bytes), sizeof(bytes)))
        throw std::runtime_error(std::string(context) + ": truncated 32-bit word");
    return std::uint32_t(bytes[0]) | std::uint32_t(bytes[1]) << 8 | std::uint32_t(bytes[2]) << 16 |
           std::uint32_t(bytes[3]) << 24;
}
} // namespace utils::binary
