#include "delivery/Manager.hpp"
#include "domain/music/usecases/MusicRandomizer.hpp"
#include "domain/music/usecases/Pe1Music.hpp"
#include "domain/randomizer/usecases/Randomizer.hpp"
#include "ui/context/UIContext.hpp"
#include "utils/RuntimePaths.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>

using namespace domain::music;
int main(int argc, char **argv) {
    if (argc == 3 && std::string(argv[1]) == "--validate-resources") {
        utils::paths::resourceRoot = argv[2];
        pe1::validatePackage();
        std::cout << "PASS: bundled PE1 music pack, 37 validated tracks\n";
        return 0;
    }
    const auto log =
        PE2Logger::getInstance((std::filesystem::temp_directory_path() / "pe1-regression.log").string());
    UIContext ui(39197, log), copy(1, log);
    ui.flags |= UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_MUSIC;
    const auto legacy = ui.serialize();
    ui.pe1_music = true;
    const auto state = ui.serialize();
    assert(state != legacy);
    copy.deserialize(state);
    assert(copy.pe1_music && copy.serialize() == state);
    assert(copy.convert().pe1_music && !copy.convert().flags.randomize_music);
    assert(copy.flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_MUSIC);
    copy.pe1_music = false;
    assert(copy.serialize() == legacy && copy.convert().flags.randomize_music);
    copy.pe1_music = true;
    copy.deserialize(legacy);
    assert(!copy.pe1_music && copy.serialize() == legacy);
    copy.pe1_music = true;
    copy.deserialize(legacy.substr(0, 296));
    assert(!copy.pe1_music);

    assert(pe1::tracks().size() == 37);
    assert(pe1::pool(MUSIC_PEACEFUL).size() == 7);
    assert(pe1::pool(MUSIC_EXPLORING).size() == 9);
    assert(pe1::pool(MUSIC_INTENSE).size() == 7);
    assert(pe1::pool(MUSIC_FIGHT).size() == 14);
    assert(pe1::pool(MUSIC_MISC).size() == 37);
    std::set<uint32_t> ids;
    std::set<std::pair<uint32_t, uint32_t>> sources;
    for (const auto &t : pe1::tracks()) {
        assert(ids.insert(t.id).second && sources.insert({t.source, t.phase}).second);
        assert(t.source != 0x18 && t.source != 0x1d);
        assert(t.id != 0x4f && t.id != 0x5a && t.id < 0x80);
        assert(t.length <= t.slotBytes);
        for (const auto &other : pe1::tracks())
            if (t.id != other.id)
                assert(t.offset + t.slotBytes <= other.offset || other.offset + other.slotBytes <= t.offset);
    }
    for (auto p : {2u, 3u, 4u})
        assert(sources.count({0x27, p}) == 1);
    assert(sources.count({4, 1}) == 1);

    const auto temp = std::filesystem::temp_directory_path() /
                      ("pe2-pe1-music-regression-" +
                       std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::remove_all(temp);
    std::filesystem::create_directories(temp / "STAGE0");
    auto write = [&](const auto &path, size_t n) {
        std::ofstream out(path, std::ios::binary);
        std::string zero(n, 0);
        out.write(zero.data(), zero.size());
    };
    std::vector<char> first;
    for (int run = 0; run < 2; ++run) {
        for (const auto &[level, entries] : music_entries_map) {
            size_t size = 0;
            for (const auto &entry : entries)
                for (auto off : entry.offsets)
                    size = std::max(size, off + 1);
            write(temp / "STAGE0" /
                      (domain::game::getNameFromLevel(level) + FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED),
                  size);
        }
        domain::inner_config_t config{};
        config.pe1_music = true;
        usecase::music::MusicRandomizer randomizer(log, std::mt19937(42), temp.string(), config);
        randomizer.randomize({});
        std::vector<char> actual;
        for (const auto &[level, entries] : music_entries_map) {
            std::ifstream in(
                temp / "STAGE0" /
                    (domain::game::getNameFromLevel(level) + FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED),
                std::ios::binary);
            // The legacy scene map lists Neo Ark 0x33C6 twice. Preserve its
            // established last-writer behavior; check the effective entry.
            std::map<size_t, const pe2_music_entry_t *> effective;
            for (const auto &entry : entries)
                for (auto off : entry.offsets)
                    effective[off] = &entry;
            for (const auto &entry : entries) {
                const auto pool = pe1::pool(entry.type);
                for (auto off : entry.offsets) {
                    if (effective[off] != &entry)
                        continue;
                    in.seekg(off);
                    const auto id = uint8_t(in.get());
                    actual.push_back(char(id));
                    assert(std::find(pool.begin(), pool.end(), id) != pool.end());
                    assert(std::find(entry.forbidden_music.begin(), entry.forbidden_music.end(), id) ==
                           entry.forbidden_music.end());
                }
            }
        }
        if (run == 0)
            first = actual;
        else
            assert(first == actual);
    }
    const auto bad = temp / "invalid.pack";
    write(bad, 12);
    bool rejected = false;
    try {
        pe1::validatePackage(bad);
    } catch (const std::runtime_error &) {
        rejected = true;
    }
    assert(rejected);
    rejected = false;
    try {
        pe1::validatePackage(temp / "missing.pack");
    } catch (const std::runtime_error &) {
        rejected = true;
    }
    assert(rejected);
    {
        const auto oldResources = utils::paths::resourceRoot, oldWork = utils::paths::workRoot;
        utils::paths::resourceRoot = temp;
        utils::paths::workRoot = temp;
        std::filesystem::create_directories(temp / "tmp");
        const auto sentinel = temp / "tmp" / "keep.txt";
        write(sentinel, 1);
        std::atomic<bool> cancel(false);
        auto progress = [](int, std::string) {};
        utils::lzss::Compressor compressor(cancel, log);
        domain::file::FileUsecase files(compressor, progress, cancel, log);
        domain::rom::RomUsecase rom("missing-disc1", "missing-disc2", cancel);
        usecase::Randomizer randomizer(log);
        delivery::Manager manager(randomizer, files, rom, cancel, progress);
        auto config = ui.convert();
        config.pe1_music = true;
        assert(manager.randomize(config) == -4);
        assert(manager.lastError().find("PE1 music pack unavailable") != std::string::npos);
        assert(std::filesystem::exists(sentinel));
        utils::paths::resourceRoot = oldResources;
        utils::paths::workRoot = oldWork;
    }
    // Exercise the production executable patch, including a late preimage
    // mismatch. This fixture is only a small gain table inside synthetic data.
    const std::array<uint8_t, 100> nativeLevels{
        96,  0,  0,   62,  90, 70, 121, 88,  90,  127, 106, 106, 100, 0,   0,   84,  0,   80, 109, 0,
        80,  80, 80,  80,  0,  0,  0,   0,   0,   96,  96,  96,  97,  98,  99,  100, 80,  51, 122, 0,
        80,  58, 94,  127, 69, 90, 98,  85,  80,  127, 90,  80,  80,  80,  94,  85,  127, 85, 94,  114,
        85,  80, 127, 90,  80, 90, 118, 125, 120, 116, 86,  127, 127, 127, 127, 105, 96,  80, 112, 127,
        123, 82, 106, 96,  96, 96, 87,  96,  96,  98,  127, 96,  96,  86,  120, 124, 124, 96, 90,  96};
    auto readBytes = [](const std::filesystem::path &path) {
        std::ifstream in(path, std::ios::binary);
        return std::vector<uint8_t>(std::istreambuf_iterator<char>(in), {});
    };
    auto writeBytes = [](const std::filesystem::path &path, const std::vector<uint8_t> &bytes) {
        std::ofstream out(path, std::ios::binary);
        out.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());
    };
    const auto executable = temp / "SLUS.CDF";
    std::vector<uint8_t> baseline(0x59800, 0xa5);
    const std::string magic = "PS-X EXE";
    std::copy(magic.begin(), magic.end(), baseline.begin());
    std::copy(nativeLevels.begin(), nativeLevels.end(), baseline.begin() + 0x591f0);
    if (argc == 4)
        baseline = readBytes(argv[3]);
    auto boosted = baseline;
    for (const auto &track : pe1::tracks()) {
        assert(baseline.at(0x591f0 + track.id) == nativeLevels.at(track.id));
        boosted[0x591f0 + track.id] = (nativeLevels[track.id] * 13 + 5) / 10;
        // Sequence startup uses table * 96, below full SPU direct gain.
        assert(boosted[0x591f0 + track.id] * 96 < 0x4000);
    }
    writeBytes(executable, baseline);
    pe1::applyVolumeBoost(temp);
    assert(readBytes(executable) == boosted);
    baseline[0x591f0 + pe1::tracks().back().id] ^= 1;
    writeBytes(executable, baseline);
    rejected = false;
    try {
        pe1::applyVolumeBoost(temp);
    } catch (const std::runtime_error &) {
        rejected = true;
    }
    assert(rejected && readBytes(executable) == baseline);
    writeBytes(executable, std::vector<uint8_t>(16, 0));
    rejected = false;
    try {
        pe1::applyVolumeBoost(temp);
    } catch (const std::runtime_error &) {
        rejected = true;
    }
    assert(rejected && readBytes(executable) == std::vector<uint8_t>(16, 0));
    std::cout << "PASS: PE1 song gains +30%, all unrelated executable bytes unchanged, "
                 "invalid/late preimage rejected before writes\n";
    if (argc >= 3) {
        auto read = [](const std::filesystem::path &path) {
            std::ifstream in(path, std::ios::binary);
            return std::vector<uint8_t>(std::istreambuf_iterator<char>(in), {});
        };
        const auto original = read(argv[1]);
        const auto package = read(argv[2]);
        auto expected = original;
        size_t at = 12;
        for (const auto &t : pe1::tracks()) {
            at += 32;
            assert(uint64_t(t.offset) + t.length <= expected.size());
            for (size_t j = 0; j < t.length; ++j)
                expected[t.offset + j] ^= package.at(at + j);
            at += t.length;
        }
        assert(at == package.size());
        const auto cdf = temp / "STAGE0.CDF";
        std::filesystem::copy_file(argv[1], cdf, std::filesystem::copy_options::overwrite_existing);
        pe1::apply(temp, argv[2]);
        assert(read(cdf) == expected);
        // A late bad preimage must reject before touching the earlier banks.
        std::filesystem::copy_file(argv[1], cdf, std::filesystem::copy_options::overwrite_existing);
        {
            std::fstream out(cdf, std::ios::binary | std::ios::in | std::ios::out);
            const auto pos = pe1::tracks().back().offset;
            out.seekp(pos);
            out.put(char(original[pos] ^ 1));
        }
        const auto before = read(cdf);
        rejected = false;
        try {
            pe1::apply(temp, argv[2]);
        } catch (const std::runtime_error &) {
            rejected = true;
        }
        assert(rejected && read(cdf) == before);
        auto corrupt = package;
        corrupt.back() ^= 1;
        {
            std::ofstream out(bad, std::ios::binary);
            out.write(reinterpret_cast<const char *>(corrupt.data()), corrupt.size());
        }
        rejected = false;
        try {
            pe1::apply(temp, bad);
        } catch (const std::runtime_error &) {
            rejected = true;
        }
        assert(rejected && read(cdf) == before);
        std::cout << "PASS: exact full-CDF patch, all other bytes unchanged, late preimage and payload "
                     "corruption rejected before writes\n";
    }
    std::filesystem::remove_all(temp);
    std::cout << "PASS: PE1 pools, phases, settings compatibility, exclusivity, deterministic scene "
                 "assignments and pack rejection\n";
}
