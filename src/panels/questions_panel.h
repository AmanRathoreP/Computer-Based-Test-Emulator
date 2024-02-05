#pragma once
#include <wx/wx.h>
#include "test_starting_info.h"
#include "selectable_buttons.h"
#include "basic_csv_functions.h"
#include "custom_buttons.h"
#include "text_input_window.h"
#include <unordered_map>
#include <rapidcsv.h>

wxDECLARE_EVENT(SECTION_NAVIGATED, wxCommandEvent);
wxDECLARE_EVENT(QUESTION_OPTION_CLICKED, wxCommandEvent);

class questionsPanel : public wxPanel
{
public:
    questionsPanel(wxWindow *, test_info&, rapidcsv::Document doc);
    void set_question(unsigned short int, unsigned short int);
    void update_result_doc(rapidcsv::Document doc);
    void enable_next(bool enable = true);
    void enable_previous(bool enable = true);

private:
        test_info test_starting_data;
        wxStaticText* question_number_text;    
        
        std::vector<selectableButton*> buttons;
        std::unordered_map<std::string, wxButton *> questions_options_buttons;
        void inline prepare_questions_options_buttons(void);

        void on_section_changed(wxCommandEvent &event);
        void on_question_option_clicked(wxCommandEvent& event);

        wxStaticBitmap* current_question_bitmap;
        rapidcsv::Document result_doc;
        wxBoxSizer* question_display_sizer;

        void disable_button(const std::string);

        wxBoxSizer* answer_options_sizer;
        std::vector<customButton*> answer_options;
        customTextCtrl* text_input_answer;
        void inline reset_answer_options(void);
        void on_answer_choosed(wxCommandEvent &event);
        unsigned short int current_section_order = 1;
        unsigned short int current_question_number = 1;


        wxDECLARE_EVENT_TABLE();
};

bool inline __compare_by_selection_order(const selectableButton*, const selectableButton*);