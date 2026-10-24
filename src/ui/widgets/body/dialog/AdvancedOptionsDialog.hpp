#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <wx/combobox.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/statline.h>
#include <wx/wx.h>

#include "ui/context/UIContext.hpp"
#include "ui/widgets/header/Header.hpp"

class AdvancedOptionsDialog : public wxFrame {
  public:
    AdvancedOptionsDialog(std::shared_ptr<UIContext>);

    enum ModificatorType {
        MODIFICATOR_TYPE_ON_OFF,
        MODIFICATOR_TYPE_MULTIPLE_OPTIONS,
    };

    enum ModificatorId {
        NO_AMMO_BOXES = 0x1,
        ONE_TIME_BOXES = 0x2,
        DEFAULT_BOXES = 0x3,
        RANDOMIZED_BOXES = 0x4,
        FULL_RANDOMIZED_BOXES = 0x5,
        UNRANDOMIZED_BOXES = 0x6,
        ENHANCE_SOME_ENEMIES = 0x7,
        ENHANCE_ALL_ENEMIES = 0x8,
        ENHANCE_DISC_2_ENEMIES = 0x9,
        NO_ENEMY_ENHANCEMENT = 0xa,
        ENHANCE_BOSSES = 0xb,
        MAGIC_ELEMENTS = 0xc,
        NEW_ARMORS = 0xd,
    };

    struct ModificatorOption {
        ModificatorId id;
        std::string name;
        bool is_default;
        uint32_t flag;
    };

    struct Modificator {
        std::string name;
        std::string tip;
        ModificatorType type;
        std::vector<ModificatorOption> options;
        uint64_t flag = 0;
        bool UIContext::*setting = nullptr;
    };

  private:
    std::shared_ptr<UIContext> ctx;
    UIContext ctx_copy;
    wxListBox *availableList;
    wxListBox *activeList;
    wxStaticText *availableLabel;
    wxStaticText *activeLabel;
    wxStaticText *mutatorDescription;
    wxButton *addButton;
    wxButton *removeButton;
    std::vector<const Modificator *> availableMods;
    std::vector<const Modificator *> activeMods;
    wxRadioButton *radiobuttons[6];
    wxSlider *colorSliders[3];
    wxStaticText *colorSliderLabels[3];
    wxPanel *colorPanel;

    // consts
    static const int _ADVANCED_OPTIONS_DIALOG_WIDTH = 600;
    static const int _ADVANCED_OPTIONS_DIALOG_HEIGHT = 650;
    static const std::string _ADVANCED_OPTIONS_DIALOG_TITLE;
    static const std::vector<Modificator> _ADVANCED_OPTIONS_MODIFICATORS;

    // event handler methods
    void OnClose(wxCloseEvent &);
    void OnApplyEvent(wxCommandEvent &);
    void OnCancelEvent(wxCommandEvent &);
    void OnDefaultEvent(wxCommandEvent &);
    void OnSliderChanged(wxCommandEvent &);
    void UpdateSlidersEnabled();
    void RefreshMutators(uint64_t selectedFlag = 0);
    void SelectMutator(wxListBox *list);
    void MoveMutator(bool activate);

    // sub-components rendering methods
    void DrawModificators();

    // sub-components
    wxPanel *panel;
    wxBoxSizer *dialog_sizer;
    wxBoxSizer *dialog_inner_sizer;
    wxComboBox *starting_costume_combobox;

    wxDECLARE_EVENT_TABLE();
};
