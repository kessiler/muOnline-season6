#include "stdafx.h"
#include "Other.h"
#include "TMemory.h"
#include "Item.h"
#include "User.h"
#include "Controller.h"
#include "Interface.h"
#include "CheatGuard.h"
#include "Protocol.h"
#include "Monster.h"
#include "ResetSystem.h"
#include "ItemPrice.h"
#include "ChatExpanded.h"
#include "ConnectEx.h"
#include "TDebugLog.h"
#ifdef __NOVUS__
#include "CraftSystem.h"
#endif
#include "VisualFix.h"
#include "OffTrade.h"
#include "ItemRank.h"
#include "ItemModel.h"
//#include "SocketItem.h"
// ----------------------------------------------------------------------------------------------

extern "C" __declspec(dllexport)void Init()
{

#ifdef __MUTEX__
	if( pMUIsLoaded == 1 )
	{
		CreateMutex(0, 1, "MuOnline");
		// ----
		if( GetLastError() == 183 )
		{
			ExitProcess(0);
		}
	}
#endif

#ifdef __LAUNCHER__
	char **	Command	= 0;
	CommandLineToArg(GetCommandLine(), &Command);
	if( strcmp("Updater", Command[1]) )
	{
		MessageBox(0, "Please start game from Launcher", "Start Error", ERROR);
		ExitProcess(0);
	}
#endif

#ifdef __NOVUS__
	gCraftSystem.Load();
#endif
#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
	gConnectEx.Load();
#endif
	gController.Load();
	gChatExpanded.Load();
	gItemPrice.Load();
	gItem.Load();
	gResetSystem.Load();
	gProtocol.Load();
	gCheatGuard.Load();
	gObjUser.Load();
	gOther.Load();
	gInterface.Load();
	gVisualFix.InitVisualFix();
	g_ItemModel.Load();
#ifdef __ALIEN__
	gOffTrade.Init();
#endif
#ifdef __RMOS__
	g_ItemRank.Load(); 
#endif
	// ----
	LoadLibrary("ttlci.dll");
	// ----
	//	gSocketItem.Load();
}
// ----------------------------------------------------------------------------------------------

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason)
	{
	case DLL_PROCESS_DETACH:
		{

		}
		break;
	case DLL_PROCESS_ATTACH:
		{
			gController.Instance = hModule;
		}
		break;
	}
	return true;
}