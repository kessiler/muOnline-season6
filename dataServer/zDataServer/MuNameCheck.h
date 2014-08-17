// MuNameCheck.h: interface for the CMuNameCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUNAMECHECK_H__5D16B2BD_5D6D_44E8_B71A_1C571E062A5D__INCLUDED_)
#define AFX_MUNAMECHECK_H__5D16B2BD_5D6D_44E8_B71A_1C571E062A5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MUNAMECHECK_COUNT	2000
#define MAX_MUNAMECHECK_SIZE	20

class CMuNameCheck  
{
public:
	CMuNameCheck();
	virtual ~CMuNameCheck();

	int CmpString(char* cmp_name);
	int NameAdd(char* sz_name);
	int NameLoad(char* filename);
	
	int ItemNameLoad(char*);
	int MonsterNpcLoad(char*);
	int SkillLoad(char*);
	int SkillNameLoad(char*);

public:
	char m_szName[MAX_MUNAMECHECK_COUNT][MAX_MUNAMECHECK_SIZE];
	int m_nMuNameCount;
};

#endif // !defined(AFX_MUNAMECHECK_H__5D16B2BD_5D6D_44E8_B71A_1C571E062A5D__INCLUDED_)
