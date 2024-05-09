#include "main_frame.h"

mainFrame::mainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxMenu* menu_advance = new wxMenu;
    menu_advance->Append(ID_load_test, "&Load Test\tCtrl+L");
    menu_advance->Append(ID_resume_test, "&Resume Test\tCtrl+H");
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
    Bind(wxEVT_MENU, &mainFrame::on_resume_test_clicked, this, ID_resume_test);
    this->start_test_button->Bind(wxEVT_BUTTON, &mainFrame::on_start_test_clicked, this);
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
    auto *create_test_frame = new testCreateFrame(this);
    create_test_frame->Show(true);
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
    wxString questions_details_file_path = wxString(folder_path).append("\\test_questions.csv");
    if (wxFileExists(file_path) and wxFileExists(questions_details_file_path))
    {
        check_weather_test_file_is_fit(file_path, questions_details_file_path);
    }
    else
    {
        wxMessageBox("Crucial file(s) to start the test is missing!\nPlease contact the test provider for more info.", "Error", wxOK | wxICON_ERROR, this);
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

bool mainFrame::check_weather_test_file_is_fit(const wxString &test_file_location, const wxString &test_questions_details_file)
{
    auto test_file_content_status = this->read_json_file(test_file_location.ToStdString());

    if (test_file_content_status == 1)
    {
        // failure to read
        wxMessageBox("Unable to read the test file check weather required permission are granted or not.", "Reading Error", wxOK | wxICON_ERROR, this);
        return false;
    }
    if (test_file_content_status == 2)
    {
        // parsing error
        wxMessageBox("Unable to parse the file!\nTest file may be corrupted.", "Parsing Error", wxOK | wxICON_ERROR, this);
        return false;
    }

    // TODO set an options in settings to eliminate hard coding of the location of result file
    wxString result_file_name = "";
    try
    {
        wxInitializer initializer;
        wxDateTime currentTime = wxDateTime::Now();
        result_file_name = this->test_starting_data.test_name + currentTime.Format(" result, %Y-%m-%d %H-%M-%S.csv");
    }
    catch (const std::exception&)
    {
        wxLogError("Failed to initialize wxWidgets.");
        result_file_name = this->test_starting_data.test_name + " result, --no_data-- test.csv";
    }

    this->test_starting_data.student_test_result_file = wxString(test_questions_details_file.ToStdString()).Truncate(test_questions_details_file.length() - wxString("test_questions.csv").length()).append(result_file_name);
    auto test_questions_details_status = this->read_csv_file(test_questions_details_file.ToStdString());
    
    if (test_questions_details_status == 1)
    {
        // failure to read
        wxMessageBox("Unable to read the questions info file check weather required permission are granted or not.", "Reading Error", wxOK | wxICON_ERROR, this);
        return false;
    }
    if (test_questions_details_status == 2)
    {
        // parsing error
        wxMessageBox("Unable to parse the file!\nTest questions file may be corrupted or not properly created.", "Parsing Error", wxOK | wxICON_ERROR, this);
        return false;
    }
    else
    {
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
    __html_stream << "<p>Duration: " << format_time_from_seconds(this->test_starting_data.duration) << "</p>";
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

void mainFrame::on_start_test_clicked(wxCommandEvent& event)
{
    this->start_test_button->Enable(false);
    this->test_info_display->SetPage("<html><body><h1>Load test via <i>Options > Load Test </i>!</h1><br><h1>Create new test via <i>Options > Create Test </i>!</h1></body></html>");
    auto* exam_frame = new examFrame(this->test_starting_data);
    exam_frame->ShowFullScreen(true);
}

unsigned short int mainFrame::read_csv_file(const std::string &file_path)
{
    rapidcsv::Document doc(file_path, rapidcsv::LabelParams(0, -1));

    // Check if the actual header matches the expected header
    std::vector<std::string> expectedHeader = {"question_file_name", "section_order", "question_number", "question_type"};
    if (doc.GetColumnNames() != expectedHeader) {
        // std::cerr << "Error: CSV header does not match the expected format." << std::endl;
        return 2; // Return 2 on parsing error
    }

    // Check each row for the expected number of columns
    const size_t expectedColumnCount = expectedHeader.size();
    for (size_t rowIdx = 0; rowIdx < doc.GetRowCount(); ++rowIdx)
    {
        const size_t actualColumnCount = doc.GetRow<std::string>(rowIdx).size();
        if (actualColumnCount != expectedColumnCount)
        {
            // std::cerr << "Error: Row " << rowIdx + 1 << " does not have the expected number of columns." << std::endl;
            return 2; // Return 2 on parsing error
        }
    }

    //TODO also check values for different columns
    // Check if all values in the specified column match any of the expected values
    std::unordered_set<std::string> expected_question_types = { "sc", "in", "mc", "fl" };
    for (size_t rowIdx = 0; rowIdx < doc.GetRowCount(); ++rowIdx) {
        const std::string actualValue = doc.GetCell<std::string>(3, rowIdx);
        if (expected_question_types.find(actualValue) == expected_question_types.end()) {
            // std::cerr << "Error: Value in row " << rowIdx + 1 << " does not match any expected value." << std::endl;
            return 2;
        }
    }

    // Add some student stats column
    doc.InsertColumn<std::string>(4, std::vector<std::string>(doc.GetRowCount(), "nv"), "question_status");
    doc.InsertColumn<unsigned int>(5, std::vector<unsigned int>(doc.GetRowCount(), 0), "time_spent");
    doc.InsertColumn<std::string>(6, std::vector<std::string>(doc.GetRowCount(), "NaN"), "answer");
    
    doc.Save(this->test_starting_data.student_test_result_file);

    return 3; // Data reading successfully completed
}

void mainFrame::on_resume_test_clicked(wxCommandEvent& event)
{
    wxMessageBox("The facility will be implemented soon!",
        "No Feature Found", wxOK | wxICON_ERROR);
}
