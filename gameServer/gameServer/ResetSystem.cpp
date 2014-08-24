#include "StdAfx.h"
#include "user.h"
#include "GameMain.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "ResetSystem.h"
#include "GameShop.h"
#include "Gate.h"
#include "ObjCalCharacter.h"
#include "LogToFile.h"
#include "MasterLevelSystem.h"
#include "MasterLevelSkillTreeSystem.h"

// -------------------------------------------------------------------------

CLogToFile g_ResetLog("Reset", ".\\LOG\\Reset", TRUE);
// -------------------------------------------------------------------------

#ifdef __CUSTOMS__
// -------------------------------------------------------------------------

ResetSystem		g_ResetSystem;
// -------------------------------------------------------------------------

ResetSystem::ResetSystem()
{
	this->Init();
}
// -------------------------------------------------------------------------

ResetSystem::~ResetSystem()
{

}
// -------------------------------------------------------------------------

void ResetSystem::Init()
{
	ZeroMemory(this->m_GroupData, sizeof(this->m_GroupData));
	this->m_GroupLoadedCount	= 0;
	this->m_MaxMoney			= 0;
	this->m_ReqCleanInventory	= true;
	this->m_ShopCleaning		= true;
	this->m_ResetStats			= true;
	this->m_ResetPoints			= true;
	this->m_NpcID				= 0;
	this->m_NpcMap				= 0;
	this->m_NpcX				= 0;
	this->m_NpcY				= 0;
	this->m_DynamicExp			= 0;
	this->m_DynamicExpMin		= 0.0f;
	this->m_DynamicExpPercent	= 0.0f;
	this->m_MarlonReset			= 0;
	this->m_MarlonStatMinLevel	= 220;
	this->m_MarlonComboMinLevel	= 220;
	this->m_BonusCommand		= 0;

	this->m_ResetSkill			= 0;
	this->m_ResetMasterLevel	= 0;
	this->m_ResetMasterStats	= 0;
	this->m_ResetMasterSKill	= 0;
}
// -------------------------------------------------------------------------

void ResetSystem::Load()
{
	this->Init();
	this->ReadGroupData(gDirPath.GetNewPath("Custom\\ResetGroup.txt"));
	this->ReadMainData(gDirPath.GetNewPath("Custom\\ResetMain.ini"));
}
// -------------------------------------------------------------------------

void ResetSystem::ReadGroupData(char * File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[ResetSystem] %s file not found", File);
		return;
	}
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
		if( Token != NUMBER )
		{
			continue;
		}
		// ----
		int GroupID = TokenNumber;
		// ----
		Token = GetToken();
		this->m_GroupData[GroupID].MinReset		= TokenNumber;
		// ----
		Token = GetToken();
		this->m_GroupData[GroupID].MaxReset		= TokenNumber;
		// ----
		Token = GetToken();
		this->m_GroupData[GroupID].ReqLevel		= TokenNumber;
		// ----
		Token = GetToken();
		this->m_GroupData[GroupID].ReqMoney		= TokenNumber;
		// ----
		Token = GetToken();
		this->m_GroupData[GroupID].ItemCount	= TokenNumber;
		// ----
		int RewardPerGroup	= 0;
		// ----
		while(true)
		{
			if( RewardPerGroup >= MAX_TYPE_PLAYER )
			{
				break;
			}
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].LevelPoint = TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].GensPoint = TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].WCoinC = TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].WCoinP = TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].GoblinPoint = TokenNumber;
			// ----
			RewardPerGroup++;
		}
		// ----
		int ItemPerGroup = 0;
		// ----
		while(true)
		{
			if( ItemPerGroup >= this->m_GroupData[GroupID].ItemCount )
			{
				break;
			}
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].ID			= TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].MinLevel		= TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].MaxLevel		= TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].MinOption		= TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].MaxOption		= TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsLuck		= TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsSkill		= TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsExcellent	= TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsAncient		= TokenNumber;
			// ----
			Token = GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsSocket		= TokenNumber;
			// ----
			ItemPerGroup++;
		}
		// ----
		this->m_GroupLoadedCount++;
		// ----
		if( this->m_GroupLoadedCount >= MAX_RESET_GROUP )
		{
			MsgBox("[ResetSystem] m_GroupLoadedCount error [%d]", this->m_GroupLoadedCount);
		}
	}
	// ----
	fclose(SMDFile);
	// ----
#if( RESETSYSTEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Loaded Groups: %d", __FUNCTION__, this->m_GroupLoadedCount);
	// ----
	for( int i = 0; i < this->m_GroupLoadedCount; i++ )
	{
		LogAddC(2, "%d", i);
		LogAddC(2, "%d %d %d %d %d", this->m_GroupData[i].MinReset, this->m_GroupData[i].MaxReset, 
			this->m_GroupData[i].ReqLevel, this->m_GroupData[i].ReqMoney, this->m_GroupData[i].ItemCount);
		// ----
		for( int k = 0; k < 7; k++ )
		{
			LogAddC(2, "%d %d %d %d %d", this->m_GroupData[i].RewardData[k].LevelPoint,
				this->m_GroupData[i].RewardData[k].GensPoint, this->m_GroupData[i].RewardData[k].WCoinC,
				this->m_GroupData[i].RewardData[k].WCoinP, this->m_GroupData[i].RewardData[k].GoblinPoint);
		}
		// ----
		for( int j = 0; j < this->m_GroupData[i].ItemCount; j++ )
		{
			LogAddC(2, "%d %d %d %d %d %d %d %d %d", this->m_GroupData[i].ItemData[j].ID,
					this->m_GroupData[i].ItemData[j].MinLevel,
					this->m_GroupData[i].ItemData[j].MaxLevel, this->m_GroupData[i].ItemData[j].MinOption,
					this->m_GroupData[i].ItemData[j].MaxOption, this->m_GroupData[i].ItemData[j].IsLuck,
					this->m_GroupData[i].ItemData[j].IsExcellent, this->m_GroupData[i].ItemData[j].IsAncient,
					this->m_GroupData[i].ItemData[j].IsSocket);
		}
		// ----
		LogAddC(2, "end");
	}
#endif
}
// -------------------------------------------------------------------------

void ResetSystem::ReadMainData(char * File)
{
	this->m_MaxMoney			= GetPrivateProfileInt("Common", "MaxMoney", 0, File);
	this->m_ShopCleaning		= GetPrivateProfileInt("Common", "PShopCleaning", 0, File);
	this->m_ReqCleanInventory	= GetPrivateProfileInt("Common", "InventoryCheck", 0, File);
	this->m_ResetStats			= GetPrivateProfileInt("Common", "ResetStats", 1, File);
	this->m_ResetPoints			= GetPrivateProfileInt("Common", "ResetPoints", 1, File);
	this->m_MarlonReset			= GetPrivateProfileFloat("Common", "MarlonReset", 0, File);
	this->m_MarlonStatMinLevel	= GetPrivateProfileFloat("Common", "MarlonStatMinLevel", 220, File);
	this->m_MarlonComboMinLevel	= GetPrivateProfileFloat("Common", "MarlonComboMinLevel", 220, File);
	this->m_BonusCommand		= GetPrivateProfileFloat("Common", "BonusCommand", 220, File);
	this->m_NpcMap				= GetPrivateProfileInt("NPC", "Map", 0, File);
	this->m_NpcID				= GetPrivateProfileInt("NPC", "ID", 0, File);
	this->m_NpcMap				= GetPrivateProfileInt("NPC", "Map", 0, File);
	this->m_NpcX				= GetPrivateProfileInt("NPC", "X", 0, File);
	this->m_NpcY				= GetPrivateProfileInt("NPC", "Y", 0, File);
	this->m_DynamicExp			= GetPrivateProfileInt("Experience", "DynamicExp", 0, File);
	this->m_DynamicExpMin		= GetPrivateProfileFloat("Experience", "MinExpRate", 1.0f, File);
	this->m_DynamicExpPercent	= GetPrivateProfileFloat("Experience", "Percent", 0.2f, File);

	this->m_ResetSkill			= GetPrivateProfileInt("Common", "ResetSkill", 0, File);
	this->m_ResetMasterLevel	= GetPrivateProfileInt("Common", "ResetMasterLevel", 0, File);
	this->m_ResetMasterStats	= GetPrivateProfileInt("Common", "ResetMasterStats", 0, File);
	this->m_ResetMasterSKill	= GetPrivateProfileInt("Common", "ResetMasterSKill", 0, File);
}
// -------------------------------------------------------------------------

int ResetSystem::GetResetMoney(LPOBJ lpUser)
{
	int Group = this->GetResetGroup(lpUser);
	// ----
	if( Group == -1 )
	{
		return -1;
	}
	// ----
	int Money = this->m_GroupData[this->GetResetGroup(lpUser)].ReqMoney * (lpUser->Reset + 1);
	// ----
	if( Money > this->m_MaxMoney )
	{
		Money = this->m_MaxMoney;
	}
	// ----
	return Money;
}
// -------------------------------------------------------------------------

int ResetSystem::GetResetGroup(LPOBJ lpUser)
{
	for( int Group = 0; Group < this->m_GroupLoadedCount; Group++ )
	{
		if( lpUser->Reset >= this->m_GroupData[Group].MinReset 
			&& lpUser->Reset < this->m_GroupData[Group].MaxReset )
		{
			return Group;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

bool ResetSystem::SearchQuestItem(LPOBJ lpUser, BYTE QuestItemID)
{
	if( this->GetResetGroup(lpUser) == (int)-1 )
	{
		LogAddC(2, "[ResetSystem] [%s] [%s] Invalid group", lpUser->AccountID, lpUser->Name);
		return false;
	}
	// ----
	WORD ItemID = this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].ID;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		if( !lpUser->pInventory[i].IsItem() || lpUser->pInventory[i].m_Type != ItemID )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Level < this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MinLevel
			|| lpUser->pInventory[i].m_Level > this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Option3 < this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MinOption
			|| lpUser->pInventory[i].m_Option3 > this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MaxOption )
		{
			continue;
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsLuck != 2 )
		{
			if( lpUser->pInventory[i].m_Option2 != this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsLuck )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSkill != 2 )
		{
			if( lpUser->pInventory[i].m_Option1 != this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSkill )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsExcellent )
		{
			if( !lpUser->pInventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsAncient )
		{
			if( !lpUser->pInventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSocket )
		{
			if( !gItemSocketOption.IsEnableSocketItem(ItemID) )
			{
				continue;
			}
		}
		// ----
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool ResetSystem::DeleteQuestItem(LPOBJ lpUser, BYTE QuestItemID)
{
	if( this->GetResetGroup(lpUser) == (int)-1 )
	{
		LogAddC(2, "[ResetSystem] [%s] [%s] Invalid group", lpUser->AccountID, lpUser->Name);
		return false;
	}
	// ----
	WORD ItemID = this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].ID;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		if( !lpUser->pInventory[i].IsItem() || lpUser->pInventory[i].m_Type != ItemID )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Level < this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MinLevel
			|| lpUser->pInventory[i].m_Level > this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Option3 < this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MinOption
			|| lpUser->pInventory[i].m_Option3 > this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].MaxOption )
		{
			continue;
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsLuck != 2 )
		{
			if( lpUser->pInventory[i].m_Option2 != this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsLuck )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSkill != 2 )
		{
			if( lpUser->pInventory[i].m_Option1 != this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSkill )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsExcellent )
		{
			if( !lpUser->pInventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsAncient )
		{
			if( !lpUser->pInventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemData[QuestItemID].IsSocket )
		{
			if( !gItemSocketOption.IsEnableSocketItem(ItemID) )
			{
				continue;
			}
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

bool ResetSystem::Dialog(LPOBJ lpUser, LPOBJ lpNpc)
{
	if(		lpNpc->Class		== this->m_NpcID 
		&&	lpNpc->MapNumber	== this->m_NpcMap
		&&	lpNpc->X			== this->m_NpcX
		&&	lpNpc->Y			== this->m_NpcY )
	{
		this->SendResetData(lpUser, true);
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------

bool ResetSystem::CheckAllReq(LPOBJ lpUser)
{
	if( this->GetResetGroup(lpUser) == (int)-1 )
	{
		LogAddC(2, "[ResetSystem] [%s] [%s] Invalid group", lpUser->AccountID, lpUser->Name);
		g_ResetLog.Output("[%s] [%s] [Fail] Invalid group (Reset: %d, Group: %d)", 
			lpUser->AccountID, lpUser->Name, lpUser->Reset, this->GetResetGroup(lpUser)); 
		return false;
	}
	// ----
	int	ResetGroup = this->GetResetGroup(lpUser);
	// ----
	if( lpUser->Level < this->m_GroupData[ResetGroup].ReqLevel )
	{
		g_ResetLog.Output("[%s] [%s] [Fail] Level is small to reset (%d / %d)", 
			lpUser->AccountID, lpUser->Name, lpUser->Level, this->m_GroupData[ResetGroup].ReqLevel); 
		return false;
	}
	// ----
	int Money = this->GetResetMoney(lpUser);
	// ----
	if( Money == -1 )
	{
		g_ResetLog.Output("[%s] [%s] [Fail] Money return == -1", 
			lpUser->AccountID, lpUser->Name);
		return false;
	}
	// ----
	if( lpUser->Money < Money )
	{
		g_ResetLog.Output("[%s] [%s] [Fail] Not have money for reset (%d / %d)", 
			lpUser->AccountID, lpUser->Name, lpUser->Money, Money); 
		return false;
	}
	// ----
	if( this->m_ReqCleanInventory )
	{
		for( int i = 0; i < 12; i++ )
		{
			if( lpUser->pInventory[i].IsItem() )
			{
				g_ResetLog.Output("[%s] [%s] [Fail] User have equiped items (ItemSlot: %d)", 
					lpUser->AccountID, lpUser->Name, i); 
				return false;
			}
		}
	}
	// ----
	if( this->m_GroupData[ResetGroup].ItemCount > 0 )
	{
		for( int i = 0; i < this->m_GroupData[ResetGroup].ItemCount; i++ )
		{
			if( !this->SearchQuestItem(lpUser, i) )
			{
				g_ResetLog.Output("[%s] [%s] [Fail] User not have req. items ([%d] ItemID: %d)",
					lpUser->AccountID, lpUser->Name, i, this->m_GroupData[ResetGroup].ItemData[i].ID); 
				return false;
			}
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------

void ResetSystem::FinishResetReq(LPOBJ lpUser)
{
	if( this->GetResetGroup(lpUser) == (int)-1 )
	{
		LogAddC(2, "[ResetSystem] [%s] [%s] Invalid group", lpUser->AccountID, lpUser->Name);
		return;
	}
	// ----
	RESET_REQ_USERDATA pSend;
	pSend.h.set((LPBYTE)&pSend, ResetProtocol::Case, ResetProtocol::ResetFinish, sizeof(pSend));
	// ----
	int ResetGroup = this->GetResetGroup(lpUser);
	// ----
	if( !this->CheckAllReq(lpUser) )
	{
		GCServerMsgStringSend("You must finish all the requirements for make Reset", lpUser->m_Index, 1);
		return;
	}
	// ----
	if( this->m_GroupData[ResetGroup].ItemCount > 0 )
	{
		for( int i = 0; i < this->m_GroupData[ResetGroup].ItemCount; i++ )
		{
			if( !this->DeleteQuestItem(lpUser, i) )
			{
				GCServerMsgStringSend("Please check quest items positions", lpUser->m_Index, 1);
				return;
			}
		}
	}
	// ----
	lpUser->Level		= 1;
	lpUser->Experience	= 0;
	// ----
	if( this->m_ResetStats )
	{
		lpUser->Strength	= DCInfo.DefClass[lpUser->Class].Strength;
		lpUser->Dexterity	= DCInfo.DefClass[lpUser->Class].Dexterity;
		lpUser->Energy		= DCInfo.DefClass[lpUser->Class].Energy;
		lpUser->Vitality	= DCInfo.DefClass[lpUser->Class].Vitality;
		lpUser->Leadership	= DCInfo.DefClass[lpUser->Class].Leadership + this->m_BonusCommand;
	}
	// ----
	if( this->m_MarlonReset )
	{
		g_QuestInfo.ReSetQuestState(lpUser, 2);
		g_QuestInfo.ReSetQuestState(lpUser, 3);
	}
	// ----
	if( m_ResetPoints )
	{
		lpUser->LevelUpPoint = this->m_GroupData[ResetGroup].RewardData[lpUser->Class].LevelPoint * (lpUser->Reset + 1);
	}
	else
	{
		lpUser->LevelUpPoint += this->m_GroupData[ResetGroup].RewardData[lpUser->Class].LevelPoint * (lpUser->Reset + 1);
	}

	if( this->m_ResetSkill && this->m_ResetMasterSKill )
	{
		for(int n = 0; n < MAGIC_SIZE;n++)
		{
			if(lpUser->Magic[n].IsMagic() == TRUE)
			{
				lpUser->Magic[n].Clear();
			}
		}
	}
	else if( this->m_ResetSkill )
	{
		for(int n = 0; n < MAGIC_SIZE;n++)
		{
			if(lpUser->Magic[n].IsMagic() == TRUE && g_MasterSkillSystem.CheckMasterLevelSkill(lpUser->Magic[n].m_Skill) == FALSE)
			{
				lpUser->Magic[n].Clear();
			}
		}
	}

	if( this->m_ResetMasterLevel )
	{
		lpUser->m_nMasterLevel = 0;
		lpUser->m_i64MasterLevelExp = 0;
		g_MasterLevelSystem.gObjNextMLExpCal(lpUser);
	}

	if( this->m_ResetMasterStats )
	{
		lpUser->m_iMasterLevelPoint = 0;
	}

	// ----
#ifdef GENS
	lpUser->m_ContributePoint		+= this->m_GroupData[ResetGroup].RewardData[lpUser->Class].GensPoint;
#endif
	lpUser->GameShop.WCoinC			+= this->m_GroupData[ResetGroup].RewardData[lpUser->Class].WCoinC;
	lpUser->GameShop.WCoinP			+= this->m_GroupData[ResetGroup].RewardData[lpUser->Class].WCoinP;
	lpUser->GameShop.GoblinPoint	+= this->m_GroupData[ResetGroup].RewardData[lpUser->Class].GoblinPoint;
	// ----
	lpUser->Money					-= this->GetResetMoney(lpUser);
	GCMoneySend(lpUser->m_Index, lpUser->Money);
	lpUser->Reset++;
	// ----
	g_ResetLog.Output("[%s] [%s] [Success] Reset: %d -> %d | Points: %d",
		lpUser->AccountID, lpUser->Name, lpUser->Reset - 1, lpUser->Reset, lpUser->LevelUpPoint);
	// ----
	if( lpUser->PartyNumber >= 0 )
	{
		gParty.Delete(lpUser->PartyNumber, lpUser->m_Index, lpUser->DBNumber);
		gParty.UpdatePKPartyPanalty(lpUser->PartyNumber);
		// ----
		for( int n = 0; n < MAX_USER_IN_PARTY; n++ )
		{
			int PartyID = gParty.m_PartyS[lpUser->PartyNumber].Number[n];
			// ----
			if( PartyID >= 0 )
			{
				GCPartyDelUserSend(PartyID);
			}
		}
		// ----
		lpUser->PartyNumber		= -1;
		lpUser->PartyTargetUser	= -1;
		CGPartyListAll(lpUser->PartyNumber);
	}
	// ----
	int Gate = -1;
	short X, Y, Level;
	BYTE MapNumber, Dir;
	// ----
	if( lpUser->Class == CLASS_ELF )
	{
		Gate = 27;
	}
	else if( lpUser->Class == CLASS_SUMMONER )
	{
		Gate = 267;
	}
	else
	{
		Gate = 17;
	}
	// ----
	gGateC.GetGate(Gate, (short&)X, (short&)Y, (BYTE&)MapNumber, (BYTE&)Dir, (short&)Level);
	// ----
	lpUser->MapNumber	= MapNumber;
	lpUser->X			= X;
	lpUser->Y			= Y;
	lpUser->Dir			= Dir;
	// ----
	DataSend(lpUser->m_Index, (LPBYTE)&pSend, pSend.h.size);
	g_ResetSystem.SendResetData(lpUser, false);
	gObjCloseSet(lpUser->m_Index, 1);
}
// -------------------------------------------------------------------------

void ResetSystem::SendResetData(LPOBJ lpUser, bool ByDialog)
{
	if( ByDialog && this->GetResetGroup(lpUser) == (int)-1 )
	{
		LogAddC(2, "[ResetSystem] [%s] [%s] Invalid group", lpUser->AccountID, lpUser->Name);
		return;
	}
	// ----
	RESET_ANS_USERDATA pSend;
	pSend.h.set((LPBYTE)&pSend, ResetProtocol::Case, ResetProtocol::UserData, sizeof(pSend));
	// ----
	pSend.ByDialog	= ByDialog;
	pSend.Reset		= lpUser->Reset;
	pSend.GrandReset = lpUser->GrandReset;
	// ----
	if( this->m_GroupData[this->GetResetGroup(lpUser)].ItemCount > 0 )
	{
		for( int i = 0; i < this->m_GroupData[this->GetResetGroup(lpUser)].ItemCount; i++ )
		{
			pSend.ItemCheck[i] = true;
			// ----
			if( !this->SearchQuestItem(lpUser, i) )
			{
				pSend.ItemCheck[i] = false;
			}
		}
	}
	// ----
	DataSend(lpUser->m_Index, (LPBYTE)&pSend, pSend.h.size);
}
// -------------------------------------------------------------------------------

float ResetSystem::GetPrivateProfileFloat(char * Section, char * Key, float DefaultValue, char * File)
{
	char	Result[255];
	char	Default[255];
	float	FloatResult;
	// ----
	wsprintf(Default, "%f", DefaultValue);
	GetPrivateProfileStringA(Section, Key, Default, Result, 255, File); 
	FloatResult = atof(Result);
	// ----
	return FloatResult;
}
// -------------------------------------------------------------------------

float ResetSystem::GetDynamicExp(LPOBJ lpUser)
{
	if( !this->m_DynamicExp )
	{
		return gAddExperience;
	}
	// ----
	float AddExperience = 0;
	// ----
	AddExperience = gAddExperience - (this->m_DynamicExpPercent * lpUser->Reset);
	// ----
	if( AddExperience < this->m_DynamicExpMin )
	{
		AddExperience = this->m_DynamicExpMin;
	}
	// ----
	return AddExperience;
}
// -------------------------------------------------------------------------------

#endif
// -------------------------------------------------------------------------------