#pragma once


class UserQuestAskInfo
{
public:
    UserQuestAskInfo(){ Init(); }
    ~UserQuestAskInfo(){}

    void Init(){quest_type_ = 0; is_complete_ = false;value_ = 0;index_id_ = 0;}
    void Clear(){quest_type_ = 0; is_complete_ = false;value_ = 0;index_id_ =0;}

    int GetQuestType() const { return quest_type_; }
    void SetQuestType(int quest_type) { quest_type_ = quest_type; }

    bool IsComplete() const { return is_complete_; }
    void SetComplete(bool is_complete) { is_complete_ = is_complete; }

    int GetValue() const { return value_; }
    void SetValue(int value) { value_ = value; }

    int GetIndexID() const { return index_id_; }
    void SetIndexID(int index_id) { index_id_ = index_id; }

protected:
    int quest_type_;
    bool is_complete_;
    int value_;
    int index_id_;
};


class UserQuestReward
{
public:
    UserQuestReward():quest_reward_type_(0),value_(0),index_id_(0){}
    ~UserQuestReward(){}

    int GetQuestRewardType() const { return quest_reward_type_; }
    void SetQuestRewardType(int quest_reward_type) { quest_reward_type_ = quest_reward_type; }

    int GetValue() const { return value_; }
    void SetValue(int value) { value_ = value; }

    int GetIndexID() const { return index_id_; }
    void SetIndexID(int index_id) { index_id_ = index_id; }

protected:
    int quest_reward_type_;
    int value_;
    int index_id_;
};

class UserQuestInfo
{
public:
    enum { kMaxAskInfo = 5 };

    UserQuestInfo(){ Init(); }
    ~UserQuestInfo(){}

    void Init();
    void Clear();
    void QuestAskInfoClear();

    bool SetEpisode(int episode_number);
    int GetEpisode();

    bool SetQuestSwitch(int quest_switch);
    int GetQuestSwitch();

    void SetAskCnt(int ask_count);
    int GetAskCnt();

    void SetStartDate(int start_date);
    int GetStartDate();

    void SetEndDate(int end_date);
    int GetEndDate();

    void SetQuestProgState(int prog_state);
    int GetQuestProgState();


    UserQuestAskInfo user_quest_ask_info_[kMaxAskInfo];

protected:
    int episode_number_;
    int quest_switch_;
    int ask_count_;
    int prog_state_;
    int start_date_;
    int end_date_;
};
