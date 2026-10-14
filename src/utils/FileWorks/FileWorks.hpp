#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <vector>

namespace utils::file_works {
void writeFourBytesLittleEndian(std::fstream &, uint32_t, std::streampos);
void writeTwoBytesLittleEndian(std::fstream &, size_t, std::streampos);
void writeByte(std::fstream &, uint8_t, std::streampos);
void writeVectorsToFile(std::fstream &, std::streampos, const std::vector<std::vector<char>> &);
} // namespace utils::file_works
