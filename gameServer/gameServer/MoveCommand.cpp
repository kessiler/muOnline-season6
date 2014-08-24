//GS-N	1.00.90 JPN		- Completed
//GS-CS 1.00.90 JPN		- Completed
#include "stdafx.h"
#include "MoveCommand.h"
#include "..\include\readscript.h"
#include "logproc.h"
#include "..\common\WzMemScript.h"
#include "DevilSquare.h"
#include "KalimaGate.h"
#include "ChaosCastle.h"
#include "CastleSiegeSync.h"
#include "CashShop.h"
#include "CashItemPeriodSystem.h"
#include "IllusionTempleEvent.h"
#include "RaklionBattleUserMng.h"

#ifdef GENS
#include "GensSystem.h"
#endif

CMoveCommand gMoveCommand;

CMoveCommand::CMoveCommand()
{
	return;
}

CMoveCommand::~CMoveCommand()
{
	return;
}

void CMoveCommand::Init()
{
	memset(this->m_MoveCommandData, 0, sizeof(this->m_MoveCommandData));

	for (int i =0;i<MAX_MOVE_COMMAND;i++)
	{
		this->m_MoveCommandData[i].Index = -1;
	}
}

int CMoveCommand::Load(char* filename)
{
	int Token;
	int counter;

	this->Init();

	SMDFile=fopen(filename, "r");	//ok

	if ( SMDFile == 0 )
	{
		MsgBox("MoveCommand %s File Load Error", filename);
		return 0;
	}

	counter=0;

	while ( true )
	{
		Token=GetToken();

		if ( Token == 2)
		{
			break;
		}
		
		
		if ( Token == 1)
		{
			this->m_MoveCommandData[counter].Index = TokenNumber;

			Token = GetToken();
			strcpy(this->m_MoveCommandData[counter].Name, TokenString);

			Token = GetToken();
			strcpy(this->m_MoveCommandData[counter].EngName, TokenString);

			Token = GetToken();
			this->m_MoveCommandData[counter].NeedZen = TokenNumber;

			Token = GetToken();
			this->m_MoveCommandData[counter].NeedLevel = TokenNumber;

			Token = GetToken();
			this->m_MoveCommandData[counter].GateNumber = TokenNumber;

			counter++;
		}
		else
		{
			this->Init();
			fclose(SMDFile);
			MsgBox("MoveCommand script load fail");
			return FALSE;
		}
	}
	fclose(SMDFile);
	return TRUE;
}

int CMoveCommand::Load(char* Buffer, int iSize)
{
	this->Init();
	CWzMemScript WzMemScript;
	int Token;
	int counter;
	
	WzMemScript.SetBuffer(Buffer, iSize);

	counter=0;

	while ( true )
	{
		Token=WzMemScript.GetToken();

		if ( Token == 2)
		{
			break;
		}
		
		
		if ( Token == 1)
		{
			this->m_MoveCommandData[counter].Index = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			strcpy(this->m_MoveCommandData[counter].Name, WzMemScript.GetString());

			Token = WzMemScript.GetToken();
			strcpy(this->m_MoveCommandData[counter].EngName, WzMemScript.GetString());

			Token = WzMemScript.GetToken();
			this->m_MoveCommandData[counter].NeedZen = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			this->m_MoveCommandData[counter].NeedLevel = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			this->m_MoveCommandData[counter].GateNumber = WzMemScript.GetNumber();

			counter++;
		}
		else
		{
			this->Init();
			MsgBox("MoveCommand script load fail");
			return FALSE;
		}
	}
	return TRUE;
}

int CMoveCommand::LoadMoveLevel(char* filename)
{
	int Token;
	int counter;

	memset(this->m_MoveLevel, -1, sizeof(this->m_MoveLevel));
		
	SMDFile=fopen(filename, "r");	//ok

	if ( SMDFile == 0 )
	{
		MsgBox("MoveLevel %s File Load Error", filename);
		return 0;
	}

	counter=0;

	while ( true )
	{
		Token=GetToken();

		if ( Token == 2)
		{
			break;
		}
		
		
		if ( Token == 1)
		{
			this->m_MoveLevel[counter].MoveLevel = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].MapNumber = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].X = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].Y = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].TX = TokenNumber;

			Token = GetToken();
			this->m_MoveLevel[counter].TY = TokenNumber;

			counter++;
		}
		else
		{
			this->Init();
			fclose(SMDFile);
			MsgBox("MoveLevel script load fail");
			return FALSE;
		}
	}
	fclose(SMDFile);
	return TRUE;
}

int CMoveCommand::GetMoveLevel(int mapnumber, int x, int y, int Class)
{
	for( int i=0;i<MAX_MOVE_COMMAND;i++)
	{
		if ( this->m_MoveLevel[i].MapNumber == mapnumber )
		{
			if ( x >= this->m_MoveLevel[i].X && x <= this->m_MoveLevel[i].TX && y >= this->m_MoveLevel[i].Y && y <= this->m_MoveLevel[i].TY )
			{
				if(mapnumber == MAP_INDEX_SWAMP_OF_CALMNESS)
				{
					return this->m_MoveLevel[i].MoveLevel;
				}

				if ( Class == CLASS_DARKLORD || Class == CLASS_MAGUMSA 
#ifdef MONK
					|| Class == CLASS_MONK
#endif
					|| Class == CLASS_SUMMONER )
				{
					return this->m_MoveLevel[i].MoveLevel * 2 / 3;
				}
				else
				{
					return this->m_MoveLevel[i].MoveLevel;
				}
			}
		}
	}

	return -1;
}

BOOL CMoveCommand::CheckMainToMove(LPOBJ lpObj)
{
	if ( BC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}

	if ( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}

	if ( KALIMA_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}

	if ( DS_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return FALSE;
	}

//#if(_GSCS==0)
	if ( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS )
	{
		return FALSE;
	}
//#endif

	if ( IT_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		if(g_IllusionTempleEvent.CheckRelics(lpObj->m_Index) != FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CMoveCommand::CheckEquipmentToMove(LPOBJ lpObj, int iTargetMapNumber)
{
	if ( iTargetMapNumber == MAP_INDEX_ATHLANSE )
	{
		if ( lpObj->pInventory[8].IsItem() != FALSE )
		{
			if ( lpObj->pInventory[8].m_Type == ITEMGET(13,2) )
			{
				return FALSE;
			}

			if ( lpObj->pInventory[8].m_Type == ITEMGET(13,3) )
			{
				return FALSE;
			}
		}
	}

	if ( iTargetMapNumber == MAP_INDEX_ICARUS )
	{
		if ( (lpObj->pInventory[8].m_Type != ITEMGET(13,3) &&
			 lpObj->pInventory[7].m_Type != ITEMGET(13,30) &&
			 lpObj->pInventory[8].m_Type != ITEMGET(13,37) &&
			 lpObj->pInventory[7].m_Type != ITEMGET(12,36) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,37) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,38) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,39) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,40) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,41) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,42) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,43) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12, 49) &&//Season 6 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12, 50) &&//Season 6 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12, 130) &&//Season 6 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12, 131) &&//Season 6 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12, 132) &&//Season 6 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12, 133) &&//Season 6 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12, 134) &&//Season 6 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12, 135) &&//Season 6 addon
#ifdef NEWWINGS
			 !IS_NEWWINGS(lpObj->pInventory[7].m_Type) &&
#endif
			 lpObj->pInventory[8].m_Type != ITEMGET(13,4) &&//Season 4.5 addon
			 (lpObj->pInventory[7].m_Type < ITEMGET(12,0) || lpObj->pInventory[7].m_Type > ITEMGET(12,6)) ) ||
			 lpObj->pInventory[8].m_Type == ITEMGET(13,2) ||
			 lpObj->pInventory[11].m_Type == ITEMGET(13,10) ||
			 lpObj->pInventory[10].m_Type == ITEMGET(13,10)  )
		{
			return FALSE;
		}

		if ( lpObj->m_RecallMon >= 0 )
		{
			GCRecallMonLife(lpObj->m_Index, 60, 0);
			gObjMonsterCallKill(lpObj->m_Index);
		}
	}

	return TRUE;
}

BOOL CMoveCommand::CheckInterfaceToMove(LPOBJ lpObj)
{
	if ( lpObj->m_IfState.use == 1 )
	{
		return FALSE;
	}

	if ( lpObj->m_bPShopOpen == true )
	{
		return FALSE;
	}

	return TRUE;
}

int CMoveCommand::FindIndex(LPSTR mapname)
{
	for ( int i=0;i<MAX_MOVE_COMMAND;i++)
	{
		if ( this->m_MoveCommandData[i].Index == -1 )
		{
			return -1;
		}

		if ( strcmp(mapname, this->m_MoveCommandData[i].Name) == 0 || strcmpi(mapname, this->m_MoveCommandData[i].EngName) == 0  )
		{
			return i;
		}
	}

	return -1;
}

BOOL CMoveCommand::CheckMoveMapBound(int iMapIndex)
{
	switch ( iMapIndex )
	{
		case 1:
		case 2:
		case 3:
		case 6:
		case 10:
		case 16:
			return TRUE;
	}
	
	return FALSE;
}

BOOL CMoveCommand::Move(LPOBJ lpObj, LPSTR mapname)
{
	int m_number = -1;
	int overlevel = 0;
	int subMoney = 2000;

	int index = this->FindIndex(mapname);

#ifdef OLDCASHSHOP
	if ( g_bUseMoveMapBound == TRUE )
	{
		if ( g_CashItemPeriodSystem.CheckHaveItemPeriodSealEffect(lpObj) == FALSE )
		{
			if ( !this->CheckMoveMapBound(index))
			{
				return FALSE;
			}
		}
	}
#endif
	int MapNumber = this->m_MoveCommandData[index].Index; //season 3.0 add-on

#ifdef GENS
	if( gGensSystem.IsMoveMapBattleZone(MapNumber) )
	{
		if( !gGensSystem.IsUserBattleZoneEnable(lpObj) )
		{
			LogAddC(2, "BattleZone not enable");
			//v4 = *(_DWORD *)a2;
			//v5 = CMsg::Get(&unk_8CFC758, 3451);
			//sub_406D7F(v5, v4, 1);
			return false;
		}
		// ----
		//v6 = *(_DWORD *)a2;
		//v7 = CMsg::Get(&unk_8CFC758, 3452);
		//sub_406D7F(v7, v6, 1);
		// ----
		gGensSystem.MoveInBattleZonePartySplit(lpObj);
	}
#endif

	if( MapNumber == 24 ) //season 3.0 changed
	{
		if ( g_CastleSiegeSync.GetCastleState() == 7 )
		{
			return FALSE;
		}

		if ( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == FALSE && g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE)
		{
			return FALSE;
		}
	}

	if(IT_MAP_RANGE(lpObj->MapNumber) != FALSE) //season 3.0 add-on
	{
		if(MapNumber == 11 || MapNumber == 12 || MapNumber == 13 || MapNumber == 23)
		{
			return FALSE;
		}
	}

	if ( index == -1 )
	{
		return FALSE;
	}

	int NeedLevel	= this->m_MoveCommandData[index].NeedLevel;
	int NeedZen		= this->m_MoveCommandData[index].NeedZen;
	int GateNumber	= this->m_MoveCommandData[index].GateNumber;

#ifdef GENS
	NeedZen			*= gGensSystem.PkPenaltyAddNeedZenMapMove(lpObj);
#endif

	if( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif
		|| lpObj->Class == CLASS_SUMMONER )
	{
		if( NeedLevel > 0 )
		{
			if( GateNumber != 273 ) //season 3.0 add-on (Swamp of Calmmness)
			{
				NeedLevel = NeedLevel * 2 / 3;
			}
		}
	}

	if ( GateNumber >= 0 )
	{
		if ( NeedLevel <= lpObj->Level )
		{
			if ( (lpObj->Money - NeedZen ) >= 0 )
			{
				if ( gObjMoveGate(lpObj->m_Index, GateNumber ) != FALSE )
				{
					lpObj->m_bPShopRedrawAbs = true;
					lpObj->Money -= NeedZen;

					LogAddTD("[%s][%s] Use [%s %s] Success : reduces money %d - %d", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(11, 185)), mapname, lpObj->Money, NeedLevel);

					GCMoneySend(lpObj->m_Index, lpObj->Money );
					
					if(lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS)//Season 4.5 addon
					{
						g_RaklionBattleUserMng.DeleteUserData(lpObj->m_Index);
					}
					return TRUE;
				}
			}
			else
			{
				::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 102)), lpObj->m_Index, 1);
			}
		}
		else
		{
			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(MSGGET(4, 93)), NeedLevel);
			::GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
		}
	}

	LogAddTD("[%s][%s] Use [%s %s] Fail GateNumber = %d", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(11, 185)), mapname, GateNumber);
	return FALSE;

}

//0041d3a0
int GetKalimaGateLevel(LPOBJ lpObj)
{
	if ( lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif
		|| lpObj->Class == CLASS_SUMMONER)
	{
		for ( int i=0;i<KALIMA_FLOORS;i++)
		{
			if ( lpObj->Level >= g_sttKALIMAGATE_LEVEL[i].iLOWER_BOUND_MAGUMSA && lpObj->Level <= g_sttKALIMAGATE_LEVEL[i].iUPPER_BOUND_MAGUMSA )
			{
				return i;
			}
		}
	}
	else
	{
		for ( int i=0;i<KALIMA_FLOORS;i++)
		{
			if ( lpObj->Level >= g_sttKALIMAGATE_LEVEL[i].iLOWER_BOUND && lpObj->Level <= g_sttKALIMAGATE_LEVEL[i].iUPPER_BOUND )
			{
				return i;
			}
		}
	}

	return -1;
}

BOOL CMoveCommand::MoveFree2Kalima(LPOBJ lpObj)
{
	int iIdx = GetKalimaGateLevel(lpObj);

	if ( iIdx >= 6 )
	{
		iIdx = 5;
	}

	if ( iIdx < 0 )
	{
		return FALSE;
	}

	int iGateNumber = iIdx + 88;

	return gObjMoveGate(lpObj->m_Index, iGateNumber);
}

//005d2a30
BOOL CMoveCommand::Move(LPOBJ lpObj,int MoveNum)
{
	int m_number = -1;
	int overlevel = 0;
	int subMoney = 2000;

	int index = this->FindIndex(MoveNum);
#ifdef OLDCASHSHOP
	if ( g_bUseMoveMapBound == TRUE )
	{
		if ( g_CashItemPeriodSystem.CheckHaveItemPeriodSealEffect(lpObj) == FALSE )
		{
			if ( !this->CheckMoveMapBound(index))
			{
				return FALSE;
			}
		}
	}
#endif
	if(this->CheckMoveNum(MoveNum) == 0)
	{
		return FALSE;
	}

	int loc6 =  MoveNum;//season 3.0 add-on

	if ( loc6 == 24 ) //season 3.0 changed
	{
		if ( g_CastleSiegeSync.GetCastleState() == 7 )
		{
			return FALSE;
		}

		if ( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == FALSE && g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE)
		{
			return FALSE;
		}
	}

	if(IT_MAP_RANGE(lpObj->MapNumber) != FALSE) //season 3.0 add-on
	{
		if(loc6 == 11 || loc6 == 12 || loc6 == 13 || loc6 == 23)
		{
			return FALSE;
		}
	}

	if ( index == -1 )
	{
		return FALSE;
	}

	int NeedLevel = this->m_MoveCommandData[index].NeedLevel;
	int NeedZen   = this->m_MoveCommandData[index].NeedZen;
	int GateNumber= this->m_MoveCommandData[index].GateNumber;

	if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif
		|| lpObj->Class == CLASS_SUMMONER)
	{
		if ( NeedLevel > 0 )
		{
			if (GateNumber != 273) //season 3.0 add-on (Swamp of Calmmness)
			{
				NeedLevel = NeedLevel *2 / 3;
			}
		}
	}

	if ( GateNumber >= 0 )
	{
		if ( NeedLevel <= lpObj->Level )
		{
			if ( (lpObj->Money - NeedZen ) >= 0 )
			{
				if ( gObjMoveGate(lpObj->m_Index, GateNumber ) != FALSE )
				{
					lpObj->m_bPShopRedrawAbs = true;
					lpObj->Money -= NeedZen;

					LogAddTD("[%s][%s] Use [%s %s] Success : reduces money %d - %d", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(11, 185)), this->m_MoveCommandData[index].EngName, lpObj->Money, NeedLevel);

					GCMoneySend(lpObj->m_Index, lpObj->Money );
					
					if(lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS)//Season 4.5 addon
					{
						g_RaklionBattleUserMng.DeleteUserData(lpObj->m_Index);
					}
					return TRUE;
				}
			}
			else
			{
				::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 102)), lpObj->m_Index, 1);
			}
		}
		else
		{
			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(MSGGET(4, 93)), NeedLevel);
			::GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
		}
	}

	LogAddTD("[%s][%s] Use [%s %s] Fail GateNumber = %d", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(11, 185)), this->m_MoveCommandData[index].EngName, GateNumber);
	return FALSE;

}

int CMoveCommand::FindIndex(int MoveNum)
{
	for ( int i=0;i<MAX_MOVE_COMMAND;i++)
	{
		if ( this->m_MoveCommandData[i].Index == -1 )
		{
			return -1;
		}

		if ( this->m_MoveCommandData[i].Index ==MoveNum )
		{
			return i;
		}
	}

	return -1;
}
int CMoveCommand::CheckMoveNum(int MoveNum)
{
	for ( int i=0;i<MAX_MOVE_COMMAND;i++)
	{
		if ( this->m_MoveCommandData[i].Index == MoveNum )
		{
			return 1;
		}

	}

	return 0;
}

MOVE_COMMAND_DATA * CMoveCommand::GetMoveCommandData(int nMapIndex)
{
	int nIndex = -1;
	// ----
	for( int i = 0; i < MAX_MOVE_COMMAND; i++ )
	{
		if( this->m_MoveCommandData[i].Index == nMapIndex )
		{
			nIndex = i;
			break;
		}
	}
	// ----
	if( nIndex >= 0 )
	{
		return &this->m_MoveCommandData[nIndex];
	}
	// ----
	return 0;
}