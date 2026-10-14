#include "ui/widgets/body/dialog/EnemiesDialog.hpp"
#include "ui/shared/Layout.hpp"

wxBEGIN_EVENT_TABLE(EnemiesOptionsDialog, wxFrame) EVT_CLOSE(EnemiesOptionsDialog::OnClose)
    wxEND_EVENT_TABLE()

        EnemiesOptionsDialog::EnemiesOptionsDialog(std::shared_ptr<UIContext> _ctx, Header *_header)
    : wxFrame(NULL, wxID_ANY, wxT(_ENEMIES_OPTIONS_DIALOG_TITLE_STR), wxDefaultPosition,
              wxSize(ENEMIES_OPTIONS_DIALOG_SIZE_WIDTH, ENEMIES_OPTIONS_DIALOG_SIZE_HEIGHT)),
      ctx(_ctx), header(_header) {
    enemy_additional_options = {
        EnemyAdditionalOption{.label = _ENEMIES_HEALTH_MULT_STR,
                              .tip = _ENEMIES_HEALTH_MULT_TIP_STR,
                              .input_edit_handler = std::bind(&EnemiesOptionsDialog::OnEnemyHealthMult, this,
                                                              std::placeholders::_1)},
        EnemyAdditionalOption{.label = _ENEMIES_DAMAGE_MULT_STR,
                              .tip = _ENEMIES_DAMAGE_MULT_TIP_STR,
                              .input_edit_handler = std::bind(&EnemiesOptionsDialog::OnEnemyDamageMult, this,
                                                              std::placeholders::_1)},
        EnemyAdditionalOption{.label = _BOSSES_HEALTH_MULT_STR,
                              .tip = _BOSSES_HEALTH_MULT_TIP_STR,
                              .input_edit_handler = std::bind(&EnemiesOptionsDialog::OnBossHealthMult, this,
                                                              std::placeholders::_1)},
        EnemyAdditionalOption{.label = _BOSSES_DAMAGE_MULT_STR,
                              .tip = _BOSSES_DAMAGE_MULT_TIP_STR,
                              .input_edit_handler = std::bind(&EnemiesOptionsDialog::OnBossDamageMult, this,
                                                              std::placeholders::_1)}};

    panel = new wxPanel(this, wxID_ANY);
    dialog_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *dialog_inner_sizer = new wxBoxSizer(wxHORIZONTAL);
    enemy_pool_sizer = new wxBoxSizer(wxVERTICAL);
    enemy_options_sizer = new wxBoxSizer(wxVERTICAL);
    dialog_inner_sizer->Add(enemy_pool_sizer, 1, wxALL, 0);
    dialog_inner_sizer->Add(enemy_options_sizer, 1, wxALL, 0);
    panel->SetSizer(dialog_sizer);
    dialog_sizer->Add(dialog_inner_sizer, 1, wxALL, 25);
    ctx_copy = UIContext(0, ctx->logger);
    ctx_copy.enemies_pool = _ctx->enemies_pool;
    std::copy(std::begin(_ctx->enemies_mults), std::end(_ctx->enemies_mults),
              std::begin(ctx_copy.enemies_mults));
    drawEnemiesScrollView();
    drawEnemiesOptions();
    ui::fitContent(this, panel);
}

void EnemiesOptionsDialog::OnClose(wxCloseEvent &event) {
    (void)event;
    Destroy();
}

void EnemiesOptionsDialog::drawEnemiesScrollView() {
    wxScrolledWindow *scrolled_window =
        new wxScrolledWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);

    wxBoxSizer *enemy_pool_label_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *enemy_pool_label = new wxStaticText(panel, wxID_ANY, _ENEMIES_POOL_LABEL);
    enemy_pool_label_sizer->Add(enemy_pool_label, 1, wxALL, 0);

    wxStaticText *string_tip = new wxStaticText(panel, wxID_ANY, wxT("[?]"));
    string_tip->SetForegroundColour(*wxBLUE);
    string_tip->SetToolTip(_ENEMIES_POOL_HINT);
    enemy_pool_label_sizer->AddSpacer(10);
    enemy_pool_label_sizer->Add(string_tip, 1, wxALL);
    enemy_pool_sizer->Add(enemy_pool_label_sizer, 0, wxALL, 0);

    enemies_pool_grid = new wxGrid(scrolled_window, wxID_ANY);
    enemies_pool_grid->CreateGrid(enemy_pool_table_records.size(), 2);
    enemies_pool_grid->SetMaxSize(wxSize(ENEMIES_OPTIONS_POOL_SIZE_WIDTH, ENEMIES_OPTIONS_POOL_SIZE_HEIGHT));
    enemies_pool_grid->SetMinSize(wxSize(ENEMIES_OPTIONS_POOL_SIZE_WIDTH, ENEMIES_OPTIONS_POOL_SIZE_HEIGHT));
    enemies_pool_grid->SetColLabelValue(1, wxT("FlagPos"));

    enemies_pool_grid->SetColLabelValue(0, _ENEMIES_POOL_TABLE_HEADER_1);

    for (size_t i = 0; i < enemy_pool_table_records.size(); i++) {
        enemies_pool_grid->SetRowLabelValue(i, enemy_pool_table_records[i].name);

        enemies_pool_grid->SetCellRenderer(i, 0, new wxGridCellBoolRenderer());
        enemies_pool_grid->SetCellEditor(i, 0, new wxGridCellBoolEditor());
        enemies_pool_grid->SetCellValue(
            i, 0, (ctx_copy.enemies_pool & enemy_pool_table_records[i].flag) ? "1" : "");
        enemies_pool_grid->SetCellAlignment(i, 0, wxALIGN_CENTER, wxALIGN_CENTER);

        enemies_pool_grid->SetCellValue(i, 1,
                                        wxString::Format(wxT("%llu"), enemy_pool_table_records[i].flag));
    }

    enemies_pool_grid->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
    enemies_pool_grid->SetRowLabelSize(ENEMIES_OPTIONS_POOL_LABEL_WIDTH);
    enemies_pool_grid->SetColSize(0, ENEMIES_OPTIONS_POOL_COLUMN_WIDTH);
    enemies_pool_grid->DisableDragColSize();
    enemies_pool_grid->DisableDragRowSize();
    enemies_pool_grid->HideCol(1);

    wxBoxSizer *grid_sizer = new wxBoxSizer(wxVERTICAL);
    grid_sizer->Add(enemies_pool_grid, 1, wxALL);
    scrolled_window->SetSizer(grid_sizer);

    enemy_pool_sizer->Add(scrolled_window, 1, wxALL, 0);

    enemies_pool_grid->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &EnemiesOptionsDialog::OnCellLeftClick, this);
}

void EnemiesOptionsDialog::drawEnemiesOptions() {
    wxBoxSizer *enemy_additiona_options_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *enemy_additiona_options_label =
        new wxStaticText(panel, wxID_ANY, _ENEMIES_ADDITIONAL_OPTIONS_STR);
    enemy_additiona_options_sizer->Add(enemy_additiona_options_label, 1, wxEXPAND, 0);

    enemy_options_sizer->Add(enemy_additiona_options_sizer, 0, wxEXPAND, 0);

    size_t index = 0;
    for (EnemyAdditionalOption &option : enemy_additional_options) {
        enemy_options_sizer->AddSpacer(15);
        wxBoxSizer *enemy_option_sizer = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText *enemy_option_label = new wxStaticText(panel, wxID_ANY, option.label);
        enemy_option_sizer->Add(enemy_option_label, 1, wxEXPAND, 0);

        wxStaticText *string_tip = new wxStaticText(panel, wxID_ANY, wxT("[?]"));
        string_tip->SetForegroundColour(*wxBLUE);
        string_tip->SetToolTip(option.tip);
        enemy_option_sizer->AddSpacer(10);
        enemy_option_sizer->Add(string_tip, 1, wxALL);

        wxTextCtrl *input = new wxTextCtrl(
            panel, wxID_ANY, wxString::Format(wxT("%.2lf"), ((double)ctx_copy.enemies_mults[index] / 100)),
            wxDefaultPosition, wxDefaultSize, 0, DoubleValidator());
        input->Bind(wxEVT_TEXT, option.input_edit_handler);
        option.input = input;

        enemy_options_sizer->Add(enemy_option_sizer, 0, wxEXPAND, 0);
        enemy_options_sizer->Add(input, 0, wxEXPAND, 0);
        index++;
    }

    enemy_options_sizer->AddSpacer(100);

    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *apply_button = new wxButton(panel, wxID_OK, wxT("Apply"));
    apply_button->Bind(wxEVT_BUTTON, &EnemiesOptionsDialog::OnApplyClick, this);
    buttons_sizer->Add(apply_button, 0, wxRIGHT, 5);
    wxButton *cancel_button = new wxButton(panel, wxID_OK, wxT("Cancel"));
    cancel_button->Bind(wxEVT_BUTTON, &EnemiesOptionsDialog::OnCancelClick, this);
    buttons_sizer->Add(cancel_button, 0, wxLEFT, 5);
    enemy_options_sizer->Add(buttons_sizer, 0, wxEXPAND, 5);

    enemy_options_sizer->AddSpacer(15);

    wxBoxSizer *additional_buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *default_button = new wxButton(panel, wxID_OK, wxT("Default"));
    default_button->Bind(wxEVT_BUTTON, &EnemiesOptionsDialog::OnDefaultClick, this);
    additional_buttons_sizer->Add(default_button, 0, wxLEFT, 0);
    enemy_options_sizer->Add(additional_buttons_sizer, 0, wxEXPAND, 5);
}

void EnemiesOptionsDialog::OnCellLeftClick(wxGridEvent &event) {
    const int row = event.GetRow();
    const int col = event.GetCol();
    if (col == 0) {
        enemies_pool_grid->SaveEditControlValue();
        enemies_pool_grid->HideCellEditControl();
        enemies_pool_grid->SetGridCursor(row, col);
        const bool checked = enemies_pool_grid->GetCellValue(row, col) == "1";
        enemies_pool_grid->SetCellValue(row, col, checked ? "" : "1");
        return;
    }
    event.Skip();
}

void EnemiesOptionsDialog::OnEnemyHealthMult(wxCommandEvent &event) {
    (void)event;
    wxString value_str = enemy_additional_options[0].input->GetValue();
    double double_value;
    value_str.ToDouble(&double_value);
    ctx_copy.enemies_mults[0] = (uint16_t)(double_value * 100);
}

void EnemiesOptionsDialog::OnEnemyDamageMult(wxCommandEvent &event) {
    (void)event;
    wxString value_str = enemy_additional_options[1].input->GetValue();
    double double_value;
    value_str.ToDouble(&double_value);
    ctx_copy.enemies_mults[1] = (uint16_t)(double_value * 100);
}

void EnemiesOptionsDialog::OnBossHealthMult(wxCommandEvent &event) {
    (void)event;
    wxString value_str = enemy_additional_options[2].input->GetValue();
    double double_value;
    value_str.ToDouble(&double_value);
    ctx_copy.enemies_mults[2] = (uint16_t)(double_value * 100);
}

void EnemiesOptionsDialog::OnBossDamageMult(wxCommandEvent &event) {
    (void)event;
    wxString value_str = enemy_additional_options[3].input->GetValue();
    double double_value;
    value_str.ToDouble(&double_value);
    ctx_copy.enemies_mults[3] = (uint16_t)(double_value * 100);
}

void EnemiesOptionsDialog::OnApplyClick(wxCommandEvent &event) {
    (void)event;
    enemies_pool_grid->SaveEditControlValue();
    enemies_pool_grid->HideCellEditControl();
    for (size_t row = 0; row < enemy_pool_table_records.size(); ++row) {
        const uint64_t mask = enemy_pool_table_records[row].flag;
        ctx_copy.enemies_pool &= ~mask;
        if (enemies_pool_grid->GetCellValue(row, 0) == "1")
            ctx_copy.enemies_pool |= mask;
    }
    ctx->enemies_pool = ctx_copy.enemies_pool;
    std::copy(std::begin(ctx_copy.enemies_mults), std::end(ctx_copy.enemies_mults),
              std::begin(ctx->enemies_mults));

    header->redrawContext();
    Destroy();
}

void EnemiesOptionsDialog::OnCancelClick(wxCommandEvent &event) {
    (void)event;
    Destroy();
}

void EnemiesOptionsDialog::OnDefaultClick(wxCommandEvent &event) {
    (void)event;

    for (size_t i = 0; i < enemy_pool_table_records.size(); i++) {
        ctx_copy.enemies_pool |= enemy_pool_table_records[i].flag;
        enemies_pool_grid->SetCellValue(i, 0, "1");
    }

    for (size_t i = 0; i < enemy_additional_options.size(); i++) {
        enemy_additional_options[i].input->SetValue("1.00");
        ctx_copy.enemies_mults[i] = 100;
    }
}
