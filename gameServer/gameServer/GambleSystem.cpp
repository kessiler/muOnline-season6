// GambleSystem.cpp: implementation of the CGambleSystem class.
//	GameServer 1.00.90	-	Completed	(Moss Merchant System)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GambleSystem.h"
#include "..\include\readscript.h"
#include "Gamemain.h"
#include "LogProc.h"
#include "LargeRand.h"
#include "dsprotocol.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGambleSystem	g_GambleSystem;

CGambleSystem::CGambleSystem()
{
	//this->Init(gDirPath.GetNewPath("GamblingItemBag.txt")); //Set Gamble System Initialization
}

CGambleSystem::~CGambleSystem()
{
	//No destructor needed
}

void CGambleSystem::Init(char* filename) //0049B880  Identical
{
	this->bLoad = FALSE; //0x4
	this->m_iGambleBagCount = 0; //0x260
	this->m_iRateKindCount = 0; //0x2C8

	int i;

	for(i = 0; i < GAMBLE_MAX_SECRET_ITEM_CLASS; i++)
	{
		this->m_iBagItemOptionRate[i] = 0;
	}

	for(i = 0; i < GAMBLE_MAX_NORMAL_ITEM; i++)
	{
		this->m_dwSpecialItemRate[i] = NULL;
	}

	for(i = 0; i < GAMBLE_MAX_SECRET_ITEM_CLASS; i++)
	{
		//empty (maybe debug log?)
	}

	for(i = 0; i < GAMBLE_MAX_SECRET_ITEM_TYPE; i++)
	{
		//empty (maybe debug log?)
	}

	for(i = 0; i < GAMBLE_MAX_ITEM_INFO; i++)
	{
		this->m_iDropRatePerItemCount[i] = 0;
	}

	this->Load(gDirPath.GetNewPath(filename));
}

void CGambleSystem::Load(char* filename) //0049B9D0  (Identical)
{
	this->bLoad = FALSE;
	this->m_iGambleBagCount = 0;

	SMDFile = fopen(filename, "r");	//ok

	if ( SMDFile == NULL )
	{
		LogAdd(lMsg.Get(453), filename);
		return;
	}
	SMDToken Token;

	int n = 0;
	int iProbability = 0;
	int Count = 0;

	while(true)
	{
		Token = GetToken();

		if ( Token == END )
		{
			break;
		}

		if ( Token == 1 )
		{
			int Type = TokenNumber;

			if (Type == 0)
			{
				Count = 0;

				while(true)
				{
					Token = GetToken();

					if( Token == 0 )
					{
						if (strcmp("end",TokenString) == 0)
						{
							break;
						}
					}

					if ( Count > GAMBLE_MAX_ITEM_INFO )
					{
						MsgBox("Load Script Error %s",filename);
						break;
					}

					m_GamblingInfo[Count].m_iSkillRate = TokenNumber;

					Token = GetToken();
					m_GamblingInfo[Count].m_iLuckRate = TokenNumber;

					Token = GetToken();
					m_GamblingInfo[Count].m_iOptionRate = TokenNumber;

					Token = GetToken();
					m_GamblingInfo[Count].m_iExcRate = TokenNumber;

					Token = GetToken();
					m_GamblingInfo[Count].m_iSortItemRate = TokenNumber;
					
					Count++;
				}
			}
			else if (Type == 1)
			{
				Count = 0;

				while(true)
				{
					Token = GetToken();

					if( Token == 0 )
					{
						if (strcmp("end",TokenString) == 0)
						{
							break;
						}
					}
					if ( Count > GAMBLE_MAX_SECRET_ITEM_CLASS )
					{
						MsgBox("Load Script Error %s",filename);
						break;
					}

					this->m_iBagItemOptionRate[Count] = TokenNumber;

					Count++;
				}
			}
			else if (Type == 2)
			{
				Count = 0;

				while(true)
				{
					Token = GetToken();

					if( Token == 0 )
					{
						if (strcmp("end",TokenString) == 0)
						{
							break;
						}
					}

					if ( Count > GAMBLE_MAX_NORMAL_ITEM )
					{
						MsgBox("Load Script Error %s",filename);
						break;
					}

					this->m_dwSpecialItemRate[Count] = TokenNumber;

					Count++;
				}
			}
			else if (Type == 3)
			{
				Count = 0;

				while(true)
				{
					Token = GetToken();

					if( Token == 0 )
					{
						if (strcmp("end",TokenString) == 0)
						{
							break;
						}
					}
					if ( Count > GAMBLE_MAX_SECRET_ITEM_CLASS )
					{
						MsgBox("Load Script Error %s",filename);
						break;
					}

					this->m_iBagItemExcRate[Count] = TokenNumber;

					Count++;
				}
			}
			else if (Type == 4)
			{
				Count = 0;

				while(true)
				{
					Token = GetToken();

					if( Token == 0 )
					{
						if (strcmp("end",TokenString) == 0)
						{
							break;
						}
					}

					if ( Count > GAMBLE_MAX_SECRET_ITEM_TYPE )
					{
						MsgBox("Load Script Error %s",filename);
						break;
					}

					this->m_iExcRatePerItemKind[Count] = TokenNumber;

					Token = GetToken();
					this->m_iGeneralExcRate[Count] = TokenNumber;

					Count++;
				}
			}
			else if (Type >= 5)
			{
				n = 0;

				while(true)
				{
					Token = GetToken();

					if( Token == 0 )
					{
						if (strcmp("end",TokenString) == 0)
						{
							this->m_iDropRatePerItemCount[iProbability] = n;
							break;
						}
					}
					
					this->BagObject[m_iGambleBagCount].m_type = TokenNumber;
						
					Token = GetToken();
					this->BagObject[m_iGambleBagCount].m_index = TokenNumber;

					Token = GetToken();
					this->BagObject[m_iGambleBagCount].m_minLevel = TokenNumber;

					Token = GetToken();
					this->BagObject[m_iGambleBagCount].m_maxLevel = TokenNumber;

					Token = GetToken();
					this->BagObject[m_iGambleBagCount].m_isskill = TokenNumber;

					Token = GetToken();
					this->BagObject[m_iGambleBagCount].m_isluck = TokenNumber;

					Token = GetToken();
					this->BagObject[m_iGambleBagCount].m_isoption = TokenNumber;

					Token = GetToken();
					this->BagObject[m_iGambleBagCount].m_isexitem = TokenNumber;

					if(this->BagObject[m_iGambleBagCount].m_minLevel > this->BagObject[m_iGambleBagCount].m_maxLevel )
					{
						MsgBox("Load Script Error %s",filename);
						return;
					}
					
					n++;
					this->m_iGambleBagCount++;

					if (this->m_iGambleBagCount > GAMBLE_MAX_ITEM-1 )
					{
						break;
					}

				}

				iProbability++;

				if(iProbability > GAMBLE_MAX_ITEM_INFO-1)
				{
					break;
				}

				this->m_iRateKindCount = iProbability;
			}
		}
	}

	fclose(SMDFile);
	LogAdd(lMsg.Get(454), filename);
	this->bLoad = TRUE;
}

BYTE CGambleSystem::GetLevel(int n) //0049C600  (Identical)
{
	if ( n <0 || n > MAX_ITEMBAG_ATTR-1 )
	{
		return 0;
	}

	if ( this->BagObject[n].m_minLevel == this->BagObject[n].m_maxLevel )
	{
		return this->BagObject[n].m_minLevel;
	}

	int sub = (this->BagObject[n].m_maxLevel - this->BagObject[n].m_minLevel) + 1;
	int level = this->BagObject[n].m_minLevel + (rand()%sub);

	return level;
}

BYTE CGambleSystem::TryGamble(int aIndex, BYTE MapNumber, int cX, int cY, BYTE btRareItemNum) //0049C6D0 (Identical)
{
	if (this->bLoad == FALSE)
	{
		return 0;
	}

	float Dur = 0;
	int ItemType = 0;
	int Level = 0;
	int MapPosX = 0;
	int MapPosY = 0;
	int Op1 = 0;
	int Op2 = 0;
	int Op3 = 0;
	DWORD n = 0; //loc10
	int loc_11 = 0;
	int NewOp = 0;

	LPOBJ lpObj = &gObj[aIndex]; //loc_13
	 
	if(this->GetBagItemCount() > 0)
	{
		if (this->m_dwSpecialItemRate[btRareItemNum] >= GetLargeRand() % 1000000)
		{
			Level = 0;
			Dur = 0;
			Op1 = 1;
			Op2 = 0;

			DWORD dwOptionRate = GetLargeRand() % 1000000;

			if ( dwOptionRate <= this->m_iBagItemOptionRate[0])
			{
				Op3 = 1;
			}
			else if ( dwOptionRate <= this->m_iBagItemOptionRate[0] + this->m_iBagItemOptionRate[1])
			{
				Op3 = 2;
			}
			else if ( dwOptionRate <= this->m_iBagItemOptionRate[0] + this->m_iBagItemOptionRate[1] + this->m_iBagItemOptionRate[2])
			{
				Op3 = 3;
			}
			else if ( dwOptionRate <= this->m_iBagItemOptionRate[0] + this->m_iBagItemOptionRate[1] + this->m_iBagItemOptionRate[2] + this->m_iBagItemOptionRate[3])
			{
				Op3 = 4;
			}

			DWORD dwNewOptionRate = GetLargeRand() % 1000000;

			if ( dwNewOptionRate <= this->m_iBagItemExcRate[0])
			{
				NewOp = this->SortExcNumberPerKind(1);
			}
			else if ( dwNewOptionRate <= this->m_iBagItemExcRate[0] + this->m_iBagItemExcRate[1])
			{
				NewOp = this->SortExcNumberPerKind(2);
			}
			else if ( dwNewOptionRate <= this->m_iBagItemExcRate[0] + this->m_iBagItemExcRate[1] + this->m_iBagItemExcRate[2])
			{
				NewOp = this->SortExcNumberPerKind(3);
			}
			else if ( dwNewOptionRate <= this->m_iBagItemExcRate[0] + this->m_iBagItemExcRate[1] + this->m_iBagItemExcRate[2] + this->m_iBagItemExcRate[3])
			{
				NewOp = this->SortExcNumberPerKind(4);
			}

			switch ( btRareItemNum ) //5 Types of Special Item
			{
				case 0:
					ItemType = ItemGetNumberMake(3, 11);
				break;
				case 1:
					ItemType = ItemGetNumberMake(5, 33);
				break;
				case 2:
					ItemType = ItemGetNumberMake(4, 24);
				break;
				case 3:
					ItemType = ItemGetNumberMake(2, 18);
				break;
				case 4:
					ItemType = ItemGetNumberMake(5, 34);
				break;
			}
		}
		else //else for common items like a bag
		{
			int iBagNumberCount = 0;
			int iRandA = 0;
			int iRandB = 0;
			int iBagNumber = 0;

			switch( btRareItemNum )
			{
				case 0:
					iBagNumber = 0;
					break;
				case 1:
					iBagNumber = 5;
					break;
				case 2:
					iBagNumber = 10;
					break;
				case 3:
					iBagNumber = 15;
					break;
				case 4:
					iBagNumber = 20;
					break;
			}

			DWORD dwSortItemRand = GetLargeRand() % 1000000;
			
			int iSortItemRate = 0;

			for ( int i = iBagNumber; i < iBagNumber + GAMBLE_MAX_NORMAL_ITEM; i++)
			{
				iSortItemRate += this->m_GamblingInfo[i].m_iSortItemRate;

				if(dwSortItemRand <= iSortItemRate)
				{
					iBagNumber = i;
					break;
				}
			}
			
			while(true) //Max 25
			{
				iRandB += this->m_iDropRatePerItemCount[iBagNumberCount];
	
				if (iBagNumber <= iBagNumberCount)
				{
					break;
				}

				iRandA += this->m_iDropRatePerItemCount[iBagNumberCount];
				iBagNumberCount++;
			}

			while(true)	//temp fix
			{
				if( iRandB > iRandA )
				{
					break;
				}
				// ---
				iRandB++;
			}

			n = (GetLargeRand() % (iRandB - iRandA)) + iRandA;
		
			Dur = 0;
			MapPosX = 0;
			MapPosY = 0;

			Level = this->GetLevel(n);

			ItemType = ItemGetNumberMake(this->BagObject[n].m_type,this->BagObject[n].m_index);

			if (ItemType == -1)
			{
				return 0;
			}

			if( this->BagObject[n].m_isskill != 0)
			{
				if (this->m_GamblingInfo[iBagNumber].m_iSkillRate >= GetLargeRand()%1000000)
				{
					Op1 = 1;
				}
			}

			if(this->BagObject[n].m_isluck != 0)
			{
				if (this->m_GamblingInfo[iBagNumber].m_iLuckRate >= GetLargeRand()%1000000)
				{
					Op2 = 1;
				}
			}

			if(this->BagObject[n].m_isoption != 0)
			{
				if (this->m_GamblingInfo[iBagNumber].m_iOptionRate >= GetLargeRand()%1000000)
				{
					int iOpt3Rand = GetLargeRand() % 1000000;

					if ( iOpt3Rand <= this->m_iBagItemOptionRate[0])
					{
						Op3 = 1;
					}
					else if ( iOpt3Rand <= this->m_iBagItemOptionRate[0] + this->m_iBagItemOptionRate[1])
					{
						Op3 = 2;
					}
					else if ( iOpt3Rand <= this->m_iBagItemOptionRate[0] + this->m_iBagItemOptionRate[1] + this->m_iBagItemOptionRate[2])
					{
						Op3 = 3;
					}
					else if ( iOpt3Rand <= this->m_iBagItemOptionRate[0] + this->m_iBagItemOptionRate[1] + this->m_iBagItemOptionRate[2] + this->m_iBagItemOptionRate[3])
					{
						Op3 = 4;
					}
				}
			}
			
			if(this->BagObject[n].m_isexitem != 0)
			{
				if (this->m_GamblingInfo[iBagNumber].m_iExcRate >= GetLargeRand()%1000000)
				{
					NewOp = this->SortExcOption();
					
					Op2 = 0;
					Op1 = 1;
					Level = 0;	
				}
			}
		}

		ItemSerialCreateSend(lpObj->m_Index,MapNumber,MapPosX,MapPosY,ItemType,Level,Dur,Op1,Op2,Op3,lpObj->m_Index,NewOp,0);
		LogAddTD("[ GAMBLING ][ TryGambling ] %s(%s) Try Item : [%s]%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",lpObj->AccountID,lpObj->Name,ItemAttribute[ItemType].Name,ItemType,Level,Op1,Op2, Op3,NewOp);
	}
	return 1;
}

int CGambleSystem::GetBagItemCount() //0049CF00 (Identical)
{
	return m_iGambleBagCount;
}

BYTE CGambleSystem::SortExcNumberPerKind(int n) //0049CF30 (Identical)
{
	BYTE btExOption = 0;
	int iFullOptRate = 0;
	int iItemIndex = 0;

	while(true)
	{
		if (this->m_iExcRatePerItemKind[iItemIndex] >= GetLargeRand() % 1000000)
		{
			btExOption |= 1 << iItemIndex;
		}
		else
		{
			iFullOptRate++;
		}

		if(iFullOptRate >= 6-n)
		{
			while(true)
			{
				iItemIndex++;

				if(iItemIndex >= GAMBLE_MAX_SECRET_ITEM_TYPE)
				{
					break;
				}

				btExOption |= 1 << iItemIndex;
			}
			break;
		}
		iItemIndex++;
	}

	return btExOption;
}

BYTE CGambleSystem::SortExcOption() //0049D020 (Identical)
{
	BYTE btExOption = 0;
	int iItemIndex = 0;
	int count = 0;

	while(true)
	{
		if (this->m_iGeneralExcRate[iItemIndex] >= GetLargeRand() % 1000000)
		{
			btExOption |= 1 << iItemIndex;
			count++;
		}

		iItemIndex++;

		if(count == 0)
		{
			if(iItemIndex >= GAMBLE_MAX_SECRET_ITEM_TYPE)
			{
				iItemIndex = 0;
			}
		}
		else if(count == 1)
		{
			if(iItemIndex >= GAMBLE_MAX_SECRET_ITEM_TYPE)
			{
				break;
			}
		}
		else if(count >= 2)
		{
			break;
		}
	}

	return btExOption;
}