#include "stdafx.h"
#include "ServerPrc.h"

Window	gWindow;

BOOL CALLBACK Dialog_Main(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	switch(Message)
	{
	case WM_INITDIALOG:
		{
			gWindow.WindowID		= Window;
			gWindow.SetTitle(Window, "zDataServer [%s] :: Main", APP_VERSION_T);
			gWindow.PrintLine(1);
			gWindow.PrintText("\t\t\t\tWelcome to %s [%s]", APP_NAME, APP_VERSION_T);
			gWindow.PrintLine(1);	

			gSettings.ReadData(".\\Settings.ini");

			Init();
			SetTimer(Window, WM_UPDATER, 60000, NULL);
		}
		break;
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
			case ID_CONTROL_SETTINGS:
				{
					DialogBoxParam(gWindow.Instance, MAKEINTRESOURCE(IDD_SETTINGS), NULL, Dialog_Settings, NULL);
				}
				break;
				// --
			case ID_CONTROL_EXEC:
				{
					DialogBoxParam(gWindow.Instance, MAKEINTRESOURCE(IDD_SQLExec), NULL, Dialog_Exec, NULL);
				}
				break;
				// --
			case IDM_ABOUT:
				{
					DialogBoxParam(gWindow.Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, Dialog_About, NULL);
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
			case WM_UPDATER:	//-> 60
				{
					if( !gGensDBSet.UpdateRanking() )
					{
						gWindow.PrintLog("[ERROR] GensRankingUpdate");
					}
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

BOOL CALLBACK Dialog_Settings(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	switch(Message)
	{
	case WM_INITDIALOG:
		{
			gWindow.SetTitle(Window, "zDataServer [%s] :: Settings", APP_VERSION_T);
		}
		break;
	case WM_COMMAND:
		break;		
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

BOOL CALLBACK Dialog_About(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	switch(Message)
	{
	case WM_INITDIALOG:
		{
			gWindow.SetTitle(Window, "zDataServer [%s] :: About", APP_VERSION_T);
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

BOOL CALLBACK Dialog_Exec(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	switch(Message)
	{
	case WM_INITDIALOG:
		{
			gWindow.SetTitle(Window, "zDataServer [%s] :: Execute", APP_VERSION_T);
		}
		break;
		// --
	case WM_COMMAND:
		{
			switch(WParam)
			{
			case IDC_EXEC:
				{
					gWindow.SQLExec(Window);
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
	char		TextBuff[1024];
	char		TimeBuff[100];
	char		LogBuff[1125];
	// ----
	SYSTEMTIME	Time;
	GetLocalTime(&Time);
	// ----
	DWORD   * Extern	 = (DWORD*)&Text;
	HWND	TextBox		 = GetDlgItem(this->WindowID, IDC_LOGWINDOW);
	unsigned int Counter = SendMessageA(TextBox, EM_GETLINECOUNT, 0, 0);
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
	sprintf(LogBuff, "%s %s\r\n", TimeBuff, TextBuff);
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

void Window::SQLExec(HWND Window)
{
	int Length	= GetWindowTextLength(GetDlgItem(Window, IDC_SQLCode))+1;
	char * Text = (char*)malloc(sizeof(char)*Length);
	// ----
	if( Text != NULL ) 
	{
		GetDlgItemText(Window, IDC_SQLCode, Text, Length);
	}
	// ----
	gGensDBSet.SQLExec(Text);
	// ----
	char		TextBuff[255];
	char		TimeBuff[100];
	char		LogBuff[500];
	// ----
	SYSTEMTIME	Time;
	GetLocalTime(&Time);
	// ----
	DWORD   * Extern	= (DWORD*)&Text;
	HWND	TextBox		= GetDlgItem(Window, IDC_SQLConsole);
	int		Counter		= SendMessageA(TextBox, EM_GETLINECOUNT, 0, 0);
	// ----
	if( Counter > 50 )
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
	sprintf(LogBuff, "%s %s\r\n", TimeBuff, TextBuff);
	// ----
	SendMessage(TextBox, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
	SendMessage(TextBox, EM_SETSEL, (WPARAM)-1, (LPARAM)0);
	SendMessage(TextBox, EM_REPLACESEL, (WPARAM)TRUE, (LPARAM)LogBuff);
}
// -----------------------------------------------------------------------