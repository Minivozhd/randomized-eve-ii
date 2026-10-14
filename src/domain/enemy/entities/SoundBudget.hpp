#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <map>
#include <stdexcept>
#include <vector>

namespace domain::enemy::audio {
#include "patches/enemy_audio/catalog.inc"

struct Bank {
    uint16_t id;
    uint32_t bytes;
};
using Banks = std::map<uint16_t, uint32_t>;
inline uint32_t aligned(uint32_t n) {
    return (n + 63u) & ~63u;
}
inline void merge(Banks &banks, Bank b) {
    banks[b.id] = std::max(banks[b.id], b.bytes);
}
inline void merge(Banks &banks, const Banks &other) {
    for (auto [id, bytes] : other)
        merge(banks, Bank{id, bytes});
}
inline uint32_t word(const uint8_t *p) {
    return uint32_t(p[0]) | uint32_t(p[1]) << 8 | uint32_t(p[2]) << 16 | uint32_t(p[3]) << 24;
}
inline void readAt(std::ifstream &in, uint64_t offset, uint8_t *dst, size_t size) {
    in.seekg(static_cast<std::streamoff>(offset));
    if (!in.read(reinterpret_cast<char *>(dst), size))
        throw std::runtime_error("Truncated sound resource archive");
}
inline Bank readBank(std::ifstream &in, uint64_t offset) {
    std::array<uint8_t, 36> h{};
    readAt(in, offset, h.data(), h.size());
    // hSPK image kind 2 is a script; hMPK/music uses another SPU region.
    if (h[0] != 6 || word(h.data() + 16) != 0x4b505368 || h[22] != 2)
        throw std::runtime_error("Unsupported USA sound bank header");
    if (h[26] || h[27])
        throw std::runtime_error("Unsupported sound wave block offset");
    const auto bytes = word(h.data() + 32);
    if (bytes > 0x80000)
        throw std::runtime_error("Invalid sound bank size");
    const uint64_t sectors = word(h.data() + 4);
    const uint32_t imageBytes = h[30] | uint32_t(h[31]) << 8;
    const uint64_t payloadEnd = 16 + ((2048u + imageBytes + 2047u) & ~2047u) + bytes;
    if (!sectors || payloadEnd > sectors * 2048)
        throw std::runtime_error("Truncated sound bank payload");
    return {uint16_t(h[20] | uint16_t(h[21]) << 8), bytes};
}

// Conservative envelope of SndLoad_ResolveSpuAddr. Round EACH enemy bank up:
// the original only rounds its final boundary, so this can reject a borderline
// pair but cannot admit a collision through alignment or variant/load order.
// Same-ID banks load once (SndBank_FreeById); reserve their largest variant.
inline uint32_t enemyCapacity(const Banks &room) {
    uint32_t type1 = 0, type5 = 0;
    for (auto [id, bytes] : room) {
        if (id >> 12 == 1)
            type1 = std::max(type1, aligned(bytes));
        if (id >> 12 == 5)
            type5 = std::max(type5, aligned(bytes));
    }
    uint32_t end = (type1 + type5 <= 0x26800) ? 0x63810 - type1 - type5 : 0x3d010;
    for (auto [id, bytes] : room) {
        if (id >> 12 == 3 && bytes)
            end = std::min(end, 0x47010u);
    }
    return end - 0x3d010;
}
inline bool fits(const Banks &room, const Banks &enemies) {
    Banks live = enemies;
    for (auto [id, bytes] : room)
        if (id >> 12 == 4)
            merge(live, Bank{id, bytes});
    uint64_t total = 0;
    if (live.size() > 3)
        return false;
    for (auto [id, bytes] : live) {
        if (id >> 12 != 4)
            throw std::runtime_error("Unexpected enemy bank type");
        total += aligned(bytes);
    }
    return total <= enemyCapacity(room);
}

class Catalog {
  public:
    void loadEnemies(const std::filesystem::path &path) {
        std::ifstream in(path, std::ios::binary);
        families.clear();
        for (auto family : soundFamilies)
            families[family] = {};
        for (auto location : soundLocations) {
            const auto bank = readBank(in, location.offset);
            if (bank.id >> 12 != 4)
                throw std::runtime_error("Unexpected enemy resource sound type");
            merge(families.at(location.family), bank);
        }
    }
    const Banks &enemy(uint8_t fileId, uint16_t slot) const {
        const auto key = (uint32_t(slot) + 1) * 100000 + uint32_t(fileId) * 100;
        const auto it = families.find(key);
        if (it == families.end())
            throw std::runtime_error("Missing USA enemy sound family: " + std::to_string(key));
        return it->second;
    }
    static std::map<uint8_t, Banks> loadRooms(const std::filesystem::path &path) {
        std::ifstream in(path, std::ios::binary);
        const auto size = std::filesystem::file_size(path);
        std::map<uint8_t, Banks> result;
        uint64_t base = 2048;
        for (size_t pos = 0; pos < 0x500; pos += 8) {
            std::array<uint8_t, 8> rec{};
            readAt(in, pos, rec.data(), rec.size());
            auto key = word(rec.data()), sectors = word(rec.data() + 4);
            if (!key && !sectors)
                return result;
            if (key % 100 != 1 || key / 100 > 255 || !sectors || base + uint64_t(sectors) * 2048 > size)
                throw std::runtime_error("Invalid room audio directory");
            auto &banks = result[static_cast<uint8_t>(key / 100)];
            // Only chain 0 is the room load. Camera subchains do not coexist
            // as separate room banks; reject unexpected audio there in audits.
            readAt(in, base, rec.data(), rec.size());
            if (word(rec.data()) != 0)
                throw std::runtime_error("Missing room base chain");
            uint64_t offset = base + uint64_t(word(rec.data() + 4)) * 2048;
            const auto end = base + uint64_t(sectors) * 2048;
            for (;;) {
                std::array<uint8_t, 16> h{};
                if (offset < base + 2048 || offset + 16 > end)
                    throw std::runtime_error("Invalid room audio chain");
                readAt(in, offset, h.data(), h.size());
                const auto count = word(h.data() + 4);
                if (!count || offset + uint64_t(count) * 2048 > end)
                    throw std::runtime_error("Invalid room resource size");
                if (h[0] == 6) {
                    auto b = readBank(in, offset);
                    const auto type = b.id >> 12;
                    if (type != 1 && type != 3 && type != 4 && type != 5)
                        throw std::runtime_error("Unsupported room audio bank type");
                    merge(banks, b);
                }
                offset += uint64_t(count) * 2048;
                if (h[1] == 255)
                    break;
                if (h[1] > 1)
                    throw std::runtime_error("Invalid room chain terminator");
            }
            base = end;
        }
        throw std::runtime_error("Unterminated room audio directory");
    }

  private:
    std::map<uint32_t, Banks> families;
};
} // namespace domain::enemy::audio
