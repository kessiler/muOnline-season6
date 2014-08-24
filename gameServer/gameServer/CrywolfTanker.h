// CrywolfTanker.h: interface for the CCrywolfTanker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFTANKER_H__8CBC3465_0A8F_4932_825C_4BC8F77DA823__INCLUDED_)
#define AFX_CRYWOLFTANKER_H__8CBC3465_0A8F_4932_825C_4BC8F77DA823__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrywolfTanker
{

public:

  CCrywolfTanker();
  virtual ~CCrywolfTanker();

  void CrywolfTankerAct(int iIndex);

};

extern CCrywolfTanker g_CrywolfMON_Tanker;

#endif // !defined(AFX_CRYWOLFTANKER_H__8CBC3465_0A8F_4932_825C_4BC8F77DA823__INCLUDED_)
