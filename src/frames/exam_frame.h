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
    rapidcsv::Document result_doc;

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
    void on_section_navigated(wxCommandEvent &event);
    void on_options_on_question_display_clicked(wxCommandEvent &event);
    unsigned short int current_section_order = 1;

    struct selected_questions_info {
        std::map<unsigned short int, unsigned short int> question;
        
        selected_questions_info(unsigned short int total_number_of_sections) {
            for (unsigned short int i = total_number_of_sections; i != 0; i--) {
                question[i] = 1;
            }
        }
    };
    selected_questions_info selected_questions = selected_questions_info(1);

    wxDECLARE_EVENT_TABLE();
};
