#include "stdafx.h"
#include "Window.h"
#include "Settings.h"
#include "ServerInit.h"
#include "ServerList.h"
#include "WhiteList.h"
#include "BlackList.h"
#include "Shellapi.h"
// -----------------------------------------------------------------------

Window	g_Window;
// -----------------------------------------------------------------------

Window::Window()
{
    CreateDirectory("Logs", 0);
}
// -----------------------------------------------------------------------

Window::~Window()
{

}
// -----------------------------------------------------------------------

BOOL CALLBACK Dialog_Main(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch(Message)
    {
    case WM_INITDIALOG:
    {
        g_Window.WindowID = Window;
        g_Window.SetTitle(Window, "%s [%s] :: Main", APP_NAME, APP_VERSION_T);
        g_Window.PrintLine(1);
        g_Window.PrintText("\t\t\t\tWelcome to %s [%s]", APP_NAME, APP_VERSION_T);
        g_Window.PrintLine(1);
        gSettings.ReadData(".\\Settings.ini");
        LoadConnectServerList(".\\ServerList.txt");
        g_WhiteList.Load();
        g_BlackList.Load();
        ServerInit();
        SetTimer(Window, WM_UPDATER, 1000, NULL);
    }
    break;
    // --
    case WM_COMMAND:
    {
        switch(WParam)
        {
        case IDC_CLEARCONSOLE:
        {
            SetWindowText(GetDlgItem(Window, IDC_LOGWINDOW), "");
        }
        break;
        // --
        case IDM_EXIT:
        {
            EndDialog(Window, false);
        }
        break;
        // --
        case IDM_ABOUT:
        {
            DialogBoxParam(g_Window.Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, Dialog_About, NULL);
        }
        break;
        // --
        case ID_RELOAD_SETTINGS:
        {
            gSettings.ReadData(".\\Settings.ini");
        }
        break;
        // --
        case ID_RELOAD_SERVERLIST:
        {
            LoadConnectServerList(".\\ServerList.txt");
        }
        break;
        // --
        case ID_WHITELIST_RELOADWHITELIST:
        {
            g_WhiteList.Load();
        }
        break;
        // --
        case ID_WHITELIST_ADDIP32781:
        {
            DialogBoxParam(g_Window.Instance, MAKEINTRESOURCE(IDD_SQLExec), NULL, Dialog_AddWhiteIP, NULL);
        }
        break;
        // --
        case ID_WHITELIST_ENABLE:
        {
            if( gSettings.WhiteList )
            {
                g_Window.PrintLog("[WhiteList] Status changed: Off");
                gSettings.WhiteList = false;
            }
            else
            {
                g_Window.PrintLog("[WhiteList] Status changed: On");
                gSettings.WhiteList = true;
            }
        }
        break;
        // --
        case ID_BLACKLIST_RELOADBLACKLIST:
        {
            g_BlackList.Load();
        }
        break;
        // --
        case ID_BLACKLIST_ADDIP:
        {
            DialogBoxParam(g_Window.Instance, MAKEINTRESOURCE(IDD_SQLExec), NULL, Dialog_AddBlackIP, NULL);
        }
        break;
        }
    }
    break;
    // --
    case WM_CLOSE:
    {
        EndDialog(Window, false);
    }
    break;
    // --
    case WM_TIMER:
    {
        switch(WParam)
        {
        case WM_UPDATER:
        {
            CheckServerState();
            g_Window.UpdateInfo();
        }
        break;
        }
    }
    break;
    }
    // ----
    return 0;
}
// -----------------------------------------------------------------------

BOOL CALLBACK Dialog_About(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch(Message)
    {
    case WM_INITDIALOG:
    {
        g_Window.SetTitle(Window, "%s [%s] :: About", APP_NAME, APP_VERSION_T);
        // ----
        CString Text;
        // ----
        Text.Format("%s", APP_NAME);
        SetDlgItemTextA(Window, IDC_APPNAME, Text);
        // ----
        Text.Format("%s", APP_VERSION_T);
        SetDlgItemTextA(Window, IDC_APPVERSION, Text);
        // ----
        Text.Format("%s", APP_DES);
        SetDlgItemTextA(Window, IDC_APPDES, Text);
        // ----
        Text.Format("%s", __DATE__);
        SetDlgItemTextA(Window, IDC_APPDATE, Text);
    }
    break;
    // --
    case WM_COMMAND:
    {
        switch(WParam)
        {
        case IDOK:
        {
            ShellExecute(NULL, "open", "http://z-team.pro", NULL, NULL, SW_SHOWNORMAL);
            EndDialog(Window, false);
        }
        break;
        }
    }
    break;
    // --
    case WM_CLOSE:
    {
        EndDialog(Window, false);
    }
    break;
    }
    // ----
    return 0;
}
// -----------------------------------------------------------------------

BOOL CALLBACK Dialog_AddWhiteIP(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch(Message)
    {
    case WM_INITDIALOG:
    {
        g_Window.SetTitle(Window, "%s [%s] :: WhiteList", APP_NAME, APP_VERSION_T);
    }
    break;
    // --
    case WM_COMMAND:
    {
        switch(WParam)
        {
        case IDC_EXEC:
        {
            int Length	= GetWindowTextLength(GetDlgItem(Window, IDC_SQLCode))+1;
            char * IP = (char*)malloc(sizeof(char)*Length);
            // ----
            if( IP != NULL )
            {
                GetDlgItemText(Window, IDC_SQLCode, IP, Length);
            }
            // ----
            g_WhiteList.AddIP(IP);
        }
        break;
        }
    }
    break;
    // --
    case WM_CLOSE:
    {
        EndDialog(Window, false);
    }
    break;
    }
    // ----
    return 0;
}
// -----------------------------------------------------------------------

BOOL CALLBACK Dialog_AddBlackIP(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{    
    switch(Message)
    {
    case WM_INITDIALOG:
    {
        g_Window.SetTitle(Window, "%s [%s] :: BlackList", APP_NAME, APP_VERSION_T);
    }
    break;
    // --
    case WM_COMMAND:
    {
        switch(WParam)
        {
        case IDC_EXEC:
        {
            int Length	= GetWindowTextLength(GetDlgItem(Window, IDC_SQLCode))+1;
            char * IP = (char*)malloc(sizeof(char)*Length);
            // ----
            if( IP != NULL )
            {
                GetDlgItemText(Window, IDC_SQLCode, IP, Length);
            }
            // ----
            g_BlackList.AddIP(IP);
        }
        break;
        }
    }
    break;
    // --
    case WM_CLOSE:
    {
        EndDialog(Window, false);
    }
    break;
    }
    // ----
    return 0;
}
// -----------------------------------------------------------------------

void Window::PrintLog(const char * Text, ...)
{
    char		TextBuff[1024]		= { 0 };
    char		TimeBuff[100]		= { 0 };
    char		LogBuff[1500]		= { 0 };
    char		LogFile[MAX_PATH]	= { 0 };
    // ----
    SYSTEMTIME	Time;
    GetLocalTime(&Time);
    // ----
    DWORD   * Extern	= (DWORD*)&Text;
    HWND	TextBox		= GetDlgItem(this->WindowID, IDC_LOGWINDOW);
    int		Counter		= SendMessageA(TextBox, EM_GETLINECOUNT, 0, 0);
    // ----
    if( Counter > gSettings.MaxLogCount )
    {
        int FirstLine	= SendMessage(TextBox, EM_LINEINDEX, 1, 0L);
        SendMessage(TextBox, EM_SETSEL, 0, MAKELONG(FirstLine, 0));
        SendMessage(TextBox, EM_REPLACESEL, 0, (long)(const char far*)"");
    }
    // ----
    Extern++;
    // ----
    sprintf(TimeBuff, "[%02d:%02d:%02d]", Time.wHour, Time.wMinute, Time.wSecond);
    // ----
    sprintf(TextBuff, Text,
            Extern[0], Extern[1], Extern[2], Extern[3], Extern[4],
            Extern[5], Extern[6], Extern[7], Extern[8]);
    // ----
    sprintf(LogBuff, "%s %s\n", TimeBuff, TextBuff);
    // ----
    if( gSettings.WriteLogs )
    {
        sprintf(LogFile, ".\\Logs\\%04d%02d%02d.log", Time.wYear, Time.wMonth, Time.wDay);
        FILE * Input = fopen(LogFile, "a");
        if (Input)
        {
            fprintf(Input, "%s", LogBuff);
            fclose(Input);
        }
    }
    // ----
    SendMessage(TextBox, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
    SendMessage(TextBox, EM_SETSEL, (WPARAM)-1, (LPARAM)0);
    SendMessage(TextBox, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)LogBuff);
}
// -----------------------------------------------------------------------

void Window::PrintText(const char * Text, ...)
{
    char		TextBuff[255];
    char		LogBuff[500];
    // ----
    SYSTEMTIME	Time;
    GetLocalTime(&Time);
    // ----
    DWORD   * Extern	= (DWORD*)&Text;
    HWND	TextBox		= GetDlgItem(this->WindowID, IDC_LOGWINDOW);
    int		Counter		= SendMessageA(TextBox, EM_GETLINECOUNT, 0, 0);
    // ----
    if( Counter > 40 )
    {
        int FirstLine	= SendMessage(TextBox, EM_LINEINDEX, 1, 0L);
        SendMessage(TextBox, EM_SETSEL, 0, MAKELONG(FirstLine, 0));
        SendMessage(TextBox, EM_REPLACESEL, 0, (long)(const char far*)"");
    }
    // ----
    Extern++;
    // ----
    sprintf(TextBuff, Text,
            Extern[0], Extern[1], Extern[2], Extern[3], Extern[4],
            Extern[5], Extern[6], Extern[7], Extern[8]);
    // ----
    sprintf(LogBuff, "%s\r\n", TextBuff);
    // ----
    SendMessage(TextBox, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
    SendMessage(TextBox, EM_SETSEL, (WPARAM)-1, (LPARAM)0);
    SendMessage(TextBox, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)LogBuff);
}
// -----------------------------------------------------------------------

void Window::SetTitle(HWND Window, const char * Text, ...)
{
    char		TextBuff[255];
    // ----
    DWORD   * Extern	= (DWORD*)&Text;
    // ----
    Extern++;
    // ----
    sprintf(TextBuff, Text,
            Extern[0], Extern[1], Extern[2], Extern[3], Extern[4],
            Extern[5], Extern[6], Extern[7], Extern[8]);
    // ----
    SetWindowTextA(Window, TextBuff);
}
// -----------------------------------------------------------------------

void Window::UpdateInfo()
{    
    CString GameServers;
    GameServers.Format("[%i / %i]", g_LiveGSCount, g_ServerListCount);
    SetDlgItemText(g_Window.WindowID, IDC_CONNECTEDCOUNT, GameServers);
    // ----
    if( !g_bJoinServerLive )
    {
        SetDlgItemText(g_Window.WindowID, IDC_CONNECTEDCOUNT2, "OFF");
    }
    else
    {
        SetDlgItemText(g_Window.WindowID, IDC_CONNECTEDCOUNT2, "ON");
    }
    // ----
    if( !gSettings.WhiteList )
    {
        SetDlgItemText(g_Window.WindowID, IDC_PORTNUMBER2, "OFF");
    }
    else
    {
        SetDlgItemText(g_Window.WindowID, IDC_PORTNUMBER2, "ON");
    }
}
// -----------------------------------------------------------------------