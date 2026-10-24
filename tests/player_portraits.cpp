#include "domain/costume/usecases/Portraits.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace domain::costume;
using namespace domain::costume::portraits;
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
        b.push_back(n >> (8 * i));
}
int main(int argc, char **argv) {
    auto dir =
        std::filesystem::temp_directory_path() /
        ("pe2-portrait-test-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directory(dir);
    try {
        require(argc == 1 || argc == 2, "Usage: pe2_player_portraits [STAGE0.CDF]");
        // Guard the entire instruction set before touching the gameplay overlay.
        std::string renderer(0x2df90, '\x5a');
        const std::pair<unsigned, unsigned> instructions[] = {
            {0x2df60, 0x2442ff8e}, {0x2df70, 0x24020038}, {0x2df78, 0x24020060}, {0x2df80, 0x24020040}};
        auto putWord = [](std::string &s, unsigned off, unsigned value) {
            for (unsigned i = 0; i < 4; ++i)
                s[off + i] = static_cast<char>(value >> (8 * i));
        };
        for (auto [off, value] : instructions)
            putWord(renderer, off, value);
        auto expectedRenderer = renderer;
        putWord(expectedRenderer, 0x2df60, 0x2442ff4e);
        putWord(expectedRenderer, 0x2df70, 0x24020040);
        putWord(expectedRenderer, 0x2df78, 0x240200e8);
        putWord(expectedRenderer, 0x2df80, 0x24020080);
        std::stringstream validRenderer(renderer);
        applyNameLayout(validRenderer);
        require(validRenderer.str() == expectedRenderer, "Name renderer changed unrelated bytes");
        for (auto [off, value] : instructions) {
            auto invalid = renderer;
            putWord(invalid, off, value ^ 1);
            std::stringstream stream(invalid);
            bool rejected = false;
            try {
                applyNameLayout(stream);
            } catch (const std::exception &) {
                rejected = true;
            }
            require(rejected && stream.str() == invalid, "Name renderer validation is not atomic");
        }
        require(assetKey(PE2_COSTUME_DEFAULT).empty(), "Default must preserve retail");
        require(assetKey(PE2_COSTUME_SHOULDER_HOLSTER_WITH_EAR_PROTECTION) ==
                    assetKey(PE2_COSTUME_SHOULDER_HOLSTER),
                "Aya ear protection must share Aya portrait");
        for (const auto &[id, c] : pe2_costumes_map)
            require(!assetKey(id).empty(), "Uncovered costume");
        require(pe2_costumes_map.size() == 20, "Catalog changed: update portrait coverage");
        Bytes original(0x7b020, 0x5a), patch{'P', 'E', '2', 'P', 'T', '0', '0', '1'}, expected = original;
        word(patch, 2);
        for (auto [off, len] :
             std::vector<std::pair<unsigned, unsigned>>{{0x77800, 0x3000}, {0x7a800, 0x800}}) {
            Bytes before(len, 0x5a), after(len, 0x33);
            word(patch, off);
            word(patch, len);
            word(patch, hash(before));
            word(patch, hash(after));
            patch.insert(patch.end(), len, 0x69);
            std::fill(expected.begin() + off, expected.begin() + off + len, 0x33);
        }
        const auto target = dir / "STAGE0.CDF", delta = dir / "test.pe2pt";
        write(target, original);
        write(delta, patch);
        applyDelta(read(delta), target);
        require(read(target) == expected, "Positive patch/bounds failed");
        auto reject = [&](const Bytes &p, const Bytes &input) {
            write(delta, p);
            write(target, input);
            bool caught = false;
            try {
                applyDelta(read(delta), target);
            } catch (const std::exception &) {
                caught = true;
            }
            require(caught && read(target) == input, "Invalid patch mutated target");
        };
        auto bad = patch;
        bad.back() ^= 1;
        reject(bad, original); // second checksum: no partial first write
        bad = patch;
        bad.resize(bad.size() - 1);
        reject(bad, original);
        bad = patch;
        bad.push_back(0);
        reject(bad, original);
        bad = patch;
        bad[13] ^= 1;
        reject(bad, original); // forbidden offset
        bad = original;
        bad[0x7a820] ^= 1;
        reject(patch, bad); // unsupported source
        reject(patch, Bytes(0x77810));
        if (argc == 2) {
            auto retail = read(argv[1]);
            require(retail.size() > 0x7b000, "Missing local retail CDF");
            retail.resize(0x7b020); // only fixed UI slots are needed, plus boundary sentinels
            for (const auto &[id, c] : pe2_costumes_map) {
                write(target, retail);
                const auto key = assetKey(id);
                applyDelta(pe2::resources::get("game/portraits/" + key + ".pe2pt"), target);
                auto actual = read(target);
                require(actual.size() == retail.size(), "CDF size changed");
                for (size_t i = 0; i < actual.size(); ++i)
                    if (i < 0x77800 || i >= 0x7b000)
                        require(actual[i] == retail[i], "Write outside UI slots");
                require(actual != retail, "Portrait not applied");
                std::cout << int(id) << " " << key << " PASS\n";
            }
        }
        std::filesystem::remove_all(dir);
        std::cout << "Portrait coverage, guards, atomic validation and write bounds passed\n";
        return 0;
    } catch (const std::exception &e) {
        std::filesystem::remove_all(dir);
        std::cerr << e.what() << '\n';
        return 1;
    }
}
