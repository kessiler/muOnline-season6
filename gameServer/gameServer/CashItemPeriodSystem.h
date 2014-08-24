// CashItemPeriodSystem.h: interface for the CCashItemPeriodSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASHITEMPERIODSYSTEM_H__11ADB3C6_2988_4060_913A_824DCFA381E8__INCLUDED_)
#define AFX_CASHITEMPERIODSYSTEM_H__11ADB3C6_2988_4060_913A_824DCFA381E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "user.h"

//#if( _GSCS )
	#define MAX_ITEM_PERIOD_INFO	1500
//#else
//	#define MAX_ITEM_PERIOD_INFO	1000
//#endif

#define	MAX_ITEM_PERIOD_EFFECT	10

struct PMSG_ANS_PERIODITEM_INSERT
{
	PBMSG_HEAD2 head;
	int iUserIndex;	// 4
	int iItemCode;	// 8
	int iItemPeriodDate;	// C
	BYTE btItemEffectType1;	// 10
	BYTE btItemEffectType2;	// 11
	BYTE btResult;	// 12
};


struct PMSG_ANS_PERIODITEM_INSERT_RESULT
{
	PBMSG_HEAD2 head;	// C1:D0:03
	BYTE btResult;	// 4
};

struct ITEM_USER_DATA
{
	bool bInUse;					// 0	|	18
	BYTE btOptionType;				// 1	|	19
	BYTE btEffectType1;				// 2	|	1A
	BYTE btEffectType2;				// 3	|	1B
	int iItemCode;					// 4    |	1C
	int iItemPeriodUsedTime;		// 8	|	20
	UINT iItemPeriodLeftTime;		// C	|	24
};/*0x10*/

struct ITEMPERIOD_INFO
{
	BYTE btUsedInfo;				// 0	|	8
	BYTE btCountItems;				// 1	|	9
	WORD wdUserIndex;				// 2	|	A
	int iUserGuid;					// 4	|	C
	DWORD dwItemCheckTickCount;		// 8	|	10
	char chCharacterName[11];		// C	|	14
	ITEM_USER_DATA	vItemEffect[MAX_ITEM_PERIOD_EFFECT]; // 18	|	1F
};/*0xB8*/

struct PMSG_ANS_PERIODITEM_DATA
{
	BYTE btInUse;					// 10
	int iItemCode;					// 14
	BYTE btOptionType;				// 18
	BYTE btEffectType1;				// 19
	BYTE btEffectType2;				// 1A
	int iItemPeriodUsedTime;		// 20
	UINT iItemPeriodLeftTime;		// 24
};/*0x14*/

struct PMSG_ANS_PERIODITEM_LIST//DiG FiX
{
	PBMSG_HEAD2 head;	//  0
	int iUserIndex;		//  4
	int iUserGuid;		//  8
	BYTE btCountItems;	//  C
	char btResult;		//  D
	PMSG_ANS_PERIODITEM_DATA	m_ListItem[MAX_ITEM_PERIOD_EFFECT];//10
};/*0xD8*/

struct PMSG_ANS_PERIODITEM_INQUIRY
{
	PBMSG_HEAD2 head;
	int iUserIndex;	// 4
	BYTE btResult;	// 8
};

struct PMSG_REQ_PERIODITEM_INSERT
{
	PBMSG_HEAD2 head;	// C1:D0:03
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	int iItemCode;	// C
	int iItemPeriodDate;	// 10
	BYTE btItemEffectType1;	// 14
	BYTE btItemEffectType2;	// 15
	char chUserID[11];	// 16
	char chExpireDate[20];	//21
	BYTE btOptionType;//DiG fix
};

struct PMSG_REQ_PERIODITEM_LIST
{
	PBMSG_HEAD2 head;	// C1:D0:05
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	char chCharacterName[11];	// C
};

struct PMSG_ANS_PERIODITEM_UPDATE
{
	PBMSG_HEAD2 head;
	int iUserIndex;	// 4
	int iItemCode;	// 8
	BYTE btResult;	// C
};


struct PMSG_ANS_PERIODITEM_DELETE
{
	PBMSG_HEAD2 head;
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	BYTE btResult;	// C
};

struct PMSG_REQ_PERIODITEM_INQUIRY
{
	PBMSG_HEAD2 head;	// C1:D0:01
	int iUserIndex;	// 4
	int iUserGuid;	// 8
};

struct PMSG_REQ_PERIODITEM_UPDATE
{
	PBMSG_HEAD2 head;	// C1:D0:07
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	int iItemCode;	// C
	int iUsedTime;	// 10
	int iLeftTime;	// 14
	char chCharacterName[11];	// 18
};

struct PMSG_REQ_PERIODITEM_DELETE
{
	PBMSG_HEAD2 head;	// C1:D0:09
	int iUserIndex;	// 4
	int iUserGuid;	// 8
	char chCharacterName[11];	// C
};

class CCashItemPeriodSystem
{

public:

	CCashItemPeriodSystem();
	virtual ~CCashItemPeriodSystem();

	void Initialize();
	void GetUserPeriodItemList(LPOBJ lpObj);
	int SetPeriodItemEffect(LPOBJ lpObj, int iItemCode,BYTE btEffectType1, BYTE btEffectType2, DWORD dwItemPeriod);
	int ClearPeriodItemEffect(LPOBJ lpObj, int ItemCode);
	void GDReqPeriodItemList(LPOBJ lpObj);
	void GDReqPeriodItemInsert(LPOBJ lpObj, int iItemCode, int iItemEffectDate);
	void GDReqPeriodItemInquiry(LPOBJ lpObj);
	void GDReqPeriodItemUpdate(LPOBJ lpObj, int iOptionIndex);
	void DGAnsPeriodItemList(PMSG_ANS_PERIODITEM_LIST* aRecv);
	void DGAnsPeriodItemInsert(PMSG_ANS_PERIODITEM_INSERT* aRecv);
	void DGAnsPeriodItemInquiry(PMSG_ANS_PERIODITEM_INQUIRY* aRecv);
	void DGAnsPeriodItemUpdate(PMSG_ANS_PERIODITEM_UPDATE* aRecv);
	void GDReqPeriodItemDelete(LPOBJ lpObj, LPSTR pchCharacterName);
	void DGAnsPeriodItemDelete(PMSG_ANS_PERIODITEM_DELETE* aRecv);
	void GCSendPeriodItemInserResult(LPOBJ lpObj, BYTE btResult);
	int CalcItemUseLeftTime(LPSTR pchBuyDate);
	class CTime CalcItemUseEndTime(int iLeftTime);
	void ConvertStringToTime(const char * pchDate,class CTime* lpTime);
	void ConvertTimeToString(const class CTime* lpTime, LPSTR pchDate);

	bool SearchAndDeleteItemPeriodEffect(LPOBJ lpObj, int iItemCode);
	int CheckHaveItemPeriodSealEffect(LPOBJ lpObj);
	ITEMPERIOD_INFO* GetItemPeriodInfo(LPOBJ lpObj);
	int GetItemPeriodInfoEffectID(int iItemCode);
	BOOL ApplyItemPeriodEffect(LPOBJ lpObj, int iItemCode, int iDuration);

	static DWORD WINAPI CCashItemPeriodSystem::PeriodCheckThread(void* lpPeriodInfo);

private:

	int iItemPeriodInfoCount;	// 4
	ITEMPERIOD_INFO ItemPeriodInfo[MAX_ITEM_PERIOD_INFO];	// 8
	HANDLE hThreadHandle;	// 130B8
};


extern CCashItemPeriodSystem g_CashItemPeriodSystem;

#endif // !defined(AFX_CASHITEMPERIODSYSTEM_H__11ADB3C6_2988_4060_913A_824DCFA381E8__INCLUDED_)
