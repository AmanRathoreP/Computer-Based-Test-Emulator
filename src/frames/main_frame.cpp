#include "main_frame.h"

mainFrame::mainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxMenu* menu_advance = new wxMenu;
    menu_advance->Append(ID_load_test, "&Load Test\tCtrl+L");
    menu_advance->Append(ID_create_test, "&Create Test\tCtrl+N");
    menu_advance->AppendSeparator();
    menu_advance->Append(wxID_EXIT, "&Close\tCtrl+W");

    wxMenu* menu_help = new wxMenu;
    menu_help->Append(wxID_ABOUT);
    menu_help->Append(ID_about_author, "About Author");

    wxMenuBar* menu_bar = new wxMenuBar;
    menu_bar->Append(menu_advance, "&Options");
    menu_bar->Append(menu_help, "&Help");

    SetMenuBar(menu_bar);

    // Create an HTML view
    this->test_info_display = new wxHtmlWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    this->start_test_button = new wxButton(this, wxID_ANY, "Start Test");

    wxFont __button_font = this->start_test_button->GetFont();
    __button_font.SetPointSize(19); // Change the desired font size
    this->start_test_button->SetFont(__button_font);

    this->start_test_button->Enable(false);

    wxBoxSizer *__v_box_sizer = new wxBoxSizer(wxVERTICAL);
    __v_box_sizer->Add(this->test_info_display, 7, wxEXPAND | wxALL, 5);

    // Add top and bottom margins to the button
    __v_box_sizer->AddSpacer(16); // Top margin
    __v_box_sizer->Add(this->start_test_button, 3, wxEXPAND | wxALIGN_CENTER_HORIZONTAL, 5);
    __v_box_sizer->AddSpacer(16); // Bottom margin

    // Use HTML and CSS to center-align the text within the wxHtmlWindow
    // TODO html tags not working
    this->test_info_display->SetPage("<html><body><h1>Load test via <i>Options > Load Test </i>!</h1><br><h1>Create new test via <i>Options > Create Test </i>!</h1></body></html>");

    SetSizerAndFit(__v_box_sizer);

    Bind(wxEVT_MENU, &mainFrame::on_about_clicked, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &mainFrame::on_about_author_clicked, this, ID_about_author);
    Bind(wxEVT_MENU, &mainFrame::on_exit_clicked, this, wxID_EXIT);
    Bind(wxEVT_MENU, &mainFrame::on_create_test_clicked, this, ID_create_test);
    Bind(wxEVT_MENU, &mainFrame::on_load_test_clicked, this, ID_load_test);
}

void mainFrame::on_exit_clicked(wxCommandEvent& event)
{
    Close(true);
}

void mainFrame::on_about_clicked(wxCommandEvent& event)
{
    wxMessageBox("This apps helps one to create and emulate test for sake of practicing",
        "About CBT Emulator", wxOK | wxICON_INFORMATION);
}

void mainFrame::on_about_author_clicked(wxCommandEvent& event)
{
    wxMessageBox("This app is created by Aman whose github id is AmanRathoreP",
        "About Author", wxOK | wxICON_INFORMATION);
}

void mainFrame::on_create_test_clicked(wxCommandEvent& event)
{
    wxMessageBox("The facility will be implemented soon!",
        "Test Creation", wxOK | wxICON_WARNING);
}

void mainFrame::on_load_test_clicked(wxCommandEvent &event)
{
    wxDirDialog dlg(this, "Choose a folder", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxString folder_path = dlg.GetPath();
        this->check_weather_all_ther_required_files_are_avaliable_in_folder(folder_path);
    }
}

void mainFrame::check_weather_all_ther_required_files_are_avaliable_in_folder(const wxString &folder_path)
{

    wxString file_path = wxString(folder_path).append("\\test_info.json");
    if (wxFileExists(file_path))
    {
        check_weather_test_file_is_fit(file_path);
    }
    else
    {
        wxMessageBox("Crucial file to start the test is missing!\nPlease contact the test provider for more info.", "Error", wxOK | wxICON_ERROR, this);
    }
}

unsigned short int mainFrame::read_json_file(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        return 1; // Return 1 on failure to read
    }

    Json::CharReaderBuilder builder;
    Json::CharReader *reader = builder.newCharReader();

    Json::Value json_data;
    std::string errs;

    Json::parseFromStream(builder, file, &json_data, &errs);
    file.close();

    if (!errs.empty())
    {
        return 2; // Return 2 on parsing error
    }

    test_starting_data.from_json(json_data);
    return 3; // Data reading successfully completed
}

bool mainFrame::check_weather_test_file_is_fit(const wxString &test_file_location)
{
    auto test_file_content = this->read_json_file(test_file_location.ToStdString());
    if (test_file_content == 1)
    {
        // failure to read
        wxMessageBox("Unable to read the test file check weather required permission are granted or not.", "Reading Error", wxOK | wxICON_ERROR, this);
        return false;
    }
    if (test_file_content == 2)
    {
        // parsing error
        wxMessageBox("Unable to parse the file!\nTest file may be corrupted.", "Parsing Error", wxOK | wxICON_ERROR, this);
        return false;
    }
    else
    {
        /*
        std::string result;
        result.append("Duration: ").append(std::to_string(test_starting_data.duration)).append(" seconds\n");
        result.append("Test Name: ").append(test_starting_data.test_name).append("\n");
        result.append("Number of Sections: ").append(std::to_string(test_starting_data.number_of_sections)).append("\n\n");

        for (unsigned short int i = 0; i < test_starting_data.number_of_sections; ++i)
        {
            result.append("Section ").append(std::to_string(i + 1)).append(":\n");
            result.append("   Number of Questions: ").append(std::to_string(test_starting_data.sections[i].number_of_questions)).append("\n");
            result.append("   Section Name: ").append(test_starting_data.sections[i].section_name).append("\n");
            result.append("   Priority: ").append(std::to_string(test_starting_data.sections[i].priority)).append("\n");
        }
        wxMessageBox(result, "JSON information", wxOK | wxICON_INFORMATION, this);
        */

        this->start_test_button->Enable(true);
        this->test_info_display->SetPage(this->generate_html_to_display_test_info());

        return true;
    }
}

std::string mainFrame::generate_html_to_display_test_info()
{
    std::stringstream __html_stream;

    // HTML header
    __html_stream << "<html><head><title>Test Information</title></head><body>";

    // Display test information
    __html_stream << "<h1>" << this->test_starting_data.test_name << "</h1>";
    __html_stream << "<p>Duration: " << this->test_starting_data.duration/60 << " minutes</p>";
    unsigned short int __total_questions = 0;
    for (unsigned short int i = 0; i < this->test_starting_data.number_of_sections; ++i) {
        __total_questions += this->test_starting_data.sections[i].number_of_questions;
    }
    __html_stream << "<p>Total Questions: " << __total_questions << "</p>";

    __html_stream << "<p>Description: " << this->test_starting_data.test_description << "</p>";

    // Display sections information
    __html_stream << "<h2>Sections:</h2>";
    __html_stream << "<ul>";
    for (unsigned short int i = 0; i < this->test_starting_data.number_of_sections; ++i)
    {
        const test_section_info &section = this->test_starting_data.sections[i];
        __html_stream << "<li>";
        __html_stream << "<strong>Section Name:</strong> " << section.section_name << "<br>";
        __html_stream << "<strong>Number of Questions:</strong> " << section.number_of_questions << "<br>";
        __html_stream << "</li>";
    }
    __html_stream << "</ul>";

    // HTML footer
    __html_stream << "</body></html>";

    return __html_stream.str();
}
