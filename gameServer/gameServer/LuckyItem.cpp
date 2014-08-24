//-> Decompiled by DarkSim | 10.03.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#include "StdAfx.h"
#include "LargeRand.h"
#include "..\include\ReadScript.h"
#include "logproc.h"
#include "MixSystem.h"
#include "..\common\winutil.h"
#include "GameMain.h"
#include "DBSockMng.h"
#include "..\common\SetItemOption.h"
#include "DSProtocol.h"
#include "LuckyItem.h"
// -------------------------------------------------------------------------------

LuckyItemManager	g_LuckyItemManager;
// -------------------------------------------------------------------------------

//005add10	-> 100%
LuckyItemManager::LuckyItemManager()
{
	this->TicketCount = 0;
}
// -------------------------------------------------------------------------------

//005add50	-> 100%
LuckyItemManager::~LuckyItemManager()
{
	// ----
}
// -------------------------------------------------------------------------------

//005addf0	-> 100%
int	LuckyItemManager::LoadLuckyItemEquipment(int iType, _LUCKY_ITEM_INFO * pLuckyItemInfo)	//OK
{
	int counter									= 0;		//ebp-8
	int iItemKind								= iType;	//ebp-0c
	_LUCKY_ITEM_EQUIPMENT * pLuckyItemEquipment = 0;		//ebp-10
	// ----
	for( int i = 0; i < this->TicketCount; i++ )			//ebp-18
	{
		if( pLuckyItemInfo[i].ItemKind == iItemKind )
		{
			pLuckyItemEquipment = pLuckyItemInfo[i].m_ItemEquipment;
		}
	}
	// ----
	if( !pLuckyItemEquipment )
	{
		MsgBox("Load LuckyItem Script Error ItemType %d", iItemKind);
		return false;
	}
	// ----
	while(true)
	{
		SMDToken Token = GetToken();
		// ----
		if( Token == NAME )
		{
			break;
		}
		// ----
		if( !strcmp("end", TokenString) )
		{
			break;
		}
		// ----
		int iType	= TokenNumber;
		Token		= GetToken();		
		int iIndex	= TokenNumber;
		// ----
		pLuckyItemEquipment[counter].m_Index = ITEMGET(iType, iIndex);
		// ----
		Token = GetToken();		
		pLuckyItemEquipment[counter].m_Skill = TokenNumber;
		Token = GetToken();		
		pLuckyItemEquipment[counter].m_Luck = TokenNumber;
		Token = GetToken();		
		pLuckyItemEquipment[counter].m_Option = TokenNumber;
		Token = GetToken();		
		pLuckyItemEquipment[counter].m_Class = TokenNumber;
		// ----
		counter++;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005ae450	-> 100%
int LuckyItemManager::LoadLuckyItemInfo(char * lpszFileName)	//OK
{
	SMDFile = fopen(lpszFileName, "r");	//ok
	// ----
	if( !SMDFile )
	{
		MsgBox("MoveCommand %s File Load Error", lpszFileName);	//;D
		return false;
	}
	// ----
	SMDToken Token;
	int i 								= 0;
	int	iCount							= 0;
	int iTicketCount					= 0;
	this->TicketCount					= 0;
	_LUCKY_ITEM_INFO LuckyItemInfo[5];
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
		if( Token == NUMBER )
		{
			int Type = TokenNumber;
			// ----
			if( Type == 0 )	//-> Tickets
			{
				while(true)
				{
					Token = GetToken();
					// ----
					if( Token == NAME && !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					int iType	= TokenNumber;
					Token		= GetToken();		
					int iIndex	= TokenNumber;
					// ----
					LuckyItemInfo[iTicketCount].ItemIndex	= ITEMGET(iType, iIndex);	//Fixed
					// ----
					Token = GetToken();		
					LuckyItemInfo[iTicketCount].ItemKind	= TokenNumber;	//Fixed
					// ----
					iTicketCount++;
					// ----
					this->TicketCount = iTicketCount;
				}
			}
			else if( Type == 2 )	//-> Helm
			{
				if( !this->LoadLuckyItemEquipment(Type, &LuckyItemInfo[0]) )
				{
					return false;
				}
			}
			else if( Type == 3 )	//-> Armor
			{
				if( !this->LoadLuckyItemEquipment(Type, &LuckyItemInfo[0]) )
				{
					return false;
				}
			}
			else if( Type == 4 )	//-> Pants
			{
				if( !this->LoadLuckyItemEquipment(Type, &LuckyItemInfo[0]) )
				{
					return false;
				}
			}
			else if( Type == 5 )	//-> Gloves
			{
				if( !this->LoadLuckyItemEquipment(Type, &LuckyItemInfo[0]) )
				{
					return false;
				}
			}
			else if( Type == 6 )	//-> Boots
			{
				if( !this->LoadLuckyItemEquipment(Type, &LuckyItemInfo[0]) )
				{
					return false;
				}
			}
			else if( Type == 20 )	//-> ItemLevelRandRate
			{
				iCount = 0;
				// ----
				while(true)
				{
					Token = GetToken();
					// ----
					if( Token == NAME && !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					int iIndex = TokenNumber;
					// ----
					for( int i = 0; i < iTicketCount; i++ )
					{
						Token = GetToken();		
						LuckyItemInfo[i].m_ItemLevelRandRate[iIndex].m_Level	= TokenNumber;
						// ----
						Token = GetToken();		
						LuckyItemInfo[i].m_ItemLevelRandRate[iIndex].m_Rate		= TokenNumber;
					}
				}
			}
			else if( Type == 21 )	//-> AddOpt3Rate
			{
				iCount = 0;
				// ----
				while(true)
				{
					Token = GetToken();
					// ----
					if( Token == NAME && !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					int iIndex = TokenNumber;
					// ----
					for( int i = 0; i < iTicketCount; i++ )
					{
						Token = GetToken();		
						LuckyItemInfo[i].AddOpt3Rate[iIndex] = TokenNumber;
					}
				}
			}
			else if( Type == 22 )	//-> AddSetOptRate
			{
				iCount = 0;
				// ----
				while(true)
				{
					Token = GetToken();
					// -----
					if( Token == NAME && !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					int iIndex = TokenNumber;
					// ----
					for( int i = 0; i < iTicketCount; i++ )
					{
						Token = GetToken();		
						LuckyItemInfo[i].AddSetOptRate[iIndex] = TokenNumber;
					}
				}
			}
			else if( Type == 23 )	//-> ItemSmeltRate
			{
				iCount = 0;
				// -----
				while(true)
				{
					Token = GetToken();
					// ----
					if( Token == NAME && !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					int iIndex = TokenNumber;
					// ----
					for( int i = 0; i < iTicketCount; i++ )
					{
						Token = GetToken();		
						LuckyItemInfo[i].m_ItemSmeltRate[iIndex].MinDur		= TokenNumber;
						Token = GetToken();		
						LuckyItemInfo[i].m_ItemSmeltRate[iIndex].MaxDur		= TokenNumber;
						Token = GetToken();		
						LuckyItemInfo[i].m_ItemSmeltRate[iIndex].SmeltRate	= TokenNumber;
					}
				}
			}
		}
	}
	// ----
	fclose(SMDFile);
	this->AddLuckyItemInfo(LuckyItemInfo, iTicketCount);
	// ----
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] %s is loaded", __FUNCTION__, lpszFileName);
#endif
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005aedb0	-> 100%
void LuckyItemManager::AddLuckyItemInfo(_LUCKY_ITEM_INFO * pLuckyItemInfo, int iItemCnt)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	for( int i = 0; i < iItemCnt; i++ )
	{
		int iIndexID = pLuckyItemInfo[i].ItemIndex;
		this->LuckyItemInfo[iIndexID] = pLuckyItemInfo[i];
	}
}
// -------------------------------------------------------------------------------

//005aee40	-> 100%
_LUCKY_ITEM_INFO * LuckyItemManager::GetLuckyItemInfo(int iItemNum)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	std::map<int, _LUCKY_ITEM_INFO>::iterator Iter = this->LuckyItemInfo.find(iItemNum);
	// ----
	if( Iter == LuckyItemInfo.end() )
	{
		return 0;
	}
	// ----
	return &Iter->second;
}
// -------------------------------------------------------------------------------

//005aeec0	-> 100%
_LUCKY_ITEM_INFO * LuckyItemManager::GetCharClassLuckyItemInfo(int iItemNum, int iCharClass)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	if( iItemNum < 0 )
	{
		return 0;
	}
	// ----
	_LUCKY_ITEM_INFO * pLuckyItemInfo = this->GetLuckyItemInfo(iItemNum);
	// ----
	if( !pLuckyItemInfo )
	{
		return 0;
	}
	// ----
	for( int i = 0; i < MAX_LUCKYINFO_ITEMEQUIPMENT; i++ )
	{
		if( pLuckyItemInfo->m_ItemEquipment[i].m_Class == iCharClass )
		{
			return pLuckyItemInfo;
		}
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

//005aef50	-> 100%
_LUCKY_ITEM_EQUIPMENT * LuckyItemManager::GetLuckyItemEquipment(_LUCKY_ITEM_INFO * pLuckyItemInfo, int iCharClass)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	if( !pLuckyItemInfo )
	{
		return 0;
	}
	// ----
	for( int i = 0; i < MAX_LUCKYINFO_ITEMEQUIPMENT; i++ )
	{
		if( pLuckyItemInfo->m_ItemEquipment[i].m_Class == iCharClass )
		{
			return &pLuckyItemInfo->m_ItemEquipment[i];
		}
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

//005aefd0	-> 100% (But very strange, need manual revision)
int LuckyItemManager::IsLuckyItemTicket(int iItemNum)	//OK (missing 0043133B)
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	if( iItemNum < 0 )
	{
		return false;
	}
	// ----
	std::map<int, _LUCKY_ITEM_INFO>::iterator Iter = this->LuckyItemInfo.find(iItemNum);
	// ----
	if( Iter == this->LuckyItemInfo.end() )
	{
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005af060	-> 100%
int LuckyItemManager::IsLuckyItemEquipment(int iItemNum)	//OK (missing 0043135B)
{
	if( iItemNum < 0 )
	{
		return false;
	}
	// ----
	for( std::map<int, _LUCKY_ITEM_INFO>::iterator Iter = this->LuckyItemInfo.begin(); Iter != this->LuckyItemInfo.end(); Iter++ )
	{
		_LUCKY_ITEM_INFO * pLuckyItemInfo = &Iter->second;
		// ----
		for( int i = 0; i < MAX_LUCKYINFO_ITEMEQUIPMENT; i++ )
		{
			if( pLuckyItemInfo->m_ItemEquipment[i].m_Index == iItemNum )
			{
				return true;
			}
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//005af140	-> 100%
_SMELT_RATE * LuckyItemManager::GetSmeltingRateInfo(int iItemNum)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	if( iItemNum < 0 )
	{
		return false;
	}
	// ----
	for( std::map<int, _LUCKY_ITEM_INFO>::iterator Iter = this->LuckyItemInfo.begin(); Iter != this->LuckyItemInfo.end(); Iter++ )
	{
		_LUCKY_ITEM_INFO * pLuckyItemInfo = &Iter->second;
		// ----
		for( int i = 0; i < MAX_LUCKYINFO_ITEMEQUIPMENT; i++ )
		{
			if( pLuckyItemInfo->m_ItemEquipment[i].m_Index == iItemNum )
			{
#pragma message("##### CHECK ME ######")
				return pLuckyItemInfo->m_ItemSmeltRate;	//-> Need check
			}
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//005af230	-> 100% (But need check-check-check)
void LuckyItemManager::LuckyItemTicketExchange(LPOBJ lpObj)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	int iValidItemCount		= 0;	//ebp-8
	int iInvalidItemCount	= 0;	//ebp-0c
	CItem NewItem;
	// ----
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	// ----
	lpObj->ChaosLock = true;
	// ----
	_LUCKY_ITEM_INFO * pLuckyItemInfo				= 0;	//ebp-e4
	_ITEM_LEVEL_RATE * pItemLevelRandRate			= 0;	//ebp-e8
	_LUCKY_ITEM_EQUIPMENT * pLuckyItemEquipment		= 0;	//ebp-ec
	int pAddOpt3Rate[4]								= { 0 };
	int piAddSetOptRate[2]							= { 0 };
	int iItemPos									= -1;
	int iItemHeight									= 4;
	int iItemWidth									= 4;
	// ----
	if( !CheckInventoryEmptySpace(lpObj, iItemHeight, iItemWidth) )	//CheckInventoryEmptySpace_
	{
		LogAddTD("[LuckyItem] - Fail - Not Empty Inventory [%s][%s] CharClass[%d]", lpObj->AccountID, lpObj->Name, lpObj->Class);
		pMsg.Result = 0xF1;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}
	// ----
	for( int i = 0; i < CHAOS_BOX_SIZE; i++ )
	{
		if( lpObj->pChaosBox[i].IsItem() )
		{
			pLuckyItemInfo = this->GetCharClassLuckyItemInfo(lpObj->pChaosBox[i].m_Type, lpObj->Class);
			// -----
			if( pLuckyItemInfo == NULL )
			{
				iInvalidItemCount++;
				iItemPos = i;
			}
			else
			{
				iValidItemCount++;
				iItemPos = i;
			}
		}
	}
	// ----
	if( iValidItemCount != 1 || iInvalidItemCount != 0 || iItemPos == -1 )
	{
		LogAddTD("[LuckyItem] - Can Not be Exchanged [%s][%s] CharClass[%d] ItemNum[%d] ItemName[%s]", lpObj->AccountID, lpObj->Name, lpObj->Class,
			lpObj->pChaosBox[iItemPos].m_Type, ItemAttribute[lpObj->pChaosBox[iItemPos].m_Type].Name);
		pMsg.Result = 0x20;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}
	// ----
	g_MixSystem.LogChaosItem(lpObj, "[LuckyItem] Item Exchange");
	LogAddTD("[LuckyItem] - Exchange Start");
	// ----
	int	iCharType		= 0;
	int Type			= 0;
	BYTE level			= 0;
	BYTE dur			= 0xFF;
	BYTE Op1			= 0;
	BYTE Op2			= 0;
	BYTE Op3			= 0;
	BYTE ExOption		= 0;
	int iMinLv			= 0;
	int iMaxLv			= 0;
	int SetOption		= 0;
	pItemLevelRandRate	= pLuckyItemInfo->m_ItemLevelRandRate;
	pLuckyItemEquipment = this->GetLuckyItemEquipment(pLuckyItemInfo, lpObj->Class);
	pAddOpt3Rate[0]		= pLuckyItemInfo->AddOpt3Rate[0];
	piAddSetOptRate[0]	= pLuckyItemInfo->AddSetOptRate[0];
	// ----
	if( pLuckyItemEquipment == 0 || pItemLevelRandRate == 0 || piAddSetOptRate == 0 || pAddOpt3Rate	== 0 )
	{
		LogAddTD("[LuckyItem] --- Error --- [%s][%s] CharClass[%d] ItemNum[%d] ItemName[%s]", lpObj->AccountID, lpObj->Name, lpObj->Class,
			lpObj->pChaosBox[iItemPos].m_Type, ItemAttribute[lpObj->pChaosBox[iItemPos].m_Type].Name);
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}
	// ----
	Type		= pLuckyItemEquipment->m_Index;
	SetOption	= gSetItemOption.LuckyItemGenSetOption(Type, piAddSetOptRate);
	level		= this->GambleGetLevel(pItemLevelRandRate, lpObj->m_Index);
	// ----
	if( pLuckyItemEquipment->m_Skill )
	{
		Op1 = 1;
	}
	// ----
	if( pLuckyItemEquipment->m_Luck )
	{
		Op2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			Op2 = 1;
		}
	}
	// ----
	DWORD dwOptionTemp = GetLargeRand() % 1000000;
	// ----
	if( pLuckyItemEquipment->m_Option )
	{
		if( dwOptionTemp <= (DWORD)pAddOpt3Rate[0] )
		{
			Op3 = 0;
		}
		// ----
		if( dwOptionTemp <= (DWORD)(pAddOpt3Rate[0] + pAddOpt3Rate[1]) )
		{
			Op3 = 1;
		}
		// ----
		if( dwOptionTemp <= (DWORD)(pAddOpt3Rate[0] + pAddOpt3Rate[1] + pAddOpt3Rate[2]) )
		{
			Op3 = 2;
		}
		// ----
		if( dwOptionTemp <= (DWORD)(pAddOpt3Rate[0] + pAddOpt3Rate[1] + pAddOpt3Rate[2] + pAddOpt3Rate[3]) )
		{
			Op3 = 3;
		}
	}
	// ----
#pragma message("#### FIX ME ####")
	//ItemSerialCreateSend(lpObj->m_Index, 0xFF, 0, 0, Type, level, dur, Op1, Op2, Op3, lpObj->m_Index, ExOption, SetOption, 0, 0, 0);
	ItemSerialCreateSend(lpObj->m_Index, 0xFF, 0, 0, Type, level, dur, Op1, Op2, Op3, lpObj->m_Index, ExOption, SetOption);
	// ----
	int	tmpSetOption = 0;
	// ----
	if( (SetOption & 0x01) != 0 )
	{
		tmpSetOption = 1;
	}
	else if( (SetOption & 0x02) != 0 )
	{
		tmpSetOption = 2;
	}
	// ----
	char szSetItemName[48] = { 0 };
	strcpy(szSetItemName, gSetItemOption.GetSetOptionName(Type, tmpSetOption));
	strcat(szSetItemName, ItemAttribute[pLuckyItemEquipment->m_Index].Name);
	LogAddTD("[LuckyItem] - Exchange End [%s][%s][%d] ItemName[%s] ItemNum[%d] Level[%d] Dur[%d] skill[%d] luck[%d] option[%d] ExOpt[%d] SetOption[%d]",
		lpObj->AccountID, lpObj->Name, lpObj->Class, szSetItemName, Type, level, dur, Op1, Op2, Op3, ExOption, SetOption);
	gObjInventoryCommit(lpObj->m_Index);
}
// -------------------------------------------------------------------------------

//005afb00	-> 100%
BYTE LuckyItemManager::GambleGetLevel(_ITEM_LEVEL_RATE * pItemLevelRandRate, int iIndex)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	BYTE level			= 0; //ebp-8
	long dwRandRate		= GetLargeRand() % 1000000; //ebp-0c
	long dwLevelTemp	= 0; //ebp-10
	// ----
	for( int i = 0; i < MAX_LUCKYINFO_LEVELRATE; i++ ) //ebp-14
	{
		dwLevelTemp += pItemLevelRandRate[i].m_Rate;
		// ----
		if( dwRandRate <= dwLevelTemp )
		{
			level = (BYTE)pItemLevelRandRate[i].m_Level;
			break;
		}
	}
	// ----
	return level;
}
// -------------------------------------------------------------------------------

//005afba0	->
void LuckyItemManager::LuckyItemSmelting(LPOBJ lpObj)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	int iValidItemCount		= 0;
	BYTE iInvalidItemCount	= 0;
	// ----
	CItem NewItem;
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	// ----
	pMsg.Result			= CB_ERROR;
	lpObj->ChaosLock	= true;
	int iItemPos		= -1;
	// ----
	for( int i = 0; i < CHAOS_BOX_SIZE; i++ )
	{
		if( lpObj->pChaosBox[i].IsItem() )
		{
			int bLuckyItemEquipment = this->IsLuckyItemEquipment(lpObj->pChaosBox[i].m_Type);
			// ----
			if( !bLuckyItemEquipment )
			{
				iInvalidItemCount++;
			}
			else
			{
				iValidItemCount++;
				iItemPos = i;
			}
		}
	}
	// ----
	if( iValidItemCount != 1 || iInvalidItemCount != 0 || iItemPos == -1 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = false;
		return;
	}
	else
	{
		LogAddTD("[LuckyItemSmelting] - Smelting Start");
		g_MixSystem.LogChaosItem(lpObj, "[LuckyItem] Item Smelting");
		// ----
		int bGambleLuckyItemSmelting = this->GambleLuckyItemSmelting(lpObj->pChaosBox[iItemPos].m_Type, (int)lpObj->pChaosBox[iItemPos].m_Durability);
		this->GDReqLuckyItemDelete(lpObj->pChaosBox[iItemPos].m_Type, lpObj->pChaosBox[iItemPos].m_Number, lpObj->m_Index);
		// ----
		int iCharType	= 0;
		int Type		= ITEMGET(14, 160);
		BYTE level		= 0;
		BYTE dur		= 0;
		BYTE Op1		= 0;
		BYTE Op2		= 0;
		BYTE Op3		= 0;
		BYTE ExOption	= 0;
		// ----
		if( !bGambleLuckyItemSmelting )
		{
			g_MixSystem.ChaosBoxInit(lpObj);
			GCUserChaosBoxSend(lpObj, 0);
			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			// ----
			LogAddTD("[LuckyItem][Smelt Item Mix] Mix Fail [%s][%s] ", lpObj->AccountID, lpObj->Name);
			// ----
			GCServerMsgStringSend(lMsg.Get(3375), lpObj->m_Index, 0x01);
			lpObj->ChaosLock = false;
			return;
		}
		else
		{
#pragma message("#### FIX ME ####")
			//ItemSerialCreateSend(lpObj->m_Index, 0xFF, 0, 0, Type, level, 1, Op1, Op2, Op3, lpObj->m_Index, ExOption, 0, 0, 0, 0);
			ItemSerialCreateSend(lpObj->m_Index, 0xFF, 0, 0, Type, level, 1, Op1, Op2, Op3, lpObj->m_Index, ExOption, 0);			
		}
		// ----
		LogAddTD("[LuckyItem][Smelt Item Mix] Mix Success [%s][%s] ItemName[%s] ItemNum[%d] Level[%d] Dur[%d] skill[%d] luck[%d] option[%d] ExOpt[%d]",
			lpObj->AccountID, lpObj->Name, ItemAttribute[Type].Name, Type, level, dur, Op1, Op2, Op3, ExOption);
		// ----
		gObjInventoryCommit(lpObj->m_Index);
		GCServerMsgStringSend(lMsg.Get(3376), lpObj->m_Index, 0x01);
		return;
	}
}
// -------------------------------------------------------------------------------

//005b0070	-> 100%
int LuckyItemManager::GambleLuckyItemSmelting(int iItemNum, int iDur)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	int iRandRate			= 0;	//ebp-0c
	_SMELT_RATE * SmeltRate = this->GetSmeltingRateInfo(iItemNum);	//ebp-8
	// ----
	for( int i = 0; i < MAX_LUCKYINFO_SMELTRATE; i++ )	//ebp-10
	{
		if( iDur <= SmeltRate[i].MaxDur && iDur >= SmeltRate[i].MinDur )
		{
			iRandRate = SmeltRate[i].SmeltRate;
			break;
		}
	}
	// ----
	int iRand = (int)(rand() / 32767.0f * 99.0f);
	LogAddTD("[LuckyItem] - Gamble Smelting iRand %d iRandRate %d Dur %d", iRand, iRandRate, iDur);
	// ----
	if( iRand < iRandRate )
	{
		return true;
	}
	// -----
	return false;
}
// -------------------------------------------------------------------------------

//005b0170	-> 100%
int LuckyItemManager::GetLuckyItemDurabilityTime()	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	return g_LuckyItemDurabilityTime;
}
// -------------------------------------------------------------------------------

//005b0190	-> 100%
BYTE LuckyItemManager::GetLevel(int iMinLv, int iMaxLv)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	if( iMinLv == iMaxLv )
	{
		return iMinLv;
	}
	// ----
	int sub		= iMaxLv - iMinLv + 1;		//ebp-8
	int level	= rand() % sub + iMinLv;	//ebp-0c
	// ----
	return level;
}
// -------------------------------------------------------------------------------

//005b01f0	-> 100%
int LuckyItemManager::LuckyItemRepaire(OBJECTSTRUCT * lpObj, int source, int target)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	int iAddDur = 255;	//ebp-8
	// ----
	if( source < 0 || source > MAIN_INVENTORY_SIZE-1 )
	{
		return false;
	}
	// ----
	if( target < 0 || target > MAIN_INVENTORY_SIZE-1 )
	{
		return false;
	}
	// ----
	if( !lpObj->pInventory[source].IsItem() || !lpObj->pInventory[target].IsItem() )
	{
		return false;
	}
	// ----
	CItem * LuckyItemEquipment	= &lpObj->pInventory[target];	//ebp-0c
	int	bLuckyItemEquipment		= this->IsLuckyItemEquipment(LuckyItemEquipment->m_Type);	//ebp-10
	// ----
	if( !bLuckyItemEquipment )
	{
		GCServerMsgStringSend(lMsg.Get(3474), lpObj->m_Index, 1);
		LogAddTD("[LuckyItem][RepaireItemUsed] - Fail - Not LuckyItem Equipment [%s][%s] Serial[%u]",
			lpObj->AccountID, lpObj->Name, LuckyItemEquipment->m_serial);
		return false;
	}
	// ----
	if( LuckyItemEquipment->m_Durability == 0.0f )
	{
		GCServerMsgStringSend(lMsg.Get(3475), lpObj->m_Index, 1);
		LogAddTD("[LuckyItem][RepaireItemUsed] - Fail - Durability Zero [%s][%s] Serial[%u]",
			lpObj->AccountID, lpObj->Name, LuckyItemEquipment->m_serial);
		return false;
	}
	// ----
	if( ItemAttribute[LuckyItemEquipment->m_Type].Durability < LuckyItemEquipment->m_Durability )
	{
		GCServerMsgStringSend(lMsg.Get(3475), lpObj->m_Index, 1);
		LogAddTD("[LuckyItem][RepaireItemUsed] - Fail - Equipment Full Durability [%s][%s] Serial[%u]",
			lpObj->AccountID, lpObj->Name, LuckyItemEquipment->m_serial);
		return false;
	}
	// ----
	if( ItemAttribute[LuckyItemEquipment->m_Type].Durability >= LuckyItemEquipment->m_Durability + 255.0f )
	{
		LuckyItemEquipment->m_Durability += 255.0f;
	}
	else
	{
		LuckyItemEquipment->m_Durability = ItemAttribute[LuckyItemEquipment->m_Type].Durability;
	}
	// ----
	GCServerMsgStringSend(lMsg.Get(3478), lpObj->m_Index, 1);
	LogAddTD("[LuckyItem][RepaireItemUsed] - Success - [%s][%s] Serial[%u]",
		lpObj->AccountID, lpObj->Name, LuckyItemEquipment->m_serial);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005b0520	-> 100%
int LuckyItemManager::GDReqLuckyItemInsert(CItem * pItem, int iObjIndex)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	WORD wItemCode			= pItem->m_Type;	//ebp-8
	long dwSerial			= pItem->m_serial;	//ebp-0c
	WORD wDurabilitySmall	= pItem->m_DurabilitySmall;	//ebp-10
	// ----
	return this->GDReqLuckyItemInsert(wItemCode, dwSerial, wDurabilitySmall, iObjIndex);
}
// -------------------------------------------------------------------------------

//005b0580	-> 100%
int LuckyItemManager::GDReqLuckyItemInsert(short wItemCode, long dwSerial, short wDurabilitySmall, int iObjIndex)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	LPOBJ lpObj = &gObj[iObjIndex];
	PMSG_REQ_LUCKYITEM_INSERT pMsg;
	// ----
	pMsg.dwUserGuid	= lpObj->DBNumber;
	memcpy(pMsg.szCharName, lpObj->Name, 10);
	pMsg.LuckyItemDBInfo.wItemCode			= wItemCode;
	pMsg.LuckyItemDBInfo.dwSerial			= dwSerial;
	pMsg.LuckyItemDBInfo.wDurabilitySmall	= wDurabilitySmall;
	// ----
	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x22, sizeof(PMSG_REQ_LUCKYITEM_INSERT));
	cDBSMng.Send((char *)&pMsg, pMsg.head.size);
	return true;
}
// -------------------------------------------------------------------------------

//005b0640	-> 100%
void LuckyItemManager::GDReqLuckyItemInsert2nd(int iObjIndex)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	int	iItemType[5];
	// ----
	LPOBJ lpObj		= &gObj[iObjIndex];
	iItemType[0]	= 2;
	iItemType[1]	= 3;
	iItemType[2]	= 4;
	iItemType[3]	= 5;
	iItemType[4]	= 6;
	int iItemCnt	= 0;
	// ----
	PMSG_REQ_LUCKYITEM_INSERT_2ND pMsg;
	// ----
	for( int i = 0; i < 5; i++ )
	{
		CItem pItem = lpObj->pInventory[iItemType[i]];
		// ----
		if( this->IsLuckyItemEquipment(pItem.m_Type) )
		{
			pMsg.LuckyItemDBInfo[iItemCnt].dwSerial			= pItem.m_Number;
			pMsg.LuckyItemDBInfo[iItemCnt].wDurabilitySmall	= pItem.m_DurabilitySmall;
			pMsg.LuckyItemDBInfo[iItemCnt].wItemCode		= pItem.m_Type;
			iItemCnt++;
		}
	}
	// ----
	pMsg.btItemCnt	= iItemCnt;
	pMsg.dwUserGuid	= lpObj->DBNumber;
	memcpy(pMsg.szCharName, lpObj->Name, 10);
	// ----
 	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x24, 80);
 	cDBSMng.Send((LPSTR)&pMsg, pMsg.head.size);
}
// -------------------------------------------------------------------------------

//005b07c0	-> 100%
void LuckyItemManager::GDReqLuckyItemDelete(short wItemCode, long dwSerial, int iObjIndex)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	LPOBJ lpObj = &gObj[iObjIndex];
	PMSG_REQ_LUCKYITEM_DELETE pMsg;
	// ----
	pMsg.wUserIndex = lpObj->m_Index;
	memcpy(pMsg.szCharName, lpObj->Name, 10);
	pMsg.wItemCode	= wItemCode;
	pMsg.dwSerial	= dwSerial;
	// ----
	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x23, sizeof(PMSG_REQ_LUCKYITEM_DELETE));
	cDBSMng.Send((char *)&pMsg, pMsg.head.size);
	// ----
	LogAddTD("[LuckyItem] GDReqLuckyItemDelete [%s][%s] ItemCode[%d] Serial[%u]",
		lpObj->AccountID, lpObj->Name, wItemCode, dwSerial);
}
// -------------------------------------------------------------------------------

//005b08a0	->
void LuckyItemManager::DGAnsLuckyItemList(PMSG_ANS_LUCKYITEM_SELECT * lpRecv)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	int iItemCnt	= lpRecv->btItemCnt;	//ebp-8
	int iObjIndex	= lpRecv->wUserIndex;	//ebp-0c
	// -----
	if( iItemCnt < 0 )
	{
		return;
	}
	// ----
	if( !OBJMAX_RANGE(iObjIndex) || !gObjIsConnected(iObjIndex) )
	{
		return;
	}
	// ----
	BYTE iDeleteItemList[12] = { 0 };
	PMSG_LUCKYITME_DB_INFO LuckyItemList[45];
	memcpy(LuckyItemList, &lpRecv[1], iItemCnt * 12);
	// ----
	for( int iInven = 0; iInven < MAIN_INVENTORY_SIZE; iInven++ )
	{
		if( gObj[iObjIndex].pInventory[iInven].IsItem() )
		{
			CItem * pItem = &gObj[iObjIndex].pInventory[iInven];
			// -----
			for( int i = 0; i < iItemCnt; i++ )
			{
				if( pItem->m_Type == LuckyItemList[i].wItemCode 
					&& pItem->m_Number == LuckyItemList[i].dwSerial )
				{
					pItem->m_DurabilitySmall = LuckyItemList[i].wDurabilitySmall;
					iDeleteItemList[i]		= 1;
					break;
				}
			}
		}
	}
	// ----
	for( int j = 0; j < iItemCnt; j++ )
	{
		if( iDeleteItemList[j] == 0 )
		{
			this->GDReqLuckyItemDelete(LuckyItemList[j].wItemCode, LuckyItemList[j].dwSerial, iObjIndex);
		}
	}
}
// -------------------------------------------------------------------------------

//005b0b80	-> 100%
void LuckyItemManager::GDReqLuckyItemSelect(OBJECTSTRUCT * lpObj)	//OK
{
#if( LUCKYITEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Start", __FUNCTION__);
#endif
	PMSG_REQ_LUCKYITEM_SELECT pMsg;
	// ----
	pMsg.wUserIndex = lpObj->m_Index;
	memcpy(pMsg.chCharacterName, lpObj->Name, 10);
	// ----
	PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x20, sizeof(PMSG_REQ_LUCKYITEM_SELECT));
	cDBSMng.Send((char *)&pMsg, pMsg.head.size);
}
// -------------------------------------------------------------------------------