// TMonsterAIGroup.h: interface for the TMonsterAIGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERAIGROUP_H__4F35CBBB_1AB6_46C7_8622_F4DE25E7149F__INCLUDED_)
#define AFX_TMONSTERAIGROUP_H__4F35CBBB_1AB6_46C7_8622_F4DE25E7149F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TMonsterAIGroupMember.h"

#define MAX_MONSTER_AI_GROUP		(50)
#define MAX_MONSTER_AI_GROUP_MEMBER	(100)

class TMonsterAIGroup  
{

public:

	TMonsterAIGroup();
	virtual ~TMonsterAIGroup();

	static void Init(int iGroupNumber);
	static void InitAll();
	static BOOL LoadData(char* lpszFileName);
	static BOOL DelAllGroupInfo();
	static BOOL DelGroupInstance(int iGroupNumber);
	static void ChangeAIOrder(int iGroupNumber, int iAIOrder);
	static TMonsterAIGroupMember* FindGroupMember(int iGroupNumber, int iGuid);
	static TMonsterAIGroupMember* FindGroupMemberToSommon(int iObjIndex, int iGroupNumber, int iGuid);
	static TMonsterAIGroupMember* FindGroupMemberToHeal(int iObjIndex, int iGroupNumber, int iGuid, int iDistance);
	static int FindGroupLeader(int iGroupNumber);
	static int FindGroupMemberObjectIndex(int iGroupNumber, int iGuid);
	static BOOL s_bDataLoad;
	static TMonsterAIGroupMember s_MonsterAIGroupMemberArray[MAX_MONSTER_AI_GROUP][MAX_MONSTER_AI_GROUP_MEMBER];
	static int s_iMonsterAIGroupMemberCount[MAX_MONSTER_AI_GROUP];

private:


};

#endif // !defined(AFX_TMONSTERAIGROUP_H__4F35CBBB_1AB6_46C7_8622_F4DE25E7149F__INCLUDED_)
