#pragma once
// ----------------------------------------------------------------------------------------------

#include "Import.h"
// ----------------------------------------------------------------------------------------------

#define INTERFACE_DEBUG		1
// ----------------------------------------------------------------------------------------------

#define MAX_OBJECT			100
#define MAX_WIN_WIDTH		640
#define MAX_WIN_HEIGHT		480
// ----------------------------------------------------------------------------------------------

enum ObjectID
{
	eSAMPLEBUTTON = 1,
	eRANK_MAIN,
	eRANK_TITLE,
	eRANK_FRAME,
	eRANK_FOOTER,
	eRANK_DIV,
	eRANK_BUTTON,
	eRANK_TAB1,
	eRANK_TAB2,
	eRANK_TAB3,
	eRANK_TAB4,
	eCAMERA_MAIN,
	eCAMERA_BUTTON1,
	eCAMERA_BUTTON2,
	eCRAFT_MAIN,
	eCRAFT_TITLE,
	eCRAFT_FRAME,
	eCRAFT_FOOTER,
	eCRAFT_DIV,
	eCRAFT_TAB1,
	eCRAFT_TAB2,
	eCRAFT_TAB3,
	eCRAFT_TAB4,
	eCRAFT_CLOSE,
	eCRAFT_INFOBG,
	eCRAFT_MONEYBG,
	eCRAFT_FINISH,
	eRESET_MAIN,
	eRESET_TITLE,
	eRESET_FRAME,
	eRESET_FOOTER,
	eRESET_DIV,
	eRESET_INFOBG,
	eRESET_MONEYBG,
	eRESET_CLOSE,
	eRESET_FINISH,
	eOFFEXP_SWITCH,
	eNEWS_MAIN,
	eNEWS_TITLE,
	eNEWS_FRAME,
	eNEWS_FOOTER,
	eNEWS_DIV,
	eNEWS_INFOBG,
	eNEWS_CLOSE,
	eNEWS_BACK,
	eTIME,
	eQUEST_MAIN,
	eQUEST_TITLE,
	eQUEST_FRAME,
	eQUEST_FOOTER,
	eQUEST_DIV,
	eQUEST_CLOSE,
};
// ----------------------------------------------------------------------------------------------

enum ColorMacro
{
	eShinyGreen		= Color4f(172, 255, 56, 255),
	eGold			= Color4f(255, 189, 25, 255),
	eWhite			= Color4f(255, 255, 255, 255),
	eWhite180		= Color4f(255, 255, 255, 180),
	eOrange			= Color4f(255, 105, 25, 255),
	eGray100		= Color4f(50, 50, 50, 100),
	eGray150		= Color4f(50, 50, 50, 150),
	eBlowPink		= Color4f(220, 20, 60, 255),
	eRed			= Color4f(225, 0, 0, 255),
	eExcellent		= Color4f(0, 225, 139, 255),
	eAncient		= Color4f(1, 223, 119, 255),
	eSocket			= Color4f(153, 102, 204, 255),
	eBlue			= Color4f(36, 242, 252, 255),
	eYellow			= Color4f(255, 255, 0, 255),
	eBrown			= Color4f(69, 39, 18, 255),
};
// ----------------------------------------------------------------------------------------------

struct InterfaceObject
{
	DWORD	ModelID;
	float	Width;
	float	Height;
	float	X;
	float	Y;
	float	MaxX;
	float	MaxY;
	DWORD	EventTick;
	bool	OnClick;
	bool	OnShow;
	BYTE	Attribute;
};
// ----------------------------------------------------------------------------------------------

class Interface
{
public:
				Interface();
	// ----
	void		Load();
	static void	Work();
	static void	LoadImages();
	static void LoadModels();
	void		BindObject(short ObjectID, DWORD ModelID, float Width, float Height, float X, float Y);
	// ----
	void		DrawLifeBar();
	void		DrawSampleButton();
	void		EventSampleButton(DWORD Event);
	void		DrawCameraUI();
	void		EventCameraUI(DWORD Event);
	void		DrawOffExpSwitch();
	void		EventOffExpSwitch(DWORD Event);
	// ----
	void		OpenCraftWindow() { this->Data[eCRAFT_MAIN].OnShow = true; pSetCursorFocus = true; };
	void		CloseCraftWindow() { this->Data[eCRAFT_MAIN].OnShow = false; pSetCursorFocus = false; };
	bool		CheckCraftWindow() { return this->Data[eCRAFT_MAIN].OnShow; };
	void		DrawCraftWindow();
	void		FillCraftTab1();
	void		FillCraftTab2();
	void		FillCraftTab3();
	void		FillCraftTab4();
	bool		EventCraftWindow_Main(DWORD Event);
	bool		EventCraftWindow_TabControl(DWORD Event);
	bool		EventCraftWindow_Close(DWORD Event);
	bool		EventCraftWindow_Finish(DWORD Event);
	// ----
	void		OpenResetWindow() { this->Data[eRESET_MAIN].OnShow = true; pSetCursorFocus = true; };
	void		CloseResetWindow() { this->Data[eRESET_MAIN].OnShow = false; pSetCursorFocus = false; };
	bool		CheckResetWindow() { return this->Data[eRESET_MAIN].OnShow; };
	void		DrawResetWindow();
	bool		EventResetWindow_Main(DWORD Event);
	bool		EventResetWindow_Close(DWORD Event);
	bool		EventResetWindow_Finish(DWORD Event);
	// ----
	void		OpenNewsWindow() { this->Data[eNEWS_MAIN].OnShow = true; pSetCursorFocus = true; };
	void		CloseNewsWindow() { this->Data[eNEWS_MAIN].OnShow = false; pSetCursorFocus = false; };
	bool		CheckNewsWindow() { return this->Data[eNEWS_MAIN].OnShow; };
	void		DrawNewsWindow();
	bool		EventNewsWindow_Main(DWORD Event);
	bool		EventNewsWindow_Close(DWORD Event);
	bool		EventNewsWindow_Back(DWORD Event);
	// ----
	void		OpenQuestDialog() { this->Data[eQUEST_MAIN].OnShow = true; pSetCursorFocus = true; };
	void		CloseQuestDialog() { this->Data[eQUEST_MAIN].OnShow = false; pSetCursorFocus = false; };
	bool		CheckQuestDialog() { return this->Data[eQUEST_MAIN].OnShow; };
	void		DrawQuestDialog();
	void		FillQuestDialog1();
	// ----
	void		SwitchTime() { this->Data[eTIME].OnShow = !this->Data[eTIME].OnShow; };
	void		DrawTime();
	// ----
	bool		CheckWindow(int WindowID);
	int			CloseWindow(int WindowID);
	int			OpenWindow(int WindowID);
	static bool	AllowGUI();
	// ----
	void		SetTextColor(BYTE Red, BYTE Greed, BYTE Blue, BYTE Opacity);
	void		DrawText(int X, int Y, LPCTSTR Text);
	int			DrawFormat(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...);
	void		DrawFormatEx(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...);
	void		DrawGUI(short ObjectID, float X, float Y);
	float		DrawRepeatGUI(short ObjectID, float X, float Y, int Count);
	void		DrawColoredGUI(short ObjectID, float X, float Y, DWORD Color);
	int			DrawToolTip(int X, int Y, LPCSTR Text, ...);
	int			DrawToolTipEx(int X, int Y, LPCSTR Text, ...);
	int			DrawMessage(int Mode, LPCSTR Text, ...);
	// ----
	bool		IsWorkZone(short ObjectID);
	bool		IsWorkZone(float X, float Y, float MaxX, float MaxY);
	float		GetResizeX(short ObjectID);
	// ----
private:
	InterfaceObject Data[MAX_OBJECT];
	// ----
}; extern Interface gInterface;
// ----------------------------------------------------------------------------------------------