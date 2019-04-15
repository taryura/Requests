#include "POP3.h"


void POP3Frame::CreateTextCtrls()

{
    wxPanel *panel = new wxPanel(this, -1);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    tc_response = new wxTextCtrl(panel, wxID_ANY, wxT(""),
        wxPoint(-1, -1), wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);

    tc_request = new wxTextCtrl(panel, wxID_ANY, wxT("Enter your command here"),
        wxPoint(-1, -1), wxDefaultSize, wxTE_PROCESS_ENTER);
    Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(POP3Frame::OnEnter));

    vbox->Add(tc_response, 1, wxEXPAND);
    vbox->Add(-1, 10);

    vbox->Add(tc_request, 0, wxEXPAND);
    vbox->Add(-1, 10);

    panel->SetSizer(vbox);

    Centre();
}

void POP3Frame::OnEnter(wxCommandEvent &event)

{

    wxString tc_request_val = tc_request->GetValue();
    if (tc_request_val != "")
    {
        wxString tc_response_val = tc_response->GetValue();

        if (!cnctor)
        {
            std::shared_ptr<ssl_connect> temp_cnctor(new  ssl_connect("pop3.mail.ru", "995"));
            cnctor = temp_cnctor;
            tc_response->SetValue(tc_response_val + tc_request_val + "\r\n"+cnctor->replyreceived +"\r\n");
            tc_request->SetValue("");
            return;
        }

        std::string to_send;
        if (cnctor->connected)
        {
            to_send = std::string(tc_request_val.mb_str()) + "\r\n";
            cnctor->connect_send (to_send);
            tc_response->SetValue(tc_response_val + tc_request_val + "\r\n"+cnctor->replyreceived +"\r\n");
            tc_request->SetValue("");
        }

        /*if (!cnctor->authorized)
        {
            std::string to_send = std::string(tc_request_val.mb_str()) + "\r\n";
            cnctor->connect_send (to_send);
        }

        std::stringstream aaa;
        aaa << "Sent: " << to_send;

        aaa << cnctor->connected << "\r\n";
        aaa << cnctor << "\r\n";
        aaa<< cnctor->replyreceived << "\r\n";
        tc_response->SetValue(tc_response_val + tc_request_val + "\r\n"+cnctor->replyreceived +"\r\n" + aaa.str());
        tc_request->SetValue("");*/

    }

    //wxString msg = "You pressed enter";
    //wxMessageBox(msg, _("Welcome to..."));
}
