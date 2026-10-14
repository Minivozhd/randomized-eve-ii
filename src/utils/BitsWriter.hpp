#pragma once

#include "utils/Common.hpp"
#include <fstream>
#include <vector>

namespace utils {
class BitsWriter {
  public:
    explicit BitsWriter(std::ofstream &current_stream);

    void writeBit(bool);
    void writeBits(const std::vector<bool> &);
    void flush(bool end_stream = false);
    std::ofstream &getCurrentStream();

  private:
    std::vector<bool> buffer;
    std::ofstream &current_stream;
};
} // namespace utils
