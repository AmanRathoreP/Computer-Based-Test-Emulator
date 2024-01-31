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

        return true;
    }
}