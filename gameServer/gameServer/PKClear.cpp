#include "StdAfx.h"
#include "PKClear.h"
#include "GameMain.h"
#include "..\include\ReadScript.h"
#include "..\common\winutil.h"
#include "logproc.h"
// -------------------------------------------------------------------------------

PKClear g_PKClear;
// -------------------------------------------------------------------------------

PKClear::PKClear()
{
	this->Init();
}
// -------------------------------------------------------------------------------

PKClear::~PKClear()
{

}
// -------------------------------------------------------------------------------

void PKClear::Init()
{
	this->m_NpcID			= -1;
	this->m_NpcMap			= -1;
	this->m_NpcX			= 0;
	this->m_NpcY			= 0;
	this->m_ReqPK			= 4;
	this->m_ReqLevel		= 1;
	this->m_ReqReset		= 0;
	this->m_ItemLoaded		= 0;
	ZeroMemory(this->m_ItemData, sizeof(this->m_ItemData));
	ZeroMemory(this->m_Cost, sizeof(this->m_Cost));
}
// -------------------------------------------------------------------------------

void PKClear::Load()
{
	this->Init();
	this->ReadData(gDirPath.GetNewPath("Custom\\PKClear.txt"));
}
// -------------------------------------------------------------------------------

void PKClear::ReadData(char * File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[PKClear] %s file not found", File);
		return;
	}
	// ----
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
			if( Category == 0 )	//-> NPC
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->m_NpcID		= TokenNumber;
				// ----
				Token = GetToken();
				this->m_NpcMap		= TokenNumber;
				// ----
				Token = GetToken();
				this->m_NpcX		= TokenNumber;
				// ----
				Token = GetToken();
				this->m_NpcY		= TokenNumber;
			}
			else if( Category == 1 ) //-> Requirements
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->m_ReqPK		= TokenNumber;
				// ----
				Token = GetToken();
				this->m_ReqLevel	= TokenNumber;
				// ----
				Token = GetToken();
				this->m_ReqReset	= TokenNumber;
			}
			else if( Category == 2 ) //-> Cost
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->m_Cost[0]		= TokenNumber;
				// ----
				Token = GetToken();
				this->m_Cost[1]		= TokenNumber;
				// ----
				Token = GetToken();
				this->m_Cost[2]		= TokenNumber;
				// ----
				Token = GetToken();
				this->m_Cost[3]		= TokenNumber;
			}
			else if( Category == 3 ) //-> Item list
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				WORD ItemType = TokenNumber;
				Token = GetToken();
				WORD ItemIndex = TokenNumber;
				this->m_ItemData[this->m_ItemLoaded].ID	 = ITEMGET(ItemType, ItemIndex);
				// ----
				Token = GetToken();
				this->m_ItemData[this->m_ItemLoaded].MinLevel	= TokenNumber;
				// ----
				Token = GetToken();
				this->m_ItemData[this->m_ItemLoaded].MaxLevel	= TokenNumber;
				// ----
				Token = GetToken();
				this->m_ItemData[this->m_ItemLoaded].MinDur	= TokenNumber;
				// ----
				Token = GetToken();
				this->m_ItemData[this->m_ItemLoaded].MaxDur	= TokenNumber;
				// ----
				this->m_ItemLoaded++;
			}
		}
	}
	// ----
	fclose(SMDFile);
	LogAddTD("[PKClear] %s file is loaded (Items: %d)", File, this->m_ItemLoaded);
}
// -------------------------------------------------------------------------------

bool PKClear::SearchItem(LPOBJ lpUser, BYTE QuestItemID)
{
	WORD ItemID = this->m_ItemData[QuestItemID].ID;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		if( !lpUser->pInventory[i].IsItem() || lpUser->pInventory[i].m_Type != ItemID )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Level < this->m_ItemData[QuestItemID].MinLevel
			|| lpUser->pInventory[i].m_Level > this->m_ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Durability < (float)this->m_ItemData[QuestItemID].MinDur
			|| lpUser->pInventory[i].m_Durability > (float)this->m_ItemData[QuestItemID].MaxDur )
		{
			continue;
		}
		// ----
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool PKClear::DeleteItem(LPOBJ lpUser, BYTE QuestItemID)
{
	WORD ItemID = this->m_ItemData[QuestItemID].ID;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		if( !lpUser->pInventory[i].IsItem() || lpUser->pInventory[i].m_Type != ItemID )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Level < this->m_ItemData[QuestItemID].MinLevel
			|| lpUser->pInventory[i].m_Level > this->m_ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Durability < (float)this->m_ItemData[QuestItemID].MinDur
			|| lpUser->pInventory[i].m_Durability > (float)this->m_ItemData[QuestItemID].MaxDur )
		{
			continue;
		}
		// ----
		gObjInventoryDeleteItem(lpUser->m_Index, i);
		GCInventoryItemDeleteSend(lpUser->m_Index, i, 1);
		// ----
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool PKClear::CheckReq(LPOBJ lpUser)
{
	if( lpUser->m_PK_Level <= this->m_ReqPK )
	{
		GCServerMsgStringSend("Your PK Level is small to PK Clear", lpUser->m_Index, 1);
		return false;
	}
	// ----
	if( lpUser->Level < this->m_ReqLevel )
	{
		GCServerMsgStringSend("Your Level is small to PK Clear", lpUser->m_Index, 1);
		return false;
	}
	// ----
	if( lpUser->Reset < this->m_ReqReset )
	{
		GCServerMsgStringSend("Your Reset is small to PK Clear", lpUser->m_Index, 1);
		return false;
	}
	// ----
	if( lpUser->Money < this->m_Cost[0] )
	{
		GCServerMsgStringSend("You need more Zen to PK Clear", lpUser->m_Index, 1);
		return false;
	}
	// ----
	if( lpUser->GameShop.WCoinC < this->m_Cost[1] )
	{
		GCServerMsgStringSend("You need more WCoinC to PK Clear", lpUser->m_Index, 1);
		return false;
	}
	// ----
	if( lpUser->GameShop.WCoinP < this->m_Cost[2] )
	{
		GCServerMsgStringSend("You need more WCoinP to PK Clear", lpUser->m_Index, 1);
		return false;
	}
	// ----
	if( lpUser->GameShop.GoblinPoint < this->m_Cost[3] )
	{
		GCServerMsgStringSend("You need more GoblinPoint to PK Clear", lpUser->m_Index, 1);
		return false;
	}
	// ----
	for( int i = 0; i < this->m_ItemLoaded; i++ )
	{
		if( !this->SearchItem(lpUser, i) )
		{
			GCServerMsgStringSend("You have not required items to PK Clear", lpUser->m_Index, 1);
			return false;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

bool PKClear::Dialog(LPOBJ lpUser, LPOBJ lpNpc)
{
	if(		lpNpc->Class		!= this->m_NpcID 
		||	lpNpc->MapNumber	!= this->m_NpcMap
		||	lpNpc->X			!= this->m_NpcX
		||	lpNpc->Y			!= this->m_NpcY )
	{
		return false;
	}
	// ----
	if( !this->CheckReq(lpUser) )
	{
		return false;
	}
	// ----
	for( int i = 0; i < this->m_ItemLoaded; i++ )
	{
		this->DeleteItem(lpUser, i);
	}
	// ----
	lpUser->Money					-= this->m_Cost[0];
	lpUser->GameShop.WCoinC			-= this->m_Cost[1];
	lpUser->GameShop.WCoinP			-= this->m_Cost[2];
	lpUser->GameShop.GoblinPoint	-= this->m_Cost[3];
	// ----
	gGameShop.GDSavePoint(lpUser->m_Index);
	GCMoneySend(lpUser->m_Index, lpUser->Money);
	// ----
	lpUser->m_PK_Count	= 0;
	lpUser->m_PK_Level	= 3;
	lpUser->m_PK_Time	= 0;
	// ----
	if( lpUser->PartyNumber >= 0 )
	{
		gParty.UpdatePKUserInfo(lpUser->PartyNumber, lpUser->m_Index, lpUser->DBNumber, lpUser->m_PK_Level);
		gParty.UpdatePKPartyPanalty(lpUser->PartyNumber);
	}
	// ----
	GCPkLevelSend(lpUser->m_Index, lpUser->m_PK_Level);
	GCServerMsgStringSend("Your PK status has been reseted", lpUser->m_Index, 1);
	return true;
}
// -------------------------------------------------------------------------------