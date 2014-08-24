// RaklionUtil.cpp: implementation of the CRaklionUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionUtil.h"
#include "..\common\winutil.h"
#include "TNotice.h"
#include "..\include\prodef.h"
#include "RaklionBattleUserMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaklionUtil g_RaklionUtil;

CRaklionUtil::CRaklionUtil()
{

}

CRaklionUtil::~CRaklionUtil()
{

}

void CRaklionUtil::NotifyRaklionChangeState(int iState, int iDetailState)
{
	PMSG_ANS_RAKLION_STATE_CHANGE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x12, sizeof(pMsg));
	pMsg.btState = iState;
	pMsg.btDetailState = iDetailState;

	if(iState == 1)
	{
		this->SendDataRaklionMapUser((LPBYTE)&pMsg,sizeof(pMsg));	
		return;
	}

	this->SendDataRaklionBossMapUser((LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::NotifyRaklionSuccessValue(int iSuccessValue)
{
	PMSG_ANS_RAKLION_SUCCESS_VALUE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x13, sizeof(pMsg));
	pMsg.btValue = iSuccessValue;

	this->SendDataRaklionBossMapUser((LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::NotifyRaklionCurrentState(int iIndex, BYTE btState, BYTE btDetailState)
{
	PMSG_ANS_RAKLION_CURRENT_STATE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x11, sizeof(pMsg));
	pMsg.btCurrentState = btState;
	pMsg.btCurrentDetailState = btDetailState;

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::NotifyRaklionWideAreaAttack(int iIndex, int iTargetIndex, int iSkillType) //00647C80 
{
	PMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x14, sizeof(pMsg));
	pMsg.btObjClassH = SET_NUMBERH(gObj[iIndex].Class);
	pMsg.btObjClassL = SET_NUMBERL(gObj[iIndex].Class);
	pMsg.btType = iSkillType;

	this->SendDataToUser(iTargetIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::NotifyRaklionUserMonsterCount(BYTE btMonsterCount, BYTE btUserCount)
{
	PMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x15, sizeof(pMsg));
	pMsg.btMonsterCount = btMonsterCount;
	pMsg.btUserCount = btUserCount;

	this->SendDataRaklionBossMapUser((LPBYTE)&pMsg, sizeof(pMsg));
}

void CRaklionUtil::SendMsgUser(LPSTR lpszMsg, ...) //00647D90 
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 1, szBuffer);

	for ( int iCount=0;iCount<g_RaklionBattleUserMng.m_vtUserInfo.size();iCount++)
	{
		if ( gObj[g_RaklionBattleUserMng.m_vtUserInfo[iCount]].Connected == PLAYER_PLAYING &&
			 gObj[g_RaklionBattleUserMng.m_vtUserInfo[iCount]].Type == OBJ_USER)
		{
			TNotice::SendNoticeToUser(g_RaklionBattleUserMng.m_vtUserInfo[iCount], &pNotice);
		}
	}
}

void CRaklionUtil::SendMsgRaklionBossMapUser(LPSTR lpszMsg, ...) //00647F00 
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);

	for ( int iCount=OBJ_STARTUSERINDEX;iCount<OBJMAX;iCount++)
	{
		if ( gObj[iCount].Connected == PLAYER_PLAYING &&
			 gObj[iCount].Type == OBJ_USER &&
			 gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS)
		{
			TNotice::SendNoticeToUser(iCount, &pNotice);
		}
	}
}

void CRaklionUtil::SendDataRaklionBossMapUser(LPBYTE lpMsg, int iSize)
{
	for ( int iCount=OBJ_STARTUSERINDEX;iCount<OBJMAX;iCount++)
	{
		if ( gObj[iCount].Connected == PLAYER_PLAYING &&
			 gObj[iCount].Type == OBJ_USER &&
			 gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS)
		{
			DataSend(iCount, lpMsg, iSize);
		}
	}
}

void CRaklionUtil::SendMsgRaklionMapUser(LPSTR lpszMsg, ...) //00648120 
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);

	for ( int iCount=OBJ_STARTUSERINDEX;iCount<OBJMAX;iCount++)
	{
		if ( gObj[iCount].Connected == PLAYER_PLAYING &&
			 gObj[iCount].Type == OBJ_USER &&
			 ( gObj[iCount].MapNumber == MAP_INDEX_RAKLION_FIELD ||
			 gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS) )
		{
			TNotice::SendNoticeToUser(iCount, &pNotice);
		}
	}
}

void CRaklionUtil::SendDataRaklionMapUser(LPBYTE lpMsg, int iSize)
{
	for ( int iCount=OBJ_STARTUSERINDEX;iCount<OBJMAX;iCount++)
	{
		if ( gObj[iCount].Connected == PLAYER_PLAYING &&
			 gObj[iCount].Type == OBJ_USER &&
			 ( gObj[iCount].MapNumber == MAP_INDEX_RAKLION_FIELD ||
			 gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS ) )
		{
			DataSend(iCount, lpMsg, iSize);
		}
	}
}

void CRaklionUtil::SendMsgAllUser(LPSTR lpszMsg, ...)
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
	
	TNotice::SendNoticeToAllUser(&pNotice);
}

void CRaklionUtil::SendDataAllUser(LPBYTE lpMsg, int iSize) //00648450 
{
	for ( int iCount=OBJ_STARTUSERINDEX;iCount<OBJMAX;iCount++)
	{
		if ( gObj[iCount].Connected == PLAYER_PLAYING &&
			 gObj[iCount].Type == OBJ_USER )
		{
			DataSend(iCount, lpMsg, iSize);
		}
	}
}

void CRaklionUtil::SendMsgToUser(int iIndex, LPSTR lpszMsg, ...) //006484F0 
{
	if ( !lpszMsg )
		return;

	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, szBuffer);
	
	TNotice::SendNoticeToUser(iIndex, &pNotice);
}

void CRaklionUtil::SendDataToUser(int iIndex, LPBYTE lpMsg, int iSize) //006485B0
{
	if ( gObj[iIndex].Connected == PLAYER_PLAYING &&
		 gObj[iIndex].Type == OBJ_USER )
	{
		DataSend(iIndex, lpMsg, iSize);
	}
}

void CRaklionUtil::SendRaklionChattingMsg(int iIndex, LPSTR lpMsg, ...) //00648620 
{
	if ( !lpMsg )
		return;

	char szBuffer[512]="";
	
	va_list pArguments;
	
	va_start( pArguments,  lpMsg);	// review
	vsprintf(szBuffer, lpMsg, pArguments);
	va_end(pArguments);

	TNotice::GCServerMsgStringSend(lpMsg, iIndex, 1);
}