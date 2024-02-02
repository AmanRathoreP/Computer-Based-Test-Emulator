#pragma once
#include <wx/wx.h>
#include "test_starting_info.h"
#include "selectable_buttons.h"

wxDECLARE_EVENT(SECTION_NAVIGATED, wxCommandEvent);

class questionsPanel : public wxPanel
{
public:
    questionsPanel(wxWindow *, test_info&); 
    void set_question(unsigned short int, unsigned short int);
private:
        test_info test_starting_data;
        wxStaticText* question_number_text;    
        
        std::vector<selectableButton*> buttons;
        void on_section_changed(wxCommandEvent& event);

        wxDECLARE_EVENT_TABLE();
};

bool inline __compare_by_selection_order(const selectableButton*, const selectableButton*);