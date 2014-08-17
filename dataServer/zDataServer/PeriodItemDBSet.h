// PeriodItemDBSet.h: interface for the CPeriodItemExDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERIODITEMDBSET_H__F9CD413E_92D0_45D8_A67F_807C8282CA0F__INCLUDED_)
#define AFX_PERIODITEMDBSET_H__F9CD413E_92D0_45D8_A67F_807C8282CA0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprodef.h"

struct PMSG_ANS_PERIODITEM_INSERT //ok
{
	PBMSG_HEAD2 head;
	int iUserIndex;
	int iItemCode;
	int iItemPeriodDate;
	BYTE btItemEffectType1;
	BYTE btItemEffectType2;
	BYTE btResult;
};

struct PMSG_REQ_PERIODITEM_INSERT //ok
{
	PBMSG_HEAD2 head;
	int iUserIndex;
	int iUserGuid;
	int iItemCode;
	int iItemPeriodDate;
	BYTE btItemEffectType1;
	BYTE btItemEffectType2;
	char chUserID[MAX_IDSTRING+1];
	char chExpireDate[20];
	BYTE btEffectCategory;
};

struct PMSG_ANS_PERIODITEM_DELETE //ok
{
	PBMSG_HEAD2 head;
	int iUserIndex;
	int iUserGuid;
	BYTE btCode;
};

struct PMSG_REQ_PERIODITEM_DELETE //ok
{
	PBMSG_HEAD2 head;
	int iUserIndex;
	int iUserGuid;
	char chCharacterName[MAX_IDSTRING+1];
};

struct ITEMPERIOD_DATEINFO_OLD //ok
{
	BYTE btUsedInfo;
	BYTE btEffectCategory;
	BYTE btEffectType1;
	BYTE btEffectType2;
	int iItemCode;
	DWORD dwItemUsedTime;
	DWORD dwItemLeftTime;
};

typedef struct //ok
{
	BYTE btUsedInfo;
	BYTE btDateInfoUseCount;
	WORD wUserIndex;
	int iUserGuid;
	DWORD dwItemCheckTickCount;
	char chCharacterName[MAX_IDSTRING+1];
	ITEMPERIOD_DATEINFO_OLD DateInfo[10];
}ITEMPERIOD_INFO, *LPITEMPERIOD_INFO;

struct PERIODITEM_DATA_EXT
{
	BYTE btUsedInfo;
	int iItemCode;
	BYTE btEffectCategory;
	BYTE btEffectType1;
	BYTE btEffectType2;
	DWORD dwItemUsedTime;
	DWORD dwItemLeftTime;
};

struct PMSG_ANS_PERIODITEM_LIST //ok
{
	PBMSG_HEAD2 head;
	int iUserIndex;
	int iUserGuid;
	BYTE btItemUseCount;
	char btResult;
	PERIODITEM_DATA_EXT Date[10];
};

struct PMSG_REQ_PERIODITEM_LIST //ok
{
	PBMSG_HEAD2 head;
	int iUserIndex;
	int iUserGuid;
	char chCharacterName[MAX_IDSTRING+1];
};

struct PMSG_REQ_PERIODITEM_UPDATE
{
	PBMSG_HEAD2 head;
	int iUserIndex;
	int iUserGuid;
	int iItemCode;
	int iUsedTime;
	int iLeftTime;
	char chCharacterName[MAX_IDSTRING+1];
};

struct PMSG_ANS_PERIODITEM_UPDATE
{
	PBMSG_HEAD2 head;
	int iUserIndex;
	int iItemCode;
	BYTE btResult;
};

class CPeriodItemDBSet  
{
public:
	CPeriodItemDBSet();
	virtual ~CPeriodItemDBSet();

	BOOL Connect();
	int Insert(int iUserGUID, char* pchCharacterName, int iItemCode, BYTE btEffectCategory, BYTE btEffectType1, BYTE btEffectType2, DWORD dwDuration, char * chExpireDate);
	int Delete(int iUserGUID, char* pchCharacterName);
	int Select(LPITEMPERIOD_INFO lpPeriodItemList);
	int Update(int iUserGUID, char* pchCharacterName, int iItemCode, DWORD dwDuration, int iLeftTime);

public:
	CQuery m_Query;
};

#endif // !defined(AFX_PERIODITEMDBSET_H__F9CD413E_92D0_45D8_A67F_807C8282CA0F__INCLUDED_)
