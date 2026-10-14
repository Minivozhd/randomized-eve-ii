#include "domain/costume/usecases/CostumeRandomizer.hpp"
#include "domain/costume/usecases/TrainingMode.hpp"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>
using namespace domain::costume::training;
namespace usecase::costumes {
struct CostumeRandomizerTestAccess {
    static void slot(CostumeRandomizer &r, unsigned slot) {
        domain::costume::pe2_costume_t costume{};
        costume.slot = static_cast<domain::costume::pe2_costume_slot_t>(slot);
        r.chooseCostumeSlot(costume, {});
    }
};
} // namespace usecase::costumes
int main(int argc, char **argv) {
    const auto path =
        std::filesystem::temp_directory_path() /
        ("pe2-training-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    try {
        std::vector<char> before(0x20000, 0x5a);
        if (argc == 3) {
            std::ifstream in(argv[1], std::ios::binary);
            before = {std::istreambuf_iterator<char>(in), {}};
        }
        {
            std::ofstream out(path, std::ios::binary);
            out.write(before.data(), before.size());
        }
        std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);
        if (argc != 3)
            for (auto p : predicates) {
                const auto o = p.address - 0x80093800;
                write(file, o, 0x90020026u | (uint32_t(p.baseRegister) << 21));
                write(file, o + 4, 0);
                write(file, o + 8, 0x38420004);
                write(file, o + 12, p.address == 0x800a74ec ? 0x03e00008u : 0x2c420001u);
            }
        file.flush();
        {
            std::ifstream in(path, std::ios::binary);
            before = {std::istreambuf_iterator<char>(in), {}};
        }
        const auto retail = before;
        // Reject an incompatible later site before writing any earlier site.
        auto last = predicates.back();
        write(file, last.address - 0x80093800 + 8, 0);
        bool rejected = false;
        try {
            apply(file);
        } catch (const std::runtime_error &) {
            rejected = true;
        }
        if (!rejected || read(file, predicates.front().address - 0x80093800 + 8) != 0x38420004)
            throw std::runtime_error("Partial or missing validation");
        write(file, last.address - 0x80093800 + 8, 0x38420004);
        apply(file);
        file.flush();
        std::ifstream in(path, std::ios::binary);
        std::vector<char> actual{std::istreambuf_iterator<char>(in), {}};
        for (auto p : predicates) {
            auto o = p.address - 0x80093800;
            for (unsigned n = 0; n < 4; ++n) {
                before[o + n] = char((0x9002eba5u | (uint32_t(p.baseRegister) << 21)) >> (n * 8));
                before[o + 8 + n] = char(0x38420002u >> (n * 8));
            }
        }
        if (before != actual)
            throw std::runtime_error("Training patch changed unrelated bytes");
        // Execute the complete production costume writer: later legacy writes
        // must not change the mode comparison back into a model-slot check.
        using namespace domain::game;
        using namespace domain::file;
        const auto dir = std::filesystem::path(path.string() + "-integration");
        const auto game =
            dir / "STAGE0" /
            (getNameFromLevel(BASE_LEVEL) + std::string(FORMAT_LEVEL_FILE) + FORMAT_DECOMPRESSED);
        const auto gallery = dir / "STAGE1" /
                             (getNameFromRoom(ROOM_AKROPOLIS_SHOOTING_GALLERY) +
                              std::string(FORMAT_ROOM_FILE) + FORMAT_DECOMPRESSED);
        const auto garage = dir / "STAGE1" /
                            (getNameFromRoom(ROOM_AKROPOLIS_MIST_PARKING) + std::string(FORMAT_ROOM_FILE) +
                             FORMAT_DECOMPRESSED);
        for (auto p :
             {dir / "SLUS.CDF", game, gallery, garage, dir / "STAGE0/enemies/00_pierce.pe2enm.decompr"}) {
            std::filesystem::create_directories(p.parent_path());
            std::ofstream out(p, std::ios::binary);
            std::vector<char> zeros(0x40000);
            out.write(zeros.data(), zeros.size());
        }
        auto log = PE2Logger::getInstance((dir / "test.log").string());
        for (unsigned slot = 1; slot <= 4; ++slot) {
            {
                std::ofstream out(game, std::ios::binary | std::ios::trunc);
                out.write(retail.data(), retail.size());
            }
            usecase::costumes::CostumeRandomizer writer(log, std::mt19937(1), dir.string() + "/", {});
            usecase::costumes::CostumeRandomizerTestAccess::slot(writer, slot);
            std::fstream check(game, std::ios::in | std::ios::binary);
            for (auto p : predicates)
                if (read(check, p.address - 0x80093800 + 8) != 0x38420002 ||
                    read(check, p.address - 0x80093800) != (0x9002eba5u | (uint32_t(p.baseRegister) << 21)))
                    throw std::runtime_error("Costume writer overwrote training mode");
            std::ifstream room(gallery, std::ios::binary);
            room.seekg(0x29d4);
            if (room.get() != int(slot))
                throw std::runtime_error("Training lost selected model");
        }
        // Logger owns its stream until exit; leave its tiny directory for the
        // normal temp cleanup on Windows, which cannot unlink an open file.
        for (auto p :
             {dir / "SLUS.CDF", game, gallery, garage, dir / "STAGE0/enemies/00_pierce.pe2enm.decompr"})
            std::filesystem::remove(p);
        if (argc == 3) {
            std::ofstream out(argv[2], std::ios::binary);
            out.write(actual.data(), actual.size());
        }
        file.close();
        in.close();
        std::filesystem::remove(path);
        std::cout << "21 training predicates: bounds and reject-before-write passed\n";
        return 0;
    } catch (const std::exception &e) {
        std::filesystem::remove(path);
        std::cerr << e.what() << '\n';
        return 1;
    }
}
