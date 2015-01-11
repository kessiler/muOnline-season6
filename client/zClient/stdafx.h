#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#define __ROOT__
//#define VM_PROTECT
//#define __MUTEX__
//#define __LAUNCHER__

#if defined __MEGAMU__ || __VIRNET__
#define NEWWINGS
#endif

#include <Windows.h>
#include "targetver.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <stdarg.h>
#include <process.h>
#include "Defines.h"
#include "resource.h"
#ifdef VM_PROTECT
	#include "VMProtectSDK.h"
#endif


#define MAX_SUBTYPE_ITEMS 512
#define ITEMGET(x,y) ( (x)*MAX_SUBTYPE_ITEMS + (y))
