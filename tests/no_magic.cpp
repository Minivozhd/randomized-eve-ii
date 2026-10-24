#include "delivery/Manager.hpp"
#include "domain/magic/usecases/NoMagic.hpp"
#include "ui/context/UIContext.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
namespace {
const std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
std::string decode(const std::string &s) {
    std::string out;
    unsigned bits = 0, count = 0;
    for (char c : s) {
        if (c == '=')
            break;
        auto v = alphabet.find(c);
        assert(v != std::string::npos);
        bits = (bits << 6) | unsigned(v);
        count += 6;
        if (count >= 8) {
            count -= 8;
            out.push_back(char(bits >> count));
        }
    }
    return out;
}
std::string encode(const std::string &s) {
    std::string out;
    unsigned bits = 0, count = 0;
    for (unsigned char c : s) {
        bits = (bits << 8) | c;
        count += 8;
        while (count >= 6) {
            count -= 6;
            out.push_back(alphabet[(bits >> count) & 63]);
        }
    }
    if (count)
        out.push_back(alphabet[(bits << (6 - count)) & 63]);
    while (out.size() % 4)
        out.push_back('=');
    return out;
}
} // namespace

int main(int argc, char **argv) {
    auto log = PE2Logger::getInstance((std::filesystem::temp_directory_path() / "pe2-no-magic.log").string());
    UIContext source(93, log), copy(0, log);
    source.flags |= UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_MAGIC |
                    UIContext::_UI_CONTEXT_FLAG__MAGIC_ELEMENTS |
                    UIContext::_UI_CONTEXT_FLAG__ACTIVE_TIME_BATTLE;
    const auto original = source.serialize();
    for (bool pe1 : {false, true})
        for (bool noMagic : {false, true}) {
            source.pe1_music = pe1;
            source.no_magic = noMagic;
            const auto encoded = source.serialize();
            copy.deserialize(encoded);
            assert(copy.pe1_music == pe1 && copy.no_magic == noMagic && copy.serialize() == encoded);
            assert(copy.flags == source.flags);
            const auto config = copy.convert();
            assert(config.no_magic == noMagic && config.pe1_music == pe1);
            assert(config.flags.randomize_magic == !noMagic);
            assert(config.flags.magic_pickup_mode == !noMagic);
            assert(config.flags.active_time_battle == !noMagic);
        }
    copy.deserialize(original);
    assert(!copy.no_magic && !copy.pe1_music && copy.convert().flags.randomize_magic);
    // Legacy 222-byte payload has no versioned trailers.
    copy.no_magic = copy.pe1_music = true;
    copy.deserialize(original.substr(0, 296));
    assert(!copy.no_magic && !copy.pe1_music);
    // Unknown, duplicate and partial tags must fail without altering settings.
    source.pe1_music = source.no_magic = true;
    const auto valid = source.serialize();
    // Malformed binary trailers must be rejected before committing settings.
    const auto bytes = decode(valid);
    for (const auto &bad :
         {bytes + "NMG1", bytes + "PEM1", bytes + "????", bytes + "N", bytes.substr(0, 225)}) {
        copy.deserialize(valid);
        bool rejected = false;
        try {
            copy.deserialize(encode(bad));
        } catch (const std::runtime_error &) {
            rejected = true;
        }
        assert(rejected && copy.serialize() == valid);
    }
    // Validation must finish before writes, including a mismatch at the last edit.
    struct Edit {
        unsigned offset, before;
    };
    constexpr Edit expected[] = {{0x7b010, 0x800ce5f0}, {0x7b014, 0x800d5234}, {0x7b018, 0x800ce738},
                                 {0x7b01c, 0x800ce894}, {0x7b024, 0x08010606}, {0xfaa0, 0x3c028011},
                                 {0xe78c, 0x3c025555},  {0xe790, 0x34425556},  {0xfaa8, 0},
                                 {0xfaac, 0x10400019},  {0xfab0, 0x00002821},  {0xfab4, 0x8c44001c},
                                 {0xfab8, 0x3c028007}};
    std::string fixture(0x7b028, '\0');
    for (const auto &e : expected)
        for (unsigned b = 0; b < 4; ++b)
            fixture[e.offset + b] = char(e.before >> (8 * b));
    for (auto bad : {std::string(40, '\0'), fixture}) {
        if (bad.size() > 40)
            bad[expected[12].offset] ^= 1;
        std::stringstream stream(bad, std::ios::in | std::ios::out | std::ios::binary);
        bool rejected = false;
        try {
            domain::magic::no_magic::apply(stream);
        } catch (const std::exception &) {
            rejected = true;
        }
        assert(rejected && stream.str() == bad);
    }
    std::stringstream stream(fixture, std::ios::in | std::ios::out | std::ios::binary);
    domain::magic::no_magic::apply(stream);
    const auto changed = stream.str();
    assert(changed.size() == fixture.size());
    assert(changed[0x7b024] == 5 && changed[0x7b025] == 5);
    for (size_t i = 0; i < fixture.size(); ++i) {
        bool allowed = false;
        for (const auto &e : expected)
            allowed |= i >= e.offset && i < e.offset + 4;
        if (!allowed)
            assert(changed[i] == fixture[i]);
    }
    // Optional explicit integration with a user-owned extracted USA package.
    if (argc == 3) {
        std::ifstream input(argv[1], std::ios::binary);
        std::stringstream package;
        package << input.rdbuf();
        assert(input && package.str().size() > 0x7b028);
        domain::magic::no_magic::apply(package);
        std::ofstream output(argv[2], std::ios::binary);
        package.seekg(0);
        output << package.rdbuf();
        assert(output);
    }
}
