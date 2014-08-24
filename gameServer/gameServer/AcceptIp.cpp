//GameServer 1.00.90 - complete
#include "stdafx.h"
#include "AcceptIp.h"
#include "GameMain.h"
#include "logproc.h"
#include "..\include\readscript.h"

CAcceptIp acceptIP;

CAcceptIp::CAcceptIp()
{
	this->Init();
}

CAcceptIp::~CAcceptIp()
{
	return;
}

void CAcceptIp::Init()
{
	for (int n=0; n<MAX_USE_IP;n++)
	{
		memset(this->szIp[n] , 0, sizeof(szIp[n]) );
		this->UseIp[n]= false;
	}
}

BOOL CAcceptIp::IsIp(LPSTR ip)
{
	for(int n=0;n<MAX_USE_IP;n++)
	{
		if ( this->UseIp[n] != false )
		{
			if ( strcmp(this->szIp[n], ip)== 0 )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CAcceptIp::Load(LPSTR filename)
{
	int count=0;
	int Token;
	
	SMDFile=fopen(filename, "r");	//ok

	if (SMDFile == NULL)
	{
		MsgBox(lMsg.Get(MSGGET(0, 112)), filename);
		return;
	}


	int n=0;

	while ( true )
	{
		Token=GetToken();

		if (Token == END)
		{
			break;
		}

		if ( Token == NAME )
		{
			strcpy(this->szIp[count] , TokenString);
			this->UseIp[count] = true;
			count++;
		}
	}

	fclose(SMDFile);
	LogAdd(lMsg.Get(MSGGET(1, 164)), count);
}


