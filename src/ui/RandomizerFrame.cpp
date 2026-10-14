#include "ui/RandomizerFrame.hpp"
#include "ui/shared/Layout.hpp"
#include "utils/RuntimePaths.hpp"

// wxBEGIN_EVENT_TABLE(RandomizerFrame, wxFrame)
//     EVT_CHOICE(1002, RandomizerFrame::OnChoice)
//     EVT_CHECKLISTBOX(1003, RandomizerFrame::OnCheckListBox)
//     EVT_BUTTON(1004, RandomizerFrame::OnButtonClick)
//     EVT_THREAD(1, RandomizerFrame::OnThreadUpdate)
//     EVT_THREAD(2, RandomizerFrame::OnThreadWorkDone)
// wxEND_EVENT_TABLE()

RandomizerFrame::RandomizerFrame()
    : wxFrame(nullptr, wxID_ANY, "Randomize Eve II", wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_FRAME_STYLE) {
    _logger = PE2Logger::getInstance((utils::paths::workRoot / "run_log.txt").string());
    panel = new RandomizerPanel(this);

    wxBoxSizer *globalSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(globalSizer);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, MAX_SEED_VALUE);
    uint16_t random_seed = dis(gen);
    std::shared_ptr<UIContext> ctx = std::make_shared<UIContext>(random_seed, _logger);

    header = new Header(panel, ctx);
    globalSizer->Add(header, 0, wxEXPAND);

    wxBoxSizer *bodySizer = new wxBoxSizer(wxHORIZONTAL);

    general_options = new GeneralOptions(panel, ctx, header);
    bodySizer->Add(general_options, 0, wxTOP, 0); // TODO: fix negative magic value

    globalSizer->Add(bodySizer, 0, wxTOP, 0);

    footer = new Footer(panel);

    globalSizer->AddSpacer(FromDIP(20));
    globalSizer->Add(footer, 0, wxEXPAND | wxBOTTOM, FromDIP(10));

    // Include native control sizes and title-bar insets instead of assuming
    // the same fixed frame height on Windows and macOS.
    ui::fitContent(this, panel);

    wxIcon frameIcon;
#ifdef __WXMSW__
    // Already linked by icon.rc. Loading a file here goes through the shell's
    // icon-path parser (e.g. ';' is interpreted as an icon index separator).
    frameIcon.LoadFile("IDI_ICON1", wxBITMAP_TYPE_ICO_RESOURCE);
#else
    frameIcon.LoadFile(wxString(utils::paths::asset("icon.ico").wstring()), wxBITMAP_TYPE_ICO);
#endif

    if (frameIcon.IsOk())
        SetIcon(frameIcon);

    panel->Layout();
    panel->Refresh();
    panel->Update();
}
