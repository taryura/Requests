#include "POP3.h"

POP3Frame::POP3Frame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    // creating a menu bar
/*    mbar = new wxMenuBar();

    CreateFileMenu ();
    CreateEditMenu();
    CreateHelpMenu ();

    SetMenuBar(mbar);

    //creating toolbars

    wxImage::AddHandler(new wxPNGHandler);

    //assigning icons and creating toolbars
    CreateToolbars();

    // create a status bar with some information about used wxWidgets version
    CreateStatusBar(3);
    SetStatusText(_("Requests"),0);
    SetStatusText(_("Ver. 0.2.1 Alpha"), 1);
    SetStatusText(_("Ready"),2);
*/
}

POP3Frame::~POP3Frame()
{
}
