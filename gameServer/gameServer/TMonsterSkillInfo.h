// TSkillElement.h: interface for the TMonsterSkillElementInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERSKILLINFO_H__EEDDEFSD_2B9F_409A_B2AA_70024491BEE5__INCLUDED_)
#define AFX_TMONSTERSKILLINFO_H__EEDDEFSD_2B9F_409A_B2AA_70024491BEE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TMonsterSkillUnit.h"

class TMonsterSkillInfo
{

public:

	TMonsterSkillInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		this->m_iMonsterIndex = -1;
		this->m_iSkillUnitCount = 0;

		for ( int i=0;i<10;i++)
		{
			this->m_iSkillUnitTypeArray[i] = -1;
			this->m_lpSkillUnitArray[i] = NULL;
		}
	}

	BOOL IsValid()
	{
		if ( this->m_iMonsterIndex == -1 || this->m_iSkillUnitCount == 0 )
			return FALSE;

		return TRUE;
	}

public:

	int m_iMonsterIndex;	// 0
	int m_iSkillUnitCount;	// 4
	int m_iSkillUnitTypeArray[10];	// 8
	TMonsterSkillUnit* m_lpSkillUnitArray[10];	// 1C
};
// <size 0x30>



#endif // !defined(AFX_TMONSTERSKILLINFO_H__EEDDEFSD_2B9F_409A_B2AA_70024491BEE5__INCLUDED_)
