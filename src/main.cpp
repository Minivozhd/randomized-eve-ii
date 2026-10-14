#include "main.hpp"
#include "utils/RuntimePaths.hpp"
#include <filesystem>
#include <wx/stdpaths.h>

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    SetAppName("PE2Randomizer");
    const auto &paths = wxStandardPaths::Get();
    const auto executableDir = std::filesystem::path(paths.GetExecutablePath().ToStdWstring()).parent_path();
    const std::vector<std::filesystem::path> candidates = {
        std::filesystem::path(paths.GetResourcesDir().ToStdWstring()),
        executableDir / "resources",
        executableDir.parent_path() / "share/pe2randomizer",
    };
    try {
        bool found = false;
        for (const auto &candidate : candidates) {
            if (!utils::paths::hasResources(candidate))
                continue;
            utils::paths::configure(candidate,
                                    std::filesystem::path(paths.GetUserLocalDataDir().ToStdWstring()));
            found = true;
            break;
        }
        if (!found)
            throw std::runtime_error("Cannot find runtime assets in the app bundle or installation. Rebuild "
                                     "or reinstall the randomizer.");
    } catch (const std::exception &error) {
        wxMessageBox(wxString::FromUTF8(error.what()), "Randomizer resources", wxOK | wxICON_ERROR);
        return false;
    }

    const wxString name = wxString::Format("RandomizedEve2-%s", wxGetUserId().c_str());
    checker = new wxSingleInstanceChecker(name);
    if (checker->IsAnotherRunning()) {
        wxMessageBox("This program is already running.", "Info", wxOK | wxICON_INFORMATION);
        delete checker;
        checker = nullptr;
        return false;
    }

    wxInitAllImageHandlers();
    std::cout << "Starting PE2 Randomizer..." << std::endl;
    RandomizerFrame *frame = new RandomizerFrame();
    frame->Show(true);

    return true;
}

int MyApp::OnExit() {
    delete checker;
    return 0;
}
