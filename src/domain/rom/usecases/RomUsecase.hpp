#pragma once

#include "domain/file/entities/PE2File.hpp"
#include <algorithm>
#include <atomic>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

namespace domain::rom {

class RomUsecase {
  private:
    std::string filename1;
    std::string filename2;
    std::atomic<bool> &m_cancel_flag;
    // ECC/EDC lookup tables
    uint8_t ecc_f_lut[256];
    uint8_t ecc_b_lut[256];
    uint32_t edc_lut[256];
    int optional_form2_edc_calculated = 1;
    std::vector<uint8_t> sync_header = {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};
    // Initialize ECC/EDC tables
    void eccedc_init();

    // Compute EDC for a block
    void edc_computeblock(std::vector<char>::const_iterator src, size_t size,
                          std::vector<char>::iterator dest);

    void ecc_computeblock(std::vector<char>::iterator src, uint32_t major_count, uint32_t minor_count,
                          uint32_t major_mult, uint32_t minor_inc, std::vector<char>::iterator dest);

    // Generate ECC for a block
    void ecc_generate(std::vector<char>::iterator sector, int zeroaddress);

    // Verify EDC for a block
    int edc_verify(std::vector<char>::const_iterator sector);

    void eccedc_generate(std::vector<char> &sector);

  public:
    RomUsecase(std::string _filename1, std::string _filename2, std::atomic<bool> &m_cancel_flag)
        : filename1(_filename1), filename2(_filename2), m_cancel_flag(m_cancel_flag) {
        eccedc_init();
    };
    size_t extractFiles(std::ifstream &, std::vector<char>, std::vector<char>, std::string);
    size_t importFiles(std::fstream &, std::vector<char>, std::string);
    int extractStages(const std::string &bin_path, const std::vector<file::pe2stage_t> &stage_files,
                      const std::string &workdir, int &current_stage,
                      std::function<void(int, std::string)> progress_cb);
    int importStages(const std::string &bin_path, const std::vector<file::pe2stage_t> &stage_files,
                     const std::string &workdir, int &current_stage,
                     std::function<void(int, std::string)> progress_cb);
};

} // namespace domain::rom
