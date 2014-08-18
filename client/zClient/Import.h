#pragma once
// ----------------------------------------------------------------------------------------------

#include "stdafx.h"
// ----------------------------------------------------------------------------------------------

#include <gl\GL.h>
#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "Ws2_32.lib")
// ----------------------------------------------------------------------------------------------

//-> Interface
#define pDrawInterface			((void(__cdecl*)()) 0x0080F790)
#define oDrawInterface_Call		0x0080F6AE
#define pDrawGUI				((void(__cdecl*)(DWORD, float, float, float, float)) 0x00790A00)
#define pDrawToolTip			((int(__cdecl*)(int X, int Y, LPCSTR Text)) 0x00597220)
#define pDrawButton				((void(__cdecl*)(DWORD, float, float, float, float, float, float)) 0x00790CF0)
#define pDrawColorButton		((void(__cdecl*)(DWORD, float, float, float, float, float, float, DWORD)) 0x00790DD0)
#define pDrawExtraColor			((void(__cdecl*)(DWORD, float, float, float, float, float, float, float, float, DWORD)) 0x637A60)
#define pDrawMapObject			((void(__cdecl*)(DWORD, float, float, float, float, float, float, float, float, float, float, float, float, DWORD)) 0x638400)
#define pDrawInfoBox			((char(__cdecl*)(char Text, int Arg2)) 0x007380B0)
#define pDrawMessage			((int(__cdecl*)(LPCSTR Text, int Mode)) 0x00597630)
#define pMakeColor				((DWORD(__cdecl*)(BYTE, BYTE, BYTE, BYTE)) 0x00412D20)
#define pTextThis				((LPVOID(*)()) 0x0041FE10)
#define pTextFormat				((int(__cdecl*)(char * Buffer, int Arg2, int LineSize, LPCTSTR Text, int LineWidth, bool Arg6, BYTE Arg7)) 0x540880)
#define pDrawColorText			((int(__cdecl*)(LPCTSTR Text, int PosX, int PosY, int Width, int Arg5, int Color, int Arg7, int Align)) 0x7D0380)
#define pDrawText				((int(__thiscall*)(LPVOID This, int PosX, int PosY, LPCTSTR Text, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)) 0x00420150)
#define pSetTextColor			((void(__thiscall*)(LPVOID This, BYTE r, BYTE g, BYTE b, BYTE h)) 0x00420040)
#define pSetBlend				((void(__cdecl*)(BYTE Mode)) 0x635FD0)
#define pLoadImage				((int(__cdecl*)(char * Folder, int Code, int Arg3, int Arg4, int Arg5, int Arg6)) 0x007721E0)
#define pLoadModel				((void(__cdecl*)(int ResourceID, char * Path, char * File, int Arg4)) 0x614D10)
#define pLoadTexture			((void(__cdecl*)(int TextureID, char * Folder, int GLREPEAT, int GLNEAREST, int GLTRUE)) 0x614710)
#define pInitModelData			((void(__cdecl*)()) 0x00617D30)
#define pInitModelData2			((void(__cdecl*)()) 0x00626190)
#define pInitTextureData		((void(__cdecl*)()) 0x0061AED0)
#define pLoadSomeForm			((void(__cdecl*)()) 0x00777440)
#define oLoadSomeForm_Call		0x00776D5E
#define pWindowThis				((LPVOID(*)()) 0x860FC0)
#define pCheckWindow			((bool(__thiscall*)(LPVOID This, int Code)) 0x0085EAD0)
#define pCloseWindow			((int(__thiscall*)(LPVOID This, int Code)) 0x0085F850)
#define pOpenWindow				((int(__thiscall*)(LPVOID This, int Code)) 0x0085EB00)
#define pOpenInfoBox			((bool(__cdecl*)(LPVOID This, int Arg2, float Arg3)) 0x7908C0)
#define	pCursorX				*(int*)0x879340C
#define	pCursorY				*(int*)0x8793410
#define pWinWidth				*(GLsizei*)0x0E61E58
#define pWinHeight				*(GLsizei*)0x0E61E5C
#define pWinWidthReal			*(float*)0xE7C3D4
#define pWinHeightReal			*(float*)0xE7C3D8
#define pWinFontHeight			*(int*)0x81C0380
#define pTextLineThis			((LPVOID(*)()) 0x8128ADC)
#define pGetTextLine			((LPCSTR(__thiscall*)(LPVOID This, int LineNumber)) 0x402320)
#define pDrawItemModel			((int(__cdecl*)(float PosX, float PosY, float Width, float Height, int ItemID, int a6, int a7, float a8, char a9)) 0x5CF310)
#define pSetCursorFocus			*(DWORD*)0xE8CB3C
#define pAllowGUI				((bool(__cdecl*)()) 0x7D4100)
#define oAllowGUI_Call1			0x007D363E
#define oAllowGUI_Call2			0x00811AF2
#define pDrawBarForm			((void(__cdecl*)(float PosX, float PosY, float Width, float Height, float Arg5, int Arg6)) 0x6378A0)
#define pGLSwitchBlend			((void(__cdecl*)()) 0x636070)
#define pGLSwitch				((void(__cdecl*)()) 0x635F50)
// ----------------------------------------------------------------------------------------------

//-> User
#define pGetPosFromAngle		((void(__cdecl*)(VAngle * Angle, int * PosX, int * PosY)) 0x635B00)
#define pPartyListStruct		*(PartyList*)0x81CB4E8
#define pGetPartyStruct(Slot)	((Slot * sizeof(PartyList)) + (*(char**)pPartyListStruct))
#define pPartyMemberCount		*(DWORD*)0x81F6B6C
#define pMapNumber				*(int*)0x0E61E18
#define pLastSkillIndex			*(DWORD*)0x81C039C
#define	pViewUnknownTargetID	*(int*)0xE61728		//item target
#define	pViewNPCTargetID		*(int*)0xE6172C
#define	pViewAttackTargetID		*(int*)0xE61730
#define pPreviewThis			((LPVOID(*)()) 0x402BC0)
#define pClearPreviewThis		((LPVOID(*)()) 0x82C5C80)
#define pClearPreview			((void(__thiscall*)(LPVOID This)) 0x4CC780)
#define pGetPreviewStruct		((DWORD(__thiscall*)(LPVOID This, int ViewportID)) 0x96A340)
#define pPreviewCharSet			((void(__cdecl*)(int ObjectIndex, BYTE * CharSet, lpViewObj Object, int Mode)) 0x57FD90)
#define oUserPreviewStruct		*(int*)0x7BC4F04
#define oUserObjectStruct		*(int*)0x8128AC8
#define oUserObjectStruct_		*(int*)0x8128AC4
#define pMasterLevel			*(short*)0x87935D8
#define pMasterPoint			*(short*)0x87935F2
#define pMasterExp				*(__int64*)0x87935E0
#define pMasterNextExp			*(__int64*)0x87935E8
#define pFrameValue1			*(BYTE*)(0x004D9F02 + 2)
#define pFrameValue2			*(DWORD*)(0x004DA3B2 + 1)
#define pFrameValue3			*(DWORD*)(0x004DA3DD + 3)
#define pFrameValue4			*(BYTE*)(0x004DA3AC + 3)
#define pPlayerState			*(int*)0x0E609E8
#define pFrameLimit				0//need check value inside struct
#define oBattleMapStart			0x0083150E
#define oBattleZoneStart		0x005D3043
#define pMaxBattleZoneCount		*(BYTE*)(0x005D3078 + 3)
#define oIsBattleServer_Call1	0x005D303A
#define oIsBattleServer_Call2	0x00831501
#define pIsMaster				((bool(__cdecl*)(BYTE Class)) 0x5878E0)
#define pIsMaster2				((bool(__cdecl*)(BYTE Class)) 0x5875A0)
#define pGetCharClass			((BYTE(__cdecl*)(BYTE Class)) 0x405230)
#define pInitEventStatus		((void(__cdecl*)(int PreviewStruct)) 0x4EFF70)
#define pEventEffectThis		((LPVOID(__cdecl*)(int Arg1)) 0x9CFF45)
#define pAddEventEffect			((LPVOID(__thiscall*)(LPVOID This, int ModelID, int a3, bool bFixRotation, float a5, float a6, float a7, float a8, float a9, float a10)) 0x4EFFF0)
#define pAddEventEffect2		((LPVOID(__thiscall*)(LPVOID This, int ModelID, int a3, int a4, float a5, float a6, float a7)) 0x4F0730)
#define pPShopThis1				((LPVOID(__cdecl*)()) 0x860FC0)
#define pPShopThis2				((LPVOID(__thiscall*)(LPVOID This)) 0x8612B0)
#define pPShopSet				((void(__thiscall*)(LPVOID This, BYTE Mode)) 0x840E20)
#define pPShopRefresh			((void(__cdecl*)(int Preview)) 0x00668460)
#define pMUHelperThis			((LPVOID(__cdecl*)()) 0x4DB240)
#define pMUHelperClass			*(int*)0xE8CB7C
#define pMUHelperClose			((void(__thiscall*)(LPVOID This)) 0x95D2D0)
#define pMUHelperStart			((void(__thiscall*)(LPVOID This)) 0x95D010)
#define pMUHelperDataSet		((bool(__thiscall*)(LPVOID This, LPBYTE Data)) 0x960A90)
#define pMUIsLoaded				*(DWORD*)0xE60974
//#define pWarehouseClass			((int(__cdecl*)()) 0x860FC0)
#define pWarehouseExThis		((int(__thiscall*)(LPVOID This)) 0x861930)
#define pWarehouseExSet			((void(__thiscall*)(int Pointer, BYTE Status)) 0x856A80)
#define pGameShopThis			((LPVOID(*)()) 0x93F220)
#define pGameShopGetGP			((double(__thiscall*)(LPVOID This)) 0x940430)
#define pUserStat				((LPVOID(*)()) 0x588D70)
#define pUpdateUserStat			((void(__thiscall*)(LPVOID This, int * Value, int Code)) 0x4EA460)
// ----------------------------------------------------------------------------------------------

//-> Other
#define oCam_Zoom				0x0D27BFC	//1037DB0
#define oCam_PosZ				0x0D255AC	//1031A64
#define oCam_RotZ				0x87933D8	//8B4ACC8
#define oCam_RotZDef			0x0D27B78	//1033AB0
#define oCam_RotY				0x0D27B88	//1033AC0
#define oCam_ClipX				0x0D2C848	//1038D28
#define oCam_ClipY				0x0D2C894	//1033A78
#define oCam_ClipZ				0x0D2C8B8	//1038D78
#define oCam_ClipXCS			0x0D2C898	//1038D58
#define oCam_ClipYCS			0x0D2C8A8	//1038D68
#define oCam_ClipX2				0x0D2C878	//1038D38
#define oCam_ClipY2				0x0D2C880	//1038D40
#define oCam_ClipZ2				0x0D2B94C	//1037DA4
#define oCam_ClipGL				0x0D2570C	//1031BCC
#define oCharColor_Loot			(0x0064B6AB + 1)
#define oCharColor_Exp			(0x0064C9A0 + 1)
#define pGetMoneyFormat			((int(__cdecl*)(double Value, char * Buff, int Arg3)) 0x5C1170)
#define pPlayEffectAnimation	((int(__cdecl*)(int EffectIndex, int PreviewStruct)) 0x666FE0)
#define oVulcanusMove			(0x00831A5C + 2)
#define oShowChatMessage_Call	0x0078AF6C
#define pShowChatMessage		((bool(__thiscall*)(LPVOID This)) 0x00788930)
#define pSetChatMessageType		((int(__thiscall*)(LPVOID This, int Type)) 0x0078B720)
#define pChatThis				((LPVOID(__thiscall*)(LPVOID This)) 0x8610D0)
#define pChatIsClosed			((bool(__thiscall*)(LPVOID This)) 0x785FC0)
#define pChatCheckLevel			((bool(__cdecl*)(int MinLevel, char * Name)) 0x597E10)
#define pChatWhisperThis		((LPVOID(__thiscall*)(LPVOID This)) 0x8614D0)
#define pChatWhisperActive		((bool(__thiscall*)(LPVOID This)) 0x849CB0)
#define pChatReserveLine		((DWORD(__cdecl*)(DWORD Arg1, DWORD Arg2, DWORD Arg3)) 0x6D6AD0)
#define pChatBoxThis			((DWORD(__thiscall*)(LPVOID This)) 0x861030)
#define pChatOutput				((void(__thiscall*)(DWORD This, char * Name, char * Message, DWORD Type, DWORD Arg4)) 0x789390)
#define pGetCommandLine			((LPSTR(WINAPI*)()) 0x00D221C8)
#define pFrameSpeed1			*(double*)0x0D27C88
#define	pFrameSpeed2			*(double*)0x0D281C0
#define pIsDisconnect			*(int*)0xE82C24
#define pCreateConnect			((int(__cdecl*)(char * IP, WORD Port)) 0x0063C960)
#define pOnLine					*(DWORD*)0x8793704
#define pMenuExitEvent			((DWORD(__cdecl*)(int Arg1)) 0x7A8880)
#define pGameWindow				*(HWND*)0x0E8C578
#define pGetMapName				((char*(__cdecl*)(int MapNumber)) 0x5D2C10)
#define pMUCommandLine			(char*)0x0E8C274
#define pGameLoad				((int(__cdecl*)()) 0x004D0FC0)
#define pGameResolutionMode		*(int*)0x0E8C240
#define pMoveListInit			((void(__thiscall*)(LPVOID This, int Arg1, int Arg2)) 0x830AC0)
// ----------------------------------------------------------------------------------------------

//-> Item
#define pGetItemColor			((void(__cdecl*)(int ModelID, float Arg2, float Arg3, VAngle * Color, bool BalrogMode)) 0x5F8C50)
#define pGetItemColor2			((void(__cdecl*)(int ModelID, float Arg2, float Arg3, VAngle * Color, bool BalrogMode)) 0x5FA4A0)
#define pSetItemPrice           ((__int64(__cdecl*)(ObjectItem * lpItem, int Mode)) 0x58E3B0)
#define pIsExpensiveItem		((bool(__cdecl*)(ObjectItem * lpItem)) 0x5C81F0)
#define pIsUpgradeItem			((bool(__thiscall*)(LPVOID This, ObjectItem * lpSource, ObjectItem * lpTarget)) 0x7DD970)
#define pAllowRepairWings		((bool(__cdecl*)(ObjectItem * lpItem)) 0x5C9DA0)
#define pSetItemOption			((void(__cdecl*)(ObjectItem * lpItem, BYTE Option, BYTE Special, BYTE Value)) 0x58B910)
#define pSetItemOptionText		((void(__cdecl*)(int ItemID, int Line, WORD OptionID, bool Arg4, int Arg5)) 0x5C2F70)
#define pSetItemTextLine		((void(__cdecl*)(int Line, LPSTR Text, ...)) 0x9CF52A)
#define pIsWings				((int(__thiscall*)(LPVOID This, DWORD ItemID)) 0x7E7720)
#define pSetItemEffect			((void(__cdecl*)(int Arg1, int ModelID, VAngle Angle, int Arg4, int ItemLevel, char Arg6, int Arg7, int Arg8, int Arg9)) 0x609E70)
#define pSetItemEffect2			((void(__cdecl*)(float a4, float a5, float a6, lpViewObj Object, float a8, DWORD ModelID, int a10, char a11, char a12, char a13, signed int a14, char a15)) 0x5655C0)
#define pGetMaxDurability		((short(__cdecl*)(ObjectItem * lpItem, int ScriptStruct, int Level)) 0x5C2170)
#define pIsFlyingMauntToMove	((bool(__thiscall*)(LPVOID This, WORD ItemID1, WORD ItemID2)) 0x8319B9)
#define pAllowStaticEffect		((void(__thiscall*)(LPVOID This, DWORD * Arg1, DWORD Arg2, int Arg3, int Arg4)) 0x544E60)
#define pPlayStaticEffect		((int(__cdecl*)(DWORD ModelID, DWORD * Arg1, float Scale, float * Color, int Model, float a6, int a7)) 0x7711C0)
#define pPlayDynamicEffect		((int(__cdecl*)(DWORD ModelID, DWORD * Arg1, int Arg2, float * Color, int Arg4, float Arg5, int Arg6)) 0x74CBE0)
// ----------------------------------------------------------------------------------------------

//-> Protocol
#define oXORFilterStart			0x00404C99
#define pActiveSocket			*(SOCKET*)(*(DWORD*)0x0E61F84 + 0x0c)
#define oDataRecv_Call			0x0065FD79				
#define pDataRecv				((void(*)(DWORD Type, LPBYTE Data, int Len, int aIndex)) 0x006639D0)
#define oWhisperChat_Call		0x00663F9E
// ----------------------------------------------------------------------------------------------

//-> Monster
#define oNpcName				0x8119F68
#define pSendChatSay			((int(__cdecl*)(char * Name, char * Text, lpViewObj lpObject, int a4, int a5)) 0x5996A0)
#define pShowMonster			((double(__cdecl*)(int MonsterID, int a2, int a3, int a4)) 0x580FC0)
// ----------------------------------------------------------------------------------------------