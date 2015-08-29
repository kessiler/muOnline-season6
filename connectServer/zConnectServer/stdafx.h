#pragma once
// -----------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#define MAX_JS_RECVQ_LIMIT		150
#define MAX_SERVER_LIST			400
// -----------------------------------------------------------------------

#define APP_NAME		"zConnectServer"
#define	APP_VERSION_T	"1.0.0.0"
#define	APP_VERSION_N	1, 0, 0, 0
#define	APP_DES			"MU Online ConnectServer emulator"
// -----------------------------------------------------------------------

#include "targetver.h"
// -----------------------------------------------------------------------

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
// -----------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
// -----------------------------------------------------------------------

#include <windows.h>
// -----------------------------------------------------------------------

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <atlstr.h>
#include <time.h>
#include <vector>
#include <map>
// -----------------------------------------------------------------------

#pragma comment(lib, "ws2_32.lib")
// -----------------------------------------------------------------------

#include "resource.h"
// -----------------------------------------------------------------------