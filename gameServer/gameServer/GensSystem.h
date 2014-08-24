#ifndef GENSSYSTEM_H
#define GENSSYSTEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GensSystemProtocol.h"

#define MAX_BATTLE_ZONE		11
#define MAX_REWARD_TABLE	8

struct RewardTable
{
	BYTE	Rank;
	int		ItemID;
	int		ItemIndex;
	BYTE	ItemLevel;
	BYTE	ItemCount;
};

enum eGensClass
{
	Private = 0,
};

const int MAX_VIEWPORT_INFO = 0x7FC + 1;



extern BOOL g_GensSystemIsEnabled;
extern int g_GensAbusingResetTime;
extern int g_GensInitialContributePoint;
extern int g_BattleZoneAddExp;
extern int g_GensMasterLevelBattleZoneAddExp;
extern int g_PkPenaltyAddNeedZenMapMove;



//--------------------------------------------------

void CGReqRegGensMember(PMSG_REQ_REG_GENS_MEMBER *lpMsg, int iIndex);
void CGReqSecedeGensMember(PMSG_REQ_SEGEDE_GENS_MEMBER *lpMsg, int iIndex);
void CGReqGensReward(PMSG_GENS_REWARD_CODE *lpMsg, int iIndex);
void CGReqGensMemberInfo(PMSG_REQ_GENS_INFO *lpMsg, int iIndex);

//--------------------------------------------------



//--------------------------------------------------

void DGAnsGensInfo(PMSG_ANS_GENS_INFO_EXDB *lpMsg);
void DGAnsRegGensMember(PMSG_ANS_REG_GENS_MEMBER_EXDB *lpMsg);
void DGAnsSecedeGensMember(PMSG_ANS_SECEDE_GENS_MEMBER_EXDB *lpMsg);
void DGAnsAbusingInfo(PMSG_ANS_ABUSING_INFO *lpMsg);
void DGAnsGensReward(PMSG_ANS_GENS_REWARD_EXDB *lpMsg);
void DGAnsGensMemberCount(PMSG_ANS_GENS_MEMBER_COUNT_EXDB *lpMsg);
void DGAnsGensRewardDayCheck(PMSG_ANS_GENS_REWARD_DAY_CHECK_EXDB *lpMsg);
//--------------------------------------------------



//--------------------------------------------------

class GensSystem
{
public:
	GensSystem(void);
	virtual ~GensSystem(void);

	int GDReqAbusingInfo(LPOBJ lpObj); 
	int ReqExDBGensInfo(LPOBJ lpObj);
	void ReqRegGensMember(LPOBJ lpObj, unsigned char btInfluence);
	int AnsRegGensMember(int iObjIndex, unsigned char btResult);
	bool IsInfluenceNPC(LPOBJ lpObj);
	int ReqSecedeGensMember(LPOBJ lpObj);
	int SendGensInfo(LPOBJ lpObj);
	int AnsSecedeGensMember(int iObjIndex, BYTE btResult);
	int SendPkPenaltyMapMove(LPOBJ lpObj);
	int GDReqSaveContributePoint(LPOBJ lpObj);
	int DBSaveAbusingKillUserName(LPOBJ lpObj);
	void BattleZoneChatMsgSend(LPOBJ obj, LPBYTE Msg, int size);
	signed int IsMoveMapBattleZone(int iMapIndex);
	void GensViewportListProtocol(LPOBJ lpObj);
	signed int IsMapBattleZone(int iMapIndex);
	void SetGensInfluence(LPOBJ lpObj, int iInfluence);
	signed int IsPkEnable(LPOBJ lpObj, LPOBJ lpTargetObj);
	signed int GetGensInfluence(LPOBJ lpObj);
	char *GetGensInfluenceName(LPOBJ lpObj);
	bool IsRegGensInfluence(LPOBJ lpObj);
	int SetUserBattleZoneEnable(LPOBJ lpObj, int bBattleZoneEnable);
	int IsUserBattleZoneEnable(LPOBJ lpObj);
	int SetContributePoint(LPOBJ lpObj, int iContributePoint);
	int AddContributePoint(LPOBJ lpObj, int iContributePoint);
	int SubContributePoint(LPOBJ lpObj, int iContributePoint);
	int GetContributePoint(LPOBJ lpObj);
	signed int PkPenaltyAddNeedZenMapMove(LPOBJ lpObj);
	void PkPenaltyDropInvenItem(LPOBJ lpObj);
	void PkPenaltyDropZen(LPOBJ lpObj);
	int SendPKPenaltyDebuff(LPOBJ lpObj);
	void CalcContributePoint(LPOBJ lpObj, LPOBJ lpTargetObj);
	signed int ChkKillUserName(LPOBJ lpObj, char *szCharName);
	signed int AbusingPenalty(LPOBJ lpObj, int iKillUserIndex);
	int InsertKillUserName(LPOBJ lpObj, char *szCharName);
	void MoveInBattleZonePartySplit(LPOBJ lpObj);
	void AbusingInfoReset(LPOBJ lpObj);
	void CalGensClass(LPOBJ lpObj);
	int SetGensRanking(LPOBJ lpObj, int iGensRanking);
	int SetGensClass(LPOBJ lpObj, int iGensClass);
	int GetGensClass(LPOBJ lpObj);
	int GetNextContributePoint(LPOBJ lpObj);
	int SetGensMemberCount(int iInfluence, int iMemberCount);
	signed int GetGensMemberCount(int iInfluence);
	int ReqExDBGensRewardCheck(LPOBJ lpObj, int iInfluence);
	int ReqExDBGensRewardComplete(LPOBJ lpObj);
	int SendGensReward(LPOBJ lpObj, unsigned char btResult);
	signed int SendGensRewardItem(LPOBJ lpObj, int iGensClass);
	bool GensRewardInventoryCheck(LPOBJ lpObj, int iItemCount);
	int ReqExDBGensMemberCount(LPOBJ lpObj);
	int ReqExDBSetGensRewardDay(void);
	int ReqExDBSetGensRanking(void);
	int LoadData(char *lpszFileName);
	int ReqGensRewardDay(LPOBJ lpObj);
	// ----
	void GDRewardReq(LPOBJ lpObj);
	void DGRewardAns(PMSG_GENS_REWARD_ANS * aRecv);
	void GDRankingUpdate();
	// ----
	BYTE		BattleZoneList[MAX_BATTLE_ZONE];
	BYTE		BattleMapList[MAX_BATTLE_ZONE];
	RewardTable RewardTableList[MAX_REWARD_TABLE+1];
	// ----
	bool	GuildCreateReqGens;
	bool	CheckGuildEnter;
	bool	CheckPartyEnter;
	bool	CheckAllianceEnter;
	bool	AllowPK;
	// ----
	int		ChatPrice;
	// ----
	BOOL b_PkPenaltyDropZen;
	BOOL b_PkPenaltyDropInvenItem;

	int iVanertMemberCount;
	int iDuprianMemberCount;

	int iIndex;

protected:
};
extern GensSystem gGensSystem;

//--------------------------------------------------

#endif //GENSSYSTEM_H