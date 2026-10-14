#pragma once

#include "delivery/Manager.hpp"
#include "domain/encounter/entities/RoomInfo.hpp"
#include "domain/file/usecases/FileUsecase.hpp"
#include "domain/randomizer/usecases/Randomizer.hpp"
#include "utils/lzss/Compressor.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <wx/progdlg.h>
#include <wx/thread.h>
#include <wx/wx.h>

class RandomizerThread : public wxThread {
  public:
    RandomizerThread(wxEvtHandler *handler, delivery::config_t config, std::shared_ptr<PE2Logger> logger)
        : wxThread(wxTHREAD_JOINABLE), m_handler(handler), config(config), _logger(logger) {}
    virtual void *Entry();
    void setCancelFlag(bool flag);

  private:
    wxEvtHandler *m_handler;
    std::atomic<bool> m_cancelFlag;
    delivery::config_t config;

    std::shared_ptr<PE2Logger> _logger;
};
