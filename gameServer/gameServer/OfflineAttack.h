#pragma once
// -------------------------------------------------------------------------------

#define MAX_OFFATTACK_MAGIC		25
#define MAX_OFFATTACK_BUFF		15
#define MAX_OFFATTACK_MAP		20
// -------------------------------------------------------------------------------

struct OFFATTACK_CHAR_DATA
{
	bool	IsOffline;
	WORD	MagicNumber;
};
// -------------------------------------------------------------------------------

struct OFFATTACK_DATA
{
	BYTE	ForbiddenMaps[MAX_OFFATTACK_MAP];
	WORD	AllowMagic[MAX_TYPE_PLAYER][MAX_OFFATTACK_MAGIC];
	WORD	BuffMagic[MAX_TYPE_PLAYER][MAX_OFFATTACK_BUFF];
};
// -------------------------------------------------------------------------------

struct OFFLINEATTACK_REQ
{
	PBMSG_HEAD2 h;
	int			MagicNumber;
};
// -------------------------------------------------------------------------------

class OfflineAttack
{
public:
			OfflineAttack();
			~OfflineAttack();
	// ----
	void	Init();
	void	ReadListData(char * File);
	void	ReadMainData(char * File);
	void	Load();
	// ----
	bool	CheckMap(LPOBJ lpUser);
	bool	CheckSkill(LPOBJ lpUser, WORD MagicNumber);
	DWORD	GetAttackDelay(LPOBJ lpUser);
	// ----
	void	StartingAttack(int UserIndex, OFFLINEATTACK_REQ * aRecv);
	void	StoppingAttack(int UserIndex);
	static void TryUseSkill(void * ArgList);
	// ----
	bool	SetBuffPartyUser(int UserIndex, CMagicInf * lpMagic);
	// ----
	void	WizardAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime);
	void	KnigthAttack(int UserIndex, int TargetIndex, int iUseBuffEachXTime);
	void	ElfAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime);
	void	MagumsaAttack(int UserIndex, int TargetIndex);
	void	LordAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime);
	void	SummonerAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime);
	void	MonkAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime);
	// ----
	OFFATTACK_CHAR_DATA CharInfo[OBJMAX];
	// ----
private:
	int		m_MaxCastTime;
	int		m_MinLevel;
	OFFATTACK_DATA m_Data;
	// ----
}; extern OfflineAttack g_OfflineAttack;
// -------------------------------------------------------------------------------