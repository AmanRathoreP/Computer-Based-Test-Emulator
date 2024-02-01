#include "questions_panel.h"

questionsPanel::questionsPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxBOTH);
    sizer->Add(new wxButton(this, wxID_ANY, "Click me FROM QUESTIONS PANEL"), 0, wxALIGN_CENTER | wxALL, 10);
    SetSizer(sizer);
}