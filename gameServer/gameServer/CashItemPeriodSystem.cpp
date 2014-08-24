//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include "CashItemPeriodSystem.h"
#include "DBSockMng.h"
#include "LogProc.h"
#include "ItemAddOption.h"
#include "..\common\winutil.h"
#include "BuffEffectSlot.h"
#include "ObjCalCharacter.h"

CCashItemPeriodSystem g_CashItemPeriodSystem;
BOOL g_bRunningPeriodCheckThread = FALSE;

CCashItemPeriodSystem::CCashItemPeriodSystem()
{

}

CCashItemPeriodSystem::~CCashItemPeriodSystem()
{

}

void CCashItemPeriodSystem::Initialize()
{
	int i;
	this->iItemPeriodInfoCount = 0;

	for( i = 0; i < MAX_ITEM_PERIOD_INFO; i++ )
	{
		this->ItemPeriodInfo[i].btUsedInfo = 0;
		this->ItemPeriodInfo[i].btCountItems = 0;
		this->ItemPeriodInfo[i].wdUserIndex = 0;
		this->ItemPeriodInfo[i].iUserGuid = 0;
		this->ItemPeriodInfo[i].dwItemCheckTickCount = 0;
		this->ItemPeriodInfo[i].chCharacterName[0] = NULL;

		for( int j = 0; j < MAX_ITEM_PERIOD_EFFECT; j++ )
		{
			this->ItemPeriodInfo[i].vItemEffect[j].bInUse = false;
			this->ItemPeriodInfo[i].vItemEffect[j].btOptionType = 0;
			this->ItemPeriodInfo[i].vItemEffect[j].btEffectType1 = 0;
			this->ItemPeriodInfo[i].vItemEffect[j].btEffectType2 = 0;
			this->ItemPeriodInfo[i].vItemEffect[j].iItemCode = -1;
			this->ItemPeriodInfo[i].vItemEffect[j].iItemPeriodUsedTime = 0;
			this->ItemPeriodInfo[i].vItemEffect[j].iItemPeriodLeftTime = 0;
		}
	}

	if( g_bRunningPeriodCheckThread == FALSE )
	{
		this->hThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CCashItemPeriodSystem::PeriodCheckThread, this->ItemPeriodInfo, 0, NULL);
		
		if( this->hThreadHandle )	//Fix
		{
			g_bRunningPeriodCheckThread = 1;
			CloseHandle(this->hThreadHandle);
		}
		else
		{
			LogAdd("CreateThread() failed with error %d", GetLastError());
		}
	}
}

DWORD CCashItemPeriodSystem::PeriodCheckThread(void * lpPeriodInfo)
{
	ITEMPERIOD_INFO* lpPeriodInfoList = (ITEMPERIOD_INFO *)lpPeriodInfo;
	int iIndex = 0;
	DWORD dwCurrentTick = 0;
	LPOBJ lpObj = NULL;
	int iUseTime = 0;
	int iOptionIndex = 0;

	while ( true )
	{
		if ( (GetTickCount() - dwCurrentTick) > 10000 )
		{
			dwCurrentTick = GetTickCount();

			for ( iIndex=0;iIndex<MAX_ITEM_PERIOD_INFO;iIndex++)
			{
				if ( lpPeriodInfoList[iIndex].btUsedInfo == 1 )
				{
					iUseTime = GetTickCount() - lpPeriodInfoList[iIndex].dwItemCheckTickCount;

					if ( iUseTime > 60000 )
					{
						lpPeriodInfoList[iIndex].dwItemCheckTickCount = GetTickCount();
						
						for( iOptionIndex = 0; iOptionIndex < MAX_ITEM_PERIOD_EFFECT; iOptionIndex++ )
						{
							if( lpPeriodInfoList[iIndex].vItemEffect[iOptionIndex].bInUse == true )
							{
								lpPeriodInfoList[iIndex].vItemEffect[iOptionIndex].iItemPeriodLeftTime--;
								lpPeriodInfoList[iIndex].vItemEffect[iOptionIndex].iItemPeriodUsedTime++;
								
								if ( lpPeriodInfoList[iIndex].vItemEffect[iOptionIndex].iItemPeriodLeftTime <= 0 )
								{
									lpObj = &gObj[lpPeriodInfoList[iIndex].wdUserIndex];
									g_CashItemPeriodSystem.GDReqPeriodItemUpdate(lpObj, iOptionIndex );
									g_CashItemPeriodSystem.ClearPeriodItemEffect(lpObj, lpPeriodInfoList[iIndex].vItemEffect[iOptionIndex].iItemCode );
								}
							}
						}
					}
				}
			}
		}
		else
		{
			Sleep(10);
		}
	}

	return 0;
}

void CCashItemPeriodSystem::GetUserPeriodItemList(LPOBJ lpObj)
{
	this->GDReqPeriodItemList(lpObj);
}

int CCashItemPeriodSystem::CalcItemUseLeftTime(LPSTR pchBuyDate)
{
	int iUsedTime = 0;
	CTime tCurrentTime = CTime::GetTickCount();
	CTime tBuyTime;
	CTimeSpan tTimeSpan;

	this->ConvertStringToTime(pchBuyDate, &tBuyTime);
	tCurrentTime - tBuyTime;
	tTimeSpan = tBuyTime - tCurrentTime;
	iUsedTime = tTimeSpan.GetTotalMinutes();

	return iUsedTime;
}

CTime CCashItemPeriodSystem::CalcItemUseEndTime(int iLeftTime)
{
	CTime tExpireTime;
	CTime tCurrentTime = CTime::GetTickCount();

	CTimeSpan tLeftTime(0, 0, iLeftTime, 0);

	tExpireTime = tCurrentTime + tLeftTime;

	return tExpireTime;
}

void CCashItemPeriodSystem::ConvertStringToTime(const char * pchDate, class CTime * lpTime)//Identical
{
	int iYear=0;
	int iMonth=0;
	int iDay=0;
	int iHour=0;
	int iMinute=0;
	int iDateLength=0;
	char chDate[20];
	char* Token;
	char seps[4]="-: ";

	if ( pchDate == NULL || lpTime == NULL )
		return;

	memcpy(chDate, pchDate, sizeof(chDate));
	iDateLength = strlen(chDate);

	if ( iDateLength <= 0 || iDateLength > 20 )
	{
		return;
	}

	Token = strtok( chDate, seps);
	iYear = atoi(Token);
	Token = strtok( NULL, seps);
	iMonth = atoi(Token);
	Token = strtok( NULL, seps);
	iDay = atoi(Token);
	Token = strtok( NULL, seps);
	iHour = atoi(Token);
	Token = strtok( NULL, seps);
	iMinute = atoi(Token);

	CTime tReturnTime(iYear, iMonth, iDay, iHour, iMinute, 0, -1);

	*lpTime = tReturnTime;
}

void CCashItemPeriodSystem::ConvertTimeToString(const CTime * lpTime, LPSTR pchDate)
{
	int iYear=0;
	int iMonth=0;
	int iDay=0;
	int iHour=0;
	int iMinute=0;
	char chDate[20];

	iYear = lpTime->GetYear();
	iMonth = lpTime->GetMonth();
	iDay = lpTime->GetDay();
	iHour = lpTime->GetHour();
	iMinute = lpTime->GetMinute();

	wsprintf(chDate, "%d-%d-%d %d:%d", iYear, iMonth, iDay, iHour, iMinute);
	memcpy(pchDate, chDate, sizeof(chDate));
}

BOOL CCashItemPeriodSystem::SetPeriodItemEffect(LPOBJ lpObj, int iItemCode, BYTE btEffectType1, BYTE btEffectType2, DWORD dwItemPeriod)
{
	int iPeriodItemEffectIndex = - 1;
	int iOptionEffect = 0;
	int iIndex = 0;
	int iOptionIndex = 0;

	iOptionEffect = g_ItemAddOption.SearchItemOptionType(iItemCode);

	if( iOptionEffect == -1 )
		return FALSE;

	this->SearchAndDeleteItemPeriodEffect(lpObj,iItemCode);

	if ( lpObj->m_iPeriodItemEffectIndex != -1 )
	{
		iPeriodItemEffectIndex = lpObj->m_iPeriodItemEffectIndex;
		this->ItemPeriodInfo[iPeriodItemEffectIndex].btCountItems++;

		for( iOptionIndex = 0; iOptionIndex < MAX_ITEM_PERIOD_EFFECT; iOptionIndex++ )
		{
			if( this->ItemPeriodInfo[iPeriodItemEffectIndex].vItemEffect[iOptionIndex].bInUse == false )
			{
				this->ItemPeriodInfo[iPeriodItemEffectIndex].vItemEffect[iOptionIndex].bInUse = true;
				this->ItemPeriodInfo[iPeriodItemEffectIndex].vItemEffect[iOptionIndex].iItemCode = iItemCode;
				this->ItemPeriodInfo[iPeriodItemEffectIndex].vItemEffect[iOptionIndex].btEffectType1 = btEffectType1;
				this->ItemPeriodInfo[iPeriodItemEffectIndex].vItemEffect[iOptionIndex].btEffectType2 = btEffectType2;
				this->ItemPeriodInfo[iPeriodItemEffectIndex].vItemEffect[iOptionIndex].btOptionType = iOptionEffect;
				this->ItemPeriodInfo[iPeriodItemEffectIndex].vItemEffect[iOptionIndex].iItemPeriodLeftTime = dwItemPeriod;
				this->ItemPeriodInfo[iPeriodItemEffectIndex].vItemEffect[iOptionIndex].iItemPeriodUsedTime = 0;

				this->ApplyItemPeriodEffect(lpObj,iItemCode,dwItemPeriod);
				return TRUE;
			}
		}
	}
	else
	{
		 
		for( iIndex = 0; iIndex < MAX_ITEM_PERIOD_INFO; iIndex++ )
		{
			if( this->ItemPeriodInfo[iIndex].btUsedInfo == FALSE )
			{
				lpObj->m_iPeriodItemEffectIndex = iIndex;
				this->ItemPeriodInfo[iIndex].btUsedInfo = TRUE;
				this->ItemPeriodInfo[iIndex].iUserGuid = lpObj->DBNumber;
				this->ItemPeriodInfo[iIndex].wdUserIndex = lpObj->m_Index;
				this->ItemPeriodInfo[iIndex].dwItemCheckTickCount = GetTickCount();
				this->ItemPeriodInfo[iIndex].btCountItems = 1;
				memcpy(this->ItemPeriodInfo[iIndex].chCharacterName,lpObj->Name,11);
				this->ItemPeriodInfo[iIndex].vItemEffect[0].bInUse = true;
				this->ItemPeriodInfo[iIndex].vItemEffect[0].iItemCode = iItemCode;
				this->ItemPeriodInfo[iIndex].vItemEffect[0].btEffectType1 = btEffectType1;
				this->ItemPeriodInfo[iIndex].vItemEffect[0].btEffectType2 = btEffectType2;
				this->ItemPeriodInfo[iIndex].vItemEffect[0].btOptionType = iOptionEffect;
				this->ItemPeriodInfo[iIndex].vItemEffect[0].iItemPeriodLeftTime = dwItemPeriod;
				this->ItemPeriodInfo[iIndex].vItemEffect[0].iItemPeriodUsedTime = 0;



				this->ApplyItemPeriodEffect(lpObj,iItemCode,dwItemPeriod);
				return TRUE;
			}
		}
	}
	return FALSE;
}

//5C95A0
BOOL CCashItemPeriodSystem::ClearPeriodItemEffect(LPOBJ lpObj, int ItemCode)//Identical
{
	int iItemInfoIndex =0 ;
	iItemInfoIndex = lpObj->m_iPeriodItemEffectIndex;

	if ( iItemInfoIndex < 0 || iItemInfoIndex > MAX_ITEM_PERIOD_INFO )
		return FALSE;

	if ( this->ItemPeriodInfo[iItemInfoIndex].btUsedInfo != TRUE )
	{
		if ( this->ItemPeriodInfo[iItemInfoIndex].wdUserIndex != lpObj->m_Index )
		{
			return FALSE;
		}
	}

	this->ItemPeriodInfo[iItemInfoIndex].btCountItems--;

	if( this->ItemPeriodInfo[iItemInfoIndex].btCountItems == 0 || ItemCode == -1 )
	{
		this->ItemPeriodInfo[iItemInfoIndex].btUsedInfo = FALSE;
		this->ItemPeriodInfo[iItemInfoIndex].wdUserIndex = 0;
		this->ItemPeriodInfo[iItemInfoIndex].iUserGuid = 0;
		this->ItemPeriodInfo[iItemInfoIndex].btCountItems = 0;
		this->ItemPeriodInfo[iItemInfoIndex].dwItemCheckTickCount = 0;
		this->ItemPeriodInfo[iItemInfoIndex].chCharacterName[0] = NULL;

		for( int iOptionIndex = 0; iOptionIndex < MAX_ITEM_PERIOD_EFFECT; iOptionIndex++ )
		{
			g_ItemAddOption.ClearItemEffect2(lpObj,this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemCode,2);
			this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].bInUse = false;
			this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemCode = 0;
			this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].btOptionType = 0;
			this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].btEffectType1 = 0;
			this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].btEffectType2 = 0;
			this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemPeriodLeftTime = 0;
			this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemPeriodUsedTime = 0;
		}
		lpObj->m_iPeriodItemEffectIndex = -1;
	}
	else
	{
		int iOptionIndex = 0;

		for( iOptionIndex = 0; iOptionIndex < MAX_ITEM_PERIOD_EFFECT; iOptionIndex++ )
		{
			if( this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemCode == ItemCode )
			{
				g_ItemAddOption.ClearItemEffect2(lpObj,this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemCode,2);
				this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].bInUse = false;
				this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemCode = 0;
				this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].btOptionType = 0;
				this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].btEffectType1 = 0;
				this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].btEffectType2 = 0;
				this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemPeriodLeftTime = 0;
				this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemPeriodUsedTime = 0;
			}
		}
	}

	int iEffectIndex = 0;

	iEffectIndex = this->GetItemPeriodInfoEffectID(ItemCode);

	switch(iEffectIndex) //Leaps
	{
	case AT_CSHOP_LEAP_OF_STRENGTH:
	case AT_CSHOP_LEAP_OF_QUICKNESS:
	case AT_CSHOP_LEAP_OF_HEALTH:
	case AT_CSHOP_LEAP_OF_ENERGY:
	case AT_CSHOP_LEAP_OF_CONTROL:
		gObjCalCharacter(lpObj->m_Index);
		break;
	}

	return TRUE;
}

//5C9A60
void CCashItemPeriodSystem::GDReqPeriodItemInsert(LPOBJ lpObj, int iItemCode, int iItemEffectDate)//Identical
{
	PMSG_REQ_PERIODITEM_INSERT pMsg;
	int iEffectType1 =0;
	int iEffectType2 =0;
	CTime tExpireDate;
	int iEffectTime = 0;

	if( g_ItemAddOption.SearchItemEffectType(iItemCode, &iEffectType1, &iEffectType2, &iEffectTime) == false )
	{
		return;
	}

	if( iItemEffectDate == -1 )
	{
		iItemEffectDate = iEffectTime / 60;
	}

	int iOptionType = g_ItemAddOption.SearchItemOptionType(iItemCode);

	if( iOptionType == -1 )
	{
		return;
	}

	pMsg.btOptionType = iOptionType;
	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid = lpObj->DBNumber;
	pMsg.iItemCode = iItemCode;
	pMsg.btItemEffectType1 = iEffectType1;
	pMsg.btItemEffectType2 = iEffectType2;
	pMsg.iItemPeriodDate = iItemEffectDate;
	memcpy(pMsg.chUserID, lpObj->Name, sizeof(pMsg.chUserID));

	tExpireDate = this->CalcItemUseEndTime(iItemEffectDate);

	wsprintf(pMsg.chExpireDate, "%d-%d-%d %d:%d", tExpireDate.GetYear(), tExpireDate.GetMonth(), tExpireDate.GetDay(), tExpireDate.GetHour(), tExpireDate.GetMinute());

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x03, sizeof(PMSG_REQ_PERIODITEM_INSERT));

	cDBSMng.Send((char *)&pMsg, sizeof(PMSG_REQ_PERIODITEM_INSERT));

	LogAddTD("[CashShop][PeriodItemInsert Req] - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d, ExpireDate : %s",
		lpObj->AccountID, lpObj->Name, pMsg.iItemCode, pMsg.btItemEffectType1, pMsg.btItemEffectType2, pMsg.iItemPeriodDate, pMsg.chExpireDate);
}

void CCashItemPeriodSystem::GDReqPeriodItemList(LPOBJ lpObj)
{
	PMSG_REQ_PERIODITEM_LIST pMsg;

	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid = lpObj->DBNumber;
	memcpy(pMsg.chCharacterName, lpObj->Name, sizeof(pMsg.chCharacterName));

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x05, sizeof(PMSG_REQ_PERIODITEM_LIST));

	cDBSMng.Send((char *)&pMsg, sizeof(PMSG_REQ_PERIODITEM_LIST));
}

void CCashItemPeriodSystem::GDReqPeriodItemInquiry(LPOBJ lpObj)
{
	PMSG_REQ_PERIODITEM_INQUIRY pMsg;

	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid = lpObj->DBNumber;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x01, sizeof(PMSG_REQ_PERIODITEM_INQUIRY));

	cDBSMng.Send((char *)&pMsg, sizeof(PMSG_REQ_PERIODITEM_INQUIRY));
}

void CCashItemPeriodSystem::GDReqPeriodItemUpdate(LPOBJ lpObj, int iOptionIndex)
{
	PMSG_REQ_PERIODITEM_UPDATE pMsg;
	ITEMPERIOD_INFO* lpItemInfo = NULL;

	if ( lpObj->m_iPeriodItemEffectIndex < 0 || lpObj->m_iPeriodItemEffectIndex >=MAX_ITEM_PERIOD_INFO )
		return;

	if( iOptionIndex < 0 || iOptionIndex > MAX_ITEM_PERIOD_EFFECT )
		return;

	lpItemInfo = &this->ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex];

	if( lpItemInfo->btUsedInfo == FALSE )
		return;

	pMsg.iUserGuid = lpObj->DBNumber;
	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iItemCode = lpItemInfo->vItemEffect[iOptionIndex].iItemCode;
	pMsg.iLeftTime = lpItemInfo->vItemEffect[iOptionIndex].iItemPeriodLeftTime;
	pMsg.iUsedTime = lpItemInfo->vItemEffect[iOptionIndex].iItemPeriodUsedTime;
	memcpy(pMsg.chCharacterName, lpItemInfo->chCharacterName, sizeof(pMsg.chCharacterName));

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x07, sizeof(PMSG_REQ_PERIODITEM_UPDATE));

	cDBSMng.Send((char *)&pMsg, sizeof(PMSG_REQ_PERIODITEM_UPDATE));
}


void CCashItemPeriodSystem::DGAnsPeriodItemInsert(PMSG_ANS_PERIODITEM_INSERT * aRecv)
{
	int iIndex = aRecv->iUserIndex;
	LPOBJ lpObj = &gObj[iIndex];
	BYTE btResult = FALSE;

	switch ( aRecv->btResult )
	{
		case 0: btResult = 1; break;
		case 1: btResult = 1; break;
		case 2: btResult = 3; break;
		default: btResult = 1;
	}

	if ( btResult == 1 )
	{
		this->SetPeriodItemEffect(lpObj, aRecv->iItemCode, aRecv->btItemEffectType1, aRecv->btItemEffectType2, aRecv->iItemPeriodDate);

		LogAddTD("[CashShop][PeriodItemInsert Ans] Success - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
			lpObj->AccountID, lpObj->Name, aRecv->iItemCode, aRecv->btItemEffectType1, aRecv->btItemEffectType2, aRecv->iItemPeriodDate);
	}
	else
	{
		LogAddTD("[CashShop][PeriodItemInserver Ans] Fail - ID : %s, Name : %s, ItemCode : %d, Effect1 : %d, Effect2 : %d, UseTime : %d, Result : %d",
			lpObj->AccountID, lpObj->Name, aRecv->iItemCode, aRecv->btItemEffectType1, aRecv->btItemEffectType2, aRecv->iItemPeriodDate, btResult);	
	}

	this->GCSendPeriodItemInserResult(lpObj, btResult);
}

void CCashItemPeriodSystem::DGAnsPeriodItemList(PMSG_ANS_PERIODITEM_LIST* aRecv)
{
	if ( aRecv->iUserIndex <= 0 || aRecv->iUserIndex > OBJMAX )
		return;

	LPOBJ lpObj = &gObj[aRecv->iUserIndex];
	int iItemLeftUseTime = 0;
	int iResult = 0;
	iResult = aRecv->btResult;

	if ( lpObj->DBNumber != aRecv->iUserGuid )
		return;

	if ( iResult == -1 )
	{
		LogAddTD("[CashShop][PeriodItemSelect Ans] Failed - Item over 1 item ID : %s, Name : %s",
			lpObj->AccountID, lpObj->Name);
	}
	else if ( iResult == -2 )
	{
		LogAddTD("[CashShop][PeriodItemSelect Ans] Failed - Item usedtime is over ID : %s, Name : %s",
			lpObj->AccountID, lpObj->Name);
	}
	
	int iCountItems = aRecv->btCountItems;

	for( int iOptionIndex = 0; iOptionIndex < iCountItems; iOptionIndex++ )
	{
		iItemLeftUseTime = aRecv->m_ListItem[iOptionIndex].iItemPeriodLeftTime;
		if ( iItemLeftUseTime > 0 )
		{
			this->SetPeriodItemEffect(lpObj, aRecv->m_ListItem[iOptionIndex].iItemCode, aRecv->m_ListItem[iOptionIndex].btEffectType1, aRecv->m_ListItem[iOptionIndex].btEffectType2, iItemLeftUseTime);
			LogAddTD("[CashShop][PeriodItemList Ans] Success - ID : %s, Name : %s, ItemCode : %d, OptionType : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
			lpObj->AccountID, lpObj->Name, aRecv->m_ListItem[iOptionIndex].iItemCode, aRecv->m_ListItem[iOptionIndex].btOptionType, aRecv->m_ListItem[iOptionIndex].btEffectType1, aRecv->m_ListItem[iOptionIndex].btEffectType2, iItemLeftUseTime);
		}
		else
		{
			LogAddTD("[CashShop][PeriodItemList Ans] Fail - ID : %s, Name : %s, ItemCode : %d, OptionType : %d, Effect1 : %d, Effect2 : %d, UseTime : %d",
			lpObj->AccountID, lpObj->Name, aRecv->m_ListItem[iOptionIndex].iItemCode, aRecv->m_ListItem[iOptionIndex].btOptionType, aRecv->m_ListItem[iOptionIndex].btEffectType1, aRecv->m_ListItem[iOptionIndex].btEffectType2, iItemLeftUseTime);
		}
	}
}

void CCashItemPeriodSystem::DGAnsPeriodItemInquiry(PMSG_ANS_PERIODITEM_INQUIRY *aRecv)
{
	return;
}

void CCashItemPeriodSystem::DGAnsPeriodItemUpdate(PMSG_ANS_PERIODITEM_UPDATE *aRecv)
{
	int iUserIndex = aRecv->iUserIndex;

	if ( iUserIndex < 0 || iUserIndex > OBJMAX )
		return;

	LPOBJ lpObj = &gObj[iUserIndex];
	ITEMPERIOD_INFO* lpItemInfo = NULL;

	if ( lpObj->m_iPeriodItemEffectIndex < 0 || lpObj->m_iPeriodItemEffectIndex >=MAX_ITEM_PERIOD_INFO )
		return;

	lpItemInfo = &this->ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex];

	if ( lpObj->m_iPeriodItemEffectIndex != lpItemInfo->wdUserIndex )
		return;

	switch ( aRecv->btResult )
	{
		case 3:
			if ( this->ClearPeriodItemEffect(lpObj, aRecv->iItemCode) == TRUE )
			{		
				LogAddTD("[CashShop][PeriodItemUpdate Ans] Remove Item Effect - ID : %s, Name : %s, ItemCode : %d, Result : %d", lpObj->AccountID, lpObj->Name, aRecv->iItemCode, aRecv->btResult);
			}
	}

	LogAddTD("[CashShop][PeriodItemUpdate Ans] - ID : %s, Name : %s, ItemCode : %d, Result %d",
		lpObj->AccountID, lpObj->Name, aRecv->iItemCode, aRecv->btResult);
}

void CCashItemPeriodSystem::GDReqPeriodItemDelete(LPOBJ lpObj, LPSTR pchCharacterName)
{
	PMSG_REQ_PERIODITEM_DELETE pMsg;
	ITEMPERIOD_INFO* lpItemInfo = NULL;
	
	pMsg.iUserIndex = lpObj->m_Index;
	pMsg.iUserGuid = lpObj->DBNumber;
	memcpy(pMsg.chCharacterName, pchCharacterName, sizeof(pMsg.chCharacterName));

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x09, sizeof(PMSG_REQ_PERIODITEM_DELETE));

	cDBSMng.Send((char*)&pMsg, sizeof(PMSG_REQ_PERIODITEM_DELETE));
}

void CCashItemPeriodSystem::DGAnsPeriodItemDelete(PMSG_ANS_PERIODITEM_DELETE *aRecv)
{
	LPOBJ lpObj = NULL;

	if ( aRecv->iUserIndex < 0 || aRecv->iUserIndex > OBJMAX )
		return;

	lpObj = &gObj[aRecv->iUserGuid];

	if ( lpObj->DBNumber != aRecv->iUserGuid )
		return;

	switch ( aRecv->btResult )
	{
		case 0:
			LogAdd( "[CashShop][PeriodItemDelete Ans] - ID : %s, Name : %s", lpObj->AccountID, lpObj->Name );
			break;
	}
}

void CCashItemPeriodSystem::GCSendPeriodItemInserResult(LPOBJ lpObj, BYTE btResult)
{
	PMSG_ANS_PERIODITEM_INSERT_RESULT pMsg;
	
	pMsg.btResult = btResult;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x03, sizeof(PMSG_ANS_PERIODITEM_INSERT_RESULT));

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg,	sizeof(PMSG_ANS_PERIODITEM_INSERT_RESULT));
}

bool CCashItemPeriodSystem::SearchAndDeleteItemPeriodEffect(LPOBJ lpObj, int iItemCode)
{
	int iItemInfoIndex = lpObj->m_iPeriodItemEffectIndex;
	int iOptionType = 0;

	iOptionType = g_ItemAddOption.SearchItemOptionType( iItemCode );

	if( iItemInfoIndex == -1 )
		return true;

	if( this->ItemPeriodInfo[iItemInfoIndex].iUserGuid != lpObj->DBNumber )
		return false;

	for( int iOptionIndex = 0; iOptionIndex < MAX_ITEM_PERIOD_EFFECT; iOptionIndex++ )
	{
		if( this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].bInUse == true &&
			this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].btOptionType == iOptionType )
		{
			this->ClearPeriodItemEffect(lpObj,this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemCode);
			return false;
		}
	}

	return true;
}

int CCashItemPeriodSystem::CheckHaveItemPeriodSealEffect(LPOBJ lpObj)
{
	int iItemInfoIndex = lpObj->m_iPeriodItemEffectIndex;

	if( iItemInfoIndex == -1 )
		return FALSE;

	if( this->ItemPeriodInfo[iItemInfoIndex].btUsedInfo == NULL )
		return FALSE;

	for( int iOptionIndex = 0; iOptionIndex < MAX_ITEM_PERIOD_EFFECT; iOptionIndex++ )
	{
		if( this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].bInUse == true )
		{
			switch(	this->ItemPeriodInfo[iItemInfoIndex].vItemEffect[iOptionIndex].iItemCode )
			{
				case ITEMGET(13,43)://Seal of Ascension
				case ITEMGET(13,44)://Seal of Wealth
				case ITEMGET(13,45)://Seal of Sustenance
				case ITEMGET(13,52)://Condor Flame lol!!!
				case ITEMGET(13,59)://Seal of Mobility
				case ITEMGET(13,62)://Seal of Healing
				case ITEMGET(13,63)://Seal of Divinity
				case ITEMGET(13,93)://Seal of Master Ascension
				case ITEMGET(13,94)://Seal of Master Wealth
					return TRUE;
			}
		}
	}

	return FALSE;
}

ITEMPERIOD_INFO* CCashItemPeriodSystem::GetItemPeriodInfo(LPOBJ lpObj)
{
	if( lpObj->m_iPeriodItemEffectIndex < 0 )
		return NULL;

	if( this->ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex].btUsedInfo == NULL )
		return NULL;

	return &this->ItemPeriodInfo[lpObj->m_iPeriodItemEffectIndex];
}

int CCashItemPeriodSystem::GetItemPeriodInfoEffectID(int iItemCode)
{
	int iValue = 0;

	switch(iItemCode)
	{
	case ITEMGET(13,43):
		iValue = AT_CSHOP_SEAL_OF_ASCENSION;
		break;
	case ITEMGET(13,44):
		iValue = AT_CSHOP_SEAL_OF_WEALTH;
		break;
	case ITEMGET(13,45):
		iValue = AT_CSHOP_SEAL_OF_SUSTENANCE;
		break;
	case ITEMGET(13,59):
		iValue = AT_CSHOP_SEAL_OF_MOBILITY;
		break;
	case ITEMGET(13,62): //Season 3.5
		iValue = AT_CSHOP_SEAL_OF_HEALING;
		break;
	case ITEMGET(13,63): //Season 3.5
		iValue = AT_CSHOP_SEAL_OF_DIVINITY;
		break;
	case ITEMGET(14,72):
		iValue = AT_CSHOP_SCROLL_OF_QUICKNESS;
		break;
	case ITEMGET(14,73):
		iValue = AT_CSHOP_SCROLL_OF_DEFENSE;
		break;
	case ITEMGET(14,74):
		iValue = AT_CSHOP_SCROLL_OF_WRATH;
		break;
	case ITEMGET(14,75):
		iValue = AT_CSHOP_SCROLL_OF_WIZARDRY;
		break;
	case ITEMGET(14,76):
		iValue = AT_CSHOP_SCROLL_OF_HEALTH;
		break;
	case ITEMGET(14,77):
		iValue = AT_CSHOP_SCROLL_OF_MANA;
		break;
	case ITEMGET(14,97): //Season 3.5
		iValue = AT_CSHOP_SCROLL_OF_BATTLE;
		break;
	case ITEMGET(14,98): //Season 3.5
		iValue = AT_CSHOP_SCROLL_OF_STRENGTHENER;
		break;
	case ITEMGET(14,78):
		iValue = AT_CSHOP_LEAP_OF_STRENGTH;
		break;
	case ITEMGET(14,79):
		iValue = AT_CSHOP_LEAP_OF_QUICKNESS;
		break;
	case ITEMGET(14,80):
		iValue = AT_CSHOP_LEAP_OF_HEALTH;
		break;
	case ITEMGET(14,81):
		iValue = AT_CSHOP_LEAP_OF_ENERGY;
		break;
	case ITEMGET(14,82):
		iValue = AT_CSHOP_LEAP_OF_CONTROL;
		break;
	case ITEMGET(13,81): //Season 4.5
		iValue = AT_CSHOP_GUARDIAN_AMULET;
		break;
	case ITEMGET(13,82): //Season 4.5
		iValue = AT_CSHOP_PROTECT_AMULET;
		break;
	case ITEMGET(13,93): //Season 4.5
		iValue = AT_CSHOP_MASTER_SEAL_OF_ASCENSION;
		break;
	case ITEMGET(13,94): //Season 4.5
		iValue = AT_CSHOP_MASTER_SEAL_OF_WEALTH;
		break;
	default:
		iValue = AT_NULL_BUFF_EFFECT;
		break;
	}

	return iValue;
}

BOOL CCashItemPeriodSystem::ApplyItemPeriodEffect(LPOBJ lpObj, int iItemCode, int iDuration)
{
	BOOL bResult = FALSE;
	int iEffectIndex = 0;

	iEffectIndex = this->GetItemPeriodInfoEffectID(iItemCode);
	
	if(iEffectIndex != 0)
	{
		int iUseTime = iDuration * 60;

		bResult = gObjAddBuffEffect(lpObj, iEffectIndex, iUseTime);

		switch(iEffectIndex) //Leaps
		{
		case AT_CSHOP_LEAP_OF_STRENGTH:
		case AT_CSHOP_LEAP_OF_QUICKNESS:
		case AT_CSHOP_LEAP_OF_HEALTH:
		case AT_CSHOP_LEAP_OF_ENERGY:
		case AT_CSHOP_LEAP_OF_CONTROL:
			gObjCalCharacter(lpObj->m_Index);
			break;
		}
	}
	else
	{
		bResult = g_ItemAddOption.SetItemEffect(lpObj, iItemCode, iDuration) == TRUE; //!( g_ItemAddOption.SetItemEffect(lpObj, iItemCode, iItemTime) - 1 );
	}

	return bResult;
}