#pragma once
#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace domain::enemy::audio {
struct GolemSoundTarget {
    const char *name;
    size_t offset;
};
inline constexpr GolemSoundTarget golemSoundTargets[] = {
    {"14_pawn_blade_golem_slot_1", 0x15e4a}, {"14_pawn_blade_golem_slot_2", 0x15e4a},
    {"16_knight_golem_slot_1", 0x221b6},     {"17_rook_blade_golem_slot_1", 0x15b12},
    {"17_rook_blade_golem_slot_2", 0x15b12}, {"27_bishop_golem_slot_1", 0x2210e},
    {"38_pawn_gun_golem_slot_1", 0x1630a},   {"38_pawn_gun_golem_slot_2", 0x1630a},
    {"39_rook_gun_golem_slot_1", 0x17242},   {"39_rook_gun_golem_slot_2", 0x17242},
};
inline void patchGolemSounds(const std::filesystem::path &directory) {
    const auto source = directory / "golem_sound_data.pe2e";
    std::array<char, 368> bytes{};
    if (std::filesystem::file_size(source) != bytes.size())
        throw std::runtime_error("Invalid golem sound table length (expected 368 bytes)");
    std::ifstream input(source, std::ios::binary);
    if (!input.read(bytes.data(), bytes.size()))
        throw std::runtime_error("Cannot read golem sound table");
    // Validate all destinations before changing any of them.
    std::array<std::fstream, std::size(golemSoundTargets)> outputs;
    for (size_t i = 0; i < outputs.size(); ++i) {
        auto target = golemSoundTargets[i];
        auto path = directory / (std::string(target.name) + ".pe2enm.decompr");
        if (std::filesystem::file_size(path) < target.offset + bytes.size())
            throw std::runtime_error("Truncated golem sound destination: " + path.string());
        outputs[i].open(path, std::ios::binary | std::ios::in | std::ios::out);
        outputs[i].seekp(target.offset);
        if (!outputs[i])
            throw std::runtime_error("Cannot open golem sound destination");
    }
    for (auto &out : outputs) {
        out.write(bytes.data(), bytes.size());
        out.flush();
        if (!out)
            throw std::runtime_error("Cannot write golem sound table");
    }
}
} // namespace domain::enemy::audio
