#pragma once

#include "utils/Common.hpp"
#include <iostream>
#include <vector>

namespace utils::lzss {
class LZLink {
  public:
    LZLink(int index, int size) : data({index, size - 2}) {}
    int getAddr();
    int getSize();
    std::vector<bool> getAsBits();

  private:
    std::vector<int> data;
};
} // namespace utils::lzss
