// RaklionObjInfo.h: interface for the RaklionObjInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONOBJINFO_H__01BB1413_2AEB_47E4_A8BF_9B40BF531572__INCLUDED_)
#define AFX_RAKLIONOBJINFO_H__01BB1413_2AEB_47E4_A8BF_9B40BF531572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_RAKLION_OBJ_INDEX	800
class CRaklionObjInfo  
{
public:
	CRaklionObjInfo() { this->Reset(); }
	virtual ~CRaklionObjInfo() { return; }

	void Reset()
	{
		for ( int iCount=0;iCount<MAX_RAKLION_OBJ_INDEX;iCount++)
		{
			this->m_iObjIndex[iCount] = -1 ;// 0060C2A0;
		}

		this->m_iObjCount = 0;
	}

	BOOL AddObj(int iIndex)
	{
		if ( this->m_iObjCount >= MAX_RAKLION_OBJ_INDEX )
		{
			return FALSE;
		}

		this->m_iObjIndex[this->m_iObjCount] = iIndex;
		this->m_iObjCount++;

		return TRUE;
	}

	int GetCount()
	{
		return this->m_iObjCount;
	}

public:

	int m_iObjCount;	// 4
	int m_iObjIndex[MAX_RAKLION_OBJ_INDEX];	// 8
};

#endif // !defined(AFX_RAKLIONOBJINFO_H__01BB1413_2AEB_47E4_A8BF_9B40BF531572__INCLUDED_)
