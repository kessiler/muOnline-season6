#include "StdAfx.h"
#include "ItemPrice.h"
#include "logproc.h"
#include "..\include\ReadScript.h"
#include "..\common\winutil.h"
#include "GameMain.h"
// -------------------------------------------------------------------------------

#ifdef __CUSTOMS__
// -------------------------------------------------------------------------------

ItemPrice	g_ItemPrice;
// -------------------------------------------------------------------------------

ItemPrice::ItemPrice()
{
	this->Init();
}
// -------------------------------------------------------------------------------

ItemPrice::~ItemPrice()
{
	// ----
}
// -------------------------------------------------------------------------------

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
// -------------------------------------------------------------------------------

void ItemPrice::ReadList(char * File)
{
	int Token;
	this->Init();
	// ----
	SMDFile = fopen(File, "r");
	// ----
	if( SMDFile == 0 )
	{
		MsgBox(lMsg.Get(MSGGET(0, 112)), File);
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
		else
		{
			this->m_Data[this->m_LoadedCount].ItemID	= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].ItemIndex	= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].ItemLevel	= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].Price		= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].Divisor	= TokenNumber;
			// ----
			this->m_LoadedCount++;
		}
	}
	// ----
	LogAddTD("[ItemPrice] [%d] Items loaded from list", this->m_LoadedCount);
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

void ItemPrice::Load()
{
	this->ReadList(gDirPath.GetNewPath("Custom\\ItemPrice.txt"));
}
// -------------------------------------------------------------------------------

bool ItemPrice::CheckItem(short ItemCode, BYTE ItemLevel, __int64 * Price, int * Divisor)
{
	for( int i = 0; i < this->m_LoadedCount; i++ )
	{
		if( ItemCode != ITEMGET(this->m_Data[i].ItemID, this->m_Data[i].ItemIndex) )
		{
			continue;
		}
		// ----
		if( this->m_Data[i].ItemLevel != -1 && ItemLevel != this->m_Data[i].ItemLevel )
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
// -------------------------------------------------------------------------------

bool ItemPrice::CheckItem(short ItemCode, BYTE ItemLevel)
{
	for( int i = 0; i < this->m_LoadedCount; i++ )
	{
		if( ItemCode != ITEMGET(this->m_Data[i].ItemID, this->m_Data[i].ItemIndex) )
		{
			continue;
		}
		// ----
		if( this->m_Data[i].ItemLevel != -1 && ItemLevel != this->m_Data[i].ItemLevel )
		{
			continue;
		}
		// ----
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

#endif
// -------------------------------------------------------------------------------