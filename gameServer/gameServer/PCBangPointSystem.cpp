// PCBangPointSystem.cpp: implementation of the CPCBangPointSystem class.
//	GS	1.00.56	JPN	0xXXXXXXXX	-	Completed (non- code file)
//	GS	1.00.90	JPN	0xXXXXXXXX	-	Completed 2:37 PM 6/6/2011
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCBangPointSystem.h"
#include "..\include\readscript.h"
#include "Gamemain.h"
#include "LogProc.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "IllusionTempleEvent.h"
#include "DevilSquare.h"
#include "..\common\winutil.h"
#include "Event.h"
#include "Shop.h"
#include "BuffEffectSlot.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Not in use on 6.3 ENG
#ifdef PCBANG
CPCBangPointTimeCondition::CPCBangPointTimeCondition() //identical
{
	return;
}

CPCBangPointTimeCondition::~CPCBangPointTimeCondition() //identical
{
	return;
}

int CPCBangPointTimeCondition::GetTimeDate(int iYear, int iMonth, int iDay) //00498030 identical
{
	if(iMonth < 3)
	{
		iYear--;
		iMonth += 12;
	}

	int loc2 = iYear / 100; //
	int loc3 = iYear % 100; //

	int loc4 = ((21 * loc2) / 4) + ((5 * loc3) / 4) + ( iDay + (iMonth + 1) * 26 / 10) - 1;

	loc4 = loc4%7;

	return loc4;
}

//004980F0
BYTE CPCBangPointTimeCondition::IsLeapYear(int iYear) //identical
{
	int loc2;

	if( (iYear % 400) != 0 && ((iYear % 4) != 0 || (iYear % 100) == 0) )
	{
		loc2 = 0;
	}
	else
	{
		loc2 = 1;
	}

	return loc2;
}

int CPCBangPointTimeCondition::GetNumberedDays(int iYear, int iMonth) //00498160 identical
{
	switch(iMonth)
	{
	case 2:
		return !!IsLeapYear(iYear) + 28;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
		break;
	default:
		return 31;
	}
}

void CPCBangPointTimeCondition::ModifyTimeDate(int iDayOfWeek, int * iYear, int * iMonth, int * iDay) //00498210 identical
{
	int loc2 = this->GetTimeDate(*iYear, *iMonth, *iDay);
	int loc3 = ((loc2 + 7) - iDayOfWeek)%7;

	if(*iDay > loc3)
	{
		*iDay -= loc3;
	}
	else
	{
		loc3 -= *iDay;

		if(*iMonth == 1)
		{
			*iMonth = 12;
			*iYear -= 1;
		}
		else
		{
			*iMonth -= 1;
		}
		*iDay = this->GetNumberedDays(*iYear, *iMonth) - loc3;
	}
}

int CPCBangPointTimeCondition::CheckUserTimeDate(int iUSR_Year, int iUSR_Month, int iUSR_Day, int iUSR_Hour, int iSVR_Year, int iSVR_Month, int iSVR_Day, int iSVR_Hour) //4982F0 Identical
{
	if( iUSR_Year == iSVR_Year &&
		iUSR_Month == iSVR_Month &&
		iUSR_Day == iSVR_Day &&
		iUSR_Hour == iSVR_Hour )
	{
		return 0;
	}

	if( (iUSR_Year == iSVR_Year && iUSR_Month == iSVR_Month && iUSR_Day == iSVR_Day && iUSR_Hour > iSVR_Hour) ||
		(iUSR_Year == iSVR_Year && iUSR_Month == iSVR_Month && iUSR_Day > iSVR_Day) ||	
		(iUSR_Year == iSVR_Year && iUSR_Month > iSVR_Month) ||			 
		(iUSR_Year > iSVR_Year) )
	{
		return 1;
	}

	return -1; //mismatch :)
}

//004983B0
CPCBangPointTimer::CPCBangPointTimer(): m_iUSR_ResetYear(2000), m_iUSR_ResetMonth(1), m_iUSR_ResetDay(1), m_iSVR_ResetDayOfWeek(0), m_iTUserIndex(0), m_bSetTimer(FALSE), m_iUserIndex(0) //Identical
{
	return;
}

//00498490
CPCBangPointTimer::~CPCBangPointTimer() //Identical
{
	return;
}

//004984C0
void CPCBangPointTimer::SetResetTimeData(int iYear, int iMonth, int iDay, int iHour, int iSvrDayWeek, int iSvrHour, TUserIndex iTUserIndex, int aIndex) //identical
{
	//User Data
	this->m_iUSR_ResetYear = iYear;
	this->m_iUSR_ResetMonth = iMonth;
	this->m_iUSR_ResetDay = iDay;
	this->m_iUSR_ResetHour = iHour;

	//CommonServer Data
	this->m_iSVR_ResetDayOfWeek = iSvrDayWeek;
	this->m_iSVR_ResetHour = iSvrHour;
	this->m_iTUserIndex = iTUserIndex;

	this->m_iUserIndex = aIndex;
}

//00498540
void CPCBangPointTimer::Clear()
{
	this->SetCheckTimer(FALSE);
}

//00498570
int CPCBangPointTimer::CheckTime() //identical
{
	int iSVR_iYear, iSVR_iMonth, iSVR_iDay, iSVR_ResetHour;
	SYSTEMTIME m_tmTime; //

	if(this->m_bSetTimer == FALSE) //always false lol
	{
		return 1;
	}

	GetLocalTime(&m_tmTime);

	if( this->m_tmPcBangResetTime.wYear == m_tmTime.wYear &&
		this->m_tmPcBangResetTime.wMonth == m_tmTime.wMonth &&
		this->m_tmPcBangResetTime.wDay == m_tmTime.wDay &&
		this->m_tmPcBangResetTime.wHour == m_tmTime.wHour)
	{
		return 1;
	}

	this->m_tmPcBangResetTime = m_tmTime;

	iSVR_iYear = m_tmTime.wYear; //loc2
	iSVR_iMonth = m_tmTime.wMonth; //loc3
	iSVR_iDay = m_tmTime.wDay; //loc4
	iSVR_ResetHour = this->m_iSVR_ResetHour; //loc5

	if( this->m_iSVR_ResetHour > m_tmTime.wHour )
	{
		iSVR_iDay--;
	}

	this->ModifyTimeDate(this->m_iSVR_ResetDayOfWeek, &iSVR_iYear, &iSVR_iMonth, &iSVR_iDay);

	if(this->CheckUserTimeDate(this->m_iUSR_ResetYear, this->m_iUSR_ResetMonth, this->m_iUSR_ResetDay, this->m_iUSR_ResetHour, iSVR_iYear, iSVR_iMonth, iSVR_iDay, iSVR_ResetHour) < 0)
	{
		this->m_iUSR_ResetYear = m_tmTime.wYear;
		this->m_iUSR_ResetMonth = m_tmTime.wMonth;
		this->m_iUSR_ResetDay = m_tmTime.wDay;
		this->m_iUSR_ResetHour = m_tmTime.wHour;
		this->m_iTUserIndex(this, this->m_iUserIndex); //identical ok you are missing the other call...
		return 2;
	}

	return 1;
}

//00498780
void CPCBangPointTimer::SetCheckTimer(BOOL bResult) //identicalcl
{
	this->m_bSetTimer = bResult;
}

//004987B0
void CPCBangPointTimer::ChangeTimeDate(int * iYear, int * iMonth, int * iDay, int * iHour) //identical #unused
{
	this->m_iUSR_ResetYear = *iYear;
	this->m_iUSR_ResetMonth = *iMonth;
	this->m_iUSR_ResetDay = *iDay;
	this->m_iUSR_ResetHour = *iHour;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BYTE	g_btPcBangPointSystemLevel = 0;
short	g_sPCBangFirstRuleTime = 600;
short	g_sPCBangFirstRuleTimePoint = 1;
short	g_sPCBangCommonRuleTime = 600;
short	g_sPCBangCommonRuleTimePoint = 1;
short	g_sPCBangMaxPoint = 1008;
short	g_sPCBangFeverStartHour = 20;
short	g_sPCBangFeverEndHour = 24;
short	g_sPCBangFeverMultipleValue = 2;
short	g_sPCBangResetDayOfWeek = 2;
short	g_sPCBangResetHour = 9;
float	g_fPCBangAddExperience = 1.0f;

CPCBangPointSystem g_PCBangPointSystem;

CPCBangPointSystem::CPCBangPointSystem() //004988F0 (identical)
{
	this->m_bPointSystemOn = FALSE;
	this->m_bPointRankEvent = FALSE;
	g_btPcBangPointSystemLevel = 1;
	this->m_btPcBangShopNumber = 14;
	this->Init();
}

CPCBangPointSystem::~CPCBangPointSystem()
{

}

void CPCBangPointSystem::Init() //004989D0 (identical)
{
	for (int i = 0; i <= MAX_PCBANG_ITEM_COUNT; i++)
	{
		memset(&this->m_PcBangSystem[i], 0, sizeof(this->m_PcBangSystem[i]));
		this->m_PcBangSystem[i].Index = -1;
	}
}

int CPCBangPointSystem::Load(char* lpszFileName) //00498A40 (identical)
{
	this->m_iPcBangIndex = 0;

	if( lpszFileName == 0 || strcmp(lpszFileName, "") == 0 )
	{
		MsgBox("[PCBangPointSystem] - File load error : File Name Error");
		return 0;
	}

	DWORD dwGetLastError;

	try
	{
		SMDFile = fopen(lpszFileName, "r");

		if( SMDFile == NULL )
		{
			dwGetLastError = GetLastError();
			MsgBox("[PCBangPointSystem] - Can't Open %s ", lpszFileName);
			return 0;
		}

		this->Init();

		int iItemCount = 0;
		SMDToken Token;
		int iType = -1;
		WORD iItemType = 0;
		WORD iItemIndex = 0;
		WORD iBuyPoint = 0;
		WORD iCondition = 0;
		BYTE iAbility = 0;
		int iKeepTime = 0;
		WORD iAddExpPercent = 0;
		WORD iGetItemPercent = 0;
		BYTE iStopExp = 0;
		PCBANG_LOAD_INFO* m_PcBangInfo;

		while( true )
		{
			Token = GetToken();

			if( Token == 2 )
			{
				break;
			}

			iType = TokenNumber;

			while( true )
			{
				if( iType == 0 )
				{
					iItemType = 0;
					iItemIndex = 0;
					iBuyPoint = 0;
					iCondition = 0;
					iAbility = 0;
					iKeepTime = 0;
					iAddExpPercent = 0;
					iGetItemPercent = 0;
					iStopExp = 0;

					Token = GetToken();

					if( strcmp("end", TokenString) == 0 )
					{
						break;
					}

					iItemType = TokenNumber;
					Token = GetToken();

					iItemIndex = TokenNumber;
					Token = GetToken();

					iBuyPoint = TokenNumber;
					Token = GetToken();

					iCondition = TokenNumber;
					Token = GetToken();

					iAbility = TokenNumber;
					Token = GetToken();

					iKeepTime = TokenNumber;
					Token = GetToken();

					iAddExpPercent = TokenNumber;
					Token = GetToken();

					iGetItemPercent = TokenNumber;
					Token = GetToken();

					iStopExp = TokenNumber;
					
					if( iItemCount < 0 || iItemCount > MAX_PCBANG_ITEM_COUNT )
					{
						MsgBox("[PCBangPointSystem] - Exceed Max Item Count (%d)", iItemCount);
						break;
					}

					m_PcBangInfo = (PCBANG_LOAD_INFO*)&this->m_PcBangSystem[iItemCount].Index;
					m_PcBangInfo->Index = (iItemType << 9) + iItemIndex;
					m_PcBangInfo->BuyPoint = iBuyPoint;
					m_PcBangInfo->Condition = iCondition;
					m_PcBangInfo->Ability = iAbility;
					m_PcBangInfo->KeepTime = iKeepTime;
					m_PcBangInfo->AddExpPercent = iAddExpPercent;
					m_PcBangInfo->GetItemPercent = iGetItemPercent;
					m_PcBangInfo->StopExp = iStopExp;
					iItemCount++;
				}
				else
				{
					Token = GetToken();

					if( strcmp("end", TokenString) == 0 )
					{
						break;
					}
				}
			}
		}

		fclose(SMDFile);
		LogAddC(2, "[PCBangPointSystem] - %s file is Loaded", lpszFileName);
	}
	
	catch( DWORD )
	{
		MsgBox("[PCBangPointSystem] Loading Exception Error (%s) File. ", lpszFileName);
	}

	this->m_bPointSystemOn = GetPrivateProfileInt("GameServerInfo", "PCBangPointSystemOn", TRUE, gDirPath.GetNewPath("commonserver.cfg"));
	
	g_btPcBangPointSystemLevel = GetPrivateProfileInt("GameServerInfo", "PCBangPointSystemLevel", 1, gDirPath.GetNewPath("commonserver.cfg"));
	
	this->m_bPointRankEvent = GetPrivateProfileInt("GameServerInfo", "PCBangPointRankingEvent", 1, gDirPath.GetNewPath("commonserver.cfg"));

	g_sPCBangFirstRuleTime = GetPrivateProfileInt("GameServerInfo", "PCBangPointFirstRuleTime", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_sPCBangFirstRuleTimePoint = GetPrivateProfileInt("GameServerInfo", "PCBangPointFirstRuleTimePoint", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_sPCBangCommonRuleTime = GetPrivateProfileInt("GameServerInfo", "PCBangPointCommonRuleTime", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_sPCBangCommonRuleTimePoint = GetPrivateProfileInt("GameServerInfo", "PCBangPointCommonRuleTimePoint", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_sPCBangMaxPoint = GetPrivateProfileInt("GameServerInfo", "PCBangMaxPoint", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_sPCBangFeverStartHour = GetPrivateProfileInt("GameServerInfo", "PCBangPointFeverStartHour", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_sPCBangFeverEndHour = GetPrivateProfileInt("GameServerInfo", "PCBangPointFeverEndHour", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_sPCBangFeverMultipleValue = GetPrivateProfileInt("GameServerInfo", "PCBangPointFeverMultipleValue", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_sPCBangResetDayOfWeek = GetPrivateProfileInt("GameServerInfo", "PCBangResetDayOfWeek", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_sPCBangResetHour = GetPrivateProfileInt("GameServerInfo", "PCBangResetHour", 1, gDirPath.GetNewPath("commonserver.cfg"));
		
	char szTemp[256];
	GetPrivateProfileString("GameServerInfo", "PCBangAddExperience", "1", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	g_fPCBangAddExperience = atof(szTemp);

	LogAdd("[PCBangPointSystem][SetPCBangPointInfo] SystemOn:%d, Level:%d, RankingEventOn:%d", this->m_bPointSystemOn, g_btPcBangPointSystemLevel, this->m_bPointRankEvent); 

	return this->m_iPcBangIndex;
}

BOOL CPCBangPointSystem::CheckPcBangRuleTime(int aIndex) //004996A0 (identical)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->m_iPcBangConnectionType == 1)
	{
		return TRUE;
	}

	return FALSE;
}

int CPCBangPointSystem::GetItemIndex(int iItemIndex) //004996F0 (identical) (3x call user.cpp)
{
	PCBANG_LOAD_INFO* m_PcBangInfo = this->m_PcBangSystem;

	for (int n = 0; n < MAX_PCBANG_ITEM_COUNT; n++)
	{
		if (this->m_PcBangSystem[n].Index == iItemIndex)
		{
			return 1;
		}
	}

	return 0;
}

int CPCBangPointSystem::ShopMapCheck(BYTE MapNumber, short X, short Y) //00499760 (identical)
{
	if( BC_MAP_RANGE(MapNumber) ||
		CC_MAP_RANGE(MapNumber) ||
		DS_MAP_RANGE(MapNumber) ||
		IT_MAP_RANGE(MapNumber) ||
		MapNumber == MAP_INDEX_KANTURU_BOSS )
	{
		return 0;
	}

	BYTE attr = MapC[MapNumber].GetAttr(X, Y);

	if( (attr &1) == 1 )
	{
		return 1;
	}

	return 0;
}

void CPCBangPointSystem::SetPointUser(int aIndex, int iRuleTime) //00499930 (identical) (1x call user.cpp)
{
	if( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if( g_btPcBangPointSystemLevel == 0 )
	{
		gObj[aIndex].m_iPcBangConnectionType = 0;
	}
	else
	{
		if( g_btPcBangPointSystemLevel == 1 )
		{
			gObj[aIndex].m_iPcBangConnectionType = iRuleTime;
		}
		else if( g_btPcBangPointSystemLevel == 2 )
		{
			gObj[aIndex].m_iPcBangConnectionType = 1;
		}
	}

	if( gObj[aIndex].m_iPcBangConnectionType == 1 )
	{
		LogAdd("[PCBangPointSystem][SetPCBangPointUser] [%s]", gObj[aIndex].AccountID);
	}

	g_PCBangPointSystem.EGUserConnectPcBang(aIndex);
}

BOOL CPCBangPointSystem::AddPoint(LPOBJ lpObj) //00499A60 (identical) (1x call user.cpp) (gObjSecondProc)
{
	if( lpObj->Type != OBJ_USER )
	{
		return TRUE;
	}

	if( this->m_bPointSystemOn == FALSE )
	{
		return TRUE;
	}

	if( lpObj->Connected < PLAYER_CONNECTED )
	{
		return TRUE;
	}

	if( lpObj->m_PcBangPointSystem.m_bPcBangPointEnable == FALSE )
	{
		return TRUE;
	}

	if( lpObj->m_PcBangPointSystem.m_dwPcBangPointTick <= 0  )
	{
		lpObj->m_PcBangPointSystem.m_dwPcBangPointTick = GetTickCount();
		return TRUE;
	}

	BOOL bFeverTime; //loc2

	SYSTEMTIME sysTime; //
	GetLocalTime(&sysTime); //
	
	bFeverTime = FALSE;

	if( g_sPCBangFeverStartHour <= g_sPCBangFeverEndHour )
	{
		if( sysTime.wHour >= g_sPCBangFeverStartHour && sysTime.wHour < g_sPCBangFeverEndHour)
		{
			bFeverTime = TRUE;
		}
	}
	else if( sysTime.wHour >= g_sPCBangFeverStartHour || sysTime.wHour < g_sPCBangFeverEndHour )
	{
		bFeverTime = TRUE;
	}

	DWORD iCommonRuleTime = (GetTickCount() - lpObj->m_PcBangPointSystem.m_dwPcBangPointTick) / 1000; //loc7

	if( iCommonRuleTime >= lpObj->m_PcBangPointSystem.m_sPcBangCommonRuleTime )
	{
		//Timer Class Start
		if(lpObj->m_PCBangPointTimer.CheckTime() != 2)
		{
			lpObj->m_PcBangPointSystem.m_dwPcBangPointTick = GetTickCount();

			if(this->CheckPcBangRuleTime(lpObj->m_Index) == 1)
			{
				int iPointValue = (bFeverTime == TRUE)?(lpObj->m_PcBangPointSystem.m_sPcBangGainPoint * g_sPCBangFeverMultipleValue):(lpObj->m_PcBangPointSystem.m_sPcBangGainPoint); //epic :)

				lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint += iPointValue;

				if(lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint < 0)
				{
					lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint = 0;
				}
				else if(lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint > g_sPCBangMaxPoint)
				{
					lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint = g_sPCBangMaxPoint;
					GCServerMsgStringSend(lMsg.Get(3393), lpObj->m_Index, 1);
				}
				else
				{
					char szTemp[256];
					wsprintf(szTemp, lMsg.Get(3392), iPointValue);
					GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				}

				this->EGReqUpdatePcBangResetPointInfo(lpObj->m_Index);

				LogAddTD("[PCBangPointSystem][Add Point] [%s] %d/Total:%d Rule(%d)", 
					lpObj->AccountID, lpObj->m_PcBangPointSystem.m_sPcBangGainPoint, 
					lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint, lpObj->m_PcBangPointSystem.m_bPcBangCommonRule);

				if(lpObj->m_PcBangPointSystem.m_bPcBangInfoSet == FALSE)
				{
					lpObj->m_PcBangPointSystem.m_bPcBangCommonRule = 1;
					lpObj->m_PcBangPointSystem.m_bPcBangInfoSet = TRUE;
					lpObj->m_PcBangPointSystem.m_sPcBangCommonRuleTime = g_sPCBangCommonRuleTime;
					lpObj->m_PcBangPointSystem.m_sPcBangGainPoint = g_sPCBangCommonRuleTimePoint;
				}
			}
		}
	}

	return TRUE;
}

void CPCBangPointSystem::EGUserConnectTest(int aIndex) //00499E20 (identical) #unused
{
	if(gObj[aIndex].m_iPcBangConnectionType == 1) //LocalServer
	{
		PMSG_REQ_REG_PC_POINT pMsg = {0};

		PHeadSetB((LPBYTE)&pMsg, 0x23, sizeof(pMsg));

		pMsg.aIndex = aIndex;

		memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);

		pMsg.btResult = 0;

		pMsg.AccountID[10] = '\0';

		DataSendEventChip((char*)&pMsg, sizeof(pMsg));

		LogAdd("[PCBangPointSystem][User Connect] [%s] - %s", 
			gObj[aIndex].AccountID, gObj[aIndex].Ip_addr);
	}
}

void CPCBangPointSystem::EGAnsPcBangPointInfo(PMSG_ANS_REG_PC_POINT* aRecv) //00499F20 (identical) (1x call event.cpp)
{
	PMSG_ANS_REG_PC_POINT* m_PCInfo = aRecv;
	int aIndex = m_PCInfo->aIndex;

	if( OBJMAX_RANGE(aIndex) == 0 )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->m_PcBangPointSystem.m_bPcBangPointEnable = TRUE;
	lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint = m_PCInfo->sPoint;
	lpObj->m_PcBangPointSystem.m_dwPcBangPointTick = GetTickCount();

	this->GCSendPcBangUserPoint(aIndex);
}

void CPCBangPointSystem::EGReqUpdatePcBangPointInfo(int aIndex) //00499FE0 #unused (identical)
{
	PMSG_ANS_REG_PC_POINT pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, 0x24, sizeof(pMsg));

	pMsg.aIndex = aIndex;

	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);

	pMsg.AccountID[10] = '\0';

	pMsg.btShopItemBuy = 0;

	pMsg.sPoint = gObj[aIndex].m_PcBangPointSystem.m_iPcBangAccumulatedPoint;

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));
}

void CPCBangPointSystem::EGAnsUserFirstPcBangUpdatePoint(PMSG_ANS_REG_PC_POINT* aRecv) //0049A0A0 (identical) (1x call event.cpp)
{
	PMSG_ANS_REG_PC_POINT* m_PCInfo = aRecv;
	int aIndex = m_PCInfo->aIndex;

	if( OBJMAX_RANGE(aIndex) == 0 )
	{
		return;
	}

	if(m_PCInfo->btShopItemBuy == 1)
	{
		this->GCSendPcBangUserPoint(aIndex);
	}
}

void CPCBangPointSystem::EGReqUpdatePcBangPointInfo(int aIndex, short sPCBangPoint) //0049A120 (identical)
{
	if(this->m_bPointRankEvent == FALSE)
	{
		return;
	}

	PMSG_REQ_UPDATE_PCBANG_USER_POINT pMsg = {0};
	
	PHeadSetB((LPBYTE)&pMsg, 0x25, sizeof(pMsg));

	pMsg.aIndex = aIndex;

	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);

	pMsg.AccountID[10] = '\0';

	pMsg.btResult = 0;

	pMsg.sPoint = sPCBangPoint;

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));
}

void CPCBangPointSystem::GCSendPcBangUserPoint(int aIndex) //0049A1E0 (identical)
{
	PMSG_ANS_PCBANG_POINT_INFO pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x04, sizeof(pMsg));

	pMsg.m_sPoint = gObj[aIndex].m_PcBangPointSystem.m_iPcBangAccumulatedPoint;
	pMsg.m_sMaxPoint = g_sPCBangMaxPoint;
	pMsg.m_btType = gObj[aIndex].m_iPcBangConnectionType;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CPCBangPointSystem::PCBangShopOpen(int aIndex) //0049A290 (identical) (1x call protocol.cpp)
{
	LPOBJ lpObj = &gObj[aIndex];

	if( lpObj->CloseType != -1 )
	{
		return;
	}

	if( gObjIsConnectedGP(aIndex) == 0 )
	{
		LogAddTD("[ANTI-HACK][PCBangPointShopOpen] Couldn't Open.. (%s)(%s)", lpObj->AccountID, lpObj->Name);
		return;
	}

	PMSG_SHOP_INFO pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x06, sizeof(pMsg));
	pMsg.Result = 0;

	if( this->ShopMapCheck(lpObj->MapNumber, lpObj->X, lpObj->Y) == 0 )
	{
		pMsg.Result = 1;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if( ShopC[this->m_btPcBangShopNumber].ItemCount < 1 )
	{
		pMsg.Result = 2;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != 18 )
	{
		pMsg.Result = 3;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if( lpObj->m_ReqWarehouseOpen != 0 )
	{
		pMsg.Result = 3;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if( gObjFixInventoryPointer(aIndex) == 0 )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}
	
	PMSG_SHOP_ITEM_COUNT pShopItem;
	BYTE SendByte[1024];

	int lOfs = 0;
	lpObj->TargetShopNumber = this->m_btPcBangShopNumber;
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 18;
	lpObj->m_ShopTime = 0;
	lOfs += sizeof(pShopItem);

	int size = lOfs + ShopC[this->m_btPcBangShopNumber].SendItemDataLen;
	PHeadSetW((LPBYTE)&pShopItem, 0x31, size);
	pShopItem.Type = 0;
	pShopItem.count = ShopC[this->m_btPcBangShopNumber].ItemCount;

	memcpy(SendByte, &pShopItem, sizeof(pShopItem));
	memcpy(&SendByte[lOfs], ShopC[this->m_btPcBangShopNumber].SendItemData, ShopC[this->m_btPcBangShopNumber].SendItemDataLen);

	DataSend(aIndex, SendByte, size);
} 

int CPCBangPointSystem::PCBangBuyItem(int aIndex, BYTE ItemPosition) //0049A660 (identical) (1x call protocol.cpp)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	PMSG_POINT_BUY_ITEM pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x05, sizeof(pMsg));
	pMsg.Result = 6;
	pMsg.ItemPos = -1;

	if( lpObj->CloseType != -1 )
	{
		return 0;
	}

	if( this->ShopMapCheck(lpObj->MapNumber, lpObj->X, lpObj->Y) == 0 )
	{
		pMsg.Result = 1;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return 0;
	}

	if( gObjFixInventoryPointer(aIndex) == 0 )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if( gObj[aIndex].m_ShopTime == 0 )
	{
		gObj[aIndex].m_ShopTime = 1;
	}

	if( gObj[aIndex].m_ShopTime > 60 )
	{
		if( lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 18 )
		{
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.use = 0;
			lpObj->m_IfState.type = 0;
		}

		pMsg.Result = 7;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return 0;
	}

	if( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != 18 )
	{
		pMsg.Result = 5;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return 0;
	}

	int iShopNumber = lpObj->TargetShopNumber;

	if( iShopNumber < 0 || iShopNumber > MAX_SHOP-1 )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return 0;
	}

	if( ItemPosition > MAX_ITEM_IN_SHOP-1 )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return 0;
	}
	
	if( ShopC[iShopNumber].m_item[ItemPosition].IsItem() == 0 )
	{
		pMsg.Result = 2;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return 0;
	}

	BOOL bItem = FALSE;
	int iItemType = ShopC[iShopNumber].m_item[ItemPosition].m_Type;
	int iPos = 0;
	PCBANG_LOAD_INFO* m_PCBangInfo = this->m_PcBangSystem;

	for( int n = 0; n < MAX_PCBANG_ITEM_COUNT; n++ )
	{
		if( this->m_PcBangSystem[n].Index == iItemType )
		{
			iPos = n;
			bItem = TRUE;
		}
	}

	if( bItem == FALSE )
	{
		pMsg.Result = 8;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return 0;
	}

	if( lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint < this->m_PcBangSystem[iPos].BuyPoint )
	{
		pMsg.Result = 9;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return 0;
	}

	if( lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint < this->m_PcBangSystem[iPos].Condition ) //wz bug
	{
		pMsg.Result = 4;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return 0;
	}

	if( this->m_PcBangSystem[iPos].Ability == 1 )
	{
		ItemByteConvert((LPBYTE)&pMsg.ItemInfo, ShopC[iShopNumber].m_item[ItemPosition]);
		pMsg.Result = 0;
		pMsg.ItemPos = -1;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		int iEffectIndex = 0;

		if( ShopC[iShopNumber].m_item[ItemPosition].m_Type == ITEMGET(13,43) )
		{
			iEffectIndex = AT_PCBANG_SEAL_OF_ASCENSION;
		}
		else if( ShopC[iShopNumber].m_item[ItemPosition].m_Type == ITEMGET(13,44) )
		{
			iEffectIndex = AT_PCBANG_SEAL_OF_WEALTH;
		}
		else if( ShopC[iShopNumber].m_item[ItemPosition].m_Type == ITEMGET(13,45) )
		{
			iEffectIndex = AT_PCBANG_SEAL_OF_SUSTENANCE;
		}
		else if( ShopC[iShopNumber].m_item[ItemPosition].m_Type == ITEMGET(13,96) )
		{
			iEffectIndex = AT_PCBANG_SEAL_OF_BALANCE;
		}

		gObjAddBuffEffect(lpObj, iEffectIndex);

		lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint -= this->m_PcBangSystem[iPos].BuyPoint;

		if( lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint < 0 )
		{
			lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint = 0;
		}

		this->EGReqUpdatePcBangResetPointInfo(aIndex); //Update Reset Time Date In Database

		this->EGReqUpdatePcBangPointInfo(aIndex, this->m_PcBangSystem[iPos].BuyPoint); //Decrease Point in Database if Rank activated

		LogAddTD("[PCBangPointSystem][BuyItem][%s][%s](%d/%d/%d) (%d) [%s][%d] LEV:%d, DUR:%d, OP:[%d][%d][%d]", 
			lpObj->AccountID, lpObj->Name, this->m_PcBangSystem[iPos].BuyPoint, lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint, 
			g_sPCBangMaxPoint, iShopNumber, ShopC[iShopNumber].m_item[ItemPosition].GetName(), this->m_PcBangSystem[iPos].KeepTime, 
			ShopC[iShopNumber].m_item[ItemPosition].m_Level, 
			(int)(ShopC[iShopNumber].m_item[ItemPosition].m_Durability), 
			ShopC[iShopNumber].m_item[ItemPosition].m_Option1, 
			ShopC[iShopNumber].m_item[ItemPosition].m_Option2, 
			ShopC[iShopNumber].m_item[ItemPosition].m_Option3);
	}
	else
	{
		pMsg.ItemPos = gObjInventoryInsertItem(aIndex, ShopC[iShopNumber].m_item[ItemPosition]);

		if( pMsg.ItemPos != 0xFF )
		{
			ItemByteConvert((LPBYTE)&pMsg.ItemInfo, ShopC[iShopNumber].m_item[ItemPosition]);
			pMsg.Result = 0;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint -= this->m_PcBangSystem[iPos].BuyPoint;

			LogAddTD("[PCBangPointSystem][BuyItem][%s][%s](%d/%d/%d) (%d) [%s][%d] LEV:%d, DUR:%d, OP:[%d][%d][%d]", 
				lpObj->AccountID, lpObj->Name, this->m_PcBangSystem[iPos].BuyPoint, 
				lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint, g_sPCBangMaxPoint, iShopNumber, 
				ShopC[iShopNumber].m_item[ItemPosition].GetName(), 
				this->m_PcBangSystem[iPos].KeepTime, 
				ShopC[iShopNumber].m_item[ItemPosition].m_Level, 
				(int)ShopC[iShopNumber].m_item[ItemPosition].m_Durability, 
				ShopC[iShopNumber].m_item[ItemPosition].m_Option1, 
				ShopC[iShopNumber].m_item[ItemPosition].m_Option2, 
				ShopC[iShopNumber].m_item[ItemPosition].m_Option3);
			
			if( lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint < 0 )
			{
				lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint = 0;
			}

			this->EGReqUpdatePcBangResetPointInfo(aIndex); //Update Reset Time Date In Database

			this->EGReqUpdatePcBangPointInfo(aIndex, this->m_PcBangSystem[iPos].BuyPoint); //Decrease Point in Database if Rank activated
		}
		else
		{
			pMsg.Result = 3;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return 0;
		}
	}

	return 1;
}

void CPCBangPointTimer::SetUserTimer(CPCBangPointTimer* pthis, int index) //0049B1A0 (identical) #hidden code inside
{
	short aIndex = index;

	return;
	pthis->m_iUserIndex = aIndex;
	pthis->SetCheckTimer(TRUE);

	int iYear = gObj[aIndex].m_PcBangPointSystem.m_sPcBangResetYear;
	int iMonth = gObj[aIndex].m_PcBangPointSystem.m_sPcBangResetMonth;
	int iDay = gObj[aIndex].m_PcBangPointSystem.m_sPcBangResetDay;
	int iHour = gObj[aIndex].m_PcBangPointSystem.m_sPcBangResetHour;

	pthis->ChangeTimeDate(&iYear, &iMonth, &iDay, &iHour);
}

void CPCBangPointSystem::EGUserConnectPcBang(int aIndex) //0049B1C0 (identical)
{
	PMSG_REQ_REG_PC_POINT pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, 0x29, sizeof(pMsg));

	pMsg.aIndex = aIndex;

	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);

	pMsg.btResult = 0;

	pMsg.AccountID[10] = '\0';

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));

	LogAdd("[PCBangPointSystem][User Connect] [%s] - %s", 
		gObj[aIndex].AccountID, gObj[aIndex].Ip_addr);
}

void CPCBangPointSystem::EGAnsUpdatePcBangResetPointInfo(PMSG_ANS_REG_PC_POINT* aRecv) //0049B2A0 (identical) (1x call event.cpp)
{
	PMSG_ANS_REG_PC_POINT* m_PCInfo = aRecv;
	int aIndex = m_PCInfo->aIndex;

	if( OBJMAX_RANGE(aIndex) == 0 )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->m_PcBangPointSystem.m_bPcBangPointEnable = TRUE;

	lpObj->m_PcBangPointSystem.m_iPcBangAccumulatedPoint = m_PCInfo->sPoint;
	lpObj->m_PcBangPointSystem.m_dwPcBangPointTick = GetTickCount();

	lpObj->m_PcBangPointSystem.m_sPcBangResetYear = m_PCInfo->sPcBangResetYear;
	lpObj->m_PcBangPointSystem.m_sPcBangResetMonth = m_PCInfo->sPcBangResetMonth;
	lpObj->m_PcBangPointSystem.m_sPcBangResetDay = m_PCInfo->sPcBangResetDay;
	lpObj->m_PcBangPointSystem.m_sPcBangResetHour = m_PCInfo->sPcBangResetHour;

	lpObj->m_PCBangPointTimer.SetResetTimeData(lpObj->m_PcBangPointSystem.m_sPcBangResetYear, lpObj->m_PcBangPointSystem.m_sPcBangResetMonth, lpObj->m_PcBangPointSystem.m_sPcBangResetDay, lpObj->m_PcBangPointSystem.m_sPcBangResetHour, g_sPCBangResetDayOfWeek, g_sPCBangResetHour, CPCBangPointTimer::SetUserTimer, aIndex);

	lpObj->m_PCBangPointTimer.SetCheckTimer(FALSE);

	if(lpObj->m_PCBangPointTimer.CheckTime() != 2)
	{
		this->GCSendPcBangUserPoint(aIndex);
	}
}

void CPCBangPointSystem::EGReqUpdatePcBangResetPointInfo(int aIndex) //0049B450 (identical) (2x call gmmng.cpp)
{
	PMSG_ANS_REG_PC_POINT pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, 0x30, sizeof(pMsg));

	pMsg.aIndex = aIndex;

	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);

	pMsg.AccountID[10] = '\0';

	pMsg.btShopItemBuy = 0;

	pMsg.sPoint = gObj[aIndex].m_PcBangPointSystem.m_iPcBangAccumulatedPoint;

	pMsg.sPcBangResetYear = gObj[aIndex].m_PcBangPointSystem.m_sPcBangResetYear;
	pMsg.sPcBangResetMonth = gObj[aIndex].m_PcBangPointSystem.m_sPcBangResetMonth;
	pMsg.sPcBangResetDay = gObj[aIndex].m_PcBangPointSystem.m_sPcBangResetDay;
	pMsg.sPcBangResetHour = gObj[aIndex].m_PcBangPointSystem.m_sPcBangResetHour;

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));
}

void CPCBangPointSystem::EGAnsUserPcBangUpdatePoint(PMSG_ANS_REG_PC_POINT* aRecv) //0049B590 (identical) (1x call event.cpp)
{
	PMSG_ANS_REG_PC_POINT* m_PCInfo = aRecv;
	int aIndex = m_PCInfo->aIndex;

	if( OBJMAX_RANGE(aIndex) == 0 )
	{
		return;
	}

	if(m_PCInfo->btShopItemBuy == 1)
	{
		this->GCSendPcBangUserPoint(aIndex);
	}
}

BOOL CPCBangPointSystem::AddExperience(LPOBJ lpObj, __int64 & Experience) //0049B610 (identical) (4x call Exp functions)
{
	if(lpObj->m_iPcBangConnectionType == 0)
	{
		return FALSE;
	}

	__int64 iCAL_EXP = int(Experience * g_fPCBangAddExperience);

	Experience = iCAL_EXP;

	return TRUE;
}
#endif