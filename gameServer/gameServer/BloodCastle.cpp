//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "GameMain.h"
#include "GameServer.h"
//#include "GameServerAuth.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "DSProtocol.h"
#include "Protocol.h"
#include "TNotice.h"
#include "..\common\winutil.h"
#include "ObjUseSkill.h"
#include "MixSystem.h"
#include "CastleSiegeSync.h"
#include "DSProtocol.h"
#include "CrywolfSync.h"
#include "MasterLevelSystem.h"
#include "BuffEffectSlot.h"
#include "ItemSocketOptionSystem.h"
#ifdef WZQUEST
#include "QuestExpProgMng.h"
#endif
#ifdef __CUSTOMS__
#include "ShopPointEx.h"
#include "ResetSystem.h"
#endif

CBloodCastle g_BloodCastle;


static const struct BLOOD_ZONE
{
	BYTE btStartX;
	BYTE btStartY;
	BYTE btEndX;
	BYTE btEndY;

} g_btCastleEntranceMapXY[MAX_BLOOD_CASTLE_LEVEL] = {

	13, 15, 15, 23,	// Blood Castle 1
	13, 15, 15, 23,	// Blood Castle 2
	13, 15, 15, 23,	// Blood Castle 3
	13, 15, 15, 23,	// Blood Castle 4
	13, 15, 15, 23,	// Blood Castle 5
	13, 15, 15, 23,	// Blood Castle 6
	13, 15, 15, 23,	// Blood Castle 7
	13, 15, 15, 23	// Blood Castle 8

}, g_btCastleBridgeMapXY[MAX_BLOOD_CASTLE_LEVEL] = {

	13, 70, 15, 75,	// Bridge of Blood Castle 1
	13, 70, 15, 75,	// Bridge of Blood Castle 2
	13, 70, 15, 75,	// Bridge of Blood Castle 3
	13, 70, 15, 75,	// Bridge of Blood Castle 4
	13, 70, 15, 75,	// Bridge of Blood Castle 5
	13, 70, 15, 75,	// Bridge of Blood Castle 6
	13, 70, 15, 75,	// Bridge of Blood Castle 7
	13, 70, 15, 75	// Bridge of Blood Castle 8

}, g_btCastleDoorMapXY[MAX_BLOOD_CASTLE_LEVEL][3]= {

	// Blood Castle 1
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 2
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 3
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 4
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 5
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 6
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 

	// Blood Castle 7
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83,	// Altar 
	
	// Blood Castle 8
	13, 76, 15, 79,	// Door Itself
	11, 80, 25, 89,	// Zone Beginh Door
	 8, 80, 10, 83	// Altar 
};

static const struct ST_REWARD_ZEN
{
	int NormalCharacter;
	int SpecialCharacter;

} g_iQuestWinExpendZEN[MAX_BLOOD_CASTLE_LEVEL] = {

	20000,	10000,
	50000,	25000,
	100000,	50000,
	150000,	80000,
	200000,	100000,
	250000,	120000,
	250000,	120000,
	250000,	120000
};

CBloodCastle::CBloodCastle() //0058FBD0
{
	this->m_bBC_EVENT_ENABLE = false;
	this->m_iBC_TIME_MIN_OPEN = 10;
	this->m_iBC_NORMAL_ITEM_DROP = 100;
	this->m_iBC_EXCEL_ITEM_DROP = 1200;
	this->m_iBC_MONSTER_REGEN = 0;

	for ( int i =0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		this->m_BridgeData[i].m_iBC_STATE = BC_STATE_NONE;
		this->m_BridgeData[i].m_iMapNumber = this->GetMapNumByBCBridge(i);
		this->m_BridgeData[i].m_iBridgeIndex = i;
		this->m_BridgeData[i].m_iBC_REMAIN_MSEC = -1;
		this->m_BridgeData[i].m_iBC_TICK_COUNT = -1;
		this->m_BridgeData[i].m_nSaintStatuePosNum = -1;
		this->m_BridgeData[i].m_nCastleDoorPosNum = -1;
		this->m_BridgeData[i].m_iAngelKingPosNum = -1;
		this->m_BridgeData[i].m_iBC_REWARD_EXP = 1.0f; //season 4.5 add-on
		this->m_BridgeData[i].m_BlockInfo = 1000;	//1.01.00
		InitializeCriticalSection(&this->m_BridgeData[i].m_critUserData);
		this->ClearBridgeData(i);
	}
}

CBloodCastle::~CBloodCastle()
{
	for ( int i =0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		DeleteCriticalSection(&this->m_BridgeData[i].m_critUserData);
	}
}


//Identical
void CBloodCastle::Init(bool bEVENT_ENABLE) //00547FD0
{
	this->m_bBC_EVENT_ENABLE = bEVENT_ENABLE;

	for ( int i=0; i<MAX_BLOOD_CASTLE_LEVEL ; i++ )
	{
		for ( int n=0;n<MAX_BLOOD_CASTLE_BOSS_MONSTER;n++)
		{
			this->m_BridgeData[i].m_iAngelKingPosNum = -1;
			this->m_BridgeData[i].m_nBossMonsterPosNum[n] = -1;
		}
	}

	for ( int n =0; n<gMSetBase.m_Count;n++)
	{
		if ( BC_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber ) != FALSE )
		{
			WORD wMonsterType = gMSetBase.m_Mp[n].m_Type; //season3 changed
			BYTE btMapNumber = gMSetBase.m_Mp[n].m_MapNumber;
			BYTE btBloodCastleIndex = this->GetBridgeIndexByMapNum(btMapNumber); //season3 changed

			if ( wMonsterType == 232 )
			{
				this->m_BridgeData[btBloodCastleIndex].m_iAngelKingPosNum = n;
				continue;
			}
			
			if ( BC_STATUE_RANGE(wMonsterType - 132) != FALSE )
			{
				this->m_BridgeData[btBloodCastleIndex].m_nSaintStatuePosNum = n;
				continue;
			}
			
			if ( wMonsterType == 131 )
			{
				this->m_BridgeData[btBloodCastleIndex].m_nCastleDoorPosNum = n;
				continue;
			}
			
			if ( wMonsterType == 89 || wMonsterType == 95 || wMonsterType == 112 || wMonsterType == 118 || wMonsterType == 124 || wMonsterType == 130 || wMonsterType == 143 || wMonsterType == 433 )
			{
				for ( int l=0;l<MAX_BLOOD_CASTLE_BOSS_MONSTER;l++)
				{
					if ( this->m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[l] == -1 )
					{
						this->m_BridgeData[btBloodCastleIndex].m_nBossMonsterPosNum[l] = n;
						break;
					}
				}
			}
		}

	}

	for ( int i=0; i < MAX_BLOOD_CASTLE_LEVEL; i++ )
	{
		this->SetState(i, BC_STATE_CLOSED);
	}
}

void CBloodCastle::Load(char* filename)
{
	int Token;
	int type;
	int BridgeCount;
	int iBridgeNum;
	// ----
	SMDFile=fopen(filename, "r");	//ok

	if (SMDFile == 0)
	{
		MsgBox("[Blood Castle] Info file Load Fail [%s]", filename);
		return;
	}

	type=-1;
	BridgeCount=-1;

	while ( true )
	{
		Token=GetToken();

		if ( Token == 2 )
		{
			break;
		}

		type=TokenNumber;

		while ( true )
		{
			if ( type == 0 )
			{
				Token=GetToken();

				if (strcmp("end", &TokenString[0]) == 0)
				{
					break;
				}

				this->m_iBC_TIME_MIN_OPEN = TokenNumber;

				Token = GetToken();
				this->m_iBC_TIME_MIN_PLAY =TokenNumber;

				Token = GetToken();
				this->m_iBC_TIME_MIN_REST = TokenNumber;
			}
			else if ( type == 1 )
			{
				Token = GetToken();

				if (strcmp("end", &TokenString[0]) == 0)
				{
					break;
				}

				this->m_iBC_NORMAL_ITEM_DROP = TokenNumber;

				Token = GetToken();
				this->m_iBC_EXCEL_ITEM_DROP = TokenNumber;

				Token = GetToken();
				this->m_iBC_MONSTER_REGEN = TokenNumber;
			}
			else if ( type == 2 )
			{
				Token = GetToken();

				if (strcmp("end", &TokenString[0]) == 0)
				{
					break;
				}
			
				iBridgeNum = TokenNumber;

				if ( BC_BRIDGE_RANGE(iBridgeNum) != FALSE )
				{
					Token = GetToken();
					this->m_BridgeData[iBridgeNum].m_iCastleStatueHealth = TokenNumber;

					Token = GetToken();
					this->m_BridgeData[iBridgeNum].m_iCastleDoorHealth = TokenNumber;
				}
				else
				{
					Token = GetToken();
					Token = GetToken();
				}
			}
			else if ( type == 3 )
			{
				Token = GetToken();

				if (strcmp("end", &TokenString[0]) == 0)
				{
					break;
				}
			
				iBridgeNum = TokenNumber;

				if ( BC_BRIDGE_RANGE(iBridgeNum) != FALSE )
				{
					Token = GetToken();
					this->m_BridgeData[iBridgeNum].m_iBC_REWARD_EXP = TokenNumber;
				}
				else
				{
					Token = GetToken();
				}
			}
			else if ( type == 4 )
			{
				Token = GetToken();

				if (strcmp("end", &TokenString[0]) == 0)
				{
					break;
				}

				BLOODCASTLE_START_TIME Schedule;

				Schedule.m_iHour = TokenNumber;

				Token = GetToken();
				Schedule.m_iMinute = TokenNumber;

				this->m_listBloodCastleOpenTime.push_back(Schedule);
			}
#ifdef __CUSTOMS__
			else if( type == 5 )
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", &TokenString[0]) )
				{
					break;
				}
				// ----
				BYTE CastleIndex = TokenNumber;
				// ----
				Token = GetToken();
				this->m_ItemReward[CastleIndex].ItemCount = TokenNumber;
				// ----
				Token = GetToken();
				WORD ItemID	= TokenNumber;
				// ----
				Token = GetToken();
				WORD ItemIndex = TokenNumber;
				// ----
				this->m_ItemReward[CastleIndex].ItemID = ITEMGET(ItemID, ItemIndex);
				// ----
				Token = GetToken();
				this->m_ItemReward[CastleIndex].ItemLevel = TokenNumber;
			}
#endif
		}
	}

	fclose(SMDFile);

	LogAdd("%s file load!", filename);
}

void CBloodCastle::LoadItemDropRate()
{
	g_bBloodCastle=GetPrivateProfileInt(COMMONSERVER_MAINSECTION, "BloodCastleEvent", 0, gDirPath.GetNewPath(COMMONSERVER_FILE));
	g_iAngelKingsPaperDropRate=GetPrivateProfileInt(COMMONSERVER_MAINSECTION, "AngelKingsPaperDropRate", 0, gDirPath.GetNewPath(COMMONSERVER_FILE));
	g_iBloodBoneDropRate=GetPrivateProfileInt(COMMONSERVER_MAINSECTION, "BloodBoneDropRate", 0, gDirPath.GetNewPath(COMMONSERVER_FILE));
	g_iStoneDropRate=GetPrivateProfileInt(COMMONSERVER_MAINSECTION, "StoneDropRate", 0, gDirPath.GetNewPath(COMMONSERVER_FILE));
	g_bStoneItemDrop=GetPrivateProfileInt(COMMONSERVER_MAINSECTION, "StoneItemDrop", 0, gDirPath.GetNewPath(COMMONSERVER_FILE));
	g_iBloodCastle_Prize=GetPrivateProfileInt(COMMONSERVER_MAINSECTION, "BloodCastlePrize", 0, gDirPath.GetNewPath(COMMONSERVER_FILE));
}

void CBloodCastle::CheckSync(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	std::list<BLOODCASTLE_START_TIME>::iterator it;
	BLOODCASTLE_START_TIME WebzenVar1;
	BLOODCASTLE_START_TIME WebzenVar2;
	int BaseTime = 0; //7
	int CheckTime = 0; //8
	DWORD CurrentTime = 0; //9
	
	tm * today; //10
	time_t ltime; //11

	int loc12;
	int loc13;

	this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

	if(this->m_listBloodCastleOpenTime.size() == 0)
	{
		MsgBox("Error : Blood Castle StartTime size is 0");
		return;
	}

	time(&ltime);
	today = localtime(&ltime);

	
	CurrentTime = (today->tm_hour * 60) + today->tm_min;
	WebzenVar1 = *this->m_listBloodCastleOpenTime.begin();

	for( it = this->m_listBloodCastleOpenTime.begin(); it != this->m_listBloodCastleOpenTime.end(); ++it )
	{
		WebzenVar2 = *it;
		BaseTime = (WebzenVar1.m_iHour * 60) + WebzenVar1.m_iMinute;
		CheckTime =	(WebzenVar2.m_iHour * 60) + WebzenVar2.m_iMinute;

		if( BaseTime == CheckTime )
		{
			if( CurrentTime < CheckTime )
			{
				WebzenVar2 = *it;
				break;
			}
			continue;
		}
		
		if( CurrentTime >= BaseTime && CurrentTime < CheckTime )
		{
			break;
		}
		else
		{
			WebzenVar1 = *it;
		}
	}

	for(loc12 = 2;loc12--;)
	{
		if(it == this->m_listBloodCastleOpenTime.end())
		{
			it = this->m_listBloodCastleOpenTime.begin();

			WebzenVar2 = (*it);
		}

		CheckTime = WebzenVar2.m_iHour*60+WebzenVar2.m_iMinute;

		if(today->tm_hour <= WebzenVar2.m_iHour && CheckTime > CurrentTime)
		{
			this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = ((CheckTime - CurrentTime)*60)*1000;
		}
		else
		{
			this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = ((1440-CurrentTime+CheckTime)*60)*1000;
		}

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ))
		{
			it++;

			if(it != this->m_listBloodCastleOpenTime.end())
			{
				WebzenVar2 = (*it);
			}
		}
		else
		{
			break;
		}
	}

	loc13 = GetTickCount();
	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= today->tm_sec * 1000;

	LogAddTD("[Blood Castle] (%d) Sync Open Time. [%d] min remain", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC/60000);
}

void CBloodCastle::ClearBridgeData(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iTOTAL_EXP = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_SUCCESS_MSG_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT = 0;
	this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER = 0;
	this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = -1;
	this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = -1;
	this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = -1;
	this->m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE = TRUE;
	this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_QUIT = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE = false;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index = -10;
	this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party = -10;

	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName , 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_CharName));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID));
	memset(this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID));
	memset(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID, 0, sizeof(this->m_BridgeData[iBridgeIndex].m_szWin_Quest_AccountID));
	

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP = 0;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex = -1;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex = -1;
			gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_bBloodCastleComplete = false;
		}

		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState= 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = false;
		this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak = false;
		this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = -1;
		this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX = -1; //season 2.5 add-on
		this->m_BridgeData[iBridgeIndex].m_BlockInfo = 1000;	//1.01.00
	}
}

void CBloodCastle::SetState(int iBridgeIndex, int iBC_STATE)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	if ( iBC_STATE < BC_STATE_NONE || iBC_STATE > BC_STATE_PLAYEND )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_STATE = iBC_STATE;

	switch ( this->m_BridgeData[iBridgeIndex].m_iBC_STATE )
	{
		case BC_STATE_NONE:
			this->SetState_None(iBridgeIndex);
			break;

		case BC_STATE_CLOSED:
			this->SetState_Closed(iBridgeIndex);
			break;

		case BC_STATE_PLAYING:
			this->SetState_Playing(iBridgeIndex);
			break;

		case BC_STATE_PLAYEND:
			this->SetState_PlayEnd(iBridgeIndex);
			break;
	}
}

void CBloodCastle::Run()
{
	if ( this->m_bBC_EVENT_ENABLE != false )
	{
		for (int iBridgeIndex=0;iBridgeIndex<MAX_BLOOD_CASTLE_LEVEL;iBridgeIndex++)
		{
			switch ( this->m_BridgeData[iBridgeIndex].m_iBC_STATE )
			{
				case BC_STATE_NONE:
					this->ProcState_None(iBridgeIndex);
					break;

				case BC_STATE_CLOSED:
					this->ProcState_Closed(iBridgeIndex);
					break;

				case BC_STATE_PLAYING:
					this->ProcState_Playing(iBridgeIndex);
					break;

				case BC_STATE_PLAYEND:
					this->ProcState_PlayEnd(iBridgeIndex);
					break;
			}
		}
		if ( szAuthKey[13] != AUTHKEY13 )
		{
			DestroyGIocp();
		}
	}

}

void CBloodCastle::ProcState_None(int iBridgeIndex)
{
}

void CBloodCastle::ProcState_Closed(int iBridgeIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

		if ( g_bBloodCastle != FALSE )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ) && this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER == false)
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = true;
				this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = true;
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ) && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && (this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC/60000) !=this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT)
			{
				this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 60000;

				if ( iBridgeIndex == 0 )
				{
					PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get( MSGGET(4, 136)), this->m_BridgeData[iBridgeIndex].m_iBC_NOTIFY_COUNT+1);
					this->SendAllUserAnyMsg((LPBYTE)&pNotice, pNotice.h.size);
				}
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER == false )
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_ENTER = true;

				if ( iBridgeIndex == 0 )
				{
					PMSG_SET_DEVILSQUARE pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
					pMsg.Type = 3;

					for (int i= OBJ_STARTUSERINDEX;i<OBJMAX;i++)
					{
						if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
						{
							if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
								{
									DataSend(i,(LPBYTE)&pMsg, pMsg.h.size);
								}
							}
						}
					}
				}
			}
		}
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0 )
	{
		if ( g_bBloodCastle != FALSE )
		{
			this->SetState(iBridgeIndex, BC_STATE_PLAYING);
		}
		else
		{
			this->SetState(iBridgeIndex, BC_STATE_CLOSED);
		}
	}
}

void CBloodCastle::ProcState_Playing(int iBridgeIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ((this->m_iBC_TIME_MIN_PLAY*60-30)*1000) )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY == false )
			{
				this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_PLAY = true;
				
				PMSG_SET_DEVILSQUARE pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = 4;
				this->SendBridgeAnyMsg((LPBYTE)&pMsg, sizeof(pMsg), iBridgeIndex);
			}
		}

		if ( this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE != false	)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN != -1 )
			{
				if ( GetTickCount() > this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN )
				{
					this->ReleaseCastleBridge(iBridgeIndex);
					this->SendCastleBridgeBlockInfo(iBridgeIndex, 0);
					LogAddTD("[Blood Castle] (%d) Bridge Change Bridge Attribute -> Open", iBridgeIndex+1);
					this->m_BridgeData[iBridgeIndex].m_dwBC_TICK_DOOR_OPEN = -1; // Prevent multiple openings
					this->m_BridgeData[iBridgeIndex].m_BlockInfo = BC_BLOCK_PLAYING;	//1.01.00

					if(this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX == -1) //season 2.5 add-on
					{
						LogAddTD("[Blood Castle] (%d) Door Not Subsist", iBridgeIndex+1);
					}
					else
					{
						LogAddTD("[Blood Castle] (%d) Door Subsist (%d)(Name: %s)(Dieregen:%d)", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX, gObj[this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX].Name, gObj[this->m_BridgeData[iBridgeIndex].m_iBC_DOOR_MONSTER_INDEX].DieRegen);
					}
				}
			}
		}

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ((this->m_iBC_TIME_MIN_PLAY*60-60)*1000) && this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START == false )
		{
			PMSG_NOTICE pNotice;

			this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = (this->m_iBC_TIME_MIN_PLAY*60)*1000;
			this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY = false;
			TNotice::MakeNoticeMsgEx((TNotice*)&pNotice, 0, lMsg.Get(MSGGET(4, 137)), iBridgeIndex+1);
			this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);
			this->ReleaseCastleEntrance(iBridgeIndex);
			this->SendCastleEntranceBlockInfo(iBridgeIndex, 0);
			this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START = true;
			this->SetMonster(iBridgeIndex);
			this->m_BridgeData[iBridgeIndex].m_BlockInfo = BC_BLOCK_CLOSED;	//1.01.00
			this->SendNoticeState(iBridgeIndex, false);

			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Start", iBridgeIndex+1);
		}

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END == false)	// Set counter to kick
		{
			this->m_BridgeData[iBridgeIndex].m_bBC_MSG_BEFORE_END = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 5;
			this->SendBridgeAnyMsg((LPBYTE)&pMsg, sizeof(pMsg), iBridgeIndex);
		}

		if ( this->CheckEveryUserDie(iBridgeIndex) != false )
		{
			PMSG_NOTICE pNotice;

			TNotice::MakeNoticeMsg(&pNotice, 0, lMsg.Get(MSGGET(4, 138)));
			this->SendBridgeAnyMsg( (LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);

			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName);
			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName);

			this->GiveReward_Fail(iBridgeIndex);
			this->SetState(iBridgeIndex, BC_STATE_CLOSED);

			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Failed -> Every User Out", iBridgeIndex+1);
		}
		else
		{
			if ( this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START != false )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_bBC_DOOR_TERMINATE_COMPLETE == false || this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE != false )
				{
					this->SendNoticeState(iBridgeIndex, 1);
				}
				else
				{
					this->SendNoticeState(iBridgeIndex, 4);
				}
			}
		}

		if ( this->CheckWinnerExist(iBridgeIndex) == true )
		{
			if ( this->CheckWinnerValid(iBridgeIndex) == true )
			{
				if ( this->CheckWinnerPartyComplete(iBridgeIndex) == true )
				{
					this->GiveReward_Win(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX, iBridgeIndex);
					this->SetState(iBridgeIndex, BC_STATE_PLAYEND);

					LogAddTD("[Blood Castle] (%d) CheckWinnerPartyComplete(iBridgeIndex) == true", iBridgeIndex+1);
					return;
				}
			}
			else
			{
				this->GiveReward_Fail(iBridgeIndex);
				this->SetState(iBridgeIndex, BC_STATE_PLAYEND); 

				LogAddTD("[Blood Castle] (%d) CheckWinnerValid(iBridgeIndex) == false", iBridgeIndex+1);
				return;
			}
		}
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0 )
	{
		if ( this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED == false )
		{
			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Castle Door [%s][%s]", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Door_AccountID, this->m_BridgeData[iBridgeIndex].m_szKill_Door_CharName);
			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Fail Result -> Destroy Saint Status [%s][%s]", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_szKill_Status_AccountID, this->m_BridgeData[iBridgeIndex].m_szKill_Status_CharName);

			this->GiveReward_Fail(iBridgeIndex);

			LogAddTD("[Blood Castle] (%d) Blood Castle Quest Failed -> Time Out", iBridgeIndex+1);
			
		}

		this->SetState(iBridgeIndex, BC_STATE_PLAYEND);
	}
}

void CBloodCastle::ProcState_PlayEnd(int iBridgeIndex)
{
	int iTICK_MSEC = GetTickCount() - this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC -= iTICK_MSEC;
		this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = GetTickCount();

		if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 30000 && this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC > 0 && this->m_BridgeData[iBridgeIndex]. m_bBC_MSG_BEFORE_QUIT == false)
		{
			this->m_BridgeData[iBridgeIndex]. m_bBC_MSG_BEFORE_QUIT = true;

			PMSG_SET_DEVILSQUARE pMsg;

			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 6;

			this->SendBridgeAnyMsg((LPBYTE)&pMsg, sizeof(pMsg), iBridgeIndex);
		}


	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= 0 )
	{
		this->SetState(iBridgeIndex, BC_STATE_CLOSED);
	}
}

//Identical
void CBloodCastle::SetState_None(int iBridgeIndex) //0054A820
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = -1;
	this->m_BridgeData[iBridgeIndex].m_iBC_TICK_COUNT = -1;
	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearBridgeData(iBridgeIndex);
	this->ClearMonster(iBridgeIndex, 1);

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

		if ( gObj[n].MapNumber == iMapNumber && gObj[n].Connected == PLAYER_PLAYING ) //season3 changed
		{
			gObjMoveGate(n, 22);
		}
	}

	this->BlockCastleDoor(iBridgeIndex);
}

//Identical
void CBloodCastle::SetState_Closed(int iBridgeIndex) //0054A960
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearBridgeData(iBridgeIndex);
	this->ClearMonster(iBridgeIndex, 1);
	this->CheckAngelKingExist(iBridgeIndex);

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

		if ( gObj[n].MapNumber == iMapNumber && gObj[n].Connected > PLAYER_LOGGED ) //season3 changed
		{
			this->SearchUserDeleteQuestItem(n);
			gObjMoveGate(n, 22);
		}
	}

	this->BlockCastleDoor(iBridgeIndex);
	this->BlockCastleBridge(iBridgeIndex);
	this->BlockCastleEntrance(iBridgeIndex);
	this->CheckSync(iBridgeIndex);
	LogAddTD("[Blood Castle] (%d) SetState CLOSED", iBridgeIndex+1);
}

//Identical
void CBloodCastle::SetState_Playing(int iBridgeIndex) //0054AAD0
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = this->m_iBC_TIME_MIN_PLAY*60*1000;
	this->CheckUsersOnConnect(iBridgeIndex);

	PMSG_NOTICE pNotice;

	TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 139)), iBridgeIndex+1, 60);
	this->SendBridgeAnyMsg((LPBYTE)&pNotice, pNotice.h.size, iBridgeIndex);

	PMSG_SERVERCMD ServerCmd;

	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = 1;
	ServerCmd.X = 45;
	ServerCmd.Y = 0;

	this->SendBridgeAnyMsg((LPBYTE)&ServerCmd, ServerCmd.h.size, iBridgeIndex);

	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].MapNumber == this->GetMapNumByBCBridge(iBridgeIndex) && gObj[n].Connected > PLAYER_LOGGED ) //season3 changed
		{
			this->SearchUserDeleteQuestItem(n);
		}
	}

	LogAddTD("[Blood Castle] (%d) SetState PLAYING", iBridgeIndex+1);
}

void CBloodCastle::SetState_PlayEnd(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SendNoticeState(iBridgeIndex, 2);
	this->ClearMonster(iBridgeIndex, 0);
	this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER = false;
	this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC = this->m_iBC_TIME_MIN_REST*60*1000;

	LogAddTD("[Blood Castle] (%d) SetState PLAYEND", iBridgeIndex+1);

	for (int n=0;n<MAX_BLOOD_CASTLE_SUB_BRIDGE;n++)
	{
		if ( OBJMAX_RANGE(this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex) != FALSE )
		{
			this->SearchUserDeleteQuestItem(this->m_BridgeData[iBridgeIndex].m_UserData[n].m_iIndex);
		}
	}
}

int CBloodCastle::GetCurrentState(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	return this->m_BridgeData[iBridgeIndex].m_iBC_STATE;
}

int CBloodCastle::GetCurrentRemainSec(int iBridgeIndex)
{
	return this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
}

//005ef3c0
int  CBloodCastle::CheckEnterLevel(int iIndex, int iLevel)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 2;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 2;
	}

	if ( gObj[iIndex].Class == CLASS_DARKLORD 
#ifdef MONK
		|| gObj[iIndex].Class == CLASS_MONK
#endif
		|| gObj[iIndex].Class == CLASS_MAGUMSA )
	{
		if ( gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA && gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
		{
			return 0;
		}

		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND_MAGUMSA )
		{
			return -1;
		}

		if ( gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND_MAGUMSA )
		{
			return 1;
		}
	}
	else
	{
		if ( gObj[iIndex].Level >= g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND && gObj[iIndex].Level <= g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
		{
			return 0;
		}

		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[iLevel-1].iLOWER_BOUND )
		{
			return -1;
		}

		if ( gObj[iIndex].Level > g_sttBLOODCASTLE_LEVEL[iLevel-1].iUPPER_BOUND )
		{
			return 1;
		}
	}

	return 2;
}

bool CBloodCastle::CheckEnterFreeTicket(int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex) )
		return false;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		return false;

	for (int x=0;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory [x].m_Type == ITEMGET(13,47) )
			{
				return true;
			}
		}
	}

	return false;
}

static const int g_iBC_ChoasMixSuccessRate[MAX_BLOOD_CASTLE_LEVEL] = { 80, 80, 80, 80, 80, 80, 80, 80 };
static const int g_iBC_ChoasMixMoney[MAX_BLOOD_CASTLE_LEVEL] = {50000 , 80000 , 150000, 250000, 400000, 600000, 850000, 1050000};

//Identical
bool CBloodCastle::BloodCastleChaosMix(int iIndex, int iLEVEL) //0054B2A0
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	int iMapNumber = this->GetMapNumByBCBridge(iLEVEL-1); //Season3 add-on

	if ( BC_MAP_RANGE(iMapNumber)  == FALSE ) //season3 changed
	{
		return false;
	}

	BOOL bMIX_RESULT = FALSE;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR; //
	gObj[iIndex].ChaosLock = TRUE;

	LogAddTD("[Blood Castle] ???????? Mix Chaos Mix Start (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);

	char szTemp[17];
	wsprintf(szTemp, "???????? Mix,%d", iLEVEL); //Season 2.5 add-on

	g_MixSystem.LogChaosItem(&gObj[iIndex], szTemp);

	int iMIX_SUCCESS_RATE = g_iBC_ChoasMixSuccessRate[iLEVEL - 1];

	if ( iMIX_SUCCESS_RATE < 0 || iMIX_SUCCESS_RATE > 100 )
	{
		DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Blood Castle] ???????? Mix Chaos Mix Failed - MixRate Out of Bound (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
		return false;
	}

	if ( g_CrywolfSync.GetOccupationState() == 0 && g_iCrywolfApplyMvpBenefit )
	{
		iMIX_SUCCESS_RATE += g_CrywolfSync.GetPlusChaosRate();
	}

	if ( iMIX_SUCCESS_RATE > 80 )
	{
		iMIX_SUCCESS_RATE = 80;
	}

	if ( gObj[iIndex].ChaosSuccessRate > 10 )
	{
		pMsg.Result = 0xF0;
		gObj[iIndex].ChaosLock = FALSE;

		DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	iMIX_SUCCESS_RATE += gObj[iIndex].ChaosSuccessRate;
	int iMIX_NEED_MONEY = g_iBC_ChoasMixMoney[iLEVEL - 1];
	int iChaosTaxMoney = (int)((__int64)(iMIX_NEED_MONEY) * (__int64)(g_CastleSiegeSync.GetTaxRateChaos(iIndex)) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iMIX_NEED_MONEY += iChaosTaxMoney;

	if ( iMIX_NEED_MONEY <  0 )
	{
		DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Blood Castle] ???????? Mix Chaos Mix Failed - MixMoney < 0 (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
		return false;
	}

	if ( (gObj[iIndex].Money - iMIX_NEED_MONEY) < 0 )
	{
		pMsg.Result = CB_BC_NOT_ENOUGH_ZEN;
		DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Blood Castle] ???????? Mix Chaos Mix Failed - Not Enough Money (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL);
		return false;
	}

	gObj[iIndex].Money -= iMIX_NEED_MONEY;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(iIndex, gObj[iIndex].Money);

	if ( (rand()%100) < iMIX_SUCCESS_RATE )
	{
		int item_num = ITEMGET(13,18);
		ItemSerialCreateSend(iIndex, -1, 0, 0, item_num, iLEVEL, 255, 0, 0, 0, -1, 0, 0);
		LogAddTD("[???????? Mix] [%s][%s] CBMix Success %d Money : %d-%d", gObj[iIndex].AccountID, gObj[iIndex].Name, iMIX_SUCCESS_RATE, gObj[iIndex].Money, iMIX_NEED_MONEY);
	}
	else
	{
		g_MixSystem.ChaosBoxInit(&gObj[iIndex]);
		GCUserChaosBoxSend(&gObj[iIndex], 0);
		DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[???????? Mix] [%s][%s] CBMix Fail %d Money : %d-%d", gObj[iIndex].AccountID, gObj[iIndex].Name, iMIX_SUCCESS_RATE, gObj[iIndex].Money, iMIX_NEED_MONEY);
		return false;
	}

	::gObjInventoryCommit(iIndex);

	return true;
}

static const struct ST_BC_EVENT_SCORE
{
	int unk0;
	int unk4;
	int unk8;
	int unkC;
	int unk10;

} g_iBC_EventScore[MAX_BLOOD_CASTLE_LEVEL] = 
{
	600, 300, 1000, 800, 400,
	600, 300, 1000, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,         
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400,
	600, 300, 1005, 800, 400 //8
};

static const int g_iBC_EventScore_Fail[MAX_BLOOD_CASTLE_LEVEL] = { -300,-300,-300,-300,-300,-300,-300,-300 };	

static const struct  ST_BC_ADD_EXP
{
	int unk0;
	int unk4;
	int unk8;
	int unkC;

} g_iBC_Add_Exp[MAX_BLOOD_CASTLE_LEVEL] =
{
    20000, 20000, 5000, 160,
	50000, 50000, 10000, 180,
	80000, 80000, 15000, 200,
	90000, 90000, 20000, 220,
	100000, 100000, 25000, 240,
	110000, 110000, 30000, 260,
	120000, 120000, 35000, 280,
	130000, 130000, 40000, 300 //BC8
};

static const int g_iBC_Party_EventPoint[MAX_USER_IN_PARTY]	= {5, 10, 15, 20, 30 };

static const int g_iBC_MONSTER_CHANGE_STATE[2][3] =
{
	//	DamageMin	DamageMax	Defense
	10,	20,	-14,
	20, 40, -32
};

//005efe70
int CBloodCastle::CheckChoasMixItem(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return -1;
	}

	int iCHAOS_MIX_LEVEL = 0;
	BOOL bIsChaosGemExist = FALSE;
	BOOL bIsAngelKingPaperExist = FALSE;
	BOOL bIsBloodBoneExist = FALSE;
	BOOL bIsOtherItemExist = FALSE;
	int iEventItemCount = 0;
	int iAngelKingPaperLevel = 0;
	int iBloodBoneLevel = 0;
	int iCharmOfLuckCount=0;

	for ( int i=0;i<CHAOS_BOX_SIZE;i++)
	{
		if ( gObj[iIndex].pChaosBox[i].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(12,15) )
			{
				bIsChaosGemExist = TRUE;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13,16) )
			{
				int iSCROLL_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsAngelKingPaperExist = TRUE;
				iAngelKingPaperLevel = iSCROLL_LEVEL;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13,17) )
			{
				int iBLOOD_BONE_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsBloodBoneExist = TRUE;
				iBloodBoneLevel = iBLOOD_BONE_LEVEL;
			}
			else if ( gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(14,53) )
			{
				iCharmOfLuckCount += gObj[iIndex].pChaosBox[i].m_Durability;
			}
			else
			{
				bIsOtherItemExist = TRUE;
			}
		}
	}

	gObj[iIndex].ChaosSuccessRate = iCharmOfLuckCount;

	if ( bIsOtherItemExist != FALSE )
	{
		return -8;
	}

	if ( bIsAngelKingPaperExist == FALSE && bIsBloodBoneExist == FALSE )
	{
		return -1;
	}

	if ( bIsAngelKingPaperExist == FALSE || bIsBloodBoneExist == FALSE )
	{
		return -11;
	}

	if ( iEventItemCount > 2 )
	{
		return -12;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		return -15;
	}

	if ( iAngelKingPaperLevel != iBloodBoneLevel )
	{
		return -9;
	}

	if ( BC_BRIDGE_RANGE(iAngelKingPaperLevel-1) == FALSE )
	{
		return -9;
	}

	if ( BC_BRIDGE_RANGE(iBloodBoneLevel-1) == FALSE )
	{
		return -9;
	}

	if ( bIsChaosGemExist == FALSE )
	{
		return -10;
	}

	if ( gObj[iIndex].Class == CLASS_DARKLORD 
#ifdef MONK
		|| gObj[iIndex].Class == CLASS_MONK
#endif
		|| gObj[iIndex].Class == CLASS_MAGUMSA )
	{
		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND_MAGUMSA )
		{
			return -14;
		}
	}
	else
	{
		if ( gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND )
		{
			return -14;
		}
	}

	if ( bIsChaosGemExist != FALSE && bIsAngelKingPaperExist != FALSE && bIsBloodBoneExist != FALSE )
	{
		return iAngelKingPaperLevel;
	}

	return -1;
}

int CBloodCastle::CheckEnterItem(int iIndex)
{
	int iITEM_LEVEL = 0;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return 0;
	}

	for ( int x=0;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,18) )
			{
				iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( CHECK_LIMIT(iITEM_LEVEL, MAX_CLOACK_LEVEL) == FALSE )
				{
					iITEM_LEVEL = 0;

				}

				if ( iITEM_LEVEL != 0 )
				{
					return iITEM_LEVEL;
				}
			}
			
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,47) )
			{
				iITEM_LEVEL = 10;
			}
		}
	}

	return iITEM_LEVEL;
}

//Identical
int CBloodCastle::CheckQuestItem(int iIndex) //0054BEC0
{
	int iITEM_LEVEL = -1;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return -1;
	}

	if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) == FALSE )
	{
		return -1;
	}

	int iBridgeIndex = this->GetBridgeIndexByMapNum(gObj[iIndex].MapNumber); //season3 add-on

	if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1 )
	{
		return -1;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return -1;
	}

	for ( int x=0;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
			{
				if ( gObj[iIndex].pInventory[x].m_Number == this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL )
				{
					iITEM_LEVEL = gObj[iIndex].pInventory[x].m_Level;

					if ( iITEM_LEVEL < 0 || iITEM_LEVEL > 2 )
					{
						iITEM_LEVEL = -1;
					}

					break;
				}
			}
		}
	}

	return iITEM_LEVEL;
}

bool CBloodCastle::CheckWalk(int iIndex, int iMoveX, int iMoveY)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}
	
	if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) == FALSE )
	{
		return false;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}

	if ( this->GetCurrentState(gObj[iIndex].MapNumber) == TRUE )
	{
		BYTE btMapAttr = MapC[gObj[iIndex].MapNumber].GetAttr(iMoveX, iMoveY);

		if ( (btMapAttr&1) != 1 )
		{
			return true;
		}
	}

	return false;
}

bool CBloodCastle::CheckCanEnter(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_CAN_ENTER;
}

bool CBloodCastle::CheckCanParty(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_CAN_PARTY;
}

bool CBloodCastle::CheckQuestItemSerial(int iBridgeIndex, CMapItem * lpItem)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == -1 )
	{
		return false;
	}
	
	if ( lpItem->IsItem() == TRUE )
	{
		if ( lpItem->m_Type == ITEMGET(13,19) )
		{
			int iLEVEL = lpItem->m_Level;

			if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == lpItem->m_Number )
				{
					return true;
				}
			}
		}
	}
	

	return false;

}

bool CBloodCastle::CheckPlayStart(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_PLAY_START;
}

int  CBloodCastle::GetRemainTime(int iBridgeIndex) //005948A0
{
	int iREMAIN_MINUTE = 0;

	if ( this->GetCurrentState(iBridgeIndex) == 1 )
	{
		iREMAIN_MINUTE = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 60000 - this->m_iBC_TIME_MIN_OPEN + 1;
	}
	else
	{
		std::list<BLOODCASTLE_START_TIME>::iterator it; //loc3 -> ebp C
		BLOODCASTLE_START_TIME WebzenVar1; //loc 4-5 -> ebp 14
		BLOODCASTLE_START_TIME WebzenVar2; //loc 6-7 -> epb 18
		int BaseTime = 0; // loc8 -> ebp 1C
		int CheckTime = 0; // loc9 -> ebp 20
		DWORD CurrentTime = 0;	// loc10-> ebp 24
		
		tm * today; //loc11 -> ebp 28
		time_t ltime; //loc12 -> ebp 2C

		int loc13; //ebp 34
		int loc14; //ebp 38

		time(&ltime); //
		today = localtime(&ltime); //
		
		CurrentTime = (today->tm_hour * 60) + today->tm_min;
		WebzenVar1 = *this->m_listBloodCastleOpenTime.begin();

		for( it = this->m_listBloodCastleOpenTime.begin(); it != this->m_listBloodCastleOpenTime.end(); ++it ) //for identical
		{
			WebzenVar2 = *it; // loc5-6
			BaseTime = (WebzenVar1.m_iHour * 60) + WebzenVar1.m_iMinute;
			CheckTime =	(WebzenVar2.m_iHour * 60) + WebzenVar2.m_iMinute;

			if( BaseTime == CheckTime )
			{
				if( CurrentTime < CheckTime )
				{
					WebzenVar2 = *it;
					break;
				}
				continue;
			}
			
			if( CurrentTime >= BaseTime && CurrentTime < CheckTime )
			{
				break;
			}
			else
			{
				WebzenVar1 = *it;
			}
		}

		for(loc13 = 2;loc13--;) //good ->func identical so far
		{
			if(it == this->m_listBloodCastleOpenTime.end())
			{
				it = this->m_listBloodCastleOpenTime.begin();

				WebzenVar2 = (*it);
			}

			CheckTime = WebzenVar2.m_iHour*60+WebzenVar2.m_iMinute;

			if(today->tm_hour <= WebzenVar2.m_iHour && CheckTime > CurrentTime)
			{
				iREMAIN_MINUTE = ((CheckTime - CurrentTime)*60)*1000;
			}
			else
			{
				iREMAIN_MINUTE = ((1440-CurrentTime+CheckTime)*60)*1000;
			}

			if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 )) //should be if ( iREMAIN_MINUTE <= ( this->m_iBC_TIME_MIN_OPEN * 60 * 1000 ))
			{
				it++;

				if(it != this->m_listBloodCastleOpenTime.end())
				{
					WebzenVar2 = (*it);
				}
			}
			else
			{
				break;
			}
		}
		loc14 = GetTickCount();
		iREMAIN_MINUTE = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC - today->tm_sec * 1000;
	}

	return iREMAIN_MINUTE;
}

//Identical
void CBloodCastle::ClearMonster(int iBridgeIndex, bool bClearCastleDoor) //0054C700
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].MapNumber == this->GetMapNumByBCBridge(iBridgeIndex))
		{
			if ( bClearCastleDoor == false && gObj[n].Class == 131)
			{
				continue;
			}

			if ( gObj[n].Class == 232 )
			{
				continue;
			}

			gObjDel(n);
		}
	}
}

//Identical
void CBloodCastle::SetMonster(int iBridgeIndex) //0054C810
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	this->SetMonsterKillCount(iBridgeIndex);
	int nCastleDoorPosNum = this->m_BridgeData[iBridgeIndex].m_nCastleDoorPosNum;

	WORD wMonsterType = gMSetBase.m_Mp[nCastleDoorPosNum].m_Type; //season3 changed
	BYTE btMapNumber = gMSetBase.m_Mp[nCastleDoorPosNum].m_MapNumber;
	BYTE btBloodCastleIndex = this->GetBridgeIndexByMapNum(btMapNumber); //season3 changed

	if ( nCastleDoorPosNum != -1 )
	{
		if ( wMonsterType == 131 )
		{
			int iIndex = ::gObjAddMonster(btMapNumber);

			if ( iIndex >= 0 )
			{
				::gObjSetPosMonster(iIndex, nCastleDoorPosNum);
				::gObjSetMonster(iIndex, wMonsterType);

				gObj[iIndex].m_cBloodCastleIndex = btBloodCastleIndex;
				gObj[iIndex].m_ItemRate = this->m_iBC_NORMAL_ITEM_DROP;
				gObj[iIndex].Dir = 1;
				gObj[iIndex].m_PosNum = -1;
				gObj[iIndex].Live = TRUE;
				gObj[iIndex].DieRegen = FALSE;
				gObj[iIndex].m_State = 1;
				gObj[iIndex].MaxRegenTime = 0;
				gObj[iIndex].MaxLife = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
				gObj[iIndex].Life = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;

				LogAddTD("[Blood Castle] (%d) Create Door Successfuly : Index(%d)", btBloodCastleIndex+1, this->m_BridgeData[btBloodCastleIndex].m_iBC_DOOR_MONSTER_INDEX = iIndex); //season 2.5 add-on
			}
			else
			{
				LogAddTD("[Blood Castle] (%d) Create Door Fail", btBloodCastleIndex+1); //season 2.5 add-on
			}
		}
		else
		{
			LogAddTD("[Blood Castle] (%d) Door monster type Error", btBloodCastleIndex+1); //season 2.5 add-on
		}
	}

	int result;
	for ( int n=0;n<gMSetBase.m_Count;n++)
	{
		if ( BC_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE  )
		{
			WORD wIndex = gMSetBase.m_Mp[n].m_Type;
			BYTE btMap   = gMSetBase.m_Mp[n].m_MapNumber;
			BYTE btBridgeIndex = this->GetBridgeIndexByMapNum(btMap); //season3 changed
			
			if ( btBridgeIndex != iBridgeIndex )
			{
				continue;
			}

			if ( wIndex == 232 )
			{
				continue;
			}

			if ( wIndex == 131 )
			{
				continue;
			}

			if ( wIndex == 89 || wIndex == 95 || wIndex == 112  || wIndex == 118 || wIndex == 124 || wIndex == 130 || wIndex == 143 || wIndex == 433 ) //season3 changed
			{
				continue;
			}

			if ( BC_STATUE_RANGE(wIndex-132) != FALSE )
			{
				continue;
			}

			result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

			if ( result >= 0 )
			{
				gObj[result].m_PosNum = n;
				gObj[result].X = gMSetBase.m_Mp[n].m_X;
				gObj[result].Y = gMSetBase.m_Mp[n].m_Y;
				gObj[result].MapNumber = gMSetBase.m_Mp[n].m_MapNumber;
				gObj[result].TX = gObj[result].X;
				gObj[result].TY = gObj[result].Y;
				gObj[result].m_OldX = gObj[result].X;
				gObj[result].m_OldY = gObj[result].Y;
				gObj[result].Dir = gMSetBase.m_Mp[n].m_Dir;
				gObj[result].StartX = gObj[result].X;
				gObj[result].StartY = gObj[result].Y;
				gObjSetMonster(result, wIndex);
				gObj[result].MaxRegenTime = this->m_iBC_MONSTER_REGEN;
				gObj[result].m_cBloodCastleIndex = btBridgeIndex;
				gObj[result].Dir = rand() % 8;
			}
		}
	}
	
}

//Identical
void CBloodCastle::SetBossMonster(int iBridgeIndex) //0054CF50
{
	int result;

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<MAX_BLOOD_CASTLE_BOSS_MONSTER;x++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x] != -1 )
		{
			int nBossMonsterPosNum = this->m_BridgeData[iBridgeIndex].m_nBossMonsterPosNum[x];

			WORD wMonsterType = gMSetBase.m_Mp[nBossMonsterPosNum].m_Type; //season3 changed
			BYTE btMapNumber = gMSetBase.m_Mp[nBossMonsterPosNum].m_MapNumber;
			BYTE btBloodCastleIndex = this->GetBridgeIndexByMapNum(btMapNumber); //season3 changed

			if ( btBloodCastleIndex != iBridgeIndex )
			{
				continue;
			}

			if ( wMonsterType == 89 || wMonsterType == 95 || wMonsterType == 112 || wMonsterType == 118 || wMonsterType == 124 || wMonsterType == 130 || wMonsterType == 143 || wMonsterType == 433)
			{
				result = ::gObjAddMonster(btMapNumber);

				if ( result >= 0 )
				{
					::gObjSetPosMonster(result, nBossMonsterPosNum);
					::gObjSetMonster(result, wMonsterType);
					gObj[result].MaxRegenTime = this->m_iBC_MONSTER_REGEN;
					gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
					gObj[result].m_ItemRate = this->m_iBC_NORMAL_ITEM_DROP;
					gObj[result].Dir = rand() % 8;
				}
			}
		}
	}
}

//Identical
void CBloodCastle::SetSaintStatue(int iBridgeIndex) //0054D1E0
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int nSaintStatuePosNum = this->m_BridgeData[iBridgeIndex].m_nSaintStatuePosNum;

	WORD wMonsterType = gMSetBase.m_Mp[nSaintStatuePosNum].m_Type; //season3 changed
	BYTE btMapNumber = gMSetBase.m_Mp[nSaintStatuePosNum].m_MapNumber;
	BYTE btBloodCastleIndex = this->GetBridgeIndexByMapNum(btMapNumber); //season3 changed

	if ( nSaintStatuePosNum != -1 )
	{
		if ( BC_STATUE_RANGE(wMonsterType-132) != FALSE )
		{
			int result = gObjAddMonster(btMapNumber);

			if ( result >= 0 )
			{
				::gObjSetPosMonster(result, nSaintStatuePosNum);
				::gObjSetMonster(result, wMonsterType);
				gObj[result].Class = rand() % 3 + 132;
				gObj[result].m_cBloodCastleIndex = btBloodCastleIndex;
				gObj[result].m_ItemRate = this->m_iBC_NORMAL_ITEM_DROP;
				gObj[result].Dir = 1;
				gObj[result].m_PosNum = -1;
				gObj[result].Live = TRUE;
				gObj[result].DieRegen = 0;
				gObj[result].m_State = 1;
				gObj[result].MaxRegenTime = 0;
				gObj[result].MaxLife = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;
				gObj[result].Life = this->m_BridgeData[btBloodCastleIndex].m_iCastleStatueHealth;

				LogAddTD("[Blood Castle] (%d) Saint Status be created.", iBridgeIndex+1); //season 2.5 add-on
			}
		}
		else
		{
			LogAddTD("[Blood Castle][Bug Tracer] (%d) It's not Saint Statue( Map:%d, BloodCastleIndex:%d, Type:%d )", iBridgeIndex+1, btMapNumber, this->GetBridgeIndexByMapNum(btMapNumber), wMonsterType); //season3 changed
		}
	}

}

int CBloodCastle::LeaveUserBridge(int iBridgeIndex, int iBridgeSubIndex, int iUserIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	if ( BC_SUB_BRIDGE_RANGE(iBridgeSubIndex) == FALSE )
	{
		return -1;
	}

	int iRET_VAL = -1;

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	if ( this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex == iUserIndex )
	{
		iRET_VAL = iUserIndex;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iIndex = -1;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iEXP = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iScore = 0;
		this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 0;
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}

int CBloodCastle::EnterUserBridge(int iBridgeIndex, int iUserIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iRET_VAL = -1;

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iUserIndex )
			{
				iRET_VAL = i;
				break;
			}
		}

		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
		{
			iRET_VAL = i;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = iUserIndex;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP = 0;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iScore = 0;
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
			gObj[iUserIndex].m_bBloodCastleComplete = false;
			break;
		}
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	return iRET_VAL;
}

int CBloodCastle::LevelUp(int iIndex, int iAddExp, int iEventType)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	int iLEFT_EXP = 0;

	if(g_MasterLevelSystem.MasterLevelUp(&gObj[iIndex], iAddExp, true, 0) != 0)
	{
		return 0;
	}
	
	if(gObjPandaSprite(&gObj[iIndex]) == TRUE) //CashShop Panda Pet (Season 4.6 Add-on)
	{
		int ReWardEXP = iAddExp * 50 / 100;
		iAddExp += (__int64)ReWardEXP;
	}
	if( gObjUnicornSprite(&gObj[iIndex]) == TRUE ) // Season 5 Episode 2 JPN
	{
		int ReWardEXP = iAddExp * 30 / 100;
		iAddExp += (__int64)ReWardEXP;
	}

	::gObjSetExpPetItem(iIndex, iAddExp);

	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %u %d MonsterIndex : %d, EventType : %d", gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].X, gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level, gObj[iIndex].Experience, iAddExp, 0, iEventType);

	if ( gObj[iIndex].Level >= MAX_CHAR_LEVEL )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 112)), gObj[iIndex].m_Index, 1);
		return 0;
	}

	if ( (gObj[iIndex].Experience + iAddExp) < gObj[iIndex].NextExp )
	{
		gObj[iIndex].Experience += iAddExp;
	}
	else
	{
		iLEFT_EXP = gObj[iIndex].Experience + iAddExp - gObj[iIndex].NextExp;
		gObj[iIndex].Experience = gObj[iIndex].NextExp;
		gObj[iIndex].Level++;

		if ( gObj[iIndex].Class == CLASS_DARKLORD 
#ifdef MONK
			|| gObj[iIndex].Class == CLASS_MONK
#endif
			|| gObj[iIndex].Class == CLASS_MAGUMSA )
		{
			gObj[iIndex].LevelUpPoint += 7;
		}
		else
		{
			gObj[iIndex].LevelUpPoint += 5;
		}

		if( gObj[iIndex].PlusStatQuestClear && gObj[iIndex].Level >= g_ResetSystem.m_MarlonStatMinLevel )
		{
			gObj[iIndex].LevelUpPoint++;

			LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].LevelUpPoint);
		}

		gObj[iIndex].MaxLife += DCInfo.DefClass[gObj[iIndex].Class].LevelLife;
		gObj[iIndex].MaxMana += DCInfo.DefClass[gObj[iIndex].Class].LevelMana;
		gObj[iIndex].Life = gObj[iIndex].MaxLife;
		gObj[iIndex].Mana = gObj[iIndex].MaxMana;
		gObjNextExpCal(&gObj[iIndex]);
		gObjSetBP(gObj[iIndex].m_Index);

		GCLevelUpMsgSend(gObj[iIndex].m_Index, 1);

		gObjCalcMaxLifePower(gObj[iIndex].m_Index);
		LogAddTD(lMsg.Get(MSGGET(2, 8)), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Level);

		//----------------------------------------------------------------------------------------------

		if( gObj[iIndex].Level == 400 && gObj[iIndex].PartyNumber >= 0 ) //Season 2.5 add-on (Party Level 400 Display)
		{
			int iPartyNumber = gObj[iIndex].PartyNumber;
			char szMsg[256];
			sprintf(szMsg,"400 LevelUp (%s)(%s) Party ",gObj[iIndex].AccountID,gObj[iIndex].Name);
			int iPartyNumIndex;

			for( int i = 0; i<MAX_USER_IN_PARTY; i++ )
			{
				iPartyNumIndex = gParty.m_PartyS[iPartyNumber].Number[i];

				if( iPartyNumIndex >= 0  )
				{
					int iSize = strlen(szMsg);
					sprintf(&szMsg[iSize],",(%s)(%s) ",gObj[iPartyNumIndex].AccountID,gObj[iPartyNumIndex].Name);
				}
			}
			LogAddTD(szMsg);
		}
	}

	GJSetCharacterInfo(&gObj[iIndex], gObj[iIndex].m_Index, 0, 0);
	
	return iLEFT_EXP;
}

//Identical
void CBloodCastle::CheckUsersOnConnect(int iBridgeIndex) //0054DFA0
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	::EnterCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex) == FALSE )
			{
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
			}
			else
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber != this->GetMapNumByBCBridge(iBridgeIndex) ) //season3 changed
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = -1;
				}
			}
		}
	}

	::LeaveCriticalSection(&this->m_BridgeData[iBridgeIndex].m_critUserData);

}

bool CBloodCastle::AddExperience(int iIndex, int iEXP)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	if ( BC_BRIDGE_RANGE(gObj[iIndex].m_cBloodCastleIndex) == FALSE )
	{
		return false;
	}

	if ( BC_SUB_BRIDGE_RANGE(gObj[iIndex].m_cBloodCastleSubIndex) == FALSE )
	{
		return false;
	}

	if ( iEXP > 0 )
	{
		this->m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_UserData[gObj[iIndex].m_cBloodCastleSubIndex].m_iEXP += iEXP;
		gObj[iIndex].m_iBloodCastleEXP += iEXP;
	}

	return true;
}

//Identical
void CBloodCastle::BlockCastleEntrance(int iBridgeIndex) //0054E300
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;j++)
		{
			int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

			MapC[iMapNumber].m_attrbuf[j * 256 + i] |= 4; //season3 changed
		}
	}
}

//Identical
void CBloodCastle::ReleaseCastleEntrance(int iBridgeIndex) //0054E420
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;j++)
		{
			int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

			MapC[iMapNumber].m_attrbuf[j * 256 + i] &= ~4; //season3 changed
		}
	}
}

//Identical
void CBloodCastle::BlockCastleBridge(int iBridgeIndex) //0054E540
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndY;j++)
		{
			int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

			MapC[iMapNumber].m_attrbuf[j * 256 + i] |= 8; //season3 changed
		}
	}
}

//Identical
void CBloodCastle::ReleaseCastleBridge(int iBridgeIndex) //0054E660
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartX;i<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndX;i++)
	{
		for ( int j= ::g_btCastleBridgeMapXY[iBridgeIndex].btStartY;j<= ::g_btCastleBridgeMapXY[iBridgeIndex].btEndY;j++)
		{
			int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

			MapC[iMapNumber].m_attrbuf[j * 256 + i] &= ~8; //season3 changed

			LogAddTD("[Blood Castle] (%d) Castle Door Subsist", iBridgeIndex+1); //season 2.5 add-on

			LogAddTD("[Blood Castle][Bug Tracer] ReleaseCastleBridge-> %d", iBridgeIndex+1);
		}
	}
}

//Identical
void CBloodCastle::BlockCastleDoor(int iBridgeIndex) //0054E7C0
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<3;x++)
	{
		for ( int y=::g_btCastleDoorMapXY[iBridgeIndex][x].btStartX; y <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndX ;y++)
		{
			for ( int z = ::g_btCastleDoorMapXY[iBridgeIndex][x].btStartY; z <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndY ; z++)
			{
				int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

				MapC[iMapNumber].m_attrbuf[z * 256 + y] |= 4; //season3 changed
			}
		}
	}
}

//Identical
void CBloodCastle::ReleaseCastleDoor(int iBridgeIndex) //0054E930
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<3;x++)
	{
		for ( int y=::g_btCastleDoorMapXY[iBridgeIndex][x].btStartX; y <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndX ;y++)
		{
			for ( int z = ::g_btCastleDoorMapXY[iBridgeIndex][x].btStartY; z <= ::g_btCastleDoorMapXY[iBridgeIndex][x].btEndY ; z++)
			{
				int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

				MapC[iMapNumber].m_attrbuf[z * 256 + y] &= ~4; //season3 changed
			}
		}
	}
}

void CBloodCastle::BlockSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y)
{
	if ( BC_MAP_RANGE(iMAP_NUM) == FALSE )
	{
		return;
	}

	for ( int i=iSTART_X;i<=iEND_X;i++)
	{
		for ( int j=iSTART_Y;j<=iEND_Y;j++ )
		{
			MapC[iMAP_NUM].m_attrbuf[j * 256 + i] |= 4;
		}
	}
}

void CBloodCastle::ReleaseSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y)
{
	if ( BC_MAP_RANGE(iMAP_NUM) == FALSE )
	{
		return;
	}

	for ( int i=iSTART_X;i<=iEND_X;i++)
	{
		for ( int j=iSTART_Y;j<=iEND_Y;j++ )
		{
			MapC[iMAP_NUM].m_attrbuf[j * 256 + i] &= ~4;
		}
	}
}

//Identical
void CBloodCastle::SendCastleEntranceBlockInfo(int iBridgeIndex, bool bLive) //0054EC80
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 4;
	(bLive)?(lpMsg->btMapSetType=0) :( lpMsg->btMapSetType=1);

	lpMsgBody[0].btX = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;
	lpMsgBody[0].btY = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;

	for (int i=OBJ_MAXMONSTER;i<OBJMAX;i++)
	{
		int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

		if ( gObj[i].MapNumber == iMapNumber ) //season3 changed
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				DataSend(i,(LPBYTE)lpMsg, lpMsg->h.size);
			}
		}
	}

}

//Identical
void CBloodCastle::SendCastleBridgeBlockInfo(int iBridgeIndex, bool bLive) //0054EE90
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 8;
	(bLive)?lpMsg->btMapSetType=0:lpMsg->btMapSetType=1;

	lpMsgBody[0].btX = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartX;
	lpMsgBody[0].btY = ::g_btCastleEntranceMapXY[iBridgeIndex].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleEntranceMapXY[iBridgeIndex].btEndY;

	for ( int i=OBJ_MAXMONSTER;i<OBJMAX;i++)
	{
		int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

		if ( gObj[i].MapNumber == iMapNumber ) //season3 changed
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				DataSend(i, (LPBYTE)lpMsg, lpMsg->h.size);

				LogAddTD("[Blood Castle][Bug Tracer] SendCastleBridgeBlockInfo-> Bridge:%d/Live:%d, (%d,%d,%d,%d)", iBridgeIndex+1, bLive, lpMsgBody[0].btX, lpMsgBody[0].btY, lpMsgBody[1].btX, lpMsgBody[1].btY);
			}
		}
	}
}

//Identical
void CBloodCastle::SendCastleDoorBlockInfo(int iBridgeIndex, bool bLive) //0054F100
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	char cTEMP_BUF[256];
	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)cTEMP_BUF;

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*6);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)&cTEMP_BUF[7];
	lpMsg->btType = 0;
	lpMsg->btCount = 3;
	lpMsg->btMapAttr = 4;
	(bLive)?lpMsg->btMapSetType=0:lpMsg->btMapSetType=1;

	lpMsgBody[0].btX = ::g_btCastleDoorMapXY[iBridgeIndex][0].btStartX;
	lpMsgBody[0].btY = ::g_btCastleDoorMapXY[iBridgeIndex][0].btStartY;
	lpMsgBody[1].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][0].btEndX;
	lpMsgBody[1].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][0].btEndY;

	lpMsgBody[2].btX = ::g_btCastleDoorMapXY[iBridgeIndex][1].btStartX;
	lpMsgBody[2].btY = ::g_btCastleDoorMapXY[iBridgeIndex][1].btStartY;
	lpMsgBody[3].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][1].btEndX;
	lpMsgBody[3].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][1].btEndY;

	lpMsgBody[4].btX = ::g_btCastleDoorMapXY[iBridgeIndex][2].btStartX;
	lpMsgBody[4].btY = ::g_btCastleDoorMapXY[iBridgeIndex][2].btStartY;
	lpMsgBody[5].btX   = ::g_btCastleDoorMapXY[iBridgeIndex][2].btEndX;
	lpMsgBody[5].btY   = ::g_btCastleDoorMapXY[iBridgeIndex][2].btEndY;

	for ( int i=OBJ_MAXMONSTER;i<OBJMAX;i++)
	{
		int iMapNumber = this->GetMapNumByBCBridge(iBridgeIndex); //season3 add-on

		if ( gObj[i].MapNumber == iMapNumber ) //season3 changed
		{
			if ( gObj[i].Connected > PLAYER_LOGGED )
			{
				DataSend(i, (LPBYTE)lpMsg, lpMsg->h.size);
			}
		}
	}
}

void CBloodCastle::SendNoticeMessage(int iBridgeIndex, char * lpszMSG)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_NOTICE pNotice;

	TNotice::MakeNoticeMsg( &pNotice, 0, lpszMSG);

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);
					}
				}
			}
		}
	}
}

void CBloodCastle::SendNoticeScore(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_NOTICE pNotice;
	pNotice.type = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get(MSGGET(4, 140)), iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP);
						DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pNotice, pNotice.h.size);
					}
				}
			}
		}
	}
}

void CBloodCastle::SendNoticeState(int iBridgeIndex, int iPlayState)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	PMSG_STATEBLOODCASTLE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
	pMsg.btPlayState = iPlayState;
	pMsg.wRemainSec = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;

	if ( iPlayState == 4 )
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT;
	}
	else
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT;
	}

	pMsg.wUserHaveWeapon = this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX;
	pMsg.btWeaponNum = this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER + 1;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
					}
				}
			}
		}
	}
}

bool CBloodCastle::CheckUserBridgeMember(int iBridgeIndex, int iIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return false;
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iIndex )
		{
			return true;
		}
	}

	return false;
}

int CBloodCastle::GetAliveUserTotalEXP(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return 0;
	}

	int iRET_EXP = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber) != FALSE )
				{
					iRET_EXP += this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iEXP;
				}
			}
		}
	}

	return iRET_EXP;
}

void CBloodCastle::SearchUserDeleteQuestItem(int iIndex)
{
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13, 19) )
			{
				int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
				{
					::gObjInventoryItemSet(iIndex, x, -1);
					::gObjInventoryDeleteItem(iIndex, x);
					::GCInventoryItemDeleteSend(iIndex, x, TRUE);

					LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Delete Angel King's Weapon (%d) [Serial:%u]", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL, gObj[iIndex].pInventory[x].m_Number);
				}
			}
		}
	}
}

//Identical
void CBloodCastle::SearchUserDropQuestItem(int iIndex) //0054FF00
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
			{
				int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

				if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
				{
					BYTE pMsg[6];
					pMsg[5] = x;
					pMsg[3] = gObj[iIndex].X;
					pMsg[4] = gObj[iIndex].Y;

					int iBC_INDEX = this->GetBridgeIndexByMapNum(gObj[iIndex].MapNumber); //season3 add-on

					this->DropItemDirectly(iBC_INDEX, gObj[iIndex].m_Index, ITEMGET(13, 19), x); //season3 add-on
					this->DropItemDirectly(gObj[iIndex].MapNumber - MAP_INDEX_BLOODCASTLE1,	gObj[iIndex].m_Index, ITEMGET(13, 19), x); //remove this

					if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) != FALSE )
					{
						int iBC_INDEX = this->GetBridgeIndexByMapNum(gObj[iIndex].MapNumber);

						if ( this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL != -1 )
						{
							if ( this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number )
							{
								this->m_BridgeData[iBC_INDEX].m_iBC_QUEST_ITEM_USER_INDEX = -1;
							}
						}

						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%u-%u]", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL, this->m_BridgeData[iBC_INDEX].m_nBC_QUESTITEM_SERIAL, gObj[iIndex].pInventory[x].m_Number);
					}
					else
					{
						LogAddTD("[Blood Castle] Force (Account:%s, Name:%s) to Drop Angel King's Weapon (%d) [Serial:%u]", gObj[iIndex].AccountID, gObj[iIndex].Name, iLEVEL, gObj[iIndex].pInventory[x].m_Number);
					}
				}
			}
		}
	}
}

void CBloodCastle::SetUserState(int iIndex, int iState)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	int iBridgeIndex = gObj[iIndex].m_cBloodCastleIndex; //2
	int iBridgeSubIndex = gObj[iIndex].m_cBloodCastleSubIndex; //3

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	if ( BC_SUB_BRIDGE_RANGE(iBridgeSubIndex) == FALSE )
	{
		return;
	}

	switch ( iState )
	{
		case 0:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 0;
			break;

		case 1:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 1;
			break;

		case 2:
			if ( gObj[iIndex].PartyNumber >= 0 )
			{
				for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++) //4
				{
					if ( i == iBridgeSubIndex )
					{
						continue;
					}

					if(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1) //season 4 add-on
					{
						continue;
					}

					if ( gObj[iIndex].PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber )
					{
						if ( BC_MAP_RANGE( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].MapNumber ) != FALSE )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 3;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
				}
			}

			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 2;
			this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS = iBridgeSubIndex;
			break;

		case 3:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 3;
			break;

		case 4:
			this->m_BridgeData[iBridgeIndex].m_UserData[iBridgeSubIndex].m_iUserState = 4;
			break;
	}
}

struct ST_BC_SCORE
{
	char CharName[MAX_ACCOUNT_LEN];
	int  iSCORE;
	int  iEXP;
	int  iZEN;
	

};

#pragma pack(1)
struct GCS_BC_GIVE_REWARD
{
	PHEADB PHeader;
	bool bWinner;
	BYTE btType;
	ST_BC_SCORE m_stBCCharScore[MAX_BLOOD_CASTLE_SUB_BRIDGE+1];
};
#pragma pack()

void CBloodCastle::GiveReward_Win(int iIndex, int iBridgeIndex)
{
	this->FixUsersPlayStateWin(iBridgeIndex);
	char szNOTIFY_MSG[256];
	int iREWARD_EXP = 0;
	int iREWARD_ZEN = 0;
	int iREWARD_SCR = 0;
	int iLEFT_TIME = this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
	int iALIVE_PARTYCOUNT = this->GetWinnerPartyCompleteCount(iBridgeIndex);
	int iADD_PARTYPOINT = this->GetWinnerPartyCompletePoint(iBridgeIndex);

	LogAddTD("[Blood Castle] (%d) Winner Party Point - Alive Party Member : %d, Alive Party Point : %d", iBridgeIndex+1, iALIVE_PARTYCOUNT, iADD_PARTYPOINT);

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}
	
	if ( this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED != false )
	{
		return;
	}

	if ( gObj[iIndex].Connected > PLAYER_LOGGED )
	{
		gObj[iIndex].Name[MAX_ACCOUNT_LEN] = 0;
		wsprintf(szNOTIFY_MSG, lMsg.Get(MSGGET(4, 141)), gObj[iIndex].Name);
	}
	else
	{
		wsprintf(szNOTIFY_MSG, lMsg.Get(MSGGET(4, 142)));
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1 )
	{
		int iTOTAL_EXP = this->GetAliveUserTotalEXP(iBridgeIndex);
		LogAddTD("[Blood Castle] (%d) Quest Complete - All Alive User's Total EXP:%d", iBridgeIndex+1, iTOTAL_EXP);

		for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
			{
				continue;
			}

			if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected < PLAYER_PLAYING )
			{
				continue;
			}

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||  gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
			{
				continue;
			}

			PMSG_NOTICE pNotice;
			TNotice::MakeNoticeMsg(&pNotice, 10, szNOTIFY_MSG);
			TNotice::SetNoticeProperty(&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
			TNotice::SendNoticeToUser(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (TNotice*)&pNotice);

			int iADD_EXP = 0;

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0;
			}
			else
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 )
				{
					if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak != false )
					{
						iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0 / 2;
					}
				}
			}

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk4;
			}

			if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Win_Quest_Index)
			{
				iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk8;
			}

			switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState  )
			{
				case 0:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].SpecialCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk0;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, 0, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP, iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 1:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].SpecialCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk4;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, 0);
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, 1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP, iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 2:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].NormalCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk8;
					iREWARD_SCR += iADD_PARTYPOINT;

					if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber ) != FALSE )
					{
						this->DropChaosGem(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex);
					}

					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, 2, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP, iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 3:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].NormalCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unkC;
					iREWARD_SCR += iADD_PARTYPOINT;

					if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber ) != FALSE )
					{
						this->DropChaosGem(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex);
					}

					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, 3, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP, iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;

				case 4:
					iADD_EXP += this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000 * ::g_iBC_Add_Exp[iBridgeIndex].unkC;
					iREWARD_EXP = this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
					iREWARD_ZEN = this->CalcSendRewardZEN(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iQuestWinExpendZEN[iBridgeIndex].NormalCharacter);
					iREWARD_SCR = ::g_iBC_EventScore[iBridgeIndex].unk10;
					this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iREWARD_SCR, iLEFT_TIME, iALIVE_PARTYCOUNT);
					LogAddTD("[Blood Castle] (%d) Quest Complete Rewarded (Account:%s, Name:%s [State:%d], Exp:%d, ExtExp:%d, Zen:%d, Score:%d, LeftTime:%d, MapNumber:%d)", iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, 4, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_iBloodCastleEXP, iREWARD_EXP, iREWARD_ZEN, iREWARD_SCR, iLEFT_TIME, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
					break;
			}

			GCS_BC_GIVE_REWARD pMsg;

			pMsg.bWinner = true;
			pMsg.btType = -1;
			memcpy(pMsg.m_stBCCharScore[0].CharName , gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, MAX_ACCOUNT_LEN);
			pMsg.m_stBCCharScore[0].iEXP = iREWARD_EXP;
			pMsg.m_stBCCharScore[0].iZEN = iREWARD_ZEN;
			pMsg.m_stBCCharScore[0].iSCORE = iREWARD_SCR;
			PHeadSetB((LPBYTE)&pMsg.PHeader, 0x93, sizeof(GCS_BC_GIVE_REWARD) - (sizeof(ST_BC_SCORE) * (MAX_BLOOD_CASTLE_SUB_BRIDGE -1)) );

			DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.PHeader.uSize);
#ifdef WZQUEST
			LPOBJ lpObj = &gObj[m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];
            g_QuestExpProgMng.ChkUserQuestTypeEventMap(256, lpObj, lpObj->m_cBloodCastleIndex, 0);
#endif
		}

		this->m_BridgeData[iBridgeIndex].m_bBC_REWARDED = true;
	}
}

void CBloodCastle::GiveReward_Fail(int iBridgeIndex)
{
	this->FixUsersPlayStateFail(iBridgeIndex);

	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int iUserWhoGotUltimateWeapon = -1;
	iUserWhoGotUltimateWeapon = this->GetWhoGotUltimateWeapon(iBridgeIndex);

	if ( iUserWhoGotUltimateWeapon != -1 )
	{
		if ( OBJMAX_RANGE(iUserWhoGotUltimateWeapon) != FALSE )
		{
			LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) got Ultimate Weapon (%d) but Failed - Time Out", iBridgeIndex+1, gObj[iUserWhoGotUltimateWeapon].AccountID, gObj[iUserWhoGotUltimateWeapon].Name, this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER);
		}
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
		{
			continue;
		}

		if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected < PLAYER_PLAYING )
		{
			continue;
		}

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 )
		{
			continue;
		}	

		int iADD_EXP = 0;

		if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Door_Index)
		{
			iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0;
		}
		else
		{
			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 )
			{
				if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bLiveWhenDoorBreak != false )
				{
					iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk0 / 2;
				}
			}
		}

		if ( (gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].PartyNumber == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Party) || this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == this->m_BridgeData[iBridgeIndex].m_iExtraEXP_Kill_Statue_Index)
		{
			iADD_EXP += ::g_iBC_Add_Exp[iBridgeIndex].unk4;
		}

		this->CalcSendRewardEXP(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, iADD_EXP);
		this->SendRewardScore(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, ::g_iBC_EventScore_Fail[iBridgeIndex], 0, 0);

		LogAddTD("[Blood Castle] (%d) Quest Fail Rewarded (Account:%s, Name:%s, Score:%d, ExtEXP:%d, LeftTime:%d)", iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, ::g_iBC_EventScore_Fail[iBridgeIndex], iADD_EXP, 0);

		GCS_BC_GIVE_REWARD pMsg;

		pMsg.bWinner = false;
		pMsg.btType = -1;
		memcpy(pMsg.m_stBCCharScore[0].CharName, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, MAX_ACCOUNT_LEN);
		pMsg.m_stBCCharScore[0].iEXP = iADD_EXP;
		pMsg.m_stBCCharScore[0].iZEN = 0;
		pMsg.m_stBCCharScore[0].iSCORE = g_iBC_EventScore_Fail[iBridgeIndex];
		PHeadSetB((LPBYTE)&pMsg.PHeader, 0x93, sizeof(GCS_BC_GIVE_REWARD) - (sizeof(ST_BC_SCORE) * (MAX_BLOOD_CASTLE_SUB_BRIDGE -1)) );

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
		{
			DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.PHeader.uSize);			

			LogAddTD("[Blood Castle] (%d) Send User Quest Fail Message (Account:%s, Name:%s [State:%d], MapNumber:%d)", iBridgeIndex+1, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].AccountID, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Name, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState, gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber);
		}	
	}
}

//Identical
int CBloodCastle::CalcSendRewardEXP(int iIndex, int iEXP) //00552510
{
	if ( iEXP <= 0 )
	{
		return 0;
	}

	__int64 iRET_EXP = 0; //loc3
	__int64 iCAL_EXP = iEXP; //loc2
	
	if ( g_CrywolfSync.GetOccupationState() == 1 && g_iCrywolfApplyMvpPenalty )
	{
		iCAL_EXP = iCAL_EXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100; //season 2.5 changed
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE ) //loc6
	{
		return 0;
	}

	if ( gObj[iIndex].Connected < PLAYER_PLAYING )
	{
		return 0;
	}

	iRET_EXP = iCAL_EXP;

	if ( gObj[iIndex].Type == OBJ_USER )
	{
		while ( iCAL_EXP > 0 )
		{
			if ( iCAL_EXP > 0 )
			{
				CheckItemOptForGetExpEx(&gObj[iIndex], iCAL_EXP, TRUE); //Seal Exp

				iCAL_EXP = (int)(iCAL_EXP) * this->m_BridgeData[gObj[iIndex].m_cBloodCastleIndex].m_iBC_REWARD_EXP; //season 4.5 add-on
				iRET_EXP = (int)(iCAL_EXP); //season 4.5 changed + fix

				iCAL_EXP = this->LevelUp(iIndex, iCAL_EXP, 4);
			}
		}

		if(g_MasterLevelSystem.IsMasterLevelUser(&gObj[iIndex]) == FALSE) //season3 add-on
		{
			GCSendExp_INT64(iIndex, (WORD)-1, iRET_EXP, 0, 0);
		}
	}

	return iRET_EXP;
}

int CBloodCastle::CalcSendRewardZEN(int iIndex, int iZEN)
{
	if ( iZEN <= 0 )
	{
		return 0;
	}

	int iRET_ZEN = 0;

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return 0;
	}

	if ( gObj[iIndex].Connected < PLAYER_PLAYING )
	{
		return 0;
	}

	if ( gObjCheckMaxZen(iIndex, iZEN) == FALSE )
	{
		iRET_ZEN = MAX_ZEN - gObj[iIndex].Money;
		gObj[iIndex].Money += iRET_ZEN;

		return iRET_ZEN;
	}

	gObj[iIndex].Money += iZEN;
	iRET_ZEN = iZEN;
	GCMoneySend(iIndex, gObj[iIndex].Money);

	return iRET_ZEN;
}

//Identical
void CBloodCastle::DropChaosGem(int iIndex) //00552850
{
	if( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}
	// ----
#ifdef __CUSTOMS__
#if defined __REEDLAN__ || __BEREZNUK__
	g_ShopPointEx.AddEventBonus(iIndex, ShopPointExEvent::BC);
#endif
	// ----
	int iMapNumber = -1;
	// ----
	switch(gObj[iIndex].MapNumber)
	{
	case MAP_INDEX_BLOODCASTLE1:
		iMapNumber = 238;
		break;
	case MAP_INDEX_BLOODCASTLE2:
		iMapNumber = 239;
		break;
	case MAP_INDEX_BLOODCASTLE3:
		iMapNumber = 240;
		break;
	case MAP_INDEX_BLOODCASTLE4:
		iMapNumber = 241;
		break;
	case MAP_INDEX_BLOODCASTLE5:
		iMapNumber = 242;
		break;
	case MAP_INDEX_BLOODCASTLE6:
		iMapNumber = 243;
		break;
	case MAP_INDEX_BLOODCASTLE7:
		iMapNumber = 244;
		break;
	case MAP_INDEX_BLOODCASTLE8:
		iMapNumber = 245;
		break;
	}
	// ----
	BYTE CastleLevel = iMapNumber - 238;
	// ----
	for( int i = 0; i < this->m_ItemReward[CastleLevel].ItemCount; i++ )
	{
		ItemSerialCreateSend(gObj[iIndex].m_Index, iMapNumber, gObj[iIndex].X, gObj[iIndex].Y, this->m_ItemReward[CastleLevel].ItemID, this->m_ItemReward[CastleLevel].ItemLevel, 0, 0, 0, 0, iIndex, 0, 0);
	}
#else
	int iMaxHitUser = iIndex;
	int iType = g_iBloodCastle_Prize;

	int iMapNumber = -1;

	switch(gObj[iIndex].MapNumber)
	{
	case MAP_INDEX_BLOODCASTLE1:
		iMapNumber = 238;
		break;
	case MAP_INDEX_BLOODCASTLE2:
		iMapNumber = 239;
		break;
	case MAP_INDEX_BLOODCASTLE3:
		iMapNumber = 240;
		break;
	case MAP_INDEX_BLOODCASTLE4:
		iMapNumber = 241;
		break;
	case MAP_INDEX_BLOODCASTLE5:
		iMapNumber = 242;
		break;
	case MAP_INDEX_BLOODCASTLE6:
		iMapNumber = 243;
		break;
	case MAP_INDEX_BLOODCASTLE7:
		iMapNumber = 244;
		break;
	case MAP_INDEX_BLOODCASTLE8:
		iMapNumber = 245;
		break;
	}

	ItemSerialCreateSend(gObj[iIndex].m_Index, iMapNumber, gObj[iIndex].X, gObj[iIndex].Y, iType, 0, 0, 0, 0, 0, iMaxHitUser, 0, 0);
#endif
}

struct PMSG_ANS_BLOODCASTLESCORE_5TH
{
	PBMSG_HEAD h;	// C1:0D
	char AccountID[10];	// 3
	char GameID[10];	// D
	int ServerCode;	// 18
	int Score;	// 1C
	int Class;	// 20
	int BridgeNum;	// 24
	int iLeftTime;	// 28
	int iAlivePartyCount;	// 2C
};

void CBloodCastle::SendRewardScore(int iIndex, int iSCORE, int iLeftTime, int iAlivePartyCount)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	PMSG_ANS_BLOODCASTLESCORE_5TH pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x0D;
	pMsg.h.size = sizeof(pMsg);
	pMsg.Score = iSCORE;
	pMsg.BridgeNum = gObj[iIndex].m_cBloodCastleIndex;
	pMsg.Class = gObj[iIndex].Class;
	pMsg.ServerCode = gGameServerCode;
	pMsg.iLeftTime = iLeftTime;
	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GameID, gObj[iIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.iAlivePartyCount = iAlivePartyCount;

	if( ::IsDevilSquareEventConnected == FALSE && ::DevilSquareEventConnect == FALSE )
	{
		wsRServerCli.Close();
		wsRServerCli.CreateSocket(ghWnd);

		if ( GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) == FALSE )
		{
			::IsDevilSquareEventConnected = FALSE;
			LogAddTD("Can not connect Ranking Server");

			return;
		}

		::IsDevilSquareEventConnected = TRUE;
	}

	if ( ::DevilSquareEventConnect == FALSE && ::IsDevilSquareEventConnected != FALSE )
	{
		wsRServerCli.DataSend((char *)&pMsg, pMsg.h.size);
	}
}

void CBloodCastle::SendBridgeAnyMsg(LPBYTE lpMsg, int iSize, int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( gObj[ this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex ].Connected == PLAYER_PLAYING )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 && gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
				{
					DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, lpMsg, iSize);
				}
			}
		}
	}

}

void CBloodCastle::SendAllUserAnyMsg(LPBYTE lpMsg, int iSize)
{
	for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING )
		{
			if ( gObj[i].Type == OBJ_USER )
			{
				DataSend(i, lpMsg, iSize);
			}
		}
	}
}

void CBloodCastle::SetMonsterKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return;
	}

	int iTOT_USER_COUNT = 0;
	int iLIVE_USER_COUNT = 0;
	int iKILL_USER_COUNT = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			iTOT_USER_COUNT++;

			if ( BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber) != FALSE )
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
				{
					iLIVE_USER_COUNT++;
				}
				else
				{
					iKILL_USER_COUNT++;
				}
			}
			else
			{
				iKILL_USER_COUNT++;
			}

		}
	}

	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT = iLIVE_USER_COUNT * 40;
	this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT = 0;

	LogAddTD("[Blood Castle] (%d) Monster Kill Count Set - Monster:%d, Current Monster:%d, USER TOT:%d, LIVE:%d, DEAD:%d", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT, iTOT_USER_COUNT, iLIVE_USER_COUNT, iKILL_USER_COUNT);
}

bool CBloodCastle::CheckMonsterKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if( this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT >= this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT )
	{
		LogAddTD("[Blood Castle][Bug Tracer] (%d) All Monster Kill Success(Max Kill Count:%d / Kill Count:%d)", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT, this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT);
		return true;
	}

	return false;
}

bool CBloodCastle::CheckMonsterKillSuccess(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_MONSTER_KILL_COMPLETE;
}

bool CBloodCastle::CheckBossKillCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT >= this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT )
	{
		LogAddTD("[Blood Castle][Bug Tracer] (%d) All Boss Monster Kill Success(Max Boss Kill Count:%d / Boss Kill Count:%d)", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT, this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
		return true;
	}

	return false;
}

bool CBloodCastle::CheckBossKillSuccess(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	return this->m_BridgeData[iBridgeIndex].m_bBC_BOSS_MONSTER_KILL_COMPLETE;
}

//Identical
bool CBloodCastle::CheckEveryUserDie(int iBridgeIndex) //005532F0
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	bool bRET_VAL = true;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex != -1 )
		{
			if ( OBJMAX_RANGE(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex) == FALSE )
			{
				LogAddTD("error-L3 : [Blood Castle] (%d) User Out of Bound UserIndex:%d, SubIndex:%d", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				continue;
			}

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].MapNumber == this->GetMapNumByBCBridge(iBridgeIndex) ) //season3 changed
			{
				if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
				{
					bRET_VAL = false;
				}
				else
				{
					LogAddTD("error-L3 : [Blood Castle] (%d) Connection Closed UserIndex:%d, SubIndex:%d", iBridgeIndex+1, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, i);
				}
			}
			else
			{
				if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg == false )
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_bSendQuitMsg = true;

					PMSG_STATEBLOODCASTLE pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
					pMsg.btPlayState = 2;
					pMsg.wRemainSec = 0;
					pMsg.wMaxKillMonster = 0;
					pMsg.wCurKillMonster = 0;
					pMsg.wUserHaveWeapon = 0;
					pMsg.btWeaponNum = -1;

					DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}

	return bRET_VAL;
}

//Identical
bool CBloodCastle::CheckAngelKingExist(int iBridgeIndex) //005535E0
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return false;
	}

	bool bRET_VAL = false;

	if ( this->m_BridgeData[iBridgeIndex].m_iAngelKingPosNum == -1 )
	{
		LogAddTD("[Blood Castle] (%d) Angel King's Monster Position Doesn't Exist", iBridgeIndex+1);
		return false;
	}

	for ( int i=0;i<OBJ_STARTUSERINDEX;i++)
	{
		if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_NPC )
		{
			if ( gObj[i].Class == 232 )
			{
				if (gObj[i].MapNumber == this->GetMapNumByBCBridge(iBridgeIndex) ) //season3 changed
				{
					bRET_VAL = true;
					break;
				}
			}
		}
	}

	if ( bRET_VAL == false )
	{
		int result = gObjAddMonster(this->GetMapNumByBCBridge(iBridgeIndex)); //season3 changed

		if ( result >= 0 )
		{
			gObjSetPosMonster(result, this->m_BridgeData[iBridgeIndex].m_iAngelKingPosNum);
			gObjSetMonster(result, 232);
			gObj[result].m_cBloodCastleIndex = this->GetBridgeIndexByMapNum(gObj[result].MapNumber);
			bRET_VAL = true;
		}
		else
		{
			LogAddTD("[Blood Castle] (%d) Angel King - gObjAdd() failed (no space to add monster, result:%d)", iBridgeIndex+1, result);
			return false;
		}
	}

	return bRET_VAL;
}

int CBloodCastle::GetWhoGotUltimateWeapon(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iBridgeUserIndex = -1;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if ( OBJMAX_RANGE(iIndex) == FALSE )
		{
			continue;
		}

		if( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		{
			continue;
		}

		if ( BC_MAP_RANGE( gObj[iIndex].MapNumber ) == FALSE )
		{
			continue;
		}

		for ( int x=0;x<INVENTORY_SIZE;x++)
		{
			if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
			{
				if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(13,19) )
				{
					int iLEVEL = gObj[iIndex].pInventory[x].m_Level;

					if ( BC_WEAPON_LEVEL_RANGE(iLEVEL) != FALSE )
					{
						if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL != -1 )
						{
							if ( this->m_BridgeData[iBridgeIndex].m_nBC_QUESTITEM_SERIAL == gObj[iIndex].pInventory[x].m_Number )
							{
								iBridgeUserIndex = iIndex;
								break;
							}
						}
					}
				}
			}
		}

	}

	return iBridgeUserIndex;
}

int CBloodCastle::GetCurrentLiveUserCount(int iBridgeIndex)
{
	if ( BC_BRIDGE_RANGE(iBridgeIndex) == FALSE )
	{
		return -1;
	}

	int iRetLiveUserCount = 0;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;

		if ( OBJMAX_RANGE(iIndex) == FALSE )
		{
			continue;
		}

		if ( BC_MAP_RANGE( gObj[iIndex].MapNumber ) == FALSE )
		{
			continue;
		}

		if( gObj[iIndex].Type != OBJ_USER  )
		{
			continue;
		}

		if ( gObj[iIndex].Connected > PLAYER_LOGGED )
		{
			iRetLiveUserCount++;
		}
	}

	return iRetLiveUserCount;
}

//Identical
BOOL CBloodCastle::DropItemDirectly(int iBridgeIndex, int iIndex, int iItemType, int iItemPos) //0059CA30
{
	if ( BC_MAP_RANGE(this->GetMapNumByBCBridge(iBridgeIndex)) == FALSE )
	{
		LogAddTD("[Blood Castle] [%s][%s] DropItemDirectly() failed (iBridgeIndex wrong - %d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, iBridgeIndex);
		return FALSE;
	}

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = TRUE;
	pResult.Ipos = iItemPos;
	int map_num = gObj[iIndex].MapNumber;
	int type = gObj[iIndex].pInventory[iItemPos].m_Type;
	int level = gObj[iIndex].pInventory[iItemPos].m_Level;
	float dur = gObj[iIndex].pInventory[iItemPos].m_Durability;
	BOOL ret = gObj[iIndex].pInventory[iItemPos].IsItem();
	BYTE Option1 = gObj[iIndex].pInventory[iItemPos].m_Option1;
	BYTE Option2 = gObj[iIndex].pInventory[iItemPos].m_Option2;
	BYTE Option3 = gObj[iIndex].pInventory[iItemPos].m_Option3;
	BYTE NOption = gObj[iIndex].pInventory[iItemPos].m_NewOption;
	DWORD s_num = gObj[iIndex].pInventory[iItemPos].m_Number;
	BYTE ItemExOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&gObj[iIndex].pInventory[iItemPos]);
	BYTE ItemExLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&gObj[iIndex].pInventory[iItemPos]);

	BYTE NewOption[MAX_EXOPTION_SIZE];
	::ItemIsBufExOption(NewOption, &gObj[iIndex].pInventory[iItemPos]);
	int PetLevel = gObj[iIndex].pInventory[iItemPos].m_PetItem_Level;
	int PetExp = gObj[iIndex].pInventory[iItemPos].m_PetItem_Exp;
	BYTE SOption = gObj[iIndex].pInventory[iItemPos].m_SetOption;
	BYTE ItemEffectEx = gObj[iIndex].pInventory[iItemPos].m_ItemOptionEx;
	int item_number = gObj[iIndex].pInventory[iItemPos].m_Number;
	char szItemName[50] = "Item";
	int aAntiLootIndex = -1; //loc38

	//Season 4 add-on
	BYTE SocketOption[5]; //
	SocketOption[0] = 0xFF;
	SocketOption[1] = 0xFF;
	SocketOption[2] = 0xFF;
	SocketOption[3] = 0xFF;
	SocketOption[4] = 0xFF;

	BYTE SocketIndex = 0; //

	gItemSocketOption.GetSocketOption(&gObj[iIndex].pInventory[iItemPos],SocketOption,SocketIndex);

	if ( MapC[map_num].ItemDrop(type, level, dur, gObj[iIndex].X, gObj[iIndex].Y,Option1, Option2, Option3, NOption, SOption, item_number, aAntiLootIndex, PetLevel, PetExp, ItemEffectEx, SocketOption, SocketIndex) == TRUE )
	{
		::gObjInventoryDeleteItem(iIndex, iItemPos);
		pResult.Result = TRUE;
		LogAddTD(lMsg.Get(MSGGET(1, 223)), gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y, s_num, szItemName, type, level, Option1, Option2, Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], SOption, ItemEffectEx>>7, ItemExOption, ItemExLevel);
	}
	else
	{
		pResult.Result = FALSE;
	}

	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	if ( pResult.Result == TRUE )
	{
		if ( iItemPos < INVENTORY_BAG_START )
		{
			if ( iItemPos== 10 || iItemPos == 11 )
			{
				gObjUseSkill.SkillChangeUse(iIndex);
			}

			::gObjMakePreviewCharSet(iIndex);

			PMSG_USEREQUIPMENTCHANGED pMsg;

			PHeadSetB((LPBYTE)&pMsg, 0x25, sizeof(PMSG_USEREQUIPMENTCHANGED));
			pMsg.NumberH = SET_NUMBERH(iIndex);
			pMsg.NumberL = SET_NUMBERL(iIndex);
			ItemByteConvert(pMsg.ItemInfo, gObj[iIndex].pInventory[iItemPos]);
			pMsg.ItemInfo[1] = iItemPos<<4;
			pMsg.ItemInfo[1] |= LevelSmallConvert(iIndex, iItemPos) & 0x0F;

			DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			MsgSendV2(&gObj[iIndex], (LPBYTE)&pMsg, pMsg.h.size);
		}
	}

	return (pResult.Result);
}

bool CBloodCastle::CheckUserHaveUlimateWeapon(int iIndex)
{
	if ( OBJMAX_RANGE(iIndex ) == FALSE )
	{
		return false;
	}

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return false;
	}

	bool bRetVal = false;

	for ( int x=0;x<INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(0,19) || gObj[iIndex].pInventory[x].m_Type == ITEMGET(5,10) || gObj[iIndex].pInventory[x].m_Type == ITEMGET(4,18) )
			{
				bRetVal = true;
				break;
			}
		}
	}

	return bRetVal;
}

bool CBloodCastle::CheckWinnerExist(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
		return false;

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX != -1 )
		return true;

	return false;
}

bool CBloodCastle::CheckWinnerValid(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_LIMIT(iBridgeIndex, MAX_BLOODCASTLE_BRIDGE_COUNT)", iBridgeIndex+1);
		return false;
	}

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1 )
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX == -1", iBridgeIndex+1);
		return false;
	}

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) )
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !gObjIsConnected(m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX)", iBridgeIndex+1);
		return false;
	}

	if ( gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleSubIndex == -1 || gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex != iBridgeIndex )
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - (gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].m_cBloodCastleIndex == -1) ...", iBridgeIndex+1);
		return false;
	}

	if ( !BC_MAP_RANGE(gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber) )
	{
		LogAddTD("[Blood Castle] (%d) CBloodCastle::CheckWinnerValid() - !CHECK_BLOODCASTLE(gObj[m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].MapNumber)", iBridgeIndex+1);
		return false;
	}

	return true;
}

bool CBloodCastle::CheckUserWinnerParty(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex))
		return false;

	if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) == FALSE )
		return false;

	if ( gObjIsConnected(iIndex) == FALSE )
		return false;

	int iPartyIndex1 = gObj[iIndex].PartyNumber;
	int iPartyIndex2 = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;

	if ( OBJMAX_RANGE(iPartyIndex1) != FALSE && iPartyIndex1 == iPartyIndex2 )
		return true;

	return false;
}

bool CBloodCastle::CheckPartyExist(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return false;

	int iPartyIndex = gObj[iIndex].PartyNumber;
	int iUserIndex;

	if ( !OBJMAX_RANGE(iPartyIndex))
		return false;

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( BC_MAP_RANGE(gObj[iUserIndex].MapNumber) && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == false )
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool CBloodCastle::CheckWinnerPartyComplete(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX))
		return false;

	int iPartyIndex = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	int iUserIndex;

	if ( !OBJMAX_RANGE(iPartyIndex))
		return true;

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( BC_MAP_RANGE(gObj[iUserIndex].MapNumber) && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == false )
					{
						return false;
					}
				}
			}
		}
	}

	return true;
}

bool CBloodCastle::SetBridgeWinner(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX) != 0 )
		return false;

	if ( !gObjIsConnected(iIndex))
		return false;

	if ( !BC_MAP_RANGE(gObj[iIndex].MapNumber))
		return false;

	this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX = iIndex;

	return true;
}

int CBloodCastle::GetWinnerPartyCompleteCount(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return false;

	if ( !gObjIsConnected(this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX))
		return false;

	int iPartyIndex = gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber;
	
	if ( !OBJMAX_RANGE(iPartyIndex))
		return false;

	int iPartyComplete=0;
	int iUserIndex;

	for ( int iPartyUserIndex =0;iPartyUserIndex<MAX_USER_IN_PARTY;iPartyUserIndex++)
	{
		iUserIndex = gParty.m_PartyS[iPartyIndex].Number[iPartyUserIndex];

		if ( gObjIsConnected(iUserIndex))
		{
			if ( BC_MAP_RANGE(gObj[iUserIndex].MapNumber) && BC_BRIDGE_RANGE(gObj[iUserIndex].m_cBloodCastleIndex) )
			{
				if ( gObj[iUserIndex].Live == 1 )
				{
					if ( gObj[iUserIndex].m_bBloodCastleComplete == true )
					{
						iPartyComplete++;
					}
				}
			}
		}
	}

	return iPartyComplete;
}

int CBloodCastle::GetWinnerPartyCompletePoint(int iBridgeIndex)
{
	int iPartyComplete = this->GetWinnerPartyCompleteCount(iBridgeIndex);
	iPartyComplete--;

	if ( CHECK_LIMIT(iPartyComplete, MAX_USER_IN_PARTY) )
		return g_iBC_Party_EventPoint[iPartyComplete];

	return 0;
}

void CBloodCastle::ChangeMonsterState(int iBridgeIndex, int iIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	int iAddDamageMax = 0;
	int iAddDamageMin = 0;
	int iAddDefense = 0;

	if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 300000 )
	{
		iAddDamageMin = g_iBC_MONSTER_CHANGE_STATE[1][0];
		iAddDamageMax = g_iBC_MONSTER_CHANGE_STATE[1][1];
		iAddDefense = g_iBC_MONSTER_CHANGE_STATE[1][2];
	}
	else if ( this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC < 600000 )
	{
		iAddDamageMin = g_iBC_MONSTER_CHANGE_STATE[0][0];
		iAddDamageMax = g_iBC_MONSTER_CHANGE_STATE[0][1];
		iAddDefense = g_iBC_MONSTER_CHANGE_STATE[0][2];
	}

	LPMONSTER_ATTRIBUTE lpMA = gMAttr.GetAttr(gObj[iIndex].Class);

	if ( lpMA == NULL )
		return;

	gObj[iIndex].m_AttackDamageMin = lpMA->m_DamageMin + iAddDamageMin;
	gObj[iIndex].m_AttackDamageMax = lpMA->m_DamageMax + iAddDamageMax;
	gObj[iIndex].m_Defense = lpMA->m_Defense + iAddDefense;
}

void CBloodCastle::FixUsersPlayStateWin(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	if ( this->m_BridgeData[iBridgeIndex].m_iMISSION_SUCCESS != -1 )
	{
		for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
		{
			if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
				continue;

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < PLAYER_PLAYING )
				continue;

			if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 ||
				 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||
				 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
				continue;

			LPOBJ lpObj = &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];

			switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState )
			{
				case 0:
					if ( OBJMAX_RANGE(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						if ( BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Live == TRUE && lpObj->Life > 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 3;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
					else 
					{
						if ( !BC_MAP_RANGE(lpObj->MapNumber) || lpObj->Live == 0 || lpObj->Life <= 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
						}
					}
					break;
				case 1:
					if ( OBJMAX_RANGE(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
					}
					break;
				case 3:
					if ( OBJMAX_RANGE(lpObj->PartyNumber) && lpObj->PartyNumber == gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						if ( !BC_MAP_RANGE(lpObj->MapNumber) || lpObj->Live == 0 || lpObj->Life <= 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 4;
						}
					}
					else 
					{
						if ( BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Live == TRUE && lpObj->Life > 0.0 )
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
						}
						else
						{
							this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
						}
					}
					break;
				case 4:
					if ( !OBJMAX_RANGE(lpObj->PartyNumber) || lpObj->PartyNumber != gObj[this->m_BridgeData[iBridgeIndex].m_iBC_COMPLETE_USER_INDEX].PartyNumber )
					{
						this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
					}
					break;
			}

			LogAddTD("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateWin() - State : %d", iBridgeIndex+1, lpObj->AccountID, lpObj->Name, this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState);
		}
	}
}

void CBloodCastle::FixUsersPlayStateFail(int iBridgeIndex)
{
	if ( !BC_BRIDGE_RANGE(iBridgeIndex) )
		return;

	for ( int i=0;i<MAX_BLOOD_CASTLE_SUB_BRIDGE;i++)
	{
		if ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == -1 )
			continue;

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected < PLAYER_PLAYING )
			continue;

		if ( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex == -1 ||
			 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex == -1 ||
			 gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != iBridgeIndex )
			continue;

		LPOBJ lpObj = &gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex];

		switch ( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState )
		{
			case 3:
				if ( !BC_MAP_RANGE(lpObj->MapNumber) || lpObj->Live == 0 || lpObj->Life <= 0.0 )
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
				}
				else
				{
					this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 0;
				}
				break;
			case 4:
				this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState = 1;
				break;
		}

		LogAddTD("[Blood Castle] (%d) [%s][%s] FixUsersPlayStateFail() - State : %d",	iBridgeIndex+1, lpObj->AccountID, lpObj->Name,	this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iUserState);
	}
}

//Identical
int CBloodCastle::GetMapNumByBCBridge(int iBridgeIndex) //00555F10
{
	int iMapNumber = 0;

	switch(iBridgeIndex)
	{
	case 0:
		iMapNumber = MAP_INDEX_BLOODCASTLE1;
		break;
	case 1:
		iMapNumber = MAP_INDEX_BLOODCASTLE2;
		break;
	case 2:
		iMapNumber = MAP_INDEX_BLOODCASTLE3;
		break;
	case 3:
		iMapNumber = MAP_INDEX_BLOODCASTLE4;
		break;
	case 4:
		iMapNumber = MAP_INDEX_BLOODCASTLE5;
		break;
	case 5:
		iMapNumber = MAP_INDEX_BLOODCASTLE6;
		break;
	case 6:
		iMapNumber = MAP_INDEX_BLOODCASTLE7;
		break;
	case 7:
		iMapNumber = MAP_INDEX_BLOODCASTLE8;
		break;
	}

	return iMapNumber;
}

//Identical
int CBloodCastle::GetBridgeIndexByMapNum(int iMAP_NUM) //00555FE0
{
	int iBridgeIndex = -1;

	switch(iMAP_NUM)
	{
	case MAP_INDEX_BLOODCASTLE1:
		iBridgeIndex = 0;
		break;
	case MAP_INDEX_BLOODCASTLE2:
		iBridgeIndex = 1;
		break;
	case MAP_INDEX_BLOODCASTLE3:
		iBridgeIndex = 2;
		break;
	case MAP_INDEX_BLOODCASTLE4:
		iBridgeIndex = 3;
		break;
	case MAP_INDEX_BLOODCASTLE5:
		iBridgeIndex = 4;
		break;
	case MAP_INDEX_BLOODCASTLE6:
		iBridgeIndex = 5;
		break;
	case MAP_INDEX_BLOODCASTLE7:
		iBridgeIndex = 6;
		break;
	case MAP_INDEX_BLOODCASTLE8:
		iBridgeIndex = 7;
		break;
	}

	return iBridgeIndex;
}

//Identical
int CBloodCastle::GetMapNumByBC_CHAOSGEM(int iMAP_NUM) //005560F0
{
	int iMapNumber = iMAP_NUM;

	switch(iMAP_NUM)
	{
	case 238:
		iMapNumber = MAP_INDEX_BLOODCASTLE1;
		break;
	case 239:
		iMapNumber = MAP_INDEX_BLOODCASTLE2;
		break;
	case 240:
		iMapNumber = MAP_INDEX_BLOODCASTLE3;
		break;
	case 241:
		iMapNumber = MAP_INDEX_BLOODCASTLE4;
		break;
	case 242:
		iMapNumber = MAP_INDEX_BLOODCASTLE5;
		break;
	case 243:
		iMapNumber = MAP_INDEX_BLOODCASTLE6;
		break;
	case 244:
		iMapNumber = MAP_INDEX_BLOODCASTLE7;
		break;
	case 245:
		iMapNumber = MAP_INDEX_BLOODCASTLE8;
		break;
	}

	return iMapNumber;
}

//Identical
int CBloodCastle::GetMapNumByBC_ULTIMATEWEAPON(int iMAP_NUM) //005561C0
{
	int iMapNumber = iMAP_NUM;

	switch(iMAP_NUM)
	{
	case 246:
		iMapNumber = MAP_INDEX_BLOODCASTLE1;
		break;
	case 247:
		iMapNumber = MAP_INDEX_BLOODCASTLE2;
		break;
	case 248:
		iMapNumber = MAP_INDEX_BLOODCASTLE3;
		break;
	case 249:
		iMapNumber = MAP_INDEX_BLOODCASTLE4;
		break;
	case 250:
		iMapNumber = MAP_INDEX_BLOODCASTLE5;
		break;
	case 251:
		iMapNumber = MAP_INDEX_BLOODCASTLE6;
		break;
	case 252:
		iMapNumber = MAP_INDEX_BLOODCASTLE7;
		break;
	case 253:
		iMapNumber = MAP_INDEX_BLOODCASTLE8;
		break;
	}

	return iMapNumber;
}
// -------------------------------------------------------------------------

//005fb900	-> 100%
int CBloodCastle::ChangeUserIndex(int iExUserIndex, int iCurrentUserIndex, int iBridgeIndex)	//OK
{
	if( !BC_BRIDGE_RANGE(iBridgeIndex) )
	{
		return false;
	}
	// ----
	EnterCriticalSection((LPCRITICAL_SECTION)&this->m_BridgeData[iBridgeIndex].m_critUserData);
	// ----
	for( int i = 0; i < MAX_BLOOD_CASTLE_SUB_BRIDGE; i++ )
	{
		if( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iExUserIndex )
		{
			this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex = iCurrentUserIndex;
			LeaveCriticalSection((LPCRITICAL_SECTION)&this->m_BridgeData[iBridgeIndex].m_critUserData);
			return true;
		}
	}
	// ----
	LeaveCriticalSection((LPCRITICAL_SECTION)&this->m_BridgeData[iBridgeIndex].m_critUserData);
	return false;
}
// -------------------------------------------------------------------------

//005fba20	-> 100%
void CBloodCastle::SendNoticeMessageToSpecificUser(int iBridgeIndex, int iUserIndex, int iPlayState)	//OK
{
	PMSG_STATEBLOODCASTLE pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x9B, sizeof(PMSG_STATEBLOODCASTLE));
	// ----
	pMsg.btPlayState	= iPlayState;
	pMsg.wRemainSec		= this->m_BridgeData[iBridgeIndex].m_iBC_REMAIN_MSEC / 1000;
	// ----
	if( iPlayState == 4 )
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT;
	}
	else
	{
		pMsg.wMaxKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_MAX_COUNT;
		pMsg.wCurKillMonster = this->m_BridgeData[iBridgeIndex].m_iBC_MONSTER_KILL_COUNT;
	}
	// ----
	pMsg.wUserHaveWeapon	= this->m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX;
	pMsg.btWeaponNum		= this->m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER + 1;
	// ----
	for( int i = 0; i < MAX_BLOOD_CASTLE_SUB_BRIDGE; i++ )
	{
		if( this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex == iUserIndex )
		{
			if( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].Connected > PLAYER_LOGGED )
			{
				if( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleIndex != -1 )
				{
					if( gObj[this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex].m_cBloodCastleSubIndex != -1 )
					{
						DataSend(this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex, (LPBYTE)&pMsg, pMsg.h.size);
					}
				}
			}
		}
	}
}
// -------------------------------------------------------------------------

//005fbca0	-> 100%
int CBloodCastle::SetCastleBlockInfo(int iBridgeIndex, int iCastleBlockInfo)	//OK
{
	if( !BC_BRIDGE_RANGE(iBridgeIndex) )
	{
		return false;
	}
	// ----
	if( iCastleBlockInfo < BC_BLOCK_NONE || iCastleBlockInfo > BC_BLOCK_PLAYEND )
	{
		return false;
	}
	// ----
	this->m_BridgeData[iBridgeIndex].m_BlockInfo = iCastleBlockInfo;
	return true;
}

//005fbd40	-> 100%
int CBloodCastle::GetCastleBlockInfo(int iBridgeIndex)	//OK
{
	if( !BC_BRIDGE_RANGE(iBridgeIndex) )
	{
		return false;
	}
	// ----
	return m_BridgeData[iBridgeIndex].m_BlockInfo;
}
// -------------------------------------------------------------------------

//005fbda0	-> 100%
int CBloodCastle::GetPlayUserCountRightNow(int iBridgeIndex)	//OK
{
	if( !BC_BRIDGE_RANGE(iBridgeIndex) )
	{
		return -1;
	}
	// ----
	int nReallyPlayUserCount = 0;	//ebp-8
	// ----
	for( int i = 0; i < MAX_BLOOD_CASTLE_SUB_BRIDGE; i++ )	//ebp-0c
	{
		int iIndex = this->m_BridgeData[iBridgeIndex].m_UserData[i].m_iIndex;	//ebp-10
		// ----
		if( !OBJMAX_RANGE(iIndex) )
		{
			continue;
		}
		// ----
		if( BC_MAP_RANGE(gObj[iIndex].MapNumber) && gObj[iIndex].Type == OBJ_USER )
		{
			if( gObj[iIndex].Connected == PLAYER_PLAYING )
			{
				nReallyPlayUserCount++;
			}
		}
	}
	// ----
	return nReallyPlayUserCount;
}
// -------------------------------------------------------------------------