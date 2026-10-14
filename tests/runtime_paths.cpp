#include "domain/file/usecases/FileUsecase.hpp"
#include "utils/RuntimePaths.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
int main() {
    namespace fs = std::filesystem;
    const auto root =
        fs::temp_directory_path() /
        ("pe2-path-test-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    const auto originalCwd = fs::current_path();
    struct Cleanup {
        fs::path directory;
        ~Cleanup() {
            std::error_code error;
            fs::remove_all(directory, error);
            if (error)
                std::cerr << "Test cleanup: " << error.message() << '\n';
        }
    };
    // Register before the logger singleton: its file closes first at exit.
    // Local input streams also leave scope before cleanup. Windows forbids
    // deleting these open files, unlike the Unix unlink behavior.
    static const Cleanup cleanup{root};
    try {
        auto resources = root / "Read Only App/Contents/Resources";
        fs::create_directories(resources / "assets");
        for (const char *name : {"aya.png", "icon.ico"})
            std::ofstream(resources / "assets" / name) << "fixture";
        utils::paths::configure(resources, root / "Writable Work");
        if (fs::current_path() != originalCwd ||
            utils::paths::asset("aya.png") != resources / "assets/aya.png")
            throw std::runtime_error("Resource lookup depends on cwd");
        fs::create_directory(utils::paths::workDirectory());
        if (fs::exists(resources / "tmp") || !fs::exists(root / "Writable Work/tmp"))
            throw std::runtime_error("Work files mixed with resources");
        // Exercise the real unpacker with cwd, resources and work in three
        // different locations. A path-helper-only test missed this regression.
        fs::create_directories(root / "Launch Directory");
        fs::current_path(root / "Launch Directory");
        std::string work = utils::paths::workDirectory();
        const std::string payload = "synthetic CDF payload";
        std::ofstream(fs::path(work) / "STAGE1.CDF", std::ios::binary) << payload;
        auto log = PE2Logger::getInstance((root / "fixture.log").string());
        std::atomic<bool> cancel(false);
        utils::lzss::Compressor compressor(cancel, log);
        domain::file::FileUsecase files(compressor, [](int, std::string) {}, cancel, log);
        std::vector<domain::file::pe2file_t> entries{{.filename = "synthetic",
                                                      .is_segmented = false,
                                                      .is_compressed = false,
                                                      .file_offset = 0,
                                                      .file_end_offset = static_cast<int>(payload.size()),
                                                      .type = domain::file::ROOM_LOGIC,
                                                      .group = domain::file::ACROPOLIS_ROOM_G}};
        files.unpack(work, entries);
        std::ifstream unpacked(fs::path(work) / "STAGE1/synthetic.pe2rml", std::ios::binary);
        const std::string actual((std::istreambuf_iterator<char>(unpacked)), {});
        if (actual != payload || fs::exists(root / "Launch Directory/tmp"))
            throw std::runtime_error("Unpacker ignores configured work directory");
        fs::current_path(originalCwd);
        bool rejected = false;
        try {
            utils::paths::configure(root / "missing", root / "bad");
        } catch (const std::runtime_error &) {
            rejected = true;
        }
        if (!rejected || fs::exists(root / "bad"))
            throw std::runtime_error("Missing resources accepted");
        std::cout << "Resource paths independent of cwd; writes isolated; no default CDF needed\n";
        return 0;
    } catch (const std::exception &e) {
        fs::current_path(originalCwd);
        std::cerr << e.what() << '\n';
        return 1;
    }
}
