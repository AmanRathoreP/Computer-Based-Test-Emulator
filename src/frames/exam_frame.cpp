#include "exam_frame.h"

wxBEGIN_EVENT_TABLE(examFrame, wxFrame)
    EVT_TIMER(examFrame::ID_timer, examFrame::OnTimer)
        wxEND_EVENT_TABLE()

            examFrame::examFrame(test_info &test_starting_data) : test_starting_data(test_starting_data), wxFrame(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxGetDisplaySize(), wxDEFAULT_FRAME_STYLE)
{
    SetWindowStyle(GetWindowStyle() | wxFRAME_NO_TASKBAR);

    wxGridBagSizer *grid_bag_sizer = new wxGridBagSizer(0, 0);

    auto __background_color = wxColor(235, 237, 237);

    // Create panels
    testInfoPanel *test_info_panel = new testInfoPanel(this);
    test_info_panel->SetBackgroundColour(__background_color);

    questionsNavigationPanel *questions_navigation_panel = new questionsNavigationPanel(this);
    questions_navigation_panel->SetBackgroundColour(__background_color);

    questionsPanel *question_display_panel = new questionsPanel(this, this->test_starting_data);
    question_display_panel->SetBackgroundColour(__background_color);

    sectionInfoPanel *questions_info_panel = new sectionInfoPanel(this);
    questions_info_panel->SetBackgroundColour(__background_color);

    // Add panels to the grid bag sizer
    static float __x_strech = 19.48f;
    static float __y_strech = 5.65f;
    grid_bag_sizer->Add(test_info_panel, wxGBPosition(0 * __y_strech, 0 * __x_strech), wxGBSpan(2 * __y_strech, 8 * __x_strech), wxEXPAND | wxALL, 5);
    grid_bag_sizer->Add(question_display_panel, wxGBPosition(2 * __y_strech, 0 * __x_strech), wxGBSpan(8 * __y_strech, 8 * __x_strech), wxEXPAND | wxALL, 5);
    grid_bag_sizer->Add(questions_info_panel, wxGBPosition(0 * __y_strech, 8 * __x_strech), wxGBSpan(2 * __y_strech, 2 * __x_strech), wxEXPAND | wxALL, 5);
    grid_bag_sizer->Add(questions_navigation_panel, wxGBPosition(2 * __y_strech, 8 * __x_strech), wxGBSpan(8 * __y_strech, 2 * __x_strech), wxEXPAND | wxALL, 5);

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