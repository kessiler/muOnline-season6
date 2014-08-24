// TRandomPoolMgr.cpp: implementation of the TRandomPoolMgr class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TRandomPoolMgr.h"
#include "LargeRand.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TRandomPoolMgr::TRandomPoolMgr()
{
	this->InitPool();
}



TRandomPoolMgr::~TRandomPoolMgr()
{
	return;
}



void TRandomPoolMgr::InitPool()
{
	this->m_listPool.clear();
	this->m_iSumWeight = 0;
}




bool TRandomPoolMgr::AddValue(int index, int weight)
{
	bool bRet = false;

	if ( index < 0 )
		return bRet;

	std::map<int,int>::iterator iter = this->m_listPool.find(index);

	if ( iter == this->m_listPool.end() )
	{
		this->m_listPool[index] = weight;
		bRet = true;
		this->m_iSumWeight += weight;
	}

	return bRet;
}



int TRandomPoolMgr::GetRandomValue(enum eRandomPoolSelectType eSelectType)
{
	int iReturnValue = 0;
	int iPoolSize = this->m_listPool.size();

	if ( iPoolSize == 0 )
		return iReturnValue;

	if ( this->m_iSumWeight <= 0 )
		eSelectType = eRANDOMPOOL_BY_RANDOM;

	if ( eSelectType == eRANDOMPOOL_BY_RANDOM )
	{
		int iPos = GetLargeRand()% (DWORD)iPoolSize;
		int iCount = 0;
		
		for ( std::map<int, int>::iterator iter = this->m_listPool.begin() ; iter != this->m_listPool.end() ; ++iter)
		{
			if ( iPos == iCount )
			{
				iReturnValue = iter->first;
				break;
			}
		}
	}
	else if ( eSelectType == eRANDOMPOOL_BY_WEIGHT )
	{
		int iRandom = GetLargeRand() % (DWORD)this->m_iSumWeight;
		int iCurRate = 0;

		for (std::map<int, int>::iterator iter = this->m_listPool.begin() ; iter != this->m_listPool.end() ; ++iter)
		{
			int iRate = iter->second;
			iCurRate += iRate;

			if ( iCurRate >= iRandom )
			{
				iReturnValue = iter->first;
				break;
			}
		}
	}

	return iReturnValue;
}