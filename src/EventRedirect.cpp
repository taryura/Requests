#include "EventRedirect.h"


EventRedirect::EventRedirect()
{
    //ctor
}

EventRedirect::EventRedirect (wxFrame *frame, int a, const wxString& title)
        : wxFrame(frame, a, title)
        {

        }
bool EventRedirect::ProcessEvent(wxEvent& event)
{
     static wxEvent* s_lastEvent = NULL;
     // Check for infinite recursion
     if (& event == s_lastEvent)
         return false;
     if (event.IsCommandEvent() &&
        !event.IsKindOf(CLASSINFO(wxChildFocusEvent)) &&
        !event.IsKindOf(CLASSINFO(wxContextMenuEvent)))
     {
         s_lastEvent = & event;
         wxControl *focusWin = wxDynamicCast(FindFocus(), wxControl);
         bool success = false;
         if (focusWin)
             success = focusWin->GetEventHandler()
                                 ->ProcessEvent(event);
         if (!success)
             success = wxFrame::ProcessEvent(event);
         s_lastEvent = NULL;
         return success;
     }

     else

     {
         return wxFrame::ProcessEvent(event);
     }
 }
