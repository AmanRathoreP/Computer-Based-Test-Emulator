#include "test_creation_from_questions_options_panel.h"

wxDEFINE_EVENT(DATA_UPDATED, wxCommandEvent);
wxDEFINE_EVENT(NEED_CURRENT_SELECTION, wxCommandEvent);
wxDEFINE_EVENT(__BASE_FOLDER_UPDATED, wxCommandEvent);
wxDEFINE_EVENT(__NEED_CURRENT_SELECTION, wxCommandEvent);
wxDEFINE_EVENT(__TRYING_TO_DELETE_SECTION, wxCommandEvent);

wxBEGIN_EVENT_TABLE(questionsOptionsForTestCreation, wxPanel)
    EVT_COMMAND(wxID_ANY, __NEED_CURRENT_SELECTION, questionsOptionsForTestCreation::extract_question_numbers_from_selection)
        EVT_COMMAND(wxID_ANY, __TRYING_TO_DELETE_SECTION, questionsOptionsForTestCreation::try_to_delete_section)
            wxEND_EVENT_TABLE()

                questionsOptionsForTestCreation::questionsOptionsForTestCreation(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    this->SetMinSize(wxSize(250, 700));
    SetBackgroundColour(*wxBLUE);

    wxButton *folder_select_button = new wxButton(this, wxID_ANY, "Select Folder");
    folder_select_button->SetToolTip("Select folder with desirable questions' image files in it!");
    folder_select_button->Bind(wxEVT_BUTTON, &questionsOptionsForTestCreation::OnSelectFolderClicked, this);

    this->folder_path_text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    this->folder_path_text->SetHint("Select Folder!!");
    this->folder_path_text->SetToolTip("Select folder with desirable questions' image files in it!");

    this->warning_time_panel = new __timeSelector(this, "Warning Time", wxColor(154, 133, 48));
    this->threshold_time_panel = new __timeSelector(this, "Threshold Time", wxColor(255, 40, 40));
    this->total_time_panel = new __timeSelector(this, "Total Time", wxColor(162, 166, 163));

    this->section_question_mapper = new __sectionQuestionMapper(this);

    wxPanel *action_buttons_panel = new wxPanel(this);
    this->preview_button = new customButton(this, wxID_ANY, "Preview", false, 23, 23, wxColor(168, 137, 245));
    this->generate_button = new customButton(this, wxID_ANY, "Generate", false, 23, 23, wxColor(68, 242, 126));
    this->preview_button->Bind(wxEVT_BUTTON, &questionsOptionsForTestCreation::OnPreviewButtonClicked, this);
    this->generate_button->Bind(wxEVT_BUTTON, &questionsOptionsForTestCreation::OnGenerateButtonClicked, this);

    this->test_name = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    this->test_name->SetHint("Test name...");
    this->test_name->SetToolTip("Write test name here");
    this->test_description = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    this->test_description->SetHint("Test description...");
    this->test_description->SetToolTip("Write test description here");

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->AddSpacer(10);
    sizer->Add(this->folder_path_text, 0, wxEXPAND | wxALL | wxALIGN_CENTER_HORIZONTAL, 2);
    sizer->Add(folder_select_button, 0, wxEXPAND | wxALL | wxALIGN_CENTER_HORIZONTAL, 2);
    sizer->AddSpacer(8);

    sizer->Add(this->warning_time_panel, 0, wxEXPAND | wxALL | wxALIGN_CENTER_HORIZONTAL, 2);
    sizer->Add(this->threshold_time_panel, 0, wxEXPAND | wxALL | wxALIGN_CENTER_HORIZONTAL, 2);
    sizer->Add(this->total_time_panel, 0, wxEXPAND | wxALL | wxALIGN_CENTER_HORIZONTAL, 2);

    sizer->AddSpacer(8);
    sizer->Add(this->section_question_mapper, 0, wxEXPAND | wxALL | wxALIGN_CENTER_HORIZONTAL, 2);
    sizer->AddSpacer(8);

    sizer->Add(this->test_name, 0, wxEXPAND | wxALL | wxALIGN_BOTTOM, 2);
    sizer->Add(this->test_description, 0, wxEXPAND | wxALL | wxALIGN_BOTTOM, 2);
    sizer->AddSpacer(8);

    sizer->Add(this->preview_button, 0, wxEXPAND | wxALL | wxALIGN_BOTTOM, 2);
    sizer->Add(this->generate_button, 0, wxEXPAND | wxALL | wxALIGN_BOTTOM, 2);
    sizer->AddSpacer(8);

    SetSizerAndFit(sizer);
}

void questionsOptionsForTestCreation::OnPreviewButtonClicked(wxCommandEvent& event){
    if (this->__folder_updated)
    {
        this->question_csv_data.Clear();
        this->question_csv_data.SetColumnName(0, "SO, QN");
        this->question_csv_data.SetColumnName(1, "File Name");
        this->question_csv_data.SetColumnName(2, "Section Name");
        try
        {
            this->question_csv_data.SetColumn<std::string>("File Name", read_files_with_particular_extension(this->folder_path_text->GetValue(), wxString("jpg")));

            std::vector<std::string> empth_col = std::vector<std::string>(this->question_csv_data.GetRowCount(), "NaN");

            this->question_csv_data.SetColumn<std::string>("SO, QN", empth_col);
            this->question_csv_data.SetColumn<std::string>("Section Name", empth_col);
        }
        catch (const std::exception &e)
        {
            wxString errorMsg(e.what(), wxConvUTF8);
            wxLogError(errorMsg);
        }

        this->section_question_mapper->clear_all_questions_in_sections_info();
        wxCommandEvent folder_update_event(DATA_UPDATED, GetId());
        GetEventHandler()->ProcessEvent(folder_update_event);
        this->__folder_updated = false;

        return;
    }

    // checking wether multiple section have same questions
    //*std::map<section_order, std::pair<questions, section_name>>;
    std::map<unsigned short int, std::pair<wxString, wxString>> section_order_questions_section_name_map;
    std::map<unsigned short int, std::vector<unsigned short int>> section_order_questions_map;
    section_order_questions_section_name_map = this->section_question_mapper->get_all_data_of_sections();

    for (auto &section : section_order_questions_section_name_map)
    {
        section_order_questions_map.insert(std::make_pair(section.first, this->get_questions_array_from_string(section.second.first, -1)));
    }

    wxString question_section_info = this->is_same_questions_in_multiple_sections(section_order_questions_map);
    if (question_section_info != "")
    {
        wxLogError(question_section_info);
        return;
    }

    std::vector<std::string> so_qn_column = std::vector<std::string>(this->question_csv_data.GetRowCount(), "NaN");
    std::vector<std::string> section_name = std::vector<std::string>(this->question_csv_data.GetRowCount(), "NaN");

    for (auto section_order_questions : section_order_questions_map)
    {
        unsigned short int question_number = 1;
        std::sort(section_order_questions.second.begin(), section_order_questions.second.end());

        for (auto question : section_order_questions.second)
        {
            so_qn_column[question] = wxString::Format("%d, %d", section_order_questions.first, question_number++).ToStdString();
            section_name[question] = section_order_questions_section_name_map[section_order_questions.first].second;
        }
    }

    this->question_csv_data.SetColumn<std::string>("SO, QN", so_qn_column);
    this->question_csv_data.SetColumn<std::string>("Section Name", section_name);
    wxCommandEvent questions_section_event(DATA_UPDATED, GetId());
    GetEventHandler()->ProcessEvent(questions_section_event);
}

void questionsOptionsForTestCreation::OnGenerateButtonClicked(wxCommandEvent &event)
{
}

void questionsOptionsForTestCreation::OnSelectFolderClicked(wxCommandEvent &event)
{
    wxDirDialog dlg(this, "Choose a folder with questions images", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxString folder_path = dlg.GetPath();
        this->folder_path_text->SetValue(folder_path);
        this->__folder_updated = true;
    }
}

std::vector<std::string> questionsOptionsForTestCreation::read_files_with_particular_extension(const wxString &folderPath, const wxString &extension)
{
    std::vector<std::string> files;
    wxDir dir(folderPath);
    if (!dir.IsOpened())
    {
        wxLogError("Failed to open directory %s", folderPath);
        return files;
    }

    wxString filename;
    bool cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_FILES);
    while (cont)
    {
        if (wxFileName(filename).GetExt().IsSameAs(extension, true))
            files.push_back(filename.ToStdString());
        cont = dir.GetNext(&filename);
    }
    return files;
}

__timeSelector::__timeSelector(wxWindow *parent, const wxString label, const wxColor background_color) : wxPanel(parent, wxID_ANY)
{
    this->SetToolTip(wxString::Format("This defines that after how much time does the progress bar of %s will end!", label));

    this->SetBackgroundColour(background_color);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL); // Vertical sizer for label and grid

    // Label
    wxStaticText *labelText = new wxStaticText(this, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    labelText->SetFont(wxFontInfo(19));
    labelText->SetToolTip(wxString::Format("This defines that after how much time does the progress bar of %s will end!", label));
    vbox->Add(labelText, 0, wxALIGN_CENTER | wxALL, 5);

    // Grid sizer for 2x2 layout
    wxGridSizer *grid = new wxGridSizer(2, 2, 5, 5); // 2 rows, 2 columns, with 5 pixels of horizontal and vertical spacing

    // Left Text Box (using wxTextCtrl)
    leftTextBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    leftTextBox->SetHint("Total questions");
    grid->Add(leftTextBox, 1, wxEXPAND);

    // Right Text Box (using wxTextCtrl)
    rightTextBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    rightTextBox->SetHint("Time for each question");
    grid->Add(rightTextBox, 1, wxEXPAND);

    // wxChoice
    wxArrayString choices;
    choices.Add("Seconds");
    choices.Add("Minutes");
    choices.Add("Hours");
    choiceBox = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
    choiceBox->SetSelection(0);
    grid->Add(choiceBox, 0, wxALIGN_CENTER_VERTICAL);

    total_time_display = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    total_time_display->SetHint("Enter valid values");
    total_time_display->SetToolTip(wxString::Format("See %s", label));
    grid->Add(total_time_display, 1, wxEXPAND);

    overall_readable_time_display = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    overall_readable_time_display->SetFont(wxFontInfo(16));
    overall_readable_time_display->SetToolTip(wxString::Format("This defines that after how much time does the progress bar of %s will end!", label));

    // Bind events for left and right text box changes
    leftTextBox->Bind(wxEVT_TEXT, &__timeSelector::OnQuestionTimeTextBoxChange, this);
    rightTextBox->Bind(wxEVT_TEXT, &__timeSelector::OnQuestionTimeTextBoxChange, this);
    choiceBox->Bind(wxEVT_CHOICE, &__timeSelector::OnQuestionTimeTextBoxChange, this);

    vbox->Add(grid, 1, wxEXPAND | wxALL, 5);
    vbox->Add(overall_readable_time_display, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

    SetSizer(vbox);
}

void __timeSelector::OnQuestionTimeTextBoxChange(wxCommandEvent &event)
{
    total_time_display->Clear();

    // Calculate and set total time if left and right text boxes are not empty
    wxString leftValueStr = leftTextBox->GetValue();
    wxString rightValueStr = rightTextBox->GetValue();

    overall_readable_time_display->SetLabel("");

    if (!leftValueStr.empty() && !rightValueStr.empty())
    {
        double leftValue, rightValue;
        if (leftValueStr.ToDouble(&leftValue) && rightValueStr.ToDouble(&rightValue))
        {
            this->total_time_in_seconds = (int)(leftValue * rightValue * time_multiplier_values_to_convert_into_seconds[choiceBox->GetSelection()]);
            total_time_display->SetValue(wxString::Format("%0.2f", (float)(leftValue * rightValue)));
            total_time_display->SetToolTip(wxString::Format("Seconds = %d\nMinutes = %.2f\nHours = %.2f", this->total_time_in_seconds, ((float)this->total_time_in_seconds) / 60, ((float)this->total_time_in_seconds) / 3600));
            overall_readable_time_display->SetLabel(this->format_time(this->total_time_in_seconds));
            return;
        }
    }
    total_time_display->SetToolTip("Enter Valid values");
}

wxString __timeSelector::format_time(unsigned int seconds)
{
    if (seconds < 60)
        return wxString::Format("%02d secs", seconds);
    else if (seconds < 3600)
        return wxString::Format("%02d:%02d", seconds / 60, seconds % 60);
    else if (seconds < 3600 * 24)
        return wxString::Format("%02d:%02d:%02d", seconds / 3600, (seconds / 60) % 60, seconds % 60);

    return wxString::Format("%04d:%02d:%02d", seconds / 3600, (seconds / 60) % 60, seconds % 60);
}

__sectionQuestionMapper::__sectionQuestionMapper(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    this->SetBackgroundColour(wxColor(178, 99, 235));
    // Main vertical sizer
    main_sizer = new wxBoxSizer(wxVERTICAL);

    // Button for adding new rows
    this->add_row_button = new wxButton(this, wxID_ANY, "Add Row");
    add_row_button->Bind(wxEVT_BUTTON, &__sectionQuestionMapper::add_row, this);

    this->rows_panel = new wxPanel(this, wxID_ANY);
    this->rows_sizer = new wxBoxSizer(wxVERTICAL); // Initialize rows_sizer

    rows_panel->SetSizer(this->rows_sizer);

    add_row("", "");
    // todo make sure to update the size dynamically
    this->rows_panel->SetMinSize(wxSize(-1, this->rows_panel->GetSize().y * (MAX_ROWS + 1.65f)));

    main_sizer->Add(rows_panel, 1, wxEXPAND | wxALIGN_TOP, 5);
    main_sizer->AddSpacer(12);
    main_sizer->Add(add_row_button, 0, wxEXPAND | wxALIGN_BOTTOM, 5);

    SetSizer(main_sizer);
}

void __sectionQuestionMapper::add_row(const wxString questions, const wxString section_name)
{
    __sectionQuestionMapper_row *new_row = new __sectionQuestionMapper_row(this->rows_panel, this->last_used_section_id++);
    sections.push_back(new_row);

    this->rows_sizer->Add(new_row, 0, wxEXPAND);

    // todo fix issue of new row not showing
    rows_panel->Layout();
    Layout();

    this->calculate_add_row_button_visibility();
}

void __sectionQuestionMapper::calculate_add_row_button_visibility(void)
{
    // Disable add button if more than MAX_ROWS rows are added
    if (this->rows_sizer->GetItemCount() >= MAX_ROWS)
    {
        this->add_row_button->Disable();
    }
    else
    {
        this->add_row_button->Enable();
    }
}

void __sectionQuestionMapper_row::OnDelete(wxCommandEvent &event)
{
    wxCommandEvent custom_event(__TRYING_TO_DELETE_SECTION, GetId());
    custom_event.SetInt(this->section_id);
    GetEventHandler()->ProcessEvent(custom_event);
}

__sectionQuestionMapper_row::__sectionQuestionMapper_row(wxWindow *parent, unsigned short int section_id) : wxPanel(parent, wxID_ANY), section_id(section_id)
{
    auto sizer = new wxBoxSizer(wxHORIZONTAL);

    this->questions_to_include_input = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    this->questions_to_include_input->SetHint("Enter Questions To Add");
    this->questions_to_include_input->SetToolTip("Use '5-7' for include question 5, 6, 7\nUse 1, 2 to include question 1 and 2\nNote: Add ',' after specifying a range of questions");
    this->section_name_input = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    this->section_name_input->SetHint("Enter Section Name!");
    this->section_name_input->SetToolTip("Write Section Name Here!");
    this->add_current_selection = new wxButton(this, wxID_ANY, "Add Selection");
    this->add_current_selection->SetToolTip("Add current selections in questions");
    this->add_current_selection->Bind(wxEVT_BUTTON, &__sectionQuestionMapper_row::OnAddCurrentSelection, this);

    this->delete_button = new wxButton(this, wxID_ANY, "X", wxDefaultPosition, wxSize(18, -1));
    this->delete_button->SetToolTip("Delete this row");
    this->delete_button->Bind(wxEVT_BUTTON, &__sectionQuestionMapper_row::OnDelete, this);

    this->clear_button = new wxButton(this, wxID_ANY, "Clear", wxDefaultPosition, wxDefaultSize);
    this->clear_button->SetToolTip("Clear questions");
    this->clear_button->Bind(wxEVT_BUTTON, &__sectionQuestionMapper_row::OnClear, this);

    this->question_type = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, this->question_types);
    this->question_type->SetSelection(0);
    this->question_type->SetToolTip("Select the type of question");

    sizer->Add(this->questions_to_include_input, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 2);
    sizer->AddSpacer(4);
    sizer->Add(this->section_name_input, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 2);
    sizer->AddSpacer(8);
    sizer->Add(this->add_current_selection, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 2);
    sizer->AddSpacer(5);
    sizer->Add(this->question_type, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 2);
    sizer->AddSpacer(2);
    sizer->Add(this->clear_button, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 2);
    sizer->Add(this->delete_button, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 2);
    SetSizer(sizer);
}

void __sectionQuestionMapper_row::OnAddCurrentSelection(wxCommandEvent &event)
{
    wxCommandEvent custom_event(__NEED_CURRENT_SELECTION, GetId());
    custom_event.SetInt(this->section_id);
    custom_event.SetString(this->get_questions_in_section());
    GetEventHandler()->ProcessEvent(custom_event);
}

void questionsOptionsForTestCreation::extract_question_numbers_from_selection(wxCommandEvent &event)
{
    wxCommandEvent need_selection_event(NEED_CURRENT_SELECTION, GetId());
    GetEventHandler()->ProcessEvent(need_selection_event);

    this->section_question_mapper->update_questions_in_section_info(event.GetInt(), this->format_questions_array_to_string(this->get_questions_array_from_string(remove_leading_symbols(this->format_questions_array_to_string(this->__current_rows_selection_from_questions_list_panel, 1) + "," + event.GetString()))));
}

void __sectionQuestionMapper::update_questions_in_section_info(const unsigned short int section_id, const wxString questions)
{
    for (auto section : sections)
    {
        if (section->get_section_id() == section_id)
            section->set_questions_in_section(questions);
    }
}

void __sectionQuestionMapper::clear_all_questions_in_sections_info(void)
{
    for (auto section : sections)
        section->set_questions_in_section("");
}

void __sectionQuestionMapper::drop_row(const unsigned short int section_id)
{
    std::vector<__sectionQuestionMapper_row *> new_sections;

    for (auto section : this->sections)
    {
        if (section->get_section_id() != section_id)
            new_sections.push_back(section);
        else
        {
            this->rows_sizer->Detach(section);
            delete section;
            this->rows_sizer->Layout();
        }
    }
    this->sections = new_sections;
}

wxString questionsOptionsForTestCreation::format_questions_array_to_string(std::vector<unsigned short int> vec, short int value_to_add_in_each_element)
{
    std::sort(vec.begin(), vec.end());

    std::string result;
    bool inRange = false;
    unsigned short int startRange = 0;

    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (!inRange)
        {
            startRange = vec[i];
            inRange = true;
        }
        if (i == vec.size() - 1 || vec[i] + 1 != vec[i + 1])
        {
            if (startRange == vec[i])
            {
                result += std::to_string(startRange + value_to_add_in_each_element);
            }
            else
            {
                result += std::to_string(startRange + value_to_add_in_each_element) + "-" + std::to_string(vec[i] + value_to_add_in_each_element);
            }
            if (i != vec.size() - 1)
            {
                result += ", ";
            }
            inRange = false;
        }
    }

    return wxString(result);
}

std::vector<unsigned short int> questionsOptionsForTestCreation::get_questions_array_from_string(wxString str, short int value_to_add_in_each_element)
{
    std::vector<unsigned short int> result;
    const std::string &rangeString = str.ToStdString();
    std::istringstream iss(rangeString);
    std::string token;

    while (std::getline(iss, token, ','))
    {
        size_t dashPos = token.find('-');
        if (dashPos != std::string::npos)
        { // If dash found, it's a range
            unsigned short int startRange = std::stoi(token.substr(0, dashPos));
            unsigned short int endRange = std::stoi(token.substr(dashPos + 1));
            for (unsigned short int i = startRange; i <= endRange; ++i)
            {
                result.push_back(i + value_to_add_in_each_element);
            }
        }
        else
        { // Single number
            result.push_back(std::stoi(token) + value_to_add_in_each_element);
        }
    }

    std::sort(result.begin(), result.end());
    auto last = std::unique(result.begin(), result.end());
    result.erase(last, result.end());

    return result;
}

void questionsOptionsForTestCreation::try_to_delete_section(wxCommandEvent &event)
{
    this->section_question_mapper->drop_row(event.GetInt());
    this->section_question_mapper->calculate_add_row_button_visibility();
}

std::map<unsigned short int, std::pair<wxString, wxString>> __sectionQuestionMapper::get_all_data_of_sections(void)
{
    // std::map<section_order, std::pair<questions, section_name>>;
    std::map<unsigned short int, std::pair<wxString, wxString>> data;

    unsigned short int section_order = 1;

    for (auto section : sections)
        data.insert(std::make_pair(section_order++, std::make_pair<wxString, wxString>(section->get_questions_in_section(), section->get_section_name())));

    return data;
}

wxString questionsOptionsForTestCreation::is_same_questions_in_multiple_sections(std::map<unsigned short int, std::vector<unsigned short int>> sections_questions_map)
{
    // todo return a proper string displaying question number and in which sections they are multiple times

    std::vector<unsigned short int> all_questions;

    for (const auto &pair : sections_questions_map)
    {
        all_questions.insert(all_questions.end(), pair.second.begin(), pair.second.end());
    }

    std::unordered_set<unsigned short int> seen_elements;

    for (const auto &num : all_questions)
    {
        // If the element is already in the set, it's a duplicate
        if (seen_elements.find(num) != seen_elements.end())
        {
            return "Multiple sections has same question(s)";
        }
        seen_elements.insert(num);
    }

    return ""; // No duplicates found
}

inline wxString questionsOptionsForTestCreation::remove_leading_symbols(const wxString& str, const wxChar symbol) {
    wxString result = str;
    if (!result.empty() && (result[0] == symbol)) {
        result = result.Mid(1); // Remove the first character
    }
    return result;
}

void __sectionQuestionMapper_row::OnClear(wxCommandEvent& event) {
    this->questions_to_include_input->SetValue("");
}
