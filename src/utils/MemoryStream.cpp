#include "utils/MemoryStream.hpp"

namespace utils {
void MemoryStream::Write(const std::vector<uint8_t> &bytes) {
    data.insert(data.end(), bytes.begin(), bytes.end());
}

void MemoryStream::WriteByte(uint8_t byte) {
    data.push_back(byte);
}

std::vector<uint8_t> MemoryStream::Read(size_t count) {
    if (position + count > data.size()) {
        count = data.size() - position;
    }

    std::vector<uint8_t> result(data.begin() + position, data.begin() + position + count);
    position += count;

    return result;
}

uint8_t MemoryStream::ReadByte() {
    if (position >= data.size()) {
        throw std::out_of_range("End of stream reached");
    }

    return data[position++];
}

size_t MemoryStream::GetPosition() const {
    return position;
}

void MemoryStream::SetPosition(size_t newPosition) {
    if (newPosition > data.size()) {
        throw std::out_of_range("New position is out of range");
    }

    position = newPosition;
}

size_t MemoryStream::GetSize() const {
    return data.size();
}

std::vector<uint8_t> MemoryStream::GetData() {
    return data;
}
} // namespace utils