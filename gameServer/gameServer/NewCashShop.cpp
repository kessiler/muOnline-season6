#include "stdafx.h"
#include "NewCashShop.h"
#include "CashItemPeriodSystem.h"
#include "wsShopServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "LogProc.h"
#include "DSProtocol.h"
#include "..\\include\\readscript.h"
#include "..\common\winutil.h"
#include "DBSockMng.h"
// -------------------------------------------------------------------------------

NewCashShop		gNewCashShop;
// -------------------------------------------------------------------------------

NewCashShop::NewCashShop()
{
	// ----
}
// -------------------------------------------------------------------------------

NewCashShop::~NewCashShop()
{
	// ----
}
// -------------------------------------------------------------------------------

#ifdef NEWCS

void NewCashShop::Initialize()
{
	this->iCashItemCount = 0;
	this->bCashItemListReload = FALSE;
	this->MapUserObject.clear();
	this->MapItemList.clear();
	this->MapPackageList.clear();

	for (int i=0;i<MAX_SHOP_ITEM;i++)
	{
		this->ShopItemList[i].btCoinType = 0;
		this->ShopItemList[i].btItemDurability = 0;
		this->ShopItemList[i].btItemExOption = 0;
		this->ShopItemList[i].btItemLevel = 0;
		this->ShopItemList[i].btItemLuck = 0;
		this->ShopItemList[i].btItemOption = 0;
		this->ShopItemList[i].btItemSetOption = 0;
		this->ShopItemList[i].btItemSkill = 0;
		this->ShopItemList[i].btItemSocketCount = 0;
		this->ShopItemList[i].btItemType = 0;
		this->ShopItemList[i].dwItemGroup = 0;
		this->ShopItemList[i].dwItemPeriodTime = 0;
		this->ShopItemList[i].dwItemType = 0;
		this->ShopItemList[i].wPrice = 0;
		this->ShopItemList[i].wUniqueID1 = 0;
		this->ShopItemList[i].wUniqueID2 = 0;
		this->ShopItemList[i].wUniqueID3 = 0;
		this->ShopItemList[i].btX = 0;
		this->ShopItemList[i].btY = 0;
	}
	for (int i=0;i<MAX_PACK_ITEM;i++)
	{
		this->PackItemList[i].PackageID = 0;
		this->PackItemList[i].btItemDurability = 0;
		this->PackItemList[i].btItemExOption = 0;
		this->PackItemList[i].btItemLevel = 0;
		this->PackItemList[i].btItemLuck = 0;
		this->PackItemList[i].btItemOption = 0;
		this->PackItemList[i].btItemSetOption = 0;
		this->PackItemList[i].btItemSkill = 0;
		this->PackItemList[i].btItemType = 0;
		this->PackItemList[i].dwItemGroup = 0;
		this->PackItemList[i].dwItemPeriodTime = 0;
		this->PackItemList[i].dwItemType = 0;
		this->PackItemList[i].ItemSeqNum = 0;
		this->PackItemList[i].btX = 0;
		this->PackItemList[i].btY = 0;
	}
}
// -------------------------------------------------------------------------------

void NewCashShop::Load(LPSTR pchFilename)
{
	SMDToken Token;

	SMDFile = fopen(pchFilename, "r");

	if ( SMDFile == NULL )
	{
		MsgBox("[InGameShop][ItemList] List load failed. [%s]", pchFilename);
		return;
	}

	this->Initialize();

	int iType = 0;
	INGAMESHOP_ITEMLIST ItemStatus;
	BOOL bResult = 0;

	while ( true )
	{
		Token = (SMDToken)GetToken();

		if ( Token == END )
			break;

		iType = TokenNumber;

		while ( true )
		{
			Token = (SMDToken)GetToken();

			if ( Token == END )
				break;
		
			if ( iType == 1 )
			{
				if ( !strcmp("end", TokenString) )
					break;

				ItemStatus.btGuid = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.dwItemIndex = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.dwItemSubIndex = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.dwItemOpt = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.wItemPackage = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.dwItemGroup = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.dwItemType = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btItemLevel = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btItemDurability = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btItemSkill = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btItemLuck = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btItemOption = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btItemExOption = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btItemSetOption = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btItemSocketCount = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btItemType = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.dwItemPeriodTime = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btCoinType = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.wPrice = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.wUniqueID1 = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.wUniqueID2 = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.wUniqueID3 = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btX = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.btY = TokenNumber;

				Token = (SMDToken)GetToken();
				ItemStatus.ItemCategory = TokenNumber;
				
				LogAddTD("[InGameShop][Load Cash Item List] - Add List - (%d/%d) Level:%d, Dur:%d, Skill:%d, Luck:%d, Add:%d, Ex:%d, X:%d, Y:%d, Price:%d",
					ItemStatus.dwItemGroup, ItemStatus.dwItemType, ItemStatus.btItemLevel, ItemStatus.btItemDurability,
					ItemStatus.btItemSkill, ItemStatus.btItemLuck, ItemStatus.btItemOption, ItemStatus.btItemExOption, 
					ItemStatus.btX, ItemStatus.btY, ItemStatus.wPrice);

				this->InsertItemStatus(&ItemStatus);
			}
		}
	}

	fclose(SMDFile);

	LogAddTD("[InGameShop][Load Item List] - Complete! - Total:%d",
		this->MapItemList.size());

	this->LoadPackages(gDirPath.GetNewPath("InGameShopPackages.txt"));
}
// -------------------------------------------------------------------------------

void NewCashShop::LoadPackages(LPSTR pchFilename)
{
	SMDToken Token;

	SMDFile = fopen(pchFilename, "r");

	if(SMDFile == NULL)
	{
		MsgBox("[InGameShop][PackageList] File not found: %s", pchFilename);
		return;
	}

	int iType = 0;
	INGAMESHOP_PACKAGELIST PackStatus;
	BOOL bResult = 0;

	while ( true )
	{
		Token = (SMDToken)GetToken();

		if ( Token == END )
			break;

		iType = TokenNumber;

		while ( true )
		{
			Token = (SMDToken)GetToken();

			if ( Token == END )
				break;
		
			if ( iType == 1 )
			{
				if ( !strcmp("end", TokenString) )
					break;

				PackStatus.Guid = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.PackageID = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.ItemSeqNum = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.dwItemGroup = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.dwItemType = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btItemLevel = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btItemDurability = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btItemSkill = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btItemLuck = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btItemOption = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btItemExOption = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btItemSetOption = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btItemType = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.dwItemPeriodTime = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.wUniqueID1 = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.wUniqueID2 = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.wUniqueID3 = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btX = TokenNumber;

				Token = (SMDToken)GetToken();
				PackStatus.btY = TokenNumber;

				LogAddTD("[InGameShop][Package Item List] Add Package Item: Pack: %d, Seq: %d", PackStatus.PackageID, PackStatus.ItemSeqNum);
				this->InsertPackStatus(&PackStatus);
			}
		}
	}
	LogAddTD("[InGameShop][Package Load List] Complete! - Total:%d", this->MapPackageList.size());
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::InsertItemStatus(INGAMESHOP_ITEMLIST * lpItemStatus)
{
	int iItemCode = 0;
	INGAMESHOP_ITEMLIST * lpNewItemStatus = new INGAMESHOP_ITEMLIST;

	memcpy(lpNewItemStatus, lpItemStatus, sizeof(INGAMESHOP_ITEMLIST));

	this->MapItemList.insert(std::pair<int, INGAMESHOP_ITEMLIST *>(lpItemStatus->btGuid, lpNewItemStatus));

	return TRUE;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::InsertPackStatus(INGAMESHOP_PACKAGELIST * lpItemStatus)
{
	int iItemCode = 0;
	INGAMESHOP_PACKAGELIST * lpNewItemStatus = new INGAMESHOP_PACKAGELIST;

	memcpy(lpNewItemStatus, lpItemStatus, sizeof(INGAMESHOP_PACKAGELIST));

	this->MapPackageList.insert(std::pair<int, INGAMESHOP_PACKAGELIST *>(lpItemStatus->Guid, lpNewItemStatus));

	return TRUE;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::IsGetAmountFromShopItem(int iItemCode) //GS-CS Weird Compilation
{
	switch ( iItemCode )
	{
		case ITEMGET(14,53):

		case ITEMGET(13,46):
		case ITEMGET(13,47):
		case ITEMGET(13,48):

		case ITEMGET(14,70): //CashShop HP Potion
	
		return TRUE;
	}

	return FALSE;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::AddUser(LPOBJ lpObj)
{
	if ( this->SearchUser(lpObj->DBNumber) )
		return FALSE;

	PMSG_REQ_INGAMESHOPINIT pInit;
	PHeadSubSetB((LPBYTE)&pInit, 0xD2, 0x00, sizeof(pInit));
	DataSend(lpObj->m_Index, (LPBYTE)&pInit, sizeof(pInit));

	CASHSHOP_VER_SCRIPT pScriptAuth;
	PHeadSubSetB((LPBYTE)&pScriptAuth, 0xD2, 0x0C, sizeof(pScriptAuth));
	pScriptAuth.Major		= 512;
	pScriptAuth.Minor		= 2011;
	pScriptAuth.Revision	= 006;
	DataSend(lpObj->m_Index, (LPBYTE)&pScriptAuth, sizeof(pScriptAuth));

	CASHSHOP_VER_BANNER pBannerAuth;
	PHeadSubSetB((LPBYTE)&pBannerAuth, 0xD2, 0x15, sizeof(pBannerAuth));
	pBannerAuth.Major		= 583;
	pBannerAuth.Minor		= 2010;
	pBannerAuth.Revision	= 005;
	DataSend(lpObj->m_Index, (LPBYTE)&pBannerAuth, sizeof(pBannerAuth));

	this->MapUserObject.insert(std::pair<int, LPOBJ>(lpObj->DBNumber, lpObj));
	//GDReqInGameShopPoint(lpObj->m_Index);
	//GDReqInGameShopItemList(lpObj->m_Index);
	return TRUE;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::DeleteUser(LPOBJ lpObj)
{
	std::map<int, OBJECTSTRUCT *>::iterator Iter;

	Iter = this->MapUserObject.find(lpObj->DBNumber);
	
	if ( Iter == this->MapUserObject.end() )
		return FALSE;

	this->MapUserObject.erase(Iter);

	for(int i=0;i<30;++i)
	{
		lpObj->ShopInventory[i].UniqueCode = 0;
		lpObj->ShopInventory[i].AuthCode = 0;
		lpObj->ShopInventory[i].ItemCode1 = 0;
		lpObj->ShopInventory[i].ItemCode2 = 0;
		lpObj->ShopInventory[i].ItemCode3 = 0;
		lpObj->GiftInventory[i].UniqueCode = 0;
		lpObj->GiftInventory[i].AuthCode = 0;
		lpObj->GiftInventory[i].ItemCode1 = 0;
		lpObj->GiftInventory[i].ItemCode2 = 0;
		lpObj->GiftInventory[i].ItemCode3 = 0;
	}

	return TRUE;
}
// -------------------------------------------------------------------------------

LPOBJ NewCashShop::SearchUser(int iUserGuid)
{
	LPOBJ lpReturnObj = NULL;

	std::map<int, OBJECTSTRUCT *>::iterator Iter;
	Iter = this->MapUserObject.find(iUserGuid);

	if ( Iter == this->MapUserObject.end() )
		return FALSE;

	lpReturnObj = Iter->second;

	return lpReturnObj;
}
// -------------------------------------------------------------------------------

struct PMSG_ANS_CASHSHOPOPEN
{
	PBMSG_HEAD2 head;	// C1:D2:02
	BYTE btResult;	// 4
};
// -------------------------------------------------------------------------------

BOOL NewCashShop::CGCashShopOpen(LPOBJ lpObj, PMSG_REQ_INGAMESHOPOPEN * lpMsg)
{
	BYTE btResult = 0;

	if ( this->bCashItemListReload == TRUE )
	{
		btResult = 6;
	}

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		LogAddC(2, "Trying to open shop with disconnected state (%s)(%d)", lpObj->AccountID, lpObj->m_Index);
		return FALSE;
	}

	if ( lpObj->Connected <= PLAYER_LOGGED || lpObj->CloseCount != -1 )
		return FALSE;

	if ( lpMsg->btShopOpenType == 0 )
	{
		if ( lpObj->m_IfState.use > 0 )
		{
			btResult = 8;
		}

		if ( btResult == 0 )
		{
			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 18;
			lpObj->m_IfState.state = 1;
			btResult = 1;
			LogAddTD("[InGameShop] [%s][%s] Opened shop", lpObj->AccountID, lpObj->Name);
		}
	}
	else if ( lpMsg->btShopOpenType == 1 )
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.type = 0;
		lpObj->m_IfState.state = 0;
		btResult = 0;
	}

	PMSG_ANS_CASHSHOPOPEN pMsg;

	pMsg.btResult = btResult;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x02, sizeof(PMSG_ANS_CASHSHOPOPEN));

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.head.size);

	return TRUE;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::CGCashInventoryItemCount(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_INVENTORY *lpMsg)
{
	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID) )
	{
		LogAddC(2, "Trying to get inventory with disconnected state (%s)(%d)", lpObj->AccountID, lpObj->m_Index);
		return FALSE;
	}
	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return FALSE;

	this->GCCashInventoryItemCount(lpObj, lpMsg);
	return true;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::CGCashPoint(LPOBJ lpObj)
{
	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		LogAddC(2, "Trying to get cash point with disconnected state (%s)(%d)", lpObj->AccountID, lpObj->m_Index);
		return FALSE;
	}

	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return FALSE;

	this->GCCashPoint(lpObj);

	return TRUE;
}
// -------------------------------------------------------------------------------

void NewCashShop::GCCashPoint(LPOBJ lpObj)
{
	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		LogAddC(2, "Trying to get cash point with disconnected state (%s)(%d)", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	PMSG_ANS_INGAMESHOP_POINT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x01, sizeof(PMSG_ANS_INGAMESHOP_POINT));

	lpObj->m_GoblinPoint	= 3500;
	lpObj->m_WCoinC			= 4500;
	lpObj->m_WCoinP			= 5500;

	pMsg.Goblin = lpObj->m_GoblinPoint;
	pMsg.WCoinC[0] = lpObj->m_WCoinC;
	pMsg.WCoinC[1] = lpObj->m_WCoinC;
	pMsg.WCoinP[0] = lpObj->m_WCoinP;
	pMsg.WCoinP[1] = lpObj->m_WCoinP;

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}
// -------------------------------------------------------------------------------

int NewCashShop::GetItemCountInShopInventory(LPOBJ lpObj, int InventoryType)
{
	int Count = 0;
	for(int i=0;i<30;++i)
	{
		if(InventoryType == 0x53)
		{
			if(lpObj->ShopInventory[i].UniqueCode != 0)
			{
				++Count;
			}
		}
		else if(InventoryType == 0x47)
		{
			if(lpObj->GiftInventory[i].UniqueCode != 0)
			{
				++Count;
			}
		}
	}
	return Count;
}
// -------------------------------------------------------------------------------

void NewCashShop::GCCashInventoryItemCount(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_INVENTORY *lpMsg)
{
	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		LogAddC(2, "Trying to get inventory items with disconnected state (%s)(%d)", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	int ItemCount = this->GetItemCountInShopInventory(lpObj, lpMsg->InventoryType);

	PMSG_ANS_INGAMESHOP_ITEMCOUNT pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x06, sizeof(pMsg));
	if(ItemCount > 0)
	{
		pMsg.OwnInv[0] = ItemCount+1;
		pMsg.OwnInv[1] = ItemCount+1;
		pMsg.GiftInv[0] = 1;
		pMsg.GiftInv[1] = 1;
	}
	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	this->GCCashItemInventory(lpObj, ItemCount, lpMsg->InventoryType);
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::GCCashItemInventory(LPOBJ lpObj, int ItemCount, int InventoryType)
{
	PMSG_ANS_INGAMESHOP_ITEM pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x0D, sizeof(pMsg));
	pMsg.Unk3 = 0x50;
	int Count = 0;
	for(int i=0;i<30;++i)
	{
		if(InventoryType == 0x53 && lpObj->ShopInventory[i].UniqueCode != 0 && lpObj->ShopInventory[i].AuthCode != 0)
		{
			pMsg.UniqueCode = lpObj->ShopInventory[i].UniqueCode;
			pMsg.AuthCode = lpObj->ShopInventory[i].AuthCode;
			pMsg.UniqueValue1 = 673;//lpObj->ShopInventory[i].ItemCode1;
			pMsg.UniqueValue2 = 20;//lpObj->ShopInventory[i].ItemCode2;
			pMsg.UniqueValue3 = 31;//lpObj->ShopInventory[i].ItemCode3;
			++Count;
		}
		else if(InventoryType == 0x47 && lpObj->GiftInventory[i].UniqueCode != 0 && lpObj->GiftInventory[i].AuthCode != 0)
		{
			pMsg.UniqueCode = lpObj->GiftInventory[i].UniqueCode;
			pMsg.AuthCode = lpObj->GiftInventory[i].AuthCode;
			pMsg.UniqueValue1 = lpObj->GiftInventory[i].ItemCode1;
			pMsg.UniqueValue2 = lpObj->GiftInventory[i].ItemCode2;
			pMsg.UniqueValue3 = lpObj->GiftInventory[i].ItemCode3;
			++Count;
		}
		else
		{
			continue;
		}
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		if(Count >= ItemCount) break;
	}
	return TRUE;
}
// -------------------------------------------------------------------------------

bool NewCashShop::InsertItemToInventory(LPOBJ lpObj, int UniqueCode, int AuthCode, int UniqueID1, int UniqueID2, int UniqueID3, BYTE Inventory)
{
	for(int i=0;i<30;++i)
	{
		if(Inventory == 1)
		{
			if(lpObj->ShopInventory[i].UniqueCode == 0 && lpObj->ShopInventory[i].AuthCode == 0)
			{
				lpObj->ShopInventory[i].UniqueCode = UniqueCode;
				lpObj->ShopInventory[i].AuthCode = AuthCode;
				lpObj->ShopInventory[i].ItemCode1 = UniqueID1;
				lpObj->ShopInventory[i].ItemCode2 = UniqueID2;
				lpObj->ShopInventory[i].ItemCode3 = UniqueID3;
				LogAddTD("[InGameShop] (%s) Add Item: %d/%d/%d/%d%d", lpObj->AccountID, UniqueCode, AuthCode, UniqueID1, UniqueID2, UniqueID3);
				return true;
			}
		}
		else if(Inventory == 2)
		{
			if(lpObj->GiftInventory[i].UniqueCode == 0 && lpObj->GiftInventory[i].AuthCode == 0)
			{
				lpObj->GiftInventory[i].UniqueCode = UniqueCode;
				lpObj->GiftInventory[i].AuthCode = AuthCode;
				lpObj->GiftInventory[i].ItemCode1 = UniqueID1;
				lpObj->GiftInventory[i].ItemCode2 = UniqueID2;
				lpObj->GiftInventory[i].ItemCode3 = UniqueID3;
				LogAddTD("[InGameShop] (%s) Add Gift: %d/%d/%d/%d%d", lpObj->AccountID, UniqueCode, AuthCode, UniqueID1, UniqueID2, UniqueID3);
				return true;
			}
		}
	}
	return false;
}
// -------------------------------------------------------------------------------

void NewCashShop::CGCashInventoryItemUse(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMUSE *lpMsg)
{
	PMSG_ANS_INGAMESHOP_ITEMUSE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x0B, sizeof(pMsg));

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		LogAddC(2, "Trying to use item with disconnected state (%s)(%d)", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return;

	INGAMESHOP_ITEMLIST * lpItem = this->SearchItemFromInventory(lpObj, lpMsg->UniqueCode, lpMsg->AuthCode);

	if(lpItem == NULL)
	{
		pMsg.result = 1;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(::CheckInventoryEmptySpace(lpObj, lpItem->btX, lpItem->btY) == false)
	{
		pMsg.result = 1;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(this->DeleteItemFromInventory(lpObj, lpMsg->UniqueCode, lpMsg->AuthCode) == true)
	{

		if(lpItem->btItemType == 1)
		{
			g_CashItemPeriodSystem.GDReqPeriodItemInsert(lpObj, lpMsg->ItemID, lpItem->dwItemPeriodTime);
		}
		else if(lpItem->btItemType == 2)
		{
			ItemSerialCreateSend(lpObj->m_Index, 236, NULL, NULL, lpMsg->ItemID, lpItem->btItemLevel, 
				lpItem->btItemDurability, lpItem->btItemSkill, lpItem->btItemLuck, lpItem->btItemOption,
				lpItem->dwItemPeriodTime, lpItem->btItemExOption, lpItem->btItemSetOption);
		}
		else
		{
			ItemSerialCreateSend(lpObj->m_Index, 235, NULL, NULL, lpMsg->ItemID, lpItem->btItemLevel, 
				lpItem->btItemDurability, lpItem->btItemSkill, lpItem->btItemLuck, lpItem->btItemOption,
				lpObj->m_Index, lpItem->btItemExOption, lpItem->btItemSetOption);
		}
		pMsg.result = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		this->GCCashPoint(lpObj);
		GDReqInGameShopItemListSave(lpObj->m_Index);
		GDReqInGameShopPointSave(lpObj->m_Index);
		LogAddTD("[InGameShop] (%s)(%s) Used Item %d/%d/%d", lpObj->AccountID, lpObj->Name, lpMsg->UniqueCode, lpMsg->AuthCode, lpMsg->ItemID);
	}
	return;
}
// -------------------------------------------------------------------------------

INGAMESHOP_ITEMLIST * NewCashShop::SearchItemFromInventory(LPOBJ lpObj, int UniqueCode, int AuthCode)
{
	INGAMESHOP_ITEMLIST * lpItem = NULL;
	for(int i=0;i<30;++i)
	{
		if(lpObj->ShopInventory[i].UniqueCode == UniqueCode && lpObj->ShopInventory[i].AuthCode == AuthCode)
		{
			lpItem = this->SearchItemList(lpObj->ShopInventory[i].ItemCode1, lpObj->ShopInventory[i].ItemCode2, lpObj->ShopInventory[i].ItemCode3);
		}
		else if(lpObj->GiftInventory[i].UniqueCode == UniqueCode && lpObj->GiftInventory[i].AuthCode == AuthCode)
		{
			lpItem = this->SearchItemList(lpObj->GiftInventory[i].ItemCode1, lpObj->GiftInventory[i].ItemCode2, lpObj->GiftInventory[i].ItemCode3);
		}
	}
	return lpItem;
}
// -------------------------------------------------------------------------------

bool NewCashShop::DeleteItemFromInventory(LPOBJ lpObj, int UniqueCode, int AuthCode)
{
	for(int i=0;i<30;++i)
	{
		if(lpObj->ShopInventory[i].UniqueCode == UniqueCode && lpObj->ShopInventory[i].AuthCode == AuthCode)
		{
			lpObj->ShopInventory[i].AuthCode = 0;
			lpObj->ShopInventory[i].UniqueCode = 0;
			lpObj->ShopInventory[i].ItemCode1 = 0;
			lpObj->ShopInventory[i].ItemCode2 = 0;
			lpObj->ShopInventory[i].ItemCode3 = 0;
			PMSG_REQ_INGAMESHOP_INVENTORY pList = {0};
			pList.InventoryType = 0x53;

			this->GCCashInventoryItemCount(lpObj, (PMSG_REQ_INGAMESHOP_INVENTORY *)&pList);
			return true;
		}
		else if(lpObj->GiftInventory[i].UniqueCode == UniqueCode && lpObj->GiftInventory[i].AuthCode == AuthCode)
		{
			lpObj->GiftInventory[i].AuthCode = 0;
			lpObj->GiftInventory[i].UniqueCode = 0;
			lpObj->GiftInventory[i].ItemCode1 = 0;
			lpObj->GiftInventory[i].ItemCode2 = 0;
			lpObj->GiftInventory[i].ItemCode3 = 0;
			PMSG_REQ_INGAMESHOP_INVENTORY pList = {0};
			pList.InventoryType = 0x47;

			this->GCCashInventoryItemCount(lpObj, (PMSG_REQ_INGAMESHOP_INVENTORY *)&pList);
			return true;
		}
	}
	return false;
}
// -------------------------------------------------------------------------------

void NewCashShop::CGCashItemBuy(LPOBJ lpObj, PMSG_REQ_INGAMESHOP_ITEMBUY *lpMsg)
{
	int iCategoryIndex = 0;
	BYTE btPosition = 0;
	BOOL bItemEmptySpace = FALSE;
	int iItemCode = 0;
	BYTE buyFlag = FALSE;
	INGAMESHOP_ITEMLIST* lpCashItemInfo = NULL;

	lpObj->m_WCoinP = 95000;
	lpObj->m_WCoinC	= 95000;
	lpObj->m_GoblinPoint = 95000;

	if ( this->bCashItemListReload == TRUE )
		return;

	if ( !gObjIsAccontConnect(lpObj->m_Index, lpObj->AccountID))
	{
		LogAddC(2, "Trying to buy item with disconnected state (%s)(%d)", lpObj->AccountID, lpObj->m_Index);
		return;
	}

	if ( lpObj->Connected !=PLAYER_PLAYING && lpObj->Type != OBJ_USER )
		return;

	lpCashItemInfo = this->SearchItemList(lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->Category, lpMsg->ItemID, lpMsg->ItemOpt);

	if(lpCashItemInfo == NULL) 
	{
		LogAddC(4, "[InGameShop] Unknown Item: Index:%d Sub:%d Category: %d ItemID: %d Opt: %d", lpMsg->ItemIndex, lpMsg->ItemSubIndex, lpMsg->Category, lpMsg->ItemID, lpMsg->ItemOpt);
		return;
	}

	switch ( lpCashItemInfo->btCoinType )
	{
		case 0:
			{
				if(lpObj->m_WCoinC >= lpCashItemInfo->wPrice)
				{
					lpObj->m_WCoinC -= lpCashItemInfo->wPrice;
					buyFlag = TRUE;
				}
				break;
			}
		case 1:
			{
				if(lpObj->m_WCoinP >= lpCashItemInfo->wPrice)
				{
					lpObj->m_WCoinP -= lpCashItemInfo->wPrice;
					buyFlag = TRUE;
				}
				break;
			}
		case 2:
			{
				if(lpObj->m_GoblinPoint >= lpCashItemInfo->wPrice)
				{
					lpObj->m_GoblinPoint -= lpCashItemInfo->wPrice;
					buyFlag = TRUE;
				}
				break;
			}
	}
	PMSG_ANS_INGAMESHOP_ITEMBUY pMsg = {-1};
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x03, sizeof(pMsg));
	if(buyFlag == TRUE)
	{
		if(lpCashItemInfo->wItemPackage != 0)
		{
			this->UsePackage(lpObj, lpCashItemInfo->wItemPackage);
	
			pMsg.Result = 0;

			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			PMSG_REQ_INGAMESHOP_INVENTORY pList = {0};
			pList.InventoryType = 0x53;

			this->GCCashInventoryItemCount(lpObj, (PMSG_REQ_INGAMESHOP_INVENTORY *)&pList);
			this->GCCashPoint(lpObj);
		}
		else if(this->GiveBuyItemToInventory(lpObj, lpCashItemInfo, 1) == true)
		{
			LogAddTD("[InGameShop] (%s)(%s) Purchase success", lpObj->AccountID, lpObj->Name);

			pMsg.Result = 0;

			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			PMSG_REQ_INGAMESHOP_INVENTORY pList = {0};
			pList.InventoryType = 0x53;

			this->GCCashInventoryItemCount(lpObj, (PMSG_REQ_INGAMESHOP_INVENTORY *)&pList);
			this->GCCashPoint(lpObj);
		}
		GDReqInGameShopItemListSave(lpObj->m_Index);
		GDReqInGameShopPointSave(lpObj->m_Index);
	}
	else
	{
		pMsg.Result = 1;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

}
// -------------------------------------------------------------------------------

bool NewCashShop::UsePackage(LPOBJ lpObj, int PackageID)
{
	std::map<int,INGAMESHOP_PACKAGELIST *>::iterator Iter;
	for(Iter = this->MapPackageList.begin(); Iter != this->MapPackageList.end(); ++Iter)
	{
		if(Iter->second->PackageID == PackageID)
		{
			this->GiveBuyItemToInventory(lpObj, Iter->second, 1);
		}
	}
	return true;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::CheckPeriodItemUsed(LPOBJ lpObj, int iItemCode) //GS-CS Fine 100% Decompiled
{
	for (int i=0;i<10;++i)
	{
/*		if(lpObj->m_iPeriodItemEffectIndex[i] != -1)
		{
			if ( iItemCode== ITEMGET(13,43) || 
			 iItemCode == ITEMGET(13,44) || 
			 iItemCode == ITEMGET(13,45) ||
			 iItemCode == ITEMGET(13,54) ||
			 iItemCode == ITEMGET(14,72) || //Fine
			 iItemCode == ITEMGET(14,73) || //Fine
			 iItemCode == ITEMGET(14,74) || //Fine
			 iItemCode == ITEMGET(14,75) || //Fine
			 iItemCode == ITEMGET(14,76) || //Fine
			 iItemCode == ITEMGET(14,77) ) //Fine
			{
				return TRUE;
			}
		}*/
	}
	return FALSE;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::CheckPeriodItem(int iItemCode) //GS-CS Fine 100% Decompiled
{
	if ( iItemCode == ITEMGET(13,43) || 
		 iItemCode == ITEMGET(13,44) || 
		 iItemCode == ITEMGET(13,45) ||
		 iItemCode == ITEMGET(13,54) ||
		 iItemCode == ITEMGET(13,59) || //Seal of Mobility, (Season2 is 13,52 & Season3 is 13,59)
		 iItemCode == ITEMGET(14,72) ||
		 iItemCode == ITEMGET(14,73) ||
		 iItemCode == ITEMGET(14,74) ||
		 iItemCode == ITEMGET(14,75) ||
		 iItemCode == ITEMGET(14,76) ||
		 iItemCode == ITEMGET(14,77) )
		{
			return TRUE;
		}

	return FALSE;
}
// -------------------------------------------------------------------------------

INGAMESHOP_ITEMLIST * NewCashShop::SearchItemList(int ItemIndex,int ItemSubIndex,int Category,int ItemID, int ItemOpt)
{
	
	INGAMESHOP_ITEMLIST* lpItemInfo = NULL;
	std::map<int,INGAMESHOP_ITEMLIST *>::iterator Iter;

	for(Iter = this->MapItemList.begin(); Iter != this->MapItemList.end(); ++Iter)
	{
		lpItemInfo = Iter->second;
		if(ItemIndex == lpItemInfo->dwItemIndex /*&& ItemSubIndex == lpItemInfo->dwItemSubIndex */&& Category == lpItemInfo->ItemCategory && ItemID == ItemGetNumberMake(lpItemInfo->dwItemGroup, lpItemInfo->dwItemType) && ItemOpt == lpItemInfo->dwItemOpt)
		{
			break;
		}
		else
		{
			lpItemInfo = NULL;
			continue;
		}
	}

	return lpItemInfo;
}
// -------------------------------------------------------------------------------

INGAMESHOP_ITEMLIST * NewCashShop::SearchItemList(int UniqueID1, int UniqueID2, int UniqueID3)
{
	
	INGAMESHOP_ITEMLIST* lpItemInfo = NULL;
	std::map<int,INGAMESHOP_ITEMLIST *>::iterator Iter;

	for(Iter = this->MapItemList.begin(); Iter != this->MapItemList.end(); ++Iter)
	{
		lpItemInfo = Iter->second;
		if(UniqueID1 == lpItemInfo->wUniqueID1 && UniqueID2 == lpItemInfo->wUniqueID2 && UniqueID3 == lpItemInfo->wUniqueID3)
		{
			break;
		}
		else
		{
			lpItemInfo = NULL;
			continue;
		}
	}

	return lpItemInfo;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::GiveBuyItemToInventory(LPOBJ lpObj, INGAMESHOP_ITEMLIST *lpItemInfo, BYTE Inventory)
{
	if(Inventory == 1)
	{
		for(int i=0;i<30;++i)
		{
			if(lpObj->ShopInventory[i].UniqueCode == 0 && lpObj->ShopInventory[i].AuthCode == 0)
			{
				lpObj->ShopInventory[i].UniqueCode = rand()%10000+4400/200;
				lpObj->ShopInventory[i].AuthCode = rand()%250000+1700/12;
				lpObj->ShopInventory[i].ItemCode1 = lpItemInfo->wUniqueID1;
				lpObj->ShopInventory[i].ItemCode2 = lpItemInfo->wUniqueID2;
				lpObj->ShopInventory[i].ItemCode3 = lpItemInfo->wUniqueID3;
				LogAddTD("[InGameShop] (%s)(%s) -> Added Item to ShopInventory (%d/%d/%d/%d/%d)", lpObj->AccountID, lpObj->Name,
					lpObj->ShopInventory[i].UniqueCode, lpObj->ShopInventory[i].AuthCode,
					lpObj->ShopInventory[i].ItemCode1, lpObj->ShopInventory[i].ItemCode2, lpObj->ShopInventory[i].ItemCode3);
				return TRUE;
			}
		}
	}
	return FALSE;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::GiveBuyItemToInventory(LPOBJ lpObj, INGAMESHOP_PACKAGELIST *lpItemInfo, BYTE Inventory)
{
	if(Inventory == 1)
	{
		for(int i=0;i<30;++i)
		{
			if(lpObj->ShopInventory[i].UniqueCode == 0 && lpObj->ShopInventory[i].AuthCode == 0)
			{
				lpObj->ShopInventory[i].UniqueCode = rand()%10000+4400/200;
				lpObj->ShopInventory[i].AuthCode = rand()%250000+1700/12;
				lpObj->ShopInventory[i].ItemCode1 = lpItemInfo->wUniqueID1;
				lpObj->ShopInventory[i].ItemCode2 = lpItemInfo->wUniqueID2;
				lpObj->ShopInventory[i].ItemCode3 = lpItemInfo->wUniqueID3;
				LogAddTD("[InGameShop] (%s)(%s) -> Added Item to ShopInventory (%d/%d/%d/%d/%d)", lpObj->AccountID, lpObj->Name,
					lpObj->ShopInventory[i].UniqueCode, lpObj->ShopInventory[i].AuthCode,
					lpObj->ShopInventory[i].ItemCode1, lpObj->ShopInventory[i].ItemCode2, lpObj->ShopInventory[i].ItemCode3);
				return TRUE;
			}
		}
	}
	return FALSE;
}
// -------------------------------------------------------------------------------

BOOL NewCashShop::IsCashItem(int iItemCode) //GS-CS Weird Compilation
{
	switch ( iItemCode )
	{
		case ITEMGET(13,43): return TRUE;
		case ITEMGET(13,44): return TRUE;
		case ITEMGET(13,45): return TRUE;
		case ITEMGET(13,46): return TRUE;
		case ITEMGET(13,47): return TRUE;
		case ITEMGET(13,48): return TRUE;
		case ITEMGET(13,52): return TRUE;
		case ITEMGET(13,53): return TRUE;
		case ITEMGET(13,54): return TRUE;
		case ITEMGET(13,55): return TRUE;
		case ITEMGET(13,56): return TRUE;
		case ITEMGET(13,57): return TRUE;
		case ITEMGET(13,58): return TRUE;
		case ITEMGET(14,53): return TRUE;
		case ITEMGET(14,54): return TRUE;
		case ITEMGET(14,58): return TRUE;
		case ITEMGET(14,59): return TRUE;
		case ITEMGET(14,60): return TRUE;
		case ITEMGET(14,61): return TRUE;
		case ITEMGET(14,62): return TRUE;
		case ITEMGET(14,70): return TRUE;
		case ITEMGET(14,71): return TRUE;
		case ITEMGET(14,72): return TRUE;
		case ITEMGET(14,73): return TRUE;
		case ITEMGET(14,74): return TRUE;
		case ITEMGET(14,75): return TRUE;
		case ITEMGET(14,76): return TRUE;
		case ITEMGET(14,77): return TRUE;
		case ITEMGET(14,78): return TRUE;
		case ITEMGET(14,79): return TRUE;
		case ITEMGET(14,80): return TRUE;
		case ITEMGET(14,81): return TRUE;
		case ITEMGET(14,82): return TRUE;

	}

	return FALSE;
}		
// -------------------------------------------------------------------------------

void NewCashShop::GDReqInGameShopItemList(int aIndex)
{
	ISHOP_REQ_ITEMLIST pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xD2, 0x02, sizeof(pMsg));

	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, 11);
	pMsg.aIndex = aIndex;

	cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
	LogAddTD("[InGameShop] Requesting Item List: [%s]", gObj[aIndex].AccountID);
}
// -------------------------------------------------------------------------------

void NewCashShop::DGAnsInGameShopItemList(LPBYTE lpRecv)
{
	ISHOP_ANS_ITEMLIST * lpMsg = (ISHOP_ANS_ITEMLIST *)(lpRecv);
	ISHOP_ITEMLIST * lpMsgBody = (ISHOP_ITEMLIST *)(lpRecv+sizeof(ISHOP_ANS_ITEMLIST));

	LPOBJ lpObj = &gObj[lpMsg->aIndex];

	for(int i=0;i<lpMsg->Count;++i)
	{
		lpMsgBody = (ISHOP_ITEMLIST *)(lpRecv+sizeof(ISHOP_ANS_ITEMLIST) + (i*sizeof(ISHOP_ITEMLIST)));
		this->InsertItemToInventory(lpObj, lpMsgBody->UniqueCode, lpMsgBody->AuthCode,
			lpMsgBody->UniqueID1, lpMsgBody->UniqueID2, lpMsgBody->UniqueID3, lpMsgBody->InventoryType);
	}	
}
// -------------------------------------------------------------------------------

void NewCashShop::GDReqInGameShopPoint(int aIndex)
{
	ISHOP_REQ_POINT pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, 0xD1, sizeof(pMsg));
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, 11);
	pMsg.aIndex = aIndex;

	cDBSMng.Send((PCHAR)&pMsg, sizeof(pMsg));
	LogAddTD("[InGameShop] Requesting Point Info: [%s]", gObj[aIndex].AccountID);
}
// -------------------------------------------------------------------------------

void NewCashShop::DGAnsInGameShopPoint(ISHOP_ANS_POINT *lpMsg)
{
	gObj[lpMsg->aIndex].m_WCoinC = lpMsg->CoinC;
	gObj[lpMsg->aIndex].m_WCoinP = lpMsg->CoinP;
	gObj[lpMsg->aIndex].m_GoblinPoint = lpMsg->Goblin;

	LogAddTD("[InGameShop] [%s] WCoinC: %f WCoinP: %f GoblinPoint: %f", gObj[lpMsg->aIndex].AccountID, gObj[lpMsg->aIndex].m_WCoinC, gObj[lpMsg->aIndex].m_WCoinP, gObj[lpMsg->aIndex].m_GoblinPoint);
}
// -------------------------------------------------------------------------------

void NewCashShop::GDReqInGameShopTimeItemsList(int aIndex)
{
	ISHOP_PITEM_REQ pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0xD3, sizeof(pMsg));

	pMsg.aIndex = aIndex;
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, 11);

	cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);
}
// -------------------------------------------------------------------------------

void NewCashShop::DGAnsInGameShopTimeItemsList(LPBYTE lpRecv)
{
	ISHOP_PITEM_SAVE * pInfo = (ISHOP_PITEM_SAVE *)(lpRecv);
	ISHOP_PITEM_INFO * pItem = (ISHOP_PITEM_INFO *)(lpRecv+sizeof(ISHOP_PITEM_SAVE));

//	g_PropItems.ErasePlayerItems(&gObj[pInfo->aIndex]);

	for(int i=0;i<pInfo->Count;++i)
	{
		pItem = (ISHOP_PITEM_INFO *)(lpRecv+sizeof(ISHOP_PITEM_SAVE)+ i*sizeof(ISHOP_PITEM_INFO));
//		g_PropItems.LoadPropertyItem(pInfo->aIndex, pItem->ItemID, pItem->Serial, pItem->Time);
	}
}
// -------------------------------------------------------------------------------

void NewCashShop::GDReqInGameShopPointSave(int aIndex)
{
	ISHOP_ANS_POINT pMsg = {0};

	PHeadSetB((LPBYTE)&pMsg, 0xD7, sizeof(pMsg));

	pMsg.aIndex = aIndex;
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, 11);
	pMsg.Result = 1;
	pMsg.CoinC = gObj[aIndex].m_WCoinC;
	pMsg.CoinP = gObj[aIndex].m_WCoinP;
	pMsg.Goblin = gObj[aIndex].m_GoblinPoint;

	cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size);
}
// -------------------------------------------------------------------------------

void NewCashShop::GDReqInGameShopItemListSave(int aIndex)
{
	char Buff[1024] = {0};
	int PacketSize = 0;
	int shopcount = 0;

	ISHOP_ANS_ITEMLIST * Info = (ISHOP_ANS_ITEMLIST *)(Buff);
	ISHOP_ITEMLIST * Item = (ISHOP_ITEMLIST *)(Buff+sizeof(ISHOP_ANS_ITEMLIST));

	for(int i=0;i<30;++i)
	{
		Item = (ISHOP_ITEMLIST *)(Buff+sizeof(ISHOP_ANS_ITEMLIST)+i*sizeof(ISHOP_ITEMLIST));
		if(gObj[aIndex].ShopInventory[i].UniqueCode != 0)
		{
			Item->UniqueCode = gObj[aIndex].ShopInventory[i].UniqueCode;
			Item->AuthCode = gObj[aIndex].ShopInventory[i].AuthCode;
			Item->UniqueID1 = gObj[aIndex].ShopInventory[i].ItemCode1;
			Item->UniqueID2 = gObj[aIndex].ShopInventory[i].ItemCode2;
			Item->UniqueID3 = gObj[aIndex].ShopInventory[i].ItemCode3;
			Item->InventoryType = 1;
			Info->Count++;
			shopcount++;
		}
	}
	for(int i=0;i<30;++i)
	{
		shopcount = shopcount+i;
		Item = (ISHOP_ITEMLIST *)(Buff+sizeof(ISHOP_ANS_ITEMLIST)+shopcount*sizeof(ISHOP_ITEMLIST));
		if(gObj[aIndex].GiftInventory[i].UniqueCode != 0)
		{
			Item->UniqueCode = gObj[aIndex].GiftInventory[i].UniqueCode;
			Item->AuthCode = gObj[aIndex].GiftInventory[i].AuthCode;
			Item->UniqueID1 = gObj[aIndex].GiftInventory[i].ItemCode1;
			Item->UniqueID2 = gObj[aIndex].GiftInventory[i].ItemCode2;
			Item->UniqueID3 = gObj[aIndex].GiftInventory[i].ItemCode3;
			Item->InventoryType = 1;
			Info->Count++;
		}
	}
	PacketSize = sizeof(ISHOP_ANS_ITEMLIST)+Info->Count*sizeof(ISHOP_ITEMLIST);

	Info->h.c = 0xC2;
	Info->h.headcode = 0xD8;
	Info->h.sizeH = HIBYTE(PacketSize);
	Info->h.sizeL = LOBYTE(PacketSize);

	Info->aIndex = aIndex;
	memcpy(Info->AccountID, gObj[aIndex].AccountID, 11);
	Info->Result = 1;

	cDBSMng.Send(Buff, PacketSize);
}
// -------------------------------------------------------------------------------

#endif