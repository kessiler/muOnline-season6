// IllusionTempleProcess.h: interface for the CIllusionTempleProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ILLUSIONTEMPLEPROCESS_H__662704B5_1FB7_4420_AAA3_99B6D68D509D__INCLUDED_)
#define AFX_ILLUSIONTEMPLEPROCESS_H__662704B5_1FB7_4420_AAA3_99B6D68D509D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\include\prodef.h"
#include "..\common\zzzitem.h"
#include "partyclass.h"

class OBJECTSTRUCT;
typedef OBJECTSTRUCT* LPOBJECTSTRUCT;

#define MAX_ILLUSIONTEMPLE_TEMPLE_COUNT	6


static int g_IllusionTemple_Default_Exp[MAX_ILLUSIONTEMPLE_TEMPLE_COUNT] =
{
	300000,//IllusionTemple1
	340000,//IllusionTemple2
	380000,//IllusionTemple3
	420000,//IllusionTemple4
	460000,//IllusionTemple5
	500000 //IllusionTemple6
};

static int g_IllusionTemple_Relics_Point_Exp[MAX_ILLUSIONTEMPLE_TEMPLE_COUNT] =
{
	50000,//IllusionTemple1
	60000,//IllusionTemple2
	70000,//IllusionTemple3
	80000,//IllusionTemple4
	90000,//IllusionTemple5
	100000 //IllusionTemple6
};

static int g_IllusionTemple_Add_Kill_Exp[MAX_ILLUSIONTEMPLE_TEMPLE_COUNT] =
{
	600, //IllusionTemple1
	1200,//IllusionTemple2
	1800,//IllusionTemple3
	2400,//IllusionTemple4
	3000,//IllusionTemple5
	3500 //IllusionTemple6
};

static int g_IllusionTemple_Move_Relics_Exp[MAX_ILLUSIONTEMPLE_TEMPLE_COUNT] =
{
	1200,//IllusionTemple1
	2400,//IllusionTemple2
	3600,//IllusionTemple3
	4800,//IllusionTemple4
	6000,//IllusionTemple5
	7200 //IllusionTemple6
};

static int g_IllusionTemple_PartyUser_Point[MAX_PARTYUSER] =
{
	10,//1 Player
	20,//2 Players
	30,//3 Players
	40,//4 Players
	50 //5 Players
};

typedef struct __ILLUSION_TEMPLE_USER_DATA
{
	int m_nIndex;
	BYTE m_btTeam;
	BYTE m_btSkillPoint;
	DWORD m_dwSkillProdectionTime;
	DWORD m_dwSkillRestraintTime;
	BYTE m_btUserKillCount;
	BYTE m_btMonsterKillCount;
	BYTE m_btRelicsMoveCount;
	BYTE m_btUseSkillCount;
	__int64 m_nRewardExp;
	int m_nRankingPoint;
	int m_bReward;
	BYTE m_btItemPos;

	__ILLUSION_TEMPLE_USER_DATA()
	{
		m_nIndex = -1;
		m_btItemPos = -1;
	}
} __ILLUSION_TEMPLE_USER_DATA, *LP__ILLUSION_TEMPLE_USER_DATA;

typedef struct 
{
	int m_nIndex;
	short m_wPosNum;
} __ILLUSION_TAMPE_OBJECT_DATA, *LP__ILLUSION_TAMPE_OBJECT_DATA;

typedef struct 
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	char Name[10];
} PMSG_RELICS_GET_USER, *LPPMSG_RELICS_GET_USER;

typedef struct 
{
	PBMSG_HEAD2 h;
	WORD wRemainSec;
	WORD wUserIndex;
	BYTE btX;
	BYTE btY;
	BYTE btAlliedPoint;
	BYTE btIllusionPoint;
	BYTE btMyTeam;
	BYTE btPartyCount;
} PMSG_ILLUSION_TEMPLE_PLAY_STATE, *LPPMSG_ILLUSION_TEMPLE_PLAY_STATE;

typedef struct 
{
	WORD wPartyUserIndex;
	BYTE byMapNumber;
	BYTE btX;
	BYTE btY;
} PMSG_ILLUSION_TEMPLE_PARTY_POS, *LPPMSG_ILLUSION_TEMPLE_PARTY_POS;

typedef struct 
{
	PBMSG_HEAD2 h;
	BYTE btTempleNumber;
	BYTE btIllusionTempleState;
} PMSG_ILLUSION_TEMPLE_STATE, *LPPMSG_ILLUSION_TEMPLE_STATE;

typedef struct 
{
	PBMSG_HEAD2 h;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	WORD wObjIndex;
} PMSG_ILLUSION_TEMPLE_SKILL_END, *LPPMSG_ILLUSION_TEMPLE_SKILL_END;

typedef struct 
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	WORD wSourceObjIndex;
	WORD wTargetObjIndex;
} PMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT, *LPPMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT;

typedef struct 
{
	PBMSG_HEAD h;
	short nINDEX;
	char szAccountID[10+1];
	char szGameID[10+1];
	WORD wServerCode;
	int nTempleNumber;
	int nClass;
	int nLevel;
	BYTE btWin;
	int nPCBangGuid;
	int nRankPoint;
} PMSG_ANS_ILLUSION_TEMPLE_SCORE, *LPPMSG_ANS_ILLUSION_TEMPLE_SCORE;

typedef struct 
{
	PBMSG_HEAD2 h;
	BYTE btAlliedPoint;
	BYTE btIllusionPoint;
	BYTE btUserCount;
} PMSG_ILLUSION_TEMPLE_RESULT, *LPPMSG_ILLUSION_TEMPLE_RESULT;

typedef struct 
{
	char GameID[MAX_IDSTRING];
	BYTE btMapNumber;
	BYTE btTeam;
	BYTE btClass;
	int nAddExp;
} PMSG_ILLUSION_TEMPLE_USER_ADD_EXP, *LPPMSG_ILLUSION_TEMPLE_USER_ADD_EXP;

class CIllusionTempleProcess  
{
	CRITICAL_SECTION m_critUserData;

	__ILLUSION_TEMPLE_USER_DATA m_UserData[10];
	
	DWORD m_dwViewSkillState[10];
	
	int m_nUserCount;
	
	BYTE m_nTempleNumber;
	BYTE m_btIllusionTempleState;
	
	__ILLUSION_TAMPE_OBJECT_DATA m_StatusData[2];
	__ILLUSION_TAMPE_OBJECT_DATA m_RelicsBoxAlliedData;

	__ILLUSION_TAMPE_OBJECT_DATA m_RelicsBoxIllusionData;
	__ILLUSION_TAMPE_OBJECT_DATA m_MonsterData[40];
	
	BOOL m_bEntrance;
	
	int m_StatusRegenTime;
	int m_nRemainTimeMin;
	int m_nRemainTimeMSec;
	int m_nTickCount;
	int m_nNextTimeMin;
	int m_nEnventEndRemainTime;
	
	BOOL m_bGameEndPosMove;
	BOOL m_bSendMsgBeforeReady;
	BOOL m_bSendMsgBeforePlay;
	
	int m_nRelicsUserIndex;

	BYTE m_btRelicsTeam;
	BYTE m_btInventoryPos;

	int m_nStatusCount;
	int m_nMonsterCount;
	int m_nAlliedUserCount;
	int m_nIllusionUserCount;
	
	BYTE m_btAlliedPoint;
	BYTE m_btIllusionPoint;
	BYTE m_btWinTeam;

	BYTE m_btAlliedClassCnt[MAX_TYPE_PLAYER];
	BYTE m_btIllusionClassCnt[MAX_TYPE_PLAYER];

	BOOL m_bAllCheckSync;
	BOOL m_bSendResult;

public:
	CIllusionTempleProcess();
	virtual ~CIllusionTempleProcess();

	void ClearUserData(int nIndex);

	void SendIllusionTemplePlayState(BYTE btTeam);
	void IllusionTempleProcessInit(int nTempleNum);
	
	void CheckSync();

	void ProcIllusionTemple(int nCurTime);
	void ProcIllusionTempleState_None(int nCurTime);
	void ProcIllusionTempleState_Ready(int nCurTime);
	void ProcIllusionTempleState_Playing(int nCurTime);
	void ProcIllusionTempleState_End(int nCurTime);

	void SetIllusionTempleState(BYTE btState);
	BYTE GetIllusionTempleState();

	void SetIllusionTempleStateNone();
	void SetIllusionTempleStateReady();
	void SetIllusionTempleStatePlaying();
	void SetIllusionTempleStateEnd();

	void SeStatusInfo(BYTE btTempleIndex, int nIndex, short wPosNum);

	int EnterUserIllusionTemple(int aIndex, BYTE btTempleIndex, BYTE btItemPos);
	int LeaveUserIllusionTemple(int aIndex);
	
	void CheckUsersOnConnect();
	
	int CheckCanEnterLevel(int aIndex);
	
	BYTE GetRemainTimeMin();
	BYTE GetEnterRemainTimeMin();
	
	void IllusionHolyRelicsStatusAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
	
	void HolyRelicsBoxAlliedAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
	void HolyRelicsBoxIllusionAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
	
	void SetInventoryPos(int aIndex, BYTE btPos);
	void SendRelicsUser(LPOBJECTSTRUCT lpObj);
	int SearchRelicsItem(int iIndex);
	void DeleteRelicsItem(LPOBJECTSTRUCT lpObj);
	void DropRelicsItem(int iIndex);
	
	void SetRemainTime();
	
	void AddNpcPosNum(int nClass, int nPosNum);
	void AddMonsterPosNum(int nClass, int nPosNum);
	void ResetPosNum();

	void AddNpcStatus();
	void AddNpcRelicsBox();
	void AddMonsterGhost();
	
	void RemoveAllObj();
	
	void SetStatusRegenTime();
	void RegenStatus();
	void RegenMonster();
	
	void SendNoticeMessage(char *lpMsg);
	
	BYTE GetUserTeam(int nIndex);

	BYTE GetUserCount()
	{
		return m_nUserCount;
	}
	
	BOOL GetEntrance()
	{
		return m_bEntrance;
	}

	BYTE IncSkillPoint(int nIndex, BYTE btIncPoint);
	BYTE DecSkillPoint(int nIndex, BYTE btDecPoint);
	BYTE GetSkillPoint(int nIndex);
	
	int SetSkillPoint(int nIndex, BYTE btSkillPoint);
	void ResetSkillPoint();
	
	void IncUserKillCount(int nIndex);
	void IncMonsterKillCount(int nIndex);
	void ResetKillCount();
	
	void IncRelicsMoveCount(int nIndex);
	void ResetRelicsMoveCount();

	void IncUseSkillCount(int nIndex);
	void ResetUseSkillCount();
	
	void IllusionTempleUseSkill(int nIndex, WORD btSkillNumber, int nTargetIndex, BYTE btDis);
	void IllusionTempleSkillProc(LPOBJECTSTRUCT lpObj);
	
	void SendIllusionTempleSkillEnd(LPOBJECTSTRUCT lpObj, WORD btState);
	
	int UseSkillProdection(LPOBJECTSTRUCT lpObj);
	int UseSkillRestraint(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
	int UseSkillTeleport(LPOBJECTSTRUCT lpObj);
	int UseSkillShieldBurn(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpTargetObj);
	
	void SendUseSkillResult(int nIndex, int nTargetIndex, WORD wSkillNumber, BYTE btResult);

	void IllusionTempleUserDie(LPOBJECTSTRUCT lpObj);
	void CalCursetTempleResult();
	
	void DropChaosGem(int nIndex);

	int GetRelicsUserIndex()
	{
		return m_nRelicsUserIndex;
	}
	
	int SearchScollOfBloodItem(int iIndex, float ScrollDur);
	int SearchTicketItem(int iIndex);
	
	void MonsterDieItemDrop(LPOBJECTSTRUCT lpObj);
	void SendAllUserAnyMsg(LPBYTE lpMsg, int iSize);
	
	int LevelUp(int iIndex, int iAddExp);
	
	void SendIllusionTempleState(BYTE btTempleState, int nIndex);
	
	void SetAllCheckSync()
	{
		m_bAllCheckSync = TRUE;
	}

	BOOL GetAllChecksync()
	{
		return m_bAllCheckSync;
	}
	
	BYTE GetRealRemainTimeMin();
	
	BOOL CheckSkillProdection(int iIllusionTempleIndex);
	BOOL CheckSkillRestraint(int iIllusionTempleIndex);
	
	void SendIllusionTempleResult();	
};

#endif // !defined(AFX_ILLUSIONTEMPLEPROCESS_H__662704B5_1FB7_4420_AAA3_99B6D68D509D__INCLUDED_)
