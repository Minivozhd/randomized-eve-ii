#pragma once

#include <cstdio>
#include <vector>

namespace utils::lzss {

class LZDictSeqDesc {
  public:
    LZDictSeqDesc(unsigned char start_index, std::vector<unsigned char> seq)
        : start_index(start_index), seq(seq), same_bytes_count(0) {}
    int getStartIndex();
    std::vector<unsigned char> getSequence();
    int getSameBytesCount();
    void calcSameBytesCountFor(const std::vector<unsigned char> &other);

  private:
    int start_index;
    std::vector<unsigned char> seq;
    int same_bytes_count;
};

} // namespace utils::lzss
