#include "exam_frame.h"

wxBEGIN_EVENT_TABLE(examFrame, wxFrame)
    EVT_TIMER(examFrame::ID_timer, examFrame::OnTimer)
        EVT_COMMAND(wxID_ANY, QUESTION_NAVIGATED, examFrame::on_question_navigated)
    EVT_COMMAND(wxID_ANY, SECTION_NAVIGATED, examFrame::on_section_navigated)
    EVT_COMMAND(wxID_ANY, QUESTION_OPTION_CLICKED, examFrame::on_options_on_question_display_clicked)
            wxEND_EVENT_TABLE()

                examFrame::examFrame(test_info &test_starting_data) : test_starting_data(test_starting_data), wxFrame(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxGetDisplaySize(), wxDEFAULT_FRAME_STYLE)
{
    SetIcon(app_icon);

    SetWindowStyle(GetWindowStyle() | wxFRAME_NO_TASKBAR);

    this->result_doc = rapidcsv::Document(this->test_starting_data.student_test_result_file);
    this->selected_questions = selected_questions_info(this->test_starting_data.number_of_sections);

    for (unsigned short int __section_order = 1; __section_order <= this->test_starting_data.number_of_sections; __section_order++)
        this->all_sections_order.push_back(__section_order);

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
    questions_info_panel->update_data(this->result_doc, { 1 });
    
    this->exam_questions_info = new sectionInfoPanel(this->test_info_panel);
    exam_questions_info->SetBackgroundColour(__background_color);
    exam_questions_info->update_data(this->result_doc, this->all_sections_order);

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

    test_info_panel->SetSizer(new wxGridSizer(1, 3, 5, 5));
    test_info_panel->GetSizer()->Add(this->exam_questions_info, 0, wxALIGN_LEFT | wxALIGN_TOP, 10);

    auto* progress_bars_sizer = new wxBoxSizer(wxVERTICAL);
    this->progress_bar_panel = new progressBar(test_info_panel, wxSize(500, 100), this->test_starting_data.duration, { {this->test_starting_data.warning_time, wxColor(154, 133, 48)}, {this->test_starting_data.threshold_time, wxColor(255, 40, 40)} });
    progress_bars_sizer->Add(this->progress_bar_panel, 0, wxEXPAND | wxALIGN_TOP, 7);
    this->warning_time_progress_bar_panel = new progressBar(test_info_panel, wxSize(500, 45), this->test_starting_data.duration - this->test_starting_data.warning_time, {}, wxColor(154, 133, 48), true);
    progress_bars_sizer->Add(this->warning_time_progress_bar_panel, 0, wxEXPAND, 7);
    this->threshold_time_progress_bar_panel = new progressBar(test_info_panel, wxSize(500, 45), this->test_starting_data.duration - this->test_starting_data.threshold_time, {}, wxColor(255, 40, 40), true);
    progress_bars_sizer->Add(this->threshold_time_progress_bar_panel, 0, wxEXPAND | wxALIGN_BOTTOM, 7);
    test_info_panel->GetSizer()->Add(progress_bars_sizer,0, wxEXPAND | wxALIGN_CENTER, 10);


    auto* test_action_sizer = new wxBoxSizer(wxVERTICAL);

    this->prepare_test_actions_buttons();

    test_action_sizer->Add(this->end_button, 0, wxALIGN_RIGHT | wxALIGN_TOP | wxEXPAND, 10);
    test_action_sizer->Add(this->halt_button, 0, wxALIGN_RIGHT | wxALIGN_TOP | wxEXPAND, 10);
    test_action_sizer->Add(this->exam_timer_display, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM, 10);

    test_info_panel->GetSizer()->Add(test_action_sizer, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM, 10);
}

void examFrame::OnTimer(wxTimerEvent &event)
{

    this->exam_remaining_time--;

    this->progress_bar_panel->update(this->test_starting_data.duration - this->exam_remaining_time);
    this->threshold_time_progress_bar_panel->update(this->test_starting_data.duration - this->exam_remaining_time);
    this->warning_time_progress_bar_panel->update(this->test_starting_data.duration - this->exam_remaining_time);
    this->exam_timer_display->SetLabel(formatted_exam_time());

    // Check if the specific time is reached
    if (this->exam_remaining_time <= this->test_starting_data.warning_time + 1)
    {
        this->exam_timer_display->SetForegroundColour(wxColor(154, 133, 48));
    }
    if (this->exam_remaining_time <= this->test_starting_data.threshold_time + 1)
    {
        this->exam_timer_display->SetForegroundColour(wxColor(255, 40, 40));
    }

    // Check if the timer has reached 0
    if (this->exam_remaining_time == 0)
    {
        // Stop the timer
        this->exam_timer->Stop();
        // End the exam
        this->finish_test();
    }
}

void examFrame::on_question_navigated(wxCommandEvent &event)
{

    this->selected_questions.question[this->current_section_order] = event.GetInt();

    data_to_save_in_result question_data = this->question_display_panel->question_data;
    unsigned short int question_row_number = find_row_number(this->result_doc, "section_order", std::to_string(question_data.section_order), "question_number", std::to_string(question_data.question_number));

    if ((question_data.section_order != 0) and (question_data.question_number != 0)) {
    this->result_doc.SetCell<std::string>(get_column_index_by_name(this->result_doc, "question_status"), question_row_number, question_data.question_status);
    this->result_doc.SetCell<std::string>(get_column_index_by_name(this->result_doc, "answer"), question_row_number, question_data.answer);
    this->result_doc.SetCell<float>(get_column_index_by_name(this->result_doc, "time_spent"), question_row_number, this->question_display_panel->get_time_spent_on_current_question() + wxAtof(wxString(this->result_doc.GetCell<std::string>(get_column_index_by_name(this->result_doc, "time_spent"), question_row_number))));
    this->question_display_panel->update_result_doc(this->result_doc);
    this->save_result_doc();
    }

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);
    this->questions_navigation_panel->refresh(this->current_section_order, this->selected_questions.question[this->current_section_order], this->result_doc);
    this->questions_info_panel->update_data(this->result_doc, { this->current_section_order });
    this->exam_questions_info->update_data(this->result_doc, this->all_sections_order);

    this->question_display_panel->enable_next(!((this->selected_questions.question[this->current_section_order] == this->test_starting_data.sections[this->test_starting_data.number_of_sections - 1].number_of_questions) and (this->current_section_order == this->test_starting_data.number_of_sections)));
    this->question_display_panel->enable_previous(!((this->selected_questions.question[this->current_section_order] == 1) and (this->current_section_order == 1)));

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);

}

void examFrame::on_section_navigated(wxCommandEvent& event)
{
 
    this->current_section_order = event.GetInt();

    data_to_save_in_result question_data = this->question_display_panel->question_data;
    unsigned short int question_row_number = find_row_number(this->result_doc, "section_order", std::to_string(question_data.section_order), "question_number", std::to_string(question_data.question_number));

    if ((question_data.section_order != 0) and (question_data.question_number != 0)) {
        this->result_doc.SetCell<std::string>(get_column_index_by_name(this->result_doc, "question_status"), question_row_number, question_data.question_status);
        this->result_doc.SetCell<std::string>(get_column_index_by_name(this->result_doc, "answer"), question_row_number, question_data.answer);
        this->result_doc.SetCell<float>(get_column_index_by_name(this->result_doc, "time_spent"), question_row_number, this->question_display_panel->get_time_spent_on_current_question() + wxAtof(wxString(this->result_doc.GetCell<std::string>(get_column_index_by_name(this->result_doc, "time_spent"), question_row_number))));
        this->question_display_panel->update_result_doc(this->result_doc);
        this->save_result_doc();
    }

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);
    this->questions_navigation_panel->refresh(this->current_section_order, this->selected_questions.question[this->current_section_order], this->result_doc);
    this->questions_info_panel->update_data(this->result_doc, { this->current_section_order });
    this->exam_questions_info->update_data(this->result_doc, this->all_sections_order);

    this->question_display_panel->enable_next(!((this->selected_questions.question[this->current_section_order] == this->test_starting_data.sections[this->test_starting_data.number_of_sections - 1].number_of_questions) and (this->current_section_order == this->test_starting_data.number_of_sections)));
    this->question_display_panel->enable_previous(!((this->selected_questions.question[this->current_section_order] == 1) and (this->current_section_order == 1)));

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);

}

void examFrame::on_options_on_question_display_clicked(wxCommandEvent& event)
{

    wxString button_string = event.GetString();

    if (button_string == "Next >>")
    {
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
    else if (button_string == "<< Previous")
    {
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

    data_to_save_in_result question_data = this->question_display_panel->question_data;
    unsigned short int question_row_number = find_row_number(this->result_doc, "section_order", std::to_string(question_data.section_order), "question_number", std::to_string(question_data.question_number));

    this->result_doc.SetCell<std::string>(get_column_index_by_name(this->result_doc, "question_status"), question_row_number, question_data.question_status);
    this->result_doc.SetCell<std::string>(get_column_index_by_name(this->result_doc, "answer"), question_row_number, question_data.answer);
    this->result_doc.SetCell<float>(get_column_index_by_name(this->result_doc, "time_spent"), question_row_number, question_data.time_spent + wxAtof(wxString(this->result_doc.GetCell<std::string>(get_column_index_by_name(this->result_doc, "time_spent"), question_row_number))));
    this->question_display_panel->update_result_doc(this->result_doc);
    this->save_result_doc();

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);
    this->questions_navigation_panel->refresh(this->current_section_order, this->selected_questions.question[this->current_section_order], this->result_doc);
    this->questions_info_panel->update_data(this->result_doc, { this->current_section_order });
    this->exam_questions_info->update_data(this->result_doc, this->all_sections_order);

    this->question_display_panel->enable_next(!((this->selected_questions.question[this->current_section_order] == this->test_starting_data.sections[this->test_starting_data.number_of_sections - 1].number_of_questions) and (this->current_section_order == this->test_starting_data.number_of_sections)));
    this->question_display_panel->enable_previous(!((this->selected_questions.question[this->current_section_order] == 1) and (this->current_section_order == 1)));

    this->question_display_panel->set_question(this->current_section_order, this->selected_questions.question[this->current_section_order]);
}

void inline examFrame::save_result_doc(void) {

    this->result_doc.Save(this->test_starting_data.student_test_result_file);

}

void inline examFrame::prepare_test_actions_buttons(void) {
    this->end_button = new wxButton(this->test_info_panel, wxID_ANY, "Capitulate");
    this->halt_button = new wxButton(this->test_info_panel, wxID_ANY, "Halt");

    this->end_button->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent& event) {
        SetCursor(wxCursor(wxCURSOR_HAND));
        }
    );
    this->end_button->Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent& event) {
        SetCursor(wxCursor(wxCURSOR_ARROW)); 
        }
    );
    this->halt_button->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent& event) {
        SetCursor(wxCursor(wxCURSOR_HAND)); 
        }
    );
    this->halt_button->Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent& event) {
        SetCursor(wxCursor(wxCURSOR_ARROW)); 
        }
    );


    this->end_button->SetFont(this->end_button->GetFont().Scale(3.3f));
    this->halt_button->SetFont(this->halt_button->GetFont().Scale(3.3f));

    this->end_button->SetBackgroundColour(wxColor(99, 255, 123));
    this->halt_button->SetBackgroundColour(wxColor(135, 178, 237));

    this->end_button->SetToolTip("Submit test");
    this->halt_button->SetToolTip("Pause test for now");

    this->end_button->Bind(wxEVT_BUTTON, &examFrame::capitulate, this);
    this->halt_button->Bind(wxEVT_BUTTON, &examFrame::halt_test, this);

}

void inline examFrame::finish_test(bool times_up) {
    if (times_up)
        wxMessageBox("Test is submited.", "Time's up!", wxOK | wxICON_INFORMATION);
    else if(wxMessageDialog(
                NULL,
                "You won't be able to resume test later\nAre you sure you want to end test?",
                "You sure?",
                wxYES_NO | wxICON_WARNING
            ).ShowModal() == wxID_NO) {
        return; // User clicked "No", don't finish the test
    }

    std::string html_file_location_with_name = this->test_starting_data.student_test_result_file;
    html_file_location_with_name.replace(html_file_location_with_name.find(".csv"), sizeof(".csv") - 1, ".html");
    htmlGenerator().create_test_summary(this->result_doc, html_file_location_with_name, this->test_starting_data);

    html_file_location_with_name = this->test_starting_data.student_test_result_file;
    html_file_location_with_name.replace(html_file_location_with_name.find(".csv"), sizeof(".csv") - 1, "(Detailed Analysis).html"); 
    htmlGenerator().create_detailed_test_analysis(this->result_doc, html_file_location_with_name, this->test_starting_data);
    
    this->Close(true);
}

void inline examFrame::halt_test(wxCommandEvent& event) {
    wxMessageBox("Halting facility will be implemented soon!",
        "No Feature Found", wxOK | wxICON_ERROR);
}
