#include "StdAfx.h"
#include "PeriodItemEx.h"
#include "BuffEffectSlot.h"
#include "logproc.h"
#include "ObjUseSkill.h"
#include "ObjCalCharacter.h"
#include "ItemAddOption.h"
#include "DSProtocol.h"
#include "..\common\winutil.h"
#include "DBSockMng.h"

// Constructor / Destructor

CPeriodItemEx g_PeriodItemEx;

CPeriodItemEx::CPeriodItemEx()	//OK
{

}

CPeriodItemEx::~CPeriodItemEx()	//OK
{

}

//006bb120
void CPeriodItemEx::Initialize()	//OK
{
	for(int i = 0; i < OBJMAXUSER; i++)
		this->m_PeriodData[i].Clear();
}

//006bb180
void PERIOD_DATA::Clear()	//OK
{
	this->bInUse = 0;
	for(int i = 0; i < MAX_ITEMDATA; i++)
		this->itemData[i].Clear();
}

//006bb1e0
int CPeriodItemEx::AddPeriodInfo(LPOBJ lpObj)	//OK
{
	int iPeriodInfoIndex = -1;
	for(int i = 0; i < OBJMAXUSER; i++)
	{
		if( this->m_PeriodData[i].bInUse == 0 )
		{
			this->m_PeriodData[i].Clear();
			this->m_PeriodData[i].bInUse = TRUE;
			this->m_PeriodData[i].lpObj = lpObj;
			this->m_PeriodData[i].DBNumber = lpObj->DBNumber;
			this->m_PeriodData[i].aIndex = lpObj->m_Index;
			this->m_PeriodData[i].btType = 0;

			memcpy(this->m_PeriodData[i].Name,lpObj->Name,sizeof(this->m_PeriodData[i].Name));
			memcpy(this->m_PeriodData[i].AccountID,lpObj->AccountID,sizeof(this->m_PeriodData[i].AccountID));

			lpObj->m_PeriodDataIndex = i;
			iPeriodInfoIndex = i;
			break;
		}
	}
return iPeriodInfoIndex;
}

//006bb360
bool CPeriodItemEx::RemovePeriodInfo(LPOBJ lpObj)	//OK
{
	int iPeriodItemSlotIndex = lpObj->m_PeriodDataIndex;

	if( IsCorrectUser(lpObj) == true &&
		iPeriodItemSlotIndex >= 0 && iPeriodItemSlotIndex < 1000 )
	{
		if( this->m_PeriodData[iPeriodItemSlotIndex].bInUse == 1 )
			this->m_PeriodData[iPeriodItemSlotIndex].Clear();
	}
return false;
}

//006bb400
bool CPeriodItemEx::IsCorrectUser(LPOBJ lpObj)	//OK
{
	int iPeriodItemSlotIndex = lpObj->m_PeriodDataIndex;

	if( iPeriodItemSlotIndex < 0 )
		return false;

	if( this->m_PeriodData[iPeriodItemSlotIndex].bInUse == 1 &&
		this->m_PeriodData[iPeriodItemSlotIndex].DBNumber == lpObj->DBNumber )
		return true;

	return false;
}

//006bb480
int CPeriodItemEx::AddPeriodItemData(LPOBJ lpObj,WORD wItemCode,BYTE btEffectType1,BYTE btEffectType2,int iDuration,long lExpireDate)	//OK
{
	int iPeriodItemSlotIndex = lpObj->m_PeriodDataIndex;

	if( !IsCorrectUser(lpObj) )
		return -1;

	for(int i = 0; i < MAX_ITEMDATA; i++)
	{
		if( !this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].bInUse )
		{
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].bInUse = TRUE;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].btType = 1;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].wItemCode = wItemCode;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].btEffectType1 = btEffectType1;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].btEffectType2 = btEffectType2;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].lCurentDate = this->GetCurrentDate();
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].lExpireDate = lExpireDate;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].iDuration = iDuration;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].iUnk3 = 0;
			return i;
		}
	}

	return -1;
}

//006bb680
int CPeriodItemEx::AddPeriodItemData(LPOBJ lpObj,WORD wItemCode,DWORD dwSerial,int iDuration)	//OK
{
	int iPeriodItemSlotIndex = lpObj->m_PeriodDataIndex;
	long lExpireDate = 0;
	int iInventoryPosition = 0;

	if( !IsCorrectUser(lpObj) )
		return -1;

	for(int i = 0; i < MAX_ITEMDATA; i++)
	{
		if( !this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].bInUse )
		{
			lExpireDate = GetExpireDate(iDuration);
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].bInUse = TRUE;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].btType = 2;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].wItemCode = wItemCode;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].dwSerial = dwSerial;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].lCurentDate = this->GetCurrentDate();
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].lExpireDate = lExpireDate;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].iDuration = iDuration;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].iUnk3 = 0;

			iInventoryPosition = GetItemFromInventory(lpObj,wItemCode,dwSerial);

			if( iInventoryPosition == -1 )
				return -2;

			lpObj->pInventory[iInventoryPosition].SetPeriodItem();
			return i;
		}
	}

return -1;
}

//006bb8d0
int CPeriodItemEx::AddPeriodItemData(LPOBJ lpObj,WORD wItemCode,DWORD dwSerial,int iDuration,long lExpireDate)	//OK
{
	int iPeriodItemSlotIndex = lpObj->m_PeriodDataIndex;

	if( !IsCorrectUser(lpObj) )
		return -1;

	int iItemSlotIndex = 0;

	for(int i = 0; i < MAX_ITEMDATA; i++)
	{
		if( !this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].bInUse )
		{
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].bInUse = TRUE;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].btType = 2;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].wItemCode = wItemCode;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].dwSerial = dwSerial;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].lExpireDate = lExpireDate;
			this->m_PeriodData[iPeriodItemSlotIndex].itemData[i].iDuration = iDuration;

			iItemSlotIndex = GetItemFromInventory(lpObj,wItemCode,dwSerial);

			if( iItemSlotIndex != -1 )
			{
				lpObj->pInventory[iItemSlotIndex].SetPeriodItem();
			}

			return i;
		}
	}
	return -1;
}

//006bbab0
bool CPeriodItemEx::RemovePeriodItemData(LPOBJ lpObj, BYTE btItemType, WORD wItemCode, unsigned long dwSerial)	//OK
{
	if( !lpObj )
		return false;

	if( lpObj->Type != OBJ_USER || lpObj->Connected < 2 )
		return false;

	if( btItemType == 1 )
	{
		BUFF_EFFECT_DATE* lpBuffData = 0;
		lpBuffData = g_BuffEffectSlot.GetEffectDataFromItemCode(wItemCode);

		if( !lpBuffData )
			return false;

		gObjRemoveBuffEffect(lpObj,lpBuffData->btIndex);
	}
	else if( btItemType == 2 )
	{
		if( dwSerial == 0 )
			return false;

		int iInventoryPosition = 0;
		iInventoryPosition = this->GetItemFromInventory(lpObj,wItemCode,dwSerial);

		if( iInventoryPosition == -1 )
			return false;

		SetDisableItemToExpire(lpObj,iInventoryPosition);
		//auto-deleting
		gObjInventoryDeleteItem(lpObj->m_Index, iInventoryPosition);
		GCInventoryItemDeleteSend(lpObj->m_Index, iInventoryPosition, 1);
		gObjCalCharacter(lpObj->m_Index);
		// ----
		if( iInventoryPosition < 12 )
		{
			if( iInventoryPosition == 10 || iInventoryPosition == 11 )
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
			}
			// ----
			gObjMakePreviewCharSet(lpObj->m_Index);
			PMSG_USEREQUIPMENTCHANGED pChange;
			PHeadSetB((LPBYTE)&pChange, 0x25, sizeof(pChange));
			pChange.NumberH = SET_NUMBERH(lpObj->m_Index);
			pChange.NumberL = SET_NUMBERL(lpObj->m_Index);
			ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[iInventoryPosition]);
			pChange.ItemInfo[1] = iInventoryPosition <<  4;
			pChange.ItemInfo[1] |= LevelSmallConvert(lpObj->m_Index, iInventoryPosition) & 0x0F;
			DataSend(lpObj->m_Index, (LPBYTE)&pChange, pChange.h.size);
			MsgSendV2(lpObj, (LPBYTE)&pChange, pChange.h.size);
		}
	}
	else
	{
		return false;
	}


	ITEMDATA ItemData;
	ItemData.btType = btItemType;
	ItemData.wItemCode = wItemCode;
	ItemData.dwSerial = dwSerial;

	RequestPeriodItemDelete(lpObj,&ItemData);
	return true;
}

//006bbc00
bool CPeriodItemEx::RemovePeriodItemData(LPOBJ lpObj,WORD wItemCode,unsigned long dwSerial)	//OK
{
	int iInventoryPosition = 0;

	if( !lpObj )
		return false;

	if( lpObj->Type != OBJ_USER || lpObj->Connected < PLAYER_LOGGED )
		return false;

	iInventoryPosition = this->GetItemFromInventory(lpObj,wItemCode,dwSerial);

	if( iInventoryPosition == -1 )
		return false;
	//auto-deleting
	gObjInventoryDeleteItem(lpObj->m_Index, iInventoryPosition);
	GCInventoryItemDeleteSend(lpObj->m_Index, iInventoryPosition, 1);
	gObjCalCharacter(lpObj->m_Index);
	// ----
	this->SetDisableItemToExpire(lpObj,iInventoryPosition);
	return true;
}

//006bbca0
void CPeriodItemEx::CheckPeriodData()	//OK
{
	LPOBJ lpObj = 0;

	for(int i = 0; i < OBJMAXUSER; i++)
	{
		if( this->m_PeriodData[i].bInUse == true && this->m_PeriodData[i].lpObj != NULL )
		{
			lpObj = this->m_PeriodData[i].lpObj;

			if( lpObj->Type == OBJ_USER && lpObj->Connected >= PLAYER_LOGGED )
			{
				for(int iItemSlotIndex = 0; iItemSlotIndex < MAX_ITEMDATA; iItemSlotIndex++)
				{
					if( this->m_PeriodData[i].itemData[iItemSlotIndex].bInUse == true && this->CheckExpireDate(this->m_PeriodData[i].itemData[iItemSlotIndex].lExpireDate) == true )
					{
						LogAddTD("[PeriodItemEx][Remove PeriodItem] Expire PeriodItem Data. User Id : %s(%d), Name : %s, Item : %s(%d), Type : %d, Serial : %u, ExpireDate : %d",
							lpObj->AccountID,lpObj->DBNumber,lpObj->Name,ItemAttribute[this->m_PeriodData[i].itemData[iItemSlotIndex].wItemCode].Name,this->m_PeriodData[i].itemData[iItemSlotIndex].wItemCode,this->m_PeriodData[i].itemData[iItemSlotIndex].btType,this->m_PeriodData[i].itemData[iItemSlotIndex].dwSerial,this->m_PeriodData[i].itemData[iItemSlotIndex].lExpireDate);
						this->RemovePeriodItemData(lpObj,this->m_PeriodData[i].itemData[iItemSlotIndex].btType,this->m_PeriodData[i].itemData[iItemSlotIndex].wItemCode,this->m_PeriodData[i].itemData[iItemSlotIndex].dwSerial);
						this->m_PeriodData[i].itemData[iItemSlotIndex].Clear();
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						gObjCalCharacter(lpObj->m_Index);
					}
				}
			}
		}
	}
}

//006bbfb0
long CPeriodItemEx::ConvertStringToTime(char const* pchTime)	//OK
{
	int iYear = 0;
	int iMonth = 0;
	int iDay = 0;
	int iHour = 0;
	int iMinute = 0;
	int iDateLength = 0;

	CTime tCurrentTime(CTime::GetTickCount());
	CTime tExpireTime;
	CTimeSpan tSpanTime;

	long lReturnTime = 0;
	char chDate[20];

	char* Token;
	char seps[4] = "-: ";

	if( !pchTime )
		return 0;

	memcpy(chDate,pchTime,sizeof(chDate));
	iDateLength = strlen(chDate);

	if( iDateLength < 0 || iDateLength > 20 )
		return 0;

	Token = strtok(chDate,seps);
	iYear = atoi(Token);

	Token = strtok(0,seps);
	iMonth = atoi(Token);

	Token = strtok(0,seps);
	iDay = atoi(Token);

	Token = strtok(0,seps);
	iHour = atoi(Token);

	Token = strtok(0,seps);
	iMinute = atoi(Token);

	CTime tReturnTime(iYear,iMonth,iDay,iHour,iMinute,0,-1);
	tSpanTime = tReturnTime - tCurrentTime;
	lReturnTime = tSpanTime.GetTotalMinutes();
	return lReturnTime;
}

//006bc1e0
//gObjUseImprovePotion
long CPeriodItemEx::GetCurrentDate()	//~OK
{
	CTime tCurrentTime(CTime::GetTickCount());
	long tCurrent = tCurrentTime.GetTime();
	return tCurrent;
}

//006bc220
//gObjUseImprovePotion
long CPeriodItemEx::GetExpireDate(int iDuration)	//~OK
{
	CTime tExpireTime;
	CTime tCurrentTime(CTime::GetTickCount());
	CTimeSpan tLeftTime(0,0,0,iDuration);

	tExpireTime = tCurrentTime + tLeftTime;
	long tReturnTime = tExpireTime.GetTime();
	return tReturnTime;
}

//006bc2b0
bool CPeriodItemEx::CheckExpireDate(long dwItemExpireDate)	//OK
{
	if( this->GetCurrentDate() > dwItemExpireDate )
		return true;
	return false;
}

//006bc300
unsigned long CPeriodItemEx::GetLeftDate(long lExpireDate)	//OK
{
	unsigned long dwReturnValue = 0;
	CTime tExpireTime(lExpireDate);
	CTime tCurrentTime(CTime::GetTickCount());
	CTimeSpan tSpanTime;
	tSpanTime = tExpireTime - tCurrentTime;
	dwReturnValue = tSpanTime.GetTotalSeconds();
	return dwReturnValue;
}



//006bc390
//CGUseItemRecv
//CGUseItemRecv
bool CPeriodItemEx::SetPeriodItemInfo(LPOBJ lpObj,WORD wItemCode,DWORD dwSerial,DWORD dwDuration)	//~OK
{
	if( !lpObj )
		return false;

	if( lpObj->Type != OBJ_USER || lpObj->Connected < PLAYER_LOGGED )
		return false;

	int iInventoryPosition = 0;
	DWORD dwExpireDate = 0;
	CItem* lpItem = NULL;
	ITEMDATA ItemData;
	LPBUFF_EFFECT_DATE lpBuffData = NULL;
	LPITEMEFFECT lpItemEffect = NULL;

	if( !dwSerial )
	{
		lpBuffData		= g_BuffEffectSlot.GetEffectDataFromItemCode(wItemCode);
		lpItemEffect	= g_ItemAddOption.GetItemEffectData(wItemCode);

		if( !lpBuffData || !lpItemEffect )
			return false;

		if( gObjAddBuffEffect(lpObj,lpBuffData->btIndex,dwDuration) == TRUE )
		{
			ItemData.Clear();
			ItemData.btType = 1;
			ItemData.wItemCode = wItemCode;
			ItemData.btTmp = lpBuffData->btViewPortIndex;
			ItemData.btEffectType1 = lpItemEffect->wEffectType1;
			ItemData.btEffectType2 = lpItemEffect->wEffectType2;
			ItemData.iDuration = dwDuration;
			ItemData.iUnk3 = 0;
			ItemData.lCurentDate = this->GetCurrentDate();
			ItemData.lExpireDate = this->GetExpireDate(dwDuration);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		iInventoryPosition = this->GetItemFromInventory(lpObj,wItemCode,dwSerial);

		if( iInventoryPosition == -1 )
		{
			return false;
		}

		lpItem = &lpObj->pInventory[iInventoryPosition];
		lpItem->SetPeriodItem();

		GCInventoryItemOneSend(lpObj->m_Index, iInventoryPosition);

		dwExpireDate = this->GetExpireDate(dwDuration);
		AddPeriodItemData(lpObj,wItemCode,dwSerial,dwDuration,dwExpireDate);
		ItemData.Clear();
		ItemData.wItemCode		= wItemCode;
		ItemData.btType			= 2;
		ItemData.dwSerial		= lpItem->GetNumber();
		ItemData.iDuration		= dwDuration;
		ItemData.iUnk3			= 0;
		ItemData.lCurentDate	= this->GetCurrentDate();
		ItemData.lExpireDate	= dwExpireDate;
	}
	RequestPeriodItemInsert(lpObj,&ItemData,dwDuration);
	return true;
}

//006bc600
bool CPeriodItemEx::SetPeriodItemInfo(LPOBJ lpObj,BYTE btItemType,WORD wItemCode,DWORD dwSerial,DWORD dwDuration)	//OK
{
	if( !lpObj )
		return false;

	if( lpObj->Type != OBJ_USER ||
		lpObj->Connected < PLAYER_LOGGED )
		return false;

	ITEMDATA ItemData;
	
	if( btItemType == 1 )
	{
		LPBUFF_EFFECT_DATE lpBuffData = NULL;
		LPITEMEFFECT lpItemEffect = NULL;

		lpBuffData = g_BuffEffectSlot.GetEffectDataFromItemCode(wItemCode);
		lpItemEffect = g_ItemAddOption.GetItemEffectData(wItemCode);

		if( !lpBuffData || !lpItemEffect )
			return false;

		ItemData.Clear();
		ItemData.btType = 1;
		ItemData.wItemCode = wItemCode;
		ItemData.btTmp = lpBuffData->btViewPortIndex;
		ItemData.btEffectType1 = lpItemEffect->wEffectType1;
		ItemData.btEffectType2 = lpItemEffect->wEffectType2;
		ItemData.iDuration = dwDuration;
		ItemData.iUnk3 = 0;
		ItemData.lCurentDate = this->GetCurrentDate();
		ItemData.lExpireDate = this->GetExpireDate(dwDuration);
	}
	else if( btItemType == 2 )
	{
		if( dwSerial == 0 )
			return false;

		int iInventoryPosition = 0;

		iInventoryPosition = this->GetItemFromInventory(lpObj,wItemCode,dwSerial);

		if( iInventoryPosition == -1 )
			return false;

		//lpObj->pInventory[iInventoryPosition].SetPeriodItem(); //not original, fix

		ItemData.Clear();
		ItemData.wItemCode = wItemCode;
		ItemData.btType = 2;
		ItemData.dwSerial = dwSerial;
		ItemData.iDuration = dwDuration;
		ItemData.iUnk3 = 0;
		ItemData.lCurentDate = this->GetCurrentDate();
		ItemData.lExpireDate = this->GetExpireDate(dwDuration);
	}
	else
	{
		return false;
	}

	RequestPeriodItemInsert(lpObj,&ItemData,dwDuration);
return true;
}

//006bc800
bool CPeriodItemEx::SetPeriodItemInfo(LPOBJ lpObj,WORD wItemCode)	//OK
{
	if( !lpObj )
		return false;

	if( lpObj->Type != OBJ_USER ||
		lpObj->Connected < PLAYER_LOGGED )
		return false;

	DWORD dwExpireDate = 0;
	CItem* lpItem = NULL;
	ITEMDATA ItemData;
	LPBUFF_EFFECT_DATE lpBuffData = NULL;
	LPITEMEFFECT lpItemEffect = NULL;

	lpBuffData = g_BuffEffectSlot.GetEffectDataFromItemCode(wItemCode);
	lpItemEffect = g_ItemAddOption.GetItemEffectData(wItemCode);

	if( !lpBuffData || !lpItemEffect )
		return false;

	if( gObjAddBuffEffect(lpObj,lpBuffData->btIndex,lpItemEffect->iEffectValidTime) == TRUE )
	{
		ItemData.Clear();
		ItemData.btType = 1;
		ItemData.wItemCode = wItemCode;
		ItemData.btTmp = lpBuffData->btViewPortIndex;
		ItemData.btEffectType1 = lpItemEffect->wEffectType1;
		ItemData.btEffectType2 = lpItemEffect->wEffectType2;
		ItemData.iDuration = lpItemEffect->iEffectValidTime;
		ItemData.iUnk3 = 0;
		ItemData.lCurentDate = this->GetCurrentDate();
		ItemData.lExpireDate = this->GetExpireDate(lpItemEffect->iEffectValidTime);
	}
	else
	{
		return FALSE;
	}

	RequestPeriodItemInsert(lpObj,&ItemData,lpItemEffect->iEffectValidTime);
	return true;
}

//006bc990
bool CPeriodItemEx::SetDisableItemToExpire(LPOBJ lpObj,int iInventoryPosition)	//OK
{
	if( !lpObj )
		return false;

	if( lpObj->Connected < PLAYER_LOGGED ||
		lpObj->Type != OBJ_USER )
		return false;

	if( iInventoryPosition == -1 )
		return false;

	if( !lpObj->pInventory[iInventoryPosition].IsPeriodItem() )
		return false;

	lpObj->pInventory[iInventoryPosition].SetPeriodItemExpire();
	GCItemListSend(lpObj->m_Index);
	g_PeriodItemEx.OnRequestPeriodItemList(lpObj);

}

//006bca60
void CPeriodItemEx::SetExpireNotMatchedData(LPOBJ lpObj)	//OK
{
	BYTE bIsValidMatchedItem = 0;
	int iSetExpiredItemCount = 0;
	int i;
	int iPeriodItemData;

	for( i = 0; i < MAIN_INVENTORY_SIZE; i++)
	{
		if( !lpObj->pInventory[i].IsItem() )
			continue;

		if( lpObj->pInventory[i].IsPeriodItem() == TRUE )
		{
			if( !lpObj->pInventory[i].IsPeriodItemExpire() )
			{
				bIsValidMatchedItem = 0;

				for( iPeriodItemData = 0; iPeriodItemData < MAX_ITEMDATA; iPeriodItemData++)
				{
					if( !this->m_PeriodData[lpObj->m_PeriodDataIndex].itemData[iPeriodItemData].bInUse )
						continue;

					if( lpObj->pInventory[i].m_Type == this->m_PeriodData[lpObj->m_PeriodDataIndex].itemData[iPeriodItemData].wItemCode &&
						lpObj->pInventory[i].m_Number == this->m_PeriodData[lpObj->m_PeriodDataIndex].itemData[iPeriodItemData].dwSerial )
					{
						bIsValidMatchedItem = TRUE;
					}
				}

				if( !bIsValidMatchedItem )
				{
					lpObj->pInventory[i].SetPeriodItemExpire();
					iSetExpiredItemCount++;
				}
			}
		}
	}

	if( iSetExpiredItemCount > 0 )
		GCItemListSend(lpObj->m_Index);
}

//006bcc90
void CPeriodItemEx::RequestPeriodItemInfo(LPOBJ lpObj)	//OK
{
	PMSG_REQ_PERIODITEMEX_SELECT pMsg;
	pMsg.wUserIndex = lpObj->m_Index;
	pMsg.dwUserGuid = lpObj->DBNumber;
	memcpy(pMsg.chCharacterName,lpObj->Name,11);

	PHeadSubSetB((LPBYTE)&pMsg,0xD1,0x03,sizeof(pMsg));

	cDBSMng.Send((char*)&pMsg,sizeof(pMsg));
}

//006bcd10
//gObjUseImprovePotion
void CPeriodItemEx::RequestPeriodItemInsert(LPOBJ lpObj,ITEMDATA* lpItemData,DWORD dwDuration)	//~OK
{
	PMSG_REQ_PERIODITEMEX_INSERT pMsg;
	pMsg.wUserIndex = lpObj->m_Index;
	pMsg.dwUserGuid = lpObj->DBNumber;
	memcpy(pMsg.chCharacterName,lpObj->Name,11);

	pMsg.wItemCode = lpItemData->wItemCode;
	pMsg.btItemType = lpItemData->btType;
	pMsg.btEffectType1 = lpItemData->btEffectType1;
	pMsg.btEffectType2 = lpItemData->btEffectType2;
	pMsg.btOptionType = lpItemData->btTmp;
	pMsg.dwSerial = lpItemData->dwSerial;
	pMsg.dwDuration = dwDuration;
	pMsg.lBuyDate = this->GetCurrentDate();
	pMsg.lExpireDate = this->GetExpireDate(dwDuration);

	PHeadSubSetB((LPBYTE)&pMsg,0xD1,0x01,sizeof(pMsg));
	LogAddTD("[PeriodItemEx][Insert PeriodItem] Request Insert PeriodItem Data. User Id : %s(%d), Name : %s, Item : %s(%d), Type : %d, Serial : %u, Duration : %d",
		lpObj->AccountID,lpObj->DBNumber,lpObj->Name,ItemAttribute[lpItemData->wItemCode].Name,lpItemData->wItemCode,lpItemData->btType,lpItemData->dwSerial,dwDuration);

	cDBSMng.Send((char*)&pMsg,sizeof(pMsg));
}

//006bce80
void CPeriodItemEx::RequestPeriodItemDelete(LPOBJ lpObj,ITEMDATA* lpItemData)	//OK
{
	PMSG_REQ_PERIODITEMEX_DELETE pMsg;
	pMsg.wUserIndex = lpObj->m_Index;
	pMsg.dwUserGuid = lpObj->DBNumber;
	memcpy(pMsg.chCharacterName,lpObj->Name,11);

	pMsg.wItemCode = lpItemData->wItemCode;
	pMsg.btItemType = lpItemData->btType;
	pMsg.dwSerial = lpItemData->dwSerial;

	PHeadSubSetB((LPBYTE)&pMsg,0xD1,0x02,sizeof(pMsg));

	LogAddTD("[PeriodItemEx][Delete PeriodItem] Request Delete PeriodItem Data. User Id : %s(%d), Name : %s, Item : %s(%d), Type : %d, Serial : %u",
		lpObj->AccountID,lpObj->DBNumber,lpObj->Name,ItemAttribute[lpItemData->wItemCode].Name,lpItemData->wItemCode,lpItemData->btType,lpItemData->dwSerial);

	cDBSMng.Send((char*)&pMsg,sizeof(pMsg));
}

//006bcf90
void CPeriodItemEx::RequestPeriodItemUpdate(LPOBJ lpObj,ITEMDATA* lpItemData)	//OK
{
	// ----
}

//006bcfb0
void CPeriodItemEx::DGPeriodItemExInsert(PMSG_ANS_PERIODITEMEX_INSERT* lpMsg)	//OK
{
	int iIndex = lpMsg->wUserIndex;
	int iInsertedSlotIndex = 0;

	if( iIndex < 0 || iIndex >= OBJMAX )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj == NULL )
		return;

	if( lpObj->Type != OBJ_USER ||
		lpObj->Connected < PLAYER_LOGGED )
		return;


	switch( lpMsg->btResultCode )
	{
	case 0:
		if( lpMsg->btItemtype == 1 )
		{
			gObjAddBuffEffectForInGameShop(lpObj,lpMsg->wItemCode,lpMsg->dwDuration);
		}
		else if( lpMsg->btItemtype == 2 )
		{
			iInsertedSlotIndex = this->AddPeriodItemData(lpObj,lpMsg->wItemCode,lpMsg->dwSerial,lpMsg->dwDuration);
			OnRequestPeriodItemListOnce(lpObj,&this->m_PeriodData[lpObj->m_PeriodDataIndex].itemData[iInsertedSlotIndex]);
		}
		break;
	default:break;
	}

	LogAddTD("[PeriodItemEx][Insert PeriodItem] OnRequest Insert PeriodItem Data. User Id : %s(%d), Name : %s, Item : %s(%d), Type : %d, Serial : %u, Duration : %d, Result : %d",
		lpObj->AccountID,lpObj->DBNumber,lpObj->Name,ItemAttribute[lpMsg->wItemCode].Name,lpMsg->wItemCode,lpMsg->btItemtype,lpMsg->dwSerial,lpMsg->dwDuration,lpMsg->btResultCode);
}

//006bd190
void CPeriodItemEx::DGPeriodItemExDelete(PMSG_ANS_PERIODITEMEX_DELETE* lpMsg)	//OK
{
	int iIndex = lpMsg->wUserIndex;

	if( iIndex < 0 || iIndex >= OBJMAX )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj == NULL )
		return;

	if( lpObj->Type != OBJ_USER ||
		lpObj->Connected < PLAYER_LOGGED )
		return;

	if( lpMsg->btResultCode )
	{

	}
	else
	{
		LogAddTD("[PeriodItemEx][Delete PeriodItem] OnRequest Delete PeriodItem Data. User Id : %s(%d), Name : %s, Result : %d",
			lpObj->AccountID,lpObj->DBNumber,lpObj->Name,lpMsg->btResultCode);
	}
}

//006bd260
void CPeriodItemEx::DGPeriodItemExSelect(PMSG_ANS_PERIODITEMEX_SELECT* lpMsg)	//OK
{
	int iIndex = lpMsg->wUserIndex;

	if( iIndex < 0 || iIndex >= OBJMAX )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj == NULL )
		return;

	if( lpObj->Type != OBJ_USER ||
		lpObj->Connected < PLAYER_LOGGED )
		return;

	if( lpMsg->btResultCode > 0 )
	{
		PERIOD_ITEMLIST ItemList;
		memcpy(&ItemList,&lpMsg->ItemList,sizeof(ItemList));

		int iItemCount = ItemList.btItemCount;
		DWORD dwDuration;

		for(int i = 0; i < iItemCount; i++)
		{
			dwDuration = this->GetLeftDate(ItemList.ItemData[i].lItemExpireDate);

			if( ItemList.ItemData[i].btItemType == 1 )
			{
				gObjAddBuffEffectForInGameShop(lpObj,ItemList.ItemData[i].wItemCode,dwDuration);
			}
			else if( ItemList.ItemData[i].btItemType == 2 )
			{
				AddPeriodItemData(lpObj,ItemList.ItemData[i].wItemCode,ItemList.ItemData[i].dwSerial,dwDuration,ItemList.ItemData[i].lItemExpireDate);
			}
	
			LogAddTD("[PeriodItemEx][SelectItemList] User Id : %s(%d), Name : %s, Item Code : %d(%s), PeriodType : %d, Serial : %u, Duration : %d, ExpireDate : %d",
				lpObj->AccountID,lpObj->DBNumber,lpObj->Name,ItemList.ItemData[i].wItemCode,ItemAttribute[ItemList.ItemData[i].wItemCode].Name,ItemList.ItemData[i].btItemType,
				ItemList.ItemData[i].dwSerial,dwDuration,ItemList.ItemData[i].lItemExpireDate);
		}
		//GCItemListSend(lpObj->m_Index);//test
		OnRequestPeriodItemList(lpObj);
	}
}

//006bd550
void CPeriodItemEx::DGPeriodItemExExpiredItemList(PMSG_ANS_PERIODITEMEX_EXPIRED_ITEMLIST* lpMsg)	//OK
{
	int iIndex = lpMsg->wUserIndex;

	if( iIndex < 0 || iIndex >= OBJMAX )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj == NULL )
		return;

	if( lpObj->Type != OBJ_USER ||
		lpObj->Connected < PLAYER_LOGGED )
		return;

	int iInventoryPosition = 0;

	for(int i = 0; i < lpMsg->btExpiredItemCount; i++)
	{
		iInventoryPosition = this->GetItemFromInventory(lpObj,lpMsg->wItemCode[i],lpMsg->dwSerial[i]);

		if( iInventoryPosition != -1 )
		{
			LogAddTD("[PeriodItemEx][SetExpiredItemList] OnRequest Expired Item Set Disable. User Id : %s(%d), Name : %s, Item : %s(%d), Serial : %u",
				lpObj->AccountID,lpObj->DBNumber,lpObj->Name,ItemAttribute[lpMsg->wItemCode[i]].Name,lpMsg->wItemCode[i],lpMsg->dwSerial[i]);

			SetDisableItemToExpire(lpObj,iInventoryPosition);
		}
	}
}

//006bd6d0
void CPeriodItemEx::DGPeriodItemExListCount(PMSG_ANS_PERIODITEMEX_LIST_COUNT* lpMsg)	//OK
{
	int iIndex = lpMsg->wUserIndex;

	if( iIndex < 0 || iIndex >= OBJMAX )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj == NULL )
		return;

	if( lpObj->Type != OBJ_USER ||
		lpObj->Connected < PLAYER_LOGGED )
		return;

	if( lpObj->DBNumber != lpMsg->dwUserGuid )
		return;

	LogAddTD("[PeriodItemEx][SelectItemListCount] End PeriodItem List Receive User Id : %s(%d), Name : %s",
		lpObj->AccountID,lpObj->DBNumber,lpObj->Name);

	SetExpireNotMatchedData(lpObj);
	OnRequestPeriodItemList(lpObj);
}

//006bd7c0
void CPeriodItemEx::DGPeriodItemExList(PMSG_ANS_PERIODITEMEX_LIST* lpMsg)	//OK
{
	int iIndex = lpMsg->wUserIndex;

	if( iIndex < 0 || iIndex >= OBJMAX )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj == NULL )
		return;

	if( lpObj->Type != OBJ_USER ||
		lpObj->Connected < PLAYER_LOGGED )
		return;

	if( lpObj->DBNumber != lpMsg->dwUserGuid )
		return;

	int iItemCount = 0;
	DWORD dwDuration = 0;

	dwDuration = this->GetLeftDate(lpMsg->lItemExpireDate);

	if( lpMsg->btItemType == 1 )
	{
		if( !gObjAddBuffEffectForInGameShop(lpObj,lpMsg->wItemCode,dwDuration) )
		{
			LogAddTD("[PeriodItemEx][SelectItemList] Failed User Id : %s(%d), Name : %s, Item Code : %d(%s), PeriodType : %d, Serial : %u, Duration : %d, ExpireDate : %d",
				lpObj->AccountID,lpObj->DBNumber,lpObj->Name,lpMsg->wItemCode,ItemAttribute[lpMsg->wItemCode].Name,lpMsg->btItemType,lpMsg->dwSerial,dwDuration,lpMsg->lItemExpireDate);
		}
		else
		{
			LogAddTD("[PeriodItemEx][SelectItemList] User Id : %s(%d), Name : %s, Item Code : %d(%s), PeriodType : %d, Serial : %u, Duration : %d, ExpireDate : %d",
				lpObj->AccountID,lpObj->DBNumber,lpObj->Name,lpMsg->wItemCode,ItemAttribute[lpMsg->wItemCode].Name,lpMsg->btItemType,lpMsg->dwSerial,dwDuration,lpMsg->lItemExpireDate);
		}
	}
	else if( lpMsg->btItemType == 2 )
	{
		if( this->AddPeriodItemData(lpObj,lpMsg->wItemCode,lpMsg->dwSerial,dwDuration,lpMsg->lItemExpireDate) != -1 )
		{
			LogAddTD("[PeriodItemEx][SelectItemList] User Id : %s(%d), Name : %s, Item Code : %d(%s), PeriodType : %d, Serial : %u, Duration : %d, ExpireDate : %d",
				lpObj->AccountID,lpObj->DBNumber,lpObj->Name,lpMsg->wItemCode,ItemAttribute[lpMsg->wItemCode].Name,lpMsg->btItemType,lpMsg->dwSerial,dwDuration,lpMsg->lItemExpireDate);
		}
		else
		{
			LogAddTD("[PeriodItemEx][SelectItemList] Failed User Id : %s(%d), Name : %s, Item Code : %d(%s), PeriodType : %d, Serial : %u, Duration : %d, ExpireDate : %d",
				lpObj->AccountID,lpObj->DBNumber,lpObj->Name,lpMsg->wItemCode,ItemAttribute[lpMsg->wItemCode].Name,lpMsg->btItemType,lpMsg->dwSerial,dwDuration,lpMsg->lItemExpireDate);
		}
	}

}

//006bdaf0
int CPeriodItemEx::GetItemFromInventory(LPOBJ lpObj,WORD wItemCode,DWORD dwSerial)	//OK
{
	int iReturnIndex = -1;

	if( !lpObj )
		return -1;

	if( lpObj->Type != OBJ_USER ||
		lpObj->Connected < PLAYER_LOGGED )
		return -1;

	for(int i = 0; i < MAIN_INVENTORY_SIZE; i++)
	{
		if( lpObj->pInventory[i].IsItem() )
		{
			if( lpObj->pInventory[i].m_Type == wItemCode )
			{
				if( lpObj->pInventory[i].GetNumber() == dwSerial )
				{
					iReturnIndex = i;
					break;
				}
			}
		}
	}
	return iReturnIndex;
}

struct PMSG_ANS_PERIODLIST_COUNT
{
	PBMSG_HEAD2 h;
	BYTE btItemCount;
};

//006bdc00
//ManagementProc (x2)
//CGMMng::_ClearInven
void CPeriodItemEx::OnRequestPeriodItemList(LPOBJ lpObj)	//OK
{
	int iPeriodItemInfoSlotIndex = 0;
	PMSG_ANS_PERIODLIST_COUNT pCount = {0};
	PMSG_ANS_PERIODLIST pList = {0};

	WORD wItemCode = 0;
	DWORD dwSerial = 0;
	int iItemSlotIndex = 0;
	long lExpireDate = 0;

	iPeriodItemInfoSlotIndex = lpObj->m_PeriodDataIndex;

	PHeadSubSetB((LPBYTE)&pCount,0xD2,0x11,sizeof(pCount));
	PHeadSubSetB((LPBYTE)&pList,0xD2,0x12,sizeof(pList));

	pCount.btItemCount = this->m_PeriodData[iPeriodItemInfoSlotIndex].btType;
	DataSend(lpObj->m_Index,(LPBYTE)&pCount,pCount.h.size);

	if( this->m_PeriodData[iPeriodItemInfoSlotIndex].bInUse == 1 )
	{
		for(int i = 0; i < MAX_ITEMDATA; i++)
		{
			if( this->m_PeriodData[iPeriodItemInfoSlotIndex].itemData[i].bInUse == TRUE )
			{
				wItemCode = this->m_PeriodData[iPeriodItemInfoSlotIndex].itemData[i].wItemCode;
				dwSerial = this->m_PeriodData[iPeriodItemInfoSlotIndex].itemData[i].dwSerial;
				iItemSlotIndex = this->GetItemFromInventory(lpObj,wItemCode,dwSerial);
				lExpireDate = this->m_PeriodData[iPeriodItemInfoSlotIndex].itemData[i].lExpireDate;

				pList.wItemCode = wItemCode;
				pList.wItemPos = iItemSlotIndex;
				pList.lExpireDate = lExpireDate;

				DataSend(lpObj->m_Index,(LPBYTE)&pList,pList.h.size);
			}
		}
	}
}

//006bde30
void CPeriodItemEx::OnRequestPeriodItemListOnce(LPOBJ lpObj,ITEMDATA* lpItemData)	//OK
{
	if( !lpObj || !lpItemData )
		return;

	if( !lpItemData->bInUse )
		return;

	PMSG_ANS_PERIODLIST pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg,0xD2,0x12,sizeof(pMsg));
	
	WORD wItemCode = 0;
	DWORD dwSerial = 0;
	int iItemSlotIndex = 0;
	DWORD dwLeftTime = 0;

	wItemCode			= lpItemData->wItemCode;
	dwSerial			= lpItemData->dwSerial;
	iItemSlotIndex		= this->GetItemFromInventory(lpObj,wItemCode,dwSerial);

	pMsg.wItemCode		= wItemCode;
	pMsg.wItemPos		= iItemSlotIndex;
	pMsg.lExpireDate	= lpItemData->lExpireDate;

	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
}

//006bdf40
void CPeriodItemEx::OnRequestPeriodExpiredItem(LPOBJ lpObj,ITEMDATA* lpItemData)	//OK
{
	// not implemented
}

//006bdf60
bool CPeriodItemEx::SendPeriodItemInfoOnce(LPOBJ lpObj,CItem* lpItem)	//OK
{
	int iPeriodInfoIndex = 0;
	iPeriodInfoIndex = lpObj->m_PeriodDataIndex;

	if( this->m_PeriodData[iPeriodInfoIndex].bInUse == TRUE &&
		this->m_PeriodData[iPeriodInfoIndex].DBNumber == lpObj->DBNumber )
	{
		for(int i = 0; i < MAX_ITEMDATA; i++)
		{
			if( this->m_PeriodData[iPeriodInfoIndex].itemData[i].bInUse == TRUE )
			{
				if( this->m_PeriodData[iPeriodInfoIndex].itemData[i].dwSerial == lpItem->GetNumber() )
				{
					this->OnRequestPeriodItemListOnce(lpObj,&this->m_PeriodData[iPeriodInfoIndex].itemData[i]);
					return true;
				}
			}
		}
	}
	return false;
}