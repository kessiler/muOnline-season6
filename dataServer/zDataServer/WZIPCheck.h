// WZIPCheck.h: interface for the CWZIPCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WZIPCHECK_H__070C97E5_61F4_4963_BD43_D56E2A17B163__INCLUDED_)
#define AFX_WZIPCHECK_H__070C97E5_61F4_4963_BD43_D56E2A17B163__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "windows.h"

typedef struct{	DWORD NetID : 8;	DWORD HostID : 24;}	IPClassA;
typedef struct{	DWORD NetID : 16;	DWORD HostID : 16;}	IPClassB;
typedef struct{	DWORD NetID : 24;	DWORD HostID : 8;}	IPClassC;
typedef struct{	DWORD NetID : 32;}						IPClassD;

typedef struct{BYTE	a4;	BYTE	a3;	BYTE	a2;	BYTE	a1;}IPSTD;

#define IPMASK_CLASS_A		0x00000000
#define IPMASK_CLASS_B		0x80000000
#define IPMASK_CLASS_C		0xC0000000
#define IPMASK_CLASS_D		0xE0000000

typedef struct {union{IPClassA	ClassA;IPClassB	ClassB;IPClassC	ClassC;IPClassD	ClassD;IPSTD STD;DWORD IP;};
}IPStruct;

typedef struct
{
	char		Country[4];
	IPStruct	IP;
	int			Length;
}IPInfo;

#include <vector>
#include <iostream>
using namespace std;

typedef vector<IPInfo*>	IPList;

class CWZIPCheck  
{
	IPStruct	m_PassIP;		// 통과 가능한 IP
	int			m_iPassIPRange;	// 통과 가능한 IP영역

	BOOL		m_bLoadFile;

	IPList		m_EntireIP;
	BOOL		ReadIP(char* lpszFileName);
public:
	CWZIPCheck();
	virtual ~CWZIPCheck();
	
	void		Init();

	BOOL		CheckIP(char* IP);
	BOOL		CheckIP(BYTE I1, BYTE I2, BYTE I3, BYTE I4);
	BOOL		CheckIP(DWORD IP);
	
//	char*		GetContry(DWORD IP);
//	char*		GetContry(BYTE I1, BYTE I2, BYTE I3, BYTE I4);
//	char*		GetContry(char* IP);
};

extern CWZIPCheck	g_WZIPCheck;

#endif // !defined(AFX_WZIPCHECK_H__070C97E5_61F4_4963_BD43_D56E2A17B163__INCLUDED_)
