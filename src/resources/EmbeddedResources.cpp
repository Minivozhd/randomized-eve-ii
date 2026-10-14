#include "resources/EmbeddedResources.hpp"
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>

namespace pe2::resources {
std::span<const Entry> catalog() {
    static const std::vector<Entry> entries = [] {
        std::vector<Entry> result;
        for (const auto family : {detail::legacy(), detail::armor(), detail::players(), detail::portraits(),
                                  detail::weapon_variants()}) {
            result.insert(result.end(), family.begin(), family.end());
        }
        std::sort(result.begin(), result.end(),
                  [](const Entry &a, const Entry &b) { return a.name < b.name; });
        return result;
    }();
    return entries;
}
Bytes get(std::string_view name) {
    const auto entries = catalog();
    const auto found =
        std::lower_bound(entries.begin(), entries.end(), name,
                         [](const Entry &entry, std::string_view key) { return entry.name < key; });
    if (found == entries.end() || found->name != name)
        throw std::runtime_error("Unknown embedded resource: " + std::string(name));
    return found->bytes;
}
std::vector<char> copy(std::string_view name) {
    const auto bytes = get(name);
    const auto *first = reinterpret_cast<const char *>(bytes.data());
    return {first, first + bytes.size()};
}
std::istringstream stream(Bytes bytes) {
    return std::istringstream(std::string(reinterpret_cast<const char *>(bytes.data()), bytes.size()),
                              std::ios::in | std::ios::binary);
}
void write(std::string_view name, const std::filesystem::path &target) {
    const auto bytes = get(name);
    std::ofstream output(target, std::ios::binary | std::ios::trunc);
    output.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());
    output.flush();
    if (!output)
        throw std::runtime_error("Cannot stage embedded resource: " + target.string());
}
} // namespace pe2::resources
