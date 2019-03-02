#ifndef EVENTREDIRECT_H
#define EVENTREDIRECT_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
//#include "winsock2.h"
#endif

#if defined(__WXMSW__)
#include "winsock2.h"
#endif

#include "Main.h"

class EventRedirect : public wxFrame
{
    public:
        EventRedirect();
        EventRedirect(wxFrame *frame, int a, const wxString& title);

    protected:

    private:
        bool ProcessEvent(wxEvent& event);
};

#endif // EVENTREDIRECT_H
