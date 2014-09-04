// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#pragma once

#include "MonsterHerd.h"
#include "MuLua.h"
#include "DoppelgangerPosInfo.h"
#include "DoppelgangerEventInfo.h"
#include "DoppelgangerItemBag.h"
#include "MonsterAttr.h"
#include "PartyClass.h"

class CDoppelgangerMonsterHerd : public MonsterHerd
{
public:
    CDoppelgangerMonsterHerd();
	virtual ~CDoppelgangerMonsterHerd(){};

    virtual int Start() { return MonsterHerd::Start(); };
    virtual int MonsterHerdItemDrop(LPOBJ lpObj) { return 0; };
    virtual void MonsterAttackAction(LPOBJ lpObj, LPOBJ lpTargetObj);

    int AddMonsterEX(int monster_type, int should_attack_first, 
                    int monster_level, int monster_hp, 
                    int attack_min, int attack_max, int monster_def);

    virtual int GetRandomLocation(BYTE & pos_x, BYTE & pos_y);
    virtual int SetTotalInfo(int map_number, int radius, int pos_info, int should_move);
    virtual void MonsterBaseAct(OBJECTSTRUCT *lpObj);
    virtual int MoveHerd();
    void ArrangeMonsterHerd();
    int IsActive();
    void SetActive(bool is_active);


    int GetMonsterSize() const { return m_mapMonsterHerd.size(); }
    void SetEndPosition(int pos_info) { this->end_pos_index_ = pos_info; }

    int GetPosIndex() const { return this->pos_index_; }
    int GetEndPosIndex() const { return this->end_pos_index_; }
    int IsMove() const { return this->should_move_; }
    void SetPosIndex(int pos_index);
private:
    int align0;
    int pos_index_;
    int ukn50;
    int end_pos_index_;
    int should_move_;
};

struct DOPPELGANGER_USERINFO
{
    int object_index;
    int user_level;

    DOPPELGANGER_USERINFO()
    {
        object_index = -1;
        user_level = 0;
    }

    bool IsUser()
    {
        return OBJMAX_RANGE(object_index);
    }
    
};


class CDoppelganger
{
public:
    CDoppelganger(void);
    ~CDoppelganger(void);

    enum { kMaxMiddleTreasureBox = 3 };
    enum { kMaxMiddleBoss = 4 };
    enum { kMaxIceWorker = 5 };
    enum { kMaxLarva = 15 };
    enum { kMaxMonsterHard = 200 };


    static int IsEventMap(int map) { return (map >= 65 && map <= 68); }


    void DoppelgangerInfoLoad();
    void DoppelgangerProcessInit();

    void SetDoppelgangerState(BYTE state);
    BYTE GetDoppelgangerState();

    void SetDoppelgangerStateNone();
    void SetDoppelgangerStateReady();
    void SetDoppelgangerStatePlaying();
    void SetDoppelgangerStateEnd();
    void SetPosIndex(int pos_index);

    void ProcDoppelganger(int current_time);
    void ProcDoppelgangerState_None(int current_time);
    void ProcDoppelgangerState_Ready(int current_time);
    void ProcDoppelgangerState_Playing(int current_time);
    void ProcDoppelgangerState_End(int current_time);

    int PickupMarkOfDimensionItem(OBJECTSTRUCT *lpObj, CMapItem *lpItem, 
                                    int item_num);

    bool EnterDoppelgangerEvent(int user_index, BYTE item_pos);
    bool LeaveDoppelgangerEvent(int aIndex);
    bool AddDoppelgangerUser(int user_index);
    void DelDoppelgangerUser(int user_index);


    void CalUserLevel();
    void ClearUserData();
    int GetUserMaxLevel();
    int GetUserMinLevel();
    int GetUserAverageLevel();
    void SetUserAverageLevel();
    int GetUserCount();
    int GetStartUserCount();
    void SendNoticeMessage(char* message);
    void SendDoppelgangerState(char state);
    void PlatformLugardAct(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj);
    void MiddleTreasureAct(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj);
    void LastTreasureAct(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj);
    bool OpenTreasureBox();
    bool OpenLastTreasureBox();

    int GetRandomValue(int range);
    int GetMonseterHerdIndex();
    void IncMonseterHerdIndex();
    void SetHerdStartPosInfo(int herd_index, int pos_info, int should_move);
    void SetHerdEndPosInfo(int herd_index, int pos_info);

    void AddMonsterHerd(int herd_index, int monster_class, 
                        int should_attack_first);

    BYTE GetKillerState();
    int GetKillerHp();
    void SetKillerHp(int killer_hp);
	void SetKillerState(BYTE state) { this->killer_state_ = state; }

    BYTE GetAngerKillerState();
    int GetAngerKillerHp();
    void SetAngerKillerHp(int anger_killer_hp);
    void SetAngerKillerState(BYTE state) { this->anger_killer_state_ = state; }

    void AddMonsterBoss(int herd_index, int monster_class, 
                        int should_attack_first);

    void AddMonsterNormal(int monster_class, int pos_x, int pos_y);
    void SetIceWorkerRegen(BYTE pos_info);

    void AddIceWorkerIndex(int monster_index);
    void DelIceWorkerIndex(int monster_index);
    bool CheckIceWorker();
    void AddLarvaIndex(int monster_index);
    void DelLarvaAll();
    void MonsterHerdStart(int herd_index);
    void CheckDoppelgangerMosterPos(OBJECTSTRUCT *lpObj);
    int GetDoppelgangerPosIndex(BYTE pos_x, BYTE pos_y);

    void SendMonsterGoalCount();
    void SendDoppelgangerMonserPos();
    void SendDoppelgangerUserPos();

    void SelfExplosion(int index, CMagicInf* magic, int target_index);
    void AngerKillerAttack(OBJECTSTRUCT *lpObj);
    void ArrangeMonsterHerd();

    void MoveDoppelgangerMonsterProc();

    bool GetRandomLocation(BYTE& pos_x, BYTE& pos_y);
    bool GetRandomLocation(BYTE& pos_x, BYTE& pos_y, signed int seed);
    bool AddMiddleTreasureBoxAll(BYTE pos_x, BYTE pos_y);
    int AddMiddleTreasureBox(int map_number, BYTE pos_x, BYTE pos_y);
    bool AddLastTreasureBox(BYTE pos_x, BYTE pos_y);

    int AddMonsterLarva(int map_number, BYTE pos_x, BYTE pos_y, 
                        int monster_level, int monster_hp, 
                        int monster_attack_min, int monster_attack_max, 
                        int monster_def);

    int CheckMapTile(int map_number, BYTE pos_x, BYTE pos_y);

    void SendMapTileInfo(OBJECTSTRUCT *lpObj, char map_set_type);
    void SendMapTileInfoAll(BYTE map_set_type);

    MONSTER_ATTRIBUTE* GetDefaultMonsterInfo(int monster_class);
    int GetDefaultMonsterLevel(int monster_class);
    int GetDefaultMonsterHp(int monster_class);
    int GetDefaultMonsterAttackMax(int monster_class);
    int GetDefaultMonsterAttackMin(int monster_class);
    int GetDefaultMonsterDefense(int monster_class);
    float GetMonsterLevelWeight();
    float GetMonsterHpWeight();
    float GetMonsterAttackWeight();
    float GetMonsterDefenseWeight();
    int GetMissionResult();
    void SetMiddleBossIndex(int monster_class, int monster_index);
    void SetMiddleBossKill(int monster_class, int monster_index);
    int GetMiddleKillBossKill(int number);
    void SendDoppelgangerTimerMsg(int message);
    void SaveDoppelgangerTmpInfo();
    int ChangeUserIndex(int ex_user_index, int current_user_index);
    void SendDoppelgangerStateToSpecificUser(int user_index, char state);
    int GetPlayUserCountRightNow();

    void GetCenterPosition(int map_number, int pos_index, 
                            BYTE &pos_x, BYTE &pos_y)
    {
        return pos_info_.GetCenterPosition(map_number, pos_index, pos_x, pos_y);
    }

    void DelTreasureBox(int object_index, int monster_class);


    void SetReadyTime(int time) { this->ready_time_ = time; }
    void SetPlayTime(int time) { this->play_time_ = time; }
    void SetEndTime(int end_time) { this->end_time_ = end_time; }
    int GetStateTime() const { return this->state_start_tick_; };
    int GetMapNumber() const { return this->map_number_; }
    void SetAddHerdMonsterTime(int time) { this->add_herd_monster_time_ = time; }
    int GetAddHerdMonsterTime() const { return this->add_herd_monster_time_; }
    void SetAddBossMonsterTime(int time) { this->add_boss_monster_time_ = time; }
    int GetAddBossMonsterTime() const { return this->add_boss_monster_time_; }
    int GetBossRegenOrder() const { return this->boos_regen_order_; }
    void SetBossRegenOrder(int regen_order){ this->boos_regen_order_ = regen_order; }
    void SendDoppelgangerResultAll();
    void SendDoppelgangerResult(OBJECTSTRUCT *lpObj, BYTE result);

private:

    CDoppelgangerPosInfo pos_info_;
    CDoppelgangerEventInfo event_info_;
    CDoppelgangerItemBag event_item_bag_;
    MONSTER_ATTRIBUTE* monster_attribute_[11];
    MULua mu_lua_;
    BYTE current_state_;
    int ukn2c4;
    int ukn2c8;
    int ukn2cc;
    int ukn2d0;
    int ukn2d4;
    int ready_time_;
    int play_time_;
    int end_time_;
    CRITICAL_SECTION user_crit_section_;
    CRITICAL_SECTION treasure_box_crit_section_;
    int ukn314;
    int mission_result_;
    int map_number_;
    int ukn320;
    int party_number_;
    int user_count_;
    int start_user_count_;
    int ukn330;
    int user_max_level_;
    int user_min_level_;
    int user_average_level_;
    int state_start_tick_;
    int ukn344;
    int add_herd_monster_time_;
    int add_boss_monster_time_;
    int ukn350;
    int monster_goal_count_;
    int ice_worker_index_[kMaxIceWorker];
    int ukn364;
    int ukn368;
    int ice_walker_spawn_time; //int ukn36c;
    char ice_walker_dead; //char ukn370;
    char killer_state_;
    int killer_hp_;
    int anger_killer_state_;
    int anger_killer_hp_;
    int larva_index_[kMaxLarva];
    int middle_boss_index_[kMaxMiddleBoss];
    int middle_boss_killed_[kMaxMiddleBoss];
    int middle_treasure_box_index_[kMaxMiddleTreasureBox];
    int last_treasure_box_index_;
    int is_treasure_box_opened_;
    int middle_treasure_box_spawn_time_;
    int is_middle_treasure_box_spawned_;
    int is_last_treasure_box_opened_;
    int ukn3fc;
    int monster_pos_;
    CRITICAL_SECTION m_cs3;
    DOPPELGANGER_USERINFO user_info_[MAX_PARTYUSER];
    CDoppelgangerMonsterHerd monster_herd_[kMaxMonsterHard];
    int ukn4c24;
    int boos_regen_order_;
};


extern CDoppelganger g_DoppleganerEvent;


extern bool g_bDoppelGangerEvent;
extern int g_nMarksOfDimensionDropRate;
extern float g_fDPMasterLevelExpRate;
extern int g_RegDoppelgangerClear;


struct PMSG_DOPPELGANGER_ENTER_ANS
{
    PBMSG_HEAD2 h;
    BYTE result;
};

struct PMSG_DOPPELGANGER_STATE
{
    PBMSG_HEAD2 h;
    BYTE state;
};


struct PMSG_DOPPELGANGER_MONSTERGOALCOUNT
{
    PBMSG_HEAD2 h;
    BYTE state;
    BYTE count;
};

struct PMSG_DOPPELGANGER_MONSTERPOS
{
    PBMSG_HEAD2 h;
    BYTE monster_pos;
};

struct DOPPELGANGER_POS_INFO
{
    short obj_index;
    BYTE map_number;
    BYTE pos_index;
};

struct PMSG_DOPPELGANGER_USERPOS
{
    PBMSG_HEAD2 h;
    short time;
    BYTE user_count;
};

struct PMSG_DOPPELGANGER_RESULT
{
    PBMSG_HEAD2 h;
    BYTE result;
    int reward_exp;
};

struct PMSG_DOPPELGANGER_ICEWORKER_STATE
{
    PBMSG_HEAD2 h;
    BYTE state;
    BYTE pos_info;
};
