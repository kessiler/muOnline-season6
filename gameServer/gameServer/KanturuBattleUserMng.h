// KanturuBattleUserMng.h: interface for the CKanturuBattleUserMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUBATTLEUSERMNG_H__D86024AB_B505_469B_9529_6A2C9E1C67F4__INCLUDED_)
#define AFX_KANTURUBATTLEUSERMNG_H__D86024AB_B505_469B_9529_6A2C9E1C67F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuBattleUser.h"

#define MAX_KANTURU_BATTLE_USER	15

class CKanturuBattleUserMng
{

public:

	CKanturuBattleUserMng();
	virtual ~CKanturuBattleUserMng();

	void ResetAllData();
	int AddUserData(int iIndex);
	int DeleteUserData(int iIndex);
	void CheckUserState();
	int MoveAllUser(int iGateNumber);
	int GetUserCount();
	int IsEmpty();
	int IsOverMaxUser();
	//int __thiscall IsBattleUser(int);
	void SetMaxUser(int iMaxUser);
	int GetMaxUser();
	void LogBattleWinnerUserInfo(BYTE btFlag, int iElapsedTime);

public:

	CKanturuBattleUser m_BattleUser[MAX_KANTURU_BATTLE_USER];	// 4
	int m_iBattleUserCount;	// B8
	int m_iBattleMaxUser;	// BC

};


extern CKanturuBattleUserMng g_KanturuBattleUserMng;

#endif // !defined(AFX_KANTURUBATTLEUSERMNG_H__D86024AB_B505_469B_9529_6A2C9E1C67F4__INCLUDED_)
