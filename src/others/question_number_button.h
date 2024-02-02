#pragma once
#include <wx/wx.h>
#include <wx/region.h>

enum button_states
{
    not_visited,
    answered,
    not_answered,
    answered_and_marked_for_review,
    not_answered_but_marked_for_review
};

class questionNumberButton : public wxButton
{
public:
    questionNumberButton(wxWindow *parent, wxWindowID id, const wxString &label);

    void set_state(button_states state);
    void set_glow(bool glow);
    void selected(bool selected = true);

private:
    button_states current_state;

    void update_button_appearance();

    void set_color(const wxColour &color);
    void make_circle(bool is_circular);
    void set_boundary_color(const wxColour &color);

    void on_button_enter(wxMouseEvent &event);
    void on_button_leave(wxMouseEvent &event);
};
