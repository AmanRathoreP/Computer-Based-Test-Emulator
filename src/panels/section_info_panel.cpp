#include "section_info_panel.h"

class __customText : public wxStaticText
{
public:
    __customText(wxWindow *parent, wxWindowID id, const wxString &label,
                       const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, wxColor fore_ground_color = wxColor(0,0,0))
        : wxStaticText(parent, id, label, pos, size)
    {

        SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MAX));
        SetForegroundColour(fore_ground_color);
    }
};

sectionInfoPanel::sectionInfoPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    wxFlexGridSizer *information_table_sizer = new wxFlexGridSizer(3, 3, 5, 5);

    for (const auto question_info : this->questions_info)
        this->button_graphic[question_info.first] = new questionNumberButton(this, wxID_ANY, "0", this->GetBackgroundColour(), question_info.first);

    for (auto &graphic : this->button_graphic)
    {
        graphic.second->Enable(false);
        graphic.second->use_label_value = true;
        graphic.second->Refresh();
    }

    information_table_sizer->Add(this->button_graphic["nv"], 0, wxEXPAND | wxALL);
    information_table_sizer->Add(new __customText(this, wxID_ANY, "Answered", wxDefaultPosition, wxDefaultSize, wxColor(0,255,0)), 0, wxALIGN_CENTER);
    information_table_sizer->Add(new __customText(this, wxID_ANY, "Not Answered", wxDefaultPosition, wxDefaultSize, wxColor(255, 0, 0)), 0, wxALIGN_CENTER);
    information_table_sizer->Add(new __customText(this, wxID_ANY, "Scrutinize", wxDefaultPosition, wxDefaultSize, wxColor(128, 0, 128)), 0, wxALIGN_CENTER);
    information_table_sizer->Add(this->button_graphic["ar"], 0, wxEXPAND | wxALL);
    information_table_sizer->Add(this->button_graphic["nr"], 0, wxEXPAND | wxALL);
    information_table_sizer->Add(new __customText(this, wxID_ANY, "Neglected", wxDefaultPosition, wxDefaultSize), 0, wxALIGN_CENTER);
    information_table_sizer->Add(this->button_graphic["a"], 0, wxEXPAND | wxALL);
    information_table_sizer->Add(this->button_graphic["n"], 0, wxEXPAND | wxALL);

    SetSizer(information_table_sizer);
}

void sectionInfoPanel::update_data(rapidcsv::Document doc, std::vector<unsigned short int> section_orders)
{
    this->questions_info = {
        {"nv", 0},
        {"n", 0},
        {"nr", 0},
        {"ar", 0},
        {"a", 0},
    };

    for (auto section_order : section_orders)
    {
        for (const auto question_info : this->questions_info)
            questions_info[question_info.first] += count_rows_with_specific_values(doc, {"section_order", "question_status"}, {std::to_string(section_order), question_info.first});
    }

    this->update_data();
}

void inline sectionInfoPanel::update_data(void)
{
    for (const auto question_info : this->questions_info)
    {
        this->button_graphic[question_info.first]->label_value = question_info.second;
        this->button_graphic[question_info.first]->Refresh();
    }
}
