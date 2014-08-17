// WZIPCheck.cpp: implementation of the CWZIPCheck class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WZIPCheck.h"
#include "ReadScript.h"
#include "Winsock.h"

CWZIPCheck	g_WZIPCheck;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWZIPCheck::CWZIPCheck()
{
/*	m_EntireIP.clear();
	m_bLoadFile = FALSE;	
	ReadIP("DATA\\AuthorizedIPList.txt");

	m_PassIP.STD.a1 = 61;
	m_PassIP.STD.a2 = 33;
	m_PassIP.STD.a3 = 48;
	m_PassIP.STD.a4 = 1;

	m_iPassIPRange = 0;*/
}

CWZIPCheck::~CWZIPCheck()
{
	IPList::iterator Itor = m_EntireIP.begin();
	for( ; Itor != m_EntireIP.end(); ++Itor)
	{
		delete (*Itor);
	}
	m_EntireIP.clear();
}

void CWZIPCheck::Init()
{
	if(ReadIP("AllowableIPList.txt") == FALSE)
	{
		m_bLoadFile = FALSE;
//		AfxMessageBox("[ANTI-HACKING][IP LIST] Failed to load Allowable IP List", 0, 0);
	}
}

BOOL CWZIPCheck::ReadIP(char* lpszFileName)
{
	m_bLoadFile = FALSE;

	if(lpszFileName == NULL || strcmp(lpszFileName, "") == 0)
	{
//		MsgBox("[IP List Load] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDFile = fopen(lpszFileName, "r");

		if(SMDFile == NULL)
		{
//			MsgBox("[IP List Load] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		IPList::iterator Itor = m_EntireIP.begin();
		for( ; Itor != m_EntireIP.end(); ++Itor)
		{
			delete (*Itor);
		}
		m_EntireIP.clear();

		SMDToken Token;
		int iType = -1;

		while( true )
		{
			Token = GetToken();	if(Token == END) break;

			iType = (int)TokenNumber;

			while( true )
			{
				if(iType == NAME)
				{
					IPInfo* pIPStruct;
					pIPStruct = new IPInfo;

					Token = GetToken();

					if(strcmp("end",TokenString)==NULL) break;
							
											pIPStruct->IP.STD.a1 = (int)TokenNumber;
					Token = (*GetToken)();	pIPStruct->IP.STD.a2 = (int)TokenNumber;
					Token = (*GetToken)();	pIPStruct->IP.STD.a3 = (int)TokenNumber;
					Token = (*GetToken)();	pIPStruct->IP.STD.a4 = (int)TokenNumber;		
					
					Token = (*GetToken)();	pIPStruct->Length = (int)TokenNumber;

					gWindow.PrintLog("[ANTI-HACKING][IP LIST] IP : [%d.%d.%d.%d] Length : %d", 
						pIPStruct->IP.STD.a1, pIPStruct->IP.STD.a2, pIPStruct->IP.STD.a3, pIPStruct->IP.STD.a4, pIPStruct->Length);

					m_EntireIP.push_back(pIPStruct);
				}
			}
		}

		fclose(SMDFile);

		m_bLoadFile = TRUE;

		gWindow.PrintLog("[IP List Load ] - %s file is Loaded", lpszFileName);
	}
	catch(...)
	{
		//MsgBox("[IP List Load] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return TRUE;
}

BOOL CWZIPCheck::CheckIP(DWORD IP)
{
	if( IP >= m_PassIP.IP && IP <= m_PassIP.IP + m_iPassIPRange )
	{	// 통과 가능한 IP대역폭이면 OK
		return TRUE;
	}

	IPInfo* pIPInfo;

	IPList::iterator Itor = m_EntireIP.begin();
	for( ; Itor != m_EntireIP.end(); ++Itor)
	{		
		pIPInfo = (*Itor);
		if( IP >= pIPInfo->IP.IP && IP <= pIPInfo->IP.IP + pIPInfo->Length )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CWZIPCheck::CheckIP(BYTE I1, BYTE I2, BYTE I3, BYTE I4)
{
	IPStruct	IP;
	IP.STD.a1 = I1;
	IP.STD.a2 = I2;
	IP.STD.a3 = I3;
	IP.STD.a4 = I4;

	if( IP.IP >= m_PassIP.IP && IP.IP <= m_PassIP.IP + m_iPassIPRange )
	{	// 통과 가능한 IP대역폭이면 OK
		return TRUE;
	}

	IPInfo* pIPInfo;
	IPList::iterator Itor = m_EntireIP.begin();
	for( ; Itor != m_EntireIP.end(); ++Itor)
	{		
		pIPInfo = (*Itor);
		if( IP.IP >= pIPInfo->IP.IP && IP.IP <= pIPInfo->IP.IP + pIPInfo->Length )
		{			
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CWZIPCheck::CheckIP(char* sIP)
{
	if(m_bLoadFile == FALSE)
	{
		gWindow.PrintLog("Not Loading Allowable IP LIST!!");
		return TRUE;
	}

	DWORD	IP = ntohl(inet_addr(sIP));

	if( IP >= m_PassIP.IP && IP <= m_PassIP.IP + m_iPassIPRange )
	{	// 통과 가능한 IP대역폭이면 OK
		return TRUE;
	}

	IPInfo* pIPInfo;	

	IPList::iterator Itor = m_EntireIP.begin();
	for( ; Itor != m_EntireIP.end(); ++Itor)
	{		
		pIPInfo = (*Itor);
		if( IP >= pIPInfo->IP.IP && IP <= pIPInfo->IP.IP + pIPInfo->Length )
		{		
			return TRUE;
		}
	}
	return FALSE;
}