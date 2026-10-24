#include "ui/widgets/body/dialog/AdvancedOptionsDialog.hpp"
#include "ui/shared/Layout.hpp"

wxBEGIN_EVENT_TABLE(AdvancedOptionsDialog, wxFrame) EVT_CLOSE(AdvancedOptionsDialog::OnClose)
    wxEND_EVENT_TABLE()

        const std::string AdvancedOptionsDialog::_ADVANCED_OPTIONS_DIALOG_TITLE = "Advanced Options";
const std::vector<AdvancedOptionsDialog::Modificator> AdvancedOptionsDialog::_ADVANCED_OPTIONS_MODIFICATORS{
    {
        .name = "Parasite Eve 1 music",
        .tip = "Replaces gameplay music with 37 adapted PE1 tracks and phases, grouped by mood. Overrides "
               "regular music randomization while enabled. The music pack is included.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = uint64_t{1} << 32,
        .setting = &UIContext::pe1_music,
    },
    {
        .name = "No magic",
        .tip = "Disables player Parasite Energy, removes its menu command and blocks the magic wheel. "
               "Overrides magic randomization, Magic elements and Active time battle while enabled. "
               "Enemy abilities and ordinary items remain available.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = uint64_t{1} << 33,
        .setting = &UIContext::no_magic,
    },
    {
        .name = "Rupert's revolver sound",
        .tip = "Uses Rupert's cafeteria cutscene gunshot for the Mongoose. Works with either ammo type and "
               "any player model; weapon stats stay unchanged.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__RUPERT_REVOLVER,
    },
    {
        .name = "Kyle's P229",
        .tip = "Uses Kyle's unsilenced P229 model, texture and gunshot. Keeps the player's ammo choices, "
               "stats and flashlight secondary attack. Kyle's companion damage uses Spartan ammo, not a "
               "separate stronger weapon table.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__KYLE_P229,
    },
    Modificator{
        .name = "Ammo boxes:",
        .tip = "No ammo boxes disables infinite ammo boxes. One-time ammo boxes refill ammo only once. "
               "Default keeps infinite refills.",
        .type = MODIFICATOR_TYPE_MULTIPLE_OPTIONS,
        .options{ModificatorOption{.id = NO_AMMO_BOXES,
                                   .name = "No ammo boxes",
                                   .is_default = false,
                                   .flag = UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES__NO_AMMO_BOXES},
                 ModificatorOption{.id = ONE_TIME_BOXES,
                                   .name = "One-time ammo boxes",
                                   .is_default = false,
                                   .flag = UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES__ONE_TIME},
                 ModificatorOption{.id = DEFAULT_BOXES,
                                   .name = "Default",
                                   .is_default = true,
                                   .flag = UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES__DEFAULT}},
    },
    Modificator{
        .name = "Ammo boxes randomization:",
        .tip = "Randomized boxes choose ammo for weapons in the generated pool. Full random boxes can choose "
               "ammo for other weapons. Unrandomized boxes keep their original ammo types, even when those types are disabled in the drop pool.",
        .type = MODIFICATOR_TYPE_MULTIPLE_OPTIONS,
        .options{
            ModificatorOption{.id = RANDOMIZED_BOXES,
                              .name = "Randomized boxes",
                              .is_default = false,
                              .flag =
                                  UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES_RANDOMIZATION__RANDOMIZED_BOXES},
            ModificatorOption{.id = FULL_RANDOMIZED_BOXES,
                              .name = "Full random boxes",
                              .is_default = false,
                              .flag =
                                  UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES_RANDOMIZATION__FULL_RANDOM_BOXES},
            ModificatorOption{.id = UNRANDOMIZED_BOXES,
                              .name = "Unrandomized boxes",
                              .is_default = true,
                              .flag = UIContext::_UI_CONTEXT_FLAG__AMMO_BOXES_RANDOMIZATION__UNRANDOMIZED}}},
    {
        .name = "Magic elements",
        .tip = "Turns Aya's magic into consumable items that can only be used from attachments.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__MAGIC_ELEMENTS,
    },
    {
        .name = "Active time battle",
        .tip = "Keeps battles running while the magic menu is open. The triggers fire primary and secondary "
               "weapons; the upper shoulder buttons select magic.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__ACTIVE_TIME_BATTLE,
    },
    {
        .name = "New armors",
        .tip = "Adds new armors with their own inventory images, stats and special effects to armor "
               "randomization.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__NEW_ARMORS,
    },
    {
        .name = "Cloak GOLEM multi-encounters",
        .tip = "Allows Knight and Bishop GOLEMs to appear alongside other enemies.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__CLOAK_GOLEM_MULT,
    },
    {
        .name = "Enable HUD coloring",
        .tip = "Enables the custom HUD color selected below.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__HUD_COLORING,
    },
    {
        .name = "Full ammo start",
        .tip = "Starts the game with a full stack of ammo for the selected starting weapon.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__FULL_AMMO_START,
    },
    {
        .name = "Entrance i-frames",
        .tip = "Grants Aya 20 frames of invulnerability when entering a room.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__INCREASED_INV_FRAMES,
    },
    {
        .name = "No white screen",
        .tip = "Skips the white flash when enemies become aggressive.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__NO_WHITE_SCREEN,
    },
    {
        .name = "No win pose",
        .tip = "Skips the reload animation at the end of a battle.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__NO_END_BATTLE_ANIM,
    },
    {
        .name = "No turning",
        .tip = "Disables automatic turning when aiming.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__NO_TURN,
    },
    {
        .name = "Inventory during battles",
        .tip = "Allows opening the inventory during battles.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__INVENTORY_MODE,
    },
    {
        .name = "Unlock replay modes",
        .tip = "Unlocks Replay, Bounty, Scavenger and Nightmare modes.",
        .type = MODIFICATOR_TYPE_ON_OFF,
        .options = {},
        .flag = UIContext::_UI_CONTEXT_FLAG__ENABLE_ALL_MODES,
    }};

AdvancedOptionsDialog::AdvancedOptionsDialog(std::shared_ptr<UIContext> _ctx)
    : wxFrame(NULL, wxID_ANY, _ADVANCED_OPTIONS_DIALOG_TITLE, wxDefaultPosition,
              wxSize(_ADVANCED_OPTIONS_DIALOG_WIDTH, _ADVANCED_OPTIONS_DIALOG_HEIGHT)),
      ctx(_ctx) {
    panel = new wxPanel(this, wxID_ANY);
    dialog_sizer = new wxBoxSizer(wxVERTICAL);
    dialog_inner_sizer = new wxBoxSizer(wxVERTICAL);

    panel->SetSizer(dialog_sizer);
    dialog_sizer->Add(dialog_inner_sizer, 1, wxEXPAND | wxALL, 25);

    ctx_copy = UIContext(0, ctx->logger);
    ctx_copy.player_model_option = _ctx->player_model_option;
    ctx_copy.flags = _ctx->flags;
    ctx_copy.pe1_music = _ctx->pe1_music;
    ctx_copy.no_magic = _ctx->no_magic;

    DrawModificators();
    ui::fitContent(this, panel);
}

void AdvancedOptionsDialog::DrawModificators() {
    auto *ammoRow = new wxBoxSizer(wxHORIZONTAL);
    size_t radioIndex = 0;
    for (const auto &mod : _ADVANCED_OPTIONS_MODIFICATORS) {
        if (mod.type != MODIFICATOR_TYPE_MULTIPLE_OPTIONS)
            continue;
        auto *group = new wxStaticBoxSizer(wxVERTICAL, panel, mod.name);
        group->GetStaticBox()->SetToolTip(mod.tip);
        bool first = true;
        for (const auto &option : mod.options) {
            auto *radio = new wxRadioButton(panel, wxID_ANY, option.name, wxDefaultPosition, wxDefaultSize,
                                            first ? wxRB_GROUP : 0);
            first = false;
            radio->SetValue((ctx_copy.flags & option.flag) != 0);
            radio->SetToolTip(mod.tip);
            radiobuttons[radioIndex++] = radio;
            group->Add(radio, 0, wxALL, 3);
        }
        if (ammoRow->GetItemCount())
            ammoRow->AddSpacer(12);
        ammoRow->Add(group, 1, wxEXPAND);
    }
    dialog_inner_sizer->Add(ammoRow, 0, wxEXPAND | wxBOTTOM, 12);

    auto *mutators = new wxStaticBoxSizer(wxVERTICAL, panel, "Special Modes / Mutators");
    auto *lists = new wxBoxSizer(wxHORIZONTAL);
    auto *availableColumn = new wxBoxSizer(wxVERTICAL);
    auto *activeColumn = new wxBoxSizer(wxVERTICAL);
    availableLabel = new wxStaticText(panel, wxID_ANY, "Available");
    activeLabel = new wxStaticText(panel, wxID_ANY, "Active");
    int listWidth = FromDIP(260);
    for (const auto &mod : _ADVANCED_OPTIONS_MODIFICATORS) {
        if (mod.type == MODIFICATOR_TYPE_ON_OFF)
            listWidth = std::max(listWidth, panel->GetTextExtent(mod.name).x + FromDIP(48));
    }
    availableList = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxSize(listWidth, FromDIP(190)), 0,
                                  nullptr, wxLB_SINGLE);
    activeList = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxSize(listWidth, FromDIP(190)), 0,
                               nullptr, wxLB_SINGLE);
    availableList->SetName("Available mutators");
    activeList->SetName("Active mutators");
    availableColumn->Add(availableLabel, 0, wxBOTTOM, 5);
    availableColumn->Add(availableList, 1, wxEXPAND);
    activeColumn->Add(activeLabel, 0, wxBOTTOM, 5);
    activeColumn->Add(activeList, 1, wxEXPAND);
    auto *actions = new wxBoxSizer(wxVERTICAL);
    addButton = new wxButton(panel, wxID_ANY, "Add >");
    removeButton = new wxButton(panel, wxID_ANY, "< Remove");
    actions->AddStretchSpacer();
    actions->Add(addButton, 0, wxEXPAND | wxBOTTOM, 8);
    actions->Add(removeButton, 0, wxEXPAND);
    actions->AddStretchSpacer();
    lists->Add(availableColumn, 1, wxEXPAND);
    lists->Add(actions, 0, wxEXPAND | wxLEFT | wxRIGHT, 12);
    lists->Add(activeColumn, 1, wxEXPAND);
    mutators->Add(lists, 1, wxEXPAND | wxALL, 8);
    mutatorDescription =
        new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
    mutatorDescription->SetMinSize(wxSize(2 * listWidth, FromDIP(64)));
    mutators->Add(mutatorDescription, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
    dialog_inner_sizer->Add(mutators, 1, wxEXPAND | wxBOTTOM, 12);
    availableList->Bind(wxEVT_LISTBOX, [this](wxCommandEvent &) { SelectMutator(availableList); });
    activeList->Bind(wxEVT_LISTBOX, [this](wxCommandEvent &) { SelectMutator(activeList); });
    availableList->Bind(wxEVT_LISTBOX_DCLICK, [this](wxCommandEvent &) { MoveMutator(true); });
    activeList->Bind(wxEVT_LISTBOX_DCLICK, [this](wxCommandEvent &) { MoveMutator(false); });
    addButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { MoveMutator(true); });
    removeButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { MoveMutator(false); });

    auto *colorGroup = new wxStaticBoxSizer(wxHORIZONTAL, panel, "HUD Color Scheme");
    const char *channelNames[] = {"Red", "Green", "Blue"};
    const int colors[] = {ctx->hud_color_r, ctx->hud_color_g, ctx->hud_color_b};
    for (size_t i = 0; i < 3; ++i) {
        colorSliderLabels[i] = new wxStaticText(panel, wxID_ANY, channelNames[i]);
        colorSliders[i] = new wxSlider(panel, wxID_ANY, colors[i], 0, 255);
        colorSliders[i]->SetName(wxString::Format("HUD %s", channelNames[i]));
        colorSliders[i]->Bind(wxEVT_SLIDER, &AdvancedOptionsDialog::OnSliderChanged, this);
        colorGroup->Add(colorSliderLabels[i], 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 8);
        colorGroup->Add(colorSliders[i], 1, wxALIGN_CENTER_VERTICAL | wxALL, 6);
    }
    colorPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, FromDIP(wxSize(32, 32)));
    colorPanel->SetBackgroundColour(wxColour(colors[0], colors[1], colors[2]));
    colorGroup->Add(colorPanel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 8);
    dialog_inner_sizer->Add(colorGroup, 0, wxEXPAND | wxBOTTOM, 12);

    auto *costumeRow = new wxBoxSizer(wxHORIZONTAL);
    costumeRow->Add(new wxStaticText(panel, wxID_ANY, "Player model:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT,
                    12);
    const wxArrayString costumes = {"Default",
                                    "Random",
                                    "Aya (Shoulder Holster with ear protection)",
                                    "Aya (Shoulder Holster)",
                                    "Aya (Leather Jacket)",
                                    "Aya (Assault Suit)",
                                    "Regina (Dino Crisis 2 / Default)",
                                    "Regina (Dino Crisis 2 / Lightweight armor)",
                                    "Jodie Bouquet (MIST Garage)",
                                    "Jodie Bouquet (Military Camp)",
                                    "Cafeteria woman NPC",
                                    "Toilet woman NPC",
                                    "Jill Valentine (Resident Evil 1)",
                                    "Kyle Madigan",
                                    "Rupert Broderick",
                                    "Gary Douglas",
                                    "Police officer",
                                    "Lieutenant",
                                    "President",
                                    "Eric Baldwin",
                                    "Soldier",
                                    "Pierce Carradine"};
    starting_costume_combobox =
        new wxComboBox(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, costumes, wxCB_READONLY);
    starting_costume_combobox->SetSelection(
        ctx_copy.player_model_option < costumes.size() ? ctx_copy.player_model_option : 0);
    starting_costume_combobox->SetToolTip(
        "Default keeps the original player model. Other options use one costume throughout the game.");
    costumeRow->Add(starting_costume_combobox, 1, wxEXPAND);
    dialog_inner_sizer->Add(costumeRow, 0, wxEXPAND | wxBOTTOM, 16);

    auto *buttons = new wxBoxSizer(wxHORIZONTAL);
    auto *defaults = new wxButton(panel, wxID_ANY, "Default");
    auto *cancel = new wxButton(panel, wxID_CANCEL, "Cancel");
    auto *apply = new wxButton(panel, wxID_OK, "Apply");
    defaults->Bind(wxEVT_BUTTON, &AdvancedOptionsDialog::OnDefaultEvent, this);
    cancel->Bind(wxEVT_BUTTON, &AdvancedOptionsDialog::OnCancelEvent, this);
    apply->Bind(wxEVT_BUTTON, &AdvancedOptionsDialog::OnApplyEvent, this);
    buttons->Add(defaults, 0);
    buttons->AddStretchSpacer();
    buttons->Add(cancel, 0, wxRIGHT, 12);
    buttons->Add(apply, 0);
    dialog_inner_sizer->Add(buttons, 0, wxEXPAND);
    RefreshMutators();
}

void AdvancedOptionsDialog::RefreshMutators(uint64_t selectedFlag) {
    availableList->Clear();
    activeList->Clear();
    availableMods.clear();
    activeMods.clear();
    wxListBox *selectedList = nullptr;
    for (const auto &mod : _ADVANCED_OPTIONS_MODIFICATORS) {
        if (mod.type != MODIFICATOR_TYPE_ON_OFF)
            continue;
        const bool active = mod.setting ? ctx_copy.*mod.setting : (ctx_copy.flags & mod.flag) != 0;
        auto *list = active ? activeList : availableList;
        auto &mods = active ? activeMods : availableMods;
        mods.push_back(&mod);
        const bool overridden = ctx_copy.no_magic && !mod.setting &&
                                (mod.flag == UIContext::_UI_CONTEXT_FLAG__MAGIC_ELEMENTS ||
                                 mod.flag == UIContext::_UI_CONTEXT_FLAG__ACTIVE_TIME_BATTLE);
        const int index = list->Append(mod.name + (active && overridden ? " (overridden by No magic)" : ""));
        if ((mod.flag) == selectedFlag) {
            list->SetSelection(index);
            selectedList = list;
        }
    }
    availableLabel->SetLabel(wxString::Format("Available (%zu)", availableMods.size()));
    activeLabel->SetLabel(wxString::Format("Active (%zu)", activeMods.size()));
    if (!selectedList) {
        selectedList = availableMods.empty() ? activeList : availableList;
        if (selectedList->GetCount())
            selectedList->SetSelection(0);
    }
    SelectMutator(selectedList);
    UpdateSlidersEnabled();
}

void AdvancedOptionsDialog::SelectMutator(wxListBox *list) {
    auto *other = list == availableList ? activeList : availableList;
    other->SetSelection(wxNOT_FOUND);
    const auto &mods = list == availableList ? availableMods : activeMods;
    const int index = list->GetSelection();
    const bool selected = index != wxNOT_FOUND;
    addButton->Enable(selected && list == availableList);
    removeButton->Enable(selected && list == activeList);
    mutatorDescription->SetLabel(selected ? wxString(mods[index]->tip)
                                          : wxString("Select a mutator to read its description."));
    // The first selection is made before the sizer assigns the final width.
    mutatorDescription->Wrap(
        std::max(mutatorDescription->GetClientSize().x, mutatorDescription->GetMinSize().x));
}

void AdvancedOptionsDialog::MoveMutator(bool activate) {
    auto *list = activate ? availableList : activeList;
    const auto &mods = activate ? availableMods : activeMods;
    const int index = list->GetSelection();
    if (index == wxNOT_FOUND)
        return;
    const auto &mod = *mods[index];
    if (mod.setting)
        ctx_copy.*mod.setting = activate;
    else if (activate)
        ctx_copy.flags |= static_cast<uint32_t>(mod.flag);
    else
        ctx_copy.flags &= ~static_cast<uint32_t>(mod.flag);
    RefreshMutators(mod.flag);
    (activate ? activeList : availableList)->SetFocus();
}

void AdvancedOptionsDialog::OnClose(wxCloseEvent &event) {
    (void)event;
    Destroy();
}

void AdvancedOptionsDialog::OnApplyEvent(wxCommandEvent &event) {
    (void)event;

    const int selected = starting_costume_combobox->GetSelection();
    ctx_copy.player_model_option = selected == wxNOT_FOUND ? 0 : static_cast<uint8_t>(selected);
    size_t radioIndex = 0;
    for (const auto &mod : _ADVANCED_OPTIONS_MODIFICATORS) {
        if (mod.type != MODIFICATOR_TYPE_MULTIPLE_OPTIONS)
            continue;
        for (const auto &option : mod.options) {
            if (radiobuttons[radioIndex++]->GetValue())
                ctx_copy.flags |= option.flag;
            else
                ctx_copy.flags &= ~option.flag;
        }
    }
    const bool items_dist_changed = (ctx_copy.flags & UIContext::_UI_CONTEXT_FLAG__MAGIC_ELEMENTS) &&
                                    ctx->items_dist[0] == 16 && ctx->items_dist[1] == 14 &&
                                    ctx->items_dist[2] == 48 && ctx->items_dist[3] == 17 &&
                                    ctx->items_dist[4] == 5;
    if (items_dist_changed) {
        for (size_t i = 0; i < 5; ++i)
            ctx_copy.items_dist[i] = ctx->items_dist[i];
        ctx_copy.items_dist[2] = 28;
        ctx_copy.items_dist[3] = 37;
    }

    ctx->player_model_option = ctx_copy.player_model_option;
    ctx->flags = ctx_copy.flags;
    ctx->pe1_music = ctx_copy.pe1_music;
    ctx->no_magic = ctx_copy.no_magic;
    if (items_dist_changed) {
        ctx->items_dist[0] = ctx_copy.items_dist[0];
        ctx->items_dist[1] = ctx_copy.items_dist[1];
        ctx->items_dist[2] = ctx_copy.items_dist[2];
        ctx->items_dist[3] = ctx_copy.items_dist[3];
        ctx->items_dist[4] = ctx_copy.items_dist[4];
    }

    if (ctx_copy.flags & UIContext::_UI_CONTEXT_FLAG__HUD_COLORING) {
        ctx->hud_color_r = colorSliders[0]->GetValue();
        ctx->hud_color_g = colorSliders[1]->GetValue();
        ctx->hud_color_b = colorSliders[2]->GetValue();
    }

    ctx->notifyListeners();
    Destroy();
}

void AdvancedOptionsDialog::OnCancelEvent(wxCommandEvent &event) {
    (void)event;
    Destroy();
}

void AdvancedOptionsDialog::OnDefaultEvent(wxCommandEvent &event) {
    (void)event;
    radiobuttons[2]->SetValue(true);
    radiobuttons[5]->SetValue(true);
    for (const auto &mod : _ADVANCED_OPTIONS_MODIFICATORS) {
        if (mod.type == MODIFICATOR_TYPE_ON_OFF)
            ctx_copy.flags &= ~static_cast<uint32_t>(mod.flag);
    }
    ctx_copy.pe1_music = false;
    ctx_copy.no_magic = false;
    ctx_copy.flags |= UIContext::_UI_CONTEXT_FLAG__ENABLE_ALL_MODES;
    RefreshMutators();
    starting_costume_combobox->SetSelection(0);
}

void AdvancedOptionsDialog::OnSliderChanged(wxCommandEvent &event) {
    (void)event;

    int red = colorSliders[0]->GetValue();
    int green = colorSliders[1]->GetValue();
    int blue = colorSliders[2]->GetValue();

    colorPanel->SetBackgroundColour(wxColour(red, green, blue));
    colorPanel->Refresh();
}

void AdvancedOptionsDialog::UpdateSlidersEnabled() {
    const bool enabled = (ctx_copy.flags & UIContext::_UI_CONTEXT_FLAG__HUD_COLORING) != 0;
    for (size_t i = 0; i < 3; ++i) {
        colorSliders[i]->Enable(enabled);
        colorSliderLabels[i]->Enable(enabled);
    }
}
