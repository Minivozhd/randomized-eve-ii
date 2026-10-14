#include "utils/BitsReader.hpp"
#include <iostream>

namespace utils {
std::pair<std::vector<bool>, bool> BitsReader::readBits(int count, bool with_remove,
                                                        int jump_at_addr_of_every_sector) {
    std::vector<bool> result;
    bool jumped = false;

    while (static_cast<size_t>(count) > buffer.size() && !input_stream.eof()) {
        if (jump_at_addr_of_every_sector > 0 &&
            input_stream.tellg() % READER_BLOCK_SIZE == jump_at_addr_of_every_sector) {
            input_stream.seekg(READER_BLOCK_SIZE - jump_at_addr_of_every_sector, std::ios_base::cur);
            jumped = true;
            continue;
        }

        unsigned char byte;
        input_stream.read(reinterpret_cast<char *>(&byte), 1);
        auto bits = utils::getBitsFromBytes(byte);

        buffer.insert(buffer.end(), bits.begin(), bits.end());
    }

    if (static_cast<size_t>(count) > buffer.size()) {
        return {std::vector<bool>(), jumped};
    }

    result.insert(result.end(), buffer.begin(), buffer.begin() + count);

    if (with_remove) {
        buffer.erase(buffer.begin(), buffer.begin() + count);
    }

    return {result, jumped};
}

std::streampos BitsReader::getCurrentStreamPosition() {
    return input_stream.tellg();
}

int BitsReader::getBufferSize() {
    return buffer.size();
}
} // namespace utils
