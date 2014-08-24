#pragma once
// -------------------------------------------------------------------------------

#ifdef __CUSTOMS__
// -------------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------------

#define MAX_MIXOPTION_DATA		100
// -------------------------------------------------------------------------------

struct MIXOPTION_DATA
{
	int	MixCode;
	int Money;
	int SuccessRate;
};
// -------------------------------------------------------------------------------

class MixOption
{
public:
			MixOption();
			~MixOption();
	// ----
	void	Init();
	void	ReadData(char * File);
	void	Load();
	// ---
	int		GetMoney(LPOBJ lpUser, int MixCode);
	int		GetSuccessRate(LPOBJ lpUser, int MixCode);
	void	GetOption(LPOBJ lpUser, int MixCode);
	// ----
private:
	int		m_LoadedCount;
	MIXOPTION_DATA m_Data[MAX_MIXOPTION_DATA];
	// ----
}; extern MixOption g_MixOption;
// -------------------------------------------------------------------------------

#endif
// -------------------------------------------------------------------------------