/***************************************************************
 * Name:      wxGUI2.cpp
 * Purpose:   Code for the main App Frame
 * Author:    Iurii Taranenko (taryura@gmail.com)
 * Created:   2018-03-05
 * Copyright: Iurii Taranenko ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "wxGUI2.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {

#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

wxGUI2Frame::wxGUI2Frame(wxFrame *frame, int a, const wxString& title)
    : EventRedirect(frame, a, title)
    {

    }

wxGUI2Frame::wxGUI2Frame(wxFrame *frame, const wxString& title)
    : EventRedirect(frame, -1, title)
{
    // creating a menu bar
    mbar = new wxMenuBar();

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

}

wxGUI2Frame::~wxGUI2Frame()
{
}
