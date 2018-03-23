/***************************************************************
 * Name:      menues.cpp
 * Purpose:   Code for App Menu items
 * Author:    Iurii Taranenko (taryura@gmail.com)
 * Created:   2018-03-06
 * Copyright: Iurii Taranenko ()
 * License:
 **************************************************************/

#include "wxGUI2.h"

void wxGUI2Frame::CreateFileMenu()
{
    wxMenu* fileMenu = new wxMenu(_T(""));

    //file menu
    fileMenu->Append(idMenuFile_New, _("&New\tCtrl-N"), _("New project"));
    fileMenu->Append(idMenuFile_Open, _("&Open\tCtrl-O"), _("Open project"));
    fileMenu->Append(idMenuFile_Save, _("&Open\tCtrl-S"), _("Save project"));
    fileMenu->AppendSeparator();

    wxMenu* imp = new wxMenu;
    //import sub menu
    imp->Append(idMenuFile_Import_FromFile, _("From &file"), _("Import project from file"));
    imp->Append(idMenuFile_Import_FromURL, _("From &URL"), _("Import project from website"));
    fileMenu->AppendSubMenu(imp, wxT("I&mport"));

    fileMenu->AppendSeparator();
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));
    return;
}

void wxGUI2Frame::CreateEditMenu()
{
    wxMenu* editMenu = new wxMenu(_T(""));
    editMenu->Append(idMenuEdit_Copy, _("&Copy\tCtrl-C"), _("Copy selected"));
    editMenu->Append(idMenuEdit_Cut, _("C&ut\tCtrl-X"), _("Cut selected"));
    editMenu->Append(idMenuEdit_Paste, _("&Paste\tCtrl-V"), _("Paste"));

    mbar->Append(editMenu, _("&Edit"));
    return;
}

void wxGUI2Frame::CreateHelpMenu()
{
    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));
    return;
}
