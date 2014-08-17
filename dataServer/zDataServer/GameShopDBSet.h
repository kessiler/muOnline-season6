#pragma once
// -------------------------------------------------------------------------------

#include "Sprodef.h"
#include "DBConBase.h"
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


class GameShopDBSet
{
public:
	GameShopDBSet();
	virtual ~GameShopDBSet();
	// ----
	bool Connect();
	// ----
	bool		SavePoint(char * AccountID, long WCoinP, long WCoinC, long GoblinPoint);
	bool		ReqPoint(char * AccountID, GAMESHOP_DG_GET_POINT * Result);
	void		InsertPoint(char * AccountID, long WCoinP, long WCoinC, long GoblinPoint);
	// ----
public:
	CQuery m_Query;
};
// -------------------------------------------------------------------------------