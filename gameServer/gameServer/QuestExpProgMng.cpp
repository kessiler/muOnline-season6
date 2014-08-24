#include "stdafx.h"
#include "QuestExpProgMng.h"
#include "QuestExpInfo.h"
#include "QuestExpUserInfo.h"
#include "QuestExpUserMng.h"

#include "user.h"
#include "logproc.h"
#include "GameMain.h"

#include "..\Lua\include\lua.hpp"

#include "../common/winutil.h"
#include "LuaFun.h"
#include "gObjMonster.h"
#include "BuffEffectSlot.h"
#include "DSProtocol.h"
#include "MuLua.h"
#include "GensSystem.h"

QuestExpProgMng g_QuestExpProgMng;

QuestExpProgMng::QuestExpProgMng(void)
{
}


QuestExpProgMng::~QuestExpProgMng(void)
{
}


void QuestExpProgMng::ReqQuestAskComplete(DWORD quest_info_index_id, int obj_index)
{
    int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id);

    if( gObj[obj_index].m_pUserQuestInfo[episode_number].user_quest_ask_info_[0].GetQuestType() == 16 )
    {
        gObj[obj_index].m_pUserQuestInfo[episode_number].user_quest_ask_info_[0].SetComplete(true);
        gObj[obj_index].m_pUserQuestInfo[episode_number].user_quest_ask_info_[0].SetValue(1);
    }
}


struct PMSG_QUEST_GIVEUP_ANS
{
    PBMSG_HEAD2 h;
    int quest_info_index_id;
};

void QuestExpProgMng::QuestExpGiveUpBtnClick(DWORD quest_info_index_id, int obj_index)
{
    int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id);
    int quest_switch = mu::quest::GetQuestSwitchFromInfoIndexId(quest_info_index_id);

    LogAddTD(
        "[QuestExp] Give Up [%s][%s] Ep[%d] QS[%d]",
        gObj[obj_index].AccountID,
        gObj[obj_index].Name,
        episode_number,
        quest_switch);

    int lua_push_quest_switch = gObj[obj_index].m_pUserQuestInfo[episode_number].GetQuestSwitch();
    lua_State* L = g_MuLuaQuestExp.GetLua();

    g_Generic_Call(L, "QuestGiveUp", "iii>", obj_index, episode_number, lua_push_quest_switch);

    gObj[obj_index].m_pUserQuestInfo[episode_number].QuestAskInfoClear();

    int send_quest_switch = gObj[obj_index].m_pUserQuestInfo[episode_number].GetQuestSwitch();

    LogAddTD(
        "[QuestExp] Give Up - SetQuestSwitch [%s][%s] Ep[%d] QS[%d]",
        gObj[obj_index].AccountID,
        gObj[obj_index].Name,
        episode_number,
        send_quest_switch);

    PMSG_QUEST_GIVEUP_ANS pMsg;
    pMsg.quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode_number, send_quest_switch);

    PHeadSubSetB((LPBYTE)&pMsg, 0xF6u, 0xF, sizeof(pMsg));
    DataSend(obj_index, (LPBYTE)&pMsg, pMsg.h.size);
}

#pragma pack(push, 1)
struct PMSG_NPC_QUESTEXP_INFO
{
    PBMSG_HEAD2 h;
    int quest_index_id;
    char ask_size;
    char reward_size;
    char reward_count;

    PMSG_NPC_QUESTEXP_INFO()
    {
        this->quest_index_id = 0;
        this->ask_size = 0;
        this->reward_size = 0;
        this->reward_count = 0;
    }
};
#pragma pack(pop)

void QuestExpProgMng::SendQuestProgress(DWORD quest_index_id, int obj_index)
{
    PMSG_NPC_QUESTEXP_INFO pMsg;
    pMsg.quest_index_id = quest_index_id;
    pMsg.reward_size = 0;
    pMsg.ask_size = 0;
    PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0xB, sizeof(pMsg));
    DataSend(obj_index, (LPBYTE)&pMsg, pMsg.h.size);

    LogAddTD(
        "[QuestExp] - Ans Send User : Selection Statements Choose One [%s][%s] QuestIndexID[0x%x]",
        gObj[obj_index].AccountID,
        gObj[obj_index].Name,
        quest_index_id);
}

#pragma pack(push, 1)
struct NPC_QUESTEXP_ASK_INFO
{
    int quest_type;
    short ukn4;
    int ukn6;
    int value;
    BYTE item_info[12];

    NPC_QUESTEXP_ASK_INFO()
    {
        this->quest_type = 0;
        this->ukn4 = 0;
        this->ukn6 = 0;
        this->value = 0;
        memset(this->item_info, 0, sizeof(this->item_info));
    }
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NPC_QUESTEXP_REWARD_INFO
{
    int quest_type;
    short ukn4;
    int value;
    BYTE item_info[12];

    NPC_QUESTEXP_REWARD_INFO()
    {
        this->quest_type = 0;
        this->ukn4 = 0;
        this->value = 0;
        memset(this->item_info, 0, sizeof(this->item_info));
    }
};
#pragma pack(pop)

struct NPC_QUESTEXP_INFO
{
    NPC_QUESTEXP_ASK_INFO ask_info[5];
    NPC_QUESTEXP_REWARD_INFO reward_info[5];
};


void QuestExpProgMng::SendQuestProgress(QuestExpInfo* quest_exp_info, 
                                            DWORD quest_index_id, 
                                            int obj_index)
{
    BYTE send_buf[2048] = {0};

    PMSG_NPC_QUESTEXP_INFO pMsg;
    int lOfs = sizeof(pMsg);

    NPC_QUESTEXP_INFO npc_quest_exp_info;

    bool bGetSendQuestInfo = GetSendQuestInfo(quest_exp_info, quest_index_id, obj_index, &npc_quest_exp_info);

    if( !bGetSendQuestInfo )
    {
        LogAddTD("[QuestExp] - Error - SendQuestProgress() [%s] [%d]",
            __FILE__, __LINE__);

        return;
    }

    memcpy(&send_buf[lOfs], &npc_quest_exp_info, sizeof(npc_quest_exp_info));
    lOfs += sizeof(npc_quest_exp_info);

    auto map_quest_ask = quest_exp_info->GetQuestExpAsk();
    auto map_quest_reward = quest_exp_info->GetQuestReward();

    int ask_size = map_quest_ask.size();

    QuestExpRewardKind* reward_kind = g_QuestExpManager.GetQuestExpRewardKind(quest_index_id);
    if( reward_kind )
    {
        if( reward_kind->GetRewardKind() == 1 )
        {
            QuestExpRewardKindRandom* reward_kind_random = 
                static_cast<QuestExpRewardKindRandom*>(reward_kind);

            pMsg.reward_count = reward_kind_random->GetRandRewardCnt();
        }
    }

    pMsg.quest_index_id = quest_index_id;
    pMsg.ask_size = ask_size;
    pMsg.reward_size = map_quest_reward.size();

    PHeadSubSetB((BYTE*)&pMsg, 0xF6, 0x0C, lOfs);
    memcpy(send_buf, &pMsg, sizeof(pMsg));
    DataSend(obj_index, send_buf, lOfs);

    LogAddTD(
        "[QuestExp] Send QuestInfo From NPC To User : [%s][%s] Ep[%d] QS[%d]",
        gObj[obj_index].AccountID,
        gObj[obj_index].Name,
        mu::quest::GetQuestEpisodeFromInfoIndexId(quest_index_id),
        mu::quest::GetQuestSwitchFromInfoIndexId(quest_index_id));
}

void QuestExpProgMng::SendQuestProgress(int episode_number, int quest_switch, int obj_index)
{
    DWORD quest_index_id = mu::quest::GetQuestInfoIndexId(episode_number, quest_switch);

    if( ChkQuestIndexIDToEpAndQSLimit(quest_index_id, obj_index) )
    {
        QuestExpInfo* quest_exp_info = g_QuestExpManager.GetQuestExpInfo(quest_index_id);
        if( quest_exp_info )
            SendQuestProgress(quest_exp_info, quest_index_id, obj_index);
        else
            SendQuestProgress(quest_index_id, obj_index);
    }
    else
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
    }
}

void QuestExpProgMng::SendQuestProgressInfo(DWORD quest_index_id, int obj_index)
{
    QuestExpInfo* quest_exp_info = g_QuestExpManager.GetQuestExpInfo(quest_index_id);

    if( quest_exp_info )
        SendQuestProgressInfo(quest_exp_info, quest_index_id, obj_index);
}


void QuestExpProgMng::SendQuestProgressInfo(QuestExpInfo* quest_exp_info, 
                                                DWORD quest_index_id, 
                                                int obj_index)
{
    BYTE send_buf[2048] = {0};

    PMSG_NPC_QUESTEXP_INFO pMsg;
    int lOfs = sizeof(pMsg);

    NPC_QUESTEXP_INFO npc_quest_exp_info;

    bool bGetSendQuestInfo = GetSendQuestInfo(quest_exp_info, quest_index_id,
                                        obj_index, &npc_quest_exp_info);
    if( !bGetSendQuestInfo )
    {
        LogAddTD("[QuestExp] - Error - SendQuestProgressInfo() [%s] [%d]",
                __FILE__, __LINE__);
    }

    memcpy(&send_buf[lOfs], &npc_quest_exp_info, sizeof(npc_quest_exp_info));
    lOfs += sizeof(npc_quest_exp_info);

    auto map_quest_ask = quest_exp_info->GetQuestExpAsk();
    auto map_quest_reward = quest_exp_info->GetQuestReward();
    int ask_size = map_quest_ask.size();


    QuestExpRewardKind* reward_kind = g_QuestExpManager.GetQuestExpRewardKind(quest_index_id);
    if( reward_kind )
    {
        if( reward_kind->GetRewardKind() == 1 )
        {
            QuestExpRewardKindRandom* reward_kind_random = 
                static_cast<QuestExpRewardKindRandom*>(reward_kind);

            pMsg.reward_count = reward_kind_random->GetRandRewardCnt();
        }
    }

    pMsg.quest_index_id = quest_index_id;
    pMsg.ask_size = ask_size;
    pMsg.reward_size = map_quest_reward.size();

    PHeadSubSetB((BYTE*)&pMsg, 0xF6u, 0x1Bu, lOfs);

    memcpy(send_buf, &pMsg, sizeof(pMsg));
    DataSend(obj_index, send_buf, lOfs);

    LogAddTD(
        "[QuestExp] Send Quest Info To QuestList : [%s][%s] Ep[%d] QS[%d]",
        gObj[obj_index].AccountID,
        gObj[obj_index].Name,
        mu::quest::GetQuestEpisodeFromInfoIndexId(quest_index_id),
        mu::quest::GetQuestSwitchFromInfoIndexId(quest_index_id));
}


bool QuestExpProgMng::ChkQuestAsk(int episode_number, int obj_index)
{
    int ask_count = 0;
    UserQuestInfo* user_quest_info = g_QuestExpUserMng.GetUserQuestInfo(episode_number, obj_index);

    if( !user_quest_info )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);

        return false;
    }

    int quest_switch = user_quest_info->GetQuestSwitch();
    DWORD quest_index_id = mu::quest::GetQuestInfoIndexId(episode_number, quest_switch);
    QuestExpInfo* quest_exp_info = g_QuestExpManager.GetQuestExpInfo(quest_index_id);

    if( !quest_exp_info )
    {
        LogAddTD(
            "[QuestExp] - Error - ChkQuestAsk : [%s][%s] QuestIndexID[0x%x]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            quest_index_id);

        return false;
    }

    auto map_quest_ask = quest_exp_info->GetQuestExpAsk();

    auto map_it = map_quest_ask.begin();
    auto map_end = map_quest_ask.end();

    while( 1 )
    {
        if( map_it == map_end)
            break;

        QuestExpAsk* quest_exp_ask = map_it->second;

        if( !quest_exp_ask )
        {
            LogAddTD(
                "[QuestExp] - Error - ChkQuestAsk *** : [%s][%s] QuestIndexID[0x%x]",
                gObj[obj_index].AccountID,
                gObj[obj_index].Name,
                quest_index_id);

            return false;
        }

        if( !(ask_count >= 0 && ask_count <= 4) )
        {
            LogAdd("[QuestExp] - Error - iAskCnt [%d] [%s] [%d]",
                    ask_count, __FILE__, __LINE__);

            return false;
        }


        int quest_type = quest_exp_ask->GetQuestType();

        switch ( quest_type )
        {
        case 0:
        case 1:
        case 16:
            break;

        case 4:
            {
                QuestGetItem* quest_get_item = 
                    static_cast<QuestGetItem*>(quest_exp_ask);

                ChkUserQuestTypeItem(quest_get_item, 
                    &user_quest_info->user_quest_ask_info_[ask_count], 
                    obj_index);
            }
            break;

        case 2:
            {
                QuestSkillLearn* quest_skill_learn = 
                    static_cast<QuestSkillLearn*>(quest_exp_ask);

                ChkUserQuestTypeSkillLearn(quest_skill_learn,
                    &user_quest_info->user_quest_ask_info_[ask_count],
                    obj_index);
            }
            break;

        case 8:
            {
                QuestLevelUp* quest_level_up = 
                        static_cast<QuestLevelUp*>(quest_exp_ask);

                ChkUserQuestTypeLevelUp(quest_level_up,
                    &user_quest_info->user_quest_ask_info_[ask_count],
                    obj_index);
            }
            break;

        case 32:
            {

            QuestBuff* quest_buff = static_cast<QuestBuff*>(quest_exp_ask);

            ChkUserQuestTypeBuff(quest_buff, 
                &user_quest_info->user_quest_ask_info_[ask_count], 
                obj_index);
            }
            break;

        case 64:
        case 65:
        case 66:
        case 256:
        case 257:
        case 258:
        case 259:
        case 260:
        case 262:
        case 263:
            break;

        case 261:
            {
                QuestNeedZen* quest_need_zen = 
                    static_cast<QuestNeedZen*>(quest_exp_ask);

                ChkUserQuestTypeNeedZen(quest_need_zen,
                    &user_quest_info->user_quest_ask_info_[ask_count],
                    obj_index);
            }

            break;

        default:
            LogAdd("[QuestExp] - Error - Unknown Ask Type [%d] [%s] [%d]",
                quest_type, __FILE__, __LINE__);

            return false;
        }

        ++ask_count;
        ++map_it;
    }

    return true;
}

bool QuestExpProgMng::GetSendQuestInfo(QuestExpInfo* quest_exp_info, 
                                        DWORD quest_index_id, 
                                        int obj_index, 
                                        NPC_QUESTEXP_INFO* npc_quest_exp_info)
{

    int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_index_id);
    UserQuestInfo* user_quest_info = g_QuestExpUserMng.GetUserQuestInfo(
                                                episode_number,
                                                obj_index);

    if( !user_quest_info )
    {
        LogAddTD(
            "[QuestExp] - Error - GetSendQuestInfo()  pUserQuestInfo == NULL : [%s][%s]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name);

        return false;
    }

    auto map_quest_ask = quest_exp_info->GetQuestExpAsk();
    int ask_count = 0;

    auto map_it = map_quest_ask.begin();
    auto map_end = map_quest_ask.end();

    while( 1 )
    {
        if( map_it == map_end )
            break;

        QuestExpAsk* quest_exp_ask = map_it->second;

        if( !quest_exp_ask )
        {
            LogAddTD("[QuestExp] - Error - GetSendQuestInfo *** : [%s][%s] QuestIndexID[0x%x]", 
                gObj[obj_index].AccountID, gObj[obj_index].Name, 
                quest_index_id);

            return false;
        }

        if( !(ask_count >= 0 && ask_count <= 4) )
        {
            LogAdd("[QuestExp] - Error - iAskCnt [%d] [%s] [%d]",
                    ask_count, __FILE__, __LINE__);

            return false;
        }

        int quest_type = quest_exp_ask->GetQuestType();

        switch ( quest_type )
        {
        case 0:
            npc_quest_exp_info->ask_info[ask_count].quest_type = 0;
            break;

        case 16:
            npc_quest_exp_info->ask_info[ask_count].quest_type = 16;

            npc_quest_exp_info->ask_info[ask_count].value = 
                user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            break;

        case 1:
            {
                QuestMonsterKill* quest_monster_kill = 
                        static_cast<QuestMonsterKill*>(quest_exp_ask);

                npc_quest_exp_info->ask_info[ask_count].quest_type = 1;

                npc_quest_exp_info->ask_info[ask_count].ukn4 = 
                    quest_monster_kill->GetMonsterIndex();
                npc_quest_exp_info->ask_info[ask_count].ukn6 = 
                    quest_monster_kill->GetMonsterKillCnt();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }
            break;

        case 4:
            {

            QuestGetItem* quest_get_item = 
                    static_cast<QuestGetItem*>(quest_exp_ask);

            ChkUserQuestTypeItem(quest_get_item, 
                &user_quest_info->user_quest_ask_info_[ask_count], 
                obj_index);

            npc_quest_exp_info->ask_info[ask_count].quest_type = 4;

            npc_quest_exp_info->ask_info[ask_count].ukn4 = 
                quest_get_item->GetQuestItemNum();

            npc_quest_exp_info->ask_info[ask_count].ukn6 = 
                quest_get_item->GetQuestItemCnt();

            npc_quest_exp_info->ask_info[ask_count].value = 
                user_quest_info->user_quest_ask_info_[ask_count].GetValue();

            CItem item;
            item.m_Type = quest_get_item->GetQuestItemNum();
            item.m_Level = quest_get_item->GetQuestItemLevel();
            item.m_Option1 = quest_get_item->GetQuestItemSkill();
            item.m_Option3 = quest_get_item->GetQuestItemOpt();
            item.m_Durability = 0;
            item.m_NewOption = quest_get_item->GetQuestItemExOpt();


            ItemByteConvert(
                npc_quest_exp_info->ask_info[ask_count].item_info,
                item);
            }
            break;

        case 2:
            {
                QuestSkillLearn* quest_skill_learn = 
                    static_cast<QuestSkillLearn*>(quest_exp_ask);

                ChkUserQuestTypeSkillLearn(quest_skill_learn, 
                    &user_quest_info->user_quest_ask_info_[ask_count], 
                    obj_index);

                npc_quest_exp_info->ask_info[ask_count].quest_type = 2;
                npc_quest_exp_info->ask_info[ask_count].ukn4 = 
                    quest_skill_learn->GetQuestSkillIndex();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }
            break;

        case 8:
            {
                QuestLevelUp* quest_level_up = 
                        static_cast<QuestLevelUp*>(quest_exp_ask);

                ChkUserQuestTypeLevelUp(quest_level_up, 
                    &user_quest_info->user_quest_ask_info_[ask_count], 
                    obj_index);

                npc_quest_exp_info->ask_info[ask_count].quest_type = 8;

                npc_quest_exp_info->ask_info[ask_count].ukn6 = 
                    quest_level_up->GetQuestLevelVal();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }
            break;

        case 32:
            {
                QuestBuff* quest_buff = static_cast<QuestBuff*>(quest_exp_ask);

                ChkUserQuestTypeBuff(quest_buff, 
                    &user_quest_info->user_quest_ask_info_[ask_count], 
                    obj_index);

                npc_quest_exp_info->ask_info[ask_count].quest_type = 32;
                npc_quest_exp_info->ask_info[ask_count].ukn4 = quest_buff->GetQuestBuffIndex();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }

            break;

        case 64:
        case 65:
        case 66:
            {
                QuestEventMapKillPoint* quest_event_map_kill_point = 
                    static_cast<QuestEventMapKillPoint*>(quest_exp_ask);

                npc_quest_exp_info->ask_info[ask_count].quest_type = quest_type;

                npc_quest_exp_info->ask_info[ask_count].ukn4 = 
                    quest_event_map_kill_point->GetQuestEventMapLevel();

                npc_quest_exp_info->ask_info[ask_count].ukn6 = 
                    quest_event_map_kill_point->GetQuestEventMapKillCnt();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }
            break;

        case 256:
        case 257:
        case 258:
        case 259:
            {
                QuestEventMapClear* quest_event_map_clear = 
                            static_cast<QuestEventMapClear*>(quest_exp_ask);

                npc_quest_exp_info->ask_info[ask_count].quest_type = quest_type;

                npc_quest_exp_info->ask_info[ask_count].ukn4 = 
                    quest_event_map_clear->GetQuestEventMapLevel();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }
            break;

        case 260:
            {
                QuestEventMapDevilPoint* quest_event_map_devil_point = 
                    static_cast<QuestEventMapDevilPoint*>(quest_exp_ask);

                npc_quest_exp_info->ask_info[ask_count].quest_type = quest_type;

                npc_quest_exp_info->ask_info[ask_count].ukn4 = 
                    quest_event_map_devil_point->GetQuestEventMapLevel();

                npc_quest_exp_info->ask_info[ask_count].ukn6 = 
                    quest_event_map_devil_point->GetQuestEventMapDevilPoint();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }
            break;

        case 261:
            {
                QuestNeedZen* quest_need_zen = 
                        static_cast<QuestNeedZen*>(quest_exp_ask);

                ChkUserQuestTypeNeedZen(quest_need_zen, 
                    &user_quest_info->user_quest_ask_info_[ask_count], 
                    obj_index);

                npc_quest_exp_info->ask_info[ask_count].quest_type = quest_type;
                npc_quest_exp_info->ask_info[ask_count].ukn6 = quest_need_zen->GetQuestNeedZen();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }
            break;

        case 262:
            {
                QuestPVP_Point* questpvp_point =
                    static_cast<QuestPVP_Point*>(quest_exp_ask);

                npc_quest_exp_info->ask_info[ask_count].quest_type = quest_type;

                npc_quest_exp_info->ask_info[ask_count].ukn6 = 
                    questpvp_point->GetQuestPVP_Point();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }

            break;

        case 263:
            {
                QuestNpcTalk* quest_npc_talk = 
                    static_cast<QuestNpcTalk*>(quest_exp_ask);

                npc_quest_exp_info->ask_info[ask_count].quest_type = quest_type;
                npc_quest_exp_info->ask_info[ask_count].ukn4 = 
                    quest_npc_talk->GetQuestNPCTalk();

                npc_quest_exp_info->ask_info[ask_count].value = 
                    user_quest_info->user_quest_ask_info_[ask_count].GetValue();
            }
            break;

        default:

            LogAdd(
                "[QuestExp] - Error - Unknown Ask Type [%d] [%s] [%d]",
                quest_type, __FILE__, __LINE__);

            return false;
        }

        ++map_it;
        ++ask_count;
    }


    QuestExpRewardKindRandom* reward_kind_random = nullptr;
    int array_reward_index[5] = {0};

    QuestExpRewardKind* reward_kind = g_QuestExpManager.GetQuestExpRewardKind(quest_index_id);
    if( reward_kind )
    {
        if( reward_kind->GetRewardKind() == 1 )
        {
            QuestExpRewardKindRandom* reward_kind_random = 
                static_cast<QuestExpRewardKindRandom*>(reward_kind);

            memcpy(array_reward_index, 
                reward_kind_random->GetRewardIndex(), 
                sizeof(array_reward_index));
        }
    }

    auto map_quest_reward = quest_exp_info->GetQuestReward();

    auto quest_reward_it = map_quest_reward.begin();
    auto quest_reward_end = map_quest_reward.end();

    int reward_count = 0;
    while( 1 )
    {
        if( quest_reward_it == quest_reward_end )
        {
            break;
        }

        QuestExpReward* quest_exp_reward = quest_reward_it->second;

        if( !quest_exp_reward )
        {
            LogAddTD(
                "[QuestExp] - Error - GetSendQuestInfo *** : [%s][%s] QuestIndexID[0x%x]", 
                gObj[obj_index].AccountID, gObj[obj_index].Name, quest_index_id);

            return false;

        }

        if( !(reward_count >= 0 && reward_count <= 4) )
        {
            LogAdd("[QuestExp] - Error - iReward [%d] [%s] [%d]",
                reward_count, __FILE__, __LINE__);

            return false;
        }

        int quest_type_for_reward = quest_exp_reward->GetRewardType();

        if( reward_kind )
        {
            for(int i = 0; i < 5; ++i )
            {
                if( array_reward_index[i] == quest_exp_reward->GetIndexID() )
                {
                    quest_type_for_reward |= 0x20;
                }
            }
        }

        //var290 = (quest_type_for_reward & 0xFFFFFFDF) - 1;
        switch ( quest_type_for_reward & 0xFFFFFFDF )
        {
        case 1:
            {
                RewardExp* reward_exp = static_cast<RewardExp*>(quest_exp_reward);

                npc_quest_exp_info->reward_info[reward_count].quest_type = quest_type_for_reward;
                npc_quest_exp_info->reward_info[reward_count].ukn4 = 0;
                npc_quest_exp_info->reward_info[reward_count].value = reward_exp->GetRewardExp();
            }
            break;

        case 2:
            {
                RewardZen* reward_zen = static_cast<RewardZen*>(quest_exp_reward);

                npc_quest_exp_info->reward_info[reward_count].quest_type = quest_type_for_reward;
                npc_quest_exp_info->reward_info[reward_count].ukn4 = 0;
                npc_quest_exp_info->reward_info[reward_count].value = reward_zen->GetRewardZen();
            }
            break;

        case 4:
            {
                RewardItem* reward_item = static_cast<RewardItem*>(quest_exp_reward);

                npc_quest_exp_info->reward_info[reward_count].quest_type = quest_type_for_reward;
                npc_quest_exp_info->reward_info[reward_count].ukn4 = reward_item->GetRewardItemNum();
                npc_quest_exp_info->reward_info[reward_count].value = reward_item->GetRewardItemCnt();
                CItem item;
                item.m_Type = reward_item->GetRewardItemNum();
                item.m_Level = reward_item->GetRewardItemLevel();
                item.m_Option1 = reward_item->GetRewardItemSkill();
                item.m_Durability = reward_item->GetRewardItemDur();
                item.m_NewOption = reward_item->GetRewardItemExOpt();
                item.m_Option3 = reward_item->GetRewardItemOpt();

                ItemByteConvert(
                    npc_quest_exp_info->reward_info[reward_count].item_info,
                    item);

                if( reward_item->GetRewardItemOverlap() == 1 )
                {
                    int item_count = reward_item->GetRewardItemCnt();
                    int item_durability = reward_item->GetRewardItemDur();
                    npc_quest_exp_info->reward_info[reward_count].value = 
                        item_durability * item_count;
                }

            }
            break;

        case 16:
            {
                RewardContribute* reward_contribute = 
                    static_cast<RewardContribute*>(quest_exp_reward);

                npc_quest_exp_info->reward_info[reward_count].quest_type = quest_type_for_reward;
                npc_quest_exp_info->reward_info[reward_count].ukn4 = 0;

                npc_quest_exp_info->reward_info[reward_count].value = 
                    reward_contribute->GetRewardContributePoint();
            }
            break;

        default:
            LogAdd("[QuestExp] - Error - Unknown Reward Type %d %s %d",
                    quest_type_for_reward, __FILE__, __LINE__);

            return false;
        }

        ++quest_reward_it;
        ++reward_count;
    }

    return true;
}

void QuestExpProgMng::ChkUserQuestTypeBuff(QuestBuff* quest_buff, 
                                    UserQuestAskInfo* user_quest_ask_info, 
                                    int obj_index)
{
    int buff_index = quest_buff->GetQuestBuffIndex();

//    if( gObjCheckUsedBuffEffect(&gObj[obj_index], buff_index) )
	if( ::gObjCheckUsedBuffEffect(&gObj[obj_index], buff_index) )
    {
        user_quest_ask_info->SetValue(1);
        user_quest_ask_info->SetComplete(true);
    }
    else
    {
        user_quest_ask_info->SetValue(0);
        user_quest_ask_info->SetComplete(false);
    }
}

void QuestExpProgMng::ChkUserQuestTypeNeedZen(QuestNeedZen* quest_need_zen, 
                                        UserQuestAskInfo* user_quest_ask_info, 
                                        int obj_index)
{
    int user_zen = gObj[obj_index].Money;

    if( user_zen < quest_need_zen->GetQuestNeedZen() )
    {
        user_quest_ask_info->SetValue(user_zen);
        user_quest_ask_info->SetComplete(false);
    }
    else
    {
        user_quest_ask_info->SetValue(user_zen);
        user_quest_ask_info->SetComplete(true);
    }
}


void QuestExpProgMng::ChkUserQuestTypeSkillLearn(QuestSkillLearn* quest_skill_learn, 
                                        UserQuestAskInfo* user_quest_ask_info,
                                        int obj_index)
{
    int skill_number = quest_skill_learn->GetQuestSkillIndex();

    for(int n = 0; n < MAX_MAGIC; ++n )
    {
        if( gObj[obj_index].Magic[n].IsMagic() )
        {
            if( gObj[obj_index].Magic[n].m_Skill == skill_number )
            {
                if ( !user_quest_ask_info->IsComplete() )
                {
                    user_quest_ask_info->SetComplete(true);
                    user_quest_ask_info->SetValue(1);
                }
            }
        }
    }
}

void QuestExpProgMng::ChkUserQuestTypeLevelUp(QuestLevelUp* quest_level_up, 
                                    UserQuestAskInfo* user_quest_ask_info, 
                                    int obj_index)
{
    int user_level = gObj[obj_index].Level;

    if( quest_level_up->GetQuestLevelVal() > user_level )
    {
        user_quest_ask_info->SetComplete(false);
        user_quest_ask_info->SetValue(user_level);
    }
    else
    {
        user_quest_ask_info->SetComplete(true);
        user_quest_ask_info->SetValue(user_level);
    }
}


void QuestExpProgMng::DeleteInventoryItem(QuestGetItem* quest_get_item, 
                                            int obj_index)
{
    if( !OBJMAX_RANGE(obj_index) || !gObjIsConnected(obj_index) )
    {
        return;
    }

    int item_count = quest_get_item->GetQuestItemCnt();
    int item_number = quest_get_item->GetQuestItemNum();
    int item_level = quest_get_item->GetQuestItemLevel();
    int item_option = quest_get_item->GetQuestItemOpt();
    int item_excellent_option = quest_get_item->GetQuestItemExOpt();
    int item_skill = quest_get_item->GetQuestItemSkill();

    for(int  x = 0; x < INVENTORY_SIZE; ++x )
    {
        if( !gObj[obj_index].pInventory[x].IsItem() )
        {
            continue;
        }

        if ( item_count <= 0 )
            return;

        if ( gObj[obj_index].pInventory[x].m_Type != item_number )
        {
            continue;
        }

        if ( gObj[obj_index].pInventory[x].m_Level != item_level )
        {
            continue;
        }

        if ( (gObj[obj_index].pInventory[x].m_Type < 0x1C00
            || gObj[obj_index].pInventory[x].m_Type > 0x1C08)
            && (gObj[obj_index].pInventory[x].m_Type < 0x1C23
            || gObj[obj_index].pInventory[x].m_Type > 0x1C28)
            && g_QuestExpManager.IsQuestItemAtt(gObj[obj_index].pInventory[x].m_Type, 1) != 1 )
        {
            if( gObj[obj_index].pInventory[x].m_Option1 != item_skill )
            {
                continue;
            }

            if( gObj[obj_index].pInventory[x].m_Option3 != item_option )
            {
                continue;
            }

            if( gObj[obj_index].pInventory[x].m_NewOption != item_excellent_option )
            {
                continue;
            }

            if( gObj[obj_index].pInventory[x].m_Option2 != 0)
            {
                continue;
            }

            if( gObj[obj_index].pInventory[x].m_SetOption != 0 )
            {
                continue;
            }

            BYTE NewOption[MAX_EXOPTION_SIZE] = {0};

            ItemIsBufExOption(NewOption, &gObj[obj_index].pInventory[x]);

            LogAddTD(
                "[QuestExp] DeleteInvenItem [%s][%s] Delete Item Info - Item:[%s,%d,%d,%d,%d] serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set[%d] 380:[%d] HO:[%d,%d] SC[%d,%d,%d,%d,%d] BonusOption[%d]",
                gObj[obj_index].AccountID,
                gObj[obj_index].Name,
                gObj[obj_index].pInventory[x].GetName(),
                gObj[obj_index].pInventory[x].m_Level,
                gObj[obj_index].pInventory[x].m_Option1,
                gObj[obj_index].pInventory[x].m_Option2,
                gObj[obj_index].pInventory[x].m_Option3,
                gObj[obj_index].pInventory[x].m_Number,
                (int)gObj[obj_index].pInventory[x].m_Durability,
                NewOption[0],
                NewOption[1],
                NewOption[2],
                NewOption[3],
                NewOption[4],
                NewOption[5],
                NewOption[6],
                gObj[obj_index].pInventory[x].m_SetOption,
                gObj[obj_index].pInventory[x].m_ItemOptionEx >> 7,
                g_kJewelOfHarmonySystem.GetItemStrengthenOption(&gObj[obj_index].pInventory[x]),
                g_kJewelOfHarmonySystem.GetItemOptionLevel(&gObj[obj_index].pInventory[x]),
                gObj[obj_index].pInventory[x].m_SocketOption[0],
                gObj[obj_index].pInventory[x].m_SocketOption[1],
                gObj[obj_index].pInventory[x].m_SocketOption[2],
                gObj[obj_index].pInventory[x].m_SocketOption[3],
                gObj[obj_index].pInventory[x].m_SocketOption[4],
				gObj[obj_index].pInventory[x].m_BonusSocketOption);

            gObjInventoryItemSet(obj_index, x, -1);
            gObj[obj_index].pInventory[x].Clear();
            GCInventoryItemDeleteSend(obj_index, x, 1);

            --item_count;
        }
        else
        {
            if ( gObjSearchItemMinus(&gObj[obj_index], x, 1) )
            {
                --x;
            }
            else
            {
                BYTE NewOption[MAX_EXOPTION_SIZE] = {0};

                ItemIsBufExOption(NewOption, &gObj[obj_index].pInventory[x]);

                LogAddTD(
                    "[QuestExp] DeleteInvenItem [%s][%s] Delete Item Info - Item:[%s,%d,%d,%d,%d] serial:[%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set[%d] 380:[%d] HO:[%d,%d] SC[%d,%d,%d,%d,%d] BonusOption[%d]",
                    gObj[obj_index].AccountID,
                    gObj[obj_index].Name,
                    gObj[obj_index].pInventory[x].GetName(),
                    gObj[obj_index].pInventory[x].m_Level,
                    gObj[obj_index].pInventory[x].m_Option1,
                    gObj[obj_index].pInventory[x].m_Option2,
                    gObj[obj_index].pInventory[x].m_Option3,
                    gObj[obj_index].pInventory[x].m_Number,
                    (int)gObj[obj_index].pInventory[x].m_Durability,
                    NewOption[0],
                    NewOption[1],
                    NewOption[2],
                    NewOption[3],
                    NewOption[4],
                    NewOption[5],
                    NewOption[6],
                    gObj[obj_index].pInventory[x].m_SetOption,
                    gObj[obj_index].pInventory[x].m_ItemOptionEx >> 7,
                    g_kJewelOfHarmonySystem.GetItemStrengthenOption(&gObj[obj_index].pInventory[x]),
                    g_kJewelOfHarmonySystem.GetItemOptionLevel(&gObj[obj_index].pInventory[x]),
                    gObj[obj_index].pInventory[x].m_SocketOption[0],
                    gObj[obj_index].pInventory[x].m_SocketOption[1],
                    gObj[obj_index].pInventory[x].m_SocketOption[2],
                    gObj[obj_index].pInventory[x].m_SocketOption[3],
                    gObj[obj_index].pInventory[x].m_SocketOption[4],
                    gObj[obj_index].pInventory[x].m_BonusSocketOption);

                gObjInventoryItemSet(obj_index, x, -1);
                gObj[obj_index].pInventory[x].Clear();
                GCInventoryItemDeleteSend(obj_index, x, 1);
            }

            --item_count;
        }

    }

}

// TODO: Replace item number magic values
void QuestExpProgMng::ChkUserQuestTypeItem(QuestGetItem* quest_get_item, 
                                        UserQuestAskInfo* user_quest_ask_info, 
                                        int obj_index)
{
    int item_count = 0;
    int item_durability = 0;
    int item_number = quest_get_item->GetQuestItemNum();
    int item_level = quest_get_item->GetQuestItemLevel();
    int item_option = quest_get_item->GetQuestItemOpt();
    int item_excellent_option = quest_get_item->GetQuestItemExOpt();
    int item_skill = quest_get_item->GetQuestItemSkill();

    for(int x = 0; x < INVENTORY_SIZE; ++x )
    {
        if( !gObj[obj_index].pInventory[x].IsItem() )
        {
            continue;
        }

        if( gObj[obj_index].pInventory[x].m_Type != item_number )
        {
            continue;
        }

        if( gObj[obj_index].pInventory[x].m_Level != item_level )
        {
            continue;
        }

        if( (gObj[obj_index].pInventory[x].m_Type < 7168 || gObj[obj_index].pInventory[x].m_Type > 7176)
            && (gObj[obj_index].pInventory[x].m_Type < 7203 || gObj[obj_index].pInventory[x].m_Type > 7208)
            && g_QuestExpManager.IsQuestItemAtt(gObj[obj_index].pInventory[x].m_Type, 1) != 1 )
        {
            if( gObj[obj_index].pInventory[x].m_Option1 == item_skill )
            {
                if( gObj[obj_index].pInventory[x].m_Option3 == item_option )
                {
                    if( gObj[obj_index].pInventory[x].m_NewOption == item_excellent_option )
                    {
                        if( !gObj[obj_index].pInventory[x].m_Option2 )
                        {
                            if( !gObj[obj_index].pInventory[x].m_SetOption )
                                ++item_count;
                        }
                    }
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

    user_quest_ask_info->SetValue(item_count);
    if( quest_get_item->GetQuestItemCnt() > item_count )
    {
        user_quest_ask_info->SetComplete(false);
    }
    else
    {
        if( !user_quest_ask_info->IsComplete() )
            user_quest_ask_info->SetComplete(true);
    }
}


void QuestExpProgMng::ChkMonsterKillPartyPlay(DWORD quest_index_id, 
                                                int ask_index, 
                                                QuestMonsterKill* quest_monster_kill, 
                                                OBJECTSTRUCT* lpObj, 
                                                OBJECTSTRUCT* lpMonsterObj)
{
    if( !quest_monster_kill )
    {
        return;
    }

    int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_index_id);
    int quest_switch = mu::quest::GetQuestSwitchFromInfoIndexId(quest_index_id);

    if( lpObj->m_pUserQuestInfo[episode_number].GetEpisode() != episode_number )
    {
        return;
    }

    if( lpObj->m_pUserQuestInfo[episode_number].GetQuestSwitch() != quest_switch )
    {
        return;
    }

    UserQuestAskInfo* user_quest_ask_info = &lpObj->m_pUserQuestInfo[episode_number].user_quest_ask_info_[ask_index];

    if( user_quest_ask_info->GetQuestType() != 1 )
    {
        return;
    }


    if( lpMonsterObj->Class != quest_monster_kill->GetMonsterIndex() )
    {
        return;
    }

    int kill_count = quest_monster_kill->GetMonsterKillCnt();

    if( kill_count > user_quest_ask_info->GetValue() )
    {
        user_quest_ask_info->SetValue(user_quest_ask_info->GetValue() + 1);

        LogAddTD(
            "[QuestExp] Party Play Ask Kill Monster [%s][%s] Ep[%d] QS[%d] MonName[%s] AskKillCnt[%d] KillCnt[%d] ",
            lpObj->AccountID,
            lpObj->Name,
            episode_number,
            quest_switch,
            lpMonsterObj->Name,
            user_quest_ask_info->GetValue(),
            quest_monster_kill->GetMonsterKillCnt());
    }

    if( quest_monster_kill->GetMonsterKillCnt() <= user_quest_ask_info->GetValue() )
    {
        if( !user_quest_ask_info->IsComplete() )
            user_quest_ask_info->SetComplete(true);
    }
}

void QuestExpProgMng::ChkUserQuestTypeMonsterKill(OBJECTSTRUCT *lpObj, 
                                                    OBJECTSTRUCT *lpMonsterObj)
{
    if ( lpObj->PartyNumber == -1 )
    {
        ChkUserQuestType(1, lpObj, lpMonsterObj, 0);
        return;
    }

    int party_number = lpObj->PartyNumber;
    int killer_obj_index = lpObj->m_Index;

    for(int party_user_count = 0; party_user_count < 5; ++party_user_count )
    {
        int party_user_index = gParty.m_PartyS[party_number].Number[party_user_count];
        if( party_user_index >= 0 )
        {
            OBJECTSTRUCT* lpPartyObj = &gObj[party_user_index];
            if( gObj[party_user_index].Connected >= PLAYER_PLAYING || !lpPartyObj->Live )
            {
                if( lpObj->MapNumber == lpPartyObj->MapNumber )
                {
                    if( gObjCalDistance(lpObj, lpPartyObj) <= 9 )
                        ChkUserQuestType(1, lpPartyObj, lpMonsterObj, killer_obj_index);
                }
            }
        }
    }

}

void QuestExpProgMng::ChkUserQuestType(int quest_type, 
                                        OBJECTSTRUCT *lpObj, 
                                        OBJECTSTRUCT *lpMonsterObj, 
                                        int killer_obj_index)
{
    if( !lpMonsterObj )
    {
        return;
    }

    for(int i = 0; i < 25; ++i )
    {
        if( lpObj->m_pUserQuestInfo[i].GetEpisode() == 0)
        {
            continue;
        }

        UserQuestInfo* user_quest_info = &lpObj->m_pUserQuestInfo[i];
        int ask_count = user_quest_info->GetAskCnt();

        for(int j = 0; j < ask_count; ++j )
        {
            if( quest_type != user_quest_info->user_quest_ask_info_[j].GetQuestType() )
            {
                continue;
            }

            UserQuestAskInfo* user_quest_ask_info = &user_quest_info->user_quest_ask_info_[j];
            int index_id = user_quest_ask_info->GetIndexID();
            int episode_number = user_quest_info->GetEpisode();
            int quest_switch = user_quest_info->GetQuestSwitch();

            QuestExpAsk* quest_exp_ask = g_QuestExpManager.GetQuestExpAsk(
                episode_number,
                quest_switch,
                index_id);

            if( !quest_exp_ask )
                return;

            if( quest_type == 1 )
            {
                QuestMonsterKill* quest_monster_kill = 
                        static_cast<QuestMonsterKill*>(quest_exp_ask);

                if( lpMonsterObj->Class != quest_monster_kill->GetMonsterIndex() )
                {
                    continue;
                }


                if( quest_monster_kill->GetMonsterKillCnt() > user_quest_ask_info->GetValue() )
                {

                    user_quest_ask_info->SetValue(user_quest_ask_info->GetValue() + 1);


                    if( !quest_monster_kill->IsPartyPlay() || lpObj->PartyNumber == -1 )
                    {
                        LogAddTD(
                            "[QuestExp] Ask Kill Monster [%s][%s] Ep[%d] QS[%d] MonName[%s] AskKillCnt[%d] KillCnt[%d] ",
                            lpObj->AccountID,
                            lpObj->Name,
                            episode_number,
                            quest_switch,
                            lpMonsterObj->Name,
                            quest_monster_kill->GetMonsterKillCnt(),
                            user_quest_ask_info->GetValue());
                    }
                    else
                    {
                        LogAddTD(
                            "[QuestExp] Party Play Ask Kill Monster [%s][%s] Ep[%d] QS[%d] MonName[%s] AskKillCnt[%d] KillCnt[%d] MonsterKiller[%s][%s]",
                            lpObj->AccountID,
                            lpObj->Name,
                            episode_number,
                            quest_switch,
                            lpMonsterObj->Name,
                            quest_monster_kill->GetMonsterKillCnt(),
                            user_quest_ask_info->GetValue(),
                            gObj[killer_obj_index].AccountID,
                            gObj[killer_obj_index].Name);
                    }
                }

                if( quest_monster_kill->GetMonsterKillCnt() <= user_quest_ask_info->GetValue() )
                {
                    if( !user_quest_ask_info->IsComplete() )
                        user_quest_ask_info->SetComplete(true);
                }

            }
            else if ( quest_type == 263 )
            {
                QuestNpcTalk* quest_npc_talk = 
                    static_cast<QuestNpcTalk*>(quest_exp_ask);

                if( quest_npc_talk->GetQuestNPCTalk() )
                {
                    if( lpMonsterObj->Class == quest_npc_talk->GetQuestNPCTalk() )
                    {
                        if ( !user_quest_ask_info->IsComplete() )
                        {
                            user_quest_ask_info->SetValue(1);
                            user_quest_ask_info->SetComplete(true);
                        }
                    }
                }
                else
                {
                    if ( lpMonsterObj->Class == 543 && gGensSystem.GetGensInfluence(lpObj) == 1
                        || lpMonsterObj->Class == 544 && gGensSystem.GetGensInfluence(lpObj) == 2 )
                    {
                        if ( !user_quest_ask_info->IsComplete() )
                        {
                            user_quest_ask_info->SetValue(1);
                            user_quest_ask_info->SetComplete(true);
                        }
                    }
                }
            }
        }
    }
}

void QuestExpProgMng::ChkUserQuestTypeEventMap(int quest_type, 
                                                OBJECTSTRUCT *lpObj, 
                                                int map_level, 
                                                int value)
{
    if( lpObj->Type != OBJ_USER )
    {
        return;
    }

    for(int i = 0; i < 25; ++i )
    {
        if( lpObj->m_pUserQuestInfo[i].GetEpisode() == 0 )
        {
            continue;
        }

        UserQuestInfo* user_quest_info = &lpObj->m_pUserQuestInfo[i];
        int ask_count = user_quest_info->GetAskCnt();

        for(int j = 0; j < ask_count; ++j )
        {
            if( quest_type != user_quest_info->user_quest_ask_info_[j].GetQuestType() )
            {
                continue;
            }

            UserQuestAskInfo* user_quest_ask_info = &user_quest_info->user_quest_ask_info_[j];
            int index_id = user_quest_ask_info->GetIndexID();
            int episode_number = user_quest_info->GetEpisode();
            int quest_switch = user_quest_info->GetQuestSwitch();

            QuestExpAsk* quest_exp_ask = g_QuestExpManager.GetQuestExpAsk(
                episode_number,
                quest_switch,
                index_id);

            switch ( quest_type )
            {
            case 64:
            case 65:
            case 66:
                {
                    QuestEventMapKillPoint* quest_event_map_kill_point = 
                        static_cast<QuestEventMapKillPoint*>(quest_exp_ask);

                    if( quest_event_map_kill_point->GetQuestEventMapLevel() - 1 == map_level )
                    {
                        int kill_count = quest_event_map_kill_point->GetQuestEventMapKillCnt();

                        if( kill_count > user_quest_ask_info->GetValue() )
                        {
                            user_quest_ask_info->SetValue(user_quest_ask_info->GetValue() + 1);

                            LogAddTD(
                                "[QuestExp] Ask Kill User [%s][%s] Ep[%d] QS[%d] RequestType[0x08%X] MapLevel[%d] AskKillCnt[%d] KillCnt[%d]",
                                lpObj->AccountID,
                                lpObj->Name,
                                episode_number,
                                quest_switch,
                                quest_type,
                                map_level,
                                quest_event_map_kill_point->GetQuestEventMapKillCnt(),
                                user_quest_ask_info->GetValue());
                        }
                    }

                    if( quest_event_map_kill_point->GetQuestEventMapKillCnt() <= user_quest_ask_info->GetValue() )
                    {
                        if( !user_quest_ask_info->IsComplete() )
                            user_quest_ask_info->SetComplete(true);
                    }
                }
                break;

            case 256:
            case 257:
            case 258:
            case 259:
                {
                    QuestEventMapClear* quest_event_map_clear = 
                        static_cast<QuestEventMapClear*>(quest_exp_ask);

                    if( quest_event_map_clear->GetQuestEventMapLevel() - 1 == map_level )
                    {
                        if( !user_quest_ask_info->IsComplete() )
                        {
                            user_quest_ask_info->SetComplete(true);
                            user_quest_ask_info->SetValue(1);

                            LogAddTD(
                                "[QuestExp] Ask Event Map Clear - Complete - [%s][%s] Ep[%d] QS[%d] QuestType[0x%08X]",
                                lpObj->AccountID,
                                lpObj->Name,
                                episode_number,
                                quest_switch,
                                quest_type);
                        }
                    }
                }
                break;

            case 260:
                {
                    QuestEventMapDevilPoint* quest_event_map_devil_point = 
                        static_cast<QuestEventMapDevilPoint*>(quest_exp_ask);

                    if( quest_event_map_devil_point->GetQuestEventMapLevel() - 1 == map_level )
                    {
                        int devil_point = quest_event_map_devil_point->GetQuestEventMapDevilPoint();

                        if( devil_point > user_quest_ask_info->GetValue() )
                        {

                            user_quest_ask_info->SetValue( 
                                value + user_quest_ask_info->GetValue());

                            LogAddTD(
                                "[QuestExp] Ask Devil Point [%s][%s] Ep[%d] QS[%d] RequestType[0x%08X] MapLevel[%d] AskDevilPoint[%d] CurDevilPoint[%d]",
                                lpObj->AccountID,
                                lpObj->Name,
                                episode_number,
                                quest_switch,
                                quest_type,
                                map_level,
                                quest_event_map_devil_point->GetQuestEventMapDevilPoint(),
                                user_quest_ask_info->GetValue());
                        }
                    }

                    if( quest_event_map_devil_point->GetQuestEventMapDevilPoint() <= user_quest_ask_info->GetValue() )
                    {
                        if( !user_quest_ask_info->IsComplete() )
                            user_quest_ask_info->SetComplete(true);
                    }
                }
                break;

            case 262:
                {
                    QuestPVP_Point* quest_pvp_point = 
                        static_cast<QuestPVP_Point*>(quest_exp_ask);

                    if( quest_pvp_point->GetQuestMapNum() == map_level )
                    {
                        if( quest_pvp_point->GetQuestPVP_Point() > user_quest_ask_info->GetValue() )
                        {

                            user_quest_ask_info->SetValue( 
                                    user_quest_ask_info->GetValue() + 1);

                            LogAddTD(
                                "[QuestExp] Ask PVP [%s][%s] Ep[%d] QS[%d] RequestType[0x08%X] MapNum[%d] AskKillCnt[%d] KillCnt[%d]",
                                lpObj->AccountID,
                                lpObj->Name,
                                episode_number,
                                quest_switch,
                                quest_type,
                                map_level,
                                quest_pvp_point->GetQuestPVP_Point(),
                                user_quest_ask_info->GetValue());
                        }
                    }

                    if( quest_pvp_point->GetQuestPVP_Point() <= user_quest_ask_info->GetValue() )
                    {
                        if ( !user_quest_ask_info->IsComplete() )
                            user_quest_ask_info->SetComplete(true);
                    }
                }

                break;

            }
        }
    }
}

void QuestExpProgMng::AddQuestSwitchList(int episode_number, int quest_switch)
{
    if( mu::quest::IsEpisode(episode_number) )
    {
        if ( quest_switch < 254 )
        {
            DWORD quest_index_id = mu::quest::GetQuestInfoIndexId(episode_number, quest_switch);
            switch_list_.push_back(quest_index_id);
        }
    }
    else
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
    }
}

#pragma pack(push, 1)
struct PMSG_QUESTSWITCH_LIST
{
    PBMSG_HEAD2 h;
    WORD target_npc;
    WORD count;
};
#pragma pack(pop)


void QuestExpProgMng::SendQuestSwitchList(int obj_index, int send_type)
{
    BYTE send_buf[1000];
    PMSG_QUESTSWITCH_LIST quest_list;
    int lOfs = sizeof(quest_list);
    quest_list.count = switch_list_.size();
    int iIndex = obj_index;

    auto list_it = switch_list_.begin();
    auto list_end = switch_list_.end();

    for(; list_it != list_end; ++list_it)
    {
        DWORD quest_index_id = *list_it;

        memcpy(&send_buf[lOfs], &quest_index_id, sizeof(quest_index_id));
        lOfs += sizeof(quest_index_id);

        int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_index_id);
        int quest_switch = mu::quest::GetQuestSwitchFromInfoIndexId(quest_index_id);

        if( send_type == 0)
        {
            LogAddTD(
                "[QuestExp] Send Quest List To NPC: [%s][%s] Ep[%d] QS[%d] QuestListCnt[%d]",
                gObj[iIndex].AccountID,
                gObj[iIndex].Name,
                episode_number,
                quest_switch,
                quest_list.count);
        }
        else if( send_type == 1 )
        {
            LogAddTD(
                "[QuestExp] Send Quest List To Event: [%s][%s] Ep[%d] QS[%d] QuestListCnt[%d]",
                gObj[iIndex].AccountID,
                gObj[iIndex].Name,
                episode_number,
                quest_switch,
                quest_list.count);
        }
        else if( send_type == 2 )
        {
            LogAddTD(
                "[QuestExp] Send Quest List Item Used: [%s][%s] Ep[%d] QS[%d] QuestListCnt[%d]",
                gObj[iIndex].AccountID,
                gObj[iIndex].Name,
                episode_number,
                quest_switch,
                quest_list.count);
        }

    }

    quest_list.target_npc = 0;

    if( send_type == 0)
    {
        PHeadSubSetB((BYTE *)&quest_list, 0xF6u, 0xAu, 0);
        quest_list.target_npc = gObj[iIndex].TargetShopNumber;
    }
    else if( send_type == 1 )
    {
        PHeadSubSetB((BYTE *)&quest_list, 0xF6u, 3u, 0);
    }

    else if( send_type == 2 )
    {
        PHeadSubSetB((BYTE *)&quest_list, 0xF6u, 4u, 0);
    }

    quest_list.h.size = lOfs;
    memcpy(send_buf, &quest_list, sizeof(quest_list));
    DataSend(iIndex, (BYTE*)send_buf, lOfs);


    // TODO: analyze why it's erased
    switch_list_.erase(switch_list_.begin(), switch_list_.end());
}

struct PMSG_QUEST_ASK_STATE
{
    PBMSG_HEAD2 h;
    int ukn4;
};


void QuestExpProgMng::SendQuestAskState(PMSG_QUEST_ASK_STATE* pMsg, int obj_index)
{
    PHeadSubSetB((BYTE *)pMsg, 0xF6u, 0xEu, 11);
    DataSend(obj_index, (BYTE*)pMsg, pMsg->h.size);

    // some asm code not generated buy decompiler, values not used
}

#pragma pack(push, 1)
struct PMSG_QUEST_COMPLETE_ANS
{
    PBMSG_HEAD2 h;
    int quest_index_id;
    char result;
};
#pragma pack(pop)


void QuestExpProgMng::GCANSQuestCompleteBtnClick(int obj_index, 
                                            DWORD quest_info_index_id, 
                                            char result)
{
    if( ChkQuestIndexIDToEpAndQSLimit(quest_info_index_id, obj_index) )
    {
        PMSG_QUEST_COMPLETE_ANS pMsg;

        pMsg.quest_index_id = quest_info_index_id;
        pMsg.result = result;
        PHeadSubSetB((BYTE *)&pMsg, 0xF6u, 0xDu, sizeof(pMsg));

        LogAddTD(
            "[QuestExp] Ans Complete [%s][%s] Ep [%d] QS [%d] Result [%d]",
            gObj[obj_index].Name,
            gObj[obj_index].AccountID,
            mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id),
            mu::quest::GetQuestSwitchFromInfoIndexId(quest_info_index_id),
            result);

        DataSend(obj_index, (BYTE*)&pMsg, pMsg.h.size);
    }
    else
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
    }
}

#pragma pack(push, 1)
struct PMSG_QUESTPROGRESS_LIST
{
    PBMSG_HEAD2 h;
    __int8 count;
};
#pragma pack(pop)


void QuestExpProgMng::SendProgressQuestList(int obj_index)
{
    BYTE send_buf[1000] = {0};
    int quest_count = 0;
    PMSG_QUESTPROGRESS_LIST pMsg;
    int lOfs = sizeof(pMsg);

    for(int i = 0; i < 25; ++i )
    {
        int episode_number = gObj[obj_index].m_pUserQuestInfo[i].GetEpisode();
        int quest_switch = gObj[obj_index].m_pUserQuestInfo[i].GetQuestSwitch();
        int progress_state = gObj[obj_index].m_pUserQuestInfo[i].GetQuestProgState();

        if( progress_state == 0)
            continue;

        if( episode_number == 0)
            continue;

        if( quest_switch == 0)
            continue;

        if( quest_switch == 254 || quest_switch == 255 )
            continue;

        DWORD quest_index_id = mu::quest::GetQuestInfoIndexId(episode_number, quest_switch);

        memcpy(&send_buf[lOfs], &quest_index_id, sizeof(quest_index_id));
        lOfs += sizeof(quest_index_id);
        ++quest_count;

        LogAddTD(
            "[QuestExp] Send QuestProg List : [%s][%s] Ep[%d] QS[%d]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            episode_number,
            quest_switch);

    }

    pMsg.count = quest_count;
    PHeadSubSetB((BYTE *)&pMsg, 0xF6u, 0x1Au, lOfs);
    memcpy(send_buf, &pMsg, 5);

    DataSend(obj_index, (BYTE *)send_buf, lOfs);
}


void QuestExpProgMng::SetQuestProg(int episode_number, int obj_index, int progress_state)
{
    if( mu::quest::IsEpisode(episode_number) )
    {
        if( gObj[obj_index].m_pUserQuestInfo[episode_number].GetQuestProgState() != 2 )
            gObj[obj_index].m_pUserQuestInfo[episode_number].SetQuestProgState(progress_state);
    }
    else
    {
        LogAddTD(
            "[QuestExp] - Error - SetQuestProg : [%s][%s] Ep[%d]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            episode_number);
    }
}

bool QuestExpProgMng::ChkQuestIndexIDToEpLimit(DWORD quest_index_id, int obj_index)
{
    int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_index_id);

    if( mu::quest::IsEpisode(episode_number) )
    {
        return true;
    }
    else
    {
        LogAddTD(
            "[QuestExp] - Error - ChkEpisodeLimit : [%s][%s] Ep[%d] QuestIndexID[0x%x]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            episode_number,
            quest_index_id);

        return false;
    }
}

bool QuestExpProgMng::ChkQuestIndexIDToQSLimit(DWORD quest_index_id, int obj_index)
{
    int quest_switch = mu::quest::GetQuestSwitchFromInfoIndexId(quest_index_id);

    if( mu::quest::IsQuestSwitch(quest_switch) )
    {
        return true;
    }
    else
    {
        LogAddTD(
            "[QuestExp] - Error - ChkQuestSwitchLimit : [%s][%s] QS[%d] QuestIndexID[0x%x]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            quest_switch,
            quest_index_id);

        return false;
    }
}

bool QuestExpProgMng::ChkQuestIndexIDToEpAndQSLimit(DWORD quest_index_id, int obj_index)
{
    int quest_switch = mu::quest::GetQuestSwitchFromInfoIndexId(quest_index_id);
    int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_index_id);

    if( !OBJMAX_RANGE(obj_index) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);

        return false;
    }

    if( !mu::quest::IsEpisode(episode_number) )
    {
        LogAddTD(
            "[QuestExp] - Error - ChkEpAndQSLimit : [%s][%s] Ep[%d] QuestIndexID[0x%x]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            episode_number,
            quest_index_id);

        return false;
    }

    if( !mu::quest::IsQuestSwitch(quest_switch) )
    {
        LogAddTD(
            "[QuestExp] - Error - ChkEpAndQSLimit : [%s][%s] QS[%d] QuestIndexID[0x%x]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            quest_switch,
            quest_index_id);

        return false;
    }

    return true;
}


void QuestExpProgMng::QuestMonsterItemDrop(DWORD quest_index_id, 
                                                OBJECTSTRUCT *lpObj, 
                                                OBJECTSTRUCT *lpMonsterObj)
{
    int max_hit_user_index = gObjMonsterTopHitDamageUser(lpMonsterObj);

    if( max_hit_user_index == -1 )
        return;

    for(int i = 0; i < 25; ++i )
    {
        int episode_number = lpObj->m_pUserQuestInfo[i].GetEpisode();
        int quest_switch = lpObj->m_pUserQuestInfo[i].GetQuestSwitch();

        if( episode_number == 0)
            continue;

        if ( quest_switch  == 0)
            continue;

        QuestDropItemInfo* quest_drop_item_info = g_QuestExpManager.GetQuestDropItemInfo(quest_switch);

        if( !quest_drop_item_info )
            continue;

        if( quest_drop_item_info->GetMonsterIndex() != lpMonsterObj->Class )
            continue;

        if( ChkQuestMonsterItemDrop(quest_drop_item_info) )
        {
            quest_drop_item_info->GetAskIndex();

            int item_number = quest_drop_item_info->GetItemNum();
            int item_level = quest_drop_item_info->GetItemLevel();

            ItemSerialCreateSend(
                lpObj->m_Index,
                lpObj->MapNumber,
                lpObj->X,
                lpObj->Y,
                item_number,
                item_level,
                0.0,
                0,
                0,
                0,
                max_hit_user_index,
                0, 
				0);

            LogAddTD(
                "[QuestExp] Monster Kill Quest Item Drop - MonsterName[%s]: [%s][%s] ItemNum[%d]",
                lpMonsterObj->Name,
                lpObj->AccountID,
                lpObj->Name,
                item_number);
        }          
    }
}

bool QuestExpProgMng::ChkQuestMonsterItemDrop(QuestDropItemInfo* quest_drop_item_info)
{
    if( quest_drop_item_info )
    {
        int rand_rate = quest_drop_item_info->GetRewardRate();

        if( (int)(rand() / 32767.0 * 9999.0) < rand_rate)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

bool QuestExpProgMng::ChkQuestMonsterItemDrop(DWORD quest_index_id)
{
    QuestDropItemInfo* quest_drop_item_info = g_QuestExpManager.GetQuestDropItemInfo(quest_index_id);

    if( quest_drop_item_info )
    {
        int rand_rate = quest_drop_item_info->GetRewardRate();

        if( (int)(rand() / 32767.0 * 9999.0) < rand_rate)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

bool QuestExpProgMng::IsQuestDropItem(int index, int type, int level)
{
    OBJECTSTRUCT* lpObj = &gObj[index];

    for(int i = 0; i < 25; ++i )
    {
        int episode_number = lpObj->m_pUserQuestInfo[i].GetEpisode();
        int quest_switch = lpObj->m_pUserQuestInfo[i].GetQuestSwitch();

        if( episode_number == 0)
            continue;

        if( quest_switch == 0 )
            continue;

        QuestDropItemInfo* quest_drop_item_info = g_QuestExpManager.GetQuestDropItemInfo(
                mu::quest::GetQuestInfoIndexId(episode_number, quest_switch));

        if( !quest_drop_item_info )
            continue;

        UserQuestAskInfo* user_quest_ask_info = &lpObj->m_pUserQuestInfo[i].user_quest_ask_info_[quest_drop_item_info->GetAskIndex() - 1];

        if( quest_drop_item_info->GetItemNum() == type )
        {
            if( !ChkQuestAsk(episode_number, index) )
            {
                LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);

                return false;
            }

            if( !user_quest_ask_info->IsComplete() )
                return true;
        }
    }

    return false;
}


void QuestExpProgMng::SetQuestTimeLimit(int obj_index, DWORD quest_index_id, int duration)
{
    OBJECTSTRUCT* lpObj = &gObj[obj_index];

    for(int i = 0; i < 25; ++i )
    {
        int episode_number = lpObj->m_pUserQuestInfo[i].GetEpisode();
        int quest_switch = lpObj->m_pUserQuestInfo[i].GetQuestSwitch();
        DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode_number, quest_switch);

        if( episode_number && quest_switch && quest_info_index_id == quest_index_id )
        {
            lpObj->m_pUserQuestInfo[i].SetStartDate(GetCurrentDate());
            lpObj->m_pUserQuestInfo[i].SetEndDate(GetExpireDate(duration));
            lpObj->m_pUserQuestInfo[i].SetQuestProgState(2);

            CTime tStartTime(lpObj->m_pUserQuestInfo[i].GetStartDate());
            CTime tEndTime(lpObj->m_pUserQuestInfo[i].GetEndDate());

            LogAddTD(
                "[QuestExp] SetQuestTimeLimit [%s][%s] Ep[%d] QS[%d] StartDate : %d-%.2d-%.2d %.2d:%.2d:%.2d EndDate : %d-%.2d-%.2d %.2d:%.2d:%.2d",
                lpObj->AccountID,
                lpObj->Name,
                episode_number,
                quest_switch,
                tStartTime.GetYear(),
                tStartTime.GetMonth(),
                tStartTime.GetDay(),
                tStartTime.GetHour(),
                tStartTime.GetMinute(),
                tStartTime.GetSecond(),
                tEndTime.GetYear(),
                tEndTime.GetMonth(),
                tEndTime.GetDay(),
                tEndTime.GetHour(),
                tEndTime.GetMinute(),
                tEndTime.GetSecond());

            return;
        }
    }
}

int QuestExpProgMng::GetCurrentDate()
{
    CTime tCurrent(CTime::GetTickCount());
    return tCurrent.GetTime();
}

int QuestExpProgMng::GetExpireDate(int duration)
{
    CTime tCurrentTime(CTime::GetTickCount());
    CTimeSpan tLeftTime(0, 0, duration, 0);

    CTime tExpireTime = tCurrentTime + tLeftTime;

    return tExpireTime.GetTime();
}

bool QuestExpProgMng::CheckExpireDate(int item_expire_date)
{
    return (GetCurrentDate() > item_expire_date);
}

int QuestExpProgMng::GetLeftDate(int lExpireDate)
{
    CTime tExpireTime(lExpireDate);
    CTime tCurrentTime(CTime::GetTickCount());

    CTimeSpan tSpantime = tExpireTime - tCurrentTime;

    return tSpantime.GetTotalMinutes();
}