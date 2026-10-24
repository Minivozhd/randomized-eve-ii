#include "domain/music/usecases/Pe1Music.hpp"
#include "domain/music/usecases/Pe1MusicData.hpp"
#include "utils/RuntimePaths.hpp"
#include <array>
#include <fstream>
#include <stdexcept>

namespace domain::music::pe1 {
namespace {
using Bytes = std::vector<uint8_t>;
uint32_t hash(const Bytes &data) {
    uint32_t h = 2166136261u;
    for (auto b : data)
        h = (h ^ b) * 16777619u;
    return h;
}
uint32_t word(std::istream &in) {
    std::array<unsigned char, 4> b{};
    if (!in.read(reinterpret_cast<char *>(b.data()), b.size()))
        throw std::runtime_error("Truncated PE1 music pack.");
    return uint32_t(b[0]) | uint32_t(b[1]) << 8 | uint32_t(b[2]) << 16 | uint32_t(b[3]) << 24;
}
std::vector<Bytes> readPackage(const std::filesystem::path &path) {
    std::ifstream in(path, std::ios::binary);
    if (!in)
        throw std::runtime_error(
            "Missing PE1 music pack. Re-extract the complete release archive; expected: " + path.string());
    char magic[8];
    if (!in.read(magic, sizeof(magic)) || std::string(magic, 8) != "PE1MUS01" || word(in) != tracks().size())
        throw std::runtime_error("Unsupported PE1 music pack version or track count.");
    std::vector<Bytes> records;
    for (const auto &t : tracks()) {
        const std::array<uint32_t, 8> expected{uint32_t(t.mood), t.source, t.phase,  t.id,
                                               t.offset,         t.length, t.before, t.after};
        for (auto n : expected)
            if (word(in) != n)
                throw std::runtime_error("PE1 music pack manifest mismatch.");
        Bytes delta(t.length);
        if (!in.read(reinterpret_cast<char *>(delta.data()), delta.size()) || hash(delta) != t.deltaHash)
            throw std::runtime_error("Damaged PE1 music pack: " + std::string(t.name));
        records.push_back(std::move(delta));
    }
    if (in.peek() != std::char_traits<char>::eof())
        throw std::runtime_error("Trailing data in PE1 music pack.");
    return records;
}
} // namespace

const std::vector<Track> &tracks() {
    return catalogue;
}
std::vector<uint8_t> pool(pe2_music_type_t mood) {
    std::vector<uint8_t> result;
    for (const auto &track : tracks())
        if (mood == MUSIC_MISC || track.mood == mood)
            result.push_back(static_cast<uint8_t>(track.id));
    return result;
}
std::filesystem::path packagePath() {
    return utils::paths::asset("game/music/pe1-music.pack");
}
void validatePackage(const std::filesystem::path &package) {
    (void)readPackage(package);
}

// USA D_800689F0 at RAM 0x800689F0, executable file offset 0x591F0.
// Boost the song scale, not signed 7-bit instrument gains. This preserves
// channel balance, reverb sends and the engine's master volume/fade controls.
void applyVolumeBoost(const std::filesystem::path &workdir) {
    constexpr std::streamoff tableOffset = 0x591f0;
    constexpr std::array<std::array<uint8_t, 2>, 37> levels{
        {{0x1d, 96}, {0x3d, 80},  {0x1f, 96},  {0x2f, 85},  {0x3a, 94},  {0x32, 90}, {0x0c, 100}, {0x45, 116},
         {0x2c, 69}, {0x56, 87},  {0x05, 70},  {0x29, 58},  {0x47, 127}, {0x1e, 96}, {0x41, 90},  {0x09, 127},
         {0x30, 80}, {0x4a, 127}, {0x37, 85},  {0x2b, 127}, {0x26, 122}, {0x11, 80}, {0x61, 96},  {0x3b, 114},
         {0x25, 51}, {0x2a, 94},  {0x23, 100}, {0x3f, 90},  {0x4c, 96},  {0x21, 98}, {0x48, 127}, {0x24, 80},
         {0x22, 99}, {0x39, 85},  {0x2e, 98},  {0x06, 121}, {0x20, 97}}};
    const auto path = workdir / "SLUS.CDF";
    std::fstream out(path, std::ios::binary | std::ios::in | std::ios::out);
    if (!out)
        throw std::runtime_error("Cannot open PE1 music volume target: " + path.string());
    char magic[8];
    if (!out.read(magic, sizeof(magic)) || std::string(magic, 8) != std::string("PS-X EXE"))
        throw std::runtime_error("Unsupported PE2 executable for PE1 music volume.");
    // Validate every selected entry before writing any: other patches in the
    // executable and volumes of all non-PE1 songs remain byte-identical.
    for (const auto &level : levels) {
        out.seekg(tableOffset + level[0]);
        const auto before = out.get();
        if (!out || before != level[1])
            throw std::runtime_error("Unsupported or modified PE2 music volume table.");
    }
    for (const auto &level : levels) {
        const auto louder = (unsigned(level[1]) * 13 + 5) / 10;
        out.seekp(tableOffset + level[0]);
        out.put(static_cast<char>(louder));
        if (!out)
            throw std::runtime_error("Cannot write PE1 music volume table.");
    }
    out.flush();
    if (!out)
        throw std::runtime_error("Cannot flush PE1 music volume table.");
}

void apply(const std::filesystem::path &workdir, const std::filesystem::path &package) {
    auto records = readPackage(package);
    const auto path = workdir / "STAGE0.CDF";
    std::fstream out(path, std::ios::binary | std::ios::in | std::ios::out | std::ios::ate);
    if (!out)
        throw std::runtime_error("Cannot open PE1 music target: " + path.string());
    const auto size = out.tellg();
    // Check every preimage and decoded payload before changing any bank. Keep
    // the original archive extents and HED addresses, shared by both USA discs.
    for (size_t i = 0; i < records.size(); ++i) {
        const auto &t = tracks()[i];
        if (t.length > t.slotBytes || uint64_t(t.offset) + t.slotBytes > uint64_t(size))
            throw std::runtime_error("PE1 music bank exceeds its CDF extent.");
        Bytes before(t.length);
        out.seekg(t.offset);
        if (!out.read(reinterpret_cast<char *>(before.data()), before.size()) || hash(before) != t.before)
            throw std::runtime_error("Unsupported or modified PE2 USA music bank: " + std::string(t.name));
        auto &after = records[i];
        for (size_t j = 0; j < after.size(); ++j)
            after[j] ^= before[j];
        if (hash(after) != t.after || std::string(after.begin(), after.begin() + 4) != "hMPK" ||
            after[4] != t.id)
            throw std::runtime_error("Invalid decoded PE1 music bank.");
    }
    for (size_t i = 0; i < records.size(); ++i) {
        out.seekp(tracks()[i].offset);
        out.write(reinterpret_cast<const char *>(records[i].data()), records[i].size());
        if (!out)
            throw std::runtime_error("Cannot write PE1 music bank.");
    }
    out.flush();
    if (!out)
        throw std::runtime_error("Cannot flush PE1 music banks.");
}
} // namespace domain::music::pe1
