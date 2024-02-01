#pragma once
#include <wx/wx.h>

class selectableButton : public wxButton
{
public:
    selectableButton(wxWindow *parent, wxWindowID id, const wxString &label, wxColor selected_background_color = wxColor(19, 235, 76), wxColor neutral_background_color = wxColor(216, 218, 237), wxColor selected_foreground_color = wxColor(49, 50, 59), wxColor neutral_foreground_color = wxColor(7, 10, 8), const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0);

    void set_selected(bool isSelected);
    bool state() const;

private:
    bool selected;
    wxColor selected_background_color;
    wxColor neutral_background_color;
    wxColor selected_foreground_color;
    wxColor neutral_foreground_color;
    void on_button_clicked(wxCommandEvent &event);
    void on_paint(wxPaintEvent &event);

    // Declare event table
    wxDECLARE_EVENT_TABLE();
};
