/***************************************************************
 * Name:      event_handlers.cpp
 * Purpose:   Code for Event handlers
 * Author:    Iurii Taranenko (taryura@gmail.com)
 * Created:   2018-03-06
 * Copyright: Iurii Taranenko ()
 * License:
 **************************************************************/

#include "wxGUI2.h"
#include "sslrequest.h"
#include "httprequest.h"
#include "POP3.h"


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
    request1 += "\r\n\r\n";
    //validation of port value
    input_validator check_result (s_port);

    /*if (request1.find("\r\n\r\n") == -1){

        wxMessageBox("Request header must end with at least two CRLF linebreaks. The content after the header may be ignored by server.", _("Error!"));
        return;
    }*/

    if (url_addr == ""){
        wxMessageBox("URL address should not be empty", _("Error!"));
    }
        else if (s_port == "" or !check_result.integer_flag){
            wxMessageBox("Port should be 0 - 65535", _("Error!"));
        }
            else if (request1 == "\r\n\r\n"){
                wxMessageBox("Request field should not be empty and should end with two line breaks.", _("Error!"));
            }
                else {
                        SetStatusText(_("Sending request..."),2);

                        if (s_port != "80"){
                        //Text in the status bar

                            sslrequest requests (url_addr, s_port, request1);
                            //requests.connect_set (request1);
                            tc_response->SetValue(requests.replyreceived);
                         }
                            else {
                                 httprequest requests;
                                 requests.connect_set (url_addr, s_port, request1);
                                 tc_response->SetValue(requests.replyreceived);
                            }


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
      wxFileDialog * openFileDialog = new wxFileDialog(this);

  if (openFileDialog->ShowModal() == wxID_OK){
      wxString fileName = openFileDialog->GetPath();
      tc_response->SaveFile(fileName);
  }


    /*wxMessageDialog *dial = new wxMessageDialog(NULL,
        wxT("Are you sure to quit?"), wxT("Question"),
        wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
    dial->ShowModal();

    wxString msg = "Supposed to save the project";
    wxMessageBox(msg, _("Welcome to...")); */
}

//import submenu

void wxGUI2Frame::OnImpFromFile(wxCommandEvent &event)
{

    //wxIMPLEMENT_APP_NO_MAIN(POP3App);

    	wxString theTitle;
	theTitle = wxGetTextFromUser(_("Enter a title"),
		_("Input text"), _("NO NAME"), this, -1, -1, true);

    POP3Frame *frame2 = new POP3Frame(0L, _(theTitle));
    //frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame2->Show();

    //wxString msg = "Supposed to import an existing project from file";
    //wxMessageBox(msg, _("Welcome to..."));
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
    tc_request->Paste();
    //wxString msg = "Supposed to paste";
    //wxMessageBox(msg, _("Welcome to..."));
}

void wxGUI2Frame::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxVERSION_STRING;
    wxMessageBox(msg, _("Welcome to..."));
}

