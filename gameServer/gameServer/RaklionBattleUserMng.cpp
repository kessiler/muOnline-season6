// RaklionBattleUserMng.cpp: implementation of the CRaklionBattleUserMng class.
// GS	1.00.90	JPN	0xXXXXXXXX	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RaklionBattleUserMng.h"
#include "LogProc.h"
#include "User.h"
#include "Raklion.h"
#include "RaklionUtil.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRaklionBattleUserMng g_RaklionBattleUserMng;

CRaklionBattleUserMng::CRaklionBattleUserMng()
{
	this->ResetAllData();
}

CRaklionBattleUserMng::~CRaklionBattleUserMng()
{

}

//0060F380
void CRaklionBattleUserMng::ResetAllData() //006449C0 (identical)
{
	while((BYTE)this->m_vtUserInfo.empty() == 0) //same as TRUE
	{
		this->m_vtUserInfo.pop_back();
		//maybe LogAddTD for debug because after there is a clear lol
	}

	this->m_vtUserInfo.clear(); //good
}

//0060F3D0
BOOL CRaklionBattleUserMng::AddUserData(int iIndex) //00644A10 (identical)
{
	if(gObjIsConnected(iIndex) == FALSE)
	{
		LogAddTD("[ RAKLION ][ BattleUser ] Add User Fail - Disconnect User [%s][%s]",
			gObj[iIndex].AccountID, gObj[iIndex].Name);
		return FALSE;
	}

	this->m_vtUserInfo.push_back(iIndex);

	g_RaklionUtil.NotifyRaklionCurrentState(iIndex, g_Raklion.GetRaklionState(), g_Raklion.GetRaklionDetailState());

	LogAddTD("[ RAKLION ][ Battle User ] Add User [%s][%s] Current Battle User:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, this->m_vtUserInfo.size());

	return TRUE;
}

//0060F4E0 
BOOL CRaklionBattleUserMng::DeleteUserData(int iIndex) //00644B20 (identical)
{
	if( iIndex < 0 || iIndex > OBJMAX-1 )
	{
		LogAddC(2, "[ RAKLION ][ BattleUser ] Delete User Fail - Unvalid Index:%d", iIndex);
		return FALSE;
	}

	for( int i = 0; i < this->m_vtUserInfo.size(); i++)
	{
		if(this->m_vtUserInfo[i] == iIndex)
		{
			this->m_vtUserInfo.erase(this->m_vtUserInfo.begin()+i);

			LogAddTD("[ RAKLION ][ BattleUser ] Delete User - [%s][%s] Current Battle User:%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, this->m_vtUserInfo.size());
				return TRUE;
		}
	}

	return FALSE;
}

//0060F610
void CRaklionBattleUserMng::CheckUserState() //00644C50 (identical)
{
	for ( int iCount=0; iCount < this->m_vtUserInfo.size(); iCount++)
	{
		if ( !gObjIsConnected(this->m_vtUserInfo[iCount]))
		{
			this->DeleteUserData(this->m_vtUserInfo[iCount]);

			LogAddTD("[ RAKLION ][ BattleUser ] Delete User - Disconnect [%s][%s]",
					gObj[this->m_vtUserInfo[iCount]].AccountID, 
					gObj[this->m_vtUserInfo[iCount]].Name);
		}

		if ( gObj[this->m_vtUserInfo[iCount]].MapNumber != MAP_INDEX_RAKLION_BOSS &&
			 gObj[this->m_vtUserInfo[iCount]].m_State == 2 &&
			 gObj[this->m_vtUserInfo[iCount]].Live == 1 &&
			 gObj[this->m_vtUserInfo[iCount]].SkillRecallParty_MapNumber != MAP_INDEX_RAKLION_BOSS )
		{
			this->DeleteUserData(this->m_vtUserInfo[iCount]);

			LogAddTD("[ RAKLION ][ BattleUser ] Delete User - Map Move [%s][%s]",
					gObj[this->m_vtUserInfo[iCount]].AccountID, 
					gObj[this->m_vtUserInfo[iCount]].Name);
		}
	}
}

//0060F870
BOOL CRaklionBattleUserMng::MoveAllUser(int iGateNumber) //00644EB0 (identical)
{
	BOOL bMoveGateSuccess = FALSE; //loc2

	for ( int iCount=0; iCount < this->m_vtUserInfo.size(); iCount++)
	{
		bMoveGateSuccess = gObjMoveGate(this->m_vtUserInfo[iCount], iGateNumber);

		if ( bMoveGateSuccess == TRUE )
		{
			LogAddTD("[ RAKLION ][ BattleUser ] [%s][%s] MoveGate(%d)",
				gObj[this->m_vtUserInfo[iCount]].AccountID, 
				gObj[this->m_vtUserInfo[iCount]].Name, iGateNumber);
		}
		else
		{
			this->DeleteUserData(this->m_vtUserInfo[iCount]);

			LogAddTD( "[ RAKLION ][ BattleUser ] [%s][%s] MoveGate Fail (%d)",
				gObj[this->m_vtUserInfo[iCount]].AccountID, 
				gObj[this->m_vtUserInfo[iCount]].Name, iGateNumber);

			gObjMoveGate(this->m_vtUserInfo[iCount], 286); //Raklion Entrance
		}
	}

	return TRUE;
}


//0060FA30
int CRaklionBattleUserMng::GetBattleUserCount() //00645070 (identical)
{
	return this->m_vtUserInfo.size();
}

//0060FA60
int CRaklionBattleUserMng::CheckBattleUsers() //006450A0 (identical)
{
	//back = wrong
	//begin = wrong
	//end = maybe wrong
	//front = wrong
	//clear = wrong
	//capacity = wrong
	return (BYTE)this->m_vtUserInfo.empty();
}

//0060FA90
void CRaklionBattleUserMng::SetBattleUserIndex(int arg1) //006450D0 (identical) 
{
#if (DEBUG_RAKLION == 1)
	this->m_vtUserInfo.push_back(arg1);	//
#endif
}

//0060FAB0
int CRaklionBattleUserMng::GetBattleUserIndex(int iIndex) //006450F0 (identical)
{
	int UserIndex = this->m_vtUserInfo[iIndex];
	return UserIndex;
}

//0060FAF0
BOOL CRaklionBattleUserMng::IsBattleUser(int iIndex) //00645130 (identical)
{
	for(int i = 0; i < this->GetBattleUserCount(); i++)
	{
		if(iIndex == this->m_vtUserInfo[i])
		{
			return TRUE;
		}
	}

	return FALSE;
}

//2 new function here but it's used by the vector (generated automatically :D)