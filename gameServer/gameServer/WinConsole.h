// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#pragma once

#include "../Lua/include/lua.hpp"

#define WINCONSOLE_LOG_TEXT_LENGTH	80
#define WINCONSOLE_LOG_TEXT_LINE	20

#define WINCONSOLE_LINE_LENGTH		1024
#define WINCONSOLE_LINE_COUNT		10

class CWinConsole
{
public:

	void SaveDbgWnd(const char *pFilename);

	HMENU GetSubMenu(void);

	static void ClearDbgWnd(void);

	HWND GetHWND(void);
	bool IsWinActive(void);
	bool HasFocusEditCtrl(void);

	static bool EnableWinConsoleOption(DWORD dwWinConsoleOpt);
	DWORD GetWinConsoleOpt(void);
	void AddWinConsoleOption(DWORD dwWinConsoleOpt);
	void SubtractWinConsoleOption(DWORD dwWinConsoleOpt);

private:
	CWinConsole(void);
	virtual ~CWinConsole(void);

	void Init(HINSTANCE hInstance);

	void ResizeControls(void);
	void Paint(HDC hDC);

	static void ChangeFromStringToBackUp(BOOL bSetNext);
	static void ChangeFromBackUpToString(BOOL bGetNext);

public:
	static BOOL LogDateChange(void);

	static void CreateDbgFile(void);
	static void PrintDbgFile(char const * szBuf);

	static HWND StartConsole(HINSTANCE hInstance, lua_State* pScriptContext);
	static void StopConsole(void);

	static void Write(char const * pString, ...);

	static LRESULT __stdcall MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT __stdcall SubclassInputEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
	static FILE* g_DbgFp;
	static int m_cline;
	static BYTE LogTextViewType[WINCONSOLE_LOG_TEXT_LINE];
	static short LogTextLength[WINCONSOLE_LOG_TEXT_LINE];
	static char LogText[WINCONSOLE_LOG_TEXT_LINE][WINCONSOLE_LOG_TEXT_LENGTH];


	static char m_CommandBuffer[WINCONSOLE_LINE_LENGTH];
	static char m_pszBackUpString[WINCONSOLE_LINE_COUNT][WINCONSOLE_LINE_LENGTH];

	static int m_iBackUpLineNo;
	static int m_iCurrentBackUpLineNo;

	static int m_MaxTextWidth;
	static volatile bool m_bWinIsActive;

	static volatile HWND m_hWnd;
	static volatile HWND m_hListBox;
	static volatile HWND m_hEditControl;

    static int LogMYear;
    static int LogMonth;
    static int LogMDay;


private:
	HINSTANCE m_hInstance;			// +04
	HMENU m_hMainMenu;				// +08
	HMENU m_hSubMenu;				// +0C
	DWORD m_dwWinConsoleOpt;		// +10
	lua_State *m_pScriptContext;	// +14
	int m_ScrollyPos;				// +18
	int m_textAreaHeight;			// +1C
};

