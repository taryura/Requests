/***************************************************************
 * Name:      event_handlers.cpp
 * Purpose:   Code for Event handlers
 * Author:    Iurii Taranenko (taryura@gmail.com)
 * Created:   2018-03-06
 * Copyright: Iurii Taranenko ()
 * License:
 **************************************************************/


#include "wxGUI2.h"
#include "a_requests.h"

//General Close Event
void wxGUI2Frame::OnClose(wxCloseEvent &event)
{
    Destroy();
}


//File Menu

void wxGUI2Frame::OnNew(wxCommandEvent &event)
{

    wxString msg = "Supposed to create a new project";
    wxMessageBox(msg, _("Welcome to..."));

}

void wxGUI2Frame::OnSend(wxCommandEvent &event)
{
    //fetching request parameters and converting it to std::string format
    wxString tc_value = tc_url->GetValue();
    std::string url_addr = std::string(tc_value.mb_str());
    tc_value = tc_port->GetValue();
    std::string s_port = std::string(tc_value.mb_str());
    tc_value = tc_request->GetValue();
    std::string request1 = std::string(tc_value.mb_str());


    if (url_addr == ""){
        wxMessageBox("URL address should not be empty", _("Error!"));
    }
        else if (s_port == ""){
            wxMessageBox("Port should be 0 - 65535", _("Error!"));
        }
            else if (request1 == ""){
                wxMessageBox("Request field should not be empty", _("Error!"));
            }
                else {
                        SetStatusText(_("Sending request..."),2);
                        sslrequest requests;

                        if (s_port != "80"){
                        //Text in the status bar

                            requests.rqst_set (url_addr, s_port, request1);
                         }
                            else {
                                 requests.rqst_set (url_addr, request1);
                            }

                tc_response->SetValue(requests.replyreceived);
                SetStatusText(_("Finished processing the request"),2);

                }






    //wxString msg = "Supposed to send a request";
    //wxMessageBox(msg, _("Welcome to..."));

}

void wxGUI2Frame::OnOpen(wxCommandEvent &event)
{

      wxFileDialog * openFileDialog = new wxFileDialog(this);

  if (openFileDialog->ShowModal() == wxID_OK){
      wxString fileName = openFileDialog->GetPath();
      tc_request->LoadFile(fileName);
  }
    //wxString msg = "Supposed to open an existing project";
    //wxMessageBox(msg, _("Welcome to..."));
}

void wxGUI2Frame::OnSave(wxCommandEvent &event)
{

    wxString msg = "Supposed to save the project";
    wxMessageBox(msg, _("Welcome to..."));
}

//import submenu

void wxGUI2Frame::OnImpFromFile(wxCommandEvent &event)
{
    wxString msg = "Supposed to import an existing project from file";
    wxMessageBox(msg, _("Welcome to..."));
}

void wxGUI2Frame::OnImpFromURL(wxCommandEvent &event)
{
    wxString msg = "Supposed to import an existing project from website";
    wxMessageBox(msg, _("Welcome to..."));
}

//Quit
void wxGUI2Frame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}



//Edit Menu

void wxGUI2Frame::OnCopy(wxCommandEvent &event)
{
    wxString msg = "Supposed to copy";
    wxMessageBox(msg, _("Welcome to..."));
}

void wxGUI2Frame::OnCut(wxCommandEvent &event)
{
    wxString msg = "Supposed to cut";
    wxMessageBox(msg, _("Welcome to..."));
}

void wxGUI2Frame::OnPaste(wxCommandEvent &event)
{
    wxString msg = "Supposed to paste";
    wxMessageBox(msg, _("Welcome to..."));
}

void wxGUI2Frame::OnAbout(wxCommandEvent &event)
{
    wxString msg = "Hello my friend";
    wxMessageBox(msg, _("Welcome to..."));
}