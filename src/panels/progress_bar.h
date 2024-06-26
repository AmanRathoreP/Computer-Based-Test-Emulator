#pragma once
#include <wx/wx.h>
#include <map>

class progressBar : public wxPanel
{
public:
    progressBar(wxWindow *parent, wxSize bar_size, unsigned int max_val, const std::map<unsigned int, wxColor> color_range_for_values, wxColor bar_color = wxColor(0,0,0), bool overlay = false, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxPanelNameStr);
    void update(unsigned int val);

private:
    unsigned int max_val;
    unsigned int current_val = 0;
    unsigned short int indicator_width = 6;
    std::map<unsigned int, wxColor> color_range_for_values;
    wxColor bar_color;
    bool show_remaining_time = false;

    void OnPaint(wxPaintEvent &event);
    wxString format_time(unsigned int);
};
