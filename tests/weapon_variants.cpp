#include "patches/weapon_variants/WeaponVariants.hpp"
#include "ui/context/UIContext.hpp"
#include <chrono>
#include <iostream>
using Bytes = std::vector<uint8_t>;
static Bytes read(const std::filesystem::path &p) {
    std::ifstream f(p, std::ios::binary);
    return {std::istreambuf_iterator<char>(f), {}};
}
static void write(const std::filesystem::path &p, const Bytes &b) {
    std::ofstream f(p, std::ios::binary);
    f.write(reinterpret_cast<const char *>(b.data()), b.size());
    if (!f)
        throw std::runtime_error("fixture write failed");
}
static void require(bool ok, const char *why) {
    if (!ok)
        throw std::runtime_error(why);
}
int main(int argc, char **argv) {
    auto dir = std::filesystem::temp_directory_path() /
               ("pe2-weapons-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directory(dir);
    try {
        // The singleton owns its log until process exit. Keep it outside the
        // removable fixture directory (Windows forbids unlinking open files).
        auto logger = PE2Logger::getInstance(dir.string() + ".log");
        UIContext defaults(53765, logger);
        require(!defaults.convert().rupert_revolver && !defaults.convert().kyle_p229,
                "Variants must default off");
        for (unsigned mode = 0; mode < 4; ++mode) {
            auto ctx = defaults;
            if (mode & 1)
                ctx.flags |= UIContext::_UI_CONTEXT_FLAG__RUPERT_REVOLVER;
            if (mode & 2)
                ctx.flags |= UIContext::_UI_CONTEXT_FLAG__KYLE_P229;
            UIContext replay(0, logger);
            replay.deserialize(ctx.serialize());
            auto cfg = replay.convert();
            require(replay.serialize() == ctx.serialize(), "High-bit flags lost on seed roundtrip");
            require(cfg.rupert_revolver == bool(mode & 1) && cfg.kyle_p229 == bool(mode & 2),
                    "Wrong delivery flags");
            require(cfg.player_model_option == defaults.player_model_option,
                    "Variant changed character selection");
        }
        patches::weapon_variants::apply(dir, false, false); // no resource access when disabled
        if (argc == 3) {
            for (int profile = 1; profile <= 2; ++profile) {
                const auto original = read(argv[profile]);
                require(!original.empty(), "Missing local CDF fixture");
                const auto path = dir / "STAGE0.CDF";
                Bytes rupert, kyle;
                for (unsigned mode = 0; mode < 4; ++mode) {
                    write(path, original);
                    patches::weapon_variants::apply(dir, mode & 1, mode & 2);
                    auto changed = read(path);
                    require(changed.size() == original.size(), "CDF size changed");
                    if (!mode)
                        require(changed == original, "Disabled variants changed bytes");
                    if (mode == 1)
                        rupert = changed;
                    if (mode == 2)
                        kyle = changed;
                    if (mode == 3)
                        for (size_t i = 0; i < original.size(); ++i)
                            require(changed[i] == uint8_t(original[i] ^ rupert[i] ^ kyle[i]),
                                    "Variants interfere");
                }
                auto bad = original;
                bad[patches::weapon_variants::packageOffset + 24] ^= 1;
                write(path, bad);
                bool rejected = false;
                try {
                    patches::weapon_variants::apply(dir, true, true);
                } catch (const std::exception &) {
                    rejected = true;
                }
                require(rejected && read(path) == bad, "Unknown bank must fail before any mutation");
            }
        } else
            require(argc == 1, "Usage: pe2_weapon_variants [retail_CDF male_CDF]");
        std::filesystem::remove_all(dir);
        std::cout << "Weapon variant flags, defaults and optional native fixtures passed\n";
        return 0;
    } catch (const std::exception &e) {
        std::error_code ignored;
        std::filesystem::remove_all(dir, ignored);
        std::cerr << e.what() << '\n';
        return 1;
    }
}
