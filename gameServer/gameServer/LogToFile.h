#ifndef LOGTOFILE_H
#define LOGTOFILE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLogToFile
{

public:

	void __cdecl Output(LPSTR fmt, ...);
	CLogToFile(const LPSTR LogFileName,const LPSTR LogDirectoryName, BOOL bWithDate);
	~CLogToFile();

private:

	FILE* m_fLogFile;	// 0
	char m_szLogFileName[260]; // 4
	char m_szLogDirectoryName[260]; // 108
	char m_cBUFFER[65536];	// 20C
	BOOL m_bWithDate; // 1020C
	CRITICAL_SECTION m_critLogToFile; 
};

#endif