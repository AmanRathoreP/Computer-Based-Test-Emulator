#include "main_frame.h"
#include <wx/wx.h>

mainFrame::mainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxMenu* menu_advance = new wxMenu;
    menu_advance->Append(ID_create_test, "&Create Test\tCtrl+N");
    menu_advance->AppendSeparator();
    menu_advance->Append(wxID_EXIT, "&Close\tCtrl+W");

    wxMenu* menu_help = new wxMenu;
    menu_help->Append(wxID_ABOUT);
    menu_help->Append(ID_about_author, "About Author");

    wxMenuBar* menu_bar = new wxMenuBar;
    menu_bar->Append(menu_advance, "&Advance");
    menu_bar->Append(menu_help, "&Help");

    SetMenuBar(menu_bar);
    
    Bind(wxEVT_MENU, &mainFrame::on_about_clicked, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &mainFrame::on_about_author_clicked, this, ID_about_author);
    Bind(wxEVT_MENU, &mainFrame::on_exit_clicked, this, wxID_EXIT);
    Bind(wxEVT_MENU, &mainFrame::on_create_test_clicked, this, ID_create_test);
}

void mainFrame::on_exit_clicked(wxCommandEvent& event)
{
    Close(true);
}

void mainFrame::on_about_clicked(wxCommandEvent& event)
{
    wxMessageBox("This apps helps one to create and emulate test for sake of practicing",
        "About CBT Emulator", wxOK | wxICON_INFORMATION);
}

void mainFrame::on_about_author_clicked(wxCommandEvent& event)
{
    wxMessageBox("This app is created by Aman whose github id is AmanRathoreP",
        "About Author", wxOK | wxICON_INFORMATION);
}

void mainFrame::on_create_test_clicked(wxCommandEvent& event)
{
    wxMessageBox("The facility will be implemented soon!",
        "Test Creation", wxOK | wxICON_WARNING);
}


