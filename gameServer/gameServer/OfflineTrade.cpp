#include "StdAfx.h"
#include "OfflineTrade.h"
#include "ConnectEx.h"
#include "giocp.h"
#include "SProtocol.h"
#include "user.h"
#include "GameMain.h"
#include "..\include\ReadScript.h"
#include "..\common\winutil.h"
#include "logproc.h"
#include "LogToFile.h"
#ifdef __ALIEN__
#include "DSProtocol.h"
#endif
// -------------------------------------------------------------------------------

CLogToFile g_OfflineTradeLog("Reset", ".\\LOG\\OfflineTrade", TRUE);
// -------------------------------------------------------------------------

OfflineTrade g_OfflineTrade;
// -------------------------------------------------------------------------------

OfflineTrade::OfflineTrade()
{
	this->Init();
}
// -------------------------------------------------------------------------------

OfflineTrade::~OfflineTrade()
{

}
// -------------------------------------------------------------------------------

void OfflineTrade::Init()
{
	this->m_ReqLevel		= 1;
	this->m_ReqReset		= 0;
	this->m_UseMapList		= false;
	this->m_LoadedMapList	= 0;
	// ----
	for( int i = 0; i < MAX_NUMBER_MAP; i++ )
	{
		this->m_MapList[i] = -1;
	}
	// ----
	ZeroMemory(this->m_Cost, sizeof(this->m_Cost));
}
// -------------------------------------------------------------------------------

void OfflineTrade::Load()
{
	this->Init();
	this->ReadData(gDirPath.GetNewPath("Custom\\OfflineTrade.txt"));
}
// -------------------------------------------------------------------------------

void OfflineTrade::ReadData(char * File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[OfflineTrade] %s file not found", File);
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
			if( Category == 0 )	//-> Requirements
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->m_ReqLevel	= TokenNumber;
				// ----
				Token = GetToken();
				this->m_ReqReset	= TokenNumber;
				// ----
				Token = GetToken();
				this->m_UseMapList	= TokenNumber;
			}
			else if( Category == 1 ) //-> Cost
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
			else if( Category == 2 ) //-> Map list
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->m_MapList[this->m_LoadedMapList] = TokenNumber;
				this->m_LoadedMapList++;
			}
#ifdef __ALIEN__
			else if( Category == 3 )	//-> Enable OffTrade
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->EnableZen	= TokenNumber;
				// ----
				Token = GetToken();
				this->EnableWCoin	= TokenNumber;
				// ----
				Token = GetToken();
				this->EnableSoul	= TokenNumber;
			}
#endif
		}
	}
	// ----
	fclose(SMDFile);
	LogAddTD("[OfflineTrade] %s file is loaded (MapList: %d (%d))", File, this->m_UseMapList, this->m_LoadedMapList);
}
// -------------------------------------------------------------------------------

bool OfflineTrade::CheckReq(int UserIndex)
{
	LPOBJ lpUser = &gObj[UserIndex];
	// ----
	if( !lpUser->m_bPShopOpen )
    {
		GCServerMsgStringSend("You need open p. shop to create offline store", UserIndex, 1);
		g_OfflineTradeLog.Output("[%s] [%s] Request aborted - not opened shop", 
			lpUser->AccountID, lpUser->Name);
		return false;
	}
	// ----
	if( lpUser->Level < this->m_ReqLevel )
	{
		GCServerMsgStringSend("Your Level is small to create offline store", UserIndex, 1);
		g_OfflineTradeLog.Output("[%s] [%s] Request aborted - small level (%d / %d)", 
			lpUser->AccountID, lpUser->Name, lpUser->Level, this->m_ReqLevel);
		return false;
	}
	// ----
	if( lpUser->Reset < this->m_ReqReset )
	{
		GCServerMsgStringSend("Your Reset is small to create offline store", UserIndex, 1);
		g_OfflineTradeLog.Output("[%s] [%s] Request aborted - small reset (%d / %d)", 
			lpUser->AccountID, lpUser->Name, lpUser->Reset, this->m_ReqReset);
		return false;
	}
	// ----
	if( lpUser->Money < this->m_Cost[0] )
	{
		GCServerMsgStringSend("You need more Zen to create offline store", UserIndex, 1);
		g_OfflineTradeLog.Output("[%s] [%s] Request aborted - short money (%d / %d)", 
			lpUser->AccountID, lpUser->Name, lpUser->Money, this->m_Cost[0]);
		return false;
	}
	// ----
	if( lpUser->GameShop.WCoinC < this->m_Cost[1] )
	{
		GCServerMsgStringSend("You need more WCoinC to create offline store", UserIndex, 1);
		g_OfflineTradeLog.Output("[%s] [%s] Request aborted - short wcoinc (%d / %d)", 
			lpUser->AccountID, lpUser->Name, lpUser->GameShop.WCoinC, this->m_Cost[1]);
		return false;
	}
	// ----
	if( lpUser->GameShop.WCoinP < this->m_Cost[2] )
	{
		GCServerMsgStringSend("You need more WCoinP to create offline store", UserIndex, 1);
		g_OfflineTradeLog.Output("[%s] [%s] Request aborted - short wcoinp (%d / %d)", 
			lpUser->AccountID, lpUser->Name, lpUser->GameShop.WCoinP, this->m_Cost[2]);
		return false;
	}
	// ----
	if( lpUser->GameShop.GoblinPoint < this->m_Cost[3] )
	{
		GCServerMsgStringSend("You need more GoblinPoint to create offline store", UserIndex, 1);
		g_OfflineTradeLog.Output("[%s] [%s] Request aborted - short wcoing (%d / %d)", 
			lpUser->AccountID, lpUser->Name, lpUser->GameShop.GoblinPoint, this->m_Cost[3]);
		return false;
	}
	// ----
	if( this->m_UseMapList )
	{
		bool RightMap = false;
		// ----
		for( int i = 0; i < MAX_NUMBER_MAP; i++ )
		{
			if( this->m_MapList[i] == lpUser->MapNumber )
			{
				RightMap = true;
				break;
			}
		}
		// ----
		if( !RightMap )
		{
			GCServerMsgStringSend("You can't open offline store on this map", UserIndex, 1);
			g_OfflineTradeLog.Output("[%s] [%s] Request aborted - invalid map (%d)", 
				lpUser->AccountID, lpUser->Name, lpUser->MapNumber);
			return false;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

#ifdef __ALIEN__
void OfflineTrade::Start(int UserIndex, int Type)
#else
void OfflineTrade::Start(int UserIndex)
#endif
{
	LPOBJ lpUser = &gObj[UserIndex];
	// ----
	g_OfflineTradeLog.Output("[%s] [%s] Send request to start offline trade", 
		lpUser->AccountID, lpUser->Name);
	// ----
	if( !this->CheckReq(UserIndex) )
	{
		return;
	}
#ifdef __ALIEN__
	switch(Type)
	{
		case 0: strcpy(lpUser->m_szPShopText, ZEN_SHOP);		break;
		case 1: strcpy(lpUser->m_szPShopText, WCOIN_SHOP);	break;
		case 2: strcpy(lpUser->m_szPShopText, SOUL_SHOP);	break;
		default:return;
	}
#endif
	// ----
	lpUser->Money					-= this->m_Cost[0];
	lpUser->GameShop.WCoinC			-= this->m_Cost[1];
	lpUser->GameShop.WCoinP			-= this->m_Cost[2];
	lpUser->GameShop.GoblinPoint	-= this->m_Cost[3];
	// ----
#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
	g_ConnectEx.SendClose(UserIndex);
#endif
	// ----
	gGameShop.GDSavePoint(UserIndex);
	GCMoneySend(UserIndex, lpUser->Money);
	lpUser->IsOffTrade = true;
    CloseClientEx(UserIndex);
	// ----
#ifdef __ALIEN__
	g_OfflineTradeLog.Output("[%s] [%s] Has been disconnected, offline shop created (Type: %d)", 
		lpUser->AccountID, lpUser->Name, Type);
#else
	g_OfflineTradeLog.Output("[%s] [%s] Has been disconnected, offline shop created", 
		lpUser->AccountID, lpUser->Name);
#endif
}
// -------------------------------------------------------------------------------

bool OfflineTrade::Update(int UserIndex, BYTE Type)
{
	LPOBJ lpUser = &gObj[UserIndex];
	// ----
	if( !lpUser->IsOffTrade )
	{
		return false;
	}
	// ----
	if( Type == 0 )
	{
		lpUser->CheckTick = GetTickCount();
	}
	else if( Type == 1 )
	{
		lpUser->ConnectCheckTime = GetTickCount();
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

void OfflineTrade::Close(char * AccountID)
{
	for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
	{
		LPOBJ lpUser = &gObj[i];
		// ----
		if( lpUser->Connected != PLAYER_PLAYING || !lpUser->IsOffTrade || strcmp(AccountID, lpUser->AccountID) )
		{
			continue;
		}
		// ----
		GJPUserClose(lpUser->AccountID);
		gObjDel(i);
		lpUser->IsOffTrade = false;
		// ----
		g_OfflineTradeLog.Output("[%s] [%s] Offline shop has been closed, account free", 
			lpUser->AccountID, lpUser->Name);
	}
}
// -------------------------------------------------------------------------------

#ifdef __ALIEN__
bool OfflineTrade::CheckExchange(LPOBJ lpUser, LPOBJ lpTargetUser, int ItemPos)
{

	if(!strcmp(lpUser->m_szPShopText,WCOIN_SHOP))
	{
		if ( lpTargetUser->GameShop.WCoinP < lpUser->Inventory1[ItemPos].m_iPShopValue )
		{
			LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Wcoin",
				lpTargetUser->AccountID, lpTargetUser->Name, lpUser->AccountID, lpUser->Name);
			GCServerMsgStringSend("Need more WCoin to buy", lpUser->m_Index, 1);
			return false;
		}
	}
	else if(!strcmp(lpUser->m_szPShopText,SOUL_SHOP))	//Soul check
	{
		if ( this->GetSoulCount(lpTargetUser->m_Index) < lpUser->Inventory1[ItemPos].m_iPShopValue )
		{
			LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Soul",
				lpTargetUser->AccountID, lpTargetUser->Name, lpUser->AccountID, lpUser->Name);
			GCServerMsgStringSend("Need more Soul to buy", lpTargetUser->m_Index, 1);
			return false;
		}
		else if( this->EmptyPointInventory(lpUser, lpUser->Inventory1[ItemPos].m_iPShopValue) )
		{
			LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Empty Invintory",
				lpTargetUser->AccountID, lpTargetUser->Name, lpUser->AccountID, lpUser->Name);
			GCServerMsgStringSend("Need more Empty Inventory", lpTargetUser->m_Index, 1);
			return false;
		}
	}
	else
	{
		if ( lpTargetUser->Money < lpUser->Inventory1[ItemPos].m_iPShopValue )
		{
			LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
				lpTargetUser->AccountID, lpTargetUser->Name, lpUser->AccountID, lpUser->Name);
			::CGPShopAnsBuyItem(lpTargetUser->m_Index, lpUser->m_Index, 0, 7);
			return false;
		}

		if ( gObjCheckMaxZen(lpUser->m_Index, lpUser->Inventory1[ItemPos].m_iPShopValue) == FALSE )
		{
			LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Exceeding Zen of the Host",
				lpTargetUser->AccountID, lpTargetUser->Name, lpUser->AccountID, lpUser->Name);
			::CGPShopAnsBuyItem(lpTargetUser->m_Index, lpUser->m_Index, 0, 8);
			return false;
		}
	} 
	return true;
}
// -------------------------------------------------------------------------------

void OfflineTrade::UpdateExchange(LPOBJ lpUSer, LPOBJ lpTargetUser, int dwCost)
{
	if(!strcmp(lpUSer->m_szPShopText, WCOIN_SHOP))
	{
		lpTargetUser->GameShop.WCoinP -= dwCost;
		lpUSer->GameShop.WCoinP += dwCost;
		gGameShop.GDSavePoint(lpTargetUser->m_Index);
		gGameShop.GDSavePoint(lpUSer->m_Index);
	}
	else if(!strcmp(lpUSer->m_szPShopText, SOUL_SHOP))
	{
		this->DeleteSoulCount(lpTargetUser->m_Index, dwCost);
		this->AddedSoul(lpUSer, dwCost);
	}
	else
	{
		lpTargetUser->Money -= dwCost;
		lpUSer->Money += dwCost;
		GCMoneySend(lpTargetUser->m_Index, lpTargetUser->Money);
		GCMoneySend(lpUSer->m_Index, lpUSer->Money);
	}
}
// -------------------------------------------------------------------------------

int OfflineTrade::GetSoulCount(int UserIndex)
{
    if( !OBJMAX_RANGE(UserIndex) )
    {
        return -1;
    }
    // ----
    int ItemCount   = 0;
    LPOBJ lpUser    = &gObj[UserIndex];
    // ----
    for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
    {
        if( !lpUser->pInventory[i].IsItem() )
        {
            continue;
        }
        // ----
        if( lpUser->pInventory[i].m_Type == ITEMGET(12, 31) )
        {
            ItemCount += 10 * (lpUser->pInventory[i].m_Level + 1);
        }
        else if( lpUser->pInventory[i].m_Type == ITEMGET(14, 14) )
        {
            ItemCount++;
        }
    }
    // ----
    return ItemCount;
}
// -------------------------------------------------------------------------------
 
int OfflineTrade::CheckItem(int UserIndex, int ItemID, BYTE ItemLevel)
{
    if( !OBJMAX_RANGE(UserIndex) )
    {
        return -1;
    }
    // ----
    LPOBJ lpUser = &gObj[UserIndex];
    // ----
    for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
    {
        if( !lpUser->pInventory[i].IsItem() )
        {
            continue;
        }
        // ----
        if(     lpUser->pInventory[i].m_Type == ItemID
            &&  lpUser->pInventory[i].m_Level == ItemLevel )
        {
            return i;
        }
    }
    // ----
    return -1;
}
// -------------------------------------------------------------------------------
 
bool OfflineTrade::DeleteSoulCount(int UserIndex, int SoulCount)
{
	LogAddTD("[OffTrade][DeBug] SoulCount: %d",SoulCount);
    if( !OBJMAX_RANGE(UserIndex) )
    {
        return false;
    }
    // ----
    if( this->GetSoulCount(UserIndex) < SoulCount )
    {
        return false;
    }
    // ----
    int DeletedItem = 0;
    LPOBJ lpUser    = &gObj[UserIndex];
    // ----
    while(true)
    {
        int ItemDiff = SoulCount - DeletedItem;
        // ----
        if( ItemDiff <= 0 )
        {
            break;
        }
        // ----
        if( ItemDiff >= 30 && this->CheckItem(UserIndex, ITEMGET(12, 31), 2) != -1 )
        {
            int ItemPos = this->CheckItem(UserIndex, ITEMGET(12, 31), 2);
            gObjInventoryDeleteItem(lpUser->m_Index, ItemPos);
            GCInventoryItemDeleteSend(lpUser->m_Index, ItemPos, 1);
            DeletedItem += 30;
        }
        else if( ItemDiff >= 20 && this->CheckItem(UserIndex, ITEMGET(12, 31), 1) != -1 )
        {
            int ItemPos = this->CheckItem(UserIndex, ITEMGET(12, 31), 1);
            gObjInventoryDeleteItem(lpUser->m_Index, ItemPos);
            GCInventoryItemDeleteSend(lpUser->m_Index, ItemPos, 1);
            DeletedItem += 20;
        }
        else if( ItemDiff >= 10 && this->CheckItem(UserIndex, ITEMGET(12, 31), 0) != -1 )
        {
            int ItemPos = this->CheckItem(UserIndex, ITEMGET(12, 31), 0);
            gObjInventoryDeleteItem(lpUser->m_Index, ItemPos);
            GCInventoryItemDeleteSend(lpUser->m_Index, ItemPos, 1);
            DeletedItem += 10;
        }
        else if( this->CheckItem(UserIndex, ITEMGET(14, 14), 0) != -1 )
        {
            int ItemPos = this->CheckItem(UserIndex, ITEMGET(14, 14), 0);
            gObjInventoryDeleteItem(lpUser->m_Index, ItemPos);
            GCInventoryItemDeleteSend(lpUser->m_Index, ItemPos, 1);
            DeletedItem++;
        }
    }
    // ----
    LogAddTD("[OfflineTrade] [%s] [%s] Deleted %d Jewels Of Soul",lpUser->AccountID, lpUser->Name);
    // ----
    return true;
}

bool OfflineTrade::EmptyPointInventory(LPOBJ lpUser, int SoulNeed)
{
	int SoulCount = 0;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
    {
		int ItemDiff = SoulNeed - SoulCount;
		// ----
        if( ItemDiff >= 30 && !lpUser->pInventory[i].IsItem() )
        {
			SoulCount += 30;
        }
		else if( ItemDiff >= 20 && !lpUser->pInventory[i].IsItem() )
		{
			SoulCount += 20;
		}
		else if( ItemDiff >= 10 && !lpUser->pInventory[i].IsItem() )
		{
			SoulCount += 10;
		}
		else if( ItemDiff >= 1 && !lpUser->pInventory[i].IsItem() )
		{
			SoulCount++;
		}
		// ----
		if( SoulCount >= SoulNeed || ItemDiff <= 0 )
		{
			break;
		}
	}
	// ----
	if( SoulCount < SoulNeed )
	{
		return false;
	}
	// ----
	return true;
}

void OfflineTrade::AddedSoul(LPOBJ lpUser, int SoulCount)
{
	int Total = 0;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
    {
		int ItemDiff = SoulCount - Total;
		// ----
        if( ItemDiff >= 30 && !lpUser->pInventory[i].IsItem() )
        {
			ItemSerialCreateSend(lpUser->m_Index, 235, 0, 0, ITEMGET(12, 31), 2, 255, 0, 0, 0, -1, 0, 0);
			Total += 30;
        }
		else if( ItemDiff >= 20 && !lpUser->pInventory[i].IsItem())
		{
			ItemSerialCreateSend(lpUser->m_Index, 235, 0, 0, ITEMGET(12, 31), 1, 255, 0, 0, 0, -1, 0, 0);
			Total += 20;
		}
		else if( ItemDiff >= 10 && !lpUser->pInventory[i].IsItem())
		{
			ItemSerialCreateSend(lpUser->m_Index, 235, 0, 0, ITEMGET(12, 31), 0, 255, 0, 0, 0, -1, 0, 0);
			Total += 10;
		}
		else if( ItemDiff >= 1 && !lpUser->pInventory[i].IsItem())
		{
			ItemSerialCreateSend(lpUser->m_Index, 235, 0, 0, ITEMGET(14, 14), 0, 255, 0, 0, 0, -1, 0, 0);
			Total++;
		}

		if( SoulCount == Total ) return;
	}
	// ----

}
#endif