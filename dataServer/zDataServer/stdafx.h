#pragma once
// -----------------------------------------------------------------------

#define VMPROTECT
// -----------------------------------------------------------------------

//#define __MUANGEL__		1
#define __ALIEN__		2	
// -----------------------------------------------------------------------

#define FIX_MAXSTAT			1
// -----------------------------------------------------------------------

#define APP_NAME		"zDataServer"
#define	APP_VERSION_T	"2.0.0.0"
#define	APP_VERSION_N	2, 0, 0, 2
#define	APP_DES			"MU Online DataServer emulator"
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
#include <stdlib.h>
#include <SQL.h>
#include <sqlext.h>
// -----------------------------------------------------------------------

#pragma comment(lib, "ws2_32.lib")
// -----------------------------------------------------------------------

#include "VMProtectSDK.h"
#include "define.h"
#include "Settings.h"
#include "zDataServer.h"
#include "Window.h"
// -----------------------------------------------------------------------