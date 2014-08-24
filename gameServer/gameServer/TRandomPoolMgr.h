// TRandomPoolMgr.h: interface for the TRandomPoolMgr class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_TRANDOMPOOLMGR_H__0C5848DA_9287_401A_8703_05D713CA4A05__INCLUDED_
#define AFX_TRANDOMPOOLMGR_H__0C5848DA_9287_401A_8703_05D713CA4A05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum eRandomPoolSelectType
{
	eRANDOMPOOL_BY_RANDOM = 0x0,
	eRANDOMPOOL_BY_WEIGHT = 0x1
};



class TRandomPoolMgr
{
public:

	TRandomPoolMgr();
	virtual ~TRandomPoolMgr();

	void InitPool();
	bool AddValue(int index, int weight);
	int GetRandomValue(enum eRandomPoolSelectType eSelectType);

private:

	std::map<int,int> m_listPool;	// 4
	int m_iSumWeight;	// 14
};

#endif // !defined(AFX_TRANDOMPOOLMGR_H__0C5848DA_9287_401A_8703_05D713CA4A05__INCLUDED_)
