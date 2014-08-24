// SetItemMacro.cpp: implementation of the CSetItemMacro class.
//	GS-N	1.00.90	JPN	- finished
//	GS-CS	1.00.90	JPN	- finished
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetItemMacro.h"
#include "..\include\readscript.h"
#include "..\common\zzzitem.h"
#include "..\common\SetItemOption.h"
#include "DSProtocol.h"
#include "user.h"
#include "logproc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSetItemMacro::CSetItemMacro()
{

}

CSetItemMacro::~CSetItemMacro()
{

}

BOOL CSetItemMacro::Load(char * szFileName)
{
	SMDFile = fopen(szFileName, "r");	//ok
	if ( SMDFile == NULL )
	{
		LogAdd("¡Ú For BroadCasting Server Set Item Macro Load!!");
		return FALSE;
	}

	SMDToken Token;
	int n = 0;
	char szName[255];
	char szLog[255];
	int iSetIndex;
	int iType;
	int iIndex;
	int iLevel;
	int iSkill;
	int iLuck;
	int iOption;
	int iExOption1;
	int iExOption2;
	int iExOption3;
	int iExOption4;
	int iExOption5;
	int iExOption6;
	int i = 0;
	int iName = 0;

	Token = GetToken();

	while ( true )
	{
		if ( Token == END )
			break;

		if ( Token == NAME )
		{
			strcpy(szName, TokenString);
			sprintf(szLog, "%d - %s\n", iName, szName);

			this->bLoadItemName[iName] = TRUE;
			strcpy(this->sMakeItemName[iName], szName);//changed
			OutputDebugString(szLog);

			Token = GetToken();
			i = 0;

			if ( Token == END )
				break;

			do
			{
				iSetIndex = TokenNumber;

				Token = GetToken();
				iType = TokenNumber;

				Token = GetToken();
				iIndex = TokenNumber;

				Token = GetToken();
				iLevel = TokenNumber;

				Token = GetToken();
				iSkill = TokenNumber;

				Token = GetToken();
				iLuck = TokenNumber;

				Token = GetToken();
				iOption = TokenNumber;

				Token = GetToken();
				iExOption1 = TokenNumber;

				Token = GetToken();
				iExOption2 = TokenNumber;

				Token = GetToken();
				iExOption3 = TokenNumber;

				Token = GetToken();
				iExOption4 = TokenNumber;

				Token = GetToken();
				iExOption5 = TokenNumber;

				Token = GetToken();
				iExOption6 = TokenNumber;
		
				this->SubItemList[iName][i].SetIndex = iSetIndex;
				this->SubItemList[iName][i].ItemIndex = ITEMGET(iType,iIndex);
				this->SubItemList[iName][i].Level = iLevel;
				this->SubItemList[iName][i].Skill = iSkill;
				this->SubItemList[iName][i].Luck = iLuck;
				this->SubItemList[iName][i].Option = iOption;
				this->SubItemList[iName][i].Ex1 = iExOption1;
				this->SubItemList[iName][i].Ex2 = iExOption2;
				this->SubItemList[iName][i].Ex3 = iExOption3;
				this->SubItemList[iName][i].Ex4 = iExOption4;
				this->SubItemList[iName][i].Ex5 = iExOption5;
				this->SubItemList[iName][i].Ex6 = iExOption6;

				wsprintf(szLog, "[%d] %s level:%d skill:%d luck:%d option:%d [%d,%d,%d,%d,%d,%d]\n",
					i, ItemAttribute[ITEMGET(iType,iIndex)].Name, iLevel,iSkill,iLuck,iOption,
					iExOption1,iExOption2,iExOption3,iExOption4,iExOption5,iExOption6);

				OutputDebugString(szLog);
				i++;

				Token = GetToken();
			}
			while ( Token == NUMBER );
	
			iName++;
		}
		else
		{
			LogAdd("¡Ú For BroadCasting Server Set Item Macro Load!!");
			return FALSE;
		}
	}
	fclose(SMDFile);

	return TRUE;
}

BOOL CSetItemMacro::MakeItem(int aIndex, char * szSetName)
{
	BOOL bFindItem = FALSE;

	for(int i = 0; i < MAX_SET_ITEM_MACRO; i++ )
	{
		if( this->bLoadItemName[i] != FALSE )
		{
			if( strcmp(this->sMakeItemName[i],szSetName) == NULL )
			{
				bFindItem = TRUE;

				for( int j = 0; j < 15; j++ )
				{
					if( this->SubItemList[i][j].ItemIndex != NULL )
					{
						int iLevel = 0;//14
						int iSkill = 0;//18
						int iLuck = 0;//1c
						int iOption = 0;//20
						BYTE btNewOption = 0;//24
						int iDur = 0;//28
						int iAddStat = 0;//2c
						int iSetOption = 0;//30

						if( this->SubItemList[i][j].SetIndex != -1 )
						{
							iSkill = this->SubItemList[i][j].Skill;
							iLuck = this->SubItemList[i][j].Luck;
							iOption = this->SubItemList[i][j].Option;
							iLevel = this->SubItemList[i][j].Level;

							btNewOption =	(BYTE)(this->SubItemList[i][j].Ex1<<5);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex2<<4);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex3<<3);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex4<<2);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex5<<1);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex6);

							gSetItemOption.GetSetOptionName(this->SubItemList[i][j].SetIndex);
							iSetOption = 1;
							
							if( rand()%100 < 80 ) 
								iSetOption |= 4;
							else 
								iSetOption |= 8;

							ItemSerialCreateSend(aIndex,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
								this->SubItemList[i][j].ItemIndex,iLevel,iDur,iSkill,iLuck,iOption,-1,btNewOption,iSetOption);
						}
						else
						{
							iSkill = this->SubItemList[i][j].Skill;
							iLuck = this->SubItemList[i][j].Luck;
							iOption = this->SubItemList[i][j].Option;
							iLevel = this->SubItemList[i][j].Level;

							btNewOption =	(BYTE)(this->SubItemList[i][j].Ex1<<5);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex2<<4);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex3<<3);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex4<<2);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex5<<1);
							btNewOption |=	(BYTE)(this->SubItemList[i][j].Ex6);
							
							ItemSerialCreateSend(aIndex,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
								this->SubItemList[i][j].ItemIndex,iLevel,iDur,iSkill,iLuck,iOption,-1,btNewOption,iSetOption);
						}
					}
				}
			}
		}
	}

	return bFindItem;
}
