#pragma once
#include <wx/wx.h>

class customTextCtrl : public wxTextCtrl
{
public:
    customTextCtrl(wxWindow *parent, wxWindowID id = wxID_ANY,
                   const wxString &value = wxEmptyString,
                   const wxPoint &pos = wxDefaultPosition,
                   const wxSize &size = wxDefaultSize,
                   long style = 0,
                   unsigned short int max_number_of_characters = 8);

    void allow_float(bool allow_float = true);

private:
    bool accept_float;
    void on_text_change(wxCommandEvent &event);

    wxDECLARE_EVENT_TABLE();
};
