#pragma once
#include "exam_frame.h"
#include "create_test_frame.h"
#include "test_starting_info.h"
#include "create_result_frame.h"
#include <wx/wx.h>
#include "basic_functions.h"
#include "../constants/graphics/app_icons.h"
#include <fstream>
#include <json/json.h>
#include <wx/html/htmlwin.h>
#include <rapidcsv.h>
#include <unordered_set>

class mainFrame :public wxFrame
{
public:
    mainFrame(const wxString& title);
    enum
    {
        ID_create_test = 1,
        ID_about_author = 2,
        ID_load_test = 3,
        ID_resume_test = 4,
        ID_create_result = 5,
    };

    struct test_info test_starting_data;

private:
    void on_exit_clicked(wxCommandEvent& event);
    void on_about_clicked(wxCommandEvent& event);
    void on_about_author_clicked(wxCommandEvent& event);
    void on_create_test_clicked(wxCommandEvent& event);
    void on_create_result_clicked(wxCommandEvent& event);
    void on_load_test_clicked(wxCommandEvent& event);
    void on_resume_test_clicked(wxCommandEvent& event);
    void check_weather_all_ther_required_files_are_avaliable_in_folder(const wxString&);
    bool check_weather_test_file_is_fit(const wxString&, const wxString&);
    unsigned short int read_json_file(const std::string&);
    unsigned short int read_csv_file(const std::string&);
    std::string generate_html_to_display_test_info();
    void on_start_test_clicked(wxCommandEvent& event);

    wxHtmlWindow* test_info_display;
    wxButton* start_test_button;
};

