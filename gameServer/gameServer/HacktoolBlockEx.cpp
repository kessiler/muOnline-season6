//-> Decompiled by DarkSim | 02.09.2013 | 1.01.03 GS-N
// -------------------------------------------------------------------------------

#include "StdAfx.h"
#include "user.h"
#include "HacktoolBlockEx.h"
#include "logproc.h"
#include "..\common\winutil.h"
#include "GameMain.h"
#include "BuffEffectSlot.h"
#include "NewPVP.h"
#include "BloodCastle.h"
#include "IllusionTempleEvent.h"
#include "ChaosCastle.h"
#include "KalimaGate.h"
#include "DevilSquare.h"
// -------------------------------------------------------------------------------

int g_iHacktoolUsePenaltyExp;
int g_iHacktoolUsePenaltyZen;
int g_iHacktoolUsePenaltyItemDropRate;
int g_iHacktoolUsePenaltyDuration;
int g_iHacktoolCheckTerm;
// -------------------------------------------------------------------------------

CHacktoolBlockEx g_HacktoolBlockEx;
// -------------------------------------------------------------------------------

CHacktoolBlockEx::CHacktoolBlockEx()
{
	this->Initialize();
}
// -------------------------------------------------------------------------------

CHacktoolBlockEx::~CHacktoolBlockEx()
{
	//Unfilled
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::Initialize()
{
	for( int i = 0; i < HACKTOOL_DATA_SIZE; i++ )
	{
		this->m_HacktoolData[i].Clear();
	}
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::LoadScript(char * pchFilename)
{
	g_iHacktoolUsePenaltyExp			= GetPrivateProfileInt("GameServerInfo", "HacktoolUsePenaltyExp", 0, pchFilename);
	g_iHacktoolUsePenaltyZen			= GetPrivateProfileInt("GameServerInfo", "HacktoolUsePenaltyZen", 0, pchFilename);
	g_iHacktoolUsePenaltyItemDropRate	= GetPrivateProfileInt("GameServerInfo", "HacktoolUsePenaltyItemDropRate", 0, pchFilename);
	g_iHacktoolUsePenaltyDuration		= GetPrivateProfileInt("GameServerInfo", "HacktoolUsePenaltyDuration", -10, pchFilename);
	g_iHacktoolCheckTerm				= GetPrivateProfileInt("GameServerInfo", "HacktoolCheckTerm", 0, pchFilename);
}
// -------------------------------------------------------------------------------

bool CHacktoolBlockEx::AddHacktoolData(WORD wHacktoolIndex, char * pchFilename, long lDuration, BYTE btBlockType)
{
	for( int i = 0; i < HACKTOOL_DATA_SIZE; i++ )
	{
		if( !this->m_HacktoolData[i].bIsUsed )
		{
			this->m_HacktoolData[i].bIsUsed			= 1;
			this->m_HacktoolData[i].wIndex			= wHacktoolIndex;
			this->m_HacktoolData[i].lCheckDuration	= lDuration;
			this->m_HacktoolData[i].btHackBlockType = btBlockType;
			this->m_HacktoolData[i].btHackCheckType = 1;
			memcpy(this->m_HacktoolData[i].chFilename, pchFilename, 64);
			// ----
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::RemoveHacktoolData(WORD wHacktoolIndex, char * pchFilename)
{
	for( int i = 0; i < HACKTOOL_DATA_SIZE; i++ )
	{
		if(		this->m_HacktoolData[i].bIsUsed == 1 
			&&	this->m_HacktoolData[i].wIndex	== wHacktoolIndex )
		{
			if( !strcmp(this->m_HacktoolData[i].chFilename, pchFilename) )
			{
				LogAddTD("[HacktoolBlockEx][Remove Hacktool Data] Remove Data Name : %s, Index : %d, CheckType : %d, BlockType : %d",
					this->m_HacktoolData[i].chFilename, this->m_HacktoolData[i].wIndex,
					this->m_HacktoolData[i].btHackCheckType, this->m_HacktoolData[i].btHackBlockType);
				// ----
				this->m_HacktoolData[i].Clear();
			}
		}
	}
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::ModifyHacktoolData()
{
	//Unfilled
}
// -------------------------------------------------------------------------------

HACKTOOL_DATA * CHacktoolBlockEx::SearchHacktoolData(WORD wHacktoolIndex)
{
	for( int i = 0; i < HACKTOOL_DATA_SIZE; i++ )
	{
		if(		this->m_HacktoolData[i].bIsUsed == 1 
			&&	this->m_HacktoolData[i].wIndex	== wHacktoolIndex )
		{
			return &this->m_HacktoolData[i];
		}
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::OnRequestHacktoolStatistics(PMSG_HACKTOOL_CHECK_USE_STATISTICS_REQ * lpMsg)
{
	PMSG_REQ_HACKTOOL_STATISTICS pMsg				= { 0 };
	PMSG_HACKTOOL_CHECK_USE_STATISTISC_ANS pResult	= { 0 };
	HACKTOOL_DATA * lpHacktoolData					= 0;
	// ----
	lpHacktoolData = this->SearchHacktoolData(lpMsg->wHacktoolIndex);
	// ----
	if( lpHacktoolData )
	{
		lpHacktoolData->btHackCheckType = 2;
	}
	else if( this->AddHacktoolData(lpMsg->wHacktoolIndex, lpMsg->chFilename, lpMsg->lExpireDate, lpMsg->btBlockType) )
	{
		lpHacktoolData = this->SearchHacktoolData(lpMsg->wHacktoolIndex);
		lpHacktoolData->btHackCheckType = 2;
	}
	else
	{
		PHeadSubSetB((LPBYTE)&pResult, 0x8D, 1, sizeof(PMSG_HACKTOOL_CHECK_USE_STATISTISC_ANS));
		// ----
		pResult.btResult			= 1;
		pResult.wHacktoolIndex		= lpMsg->wHacktoolIndex;
		pResult.wHacktoolUseCount	= 0;
		pResult.wTotalUserCount		= 0;
		// ----
		wsJServerCli.DataSend((char*)&pResult, pResult.head.size);
	}
	// ----
	pMsg.h.setE((LPBYTE)&pMsg, 0x8A, sizeof(PMSG_REQ_HACKTOOL_STATISTICS));
	pMsg.dwKey = lpMsg->wHacktoolIndex;
	memcpy(pMsg.Filename, lpMsg->chFilename, 64);
	// ----
	for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
	{
		if( gObj[i].Connected >= PLAYER_LOGGED && gObj[i].Type == OBJ_USER )
		{
			for( int iSlotIndex = 0; iSlotIndex < HACKTOOL_DATA_SIZE; iSlotIndex++ )
			{
				if( !gObj[i].m_HacktoolUseInfo[iSlotIndex].btUseType )
				{
					gObj[i].m_HacktoolUseInfo[iSlotIndex].Set(0, 1, lpMsg->wHacktoolIndex);
					break;
				}
			}
		}
	}
	// ----
	DataSendAll((LPBYTE)&pMsg, sizeof(PMSG_REQ_HACKTOOL_STATISTICS));
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::OnRequestHacktoolBlock(PMSG_HACKTOOL_BLOCK_REQ * lpMsg)
{
	if( this->AddHacktoolData(lpMsg->wHacktoolIndex, lpMsg->chFilename, 
		lpMsg->lExpireDate, lpMsg->btBlockType) )
	{
		return;
	}
	// ----
	PMSG_HACKTOOL_BLOCK_ANS pResult;
	pResult.head.set((LPBYTE)&pResult, 0x8D, 2, sizeof(PMSG_HACKTOOL_BLOCK_ANS));
	pResult.btResult = 1;
	wsJServerCli.DataSend((char*)&pResult, pResult.head.size);
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::OnRequestHacktoolBlockRemove(PMSG_HACKTOOL_BLOCK_REMOVE_REQ * lpMsg)
{
	this->RemoveHacktoolData(lpMsg->wHacktoolIndex, lpMsg->chFilename);
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::OnRequestUserHacktoolUseInfo(LPOBJ lpObj, DWORD dwHacktoolIndex, BYTE btResult)
{
	HACKTOOL_DATA * lpHacktoolData;
	// ----
	for( int i = 0; i < HACKTOOL_DATA_SIZE; i++ )
	{
		if(	lpObj->m_HacktoolUseInfo[i].btCheckType	== 1 
			&&	lpObj->m_HacktoolUseInfo[i].wHacktoolIndex == dwHacktoolIndex )
		{
			if( btResult == 1 )
			{
				lpObj->m_HacktoolUseInfo[i].btUseType = 2;
				// ----
				if( this->SetHacktoolUseDisadvantage(lpObj, 2) )
				{
					GCServerMsgStringSend(lMsg.Get(3461), lpObj->m_Index, 1);
					CloseClient(lpObj->m_Index);
				}
				else if( this->SetHacktoolUseDisadvantage(lpObj, 4) )
				{
					GCServerMsgStringSend(lMsg.Get(3462), lpObj->m_Index, 1);
					this->SetUseForceMove2Town(lpObj);
				}
				else if( !this->SetHacktoolUseDisadvantage(lpObj, 8) && !this->SetHacktoolUseDisadvantage(lpObj, 16) )
				{
					if( this->SetHacktoolUseDisadvantage(lpObj, 32) )	//GM event?
					{
						GCServerMsgStringSend(lMsg.Get(3460), lpObj->m_Index, 1);
						gObjAddBuffEffect(lpObj, 120, 0, 0, 0, 0, g_iHacktoolUsePenaltyDuration);
					}
					else if( this->SetHacktoolUseDisadvantage(lpObj, 1) )
					{
						GCServerMsgStringSend(lMsg.Get(3463), lpObj->m_Index, 1);
					}
				}
				// ----
				lpHacktoolData = this->SearchHacktoolData(dwHacktoolIndex);
				// ----
				if( lpHacktoolData )
				{
					LogAddTD("[HackBlockEx][SetHackUseInfo] Use Hacktool User Id : %s(%d), Name : %s,  Hacktool Name : %s, Index : %d, BlockType : %d",
						lpObj->AccountID, lpObj->DBNumber, lpObj->Name,
						lpHacktoolData->chFilename, dwHacktoolIndex, lpHacktoolData->btHackBlockType);
				}
			}
			else
			{
				lpObj->m_HacktoolUseInfo[i].btUseType = 1;
				lpHacktoolData = this->SearchHacktoolData(dwHacktoolIndex);
			}
			break;
		}
	}
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::SetHacktoolDisadvantage()
{
	for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
	{
		if( gObj[i].Connected > PLAYER_LOGGED && gObj[i].Type == OBJ_USER )
		{
			for( int iHacktoolSlot = 0; iHacktoolSlot < HACKTOOL_DATA_SIZE; iHacktoolSlot++ )
			{
				//Unfilled
				//Very strange, maybe debug output
			}
		}
	}
}
// -------------------------------------------------------------------------------

bool CHacktoolBlockEx::SetHacktoolUseDisadvantage(LPOBJ lpObj, BYTE btBlockType)
{
	if( !lpObj )
	{
		return false;
	}
	// ----
	if( lpObj->Connected < PLAYER_LOGGED || lpObj->Type != OBJ_USER)
	{
		return false;
	}
	// ----
	for( int i = 0; i < HACKTOOL_DATA_SIZE; i++ )
	{
		if( lpObj->m_HacktoolUseInfo[i].btUseType == 2 )
		{
			HACKTOOL_DATA * lpHacktoolData = this->SearchHacktoolData(lpObj->m_HacktoolUseInfo[i].wHacktoolIndex);
			// ----
			if( lpHacktoolData )
			{
				if( (btBlockType & lpHacktoolData->btHackBlockType) == btBlockType )
				{
					return true;
				}
			}
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::SetUseForceMove2Town(LPOBJ lpObj)
{
	if( BC_MAP_RANGE(lpObj->MapNumber) )
	{
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(lpObj->MapNumber);
		// ----
		if( g_BloodCastle.GetCurrentState(iBridgeIndex) == BC_STATE_PLAYING )
		{
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		else
		{
			g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
		}
	}
	// ----
	if( IT_MAP_RANGE(lpObj->MapNumber) )
	{
		g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
	}
	// ----
	if( (lpObj->m_IfState.use) != 0 )
	{
		if( lpObj->m_IfState.type  == 3 )
		{
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.type	= 0;
			lpObj->m_IfState.use	= 0;
		}
	}
	// ----
	if( lpObj->MapNumber == MAP_INDEX_DEVIAS )
	{
		gObjMoveGate(lpObj->m_Index, 22);
	}
	else if( lpObj->MapNumber == MAP_INDEX_NORIA )
	{
		gObjMoveGate(lpObj->m_Index, 27);
	}
	else if( lpObj->MapNumber == MAP_INDEX_LOSTTOWER )
	{
		gObjMoveGate(lpObj->m_Index, 42);
	}
	else if( lpObj->MapNumber == MAP_INDEX_ATHLANSE )
	{
		gObjMoveGate(lpObj->m_Index, 49);
	}
	else if( lpObj->MapNumber == MAP_INDEX_TARKAN )
	{
		gObjMoveGate(lpObj->m_Index, 57);
	}
	else if( BC_MAP_RANGE(lpObj->MapNumber) )
	{
		gObjMoveGate(lpObj->m_Index, 22);
	}
	else if( CC_MAP_RANGE(lpObj->MapNumber) )
	{
		gObjMoveGate(lpObj->m_Index, 22);
	}
	else if( KALIMA_MAP_RANGE(lpObj->MapNumber) )
	{
		gObjMoveGate(lpObj->m_Index, 22);
	}
	else if( CC_MAP_RANGE(lpObj->MapNumber) )
	{
		gObjMoveGate(lpObj->m_Index, 22);
	}
	else if( lpObj->MapNumber == MAP_INDEX_AIDA )
	{
		gObjMoveGate(lpObj->m_Index, 27);
	}
	else if( lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
	{
		gObjMoveGate(lpObj->m_Index, 27);
	}
	else if( lpObj->MapNumber == MAP_INDEX_ELBELAND )
	{
		gObjMoveGate(lpObj->m_Index, 267);
	}
	else if( lpObj->MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS )
	{
		gObjMoveGate(lpObj->m_Index, 273);
	}
	else if( lpObj->MapNumber == MAP_INDEX_RAKLION_FIELD )
	{
		gObjMoveGate(lpObj->m_Index, 286);
	}
	else if( g_NewPVP.IsVulcanusMap(lpObj->MapNumber) )
	{
		gObjMoveGate(lpObj->m_Index, 294);
	}
	else if( g_NewPVP.IsPKFieldMap(lpObj->MapNumber) )
	{
		g_NewPVP.Reset(*lpObj);
		gObjMoveGate(lpObj->m_Index, 294);
	}
	else
	{
		gObjMoveGate(lpObj->m_Index, 17);
	}
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::SetUseForceMove2GMMap(LPOBJ lpObj)
{
	//Unfilled
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::CheckHacktoolUseConnectedUser(LPOBJ lpObj)
{
	if( !lpObj )
	{
		return;
	}
	// ----
	if( lpObj->Connected < PLAYER_LOGGED || lpObj->Type != OBJ_USER)
	{
		return;
	}
	// ----
	PMSG_REQ_HACKTOOL_STATISTICS pMsg = { 0 };
	pMsg.h.setE((LPBYTE)&pMsg, 0x8A, sizeof(PMSG_REQ_HACKTOOL_STATISTICS));
	// ----
	for( int i = 0; i < HACKTOOL_DATA_SIZE; i++ )
	{
		if( this->m_HacktoolData[i].bIsUsed )
		{
			lpObj->m_dwHacktoolCheckTerm				= GetTickCount();
			lpObj->m_HacktoolUseInfo[i].btCheckType		= 1;
			lpObj->m_HacktoolUseInfo[i].btUseType		= 0;
			lpObj->m_HacktoolUseInfo[i].wHacktoolIndex	= this->m_HacktoolData[i].wIndex;
		}
	}
}
// -------------------------------------------------------------------------------

void CHacktoolBlockEx::CheckHacktoolUse()
{
	PMSG_REQ_HACKTOOL_STATISTICS pMsg = { 0 };
	pMsg.h.setE((LPBYTE)&pMsg, 0x8A, sizeof(PMSG_REQ_HACKTOOL_STATISTICS));
	// ----
	for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
	{
		if(		gObj[i].Type == OBJ_USER
			&&	gObj[i].Connected >= PLAYER_LOGGED )
		{
			if( (g_iHacktoolCheckTerm + gObj[i].m_dwHacktoolCheckTerm) <= GetTickCount() )
			{
				gObj[i].m_dwHacktoolCheckTerm = GetTickCount();
				// ----
				for( int iHacktoolIndex = 0; iHacktoolIndex < HACKTOOL_DATA_SIZE; iHacktoolIndex++ )
				{
					if( this->m_HacktoolData[iHacktoolIndex].bIsUsed )
					{
						pMsg.dwKey = this->m_HacktoolData[iHacktoolIndex].wIndex;
						memcpy(pMsg.Filename, this->m_HacktoolData[iHacktoolIndex].chFilename, 0x40);
						DataSend(i, (LPBYTE)&pMsg, sizeof(PMSG_REQ_HACKTOOL_STATISTICS));
					}
				}
			}
		}
	}
}
// -------------------------------------------------------------------------------

void HACKTOOL_DATA::Clear()
{
	this->bIsUsed			= 0;
	this->wIndex			= 0;
	this->btHackCheckType	= 0;
	this->btHackBlockType	= 0;
	this->lCheckDuration	= 0;
}
// -------------------------------------------------------------------------------

void HACKTOOL_USE_INFO::Set(BYTE UseType, BYTE CheckType, WORD HacktoolIndex)
{
	this->btUseType			= UseType;
	this->btCheckType		= CheckType;
	this->wHacktoolIndex	= HacktoolIndex;
}
// -------------------------------------------------------------------------------