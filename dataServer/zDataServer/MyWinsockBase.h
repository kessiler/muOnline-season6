#ifndef __MYWSBASE_H__
#define __MYWSBASE_H__

#include <winsock2.h>

class MyWinsockBase
{
private:
	BOOL    Startup();

protected:
	int		m_iMaxSockets;				// 소켓 최대 개수
	HWND	m_hWnd;					// 윈도우 메시지를 처리할 윈도우 핸들
	SOCKET	m_socket;				// 서버 소켓
	int     m_sendbufsize;			// 소켓의 보내기 버퍼 크기 	
	int     m_recvbufsize;			// 소켓의 받기 버퍼 크기
	BOOL	m_Connect; //new value

public :
	BOOL CreateSocket();
	BOOL Close();
	BOOL Close(SOCKET socket);
	BOOL Close2(SOCKET socket);
	
	SOCKET GetSocket();
	int GetSendBuffSize();
	int GetRecvBuffSize();

	BOOL GetConnect();
	void SetConnect(BOOL connected);

	MyWinsockBase();
	~MyWinsockBase();
};

#endif


