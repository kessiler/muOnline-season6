//	GS-CS	1.00.90	JPN	0xXXXXXXXX	-	Completed
#include "stdafx.h"
#include "KalimaGate.h"
#include "LogProc.h"
#include "DevilSquare.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "illusiontempleevent.h"

#ifdef IMPERIAL
#include "ImperialGuardian.h"
#endif

CKalimaGate g_KalimaGate;

//00414A40 - identical
CKalimaGate::CKalimaGate()
{
	InitializeCriticalSection(&this->m_critKalimaGate );
}

//00414AD0 - identical
CKalimaGate::~CKalimaGate()
{
	DeleteCriticalSection(&this->m_critKalimaGate );
}

//00414B10 - identical
BOOL CKalimaGate::CreateKalimaGate(int iIndex, BYTE btLevel, BYTE cTX, BYTE cTY)
{
	BOOL bKalimaGateCreateSucceed=FALSE;
	int iMonsterIndex=-1;

	
	EnterCriticalSection(&this->m_critKalimaGate );
	
	__try
	{
		int iKalimaGateLevel;
		int iMapNumber;
		BYTE btMapAttr;
		int iMonsterType;
		BYTE cX;
		BYTE cY;

		
		if ( gObjIsConnected(iIndex) == PLAYER_EMPTY )
		{
			return false;
		}

		LogAddTD("[Kalima] [%s][%s] Try to Create Kalima Gate", gObj[iIndex].AccountID, gObj[iIndex].Name);
		iKalimaGateLevel = this->GetKalimaGateLevel(iIndex);

		if ( iKalimaGateLevel < btLevel-1 )
		{
			return false;
		}

		iKalimaGateLevel = btLevel-1;

		if (  DS_MAP_RANGE(gObj[iIndex].MapNumber) )
		{
			return false;
		}

		if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) )
		{
			return false;
		}

		if ( CC_MAP_RANGE(gObj[iIndex].MapNumber) )
		{
			return false;
		}

		if ( CHECK_ILLUSIONTEMPLE(gObj[iIndex].MapNumber) )//NEW
		{
			return false;
		}

#ifdef IMPERIAL
		if( CImperialGuardian::IsEventMap(gObj[iIndex].MapNumber) )
		{
			return false;
		}
#endif

		if ( KALIMA_FLOOR_RANGE(iKalimaGateLevel)== FALSE )
		{
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Gate Level Check Error : %d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, iKalimaGateLevel+1);
			return false;
		}

		iMapNumber = gObj[iIndex].MapNumber;

		btMapAttr = MapC[iMapNumber].GetAttr(gObj[iIndex].X, gObj[iIndex].Y);

		if ( btMapAttr &1 != 0 )
		{
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Called in Saftey Area (Map:%d, X:%d, Y:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber,
				gObj[iIndex].X, gObj[iIndex].Y);
			return false;
		}

		if ( gObj[iIndex].m_cKalimaGateExist == TRUE )
		{
			GCServerMsgStringSend(lMsg.Get( MSGGET( 4, 224)), iIndex, 1);
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Already Have Gate (SummonIndex:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_iKalimaGateIndex);
			return false;
		}

		iMonsterType = iKalimaGateLevel+152;
		cX = cTX;
		cY = cTY;

		btMapAttr = MapC[iMapNumber].GetAttr(cX, cY);

		if ( btMapAttr != 0 )
		{
			if (this->GetRandomLocation(iMapNumber, cX, cY) == 0)
			{
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Failed to get Location (MapNumber:%d, X:%d, Y:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber, cX, cY);
			return false;
			}
		}

		if ( gObj[iIndex].MapNumber == 10 )
		{
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate - Uable to Summon in this Map (MapNumber:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber);
			return false;
		}

		iMonsterIndex = gObjAddMonster(iMapNumber);

		if ( iMonsterIndex >= 0 )
		{
			LPMONSTER_ATTRIBUTE lpMATTR = gMAttr.GetAttr(iMonsterType);
			
			if ( lpMATTR == NULL )
			{
				gObjDel(iMonsterIndex);
				LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - lpMATTR == NULL (SummonIndex:%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iMonsterIndex);
				return false;
			}

			gObjSetMonster(iMonsterIndex, iMonsterType);
			gObj[iMonsterIndex].Live = TRUE;
			gObj[iMonsterIndex].Life = 1000.0;
			gObj[iMonsterIndex].MaxLife = 1000.0;
			gObj[iMonsterIndex].m_PosNum = -1;
			gObj[iMonsterIndex].X = cX;
			gObj[iMonsterIndex].Y = cY;
			gObj[iMonsterIndex].MTX = cX;
			gObj[iMonsterIndex].MTY = cY;
			gObj[iMonsterIndex].TX = cX;
			gObj[iMonsterIndex].TY = cY;
			gObj[iMonsterIndex].m_OldX = cX;
			gObj[iMonsterIndex].m_OldY = cY;
			gObj[iMonsterIndex].StartX = cX;
			gObj[iMonsterIndex].StartY = cY;
			gObj[iMonsterIndex].MapNumber = iMapNumber;
			gObj[iMonsterIndex].m_MoveRange = 0;
			gObj[iMonsterIndex].m_cKalimaGateEnterCount = 0;

			gObj[iIndex].m_cKalimaGateExist = TRUE;
			gObj[iIndex].m_iKalimaGateIndex = iMonsterIndex;

			gObj[iMonsterIndex].Level = lpMATTR->m_Level;
			gObj[iMonsterIndex].Type = OBJ_MONSTER;
			gObj[iMonsterIndex].MaxRegenTime = 1000;
			gObj[iMonsterIndex].Dir = 1;

			gObj[iMonsterIndex].m_RecallMon = iIndex;
			gObj[iMonsterIndex].RegenTime = GetTickCount();
			gObj[iMonsterIndex].m_Attribute = iKalimaGateLevel + 51;
			gObj[iMonsterIndex].DieRegen = FALSE;
			
			LogAddTD("[Kalima] [%s][%s] Create Kalima Gate Successfuly (Live:%d, GateTick:%u, DieRegen:%d, Map:%d, X:%d, Y:%d, SummonIndex:%d, OwnerIndex:%d, GateNo.:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iMonsterIndex].Live, gObj[iMonsterIndex].RegenTime,
				gObj[iMonsterIndex].DieRegen, gObj[iMonsterIndex].MapNumber, gObj[iMonsterIndex].X,
				gObj[iMonsterIndex].Y, iMonsterIndex, iIndex, iKalimaGateLevel+1);
			bKalimaGateCreateSucceed = TRUE;
		}
	}
	__finally
	{
		LeaveCriticalSection(&this->m_critKalimaGate);
	}

	return true;
}

//00415910 - identical
void CKalimaGate::KalimaGateAct(int iIndex)
{
	LPOBJ lpObj=NULL;
	LPOBJ lpObjCallOwner=NULL;

	if ( OBJMAX_RANGE(iIndex ) == FALSE)
	{
		return;
	}

	if ( gObj[iIndex].Live == FALSE )
	{
		return;
	}

	if ( gObjIsConnected( gObj[iIndex].m_RecallMon ) == PLAYER_EMPTY )
	{
		LogAddTD("[Kalima] Kalima Gate Vanished - Summoner Vanished (SummonIndex:%d, EnterCount:%d)",
			iIndex, gObj[iIndex].m_cKalimaGateEnterCount );
		GCDiePlayerSend( &gObj[iIndex], iIndex, 0, 0);
		gObjDel(iIndex);
		gObjCharZeroSet(iIndex);
		return;
	}

	lpObj = &gObj[iIndex];
	lpObjCallOwner = &gObj[lpObj->m_RecallMon];

	if ( gObj[gObj[iIndex].m_RecallMon].DieRegen > 0 )
	{
		lpObjCallOwner->AccountID[MAX_ACCOUNT_LEN]=0;
		lpObjCallOwner->Name[MAX_ACCOUNT_LEN]=0;

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Summoner Died (SummonIndex:%d, EnterCount:%d)",
			lpObjCallOwner->AccountID, lpObjCallOwner->Name,
			iIndex, gObj[iIndex].m_cKalimaGateEnterCount );
		this->DeleteKalimaGate(iIndex, gObj[iIndex].m_RecallMon);
		return;
	}

	if ( gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMA_ENTER )
	{
		lpObjCallOwner->AccountID[MAX_ACCOUNT_LEN]=0;
		lpObjCallOwner->Name[MAX_ACCOUNT_LEN]=0;
		
		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Enter Count Over (SummonIndex:%d, EnterCount:%d)",
			lpObjCallOwner->AccountID, lpObjCallOwner->Name,
			iIndex, gObj[iIndex].m_cKalimaGateEnterCount );
		this->DeleteKalimaGate(iIndex, gObj[iIndex].m_RecallMon);
		return;
	}
		
	DWORD dwTickCount = GetTickCount();

	if ( dwTickCount > lpObj->RegenTime + KALIMA_GATE_TIMEOUT )
	{
		lpObjCallOwner->AccountID[MAX_ACCOUNT_LEN]=0;
		lpObjCallOwner->Name[MAX_ACCOUNT_LEN]=0;
		
		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Time Out (SummonIndex:%d, EnterCount:%d)",
			lpObjCallOwner->AccountID, lpObjCallOwner->Name,
			iIndex, gObj[iIndex].m_cKalimaGateEnterCount );
		this->DeleteKalimaGate(lpObj->m_Index, lpObjCallOwner->m_Index);
		return;
	}

	if ( lpObj->VPCount < 1 )
	{
		return;
	}

	int tObjNum=-1;

	for ( int i=0;i<MAX_VIEWPORT_MONSTER;i++)
	{
		tObjNum = lpObj->VpPlayer[i].number;

		if ( tObjNum >= 0 )
		{
			if ( gObj[tObjNum].Type == OBJ_USER )
			{
				if ( gObj[tObjNum].Live != FALSE )
				{
					if ( abs(lpObj->Y - gObj[tObjNum].Y) < 2 )
					{
						if ( abs(lpObj->X - gObj[tObjNum].X) < 2 )
						{
							if ( lpObj->m_RecallMon == tObjNum || (gObj[tObjNum].PartyNumber != -1 && lpObjCallOwner->PartyNumber == gObj[tObjNum].PartyNumber ))
							{
								int iMoveMapLevel = lpObj->m_Attribute -51;
								int iMapNumber = iMoveMapLevel + 24;
								int iGateNumber = g_iKalimaGateGateIndex[iMoveMapLevel];
								int iOverLevel = this->GetKalimaGateLevel(tObjNum);

								if ( iOverLevel >= iMoveMapLevel )
								{
									if ( gObjMoveGate(tObjNum, iGateNumber) == FALSE )
									{
										LogAddTD("[Kalima] [%s][%s] - [%s][%s] move fail (MapNumber:%d)", 
											lpObjCallOwner->AccountID, lpObjCallOwner->Name,
											gObj[tObjNum].AccountID, gObj[tObjNum].Name, iMapNumber);
									}
									else
									{
										LogAddTD("[Kalima] [%s][%s] - [%s][%s] Transported by Kalima Gate (SummonIndex:%d, GateNo.:%d, MapNumber:%d)",
											lpObjCallOwner->AccountID, lpObjCallOwner->Name, 
											gObj[tObjNum].AccountID, gObj[tObjNum].Name, iIndex, iMoveMapLevel + 1, iMapNumber);
										gObj[iIndex].m_cKalimaGateEnterCount ++;

										if ( gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMA_ENTER )
										{
											return;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//00415FC0 - identical
int CKalimaGate::CheckOverlapKundunMark(int iIndex, BYTE btLevel)
{
	for ( int x = 0;x<MAIN_INVENTORY_SIZE;x++ )
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14, 29) )
			{
				if ( gObj[iIndex].pInventory[x].m_Level == btLevel )
				{
					int iITEM_DUR = gObj[iIndex].pInventory[x].m_Durability;

					if ( KUNDUN_MARK_RANGE(iITEM_DUR) != FALSE )
					{
						return x;
					}
				}
			}
		}
	}

	return -1;
}

//00416120 - identical
BOOL CKalimaGate::GetRandomLocation(int iMapNumber, BYTE & cX, BYTE & cY)
{
	int iCount = 100;
	int iUX = cX;
	int iUY = cY;

	while ( iCount-- > 0 )
	{
		cX = ( rand() %6) * (((rand()%2==0)?-1:1)) + iUX;
		cY = (rand()%6) * (((rand()%2==0)?-1:1)) + iUY;

		BYTE btMapAttr = MapC[iMapNumber].GetAttr(cX,cY);

		if ( btMapAttr == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}
	
//00416250 - identical
int CKalimaGate::GetKalimaGateLevel(int iIndex)
{
	if ( gObj[iIndex].Class == CLASS_MAGUMSA 
#ifdef MONK
		|| gObj[iIndex].Class == CLASS_MONK
#endif
		|| gObj[iIndex].Class == CLASS_DARKLORD )	// DarkLord and MagicGLadiaro
	{
		for ( int i =0;i<KALIMA_FLOORS;i++)
		{
			if ( gObj[iIndex].Level >= g_sttKALIMAGATE_LEVEL[i].iLOWER_BOUND_MAGUMSA )
			{
				if ( gObj[iIndex].Level <= g_sttKALIMAGATE_LEVEL[i].iUPPER_BOUND_MAGUMSA )
				{
					return i;
				}
			}
		}
	}
	else
	{
		for ( int i =0;i<KALIMA_FLOORS;i++)
		{
			if ( gObj[iIndex].Level >= g_sttKALIMAGATE_LEVEL[i].iLOWER_BOUND )
			{
				if ( gObj[iIndex].Level <= g_sttKALIMAGATE_LEVEL[i].iUPPER_BOUND )
				{
					return i;
				}
			}
		}
	}
	return -1;
}

//004163C0 - identical
BOOL CKalimaGate::DeleteKalimaGate(int iKalimaGateIndex, int iCallOwnerIndex)
{
	EnterCriticalSection(&this->m_critKalimaGate);

	__try
	{
		if ( OBJMAX_RANGE(iKalimaGateIndex )== false || OBJMAX_RANGE(iCallOwnerIndex ) == false)
		{
			LogAddTD("[Kalima] DeleteKalimaGate() - out of Index (iKalimaGateIndex:%d, iCallOwnerIndex:%d)",
				iKalimaGateIndex, iCallOwnerIndex);
			return 0;
		}

		GCDiePlayerSend( &gObj[iKalimaGateIndex], iKalimaGateIndex, 0 , 0);
		gObj[iCallOwnerIndex].m_cKalimaGateExist = FALSE;
		gObj[iCallOwnerIndex].m_iKalimaGateIndex = -1;
		gObjDel(iKalimaGateIndex);
		gObjCharZeroSet(iKalimaGateIndex);
	}
	__finally
	{
		LeaveCriticalSection(&this->m_critKalimaGate);
	}
	return true;
}

//00416570 - identical
BOOL CKalimaGate::DeleteKalimaGate(int iCallOwnerIndex)
{
	if ( OBJMAX_RANGE(iCallOwnerIndex ) == FALSE )
	{	
		LogAddTD("[Kalima] DeleteKalimaGate() - out of Index (iCallOwnerIndex:%d)",
			iCallOwnerIndex);
		return false;
	}

	int iKalimaGateIndex = gObj[iCallOwnerIndex].m_iKalimaGateIndex;

	if (  OBJMAX_RANGE (iKalimaGateIndex ) == FALSE )
	{	
		return false;
	}

	if ( gObj[iKalimaGateIndex].Live == FALSE || !KALIMA_ATTRIBUTE_RANGE(gObj[iKalimaGateIndex].m_Attribute) )
	{
		return FALSE;
	}

	return this->DeleteKalimaGate(iKalimaGateIndex, iCallOwnerIndex);
}

//004166C0 - identical
BOOL CKalimaGate::CreateKalimaGate2(int iIndex, int iMonMapNumber, BYTE cTX, BYTE cTY)
{	
	BOOL bKalimaGateCreateSucceed = FALSE;
	int iMonsterIndex = -1;

	EnterCriticalSection(&this->m_critKalimaGate);

	__try
	{
		if ( !gObjIsConnected(iIndex))
			return FALSE;

		LogAddTD("[Kalima] [%s][%s] Try to Create Kalima Gate (NextMap)",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		if ( iMonMapNumber < MAP_INDEX_KALIMA1 || iMonMapNumber > MAP_INDEX_KALIMA6-1 )
		{
			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate (NextMap) - MapNumber Error : %d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, iMonMapNumber);

			return FALSE;
		}

		if ( gObj[iIndex].m_cKalimaGateExist == TRUE )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 224)), iIndex, 1);

			LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate (NextMap) - Already Have Gate (SummonIndex:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_iKalimaGateIndex);
	
			return FALSE;
		}

		int iMonsterType = iMonMapNumber + 129;
		BYTE cX = cTX;
		BYTE cY = cTY;
		BYTE btMapAttr = MapC[iMonMapNumber].GetAttr(cX, cY);

		if ( btMapAttr != 0 )
		{
			if ( this->GetRandomLocation(iMonMapNumber, cX, cY) == FALSE )
			{
				LogAddTD("[Kalima] [%s][%s] Failed to Summon Kalima Gate (NextMap) - Failed to get Location (MapNumber:%d, X:%d, Y:%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iMonMapNumber, cX, cY);

				return FALSE;
			}
		}

		iMonsterIndex = gObjAddMonster(iMonMapNumber);

		if ( iMonsterIndex >= 0 )
		{
			MONSTER_ATTRIBUTE * lpMATTR = gMAttr.GetAttr(iMonsterType);

			if ( !lpMATTR )
			{
				gObjDel(iMonsterIndex);

				LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished (NextMap) - lpMATTR == NULL (SummonIndex:%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iMonsterIndex);

				return FALSE;
			}

			gObjSetMonster(iMonsterIndex, iMonsterType);
			gObj[iMonsterIndex].Live = TRUE;
			gObj[iMonsterIndex].Life = 1000.0;
			gObj[iMonsterIndex].MaxLife = 1000.0;
			gObj[iMonsterIndex].m_PosNum = -1;
			gObj[iMonsterIndex].X = cX;
			gObj[iMonsterIndex].Y = cY;
			gObj[iMonsterIndex].MTX = cX;
			gObj[iMonsterIndex].MTY = cY;
			gObj[iMonsterIndex].TX = cX;
			gObj[iMonsterIndex].TY = cY;
			gObj[iMonsterIndex].m_OldX = cX;
			gObj[iMonsterIndex].m_OldY = cY;
			gObj[iMonsterIndex].StartX = cX;
			gObj[iMonsterIndex].StartY = cY;
			gObj[iMonsterIndex].MapNumber = iMonMapNumber;
			gObj[iMonsterIndex].m_MoveRange = 0;
			gObj[iMonsterIndex].m_cKalimaGateEnterCount = 0;

			gObj[iIndex].m_cKalimaGateExist = TRUE;
			gObj[iIndex].m_iKalimaGateIndex = iMonsterIndex;

			gObj[iMonsterIndex].Level = lpMATTR->m_Level;
			gObj[iMonsterIndex].Type = OBJ_MONSTER;
			gObj[iMonsterIndex].MaxRegenTime = 1000;
			gObj[iMonsterIndex].Dir = 1;

			gObj[iMonsterIndex].m_RecallMon = iIndex;
			gObj[iMonsterIndex].RegenTime = GetTickCount();
			gObj[iMonsterIndex].m_Attribute = 58;
			gObj[iMonsterIndex].DieRegen = FALSE;
			
			LogAddTD("[Kalima] [%s][%s] Create Kalima Gate Successfuly (NextMap) (Live:%d, GateTick:%u, DieRegen:%d, Map:%d, X:%d, Y:%d, SummonIndex:%d, OwnerIndex:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iMonsterIndex].Live, gObj[iMonsterIndex].RegenTime,
				gObj[iMonsterIndex].DieRegen, gObj[iMonsterIndex].MapNumber, gObj[iMonsterIndex].X,
				gObj[iMonsterIndex].Y, iMonsterIndex, iIndex);
			bKalimaGateCreateSucceed = TRUE;


		}
	}
	__finally
	{
		LeaveCriticalSection(&this->m_critKalimaGate);
	}

	return TRUE;
}

//00416F70 - identical
void CKalimaGate::KalimaGateAct2(int iIndex)
{
	LPOBJ lpObj = NULL;
	LPOBJ lpObjCallOwner = NULL;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	if ( gObj[iIndex].Live == 0 )
		return;

	if ( !gObjIsConnected(gObj[iIndex].m_RecallMon ) )
	{
		LogAddTD("[Kalima] Kalima Gate Vanished (NextMap) - Summoner Vanished (SummonIndex:%d, EnterCount:%d)",
			iIndex, gObj[iIndex].m_cKalimaGateEnterCount);

		GCDiePlayerSend(&gObj[iIndex], iIndex, 0, 0);
		gObjDel(iIndex);
		gObjCharZeroSet(iIndex);

		return;
	}

	lpObj = &gObj[iIndex];
	lpObjCallOwner = &gObj[lpObj->m_RecallMon];

	if ( gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMA_ENTER )
	{
		lpObjCallOwner->AccountID[MAX_ACCOUNT_LEN]=0;
		lpObjCallOwner->Name[MAX_ACCOUNT_LEN]=0;

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - Enter Count Over (SummonIndex:%d, EnterCount:%d)",
			lpObjCallOwner->AccountID, lpObjCallOwner->Name, iIndex, gObj[iIndex].m_cKalimaGateEnterCount);

		this->DeleteKalimaGate(iIndex, gObj[iIndex].m_RecallMon);

		return;
	}

	DWORD dwTickCount = GetTickCount();

	if ( dwTickCount > (lpObj->RegenTime+60000) )
	{
		lpObjCallOwner->AccountID[MAX_ACCOUNT_LEN]=0;
		lpObjCallOwner->Name[MAX_ACCOUNT_LEN]=0;

		LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished (NextMap) - Time Out (SummonIndex:%d, EnterCount:%d)",
			lpObjCallOwner->AccountID, lpObjCallOwner->Name, iIndex, gObj[iIndex].m_cKalimaGateEnterCount);

		this->DeleteKalimaGate(lpObj->m_Index, lpObjCallOwner->m_Index);

		return;
	}

	if ( lpObj->VPCount < 1 )
		return;

	if ( lpObj->MapNumber < MAP_INDEX_KALIMA1 || lpObj->MapNumber > MAP_INDEX_KALIMA6-1 )
		return;

	int tObjNum = -1;

	for ( int i=0;i<MAX_VIEWPORT_MONSTER;i++)
	{
		tObjNum = lpObj->VpPlayer[i].number;

		if ( tObjNum >= 0 )
		{
			if ( gObj[tObjNum].Type == OBJ_USER )
			{
				if ( gObj[tObjNum].Live )
				{
					if ( abs(lpObj->Y - gObj[tObjNum].Y) < 2 )
					{
						if ( abs(lpObj->X - gObj[tObjNum].X) < 2 )
						{
							if ( lpObj->m_RecallMon == tObjNum || (gObj[tObjNum].PartyNumber != -1 && lpObjCallOwner->PartyNumber == gObj[tObjNum].PartyNumber ))
							{
								int iMoveMapLevel = lpObj->MapNumber - MAP_INDEX_KALIMA1+1;	// #error "Deatwway - remove the -1
								int iGateNumber = g_iKalimaGateGateIndex[iMoveMapLevel];

								if ( !gObjMoveGate(tObjNum, iGateNumber))
								{
									LogAddTD("[Kalima] [%s][%s] - [%s][%s] move fail (MapNumber:%d)",
										lpObjCallOwner->AccountID, lpObjCallOwner->Name,
										gObj[tObjNum].AccountID, gObj[tObjNum].Name,
										lpObj->MapNumber);
								}
								else
								{
									LogAddTD("[Kalima] [%s][%s] - [%s][%s] Transported by Kalima Gate (NextMap) (SummonIndex:%d, GateNo.:%d, MapNumber:%d)",
										lpObjCallOwner->AccountID, lpObjCallOwner->Name,
										gObj[tObjNum].AccountID, gObj[tObjNum].Name,
										iIndex, iMoveMapLevel+1, lpObj->MapNumber);

									gObj[iIndex].m_cKalimaGateEnterCount++;

									if ( gObj[iIndex].m_cKalimaGateEnterCount >= MAX_KALIMA_ENTER )
										break;
								}
							}
						}
					}
				}
			}
		}
	}
}

//00417570 - identical
int CKalimaGate::GetKalimaGateLevel2(int iIndex)
{
	if ( gObj[iIndex].Class == CLASS_MAGUMSA 
#ifdef MONK
		|| gObj[iIndex].Class == CLASS_MONK
#endif
		|| gObj[iIndex].Class == CLASS_DARKLORD )	// DarkLord and MagicGLadiaro
	{
		for ( int i =0;i<KALIMA_FLOORS;i++)
		{
			if ( gObj[iIndex].Level >= g_sttKALIMAGATE_LEVEL[i].iLOWER_BOUND_MAGUMSA )
			{
				if ( gObj[iIndex].Level <= g_sttKALIMAGATE_LEVEL[i].iUPPER_BOUND_MAGUMSA )
				{
					return i;
				}
			}
		}
	}
	else
	{
		for ( int i =0;i<KALIMA_FLOORS;i++)
		{
			if ( gObj[iIndex].Level >= g_sttKALIMAGATE_LEVEL[i].iLOWER_BOUND )
			{
				if ( gObj[iIndex].Level <= g_sttKALIMAGATE_LEVEL[i].iUPPER_BOUND )
				{
					return i;
				}
			}
		}
	}
	return -1;
}