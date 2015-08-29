#if !defined(__SERVERINIT_H__)
#define __SERVERINIT_H__

#include "WzUdp.h"

typedef struct
{
    char Adress[200];
    WORD Port;
    char User[20];
    char Pass[20];
    char VersionFileName[20];

    BYTE Version1;
    BYTE Version2;
    BYTE Version3;

} ServerInfo, *LP_ServerInfo;

extern ServerInfo FtpServer;
extern ServerInfo HttpServer;
extern WzUdp cUdpSoc;
extern BYTE XorKey[3];


void ServerInit();
void LoadIniFile();

#endif