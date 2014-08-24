// CannonTower.h: interface for the CCannonTower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANNONTOWER_H__DD8C3ABB_D613_4E50_B4F1_0127230C40BC__INCLUDED_)
#define AFX_CANNONTOWER_H__DD8C3ABB_D613_4E50_B4F1_0127230C40BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#if (_GSCS==1)
class CCannonTower  
{
public:
	CCannonTower();
	virtual ~CCannonTower();
	void CannonTowerAct(int iIndex);
};

extern CCannonTower g_CsNPC_CannonTower;

//#endif

#endif // !defined(AFX_CANNONTOWER_H__DD8C3ABB_D613_4E50_B4F1_0127230C40BC__INCLUDED_)
