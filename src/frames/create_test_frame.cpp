#include "create_test_frame.h"

wxBEGIN_EVENT_TABLE(testCreateFrame, wxFrame)
EVT_COMMAND(wxID_ANY, DATA_UPDATED, testCreateFrame::on_data_updated)
wxEND_EVENT_TABLE()

testCreateFrame::testCreateFrame(wxFrame *parent) : wxFrame(parent, wxID_ANY, "Test creation window", wxDefaultPosition, wxGetDisplaySize(), wxDEFAULT_FRAME_STYLE)
{
    this->questions_options = new questionsOptionsForTestCreation(this);
    this->questions_list = new questionsListForTestCreation(this);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);

    mainSizer->Add(this->questions_list, 5, wxEXPAND | wxALL, 10);
    mainSizer->Add(this->questions_options, 1, wxEXPAND | wxALL, 10);

    SetSizerAndFit(mainSizer);
}
void testCreateFrame::on_data_updated(wxCommandEvent& event){
    this->questions_list->update_data(this->questions_options->question_csv_data);
}
