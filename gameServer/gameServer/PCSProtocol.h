#ifndef PCSPROTOCOL_H__
#define PCSPROTOCOL_H__

//#include "..\common\zzzitem.h"

#define MAX_NTF_TRANSACTIONS	16

#pragma pack(1)
struct sellItemPrice
{
	DWORD dwPriceGuid;	// 0
	DWORD dwItemGuid;	// 4
	DWORD dwUseTime;	// 8
	DWORD dwAmount;	// C
	DWORD dwPrice;	// 10
	DWORD dwSellRate;	// 14
};

struct sellItem
{
	DWORD dwItemGuid;	// 0
	DWORD dwItemCODE;	// 4
	DWORD dwCategoryID;	// 8
	DWORD dwUseType;	// C
	DWORD dwBuyType;	// 10
	DWORD dwCoolTime;	// 14
	double dbVarEndDate;	// 18
	DWORD dwLimitSellCount;	// 20
	DWORD dwState;	// 24
	char szItemName[64];	// 28
	char szItemDesc[128];	// 68
	DWORD dwPriceCount;	// E8
	sellItemPrice itemPrice[8];	// EC
};


struct sellPackage
{
	DWORD dwPackageGuid;	// 0
	sellItemPrice itemPrice;	// 4
	DWORD dwImageCode;	// 1C
	DWORD dwCategory;	// 20
	DWORD dwBuyType;	// 24
	DWORD dwLimitSellCount;	// 28
	DWORD dwInItemCount;	// 2C
	double dbVarEndDate;	// 30
	DWORD dwState;	// 38
	sellItemPrice inItemPrices[16];	// 3C
	char szPackageName[64];	// 1BC
	char szPackageDesc[128];	// 1FC
};


struct buyItemTransaction
{
	DWORD dwTransactionGuid;	// 0
	DWORD dwPriceGuid;	// 4
	DWORD dwResult;	// 8
};


struct itemBranch
{
	DWORD dwBranchID;	// 0
	DWORD dwGuid;	// 4
	DWORD dwItemType;	// 8
};



#pragma pack()



namespace protocol
{
	class HDRMessageID
	{
	public:
		WORD category;	// 0
		WORD messageID;	// 2
	};

	class MSG_HEADER
	{
	public:

		DWORD size;	// 0
		union
		{
			DWORD id;	// 4
			HDRMessageID hdrid;	// 4
		};


	};


	class MSG_GTOS_USER_CASH_REQ : public MSG_HEADER
	{
	public:
		DWORD dwUserGuid;	// 8	

		MSG_GTOS_USER_CASH_REQ()
		{
			this->size = sizeof(MSG_GTOS_USER_CASH_REQ);
			this->id = 0x100;
		};	// line : 74

		~MSG_GTOS_USER_CASH_REQ()
		{
		};

	};

	class MSG_STOG_USER_CASH_ANS : public MSG_HEADER
	{
	public:
		DWORD dwUserGuid;	// 8
		DWORD dwUserCash;	// C

		MSG_STOG_USER_CASH_ANS()
		{
			this->size = sizeof(MSG_STOG_USER_CASH_ANS);
			this->id = 0x200;
		};

		~MSG_STOG_USER_CASH_ANS()
		{
		};
	};

	class MSG_GTOS_ITEM_LIST_REQ : public MSG_HEADER
	{
	public:
		MSG_GTOS_ITEM_LIST_REQ()
		{
			this->size = sizeof(MSG_GTOS_ITEM_LIST_REQ);
			this->id = 0x300;
		};

		~MSG_GTOS_ITEM_LIST_REQ()
		{
		};
	};


	class MSG_STOG_ITEM_LIST_ANS : public MSG_HEADER
	{
	public:
		MSG_STOG_ITEM_LIST_ANS()
		{
			this->size = sizeof(MSG_STOG_ITEM_LIST_ANS);
			this->id = 0x400;
		};
		~MSG_STOG_ITEM_LIST_ANS()
		{
			return;
		};

		void resize();

		DWORD dwItemCount;	// 8
		sellItem sellItems[128];	// C
	};



	class MSG_GTOS_PACKAGE_LIST_REQ : public MSG_HEADER
	{
	public:
		MSG_GTOS_PACKAGE_LIST_REQ()
		{
			this->size = sizeof(MSG_GTOS_PACKAGE_LIST_REQ);
			this->id = 0x500;
		};

		~MSG_GTOS_PACKAGE_LIST_REQ()
		{
		};
	};



	class MSG_STOG_PACKAGE_LIST_ANS : public MSG_HEADER
	{
	public:
		MSG_STOG_PACKAGE_LIST_ANS()
		{
			this->size = sizeof(MSG_STOG_PACKAGE_LIST_ANS);
			this->id = 0x600;
		};

		~MSG_STOG_PACKAGE_LIST_ANS()
		{
		};
		void resize();

		DWORD dwPackageCount;	// 8
		sellPackage sellPackages[96];	// C
	};


	class MSG_GTOS_BRANCH_ITEM_LIST_REQ : public MSG_HEADER
	{
	public:
		MSG_GTOS_BRANCH_ITEM_LIST_REQ()
		{
			this->size = sizeof(MSG_GTOS_BRANCH_ITEM_LIST_REQ);
			this->id = 0x700;
		};

		~MSG_GTOS_BRANCH_ITEM_LIST_REQ()
		{
		};

	};

	class MSG_STOG_BRANCH_ITEM_LIST_ANS : public MSG_HEADER
	{
	public:
		DWORD dwItemCount;	// 8
		itemBranch branchItems[512];	// C

		MSG_STOG_BRANCH_ITEM_LIST_ANS()
		{
			this->size = sizeof(MSG_STOG_BRANCH_ITEM_LIST_ANS);
			this->id = 0x800;
		};


		~MSG_STOG_BRANCH_ITEM_LIST_ANS()
		{
		};

		void resize();
	};

	class MSG_GTOS_BUY_ITEM_REQ : public MSG_HEADER
	{
	public:
		DWORD dwServerGuid;	// 8
		DWORD dwUserGuid;	// C
		DWORD dwCharGuid;	// 10
		char szCharName[96];	// 14
		DWORD dwCount;	// 74
		DWORD dwPriceGuids[16];	// 78

		MSG_GTOS_BUY_ITEM_REQ()
		{
			this->size = sizeof(MSG_GTOS_BUY_ITEM_REQ);
			this->id = 0x900;
		};

		~MSG_GTOS_BUY_ITEM_REQ()
		{
		};

		void resize();
	};

	class MSG_STOG_BUY_ITEM_ANS : public MSG_HEADER
	{
	public:
		DWORD dwServerGuid;	// 8
		DWORD dwUserGuid;	// C
		DWORD dwCharGuid;	// 10
		DWORD dwUserCash;	// 14
		DWORD dwResult;	// 18
		DWORD dwCount;	// 1C
		buyItemTransaction transactions[16];	// 20

		MSG_STOG_BUY_ITEM_ANS()
		{
			this->size = sizeof(MSG_STOG_BUY_ITEM_ANS);
			this->id = 0xA00;
		};

		~MSG_STOG_BUY_ITEM_ANS()
		{
		};

		void resize();
	};

	class MSG_GTOS_BUY_ITEM_NTF : public MSG_HEADER
	{
	public:
		MSG_GTOS_BUY_ITEM_NTF()
		{
			this->size = sizeof(MSG_GTOS_BUY_ITEM_NTF);
			this->id = 0xB00;
		};


		~MSG_GTOS_BUY_ITEM_NTF()
		{
		};

		void resize()
		{
			this->size -= (16 - (DWORD)((WORD)(this->dwCount))) * sizeof(buyItemTransaction);
		};

		DWORD dwServerGuid;	// 8
		DWORD dwUserGuid;	// C
		DWORD dwCharGuid;	// 10
		DWORD dwCount;	// 14
		buyItemTransaction transactions[16];	// 18

	};

}


#endif /*PCSPROTOCOL_H__*/