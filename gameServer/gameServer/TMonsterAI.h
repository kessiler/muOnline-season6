// TMonsterAI.h: interface for the TMonsterAI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERAI_H__0780F804_C72A_4667_BB23_25ECCA8C056A__INCLUDED_)
#define AFX_TMONSTERAI_H__0780F804_C72A_4667_BB23_25ECCA8C056A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class TMonsterAI  
{

public:

	TMonsterAI();
	virtual ~TMonsterAI();

	static void  MonsterMove(int iIndex);
	static void  MonsterMoveProc();
	static void  MonsterAIProc();
	static BOOL  RunAI(int iIndex, int iMonsterClass);
	static BOOL  UpdateCurrentAIUnit(int iIndex);
	static void  MonsterStateMsgProc(int iIndex);
	static void  ProcessStateMsg(struct OBJECTSTRUCT* lpObj, int iMsgCode, int iIndex, int aMsgSubCode);

private:

};



#endif // !defined(AFX_TMONSTERAI_H__0780F804_C72A_4667_BB23_25ECCA8C056A__INCLUDED_)
