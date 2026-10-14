#pragma once
#include "domain/costume/usecases/MalePlayers.hpp"
#include "patches/weapon_variants/generated.hpp"
#include "resources/EmbeddedResources.hpp"
#include "utils/Binary.hpp"
#include <fstream>
#include <stdexcept>

namespace patches::weapon_variants {
inline void apply(const std::filesystem::path &work, bool rupert, bool kyle) {
    if (!rupert && !kyle)
        return;
    const auto stage = work / "STAGE0.CDF";
    std::vector<pe2::resources::Bytes> patches;
    if (rupert)
        patches.push_back(pe2::resources::get("game/weapon_variants/rupert.pe2mp"));
    if (kyle) {
        // Select by actual bytes: Random player can resolve to either skeleton.
        // Unknown banks fail closed instead of overwriting newer animations.
        std::ifstream input(stage, std::ios::binary);
        std::vector<uint8_t> bytes(packageSize);
        input.seekg(packageOffset);
        if (!input.read(reinterpret_cast<char *>(bytes.data()), bytes.size()))
            throw std::runtime_error("Kyle P229: cannot read the USA weapon package");
        const auto hash = utils::binary::fnv1a(bytes);
        std::string profile;
        if (hash == retailHash)
            profile = "retail";
        else if (hash == maleHash)
            profile = "male";
        else
            throw std::runtime_error(
                "Kyle P229: unsupported weapon bank; rebuild weapon variant patches after animation changes");
        patches.push_back(pe2::resources::get("game/weapon_variants/kyle-" + profile + ".pe2mp"));
    }
    // Shared delta reader validates every preimage/bound before the first write.
    try {
        domain::costume::male::applyDeltas(patches, {stage});
    } catch (const std::exception &e) {
        throw std::runtime_error(std::string("Weapon variants: ") + e.what());
    }
}
} // namespace patches::weapon_variants
