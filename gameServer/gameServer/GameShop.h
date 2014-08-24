#pragma once
// -------------------------------------------------------------------------------

#include "GameShopProtocol.h"
// -------------------------------------------------------------------------------

#define GAMESHOP_DEBUG		0
#define MAX_STORAGE_ITEM	30
#define MAX_PACKAGE_ITEM	20
// -------------------------------------------------------------------------------

struct GAMESHOP_OPTION_DATA
{
	bool	ActiveGameShop;
	// ----
	short	ScriptMajor;
	short	ScriptMinor;
	short	ScriptRevision;
	short	BannerMajor;
	short	BannerMinor;
	short	BannerRevision;
	// ----
	int		AddGPBonus;
};
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_USER_STORAGE
{
	int		AuthIndex;
	int		AuthID;
	short	ItemID;
	int		IBSID;
	int		IBSCategory;
	int		IBSIndex;
	int		PackageID;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

struct GAMESHOP_USER_DATA
{
	double	WCoinP;
	double	WCoinC;
	double	GoblinPoint;
	// ----
	GAMESHOP_USER_STORAGE MainStorage[MAX_STORAGE_ITEM];
	//GAMESHOP_USER_STORAGE GiftStorage[MAX_STORAGE_ITEM];
};
// -------------------------------------------------------------------------------

struct GAMESHOP_ITEM_DATA
{
	short	PackageID;
	short	ItemID;
	BYTE	X;
	BYTE	Y;
	BYTE	Level;
	BYTE	Durability;
	BYTE	Skill;
	BYTE	Luck;
	BYTE	Option;
	BYTE	Excellent;
	int		Duration;
	int		Price;
	BYTE	PointType;
	int		GPBonus;
	int		IBSID;
	int		IBSCategory;
	int		IBSIndex;
	// ----
	GAMESHOP_ITEM_DATA::GAMESHOP_ITEM_DATA()
	{
		this->PackageID		= -1;
		this->ItemID		= -1;
		this->X				= -1;
		this->Y				= -1;
		this->Level			= 0;
		this->Durability	= -1;
		this->Skill			= 0;
		this->Luck			= 0;
		this->Option		= 0;
		this->Excellent		= 0;
		this->Duration		= 0;
		this->Price			= 0;
		this->PointType		= -1;
		this->GPBonus		= 0;
		this->IBSID			= 0;
		this->IBSCategory	= 0;
		this->IBSIndex		= 0;
	}
};
// -------------------------------------------------------------------------------

struct GAMESHOP_PACKAGE_DATA
{
	short	PackageID;
	int		IBSCategory;
	int		Price;
	BYTE	PointType;
	int		GPBonus;
	BYTE	ItemCount;
	// ----
	GAMESHOP_ITEM_DATA	Item[MAX_PACKAGE_ITEM];
	// ----
	GAMESHOP_PACKAGE_DATA::GAMESHOP_PACKAGE_DATA()
	{
		this->PackageID		= -1;
		this->IBSCategory	= -1;
		this->Price			= 0;
		this->PointType		= -1;
		this->GPBonus		= 0;
		this->ItemCount		= 0;
	}
};
// -------------------------------------------------------------------------------

class GameShop
{
public:
				GameShop();
	virtual		~GameShop();
	// ----
	void		Load();
	void		ReadItemList(char * File);
	void		ReadPackageList(char * File);
	void		ReadCategoryList(char * File);
	// ----
	void		InsertItem(GAMESHOP_ITEM_DATA * lpItem, int Index);
	void		InsertPackage(GAMESHOP_PACKAGE_DATA * lpPackage, int Index);
	void		InsertPackagePart(GAMESHOP_ITEM_DATA * lpItem);
	// ----
	void		SendVersion(int aIndex);
	void		AddUser(int aIndex);
	void		InitUser(int aIndex);
	// ----
	void		RequestPoint(int aIndex);
	void		RequestOpenShop(int aIndex, GAMESHOP_REQ_OPEN * lpRequest);
	void		RequestItemBuy(int aIndex, GAMESHOP_REQ_BUY * lpRequest);
	bool		RequestPackageBuy(int aIndex, GAMESHOP_REQ_BUY * lpRequest);
	void		RequestStorage(int aIndex, GAMESHOP_REQ_STORAGE * lpRequest);
	void		RequestStorageItemUse(int aIndex, GAMESHOP_REQ_STORAGEITEMUSE * lpRequest);
	// ----
	BYTE		GetPointType(int IBSCategory);
	bool		AddItemInStorage(int aIndex, GAMESHOP_ITEM_DATA * lpItem, BYTE Type);
	bool		AddPackageInStorage(int aIndex, GAMESHOP_ITEM_DATA * lpItem, BYTE Type);
	int			GetItemCountInStorage(int aIndex, int ClientStorageType);
	bool		DeleteItemInStorage(int aIndex, int AuthIndex, int AuthID);
	bool		IsInventoryBuff(short ItemID);
	bool		CanBeBuy(int aIndex, short ItemID);
	// ----
	GAMESHOP_ITEM_DATA		*	GetItem(short ItemID, int IBSCategory, int IBSIndex, int PackageID);
	GAMESHOP_ITEM_DATA		*	GetStorageItem(int aIndex, int AuthIndex, int AuthID);
	GAMESHOP_PACKAGE_DATA	*	GetPackage(int PackageID);
	int							GetPackageIndex(int PackageID);
	// ----
	void		GDSaveStorageItem(int aIndex, GAMESHOP_USER_STORAGE Item);
	void		GDDeleteStorageItem(int aIndex, GAMESHOP_USER_STORAGE Item);
	void		GDReqStorage(int aIndex);
	void		DGGetStorage(GAMESHOP_GD_STORAGE * aRecv);
	void		GDSavePoint(int aIndex);
	void		GDReqPoint(int aIndex);
	void		DGGetPoint(GAMESHOP_DG_GET_POINT * aRecv);
	void		GDSaveUserInfo(int aIndex);
	void		GDReqUserInfo(int aIndex);
	// ----
private:
	GAMESHOP_OPTION_DATA					OptionData;
	std::map<int, GAMESHOP_ITEM_DATA*>		ItemList;
	std::map<int, GAMESHOP_PACKAGE_DATA*>	PackageList;
	int UserCount;
	// ----
	int			ItemCount;
	int			PackageCount;
	// ----
	BYTE		PointCategory[3][50];
	// ----
}; extern GameShop	gGameShop;
// -------------------------------------------------------------------------------