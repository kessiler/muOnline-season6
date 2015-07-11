#include "StdAfx.h"
#include "ShopPointEx.h"
#include "user.h"
#include "GameMain.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "GameShop.h"
// -------------------------------------------------------------------------------

ShopPointEx g_ShopPointEx;
// -------------------------------------------------------------------------------

ShopPointEx::ShopPointEx()
{
	this->Init();
}
// -------------------------------------------------------------------------------

ShopPointEx::~ShopPointEx()
{
	// ----
}
// -------------------------------------------------------------------------------

void ShopPointEx::Init()
{
	this->m_IsLoaded			= false;
	this->m_TimeBonusOn			= false;
	this->m_TimeBonusAdd		= 0;
	this->m_TimeBonusInterval	= 0;
	this->m_MonsterLoaded		= 0;
	// ----
	for( int i = 0; i < MAX_SHOPPOINTEX_EVENT; i++ )
	{
		this->m_EventBonus[i].AddCoinC	= 0;
		this->m_EventBonus[i].AddCoinP	= 0;
		this->m_EventBonus[i].AddCoinG	= 0;
	}
	// ----
	for( int i = 0; i < MAX_SHOPPOINTEX_MONSTER; i++ )
	{
		this->m_MonsterBonus[i].MonsterID	= -1;
		this->m_MonsterBonus[i].MapNumber	= -1;
		this->m_MonsterBonus[i].AddCoinC	= 0;
		this->m_MonsterBonus[i].AddCoinP	= 0;
		this->m_MonsterBonus[i].AddCoinG	= 0;
	}
}
// -------------------------------------------------------------------------------

void ShopPointEx::Load()
{
	this->Init();
	this->ReadData(gDirPath.GetNewPath("Custom\\ShopPointEx.txt"));
}
// -------------------------------------------------------------------------------

void ShopPointEx::ReadData(char * File)
{
	int Category;
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[ShopPointEx] %s file not found", File);
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
		Category = TokenNumber;
		// ----
		while(true)
		{
			if( Category == 0 )			//-> Time Bonus
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				this->m_TimeBonusOn = TokenNumber;
				// ----
				Token = GetToken();
				this->m_TimeBonusAdd = TokenNumber;
				// ----
				Token = GetToken();
				this->m_TimeBonusInterval = TokenNumber;

			}
			else if( Category == 1 )	//-> Event Bonus
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				int Slot = TokenNumber;
				// ----
				Token = GetToken();
				this->m_EventBonus[Slot].AddCoinC = TokenNumber;
				// ----
				Token = GetToken();
				this->m_EventBonus[Slot].AddCoinP = TokenNumber;
				// ----
				Token = GetToken();
				this->m_EventBonus[Slot].AddCoinG = TokenNumber;
			}
			else if( Category == 2 )	//-> Monster Bonus
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				int Slot = this->m_MonsterLoaded;
				// ----
				this->m_MonsterBonus[Slot].MonsterID = TokenNumber;
				// ----
				Token = GetToken();
				this->m_MonsterBonus[Slot].MapNumber = TokenNumber;
				// ----
				Token = GetToken();
				this->m_MonsterBonus[Slot].AddCoinC = TokenNumber;
				// ----
				Token = GetToken();
				this->m_MonsterBonus[Slot].AddCoinP = TokenNumber;
				// ----
				Token = GetToken();
				this->m_MonsterBonus[Slot].AddCoinG = TokenNumber;
				// ----
				this->m_MonsterLoaded++;
			}
		}
	}
	// ----
	this->m_IsLoaded = true;
	LogAddTD("[ShopPointEx] [%s] File is loaded (Monsters: %d)", File, this->m_MonsterLoaded);
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

int ShopPointEx::GetMonsterSlot(int Class, int Map)
{
	for( int i = 0; i < this->m_MonsterLoaded; i++ )
	{
		if( this->m_MonsterBonus[i].MonsterID != Class )
		{
			continue;
		}
		// ----
		if( this->m_MonsterBonus[i].MapNumber != Map && this->m_MonsterBonus[i].MapNumber != -1 )
		{
			continue;
		}
		// ----
		return i;
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

void ShopPointEx::AddTimeBonus(int UserIndex)
{
	if( !this->m_IsLoaded || !this->m_TimeBonusOn )
	{
		return;
	}
	// ----
	if( gObj[UserIndex].m_ShopPointExTime % this->m_TimeBonusInterval != 0 )
	{
		return;
	}
	// ----
	gObj[UserIndex].m_ShopPointExTime = 0;
	gObj[UserIndex].GameShop.GoblinPoint += this->m_TimeBonusAdd;
	this->SendNotify(UserIndex, 0, 0, this->m_TimeBonusAdd);
}
// -------------------------------------------------------------------------------

void ShopPointEx::AddEventBonus(int UserIndex, ShopPointExEvent::T Event)
{
	if( !this->m_IsLoaded )
	{
		return;
	}
	// ----
	gObj[UserIndex].GameShop.WCoinC			+= this->m_EventBonus[Event].AddCoinC;
	gObj[UserIndex].GameShop.WCoinP			+= this->m_EventBonus[Event].AddCoinP;
	gObj[UserIndex].GameShop.GoblinPoint	+= this->m_EventBonus[Event].AddCoinG;
	// ----
	this->SendNotify(UserIndex,
		this->m_EventBonus[Event].AddCoinC,
		this->m_EventBonus[Event].AddCoinP,
		this->m_EventBonus[Event].AddCoinG);
}
// -------------------------------------------------------------------------------

void ShopPointEx::AddMonsterBonus(int UserIndex, int MonsterIndex)
{
	if( !this->m_IsLoaded )
	{
		return;
	}
	// ----
	int MonsterSlot = this->GetMonsterSlot(gObj[MonsterIndex].Class, gObj[MonsterIndex].MapNumber);
	// ----
	if( MonsterSlot == -1 )
	{
		return;
	}
	// ----
	gObj[UserIndex].GameShop.WCoinC			+= this->m_MonsterBonus[MonsterSlot].AddCoinC;
	gObj[UserIndex].GameShop.WCoinP			+= this->m_MonsterBonus[MonsterSlot].AddCoinP;
	gObj[UserIndex].GameShop.GoblinPoint	+= this->m_MonsterBonus[MonsterSlot].AddCoinG;
	// ----
	this->SendNotify(UserIndex, 
		this->m_MonsterBonus[MonsterSlot].AddCoinC,
		this->m_MonsterBonus[MonsterSlot].AddCoinP,
		this->m_MonsterBonus[MonsterSlot].AddCoinG);
}
// -------------------------------------------------------------------------------

void ShopPointEx::SendNotify(int UserIndex, int CoinC, int CoinP, int CoinG)
{
	gGameShop.GDSavePoint(UserIndex);
	// ----
	char Text[256];
	// ----
	if( CoinC > 0 )
	{
		ZeroMemory(Text, 255);
		wsprintf(Text, "Obtained %d WCoinC", CoinC);
		GCServerMsgStringSend(Text, UserIndex, 1);
	}
	// ----
	if( CoinP > 0 )
	{
		ZeroMemory(Text, 255);
		wsprintf(Text, "Obtained %d WCoinP", CoinP);
		GCServerMsgStringSend(Text, UserIndex, 1);
	}
	// ----
	if( CoinG > 0 )
	{
		ZeroMemory(Text, 255);
		wsprintf(Text, "Obtained %d GoblinPoint", CoinG);
		GCServerMsgStringSend(Text, UserIndex, 1);
	}
}
// -------------------------------------------------------------------------------