#include "ui/widgets/body/dialog/ItemsDialog.hpp"
#include "ui/shared/Layout.hpp"

wxBEGIN_EVENT_TABLE(ItemsOptionsDialog, wxFrame) EVT_CLOSE(ItemsOptionsDialog::OnClose) wxEND_EVENT_TABLE()

    ItemsOptionsDialog::ItemsOptionsDialog(std::shared_ptr<UIContext> _ctx, Header *_header)
    : wxFrame(NULL, wxID_ANY, wxT(_ITEMS_OPTIONS_DIALOG_TITLE_STR), wxDefaultPosition,
              wxSize(ITEMS_OPTIONS_DIALOG_SIZE_WIDTH, ITEMS_OPTIONS_DIALOG_SIZE_HEIGHT)),
      ctx(_ctx), header(_header) {
    panel = new wxPanel(this, wxID_ANY);
    dialog_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *dialog_inner_sizer = new wxBoxSizer(wxHORIZONTAL);
    item_pool_sizer = new wxBoxSizer(wxVERTICAL);
    item_options_sizer = new wxBoxSizer(wxVERTICAL);
    dialog_inner_sizer->Add(item_pool_sizer, 1, wxALL, 0);
    dialog_inner_sizer->Add(item_options_sizer, 1, wxALL, 0);
    panel->SetSizer(dialog_sizer);
    dialog_sizer->Add(dialog_inner_sizer, 1, wxALL, 25);

    ctx_copy = UIContext(0, ctx->logger);
    std::copy(std::begin(_ctx->items_drop_pool), std::end(_ctx->items_drop_pool),
              std::begin(ctx_copy.items_drop_pool));
    std::copy(std::begin(_ctx->items_shop_pool), std::end(_ctx->items_shop_pool),
              std::begin(ctx_copy.items_shop_pool));
    std::copy(std::begin(_ctx->items_shop_costs), std::end(_ctx->items_shop_costs),
              std::begin(ctx_copy.items_shop_costs));
    std::copy(std::begin(_ctx->items_dist), std::end(_ctx->items_dist), std::begin(ctx_copy.items_dist));
    std::copy(std::begin(_ctx->items_start_options), std::end(_ctx->items_start_options),
              std::begin(ctx_copy.items_start_options));
    ctx_copy.items_shop_cost_mult = _ctx->items_shop_cost_mult;

    item_probability_categories = {ItemProbabilityCategory{
                                       .label = _ITEMS_PROB_HP_RECOVERY_STR,
                                       .tip = _ITEMS_PROB_HP_RECOVERY_TIP_STR,
                                   },
                                   ItemProbabilityCategory{
                                       .label = _ITEMS_PROB_MP_RECOVERY_STR,
                                       .tip = _ITEMS_PROB_MP_RECOVERY_TIP_STR,
                                   },
                                   ItemProbabilityCategory{
                                       .label = _ITEMS_PROB_AMMO_STR,
                                       .tip = _ITEMS_PROB_AMMO_TIP_STR,
                                   },
                                   ItemProbabilityCategory{
                                       .label = _ITEMS_PROB_MISC_STR,
                                       .tip = _ITEMS_PROB_MISC_TIP_STR,
                                   },
                                   ItemProbabilityCategory{
                                       .label = _ITEMS_PROB_UNIQUE_STR,
                                       .tip = _ITEMS_PROB_UNIQUE_TIP_STR,
                                   }};

    starting_item_options = {
        StartinItemOption{_ITEMS_STARTING_WEAPON_BOX_STR,
                          _ITEMS_STARTING_WEAPON_BOX_TIP_STR,
                          [this](wxCommandEvent &event) {
                              (void)event;
                              (void)this;
                          },
                          {
                              _ITEMS_LABEL_WEAPON_RANDOM_STARTING_WEAPON,
                              _ITEMS_LABEL_WEAPON_DEFAULT,
                              _ITEMS_LABEL_WEAPON_FULL_RANDOM,
                              _ITEMS_LABEL_WEAPON_TONFA_ONLY_START,
                              _ITEMS_LABEL_WEAPON_M93R,
                              _ITEMS_LABEL_WEAPON_M950,
                              _ITEMS_LABEL_WEAPON_P08,
                              _ITEMS_LABEL_WEAPON_P229,
                              _ITEMS_LABEL_WEAPON_MONGOOSE,
                              _ITEMS_LABEL_WEAPON_GRENADE_PISTOL,
                              _ITEMS_LABEL_WEAPON_MM1,
                              _ITEMS_LABEL_WEAPON_PA3,
                              _ITEMS_LABEL_WEAPON_SP12,
                              _ITEMS_LABEL_WEAPON_AS12,
                              _ITEMS_LABEL_WEAPON_M4A1,
                              _ITEMS_LABEL_WEAPON_M249,
                              _ITEMS_LABEL_WEAPON_HYPERVELOCITY,
                              _ITEMS_LABEL_WEAPON_GUNBLADE,
                              _ITEMS_LABEL_WEAPON_MP5A5,
                              _ITEMS_LABEL_WEAPON_P08_SNAIL,
                              _ITEMS_LABEL_M4A1_1_CLIP,
                              _ITEMS_LABEL_M4A1_2_CLIPS,
                              _ITEMS_LABEL_M4A1_HAMMER,
                              _ITEMS_LABEL_M4A1_KNIFE,
                              _ITEMS_LABEL_M4A1_GRENADE,
                              _ITEMS_LABEL_M4A1_PYKE,
                              _ITEMS_LABEL_M4A1_JAVELIN,
                              _ITEMS_LABEL_MP5A5_1_CLIP,
                              _ITEMS_LABEL_MP5A5_2_CLIPS,
                          },
                          &starting_weapon_box},
        StartinItemOption{_ITEMS_STARTING_ARMOR_BOX_STR,
                          _ITEMS_STARTING_ARMOR_BOX_TIP_STR,
                          [this](wxCommandEvent &event) {
                              (void)event;
                              (void)this;
                          },
                          {
                              _ITEMS_LABEL_ARMOR_RANDOM_STARTING_ARMOR,
                              _ITEMS_LABEL_ARMOR_DEFAULT,
                              _ITEMS_LABEL_ARMOR_FULL_RANDOM_ARMOR,
                              _ITEMS_LABEL_ARMOR_LEATHER_JACKET,
                              _ITEMS_LABEL_ARMOR_TACTICAL_ARMOR,
                              _ITEMS_LABEL_ARMOR_COMBAT_ARMOR,
                              _ITEMS_LABEL_ARMOR_ASSAULT_SUIT,
                              _ITEMS_LABEL_ARMOR_PASGT_VEST,
                              _ITEMS_LABEL_ARMOR_TACTICAL_VEST,
                              _ITEMS_LABEL_ARMOR_EOD_SUIT,
                              _ITEMS_LABEL_ARMOR_TURTLE_VEST,
                              _ITEMS_LABEL_ARMOR_CHICKEN_PLATE,
                              _ITEMS_LABEL_ARMOR_NBC_SUIT,
                              _ITEMS_LABEL_ARMOR_PSYSUIT,
                              _ITEMS_LABEL_ARMOR_AYA_SPECIAL,
                              _ITEMS_LABEL_ARMOR_SHOULDER_HOLSTER,
                              _ITEMS_LABEL_ARMOR_MONK_ROBE,
                          },
                          &starting_armor_box},
        StartinItemOption{_ITEMS_DRYFIELD_ARMOR_BOX_STR,
                          _ITEMS_DRYFIELD_ARMOR_BOX_TIP_STR,
                          [this](wxCommandEvent &event) {
                              (void)event;
                              (void)this;
                          },
                          {
                              _ITEMS_LABEL_ARMOR_RANDOM_STARTING_ARMOR,
                              _ITEMS_LABEL_ARMOR_DEFAULT,
                              _ITEMS_LABEL_ARMOR_FULL_RANDOM_ARMOR,
                              _ITEMS_LABEL_ARMOR_LEATHER_JACKET,
                              _ITEMS_LABEL_ARMOR_TACTICAL_ARMOR,
                              _ITEMS_LABEL_ARMOR_COMBAT_ARMOR,
                              _ITEMS_LABEL_ARMOR_ASSAULT_SUIT,
                              _ITEMS_LABEL_ARMOR_PASGT_VEST,
                              _ITEMS_LABEL_ARMOR_TACTICAL_VEST,
                              _ITEMS_LABEL_ARMOR_EOD_SUIT,
                              _ITEMS_LABEL_ARMOR_TURTLE_VEST,
                              _ITEMS_LABEL_ARMOR_CHICKEN_PLATE,
                              _ITEMS_LABEL_ARMOR_NBC_SUIT,
                              _ITEMS_LABEL_ARMOR_PSYSUIT,
                              _ITEMS_LABEL_ARMOR_AYA_SPECIAL,
                              _ITEMS_LABEL_ARMOR_SHOULDER_HOLSTER,
                              _ITEMS_LABEL_ARMOR_MONK_ROBE,
                          },
                          &dryfield_armor_box},
    };

    DrawItemsAdditionalOptionsHeader();
    DrawItemsScrollView();
    DrawItemsProbabilities();
    DrawItemsStartingOptions();
    DrawItemsAcceptAndCancelButtons();
    ui::fitContent(this, panel);
}

void ItemsOptionsDialog::DrawItemsScrollView() {
    wxScrolledWindow *scrolled_window =
        new wxScrolledWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);

    wxBoxSizer *item_pool_label_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *enemy_pool_label = new wxStaticText(panel, wxID_ANY, _ITEMS_POOL_LABEL);
    item_pool_label_sizer->Add(enemy_pool_label, 1, wxALL, 0);

    wxStaticText *string_tip = new wxStaticText(panel, wxID_ANY, wxT("[?]"));
    string_tip->SetForegroundColour(*wxBLUE);
    string_tip->SetToolTip(_ITEMS_POOL_HINT);
    item_pool_label_sizer->AddSpacer(10);
    item_pool_label_sizer->Add(string_tip, 1, wxALL);
    item_pool_sizer->Add(item_pool_label_sizer, 0, wxALL, 0);

    items_pool_grid = new wxGrid(scrolled_window, wxID_ANY);
    items_pool_grid->CreateGrid(item_pool_table_records.size(), 5);
    items_pool_grid->SetMaxSize(wxSize(ITEMS_OPTIONS_POOL_SIZE_WIDTH, ITEMS_OPTIONS_POOL_SIZE_HEIGHT));
    items_pool_grid->SetMinSize(wxSize(ITEMS_OPTIONS_POOL_SIZE_WIDTH, ITEMS_OPTIONS_POOL_SIZE_HEIGHT));

    items_pool_grid->SetColLabelValue(0, _ITEMS_POOL_TABLE_HEADER_1);
    items_pool_grid->SetColLabelValue(1, _ITEMS_POOL_TABLE_HEADER_2);
    items_pool_grid->SetColLabelValue(2, _ITEMS_POOL_TABLE_HEADER_3);
    items_pool_grid->SetColLabelValue(3, "FlagPosLow");
    items_pool_grid->SetColLabelValue(4, "FlagPosHigh");

    for (size_t i = 0; i < item_pool_table_records.size(); i++) {
        items_pool_grid->SetRowLabelValue(i, item_pool_table_records[i].name);

        items_pool_grid->SetCellRenderer(i, 0, new wxGridCellBoolRenderer());
        items_pool_grid->SetCellEditor(i, 0, new wxGridCellBoolEditor());
        items_pool_grid->SetCellValue(i, 0,
                                      ((ctx_copy.items_drop_pool[0] & item_pool_table_records[i].flag[0]) ||
                                       (ctx_copy.items_drop_pool[1] & item_pool_table_records[i].flag[1]))
                                          ? "1"
                                          : "");
        items_pool_grid->SetCellAlignment(i, 0, wxALIGN_CENTER, wxALIGN_CENTER);

        items_pool_grid->SetCellRenderer(i, 1, new wxGridCellBoolRenderer());
        items_pool_grid->SetCellEditor(i, 1, new wxGridCellBoolEditor());
        items_pool_grid->SetCellValue(i, 1,
                                      ((ctx_copy.items_shop_pool[0] & item_pool_table_records[i].flag[0]) ||
                                       (ctx_copy.items_shop_pool[1] & item_pool_table_records[i].flag[1]))
                                          ? "1"
                                          : "");
        items_pool_grid->SetCellAlignment(i, 1, wxALIGN_CENTER, wxALIGN_CENTER);

        items_pool_grid->SetCellRenderer(i, 2, new wxGridCellNumberRenderer());
        items_pool_grid->SetCellEditor(i, 2, new wxGridCellNumberEditor());
        items_pool_grid->SetCellValue(i, 2, wxString::Format(wxT("%d"), ctx_copy.items_shop_costs[i]));
        items_pool_grid->SetCellAlignment(i, 2, wxALIGN_CENTER, wxALIGN_CENTER);

        items_pool_grid->SetCellValue(i, 3,
                                      wxString::Format(wxT("%llu"), item_pool_table_records[i].flag[0]));
        items_pool_grid->SetCellValue(i, 4,
                                      wxString::Format(wxT("%llu"), item_pool_table_records[i].flag[1]));
    }

    items_pool_grid->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
    items_pool_grid->SetRowLabelSize(ITEMS_OPTIONS_POOL_LABEL_WIDTH);
    items_pool_grid->SetColSize(0, ITEMS_OPTIONS_POOL_COLUMN_WIDTH);
    items_pool_grid->SetColSize(1, ITEMS_OPTIONS_POOL_COLUMN_WIDTH);
    items_pool_grid->SetColSize(2, ITEMS_OPTIONS_POOL_COLUMN_WIDTH);
    items_pool_grid->HideCol(3);
    items_pool_grid->HideCol(4);
    items_pool_grid->DisableDragColSize();
    items_pool_grid->DisableDragRowSize();

    wxBoxSizer *grid_sizer = new wxBoxSizer(wxVERTICAL);
    grid_sizer->Add(items_pool_grid, 1, wxALL);
    scrolled_window->SetSizer(grid_sizer);

    item_pool_sizer->Add(scrolled_window, 1, wxALL, 0);

    items_pool_grid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &ItemsOptionsDialog::OnCellLeftClick, this);
}

void ItemsOptionsDialog::DrawItemsAdditionalOptionsHeader() {
    item_additional_options_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *item_additional_options_label =
        new wxStaticText(panel, wxID_ANY, _ITEMS_ADDITIONAL_OPTIONS_STR);
    item_additional_options_sizer->Add(item_additional_options_label, 1, wxEXPAND, 0);

    item_options_sizer->Add(item_additional_options_sizer, 0, wxEXPAND, 0);

    wxBoxSizer *shop_cost_mult_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *shop_cost_mult_label = new wxStaticText(panel, wxID_ANY, _ITEMS_SHOP_COST_MULT_STR);
    shop_cost_mult_sizer->Add(shop_cost_mult_label, 1, wxEXPAND, 0);

    wxStaticText *string_cost_mult_tip = new wxStaticText(panel, wxID_ANY, wxT("[?]"));
    string_cost_mult_tip->SetForegroundColour(*wxBLUE);
    string_cost_mult_tip->SetToolTip(_ITEMS_SHOP_COST_MULT_TIP_STR);
    shop_cost_mult_sizer->Add(string_cost_mult_tip, 1, wxALL);

    shop_cost_mult_input = new wxTextCtrl(
        panel, wxID_ANY, wxString::Format(wxT("%.2lf"), ((double)ctx_copy.items_shop_cost_mult / 100)));
    shop_cost_mult_input->Bind(wxEVT_TEXT, &ItemsOptionsDialog::OnShopCostMult, this);

    shop_cost_mult_sizer->Add(shop_cost_mult_input, 1, wxEXPAND, 0);
    item_options_sizer->AddSpacer(15);
    item_options_sizer->Add(shop_cost_mult_sizer, 0, wxEXPAND | wxALL, 0);
}

void ItemsOptionsDialog::DrawItemsProbabilities() {
    wxStaticBox *static_box = new wxStaticBox(panel, wxID_ANY, wxT(_ITEMS_PROBABILITY_LABEL_STR));
    wxStaticBoxSizer *static_box_probability_sizer = new wxStaticBoxSizer(static_box, wxVERTICAL);

    item_options_sizer->AddSpacer(15);
    item_options_sizer->Add(static_box_probability_sizer, 0, wxALL, 0);

    for (size_t i = 0; i < item_probability_categories.size(); i++) {
        static_box_probability_sizer->AddSpacer(15);

        wxBoxSizer *item_option_sizer = new wxBoxSizer(wxHORIZONTAL);

        wxStaticText *item_option_label =
            new wxStaticText(panel, wxID_ANY, item_probability_categories[i].label);
        item_option_sizer->Add(item_option_label, 1, wxEXPAND, 0);

        wxStaticText *string_tip = new wxStaticText(panel, wxID_ANY, wxT("[?]"));
        string_tip->SetForegroundColour(*wxBLUE);
        string_tip->SetToolTip(item_probability_categories[i].tip);
        item_option_sizer->Add(string_tip, 1, wxALL);

        double probValue = ((double)ctx_copy.items_dist[i] / 100.0);
        item_probability_categories[i].input =
            new wxTextCtrl(panel, wxID_ANY, wxString::Format(wxT("%.2lf"), probValue), wxDefaultPosition,
                           wxDefaultSize, wxTE_PROCESS_ENTER);

        // (Optional) Also keep the existing event binding if you still want
        // OnHPRecCostMult, OnMPRecCostMult, etc. to run.
        // item_probability_categories[i].input->Bind(wxEVT_TEXT,
        //     item_probability_categories[i].input_edit_handler);

        item_option_sizer->Add(item_probability_categories[i].input, 1, wxEXPAND, 0);

        static_box_probability_sizer->Add(item_option_sizer, 0, wxEXPAND, 0);

        item_probability_categories[i].input->Bind(wxEVT_KILL_FOCUS, [this, i](wxFocusEvent &evt) {
            this->OnProbabilityTextChanged(i);
            evt.Skip();
        });

        // BIND: On press Enter
        item_probability_categories[i].input->Bind(wxEVT_TEXT_ENTER, [this, i](wxCommandEvent &evt) {
            this->OnProbabilityTextChanged(i);
            evt.Skip();
        });
    }
}

void ItemsOptionsDialog::DrawItemsStartingOptions() {
    wxStaticBox *static_box = new wxStaticBox(panel, wxID_ANY, wxT(_ITEMS_STARTING_OPTIONS_STR));
    wxStaticBoxSizer *static_box_starting_options_sizer = new wxStaticBoxSizer(static_box, wxVERTICAL);

    item_options_sizer->AddSpacer(15);
    item_options_sizer->Add(static_box_starting_options_sizer, 0, wxALL, 0);

    size_t option_index = 0;
    for (const StartinItemOption &option : starting_item_options) {
        static_box_starting_options_sizer->AddSpacer(15);
        wxBoxSizer *starting_item_sizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText *starting_item_option_label = new wxStaticText(panel, wxID_ANY, option.label);
        starting_item_sizer->Add(starting_item_option_label, 1, wxEXPAND, 0);

        wxStaticText *string_tip = new wxStaticText(panel, wxID_ANY, wxT("[?]"));
        string_tip->SetForegroundColour(*wxBLUE);
        string_tip->SetToolTip(option.tip);
        starting_item_sizer->Add(string_tip, 1, wxALL);

        wxArrayString items;
        for (const std::string &item : option.options) {
            items.Add(item);
        }

        *(option.comboBox) =
            new wxComboBox(panel, wxID_ANY, option.options[ctx_copy.items_start_options[option_index]],
                           wxDefaultPosition, wxSize(180, -1), items, wxCB_READONLY);
        starting_item_sizer->Add(*(option.comboBox), 1, wxEXPAND, 0);

        static_box_starting_options_sizer->Add(starting_item_sizer, 0, wxEXPAND, 0);

        option_index++;
    }
}

void ItemsOptionsDialog::DrawItemsAcceptAndCancelButtons() {
    item_options_sizer->AddSpacer(50);

    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *apply_button = new wxButton(panel, wxID_OK, wxT("Apply"));
    apply_button->Bind(wxEVT_BUTTON, &ItemsOptionsDialog::OnApplyClick, this);
    buttons_sizer->Add(apply_button, 0, wxRIGHT, 5);
    wxButton *cancel_button = new wxButton(panel, wxID_OK, wxT("Cancel"));
    cancel_button->Bind(wxEVT_BUTTON, &ItemsOptionsDialog::OnCancelClick, this);
    buttons_sizer->Add(cancel_button, 0, wxCENTER, 5);
    wxButton *default_button = new wxButton(panel, wxID_OK, wxT("Default"));
    default_button->Bind(wxEVT_BUTTON, &ItemsOptionsDialog::OnDefaultClick, this);
    buttons_sizer->Add(default_button, 0, wxLEFT, 5);
    item_options_sizer->Add(buttons_sizer, 0, wxEXPAND, 5);
}

void ItemsOptionsDialog::OnClose(wxCloseEvent &event) {
    (void)event;
    Destroy();
}

void ItemsOptionsDialog::OnCellLeftClick(wxGridEvent &event) {
    const int row = event.GetRow();
    const int col = event.GetCol();
    if (col == 0 || col == 1) {
        // The grid is the source of truth, including keyboard edits. Do not
        // parse a 64-bit mask through wxAtol (32-bit long on Windows).
        items_pool_grid->SaveEditControlValue();
        items_pool_grid->HideCellEditControl();
        items_pool_grid->SetGridCursor(row, col);
        const bool checked = items_pool_grid->GetCellValue(row, col) == "1";
        items_pool_grid->SetCellValue(row, col, checked ? "" : "1");
        return;
    }
    event.Skip();
}

void ItemsOptionsDialog::OnShopCostMult(wxCommandEvent &event) {
    (void)event;
    wxString value_str = shop_cost_mult_input->GetValue();
    double double_value;
    value_str.ToDouble(&double_value);
    ctx_copy.items_shop_cost_mult = (uint16_t)(double_value * 100);
}

void ItemsOptionsDialog::OnApplyClick(wxCommandEvent &event) {
    (void)event;

    items_pool_grid->SaveEditControlValue();
    items_pool_grid->HideCellEditControl();
    OnProbabilityTextChanged(0);

    double totalSum = 0.0;
    for (const auto &prob : ctx_copy.items_dist) {
        totalSum += prob / 100.0;
    }

    if (std::abs(totalSum - 1.0) > 1e-6) {
        wxMessageDialog errorDialog(
            this,
            wxString::Format(
                "Error: Probabilities do not sum to 1.0 (current sum: %.2f). Please adjust the values.",
                totalSum),
            "Invalid Probabilities", wxOK | wxICON_ERROR);
        errorDialog.ShowModal();
        return;
    }

    size_t numRows = items_pool_grid->GetNumberRows();
    for (size_t row = 0; row < numRows; ++row) {
        uint16_t shop_cost = wxAtol(items_pool_grid->GetCellValue(row, 2));
        ctx_copy.items_shop_costs[row] = shop_cost;
        for (size_t word = 0; word < 2; ++word) {
            const uint64_t mask = item_pool_table_records[row].flag[word];
            ctx_copy.items_drop_pool[word] &= ~mask;
            ctx_copy.items_shop_pool[word] &= ~mask;
            if (items_pool_grid->GetCellValue(row, 0) == "1")
                ctx_copy.items_drop_pool[word] |= mask;
            if (items_pool_grid->GetCellValue(row, 1) == "1")
                ctx_copy.items_shop_pool[word] |= mask;
        }
    }

    for (size_t i = 0; i < starting_item_options.size(); i++) {
        int selected_index = (*(starting_item_options[i].comboBox))->GetSelection();
        uint8_t selection = 0;
        if (selected_index != wxNOT_FOUND) {
            selection = (uint8_t)selected_index;
        }
        ctx_copy.items_start_options[i] = selection;
    }

    std::copy(std::begin(ctx_copy.items_drop_pool), std::end(ctx_copy.items_drop_pool),
              std::begin(ctx->items_drop_pool));
    std::copy(std::begin(ctx_copy.items_shop_pool), std::end(ctx_copy.items_shop_pool),
              std::begin(ctx->items_shop_pool));
    std::copy(std::begin(ctx_copy.items_shop_costs), std::end(ctx_copy.items_shop_costs),
              std::begin(ctx->items_shop_costs));
    std::copy(std::begin(ctx_copy.items_dist), std::end(ctx_copy.items_dist), std::begin(ctx->items_dist));
    std::copy(std::begin(ctx_copy.items_start_options), std::end(ctx_copy.items_start_options),
              std::begin(ctx->items_start_options));
    ctx->items_shop_cost_mult = ctx_copy.items_shop_cost_mult;

    header->redrawContext();
    Destroy();
}

void ItemsOptionsDialog::OnCancelClick(wxCommandEvent &event) {
    (void)event;
    Destroy();
}

void ItemsOptionsDialog::OnDefaultClick(wxCommandEvent &event) {
    (void)event;

    for (size_t i = 0; i < starting_item_options.size(); i++) {
        (*starting_item_options[i].comboBox)->SetSelection(0);
    }

    shop_cost_mult_input->SetValue(wxString::Format(wxT("%.2lf"), (1.00f)));
    ctx_copy.items_shop_cost_mult = (uint16_t)100;
    double deafult_dists[5] = {0.16, 0.14, 0.48, 0.17, 0.05};

    for (size_t i = 0; i < item_probability_categories.size(); i++) {
        ctx_copy.items_dist[i] = (uint8_t)(deafult_dists[i] * 100);
        item_probability_categories[i].input->SetValue(wxString::Format(wxT("%.2lf"), deafult_dists[i]));
    }

    for (size_t i = 0; i < item_pool_table_records.size(); i++) {
        items_pool_grid->SetCellValue(i, 0, item_pool_table_records[i].drop ? "1" : "");
        items_pool_grid->SetCellValue(i, 1, item_pool_table_records[i].shop ? "1" : "");
        items_pool_grid->SetCellValue(i, 2, wxString::Format(wxT("%d"), item_pool_table_records[i].cost));
    }
}

void ItemsOptionsDialog::OnProbabilityTextChanged(size_t /*idx*/) {
    double currentVals[5];

    for (size_t i = 0; i < item_probability_categories.size(); i++) {
        double val = 0.0;
        item_probability_categories[i].input->GetValue().ToDouble(&val);

        currentVals[i] = val;
    }

    for (size_t i = 0; i < item_probability_categories.size(); i++) {
        ctx_copy.items_dist[i] = static_cast<uint8_t>(std::lround(currentVals[i] * 100));
    }
}
