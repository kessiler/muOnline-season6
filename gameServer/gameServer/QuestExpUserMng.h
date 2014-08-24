#pragma once


class QuestExpRewardKindRandom;
class QuestExpAsk;
class UserQuestInfo;
struct PMSG_ANS_QUESTEXP_INFO;

class QuestExpUserMng
{
public:
    QuestExpUserMng(void);
    ~QuestExpUserMng(void);

    int IsProgQuestInfo(int episode_number, int obj_index);
    int IsProgQuestInfo(DWORD quest_info_index_id, int obj_index);
    bool IsQuestComplete(int episode_number, int obj_index);
    bool QuestExpExtentCheck(int x, int y, int w, int h);
    bool QuestExpCheckInventoryEmptySpace(BYTE* temp_inventory_map, int item_height, int item_width, int obj_index);
    BYTE QuestExpOnlyInventoryRectCheck(BYTE *temp_inventory_map, int sx, int sy, int width, int height, int obj_index);
    void QuestExpTempInventoryItemSet(BYTE *temp_inventory_map, int itempos, int xl, int yl, BYTE set_byte, int obj_index);
    bool GetRandomRewardItemIndex(int* set_item, QuestExpRewardKindRandom* reward_kind_random);
    bool IsRandRewardIndex(int reward_index_id, QuestExpRewardKindRandom* reward_kind_random);
    bool IsRandResultReward(int reward_index_id, QuestExpRewardKindRandom* reward_kind_random, int* rand_result_index_id);
    int InvenChk_EnableReward(int episode_number, int obj_index, int* rand_result_index_id);
    int SendQuestReward(int episode_number, int obj_index);
    void InitUserQuestAskInfo(int episode_number, int obj_index);
    void SetQuestSwitch(int episode_number, int quest_switch, int obj_index);
    bool AddUserQuestAskInfo(DWORD quest_info_index_id, int obj_index);
    void AddUserQuestAskMonsterKill(QuestExpAsk* quest_exp_ask, int obj_index, int episode_number, int ask_count);
    int GetQuestSwitch(int episode_number, int obj_index);
    void UserAllQuestInfoSave(int obj_index);
    void UserQuestInfoSave(DWORD quest_info_index_id, int obj_index);
    void DB_ReqUserQuestInfo(int obj_index);
    void UserQuestInfoLoad(PMSG_ANS_QUESTEXP_INFO* lpRecv);
    UserQuestInfo* GetUserQuestInfo(int episode_number, int obj_index);

    int GetQuestProgState(DWORD quest_info_index_id, int obj_index);
    int GetQuestProgState(int ep, int obj_index);

    bool IsQuestAccept(DWORD quest_info_index_id, int obj_index);
    bool InventoryExtentCheck(int x, int y, int w, int h);
    int CheckOutOfInventory(int aIndex, int sy, int height);
};


extern QuestExpUserMng g_QuestExpUserMng;