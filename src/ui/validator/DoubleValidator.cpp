#include "ui/validator/DoubleValidator.hpp"

DoubleValidator::DoubleValidator() {}

wxObject *DoubleValidator::Clone() const {
    return new DoubleValidator(*this);
}

bool DoubleValidator::Validate(wxWindow *parent) {
    wxTextCtrl *ctrl = dynamic_cast<wxTextCtrl *>(m_validatorWindow);
    if (!ctrl)
        return false;

    wxString value = ctrl->GetValue();
    double double_value;
    if (!value.ToDouble(&double_value)) {
        wxMessageBox("Please enter a valid number", "Invalid Input", wxOK | wxICON_ERROR, parent);
        return false;
    }
    if (double_value < 0 || double_value >= 100) {
        wxMessageBox("Value must be between 0 and 99.99", "Invalid Input", wxOK | wxICON_ERROR, parent);
        return false;
    }
    return true;
}

bool DoubleValidator::TransferToWindow() {
    return true;
}

bool DoubleValidator::TransferFromWindow() {
    return true;
}
