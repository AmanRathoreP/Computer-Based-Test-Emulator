#pragma once
#include <wx/wx.h>

class questionNumberButton : public wxButton
{
public:
    questionNumberButton(wxWindow* parent, wxWindowID id, const wxString& label, wxColor camouflage_color, std::string current_state = "nv");

    void set_state(std::string state);
    void selected(bool selected = true);
    unsigned short int label_value = 0;
    bool use_label_value = false;

private:
    std::string current_state = "nv";
    bool is_circular = false;
    bool is_pressed = false;
    bool is_selected = false;
    wxColor camouflage_color;
    wxColor background_color = wxColor(255, 255, 255);
    wxColor border_color = wxColor(0, 0, 0);

    void update_button_appearance();

    void make_circle(bool is_circular);

    void on_button_enter(wxMouseEvent& event);
    void on_button_leave(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);

    void OnPaint(wxPaintEvent& event);

    wxDECLARE_EVENT_TABLE();
};