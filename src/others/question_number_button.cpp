#include "question_number_button.h"

wxBEGIN_EVENT_TABLE(questionNumberButton, wxButton)
EVT_PAINT(questionNumberButton::OnPaint)
EVT_LEFT_DOWN(questionNumberButton::OnLeftDown)
EVT_LEFT_UP(questionNumberButton::OnLeftUp)
wxEND_EVENT_TABLE()

questionNumberButton::questionNumberButton(wxWindow* parent, int id, const wxString& label, wxColor camouflage_color, std::string current_state)
    : wxButton(parent, id, label), camouflage_color(camouflage_color), current_state(current_state)
{
    this->SetBackgroundColour(camouflage_color);
    SetMinSize(wxSize(51, 51));
    SetFont(wxFont(25, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN));
    Bind(wxEVT_ENTER_WINDOW, &questionNumberButton::on_button_enter, this);
    Bind(wxEVT_LEAVE_WINDOW, &questionNumberButton::on_button_leave, this);
    update_button_appearance();
}

void questionNumberButton::set_state(std::string state)
{
    this->current_state = state;
    update_button_appearance();
}

void questionNumberButton::update_button_appearance()
{
    if (this->current_state == "nv")
    {
        this->background_color = wxColour(255, 255, 255);
        this->border_color = wxColor(0, 0, 0);
        make_circle(false);
    }
    else if (this->current_state == "n")
    {
        this->background_color = wxColour(255, 0, 0);
        this->border_color = wxColor(255, 0, 0);
        make_circle(false);
    }
    else if (this->current_state == "ar")
    {
        this->background_color = wxColour(0, 255, 0);
        this->border_color = wxColor(128, 0, 128);
        make_circle(true);
    }
    else if (this->current_state == "a")
    {
        this->background_color = wxColour(0, 255, 0);
        this->border_color = wxColor(0, 255, 0);
        make_circle(false);
    }
    else if (this->current_state == "nr")
    {
        this->background_color = wxColor(255, 0, 0);
        this->border_color = wxColour(128, 0, 128);
        make_circle(true);
    }
}


void questionNumberButton::make_circle(bool is_circular)
{
    this->is_circular = is_circular;
}


void questionNumberButton::on_button_enter(wxMouseEvent& event)
{
    if (!this->is_pressed) {
        SetCursor(wxCursor(wxCURSOR_HAND));
    }
}

void questionNumberButton::on_button_leave(wxMouseEvent& event)
{
    if (!this->is_pressed) {
        SetCursor(wxCursor(wxCURSOR_ARROW));
    }
}

void questionNumberButton::selected(bool selected)
{
    //TODO fill gradient if selected
    this->is_selected = selected;
    if (selected)
    {
        SetFont(wxFont(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MAX));
    }
    else
    {
        SetFont(wxFont(23, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN));
    }
}

void questionNumberButton::OnLeftDown(wxMouseEvent& event)
{
    this->is_pressed = true;
    Refresh();
    event.Skip();
}

void questionNumberButton::OnLeftUp(wxMouseEvent& event)
{
    this->is_pressed = false;
    Refresh();
    event.Skip();
}

void questionNumberButton::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    wxCoord width, height;
    GetSize(&width, &height);

    wxCoord radius = wxMin(width, height) / 2;

    // Draw a ellipse or rectangle based on the current state
    dc.SetBrush(wxBrush(this->background_color));
    dc.SetPen(wxPen(this->border_color, 9));

    float scaling_factor = 0.9f; //must be less than or equal to 1
    if (this->is_pressed)
        scaling_factor = 0.73f;
    auto drawing_area = wxRect(((1 - scaling_factor) * width) / 2, ((1 - scaling_factor) * height) / 2, width * scaling_factor, height * scaling_factor);

    if (this->is_circular) {
        dc.DrawEllipse(drawing_area);
    }
    else {
        dc.DrawRectangle(drawing_area);
    }

    // Draw button label
    dc.SetTextForeground(*wxBLACK);
    dc.SetFont(GetFont());
    wxString labelText = this->use_label_value ? wxString::Format("%d", this->label_value) : GetLabel();
    wxSize labelSize = dc.GetTextExtent(labelText);

    dc.DrawText(labelText, (width - labelSize.GetWidth()) / 2, (height - labelSize.GetHeight()) / 2);
}
