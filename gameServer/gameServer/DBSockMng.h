// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef DBSOCKMNG_H
#define DBSOCKMNG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wsJoinServerCli.h"

#define DS_UNINITIALIZED -1
#define DS_0 0
#define DS_1 1


class DBSockMng
{

public:

	DBSockMng();
	virtual ~DBSockMng();

	void Connect();
	void ActiveChange();
	int Send(int server, char* buf, int len);
	int Send(char* buf, int len);
	int MsgProc(int line, WPARAM wParam, LPARAM lParam);
	int GetActiveDS(){	return m_ActiveServer;	}

public:

	wsJoinServerCli wsCDBS[2];	// 0
	int m_ActiveServer;	/// 320054
} ;


extern DBSockMng cDBSMng;

#endif /* DBSOCKMNG_H */