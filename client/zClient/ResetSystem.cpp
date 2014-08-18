#include "stdafx.h"
#include "ResetSystem.h"
#include "ScriptEncode.h"
#include "User.h"
#include "Protocol.h"
#include "Import.h"
#include "TDebugLog.h"
#include "Interface.h"
// -------------------------------------------------------------------------------

ResetSystem		gResetSystem;
// -------------------------------------------------------------------------------

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
}
// -------------------------------------------------------------------------

void ResetSystem::Load()
{
	this->Init();
	this->ReadGroupData("Data\\Custom\\ResetGroup.z");
}
// -------------------------------------------------------------------------

void ResetSystem::ReadGroupData(char * File)
{
	int Token;
	// ----
	if( !g_ScriptEncode.ReadScript(File) )
	{
		MessageBox(NULL, "file not found", "[ResetSystem]", ERROR);
		return;
	}
	// ----
	while(true)
	{
		Token = g_ScriptEncode.GetToken();
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
		int GroupID = g_ScriptEncode.GetNumber();
		// ----
		g_ScriptEncode.GetToken();
		this->m_GroupData[GroupID].MinReset		= g_ScriptEncode.GetNumber();
		// ----
		g_ScriptEncode.GetToken();
		this->m_GroupData[GroupID].MaxReset		= g_ScriptEncode.GetNumber();
		// ----
		g_ScriptEncode.GetToken();
		this->m_GroupData[GroupID].ReqLevel		= g_ScriptEncode.GetNumber();
		// ----
		g_ScriptEncode.GetToken();
		this->m_GroupData[GroupID].ReqMoney		= g_ScriptEncode.GetNumber();
		// ----
		g_ScriptEncode.GetToken();
		this->m_GroupData[GroupID].ItemCount	= g_ScriptEncode.GetNumber();
		// ----
		int RewardPerGroup	= 0;
		// ----
		while(true)
		{
			if( RewardPerGroup >= 7 )
			{
				break;
			}
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].LevelPoint = g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].GensPoint = g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].WCoinC = g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].WCoinP = g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].RewardData[RewardPerGroup].GoblinPoint = g_ScriptEncode.GetNumber();
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
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].ID			= g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].MinLevel		= g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].MaxLevel		= g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].MinOption		= g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].MaxOption		= g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsLuck		= g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsSkill		= g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsExcellent	= g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsAncient		= g_ScriptEncode.GetNumber();
			// ----
			g_ScriptEncode.GetToken();
			this->m_GroupData[GroupID].ItemData[ItemPerGroup].IsSocket		= g_ScriptEncode.GetNumber();
			// ----
			ItemPerGroup++;
		}
		// ----
		this->m_GroupLoadedCount++;
	}
}
// -------------------------------------------------------------------------

int ResetSystem::GetResetMoney()
{
	int Money = this->m_GroupData[this->GetResetGroup()].ReqMoney * (gObjUser.m_Reset + 1);
	// ----
	if( Money > MAX_RESET_MONEY )
	{
		Money = MAX_RESET_MONEY;
	}
	// ----
	return Money;
}
// -------------------------------------------------------------------------

int ResetSystem::GetResetGroup()
{
	for( int Group = 0; Group < this->m_GroupLoadedCount; Group++ )
	{
		if( gObjUser.m_Reset >= this->m_GroupData[Group].MinReset 
			&& gObjUser.m_Reset < this->m_GroupData[Group].MaxReset )
		{
			return Group;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

bool ResetSystem::CheckAllReq()
{
	if( gObjUser.lpPlayer->MoneyInventory 
		< this->m_GroupData[this->GetResetGroup()].ReqMoney )
	{
		return false;
	}
	// ----
	if( gObjUser.lpPlayer->Level < this->m_GroupData[this->GetResetGroup()].ReqLevel )
	{
		return false;
	}
	// ----
	for( int i = 0; i < this->m_GroupData[this->GetResetGroup()].ItemCount; i++ )
	{
		if( !gObjUser.m_ResetItemCheck[i] )
		{
			return false;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------

void ResetSystem::SetData(RESET_ANS_USERDATA * aRecv)
{
	ZeroMemory(gObjUser.m_ResetItemCheck, sizeof(gObjUser.m_ResetItemCheck));
	// ----
	gObjUser.m_Reset = aRecv->Reset;
	gObjUser.m_GrandReset = aRecv->GrandReset;
	// ----
	TDEBUG("GR: %d", gObjUser.m_GrandReset);
	// ----
	for( int i = 0; i < 5; i++ )
	{
		gObjUser.m_ResetItemCheck[i] = aRecv->ItemCheck[i];
	}
	// ----
	if( aRecv->ByDialog )
	{
		gInterface.OpenResetWindow();
	}
}
// -------------------------------------------------------------------------

void ResetSystem::ReqResetFinish()
{
	RESET_REQ_USERDATA pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 6, sizeof(pRequest));
	gProtocol.DataSend((LPBYTE)&pRequest, pRequest.h.size);
}
// -------------------------------------------------------------------------

void ResetSystem::GetResetResult()
{
	gInterface.CloseResetWindow();
}
// -------------------------------------------------------------------------