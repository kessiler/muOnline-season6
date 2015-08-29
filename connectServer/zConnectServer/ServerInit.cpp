#include "stdafx.h"
#include "ServerInit.h"
#include "ServerList.h"
#include "giocp.h"
#include "Window.h"
#include "Settings.h"

ServerInfo FtpServer;
ServerInfo HttpServer;
WzUdp cUdpSoc;

BYTE XorKey[3] = {0,0,0};

void ServerInit()
{
    cUdpSoc.CreateSocket();
    cUdpSoc.RecvSet(gSettings.UpdatePort);
    cUdpSoc.Run();

    if( CreateGIocp(gSettings.ServerPort) == FALSE )
    {
        g_Window.PrintLog("[ERROR] Failed starting server on port [%d]",gSettings.ServerPort);
    }
}