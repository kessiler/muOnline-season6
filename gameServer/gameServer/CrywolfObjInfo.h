// CrywolfObjInfo.h: interface for the CCrywolfObjInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFOBJINFO_H__2EEB27CF_775F_4F38_AB30_14D63464E751__INCLUDED_)
#define AFX_CRYWOLFOBJINFO_H__2EEB27CF_775F_4F38_AB30_14D63464E751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_CRYWOLF_OBJ_INFO	300

class CCrywolfObjInfo  
{

public:

	CCrywolfObjInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		for ( int i=0;i<MAX_CRYWOLF_OBJ_INFO;i++)
		{
			this->m_iObjIndex[i] = -1;
		}

		this->m_iObjCount = 0;
	}

	BOOL AddObj(int iIndex)
	{
		if ( this->m_iObjCount >= MAX_CRYWOLF_OBJ_INFO )
		{
			return FALSE;
		}

		this->m_iObjIndex[this->m_iObjCount] = iIndex;
		this->m_iObjCount++;
		
		return TRUE;
	}

public:

	int m_iObjCount;	// 0
	int m_iObjIndex[MAX_CRYWOLF_OBJ_INFO];	// 4
};

#endif // !defined(AFX_CRYWOLFOBJINFO_H__2EEB27CF_775F_4F38_AB30_14D63464E751__INCLUDED_)
