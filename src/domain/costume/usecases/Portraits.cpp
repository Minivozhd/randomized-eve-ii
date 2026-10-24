#include "domain/costume/usecases/Portraits.hpp"
#include "domain/costume/usecases/MalePlayers.hpp"
#include "resources/EmbeddedResources.hpp"
#include "utils/Binary.hpp"
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <vector>

namespace domain::costume::portraits {
void applyNameLayout(std::iostream &gameplay) {
    // Gp_HpMpBarTask: use a reserved 128x8 atlas strip at (64,232).
    // Keep the right edge before the portrait, moving only the left edge.
    // Instructions and all following callbacks retain their retail addresses.
    struct Edit {
        unsigned offset, before, after;
    };
    constexpr Edit edits[] = {
        {0x2df60, 0x2442ff8e, 0x2442ff4e},
        {0x2df70, 0x24020038, 0x24020040},
        {0x2df78, 0x24020060, 0x240200e8},
        {0x2df80, 0x24020040, 0x24020080},
    };
    for (const auto &e : edits) {
        gameplay.seekg(e.offset);
        if (utils::binary::readLittleEndianWord(gameplay, "player name layout") != e.before)
            throw std::runtime_error("Unsupported player name renderer");
    }
    for (const auto &e : edits) {
        gameplay.seekp(e.offset);
        for (unsigned i = 0; i != 4; ++i)
            gameplay.put(static_cast<char>(e.after >> (8 * i)));
    }
    gameplay.flush();
    if (!gameplay)
        throw std::runtime_error("Cannot write player name renderer");
}
std::string assetKey(pe2_costume_id_t costume) {
    if (auto key = male::assetKey(costume); !key.empty())
        return key;
    switch (costume) {
    case PE2_COSTUME_DEFAULT:
        return {};
    case PE2_COSTUME_LEATHER_JACKET:
    case PE2_COSTUME_ASSAULT_SUIT:
    case PE2_COSTUME_SHOULDER_HOLSTER:
    case PE2_COSTUME_SHOULDER_HOLSTER_WITH_EAR_PROTECTION:
        return "aya";
    case PE2_COSTUME_JUDIE_MIST_GARAGE:
        return "jodie";
    case PE2_COSTUME_JUDIE_MILITARY_CAMP:
        return "jodie-camp";
    case PE2_COSTUME_CAFETERIA_WOMAN:
        return "cafeteria";
    case PE2_COSTUME_TOILET_WOMAN:
        return "toilet";
    case PE2_COSTUME_DC2_REGINA_DEFAULT:
    case PE2_COSTUME_DC2_REGINA_LIGHTWEIGHT_ARMOR:
        return "regina";
    case PE2_COSTUME_RE1_JILL:
        return "jill";
    default:
        throw std::runtime_error("No menu portrait for selected player model");
    }
}
void applyDelta(pe2::resources::Bytes patch, const std::filesystem::path &stage0) {
    auto input = pe2::resources::stream(patch);
    char magic[8];
    if (!input.read(magic, 8) || std::string(magic, 8) != "PE2PT001")
        throw std::runtime_error("Invalid embedded player portrait resource");
    if (utils::binary::readLittleEndianWord(input, "portraits") != 2)
        throw std::runtime_error("Invalid player portrait record count");
    std::fstream file(stage0, std::ios::in | std::ios::out | std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open player portrait target: " + stage0.string());
    struct Record {
        uint32_t offset;
        std::vector<uint8_t> bytes;
    };
    std::vector<Record> records;
    for (unsigned i = 0; i < 2; ++i) {
        auto offset = utils::binary::readLittleEndianWord(input, "portraits"),
             length = utils::binary::readLittleEndianWord(input, "portraits"),
             before = utils::binary::readLittleEndianWord(input, "portraits"),
             after = utils::binary::readLittleEndianWord(input, "portraits");
        // This format may only change the two retail UI resource slots.
        if (offset != (i == 0 ? 0x77800u : 0x7a800u) || length != (i == 0 ? 0x3000u : 0x800u))
            throw std::runtime_error("Player portrait patch exceeds UI resource bounds");
        std::vector<uint8_t> delta(length), bytes(length);
        if (!input.read(reinterpret_cast<char *>(delta.data()), length))
            throw std::runtime_error("Truncated player portrait data");
        file.seekg(offset);
        if (!file.read(reinterpret_cast<char *>(bytes.data()), length) ||
            utils::binary::fnv1a(bytes) != before)
            throw std::runtime_error("Player portraits require supported unmodified USA menu resources");
        for (size_t j = 0; j < length; ++j)
            bytes[j] ^= delta[j];
        if (utils::binary::fnv1a(bytes) != after)
            throw std::runtime_error("Player portrait checksum mismatch");
        records.push_back({offset, std::move(bytes)});
    }
    if (input.peek() != std::char_traits<char>::eof())
        throw std::runtime_error("Unexpected player portrait patch tail");
    // Verify both slots and every checksum before changing either resource.
    for (const auto &record : records) {
        file.seekp(record.offset);
        if (!file.write(reinterpret_cast<const char *>(record.bytes.data()), record.bytes.size()))
            throw std::runtime_error("Cannot write player portrait patch");
    }
    file.flush();
    if (!file)
        throw std::runtime_error("Cannot flush player portrait patch");
}
void apply(pe2_costume_id_t costume, const std::filesystem::path &temp) {
    const auto key = assetKey(costume);
    if (key.empty())
        return;
    applyDelta(pe2::resources::get("game/portraits/" + key + ".pe2pt"), temp / "STAGE0.CDF");
}
} // namespace domain::costume::portraits
