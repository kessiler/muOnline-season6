#pragma once
// -------------------------------------------------------------------------------

#define MAX_SHOPPOINTEX_EVENT		8
#define MAX_SHOPPOINTEX_MONSTER		1024
// -------------------------------------------------------------------------------

namespace ShopPointExEvent
{
	enum T
	{
		BC	= 0,
		DS	= 1,
		CC	= 2,
		IT	= 3,
		CW	= 4,
		CS	= 5,
		DG	= 6,
		IG	= 7,
	};
};
// -------------------------------------------------------------------------------

struct SHOPPOINTEX_EVENT
{
	int		AddCoinC;
	int		AddCoinP;
	int		AddCoinG;
};
// -------------------------------------------------------------------------------

struct SHOPPOINTEX_MONSTER
{
	DWORD	MonsterID;
	int		MapNumber;
	int		AddCoinC;
	int		AddCoinP;
	int		AddCoinG;
};
// -------------------------------------------------------------------------------

class ShopPointEx
{
public:
			ShopPointEx();
			~ShopPointEx();
	// ----
	void	Init();
	void	Load();
	void	ReadData(char * File);
	// ----
	int		GetMonsterSlot(int Class, int Map);
	// ----
	void	AddTimeBonus(int UserIndex);
	void	AddEventBonus(int UserIndex, ShopPointExEvent::T Event);
	void	AddMonsterBonus(int UserIndex, int MonsterIndex);
	// ----
	void	SendNotify(int UserIndex, int CoinC, int CoinP, int CoinG);
	// ----
private:
	bool	m_IsLoaded;
	bool	m_TimeBonusOn;
	int		m_TimeBonusAdd;
	DWORD	m_TimeBonusInterval;
	int		m_MonsterLoaded;
	// ----
	SHOPPOINTEX_EVENT	m_EventBonus[MAX_SHOPPOINTEX_EVENT];
	SHOPPOINTEX_MONSTER m_MonsterBonus[MAX_SHOPPOINTEX_MONSTER];
	// ----
}; extern ShopPointEx g_ShopPointEx;
// -------------------------------------------------------------------------------