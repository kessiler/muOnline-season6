#include "stdafx.h"
#include "QuestExpUserMng.h"
#include "QuestExpInfo.h"
#include "QuestExpUserInfo.h"
#include "user.h"
#include "logproc.h"
#include "MasterLevelSystem.h"
#include "QuestExpProgMng.h"
#include "DBSockMng.h"
#include "DSProtocol.h"
#include "..\common\winutil.h"
#include "BuffEffectSlot.h"
#include "GensSystem.h"
#include "GameMain.h"


QuestExpUserMng g_QuestExpUserMng;

QuestExpUserMng::QuestExpUserMng(void)
{
}

QuestExpUserMng::~QuestExpUserMng(void)
{
}

int QuestExpUserMng::IsProgQuestInfo(int episode_number, int obj_index)
{
    if( mu::quest::IsEpisode(episode_number) )
    {
        return gObj[obj_index].m_pUserQuestInfo[episode_number].GetAskCnt();
    }
    else
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return 0;
    }
}

// TODO: OVERLOADED unsigned long / int
int QuestExpUserMng::IsProgQuestInfo(DWORD quest_info_index_id, int obj_index)
{
    return IsProgQuestInfo(
        mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id), 
        obj_index);
}


bool QuestExpUserMng::IsQuestComplete(int episode_number, int obj_index)
{

    UserQuestInfo* user_quest_info = GetUserQuestInfo(episode_number, obj_index);

    if( !user_quest_info )
    {
        LogAddTD( "[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return false;
    }


    int ask_count = user_quest_info->GetAskCnt();

    // UserQuestInfo::GetAskCnt() will always return valid value
    // so this check is useless
    if( ask_count >= UserQuestInfo::kMaxAskInfo )
    {
        LogAddTD(
            "[QuestExp] - Error - IsQuestComplete [%s][%s] iAskCnt[%d]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            ask_count);

        return false;
    }


    if( !g_QuestExpProgMng.ChkQuestAsk(episode_number, obj_index) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]",
            __FILE__, __LINE__);

        return false;
    }


    // user_quest_info is queried earlier
    // no reason to access directly

    for(int i = 0; i < ask_count; ++i )
    {
        if ( !gObj[obj_index].m_pUserQuestInfo[episode_number].user_quest_ask_info_[i].IsComplete() )
            return 0;
    }


    // review this
    if( ask_count )
    {
        //quest_switch = UserQuestInfo__GetQuestSwitch((char *)gObj[obj_index].m_pUserQuestInfo + 104 * episode_number);
        int quest_switch = user_quest_info->GetQuestSwitch();

        LogAddTD(
            "[QuestExp] AskQuestComplete [%s][%s] Ep[%d] QS[%d]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            episode_number,
            quest_switch);

        return true;
    }
    else
    {
        //v4 = UserQuestInfo__GetQuestSwitch((char *)gObj[obj_index].m_pUserQuestInfo + 104 * episode_number);

        int quest_switch = user_quest_info->GetQuestSwitch();

        LogAddTD(
            "[QuestExp] IsQuestComplete [%s][%s] Ep[%d] QS[%d] ***HackUser***",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            episode_number,
            quest_switch);

        return false;
    }
}

bool QuestExpUserMng::QuestExpExtentCheck(int x, int y, int w, int h)
{
    return (x >= 0 && x < w && y >= 0 && y < h);
}

bool QuestExpUserMng::QuestExpCheckInventoryEmptySpace(BYTE* temp_inventory_map, 
                                int item_height, int item_width, int obj_index)
{
    for(int h = 0; h < 24; ++h )
    {
        for(int w = 0; w < 8; ++w )
        {
            if( temp_inventory_map[w + h*8] == 255 )
            {
                BYTE blank = QuestExpOnlyInventoryRectCheck(temp_inventory_map,
                                                              w, h,
                                                              item_width,
                                                              item_height,
                                                              obj_index);

                if( blank == 254 )
                    return false;

                if( blank != 255 )
                {
                    QuestExpTempInventoryItemSet(
                        temp_inventory_map,
                        blank,
                        item_width,
                        item_height,
                        1,
                        obj_index);

                    return true;
                }
            }
        }
    }

    return false;
}


BYTE QuestExpUserMng::QuestExpOnlyInventoryRectCheck(BYTE *temp_inventory_map, 
                                                    int sx, int sy, 
                                                    int width, int height, 
                                                    int obj_index)
{

    //TODO: need to replace magic values with constants

    if( width + sx > 8 )
    {
        return -1;
    }

	if( height + sy > 4 * gObj[obj_index].ExpandedInventory + 8 )
    {
        return -2;
    }

    int bResult = CheckOutOfInventory(obj_index, sy, height);

    if( bResult ==0 )
    {
        return -1;
    }

    if ( bResult == -1 )
    {
        return -2;
    }

    for(int y = 0; y < height; ++y )
    {
        for(int x = 0; x < width; ++x )
        {
            if(InventoryExtentCheck(x + sx, y + sy, 8, 24) != 1 )
            {
                LogAdd("error : %s %d", __FILE__, __LINE__);

                return -1;
            }

            if( temp_inventory_map[x + sx + (y + sy)*8] != 255 )
                return -1;
        }
    }

    BYTE blank = sx + 8 * sy + 12;

    return blank;
}

void QuestExpUserMng::QuestExpTempInventoryItemSet(BYTE *temp_inventory_map, 
                                                    int itempos, 
                                                    int xl, int yl, 
                                                    BYTE set_byte, 
                                                    int obj_index)
{

    int itempos_x = (itempos - 12) % 8;
    int itempos_y = (itempos - 12) / 8;

    if( CheckOutOfInventory(obj_index, itempos_y, yl) == 1 )
    {
        for(int y = 0; y < yl; ++y )
        {
            for(int x = 0; x < xl; ++x )
            {
                if( InventoryExtentCheck(x + itempos_x, y + itempos_y, 8, 24) != 1 )
                {
                    LogAdd("error : %s %d", __FILE__, __LINE__);

                    return;
                }

                temp_inventory_map[x + itempos_x + (y + itempos_y)*8] = set_byte;
            }
        }
    }
}

bool QuestExpUserMng::GetRandomRewardItemIndex(int* set_item, 
                                QuestExpRewardKindRandom* reward_kind_random)
{
    if( !reward_kind_random )
    {
        LogAddTD("[QuestExp] - Error - GetRandomRewardItemIndex(NULL)");
        return false;
    }


    int reward_count = reward_kind_random->GetRandRewardCnt();
    int reward_item_total_count = reward_kind_random->GetRewardItemTotCnt();
    const int* reward_index = reward_kind_random->GetRewardIndex();
    const int* reward_rate = reward_kind_random->GetRewardRate();
    memset(set_item, -1, 20);

    for(int i = 0; i < reward_count; ++i )
    {
        int set_item_tmp = 0;
        bool is_set_item = false;
        int iRand = (rand() / 32767.0 * 9999.0);

        for(int j = 0; j < reward_item_total_count; ++j )
        {
            if( set_item[j] == -1 )
            {
                if( iRand < reward_rate[j] )
                {
                    set_item_tmp = j;
                    is_set_item = true;
                }
            }
        }

        if( is_set_item )
        {
            set_item[set_item_tmp] = set_item_tmp;
            LogAddTD("[QuestExp] - (RandomSetItem) RandomValue [%d] : [%d]", 
                iRand, set_item[set_item_tmp] + 1);
        }

        if( !is_set_item )
        {
            int rand_rate_temp = 0;
            set_item_tmp = 0;

            for(int k = 0; k < reward_item_total_count; ++k )
            {
                if( set_item[k] == -1 )
                {
                    if( rand_rate_temp < reward_rate[k] )
                    {
                        rand_rate_temp = (reward_rate[k]);
                        set_item_tmp = k;
                    }
                }
            }

            set_item[set_item_tmp] = set_item_tmp;
            LogAddTD("[QuestExp] - (SetItem) RandomValue [%d] : [%d]", 
                iRand, set_item[set_item_tmp] + 1);
        }
    }

    return true;
}

bool QuestExpUserMng::IsRandRewardIndex(int reward_index_id, 
                                QuestExpRewardKindRandom *reward_kind_random)
{
    if( reward_kind_random )
    {
        const int* rand_reward_index_id = reward_kind_random->GetRewardIndex();
        int reward_item_total_count = reward_kind_random->GetRewardItemTotCnt();

        for(int i = 0; i < reward_item_total_count; ++i )
        {
            if( rand_reward_index_id[i] == reward_index_id )
                return true;
        }

        return false;
    }
    else
    {
        LogAddTD("[QuestExp] - Error - %s(NULL)", __FUNCTION__);

        return false;
    }
}

bool QuestExpUserMng::IsRandResultReward(int reward_index_id, 
                                  QuestExpRewardKindRandom* reward_kind_random, 
                                  int* rand_result_index_id)
{
    if( reward_kind_random )
    {
        const int* rand_reward_index_id = reward_kind_random->GetRewardIndex();
        int rand_reward_count = reward_kind_random->GetRandRewardCnt();
        int reward_item_total_count = reward_kind_random->GetRewardItemTotCnt();

        for(int i = 0; i < reward_item_total_count; ++i )
        {
            if( rand_reward_index_id[i] == reward_index_id 
                && rand_result_index_id[i] == reward_index_id )
            {
                return true;
            }
        }

        return false;
    }
    else
    {
        LogAddTD("[QuestExp] - Error - %s(NULL)", __FUNCTION__);

        return false;
    }
}


//TODO: replace magic values
int QuestExpUserMng::InvenChk_EnableReward(int episode_number, 
                                            int obj_index, 
                                            int* rand_result_index_id)
{
    if( !mu::quest::IsEpisode(episode_number) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);

        return 0;
    }

    int iResult = 0;
    int quest_switch = gObj[obj_index].m_pUserQuestInfo[episode_number].GetQuestSwitch();

    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode_number,
                                                                quest_switch);

    QuestExpInfo* quest_exp_info = g_QuestExpManager.GetQuestExpInfo(quest_info_index_id);

    if( !quest_exp_info )
    {
        LogAddTD(
            "[QuestExp] - Error - InvenChk_EnableReward [%s][%s] Ep[%d] QS[%d]",
            gObj[obj_index].AccountID, gObj[obj_index].Name,
            episode_number, quest_switch);

        return 0;
    }

    auto map_quest_reward = quest_exp_info->GetQuestReward();

    BYTE temp_inventory_map[192];
    memcpy(temp_inventory_map, gObj[obj_index].pInventoryMap, 192);

    int random_reward_index[5] = {0};

    QuestExpRewardKindRandom* reward_kind_random = nullptr;
    QuestExpRewardKind* reward_kind = g_QuestExpManager.GetQuestExpRewardKind(quest_info_index_id);

    if( reward_kind && reward_kind->GetRewardKind() == 1 )
    {
        QuestExpRewardKindRandom* reward_kind_random = 
            static_cast<QuestExpRewardKindRandom*>(reward_kind);

        bool bGetRandomRewardItemIndex = GetRandomRewardItemIndex(random_reward_index, reward_kind_random);

        if( !bGetRandomRewardItemIndex )
        {
            return 0;
        }

        memcpy(rand_result_index_id, random_reward_index, sizeof(random_reward_index));
    }

    auto map_it = map_quest_reward.begin();
    auto map_end = map_quest_reward.end();

    for(; map_it != map_end; ++map_it)
    {

        QuestExpReward* quest_exp_reward = map_it->second;

        if( !quest_exp_reward )
        {
            LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
            return 0;
        }

        int quest_type = quest_exp_reward->GetRewardType();

        if( reward_kind )
        {

            bool bRandResultReward = IsRandResultReward(quest_exp_reward->GetIndexID(), 
                                                        reward_kind_random, 
                                                        random_reward_index);

            bool bRandIndex = IsRandRewardIndex(quest_exp_reward->GetIndexID(), 
                                                            reward_kind_random);

            if( !bRandResultReward && bRandIndex)
            {
                quest_type = 0;
            }
        }

        if( quest_type == 2 )
        {
            RewardZen* reward_zen = static_cast<RewardZen*>(quest_exp_reward);

            if ( !gObjCheckMaxZen(obj_index, reward_zen->GetRewardZen()) )
            {
                return 2;
            }
        }
        else if( quest_type == 4 )
        {
            RewardItem* reward_item = static_cast<RewardItem*>(quest_exp_reward);

            int item_count = reward_item->GetRewardItemCnt();
            int item_num = reward_item->GetRewardItemNum();

            for(int j = 0; j < item_count; ++j )
            {
                int item_height = 0;
                int item_width = 0;
                ItemGetSize(item_num, item_width, item_height);

                if ( !QuestExpCheckInventoryEmptySpace(temp_inventory_map,
                                            item_height, item_width, obj_index))
                {
                    return 3;
                }
            }
        }
    }

    return 1;
}

int QuestExpUserMng::SendQuestReward(int episode_number, int obj_index)
{
    if( !mu::quest::IsEpisode(episode_number) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);

        return 0;
    }

    int RandResultIndexId[5];
    int iResult = InvenChk_EnableReward(episode_number, obj_index, RandResultIndexId);
    int quest_switch = gObj[obj_index].m_pUserQuestInfo[episode_number].GetQuestSwitch();

    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode_number, quest_switch);

    if( iResult != 1 )
    {
        g_QuestExpProgMng.GCANSQuestCompleteBtnClick(obj_index, quest_info_index_id, iResult);
        return iResult;
    }

    QuestExpInfo* quest_exp_info = g_QuestExpManager.GetQuestExpInfo(quest_info_index_id);
    if( !quest_exp_info )
    {
        LogAddTD(
            "[QuestExp] - Error - QuestExpInfo [%s][%s] Ep [%d] QS[%d] QuestInfoIndexID [0x%x]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            episode_number,
            quest_switch,
            quest_info_index_id);

        return 0;
    }

    QuestExpRewardKindRandom* reward_kind_random = nullptr;
    QuestExpRewardKind* reward_kind = g_QuestExpManager.GetQuestExpRewardKind(quest_info_index_id);

    if( reward_kind )
    {
        if( reward_kind->GetRewardKind() == 1 )
            reward_kind_random = static_cast<QuestExpRewardKindRandom*>(reward_kind);
    }

    auto map_quest_reward = quest_exp_info->GetQuestReward();

    auto reward_iter = map_quest_reward.begin();
    auto map_end = map_quest_reward.end();

    for(; reward_iter != map_end; ++reward_iter)
    {        
        QuestExpReward* quest_exp_reward = reward_iter->second;
        int quest_type = quest_exp_reward->GetRewardType();

        char log_buf[128] = {0};
        wsprintf(log_buf, "[QuestExp] ");

        if( reward_kind )
        {

            bool is_rand_result_reward = IsRandResultReward(quest_exp_reward->GetIndexID(), 
                                                reward_kind_random, 
                                                RandResultIndexId);

            bool is_rand_reward_index = IsRandRewardIndex(quest_exp_reward->GetIndexID(), 
                                                reward_kind_random);
            if( !is_rand_result_reward && is_rand_reward_index )
                quest_type = 0;
            else
                wsprintf(log_buf, "[QuestExp] (Random) ");

        }

        switch ( quest_type )
        {
        case 1:
            {
                RewardExp* reward_exp = 
                    static_cast<RewardExp*>(quest_exp_reward);

                __int64 exp = reward_exp->GetRewardExp();
                //if( gObjCheckUsedBuffEffect(&gObj[obj_index], 42)
                //    || gObjCheckUsedBuffEffect(&gObj[obj_index], 31) )
				if( ::gObjCheckUsedBuffEffect(&gObj[obj_index], 42)
					|| ::gObjCheckUsedBuffEffect(&gObj[obj_index], 31) )
                {
                    exp = 0;
                }

                gObj[obj_index].Experience += exp;
                gObjLevelUp(&gObj[obj_index], exp, 0, 0);

                //if ( !g_MasterLevelSystem.IsMasterLevelUser(&gObj[obj_index]) )
                //    GCSendExp_INT64(obj_index, 65535, exp, 0, 0);

				if( !g_MasterLevelSystem.IsMasterLevelUser(&gObj[obj_index]) )
							GCSendExp_INT64(obj_index, -1, exp, 0, 0);

				strcat(log_buf, "Reward Exp [%s][%s] Exp[%I64d] Ep[%d] QS[%d]");
                LogAddTD(log_buf, gObj[obj_index].AccountID, gObj[obj_index].Name, 
                    reward_exp->GetRewardExp(), episode_number, quest_switch);
            }

            break;

        case 2:
            {
                RewardZen* reward_zen = 
                    static_cast<RewardZen*>(quest_exp_reward);

                gObj[obj_index].Money += reward_zen->GetRewardZen();
                GCMoneySend(obj_index, gObj[obj_index].Money);

                strcat(log_buf, "Reward Money [%s][%s] Money[%d] Ep[%d] QS[%d]");
                LogAddTD(log_buf, gObj[obj_index].AccountID, gObj[obj_index].Name, 
                    reward_zen->GetRewardZen(), episode_number, quest_switch);
            }

            break;

        case 4:
            {
                RewardItem* reward_item = 
                    static_cast<RewardItem*>(quest_exp_reward);

                int reward_item_count = reward_item->GetRewardItemCnt();
                for(int i = 0; i < reward_item_count; ++i )
                {
                    ItemSerialCreateSend(
                        obj_index,
                        0xEFu,
                        gObj[obj_index].X,
                        gObj[obj_index].Y,
                        reward_item->GetRewardItemNum(),
                        reward_item->GetRewardItemLevel(),
                        reward_item->GetRewardItemDur(),
                        reward_item->GetRewardItemSkill(),
                        0,
                        reward_item->GetRewardItemOpt(),
                        obj_index,
                        reward_item->GetRewardItemExOpt(),
                        0);

                    char szRewardItemName[32] = "";

                    int reward_item_number = reward_item->GetRewardItemNum();
                    ITEM_ATTRIBUTE* pItemAttribute = GetItemAttr(reward_item_number);
                    if( pItemAttribute )
                        strcpy(szRewardItemName, pItemAttribute->Name);

                    char item_log_buf[128] = "";

                    strcpy(item_log_buf, log_buf);
                    strcat(item_log_buf, "Reward Item [%s][%s] Ep[%d] QS[%d] ItemName[%s] Item Num[%d] Level[%d] Dur[%d] skill[%d]");

                    LogAddTD(
                        item_log_buf,
                        gObj[obj_index].AccountID,
                        gObj[obj_index].Name,
                        episode_number,
                        quest_switch,
                        szRewardItemName,
                        reward_item->GetRewardItemNum(),
                        reward_item->GetRewardItemLevel(),
                        reward_item->GetRewardItemDur(),
                        reward_item->GetRewardItemSkill());
                } // for
            }

            break;

        case 16:
            {
                RewardContribute* reward_contribute = 
                    static_cast<RewardContribute*>(quest_exp_reward);

                int contribute_point = reward_contribute->GetRewardContributePoint();
                gGensSystem.AddContributePoint(&gObj[obj_index], contribute_point);

                strcat(log_buf, "Reward Contirbute Point [%s][%s] Point[%d] Ep[%d] QS[%d]");
                LogAddTD(log_buf, gObj[obj_index].AccountID, gObj[obj_index].Name, 
                    contribute_point, episode_number, 
                    quest_switch);

                char client_msg[128] = "";
                wsprintf(client_msg, lMsg.Get(3456), contribute_point, 
                    gGensSystem.GetContributePoint(&gObj[obj_index]));

                GCServerMsgStringSend(client_msg, obj_index, 1);
            }

            break;

        default:
            continue;
        }
    }

    InitUserQuestAskInfo(episode_number, obj_index);
    g_QuestExpProgMng.GCANSQuestCompleteBtnClick(obj_index, quest_info_index_id, 1);

    return iResult;
}

void QuestExpUserMng::InitUserQuestAskInfo(int episode_number, int obj_index)
{
    if( mu::quest::IsEpisode(episode_number) )
    {
        gObj[obj_index].m_pUserQuestInfo[episode_number].QuestAskInfoClear();
    }
    else
    {
        LogAddTD( "[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
    }
}

void QuestExpUserMng::SetQuestSwitch(int episode_number, int quest_switch, int obj_index)
{
    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode_number, quest_switch);
    UserQuestInfo* user_quest_info = GetUserQuestInfo(episode_number, obj_index);

    if( !user_quest_info )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
    }

    int user_quest_info_index_id = mu::quest::GetQuestInfoIndexId(
                                        user_quest_info->GetEpisode(),
                                        user_quest_info->GetQuestSwitch());

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpAndQSLimit(quest_info_index_id, obj_index) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
    }

    if( user_quest_info_index_id != quest_info_index_id )
    {
        if( user_quest_info->SetEpisode(episode_number) )
        {
            if( user_quest_info->SetQuestSwitch(quest_switch) )
            {
                int user_quest_switch = gObj[obj_index].m_pUserQuestInfo[episode_number].GetQuestSwitch();
                int user_episode_number = gObj[obj_index].m_pUserQuestInfo[episode_number].GetEpisode();

                LogAddTD(
                    "[QuestExp] Set Quest Switch [%s][%s] EP[%d] QS[%d]",
                    gObj[obj_index].AccountID,
                    gObj[obj_index].Name,
                    user_episode_number,
                    user_quest_switch);

                if( AddUserQuestAskInfo(quest_info_index_id, obj_index) )
                    gObj[obj_index].m_pUserQuestInfo[episode_number].SetQuestProgState(1);
            }
            else
            {
                int user_quest_switch = gObj[obj_index].m_pUserQuestInfo[episode_number].GetQuestSwitch();
                int user_episode_number = gObj[obj_index].m_pUserQuestInfo[episode_number].GetEpisode();
                LogAddTD(
                    "[QuestExp] - Error - SetQuestSwitch [%s][%s] Cur EP[%d] Cur QS[%d]",
                    gObj[obj_index].AccountID,
                    gObj[obj_index].Name,
                    user_episode_number,
                    user_quest_switch);
            }
        }
        else
        {
            int user_quest_switch = gObj[obj_index].m_pUserQuestInfo[episode_number].GetQuestSwitch();
            int user_episode_number = gObj[obj_index].m_pUserQuestInfo[episode_number].GetEpisode();
            LogAddTD(
                "[QuestExp] - Error - SetQuestSwitch [%s][%s] Cur EP[%d] Cur QS[%d]",
                gObj[obj_index].AccountID,
                gObj[obj_index].Name,
                user_episode_number,
                user_quest_switch);
        }
    }
}

bool QuestExpUserMng::AddUserQuestAskInfo(DWORD quest_info_index_id, int obj_index)
{
    QuestExpInfo* quest_exp_info = g_QuestExpManager.GetQuestExpInfo(quest_info_index_id);
    if( !quest_exp_info )
    {
        return false;
    }

    int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id);
    auto MapQuestExpAsk = quest_exp_info->GetQuestExpAsk();

    int ask_count = 0;
    UserQuestInfo* user_quest_info = &gObj[obj_index].m_pUserQuestInfo[episode_number];

    user_quest_info->SetAskCnt(MapQuestExpAsk.size());
    auto AskIter = MapQuestExpAsk.begin();
    auto map_end = MapQuestExpAsk.end();

    while ( 1 )
    {
        if(AskIter == map_end)
            break;

        QuestExpAsk* quest_exp_ask = AskIter->second;

        if( !quest_exp_ask )
        {
            LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);

            return false;
        }


        if( !(ask_count >= 0 && ask_count <= 4) )
        {
            LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);

            return false;
        }

        int quest_type = quest_exp_ask->GetQuestType();

        switch ( quest_type )
        {
        case 0:
            user_quest_info->user_quest_ask_info_[ask_count].SetComplete(true);
            user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(0);
            user_quest_info->SetAskCnt(0);
            break;

        case 16:
            {
                QuestTutorial* quest_tutorial = 
                    static_cast<QuestTutorial*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_tutorial->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(16);
            }
            break;

        case 1:
            AddUserQuestAskMonsterKill(quest_exp_ask, obj_index, episode_number, ask_count);
            break;

        case 2:
            {
                QuestSkillLearn* quest_skill_learn = 
                    static_cast<QuestSkillLearn*>(quest_exp_ask);


                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_skill_learn->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);
                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(2);
            }
            break;

        case 4:
            {
                QuestGetItem* quest_get_item = 
                    static_cast<QuestGetItem*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_get_item->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);
                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(4);
            }
            break;

        case 8:
            {
                QuestLevelUp* quest_level_up = 
                    static_cast<QuestLevelUp*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_level_up->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);
                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(8);
            }

            break;

        case 32:
            {
                QuestBuff* quest_buff = 
                    static_cast<QuestBuff*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_buff->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);
                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(32);
            }

            break;

        case 64:
        case 65:
        case 66:
            {
                QuestEventMapKillPoint* quest_event_map_kill_point = 
                    static_cast<QuestEventMapKillPoint*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_event_map_kill_point->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);

                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(
                    quest_event_map_kill_point->GetQuestType());
            }

            break;

        case 256:
        case 257:
        case 258:
        case 259:
            {
                QuestEventMapClear* quest_event_map_clear = 
                    static_cast<QuestEventMapClear*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_event_map_clear->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);

                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(
                    quest_event_map_clear->GetQuestType());
            }

            break;

        case 260:
            {
                QuestEventMapDevilPoint* quest_event_map_devil_point = 
                    static_cast<QuestEventMapDevilPoint*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_event_map_devil_point->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);
                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(260);
            }

            break;

        case 261:
            {
                QuestNeedZen* quest_need_zen = 
                    static_cast<QuestNeedZen*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_need_zen->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);
                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(261);
            }
            break;

        case 262:
            {
                QuestPVP_Point* quest_pvp_point = 
                    static_cast<QuestPVP_Point*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_pvp_point->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);
                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(262);
            }

            break;

        case 263:
            {
                QuestNpcTalk* quest_npc_talk = 
                    static_cast<QuestNpcTalk*>(quest_exp_ask);

                user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                    quest_npc_talk->GetIndexID());

                user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);
                user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(263);
            }

            break;

        default:
            return false;
        }

		++AskIter;
        ++ask_count;
    }

    return true;
}

void QuestExpUserMng::AddUserQuestAskMonsterKill(QuestExpAsk* quest_exp_ask, 
                            int obj_index, int episode_number, int ask_count)
{
    QuestMonsterKill* quest_monster_kill = 
        static_cast<QuestMonsterKill*>(quest_exp_ask);

    UserQuestInfo* user_quest_info = &gObj[obj_index].m_pUserQuestInfo[episode_number];

    user_quest_info->user_quest_ask_info_[ask_count].SetIndexID(
                                            quest_monster_kill->GetIndexID());

    user_quest_info->user_quest_ask_info_[ask_count].SetComplete(false);
    user_quest_info->user_quest_ask_info_[ask_count].SetQuestType(1);
}

int QuestExpUserMng::GetQuestSwitch(int episode_number, int obj_index)
{
    if( mu::quest::IsEpisode(episode_number) )
    {
        return gObj[obj_index].m_pUserQuestInfo[episode_number].GetQuestSwitch();
    }
    else
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return -1;
    }
}

struct PMSG_QUESTEXP_INFO
{
    PWMSG_HEAD h;
    BYTE btQuestCnt;
    char szCharName[10+1]; 
};

#pragma pack(push, 1)
struct QUESTEXP_INFO
{
    DWORD dwQuestIndexID;
    WORD wProgState;
    BYTE btAskIndex[5];
    BYTE btAskValue[5];
    BYTE btAskState[5];
    int lStartDate;
    int lEndDate;

    QUESTEXP_INFO()
    {
        this->dwQuestIndexID = 0;
        this->wProgState = 0;
        this->lStartDate = 0;
        this->lEndDate = 0;

        memset(btAskIndex, 0, sizeof(btAskIndex));
        memset(btAskValue, 0, sizeof(btAskValue));
        memset(btAskState, 0, sizeof(btAskState));
    }

};
#pragma pack(pop)


void QuestExpUserMng::UserAllQuestInfoSave(int obj_index)
{
    BYTE send_buf[2048] = {0};

    int iIndex = obj_index;
    QUESTEXP_INFO quest_info[mu::quest::kMaxEpisodeCount];
    PMSG_QUESTEXP_INFO pMsg;
    int lOfs = sizeof(pMsg);

    strcpy(pMsg.szCharName, gObj[iIndex].Name);
    int episode_count = 1;
    int quest_count = 0;

    while( episode_count < mu::quest::kMaxEpisodeCount )
    {
        UserQuestInfo* user_quest_info = &gObj[iIndex].m_pUserQuestInfo[episode_count];
        int episode_number = user_quest_info->GetEpisode();
        int quest_switch = user_quest_info->GetQuestSwitch();

        if( episode_number > 0 )
        {
            quest_info[quest_count].dwQuestIndexID = mu::quest::GetQuestInfoIndexId(episode_number, quest_switch);
            quest_info[quest_count].wProgState = user_quest_info->GetQuestProgState();
            quest_info[quest_count].lStartDate = user_quest_info->GetStartDate();
            quest_info[quest_count].lEndDate = user_quest_info->GetEndDate();
            int ask_count = user_quest_info->GetAskCnt();

            if( ask_count == 0 )

            {
                LogAddTD("[QuestExp] AllQuestInfoSave [%s][%s] EP[%d] QS[%d]",
                    gObj[iIndex].AccountID,
                    gObj[iIndex].Name,
                    episode_number,
                    quest_switch);
            }

            for(int i = 0; i < ask_count; ++i )
            {
                quest_info[quest_count].btAskIndex[i] = user_quest_info->user_quest_ask_info_[i].GetIndexID();
                quest_info[quest_count].btAskValue[i] = user_quest_info->user_quest_ask_info_[i].GetValue();
                quest_info[quest_count].btAskState[i] = user_quest_info->user_quest_ask_info_[i].IsComplete();

                LogAddTD(
                    "[QuestExp] AllQuestInfoSave [%s][%s] EP[%d] QS[%d] AskIndex[0x%x] AskValue[%d] AskState[%d]",
                    gObj[iIndex].AccountID,
                    gObj[iIndex].Name,
                    episode_number,
                    quest_switch,
                    quest_info[quest_count].btAskIndex[i],
                    quest_info[quest_count].btAskValue[i],
                    quest_info[quest_count].btAskState[i]);
            }

            memcpy(&send_buf[lOfs], &quest_info[quest_count], sizeof(quest_info[0]));
            lOfs += sizeof(quest_info[0]);
            ++quest_count;
        }
        ++episode_count;
    }

    if( quest_count > 0 )
    {
        pMsg.btQuestCnt = quest_count;
        PHeadSetW((BYTE*)&pMsg, 0xF6, lOfs);
        memcpy(send_buf, &pMsg, sizeof(pMsg));
        cDBSMng.Send((PCHAR)send_buf, lOfs);
    }
}

void QuestExpUserMng::UserQuestInfoSave(DWORD quest_info_index_id, int obj_index)
{
    BYTE send_buf[2048] = {0};
    
    int iIndex = obj_index;
    QUESTEXP_INFO quest_info;
    PMSG_QUESTEXP_INFO pMsg;
    strcpy(pMsg.szCharName, gObj[iIndex].Name);

    int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id);
    int quest_switch = mu::quest::GetQuestSwitchFromInfoIndexId(quest_info_index_id);
    quest_info.dwQuestIndexID = quest_info_index_id;

    int ask_count = gObj[iIndex].m_pUserQuestInfo[episode_number].GetAskCnt();
    int prog_state = gObj[iIndex].m_pUserQuestInfo[episode_number].GetQuestProgState();

    quest_info.lStartDate = gObj[iIndex].m_pUserQuestInfo[episode_number].GetStartDate();
    quest_info.lEndDate = gObj[iIndex].m_pUserQuestInfo[episode_number].GetEndDate();
    quest_info.wProgState = prog_state;

    if( ask_count == 0 )
    {
        LogAddTD(
            "[QuestExp] QuestInfoSave [%s][%s] EP[%d] QS[%d]",
            gObj[iIndex].AccountID,
            gObj[iIndex].Name,
            episode_number,
            quest_switch);
    }

    for(int i = 0; i < ask_count; ++i )
    {
        quest_info.btAskIndex[i] = gObj[iIndex].m_pUserQuestInfo[episode_number].user_quest_ask_info_[i].GetIndexID();
        quest_info.btAskValue[i] = gObj[iIndex].m_pUserQuestInfo[episode_number].user_quest_ask_info_[i].GetValue();
        quest_info.btAskState[i] = gObj[iIndex].m_pUserQuestInfo[episode_number].user_quest_ask_info_[i].IsComplete();

        LogAddTD(
            "[QuestExp] QuestInfoSave [%s][%s] EP[%d] QS[%d] AskIndex[0x%x] AskValue[%d] AskState[%d]",
            gObj[iIndex].AccountID,
            gObj[iIndex].Name,
            episode_number,
            quest_switch,
            quest_info.btAskIndex[i],
            quest_info.btAskValue[i],
            quest_info.btAskState[i]);
    }

    pMsg.btQuestCnt = 1;
    memcpy(&send_buf[sizeof(pMsg)], &quest_info, sizeof(quest_info));

    PHeadSetW((BYTE*)&pMsg, 0xF6, sizeof(pMsg)+sizeof(quest_info));
    memcpy(send_buf, &pMsg, sizeof(pMsg));

    cDBSMng.Send((PCHAR)send_buf, sizeof(pMsg)+sizeof(quest_info));
}


#pragma pack(push, 1)
struct PMSG_REQ_QUESTEXP_INFO
{
    PBMSG_HEAD h;
    int iUserIndex;
    char szCharName[10+1];
};
#pragma pack(pop)

void QuestExpUserMng::DB_ReqUserQuestInfo(int obj_index)
{
    PMSG_REQ_QUESTEXP_INFO pMsg;

    pMsg.iUserIndex = obj_index;
    pMsg.szCharName[10] = 0;
    memcpy(pMsg.szCharName, gObj[obj_index].Name, 10);

    PHeadSetB((BYTE*)&pMsg, 0xF7, sizeof(pMsg));
    cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);
}


#pragma pack(push, 1)
struct PMSG_ANS_QUESTEXP_INFO
{
    PWMSG_HEAD head;
    BYTE btQuestCnt;
    int iUserIndex;
};
#pragma pack(pop)


void QuestExpUserMng::UserQuestInfoLoad(PMSG_ANS_QUESTEXP_INFO* lpRecv)
{
    int quest_count = lpRecv->btQuestCnt;
    int obj_index = lpRecv->iUserIndex;

    if( !OBJMAX_RANGE(obj_index) || !gObjIsConnected(obj_index) )
    {
        return;
    }

    if( !(quest_count >= 0 && quest_count <= 24) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }
           
    QUESTEXP_INFO quest_info[25];

    memcpy(quest_info, ((BYTE*)lpRecv)+sizeof(*lpRecv), quest_count * sizeof(quest_info[0]));

    for(int i = 0; i < quest_count; ++i )
    {
        DWORD quest_info_index_id = quest_info[i].dwQuestIndexID;
        int episode_number = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id);
        int quest_switch = mu::quest::GetQuestSwitchFromInfoIndexId(quest_info_index_id);
        int ask_count = 0;

        UserQuestInfo* user_quest_info = &gObj[obj_index].m_pUserQuestInfo[episode_number];
        user_quest_info->SetEpisode(episode_number);
        user_quest_info->SetQuestSwitch(quest_switch);

        int prog_state = quest_info[i].wProgState;
        user_quest_info->SetQuestProgState(prog_state);

        user_quest_info->SetStartDate(quest_info[i].lStartDate);
        user_quest_info->SetEndDate(quest_info[i].lEndDate);

        for(int j = 0; j < 5; ++j )
        {
            if( quest_info[i].btAskIndex[j] )
            {
                QuestExpAsk* quest_exp_ask = g_QuestExpManager.GetQuestExpAsk(
                                                episode_number,
                                                quest_switch,
                                                quest_info[i].btAskIndex[j]);

                if( !quest_exp_ask )
                {
                    LogAddTD(
                        "[QuestExp] UserQuestInfo DB Error [%s][%s] - pQuestExpAsk NULL",
                        gObj[obj_index].AccountID,
                        gObj[obj_index].Name);

                    return;
                }

                int quest_type = quest_exp_ask->GetQuestType();
                int ask_index = quest_info[i].btAskIndex[j];
                int ask_value = quest_info[i].btAskValue[j];
                int ask_state = (quest_info[i].btAskState[j] == 1);

                user_quest_info->user_quest_ask_info_[j].SetQuestType(quest_type);
                user_quest_info->user_quest_ask_info_[j].SetIndexID(ask_index);
                user_quest_info->user_quest_ask_info_[j].SetValue(ask_value);
                user_quest_info->user_quest_ask_info_[j].SetComplete(ask_state);

                LogAddTD(
                    "[QuestExp] Get DB UserQuestInfo [%s][%s] Ep[%d] QS[%d] QuestType[0x%x] AskIndex[%d] AskValue[%d] State[%d] ProgState[%d]",
                    gObj[obj_index].AccountID,
                    gObj[obj_index].Name,
                    episode_number,
                    quest_switch,
                    quest_type,
                    ask_index,
                    ask_value,
                    ask_state,
                    prog_state);


                ++ask_count;
            }
        }

        user_quest_info->SetAskCnt(ask_count);
    }

    if( !gObj[obj_index].m_bUserQuestInfoSent )
    {
        PBMSG_HEAD2 pMsg;

        gObj[obj_index].m_bUserQuestInfoSent = 1;

        PHeadSubSetB((BYTE*)&pMsg, 0xF6, 0x20, 4);
        DataSend(obj_index, (BYTE*)&pMsg, pMsg.size);
    }
}

UserQuestInfo* QuestExpUserMng::GetUserQuestInfo(int episode_number, int obj_index)
{
    if( mu::quest::IsEpisode(episode_number) )
    {
        return &gObj[obj_index].m_pUserQuestInfo[episode_number];
    }
    else
    {
        LogAddTD(
            "[QuestExp] - Error - GetUserQuestInfo [%s][%s] Ep[%d]",
            gObj[obj_index].AccountID,
            gObj[obj_index].Name,
            episode_number);

        return nullptr;
    }
}

// TODO: OVERLOADED unsigned long / int
int QuestExpUserMng::GetQuestProgState(DWORD quest_info_index_id, int obj_index)
{
    return GetQuestProgState(
                mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id), 
                obj_index);
}

int QuestExpUserMng::GetQuestProgState(int ep, int obj_index)
{
    if( gObj[obj_index].m_pUserQuestInfo[ep].GetQuestProgState() == 2 )
    {
        int end_date = gObj[obj_index].m_pUserQuestInfo[ep].GetEndDate();

        if( g_QuestExpProgMng.CheckExpireDate(end_date) == 1 )
            gObj[obj_index].m_pUserQuestInfo[ep].SetQuestProgState(0);
    }

    return gObj[obj_index].m_pUserQuestInfo[ep].GetQuestProgState();
}

bool QuestExpUserMng::IsQuestAccept(DWORD quest_info_index_id, int obj_index)
{
    int prog_count = 0;
    for(int i = 0; i < 25; ++i )
    {
        if( IsProgQuestInfo(i, obj_index) > 0 )
            ++prog_count;
    }

    if(prog_count >= 10 && IsProgQuestInfo(quest_info_index_id, obj_index) <= 0)
        return false;
    else
        return true;
}


bool QuestExpUserMng::InventoryExtentCheck(int x, int y, int w, int h)
{
    int end_of_range = 0;
    int start_of_range = 0;

    if( y < 0 )
        return 0;

    if( y < 8 )
    {
        start_of_range = 0;
        end_of_range = 8;
    }
    else if( y < 12 )
    {
        start_of_range = 8;
        end_of_range = 12;
    }
    else if( y < 16 )
    {
        start_of_range = 12;
        end_of_range = 16;
    }
    else if( y < 20 )
    {
        start_of_range = 16;
        end_of_range = 20;
    }
    else if( y < 24 )
    {
        start_of_range = 20;
        end_of_range = 24;
    }
    else if( y < 28 )
    {
        start_of_range = 24;
        end_of_range = 28;
    }
    else
    {
        return false;
    }

    if( end_of_range > h )
        end_of_range = h;

    return (x >= 0 && x < w && y >= start_of_range && y < end_of_range);
}


int QuestExpUserMng::CheckOutOfInventory(int aIndex, int sy, int height)
{
    int end_of_item_y = sy + height - 1;

	if( sy >= 4 * gObj[aIndex].ExpandedInventory + 8 )
        return -1;

    if( sy < 0 )
        return 0;

    if( sy < 8 )
    {
        if( end_of_item_y >= 8 )
            return 0;
    }
    else if( sy < 12 )
    {
        if ( end_of_item_y >= 12 )
            return 0;
    }
    else if( sy < 16 )
    {
        if( end_of_item_y >= 16 )
            return 0;
    }
    else if( sy < 20 )
    {
        if( end_of_item_y >= 20 )
            return 0;
    }
    else if( sy < 24 )
    {
        if( end_of_item_y >= 24 )
            return 0;
    }
    else
    {
        return 0;
    }

    return 1;
}