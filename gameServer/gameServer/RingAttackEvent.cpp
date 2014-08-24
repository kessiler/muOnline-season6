//GameServer 1.00.77 JPN - Completed
//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include "RingAttackEvent.h"
#include "gObjMonster.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "protocol.h"
#include "LogProc.h"
#include "Event.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "BuffEffectSlot.h"

CRingMonsterHerd::CRingMonsterHerd()
{
	return;
}

CRingMonsterHerd::~CRingMonsterHerd()
{
	return;
}

BOOL CRingMonsterHerd::Start()
{
	this->m_bEventSuccess = FALSE;
	return this->MonsterHerd::Start();
}


BOOL CRingMonsterHerd::MonsterHerdItemDrop(LPOBJ lpObj)
{
	if ( lpObj->Class == 135 )
	{
		int iIndex = gObjMonsterTopHitDamageUser(lpObj);
		int itemnumber = ItemGetNumberMake(14, 13);

		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, itemnumber, 0, 0, 0, 0, 0, iIndex, 0, 0);

		char szTemp[256];
		wsprintf(szTemp, lMsg.Get(MSGGET(4, 181)), gObj[iIndex].Name, gMapName[lpObj->MapNumber]);

		AllSendServerMsg( szTemp );

		LogAddTD("[Ring Event] White Wizard Killed by [%s][%s], MapNumber:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber);
		return TRUE;
	}
	
	if ( lpObj->Class == 136 || lpObj->Class == 137)
	{
		if ( (rand()%100) < 30 )
		{
			int iIndex = gObjMonsterTopHitDamageUser(lpObj);
			int itemnumber = ItemGetNumberMake(13, 20);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, itemnumber, 0, 30, 0, 0, 0, iIndex, 0, 0);
			return TRUE;
		}

		if ( (rand() % g_iRingOrcKillGiftRate) == 0 )
		{
			int iIndex = gObjMonsterTopHitDamageUser(lpObj);
			this->SendEventGiftWinner(iIndex, 1);
			return TRUE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(10000, (BYTE)lpObj->X, (BYTE)lpObj->Y);
		return TRUE;
	}

	return FALSE;

}


void CRingMonsterHerd::MonsterAttackAction(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(gObjCheckUsedBuffEffect(lpObj, AT_ICE_ARROW) == 1)
	{
		return;
	}

	if(gObjCheckUsedBuffEffect(lpObj, AT_STUN) == 1)
	{
		return;
	}

	if(gObjCheckUsedBuffEffect(lpObj, AT_SLEEP) == 1)
	{
		return;
	}

	if ( lpObj == NULL )
	{
		return;
	}

	if ( lpTargetObj == NULL )
	{
		return;
	}

	if ( lpObj->Connected < PLAYER_PLAYING || lpObj->Type != OBJ_MONSTER )
	{
		return;
	}

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
	{
		return;
	}

	if ( lpObj->Class == 135 )
	{
		this->OrderAttack(lpObj, lpTargetObj, 90 );
	}
}

struct PMSG_REQ_REG_RINGGIFT
{
	PBMSG_HEAD h;	// C1:10
	int iINDEX;	// 4
	char szUID[11];	// 8
	BYTE btGiftKind;	// 13
};

void CRingMonsterHerd::SendEventGiftWinner(int iIndex, int iGiftKind)
{
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if ( gObj[iIndex].UseEventServer != FALSE )
	{
		return;
	}

	gObj[iIndex].UseEventServer = TRUE;

	PMSG_REQ_REG_RINGGIFT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x10, sizeof(pMsg));
	pMsg.iINDEX  = iIndex;
	memcpy(pMsg.szUID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN);
	pMsg.szUID[MAX_ACCOUNT_LEN+1] = 0;
	pMsg.btGiftKind = iGiftKind;

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));

	LogAddTD("[Ring Event] [%s][%s] Request to Register Gift - Gift Kind (%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name,  iGiftKind);
}

static int g_RingEventMapNum[MAX_RINGMONSTER_MAP] = { MAP_INDEX_RORENCIA,MAP_INDEX_DEVIAS,MAP_INDEX_NORIA};

static int g_RingMapPosition[MAX_RINGMONSTER_MAP][4] =
{

	66, 44, 191, 197,
	111, 14, 232, 125,
	136, 53, 223, 181
};


static int g_RingMapDestPosition[MAX_RINGMONSTER_MAP][8] = {	133, 79, 87, 126,
														133, 178, 180, 126,
														164, 42, 221, 85,
														164, 42, 221, 85,
														160, 45, 160, 45, 
														152, 117, 209, 133 };


CRingAttackEvent::CRingAttackEvent()
{
	this->m_bDoEvent = FALSE;
	this->m_bHasData = FALSE;
	this->m_iRADIUS_MIN = 3;
	this->m_iRADIUS_MAX = 6;
	this->m_iMOVE_RAND_SIZE = 15;
	InitializeCriticalSection(&this->m_critMonsterAddData);
}


CRingAttackEvent::~CRingAttackEvent()
{
	DeleteCriticalSection(&this->m_critMonsterAddData);
}

void CRingAttackEvent::StartEvent()
{
	int iRandX;
	int iRandY;

	if ( this->m_bHasData == 0 )
	{
		return;
	}
	
	int loc4 = 0;//Season 4.5 addon

	loc4 = rand() % 3;//Season 4.5 addon

	for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
	{
		if(loc4 == i)//Season 4.5 addon
		{
			continue;
		}

		int iCount = 1000;

		while ( iCount-- != 0 )
		{
			iRandX = rand() % (g_RingMapPosition[i][2] - g_RingMapPosition[i][0]) + g_RingMapPosition[i][0];
			iRandY = rand() % (g_RingMapPosition[i][3] - g_RingMapPosition[i][1]) + g_RingMapPosition[i][1];

			if ( this->m_RingMonsterHerd[i].SetTotalInfo(g_RingEventMapNum[i] , 3, iRandX, iRandY ) != FALSE )
			{
				break;
			}
		}

		if ( iCount == 0 )
		{

		}
		else
		{
			if ( this->m_vtMonsterAddData.empty() != false )
			{
				LogAddTD("[Ring Event] - Error : No Monster Data Exist");
				continue;
			}

			LogAddTD("[Ring Event] - Monster Start Position MapNumber:%d, X:%d, Y:%d", g_RingEventMapNum[i], iRandX, iRandY);
		}

		EnterCriticalSection(&this->m_critMonsterAddData);

		for ( std::vector<RINGMONSTER_DATA>::iterator it = this->m_vtMonsterAddData.begin() ; it != this->m_vtMonsterAddData.end() ; it++)
		{
			RINGMONSTER_DATA & pRMD = *it;
			iCount = 100;

			while ( iCount-- != 0 )
			{
				if ( this->m_RingMonsterHerd[i].AddMonster(pRMD.m_iType, pRMD.m_bDoRegen, pRMD.m_bDoAttackFirst) != FALSE )
				{
					break;
				}
			}
		}

		LeaveCriticalSection(&this->m_critMonsterAddData);
		this->m_RingMonsterHerd[i].Start();
	}
}

void CRingAttackEvent::StopEvent()
{
	for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
	{
		this->m_RingMonsterHerd[i].Stop();
	}

	this->m_bMonsterToDest = FALSE;
}

void CRingAttackEvent::EnableEvent(BOOL bDoEvent)
{
	this->m_bDoEvent = bDoEvent;

	if ( this->m_bDoEvent )
	{
		this->SetState(1);
	}
	else
	{
		this->SetState(0);
	}
}

BOOL CRingAttackEvent::Load(char * lpszFileName)
{
	SMDFile = fopen(lpszFileName, "r");	//ok

	if ( SMDFile == NULL )
	{
		MsgBox("[Ring Event] Info file Load Fail [%s]", lpszFileName);
		return FALSE;
	}

	EnterCriticalSection(&this->m_critMonsterAddData);
	this->m_vtMonsterAddData.erase( this->m_vtMonsterAddData.begin(), this->m_vtMonsterAddData.end() );
	LeaveCriticalSection(&this->m_critMonsterAddData);

	this->m_vtEventTime.erase( this->m_vtEventTime.begin(), this->m_vtEventTime.end() );

	this->m_bHasData = FALSE;

	int Token;
	int type = -1;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		type = TokenNumber;

		while ( true )
		{
			if ( type == 0 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				this->m_iTIME_MIN_OPEN = TokenNumber;

				Token = GetToken();
				this->m_iTIME_MIN_PLAY = TokenNumber;

				Token = GetToken();
				this->m_iTIME_MONSTER_TO_DEST = TokenNumber;
			}
			else if ( type == 1 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				this->m_iMOVE_RAND_SIZE = TokenNumber;

				Token = GetToken();
				this->m_iRADIUS_MIN = TokenNumber;

				Token = GetToken();
				this->m_iRADIUS_MAX = TokenNumber;
			}
			else if ( type == 2 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				RINGMONSTER_DATA pRMD;

				pRMD.m_iType = TokenNumber;

				Token = GetToken();
				pRMD.m_bDoRegen = TokenNumber;

				Token= GetToken();
				pRMD.m_bDoAttackFirst = TokenNumber;

				EnterCriticalSection(&this->m_critMonsterAddData);
				this->m_vtMonsterAddData.push_back(pRMD);
				LeaveCriticalSection(&this->m_critMonsterAddData);
			}
			else if ( type == 3 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				RINGMONSTER_EVENT_TIME pRET;

				pRET.m_iHour = TokenNumber;

				Token = GetToken();
				pRET.m_iMinute = TokenNumber;

				this->m_vtEventTime.push_back(pRET);
			}
		}
	}

	fclose(SMDFile);
	LogAdd("%s file load!", lpszFileName);
	this->m_bHasData = TRUE;

	return TRUE;
}


void CRingAttackEvent::Run()
{
	if ( this->m_bMenualStart )
		return;

	if ( this->m_bDoEvent )
	{
		switch ( this->m_iEVENT_STATE )
		{
			case 0:
				this->ProcState_None();
				break;
			case 1:
				this->ProcState_Closed();
				break;
			case 2:
				this->ProcState_Playing();
		}
	}
}	

void CRingAttackEvent::CheckSync()
{
	if ( this->m_vtEventTime.empty() != 0 )
	{
		LogAddC(2, "[Ring Event] No Schedule Time Data");
		this->SetState(0);

		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	BOOL bTIME_CHANGED = FALSE;

	for ( std::vector<RINGMONSTER_EVENT_TIME>::iterator it = this->m_vtEventTime.begin(); it != this->m_vtEventTime.end() ; it++ )
	{
		RINGMONSTER_EVENT_TIME & pRET = *it;

		if ( (sysTime.wHour * 60 + sysTime.wMinute) < (pRET.m_iHour * 60 + pRET.m_iMinute ))
		{
			if ( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_iHour * 60 + pRET.m_iMinute) )
			{
				bTIME_CHANGED = TRUE;
				iMIN_HOUR = pRET.m_iHour;
				iMIN_MINUTE = pRET.m_iMinute;
			}
		}
	}

	if ( bTIME_CHANGED == FALSE )
	{
		std::vector<RINGMONSTER_EVENT_TIME>::iterator it = this->m_vtEventTime.begin();
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;

		for ( ; it != this->m_vtEventTime.end() ; it++ )
		{
			RINGMONSTER_EVENT_TIME & pRET = *it;

			if ( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_iHour * 60 + pRET.m_iMinute) )
			{
				bTIME_CHANGED = 2;
				iMIN_HOUR = pRET.m_iHour;
				iMIN_MINUTE = pRET.m_iMinute;
			}
		}
	}

	switch ( bTIME_CHANGED )
	{
		case 1:
			this->m_iTIME_MSEC_REMAIN = ((iMIN_HOUR * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond)) * 1000;
			break;
		case 2:
			this->m_iTIME_MSEC_REMAIN = (((iMIN_HOUR + 24) * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond)) * 1000;
			break;
		default:
			LogAddC(2, "[Ring Event] No Schedule Time Data");
			this->SetState(0);
			return;
	}

	this->m_iTIME_TICK_COUNT = GetTickCount();

	LogAddTD("[Ring Event] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		this->m_iTIME_MSEC_REMAIN/60000, iMIN_HOUR, iMIN_MINUTE);

	this->m_iTIME_NOTIFY_COUNT = this->m_iTIME_MIN_OPEN;
}


void CRingAttackEvent::SetState(int iSTATE)
{
	if ( this->m_iEVENT_STATE < 0 || this->m_iEVENT_STATE > 2 )
		return;

	this->m_iEVENT_STATE = iSTATE;

	switch ( this->m_iEVENT_STATE )
	{
		case 0:
			this->SetState_None();
			break;
		case 1:
			this->SetState_Closed();
			break;
		case 2:
			this->SetState_Playing();
			break;
	}
}


void CRingAttackEvent::ProcState_None()
{
	return;
}


struct PMSG_ANS_CL_EFFECT
{
	PBMSG_HEAD h;	// C1:9E
	WORD wEffectNum;	// 4
};

void CRingAttackEvent::ProcState_Closed()
{
	int iTICK_MSEC = GetTickCount() - this->m_iTIME_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		this->m_iTIME_TICK_COUNT = GetTickCount();

		if ( this->m_bDoEvent )
		{
			if ( this->m_iTIME_MSEC_REMAIN <= (this->m_iTIME_MIN_OPEN * 60 * 1000 ) &&
				 this->m_iTIME_MSEC_REMAIN > 0 &&
				 (this->m_iTIME_MSEC_REMAIN / 60000 ) != this->m_iTIME_NOTIFY_COUNT )
			{
				this->m_iTIME_NOTIFY_COUNT = this->m_iTIME_MSEC_REMAIN / 60000;

				if ( (this->m_iTIME_NOTIFY_COUNT+1) == 3 )
				{
					char szTemp[256];
					wsprintf(szTemp, lMsg.Get(MSGGET(4, 183)));
					AllSendServerMsg(szTemp);

					PMSG_ANS_CL_EFFECT pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
					pMsg.wEffectNum = 0;

					DataSendAll((LPBYTE)&pMsg, pMsg.h.size);

					LogAddTD("[Ring Event] - Before 3 minutes - Advertise");
				}
			}
		}
	}

	if ( this->m_iTIME_MSEC_REMAIN <= 0 )
	{
		char szTemp[256];
		wsprintf(szTemp, lMsg.Get(MSGGET(4, 184)));
		AllSendServerMsg(szTemp);

		PMSG_ANS_CL_EFFECT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
		pMsg.wEffectNum = 1;

		DataSendAll((LPBYTE)&pMsg, pMsg.h.size);

		if ( this->m_bDoEvent )
		{
			this->SetState(2);
		}
		else
		{
			this->SetState(0);
		}

		LogAddTD("[Ring Event] - Event Started");
	}
}


void CRingAttackEvent::ProcState_Playing()
{
	int iTICK_MSEC = GetTickCount() - this->m_iTIME_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		this->m_iTIME_TICK_COUNT = GetTickCount();

		if ( (rand()%10) == 0 )
		{
			this->Move();
		}
	}

	static int s_iTIME_REMAIN = -1;

	if ( ((this->m_iTIME_MSEC_REMAIN / 1000 ) % 30 ) == 0 && ((this->m_iTIME_MSEC_REMAIN / 1000) / 30) != s_iTIME_REMAIN  )
	{
		s_iTIME_REMAIN = this->m_iTIME_MSEC_REMAIN / 1000 / 30;

		for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
		{
			BOOL bLive = FALSE;

			for ( int n=0;n<OBJ_STARTUSERINDEX;n++)
			{
				if ( gObj[n].Class == 135 )
				{
					if ( gObj[n].m_bIsInMonsterHerd )
					{
						if ( gObj[n].Live && gObj[n].MapNumber == g_RingEventMapNum[i] )
						{
							bLive = TRUE;
						}
					}
				}
			}

			LogAddTD("[Ring Event] - per 30'S Inform MapNumber:%d, X:%d, Y:%d, Boss Live:%d",
				g_RingEventMapNum[i], this->m_RingMonsterHerd[i].GetX(), this->m_RingMonsterHerd[i].GetY(), bLive);
		}
	}

	if ( this->m_iTIME_MSEC_REMAIN <= (this->m_iTIME_MONSTER_TO_DEST * 60 * 1000) && this->m_bMonsterToDest == FALSE )
	{
		
		for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
		{
			BYTE iRandX;
			BYTE iRandY;
			BYTE ucRAND;

			ucRAND = rand()%4;
			iRandX = g_RingMapDestPosition[i][ucRAND*2];
			iRandY = g_RingMapDestPosition[i][ucRAND*2+1];

			if ( this->m_RingMonsterHerd[i].MoveHerd(iRandX, iRandY) != FALSE )
			{
				LogAddTD("[Ring Event] - Monster Herd Move MapNumber:%d, X:%d, Y:%d",
					g_RingEventMapNum[i], iRandX, iRandY);
			}
		}

		this->m_bMonsterToDest = TRUE;
	}

	if ( this->m_iTIME_MSEC_REMAIN <= 0 )
	{
		char szTemp[256];
		wsprintf(szTemp, lMsg.Get(MSGGET(4, 185)));
		AllSendServerMsg(szTemp);

		PMSG_ANS_CL_EFFECT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
		pMsg.wEffectNum = 2;

		DataSendAll((LPBYTE)&pMsg, pMsg.h.size);

		if ( this->m_bDoEvent )
		{
			this->SetState(1);
		}
		else
		{
			this->SetState(0);
		}

		LogAddTD("[Ring Event] - Event Ended");
	}
}


void CRingAttackEvent::SetState_None()
{
	this->StopEvent();
}


void CRingAttackEvent::SetState_Closed()
{
	this->StopEvent();

	if ( this->m_bDoEvent )
	{
		this->CheckSync();
	}
	else
	{
		this->SetState(0);
	}
}


void CRingAttackEvent::SetState_Playing()
{
	this->m_iTIME_MSEC_REMAIN = this->m_iTIME_MIN_PLAY * 60 * 1000;
	this->StartEvent();
}


void CRingAttackEvent::Move()
{
	if ( this->m_bDoEvent )
	{
		if ( this->m_bMonsterToDest )
			return;

		BYTE iRandX;
		BYTE iRandY;
		BYTE iCX;
		BYTE iCY;

		for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
		{
			int iCount = 1000;

			if ( this->m_RingMonsterHerd[i].GetCurrentLocation(iCX, iCY) == FALSE )
				return;

			while ( iCount-- != 0 )
			{
				iRandX = (rand() % this->m_iMOVE_RAND_SIZE) * ( (rand() % 3 -1) * -1 ) + iCX;
				iRandY = (rand() % this->m_iMOVE_RAND_SIZE) * ( (rand() % 3 -1) * -1 ) + iCY;

				if ( iRandX < g_RingMapPosition[i][0] || iRandX > g_RingMapPosition[i][2] )
					continue;

				if ( iRandY < g_RingMapPosition[i][1] || iRandY > g_RingMapPosition[i][3] )
					continue;

				if ( this->m_RingMonsterHerd[i].CheckLocation(iRandX, iRandY) != FALSE )
					break;
			}

			if ( this->m_RingMonsterHerd[i].MoveHerd(iRandX, iRandY) != FALSE )
			{
				LogAddTD("[Ring Event] - Monster Herd Move MapNumber:%d, X:%d, Y:%d",
					g_RingEventMapNum[i], iRandX, iRandY);
			}

			int iDIVIDE = this->m_iRADIUS_MAX - this->m_iRADIUS_MIN;

			if ( iDIVIDE <= 0 )
				iDIVIDE = 1;

			int iRADIUS = iDIVIDE + this->m_iRADIUS_MIN;

			if ( iRADIUS <= 3 )
				iRADIUS = 3;

			this->m_RingMonsterHerd[i].SetRadius((rand() % (this->m_iRADIUS_MAX - this->m_iRADIUS_MIN)) + this->m_iRADIUS_MIN);
		}
	}
}


void CRingAttackEvent::Start_Menual()
{
	this->SetMenualStart(TRUE);
	this->StopEvent();

	LogAddTD("????[Event Management] [Start] RingAttack Event!");
	this->m_iTIME_MIN_PLAY = 30;

	char szTemp[256];

	wsprintf(szTemp, lMsg.Get(MSGGET(4, 184)));
	AllSendServerMsg(szTemp);

	PMSG_ANS_CL_EFFECT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
	pMsg.wEffectNum = 1;

	DataSendAll((LPBYTE)&pMsg, pMsg.h.size);
	this->SetState(2);
}



void CRingAttackEvent::End_Menual()
{
	this->SetMenualStart(0);
}