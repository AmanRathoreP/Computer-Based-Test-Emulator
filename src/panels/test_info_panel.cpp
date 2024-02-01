#include "test_info_panel.h"

testInfoPanel::testInfoPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxBOTH);
    sizer->Add(new wxButton(this, wxID_ANY, "Click me FROM TEST INFO PANEL"), 0, wxALIGN_CENTER | wxALL, 10);
    SetSizer(sizer);
}