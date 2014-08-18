#include "stdafx.h"
#include "ItemPrice.h"
#include "ScriptEncode.h"
#include "Object.h"
// -------------------------------------------------------------------------------------------------------------

ItemPrice	gItemPrice;
// ----------------------------------------------------------------------------------------------

ItemPrice::ItemPrice()
{
	this->Init();
}
// ----------------------------------------------------------------------------------------------

ItemPrice::~ItemPrice()
{
	// ----
}
// ----------------------------------------------------------------------------------------------

void ItemPrice::Init()
{
	for( int i = 0; i < MAX_ITEMPRICE_ITEM; i++ )
	{
		this->m_Data[i].ItemID		= -1;
		this->m_Data[i].ItemIndex	= -1;
		this->m_Data[i].ItemLevel	= -1;
		this->m_Data[i].Price		= -1;
		this->m_Data[i].Divisor		= -1;
	}
	// ----
	this->m_LoadedCount = 0;
}
// ----------------------------------------------------------------------------------------------

void ItemPrice::ReadList(char * File)
{
	this->Init();
	int Token;
	// ----
	if( !g_ScriptEncode.ReadScript(File) )
	{
		MessageBox(NULL, "file not found", "[ItemPrice]", ERROR);
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
		else
		{
			this->m_Data[this->m_LoadedCount].ItemID	= g_ScriptEncode.GetNumber();
			g_ScriptEncode.GetToken();
			this->m_Data[this->m_LoadedCount].ItemIndex	= g_ScriptEncode.GetNumber();
			g_ScriptEncode.GetToken();
			this->m_Data[this->m_LoadedCount].ItemLevel	= g_ScriptEncode.GetNumber();
			g_ScriptEncode.GetToken();
			this->m_Data[this->m_LoadedCount].Price		= g_ScriptEncode.GetNumber();
			g_ScriptEncode.GetToken();
			this->m_Data[this->m_LoadedCount].Divisor	= g_ScriptEncode.GetNumber();
			// ----
			this->m_LoadedCount++;
		}
	}
}
// ----------------------------------------------------------------------------------------------

void ItemPrice::Load()
{
	this->ReadList("Data\\Custom\\ItemPrice.z");
}
// ----------------------------------------------------------------------------------------------

bool ItemPrice::CheckItem(short ItemCode, BYTE ItemLevel, __int64 * Price, int * Divisor)
{
	for( int i = 0; i < this->m_LoadedCount; i++ )
	{
		if( ItemCode != ITEM(this->m_Data[i].ItemID, this->m_Data[i].ItemIndex) )
		{
			continue;
		}
		// ----
		if( ItemLevel != this->m_Data[i].ItemLevel )
		{
			continue;
		}
		// ----
		if( this->m_Data[i].Price == -1 || this->m_Data[i].Divisor == -1 )
		{
			continue;
		}
		// ----
		*Price		= this->m_Data[i].Price;
		*Divisor	= this->m_Data[i].Divisor;
		return true;
	}
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------