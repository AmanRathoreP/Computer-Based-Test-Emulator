#include "create_test_frame.h"

wxBEGIN_EVENT_TABLE(testCreateFrame, wxFrame)
EVT_COMMAND(wxID_ANY, DATA_UPDATED, testCreateFrame::on_data_updated)
EVT_COMMAND(wxID_ANY, NEED_CURRENT_SELECTION, testCreateFrame::on_need_current_selection)
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

void testCreateFrame::on_need_current_selection(wxCommandEvent& event) { 
    this->questions_options->__current_rows_selection_from_questions_list_panel = this->questions_list->get_selected_rows(); 
}
