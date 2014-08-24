//-> Revised by DarkSim | 18.02.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#include "stdafx.h"
#include "ProbabilityItemBag.h"
#include "..\common\SetItemOption.h"
#include "gObjMonster.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "logproc.h"
#include "LargeRand.h"
// -------------------------------------------------------------------------------

//004b8d70	->	100%
CProbabilityItemBag::CProbabilityItemBag()
{
	// ----
}
// -------------------------------------------------------------------------------

//004b8e60	-> 100%
CProbabilityItemBag::~CProbabilityItemBag()
{

}
// -------------------------------------------------------------------------------

//004b8e90	-> 100%
void CProbabilityItemBag::ProbabilityItemBagInit(char * name)
{ 
	this->m_bLoad				= 0;
	this->m_sEventName[0]		= 0;
	this->m_iEventItemType		= -1;
	this->m_iEventItemLevel		= 0;
	this->m_iDropZen			= 0;
	this->m_iEventItemDropRate	= 0;
	this->m_iItemDropRate		= 0;
	this->m_iExItemDropRate		= 0;
	this->m_iBagObjectCount		= 0;
	this->m_iRateKindCount		= 0;
	// ----
	this->LoadItem(gDirPath.GetNewPath(name));
}
// -------------------------------------------------------------------------------

//004b8f60	->
void CProbabilityItemBag::LoadItem(char * script_file)
{
	int Token;

	this->m_bLoad = FALSE;

	SMDFile = fopen(script_file, "r");	//ok

	if ( SMDFile == NULL )
	{
		LogAdd(lMsg.Get(MSGGET(1, 197)), script_file);
		return;
	}

	int n=0;
	int	iProbability=0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			int st = TokenNumber;	// script_type

			if ( st == 0 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					int map = TokenNumber;

					if ( MAX_MAP_RANGE(map) == FALSE )
					{
						MsgBox("ExEvent ItemBag LoadFail [%s]", script_file);
						return;
					}

					Token = GetToken();
					this->DropMapInfo[map].m_bIsDrop = TokenNumber;

					Token = GetToken();
					this->DropMapInfo[map].m_MinMonsterLevel = TokenNumber;

					Token = GetToken();
					this->DropMapInfo[map].m_MaxMonsterLevel = TokenNumber;
				}
			
			}
			else if ( st == 1 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					strcpy(this->m_sEventName, TokenString);

					Token = GetToken();
					this->m_iDropZen = TokenNumber;

					Token = GetToken();
					int type = TokenNumber;

					Token = GetToken();
					int index = TokenNumber;

					this->m_iEventItemType = ITEMGET(type, index);

					Token = GetToken();
					this->m_iEventItemLevel = TokenNumber;

					Token = GetToken();
					this->m_iEventItemDropRate = TokenNumber;

					Token = GetToken();
					this->m_iItemDropRate = TokenNumber;

					Token = GetToken();
					this->m_iExItemDropRate = TokenNumber;

					char szTemp[256];

					wsprintf(szTemp, "[%s] Eventitemnum = %d,EventItemLevel = %d, EventItemDropRate = %d, ItemDropRate = %d, ExItemDropRate = %d", this->m_sEventName, this->m_iEventItemType, this->m_iEventItemLevel, this->m_iEventItemDropRate, this->m_iItemDropRate, this->m_iExItemDropRate);
					LogAddTD(szTemp);
				}
			}
			else if ( st == 2 )
			{
				iProbability = 0; //ebp-10

				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					if( iProbability == 0) //good
					{
						this->m_iEventItemDropRateEx[iProbability] = (int)TokenNumber; //good
					}
					else
					{
						this->m_iEventItemDropRateEx[iProbability] = this->m_iEventItemDropRateEx[iProbability-1] + (int)TokenNumber; //great
					}

					n = 0;

					while ( true )
					{
						Token = GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", TokenString) == 0 )
							{
								this->m_iDropRatePerItemCount[iProbability] = n;
								break;
							}
						}

						this->BagObject[this->m_iBagObjectCount].m_type = TokenNumber;

						Token = GetToken();
						this->BagObject[this->m_iBagObjectCount].m_index = TokenNumber;

						Token = GetToken();
						this->BagObject[this->m_iBagObjectCount].m_minLevel = TokenNumber;

						Token = GetToken();
						this->BagObject[this->m_iBagObjectCount].m_maxLevel = TokenNumber;

						Token = GetToken();
						this->BagObject[this->m_iBagObjectCount].m_isskill = TokenNumber;

						Token = GetToken();
						this->BagObject[this->m_iBagObjectCount].m_isluck = TokenNumber;

						Token = GetToken();
						this->BagObject[this->m_iBagObjectCount].m_isoption = TokenNumber;

						Token = GetToken();
						this->BagObject[this->m_iBagObjectCount].m_isexitem = TokenNumber;

						if(this->BagObject[this->m_iBagObjectCount].m_minLevel > this->BagObject[this->m_iBagObjectCount].m_maxLevel)
						{
							MsgBox("Load Script Error %s", script_file);
							return;
						}
						
						n++;
						this->m_iBagObjectCount++;

						if ( this->m_iBagObjectCount > MAX_ITEMBAG_ATTR-1 )
						{
							break;
						}
					}

					iProbability++;

					if(iProbability > 19)
					{
						break;
					}
					continue;
				}
				this->m_iRateKindCount = iProbability;
			}
			else if ( st == 3 )
			{
				iProbability = this->m_iRateKindCount;

				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					if( iProbability == 0) //good
					{
						this->m_iEventItemDropRateEx[iProbability] = (int)TokenNumber; //good
					}
					else
					{
						this->m_iEventItemDropRateEx[iProbability] = this->m_iEventItemDropRateEx[iProbability-1] + (int)TokenNumber; //great
					}

					n = 0;

					while ( true )
					{
						Token = GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", TokenString) == 0 )
							{
								this->m_iDropRatePerItemCount[iProbability] = n;
								break;
							}
						}

						this->BagObject[this->m_iBagObjectCount].m_type = TokenNumber;

						Token = GetToken();
						this->BagObject[this->m_iBagObjectCount].m_index = TokenNumber;

						this->BagObject[this->m_iBagObjectCount].m_isSetItem = 1; //season 3.5 add-on
						this->BagObject[this->m_iBagObjectCount].m_minLevel = 0;
						this->BagObject[this->m_iBagObjectCount].m_maxLevel = 0;
						this->BagObject[this->m_iBagObjectCount].m_isskill = 0;
						this->BagObject[this->m_iBagObjectCount].m_isluck = 0;
						this->BagObject[this->m_iBagObjectCount].m_isoption = 0;
						this->BagObject[this->m_iBagObjectCount].m_isexitem = 0;

						if(this->BagObject[this->m_iBagObjectCount].m_minLevel > this->BagObject[this->m_iBagObjectCount].m_maxLevel)
						{
							MsgBox("Load Script Error %s", script_file);
							return;
						}
						
						n++;
						this->m_iBagObjectCount++;

						if ( this->m_iBagObjectCount > MAX_ITEMBAG_ATTR-1 )
						{
							break;
						}
					}

					iProbability++;

					if(iProbability > 19)
					{
						break;
					}
					continue;
				}
				this->m_iRateKindCount = iProbability;
			}
		}
	}

	fclose(SMDFile);
	LogAdd(lMsg.Get(MSGGET(1, 198)), script_file);
	this->m_bLoad = TRUE;
}
// -------------------------------------------------------------------------------

//004b9e80	-> 100%
BYTE CProbabilityItemBag::GetLevel(int n) 
{ 
	if( !IS_PROBABILITY_RANGE(n) )
	{
		return false;
	}
	// ----
	if( this->BagObject[n].m_minLevel == this->BagObject[n].m_maxLevel )
	{
		return this->BagObject[n].m_minLevel;
	}
	// ----
	int sub		= (this->BagObject[n].m_maxLevel - this->BagObject[n].m_minLevel) + 1;
	int level	= this->BagObject[n].m_minLevel + (rand() % sub);
	// ----
	return level;
}
// -------------------------------------------------------------------------------

//004b9f70	-> 100%
int CProbabilityItemBag::IsEnableEventItemDrop(int aIndex)
{ 
	int iMapNumber = gObj[aIndex].MapNumber;
	// ----
	if( !this->DropMapInfo[iMapNumber].m_bIsDrop )
	{
		return false;
	}
	// ----
	int iLevel = gObj[aIndex].Level;
	// ----
	if( iLevel < this->DropMapInfo[iMapNumber].m_MinMonsterLevel || iLevel > this->DropMapInfo[iMapNumber].m_MaxMonsterLevel )
	{
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004ba040	-> 100% (But i think that it for loop...)
int CProbabilityItemBag::GetDropItemNumber(int nDropItemRate)
{ 
	int nRateCnt			= this->m_iRateKindCount;
	int iDropItemNum		= 0;
	int ItemRateIndexPos	= this->GetBagCount() - 1;
	// ----
	while(nRateCnt-- != 0)
	{
		if( nRateCnt == 0 )
		{
			iDropItemNum = rand() % this->m_iDropRatePerItemCount[0];
			break;
		}
		// ----
		if( nDropItemRate >= 10000 - this->m_iEventItemDropRateEx[nRateCnt] 
		&& nDropItemRate < 10000 - this->m_iEventItemDropRateEx[nRateCnt - 1] )
		{
			iDropItemNum = ItemRateIndexPos - rand() % this->m_iDropRatePerItemCount[nRateCnt];
			break;
		}
		// ----
		ItemRateIndexPos = ItemRateIndexPos - this->m_iDropRatePerItemCount[nRateCnt];
	}
	// ----
	return iDropItemNum;
}
// -------------------------------------------------------------------------------

//004ba140	-> 100%
int CProbabilityItemBag::GetBagCount()
{ 
	return this->m_iBagObjectCount;
}
// -------------------------------------------------------------------------------

//004ba170	-> 100%
char * CProbabilityItemBag::GetEventName()
{
	return this->m_sEventName;
}
// -------------------------------------------------------------------------------

//004ba190	-> Complete, but some names have wrong positions & not identical all :D
int CProbabilityItemBag::DropNewYearLuckyBagEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0;	//ebp-8
	int iType			= 0;	//ebp-0c
	int iLevel			= 0;	//ebp-10
	int X				= 0;	//ebp-14
	int Y				= 0;	//ebp-18
	int iOption1		= 0;	//ebp-1c
	int iOption2		= 0;	//ebp-20
	int iOption3		= 0;	//ebp-24
	int nDropItemRate	= 0;	//ebp-28
	int nRateCnt		= 0;	//ebp-2c
	int iExOption		= 0;	//ebp-30
	// ----
	LPOBJ lpObj		= &gObj[aIndex];	//ebp-34
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = GetLargeRand() % 1000000;	//ebp-38
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 1000000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[ NewYearLuckyBagMonsterEvent ] NewYearLuckyBagDrop [%s][%s] [%d Zen]",
			lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumberEx(iDropItemRate);	//ebp-3c
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if ( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[ NewYearLuckyBagMonsterEvent ] NewYearLuckyBagDrop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004ba620	-> Complete, but some names have wrong positions & not identical all :D
int CProbabilityItemBag::DropGMPresentBoxEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0.0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = rand() % 10000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 10000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[ GMPresentBox ] GMPresentBoxItemDrop [%s][%s] [%d Zen]",
			lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumber(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption == 1 )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	else
	{
		if( this->BagObject[iDropItemNum].m_isoption > 1 )
		{
			iOption3 = this->BagObject[iDropItemNum].m_isoption >> 2;	//-> New fix ;D
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if ( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	if( iType == ITEMGET(13, 20) )
	{
		iLevel		= 0;
		fDur		= 255.0;
	}
	// ----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[ GMPresentBox ] GMPresentBoxItemDrop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004bab40	->
int CProbabilityItemBag::DropIllusionTempleGhostItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = rand() % 10000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 10000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[ ILLUSIONTempleEvent ] ILLUSIONTempleEventDrop [%s][%s] [%d Zen]",
			lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumber(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption == 1 )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if ( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[ ILLUSIONTempleEvent ] ILLUSIONTempleEventDrop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004bafe0	->
int CProbabilityItemBag::DropEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0.0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = rand() % 10000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 10000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[%s] Drop Zen [%s][%s] [%d Zen]",
			this->GetEventName(), lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumber(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isSetItem )
	{
			this->DropEventSetItem(aIndex, iType, btMapNumber, X, Y);
			return true;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = false;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption == 1 )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	else
	{
		if( this->BagObject[iDropItemNum].m_isoption > 1 )
		{
			iOption3 = this->BagObject[iDropItemNum].m_isoption >> 2;	//-> New fix ;D
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel	= 0;
	}
	// ----
	if( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) || iType == ITEMGET(14, 22) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	if(		iType == ITEMGET(13, 0)		|| iType == ITEMGET(13, 1)	|| iType == ITEMGET(13, 2) 
		||	iType == ITEMGET(13, 8)		|| iType == ITEMGET(13, 9)	|| iType == ITEMGET(13, 12) 
		||	iType == ITEMGET(13, 13)	|| iType == ITEMGET(13, 38) || iType == ITEMGET(13, 39) 
		||	iType == ITEMGET(13, 40)	|| iType == ITEMGET(13, 41) )
		{
			iLevel	= 0;
			fDur	= 255.0;
		}
	// -----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[%s] Drop Item [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		this->GetEventName(), lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004bb5d0	->
void CProbabilityItemBag::DropEventSetItem(int aIndex, int itemnum, BYTE btMapNumber, int X, int Y)
{ 
	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	ItemSerialCreateSend(aIndex,btMapNumber,X,Y,itemnum,0,0,Option1,Option2,Option3,aIndex,0,SetOption);

	int loc10 = 0;

	if((SetOption&0x01) != 0)
	{
		loc10 = 1;
	}
	else if((SetOption&0x02) != 0)
	{
		loc10 = 2;
	}

	LogAddTD("[%s] Drop Set Item [%s][%s] : (%d)(%d/%d) Itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",this->GetEventName(), gObj[aIndex].AccountID,gObj[aIndex].Name,
		btMapNumber, X, Y, itemnum,Option1,Option2,Option3,SetOption, gSetItemOption.GetSetOptionName(itemnum, loc10));
}
// -------------------------------------------------------------------------------

//004bb7f0	->
int CProbabilityItemBag::DropCherryBlossomEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY, int * pMoney)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = rand() % 10000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 10000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[%s] Drop Zen [%s][%s] [%d Zen]",
			this->GetEventName(), lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumber(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	if( btMapNumber == 255 )
	{
		X = 0;
		Y = 0;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isSetItem )
	{
		this->DropEventSetItem(aIndex, iType, btMapNumber, X, Y);
		return true;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if ( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[%s] Drop Item [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		this->GetEventName(), lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004bbd40	->
int CProbabilityItemBag::DropRaklionSelupanItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY) 
{ 
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur = 0; //2
	int type = 0;//3
	int level = 0;//4
	int x = 0;//5
	int y = 0;//6
	int Option1 = 0;//7
	int Option2 = 0;//8
	int Option3 = 0;//9
	int DropItemNum = 0;//10
	int ExOption = 0;//11
	int loc12 = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum =  rand() % this->GetBagCount();
		dur = 0;

		if ( cX == 0 && cY == 0 )
		{
			x = lpObj->X;
			y = lpObj->Y;
		}
		else
		{
			x = cX;
			y = cY;
		}

		level = this->GetLevel(DropItemNum);
		type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

		if ( type == -1 )
		{
			return FALSE;
		}

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
		{
			Option1 =1;
		}

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
		{
			Option2 = 0;

			if ( (rand()%2) == 0 )
			{
				Option2 = 1;
			}
		}

		if ( this->BagObject[DropItemNum].m_isoption != 0 )
		{
			if ( rand()%5 < 1 )
			{
				Option3 = 3;
			}
			else
			{
				Option3 = rand()%3;
			}
		}

		if ( this->BagObject[DropItemNum].m_isexitem != 0 )	//fixed
		{
			loc12 = NewOptionRand(0);
			//Option1 = 1;
			//level = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, loc12, 0);
		LogAddTD("[ RAKLION ][ Reward ] Selupan Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3, loc12);

	}
	return TRUE;
}
// -------------------------------------------------------------------------------

//004bc0c0	-> 100%
int CProbabilityItemBag::GetDropItemNumberEx(int nDropItemRate)
{ 
	int nRateCnt			= this->m_iRateKindCount;
	int iDropItemNum		= 0;
	int ItemRateIndexPos	= this->GetBagCount() - 1;
	// ----
	while(nRateCnt-- != 0)
	{
		if( nRateCnt == 0 )
		{
			iDropItemNum = GetLargeRand() % this->m_iDropRatePerItemCount[0];
			break;
		}
		// ----
		if( nDropItemRate >= 1000000 - this->m_iEventItemDropRateEx[nRateCnt] 
		&& nDropItemRate < 1000000 - this->m_iEventItemDropRateEx[nRateCnt - 1] )
		{
			iDropItemNum = ItemRateIndexPos - GetLargeRand() % this->m_iDropRatePerItemCount[nRateCnt];
			break;
		}
		// ----
		ItemRateIndexPos = ItemRateIndexPos - this->m_iDropRatePerItemCount[nRateCnt];
	}
	// ----
	return iDropItemNum;
}
// -------------------------------------------------------------------------------

//004bc1c0	->
int CProbabilityItemBag::DropEventItemEx(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0.0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = GetLargeRand() % 1000000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 1000000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[NewPVP] [%s] Drop Zen [%s][%s] [%d Zen]",
			this->GetEventName(), lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumberEx(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	if( btMapNumber == 255 )
	{
		X = 0;
		Y = 0;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isSetItem )
	{
			this->DropEventSetItem(aIndex, iType, btMapNumber, X, Y);
			return true;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption == 1 )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// -----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[NewPVP] [%s] Drop Item [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		this->GetEventName(), lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004bc6e0	->
int CProbabilityItemBag::DropGoldenBoxItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = rand() % 10000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 10000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[TEMP_LOG] DropGoldenBoxItem [%s][%s] [%d Zen]",
			lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumber(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption == 1 )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	if( iType == 7238 || iType == 7239 )
	{
		fDur = 50.0;
	}
	else if( iType == 7253 || iType == 7254 || iType == 7255 )
	{
		fDur = 10.0;
	}
	else if( iType == 7221 )
	{
		fDur = 10.0;
	}
	// ----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[TEMP_LOG] DropGoldenBoxItem [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004bcbd0	->
int CProbabilityItemBag::DropSilverBoxItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0.0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = rand() % 10000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 10000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[TEMP_LOG] DropSilverBoxItem [%s][%s] [%d Zen]",
			lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumber(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption == 1 )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	if( iType == 7238 || iType == 7239 )
	{
		fDur = 5.0;
	}
	else if( iType == 7253 || iType == 7254 || iType == 7255 )
	{
		fDur = 1.0;
	}
	else if( iType == 7221 )
	{
		fDur = 10.0;
	}
	// ----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[TEMP_LOG] DropSilverBoxItem [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004bd0c0	->
int CProbabilityItemBag::DropJewelleryCaseItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0.0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = rand() % 10000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 10000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[TEMP_LOG] DropJewelleryCaseItem [%s][%s] [%d Zen]",
			lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumber(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption == 1 )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	if( iType == 7238 || iType == 7239 )
	{
		fDur = 50.0;
	}
	else if( iType == 7253 || iType == 7254 || iType == 7255 )
	{
		fDur = 10.0;
	}
	else if( iType == 7221 )
	{
		fDur = 10.0;
	}
	// ----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[TEMP_LOG] DropJewelleryCaseItem [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004bd5b0	->
int CProbabilityItemBag::DropNewMonsterItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0.0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = GetLargeRand() % 1000000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 1000000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[%s] DropGroupMonsterItem [%s][%s] [%d Zen]",
			this->GetEventName(), lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumberEx(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isSetItem )
	{
			this->DropEventSetItem(aIndex, iType, btMapNumber, X, Y);
			return true;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption == 1 )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	if( iType == 7238 || iType == 7239 )
	{
		fDur = 50.0;
	}
	else if( iType == 7253 || iType == 7254 || iType == 7255 )
	{
		fDur = 10.0;
	}
	else if( iType == 7221 )
	{
		fDur = 10.0;
	}
	// ----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[%s] [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		this->GetEventName(), lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004bdb10	->
int CProbabilityItemBag::DropBoxOfGRPColorItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{ 
	if( !this->m_bLoad )
	{
		return false;
	}
	// ----
	float fDur			= 0.0;
	int iType			= 0;
	int iLevel			= 0;
	int X				= 0;
	int Y				= 0;
	int iOption1		= 0;
	int iOption2		= 0;
	int iOption3		= 0;
	int nDropItemRate	= 0;
	int nRateCnt		= 0;
	int iExOption		= 0;
	// ----
	LPOBJ lpObj		= &gObj[aIndex];
	// ----
	if( this->GetBagCount() <= 0 )
	{
		return true;
	}
	// ----
	int iDropItemRate = GetLargeRand() % 1000000;
	// ----
	if ( !this->m_iRateKindCount || iDropItemRate < 1000000 - this->m_iEventItemDropRateEx[this->m_iRateKindCount-1] )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, cX, cY);
		LogAddTD("[%s] DropGroupMonsterItem [%s][%s] [%d Zen]",
			this->GetEventName(), lpObj->AccountID, lpObj->Name, this->m_iDropZen);
		// ----
		return true;
	}
	// ----
	int iDropItemNum = this->GetDropItemNumberEx(iDropItemRate);
	// ----
	if( cX || cY )
	{
		X = cX;
		Y = cY;
	}
	else
	{
		X = lpObj->X;
		Y = lpObj->Y;
	}
	// ----
	iLevel	= this->GetLevel(iDropItemNum);
	iType	= ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);
	// ----
	if( iType == -1 )
	{
		return false;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isSetItem )
	{
		this->DropEventSetItem(aIndex, iType, btMapNumber, X, Y);
		return true;
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isskill )
	{
		iOption1 = true;
	}
	// -----
	if( this->BagObject[iDropItemNum].m_isluck )
	{
		iOption2 = 0;
		// ----
		if( !(rand() % 2) )
		{
			iOption2 = true;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isoption == 1 )
	{
		if( rand() % 5 >= 1 )
		{
			iOption3 = rand() % 3;
		}
		else
		{
			iOption3 = 3;
		}
	}
	// ----
	if( this->BagObject[iDropItemNum].m_isexitem )	//fixed
	{
		iExOption	= NewOptionRand(0);
		//iOption2	= false;
		//iOption1	= true;
		//iLevel		= 0;
	}
	// ----
	if( iType == ITEMGET(12, 15) || iType == ITEMGET(14, 13) || iType == ITEMGET(14, 14) )
	{
		iOption1	= false;
		iOption2	= false;
		iOption3	= 0;
		iLevel		= 0;
	}
	// ----
	if( iType == 7238 || iType == 7239 )
	{
		fDur = 50.0;
	}
	else if( iType == 7253 || iType == 7254 || iType == 7255 )
	{
		fDur = 10.0;
	}
	else if( iType == 7221 )
	{
		fDur = 10.0;
	}
	// ----
	ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur, iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);
	LogAddTD("[%s] [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		this->GetEventName(), lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	// ----
	return true;
}
// -------------------------------------------------------------------------------