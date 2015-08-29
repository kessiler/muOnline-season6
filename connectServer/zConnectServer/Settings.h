#pragma once
// -----------------------------------------------------------------------

class Settings
{
public:
    WORD	ServerPort;
    WORD	UpdatePort;
    char	FTPIP[20];
    WORD	FTPPort;
    char	FTPLogin[20];
    char	FTPPassword[20];
    char	FTPVersionFile[20];
    char	FTPClientVersion[20];
    bool	WriteLogs;
    int		MaxServerCount;
    int		MaxLogCount;
    bool	WhiteList;
    // ----
    void	ReadData(char * File);
    // ----
};
extern Settings gSettings;
// -----------------------------------------------------------------------