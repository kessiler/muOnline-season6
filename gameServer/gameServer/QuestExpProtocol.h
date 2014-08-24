#pragma once
#include "..\include\prodef.h"

struct PMSG_REQ_QUESTEXP
{
    PBMSG_HEAD2 h;
    DWORD quest_info_index_id;
    BYTE choose;
};

#pragma pack(push, 1)
struct PMSG_ANS_QUESTEXP
{
    PBMSG_HEAD2 h;
    BYTE result;
};
#pragma pack(pop)

struct PMSG_REQ_QUESTEXP_COMPLETE
{
    PBMSG_HEAD2 h;
    DWORD quest_info_index_id;
};

struct PMSG_REQ_QUESTEXP_GIVEUP
{
    PBMSG_HEAD2 h;
    DWORD quest_info_index_id;
};

struct PMSG_REQ_QUESTEXP_ASK_COMPLETE
{
    PBMSG_HEAD2 h;
    DWORD quest_info_index_id;
};

struct PMSG_REQ_QUESTEXP_PROGRESS_LIST
{
    PBMSG_HEAD2 h;
    DWORD quest_info_index_id;
};

struct PMSG_REQ_QUESTEXP_PROGRESS_INFO
{
    PBMSG_HEAD2 h;
    DWORD quest_info_index_id;
};

struct PMSG_REQ_EVENT_ITEM_EP_LIST
{
    PBMSG_HEAD2 h;
};

struct PMSG_REQ_NPC_QUESTEXP
{
    PBMSG_HEAD2 h;
};

struct PMSG_REQ_ATTDEF_POWER_INC
{
    PBMSG_HEAD2 h;
};

void CGReqQuestSwitch(PMSG_REQ_QUESTEXP *pMsg, int aIndex);
void CGReqQuestProgress(PMSG_REQ_QUESTEXP *pMsg, int aIndex);
void CGReqQuestComplete(PMSG_REQ_QUESTEXP_COMPLETE *pMsg, int aIndex);
void CGReqQuestGiveUp(PMSG_REQ_QUESTEXP_GIVEUP *pMsg, int aIndex);
void CGReqTutorialKeyComplete(PMSG_REQ_QUESTEXP_ASK_COMPLETE *pMsg, int aIndex);
void CGReqProgressQuestList(PMSG_REQ_QUESTEXP_PROGRESS_LIST *pMsg, int aIndex);
void CGReqProgressQuestInfo(PMSG_REQ_QUESTEXP_PROGRESS_INFO *pMsg, int aIndex);
void CGReqEventItemQuestList(PMSG_REQ_EVENT_ITEM_EP_LIST *pMsg, int aIndex);

void CGReqQuestExp(PMSG_REQ_NPC_QUESTEXP *pMsg, int aIndex);
void CGReqAttDefPowerInc(PMSG_REQ_ATTDEF_POWER_INC *pMsg, int aIndex);