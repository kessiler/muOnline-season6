#include "stdafx.h"
#include "QuestExpProtocol.h"

#include "QuestExpUserMng.h"
#include "user.h"
#include "MuLua.h"
#include "QuestExpProgMng.h"
#include "..\Lua\include\lua.hpp"
#include "LuaFun.h"
#include "logproc.h"
#include "NpcTalk.h"
#include "..\common\winutil.h"

void CGReqQuestSwitch(PMSG_REQ_QUESTEXP *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    OBJECTSTRUCT* lpObj = &gObj[aIndex];

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->quest_info_index_id, aIndex) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    LogAddTD(
        "[QuestExp] Selection Episode List Choose One [%s][%s] QuestInfoIndexID[0x%x] Choose[%d]",
        gObj[aIndex].AccountID,
        gObj[aIndex].Name,
        pMsg->quest_info_index_id,
        pMsg->choose);

    lua_State* L = g_MuLuaQuestExp.GetLua();

    if( !L )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    if( g_QuestExpUserMng.IsQuestAccept(pMsg->quest_info_index_id, aIndex) )
    {
        g_Generic_Call(L, "CGReqQuestSwitch", "iii>", pMsg->quest_info_index_id, pMsg->choose, aIndex);
    }
    else
    {
        PMSG_ANS_QUESTEXP pAnsMsg;
        pAnsMsg.result = 1;

        PHeadSubSetB((LPBYTE)&pAnsMsg, 0xF6u, 0, 5);
        DataSend(gObj[aIndex].m_Index, (LPBYTE)&pAnsMsg, sizeof(pAnsMsg));
    }
}

void CGReqQuestProgress(PMSG_REQ_QUESTEXP *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    OBJECTSTRUCT* lpObj = &gObj[aIndex];

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->quest_info_index_id, aIndex) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    LogAddTD(
        "[QuestExp] Selection Statements Choose One - User NPC Talking [%s][%s] QuestInfoIndexID[0x%x] Choose[%d]",
        gObj[aIndex].AccountID,
        gObj[aIndex].Name,
        pMsg->quest_info_index_id,
        pMsg->choose);

    lua_State* L = g_MuLuaQuestExp.GetLua();

    if( !L )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    g_Generic_Call(L, "CGReqQuestProgress", "iii>", 
                        pMsg->quest_info_index_id, pMsg->choose, aIndex);
}

void CGReqQuestComplete(PMSG_REQ_QUESTEXP_COMPLETE *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    OBJECTSTRUCT* lpObj = &gObj[aIndex];

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->quest_info_index_id, aIndex) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    LogAddTD(
        "[QuestExp] ReqQuestComplete [%s][%s] QuestInfoIndexID[0x%x]",
        gObj[aIndex].AccountID,
        gObj[aIndex].Name,
        pMsg->quest_info_index_id);

    lua_State* L = g_MuLuaQuestExp.GetLua();

    if( !L )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    g_Generic_Call(L, "CGReqQuestComplete", "ii>", pMsg->quest_info_index_id, aIndex);
}

void CGReqQuestGiveUp(PMSG_REQ_QUESTEXP_GIVEUP *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->quest_info_index_id, aIndex) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    g_QuestExpProgMng.QuestExpGiveUpBtnClick(pMsg->quest_info_index_id, aIndex);
}

void CGReqTutorialKeyComplete(PMSG_REQ_QUESTEXP_ASK_COMPLETE *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->quest_info_index_id, aIndex) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    g_QuestExpProgMng.ReqQuestAskComplete(pMsg->quest_info_index_id, aIndex);
}

void CGReqProgressQuestList(PMSG_REQ_QUESTEXP_PROGRESS_LIST *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    g_QuestExpProgMng.SendProgressQuestList(aIndex);
}

void CGReqProgressQuestInfo(PMSG_REQ_QUESTEXP_PROGRESS_INFO *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    if( !g_QuestExpProgMng.ChkQuestIndexIDToEpLimit(pMsg->quest_info_index_id, aIndex) )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;

    }    

    g_QuestExpProgMng.SendQuestProgressInfo(pMsg->quest_info_index_id, aIndex);
}

void CGReqEventItemQuestList(PMSG_REQ_EVENT_ITEM_EP_LIST *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    lua_State* L = g_MuLuaQuestExp.GetLua();

    if( !L )
    {
        LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
        return;
    }

    g_Generic_Call(L, "ItemAndEvent", "i>", aIndex);
}

void CGReqQuestExp(PMSG_REQ_NPC_QUESTEXP *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    OBJECTSTRUCT* lpObj = &gObj[aIndex];

    if( !gObjIsConnected(aIndex) )
        return;

    if( lpObj->m_bUserQuestInfoSent == 1 )
    {
        lua_State* L = g_MuLuaQuestExp.GetLua();

        if( !L )
        {
            LogAddTD("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
            return;
        }

        g_Generic_Call(L, "NpcTalkClick", "ii>", lpObj->TargetShopNumber, lpObj->m_Index);
    }
}

void CGReqAttDefPowerInc(PMSG_REQ_ATTDEF_POWER_INC *pMsg, int aIndex)
{
    if( !OBJMAX_RANGE(aIndex) )
        return;

    if( !gObjIsConnected(aIndex) )
        return;

    NpcShadowPhantom(aIndex);
}