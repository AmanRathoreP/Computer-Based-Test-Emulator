#pragma once
#include "custom_buttons.h"
#include <vector>
#include <rapidcsv.h>
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/dir.h>
#include <wx/filename.h>

#define MAX_ROWS 12

class __timeSelector;
class __sectionQuestionMapper;
class __sectionQuestionMapper_row;

wxDECLARE_EVENT(wxEVT_ADD_ROW, wxCommandEvent);
wxDECLARE_EVENT(DATA_UPDATED, wxCommandEvent);

class questionsOptionsForTestCreation : public wxPanel
{
public:
    questionsOptionsForTestCreation(wxWindow *parent);
    rapidcsv::Document question_csv_data;

protected:
    std::vector<std::string> read_files_with_particular_extension(const wxString &folderPath, const wxString &extension);

private:
    wxTextCtrl *folder_path_text;
    __timeSelector *threshold_time_panel;
    __timeSelector *warning_time_panel;
    __timeSelector *total_time_panel;
    customButton* preview_button;
    customButton* generate_button;
    wxTextCtrl* test_name;
    wxTextCtrl* test_description;

    void OnSelectFolderClicked(wxCommandEvent &event);
    void OnGenerateButtonClicked(wxCommandEvent& event);
    void OnPreviewButtonClicked(wxCommandEvent& event);
};

class __timeSelector : public wxPanel
{
public:
    __timeSelector(wxWindow *parent, const wxString label, const wxColor background_color);
    unsigned int total_time_in_seconds;

protected:
    wxString format_time(unsigned int seconds);

private:
    wxTextCtrl *leftTextBox;
    wxTextCtrl *rightTextBox;
    wxChoice *choiceBox;
    wxTextCtrl *total_time_display;
    wxStaticText *overall_readable_time_display;
    const unsigned short int time_multiplier_values_to_convert_into_seconds[3] = {1, 60, 3600};
    bool change_total_time_display = true;

    void OnQuestionTimeTextBoxChange(wxCommandEvent &event);
};

class __sectionQuestionMapper : public wxPanel
{
public:
    __sectionQuestionMapper(wxWindow *parent);
    void calculate_add_row_button_visibility(void);

private:
    wxBoxSizer *main_sizer;
    wxBoxSizer *rows_sizer;
    wxButton *add_row_button;
    wxPanel *rows_panel;
    std::vector<std::pair<wxTextCtrl *, wxTextCtrl *>> text_ctrls_; // Vector to store text control pointers

    void add_row(const wxString questions, const wxString section_name);
    void add_row(wxCommandEvent &event) { this->add_row("", ""); }
};

class __sectionQuestionMapper_row : public wxPanel
{
public:
    __sectionQuestionMapper_row(wxWindow *parent);
    std::pair<std::string, std::string> questions_sectionName;

private:
    wxTextCtrl *questions_to_include_input;
    wxTextCtrl *section_name_input;
    wxButton *add_current_selection;
    wxButton *delete_button;
    
    void OnDelete(wxCommandEvent& event);
};