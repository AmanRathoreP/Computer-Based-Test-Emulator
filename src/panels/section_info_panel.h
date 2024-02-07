#pragma once
#include <wx/wx.h>
#include <unordered_map>
#include "question_number_button.h"
#include "basic_csv_functions.h"

class sectionInfoPanel : public wxPanel
{
public:
    sectionInfoPanel(wxWindow *);
    void update_data(rapidcsv::Document doc, std::vector<unsigned short int> section_orders);

private:
    std::unordered_map<std::string, questionNumberButton*> button_graphic;
    std::unordered_map<std::string, unsigned short int> questions_info = {
        {"nv", 0},
        {"n", 0},
        {"nr", 0},
        {"ar", 0},
        {"a", 0},
    };
    void inline update_data(void);
};
