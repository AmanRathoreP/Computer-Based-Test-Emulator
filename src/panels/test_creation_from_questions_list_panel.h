#pragma once
#include <rapidcsv.h>
#include <wx/wx.h>
#include <wx/grid.h>

class questionsListForTestCreation : public wxPanel
{
public:
    questionsListForTestCreation(wxWindow*);
    void update_data(const rapidcsv::Document);
    std::vector<unsigned short int> get_selected_rows(void);

private:
    wxGrid* questions_info_grid;
    void add_row(const std::string&, const std::string&, const std::string&);
};
