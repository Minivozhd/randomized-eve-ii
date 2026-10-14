#include "utils/lzss/Compressor.hpp"

namespace utils::lzss {
void Compressor::decompress(const std::string &input_filename, const std::string &output_filename,
                            int file_offset) {
    // TODO: logging and error handling + getting buffers from above
    size_t buffer_size = 1024 * 1024; // 1 MiB
    char *in_buffer = new char[buffer_size];
    char *out_buffer = new char[buffer_size];

    std::ifstream in_file(input_filename, std::ios::binary);
    in_file.rdbuf()->pubsetbuf(in_buffer, buffer_size);

    std::ofstream out_file(output_filename, std::ios::binary);
    out_file.rdbuf()->pubsetbuf(out_buffer, buffer_size);

    utils::BitsReader reader(in_file);
    LZDict lzd;

    in_file.seekg(0, std::ios::end);
    std::streampos end_of_file = in_file.tellg(); // Get the end of file position
    in_file.seekg(0, std::ios::beg);
    in_file.seekg(file_offset, std::ios::beg);
    while (in_file.tellg() < end_of_file) {
        if (m_cancelFlag.load()) {
            return;
        }

        auto [bits, jumped1] = reader.readBits(1, true, 0);
        auto [bits1, jumped2] = reader.readBits(8, true, 0);

        if (bits[0]) {
            if (!bits1.empty()) {
                std::vector<unsigned char> bs = utils::getBytesFromBits(bits1);
                lzd.addDecodedBytes(bs);
                out_file.write(reinterpret_cast<const char *>(bs.data()), bs.size());
            } else {
                break;
            }
        } else {
            if (!bits1.empty()) {
                auto byte1 = utils::getBytesFromBits(bits1)[0];
                if (byte1 == 0) {
                    std::cout << "Zero link found in "
                              << static_cast<long>(reader.getCurrentStreamPosition()) -
                                     reader.getBufferSize() / 8 - 16
                              << "! End decompression!" << std::endl;
                    logger->log("Zero link found in ",
                                static_cast<long>(reader.getCurrentStreamPosition()) -
                                    reader.getBufferSize() / 8 - 16,
                                "! End decompression!");
                    break;
                }
                auto [bits2, jumped] = reader.readBits(4, true, 0);
                LZLink link(byte1, utils::getBytesFromBits(bits2)[0] + 2);
                auto range_by_link = lzd.getRangeByLink(link);
                out_file.write(reinterpret_cast<const char *>(range_by_link.data()), range_by_link.size());
            } else {
                break;
            }
        }
    }

    in_file.close();
    out_file.close();
    delete[] in_buffer;
    delete[] out_buffer;
}

void Compressor::compress(const std::string &input_filename, const std::string &output_filename) {
    // TODO: logging and error handling + getting buffers from above
    std::ifstream in_file(input_filename, std::ios::binary);
    if (!in_file.is_open()) {
        std::cerr << "Failed to open the file " << input_filename << std::endl;
        logger->log("Failed to open the file ", input_filename);
    }

    std::ofstream out_file(output_filename, std::ios::binary);
    LZDict lzd;
    LZBuffer lzb(17);
    utils::BitsWriter writer(out_file);

    logger->log(input_filename, ": compressing...");

    in_file.seekg(0, std::ios::end);
    std::streampos end_of_file = in_file.tellg();
    in_file.seekg(0, std::ios::beg);

    bool is_end_of_file = false;
    lzb.fillBufferWithNextData(in_file);
    while (!lzb.isEmpty() && !is_end_of_file) {
        if (in_file.tellg() >= end_of_file) {
            is_end_of_file = true;
        }
        if (m_cancelFlag.load()) {
            return;
        }

        std::deque<int> temp_deque = lzb.getData();
        std::vector<unsigned char> temp_vector(temp_deque.begin(), temp_deque.end());
        LZLink link_on_seq = lzd.getLinkOnSeq(temp_vector);

        if (link_on_seq.getAddr() == -1) {
            writer.writeBit(true);
            int first_byte = lzb.getFirstByte();
            std::vector<bool> bits_from_byte = utils::getBitsFromBytes(first_byte);
            writer.writeBits(bits_from_byte);
            lzb.removeDataFromBufferStart(1);
            lzd.addDecodedByte(first_byte);
        } else {
            writer.writeBit(false);
            std::vector<bool> as_bits = link_on_seq.getAsBits();
            writer.writeBits(as_bits);
            auto r = lzb.getRange(link_on_seq.getSize());
            std::deque<int> temp_deque = r;
            std::vector<unsigned char> temp_vector(temp_deque.begin(), temp_deque.end());
            lzd.addDecodedBytes(temp_vector);
            lzb.removeDataFromBufferStart(link_on_seq.getSize());
        }
        if (!is_end_of_file) {
            lzb.fillBufferWithNextData(in_file);
        }
    }
    while (!lzb.isEmpty()) {
        std::deque<int> temp_deque = lzb.getData();
        std::vector<unsigned char> temp_vector(temp_deque.begin(), temp_deque.end());
        LZLink link_on_seq = lzd.getLinkOnSeq(temp_vector);

        if (link_on_seq.getAddr() == -1) {
            writer.writeBit(true);
            int first_byte = lzb.getFirstByte();
            std::vector<bool> bits_from_byte = utils::getBitsFromBytes(first_byte);
            writer.writeBits(bits_from_byte);
            lzb.removeDataFromBufferStart(1);
            lzd.addDecodedByte(first_byte);
        } else {
            writer.writeBit(false);
            std::vector<bool> as_bits = link_on_seq.getAsBits();
            writer.writeBits(as_bits);
            auto r = lzb.getRange(link_on_seq.getSize());
            std::deque<int> temp_deque = r;
            std::vector<unsigned char> temp_vector(temp_deque.begin(), temp_deque.end());
            lzd.addDecodedBytes(temp_vector);
            lzb.removeDataFromBufferStart(link_on_seq.getSize());
        }
    }
    writer.flush();

    // Add zero link
    LZLink lzl(0, 2);
    writer.writeBit(false);
    std::vector<bool> as_bits = lzl.getAsBits();
    writer.writeBits(as_bits);
    writer.flush(true);

    in_file.close();
    out_file.close();
}
} // namespace utils::lzss
