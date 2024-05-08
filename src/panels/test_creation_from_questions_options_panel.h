#pragma once
#include "custom_buttons.h"
#include "test_starting_info.h"
#include "basic_csv_functions.h"
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <array>
#include <fstream>
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
wxDECLARE_EVENT(NEED_CURRENT_SELECTION, wxCommandEvent);

class questionsOptionsForTestCreation : public wxPanel
{
public:
    questionsOptionsForTestCreation(wxWindow *parent);
    rapidcsv::Document question_csv_data;
    std::vector<unsigned short int> __current_rows_selection_from_questions_list_panel;

protected:
    std::vector<std::string> read_files_with_particular_extension(const wxString &folderPath, const wxString &extension);
    wxString format_questions_array_to_string(std::vector<unsigned short int> vec, short int value_to_add_in_each_element = 0);
    std::vector<unsigned short int> get_questions_array_from_string(wxString str, short int value_to_add_in_each_element = 0);
    inline wxString remove_leading_symbols(const wxString& str, const wxChar symbol = ',');
    template<typename T> std::vector<T> drop_elements_with_specific_vals(std::vector<T> vec, const T& value_to_drop);
    inline std::vector<std::string> drop_elements_with_specific_vals(std::vector<std::string> vec, const std::string& value_to_drop = "NaN") { return drop_elements_with_specific_vals<std::string>(vec, value_to_drop); };
    inline bool does_file_exists(const std::string& file_path_and_name) { return wxFileName(file_path_and_name).FileExists(); };

private:
    wxTextCtrl *folder_path_text;
    __timeSelector *threshold_time_panel;
    __timeSelector *warning_time_panel;
    __timeSelector *total_time_panel;
    customButton* preview_button;
    customButton* generate_button;
    wxTextCtrl* test_name;
    wxTextCtrl* test_description;
    __sectionQuestionMapper *section_question_mapper;
    bool __folder_updated = false;

    void OnSelectFolderClicked(wxCommandEvent &event);
    void OnGenerateButtonClicked(wxCommandEvent& event);
    void OnPreviewButtonClicked(wxCommandEvent& event);
    void extract_question_numbers_from_selection(wxCommandEvent &event);
    void try_to_delete_section(wxCommandEvent &event);
    wxString is_same_questions_in_multiple_sections(std::map<unsigned short int, std::vector<unsigned short int>> sections_questions_map);

    wxDECLARE_EVENT_TABLE();
};

class __timeSelector : public wxPanel
{
public:
    __timeSelector(wxWindow *parent, const wxString label, const wxColor background_color);
    inline unsigned int get_total_time_in_seconds(void);

protected:
    wxString format_time(unsigned int seconds);

private:
    wxTextCtrl *leftTextBox;
    wxTextCtrl *rightTextBox;
    wxChoice *choiceBox;
    wxTextCtrl *total_time_display;
    wxStaticText *overall_readable_time_display;
    unsigned int total_time_in_seconds;
    const unsigned short int time_multiplier_values_to_convert_into_seconds[3] = {1, 60, 3600};
    bool change_total_time_display = true;

    void OnQuestionTimeTextBoxChange(wxCommandEvent &event);
};

class __sectionQuestionMapper : public wxPanel
{
public:
    __sectionQuestionMapper(wxWindow *parent);
    void calculate_add_row_button_visibility(void);
    void update_questions_in_section_info(const unsigned short int section_id, const wxString questions);
    void clear_all_questions_in_sections_info(void);
    void drop_row(const unsigned short int section_id);
    std::map<unsigned short int, std::array<wxString, 3>> get_all_data_of_sections(void);

private:
    wxBoxSizer *main_sizer;
    wxBoxSizer *rows_sizer;
    wxButton *add_row_button;
    wxPanel *rows_panel;
    std::vector<std::pair<wxTextCtrl *, wxTextCtrl *>> text_ctrls_; // Vector to store text control pointers
    std::vector<__sectionQuestionMapper_row *> sections;
    unsigned short int last_used_section_id = 1;

    void add_row(const wxString questions, const wxString section_name);
    void add_row(wxCommandEvent &event) { this->add_row("", ""); }
};

class __sectionQuestionMapper_row : public wxPanel
{
public:
    __sectionQuestionMapper_row(wxWindow *parent, unsigned short int section_id);
    std::pair<std::string, std::string> questions_sectionName;
    wxButton *add_current_selection;
    unsigned short int get_section_id(void) { return this->section_id; };
    void set_questions_in_section(const wxString questions) { this->questions_to_include_input->SetValue(questions); }
    wxString get_questions_in_section() { return questions_to_include_input->GetValue(); }
    wxString get_section_name() { return section_name_input->GetValue(); }
    wxString get_question_type(void) { return  wxArrayString{ "sc", "mc", "in", "fl" }.Item(this->question_type->GetSelection()); };

private:
    wxTextCtrl *questions_to_include_input;
    wxTextCtrl *section_name_input;
    wxButton *delete_button;
    wxButton *clear_button;
    unsigned short int section_id = 0;
    wxChoice* question_type;
    const wxArrayString question_types = {"Single Correct", "Multi Correct", "Integers only", "Float"};


    void OnDelete(wxCommandEvent& event);
    void OnAddCurrentSelection(wxCommandEvent &event);
    void OnClear(wxCommandEvent& event);
};
