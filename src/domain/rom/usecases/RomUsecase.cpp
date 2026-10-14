#include "domain/rom/usecases/RomUsecase.hpp"
#include <filesystem>
#include <functional>

namespace domain::rom {

void RomUsecase::eccedc_init() {
    uint32_t i, j, edc;
    for (i = 0; i < 256; i++) {
        j = (i << 1) ^ (i & 0x80 ? 0x11D : 0);
        ecc_f_lut[i] = (uint8_t)j;
        ecc_b_lut[i ^ j] = (uint8_t)i;
        edc = i;
        for (j = 0; j < 8; j++) {
            edc = (edc >> 1) ^ (edc & 1 ? 0xD8018001 : 0);
        }
        edc_lut[i] = edc;
    }
}

void RomUsecase::edc_computeblock(std::vector<char>::const_iterator src, size_t size,
                                  std::vector<char>::iterator dest) {
    uint32_t edc = 0;

    for (size_t i = 0; i < size; ++i, ++src) {
        edc = (edc >> 8) ^ edc_lut[(edc ^ static_cast<uint8_t>(*src)) & 0xFF];
    }

    // Assuming dest points to an array of at least 4 bytes
    *dest++ = static_cast<char>((edc >> 0) & 0xFF);
    *dest++ = static_cast<char>((edc >> 8) & 0xFF);
    *dest++ = static_cast<char>((edc >> 16) & 0xFF);
    *dest = static_cast<char>((edc >> 24) & 0xFF);
}

void RomUsecase::ecc_generate(std::vector<char>::iterator sector, int zeroaddress) {
    std::vector<char> saved_address(4);

    // Save the address and zero it out, if necessary
    if (zeroaddress) {
        std::copy(sector + 12, sector + 16, saved_address.begin());
        std::fill(sector + 12, sector + 16, 0);
    }

    // Compute ECC P code
    ecc_computeblock(sector + 0xC, 86, 24, 2, 86, sector + 0x81C);

    // Compute ECC Q code
    ecc_computeblock(sector + 0xC, 52, 43, 86, 88, sector + 0x8C8);

    // Restore the address, if necessary
    if (zeroaddress) {
        std::copy(saved_address.begin(), saved_address.end(), sector + 12);
    }
}

void RomUsecase::ecc_computeblock(std::vector<char>::iterator src, uint32_t major_count, uint32_t minor_count,
                                  uint32_t major_mult, uint32_t minor_inc, std::vector<char>::iterator dest) {
    uint32_t size = major_count * minor_count;
    uint32_t major, minor;
    for (major = 0; major < major_count; major++) {
        uint32_t index = (major >> 1) * major_mult + (major & 1);
        uint8_t ecc_a = 0;
        uint8_t ecc_b = 0;

        for (minor = 0; minor < minor_count; minor++) {
            uint8_t temp = static_cast<uint8_t>(*(src + index));
            index += minor_inc;
            if (index >= size) {
                index -= size;
            }
            ecc_a ^= temp;
            ecc_b ^= temp;
            ecc_a = ecc_f_lut[ecc_a];
        }

        ecc_a = ecc_b_lut[ecc_f_lut[ecc_a] ^ ecc_b];
        *(dest + major) = static_cast<char>(ecc_a);
        *(dest + major + major_count) = static_cast<char>(ecc_a ^ ecc_b);
    }
}

void RomUsecase::eccedc_generate(std::vector<char> &sector) {
    // Generate sync
    std::copy(sync_header.begin(), sync_header.end(), sector.begin());

    switch (sector[0x0F]) {
    case 0x00:
        // Mode 0: no data; generate zeroes
        std::fill(sector.begin() + 0x10, sector.begin() + 0x930, 0);
        break;

    case 0x01:
        // Mode 1
        // Compute EDC
        edc_computeblock(sector.begin() + 0x00, 0x810, sector.begin() + 0x810);
        // Zero out reserved area
        std::fill(sector.begin() + 0x814, sector.begin() + 0x81C, 0);
        // Generate ECC P/Q codes
        ecc_generate(sector.begin(), 0);
        break;

    case 0x02:
        // Mode 2
        // Make sure XA flags match
        std::copy(sector.begin() + 0x10, sector.begin() + 0x14, sector.begin() + 0x14);

        if (!(sector[0x12] & 0x20)) {
            // Form 1: Compute EDC
            edc_computeblock(sector.begin() + 0x10, 0x808, sector.begin() + 0x818);
            // Generate ECC P/Q codes
            ecc_generate(sector.begin(), 1);
        } else {
            // Form 2: Compute EDC
            if (optional_form2_edc_calculated == 0) {
                std::fill(sector.begin() + 0x92C, sector.begin() + 0x930, 0);
            } else {
                edc_computeblock(sector.begin() + 0x10, 0x91C, sector.begin() + 0x92C);
            }
        }
        break;
    }
}

int RomUsecase::extractStages(const std::string &bin_path, const std::vector<file::pe2stage_t> &stage_files,
                              const std::string &workdir, int &current_stage,
                              std::function<void(int, std::string)> progress_cb) {
    std::ifstream file(bin_path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open input file for extraction: " + bin_path);
    }
    for (auto stage : stage_files) {
        if (m_cancel_flag.load()) {
            break;
        }
        int ret = extractFiles(file, stage.starting_signature, stage.ending_signature,
                               workdir + stage.filename + ".CDF");
        if (ret == 0) {
            std::cout << "Something went wrong..." << std::endl;
            throw std::runtime_error("Failed extract Bin Files");
        } else if (ret == 1) {
            file.close();
            return 1;
        }
        current_stage++;
        progress_cb(100 / 7 * current_stage, "Step 1/4 : Unpacking bin files...");
    }
    file.close();
    return 0;
}

int RomUsecase::importStages(const std::string &bin_path, const std::vector<file::pe2stage_t> &stage_files,
                             const std::string &workdir, int &current_stage,
                             std::function<void(int, std::string)> progress_cb) {
    std::fstream file(bin_path, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open input file for import: " + bin_path);
    }
    for (auto stage : stage_files) {
        if (m_cancel_flag.load()) {
            file.close();
            return 1;
        }
        std::cout << "Starting importing the file: " + workdir + stage.filename + ".CDF" << std::endl;
        int ret = importFiles(file, stage.starting_signature, workdir + stage.filename + ".CDF");
        if (ret == 1) {
            file.close();
            return 1;
        }
        std::cout << "Finishing importing the file: " + workdir + stage.filename + ".CDF" << std::endl;
        file.seekg(0);
        file.seekp(0);
        current_stage++;
        progress_cb(100 / 10 * current_stage, "Step 4/4 : Packing bin files...");
    }
    file.close();
    return 0;
}

size_t RomUsecase::extractFiles(std::ifstream &bin_file, std::vector<char> start_signature,
                                std::vector<char> end_signature, std::string output_file_path) {
    bool is_file_open = false;
    std::ofstream output_file;
    size_t written_bytes = 0;

    while (true) {
        if (m_cancel_flag.load()) {
            return 1;
        }
        std::vector<char> chunk(2352); // Read sectors
        bin_file.read(chunk.data(), chunk.size());

        if (bin_file.fail() && !bin_file.eof()) {
            throw std::runtime_error(
                "Error! Couldn't work with bin file! Check if it is not opened in any other program.");
            break;
        }

        if (bin_file.eof()) // If end of file
            break;

        // If we found the start of a file, open a new output file
        if (std::search(chunk.begin(), chunk.end(), start_signature.begin(), start_signature.end()) !=
                chunk.end() &&
            !is_file_open) {
            output_file.open(output_file_path, std::ios::binary);
            is_file_open = true;
        }

        // If we have an open file, write to it
        if (is_file_open) {
            output_file.write(chunk.data() + 24, chunk.size() - 304);
            written_bytes += 2024;
        }

        // If we found the end of a file, close the output file
        if (std::search(chunk.begin(), chunk.end(), end_signature.begin(), end_signature.end()) !=
                chunk.end() &&
            is_file_open) {
            output_file.close();
            is_file_open = false;
            break;
        }
    }

    // If we reached end of file but the file is still open, close it
    if (is_file_open) {
        output_file.close();
    }

    return written_bytes;
}

size_t RomUsecase::importFiles(std::fstream &bin_file, std::vector<char> start_signature,
                               std::string input_file_path) {
    bool is_file_open = false;
    std::ifstream input_file;
    size_t written_bytes = 0;
    int i = 0;

    while (true) {
        if (m_cancel_flag.load()) {
            return 1;
        }
        std::vector<char> chunk(2352); // Read sectors
        bin_file.read(chunk.data(), chunk.size());

        if (bin_file.eof()) // If end of file
            break;

        // If we found the start of a file, open the input file
        if (std::search(chunk.begin(), chunk.end(), start_signature.begin(), start_signature.end()) !=
                chunk.end() &&
            !is_file_open) {
            input_file.open(input_file_path, std::ios::binary);
            is_file_open = true;
        }

        // If we reached the end of the input file, close it
        if (input_file.eof() && is_file_open) {
            input_file.close();
            is_file_open = false;
            break;
        }

        // If we have an open file, read from it and replace the chunk in the original file
        if (is_file_open) {
            std::vector<char> replace_chunk(2048);
            input_file.read(replace_chunk.data(), replace_chunk.size());
            if (input_file.eof()) {
                break;
            }

            std::copy(replace_chunk.begin(), replace_chunk.end(), chunk.begin() + 24);
            eccedc_generate(chunk);

            bin_file.seekp(i * (2352));
            bin_file.write(chunk.data(), chunk.size());
            written_bytes += replace_chunk.size();
            bin_file.seekg((i + 1) * 2352);
        }
        i++;
    }

    // If we reached end of original file but the input file is still open, close it
    if (is_file_open) {
        input_file.close();
    }

    return written_bytes;
}

} // namespace domain::rom
