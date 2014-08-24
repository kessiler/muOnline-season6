//-> Decompiled by DarkSim | 23.01.2013 | 1.01.00 GS-N (All names are original)
// -------------------------------------------------------------------------------


#include "StdAfx.h"
#include "user.h"
#include "ItemSeedSphere.h"
// -------------------------------------------------------------------------------

//005a4750	-> 100%
CSeedItem::CSeedItem()
{
	this->Clear();
}
// -------------------------------------------------------------------------------

//005a4780	-> 100%
CSeedItem::~CSeedItem()
{
	// ----
}
// -------------------------------------------------------------------------------

//005a47a0	-> 100%
void CSeedItem::Clear()
{
	this->m_SeedIndex			= 0;
	this->m_SeedType			= 0;
	this->m_ItemLevel			= 0;
	this->m_SeedOptionValueType = 0;
	// ----
	for( int i = 0; i < MAX_SOCKET_SLOT; i++ )
	{
		this->m_SeedOptionValue[i] = 0;
	}
	// ----
	this->m_SeedExtractRate = 0;
	this->m_ItemCode		= 0;
}
// -------------------------------------------------------------------------------

//005a4820	-> 100%
bool CSeedItem::IsMatched(CItem * lpItem)
{
	if( lpItem )
	{
		if( lpItem->IsItem() )
		{
			return this->m_ItemCode == lpItem->m_Type && this->m_ItemLevel == lpItem->m_Level;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//005a4890	-> 100%
void CSeedItem::SetOption(BYTE SeedIndex, BYTE SeedType, BYTE ItemLevel, BYTE SeedOptionValueType, LPBYTE SeeedOptionValue, int ExtractRate)
{
	this->m_SeedIndex			= SeedIndex;
	this->m_SeedType			= SeedType;
	this->m_ItemLevel			= ItemLevel;
	this->m_SeedOptionValueType = SeedOptionValueType;
	// ----
	for( int i = 0; i < MAX_SOCKET_SLOT; ++i )
	{
		this->m_SeedOptionValue[i] = SeeedOptionValue[i];
	}
	// ----
	this->m_SeedExtractRate = ExtractRate;
	// ----
	switch( SeedType )
	{
	case 1:	//-> Fire
		{
			this->m_ItemCode = ITEMGET(12, 60);
		}
		break;
		// --
	case 2:	//-> Water
		{
			this->m_ItemCode = ITEMGET(12, 61);
		}
		break;
		// --
	case 3:	//-> Ice
		{
			this->m_ItemCode = ITEMGET(12, 62);
		}
		break;
		// --
	case 4:	//-> Wind
		{
			this->m_ItemCode = ITEMGET(12, 63);
		}
		break;
		// --
	case 5:	//-> Thunder
		{
			this->m_ItemCode = ITEMGET(12, 64);
		}
		break;
		// --
	case 6:	//-> Earth
		{
			this->m_ItemCode = ITEMGET(12, 65);
		}
		break;
		// --
	case 7:	//-> Unique
		{
			this->m_ItemCode = ITEMGET(12, 66);
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//005a49d0	-> 100%
CSphereItem::CSphereItem()
{
	// ----
}
// -------------------------------------------------------------------------------

//005a49f0	-> 100%
CSphereItem::~CSphereItem()
{
	// ----
}
// -------------------------------------------------------------------------------

//005a4a10	-> 100%
void CSphereItem::Clear()
{
	this->m_SphereLevel			= 0;
	this->m_ValueIncreaseRate	= 0;
	this->m_ItemCode			= 0;
}
// -------------------------------------------------------------------------------

//005a4a50	-> 100%
void CSphereItem::SetOption(BYTE SphereLevel, int ValueIncreaseRate)
{
	this->m_SphereLevel			= SphereLevel;
	this->m_ValueIncreaseRate	= ValueIncreaseRate;
	// ----
	switch( SphereLevel )
	{
	case 1:
		{
			this->m_ItemCode = ITEMGET(12, 70);
		}
		break;
		// --
	case 2:
		{
			this->m_ItemCode = ITEMGET(12, 71);
		}
		break;
		// --
	case 3:
		{
			this->m_ItemCode = ITEMGET(12, 72);
		}
		break;
		// --
	case 4:
		{
			this->m_ItemCode = ITEMGET(12, 73);
		}
		break;
		// --
	case 5:
		{
			this->m_ItemCode = ITEMGET(12, 74);
		}
		break;
	}
}
// -------------------------------------------------------------------------------

//005a4b10	-> 100%
CSeedSphereItem::CSeedSphereItem()
{
	this->Clear();
}
// -------------------------------------------------------------------------------

//005a4b40	-> 100%
CSeedSphereItem::~CSeedSphereItem()
{
	// ----
}
// -------------------------------------------------------------------------------

//005a4b60	-> 100%
void CSeedSphereItem::Clear()
{
	this->m_SeedSphereIndex		= 0;
	this->m_SeedIndex			= 0;
	this->m_SeedType			= 0;
	this->m_ItemLevel			= 0;
	this->m_SeedOptionValueType	= 0;
	this->m_SeedOptionValue		= 0;
	this->m_ItemCode			= 0;
}
// -------------------------------------------------------------------------------

//005a4bc0	-> 99% (Need check 1 line)
bool CSeedSphereItem::SetOption(CSeedItem * SeedItem, BYTE SphereLevel)
{
	if( !SeedItem || !SphereLevel )
	{
		return false;
	}
	// ----
	this->m_SeedSphereIndex		= SeedItem->m_SeedIndex + 50 * SphereLevel;
	this->m_SeedIndex			= SeedItem->m_SeedIndex;
	this->m_SeedType			= SeedItem->m_SeedType;
	this->m_ItemLevel			= SeedItem->m_ItemLevel;
	this->m_SeedOptionValueType = SeedItem->m_SeedOptionValueType;
	this->m_SphereLevel			= SphereLevel;
	this->m_SeedOptionValue		= *(&SeedItem->m_SeedOptionValueType + SphereLevel);	//-> Need check
	// ----
	switch( SeedItem->m_SeedType )
	{
	case 1:
		{
			this->m_ItemCode = ITEMGET(12, 100);
		}
		break;
		// --
	case 2:
		{
			this->m_ItemCode = ITEMGET(12, 101);
		}
		break;
		// --
	case 3:
		{
			this->m_ItemCode = ITEMGET(12, 102);
		}
		break;
		// --
	case 4:
		{
			this->m_ItemCode = ITEMGET(12, 103);
		}
		break;
		// --
	case 5:
		{
			this->m_ItemCode = ITEMGET(12, 104);
		}
		break;
		// --
	case 6:
		{
			this->m_ItemCode = ITEMGET(12, 105);
		}
		break;
		// --
	case 7:
		{
			// ----
		}
		break;
		// --
	default:
		{
			return false;
		}
		break;
	}
	// ----
	this->m_ItemCode += 6 * (this->m_SphereLevel - 1);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//005a4d60	-> 100%
void CSeedSphereItem::AddOptionValue(BYTE btOptionValue)
{
	if( btOptionValue + this->m_SeedOptionValue <= -2 )	//65534
	{
		this->m_SeedOptionValue += btOptionValue;
	}
}
// -------------------------------------------------------------------------------