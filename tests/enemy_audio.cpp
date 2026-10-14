#include "domain/encounter/entities/Encounter.hpp"
#include "domain/enemy/entities/GolemSounds.hpp"
#include "domain/enemy/entities/SoundBudget.hpp"
#include "domain/enemy/usecases/EnemyRandomizer.hpp"
#include <iostream>
using namespace domain::enemy::audio;
static void require(bool ok, const char *message) {
    if (!ok)
        throw std::runtime_error(message);
}

static void synthetic() {
    const Banks day{{0x1214, 0x2e40}, {0x5214, 0x16770}};
    const Banks night{{0x1314, 0x2e40}, {0x5314, 0xb270}};
    const Banks pair{{0x4004, 0x9d00}, {0x4007, 0x7a40}};
    require(enemyCapacity(day) == 53824, "Water Tower day budget");
    require(!fits(day, pair), "Diver + Rat corrupt day Water Tower audio");
    require(fits(night, pair), "Safe night mixed encounter should remain allowed");
    require(fits(day, {{0x4004, 0x9d00}}), "Single Diver should fit");
    require(fits(day, {{0x400f, 0x4b70}, {0x4026, 0x4550}}), "Small sound banks should still mix");
    require(fits({}, {{0x4001, 0x26800}}), "Exact boundary should fit");
    require(!fits({}, {{0x4001, 0x26801}}), "One-byte overflow must fail");
    require(!fits({}, {{0x4001, 1}, {0x4002, 1}, {0x4003, 1}, {0x4004, 1}}), "Fourth enemy bank unsupported");
    require(!fits({{0x3001, 100}}, {{0x4001, 0xa001}}), "Type-3 fixed bank collision");
    require(!fits({{0x4002, 0x20000}}, {{0x4001, 0x10000}}), "Resident enemy bank must count");
    Banks shared{{0x4016, 0x85f0}};
    merge(shared, Bank{0x4016, 0x9e10});
    require(shared.size() == 1 && shared.at(0x4016) == 0x9e10, "Shared banks reserve largest variant once");

    const auto directory = std::filesystem::temp_directory_path() / "pe2-golem-audio-regression";
    std::filesystem::create_directories(directory);
    std::array<char, 368> bytes{};
    for (size_t n = 0; n < bytes.size(); ++n)
        bytes[n] = static_cast<char>(n);
    auto source = directory / "golem_sound_data.pe2e";
    {
        std::ofstream out(source, std::ios::binary);
        out.write(bytes.data(), bytes.size());
    }
    for (auto target : golemSoundTargets) {
        std::vector<char> fixture(target.offset + bytes.size() + 64, '\x5a');
        std::ofstream out(directory / (std::string(target.name) + ".pe2enm.decompr"), std::ios::binary);
        out.write(fixture.data(), fixture.size());
    }
    patchGolemSounds(directory);
    for (auto target : golemSoundTargets) {
        std::ifstream in(directory / (std::string(target.name) + ".pe2enm.decompr"), std::ios::binary);
        std::vector<char> actual((std::istreambuf_iterator<char>(in)), {});
        require(actual.size() == target.offset + bytes.size() + 64, "Sound patch extended overlay");
        for (size_t n = 0; n < actual.size(); ++n)
            require(actual[n] == (n >= target.offset && n < target.offset + bytes.size()
                                      ? bytes[n - target.offset]
                                      : '\x5a'),
                    "Sound patch changed adjacent data");
    }
    for (size_t size : {0u, 367u, 369u}) {
        std::ofstream(source, std::ios::binary | std::ios::trunc)
            .write(bytes.data(), std::min(size, bytes.size()));
        if (size == 369)
            std::ofstream(source, std::ios::binary | std::ios::app).put('x');
        bool rejected = false;
        try {
            patchGolemSounds(directory);
        } catch (const std::exception &) {
            rejected = true;
        }
        require(rejected, "Malformed sound table accepted");
    }
    std::filesystem::remove_all(directory);
}

// Optional local evidence: inspect the ACTUAL resource descriptors written in
// extracted room overlays, not the generator's candidate list. No ROM fixtures
// are shipped. Usage: pe2_enemy_audio /path/to/smoke/tmp [--allow-collisions]
namespace usecase::enemies {
struct EnemyRandomizerTestAccess {
    static void checkSelection(const std::filesystem::path &root) {
        auto log = PE2Logger::getInstance(
            (std::filesystem::temp_directory_path() / "pe2-audio-selection.log").string());
        auto rooms = domain::encounter::encounters_per_level_map.at(domain::game::DRYFIELD_DAY_LEVEL);
        auto room = *std::find_if(rooms.begin(), rooms.end(), [](const auto &r) {
            return r.name == domain::game::ROOM_DRYFIELD_DAY_WATER_TOWER;
        });
        auto banks = domain::enemy::audio::Catalog::loadRooms(root / "STAGE2.CDF").at(room.id);
        auto enc = room.encounters.back();
        domain::inner_config_t config{};
        config.randomized_enemies_pool = {SUCKLER_ID, FAILED_BRUTE_ID};
        bool oldPairSeen = false;
        for (unsigned seed = 0; seed < 128; ++seed) {
            EnemyRandomizer original(log, std::mt19937(seed), "", config);
            // Old selection path: no audio filter. It must actually select the
            // collision used by the separate sample-upload reproducer.
            const auto old = original.generateRandomEnemyIds(100, room, enc);
            if (old.size() == 2)
                oldPairSeen = true;
            EnemyRandomizer repaired(log, std::mt19937(seed), "", config);
            repaired.soundCatalog.loadEnemies(root / "STAGE0.CDF");
            repaired.roomSoundBanks = banks;
            repaired.checkSoundBudget = true;
            const auto selected = repaired.generateRandomEnemyIds(100, room, enc);
            require(selected.size() == 1, "Water Tower selection still admits Suckler + Failed Brute");
            repaired.roomSoundBanks = {{0x1314, 0x2e40}, {0x5314, 0xb270}};
            const auto nightSelected = repaired.generateRandomEnemyIds(100, room, enc);
            require(nightSelected.size() == 2, "Roomier sound budget should retain mixing");
        }
        require(oldPairSeen, "Reproducer pair was not selectable before audio filtering");
    }
};
} // namespace usecase::enemies

static int audit(const std::filesystem::path &root) {
    usecase::enemies::EnemyRandomizerTestAccess::checkSelection(root);
    Catalog catalog;
    catalog.loadEnemies(root / "STAGE0.CDF");
    for (auto [id, enemy] : enemies_map)
        for (auto [slot, file] : enemy.available_files)
            (void)catalog.enemy(enemy.file_id, file.first);
    require(catalog.enemy(0xe6, 2).empty(), "Dummy chain unexpectedly loads sound");
    unsigned checked = 0, collisions = 0;
    for (auto [level, rooms] : domain::encounter::encounters_per_level_map) {
        // Each level's room group uses the same stage ordinal (1..5).
        const auto stage = static_cast<unsigned>(level);
        const auto stageRoot = root / ("STAGE" + std::to_string(stage));
        const auto sounds = Catalog::loadRooms(root / ("STAGE" + std::to_string(stage) + ".CDF"));
        for (const auto &room : rooms) {
            auto name = getNameFromRoom(room.name);
            std::ifstream input(stageRoot / (name + ".pe2rml.decompr"), std::ios::binary);
            if (!input)
                throw std::runtime_error("Missing room fixture: " + name);
            std::vector<uint8_t> data((std::istreambuf_iterator<char>(input)), {});
            auto u32 = [&](size_t p) {
                if (p + 4 > data.size())
                    throw std::runtime_error("Room descriptor out of bounds");
                return word(data.data() + p);
            };
            for (const auto &enc : room.encounters) {
                if (enc.mask == domain::encounter::UNRANDOMIZED)
                    continue;
                const auto ptr = u32(enc.enemy_record_descriptor_list_addr_offset + 4);
                require(ptr >= 0x8017d5c0, "Invalid resource pointer");
                size_t pos = ptr - 0x8017d5c0;
                Banks banks;
                std::string ids;
                unsigned records = 0;
                for (;; pos += 12) {
                    require(pos + 12 <= data.size() && records < 16, "Unterminated resource list");
                    if (data[pos] == 255)
                        break;
                    ++records;
                    const auto file = data[pos + 2];
                    const uint16_t slot = data[pos + 4] | uint16_t(data[pos + 5]) << 8;
                    merge(banks, catalog.enemy(file, slot));
                    ids += std::to_string(data[pos]) + ",";
                }
                ++checked;
                const auto &roomBanks = sounds.at(room.id);
                if (!fits(roomBanks, banks)) {
                    ++collisions;
                    std::cout << "COLLISION STAGE" << stage << "/" << name << " encounter=" << int(enc.id)
                              << " actors=" << ids << " capacity=" << enemyCapacity(roomBanks) << '\n';
                }
            }
        }
    }
    std::cout << "Checked " << checked << " randomized encounters; collisions=" << collisions << '\n';
    return collisions ? 1 : 0;
}
int main(int argc, char **argv) {
    try {
        synthetic();
        if (argc > 1) {
            int result = audit(argv[1]);
            return argc > 2 ? 0 : result;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 2;
    }
}
