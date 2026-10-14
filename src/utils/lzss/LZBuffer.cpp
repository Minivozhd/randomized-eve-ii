#include "utils/lzss/LZBuffer.hpp"
#include <iostream>

namespace utils::lzss {
LZBuffer::LZBuffer(int max_capacity) : max_capacity(max_capacity) {}

bool LZBuffer::isEmpty() const {
    return data.empty();
}

int LZBuffer::getFirstByte() const {
    return data.front();
}

std::deque<int> LZBuffer::getRange(int range) const {
    return std::deque<int>(data.begin(), data.begin() + range);
}

void LZBuffer::removeDataFromBufferStart(int amount) {
    data.erase(data.begin(), data.begin() + amount);
}

void LZBuffer::fillBufferWithNextData(std::ifstream &input_file) {
    while (data.size() < static_cast<unsigned char>(max_capacity)) {
        char byte;
        if (!input_file.get(byte)) {
            break;
        }
        data.push_back(static_cast<int>(byte));
    }
}

const std::deque<int> &LZBuffer::getData() const {
    return data;
}
} // namespace utils::lzss
