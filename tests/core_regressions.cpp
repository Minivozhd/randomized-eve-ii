#include "domain/armor/entities/Armor.hpp"
#include "domain/armor/entities/ArmorAssets.hpp"
#include "domain/armor/entities/ArmorLabels.hpp"
#include "domain/enemy/entities/DummyResources.hpp"
#include "domain/enemy/usecases/EnemyRandomizer.hpp"
#include "domain/item/usecases/ItemRandomizer.hpp"
#include "fixtures/playtest_20261012.hpp"
#include "resources/EmbeddedResources.hpp"
#include "ui/context/UIContext.hpp"
// Legacy armor flag macros share names with item category enum members.
#undef HP_RECOVERY
#undef MP_RECOVERY
#include <filesystem>
#include <iostream>
#include <stdexcept>

using namespace domain::item;
static void require(bool ok, const char *message) {
    if (!ok)
        throw std::runtime_error(message);
}
static void testHudPersistence(std::shared_ptr<PE2Logger> log) {
    UIContext source(59142, log);
    source.flags |= UIContext::_UI_CONTEXT_FLAG__HUD_COLORING;
    for (const auto rgb : std::vector<std::array<uint8_t, 3>>{{255, 51, 0}, {0, 0, 0}, {1, 127, 255}}) {
        source.hud_color_r = rgb[0];
        source.hud_color_g = rgb[1];
        source.hud_color_b = rgb[2];
        UIContext loaded(0, log);
        loaded.deserialize(source.serialize());
        require(loaded.hud_color_r == rgb[0] && loaded.hud_color_g == rgb[1] && loaded.hud_color_b == rgb[2],
                "HUD RGB lost on state load");
        auto config = loaded.convert();
        require(config.hud_color_r == rgb[0] && config.hud_color_g == rgb[1] && config.hud_color_b == rgb[2],
                "HUD RGB lost before generation");
        require(loaded.serialize() == source.serialize(), "New state round trip changed settings");
    }
    auto valid = source.serialize();
    // 222-byte legacy payload occupies exactly 296 Base64 characters.
    auto legacy = valid.substr(0, 296);
    source.deserialize(legacy);
    require(source.seed == 59142 && source.hud_color_r == 255 && source.hud_color_g == 255 &&
                source.hud_color_b == 255,
            "Legacy profile must load with deterministic neutral HUD");
    const auto before = source.serialize();
    for (const auto &invalid : std::vector<std::string>{valid.substr(0, 300), legacy + "SEVEMQABAg=="}) {
        bool rejected = false;
        try {
            source.deserialize(invalid);
        } catch (const std::runtime_error &) {
            rejected = true;
        }
        require(rejected && source.serialize() == before, "Invalid state extension changed live settings");
    }
}
static void testSpawnGrid() {
    using namespace utils::geometry_works;
    for (int origin : {-4096, 0, 4096}) {
        std::vector<Zone> zones{Zone({Point(origin, origin), Point(origin + 2048, origin),
                                      Point(origin + 2048, origin + 1536), Point(origin, origin + 1536)})};
        auto grid = createGrid(zones, {});
        require(grid.size() == 7 && grid.front().size() == 9,
                "Spawn grid bounds depend on uninitialized maxima");
        auto reference = findLongestRectangleInPolygons(zones, {});
        for (int i = 0; i < 10; ++i) {
            auto actual = findLongestRectangleInPolygons(zones, {});
            require(actual.topLeft.x == reference.topLeft.x && actual.topLeft.y == reference.topLeft.y &&
                        actual.width == reference.width && actual.length == reference.length,
                    "Spawn rectangle is not repeatable");
        }
    }
}
static void testDummyResources() {
    auto path = std::filesystem::temp_directory_path() / "pe2-dummy-task-regression.bin";
    constexpr size_t offset = domain::enemy::dummyTaskOffset;
    auto write = [&](const auto &b) {
        std::ofstream out(path, std::ios::binary | std::ios::trunc);
        out.write(reinterpret_cast<const char *>(b.data()), b.size());
    };
    auto read = [&]() {
        std::ifstream in(path, std::ios::binary);
        return std::vector<unsigned char>(std::istreambuf_iterator<char>(in), {});
    };
    for (size_t size : {size_t(30170), size_t(74860)}) {
        std::vector<unsigned char> original(size, 0xa5);
        if (size > offset)
            std::fill(original.begin() + offset, original.begin() + offset + 12, 0);
        write(original);
        domain::enemy::finalizeDummyResources(path);
        auto actual = read();
        require(actual.size() == std::max(size, offset + 12), "Dummy task exceeds required size");
        const std::vector<unsigned char> desc{0, 0, 0x60, 0, 0xc4, 0xde, 2, 0x80, 0, 0, 0, 0};
        require(std::equal(desc.begin(), desc.end(), actual.begin() + offset), "Invalid dummy TaskDesc");
        for (size_t i = 0; i < size; ++i)
            if (i < offset || i >= offset + 12)
                require(original[i] == actual[i], "Dummy patch changed neighboring resources");
        domain::enemy::finalizeDummyResources(path);
        require(read() == actual, "Dummy finalization is not idempotent");
    }
    for (size_t size : {offset + 12, size_t(0x17b31)}) {
        std::vector<unsigned char> collision(size, 0xa5);
        write(collision);
        bool rejected = false;
        try {
            domain::enemy::finalizeDummyResources(path);
        } catch (const std::runtime_error &) {
            rejected = true;
        }
        require(rejected && read() == collision, "Unsafe dummy layout was not rejected before write");
    }
    std::filesystem::remove(path);
}
static void testCloakResources() {
    auto root = std::filesystem::temp_directory_path() / "pe2-cloak-relocation-regression";
    std::filesystem::create_directories(root);
    std::vector<unsigned char> source(0x223cc, 0x37), fixed(0x123d4, 0);
    auto word = [](auto &b, size_t p, uint32_t v) {
        for (size_t i = 0; i < 4; ++i)
            b[p + i] = (v >> (8 * i)) & 255;
    };
    word(source, 0x22368, 0x00600001);
    word(source, 0x2236c, 0x80138340);
    word(source, 0x22370, 0x8013dbd4);
    word(source, 0x21ddc, 0x80153bec); // attack table pointer, previously not relocated
    std::copy(source.begin() + 0x17dec, source.end(), fixed.begin() + 0x7dec);
    word(fixed, 0x11ddc, 0x80173bec);
    auto write = [&](const char *name, const auto &data) {
        std::ofstream out(root / name, std::ios::binary | std::ios::trunc);
        out.write(reinterpret_cast<const char *>(data.data()), data.size());
    };
    write("16_knight_golem_slot_1.pe2enm.decompr", source);
    write("dummy_monster.pe2e.decompr", fixed);
    domain::enemy::finalizeDummyResources(root / "dummy_monster.pe2e.decompr");
    domain::enemy::validateCloakResources(root);
    for (size_t damaged : {size_t(0x1236c), size_t(0x11be8), size_t(0x11ddc), size_t(0x7dec)}) {
        auto bad = fixed;
        bad[damaged] ^= 0x80;
        write("dummy_monster.pe2e.decompr", bad);
        bool rejected = false;
        try {
            domain::enemy::validateCloakResources(root);
        } catch (const std::runtime_error &) {
            rejected = true;
        }
        require(rejected, "Missing/corrupt cloak startup or animation data passed generation guard");
    }
    std::filesystem::remove_all(root);
}
namespace usecase::items {
struct ItemRandomizerTestAccess {
    static void table(ItemRandomizer &r, domain::encounter::pe2_enc_rewards_segment_t segment,
                      std::vector<domain::encounter::pe2_enc_reward_descriptor_t> rows) {
        r.randomizeRewards(domain::game::AKROPOLIS_LEVEL, segment, rows);
    }
    static void shops(ItemRandomizer &r) {
        r.randomizeShops();
    }
    static void bonusReward(ItemRandomizer &r, uint8_t id) {
        r.best_dropped_unique_items = {{id, 100}};
        r.updateAkropolisScript();
        const auto &bucket = r.available_unique_items.at(LEGENDARY);
        require(std::find(bucket.begin(), bucket.end(), id) != bucket.end(),
                "Bonus weapon not restored to Legendary pool");
    }
    static void start(ItemRandomizer &r) {
        pe2_randomizer_context context{};
        r.randomizeStartingWeaponAndArmor(context);
    }
    static uint8_t roll(ItemRandomizer &r) {
        return r.randomizeItem({}, PICKUP_DEFAULT, false);
    }
    static void ammo(ItemRandomizer &r, uint8_t id) {
        r.available_ammo_items[COMMON] = {id};
    }
    static void boxes(ItemRandomizer &r, domain::game::pe2_level_name_t level) {
        r.randomizePickups(level, {}, false, false);
    }
    static void rewards(ItemRandomizer &r, size_t count) {
        for (size_t i = 0; i < count; ++i)
            r.best_dropped_unique_items.push_back({uint8_t(0x81 + i), 50});
        r.updateAkropolisScript();
        require(r.best_dropped_unique_items.size() == count,
                "Reward reordering changed the number of rewards");
    }
};
} // namespace usecase::items
static void testPlaytestStarts(std::shared_ptr<PE2Logger> log) {
    const auto dir = std::filesystem::temp_directory_path() / "pe2-playtest-starts";
    const auto base = dir / "STAGE0/00_base.pe2lvl.decompr";
    const auto gallery = dir / "STAGE1/19_shooting_gallery.pe2rml.decompr";
    for (auto p : {base, gallery}) {
        std::filesystem::create_directories(p.parent_path());
        std::ofstream f(p, std::ios::binary);
        std::vector<char> zeros(0x90000);
        f.write(zeros.data(), zeros.size());
    }
    auto byte = [&](const auto &path, size_t offset) {
        std::ifstream f(path, std::ios::binary);
        f.seekg(offset);
        return f.get();
    };
    for (auto state : playtestStates) {
        UIContext ui(0, log);
        ui.deserialize(state);
        const auto converted = ui.convert();
        domain::inner_config_t cfg;
        cfg.randomized_items_drop_pool = converted.randomized_items_drop_pool;
        cfg.weapon_randomization_strategy =
            static_cast<pe2_randomizer_strategy_t>(converted.weapon_randomization_strategy);
        cfg.flags.full_ammo = converted.flags.full_ammo;
        require(std::find(cfg.randomized_items_drop_pool.begin(), cfg.randomized_items_drop_pool.end(),
                          0x81) == cfg.randomized_items_drop_pool.end(),
                "Captured state enables M93R");
        for (unsigned seed = 0; seed < 96; ++seed) {
            usecase::items::ItemRandomizer writer(log, std::mt19937(seed), dir.string() + "/", cfg);
            usecase::items::ItemRandomizerTestAccess::start(writer);
            for (auto offset : {0x245f0, 0x2472c, 0x24758, 0x24a54, 0x27310, 0x27344}) {
                const int id = byte(base, offset);
                require(std::find(cfg.randomized_items_drop_pool.begin(),
                                  cfg.randomized_items_drop_pool.end(),
                                  id) != cfg.randomized_items_drop_pool.end(),
                        "Random start selected a forbidden weapon");
            }
            const int ammo = byte(base, 0x24744);
            require(std::find(cfg.randomized_items_drop_pool.begin(), cfg.randomized_items_drop_pool.end(),
                              ammo) != cfg.randomized_items_drop_pool.end(),
                    "Start granted forbidden ammunition");
        }
        // Flare survives explicit disable, persistence and config conversion.
        ui.items_drop_pool[0] &= ~UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__FLARE;
        ui.items_shop_pool[0] &= ~UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__FLARE;
        UIContext restored(0, log);
        restored.deserialize(ui.serialize());
        const auto off = restored.convert();
        require(std::find(off.randomized_items_drop_pool.begin(), off.randomized_items_drop_pool.end(),
                          0x3a) == off.randomized_items_drop_pool.end(),
                "Flare reenabled on round trip");
        require(std::find(off.randomized_items_shop_pool.begin(), off.randomized_items_shop_pool.end(),
                          0x3a) == off.randomized_items_shop_pool.end(),
                "Shop Flare reenabled on round trip");
    }
    for (uint8_t weapon : {0x8a, 0x8b}) {
        domain::inner_config_t cfg;
        cfg.weapon_randomization_strategy = RANDO_STRATEGY_FIXED;
        cfg.starting_weapon = weapon;
        cfg.randomized_items_drop_pool = {weapon, 0xa9, 0xaa};
        usecase::items::ItemRandomizer writer(log, std::mt19937(1), dir.string() + "/", cfg);
        usecase::items::ItemRandomizerTestAccess::start(writer);
        require(byte(base, 0x24744) == 0xa9, "Grenade start did not prefer enabled normal grenades");
        require(byte(gallery, 0x7974) == 0, "Unsupported launcher inserted into gallery whitelist");
    }
    std::filesystem::remove_all(dir);
}
static void testDropWeights(std::shared_ptr<PE2Logger> log) {
    using namespace domain::item;
    // A depleted unique pool renormalizes the remaining category weights.
    // Keep consumable pools stable so this measures the selector, not depletion.
    domain::inner_config_t cfg;
    cfg.randomized_items_drop_pool = {0x01, 0x06, 0xac, 0x04};
    cfg.item_classes_distribution = {
        {HP_RECOVERY, 16}, {MP_RECOVERY, 14}, {AMMO, 30}, {MISC, 7}, {UNIQUE, 33}};
    usecase::items::ItemRandomizer r(log, std::mt19937(2329920451U), "", cfg);
    usecase::items::ItemRandomizerTestAccess::ammo(r, 0xac);
    std::map<pe2_item_type_t, unsigned> counts;
    constexpr unsigned samples = 12000;
    for (unsigned i = 0; i < samples; ++i)
        ++counts[items_map.at(usecase::items::ItemRandomizerTestAccess::roll(r)).type];
    for (auto type : {HP_RECOVERY, MP_RECOVERY, AMMO, MISC}) {
        const double actual = double(counts[type]) / samples;
        const double expected = cfg.item_classes_distribution.at(type) / 67.0;
        require(std::abs(actual - expected) < 0.025,
                "Drop category ratio differs from eligible configured weights");
    }
}
static void testShopWeaponAmmo(std::shared_ptr<PE2Logger> log) {
    using namespace domain::item;
    const auto dir = std::filesystem::temp_directory_path() / "pe2-core-regression-shops";
    for (const auto &shop : shops_vector)
        for (const auto &location : shop.location) {
            auto path = dir / (location + std::string(FORMAT_ROOM_FILE) + FORMAT_DECOMPRESSED);
            std::filesystem::create_directories(path.parent_path());
            std::ofstream out(path, std::ios::binary);
            std::vector<char> bytes(0x40000, 0);
            out.write(bytes.data(), bytes.size());
        }
    // Exercise the actual shop writer with every selectable weapon/attachment,
    // including items with built-in charges and no consumable ammunition.
    for (uint8_t weapon : all_weapons_and_attachments) {
        domain::inner_config_t cfg;
        cfg.randomized_items_shop_pool = {weapon, 0xa0};
        usecase::items::ItemRandomizer r(log, std::mt19937(2630469624U), dir.string() + "/", cfg);
        usecase::items::ItemRandomizerTestAccess::shops(r);
        for (const auto &shop : shops_vector)
            for (size_t i = 0; i < shop.location.size(); ++i) {
                std::ifstream in(dir /
                                     (shop.location[i] + std::string(FORMAT_ROOM_FILE) + FORMAT_DECOMPRESSED),
                                 std::ios::binary);
                in.seekg(shop.offset[i]);
                require(in.get() == weapon && in.get() == 0, "Shop lost the selected weapon");
                auto ammoOffset = shop.offset[i] + ((shop.weapon * 2 + 2 + 3) & ~size_t(3));
                in.seekg(ammoOffset);
                for (size_t j = 0; j < shop.ammo; ++j) {
                    int id = in.get(), high = in.get();
                    if (j == 0)
                        require(id == 0xa0, "Shop ammo list empty despite an enabled ammo type");
                    require((id == 0 || id == 0xa0) && high == 0, "Shop leaked a disabled ammo type");
                }
            }
    }
    std::filesystem::remove_all(dir);
}
namespace usecase::enemies {
struct EnemyRandomizerTestAccess {
    static void room(EnemyRandomizer &r, std::fstream &file, domain::encounter::pe2_room_encounters_t room) {
        std::fstream unused;
        std::vector<std::vector<char>> descriptors;
        std::vector<domain::encounter::pe2_enc_reward_descriptor_t> rewards;
        size_t offset = room.encounters_offset, pool = 0;
        // Stage 0 bypasses archive relocation: this fixture checks the room
        // writer itself; the separate CDF test covers relocation.
        r.randomizeEnemiesInRoom(unused, file, room, descriptors, rewards, offset, pool, true,
                                 domain::game::STAGE0);
    }
    static void cachedZones(EnemyRandomizer &r) {
        using namespace utils::geometry_works;
        domain::encounter::pe2_room_encounters_t room{};
        room.environment.spawn_zones = {
            Zone({Point(-4000, -4000), Point(4000, -4000), Point(4000, 4000), Point(-4000, 4000)})};
        room.environment.dead_zones = {
            Zone({Point(-2000, -2000), Point(2000, -2000), Point(2000, 2000), Point(-2000, 2000)})};
        std::map<pe2_enemy_movement_t, EnemyRandomizer::SpawnZones> cache;
        auto first = enemies_map.at(PAWN_BLADE_GOLEM_ID);
        auto second = first;
        second.id = PAWN_GUN_GOLEM_ID;
        for (auto enemy : {first, second}) {
            const auto &zones = r.cachedSpawningZones(cache, room, enemy);
            require(zones.forbidden.size() == 1 && zones.fromWaterToGround,
                    "Cached enemy lost obstacle/water state");
            Circle obstacle(Point(0, 0), 128);
            require(!canPlaceCircle(obstacle, zones.allowed, zones.forbidden, false, {}),
                    "Cached second enemy may spawn inside obstacle");
        }
    }
    static void relocate(EnemyRandomizer &r, std::fstream &level) {
        domain::file::pe2file_t file{};
        file.filename = "01_square";
        file.file_offset = 0x800;
        domain::encounter::pe2_room_encounters_t room{};
        room.name = domain::game::ROOM_AKROPOLIS_SQUARE;
        r.updateConsequentFileOffset({file}, level, room, domain::game::STAGE1, 0x200);
    }
};
} // namespace usecase::enemies

static void testAccessTunnel(std::shared_ptr<PE2Logger> log, domain::inner_config_t cfg) {
    using namespace domain::enemy;
    using namespace domain::encounter;
    const auto &rooms = encounters_per_level_map.at(domain::game::MINES_AND_SHELTER_LEVEL);
    auto found = std::find_if(rooms.begin(), rooms.end(), [](const auto &r) {
        return r.name == domain::game::ROOM_MINES_AND_SHELTER_ACCESS_TUNNEL;
    });
    require(found != rooms.end(), "Access Tunnel metadata missing");
    auto room = *found;
    require(room.encounters_offset == 0x27f0 && room.encounters_end == 0x2880,
            "Unexpected Access Tunnel layout");
    auto path = std::filesystem::temp_directory_path() / "pe2-access-tunnel-regression.bin";
    // Synthetic data only. The last original terminator occupies 2880..288f;
    // 2890 onwards contains adjacent data and encounter pointers, not actors.
    std::vector<uint8_t> original(0x29dc, 0xa5);
    auto word = [](auto &b, size_t p, uint32_t v) {
        for (size_t n = 0; n < 4; ++n)
            b[p + n] = (v >> (n * 8)) & 255;
    };
    auto readWord = [](const auto &b, size_t p) {
        return uint32_t(b[p]) | uint32_t(b[p + 1]) << 8 | uint32_t(b[p + 2]) << 16 | uint32_t(b[p + 3]) << 24;
    };
    for (size_t p = 0x27f0; p <= 0x2880; p += 16) {
        std::fill(original.begin() + p, original.begin() + p + 16, 0);
        original[p] = DIVER_ID;
    }
    for (size_t p : {0x2840, 0x2860, 0x2880})
        original[p] = 0xff;
    const size_t starts[] = {0x27f0, 0x2850, 0x2870};
    for (size_t i = 0; i < 3; ++i)
        word(original, room.encounters[i].enemy_record_descriptor_list_addr_offset,
             ROOM_INFO_FILE_LOAD_ADDRESS + starts[i]);
    auto write = [&]() {
        std::ofstream out(path, std::ios::binary | std::ios::trunc);
        out.write(reinterpret_cast<const char *>(original.data()), original.size());
    };
    for (bool keepLast : {false, true})
        for (const auto &pool : std::vector<std::vector<uint8_t>>{
                 {DIVER_ID}, {DIVER_ID, RAT_ID}, {PAWN_BLADE_GOLEM_ID}, {DIVER_ID, PAWN_BLADE_GOLEM_ID}}) {
            auto variant = room;
            if (keepLast)
                variant.encounters.back().mask = UNRANDOMIZED;
            cfg.randomized_enemies_pool = pool;
            for (unsigned seed = 0; seed < 64; ++seed) {
                try {
                    write();
                    usecase::enemies::EnemyRandomizer r(log, std::mt19937(seed), "", cfg);
                    {
                        std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
                        usecase::enemies::EnemyRandomizerTestAccess::room(r, file, variant);
                    }
                    std::vector<uint8_t> actual;
                    {
                        std::ifstream in(path, std::ios::binary);
                        actual.assign(std::istreambuf_iterator<char>(in), {});
                    }
                    size_t next = 0x27f0;
                    for (size_t i = 0; i < 3; ++i) {
                        auto p =
                            readWord(actual, variant.encounters[i].enemy_record_descriptor_list_addr_offset) -
                            ROOM_INFO_FILE_LOAD_ADDRESS;
                        require(p == next, "Encounter pointer does not follow previous terminator");
                        size_t actors = 0;
                        while (true) {
                            require(p <= 0x2880, "Access Tunnel overwrote data after encounter end");
                            bool end = actual[p] == 0xff;
                            p += 16;
                            if (end)
                                break;
                            ++actors;
                        }
                        require(actors > 0, "Bounded encounter lost all actors");
                        if (keepLast && i == 2)
                            require(std::equal(original.begin() + 0x2870, original.begin() + 0x2890,
                                               actual.begin() + next),
                                    "Unrandomized encounter changed");
                        next = p;
                    }
                    for (size_t p = 0x2890; p < original.size(); ++p) {
                        bool pointer = false;
                        for (const auto &enc : room.encounters)
                            pointer |= p >= enc.enemy_record_descriptor_list_addr_offset &&
                                       p < enc.enemy_record_descriptor_list_addr_offset + 8;
                        if (!pointer)
                            require(actual[p] == original[p], "Adjacent room data changed");
                    }
                } catch (const std::exception &e) {
                    throw std::runtime_error("Access Tunnel seed=" + std::to_string(seed) +
                                             ", keepLast=" + std::to_string(keepLast) +
                                             ", poolFirst=" + std::to_string(pool.front()) + ": " + e.what());
                }
            }
        }
    // An impossible declared capacity must fail before patching the room.
    write();
    room.encounters_end = room.encounters_offset;
    bool threw = false;
    try {
        usecase::enemies::EnemyRandomizer r(log, std::mt19937(0), "", cfg);
        std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
        usecase::enemies::EnemyRandomizerTestAccess::room(r, file, room);
    } catch (const std::runtime_error &) {
        threw = true;
    }
    require(threw, "Impossible room capacity accepted");
    {
        std::ifstream in(path, std::ios::binary);
        std::vector<uint8_t> actual(std::istreambuf_iterator<char>(in), {});
        require(actual == original, "Capacity rejection modified the room");
    }
    room = *found;
    room.encounters.back().mask = UNRANDOMIZED;
    original[0x2880] = DIVER_ID;
    write();
    threw = false;
    try {
        usecase::enemies::EnemyRandomizer r(log, std::mt19937(0), "", cfg);
        std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
        usecase::enemies::EnemyRandomizerTestAccess::room(r, file, room);
    } catch (const std::runtime_error &) {
        threw = true;
    }
    require(threw, "Unterminated preserved encounter read past the record area");
    std::filesystem::remove(path);
}

static void testArmorCatalogue() {
    using namespace domain::armor;
    auto textFits = [](const std::string &name, const std::string &description) {
        require(name.size() + description.size() + 2 <= 80, "Armor label exceeds per-slot text budget");
        size_t start = 0, lines = 0;
        for (size_t i = 0; i < description.size(); ++i) {
            require(description[i] >= 32 || description[i] == '\n',
                    "Unsupported armor description character");
            if (description[i] == '\n') {
                require(i - start <= 32, "Armor description exceeds line width");
                start = i + 1;
                ++lines;
            }
        }
        require(lines <= 2 && start == description.size(), "Armor description must end within two lines");
    };
    const std::string assets = "game/armor/";
    for (const auto &[rarity, armors] : armors_pool)
        for (const auto &armor : armors) {
            textFits(armor.name, armor.description);
            const auto texture = pe2::resources::copy(assets + (armor.image_file + ".png.bin.compr"));
            const auto palette = pe2::resources::copy(assets + (armor.image_file + ".png.pal.compr"));
            try {
                validateArmorImage(texture, palette);
            } catch (const std::exception &e) {
                throw std::runtime_error(armor.name + ": " + e.what());
            }
            for (const auto &[id, slot] : armor_slots_map)
                if (slot.rarity == rarity)
                    require(texture.size() < slot.special_features_text_offset - 0x20 - slot.texture_offset,
                            "Armor image does not fit a possible destination slot");
        }
    size_t largestPool = 0;
    for (const auto &[id, slot] : armor_slots_map) {
        textFits(slot.default_name, slot.default_description);
        auto validateRecord = [&](const auto &name, const auto &description) {
            const auto record = armorLabelRecord(name, description, id == 0x65);
            size_t cursor = 0;
            std::vector<std::string> fields;
            for (int field = 0; field < 6; ++field) {
                const auto end = record.find('\n', cursor);
                require(end != std::string::npos, "Armor text parser escapes its six-field record");
                fields.push_back(record.substr(cursor, end - cursor));
                cursor = end + 1;
            }
            require(fields[0] == name, "Identified armor name changed");
            require(fields[3] == (id == 0x65 ? "Armor" : ""), "Unidentified armor borrows adjacent name");
            require(fields[4].empty() || fields[4] == "Unidentified armor.",
                    "Invalid unidentified description");
            require(fields[5].empty(), "Unidentified armor reads adjacent record");
            return record.size();
        };
        size_t largest = validateRecord(slot.default_name, slot.default_description);
        for (const auto &armor : armors_pool.at(slot.rarity))
            largest = std::max(largest, validateRecord(armor.name, armor.description));
        largestPool += largest;
    }
    require(largestPool <= ARMOR_LABELS_LIST_END_OFFSET - ARMOR_LABELS_LIST_START_OFFSET,
            "Six-field armor catalogue exceeds original text pool");
    auto texture = pe2::resources::copy(assets + "118mini.png.bin.compr");
    auto palette = pe2::resources::copy(assets + "118mini.png.pal.compr");
    // Six or more zeros yield to the next CD sector. The next strip must
    // begin at that boundary, otherwise the retail loader discards its start.
    size_t split = 0;
    decodeArmorStream(texture, split, 4096);
    while (split < texture.size() && texture[split] == 0)
        ++split;
    require(split < 2016, "Test first strip no longer fits first sector");
    auto aligned = texture;
    aligned.insert(aligned.begin() + split, 2016 - split, 0);
    validateArmorImage(aligned, palette);
    aligned.insert(aligned.begin() + 2016, 0);
    bool badAlignment = false;
    try {
        validateArmorImage(aligned, palette);
    } catch (const std::runtime_error &) {
        badAlignment = true;
    }
    require(badAlignment, "CD padding that discards a strip prefix accepted");
    // A fourth strip reproduces the oversized Green Suit failure. Reject it
    // even if its compressed byte count would fit the disk slot.
    size_t end = 0;
    decodeArmorStream(texture, end, 4096);
    const std::vector<char> extraStrip(texture.begin(), texture.begin() + end);
    texture.insert(texture.end(), extraStrip.begin(), extraStrip.end());
    bool threw = false;
    try {
        validateArmorImage(texture, palette);
    } catch (const std::runtime_error &) {
        threw = true;
    }
    require(threw, "Oversized inventory image accepted");
    threw = false;
    palette.resize(1);
    try {
        validateArmorImage(texture, palette);
    } catch (const std::runtime_error &) {
        threw = true;
    }
    require(threw, "Truncated palette accepted");
}
int main() {
    try {
        auto log = PE2Logger::getInstance("regressions.log");
        testHudPersistence(log);
        testPlaytestStarts(log);
        testSpawnGrid();
        usecase::enemies::EnemyRandomizer zoneWriter(log, std::mt19937(1), "", {});
        usecase::enemies::EnemyRandomizerTestAccess::cachedZones(zoneWriter);
        UIContext ui(53765, log);
        // Captured from the retired AK smoke build: loading it must keep
        // ordinary M4A1 pools/start while discarding the old mutator bit.
        UIContext legacyWeapons(0, log);
        legacyWeapons.deserialize(
            "BdIAAM4gBQj/////AQAAAGQAZABkAGQA/////9//EX//AQAAAAAAAP///////x9//"
            "wEAAAAAAABkALQAXgGgACADQAFEAqAAiBMQJywB9AEQJ3AXvgDIAMgPgAfoAxAnECcgThAnTB3QB7A2ZBnQB3AXQB/"
            "gLnAXNCFMHUAfmDoUCuj90Ad2FsQJcBfUMIgTzFugDxwl5FegD6hh6P1AnBAnZAAsAfQBoA8oI5ABIAOQAZYAXgEgA5YAbGt"
            "sa2xrbGvQB+gD9AFkABAOMBEFDgAAZAAA//////////8B");
        require((legacyWeapons.flags & 0x08000000u) == 0, "Retired weapon mutator survived preset load");
        auto legacyConfig = legacyWeapons.convert();
        require(legacyConfig.starting_weapon == 0x8f, "Retired preset changed the original M4A1 start");
        require(std::find(legacyConfig.randomized_items_drop_pool.begin(),
                          legacyConfig.randomized_items_drop_pool.end(),
                          0x42) != legacyConfig.randomized_items_drop_pool.end(),
                "M4A1 attachments still filtered");
        const auto cleanState = ui.serialize();
        ui.flags |= 0x08000000u;
        require(ui.serialize() == cleanState, "Retired mutator was serialized again");
        ui.flags &= ~0x08000000u;
        // Values above bit 31 must survive persistence on Windows as well as macOS.
        ui.items_drop_pool[0] = (uint64_t{1} << 50) | (uint64_t{1} << 63);
        ui.items_drop_pool[1] = 1;
        ui.items_shop_pool[0] = 0;
        ui.items_shop_pool[1] = 1;
        UIContext copy(1, log);
        copy.deserialize(ui.serialize());
        require(copy.items_drop_pool[0] == ui.items_drop_pool[0] && copy.items_drop_pool[1] == 1,
                "64-bit drop flags lost");
        require(copy.items_shop_pool[0] == 0 && copy.items_shop_pool[1] == 1, "Shop flags lost");
        auto converted = copy.convert();
        require(converted.randomized_items_shop_pool == std::vector<uint8_t>{0xae},
                "Slug shop mask reads wrong word");
        ui.items_drop_pool[0] = UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_RIOT;
        ui.items_drop_pool[1] = 0;
        require(ui.convert().randomized_items_drop_pool == std::vector<uint8_t>{0xaa},
                "Riot is wrong or duplicated");
        ui.flags &= ~UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_ITEMS;
        require(!ui.convert().flags.randomize_shops, "Disabled item randomization still changes shops");
        domain::inner_config_t cfg;
        cfg.item_classes_distribution = {
            {HP_RECOVERY, 16}, {MP_RECOVERY, 14}, {AMMO, 48}, {MISC, 17}, {UNIQUE, 5}};
        cfg.randomized_items_drop_pool = {0x01};
        cfg.randomized_items_shop_pool = {0xa0};
        usecase::items::ItemRandomizer onlyMedicine(log, std::mt19937(3325759884U), "", cfg);
        for (int i = 0; i < 1000; ++i)
            require(usecase::items::ItemRandomizerTestAccess::roll(onlyMedicine) == 0x01,
                    "Drop leaked a shop item or pistol ammo");
        cfg.randomized_items_drop_pool = {0xac};
        usecase::items::ItemRandomizer shotgun(log, std::mt19937(53765), "", cfg);
        usecase::items::ItemRandomizerTestAccess::ammo(shotgun, 0xac);
        for (int i = 0; i < 1000; ++i)
            require(usecase::items::ItemRandomizerTestAccess::roll(shotgun) == 0xac,
                    "Ammo restrictions ignored");
        cfg.randomized_items_drop_pool = {0x90};
        usecase::items::ItemRandomizer op(log, std::mt19937(53765), "", cfg);
        require(usecase::items::ItemRandomizerTestAccess::roll(op) == 0x90, "Enabled OP weapon cannot drop");
        bool threw = false;
        try {
            usecase::items::ItemRandomizerTestAccess::roll(op);
        } catch (const std::runtime_error &) {
            threw = true;
        }
        require(threw, "Exhausted unique pool silently fell back to forbidden items");
        // A box is chosen once from acquired ammo and remains stable on later passes.
        auto dir = std::filesystem::temp_directory_path() / "pe2-core-regression-boxes";
        std::filesystem::create_directories(dir / "STAGE0");
        auto path = dir / "STAGE0/01_akropolis.pe2lv.decompr";
        // Use the same names/extensions as the production code.
        path = dir / "STAGE0" /
               (domain::game::getNameFromLevel(domain::game::AKROPOLIS_LEVEL) +
                std::string(FORMAT_LEVEL_FILE) + FORMAT_DECOMPRESSED);
        {
            std::ofstream out(path, std::ios::binary);
            std::vector<char> bytes(0x3000);
            out.write(bytes.data(), bytes.size());
        }
        cfg.randomized_items_drop_pool = {0xac};
        cfg.ammo_boxes_rando_option = 1;
        usecase::items::ItemRandomizer box(log, std::mt19937(53765), dir.string() + "/", cfg);
        usecase::items::ItemRandomizerTestAccess::ammo(box, 0xac);
        usecase::items::ItemRandomizerTestAccess::boxes(box, domain::game::AKROPOLIS_LEVEL);
        usecase::items::ItemRandomizerTestAccess::ammo(box, 0xa0);
        usecase::items::ItemRandomizerTestAccess::boxes(box, domain::game::AKROPOLIS_LEVEL);
        {
            std::ifstream in(path, std::ios::binary);
            in.seekg(0x24ce);
            require(in.get() == 0xac, "Ammo box changed on repeated pickup pass");
        }
        for (int mode : {0, 1, 2}) {
            {
                std::fstream out(path, std::ios::in | std::ios::out | std::ios::binary);
                out.seekp(0x24ce);
                out.put(char(0xa0));
            }
            auto boxCfg = cfg;
            boxCfg.ammo_boxes_rando_option = mode;
            boxCfg.randomized_items_drop_pool = {0xac};
            usecase::items::ItemRandomizer b(log, std::mt19937(1), dir.string() + "/", boxCfg);
            usecase::items::ItemRandomizerTestAccess::ammo(b, 0xac);
            usecase::items::ItemRandomizerTestAccess::boxes(b, domain::game::AKROPOLIS_LEVEL);
            std::ifstream in(path, std::ios::binary);
            in.seekg(0x24ce);
            require(in.get() == 0xac, "Box mode leaked forbidden vanilla 9mm");
        }
        // Short replacement lists must not leave reachable vanilla reward rows
        // containing disabled items after the new rows.
        for (size_t count : {size_t(1), size_t(2)}) {
            std::vector<uint8_t> fixture(128, 0xa5);
            for (size_t i = 0; i < 2; ++i) {
                const size_t p = 16 + i * 12;
                fixture[p] = 0;
                fixture[p + 1] = uint8_t(i + 1);
                fixture[p + 2] = 2;
                fixture[p + 3] = 1;
                fixture[p + 4] = 0xaa; // disabled Riot in the old table
            }
            {
                std::ofstream out(path, std::ios::binary | std::ios::trunc);
                out.write(reinterpret_cast<char *>(fixture.data()), fixture.size());
            }
            auto rewardCfg = cfg;
            rewardCfg.randomized_items_drop_pool = {0x01};
            usecase::items::ItemRandomizer writer(log, std::mt19937(1), dir.string() + "/", rewardCfg);
            std::vector<domain::encounter::pe2_enc_reward_descriptor_t> rows(count);
            for (size_t i = 0; i < count; ++i) {
                rows[i].stage_id = 1;
                rows[i].room_id = 3;
                rows[i].encounter_id = uint8_t(i);
            }
            usecase::items::ItemRandomizerTestAccess::table(writer, {16, 2, domain::game::MODE_NIGTHMARE},
                                                            rows);
            std::vector<uint8_t> actual(fixture.size());
            {
                std::ifstream in(path, std::ios::binary);
                in.read(reinterpret_cast<char *>(actual.data()), actual.size());
            }
            for (size_t i = 0; i < count; ++i)
                require(actual[16 + i * 12 + 4] == 1, "Allowed reward not written");
            if (count < 2)
                require(actual[28] == 0xff && actual[29] == 0xff && actual[30] == 0xff && actual[31] == 0xff,
                        "Stale vanilla reward remains reachable");
            require(std::equal(actual.begin(), actual.begin() + 16, fixture.begin()) &&
                        std::equal(actual.begin() + 40, actual.end(), fixture.begin() + 40),
                    "Reward writer crossed reserved table");
        }
        std::filesystem::remove(path);
        path = dir / "STAGE0" /
               (domain::game::getNameFromLevel(domain::game::BASE_LEVEL) + std::string(FORMAT_LEVEL_FILE) +
                FORMAT_DECOMPRESSED);
        for (size_t count = 0; count <= 3; ++count) {
            std::vector<uint8_t> fixture(0x25000, 0xa5);
            {
                std::ofstream out(path, std::ios::binary);
                out.write(reinterpret_cast<const char *>(fixture.data()), fixture.size());
            }
            usecase::items::ItemRandomizer rewards(log, std::mt19937(1), dir.string() + "/", cfg);
            usecase::items::ItemRandomizerTestAccess::rewards(rewards, count);
            if (count > 0)
                fixture[0x245d0] = count == 3 ? 0x83 : 0x81;
            if (count > 1)
                fixture[0x245d8] = 0x82;
            if (count == 3) {
                fixture[0x245b0] = 0x7f;
                fixture[0x245b8] = 1;
            }
            std::vector<uint8_t> actual(fixture.size());
            {
                std::ifstream in(path, std::ios::binary);
                in.read(reinterpret_cast<char *>(actual.data()), actual.size());
            }
            require(actual == fixture, "Akropolis rewards changed missing slots or neighboring script bytes");
        }
        std::filesystem::create_directories(dir / "STAGE1");
        const auto gallery = dir / "STAGE1/19_shooting_gallery.pe2rml.decompr";
        {
            std::ofstream out(gallery, std::ios::binary);
            std::vector<char> data(0x10000);
            out.write(data.data(), data.size());
        }
        for (uint8_t id : {0x90, 0x95, 0x96}) {
            auto bonusCfg = cfg;
            bonusCfg.weapon_randomization_strategy = RANDO_STRATEGY_FIXED;
            bonusCfg.starting_weapon = id;
            bonusCfg.randomized_items_drop_pool = all_ammo;
            usecase::items::ItemRandomizer bonus(log, std::mt19937(1), dir.string() + "/", bonusCfg);
            usecase::items::ItemRandomizerTestAccess::start(bonus);
            usecase::items::ItemRandomizerTestAccess::bonusReward(bonus, id);
            {
                std::ifstream in(path, std::ios::binary);
                in.seekg(0x245d0);
                require(in.get() == id, "Bonus reward script lost weapon");
            }
        }
        std::filesystem::remove(gallery);
        std::filesystem::remove(dir / "STAGE1");
        std::filesystem::remove(path);
        std::filesystem::remove(dir / "STAGE0");
        std::filesystem::remove(dir);
        auto archiveDir = std::filesystem::temp_directory_path() / "pe2-core-regression-archive";
        std::filesystem::create_directories(archiveDir);
        auto archivePath = archiveDir / "STAGE1.CDF";
        std::vector<uint8_t> bytes(0x2800, 0);
        auto word = [&](size_t offset, uint32_t value) {
            for (size_t i = 0; i < 4; ++i)
                bytes[offset + i] = (value >> (i * 8)) & 255;
        };
        bytes[0x800] = 1;
        bytes[0x801] = 1;
        word(0x804, 2);
        word(0x808, 0x8017d5c0);
        bytes[0x1800] = 4;
        bytes[0x1801] = 255;
        word(0x1804, 2);
        word(0x1808, 0x80190000);
        word(0x1818, 0x80191000);
        word(0x181c, 0x80192000);
        // Data after the fixed CAP directory must not be treated as headers.
        word(0x1c08, 0x80193000);
        {
            std::ofstream out(archivePath, std::ios::binary);
            out.write(reinterpret_cast<char *>(bytes.data()), bytes.size());
        }
        usecase::enemies::EnemyRandomizer enemy(log, std::mt19937(1), archiveDir.string() + "/", cfg);
        std::fstream unused;
        usecase::enemies::EnemyRandomizerTestAccess::relocate(enemy, unused);
        std::vector<uint8_t> actual(bytes.size());
        {
            std::ifstream in(archivePath, std::ios::binary);
            in.read(reinterpret_cast<char *>(actual.data()), actual.size());
        }
        word(0x1808, 0x80190200);
        word(0x1818, 0x80191200);
        word(0x181c, 0x80192200);
        require(actual == bytes,
                "Relocation changed disk sector counts, CAP payload, or omitted a destination");
        std::filesystem::remove(archivePath);
        std::filesystem::remove(archiveDir);
        testAccessTunnel(log, cfg);
        testArmorCatalogue();
        testDropWeights(log);
        testShopWeaponAmmo(log);
        testDummyResources();
        testCloakResources();
        std::cout
            << "Item masks, persistence, drops, ammo boxes, CDF relocation and encounter bounds passed\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
