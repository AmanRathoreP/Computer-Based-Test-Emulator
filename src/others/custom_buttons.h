#pragma once
#include <wx/wx.h>

class customButton : public wxButton
{
public:
    customButton(wxWindow *parent,
                   wxWindowID id,
                   const wxString &label,
                   const bool selectable = false,
                   const unsigned short int deselected_font_size = 25,
                   const unsigned short int selected_font_size = 32,
                   const wxColor deselected_background_color = wxColor(255, 255, 255),
                   const wxColor selected_background_color = wxColor(120, 120, 120),
                   const wxColor deselected_foreground_color = wxColor(120, 120, 120),
                   const wxColor selected_foreground_color = wxColor(255, 255, 255));

    void set_selected(bool selected = true);
    bool current_state;

private:
    const bool selectable;
    const unsigned short int deselected_font_size;
    const unsigned short int selected_font_size;
    const wxColor deselected_background_color;
    const wxColor selected_background_color;
    const wxColor deselected_foreground_color;
    const wxColor selected_foreground_color;

    void inline on_button_enter(wxMouseEvent &event);
    void inline on_button_leave(wxMouseEvent &event);
};
