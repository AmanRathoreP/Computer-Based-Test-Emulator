#pragma once
#include <wx/wx.h>

class mainFrame :public wxFrame
{
public:
    mainFrame(const wxString& title);
    enum
    {
        ID_create_test = 1,
        ID_about_author = 2
    };

private:
    void on_exit_clicked(wxCommandEvent& event);
    void on_about_clicked(wxCommandEvent& event);
    void on_about_author_clicked(wxCommandEvent& event);
    void on_create_test_clicked(wxCommandEvent& event);

};

