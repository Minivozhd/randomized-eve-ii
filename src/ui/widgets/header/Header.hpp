#pragma once

#include <map>
#include <qrencode.h>
#include <random>
#include <wx/event.h>
#include <wx/rawbmp.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/statbox.h>
#include <wx/window.h>
#include <wx/wx.h>

#include "ui/context/UIContext.hpp"
#include "ui/shared/Colors.hpp"
#include "ui/shared/Fonts.hpp"

#include "BuildInfo.hpp"
#define _VERSION_STR "v" PE2_VERSION " / " PE2_RELEASE_DATE
#define _RANDOMIZATION_STATE_OPTIONS_STR "Randomization State Options:"
#define _RANDOMIZATION_STATE_STR "Rando State:"
#define _RANDOMIZATION_SEED_STR "Rando Seed:"
#define _RANDOMIZE_SEED_BUTTON_STR "Randomize Seed"
#define _RANDOMIZATION_STATE_TIP_STR                                                                         \
    "You may manually enter the randomization state string here. Or just customize your state with UI "      \
    "options.\n\nRandomization state string - is string with all options and seed serialized in one utf-8 "  \
    "array of letters. Later this string will be appended to the randomized rom name after timestamp. From " \
    "this string you can restore your seed and all customized options.\n\nNOTE: serialization string might " \
    "differ from version to version, which makes them incompatible."
#define _RANDOMIZATION_SEED_TIP_STR                                                                          \
    "You may manually enter the randomization seed here. Or just hit the \"Randomize Seed\" button to get "  \
    "random seed with applied options.\n\nRandomization seed is a number, that initializes pseudorandom "    \
    "generator. A pseudorandom number generator's number sequence is completely determined by the seed, "    \
    "making it consistent on one operating system under same randomization options.\n\nNOTE: randomization " \
    "options might differ from version to version, which makes seeds from different versions incompatible."

#define HEADER_VERSION_LABEL_PADDING 10
#define QRCODE_PADDING 25
#define HEADER_INPUT_PADDING 75

#define QRCODE_BITMAP_SIZE 150
#define HEADER_LABEL_SIZE 128
#define HEADER_INPUT_SIZE 450
#define HEADER_INPUT_HEIGHT 100

#define HEADER_SIZE_WIDTH 1100
#define HEADER_SIZE_HEIGHT 300

#define MAX_SEED_VALUE 4294967295UL

class Header : public wxPanel {
  public:
    Header(wxWindow *, std::shared_ptr<UIContext>);

    struct HeaderInput {
        std::string label;
        std::string tip;
    };

    enum HeaderInputType { HEADER_INPUT_STATE, HEADER_INPUT_SEED };
    void redrawContext();

  private:
    int qr_size = 128;
    std::shared_ptr<UIContext> ctx;

    // sub-components rendering methods
    void drawVersionString();
    void drawQRCodeBitmap(std::string);
    void drawSingleTextInput(wxTextCtrl **, wxStaticBoxSizer *, Header::HeaderInput, wxString, bool);
    void drawTextInputs(std::string, wxString);

    // event handler methods
    void OnContextTextChanged(wxCommandEvent &event);
    void OnSeedTextChanged(wxCommandEvent &event);
    void OnRandomizeSeedClick(wxCommandEvent &event);

    // utility functions
    wxBitmap GenerateQRCode(const wxString &);

    // sub-components
    wxBoxSizer *header_main_sizer_wrapper;
    wxBoxSizer *header_main_sizer;
    wxBoxSizer *random_seed_vertical_sizer;
    wxTextCtrl *input_state_string;
    wxTextCtrl *input_seed;
    wxStaticBitmap *qr_code_bitmap;

    // utility structs
    std::map<HeaderInputType, HeaderInput> header_inputs{
        {
            HEADER_INPUT_STATE,
            HeaderInput{
                .label = _RANDOMIZATION_STATE_STR,
                .tip = _RANDOMIZATION_STATE_TIP_STR,
            },
        },
        {HEADER_INPUT_SEED, HeaderInput{
                                .label = _RANDOMIZATION_SEED_STR,
                                .tip = _RANDOMIZATION_SEED_TIP_STR,
                            }}};

    // wxDECLARE_EVENT_TABLE();
};
