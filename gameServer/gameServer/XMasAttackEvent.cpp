//GameServer 1.00.90	-	Completed (Not all function order are correct but code is 100% identical)
#include "stdafx.h"
#include "XMasAttackEvent.h"
#include "gObjMonster.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "protocol.h"
#include "LogProc.h"
#include "Event.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "BuffEffectSlot.h"

short ErrorType = 360;

CXMasMonsterHerd::CXMasMonsterHerd()
{
	return;
}

CXMasMonsterHerd::~CXMasMonsterHerd()
{
	return;
}

BOOL CXMasMonsterHerd::Start() //identical
{
	return this->MonsterHerd::Start();
}

BOOL CXMasMonsterHerd::MonsterHerdItemDrop(LPOBJ lpObj) //004B3660 identical
{
	if ( lpObj->Class != 476 && lpObj->Class != 466 )
	{
		return FALSE;
	}

	if ( lpObj->Class == 476 )
	{
		int count = 0;

		int irandreward = rand()%3; //loc3

		switch(irandreward)
		{
		case 0:
			{
				int iAttack = 0; //4
				int iDefense = 0; //5

				if(lpObj->Level <= 180)
				{
					iAttack = lpObj->Level / 3 + 45;
					iDefense = lpObj->Level / 5 + 50;
				}
				else
				{
					iAttack = 105;
					iDefense = 86;
				}

				gObjAddBuffEffectWideArea(lpObj, 10, count, AT_XMASS_REWARD, 2, iAttack, 3, iDefense, 1800);
			}
			break;
		case 1:
			gObjAddBuffEffectWideArea(lpObj, 10, count, AT_XMASS_HEALTH, 4, 500, 0, 0, 1800);
			break;
		case 2:
			gObjAddBuffEffectWideArea(lpObj, 10, count, AT_XMASS_MANA, 5, 500, 0, 0, 1800);
			break;
		};

		LogAddTD("[XMasAttackEvent] White Wizard Killed, MapNumber:%d", lpObj->MapNumber);
		return TRUE;
	}
	
	if ( lpObj->Class == 466 )
	{
		if ( (rand()%100) < g_iXMasAttackEventDropRate )
		{
			int iIndex = gObjMonsterTopHitDamageUser(lpObj);
			int itemnumber = ItemGetNumberMake(13, 66);

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y, itemnumber, 0, 10, 0, 0, 0, iIndex, 0, 0);
			return TRUE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(g_iXMasAttackEventDropZen, (BYTE)lpObj->X, (BYTE)lpObj->Y);
		return TRUE;
	}

	return FALSE;

}

//static int g_XMasEventMapNum[3] = {MAP_INDEX_RORENCIA,MAP_INDEX_DEVIAS,MAP_INDEX_NORIA};
//static int g_XMasMapPosition[3][4] = {66, 44, 191, 197, 111, 14, 232, 125, 136, 53, 223, 181};

static int g_XMasMapDestPosition[3][8] = {	133, 79, 87, 126, 
											133, 178, 180, 126, 
											164, 42, 221, 85, 
											164, 42, 221, 85, 
											160, 75, 160, 75, 
											152, 117, 209, 133 };

CXMasAttackEvent::CXMasAttackEvent(): m_nEvent_State(0), m_iMOVE_RAND_SIZE(15), m_iRADIUS_MIN(3), m_iRADIUS_MAX(6), m_iTIME_MIN_OPEN(0), m_iTIME_MIN_PLAY(0), m_iTIME_MONSTER_TO_DEST(0), m_iTIME_NOTIFY_COUNT(0), m_iUnk90(0)
{
}


CXMasAttackEvent::~CXMasAttackEvent() //004B3AB0
{
	this->ClearMonsterHerd();
}

BOOL CXMasAttackEvent::Load(char * lpszFileName) //004B3B40 identical
{
	SMDFile = fopen(lpszFileName, "r");	//ok

	if ( SMDFile == NULL )
	{
		MsgBox("[XMasAttackEvent] Info file Load Fail [%s]", lpszFileName);
		return FALSE;
	}

	this->Clear();

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

				this->m_iTIME_MIN_OPEN = (int)TokenNumber * 60000; //80

				Token = GetToken();
				this->m_iTIME_MIN_PLAY = (int)TokenNumber * 60000; //84

				Token = GetToken();
				this->m_iTIME_MONSTER_TO_DEST = (int)TokenNumber * 60000; //88
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

				XMASMONSTER_DATA pXMD;

				pXMD.m_iType = TokenNumber;

				Token = GetToken();
				pXMD.m_bDoRegen = TokenNumber;

				Token= GetToken();
				pXMD.m_bDoAttackFirst = TokenNumber;

				EnterCriticalSection(&this->m_critMonsterAddData);
				this->m_vtMonsterAddData.push_back(pXMD);
				LeaveCriticalSection(&this->m_critMonsterAddData);
			}
			else if ( type == 3 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				XMASMONSTER_EVENT_TIME pXET;

				pXET.m_iHour = TokenNumber;

				Token = GetToken();
				pXET.m_iMinute = TokenNumber;

				this->m_vtEventTime.push_back(pXET);
			}
			else if ( type == 4 )
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				XMASMONSTER_MAP_INFO pXMP;

				pXMP.m_btMapNumber = TokenNumber;

				Token = GetToken();
				pXMP.m_iMinX = TokenNumber;

				Token = GetToken();
				pXMP.m_iMinY = TokenNumber;

				Token = GetToken();
				pXMP.m_iMaxX = TokenNumber;

				Token = GetToken();
				pXMP.m_iMaxY = TokenNumber;

				this->m_vtXMasMonsterMapInfo.push_back(pXMP);
			}
		}
	}

	fclose(SMDFile);
	LogAdd("[XMasAttackEvent] - %s file load!", lpszFileName);
	this->m_bHasData = TRUE;

	return TRUE;
}

void CXMasAttackEvent::StartEvent() //004B43E0 identical
{
	if ( this->m_bDoEvent == FALSE )
	{
		return;
	}

	if ( this->m_bHasData == FALSE )
	{
		return;
	}

	if( this->m_vtMonsterAddData.empty() != FALSE )
	{
		return;
	}

	this->ClearMonsterHerd();
	this->CreateMonsterHerd();

	for (int i = 0; i < this->m_vtXMasMonsterMapInfo.size(); i++)
	{
		XMASMONSTER_MAP_INFO MonsterMapInfo = this->m_vtXMasMonsterMapInfo[i]; //perfect
		
		CXMasMonsterHerd * vtHerdInfo = this->m_vtXMasMonsterHerd[i]; //perfect

		int iCount = 1000;
		int iRandX = 0;
		int iRandY = 0;

		while ( iCount-- != 0 )
		{
			iRandX = rand() % (MonsterMapInfo.m_iMaxX - MonsterMapInfo.m_iMinX) + MonsterMapInfo.m_iMinX; //perfect
			iRandY = rand() % (MonsterMapInfo.m_iMaxY - MonsterMapInfo.m_iMinX) + MonsterMapInfo.m_iMinY; //perfect + wz bug

			if ( vtHerdInfo->SetTotalInfo(MonsterMapInfo.m_btMapNumber, 3, iRandX, iRandY ) == TRUE )
			{
				break;
			}
		}

		if ( iCount == 0 )
		{

		}
		else
		{
			if ( this->m_vtMonsterAddData.empty() != FALSE )
			{
				LogAddTD("[XMasAttackEvent] - Error : No Monster Data Exist");
				continue;
			}
			LogAddTD("[XMasAttackEvent] - Monster Start Position MapNumber:%d, X:%d, Y:%d", MonsterMapInfo.m_btMapNumber, iRandX, iRandY);
		}
		this->AddMonsterToHerd(vtHerdInfo);
		vtHerdInfo->Start();
	}
}

void CXMasAttackEvent::StopEvent() //identical
{
	std::vector<CXMasMonsterHerd*>::iterator it = this->m_vtXMasMonsterHerd.begin();

	for (; it != this->m_vtXMasMonsterHerd.end(); ++it)
	{
		CXMasMonsterHerd * vtHerdInfo = *it; //perfect

		vtHerdInfo->Stop();
	}

	this->m_bMonsterToDest = FALSE;
}

void CXMasAttackEvent::EnableEvent(BOOL bDoEvent) //identical
{
	this->m_bDoEvent = bDoEvent;

	if ( this->m_bDoEvent )
	{
		this->SetState(ATTACKEVENT_STATE_CLOSED);
	}
	else
	{
		this->SetState(ATTACKEVENT_STATE_NONE);
	}
}

//identical
void CXMasAttackEvent::SetState(int nSate) //004B46A0 identical
{
	if ( !(nSate >= ATTACKEVENT_STATE_NONE || nSate <= ATTACKEVENT_STATE_PLAYING) )
	{
		LogAddTD("%s	%s	%s	%s	%d","(nSate >= ATTACKEVENT_STATE_NONE) || (nSate <= ATTACKEVENT_STATE_PLAYING)", "", "NULL", __FILE__, ErrorType+1);
		return;
	}

	if ( !(this->m_nEvent_State >= ATTACKEVENT_STATE_NONE || this->m_nEvent_State <= ATTACKEVENT_STATE_PLAYING) )
	{
		LogAddTD("%s	%s	%s	%s	%d","(m_nEventState >= ATTACKEVENT_STATE_NONE) || (m_nEventState <= ATTACKEVENT_STATE_PLAYING)", "", "NULL", __FILE__, ErrorType+2);
		return;
	}

	this->m_nEvent_State = nSate;

	switch ( this->m_nEvent_State )
	{
		case ATTACKEVENT_STATE_NONE:
			this->SetState_None();
			break;
		case ATTACKEVENT_STATE_CLOSED:
			this->SetState_Closed();
			break;
		case ATTACKEVENT_STATE_PLAYING:
			this->SetState_Playing();
			break;
	}
}

void CXMasAttackEvent::ProcState_None() //identical
{
	return;
}

void CXMasAttackEvent::ProcState_Closed() //identical
{
	if(this->m_bDoEvent == FALSE)
	{
		this->SetState(ATTACKEVENT_STATE_NONE);
		LogAddTD("%s	%s	%s	%s	%d","m_bDoEvent","","SetState(ATTACKEVENT_STATE_NONE)",__FILE__, ErrorType+1);
		return;
	}

	int iTICK_MSEC = this->m_EventTimer.GetMiliSeconds();

	if ( iTICK_MSEC >= 1000 )
	{
		if(iTICK_MSEC/60000 != this->m_iTIME_NOTIFY_COUNT)
		{
			this->m_iTIME_NOTIFY_COUNT = iTICK_MSEC/60000;

			if(this->m_iTIME_NOTIFY_COUNT + 1 == m_iTIME_MIN_OPEN/60000)
			{
				LogAddTD("[XMasAttackEvent] - Before %d minutes - Advertise",this->m_iTIME_NOTIFY_COUNT+1);
			}
		}
	}

	if ( iTICK_MSEC == 0 )
	{
		this->SendMessage(lMsg.Get(2583), 1);

		if ( this->m_bDoEvent != FALSE )
		{
			this->SetState(ATTACKEVENT_STATE_PLAYING);
		}
		else
		{
			this->SetState(ATTACKEVENT_STATE_NONE);
		}

		LogAddTD("[XMasAttackEvent] - Event Started");
	}
}

void CXMasAttackEvent::SendMessage(LPSTR lpMsg, int Effect) //identical
{
	char szTemp[256];

	wsprintf(szTemp, lpMsg);
	AllSendServerMsg(szTemp);

	PMSG_ANS_CL_EFFECT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
	pMsg.wEffectNum = Effect;

	DataSendAll((LPBYTE)&pMsg, pMsg.h.size);
}


void CXMasAttackEvent::ProcState_Playing() //004B49E0 identical
{
	int iTICK_MSEC = this->m_EventTimer.GetMiliSeconds();

	int loc3 = iTICK_MSEC / 1000;

	if ( iTICK_MSEC >= 1000 )
	{
		if ( (rand()%20) == 0 )
		{
			this->Move();
		}
	}

	static int s_iTIME_REMAIN = -1;

	if ( (loc3 / 30 ) != s_iTIME_REMAIN && (loc3 % 30) == 0  )
	{
		s_iTIME_REMAIN = loc3 / 30;

		for ( int i=0;i<this->m_vtXMasMonsterHerd.size();i++) //loc4
		{
			CXMasMonsterHerd * vtHerdInfo = this->m_vtXMasMonsterHerd[i]; //perfect loc5

			BOOL bLive = FALSE; //lo6

			for ( int n=0;n<OBJ_STARTUSERINDEX;n++) //loc7
			{
				if ( gObj[n].Class == 476 )
				{
					if ( gObj[n].m_bIsInMonsterHerd )
					{
						if ( gObj[n].Live && gObj[n].MapNumber == vtHerdInfo->GetMapNumber() )
						{
							bLive = TRUE;
							break;
						}
					}
				}
			}
			LogAddTD("[XMasAttackEvent] - per 30'S Inform MapNumber:%d, X:%d, Y:%d, Boss Live:%d", vtHerdInfo->GetMapNumber(), vtHerdInfo->GetX(), vtHerdInfo->GetY(), bLive);
			this->EventLog("군단목적지[%s] X:%d, Y:%d",vtHerdInfo->GetMapNumber(), vtHerdInfo->GetX(), vtHerdInfo->GetY());
		}
		LogAddTD("[XMasAttackEvent] - NotifySec: %d", s_iTIME_REMAIN);
	}

	if (this->m_bMonsterToDest == FALSE && iTICK_MSEC <= this->m_iTIME_MONSTER_TO_DEST)
	{		
		for ( int i=0;i<this->m_vtXMasMonsterHerd.size();i++) //loc8
		{
			CXMasMonsterHerd * vtHerdInfo = this->m_vtXMasMonsterHerd[i]; //perfect loc9

			BYTE ucRAND; //loc11
			BYTE iRandX; //loc12
			BYTE iRandY; //loc13			

			ucRAND = rand()%4;
			iRandX = g_XMasMapDestPosition[i][ucRAND*2];
			iRandY = g_XMasMapDestPosition[i][ucRAND*2+1];

			if ( vtHerdInfo->MoveHerd(iRandX, iRandY) != FALSE )
			{
				LogAddTD("[XMasAttackEvent] - Monster Herd Move MapNumber:%d, X:%d, Y:%d",
					vtHerdInfo->GetMapNumber(), iRandX, iRandY);
				this->EventLog("군단목적지[%s] X:%d, Y:%d",vtHerdInfo->GetMapNumber(), iRandX, iRandY);
			}
		}

		this->m_bMonsterToDest = TRUE;
	}

	if ( iTICK_MSEC == 0 )
	{
		if ( this->m_bDoEvent != FALSE )
		{
			this->SetState(ATTACKEVENT_STATE_CLOSED);
		}
		else
		{
			this->SetState(ATTACKEVENT_STATE_NONE);
		}

		LogAddTD("[XMasAttackEvent] - Event Ended");
	}
}

BYTE CXMasMonsterHerd::GetX() //identical
{
	return this->m_iCUR_X;
}

BYTE CXMasMonsterHerd::GetY() //identical
{
	return this->m_iCUR_Y;
}

int CXMasMonsterHerd::GetMapNumber() //identical
{
	return this->m_iMapNumber;
}

void CXMasAttackEvent::SetState_None() //identical
{
	this->StopEvent();
}

void CXMasAttackEvent::SetState_Closed() //identical
{
	this->StopEvent();

	if ( this->m_bDoEvent )
	{
		this->CheckSync();
	}
	else
	{
		this->SetState(ATTACKEVENT_STATE_NONE);
	}
}

void CXMasAttackEvent::SetState_Playing() //identical
{
	this->m_EventTimer.SetMiliSeconds(this->m_iTIME_MIN_PLAY);
	this->StartEvent();
}


void CXMasAttackEvent::Move() //identical
{
	if(this->m_bDoEvent == FALSE)
	{
		return;
	}

	if(this->m_bMonsterToDest != FALSE)
	{
		return;
	}

	for(int i = 0; i < this->m_vtXMasMonsterHerd.size(); i++)
	{
		XMASMONSTER_MAP_INFO MonsterMapInfo = this->m_vtXMasMonsterMapInfo[i]; //perfect
		
		CXMasMonsterHerd * vtHerdInfo = this->m_vtXMasMonsterHerd[i]; //perfect

		if(this->MonsterHerdMoveCheck(vtHerdInfo, &MonsterMapInfo) == FALSE)
		{
			break;
		}

		int iDIVIDE = this->m_iRADIUS_MAX - this->m_iRADIUS_MIN;

		if ( iDIVIDE <= 0 )
			iDIVIDE = 1;

		int iRADIUS = iDIVIDE + this->m_iRADIUS_MIN;

		if ( iRADIUS <= 3 )
			iRADIUS = 3;

		vtHerdInfo->SetRadius((rand() % (this->m_iRADIUS_MAX - this->m_iRADIUS_MIN)) + this->m_iRADIUS_MIN);
	}
}

void CXMasAttackEvent::EventLog(LPSTR Msg, int MapNumber, BYTE cX, BYTE cY) //empty
{
}

void CXMasAttackEvent::CreateMonsterHerd() //identical
{
	EnterCriticalSection(&this->m_critMonsterAddData);

	for ( std::vector<XMASMONSTER_MAP_INFO>::iterator it = this->m_vtXMasMonsterMapInfo.begin(); it != this->m_vtXMasMonsterMapInfo.end() ; it++ )
	{
		CXMasMonsterHerd & vtHerdInfo = *(new CXMasMonsterHerd);
		this->m_vtXMasMonsterHerd.push_back(&vtHerdInfo);
	}

	LeaveCriticalSection(&this->m_critMonsterAddData);		
}

void CXMasAttackEvent::AddMonsterToHerd(CXMasMonsterHerd * vtHerdInfo) //004B5210 identical
{
		EnterCriticalSection(&this->m_critMonsterAddData);

		for ( std::vector<XMASMONSTER_DATA>::iterator it = this->m_vtMonsterAddData.begin() ; it != this->m_vtMonsterAddData.end() ; it++)
		{
			XMASMONSTER_DATA & pXMD = * it;
			int iCount = 100;

			while ( iCount-- != 0 )
			{
				if ( vtHerdInfo->AddMonster(pXMD.m_iType, pXMD.m_bDoRegen, pXMD.m_bDoAttackFirst) != FALSE )
				{
					break;
				}
			}
		}

		LeaveCriticalSection(&this->m_critMonsterAddData);
}

void CXMasAttackEvent::ClearMonsterHerd() //004B52E0 identical
{
	EnterCriticalSection(&this->m_critMonsterAddData);

	std::vector<CXMasMonsterHerd*>::iterator it = this->m_vtXMasMonsterHerd.begin();

	while (it != this->m_vtXMasMonsterHerd.end())
	{
		CXMasMonsterHerd * pXMH = *it;
		this->m_vtXMasMonsterHerd.erase(it);
		delete pXMH;
	}

	LeaveCriticalSection(&this->m_critMonsterAddData);
}

BOOL CXMasAttackEvent::MonsterHerdMoveCheck(CXMasMonsterHerd * vtHerdInfo, XMASMONSTER_MAP_INFO * MonsterMapInfo) //004B53A0 identical
{
	int iCount = 1000; //2
	BYTE iRandX; //3
	BYTE iRandY; //4
	BYTE iCX; //5
	BYTE iCY; //6

	if(vtHerdInfo->GetCurrentLocation(iCX, iCY) == FALSE)
	{
		return FALSE;
	}

	while(iCount-- != 0)
	{
		iRandX = (rand() % this->m_iMOVE_RAND_SIZE) * ( (rand() % 3 -1) * -1 ) + iCX;
		iRandY = (rand() % this->m_iMOVE_RAND_SIZE) * ( (rand() % 3 -1) * -1 ) + iCY;

		if ( iRandX < MonsterMapInfo->m_iMinX || iRandX > MonsterMapInfo->m_iMaxX )
			continue;

		if ( iRandY < MonsterMapInfo->m_iMinY || iRandY > MonsterMapInfo->m_iMaxY )
			continue;

		if ( vtHerdInfo->CheckLocation(iRandX, iRandY) != FALSE )
		{
			break;
		}
	}

	vtHerdInfo->MoveHerd(iRandX, iRandY);
	return TRUE;
}


void CXMasAttackEvent::Run() //identical
{
	if ( this->m_bMenualStart )
		return;

	if ( this->m_bDoEvent == FALSE)
		return;

	switch ( this->m_nEvent_State )
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

void CXMasAttackEvent::CheckSync() //identical
{
	if ( this->m_vtEventTime.empty() != 0 )
	{
		LogAddC(2, "[XMasAttackEvent] No Schedule Time Data");
		this->SetState(ATTACKEVENT_STATE_NONE);
		return;
	}

	BOOL bTIME_CHANGED = FALSE;
	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	for ( std::vector<XMASMONSTER_EVENT_TIME>::iterator it = this->m_vtEventTime.begin(); it != this->m_vtEventTime.end() ; it++ )
	{
		XMASMONSTER_EVENT_TIME & pRET = *it;

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
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;

		std::vector<XMASMONSTER_EVENT_TIME>::iterator it = this->m_vtEventTime.begin();

		for ( ; it != this->m_vtEventTime.end() ; it++ )
		{
			XMASMONSTER_EVENT_TIME & pRET = *it;

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
			{
				int m_iTIME_MSEC_REMAIN = ((iMIN_HOUR * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond));
				this->m_EventTimer.SetSeconds(m_iTIME_MSEC_REMAIN);
			}
			break;
		case 2:
			{
				int m_iTIME_MSEC_REMAIN = (((iMIN_HOUR + 24) * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond));
				this->m_EventTimer.SetSeconds(m_iTIME_MSEC_REMAIN);
			}
			break;
		default:
			LogAddC(2, "[XMasAttackEvent] No Schedule Time Data");
			this->SetState(ATTACKEVENT_STATE_NONE);
			return;
	}

	LogAddTD("[XMasAttackEvent] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		this->m_EventTimer.GetMinutes(), iMIN_HOUR, iMIN_MINUTE);

	this->m_iTIME_NOTIFY_COUNT = this->m_iTIME_MIN_OPEN;
}

void CXMasAttackEvent::Start_Menual() //identical = virtual 0x18
{
	this->SetMenualStart(TRUE);
	this->StopEvent();

	LogAddTD("○●[Event Management] [Start] XMasAttackEvent!");
	this->m_iTIME_MIN_PLAY = 30;

	this->SendMessage(lMsg.Get(2583), 1);
	this->SetState(ATTACKEVENT_STATE_PLAYING);
}

void CXMasAttackEvent::End_Menual() //identical = virtual 0x10
{
	this->SetMenualStart(FALSE);
}