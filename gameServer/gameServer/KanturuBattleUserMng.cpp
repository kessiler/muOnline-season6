// KanturuBattleUserMng.cpp: implementation of the CKanturuBattleUserMng class.
//	GS-CS	1.00.90	JPN		-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuBattleUserMng.h"
#include "KanturuUtil.h"
#include "LogProc.h"

CKanturuBattleUserMng g_KanturuBattleUserMng;
static CKanturuUtil KANTURU_UTIL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuBattleUserMng::CKanturuBattleUserMng()
{
	this->ResetAllData();
}

CKanturuBattleUserMng::~CKanturuBattleUserMng()
{
	return;
}



void CKanturuBattleUserMng::ResetAllData()
{
	this->m_iBattleUserCount = 0;
	this->SetMaxUser(MAX_KANTURU_BATTLE_USER);

	for ( int iCount=0;iCount<MAX_KANTURU_BATTLE_USER;iCount++)
	{
		this->m_BattleUser[iCount].ResetData();
	}
}



BOOL CKanturuBattleUserMng::AddUserData(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
	{
		LogAddTD("[ KANTURU ][ BattleUser ] Add User Fail - Disconnect User [%s][%s]",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		return FALSE;
	}

	if ( this->IsOverMaxUser() )
	{
		LogAddTD("[ KANTURU ][ BattleUser ] Add User Fail - Over Max User [%s][%s]",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		return FALSE;
	}

	for ( int iCount=0;iCount<MAX_KANTURU_BATTLE_USER;iCount++)
	{
		if ( this->m_BattleUser[iCount].IsUseData() == FALSE )
		{
			this->m_BattleUser[iCount].SetIndex(iIndex);
			this->m_iBattleUserCount++;
			
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CKanturuBattleUserMng::DeleteUserData(int iIndex)
{
	if ( iIndex < 0 || iIndex > OBJMAX-1)
	{
		LogAddTD("[ KANTURU ][ BattleUser ] Delete User Fail - Unvalid Index:%d",
			iIndex);

		return FALSE;
	}

	for ( int iCount=0;iCount<MAX_KANTURU_BATTLE_USER;iCount++)
	{
		if ( this->m_BattleUser[iCount].IsUseData() == TRUE )
		{
			if ( this->m_BattleUser[iCount].GetIndex() == iIndex )
			{
				this->m_BattleUser[iCount].ResetData();
				this->m_iBattleUserCount--;
//#if (_GSCS == 0)
				gObj[iIndex].m_bKanturuEntranceByNPC = FALSE;
//#endif

				LogAddTD("[ KANTURU ][ BattleUser ] Delete User - [%s][%s] Current Battle User:%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, this->m_iBattleUserCount);

				return TRUE;
			}
		}
	}

	return FALSE;
}



void CKanturuBattleUserMng::CheckUserState()
{
	int iIndex;

	for ( int iCount=0;iCount<MAX_KANTURU_BATTLE_USER;iCount++)
	{
		if ( this->m_BattleUser[iCount].IsUseData() == TRUE )
		{
			iIndex = this->m_BattleUser[iCount].GetIndex();

			if ( !gObjIsConnected(iIndex))
			{
				this->DeleteUserData(iIndex);

				LogAddTD("[ KANTURU ][ BattleUser ] Delete User - Disconnect [%s][%s]",
					gObj[iIndex].AccountID, gObj[iIndex].Name);
			}

			if ( gObj[iIndex].MapNumber != MAP_INDEX_KANTURU_BOSS &&
				 gObj[iIndex].m_State == 2 &&
				 gObj[iIndex].Live == 1 )
			{
				this->DeleteUserData(iIndex);

				LogAddTD("[ KANTURU ][ BattleUser ] Delete User - Map Move [%s][%s]",
					gObj[iIndex].AccountID, gObj[iIndex].Name);
			}
		}
	}
}



BOOL CKanturuBattleUserMng::MoveAllUser(int iGateNumber)
{
	int iIndex;
	BOOL bMoveGateSuccess = FALSE;

	for ( int iCount=0;iCount<MAX_KANTURU_BATTLE_USER;iCount++)
	{
		iIndex = this->m_BattleUser[iCount].GetIndex();

		if ( this->m_BattleUser[iCount].IsUseData()  )
		{
			bMoveGateSuccess = gObjMoveGate(iIndex, iGateNumber);

			if ( bMoveGateSuccess == TRUE )
			{
				LogAddTD("[ KANTURU ][ BattleUser ] [%s][%s] MoveGate(%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iGateNumber);
			}
			else
			{
				this->DeleteUserData(iIndex);

				LogAddTD( "[ KANTURU ][ BattleUser ] [%s][%s] MoveGate Fail (%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iGateNumber);

				gObjMoveGate(iIndex, 137);
			}
		}
	}

	return TRUE;
}



void CKanturuBattleUserMng::SetMaxUser(int iMaxUser)
{
	this->m_iBattleMaxUser = iMaxUser;

	if ( iMaxUser > MAX_KANTURU_BATTLE_USER )
	{
		this->m_iBattleMaxUser = MAX_KANTURU_BATTLE_USER;

		LogAddTD("[ KANTURU ][ BattleUser ] Set Max User:%d",
			iMaxUser);
	}
}



int CKanturuBattleUserMng::GetMaxUser()
{
	return this->m_iBattleMaxUser;
}


int CKanturuBattleUserMng::GetUserCount()
{
	return this->m_iBattleUserCount;
}


BOOL CKanturuBattleUserMng::IsEmpty()
{
	if ( this->m_iBattleUserCount <= 0 )
	{
		return TRUE;
	}

	return FALSE;
}


BOOL CKanturuBattleUserMng::IsOverMaxUser()
{
	if ( this->m_iBattleUserCount >=  MAX_KANTURU_BATTLE_USER )
	{
		return TRUE;
	}

	return FALSE;
}


void CKanturuBattleUserMng::LogBattleWinnerUserInfo(BYTE btFlag, int iElapsedTime)
{
	int iIndex = -1;
	float fTime = iElapsedTime / 1000.0f;

	for ( int iCount=0;iCount<MAX_KANTURU_BATTLE_USER;iCount++)
	{
		if ( this->m_BattleUser[iCount].IsUseData() == TRUE )
		{
			iIndex = this->m_BattleUser[iCount].GetIndex();
			
			LogAddTD("[ KANTURU ][ BATTLE WINNER ] [%d/ElapsedTime:%0.3f] [%s][%s] MapNumber[%d]-(%d/%d)",
				btFlag, fTime, gObj[iIndex].AccountID, gObj[iIndex].Name,
				gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);

		}
	}
}