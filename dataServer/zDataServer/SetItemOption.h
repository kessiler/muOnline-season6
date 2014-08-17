// SetItemOption.h: interface for the CSetItemOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETITEMOPTION_H__CB7F077C_7433_4F1D_AD2A_0BC7017B3614__INCLUDED_)
#define AFX_SETITEMOPTION_H__CB7F077C_7433_4F1D_AD2A_0BC7017B3614__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItemDef.h"

typedef struct
{
	BOOL IsLoad;
	int OptionIndex[2];
	int ChaosMixLevel[2];
}SETITEM_TYPEINFO, *LPSETITEM_TYPEINFO;

typedef struct
{
	BOOL IsLoad;
	char Name[255];
	int SetOptionCount;
	int SetOptionTable[6][2];
	int SetOptionTableValue[6][2];
	int SetExPOptionTable[2];
	int SetExPOptionTableValue[2];
	int SetFullOtionTable[5];
	int SetFullOtionTableValue[5];
	BYTE RequireClass[MAX_CLASSTYPE+1];
}SETITEM_OPTIONINFO, *LPSETITEM_OPTIONINFO;

class CSetItemOption  
{
public:
	CSetItemOption();
	virtual ~CSetItemOption();

	BOOL LoadTypeInfo(char* filename);
	int LoadOptionInfo(char* filename);
	int IsSetItem(int itemnum);
	int GetOptionTableIndex(int itemnum, int setoption);
	char* GetSetOptionName(int itemnum, int settype);
	char* GetSetOptionName(int optionindex);
	int GetSetOption(int optionindex, int optablenum, int& op1, int& op2, int& opvalue1, int& opvalue2, int Class, int ChangeUp);
	int GetMaxSetOptionCount(int optionindex);
	int GetGetFullSetOption(int optionindex, int& op1, int& op2, int& op3, int& op4, int& op5, int& opvalue1, int& opvalue2, int& opvalue3, int& opvalue4, int& opvalue5, int Class, int ChangeUp);
	int IsRequireClass(int optionindex, int Class, int ChangeUp);
	int CheckMixContition(int itemnum, int mixitemlevel);
	int MakeSetOption(int itemnum, int mixitemlevel);
	int GenRandomItemNum();
	int GenSetOption(int itemnum);
	LPSETITEM_TYPEINFO GetSetType(int);

public:
	SETITEM_TYPEINFO m_SetItemTypeInfo[MAX_ITEM];
	SETITEM_OPTIONINFO m_SetItemOptionInfo[80];
};

extern CSetItemOption	gSetItemOption;

#endif // !defined(AFX_SETITEMOPTION_H__CB7F077C_7433_4F1D_AD2A_0BC7017B3614__INCLUDED_)
