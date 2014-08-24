// TMonsterAIUtil.h: interface for the TMonsterAIUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERAIUTIL_H__D2E082C2_05F0_49E5_9177_3592AAF616E7__INCLUDED_)
#define AFX_TMONSTERAIUTIL_H__D2E082C2_05F0_49E5_9177_3592AAF616E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

class TMonsterAIUtil  
{

public:

	TMonsterAIUtil();
	virtual ~TMonsterAIUtil();

	BOOL FindPathToMoveMonster(LPOBJ lpObj, int iTargetX, int iTargetY, int iMaxPathCount, BOOL bPreventOverMoving);
	BOOL SendMonsterMoveMsg(LPOBJ lpObj);
	BOOL CheckMovingCondition(LPOBJ lpObj);
	void SendMonsterV2Msg(LPOBJ lpObj, LPBYTE lpMsg, int size);
	BOOL CheckMoveRange(LPOBJ lpObj, int iTargetX, int iTargetY);
	BOOL GetXYToPatrol(LPOBJ lpObj);
	BOOL GetXYToEascape(LPOBJ lpObj);
	BOOL GetXYToChase(LPOBJ lpObj);
	BOOL FindMonViewportObj(int iObjIndex, int iTargetObjIndex);
	BOOL FindMonViewportObj2(int iObjIndex, int iTargetObjIndex);
	void __cdecl SendChattingMsg(int iObjIndex, char* lpszMsg, ...);

};

#endif // !defined(AFX_TMONSTERAIUTIL_H__D2E082C2_05F0_49E5_9177_3592AAF616E7__INCLUDED_)
