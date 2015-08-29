#include "stdafx.h"
#include "Settings.h"
#include "Window.h"
#include "ServerInit.h"
// -----------------------------------------------------------------------

Settings	gSettings;
// -----------------------------------------------------------------------

void Settings::ReadData(char * File)
{
    FILE * Data = fopen(File, "r");
    // ----
    if( !Data )
    {
        g_Window.PrintLog("[%s] Server settings load fail", File);
        MessageBox(NULL, "Enable to load Settings.ini", "Loading Error", ERROR);
    }
    else
    {
        this->ServerPort		= GetPrivateProfileIntA("Connect", "ServerPort", 44405, File);
        this->UpdatePort		= GetPrivateProfileIntA("Connect", "UpdatePort", 55557, File);
        // ----
        this->WhiteList			= GetPrivateProfileIntA("Settings", "WhiteList", 1, File);
        this->WriteLogs			= GetPrivateProfileIntA("Settings", "WriteLogs", 1, File);
        this->MaxServerCount	= GetPrivateProfileIntA("Settings", "MaxServerCount", 400, File);
        this->MaxLogCount		= GetPrivateProfileIntA("Settings", "MaxLogCount", 100, File);
        // ----
        GetPrivateProfileString("FTP", "IP", "NONE", FtpServer.Adress, 20, File);
        FtpServer.Port			= GetPrivateProfileInt("FTP", "Port", 80, File);
        GetPrivateProfileString("FTP", "Login", "", FtpServer.User, 20, File);
        GetPrivateProfileString("FTP", "Password", "", FtpServer.Pass, 20, File);
        GetPrivateProfileString("FTP", "VersionFile", "", FtpServer.VersionFileName, 20, File);
        GetPrivateProfileString("FTP", "ClientVersion", "0.00.00", this->FTPClientVersion, 10, File);
        // ----
        CString Text;
        Text.Format("%i", this->ServerPort);
        SetDlgItemText(g_Window.WindowID, IDC_PORTNUMBER, Text);
        // ----
        g_Window.PrintLog("[%s] Server settings loaded", File);
        fclose(Data);
    }
}
// -----------------------------------------------------------------------