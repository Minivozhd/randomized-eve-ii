#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/wx.h>

#include "ui/context/UIContext.hpp"
#include "ui/validator/DoubleValidator.hpp"
#include "ui/widgets/header/Header.hpp"

#define _ENEMIES_OPTIONS_DIALOG_TITLE_STR "Enemies configuration options"
#define _ENEMIES_POOL_TABLE_HEADER_1 "Include"
#define _ENEMIES_POOL_LABEL "Enemies Pool:"
#define _ENEMIES_POOL_HINT                                                                                   \
    "Enemies Pool Table for randomization\n\nToggle checkboxes to include/exclude certain enemy types for "  \
    "randomization.\n\nNOTE: if randomizer can't spawn any enemies due to certain room restrictions and/or " \
    "most/all enemies being not included in randomization pool, than Odd Strangers drop."
#define _ENEMIES_ADDITIONAL_OPTIONS_STR "Additional Options:"
#define _ENEMIES_HEALTH_MULT_STR "Enemy Defense Multiplier:"
#define _ENEMIES_HEALTH_MULT_TIP_STR                                                                         \
    "Set enemy defense multiplier.\n\nAll enemies have <multiplier> x <defense>. Defense affects damage to " \
    "enemies. This multiplier will affect all modes and will be applied proportionally."
#define _ENEMIES_DAMAGE_MULT_STR "Enemy Damage Multiplier:"
#define _ENEMIES_DAMAGE_MULT_TIP_STR                                                                         \
    "Set enemy damage multiplier.\n\nAll enemies have <multiplier> x <base_damage>. This multiplier will "   \
    "affect all modes and will be applied proportionally."
#define _BOSSES_HEALTH_MULT_STR "Boss Health Multiplier:"
#define _BOSSES_HEALTH_MULT_TIP_STR                                                                          \
    "Set boss health multiplier.\n\nAll bosses have <multiplier> x <base_health>. This multiplier will "     \
    "affect all modes and will be applied proportionally."
#define _BOSSES_DAMAGE_MULT_STR "Boss Damage Multiplier:"
#define _BOSSES_DAMAGE_MULT_TIP_STR                                                                          \
    "Set boss damage multiplier.\n\nAll bosses have <multiplier> x <base_damage>. This multiplier will "     \
    "affect all modes and will be applied proportionally."

#define ENEMIES_OPTIONS_DIALOG_SIZE_WIDTH 500
#define ENEMIES_OPTIONS_DIALOG_SIZE_HEIGHT 500

#define ENEMIES_OPTIONS_POOL_SIZE_WIDTH 250
#define ENEMIES_OPTIONS_POOL_SIZE_HEIGHT 400
#define ENEMIES_OPTIONS_POOL_LABEL_WIDTH 150
#define ENEMIES_OPTIONS_POOL_COLUMN_WIDTH 80

class EnemiesOptionsDialog : public wxFrame {
  public:
    EnemiesOptionsDialog(std::shared_ptr<UIContext>, Header *);

    struct EnemyPoolTableRecord {
        std::string name;
        bool include;
        uint8_t id;
        uint64_t flag;
    };

    struct EnemyAdditionalOption {
        std::string label;
        std::string tip;
        std::function<void(wxCommandEvent &)> input_edit_handler;
        wxTextCtrl *input = nullptr;
    };

  private:
    std::shared_ptr<UIContext> ctx;
    UIContext ctx_copy;
    Header *header;

    // event handler methods
    void OnClose(wxCloseEvent &);
    void OnCellLeftClick(wxGridEvent &);
    void OnEnemyHealthMult(wxCommandEvent &);
    void OnEnemyDamageMult(wxCommandEvent &);
    void OnBossHealthMult(wxCommandEvent &);
    void OnBossDamageMult(wxCommandEvent &);
    void OnApplyClick(wxCommandEvent &);
    void OnCancelClick(wxCommandEvent &);
    void OnDefaultClick(wxCommandEvent &);

    // sub-components rendering methods
    void drawEnemiesScrollView();
    void drawEnemiesOptions();

    // sub-components
    wxPanel *panel;
    wxBoxSizer *dialog_sizer;
    wxBoxSizer *enemy_pool_sizer;
    wxBoxSizer *enemy_options_sizer;
    wxGrid *enemies_pool_grid;

    // utils structs
    std::vector<EnemyAdditionalOption> enemy_additional_options;
    std::vector<EnemyPoolTableRecord> enemy_pool_table_records{
        EnemyPoolTableRecord{
            .name = "Chaser",
            .include = true,
            .id = 0x01,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__CHASER,
        },
        EnemyPoolTableRecord{
            .name = "Brain Stinger",
            .include = true,
            .id = 0x03,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__BRAIN_STINGER,
        },
        EnemyPoolTableRecord{
            .name = "Diver",
            .include = true,
            .id = 0x04,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__DIVER,
        },
        EnemyPoolTableRecord{
            .name = "Stalker",
            .include = true,
            .id = 0x06,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__STALKER,
        },
        EnemyPoolTableRecord{
            .name = "Rat",
            .include = true,
            .id = 0x07,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__RAT,
        },
        EnemyPoolTableRecord{
            .name = "Butterfly",
            .include = true,
            .id = 0x08,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__BUTTERFLY,
        },
        EnemyPoolTableRecord{
            .name = "Lesser Stranger",
            .include = true,
            .id = 0x0a,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__LESSER_STRANGER,
        },
        EnemyPoolTableRecord{
            .name = "Brute",
            .include = true,
            .id = 0x0b,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__BRUTE,
        },
        EnemyPoolTableRecord{
            .name = "Suckler",
            .include = true,
            .id = 0x0c,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__SUCKLER,
        },
        EnemyPoolTableRecord{
            .name = "Horned Stranger",
            .include = true,
            .id = 0x0d,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__HORNED_STRANGER,
        },
        EnemyPoolTableRecord{
            .name = "Mind Suckler",
            .include = true,
            .id = 0x0f,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__MIND_SUCKLER,
        },
        EnemyPoolTableRecord{
            .name = "Scavanger",
            .include = true,
            .id = 0x10,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__SCAVANGER,
        },
        EnemyPoolTableRecord{
            .name = "Odd Stranger",
            .include = true,
            .id = 0x12,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__ODD_STRANGER,
        },
        EnemyPoolTableRecord{
            .name = "Grinning Stranger",
            .include = true,
            .id = 0x13,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__GRINNING_STRANGER,
        },
        EnemyPoolTableRecord{
            .name = "Pawn Blade GOLEM",
            .include = true,
            .id = 0x14,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__PAWN_BLADE_GOLEM,
        },
        EnemyPoolTableRecord{
            .name = "Turret",
            .include = true,
            .id = 0x15,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__TURRET,
        },
        EnemyPoolTableRecord{
            .name = "Knight GOLEM",
            .include = true,
            .id = 0x16,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__KNIGHT_GOLEM,
        },
        EnemyPoolTableRecord{
            .name = "Rook blade GOLEM",
            .include = true,
            .id = 0x17,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__ROOK_BLADE_GOLEM,
        },
        EnemyPoolTableRecord{
            .name = "Amoeba",
            .include = true,
            .id = 0x18,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__AMOEBA,
        },
        EnemyPoolTableRecord{
            .name = "Scorpion",
            .include = true,
            .id = 0x19,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__SCORPION,
        },
        EnemyPoolTableRecord{
            .name = "White Worm",
            .include = true,
            .id = 0x1a,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__WHITE_WORM,
        },
        EnemyPoolTableRecord{
            .name = "Bat",
            .include = true,
            .id = 0x25,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__BAT,
        },
        EnemyPoolTableRecord{
            .name = "Beetle",
            .include = true,
            .id = 0x26,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__BEETLE,
        },
        EnemyPoolTableRecord{
            .name = "Bishop GOLEM",
            .include = true,
            .id = 0x27,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__BISHOP_GOLEM,
        },
        EnemyPoolTableRecord{
            .name = "Blood Suckler",
            .include = true,
            .id = 0x28,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__BLOOD_SUCKLER,
        },
        EnemyPoolTableRecord{
            .name = "Failed Chaser",
            .include = true,
            .id = 0x2c,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__FAILED_CHASER,
        },
        EnemyPoolTableRecord{
            .name = "Failed Suckler",
            .include = true,
            .id = 0x2e,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__FAILED_SUCKLER,
        },
        EnemyPoolTableRecord{
            .name = "Mossback brute",
            .include = true,
            .id = 0x31,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__MOSSBACK_BRUTE,
        },
        EnemyPoolTableRecord{
            .name = "Green worm",
            .include = true,
            .id = 0x37,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__GREEN_WORM,
        },
        EnemyPoolTableRecord{
            .name = "Pawn Gun GOLEM",
            .include = true,
            .id = 0x38,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__PAWN_GUN_GOLEM,
        },
        EnemyPoolTableRecord{
            .name = "Rook Gun GOLEM",
            .include = true,
            .id = 0x39,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__ROOK_GUN_GOLEM,
        },
        EnemyPoolTableRecord{
            .name = "Failed Brute",
            .include = true,
            .id = 0x46,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__FAILED_BRUTE,
        },
        EnemyPoolTableRecord{
            .name = "Failed Stranger",
            .include = true,
            .id = 0x48,
            .flag = UIContext::_UI_CONTEXT_ENEMIES_POOL__FAILED_STRANGER,
        },
    };

    wxDECLARE_EVENT_TABLE();
};
