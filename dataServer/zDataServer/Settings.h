#pragma once
// -----------------------------------------------------------------------

class Settings
{
public:
	int		ServerPort;
	int		MaxThreadCount;
	int		MaxLogCount;
	int		MaxBuffSize;
	int		MaxServerCount;
	bool	CreateNewAccounts;
	bool	UseDefaultClass;
	// ----
	char	ConnectDNS[52];
	char	ConnectLogin[52];
	char	ConnectPassword[52];
	// ----
	void	ReadData(char * File);
	// ----
}; extern Settings gSettings;
// -----------------------------------------------------------------------