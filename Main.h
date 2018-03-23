/***************************************************************
 * Name:      Main.h
 * Purpose:   Defines Application Class
 * Author:    Iurii Taranenko (taryura@gmail.com)
 * Created:   2018-03-05
 * Copyright: Iurii Taranenko ()
 * License:
 **************************************************************/

#ifndef WXGUI2APP_H
#define WXGUI2APP_H

#include <wx/app.h>

class wxGUI2App : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // WXGUI2APP_H
