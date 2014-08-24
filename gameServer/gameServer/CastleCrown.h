// CastleCrown.h: interface for the CCastleCrown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CASTLECROWN_H__1494B89A_2CEA_41C6_9B1A_C5CF4064943F__INCLUDED_)
#define AFX_CASTLECROWN_H__1494B89A_2CEA_41C6_9B1A_C5CF4064943F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//#if (_GSCS==1)

class CCastleCrown {
public:
	CCastleCrown();
	virtual ~CCastleCrown();
	void CastleCrownAct(int iIndex);
};

extern CCastleCrown g_CsNPC_CastleCrown;

//#endif

#endif // !defined(AFX_CASTLECROWN_H__1494B89A_2CEA_41C6_9B1A_C5CF4064943F__INCLUDED_)
