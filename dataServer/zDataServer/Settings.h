#pragma once
// -----------------------------------------------------------------------

class Settings
{
public:
	unsigned int ServerPort;
	unsigned int MaxThreadCount;
	unsigned int MaxLogCount;
	unsigned int MaxBuffSize;
	unsigned int MaxServerCount;
	unsigned int CreateNewAccounts;
	unsigned int UseDefaultClass;
	// ----
	char	ConnectDNS[52];
	char	ConnectLogin[52];
	char	ConnectPassword[52];
	// ----
	void	ReadData(char * File);
	// ----
}; extern Settings gSettings;
// ----------------------------------------------------------------------