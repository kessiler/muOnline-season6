// GuardianStatue.h: interface for the CGuardianStatue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUARDIANSTATUE_H__CA8EF556_B0F0_4F40_8485_B5DF486AE700__INCLUDED_)
#define AFX_GUARDIANSTATUE_H__CA8EF556_B0F0_4F40_8485_B5DF486AE700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#if (_GSCS==1)

class CGuardianStatue  
{

public:

	CGuardianStatue();
	virtual ~CGuardianStatue();

	void GuardianStatueAct(int iIndex);

};

extern CGuardianStatue g_CsNPC_GuardianStatue;

//#endif

#endif // !defined(AFX_GUARDIANSTATUE_H__CA8EF556_B0F0_4F40_8485_B5DF486AE700__INCLUDED_)
