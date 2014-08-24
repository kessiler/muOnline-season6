#ifndef TUNION_H
#define TUNION_H



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "..\common\TSync.h"
#include "TUnionInfo.h"

class TUnion
{

public:

	TUnion();
	virtual ~TUnion();

	int GetGuildRelationShipCount(int iGuildNumber, int iRelationShipType);
	int GetGuildRelationShip(int iGuildNumber, int iTargetGuildNumber);
	int SetGuildUnionMemberList(int iMasterGuildNumber, int iCount, int const * iList);
	int SetGuildRivalMemberList(int iMasterGuildNumber, int iCount, int const * iList);
	int GetGuildUnionMemberList(int iMasterGuildNumber, int& iCount, int * iList);
	int GetUnionName(int iMasterGuildNumber, char* szMasterGuildName);
	int AddUnion(int iMasterGuildNumber, char* szUnionMasterGuildName);
	int DelUnion(int iMasterGuildNumber);
	int DelAllUnion();
	TUnionInfo* SearchUnion(int iMasterGuildNumber);

private:

	std::map<int, TUnionInfo *>	m_mpUnion;	// 4
	TSync m_Sync;	// 14

};

extern TUnion UnionManager;

#endif