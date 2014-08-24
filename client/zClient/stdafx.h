#pragma once
// ----------------------------------------------------------------------------------------------

//#define VM_PROTECT
// ----------------------------------------------------------------------------------------------

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
//#define __VIRNET__		19
// ----------------------------------------------------------------------------------------------

#if defined __MEGAMU__ || __VIRNET__
#define NEWWINGS
#endif
// ----------------------------------------------------------------------------------------------

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
// ----------------------------------------------------------------------------------------------
#ifdef VM_PROTECT
	#include "VMProtectSDK.h"
#endif
#include "Defines.h"
#include "resource.h"
// ----------------------------------------------------------------------------------------------


#define MAX_SUBTYPE_ITEMS 512
#define ITEMGET(x,y) ( (x)*MAX_SUBTYPE_ITEMS + (y))
#define _CRT_SECURE_NO_WARNINGS 1