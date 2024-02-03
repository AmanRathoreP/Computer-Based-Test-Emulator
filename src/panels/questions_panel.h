#pragma once
#include <wx/wx.h>
#include "test_starting_info.h"
#include "selectable_buttons.h"
#include "basic_csv_functions.h"
#include <unordered_map>
#include <rapidcsv.h>

wxDECLARE_EVENT(SECTION_NAVIGATED, wxCommandEvent);

class questionsPanel : public wxPanel
{
public:
    questionsPanel(wxWindow *, test_info&); 
    void set_question(unsigned short int, unsigned short int);
    void update_result_doc(rapidcsv::Document doc);

private:
        test_info test_starting_data;
        wxStaticText* question_number_text;    
        
        std::vector<selectableButton*> buttons;
        std::unordered_map<std::string, wxButton *> questions_options_buttons;
        void inline prepare_questions_options_buttons(void);

        void on_section_changed(wxCommandEvent &event);

        wxStaticBitmap* current_question_bitmap;
        rapidcsv::Document result_doc;
        wxBoxSizer* question_display_sizer;

        wxDECLARE_EVENT_TABLE();
};

bool inline __compare_by_selection_order(const selectableButton*, const selectableButton*);