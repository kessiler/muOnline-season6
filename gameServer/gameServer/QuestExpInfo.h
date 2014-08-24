#pragma once

#include "../common/zzzitem.h"

class QuestExpAsk
{
public:
    QuestExpAsk():quest_type_(0), index_id_(0){};
    virtual ~QuestExpAsk(){};

    int GetQuestType() const { return quest_type_; }
    void SetQuestType(int quest_type) { quest_type_ = quest_type; }

    int GetIndexID() const { return index_id_; }
    void SetIndexID(int index_id) { index_id_ = index_id; }

protected:
    int quest_type_;
    int index_id_;
};

class QuestExpReward
{
public:
    QuestExpReward():reward_type_(0),index_id_(0){};
    virtual ~QuestExpReward(){};

    int GetRewardType() const { return reward_type_; }
    void SetRewardType(int reward_type) { reward_type_ = reward_type; }

    int GetIndexID() const { return index_id_; }
    void SetIndexID(int index_id) { index_id_ = index_id; }

protected:
    int reward_type_;
    int index_id_;

};



class QuestGetItem : public QuestExpAsk
{
public:
    QuestGetItem():item_number_(0), item_count_(0), item_level_(0),
                    item_durability_(0), item_skill_(0), 
                    item_excellent_option_(0), item_option_(0){};

    virtual ~QuestGetItem(){};

    int GetQuestItemNum() const { return item_number_; }
    void SetQuestItemNum(int item_type, int item_index) { 
                                item_number_ = ITEMGET(item_type, item_index); }

    int GetQuestItemCnt() const { return item_count_; }
    void SetQuestItemCnt(int item_count) { item_count_ = item_count; }

    BYTE GetQuestItemLevel() const { return item_level_; }
    void SetQuestItemLevel(BYTE item_level) { item_level_ = item_level; }

    BYTE GetQuestItemSkill() const { return item_skill_; }
    void SetQuestItemSkill(BYTE item_skill) { item_skill_ = item_skill; }

    BYTE GetQuestItemDur() const { return item_durability_; }
    void SetQuestItemDur(BYTE item_durability) { item_durability_ = item_durability; }

    BYTE GetQuestItemExOpt() const { return item_excellent_option_; }
    void SetQuestItemExOpt(BYTE item_excellent_option) { item_excellent_option_ = item_excellent_option; }

    BYTE GetQuestItemOpt() const { return item_option_; }
    void SetQuestItemOpt(BYTE item_option) { item_option_ = item_option; }

protected:
    int item_number_;
    int item_count_;
    BYTE item_level_;
    BYTE item_durability_;
    BYTE item_skill_;
    BYTE item_excellent_option_;
    BYTE item_option_;
};



class QuestMonsterKill : public QuestExpAsk
{
public:
    QuestMonsterKill():kill_count_(0), monster_index_(0), is_party_play_(false){}
    virtual ~QuestMonsterKill(){};

    int GetMonsterKillCnt() const { return kill_count_; }
    void SetMonsterKillCnt(int kill_count) { kill_count_ = kill_count; }

    int GetMonsterIndex() const { return monster_index_; }
    void SetMonsterIndex(int monster_index) { monster_index_ = monster_index; }

    bool IsPartyPlay() const { return is_party_play_; }
    void SetPartyPlay(bool is_party_play) { is_party_play_ = is_party_play; }

protected:
    int kill_count_;
    int monster_index_;
    bool is_party_play_;
};

class RewardExp : public QuestExpReward
{
public:
    RewardExp():experience_(0){}
    virtual ~RewardExp(){}

    __int64 GetRewardExp() const { return experience_; }
    void SetRewardExp(__int64 experience) { experience_ = experience; }

protected:
    __int64 experience_;
};



class RewardItem : public QuestExpReward
{
public:

    RewardItem():item_number_(0), item_count_(0), item_level_(0),
                item_durability_(0), item_skill_(0), 
                item_excellent_option_(0), item_option_(0), item_overlap_(0){}

    virtual ~RewardItem(){}

    int GetRewardItemNum() const { return item_number_; }
    void SetRewardItemNum(int item_type, int item_index) { 
        item_number_ = ITEMGET(item_type, item_index); }

    int GetRewardItemCnt() const { return item_count_; }
    void SetRewardItemCnt(int item_count) { item_count_ = item_count; }

    BYTE GetRewardItemLevel() const { return item_level_; }
    void SetRewardItemLevel(BYTE item_level) { item_level_ = item_level; }

    BYTE GetRewardItemSkill() const { return item_skill_; }
    void SetRewardItemSkill(BYTE item_skill) { item_skill_ = item_skill; }

    BYTE GetRewardItemDur() const { return item_durability_; }
    void SetRewardItemDur(BYTE item_durability) { item_durability_ = item_durability; }

    BYTE GetRewardItemExOpt() const { return item_excellent_option_; }
    void SetRewardItemExOpt(BYTE item_excellent_option) { item_excellent_option_ = item_excellent_option; }

    BYTE GetRewardItemOpt() const { return item_option_; }
    void SetRewardItemOpt(BYTE item_option) { item_option_ = item_option; }

    BYTE GetRewardItemOverlap() const { return item_overlap_; }
    void SetRewardItemOverlap(BYTE item_overlap) { item_overlap_ = item_overlap; }

protected:
    int item_number_;
    BYTE item_level_;
    BYTE item_durability_;
    int item_count_;
    BYTE item_skill_;
    BYTE item_option_;
    BYTE item_excellent_option_;
    BYTE item_overlap_;
};

class RewardContribute : public QuestExpReward
{
public:
    RewardContribute():contribute_point_(0){}
    virtual ~RewardContribute(){}

    int GetRewardContributePoint() const { return contribute_point_; }
    void SetRewardContributePoint(int contribute_point) { contribute_point_ = contribute_point; }

protected:
    int contribute_point_;
};

class QuestTutorial : public QuestExpAsk
{
public:
    QuestTutorial(){}
    virtual ~QuestTutorial(){};
};

class QuestProgress : public QuestExpAsk
{
public:
    QuestProgress(){}
    virtual ~QuestProgress(){};
};

class QuestSkillLearn : public QuestExpAsk
{
public:
    QuestSkillLearn():skill_index_(0){}
    virtual ~QuestSkillLearn(){};

    int GetQuestSkillIndex() const { return skill_index_; }
    void SetQuestSkillIndex(int skill_index) { skill_index_ = skill_index; }

protected:
      int skill_index_;
};

class QuestLevelUp : public QuestExpAsk
{
public:
    QuestLevelUp():level_value_(0){}
    virtual ~QuestLevelUp(){};

    int GetQuestLevelVal() const { return level_value_; }
    void SetQuestLevelVal(int level_value) { level_value_ = level_value; }

protected:
    int level_value_;
};

class QuestBuff : public QuestExpAsk
{
public:
    QuestBuff():buff_index_(0){}
    virtual ~QuestBuff(){};

    int GetQuestBuffIndex() const { return buff_index_; }
    void SetQuestBuffIndex(int buff_index) { buff_index_ = buff_index; }

protected:
    int buff_index_;
};

class QuestNeedZen : public QuestExpAsk
{
public:
    QuestNeedZen():zen_amount_(0){}
    virtual ~QuestNeedZen(){}

    int GetQuestNeedZen() const { return zen_amount_; }
    void SetQuestNeedZen(int zen_amount) { zen_amount_ = zen_amount; }

protected:
    int zen_amount_;
};


class QuestPVP_Point : public QuestExpAsk
{
public:
    QuestPVP_Point():pvp_point_(0), quest_map_number_(0){}
    virtual ~QuestPVP_Point(){};

    int GetQuestPVP_Point() const { return pvp_point_; }
    void SetQuestPVP_Point(int pvp_point) { pvp_point_ = pvp_point; }

    int GetQuestMapNum() const { return quest_map_number_; }
    void SetQuestMapNum(int quest_map_number) { quest_map_number_ = quest_map_number; }

protected:
    int pvp_point_;
    int quest_map_number_;
};

class QuestNpcTalk : public QuestExpAsk
{
public:
    QuestNpcTalk():npc_talk_(0){}
    virtual ~QuestNpcTalk(){};

    int GetQuestNPCTalk() const { return npc_talk_; }
    void SetQuestNPCTalk(int npc_talk) { npc_talk_ = npc_talk; }

protected:
    int npc_talk_;
};


class RewardZen : public QuestExpReward
{
public:
    RewardZen():zen_amount_(0){}
    virtual ~RewardZen(){};

    int GetRewardZen() const { return zen_amount_; }
    void SetRewardZen(int zen_amount) { zen_amount_ = zen_amount; }

protected:
    int zen_amount_;
};


class QuestEventMapKillPoint : public QuestExpAsk
{
public:
    QuestEventMapKillPoint():kill_count_(0), level_(-1), ukn(0){}
    virtual ~QuestEventMapKillPoint(){};

    int GetQuestEventMapKillCnt() const { return kill_count_; }
    void SetQuestEventMapKillCnt(int Kill_count) { kill_count_ = Kill_count; }

    int GetQuestEventMapLevel() const { return level_; }
    void SetQuestEventMapLevel(int level) { level_ = level; }

protected:
    int kill_count_;
    int level_;
    int ukn;
};


class QuestEventMapClear : public QuestExpAsk
{
public:
    QuestEventMapClear():level_(0){}
    virtual ~QuestEventMapClear(){};

    int GetQuestEventMapLevel() const { return level_; }
    void SetQuestEventMapLevel(int level) { level_ = level; }

protected:
    int level_;
};

class QuestEventMapDevilPoint : public QuestExpAsk
{
public:
    QuestEventMapDevilPoint():point_(0),level_(0){}
    virtual ~QuestEventMapDevilPoint(){};

    int GetQuestEventMapDevilPoint() const { return point_; }
    void SetQuestEventMapDevilPoint(int point) { point_ = point; }

    int GetQuestEventMapLevel() const { return level_; }
    void SetQuestEventMapLevel(int level) { level_ = level; }

protected:
    int point_;
    int level_;
};



class QuestExpRewardKind
{
public:
    QuestExpRewardKind(){}
    virtual ~QuestExpRewardKind(){}

    int GetQuestEpisode() const { return quest_episode_; }
    void SetQuestEpisode(int quest_episode) { quest_episode_ = quest_episode; }

    int GetQuestSwitch() const { return quest_switch_; }
    void SetQuestSwitch(int quest_switch) { quest_switch_ = quest_switch; }

    int GetRewardKind() const { return reward_kind_; }
    void SetRewardKind(int reward_kind) { reward_kind_ = reward_kind; }

protected:
    int quest_episode_;
    int quest_switch_;
    int reward_kind_;
};

class QuestExpRewardKindRandom : public QuestExpRewardKind
{
public:
    enum { kMaxRewardCount = 5 };

    QuestExpRewardKindRandom()
    { 
        memset(reward_indexes_, -1, sizeof(reward_indexes_));
        memset(reward_rates_, -1, sizeof(reward_rates_));
    }

    virtual ~QuestExpRewardKindRandom(){}

    int GetRandRewardCnt() const { return random_reward_count_; }
    void SetRandRewardCnt(int random_reward_count) { random_reward_count_ = random_reward_count; }

    int GetRewardItemTotCnt() const { return reward_item_total_count_; }
    void SetRewardItemTotCnt(int reward_item_total_count) { reward_item_total_count_ = reward_item_total_count; }

    const int* GetRewardIndex() const { return reward_indexes_; }
    void SetRewardIndex(int* reward_index) 
    { 
        memcpy(reward_indexes_, reward_index, sizeof(reward_indexes_)); 
    }

    const int* GetRewardRate() const { return reward_rates_; }

    void SetRewardRate(int* reward_rates) 
    { 
        memcpy(reward_rates_, reward_rates, sizeof(reward_rates_)); 
    }


protected:
    int random_reward_count_;
    int reward_item_total_count_;
    int reward_indexes_[kMaxRewardCount];
    int reward_rates_[kMaxRewardCount];
};


class QuestDropItemInfo
{
public:
    QuestDropItemInfo():quest_episode_(0), quest_switch_(0), ask_index_(0),
                        monster_index_(0), reward_rate_(0), item_number_(0),
                        item_level_(0), item_count_(0){}

    ~QuestDropItemInfo(){}

    int GetQuestEpisode() const { return quest_episode_; }
    void SetQuestEpisode(int quest_episode) { quest_episode_ = quest_episode; }

    int GetQuestSwitch() const { return quest_switch_; }
    void SetQuestSwitch(int quest_switch) { quest_switch_ = quest_switch; }

    int GetAskIndex() const { return ask_index_; }
    void SetAskIndex(int ask_index) { ask_index_ = ask_index; }

    int GetMonsterIndex() const { return monster_index_; }
    void SetMonsterIndex(int monster_index) { monster_index_ = monster_index; }

    int GetRewardRate() const { return reward_rate_; }
    void SetRewardRate(int reward_rate) { reward_rate_ = reward_rate; }

    int GetItemNum() const { return item_number_; }
    void SetItemNum(int item_type, int item_index) { 
                        item_number_ = ITEMGET(item_type, item_index); }

    int GetItemLevel() const { return item_level_; }
    void SetItemLevel(int item_level) { item_level_ = item_level; }

    int GetItemCnt() const { return item_count_; }
    void SetItemCnt(int item_count) { item_count_ = item_count; }

protected:
    int quest_episode_;
    int quest_switch_;
    int ask_index_;
    int monster_index_;
    int reward_rate_;
    int item_number_;
    int item_level_;
    int item_count_;
};



class QuestExpInfo
{
public:
    enum { kMaxQuestExpAsk = 6 };
    enum { kMaxQuestExpReward = 5 };


    QuestExpInfo();
    ~QuestExpInfo();

	QuestExpInfo(const QuestExpInfo& rhs);
	QuestExpInfo& operator=(const QuestExpInfo& rhs);

    int GetQuestEpisode() const { return quest_episode_; }
    void SetQuestEpisode(int quest_episode) { quest_episode_ = quest_episode; }

    int GetQuestSwitch() const { return quest_switch_; }
    void SetQuestSwitch(int quest_switch) { quest_switch_ = quest_switch; }

    int GetAskCnt() const { return ask_count_; }
    void SetAskCnt(int ask_count) { ask_count_ = ask_count; }

    int GetRewardCnt() const { return reward_count_; }
    void SetRewardCnt(int reward_count) { reward_count_ = reward_count; }

    bool CheckQuestExpAskIndex(int index_id) const 
    { 
        return (index_id >= 0 && index_id < kMaxQuestExpAsk);
    }

    bool CheckQuestExpRewardIndex(int index_id) const 
    { 
        return (index_id >= 0 && index_id < kMaxQuestExpReward);
    }

    void SetQuestExpAsk(QuestExpAsk *quest_exp_ask);
    void SetQuestExpAsk(QuestExpAsk *quest_exp_ask, int index_id);
    std::map<int, QuestExpAsk*> GetQuestExpAsk();

    void SetQuestReward(QuestExpReward *quest_exp_reward);
    std::map<int, QuestExpReward*> GetQuestReward();

    void Release();

private:
    int quest_episode_;
    int quest_switch_;
    int ask_count_;
    int reward_count_;
    std::map<int, QuestExpAsk*> map_quest_ask_;
    std::map<int, QuestExpReward*> map_quest_reward_;
};



namespace mu
{

namespace quest
{

static const int kMaxEpisodeCount = 25;
static const int kMaxSwitchCount = 256;

inline static bool IsEpisode(int episode_number)
{
    return (episode_number >= 0 && episode_number < kMaxEpisodeCount);
}

inline static bool IsQuestSwitch(int quest_switch)
{
    return (quest_switch >= 0 && quest_switch < kMaxSwitchCount);
}

inline static DWORD GetQuestInfoIndexId(int episode_number, int quest_switch)
{
    return (quest_switch + (episode_number << 16));
}

inline static int GetQuestEpisodeFromInfoIndexId(DWORD quest_info_index_id)
{
    return (quest_info_index_id >> 16);
}

inline static int GetQuestSwitchFromInfoIndexId(DWORD quest_info_index_id)
{
    return  (quest_info_index_id % 0x10000);
}

} // namespace quest

} // namespace mu


struct QUEST_ITEM_INFO
{
    QUEST_ITEM_INFO():item_number(0), item_attribute(0), episode_number(0){}

    int item_number;
    int item_attribute;
    int episode_number;
};

class QuestExpManager
{
public:

    QuestExpManager();
    ~QuestExpManager();

    void QuestExpItemInit();
    void QuestInfoReload();

    void SetQuestExpInfo(QuestExpAsk* quest_exp_ask, int episode_number, int quest_switch);
    void SetQuestExpInfo(QuestExpReward* quest_exp_reward, int episode_number, int quest_switch);

    void AddQuestExpInfoList(QuestExpAsk* quest_exp_ask, DWORD quest_info_index_id);
    void AddQuestExpInfoList(QuestExpReward *quest_exp_reward, DWORD quest_info_index_id);
    void AddQuestExpInfoList(QuestExpInfo* quest_exp_info, DWORD quest_info_index_id);

    void AddQuestExpRewardKind(QuestExpRewardKind* quest_exp_reward_kind, DWORD quest_info_index_id);
    void AddQuestExpRewardKind(QuestExpRewardKind* quest_exp_reward_kind);

    QuestExpRewardKind* GetQuestExpRewardKind(DWORD quest_info_index_id);

    QuestExpAsk* GetQuestExpAsk(int episode_number, int quest_switch, int index_id);
    QuestExpAsk* GetQuestExpAsk(DWORD quest_info_index_id, int index_id);

    QuestExpReward* GetQuestExpReward(int episode_number, int quest_switch, char index_id);

    QuestExpInfo* GetQuestExpInfo(DWORD quest_info_index_id);
    QuestExpInfo* GetQuestExpInfo(int episode_number, int quest_switch);

    void AddQuestDropItemInfo(QuestDropItemInfo* quest_drop_item_info, DWORD quest_info_index_id);

    QuestDropItemInfo* GetQuestDropItemInfo(DWORD quest_info_index_id);
    void AddQuestItemInfo(QUEST_ITEM_INFO quest_item_info);
    bool IsQuestItemInfo(int item_number);
    bool IsQuestItemAtt(int item_number, int quest_item_att);
    int GetQuestItemEp(int item_number);

protected:
    std::map<int, QuestExpInfo> map_quest_exp_info_;
    std::map<DWORD, QuestExpRewardKind*> map_quest_exp_reward_kind_;
    std::map<DWORD, QuestDropItemInfo> map_quest_drop_item_info_;
    std::vector<QUEST_ITEM_INFO> vt_quest_item_info_;
};

extern QuestExpManager g_QuestExpManager;