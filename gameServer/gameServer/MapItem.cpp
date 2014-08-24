//	GS-N	1.00.77	JPN		-	Completed
//	GS-CS	1.00.90	JPN		-	Completed
#include "stdafx.h"
//#include "MapItem.h"
#include "GameMain.h"

CMapItem::CMapItem()
{
	this->Init();
}

CMapItem::~CMapItem()
{
	return;

}

void CMapItem::Init()
{
	this->VPCount = 0;
	this->m_State = 0;

	for ( int n=0;n<MAX_VIEWPORT;n++ )
	{
		memset(&this->VpPlayer[n], 0, sizeof(VIEWPORT_PLAYER_STRUCT));
	}
}

void CMapItem::CreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, BYTE ItemEffectEx,BYTE SocketOption[5], BYTE SocketIndex) //identical gs-cs 56
{
	this->Init();

	this->m_Level = level;
	this->m_Durability = dur;
	this->Convert(type, Option1, Option2, Option3, NOption, SOption, ItemEffectEx, SocketOption, SocketIndex, 0, CURRENT_DB_VERSION);
	this->px = x;
	this->py= y;
	this->live = true;
	this->Give = false;
	this->m_State = 1;

	if ( this->m_QuestItem != false )
	{
		this->m_Time = GetTickCount() + 60000;
		this->m_LootTime = GetTickCount() + 600000;
		this->m_Number = number;
	}
	else
	{
		this->m_Time = GetTickCount() + gItemsDurationTime;
		this->m_LootTime = GetTickCount() + gLootingTime*1000;
		this->m_Number = number;
	}

}
#include "logproc.h"

void CMapItem::DropCreateItem(int type, int level, int x, int y, float dur, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int PetLevel, int PetExp, BYTE ItemEffectEx,BYTE SocketOption[5], BYTE SocketIndex) //identical gs-cs 56
{
	this->Init();

	this->m_Level = level;
	this->m_Durability = dur;
	
	this->Convert(type, Option1, Option2, Option3, NOption, SOption, ItemEffectEx, SocketOption, SocketIndex, 0, CURRENT_DB_VERSION);

	this->SetPetItemInfo(PetLevel, PetExp);
	this->px = x;
	this->py= y;
	this->live = true;
	this->Give = false;
	this->m_State = 1;

	if ( this->m_QuestItem != false )
	{
		if ( type == ITEMGET(14,11) )
		{
			this->m_Time = GetTickCount() + 1000;
		}
		else
		{
			this->m_Time = GetTickCount() + 60000;
		}

		this->m_LootTime = GetTickCount() + 600000;
		this->m_Number = number;
	}
	else 
	{
		if ( type == ITEMGET(14,11) )
		{
			this->m_Time = GetTickCount() + 1000;
		}
		else
		{
			this->m_Time = GetTickCount() + gItemsDurationTime;
		}

		this->m_LootTime = GetTickCount() + gLootingTime*1000;
		this->m_Number = number;
	}

	if ( type == ITEMGET(13,19) )
	{
		if ( ( level  < 0)?FALSE:(level>2)?FALSE:TRUE)
		{
			this->m_Time = GetTickCount() + 900000;
			this->m_LootTime = GetTickCount();
		}
	}

	if ( type == ITEMGET(14,64) )
	{
		this->m_Time = GetTickCount() + 20000;
	}
}