#pragma once
// -----------------------------------------------------------------------

#include "resource.h"
#include <io.h>
// -----------------------------------------------------------------------

#define WM_UPDATER					1000
// -----------------------------------------------------------------------

BOOL CALLBACK Dialog_Main(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
BOOL CALLBACK Dialog_About(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
BOOL CALLBACK Dialog_AddWhiteIP(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
BOOL CALLBACK Dialog_AddBlackIP(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
// -----------------------------------------------------------------------

class Window
{
public:
    Window();
    ~Window();
    // ----
    HWND		WindowID;
    HINSTANCE	Instance;
    // ----
    void		InitInstance(HINSTANCE Out)
    {
        this->Instance = Out;
        DialogBoxParam(this->Instance, MAKEINTRESOURCE(IDD_MAIN), NULL, Dialog_Main, NULL);
    }
    // ----
    void		PrintLog(const char * Text, ...);
    void		PrintText(const char * Text, ...);
    void		PrintLine(BYTE Type)
    {
        switch(Type)
        {
        case 0:
            this->PrintText("--------------------------------------------------------------------------------------------------------------------------------------------");
            break;
        // --
        case 1:
            this->PrintText("======================================================================");
            break;
        }
    }
    // ----
    void		SetTitle(HWND Window, const char * Text, ...);
    // ----
    void		UpdateInfo();
    // ----
};
extern Window g_Window;
// -----------------------------------------------------------------------