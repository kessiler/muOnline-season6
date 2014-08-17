#include "stdafx.h"
#include "Settings.h"
#include "Window.h"
// -----------------------------------------------------------------------

Settings	gSettings;
// -----------------------------------------------------------------------

void Settings::ReadData(char * File)
{
	FILE * Data = fopen(File, "r");
	// ----
	if( !Data )
	{
		gWindow.PrintLine(0);
		gWindow.PrintLog("Loading settings fail [%s]", File);
		gWindow.PrintLine(0);
	}
	else
	{
		this->ServerPort		= GetPrivateProfileIntA("Connect", "ServerPort", 55960, File);
		this->MaxThreadCount	= GetPrivateProfileIntA("Settings", "MaxThreadCount", 16, File);
		this->MaxLogCount		= GetPrivateProfileIntA("Settings", "MaxLogCount", 100, File);
		this->MaxServerCount	= GetPrivateProfileIntA("Settings", "MaxServerCount", 100, File);
		this->MaxBuffSize		= GetPrivateProfileIntA("Settings", "MaxBuffSize", 16384, File);
		this->CreateNewAccounts	= GetPrivateProfileIntA("Settings", "CreateAccounts", true, File);
		this->UseDefaultClass	= GetPrivateProfileIntA("Settings", "UseDefaultClass", true, File);
		// ----
		GetPrivateProfileString("Connect", "DNS", "MuOnline", this->ConnectDNS, sizeof(this->ConnectDNS), File);
		GetPrivateProfileString("Connect", "DBLogin", "Admin", this->ConnectLogin, sizeof(this->ConnectLogin), File);
		GetPrivateProfileString("Connect", "DBPassword", "thdzlqhem", this->ConnectPassword, sizeof(this->ConnectPassword), File);
		// ----
		CString Text;
		Text.Format("%i", this->ServerPort);
		SetDlgItemText(gWindow.WindowID, IDC_PORTNUMBER, Text);
		// ----
		Text.Format("%s", this->ConnectDNS);
		SetDlgItemText(gWindow.WindowID, IDC_DNSNAME, Text);
		// ----
		Text.Format("%i", this->MaxBuffSize);
		SetDlgItemText(gWindow.WindowID, IDC_BUFFSIZE, Text);
		// ----
		Text.Format("%i", this->MaxThreadCount);
		SetDlgItemText(gWindow.WindowID, IDC_THREADCOUNT, Text);
		// ----
		gWindow.PrintLog("Loading settings success [%s]", File);
	}
}
// -----------------------------------------------------------------------