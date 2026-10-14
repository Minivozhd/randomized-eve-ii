#include "domain/costume/usecases/MalePlayers.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
using namespace domain::costume;
using namespace domain::costume::male;
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
static uint32_t hash(const Bytes &b) {
    uint32_t h = 2166136261;
    for (auto x : b)
        h = (h ^ x) * 16777619u;
    return h;
}
static void word(Bytes &b, uint32_t n) {
    for (int i = 0; i < 4; ++i)
        b.push_back(n >> (i * 8));
}
int main(int argc, char **argv) {
    auto dir =
        std::filesystem::temp_directory_path() /
        ("pe2-male-test-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directory(dir);
    try {
        require(argc == 1 || argc == 8,
                "Usage: pe2_male_players [STAGE0 gameplay STAGE4 STAGE5 STAGE1 STAGE2 STAGE3]");
        require(assetKey(PE2_COSTUME_DEFAULT).empty() && assetKey(PE2_COSTUME_RE1_JILL).empty(),
                "Existing characters changed profile");
        unsigned count = 0;
        for (auto [id, c] : pe2_costumes_map)
            if (!assetKey(id).empty()) {
                ++count;
                require(c.slot == PE2_COSTUME_SLOT_3 && !c.custom, "Invalid male slot");
            }
        require(count == 9, "Missing male models");
        auto first = dir / "STAGE0.CDF", second = dir / "gameplay", patchFile = dir / "test.pe2mp";
        Bytes a(256, 0x12), b(256, 0x23), patch{'P', 'E', '2', 'M', 'P', '0', '0', '1'};
        word(patch, 2);
        for (uint32_t i = 0; i < 2; ++i) {
            Bytes old(8, i ? 0x23 : 0x12), next(8, 0x66);
            word(patch, i);
            word(patch, 16);
            word(patch, 8);
            word(patch, hash(old));
            word(patch, hash(next));
            for (auto x : old)
                patch.push_back(x ^ 0x66);
        }
        write(first, a);
        write(second, b);
        write(patchFile, patch);
        applyDeltas({read(patchFile)}, {first, second});
        auto aa = a, bb = b;
        std::fill(aa.begin() + 16, aa.begin() + 24, 0x66);
        std::fill(bb.begin() + 16, bb.begin() + 24, 0x66);
        require(read(first) == aa && read(second) == bb, "Positive patch/bounds failed");
        auto reject = [&](Bytes bad) {
            write(first, a);
            write(second, b);
            write(patchFile, bad);
            bool caught = false;
            try {
                applyDeltas({read(patchFile)}, {first, second});
            } catch (const std::exception &) {
                caught = true;
            }
            require(caught && read(first) == a && read(second) == b, "Invalid patch partially applied");
        };
        auto bad = patch;
        bad.back() ^= 1;
        reject(bad);
        bad = patch;
        bad.pop_back();
        reject(bad);
        bad = patch;
        bad.push_back(0);
        reject(bad);
        bad = patch;
        bad[12] = 2;
        reject(bad);
        bad = patch;
        bad[8] = 0;
        reject(bad);
        bad = patch;
        bad[40] = 0;
        reject(bad); // overlap second record
        if (argc == 8) {
            auto retail = read(argv[1]), game = read(argv[2]), stage4 = read(argv[3]), stage5 = read(argv[4]);
            require(retail.size() > 0x820000 && !game.empty() && stage4.size() > 0x800000 &&
                        stage5.size() > 0x1360000,
                    "Missing local retail files");
            retail.resize(0x820000);
            auto stage1 = read(argv[5]), stage2 = read(argv[6]), stage3 = read(argv[7]);
            auto fourth = dir / "STAGE4.CDF", fifth = dir / "STAGE5.CDF";
            for (auto [id, c] : pe2_costumes_map) {
                const auto key = assetKey(id);
                if (key.empty())
                    continue;
                write(first, retail);
                write(second, game);
                write(fourth, stage4);
                write(fifth, stage5);
                write(dir / "STAGE1.CDF", stage1);
                write(dir / "STAGE2.CDF", stage2);
                write(dir / "STAGE3.CDF", stage3);
                applyDeltas({pe2::resources::get("game/players/" + key + ".pe2mp"),
                             pe2::resources::get("game/players/shared.pe2mp")},
                            {first, second, fourth, fifth, dir / "STAGE1.CDF", dir / "STAGE2.CDF",
                             dir / "STAGE3.CDF"});
                require(read(first).size() == retail.size() && read(second).size() == game.size(),
                        "Resource size changed");
                require(read(first) != retail && read(fourth) != stage4 && read(fifth) != stage5,
                        "Male patch was not applied");
                require(read(dir / "STAGE1.CDF") != stage1 && read(dir / "STAGE2.CDF") != stage2 &&
                            read(dir / "STAGE3.CDF") != stage3,
                        "CD-audio restore patch was not applied");
                auto patchedGame = read(second);
                constexpr size_t aimStart = 0x80103f70 - 0x80093800, aimEnd = 0x8010403c - 0x80093800;
                require(std::equal(game.begin(), game.begin() + aimStart, patchedGame.begin()) &&
                            std::equal(game.begin() + aimEnd, game.end(), patchedGame.begin() + aimEnd),
                        "Male patch changed gameplay outside the bounded aim dispatcher (including death "
                        "dispatch)");
                require(!std::equal(game.begin() + aimStart, game.begin() + aimEnd,
                                    patchedGame.begin() + aimStart),
                        "P08 shoulder aim dispatcher was not installed");
                std::cout << key << " PASS\n";
            }
        }
        std::filesystem::remove_all(dir);
        std::cout << "Male catalog, checksums, bounds and atomic validation passed\n";
        return 0;
    } catch (const std::exception &e) {
        std::filesystem::remove_all(dir);
        std::cerr << e.what() << '\n';
        return 1;
    }
}
