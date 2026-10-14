#include "utils/lzss/LZDict.hpp"

namespace utils::lzss {
void LZDict::addDecodedBytes(std::vector<unsigned char> &bs) {
    for (unsigned char b : bs) {
        addDecodedByte(b);
    }
}

void LZDict::addDecodedByte(unsigned char b) {
    data[write_pos] = b;
    ++write_pos;
    if (static_cast<size_t>(write_pos) >= data.size()) {
        write_pos = 0;
        totally_filled_once = true;
    }
}

std::vector<unsigned char> LZDict::getRangeByLink(LZLink &link) {
    std::vector<unsigned char> byte_list;
    int index = link.getAddr();

    while (byte_list.size() < static_cast<size_t>(link.getSize())) {
        byte_list.push_back(data[index]);
        ++index;
        addDecodedByte(byte_list.back());
        if (static_cast<size_t>(index) >= data.size()) {
            index = 0;
        }
    }
    return byte_list;
}

std::vector<LZDictSeqDesc> LZDict::buildAvailableSequence(int size) {
    unsigned char start_index = 1;
    std::vector<LZDictSeqDesc> lz_dict_seq_desc_list;
    lz_dict_seq_desc_list.reserve(256); // Reserve maximum possible size

    std::array<unsigned char, 256> num_array;
    std::array<unsigned char, 17> array; // Changed to std::array
    size_t s_size = static_cast<size_t>(size);
    while (start_index > 0) {
        unsigned char index1 = write_pos;
        std::copy(data.begin(), data.end(), num_array.begin());
        if (totally_filled_once || start_index < write_pos) {
            unsigned char index2 = start_index;
            size_t array_size = 0; // To track current size of 'array'
            while (array_size < s_size) {
                array[array_size] = num_array[index2]; // Directly accessing elements
                num_array[index1] = array[array_size];
                ++array_size; // Increase 'array' size
                ++index1;
                if (static_cast<size_t>(index1) >= num_array.size()) {
                    index1 = 0;
                }
                ++index2;
                if (static_cast<size_t>(index2) >= num_array.size()) {
                    index2 = 0;
                }
            }
            lz_dict_seq_desc_list.emplace_back(
                start_index,
                std::vector<unsigned char>(array.begin(),
                                           array.begin() + array_size)); // Convert to vector only when needed
            ++start_index;
            if (static_cast<size_t>(start_index) >= num_array.size()) {
                start_index = 0;
            }
        } else {
            break;
        }
    }
    return lz_dict_seq_desc_list;
}

LZLink LZDict::getLinkOnSeq(std::vector<unsigned char> &seq) {
    std::vector<LZDictSeqDesc> lz_dict_seq_desc_list = buildAvailableSequence(seq.size());
    if (lz_dict_seq_desc_list.empty()) {
        return LZLink(-1, -1); // No valid link.
    }

    for (auto &lzdsd : lz_dict_seq_desc_list) {
        lzdsd.calcSameBytesCountFor(seq);
    }

    LZDictSeqDesc lzdsd = lz_dict_seq_desc_list[0];
    for (auto &dict_seq : lz_dict_seq_desc_list) {
        if (dict_seq.getSameBytesCount() > lzdsd.getSameBytesCount()) {
            lzdsd = dict_seq;
        }
    }

    LZLink link_on_seq(-1, -1); // No valid link.
    if (lzdsd.getSameBytesCount() >= 2) {
        link_on_seq = LZLink(lzdsd.getStartIndex(), lzdsd.getSameBytesCount());
    }

    return link_on_seq;
}

} // namespace utils::lzss
