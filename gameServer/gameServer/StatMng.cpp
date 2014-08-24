#include "stdafx.h"
#include "StatMng.h"
#include "user.h"
// -------------------------------------------------------------------------------

CStatMng	gStatMng;
// -------------------------------------------------------------------------------

//00532b70	-> 100%
CStatMng::CStatMng()
{
	this->Init();
}
// -------------------------------------------------------------------------------

//00532bf0	-> 100%
CStatMng::~CStatMng()
{
	// ----
}
// -------------------------------------------------------------------------------

//00532c20	-> 100%
void CStatMng::Init()
{
	int firstState			= 2;
	int MagunfirstState		= 2;
	int iDarkLordFirstState = 2;
	// ----
	for( int i = 0; i < STAT_TABLE_SIZE; i++ )
	{
		if( !((i+1) % 10) )
		{
			firstState			+= 3 * (i + 11) / 400 + 2;
			MagunfirstState		+= 3 * (i + 11) / 700 + 2;
			iDarkLordFirstState += 3 * (i + 11) / 500 + 2;
		}
		// ----
		this->gObjNormalAddStatTable[i]		= firstState;
		this->gObjMagumAddStatTable[i]		= MagunfirstState;
		this->gObjDarkLordAddStatTable[i]	= iDarkLordFirstState;
	}
	// ----
	firstState			= 2;
	MagunfirstState		= 2;
	iDarkLordFirstState = 2;
	// ----
	for( int i=0; i < STAT_TABLE_SIZE; i++ )
	{
		if( !((i+1) % 10) )
		{
			firstState			+= 3 * (i + 11) / 400 + 2;
			MagunfirstState		+= 3 * (i + 11) / 700 + 2;
			iDarkLordFirstState += 3 * (i + 11) / 500 + 2;
		}
		// ----
		this->gObjNormalMinusStatTable[i]	= firstState;
		this->gObjMagumMinusStatTable[i]	= MagunfirstState;
		this->gObjDarkLordMinusStatTable[i] = iDarkLordFirstState;
	}
}
// -------------------------------------------------------------------------------

//00532e40	-> 100%
int CStatMng::GetMaxStat(int level, int Class)
{
	if( level < 1 || level > MAX_CHAR_LEVEL )
	{
		return false;
	}
	// ----
	if( Class == CLASS_MAGUMSA )
	{
		return this->gObjMagumAddStatTable[level-1];
	}
	// ----
	if( Class == CLASS_DARKLORD )
	{
		return this->gObjDarkLordAddStatTable[level-1];
	}
	// ----
	return this->gObjNormalAddStatTable[level-1];
}
// -------------------------------------------------------------------------------

//00532ec0	-> 100%
int CStatMng::GetMaxMinusStat(int iLevel, int iClass)
{
	if( iLevel < 1 || iLevel > MAX_CHAR_LEVEL )
	{
		return false;
	}
	// ----
	if( iClass == CLASS_MAGUMSA)
	{
		return this->gObjMagumMinusStatTable[iLevel-1];
	}
	// ----
	if( iClass == CLASS_DARKLORD )
	{
		return this->gObjDarkLordMinusStatTable[iLevel-1];
	}
	// ----
	return this->gObjNormalMinusStatTable[iLevel-1];
}
// -------------------------------------------------------------------------------