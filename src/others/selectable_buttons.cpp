#include "selectable_buttons.h"

wxBEGIN_EVENT_TABLE(selectableButton, wxButton)
    EVT_BUTTON(wxID_ANY, selectableButton::on_button_clicked)
        EVT_PAINT(selectableButton::on_paint)
            wxEND_EVENT_TABLE()

                selectableButton::selectableButton(wxWindow *parent, wxWindowID id, const wxString &label, wxColor selected_background_color, wxColor neutral_background_color, wxColor selected_foreground_color, wxColor neutral_foreground_color, const wxPoint &pos, const wxSize &size, long style)
    : wxButton(parent, id, label, pos, size, style), selected(false), selected_background_color(selected_background_color), neutral_background_color(neutral_background_color), selected_foreground_color(selected_foreground_color), neutral_foreground_color(neutral_foreground_color)
{
    SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &event)
         {
        // Deselect when clicking outside the button
        if (!GetClientRect().Contains(event.GetPosition())) {
            set_selected(false);
        }
        event.Skip(); });
}

void selectableButton::set_selected(bool isSelected)
{
    selected = isSelected;
    Refresh();
}

bool selectableButton::state() const
{
    return selected;
}

void selectableButton::on_button_clicked(wxCommandEvent &event)
{
    // Toggle the selected state when the button is clicked
    this->set_selected(!selected);
    event.Skip();
}

void selectableButton::on_paint(wxPaintEvent &event)
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