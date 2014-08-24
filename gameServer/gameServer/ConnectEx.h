#pragma once
// -------------------------------------------------------------------------------

#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
// -------------------------------------------------------------------------------

#include "..\include\prodef.h"
// -------------------------------------------------------------------------------

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
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct CONNECTEX_CLOSE
{
	PBMSG_HEAD2	h;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

class ConnectEx
{
public:
	void	SendLogin(int UserIndex, CONNECTEX_LOGIN * Data);
	void	SendGame(int UserIndex);
	void	SendClose(int UserIndex);
	// ----
}; extern ConnectEx g_ConnectEx;
// -------------------------------------------------------------------------------

#endif
// -------------------------------------------------------------------------------