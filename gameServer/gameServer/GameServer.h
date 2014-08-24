#ifndef	GAMESERVER_H
#define	GAMESERVER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RingAttackEvent.h"
#include "XMasAttackEvent.h"

#include "CWhatsUpDummyServer.h"

#define WM_GM_SERVER_MSG_PROC			0x401
#define WM_GM_CLIENT_MSG_PROC			0x402
#define WM_GM_JOIN_CLIENT_MSG_PROC		0x403
#define WM_START_SERVER					0x405
#define WM_GM_DATA1_CLIENT_MSG_PROC		0x40A
#define WM_GM_DATA2_CLIENT_MSG_PROC		0x40B
#define WM_GM_RANKING_CLIENT_MSG_PROC	0x40C
#define WM_GM_EVENTCHIP_CLIENT_MSG_PROC	0x40D
#define WM_GM_EXDATA_CLIENT_MSG_PROC	0x40E
#define WM_GM_CONNECT_SERVER_MSG		0x40F

// TIMER Messages
#define	WM_LOG_PAINT					0x64
#define WM_FIRST_MSG_PROCESS			0x65
#define WM_AI_MONSTER_PROC				0x1F4
#define WM_SECOND_MSG_PROCESS			0x3E8
#define WM_SET_DATE						0x3E9
#define WM_GS_CLOSE						0x3EA
#define WM_LOG_DATE_CHANGE				0x3EB
#define WM_EVENT_RUN_PROC				0x3EC
#define WM_MOVE_MONSTER_PROC			0x3ED
#define WM_MONSTER_AND_MSG_PROC			0x3EE
#define WM_CONNECT_DATASERVER			0x3EF
#define WM_GUILD_LIST_REQUEST			0x3F0
#define WM_AI_MONSTER_MOVE_PROC			0x3F2
#ifdef __ALIEN__
#define WM_CONNECTMEMBERUPDATE			0x3F3
#endif

extern char ExDbIp[256];
extern char DataServerIp2[256];
extern char DataServerIp[256];
extern char JoinServerIp[256];
extern int gServerReady;
extern int gCloseMsg;
extern int gCloseMsgTime;
extern int gCurPaintPlayer;
extern int gCurPaintType;
extern int gCurPaintMapNumber;
extern CRingMonsterHerd g_MonsterHerdTest;

extern CRingAttackEvent g_RingAttackEvent;

extern CXMasAttackEvent g_XMasAttackEvent;

extern CWhatsUpDummyServer gWhatsUpDummyServer;

extern HINSTANCE hInst; // Current Instance
extern HWND ghWnd;

extern char szTitle[100];
extern char szWindowClass[64];

#define RANKING_SERVER_PORT 44455
#define EVENT_CHIP_SERVER_PORT 44456
#define MAX_LENGTH_SN 100

BOOL AllServerStart(void); // Start all Servers

#endif