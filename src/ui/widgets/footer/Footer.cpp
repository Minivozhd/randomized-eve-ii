#include "ui/widgets/footer/Footer.hpp"
#include "BuildInfo.hpp"

const std::string Footer::_FOOTER_CREDITS_STR = "Strog, TheVozhd";
const std::string Footer::_FOOTER_YEAR_STR = PE2_COPYRIGHT_YEARS;

Footer::Footer(wxWindow *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxTRANSPARENT_WINDOW) {
    wxBoxSizer *footer_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *discord_button = new wxButton(this, wxID_OK, "Join Discord");
    discord_button->Bind(wxEVT_BUTTON, &Footer::onDiscordButtonClick, this);
    buttons_sizer->Add(discord_button, 0, wxEXPAND | wxALL, 5);

    wxButton *kofi_button = new wxButton(this, wxID_OK, "Support Us");
    kofi_button->Bind(wxEVT_BUTTON, &Footer::onKofiButtonClock, this);
    buttons_sizer->Add(kofi_button, 0, wxEXPAND | wxALL, 5);

    footer_sizer->Add(buttons_sizer, 0, wxCENTER, 5);

    wxStaticText *credits = new wxStaticText(this, wxID_ANY, Footer::_FOOTER_CREDITS_STR);
    credits->SetForegroundColour(uiColorGray);
    wxStaticText *year = new wxStaticText(this, wxID_ANY, Footer::_FOOTER_YEAR_STR);
    year->SetForegroundColour(uiColorGray);

    footer_sizer->Add(credits, 0, wxCENTER, 5);
    footer_sizer->Add(year, 0, wxCENTER, 5);

    SetSizer(footer_sizer);
}

void Footer::onDiscordButtonClick(wxCommandEvent &event) {
    (void)event;
    wxLaunchDefaultBrowser("https://discord.gg/ybyPJYyswW");
}

void Footer::onKofiButtonClock(wxCommandEvent &event) {
    (void)event;
    wxLaunchDefaultBrowser("https://ko-fi.com/thevozhd");
}
