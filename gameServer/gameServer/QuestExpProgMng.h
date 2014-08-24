#pragma once

class QuestExpInfo;
class QuestBuff;
class UserQuestAskInfo;
class QuestNeedZen;
class QuestSkillLearn;
class QuestLevelUp;
class QuestGetItem;
class QuestMonsterKill;
class QuestDropItemInfo;


struct PMSG_QUEST_ASK_STATE;
struct OBJECTSTRUCT;

struct NPC_QUESTEXP_INFO;

class QuestExpProgMng
{
public:
    QuestExpProgMng(void);
    ~QuestExpProgMng(void);

    void ReqQuestAskComplete(DWORD quest_info_index_id, int obj_index);
    void QuestExpGiveUpBtnClick(DWORD quest_info_index_id, int obj_index);
    void SendQuestProgress(DWORD quest_index_id, int obj_index);
    void SendQuestProgress(QuestExpInfo* quest_exp_info, DWORD quest_index_id, int obj_index);
    void SendQuestProgress(int episode_number, int quest_switch, int obj_index);
    void SendQuestProgressInfo(DWORD quest_index_id, int obj_index);
    void SendQuestProgressInfo(QuestExpInfo* quest_exp_info, DWORD quest_index_id, int obj_index);
    bool ChkQuestAsk(int episode_number, int obj_index);
    bool GetSendQuestInfo(QuestExpInfo* quest_exp_info, DWORD quest_index_id, int obj_index, NPC_QUESTEXP_INFO* npc_quest_exp_info);
    void ChkUserQuestTypeBuff(QuestBuff* quest_buff, UserQuestAskInfo* user_quest_ask_info, int obj_index);
    void ChkUserQuestTypeNeedZen(QuestNeedZen* quest_need_zen, UserQuestAskInfo* user_quest_ask_info, int obj_index);
    void ChkUserQuestTypeSkillLearn(QuestSkillLearn* quest_skill_learn, UserQuestAskInfo* user_quest_ask_info, int obj_index);
    void ChkUserQuestTypeLevelUp(QuestLevelUp* quest_level_up, UserQuestAskInfo* user_quest_ask_info, int obj_index);
    void DeleteInventoryItem(QuestGetItem* quest_get_item, int obj_index);
    void ChkUserQuestTypeItem(QuestGetItem* quest_get_item, UserQuestAskInfo* user_quest_ask_info, int obj_index);
    void ChkMonsterKillPartyPlay(DWORD quest_index_id, int ask_index, QuestMonsterKill* quest_monster_kill, OBJECTSTRUCT* lpObj, OBJECTSTRUCT* lpMonsterObj);
    void ChkUserQuestTypeMonsterKill(OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpMonsterObj);
    void ChkUserQuestType(int quest_type, OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpMonsterObj, int killer_obj_index);
    void ChkUserQuestTypeEventMap(int quest_type, OBJECTSTRUCT *lpObj, int map_level, int value);
    void AddQuestSwitchList(int episode_number, int quest_switch);
    void SendQuestSwitchList(int obj_index, int send_type);
    void SendQuestAskState(PMSG_QUEST_ASK_STATE* pMsg, int obj_index);
    void GCANSQuestCompleteBtnClick(int obj_index, DWORD quest_info_index_id, char result);
    void SendProgressQuestList(int obj_index);
    void SetQuestProg(int episode_number, int obj_index, int progress_state);
    bool ChkQuestIndexIDToEpLimit(DWORD quest_index_id, int obj_index);
    bool ChkQuestIndexIDToQSLimit(DWORD quest_index_id, int obj_index);
    bool ChkQuestIndexIDToEpAndQSLimit(DWORD quest_index_id, int obj_index);
    void QuestMonsterItemDrop(DWORD quest_index_id, OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpMonsterObj);
    bool ChkQuestMonsterItemDrop(QuestDropItemInfo* quest_drop_item_info);
    bool ChkQuestMonsterItemDrop(DWORD quest_index_id);
    bool IsQuestDropItem(int index, int type, int level);
    void SetQuestTimeLimit(int obj_index, DWORD quest_index_id, int duration);
    int GetCurrentDate();
    int GetExpireDate(int duration);
    bool CheckExpireDate(int item_expire_date);
    int GetLeftDate(int lExpireDate);
private:
    std::vector<int> switch_list_;
};

extern QuestExpProgMng g_QuestExpProgMng;