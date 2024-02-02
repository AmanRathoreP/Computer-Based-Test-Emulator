#include "question_number_button.h"

questionNumberButton::questionNumberButton(wxWindow *parent, int id, const wxString &label)
    : wxButton(parent, id, label), current_state(not_visited)
{
    SetMinSize(wxSize(51, 51));
    SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN));
    Bind(wxEVT_ENTER_WINDOW, &questionNumberButton::on_button_enter, this);
    Bind(wxEVT_LEAVE_WINDOW, &questionNumberButton::on_button_leave, this);
}

void questionNumberButton::set_state(button_states state)
{
    current_state = state;
    update_button_appearance();
}

void questionNumberButton::set_glow(bool glow)
{
    if (glow)
    {
        SetBackgroundColour(wxColour(255, 255, 0)); // Yellow color for glow
    }
    else
    {
        update_button_appearance();
    }
}

void questionNumberButton::update_button_appearance()
{
    switch (current_state)
    {
    case not_visited:
        set_color(wxColour(255, 255, 255));
        make_circle(false);
        break;
    case answered:
        set_color(wxColour(0, 255, 0));
        make_circle(false);
        break;
    case not_answered:
        set_color(wxColour(255, 0, 0));
        make_circle(false);
        break;
    case answered_and_marked_for_review:
        set_color(wxColour(0, 255, 0));
        make_circle(true);
        set_boundary_color(wxColour(128, 0, 128));
        break;
    case not_answered_but_marked_for_review:
        set_color(wxColour(128, 0, 128));
        make_circle(true);
        break;
    }
}

void questionNumberButton::set_color(const wxColour &color)
{
    SetBackgroundColour(color);
}

void questionNumberButton::make_circle(bool is_circular)
{
    // TODO make this function work
    if (is_circular)
    {
    }
    else
    {
    }
}

void questionNumberButton::set_boundary_color(const wxColour &color)
{
    SetForegroundColour(color);
}

void questionNumberButton::on_button_enter(wxMouseEvent &event)
{
    SetCursor(wxCursor(wxCURSOR_HAND));
}

void questionNumberButton::on_button_leave(wxMouseEvent &event)
{
    SetCursor(wxCursor(wxCURSOR_ARROW));
}

void questionNumberButton::selected(bool selected)
{
    if (selected)
    {
        SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MAX));
    }
    else
    {
        SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN));
    }
}
