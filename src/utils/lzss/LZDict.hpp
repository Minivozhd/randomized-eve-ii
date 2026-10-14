#pragma once

#include "utils/Common.hpp"
#include "utils/lzss/LZDictSeqDesc.hpp"
#include "utils/lzss/LZLink.hpp"
#include <array>
#include <chrono>
#include <vector>

#define INITIAL_DATA_SIZE 256

namespace utils::lzss {
class LZDict {
  public:
    LZDict() : data(INITIAL_DATA_SIZE, 0), write_pos(1), totally_filled_once(false) {}
    void addDecodedBytes(std::vector<unsigned char> &bs);
    void addDecodedByte(unsigned char b);
    std::vector<unsigned char> getRangeByLink(LZLink &link);
    std::vector<LZDictSeqDesc> buildAvailableSequence(int size);
    LZLink getLinkOnSeq(std::vector<unsigned char> &seq);

  private:
    std::vector<unsigned char> data;
    int write_pos;
    bool totally_filled_once;
};
} // namespace utils::lzss
