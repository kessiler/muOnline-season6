//GameServer 1.00.90 - complete
#include "stdafx.h"
#include "LogToFile.h"
#include "Gamemain.h"


CLogToFile::CLogToFile(const LPSTR LogFileName,const LPSTR LogDirectoryName, BOOL bWithDate)
{
	if (strcmp(LogFileName, "") == 0)
	{
		strcpy(this->m_szLogFileName , "LOGDATA");
	}
	else
	{
		strcpy(this->m_szLogFileName , LogFileName);
	}

	
	if (!strcmp(LogDirectoryName, ""))
	{
		strcpy(this->m_szLogDirectoryName , "Logs");
	}
	else
	{
		strcpy(this->m_szLogDirectoryName  , LogDirectoryName);
	}
	
	this->m_bWithDate  = bWithDate;	

	CreateDirectory(LogDirectoryName, NULL);

	InitializeCriticalSection(&this->m_critLogToFile );
	
}


CLogToFile::~CLogToFile()
{
	DeleteCriticalSection(&this->m_critLogToFile );
}


void CLogToFile::Output( LPSTR fmt, ...)
{
	va_list argptr;
	int iChrWritten;
	char szLogFileName[260]; // Is StringZero Terminated

	SYSTEMTIME strSystime;
	
	EnterCriticalSection(&this->m_critLogToFile );

	va_start(argptr, fmt);
	iChrWritten=vsprintf(this->m_cBUFFER , fmt, argptr);
	va_end(argptr);

	GetLocalTime(&strSystime);

	wsprintf(szLogFileName, "%s\\%s %04d-%02d-%02d_%s.txt",
		&this->m_szLogDirectoryName[0] , &this->m_szLogFileName [0],
		strSystime.wYear, strSystime.wMonth, strSystime.wDay,
		szServerName);

	if ( (this->m_fLogFile = fopen(szLogFileName, "a+"))==0)
	{
		LeaveCriticalSection(&this->m_critLogToFile );	
	}
	else
	{
		if (this->m_bWithDate ==0)
		{
			fprintf(this->m_fLogFile , "%s\n", this->m_cBUFFER);
		}
		else
		{
			fprintf(this->m_fLogFile , "%d:%d:%d  %s\n", strSystime.wHour, strSystime.wMinute, strSystime.wSecond, this->m_cBUFFER);
		}
		fclose(this->m_fLogFile );

		LeaveCriticalSection(&this->m_critLogToFile );
	}
}
