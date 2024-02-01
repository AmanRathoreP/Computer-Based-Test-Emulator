#include "section_info_panel.h"

sectionInfoPanel::sectionInfoPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxBOTH);
    sizer->Add(new wxButton(this, wxID_ANY, "Click me FROM SECTION INFO PANEL"), 0, wxALIGN_CENTER | wxALL, 10);
    SetSizer(sizer);
}