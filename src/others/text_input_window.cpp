#include "text_input_window.h"

wxBEGIN_EVENT_TABLE(customTextCtrl, wxTextCtrl)
    EVT_TEXT(wxID_ANY, customTextCtrl::on_text_change)
        wxEND_EVENT_TABLE()

            customTextCtrl::customTextCtrl(wxWindow *parent, wxWindowID id,
                                           const wxString &value,
                                           const wxPoint &pos,
                                           const wxSize &size,
                                           long style,
                                           unsigned short int max_number_of_characters)
    : wxTextCtrl(parent, id, value, pos, size, style),
      accept_float(false)
{
    SetMaxLength(max_number_of_characters);
}

void customTextCtrl::allow_float(bool allow_float)
{
    this->accept_float = allow_float;
    Clear();
}

void customTextCtrl::on_text_change(wxCommandEvent &event)
{

    wxString input = GetValue();
    wxString filtered_value_of_input;

    std::string characters_to_preserve_string = "0123456789";
    if (this->accept_float)
        characters_to_preserve_string += '.';
    std::vector<char> characters_to_preserve(characters_to_preserve_string.begin(), characters_to_preserve_string.end());
    if (input[0] == '-' or input[0] == '+')
        filtered_value_of_input += input[0];
    for (wxChar ch : input)
    {
        if (std::find(characters_to_preserve.begin(), characters_to_preserve.end(), ch) != characters_to_preserve.end())
            filtered_value_of_input += ch;
    }

    ChangeValue(filtered_value_of_input);
    event.Skip();
}