#include "ui/widgets/body/dialog/MusicDialog.hpp"
#include "ui/shared/Layout.hpp"

wxBEGIN_EVENT_TABLE(MusicOptionsDialog, wxFrame) EVT_CLOSE(MusicOptionsDialog::OnClose) wxEND_EVENT_TABLE()

    const std::string MusicOptionsDialog::_MUSIC_OPTIONS_DIALOG_TITLE_STR = "Music configuration options";
const std::string MusicOptionsDialog::_MUSIC_OPTIONS_GRID_NAME_STR = "Music Pool:";
const std::string MusicOptionsDialog::_MUSIC_OPTIONS_GRID_COLUMN_LABEL_STR = "Drop";
const std::string MusicOptionsDialog::_MUSIC_OPTIONS_GRID_TIP_STR =
    "Music Pool Table for randomization\n\nToggle checkboxes to include/exclude certain music options for "
    "randomization.\n - \"Drop\" column defines if this music will be included or excluded from "
    "rolling.\n\nNOTE: all tracks are divided into 4 groups: PEACEFUL, EXPLORING, INTENSE and FIGHT. If a "
    "catergory is not fully unchecked, than it will roll a track only from the same caterogy, meaning all "
    "fight musics will be substituted with only fight musics. However, if all fight musics are disabled, "
    "than the track will be chosen among all the available options in any category.";
const std::vector<MusicOptionsDialog::MusicOption> MusicOptionsDialog::_MUSIC_OPTIONS_GRID_ITEMS{
    MusicOptionsDialog::MusicOption{"cafeteria jazz (outsize)", 0x05, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__CAFETERIA_JAZZ_OUTSIDE},
    MusicOptionsDialog::MusicOption{"metamorphosis (stranger boss fight)", 0x06, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__STRANGER_BOSS_MUSIC},
    MusicOptionsDialog::MusicOption{"cafeteria jazz (inside)", 0x09, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__CAFETERIA_JAZZ_OUTSIDE},
    MusicOptionsDialog::MusicOption{"winning no.9", 0x0c, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__WINNING_NO_9},
    MusicOptionsDialog::MusicOption{"M.I.S.T. (garage)", 0x11, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__MIST_GARAGE},
    MusicOptionsDialog::MusicOption{"no.9 cutscene", 0x12, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__NO9_CUT_SCENE},
    MusicOptionsDialog::MusicOption{"weird man (no.9 fight)", 0x14, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__WEIRD_MAN},
    MusicOptionsDialog::MusicOption{"ambush! (akropolis fight)", 0x15, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__AMBUSH},
    MusicOptionsDialog::MusicOption{"hunt in dryfield (dryfield day fight)", 0x16, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__HUNT_IN_DRYFIELD},
    MusicOptionsDialog::MusicOption{"stealth assault (dryfield night fight)", 0x17, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__FIGHT_1},
    MusicOptionsDialog::MusicOption{
        "the first encounter (stranger scene)", 0x1d, true,
        UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__FIRST_STRANGER_SCENE_EATING_CORPSE},
    MusicOptionsDialog::MusicOption{"aya again (akropolis approach scene)", 0x1f, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_APPROACH_SCENE},
    MusicOptionsDialog::MusicOption{"don't move (elevator scene)", 0x20, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__GO_OUT_OF_ACROPOLIS_ELEVATOR},
    MusicOptionsDialog::MusicOption{
        "nightmare in the battlefield (first enetering square)", 0x21, true,
        UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_ENTERING_SQUARE},
    MusicOptionsDialog::MusicOption{"deadly calm (akropolis peace)", 0x22, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_2},
    MusicOptionsDialog::MusicOption{"watch out (akropolis peace 2)", 0x23, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_3},
    MusicOptionsDialog::MusicOption{"watch out! (akropolis peace 3)", 0x24, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_4},
    MusicOptionsDialog::MusicOption{"returning to the base (Baldwin cabinet)", 0x25, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__RETURNING_TO_MIST},
    MusicOptionsDialog::MusicOption{"do something! (akropolis roof peace)", 0x26, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__ACROPOLIS_PEACE_5},
    MusicOptionsDialog::MusicOption{"ghost town (dryfield day peace)", 0x29, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_1},
    MusicOptionsDialog::MusicOption{
        "Douglas' blues (meeting Doug)", 0x2a, true,
        UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_2_MEETING_DOUGH},
    MusicOptionsDialog::MusicOption{"vagrants (bar music)", 0x2b, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_BAR_MUSIC},
    MusicOptionsDialog::MusicOption{"water tower", 0x2c, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x2c},
    MusicOptionsDialog::MusicOption{"hiding place (Doug's truck)", 0x2d, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_3},
    MusicOptionsDialog::MusicOption{"dryfield (dryfield day peace 2)", 0x2e, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__DRYFIELD_DAY_PEACE_4},
    MusicOptionsDialog::MusicOption{"the bottom of the well", 0x2f, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x2f},
    MusicOptionsDialog::MusicOption{"stealth assault", 0x30, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x30},
    MusicOptionsDialog::MusicOption{"heaven-sent killer", 0x31, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x31},
    MusicOptionsDialog::MusicOption{"from dusk til down (dryfield night peace)", 0x37, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x37},
    MusicOptionsDialog::MusicOption{"gigantic burner (burner fight)", 0x38, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x37},
    MusicOptionsDialog::MusicOption{"dark field (dryfield night Doug)", 0x39, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x39},
    MusicOptionsDialog::MusicOption{"Douglas' grief", 0x3a, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3a},
    MusicOptionsDialog::MusicOption{"voice of the mitochondria", 0x3b, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3b},
    MusicOptionsDialog::MusicOption{"pick up the gauntlet (chaser boss fight)", 0x3c, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3c},
    MusicOptionsDialog::MusicOption{"abandoned mine (mines peace)", 0x3d, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3d},
    MusicOptionsDialog::MusicOption{"chaser boss stand up???", 0x3e, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3e},
    MusicOptionsDialog::MusicOption{"breeding room sounds", 0x3f, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x3f},
    MusicOptionsDialog::MusicOption{"into the shelter", 0x41, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x41},
    MusicOptionsDialog::MusicOption{"crawling waste emperor (glutton boss fight)", 0x42, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x42},
    MusicOptionsDialog::MusicOption{"hold your breath (gas puzzle music)", 0x43, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x43},
    MusicOptionsDialog::MusicOption{"wipe out the creatures (shelter fight music 1)", 0x44, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x44},
    MusicOptionsDialog::MusicOption{"stalker (brain stinger fight)", 0x45, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x45},
    MusicOptionsDialog::MusicOption{"battle on the waterside (shelter sewer fight music)", 0x46, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x46},
    MusicOptionsDialog::MusicOption{"inner most part of the shelter", 0x47, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x47},
    MusicOptionsDialog::MusicOption{"0x48 ???", 0x48, true, UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x48},
    MusicOptionsDialog::MusicOption{"mitochondria reactor", 0x49, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x49},
    MusicOptionsDialog::MusicOption{"chase (glutton stage 2 fight)", 0x4a, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4a},
    MusicOptionsDialog::MusicOption{"passing through the sewer you'll find...", 0x4b, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4b},
    MusicOptionsDialog::MusicOption{"man made nature (entering neo ark)", 0x4c, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4c},
    MusicOptionsDialog::MusicOption{"ark (neo ark peace)", 0x4d, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4d},
    MusicOptionsDialog::MusicOption{"fool's paradise (neo ark peace 2)", 0x4e, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4e},
    // MusicOptionsDialog::MusicOption{
    //     "mitochondria reactor",
    //     0x4f,
    //     true,
    //     UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x4e
    // },
    MusicOptionsDialog::MusicOption{"sigh of relief", 0x50, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__SIGH_OF_RELIEF},
    MusicOptionsDialog::MusicOption{"negative heritage (military music)", 0x51, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x51},
    MusicOptionsDialog::MusicOption{"golem soldiers (golems after neo ark)", 0x52, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x52},
    MusicOptionsDialog::MusicOption{"killing field", 0x53, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x53},
    MusicOptionsDialog::MusicOption{"cruelty of eve's fate (eve room)", 0x55, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__CRUELTY_OF_EVES_FATE},
    MusicOptionsDialog::MusicOption{"prestige of a nation (president's music)", 0x56, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x56},
    MusicOptionsDialog::MusicOption{"intrusion (later shelter peace)", 0x59, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x59},
    MusicOptionsDialog::MusicOption{"pod room", 0x5a, true, UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5a},
    MusicOptionsDialog::MusicOption{"brace yourself (before brahman intense)", 0x5d, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5d},
    MusicOptionsDialog::MusicOption{"brahman", 0x5e, true, UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5e},
    MusicOptionsDialog::MusicOption{"distorted evolution (eve fight)", 0x5f, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x5f},
    MusicOptionsDialog::MusicOption{"logic of superpower (president music 2)", 0x60, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x60},
    MusicOptionsDialog::MusicOption{"aya's diary", 0x61, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x61},
    MusicOptionsDialog::MusicOption{"game over music", 0x62, true,
                                    UIContext::_UI_CONTEXT_MUSIC_OPTIONS_POOL__0x62},
};

MusicOptionsDialog::MusicOptionsDialog(std::shared_ptr<UIContext> _ctx, Header *_header)
    : wxFrame(NULL, wxID_ANY, _MUSIC_OPTIONS_DIALOG_TITLE_STR, wxDefaultPosition,
              wxSize(_MUSIC_OPTIONS_DIALOG_WIDTH, _MUSIC_OPTIONS_DIALOG_HEIGHT)),
      ctx(_ctx), header(_header) {
    panel = new wxPanel(this, wxID_ANY);
    dialog_sizer = new wxBoxSizer(wxVERTICAL);
    dialog_inner_sizer = new wxBoxSizer(wxVERTICAL);

    panel->SetSizer(dialog_sizer);
    dialog_sizer->Add(dialog_inner_sizer, 1, wxEXPAND | wxALL, 25);

    ctx_copy = UIContext(0, ctx->logger);
    ctx_copy.music_pool = _ctx->music_pool;

    DrawMusicGrid();
    ui::fitContent(this, panel);
}

void MusicOptionsDialog::DrawMusicGrid() {
    music_scrolled_window =
        new wxScrolledWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);

    wxBoxSizer *music_pool_label_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *music_pool_label = new wxStaticText(panel, wxID_ANY, _MUSIC_OPTIONS_GRID_NAME_STR);
    music_pool_label_sizer->Add(music_pool_label, 1, wxALL, 0);

    wxStaticText *string_tip = new wxStaticText(panel, wxID_ANY, wxT("[?]"));
    string_tip->SetForegroundColour(*wxBLUE);
    string_tip->SetToolTip(_MUSIC_OPTIONS_GRID_TIP_STR);
    music_pool_label_sizer->AddSpacer(10);
    music_pool_label_sizer->Add(string_tip, 1, wxALL);

    music_grid_items = new wxGrid(music_scrolled_window, wxID_ANY);
    music_grid_items->CreateGrid(_MUSIC_OPTIONS_GRID_ITEMS.size(), 2);
    music_grid_items->SetMaxSize(wxSize(_MUSIC_OPTIONS_POOL_GRID_WIDTH, _MUSIC_OPTIONS_POOL_GRID_HEIGHT));
    music_grid_items->SetMinSize(wxSize(_MUSIC_OPTIONS_POOL_GRID_WIDTH, _MUSIC_OPTIONS_POOL_GRID_HEIGHT));

    music_grid_items->SetColLabelValue(0, _MUSIC_OPTIONS_GRID_COLUMN_LABEL_STR);

    for (size_t i = 0; i < _MUSIC_OPTIONS_GRID_ITEMS.size(); i++) {
        music_grid_items->SetRowLabelValue(i, _MUSIC_OPTIONS_GRID_ITEMS[i].name);

        music_grid_items->SetCellRenderer(i, 0, new wxGridCellBoolRenderer());
        music_grid_items->SetCellEditor(i, 0, new wxGridCellBoolEditor());
        music_grid_items->SetCellValue(i, 0,
                                       (_MUSIC_OPTIONS_GRID_ITEMS[i].flag & ctx_copy.music_pool) ? "1" : "");
        music_grid_items->SetCellAlignment(i, 0, wxALIGN_CENTER, wxALIGN_CENTER);

        music_grid_items->SetCellValue(i, 1,
                                       wxString::Format(wxT("%llu"), _MUSIC_OPTIONS_GRID_ITEMS[i].flag));
    }

    music_grid_items->SetRowLabelAlignment(wxALIGN_LEFT, wxALIGN_CENTER);
    music_grid_items->SetRowLabelSize(_MUSIC_OPTIONS_POOL_ROW_WIDTH);
    music_grid_items->SetColSize(0, _MUSIC_OPTIONS_POOL_COLUMN_WIDTH);
    music_grid_items->HideCol(1);
    music_grid_items->DisableDragColSize();
    music_grid_items->DisableDragRowSize();

    music_grid_items->Bind(wxEVT_GRID_CELL_LEFT_CLICK, &MusicOptionsDialog::OnCellLeftClick, this);

    wxBoxSizer *grid_sizer = new wxBoxSizer(wxVERTICAL);
    grid_sizer->Add(music_grid_items, 1, wxALL | wxEXPAND, 0);
    music_scrolled_window->SetSizer(grid_sizer);

    dialog_inner_sizer->Add(music_pool_label_sizer, 0, wxALL, 0);
    dialog_inner_sizer->Add(music_scrolled_window, 1, wxALL, 0);

    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *accept_button = new wxButton(panel, wxID_OK, wxT("Apply"));
    accept_button->Bind(wxEVT_BUTTON, &MusicOptionsDialog::OnApplyEvent, this);
    wxButton *cancel_button = new wxButton(panel, wxID_OK, wxT("Cancel"));
    cancel_button->Bind(wxEVT_BUTTON, &MusicOptionsDialog::OnCancelEvent, this);
    wxButton *default_button = new wxButton(panel, wxID_OK, wxT("Default"));
    default_button->Bind(wxEVT_BUTTON, &MusicOptionsDialog::OnDefaultEvent, this);
    buttons_sizer->Add(accept_button, 1, wxALL, 0);
    buttons_sizer->AddSpacer(15);
    buttons_sizer->Add(cancel_button, 1, wxALL, 0);
    buttons_sizer->AddSpacer(15);
    buttons_sizer->Add(default_button, 1, wxALL, 0);

    dialog_inner_sizer->AddSpacer(15);
    dialog_inner_sizer->Add(buttons_sizer, 0, wxALL, 0);
}

void MusicOptionsDialog::OnCellLeftClick(wxGridEvent &event) {
    const int row = event.GetRow();
    const int col = event.GetCol();
    if (col == 0) {
        music_grid_items->SaveEditControlValue();
        music_grid_items->HideCellEditControl();
        music_grid_items->SetGridCursor(row, col);
        const bool checked = music_grid_items->GetCellValue(row, col) == "1";
        music_grid_items->SetCellValue(row, col, checked ? "" : "1");
        return;
    }
    event.Skip();
}

void MusicOptionsDialog::OnApplyEvent(wxCommandEvent &event) {
    (void)event;
    music_grid_items->SaveEditControlValue();
    music_grid_items->HideCellEditControl();
    for (size_t row = 0; row < _MUSIC_OPTIONS_GRID_ITEMS.size(); ++row) {
        const uint64_t mask = _MUSIC_OPTIONS_GRID_ITEMS[row].flag;
        ctx_copy.music_pool &= ~mask;
        if (music_grid_items->GetCellValue(row, 0) == "1")
            ctx_copy.music_pool |= mask;
    }
    ctx->music_pool = ctx_copy.music_pool;
    header->redrawContext();
    Destroy();
}

void MusicOptionsDialog::OnCancelEvent(wxCommandEvent &event) {
    (void)event;
    Destroy();
}

void MusicOptionsDialog::OnDefaultEvent(wxCommandEvent &event) {
    (void)event;

    for (size_t i = 0; i < _MUSIC_OPTIONS_GRID_ITEMS.size(); i++) {
        music_grid_items->SetCellValue(i, 0, "1");
        ctx_copy.music_pool |= _MUSIC_OPTIONS_GRID_ITEMS[i].flag;
    }
}

void MusicOptionsDialog::OnClose(wxCloseEvent &event) {
    (void)event;
    Destroy();
}