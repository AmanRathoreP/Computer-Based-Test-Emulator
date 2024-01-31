#pragma once
#include <wx/wx.h>
#include <fstream>
#include <json/json.h>
#include <wx/html/htmlwin.h>

class mainFrame :public wxFrame
{
public:
    mainFrame(const wxString& title);
    enum
    {
        ID_create_test = 1,
        ID_about_author = 2,
        ID_load_test = 3,
    };

    struct test_section_info {
        unsigned short int number_of_questions;
        std::string section_name;
        unsigned short int priority;

        void from_json(const Json::Value& json_data) {
            number_of_questions = json_data.get("number of questions", 0).asInt();
            section_name = json_data.get("section name", "unidentified section").asString();
            priority = json_data.get("section order", 0).asInt();
        }
    };

    struct test_info {
        unsigned int duration; // in seconds
        std::string test_name;
        std::string test_description;

        unsigned short int number_of_sections;
        test_section_info* sections;
        void from_json(const Json::Value& json_data) {
            number_of_sections = json_data.get("total number of sections", 0).asUInt();
            sections = new test_section_info[number_of_sections];
            duration = json_data.get("duration", 0).asUInt();
            test_name = json_data.get("test name", "no name provided").asString();
            test_description = json_data.get("test description", "no test description provided").asString();

            // Deserialize the array of sections
            const Json::Value& __sections = json_data["sections"];
            for (unsigned short int i = 0; i < number_of_sections; ++i) {
                sections[i].from_json(__sections[i]);
            }
        }
    };
    
    struct test_info test_starting_data;

private:
    void on_exit_clicked(wxCommandEvent& event);
    void on_about_clicked(wxCommandEvent& event);
    void on_about_author_clicked(wxCommandEvent& event);
    void on_create_test_clicked(wxCommandEvent& event);
    void on_load_test_clicked(wxCommandEvent& event);
    void check_weather_all_ther_required_files_are_avaliable_in_folder(const wxString&);
    bool check_weather_test_file_is_fit(const wxString&);
    unsigned short int read_json_file(const std::string&);
    std::string generate_html_to_display_test_info();

    wxHtmlWindow* test_info_display;
    wxButton* start_test_button;
};

