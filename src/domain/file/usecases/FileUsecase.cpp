#include "domain/file/usecases/FileUsecase.hpp"

namespace domain::file {
FileUsecase::FileUsecase(utils::lzss::Compressor &_compressor,
                         std::function<void(int, std::string)> _progress_callback,
                         std::atomic<bool> &_cancel_flag, std::shared_ptr<PE2Logger> _logger)
    : compressor(_compressor), m_progress_callback(_progress_callback), m_cancel_flag(_cancel_flag),
      logger(_logger) {}

void FileUsecase::initDir(std::string dir, bool is_root) {
    std::filesystem::path directory_path(dir);
    if (std::filesystem::exists(directory_path) && is_root) {
        utils::deleteDirectory(directory_path.string());
    }
    // Existing directories are normal during unpacking. Real failures
    // must reach Manager rather than being logged and ignored.
    std::filesystem::create_directories(directory_path);
    if (!std::filesystem::is_directory(directory_path))
        throw std::runtime_error("Not a working directory: " + directory_path.string());
}

void FileUsecase::assembleSegmentedFile(const std::string &filename) {
    std::ifstream input_file(filename, std::ios::binary | std::ios::ate);
    std::ofstream output_file(filename + ".grouped", std::ios::binary);

    if (!input_file) {
        std::cerr << "[FileUsecase] Error opening input file: " << filename << std::endl;
        logger->log("[FileUsecase] Error opening input file: ", filename);
        return;
    }

    if (!output_file) {
        std::cerr << "Error opening output file: " << filename + ".grouped" << std::endl;
        logger->log("Error opening output file: ", filename + ".grouped");
        return;
    }

    std::streamsize file_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    size_t blocks_num = file_size / FILE_SEGMENT_BLOCK_SIZE;

    std::array<char, FILE_SEGMENT_BLOCK_SIZE> buffer{};

    for (size_t i = 0; i < blocks_num; ++i) {
        input_file.read(buffer.data(), FILE_SEGMENT_BLOCK_SIZE);
        output_file.write(buffer.data(), FILE_VALUE_SIZE);
    }

    size_t remaining_bytes = file_size % FILE_SEGMENT_BLOCK_SIZE;
    if (remaining_bytes > 0) {
        input_file.read(buffer.data(), remaining_bytes);
        output_file.write(buffer.data(), remaining_bytes);
    }
}

void FileUsecase::disassembleSegmentedFile(const std::string &filename, bool skip_first_line = true) {
    std::ifstream input_file(filename, std::ios::binary | std::ios::ate);
    std::ofstream output_file(filename + ".ungrouped", std::ios::binary);

    if (!input_file) {
        std::cerr << "[FileUsecase] Error opening input file: " << filename << std::endl;
        logger->log("[FileUsecase] Error opening input file: ", filename);
        return;
    }

    if (!output_file) {
        std::cerr << "Error opening output file: " << filename + ".ungrouped" << std::endl;
        logger->log("Error opening output file: ", filename + ".ungrouped");
        return;
    }

    input_file.seekg(0, std::ios::beg);
    logger->log("[Disassembler] seekg: ", filename);

    std::array<char, FILE_VALUE_SIZE> buffer{};
    std::array<char, FILE_SEPARATOR_SIZE> separator = {0};

    int block = 1;
    while (input_file) {
        input_file.read(buffer.data(),
                        skip_first_line ? FILE_VALUE_SIZE - FILE_HEADER_OFFSET : FILE_VALUE_SIZE);
        logger->log("[Disassembler] read: ", filename, " block = ", block++);
        skip_first_line = false;

        if (input_file.gcount() == 0) {
            break;
        }
        logger->log("[Disassembler] gcount: ", filename, " block = ", block++,
                    "gcount = ", input_file.gcount());

        output_file.write(buffer.data(), input_file.gcount());
        logger->log("[Disassembler] write: ", filename, " block = ", block++);

        if (input_file.peek() != EOF) {
            output_file.write(separator.data(), FILE_SEPARATOR_SIZE);
        }
        logger->log("[Disassembler] peek: ", filename, " block = ", block++);
    }
}

bool FileUsecase::checkFileHeader(const std::string &bs) {
    bool cond1 = std::find(fileFormats.begin(), fileFormats.end(), bs.substr(0, 2)) != fileFormats.end();
    bool cond3 = bs.substr(5, 3) == "\x00\x00\x00";
    bool cond4 = bs.substr(12) == "\x00\x00\x00\x00";
    return cond1 && cond3 && cond4;
}

std::string FileUsecase::getStageFileNameFromFile(pe2file_t &pe2file) {
    std::string result;

    switch (pe2file.group) {
    case LEVEL_G:
        result = "STAGE0";
        break;
    case ACROPOLIS_ROOM_G:
        result = "STAGE1";
        break;
    case DRYFIELD_DAY_ROOM_G:
        result = "STAGE2";
        break;
    case DRYFIELD_NIGHT_ROOM_G:
        result = "STAGE3";
        break;
    case MINES_AND_UPPER_SHELTER_ROOM_G:
        result = "STAGE4";
        break;
    case NEO_ARK_AND_LOWER_SHELTER_ROOM_G:
        result = "STAGE5";
        break;
    case ENEMY_G:
        result = "STAGE0";
        break;
    default:
        std::cout << "Couldn't resolve pe2file.group" << std::endl;
        logger->log("Couldn't resolve pe2file.group");
        std::cout << pe2file.filename << std::endl;
        logger->log(pe2file.filename);
        std::cerr << "Couldn't resolve pe2file.group" << std::endl;
        logger->log("Couldn't resolve pe2file.group");
        break;
    }

    return result;
}

std::string FileUsecase::getFileFormat(pe2file_t &pe2file) {
    std::string result;

    switch (pe2file.type) {
    case LEVEL:
        result = ".pe2lvl";
        break;
    case ROOM_LOGIC:
        result = ".pe2rml";
        break;
    case ENEMY_LOGIC:
        result = ".pe2enm";
        break;
    default:
        std::cerr << "Couldn't resolve pe2file.type" << std::endl;
        logger->log("Couldn't resolve pe2file.type");
        break;
    }

    return result;
}

void FileUsecase::unpackFile(std::string &workdir, pe2file_t &pe2file) {
    std::string stage = getStageFileNameFromFile(pe2file);
    std::ifstream input_file(workdir + stage + ".CDF", std::ios::binary);
    std::vector<char> bs(pe2file.file_end_offset - pe2file.file_offset, '\0');
    input_file.seekg(pe2file.file_offset);
    if (!input_file.read(bs.data(), bs.size()))
        throw std::runtime_error("Cannot read stage resource: " + workdir + stage + ".CDF / " +
                                 pe2file.filename);
    input_file.close();

    initDir(workdir + stage + "/", false);
    if (stage == "STAGE0") {
        initDir(workdir + stage + "/enemies", false);
    }
    std::cout << "stage dir was created for file " + pe2file.filename << std::endl;
    logger->log("stage dir was created for file " + pe2file.filename);
    std::string file_format = getFileFormat(pe2file);
    std::string new_filename = workdir + stage + "/" + pe2file.filename + file_format;
    std::ofstream output_file(new_filename, std::ios::binary);
    output_file.write(bs.data(), bs.size());
    output_file.close();
    if (!output_file)
        throw std::runtime_error("Cannot write unpacked resource: " + new_filename);

    std::string compressed_filename = new_filename;
    if (pe2file.is_segmented) {
        assembleSegmentedFile(new_filename);
        compressed_filename += ".grouped";
        std::filesystem::remove(new_filename.c_str());
        int result = std::rename(compressed_filename.c_str(), new_filename.c_str());
        if (result != 0) {
            perror("Error renaming file");
        }
        compressed_filename = new_filename;
    }

    if (pe2file.is_compressed) {
        compressor.decompress(compressed_filename, compressed_filename + ".decompr", FILE_HEADER_OFFSET);
    }
}

void FileUsecase::packFile(const std::string &workdir, pe2file_t &pe2file) {
    std::string stage = getStageFileNameFromFile(pe2file);
    std::string fileFormat = getFileFormat(pe2file);
    std::string decompressed_filename = workdir + stage + "/" + pe2file.filename + fileFormat + ".decompr";

    std::cout << decompressed_filename << std::endl;
    compressor.compress(decompressed_filename, decompressed_filename + ".compr");

    std::string compressed_file_name = decompressed_filename + ".compr";

    if (pe2file.is_segmented) {
        disassembleSegmentedFile(compressed_file_name);
        std::filesystem::remove(compressed_file_name.c_str());
        int result = std::rename((compressed_file_name + ".ungrouped").c_str(), compressed_file_name.c_str());
        if (result != 0) {
            perror("Error renaming file");
        }
    }

    std::ifstream compressed_file(compressed_file_name, std::ios::binary | std::ios::ate);
    if (!compressed_file) {
        throw std::runtime_error("Failed to open file");
    }
    std::streamsize size = compressed_file.tellg();
    std::vector<uint8_t> data(size);
    compressed_file.seekg(0, std::ios::beg);
    if (!compressed_file.read(reinterpret_cast<char *>(data.data()), data.size())) {
        throw std::runtime_error("Failed to read file");
    }

    logger->log("packFile: zeroing = ",
                pe2file.file_end_offset - FILE_HEADER_OFFSET - size - pe2file.file_offset);
    if (pe2file.file_end_offset < pe2file.file_offset + FILE_HEADER_OFFSET || size < 0 ||
        static_cast<uint64_t>(size) >
            static_cast<uint64_t>(pe2file.file_end_offset - pe2file.file_offset - FILE_HEADER_OFFSET)) {
        throw std::runtime_error("File " + pe2file.filename + " appears to be too large after compressing");
    }
    std::vector<uint8_t> zeroes(pe2file.file_end_offset - FILE_HEADER_OFFSET - size - pe2file.file_offset);
    std::cout << "!!! COMPRESSED FILE SIZE " << size << std::endl;
    logger->log("!!! COMPRESSED FILE SIZE ", size);
    std::cout << "!!! HEADER SIZE " << FILE_HEADER_OFFSET << std::endl;
    logger->log("!!! HEADER SIZE ", FILE_HEADER_OFFSET);
    std::cout << "!!! ZEROES SIZE "
              << pe2file.file_end_offset - FILE_HEADER_OFFSET - size - pe2file.file_offset << std::endl;
    logger->log("!!! ZEROES SIZE ",
                pe2file.file_end_offset - FILE_HEADER_OFFSET - size - pe2file.file_offset);
    std::fill(zeroes.begin(), zeroes.end(), 0);

    data.insert(data.end(), zeroes.begin(), zeroes.end());

    std::string filename = workdir + stage + ".CDF";
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }
    file.seekp(pe2file.file_offset + FILE_HEADER_OFFSET, std::ios::beg);
    if (!file.write(reinterpret_cast<const char *>(data.data()), data.size())) {
        throw std::runtime_error("Failed to write block to file");
    }
}

void FileUsecase::unpack(std::string &workdir, std::vector<pe2file_t> &files) {
    initDir(workdir, false);
    std::cout << "tmp dir was created" << std::endl;
    logger->log("tmp dir was created");
    int i = 0;
    for (pe2file_t file : files) {
        if (m_cancel_flag.load()) {
            break;
        }
        unpackFile(workdir, file);
        std::cout << file.filename << std::endl;
        logger->log(file.filename);
        int progress = (i * 100) / files.size();
        i++;
        m_progress_callback(progress, "Step 2/4 : Unpacking stage CDF files...");
    }
}

void FileUsecase::worker(const std::string &workdir, std::vector<pe2file_t> &files,
                         std::atomic<size_t> &next_index,
                         std::vector<std::shared_ptr<std::atomic<int>>> &progress_vector,
                         size_t progress_index, std::promise<void> &promise) {
    size_t index;
    while ((index = next_index++) < files.size()) {
        try {
            logger->log("[INF] [FileUsecase] [WorkerThread] before cancel flag");
            if (m_cancel_flag.load()) {
                promise.set_value();
                return;
            }
            logger->log("[INF] [FileUsecase] [WorkerThread] after cancel flag ");
            logger->log("[INF] [FileUsecase] [WorkerThread] before packFile " + files[index].filename);
            packFile(workdir, files[index]);

            logger->log("[INF] [FileUsecase] [WorkerThread] after packFile " + files[index].filename);
            int progress = (index * 100) / (files.size());
            logger->log("[INF] [FileUsecase] [WorkerThread] after progress computation " +
                        files[index].filename);
            *progress_vector[progress_index] = progress;
            logger->log("[INF] [FileUsecase] [WorkerThread] after progress vector assigment " +
                        files[index].filename);
        } catch (const std::exception &e) {
            logger->log("[ERR] [FileUsecase] [WorkerThread] something went wrong in FileUsecase worker");
            logger->log("[ERR] [FileUsecase] [WorkerThread] the problem is with file: ",
                        files[index].filename);
            promise.set_exception(std::current_exception());
            return;
        }
    }
    promise.set_value();
}

void FileUsecase::pack(const std::string &workdir, std::vector<pe2file_t> &files) {
    size_t num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) {
        num_threads = 1;
    }

    std::cout << "THREADS TO USE: " << num_threads << std::endl;
    logger->log("THREADS TO USE: ", num_threads);

    std::vector<std::thread> thread_pool;
    std::vector<std::promise<void>> promises(num_threads);
    std::vector<std::future<void>> futures;

    std::atomic<size_t> next_index(0);

    std::vector<std::shared_ptr<std::atomic<int>>> progress_vector(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        progress_vector[i] = std::make_shared<std::atomic<int>>(0);
    }

    logger->log("[INF] [FileUsecase] Preparing worker threads...");
    for (size_t i = 0; i < num_threads; ++i) {
        progress_vector[i] = std::make_shared<std::atomic<int>>(0);
        futures.emplace_back(promises[i].get_future());
        thread_pool.emplace_back([this, &workdir, &files, &next_index, &progress_vector, i, &promises] {
            worker(workdir, files, next_index, progress_vector, i, promises[i]);
        });
    }

    try {
        while (std::any_of(futures.begin(), futures.end(), [](auto &future) {
            return future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready;
        })) {
            if (m_cancel_flag.load()) {
                break;
            }
            int totalProgress = 0;
            for (const auto &progress : progress_vector) {
                totalProgress += *progress;
            }
            totalProgress /= num_threads;
            m_progress_callback(totalProgress, "Step 3/4 : Packing stage CDF files...");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const std::exception &e) {
        logger->log("[ERR] [FileUsecase] while iterating through files for checking their progress.");
    }
    logger->log("[INF] [FileUsecase] Finishing progress bar.");

    for (std::thread &t : thread_pool) {
        if (t.joinable()) {
            t.join();
        }
    }

    logger->log("[INF] [FileUsecase] Finishing joining threads.");
    for (auto &future : futures) {
        try {
            future.get();
        } catch (const std::exception &e) {
            std::cerr << "Exception from thread: " << e.what() << std::endl;
            logger->log("[ERR] [FileUsecase] Error found in one of the threads.");
            logger->log("[ERR] [FileUsecase] ", e.what());
            logger->log("[ERR] [FileUsecase] Error code : () ", std::strerror(errno), " )");
            throw;
        }
    }
    logger->log("[INF] [FileUsecase] Finishing for checking the threads.");

    // Pack specific logic files to stage0
    for (auto file : stage0_special_files_info) {
        std::string filename = workdir + "STAGE0.CDF";
        std::fstream stage_file(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (!stage_file) {
            std::cout << "Failed to open stage file: STAGE0.CDF";
            logger->log("[ERR] [FileUsecase] Failed to open stage file: STAGE0.CDF");
            throw std::runtime_error("Failed to open file");
        }

        compressor.compress(workdir + "STAGE0/" + file.filename + ".pe2e.decompr",
                            workdir + "STAGE0/" + file.filename + ".pe2e.decompr.compr");

        std::ifstream compressed_file(workdir + "STAGE0/" + file.filename + ".pe2e.decompr.compr",
                                      std::ios::binary | std::ios::ate);
        if (!compressed_file) {
            std::cout << "Failed to open file: "
                      << workdir + "STAGE0/" + file.filename + ".pe2e.decompr.compr";
            logger->log("[ERR] [FileUsecase] Failed to open file : STAGE0/", file.filename, ".");
            throw std::runtime_error("Failed to open file");
        }
        std::streamsize size = compressed_file.tellg();
        std::vector<uint8_t> data(size);
        compressed_file.seekg(0, std::ios::beg);
        if (!compressed_file.read(reinterpret_cast<char *>(data.data()), data.size())) {
            throw std::runtime_error("Failed to read file");
        }

        if (file.file_end_offset < FILE_HEADER_OFFSET + size + file.file_offset) {
            logger->log("[ERR] [FileUsecase] Couldn't pack PE2 file : ", file.filename,
                        " to STAGE file because of its size.");
            throw std::runtime_error("Couldn't pack PE2 file : " + file.filename +
                                     " to STAGE file because of its size.");
        }
        std::vector<uint8_t> zeroes(file.file_end_offset - FILE_HEADER_OFFSET - size - file.file_offset);
        std::fill(zeroes.begin(), zeroes.end(), 0);

        data.insert(data.end(), zeroes.begin(), zeroes.end());

        stage_file.seekp(file.file_offset + FILE_HEADER_OFFSET, std::ios::beg);
        if (!stage_file.write(reinterpret_cast<const char *>(data.data()), data.size())) {
            logger->log("[ERR] [FileUsecase] Failed to write the PE2 file : ", file.filename,
                        " to STAGE file.");
            throw std::runtime_error("Failed to write block to file");
        }
    }
}
} // namespace domain::file
