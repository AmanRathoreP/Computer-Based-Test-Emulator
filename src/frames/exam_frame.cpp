#include "exam_frame.h"

wxBEGIN_EVENT_TABLE(examFrame, wxFrame)
    EVT_TIMER(examFrame::ID_timer, examFrame::OnTimer)
        wxEND_EVENT_TABLE()

            examFrame::examFrame(test_info &test_starting_data) : test_starting_data(test_starting_data), wxFrame(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxGetDisplaySize(), wxDEFAULT_FRAME_STYLE)
{
    SetWindowStyle(GetWindowStyle() | wxFRAME_NO_TASKBAR);

    // Create the main sizer
    wxBoxSizer *__main_sizer_horizontal = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *__left_sizer_vertical = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *__right_sizer_vertical = new wxBoxSizer(wxVERTICAL);

    wxPanel *test_info_panel = new wxPanel(this, wxID_ANY);
    test_info_panel->SetBackgroundColour(wxColour(255, 255, 255));
    wxPanel *questions_navigation_panel = new wxPanel(this, wxID_ANY);
    questions_navigation_panel->SetBackgroundColour(wxColour(128, 128, 128));
    wxPanel *question_display_panel = new wxPanel(this, wxID_ANY);
    question_display_panel->SetBackgroundColour(wxColour(10, 10, 10));
    wxPanel *questions_info_panel = new wxPanel(this, wxID_ANY);
    questions_info_panel->SetBackgroundColour(wxColour(214, 47, 171));

    // Create and add the timer display
    this->exam_remaining_time = this->test_starting_data.duration;
    this->exam_timer_display = new wxStaticText(test_info_panel, wxID_ANY, formatted_exam_time());
    this->exam_timer_display->SetFont(wxFontInfo(26).Family(wxFONTFAMILY_SWISS));
    this->exam_timer_display->SetForegroundColour(wxColour(0, 0, 0)); // Initial color: black
    exam_timer = new wxTimer(this, this->ID_timer);
    exam_timer->Start(1000);

    test_info_panel->SetSizer(new wxBoxSizer(wxVERTICAL));
    test_info_panel->GetSizer()->AddStretchSpacer();
    test_info_panel->GetSizer()->Add(this->exam_timer_display, 0, wxALIGN_RIGHT | wxALL, 10); // Use wxALIGN_RIGHT without specifying wxALIGN_LEFT

    __left_sizer_vertical->Add(test_info_panel, 2.4, wxEXPAND | wxALL, 0);
    __left_sizer_vertical->Add(question_display_panel, 7.6, wxEXPAND | wxALL, 0);
    __right_sizer_vertical->Add(questions_info_panel, 2.4, wxEXPAND | wxALL, 0);
    __right_sizer_vertical->Add(questions_navigation_panel, 7.6, wxEXPAND | wxALL, 0);

    // Add panels to the main sizer with the specified ratios
    __main_sizer_horizontal->Add(__left_sizer_vertical, 7.6, wxEXPAND | wxALL, 0);
    __main_sizer_horizontal->Add(__right_sizer_vertical, 2.4, wxEXPAND | wxALL, 0);

    // Set the main sizer for the frame
    SetSizerAndFit(__main_sizer_horizontal);
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