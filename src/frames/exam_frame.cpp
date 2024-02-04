#include "exam_frame.h"

wxBEGIN_EVENT_TABLE(examFrame, wxFrame)
    EVT_TIMER(examFrame::ID_timer, examFrame::OnTimer)
        EVT_COMMAND(wxID_ANY, QUESTION_NAVIGATED, examFrame::on_question_navigated)
    EVT_COMMAND(wxID_ANY, SECTION_NAVIGATED, examFrame::on_section_navigated)
    EVT_COMMAND(wxID_ANY, QUESTION_OPTION_CLICKED, examFrame::on_options_on_question_display_clicked)
            wxEND_EVENT_TABLE()

                examFrame::examFrame(test_info &test_starting_data) : test_starting_data(test_starting_data), wxFrame(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxGetDisplaySize(), wxDEFAULT_FRAME_STYLE)
{
    SetWindowStyle(GetWindowStyle() | wxFRAME_NO_TASKBAR);

    this->result_doc = rapidcsv::Document(this->test_starting_data.student_test_result_file);
    this->selected_questions = selected_questions_info(this->test_starting_data.number_of_sections);

    wxGridBagSizer *grid_bag_sizer = new wxGridBagSizer(0, 0);

    auto __background_color = wxColor(235, 237, 237);

    // Create panels
    this->test_info_panel = new testInfoPanel(this);
    test_info_panel->SetBackgroundColour(__background_color);

    this->questions_navigation_panel = new questionsNavigationPanel(this, this->current_section_order, this->selected_questions.question[this->current_section_order], this->result_doc, this->test_starting_data.number_of_sections);
    questions_navigation_panel->SetBackgroundColour(__background_color);

    this->question_display_panel = new questionsPanel(this, this->test_starting_data, this->result_doc);
    question_display_panel->SetBackgroundColour(__background_color);

    this->questions_info_panel = new sectionInfoPanel(this);
    questions_info_panel->SetBackgroundColour(__background_color);

    // Add panels to the grid bag sizer
    static float __x_stretch = 19.48f;
    static float __y_stretch = 5.65f;
    grid_bag_sizer->Add(test_info_panel, wxGBPosition(0 * __y_stretch, 0 * __x_stretch), wxGBSpan(2 * __y_stretch, 8 * __x_stretch), wxEXPAND | wxALL, 5);
    grid_bag_sizer->Add(question_display_panel, wxGBPosition(2 * __y_stretch, 0 * __x_stretch), wxGBSpan(8 * __y_stretch, 8 * __x_stretch), wxEXPAND | wxALL, 5);
    grid_bag_sizer->Add(questions_info_panel, wxGBPosition(0 * __y_stretch, 8 * __x_stretch), wxGBSpan(2 * __y_stretch, 2 * __x_stretch), wxEXPAND | wxALL, 5);
    grid_bag_sizer->Add(questions_navigation_panel, wxGBPosition(2 * __y_stretch, 8 * __x_stretch), wxGBSpan(8 * __y_stretch, 2 * __x_stretch), wxEXPAND | wxALL, 5);

    SetSizerAndFit(grid_bag_sizer);

    // Create and add the timer display
    this->exam_remaining_time = this->test_starting_data.duration;
    this->exam_timer_display = new wxStaticText(test_info_panel, wxID_ANY, formatted_exam_time());
    this->exam_timer_display->SetFont(wxFontInfo(35).Family(wxFONTFAMILY_SWISS));
    this->exam_timer_display->SetForegroundColour(wxColour(0, 0, 0)); // Initial color: black
    exam_timer = new wxTimer(this, this->ID_timer);
    exam_timer->Start(1000);

    test_info_panel->SetSizer(new wxBoxSizer(wxVERTICAL));
    test_info_panel->GetSizer()->AddStretchSpacer();
    test_info_panel->GetSizer()->Add(this->exam_timer_display, 0, wxALIGN_RIGHT | wxALL, 10);
}

void examFrame::OnTimer(wxTimerEvent &event)
{

    this->exam_remaining_time--;

    this->exam_timer_display->SetLabel(formatted_exam_time());

    // Check if the specific time is reached
    if (this->exam_remaining_time <= this->test_starting_data.threshold_time)
    {
        this->exam_timer_display->SetForegroundColour(wxColor(255, 40, 40));
    }

    // Check if the timer has reached 0
    if (this->exam_remaining_time == 0)
    {
        // Stop the timer
        this->exam_timer->Stop();
        // End the exam
        wxMessageBox("Exam time is over", "Time's up!", wxOK | wxICON_WARNING);
    }
}

wxString examFrame::formatted_exam_time(void)
{
    if (this->exam_remaining_time <= 60 * 10)
    {
        return wxString::Format("%01d:%02d", this->exam_remaining_time / 60, this->exam_remaining_time % 60);
    }
    else if (this->exam_remaining_time <= 3600)
    {
        return wxString::Format("%02d:%02d", this->exam_remaining_time / 60, this->exam_remaining_time % 60);
    }
    else if (this->exam_remaining_time <= 3600 * 24)
    {
        return wxString::Format("%03d:%02d", this->exam_remaining_time / 60, this->exam_remaining_time % 60);
    }
    return wxString::Format("%04d:%02d", this->exam_remaining_time / 60, this->exam_remaining_time % 60);
}

void examFrame::on_question_navigated(wxCommandEvent &event)
{
    if (this->selected_questions.question[this->current_section_order] == event.GetInt())
        return;

    this->selected_questions.question[this->current_section_order] = event.GetInt();
    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);


    this->question_display_panel->enable_next(!((this->selected_questions.question[this->current_section_order] == this->test_starting_data.sections[this->test_starting_data.number_of_sections - 1].number_of_questions) and (this->current_section_order == this->test_starting_data.number_of_sections)));
    this->question_display_panel->enable_previous(!((this->selected_questions.question[this->current_section_order] == 1) and (this->current_section_order == 1)));

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);
}

void examFrame::on_section_navigated(wxCommandEvent& event)
{
    if (this->current_section_order == event.GetInt())
        return;

    this->current_section_order = event.GetInt();

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);
    this->questions_navigation_panel->refresh(this->current_section_order, this->selected_questions.question[this->current_section_order], this->result_doc);


    this->question_display_panel->enable_next(!((this->selected_questions.question[this->current_section_order] == this->test_starting_data.sections[this->test_starting_data.number_of_sections - 1].number_of_questions) and (this->current_section_order == this->test_starting_data.number_of_sections)));
    this->question_display_panel->enable_previous(!((this->selected_questions.question[this->current_section_order] == 1) and (this->current_section_order == 1)));

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);
}

void examFrame::on_options_on_question_display_clicked(wxCommandEvent& event)
{
    if (event.GetString() == wxString("Next >>")) {
        if (this->test_starting_data.sections[this->current_section_order - 1].number_of_questions > this->selected_questions.question[this->current_section_order]) {
            // navigate to next question in the current section
            this->selected_questions.question[this->current_section_order] += 1;
        }
        else{
            // navigate to first question in the next section
            this->current_section_order += 1;
            this->selected_questions.question[this->current_section_order] = 1;
        }
        this->questions_navigation_panel->refresh(this->current_section_order, this->selected_questions.question[this->current_section_order], this->result_doc);
    }
    else if (event.GetString() == wxString("<< Previous")) {
        if (this->selected_questions.question[this->current_section_order] != 1) {
            // navigate to previous question in the current section
            this->selected_questions.question[this->current_section_order] -= 1;
        }
        else{
            // navigate to last question in the previous section
            this->current_section_order -= 1;
            this->selected_questions.question[this->current_section_order] = this->test_starting_data.sections[this->current_section_order - 1].number_of_questions;
        }
        this->questions_navigation_panel->refresh(this->current_section_order, this->selected_questions.question[this->current_section_order], this->result_doc);
    }

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);
    this->questions_navigation_panel->refresh(this->current_section_order, this->selected_questions.question[this->current_section_order], this->result_doc);


    this->question_display_panel->enable_next(!((this->selected_questions.question[this->current_section_order] == this->test_starting_data.sections[this->test_starting_data.number_of_sections - 1].number_of_questions) and (this->current_section_order == this->test_starting_data.number_of_sections)));
    this->question_display_panel->enable_previous(!((this->selected_questions.question[this->current_section_order] == 1) and (this->current_section_order == 1)));

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);
}
