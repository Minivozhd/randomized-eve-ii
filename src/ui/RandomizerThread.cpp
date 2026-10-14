#include "ui/RandomizerThread.hpp"

void *RandomizerThread::Entry() {
    m_cancelFlag = false;
    // Initializing modules
    utils::lzss::Compressor compressor = utils::lzss::Compressor(m_cancelFlag, _logger);
    domain::file::FileUsecase fileUsecase = domain::file::FileUsecase(
        compressor,
        [this](int progress, std::string dialog_name) {
            // Post an event to the main thread to update the progress bar
            wxThreadEvent event(wxEVT_THREAD, 1);
            event.SetInt(progress);
            event.SetString(dialog_name);
            m_handler->QueueEvent(event.Clone());
        },
        m_cancelFlag, _logger);
    usecase::Randomizer randomizerImpl = usecase::Randomizer(_logger);

    // Initializing main app manager

    domain::rom::RomUsecase romUsecase =
        domain::rom::RomUsecase(config.input_file_1, config.input_file_2, m_cancelFlag);
    delivery::Manager manager = delivery::Manager(randomizerImpl, fileUsecase, romUsecase, m_cancelFlag,
                                                  [this](int progress, std::string dialog_name) {
                                                      // Post an event to the main thread to update the
                                                      // progress bar
                                                      wxThreadEvent event(wxEVT_THREAD, 1);
                                                      event.SetInt(progress);
                                                      event.SetString(dialog_name);
                                                      m_handler->QueueEvent(event.Clone());
                                                  });

    int ret = manager.randomize(config);

    // Completion and its dialog belong to the GUI thread. Preserve the actual
    // exception: a path/space/settings failure does not prove ROM corruption.
    if (ret < 0)
        _logger->log("[ERR] ", manager.lastError());
    wxThreadEvent event(wxEVT_THREAD, 2);
    event.SetInt(ret);
    event.SetString(wxString::FromUTF8(manager.lastError()));
    m_handler->QueueEvent(event.Clone());

    return nullptr;
}

void RandomizerThread::setCancelFlag(bool flag) {
    m_cancelFlag = flag;
}