#pragma once
#include "basic_csv_functions.h"
#include <rapidcsv.h>
#include <wx/wx.h>
#include <wx/progdlg.h>
#include <map>

#define SPLITTER "\\//`//\\"

class questionsListForResultCreation_row;

class questionsListForResultCreation : public wxScrolled<wxPanel>
{
public:
    questionsListForResultCreation(wxWindow *, rapidcsv::Document questions_csv);
    rapidcsv::Document get_csv(void) { return this->questions_csv; };

private:
    rapidcsv::Document questions_csv;
    std::vector<questionsListForResultCreation_row*> questions_row;
    void update_real_answer(wxCommandEvent& event);
    
    wxDECLARE_EVENT_TABLE();
};

class questionsListForResultCreation_row : public wxPanel
{
public:
    struct questionData {
        /*
        Q-000583.jpg,1,1,sc,a,523.805,"A,", "REAL ANSWER"
        */
        std::string file_name;
        unsigned short int order;
        unsigned short int number;
        std::string type;
        std::string status;
        float time_spent;
        std::string answer_marked;
        std::string actual_answer;
    };
    questionsListForResultCreation_row(wxWindow *, questionData question_data);

private:
    questionData question_data;
    wxStaticText* file_name;
    wxStaticText* order;
    wxStaticText* number;
    wxChoice* answer_sc;
    wxTextCtrl* answer_fl_in;
};
