#include "stdafx.h"
#include "ViewportDraw.h"
// ----------------------------------------------------------------------------------------------

#define pViewObjStruct			*(int*)0x7B650F8
#define pGetObjectHeadPos		((void(__cdecl*)(MU3Float * Arg1, int * PosX, int * PosY)) 0x612930)
#define pSetTextColor			((void(__thiscall*)(LPVOID This, BYTE r, BYTE g, BYTE b, BYTE h)) 0x420BD4)
#define pDrawText				((int(__thiscall*)(LPVOID This, int PosX, int PosY, LPCTSTR Text, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)) 0x420E93)
#define pTextThis				((LPVOID(*)()) 0x0041FE10)

void TestDraw()
{
	for( int ViewportID = 0; ViewportID < 400; ViewportID++ )	//Viewobject max
	{
		Preview Object = *(Preview*)((char*)&pViewObjStruct + 1304 * ViewportID);
		// ----
		if( !Object.Visible )	//Need test
		{
			continue;
		}
		// ----
		char Display[20];
		int PosX, PosY;
		MU3Float Position;
		Position.x = Object.Unknown952;
		Position.y = Object.Unknown956;
		Position.z = Object.Unknown960 + Object.SelectZone + 100.0f;
		// ----
		pGetObjectHeadPos(&Position, &PosX, &PosY);
		sprintf(Display, "Viewport");
		pSetTextColor(pTextThis(), 0xFF, 0xE6, 0xD2, 0xFF);
		pDrawText(pTextThis(), PosX, PosY - 6, Display, 0, 0, (LPINT)1, 0);
	}
}