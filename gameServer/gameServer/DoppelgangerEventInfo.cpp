// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "DoppelgangerEventInfo.h"

#include "logproc.h"
//#include "dirpath.h"
#include "..\common\ReadScript.h"
#include "GameMain.h"

CDoppelgangerEventInfo::CDoppelgangerEventInfo(void)
{
}


CDoppelgangerEventInfo::~CDoppelgangerEventInfo(void)
{
}

void CDoppelgangerEventInfo::LoadDopplegangereventinfo()
{
    char script_file[MAX_PATH+1] = {0};

//    strcpy(script_file, gDirPath.GetNewPath("DoppelgangerEventInfo.txt"));
    strcpy(script_file, "..\\data\\Event\\DoppelgangerEventInfo.dat" );
    this->loaded_ = false;

    SMDFile = fopen(script_file, "r");

    if ( !SMDFile )
    {
        LogAdd(lMsg.Get(453), script_file);
        return;
    }

    while ( 1 )
    {
        SMDToken token = GetToken();
        if ( token == SMDToken::END )
            break;

        if ( token == SMDToken::NUMBER )
        {
            DOPPELGANGER_EVENT_INFO event_info;

			int average_level = TokenNumber;

            token = GetToken();
            event_info.monster_levelweight[0] = TokenNumber;
            token = GetToken();
            event_info.monster_levelweight[1] = TokenNumber;
            token = GetToken();
            event_info.monster_levelweight[2] = TokenNumber;
            token = GetToken();
            event_info.monster_levelweight[3] = TokenNumber;
            token = GetToken();
            event_info.monster_levelweight[4] = TokenNumber;

            token = GetToken();
            event_info.monster_hpweight[0] = TokenNumber;
            token = GetToken();
            event_info.monster_hpweight[1] = TokenNumber;
            token = GetToken();
            event_info.monster_hpweight[2] = TokenNumber;
            token = GetToken();
            event_info.monster_hpweight[3] = TokenNumber;
            token = GetToken();
            event_info.monster_hpweight[4] = TokenNumber;

            token = GetToken();
            event_info.monster_attackweight[0] = TokenNumber;
            token = GetToken();
            event_info.monster_attackweight[1] = TokenNumber;
            token = GetToken();
            event_info.monster_attackweight[2] = TokenNumber;
            token = GetToken();
            event_info.monster_attackweight[3] = TokenNumber;
            token = GetToken();
            event_info.monster_attackweight[4] = TokenNumber;

            token = GetToken();
            event_info.monster_defenseweight[0] = TokenNumber;
            token = GetToken();
            event_info.monster_defenseweight[1] = TokenNumber;
            token = GetToken();
            event_info.monster_defenseweight[2] = TokenNumber;
            token = GetToken();
            event_info.monster_defenseweight[3] = TokenNumber;
            token = GetToken();
            event_info.monster_defenseweight[4] = TokenNumber;

            token = GetToken();
            event_info.middleitem_dropcount = TokenNumber;

            token = GetToken();
            event_info.middleitem_bagindex[0] = TokenNumber;
            token = GetToken();
            event_info.middleitem_droprate[0] = TokenNumber;
            token = GetToken();
            event_info.middleitem_bagindex[1] = TokenNumber;
            token = GetToken();
            event_info.middleitem_droprate[1] = TokenNumber;
            token = GetToken();
            event_info.middleitem_bagindex[2] = TokenNumber;
            token = GetToken();
            event_info.middleitem_droprate[2] = TokenNumber;

            token = GetToken();
            event_info.lastitem_dropcount = TokenNumber;
            token = GetToken();
            event_info.lastitem_bagindex[0] = TokenNumber;
            token = GetToken();
            event_info.lastitem_droprate[0] = TokenNumber;
            token = GetToken();
            event_info.lastitem_bagindex[1] = TokenNumber;
            token = GetToken();
            event_info.lastitem_droprate[1] = TokenNumber;
            token = GetToken();
            event_info.lastitem_bagindex[2] = TokenNumber;
            token = GetToken();
            event_info.lastitem_droprate[2] = TokenNumber;
            token = GetToken();
            event_info.lastitem_bagindex[3] = TokenNumber;
            token = GetToken();
            event_info.lastitem_droprate[3] = TokenNumber;
            token = GetToken();
            event_info.lastitem_bagindex[4] = TokenNumber;
            token = GetToken();
            event_info.lastitem_droprate[4] = TokenNumber;
            token = GetToken();
            event_info.lastitem_bagindex[5] = TokenNumber;
            token = GetToken();
            event_info.lastitem_droprate[5] = TokenNumber;

			events_info_.insert(std::pair<int, DOPPELGANGER_EVENT_INFO>(average_level, event_info));
        }
    }

    fclose(SMDFile);

    LogAdd("Load Doppelganger EventInfo : %s", script_file);
    this->loaded_ = true;
}

void CDoppelgangerEventInfo::ClearDoppelgangerEventInfo()
{
    events_info_.clear();
}

DOPPELGANGER_EVENT_INFO* CDoppelgangerEventInfo::GetDoppelgangerEventInfo(int user_level)
{
    int max_level = ((user_level - 1) / 10) * 10 + 10;

    auto it = events_info_.find(max_level);

    if(it == events_info_.end())
        return nullptr;
    else
        return &it->second;
}

bool CDoppelgangerEventInfo::SetDoppelgangerInfo(int user_count, int user_level)
{
    user_count_ = user_count;

    DOPPELGANGER_EVENT_INFO* event_info = GetDoppelgangerEventInfo(user_level);
    if ( event_info )
    {
        current_event_info_ = *event_info;
        return true;    
    }
    else
    {
        return false;
    }
}

float CDoppelgangerEventInfo::GetMonsterLevelWeight()
{
    return current_event_info_.monster_levelweight[user_count_ - 1];
}

float CDoppelgangerEventInfo::GetMonsterHpWeight()
{
    return current_event_info_.monster_hpweight[user_count_ - 1];
}

float CDoppelgangerEventInfo::GetMonsterAttackWeight()
{
    return current_event_info_.monster_attackweight[user_count_ = -1];
}

float CDoppelgangerEventInfo::GetMonsterDefenseWeight()
{
    return current_event_info_.monster_defenseweight[user_count_ - 1];
}

int CDoppelgangerEventInfo::GetMiddleItemDropCnt()
{
    return current_event_info_.middleitem_dropcount;
}

int CDoppelgangerEventInfo::GetMiddleItemBagIndex(int reward_index)
{
    return current_event_info_.middleitem_bagindex[reward_index];
}

int CDoppelgangerEventInfo::GetMiddleItemDropRate(int reward_index)
{
    return current_event_info_.middleitem_droprate[reward_index];
}

int CDoppelgangerEventInfo::GetLastItemDropCnt()
{
    return current_event_info_.lastitem_dropcount;
}

int CDoppelgangerEventInfo::GetLastItemBagIndex(int reward_index)
{
    return current_event_info_.lastitem_bagindex[reward_index];
}

int CDoppelgangerEventInfo::GetLastItemDropRate(int reward_index)
{
    return current_event_info_.lastitem_droprate[reward_index];
}