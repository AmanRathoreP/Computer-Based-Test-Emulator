#include "selectable_buttons.h"

                selectableButton::selectableButton(wxWindow *parent, wxWindowID id, const wxString &label, const unsigned short int section_order, wxColor selected_background_color, wxColor neutral_background_color, wxColor selected_foreground_color, wxColor neutral_foreground_color, const wxPoint &pos, const wxSize &size, long style)
    : wxButton(parent, id, label, pos, size, style), selected(false), selected_background_color(selected_background_color), neutral_background_color(neutral_background_color), selected_foreground_color(selected_foreground_color), neutral_foreground_color(neutral_foreground_color), section_order(section_order)
{
    SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    Bind(wxEVT_ENTER_WINDOW, &selectableButton::on_button_enter, this);
    Bind(wxEVT_LEAVE_WINDOW, &selectableButton::on_button_leave, this);
}

void selectableButton::set_selected(bool isSelected)
{
    selected = isSelected;
    this->selection_change();
}

void selectableButton::selection_change(void)
{
    if (this->selected)
    {
        this->SetBackgroundColour(this->selected_background_color);
        this->SetForegroundColour(this->selected_foreground_color);
    }
    else
    {
        this->SetBackgroundColour(this->neutral_background_color);
        this->SetForegroundColour(this->neutral_foreground_color);
    }
}

void selectableButton::on_button_enter(wxMouseEvent& event)
{
    SetCursor(wxCursor(wxCURSOR_HAND));
}

void selectableButton::on_button_leave(wxMouseEvent& event)
{
    SetCursor(wxCursor(wxCURSOR_ARROW));
}
