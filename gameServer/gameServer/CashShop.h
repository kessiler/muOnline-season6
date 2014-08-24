// CashShop.h: interface for the CCashShop class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#ifdef OLDCASHSHOP


#include "PCSProtocol.h"
#include "..\common\zzzitem.h"
#include "protocol.h"
#include "wsShopServerCli.h"


#define MAX_CASH_SHOP_CATEGORY	4
#define MAX_CASH_SHOP_ITEM		135
#define MAX_CASH_SHOP_PROTOCOL	15
#define MAX_CASH_SHOP_PROTOCOL_DATA	9
#define MAX_CASH_ITEM_BRANCH	100

#pragma pack(1)

struct CASHSHOP_ITEM_STATUS
{
	BYTE btItemType;	// 0
	WORD wItemIndex;	// 1
	BYTE btItemLevel;	// 3
	BYTE btDurability;	// 4
	BYTE btSkillOption;	// 5
	BYTE btLuckOption;	// 6
	BYTE btAddOption;	// 7
	BYTE btExOption;	// 8
	BYTE btItemInfo[MAX_ITEM_INFO];	// 9
};


struct CASHSHOP_ITEMLIST
{
	DWORD dwItemGuid;	// 0
	DWORD dwPriceGuid;	// 4
	BYTE btCategoryCode;	// 8
	BYTE btItemType;	// 9
	WORD wItemIndex;	// A
	BYTE btItemLevel;	// C
	BYTE btItemSkillOpion;	// D
	BYTE btItemLuckOption;	// E 
	BYTE btItemAddOption;	// F
	BYTE btItemExOption;	// 10
	BYTE btItemSaleRatio;	// 11
	WORD wItemPrice;	// 12
	BYTE btItemDuration;	// 14
	BYTE btSpecialOption;	// 15
	BYTE btItemUsedType;	// 16
	BYTE btItemInfo[MAX_ITEM_INFO];	// 17
	DWORD dwItemUsePeriod;	// 1E
	CItem ItemInfo;	// 22

	CASHSHOP_ITEMLIST(){};
};






struct PMSG_REQ_CASHSHOPOPEN
{
	PBMSG_HEAD2 head;
	BYTE btShopOpenType;	// 4
	BYTE btShopOpenAlready;	// 5
};


struct PMSG_REQ_CASHITEMLIST
{
	PBMSG_HEAD2 head;
	BYTE btCategoryIndex;	// 4
	BYTE btPageIndex;	// 5
	BYTE btPageLoaded;	// 6
};


struct PMSG_REQ_CASHITEM_BUY
{
	PBMSG_HEAD2 head;
	DWORD dwItemPriceGuid;	// 4
	BYTE btCategoryIndex;	// 8
};


struct CASHSHOP_ITEMLIST_TO_PROTOCOL
{
	DWORD dwItemGuid;	// 0
	BYTE btCategoryCode;	// 4
	BYTE btItemDuration;	// 5
	BYTE btItemSaleRatio;	// 6
	WORD wItemPrice;	// 7
	BYTE btSpecialOption;	// 9
	BYTE btItemInfo[MAX_ITEM_INFO];	// A
	DWORD dwItemUsePeriod;	// 11
};


struct PMSG_ANS_CASHITEMLIST
{
	PBMSG_HEAD2 head;	// C1:F5:06
	BYTE btCategoryIndex;	// 4
	BYTE btPageIndex;	// 5
	BYTE btItemCount;	//	// 6
	CASHSHOP_ITEMLIST_TO_PROTOCOL ItemInfo[MAX_CASH_SHOP_PROTOCOL_DATA];	// 7
};


struct CASHSHOP_BRANCH_ITEMLIST
{
	DWORD dwItemGuid;	// 0
	int iBranchType;	// 4
};


#pragma pack()

class CCashShop
{

public:

  	CCashShop();
	virtual ~CCashShop();

	void Initialize();
	void Load(LPSTR pchFilename);
	void LoadShopOption(LPSTR pchFilename);
	void LoadTestScript(LPSTR pchFilename);
	BOOL ConnectShopServer(LPSTR pchIpAddress, int iPortNumber);
	BOOL ReConnectShopServer();
	void CheckShopServerConnectState();
	void CashShopOptioNReload();
	BOOL InsertItemStatus(CASHSHOP_ITEM_STATUS* lpItemStatus);
	BOOL SetItemInfoFromShop(protocol::MSG_STOG_ITEM_LIST_ANS* lpMsg);
	BOOL SetPackageItemInfoFromShop(protocol::MSG_STOG_PACKAGE_LIST_ANS* lpMsg);
	void MakeItemList(CASHSHOP_ITEM_STATUS* lpItemStatus, sellItem* lpItemSellInfo);
	BOOL CheckValidItemInfo(sellItem* lpItemInfo);
	CASHSHOP_ITEM_STATUS* GetCashItemStatus(int iItemCode);
	BOOL IsGetSocketSeedFromShopItem(int iItemCode);
	BOOL IsGetAmountFromShopItem(int iItemCode);
	CASHSHOP_ITEMLIST* SearchItemList(int iItemGuid);
	BOOL AddUser(LPOBJ lpObj);
	BOOL DeleteUser(LPOBJ lpObj);
	LPOBJ SearchUser(int iUserGuid);
	BOOL CGCashShopOpen(LPOBJ lpObj, PMSG_REQ_CASHSHOPOPEN* lpMsg);
	BOOL CGCashPoint(LPOBJ lpObj);
	BOOL GCCashItemListSend(LPOBJ lpObj,  PMSG_REQ_CASHITEMLIST* lpMsg);
	void CGCashItemBuy(LPOBJ lpObj,  PMSG_REQ_CASHITEM_BUY* lpMsg);
	void GCCashPoint(LPOBJ lpObj, DWORD dwCashPoint);
	void GCCashItemBuyResult(LPOBJ lpObj, BYTE btResult);
	int CheckInventoryEmptySpace(LPOBJ lpObj, struct CASHSHOP_ITEMLIST* lpItemInfo);
	BOOL GiveBuyItemToInventory(LPOBJ lpObj, int iItemGuid);
	BOOL CheckPeriodItemUsed(LPOBJ lpObj, int iItemCode);
	BOOL CheckPeriodItem(int iItemCode);
	void GSReqCashPoint(DWORD dwUserGuid);
	void GSReqCashItemList();
	void GSReqPackageItemList();
	void GSReqBuyCashItem(DWORD dwServerGuid, DWORD dwUserGuid, DWORD dwCharacterGuid, LPSTR szCharacterName, DWORD dwCount, DWORD dwPriceGuid);
	void GSNtfBuyCashItem(protocol::MSG_GTOS_BUY_ITEM_NTF* pMsg);
	void GSReqBranchItemList();
	void SetBranchItem(DWORD dwItemGuid, int iBranchType);
	int  GetBranchType(DWORD dwItemGuid);
	void MakeItemListProtocol();
	LPBYTE GetItemList(int iCategory, int iPageCount);


	static long  CCashShop::ParentWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, long lParam);
	static WNDPROC m_lpOldProc;
  

private:

	int iCashItemCount;	// 4
	int iCashItemPageNumber;	// 8
	BOOL bCashItemListReload;	// C
	std::map<int,OBJECTSTRUCT *> MapUserObject;	//
	std::map<int,CASHSHOP_ITEMLIST *> MapCashItemList;	//
	std::map<int,CASHSHOP_ITEM_STATUS *> MapCashItemStatus;	//
	CASHSHOP_ITEMLIST CashItemList[MAX_CASH_SHOP_CATEGORY][MAX_CASH_SHOP_ITEM];	//
	CASHSHOP_ITEMLIST_TO_PROTOCOL CashItemListCompress[MAX_CASH_SHOP_CATEGORY][MAX_CASH_SHOP_ITEM];	//
	PMSG_ANS_CASHITEMLIST CashItemProtocol[MAX_CASH_SHOP_CATEGORY][MAX_CASH_SHOP_PROTOCOL];	//
	int iAddItemCountInCategory[MAX_CASH_SHOP_CATEGORY];	//
	int iCashItemCountInCategory[MAX_CASH_SHOP_CATEGORY];	//
	CASHSHOP_BRANCH_ITEMLIST BranchItemList[MAX_CASH_ITEM_BRANCH];	//
	int iBranchItemCount;	//
	DWORD dwCheckShopServerConnectStatePeriod;	//
};

extern BOOL g_bUseMoveMapBound;
extern CCashShop g_CashShop;
extern wsShopServerCli g_ShopServerClient;

void ShopServerProtocolCore(DWORD protoNum, LPBYTE aRecv, int aLen);
void SGAnsCashPoint(protocol::MSG_STOG_USER_CASH_ANS* aRecv);
void  SGAnsCashItemList(protocol::MSG_STOG_ITEM_LIST_ANS* aRecv);
void SGAnsPackageItemList(protocol::MSG_STOG_PACKAGE_LIST_ANS* aRecv);
void SGAnsBranchItemList(protocol::MSG_STOG_BRANCH_ITEM_LIST_ANS* lpMsg);
void SGAnsBuyCashItem( protocol::MSG_STOG_BUY_ITEM_ANS* aRecv);

extern BOOL g_bUseLotteryEvent;
#endif

BOOL IsCashItem(int iItemCode);
BOOL IsPremiumItem(int iItemCode);