//GS-N 1.00.90 - 0xXXXXXXXX  - complete
//GS-CS 1.00.90 - 0xXXXXXXXX  - complete
#include "stdafx.h"
#include "MonsterHerd.h"
#include "resource.h"
#include "GameServer.h"
#include "Gamemain.h"
#include "Giocp.h"
#include "logproc.h"
//#include "GameServerAuth.h"
#include "RingAttackEvent.h"
#include "XMasAttackEvent.h"
#include "CWhatsUpDummyServer.h"
#include "DBSockMng.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "SProtocol.h"
#include "ChaosCastle.h"
#include "Protocol.h"
#include "AcceptIP.h"
#include "user.h"
#include "closeplayer.h"
#include "MapServerManager.h"
#include "protocol.h"
#include "Shop.h"
#include "Weapon.h"
#include "CastleSiegeSync.h"
#include "BloodCastle.h"
#include "EledoradoEvent.h"
#include "EventManagement.h"
#include "CashShop.h"
#include "TServerInfoDisplayer.h"
#include "TMonsterAI.h"
#include "Kanturu.h"
#include "IllusionTempleEvent.h"
#include "MiniDump.h"
#include "Raklion.h"
#include "gObjMonster.h"
#include "ResetSystem.h"
#include "TemporaryUserManager.h"
#include "MonsterRegenSystem.h"
#include "CheatGuard.h"

#ifdef NPVP
#include "NewPVP.h" // 1.00.92
#endif

#ifdef GENS
#include "GensSystem.h"
#endif

#ifdef PERIOD
#include "PeriodItemEx.h"
#endif

#ifdef IMPERIAL
#include "ImperialGuardian.h"
#endif

#ifdef DP
#include "Doppelganger.h"
#endif

#include "MUHelper.h"

#ifdef __CUSTOMS__
#include "ItemPrice.h"
#include "BalanceSystem.h"
#include "ResetSystem.h"
#include "MixOption.h"
#include "NewsBoard.h"
#include "PVPZone.h"
#include "DropEx.h"
#include "ChatFilter.h"
#include "JewelsEx.h"
#include "ShopPointEx.h"
#include "OfflineTrade.h"
#include "PKClear.h"
#include "ClassCalc.h"
#include "DropEvent.h"
#ifdef QUESTSYSTEM
#include "QuestSystem.h"
#endif
#endif
#ifdef __NOVUS__
#include "CraftSystem.h"
#endif
#ifdef __MAKELO__
#include "OfflineAttack.h"
#endif
#ifdef __ALIEN__
#include "ConMember.h"
#include "ItemOption.h"
#include "MonsterSpawner.h"
#endif

#include "GameShop.h"
#include "SkillAdditionInfo.h"

#ifdef OFFEXP
#include "OffExp.h"
#endif

char ExDbIp[256];
char DataServerIp2[256];
char DataServerIp[256];
char JoinServerIp[256];
int gServerReady;
int gCloseMsg;
int gCloseMsgTime;
int gCurPaintPlayer;
int gCurPaintType;
int gCurPaintMapNumber;

CRingMonsterHerd g_MonsterHerdTest;
CRingAttackEvent g_RingAttackEvent;
CXMasAttackEvent g_XMasAttackEvent;
CWhatsUpDummyServer gWhatsUpDummyServer;


HINSTANCE hInst;
HWND ghWnd;

char szTitle[100];
char szWindowClass[64];

HWND g_hCsLogDlgProc = NULL;

BOOL GameServerStart(void);
int InitInstance(HINSTANCE, int);
ATOM MyRegisterClass(HINSTANCE);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK About(HWND hInst, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef VMPROTECT
	VMBEGIN
	if( VMProtectIsDebuggerPresent(TRUE) != 0 )
	{
		g_License.SendAlert(3, 0);
		g_License.Destruct();
		ExitProcess(0);
	}
	VMEND
#endif
#if defined __WHITE__ || __BEREZNUK__
	LoadLibraryA("NSEngine.dll");
#endif
	// ----
	CMiniDump::Begin();
	MSG msg;
	HACCEL hAccelTable;
	// ----
	if( gLanguage == 0 ) 
	{
		WIN32_FIND_DATA  wfd;
		if( FindFirstFile(COMMONSERVER_PATH,&wfd) == INVALID_HANDLE_VALUE )
		{
			MessageBox(NULL, "Cannot find M:\\ Drive.", "Error", MB_OK|MB_ICONHAND|MB_APPLMODAL);
			return 0;
		}
	}	
	// ----
	LoadString(hInstance, IDS_APP_TITLE, szTitle, 100);
	LoadString(hInstance, IDS_GAMESERVER, szWindowClass, 100);
	// ----
	MyRegisterClass(hInstance);
	InitCommonControls();
	// ----
	if( !InitInstance(hInstance, nCmdShow) )
	{
		return 0;
	}
	// ----
	ShowWindow(ghWnd, SW_SHOWNORMAL);
	hAccelTable = LoadAccelerators(hInstance, (LPCSTR)IDA_GAMESERVER);
	// ----
	GiocpInit();
	GameMainInit(ghWnd);
	// ----
	memset(JoinServerIp, 0, sizeof(JoinServerIp));
	memset(DataServerIp, 0, sizeof(DataServerIp));
	memset(DataServerIp2, 0, sizeof(DataServerIp2));
	memset(ExDbIp, 0, sizeof(ExDbIp));
	GameServerPort	= 0;
	GameServerUpdPort = 0;
	JoinServerPort	= 0;
	DataServerPort	= 0;
	DataServerPort2 = 0;
	ExDbPort		= 0;
	// ----
	GameServerPort		= GetPrivateProfileIntA("Connection", "ServerPort", 55901, SERVER_INFO_PATH);
	GameServerUpdPort	= GetPrivateProfileIntA("Connection", "UpdatePort", 60006, SERVER_INFO_PATH);
	GetPrivateProfileStringA("Connection", "JoinServerIP", "127.0.0.1", JoinServerIp, sizeof(JoinServerIp), SERVER_INFO_PATH);
	JoinServerPort	= GetPrivateProfileIntA("Connection", "JoinServerPort", 55970, SERVER_INFO_PATH);
	GetPrivateProfileStringA("Connection", "DataServer1IP", "127.0.0.1", DataServerIp, sizeof(DataServerIp), SERVER_INFO_PATH);
	DataServerPort	= GetPrivateProfileIntA("Connection", "DataServer1Port", 55960, SERVER_INFO_PATH);
	GetPrivateProfileStringA("Connection", "DataServer2IP", "127.0.0.1", DataServerIp2, sizeof(DataServerIp2), SERVER_INFO_PATH);
	DataServerPort2	= GetPrivateProfileIntA("Connection", "DataServer2Port", DataServerPort+2, SERVER_INFO_PATH);
	GetPrivateProfileStringA("Connection", "ExDBServerIP", "127.0.0.1", ExDbIp, sizeof(ExDbIp), SERVER_INFO_PATH);
	ExDbPort		= GetPrivateProfileIntA("Connection", "ExDBServerPort", 55906, SERVER_INFO_PATH);
	GetPrivateProfileStringA("Connection", "RankingServerIP", "127.0.0.1", RankingServerIP, sizeof(RankingServerIP), SERVER_INFO_PATH);
	RankingServerPort		= GetPrivateProfileIntA("Connection", "RankingServerPort", 44455, SERVER_INFO_PATH);
	GetPrivateProfileStringA("Connection", "EventServerIP", "127.0.0.1", EventServerIP, sizeof(EventServerIP), SERVER_INFO_PATH);
	EventServerPort		= GetPrivateProfileIntA("Connection", "EventServerPort", 44456, SERVER_INFO_PATH);
	// ----
	gWhatsUpDummyServer.Start(ghWnd, GameServerPort+1);
	AllServerStart();
	// ----
	while( GetMessage(&msg, NULL, 0, 0) != 0 )
	{
		if( !TranslateAccelerator(msg.hwnd, hAccelTable, &msg) )
		{
		    TranslateMessage(&msg);
		    DispatchMessage(&msg);
		}
	}
	// ----
	return msg.wParam;
}

BOOL AllServerStart(void){
	
	if (JoinServerConnected==0)
	{
		if (GMJoinServerConnect(JoinServerIp, WM_GM_JOIN_CLIENT_MSG_PROC) == 0)
		{	
			// LoginServer connect fail
			MsgBox(lMsg.Get(MSGGET(1, 144)));
			return 0;
		}
	}

	if ((DevilSquareEventConnect==0) && (IsDevilSquareEventConnected==0))
	{
		if (GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) == 0)
		{
			MsgBox("Ranking Server?? ???? ??µ?");
			return 0;
		}
		IsDevilSquareEventConnected=1;
	}

	if ((EventChipServerConnect!=0) && (IsEventChipServerConnected==0))
	{
		if (GMEventChipServerConnect(gEventChipServerIp, WM_GM_EVENTCHIP_CLIENT_MSG_PROC) == 0)
		{
			MsgBox("Event Chip Server?? ???? ??µ?");
			return 0;
		}
		IsEventChipServerConnected=1;
	}

	
	cDBSMng.Connect();
	return TRUE;
}

BOOL GameServerStart(void) 
{
	if (GameMainServerCreate(WM_GM_SERVER_MSG_PROC, WM_GM_CLIENT_MSG_PROC) == FALSE )
	{
		MsgBox(lMsg.Get(MSGGET( 1, 142) ));
		return FALSE;
	}

	CreateGIocp(GameServerPort);

	SetTimer(ghWnd, WM_LOG_PAINT, 2000, NULL);
	SetTimer(ghWnd, WM_FIRST_MSG_PROCESS, 1000, NULL);

	SetTimer(ghWnd, WM_SET_DATE, 60000, NULL);
	SetTimer(ghWnd, WM_LOG_DATE_CHANGE, 60000, NULL);
	SetTimer(ghWnd, WM_CONNECT_DATASERVER, 10000, NULL);

	SetTimer(ghWnd, WM_SECOND_MSG_PROCESS, 1000, NULL);
	SetTimer(ghWnd, WM_GS_CLOSE, 1000, NULL);
	SetTimer(ghWnd, WM_MONSTER_AND_MSG_PROC, 500, NULL);

	SetTimer(ghWnd, WM_MOVE_MONSTER_PROC, 300, NULL);
	SetTimer(ghWnd, WM_EVENT_RUN_PROC, 100, NULL);

	SetTimer(ghWnd, WM_AI_MONSTER_MOVE_PROC, 100, NULL);
	SetTimer(ghWnd, WM_AI_MONSTER_PROC, 1011, NULL);

	SetTimer(ghWnd, WM_GUILD_LIST_REQUEST, 1000, NULL);

#ifdef __ALIEN__
	g_ConnectMemberUpdate = GetPrivateProfileInt("Common", "ConnectMemberUpdate", 0, gDirPath.GetNewPath("Custom\\Aliennation.ini"));
	SetTimer(ghWnd, WM_CONNECTMEMBERUPDATE, g_ConnectMemberUpdate * 1000, NULL);
#endif

	return TRUE;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_NORMAL_V1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDR_MENU1;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON_MINIMIZED);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	HWND hWnd;
	hInst=hInstance;

#ifdef ZEONWINDOW
    hWnd = CreateWindowEx(
        0,
        szWindowClass,
        szTitle,
        WS_OVERLAPPED+WS_CAPTION+WS_SYSMENU+WS_MINIMIZEBOX+WS_VISIBLE,
        CW_USEDEFAULT, 0, GAMESERVER_WIDTH, GAMESERVER_HEIGHT,
        NULL, NULL, hInstance, NULL);
#else
	hWnd = CreateWindowEx(
        0,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT,0,
        NULL, NULL, hInstance, NULL);
#endif

	if (hWnd==0) 
	{
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	ghWnd=hWnd;
	return TRUE;
}

BOOL CALLBACK CsLogDlgProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT wmId, wmEvent;
	// ----
	switch(message) 
	{
	case WM_CREATE:
		{
			g_hCsLogDlgProc = CreateDialogParam(hInst, LPCTSTR(IDD_CASTLE_SIEGE_LOG), hWnd, CsLogDlgProc, 0);
			return 0;
		}
		break;
		// --
	case WM_COMMAND:
		{
			wmId    = wParam & 0xFFFF & 0xFFFF; 
			wmEvent = (wParam >> 16) & 0xFFFF; 
			// ----
			switch(wmId)
			{
			case ID_FILE_EXIT:
				SendMessage(hWnd, WM_CLOSE, 0, 0);
				break;
				// --
			case ID_RELOAD_DATA:
				ReadCommonServerInfo();
				break;
				// --
			case ID_RELOAD_CUSTOM:
				g_CheatGuard.Load();
#ifdef __CUSTOMS__
				LoadMapSettings(gDirPath.GetNewPath("Custom\\MapData.txt"));
				g_ResetSystem.Load();
				g_ItemPrice.Load();
				g_MixOption.Load();
				g_NewsBoard.Load();
				g_PVPZone.Load();
				g_DropEx.Load();
				g_ChatFilter.Load();
				g_JewelsEx.Load();
				g_OfflineTrade.Load();
				g_PKClear.Load();
				g_ClassCalc.Load();
				gBalanceSystem.Load();
#if defined __REEDLAN__ || __BEREZNUK__
				g_ShopPointEx.Load();
#endif
#ifdef __NOVUS__
				g_CraftSystem.Load();
#endif
#ifdef __MAKELO__
				g_OfflineAttack.Load();
#endif

				
#ifdef __ALIEN__
				gItemOption.Init();
				//gMonsterSpawner.Load();
				g_ComboAttackPower		= GetPrivateProfileInt("Common", "ComboAttackPower", 0, gDirPath.GetNewPath("Custom\\Aliennation.ini"));
				g_SuperPanda			= GetPrivateProfileInt("Common", "SuperPanda", 0, gDirPath.GetNewPath("Custom\\Aliennation.ini"));
				g_ConnectMemberUpdate	= GetPrivateProfileInt("Common", "ConnectMemberUpdate", 0, gDirPath.GetNewPath("Custom\\Aliennation.ini"));
				g_HarmonyOptionOnSocket = GetPrivateProfileInt("Common", "HarmonyOptionOnSocket", 0, gDirPath.GetNewPath("Custom\\Aliennation.ini"));
				g_GuildCreateReset		= GetPrivateProfileInt("Common", "GuildCreateReset", 0, gDirPath.GetNewPath("Custom\\Aliennation.ini"));
				KillTimer(ghWnd, WM_CONNECTMEMBERUPDATE);
				SetTimer(ghWnd, WM_CONNECTMEMBERUPDATE, g_ConnectMemberUpdate * 1000, NULL);
#endif
#ifdef QUESTSYSTEM
				g_QuestSystem.Load();
#endif
#endif
				break;
				// --
			case ID_RELOAD_EVENT:
				if(		g_CastleSiege.Ready(g_MapServerManager.GetMapSvrGroup())
					&&	g_CastleSiege.LoadData(gDirPath.GetNewPath("Event\\CastleSiege.dat")) )
				{
						g_CastleSiege.LoadPreFixData(gDirPath.GetNewPath("CommonServer.cfg"));
						g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);
				}
				// ----
				ReadEventInfo(MU_EVENT_ALL);
				ReadCommonServerInfo();
				break;
				// --
			case ID_RELOAD_GAMESHOP:
				gGameShop.Load();
				break;
				// --
			case ID_RELOAD_ITEM:
				LoadItemBag();
				break;
				// --
			case ID_RELOAD_MONSTER:
				GameMonsterAllCloseAndReLoad();
				break;
				// --
			case ID_RELOAD_SHOP:
				ShopDataLoad();
				break;
				// --
			case ID_RELOAD_SKILL:
				g_SkillAdditionInfo.Load("Skill\\SkillAdditionInfo.dat");
				break;
				// --
			case ID_PLAYERS_CLOSEALL:
				gObjAllDisconnect();
				break;
				// --
			case ID_CONNECTION_LOGOUTALL:
				gObjAllLogOut();
				break;
				// --
			case ID_CONNECTION_AUTOCLOSE1:
				if( gCloseMsg == 0 )
				{
					gCloseMsgTime	= 60;
					gCloseMsg		= 3;
					AllSendServerMsg(lMsg.Get(MSGGET(4, 106)));
				}
				break;
				// --
			case ID_CONNECTION_AUTOCLOSE2:
				if( gCloseMsg == 0 )
				{
					gCloseMsgTime	= 180;
					gCloseMsg		= 2;
					AllSendServerMsg(lMsg.Get(MSGGET(4, 105)));
				}
				break;
				// --
			case ID_CONNECTION_AUTOCLOSE3:
				if( gCloseMsg == 0 )
				{
					gCloseMsgTime	= 300;
					gCloseMsg		= 1;
					AllSendServerMsg(lMsg.Get(MSGGET(4, 104)));
				}
				break;
				// --
			case ID_SLOT_INCREASEBY10:
				gServerMaxUser += 10;
				if( gServerMaxUser > OBJMAXUSER-1 ) gServerMaxUser = OBJMAXUSER-1;
				break;
				// --
			case ID_SLOT_INCREASEBY50:
				gServerMaxUser += 50;
				if( gServerMaxUser > OBJMAXUSER-1 ) gServerMaxUser = OBJMAXUSER-1;
				break;
				// --
			case ID_SLOT_INCREASEBY100:
				gServerMaxUser += 100;
				if( gServerMaxUser > OBJMAXUSER-1 ) gServerMaxUser = OBJMAXUSER-1;
				break;
				// --
			case ID_SLOT_DECREASEBY10:
				gServerMaxUser -= 10;
				if( gServerMaxUser < 0 ) gServerMaxUser = 0;
				break;
				// --
			case ID_SLOT_DECREASEBY50:
				gServerMaxUser -= 50;
				if( gServerMaxUser < 0 ) gServerMaxUser = 0;
				break;
				// --
			case ID_SLOT_DECREASEBY100:
				gServerMaxUser -= 100;
				if( gServerMaxUser < 0 ) gServerMaxUser = 0;
				break;
				// --
			case ID_HELP_ABOUT:
				DialogBox(hInst, (LPCTSTR)IDD_ABOUT_BOX, hWnd, (DLGPROC)About);
				break;
				// --
			case IDM_LOG_PAINT:
				gCurPaintType=0;
				break;
				// --
			case IDM_LOG_CASTLESIEGE:
				SendMessage(GetDlgItem(g_hCsLogDlgProc,	IDE_EDIT_LOG), LB_ADDSTRING, 0, (rand() % 2) ? (LONG)"HELLO WORLD" : (LONG)"????");
				ShowWindow(g_hCsLogDlgProc, SW_SHOWNORMAL);
				break;
				// --
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
				break;
			}
		}
		break;
		// --
		case WM_GM_SERVER_MSG_PROC:
			GMServerMsgProc(wParam, lParam);
			break;
		case WM_GM_CLIENT_MSG_PROC:
			{
				int wp = wParam;
				int lp = lParam;
				GMClientMsgProc(wParam, lParam);
			}
			break;
		case WM_GM_JOIN_CLIENT_MSG_PROC:
			GMJoinClientMsgProc(wParam, lParam);
			break;
		case WM_GM_RANKING_CLIENT_MSG_PROC:
			GMRankingClientMsgProc(wParam, lParam);
			break;
		case WM_GM_EVENTCHIP_CLIENT_MSG_PROC:
			GMEventChipClientMsgProc(wParam, lParam);
			break;
		case WM_GM_DATA1_CLIENT_MSG_PROC:
			cDBSMng.MsgProc(0, wParam, lParam);
			break;
		case WM_GM_DATA2_CLIENT_MSG_PROC:
			cDBSMng.MsgProc(1, wParam, lParam);
			break;
		case WM_GM_EXDATA_CLIENT_MSG_PROC:
			ExDataClientMsgProc(wParam, lParam);
			break;
		case WM_START_SERVER:
			if ( gServerReady == 2 )
				GameServerStart();
			break;
		case WM_TIMER:
			switch ( wParam )
			{
#ifdef __ALIEN__
			case WM_CONNECTMEMBERUPDATE:
				ConMember.Load(gDirPath.GetNewPath("Other\\ConnectMember.txt"));
				break;
#endif
			case WM_GUILD_LIST_REQUEST:
				DGGuildListRequest();
				KillTimer(ghWnd, WM_GUILD_LIST_REQUEST);
				break;
			case WM_LOG_PAINT:
				{
					RECT rect;
					HDC hdc = GetDC(hWnd);
					GetClientRect(hWnd, &rect);
					FillRect(hdc, &rect, (HBRUSH)GetStockObject(0));
					ReleaseDC(hWnd, hdc);

					if ( gCurPaintType == 0)
					{
						if ( LogTextPaint != NULL )
						{
							LogTextPaint(hWnd);
						}
					}

					g_ServerInfoDisplayer.Run(hWnd);
					gObjViewportPaint(hWnd, gCurPaintPlayer);
					GJNotifyMaxUserCount();
				}
				break;
			case WM_MONSTER_AND_MSG_PROC:
				MonsterAndMsgProc();
#ifdef NPVP
				g_NewPVP.Run();
#endif
				break;
			case WM_MOVE_MONSTER_PROC:
				MoveMonsterProc();
				g_Kanturu.UserMonsterCountCheck();
				break;
			case WM_EVENT_RUN_PROC:
#ifdef DP
				g_DoppleganerEvent.MoveDoppelgangerMonsterProc();
#endif
				g_BloodCastle.Run();
				g_RingAttackEvent.Run();
				g_ChaosCastle.Run();
				g_CastleSiege.Run();
				g_CastleDeepEvent.Run();
				g_CsNPC_Weapon.WeaponAttackProc();
				g_Crywolf.Run();
				g_Kanturu.Run();
				g_IllusionTempleEvent.IllusionTempleRun();
				g_Raklion.Run();
				g_XMasAttackEvent.Run();
#ifdef DP
				g_DoppleganerEvent.ProcDoppelganger((int)GetTickCount());
#endif
#ifdef IMPERIAL
				g_ImperialGuardian.Run();
#endif
				g_MonsterRegenSystem.Run();
				break;
			case WM_AI_MONSTER_MOVE_PROC:
				TMonsterAI::MonsterMoveProc();
#ifdef OFFEXP
				OffExp.MainFunction();
#endif
				break;
			case WM_AI_MONSTER_PROC:
				TMonsterAI::MonsterAIProc();
				break;
			case WM_FIRST_MSG_PROCESS:
				{
					int n;
					// ----
					for ( n = 0; n < OBJMAX; n++ )
					{
						gObjStateSetCreate(n);
					}
					for ( n = 0; n < OBJMAX; n++ )
					{
						gObjViewportListDestroy(n);
					}
					for ( n = 0; n < OBJMAX; n++ )
					{
						gObjViewportListCreate(n);
					}
					for ( n = 0; n < OBJMAX; n++ )
					{
						gObjViewportListProtocol(n);
					}

					if ( gDoPShopOpen != FALSE )
					{
						for ( n = OBJ_STARTUSERINDEX; n < OBJMAX; n++ )
						{
							PShop_ViewportListRegenarate(n);
						}
						for ( n = OBJ_STARTUSERINDEX; n < OBJMAX; n++ )
						{
							if (gObjIsConnected(n) != PLAYER_EMPTY )
							{
								gObj[n].m_bPShopItemChange = false;
							}
						}
					}

					for ( n = OBJ_STARTUSERINDEX; n < OBJMAX; n++ )
					{
						gObjUnionUpdateProc(n);
					}

					gObjSetState();
					GameServerInfoSend();
				}
				break;
			case WM_SECOND_MSG_PROCESS:
				{
					for (int n = 0;n<MAX_NUMBER_MAP;n++)
					{
						MapC[n].WeatherVariationProcess();
					}

					wsGServer.m_SendSec = wsGServer.m_SendSecTmp;
					wsGServer.m_RecvSec = wsGServer.m_RecvSecTmp;

					wsGServer.m_SendSecTmp = 0;
					wsGServer.m_RecvSecTmp = 0;

					DragonEvent->Run();
					AttackEvent->Run();
					gEledoradoEvent.Run();
					g_EventManager.Run();
#if __ALIEN__
					gMonsterSpawner.TickTime();
#endif

#ifdef __CUSTOMS__
					DropEvent.TickTime();
#endif
				}
				break;
			case WM_GS_CLOSE:
				if ( gCloseMsg != 0 )
				{
					gCloseMsgTime--;

					if ( gCloseMsgTime <= 1 )
					{
						if ( gCloseMsgTime == 1 )
						{
							AllSendServerMsg(lMsg.Get(MSGGET(1, 193)));
							LogAddC(2, lMsg.Get(MSGGET(1, 193)));
						}
					}
					else
					{
						if ( (gCloseMsgTime % 10) == 0 )
						{
							char szTemp[256];

							wsprintf( szTemp, lMsg.Get(MSGGET(1, 194)), gCloseMsgTime);
							AllSendServerMsg(szTemp);
							LogAddC(2, szTemp);
						}
					}

					if ( gCloseMsgTime < 0 )
					{
						KillTimer(ghWnd, WM_GS_CLOSE);
						gObjAllLogOut();
						gCloseMsgTime = 0;
						gCloseMsg = 0;
					}
				}
				gObjSecondProc();
				//#if (_GSCS == 1 )
				if( cDBSMng.GetActiveDS() > DS_UNINITIALIZED && g_CastleSiege.GetDataLoadState() == 2 )
				{
					g_CastleSiege.DataRequest();
				}

				g_CastleSiege.SendCastleStateSync();

				//#endif
				g_CastleSiegeSync.AdjustTributeMoney();
				//#if(_GSCS == 1)
				g_Crywolf.CrywolfSecondAct();
				//#endif
#ifdef PERIOD
				g_PeriodItemEx.CheckPeriodData();
#endif
				/* CCashShopInGame::CheckMileageUpdate(&g_CashShopInGame);
				CCashShopInGame::CheckShopServerReconnect(&g_CashShopInGame);*/
				//1.01.03
				g_HacktoolBlockEx.CheckHacktoolUse();
				break;
			case WM_SET_DATE:
				gSetDate();
				WhisperCash.TimeCheckCashDelete();
				break;
			case WM_CONNECT_DATASERVER:
				cDBSMng.Connect();
#ifndef NPVP
				gObjCheckAllUserDuelStop();
#endif
				break;
			case WM_LOG_DATE_CHANGE:
				CTemporaryUserManager::Instance()->ClearOverTimeUserData(60);
				//#if (_GSCS == 1 )
				if( LogDateChange() == TRUE )
				{
					g_iCastleItemMixLimit = 1;
				}
				//#else
				//					LogDateChange();
				//#endif
				break;
			}
			break;
		case WM_CLOSE:
			if ( gLanguage != 3 )
			{
				if (MessageBox(ghWnd, "GameServer close?", "Close", MB_YESNO|MB_APPLMODAL) == IDYES)
				{
					DestroyWindow(hWnd);
				}
			}
			else
			{
				DestroyWindow(hWnd);
			}
			break;
		case WM_KEYDOWN:
			switch ( wParam )
			{
			case VK_F1 :
				gCurPaintPlayer--;
				if( gCurPaintPlayer < 0 ) gCurPaintPlayer = 0;
				break;
			case VK_F2 :

				break;
			case VK_F4 :
				//GameMainServerCreate(WM_ASS_SERVERMSG, WM_ASS_CLIENTMSG);
				//TestDSSend();
				{
					/*PMSG_IDPASS Msg;
					strcpy(Msg.Id,"saemsong");
					strcpy(Msg.Pass,"ssman");	
					Msg.CliVersion[0] = szClientVersion[0];
					Msg.CliVersion[1] = szClientVersion[1];
					Msg.CliVersion[2] = szClientVersion[2];
					CSPJoinIdPassRequest(&Msg, 0);
					*/
				}
				break;
			case VK_F5 : 
				gServerMaxUser-=10;
				if( gServerMaxUser < 10 ) gServerMaxUser = 10;
				break;
			case VK_F6 :
				gServerMaxUser+=10;
				if( gServerMaxUser > 1500 ) gServerMaxUser = 1500;
				break;
			case VK_F7 :
				{
					PMSG_CHARMAPJOIN Msg;
					strcpy(Msg.Name,"?????®");
					CGPCharacterMapJoinRequest(&Msg, 0);
				}
				break;
			case VK_F9 :				
#if( TESTSERVER == 1 )
				{
					TestDSSend();
					PMSG_IDPASS pMsg;
					for( int n=0; n<100; n++)
					{
						CSPJoinIdPassRequestTEST(&pMsg, 0);
					}
				}
#endif
				break;
			case VK_F11:
				{
					DialogBox(hInst, (LPCTSTR)IDD_ABOUT_BOX, hWnd, (DLGPROC)About);
				}
				break;
			}
			break;
		case WM_DESTROY:
			GameMainFree();
			GiocpDelete();
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if ( ( wParam & 0xFFFF & 0xFFFF ) == IDOK || (wParam & 0xFFFF & 0xFFFF ) == IDCANCEL) 
			{
				EndDialog(hDlg, (wParam& 0xFFFF & 0xFFFF ));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

//#if(_GSCS == 1)
BOOL CALLBACK CsLogDlgProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch ( uMessage )
	{
		case WM_INITDIALOG:	
			SendMessage(hWnd, WM_SIZE, 0, 0);
			break;
		case WM_CLOSE:
			ShowWindow(hWnd, SW_HIDE);
			return TRUE;
		case WM_SIZE:
			{
				RECT pRect;
				GetClientRect(hWnd, &pRect);
				MoveWindow(GetDlgItem(hWnd, IDE_EDIT_LOG), pRect.left, pRect.top, pRect.right, pRect.bottom, TRUE);
			}
			return TRUE;
	}
	return FALSE;
}
//#endif