#pragma once

#include <algorithm>
#include <array>
#include <map>
#include <string>
#include <type_traits>
#include <vector>
#include <wx/combobox.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/wx.h>

#include "ui/context/UIContext.hpp"
#include "ui/validator/DoubleValidator.hpp"
#include "ui/widgets/header/Header.hpp"

#define _ITEMS_OPTIONS_DIALOG_TITLE_STR "Items configuration options"
#define _ITEMS_ADDITIONAL_OPTIONS_STR "Items additional options"
#define _ITEMS_POOL_LABEL "Items Pool:"
#define _ITEMS_PROBABILITY_LABEL_STR "Item probability distribution:"
#define _ITEMS_STARTING_OPTIONS_STR "Starting items options:"
#define _ITEMS_SHOP_COST_MULT_STR "Shop cost multiplier: "
#define _ITEMS_SHOP_COST_MULT_TIP_STR                                                                        \
    "Set shop cost multiplier.\n\nAll shop items will cost <multiplier> x <base_shop_cost> BP."
#define _ITEMS_POOL_HINT                                                                                     \
    "Items Pool Table for randomization\n\nToggle checkboxes to include/exclude certain items for "          \
    "randomization.\n - \"Drop\" column defines if this item will be included or excluded for dropping as "  \
    "a pickup or as a reward for beaten encounters.\n - \"Shop\" column defines if the selected items will " \
    "be included or excluded for dropping in shops.\n - \"Cost\" column defines the actual cost of an "      \
    "item.\n\nNOTE: if randomizer can't spawn any item due all items being not included in randomization "   \
    "pool, than Recovery 1 will always drop for pickups and rewards, while shops will be completely "        \
    "bugged, so be careful."
#define _ITEMS_POOL_TABLE_HEADER_1 "Drop"
#define _ITEMS_POOL_TABLE_HEADER_2 "Shop"
#define _ITEMS_POOL_TABLE_HEADER_3 "Cost"
#define _ITEMS_PROB_HP_RECOVERY_STR "HP Recovery:"
#define _ITEMS_PROB_MP_RECOVERY_STR "MP Recovery:"
#define _ITEMS_PROB_AMMO_STR "Ammo:"
#define _ITEMS_PROB_MISC_STR "Misc:"
#define _ITEMS_PROB_UNIQUE_STR "Unique:"
#define _ITEMS_PROB_HP_RECOVERY_TIP_STR                                                                      \
    "Enter HP Recovery probability here.\n\nThis value (0.0 - 1.0) determines the probability of dropping "  \
    "HP Recovery items (Recovery 1, Recovery 2, Recovery 3, Protein Capsule, Ringer's Solution) as pickups " \
    "and encounter rewards."
#define _ITEMS_PROB_MP_RECOVERY_TIP_STR                                                                      \
    "Enter MP Recovery probability here.\n\nThis value (0.0 - 1.0) determines the probability of dropping "  \
    "MP Recovery items (MP Boost 1, MP Boost 2, Cola, Lipstick) as pickups and encounter rewards."
#define _ITEMS_PROB_AMMO_TIP_STR                                                                             \
    "Enter ammo probability here.\n\nThis value (0.0 - 1.0) determines the probability of dropping ammo "    \
    "items as pickups and encounter rewards."
#define _ITEMS_PROB_MISC_TIP_STR                                                                             \
    "Enter misc items probability here.\n\nThis value (0.0 - 1.0) determines the probability of dropping "   \
    "misc items (for example, items like Stim, Penicillin, Flare, Pepper Spray) as pickups and encounter "   \
    "rewards."
#define _ITEMS_PROB_UNIQUE_TIP_STR                                                                           \
    "Enter unique items probability here.\n\nThis value (0.0 - 1.0) determines the probability of dropping " \
    "unique items (armors, weapons, attachments, artifacts) as pickups and encounter rewards."
#define _ITEMS_STARTING_WEAPON_BOX_STR "Starting weapon: "
#define _ITEMS_STARTING_WEAPON_BOX_TIP_STR                                                                   \
    "Select your weapon option to start the game with.\n\n - Random starting weapon means that starting "    \
    "weapon will be randomzied among small pool of weapons that are considerd to be wearker. This "          \
    "includes: m93r, p08, pa3 and m4a1.\n - No random weapon option means that you will start with game "    \
    "default option (m93r).\n - Full random option will set initial weapon to be fully random meaning that " \
    "it can be any available weapon including hypervelocity and gunblade.\n - Tonfa only start means that "  \
    "m93r will be substituted with second tonfa.\n - Any other options set fixed weapon.\n\nNOTE: Dropped "  \
    "or selected weapon affects ammo drops, meaning rifle round will drop if m4a1 is your primary weapon "   \
    "and etc. This doesn't include ammo boxes though."
#define _ITEMS_STARTING_ARMOR_BOX_STR "Starting armor: "
#define _ITEMS_STARTING_ARMOR_BOX_TIP_STR                                                                    \
    "Select your armor option to start the game with when leaving shooting range and going to "              \
    "acropolis.\n\n - Random starting armor means that starting armor will be randomzied among small pool "  \
    "of armors that are considerd to be wearker. This includes: Leather jacket, PASGT vest, assault suit, "  \
    "turtle vest and shoulder holster.\n - No random armor option means that you will start with game "      \
    "default option (leather jacket).\n - Full random option will set initial armor to be fully random "     \
    "meaning that it can be any available armor including monk robe.\n - Any other options set fixed "       \
    "armor.\n\nNOTE: This armor will be available only during acropolis and MIST garage events."
#define _ITEMS_DRYFIELD_ARMOR_BOX_STR "Dryfield armor: "
#define _ITEMS_DRYFIELD_ARMOR_BOX_TIP_STR                                                                    \
    "Select your armor option to wear when going to dryfield.\n\n - Random starting armor means that "       \
    "dryfield armor will be randomzied among small pool of armors that are considerd to be wearker. This "   \
    "includes: Leather jacket, PASGT vest, assault suit, turtle vest and shoulder holster.\n - No random "   \
    "armor option means that you will go to dryfield with game default option (assault suit).\n - Full "     \
    "random option will set dryfield armor to be fully random meaning that it can be any available armor "   \
    "including monk robe.\n - Any other options set fixed armor."

#define ITEMS_OPTIONS_DIALOG_SIZE_WIDTH 800
#define ITEMS_OPTIONS_DIALOG_SIZE_HEIGHT 600
#define ITEMS_OPTIONS_POOL_SIZE_WIDTH 380
#define ITEMS_OPTIONS_POOL_SIZE_HEIGHT 500
#define ITEMS_OPTIONS_POOL_LABEL_WIDTH 150
#define ITEMS_OPTIONS_POOL_COLUMN_WIDTH 70

#define _ITEMS_LABEL_WEAPON_RANDOM_STARTING_WEAPON "Random starting weapon"
#define _ITEMS_LABEL_WEAPON_DEFAULT "No random weapon (Game default)"
#define _ITEMS_LABEL_WEAPON_FULL_RANDOM "Full random weapon"
#define _ITEMS_LABEL_WEAPON_TONFA_ONLY_START "Tonfa only start"
#define _ITEMS_LABEL_WEAPON_M93R "m93r"
#define _ITEMS_LABEL_WEAPON_M950 "m950"
#define _ITEMS_LABEL_WEAPON_P08 "p08"
#define _ITEMS_LABEL_WEAPON_P229 "p229"
#define _ITEMS_LABEL_WEAPON_MONGOOSE "Mongoose"
#define _ITEMS_LABEL_WEAPON_GRENADE_PISTOL "Grenade pistol"
#define _ITEMS_LABEL_WEAPON_MM1 "MM1 grenade launcher"
#define _ITEMS_LABEL_WEAPON_PA3 "PA3 shotgun"
#define _ITEMS_LABEL_WEAPON_SP12 "SP12 shotgun"
#define _ITEMS_LABEL_WEAPON_AS12 "AS12 shotgun"
#define _ITEMS_LABEL_WEAPON_M4A1 "m4a1 rifle"
#define _ITEMS_LABEL_WEAPON_M249 "m249 machine gun"
#define _ITEMS_LABEL_WEAPON_HYPERVELOCITY "Hypervelocity gun"
#define _ITEMS_LABEL_WEAPON_GUNBLADE "Gunblade"
#define _ITEMS_LABEL_WEAPON_MP5A5 "mp5a5 submachine gun"
#define _ITEMS_LABEL_WEAPON_P08_SNAIL "p08 with snail magazine"
#define _ITEMS_LABEL_M4A1_1_CLIP "m4a1 + 1 clip"
#define _ITEMS_LABEL_M4A1_2_CLIPS "m4a1 + 2 clips"
#define _ITEMS_LABEL_M4A1_HAMMER "m4a1 + hammer"
#define _ITEMS_LABEL_M4A1_KNIFE "m4a1 + M9 bayonet"
#define _ITEMS_LABEL_M4A1_GRENADE "m4a1 + M203 grenade launcher"
#define _ITEMS_LABEL_M4A1_PYKE "m4a1 + pyke"
#define _ITEMS_LABEL_M4A1_JAVELIN "m4a1 + javelin"
#define _ITEMS_LABEL_MP5A5_1_CLIP "mp5a5 + 1 clip"
#define _ITEMS_LABEL_MP5A5_2_CLIPS "mp5a5 + 2 clips"
#define _ITEMS_LABEL_ARMOR_RANDOM_STARTING_ARMOR "Random starting armor"
#define _ITEMS_LABEL_ARMOR_DEFAULT "No random armor (Game default)"
#define _ITEMS_LABEL_ARMOR_FULL_RANDOM_ARMOR "Full random armor"
#define _ITEMS_LABEL_ARMOR_LEATHER_JACKET "Leather jacket"
#define _ITEMS_LABEL_ARMOR_TACTICAL_ARMOR "Tactical armor"
#define _ITEMS_LABEL_ARMOR_COMBAT_ARMOR "Combat armor"
#define _ITEMS_LABEL_ARMOR_ASSAULT_SUIT "Assault suit"
#define _ITEMS_LABEL_ARMOR_PASGT_VEST "PASGT vest"
#define _ITEMS_LABEL_ARMOR_TACTICAL_VEST "Tactical vest"
#define _ITEMS_LABEL_ARMOR_EOD_SUIT "EOD suit"
#define _ITEMS_LABEL_ARMOR_TURTLE_VEST "Turtle vest"
#define _ITEMS_LABEL_ARMOR_CHICKEN_PLATE "Chicken plate"
#define _ITEMS_LABEL_ARMOR_NBC_SUIT "NBC suit"
#define _ITEMS_LABEL_ARMOR_PSYSUIT "PsySuit"
#define _ITEMS_LABEL_ARMOR_AYA_SPECIAL "Aya special"
#define _ITEMS_LABEL_ARMOR_SHOULDER_HOLSTER "Shoulder holster"
#define _ITEMS_LABEL_ARMOR_MONK_ROBE "Monk robe"

#define _ITEMS_VALUE_WEAPON_RANDOM_STARTING_WEAPON 0xff
#define _ITEMS_VALUE_WEAPON_DEFAULT 0xfe
#define _ITEMS_VALUE_WEAPON_FULL_RANDOM 0xfd
#define _ITEMS_VALUE_WEAPON_TONFA_ONLY_START 0x92
#define _ITEMS_VALUE_WEAPON_M93R 0x81
#define _ITEMS_VALUE_WEAPON_M950 0x82
#define _ITEMS_VALUE_WEAPON_P08 0x83
#define _ITEMS_VALUE_WEAPON_P229 0x84
#define _ITEMS_VALUE_WEAPON_MONGOOSE 0x88
#define _ITEMS_VALUE_WEAPON_GRENADE_PISTOL 0x8a
#define _ITEMS_VALUE_WEAPON_MM1 0x8b
#define _ITEMS_VALUE_WEAPON_PA3 0x8c
#define _ITEMS_VALUE_WEAPON_SP12 0x8d
#define _ITEMS_VALUE_WEAPON_AS12 0x8e
#define _ITEMS_VALUE_WEAPON_M4A1 0x8f
#define _ITEMS_VALUE_WEAPON_M249 0x90
#define _ITEMS_VALUE_WEAPON_HYPERVELOCITY 0x95
#define _ITEMS_VALUE_WEAPON_GUNBLADE 0x96
#define _ITEMS_VALUE_WEAPON_MP5A5 0x9d
#define _ITEMS_VALUE_WEAPON_P08_SNAIL 0x80
#define _ITEMS_VALUE_M4A1_1_CLIP 0x93
#define _ITEMS_VALUE_M4A1_2_CLIPS 0x94
#define _ITEMS_VALUE_M4A1_HAMMER 0x98
#define _ITEMS_VALUE_M4A1_KNIFE 0x99
#define _ITEMS_VALUE_M4A1_GRENADE 0x9a
#define _ITEMS_VALUE_M4A1_PYKE 0x9b
#define _ITEMS_VALUE_M4A1_JAVELIN 0x9c
#define _ITEMS_VALUE_MP5A5_1_CLIP 0x9e
#define _ITEMS_VALUE_MP5A5_2_CLIPS 0x9f
#define _ITEMS_VALUE_ARMOR_RANDOM_STARTING_ARMOR 0xfc
#define _ITEMS_VALUE_ARMOR_DEFAULT 0xfb
#define _ITEMS_VALUE_ARMOR_FULL_RANDOM_ARMOR 0xfa
#define _ITEMS_VALUE_ARMOR_LEATHER_JACKET 0x60
#define _ITEMS_VALUE_ARMOR_TACTICAL_ARMOR 0x61
#define _ITEMS_VALUE_ARMOR_COMBAT_ARMOR 0x62
#define _ITEMS_VALUE_ARMOR_ASSAULT_SUIT 0x63
#define _ITEMS_VALUE_ARMOR_PASGT_VEST 0x64
#define _ITEMS_VALUE_ARMOR_TACTICAL_VEST 0x65
#define _ITEMS_VALUE_ARMOR_EOD_SUIT 0x66
#define _ITEMS_VALUE_ARMOR_TURTLE_VEST 0x67
#define _ITEMS_VALUE_ARMOR_CHICKEN_PLATE 0x68
#define _ITEMS_VALUE_ARMOR_NBC_SUIT 0x69
#define _ITEMS_VALUE_ARMOR_PSYSUIT 0x6a
#define _ITEMS_VALUE_ARMOR_AYA_SPECIAL 0x6b
#define _ITEMS_VALUE_ARMOR_SHOULDER_HOLSTER 0x6c
#define _ITEMS_VALUE_ARMOR_MONK_ROBE 0x6d

class ItemsOptionsDialog : public wxFrame {
  public:
    ItemsOptionsDialog(std::shared_ptr<UIContext>, Header *);

    struct ItemPoolTableRecord {
        std::string name;
        bool drop;
        bool shop;
        int cost;
        uint8_t id;
        uint64_t flag[2];
    };

    struct ItemProbabilityCategory {
        std::string label;
        std::string tip;
        wxTextCtrl *input = nullptr;
    };

    struct StartinItemOption {
        std::string label;
        std::string tip;
        std::function<void(wxCommandEvent &)> on_select_handler;
        std::vector<std::string> options;
        wxComboBox **comboBox;
    };

    struct ArmorItem {
        std::string label;
        uint8_t id;
    };

  private:
    std::shared_ptr<UIContext> ctx;
    UIContext ctx_copy;
    Header *header;

    // event handler methods
    void OnClose(wxCloseEvent &);
    void OnCellLeftClick(wxGridEvent &);
    void OnShopCostMult(wxCommandEvent &);
    void OnApplyClick(wxCommandEvent &);
    void OnCancelClick(wxCommandEvent &);
    void OnDefaultClick(wxCommandEvent &);
    void OnProbabilityTextChanged(size_t);

    // sub-components rendering methods
    void DrawItemsScrollView();
    void DrawItemsAdditionalOptionsHeader();
    void DrawItemsProbabilities();
    void DrawItemsStartingOptions();
    void DrawItemsAcceptAndCancelButtons();

    // sub-components
    wxPanel *panel;
    wxBoxSizer *dialog_sizer;
    wxBoxSizer *item_pool_sizer;
    wxBoxSizer *item_options_sizer;
    wxBoxSizer *item_additional_options_sizer;
    wxGrid *items_pool_grid;
    wxComboBox *starting_weapon_box;
    wxComboBox *starting_armor_box;
    wxComboBox *dryfield_armor_box;
    wxTextCtrl *shop_cost_mult_input;

    // utility structs
    std::vector<ItemProbabilityCategory> item_probability_categories;
    std::array<StartinItemOption, std::extent_v<decltype(UIContext::items_start_options)>>
        starting_item_options;
    std::vector<ItemPoolTableRecord> item_pool_table_records{
        ItemPoolTableRecord{"[Co] Recovery1",
                            true,
                            true,
                            0x64,
                            0x1,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY1, 0x0}},
        ItemPoolTableRecord{"[Co] Recovery2",
                            true,
                            true,
                            0xb4,
                            0x2,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY2, 0x0}},
        ItemPoolTableRecord{"[Co] Recovery3",
                            true,
                            true,
                            0x15e,
                            0x3,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RECOVERY3, 0x0}},
        ItemPoolTableRecord{"[Co] Stim",
                            true,
                            true,
                            0xa0,
                            0x4,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__STIM, 0x0}},
        ItemPoolTableRecord{"[Co] Cola",
                            true,
                            true,
                            0x320,
                            0x5,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COLA, 0x0}},
        ItemPoolTableRecord{"[Co] MP Boost 1",
                            true,
                            true,
                            0x140,
                            0x6,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST1, 0x0}},
        ItemPoolTableRecord{"[Co] MP Boost 2",
                            true,
                            true,
                            0x244,
                            0x7,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__MP_BOOST2, 0x0}},
        ItemPoolTableRecord{"[Co] Penicillin",
                            true,
                            true,
                            0xa0,
                            0x8,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PENICILLIN, 0x0}},
        ItemPoolTableRecord{"[Co] Lipstick",
                            true,
                            true,
                            0x1388,
                            0xb,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__LIPSTICK, 0x0}},
        ItemPoolTableRecord{"[Co] Belt Pouch",
                            true,
                            true,
                            0x2710,
                            0xd,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__BELT_POUCH, 0x0}},
        ItemPoolTableRecord{"[Co] Flare",
                            true,
                            true,
                            0x12c,
                            0x3a,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__FLARE, 0x0}},
        ItemPoolTableRecord{"[Co] Pepper Spray",
                            true,
                            true,
                            0x1f4,
                            0x3b,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PEPPER_SPRAY, 0x0}},
        ItemPoolTableRecord{"[Co] Proteine Capsule",
                            true,
                            true,
                            0x2710,
                            0x3c,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__PROTEINE_CAPSULE, 0x0}},
        ItemPoolTableRecord{"[Co] Ringer's solution",
                            true,
                            true,
                            0x1770,
                            0x3d,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__RINGERS_SOLUTION, 0x0}},
        ItemPoolTableRecord{"[Co] Eau de toilette",
                            true,
                            true,
                            0xbe,
                            0x3e,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__EAU_DE_TOILETTE, 0x0}},
        ItemPoolTableRecord{"[Co] Combat light",
                            true,
                            true,
                            0xc8,
                            0x41,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__CONSUMABLE__COMBAT_LIGHT, 0x0}},
        ItemPoolTableRecord{"[At] SMG Clip Holder",
                            true,
                            true,
                            0xfc8,
                            0x9,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SMG_CLIP_HOLDER, 0x0}},
        ItemPoolTableRecord{"[At] Rifle Clip Holder",
                            true,
                            true,
                            0x780,
                            0xa,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__RIFLE_CLIP_HOLDER, 0x0}},
        ItemPoolTableRecord{"[At] Snail Magazine",
                            true,
                            true,
                            0x3e8,
                            0x0c,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__SNAIL_MAGAZINE, 0x0}},
        ItemPoolTableRecord{"[At] Hammer m4a1 Attach.",
                            true,
                            true,
                            0x2710,
                            0x42,
                            {UIContext::_UI_CONTEXT_ITEMS_COST__ATTACHMENT__HAMMER_M4A1, 0x0}},
        ItemPoolTableRecord{"[At] Pyke m4a1 Attach.",
                            true,
                            true,
                            0x2710,
                            0x43,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__PYKE_M4A1, 0x0}},
        ItemPoolTableRecord{"[At] Javelin m4a1 Attach.",
                            true,
                            true,
                            0x4e20,
                            0x44,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__JAVELIN_M4A1, 0x0}},
        ItemPoolTableRecord{"[At] m203 m4a1 Attach.",
                            true,
                            true,
                            0x2710,
                            0x45,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M203_M4A1, 0x0}},
        ItemPoolTableRecord{"[At] m9 knife m4a1 Attach.",
                            true,
                            true,
                            0x1d4c,
                            0x46,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ATTACHMENT__M9_M4A1, 0x0}},
        ItemPoolTableRecord{"[Ar] Leather jacket",
                            true,
                            true,
                            0x7d0,
                            0x60,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__LEATHER_JACKET, 0x0}},
        ItemPoolTableRecord{"[Ar] Tactical armor",
                            true,
                            true,
                            0x36b0,
                            0x61,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_ARMOR, 0x0}},
        ItemPoolTableRecord{"[Ar] Combat armor",
                            true,
                            true,
                            0x1964,
                            0x62,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__COMBAT_ARMOR, 0x0}},
        ItemPoolTableRecord{"[Ar] Assault suit",
                            true,
                            true,
                            0x7d0,
                            0x63,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__ASSAULT_SUIT, 0x0}},
        ItemPoolTableRecord{"[Ar] PASGT vest",
                            true,
                            true,
                            0x1770,
                            0x64,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PASGT_VEST, 0x0}},
        ItemPoolTableRecord{"[Ar] Tactical vest",
                            true,
                            true,
                            0x1f40,
                            0x65,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TACTICAL_VEST, 0x0}},
        ItemPoolTableRecord{"[Ar] EOD suit",
                            true,
                            true,
                            0x2ee0,
                            0x66,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__EOD_SUIT, 0x0}},
        ItemPoolTableRecord{"[Ar] Turtle vest",
                            true,
                            true,
                            0x1770,
                            0x67,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__TURTLE_VEST, 0x0}},
        ItemPoolTableRecord{"[Ar] Chicken plate",
                            true,
                            true,
                            0x2134,
                            0x68,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__CHICKEN_PLATE, 0x0}},
        ItemPoolTableRecord{"[Ar] NBC suit",
                            true,
                            true,
                            0x1d4c,
                            0x69,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__NBC_SUIT, 0x0}},
        ItemPoolTableRecord{"[Ar] PsySuit",
                            true,
                            true,
                            0x1f40,
                            0x6a,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__PSYSUIT, 0x0}},
        ItemPoolTableRecord{"[Ar] Aya Special",
                            true,
                            true,
                            0x3a98,
                            0x6b,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__AYA_SPECIAL, 0x0}},
        ItemPoolTableRecord{"[Ar] Shoulder holster",
                            true,
                            true,
                            0xa14,
                            0x6c,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__SHOULDER_HOLSTER, 0x0}},
        ItemPoolTableRecord{"[Ar] Monk robe",
                            false,
                            true,
                            0xfde8,
                            0x6d,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__ARMOR__MONK_ROBE, 0x0}},
        ItemPoolTableRecord{"[We] m93r",
                            true,
                            true,
                            0x7d0,
                            0x81,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M93R, 0x0}},
        ItemPoolTableRecord{"[We] m950",
                            true,
                            true,
                            0x1676,
                            0x82,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M950, 0x0}},
        ItemPoolTableRecord{"[We] p08",
                            true,
                            true,
                            0x9c4,
                            0x83,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P08, 0x0}},
        ItemPoolTableRecord{"[We] p229",
                            true,
                            true,
                            0x1770,
                            0x8,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__P229, 0x0}},
        ItemPoolTableRecord{"[We] Mongoose",
                            true,
                            true,
                            0x30d4,
                            0x88,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MONGOOSE, 0x0}},
        ItemPoolTableRecord{"[We] Grenade Pistol",
                            true,
                            true,
                            0x1388,
                            0x8a,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__GRENADE_PISTOL, 0x0}},
        ItemPoolTableRecord{"[We] Grenade Launcher MM1",
                            true,
                            true,
                            0x5bcc,
                            0x8b,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MM1, 0x0}},
        ItemPoolTableRecord{"[We] Shotgun PA3",
                            true,
                            true,
                            0xfa0,
                            0x8c,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__PA3, 0x0}},
        ItemPoolTableRecord{"[We] Shotgun SP12",
                            true,
                            true,
                            0x251c,
                            0x8d,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__SP12, 0x0}},
        ItemPoolTableRecord{"[We] Shotgun AS12",
                            true,
                            true,
                            0x57e4,
                            0x8e,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__AS12, 0x0}},
        ItemPoolTableRecord{"[We] m4a1 Rifle",
                            true,
                            true,
                            0xfa0,
                            0x8f,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M4A1, 0x0}},
        ItemPoolTableRecord{"[We] m249 Machine Gun",
                            false,
                            true,
                            0x61a8,
                            0x90,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__M249, 0x0}},
        ItemPoolTableRecord{"[We] Hypervelocity",
                            false,
                            true,
                            0xfde8,
                            0x95,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__HYPERVELOCITY, 0x0}},
        ItemPoolTableRecord{"[We] Gunblade",
                            false,
                            true,
                            0x7530,
                            0x96,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__GUNBLADE, 0x0}},
        ItemPoolTableRecord{"[We] mp5a5",
                            true,
                            true,
                            0x2710,
                            0x9d,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__WEAPON__MP5A5, 0x0}},
        ItemPoolTableRecord{"[Am] 9mm",
                            false,
                            true,
                            0x64,
                            0xa0,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_PB, 0x0}},
        ItemPoolTableRecord{"[Am] 9mm Hydra",
                            true,
                            true,
                            0x12c,
                            0xa1,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_HYDRA, 0x0}},
        ItemPoolTableRecord{"[Am] 9mm Spartan",
                            true,
                            true,
                            0x1f4,
                            0xa2,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__PISTOL_SPARTAN, 0x0}},
        ItemPoolTableRecord{"[Am] 44mm Magnum",
                            true,
                            true,
                            0xfa0,
                            0xa6,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM, 0x0}},
        ItemPoolTableRecord{"[Am] 44mm Maeda",
                            true,
                            true,
                            0x2328,
                            0xa7,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__MAGNUM_MAEDA, 0x0}},
        ItemPoolTableRecord{"[Am] Grenades",
                            true,
                            true,
                            0x190,
                            0xa9,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES, 0x0}},
        ItemPoolTableRecord{"[Am] Grenades Riot",
                            true,
                            true,
                            0x320,
                            0xaa,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_RIOT, 0x0}},
        ItemPoolTableRecord{"[Am] Grenades Airburst",
                            true,
                            true,
                            0x190,
                            0xab,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__GRENADES_AIRBIRST, 0x0}},
        ItemPoolTableRecord{"[Am] Shells Buckshot",
                            true,
                            true,
                            0x96,
                            0xac,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_BUCKSHOT, 0x0}},
        ItemPoolTableRecord{"[Am] Shells Firefly",
                            true,
                            true,
                            0x15e,
                            0xad,
                            {UIContext::_UI_CONTEXT_ITEMS_POOL_LOW_INT__AMMO__SHELLS_FIREFLY, 0x0}},
        ItemPoolTableRecord{"[Am] Shells Slug",
                            true,
                            true,
                            0x320,
                            0xae,
                            {0x0, UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__SHELLS_SLUG}},
        ItemPoolTableRecord{"[Am] Rifle Rounds",
                            true,
                            true,
                            0x96,
                            0xaf,
                            {0x0, UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__AMMO__RIFLE_ROUNDS}},
        ItemPoolTableRecord{"[Mi] Crystall Skull",
                            true,
                            true,
                            0x6b6c,
                            0x36,
                            {0x0, UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__CRYSTAL_SKULL}},
        ItemPoolTableRecord{"[Mi] Medicine Wheel",
                            true,
                            true,
                            0x6b6c,
                            0x37,
                            {0x0, UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MEDICINE_WHEEL}},
        ItemPoolTableRecord{"[Mi] Holy Water",
                            true,
                            true,
                            0x6b6c,
                            0x38,
                            {0x0, UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HOLY_WATER}},
        ItemPoolTableRecord{"[Mi] Ofuda Charm",
                            true,
                            true,
                            0x6b6c,
                            0x39,
                            {0x0, UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__OFUDA_CHARM}},
        ItemPoolTableRecord{"[Mi] Hunter goggles",
                            true,
                            true,
                            0x7d0,
                            0x3f,
                            {0x0, UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__HUNTER_GOGGLES}},
        ItemPoolTableRecord{"[Mi] GPS",
                            true,
                            true,
                            0x3e8,
                            0x40,
                            {0x0, UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__GPS}},
        ItemPoolTableRecord{"[Mi] MD Player",
                            true,
                            true,
                            0x1f4,
                            0xe,
                            {0x0, UIContext::_UI_CONTEXT_ITEMS_POOL_HIGH_INT__MISC__MD_PLAYER}}};
    std::map<std::string, uint8_t> str_to_uint_item_map{
        {_ITEMS_LABEL_WEAPON_RANDOM_STARTING_WEAPON, _ITEMS_VALUE_WEAPON_RANDOM_STARTING_WEAPON},
        {_ITEMS_LABEL_WEAPON_DEFAULT, _ITEMS_VALUE_WEAPON_DEFAULT},
        {_ITEMS_LABEL_WEAPON_FULL_RANDOM, _ITEMS_VALUE_WEAPON_FULL_RANDOM},
        {_ITEMS_LABEL_WEAPON_TONFA_ONLY_START, _ITEMS_VALUE_WEAPON_TONFA_ONLY_START},
        {_ITEMS_LABEL_WEAPON_M93R, _ITEMS_VALUE_WEAPON_M93R},
        {_ITEMS_LABEL_WEAPON_M950, _ITEMS_VALUE_WEAPON_M950},
        {_ITEMS_LABEL_WEAPON_P08, _ITEMS_VALUE_WEAPON_P08},
        {_ITEMS_LABEL_WEAPON_P229, _ITEMS_VALUE_WEAPON_P229},
        {_ITEMS_LABEL_WEAPON_MONGOOSE, _ITEMS_VALUE_WEAPON_MONGOOSE},
        {_ITEMS_LABEL_WEAPON_GRENADE_PISTOL, _ITEMS_VALUE_WEAPON_GRENADE_PISTOL},
        {_ITEMS_LABEL_WEAPON_MM1, _ITEMS_VALUE_WEAPON_MM1},
        {_ITEMS_LABEL_WEAPON_PA3, _ITEMS_VALUE_WEAPON_PA3},
        {_ITEMS_LABEL_WEAPON_SP12, _ITEMS_VALUE_WEAPON_SP12},
        {_ITEMS_LABEL_WEAPON_AS12, _ITEMS_VALUE_WEAPON_AS12},
        {_ITEMS_LABEL_WEAPON_M4A1, _ITEMS_VALUE_WEAPON_M4A1},
        {_ITEMS_LABEL_WEAPON_M249, _ITEMS_VALUE_WEAPON_M249},
        {_ITEMS_LABEL_WEAPON_HYPERVELOCITY, _ITEMS_VALUE_WEAPON_HYPERVELOCITY},
        {_ITEMS_LABEL_WEAPON_GUNBLADE, _ITEMS_VALUE_WEAPON_GUNBLADE},
        {_ITEMS_LABEL_WEAPON_MP5A5, _ITEMS_VALUE_WEAPON_MP5A5},
        {_ITEMS_LABEL_WEAPON_P08_SNAIL, _ITEMS_VALUE_WEAPON_P08_SNAIL},
        {_ITEMS_LABEL_M4A1_1_CLIP, _ITEMS_VALUE_M4A1_1_CLIP},
        {_ITEMS_LABEL_M4A1_2_CLIPS, _ITEMS_VALUE_M4A1_2_CLIPS},
        {_ITEMS_LABEL_M4A1_HAMMER, _ITEMS_VALUE_M4A1_HAMMER},
        {_ITEMS_LABEL_M4A1_KNIFE, _ITEMS_VALUE_M4A1_KNIFE},
        {_ITEMS_LABEL_M4A1_GRENADE, _ITEMS_VALUE_M4A1_GRENADE},
        {_ITEMS_LABEL_M4A1_PYKE, _ITEMS_VALUE_M4A1_PYKE},
        {_ITEMS_LABEL_M4A1_JAVELIN, _ITEMS_VALUE_M4A1_JAVELIN},
        {_ITEMS_LABEL_MP5A5_1_CLIP, _ITEMS_VALUE_MP5A5_1_CLIP},
        {_ITEMS_LABEL_MP5A5_2_CLIPS, _ITEMS_VALUE_MP5A5_2_CLIPS},
        {_ITEMS_LABEL_ARMOR_RANDOM_STARTING_ARMOR, _ITEMS_VALUE_ARMOR_RANDOM_STARTING_ARMOR},
        {_ITEMS_LABEL_ARMOR_DEFAULT, _ITEMS_VALUE_ARMOR_DEFAULT},
        {_ITEMS_LABEL_ARMOR_FULL_RANDOM_ARMOR, _ITEMS_VALUE_ARMOR_FULL_RANDOM_ARMOR},
        {_ITEMS_LABEL_ARMOR_LEATHER_JACKET, _ITEMS_VALUE_ARMOR_LEATHER_JACKET},
        {_ITEMS_LABEL_ARMOR_TACTICAL_ARMOR, _ITEMS_VALUE_ARMOR_TACTICAL_ARMOR},
        {_ITEMS_LABEL_ARMOR_COMBAT_ARMOR, _ITEMS_VALUE_ARMOR_COMBAT_ARMOR},
        {_ITEMS_LABEL_ARMOR_ASSAULT_SUIT, _ITEMS_VALUE_ARMOR_ASSAULT_SUIT},
        {_ITEMS_LABEL_ARMOR_PASGT_VEST, _ITEMS_VALUE_ARMOR_PASGT_VEST},
        {_ITEMS_LABEL_ARMOR_TACTICAL_VEST, _ITEMS_VALUE_ARMOR_TACTICAL_VEST},
        {_ITEMS_LABEL_ARMOR_EOD_SUIT, _ITEMS_VALUE_ARMOR_EOD_SUIT},
        {_ITEMS_LABEL_ARMOR_TURTLE_VEST, _ITEMS_VALUE_ARMOR_TURTLE_VEST},
        {_ITEMS_LABEL_ARMOR_CHICKEN_PLATE, _ITEMS_VALUE_ARMOR_CHICKEN_PLATE},
        {_ITEMS_LABEL_ARMOR_NBC_SUIT, _ITEMS_VALUE_ARMOR_NBC_SUIT},
        {_ITEMS_LABEL_ARMOR_PSYSUIT, _ITEMS_VALUE_ARMOR_PSYSUIT},
        {_ITEMS_LABEL_ARMOR_AYA_SPECIAL, _ITEMS_VALUE_ARMOR_AYA_SPECIAL},
        {_ITEMS_LABEL_ARMOR_SHOULDER_HOLSTER, _ITEMS_VALUE_ARMOR_SHOULDER_HOLSTER},
        {_ITEMS_LABEL_ARMOR_MONK_ROBE, _ITEMS_VALUE_ARMOR_MONK_ROBE},
    };
    std::map<uint8_t, std::string> uint_to_str_item_map{
        {_ITEMS_VALUE_WEAPON_RANDOM_STARTING_WEAPON, _ITEMS_LABEL_WEAPON_RANDOM_STARTING_WEAPON},
        {_ITEMS_VALUE_WEAPON_DEFAULT, _ITEMS_LABEL_WEAPON_DEFAULT},
        {_ITEMS_VALUE_WEAPON_FULL_RANDOM, _ITEMS_LABEL_WEAPON_FULL_RANDOM},
        {_ITEMS_VALUE_WEAPON_TONFA_ONLY_START, _ITEMS_LABEL_WEAPON_TONFA_ONLY_START},
        {_ITEMS_VALUE_WEAPON_M93R, _ITEMS_LABEL_WEAPON_M93R},
        {_ITEMS_VALUE_WEAPON_M950, _ITEMS_LABEL_WEAPON_M950},
        {_ITEMS_VALUE_WEAPON_P08, _ITEMS_LABEL_WEAPON_P08},
        {_ITEMS_VALUE_WEAPON_P229, _ITEMS_LABEL_WEAPON_P229},
        {_ITEMS_VALUE_WEAPON_MONGOOSE, _ITEMS_LABEL_WEAPON_MONGOOSE},
        {_ITEMS_VALUE_WEAPON_GRENADE_PISTOL, _ITEMS_LABEL_WEAPON_GRENADE_PISTOL},
        {_ITEMS_VALUE_WEAPON_MM1, _ITEMS_LABEL_WEAPON_MM1},
        {_ITEMS_VALUE_WEAPON_PA3, _ITEMS_LABEL_WEAPON_PA3},
        {_ITEMS_VALUE_WEAPON_SP12, _ITEMS_LABEL_WEAPON_SP12},
        {_ITEMS_VALUE_WEAPON_AS12, _ITEMS_LABEL_WEAPON_AS12},
        {_ITEMS_VALUE_WEAPON_M4A1, _ITEMS_LABEL_WEAPON_M4A1},
        {_ITEMS_VALUE_WEAPON_M249, _ITEMS_LABEL_WEAPON_M249},
        {_ITEMS_VALUE_WEAPON_HYPERVELOCITY, _ITEMS_LABEL_WEAPON_HYPERVELOCITY},
        {_ITEMS_VALUE_WEAPON_GUNBLADE, _ITEMS_LABEL_WEAPON_GUNBLADE},
        {_ITEMS_VALUE_WEAPON_MP5A5, _ITEMS_LABEL_WEAPON_MP5A5},
        {_ITEMS_VALUE_WEAPON_P08_SNAIL, _ITEMS_LABEL_WEAPON_P08_SNAIL},
        {_ITEMS_VALUE_M4A1_1_CLIP, _ITEMS_LABEL_M4A1_1_CLIP},
        {_ITEMS_VALUE_M4A1_2_CLIPS, _ITEMS_LABEL_M4A1_2_CLIPS},
        {_ITEMS_VALUE_M4A1_HAMMER, _ITEMS_LABEL_M4A1_HAMMER},
        {_ITEMS_VALUE_M4A1_KNIFE, _ITEMS_LABEL_M4A1_KNIFE},
        {_ITEMS_VALUE_M4A1_GRENADE, _ITEMS_LABEL_M4A1_GRENADE},
        {_ITEMS_VALUE_M4A1_PYKE, _ITEMS_LABEL_M4A1_PYKE},
        {_ITEMS_VALUE_M4A1_JAVELIN, _ITEMS_LABEL_M4A1_JAVELIN},
        {_ITEMS_VALUE_MP5A5_1_CLIP, _ITEMS_LABEL_MP5A5_1_CLIP},
        {_ITEMS_VALUE_MP5A5_2_CLIPS, _ITEMS_LABEL_MP5A5_2_CLIPS},
        {_ITEMS_VALUE_ARMOR_RANDOM_STARTING_ARMOR, _ITEMS_LABEL_ARMOR_RANDOM_STARTING_ARMOR},
        {_ITEMS_VALUE_ARMOR_DEFAULT, _ITEMS_LABEL_ARMOR_DEFAULT},
        {_ITEMS_VALUE_ARMOR_FULL_RANDOM_ARMOR, _ITEMS_LABEL_ARMOR_FULL_RANDOM_ARMOR},
        {_ITEMS_VALUE_ARMOR_LEATHER_JACKET, _ITEMS_LABEL_ARMOR_LEATHER_JACKET},
        {_ITEMS_VALUE_ARMOR_TACTICAL_ARMOR, _ITEMS_LABEL_ARMOR_TACTICAL_ARMOR},
        {_ITEMS_VALUE_ARMOR_COMBAT_ARMOR, _ITEMS_LABEL_ARMOR_COMBAT_ARMOR},
        {_ITEMS_VALUE_ARMOR_ASSAULT_SUIT, _ITEMS_LABEL_ARMOR_ASSAULT_SUIT},
        {_ITEMS_VALUE_ARMOR_PASGT_VEST, _ITEMS_LABEL_ARMOR_PASGT_VEST},
        {_ITEMS_VALUE_ARMOR_TACTICAL_VEST, _ITEMS_LABEL_ARMOR_TACTICAL_VEST},
        {_ITEMS_VALUE_ARMOR_EOD_SUIT, _ITEMS_LABEL_ARMOR_EOD_SUIT},
        {_ITEMS_VALUE_ARMOR_TURTLE_VEST, _ITEMS_LABEL_ARMOR_TURTLE_VEST},
        {_ITEMS_VALUE_ARMOR_CHICKEN_PLATE, _ITEMS_LABEL_ARMOR_CHICKEN_PLATE},
        {_ITEMS_VALUE_ARMOR_NBC_SUIT, _ITEMS_LABEL_ARMOR_NBC_SUIT},
        {_ITEMS_VALUE_ARMOR_PSYSUIT, _ITEMS_LABEL_ARMOR_PSYSUIT},
        {_ITEMS_VALUE_ARMOR_AYA_SPECIAL, _ITEMS_LABEL_ARMOR_AYA_SPECIAL},
        {_ITEMS_VALUE_ARMOR_SHOULDER_HOLSTER, _ITEMS_LABEL_ARMOR_SHOULDER_HOLSTER},
        {_ITEMS_VALUE_ARMOR_MONK_ROBE, _ITEMS_LABEL_ARMOR_MONK_ROBE},
    };

    wxDECLARE_EVENT_TABLE();
};
