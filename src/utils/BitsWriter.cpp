#include "utils/BitsWriter.hpp"

namespace utils {
BitsWriter::BitsWriter(std::ofstream &current_stream) : current_stream(current_stream) {}

void BitsWriter::writeBit(bool bit) {
    buffer.push_back(bit);
    if (buffer.size() == 8) {
        flush();
    }
}

void BitsWriter::writeBits(const std::vector<bool> &bits) {
    for (bool bit : bits) {
        writeBit(bit);
    }
}

void BitsWriter::flush(bool end_stream) {
    if (buffer.size() == 8) {
        char byte = 0;
        for (int i = 0; i < 8; ++i) {
            byte |= (buffer[i] << (7 - i));
        }
        current_stream.put(byte);
        buffer.clear();
    }

    if (end_stream && !buffer.empty()) {
        while (buffer.size() < 8) {
            buffer.push_back(0);
        }
        flush();
    }
}

std::ofstream &BitsWriter::getCurrentStream() {
    return current_stream;
}
} // namespace utils
