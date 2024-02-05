#pragma once
#include <wx/wx.h>
#include <wx/region.h>

class questionNumberButton : public wxButton
{
public:
    questionNumberButton(wxWindow *parent, wxWindowID id, const wxString &label);

    void set_state(std::string state);
    void set_glow(bool glow);
    void selected(bool selected = true);

private:
    std::string current_state;

    void update_button_appearance();

    void set_color(const wxColour &color);
    void make_circle(bool is_circular);
    void set_boundary_color(const wxColour &color);

    void on_button_enter(wxMouseEvent &event);
    void on_button_leave(wxMouseEvent &event);
};
