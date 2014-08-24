// Crywolf.h: interface for the CCrywolf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLF_H__ADDDADAD_136F_4C82_9920_91B557B8C9B5__INCLUDED_)
#define AFX_CRYWOLF_H__ADDDADAD_136F_4C82_9920_91B557B8C9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapClass.h"
#include "CrywolfObjInfo.h"
#include "CrywolfStateTimeInfo.h"

#define MAX_CRYWOLF_STATE_TIME	20
#define MAX_CRYWOLF_MONSTER_GROUP	20
#define MAX_CRYWOLF_STATE	7

#define CRYWOLF_STATE_NONE		0
#define CRYWOLF_STATE_NOTIFY_1	1
#define CRYWOLF_STATE_NOTIFY_2	2
#define CRYWOLF_STATE_READY		3
#define CRYWOLF_STATE_START		4
#define CRYWOLF_STATE_END		5
#define CRYWOLF_STATE_ENDCYCLE	6

struct PMSG_ANS_CRYWOLF_INFO
{
	PBMSG_HEAD2 h;	// C1:BD:00
	BYTE btOccupationState;	// 4
	BYTE btCrywolfState;	// 5
};


struct PMSG_ANS_CRYWOLF_LEFTTIME
{
	PBMSG_HEAD2 h;	// C1:BD:04
	BYTE btHour;	// 4
	BYTE btMinute;	// 5
};



struct PMSG_ANS_CRYWOLF_STATUE_ALTAR_INFO
{
	PBMSG_HEAD2 h;	// C1:BD:02
	int iCrywolfStatueHP;	// 4
	BYTE btAltarState1;	// 8
	BYTE btAltarState2;	// 9
	BYTE btAltarState3;	// A
	BYTE btAltarState4;	// B
	BYTE btAltarState5;	// C
};



class CCrywolf
{

public:

	CCrywolf();
	virtual ~CCrywolf();

	BOOL LoadData(LPSTR lpszFileName);
	void Init();
	void Run();
	void DelAllData();
	void LoadCrywolfMapAttr(LPSTR lpszFileName, int iOccupationState);
	void SetState(int iCrywolfState);
	void SetNextState(int iCurrentState);
	int CheckStateTimeSync();
	void SetState_NONE();
	void SetState_NOTIFY_1();
	void SetState_NOTIFY_2();
	void SetState_READY();
	void SetState_START();
	void SetState_END();
	void SetState_ENDCYCLE();
	void ProcState_NONE();
	void ProcState_NOTIFY_1();
	void ProcState_NOTIFY_2();
	void ProcState_READY();
	void ProcState_START();
	void ProcState_END();
	void ProcState_ENDCYCLE();
	void CrywolfNpcAct(int iIndex);
	void CrywolfMonsterAct(int iIndex);
	void CrywolfSecondAct();
	int GetCrywolfState(){return this->m_iCrywolfState;}
	int GetOccupationState(){return this->m_iOccupationState;}
	void SetDBDataLoad(BOOL bIsLoaded){this->m_bDBDataLoadOK = bIsLoaded;}
	void SetCrywolfState(int iCrywolfState){this->m_iCrywolfState = iCrywolfState;}
	void SetCrywolfStateAppliedTime(int iCrywolfState){this->m_StateTimeInfo[iCrywolfState].SetAppliedTime();}
	void SetOccupationState(int iOccupationState){this->m_iOccupationState = iOccupationState;}
	void SetCrywolfBossIndex(int iBossIndex){this->m_iBossIndex = iBossIndex;}
	void NotifyCrywolfCurrentState();
	void NotifyCrywolfStatueAndAltarInfo();
	void NotifyCrywolfBossMonsterInfo();
	void NotifyCrywolfStateLeftTime();
	void NotifyCrywolfStageEffectOnOff(BYTE btOnOff);
	void NotifyCrywolfPersonalRank();
	void NotifyCrywolfHeroList();
	void ChangeAI(int iAIOrder);
	void TurnUpBoss();
	void SetCrywolfCommonNPC(int iOccupationState);
	void SetCrywolfMapAttr(int iOccupationState);
	void SetCrywolfAllCommonMonsterState(int iMonsterState, int iMode);
	void RemoveCrywolfCommonMonster();
	void CreateCrywolfCommonMonster();
	void CrywolfMonsterDieProc(int iMonIndex, int iKillerIndex);
	void MakeRewardForAltarElf(int iAltarUserIndex);
	void MakeRewardForHeroListTop5(int iUserIndex);
	void OperateGmCommand(int iUserIndex, int iCommand);
	void CrywolfServerGroupSync();
	void CrywolfInfoDBSave();
	void CrywolfInfoDBLoad();
	void ApplyCrywolfDBInfo(int iCrywolfState, int iOccupationState);
	int CalcGettingScore(int iUserIndex, int iMonIndex, int iScoreType);
	int CalcGettingRank(int iUserIndex);
	int CalcGettingRewardExp(int iUserIndex, int iMVPRank);
	void GiveRewardExp(int iUserIndex, int iRewardExp);
	void ResetAllUserMVPScore();

private:

	BOOL m_bFileDataLoad;	// 4
	BOOL m_bDBDataLoadOK;	// 8
	BOOL m_bDBDataLoading;	// C
	int m_iScheduleMode;	// 10
	MapClass m_CrywolfMapAttr[3];	// 14
	int m_iCrywolfState;	// F314C
	int m_iOccupationState;	// F3150
	
//#if(_GSCS==1)
public:
//#endif
	CCrywolfObjInfo m_ObjCommonNPC;	// F3154
	CCrywolfObjInfo m_ObjSpecialNPC;	// F3608
	CCrywolfObjInfo m_ObjCommonMonster;	// F3ABC
	CCrywolfObjInfo m_ObjSpecialMonster;	// F3F70

//#if(_GSCS==1)
private:
//#endif
	CCrywolfStateTimeInfo m_StartTimeInfo[MAX_CRYWOLF_STATE_TIME];	// F4424
	int m_StartTimeInfoCount;	// F4794
	CCrywolfStateTimeInfo m_StateTimeInfo[MAX_CRYWOLF_STATE];	//F4798
	DWORD m_dwCrywolfNotifyMsgStartTick;	// F48CC
	DWORD m_dwCrywolfStartProcTick;	// F48D0
	BOOL m_bTurnUpBoss;	// F48D4
	BOOL m_bChangeAI;	// F48D8
	int m_iMonsterGroupNumberArray[MAX_CRYWOLF_MONSTER_GROUP];	// F48DC
	int m_iMonsterGroupNumberCount;	// F492C
	int m_iMonsterGroupChangeAITime;	// F4930
	int m_iBossIndex;	// F4934
	int m_iBossGroupNumber;	// F4938
	int m_iBossTurnUpTime;	// F493C
	int m_iMVPScoreTable[7];	// F4940
	int m_iMVPRankScoreTable[5];	// F495C
	int m_iMVPRankExpTable[5];	// F4970
};

extern CCrywolf g_Crywolf;

#endif // !defined(AFX_CRYWOLF_H__ADDDADAD_136F_4C82_9920_91B557B8C9B5__INCLUDED_)
