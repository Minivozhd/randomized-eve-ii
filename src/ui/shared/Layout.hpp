#pragma once

#include <wx/frame.h>
#include <wx/sizer.h>

namespace ui {
// Fit the client content plus the platform's window decorations. Prevent
// shrinking below native control sizes, which differ on macOS and Windows.
inline void fitContent(wxFrame *frame, wxWindow *content) {
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(content, 1, wxEXPAND);
    frame->SetSizerAndFit(sizer);
    frame->SetMinSize(frame->GetSize());
}
} // namespace ui
