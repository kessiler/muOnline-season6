#ifndef TUNIONINFO_H
#define TUNIONINFO_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\TSync.h"

#define MAX_UNION_GUILD 100
#define MAX_UNION_MEMBER	5

class TUnionInfo
{

public:

	TUnionInfo();
	virtual ~TUnionInfo();

	int IsUnionMember(int iGuildNumber);
	int IsRivalMember(int iGuildNumber);
	int SetUnionMaster(int iGuildNumber, char* szGuildName);
	int GetUnionMemberCount();
	int GetRivalMemberCount();
	void Clear(int iRelationShipType);

public:

	int m_iUnionTimeStamp;	// 4
	int m_iRivalTimeStamp;	// 8
	int m_iMasterGuild;	// C
	char m_szMasterGuild[9];	// 10
	std::vector<int> m_vtUnionMember;	// 1C
	std::vector<int> m_vtRivalMember;	// 2C
	TSync m_Sync;	// 3C

};


#endif