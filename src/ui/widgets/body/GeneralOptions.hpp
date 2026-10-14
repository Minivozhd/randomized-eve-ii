#pragma once

#include <functional>
#include <map>
#include <wx/dataview.h>
#include <wx/filename.h>
#include <wx/progdlg.h>
#include <wx/statbox.h>
#include <wx/thread.h>
#include <wx/wfstream.h>
#include <wx/wx.h>

#include "ui/RandomizerThread.hpp"
#include "ui/context/UIContext.hpp"
#include "ui/shared/Colors.hpp"
#include "ui/shared/Fonts.hpp"
#include "ui/widgets/body/dialog/AdvancedOptionsDialog.hpp"
#include "ui/widgets/body/dialog/EnemiesDialog.hpp"
#include "ui/widgets/body/dialog/ItemsDialog.hpp"
#include "ui/widgets/body/dialog/MagicDialog.hpp"
#include "ui/widgets/body/dialog/MusicDialog.hpp"
#include "ui/widgets/header/Header.hpp"

#define _STR_GENERAL_OPTIONS_STR "General options:"
#define _STR_RANDOMIZER_BUTTONS_STR ""
#define _STR_ENEMIES_CHECKBOX_LABEL "Randomize enemies"
#define _STR_ITEMS_CHECKBOX_LABEL "Randomize items"
#define _STR_MAGIC_CHECKBOX_LABEL "Randomize magic"
#define _STR_MUSIC_CHECKBOX_LABEL "Randomize music"
#define _STR_ENEMIES_BUTTON_LABEL "Configure enemies"
#define _STR_ITEMS_BUTTON_LABEL "Configure items"
#define _STR_MAGIC_BUTTON_LABEL "Configure magic"
#define _STR_MUSIC_BUTTON_LABEL "Configure music"

#define GENERAL_OPTIONS_SIZE_WIDTH 650
#define GENERAL_OPTIONS_SIZE_HEIGHT 300
#define GENERAL_OPTIONS_BUTTON_SIZE_WIDTH 150

class GeneralOptions : public wxPanel {
  public:
    GeneralOptions(wxWindow *, std::shared_ptr<UIContext>, Header *);

    enum RandomizationOptionType {
        GENERAL_OPTIONS_ENEMIES,
        GENERAL_OPTIONS_ITEMS,
        GENERAL_OPTIONS_MAGIC,
        GENERAL_OPTIONS_MUSIC,
    };

    struct RandomizationOption {
        std::string checkbox_label;
        std::string button_label;
        std::function<void(wxCommandEvent &)> checkbox_click_handeler;
        std::function<void(wxCommandEvent &)> button_click_handler;
    };

    // sub-components
    wxBoxSizer *general_options_sizer;
    wxCheckBox *checkboxes[4];
    wxButton *enemies_randomization_button;
    wxButton *items_randomization_button;
    wxButton *magic_randomization_button;
    wxButton *music_randomization_button;
    wxButton *choose_disc_1_button;
    wxButton *choose_disc_2_button;
    wxButton *randomize_button;

  private:
    std::shared_ptr<UIContext> ctx;
    Header *header;
    wxProgressDialog *m_progressDialog;
    RandomizerThread *m_randomizerThread;

    // sub-components rendering methods
    void drawRandomizationOptions();
    void drawRandomizationButtons();

    // event handler methods
    void onEnemiesCheckboxClick(wxCommandEvent &);
    void onEnemiesButtonClick(wxCommandEvent &);
    void onItemsCheckboxClick(wxCommandEvent &);
    void onItemsButtonClick(wxCommandEvent &);
    void onMagicCheckboxClick(wxCommandEvent &);
    void onMagicButtonClick(wxCommandEvent &);
    void onMusicCheckboxClick(wxCommandEvent &);
    void onMusicButtonClick(wxCommandEvent &);
    void onAdvancedOptionsClick(wxCommandEvent &);
    void onChooseDisc1Click(wxCommandEvent &);
    void onChooseDisc2Click(wxCommandEvent &);
    void onRandomizeClick(wxCommandEvent &);
    void onThreadUpdate(wxThreadEvent &);
    void onThreadWorkDone(wxThreadEvent &);

    void updateCheckboxes();

    // utility structs
    std::map<RandomizationOptionType, RandomizationOption> randomization_options;

    wxDECLARE_EVENT_TABLE();
};
