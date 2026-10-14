#pragma once

/*
    DELIVERY MODULE

    The delivery module is responsible for orchestrating the entire randomization
    process. It acts as a high-level coordinator that manages the workflow of
    extracting game files from ROM binaries, unpacking them, applying randomization
    through various randomizer components, repacking the modified files, and
    importing them back into the ROM binaries. The module serves as the main entry
    point for the randomization pipeline, handling file operations, progress
    tracking, and error management throughout the process.
*/

#include "delivery/Config.hpp"
#include "domain/config/innerconfig.hpp"
#include "domain/file/entities/PE2File.hpp"
#include "domain/file/usecases/FileUsecase.hpp"
#include "domain/item/entities/Item.hpp"
#include "domain/randomizer/entities/Strategy.hpp"
#include "domain/randomizer/usecases/IRandomizer.hpp"
#include "domain/rom/usecases/RomUsecase.hpp"
#include <atomic>
#include <chrono>
#include <filesystem>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

namespace delivery {
/*
    MANAGER CLASS

    Main orchestrator class for the randomization process. Manages the complete
    workflow from extracting game files from ROM binaries to importing randomized
    files back. Coordinates between different components: randomizer, packer,
    and ROM patcher. Handles progress reporting and cancellation support.

    attributes:

    RANDOMIZER - reference to the randomizer use case that performs the actual
                 randomization logic
    PACKER - reference to the packer use case that handles file compression and
             decompression operations
    ROM_PATCHER - reference to the ROM patcher that handles extraction and
                  import of stage files from/to ROM binaries
    M_CANCEL_FLAG - atomic flag for cancellation support during long operations
    M_PROGRESS_CALLBACK - callback function for progress reporting with
                          percentage and status message
    CURRENT_STAGE - current stage index being processed
    WORKDIR - working directory path for temporary files

    public methods:

    MANAGER - constructor that initializes all dependencies
    ~MANAGER - destructor
    RANDOMIZE - main method that executes the complete randomization workflow

    private methods:

    GENERATEINNERCONFIG - converts delivery config to domain inner config
    EXTRACTFROMBINFILES - extracts stage files from ROM binary files
    IMPORTTOBINFILES - imports stage files back into ROM binary files
    GENERATEFILEPATHS - generates output file paths with timestamps
*/
class Manager {
  public:
    /*
        Constructs a Manager instance with all required dependencies.

        @param _randomizer - reference to the randomizer use case
        @param _packer - reference to the packer use case
        @param _patcher - reference to the ROM patcher
        @param cancel_flag - atomic flag for cancellation support
        @param callback - progress callback function
    */
    Manager(usecase::IRandomizer &, domain::file::FileUsecase &, domain::rom::RomUsecase &,
            std::atomic<bool> &, std::function<void(int, std::string)>);

    /*
        Destructor for Manager class.
    */
    ~Manager();

    /*
        Executes the complete randomization workflow. Extracts files from ROM
        binaries, unpacks them, applies randomization, repacks files, and
        imports them back into ROM binaries. Returns 0 on success, negative
        values on error, and 1 on cancellation.

        @param config - delivery configuration containing all randomization
                       settings and file paths
        @return 0 on success, negative error codes on failure, 1 on cancellation
    */
    int randomize(const config_t &);
    const std::string &lastError() const {
        return last_error;
    }

  private:
    usecase::IRandomizer &randomizer;
    domain::file::FileUsecase &file_usecase;
    domain::rom::RomUsecase &rom_usecase;
    std::atomic<bool> &m_cancel_flag;
    std::function<void(int, std::string)> m_progress_callback;
    int current_stage;

    std::string workdir;
    std::string last_error;

    /*
        Converts delivery configuration to domain inner configuration format.
        Maps delivery config flags and settings to the internal configuration
        structure used by domain layer components.

        @param conf - delivery configuration to convert
        @return inner configuration structure for domain layer
    */
    inner_config_t generateInnerConfig(const config_t &conf);

    /*
        Extracts stage files from ROM binary files to the working directory.

        @param filename - path to the ROM binary file
        @param stage_files - vector of stage file descriptors to extract
    */
    void extractFromBinFiles(const std::string &, const std::vector<domain::file::pe2stage_t> &);

    /*
        Imports stage files from the working directory back into ROM binary files.

        @param filename - path to the ROM binary file
        @param stage_files - vector of stage file descriptors to import
    */
    void importToBinFiles(const std::string &, const std::vector<domain::file::pe2stage_t> &);

    /*
        Generates output file paths with timestamps and creates corresponding
        CUE and INI files. Creates a backup copy of the input file with a
        timestamp suffix and updates the CUE file to reference the new binary.
        Also creates an INI file containing the randomization state string.

        @param input_file - path to the input ROM binary file
        @param serialization_string - randomization state string to save in INI
        @return pair of strings containing new binary file path and CUE file path
    */
    std::pair<std::string, std::string> generateFilePaths(const std::string &, const std::string &);
};
} // namespace delivery
