#include "utils/Common.hpp"

namespace utils {
std::vector<bool> getBitsFromBytes(unsigned char byte) {
    std::vector<bool> result;
    for (int i = 7; i >= 0; --i) {
        result.push_back((byte >> i) & 1);
    }
    return result;
}

std::vector<unsigned char> getBytesFromBits(const std::vector<bool> &bits) {
    int length = bits.size();
    while (length % 8 > 0) {
        length++;
    }

    std::vector<unsigned char> result;
    for (int i = 0; i < length; i += 8) {
        std::bitset<8> byte;
        for (int j = 0; j < 8 && static_cast<size_t>(i + j) < bits.size(); ++j) {
            byte.set(bits.size() - 1 - j, bits[i + j]);
        }
        result.push_back(static_cast<unsigned char>(byte.to_ulong()));
    }

    return result;
}

std::vector<uint8_t> intersection(std::vector<uint8_t> v1, std::vector<uint8_t> v2) {
    std::vector<uint8_t> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v3));
    return v3;
}

int16_t minFrom4Nums(int16_t x1, int16_t x2, int16_t x3, int16_t x4) {
    if (x1 <= x2 && x1 <= x3 && x1 <= x4) {
        return x1;
    } else if (x2 <= x3 && x2 <= x4) {
        return x2;
    } else if (x3 <= x4) {
        return x3;
    } else {
        return x4;
    }
}

int16_t maxFrom4Nums(int16_t x1, int16_t x2, int16_t x3, int16_t x4) {
    if (x1 >= x2 && x1 >= x3 && x1 >= x4) {
        return x1;
    } else if (x2 >= x3 && x2 >= x4) {
        return x2;
    } else if (x3 >= x4) {
        return x3;
    } else {
        return x4;
    }
}

void deleteDirectory(const std::string &path) {
    try {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            for (const auto &entry : std::filesystem::directory_iterator(path)) {
                if (std::filesystem::is_directory(entry.path())) {
                    deleteDirectory(entry.path().string());
                } else if (std::filesystem::is_regular_file(entry.path())) {
                    std::filesystem::remove(entry.path());
                }
            }

            std::filesystem::remove(path);
        } else {
            std::cout << "Directory not found: " << path << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error deleting directory: " << e.what() << std::endl;
    }
}
} // namespace utils
