#pragma once

#include "utils/Common.hpp"
#include <fstream>
#include <vector>

#define READER_BLOCK_SIZE 2048L

namespace utils {
class BitsReader {
  public:
    BitsReader(std::ifstream &input_stream) : input_stream(input_stream) {}

    std::pair<std::vector<bool>, bool> readBits(int, bool, int);
    std::streampos getCurrentStreamPosition();
    int getBufferSize();

  private:
    std::ifstream &input_stream;
    std::vector<bool> buffer;
};
} // namespace utils
