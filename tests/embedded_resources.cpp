#include "fixtures/resource_fingerprints.hpp"
#include "resources/EmbeddedResources.hpp"
#include "support/Test.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>

int main() {
    namespace fs = std::filesystem;
    const auto root =
        fs::temp_directory_path() /
        ("pe2-embedded-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    try {
        const auto entries = pe2::resources::catalog();
        test::expectEqual(entries.size(), std::size(test::payloadFingerprints),
                          "Embedded catalogue coverage");
        std::set<std::string_view> names;
        for (const auto &entry : entries)
            test::require(names.insert(entry.name).second, "Duplicate embedded key");
        for (const auto &expected : test::payloadFingerprints) {
            const auto bytes = pe2::resources::get(expected.name);
            test::expectEqual(bytes.size(), expected.size, "Embedded size");
            std::uint32_t hash = 2166136261u;
            for (auto byte : bytes)
                hash = (hash ^ byte) * 16777619u;
            test::expectEqual(hash, expected.hash, "Embedded payload differs from original release");
            auto stream = pe2::resources::stream(bytes);
            const std::vector<char> actual(std::istreambuf_iterator<char>(stream), {});
            test::require(actual == pe2::resources::copy(expected.name), "Stream/vector binary fidelity");
        }
        for (auto name : {"", "missing", "../binary/data.bin", "game/players/KYLE.pe2mp"}) {
            bool rejected = false;
            try {
                pe2::resources::get(name);
            } catch (const std::runtime_error &) {
                rejected = true;
            }
            test::require(rejected, "Unknown resource accepted");
        }
        fs::create_directory(root);
        const auto target = root / "dummy";
        pe2::resources::write("binary/dummy_monster.pe2e.decompr", target);
        {
            std::ifstream file(target, std::ios::binary);
            const std::vector<char> actual(std::istreambuf_iterator<char>(file), {});
            test::require(actual == pe2::resources::copy("binary/dummy_monster.pe2e.decompr"),
                          "Staged dummy differs");
        }
        bool rejected = false;
        try {
            pe2::resources::write("binary/data.bin", root / "missing" / "file");
        } catch (const std::runtime_error &) {
            rejected = true;
        }
        test::require(rejected, "Unwritable resource destination accepted");
        fs::remove_all(root);
        std::cout << "264 embedded payloads preserve release bytes; lookup and staging guards pass\n";
    } catch (const std::exception &e) {
        std::error_code ignored;
        fs::remove_all(root, ignored);
        std::cerr << e.what() << '\n';
        return 1;
    }
}
