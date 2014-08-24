// RaklionUtil.h: interface for the CRaklionUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONUTIL_H__EA94DB42_362B_40BD_921B_A133FBFAE202__INCLUDED_)
#define AFX_RAKLIONUTIL_H__EA94DB42_362B_40BD_921B_A133FBFAE202__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\winutil.h"

struct PMSG_ANS_RAKLION_STATE_CHANGE
{
	PBMSG_HEAD2 h;// C1:D1:12
	BYTE btState;	// 4
	BYTE btDetailState;// 5
};

#pragma pack(1)
struct PMSG_ANS_RAKLION_SUCCESS_VALUE
{
	PBMSG_HEAD2 h;	// C1:D1:13
	BYTE btValue;	// 4
};
#pragma pack()

struct PMSG_ANS_RAKLION_CURRENT_STATE
{
	PBMSG_HEAD2 h;	// C1:D1:11
	BYTE btCurrentState;	// 4
	BYTE btCurrentDetailState;	// 5
};

#pragma pack(1)
struct PMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK
{
	PBMSG_HEAD2 h;	// C1:D1:06
	BYTE btObjClassH;	// 4
	BYTE btObjClassL;	// 5
	BYTE btType;	// 6
};
#pragma pack()

struct PMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT
{
	PBMSG_HEAD2 h;	// C1:D1:15
	BYTE btMonsterCount;	// 4
	BYTE btUserCount;	// 5
};

class CRaklionUtil  
{
public:
	CRaklionUtil();
	virtual ~CRaklionUtil();
	void NotifyRaklionChangeState(int iState, int iDetailState);
	void NotifyRaklionSuccessValue(int iSucessValue);
	void NotifyRaklionCurrentState(int iIndex, BYTE btState, BYTE btDetailState);
	void NotifyRaklionWideAreaAttack(int iIndex, int iTargetIndex, int iSkillType);
	void NotifyRaklionUserMonsterCount(BYTE btMonsterCount, BYTE btUserCount);
	void __cdecl SendMsgUser(LPSTR lpszMsg, ...);
	void __cdecl SendMsgRaklionBossMapUser(LPSTR lpszMsg, ...);
	void SendDataRaklionBossMapUser(LPBYTE lpMsg, int iSize);
	void __cdecl SendMsgRaklionMapUser(LPSTR lpszMsg, ...);
	void SendDataRaklionMapUser(LPBYTE lpMsg, int iSize);
	void __cdecl SendMsgAllUser(LPSTR lpszMsg, ...);
	void SendDataAllUser(LPBYTE lpMsg, int iSize);
	void __cdecl SendMsgToUser(int iIndex, LPSTR lpszMsg, ...);
	void SendDataToUser(int iIndex, LPBYTE lpMsg, int iSize);
	void __cdecl SendRaklionChattingMsg(int iIndex, LPSTR lpMsg, ...);
};

extern CRaklionUtil g_RaklionUtil;

#endif // !defined(AFX_RAKLIONUTIL_H__EA94DB42_362B_40BD_921B_A133FBFAE202__INCLUDED_)
