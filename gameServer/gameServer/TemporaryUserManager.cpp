#include "stdafx.h"
#include "TemporaryUserManager.h"
#include "GameMain.h"
#include "ImperialGuardian.h"
#include "BloodCastle.h"
#include "IllusionTempleEvent.h"
#include "DevilSquare.h"
#include "DBSockMng.h"
#include "..\\Common\\Winutil.h"
#include "logproc.h"
#include "Doppelganger.h"
// -------------------------------------------------------------------------

CTemporaryUserManager * CTemporaryUserManager::m_pInstance = NULL;
// -------------------------------------------------------------------------

//00554b60	-> 100%
int CTemporaryUserManager::SearchEmptyIndex() const	//static?
{
	for( int i = 0; i < MAX_TEMP_SLOT; i++ )
	{
		if( !this->m_UserData[i].UserIndex )
		{
			return i;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

//00554bc0	-> 100%
void CTemporaryUserManager::ClearSpecificUserData(int SlotIndex)
{
	if( !IS_TEMP_SLOT(SlotIndex) )
	{
		return;
	}
	// ----
	if( OBJMAX_RANGE(m_UserData[SlotIndex].UserIndex) )
	{
		gObj[this->m_UserData[SlotIndex].UserIndex].Connected = 0;
		memset(&m_UserData[SlotIndex], 0, sizeof(m_UserData[SlotIndex]));
	}
}
// -------------------------------------------------------------------------

//00554ca0	-> 100%
bool CTemporaryUserManager::AddUserData(const char * Name, int UserIndex, int PartyNumber)	//OK
{
	EnterCriticalSection(&m_cs);
	int iSlot = SearchEmptyIndex();
	LeaveCriticalSection(&m_cs);
	// ----
	if( iSlot == -1 )
	{
		return false;
	}
	// ----
	this->m_UserData[iSlot].Tick					= GetTickCount();
	memcpy(this->m_UserData[iSlot].Name, Name, sizeof(m_UserData[iSlot].Name));
	this->m_UserData[iSlot].UserIndex				= UserIndex;
	this->m_UserData[iSlot].Change					= gObj[UserIndex].m_Change;
	this->m_UserData[iSlot].PartyNumber				= PartyNumber;
	this->m_UserData[iSlot].MapNumber				= gObj[UserIndex].MapNumber;
	this->m_UserData[iSlot].iIllusionTempleIndex	= gObj[UserIndex].m_iIllusionTempleIndex;
	this->m_UserData[iSlot].ImperialGuardianIndex	= g_ImperialGuardian.GetCurrentZoneIndex(UserIndex);
	this->m_UserData[iSlot].BloodCastleIndex		= gObj[UserIndex].m_cBloodCastleIndex;
	this->m_UserData[iSlot].cBloodCastleSubIndex	= gObj[UserIndex].m_cBloodCastleSubIndex;
	this->m_UserData[iSlot].iBloodCastleEXP			= gObj[UserIndex].m_iBloodCastleEXP;
	this->m_UserData[iSlot].EventScore				= gObj[UserIndex].m_nEventScore;
	this->m_UserData[iSlot].EventExp				= gObj[UserIndex].m_nEventExp;
	this->m_UserData[iSlot].EventMoney				= gObj[UserIndex].m_nEventMoney;
	this->m_UserData[iSlot].bDevilSquareIndex		= gObj[UserIndex].m_bDevilSquareIndex;
	this->m_UserData[iSlot].bDevilSquareAuth		= gObj[UserIndex].m_bDevilSquareAuth;
	// ----
	return true;
}
// -------------------------------------------------------------------------

//00554f70	-> 100%
int CTemporaryUserManager::AddToPriviousParty(int UserIndex, int SlotIndex)	//OK
{
	if( !IS_TEMP_SLOT(SlotIndex) )
	{
		return false;
	}
	// ----
    if( strcmp(this->m_UserData[SlotIndex].Name, gObj[UserIndex].Name) )
    {
		return false;
    }
	// ----
	gParty.Delete(m_UserData[SlotIndex].PartyNumber, m_UserData[SlotIndex].UserIndex, gObj[UserIndex].DBNumber);
	// ----
	if( gParty.Add(m_UserData[SlotIndex].PartyNumber,UserIndex, gObj[UserIndex].DBNumber, gObj[UserIndex].Level) >= 0 )
	{
		gObj[UserIndex].PartyNumber = this->m_UserData[SlotIndex].PartyNumber;
		CGPartyListAll(this->m_UserData[SlotIndex].PartyNumber);
		gParty.PartyLog(this->m_UserData[SlotIndex].PartyNumber, UserIndex, 2);
	}
	else
	{
		//GCServerMsgStringSend("¦-¦- -áLL -¦¦¦ LL+¦+-.", UserIndex, 1);
	}
	// ----
	ClearSpecificUserData(SlotIndex);
	// ----
	return true;
}
// -------------------------------------------------------------------------

//00555150	-> 100%
int CTemporaryUserManager::ClearOverTimeUserData(int Second)	//OK
{
	DWORD dwCurrentTickCount = GetTickCount();
	// ----
	for( int i = 0; i < MAX_TEMP_SLOT; i++ )
	{
		if( !this->m_UserData[i].UserIndex )
		{
			continue;
		}
		// ----
		if( dwCurrentTickCount - this->m_UserData[i].Tick <= 1000 * Second )
		{
			continue;
		}
		// ----
		int PartyNumber = this->m_UserData[i].PartyNumber;
		// ----
		if( OBJMAX_RANGE(PartyNumber) )
		{
			if( CheckInEventMap(i)		== TEMP_NONE 
				|| CheckInEventMap(i)	== TEMP_BLOODCASTLE 
				|| CheckInEventMap(i)	== TEMP_DEVILSQUARE )
			{
				gParty.Delete(PartyNumber, m_UserData[i].UserIndex, gObj[m_UserData[i].UserIndex].DBNumber);
				gParty.UpdatePKPartyPanalty(PartyNumber);
				CGPartyListAll(PartyNumber);
				// ----
				if( gParty.GetCount(PartyNumber) >= 2 )
				{
					continue;
				}
				// ----
				for( int n = 0; n < MAX_USER_IN_PARTY; n++ )
				{
					int number = gParty.m_PartyS[PartyNumber].Number[n];
					// ----
					if( number >= 0 )
					{
						gParty.Delete(PartyNumber, n);
						gObj[number].PartyNumber		= -1;
						gObj[number].PartyTargetUser	= -1;
						GCPartyDelUserSend(number);
					}
				}
				// ----
				gParty.Destroy(PartyNumber);
			}
			// ----
			TidyUpTempuserdate(i);
		}
		else
		{
			TidyUpTempuserdate(i);
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------

//005553d0	-> 100%
int CTemporaryUserManager::TidyUpTempuserdate(int SlotIndex)	//OK
{
	if( !IS_TEMP_SLOT(SlotIndex) )
	{
		return -1;
	}	
	// ----
    int nPartyNumber = this->m_UserData[SlotIndex].PartyNumber;
	// ----
	TEMPORARY_SET_EXGAMESERVERCODE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xA6, sizeof(TEMPORARY_SET_EXGAMESERVERCODE));
	memcpy(pMsg.szCharName, this->m_UserData[SlotIndex].Name, 11);
	pMsg.sExGameServerCode = -1;
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
	// ----
    LeaveEvent(SlotIndex);
    ClearSpecificUserData(SlotIndex);
	// ----
	return nPartyNumber;
}
// -------------------------------------------------------------------------

//005554b0	-> 100%
void CTemporaryUserManager::DeleteUserInParty(int SlotIndex)	//Need connect with DS, good
{
	if( !IS_TEMP_SLOT(SlotIndex) )
	{
		return;
	}	
	// ----
    int nPartyNumber = this->m_UserData[SlotIndex].PartyNumber;
	// ----
    if( !OBJMAX_RANGE(nPartyNumber) )
    {
		return;
	}
	// ----
	gParty.Delete(nPartyNumber, m_UserData[SlotIndex].UserIndex, gObj[m_UserData[SlotIndex].UserIndex].DBNumber);
	gParty.UpdatePKPartyPanalty(nPartyNumber);
	// ----
    CGPartyListAll(nPartyNumber);
	// ----
    if( gParty.GetCount(nPartyNumber) >= 2 )
    {
		return;
	}
	// ----
	for( int n = 0; n < 5; n++ )
	{
		int number = gParty.m_PartyS[nPartyNumber].Number[n];
		// ----
		if( number >= 0 )
		{
			gParty.Delete(nPartyNumber, n);
			gObj[number].PartyNumber		= -1;
			gObj[number].PartyTargetUser	= -1;
			GCPartyDelUserSend(number);
		}
	}
	// ----
	gParty.Destroy(nPartyNumber);
    TidyUpTempuserdate(SlotIndex);
}
// -------------------------------------------------------------------------

//00555690	-> 100%
int CTemporaryUserManager::DeleteAllPartyUserData(int PartyNumber)	//OK
{
    if( !OBJMAX_RANGE(PartyNumber) )
    {
		return false;
	}
	// ----
	for( int i = 0; i < MAX_TEMP_SLOT; i++ )
	{
		if( this->m_UserData[i].PartyNumber == PartyNumber )
		{
			this->TidyUpTempuserdate(i);
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------

//00555720	-> 100%
int CTemporaryUserManager::CheckMatchCharacter(const char * CharacterName)
{
	if( CharacterName == NULL)
	{
		return -1;
	}
	// ----
    for( int i = 0; i < MAX_TEMP_SLOT; i++ )
    {
		if( this->m_UserData[i].UserIndex && !strcmp(CharacterName, this->m_UserData[i].Name) )
		{
			return i;
		}
    }
	// ----
	return -1;
}
// -------------------------------------------------------------------------

//005557b0	-> 100%
int CTemporaryUserManager::CheckMatchIndex(int UserIndex)
{
	for( int i = 0; i < MAX_TEMP_SLOT; i++ )
	{
		if( this->m_UserData[i].UserIndex == UserIndex )
		{
			return i;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

//00555810	-> 100%
int CTemporaryUserManager::CheckInEventMap(int SlotIndex)	//
{
	if( !IS_TEMP_SLOT(SlotIndex) )
	{
		return -1;
	}
	// ----
    if( BC_MAP_RANGE(this->m_UserData[SlotIndex].MapNumber) )
    {
      return TEMP_BLOODCASTLE;
    }
	// ----
    if( IT_MAP_RANGE(m_UserData[SlotIndex].MapNumber) )
	{
		return TEMP_ILLUSION;
	}
	// ----
    if( this->m_UserData[SlotIndex].MapNumber >= 69 && this->m_UserData[SlotIndex].MapNumber <= 72 )	//-> Need define
	{
		return TEMP_IMPERIAL;
	}
	// ----
	if( this->m_UserData[SlotIndex].MapNumber >= 65 && this->m_UserData[SlotIndex].MapNumber <= 68 )	//-> Need define
	{
		return TEMP_DOPPEL;
	}
	// ----
    if( DS_MAP_RANGE(m_UserData[SlotIndex].MapNumber) )
	{
		return TEMP_DEVILSQUARE;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------

//00555a50	-> 
void CTemporaryUserManager::LeaveEvent(int SlotIndex)
{
  int bResult = CheckInEventMap(SlotIndex);
  // ----
  if( bResult == TEMP_BLOODCASTLE )
  {
	  return;
  }
  else if( bResult == TEMP_ILLUSION )
  {
	  g_IllusionTempleEvent.LeaveIllusionTemple(this->m_UserData[SlotIndex].UserIndex, this->m_UserData[SlotIndex].MapNumber);
  }
  else if( bResult == TEMP_IMPERIAL )
  {
	  return;
  }
  else if( bResult == TEMP_DOPPEL )
  {
	  g_DoppleganerEvent.LeaveDoppelgangerEvent(this->m_UserData[SlotIndex].UserIndex);
  }
  else if( bResult == TEMP_DEVILSQUARE )
  {
	//Need complete ground delete
	  return;	//g_DevilSquare.LeaveDevilSquare(this->m_UserData[SlotIndex].bDevilSquareIndex, this->m_UserData[SlotIndex].UserIndex);
  }
}
// -------------------------------------------------------------------------

//00555b30	-> 100%
int CTemporaryUserManager::ResumeEvent(int SlotIndex, int CurUserIndex)
{
	int bResult = IS_TEMP_SLOT(SlotIndex);	//ebp-0c?
	// ----
	if( !bResult )
	{
		return false;
	}
	// ----
	switch(CheckInEventMap(SlotIndex))
	{
	case TEMP_BLOODCASTLE:
		{
			return this->ResumeBloodCastle(SlotIndex, CurUserIndex);
		}
		break;
		// --
	case TEMP_ILLUSION:
		{
			return this->ResumeIllusionTemple(SlotIndex, CurUserIndex);
		}
		break;
		// --
	case TEMP_IMPERIAL:
		{
			return this->ResumeImperialguardian(SlotIndex, CurUserIndex);
		}
		break;
		// --
	case TEMP_DOPPEL:
		{
			return this->ResumeDoppelganger(SlotIndex, CurUserIndex);
		}
		break;
		// --
	case TEMP_DEVILSQUARE:
		{
			return this->ResumeDevilsquare(SlotIndex, CurUserIndex);
		}
		break;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------

//00555c30	->
int CTemporaryUserManager::ResumeIllusionTemple(int SlotIndex, int CurUserIndex)
{
	int bResult = IS_TEMP_SLOT(SlotIndex);
	// ----
	if( !bResult )
	{
		return false;
	}
	// ----
	if( g_IllusionTempleEvent.GetIllusionTempleState(this->m_UserData[SlotIndex].MapNumber) == 3 )
	{
		return false;
	}
	// ----
	//if( !g_IllusionTempleEvent.ChangeTempleUserInformation(...) )
	//{
	//	return false;
	//}
	// ----
	gObj[CurUserIndex].m_iIllusionTempleIndex = this->m_UserData[SlotIndex].iIllusionTempleIndex;
	// ----
	if( !g_IllusionTempleEvent.GetIllusionTempleState(this->m_UserData[SlotIndex].MapNumber) )
	{
		return false;
	}
	// ----
	PMSG_ILLUSION_TEMPLE_STATE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));
	// ----
	pMsg.btTempleNumber			= this->m_UserData[SlotIndex].iIllusionTempleIndex;	//-> Need check
	pMsg.btIllusionTempleState	= 1;
	// ----
	DataSend(CurUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
	// ----
	return true;
}
// -------------------------------------------------------------------------

//00555e40	-> 100%
int CTemporaryUserManager::ResumeImperialguardian(int SlotIndex, int CurUserIndex)
{
	int bResult = IS_TEMP_SLOT(SlotIndex);
	// ----
	if( !bResult )
	{
		return false;
	}
	// ----
	bResult = g_ImperialGuardian.GetPlayUserCountRightNow(this->m_UserData[SlotIndex].ImperialGuardianIndex);
	// ----
	if( bResult < 1 )
	{
		return false;
	}
	// ----
	bResult = g_ImperialGuardian.ChangeUserIndex(this->m_UserData[SlotIndex].UserIndex, CurUserIndex, this->m_UserData[SlotIndex].ImperialGuardianIndex);
	// ----
	return bResult;
}
// -------------------------------------------------------------------------

//00555f00	-> 100%
int CTemporaryUserManager::ResumeBloodCastle(int SlotIndex, int CurUserIndex)
{
	int bResult = IS_TEMP_SLOT(SlotIndex);
	// ----
	if( !bResult )
	{
		return false;
	}
	// ----
	if( g_BloodCastle.GetPlayUserCountRightNow(this->m_UserData[SlotIndex].BloodCastleIndex) < 1 )
	{
		return false;
	}
	// -----
	if( g_BloodCastle.ChangeUserIndex(this->m_UserData[SlotIndex].UserIndex, CurUserIndex, this->m_UserData[SlotIndex].BloodCastleIndex) )
	{
		gObj[CurUserIndex].m_cBloodCastleIndex		= this->m_UserData[SlotIndex].BloodCastleIndex;
		gObj[CurUserIndex].m_cBloodCastleSubIndex	= this->m_UserData[SlotIndex].cBloodCastleSubIndex;
		gObj[CurUserIndex].m_iBloodCastleEXP		= this->m_UserData[SlotIndex].iBloodCastleEXP;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------

//00556050
int CTemporaryUserManager::ResumeDoppelganger(int SlotIndex, int CurUserIndex)
{
	int bResult = IS_TEMP_SLOT(SlotIndex);
	// ----
	if( !bResult )
	{
		return false;
	}
	// ----
	bResult = g_DoppleganerEvent.GetDoppelgangerState();
	// ----
 	if( bResult == 3 )
 	{
 		return false;
 	}
 	// ----
	bResult = g_DoppleganerEvent.GetPlayUserCountRightNow();
	// ----
 	if( bResult < 1 )
 	{
 		return false;
 	}
	// ----
	bResult = g_DoppleganerEvent.ChangeUserIndex(this->m_UserData[SlotIndex].UserIndex, CurUserIndex);
	// ----
	return bResult;
}
// -------------------------------------------------------------------------

//00556110
int CTemporaryUserManager::ResumeDevilsquare(int SlotIndex, int CurUserIndex)
{
	return 0;
}
// -------------------------------------------------------------------------

//005562d0	-> 100%
void CTemporaryUserManager::SendEventMapInfo(int SlotIndex, int CurUserIndex)
{
	int bResult = this->CheckInEventMap(SlotIndex);
	// -----
	if( bResult == TEMP_ILLUSION )
	{
		/*_ILLUSIONTEMPLE_BARRIER pMsg;
		// ----
		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));
		// ----
		pMsg.m_btFloorIndex = this->m_TempUserMngInfo[SlotIndex].m_ITempleIndex;
		BYTE ITState		= this->m_TempUserMngInfo[SlotIndex].m_MapNumber;
		pMsg.m_btState		= 0;//g_IllusionTempleEvent.GetIllusionTempleState(ITState);
		// -----
		DataSend(CurUserIndex, (LPBYTE)&pMsg, pMsg.h.size);*/
		return;
	}
	else if( bResult == TEMP_BLOODCASTLE )
	{
		bResult = g_BloodCastle.GetPlayUserCountRightNow(this->m_UserData[SlotIndex].BloodCastleIndex);
		// ----
		if( bResult )
		{
			bResult = g_BloodCastle.GetCastleBlockInfo(this->m_UserData[SlotIndex].BloodCastleIndex);
			// ----
			if( bResult == 1003 )
			{
				g_BloodCastle.SendCastleDoorBlockInfo(this->m_UserData[SlotIndex].BloodCastleIndex, 0);
			}
			else if( bResult == 1002 )
			{
				g_BloodCastle.SendCastleBridgeBlockInfo(this->m_UserData[SlotIndex].BloodCastleIndex, 0);
			}
			else if( bResult == 1001 )
			{
				g_BloodCastle.SendCastleEntranceBlockInfo(this->m_UserData[SlotIndex].BloodCastleIndex, 0);
			}
			// ----
			bResult = g_BloodCastle.GetCurrentState(this->m_UserData[SlotIndex].BloodCastleIndex);
			// ----
			if (bResult == BC_STATE_PLAYING)
			{
				g_BloodCastle.SendNoticeMessageToSpecificUser(this->m_UserData[SlotIndex].BloodCastleIndex, CurUserIndex, 0);
			}
		}
	}
	else if( bResult == TEMP_DOPPEL )
	{
		bResult = g_DoppleganerEvent.GetPlayUserCountRightNow();
		// ----
		if( bResult < 1 )
		{
			this->LeaveEvent(SlotIndex);
		}
		// ----
		bResult = g_DoppleganerEvent.GetDoppelgangerState();
		// ----
		if( bResult >= 2 )
		{
			g_DoppleganerEvent.SendMapTileInfo(&gObj[CurUserIndex], 1);
			g_DoppleganerEvent.SendDoppelgangerStateToSpecificUser(CurUserIndex, 2);
		}
		else if( bResult >= 1 )
		{
			g_DoppleganerEvent.SendDoppelgangerStateToSpecificUser(CurUserIndex, 0);
			g_DoppleganerEvent.SendDoppelgangerStateToSpecificUser(CurUserIndex, 1);
		}
	}
	else if( bResult == TEMP_DEVILSQUARE )
	{
		return;
	}
	else if( bResult == TEMP_IMPERIAL )
	{
		bResult = g_ImperialGuardian.GetPlayUserCountRightNow(this->m_UserData[SlotIndex].ImperialGuardianIndex);
		// ----			
		if( bResult )
		{
			g_ImperialGuardian.SendCurStateToUser(CurUserIndex, this->m_UserData[SlotIndex].ImperialGuardianIndex);
			g_ImperialGuardian.SendCurGateInfoToUser(CurUserIndex, this->m_UserData[SlotIndex].ImperialGuardianIndex);
		}
	}
}
// -------------------------------------------------------------------------