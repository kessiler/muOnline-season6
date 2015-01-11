#include "stdafx.h"
#include "ItemRank.h"
#include "ScriptEncode.h"
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
	this->Read("Data\\Custom\\ItemRank.z");
}
// -------------------------------------------------------------------------------

void ItemRank::Read(char * File)
{
	if( !g_ScriptEncode.ReadScript(File) )
	{
		MessageBox(NULL, "file not found", "[ItemRank]", ERROR);
		return;
	}
	while(true)
	{
		if( g_ScriptEncode.GetToken() == END )
		{
			break;
		}
		// ----
		int Type = (int)g_ScriptEncode.GetNumber();
		// ----
		if( Type == 0 ) //RankList
		{
			while(true)
			{
				g_ScriptEncode.GetToken();
				// ----
				if( strcmp("end", g_ScriptEncode.GetString()) == 0 )
				{
					break;
				}
				// ----
				ItemRank_Data lpData;
				// ----
				lpData.RankID = g_ScriptEncode.GetNumber();
				// ----
				g_ScriptEncode.GetToken();
				lpData.OptionValue[0] = g_ScriptEncode.GetNumber();
				// ----
				g_ScriptEncode.GetToken();
				lpData.OptionValue[1] = g_ScriptEncode.GetNumber();
				// ----
				g_ScriptEncode.GetToken();
				lpData.OptionValue[2] = g_ScriptEncode.GetNumber();
				// ----
				g_ScriptEncode.GetToken();
				lpData.OptionValue[3] = g_ScriptEncode.GetNumber();
				// ----
				g_ScriptEncode.GetToken();
				lpData.OptionValue[4] = g_ScriptEncode.GetNumber();
				// ----
				g_ScriptEncode.GetToken();
				lpData.OptionValue[5] = g_ScriptEncode.GetNumber();
				// ----
				this->m_Data.push_back(lpData);
			}
		}
		else if( Type == 1 ) //ItemList
		{
			while(true)
			{
				g_ScriptEncode.GetToken();
				// ----
				if( strcmp("end", g_ScriptEncode.GetString()) == 0 )
				{
					break;
				}
				// ----
				ItemRank_List lpData;
				WORD ItemType = -1, ItemIndex = -1;
				// ----
				lpData.RankID = g_ScriptEncode.GetNumber();
				// ----
				g_ScriptEncode.GetToken();
				ItemType = g_ScriptEncode.GetNumber();
				// ----
				g_ScriptEncode.GetToken();
				ItemIndex = g_ScriptEncode.GetNumber();
				// ----
				lpData.ItemType = ITEM(ItemType, ItemIndex);
				// ----
				short Index = this->GetDataIndex(lpData.RankID);
				// ----
				if( Index == -1 )
				{
					continue;
				}
				// ----
				this->m_Data[Index].ItemList.push_back(lpData);
			}
		}
	}
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