// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#pragma once
#include "PartyClass.h"

struct DOPPELGANGER_EVENT_INFO
{
    float monster_levelweight[MAX_PARTYUSER];
    float monster_hpweight[MAX_PARTYUSER];
    float monster_attackweight[MAX_PARTYUSER];
    float monster_defenseweight[MAX_PARTYUSER];
    int ukn50;
    int middleitem_dropcount;
    int middleitem_bagindex[3];
    int middleitem_droprate[3];
    int lastitem_dropcount;
    int lastitem_bagindex[6];
    int lastitem_droprate[6];
};


class CDoppelgangerEventInfo
{
public:
    CDoppelgangerEventInfo(void);
    virtual ~CDoppelgangerEventInfo(void);
    
    void LoadDopplegangereventinfo();
    void ClearDoppelgangerEventInfo();
    DOPPELGANGER_EVENT_INFO *GetDoppelgangerEventInfo(int user_level);
    bool SetDoppelgangerInfo(int user_count, int user_level);
    float GetMonsterLevelWeight();
    float GetMonsterHpWeight();
    float GetMonsterAttackWeight();
    float GetMonsterDefenseWeight();
    int GetMiddleItemDropCnt();
    int GetMiddleItemBagIndex(int reward_index);
    int GetMiddleItemDropRate(int reward_index);
    int GetLastItemDropCnt();
    int GetLastItemBagIndex(int reward_index);
    int GetLastItemDropRate(int reward_index);

private:
    bool loaded_;
    std::map<int, DOPPELGANGER_EVENT_INFO> events_info_;
    DOPPELGANGER_EVENT_INFO current_event_info_;
    int user_count_;
};

