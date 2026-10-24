#include "ui/widgets/body/GeneralOptions.hpp"

wxBEGIN_EVENT_TABLE(GeneralOptions, wxPanel) EVT_THREAD(1, GeneralOptions::onThreadUpdate)
    EVT_THREAD(2, GeneralOptions::onThreadWorkDone) wxEND_EVENT_TABLE()

        GeneralOptions::GeneralOptions(wxWindow *parent, std::shared_ptr<UIContext> _ctx, Header *_header)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxTRANSPARENT_WINDOW),
      ctx(_ctx), header(_header) {
    SetMinSize(FromDIP(wxSize(GENERAL_OPTIONS_SIZE_WIDTH, -1)));

    ctx->addListener([this]() { updateCheckboxes(); });

    randomization_options = {
        {GENERAL_OPTIONS_ENEMIES,
         RandomizationOption{.checkbox_label = _STR_ENEMIES_CHECKBOX_LABEL,
                             .button_label = _STR_ENEMIES_BUTTON_LABEL,
                             .checkbox_click_handeler = std::bind(&GeneralOptions::onEnemiesCheckboxClick,
                                                                  this, std::placeholders::_1),
                             .button_click_handler = std::bind(&GeneralOptions::onEnemiesButtonClick, this,
                                                               std::placeholders::_1)}},
        {GENERAL_OPTIONS_ITEMS,
         RandomizationOption{.checkbox_label = _STR_ITEMS_CHECKBOX_LABEL,
                             .button_label = _STR_ITEMS_BUTTON_LABEL,
                             .checkbox_click_handeler = std::bind(&GeneralOptions::onItemsCheckboxClick, this,
                                                                  std::placeholders::_1),
                             .button_click_handler = std::bind(&GeneralOptions::onItemsButtonClick, this,
                                                               std::placeholders::_1)}},
        {GENERAL_OPTIONS_MAGIC,
         RandomizationOption{.checkbox_label = _STR_MAGIC_CHECKBOX_LABEL,
                             .button_label = _STR_MAGIC_BUTTON_LABEL,
                             .checkbox_click_handeler = std::bind(&GeneralOptions::onMagicCheckboxClick, this,
                                                                  std::placeholders::_1),
                             .button_click_handler = std::bind(&GeneralOptions::onMagicButtonClick, this,
                                                               std::placeholders::_1)}},
        {GENERAL_OPTIONS_MUSIC,
         RandomizationOption{.checkbox_label = _STR_MUSIC_CHECKBOX_LABEL,
                             .button_label = _STR_MUSIC_BUTTON_LABEL,
                             .checkbox_click_handeler = std::bind(&GeneralOptions::onMusicCheckboxClick, this,
                                                                  std::placeholders::_1),
                             .button_click_handler = std::bind(&GeneralOptions::onMusicButtonClick, this,
                                                               std::placeholders::_1)}}};

    general_options_sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(general_options_sizer);

    drawRandomizationOptions();
    drawRandomizationButtons();
    updateCheckboxes();
}

void GeneralOptions::drawRandomizationOptions() {
    wxStaticBox *static_box = new wxStaticBox(this, wxID_ANY, wxT(_STR_GENERAL_OPTIONS_STR));
    wxStaticBoxSizer *static_box_sizer = new wxStaticBoxSizer(static_box, wxVERTICAL);
    size_t i = 0;
    for (const auto &key_value : randomization_options) {
        wxBoxSizer *option_sizer = new wxBoxSizer(wxHORIZONTAL);

        const RandomizationOption &randomization_option = key_value.second;
        checkboxes[i] = new wxCheckBox(this, wxID_ANY, randomization_option.checkbox_label);
        checkboxes[i]->Bind(wxEVT_CHECKBOX, randomization_option.checkbox_click_handeler);
        checkboxes[i]->SetValue(true);
        wxButton *button = new wxButton(this, wxID_ANY, randomization_option.button_label);
        button->Bind(wxEVT_BUTTON, randomization_option.button_click_handler);
        if (i == 3)
            music_randomization_button = button;
        button->SetMinSize(wxSize(GENERAL_OPTIONS_BUTTON_SIZE_WIDTH, -1));

        option_sizer->Add(checkboxes[i], 0, wxLEFT, 5);
        option_sizer->AddStretchSpacer(1);
        option_sizer->Add(button, 0, wxRIGHT, 5);

        static_box_sizer->Add(option_sizer, 0, wxEXPAND | wxALL, 5);
        i++;
    }

    wxBoxSizer *advanced_options_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *advanced_options_button = new wxButton(this, wxID_ANY, wxT("Advanced Options"));
    advanced_options_button->SetMinSize(wxSize(GENERAL_OPTIONS_BUTTON_SIZE_WIDTH, -1));
    advanced_options_button->Bind(wxEVT_BUTTON, &GeneralOptions::onAdvancedOptionsClick, this);
    advanced_options_sizer->AddStretchSpacer(1);
    advanced_options_sizer->Add(advanced_options_button, 0, wxALL, 5);

    static_box_sizer->Add(advanced_options_sizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer *general_options_sizer_wrapper = new wxBoxSizer(wxHORIZONTAL);
    general_options_sizer_wrapper->AddSpacer(180);
    general_options_sizer_wrapper->Add(static_box_sizer, 1, wxEXPAND | wxALL, 0);

    general_options_sizer->Add(general_options_sizer_wrapper, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT,
                               FromDIP(25));
}

void GeneralOptions::drawRandomizationButtons() {
    wxStaticBox *static_box = new wxStaticBox(this, wxID_ANY, wxT(_STR_RANDOMIZER_BUTTONS_STR));
    wxStaticBoxSizer *static_box_sizer = new wxStaticBoxSizer(static_box, wxHORIZONTAL);

    choose_disc_1_button = new wxButton(this, wxID_ANY, wxT("Choose Disc 1"));
    choose_disc_1_button->Bind(wxEVT_BUTTON, &GeneralOptions::onChooseDisc1Click, this);
    choose_disc_2_button = new wxButton(this, wxID_ANY, wxT("Choose Disc 2"));
    choose_disc_2_button->Bind(wxEVT_BUTTON, &GeneralOptions::onChooseDisc2Click, this);
    randomize_button = new wxButton(this, wxID_ANY, wxT("Randomize"));
    randomize_button->Enable(false);
    randomize_button->Bind(wxEVT_BUTTON, &GeneralOptions::onRandomizeClick, this);

    static_box_sizer->Add(choose_disc_1_button, 1, wxALL, 5);
    static_box_sizer->Add(choose_disc_2_button, 1, wxALL, 5);
    static_box_sizer->Add(randomize_button, 1, wxALL, 5);

    wxBoxSizer *buttons_sizer_wrapper = new wxBoxSizer(wxHORIZONTAL);
    buttons_sizer_wrapper->AddSpacer(180);
    buttons_sizer_wrapper->Add(static_box_sizer, 1, wxEXPAND | wxALL, 0);

    // Keep the native button height: proportional rows in a fixed-height
    // panel compressed these controls almost to zero on macOS.
    general_options_sizer->Add(buttons_sizer_wrapper, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, FromDIP(25));
}

void GeneralOptions::onEnemiesCheckboxClick(wxCommandEvent &event) {
    ctx->flags ^= UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_ENEMIES;
    header->redrawContext();
    (void)event;
}

void GeneralOptions::onEnemiesButtonClick(wxCommandEvent &event) {
    (void)event;
    EnemiesOptionsDialog *dialog = new EnemiesOptionsDialog(ctx, header);
    dialog->Show(true);
}

void GeneralOptions::onItemsCheckboxClick(wxCommandEvent &event) {
    ctx->flags ^= UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_ITEMS;
    header->redrawContext();
    (void)event;
}

void GeneralOptions::onItemsButtonClick(wxCommandEvent &event) {
    (void)event;
    ItemsOptionsDialog *dialog = new ItemsOptionsDialog(ctx, header);
    dialog->Show(true);
}

void GeneralOptions::onMagicCheckboxClick(wxCommandEvent &event) {
    if (ctx->no_magic)
        return;
    ctx->flags ^= UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_MAGIC;
    header->redrawContext();
    (void)event;
}

void GeneralOptions::onMagicButtonClick(wxCommandEvent &event) {
    if (ctx->no_magic)
        return;
    (void)event;
    MagicOptionsDialog *dialog = new MagicOptionsDialog(ctx, header);
    dialog->Show(true);
}

void GeneralOptions::onMusicCheckboxClick(wxCommandEvent &event) {
    if (ctx->pe1_music)
        return;
    ctx->flags ^= UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_MUSIC;
    header->redrawContext();
    (void)event;
}

void GeneralOptions::onMusicButtonClick(wxCommandEvent &event) {
    (void)event;
    if (ctx->pe1_music)
        return;
    MusicOptionsDialog *dialog = new MusicOptionsDialog(ctx, header);
    dialog->Show(true);
}

void GeneralOptions::onAdvancedOptionsClick(wxCommandEvent &event) {
    (void)event;
    AdvancedOptionsDialog *dialog = new AdvancedOptionsDialog(ctx);
    dialog->Show(true);
}

void GeneralOptions::onChooseDisc1Click(wxCommandEvent &event) {
    (void)event;

    wxFileDialog openFileDialog(this, _("Choose disk 1 file"), "", "", "*.bin",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk()) {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }

    ctx->disc_1 = openFileDialog.GetPath();
    if (!(ctx->disc_1.empty()) && !(ctx->disc_2.empty())) {
        randomize_button->Enable(true);
    }
}

void GeneralOptions::onChooseDisc2Click(wxCommandEvent &event) {
    (void)event;

    wxFileDialog openFileDialog(this, _("Choose disk 2 file"), "", "", "*.bin",
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxFileInputStream input_stream(openFileDialog.GetPath());
    if (!input_stream.IsOk()) {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }

    ctx->disc_2 = openFileDialog.GetPath();
    if (!(ctx->disc_2.empty()) && !(ctx->disc_1.empty())) {
        randomize_button->Enable(true);
    }
}

void GeneralOptions::onRandomizeClick(wxCommandEvent &event) {
    (void)event;

    randomize_button->Enable(false);

    m_progressDialog = new wxProgressDialog("Randomizing...", "Please wait...", 100, this,
                                            wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_CAN_ABORT);
    m_progressDialog->Update(0);

    m_randomizerThread = new RandomizerThread(this, ctx->convert(), ctx->logger);
    if (m_randomizerThread->Create() != wxTHREAD_NO_ERROR) {
        wxLogError("Can't create thread!");
        delete m_randomizerThread;
        return;
    }

    m_randomizerThread->Run();

    ctx->disc_1 = "";
    ctx->disc_2 = "";
}

void GeneralOptions::onThreadUpdate(wxThreadEvent &event) {
    int progress = event.GetInt();

    if (event.GetId() == 1) {
        bool shouldContinue = m_progressDialog->Update(progress);
        m_progressDialog->Update(m_progressDialog->GetValue(), event.GetString());

        if (!shouldContinue) {
            m_randomizerThread->setCancelFlag(true);
        }
    } else if (event.GetId() == 2) {
        m_progressDialog->Update(100);
    }
}

void GeneralOptions::onThreadWorkDone(wxThreadEvent &event) {
    if (m_progressDialog) {
        m_progressDialog->Update(100);
        delete m_progressDialog;
        m_progressDialog = nullptr;
    }

    if (event.GetInt() == 0)
        wxMessageBox("Randomized ROM is ready!", "Information", wxOK | wxICON_INFORMATION, this);
    else if (event.GetInt() < 0)
        wxMessageBox(event.GetString(), "Randomization error", wxOK | wxICON_ERROR, this);
}

void GeneralOptions::updateCheckboxes() {
    checkboxes[0]->SetValue(ctx->flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_ENEMIES);
    checkboxes[1]->SetValue(ctx->flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_ITEMS);
    checkboxes[2]->SetValue(!ctx->no_magic && (ctx->flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_MAGIC));
    checkboxes[2]->Enable(!ctx->no_magic);
    magic_randomization_button->Enable(!ctx->no_magic);
    checkboxes[3]->SetValue(!ctx->pe1_music && (ctx->flags & UIContext::_UI_CONTEXT_FLAG__RANDOMIZE_MUSIC));
    checkboxes[3]->Enable(!ctx->pe1_music);
    music_randomization_button->Enable(!ctx->pe1_music);
}
