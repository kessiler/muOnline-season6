#include "stdafx.h"
#include "CraftSystem.h"
#include "ReadScript.h"
#include "User.h"
#include "Protocol.h"
#include "Import.h"
#include "Interface.h"
// -------------------------------------------------------------------------------

CraftSystem gCraftSystem;
// -------------------------------------------------------------------------------

CraftSystem::CraftSystem()
{
	ZeroMemory(this->m_StageData, sizeof(m_StageData));
}
// -------------------------------------------------------------------------------

CraftSystem::~CraftSystem()
{

}
// -------------------------------------------------------------------------------

void CraftSystem::Load()
{
	this->ReadStageData("Data\\Custom\\CraftStage.txt");
}
// -------------------------------------------------------------------------------

void CraftSystem::ReadStageData(char * File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MessageBox(NULL, "file not found", "[CraftSystem]", ERROR);
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
	}
	// ----
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

bool CraftSystem::IsNotCrafter()
{
	if(		gObjUser.m_CraftLevel == 0
		&&	gObjUser.m_CraftStage == 0
		&&	gObjUser.m_CraftQuest == 0 )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

BYTE CraftSystem::CheckStageReq()
{
	if( gObjUser.lpPlayer->Level < this->m_StageData[gObjUser.m_CraftStage].ReqLevel )
	{
		return 1;
	}
	// ----
	if( gObjUser.m_Reset < this->m_StageData[gObjUser.m_CraftStage].ReqReset )
	{
		return 2;
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

bool CraftSystem::CheckQuestReq()
{
	if( gObjUser.lpPlayer->MoneyInventory 
		< this->m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ReqMoney )
	{
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

bool CraftSystem::CheckAllReq()
{
	if( !this->CheckQuestReq() || CheckStageReq() )
	{
		return false;
	}
	// ----
	for( int i = 0; i < this->m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemCount; i++ )
	{
		if( !gObjUser.m_CraftItemCheck[i] )
		{
			return false;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

void CraftSystem::SetData(CRAFT_ANS_USERDATA * aRecv)
{
	ZeroMemory(gObjUser.m_CraftItemCheck, sizeof(gObjUser.m_CraftItemCheck));
	// ----
	gObjUser.m_CraftLevel	= aRecv->CraftLevel;
	gObjUser.m_CraftStage	= aRecv->CraftStage;
	gObjUser.m_CraftQuest	= aRecv->CraftQuest;
	// ----
	for( int i = 0; i < 5; i++ )
	{
		gObjUser.m_CraftItemCheck[i] = aRecv->ItemCheck[i];
	}
	// ----
	if( aRecv->ByDialog )
	{
		gInterface.OpenCraftWindow();
	}
}
// ----------------------------------------------------------------------------------------------

void CraftSystem::ReqQuestFinish()
{
	CRAFT_REQ_QUESTFINISH pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 1, sizeof(pRequest));
	gProtocol.DataSend((LPBYTE)&pRequest, pRequest.h.size);
}
// ----------------------------------------------------------------------------------------------

void CraftSystem::GetQuestResult(CRAFT_ANS_QUESTFINISH * aRecv)
{
	gInterface.CloseCraftWindow();
	// ----
	if( aRecv->Result == true )
	{
		pPlayEffectAnimation(105, oUserPreviewStruct);		
	}
}
// ----------------------------------------------------------------------------------------------