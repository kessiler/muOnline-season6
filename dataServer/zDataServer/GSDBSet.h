// GSDBSet.h: interface for the CGSDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GSDBSET_H__22255CC2_C2B4_44DB_A166_C1F57E107D79__INCLUDED_)
#define AFX_GSDBSET_H__22255CC2_C2B4_44DB_A166_C1F57E107D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprodef.h"
#include "CQuery.h"
class char_ID
{
public:
	char_ID(char* szName)
	{
		memset(Name, 0, 11);
		if(szName != NULL)	memcpy(Name, szName, 10);
	};

	char* GetBuffer()
	{
		return (char*)this;
	};

public:
	char Name[MAX_IDSTRING+1];	
};

typedef struct
{
	PBMSG_HEAD h;
	BYTE x;
	BYTE y;
	BYTE MapNumber;
	short Type;
	BYTE Level;
	BYTE Dur;
	BYTE Op1;
	BYTE Op2;
	BYTE Op3;
	BYTE NewOption;
	int aIndex;
	short lootindex;
	BYTE SetOption;
	DWORD dwTime;
	WORD wRemainTime;
	WORD wLevel;
	WORD wLevel_T;
	char szName[MAX_IDSTRING];
	char szName_T[MAX_IDSTRING];
	BYTE btClass;
	BYTE btClass_T;
	WORD wItemIndex;
}SDHP_PROPERTY_ITEMCREATE, *LPSDHP_PROPERTY_ITEMCREATE;

struct SDHP_LOAD_MEDALINFO_RESULT
{
	PBMSG_HEAD h;
	WORD wIndex;
	DWORD dwItemSerial;
	DWORD dwTime;
	WORD wRemainTime;
	WORD wLevel;
	WORD wLevel_T;
	char szName[MAX_IDSTRING];
	char szName_T[MAX_IDSTRING];
	BYTE btClass;
	BYTE btClass_T;
	WORD wItemIndex;
};

class CGSDBSet  
{
public:
	CQuery	m_Query;

	
	CGSDBSet();
	virtual ~CGSDBSet();
	
	BOOL	Connect();

	DWORD	GetCount();
	BOOL	SetCount(int count);
	BOOL	CreateZen();
	BOOL	CreateItem(DWORD& sirial_number);
	BOOL	CreatePropertyItemInfo(DWORD serial, SDHP_PROPERTY_ITEMCREATE* lpMsg);
	int		GetPropertyItemInfo(DWORD serial, SDHP_LOAD_MEDALINFO_RESULT* lpMsg);
	int		GetItemSerial(DWORD& sirial_number, int MakeSeiralCount);
	int		CheckMuDBIdentification();
};

#endif // !defined(AFX_GSDBSET_H__22255CC2_C2B4_44DB_A166_C1F57E107D79__INCLUDED_)
