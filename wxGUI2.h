/***************************************************************
 * Name:      wxGUI2.h
 * Purpose:   Defines Application Event IDs and their handlers
 * Author:    Iurii Taranenko (taryura@gmail.com)
 * Created:   2018-03-05
 * Copyright: Iurii Taranenko ()
 * License:
 **************************************************************/

#ifndef WXGUI2MAIN_H
#define WXGUI2MAIN_H

#include "EventRedirect.h"

class wxGUI2Frame: public EventRedirect
{
    public:
        wxGUI2Frame(wxFrame *frame, const wxString& title);
        wxGUI2Frame(wxFrame *frame, int a, const wxString& title);
        ~wxGUI2Frame();
        wxMenuBar *mbar;

    private:

        //declaring event IDs
        enum
        {
            idMenuQuit = 1000,

            //File menu
            idMenuFile_New,
            idMenuFile_Send,
            idMenuFile_Open,
            idMenuFile_Save,
            idMenuFile_Import_FromFile,
            idMenuFile_Import_FromURL,

            //Edit Menu
            idMenuEdit_Copy,
            idMenuEdit_Cut,
            idMenuEdit_Paste,

            //Help Menu
            idMenuAbout

        };
        //declaring event handlers

        //general Close event
        void OnClose(wxCloseEvent& event);

        //file menu

        void OnNew(wxCommandEvent& event);
        void OnSend(wxCommandEvent &event);
        void OnOpen(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);
        void OnImpFromFile(wxCommandEvent& event);
        void OnImpFromURL(wxCommandEvent& event);
        void OnQuit(wxCommandEvent& event);

        //Edit menu
        void OnCopy(wxCommandEvent& event);
        void OnCut(wxCommandEvent& event);
        void OnPaste(wxCommandEvent& event);



        //Help Menu
        void OnAbout(wxCommandEvent& event);

        //Menu creation methods of the main class
        void CreateFileMenu();
        void CreateEditMenu();
        void CreateHelpMenu();

        //Toolbars creation methods
        void CreateToolbars();
        //toolbars and UI interface elements pointers
        wxToolBar *toolbar1;
        wxToolBar *toolbar2;
        wxTextCtrl *tc_request;
        wxTextCtrl *tc_response;
        wxTextCtrl *tc_url;
        wxTextCtrl *tc_port;

        //Override function - redirects CUT COPY PASTE events from main frame
        //to the focused text control


        DECLARE_EVENT_TABLE()
};


#endif // WXGUI2MAIN_H
