#include "domain/costume/usecases/MalePlayers.hpp"
#include "resources/EmbeddedResources.hpp"
#include "utils/Binary.hpp"
#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace domain::costume::male {
std::string assetKey(pe2_costume_id_t costume) {
    switch (costume) {
    case PE2_COSTUME_KYLE:
        return "kyle";
    case PE2_COSTUME_RUPERT:
        return "rupert";
    case PE2_COSTUME_DOUGLAS:
        return "douglas";
    case PE2_COSTUME_POLICE:
        return "police";
    case PE2_COSTUME_GENERAL:
        return "general";
    case PE2_COSTUME_PRESIDENT:
        return "president";
    case PE2_COSTUME_BALDWIN:
        return "baldwin";
    case PE2_COSTUME_SOLDIER:
        return "soldier";
    case PE2_COSTUME_PIERCE:
        return "pierce";
    default:
        return {};
    }
}
void applyDeltas(const std::vector<pe2::resources::Bytes> &patches,
                 const std::vector<std::filesystem::path> &targets) {
    struct Record {
        uint32_t target, offset;
        std::vector<uint8_t> bytes;
    };
    std::vector<Record> records;
    std::vector<std::fstream> files;
    for (const auto &path : targets) {
        files.emplace_back(path, std::ios::in | std::ios::out | std::ios::binary);
        if (!files.back())
            throw std::runtime_error("Cannot open male player patch target: " + path.string());
    }
    size_t total = 0;
    for (const auto &path : patches) {
        auto input = pe2::resources::stream(path);
        char magic[8];
        if (!input.read(magic, 8) || std::string(magic, 8) != "PE2MP001")
            throw std::runtime_error("Invalid embedded male player resource");
        const auto count = utils::binary::readLittleEndianWord(input, "male");
        if (count == 0 || count > 4096)
            throw std::runtime_error("Invalid male player record count");
        for (uint32_t i = 0; i < count; ++i) {
            const auto target = utils::binary::readLittleEndianWord(input, "male"),
                       offset = utils::binary::readLittleEndianWord(input, "male"),
                       length = utils::binary::readLittleEndianWord(input, "male"),
                       before = utils::binary::readLittleEndianWord(input, "male"),
                       after = utils::binary::readLittleEndianWord(input, "male");
            if (target >= files.size() || length == 0 || length > 1024 * 1024 ||
                (total += length) > 8 * 1024 * 1024)
                throw std::runtime_error("Invalid male player patch bounds");
            for (const auto &record : records)
                if (record.target == target &&
                    uint64_t(offset) < uint64_t(record.offset) + record.bytes.size() &&
                    uint64_t(record.offset) < uint64_t(offset) + length)
                    throw std::runtime_error("Overlapping male player patches");
            std::vector<uint8_t> delta(length), bytes(length);
            if (!input.read(reinterpret_cast<char *>(delta.data()), length))
                throw std::runtime_error("Truncated male player data");
            auto &file = files[target];
            file.seekg(offset);
            if (!file.read(reinterpret_cast<char *>(bytes.data()), length) ||
                utils::binary::fnv1a(bytes) != before)
                throw std::runtime_error("Male players require supported USA resources at offset " +
                                         std::to_string(offset));
            for (size_t j = 0; j < length; ++j)
                bytes[j] ^= delta[j];
            if (utils::binary::fnv1a(bytes) != after)
                throw std::runtime_error("Male player checksum mismatch");
            records.push_back({target, offset, std::move(bytes)});
        }
        if (input.peek() != std::char_traits<char>::eof())
            throw std::runtime_error("Unexpected male player patch tail");
    }
    // Validate the model, all weapon banks and voice edits before any write.
    for (const auto &record : records) {
        auto &file = files[record.target];
        file.seekp(record.offset);
        if (!file.write(reinterpret_cast<const char *>(record.bytes.data()), record.bytes.size()))
            throw std::runtime_error("Cannot write male player patch");
    }
    for (auto &file : files) {
        file.flush();
        if (!file)
            throw std::runtime_error("Cannot flush male player patch");
    }
}
void apply(pe2_costume_id_t costume, const std::filesystem::path &temp) {
    const auto key = assetKey(costume);
    if (key.empty())
        return;
    applyDeltas({pe2::resources::get("game/players/" + key + ".pe2mp"),
                 pe2::resources::get("game/players/shared.pe2mp")},
                {temp / "STAGE0.CDF", temp / "STAGE0/00_base.pe2lvl.decompr", temp / "STAGE4.CDF",
                 temp / "STAGE5.CDF", temp / "STAGE1.CDF", temp / "STAGE2.CDF", temp / "STAGE3.CDF"});
}
} // namespace domain::costume::male
