#include "result_creation_list_panel.h"

wxDEFINE_EVENT(__REAL_ANSWER_CHANGED, wxCommandEvent);

wxBEGIN_EVENT_TABLE(questionsListForResultCreation, wxPanel)
EVT_COMMAND(wxID_ANY, __REAL_ANSWER_CHANGED, questionsListForResultCreation::update_real_answer)
wxEND_EVENT_TABLE()


class customStaticText : public wxStaticText {
public:
    customStaticText(wxWindow* parent, const wxString& label,const wxString tool_tip,
        const wxSize& size = wxDefaultSize,
        const wxPoint& pos = wxDefaultPosition,
        long style = wxALIGN_LEFT,
        const wxString& name = wxStaticTextNameStr)
        : wxStaticText(parent, wxID_ANY, label, pos, size, style, name) {

        SetFont(wxFont(wxFontInfo(22).Family(wxFONTFAMILY_SWISS)));
        SetToolTip(tool_tip);
    }
};

questionsListForResultCreation::questionsListForResultCreation(wxWindow *parent, rapidcsv::Document questions_csv)
    : wxScrolled<wxPanel>(parent, wxID_ANY), questions_csv(questions_csv)
{
    this->questions_csv.InsertColumn<std::string>(this->questions_csv.GetColumnCount(), std::vector<std::string>(questions_csv.GetRowCount(), "NaN"), "real_answer");
    this->questions_csv.InsertColumn<std::string>(this->questions_csv.GetColumnCount(), std::vector<std::string>(questions_csv.GetRowCount(), "NaN"), "validity");

    this->SetMinSize(wxSize(500, 700));
    this->SetBackgroundColour(wxColor(116, 151, 219));


    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxProgressDialog dialog("Preparing window", "Progress", 100, parent, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
    for (int row = 0; row < questions_csv.GetRowCount(); row++) {
        std::vector<std::string>current_row = this->questions_csv.GetRow<std::string>(row);
        
        questionsListForResultCreation_row* question_row = new questionsListForResultCreation_row(this, questionsListForResultCreation_row::questionData{
            current_row[0],
            (unsigned short int)std::stoi(current_row[1]),
            (unsigned short int)std::stoi(current_row[2]),
            current_row[3],
            current_row[4],
            std::stof(current_row[5]),
            current_row[6],
            current_row[7]
            });
        
        questions_row.push_back(question_row);
        sizer->Add(question_row, 0, wxEXPAND | wxALL, 15);

        dialog.Update((row * 100) / questions_csv.GetRowCount(), wxString::Format("%d of %d done", row, questions_csv.GetRowCount()));
    }

    SetSizer(sizer);
    SetScrollRate(0, 10);
}

questionsListForResultCreation_row::questionsListForResultCreation_row(wxWindow *parent, questionData question_data)
    : wxPanel(parent, wxID_ANY), question_data(question_data)
{
    SetBackgroundColour(*wxYELLOW);
    this->file_name = new customStaticText(this, question_data.file_name, "File name of question");
    this->order = new customStaticText(this, std::to_string(question_data.order), "Section order of question");
    this->number = new customStaticText(this, std::to_string(question_data.number), "Question number relative to the section");

    wxBoxSizer* row_sizer = new wxBoxSizer(wxHORIZONTAL);
    row_sizer->AddSpacer(10);
    row_sizer->Add(this->file_name, 0, wxALIGN_CENTER_VERTICAL, 5);
    row_sizer->AddSpacer(30);
    row_sizer->Add(this->order, 0, wxALIGN_CENTER_VERTICAL, 5);
    row_sizer->AddSpacer(30);
    row_sizer->Add(this->number, 0, wxALIGN_CENTER_VERTICAL, 5);
    row_sizer->AddSpacer(75);
    if (question_data.type == "sc")
    {
        this->answer_sc = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, { "A" , "B", "C", "D"});
        answer_sc->SetToolTip("Choose the correct option of the question");
        answer_sc->SetFont(wxFont(wxFontInfo(20).Family(wxFONTFAMILY_SWISS)));

        answer_sc->Bind(wxEVT_CHOICE, [=](wxCommandEvent& event) {
            wxCommandEvent answer_change_event(__REAL_ANSWER_CHANGED, GetId());
            answer_change_event.SetString(this->question_data.file_name + SPLITTER + answer_sc->GetString(answer_sc->GetSelection()) + ",");
            GetEventHandler()->ProcessEvent(answer_change_event);
            });

        row_sizer->Add(this->answer_sc, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 10);
    }
    else if (question_data.type == "in" || question_data.type == "fl")
    {
        this->answer_fl_in = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
        answer_fl_in->SetHint("Enter the correct answer of the question");
        answer_fl_in->SetToolTip("Enter the correct answer of the question");
        answer_fl_in->SetFont(wxFont(wxFontInfo(20).Family(wxFONTFAMILY_SWISS)));

        answer_fl_in->Bind(wxEVT_TEXT, [=](wxCommandEvent& event) {
            wxCommandEvent answer_change_event(__REAL_ANSWER_CHANGED, GetId());
            answer_change_event.SetString(this->question_data.file_name + SPLITTER + answer_fl_in->GetValue());
            GetEventHandler()->ProcessEvent(answer_change_event);
            });

        row_sizer->Add(this->answer_fl_in, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL | wxEXPAND, 10);
    }
    else if (question_data.type == "mc")
    {
        wxMessageBox("Result creation with multicorrect options functionality is not avaliable.", "Error", wxOK | wxICON_ERROR, this);
    }
    SetSizerAndFit(row_sizer);
}

void questionsListForResultCreation::update_real_answer(wxCommandEvent& event){ 
    auto str = event.GetString().ToStdString();
    size_t pos = str.find(std::string(SPLITTER));
    auto row = find_row_number(this->questions_csv, "question_file_name", str.substr(0, pos));
    auto real_answer = str.substr(pos + std::string(SPLITTER).length());
    real_answer.erase(std::remove_if(real_answer.begin(), real_answer.end(), [](char c) { return std::isspace(c); }), real_answer.end()); //removing spaces
    
    this->questions_csv.SetCell<std::string>(7, row, real_answer);

    auto user_answer = this->questions_csv.GetCell<std::string>(6, row);

    if (user_answer == "NaN")
        this->questions_csv.SetCell<std::string>(8, row, "NaN");
    else if (user_answer.find(real_answer) != std::string::npos)
        //answer is correct
        this->questions_csv.SetCell<std::string>(8, row, "1");
    else
        this->questions_csv.SetCell<std::string>(8, row, "0");

};
