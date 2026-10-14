#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace domain::armor {
// Inventory graphics use 8-bit indices, uploaded in 128 x 32 pixel strips.
// Decode with the game's 256-byte dictionary and nine-bit end marker.
inline std::vector<uint8_t> decodeArmorStream(const std::vector<char> &data, size_t &cursor, size_t limit) {
    std::array<uint8_t, 256> ring{};
    size_t write = 1, bit = cursor * 8;
    std::vector<uint8_t> out;
    auto bits = [&](unsigned n) {
        unsigned value = 0;
        while (n--) {
            if (bit >= data.size() * 8)
                throw std::runtime_error("Truncated armor image stream");
            value = (value << 1) | ((static_cast<uint8_t>(data[bit / 8]) >> (7 - bit % 8)) & 1);
            ++bit;
        }
        return value;
    };
    auto put = [&](uint8_t value) {
        if (out.size() == limit)
            throw std::runtime_error("Armor image exceeds decoded size limit");
        out.push_back(value);
        ring[write] = value;
        write = (write + 1) & 255;
    };
    for (;;) {
        if (bits(1))
            put(bits(8));
        else {
            unsigned source = bits(8);
            if (!source)
                break;
            unsigned count = bits(4) + 2;
            while (count--) {
                put(ring[source]);
                source = (source + 1) & 255;
            }
        }
    }
    cursor = (bit + 7) / 8;
    return out;
}

inline void validateArmorImage(const std::vector<char> &texture, const std::vector<char> &palette) {
    size_t cursor = 0;
    const auto colors = decodeArmorStream(palette, cursor, 512);
    if (colors.empty() || colors.size() % 2)
        throw std::runtime_error("Invalid armor palette size");
    for (; cursor < palette.size(); ++cursor)
        if (palette[cursor])
            throw std::runtime_error("Unexpected armor palette trailing data");
    cursor = 0;
    for (unsigned strip = 0; strip < 3; ++strip) {
        const auto pixels = decodeArmorStream(texture, cursor, 4096);
        if (pixels.size() != 4096)
            throw std::runtime_error("Armor image must contain three complete 128x32 strips");
        for (auto index : pixels)
            if (index >= colors.size() / 2)
                throw std::runtime_error("Armor image references missing palette color");
        // Short padding is skipped in-place. Six zero bytes make the retail
        // loader yield until the next CD sector (TEX data starts at +0x20).
        size_t padding = 0;
        while (cursor < texture.size() && texture[cursor] == 0) {
            ++cursor;
            ++padding;
        }
        if (strip < 2 && padding >= 6 && (cursor + 0x20) % 0x800 != 0)
            throw std::runtime_error("Armor strip padding would resume outside the next CD sector");
    }
    if (cursor != texture.size())
        throw std::runtime_error("Armor image exceeds 128x96 inventory dimensions");
}
} // namespace domain::armor
