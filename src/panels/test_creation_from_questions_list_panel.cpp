#include "test_creation_from_questions_list_panel.h"

questionsListForTestCreation::questionsListForTestCreation(wxWindow *parent)
    : wxPanel(parent, wxID_ANY)
{
    this->SetBackgroundColour(*wxRED);
    this->questions_info_grid = new wxGrid(this, wxID_ANY);

    this->questions_info_grid->CreateGrid(0, 3); // 3 columns
    this->questions_info_grid->EnableEditing(false);
    this->questions_info_grid->EnableGridLines(true);
    this->questions_info_grid->SetRowLabelSize(wxGRID_AUTOSIZE);
    this->questions_info_grid->SetColLabelSize(wxGRID_AUTOSIZE);
    this->questions_info_grid->SetColLabelValue(0, "SO, QN");
    this->questions_info_grid->SetColLabelValue(1, "File Name");
    this->questions_info_grid->SetColLabelValue(2, "Section Name");

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(questions_info_grid, 1, wxEXPAND | wxALL, 5);
    SetSizerAndFit(sizer);

   this->add_row("Sample 1", "Sample 2", "Sample 3");
}

void questionsListForTestCreation::add_row(const std::string &col1, const std::string&col2, const std::string&col3)
{
    int row = questions_info_grid->GetNumberRows();
    this->questions_info_grid->AppendRows(1);
    this->questions_info_grid->SetCellValue(row, 0, col1);
    this->questions_info_grid->SetCellValue(row, 1, col2);
    this->questions_info_grid->SetCellValue(row, 2, col3);
}

void questionsListForTestCreation::update_data(const rapidcsv::Document doc)
{
    this->questions_info_grid->DeleteRows(0, this->questions_info_grid->GetNumberRows());

    auto col1_SO_QN = doc.GetColumn<std::string>("SO, QN");
    auto col2_file_name = doc.GetColumn<std::string>("File Name");
    auto col3_sec_name = doc.GetColumn<std::string>("Section Name");

    for (int row = 0; row < col3_sec_name.size(); row++)
        this->add_row(col1_SO_QN[row], col2_file_name[row], col3_sec_name[row]);
}

