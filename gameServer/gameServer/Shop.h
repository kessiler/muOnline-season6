#ifndef ___SHOP_H
#define ___SHOP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\zzzitem.h"

#define MAX_SHOP			21	//1.01.00
#define MAX_ITEM_IN_SHOP	120

class CShop
{

public:

	CShop();
	virtual ~CShop();

	void Init();
	BOOL LoadShopItem(int Shopnumber);
	BOOL LoadShopItem(char* filename);
	int InsertItem(int type, int index, int level, int dur, int op1,  int op2, int op3, int Excellent, BYTE Ancient);
	int InentoryMapCheck(int sx, int sy, int width,  int height);


public:

	BYTE ShopInventoryMap[MAX_ITEM_IN_SHOP];	// 4
	int ItemCount;	// 7C
	CItem m_item[MAX_ITEM_IN_SHOP];	// 80
	BYTE SendItemData[MAX_ITEM_IN_SHOP*(MAX_ITEM_INFO+1)];	// 5300
	int SendItemDataLen;	// 56C0
};

BOOL ShopDataLoad();
extern CShop ShopC[MAX_SHOP];

#endif