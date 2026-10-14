#pragma once
#include <cstdint>
#include <filesystem>
#include <span>
#include <sstream>
#include <string_view>
#include <vector>

namespace pe2::resources {
using Bytes = std::span<const std::uint8_t>;
struct Entry {
    std::string_view name;
    Bytes bytes;
};
// Stable views into compiled-in, read-only storage. Unknown keys fail closed.
std::span<const Entry> catalog();
Bytes get(std::string_view name);
std::vector<char> copy(std::string_view name);
std::istringstream stream(Bytes bytes);
void write(std::string_view name, const std::filesystem::path &target);
namespace detail {
std::span<const Entry> legacy();
std::span<const Entry> armor();
std::span<const Entry> players();
std::span<const Entry> portraits();
std::span<const Entry> weapon_variants();
} // namespace detail
} // namespace pe2::resources
