#include "questions_navigation_panel.h"

wxDEFINE_EVENT(QUESTION_NAVIGATED, wxCommandEvent);

wxBEGIN_EVENT_TABLE(questionsNavigationPanel, wxPanel)
EVT_BUTTON(wxID_ANY, questionsNavigationPanel::on_question_changed)
wxEND_EVENT_TABLE()

questionsNavigationPanel::questionsNavigationPanel(wxWindow* parent, unsigned short int section_order, unsigned short int selected_question, rapidcsv::Document doc, unsigned short int number_of_sections) : wxPanel(parent, wxID_ANY)
{
    this->grid_sizer = new wxGridSizer(7, 5, 0, 0);
    SetSizer(grid_sizer);

    unsigned short int max_number_of_questions_per_section = 0;

    for (unsigned short int section_number = 1; section_number <= number_of_sections; section_number++) {
    std::vector<std::string> columnNames = {"section_order"};
    std::vector<std::string> targetValues = {std::to_string(section_number)};
    if (max_number_of_questions_per_section < count_rows_with_specific_values(doc, columnNames, targetValues))
        max_number_of_questions_per_section = count_rows_with_specific_values(doc, columnNames, targetValues);
    }
    
        // Create buttons and add them to the grid sizer
        for (int i = 1; i <= max_number_of_questions_per_section; i++)
        {
            auto* button = new questionNumberButton(this, wxID_ANY, wxString::Format("%d", i));
            buttons.push_back(button);
            this->grid_sizer->Add(button, 0, wxEXPAND | wxALL, 5);
    }

    this->refresh(section_order, selected_question, doc);

    // Make the panel scrollable
    // TODO make it scrollable
    wxScrolledWindow *scrolled_window = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    scrolled_window->SetSizer(this->grid_sizer);
    scrolled_window->SetScrollRate(20, 20); // Set scroll rate as needed
}

void questionsNavigationPanel::on_question_changed(wxCommandEvent& event) {
    auto* pressed_button = dynamic_cast<questionNumberButton*>(event.GetEventObject());
    if (pressed_button) {
        int button_number = wxAtoi(pressed_button->GetLabel());

        // Set the clicked button to selected (true)
        pressed_button->selected(true);

        // Set all other buttons to selected (false)
        for (auto* button : buttons) {
            if (button != pressed_button) {
                button->selected(false);
            }
        }

        wxCommandEvent custom_event(QUESTION_NAVIGATED, GetId());
        custom_event.SetInt(button_number);
        GetEventHandler()->ProcessEvent(custom_event);
    }
    event.Skip();
}

void questionsNavigationPanel::refresh(unsigned short int section_order, unsigned short int selected_question, rapidcsv::Document doc) {
    
    std::vector<std::string> columnNames = { "section_order" };
    std::vector<std::string> targetValues = { std::to_string(section_order) };

    int number_of_questions = count_rows_with_specific_values(doc, columnNames, targetValues);

    unsigned short int visible_buttons = 0;
    for (auto* button : buttons) {
        if (button->IsShown())
            visible_buttons += 1;
    }

    if (visible_buttons > number_of_questions) {
        for (unsigned short int i = visible_buttons; i > number_of_questions; i--)
        {
            /*
            this->grid_sizer->Remove(wxAtoi(buttons[i - 1]->GetLabel()) - 1);
            buttons.erase(buttons.begin() + i - 1);
             buttons[i - 1]->Destroy();
             */
            buttons[i - 1]->Show(false);
        }
    }
    else if ((visible_buttons < number_of_questions)) {
        for (unsigned short int i = visible_buttons; i < number_of_questions; i++)
        {
            /*
         auto* button = new questionNumberButton(this, wxID_ANY, wxString::Format("%d", i));
            buttons.push_back(button);
        this->grid_sizer->Add(button, 0, wxEXPAND | wxALL, 5);
        */
            buttons[i]->Show();
        }
    }

    for (auto* button : buttons) {
        //TODO also check state of the question and update accordingly
        if (wxAtoi(button->GetLabel()) == selected_question)
            button->selected(true);
        else
            button->selected(false);
    }
}
