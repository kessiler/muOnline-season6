// IllusionTempleEvent.h: interface for the CIllusionTempleEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ILLUSIONTEMPLEEVENT_H__5AFAC037_1E27_42E9_A450_79C351208CFB__INCLUDED_)
#define AFX_ILLUSIONTEMPLEEVENT_H__5AFAC037_1E27_42E9_A450_79C351208CFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CHECK_ILLUSIONTEMPLE(map) ((map<MAP_INDEX_ILLUSION_TEMPLE1)?FALSE:(map>MAP_INDEX_ILLUSION_TEMPLE6)?FALSE:TRUE)
#define IT_MAP_RANGE(map) ((map<MAP_INDEX_ILLUSION_TEMPLE1)?FALSE:(map>MAP_INDEX_ILLUSION_TEMPLE6)?FALSE:TRUE)

#pragma warning ( disable : 4786 )

#include <list>
using namespace std;

#include "IllusionTempleProcess.h"

class OBJECTSTRUCT;
typedef OBJECTSTRUCT* LPOBJECTSTRUCT;

extern int g_bIllusionTempleEvent;
extern int g_iOldScrollDropRate;
extern int g_iCovenantOfIllusionDropRate;
extern int g_bUseMaxLevelIllusionTemple;
extern int g_bSaveIllusionTempleRankingPoint;
extern int g_iIllusionTempleRewardPercent;
extern int g_btIllusionTempleMinUseSkillCnt;
extern int g_iIllusionTempleRewardPercentMaster;

#ifdef __CUSTOMS__
struct ILLUSIONTEMPL_ITEMREWARD
{
	BYTE	CastleLevel;
	BYTE	ItemCount;
	WORD	ItemID;
	BYTE	ItemLevel;
};
#endif

struct __ILLUSION_TEMPLE_STARTTIME
{
	int nHour;
	int nMin;
};

typedef struct 
{
	PBMSG_HEAD h;
	char AccountID[MAX_IDSTRING];
	char GameID[MAX_IDSTRING];
	int ServerCode;
	int iObjIndex;
	int bEnterCheck;
	int iItemPos;
	int iItemLevel;
	int iTempleNumber;
} PMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK, *LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK;

typedef struct 
{
	PBMSG_HEAD h;
	char AccountID[MAX_IDSTRING];
	char GameID[MAX_IDSTRING];
	int ServerCode;
	int iObjIndex;
	int iLeftCount;
} PMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT, *LPPMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT;

typedef struct 
{
	PBMSG_HEAD2 h;
	BYTE Result;
} PMSG_RESULT_ENTER_ILLUSION_TEMPLE, *LPPMSG_RESULT_ENTER_ILLUSION_TEMPLE;

typedef struct 
{
	PBMSG_HEAD h;
	char AccountID[MAX_IDSTRING];
	char GameID[MAX_IDSTRING];
	int ServerCode;
	int iObjIndex;
	int iItemPos;
	int iItemLevel;
	int iTempleNumber;
} PMSG_REQ_ILLUSION_TMPLE_ENTERCOUNTCHECK, *LPPMSG_REQ_ILLUSION_TMPLE_ENTERCOUNTCHECK;

typedef struct 
{
	PBMSG_HEAD h;
	char AccountID[MAX_IDSTRING];
	char GameID[MAX_IDSTRING];
	int ServerCode;
} PMSG_REQ_ILLUSION_TEMPLE_INC_COUNT, *LPPMSG_REQ_ILLUSION_TEMPLE_INC_COUNT;

class CIllusionTempleEvent  
{
	CIllusionTempleProcess m_cIllusionTempleProc[MAX_ILLUSIONTEMPLE_TEMPLE_COUNT];
	int m_nMirageIndex;

public:
	CIllusionTempleEvent();
	virtual ~CIllusionTempleEvent();

	BOOL IllusionTempleEventInit();
	BOOL LoadIllusionTempleEventInfo();
	BOOL LoadIllusionTempleScript(char* filename);
	void IllusionTempleRun();
	void SetInventoryPos(BYTE btMapNumber, int aIndex, BYTE btPos);
	void SendRelicsUser(LPOBJECTSTRUCT lpObj);
	void SeStatusInfo(BYTE btTempleIndex, int nIndex, short wPosNum);
	BOOL EnterIllusionTemple(int aIndex, BYTE btTempleIndex, BYTE btItemPos);
	BOOL GEReqIllusionTempleEnterCountCheck(int aIndex, BYTE btTempleIndex, BYTE btItemPos, int nItemLevel);
	void EGAnsIllusionTempleEnterCountCheck(LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK aRecv);
	void GEReqIllusionTempleIncEnterCount(LPOBJECTSTRUCT lpObj);
	BOOL LeaveIllusionTemple(int aIndex, BYTE btMapNumber);
	BOOL CheckEnterLevel(int nIndex, int nItemLevel);
	void IllusionHolyRelicsStatusAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
	void HolyRelicsBoxAlliedAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
	void HolyRelicsBoxIllusionAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
	void PlatformMirageAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj);
	void DeleteRelicsItem(BYTE btMapNumber, LPOBJECTSTRUCT lpObj);
	void DropRelicsItem(BYTE btMapNumber, int nIndex);
	void SetIllusionTempleState(BYTE btState, BYTE btMapNumber);
	BYTE GetIllusionTempleState(BYTE btMapNumber);
	void AddNpcPosNum(BYTE btMapNumber, int nClass, int nPosNum);
	void AddMonsterPosNum(BYTE btMapNumber, int nClass, int nPosNum);
	void ResetPosNum();
	BYTE GetUserTeam(BYTE btMapNumber, int nIndex);
	void SetStatusRegenTime(BYTE btMapNumber);
	void IllusionTempleChaosMixItem(LPOBJECTSTRUCT lpObj);
	BYTE IncSkillPoint(int nIndex, BYTE btMapNumber, BYTE btObjectType);
	BYTE DecSkillPoint(int nIndex, BYTE btMapNumber, BYTE btDecPoint);
	void IllusionTempleUseSkill(int iIndex, WORD MagicNumber, int wTargetObjIndex, BYTE btDis);
	void EventSkillProc(LPOBJECTSTRUCT lpObj);
	void IllusionTempleUserDie(LPOBJECTSTRUCT lpObj);
	void IllusionTempleUserDieRegen(LPOBJECTSTRUCT lpObj);
	void IncUseSkillCount(int nIndex, BYTE btMapNumber);
	void SetMirageIndex(int nIndex);
	void SendIllusionTempleUserCount();
	int CheckWearingMOPH(int nUserIndex);
	int CheckChangeRing(int nType);
	int CheckRelics(int nIndex);
	void MonsterDieItemDrop(LPOBJECTSTRUCT lpObj);
	void ReqEventReward(int nIndex);
	BYTE GetRemainTime();
	int CheckSkillProdection(int iIllusionTempleIndex, BYTE btMapNumber);
	int CheckSkillRestraint(int iIllusionTempleIndex, BYTE btMapNumber);
	int CheckCanEnterLevel(int aIndex);
	int CheckCharmCount(int aIndex);
	void EGReqIllusionTempleEnterCount(int iIndex);
	void EGAnsIllusionTempleEnterCount(LPPMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT lpMsg);

private:
	BYTE IllusionTempleChaosMix(int nIndex, int nLEVEL, int iCharmOfLuckCount);
	int CheckChaosMixItem(int nIndex);

public:
	static int m_nIllusionTempleOpenTime;
	static int m_nIllusionTempleReadyTime;
	static int m_nIllusionTemplePlayTime;
	static int m_nIllusionTempleEndTime;
	int m_MinPlayerCount;	//-> Custom
	static std::list<__ILLUSION_TEMPLE_STARTTIME> m_lstIllusionTempleStartTime;

#ifdef __CUSTOMS__
	ILLUSIONTEMPL_ITEMREWARD m_ItemReward[MAX_ILLUSIONTEMPLE_TEMPLE_COUNT];
#endif

};

extern CIllusionTempleEvent	g_IllusionTempleEvent;

#endif // !defined(AFX_ILLUSIONTEMPLEEVENT_H__5AFAC037_1E27_42E9_A450_79C351208CFB__INCLUDED_)
