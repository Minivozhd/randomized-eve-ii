#include "utils/lzss/LZLink.hpp"

namespace utils::lzss {
int LZLink::getAddr() {
    return data[0];
}

int LZLink::getSize() {
    return data[1] + 2;
}

std::vector<bool> LZLink::getAsBits() {
    std::vector<bool> bool_list;
    for (int i = 0; i < 2; ++i) {
        std::vector<bool> bits_from_byte = utils::getBitsFromBytes(data[i]);
        bool_list.insert(bool_list.end(), bits_from_byte.begin(), bits_from_byte.end());
    }
    bool_list.erase(bool_list.begin() + 8, bool_list.begin() + 12);
    return bool_list;
}
} // namespace utils::lzss
