#pragma once

#include <deque>
#include <fstream>

namespace utils::lzss {
class LZBuffer {
  public:
    LZBuffer(int max_capacity);

    bool isEmpty() const;
    int getFirstByte() const;
    std::deque<int> getRange(int range) const;
    void removeDataFromBufferStart(int amount);
    void fillBufferWithNextData(std::ifstream &input_file);
    const std::deque<int> &getData() const;

  private:
    int max_capacity;
    std::deque<int> data;
};
} // namespace utils::lzss
