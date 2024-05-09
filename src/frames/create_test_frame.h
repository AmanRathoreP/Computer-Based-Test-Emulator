#pragma once
#include "./../panels/test_creation_from_questions_options_panel.h"
#include "./../panels/test_creation_from_questions_list_panel.h"
#include "../constants/graphics/frame_icons.h"
#include <vector>
#include <wx/wx.h>

class testCreateFrame : public wxFrame
{
public:
    testCreateFrame(wxFrame *);

private:
    questionsOptionsForTestCreation *questions_options;
    questionsListForTestCreation *questions_list;
    void on_data_updated(wxCommandEvent& event);
    void on_need_current_selection(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};
