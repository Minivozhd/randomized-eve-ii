#include "domain/randomizer/usecases/Randomizer.hpp"
#include "support/Mips.hpp"
#include "utils/RuntimePaths.hpp"
#include <chrono>

namespace usecase {
struct RandomizerTestAccess {
    static void magic(Randomizer &r) {
        r.enableMagicElements();
    }
};
} // namespace usecase

namespace {
// Original Gp_UseItemTask entry checks. Stop before rendering, casting or an
// actor dereference: these tests exercise the gate, not the complete game.
constexpr unsigned entry[] = {
    0x24434c08, 0x8062000a, 0,          0x144000a5, 0x3c028011, 0x8062000e, 0,
    0x144000a1, 0x3c028011, 0x3c028011, 0x8c425760, 0,          0x10400019, 0x00002821,
};
unsigned route(test::Mips &cpu, unsigned mode, unsigned queued, unsigned flags) {
    cpu.write(0x80114c12, mode, 1);
    cpu.write(0x80114c16, queued, 1);
    cpu.write(0x80114c0e, flags, 1);
    cpu.r[2] = 0x80110000;
    cpu.pc = 0x800a327c;
    cpu.next = cpu.pc + 4;
    cpu.load.reset();
    for (unsigned n = 0; n < 32; ++n) {
        if (cpu.pc == 0x800a34c8 || cpu.pc == 0x800a3520 || cpu.pc == 0x800a32b4)
            return cpu.pc;
        cpu.step();
    }
    throw std::runtime_error("Magic wheel gate did not terminate");
}
} // namespace

int main(int argc, char **argv) {
    const auto root =
        std::filesystem::temp_directory_path() /
        ("pe2-magic-elements-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    utils::paths::configureWorkDirectory(root);
    const auto path = std::filesystem::path(utils::paths::workDirectory()) /
                      getNameFromStage(domain::game::STAGE0) /
                      (getNameFromLevel(domain::game::BASE_LEVEL) + FORMAT_LEVEL_FILE + FORMAT_DECOMPRESSED);
    std::filesystem::create_directories(path.parent_path());
    std::vector<unsigned char> fixture(0x90000);
    if (argc == 3) {
        std::ifstream original(argv[1], std::ios::binary);
        fixture.assign(std::istreambuf_iterator<char>(original), {});
        test::require(fixture.size() > 0x803d0, "Missing extracted gameplay package");
    }
    for (unsigned i = 0; i < std::size(entry); ++i)
        for (unsigned b = 0; b < 4; ++b)
            fixture[0xfa7c + 4 * i + b] = entry[i] >> (8 * b);
    {
        std::ofstream out(path, std::ios::binary);
        out.write(reinterpret_cast<const char *>(fixture.data()), fixture.size());
    }
    // The singleton logger stays open until process exit. Keep its diagnostic
    // outside the removable fixture directory (Windows locks open files).
    auto log = PE2Logger::getInstance(root.string() + ".log");
    usecase::Randomizer randomizer(log);
    usecase::RandomizerTestAccess::magic(randomizer);
    std::ifstream input(path, std::ios::binary);
    std::vector<unsigned char> patched(std::istreambuf_iterator<char>(input), {});
    input.close();
    test::require(patched.size() == fixture.size(), "Magic patch changed package size");
    if (argc == 3) {
        std::ofstream output(argv[2], std::ios::binary);
        output.write(reinterpret_cast<const char *>(patched.data()), patched.size());
        test::require(bool(output), "Cannot write diagnostic gameplay package");
    }
    test::Mips cpu;
    cpu.put(0x80093800, patched);
    // Existing cast / queued item must still reach the native item-cast path.
    for (unsigned flags : {0u, 1u, 0x10u, 0xffu}) {
        test::expectEqual(route(cpu, 0, 0, flags), 0x800a34c8u, "Ordinary PE wheel remains reachable");
        for (unsigned queued = 1; queued <= 18; ++queued)
            test::expectEqual(route(cpu, 0, queued, flags), 0x800a3520u, "Item cast was blocked");
        for (unsigned mode : {1u, 2u, 3u})
            test::expectEqual(route(cpu, mode, 0, flags), 0x800a3520u, "Existing cast was blocked");
    }
    // Negative control: released 1.3.0 NOP retained a non-null LUI value and
    // followed an unrelated resource-table word as an actor pointer.
    cpu.write(0x800a32a0, 0x3c028011);
    test::expectEqual(route(cpu, 0, 0, 0), 0x800a32b4u, "Old unsafe gate no longer reproduced");
    std::filesystem::remove_all(root);
}
