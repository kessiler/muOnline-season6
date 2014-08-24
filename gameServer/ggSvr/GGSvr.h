// GGSvr.h: interface for the GGSvr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GGSVR_H__86EEE1D4_2A48_481C_9BE6_C4A4950B4CCF__INCLUDED_)
#define AFX_GGSVR_H__86EEE1D4_2A48_481C_9BE6_C4A4950B4CCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef GGSVR_EXPORTS
#define GGSVR_API __declspec(dllexport)
#else
#define GGSVR_API __declspec(dllimport)
#endif

#include "CSAuth2.h"


struct GG_AUTH
{
	DWORD dwProtocolCount;	// 0
	DWORD nMaxCount;	// 4
	_GG_AUTH_PROTOCOL * lpCurrentAuthProtocol;	// 8
	char szGGAuthName[256];	// C

};


DWORD   InitGameguardAuth(LPSTR szGGAuthName, DWORD nNumberOfUser);
void    CleanupGameguardAuth();
DWORD   AddAuthProtocol(LPSTR lpszAuthDll);
DWORD   LoadAuthProtocol(LPSTR lpszAuthDll, BOOL Flag);
DWORD   UnloadAuthProtocol(_GG_AUTH_PROTOCOL * pAuthProtocol);
CCSAuth2 *   GGAuthInitUser();
void   GGAuthCloseUser(CCSAuth2 * lpGGUser);
void   GGAuthGetQuery(CCSAuth2 * lpGGUser, _GG_AUTH_DATA * lpAuthQuery);
DWORD   GGAuthCheckAnswer(CCSAuth2 * lpGGUser, _GG_AUTH_DATA * lpAuthAnswer);


extern GG_AUTH ggGlobal;

#endif // !defined(AFX_GGSVR_H__86EEE1D4_2A48_481C_9BE6_C4A4950B4CCF__INCLUDED_)
