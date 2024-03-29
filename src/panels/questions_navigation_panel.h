#pragma once
#include "question_number_button.h"
#include "basic_csv_functions.h"
#include <wx/wx.h>

wxDECLARE_EVENT(QUESTION_NAVIGATED, wxCommandEvent);

class questionsNavigationPanel : public wxPanel
{
public:
    questionsNavigationPanel(wxWindow *, unsigned short int, unsigned short int, rapidcsv::Document, unsigned short int);
    void refresh(unsigned short int, unsigned short int, rapidcsv::Document);

private:
    wxGridSizer *grid_sizer;
    std::vector<questionNumberButton *> buttons;
    void on_question_changed(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};
