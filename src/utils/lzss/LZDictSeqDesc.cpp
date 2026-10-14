#include "utils/lzss/LZDictSeqDesc.hpp"

namespace utils::lzss {
int LZDictSeqDesc::getStartIndex() {
    return start_index;
}

std::vector<unsigned char> LZDictSeqDesc::getSequence() {
    return seq;
}

int LZDictSeqDesc::getSameBytesCount() {
    return same_bytes_count;
}

void LZDictSeqDesc::calcSameBytesCountFor(const std::vector<unsigned char> &other) {
    same_bytes_count = 0;
    for (size_t i = 0; i < seq.size(); ++i) {
        if (seq[i] == other[i]) {
            ++same_bytes_count;
        } else {
            break;
        }
    }
}
} // namespace utils::lzss
