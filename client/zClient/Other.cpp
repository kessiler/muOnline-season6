#include "stdafx.h"
#include "Other.h"
#include "TMemory.h"
#include "Import.h"
#include "User.h"
#include "ScriptEncode.h"
#include "TDebugLog.h"
#include "Interface.h"
#include "Camera.h"
// ----------------------------------------------------------------------------------------------

Other	gOther;
DWORD	GuildAssistEx_Buff;
// ----------------------------------------------------------------------------------------------

Naked(GuildAssistEx)
{
	_asm
	{
		mov GuildAssistEx_Buff, ecx
	}
	// ----
	if( GuildAssistEx_Buff == 128 || GuildAssistEx_Buff == 64 )
	{
		_asm
		{
			mov GuildAssistEx_Buff, 0x0078F4AD
			jmp GuildAssistEx_Buff
		}
	}
	else
	{
		_asm
		{
			mov GuildAssistEx_Buff, 0x0078F42A
			jmp GuildAssistEx_Buff
		}
	}
}
// ----------------------------------------------------------------------------------------------

void Other::Load()
{
#ifdef __NOVUS__
	SetRange((LPVOID)0x0078F41E, 6, ASM::NOP);
	SetOp((LPVOID)0x0078F41E, GuildAssistEx, ASM::JMP);
#endif
	// ----
	SetOp((LPVOID)0x004D2180, (LPVOID)this->GameLoad, ASM::CALL);
	// ----
	SetOp((LPVOID)0x00830A56, (LPVOID)this->MoveListInit, ASM::CALL);
	SetOp((LPVOID)0x00832D88, (LPVOID)this->MoveListInit, ASM::CALL);
	// ----
	this->Crack();
	this->Changer();
}
// ----------------------------------------------------------------------------------------------

void Other::Crack()
{
	SetByte((PVOID)0x004D1E69, 0xEB);			//-> Disable MU.exe call
	SetByte((PVOID)0x004D2246, 0xEB);			//-> Disable Config.ini
	// ----
	SetWord((PVOID)(0x0040B4BA + 2), 0x350);	//-> Disable CMStarter
	SetWord((PVOID)(0x0040B4C3 + 2), 0x350);	//-> Disable CMStarter
	SetByte((PVOID)(0x0040B4CD + 2), 0x18);		//-> Disable CMStarter
	// ----
	SetByte((PVOID)0x004D9D24, 0xEB);			//-> Disable web check
	SetByte((PVOID)(0x0040AF09 + 1), 0x00);		//-> Enable account field
	// ----
	SetByte((PVOID)0x004D559C, 0xEB);
	SetByte((PVOID)0x00633F7A, 0xEB);
	SetByte((PVOID)0x00634403, 0xEB);
	SetByte((PVOID)0x0063E6C4, 0xEB);
	SetRange((PVOID)0x004D556F, 5, ASM::NOP);	//-> Disable ResourceGuard Call
	SetRange((PVOID)0x00633F4D, 5, ASM::NOP);	//-> Disable ResourceGuard Call
	SetRange((PVOID)0x006343D6, 5, ASM::NOP);	//-> Disable ResourceGuard Call
	SetRange((PVOID)0x0063E697, 5, ASM::NOP);	//-> Disable ResourceGuard Call
}
// ----------------------------------------------------------------------------------------------

void Other::Changer()
{
	VMBEGIN
	// ----
#ifdef __ROOT__
	SetByte((PVOID)0x0095CE90, 0xEB);
	SetRange((PVOID)0x0095DE51, 2, ASM::NOP);
#endif
	// ----
	char IP[20];
	// ----
	if( !g_ScriptEncode.ReadScript("Data\\Custom\\Common.z") )
	{
		MessageBox(NULL, "file not found", "[Common]", ERROR);
		return;
	}
	// ----
	g_ScriptEncode.GetToken();
	memcpy(IP, g_ScriptEncode.GetString(), sizeof(IP)-1);
	// ----
	WriteMemory((LPVOID)0x00E611B2, IP, sizeof(IP));
	//char ii[] = "192.168.0.100";	//DeBug
	//WriteMemory((LPVOID)0x00E611B2, ii, sizeof(ii));
	// ----
	SetByte((PVOID)oCharColor_Loot, 8);
	SetByte((PVOID)oVulcanusMove, 0);
	//-> Money transfer limit
	SetByte((PVOID)(0x007AF025 + 1), 9);
	SetByte((PVOID)(0x007AF2C5 + 1), 9);
	SetByte((PVOID)(0x007AF645 + 1), 9);
	SetByte((PVOID)(0x007B01C5 + 1), 9);
	//-> Speed bug fix
	//SetByte((PVOID)(0x00649E24 + 3), 14);	//-> 1 Method
	//SetByte((PVOID)(0x00556C32 + 6), 2);	//-> 2 Method
	//-> Siege minimap fix
	SetRange((PVOID)0x006505FB, 4, ASM::NOP);
	//-> Character delete level
	SetDword((PVOID)(0x00405B7A+2), 401); // Delete rule
	SetDword((PVOID)(0x0040CC3F+1), 401); // Error message
	// ----
	static char* LogName	= "Logs\\Error.log";
	static char* LogName2	= "Logs\\Error_%d.log";
	static char* DumpName	= "Logs\\Error.dmp";
	static char* ScreenName	= "Images\\Screen(%02d_%02d-%02d_%02d)-%04d.jpg";
	// ----
	SetDword((PVOID)(0x0096A8C8+1), (DWORD)LogName);
	SetDword((PVOID)(0x0096A94F+1), (DWORD)LogName);
	SetDword((PVOID)(0x0096A9DB+1), (DWORD)LogName2);
	SetDword((PVOID)(0x004D1D0B+1), (DWORD)DumpName);
	SetDword((PVOID)(0x004D9F54+1), (DWORD)ScreenName);
	// ----
	VMEND
}
// ----------------------------------------------------------------------------------------------

int Other::GameLoad()
{
#if defined __BEREZNUK__ || __ALIEN__
	pGameLoad();
	// ----
	switch(pGameResolutionMode)
	{
	case 5:
		pWinWidth		= 1280;
		pWinHeight		= 720;
		break;
		// --
	case 6:
		pWinWidth		= 1280;
		pWinHeight		= 800;
		break;
		// --
	case 7:
		pWinWidth		= 1366;
		pWinHeight		= 768;
		break;
		// --
	case 8:
		pWinWidth		= 1440;
		pWinHeight		= 900;
		break;
		// --
	case 9:
		pWinWidth		= 1680;
		pWinHeight		= 1050;
		break;
		// --
	case 10:
		pWinWidth		= 1920;
		pWinHeight		= 1080;
		break;
		// --
	case 11:
		pWinWidth		= 1920;
		pWinHeight		= 1200;
		break;
		// --
	}
	// ----
	pWinFontHeight	= 15;
	pWinWidthReal	= (float)pWinWidth / MAX_WIN_WIDTH;
	pWinHeightReal	= (float)pWinHeight / MAX_WIN_HEIGHT;
	SetDouble((LPVOID)oCam_ClipX, gCamera.Default.ClipX + 500);
	SetFloat((LPVOID)oCam_ClipGL, gCamera.Default.ClipGL + 200);
	// ----
	return 1;
#else
	return pGameLoad();
#endif
}
// ----------------------------------------------------------------------------------------------

void Other::MoveListInit(LPVOID This, LPVOID EDX, int Arg1, int Arg2)
{
#if defined __BEREZNUK__ || __ALIEN__
	GLsizei TempWidth = pWinWidth;
	pWinWidth = 1280;
	pMoveListInit(This, Arg1, Arg2);
	pWinWidth = TempWidth;
#else
	return pMoveListInit(This, Arg1, Arg2);
#endif
}
// ----------------------------------------------------------------------------------------------