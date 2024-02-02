#include "questions_navigation_panel.h"

wxDEFINE_EVENT(QUESTION_NAVIGATED, wxCommandEvent);

wxBEGIN_EVENT_TABLE(questionsNavigationPanel, wxPanel)
EVT_BUTTON(wxID_ANY, questionsNavigationPanel::on_question_changed)
wxEND_EVENT_TABLE()

questionsNavigationPanel::questionsNavigationPanel(wxWindow *parent, unsigned short int number_of_questions, unsigned short int selected_question) : wxPanel(parent, wxID_ANY)
{
    this->grid_sizer = new wxGridSizer(5, 0, 0);
    SetSizer(grid_sizer);

    // Create buttons and add them to the grid sizer
    for (int i = 1; i <= number_of_questions; ++i)
    {
        auto *button = new questionNumberButton(this, wxID_ANY, wxString::Format("%d", i));
        buttons.push_back(button);
        this->grid_sizer->Add(button, 0, wxEXPAND | wxALL, 5);

        if (i == selected_question)
        {
            button->selected(true);
        }
    }

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
