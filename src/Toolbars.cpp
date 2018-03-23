/***************************************************************
 * Name:      Toolbars.cpp
 * Purpose:   Code for Toolbars items
 * Author:    Iurii Taranenko (taryura@gmail.com)
 * Created:   2018-03-06
 * Copyright: Iurii Taranenko ()
 * License:
 **************************************************************/

#include "wxGUI2.h"

void wxGUI2Frame::CreateToolbars()
{
    wxBitmap exit(wxT("exit.png"), wxBITMAP_TYPE_PNG);
    wxBitmap newb(wxT("new.png"), wxBITMAP_TYPE_PNG);
    wxBitmap open(wxT("open.png"), wxBITMAP_TYPE_PNG);
    wxBitmap save(wxT("save.png"), wxBITMAP_TYPE_PNG);

    wxPanel *panel = new wxPanel(this, -1);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    toolbar1 = new wxToolBar(panel, wxID_ANY, wxDefaultPosition,
		wxDefaultSize, wxBORDER_NONE|wxTB_FLAT|wxTB_TEXT);
    toolbar1->AddTool(idMenuFile_New, wxT("New"), newb);
    toolbar1->AddTool(idMenuFile_Open, wxT("Open"), open);
    toolbar1->AddTool(idMenuFile_Save, wxT(""), save);
    toolbar1->AddTool(wxID_EXIT, wxT("Exit application"), exit);
    toolbar1->Realize();

    vbox->Add(toolbar1, 0, wxEXPAND);

        Connect(wxID_EXIT, wxEVT_COMMAND_TOOL_CLICKED,
        wxCommandEventHandler(wxGUI2Frame::OnQuit));

    //SetSizer(vbox);

    wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hbox11 = new wxBoxSizer(wxHORIZONTAL);
    //URL text field
    wxStaticText *st_url =  new wxStaticText(panel, wxID_ANY,
        wxT("URL:"));
    hbox1->Add(st_url, 0, wxRIGHT, 8);

    tc_url = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(tc_url, 1);

    //Port text field
    wxStaticText *st_port =  new wxStaticText(panel, wxID_ANY,
        wxT("Port:"));
    hbox11->Add(st_port, 0, wxRIGHT, 8);

    tc_port = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition);
    hbox11->Add(tc_port, 0);

    vbox->Add(hbox1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(hbox11, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    vbox->Add(-1, 10);

    wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hbox3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox4 = new wxBoxSizer(wxVERTICAL);


    //Request
    wxStaticText *st_request = new wxStaticText(panel, wxID_ANY,
        wxT("Request:"));
    hbox3->Add(st_request, 0);
    tc_request = new wxTextCtrl(panel, wxID_ANY, wxT(""),
    wxPoint(-1, -1), wxDefaultSize, wxTE_MULTILINE);
    hbox3->Add(tc_request, 2, wxEXPAND);

    hbox2->Add(hbox3, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    hbox2->Add(1, 1);

    //Response
    wxStaticText *st_response = new wxStaticText(panel, wxID_ANY,
    wxT("Response:"));
    hbox4->Add(st_response, 0);
    tc_response = new wxTextCtrl(panel, wxID_ANY, wxT(""),
    wxPoint(-1, -1), wxDefaultSize, wxTE_MULTILINE);
    hbox4->Add(tc_response, 2, wxEXPAND);

    hbox2->Add(hbox4, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    vbox->Add(hbox2, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    vbox->Add(-1, 10);

    panel->SetSizer(vbox);

    Centre();


    return;
}
