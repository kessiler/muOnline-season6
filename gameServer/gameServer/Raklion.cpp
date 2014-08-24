// Raklion.cpp: implementation of the CRaklion class.
// GS 1.00.90 -  Finished. 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Raklion.h"
#include "GameMain.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "User.h"
#include "RaklionUtil.h"
#include "RaklionBattleUserMng.h"
#include "gObjMonster.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int g_iUseRaklionEvent = 1;

CRaklion g_Raklion;

CRaklion::CRaklion()
{
	tm *today;
	time_t ltime;

	time(&ltime);
	today=localtime(&ltime);
	
	this->iYear = today->tm_year;
	this->iMon = today->tm_mon;
	this->iDay = today->tm_mday;
	this->isUsed = 0;

	this->ClearData();
}

CRaklion::~CRaklion()
{

}

void CRaklion::Init()
{
	//empty
}

void CRaklion::ClearData()
{
	this->m_iUnk16 = 0;
	this->m_iFileDataLoad = 0;
	this->m_iAppearanceMSec = 0;
	this->m_iBossZoneCloseMSec = 0;
	this->m_iBossZoneOpenMSec = 0;
	this->m_iBossEggHalfChecked = 0;
	this->m_iBossEggCount =0;
}

BOOL CRaklion::LoadData(char *szFileName)
{
	this->m_iFileDataLoad = 0;

	if(!szFileName || !strcmp(szFileName,""))
	{
		MsgBox("[ RAKLION ] - File load error : File Name Error");
		return FALSE;
	}

	try
	{

		SMDFile = fopen(szFileName,"r");	//ok
		
		if( SMDFile == NULL)
		{
			DWORD dwError = GetLastError();
			MsgBox("[ RAKLION ] - Can't Open %s ",szFileName);
			return FALSE;
		}

		enum SMDToken Token;
		int iType = -1;
		int iAppearanceDelay;
		int iZoneCloseDelay;
		int iZoneOpenDelay;
		int iBossEggHalf;
		int iBossEggMax;
		int iBossSkillDelay;

		while ( true )
		{

			Token = GetToken();

			if ( Token == END)
			{
				break;
			}

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					iAppearanceDelay = 0;
					iZoneCloseDelay = 0;
					iZoneOpenDelay = 0;

					Token = GetToken();

					if( strcmp("end", TokenString) == 0 )
					{
						break;
					}
					
					iAppearanceDelay = TokenNumber;

					Token = GetToken();
					iZoneCloseDelay = TokenNumber;

					Token = GetToken();
					iZoneOpenDelay = TokenNumber;

					if (iAppearanceDelay < 0 || iAppearanceDelay > MAX_RAKLION_APPEARANCE_DELAY )
					{
						MsgBox("[ RAKLION ] - Exceed AppearanceDelay : (%d)",iAppearanceDelay);
						break;
					}
					if (iZoneCloseDelay < 0 || iZoneCloseDelay >= MAX_RAKLION_BOSSZONECLOSE_DELAY )
					{
						MsgBox("[ RAKLION ] - Exceed BosszoneCloseDelay : (%d)",iZoneCloseDelay);
						break;
					}
					if (iZoneOpenDelay < 0 || iZoneOpenDelay >= MAX_RAKLION_BOSSZONEOPEN_DELAY )
					{
						MsgBox("[ RAKLION ] - Exceed BosszoneOpenDelay : (%d)", iZoneOpenDelay);
						break;
					}

					this->m_iAppearanceDelay	= iAppearanceDelay;
					this->m_iBossZoneCloseDelay	= iZoneCloseDelay;
					this->m_iBossZoneOpenDelay	= iZoneOpenDelay;

				}
				
				else if ( iType == 1 )
				{
					iBossEggHalf = 0;
					iBossEggMax = 0;

					Token = GetToken();

					if ( strcmp("end",TokenString) == 0)
					{
						break;
					}

					iBossEggHalf = TokenNumber;

					Token = GetToken();
					iBossEggMax = TokenNumber;


					if(iBossEggHalf < 0 )
					{
						MsgBox("[ RAKLION ] - BossEggHalf count error : (%d)",iBossEggHalf);
						break;
					}

					if(iBossEggMax < 0 )
					{
						MsgBox("[ RAKLION ] - BossEggMax count error : (%d)",iBossEggMax);
						break;
					}

					this->m_iBossEggHalf = iBossEggHalf;
					this->m_iBossEggMax = iBossEggMax;
				}

				else if ( iType == 2 )
				{
					iBossSkillDelay = 0;

					Token = GetToken();

					if ( strcmp("end",TokenString) == 0)
					{
						break;
					}

					iBossSkillDelay = TokenNumber;

					LogAddC(2, "SkillDelay = %d", iBossSkillDelay);

					if ( iBossSkillDelay < 0)
					{
						MsgBox("[ RAKLION ] - BossSkillDelay count error : (%d)",iBossSkillDelay);
						break;
					}

					this->m_RaklionBattleOfSelupan.SetSelupanSkillDelay(iBossSkillDelay);
				}

				else if ( iType == 3 )
				{
					int iPattern[6] = { 0 };


					Token = GetToken();

					if ( strcmp("end",TokenString) == 0)
					{
						break;
					}

					iPattern[0] = TokenNumber;

					Token = GetToken();
					iPattern[1] = TokenNumber;

					Token = GetToken();
					iPattern[2] = TokenNumber;

					Token = GetToken();
					iPattern[3] = TokenNumber;

					Token = GetToken();
					iPattern[4] = TokenNumber;

					Token = GetToken();
					iPattern[5] = TokenNumber;

					for ( int i = 0; i < 6; i++)
					{
						if(iPattern[i] < 0)
						{
							MsgBox("[ RAKLION ] - BossPatternCondition[%d] error : (%d)", i, iPattern[i]);
							break;
						}
						this->m_RaklionBattleOfSelupan.SetPatternCondition(i,iPattern[i]);
					}
				}
				else
				{
					Token = (SMDToken)GetToken();
					
					if ( !strcmp("end", TokenString))
						break;
				}
			}
		}

		fclose(SMDFile);
		LogAddC(2,"[ RAKLION ] - %s file is Loaded", szFileName);
		this->m_iFileDataLoad = 1;
	}
	catch ( DWORD )
	{
		MsgBox("[ RAKLION ] - Loading Exception Error (%s) File. ",szFileName);
	}

	return this->m_iFileDataLoad;
}

void CRaklion::CheckData() //00646050 
{
	return;
	//come code lacking due to return
}

void CRaklion::Run()
{
	int loc2 = 0; //loc2

	time_t Time; //loc3
	tm *today; //loc4

	time(&Time);
	today = localtime(&Time);

	if( this->iDay != today->tm_mday ||
		this->iMon != today->tm_mon ||
		this->iYear != today->tm_year)
	{
		this->iDay = today->tm_mday;
		this->iMon = today->tm_mon;
		this->iYear = today->tm_year;
		this->isUsed = 0;
	}
	
	if(this->isUsed == 0 && !today->tm_hour && !today->tm_min && !today->tm_sec)
	{
		for ( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
		{
			if(gObj[i].MapNumber == MAP_INDEX_RAKLION_FIELD || gObj[i].MapNumber == MAP_INDEX_RAKLION_BOSS)
			{
				if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
				{
					loc2++;
					LogAdd("[RAKLION][IsInRaklionEvent] %d/%d/%d %dth User : %s (%s)",
						today->tm_year+1900,today->tm_mon+1,today->tm_mday,loc2,gObj[i].Name,gObj[i].AccountID);
				}
			}
		}
		this->isUsed = 1;
	}

	if ( !this->IsRaklionEnable() )
		return;

	this->CheckUserOnRaklionBossMap();

	g_RaklionBattleUserMng.CheckUserState();

	switch ( this->m_iRaklionState )
	{
	case RAKLION_STATE_IDLE:			this->ProcState_IDLE();			break;
	case RAKLION_STATE_NOTIFY_1:		this->ProcState_NOTIFY_1();		break;
	case RAKLION_STATE_STANDBY:			this->ProcState_STANDBY();		break;
	case RAKLION_STATE_NOTIFY_2:		this->ProcState_NOTIFY_2();		break;
	case RAKLION_STATE_READY:			this->ProcState_READY();		break;
	case RAKLION_STATE_START_BATTLE:	this->ProcState_START_BATTLE();	break;
	case RAKLION_STATE_NOTIFY_3:		this->ProcState_NOTIFY_3();		break;
	case RAKLION_STATE_CLOSE_DOOR:		this->ProcState_CLOSE_DOOR();	break;
	case RAKLION_STATE_ALL_USER_DIE:	this->ProcState_ALL_USER_DIE();	break;
	case RAKLION_STATE_NOTIFY_4:		this->ProcState_NOTIFY_4();		break;
	case RAKLION_STATE_END:				this->ProcState_END();			break;
	}

}

void CRaklion::SetState(int iRaklionState)
{
	switch ( iRaklionState )
	{
	case RAKLION_STATE_IDLE:			this->SetState_IDLE();			break;
	case RAKLION_STATE_NOTIFY_1:		this->SetState_NOTIFY_1();		break;
	case RAKLION_STATE_STANDBY:			this->SetState_STANDBY();		break;
	case RAKLION_STATE_NOTIFY_2:		this->SetState_NOTIFY_2();		break;
	case RAKLION_STATE_READY:			this->SetState_READY();			break;
	case RAKLION_STATE_START_BATTLE:	this->SetState_START_BATTLE();	break;
	case RAKLION_STATE_NOTIFY_3:		this->SetState_NOTIFY_3();		break;
	case RAKLION_STATE_CLOSE_DOOR:		this->SetState_CLOSE_DOOR();	break;
	case RAKLION_STATE_ALL_USER_DIE:	this->SetState_ALL_USER_DIE();	break;
	case RAKLION_STATE_NOTIFY_4:		this->SetState_NOTIFY_4();		break;
	case RAKLION_STATE_END:				this->SetState_END();			break;
	}

	g_RaklionUtil.NotifyRaklionChangeState(this->GetRaklionState(),this->m_RaklionBattleOfSelupan.GetBattleOfSelupanState());

}

void CRaklion::SetNextState(int iCurrentState) //006464F0 
{
	int iNextState = iCurrentState + 1;

	if( iNextState > 12 )
	{
		iNextState = 0;
	}

	this->SetState(iNextState);
}

void CRaklion::ChangeState(int iState, int iDetailState) //00646540
{
	//some stuff
}

void CRaklion::SetState_IDLE()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> IDLE", this->m_iRaklionState);

	this->SetRaklionState(RAKLION_STATE_IDLE);
}

void CRaklion::SetState_NOTIFY_1()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> NOTIFY_1", this->m_iRaklionState);

	this->SetRaklionState(RAKLION_STATE_NOTIFY_1);
}

void CRaklion::SetState_STANDBY()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> STANDBY", this->m_iRaklionState);

	this->m_iAppearanceMSec = GetTickCount();

	this->SetRaklionState(RAKLION_STATE_STANDBY);
}

void CRaklion::SetState_NOTIFY_2()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> NOTIFY_2", this->m_iRaklionState);

	int iGateClosedMsg = this->m_iBossZoneCloseDelay / 60;

	if(iGateClosedMsg == 0)
	{
		iGateClosedMsg = 1;
	}

	g_RaklionUtil.SendMsgAllUser(lMsg.Get(3410),iGateClosedMsg);

	this->SetRaklionState(RAKLION_STATE_NOTIFY_2);
}

void CRaklion::SetState_READY()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> READY", this->m_iRaklionState);
	
	this->m_RaklionBattleOfSelupan.SetState(1);
	
	this->SetRaklionState(RAKLION_STATE_READY);
}

void CRaklion::SetState_START_BATTLE()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> START_BATTLE", this->m_iRaklionState);
	
	this->m_iBossZoneCloseMSec = GetTickCount();
	
	this->SetRaklionState(RAKLION_STATE_START_BATTLE);
}

void CRaklion::SetState_NOTIFY_3()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> NOTIFY_3", this->m_iRaklionState);
	
	g_RaklionUtil.SendMsgAllUser(lMsg.Get(3414));
	
	this->SetRaklionState(RAKLION_STATE_NOTIFY_3);

}

void CRaklion::SetState_CLOSE_DOOR()
{	
	LogAddC(7, "[ RAKLION ] State(%d) -> CLOSE_DOOR", this->m_iRaklionState);

	this->SetRaklionState(RAKLION_STATE_CLOSE_DOOR);
}

void CRaklion::SetState_ALL_USER_DIE()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> ALL_USER_DIE", this->m_iRaklionState);
	
	this->m_RaklionBattleOfSelupan.DeleteSelupan();
	
	this->m_RaklionBattleOfSelupan.DeleteSummonMonster();
	
	this->SetRaklionState(RAKLION_STATE_ALL_USER_DIE);
}

void CRaklion::SetState_NOTIFY_4()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> NOTIFY_4", this->m_iRaklionState);

	this->m_iBossZoneOpenMSec = GetTickCount();
	
	int iSuccesValue = this->m_RaklionBattleOfSelupan.GetSuccessValue();

	int iGateOpen = this->m_iBossZoneOpenDelay / 60;

	if(iGateOpen == 0)
	{
		iGateOpen = 1;
	}

	g_RaklionUtil.SendMsgAllUser(lMsg.Get(3413),iGateOpen);
	g_RaklionUtil.NotifyRaklionSuccessValue(iSuccesValue);

	this->SetRaklionState(RAKLION_STATE_NOTIFY_4);
}

void CRaklion::SetState_END()
{
	LogAddC(7, "[ RAKLION ] State(%d) -> END", this->m_iRaklionState);

	this->m_RaklionBattleOfSelupan.DeleteSelupan();
	this->m_RaklionBattleOfSelupan.DeleteSummonMonster();
	this->m_RaklionBattleOfSelupan.ResetAllData();

	this->ClearData();

	g_RaklionUtil.SendMsgAllUser(lMsg.Get(3411));

	this->SetRaklionState(RAKLION_STATE_END);

	this->BossEggRegenAll();

}

void CRaklion::ProcState_IDLE()
{
	if ( this->GetBossEggCount() == 0 )
	{
		this->SetState(RAKLION_STATE_STANDBY);
		return;
	}

	if ( this->m_iBossEggHalfChecked == 0)
	{
		if ( this->GetBossEggCount() == this->m_iBossEggHalf )
		{
			this->SetState(RAKLION_STATE_NOTIFY_1);

			this->m_iBossEggHalfChecked = 1;
		}
	}
}

void CRaklion::ProcState_NOTIFY_1()
{
	this->SetState(RAKLION_STATE_IDLE);
}

void CRaklion::ProcState_STANDBY()
{
	if ( 1000 * this->m_iAppearanceDelay <= GetTickCount() - this->m_iAppearanceMSec )
	{
		this->SetState(RAKLION_STATE_NOTIFY_2);
	}
}

void CRaklion::ProcState_NOTIFY_2()
{
	this->SetState(RAKLION_STATE_READY);
}

void CRaklion::ProcState_READY()
{
	this->SetState(RAKLION_STATE_START_BATTLE);
}

void CRaklion::ProcState_START_BATTLE()
{
	if ( 1000 * this->m_iBossZoneCloseDelay < GetTickCount() - this->m_iBossZoneCloseMSec )
	{
		this->SetState(RAKLION_STATE_NOTIFY_3);
	}

	this->m_RaklionBattleOfSelupan.SelupanLifeCheck();

	this->m_RaklionBattleOfSelupan.Run();
}

void CRaklion::ProcState_NOTIFY_3()
{
	this->m_RaklionBattleOfSelupan.SelupanLifeCheck();

	this->m_RaklionBattleOfSelupan.Run();

	this->SetState(RAKLION_STATE_CLOSE_DOOR);
}

void CRaklion::ProcState_CLOSE_DOOR()
{
	if ( g_RaklionBattleUserMng.CheckBattleUsers() == 1)
	{
		this->m_RaklionBattleOfSelupan.SetSuccessValue(FALSE);
		this->SetState(RAKLION_STATE_ALL_USER_DIE);
		return;
	}		

	if ( this->m_RaklionBattleOfSelupan.GetBattleOfSelupanState() == 9 )
	{
		this->m_RaklionBattleOfSelupan.SetSuccessValue(TRUE);
		this->SetState(RAKLION_STATE_NOTIFY_4);
		return;
	}
	
	this->m_RaklionBattleOfSelupan.SelupanLifeCheck();
	this->m_RaklionBattleOfSelupan.Run();


}

void CRaklion::ProcState_ALL_USER_DIE()
{	
	this->SetState(RAKLION_STATE_NOTIFY_4);
}

void CRaklion::ProcState_NOTIFY_4()
{
	if ( 1000 * this->m_iBossZoneOpenDelay < GetTickCount() - this->m_iBossZoneOpenMSec )
	{
		this->SetState(RAKLION_STATE_END);
	}
}

void CRaklion::ProcState_END()
{	
	this->SetState(RAKLION_STATE_IDLE);
}

void CRaklion::SetRaklionEnable(BOOL bEnable)
{
	this->m_IsRaklionEnable = bEnable;
}

BOOL CRaklion::IsRaklionEnable()
{
	if(g_iUseRaklionEvent != 0 && this->m_IsRaklionEnable != 0)
	{
		return TRUE;
	}

	return FALSE;
}

void CRaklion::SetRaklionState(int iRaklionState)
{
	this->m_iRaklionState = iRaklionState;
}

int CRaklion::GetRaklionState() 
{ 
	return this->m_iRaklionState; 
}

void CRaklion::SetRaklionDetailState(int iState)
{
	this->m_RaklionBattleOfSelupan.SetBattleOfSelupanState(iState);
}

int CRaklion::GetRaklionDetailState()
{
	return this->m_RaklionBattleOfSelupan.GetBattleOfSelupanState();
}

void CRaklion::BossEggRegen()
{
	if ( this->m_iBossEggCount >= this->m_iBossEggMax )
	{
		LogAddC(2, "[RAKLION] BossEggDieCount Error : (%d)",this->m_iBossEggCount);
		return;
	}

	this->m_iBossEggCount++;
}

void CRaklion::BossEggDie()
{
	if ( this->m_iBossEggCount <= 0 )
	{
		LogAddC(2, "[RAKLION] BossEggDieCount Error : (%d)",this->m_iBossEggCount);
		return;
	}

	this->m_iBossEggCount--;
}

int CRaklion::GetBossEggCount()
{
  return this->m_iBossEggCount;
}

void CRaklion::BossEggRegenAll()
{
	for ( int i=0;i<OBJ_MAXMONSTER;i++)
	{
		if (gObj[i].Class == 460 || 
			gObj[i].Class == 461 || 
			gObj[i].Class == 462 )
		{
			gObjSetMonster(gObj[i].m_Index,gObj[i].Class);

			if (gObjMonsterRegen(&gObj[i]))
			{
				gObj[i].Live = TRUE;
				gObj[i].DieRegen = FALSE;
				this->BossEggRegen();
			}
		}
	}

	LogAddC(5, "[ RAKLION ] [ BossEggRegen ] : BossEgg(%d ea)",this->GetBossEggCount());
}

void CRaklion::BossEggDeleteAll()
{
	for ( int i=0;i<OBJ_MAXMONSTER;i++)
	{
		if (gObj[i].Class == 460 || 
			gObj[i].Class == 461 || 
			gObj[i].Class == 462 )
		{
			gObjDel(i);
			gObj[i].Live = 0;
			gObj[i].m_State = 0;
			gObjViewportListProtocolDestroy(&gObj[i]);
			gObjViewportClose(&gObj[i]);
			this->BossEggDie();
		}
	}

	LogAddC(5, "[ RAKLION ] [ BossEggDelete ] : BossEgg(%d ea)",this->GetBossEggCount());
}

void CRaklion::CheckUserOnRaklionBossMap()
{
	for ( int iCount=OBJ_STARTUSERINDEX;iCount<OBJMAX;iCount++)
	{
		if ( gObj[iCount].Connected == PLAYER_PLAYING &&
			 gObj[iCount].Type == OBJ_USER &&
			 gObj[iCount].MapNumber == MAP_INDEX_RAKLION_BOSS)
		{
			if ( (gObj[iCount].Authority&2) != 2 )
			{
				if (g_RaklionBattleUserMng.IsBattleUser(iCount) != 0)
				{
					continue;
				}

				if (gObj[iCount].RegenOk == 0 &&
					gObj[iCount].m_State == 2 &&
					gObj[iCount].Live == TRUE)
				{
					gObjMoveGate(iCount, 286);
					LogAddC(2, "[ RAKLION ][ Invalid User ] Invalid Raklion Boss Map User[%s][%s]",
						gObj[iCount].AccountID, gObj[iCount].Name);
				}
			}	
		}
	}
}

int CRaklion::CheckEnterRaklion(int iUserIndex)
{
	if ( !gObjIsConnected(iUserIndex) )
		return -1;

	if( this->GetRaklionState() == RAKLION_STATE_CLOSE_DOOR )
	{
		LogAddTD("[ RAKLION ][ Entrance Fail ] [%s][%s] State(%d)",
			gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,this->GetRaklionState());
		
		return 3;
	}

	if ( gObj[iUserIndex].MapNumber != MAP_INDEX_RAKLION_BOSS ) 
	{
		LogAddTD("[ RAKLION ][ Entrance Fail ] Invalid Map Number(%d) [%s][%s] State(%d)",
			gObj[iUserIndex].MapNumber, gObj[iUserIndex].AccountID, gObj[iUserIndex].Name,this->GetRaklionState());

		return 4;
	}
	
	return 0;
}

void CRaklion::OperateGmCommand(int iUserIndex, int iCommand) //006475F0
{
	return;
}

//#include "LogToFile.h"
//CLogToFile RAKLION_TEST_LOG("Raklion", ".\\LOG\\Raklion", TRUE);//identical

void CRaklion::UserMonsterCountCheck()
{
#if(DEBUG_RAKLION == 0)
	return;
#endif

	int iUserCount_Live = 0;
	int iUserCount_Die = 0;

	for ( int iAllUserCount=OBJ_STARTUSERINDEX;iAllUserCount<OBJMAX;iAllUserCount++)
	{
		if ( gObj[iAllUserCount].MapNumber == MAP_INDEX_RAKLION_BOSS &&
			 gObj[iAllUserCount].Type == OBJ_USER &&
			 (gObj[iAllUserCount].Authority&2) != 2 )
		{
			if ( gObj[iAllUserCount].Live == TRUE )
			{
				iUserCount_Live++;
			}
			else
			{
				iUserCount_Die++;
			}
		}
	}

	//RAKLION_TEST_LOG.Output("[ RAKLION ][ Debug ] 2. All User Check:%d/(Die:%d)", iUserCount_Live, iUserCount_Die);

	int iMonsterCount=0;

	for ( int iAllMonsterCount=0;iAllMonsterCount<OBJ_STARTUSERINDEX;iAllMonsterCount++)
	{
		if ( gObj[iAllMonsterCount].MapNumber == MAP_INDEX_RAKLION_BOSS &&
			 gObj[iAllMonsterCount].Connected == PLAYER_PLAYING &&
			 gObj[iAllMonsterCount].Type == OBJ_MONSTER )
		{
			if ( gObj[iAllMonsterCount].Class != 459 )
			{
				iMonsterCount++;

				/*RAKLION_TEST_LOG.Output("[ RAKLION ][ Debug ] 4. Monster Info %s(%d/ObjIndex:%d) [%d]%d-%d HP:%0.1f IsAlive:%d Connected:%d State:%d ActState:Attack(%d)/Move(%d) DieRegen:%d",
					gObj[iAllMonsterCount].Name, gObj[iAllMonsterCount].Class,
					gObj[iAllMonsterCount].m_Index, gObj[iAllMonsterCount].MapNumber,
					gObj[iAllMonsterCount].X, gObj[iAllMonsterCount].Y,
					gObj[iAllMonsterCount].Life, gObj[iAllMonsterCount].Live,
					gObj[iAllMonsterCount].Connected, gObj[iAllMonsterCount].m_State,
					gObj[iAllMonsterCount].m_ActState.Attack, gObj[iAllMonsterCount].m_ActState.Move,
					gObj[iAllMonsterCount].DieRegen);*/
			}
		}
	}
}
