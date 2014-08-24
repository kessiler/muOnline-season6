#include "StdAfx.h"
#include "QuestExpInfo.h"

#include "logproc.h"

QuestExpInfo::QuestExpInfo():
quest_episode_(0),
quest_switch_(0),
ask_count_(0),
reward_count_(0)
{

}

QuestExpInfo::~QuestExpInfo()
{
//    Release();
}


QuestExpInfo::QuestExpInfo(const QuestExpInfo& rhs)
{
    if(this == &rhs)
        return;

    this->ask_count_ = rhs.ask_count_;
    this->quest_episode_ = rhs.quest_episode_;
    this->quest_switch_ = rhs.quest_switch_;
	this->reward_count_ = rhs.reward_count_;
    this->map_quest_ask_ = rhs.map_quest_ask_;
    this->map_quest_reward_ = rhs.map_quest_reward_;
}

QuestExpInfo& QuestExpInfo::operator=(const QuestExpInfo& rhs)
{
    if(this == &rhs)
        return *this;

    this->ask_count_ = rhs.ask_count_;
    this->quest_episode_ = rhs.quest_episode_;
    this->quest_switch_ = rhs.quest_switch_;
	this->reward_count_ = rhs.reward_count_;
    this->map_quest_ask_ = rhs.map_quest_ask_;
    this->map_quest_reward_ = rhs.map_quest_reward_;

    return *this;
}


void QuestExpInfo::SetQuestExpAsk(QuestExpAsk *quest_exp_ask)
{
    int index_id = quest_exp_ask->GetIndexID();

    if( CheckQuestExpAskIndex(index_id) )
    {
        map_quest_ask_[index_id] = quest_exp_ask;
    }
    else
    {
        MsgBox("[QuestExp] - Error - AskIndex Overflow iIndexID [%d] [%s] [%d]",
               index_id, __FILE__, __LINE__);
    }
}

void QuestExpInfo::SetQuestExpAsk(QuestExpAsk *quest_exp_ask, int index_id)
{
    if( CheckQuestExpAskIndex(index_id) )
    {
        map_quest_ask_[index_id] = quest_exp_ask;
        quest_exp_ask->SetIndexID(index_id);
    }
    else
    {
        MsgBox("[QuestExp] - Error - AskIndex Overflow iIndexID [%d] [%s] Line:[%d]",
            index_id, __FILE__, __LINE__);
    }
}

std::map<int, QuestExpAsk*> QuestExpInfo::GetQuestExpAsk()
{
    return map_quest_ask_;
}

void QuestExpInfo::SetQuestReward(QuestExpReward *quest_exp_reward)
{
    if ( CheckQuestExpRewardIndex(reward_count_) )
    {
        map_quest_reward_[reward_count_] = quest_exp_reward;
        quest_exp_reward->SetIndexID(reward_count_++);
    }
    else
    {
        MsgBox("[QuestExp] - Error - RewardIndex Overflow m_iRewardID [%d] [%s] Line:[%d]",
            reward_count_, __FILE__, __LINE__);
    }
}

std::map<int, QuestExpReward*> QuestExpInfo::GetQuestReward()
{
    return map_quest_reward_;
}

void QuestExpInfo::Release()
{
    auto ask_it = map_quest_ask_.begin();
    auto ask_end = map_quest_ask_.end();

    for(; ask_it != ask_end; ++ask_it)
    {
        delete ask_it->second;
        ask_it->second = nullptr;
    }


    auto reward_it = map_quest_reward_.begin();
    auto reward_end = map_quest_reward_.end();

    for(; reward_it != reward_end; ++reward_it)
    {
        delete reward_it->second;
        reward_it->second = nullptr;
    }
}

QuestExpManager g_QuestExpManager;

QuestExpManager::QuestExpManager()
{

}

QuestExpManager::~QuestExpManager()
{
    auto kind_it = map_quest_exp_reward_kind_.begin();
    auto kind_end = map_quest_exp_reward_kind_.end();

    for(; kind_it != kind_end; ++kind_it)
    {
        delete kind_it->second;
        kind_it->second = nullptr;
    }
}

void QuestExpManager::QuestExpItemInit()
{
    auto vt_it = vt_quest_item_info_.begin();
    auto vt_end = vt_quest_item_info_.end();

    for(; vt_it != vt_end; ++vt_it)
    {
        ItemAttribute[vt_it->item_number].QuestItem = true;
    }
}

void QuestExpManager::QuestInfoReload()
{
    auto map_it = map_quest_exp_info_.begin();
    auto map_end = map_quest_exp_info_.end();

    for(; map_it != map_end; ++map_it)
    {
        map_it->second.Release();
    }
}

void QuestExpManager::SetQuestExpInfo(QuestExpAsk* quest_exp_ask, 
                                        int episode_number, int quest_switch)
{

    QuestExpAsk* quest_ask = nullptr;
    int quest_type = quest_exp_ask->GetQuestType();

	if( quest_type > 263)
	{
		__asm int 3
	}

    switch ( quest_type )
    {
        case 0:
            quest_ask = new QuestProgress(
                *static_cast<QuestProgress*>(quest_exp_ask));

            break;

        case 1:
            quest_ask = new QuestMonsterKill(
                *static_cast<QuestMonsterKill*>(quest_exp_ask));

            break;

        case 16:
            quest_ask = new QuestTutorial(
                *static_cast<QuestTutorial*>(quest_exp_ask));

            break;


        case 4:
            quest_ask = new QuestGetItem(
                *static_cast<QuestGetItem*>(quest_exp_ask));
            break;


        case 2:
            quest_ask = new QuestSkillLearn(
                *static_cast<QuestSkillLearn*>(quest_exp_ask));

            break;


        case 8:
            quest_ask = new QuestLevelUp(
                *static_cast<QuestLevelUp*>(quest_exp_ask));

            break;


        case 32:
            quest_ask = new QuestBuff(
                *static_cast<QuestBuff*>(quest_exp_ask));

            break;


        case 64:
        case 65:
        case 66:
            quest_ask = new QuestEventMapKillPoint(
                *static_cast<QuestEventMapKillPoint*>(quest_exp_ask));

            break;


        case 256:
        case 257:
        case 258:
        case 259:
            quest_ask = new QuestEventMapClear(
                *static_cast<QuestEventMapClear*>(quest_exp_ask));

            break;


        case 260:
            quest_ask = new QuestEventMapDevilPoint(
                *static_cast<QuestEventMapDevilPoint*>(quest_exp_ask));

            break;

        case 261:
            quest_ask = new QuestNeedZen(
                *static_cast<QuestNeedZen*>(quest_exp_ask));

            break;


        case 262:
            quest_ask = new QuestPVP_Point(
                *static_cast<QuestPVP_Point*>(quest_exp_ask));

            break;


        case 263:
            quest_ask = new QuestNpcTalk(
                *static_cast<QuestNpcTalk*>(quest_exp_ask));
            break;

        default:
            break;
    }


    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode_number, 
                                                                quest_switch);

    QuestExpInfo* quest_exp_info = GetQuestExpInfo(quest_info_index_id);

    if( quest_exp_info )
        quest_exp_info->SetQuestExpAsk(quest_ask);
    else
        AddQuestExpInfoList(quest_ask, quest_info_index_id);
}


void QuestExpManager::AddQuestExpInfoList(QuestExpAsk* quest_exp_ask, 
                                                DWORD quest_info_index_id)
{
    QuestExpInfo quest_exp_info;

    quest_exp_info.SetQuestExpAsk(quest_exp_ask);
    AddQuestExpInfoList(&quest_exp_info, quest_info_index_id);
}


void QuestExpManager::SetQuestExpInfo(QuestExpReward* quest_exp_reward, 
                                           int episode_number, int quest_switch)
{

    QuestExpReward* quest_reward = nullptr;

    switch( quest_exp_reward->GetRewardType() )
    {
        case 1:
            quest_reward = new RewardExp(
                                    *static_cast<RewardExp*>(quest_exp_reward));
            break;


        case 2:
            quest_reward = new RewardZen(
                                        *static_cast<RewardZen*>(quest_exp_reward));
            break;


        case 4:
            quest_reward = new RewardItem(
                                       *static_cast<RewardItem*>(quest_exp_reward));
            break;


        case 16:

            quest_reward = new RewardContribute(
                                *static_cast<RewardContribute*>(quest_exp_reward));
            break;

        default:
            break;
    }


    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode_number, 
                                                                  quest_switch);


    QuestExpInfo* quest_exp_info = GetQuestExpInfo(quest_info_index_id);

    if( quest_exp_info )
        quest_exp_info->SetQuestReward(quest_reward);
    else
        AddQuestExpInfoList(quest_reward, quest_info_index_id);
}

void QuestExpManager::AddQuestExpInfoList(QuestExpReward *quest_exp_reward, 
                                                        DWORD quest_info_index_id)
{
    QuestExpInfo quest_exp_info;

    quest_exp_info.SetQuestReward(quest_exp_reward);
    AddQuestExpInfoList(&quest_exp_info, quest_info_index_id);
}

void QuestExpManager::AddQuestExpInfoList(QuestExpInfo* quest_exp_info, 
                                                        DWORD quest_info_index_id)
{
    int quest_episode = mu::quest::GetQuestEpisodeFromInfoIndexId(quest_info_index_id);
    int quest_switch = mu::quest::GetQuestSwitchFromInfoIndexId(quest_info_index_id);

    quest_exp_info->SetQuestEpisode(quest_episode);
    quest_exp_info->SetQuestSwitch(quest_switch);

    map_quest_exp_info_[quest_info_index_id] = *quest_exp_info;
}


void QuestExpManager::AddQuestExpRewardKind(QuestExpRewardKind* quest_exp_reward_kind, 
                                                    DWORD quest_info_index_id)
{
    if( quest_exp_reward_kind )
    {
        QuestExpRewardKind* quest_reward_kind = nullptr;

        if ( quest_exp_reward_kind->GetRewardKind() == 1 )
        {
            quest_reward_kind = new QuestExpRewardKindRandom(
                *static_cast<QuestExpRewardKindRandom*>(quest_exp_reward_kind));
        }

        map_quest_exp_reward_kind_[quest_info_index_id] = quest_reward_kind;
    }
    else
    {
        MsgBox("[QuestExp] - Error -  AddQuestExpRewardKind dwQuestInfoIndexID 0x%08x", 
            quest_info_index_id);
    }
}

void QuestExpManager::AddQuestExpRewardKind(QuestExpRewardKind* quest_exp_reward_kind)
{
    if( quest_exp_reward_kind )
    {
        int episode_number = quest_exp_reward_kind->GetQuestEpisode();
        int quest_switch = quest_exp_reward_kind->GetQuestSwitch();

        DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode_number, 
                                                                  quest_switch);

        AddQuestExpRewardKind(quest_exp_reward_kind, quest_info_index_id);
    }
    else
    {
        MsgBox("[QuestExp] - Error -  AddQuestExpRewardKind");
    }
}

QuestExpRewardKind* QuestExpManager::GetQuestExpRewardKind(DWORD quest_info_index_id)
{
    auto map_it = map_quest_exp_reward_kind_.find(quest_info_index_id);
    auto map_end = map_quest_exp_reward_kind_.end();

    if(map_it == map_end)
        return nullptr;
    else
        return map_it->second;
}

QuestExpAsk* QuestExpManager::GetQuestExpAsk(int episode_number, 
                                                int quest_switch, int index_id)
{
    QuestExpInfo* quest_exp_info = GetQuestExpInfo(episode_number, quest_switch);

    if( quest_exp_info )
    {

        auto map_quest_exp_ask = quest_exp_info->GetQuestExpAsk();

        auto map_it = map_quest_exp_ask.find(index_id);
        auto map_end = map_quest_exp_ask.end();

        if(map_it == map_end)
            return nullptr;
        else
            return map_it->second;
    }
    else
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return nullptr;
    }
}


QuestExpAsk* QuestExpManager::GetQuestExpAsk(DWORD quest_info_index_id, int index_id)
{
    QuestExpInfo* quest_exp_info = GetQuestExpInfo(quest_info_index_id);

    if( quest_exp_info )
    {
        auto map_quest_exp_ask = quest_exp_info->GetQuestExpAsk();
        auto map_it = map_quest_exp_ask.find(index_id);
        auto map_end = map_quest_exp_ask.end();

        if(map_it == map_end)
            return nullptr;
        else
            return map_it->second;
    }
    else
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return nullptr;
    }
}

QuestExpReward* QuestExpManager::GetQuestExpReward(int episode_number, 
                                                int quest_switch, char index_id)
{

    QuestExpInfo* quest_exp_info = GetQuestExpInfo(episode_number, quest_switch);

    if( quest_exp_info )
    {
        auto map_quest_reward = quest_exp_info->GetQuestReward();

        auto map_it = map_quest_reward.find(index_id);
        auto map_end = map_quest_reward.end();

        if(map_it == map_end)
            return nullptr;
        else
            return map_it->second;
    }
    else
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return nullptr;
    }
}

QuestExpInfo* QuestExpManager::GetQuestExpInfo(DWORD quest_info_index_id)
{
    auto map_it = map_quest_exp_info_.find(quest_info_index_id);
    auto map_end = map_quest_exp_info_.end();

    if(map_it == map_end)
        return nullptr;
    else
        return &map_it->second;
}

QuestExpInfo* QuestExpManager::GetQuestExpInfo(int episode_number, int quest_switch)
{

    DWORD quest_info_index_id = mu::quest::GetQuestInfoIndexId(episode_number, 
                                                                 quest_switch);

    auto map_it = map_quest_exp_info_.find(quest_info_index_id);
    auto map_end = map_quest_exp_info_.end();

    if(map_it == map_end)
        return nullptr;
    else
        return &map_it->second;
}

void QuestExpManager::AddQuestDropItemInfo(QuestDropItemInfo* quest_drop_item_info, 
                                                        DWORD quest_info_index_id)
{
    map_quest_drop_item_info_[quest_info_index_id] = *quest_drop_item_info;
}

QuestDropItemInfo* QuestExpManager::GetQuestDropItemInfo(DWORD quest_info_index_id)
{
    auto map_it = map_quest_drop_item_info_.find(quest_info_index_id);
    auto map_end = map_quest_drop_item_info_.end();

    if(map_it == map_end)
        return nullptr;
    else
        return &map_it->second;
}

// better change to const reference
void QuestExpManager::AddQuestItemInfo(QUEST_ITEM_INFO quest_item_info)
{
    vt_quest_item_info_.push_back(quest_item_info);
}

bool QuestExpManager::IsQuestItemInfo(int item_number)
{
    auto vt_it = vt_quest_item_info_.begin();
    auto vt_end = vt_quest_item_info_.end();

    for(; vt_it != vt_end; ++vt_it)
    {
        if( vt_it->item_number == item_number )
            return true;
    }

    return false;
}

bool QuestExpManager::IsQuestItemAtt(int item_number, int quest_item_att)
{
    auto vt_it = vt_quest_item_info_.begin();
    auto vt_end = vt_quest_item_info_.end();

    for(; vt_it != vt_end; ++vt_it)
    {
        if( vt_it->item_number == item_number 
            && vt_it->item_attribute == quest_item_att)
        {
            return true;
        }
    }

    return false;
}

int QuestExpManager::GetQuestItemEp(int item_number)
{
    auto vt_it = vt_quest_item_info_.begin();
    auto vt_end = vt_quest_item_info_.end();

    for(; vt_it != vt_end; ++vt_it)
    {
        if( vt_it->item_number == item_number)
        {
            vt_it->episode_number;
        }
    }

    return 0;
}