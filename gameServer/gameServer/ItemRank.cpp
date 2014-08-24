#include "StdAfx.h"
#include "ItemRank.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "GameMain.h"
// -------------------------------------------------------------------------------

ItemRank g_ItemRank;
// -------------------------------------------------------------------------------

ItemRank::ItemRank()
{
	this->Init();
}
// -------------------------------------------------------------------------------

ItemRank::~ItemRank()
{

}
// -------------------------------------------------------------------------------

void ItemRank::Init()
{
	this->m_Data.clear();
}
// -------------------------------------------------------------------------------

void ItemRank::Load()
{
	this->Init();
	this->Read(gDirPath.GetNewPath("Custom\\ItemRank.dat"));
}
// -------------------------------------------------------------------------------

void ItemRank::Read(char * File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[ItemRank] %s file not found", File);
		return;
	}
	// ----
	while(true)
	{
		SMDToken Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		int Type = (int)TokenNumber;
		// ----
		if( Type == 0 ) //RankList
		{
			while(true)
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				ItemRank_Data lpData;
				// ----
				lpData.RankID = TokenNumber;
				// ----
				Token = GetToken();
				lpData.OptionValue[0] = TokenNumber;
				// ----
				Token = GetToken();
				lpData.OptionValue[1] = TokenNumber;
				// ----
				Token = GetToken();
				lpData.OptionValue[2] = TokenNumber;
				// ----
				Token = GetToken();
				lpData.OptionValue[3] = TokenNumber;
				// ----
				Token = GetToken();
				lpData.OptionValue[4] = TokenNumber;
				// ----
				Token = GetToken();
				lpData.OptionValue[5] = TokenNumber;
				// ----
				this->m_Data.push_back(lpData);
			}
		}
		else if( Type == 1 ) //ItemList
		{
			while(true)
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				ItemRank_List lpData;
				WORD ItemType = -1, ItemIndex = -1;
				// ----
				lpData.RankID = TokenNumber;
				// ----
				Token = GetToken();
				ItemType = TokenNumber;
				// ----
				Token = GetToken();
				ItemIndex = TokenNumber;
				// ----
				lpData.ItemType = ITEMGET(ItemType, ItemIndex);
				// ----
				short Index = this->GetDataIndex(lpData.RankID);
				// ----
				if( Index == -1 )
				{
					MsgBox("[ItemRank] Invalid RankID (Item: %d/%d - RankID: %d)", 
						ItemType, ItemIndex, lpData.RankID);
				}
				// ----
				this->m_Data[Index].ItemList.push_back(lpData);
			}
		}
	}
	// ----
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

bool ItemRank::IsRankItem(WORD ItemType)
{
	for( int i = 0; i < this->m_Data.size(); i++ )
	{
		for( int j = 0; j < this->m_Data[i].ItemList.size(); j++ )
		{
			if( this->m_Data[i].ItemList[j].ItemType == ItemType )
			{
				return true;
			}
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

short ItemRank::GetValue(WORD ItemType, BYTE OptionType)
{
	for( int i = 0; i < this->m_Data.size(); i++ )
	{
		for( int j = 0; j < this->m_Data[i].ItemList.size(); j++ )
		{
			if( this->m_Data[i].ItemList[j].ItemType == ItemType )
			{
				return this->m_Data[i].OptionValue[OptionType];
			}
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

short ItemRank::GetDataIndex(BYTE RankID)
{
	for( int i = 0; i < this->m_Data.size(); i++ )
	{
		if( this->m_Data[i].RankID == RankID )
		{
			return i;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------