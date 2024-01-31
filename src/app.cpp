#include "app.h"
#include "frames/main_frame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(myApp);

bool myApp::OnInit() {
	mainFrame* main_frame = new mainFrame("CBT Emulator");
	main_frame->Show();
	main_frame->SetSize(16*50, 9*50);
	main_frame->SetMinSize(wxSize(16 * 40, 9 * 40));
	main_frame->SetMaxSize(wxSize(16 * 70, 9 * 70));

	return true;
}