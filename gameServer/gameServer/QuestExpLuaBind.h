#pragma once


#include "..\Lua\include\lua.hpp"
#include "Luna.h"

class QuestExpLuaBind
{
public:
    QuestExpLuaBind(void);
	QuestExpLuaBind(lua_State* L){};

    virtual ~QuestExpLuaBind(void);


    int IsFreeServer(lua_State *L);
    int IsGlobalQuest(lua_State *L);
    int IsUsingExtendedWarehouse(lua_State *L);
    int GetNpcIndex(lua_State *L);
    int GetCharClass(lua_State *L);
    int IsMasterLevel(lua_State *L);
    int GetGensInfluence(lua_State *L);
    int NpcShadowPhantomBuff(lua_State *L);
    int ChkProgQuest(lua_State *L);
    int GetUserLv(lua_State *L);
    int GetInvenItemFind(lua_State *L);
    int SetQuestSwitch(lua_State *L);
    int SetQuestMonsterKill(lua_State* L);
    int DeleteInvenItem(lua_State *L);
    int SetQuestGetItem(lua_State *L);
    int SetQuestSkillLearn(lua_State *L);
    int SetQuestLevelUp(lua_State *L);
    int SetTutorial(lua_State *L);
    int SetQuestBuff(lua_State* L);
    int SetQuestEventMapKillPoint(lua_State *L);
    int SetQuestEventMapClear(lua_State *L);
    int SetQuestEventMapDevilPoint(lua_State *L);
    int SetQuestProgress(lua_State *L);
    int SetQuestRewardExp(lua_State *L);
    int SetQuestRewardZen(lua_State *L);
    int SetQuestRewardItem(lua_State *L);
    int SetQuestRewardContribute(lua_State *L);
    int SetQuestRewardRandom(lua_State *L);
    int Make_QuestIndexIDToEP(lua_State *L);
    int temp(lua_State *L);
    int GetQuestSwitch(lua_State *L);
    int SendQuestProgress(lua_State *L);
    int AddQuestExpNpcTalk(lua_State *L);
    int AddItemAndEtcQuestList(lua_State *L);
    int SendQuestSwitchList(lua_State *L);
    int SendQuestItemAndEventList(lua_State *L);
    int IsQuestComplete(lua_State *L);
    int SendQuestReward(lua_State *L);
    int SendQuestComplete(lua_State *L);
    int SetQuestProg(lua_State *L);
    int SendQuestItemUseList(lua_State *L);
    int SetQuestDropItem(lua_State *L);
    int SetQuestTimeLimit(lua_State *L);
    int GetQuestTimeLimitState(lua_State *L);
    int SetQuestItemInfo(lua_State *L);
    int SetQuestGetZen(lua_State *L);
    int SubQuestZen(lua_State *L);
    int SetQuestPVP_Point(lua_State *L);
    int SetQuestNPCTalk(lua_State *L);
    int SetQuestNPCTeleportPosInfo(lua_State *L);
    int SendQuestConfirmBtn(lua_State *L);


    static const char className[];
    static const Luna<QuestExpLuaBind>::RegType Register[];
};

