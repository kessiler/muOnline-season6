// Raklion.h: interface for the CRaklion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLION_H__CCDE5834_0D07_423B_B33A_2D9554F8A789__INCLUDED_)
#define AFX_RAKLION_H__CCDE5834_0D07_423B_B33A_2D9554F8A789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RaklionBattleOfSelupan.h"


#define MAX_RAKLION_APPEARANCE_DELAY		10
#define MAX_RAKLION_BOSSZONECLOSE_DELAY		500
#define MAX_RAKLION_BOSSZONEOPEN_DELAY		21600 * 7// 500

#define RAKLION_STATE_IDLE					0
#define RAKLION_STATE_NOTIFY_1				1
#define RAKLION_STATE_STANDBY				2
#define RAKLION_STATE_NOTIFY_2				3
#define RAKLION_STATE_READY					4
#define RAKLION_STATE_START_BATTLE			5
#define RAKLION_STATE_NOTIFY_3				6
#define RAKLION_STATE_CLOSE_DOOR			7
#define RAKLION_STATE_ALL_USER_DIE			8
#define RAKLION_STATE_NOTIFY_4				9
#define RAKLION_STATE_END					10

class CRaklion  
{
public:
	CRaklion();
	virtual ~CRaklion();
	void Init();
	void ClearData();
	BOOL LoadData(char *szFileName);
	void CheckData();
	void Run();
	void SetState(int iRaklionState);
	void SetNextState(int iState);
	void ChangeState(int iState, int iDetailState);
	void SetState_IDLE();
	void SetState_NOTIFY_1();
	void SetState_STANDBY();
	void SetState_NOTIFY_2();
	void SetState_READY();
	void SetState_START_BATTLE();
	void SetState_NOTIFY_3();
	void SetState_CLOSE_DOOR();
	void SetState_ALL_USER_DIE();
	void SetState_NOTIFY_4();
	void SetState_END();
	void ProcState_IDLE();
	void ProcState_NOTIFY_1();
	void ProcState_STANDBY();
	void ProcState_NOTIFY_2();
	void ProcState_READY();
	void ProcState_START_BATTLE();
	void ProcState_NOTIFY_3();
	void ProcState_CLOSE_DOOR();
	void ProcState_ALL_USER_DIE();
	void ProcState_NOTIFY_4();
	void ProcState_END();
	void SetRaklionEnable(BOOL bEnable);
	BOOL IsRaklionEnable();
	void SetRaklionState(int iRaklionState);
	int GetRaklionState();
	void SetRaklionDetailState(int iState);
	int GetRaklionDetailState();
	void BossEggRegen();
	void BossEggDie();
	int GetBossEggCount();
	void BossEggRegenAll();
	void BossEggDeleteAll();
	void CheckUserOnRaklionBossMap();
	int CheckEnterRaklion(int iUserIndex);
	void OperateGmCommand(int iUserIndex, int iCommand);
	void UserMonsterCountCheck();
	
public:

	BOOL				m_IsRaklionEnable;				//4
	int					m_iRaklionState;				//8
	BOOL				m_bIsSucccess;					//C 
	BOOL				m_iUnk16;						//10 
	int					m_iAppearanceMSec;				//14
	int					m_iBossZoneCloseMSec;			//18
	int					m_iBossZoneOpenMSec;			//1C
	int					m_iFileDataLoad;				//20
	int					m_iAppearanceDelay;				//24
	int					m_iBossZoneCloseDelay;			//28
	int					m_iBossZoneOpenDelay;			//2C
	int					m_iBossEggHalf;					//30
	int					m_iBossEggMax;					//34

	CRaklionBattleOfSelupan m_RaklionBattleOfSelupan;	//38

	int m_iBossEggHalfChecked;							//E08
	int m_iBossEggCount;								//E0C
	int isUsed;											//E10
	int iYear;											//E14
	int iMon;											//E18
	int iDay;											//E1C
};

extern CRaklion g_Raklion;

extern int g_iUseRaklionEvent;

#endif // !defined(AFX_RAKLION_H__CCDE5834_0D07_423B_B33A_2D9554F8A789__INCLUDED_)
