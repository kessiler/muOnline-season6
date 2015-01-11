#include "stdafx.h"
#include "ConnectEx.h"
#include "Import.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "Protocol.h"
#include "User.h"
#include "Interface.h"
// ----------------------------------------------------------------------------------------------

ConnectEx gConnectEx;
// ----------------------------------------------------------------------------------------------

DWORD GetAccountInfo_Buff;
DWORD GetAccountInfo_Buff2;
DWORD DisconnectExcept_Buff;
DWORD DisconnectEvent_Buff;
DWORD MenuExit_Buff;
int Counter;
// ----------------------------------------------------------------------------------------------

Naked(GetAccountInfo)
{
	_asm
	{
		lea ecx, [ebp-0x18]
		mov GetAccountInfo_Buff, ecx
		lea ecx, [ebp-0x30]
		mov GetAccountInfo_Buff2, ecx
	}
	// ----
	memcpy(gConnectEx.m_AccountID, (char*)GetAccountInfo_Buff, 10);
	memcpy(gConnectEx.m_Password, (char*)GetAccountInfo_Buff2, 20);
	*(DWORD*)0x8793714 = 1;
	// ----
	_asm
	{
		mov GetAccountInfo_Buff, 0x0040B90D
		jmp GetAccountInfo_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(DisconnectEvent)
{
	_asm
	{
		mov DisconnectEvent_Buff, ecx
	}
	// ----
	if( DisconnectEvent_Buff == 32 )
	{
		if( gConnectEx.m_ConnectState != ConnectExType::OnForceDisconnect )
		{
			gConnectEx.m_ConnectState = ConnectExType::OnReconnect;
		}
		else
		{
			SetByte((LPVOID)0x004DA422, 0x75);
		}
		// ----
		_asm
		{
			mov DisconnectEvent_Buff, 0x004D038B
			jmp DisconnectEvent_Buff
		}
	}
	// ----
	_asm
	{
		mov DisconnectEvent_Buff, 0x004D0430
		jmp DisconnectEvent_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(MenuExit)
{
	gConnectEx.m_ConnectState = ConnectExType::OnForceDisconnect;
	// ----
	_asm
	{
		mov MenuExit_Buff, 0x007A88AD
		jmp MenuExit_Buff
	}
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::Load()
{
	this->m_ConnectState	= ConnectExType::OnLine;
	this->m_LastSendTick	= 0;
	this->m_ReconnectTick	= 0;
	this->m_WantMUHelper	= false;
	// ----
	SetRange((LPVOID)0x007A889B, 18, ASM::NOP);
	SetOp((LPVOID)0x007A889B, (LPVOID)MenuExit, ASM::JMP);
	SetRange((LPVOID)0x004D035A, 9, ASM::NOP);
	SetOp((LPVOID)0x004D035A, (LPVOID)DisconnectEvent, ASM::JMP);
	SetRange((LPVOID)0x0040B903, 10, ASM::NOP);
	SetOp((LPVOID)0x0040B903, (LPVOID)GetAccountInfo, ASM::JMP);
	SetByte((LPVOID)0x004DA422, 0xEB);
	SetRange((LPVOID)0x0063CD76, 5, ASM::NOP);	//-> You are connected to the server
	SetOp((LPVOID)0x0040BC24, this->CreateConnect, ASM::CALL);
	SetOp((LPVOID)0x004D762C, this->CreateConnect, ASM::CALL);
	SetOp((LPVOID)0x004EF622, this->CreateConnect, ASM::CALL);
	SetOp((LPVOID)0x0063CD3A, this->CreateConnect, ASM::CALL);
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::Run()
{
	DWORD Delay = GetTickCount() - this->m_LastSendTick;
	// ----
	if(	this->m_ConnectState != ConnectExType::OnReconnect )
	{
		return;
	}
	// ----
#ifdef __MUANGEL__
	if( !this->m_ReconnectTick )
	{
		this->m_ReconnectTick = GetTickCount();
	}
	// ----
	if( GetTickCount() - this->m_ReconnectTick >= 60000 * 3 )
	{
		this->m_ConnectState = ConnectExType::OnForceDisconnect;
		ExitProcess(0);
	}
#endif
	// ----
	this->ClearGame();
	this->DrawProgres();
	// ----
	if( Delay < 5000 || pPlayerState != GameProcess )
	{
		return;
	}
	// ----
	if( pOnLine == 0 )
	{
		this->Reconnect();
	}
	// ----
	if( pOnLine == 1 )
	{
		this->Relogin();
	}
	// ----
	this->m_LastSendTick = GetTickCount();
}
// ----------------------------------------------------------------------------------------------

int ConnectEx::CreateConnect(char * IP, WORD Port)
{
	if( Port == 44405 )
	{
		sprintf(gConnectEx.m_CSIP, "%s", IP);
		gConnectEx.m_CSPort = Port;
	}
	// ----
	sprintf(gConnectEx.m_GSIP, "%s", IP);
	gConnectEx.m_GSPort = Port;
	// ----
	return pCreateConnect(IP, Port);
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::ClearGame()
{
	if( *(BYTE*)(pMUHelperClass + 8) )
	{
		this->m_WantMUHelper = true;
		pMUHelperClose(pMUHelperThis());
	}
	// ----
	pPartyMemberCount = 0;
	*(BYTE*)(oUserPreviewStruct + 780)	= 0; //gObjUser.lpViewPlayer->m_Model.Unknown4 = 0;
	pPShopSet(pPShopThis2(pPShopThis1()), 0);
	pPShopRefresh(oUserPreviewStruct);
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::RestoreGame()
{
#ifdef __MUANGEL__
	this->m_ReconnectTick	= 0;
#endif
	// ----
	this->m_LastSendTick	= 0;
	this->m_ConnectState	= ConnectExType::OnLine;
	// ----
	if( this->m_WantMUHelper )
	{
		this->m_WantMUHelper = false;
		// ----
		if( !gObjUser.lpViewPlayer->InSafeZone )
		{
			pMUHelperStart(pMUHelperThis());
		}
	}
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::DrawProgres()
{
	float StartX = (MAX_WIN_WIDTH / 2.0f) - (160.0f / 2.0f);
	// ----
	Counter++;
	// ----
	if( Counter > 150 )
	{
		Counter = 0;
	}
	// ----
	pDrawGUI(0x7A65, StartX, 100.0f, 160.0f, 18.0f);
	pDrawGUI(0x7A66, StartX + 5.0f, 105.0f, Counter, 8.0f);
	// ----
	if( pOnLine == 0 )
	{
		pDrawColorText("Reconnecting...", StartX, 105, 160, 0, eBlue, 0, 3);
	}
	else if( pOnLine == 1 )
	{
		pDrawColorText("Relogin...", StartX, 105, 160, 0, eBlue, 0, 3);
	}
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::Reconnect()
{
	PMSG_ANS_SERVER_INFO pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xF4, 3, sizeof(PMSG_ANS_SERVER_INFO));
	pRequest.Port = this->m_GSPort;
	memcpy(pRequest.Ip, this->m_GSIP, 16);
	pDataRecv(pRequest.h.headcode, (LPBYTE)&pRequest, pRequest.h.size, 0);
}
// ----------------------------------------------------------------------------------------------

void ConnectEx::Relogin()
{
	CONNECTEX_LOGIN pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 12, sizeof(CONNECTEX_LOGIN));
	memcpy(pRequest.AccountID, this->m_AccountID, 10);
	memcpy(pRequest.Password, this->m_Password, 10);
	memcpy(pRequest.Name, gObjUser.lpPlayer->Name, 10);
	pRequest.TickCount = GetTickCount();
	gProtocol.DataSend((LPBYTE)&pRequest, pRequest.h.size);
}
// ----------------------------------------------------------------------------------------------