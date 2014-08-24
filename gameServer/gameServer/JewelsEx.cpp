#include "StdAfx.h"
#include "JewelsEx.h"
#include "GameMain.h"
#include "logproc.h"
#include "..\include\ReadScript.h"
#include "..\common\winutil.h"
#include "LuckyItem.h"
// -------------------------------------------------------------------------

JewelsEx g_JewelsEx;
// -------------------------------------------------------------------------

JewelsEx::JewelsEx()
{
	this->Init();
}
// -------------------------------------------------------------------------

JewelsEx::~JewelsEx()
{

}
// -------------------------------------------------------------------------

void JewelsEx::Init()
{
	this->m_LoadedCount = 0;
	// ----
	for( int i = 0 ; i < JEWELSEX_ITEMS; i++ )
	{
		this->m_Data[i].ItemID				= -1;
		this->m_Data[i].SuccessRate			= 0;
		this->m_Data[i].AllowAccessory		= 0;
		this->m_Data[i].AllowWings			= 0;
		this->m_Data[i].AddLevel			= 0;
		this->m_Data[i].MinLevel			= 0;
		this->m_Data[i].MaxLevel			= 0;
		this->m_Data[i].AddSkill			= 0;
		this->m_Data[i].AddLuck				= 0;
		this->m_Data[i].AddOption			= 0;
		this->m_Data[i].AddExc				= 0;
		this->m_Data[i].AddExcOption		= 0;
		this->m_Data[i].MaxExcOptionCount	= 0;
	}
}
// -------------------------------------------------------------------------

void JewelsEx::Load()
{
	this->Init();
	this->ReadData(gDirPath.GetNewPath("Custom\\JewelsEx.txt"));
}
// -------------------------------------------------------------------------

void JewelsEx::ReadData(char * File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[JewelsEx] %s file not found", File);
		return;
	}
	// ----
	int Category			= -1;
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
			if( this->IsJewelEx(ITEMGET(14, Category)) )
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->m_Data[this->m_LoadedCount].ItemID			= ITEMGET(14, Category);
				this->m_Data[this->m_LoadedCount].SuccessRate		= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].AllowAccessory		= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].AllowWings			= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].AddLevel			= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].MinLevel			= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].MaxLevel			= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].AddSkill			= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].AddLuck			= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].AddOption			= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].AddExc			= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].AddExcOption		= TokenNumber;
				Token = GetToken();
				this->m_Data[this->m_LoadedCount].MaxExcOptionCount	= TokenNumber;
				// ----
#if( JEWELSEX_DEBUG == 1 )
				LogAddTD("[DEBUG] [JewelsEx] Item [%d] data:", this->m_Data[this->m_LoadedCount].ItemID);
				LogAdd("%d | %d | %d", 
					this->m_Data[this->m_LoadedCount].SuccessRate,
					this->m_Data[this->m_LoadedCount].AllowAccessory,
					this->m_Data[this->m_LoadedCount].AllowWings);
				LogAdd("%d | %d | %d", 
					this->m_Data[this->m_LoadedCount].AddLevel,
					this->m_Data[this->m_LoadedCount].MinLevel,
					this->m_Data[this->m_LoadedCount].MaxLevel);
				LogAdd("%d | %d | %d", 
					this->m_Data[this->m_LoadedCount].AddSkill,
					this->m_Data[this->m_LoadedCount].AddLuck,
					this->m_Data[this->m_LoadedCount].AddOption);
				LogAdd("%d | %d | %d", 
					this->m_Data[this->m_LoadedCount].AddExc,
					this->m_Data[this->m_LoadedCount].AddExcOption,
					this->m_Data[this->m_LoadedCount].MaxExcOptionCount);
#endif
				// ----
				this->m_LoadedCount++;
			}
		}
	}
	// ----
	fclose(SMDFile);
	LogAddTD("[JewelsEx] %s file is loaded [%d] jewels", File, this->m_LoadedCount);
}
// -------------------------------------------------------------------------

int JewelsEx::GetJewelID(int ItemID)
{
	for( int i = 0; i < JEWELSEX_ITEMS; i++ )
	{
		if( this->m_Data[i].ItemID == ItemID )
		{
			return i;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

bool JewelsEx::IsJewelEx(int ItemID)
{
#if( JEWELSEX_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [JewelsEx] IsJewelEx");
#endif
	// ----
	if( ItemID >= ITEMGET(14, 180) && ItemID <= ITEMGET(14, 184) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------

BYTE JewelsEx::GetExcOptionCount(BYTE ExcellentOption)
{
	int ExcellentCount = 0;
	// ----
	for( int i = 0; i < 6; i++ )
	{
		if( (ExcellentOption >> i) & 1 )
		{
			ExcellentCount++;
		}
	}
	// ----
	return ExcellentCount;
}
// -------------------------------------------------------------------------

BYTE JewelsEx::GetExcUnusedOption(BYTE ExcellentOption)
{
	int arrayopt[6]	= { 0, 0, 0, 0, 0, 0};
	int num			= 32;
	int amountopt	= 0;
	int added		= 0;
	// ----
	for( int i = 5; i >= 0; i-- )
	{
		if( ExcellentOption >= num )
		{
			arrayopt[i] = 1;
			amountopt++;
			ExcellentOption -= num;
		}
		num = num / 2;
	}
	// ----
	int randomopt = rand()%(6-amountopt)+1;
	amountopt = 0;

	for(int i=0; i<6; i++)
	{
		if(!arrayopt[i])
		{ 
			amountopt++;
			if( amountopt == randomopt )
			{
				arrayopt[i] = 1;
				break;
			}
		}
		else
		{
			continue;
		}
	}

	num = 1; 
	ExcellentOption = 0;

	for (int i=0 ; i<6; i++)
	{
		if (arrayopt[i])
		{
			ExcellentOption += num;
		}
		num = num * 2;
	}

	return ExcellentOption;
}
// -------------------------------------------------------------------------

BYTE JewelsEx::GetExcNewOption(BYTE Amount)
{
	int Data[6]		= {1, 2, 4, 8, 16, 32};
	BYTE Excellent	= 0;
	// ----
	std::random_shuffle(Data, Data + 6);
	// ----
	for( int i = 0; i < Amount; i++ )
	{
		Excellent += Data[i];
	}
	// ----
	return Excellent;
}
// -------------------------------------------------------------------------

bool JewelsEx::IsWings(int ItemID)
{
	if(		ItemID >= ITEMGET(12, 0) && ItemID <= ITEMGET(12, 6)
		||	ItemID >= ITEMGET(12, 36) && ItemID <= ITEMGET(12, 43)
		||	ItemID == ITEMGET(13, 30)
		||	ItemID == ITEMGET(12, 49)
		||	ItemID == ITEMGET(12, 50)
#ifdef NEWWINGS
		||	IS_NEWWINGS(ItemID) 
#endif
		)
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------

bool JewelsEx::IsAccessory(int ItemID)
{
	if(		ItemID >= ITEMGET(13, 21) && ItemID <= ITEMGET(13, 29)
		||	ItemID == ITEMGET(13, 8)
		||	ItemID == ITEMGET(13, 9)
		||	ItemID == ITEMGET(13, 12)
		||	ItemID == ITEMGET(13, 13) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------

bool JewelsEx::CheckItem(LPOBJ lpUser, BYTE ID, BYTE Target)
{
	if(		lpUser->pInventory[Target].IsPeriodItem()
		||	g_LuckyItemManager.IsLuckyItemEquipment(lpUser->pInventory[Target].m_Type)
		||	lpUser->pInventory[Target].m_Type == ITEMGET(4, 7)
		||	lpUser->pInventory[Target].m_Type == ITEMGET(4, 5) )
	{
		LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Wrong target)",
				lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
		return false;
	}
	// ----
	if( this->m_Data[ID].AllowWings == 1 )
	{
		if( !this->IsWings(lpUser->pInventory[Target].m_Type) )
		{
			LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Wrong target)",
				lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
			return false;
		}
		// ----
		return true;
	}
	// ----
	if( this->m_Data[ID].AllowAccessory == 1 )
	{
		if( !this->IsAccessory(lpUser->pInventory[Target].m_Type) )
		{
			LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Wrong target)",
				lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
			return false;
		}
		// ----
		return true;
	}
	// ----
	if( lpUser->pInventory[Target].m_Type >= ITEMGET(12, 0) )
	{
		bool Flag = false;
		// ----
		if(	this->IsWings(lpUser->pInventory[Target].m_Type) )
		{
			if( this->m_Data[ID].AllowWings == 2 )
			{
				Flag = true;
			}
		}
		else if( this->IsAccessory(lpUser->pInventory[Target].m_Type) )
		{
			if( this->m_Data[ID].AllowAccessory == 2 )
			{
				Flag = true;
			}
		}
		// ----
		if( !Flag )
		{
			LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Wrong target)",
				lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
			return false;
		}
	}
	// ----
	if( this->m_Data[ID].AddLevel > 0 )
	{
		if(		lpUser->pInventory[Target].m_Level < this->m_Data[ID].MinLevel 
			||	lpUser->pInventory[Target].m_Level > this->m_Data[ID].MaxLevel )
		{
			LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Level range)",
				lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
			return false;
		}
	}
	// ----
	if( this->m_Data[ID].AddSkill > 0 && this->m_Data[ID].AddSkill != 2 )
	{
		if( ItemAttribute[lpUser->pInventory[Target].m_Type].SkillType == 0 )
		{
			LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Item cant have skill)",
				lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
			return false;
		}
		// ----
		if( lpUser->pInventory[Target].m_Option1 == 1 )
		{
			LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Item have skill)",
				lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
			return false;
		}
	}
	// ----
	if( this->m_Data[ID].AddLuck > 0 && this->m_Data[ID].AddLuck != 2 )
	{
		if( lpUser->pInventory[Target].m_Option2 == 1 )
		{
			LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Item have luck)",
				lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
			return false;
		}
	}
	// ----
	if( this->m_Data[ID].AddExc > 0 )
	{
		if( this->m_Data[ID].AddExc == 1 )
		{
			if( this->GetExcOptionCount(lpUser->pInventory[Target].m_NewOption) >= this->m_Data[ID].MaxExcOptionCount )
			{
				LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Item have max excellent options)",
					lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
				return false;
			}
		}
		else if( this->m_Data[ID].AddExc == 2 )
		{
			if( this->GetExcOptionCount(lpUser->pInventory[Target].m_NewOption) )
			{
				LogAddTD("[JewelsEx] [%s] [%s] [%d] Upgrade is fail (Item have excellent status)",
					lpUser->AccountID, lpUser->Name, this->m_Data[ID].ItemID);
				return false;
			}
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------

void JewelsEx::Upgrade(LPOBJ lpUser, BYTE Source, BYTE Target)
{
	if( Source < 0 || Source > MAIN_INVENTORY_SIZE - 1 )
	{
		GCReFillSend(lpUser->m_Index, lpUser->Life, 0xFD, 1, lpUser->iShield);
		return;
	}
	// ----
	if( Target < 0 || Target > MAIN_INVENTORY_SIZE - 1 )
	{
		GCReFillSend(lpUser->m_Index, lpUser->Life, 0xFD, 1, lpUser->iShield);
		return;
	}
	// ----
	if( !lpUser->pInventory[Source].IsItem() || !lpUser->pInventory[Target].IsItem() )
	{
		GCReFillSend(lpUser->m_Index, lpUser->Life, 0xFD, 1, lpUser->iShield);
		return;
	}
	// ----
	int ID				= GetJewelID(lpUser->pInventory[Source].m_Type);
	BYTE SuccessRate	= rand() % 100;
	// ----
	if( ID == -1 )
	{
		LogAddC(2, "[JewelsEx] [%s] [%s] [%d] Error to get info from data",
			lpUser->AccountID, lpUser->Name, lpUser->pInventory[Source].m_Type);
		GCReFillSend(lpUser->m_Index, lpUser->Life, 0xFD, 1, lpUser->iShield);
		return;
	}
	// ----
	if( !this->CheckItem(lpUser, ID, Target) )
	{
		GCReFillSend(lpUser->m_Index, lpUser->Life, 0xFD, 1, lpUser->iShield);
		return;
	}
	// ----
	if( SuccessRate > this->m_Data[ID].SuccessRate )
	{
		LogAddTD("[JewelsEx] [%s] [%s] [%d] Success rate is small [%d / %d]",
			lpUser->AccountID, lpUser->Name, lpUser->pInventory[Source].m_Type, 
			SuccessRate, this->m_Data[ID].SuccessRate);
		gObjInventoryItemSet(lpUser->m_Index, Source, -1);
		lpUser->pInventory[Source].Clear();
		GCInventoryItemDeleteSend(lpUser->m_Index, Source, 1);
		return;
	}
	// ----
	if( this->m_Data[ID].AddLevel > 0 )
	{
		lpUser->pInventory[Target].m_Level += this->m_Data[ID].AddLevel;
		// ----
		if( lpUser->pInventory[Target].m_Level > 15 )
		{
			lpUser->pInventory[Target].m_Level = 15;
		}
	}
	// ----
	if( this->m_Data[ID].AddSkill > 0 )
	{
		lpUser->pInventory[Target].m_Option1 = 1;
	}
	// ----
	if( this->m_Data[ID].AddLuck > 0 )
	{
		lpUser->pInventory[Target].m_Option2 = 1;
	}
	// ----
	if( this->m_Data[ID].AddOption > 0 )
	{
		lpUser->pInventory[Target].m_Option3 += this->m_Data[ID].AddOption;
		// ----
		if( lpUser->pInventory[Target].m_Option3 > 7 )
		{
			lpUser->pInventory[Target].m_Option3 = 7;
		}
	}
	// ----
	if( this->m_Data[ID].AddExc > 0 )
	{
		if( this->m_Data[ID].AddExc == 1 )
		{
			BYTE NewOption = this->GetExcUnusedOption(lpUser->pInventory[Target].m_NewOption);
			lpUser->pInventory[Target].m_NewOption = NewOption;
		}
		else if( this->m_Data[ID].AddExc == 2 || this->m_Data[ID].AddExc == 3 )
		{
			if( this->m_Data[ID].AddExcOption )
			{
				lpUser->pInventory[Target].m_NewOption = this->m_Data[ID].AddExcOption;
			}
			else
			{
				lpUser->pInventory[Target].m_NewOption = this->GetExcNewOption(this->m_Data[ID].MaxExcOptionCount);
			}
		}
	}
	// ----
	LogAddTD("[JewelsEx] [%s] [%s] [%d] (%d) Item has been upgraded",
		lpUser->AccountID, lpUser->Name, lpUser->pInventory[Source].m_Type, lpUser->pInventory[Target].m_Type);
	// ----
	gObjInventoryItemSet(lpUser->m_Index, Source, -1);
	lpUser->pInventory[Source].Clear();
	GCInventoryItemOneSend(lpUser->m_Index, Target);
	GCInventoryItemDeleteSend(lpUser->m_Index, Source, 1);
}
// -------------------------------------------------------------------------