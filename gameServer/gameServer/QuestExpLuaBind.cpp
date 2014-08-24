#include "stdafx.h"
#include "QuestExpLuaBind.h"
#include "QuestExpUserMng.h"
#include "QuestExpProgMng.h"
#include "QuestExpUserInfo.h"
#include "QuestExpInfo.h"
#include "gObjMonster.h"
#include "logproc.h"
#include "user.h"
#include "NpcTalk.h"
#include "MasterLevelSystem.h"
#include "GameMain.h"
#include "GensSystem.h"

const char QuestExpLuaBind::className[] = "QuestExpLuaBind";
const Luna<QuestExpLuaBind>::RegType QuestExpLuaBind::Register[]=
{
    { "IsFreeServer",               &QuestExpLuaBind::IsFreeServer },
    { "IsGlobalQuest",              &QuestExpLuaBind::IsGlobalQuest },
    { "IsUsingExtendedWarehouse",   &QuestExpLuaBind::IsUsingExtendedWarehouse },
    { "GetNpcIndex",                &QuestExpLuaBind::GetNpcIndex },
    { "GetCharClass",               &QuestExpLuaBind::GetCharClass },
    { "IsMasterLevel",              &QuestExpLuaBind::IsMasterLevel },
    { "GetGensInfluence",           &QuestExpLuaBind::GetGensInfluence },
    { "NpcShadowPhantomBuff",       &QuestExpLuaBind::NpcShadowPhantomBuff },
    { "ChkProgQuest",               &QuestExpLuaBind::ChkProgQuest },
    { "GetUserLv",                  &QuestExpLuaBind::GetUserLv },
    { "GetInvenItemFind",           &QuestExpLuaBind::GetInvenItemFind },
    { "SetQuestSwitch",             &QuestExpLuaBind::SetQuestSwitch },
    { "SetQuestMonsterKill",        &QuestExpLuaBind::SetQuestMonsterKill },
    { "DeleteInvenItem",            &QuestExpLuaBind::DeleteInvenItem },
    { "SetQuestGetItem",            &QuestExpLuaBind::SetQuestGetItem },
    { "SetQuestSkillLearn",         &QuestExpLuaBind::SetQuestSkillLearn },
    { "SetQuestLevelUp",            &QuestExpLuaBind::SetQuestLevelUp },
    { "SetTutorial",                &QuestExpLuaBind::SetTutorial },
    { "SetQuestBuff",               &QuestExpLuaBind::SetQuestBuff },
    { "SetQuestEventMapKillPoint",  &QuestExpLuaBind::SetQuestEventMapKillPoint },
    { "SetQuestEventMapClear",      &QuestExpLuaBind::SetQuestEventMapClear },
    { "SetQuestEventMapDevilPoint", &QuestExpLuaBind::SetQuestEventMapDevilPoint },
    { "SetQuestProgress",           &QuestExpLuaBind::SetQuestProgress },
    { "SetQuestRewardExp",          &QuestExpLuaBind::SetQuestRewardExp },
    { "SetQuestRewardZen",          &QuestExpLuaBind::SetQuestRewardZen },
    { "SetQuestRewardItem",         &QuestExpLuaBind::SetQuestRewardItem },
    { "SetQuestRewardContribute",   &QuestExpLuaBind::SetQuestRewardContribute },
    { "SetQuestRewardRandom",       &QuestExpLuaBind::SetQuestRewardRandom },
    { "Make_QuestIndexIDToEP",      &QuestExpLuaBind::Make_QuestIndexIDToEP },
    { "temp",                       &QuestExpLuaBind::temp },
    { "GetQuestSwitch",             &QuestExpLuaBind::GetQuestSwitch },
    { "SendQuestProgress",          &QuestExpLuaBind::SendQuestProgress },
    { "AddQuestExpNpcTalk",         &QuestExpLuaBind::AddQuestExpNpcTalk },
    { "AddItemAndEtcQuestList",     &QuestExpLuaBind::AddItemAndEtcQuestList },
    { "SendQuestSwitchList",        &QuestExpLuaBind::SendQuestSwitchList },
    { "SendQuestItemAndEventList",  &QuestExpLuaBind::SendQuestItemAndEventList },
    { "IsQuestComplete",            &QuestExpLuaBind::IsQuestComplete },
    { "SendQuestReward",            &QuestExpLuaBind::SendQuestReward },
    { "SendQuestComplete",          &QuestExpLuaBind::SendQuestComplete },
    { "SetQuestProg",               &QuestExpLuaBind::SetQuestProg },
    { "SendQuestItemUseList",       &QuestExpLuaBind::SendQuestItemUseList },
    { "SetQuestDropItem",           &QuestExpLuaBind::SetQuestDropItem },
    { "SetQuestTimeLimit",          &QuestExpLuaBind::SetQuestTimeLimit },
    { "GetQuestTimeLimitState",     &QuestExpLuaBind::GetQuestTimeLimitState },
    { "SetQuestItemInfo",           &QuestExpLuaBind::SetQuestItemInfo },
    { "SetQuestGetZen",             &QuestExpLuaBind::SetQuestGetZen },
    { "SubQuestZen",                &QuestExpLuaBind::SubQuestZen },
    { "SetQuestPVP_Point",          &QuestExpLuaBind::SetQuestPVP_Point },
    { "SetQuestNPCTalk",            &QuestExpLuaBind::SetQuestNPCTalk },
    { "SetQuestNPCTeleportPosInfo", &QuestExpLuaBind::SetQuestNPCTeleportPosInfo },
    { "SendQuestConfirmBtn",        &QuestExpLuaBind::SendQuestConfirmBtn },

    { 0 }
};


QuestExpLuaBind::QuestExpLuaBind(void)
{
}


QuestExpLuaBind::~QuestExpLuaBind(void)
{
}

int gFreeServer = 0;
int gNotGlobalQuest = 0;
int g_QuestExpAddExperience = 1;

int QuestExpLuaBind::IsFreeServer(lua_State *L)
{
    lua_pushnumber(L, gFreeServer);
    return 1;
}

int QuestExpLuaBind::IsGlobalQuest(lua_State *L)
{
    lua_pushnumber(L, gNotGlobalQuest);
    return 1;
}

int QuestExpLuaBind::IsUsingExtendedWarehouse(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);
	int extended_warehouse_count = gObj[obj_index].ExpandedWarehouse;
    lua_settop(L, -2);

    lua_pushnumber(L, extended_warehouse_count > 0);
    return 1;
}

int QuestExpLuaBind::GetNpcIndex(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);
    int npc_index = gObj[obj_index].TargetShopNumber;
    lua_settop(L, -2);

    lua_pushnumber(L, npc_index);
    return 1;
}

int QuestExpLuaBind::GetCharClass(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);
    int char_type = gObj[obj_index].Class;
    lua_settop(L, -2);

    lua_pushnumber(L, char_type);
    return 1;
}

int QuestExpLuaBind::IsMasterLevel(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);
    lua_settop(L, -2);

//    if( g_MasterLevelSystem.IsMasterLevelUser(&gObj[obj_index]) )
	if( g_MasterLevelSystem.IsMasterLevelUser(&gObj[obj_index]) )
    {
        lua_pushnumber(L, 1);
    }
    else
    {
        lua_pushnumber(L, 0);
    }

    return 1;
}

int QuestExpLuaBind::GetGensInfluence(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);
    int gens_influence = gGensSystem.GetGensInfluence(&gObj[obj_index]);
    lua_settop(L, -2);

    lua_pushnumber(L, gens_influence);
    return 1;
}

int QuestExpLuaBind::NpcShadowPhantomBuff(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);
    NpcShadowPhantom(obj_index);
    lua_settop(L, -2);

    return 0;
}

int QuestExpLuaBind::ChkProgQuest(lua_State *L)
{
    int episode_number = luaL_checkinteger(L, 2);
    int obj_index = luaL_checkinteger(L, 3);
    int res = g_QuestExpUserMng.IsProgQuestInfo(episode_number, obj_index);
    lua_settop(L, -3);

    lua_pushnumber(L, res);
    return 1;
}

int QuestExpLuaBind::GetUserLv(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);
    int user_level = gObj[obj_index].Level;
    lua_settop(L, -2);

    lua_pushnumber(L, user_level);
    return 1;
}

// TODO: Replace item number magic values
int QuestExpLuaBind::GetInvenItemFind(lua_State *L)
{
    int item_type = luaL_checkinteger(L, 2);
    int item_index = luaL_checkinteger(L, 3);
    int item_level = luaL_checkinteger(L, 4);
    int item_skill = luaL_checkinteger(L, 5);
    int item_option = luaL_checkinteger(L, 6);
    int item_excellent_option = luaL_checkinteger(L, 7);
    int obj_index = luaL_checkinteger(L, 8);
    int item_count = 0;
    int item_durability = 0;

    for(int x = 0; x < INVENTORY_SIZE; ++x )
    {
        if( !gObj[obj_index].pInventory[x].IsItem() )
        {
            continue;
        }

        if ( gObj[obj_index].pInventory[x].m_Type != ITEMGET(item_type, item_index) )
        {
            continue;
        }

        if( gObj[obj_index].pInventory[x].m_Level != item_level )
        {
            continue;
        }

        if ( (gObj[obj_index].pInventory[x].m_Type < (signed int)0x1C00u
            || gObj[obj_index].pInventory[x].m_Type > (signed int)0x1C08u)
            && (gObj[obj_index].pInventory[x].m_Type < (signed int)0x1C23u
            || gObj[obj_index].pInventory[x].m_Type > (signed int)0x1C28u)
            && g_QuestExpManager.IsQuestItemAtt(gObj[obj_index].pInventory[x].m_Type, 1) != 1 )
        {

            if( gObj[obj_index].pInventory[x].m_Option1 == item_skill 
                && gObj[obj_index].pInventory[x].m_Option3 == item_option
                && gObj[obj_index].pInventory[x].m_NewOption == item_excellent_option)
            {

                if( gObj[obj_index].pInventory[x].m_Option2 == 0
                    && gObj[obj_index].pInventory[x].m_SetOption == 0 )
                {
                    ++item_count;
                }
            }
        }
        else
        {
            item_durability = (item_durability + gObj[obj_index].pInventory[x].m_Durability);
        }
    }

    if( item_durability > 0 )
        item_count = item_durability;

    lua_settop(L, -8);

    lua_pushnumber(L, item_count);

    return 1;
}


int QuestExpLuaBind::SetQuestSwitch(lua_State *L)
{
    int episode_number = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);
    int obj_index = luaL_checkinteger(L, 4);

    g_QuestExpUserMng.SetQuestSwitch(episode_number, quest_switch, obj_index);
    lua_settop(L, -4);

    return 0;
}

int QuestExpLuaBind::SetQuestMonsterKill(lua_State* L)
{
    int n = lua_gettop(L);
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int monster_index = luaL_checkinteger(L, 5);
    int monster_count = luaL_checkinteger(L, 6);
    int party_play = 0;

    QuestMonsterKill quest_monster_kill;

    try
    {
        quest_monster_kill.SetIndexID(ask);
        quest_monster_kill.SetMonsterIndex(monster_index);
        quest_monster_kill.SetMonsterKillCnt(monster_count);
        quest_monster_kill.SetQuestType(1);

        if( n == 7 )
        {
            party_play = luaL_checkinteger(L, 7);
            quest_monster_kill.SetPartyPlay(party_play);
        }

        g_QuestExpManager.SetQuestExpInfo(&quest_monster_kill, episode, quest_switch);

        lua_settop(L, -(n - 1) - 1);
    }
    catch(...)
    {
    }

    return 0;
}


int QuestExpLuaBind::DeleteInvenItem(lua_State *L)
{
    int item_type = luaL_checkinteger(L, 2);
    int item_index = luaL_checkinteger(L, 3);
    int item_level = luaL_checkinteger(L, 4);
    int item_skill = luaL_checkinteger(L, 5);
    int item_count = luaL_checkinteger(L, 6);
    int item_option = luaL_checkinteger(L, 7);
    int item_excellent_option = luaL_checkinteger(L, 8);
    int obj_index = luaL_checkinteger(L, 9);

    QuestGetItem quest_get_item;

    try
    {
        quest_get_item.SetIndexID(0);
        quest_get_item.SetQuestItemNum(item_type, item_index);
        quest_get_item.SetQuestItemLevel(item_level);
        quest_get_item.SetQuestItemCnt(item_count);
        quest_get_item.SetQuestItemSkill(item_skill);
        quest_get_item.SetQuestItemOpt(item_option);
        quest_get_item.SetQuestItemExOpt(item_excellent_option);
        quest_get_item.SetQuestType(0);

        g_QuestExpProgMng.DeleteInventoryItem(&quest_get_item, obj_index);
        lua_settop(L, -9);
    }
    catch(...)
    {
    }

    return 1;
}

int QuestExpLuaBind::SetQuestGetItem(lua_State *L)
{
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int item_type = luaL_checkinteger(L, 5);
    int item_index = luaL_checkinteger(L, 6);
    int item_level = luaL_checkinteger(L, 7);
    int item_skill = luaL_checkinteger(L, 8);
    int quest_item_count = luaL_checkinteger(L, 9);
    int item_option = luaL_checkinteger(L, 10);
    int item_excellent_option = luaL_checkinteger(L, 11);

    QuestGetItem quest_get_item;

    try
    {
        quest_get_item.SetIndexID(ask);
        quest_get_item.SetQuestItemNum(item_type, item_index);
        quest_get_item.SetQuestItemLevel(item_level);
        quest_get_item.SetQuestItemCnt(quest_item_count);
        quest_get_item.SetQuestItemSkill(item_skill);
        quest_get_item.SetQuestItemOpt(item_option);
        quest_get_item.SetQuestItemExOpt(item_excellent_option);
        quest_get_item.SetQuestType(4);

        g_QuestExpManager.SetQuestExpInfo(&quest_get_item, episode, quest_switch);

        lua_settop(L, -11);
    }
    catch(...){}

    return 0;
}


int QuestExpLuaBind::SetQuestSkillLearn(lua_State *L)
{
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int skill_index = luaL_checkinteger(L, 5);
    QuestSkillLearn quest_skill_learn;

    try
    {
        quest_skill_learn.SetIndexID(ask);
        quest_skill_learn.SetQuestSkillIndex(skill_index);
        quest_skill_learn.SetQuestType(2);
        g_QuestExpManager.SetQuestExpInfo(&quest_skill_learn, episode, quest_switch);

        lua_settop(L, -5);
    }
    catch(...){}

    return 0;
}


int QuestExpLuaBind::SetQuestLevelUp(lua_State *L)
{
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int leve_value = luaL_checkinteger(L, 5);

    QuestLevelUp quest_level_up;

    try
    {
        quest_level_up.SetIndexID(ask);
        quest_level_up.SetQuestLevelVal(leve_value);
        quest_level_up.SetQuestType(8);

        g_QuestExpManager.SetQuestExpInfo(&quest_level_up, episode, quest_switch);

        lua_settop(L, -5);
    }
    catch(...){}

    return 0;
}

int QuestExpLuaBind::SetTutorial(lua_State *L)
{
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    QuestTutorial quest_tutorial;

    try
    {

        quest_tutorial.SetIndexID(ask);
        quest_tutorial.SetQuestType(16);

        g_QuestExpManager.SetQuestExpInfo(&quest_tutorial, episode, quest_switch);

        lua_settop(L, -4);

    }
    catch(...){}

    return 0;
}

int QuestExpLuaBind::SetQuestBuff(lua_State* L)
{
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int buff_index = luaL_checkinteger(L, 5);

    QuestBuff quest_buff;

    try
    {
        quest_buff.SetIndexID(ask);
        quest_buff.SetQuestBuffIndex(buff_index);
        quest_buff.SetQuestType(32);

        g_QuestExpManager.SetQuestExpInfo(&quest_buff, episode, quest_switch);
        lua_settop(L, -5);
    }
    catch(...){}

    return 0;
}

int QuestExpLuaBind::SetQuestEventMapKillPoint(lua_State *L)
{
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int quest_type = luaL_checkinteger(L, 5);
    int event_map_level = luaL_checkinteger(L, 6);
    int kill_count = luaL_checkinteger(L, 7);

    QuestEventMapKillPoint quest_event_map_kill_point;

    try
    {

        quest_event_map_kill_point.SetIndexID(ask);
        quest_event_map_kill_point.SetQuestEventMapKillCnt(kill_count);
        quest_event_map_kill_point.SetQuestEventMapLevel(event_map_level);
        quest_event_map_kill_point.SetQuestType(quest_type);

        g_QuestExpManager.SetQuestExpInfo(&quest_event_map_kill_point,
            episode, quest_switch);
        lua_settop(L, -7);
    }
    catch(...){}

    return 0;
}

int QuestExpLuaBind::SetQuestEventMapClear(lua_State *L)
{
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int quest_type = luaL_checkinteger(L, 5);
    int event_map_level = luaL_checkinteger(L, 6);

    QuestEventMapClear quest_event_map_clear;

    try
    {
        quest_event_map_clear.SetIndexID(ask);
        quest_event_map_clear.SetQuestEventMapLevel(event_map_level);
        quest_event_map_clear.SetQuestType(quest_type);
        g_QuestExpManager.SetQuestExpInfo(&quest_event_map_clear, episode, quest_switch);

        lua_settop(L, -6);
    }
    catch(...){}

    return 0;
}


int QuestExpLuaBind::SetQuestEventMapDevilPoint(lua_State *L)
{
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int event_map_level = luaL_checkinteger(L, 5);
    int event_map_devil_point = luaL_checkinteger(L, 6);

    QuestEventMapDevilPoint quest_event_map_devil_point;

    try
    {
        quest_event_map_devil_point.SetIndexID(ask);
        quest_event_map_devil_point.SetQuestEventMapLevel(event_map_level);
        quest_event_map_devil_point.SetQuestEventMapDevilPoint(event_map_devil_point);
        quest_event_map_devil_point.SetQuestType(260);

        g_QuestExpManager.SetQuestExpInfo(&quest_event_map_devil_point,
            episode, quest_switch);

        lua_settop(L, -6);
    }
    catch(...){}

    return 0;
}

int QuestExpLuaBind::SetQuestProgress(lua_State *L)
{
    int ask = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);

    QuestProgress quest_progress;

    try
    {
        quest_progress.SetIndexID(ask);
        quest_progress.SetQuestType(0);
        g_QuestExpManager.SetQuestExpInfo(&quest_progress, episode, quest_switch);

        lua_settop(L, -4);
    }
    catch(...){}
    
    return 0;
}

int QuestExpLuaBind::SetQuestRewardExp(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);
    int exp = g_QuestExpAddExperience * luaL_checkinteger(L, 4);
    RewardExp reward_exp;

    try
    {
        reward_exp.SetRewardExp(exp);
        reward_exp.SetRewardType(1);
        g_QuestExpManager.SetQuestExpInfo(&reward_exp, episode, quest_switch);
        lua_settop(L, -4);
    }
    catch(...){}
    
    return 0;
}

int QuestExpLuaBind::SetQuestRewardZen(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);
    int zen_amount = luaL_checkinteger(L, 4);

    RewardZen reward_zen;

    try
    {
        reward_zen.SetRewardZen(zen_amount);
        reward_zen.SetRewardType(2);
        g_QuestExpManager.SetQuestExpInfo(&reward_zen, episode, quest_switch);
        lua_settop(L, -4);
    }
    catch(...){}
    
    return 0;
}

int QuestExpLuaBind::SetQuestRewardItem(lua_State *L)
{
    int n = lua_gettop(L);
    int episode = 0;
    int quest_switch = 0;
    int item_type = 0;
    int item_index = 0;
    int item_level = 0;
    int item_durability = 0;
    int item_skill = 0;
    int item_count = 0;
    int item_overlap = 0;
    int item_option = 0;
    int item_excellent_option = 0;

    if( n == 9 )
    {
        episode = luaL_checkinteger(L, 2);
        quest_switch = luaL_checkinteger(L, 3);
        item_type = luaL_checkinteger(L, 4);
        item_index = luaL_checkinteger(L, 5);
        item_level = luaL_checkinteger(L, 6);
        item_durability = luaL_checkinteger(L, 7);
        item_skill = luaL_checkinteger(L, 8);
        item_count = luaL_checkinteger(L, 9);
        lua_settop(L, -9);
    }

    if( n == 10 )
    {
        episode = luaL_checkinteger(L, 2);
        quest_switch = luaL_checkinteger(L, 3);
        item_type = luaL_checkinteger(L, 4);
        item_index = luaL_checkinteger(L, 5);
        item_level = luaL_checkinteger(L, 6);
        item_durability = luaL_checkinteger(L, 7);
        item_skill = luaL_checkinteger(L, 8);
        item_count = luaL_checkinteger(L, 9);
        item_overlap = luaL_checkinteger(L, 10);
        lua_settop(L, -10);
    }

    if( n == 11 )
    {
        episode = luaL_checkinteger(L, 2);
        quest_switch = luaL_checkinteger(L, 3);
        item_type = luaL_checkinteger(L, 4);
        item_index = luaL_checkinteger(L, 5);
        item_level = luaL_checkinteger(L, 6);
        item_durability = luaL_checkinteger(L, 7);
        item_skill = luaL_checkinteger(L, 8);
        item_count = luaL_checkinteger(L, 9);
        item_overlap = luaL_checkinteger(L, 10);
        item_option = luaL_checkinteger(L, 11);
        lua_settop(L, -11);
    }

    if( n == 12 )
    {
        episode = luaL_checkinteger(L, 2);
        quest_switch = luaL_checkinteger(L, 3);
        item_type = luaL_checkinteger(L, 4);
        item_index = luaL_checkinteger(L, 5);
        item_level = luaL_checkinteger(L, 6);
        item_durability = luaL_checkinteger(L, 7);
        item_skill = luaL_checkinteger(L, 8);
        item_count = luaL_checkinteger(L, 9);
        item_overlap = luaL_checkinteger(L, 10);
        item_option = luaL_checkinteger(L, 11);
        item_excellent_option = luaL_checkinteger(L, 12);
        lua_settop(L, -12);
    }

    RewardItem reward_item;

    try
    {
        reward_item.SetRewardItemNum(item_type, item_index);
        reward_item.SetRewardItemLevel(item_level);
        reward_item.SetRewardItemDur(item_durability);
        reward_item.SetRewardItemCnt(item_count);
        reward_item.SetRewardItemSkill(item_skill);
        reward_item.SetRewardItemOverlap(item_overlap);
        reward_item.SetRewardItemOpt(item_option);
        reward_item.SetRewardItemExOpt(item_excellent_option);
        reward_item.SetRewardType(4);
        g_QuestExpManager.SetQuestExpInfo(&reward_item, episode, quest_switch);
    }
    catch(...){}
    
    return 0;
}

int QuestExpLuaBind::SetQuestRewardContribute(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);
    int contribute_point = luaL_checkinteger(L, 4);
    RewardContribute reward_contribute;

    try
    {
        reward_contribute.SetRewardContributePoint(contribute_point);
        reward_contribute.SetRewardType(16);

        g_QuestExpManager.SetQuestExpInfo(&reward_contribute, episode, quest_switch);
        lua_settop(L, -4);
    }
    catch(...){}
    
    return 0;
}

int QuestExpLuaBind::SetQuestRewardRandom(lua_State *L)
{
    QuestExpRewardKindRandom reward_kind_random;

    try
    {
        int reward_indexes[5] = {0};
        int reward_rates[5] = {0};

        int item_count = 0;
        for(int i = 0; i < 5; ++i )
        {
            reward_indexes[i] = -1;
            reward_rates[i] = -1;
        }

        int episode = luaL_checkinteger(L, 2);
        int quest_switch = luaL_checkinteger(L, 3);
        int reward_count = luaL_checkinteger(L, 4);

        reward_kind_random.SetQuestEpisode(episode);
        reward_kind_random.SetQuestSwitch(quest_switch);
        reward_kind_random.SetRandRewardCnt(reward_count);

        int n = lua_gettop(L);
        for(int j = 5; j <= n; j += 2 )
        {
            int reward_index = luaL_checkinteger(L, j);
            int reward_rate = luaL_checkinteger(L, j + 1);

            reward_indexes[item_count] = reward_index - 1;
            reward_rates[item_count] = reward_rate;

            ++item_count;
        }

        reward_kind_random.SetRewardItemTotCnt(item_count);
        lua_settop(L, -(n - 1) - 1);

        reward_kind_random.SetRewardRate(reward_rates);
        reward_kind_random.SetRewardIndex(reward_indexes);
        reward_kind_random.SetRewardKind(1);

        g_QuestExpManager.AddQuestExpRewardKind(&reward_kind_random);

    }
    catch(...){}
    

    return 0;
}


int QuestExpLuaBind::Make_QuestIndexIDToEP(lua_State *L)
{
    DWORD quest_info_index_id = luaL_checknumber(L, 2);
    int episode = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id);

    if( mu::quest::IsEpisode(episode) )
    {
        lua_settop(L, -2);
        lua_pushnumber(L, episode);

        return 1;
    }
    else
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);

        // probably it's a bug, since nothing is returned to lua
        return 1;
    }

}

int QuestExpLuaBind::temp(lua_State *L)
{
    return 0;
}


int QuestExpLuaBind::GetQuestSwitch(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int obj_index = luaL_checkinteger(L, 3);
    int quest_switch = g_QuestExpUserMng.GetQuestSwitch(episode, obj_index);

    if( quest_switch == -1 )
    {
        LogAddTD("[QuestExp] - Error - LuaGetQuestSwitch [%s][%s] Episode[%d]",
            gObj[obj_index].AccountID, gObj[obj_index].Name, episode);

        // bug?
        return 1;
    }
    else
    {
        lua_settop(L, -3);
        lua_pushnumber(L, quest_switch);
        
        return 1;
    }
}

int QuestExpLuaBind::SendQuestProgress(lua_State *L)
{

    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);
    int obj_index = luaL_checkinteger(L, 4);

    g_QuestExpUserMng.SetQuestSwitch(episode, quest_switch, obj_index);
    g_QuestExpProgMng.SendQuestProgress(episode, quest_switch, obj_index);

    lua_settop(L, -4);

    return 0;
}

int QuestExpLuaBind::AddQuestExpNpcTalk(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);

    g_QuestExpProgMng.AddQuestSwitchList(episode, quest_switch);

    lua_settop(L, -3);

    return 0;
}

int QuestExpLuaBind::AddItemAndEtcQuestList(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);

    g_QuestExpProgMng.AddQuestSwitchList(episode, quest_switch);

    lua_settop(L, -3);

    return 0;
}

int QuestExpLuaBind::SendQuestSwitchList(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);

    g_QuestExpProgMng.SendQuestSwitchList(obj_index, 0);

    lua_settop(L, -2);

    return 0;
}

int QuestExpLuaBind::SendQuestItemAndEventList(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);

    g_QuestExpProgMng.SendQuestSwitchList(obj_index, 1);

    lua_settop(L, -2);

    return 0;
}

int QuestExpLuaBind::IsQuestComplete(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int obj_index = luaL_checkinteger(L, 3);
    bool is_complete = g_QuestExpUserMng.IsQuestComplete(episode, obj_index);

    lua_settop(L, -3);
    lua_pushboolean(L, is_complete);

    return 1;
}

int QuestExpLuaBind::SendQuestReward(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int obj_index = luaL_checkinteger(L, 3);
    int res = g_QuestExpUserMng.SendQuestReward(episode, obj_index);

    if( res == 1 )
        g_QuestExpProgMng.SetQuestProg(episode, obj_index, 0);

    lua_settop(L, -3);
    lua_pushnumber(L, res);

    return 1;
}

int QuestExpLuaBind::SendQuestComplete(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);
    int obj_index = luaL_checkinteger(L, 4);

    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode, quest_switch);

    g_QuestExpProgMng.GCANSQuestCompleteBtnClick(obj_index, quest_info_index_id, 1);
    g_QuestExpUserMng.InitUserQuestAskInfo(episode, obj_index);
    g_QuestExpProgMng.SetQuestProg(episode, obj_index, 0);

    lua_settop(L, -4);

    return 0;
}

int QuestExpLuaBind::SetQuestProg(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int obj_index = luaL_checkinteger(L, 3);
    int prog_state = luaL_checkinteger(L, 4);

    g_QuestExpProgMng.SetQuestProg(episode, obj_index, prog_state);

    lua_settop(L, -4);

    return 0;
}

int QuestExpLuaBind::SendQuestItemUseList(lua_State *L)
{
    int obj_index = luaL_checkinteger(L, 2);

    g_QuestExpProgMng.SendQuestSwitchList(obj_index, 2);

    lua_settop(L, -2);

    return 0;
}

int QuestExpLuaBind::SetQuestDropItem(lua_State *L)
{
    int n = lua_gettop(L);
    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);
    int ask_index = luaL_checkinteger(L, 4);
    int monster_index = luaL_checkinteger(L, 5);
    int reward_rate = luaL_checkinteger(L, 6);
    int item_type = luaL_checkinteger(L, 7);
    int item_index = luaL_checkinteger(L, 8);
    int item_level = luaL_checkinteger(L, 9);
    int item_count = luaL_checkinteger(L, 10);

    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode, quest_switch);

    QuestDropItemInfo quest_drop_item_info;

    try
    {
        quest_drop_item_info.SetQuestEpisode(episode);
        quest_drop_item_info.SetQuestSwitch(quest_switch);
        quest_drop_item_info.SetAskIndex(ask_index);
        quest_drop_item_info.SetMonsterIndex(monster_index);
        quest_drop_item_info.SetRewardRate(reward_rate);
        quest_drop_item_info.SetItemNum(item_type, item_index);
        quest_drop_item_info.SetItemLevel(item_level);
        quest_drop_item_info.SetItemCnt(item_count);

        g_QuestExpManager.AddQuestDropItemInfo(&quest_drop_item_info, quest_info_index_id);

        lua_settop(L, -10);
    }
    catch(...){}
    
    return 0;
}

int QuestExpLuaBind::SetQuestTimeLimit(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);
    int obj_index = luaL_checkinteger(L, 4);
    int time_limit = luaL_checkinteger(L, 5);

    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode, quest_switch);

    g_QuestExpProgMng.SetQuestTimeLimit(obj_index, quest_info_index_id, time_limit);

    lua_settop(L, -5);

    return 0;
}

int QuestExpLuaBind::GetQuestTimeLimitState(lua_State *L)
{
    int state = 0;
    int episode = luaL_checkinteger(L, 2);
    int obj_index = luaL_checkinteger(L, 3);

    if ( g_QuestExpUserMng.GetQuestProgState(episode, obj_index) == 2 )
        state = 1;

    lua_settop(L, -3);
    lua_pushnumber(L, state);

    return 1;
}

int QuestExpLuaBind::SetQuestItemInfo(lua_State *L)
{
    int n = lua_gettop(L);
    int episode_number = 0;

    int item_type = luaL_checkinteger(L, 2);
    int item_index = luaL_checkinteger(L, 3);
    int quest_item_attr = luaL_checkinteger(L, 4);

    QUEST_ITEM_INFO quest_item_info;
    quest_item_info.item_number = ITEMGET(item_type, item_index);
    quest_item_info.item_attribute = quest_item_attr;

    if( n == 5 )
    {
        episode_number = luaL_checkinteger(L, 5);
        quest_item_info.episode_number = episode_number;
    }

    g_QuestExpManager.AddQuestItemInfo(quest_item_info);

    lua_settop(L, -(n - 1) - 1);

    return 0;
}

int QuestExpLuaBind::SetQuestGetZen(lua_State *L)
{
    int ask_index = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int need_zen = luaL_checkinteger(L, 5);

    QuestNeedZen quest_need_zen;

    try
    {
        quest_need_zen.SetIndexID(ask_index);
        quest_need_zen.SetQuestType(261);
        quest_need_zen.SetQuestNeedZen(need_zen);

        g_QuestExpManager.SetQuestExpInfo(&quest_need_zen, episode, quest_switch);

        lua_settop(L, -5);
    }
    catch(...){}
   
    return 0;
}

int QuestExpLuaBind::SubQuestZen(lua_State *L)
{
    int sub_money = luaL_checkinteger(L, 2);
    int obj_index = luaL_checkinteger(L, 3);

    if( gObj[obj_index].Money >= sub_money )
        gObj[obj_index].Money -= sub_money;

    GCMoneySend(obj_index, gObj[obj_index].Money);
    lua_settop(L, -3);

    return 0;
}

int QuestExpLuaBind::SetQuestPVP_Point(lua_State *L)
{
    int n = lua_gettop(L);
    int ask_index = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int map_number = luaL_checkinteger(L, 5);
    int pvp_point = luaL_checkinteger(L, 6);

    QuestPVP_Point quest_pvp_point;

    try
    {
        quest_pvp_point.SetIndexID(ask_index);
        quest_pvp_point.SetQuestType(262);
        quest_pvp_point.SetQuestMapNum(map_number);
        quest_pvp_point.SetQuestPVP_Point(pvp_point);

        g_QuestExpManager.SetQuestExpInfo(&quest_pvp_point, episode, quest_switch);

        lua_settop(L, -6);
    }
    catch(...){}
   
    return 0;
}

int QuestExpLuaBind::SetQuestNPCTalk(lua_State *L)
{
    int n = lua_gettop(L);
    int ask_index = luaL_checkinteger(L, 2);
    int episode = luaL_checkinteger(L, 3);
    int quest_switch = luaL_checkinteger(L, 4);
    int npc_index = luaL_checkinteger(L, 5);

    QuestNpcTalk quest_npc_talk;

    quest_npc_talk.SetIndexID(ask_index);
    quest_npc_talk.SetQuestType(263);
    quest_npc_talk.SetQuestNPCTalk(npc_index);

    g_QuestExpManager.SetQuestExpInfo(&quest_npc_talk, episode, quest_switch);

    lua_settop(L, -5);

    return 0;
}

int QuestExpLuaBind::SetQuestNPCTeleportPosInfo(lua_State *L)
{
    int n = lua_gettop(L);
    int index = luaL_checkinteger(L, 2);
    int map_number = luaL_checkinteger(L, 3);
    int pos_x = luaL_checkinteger(L, 4);
    int pos_y = luaL_checkinteger(L, 5);
    int direction = luaL_checkinteger(L, 6);

    gQuestExpNpcTeleport.m_QuestNPCTeleportPos[index].mapnum = map_number;
    gQuestExpNpcTeleport.m_QuestNPCTeleportPos[index].x = pos_x;
    gQuestExpNpcTeleport.m_QuestNPCTeleportPos[index].y = pos_y;
    gQuestExpNpcTeleport.m_QuestNPCTeleportPos[index].dir = direction;

    lua_settop(L, -(n - 1) - 1);

    return 0;
}

int QuestExpLuaBind::SendQuestConfirmBtn(lua_State *L)
{
    int episode = luaL_checkinteger(L, 2);
    int quest_switch = luaL_checkinteger(L, 3);
    int obj_index = luaL_checkinteger(L, 4);

    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode, quest_switch);

    g_QuestExpProgMng.GCANSQuestCompleteBtnClick(obj_index, quest_info_index_id, 1);

    if( gObj[obj_index].m_pUserQuestInfo[episode].GetAskCnt() > 0 )
    {
        gObj[obj_index].m_pUserQuestInfo[episode].SetAskCnt(0);

        for(int i = 0; i < 5; ++i )
            gObj[obj_index].m_pUserQuestInfo[episode].user_quest_ask_info_[i].Clear();
    }

    lua_settop(L, -4);

    return 0;
}