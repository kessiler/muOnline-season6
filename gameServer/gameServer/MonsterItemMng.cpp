//-> Decompiled by DarkSim | 09.07.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#include "stdafx.h"
#include "MonsterItemMng.h"
#include "MonsterAttr.h"
#include "GameMain.h"
#include "LogProc.h"
#include "CashShop.h"
#include "LuckyItem.h"
#include "..\include\ReadScript.h"
#include "LargeRand.h"
// -------------------------------------------------------------------------------

CMonsterItemMng g_MonsterItemMng;
// -------------------------------------------------------------------------------

//0042ed00	-> 100% (Identical)
CMonsterItemMng::CMonsterItemMng()
{
	return;
}
// -------------------------------------------------------------------------------

//0042ee30	-> 100% (Identical)
CMonsterItemMng::~CMonsterItemMng()
{
	this->Clear();
}
// -------------------------------------------------------------------------------

//0042ee60	-> 100% (Identical)
void CMonsterItemMng::Init()
{
	memset(this->m_iMonsterInvenItemCount, 0, sizeof(this->m_iMonsterInvenItemCount));
	memset(this->m_iMonsterInvenExItemCount, 0, sizeof(this->m_iMonsterInvenItemCount));
	// ----
	for( int i = 0; i < MAX_MONSTER_LEVEL; i++ )
	{
		if( this->m_MonsterInvenItems[i] != NULL )	//wz mistake, fixed
		{
			delete[] this->m_MonsterInvenItems[i];
		}
		// ----
		this->m_MonsterInvenItems[i] = new CItem[MAX_MONSTER_ITEM];
		// ----
		if( this->m_MonsterInvenExItems[i] != NULL )	//wz mistake, fixed
		{
			delete[] this->m_MonsterInvenExItems[i];
		}
		// ----
		this->m_MonsterInvenExItems[i] = new CItem[MAX_MONSTER_EXITEM];	
	}
	// ----
	LPMONSTER_ATTRIBUTE lpm;
	// ----
	for( int i = 0; i < MAX_MONSTER_TYPE; i++ )
	{
		lpm = gMAttr.GetAttr(i);
		// ----
		if( lpm != NULL )
		{
			strcpy( this->MonsterName, lpm->m_Name);
			// ----
			if( lpm->m_Level != 0 )
			{
				this->gObjGiveItemSearchEx(lpm->m_Level, lpm->m_MaxItemLevel);
			}
		}
	}
	// ----
	this->LoadMonsterItemDropRate();
	this->MakeJewelItem();
}
// -------------------------------------------------------------------------------

//0042f090	-> 100% (Identical)
void CMonsterItemMng::Clear()
{
	memset(this->m_iMonsterInvenItemCount, 0, sizeof(this->m_iMonsterInvenItemCount));
	// ---
	for( int i = 0; i < MAX_MONSTER_LEVEL; i++ )
	{
		delete this->m_MonsterInvenItems[i];
	}
	// ----
	memset(this->m_iMonsterInvenExItemCount, 0, sizeof(this->m_iMonsterInvenItemCount));
	// ----
	for( int cnt = 0; cnt < MAX_MONSTER_LEVEL; cnt++ )
	{
		delete this->m_MonsterInvenExItems[cnt];
	}
	// ----
	if( this->m_MonsterJewelItem1 != NULL )
	{
		delete this->m_MonsterJewelItem1;
		this->m_MonsterJewelItem1 = NULL;
	}
	// ----
	if( this->m_MonsterJewelItem2 != NULL )
	{
		delete this->m_MonsterJewelItem2;
		this->m_MonsterJewelItem2 = NULL;
	}
	// ----
	if( this->m_MonsterJewelItem3 != NULL )
	{
		delete this->m_MonsterJewelItem3;
		this->m_MonsterJewelItem3 = NULL;
	}
	// ----
	if( this->m_MonsterJewelItem4 != NULL )
	{
		delete this->m_MonsterJewelItem4;
		this->m_MonsterJewelItem4 = NULL;
	}
	// ----
	if( this->m_MonsterJewelItem5 != NULL )
	{
		delete this->m_MonsterJewelItem5;
		this->m_MonsterJewelItem5 = NULL;
	}
}
// -------------------------------------------------------------------------------

//0042f2a0	-> 100% (Identical)
BYTE CMonsterItemMng::InsertItem(int monsterlevel, int type, int index, int itemlevel, int op1, int op2, int op3)
{
	if( monsterlevel > MAX_MONSTER_LEVEL || monsterlevel < 0 )
	{
		return -1;
	}
	// ----
	int itemcount = this->m_iMonsterInvenItemCount[monsterlevel];
	// ----
	if( itemcount >= MAX_MONSTER_ITEM )
	{
		return -1;
	}
	// ----
	CItem * item	= &this->m_MonsterInvenItems[monsterlevel][itemcount];
	int item_type	= (type * MAX_SUBTYPE_ITEMS) + index;
	item->m_Level	= itemlevel;
	// ----
	item->Convert(item_type, op1, op2, op3, 0, 0, 0, 0, 0xFF, 0, CURRENT_DB_VERSION);
	// ----
	if( type == 13 && index == 10 )
	{
		item->m_Durability = rand() % 100 + 100.0f;
	}
	else
	{
		item->m_Durability = item->m_BaseDurability;
	}
	// ----
	itemcount++;
	this->m_iMonsterInvenItemCount[monsterlevel] = itemcount;
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

//0042f3e0	-> 100% (Identical)
CItem * CMonsterItemMng::GetItem(int monsterlevel)
{
	if( monsterlevel > MAX_MONSTER_LEVEL || monsterlevel < 0 )
	{
		return 0;
	}
	// ----
	int itemcount = this->m_iMonsterInvenItemCount[monsterlevel];
	// ----
	if( itemcount <= 0 )
	{
		return 0;
	}
	// ----
	int itemindex = rand() % itemcount;
	// ----
	return &this->m_MonsterInvenItems[monsterlevel][itemindex];
}
// -------------------------------------------------------------------------------

//0042f460	-> 100% (Identical)
void CMonsterItemMng::gObjGiveItemSearch(int monsterlevel, int maxlevel)
{
	int result;
	int incount = 0;
	int type, index;
	int BallTable[22];
	// ----
	if( monsterlevel > MAX_MONSTER_LEVEL-1 )
	{
		LogAdd("error-L3 : Monster Level Overflow~ %s %d", __FILE__, __LINE__ );	//Line: 8
		return;
	}
	// ----
	BallTable[0]	= 7;
	BallTable[1]	= 8;
	BallTable[2]	= 9;
	BallTable[3]	= 10;
	BallTable[4]	= 11;
	BallTable[5]	= 12;
	BallTable[6]	= 13;
	BallTable[7]	= 14;
	BallTable[8]	= 16;
	BallTable[9]	= 17;
	BallTable[10]	= 18;
	BallTable[11]	= 19;
	BallTable[12]	= 21;
	BallTable[13]	= 22;
	BallTable[14]	= 23;
	BallTable[15]	= 24;
	BallTable[16]	= 35;
	BallTable[17]	= 44;
    BallTable[18]	= 45;
    BallTable[19]	= 46;
    BallTable[20]	= 47;
    BallTable[21]	= 48;
	// ----
	while(true)
	{
		if( (rand() % 20) ==  0 )
		{
			if( (rand() % 2) != 0 )
			{
				type	= MAX_TYPE_ITEMS-1;
				index	= rand() % (g_MaxItemIndexOfEachItemType[type] + 1);
			}
			else
			{
				type	= 12;
				index	= BallTable[rand() % 22];
			}
		}
		else
		{
			type				= rand() % MAX_TYPE_ITEMS;
			int iMaxItemIndex	= g_MaxItemIndexOfEachItemType[type]+1;
			index				= rand() % iMaxItemIndex;
			// ----
			if( type == 15 || (type == 12 && index != 15 ) )
			{
				continue;
			}
		}
		// ----
		if( type == 13 && index == 3 )
		{
			continue;
		}
		// ----
		if(		(type == 13 && index == 32)
			||	(type == 13 && index == 33)
			||	(type == 13 && index == 34)
			||	(type == 13 && index == 35)
			||	(type == 13 && index == 36)
			||	(type == 13 && index == 37) )
		{
			continue;
		}
		// ----
		if(		(type == 14 && index == 35)
			||	(type == 14 && index == 36)
			||	(type == 14 && index == 37)
			||	(type == 14 && index == 38)
			||	(type == 14 && index == 39)
			||	(type == 14 && index == 40) )
		{
			continue;
		}
		// ----
        if( IsCashItem(ITEMGET(type, index)) == true )
		{
			continue;
		}
        // ----
		if( IsPremiumItem(ITEMGET(type, index)) == true )
		{
			continue;
		}
		// ----
		if( g_LuckyItemManager.IsLuckyItemTicket(ITEMGET(type, index)) == true )
		{
			continue;
		}
		// ----
		if( g_LuckyItemManager.IsLuckyItemEquipment(ITEMGET(type, index)) == true )
		{
			continue;
		}
		// ----
		if(		(type == 14 && index == 162)
			||	(type == 14 && index == 163)
			||	(type == 14 && index == 164)
			||	(type == 14 && index == 165)
			||	(type == 14 && index == 166) )
		{
			continue;
		}
		// ----
		if(		(type == 14 && index == 45)
			||	(type == 14 && index == 46)
			||	(type == 14 && index == 47)
			||	(type == 14 && index == 48)
			||	(type == 14 && index == 49)
			||	(type == 14 && index == 50) )
		{
			continue;
		}
		// ----
		if( type == 13 && index == 41 )
		{
			continue;
		}
		// ----
		if(		(type == 13 && index < 8) 
			||	(type == 14 && (index == 9 || index == 10 || index == 17 || index == 18)) 
			||	(type == 13 && index == 14) 
			||	(type == 14 && index == 31) )
		{
			int perc			= rand() % 11;
			BOOL bCheckDevil	= false;
			// ----
			if( type == 12 && index == 15 )
			{
				if( monsterlevel >= 13 && monsterlevel <= 66 )
				{
					perc = rand() % 7 ;
					// ----
					if( perc < 3 )
					{
						perc = 0;
					}
				}
				else
				{
					perc = 1;
				}
			}
			// ----
			if( type == 14 && index == 17 )
			{
				if( gEyesOfDevilSquareDropRate <= 0 )
				{
					perc		= 1;
					bCheckDevil = false;
				}
				else
				{
					perc		= rand() % gEyesOfDevilSquareDropRate;
					bCheckDevil = true;
				}
				// ----
				if( gDevilSquareEvent == 0 )
				{
					perc = 1;
				}
			}
			// ----
			if( type == 14 && index == 18 )
			{
				if( gKeyOfDevilSquareDropRate <= 0 )
				{
					perc		= 1;
					bCheckDevil = false;
				}
				else
				{
					perc		= rand() % gKeyOfDevilSquareDropRate;
					bCheckDevil = true;
				}
				// ----
				if( gDevilSquareEvent == 0 )
				{
					perc = 1;
				}
			}
			// ----
			if( perc == 0 )
			{
				if( bCheckDevil == true )
				{
					int devilitemlevel = 0;
					// ----
					if( (rand() % 5) != 0 )
					{
						if( monsterlevel < 3 )
						{
							devilitemlevel = 0;
						}
						else if( monsterlevel < 36 ) 
						{
							devilitemlevel = 1;
						}
						else if( monsterlevel < 47 )
						{
							devilitemlevel = 2;
						}
						else if( monsterlevel < 60 )
						{
							devilitemlevel = 3;
						}
						else if( monsterlevel < 70 )
						{
							devilitemlevel = 4;
						}
						else if( monsterlevel < 80 )
						{
							devilitemlevel = 5;
						}
						else 
						{
							devilitemlevel = 6;
						}
						// ----
						if( devilitemlevel != 0 )
						{
							if( this->InsertItem(monsterlevel, type, index, devilitemlevel, 0, 0, 0) != 0xFF )
							{
								incount++;
								// ----
								if( incount > MAX_MONSTER_ITEM - 1 )
								{
									return;
								}
							}
						}
					}
				}
				else if( zzzItemLevel(type, index, monsterlevel) == true )
				{
					if( this->InsertItem(monsterlevel, type, index, 0, 0, 0, 0) != (BYTE)-1 )
					{
						incount++;
						// ----
						if( incount > MAX_MONSTER_ITEM - 1 )
						{
							return;
						}
					}
				}
			}
		}
		else
		{
			result = GetLevelItem(type, index, monsterlevel);
			// ----
			if( result >= 0 )
			{
				if( (type == 13 && index == 10) || (type == 12 && index == 11 ) )
				{
					if( this->InsertItem(monsterlevel, type, index, result, 0, 0, 0) != 0xFF )
					{
						incount++;
						// ----
						if( incount > MAX_MONSTER_ITEM - 1 )
						{
							return;
						}
					}
				}
				else if( result <= maxlevel )
				{
					if( type == 12 )
					{
						if( index != 11 )
						{
							result = 0;
						}
					}
					// ----
					if( type == 12 && index == 11 )
					{
						// --
					}
					else
					{
						if( result > maxlevel )
						{
							result = maxlevel;
						}
					}
					// ----
					if( (type == 4 && index == 7) || (type == 4 && index == 15) )
					{
						result = 0;
					}
					// ----
					if( this->InsertItem(monsterlevel, type, index,result, 0, 0, 0) != (BYTE)-1 )
					{
						incount++;
						// ----
						if( incount > MAX_MONSTER_ITEM - 1 )
						{
							return;
						}
					}
				}
			}
		}
		// ----
		if( this->m_iMonsterInvenItemCount[monsterlevel] >= MAX_MONSTER_ITEM )
		{
			return;
		}
	}
}
// -------------------------------------------------------------------------------

//0042fd30	-> 100% (Identical)
void CMonsterItemMng::LoadMonsterItemDropRate()
{
	char * script_file		= gDirPath.GetNewPath("Monster\\MonsterItemDropRate.dat");
	this->m_bScriptLoaded	= false;
	SMDFile					= fopen(script_file, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("File Open Error : MonsterItemDropRate.dat");
		LogAdd(lMsg.Get(453), script_file);
		return;
	}
	// ----
	SMDToken Token;
	int nDropRatePerItemCnt = 0;
	int nRateCnt			= 0;
	int nLevel				= 0;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == NAME && !strcmp("end", &TokenString[0]) )
		{
			break;
		}
		// ----
		int MonsterLevel									= TokenNumber;
		this->RateData[MonsterLevel].m_MonsterLevel			= MonsterLevel;
		// ----
		Token = GetToken();
		this->RateData[MonsterLevel].m_MagicBookRate		= TokenNumber * 10000000.0f;
		// ----
		Token = GetToken();
		this->RateData[MonsterLevel].m_JewelOfBlessRate		= TokenNumber * 10000000.0f;
		// ----
		Token = GetToken();
		this->RateData[MonsterLevel].m_JewelOfSoulRate		= TokenNumber * 10000000.0f;
		// ----
		Token = GetToken();
		this->RateData[MonsterLevel].m_JewelOfLifeRate		= TokenNumber * 10000000.0f;
		// ----
		Token = GetToken();
		this->RateData[MonsterLevel].m_JewelOfCreationRate	= TokenNumber * 10000000.0f;
		// ----
		Token = GetToken();
		this->RateData[MonsterLevel].m_JewelOfChaosRate		= TokenNumber * 10000000.0f;
		// ----
		Token = GetToken();
		this->RateData[MonsterLevel].m_NormalItemRate		= TokenNumber * 10000000.0f;
		// ----
		int nSum = this->RateData[MonsterLevel].m_MagicBookRate
			+ this->RateData[MonsterLevel].m_JewelOfBlessRate
			+ this->RateData[MonsterLevel].m_JewelOfSoulRate
			+ this->RateData[MonsterLevel].m_JewelOfLifeRate
			+ this->RateData[MonsterLevel].m_JewelOfChaosRate
			+ this->RateData[MonsterLevel].m_JewelOfCreationRate
			+ this->RateData[MonsterLevel].m_NormalItemRate;
		// ----
		this->RateData[MonsterLevel].m_TotalDropRate = nSum;
		// ----
		nLevel++;
		// ----
		if( nLevel >= MAX_MONSTER_LEVEL )
		{
			break;
		}
	}
	// ----
	fclose(SMDFile);
	LogAdd(lMsg.Get(454), script_file);
	this->m_bScriptLoaded = true;
}
// -------------------------------------------------------------------------------

//00430530	-> 100% (Identical)
void CMonsterItemMng::MakeJewelItem()
{
	int item_type;
	// ----
	this->m_MonsterJewelItem1 = new CItem;
	this->m_MonsterJewelItem2 = new CItem;
	this->m_MonsterJewelItem3 = new CItem;
	this->m_MonsterJewelItem4 = new CItem;
	this->m_MonsterJewelItem5 = new CItem;
	// ----
	item_type = ITEMGET(14, 13);
	this->m_MonsterJewelItem1->Convert(item_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_MonsterJewelItem1->m_Level = 0;
	this->m_MonsterJewelItem1->m_Durability = this->m_MonsterJewelItem1->m_BaseDurability;
	// ----
	item_type = ITEMGET(14, 14);
	this->m_MonsterJewelItem2->Convert(item_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_MonsterJewelItem2->m_Level = 0;
	this->m_MonsterJewelItem2->m_Durability = this->m_MonsterJewelItem2->m_BaseDurability;
	// ----
	item_type = ITEMGET(12, 15);
	this->m_MonsterJewelItem3->Convert(item_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_MonsterJewelItem3->m_Level = 0;
	this->m_MonsterJewelItem3->m_Durability = this->m_MonsterJewelItem3->m_BaseDurability;
	// ----
	item_type = ITEMGET(14, 16);
	this->m_MonsterJewelItem4->Convert(item_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_MonsterJewelItem4->m_Level = 0;
	this->m_MonsterJewelItem4->m_Durability = this->m_MonsterJewelItem4->m_BaseDurability;
	// ----
	item_type = ITEMGET(14, 22);
	this->m_MonsterJewelItem5->Convert(item_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
	this->m_MonsterJewelItem5->m_Level = 0;
	this->m_MonsterJewelItem5->m_Durability = this->m_MonsterJewelItem5->m_BaseDurability;
}
// -------------------------------------------------------------------------------

//00430960	-> 100% (Identical)
void CMonsterItemMng::gObjGiveItemSearchEx(int monsterlevel, int maxlevel)
{
	this->NormalGiveItemSearch(monsterlevel, maxlevel);
	this->CMonsterItemMng::MagicBookGiveItemSearch(monsterlevel, maxlevel);
}
// -------------------------------------------------------------------------------

//004309b0	-> 100% (Identical)
void CMonsterItemMng::MagicBookGiveItemSearch(int monsterlevel, int maxlevel)
{
	int result	= 0;
	int incount = 0;
	int type, index;
	int BallTable[22];
	// ----
	if( monsterlevel > MAX_MONSTER_LEVEL - 1 )
	{
		LogAdd("error-L3 : Monster Level Overflow~ %s %d", __FILE__, __LINE__);	//Line: 8
		return;
	}
	// ----
	BallTable[0]	= 7;
	BallTable[1]	= 8;
	BallTable[2]	= 9;
	BallTable[3]	= 10;
	BallTable[4]	= 11;
	BallTable[5]	= 12;
	BallTable[6]	= 13;
	BallTable[7]	= 14;
	BallTable[8]	= 16;
	BallTable[9]	= 17;
	BallTable[10]	= 18;
	BallTable[11]	= 19;
	BallTable[12]	= 21;
	BallTable[13]	= 22;
	BallTable[14]	= 23;
	BallTable[15]	= 24;
	BallTable[16]	= 35;
	BallTable[17]	= 44;
	BallTable[18]	= 45;
	BallTable[19]	= 46;
	BallTable[20]	= 47;
	BallTable[21]	= 48;
	// ----
	while(true)
	{
		if( rand() % 2 )
		{
			type	= MAX_TYPE_ITEMS - 1;
			index	= GetLargeRand() % (g_MaxItemIndexOfEachItemType[type] + 1);
		}
		else
		{
			type	= 12;
			index	= BallTable[GetLargeRand() % 22];
		}
		// ----
		if( type == 12 && index == 11 )
		{
			CItem * item		= &this->m_MonsterInvenExItems[monsterlevel][incount];
			result				= GetLevelItem(type, index, monsterlevel);
			// ----
			if( result >= 0 )
			{
				int item_type		= ITEMGET(type, index);
				item->Convert(item_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
				item->m_Level		= result;
				item->m_Durability	= item->m_BaseDurability;
				// ----
				incount++;
				// ----
				if( incount > MAX_MONSTER_EXITEM - 1 )
				{
					break;
				}
			}
			continue;
		}
		else
		{
			result = GetLevelItem(type, index, monsterlevel);
			// ----
			if( result != 0 )
			{
				continue;
			}
			// ----
			CItem * item	= &this->m_MonsterInvenExItems[monsterlevel][incount];
			int item_type	= ITEMGET(type, index);
			// ----
			item->Convert(item_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, 3);
			item->m_Level		= 0;
			item->m_Durability	= item->m_BaseDurability;
			// ----
			incount++;
			// ----
			if( incount > MAX_MONSTER_EXITEM - 1 )
			{
				break;
			}
		}
	}
	// ----
	this->m_iMonsterInvenExItemCount[monsterlevel] = incount;
}
// -------------------------------------------------------------------------------

//00430d00	-> 100% (Identical)
void CMonsterItemMng::NormalGiveItemSearch(int monsterlevel, int maxlevel)
{
	int result;
	int incount = 0;
	int type, index;
	// ----
	if( monsterlevel > 
		MAX_MONSTER_LEVEL - 1 )
	{
		LogAdd("error-L3 : Monster Level Overflow~ %s %d", __FILE__, __LINE__);	//Line: 8
		return;
	}
	// ----
	while(true)
	{
		type				= rand() % MAX_TYPE_ITEMS;
		int iMaxItemIndex	= g_MaxItemIndexOfEachItemType[type] + 1;
		index				= rand() % iMaxItemIndex;
		// ----
		if( type == 15 || type == 12 )
		{
			continue;
		}
		// ----
		if( !this->CheckMonsterDropItem(type, index) )
		{
			continue;
		}
		// ----
		if(		(type == 13 && index < 8) 
			||	(type == 14 && (index == 9 || index == 10 || index == 17 || index == 18)) 
			||	(type == 13 && index == 14) 
			||	(type == 14 && index == 31) )
		{
			int perc			= rand() % 8;
			BOOL bCheckDevil	= false;
			// ----
			if( type == 12 && index == 15 )
			{
				if( monsterlevel >= 13 && monsterlevel <= 66 )
				{
					perc = rand() % 7 ;
					// ----
					if( perc < 3 )
					{
						perc = 0;
					}
				}
				else
				{
					perc = 1;
				}
			}
			// ----
			if( type == 14 && index == 17 )
			{
				if( gEyesOfDevilSquareDropRate <= 0 )
				{
					perc		= 1;
					bCheckDevil = false;
				}
				else
				{
					perc		= rand() % gEyesOfDevilSquareDropRate;
					bCheckDevil = true;
				}
				// ----
				if( gDevilSquareEvent == 0 )
				{
					perc = 1;
				}
			}
			// ----
			if( type == 14 && index == 18 )
			{
				if( gKeyOfDevilSquareDropRate <= 0 )
				{
					perc		= 1;
					bCheckDevil = false;
				}
				else
				{
					perc		= rand() % gKeyOfDevilSquareDropRate;
					bCheckDevil = true;
				}
				// ----
				if( gDevilSquareEvent == 0 )
				{
					perc = 1;
				}
			}
			// ----
			if( perc == 0 )
			{
				if( bCheckDevil == true )
				{
					int devilitemlevel = 0;
					// ----
					if( (rand() % 5) != 0 )
					{
						if( monsterlevel < 3 )
						{
							devilitemlevel = 0;
						}
						else if( monsterlevel < 36 ) 
						{
							devilitemlevel = 1;
						}
						else if( monsterlevel < 47 )
						{
							devilitemlevel = 2;
						}
						else if( monsterlevel < 60 )
						{
							devilitemlevel = 3;
						}
						else if( monsterlevel < 70 )
						{
							devilitemlevel = 4;
						}
						else if( monsterlevel < 80 )
						{
							devilitemlevel = 5;
						}
						else 
						{
							devilitemlevel = 6;
						}
						// ----
						if( devilitemlevel != 0 )
						{
							if( this->InsertItem(monsterlevel, type, index, devilitemlevel, 0, 0, 0) != 0xFF )
							{
								incount++;
								// ----
								if( incount > MAX_MONSTER_ITEM - 1 )
								{
									return;
								}
							}
						}
					}
				}
				else if( zzzItemLevel(type, index, monsterlevel) == true )
				{
					if( this->InsertItem(monsterlevel, type, index, 0, 0, 0, 0) != (BYTE)-1 )
					{
						incount++;
						// ----
						if( incount > MAX_MONSTER_ITEM - 1 )
						{
							return;
						}
					}
				}
			}
		}
		else
		{
			result = GetLevelItem(type, index, monsterlevel);
			// ----
			if( result >= 0 )
			{
				if( result <= maxlevel )
				{
					if( type == 12 )
					{
						if( index != 11 )
						{
							result = 0;
						}
					}
					// ----
					if( type == 12 && index == 11 )
					{
						// --
					}
					else
					{
						if( result > maxlevel )
						{
							result = maxlevel;
						}
					}
					// ----
					if( (type == 4 && index == 7) || (type == 4 && index == 15) )
					{
						result = 0;
					}
					// ----
					if( this->InsertItem(monsterlevel, type, index,result, 0, 0, 0) != (BYTE)-1 )
					{
						incount++;
						// ----
						if( incount > MAX_MONSTER_ITEM - 1 )
						{
							return;
						}
					}
				}
			}
		}
		// ----
		if( this->m_iMonsterInvenItemCount[monsterlevel] >= MAX_MONSTER_ITEM )
		{
			break;
		}
	}
}
// -------------------------------------------------------------------------------

//004311f0	-> 100% (Identical)
int	CMonsterItemMng::CheckMonsterDropItem(int type, int index)
{
	if(		(type == 14 && index == 13)
		||	(type == 14 && index == 14)
		||	(type == 14 && index == 16)
		||	(type == 14 && index == 22)
		||	(type == 12 && index == 15) )
	{
		return false;
	}
	// ----
	if( type == 13 && index == 3 )
	{
		return false;
	}
	// ----
	if(		(type == 13 && index == 32)
		||	(type == 13 && index == 33)
		||	(type == 13 && index == 34)
		||	(type == 13 && index == 35)
		||	(type == 13 && index == 36) 
		||	(type == 13 && index == 37) )
	{
		return false;
	}
	// ----
	if(		(type == 14 && index == 35)
		||	(type == 14 && index == 36)
		||	(type == 14 && index == 37)
		||	(type == 14 && index == 38)
		||	(type == 14 && index == 39) 
		||	(type == 14 && index == 40) )
	{
		return false;
	}
	// ----
	if( IsCashItem(ITEMGET(type, index)) == true )
	{
		return false;
	}
	// ----
	if( IsPremiumItem(ITEMGET(type, index)) == true )
	{
		return false;
	}
	// ----
	if( g_LuckyItemManager.IsLuckyItemTicket(ITEMGET(type, index)) == true )
	{
		return false;
	}
	// ----
	if( g_LuckyItemManager.IsLuckyItemEquipment(ITEMGET(type, index)) == true )
	{
		return false;
	}
	// ----
	if(		(type == 14 && index == 45)
		||	(type == 14 && index == 46)
		||	(type == 14 && index == 47)
		||	(type == 14 && index == 48)
		||	(type == 14 && index == 49) 
		||	(type == 14 && index == 50) )
	{
		return false;
	}
	// ----
	if( type == 13 && index == 41 )
	{
		return false;
	}
	// ----
	if(		(type == 14 && index == 162)
		||	(type == 14 && index == 163)
		||	(type == 14 && index == 164)
		||	(type == 14 && index == 165)
		||	(type == 14 && index == 166) )
	{
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004314c0	-> 100% (Identical)
CItem * CMonsterItemMng::GetItemEx(int monsterlevel)
{
	if( m_bScriptLoaded == false )
	{
		return 0;
	}
	// ----
	if( monsterlevel > MAX_MONSTER_LEVEL || monsterlevel < 0 )
	{
		return 0;
	}
	// ----
	int sum			= 0;
	int nRandValue	= GetLargeRand() % 10000000;
	sum				+= this->RateData[monsterlevel].m_MagicBookRate;
	// ----
	if( nRandValue < sum )
	{
		int itemcount = this->m_iMonsterInvenExItemCount[monsterlevel];
		// ----
		if( itemcount <= 0 )
		{
			return 0;
		}
		else
		{
			int itemindex = GetLargeRand() % itemcount;
			return &this->m_MonsterInvenExItems[monsterlevel][itemindex];
		}
	}
	// ----
	sum	+= this->RateData[monsterlevel].m_JewelOfBlessRate;
	// ----
	if( nRandValue < sum )
	{
		return this->m_MonsterJewelItem1;
	}
	// ----
	sum	+= this->RateData[monsterlevel].m_JewelOfSoulRate;
	// ----
	if( nRandValue < sum )
	{
		return this->m_MonsterJewelItem2;
	}
	// ----
	sum	+= this->RateData[monsterlevel].m_JewelOfLifeRate;
	// ----
	if( nRandValue < sum )
	{
		return this->m_MonsterJewelItem3;
	}
	// ----
	sum	+= this->RateData[monsterlevel].m_JewelOfCreationRate;
	// ----
	if( nRandValue < sum )
	{
		return this->m_MonsterJewelItem4;
	}
	// ----
	sum	+= this->RateData[monsterlevel].m_JewelOfChaosRate;
	// ----
	if( nRandValue < sum )
	{
		return this->m_MonsterJewelItem5;
	}
	// ----
	int itemcount = this->m_iMonsterInvenItemCount[monsterlevel];
	// ----
	if( itemcount <= 0 )
	{
		return 0;
	}
	else
	{
		int itemindex = GetLargeRand() % itemcount;
		return &this->m_MonsterInvenItems[monsterlevel][itemindex];
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------