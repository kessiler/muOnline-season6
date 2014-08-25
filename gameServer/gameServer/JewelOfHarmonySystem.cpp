//1.01.00 ok

#include "stdafx.h"
#include "JewelOfHarmonySystem.h"
#include "Gamemain.h"
#include "LogProc.h"
#include "CastleSiegeSync.h"
#include "..\include\Readscript.h"
#include "..\common\winutil.h"
#include "DSProtocol.h"
#include "MixSystem.h"
#include "LuckyItem.h"

CJewelOfHarmonySystem g_kJewelOfHarmonySystem;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJewelOfHarmonySystem::CJewelOfHarmonySystem()
{
	this->m_bEnable = TRUE;
	this->_InitOption();
}

CJewelOfHarmonySystem::~CJewelOfHarmonySystem()
{
	return;
}


//00637080	-> OK (need check memset)
void CJewelOfHarmonySystem::_InitOption()
{
	this->JEWEL_OF_HARMONY_ITEMINDEX			= ITEMGET(14,41);
	this->JEWEL_OF_HARMONY_PURITY_ITEMINDEX		= ITEMGET(14,42);
	this->JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX	= ITEMGET(14,43);
	this->JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX	= ITEMGET(14,44);

	memset(this->m_itemOption, 0, sizeof(this->m_itemOption));	//0x5300

	this->m_bSystemPrutiyJewel = GetPrivateProfileInt("GameServerInfo", "PurityHarmonyJewel", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_bSystemMixSmeltingStone = GetPrivateProfileInt("GameServerInfo", "MixSmeltingStoneItem", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_bSystemRestoreStrengthen = GetPrivateProfileInt("GameServerInfo", "RestoreStrengthenItem", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_bSystemStrengthenItem = GetPrivateProfileInt("GameServerInfo", "StrengthenItem", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_bSystemSmeltingItem = GetPrivateProfileInt("GameServerInfo", "SmeltItemBySmeltingStone", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_iRatePuritySuccess = GetPrivateProfileInt("GameServerInfo", "PuritySuccessRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_iZenForPurity = GetPrivateProfileInt("GameServerInfo", "PurityNeedZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_iRateMixSmeltingStoneNor = GetPrivateProfileInt("GameServerInfo", "SmeltingStoneSuccesRateNormal", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_iRateMixSmeltingStoneExt = GetPrivateProfileInt("GameServerInfo", "SmeltingStoneSuccesRateExcellent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_iZenForMixSmeltingStone = GetPrivateProfileInt("GameServerInfo", "SmeltingNeedZen", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_iRateStrengthenSuccess = GetPrivateProfileInt("GameServerInfo", "StrengthenSuccessRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_iRateSmeltingSuccessNor = GetPrivateProfileInt("GameServerInfo", "SmeltingItemSuccessRateNor", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_iRateSmeltingSuccessExt = GetPrivateProfileInt("GameServerInfo", "SmeltingItemSuccessRateExt", 0, gDirPath.GetNewPath("commonserver.cfg"));
}

//006373a0	-> OK
BOOL CJewelOfHarmonySystem::LoadScript(LPSTR lpszFileName)
{
	SMDToken Token;
	SMDFile = fopen(lpszFileName, "r");	//ok

	if ( SMDFile == NULL )
		return FALSE;

	this->_InitOption();


	while ( true )
	{
		Token = (SMDToken)GetToken();

		if ( Token == END )
			break;

		if ( Token == NUMBER )
		{
			int Type = TokenNumber;

			while ( true )
			{
				Token = (SMDToken)GetToken();
				int Index = TokenNumber;

				if ( Token == NAME )
				{
					if ( !strcmp("end", TokenString))
					{
						break;
					}
				}

				JEWELOFHARMONY_ITEM_OPTION * p = &this->m_itemOption[Type][Index];
				
				Token = (SMDToken)GetToken();
				strcpy(p->szOptionName, TokenString);

				Token = (SMDToken)GetToken();
				p->iRandomWeight = TokenNumber;

				Token = (SMDToken)GetToken();
				p->iRequireLevel = TokenNumber;

				for ( int i=0;i<MAX_JOH_ITEM_OPTION;i++)	//OK
				{
					Token = (SMDToken)GetToken();
					p->iItemEffectValue[i] = TokenNumber;

					Token = (SMDToken)GetToken();
					p->iZenForRestore[i] = TokenNumber;
				}

				p->bValid = TRUE;
			}
		}
	}
	// ----
	fclose(SMDFile); //wz mistake, fixed
	// ----
	return TRUE;
}

//00637a00	-> OK
BOOL CJewelOfHarmonySystem::LoadScriptOfSmelt(LPSTR lpszFileName)
{
	SMDToken Token;
	SMDFile = fopen(lpszFileName, "r");	//ok

	if ( SMDFile == NULL )
		return FALSE;

	this->m_mapEnableMixList.clear();

	while ( true )
	{
		Token = (SMDToken)GetToken();

		if ( Token == END )
			break;

		if ( Token == NUMBER )
		{
			int Type = TokenNumber;

			while ( true )
			{
				Token = (SMDToken)GetToken();
				int index = TokenNumber;

				if ( Token == NAME )
				{
					if ( !strcmp("end", TokenString))
					{
						break;
					}
				}

				Token = (SMDToken)GetToken();

				Token = (SMDToken)GetToken();

				int reqLev = TokenNumber;
				int idx = ITEMGET(Type, index);

				this->m_mapEnableMixList[idx]= reqLev;
			}
		}
	}
	// ----
	fclose(SMDFile);//wz mistake, fixed
	// ----
	return TRUE;
}


//00637b50	-> OK
BOOL CJewelOfHarmonySystem::IsJewelOfHarmonyOriginal(short type)
{
	BOOL bRet = FALSE;

	if ( type == this->JEWEL_OF_HARMONY_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}


//00637b90	-> OK
BOOL CJewelOfHarmonySystem::IsJewelOfHarmonyPurity(short type)
{
	BOOL bRet = FALSE;

	if ( type == this->JEWEL_OF_HARMONY_PURITY_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

//00637bd0	-> OK
BOOL CJewelOfHarmonySystem::IsJewelOfHarmonySmeltingItems(short type)
{
	BOOL bRet = FALSE;

	if ( this->_IsJewelOfHarmonySmeltingItemNor(type) == TRUE || this->_IsJewelOfHarmonySmeltingItemExt(type) == TRUE )
		bRet = TRUE;

	return bRet;
}

//00637c30	-> OK
BOOL CJewelOfHarmonySystem::_IsJewelOfHarmonySmeltingItemNor(short type)
{
	BOOL bRet = FALSE;

	if ( type == this->JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

//00637c70	-> OK
BOOL CJewelOfHarmonySystem::_IsJewelOfHarmonySmeltingItemExt(short type)
{
	BOOL bRet = FALSE;

	if ( type == this->JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX )
		bRet = TRUE;

	return bRet;
}

//00637CB0	-> OK
void CJewelOfHarmonySystem::SetEnableToUsePuritySystem(BOOL bEnable)
{
	this->m_bEnable = bEnable;

	LogAddTD("[JewelOfHarmony][PuritySystem] Enable %d", bEnable);
}

//00637CF0	-> OK
BOOL CJewelOfHarmonySystem::IsEnableToUsePuritySystem()
{
	return this->m_bEnable;
}

//00637D10	-> OK
void CJewelOfHarmonySystem::PurityJewelOfHarmony(LPOBJ lpObj)
{
	if ( this->m_bSystemPrutiyJewel != TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 50)), lpObj->m_Index, 1);
		return;
	}

	if ( this->IsEnableToUsePuritySystem() == FALSE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 50)), lpObj->m_Index, 1);
		LogAddTD("[JewelOfHarmony][%s][%s] Not Purtiy Time ", lpObj->AccountID, lpObj->Name);
		return;
	}

	lpObj->ChaosLock = TRUE;
	// Chaos Lock was Enabled
		int iJewelOfHarmonyItemCount = 0;
		int iInvalidItemCount = 0;
		int iChaosMixPrice = 0;
		PMSG_CHAOSMIXRESULT pMsg;
		PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(PMSG_CHAOSMIXRESULT));

		pMsg.Result = CB_ERROR;

		for ( int n =0;n<CHAOS_BOX_SIZE;n++)
		{
			if ( lpObj->pChaosBox[n].IsItem() == TRUE )
			{
				if ( lpObj->pChaosBox[n].m_Type == this->JEWEL_OF_HARMONY_ITEMINDEX )
				{
					iJewelOfHarmonyItemCount++;
				}
				else
				{
					iInvalidItemCount++;
				}
			}
		}

		if ( iInvalidItemCount > 0 || iJewelOfHarmonyItemCount !=1 )
		{
			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}

		iChaosMixPrice = this->m_iZenForPurity;
		int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

		if (iChaosTaxMoney < 0 )
			iChaosTaxMoney = 0;

		iChaosMixPrice += iChaosTaxMoney;

		if ( iChaosMixPrice < 0 )
			iChaosMixPrice = 0;

		if ( lpObj->Money < iChaosMixPrice )
		{
			pMsg.Result = CB_NOT_ENOUGH_ZEN;
			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}

		lpObj->Money -= iChaosMixPrice;
		g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
		GCMoneySend(lpObj->m_Index, lpObj->Money);
		g_MixSystem.LogChaosItem(lpObj, "JewelOfHarmony][Purity"); //LogChaosItem(lpObj, "JewelOfHarmony][Purity");

		LogAddTD("[JewelOfHarmony][Purity] - Mix Start");


		int iRate = rand() % 100;

		if ( iRate < this->m_iRatePuritySuccess )
		{
			int iItemType = this->JEWEL_OF_HARMONY_PURITY_ITEMINDEX;
			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iItemType, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0);
			gObjInventoryCommit(lpObj->m_Index);

			LogAddTD("[JewelOfHarmony][Purity] Purity Success [%s][%s] Rate %d/%d",
				lpObj->AccountID, lpObj->Name, iRate, this->m_iRatePuritySuccess);
		}
		else
		{
			g_MixSystem.ChaosBoxInit(lpObj); //ChaosBoxInit(lpObj);
			GCUserChaosBoxSend(lpObj, 0);
			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			LogAddTD("[JewelOfHarmony][Purity] Purity Fail [%s][%s] Rate %d/%d",
				lpObj->AccountID, lpObj->Name, iRate, this->m_iRatePuritySuccess);
		}
	// Chaos Lock was Disabled
	lpObj->ChaosLock = FALSE;
}

//00638160	-> OK
BYTE CJewelOfHarmonySystem::GetItemStrengthenOption(CItem *pItem)
{
	return ( pItem->m_JewelOfHarmonyOption & 0xF0 ) >> 4;
}

//006381A0	-> OK
BYTE CJewelOfHarmonySystem::GetItemOptionLevel(CItem *pItem)
{
	return this->_GetItemOptionLevel(pItem);
}

//006381D0	-> OK
BYTE CJewelOfHarmonySystem::_GetItemOptionLevel(CItem *pItem)
{
	return ( pItem->m_JewelOfHarmonyOption & 0x0F ) ;
}

//00638200	-> OK
BOOL CJewelOfHarmonySystem::IsStrengthenByJewelOfHarmony(CItem *pItem)
{
	if ( this->GetItemStrengthenOption(pItem) >0  )
		return TRUE;

	return FALSE;
}

//00638240	-> OK
BOOL CJewelOfHarmonySystem::IsActive(CItem *pItem)
{
	if ( this->IsStrengthenByJewelOfHarmony(pItem) == FALSE )
		return FALSE;

	int iOptionLevel = this->_GetItemOptionLevel(pItem);

	if ( pItem->m_Level < iOptionLevel )
		return FALSE;

	return TRUE;
}

//006382B0	-> OK
BYTE CJewelOfHarmonySystem::_GetItemOptionRequireLevel(CItem * pItem)
{
	int iItemType = this->_GetItemType(pItem);
	BYTE iItemOption = this->GetItemStrengthenOption(pItem);

	if ( iItemType == 0 || iItemOption > MAX_JOH_ITEM_OPTION+1 || iItemOption == 0 )
		return 0;

	return this->m_itemOption[iItemType][iItemOption].iRequireLevel;
}

//00638350	-> OK
int CJewelOfHarmonySystem::_GetItemType(CItem *pItem)
{
	int iItemType = JEWELOFHARMONY_ITEM_TYPE_NULL;


	if ( (pItem->m_Type >= ITEMGET(0,0) && pItem->m_Type <ITEMGET(4,0)) 
		|| (pItem->m_Type >= ITEMGET(4,0) && pItem->m_Type <ITEMGET(4,7)) 
		|| (pItem->m_Type >= ITEMGET(4,8) && pItem->m_Type <ITEMGET(4,15)) 
		|| (pItem->m_Type >= ITEMGET(4,16) && pItem->m_Type <ITEMGET(5,0)) )
	{
		iItemType =JEWELOFHARMONY_ITEM_TYPE_WEAPON;
	}

	if (pItem->m_Type >= ITEMGET(5,0) && pItem->m_Type <ITEMGET(6,0) )
	{
		iItemType =JEWELOFHARMONY_ITEM_TYPE_STAFF;
	}

	if (pItem->m_Type >= ITEMGET(6,0) && pItem->m_Type <ITEMGET(12,0) )
	{
		iItemType =JEWELOFHARMONY_ITEM_TYPE_DEFENSE;
	}

	return iItemType;
}

//00638470	-> 
int CJewelOfHarmonySystem::_GetSelectRandomOption(CItem * pItem, int iItemType)
{
	if ( iItemType == 0 )
		return iItemType;

	int iItemOption = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;
	this->m_kRandomPool.InitPool();

	for ( int idx =1;idx<MAX_JOH_ITEM_INDEX;idx++)
	{
		if ( this->m_itemOption[iItemType][idx].bValid == FALSE )
			continue;

		if ( pItem->m_Level < this->m_itemOption[iItemType][idx].iRequireLevel )
			continue;

		if ( iItemOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR || iItemOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR)
		{
			if ( pItem->m_RequireStrength == 0 )
				continue;
		}

		if ( iItemOption == AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX || iItemOption == AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX)
		{
			if ( pItem->m_RequireDexterity == 0 )
				continue;
		}

		this->m_kRandomPool.AddValue(idx, this->m_itemOption[iItemType][idx].iRandomWeight);
	}

	iItemOption = this->m_kRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
	return iItemOption;
}

//006385E0	-> OK [NEW]
BOOL CJewelOfHarmonySystem::StrengthenItemByJewelOfRise(LPOBJ lpObj, int source, int target)
{
	if ( this->m_bSystemStrengthenItem == FALSE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 52)), lpObj->m_Index, 1);
		return FALSE;
	}

	if ( source < 0 || source > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( target < 0 || target > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( lpObj->pInventory[source].IsItem() == FALSE )
		return FALSE;

	if ( lpObj->pInventory[target].IsItem() == FALSE )
		return FALSE;

	CItem * pSource = &lpObj->pInventory[source];
	CItem * pTarget = &lpObj->pInventory[target];

	if( g_LuckyItemManager.IsLuckyItemEquipment(pTarget->m_Type) )
	{
		return false;
	}

	if ( this->IsStrengthenByJewelOfHarmony(pTarget) == TRUE )
	{
		LogAddTD("[LuckyItem][Strengthen Item] Already Strengtened [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		return FALSE;
	}

	int iItemType = this->_GetItemType(pTarget);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		LogAddTD("[LuckyItem][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) Invalid ItemType[%d]",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, pSource->m_Number, iItemType);
		
		return FALSE;
	}

	int iItemOption = this->_GetSelectRandomOption(pTarget, iItemType);

	if ( iItemOption == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		LogAddTD("[LuckyItem][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) ItemType[%d]",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, pSource->m_Number, iItemType);
		
		return FALSE;
	}

	int iItemOptionLevel = this->m_itemOption[iItemType][iItemOption].iRequireLevel;
	int iSuccessRate = rand() % 100;

	if ( iSuccessRate >= this->m_iRateStrengthenSuccess )
	{
		LogAddTD("[LuckyItem][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) Rate (%d/%d)",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, pSource->m_Number, iSuccessRate, this->m_iRateStrengthenSuccess);
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 45)), lpObj->m_Index, 1);
		return TRUE;
	}


	this->_MakeOption(pTarget, iItemOption, iItemOptionLevel);

	LogAddTD("[LuckyItem][Strengthen Item] Strengthen Success [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) Rate (%d/%d) Option %d OptionLevel %d",
		lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type, pTarget->m_Number,
		pSource->m_Number, iSuccessRate, this->m_iRateStrengthenSuccess,
		iItemOption, iItemOptionLevel);

	GCServerMsgStringSend(lMsg.Get(MSGGET(13, 46)), lpObj->m_Index, 1);

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,
		lpObj->pInventory[target].m_Level, lpObj->pInventory[target].IsExtItem(),
		lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type,
		lpObj->pInventory[target].m_Option1, lpObj->pInventory[target].m_Option2,
		lpObj->pInventory[target].m_Option3, lpObj->pInventory[target].m_NewOption,
		lpObj->pInventory[target].m_SetOption, lpObj->pInventory[target].m_ItemOptionEx, NULL, 0xFF, 0, 3);


	return TRUE;
}

//00638C00	->
BOOL CJewelOfHarmonySystem::StrengthenItemByJewelOfHarmony(LPOBJ lpObj, int source, int target)
{
	if ( this->m_bSystemStrengthenItem == FALSE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 52)), lpObj->m_Index, 1);
		return FALSE;
	}

	if ( source < 0 || source > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( target < 0 || target > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( lpObj->pInventory[source].IsItem() == FALSE )
		return FALSE;

	if ( lpObj->pInventory[target].IsItem() == FALSE )
		return FALSE;

	CItem * pSource = &lpObj->pInventory[source];
	CItem * pTarget = &lpObj->pInventory[target];

	if ( this->IsStrengthenByJewelOfHarmony(pTarget) == TRUE )
	{
		LogAddTD("[JewelOfHarmony][Strengthen Item] Already Strengtened [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		return FALSE;
	}

	if( gAllowJOHonAncient == 0)
	{
		if (pTarget->IsSetItem() != FALSE)
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(13, 44)), lpObj->m_Index, 1);
			LogAddTD("[JewelOfHarmony][Strengthen Item] SetItem not enable to Strengthened [%s][%s]",
				lpObj->AccountID, lpObj->Name);

			return FALSE;
		}
	}

	int iItemType = this->_GetItemType(pTarget);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
	{
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) Invalid ItemType[%d]",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, pSource->m_Number, iItemType);
		
		return FALSE;
	}

	int iItemOption = this->_GetSelectRandomOption(pTarget, iItemType);

	if ( iItemOption == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
	{
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail - NOT OPTION [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) ItemType[%d]",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, pSource->m_Number, iItemType);
		
		return FALSE;
	}

	int iItemOptionLevel = this->m_itemOption[iItemType][iItemOption].iRequireLevel;
	int iSuccessRate = rand() % 100;

	if ( iSuccessRate >= this->m_iRateStrengthenSuccess )
	{
		LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Fail [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) Rate (%d/%d)",
			lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, pSource->m_Number, iSuccessRate, this->m_iRateStrengthenSuccess);
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 45)), lpObj->m_Index, 1);
		return TRUE;
	}


	this->_MakeOption(pTarget, iItemOption, iItemOptionLevel);

	LogAddTD("[JewelOfHarmony][Strengthen Item] Strengthen Success [%s][%s] Name[%s] Type[%d] Serial[%u] JewelSerial(%u) Rate (%d/%d) Option %d OptionLevel %d", //season 2.5 changed -> identical
		lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type, pTarget->m_Number,
		pSource->m_Number, iSuccessRate, this->m_iRateStrengthenSuccess,
		iItemOption, iItemOptionLevel);

	GCServerMsgStringSend(lMsg.Get(MSGGET(13, 46)), lpObj->m_Index, 1);

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,
		lpObj->pInventory[target].m_Level, lpObj->pInventory[target].IsExtItem(),
		lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	lpObj->pInventory[target].Convert(lpObj->pInventory[target].m_Type,
		lpObj->pInventory[target].m_Option1, lpObj->pInventory[target].m_Option2,
		lpObj->pInventory[target].m_Option3, lpObj->pInventory[target].m_NewOption,
		lpObj->pInventory[target].m_SetOption, lpObj->pInventory[target].m_ItemOptionEx, NULL, 0xFF, 0, 3);


	return TRUE;
}

//00639260	-> OK (Maybe not changed, need check again)
BOOL CJewelOfHarmonySystem::_MakeOption(CItem *pItem, BYTE btOptionType, BYTE btOptionLevel)
{
	pItem->m_JewelOfHarmonyOption = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;
	pItem->m_JewelOfHarmonyOption |= 16 * btOptionType/*btOptionType << 4*/;
	pItem->m_JewelOfHarmonyOption |= btOptionLevel & 0x0F;

	return TRUE;
}


#pragma warning ( disable : 4101 )
//006392E0	-> OK
void CJewelOfHarmonySystem::StrengthenItemByMacro(LPOBJ lpObj, BYTE invenrotyTargetPos, BYTE btOptionType,  BYTE btOptionLevel)
{
	return;
	
	int iType;
	CItem * pItem;
	char szMsg[256];
	{
		char szMsg[256];
	}
	{
		char szMsg[256];
	}
	{
		char szMsg[256];
	}
	{
		BYTE iValue;
	}
	{
		
		char szMsg[256];
	}
	{
		char szMsg[256];
	}
	{
		char szMsg[256];
	}
	{
		char szMsg	[256];
	}
}
#pragma warning ( default : 4101 )

//00639300	-> 
BYTE CJewelOfHarmonySystem::ShowStrengthenOption(CItem *pItem)
{
	BYTE bResult = -1;

	if ( this->IsStrengthenByJewelOfHarmony(pItem) == FALSE )
		return bResult;

	int type = this->_GetItemType(pItem);
	bResult = this->GetItemStrengthenOption(pItem);
	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);
	
	char buf[256];
	sprintf(buf, "??°? : %20s    type:%d, option:%d optionLevel %d value %d \n", 
		this->m_itemOption[type][bResult].szOptionName, type,
		bResult, iItemOptionLevel, this->m_itemOption[type][bResult].iItemEffectValue[iItemOptionLevel]);
	
	OutputDebugString(buf);
	return bResult;
}

//00639430	-> OK
BYTE CJewelOfHarmonySystem::MakeCharSetData(LPOBJ lpObj)
{
	BYTE btResult = 0;

	if ( this->IsActive(&lpObj->pInventory[0]) == TRUE )
	{
		int iOptionLevel = this->_GetItemOptionLevel(&lpObj->pInventory[0]);

		if ( iOptionLevel > 5 && iOptionLevel < 9 )
			btResult |= 0x40;
		else if ( iOptionLevel > 8 && iOptionLevel < 13 )
			btResult |= 0x80;
		else if ( iOptionLevel > 12 )
			btResult |= 0xC0;
	}

	if ( this->IsActive(&lpObj->pInventory[1]) == TRUE )
	{
		int iOptionLevel = this->_GetItemOptionLevel(&lpObj->pInventory[1]);

		if ( iOptionLevel > 5 && iOptionLevel < 9 )
			btResult |= 0x10;
		else if ( iOptionLevel > 8 && iOptionLevel < 13 )
			btResult |= 0x20;
		else if ( iOptionLevel > 12 )
			btResult |= 0x30;
	}

	return btResult;
}

//00639580	-> OK
void CJewelOfHarmonySystem::SetApplyStrengthenItem(LPOBJ lpObj)
{
	JEWELOFHARMONY_ITEM_EFFECT * pItemEffect = &lpObj->m_JewelOfHarmonyEffect;

	this->InitEffectValue(pItemEffect);

	int iItemIndex = 0;

	for ( iItemIndex =0;iItemIndex <INVETORY_WEAR_SIZE ; iItemIndex++)
	{
		if ( lpObj->pInventory[iItemIndex].IsItem() != FALSE )
		{
			if ( lpObj->pInventory[iItemIndex].m_IsValidItem !=  false )
			{
				BOOL bResult = this->_CalcItemEffectValue(&lpObj->pInventory[iItemIndex], pItemEffect);
			}
		}
	}

	lpObj->m_AttackDamageMinLeft += pItemEffect->HJOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinLeft += pItemEffect->HJOpAddAttackDamage;
	lpObj->m_AttackDamageMinRight += pItemEffect->HJOpAddMinAttackDamage;
	lpObj->m_AttackDamageMinRight += pItemEffect->HJOpAddAttackDamage;

	lpObj->m_AttackDamageMaxLeft += pItemEffect->HJOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxLeft += pItemEffect->HJOpAddAttackDamage;
	lpObj->m_AttackDamageMaxRight += pItemEffect->HJOpAddMaxAttackDamage;
	lpObj->m_AttackDamageMaxRight += pItemEffect->HJOpAddAttackDamage;

	lpObj->m_MagicDamageMin += pItemEffect->HJOpAddMagicPower;
	lpObj->m_MagicDamageMax += pItemEffect->HJOpAddMagicPower;

	lpObj->m_Defense += pItemEffect->HJOpAddDefense * 10 / 20;

	lpObj->AddBP += pItemEffect->HJOpAddMaxAG;
	
	lpObj->AddLife += pItemEffect->HJOpAddMaxHP;

	lpObj->DamageMinus += pItemEffect->HJOpAddDamageDecrease;
}

//00639840	-> OK
int CJewelOfHarmonySystem::GetItemEffectValue(CItem * pItem, int iOptionType)
{
	int iItemType = this->_GetItemType(pItem);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return 0;

	int iItemOptionType = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;

	iItemOptionType = this->GetItemStrengthenOption(pItem);

	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM ||iItemOptionType != iOptionType )
		return 0;

	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);

	if ( iItemOptionLevel >= MAX_JOH_ITEM_OPTION )
		return 0;

	int iItemEffectValue = this->m_itemOption[iItemType][iItemOptionType].iItemEffectValue[iItemOptionLevel];
	return iItemEffectValue;
}

//00639910	-> OK
BOOL CJewelOfHarmonySystem::_CalcItemEffectValue(CItem *pItem, JEWELOFHARMONY_ITEM_EFFECT * pItemEffect)
{
	int iItemType = this->_GetItemType(pItem);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return 0;

	int iItemOptionType = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;

	iItemOptionType = this->GetItemStrengthenOption(pItem);

	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
		return 0;

	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);

	if ( iItemOptionLevel >= MAX_JOH_ITEM_OPTION )
		return 0;


	if ( this->IsActive(pItem) == FALSE )
		return FALSE;

	int iItemEffectValue = this->m_itemOption[iItemType][iItemOptionType].iItemEffectValue[iItemOptionLevel];

	BOOL bResult = TRUE;

	switch ( iItemType )
	{
		case JEWELOFHARMONY_ITEM_TYPE_WEAPON:
			switch (iItemOptionType)
			{
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE:
					pItemEffect->HJOpAddMinAttackDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_MAXATTACKDAMAGE:
					pItemEffect->HJOpAddMaxAttackDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_STR:
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_REQUIRE_DEX:
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKDAMAGE_BOTH:
					pItemEffect->HJOpAddMinAttackDamage += iItemEffectValue;
					pItemEffect->HJOpAddMaxAttackDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_CRITICALDAMAGE:
					pItemEffect->HJOpAddCriticalDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_SKILLDAMAGE:
					pItemEffect->HJOpAddSkillAttack += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_ATTACKSUCCESRATE_PVP:
					pItemEffect->HJOpAddAttackSuccessRatePVP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_DECREASE_SD_RATE:
					pItemEffect->HJOpDecreaseSDRate += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_WEAPON_IMPROVE_IGNORE_SD_RATE:
					pItemEffect->HJOpAddIgnoreSDRate += iItemEffectValue;
					break;
				default:
					bResult = FALSE;
			}
			break;

		case JEWELOFHARMONY_ITEM_TYPE_STAFF:
			switch (iItemOptionType)
			{
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_MAGICDAMAGE:
					pItemEffect->HJOpAddMagicPower += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_STR:
				case AT_JEWELOFHARMONY_STAFF_DECREASE_REQUIRE_DEX:
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_SKILLDAMAGE:
					pItemEffect->HJOpAddSkillAttack += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_CRITICALDAMAGE:
					pItemEffect->HJOpAddCriticalDamage += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_DECREASE_SD_RATE:
					pItemEffect->HJOpDecreaseSDRate += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_ATTACKSUCCESRATE_PVP:
					pItemEffect->HJOpAddAttackSuccessRatePVP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_STAFF_IMPROVE_IGNORE_SD_RATE:
					pItemEffect->HJOpAddIgnoreSDRate += iItemEffectValue;
					break;
				default:
					bResult = FALSE;
			}
			break;

		case JEWELOFHARMONY_ITEM_TYPE_DEFENSE:
			switch (iItemOptionType)
			{
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DEFENSE:
					pItemEffect->HJOpAddDefense += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_AG:
					pItemEffect->HJOpAddMaxAG += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_MAX_HP:
					pItemEffect->HJOpAddMaxHP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_HP:
					pItemEffect->HJOpAddRefillHP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_AUTO_REFILL_MP:
					pItemEffect->HJOpAddRefillMP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_BLOCKSUCCESRATE_PVP:
					pItemEffect->HJOpAddDefenseSuccessRatePvP += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_DAMAGE_DECREASE:
					pItemEffect->HJOpAddDamageDecrease += iItemEffectValue;
					break;
				case AT_JEWELOFHARMONY_DEFENSE_IMPROVE_SD_RATE:
					pItemEffect->HJOpAddSDRate += iItemEffectValue;
					break;
				default:
					bResult = FALSE;
			}
			break;

		default:
			bResult = FALSE;
	}

	return bResult;
}

//00639DC0	-> OK
void CJewelOfHarmonySystem::InitEffectValue(JEWELOFHARMONY_ITEM_EFFECT * pItemEffect)
{
	pItemEffect->HJOpAddMinAttackDamage = 0;
	pItemEffect->HJOpAddMaxAttackDamage = 0;
	pItemEffect->HJOpRequireStr = 0;
	pItemEffect->HJOpRequireDex = 0;
	pItemEffect->HJOpAddAttackDamage = 0;
	pItemEffect->HJOpAddCriticalDamage = 0;
	pItemEffect->HJOpAddSkillAttack = 0;
	pItemEffect->HJOpAddAttackSuccessRatePVP = 0;
	pItemEffect->HJOpDecreaseSDRate = 0;
	pItemEffect->HJOpAddIgnoreSDRate = 0;
	pItemEffect->HJOpAddMagicPower = 0;
	pItemEffect->HJOpAddDefense = 0;
	pItemEffect->HJOpAddMaxAG = 0;
	pItemEffect->HJOpAddMaxHP = 0;
	pItemEffect->HJOpAddRefillHP = 0;
	pItemEffect->HJOpAddRefillMP = 0;
	pItemEffect->HJOpAddDefenseSuccessRatePvP = 0;
	pItemEffect->HJOpAddDamageDecrease = 0;
	pItemEffect->HJOpAddSDRate = 0;
}

//00639EB0	-> OK
BOOL CJewelOfHarmonySystem::IsEnableToMakeSmeltingStoneItem(CItem * pItem)
{
	if ( this->_GetItemType(pItem) == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return FALSE;

	if ( this->IsStrengthenByJewelOfHarmony(pItem) == TRUE )
		return FALSE;

	if ( pItem->IsSetItem() != FALSE)
		return FALSE;

	std::map<int,int>::iterator iter = this->m_mapEnableMixList.find(pItem->m_Type);

	if ( iter != this->m_mapEnableMixList.end() )
	{
		int reqLevel = iter->second;

		if ( pItem->m_Level < reqLevel )
			return FALSE;

	}

	return TRUE;
}

//00639FA0	-> OK
BOOL CJewelOfHarmonySystem::MakeSmeltingStoneItem(LPOBJ lpObj)
{
	if ( this->m_bSystemMixSmeltingStone != TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13,53)), lpObj->m_Index, 1);
		return TRUE;
	}

	lpObj->ChaosLock = TRUE;
	int iValidItemCount = 0;
	int iInvalidItemCount = 0;
	int iMakeSmeltingStoneMixPrice = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = 0;
	int iItemPos = -1;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( this->IsEnableToMakeSmeltingStoneItem(&lpObj->pChaosBox[n]) == TRUE )
			{
				iValidItemCount++;
				iItemPos = n;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iValidItemCount != 1 ||
		 iInvalidItemCount ||
		 iItemPos == -1 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	int JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE;
	BOOL bIsItemNormal = TRUE;

	if ( lpObj->pChaosBox[iItemPos].IsExtItem() == TRUE )
	{
		bIsItemNormal = FALSE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE = this->m_iRateMixSmeltingStoneExt;
	}
	else
	{
		bIsItemNormal = TRUE;
		JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE = this->m_iRateMixSmeltingStoneNor;
	}

	iMakeSmeltingStoneMixPrice = this->m_iZenForMixSmeltingStone;
	int iChaosTaxMoney = iMakeSmeltingStoneMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
		iChaosTaxMoney = 0;

	iMakeSmeltingStoneMixPrice += iChaosTaxMoney;

	if ( iMakeSmeltingStoneMixPrice < 0 )
		iMakeSmeltingStoneMixPrice = 0;

	if ( lpObj->Money < iMakeSmeltingStoneMixPrice )
	{
		pMsg.Result = 2;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	lpObj->Money -= iMakeSmeltingStoneMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	g_MixSystem.LogChaosItem(lpObj, "JewelOfHarmony][Smelt Item Mix"); //LogChaosItem(lpObj, "JewelOfHarmony][Smelt Item Mix");
	LogAddTD("[JewelOfHarmony][Smelt Item Mix] - Mix Start");

	int iRate = rand() % 100;

	if ( iRate < JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE )
	{
		int iItemType;

		if ( bIsItemNormal == TRUE )
			iItemType = this->JEWEL_OF_HARMONY_SMELT_NOR_ITEMINDEX;
		else
			iItemType = this->JEWEL_OF_HARMONY_SMELT_EXT_ITEMINDEX;

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, iItemType, 0,
							1, 0, 0, 0, lpObj->m_Index, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);

		LogAddTD("[JewelOfHarmony][Smelt Item Mix] Smelting Stone Normal[%d] Mix Success [%s][%s], Money(%d-%d) Rate(%d/%d)",
			bIsItemNormal, lpObj->AccountID, lpObj->Name,
			lpObj->Money, iMakeSmeltingStoneMixPrice, iRate,
			JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE);

		return TRUE; // Season 4.5 addon
	}
	else
	{
		g_MixSystem.ChaosBoxInit(lpObj); //ChaosBoxInit(lpObj);
		GCUserChaosBoxSend(lpObj, 0);
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		LogAddTD("[JewelOfHarmony][Smelt Item Mix] Smelting Stone Normal[%d] Mix Fail [%s][%s], Money : %d-%d Rate(%d/%d)",
			bIsItemNormal, lpObj->AccountID, lpObj->Name,
			lpObj->Money, iMakeSmeltingStoneMixPrice, iRate,
			JEWEL_OF_HARMONY_MAKE_SMELTINGSTONE_RATE);
	}

	lpObj->ChaosLock = FALSE;
	return TRUE;
}

//0063A450	->
BOOL CJewelOfHarmonySystem::SmeltItemBySmeltingStone(LPOBJ lpObj, int source, int target)
{
	if ( this->m_bSystemSmeltingItem == FALSE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13,51)), lpObj->m_Index, 1);
		return FALSE;
	}

	if ( source < 0 || source > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( target < 0 || target > MAIN_INVENTORY_SIZE-1 )
		return FALSE;

	if ( lpObj->pInventory[source].IsItem() == FALSE )
		return FALSE;

	if ( lpObj->pInventory[target].IsItem() == FALSE )
		return FALSE;

	CItem * pSource = &lpObj->pInventory[source];
	CItem * pTarget = &lpObj->pInventory[target];

	if ( !this->IsStrengthenByJewelOfHarmony(pTarget)  )
	{
		LogAddTD("[JewelOfHarmony][Smelt Item] Not Strengthen Item [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		return FALSE;
	}

	int iItemOptionLevel = this->_GetItemOptionLevel(pTarget);

	if( iItemOptionLevel >= 13 )	//1.01.00
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13,41)), lpObj->m_Index, 1);
		LogAddTD("[JewelOfHarmony][Smelt Item] Already Have Max OptionLevel [%s][%s] OptionLevel [%d] ItemLevel [%d]",
			lpObj->AccountID, lpObj->Name, iItemOptionLevel, pTarget->m_Level);
		return false;
	}

	if ( iItemOptionLevel >= pTarget->m_Level )
	{
		if ( iItemOptionLevel == 13 )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(13,41)), lpObj->m_Index, 1);
		}
		else
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(13,40)), lpObj->m_Index, 1);
		}

		LogAddTD("[JewelOfHarmony][Smelt Item] Already Have Max OptionLevel [%s][%s] OptionLevel [%d] ItemLevel [%d]",
			lpObj->AccountID, lpObj->Name, iItemOptionLevel, pTarget->m_Level);

		return FALSE;
	}

	if ( this->_GetItemType(pTarget) == JEWELOFHARMONY_ITEM_TYPE_WEAPON )
	{
		if ( this->GetItemStrengthenOption(pTarget) == AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE )
		{
			int iNextLevel = this->_GetItemOptionLevel(pTarget)+1;
			BYTE iValue = this->m_itemOption[JEWELOFHARMONY_ITEM_TYPE_WEAPON][AT_JEWELOFHARMONY_WEAPON_IMPROVE_MINATTACKDAMAGE].iItemEffectValue[iNextLevel];

			if ( (pTarget->m_DamageMin+iValue) > (pTarget->m_DamageMax-1) )
			{
				GCServerMsgStringSend(lMsg.Get(MSGGET(13,41)), lpObj->m_Index, 1);
				return FALSE;
			}
		}
	}

	int iSuccessRate = rand() % 100;
	int iRateSmeltingSuccess = 0;
	BOOL bIsNormalSmeltingStone = this->_IsJewelOfHarmonySmeltingItemNor(pSource->m_Type);

	if ( bIsNormalSmeltingStone == TRUE )
		iRateSmeltingSuccess = this->m_iRateSmeltingSuccessNor;
	else
		iRateSmeltingSuccess = this->m_iRateSmeltingSuccessExt;

	if ( iSuccessRate >= iRateSmeltingSuccess )
	{
		int iItemOptionNewLevel = _GetItemOptionRequireLevel(pTarget);
		pTarget->m_JewelOfHarmonyOption = pTarget->m_JewelOfHarmonyOption & 0xF0;
		pTarget->m_JewelOfHarmonyOption |= iItemOptionNewLevel & 0x0F;
		this->ShowStrengthenOption(pTarget);

		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Fail by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%u] JewelSerial(%u) Rate(%d/%d) Level(%d->%d)",
			bIsNormalSmeltingStone, lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, pSource->m_Number, iSuccessRate, iRateSmeltingSuccess, iItemOptionLevel, iItemOptionNewLevel);
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 47)), lpObj->m_Index, 1);
	}
	else
	{
		int iItemOptionNewLevel = iItemOptionLevel+1;
		pTarget->m_JewelOfHarmonyOption = pTarget->m_JewelOfHarmonyOption & 0xF0;
		pTarget->m_JewelOfHarmonyOption |= iItemOptionNewLevel & 0x0F;
		this->ShowStrengthenOption(pTarget);
		int iItemOptionType = this->GetItemStrengthenOption(pTarget); //loc12

		LogAddTD("[JewelOfHarmony][Smelt Item] Smelt Item Success by Normal[%d] [%s][%s] Name[%s] Type [%d] Serial [%u] JewelSerial(%u) Rate(%d/%d) Option(%d) Level(%d->%d)",
			bIsNormalSmeltingStone, lpObj->AccountID, lpObj->Name, pTarget->GetName(), pTarget->m_Type,
			pTarget->m_Number, pSource->m_Number, iSuccessRate, iRateSmeltingSuccess, iItemOptionType, iItemOptionLevel, iItemOptionNewLevel & 0x0F);
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 48)), lpObj->m_Index, 1);
	}

	return TRUE;
}

//0063AA30	-> OK
int CJewelOfHarmonySystem::_GetZenForRestoreItem(CItem * pItem)
{
	int iItemType = this->_GetItemType(pItem);

	if ( iItemType == JEWELOFHARMONY_ITEM_TYPE_NULL )
		return -1;

	int iItemOptionType = AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM;
	iItemOptionType = this->GetItemStrengthenOption(pItem);

	if ( iItemOptionType == AT_JEWELOFHARMONY_NOT_STRENGTHEN_ITEM )
		return -1;

	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);

	if ( iItemOptionLevel >= MAX_JOH_ITEM_OPTION )
		return -1;

	int iZenForRestore = this->m_itemOption[iItemType][iItemOptionType].iZenForRestore[iItemOptionLevel];
	return iZenForRestore;
}

//0063AB00	-> OK
BOOL CJewelOfHarmonySystem::RestoreStrengthenItem(LPOBJ lpObj)
{
	if ( this->m_bSystemRestoreStrengthen != TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13,54)), lpObj->m_Index, 1);
		return TRUE;
	}

	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = 0;
	int iStrengtenItemCount = 0;
	int iInvalidItemCount = 0;
	CItem * pItem = NULL;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( this->IsStrengthenByJewelOfHarmony(&lpObj->pChaosBox[n]) == TRUE )
			{
				iStrengtenItemCount++;
				pItem = &lpObj->pChaosBox[n];
			}
		}
	}

	if ( iStrengtenItemCount != 1 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
	
	int iItemOption = this->GetItemStrengthenOption(pItem);
	int iItemOptionLevel = this->_GetItemOptionLevel(pItem);
	int JEWEL_OF_HARMONY_RETORE_NEEDZEN = this->_GetZenForRestoreItem(pItem);

	if ( JEWEL_OF_HARMONY_RETORE_NEEDZEN < 0 ) 
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	int iChaosTaxMoney = JEWEL_OF_HARMONY_RETORE_NEEDZEN * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
		iChaosTaxMoney = 0;

	JEWEL_OF_HARMONY_RETORE_NEEDZEN += iChaosTaxMoney;

	if ( JEWEL_OF_HARMONY_RETORE_NEEDZEN < 0 )
		JEWEL_OF_HARMONY_RETORE_NEEDZEN = 0;

	if ( lpObj->Money < JEWEL_OF_HARMONY_RETORE_NEEDZEN )
	{
		pMsg.Result = 2;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	lpObj->Money -= JEWEL_OF_HARMONY_RETORE_NEEDZEN;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	g_MixSystem.LogChaosItem(lpObj, "JewelOfHarmony][Restore Item"); //LogChaosItem(lpObj, "JewelOfHarmony][Restore Item");
	LogAddTD("[JewelOfHarmony][Restore Item] - Restore Start");

	LogAddTD("[JewelOfHarmony][Restore Item] Restore Strengthened Item [%s][%s] Name[%s] ItemType[%d] Serial[%u] OptionType[%d] OptionLevel [%d] Money %d-%d",
		lpObj->AccountID, lpObj->Name, pItem->GetName(), pItem->m_Type,
		pItem->m_Number, iItemOption, iItemOptionLevel, 
		lpObj->Money, JEWEL_OF_HARMONY_RETORE_NEEDZEN);

	pItem->m_JewelOfHarmonyOption = 0;
	GCUserChaosBoxSend(lpObj, 0);
	this->ShowStrengthenOption(pItem);
	lpObj->ChaosLock = FALSE;

	return TRUE;
}

//0063AED0	-> OK
BOOL CJewelOfHarmonySystem::NpcJewelOfHarmony(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( lpObj->m_IfState.use > 0 )
		return TRUE;

	PMSG_TALKRESULT pResult;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	
	if ( lpNpc->Class == 368 )
	{
		pResult.result = 17;
		pResult.level1 = this->m_iRatePuritySuccess;

		if ( this->m_bSystemPrutiyJewel != TRUE )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(13,50)), lpObj->m_Index, 1);
			return TRUE;
		}

		if ( !this->IsEnableToUsePuritySystem() )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(13,50)), lpObj->m_Index, 1);
			LogAddTD("[JewelOfHarmony][%s][%s] Not Purtiy Time ",
				lpObj->AccountID, lpObj->Name);

			return TRUE;
		}
	}
	else  if ( lpNpc->Class == 369 )
	{
		pResult.result = 18;
		pResult.level1 = this->m_iRateMixSmeltingStoneNor;
		pResult.level2 = this->m_iRateMixSmeltingStoneExt;

		if ( this->m_bSystemMixSmeltingStone != TRUE )
		{
			ChatTargetSend(lpNpc, lMsg.Get(MSGGET(13,53)), lpObj->m_Index);
			return TRUE;
		}
	}
	else if ( lpNpc->Class == 370 )
	{
		pResult.result = 19;

		if ( this->m_bSystemRestoreStrengthen != TRUE )
		{
			ChatTargetSend(lpNpc, lMsg.Get(MSGGET(13,54)), lpObj->m_Index);
			return TRUE;
		}
	}
	else
	{
		LogAddTD("[JewelOfHarmony][%s][%s] Open Chaos Box Failed NpcType [%d]",
			lpObj->AccountID, lpObj->Name, lpNpc->Class);

		return TRUE;
	}

	if ( bCanChaosBox == TRUE )
	{
		if ( lpObj->m_bPShopOpen == true )
		{
			LogAdd("[JewelOfHarmony][%s][%s] is Already Opening PShop, ChaosBox Failed",
				lpObj->AccountID, lpObj->Name);

			GCServerMsgStringSend(lMsg.Get(MSGGET(4,194)), lpObj->m_Index, 1);
			return TRUE;
		}

		lpObj->m_IfState.type = 7;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;
		lpObj->bIsChaosMixCompleted = false;

		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		gObjInventoryTrans(lpObj->m_Index);

		LogAddTD("[JewelOfHarmony][%s][%s] Open Chaos Box",
			lpObj->AccountID, lpObj->Name);

		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
		GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
	}

	return TRUE;
}

//0063B2D0	-> OK
BOOL CJewelOfHarmonySystem::IsEnableToTrade(OBJECTSTRUCT * lpObj) 
{
	BOOL bRet = TRUE;

	for ( int n=0;n<TRADE_BOX_SIZE;n++)
	{
		if ( lpObj->Trade[n].IsItem() == TRUE )
		{
			if ( this->IsStrengthenByJewelOfHarmony(&lpObj->Trade[n]) == TRUE )
			{
				bRet = FALSE;
			}
		}
	}

	return bRet;
}