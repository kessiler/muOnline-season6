#pragma once
// ----------------------------------------------------------------------------------------------

#include "ProtocolDefine.h"
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct CONNECTEX_LOGIN
{
	PBMSG_HEAD2	h;
	char		AccountID[10];
	char		Password[10];
	char		Name[10];
	DWORD		TickCount;
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------

#pragma pack(push, 1)
struct PMSG_ANS_SERVER_INFO
{
	PBMSG_HEAD2 h;
	char		Ip[16];
	WORD		Port;
};
#pragma pack(pop)
// ----------------------------------------------------------------------------------------------

namespace ConnectExType
{
	enum T
	{
		OnLine				= 0,
		OnReconnect			= 1,
		OnForceDisconnect	= 2,
	};
};
// ----------------------------------------------------------------------------------------------

class ConnectEx
{
public:
	void	Load();
	void	Run();
	// ----
	static int CreateConnect(char * IP, WORD Port);
	// ----
	void	ClearGame();
	void	RestoreGame();
	// ----
	void	DrawProgres();
	void	LoginReq();
	void	Reconnect();
	void	Relogin();
	// ----
	int		m_ConnectState;
	char	m_AccountID[11];
	char	m_Password[21];
	// ----
private:
	char	m_CSIP[16];
	WORD	m_CSPort;
	char	m_GSIP[16];
	WORD	m_GSPort;
	// ----
	DWORD	m_LastSendTick;
	DWORD	m_ReconnectTick;
	bool	m_WantMUHelper;
	// ----
}; extern ConnectEx gConnectEx;
// ----------------------------------------------------------------------------------------------