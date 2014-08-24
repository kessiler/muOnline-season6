#ifndef WZMULTICASTSOCK_H
#define WZMULTICASTSOCK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_MULTICAST_SOCK 32


struct IPINFO_NODE 
{
	BOOL m_bInUsed;	// 0
	char m_szIPADDR[16];	// 4
};
// <size 0x14>

class CWzMultiCastSock 
{
public:

	CWzMultiCastSock();
	~CWzMultiCastSock();

	int  DoEnumIpAddress();
	int  GetEnummedIpAddress(int iIPINDEX, LPSTR lpszOutputIpAddress);
	int  StartMultiCast(LPSTR lpszIPADDR);
	int  ReStartMultiCast();
	void  EndMultiCast();
	int  SendData(char* lpBUFFER, int iBUFSIZE);

protected:

	void  SetErrorMsgFlag(int);	// Dont have prefix Deathway
	unsigned int  GetLastErrorNum();// Dont have prefix Deathway
	char*  GetLastErrorMsg();// Dont have prefix Deathway
	void  SetRecvNotifyFunc(void  (*)(CWzMultiCastSock*, int, int, int, char*, int));// Dont have prefix Deathway
	void  SetErrorNotifyFunc(void  (*)(CWzMultiCastSock*, int, char*));// Dont have prefix Deathway
	void  CWzMultiCastSock::SaveLastError(int iErrorNo);
	void  CWzMultiCastSock::SavePrivateErrorMessage(LPSTR lpszErrorMessage);

	static unsigned int __stdcall CWzMultiCastSock::RecvThreadProc(void* lpParam);

private:	

	unsigned int m_skSocket;	// 0
	char m_szIPADDR_JOIN[16];	// 4
	struct IPINFO_NODE m_stIPINFO_LIST[MAX_MULTICAST_SOCK];	// 14
	int m_iIPINFO_COUNT;	// 294
	unsigned int m_uLastErrorNum;	// 298
	char m_szLastErrorMessage[260];	// 29C
	HANDLE m_hWorkerThread;	// 3A0
	unsigned int m_idWorkerThread;	// 3A4
	void  (*m_lpfnCALLBACK_RECVNOTIFY)(CWzMultiCastSock*, int, int, int, char*, int);	// 3A8
	void  (*m_lpfnCALLBACK_ERRORNOTIFY)(CWzMultiCastSock*, int, char*);	// 3AC
};


#endif