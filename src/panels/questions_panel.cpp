#include "questions_panel.h"

questionsPanel::questionsPanel(wxWindow *parent, test_info& test_starting_data) : wxPanel(parent, wxID_ANY), test_starting_data(test_starting_data)
{

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxScrolledWindow *scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxALIGN_TOP);
    scrolledWindow->SetBackgroundColour(wxColor(45, 71, 120));

    wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    for (int i = 1; i <= this->test_starting_data.number_of_sections; ++i)
    {
        wxString buttonLabel = this->test_starting_data.sections[i-1].section_name;
        auto *button = new selectableButton(scrolledWindow, wxID_ANY, buttonLabel);
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