// KanturuEntranceNPC.cpp: implementation of the CKanturuEntranceNPC class.
//	GS-CS	1.00.90	JPN		-	Completed
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KanturuEntranceNPC.h"
#include "Kanturu.h"
#include "KanturuBattleUserMng.h"
#include "LogProc.h"
#include "KanturuUtil.h"
#include "..\common\winutil.h"

static CKanturuUtil KANTURU_UTIL;

CKanturuEntranceNPC g_KanturuEntranceNPC;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuEntranceNPC::CKanturuEntranceNPC()
{
	return;
}

CKanturuEntranceNPC::~CKanturuEntranceNPC()
{
	return;
}


struct PMSG_ANS_KANTURU_STATE_INFO
{
	PBMSG_HEAD2 h;	// C1:D1:00
	BYTE btState;	// 4
	BYTE btDetailState;	// 5
	BYTE btEnter;	// 6
	BYTE btUserCount;	// 7
	int iRemainTime;	// 8
};


void CKanturuEntranceNPC::NotifyEntranceInfo(int iIndex)
{
	PMSG_ANS_KANTURU_STATE_INFO pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x00, sizeof(pMsg));
	int iCurrentState = g_Kanturu.GetKanturuState();
	int iCurrentDetailState = g_Kanturu.GetKanturuDetailState();
	pMsg.btState = iCurrentState;
	pMsg.btDetailState = iCurrentDetailState;
	pMsg.btEnter = g_Kanturu.CheckCanEnterKanturuBattle();
	pMsg.btUserCount = g_KanturuBattleUserMng.GetUserCount();
	pMsg.iRemainTime = g_Kanturu.GetRemainTime();

	KANTURU_UTIL.SendDataToUser(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}



void CKanturuEntranceNPC::NotifyResultEnterKanturuBossMap(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	BOOL bMoveGateSuccess = FALSE;
	int iResult = g_Kanturu.CheckEnterKanturu(iIndex);

	if ( iResult == 0 )
	{
		int iCurrentState = g_Kanturu.GetKanturuState();

		if ( iCurrentState == KANTURU_STATE_BATTLE_OF_MAYA )
		{
			bMoveGateSuccess = gObjMoveGate(iIndex, 133);

			if ( bMoveGateSuccess ==TRUE )
			{
				g_KanturuBattleUserMng.AddUserData(iIndex);

				LogAddTD("[ KANTURU ][ Battle User ] Add User [%s][%s] Current Battle User:%d State:[%d-%d] (CharInfoSave : Class=%d, Level=%d, LvPoint=%d, Exp=%u, Str=%d, Dex=%d, Vit=%d, Energy=%d, LeaderShip=%d, Map=%d, Pk=%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name,
					g_KanturuBattleUserMng.GetUserCount(), g_Kanturu.GetKanturuState(),
					g_Kanturu.GetKanturuDetailState(),
					gObj[iIndex].Class, gObj[iIndex].Level, gObj[iIndex].LevelUpPoint,
					gObj[iIndex].Experience, gObj[iIndex].Strength, gObj[iIndex].Dexterity,
					gObj[iIndex].Vitality, gObj[iIndex].Energy, gObj[iIndex].Leadership, 
					gObj[iIndex].MapNumber, gObj[iIndex].m_PK_Level);

				KANTURU_UTIL.NotifyKanturuCurrentState(iIndex, 
					g_Kanturu.GetKanturuState(),g_Kanturu.GetKanturuDetailState());

//#if (_GSCS == 0)
				gObj[iIndex].m_bKanturuEntranceByNPC = TRUE;
//#endif
			}
		}
		else if ( iCurrentState == KANTURU_STATE_TOWER_OF_REFINEMENT )
		{
			bMoveGateSuccess = gObjMoveGate(iIndex, 135);

			KANTURU_UTIL.NotifyKanturuCurrentState(iIndex, 
				g_Kanturu.GetKanturuState(),g_Kanturu.GetKanturuDetailState());

//#if (_GSCS == 0)
			gObj[iIndex].m_bKanturuEntranceByNPC = TRUE;
//#endif
		}
		else
		{
			LogAddTD("[ KANTURU ][ BossMapMove ] Error - State(%d) Character[%s][%s]",
				iCurrentState, lpObj->AccountID, lpObj->Name);
		}
	}
	else
	{
		KANTURU_UTIL.NotifyKanturuEntranceReqResult(iIndex, iResult);

		LogAddTD("[ KANTURU ][ BossMapMove ] Fail - Result(%d) Character[%s][%s]",
			iResult, lpObj->AccountID, lpObj->Name);

//#if (_GSCS == 0)
		gObj[iIndex].m_bKanturuEntranceByNPC = FALSE;
//#endif
	}
}
