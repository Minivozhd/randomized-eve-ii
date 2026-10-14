#pragma once

#include "utils/Logger.hpp"
#include <algorithm>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

namespace utils {
std::vector<bool> getBitsFromBytes(unsigned char);
std::vector<unsigned char> getBytesFromBits(const std::vector<bool> &);
std::vector<uint8_t> intersection(std::vector<uint8_t>, std::vector<uint8_t>);
// TODO: replace to file operations?
int16_t minFrom4Nums(int16_t, int16_t, int16_t, int16_t);
int16_t maxFrom4Nums(int16_t, int16_t, int16_t, int16_t);
void deleteDirectory(const std::string &);
} // namespace utils
