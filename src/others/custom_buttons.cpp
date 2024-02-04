#include "custom_buttons.h"

customButton::customButton(wxWindow *parent,
                            wxWindowID id,
                            const wxString &label,
                            const bool selectable,
                            const unsigned short int deselected_font_size,
                            const unsigned short int selected_font_size,
                            const wxColor deselected_background_color,
                            const wxColor selected_background_color,
                            const wxColor deselected_foreground_color,
                            const wxColor selected_foreground_color):
                            wxButton(parent, id, label),
                                current_state(false),
                                selectable(selectable),
                                deselected_font_size(deselected_font_size),
                                selected_font_size(selected_font_size),
                                deselected_background_color(deselected_background_color),
                                selected_background_color(selected_background_color),
                                deselected_foreground_color(deselected_foreground_color),
                                selected_foreground_color(selected_foreground_color)
{

    SetFont(wxFont(this->deselected_font_size, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    this->SetBackgroundColour(this->deselected_background_color);
    this->SetForegroundColour(this->deselected_foreground_color);
    Bind(wxEVT_ENTER_WINDOW, &customButton::on_button_enter, this);
    Bind(wxEVT_LEAVE_WINDOW, &customButton::on_button_leave, this);
}

void inline customButton::on_button_enter(wxMouseEvent &event)
{
    SetCursor(wxCursor(wxCURSOR_HAND));
}

void inline customButton::on_button_leave(wxMouseEvent &event)
{
    SetCursor(wxCursor(wxCURSOR_ARROW));
}

void customButton::set_selected(bool selected)
{
    this->current_state = selected and this->selectable;
    if (this->current_state)
    {
        this->SetBackgroundColour(this->selected_background_color);
        this->SetForegroundColour(this->selected_foreground_color);
        SetFont(wxFont(this->selected_font_size, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    }
    else
    {
        this->SetBackgroundColour(this->deselected_background_color);
        this->SetForegroundColour(this->deselected_foreground_color);
        SetFont(wxFont(this->deselected_font_size, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    }
}
