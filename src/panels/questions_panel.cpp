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

    wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
    verticalSizer->Add(scrolledWindow, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 10);

    SetSizerAndFit(verticalSizer);
}
