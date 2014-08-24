// KanturuUtil.h: interface for the CKanturuUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KANTURUUTIL_H__F4F5A116_190C_42DA_9D05_982E6BCD73D5__INCLUDED_)
#define AFX_KANTURUUTIL_H__F4F5A116_190C_42DA_9D05_982E6BCD73D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TNotice.h"

class CKanturuUtil  
{

public:

	CKanturuUtil();
	virtual ~CKanturuUtil();

	void NotifyKanturuChangeState(int iState, int iDetailState);
	void NotifyKanturuEntranceReqResult(int iIndex, int iResult);
	void NotifyKanturuSuccessValue(int iSuccessValue);
	void NotifyKanturuCurrentState(int iIndex, int iState, int iDetailState);
	void NotifyKanturuBattleTime(int iBattleTime);
	void NotifyKanturuWideAreaAttack(int iIndex, int iTargetIndex, int iSkillType);
	void NotifyKanturuUserMonsterCount(int iMonsterCount, int iUserCount);
	void __cdecl SendMsgKanturuBattleUser(LPSTR lpszMsg, ...);
	void SendDataKanturuBattleUser(LPBYTE lpMsg, int iSize);
	void __cdecl SendMsgKauturuBossMapUser(LPSTR lpszMsg, ...);
	void SendDataKanturuBossMapUser(unsigned char* lpMsg, int iSize);
	void __cdecl SendMsgKauturuMapUser(char* lpszMsg, ...);
	void SendDataKanturuMapUser(unsigned char* lpMsg, int iSize);
	void __cdecl SendMsgAllUser(LPSTR lpszMsg, ...);
	void SendDataAllUser(unsigned char* lpMsg, int iSize);
	void __cdecl SendMsgToUser(int iIndex, char* lpszMsg, ...);
	void SendDataToUser(int iIndex, unsigned char* lpMsg, int iSize);
	void __cdecl SendKanturuChattingMsg(int iIndex, char* lpszMsg, ...);
	void SendDataKanturuTimeAttackEvent(int iIndex, BYTE btFlag, int iClearTime);


};

#endif // !defined(AFX_KANTURUUTIL_H__F4F5A116_190C_42DA_9D05_982E6BCD73D5__INCLUDED_)
