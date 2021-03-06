#ifndef POP3_H
#define POP3_H

#include "wxGUI2.h"
#include "ssl_connect.h"


class POP3Frame : public wxFrame
{
    public:
        POP3Frame(wxFrame *frame, const wxString& title);
        ~POP3Frame();

    private:
        std::shared_ptr<ssl_connect> cnctor;
        void CreateTextCtrls();
        void OnEnter(wxCommandEvent &event);
        wxTextCtrl *tc_request;
        wxTextCtrl *tc_response;

};

#endif // POP3_H
