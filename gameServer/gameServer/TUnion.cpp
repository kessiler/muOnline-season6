//	GS-N	1.00.90	JPN			-	Completed
//	GS-CS	1.00.90	JPN			-	Completed
#include "stdafx.h"
#include "TUnion.h"
#include "TUnionInfo.h"

TUnion UnionManager;


TUnion::TUnion()
{
	return;
}

TUnion::~TUnion()
{
	return;
}

int TUnion::DelAllUnion()
{
	this->m_Sync.Lock();
	std::map<int, TUnionInfo*>::iterator _PosItor;
		
	
	for ( _PosItor = this->m_mpUnion.begin() ; _PosItor != this->m_mpUnion.end() ; ++_PosItor )
	{
		delete _PosItor->second;
	}

	this->m_mpUnion.clear();
	this->m_Sync.Unlock();
	return -1;
}


int TUnion::GetGuildRelationShip(int iGuildNumber, int iTargetGuildNumber)
{
	TUnionInfo * pUnionInfo = this->SearchUnion(iGuildNumber);

	if ( pUnionInfo == NULL )
	{
		return 0;
	}

	if ( pUnionInfo->IsUnionMember(iTargetGuildNumber) != FALSE )
	{
		return 1;
	}

	if ( pUnionInfo->IsRivalMember(iTargetGuildNumber) != FALSE )
	{
		return 2;
	}
	
	return 0;
}


int TUnion::GetGuildRelationShipCount(int iGuildNumber, int iRelationShipType)
{
	TUnionInfo * pUnionInfo = this->SearchUnion(iGuildNumber);

	if ( pUnionInfo == NULL )
	{
		return 0;
	}

	if ( iRelationShipType == 1 )
	{
		return pUnionInfo->GetUnionMemberCount();
	}
	else if ( iRelationShipType == 2 )
	{
		return pUnionInfo->GetRivalMemberCount();
	}
	return 0;
}

TUnionInfo * TUnion::SearchUnion(int iMasterGuildNumber)
{
	this->m_Sync.Lock();

	std::map<int, TUnionInfo *>::iterator _Itor = this->m_mpUnion.find(iMasterGuildNumber);

	if ( _Itor != this->m_mpUnion.end() )
	{
		this->m_Sync.Unlock();
		return (*(_Itor)).second;
	}

	this->m_Sync.Unlock();
	return NULL;
}

BOOL TUnion::AddUnion(int iMasterGuildNumber, char* szUnionMasterGuildName)
{
	TUnionInfo * pUnionInfo = this->SearchUnion(iMasterGuildNumber);

	if ( pUnionInfo == NULL )
	{
		this->m_Sync.Lock();
		pUnionInfo = new TUnionInfo;
		pUnionInfo->SetUnionMaster(iMasterGuildNumber, szUnionMasterGuildName);
		this->m_mpUnion[iMasterGuildNumber] = pUnionInfo;
		this->m_Sync.Unlock();
		return TRUE;
	}

	this->m_Sync.Lock();
	pUnionInfo->SetUnionMaster(iMasterGuildNumber, szUnionMasterGuildName);
	this->m_Sync.Unlock();
	return FALSE;
}


int TUnion::DelUnion(int iMasterGuildNumber)
{

	TUnionInfo * pUnionInfo = this->SearchUnion(iMasterGuildNumber);

	if ( pUnionInfo != NULL )
	{
		this->m_Sync.Lock();
		this->m_mpUnion.erase(iMasterGuildNumber);
		delete pUnionInfo;
		this->m_Sync.Unlock();
		return TRUE;
	}

	return -1;
}


int TUnion::SetGuildUnionMemberList(int iMasterGuildNumber, int iCount, int const * iList)
{
	TUnionInfo * pUnionInfo = this->SearchUnion(iMasterGuildNumber);

	if ( pUnionInfo != NULL )
	{
		pUnionInfo->Clear(1);
		this->m_Sync.Lock();

		for ( int i=0;i<iCount;i++)
		{
			pUnionInfo->m_vtUnionMember.push_back(iList[i]);
		}

		this->m_Sync.Unlock();
		return TRUE;
	}

	return -1;
}


int TUnion::SetGuildRivalMemberList(int iMasterGuildNumber, int iCount, int const * iList)
{
	TUnionInfo * pUnionInfo = this->SearchUnion(iMasterGuildNumber);

	if ( pUnionInfo != NULL )
	{
		pUnionInfo->Clear(2);
		this->m_Sync.Lock();

		for ( int i=0;i<iCount;i++)
		{
			pUnionInfo->m_vtRivalMember.push_back(iList[i]);
		}

		this->m_Sync.Unlock();
		return TRUE;
	}

	return -1;
}

int TUnion::GetGuildUnionMemberList(int iMasterGuildNumber, int & iCount, int * iList)
{

	TUnionInfo * pUnionInfo = this->SearchUnion(iMasterGuildNumber);

	if ( pUnionInfo != NULL )
	{
		this->m_Sync.Lock();
		int * iB = &*pUnionInfo->m_vtUnionMember.begin();
		int * iE = &*pUnionInfo->m_vtUnionMember.end();
		
		for ( ; iB != iE ; iB++ )
		{
			iList[iCount] = *iB;
			iCount++;
		}

		this->m_Sync.Unlock();

		if ( iCount > 0 &&  iCount < 100 )
		{
			return TRUE;
		}
	}

	return -1;
}


BOOL TUnion::GetUnionName(int iMasterGuildNumber, char* szMasterGuildName)
{
	TUnionInfo * pUnionInfo = this->SearchUnion(iMasterGuildNumber);

	if ( pUnionInfo != NULL )
	{
		this->m_Sync.Lock();
		memcpy(szMasterGuildName, pUnionInfo->m_szMasterGuild, sizeof(pUnionInfo->m_szMasterGuild)-1);
		this->m_Sync.Unlock();
		return TRUE;
	}

	return -1;
}