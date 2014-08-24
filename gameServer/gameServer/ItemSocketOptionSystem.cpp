//-> Decompiled by DarkSim | 25.01.2013 | 1.01.00 GS-N (All names are original)
// -------------------------------------------------------------------------------

#include "StdAfx.h"
#include "user.h"
#include "..\\Include\\ReadScript.h"
#include "LogProc.h"
#include "GameMain.h"
#include "LargeRand.h"
#include "ItemSocketOptionSystem.h"
// -------------------------------------------------------------------------------

int g_SocketOptionSystem;					//-> 08aded50
int g_RequireMoneyForSeedExtract;			//-> 08adf31c
int g_RequireMoneyForSeedSphereComposite;	//-> 08adf320
int g_RequireMoneyForSeedSphereSet;			//-> 08adf324
int g_RequireMoneyForSeedSphereRemove;		//-> 08adf328
int g_ShopBuySocketItemSlotCount;			//-> 08adf32c
// -------------------------------------------------------------------------------

CItemSocketOptionSystem		gItemSocketOption;
// -------------------------------------------------------------------------------

//	-> 0%
CItemSocketOptionSystem::CItemSocketOptionSystem()
{

}
// -------------------------------------------------------------------------------

//	-> 0%
CItemSocketOptionSystem::~CItemSocketOptionSystem()
{

}
// -------------------------------------------------------------------------------

//005a50c0	-> 100%
void CItemSocketOptionSystem::Initialize()
{
	int iClearIndex;
	// ----
	for( iClearIndex = 0; iClearIndex < MAX_SEED_COUNT; iClearIndex++ )
	{
		this->m_SeedData[iClearIndex].Clear();
	}
	// ----
	for( iClearIndex = 0; iClearIndex < MAX_SOCKET_SLOT; iClearIndex++ )
	{
		this->m_SphereData[iClearIndex].Clear();
	}
	// ----
	for( iClearIndex = 0; iClearIndex < MAX_SEED_TYPE; iClearIndex++ )
	{
		this->m_BonusSocketOptionData[iClearIndex].Clear();
	}
	// ----
	this->m_MakeSocketSlotCountPool.InitPool();
	this->m_SeedExtractPool.InitPool();
	// ----
	for( iClearIndex = 0; iClearIndex < MAX_SOCKET_SLOT; iClearIndex++ )
	{
		this->m_SphereDropRate[iClearIndex]		= 0;
		this->m_SphereDropLevel[iClearIndex]	= 0;
	}
	// ----
#ifdef __CUSTOMS__
	for( iClearIndex = 0; iClearIndex < MAX_SOCKET_ITEM; iClearIndex++ )
	{
		this->m_SocketItemList[iClearIndex].ItemID		= -1;
		this->m_SocketItemList[iClearIndex].ItemIndex	= -1;
	}
	// ----
	this->m_SocketItemCount = 0;
#endif
}
// -------------------------------------------------------------------------------

//005a5200	-> 100%
bool CItemSocketOptionSystem::LoadOptionScript(char * pchFileName)
{
	this->Initialize();
	SMDFile = fopen(pchFileName, "r");	//ok
	// ----
	if( !SMDFile )
	{
		MsgBox("[SocketOption] SocketOption Script file Load failed. (%s)", pchFileName);
		return false;
	}
	// ----
	SMDToken Token;
	// ----
	int iCategoryIndex				= 0;
	int iSeedOptionDataCount		= 0;
	int iSphereOptionDataCount		= 0;
	int iBonusSocketOptionDataCount	= 0;
	int iSetSocketoptionDataCount	= 0;
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
		if( !strcmp("end", TokenString) )
		{
			break;
		}
		// ----
		if( Token == NUMBER )
		{
			iCategoryIndex = TokenNumber;
			// ----
			while(true)
			{
				if( iCategoryIndex == 0 )
				{
					BYTE btSeedIndex					= 0;
					BYTE btSeedType						= 0;
					BYTE btItemLevel					= 0;
					BYTE btOptionValueType				= 0;
					BYTE btOptionValue[MAX_SOCKET_SLOT] = { 0 };
					int iExtractRate					= 0;
					int iItemCode						= 0;
					// ----
					Token = GetToken();
					// ----
					if( !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					btSeedIndex			= TokenNumber;
					// ----
					Token = GetToken();		
					btSeedType			= TokenNumber;	
					// ----
					Token = GetToken();		
					btItemLevel			= TokenNumber;	
					// ----
					Token = GetToken();
					// ----
					Token = GetToken();		
					btOptionValueType	= TokenNumber;		
					// ----
					Token = GetToken();		
					btOptionValue[0]	= TokenNumber;	
					Token = GetToken();		
					btOptionValue[1]	= TokenNumber;	
					Token = GetToken();		
					btOptionValue[2]	= TokenNumber;		
					Token = GetToken();		
					btOptionValue[3]	= TokenNumber;	
					Token = GetToken();		
					btOptionValue[4]	= TokenNumber;
					// ----
					Token = GetToken();		
					iExtractRate		= TokenNumber;
					// ----
					this->m_SeedExtractPool.AddValue(btSeedIndex, iExtractRate);				
					this->m_SeedData[iSeedOptionDataCount].SetOption(btSeedIndex, btSeedType, btItemLevel, btOptionValueType, btOptionValue, iExtractRate);
					// ----
					iSeedOptionDataCount++;
					// ----
					if( iSeedOptionDataCount > MAX_SEED_COUNT )
					{
						MsgBox("[ERROR] MAX SeedSphere Option Data Overflow! [%d]", iSeedOptionDataCount);
						break;
					}
				}
				else if( iCategoryIndex == 1 )
				{
					Token = GetToken();
					// ----
					if( !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					BONUS_SOCKET_OPTION BonusOptionData;
					memset(&BonusOptionData, 0, sizeof(BonusOptionData));
					// ----
					BonusOptionData.OptionIndex			= TokenNumber;
					// ----
					Token = GetToken();		
					BonusOptionData.ItemTypeBegin		= TokenNumber;
					// ----
					Token = GetToken();		
					BonusOptionData.ItemTypeEnd			= TokenNumber;
					// ----
					Token = GetToken();
					// ----
					Token = GetToken();		
					BonusOptionData.OptionValueType		= TokenNumber;
					// ----
					Token = GetToken();		
					BonusOptionData.OptionValue			= TokenNumber;
					// ----
					Token = GetToken();		
					BonusOptionData.ExtractRate			= TokenNumber;
					// ----
					Token = GetToken();		
					BonusOptionData.SocketSlotType[0]	= TokenNumber;
					Token = GetToken();		
					BonusOptionData.SocketSlotType[1]	= TokenNumber;
					Token = GetToken();		
					BonusOptionData.SocketSlotType[2]	= TokenNumber;
					Token = GetToken();		
					BonusOptionData.SocketSlotType[3]	= TokenNumber;
					Token = GetToken();		
					BonusOptionData.SocketSlotType[4]	= TokenNumber;
					// ----
					for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
					{
						if( BonusOptionData.SocketSlotType[i] )
						{
							BonusOptionData.SocketSlotCount++;
						}
					}
					// ----
					switch(BonusOptionData.OptionIndex)
					{
					case 0:	
						BonusOptionData.EffectType = 0;	
						break;
					case 1:	
						BonusOptionData.EffectType = 18;	
						break;
					case 2:	
						BonusOptionData.EffectType = 0;	
						break;
					case 3:	
						BonusOptionData.EffectType = 18;	
						break;
					case 4:	
						BonusOptionData.EffectType = 11;	
						break;
					case 5:	
						BonusOptionData.EffectType = 22;	
						break;
					}
					// ----
					memcpy(&this->m_BonusSocketOptionData[iBonusSocketOptionDataCount], &BonusOptionData, sizeof(BonusOptionData));
					iBonusSocketOptionDataCount++;
				}
				else if( iCategoryIndex == 2 )
				{
					int iSetSocketOptionIndex		= 0;
					BYTE btSetSocketType			= 0;
					BYTE btSetSocketOptionValueType = 0;
					BYTE btSetSocketOptionValue		= 0;
					int iExtractRate				= 0;
					BYTE btSetSocketTypeIndex[MAX_SEED_TYPE] = { 0 };
					// ----
					Token = GetToken();
					// ----
					if( !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					iSetSocketOptionIndex		= TokenNumber;
					// ----
					Token = GetToken();		
					btSetSocketType				= TokenNumber;
					// ----
					Token = GetToken();
					// ----
					Token = GetToken();		
					btSetSocketOptionValueType	= TokenNumber;
					// ----
					Token = GetToken();		
					btSetSocketOptionValue		= TokenNumber;
					// ----
					Token = GetToken();		
					iExtractRate				= TokenNumber;
					// ----
					Token = GetToken();		
					btSetSocketTypeIndex[0]		= TokenNumber;
					Token = GetToken();		
					btSetSocketTypeIndex[1]		= TokenNumber;
					Token = GetToken();		
					btSetSocketTypeIndex[2]		= TokenNumber;
					Token = GetToken();		
					btSetSocketTypeIndex[3]		= TokenNumber;
					Token = GetToken();		
					btSetSocketTypeIndex[4]		= TokenNumber;
					Token = GetToken();		
					btSetSocketTypeIndex[5]		= TokenNumber;
					// ----
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].OptionIndex		= iSetSocketoptionDataCount;
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].OptionValueType	= btSetSocketOptionValueType;
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].OptionValue		= btSetSocketOptionValue;
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].ExtractRate		= iExtractRate;
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[0]	= btSetSocketTypeIndex[0];
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[1]	= btSetSocketTypeIndex[1];
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[2]	= btSetSocketTypeIndex[2];
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[3]	= btSetSocketTypeIndex[3];
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[4]	= btSetSocketTypeIndex[4];
					this->m_SetSocketOptionData[iSetSocketoptionDataCount].SeedTypeTable[5]	= btSetSocketTypeIndex[5];
					// ----
					switch(iSetSocketOptionIndex)
					{
					case 0:	
						this->m_SetSocketOptionData[iSetSocketoptionDataCount].EffectType = 44;	
						break;
					case 1:	
						this->m_SetSocketOptionData[iSetSocketoptionDataCount].EffectType = 45;	
						break;
					}
					// ----
					iSetSocketoptionDataCount++;
				}
#ifdef __CUSTOMS__
				else if( iCategoryIndex == 3 )
				{
					Token = GetToken();
					// ----
					if( !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					this->m_SocketItemList[this->m_SocketItemCount].ItemID		= TokenNumber;
					Token = GetToken();		
					this->m_SocketItemList[this->m_SocketItemCount].ItemIndex	= TokenNumber;
					this->m_SocketItemCount++;
				}
#endif
			}
		}
	}
	// ----
	fclose(SMDFile);	//wz mistake, fixed
	// ----
	this->LoadCommonServerScript(); 
	return true;
}
// -------------------------------------------------------------------------------

//005a5ec0	-> 100%
void CItemSocketOptionSystem::LoadCommonServerScript()
{ 
	//-> Many profiles use load from 0 to 4, maybe be cool use 'for'?
	int iNoneSlotItemRate = 10000;	//ebp-8
	// ----
	this->m_MakeSocketSlotCountPool.InitPool();
	this->m_TwoHandWeaponMakeSocketSlotCountPool.InitPool();
	// ----
	this->m_MakeSocketSlotRate[0] = GetPrivateProfileIntA("GameServerInfo", "SocketSlotCountRate1", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_MakeSocketSlotRate[1] = GetPrivateProfileIntA("GameServerInfo", "SocketSlotCountRate2", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_MakeSocketSlotRate[2] = GetPrivateProfileIntA("GameServerInfo", "SocketSlotCountRate3", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_MakeSocketSlotRate[3] = GetPrivateProfileIntA("GameServerInfo", "SocketSlotCountRate4", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_MakeSocketSlotRate[4] = GetPrivateProfileIntA("GameServerInfo", "SocketSlotCountRate5", 0, gDirPath.GetNewPath("commonserver.cfg"));
	// ----
	this->m_TwoHandWeaponMakeSocketSlotRate[0] = GetPrivateProfileIntA("GameServerInfo", "TwoHandWeaponSocketSlotCountRate1", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_TwoHandWeaponMakeSocketSlotRate[1] = GetPrivateProfileIntA("GameServerInfo", "TwoHandWeaponSocketSlotCountRate2", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_TwoHandWeaponMakeSocketSlotRate[2] = GetPrivateProfileIntA("GameServerInfo", "TwoHandWeaponSocketSlotCountRate3", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_TwoHandWeaponMakeSocketSlotRate[3] = GetPrivateProfileIntA("GameServerInfo", "TwoHandWeaponSocketSlotCountRate4", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_TwoHandWeaponMakeSocketSlotRate[4] = GetPrivateProfileIntA("GameServerInfo", "TwoHandWeaponSocketSlotCountRate5", 0, gDirPath.GetNewPath("commonserver.cfg"));
	// ----
	g_RequireMoneyForSeedExtract			= GetPrivateProfileIntA("GameServerInfo", "SeedExtractMoney", 1000000, gDirPath.GetNewPath("commonserver.cfg"));
	g_RequireMoneyForSeedSphereComposite	= GetPrivateProfileIntA("GameServerInfo", "SeedSphereCompositeMoney", 1000000, gDirPath.GetNewPath("commonserver.cfg"));
	g_RequireMoneyForSeedSphereSet			= GetPrivateProfileIntA("GameServerInfo", "SeedSphereSetMoney", 1000000, gDirPath.GetNewPath("commonserver.cfg"));
	g_RequireMoneyForSeedSphereRemove		= GetPrivateProfileIntA("GameServerInfo", "SeedSphereRemoveMoney", 1000000, gDirPath.GetNewPath("commonserver.cfg"));
	// ----
	this->m_MakeSocketSlotCountPool.AddValue(1, this->m_MakeSocketSlotRate[0]);
	this->m_MakeSocketSlotCountPool.AddValue(2, this->m_MakeSocketSlotRate[1]);
	this->m_MakeSocketSlotCountPool.AddValue(3, this->m_MakeSocketSlotRate[2]);
	this->m_MakeSocketSlotCountPool.AddValue(4, this->m_MakeSocketSlotRate[3]);
	this->m_MakeSocketSlotCountPool.AddValue(5, this->m_MakeSocketSlotRate[4]);
	// ----
	this->m_TwoHandWeaponMakeSocketSlotCountPool.AddValue(1, this->m_TwoHandWeaponMakeSocketSlotRate[0]);
	this->m_TwoHandWeaponMakeSocketSlotCountPool.AddValue(2, this->m_TwoHandWeaponMakeSocketSlotRate[1]);
	this->m_TwoHandWeaponMakeSocketSlotCountPool.AddValue(3, this->m_TwoHandWeaponMakeSocketSlotRate[2]);
	this->m_TwoHandWeaponMakeSocketSlotCountPool.AddValue(4, this->m_TwoHandWeaponMakeSocketSlotRate[3]);
	this->m_TwoHandWeaponMakeSocketSlotCountPool.AddValue(5, this->m_TwoHandWeaponMakeSocketSlotRate[4]);
	// ----
	g_ShopBuySocketItemSlotCount	= GetPrivateProfileIntA("GameServerInfo", "ShopBuySocketItemSlotCount", 3, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_SphereDropOn			= GetPrivateProfileIntA("GameServerInfo", "SphereDropOn", 0, gDirPath.GetNewPath("commonserver.cfg"));
	// ----
	this->m_SphereDropRate[0] = GetPrivateProfileIntA("GameServerInfo", "SphereDropRate_Lv1", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_SphereDropRate[1] = GetPrivateProfileIntA("GameServerInfo", "SphereDropRate_Lv2", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_SphereDropRate[2] = GetPrivateProfileIntA("GameServerInfo", "SphereDropRate_Lv3", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_SphereDropRate[3] = GetPrivateProfileIntA("GameServerInfo", "SphereDropRate_Lv4", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_SphereDropRate[4] = GetPrivateProfileIntA("GameServerInfo", "SphereDropRate_Lv5", 0, gDirPath.GetNewPath("commonserver.cfg"));
	// ----
	this->m_SphereDropLevel[0] = GetPrivateProfileIntA("GameServerInfo", "SphereDropLevel_Lv1", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_SphereDropLevel[1] = GetPrivateProfileIntA("GameServerInfo", "SphereDropLevel_Lv2", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_SphereDropLevel[2] = GetPrivateProfileIntA("GameServerInfo", "SphereDropLevel_Lv3", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_SphereDropLevel[3] = GetPrivateProfileIntA("GameServerInfo", "SphereDropLevel_Lv4", 0, gDirPath.GetNewPath("commonserver.cfg"));
	this->m_SphereDropLevel[4] = GetPrivateProfileIntA("GameServerInfo", "SphereDropLevel_Lv5", 0, gDirPath.GetNewPath("commonserver.cfg"));
}
// -------------------------------------------------------------------------------

//005a65c0	-> 100%
bool CItemSocketOptionSystem::IsEnableSocketItem(CItem * lpItemData)
{ 
	if( !lpItemData || !lpItemData->IsItem() )
	{
		return false;
	}
	// ----
	return this->IsEnableSocketItem(lpItemData->m_Type);
}
// -------------------------------------------------------------------------------

//005a6610	-> 0% (Not originaly)
bool CItemSocketOptionSystem::IsEnableSocketItem(int ItemCode)
{
#ifdef __CUSTOMS__
	for( int i = 0; i < this->m_SocketItemCount; i++ )
	{
		if( ItemCode == ITEMGET(this->m_SocketItemList[i].ItemID, this->m_SocketItemList[i].ItemIndex) )
		{
			return true;
		}
		else if( this->m_SocketItemList[i].ItemID == -1 && this->m_SocketItemList[i].ItemIndex != -1 )
		{
			for( int Category = 7; Category < 12; Category++ )
			{
				if( ItemCode == ITEMGET(Category, this->m_SocketItemList[i].ItemIndex) )
				{
					return true;
				}
			}
		}
	}
	// ----
	return false;
#else
	if( ItemCode >= ITEMGET(0, 26) && ItemCode <= ITEMGET(0, 28) )
	{
		return true;
	}
	else if( ItemCode == ITEMGET(2, 16) || ItemCode == ITEMGET(2, 17) )
	{
		return true;
	}
	else if( ItemCode == ITEMGET(4, 23) )
	{
		return true;
	}
	else if( ItemCode >= ITEMGET(5, 30) && ItemCode <= ITEMGET(5, 32) 
		|| ItemCode == ITEMGET(5, 20) )
	{
		return true;
	}
	else if( ItemCode >= ITEMGET(6, 17) && ItemCode <= ITEMGET(6, 20) )
	{
		return true;
	}
	else
	{
		for( int Category = 7; Category < 12; Category++ )
		{
			if( ItemCode >= ITEMGET(Category, 44) && ItemCode <= ITEMGET(Category, 53) )
			{
				return true;
			}
		}
	}
	// ----
	return false;
#endif
}
// -------------------------------------------------------------------------------

//005a6780	-> 100%
BYTE CItemSocketOptionSystem::GetEmptySlotCount(CItem * lpItemData)
{ 
	BYTE btEmptySlotCount = 0;
	// ----
	for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
	{
		if( lpItemData->m_SocketOption[i] == 0xFE )
		{
			btEmptySlotCount++;
		}
	}
	// ----
	return btEmptySlotCount;
}
// -------------------------------------------------------------------------------

//005a67f0	-> 100%
BYTE CItemSocketOptionSystem::GetMaxSlotCount(int iItemCode)
{ 
	if( ItemAttribute[iItemCode].HaveItemInfo != 1 )
	{
		return false;
	}
	// ----
	if( ItemAttribute[iItemCode].TwoHand )
	{
		for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
		{
			if( !this->m_TwoHandWeaponMakeSocketSlotRate[i] )	//+327
			{
				return i;
			}
		}
	}
	else
	{
		for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
		{
			if( !this->m_MakeSocketSlotRate[i] )	//+322
			{
				return i;
			}
		}
	}
	// ----
	return MAX_SOCKET_SLOT;
}
// -------------------------------------------------------------------------------

//005a68c0	-> 100%
bool CItemSocketOptionSystem::GetSeedSphereEffect(BYTE btSeedSphereIndex, BYTE & pbSeedType, BYTE & pbEffectValueType, BYTE & pbEffectValue)
{ 
	BYTE SeedIndex				= 0;
	BYTE SphereLevel			= 0;
	CSeedItem * lpSeedData		= 0;
	CSphereItem * lpSphereData	= 0;
	CSeedSphereItem SeedSphere;
	// ----
	SeedIndex		= btSeedSphereIndex % 50;
	SphereLevel		= (btSeedSphereIndex - SeedIndex) / 50 + 1;
	// ----
	lpSeedData		= this->GetSeedData(SeedIndex);
	lpSphereData	= this->GetSphereData(SphereLevel);
	// ----
	if( !lpSeedData )
	{
		return false;
	}
	// ----
	SeedSphere.SetOption(lpSeedData, SphereLevel);
	// ----
    pbSeedType			= SeedSphere.m_SeedIndex;
    pbEffectValueType	= SeedSphere.m_SeedOptionValueType;
    pbEffectValue		= LOBYTE(SeedSphere.m_SeedOptionValue);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005a6a10	-> 100%
void CItemSocketOptionSystem::GetSocketOption(CItem * lpItemData, LPBYTE pbSocketOption, BYTE & pbSocketBonusOption)
{ 
	if( !lpItemData || !lpItemData->IsItem() || !this->IsEnableSocketItem(lpItemData) )
	{
		return;
	}
	// ----
	for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
	{
		pbSocketOption[i] = lpItemData->m_SocketOption[i]; //+188
	}
	// ----
	pbSocketBonusOption = lpItemData->m_BonusSocketOption; //+187

}
// -------------------------------------------------------------------------------

//005a6ab0	-> 100%
bool CItemSocketOptionSystem::IsSocketItem(CItem * lpItemData)
{ 
	if( !lpItemData )
	{
		return false;
	}
	// ----
	if( !lpItemData->IsItem() )
	{
		return false;
	}
	// ----
	if( !this->IsEnableSocketItem(lpItemData) )
	{
		return false;
	}
	// ----
	for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
	{
		if( lpItemData->m_SocketOption[i] != 0xFF )
		{
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//005a6b50	-> 100%
bool CItemSocketOptionSystem::IsSeedItem(int ItemCode)
{ 
	return ItemCode >= ITEMGET(12, 60) && ItemCode <= ITEMGET(12, 65);
}
// -------------------------------------------------------------------------------

//005a6ba0	-> 100%
bool CItemSocketOptionSystem::IsSphereItem(int ItemCode)
{ 
	return ItemCode >= ITEMGET(12, 70) && ItemCode <= ITEMGET(12, 74);
}
// -------------------------------------------------------------------------------

//005a6bf0	-> 100%
bool CItemSocketOptionSystem::IsSeedSphereItem(int ItemCode)
{ 
	BYTE btItemType		= 0;	//ebp-8
	BYTE btItemIndex	= 0;	//ebp-0c
	// ----
	btItemIndex = ItemCode % 512;
	btItemType	= (ItemCode - btItemIndex) / 512 ;
	// ----
	if( btItemType == 12 )
	{
		if( btItemIndex >= 100 && btItemIndex <= 129 )
		{
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//005a6c90	-> 100%
BYTE CItemSocketOptionSystem::GetSeedIndex(int ItemCode, BYTE ItemLevel)
{ 
	for( int i = 0; i < MAX_SEED_COUNT; i++ )
	{
		if( this->m_SeedData[i].m_ItemCode == ItemCode && this->m_SeedData[i].m_ItemLevel == ItemLevel )
		{
			return this->m_SeedData[i].m_SeedIndex;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

//005a6d20	-> 100%
BYTE CItemSocketOptionSystem::GetSphereLevel(int ItemCode)
{
	switch(ItemCode)
	{
	case ITEMGET(12, 70):
		{
			return 1;
		}
		break;
		// --
	case ITEMGET(12, 71):
		{
			return 2;
		}
		break;
		// --
	case ITEMGET(12, 72):
		{
			return 3;
		}
		break;
		// --
	case ITEMGET(12, 73):
		{
			return 4;
		}
		break;
		// --
	case ITEMGET(12, 74):
		{
			return 5;
		}
		break;
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

//005a6da0	-> 100%
bool CItemSocketOptionSystem::CheckItemForSameSeedType(CItem * lpItemData, WORD SeedSphereItemCode, BYTE ItemLevel)
{ 
	BYTE btSeedIndex		= 0;	//ebp-8
	BYTE btSeedSphereIndex	= 0;	//ebp-0c
	// ----
	if( !this->IsEnableSocketItem(lpItemData) )
	{
		return false;
	}
	// ----
	btSeedSphereIndex	= this->GetSeedSphereIndexForItemCode(SeedSphereItemCode, ItemLevel);
	btSeedIndex			= btSeedSphereIndex % 50;
	// ----
	for (int i = 0; i < MAX_SOCKET_SLOT; i++ )
	{
		if( lpItemData->m_SocketOption[i] == 0xFF || lpItemData->m_SocketOption[i] == 0xFE )
		{
			continue;
		}
		// ----
		if(	lpItemData->m_SocketOption[i] % 50 == btSeedIndex )
		{
			return false;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005a6eb0	-> 100%
BYTE CItemSocketOptionSystem::GetSeedSphereIndexForItemCode(WORD ItemCode, BYTE ItemLevel)
{ 
	BYTE btSphereLevel		= 0;
	BYTE btSeedType			= 0;
	BYTE btSeedSphereIndex	= 0;
	// ----
	btSphereLevel	= (ItemCode % 512 - 100) / MAX_SEED_TYPE;
	btSeedType		= (ItemCode % 512 - 100) % MAX_SEED_TYPE + 1;
	// ----
	for( int i = 0; i < MAX_SEED_COUNT; i++ )
	{
		if(	this->m_SeedData[i].m_SeedType == btSeedType && this->m_SeedData[i].m_ItemLevel == ItemLevel )
		{
			btSeedSphereIndex = btSphereLevel * 50 + this->m_SeedData[i].m_SeedIndex;
			return btSeedSphereIndex;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

//005a6fd0	-> 100%
long CItemSocketOptionSystem::GetSeedSphereValue(BYTE btSeedSphereIndex)
{ 
	BYTE btSeedIndex			= 0;
	BYTE btSphereLevel			= 0;
	CSeedItem * lpSeedData		= 0;
	CSphereItem * lpSphereData	= 0;
	// ----
	CSeedSphereItem SeedSphere;
	// ----
	long dwSeedSphereValue		= 0;
	// ----
	btSeedIndex		= btSeedSphereIndex % 50;
	btSphereLevel	= (btSeedSphereIndex - btSeedSphereIndex % 50) / 50 + 1;
	lpSeedData		= this->GetSeedData(btSeedSphereIndex % 50);
	// ----
	if( !lpSeedData )
	{
		return 0;
	}
	// ----
	SeedSphere.SetOption(lpSeedData, btSphereLevel);
	dwSeedSphereValue = ItemAttribute[SeedSphere.GetItemCode()].BuyMoney;
	// ----
	return dwSeedSphereValue;
}
// -------------------------------------------------------------------------------

//005a7110	-> 100%
bool CItemSocketOptionSystem::CheckSetSeedSphereItemType(CItem * lpItemData, BYTE btSeedSphereIndex)
{ 
	if( !this->IsEnableSocketItem(lpItemData) )
	{
		return false;
	}
	// ----
	BYTE btItemType			= 0;	//ebp-8
	BYTE btItemIndex		= 0;	//ebp-0c
	BYTE btSeedSphereType 	= 0;	//ebp-10
	// ----
	btItemIndex			= lpItemData->m_Type % 512;
	btItemType			= (lpItemData->m_Type - btItemIndex) / 512;
	btSeedSphereType	= this->GetSocketType(btSeedSphereIndex);
	// ----
	if( btItemType >= 0 && btItemType <= 5 )
	{
		if( btSeedSphereType == 1 || btSeedSphereType == 3 || btSeedSphereType == 5 )
		{
			return true;
		}
	}
	else if( btItemType >= 6 && btItemType <= 11 )
	{
		if( btSeedSphereType == 2 || btSeedSphereType == 4 || btSeedSphereType == 6 )
		{
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//005a7260	-> 100%
bool CItemSocketOptionSystem::IsEnableDropSocketItemMap(int iMapIndex)
{ 
	return iMapIndex >= 57 && iMapIndex <= 58;	//-> Raklion, Raklion Boss
}
// -------------------------------------------------------------------------------

//005a72a0	-> 100%
int	CItemSocketOptionSystem::GetSphereDropInfo(int iMonsterLevel)
{
	int iItemCode		= -1;	//ebp-8
	int iSphereLevel	= 0;	//ebp-0c
	// ----
	if( this->m_SphereDropOn != 1 )
	{
		return -1;
	}
	// ----
	if( this->m_SphereDropLevel[4] && this->m_SphereDropLevel[4] <= iMonsterLevel )
	{
		iSphereLevel = 5;
	}
	else if( this->m_SphereDropLevel[3] && this->m_SphereDropLevel[3] <= iMonsterLevel )
	{
		iSphereLevel = 4;
	}
	else if( this->m_SphereDropLevel[2] && this->m_SphereDropLevel[2] <= iMonsterLevel )
	{
		iSphereLevel = 3;
	}
	else if( this->m_SphereDropLevel[1] && this->m_SphereDropLevel[1] <= iMonsterLevel )
	{
		iSphereLevel = 2;
	}
	else if( !this->m_SphereDropLevel[0] || this->m_SphereDropLevel[0] > iMonsterLevel )
	{
		iSphereLevel = 1;
	}
	else
	{
		return -1;
	}
	// ----
	for( int i = 0; i < iSphereLevel; i++ )
	{
		if( this->m_SphereDropRate[i] >= GetLargeRand() % MAX_SPHERE_DROP )
		{
			iItemCode = i + ITEMGET(12, 70);
		}
	}
	// ----
	return iItemCode;
}
// -------------------------------------------------------------------------------

//005a7430	-> 100%
BYTE CItemSocketOptionSystem::GetSeedSphereIndex(BYTE btSeedIndex, BYTE btSphereLevel)
{ 
	return btSeedIndex + 50 * btSphereLevel;
}
// -------------------------------------------------------------------------------

//005a7470	-> 100%
bool CItemSocketOptionSystem::MakeSeedSphere(CSeedSphereItem * lpSeedSphere, BYTE btSeedIndex, BYTE btSphereLevel)
{ 
	CSeedItem * lpSeedData		= 0;	//ebp-8
	CSphereItem * lpSphereData	= 0;	//ebp-0c
	// ----
	lpSeedData = this->GetSeedData(btSeedIndex);
	// ----
	if( !lpSeedData )
	{
		return false;
	}
	// ----
	if( !lpSeedSphere->SetOption(lpSeedData, btSphereLevel) )
	{
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005a74f0	-> 100%
CSeedItem * CItemSocketOptionSystem::GetSeedData(BYTE btSeedIndex)
{ 
	for( int i = 0; i < MAX_SEED_COUNT; i++ )
	{
		if( this->m_SeedData[i].m_SeedIndex == btSeedIndex )
		{
			return &this->m_SeedData[i];
		}
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

//005a7570	-> 100%
CSphereItem * CItemSocketOptionSystem::GetSphereData(BYTE btSphereLevel)
{ 
	for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
	{
		if( this->m_SphereData[i].m_SphereLevel == btSphereLevel )
		{
			return &this->m_SphereData[i];
		}
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

//005a75f0	-> 100%
BYTE CItemSocketOptionSystem::GetExtractSeed()
{ 
	return this->m_SeedExtractPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
}
// -------------------------------------------------------------------------------

//005a7630	-> 100%
BYTE CItemSocketOptionSystem::GetSocketType(BYTE btSeedSphereIndex)
{ 
	BYTE btSeedIndex		= 0;	//ebp-8
	CSeedItem * lpSeedData	= 0;	//ebp-0c
	// ----
	btSeedIndex = btSeedSphereIndex % 50;
	lpSeedData	= this->GetSeedData(btSeedIndex);
	// ----
	if( !lpSeedData )
	{
		return 0;
	}
	// ----
	return lpSeedData->m_SeedType;
}
// -------------------------------------------------------------------------------

//005a76a0	-> 100%
BONUS_SOCKET_OPTION * CItemSocketOptionSystem::GetBonusSocketOption(BYTE btBonusSocketOptionIndex)
{ 
	//__ehhandler
	for( int i = 0; i < MAX_SEED_TYPE; i++ )
	{
		if( this->m_BonusSocketOptionData[i].OptionIndex == btBonusSocketOptionIndex )
		{
			return &this->m_BonusSocketOptionData[i];
		}
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

//005a7720	-> 100%
int	CItemSocketOptionSystem::ApplySeedSphereEffect(LPOBJ lpObj)
{ 
	int iTotalSeedSphereCount	= 0;	//ebp-14
	BYTE SeedSphereIndex		= 0;	//ebp-18
	BYTE SeedIndex				= 0;	//ebp-1c
	BYTE SphereLevel			= 0;	//ebp-20
	// ----
	CSeedSphereItem SeedSphere;			//ebp-30??
	// ----
	for( int iEquip = 0; iEquip < 12; iEquip++ )	//Inventory
	{
		if( !this->IsEnableSocketItem(&lpObj->pInventory[iEquip]) )
		{
			continue;
		}
		// ----
		for( int iSocketSlot = 0; iSocketSlot < MAX_SOCKET_SLOT; iSocketSlot++ )
		{
			SeedSphereIndex = lpObj->pInventory[iEquip].m_SocketOption[iSocketSlot];
			// ----
			if( SeedSphereIndex == 0xFF || SeedSphereIndex == 0xFE )
			{
				continue;
			}
			// ----
			SeedIndex	= SeedSphereIndex % 50;
			SphereLevel = (SeedSphereIndex - SeedIndex) / 50 + 1;
			// ----
			if( !this->MakeSeedSphere(&SeedSphere, SeedIndex, SphereLevel) )
			{
				continue;
			}
			// ----
			this->SetEffect(lpObj, 1, SeedIndex, SeedSphere.m_SeedOptionValueType, SeedSphere.m_SeedOptionValue);
			iTotalSeedSphereCount++;

		}
	}
	// ----
	return iTotalSeedSphereCount;
}
// -------------------------------------------------------------------------------

//005a7920	-> 100%
void CItemSocketOptionSystem::ApplyBonusOptionEffect(LPOBJ lpObj)
{ 
	BYTE btBonusSocketIndex					= 0;	//ebp-8
	BONUS_SOCKET_OPTION * lpBonusOptionData = 0;	//ebp-0c	
	// ----
	for( int iEquip = 0; iEquip < INVETORY_WEAR_SIZE; iEquip++ )	//ebp-10
	{
		if( !this->IsEnableSocketItem(&lpObj->pInventory[iEquip]) )
		{
			continue;
		}
		// ----
		btBonusSocketIndex = lpObj->pInventory[iEquip].m_BonusSocketOption;
		// ----
		if( btBonusSocketIndex == 0xFF || btBonusSocketIndex == 0xFE )
		{
			continue;
		}
		// ----
		lpBonusOptionData = this->GetBonusSocketOption(btBonusSocketIndex);
		// ----
		if( !lpBonusOptionData )
		{
			continue;
		}
		// ----
		LogAddTD("[SocketItem] [%s][%s] Set Bonus Socket Option - OptionIndex : %d, EffectType : %d, OptionType : %d, OptionValue : %d",
			lpObj->AccountID,
			lpObj->Name,
			lpBonusOptionData->OptionIndex,
			lpBonusOptionData->EffectType,
			lpBonusOptionData->OptionValueType,
			lpBonusOptionData->OptionValue);
		// ----
		this->SetEffect(lpObj, 2, lpBonusOptionData->EffectType, lpBonusOptionData->OptionValueType, lpBonusOptionData->OptionValue);
	}
}
// -------------------------------------------------------------------------------

//005a7aa0	-> 100%
void CItemSocketOptionSystem::ApplySetOptionEffect(LPOBJ lpObj)
{
	BYTE btTotalSeedTypeList[8];		//ebp-0c
	// ----
	BYTE btSeedType				= 0;	//ebp-10
	BYTE btMatchedCount			= 0;	//ebp-14
	// ----
	memset(&btTotalSeedTypeList, 0, 8);
	// -----
	for( int iUserSocketIndex = 0; iUserSocketIndex < 35; iUserSocketIndex++ )	//ebp-18
	{
		btSeedType = this->GetSocketType(lpObj->m_SeedOptionList[iUserSocketIndex].m_SocketOptionIndex);
		// ----
		if( !btSeedType )
		{
			break;
		}
		// ----
		btTotalSeedTypeList[btSeedType - 1]	+= lpObj->m_SeedOptionList[iUserSocketIndex].m_SocketOptionCount;
	}
	// ----
	for( int iSetOptionIndex = 0; iSetOptionIndex < 2; iSetOptionIndex++ )
	{
		for( int i = 0; i < MAX_SEED_TYPE; i++ )
		{
			if( this->m_SetSocketOptionData[iSetOptionIndex].SeedTypeTable[i] <= btTotalSeedTypeList[i] )
			{
				btMatchedCount++;
			}
		}
		// ----
		if( btMatchedCount == MAX_SEED_TYPE )
		{
			this->SetEffect(lpObj, 3,
			this->m_SetSocketOptionData[iSetOptionIndex].EffectType, 
			this->m_SetSocketOptionData[iSetOptionIndex].OptionValueType,
			this->m_SetSocketOptionData[iSetOptionIndex].OptionValue);
		}
		// ----
		btMatchedCount = 0;
	}
}
// -------------------------------------------------------------------------------

//005a7c50	-> 100%
bool CItemSocketOptionSystem::SetApplySocketEffect(LPOBJ lpObj)
{ 
	int iTotalSeedSphereCount = 0;	//ebp-8
	// ----
	if( !lpObj )
	{
		return false;
	}
	// ----
	if( lpObj->Type != 1 || lpObj->Connected < 3 )
	{
		return false;
	}
	// ----
	this->SetClearSocketEffect(lpObj);
	iTotalSeedSphereCount = this->ApplySeedSphereEffect(lpObj);
	// ----
	if( iTotalSeedSphereCount < 0 )
	{
		return false;
	}
	// ----
	this->ApplyBonusOptionEffect(lpObj);
    this->ApplySetOptionEffect(lpObj);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005a7cf0	-> 100%
void CItemSocketOptionSystem::SetClearSocketEffect(LPOBJ lpObj)
{ 
	if( !lpObj )
	{
		return;
	}
	// ----
	if( lpObj->Type != 1 || lpObj->Connected < 3 )
	{
		return;
	}
	// ----
	int iIndex = 0;	//ebp-8
	// ----
	for( iIndex = 0; iIndex < 35 && lpObj->m_SeedOptionList[iIndex].m_SocketOptionIndex != 0xFF; iIndex++ )
	{
		lpObj->m_SeedOptionList[iIndex].Clear();
	}
	// ----
	for( iIndex = 0; iIndex < 7 && lpObj->m_BonusOptionList[iIndex].m_SocketOptionIndex != 0xFF; iIndex++ )
	{
		lpObj->m_BonusOptionList[iIndex].Clear();
	}
	// ----
	for( iIndex = 0; iIndex < 2 && lpObj->m_SetOptionList[iIndex].m_SocketOptionIndex != 0xFF; iIndex++ )
	{
		lpObj->m_SetOptionList[iIndex].Clear();
	}
	// ----
    lpObj->m_btRefillHPSocketOption			= 0;
    lpObj->m_btRefillMPSocketOption			= 0;
    lpObj->m_wSocketOptionMonsterDieGetHp	= 0;
    lpObj->m_wSocketOptionMonsterDieGetMana = 0;
    lpObj->m_btAGReduceRate					= 0;
}
// -------------------------------------------------------------------------------

//005a7eb0	- 100%
void CItemSocketOptionSystem::SetEffect(LPOBJ lpObj, BYTE btOptionType, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{ 
	if( btEffectType >= 0 && btEffectType <= 9 )
	{
		this->_SetFireEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if( btEffectType >= 10 && btEffectType <= 15 )
	{
		this->_SetWaterEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if( btEffectType >= 16 && btEffectType <= 20 )
	{
		this->_SetIceEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if( btEffectType >= 21 && btEffectType <= 28 )
	{
		this->_SetWindEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if( btEffectType >= 29 && btEffectType <= 33 )
	{
		this->_SetThunderEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if( btEffectType >= 34 && btEffectType <= 40 )
	{
		this->_SetEarthEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if( btEffectType >= 41 && btEffectType <= 43 )
	{
		this->_SetUniqueEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	else if( btEffectType >= 44 && btEffectType <= 45 )
	{
		this->_SetSetEffect(lpObj, btEffectType, btEffectValueType, wEffectValue);
	}
	// ----
	for( int i = 0; i < 43; i++ )
	{
		if( lpObj->m_SeedOptionList[i].m_SocketOptionIndex == btEffectType && lpObj->m_SeedOptionList[i].m_SocketOptionValueType == btEffectValueType )
		{
			lpObj->m_SeedOptionList[i].m_SocketOptionValue += wEffectValue;
			lpObj->m_SeedOptionList[i].m_SocketOptionCount++;
			return;
		}
		// ----
		if( lpObj->m_SeedOptionList[i].m_SocketOptionIndex == 0xFF )
		{
			lpObj->m_SeedOptionList[i].m_SocketOptionIndex		= btEffectType;
			lpObj->m_SeedOptionList[i].m_SocketOptionValueType	= btEffectValueType;
			lpObj->m_SeedOptionList[i].m_SocketOptionValue		= wEffectValue;
			lpObj->m_SeedOptionList[i].m_SocketOptionCount		= 1;
			return;
		}
	}
}
// -------------------------------------------------------------------------------

//005a8250	-> 100% (Not writed by WZ)
void CItemSocketOptionSystem::ClearEffect(LPOBJ lpObj, SOCKET_OPTION_LIST * lpOptionData)
{ 
	// ---
}
// -------------------------------------------------------------------------------

//005a8270	-> 100%
void CItemSocketOptionSystem::_SetFireEffect(LPOBJ lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iCalcEffectValue = 0;	//ebp-8
	// ----
	switch(btEffectType)	//ebp-0c
	{
	case 0:
		{
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMaxLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxLeft	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMinLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinLeft	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMaxRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxRight	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMinRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinRight	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMin			+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMax			+= iCalcEffectValue;
		}
		break;
		// --
	case 1:
		{
			iCalcEffectValue		= this->CalcEffectValue(lpObj->m_AttackSpeed, wEffectValue, btEffectValueType);
			lpObj->m_AttackSpeed	+= iCalcEffectValue;
			// ----
			iCalcEffectValue		= this->CalcEffectValue(lpObj->m_MagicSpeed, wEffectValue, btEffectValueType);
			lpObj->m_MagicSpeed		+= iCalcEffectValue;
		}
		break;
		// --
	case 2:
		{
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMaxLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxLeft	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMaxRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxRight	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMax			+= iCalcEffectValue;
		}
		break;
		// --
	case 3:
		{
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMinLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinLeft	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMinRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinRight	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMin			+= iCalcEffectValue;
		}
		break;
		// --
	case 4:
		{
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMaxLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxLeft	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMinLeft, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinLeft	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMaxRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMaxRight	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_AttackDamageMinRight, wEffectValue, btEffectValueType);
			lpObj->m_AttackDamageMinRight	+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_MagicDamageMin, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMin			+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_MagicDamageMax, wEffectValue, btEffectValueType);
			lpObj->m_MagicDamageMax			+= iCalcEffectValue;
		}
		break;
		// --
	case 5:
		{
			lpObj->m_btAGReduceRate += wEffectValue;
			// ----
			if( lpObj->m_btAGReduceRate > 100 )
			{
				lpObj->m_btAGReduceRate = 100;
			}
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//005a8830	-> 100%
void CItemSocketOptionSystem::_SetWaterEffect(LPOBJ lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{ 
	int iCalcEffectValue = 0;	//ebp-8
	// ----
	switch(btEffectType)	//ebp-0c
	{
	case 10:
		{
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_SuccessfulBlocking, wEffectValue, btEffectValueType);
			lpObj->m_SuccessfulBlocking		+= iCalcEffectValue;
		}
		break;
		// --
	case 11:
		{
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_Defense, wEffectValue, btEffectValueType);
			lpObj->m_Defense				+= iCalcEffectValue;
			// ----
			iCalcEffectValue				= this->CalcEffectValue(lpObj->m_MagicDefense, wEffectValue, btEffectValueType);
			lpObj->m_MagicDefense			+= iCalcEffectValue;
		}
		break;
		// --
	case 12:
		{
			lpObj->SetOpImproveSheldDefence += wEffectValue;
		}
		break;
		// --
	case 13:
		{
			lpObj->DamageMinus				+= wEffectValue;
		}
		break;
		// --
	case 14:
		{
			lpObj->DamageReflect			+= wEffectValue;
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//005a89c0	-> 100%
void CItemSocketOptionSystem::_SetIceEffect(LPOBJ lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{ 
	int iAddValue = 0;	//ebp-8
	// ----
	switch(btEffectType)	//ebp-0c
	{
	case 16:
		{
			iAddValue								= this->CalcEffectValue(lpObj->AddLife + lpObj->MaxLife, wEffectValue, btEffectValueType);
			lpObj->m_wSocketOptionMonsterDieGetHp	+= iAddValue;
		}
		break;
		// --
	case 17:
		{
			iAddValue								= this->CalcEffectValue(lpObj->AddMana + lpObj->MaxMana, wEffectValue, btEffectValueType);
			lpObj->m_wSocketOptionMonsterDieGetMana	+= iAddValue;
		}
		break;
		// --
	case 18:
		{
			iAddValue								= this->CalcEffectValue(lpObj->SetOpAddSkillAttack, wEffectValue, btEffectValueType);
			lpObj->SetOpAddSkillAttack				+= iAddValue;
		}
		break;
		// --
	case 19:
		{
			lpObj->m_AttackRating					+= wEffectValue;
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//005a8b60	-> 100%
void CItemSocketOptionSystem::_SetWindEffect(LPOBJ lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	int iAddValue = 0;	//ebp-8
	// ----
	switch(btEffectType)	//ebp-0c
	{
	case 21:
		{
			iAddValue						= this->CalcEffectValue(lpObj->AddLife + lpObj->MaxLife, wEffectValue, btEffectValueType);
			lpObj->m_btRefillHPSocketOption	+= iAddValue;
		}
		break;
		// --
	case 22:
		{
			iAddValue						= this->CalcEffectValue(lpObj->AddLife + lpObj->MaxLife, wEffectValue, btEffectValueType);
			lpObj->AddLife					+= iAddValue;
			// ----
			GCReFillSend(lpObj->m_Index, lpObj->AddLife + lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield + lpObj->iMaxShield);
			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		}
		break;
		// --
	case 23:
		{
			iAddValue						= this->CalcEffectValue(lpObj->AddMana + lpObj->MaxMana, wEffectValue, btEffectValueType);
			lpObj->AddMana					+= iAddValue;
			// ----
			GCManaSend(lpObj->m_Index, lpObj->AddMana + lpObj->MaxMana, 0xFE, 0, lpObj->AddBP + lpObj->MaxBP);
			GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
		}
		break;
		// --
	case 24:
		{
			iAddValue						= this->CalcEffectValue(lpObj->AddMana + lpObj->MaxMana, wEffectValue, btEffectValueType);
			lpObj->m_btRefillMPSocketOption += iAddValue;
		}
		break;
		// --
	case 25:
		{
			iAddValue						= this->CalcEffectValue(lpObj->AddBP + lpObj->MaxBP, wEffectValue, btEffectValueType);
			lpObj->AddBP					+= iAddValue;
			// ----
			GCManaSend(lpObj->m_Index, lpObj->AddMana + lpObj->MaxMana, 0xFE, 0, lpObj->AddBP + lpObj->MaxBP);
			GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
		}
		break;
		// --
	case 26:
		{
			iAddValue						= this->CalcEffectValue(lpObj->SetOpIncAGValue, wEffectValue, btEffectValueType);
			lpObj->SetOpIncAGValue			+= iAddValue;
		}
		break;
		// --
	case 27:
		{
			lpObj->MonsterDieGetMoney		+= wEffectValue;
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//005a8f90	-> 100%
void CItemSocketOptionSystem::_SetThunderEffect(LPOBJ lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{ 
	int iAddValue = 0;	//ebp-8
	// ----
	switch(btEffectType)
	{
	case 29:
		{
			iAddValue						= this->CalcEffectValue(lpObj->SetOpAddExDamage, wEffectValue, btEffectValueType);
			lpObj->SetOpAddExDamage			+= iAddValue;
		}
		break;
		// --
	case 30:
		{
			lpObj->m_ExcelentDamage			+= wEffectValue;
		}
		break;
		// --
	case 31:
		{
			iAddValue						= this->CalcEffectValue(lpObj->SetOpAddCriticalDamage, wEffectValue, btEffectValueType);
			lpObj->SetOpAddCriticalDamage	+= iAddValue;
		}
		break;
		// --
	case 32:
		{
			lpObj->m_CriticalDamage			+= wEffectValue;
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//005a90e0	-> 100%
void CItemSocketOptionSystem::_SetEarthEffect(LPOBJ lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{
	//WZ forgot about command & DL xD
	int iAddValue = 0;
	// ----
	switch(btEffectType)
	{
	case 34:
		{
			iAddValue			= this->CalcEffectValue(lpObj->AddStrength + lpObj->Strength, wEffectValue, btEffectValueType);
			lpObj->AddStrength	+= iAddValue;
		}
		break;
		// --
	case 35:
		{
			iAddValue			= this->CalcEffectValue(lpObj->AddDexterity + lpObj->Dexterity, wEffectValue, btEffectValueType);
			lpObj->AddDexterity	+= iAddValue;
		}
		break;
		// --
	case 36:
		{
			iAddValue			= this->CalcEffectValue(lpObj->AddVitality + lpObj->Vitality, wEffectValue, btEffectValueType);
			lpObj->AddVitality	+= iAddValue;
		}
		break;
		// --
	case 37:
		{
			iAddValue			= this->CalcEffectValue(lpObj->AddEnergy + lpObj->Energy, wEffectValue, btEffectValueType);
			lpObj->AddEnergy	+= iAddValue;
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//005a92b0	-> 100% (Not writed by WZ)
void CItemSocketOptionSystem::_SetUniqueEffect(LPOBJ lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{ 
	// ----
}
// -------------------------------------------------------------------------------

//005a92e0	-> 100%
void CItemSocketOptionSystem::_SetSetEffect(LPOBJ lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue)
{ 
	int iAddValue = 0;
	// ----
	switch(btEffectType)
	{
	case 44:
		{
			lpObj->SetOpDoubleDamage	+= wEffectValue;
		}
		break;
		// --
	case 45:
		{
			lpObj->SetOpIgnoreDefense	+= wEffectValue;
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//005a9360	-> 100%
int	CItemSocketOptionSystem::CalcEffectValue(int iInputValue, WORD wEffectValue, BYTE btEffectValueType)
{ 
	int iReturnValue = 0;	//ebp-8
	// ----
	if( !wEffectValue )
	{
		return false;
	}
	// ----
	switch(btEffectValueType)
	{
	case 1:
		{
			iReturnValue = wEffectValue;
		}
		break;
		// --
	case 2:
		{
			iReturnValue = iInputValue * wEffectValue / 100;
		}
		break;
		// --
	case 3:
		{
			iReturnValue = iInputValue / wEffectValue;
		}
		break;
		// --
	case 4:
		{
			iReturnValue = iInputValue / wEffectValue;
		}
		break;
		// --
	case 5:
		{
			iReturnValue = iInputValue / wEffectValue;
		}
		break;
		// --
	default:
		{
			iReturnValue = -1;
		}
		break;
	}
	// ----
	return iReturnValue;
}
// -------------------------------------------------------------------------------

//005a9470	-> 100%
void CItemSocketOptionSystem::SetRequireStatOption(CItem * lpItemdata)
{
	if( !lpItemdata )
	{
		return;
	}
	// ----
	if( !lpItemdata->IsItem() || !this->IsSocketItem(lpItemdata) )
	{
		return;
	}
	// ----
	BYTE SeedIndex				= 0;
	BYTE SphereLevel			= 0;
	BYTE SeedSphereIndex		= 0;
	CSeedSphereItem SeedSphere;			//ebp-2c
	int iReduceValue			= 0;
	// ----
	for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
	{
		if( lpItemdata->m_SocketOption[i] == 0xFF || lpItemdata->m_SocketOption[i] == 0xFE )
		{
			continue;
		}
		// ----
		SeedSphereIndex = lpItemdata->m_SocketOption[i];
        SeedIndex		= SeedSphereIndex % 50;
        SphereLevel		= (SeedSphereIndex - SeedIndex) / 50 + 1;
		// ----
		if( !this->MakeSeedSphere(&SeedSphere, SeedIndex, SphereLevel) )
		{
			continue;
		}
		// ----
		switch(SeedIndex)
		{
		case 38:
			{
				iReduceValue				= this->CalcEffectValue(lpItemdata->m_HJOpStrength, SeedSphere.m_SeedOptionValue, SeedSphere.m_SeedOptionValueType);
				lpItemdata->m_HJOpStrength	+= iReduceValue;
			}
			break;
			// --
		case 39:
			{
				iReduceValue				= this->CalcEffectValue(lpItemdata->m_HJOpDexterity, SeedSphere.m_SeedOptionValue, SeedSphere.m_SeedOptionValueType);
				lpItemdata->m_HJOpDexterity += iReduceValue;
			}
			break;
			// --
		case 20:
			{
				lpItemdata->m_ImproveDurabilityRate	+= SeedSphere.m_SeedOptionValue;
			}
			break;
		}
	}
}
// -------------------------------------------------------------------------------

//005a96f0	-> 100%
void CItemSocketOptionSystem::MakeSocketSlot(CItem * lpItemData, BYTE btSocketSlotCount)
{ 
	lpItemData->MakeSocketSlot(btSocketSlotCount);
}
// -------------------------------------------------------------------------------

//005a9720	-> 100%
BYTE CItemSocketOptionSystem::GetMakeSocketSlotCount(int iItemCode)
{ 
	int iSocketSlotCount = 0;
	// ----
	if( ItemAttribute[iItemCode].HaveItemInfo )
	{
		if( !ItemAttribute[iItemCode].TwoHand )
		{
			iSocketSlotCount	= this->m_MakeSocketSlotCountPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
		}
		else
		{
			iSocketSlotCount	= this->m_TwoHandWeaponMakeSocketSlotCountPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
		}
	}
	// ----
	return iSocketSlotCount;	//(BYTE)
}
// -------------------------------------------------------------------------------

//005a97b0	-> 100%
void CItemSocketOptionSystem::ClearUserData(LPOBJ lpObj)
{
	int iIndex = 0;	//ebp-8
	// ----
	if( !lpObj || lpObj->Type != 1 )
	{
		return;
	}
	// ----
	//-> Need define 35, 7, 2 counters
	// ----
	//MAX_SOCKET_SLOT * MAX_ITEM_SLOT (5 * 5 = 35)? Or maybe simply max value for list
	for( iIndex = 0; iIndex < 35; iIndex++ )
	{
		lpObj->m_SeedOptionList[iIndex].Clear();
	}
	// ----
	//???
	for( iIndex = 0; iIndex < 7; iIndex++ )
	{
		lpObj->m_BonusOptionList[iIndex].Clear();
	}
	// ----
	//???
	for( iIndex = 0; iIndex < 2; iIndex++ )
	{
		lpObj->m_SetOptionList[iIndex].Clear();
	}
}
// -------------------------------------------------------------------------------

//005a98b0	-> 100%
bool CItemSocketOptionSystem::SetSocketSlot(CItem * lpTargetItem, CItem * lpSeedSphere, BYTE btSocketSlotIndex)
{ 
	if( !lpTargetItem || !lpSeedSphere )
	{
		return false;
	}
	// ----
	if( !lpTargetItem->IsItem() || !lpSeedSphere->IsItem() )
	{
		return false;
	}
	// ----
	if( !this->IsEnableSocketItem(lpTargetItem) )
	{
		return false;
	}
	// ----
	if( btSocketSlotIndex > 5 )
	{
		return false;
	}
	// ----
	if( lpTargetItem->m_SocketOption[btSocketSlotIndex] != 0xFE )
	{
		return false;
	}
	// ----
	BYTE btItemIndex		= 0;	//ebp-8
	BYTE btSeedIndex		= 0;	//ebp-0c
	BYTE btSphereLevel		= 0;	//ebp-10
	BYTE btSeedSphereIndex	= 0;	//ebp-14
	// ----
	btItemIndex				= lpTargetItem->m_Type % 512;		//OK
	btSeedIndex				= btItemIndex - 100;				//OK
	btSphereLevel			= (btSeedIndex - 1) / 6 + 1;		//:D
	btSeedSphereIndex		= btSphereLevel * 50 + btSeedIndex;	//OK
	// ----
	lpTargetItem->m_SocketOption[btSocketSlotIndex] = btSeedSphereIndex;
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005a9a30	-> 100%
bool CItemSocketOptionSystem::SetSocketSlot(CItem * lpTargetItem, BYTE btSeedIndex, BYTE btSphereLevel, BYTE btSocketSlotIndex)
{ 
	if( !lpTargetItem )
	{
		return false;
	}
	// ----
	if( !lpTargetItem->IsItem() )
	{
		return false;
	}
	// ----
	if( !this->IsEnableSocketItem(lpTargetItem) )
	{
		return false;
	}
	// ----
	if( btSocketSlotIndex > 5 )
	{
		return false;
	}
	// ----
	if( lpTargetItem->m_SocketOption[btSocketSlotIndex] != 0xFE )
	{
		return false;
	}
	// ----
	BYTE btSeedSphereIndex = 0;
	// ----
	btSeedSphereIndex								= btSeedIndex + 50 * btSphereLevel;
	lpTargetItem->m_SocketOption[btSocketSlotIndex] = btSeedSphereIndex;
	// ----
    this->SetBonusSocketOption(lpTargetItem);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005a9b30	-> 100%
bool CItemSocketOptionSystem::SetSocketSlot(CItem * lpTargetItem, BYTE btSeedSphereIndex, BYTE btSocketSlotIndex)
{ 
	if( !lpTargetItem || !lpTargetItem->IsItem())
	{
		return false;
	}
	// ----
	if( !this->IsEnableSocketItem(lpTargetItem) )
	{
		return false;
	}
	// ----
	if( btSocketSlotIndex > 5 )
	{
		return false;
	}
	// ----
	if( lpTargetItem->m_SocketOption[btSocketSlotIndex] != 0xFE )
	{
		return false;
	}
	// ----
	lpTargetItem->m_SocketOption[btSocketSlotIndex] = btSeedSphereIndex;
	this->SetBonusSocketOption(lpTargetItem);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005a9c00	-> 95% (Need recheck)
bool CItemSocketOptionSystem::SetBonusSocketOption(CItem * lpItemData)
{
	BYTE SeedSphereType[5];	//???
	// ----
	if( !lpItemData )
	{
		return false;
	}
	// ----
	if( !lpItemData->IsItem() || !this->IsEnableSocketItem(lpItemData) )
	{
		return false;
	}
	// ----
	for( int iSlotIndex = 0; iSlotIndex < MAX_SOCKET_SLOT; iSlotIndex++ )
	{
		if( lpItemData->m_SocketOption[iSlotIndex] == 0xFF || lpItemData->m_SocketOption[iSlotIndex] == 0xFE )
		{
			continue;
		}
		// ----
		SeedSphereType[iSlotIndex] = this->GetSocketType(lpItemData->m_SocketOption[iSlotIndex]);
	}
	// ----
	BYTE btItemType 		= 0;	//ebp-14
	WORD wItemIndex			= 0;	//ebp-18
	BYTE btSocketSlotCount	= 0;	//ebp-1c
	// ----
	wItemIndex	= lpItemData->m_Type % 512;
	btItemType	= (lpItemData->m_Type - wItemIndex) / 512;
	// ----
	for( int iBonusIndex = 0; iBonusIndex < MAX_SEED_TYPE; iBonusIndex++ )
	{
		if( btItemType < this->m_BonusSocketOptionData[iBonusIndex].ItemTypeBegin || btItemType > this->m_BonusSocketOptionData[iBonusIndex].ItemTypeEnd )
		{
			continue;
		}
		// ----
		for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
		{
			if( this->m_BonusSocketOptionData[iBonusIndex].SocketSlotType[i] == SeedSphereType[i] && SeedSphereType[i] )
			{
				btSocketSlotCount++;
			}
			// ----
			if( btSocketSlotCount < this->m_BonusSocketOptionData[iBonusIndex].SocketSlotCount )
			{
				continue;
			}
			// ----
			if(	rand() % 100 < this->m_BonusSocketOptionData[iBonusIndex].ExtractRate )
			{
				lpItemData->m_BonusSocketOption = this->m_BonusSocketOptionData[iBonusIndex].OptionIndex;
				// ----
				LogAddTD("[SocketItem] Set Bonus Socket Option Success - OptionIndex : %d, ItemCode : %d, SocketOption[%d,%d,%d,%d,%d]",
					lpItemData->m_BonusSocketOption,
					lpItemData->m_Type,
					lpItemData->m_SocketOption[0],
					lpItemData->m_SocketOption[1],
					lpItemData->m_SocketOption[2],
					lpItemData->m_SocketOption[3],
					lpItemData->m_SocketOption[4]);
				// ----
				return true;
			}
			// ----
			lpItemData->m_BonusSocketOption = 0xFF;
			return false;
		}
		// ----
		btSocketSlotCount = 0;	//-> Need check it...
	}
	// ----
	lpItemData->m_BonusSocketOption = 0xFF;
	return false;
}
// -------------------------------------------------------------------------------

//005a9f40	-> 100%
bool CItemSocketOptionSystem::ClearSocketSlot(CItem * lpTargetItem, BYTE btSocketSlotIndex)
{
	if( !lpTargetItem )
	{
		return false;
	}
	// ----
	if( btSocketSlotIndex > 5 )
	{
		return false;
	}
	// ----
	if( lpTargetItem->m_SocketOption[btSocketSlotIndex] == 0xFF || lpTargetItem->m_SocketOption[btSocketSlotIndex] == 0xFE )
	{
		return false;
	}
	// ----
	lpTargetItem->m_SocketOption[btSocketSlotIndex] = 0xFE;
	this->SetBonusSocketOption(lpTargetItem);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005aa020	-> 100%
void SOCKET_OPTION_LIST::Clear()
{
	this->m_SocketOptionIndex		= 0xFF;
	this->m_SocketOptionValue		= 0;
	this->m_SocketOptionValueType	= 0;
	this->m_SocketOptionCount		= 0;
}
// -------------------------------------------------------------------------------

//005aa0b0	-> 100%
int CSeedSphereItem::GetItemCode()
{
	return this->m_ItemCode;
}
// -------------------------------------------------------------------------------

//005aa0d0	-> 100%
void BONUS_SOCKET_OPTION::Clear()
{
	this->OptionIndex		= 0;
	this->EffectType		= 0;
	this->ItemTypeBegin		= 0;
	this->ItemTypeEnd		= 0;
	this->OptionValueType	= 0;
	this->OptionValue		= 0;
	this->ExtractRate		= 0;
	// ----
	for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
	{
		this->SocketSlotType[i] = 0;
	}
	// ----
	this->SocketSlotCount = 0;
}
// -------------------------------------------------------------------------------