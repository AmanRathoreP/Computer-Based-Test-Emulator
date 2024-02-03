#include "questions_panel.h"

wxDEFINE_EVENT(SECTION_NAVIGATED, wxCommandEvent);

wxBEGIN_EVENT_TABLE(questionsPanel, wxPanel)
EVT_BUTTON(wxID_ANY, questionsPanel::on_section_changed)
wxEND_EVENT_TABLE()

questionsPanel::questionsPanel(wxWindow *parent, test_info& test_starting_data) : wxPanel(parent, wxID_ANY), test_starting_data(test_starting_data)
{

    wxPNGHandler* handler = new wxPNGHandler;
    wxImage::AddHandler(handler);

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

    wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* question_number_and_sections_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *question_options_sizer = new wxBoxSizer(wxVERTICAL);

    question_number_and_sections_sizer->Add(this->question_number_text, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    question_number_and_sections_sizer->Add(scrolledWindow, 5, wxALIGN_RIGHT);
    question_options_sizer->Add(question_related_options_sizer, 10, wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL);
    question_options_sizer->Add(questions_navigation_options_sizer, 7, wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL);
    verticalSizer->Add(question_number_and_sections_sizer, 4, wxEXPAND | wxALIGN_TOP | wxALL);
    verticalSizer->Add(question_display_window, 44, wxEXPAND | wxALIGN_CENTER_HORIZONTAL);
    verticalSizer->Add(question_options_sizer, 5, wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL);

    SetSizerAndFit(verticalSizer);
}

void questionsPanel::set_question(unsigned short int section_order, unsigned short int question_number) 
{
    
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

    questions_options_buttons["SAVE & NEXT"]->SetBackgroundColour(wxColour(0, 255, 0));
    questions_options_buttons["SAVE & MARK FOR REVIEW"]->SetBackgroundColour(wxColor(237, 205, 109));
    questions_options_buttons["CLEAR RESPONSE"]->SetBackgroundColour(wxColor(199, 212, 202));
    questions_options_buttons["MARK FOR REVIEW & NEXT"]->SetBackgroundColour(wxColour(128, 0, 128));
    questions_options_buttons["MARK FOR REVIEW & NEXT"]->SetForegroundColour(wxColor(255, 255, 255));
    // questions_options_buttons["<< Previous"]->SetBackgroundColour(wxColor(0, 0, 0));
    // questions_options_buttons["Next >>"]->SetBackgroundColour(wxColor(0, 0, 0));

    float big_font_scaling_factor = 2.3f;
    float small_font_scaling_factor = 1.3f;
    questions_options_buttons["SAVE & NEXT"]->SetFont(questions_options_buttons["SAVE & NEXT"]->GetFont().Scale(big_font_scaling_factor));
    questions_options_buttons["SAVE & MARK FOR REVIEW"]->SetFont(questions_options_buttons["SAVE & MARK FOR REVIEW"]->GetFont().Scale(big_font_scaling_factor));
    questions_options_buttons["CLEAR RESPONSE"]->SetFont(questions_options_buttons["CLEAR RESPONSE"]->GetFont().Scale(big_font_scaling_factor));
    questions_options_buttons["MARK FOR REVIEW & NEXT"]->SetFont(questions_options_buttons["MARK FOR REVIEW & NEXT"]->GetFont().Scale(big_font_scaling_factor));
    questions_options_buttons["<< Previous"]->SetFont(questions_options_buttons["<< Previous"]->GetFont().Scale(small_font_scaling_factor));
    questions_options_buttons["Next >>"]->SetFont(questions_options_buttons["Next >>"]->GetFont().Scale(small_font_scaling_factor));
}

void questionsPanel::update_result_doc(rapidcsv::Document doc) {
    static bool called_for_first_time = true;
    
    this->result_doc = doc;

    if (called_for_first_time) 
    {
        this->set_question(1, 1);
        called_for_first_time = false;
    }

}
