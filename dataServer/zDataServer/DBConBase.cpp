// DBConBase.cpp: implementation of the CDBConBase class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBConBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBConBase::CDBConBase()
{

}

CDBConBase::~CDBConBase()
{

}

BOOL CDBConBase::Connect(char* szConID, char* szConPass)
{
	if(m_DBQuery.Connect(3, gSettings.ConnectDNS, szConID, szConPass) == FALSE)
	{
		//MsgBox("%s DB Connect Fail", gSettings.ConnectDNS);
		return FALSE;
	}

	return TRUE;
}