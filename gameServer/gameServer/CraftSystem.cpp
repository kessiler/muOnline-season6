#include "StdAfx.h"
#include "user.h"
#include "CraftSystem.h"
#include "GameMain.h"
#include "logproc.h"
#include "..\include\ReadScript.h"
#include "..\common\winutil.h"
// -------------------------------------------------------------------------------

#ifdef __NOVUS__
// -------------------------------------------------------------------------------

CraftSystem g_CraftSystem;
// -------------------------------------------------------------------------------

CraftSystem::CraftSystem()
{
	this->Init();
}
// -------------------------------------------------------------------------------

CraftSystem::~CraftSystem()
{

}
// -------------------------------------------------------------------------------

void CraftSystem::Init()
{
	ZeroMemory(this->m_StageData, sizeof(m_StageData));
	this->m_StageLoadedCount	= 0;
	this->m_NpcID				= 0;
	this->m_NpcMap				= 0;
	this->m_NpcX				= 0;
	this->m_NpcY				= 0;
}
// -------------------------------------------------------------------------------

void CraftSystem::Load()
{
	this->Init();
	this->ReadStageData(gDirPath.GetNewPath("Custom\\CraftStage.txt"));
	this->ReadMainData(gDirPath.GetNewPath("Custom\\CraftMain.ini"));
}
// -------------------------------------------------------------------------------

void CraftSystem::ReadStageData(char * File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[CraftSystem] %s file not found", File);
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
		int StageID = TokenNumber;
		// ----
		Token = GetToken();
		strcpy(this->m_StageData[StageID].StageName, TokenString);
		// ----
		Token = GetToken();
		this->m_StageData[StageID].QuestCount = TokenNumber;
		// ----
		Token = GetToken();
		this->m_StageData[StageID].ReqLevel = TokenNumber;
		// ----
		Token = GetToken();
		this->m_StageData[StageID].ReqReset = TokenNumber;
		// ----
		int QuestPerStage	= 0;
		// ----
		while(true)
		{
			Token = GetToken();
			// ----
			if( QuestPerStage >= this->m_StageData[StageID].QuestCount || !strcmp("end", TokenString) )
			{
				break;
			}
			// ----
			strcpy(this->m_StageData[StageID].QuestData[QuestPerStage].QuestName, TokenString);
			// ----
			Token = GetToken();
			this->m_StageData[StageID].QuestData[QuestPerStage].ItemCount = TokenNumber;
			// ----
			Token = GetToken();
			this->m_StageData[StageID].QuestData[QuestPerStage].ReqMoney = TokenNumber;
			// ----
			int	ItemPerQuest = 0;
			// ----
			while(true)
			{
				if( ItemPerQuest >= this->m_StageData[StageID].QuestData[QuestPerStage].ItemCount )
				{
					break;
				}
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].ID = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].IsCrafted = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].MinLevel = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].MaxLevel = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].MinOption = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].MaxOption = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].IsLuck = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].IsSkill = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].IsExcellent = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].IsAncient = TokenNumber;
				// ----
				Token = GetToken();
				this->m_StageData[StageID].QuestData[QuestPerStage].ItemData[ItemPerQuest].IsSocket = TokenNumber;
				// ----
				ItemPerQuest++;
			}
			// ----
			QuestPerStage++;
		}
		// ----
		this->m_StageLoadedCount++;
		// ----
		if( this->m_StageLoadedCount >= MAX_CRAFT_STAGE_COUNT )
		{
			MsgBox("[CraftSystem] m_StageLoadedCount error [%d]", this->m_StageLoadedCount);
		}
	}
	// ----
	fclose(SMDFile);
	LogAddTD("[CraftSystem] Loaded Stages: %d", this->m_StageLoadedCount);
	// ----
#if( CRAFTSYSTEM_DEBUG == 1 )
	LogAddC(2, "[DEBUG] [%s] Loaded Stages: %d", __FUNCTION__, this->m_StageLoadedCount);
	// ----
	for( int i = 0; i < this->m_StageLoadedCount; i++ )
	{
		LogAddC(2, "%d", i);
		LogAddC(2, "%s %d %d %d", this->m_StageData[i].StageName, this->m_StageData[i].QuestCount, 
			this->m_StageData[i].ReqLevel, this->m_StageData[i].ReqReset);
		// ----
		for( int j = 0; j < this->m_StageData[i].QuestCount; j++ )
		{
			LogAddC(2, "%d %d", this->m_StageData[i].QuestData[j].ItemCount, 
				this->m_StageData[i].QuestData[j].ReqMoney);
			// ----
			for( int k = 0; k < this->m_StageData[i].QuestData[j].ItemCount; k++ )
			{
				LogAddC(2, "%d %d %d %d %d %d %d %d %d %d", this->m_StageData[i].QuestData[j].ItemData[k].ID,
					this->m_StageData[i].QuestData[j].ItemData[k].IsCrafted, this->m_StageData[i].QuestData[j].ItemData[k].MinLevel,
					this->m_StageData[i].QuestData[j].ItemData[k].MaxLevel, this->m_StageData[i].QuestData[j].ItemData[k].MinOption,
					this->m_StageData[i].QuestData[j].ItemData[k].MaxOption, this->m_StageData[i].QuestData[j].ItemData[k].IsLuck,
					this->m_StageData[i].QuestData[j].ItemData[k].IsExcellent, this->m_StageData[i].QuestData[j].ItemData[k].IsAncient,
					this->m_StageData[i].QuestData[j].ItemData[k].IsSocket);
			}
		}
		// ----
		LogAddC(2, "end");
	}
#endif
}
// -------------------------------------------------------------------------------

void CraftSystem::ReadMainData(char * File)
{
	this->m_NpcID	= GetPrivateProfileIntA("NPC", "ID", 0, File);
	this->m_NpcMap	= GetPrivateProfileIntA("NPC", "Map", 0, File);
	this->m_NpcX	= GetPrivateProfileIntA("NPC", "X", 0, File);
	this->m_NpcY	= GetPrivateProfileIntA("NPC", "Y", 0, File);
}
// -------------------------------------------------------------------------------

bool CraftSystem::CheckStageReq(LPOBJ lpUser)
{
	if( lpUser->Level < this->m_StageData[lpUser->m_CraftStage].ReqLevel )
	{
		return false;
	}
	// ----
	if( lpUser->Reset < this->m_StageData[lpUser->m_CraftStage].ReqReset )
	{
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

bool CraftSystem::CheckQuestReq(LPOBJ lpUser)
{
	if( lpUser->Money < this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ReqMoney )
	{
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

bool CraftSystem::SearchQuestItem(LPOBJ lpUser, BYTE QuestItemID)
{
	WORD ItemID = this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].ID;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		if( !lpUser->pInventory[i].IsItem() || lpUser->pInventory[i].m_Type != ItemID )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Level < this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].MinLevel
			|| lpUser->pInventory[i].m_Level > this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Option3 < this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].MinOption
			|| lpUser->pInventory[i].m_Option3 > this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].MaxOption )
		{
			continue;
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsLuck != 2 )
		{
			if( lpUser->pInventory[i].m_Option2 != this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsLuck )
			{
				continue;
			}
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsSkill != 2 )
		{
			if( lpUser->pInventory[i].m_Option1 != this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsSkill )
			{
				continue;
			}
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsExcellent )
		{
			if( !lpUser->pInventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsAncient )
		{
			if( !lpUser->pInventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsSocket )
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

bool CraftSystem::DeleteQuestItem(LPOBJ lpUser, BYTE QuestItemID)
{
	WORD ItemID = this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].ID;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		if( !lpUser->pInventory[i].IsItem() || lpUser->pInventory[i].m_Type != ItemID )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Level < this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].MinLevel
			|| lpUser->pInventory[i].m_Level > this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].MaxLevel )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Option3 < this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].MinOption
			|| lpUser->pInventory[i].m_Option3 > this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].MaxOption )
		{
			continue;
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsLuck != 2 )
		{
			if( lpUser->pInventory[i].m_Option2 != this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsLuck )
			{
				continue;
			}
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsSkill != 2 )
		{
			if( lpUser->pInventory[i].m_Option1 != this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsSkill )
			{
				continue;
			}
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsExcellent )
		{
			if( !lpUser->pInventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsAncient )
		{
			if( !lpUser->pInventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemData[QuestItemID].IsSocket )
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

bool CraftSystem::CheckAllReq(LPOBJ lpUser)
{
	if( !CheckStageReq(lpUser) || !CheckQuestReq(lpUser) )
	{
		return false;
	}
	// ----
	for( int i = 0; i < this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemCount; i++ )
	{
		if( !this->SearchQuestItem(lpUser, i) )
		{
			return false;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

bool CraftSystem::Dialog(LPOBJ lpUser, LPOBJ lpNpc)
{
	if(		lpNpc->Class		== this->m_NpcID 
		&&	lpNpc->MapNumber	== this->m_NpcMap
		&&	lpNpc->X			== this->m_NpcX
		&&	lpNpc->Y			== this->m_NpcY )
	{
		this->SendCraftData(lpUser, true);
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

void CraftSystem::SendCraftData(LPOBJ lpUser, bool ByDialog)
{
	CRAFT_ANS_USERDATA pSend;
	pSend.h.set((LPBYTE)&pSend, CraftProtocol::Case, CraftProtocol::UserData, sizeof(pSend));
	// ----
	pSend.ByDialog		= ByDialog;
	pSend.CraftLevel	= lpUser->m_CraftLevel;
	pSend.CraftStage	= lpUser->m_CraftStage;
	pSend.CraftQuest	= lpUser->m_CraftQuest;
	// ----
	for( int i = 0; i < this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemCount; i++ )
	{
		pSend.ItemCheck[i] = true;
		// ----
		if( !this->SearchQuestItem(lpUser, i) )
		{
			pSend.ItemCheck[i] = false;
		}
	}
	// ----
	DataSend(lpUser->m_Index, (LPBYTE)&pSend, pSend.h.size);
}
// -------------------------------------------------------------------------------

void CraftSystem::FinishQuestReq(LPOBJ lpUser)
{
	CRAFT_ANS_QUESTFINISH pSend;
	pSend.h.set((LPBYTE)&pSend, CraftProtocol::Case, CraftProtocol::QuestFinish, sizeof(pSend));
	pSend.Result = false;
	// ----
	if( !this->CheckAllReq(lpUser) )
	{
		GCServerMsgStringSend("You must finish all the requirements for finish quest", lpUser->m_Index, 1);
		DataSend(lpUser->m_Index, (LPBYTE)&pSend, pSend.h.size);
		return;
	}
	// ----
	for( int i = 0; i < this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ItemCount; i++ )
	{
		if( !this->DeleteQuestItem(lpUser, i) )
		{
			GCServerMsgStringSend("Please check quest items positions", lpUser->m_Index, 1);
			DataSend(lpUser->m_Index, (LPBYTE)&pSend, pSend.h.size);
			return;
		}
	}
	// ----
	lpUser->Money -= this->m_StageData[lpUser->m_CraftStage].QuestData[lpUser->m_CraftQuest].ReqMoney;
	GCMoneySend(lpUser->m_Index, lpUser->Money);
	// ----
	lpUser->m_CraftQuest++;
	// ----
	if( lpUser->m_CraftQuest >= this->m_StageData[lpUser->m_CraftStage].QuestCount )
	{
		pSend.Result = true;
		lpUser->m_CraftStage++;
		lpUser->m_CraftLevel++;
		lpUser->m_CraftQuest = 0;
	}
	// ----
	DataSend(lpUser->m_Index, (LPBYTE)&pSend, pSend.h.size);
	// ----
	PMSG_SERVERCMD ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType	= 0;
	ServerCmd.X			= lpUser->X;
	ServerCmd.Y			= lpUser->Y;
	MsgSendV2(lpUser, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	DataSend(lpUser->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	// ----
	this->SendCraftData(lpUser, false);
}
// -------------------------------------------------------------------------------

#endif
// -------------------------------------------------------------------------------