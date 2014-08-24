#pragma once
// -------------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------------

#define MAX_PVPZONE	30
// -------------------------------------------------------------------------------

struct PVPZONE_DATA
{
	WORD	MapNumber;
	BYTE	X1;
	BYTE	X2;
	BYTE	Y1;
	BYTE	Y2;
};
// -------------------------------------------------------------------------------

class PVPZone
{
public:
			PVPZone();
			~PVPZone();
	// ----
	void	Init();
	void	Load();
	void	ReadData(char * File);
	// ----
	bool	IsPVPZone(LPOBJ lpUser);
	// ----
private:
	PVPZONE_DATA m_Data[MAX_PVPZONE];
	int		m_LoadedCount;
	// ----
}; extern PVPZone g_PVPZone;
// -------------------------------------------------------------------------------