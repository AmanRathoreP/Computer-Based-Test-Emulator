#include "progress_bar.h"

progressBar::progressBar(wxWindow *parent, wxSize bar_size, unsigned int max_val, const std::map<unsigned int, wxColor> color_range_for_values, wxColor bar_color, bool overlay, wxWindowID id, const wxPoint &pos,
                         const wxSize &size, long style, const wxString &name)
    : wxPanel(parent, id, pos, size, style, name), max_val(max_val), bar_color(bar_color), show_remaining_time(overlay)
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

    wxColor color_to_use = this->bar_color;
    auto it = this->color_range_for_values.upper_bound(this->current_val);
    if (it != this->color_range_for_values.begin())
    {
        --it;
        color_to_use = it->second;
    }
    
    dc.SetBrush(wxBrush(color_to_use));
    dc.DrawRectangle(wxRect(0, 0, (int)(width * (((float)this->current_val) / (this->max_val))), height));

    for (const auto& indicator : this->color_range_for_values) {
        dc.SetBrush(wxBrush(indicator.second));
        dc.DrawRectangle(wxRect((width * (((float)indicator.first) / this->max_val))-(this->indicator_width/2), 0, this->indicator_width, height));
    }

    if (this->show_remaining_time)
    {
        dc.SetTextForeground(*wxBLACK);
        dc.SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_HEAVY));
        wxString labelText = this->current_val > this->max_val ? "+" + format_time(this->current_val - this->max_val) : format_time(this->max_val - this->current_val);
        wxSize labelSize = dc.GetTextExtent(labelText);
        dc.DrawText(labelText, (width - labelSize.GetWidth()) / 2, (height - labelSize.GetHeight()) / 2);
    }
}

void progressBar::update(unsigned int val)
{
    this->current_val = val;
    Refresh();
}

wxString progressBar::format_time(unsigned int seconds)
{
   if (seconds < 60)
        return wxString::Format("%02d secs", seconds);
    else if (seconds < 3600)
        return wxString::Format("%02d:%02d", seconds / 60, seconds % 60);
    else if (seconds < 3600 * 24)
       return wxString::Format("%02d:%02d:%02d", seconds / 3600, (seconds / 60) % 60, seconds % 60);

    return wxString::Format("%04d:%02d:%02d", seconds / 3600, (seconds / 60) % 60, seconds % 60);
}