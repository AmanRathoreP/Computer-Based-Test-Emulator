#include "create_result_frame.h"

resultCreateFrame::resultCreateFrame(wxFrame *parent, const wxString test_location) : wxFrame(parent, wxID_ANY, "Result creation window", wxDefaultPosition, wxGetDisplaySize(), wxDEFAULT_FRAME_STYLE), path(test_location)
{
    if (this->dirty_path())
        return;

    SetIcon(wxIcon(generate_icon));
    SetBackgroundColour(wxColor(184, 182, 152));

#ifdef __WXMSW__  // Windows
    wxChar __path_separator = '\\';
#else
    wxChar __path_separator = '/';
#endif
    
    this->validity_report_csv_file_path = this->result_csv_file_name;
    this->validity_report_csv_file_path.Replace(" result, ", " validity report, ");
    this->validity_report_csv_file_path = this->path + __path_separator + this->validity_report_csv_file_path;
    this->questions_list = new questionsListForResultCreation(this, rapidcsv::Document((this->path + __path_separator + this->result_csv_file_name).ToStdString(), rapidcsv::LabelParams(0, -1)));
    this->generate_button = new customButton(this, wxID_ANY, "Generate Result", false, 23, 23, wxColor(68, 242, 126));
    this->generate_button->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        if (wxFileName(this->validity_report_csv_file_path).FileExists() && (wxMessageDialog(this->GetParent(),
            "CSV of validity report of questions already exists.\nAre you sure you want to override it?",
            "File already exists!",
            wxYES_NO | wxICON_WARNING).ShowModal() == wxID_NO))
            return;
        else
            this->questions_list->get_csv().Save(this->validity_report_csv_file_path.ToStdString());
        });

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(this->questions_list, 1, wxEXPAND | wxALIGN_TOP, 10);
    mainSizer->Add(this->generate_button, 0, wxEXPAND | wxALIGN_BOTTOM, 50);

    SetSizerAndFit(mainSizer);
}

bool resultCreateFrame::dirty_path(void) {
    wxDir dir(this->path);
    wxString filename;

    bool cont = dir.GetFirst(&filename, "*.csv", wxDIR_FILES);
    while (cont) {
        if (filename.Contains(" result, "))
        {
            this->result_csv_file_name = filename;
            this->validity_report_csv_file_path.Replace(" result, ", " validity report, ");
            return false;
        }
        cont = dir.GetNext(&filename);
    }
    return true;
}
