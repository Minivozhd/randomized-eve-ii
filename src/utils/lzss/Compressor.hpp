#pragma once

#include "utils/BitsReader.hpp"
#include "utils/BitsWriter.hpp"
#include "utils/Common.hpp"
#include "utils/Logger.hpp"
#include "utils/lzss/LZBuffer.hpp"
#include "utils/lzss/LZDict.hpp"
#include "utils/lzss/LZDictSeqDesc.hpp"
#include "utils/lzss/LZLink.hpp"
#include <atomic>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define LZSS_HEADER_OFFSET 16

namespace utils::lzss {
class Compressor {
  public:
    Compressor(std::atomic<bool> &cancelFlag, std::shared_ptr<PE2Logger> _logger)
        : m_cancelFlag(cancelFlag), logger(_logger) {};
    void decompress(const std::string &, const std::string &, int);
    void compress(const std::string &, const std::string &);

  private:
    std::atomic<bool> &m_cancelFlag;
    std::shared_ptr<PE2Logger> logger;
};
} // namespace utils::lzss
