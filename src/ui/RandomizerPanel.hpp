#pragma once

#include <wx/wx.h>

class RandomizerPanel : public wxPanel {
  public:
    RandomizerPanel(wxWindow *parent);

  private:
    wxImage m_backgroundImage;   // Store the original image
    wxBitmap m_backgroundBitmap; // Store the scaled bitmap
    void OnPaint(wxPaintEvent &);
    void OnSize(wxSizeEvent &);
};
