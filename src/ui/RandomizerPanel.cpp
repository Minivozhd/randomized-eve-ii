#include "ui/RandomizerPanel.hpp"
#include "utils/RuntimePaths.hpp"

RandomizerPanel::RandomizerPanel(wxWindow *parent) : wxPanel(parent) {
    if (!m_backgroundImage.LoadFile(wxString(utils::paths::asset("aya.png").wstring()), wxBITMAP_TYPE_PNG)) {
        wxMessageBox("Could not load background image!");
    }

    Bind(wxEVT_PAINT, &RandomizerPanel::OnPaint, this);
    Bind(wxEVT_SIZE, &RandomizerPanel::OnSize, this);
}

void RandomizerPanel::OnPaint(wxPaintEvent &event) {
    wxPaintDC dc(this);
    if (m_backgroundBitmap.IsOk()) {
        int xPos = this->GetSize().GetWidth() - m_backgroundBitmap.GetWidth() - 50;
        dc.DrawBitmap(m_backgroundBitmap, xPos, 0);
    } else {
        event.Skip();
    }
}

void RandomizerPanel::OnSize(wxSizeEvent &event) {
    int newHeight = event.GetSize().GetHeight();

    if (m_backgroundImage.IsOk()) {
        int originalWidth = m_backgroundImage.GetWidth();
        int originalHeight = m_backgroundImage.GetHeight();
        int newWidth = originalWidth * newHeight / originalHeight;

        wxImage resizedImage = m_backgroundImage.Scale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);
        m_backgroundBitmap = wxBitmap(resizedImage);
    }

    event.Skip();
}
