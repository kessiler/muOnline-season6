#include "StdAfx.h"
#include "QuestExpUserInfo.h"
#include "QuestExpInfo.h"

#include "logproc.h"

void UserQuestInfo::Init()
{
    this->ask_count_ = 0;
    this->episode_number_ = 0;
    this->quest_switch_ = 0;
    this->prog_state_ = 0;
    this->start_date_ = 0;
    this->end_date_ = 0;
}

void UserQuestInfo::Clear()
{
    this->ask_count_ = 0;
    this->episode_number_ = 0;
    this->quest_switch_ = 0;
    this->prog_state_ = 0;
    this->start_date_ = 0;
    this->end_date_ = 0;

    for(int i = 0; i < kMaxAskInfo; ++i )
        user_quest_ask_info_[i].Clear();
}

void UserQuestInfo::QuestAskInfoClear()
{
    this->ask_count_ = 0;
    this->prog_state_ = 0;
    this->start_date_ = 0;
    this->end_date_ = 0;

    for(int i = 0; i < kMaxAskInfo; ++i )
        user_quest_ask_info_[i].Clear();
}

bool UserQuestInfo::SetEpisode(int episode_number)
{
    if( mu::quest::IsEpisode(episode_number) )
    {
        this->episode_number_ = episode_number;
        return true;
    }
    else
    {
        LogAddTD("[QuestExp] - Error - (UserQuestInfo::SetEpisode) EP[%d]", episode_number);
        return false;
    }
}

int UserQuestInfo::GetEpisode()
{
    return this->episode_number_;
}

bool UserQuestInfo::SetQuestSwitch(int quest_switch)
{
    if( mu::quest::IsQuestSwitch(quest_switch) )
    {
        this->quest_switch_ = quest_switch;
        return true;;
    }
    else
    {
        LogAddTD("[QuestExp] - Error - (UserQuestInfo::SetQuestSwitch) QS[%d]", quest_switch);
        return false;;
    }
}

int UserQuestInfo::GetQuestSwitch()
{
    return this->quest_switch_;
}

void UserQuestInfo::SetAskCnt(int ask_count)
{
    if ( ask_count >= 0 && ask_count <= kMaxAskInfo)
    {
        this->ask_count_ = ask_count;
    }
    else
    {
        LogAdd("[QuestExp] - Error - iAsk [%d] [%s] Line:[%d]",
                ask_count, __FILE__, __LINE__);
    }
}

int UserQuestInfo::GetAskCnt()
{
    if( this->ask_count_ >= 0 && this->ask_count_ <= kMaxAskInfo )
    {
        return this->ask_count_;
    }
    else
    {
        LogAdd("[QuestExp] - Error - m_iAskCnt [%d] [%s] Line:[%d]",
            this->ask_count_, __FILE__, __LINE__);

        return 0;
    }
}

void UserQuestInfo::SetStartDate(int start_date)
{
    this->start_date_ = start_date;
}

int UserQuestInfo::GetStartDate()
{
    return this->start_date_;
}

void UserQuestInfo::SetEndDate(int end_date)
{
    this->end_date_ = end_date;
}

int UserQuestInfo::GetEndDate()
{
    return this->end_date_;
}

void UserQuestInfo::SetQuestProgState(int prog_state)
{
    this->prog_state_ = prog_state;
}

int UserQuestInfo::GetQuestProgState()
{
    return this->prog_state_;
}