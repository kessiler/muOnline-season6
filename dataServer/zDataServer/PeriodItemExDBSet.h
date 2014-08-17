// PeriodItemExDBSet.h: interface for the CPeriodItemExDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERIODITEMEXDBSET_H__F9CD413E_92D0_45D8_A67F_807C8282CA0F__INCLUDED_)
#define AFX_PERIODITEMEXDBSET_H__F9CD413E_92D0_45D8_A67F_807C8282CA0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprodef.h"

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
	char chCharacterName[MAX_IDSTRING+1];
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
	char chCharacterName[MAX_IDSTRING+1];
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
	char chCharacterName[MAX_IDSTRING+1];
	ITEMPERIOD_DATEINFO ItemData[100];
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
	char chCharacterName[MAX_IDSTRING+1];
};

struct PMSG_ANS_PERIODITEMEX_EXPIRED_ITEMLIST
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	BYTE btExpiredItemCount;
	WORD wItemCode[30];
	DWORD dwSerial[30];
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

class CPeriodItemExDBSet  
{
public:
	CPeriodItemExDBSet();
	virtual ~CPeriodItemExDBSet();

	BOOL Connect();
	int Insert(DWORD dwUserGUID, char* pchCharacterName, BYTE btItemType, WORD wItemCode, BYTE btEffectCategory, BYTE btEffectType1, BYTE btEffectType2, DWORD dwSerial, DWORD dwDuration, long lBuyDate, long lExpireDate);
	int Delete(DWORD dwUserGUID, char* pchCharacterName, WORD wItemCode, BYTE btItemType, DWORD dwSerial);
	int Select(LPPERIOD_ITEMLIST lpPeriodItemList);
	int Update();
	BOOL SelectExpiredItemList(DWORD dwUserGuid, char* pchCharacterName, LPPERIOD_EXPIRED_ITEMLIST lpExpiredItemList);

public:
	CQuery m_Query;
};

typedef struct
{
	WORD wItemCode;
	DWORD dwSerial;
	WORD wDurabilitySmall;
}PMSG_LUCKYITME_DB_INFO, *LPPMSG_LUCKYITME_DB_INFO;

struct PMSG_REQ_LUCKYITEM_INSERT
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	char szCharName[MAX_IDSTRING+1];
	PMSG_LUCKYITME_DB_INFO LuckyItemDBInfo;
};

struct PMSG_REQ_LUCKYITEM_INSERT_2ND
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	BYTE btItemCnt;
	char szCharName[MAX_IDSTRING+1];
	PMSG_LUCKYITME_DB_INFO LuckyItemDBInfo[5];
};

struct PMSG_REQ_LUCKYITEM_DELETE
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	char szCharName[MAX_IDSTRING+1];
	WORD wItemCode;
	DWORD dwSerial;
};

struct PMSG_ANS_LUCKYITEM_SELECT
{
	PWMSG_HEAD head;
	WORD wUserIndex;
	BYTE btResultCode;
	BYTE btItemCnt;
};

struct PMSG_REQ_LUCKYITEM_SELECT
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	char chCharacterName[MAX_IDSTRING+1];
};

class LuckyItemDBSet
{
public:
	LuckyItemDBSet();
	virtual ~LuckyItemDBSet();

	BOOL Connect();

	int Insert(DWORD dwUserGuid, char* szCharName, WORD wItemCode, DWORD dwSerial, WORD wDurabilitySmall);
	int Delete(char* szCharName, WORD wItemCode, DWORD dwSerial);
	int SelectLuckyItemList(char* szCharName, LPPMSG_LUCKYITME_DB_INFO pLuckyItemInfo);

public:
	CQuery m_Query;
 };

#endif // !defined(AFX_PERIODITEMEXDBSET_H__F9CD413E_92D0_45D8_A67F_807C8282CA0F__INCLUDED_)
