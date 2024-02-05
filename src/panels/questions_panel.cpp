#include "questions_panel.h"

wxDEFINE_EVENT(SECTION_NAVIGATED, wxCommandEvent);
wxDEFINE_EVENT(QUESTION_OPTION_CLICKED, wxCommandEvent);

wxBEGIN_EVENT_TABLE(questionsPanel, wxPanel)
EVT_BUTTON(wxID_ANY, questionsPanel::on_section_changed)
EVT_BUTTON(wxID_ANY, questionsPanel::on_answer_choosed)
wxEND_EVENT_TABLE()

questionsPanel::questionsPanel(wxWindow *parent, test_info& test_starting_data, rapidcsv::Document doc) : wxPanel(parent, wxID_ANY), test_starting_data(test_starting_data)
{

    wxPNGHandler* handler = new wxPNGHandler;
    wxImage::AddHandler(handler);

    this->result_doc = doc;

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxScrolledWindow *scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxALIGN_TOP);
    scrolledWindow->SetBackgroundColour(wxColor(45, 71, 120));

    wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    for (int i = 1; i <= this->test_starting_data.number_of_sections; ++i)
    {
        auto *button = new selectableButton(scrolledWindow, wxID_ANY, this->test_starting_data.sections[i - 1].section_name, this->test_starting_data.sections[i - 1].priority);
        buttons.push_back(button);
    }
    std::sort(this->buttons.begin(), this->buttons.end(), __compare_by_selection_order);
    this->buttons.front()->set_selected(true);
    for (const auto& button : buttons) {
        horizontalSizer->Add(button, 0, wxALIGN_CENTER | wxALL, 10);
    }

    scrolledWindow->SetSizer(horizontalSizer);
    scrolledWindow->SetScrollRate(10, 0); // Set horizontal scroll rate

    // Create a wxStaticText for displaying the question number
    this->question_number_text = new wxStaticText(this, wxID_ANY, "Question: 01", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT | wxST_NO_AUTORESIZE);
    this->question_number_text->SetFont(wxFont(27, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    wxBoxSizer *question_related_options_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *questions_navigation_options_sizer = new wxBoxSizer(wxHORIZONTAL);

    this->prepare_questions_options_buttons();

    question_related_options_sizer->Add(questions_options_buttons["SAVE & NEXT"], 11, wxALIGN_CENTER_VERTICAL);
    question_related_options_sizer->Add(questions_options_buttons["SAVE & MARK FOR REVIEW"], 22, wxALIGN_CENTER_VERTICAL);
    question_related_options_sizer->Add(questions_options_buttons["CLEAR RESPONSE"], 16, wxALIGN_CENTER_VERTICAL);
    question_related_options_sizer->Add(questions_options_buttons["MARK FOR REVIEW & NEXT"], 22, wxALIGN_CENTER_VERTICAL);

    questions_navigation_options_sizer->Add(questions_options_buttons["<< Previous"], 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT);
    questions_navigation_options_sizer->Add(questions_options_buttons["Next >>"], 1, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);

    wxScrolledWindow *question_display_window = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
   this->question_display_sizer = new wxBoxSizer(wxVERTICAL);

    question_display_window->SetBackgroundColour(wxColor(255, 100, 76));
    
    this->current_question_bitmap = new wxStaticBitmap(question_display_window, 
        wxID_ANY, 
        wxBitmap(), //empty bitmap
        wxPoint(0, 0), 
        wxSize(500, 500));

    question_display_sizer->Add(this->current_question_bitmap);
    question_display_window->SetSizer(question_display_sizer);
    question_display_window->SetScrollRate(0, 20);

    this->answer_options_sizer = new wxBoxSizer(wxHORIZONTAL);
    this->text_input_answer = new customTextCtrl(this, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(143, 39), 0);
    this->text_input_answer->SetHint("Ans...");
    this->text_input_answer->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    this->text_input_answer->Bind(wxEVT_TEXT, [this](wxCommandEvent& event) {
        this->update_question_options();
        });
    this->answer_options_sizer->Add(this->text_input_answer, 0, wxALIGN_CENTER_VERTICAL);
    
    std::vector<char> options = { 'A', 'B', 'C', 'D' };
    for (unsigned short int i = 0; i < options.size(); i++)
    {
        this->answer_options.push_back(new customButton(this, wxID_ANY, options[i], true, 29, 35));
        this->answer_options_sizer->Add(this->answer_options[i], 0, wxALIGN_BOTTOM);
    }


    wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* question_number_and_sections_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *question_options_sizer = new wxBoxSizer(wxVERTICAL);

    question_number_and_sections_sizer->Add(this->question_number_text, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    question_number_and_sections_sizer->Add(scrolledWindow, 5, wxALIGN_RIGHT);
    question_options_sizer->Add(this->answer_options_sizer, 14, wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL);
    question_options_sizer->Add(question_related_options_sizer, 10, wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL);
    question_options_sizer->Add(questions_navigation_options_sizer, 7, wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL);
    verticalSizer->Add(question_number_and_sections_sizer, 4, wxEXPAND | wxALIGN_TOP | wxALL);
    verticalSizer->Add(question_display_window, 40, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
    verticalSizer->Add(question_options_sizer, 9, wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL);

    SetSizerAndFit(verticalSizer);
    this->set_question(1, 1);
}

void questionsPanel::set_question(unsigned short int section_order, unsigned short int question_number) 
{
    this->question_time_tracker_timer = std::chrono::high_resolution_clock::now();
 
    this->current_section_order = section_order;
    this->current_question_number = question_number;

    unsigned short int question_row_number = find_row_number(this->result_doc, "section_order", std::to_string(section_order), "question_number", std::to_string(question_number));

    wxImage question_image;
    question_image.LoadFile(
        wxString(this->test_starting_data.student_test_result_file).Truncate(this->test_starting_data.student_test_result_file.length() - wxString("result.csv").length()).append(this->result_doc.GetCell<std::string>(0, question_row_number))
    );

    unsigned short int max_width = this->question_display_sizer->GetSize().GetWidth() * 0.95;
    unsigned short int max_height = this->question_display_sizer->GetSize().GetHeight() * 0.95;
    question_image = question_image.Scale(max_width, question_image.GetHeight() * (static_cast<float>(max_width) / question_image.GetWidth()));
    if (question_image.GetHeight() > max_height)
        question_image = question_image.Scale(max_width * (static_cast<float>(max_height) / question_image.GetHeight()), max_height);
        
    this->current_question_bitmap->SetSize(question_image.GetSize());
    this->current_question_bitmap->SetBitmap(wxBitmap(question_image));


    this->question_number_text->SetLabel(wxString::Format("Question: %02d", question_number));

    for (auto* button : buttons) {
        button->set_selected(false);
        if (button->section_order == section_order) {
            button->set_selected(true);
        }
    }

    wxString previous_answer = wxString(this->result_doc.GetCell<std::string>(get_column_index_by_name(this->result_doc, "answer"), question_row_number));
    this->question_data.answer = previous_answer.ToStdString();
    this->question_data.section_order = this->current_section_order;
    this->question_data.question_number = this->current_question_number;
    this->question_data.question_status = this->result_doc.GetCell<std::string>(get_column_index_by_name(this->result_doc, "question_status"), question_row_number);
    if (this->question_data.question_status == "nv")
        this->question_data.question_status = "n";
    std::string question_type = this->result_doc.GetCell<std::string>(3, find_row_number(this->result_doc, "section_order", std::to_string(this->current_section_order), "question_number", std::to_string(this->current_question_number)));
    if (question_type == "in") {
        //display entry box for integers
        for (auto* option : answer_options) {
            option->Show(false);
        }
        this->text_input_answer->allow_float(false);
        this->text_input_answer->Show(true);
        if (previous_answer != "NaN")
        {
            this->text_input_answer->SetValue(previous_answer);
        }
    }
    else if (question_type == "fl") {
        //display entry box for floats
        for (auto* option : answer_options) {
            option->Show(false);
        }
        this->text_input_answer->allow_float();
        this->text_input_answer->Show(true);
        if (previous_answer != "NaN")
        {
            this->text_input_answer->SetValue(previous_answer);
        }
    }
    else if (question_type == "sc") {
        //single correct
        this->reset_answer_options();
        for (auto* option : answer_options) {
            option->Show(true);
            if ((previous_answer != "NaN") and (previous_answer.Find(option->GetLabel()) != wxNOT_FOUND))
            {
                option->set_selected();
            }
        }
        this->text_input_answer->Clear();
        this->text_input_answer->Show(false);
    }
    else if (question_type == "mc") {
        //multi correct
        this->reset_answer_options();
        for (auto* option : answer_options) {
            option->Show(true);
            if ((previous_answer != "NaN") and (previous_answer.Find(option->GetLabel()) != wxNOT_FOUND))
            {
                option->set_selected();
            }
        }
        this->text_input_answer->Clear();
        this->text_input_answer->Show(false);
    }
    
    this->update_question_options();
}

void questionsPanel::on_section_changed(wxCommandEvent& event) {
    auto* pressed_button = dynamic_cast<selectableButton*>(event.GetEventObject());
    if (pressed_button) {
        unsigned short int button_number = pressed_button->section_order;

        // Set the clicked button to selected (true)
        pressed_button->set_selected(true);

        // Set all other buttons to selected (false)
        for (auto* button : buttons) {
            if (button->section_order != pressed_button->section_order) {
                button->set_selected(false);
            }
        }

        wxCommandEvent custom_event(SECTION_NAVIGATED, GetId());
        custom_event.SetInt(button_number);
        GetEventHandler()->ProcessEvent(custom_event);
    }
    event.Skip();
}

bool inline __compare_by_selection_order(const selectableButton* a, const selectableButton* b) {
    return a->section_order < b->section_order;
}

void inline questionsPanel::prepare_questions_options_buttons(void)
{
    questions_options_buttons["SAVE & NEXT"] = new wxButton(this, wxID_ANY, "SAVE && NEXT");
    questions_options_buttons["SAVE & MARK FOR REVIEW"] = new wxButton(this, wxID_ANY, "SAVE && MARK FOR REVIEW");
    questions_options_buttons["CLEAR RESPONSE"] = new wxButton(this, wxID_ANY, "CLEAR RESPONSE");
    questions_options_buttons["MARK FOR REVIEW & NEXT"] = new wxButton(this, wxID_ANY, "MARK FOR REVIEW && NEXT");
    questions_options_buttons["<< Previous"] = new wxButton(this, wxID_ANY, "<< Previous");
    questions_options_buttons["Next >>"] = new wxButton(this, wxID_ANY, "Next >>");

    for (const auto& button : questions_options_buttons)
    {
        button.second->Bind(wxEVT_BUTTON, &questionsPanel::on_question_option_clicked, this);
    }

    questions_options_buttons["SAVE & NEXT"]->SetBackgroundColour(wxColour(0, 255, 0));
    questions_options_buttons["SAVE & MARK FOR REVIEW"]->SetBackgroundColour(wxColor(237, 205, 109));
    questions_options_buttons["CLEAR RESPONSE"]->SetBackgroundColour(wxColor(199, 212, 202));
    questions_options_buttons["MARK FOR REVIEW & NEXT"]->SetBackgroundColour(wxColour(128, 0, 128));
    questions_options_buttons["MARK FOR REVIEW & NEXT"]->SetForegroundColour(wxColor(255, 255, 255));

    float big_font_scaling_factor = 2.3f;
    float small_font_scaling_factor = 1.3f;
    questions_options_buttons["SAVE & NEXT"]->SetFont(questions_options_buttons["SAVE & NEXT"]->GetFont().Scale(big_font_scaling_factor));
    questions_options_buttons["SAVE & MARK FOR REVIEW"]->SetFont(questions_options_buttons["SAVE & MARK FOR REVIEW"]->GetFont().Scale(big_font_scaling_factor));
    questions_options_buttons["CLEAR RESPONSE"]->SetFont(questions_options_buttons["CLEAR RESPONSE"]->GetFont().Scale(big_font_scaling_factor));
    questions_options_buttons["MARK FOR REVIEW & NEXT"]->SetFont(questions_options_buttons["MARK FOR REVIEW & NEXT"]->GetFont().Scale(big_font_scaling_factor));
    questions_options_buttons["<< Previous"]->SetFont(questions_options_buttons["<< Previous"]->GetFont().Scale(small_font_scaling_factor));
    questions_options_buttons["Next >>"]->SetFont(questions_options_buttons["Next >>"]->GetFont().Scale(small_font_scaling_factor));

    for (auto& pair : this->questions_options_buttons) {
        pair.second->Bind(wxEVT_ENTER_WINDOW, [this](wxMouseEvent& event) {
            SetCursor(wxCursor(wxCURSOR_HAND));
            }
        );
        pair.second->Bind(wxEVT_LEAVE_WINDOW, [this](wxMouseEvent& event) {
            SetCursor(wxCursor(wxCURSOR_ARROW));
            }
        );
    }

    //TODO deal with the case where only one question is there in the test
    this->enable_previous(false);
}

void questionsPanel::update_result_doc(rapidcsv::Document doc) {
    this->result_doc = doc;
}

void questionsPanel::on_question_option_clicked(wxCommandEvent& event)
{
    auto* pressed_button = dynamic_cast<wxButton*>(event.GetEventObject());
    if (pressed_button)
    {
        this->question_data.section_order = this->current_section_order;
        this->question_data.question_number = this->current_question_number;
        
        wxString button_name = pressed_button->GetLabel();

        if (button_name == "SAVE && NEXT")
        {
            // button only enabled when atleast one answer is provided
            this->question_data.answer = get_answer_of_current_question();
            this->question_data.question_status = "a";
        }
        else if (button_name == "SAVE && MARK FOR REVIEW")
        {
            // button only enabled when atleast one answer is provided
            this->question_data.answer = get_answer_of_current_question();
            this->question_data.question_status = "ar";
        }
        else if (button_name == "CLEAR RESPONSE")
        {
            this->question_data.question_status = "n";
            this->question_data.answer = "NaN";
            this->set_question(this->current_section_order, this->current_question_number);
        }
        else if (button_name == "MARK FOR REVIEW && NEXT")
        {
            // button only enabled when no answer is provided
            this->question_data.answer = "NaN";
            this->question_data.question_status = "nr";
        }
        this->question_data.time_spent = this->get_time_spent_on_current_question();

        // Notify any listeners about the button click
        wxCommandEvent custom_event(QUESTION_OPTION_CLICKED, GetId());
        custom_event.SetString(button_name); // Set additional data if needed
        GetEventHandler()->ProcessEvent(custom_event);

        // Pretending like next is pressed so that we don't need to handle too much stuff in exam frame
        if ((button_name == "SAVE && NEXT") or (button_name == "MARK FOR REVIEW && NEXT") or (button_name == "SAVE && MARK FOR REVIEW")){
            custom_event.SetString("Next >>"); // Set additional data if needed
            GetEventHandler()->ProcessEvent(custom_event);
        }
    }
    event.Skip();
}

void questionsPanel::enable_next(bool enable){
    questions_options_buttons["Next >>"]->Enable(enable);
}

void  questionsPanel::enable_previous(bool enable){
    questions_options_buttons["<< Previous"]->Enable(enable);
}

void inline questionsPanel::reset_answer_options(void){
    for (auto* button : this->answer_options) {
        button->set_selected(false);
    }
}

void questionsPanel::on_answer_choosed(wxCommandEvent& event) {
    auto* pressed_button = dynamic_cast<customButton*>(event.GetEventObject());
    if (pressed_button) {
       //checking if question is single correct
        if ("sc" == this->result_doc.GetCell<std::string>(3, find_row_number(this->result_doc, "section_order", std::to_string(this->current_section_order), "question_number", std::to_string(this->current_question_number)))) {
            for (auto* button : this->answer_options) {
                button->set_selected(false);
            }
        }
        pressed_button->set_selected(not pressed_button->current_state);
        this->update_question_options();
    }
    event.Skip();
}

std::string questionsPanel::get_answer_of_current_question(void)
{
    wxString answer = "";
    for (auto *button : this->answer_options)
    {
        if (button->current_state)
            answer.append(button->GetLabel()).append(',');
    }

    answer.append(this->text_input_answer->GetValue());

    return answer.ToStdString();
}

void questionsPanel::update_question_options(void){
    bool atleast_one_answer_provided = false;

    for (auto* button : this->answer_options)
        atleast_one_answer_provided = atleast_one_answer_provided || button->current_state;

    atleast_one_answer_provided = atleast_one_answer_provided || !this->text_input_answer->GetValue().empty();

    questions_options_buttons["SAVE & NEXT"]->Enable(atleast_one_answer_provided);
    questions_options_buttons["SAVE & MARK FOR REVIEW"]->Enable(atleast_one_answer_provided);
    questions_options_buttons["MARK FOR REVIEW & NEXT"]->Enable(!atleast_one_answer_provided);
}

float questionsPanel::get_time_spent_on_current_question(void) {
    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->question_time_tracker_timer).count();
    return static_cast<float>(elapsedMilliseconds) / 1000.0f;
}
