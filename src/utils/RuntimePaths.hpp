#pragma once
#include <filesystem>
#include <stdexcept>

namespace utils::paths {
// Initialized once, before workers start. CLI tests default to their own cwd.
inline std::filesystem::path resourceRoot = ".";
inline std::filesystem::path workRoot = ".";

inline bool hasResources(const std::filesystem::path &root) {
    return std::filesystem::is_regular_file(root / "assets/aya.png") &&
           std::filesystem::is_regular_file(root / "assets/icon.ico");
}
inline void configureWorkDirectory(const std::filesystem::path &work) {
    const auto absolute = std::filesystem::absolute(work);
    std::filesystem::create_directories(absolute);
    workRoot = absolute;
}
inline void configure(const std::filesystem::path &resources, const std::filesystem::path &work) {
    if (!hasResources(resources))
        throw std::runtime_error("Missing randomizer runtime assets");
    resourceRoot = std::filesystem::absolute(resources);
    configureWorkDirectory(work);
}
inline std::filesystem::path asset(const std::filesystem::path &relative) {
    return resourceRoot / "assets" / relative;
}
inline std::string workDirectory() {
    return (workRoot / "tmp").generic_string() + "/";
}
} // namespace utils::paths
