// RaklionBattleUserMng.h: interface for the CRaklionBattleUserMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONBATTLEUSERMNG_H__4FFACD54_C4F5_4E05_8C5E_E0ECC3D71C1F__INCLUDED_)
#define AFX_RAKLIONBATTLEUSERMNG_H__4FFACD54_C4F5_4E05_8C5E_E0ECC3D71C1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CRaklionBattleUserMng  
{
public:
	CRaklionBattleUserMng();
	virtual ~CRaklionBattleUserMng();
	void ResetAllData();
	int AddUserData(int iIndex);
	int DeleteUserData(int iIndex);
	void CheckUserState();
	int MoveAllUser(int iGateNumber);
	
	//added by hermex
	int GetBattleUserCount();
	int CheckBattleUsers();
	void SetBattleUserIndex(int arg1) ;//006450D0 (identical) 
	int GetBattleUserIndex(int iIndex);
	BOOL IsBattleUser(int iIndex);
	
public:
	std::vector<int> m_vtUserInfo; //0x4?
//	int m_iBattleUserCount;	//0x14
//	int m_iBattleMaxUser; //0x18
};

extern CRaklionBattleUserMng g_RaklionBattleUserMng;

#endif // !defined(AFX_RAKLIONBATTLEUSERMNG_H__4FFACD54_C4F5_4E05_8C5E_E0ECC3D71C1F__INCLUDED_)
