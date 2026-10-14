#include "ui/widgets/header/Header.hpp"

// wxBEGIN_EVENT_TABLE(Header, wxPanel)
// wxEND_EVENT_TABLE()

size_t getRandomInt() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, MAX_SEED_VALUE);

    return dist(rng);
}

Header::Header(wxWindow *parent, std::shared_ptr<UIContext> _ctx)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxTRANSPARENT_WINDOW),
      ctx(_ctx) {
    SetMinSize(FromDIP(wxSize(HEADER_SIZE_WIDTH, -1)));

    ctx->addListener([this]() { redrawContext(); });

    header_main_sizer_wrapper = new wxBoxSizer(wxVERTICAL);

    header_main_sizer = new wxBoxSizer(wxHORIZONTAL);
    random_seed_vertical_sizer = new wxBoxSizer(wxVERTICAL);

    drawVersionString();
    drawQRCodeBitmap(_ctx->serialize());
    drawTextInputs(_ctx->serialize(), wxString::Format(wxT("%zu"), (size_t)_ctx->seed));
    header_main_sizer_wrapper->Add(header_main_sizer, 0, wxALL | wxEXPAND);

    header_main_sizer->Add(random_seed_vertical_sizer, 1, wxLEFT | wxTOP | wxRIGHT, QRCODE_PADDING);
    header_main_sizer->Add(HEADER_INPUT_PADDING, 0, 0);

    random_seed_vertical_sizer->Layout();
    header_main_sizer_wrapper->Layout();

    SetSizer(header_main_sizer_wrapper);
}

void Header::redrawContext() {
    input_state_string->ChangeValue(wxString::Format(wxT("%s"), ctx->serialize()));
    qr_code_bitmap->SetBitmap(GenerateQRCode(ctx->serialize()));
}

// TODO:???
void Header::drawVersionString() {
    wxBoxSizer *version_string_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *version_str = new wxStaticText(this, wxID_ANY, _VERSION_STR);
    version_str->SetForegroundColour(uiColorGray);
    version_string_sizer->AddStretchSpacer(1);
    version_string_sizer->Add(version_str, 0, wxEXPAND | wxRIGHT);
    header_main_sizer_wrapper->Add(version_string_sizer, 0, wxALL | wxEXPAND);
}

void Header::drawQRCodeBitmap(std::string init_data) {
    qr_code_bitmap = new wxStaticBitmap(this, wxID_ANY, GenerateQRCode(init_data));
    qr_code_bitmap->SetMinSize(wxSize(QRCODE_BITMAP_SIZE, QRCODE_BITMAP_SIZE));
    qr_code_bitmap->SetMaxSize(wxSize(QRCODE_BITMAP_SIZE, QRCODE_BITMAP_SIZE));

    header_main_sizer->Add(0, 0, 0);
    header_main_sizer->Add(qr_code_bitmap, 0, wxLEFT | wxTOP, QRCODE_PADDING);
}

void Header::drawSingleTextInput(wxTextCtrl **text_input, wxStaticBoxSizer *static_box_sizer,
                                 Header::HeaderInput header_input, wxString default_value,
                                 bool multiline = false) {
    wxBoxSizer *string_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *string_label = new wxStaticText(this, wxID_ANY, header_input.label);
    string_label->SetMinSize(wxSize(HEADER_LABEL_SIZE, -1));

    wxStaticText *string_tip = new wxStaticText(this, wxID_ANY, wxT("[?]"));
    string_tip->SetForegroundColour(*wxBLUE);
    string_tip->SetToolTip(header_input.tip);

    if (multiline) {
        *text_input =
            new wxTextCtrl(this, wxID_ANY, default_value, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
        (*text_input)->SetMaxSize(wxSize(HEADER_INPUT_SIZE, HEADER_INPUT_HEIGHT));
    } else {
        *text_input = new wxTextCtrl(this, wxID_ANY, default_value);
        (*text_input)->SetMaxSize(wxSize(HEADER_INPUT_SIZE, -1));
    }
    string_sizer->Add(string_label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    string_sizer->Add(string_tip, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    string_sizer->Add(*text_input, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    static_box_sizer->Add(string_sizer, 0, wxEXPAND | wxALL, 5);
}

void Header::drawTextInputs(std::string state_str_default_val, wxString seed_default_val) {
    wxStaticBox *staticBox = new wxStaticBox(this, wxID_ANY, wxT(_RANDOMIZATION_STATE_OPTIONS_STR));
    staticBox->SetMinSize(wxSize(600, -1));
    staticBox->SetMaxSize(wxSize(600, -1));
    wxStaticBoxSizer *staticBoxSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);

    drawSingleTextInput(&input_state_string, staticBoxSizer,
                        header_inputs[Header::HeaderInputType::HEADER_INPUT_STATE],
                        wxString(state_str_default_val), true);
    input_state_string->Bind(wxEVT_TEXT, &Header::OnContextTextChanged, this);

    drawSingleTextInput(&input_seed, staticBoxSizer,
                        header_inputs[Header::HeaderInputType::HEADER_INPUT_SEED], seed_default_val);
    input_seed->Bind(wxEVT_TEXT, &Header::OnSeedTextChanged, this);

    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton *randomize_seed_button = new wxButton(this, wxID_OK, _RANDOMIZE_SEED_BUTTON_STR);
    randomize_seed_button->Bind(wxEVT_BUTTON, &Header::OnRandomizeSeedClick, this);
    buttons_sizer->Add(randomize_seed_button);

    staticBoxSizer->Add(buttons_sizer, 0, wxEXPAND | wxALL, 5);
    random_seed_vertical_sizer->Add(staticBoxSizer, 0, wxFIXED_MINSIZE | wxALL, 5);
}

// TODO: Move to other class
wxBitmap Header::GenerateQRCode(const wxString &text) {
    QRcode *qrcode = QRcode_encodeString(text.mb_str(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);
    if (!qrcode) {
        return wxBitmap();
    }

    qr_size = qrcode->width > 0 ? qrcode->width : 1;
    int imageSize = qr_size * 2;
    wxImage image(imageSize, imageSize);
    unsigned char *data = image.GetData();

    int scale = std::max(imageSize / qr_size, 1);

    for (int y = 0; y < qr_size; y++) {
        for (int x = 0; x < qr_size; x++) {
            unsigned char color = qrcode->data[y * qr_size + x] & 1 ? 0 : 255;
            for (int dy = 0; dy < scale; dy++) {
                for (int dx = 0; dx < scale; dx++) {
                    int pixelX = x * scale + dx;
                    int pixelY = y * scale + dy;
                    data[(pixelY * imageSize + pixelX) * 3 + 0] = color;
                    data[(pixelY * imageSize + pixelX) * 3 + 1] = color;
                    data[(pixelY * imageSize + pixelX) * 3 + 2] = color;
                }
            }
        }
    }

    QRcode_free(qrcode);
    return wxBitmap(image);
}

void Header::OnContextTextChanged(wxCommandEvent &event) {
    (void)event;
    std::string context_str = std::string(input_state_string->GetValue().ToUTF8().data());

    std::cout << "on context text change call: " << context_str << std::endl;

    UIContext ctx_copy(*ctx);
    try {
        ctx->deserialize(context_str);
        input_seed->ChangeValue(wxString::Format(wxT("%zu"), (size_t)ctx->seed));
        ctx->notifyListeners();
    } catch (const std::runtime_error &e) {
        *ctx = ctx_copy;
        input_seed->ChangeValue(wxString::Format(wxT("%zu"), (size_t)ctx->seed));
    }
}

void Header::OnSeedTextChanged(wxCommandEvent &event) {
    (void)event;
    wxString value = input_seed->GetValue();
    wxString filtered_value;
    input_seed->GetInsertionPoint();

    for (wxChar c : value) {
        if (wxIsdigit(c)) {
            filtered_value += c;
        }
    }

    unsigned long seed_value;
    if (!filtered_value.ToULong(&seed_value)) {
        seed_value = 0;
    }
    if (seed_value > MAX_SEED_VALUE) {
        seed_value = MAX_SEED_VALUE;
    }

    size_t insertion_point = input_seed->GetInsertionPoint();
    ctx->seed = seed_value;
    input_seed->ChangeValue(wxString::Format(wxT("%lu"), seed_value));
    input_seed->SetInsertionPoint(insertion_point);
    redrawContext();
}

void Header::OnRandomizeSeedClick(wxCommandEvent &event) {
    (void)event;
    ctx->seed = getRandomInt();
    input_seed->ChangeValue(wxString::Format(wxT("%zu"), (size_t)ctx->seed));
    redrawContext();
}
