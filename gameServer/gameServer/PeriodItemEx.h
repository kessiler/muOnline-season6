#pragma once

#include "user.h"

#define MAX_ITEMDATA	30

struct ITEMDATA
{
	void Clear()
	{
		bInUse = 0;
		wItemCode = 0;
		lCurentDate = 0;
	}

	BYTE bInUse;
	BYTE btType;
	WORD wItemCode;
	BYTE btTmp;
	BYTE btEffectType1;
	BYTE btEffectType2;
	DWORD dwSerial;
	long lCurentDate;
	long lExpireDate;
	int iUnk3;
	int iDuration;
};

struct PERIOD_DATA
{
	void Clear();
	BYTE		bInUse;		// 0
	BYTE		btType;		// 1
	short		aIndex;		// 2
	int			DBNumber;	// 4
	LPOBJ		lpObj;		// 8
	char		Name[11];	// C
	char		AccountID[11]; // 18	
	ITEMDATA	itemData[MAX_ITEMDATA];	// 24
};

struct PMSG_ANS_PERIODITEMEX_INSERT
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	BYTE btResultCode;
	BYTE btItemtype;
	WORD wItemCode;
	BYTE btOptionType;
	BYTE btEffectType1;
	BYTE btEffectType2;
	DWORD dwSerial;
	DWORD dwDuration;
	long lBuyDate;
	long lExpireDate;
};

struct PMSG_REQ_PERIODITEMEX_INSERT
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	char chCharacterName[11];
	BYTE btItemType;
	WORD wItemCode;
	BYTE btOptionType;
	BYTE btEffectType1;
	BYTE btEffectType2;
	DWORD dwSerial;
	DWORD dwDuration;
	long lBuyDate;
	long lExpireDate;
};

struct PMSG_ANS_PERIODITEMEX_DELETE
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	BYTE btResultCode;
};

struct PMSG_REQ_PERIODITEMEX_DELETE
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	DWORD dwSerial;
	WORD wItemCode;
	BYTE btItemType;
	char chCharacterName[11];
};

struct ITEMPERIOD_DATEINFO
{
	BYTE btUsedInfo;
	BYTE btItemType;
	WORD wItemCode;
	BYTE btEffectCategory;
	BYTE btEffectType1;
	BYTE btEffectType2;
	DWORD dwSerial;
	long lItemBuyDate;
	long lItemExpireDate;
};

typedef struct
{
	DWORD dwUserGuid;
	BYTE btItemCount;
	char chCharacterName[11];
	ITEMPERIOD_DATEINFO ItemData[30];
}PERIOD_ITEMLIST, *LPPERIOD_ITEMLIST;

struct PMSG_ANS_PERIODITEMEX_LIST
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	BYTE btUsedInfo;
	BYTE btItemType;
	DWORD dwSerial;
	long lItemBuyDate;
	long lItemExpireDate;
	WORD wItemCode;
	BYTE btEffectCategory;
	BYTE btEffectType1;
	BYTE btEffectType2;
};

struct PMSG_REQ_PERIODITEMEX_SELECT
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	char chCharacterName[11];
};

struct PMSG_ANS_PERIODITEMEX_LIST_COUNT
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	BYTE btResult;
	BYTE btListCount;
	DWORD dwUserGuid;
};

struct PERIOD_EXPIRED_ITEM
{
	WORD wItemCode;
	DWORD dwSerial;
};

typedef struct
{
	BYTE btItemCount;
	PERIOD_EXPIRED_ITEM ExpiredItem[30];
}PERIOD_EXPIRED_ITEMLIST, *LPPERIOD_EXPIRED_ITEMLIST;

struct PMSG_ANS_PERIODITEMEX_EXPIRED_ITEMLIST
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	BYTE btExpiredItemCount;
	WORD wItemCode[20];
	DWORD dwSerial[20];
};

//#pragma push(pack())
//#pragma pack(1)

struct PMSG_ANS_PERIODITEMEX_SELECT
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	BYTE btResultCode;
	PERIOD_ITEMLIST ItemList;
};

//#pragma pack(pop())

#pragma pack(push, 1)
struct PMSG_ANS_PERIODLIST
{
	PBMSG_HEAD2 h;
	WORD wItemCode;		//+4
	WORD wItemPos;		//+6
	long lExpireDate;	//+8
};
#pragma pack(pop)

class CPeriodItemEx
{
public:
	CPeriodItemEx();
	virtual ~CPeriodItemEx();

	void Initialize();
	int AddPeriodInfo(LPOBJ lpObj);
	bool RemovePeriodInfo(LPOBJ lpObj);
	bool IsCorrectUser(LPOBJ lpObj);
	int AddPeriodItemData(LPOBJ lpObj,WORD wItemCode,BYTE btEffectType1,BYTE btEffectType2,int iDuration,long lExpireDate);
	int AddPeriodItemData(LPOBJ lpObj,WORD wItemCode,DWORD dwSerial,int iDuration);
	int AddPeriodItemData(LPOBJ lpObj,WORD wItemCode,DWORD dwSerial,int iDuration,long lExpireDate);
	bool RemovePeriodItemData(LPOBJ lpObj,BYTE btItemType,WORD wItemCode,unsigned long dwSerial);
	bool RemovePeriodItemData(LPOBJ lpObj,WORD wItemCode,unsigned long dwSerial);
	void CheckPeriodData();
	long ConvertStringToTime(char const* pchTime);
	long GetCurrentDate();
	long GetExpireDate(int iDuration);
	bool CheckExpireDate(long dwItemExpireDate);
	unsigned long GetLeftDate(long lExpireDate);
	bool SetPeriodItemInfo(LPOBJ lpObj,WORD wItemCode,DWORD dwSerial,DWORD dwDuration);
	bool SetPeriodItemInfo(LPOBJ lpObj,BYTE btItemType,WORD wItemCode,DWORD dwSerial,DWORD dwDuration);
	bool SetPeriodItemInfo(LPOBJ lpObj,WORD wItemCode);
	bool SetDisableItemToExpire(LPOBJ lpObj,int iInventoryPosition);
	void SetExpireNotMatchedData(LPOBJ lpObj);
	void RequestPeriodItemInfo(LPOBJ lpObj);
	void RequestPeriodItemInsert(LPOBJ lpObj,ITEMDATA* lpItemData,DWORD dwDuration);
	void RequestPeriodItemDelete(LPOBJ lpObj,ITEMDATA* lpItemData);
	void RequestPeriodItemUpdate(LPOBJ lpObj,ITEMDATA* lpItemData);
	void DGPeriodItemExInsert(PMSG_ANS_PERIODITEMEX_INSERT* lpMsg);
	void DGPeriodItemExDelete(PMSG_ANS_PERIODITEMEX_DELETE* lpMsg);
	void DGPeriodItemExSelect(PMSG_ANS_PERIODITEMEX_SELECT* lpMsg);
	void DGPeriodItemExExpiredItemList(PMSG_ANS_PERIODITEMEX_EXPIRED_ITEMLIST * lpMsg);
	void DGPeriodItemExListCount(PMSG_ANS_PERIODITEMEX_LIST_COUNT* lpMsg);
	void DGPeriodItemExList(PMSG_ANS_PERIODITEMEX_LIST* lpMsg);
	int GetItemFromInventory(LPOBJ lpObj,WORD wItemCode,DWORD dwSerial);
	void OnRequestPeriodItemList(LPOBJ lpObj);
	void OnRequestPeriodItemListOnce(LPOBJ lpObj,ITEMDATA* lpItemData);
	void OnRequestPeriodExpiredItem(LPOBJ lpObj,ITEMDATA* lpItemData);
	bool SendPeriodItemInfoOnce(LPOBJ lpObj,CItem* lpItem);

public:
	PERIOD_DATA m_PeriodData[OBJMAXUSER];	// 4
};

extern CPeriodItemEx g_PeriodItemEx;
