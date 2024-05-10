#pragma once
#include "custom_buttons.h"
#include "../constants/graphics/frame_icons.h"
#include "./../panels/result_creation_list_panel.h"
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/filename.h>

class resultCreateFrame : public wxFrame
{
public:
    resultCreateFrame(wxFrame *, const wxString test_location);
    bool dirty_path(void);

private:
    wxString path;
    wxString result_csv_file_name;
    wxString validity_report_csv_file_path;
    questionsListForResultCreation* questions_list;
    customButton* generate_button;
};
