// PetDBSet.h: interface for the CPetDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PETDBSET_H__7E383225_7C60_45A4_8748_60FDF2C22CC4__INCLUDED_)
#define AFX_PETDBSET_H__7E383225_7C60_45A4_8748_60FDF2C22CC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprodef.h"

#include "DBConBase.h"

class CPetDBSet  : public CDBConBase
{
public:
	CPetDBSet();
	virtual ~CPetDBSet();

	BOOL Connect();
	BOOL LoadPetInfo(DWORD number, int& Level, int& Exp);
	BOOL SavePetInfo(DWORD number, int Level, int Exp);
	BOOL CreatePetItemInfo(DWORD serial, int Level, int Exp);
};

#endif // !defined(AFX_PETDBSET_H__7E383225_7C60_45A4_8748_60FDF2C22CC4__INCLUDED_)
