#include "questions_panel.h"

wxDEFINE_EVENT(SECTION_NAVIGATED, wxCommandEvent);

wxBEGIN_EVENT_TABLE(questionsPanel, wxPanel)
EVT_BUTTON(wxID_ANY, questionsPanel::on_section_changed)
wxEND_EVENT_TABLE()

questionsPanel::questionsPanel(wxWindow *parent, test_info& test_starting_data) : wxPanel(parent, wxID_ANY), test_starting_data(test_starting_data)
{

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

    wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* question_number_and_sections_sizer = new wxBoxSizer(wxHORIZONTAL);
    question_number_and_sections_sizer->Add(this->question_number_text, 1, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    question_number_and_sections_sizer->Add(scrolledWindow, 5, wxALIGN_RIGHT);
    verticalSizer->Add(question_number_and_sections_sizer, 0, wxEXPAND | wxALIGN_TOP | wxALL);

    SetSizerAndFit(verticalSizer);
}

void questionsPanel::set_question(unsigned short int section_order, unsigned short int question_number) 
{
    //display that question
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