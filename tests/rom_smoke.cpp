#include "delivery/Manager.hpp"
#include "domain/randomizer/usecases/Randomizer.hpp"
#include "ui/context/UIContext.hpp"
#include "utils/RuntimePaths.hpp"
#include <filesystem>
#include <iostream>
#include <limits>

// Explicit opt-in integration tool. The input discs are read-only; all output
// and intermediate files belong to a new, caller-selected workspace.
int main(int argc, char **argv) {
    namespace fs = std::filesystem;
    if (argc < 4 || argc > 6) {
        std::cerr << "Usage: pe2_rom_smoke DISC1.bin DISC2.bin NEW_OUTPUT_DIR [SEED] [STATE]\n";
        return 2;
    }
    try {
        const auto first = fs::absolute(argv[1]);
        const auto second = fs::absolute(argv[2]);
        const auto output = fs::absolute(argv[3]);
        if (!fs::is_regular_file(first) || !fs::is_regular_file(second))
            throw std::runtime_error("Both original disc images are required");
        if (fs::exists(output))
            throw std::runtime_error("Output directory must not already exist");
        const auto requestedSeed = argc > 4 ? std::stoull(argv[4]) : 53765u;
        if (requestedSeed > std::numeric_limits<std::uint32_t>::max())
            throw std::runtime_error("Seed exceeds 32 bits");
        fs::create_directories(output);
        utils::paths::configureWorkDirectory(output);
        for (const auto &[source, name] : {std::pair{first, "disc1"}, std::pair{second, "disc2"}}) {
            auto cue = source;
            cue.replace_extension(".cue");
            fs::copy_file(source, output / (std::string(name) + ".bin"));
            fs::copy_file(cue, output / (std::string(name) + ".cue"));
        }
        auto logger = PE2Logger::getInstance((output / "smoke.log").string());
        std::atomic<bool> cancel(false);
        auto progress = [](int, std::string) {};
        utils::lzss::Compressor compressor(cancel, logger);
        domain::file::FileUsecase files(compressor, progress, cancel, logger);
        domain::rom::RomUsecase rom((output / "disc1.bin").string(), (output / "disc2.bin").string(), cancel);
        usecase::Randomizer randomizer(logger);
        delivery::Manager manager(randomizer, files, rom, cancel, progress);
        UIContext context(static_cast<std::uint32_t>(requestedSeed), logger);
        if (argc > 5)
            context.deserialize(argv[5]);
        auto config = context.convert();
        UIContext replay(0, logger);
        replay.deserialize(config.randomization_state_string);
        if (replay.serialize() != context.serialize())
            throw std::runtime_error("State round-trip failed");
        config.input_file_1 = (output / "disc1.bin").string();
        config.input_file_2 = (output / "disc2.bin").string();
        const auto result = manager.randomize(config);
        if (result != 0)
            throw std::runtime_error(manager.lastError());
        std::cout << "Full generation succeeded. Output: " << output << '\n';
        return 0;
    } catch (const std::exception &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
