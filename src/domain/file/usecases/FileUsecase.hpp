#pragma once

#include "domain/file/entities/PE2File.hpp"
#include "utils/Common.hpp"
#include "utils/Logger.hpp"
#include "utils/lzss/Compressor.hpp"
#include <array>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#define FILE_SEPARATOR_SIZE 512
#define FILE_SEGMENT_BLOCK_SIZE 2048
#define FILE_VALUE_SIZE (FILE_SEGMENT_BLOCK_SIZE - FILE_SEPARATOR_SIZE)
#define FILE_HEADER_SIZE 16
#define FILE_HEADER_OFFSET 16

namespace domain::file {
class FileUsecase {
  public:
    FileUsecase(utils::lzss::Compressor &, std::function<void(int, std::string)>, std::atomic<bool> &,
                std::shared_ptr<PE2Logger>);
    void initDir(std::string, bool);
    void unpack(std::string &, std::vector<pe2file_t> &);
    void pack(const std::string &, std::vector<pe2file_t> &);

  private:
    void assembleSegmentedFile(const std::string &);
    void disassembleSegmentedFile(const std::string &, bool);
    bool checkFileHeader(const std::string &);
    std::string getStageFileNameFromFile(pe2file_t &);
    std::string getFileFormat(pe2file_t &);
    void unpackFile(std::string &, pe2file_t &);
    void packFile(const std::string &, pe2file_t &);
    void worker(const std::string &, std::vector<pe2file_t> &, std::atomic<size_t> &,
                std::vector<std::shared_ptr<std::atomic<int>>> &, size_t, std::promise<void> &);

    utils::lzss::Compressor &compressor;
    const std::vector<std::string> fileFormats = {"\x04\xff", "\x02\x01", "\x05\xff", "\x06\x01"};
    std::function<void(int, std::string)> m_progress_callback;
    std::atomic<bool> &m_cancel_flag;
    std::shared_ptr<PE2Logger> logger;
};
} // namespace domain::file
