#include "stdafx.h"
#include "ProtocolDefine.h"

#ifdef __ALIEN__

struct PMSG_ANS_BUYLIST_FROM_PSHOP
{
	PWMSG_HEAD2 h;
	BYTE Result;
	BYTE NumberH;
	BYTE NumberL;
	BYTE btName[10];
	char szPShopText[36];
	BYTE btCount;
};

#define pShowPrice		 ((int(*)(int a1, const char *a2, ...)) 0x9CF52A)

#define ZEN_SHOP	".::ZEN SHOP::."
#define WCOIN_SHOP	".::WCOIN SHOP::."
#define SOUL_SHOP	".::SOUL SHOP::."

class cOffTrade
{
public:
	void Init();

	char TypeShop;

	void RecvPShop(PMSG_ANS_BUYLIST_FROM_PSHOP *Data);

	static void ShowPrice(DWORD a1, const char *a2, char *Price);
	static void ShowTwoString(DWORD a1, const char *a2);
};
extern cOffTrade gOffTrade;

#endif