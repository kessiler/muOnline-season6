#pragma once
// ----
#pragma warning(disable: 4786)
// ----
#pragma comment(lib, "wsock32.lib" )
#pragma comment(lib, "ws2_32.lib" )
#pragma comment(lib, "COMCTL32.lib" )
#pragma comment(lib, "../lua/lib/lua51.lib")
// -------------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT			0x500
// -------------------------------------------------------------------------------

//#define LICENSE
//#define VMPROTECT
// ------------------------------------------------------------------------------

#define __CUSTOMS__		1
// ------------------------------------------------------------------------------

#define __ROOT__		1
//#define __BEREZNUK__	2
//#define __MIX__		4
//#define __WHITE__		6
//#define __LEGEND__	7
//#define __ESSIN__		8
//#define __REEDLAN__	9
//#define __MUANGEL__	10
//#define __MEGAMU__	14
//#define __ALIEN__		16
//#define __VIRNET__	19
// ------------------------------------------------------------------------------

#if defined __VIRNET__
#define NEWWINGS
#endif
// ------------------------------------------------------------------------------

#if defined __MEGAMU__
#define NEWWINGS
#define IS_NEWWINGS(x)		((x < ITEMGET(12, 180)) ? FALSE : (x > ITEMGET(12, 193)) ? FALSE : TRUE)
#endif
// ------------------------------------------------------------------------------

#define IS_NEWWINGS(x)		((x < ITEMGET(12, 180)) ? FALSE : (x > ITEMGET(12, 186)) ? FALSE : TRUE)
//#define QUESTSYSTEM		1

#if __ALIEN__
#define OFFEXP
#endif


//#define TVTEVENT
#define DROP_EVENT
// ------------------------------------------------------------------------------

#include <afxwin.h> 
#include <windows.h>
#include <winbase.h>
#include <winsock2.h>
#include "COMMCTRL.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <math.h>
#include <process.h>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <afx.h>
#include "MuMsg.h"
#include "WzAG.h"
#ifdef VM_PROTECT
#include "VMProtectSDK.h"
#endif
// ------------------------------------------------------------------------------

#define OPTIMIZATION
#define NPVP
#define IMPERIAL
#define DP
#define GENS
#define EXPERIENCE_EXTERN			//-> for unlimit exp view
#define HACKCONTROL
#define PACKET_TRACE
#define SEASON5DOT4_ENG				// Season 5.4 ENG update define
#define SEASON6DOT3_ENG				// Season 6.3 ENG update define
#define	MONK						// RF define
#define PERIOD						// Period System
#define TEMP_PERIOD_VAR	0			// Period temp var
#define EXPINV						// Expended inventory & warehouse
#define FIX_MAXSTAT		1			// Support stats to 65k
//#define PCBANG
#define GAMESHOP
#define MUHELPER
#define DEBUG_PROTOCOL			0
#define CHARCREATE_TEMP
#define WZQUEST						// Season 6.2 quest system
// -------------------------------------------------------------------------------

#define MAX_IDSTRING			10
#define PMHC_BYTE				0xC1 // 클라이언트가 보내는 1바이트짜리 프로토콜 
#define PMHC_WORD				0xC2 // 클라이언트가 보내는 2바이트짜리 프로토콜 
#define PMHCE_BYTE				0xC3 // 클라이언트가 보내는 1바이트짜리 프로토콜 
#define PMHCE_WORD				0xC4 // 클라이언트가 보내는 2바이트짜리 프로토콜 
// -------------------------------------------------------------------------------

#define DB_MAX_INVENTORY_SIZE	3776
#define DB_MAX_VAULT_SIZE		3840

#ifdef EXPINV
#define MAX_INVENTORY_H			24
#define MAX_INVENTORY_H2		28
#define MAX_INVENTORY_LEN		192
#define MAX_WAREHOUSE_H			30
#else
//#define DB_MAX_INVENTORY_SIZE	1728
//#define DB_MAX_VAULT_SIZE		1920
#define MAX_INVENTORY_H			8
#define MAX_INVENTORY_H2		12
#define MAX_INVENTORY_LEN		64
#define MAX_WAREHOUSE_H			15
#endif

#define GAMESERVER_VERSION		"1.0.0.99"
#define GAMESERVER_NAME			"zGameServer"
// ----
#define ZEONWINDOW_STAT				// Zeon custom statistic on window
#define ZEONWINDOW					// Zeon custom window for gameserver
#define GAMESERVER_WIDTH		750	// Width of window
#define GAMESERVER_HEIGHT		600 // Height of window
// ----
#define ZEONGAMESERVER				// Zeon custom define
//#define PARTYKILLPKSET			// Allow party kill additional
#define ANTIHACKTOOL				// Enable antihack tool (speed check and etc)
// ----
#define TESTSERVER				0	// Test server status
#define DEBUG_RAKLION			0	// Write debug of Raklion
#define DEBUG_MINIDUMP			0	// Creating dump file
#define DEBUG_IT				0	// Write debug of Illusion Temple
#define DEBUG_BUFF_EFFECT		0	// Write debug of Buff Effect
#define DEBUG_SUMMONER_SKILL	0	// Write debug of Summoner skills
#define DEBUG_EVENT_COUNTER		0	// Write debug of Event counter
#define LOG_INMEDIATLY			0	// ?
// ----
#define REMOVE_CHECKSUM			1	// Removing CheckSum checking
#define USE_M_DRIVE				0	// Test M: drive use
#define GS_LIFE_28_UP			1	// Use 28 life addition option
// ----
#if (USE_M_DRIVE == 1)
#define FINAL_PATH				"M:\\"
#define COMMONSERVER_PATH		"M:\\commonserver.cfg"
#define SERVER_INFO_PATH		"Serverinfo.dat"
#else
#define FINAL_PATH				"..\\Data\\"
#define COMMONSERVER_PATH		"..\\Data\\CommonServer.cfg"
#define SERVER_INFO_PATH		"Data\\Serverinfo.dat"
#endif
// -------------------------------------------------------------------------------

//GMO
#define PROTOCOL_MOVE			0xD4
#define PROTOCOL_POSITION		0x15
#define PROTOCOL_ATTACK			0x11
#define PROTOCOL_BEATTACK		0xDB//0xD7
// -------------------------------------------------------------------------------