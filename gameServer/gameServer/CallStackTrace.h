#ifndef CALLSTACKTRACE_H
#define CALLSTACKTRACE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCallStackTrace
{
public:
	CCallStackTrace(char * const szLogID,char * const szLogFileName, char * const szLogDirectoryName);
	virtual ~CCallStackTrace();
	int SaveCallStack(BOOL bWriteInFile);

private:
	void Output();
	int TraceStack(BOOL bWriteInFile);
	DWORD CCallStackTrace::ConvertAddress(HANDLE hProcess,DWORD address, char* output_buffer);
	static DWORD __stdcall CCallStackTrace::GetModuleBase(HANDLE hProcess, DWORD address);

private:
	FILE * m_fLogFile;	// 4
	char m_szLogID[260];	// 8
	char m_szLogFileName[260];	// 10C
	char m_szLogDirectoryName[260];	// 210
	char m_szCallFuncName[260];	// 314
	CRITICAL_SECTION m_critLogToFile;	// 418
	std::string m_strCallStackLog;	// 430
};
	
#endif