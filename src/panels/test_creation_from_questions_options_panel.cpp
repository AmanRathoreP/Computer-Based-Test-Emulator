#include "test_creation_from_questions_options_panel.h"

wxDEFINE_EVENT(DATA_UPDATED, wxCommandEvent);

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
    sizer->Add(new __sectionQuestionMapper(this), 0, wxEXPAND | wxALL | wxALIGN_CENTER_HORIZONTAL, 2);
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
    wxCommandEvent custom_event(DATA_UPDATED, GetId());
    GetEventHandler()->ProcessEvent(custom_event);
}
void questionsOptionsForTestCreation::OnGenerateButtonClicked(wxCommandEvent& event){}


void questionsOptionsForTestCreation::OnSelectFolderClicked(wxCommandEvent &event)
{
    wxDirDialog dlg(this, "Choose a folder with questions images", "", wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (dlg.ShowModal() == wxID_OK)
    {
        wxString folder_path = dlg.GetPath();
        this->folder_path_text->SetValue(folder_path);

        this->question_csv_data.Clear();
        this->question_csv_data.SetColumnName(0, "SO, QN");
        this->question_csv_data.SetColumnName(1, "File Name");
        this->question_csv_data.SetColumnName(2, "Section Name");
        try
        {
            this->question_csv_data.SetColumn<std::string>("SO, QN", read_files_with_particular_extension(folder_path, wxString("jpg")));
            this->question_csv_data.SetColumn<std::string>("File Name", read_files_with_particular_extension(folder_path, wxString("jpg")));
            this->question_csv_data.SetColumn<std::string>("Section Name", read_files_with_particular_extension(folder_path, wxString("jpg")));
        }
        catch (const std::exception &e)
        {
            wxString errorMsg(e.what(), wxConvUTF8);
            wxLogError(errorMsg);
        }
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
    __sectionQuestionMapper_row *new_row = new __sectionQuestionMapper_row(this->rows_panel);
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
    wxWindow *parent = GetParent();
    if (!parent)
        return;

    wxSizer *parentSizer = parent->GetSizer();
    if (!parentSizer)
        return;

    if (parentSizer->GetItemCount() <= 1)
        return;

    parentSizer->Detach(this);
    parentSizer->Layout();
    delete this;

    // Find the parent of the rows panel, which is the mapper
    __sectionQuestionMapper *mapper = dynamic_cast<__sectionQuestionMapper *>(parent->GetParent());
    if (!mapper)
        return;

    mapper->calculate_add_row_button_visibility();
}

__sectionQuestionMapper_row::__sectionQuestionMapper_row(wxWindow *parent) : wxPanel(parent, wxID_ANY)
{
    auto sizer = new wxBoxSizer(wxHORIZONTAL);

    this->questions_to_include_input = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    this->questions_to_include_input->SetHint("Enter Questions To Add");
    this->questions_to_include_input->SetToolTip("Use '5-7' for include question 5, 6, 7\nUse 1, 2 to include question 1 and 2\nNote: Add ',' after specifying a range of questions");
    this->section_name_input = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    this->section_name_input->SetHint("Enter Section Name!");
    this->section_name_input->SetToolTip("Write Section Name Here!");
    this->add_current_selection = new wxButton(this, wxID_ANY, "Add Selection");
    this->add_current_selection->SetToolTip("Add current selections in questions");

    this->delete_button = new wxButton(this, wxID_ANY, "X", wxDefaultPosition, wxSize(18, -1));
    this->delete_button->SetToolTip("Delete this row");
    this->delete_button->Bind(wxEVT_BUTTON, &__sectionQuestionMapper_row::OnDelete, this);

    sizer->Add(this->questions_to_include_input, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 2);
    sizer->AddSpacer(4);
    sizer->Add(this->section_name_input, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 2);
    sizer->AddSpacer(10);
    sizer->Add(this->add_current_selection, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL, 2);
    sizer->AddSpacer(2);
    sizer->Add(this->delete_button, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 2);

    SetSizer(sizer);
}
