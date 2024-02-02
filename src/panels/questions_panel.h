#pragma once
#include <wx/wx.h>
#include "test_starting_info.h"
#include "selectable_buttons.h"

class questionsPanel : public wxPanel
{
public:
    questionsPanel(wxWindow *, test_info&); 
    void set_question(unsigned short int, unsigned short int);
private:
        test_info test_starting_data;
        wxStaticText* question_number_text;
};
