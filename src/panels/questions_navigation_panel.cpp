#include "questions_navigation_panel.h"

questionsNavigationPanel::questionsNavigationPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxBOTH);
    sizer->Add(new wxButton(this, wxID_ANY, "Click me FROM QUESTION NAVIGATION PANEL"), 0, wxALIGN_CENTER | wxALL, 10);
    SetSizer(sizer);
}