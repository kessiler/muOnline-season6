//-> Decompiled by DarkSim | 18.02.2013 | 1.01.00 GS-N (All names are original)
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------------

#define MASTER_MAX_LEVEL	200
#define MASTER_MAX_POINT	200
#define MASTER_ZEN_DEC		4
#define MASTER_MAX_LIST		200
// -------------------------------------------------------------------------------

struct MLP_ANS_MASTERLEVEL_INFO	//-> 100%
{
	PBMSG_HEAD	h;
	// ----
	BYTE		btResult;
	int			iUserIndex;
	char		szCharName[MAX_IDSTRING + 1];
	short		nMLevel;
	__int64		i64MLExp;
	__int64		i64NextMLExp;
	short		nMLPoint;
};
// -------------------------------------------------------------------------------

struct MLP_REQ_MASTERLEVEL_INFO	//-> 100%
{
    PBMSG_HEAD	h;
	// ----
    int			iUserIndex;
    char		szCharName[MAX_IDSTRING + 1];
};
// -------------------------------------------------------------------------------

struct MLP_REQ_MASTERLEVEL_INFOSAVE	//-> 100%
{
    PBMSG_HEAD	h;
	// ----
    char		szCharName[MAX_IDSTRING + 1];
    short		nMLevel;
    __int64		i64MLExp;
    __int64		i64NextMLExp;
    short		nMLPoint;
};
// -------------------------------------------------------------------------------

struct PMSG_MASTERLEVEL_INFO	//-> 100%
{
    PBMSG_HEAD	h;
	// ----
    BYTE		subcode;
    short		nMLevel;
    BYTE		btMLExp1;
    BYTE		btMLExp2;
    BYTE		btMLExp3;
    BYTE		btMLExp4;
    BYTE		btMLExp5;
    BYTE		btMLExp6; 
    BYTE		btMLExp7;
    BYTE		btMLExp8;
    BYTE		btMLNextExp1;
    BYTE		btMLNextExp2;
    BYTE		btMLNextExp3;
    BYTE		btMLNextExp4;
    BYTE		btMLNextExp5;
    BYTE		btMLNextExp6;
    BYTE		btMLNextExp7;
    BYTE		btMLNextExp8;
    short		nMLPoint;
    WORD		wMaxLife;
    WORD		wMaxMana;
    WORD		wMaxShield;
    WORD		wMaxBP;
};
// -------------------------------------------------------------------------------

struct PMSG_MASTERLEVEL_UP	//-> 100%
{
    PBMSG_HEAD	h;
	// ----
    BYTE		subcode;
    short		nMLevel;
    short		nAddMLPoint;
    short		nMLPoint;
    short		nMaxMLPoint;
    WORD		wMaxLife;
    WORD		wMaxMana;
    WORD		wMaxShield;
    WORD		wMaxBP;
};
// -------------------------------------------------------------------------------

class CMasterLevelSystem	//-> Complete
{
public:
					CMasterLevelSystem();
	virtual			~CMasterLevelSystem();
	// ----
	void			LoadData();
	void			ReadMonsterList(char * File);
	void			SetMasterLevelExpTlb();
	void			gObjNextMLExpCal(LPOBJ lpObj);
	int				MasterLevelUp(LPOBJ lpObj, __int64 iAddExp, bool bEventMapReward, int iMonsterType);
	int				IsMasterLevelUser(LPOBJ lpObj);
	int				CheckMLGetExp(LPOBJ lpObj, LPOBJ lpTargetObj);
	__int64			GetMasterLevelExpTlbInfo(int iMasterLevel);
	int				GetDieDecExpRate(LPOBJ lpObj);
	int				GetDieDecZenRate(LPOBJ lpObj);
	// ----
	void			GDReqMasterLevelInfo(LPOBJ lpObj);
	void			DGAnsMasterLevelInfo(BYTE * aRecv);
	void			GDReqMasterLevelInfoSave(LPOBJ lpObj);
	void			GCMasterLevelInfo(LPOBJ lpObj);
	void			GCMasterLevelUpInfo(LPOBJ lpObj);
	// ----
private:
	__int64			m_i64MasterLevelExpTlb[MASTER_MAX_LEVEL + 1];
    int				m_iMinMonsterKillLevel;
    float			m_fAddExpRate;
	bool			m_UseMonsterList;	//Custom
	int				m_MonsterList[MASTER_MAX_LIST];
	// ----
}; extern CMasterLevelSystem g_MasterLevelSystem;
// -------------------------------------------------------------------------------