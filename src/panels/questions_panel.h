#pragma once
#include <wx/wx.h>
#include "test_starting_info.h"
#include "selectable_buttons.h"

class questionsPanel : public wxPanel
{
public:
    questionsPanel(wxWindow *, test_info&); 
private:
        test_info test_starting_data;
};
