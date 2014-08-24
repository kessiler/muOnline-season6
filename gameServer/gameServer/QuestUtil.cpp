// QuestUtil.cpp: implementation of the CQuestUtil class.
// GS-N	1.00.56	JPN-CS	0xXXXXXXXX	-	Completed 
// GS-N	1.00.90	JPN-CS	0xXXXXXXXX	-	Completed 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestUtil.h"
#include "logproc.h"
#include "DBSockMng.h"
#include "user.h"
#include "..\common\winutil.h"

CQuestUtil g_QuestUtil;

CQuestUtil::CQuestUtil()
{

}

CQuestUtil::~CQuestUtil()
{

}

void CQuestUtil::Quest3rdRequestInfo(LPOBJ lpObj) 
{
	PMSG_004A2100 pMsg;

	if(lpObj->m_i3rdQuestState == 1)
	{
		return;
	}

	pMsg.h.set((LPBYTE)&pMsg, 0x13, sizeof(pMsg));
	pMsg.iUserIndex = lpObj->m_Index;
	memcpy(pMsg.chCharacterName, lpObj->Name, 10);
	pMsg.chCharacterName[10] = '\0';
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void CQuestUtil::Quest3rdLoadInfo(LPBYTE aRecv) 
{
	PMSG_ANS_MONSTERKILLINFO * pRecv = (PMSG_ANS_MONSTERKILLINFO *)aRecv;

	if(!gObjIsConnectedGP(pRecv->Index))
	{
		return;
	}

	int aIndex = pRecv->Index;

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_i3rdQuestState == 0)
	{
		lpObj->m_i3rdQuestIndex = pRecv->QuestIndex;

		lpObj->MonsterKillInfo[0].MonIndex = pRecv->KillInfo[0].MonIndex;
		lpObj->MonsterKillInfo[0].KillCount = pRecv->KillInfo[0].KillCount;

		lpObj->MonsterKillInfo[1].MonIndex = pRecv->KillInfo[1].MonIndex;
		lpObj->MonsterKillInfo[1].KillCount = pRecv->KillInfo[1].KillCount;

		lpObj->MonsterKillInfo[2].MonIndex = pRecv->KillInfo[2].MonIndex;
		lpObj->MonsterKillInfo[2].KillCount = pRecv->KillInfo[2].KillCount;

		lpObj->MonsterKillInfo[3].MonIndex = pRecv->KillInfo[3].MonIndex;
		lpObj->MonsterKillInfo[3].KillCount = pRecv->KillInfo[3].KillCount;

		lpObj->MonsterKillInfo[4].MonIndex = pRecv->KillInfo[4].MonIndex;
		lpObj->MonsterKillInfo[4].KillCount = pRecv->KillInfo[4].KillCount;

		lpObj->m_i3rdQuestState = 1;

		LogAddTD("[Quest] Set MonsterKillInfo [%s][%s](%d) (%d : %d/%d, %d/%d, %d/%d, %d/%d, %d/%d)", lpObj->AccountID, lpObj->Name, lpObj->m_Index, lpObj->m_i3rdQuestIndex,
		lpObj->MonsterKillInfo[0].MonIndex, lpObj->MonsterKillInfo[0].KillCount, 
		lpObj->MonsterKillInfo[1].MonIndex, lpObj->MonsterKillInfo[1].KillCount,
		lpObj->MonsterKillInfo[2].MonIndex, lpObj->MonsterKillInfo[2].KillCount,
		lpObj->MonsterKillInfo[3].MonIndex, lpObj->MonsterKillInfo[3].KillCount,
		lpObj->MonsterKillInfo[4].MonIndex, lpObj->MonsterKillInfo[4].KillCount);
	}
}

void CQuestUtil::Quest3rdSaveInfo(LPOBJ lpObj) 
{
	PMSG_SAVE_MONSTERKILLINFO pMsg;

	if(lpObj->m_i3rdQuestState == 0)
	{
		return;
	}

	PHeadSetB( (LPBYTE)&pMsg, 0x14, sizeof(pMsg)); //check packet with load
	
	memcpy(pMsg.szName, lpObj->Name, 10);
	pMsg.Index = lpObj->m_Index;
	pMsg.QuestIndex = lpObj->m_i3rdQuestIndex;

	pMsg.KillInfo[0].MonIndex = lpObj->MonsterKillInfo[0].MonIndex;
	pMsg.KillInfo[0].KillCount = lpObj->MonsterKillInfo[0].KillCount;

	pMsg.KillInfo[1].MonIndex = lpObj->MonsterKillInfo[1].MonIndex;
	pMsg.KillInfo[1].KillCount = lpObj->MonsterKillInfo[1].KillCount;

	pMsg.KillInfo[2].MonIndex = lpObj->MonsterKillInfo[2].MonIndex;
	pMsg.KillInfo[2].KillCount = lpObj->MonsterKillInfo[2].KillCount;

	pMsg.KillInfo[3].MonIndex = lpObj->MonsterKillInfo[3].MonIndex;
	pMsg.KillInfo[3].KillCount = lpObj->MonsterKillInfo[3].KillCount;

	pMsg.KillInfo[4].MonIndex = lpObj->MonsterKillInfo[4].MonIndex;
	pMsg.KillInfo[4].KillCount = lpObj->MonsterKillInfo[4].KillCount;
	
	LogAddTD("[Quest] Save MonsterKillInfo [%s][%s] (%d : %d/%d, %d/%d, %d/%d, %d/%d, %d/%d)", lpObj->AccountID, lpObj->Name, pMsg.QuestIndex,
		pMsg.KillInfo[0].MonIndex, pMsg.KillInfo[0].KillCount, 
		pMsg.KillInfo[1].MonIndex, pMsg.KillInfo[1].KillCount,
		pMsg.KillInfo[2].MonIndex, pMsg.KillInfo[2].KillCount,
		pMsg.KillInfo[3].MonIndex, pMsg.KillInfo[3].KillCount,
		pMsg.KillInfo[4].MonIndex, pMsg.KillInfo[4].KillCount);

	cDBSMng.Send((char*)&pMsg, sizeof(pMsg));
}