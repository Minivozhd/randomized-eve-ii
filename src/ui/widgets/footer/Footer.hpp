#pragma once

#include "ui/shared/Colors.hpp"
#include <string>
#include <wx/wx.h>

class Footer : public wxPanel {
  public:
    Footer(wxWindow *);

  private:
    const static int _FOOTER_WIDTH = 1100;
    const static int _FOOTER_HEIGHT = 300;
    const static std::string _FOOTER_CREDITS_STR;
    const static std::string _FOOTER_YEAR_STR;

    void onDiscordButtonClick(wxCommandEvent &);
    void onKofiButtonClock(wxCommandEvent &);
};
