#pragma once

#include <algorithm>
#include <vector>
#include <wx/grid.h>
#include <wx/wx.h>

#include "ui/context/UIContext.hpp"
#include "ui/widgets/header/Header.hpp"

class MusicOptionsDialog : public wxFrame {
  public:
    MusicOptionsDialog(std::shared_ptr<UIContext>, Header *);

    struct MusicOption {
        std::string name;
        uint8_t id;
        bool value;
        uint64_t flag;
    };

  private:
    std::shared_ptr<UIContext> ctx;
    UIContext ctx_copy;
    Header *header;

    // consts
    static const std::string _MUSIC_OPTIONS_DIALOG_TITLE_STR;
    static const std::string _MUSIC_OPTIONS_GRID_NAME_STR;
    static const std::string _MUSIC_OPTIONS_GRID_COLUMN_LABEL_STR;
    static const std::string _MUSIC_OPTIONS_GRID_TIP_STR;
    static const std::vector<MusicOption> _MUSIC_OPTIONS_GRID_ITEMS;
    static const int _MUSIC_OPTIONS_POOL_COLUMN_WIDTH = 70;
    static const int _MUSIC_OPTIONS_POOL_ROW_WIDTH = 300;
    static const int _MUSIC_OPTIONS_POOL_GRID_WIDTH = 400;
    static const int _MUSIC_OPTIONS_POOL_GRID_HEIGHT = 350;
    static const int _MUSIC_OPTIONS_DIALOG_WIDTH = 500;
    static const int _MUSIC_OPTIONS_DIALOG_HEIGHT = 500;

    // event handler methods
    void OnClose(wxCloseEvent &);
    void OnCellLeftClick(wxGridEvent &);
    void OnApplyEvent(wxCommandEvent &);
    void OnCancelEvent(wxCommandEvent &);
    void OnDefaultEvent(wxCommandEvent &);

    // sub-components rendering methods
    void DrawMusicGrid();

    // sub-components
    wxPanel *panel;
    wxBoxSizer *dialog_sizer;
    wxBoxSizer *dialog_inner_sizer;
    wxGrid *music_grid_items;
    wxScrolledWindow *music_scrolled_window;

    wxDECLARE_EVENT_TABLE();
};
