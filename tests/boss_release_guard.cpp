#include "delivery/Manager.hpp"
#include "ui/context/UIContext.hpp"
#include "utils/RuntimePaths.hpp"
#include <chrono>
#include <fstream>
#include <stdexcept>

#if PE2_ENABLE_EXPERIMENTAL_BOSSES
#error This regression must exercise the release build without experimental boss code.
#endif

namespace {
void require(bool value) {
    if (!value)
        throw std::runtime_error("Experimental boss release guard failed");
}
class UnusedRandomizer final : public usecase::IRandomizer {
  public:
    void randomize(domain::inner_config_t) override {
        throw std::runtime_error("Release gate reached the randomizer");
    }
};
} // namespace

int main() {
    const auto dir =
        std::filesystem::temp_directory_path() /
        ("pe2-boss-release-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    std::filesystem::create_directories(dir / "tmp");
    struct Cleanup {
        std::filesystem::path directory;
        ~Cleanup() {
            std::error_code error;
            std::filesystem::remove_all(directory, error);
        }
    };
    // Register before the logger singleton so its Windows file handle closes
    // before deleting the fixture directory at process exit.
    static const Cleanup cleanup{dir};
    try {
        utils::paths::workRoot = dir;
        const auto marker = dir / "tmp/preserve.txt";
        std::ofstream(marker) << "previous work";
        auto log = PE2Logger::getInstance((dir / "test.log").string());
        std::atomic<bool> cancel(false);
        int progressCalls = 0;
        auto progress = [&](int, std::string) { ++progressCalls; };
        utils::lzss::Compressor compressor(cancel, log);
        domain::file::FileUsecase files(compressor, progress, cancel, log);
        domain::rom::RomUsecase rom("missing-disc1.bin", "missing-disc2.bin", cancel);
        UnusedRandomizer randomizer;
        delivery::Manager manager(randomizer, files, rom, cancel, progress);
        UIContext clean(42, log);
        const auto originalFlags = clean.flags;
        require(!clean.convert().boss_prototype && !clean.convert().stranger_row);
        const uint32_t prototype = UIContext::_UI_CONTEXT_FLAG__BOSS_PROTOTYPE;
        const uint32_t row = UIContext::_UI_CONTEXT_FLAG__STRANGER_ROW;
        for (const auto bits : {prototype, row, prototype | row}) {
            clean.flags = originalFlags | bits;
            UIContext replay(0, log);
            replay.deserialize(clean.serialize());
            require(replay.serialize() == clean.serialize());
            auto config = replay.convert();
            require(manager.randomize(config) == -4);
            require(manager.lastError().find("unavailable in this release build") != std::string::npos);
            require(progressCalls == 0 && std::filesystem::exists(marker));
        }
        // Also reject direct API callers without any serialized state/INI.
        auto direct = UIContext(42, log).convert();
        direct.stranger_row = true;
        direct.randomization_state_string.clear();
        require(manager.randomize(direct) == -4 && progressCalls == 0);
        require(std::filesystem::exists(marker));
#if !PE2_ENABLE_DEBUG_WEAPONS
        auto weapons = UIContext(42, log).convert();
        weapons.debug_weapons = true;
        require(manager.randomize(weapons) == -4 && progressCalls == 0);
        require(manager.lastError().find("Debug weapons") != std::string::npos);
        require(std::filesystem::exists(marker));
#endif
        // The original Enhanced bosses setting is independent of this prototype.
        clean.flags = originalFlags | UIContext::_UI_CONTEXT_FLAG__ENHANCED_BOSSES;
        const auto ordinary = clean.convert();
        require(ordinary.flags.enhanced_bosses && !ordinary.boss_prototype && !ordinary.stranger_row);
    } catch (...) {
        throw;
    }
}
