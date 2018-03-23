/***************************************************************
 * Name:      events.cpp
 * Purpose:   Defines Event table
 * Author:    Iurii Taranenko (taryura@gmail.com)
 * Created:   2018-03-06
 * Copyright: Iurii Taranenko ()
 * License:
 **************************************************************/

#include "wxGUI2.h"

BEGIN_EVENT_TABLE(wxGUI2Frame, wxFrame)
    EVT_CLOSE(wxGUI2Frame::OnClose)

    //file menu

    EVT_MENU(idMenuFile_New, wxGUI2Frame::OnNew)
    EVT_MENU(idMenuFile_Open, wxGUI2Frame::OnOpen)
    EVT_MENU(idMenuFile_Save, wxGUI2Frame::OnSave)
    //import sub menu
    EVT_MENU(idMenuFile_Import_FromFile, wxGUI2Frame::OnImpFromFile)
    EVT_MENU(idMenuFile_Import_FromURL, wxGUI2Frame::OnImpFromURL)
    //quit
    EVT_MENU(idMenuQuit, wxGUI2Frame::OnQuit)

    //edit menu
    EVT_MENU(idMenuEdit_Copy, wxGUI2Frame::OnCopy)
    EVT_MENU(idMenuEdit_Cut, wxGUI2Frame::OnCut)
    EVT_MENU(idMenuEdit_Paste, wxGUI2Frame::OnPaste)

    //help menu
    EVT_MENU(idMenuAbout, wxGUI2Frame::OnAbout)

END_EVENT_TABLE()
