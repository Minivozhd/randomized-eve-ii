#pragma once

#include "ui/widgets/body/GeneralOptions.hpp"
#include "ui/widgets/footer/Footer.hpp"
#include "ui/widgets/header/Header.hpp"
#include <cstdint>
#include <memory>
#include <qrencode.h>
#include <random>
#include <sstream>
#include <vector>
#include <wx/checklst.h>
#include <wx/choice.h>
#include <wx/dataview.h>
#include <wx/filename.h>
#include <wx/progdlg.h>
#include <wx/rawbmp.h>
#include <wx/thread.h>
#include <wx/wfstream.h>
#include <wx/wx.h>

#include "domain/magic/entities/Magic.hpp"
#include "ui/RandomizerFrameEntities.hpp"
#include "ui/RandomizerPanel.hpp"
#include "ui/RandomizerThread.hpp"
#include "ui/context/UIContext.hpp"

class RandomizerFrame : public wxFrame {
  public:
    RandomizerFrame();

  private:
    // widgets
    RandomizerPanel *panel;
    Header *header;
    GeneralOptions *general_options;
    Footer *footer;

    // RandomizerThread *m_randomizerThread;
    // wxDECLARE_EVENT_TABLE();

    // utils
    std::shared_ptr<PE2Logger> _logger;
};
