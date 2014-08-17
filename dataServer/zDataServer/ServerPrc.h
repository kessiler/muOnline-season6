#ifndef __SERVERPRC_H__
#define __SERVERPRC_H__


#include "Giocp.h"

#define MAX_SERVEROBJECT			100	// 서버 오브젝트 최대 개수



typedef struct
{
	int  Connected;		// 접속되어 있나? 0 : 접속이 안되어 있다.
						// 1 : 접속은 되었으나 아직 인증이 안된상태
	                    // 2 : 서버 인증이 완료된 상태
	char Ip[20];		// 접속 주소
	WORD Port;			// 서버일 경우 포트번호
	BYTE Type;			// 접속 프로그램의 타입
	char ServerName[51];

	WORD ServerCode;
	int pServer;
	int iMapSvrGroup;
	
	SOCKET m_socket;
	LPPER_SOCKET_CONTEXT	lpPerSocketContext;
	//PER_SOCKET_CONTEXT PerSocketContext;
} ServerObject_Struct, * LPServerObject_Struct;


extern void  gSObjInit();
extern short gSObjAdd(SOCKET socket, char *ip);
extern short gSObjDel(int index);
extern BOOL  gSObjSetInfo(int aIndex, WORD port, int type, char * servername, WORD ServerCode);
extern int gSObjIndexSerrchbyServerCode(WORD ServerCode);

extern ServerObject_Struct	gSObj[];

#endif
