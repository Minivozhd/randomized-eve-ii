#include "patches/title_screen/TitleBackground.hpp"
#include <chrono>
#include <iostream>

using namespace patches::title;

static std::vector<uint8_t> read(const std::filesystem::path &path) {
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
static void write(const std::filesystem::path &path, const std::vector<uint8_t> &data) {
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
    if (!file)
        throw std::runtime_error("Cannot write test fixture");
}
int main(int argc, char **argv) {
    auto path = std::filesystem::temp_directory_path() /
                ("pe2-title-regression-" +
                 std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".CDF");
    try {
        // Optional local ROM validation. Original data is never embedded in tests.
        if (argc == 3) {
            std::filesystem::copy_file(argv[1], argv[2]);
            const auto before = read(argv[2]);
            applyBackground(argv[2]);
            auto after = read(argv[2]);
            if (before.size() != after.size())
                throw std::runtime_error("CDF size changed");
            for (size_t i = 0; i < before.size(); ++i)
                if ((i < kChunkOffset + 16 || i >= kChunkOffset + 16 + kCapacity) && before[i] != after[i])
                    throw std::runtime_error("Write outside title background");
            applyBackground(argv[2]);
            if (read(argv[2]) != after)
                throw std::runtime_error("Second application changed data");
            std::cout << "Original resource accepted; exact write bounds and idempotency verified\n";
            return 0;
        }
        std::vector<uint8_t> data(kChunkOffset + 16 + kCapacity + 32, 0xa5);
        std::copy(kChunkHeader.begin(), kChunkHeader.end(), data.begin() + kChunkOffset);
        auto expectRejected = [&] {
            write(path, data);
            bool rejected = false;
            try {
                applyBackground(path);
            } catch (const std::runtime_error &) {
                rejected = true;
            }
            if (!rejected || read(path) != data)
                throw std::runtime_error("Invalid input was accepted or changed");
        };
        expectRejected(); // right header, unknown source artwork
        std::fill(data.begin() + kChunkOffset + 16, data.begin() + kChunkOffset + 16 + kCapacity, 0);
        std::copy(std::begin(kBackground), std::end(kBackground), data.begin() + kChunkOffset + 16);
        write(path, data);
        applyBackground(path);
        if (read(path) != data)
            throw std::runtime_error("Idempotent patch changed adjacent bytes");
        data[kChunkOffset] = 1;
        expectRejected(); // wrong chunk type even with valid new image
        data.resize(kChunkOffset + 8);
        expectRejected(); // truncated header/payload
        std::filesystem::remove(path);
        std::cout << "Title resource guards and idempotency passed\n";
        return 0;
    } catch (const std::exception &e) {
        std::filesystem::remove(path);
        std::cerr << e.what() << '\n';
        return 1;
    }
}
