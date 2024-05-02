#include "progress_bar.h"

progressBar::progressBar(wxWindow *parent, wxSize bar_size, unsigned int max_val, const std::map<unsigned int, wxColor> color_range_for_values, wxWindowID id, const wxPoint &pos,
                              const wxSize &size, long style, const wxString &name)
    : wxPanel(parent, id, pos, size, style, name), max_val(max_val)
{
    this->SetBackgroundColour(wxColor(100, 100, 100));
    this->SetMaxSize(wxSize(bar_size.x, bar_size.y));
    this->SetMinSize(wxSize(bar_size.x, bar_size.y));

    for (const auto &pair : color_range_for_values)
        this->color_range_for_values[this->max_val - pair.first] = pair.second;

    Bind(wxEVT_PAINT, &progressBar::OnPaint, this);
}

void progressBar::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    wxCoord width, height;
    GetSize(&width, &height);

    wxColor color_to_use = wxColor(0, 0, 255);
    auto it = this->color_range_for_values.upper_bound(this->current_val);
    if (it != this->color_range_for_values.begin())
    {
        --it;
        color_to_use = it->second;
    }
    
    dc.SetBrush(wxBrush(color_to_use));
    dc.DrawRectangle(wxRect(0, 0, (int)(width * (((float)this->current_val) / (this->max_val))), height));
}

void progressBar::update(unsigned int val)
{
    this->current_val = val;
    Refresh();
}

