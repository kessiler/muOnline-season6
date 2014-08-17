// SetItemOption.cpp: implementation of the CSetItemOption class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetItemOption.h"
#include "ReadScript.h"
#include "zzzitem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSetItemOption	gSetItemOption;

CSetItemOption::CSetItemOption()
{

}

CSetItemOption::~CSetItemOption()
{

}

BOOL CSetItemOption::LoadTypeInfo(char* filename)
{
	int i;
	int Token;

	SMDFile=fopen(filename,"r");

	if (SMDFile == NULL )
	{
		return FALSE;
	}
	memset(m_SetItemTypeInfo , -1, sizeof(m_SetItemTypeInfo) );
	
	for ( i=0; i<MAX_ITEM; i++)
	{
		m_SetItemTypeInfo[i].IsLoad = FALSE;
	}
	
	while ( true )
	{
		int number;
		
		Token=GetToken();

		if (Token == END)
		{
			break;
		}

		if (Token == NUMBER)
		{
			number = (int)TokenNumber;
			
			while ( true )
			{
				int number2;
				LPSETITEM_TYPEINFO pointer;

				Token = GetToken();
				number2 = (int)TokenNumber;

				if ( ( Token == NAME) && (strcmp("end", TokenString) == 0))
				{
					break;
				}

				pointer = &m_SetItemTypeInfo[number*MAX_ITEM_INDEX+number2];

				Token=GetToken();
				pointer->OptionIndex[0] = (int)TokenNumber;
				
				Token=GetToken();
				pointer->OptionIndex[1] = (int)TokenNumber;
				
				Token=GetToken();
				pointer->ChaosMixLevel[0] = (int)TokenNumber;
				
				Token=GetToken();
				pointer->ChaosMixLevel[1] = (int)TokenNumber;

				pointer->IsLoad=TRUE;
			}
		}

	}

	return TRUE;
}

BOOL CSetItemOption::LoadOptionInfo(char* filename)
{
	int i;
	int Token;
	
	SMDFile=fopen(filename, "r");
	if (SMDFile == NULL )
	{
		return 0;
	}
	memset(&m_SetItemOptionInfo, 0, sizeof(m_SetItemOptionInfo));

	for ( i=0;i<80;i++)
	{
		memset(&m_SetItemOptionInfo[i].SetOptionTable[0], -1, sizeof(m_SetItemOptionInfo[0].SetOptionTable) );
		memset(&m_SetItemOptionInfo[i].SetExPOptionTable, -1, sizeof(m_SetItemOptionInfo[0].SetExPOptionTable) );
		memset(&m_SetItemOptionInfo[i].SetFullOtionTable[0], -1, sizeof(m_SetItemOptionInfo[0].SetFullOtionTable) );
	}

	while ( true )
	{
		Token=GetToken();
		int number= (int)TokenNumber;
		
		if (Token==0 && strcmp("end", TokenString) == 0)
		{
			break;
		}

		LPSETITEM_OPTIONINFO p;
		p=&m_SetItemOptionInfo[number];

		Token=GetToken();
		strcpy(p->Name, TokenString);

		for (i=0;i<6;i++)
		{
			Token=GetToken();
			p->SetOptionTable[i][0] = (int)TokenNumber;

			Token=GetToken();
			p->SetOptionTableValue[i][0] =(int)TokenNumber;

			Token=GetToken();
			p->SetOptionTable[i][1] = (int)TokenNumber;

			Token=GetToken();
			p->SetOptionTableValue[i][1] =(int)TokenNumber;

			if ( (p->SetOptionTable[i][0] != -1) || (p->SetOptionTable[i][1] != -1) )
			{
				p->SetOptionCount++;
			}
		}
		
		Token=GetToken();
		p->SetExPOptionTable[0]  = (int)TokenNumber;

		Token=GetToken();
		p->SetExPOptionTableValue[0] = (int)TokenNumber;

		if ( p->SetExPOptionTable[0] != -1)
		{
			p->SetOptionCount ++;
		}

		Token=GetToken();
		p->SetExPOptionTable[1]  = (int)TokenNumber;
		Token=GetToken();
		p->SetExPOptionTableValue[1] = (int)TokenNumber;

		if ( p->SetExPOptionTable[1] != -1)
		{
			p->SetOptionCount ++;
		}
		
		Token=GetToken();
		p->SetFullOtionTable[0]=(int)TokenNumber;

		Token=GetToken();
		p->SetFullOtionTableValue[0]=(int)TokenNumber;

		Token=GetToken();
		p->SetFullOtionTable[1]=(int)TokenNumber;

		Token=GetToken();
		p->SetFullOtionTableValue[1]=(int)TokenNumber;

		Token=GetToken();
		p->SetFullOtionTable[2]=(int)TokenNumber;

		Token=GetToken();
		p->SetFullOtionTableValue[2]=(int)TokenNumber;

		Token=GetToken();
		p->SetFullOtionTable[3]=(int)TokenNumber;

		Token=GetToken();
		p->SetFullOtionTableValue[3]=(int)TokenNumber;

		Token=GetToken();
		p->SetFullOtionTable[4]=(int)TokenNumber;

		Token=GetToken();
		p->SetFullOtionTableValue[4]=(int)TokenNumber;

		Token=GetToken();
		p->RequireClass[CLASS_WIZARD]=(int)TokenNumber;

		Token=GetToken();
		p->RequireClass[CLASS_KNIGHT]=(int)TokenNumber;

		Token=GetToken();
		p->RequireClass[CLASS_ELF]=(int)TokenNumber;

		Token=GetToken();
		p->RequireClass[CLASS_MAGUMSA]=(int)TokenNumber;

		Token=GetToken();
		p->RequireClass[CLASS_DARKLORD]=(int)TokenNumber;

		Token=GetToken();
		p->RequireClass[CLASS_SUMMONER]=(int)TokenNumber;
		
		Token=GetToken();
		p->RequireClass[CLASS_MONK]=(int)TokenNumber;

		p->IsLoad=TRUE;
	}

	return 1;
}

int CSetItemOption::IsSetItem(int itemnum)	
{
	if (itemnum<0 || itemnum > MAX_ITEM-1)
	{
		gWindow.PrintLog("IsSetItem Check error: (itemnum:%d)", itemnum);
		return 0;
	}

	return m_SetItemTypeInfo[itemnum].IsLoad ;
}

int CSetItemOption::GetOptionTableIndex(int itemnum, int setoption)	
{
	if (itemnum<0 || itemnum > MAX_ITEM-1)
	{
		gWindow.PrintLog("GetOptionTableIndex Check error: (itemnum:%d)", itemnum);
		return -1;
	}

	if (setoption != 1 && setoption != 2)
	{
		gWindow.PrintLog("GetOptionTableIndex setoption Check error: (itemnum:%d)", itemnum);
		return -1;
	}

	if (m_SetItemTypeInfo[itemnum].IsLoad == FALSE)
	{
		return -1;
	}

	return m_SetItemTypeInfo[itemnum].OptionIndex[setoption-1];
}

char * CSetItemOption::GetSetOptionName(int optionindex)
{
	if ( optionindex < 0 || optionindex > 79)
	{
		gWindow.PrintLog("GetSetOptionName Check error: (itemnum:%d)", optionindex);
		return "";
	}
	if (m_SetItemOptionInfo[optionindex].IsLoad   ==0)
	{
		return "";
	}
	return &m_SetItemOptionInfo[optionindex].Name[0];
}
	
char * CSetItemOption::GetSetOptionName(int itemnum, int settype)
{
	int optionindex;

	if (itemnum<0 || itemnum > MAX_ITEM-1)
	{
		gWindow.PrintLog("GetSetOptionName Check error: (itemnum:%d)", itemnum);
		return "";
	}

	if (m_SetItemTypeInfo[itemnum].IsLoad == FALSE)
	{
		return "";
	}
	
	optionindex= m_SetItemTypeInfo[itemnum].OptionIndex[settype-1];
	
	if (optionindex<0 || optionindex > 79)
	{	
		gWindow.PrintLog("GetSetOptionName Check error: (itemnum:%d)", optionindex);
		return "";
	}

	if (m_SetItemOptionInfo[optionindex].IsLoad  == 0)
	{
		return "";
	}

	return &m_SetItemOptionInfo[optionindex].Name[0];
}

int CSetItemOption::GetSetOption(int optionindex, int optablenum, int& op1, int& op2, int& opvalue1,int& opvalue2, int Class, int ChangeUp)
{
	opvalue1=0;
	opvalue2=0;

	if (optionindex <0 || optionindex > 79)
	{
		gWindow.PrintLog("GetSetOption Check error: (itemnum:%d)", optionindex);
		return 0;
	}

	if (m_SetItemOptionInfo[optionindex].IsLoad==0)
	{
		return 0;
	}

	if (optablenum >= m_SetItemOptionInfo[optionindex].SetOptionCount)
	{
		return 0;
	}
	int isRequireClass;

	isRequireClass=IsRequireClass(optionindex, Class, ChangeUp);

	if (optablenum <6)
	{
		op1=m_SetItemOptionInfo[optionindex].SetOptionTable[optablenum][0];
		opvalue1=m_SetItemOptionInfo[optionindex].SetOptionTableValue[optablenum][0];

		if ( op1 > 183 )
		{
			if ( isRequireClass == 0 )
			{
				op1 = -1;
			}
		}

		if ( op1 != -1 )
		{
			op1 += 0xA0;
		}

		op2=m_SetItemOptionInfo[optionindex].SetOptionTable[optablenum][1];
		opvalue2=m_SetItemOptionInfo[optionindex].SetOptionTableValue[optablenum][1];

		if ( op2 > 183 )
		{
			if ( isRequireClass == 0 )
			{
				op2 = -1;
			}
		}

		if ( op2 != -1 )
		{
			op2 += 0xA0;
		}
	}
	else
	{
		op1=m_SetItemOptionInfo[optionindex].SetExPOptionTable[optablenum-6];
		opvalue1=m_SetItemOptionInfo[optionindex].SetExPOptionTableValue[optablenum-6];

		if ( op1 > 183 )
		{
			if ( isRequireClass == 0 )
			{
				op1 = -1;
			}
		}

		if ( op1 != -1 )
		{
			op1 += 0xA0;
		}

		op2 = -1;
	}

	return 1;
}

int CSetItemOption::GetMaxSetOptionCount(int optionindex)
{
	if (optionindex <0 || optionindex > 79)
	{
		gWindow.PrintLog("GetMaxOptionCount Check error: (itemnum:%d)", optionindex);
		return -1;
	}

	if (m_SetItemOptionInfo[optionindex].IsLoad == 0)
	{
		return -1;
	}

	return m_SetItemOptionInfo[optionindex].SetOptionCount;
}

int CSetItemOption::GetGetFullSetOption(int optionindex, int& op1, int& op2, int& op3, int& op4, int& op5, int& opvalue1, int& opvalue2, int& opvalue3, int& opvalue4, int& opvalue5, int Class, int ChangeUp)
{
	opvalue1 = 0;
	opvalue2 = 0;
	opvalue3 = 0;
	opvalue4 = 0;
	opvalue5 = 0;

	if (optionindex <0 || optionindex > 79)
	{
		gWindow.PrintLog("GetMaxOptionCount Check error: (itemnum:%d)", optionindex);
		return 0;
	}

	if (m_SetItemOptionInfo[optionindex].IsLoad == FALSE)
	{
		return 0;
	}

	BOOL isRequireClass=IsRequireClass(optionindex, Class, ChangeUp);
	op1 = m_SetItemOptionInfo[optionindex].SetFullOtionTable[0];
	opvalue1 = m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[0];

	if ( op1 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op1 = -1;
		}
	}

	if ( op1 != -1 )
	{
		op1 += 0xA0;
	}

	op2=m_SetItemOptionInfo[optionindex].SetFullOtionTable[1];
	opvalue2=m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[1];

	if ( op2 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op2 = -1;
		}
	}

	if ( op2 != -1 )
	{
		op2 += 0xA0;
	}

	op3=m_SetItemOptionInfo[optionindex].SetFullOtionTable[2];
	opvalue3=m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[2];

	if ( op3 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op3 = -1;
		}
	}

	if ( op3 != -1 )
	{
		op3 += 0xA0;
	}

	op4=m_SetItemOptionInfo[optionindex].SetFullOtionTable[3];
	opvalue4=m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[3];

	if ( op4 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op4 = -1;
		}
	}

	if ( op4 != -1 )
	{
		op4 += 0xA0;
	}

	op5=m_SetItemOptionInfo[optionindex].SetFullOtionTable[4];
	opvalue5=m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[4];

	if ( op5 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op5 = -1;
		}
	}

	if ( op5 != -1 )
	{
		op5 += 0xA0;
	}

	return 1;
}

BOOL CSetItemOption::IsRequireClass(int optionindex, int Class, int ChangeUP)
{
	int requireclass;
	
	if (optionindex < 0 || optionindex > 79)
	{
		gWindow.PrintLog("GetMaxOptionCount Check error: (itemnum:%d)", optionindex);
		return 0;
	}

	if (m_SetItemOptionInfo[optionindex].IsLoad == FALSE)
	{
		return 0;
	}

	if ( Class <0 || Class > MAX_CLASSTYPE-1)
	{
		return 0;
	}

	requireclass= m_SetItemOptionInfo[optionindex].RequireClass[Class];

	if (requireclass == 0)
	{
		return 0;
	}

	if (requireclass > 1)
	{
		if (requireclass != ChangeUP+1)
		{
			return 0;
		}
	}
	return 1;
}

int CSetItemOption::CheckMixContition(int itemnum, int mixitemlevel)
{
	if (mixitemlevel == -1)
	{
		return 0;
	}

	if ( itemnum <0 || itemnum > MAX_ITEM-1)
	{
		gWindow.PrintLog("MakeSetOption Check error: (itemnum:%d)", itemnum);
		return 0;
	}

	if (m_SetItemTypeInfo[itemnum].IsLoad == 0)
	{
		return 0;
	}

	if (m_SetItemTypeInfo[itemnum].ChaosMixLevel[0] == mixitemlevel || m_SetItemTypeInfo[itemnum].ChaosMixLevel[1] == mixitemlevel)
	{
		return 1;
	}

	return 0;
}

int CSetItemOption::MakeSetOption(int itemnum, int mixitemlevel)
{
	if ( itemnum <0 || itemnum > MAX_ITEM-1)
	{
		gWindow.PrintLog("MakeSetOption Check error: (itemnum:%d)", itemnum);
		return 0;
	}

	if (m_SetItemTypeInfo[itemnum].IsLoad == FALSE)
	{
		return 0;
	}

	if (m_SetItemTypeInfo[itemnum].ChaosMixLevel[0] == m_SetItemTypeInfo[itemnum].ChaosMixLevel[1])
	{
		return (rand() % 2) +1;
	}
	else if ( m_SetItemTypeInfo[itemnum].ChaosMixLevel[0] == mixitemlevel )
	{
		return 1;
	}
	else if ( m_SetItemTypeInfo[itemnum].ChaosMixLevel[1] == mixitemlevel )
	{
		return 2;
	}

	return 0;
}

int CSetItemOption::GenRandomItemNum()
{
	int iItemIndex;

	do
	{
		int type = (rand()%MAX_ITEM_TYPE);
		int maxindex = ( g_MaxItemIndexOfEachItemType[type] <= 0 ) ?  MAX_ITEM_INDEX : g_MaxItemIndexOfEachItemType[type];
		int index = rand() % maxindex;
		iItemIndex = ((type*MAX_ITEM_INDEX)+index);
		
		if ( IsSetItem(iItemIndex) != FALSE )
		{
			return iItemIndex;
		}
	}
	while ( true );

	return 0;
}

int CSetItemOption::GenSetOption(int itemnum)
{
	if ( itemnum < 0 || itemnum > MAX_ITEM-1)
	{
		gWindow.PrintLog("GenSetType Check error: (itemnum:%d)", itemnum);
		return FALSE;
	}

	if ( m_SetItemTypeInfo[itemnum].IsLoad == FALSE )
	{
		return FALSE;
	}

	int SetOption;

	if ( m_SetItemTypeInfo[itemnum].OptionIndex[0] && m_SetItemTypeInfo[itemnum].OptionIndex[1] )
	{
		SetOption = rand() % 2 + 1;
	}
	else if ( m_SetItemTypeInfo[itemnum].OptionIndex[0] )
	{
		SetOption = 1;
	}
	else
	{
		SetOption = 2;
	}

	if ( (rand()%100) < 80 )
	{
		SetOption |= 4;
	}
	else
	{
		SetOption |= 8;
	}

	return SetOption;
}