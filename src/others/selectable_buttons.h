#pragma once
#include <wx/wx.h>

class selectableButton : public wxButton
{
public:
    selectableButton(wxWindow *parent, wxWindowID id, const wxString &label, const unsigned short int section_order, wxColor selected_background_color = wxColor(19, 235, 76), wxColor neutral_background_color = wxColor(216, 218, 237), wxColor selected_foreground_color = wxColor(49, 50, 59), wxColor neutral_foreground_color = wxColor(7, 10, 8), const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);

    void set_selected(bool isSelected);
    bool selected;
    unsigned short int section_order = 0;

private:
    wxColor selected_background_color;
    wxColor neutral_background_color;
    wxColor selected_foreground_color;
    wxColor neutral_foreground_color;
    void selection_change(void);

    void on_button_enter(wxMouseEvent& event);
    void on_button_leave(wxMouseEvent& event);
};
