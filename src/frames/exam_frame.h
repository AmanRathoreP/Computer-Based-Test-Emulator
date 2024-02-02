#pragma once
#include "main_frame.h"
#include "test_starting_info.h"
#include "./../panels/questions_panel.h"
#include "./../panels/questions_navigation_panel.h"
#include "./../panels/section_info_panel.h"
#include "./../panels/test_info_panel.h"
#include <wx/wx.h>
#include <wx/gbsizer.h>
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
    testInfoPanel *test_info_panel;
    questionsNavigationPanel *questions_navigation_panel;
    questionsPanel *question_display_panel;
    sectionInfoPanel *questions_info_panel;

    wxTimer *exam_timer; // resolution = 1 second
    wxStaticText *exam_timer_display;
    test_info test_starting_data;
    wxString formatted_exam_time(void);

    void on_question_navigated(wxCommandEvent &event);
    unsigned short int current_question = 1;
    unsigned short int current_section_order = 1;

    wxDECLARE_EVENT_TABLE();
};
