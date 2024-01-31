#pragma once
#include "main_frame.h"
#include "test_starting_info.h"
#include <wx/wx.h>
#include <string>

class examFrame : public wxFrame
{
public:
    examFrame(test_info &);
    unsigned int exam_remaining_time; // in seconds
    enum
    {
        ID_timer = 4,
    };
    void OnTimer(wxTimerEvent &);

private:
    wxTimer *exam_timer; // resolution = 1 second
    wxStaticText *exam_timer_display;
    test_info test_starting_data;
    wxString formatted_exam_time(void);

    wxDECLARE_EVENT_TABLE();
};