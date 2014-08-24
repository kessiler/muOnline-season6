// GGSvr.cpp: implementation of the GGSvr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GGSvr.h"




GG_AUTH ggGlobal;




DWORD  InitGameguardAuth(LPSTR szGGAuthName, DWORD nNumberOfUser)
{
	ggGlobal.dwProtocolCount = 0;
	ggGlobal.nMaxCount  = nNumberOfUser;
	ggGlobal.lpCurrentAuthProtocol = NULL;
	ggGlobal.szGGAuthName[0] = 0;

	if ( szGGAuthName != NULL && szGGAuthName[0] != '\0')
	{
		if ( szGGAuthName[strlen(szGGAuthName)-1] == '\\' )
		{
			strcpy(szGGAuthName, ggGlobal.szGGAuthName);
		}
		else
		{
			sprintf(ggGlobal.szGGAuthName,"%s%c", szGGAuthName, '\\');
		}
	}

	DWORD dwResult = LoadAuthProtocol("ggauth.dll", TRUE);

	if ( dwResult != 0 )
	{
		CleanupGameguardAuth();
		return dwResult;
	}
	return 0;
}



void  CleanupGameguardAuth()
{
	while ( ggGlobal.lpCurrentAuthProtocol != NULL )
	{
		_GG_AUTH_PROTOCOL * pAuthProtocol = ggGlobal.lpCurrentAuthProtocol;
		ggGlobal.lpCurrentAuthProtocol = pAuthProtocol->lpPrevProtocol;

		if ( pAuthProtocol->hDllHinstance )
			FreeLibrary((HMODULE)pAuthProtocol->hDllHinstance);

		free(pAuthProtocol);
	}
}


DWORD  AddAuthProtocol(LPSTR lpszAuthDll)
{
	return LoadAuthProtocol(lpszAuthDll, FALSE);
}


DWORD  LoadAuthProtocol(LPSTR lpszAuthDll, BOOL Flag)	// Perfect
{
	_GG_AUTH_PROTOCOL * pProtocol = (_GG_AUTH_PROTOCOL *)malloc(sizeof(_GG_AUTH_PROTOCOL));
	
	if ( pProtocol == NULL )
		return 1;

	char szTemp[260];

	sprintf(szTemp, "%s%s", ggGlobal.szGGAuthName, lpszAuthDll);

	pProtocol->hDllHinstance = LoadLibrary(szTemp);

	if ( pProtocol->hDllHinstance == NULL )
	{
		free(pProtocol);
		return 2;
	}

	DWORD (*PtrcGetVersion)(void) = (DWORD (*)(void))GetProcAddress(pProtocol->hDllHinstance, "PrtcGetVersion"); 
	pProtocol->PtrcGetAuthQuery = (DWORD  (*)(int, LPGGAUTH, _GG_AUTH_DATA*, _GG_AUTH_DATA*))GetProcAddress(pProtocol->hDllHinstance, "PrtcGetAuthQuery"); 
	pProtocol->PtrcCheckAuthAnswer = (DWORD  (*)(int, LPGGAUTH, _GG_AUTH_DATA*, _GG_AUTH_DATA*))GetProcAddress(pProtocol->hDllHinstance, "PrtcCheckAuthAnswer"); 

	if ( PtrcGetVersion != NULL && pProtocol->PtrcGetAuthQuery != NULL && pProtocol->PtrcCheckAuthAnswer != NULL )
	{
		pProtocol->dwPtrcVersion = PtrcGetVersion();
		pProtocol->dwQueryCount = 0;
		pProtocol->unkC = 0;

		if ( Flag != FALSE )
		{
			pProtocol->unkC = 3;
			pProtocol->lpPrevProtocol = NULL;
		}
		else
		{
			pProtocol->unkC = 1;
			ggGlobal.lpCurrentAuthProtocol->unkC &= -2;
			pProtocol->lpPrevProtocol = ggGlobal.lpCurrentAuthProtocol;
		}

		ggGlobal.lpCurrentAuthProtocol = pProtocol;
		ggGlobal.dwProtocolCount ++;

		return 0;
	}
	else
	{
		FreeLibrary(pProtocol->hDllHinstance);
		free(pProtocol);
		return 3;
	}

}

//#pragma optimize("ty", on) 

DWORD  UnloadAuthProtocol(_GG_AUTH_PROTOCOL * pAuthProtocol)
{
	_GG_AUTH_PROTOCOL * pProtocol = ggGlobal.lpCurrentAuthProtocol;
	_GG_AUTH_PROTOCOL * pTempProtocol = pProtocol->lpPrevProtocol;
	if ( ggGlobal.dwProtocolCount <= 1 || pTempProtocol == NULL )
		return 10;

	
	/*if ( 
		return 10;*/
	
	while ( true )
	{
		if ( pTempProtocol == pAuthProtocol )
			break;

		pProtocol = pTempProtocol;
		pTempProtocol = pTempProtocol->lpPrevProtocol;
		if ( pTempProtocol == NULL )
		{
			return 10;
		}
	}


	pProtocol->lpPrevProtocol = pAuthProtocol->lpPrevProtocol;

	if ( pAuthProtocol->hDllHinstance )
		FreeLibrary(pAuthProtocol->hDllHinstance);

	free(pAuthProtocol);
	ggGlobal.dwProtocolCount--;

	return 0;
}




CCSAuth2 *  GGAuthInitUser()
{
	CCSAuth2 * pNew = new CCSAuth2;

	if ( pNew != NULL )
		return pNew;

	return NULL;
}



void  GGAuthCloseUser(CCSAuth2 * lpGGUser)
{
	if ( lpGGUser != NULL )
		delete lpGGUser;
}


void  GGAuthGetQuery(CCSAuth2 * lpGGUser, _GG_AUTH_DATA * lpAuthQuery)
{
	lpGGUser->GetAuthQuery();
	lpAuthQuery->dwIndex = lpGGUser->m_AuthQuery.dwIndex;
	lpAuthQuery->dwValue1 = lpGGUser->m_AuthQuery.dwValue1;
	lpAuthQuery->dwValue2 = lpGGUser->m_AuthQuery.dwValue2;
	lpAuthQuery->dwValue3 = lpGGUser->m_AuthQuery.dwValue3;
}


DWORD  GGAuthCheckAnswer(CCSAuth2 * lpGGUser, _GG_AUTH_DATA * lpAuthAnswer)
{
	lpAuthAnswer->dwIndex = lpGGUser->m_AuthAnswer.dwIndex;
	lpAuthAnswer->dwValue1 = lpGGUser->m_AuthAnswer.dwValue1;
	lpAuthAnswer->dwValue2 = lpGGUser->m_AuthAnswer.dwValue2;
	lpAuthAnswer->dwValue3 = lpGGUser->m_AuthAnswer.dwValue3;
	return lpGGUser->CheckAuthAnswer();
}