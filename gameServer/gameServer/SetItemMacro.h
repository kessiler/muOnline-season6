// SetItemMacro.h: interface for the CSetItemMacro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETITEMMACRO_H__068E3238_E18F_4890_AB7D_8E7E63BD3F91__INCLUDED_)
#define AFX_SETITEMMACRO_H__068E3238_E18F_4890_AB7D_8E7E63BD3F91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_SET_ITEM_MACRO	50

struct SETITEMLIST
{
	int SetIndex;	// 0
	int ItemIndex;	// 4
	int Level;	// 8
	int Skill;	// C
	int Luck;	// 10
	int Option;	// 14
	int Ex1;	// 18
	int Ex2;	// 1C
	int Ex3;	// 20
	int Ex4;	// 24
	int Ex5;	// 28
	int Ex6;	// 2C
};


class CSetItemMacro  
{

public:

	CSetItemMacro();
	~CSetItemMacro();

	BOOL Load(char* szFileName);
	BOOL MakeItem(int aIndex, char* szSetName);

private:

  SETITEMLIST SubItemList[MAX_SET_ITEM_MACRO][15];	// 0
  BOOL bLoadItemName[MAX_SET_ITEM_MACRO];	// 8CA0
  char sMakeItemName[MAX_SET_ITEM_MACRO][255];	// 8D68

};

#endif // !defined(AFX_SETITEMMACRO_H__068E3238_E18F_4890_AB7D_8E7E63BD3F91__INCLUDED_)
