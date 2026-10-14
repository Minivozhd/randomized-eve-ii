#pragma once
#include <algorithm>
#include <stdexcept>
#include <string>

namespace domain::armor {
// Gp_GetItemText indexes six fields, including three unidentified fields.
// A NUL is a field separator, not a bounds check: omitting the last fields
// makes an unidentified item borrow the next armor's name.
inline std::string armorLabelRecord(const std::string &name, std::string description, bool unidentified) {
    if (name.find_first_of("\n\r") != std::string::npos || name.find('\0') != std::string::npos ||
        description.find('\0') != std::string::npos)
        throw std::runtime_error("Invalid armor text field");
    while (!description.empty() && description.back() == '\n')
        description.pop_back();
    const auto lines = std::count(description.begin(), description.end(), '\n') + 1;
    if (lines > 2)
        throw std::runtime_error("Armor description exceeds two fields");
    std::string record = name + '\n' + description + '\n';
    if (lines == 1)
        record += '\n';
    // Only the retail Tactical Vest slot has an unidentified description.
    // Keep other slots' initial identification behavior unchanged.
    record += unidentified ? "Armor\nUnidentified armor.\n\n" : "\n\n\n";
    record += '\0';
    while (record.size() % 4)
        record += '\0';
    return record;
}
} // namespace domain::armor
