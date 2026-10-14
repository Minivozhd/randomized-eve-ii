#include "utils/FileWorks/FileWorks.hpp"

void utils::file_works::writeFourBytesLittleEndian(std::fstream &outfile, uint32_t init_value,
                                                   std::streampos pos) {
    if (pos != 0) {
        outfile.seekp(pos);
    }
    uint32_t value = (uint32_t)init_value;

    unsigned char bytes[4];
    bytes[0] = (value & 0x000000FF);
    bytes[1] = (value & 0x0000FF00) >> 8;
    bytes[2] = (value & 0x00FF0000) >> 16;
    bytes[3] = (value & 0xFF000000) >> 24;

    outfile.write(reinterpret_cast<char *>(bytes), 4);
}

void utils::file_works::writeTwoBytesLittleEndian(std::fstream &outfile, size_t init_value,
                                                  std::streampos pos) {
    if (pos != 0) {
        outfile.seekp(pos);
    }
    uint16_t value = (uint16_t)init_value;

    unsigned char bytes[2];
    bytes[0] = (value & 0x00FF);
    bytes[1] = (value & 0xFF00) >> 8;

    outfile.write(reinterpret_cast<char *>(bytes), sizeof(bytes));
}

void utils::file_works::writeByte(std::fstream &outfile, uint8_t value, std::streampos pos) {
    outfile.seekp(pos);
    outfile.write(reinterpret_cast<const char *>(&value), sizeof(uint8_t));
}

void utils::file_works::writeVectorsToFile(std::fstream &outfile, std::streampos pos,
                                           const std::vector<std::vector<char>> &data) {
    outfile.seekp(pos);

    for (const auto &vec : data) {
        outfile.write(vec.data(), vec.size());
    }
}