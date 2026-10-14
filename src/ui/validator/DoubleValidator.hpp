#pragma once

#include <wx/wx.h>

class DoubleValidator : public wxValidator {
  public:
    DoubleValidator();

    virtual wxObject *Clone() const wxOVERRIDE;
    virtual bool Validate(wxWindow *parent) wxOVERRIDE;
    virtual bool TransferToWindow() wxOVERRIDE;
    virtual bool TransferFromWindow() wxOVERRIDE;
};
