//-> Decompiled by DarkSim | 11.11.2012 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#include "StdAfx.h"
#include "JewelMixSystem.h"
#include "user.h"
#include "logproc.h"
#include "Gamemain.h"
#include "DSProtocol.h"
#include "PeriodItemEx.h"	//-> New
// -------------------------------------------------------------------------------

//00925638	-> 100%
static const JEWELMIX_INFO g_JewelMixInfo[10][3] = 
{
	10,		500000,		0x1c0d,		0x181e,
	20,		1000000,	0x1c0d,		0x181e,
	30,		1500000,	0x1c0d,		0x181e,
	// ----
	10,		500000,		0x1c0e,		0x181f,
	20,		1000000,	0x1c0e,		0x181f,
	30,		1500000,	0x1c0e,		0x181f,
	// ----
	10,		500000,		0x1c10,		0x1888,	//-> New from this, 6.2
	20,		1000000,	0x1c10,		0x1888,
	30,		1500000,	0x1c10,		0x1888,
	// ----
	10,		500000,		0x1c16,		0x1889,
	20,		1000000,	0x1c16,		0x1889,
	30,		1500000,	0x1c16,		0x1889,
	// ----
	10,		500000,		0x1c1f,		0x188a,
	20,		1000000,	0x1c1f,		0x188a,
	30,		1500000,	0x1c1f,		0x188a,
	// ----
	10,		500000,		0x1c29,		0x188b,
	20,		1000000,	0x1c29,		0x188b,
	30,		1500000,	0x1c29,		0x188b,
	// ----
	10,		500000,		0x1c2a,		0x188c,
	20,		1000000,	0x1c2a,		0x188c,
	30,		1500000,	0x1c2a,		0x188c,
	// ----
	10,		500000,		0x180f,		0x188d,
	20,		1000000,	0x180f,		0x188d,
	30,		1500000,	0x180f,		0x188d,
	// ----
	10,		500000,		0x1c2b,		0x188e,
	20,		1000000,	0x1c2b,		0x188e,
	30,		1500000,	0x1c2b,		0x188e,
	// ----
	10,		500000,		0x1c2c,		0x188f,
	20,		1000000,	0x1c2c,		0x188f,
	30,		1500000,	0x1c2c,		0x188f
};
// -------------------------------------------------------------------------------

CJewelMixSystem::CJewelMixSystem()
{
	// ----
}
// -------------------------------------------------------------------------------

CJewelMixSystem::~CJewelMixSystem()
{
	// ----
}
// -------------------------------------------------------------------------------

//005ac3c0	-> 100% (Not changed)
int CJewelMixSystem::GetJewelCount(int iIndex, int iJewelType)
{
	if( !gObjIsConnected(iIndex) )
	{
		return -1;
	}
	// ----
	int iItemType;
	// ----
	switch(iJewelType)
	{
		case 0:
			iItemType = 0x1c0d;
			break;
			// --
		case 1:
			iItemType = 0x1c0e;
			break;
			// --
		default:
			return -1;
			break;
	}	
	// ----
	int iItemCount = gObjGetItemCountInIventory(iIndex, iItemType);
	// ----
	return iItemCount;
}
// -------------------------------------------------------------------------------

//005ac440	-> 100% (Not changed)
int CJewelMixSystem::GetJewelCountPerLevel(int iJewelType, int iJewelLevel)
{
	int iJewelCount;
	// ----
	if( iJewelLevel < 0 || iJewelLevel > 2 )
	{
		return -1;
	}
	// ----
	if( iJewelType )
	{
		if( iJewelType != 1 ) //-> Need rewrite, becoz it switch ><
		{
			return -1;
		}
		// ----
		iJewelCount = g_JewelMixInfo[0][iJewelLevel].m_iJewelCount;
	}
	else
	{
		iJewelCount = g_JewelMixInfo[0][iJewelLevel].m_iJewelCount;
	}
	// ----
	return iJewelCount;
}
// -------------------------------------------------------------------------------

//005ac4e0	-> 100%
int	CJewelMixSystem::MixJewel(int iIndex, int iJewelType, int iMixType)
{
	if( !gObjIsConnected(iIndex) )
	{
		return false;
	}
	// ----
	LPINTERFACE_STATE lpIfState = &gObj[iIndex].m_IfState;
	// ----
	if( lpIfState->use != 1 || lpIfState->type != 20 )
	{
		GCAnsJewelMix(iIndex, 6);	//-> 6: Protect
		LogAddTD("[ANTI-HACK][protocol] [Mix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		// ----
		return false;
	}
	// ----
	if( gObjCheckInventorySerial0Item(&gObj[iIndex]) )
	{
		MsgOutput(iIndex, lMsg.Get(3354));
		GCAnsJewelMix(iIndex, 4);	//-> 4: Serial
		LogAddTD("[ANTI-HACK][Serial 0 Item] [Mix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		// ----
		return false;
	}
	// ----
	if( gObj[iIndex].ChaosLock )
	{
		LogAddTD("[JewelMix] [%s][%s] Chaos Mix is already working", gObj[iIndex].AccountID, gObj[iIndex].Name);
		GCAnsJewelMix(iIndex, 0);	//-> 0: ChaosState
		// ----
		return false;
	}
	// ----
	gObj[iIndex].ChaosLock = true;
	// ----
	if( !CHECK_LIMIT(iJewelType, 10) )
	{
		LogAddTD("[JewelMix] [%s][%s] Mix iJewelType is out of bound : %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iJewelType);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelMix(iIndex, 2);	//-> 2: JewelType
		// ----
		return false;
	}
	// ----
	if( !CHECK_LIMIT(iMixType, 3) )
	{
		LogAddTD("[JewelMix] [%s][%s] iMixType is out of bound : %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iMixType);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelMix(iIndex, 3);	//-> 3: MixType
		// ----
		return false;
	}
	// ----
	int iItemType		= g_JewelMixInfo[iJewelType][iMixType].m_iSourceType;
	int iJewelCount		= g_JewelMixInfo[iJewelType][iMixType].m_iJewelCount;
	int iMixMoney		= g_JewelMixInfo[iJewelType][iMixType].m_iMixMoney;
	int iChangeType		= g_JewelMixInfo[iJewelType][iMixType].m_iChangeType;
	// ----
	if( iJewelCount < 1 )
	{
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelMix(iIndex, 0);	//-> 0: ChaosState
		// ----
		return false;
	}
	// ----
	int iUserJewelCount = gObjGetItemCountInIventory(iIndex, iItemType);
	// ----
	if( iJewelCount > iUserJewelCount )
	{
		LogAddTD("[JewelMix] [%s][%s] lack of jewel to mix : %d / %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iUserJewelCount, iJewelCount);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelMix(iIndex, 4);	//-> 4: Serial
		// ----
		return false;
	}
	// ----
	if( iMixMoney > gObj[iIndex].Money )
	{
		LogAddTD("[JewelMix] [%s][%s] lack of money to mix : %d / %d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Money, iMixMoney);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelMix(iIndex, 5);	//-> 5: Money
		// ----
		return false;
	}
	// ----
	int iDelJewelCount	= 0;
	int bItemDelOK		= false;	//-> Fucking BOOL, need use bool for BYTE returns ><
	// ----
	for( int x = INVETORY_WEAR_SIZE; x < MAIN_INVENTORY_SIZE; x++ ) //-> Changed 76 to 204 from 6.2
	{
		if( gObj[iIndex].pInventory[x].IsItem() )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == iItemType )	//-> Fix, my fucking error)
			{
				LogAddTD("[JewelMix] [%s][%s] Mix - Delete Jewel, Type:%d, Level:%d, Serial:%u", 
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].pInventory[x].m_Type, 
					gObj[iIndex].pInventory[x].m_Level, gObj[iIndex].pInventory[x].m_Number);
				// ----
				gObjInventoryItemSet(iIndex, x, -1);
				gObj[iIndex].pInventory[x].Clear();
				// ----
				iDelJewelCount++;
				// ----
				if( iJewelCount <= iDelJewelCount )
				{
					bItemDelOK = true;
					break;
				}
			}
		}
	}
	// ----
	GCItemListSend(iIndex);
	// ----
#ifdef PERIOD
	g_PeriodItemEx.OnRequestPeriodItemList(&gObj[iIndex]); //-> New
#endif
	// ----
	if( !bItemDelOK )
	{
		LogAddTD("[JewelMix] [%s][%s] lack of jewel to mix (in deleting) : %d / %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iDelJewelCount, iJewelCount);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelMix(iIndex, 0);	//-> 0: ChaosState
		// ----
		return false;
	}
	// ----
	ItemSerialCreateSend(iIndex, 235, gObj[iIndex].X, gObj[iIndex].Y, iChangeType, iMixType, 0, 0, 0, 0, iIndex, 0, 0);
	gObj[iIndex].ChaosLock	= false;
	gObj[iIndex].Money		-= iMixMoney;
	GCMoneySend(iIndex, gObj[iIndex].Money);
	GCAnsJewelMix(iIndex, 1);
	LogAddTD("[JewelMix] [%s][%s] jewel mix succeed : ItemType:%d, JewelCount:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, iItemType, iJewelCount);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005aceb0	-> 100%
int CJewelMixSystem::UnMixJewel(int iIndex, int iJewelType, int iJewelLevel, int iInventoryPos)
{
	if( !gObjIsConnected(iIndex) )
	{
		return false;
	}
	// ----
	LPINTERFACE_STATE lpIfState = &gObj[iIndex].m_IfState;
	// ----
	if( lpIfState->use != 1 || lpIfState->type != 20 )
	{
		GCAnsJewelUnMix(iIndex, 9);
		LogAddTD("[ANTI-HACK][protocol] [UnMix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		// ----
		return false;
	}
	// ----
	if( gObjCheckInventorySerial0Item(&gObj[iIndex]) )
	{
		MsgOutput(iIndex, lMsg.Get(3354));
		GCAnsJewelUnMix(iIndex, 0);
		LogAddTD("[ANTI-HACK][Serial 0 Item] [UnMix Jewel] (%s)(%s)", gObj[iIndex].AccountID, gObj[iIndex].Name);
		// ----
		return false;
	}
	// ----
	if( gObj[iIndex].ChaosLock )
	{
		LogAddTD("[JewelMix] [%s][%s] Chaos Mix is already working", gObj[iIndex].AccountID, gObj[iIndex].Name);
		GCAnsJewelUnMix(iIndex, 0);
		// ----
		return false;
	}
	// ----
	gObj[iIndex].ChaosLock = true;
	// ----
	if( !CHECK_LIMIT(iJewelType, 10) )
	{
		LogAddTD("[JewelMix] [%s][%s] UnMix iJewelType is out of bound : %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iJewelType);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelUnMix(iIndex, 2);
		// ----
		return false;
	}
	// ----
	if( !CHECK_LIMIT(iInventoryPos, MAIN_INVENTORY_SIZE+1) )
	{
		LogAddTD("[JewelMix] [%s][%s] iInventoryPos is out of bound : %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iInventoryPos);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelUnMix(iIndex, 5);
		// ----
		return false;
	}
	// ----
	if( !gObj[iIndex].pInventory[iInventoryPos].IsItem() )
	{
		LogAddTD("[JewelMix] [%s][%s] Item to unmix is not exist", gObj[iIndex].AccountID, gObj[iIndex].Name);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelUnMix(iIndex, 4);
		// ----
		return false;
	}
	if( gObj[iIndex].pInventory[iInventoryPos].m_Level != iJewelLevel )
	{
		LogAddTD("[JewelMix] [%s][%s] iJewelLevel is different from request : %d / %d", gObj[iIndex].AccountID, gObj[iIndex].Name, 
			gObj[iIndex].pInventory[iInventoryPos].m_Level, iJewelLevel);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelUnMix(iIndex, 3);
		// ----
		return false;
	}
	// ----
	int iInventoryItemType	= gObj[iIndex].pInventory[iInventoryPos].m_Type;
	int iInventoryItemLevel	= gObj[iIndex].pInventory[iInventoryPos].m_Level;
	// ----
	if( !CHECK_LIMIT(iInventoryItemLevel, 4) )
	{
		LogAddTD("[JewelMix] [%s][%s] iInventoryItemLevel is out of bound : %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iInventoryItemLevel);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelUnMix(iIndex, 3);
		// ----
		return false;
	}
	// ----
	int iItemType	= g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iChangeType;
	int iJewelCount = g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iJewelCount;
	int iChangeType = g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iSourceType;
	int iMixMoney	= 1000000;
	// ----
	if ( iInventoryItemType != iItemType )
	{
		LogAddTD("[JewelMix] [%s][%s] iItemType is different from request : %d / %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iInventoryItemType, iItemType);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelUnMix(iIndex, 6);
		// -----
		return false;
	}
	// ----
	if( gObj[iIndex].Money < iMixMoney )
	{
		LogAddTD("[JewelMix] [%s][%s] lack of money to unmix : %d / %d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].Money, iMixMoney);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelUnMix(iIndex, 8);
		// ----
		return false;
	}
	// ----
	int iEmptyCount = 0;
	// ----
	for( int x = 0; x < 8 * (4 * gObj[iIndex].ExpandedInventory + 8); x++ )	//fixed
	{
		if( gObj[iIndex].pInventoryMap[x] == 255 )
		{
			iEmptyCount++;
		}
	}
	// ----
	if( iEmptyCount < iJewelCount )
	{
		LogAddTD("[JewelMix] [%s][%s] lack of empty slot to unmix : %d / %d", gObj[iIndex].AccountID, gObj[iIndex].Name, iEmptyCount, iJewelCount);
		gObj[iIndex].ChaosLock = false;
		GCAnsJewelUnMix(iIndex, 7);
		// ----
		return false;
	}
	// ----
	LogAddTD("[JewelMix] [%s][%s] UnMix - Delete Jewel, Type:%d, Level:%d, Serial:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].pInventory[iInventoryPos].m_Type,
		gObj[iIndex].pInventory[iInventoryPos].m_Level, gObj[iIndex].pInventory[iInventoryPos].m_Number);
	gObjInventoryItemSet(iIndex, iInventoryPos, -1);
	gObjInventoryDeleteItem(iIndex, iInventoryPos);
	GCInventoryItemDeleteSend(iIndex, iInventoryPos, 1);
	// ----
	int	iCrtJewelCount	= 0;
	int	bItemCrtOK		= false; //-> BBB (Big Bad Bool) :D
	// ----
	for( int x = 0; x < 8 * (4 * gObj[iIndex].ExpandedInventory + 8); x++ ) //-> From 64 to 192, 6.2
	{
		if( gObj[iIndex].pInventoryMap[x] == 255 )
		{
			ItemSerialCreateSend(iIndex, 235, gObj[iIndex].X, gObj[iIndex].Y, iChangeType, 0, 0, 0, 0, 0, iIndex, 0, 0);
			iCrtJewelCount++;
			// ----
			if( iJewelCount <= iCrtJewelCount )
			{
				bItemCrtOK = true;
				break;
			}
		}
	}
	// ----
	gObj[iIndex].ChaosLock = false;
	// ----
	if( !bItemCrtOK )
	{
		LogAddTD("[JewelMix] [%s][%s] jewel unmix failed : ItemType:%d, JewelCount:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, iItemType, iJewelCount);
		GCAnsJewelUnMix(iIndex, 0);		
	}
	else
	{
		LogAddTD("[JewelMix] [%s][%s] jewel unmix succeed : ItemType:%d, JewelCount:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, iItemType, iJewelCount);
		gObj[iIndex].Money -= iMixMoney;
		GCMoneySend(iIndex, gObj[iIndex].Money);
		GCAnsJewelUnMix(iIndex, 1);
	}
	// -----
	return true;
}
// -------------------------------------------------------------------------------