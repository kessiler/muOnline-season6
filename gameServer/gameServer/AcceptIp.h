// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef ACCEPTIP_H
#define ACCEPTIP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_USE_IP	100

class CAcceptIp
{

public:

	CAcceptIp();
	virtual ~CAcceptIp();

	void Init();
	BOOL IsIp(LPSTR ip);
	void Load(LPSTR filename);

private:

	bool UseIp[MAX_USE_IP];
	char szIp[MAX_USE_IP][16];
};

extern CAcceptIp acceptIP;

#endif