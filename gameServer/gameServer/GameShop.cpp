#include "StdAfx.h"
#include "user.h"
#include "GameShop.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "logproc.h"
#include "CashItemPeriodSystem.h"
#include "PeriodItemEx.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "BuffScriptLoader.h"
#include "DBSockMng.h"
// -------------------------------------------------------------------------------

GameShop	gGameShop;
// -------------------------------------------------------------------------------

GameShop::GameShop()
{
	this->OptionData.ActiveGameShop	= 1;
	this->OptionData.AddGPBonus		= 0;
	// ----
	this->OptionData.ScriptMajor	= 0;
	this->OptionData.ScriptMinor	= 0;
	this->OptionData.ScriptRevision	= 0;
	this->OptionData.BannerMajor	= 0;
	this->OptionData.BannerMinor	= 0;
	this->OptionData.BannerRevision	= 0;
	// ----
	this->UserCount					= 0;
	// ----
	ZeroMemory(this->PointCategory, sizeof(this->PointCategory));
}
// -------------------------------------------------------------------------------

GameShop::~GameShop()
{

}
// -------------------------------------------------------------------------------

void GameShop::Load()	//-> Complete
{
	char * OptionFile	= gDirPath.GetNewPath("GameShop\\GameShop.ini");
	// ----
	this->OptionData.ActiveGameShop	= GetPrivateProfileIntA("Main", "Active", 1, OptionFile);
	this->OptionData.AddGPBonus		= GetPrivateProfileIntA("GoblinPointBonus", "AddBonus", 0, OptionFile);
	// ----
	this->OptionData.ScriptMajor	= GetPrivateProfileIntA("ScriptVersion", "Major", 512, OptionFile);
	this->OptionData.ScriptMinor	= GetPrivateProfileIntA("ScriptVersion", "Minor", 2012, OptionFile);
	this->OptionData.ScriptRevision	= GetPrivateProfileIntA("ScriptVersion", "Revision", 10, OptionFile);
	this->OptionData.BannerMajor	= GetPrivateProfileIntA("BannerVersion", "Major", 512, OptionFile);
	this->OptionData.BannerMinor	= GetPrivateProfileIntA("BannerVersion", "Minor", 2010, OptionFile);
	this->OptionData.BannerRevision	= GetPrivateProfileIntA("BannerVersion", "Revision", 5, OptionFile);
	// ----
	if( !this->OptionData.ActiveGameShop )
	{
		LogAdd("[GameShop] GameShop is disabled");
		return;
	}
	// ----
	this->ItemList.clear();
	this->PackageList.clear();
	ZeroMemory(this->PointCategory, sizeof(this->PointCategory));
	// ----
	this->ReadCategoryList(gDirPath.GetNewPath("GameShop\\GameShopCategory.txt"));
	this->ReadItemList(gDirPath.GetNewPath("GameShop\\GameShopItem.txt"));
	this->ReadPackageList(gDirPath.GetNewPath("GameShop\\GameShopPackage.txt"));
}
// -------------------------------------------------------------------------------

void GameShop::ReadItemList(char * File)	//-> Complete
{
	SMDToken Token;
	SMDFile = fopen(File, "r");	//ok
	// ----
	if( !SMDFile )
	{
		MsgBox("[GameShop] %s file not found", File);
		return;
	}
	// ----
	GAMESHOP_ITEM_DATA		pItem;
	this->ItemCount			= 0;
	int Category			= -1;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		Category = TokenNumber;
		// ----
		while(true)
		{
			if( Category == 0 )	//-> Item list
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				pItem.ItemID		= TokenNumber;
				// ----
				pItem.X				= ItemAttribute[pItem.ItemID].Width;
				pItem.Y				= ItemAttribute[pItem.ItemID].Height;
				// ----
				Token				= GetToken();
				pItem.Level			= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.Durability	= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.Skill			= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.Luck			= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.Option		= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.Excellent		= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.Duration		= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.Price			= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.GPBonus		= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.IBSID			= TokenNumber;
				// ----
				Token				= GetToken();
				pItem.IBSCategory	= TokenNumber;
				// ----
				pItem.PointType		= this->GetPointType(pItem.IBSCategory);
				// ----
				Token				= GetToken();
				pItem.IBSIndex		= TokenNumber;
				// ----
				this->InsertItem(&pItem, this->ItemCount);
				this->ItemCount++;
			}
		}
	}
	// ----
	fclose(SMDFile);
	LogAddTD("[GameShop] %s file is loaded [%d] items", File, this->ItemCount);
}
// -------------------------------------------------------------------------------

void GameShop::ReadPackageList(char * File)	//-> Complete
{
	SMDToken Token;
	SMDFile = fopen(File, "r");	//ok
	// ----
	if( !SMDFile )
	{
		MsgBox("[GameShop] %s file not found", File);
		return;
	}
	// ----
	GAMESHOP_PACKAGE_DATA	pPackage;
	GAMESHOP_ITEM_DATA		pPart;
	this->PackageCount		= 0;
	int Category			= -1;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		Category = TokenNumber;
		// ----
		while(true)
		{
			if( Category == 0 )	//-> Package list
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				pPackage.PackageID		= TokenNumber;
				// ----
				Token = GetToken();
				pPackage.IBSCategory	= TokenNumber;
				// ----
				Token = GetToken();
				pPackage.Price			= TokenNumber;
				// ----
				pPackage.PointType		= this->GetPointType(pPackage.IBSCategory);
				// ----
				Token = GetToken();
				pPackage.GPBonus		= TokenNumber;
				// ----
				this->InsertPackage(&pPackage, this->PackageCount);
				// ----
				this->PackageCount++;
			}
			else if( Category > 0 && Category < 1000 )
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				pPart.PackageID		= Category;
				// ----
				pPart.ItemID		= TokenNumber;
				// ----
				pPart.X				= ItemAttribute[pPart.ItemID].Width;
				pPart.Y				= ItemAttribute[pPart.ItemID].Height;
				// ----
				Token = GetToken();
				pPart.Level			= TokenNumber;
				// ----
				Token = GetToken();
				pPart.Durability	= TokenNumber;
				// ----
				Token = GetToken();
				pPart.Skill			= TokenNumber;
				// ----
				Token = GetToken();
				pPart.Luck			= TokenNumber;
				// ----
				Token = GetToken();
				pPart.Option		= TokenNumber;
				// ----
				Token = GetToken();
				pPart.Excellent		= TokenNumber;
				// ----
				Token = GetToken();
				pPart.Duration		= TokenNumber;
				// ----
				Token = GetToken();
				pPart.IBSID			= TokenNumber;
				// ----
				this->InsertPackagePart(&pPart);
			}
		}
	}
	// ----
	fclose(SMDFile);
	LogAddTD("[GameShop] %s file is loaded [%d] packages", File, this->PackageCount);
}
// -------------------------------------------------------------------------------

void GameShop::ReadCategoryList(char * File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[GameShop] %s file not found", File);
		return;
	}
	// ----
	BYTE WCoinCCounter = 0, WCoinPCounter = 0, GPCounter = 0;
	int Category = -1;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		Category = TokenNumber;
		// ----
		while(true)
		{
			if( Category == 0 )	//-> WCoinC
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->PointCategory[Category][WCoinCCounter] = (int)TokenNumber;
				WCoinCCounter++;
			}
			else if( Category == 1 )
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->PointCategory[Category][WCoinPCounter] = (int)TokenNumber;
				WCoinPCounter++;
			}
			else if( Category == 2 )
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->PointCategory[Category][GPCounter] = (int)TokenNumber;
				GPCounter++;
			}
		}
	}
	// ----
	fclose(SMDFile);
	LogAddTD("[GameShop] %s file is loaded", File);
}
// -------------------------------------------------------------------------------

void GameShop::InsertItem(GAMESHOP_ITEM_DATA * lpItem, int Index)	//-> Complete
{
	GAMESHOP_ITEM_DATA * lpTempItem = new GAMESHOP_ITEM_DATA;
	memcpy(lpTempItem, lpItem, sizeof(GAMESHOP_ITEM_DATA));
	this->ItemList.insert(std::pair<int, GAMESHOP_ITEM_DATA *>(Index, lpTempItem));
}
// -------------------------------------------------------------------------------

void GameShop::InsertPackage(GAMESHOP_PACKAGE_DATA * lpPackage, int Index)	//-> Complete
{
	GAMESHOP_PACKAGE_DATA * lpTempPackage = new GAMESHOP_PACKAGE_DATA;
	memcpy(lpTempPackage, lpPackage, sizeof(GAMESHOP_PACKAGE_DATA));
	this->PackageList.insert(std::pair<int, GAMESHOP_PACKAGE_DATA *>(Index, lpTempPackage));
}
// -------------------------------------------------------------------------------

void GameShop::InsertPackagePart(GAMESHOP_ITEM_DATA * lpItem)	
{
	int Index = this->GetPackageIndex(lpItem->PackageID);
	// ----
	if( Index == -1 )
	{
		LogAddC(3, "[GameShop] [%s] error", __FUNCTION__);
		return;
	}
	// ----
	int ItemPos = this->PackageList[Index]->ItemCount;
	// ----
	PackageList.at(Index)->Item[ItemPos].PackageID		= lpItem->PackageID;
	PackageList.at(Index)->Item[ItemPos].ItemID			= lpItem->ItemID;
	PackageList.at(Index)->Item[ItemPos].X				= lpItem->X;
	PackageList.at(Index)->Item[ItemPos].Y				= lpItem->Y;
	PackageList.at(Index)->Item[ItemPos].Level			= lpItem->Level;
	PackageList.at(Index)->Item[ItemPos].Durability		= lpItem->Durability;
	PackageList.at(Index)->Item[ItemPos].Skill			= lpItem->Skill;
	PackageList.at(Index)->Item[ItemPos].Luck			= lpItem->Luck;
	PackageList.at(Index)->Item[ItemPos].Option			= lpItem->Option;
	PackageList.at(Index)->Item[ItemPos].Excellent		= lpItem->Excellent;
	PackageList.at(Index)->Item[ItemPos].Duration		= lpItem->Duration;
	PackageList.at(Index)->Item[ItemPos].IBSID			= lpItem->IBSID;
	// ----
	this->PackageList.at(Index)->ItemCount++;
}
// -------------------------------------------------------------------------------

void GameShop::SendVersion(int aIndex)	//-> Complete
{
	GAMESHOP_INFO_VERSION pScript;
	GAMESHOP_INFO_VERSION pBanner;
	// ----
	PHeadSubSetB((LPBYTE)&pScript, 0xD2, cScriptVer, sizeof(pScript));
	PHeadSubSetB((LPBYTE)&pBanner, 0xD2, cBannerVer, sizeof(pBanner));
	// ----
	pScript.Major		= this->OptionData.ScriptMajor;
	pScript.Minor		= this->OptionData.ScriptMinor;
	pScript.Revision	= this->OptionData.ScriptRevision;
	// ----
	pBanner.Major		= this->OptionData.BannerMajor;
	pBanner.Minor		= this->OptionData.BannerMinor;
	pBanner.Revision	= this->OptionData.BannerRevision;
	// -----
	DataSend(aIndex, (LPBYTE)&pScript, sizeof(pScript));
	DataSend(aIndex, (LPBYTE)&pBanner, sizeof(pBanner));
}
// -------------------------------------------------------------------------------

void GameShop::AddUser(int aIndex)	//-> Complete
{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] [%s] [%s]", __FUNCTION__, &gObj[aIndex].AccountID, &gObj[aIndex].Name);
#endif
	// ----
	if( !this->OptionData.ActiveGameShop )
	{
		return;
	}
	// ----
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	/*if( this->UserCount >= OBJMAXUSER || this->GetUserSlot(aIndex) >= 0 )
	{
		this->UserCount = OBJMAXUSER;
		return;
	}
	// ----
	this->UserCount++;
	// ----
	ZeroMemory(&this->UserData[this->UserCount], sizeof(&this->UserData[this->UserCount]));
	this->UserData[this->UserCount].aIndex = aIndex;*/
	// ----
	this->SendVersion(aIndex);
	this->GDReqUserInfo(aIndex);
	this->GDReqStorage(aIndex);
	this->RequestPoint(aIndex);
}
// -------------------------------------------------------------------------------

void GameShop::InitUser(int aIndex)
{
	gObj[aIndex].GameShop.WCoinP		= 0;
	gObj[aIndex].GameShop.WCoinC		= 0;
	gObj[aIndex].GameShop.GoblinPoint	= 0;
	// ----
	for( int i = 0; i < MAX_STORAGE_ITEM; i++ )
	{
		gObj[aIndex].GameShop.MainStorage[i].AuthID			= 0;
		gObj[aIndex].GameShop.MainStorage[i].AuthIndex		= 0;
		gObj[aIndex].GameShop.MainStorage[i].IBSCategory	= 0;
		gObj[aIndex].GameShop.MainStorage[i].IBSID			= 0;
		gObj[aIndex].GameShop.MainStorage[i].IBSIndex		= 0;
		gObj[aIndex].GameShop.MainStorage[i].ItemID			= 0;
		gObj[aIndex].GameShop.MainStorage[i].PackageID		= 0;
	}
}
// -------------------------------------------------------------------------------

void GameShop::RequestPoint(int aIndex)	//-> Complete
{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] [%s] [%s]", __FUNCTION__, &gObj[aIndex].AccountID, &gObj[aIndex].Name);
#endif
	// ----
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	this->GDReqUserInfo(aIndex);
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	GAMESHOP_ANS_POINT pPoint;
	PHeadSubSetB((LPBYTE)&pPoint, 0xD2, cPoint, sizeof(GAMESHOP_ANS_POINT));
	// ----
	pPoint.WCoinC[0]	= lpUser->GameShop.WCoinC;
	pPoint.WCoinC[1]	= lpUser->GameShop.WCoinC;
	pPoint.WCoinP[0]	= lpUser->GameShop.WCoinP;
	pPoint.WCoinP[1]	= lpUser->GameShop.WCoinP;
	pPoint.GoblinPoint	= lpUser->GameShop.GoblinPoint;
	// ----
	DataSend(aIndex, (LPBYTE)&pPoint, pPoint.h.size);
}
// -------------------------------------------------------------------------------

void GameShop::RequestOpenShop(int aIndex, GAMESHOP_REQ_OPEN * lpRequest)	//-> Complete
{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] [%s] [%s]", __FUNCTION__, &gObj[aIndex].AccountID, &gObj[aIndex].Name);
#endif
	// ----
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	GAMESHOP_REQ_OPEN pOpenShop;
	PHeadSubSetB((LPBYTE)&pOpenShop, 0xD2, cShopOpen, sizeof(pOpenShop));
	// ----
	pOpenShop.Result = 0;
	// ----
	if( !this->OptionData.ActiveGameShop )
	{
		pOpenShop.Result = 1;
	}
	// ----
	if( lpRequest->Result == 0 )
	{
		this->GDReqUserInfo(aIndex);
		// ----
		if( lpUser->m_IfState.use > 0 )
		{
			pOpenShop.Result = 8;
		}
		// ----
		if( pOpenShop.Result == 0 )
		{
			lpUser->m_IfState.use	= 1;
			lpUser->m_IfState.type	= 18;
			lpUser->m_IfState.state	= 1;
			pOpenShop.Result		= 1;
		}
	}
	else if( lpRequest->Result == 1 )
	{
		lpUser->m_IfState.use		= 0;
		lpUser->m_IfState.type		= 0;
		lpUser->m_IfState.state		= 0;
		pOpenShop.Result			= 0;
	}
	// ----
	DataSend(aIndex, (LPBYTE)&pOpenShop, sizeof(pOpenShop));
}
// -------------------------------------------------------------------------------

void GameShop::RequestItemBuy(int aIndex, GAMESHOP_REQ_BUY * lpRequest)
{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(3, "[DEBUG] [%s] %d %d %d [%d %d]", __FUNCTION__, 
		lpRequest->ItemID, lpRequest->IBSCategory, lpRequest->IBSIndex, lpRequest->IBSPackageID, lpRequest->Unknown18);
#endif
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	GAMESHOP_ANS_BUY pBuy;
	PHeadSubSetB((LPBYTE)&pBuy, 0xD2, cBuy, sizeof(pBuy));
	// ----
	if( !gObjIsConnected(aIndex) || lpUser->Connected != PLAYER_PLAYING )
	{
		return;
	}
	// ----
	LogAddTD("[GameShop] [RequestItemBuy] [%s] %d %d %d [%d %d]",
		&gObj[aIndex].AccountID, lpRequest->ItemID, 
		lpRequest->IBSCategory, lpRequest->IBSIndex, 
		lpRequest->IBSPackageID, lpRequest->Unknown18);
	// ----
	if( !CanBeBuy(aIndex, lpRequest->ItemID) )
	{
		LogAddTD("[GameShop] [RequestItemBuy] [%s] !CanBeBuy", &gObj[aIndex].AccountID);
		pBuy.Result = true;
		DataSend(aIndex, (LPBYTE)&pBuy, pBuy.h.size);
		return;
	}
	// ----
	if( this->RequestPackageBuy(aIndex, lpRequest) )
	{
		return;
	}
	// ----
	GAMESHOP_ITEM_DATA * lpItem = this->GetItem(lpRequest->ItemID, lpRequest->IBSCategory, lpRequest->IBSIndex, 0);
	// ----
	if( !lpItem )
	{
		return;
	}
	// ----
	pBuy.Result = true;
	// ----
	switch(lpItem->PointType)
	{
	case 0:	//-> WCoinC
		{
			if( lpUser->GameShop.WCoinC >= lpItem->Price )
			{
				lpUser->GameShop.WCoinC	-= lpItem->Price;
				pBuy.Result = false;
			}
		}
		break;
		// --
	case 1:	//-> WCoinP
		{
			if( lpUser->GameShop.WCoinP >= lpItem->Price )
			{
				lpUser->GameShop.WCoinP	-= lpItem->Price;
				pBuy.Result = false;
			}
		}
		break;
		// --
	case 2:	//-> GoblinPoint
		{
			if( lpUser->GameShop.GoblinPoint >= lpItem->Price )
			{
				lpUser->GameShop.GoblinPoint -= lpItem->Price;
				pBuy.Result = false;
			}
		}
		break;
	}
	// ----
	if( !pBuy.Result )
	{
		if( this->AddItemInStorage(aIndex, lpItem, 0) )
		{
			LogAddTD("[GameShop] [RequestItemBuy] [ADD] [%s] (%d) %d %d %d",
				&gObj[aIndex].AccountID, lpItem->ItemID, 
				lpItem->IBSCategory, lpItem->IBSIndex, 
				lpItem->IBSID);
		}
		// ----
		if( lpItem->GPBonus )
		{
			lpUser->GameShop.GoblinPoint += lpItem->GPBonus;
		}
		else if( this->OptionData.AddGPBonus )
		{
			lpUser->GameShop.GoblinPoint += this->OptionData.AddGPBonus;
		}
		// ----
		GAMESHOP_REQ_STORAGE pUpdate;
		pUpdate.StorageType = 0x53;
		// ----
		this->GDSaveUserInfo(aIndex);
		this->RequestStorage(aIndex, &pUpdate);
		this->RequestPoint(aIndex);
	}
	// ----
	DataSend(aIndex, (LPBYTE)&pBuy, pBuy.h.size);
}
// -------------------------------------------------------------------------------

bool GameShop::RequestPackageBuy(int aIndex, GAMESHOP_REQ_BUY * lpRequest)
{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(3, "[DEBUG] [%s] %d %d %d [%d %d]", __FUNCTION__, 
		lpRequest->ItemID, lpRequest->IBSCategory, lpRequest->IBSIndex, lpRequest->IBSPackageID, lpRequest->Unknown18);
#endif
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	if( !gObjIsConnected(aIndex) || lpUser->Connected != PLAYER_PLAYING )
	{
		return false;
	}
	// ----
	GAMESHOP_PACKAGE_DATA * lpPackage = this->GetPackage(lpRequest->IBSPackageID);
	// ----
	if( !lpPackage )
	{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Invalid package", __FUNCTION__);
#endif
		return false;
	}
	// ----
	GAMESHOP_ANS_BUY pBuy;
	PHeadSubSetB((LPBYTE)&pBuy, 0xD2, cBuy, sizeof(pBuy));
	// ----
	pBuy.Result = true;
	// ----
	switch(lpPackage->PointType)
	{
	case 0:	//-> WCoinC
		{
			if( lpUser->GameShop.WCoinC >= lpPackage->Price )
			{
				lpUser->GameShop.WCoinC	-= lpPackage->Price;
				pBuy.Result = false;
			}
		}
		break;
		// --
	case 1:	//-> WCoinP
		{
			if( lpUser->GameShop.WCoinP >= lpPackage->Price )
			{
				lpUser->GameShop.WCoinP	-= lpPackage->Price;
				pBuy.Result = false;
			}
		}
		break;
		// --
	case 2:	//-> GoblinPoint
		{
			if( lpUser->GameShop.GoblinPoint >= lpPackage->Price )
			{
				lpUser->GameShop.GoblinPoint -= lpPackage->Price;
				pBuy.Result = false;
			}
		}
		break;
	}
	// ----
	if( !pBuy.Result )
	{
		for( int i = 0; i < lpPackage->ItemCount; i++ )
		{
			this->AddPackageInStorage(aIndex, &lpPackage->Item[i], 0);
		}
		// ----
		if( lpPackage->GPBonus )
		{
			lpUser->GameShop.GoblinPoint += lpPackage->GPBonus;
		}
		else if( this->OptionData.AddGPBonus )
		{
			lpUser->GameShop.GoblinPoint += this->OptionData.AddGPBonus;
		}
		// ----
		GAMESHOP_REQ_STORAGE pUpdate;
		pUpdate.StorageType = 0x53;
		// ----
		this->GDSaveUserInfo(aIndex);
		this->RequestStorage(aIndex, &pUpdate);
		this->RequestPoint(aIndex);
	}
	// ----
	DataSend(aIndex, (LPBYTE)&pBuy, pBuy.h.size);
	return true;
}
// -------------------------------------------------------------------------------

void GameShop::RequestStorage(int aIndex, GAMESHOP_REQ_STORAGE * lpRequest)
{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] [%s] [%s]", __FUNCTION__, &gObj[aIndex].AccountID, &gObj[aIndex].Name);
#endif
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	GAMESHOP_ANS_STORAGE_COUNT	pStorageCount = { 0 };
	GAMESHOP_ANS_STORAGE_ITEM	pStorageItem;
	// ----
	PHeadSubSetB((LPBYTE)&pStorageCount, 0xD2, cStorageCount, sizeof(pStorageCount));
	PHeadSubSetB((LPBYTE)&pStorageItem, 0xD2, cStorageItem, sizeof(pStorageItem));
	// ----
	int ItemCount = this->GetItemCountInStorage(aIndex, lpRequest->StorageType);
	// ----
	if( ItemCount )
	{
		pStorageCount.MainStorage[0] = ItemCount+1;
		pStorageCount.MainStorage[1] = ItemCount+1;
		pStorageCount.GiftStorage[0] = 1;
		pStorageCount.GiftStorage[1] = 1;
	}
	// ----
	DataSend(aIndex, (LPBYTE)&pStorageCount, pStorageCount.h.size);
	// ----
	pStorageItem.Unk3		= 0x50;
	pStorageItem.IBSUnknown	= 673;
	// ----
	int Counter = 0;
	// ----
	for( int i = 0; i < MAX_STORAGE_ITEM; i++ )
	{
		if( lpRequest->StorageType == 0x53 && lpUser->GameShop.MainStorage[i].AuthIndex != 0 && lpUser->GameShop.MainStorage[i].AuthID != 0 )
		{
			pStorageItem.AuthIndex	= lpUser->GameShop.MainStorage[i].AuthIndex;
			pStorageItem.AuthID		= lpUser->GameShop.MainStorage[i].AuthID;
			pStorageItem.IBSID		= lpUser->GameShop.MainStorage[i].IBSID;
			pStorageItem.IBSIndex	= lpUser->GameShop.MainStorage[i].IBSIndex;
			Counter++;
		}
		/*else if( lpRequest->StorageType == 0x47 && lpUser->GameShop.GiftStorage[i].AuthIndex != 0 && lpUser->GameShop.GiftStorage[i].AuthID != 0 )
		{
			pStorageItem.AuthIndex	= lpUser->GameShop.GiftStorage[i].AuthIndex;
			pStorageItem.AuthID		= lpUser->GameShop.GiftStorage[i].AuthID;
			pStorageItem.IBSID		= lpUser->GameShop.GiftStorage[i].IBSID;
			pStorageItem.IBSIndex	= lpUser->GameShop.GiftStorage[i].IBSIndex;
			Counter++;
		}*/
		else
		{
			continue;
		}
		// ----
		DataSend(aIndex, (LPBYTE)&pStorageItem, pStorageItem.h.size);
		// ----
		if( Counter >= ItemCount )
		{
			break;
		}
	}
}
// -------------------------------------------------------------------------------

void GameShop::RequestStorageItemUse(int aIndex, GAMESHOP_REQ_STORAGEITEMUSE * lpRequest)
{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] [%s] [%s]", __FUNCTION__, &gObj[aIndex].AccountID, &gObj[aIndex].Name);
#endif
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	if( !gObjIsConnected(aIndex) || lpUser->Connected != PLAYER_PLAYING )
	{
		return;
	}
	// ----
	GAMESHOP_ANS_STORAGE_ITEM_USE pAnswer;
	PHeadSubSetB((LPBYTE)&pAnswer, 0xD2, cStorageItemUse, sizeof(pAnswer));
	// ----
	GAMESHOP_ITEM_DATA * lpItem = this->GetStorageItem(aIndex, lpRequest->AuthIndex, lpRequest->AuthID);
	// ----
	if( !lpItem || !CheckInventoryEmptySpace(lpUser, lpItem->X, lpItem->Y) )
	{
		pAnswer.Result = true;
		DataSend(aIndex, (LPBYTE)&pAnswer, pAnswer.h.size);
		LogAddC(3, "[GameShop] %d", __LINE__);
		return;
	}
	// ----
	if( !DeleteItemInStorage(aIndex, lpRequest->AuthIndex, lpRequest->AuthID) )
	{
		LogAddC(3, "[GameShop] %d", __LINE__);
		return;
	}
	// ----
	if( g_BuffScript.GetBuffDataFromItemCode(lpItem->ItemID) //-> Buff
		&& !this->IsInventoryBuff(lpItem->ItemID) )	
	{
		g_CashItemPeriodSystem.SearchAndDeleteItemPeriodEffect(lpUser, lpItem->ItemID);	//fix
		g_CashItemPeriodSystem.GDReqPeriodItemInsert(lpUser, lpItem->ItemID, lpItem->Duration);
	}
	else if( lpItem->Duration > 0 )	//-> Period item
	{
		ItemSerialCreateSend(aIndex, 236, 0, 0, lpItem->ItemID, lpItem->Level, lpItem->Durability, 
			lpItem->Skill, lpItem->Luck, lpItem->Option, lpItem->Duration * 60, lpItem->Excellent, 0);
	}
	else	//-> Simply item
	{
		ItemSerialCreateSend(aIndex, 235, 0, 0, lpItem->ItemID, lpItem->Level, lpItem->Durability, 
			lpItem->Skill, lpItem->Luck, lpItem->Option, aIndex, lpItem->Excellent, 0);
	}
	// ----
	pAnswer.Result = false;
	DataSend(aIndex, (LPBYTE)&pAnswer, pAnswer.h.size);
	// ----
	this->GDSaveUserInfo(aIndex);
}
// -------------------------------------------------------------------------------

BYTE GameShop::GetPointType(int IBSCategory)	//-> Complete
{
	for( int i = 0; i < 30; i++ )
	{
		if( this->PointCategory[0][i] == IBSCategory )
		{
			return 0;
		}
		// ----
		if( this->PointCategory[1][i] == IBSCategory )
		{
			return 1;
		}
		// ----
		if( this->PointCategory[2][i] == IBSCategory )
		{
			return 2;
		}
	}
	// ----
	/*switch(IBSCategory)
	{
	case 10: case 13: case 14:
	case 22: case 23: case 24:
	case 25: case 26:
		return 0;
		break;
		// --
	case 11: case 27: case 28:
	case 29: case 30: case 31:
	case 32: case 46:
		return 1;
		break;
		// --
	case 57: case 56: case 34:
	case 45:
		return 2;
		break;
	}*/
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

bool GameShop::AddItemInStorage(int aIndex, GAMESHOP_ITEM_DATA * lpItem, BYTE Type)	//-> Complete
{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] [%s] [%s]", __FUNCTION__, &gObj[aIndex].AccountID, &gObj[aIndex].Name);
#endif
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	if( Type == 0 )	//-> Main Storage
	{
		for( int i = 0; i < MAX_STORAGE_ITEM; i++ )
		{
			if( lpUser->GameShop.MainStorage[i].AuthIndex == 0 && lpUser->GameShop.MainStorage[i].AuthID == 0 )
			{
				lpUser->GameShop.MainStorage[i].AuthIndex	= rand()%10000+4400/200;
				lpUser->GameShop.MainStorage[i].AuthID		= rand()%250000+1700/12;
				lpUser->GameShop.MainStorage[i].ItemID		= lpItem->ItemID;
				lpUser->GameShop.MainStorage[i].IBSID		= lpItem->IBSID;
				lpUser->GameShop.MainStorage[i].IBSCategory	= lpItem->IBSCategory;
				lpUser->GameShop.MainStorage[i].IBSIndex	= lpItem->IBSIndex;
				GDSaveStorageItem(aIndex, lpUser->GameShop.MainStorage[i]);
				// ----
				return true;
			}
		}
	}
	/*else if( Type == 1 )	//-> Gift Storage
	{
		for( int i = 0; i < MAX_STORAGE_ITEM; i++ )
		{
			if( lpUser->GameShop.GiftStorage[i].AuthIndex == 0 && lpUser->GameShop.GiftStorage[i].AuthID == 0 )
			{
				lpUser->GameShop.GiftStorage[i].AuthIndex	= rand()%10000+4400/200;
				lpUser->GameShop.GiftStorage[i].AuthID		= rand()%250000+1700/12;
				lpUser->GameShop.GiftStorage[i].ItemID		= lpItem->ItemID;
				lpUser->GameShop.GiftStorage[i].IBSID		= lpItem->IBSID;
				lpUser->GameShop.GiftStorage[i].IBSCategory	= lpItem->IBSCategory;
				lpUser->GameShop.GiftStorage[i].IBSIndex	= lpItem->IBSIndex;
				// ----
				return true;
			}
		}
	}*/
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool GameShop::AddPackageInStorage(int aIndex, GAMESHOP_ITEM_DATA * lpItem, BYTE Type)	//-> Complete
{
#if( GAMESHOP_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] [%s] [%s]", __FUNCTION__, &gObj[aIndex].AccountID, &gObj[aIndex].Name);
#endif
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	if( Type == 0 )	//-> Main Storage
	{
		for( int i = 0; i < MAX_STORAGE_ITEM; i++ )
		{
			if( lpUser->GameShop.MainStorage[i].AuthIndex == 0 && lpUser->GameShop.MainStorage[i].AuthID == 0 )
			{
				lpUser->GameShop.MainStorage[i].AuthIndex	= rand()%10000+4400/200;
				lpUser->GameShop.MainStorage[i].AuthID		= rand()%250000+1700/12;
				lpUser->GameShop.MainStorage[i].ItemID		= lpItem->ItemID;
				lpUser->GameShop.MainStorage[i].IBSID		= lpItem->IBSID;
				lpUser->GameShop.MainStorage[i].IBSCategory	= 0;
				lpUser->GameShop.MainStorage[i].IBSIndex	= 0;
				lpUser->GameShop.MainStorage[i].PackageID	= lpItem->PackageID;
				GDSaveStorageItem(aIndex, lpUser->GameShop.MainStorage[i]);
				// ----
				return true;
			}
		}
	}
	/*else if( Type == 1 )	//-> Gift Storage
	{
		for( int i = 0; i < MAX_STORAGE_ITEM; i++ )
		{
			if( lpUser->GameShop.GiftStorage[i].AuthIndex == 0 && lpUser->GameShop.GiftStorage[i].AuthID == 0 )
			{
				lpUser->GameShop.GiftStorage[i].AuthIndex	= rand()%10000+4400/200;
				lpUser->GameShop.GiftStorage[i].AuthID		= rand()%250000+1700/12;
				lpUser->GameShop.GiftStorage[i].ItemID		= lpItem->ItemID;
				lpUser->GameShop.GiftStorage[i].IBSID		= lpItem->IBSID;
				lpUser->GameShop.GiftStorage[i].IBSCategory	= 0;
				lpUser->GameShop.GiftStorage[i].IBSIndex	= 0;
				lpUser->GameShop.GiftStorage[i].PackageID	= lpItem->PackageID;
				// ----
				return true;
			}
		}
	}*/
	// ----
	return false;
}
// -------------------------------------------------------------------------------

int GameShop::GetItemCountInStorage(int aIndex, int ClientStorageType)	//-> Complete
{
	int Count		= 0;
	LPOBJ lpUser	= &gObj[aIndex];
	// ----
	for( int i = 0; i < MAX_STORAGE_ITEM; i++ )
	{
		if( ClientStorageType == 0x53 )
		{
			if( lpUser->GameShop.MainStorage[i].AuthIndex != 0 && lpUser->GameShop.MainStorage[i].AuthID != 0 )
			{
				Count++;
			}
		}
		else if( ClientStorageType == 0x47 )
		{
			//if( lpUser->GameShop.GiftStorage[i].AuthIndex != 0 && lpUser->GameShop.GiftStorage[i].AuthID != 0 )
			//{
			//	Count++;
			//}
		}
	}
	// ----
	return Count;
}
// -------------------------------------------------------------------------------

bool GameShop::DeleteItemInStorage(int aIndex, int AuthIndex, int AuthID)	//-> Complete
{
	GAMESHOP_REQ_STORAGE pUpdate = { 0 };
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	for( int i = 0; i < MAX_STORAGE_ITEM; i++ )
	{
		if( lpUser->GameShop.MainStorage[i].AuthIndex == AuthIndex && lpUser->GameShop.MainStorage[i].AuthID == AuthID )
		{
			pUpdate.StorageType = 0x53;
			GDDeleteStorageItem(aIndex, lpUser->GameShop.MainStorage[i]);
			ZeroMemory(&lpUser->GameShop.MainStorage[i], sizeof(lpUser->GameShop.MainStorage[i]));
			this->RequestStorage(aIndex, &pUpdate);
			return true;
		}
		//else if( lpUser->GameShop.GiftStorage[i].AuthIndex == AuthIndex && lpUser->GameShop.GiftStorage[i].AuthID == AuthID )
		//{
		//	pUpdate.StorageType = 0x47;
		//	ZeroMemory(&lpUser->GameShop.GiftStorage[i], sizeof(lpUser->GameShop.GiftStorage[i]));
		//	this->RequestStorage(aIndex, &pUpdate);
		//	return true;
		//}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool GameShop::IsInventoryBuff(short ItemID)
{
	if( ItemID >= ITEMGET(13, 128) && ItemID <= ITEMGET(13, 134) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool GameShop::CanBeBuy(int aIndex, short ItemID)
{
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	if( GetItemCountInStorage(aIndex, 0x53) >= MAX_STORAGE_ITEM - 1 )
	{
		GCServerMsgStringSend("Your GameShop inventory is full", aIndex, 1);
		return false;
	}
	// ----
	if( ItemID == ITEMGET(13, 93) || ItemID == ITEMGET(13, 94) || ItemID == ITEMGET(14, 114) )
	{
		if( !lpUser->ThirdChangeUp )
		{
			GCServerMsgStringSend("You need 3rd class to buy master items", aIndex, 1);
			return false;
		}
	}
	else if( ItemID == ITEMGET(13, 138) || ItemID == ITEMGET(13, 143) )
	{
		if( lpUser->Class == CLASS_MONK )
		{
			GCServerMsgStringSend("This ticket not have items for you", aIndex, 1);
			return false;
		}
	}
	else if( ItemID == ITEMGET(14, 162) )
	{
		if( lpUser->ExpandedInventory >= 2 )
		{
			GCServerMsgStringSend("Your inventory expanded to maximum", aIndex, 1);
			return false;
		}
	}
	else if( ItemID == ITEMGET(14, 163) )
	{
		if( lpUser->ExpandedWarehouse >= 1 )
		{
			GCServerMsgStringSend("Your warehouse expanded to maximum", aIndex, 1);
			return false;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

GAMESHOP_ITEM_DATA * GameShop::GetItem(short ItemID, int IBSCategory, int IBSIndex, int PackageID)
{
	GAMESHOP_ITEM_DATA * lpItem = NULL;
	std::map<int, GAMESHOP_ITEM_DATA *>::iterator Iter;
	// ----
	if( IBSCategory == 0 && PackageID > 0 )
	{
		GAMESHOP_PACKAGE_DATA * Package = GetPackage(PackageID);
		// ----
		if( Package )
		{
			for( int i = 0; i < Package->ItemCount; i++ )
			{
				if( Package->Item[i].ItemID == ItemID )
				{
					lpItem = &Package->Item[i];
					return lpItem;
				}
			}
		}
	}
	// ----
	for( Iter = this->ItemList.begin(); Iter != this->ItemList.end(); Iter++ )
	{
		lpItem = Iter->second;
		// ----
		if( ItemID == lpItem->ItemID && IBSCategory == lpItem->IBSCategory && IBSIndex == lpItem->IBSIndex )
		{
			break;
		}
		else
		{
			lpItem = NULL;
			continue;
		}
	}
	// ----
	return lpItem;
}
// -------------------------------------------------------------------------------

GAMESHOP_ITEM_DATA * GameShop::GetStorageItem(int aIndex, int AuthIndex, int AuthID)	//-> Complete
{
	GAMESHOP_ITEM_DATA * lpItem = NULL;
	LPOBJ lpUser				= &gObj[aIndex];
	// ----
	for( int i = 0; i < MAX_STORAGE_ITEM; i++ )
	{
		if( lpUser->GameShop.MainStorage[i].AuthIndex == AuthIndex && lpUser->GameShop.MainStorage[i].AuthID == AuthID)
		{
			lpItem = this->GetItem(lpUser->GameShop.MainStorage[i].ItemID, lpUser->GameShop.MainStorage[i].IBSCategory, 
				lpUser->GameShop.MainStorage[i].IBSIndex, lpUser->GameShop.MainStorage[i].PackageID);
			break;
		}
/*		else if( lpUser->GameShop.GiftStorage[i].AuthIndex == AuthIndex && lpUser->GameShop.GiftStorage[i].AuthID == AuthID)
		{
			lpItem = this->GetItem(lpUser->GameShop.GiftStorage[i].ItemID, lpUser->GameShop.GiftStorage[i].IBSCategory, 
				lpUser->GameShop.GiftStorage[i].IBSIndex, lpUser->GameShop.GiftStorage[i].PackageID);
			break;
		}*/
	}
	// ----
	return lpItem;
}
// -------------------------------------------------------------------------------

GAMESHOP_PACKAGE_DATA * GameShop::GetPackage(int PackageID)
{
	GAMESHOP_PACKAGE_DATA * lpPackage = NULL;
	std::map<int, GAMESHOP_PACKAGE_DATA *>::iterator Iter;
	// ----
	for( Iter = this->PackageList.begin(); Iter != this->PackageList.end(); Iter++ )
	{
		lpPackage = Iter->second;
		// ----
		if( PackageID == lpPackage->PackageID )
		{
			break;
		}
		else
		{
			lpPackage = NULL;
			continue;
		}
	}
	// ----
	return lpPackage;
}
// -------------------------------------------------------------------------------

int GameShop::GetPackageIndex(int PackageID)
{
	for( int i = 0; i < this->PackageCount; i++ )
	{
		if( this->PackageList[i]->PackageID == PackageID )
		{
			return i;
			break;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

void GameShop::GDSaveStorageItem(int aIndex, GAMESHOP_USER_STORAGE Item)
{
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	GAMESHOP_GD_SAVE_STORAGEITEM pRequest;
	PHeadSubSetW((LPBYTE)&pRequest, 0xD3, 1, sizeof(GAMESHOP_GD_SAVE_STORAGEITEM));
	memcpy(&pRequest.Item, &Item, sizeof(pRequest.Item));
	memcpy(pRequest.AccountID, lpUser->AccountID, MAX_IDSTRING);
	pRequest.UserIndex = aIndex;
	cDBSMng.Send((char*)&pRequest, sizeof(GAMESHOP_GD_SAVE_STORAGEITEM));
}
// -------------------------------------------------------------------------------

void GameShop::GDDeleteStorageItem(int aIndex, GAMESHOP_USER_STORAGE Item)
{
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	GAMESHOP_GD_DEL_STORAGEITEM pRequest;
	PHeadSubSetW((LPBYTE)&pRequest, 0xD3, 2, sizeof(GAMESHOP_GD_SAVE_STORAGEITEM));
	memcpy(&pRequest.Item, &Item, sizeof(pRequest.Item));
	memcpy(pRequest.AccountID, lpUser->AccountID, MAX_IDSTRING);
	pRequest.UserIndex = aIndex;
	cDBSMng.Send((char*)&pRequest, sizeof(GAMESHOP_GD_SAVE_STORAGEITEM));
}
// -------------------------------------------------------------------------------

void GameShop::GDReqStorage(int aIndex)
{
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	GAMESHOP_GD_STORAGE pRequest;
	PHeadSubSetW((LPBYTE)&pRequest, 0xD3, 3, sizeof(GAMESHOP_GD_STORAGE));
	memcpy(pRequest.AccountID, lpUser->AccountID, MAX_IDSTRING);
	pRequest.UserIndex = aIndex;
	cDBSMng.Send((char*)&pRequest, sizeof(GAMESHOP_GD_STORAGE));
}
// -------------------------------------------------------------------------------

void GameShop::DGGetStorage(GAMESHOP_GD_STORAGE * aRecv)
{
	if( !gObjIsConnected(aRecv->UserIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpUser = &gObj[aRecv->UserIndex];
	// ----
	for( int i = 0; i < aRecv->ItemCount; i++ )
	{
		lpUser->GameShop.MainStorage[i].AuthIndex = aRecv->Storage[i].AuthIndex;
		lpUser->GameShop.MainStorage[i].AuthID = aRecv->Storage[i].AuthID;
		lpUser->GameShop.MainStorage[i].ItemID = aRecv->Storage[i].ItemID;
		lpUser->GameShop.MainStorage[i].IBSCategory = aRecv->Storage[i].IBSCategory;
		lpUser->GameShop.MainStorage[i].IBSID = aRecv->Storage[i].IBSID;
		lpUser->GameShop.MainStorage[i].IBSIndex = aRecv->Storage[i].IBSIndex;
		lpUser->GameShop.MainStorage[i].PackageID = aRecv->Storage[i].PackageID;
	}
}
// -------------------------------------------------------------------------------

void GameShop::GDSavePoint(int aIndex)
{
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	GAMESHOP_GD_SAVE_POINT pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xD2, 0x3, sizeof(pRequest));
	memcpy(pRequest.AccountID, lpUser->AccountID, 11);
	pRequest.UserIndex		= aIndex;
	pRequest.WCoinC			= lpUser->GameShop.WCoinC;
	pRequest.WCoinP			= lpUser->GameShop.WCoinP;
	pRequest.GoblinPoint	= lpUser->GameShop.GoblinPoint;
	cDBSMng.Send((char*)&pRequest, pRequest.h.size);
}
// -------------------------------------------------------------------------------

void GameShop::GDReqPoint(int aIndex)
{
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	GAMESHOP_GD_REQ_POINT pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xD2, 0x4, sizeof(pRequest));
	memcpy(pRequest.AccountID, lpUser->AccountID, 11);
	pRequest.UserIndex = aIndex;
	cDBSMng.Send((char*)&pRequest, pRequest.h.size);
}
// -------------------------------------------------------------------------------

void GameShop::DGGetPoint(GAMESHOP_DG_GET_POINT * aRecv)
{
	if( !gObjIsConnected(aRecv->UserIndex) )
	{
		LogAddC(2, "[GameShop][%s] Error", __FUNCTION__);
		return;
	}
	// ----
	LPOBJ lpUser = &gObj[aRecv->UserIndex];
	// ----
	if( aRecv->WCoinC < 0 )
	{
		aRecv->WCoinC = 0;
	}
	else if( aRecv->WCoinP < 0 )
	{
		aRecv->WCoinP = 0;
	}
	else if( aRecv->GoblinPoint < 0 )
	{
		aRecv->GoblinPoint = 0;
	}
	// ----
	lpUser->GameShop.WCoinC		= (double)aRecv->WCoinC;
	lpUser->GameShop.WCoinP		= (double)aRecv->WCoinP;
	lpUser->GameShop.GoblinPoint	= (double)aRecv->GoblinPoint;
}
// -------------------------------------------------------------------------------

void GameShop::GDSaveUserInfo(int aIndex)	//-> Complete
{
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	this->GDSavePoint(aIndex);
}
// -------------------------------------------------------------------------------

void GameShop::GDReqUserInfo(int aIndex)	//-> Complete
{
	if( !gObjIsConnected(aIndex) )
	{
		return;
	}
	// ----
	this->GDReqPoint(aIndex);
}
// -------------------------------------------------------------------------------