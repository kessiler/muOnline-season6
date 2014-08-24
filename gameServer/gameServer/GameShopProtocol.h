#pragma once
// -------------------------------------------------------------------------------

#include "..\include\prodef.h"
// -------------------------------------------------------------------------------

enum eGAMESHOP
{
	cPoint			= 0x01,
	cShopOpen		= 0x02,
	cBuy			= 0x03,
	cStorageCount	= 0x06,
	cStorageItemUse	= 0x0B,
	cScriptVer		= 0x0C,
	cStorageItem	= 0x0D,
	cBannerVer		= 0x15,
};
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_INFO_VERSION //-> 10
{
	PBMSG_HEAD2 h;
	// ----
	short	Major;
	short	Minor;
	short	Revision;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_REQ_OPEN //-> 5
{
	PBMSG_HEAD2 h;
	// ----
	BYTE	Result;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_REQ_BUY //-> 22
{
	PBMSG_HEAD2	h;
	// ----
	int		IBSPackageID;
	int		IBSCategory;
	int		IBSIndex;
	short	ItemID;
	int		Unknown18;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_REQ_STORAGE //-> 10
{
	PBMSG_HEAD2	h;
	// ----
	int		Result;
	BYTE	StorageType;
	BYTE	Unknown10;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_REQ_STORAGEITEMUSE //-> 15
{
	PBMSG_HEAD2	h;
	// ----
	int		AuthIndex;
	int		AuthID;
	short	ItemID;
	BYTE	Unknown15;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_ANS_POINT
{
	PBMSG_HEAD2 h;
	// ----
	BYTE	Result;
	double	WCoinC[2];
	double	WCoinP[2];
	double	GoblinPoint;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_ANS_BUY
{
	PBMSG_HEAD2 h;
	// ----
	bool	Result;
	int		Unknown;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_ANS_STORAGE_COUNT
{
	PBMSG_HEAD2 h;
	// ----
	short	MainStorage[2];
	short	GiftStorage[2];
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_ANS_STORAGE_ITEM
{
	PBMSG_HEAD2	h;
	// ----
	int		AuthIndex;
	int		AuthID;
	int		IBSUnknown;
	int		IBSID;
	int		IBSIndex;
	int		Unk1;
	int		Unk2;
	BYTE	Unk3;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_ANS_STORAGE_ITEM_USE
{
	PBMSG_HEAD2	h;
	// ----
	BYTE Result;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_GD_SAVE_POINT
{
	PBMSG_HEAD2	h;
	// ----
	WORD	UserIndex;
	char	AccountID[MAX_IDSTRING+1];
	long	WCoinP;
	long	WCoinC;
	long	GoblinPoint;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_GD_REQ_POINT
{
	PBMSG_HEAD2	h;
	// ----
	WORD	UserIndex;
	char	AccountID[MAX_IDSTRING+1];
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_DG_GET_POINT
{
	PBMSG_HEAD2	h;
	// ----
	WORD	UserIndex;
	long	WCoinP;
	long	WCoinC;
	long	GoblinPoint;
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct STORAGE
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

#pragma pack(push, 1)
struct GAMESHOP_GD_SAVE_STORAGEITEM
{
	PWMSG_HEAD2 h;
	WORD	UserIndex;
	char	AccountID[MAX_IDSTRING+1];
	STORAGE Item; 
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_GD_DEL_STORAGEITEM
{
	PWMSG_HEAD2 h;
	WORD	UserIndex;
	char	AccountID[MAX_IDSTRING+1];
	STORAGE Item; 
};
#pragma pack(pop)
// -------------------------------------------------------------------------------

#pragma pack(push, 1)
struct GAMESHOP_GD_STORAGE
{
	PWMSG_HEAD2 h;
	WORD	UserIndex;
	char	AccountID[MAX_IDSTRING+1];
	int		ItemCount;
	STORAGE Storage[30]; 
};
#pragma pack(pop)
// -------------------------------------------------------------------------------