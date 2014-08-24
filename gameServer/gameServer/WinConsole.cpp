// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "WinConsole.h"

#include "logproc.h"

// menu items
#define IDM_WINCONS_EXIT	3000
#define	IDM_WINCONS_SAVE	3001
#define IDM_WINCONS_CLEAR	3002
#define IDM_WINCONS_DEBUG	3004
#define IDM_WINCONS_TOPMOST	3005


CWinConsole* g_Console = NULL;

CRITICAL_SECTION WinConCritical;
WNDPROC lpfnInputEdit;

FILE* CWinConsole::g_DbgFp = NULL;
int CWinConsole::m_cline = 0;
BYTE CWinConsole::LogTextViewType[WINCONSOLE_LOG_TEXT_LINE] = {0};
short CWinConsole::LogTextLength[WINCONSOLE_LOG_TEXT_LINE] = {0};
char CWinConsole::LogText[WINCONSOLE_LOG_TEXT_LINE][WINCONSOLE_LOG_TEXT_LENGTH]={0};

char CWinConsole::m_CommandBuffer[WINCONSOLE_LINE_LENGTH]={0};
char CWinConsole::m_pszBackUpString[WINCONSOLE_LINE_COUNT][WINCONSOLE_LINE_LENGTH]={0};
int CWinConsole::m_iBackUpLineNo = 0;
int CWinConsole::m_iCurrentBackUpLineNo = 0;

int CWinConsole::m_MaxTextWidth = 0;
volatile bool CWinConsole::m_bWinIsActive = false;

volatile HWND CWinConsole::m_hWnd = NULL;
volatile HWND CWinConsole::m_hListBox = NULL;
volatile HWND CWinConsole::m_hEditControl = NULL;

int CWinConsole::LogMYear = 0;
int CWinConsole::LogMonth = 0;
int CWinConsole::LogMDay = 0;


int Debug_Print(lua_State *L);
int Clear_DebugWindow(lua_State *L);
int Save_DebugWindowContents(lua_State *L);


void CWinConsole::SaveDbgWnd(const char *pFilename)
{
	if ( !pFilename )
	{
		CWinConsole::Write("SaveDbgWnd() Error!! - file name is invlid!!");
		return;
	}

	FILE *fp = fopen(pFilename, "at");
	if ( !fp )
	{
		CWinConsole::Write("SaveDbgWnd() Error!! - file open failure!");
		return;
	}

	int nItemCnt = SendMessage(CWinConsole::m_hListBox, LB_GETCOUNT, 0, 0);
	if ( nItemCnt != -1 && nItemCnt <= 0 )
	{
		CWinConsole::Write("SaveDbgWnd() Error!! - item is invalid!");
		fclose(fp);

		return;
	}

	SYSTEMTIME retSystemTime;
	GetLocalTime(&retSystemTime);
	fprintf(fp,
		"------------- MULOG Save [Start]  [%04d/%02d/%02d %02d:%02d:%02d] -------------\n",
		retSystemTime.wYear,
		retSystemTime.wMonth,
		retSystemTime.wDay,
		retSystemTime.wHour,
		retSystemTime.wMinute,
		retSystemTime.wSecond);

	for (int i = 0; i < nItemCnt; ++i )
	{
		char szGetString[1024]={0};

		if ( SendMessage(CWinConsole::m_hListBox, LB_GETTEXT, i, (LPARAM)szGetString) == -1 )
			break;

		fputs(szGetString, fp);
		fputs("\n", fp);
	}

	fprintf(fp, "---------------------- MULOG Save [Completed] -----------------------\n\n");

	fclose(fp);
}

BOOL CWinConsole::LogDateChange(void)
{
	time_t ltime;
	tm* today;
	char szTemp[250];

	time(&ltime);
	today = localtime(&ltime);

	today->tm_year += 1900;
	++today->tm_mon;

	if ( today->tm_year <= CWinConsole::LogMYear
		&& today->tm_mon <= CWinConsole::LogMonth
		&& today->tm_mday <= CWinConsole::LogMDay )
	{
		return FALSE;
	}

    CWinConsole::LogMYear = today->tm_year;
    CWinConsole::LogMonth = today->tm_mon;
    CWinConsole::LogMDay = today->tm_mday;

    EnterCriticalSection(&WinConCritical);

    memset(szTemp, 0, 250);

    wsprintf(szTemp, "log/%02d%02d%02d_LuaDbg.log",
      CWinConsole::LogMYear, CWinConsole::LogMonth, CWinConsole::LogMDay);

    if ( CWinConsole::g_DbgFp )
    {
		fclose(CWinConsole::g_DbgFp);
		CWinConsole::g_DbgFp = NULL;
    }

    CWinConsole::g_DbgFp = fopen(szTemp, "a+tc");
    LeaveCriticalSection(&WinConCritical);
  
	return TRUE;
}


void CWinConsole::CreateDbgFile(void)
{
	time_t ltime;
	tm* today;
	char szTemp[250]={0};

	time(&ltime);
	today = localtime(&ltime);

	today->tm_year += 1900;

    CWinConsole::LogMYear = today->tm_year;
    CWinConsole::LogMonth = today->tm_mon+1;
    CWinConsole::LogMDay = today->tm_mday;

	CreateDirectory("./LUA_DEBUG_LOG", 0);

	wsprintf(szTemp, "LUA_DEBUG_LOG/%02d%02d%02d_LuaDbg.txt",
		CWinConsole::LogMYear, CWinConsole::LogMonth, CWinConsole::LogMDay);

	if ( CWinConsole::g_DbgFp )
	{
		fclose(CWinConsole::g_DbgFp);
		CWinConsole::g_DbgFp = NULL;
	}

	CWinConsole::g_DbgFp = fopen(szTemp, "a+tc");

	if ( !CWinConsole::g_DbgFp )
		LogAdd("LuaDbgLog file create error");

}


void CWinConsole::PrintDbgFile(char const *szBuf)
{
	// nothing found
}


void Debug_AddDebugPrint(lua_State *L)
{
	luaL_reg DebugGlue[] =
	{
		{"DebugPrint", Debug_Print},
		{NULL, NULL}
	};


	for (int i = 0; DebugGlue[i].name; ++i )
	{
		lua_pushcclosure(L, DebugGlue[i].func, 0);
		lua_setfield(L, LUA_GLOBALSINDEX, DebugGlue[i].name);
	}
}

static int Debug_Print(lua_State *L)
{
#ifdef _DEBUG
	// here must be some code
	// no code in executable, since it was build without _DEBUG
#endif

	return 0;
}


HWND CWinConsole::StartConsole(HINSTANCE hInstance, lua_State* pScriptContext)
{

	// need to add exception handling

	InitializeCriticalSection(&WinConCritical);
	if ( !g_Console )
	{
		g_Console = new CWinConsole();
		CWinConsole::CreateDbgFile();
	}

	if ( !CWinConsole::m_hWnd )
		g_Console->Init(hInstance);

	if ( pScriptContext )
	{
		g_Console->m_pScriptContext = pScriptContext;
		Debug_AddDebugPrint(pScriptContext);

		luaL_Reg DebugGlue[] =
		{
			{"cleardbgwnd",	Clear_DebugWindow},
			{"savedbgwnd",	Save_DebugWindowContents},
			{NULL, NULL}
		};

		for (int i = 0; DebugGlue[i].name; ++i )
		{
			lua_pushcclosure(pScriptContext, DebugGlue[i].func, 0);
			lua_setfield(pScriptContext, LUA_GLOBALSINDEX, DebugGlue[i].name);
		}
	}

  return CWinConsole::m_hWnd;
}

static int Clear_DebugWindow(lua_State *L)
{
	CWinConsole::ClearDbgWnd();

	return 0;
}

static int Save_DebugWindowContents(lua_State *L)
{
	g_Console->SaveDbgWnd( luaL_checklstring(L, 1, 0) );

	return 0;
}


void CWinConsole::StopConsole(void)
{
	if ( g_Console )
	{
		delete g_Console;
		g_Console = NULL;
	}

	if ( CWinConsole::m_hWnd )
		DestroyWindow(CWinConsole::m_hWnd);

	if ( CWinConsole::g_DbgFp )
	{
		fclose(CWinConsole::g_DbgFp);
		CWinConsole::g_DbgFp = NULL;
	}

	DeleteCriticalSection(&WinConCritical);

}

CWinConsole::CWinConsole(void)
{
	CWinConsole::m_hWnd = NULL;
	m_pScriptContext = NULL;

	memset(CWinConsole::m_CommandBuffer, 0, sizeof(CWinConsole::m_CommandBuffer));
	memset(CWinConsole::m_pszBackUpString, 0, sizeof(CWinConsole::m_pszBackUpString));

	CWinConsole::m_iBackUpLineNo = 0;
	CWinConsole::m_iCurrentBackUpLineNo = 0;

	m_dwWinConsoleOpt = 1;
	m_hMainMenu = 0;
	m_hSubMenu = 0;

	CWinConsole::m_cline = 0;
}


CWinConsole::~CWinConsole(void)
{
	if ( CWinConsole::m_hWnd )
		DestroyWindow(CWinConsole::m_hWnd);

	if ( CWinConsole::g_DbgFp )
	{
		fclose(CWinConsole::g_DbgFp);
		CWinConsole::g_DbgFp = NULL;
	}

	DeleteCriticalSection(&WinConCritical);
}


void CWinConsole::ResizeControls()
{

	RECT Rect;

	GetClientRect(CWinConsole::m_hWnd, &Rect);

	SetWindowPos(CWinConsole::m_hEditControl, NULL,
		Rect.left + 2, Rect.bottom - 18,
		Rect.right - Rect.left - 4,  16,
		SWP_NOZORDER);

	SetWindowPos(CWinConsole::m_hListBox, NULL,
		Rect.left + 2, Rect.top + 2,
		Rect.right - Rect.left - 4, Rect.bottom - Rect.top - 24,
		SWP_NOZORDER);

	InvalidateRect(CWinConsole::m_hWnd, NULL, TRUE);
}

void CWinConsole::Paint(HDC hDC)
{

}

LRESULT __stdcall CWinConsole::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch(uMsg)
	{
		case WM_ACTIVATEAPP: 
			CWinConsole::m_bWinIsActive = (wParam != FALSE);
		return 0;
    
		case WM_ACTIVATE:
			CWinConsole::m_bWinIsActive = ( wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE);
		return 0;

		case WM_DESTROY:
			CWinConsole::m_bWinIsActive = false;
			CWinConsole::m_hWnd = NULL;
		break;

		case WM_CLOSE:
        return 0;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			g_Console->Paint(hdc);
			EndPaint(hWnd, &ps);
        break;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDM_WINCONS_EXIT:
					if ( MessageBox(hWnd, "Do you want to quit?", "MU Console Window", 
							MB_YESNO | MB_ICONQUESTION) == IDYES )
					{
						StopConsole();
						return 0;
					}
				break;

				case IDM_WINCONS_SAVE:
				{
					char szFullPath[1024] = {0};
					char szFileName[1024] = {0};	// not used
					OPENFILENAME ofn;
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = NULL;
					ofn.hInstance = NULL;
					ofn.lpstrFilter = "MU LOG Files (*.txt)";
					ofn.lpstrDefExt = "txt";
					ofn.lpstrFile = szFullPath;
					ofn.nMaxFile = sizeof(szFullPath);	// in asm value 1280 is used

					if ( GetSaveFileName(&ofn) != 0 )
						g_Console->SaveDbgWnd(szFullPath);
				}
				break;

				case IDM_WINCONS_CLEAR:
					CWinConsole::ClearDbgWnd();
				break;

				case IDM_WINCONS_DEBUG:
					if ( CWinConsole::EnableWinConsoleOption(2) )
					{
						CheckMenuItem(g_Console->GetSubMenu(), IDM_WINCONS_DEBUG, MF_UNCHECKED);
						g_Console->SubtractWinConsoleOption(2);
					}
					else
					{
						CheckMenuItem(g_Console->GetSubMenu(), IDM_WINCONS_DEBUG, MF_CHECKED);
						g_Console->AddWinConsoleOption(2);
					}
				break;

				case IDM_WINCONS_TOPMOST:
					if ( CWinConsole::EnableWinConsoleOption(4) )
					{
						CheckMenuItem(g_Console->GetSubMenu(), IDM_WINCONS_TOPMOST, MF_UNCHECKED);
						g_Console->SubtractWinConsoleOption(2); // bug? maybe 4

						SetWindowPos(CWinConsole::m_hWnd, (HWND)HWND_NOTOPMOST, 
								0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
					}
					else
					{
						CheckMenuItem(g_Console->GetSubMenu(), IDM_WINCONS_TOPMOST, MF_CHECKED);
						g_Console->AddWinConsoleOption(4);

						SetWindowPos(CWinConsole::m_hWnd, (HWND)HWND_TOPMOST, 
								0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
					}
				break;

			} //  switch(LOWORD(wParam))

		}
		break; // case WM_COMMAND:


		case WM_SIZING:
		case WM_SIZE:
			g_Console->ResizeControls();
		break;

		case WM_SETCURSOR:
			SetCursor(LoadCursor(NULL, (LPCSTR)IDC_ARROW));
			ShowCursor(TRUE);
		break;

		case 1028:	// 0x404
			CWinConsole::Write(CWinConsole::m_CommandBuffer);

			if ( g_Console->m_pScriptContext )
			{
				if ( luaL_loadbuffer(g_Console->m_pScriptContext, CWinConsole::m_CommandBuffer, strlen(CWinConsole::m_CommandBuffer), NULL) )
					CWinConsole::Write("Error loading Command\n");

				if ( lua_pcall(g_Console->m_pScriptContext, 0, -1, 0) )
				{
					CWinConsole::Write("Error in Command\n");
					CWinConsole::Write(luaL_checklstring(g_Console->m_pScriptContext, -1, NULL));
				}
			}

			ZeroMemory(CWinConsole::m_CommandBuffer, sizeof(CWinConsole::m_CommandBuffer));
		break;

	} // switch(uMsg)


	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


HMENU CWinConsole::GetSubMenu(void)
{
	return m_hSubMenu;
}

void CWinConsole::Write(char const * pString, ...)
{
	va_list vl;

	va_start(vl, pString);

	int iTimeOut = 10;

	EnterCriticalSection(&WinConCritical);

	if ( pString && g_Console && CWinConsole::m_hWnd )
	{
		if( !CWinConsole::EnableWinConsoleOption(1) )
			return; // trick or dead-lock provoking?

		char szBuffer[1024]={0};

		Sleep(1);

		vsprintf(szBuffer, pString, vl);
		va_end(vl);

		char buf[1024]={0};


		int indx = 0;
		int tempMaxTextWidth = 0;

		// strange
		if ( strlen(szBuffer) > WINCONSOLE_LINE_LENGTH )
		{
			//TRACE_LOG("CWinConsole::Write - OverFlow");

			return; // trick or dead-lock provoking?
		}

		for (int i = 0; i < strlen(szBuffer); ++i )
		{
			Sleep(1);

			if ( szBuffer[i] != '\n' )
			{
				buf[indx++] = szBuffer[i];
				continue;
			}


			buf[indx] = 0;
			DWORD lpdwResult = 0;
			LRESULT lResult = SendMessageTimeout(CWinConsole::m_hListBox, LB_ADDSTRING,
						0, (LPARAM)buf, SMTO_NORMAL, iTimeOut, &lpdwResult);

			if ( CWinConsole::EnableWinConsoleOption(2) )
			{
				OutputDebugString(buf);
				OutputDebugString("\n");
			}

			if ( lResult == -2 || lResult == -1 )
				return; // trick or dead-lock provoking?

			DWORD dwLine = lpdwResult;
			SendMessageTimeout(CWinConsole::m_hListBox, LB_SETCURSEL, 
				dwLine, 0, SMTO_NORMAL, iTimeOut, &lpdwResult);

			HDC hDC = GetDC(CWinConsole::m_hWnd);

			if ( hDC )
			{
				SIZE TextSize;
				GetTextExtentPoint32(hDC, buf, indx, &TextSize);
				if ( TextSize.cx > tempMaxTextWidth )
					tempMaxTextWidth = TextSize.cx;
				ReleaseDC(CWinConsole::m_hWnd, hDC);
			}

			indx = 0;

		} // for

		if(indx > 0)
		{
			buf[indx] = 0;
			DWORD lpdwResult = 0;

			Sleep(1);

			LRESULT lResult = SendMessageTimeout(CWinConsole::m_hListBox, LB_ADDSTRING,
					0, (LPARAM)buf, SMTO_NORMAL, iTimeOut, &lpdwResult);;

			if ( CWinConsole::EnableWinConsoleOption(2) )
			{
				OutputDebugString(buf);
				OutputDebugString("\n");
			}

			if ( lResult == -2 || lResult == -1 )
				return; // trick or dead-lock provoking?

			DWORD dwLine = lpdwResult;
			SendMessageTimeout(CWinConsole::m_hListBox, LB_SETCURSEL, 
				dwLine, 0, SMTO_NORMAL, iTimeOut, &lpdwResult);

			HDC hDC = GetDC(CWinConsole::m_hWnd);

			if ( hDC )
			{
				SIZE TextSize;
				GetTextExtentPoint32A(hDC, buf, indx, &TextSize);

				if ( TextSize.cx > tempMaxTextWidth )
					tempMaxTextWidth = TextSize.cx;

				ReleaseDC(CWinConsole::m_hWnd, hDC);
			}

		} // if(indx > 0)


		if ( CWinConsole::m_MaxTextWidth < tempMaxTextWidth )
		{
			CWinConsole::m_MaxTextWidth = tempMaxTextWidth;

			DWORD lpdwResult = 0;
			SendMessageTimeout(CWinConsole::m_hListBox, LB_SETHORIZONTALEXTENT,
					CWinConsole::m_MaxTextWidth + 10, 0,
					SMTO_NORMAL, iTimeOut, &lpdwResult);

		}

		InvalidateRect(CWinConsole::m_hWnd, NULL, TRUE);

	}

	LeaveCriticalSection(&WinConCritical);
}

LRESULT __stdcall CWinConsole::SubclassInputEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	long lSizeofString;

	switch(message)
	{
		case WM_KEYFIRST:
		{
			// didn't find any info about this
			switch(wParam)
			{
				case 38:
					CWinConsole::ChangeFromBackUpToString(1);
				break;

				case 40:
					CWinConsole::ChangeFromBackUpToString(0);
				break;
			}
		}
		break;

		case WM_CHAR:
			if(wParam == 13)
			{
				lSizeofString = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
				SendMessage(hWnd, WM_GETTEXT, lSizeofString + 1, (LPARAM)CWinConsole::m_CommandBuffer);

				CWinConsole::ChangeFromStringToBackUp(1);

				SendMessage(CWinConsole::m_hWnd, 0x404, 0, lSizeofString);
				SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)"");

				return 1;
			}
		break;
	}


	return CallWindowProc(lpfnInputEdit, hWnd, message, wParam, lParam);
}

void CWinConsole::Init(HINSTANCE hInstance)
{
	WNDCLASS WndClass;
	m_hInstance = hInstance;

	m_ScrollyPos = 0;
	m_hMainMenu = CreateMenu();
	m_hSubMenu = CreatePopupMenu();

	AppendMenuA(m_hSubMenu, 0, IDM_WINCONS_TOPMOST, "윈도 항상 최상위(&T)");	// [T]opmost ?
	AppendMenuA(m_hSubMenu, MF_SEPARATOR, 0, 0);
	AppendMenuA(m_hSubMenu, 0, IDM_WINCONS_DEBUG, "디버그창 출력(&D)");			// [D]ebugOut ?
	AppendMenuA(m_hSubMenu, MF_SEPARATOR, 0, 0);
	AppendMenuA(m_hSubMenu, 0, IDM_WINCONS_SAVE, "콘솔 윈도 저장(&S)");			// [S]ave ?
	AppendMenuA(m_hSubMenu, 0, IDM_WINCONS_CLEAR, "콘솔 윈도 지우기(&C)");		// [C]lear ?
	AppendMenuA(m_hSubMenu, MF_SEPARATOR, 0, 0);
	AppendMenuA(m_hSubMenu, 0, IDM_WINCONS_EXIT, "콘솔 윈도 종료(&X)");			// e[X]it ?
	InsertMenuA(m_hMainMenu, 0, MF_POPUP, (UINT_PTR)m_hSubMenu, "콘솔 윈도(&C)"); // no idea


	WndClass.style = 0;
	WndClass.lpfnWndProc = (WNDPROC)CWinConsole::MsgProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 4;			// ??
	WndClass.hInstance = m_hInstance;
	WndClass.hIcon = NULL;
	WndClass.hCursor = LoadCursor(NULL, (LPCSTR)IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "WinConsole";
	RegisterClass(&WndClass);

	CWinConsole::m_hWnd = CreateWindowEx(0, "WinConsole", "MU WinConsole",
							WS_OVERLAPPEDWINDOW,
							0, 0, 640, 480,
							NULL, m_hMainMenu, m_hInstance, NULL);

	UpdateWindow(CWinConsole::m_hWnd);


	CWinConsole::m_hListBox = CreateWindowEx(0, "LISTBOX", "",
					WS_CHILD|WS_VSCROLL|WS_HSCROLL|WS_BORDER|(LBS_HASSTRINGS | LBS_USETABSTOPS),
					2, 2, 636, 462,
					CWinConsole::m_hWnd, 
					(HMENU)0xA7, m_hInstance, NULL);

	ShowWindow(CWinConsole::m_hListBox, SW_SHOW);
	UpdateWindow(CWinConsole::m_hListBox);


	CWinConsole::m_MaxTextWidth = 320;
	CWinConsole::m_hEditControl = CreateWindowEx(0, "EDIT", "",
							WS_CHILD | ES_AUTOHSCROLL,
							2, 462, 636, 16,
							CWinConsole::m_hWnd,
							(HMENU)0xA7, m_hInstance, 0);

	ShowWindow(CWinConsole::m_hEditControl, SW_SHOW);
	UpdateWindow(CWinConsole::m_hEditControl);


	SendMessage(CWinConsole::m_hEditControl, EM_SETLIMITTEXT, WINCONSOLE_LINE_LENGTH, 0);
	m_ScrollyPos = 0;

	lpfnInputEdit = (WNDPROC)SetWindowLong(
							CWinConsole::m_hEditControl,
							GWL_WNDPROC,
							(LONG)CWinConsole::SubclassInputEditProc);

	g_Console->ResizeControls();

}

void CWinConsole::ClearDbgWnd(void)
{
	DWORD lpdwResult;
	int iTimeOut;

	if ( g_Console && CWinConsole::m_hWnd)
	{
		iTimeOut = 1;
		lpdwResult = 0;

		SendMessageTimeout(CWinConsole::m_hListBox, LB_RESETCONTENT,
				0, 0, SMTO_NORMAL, 1, &lpdwResult);

		CWinConsole::m_MaxTextWidth = 320;

		SendMessageTimeout(CWinConsole::m_hListBox, LB_SETHORIZONTALEXTENT,
				CWinConsole::m_MaxTextWidth + 10, 0, 
				SMTO_NORMAL, iTimeOut, &lpdwResult);

		InvalidateRect(CWinConsole::m_hWnd, NULL, TRUE);
	}
}


HWND CWinConsole::GetHWND(void)
{
	return CWinConsole::m_hWnd;
}

bool CWinConsole::IsWinActive(void)
{
	return CWinConsole::m_bWinIsActive;
}

bool CWinConsole::HasFocusEditCtrl(void)
{
	if(CWinConsole::m_hEditControl)
		return (CWinConsole::m_hEditControl == GetFocus());

	return false;
}

void CWinConsole::ChangeFromStringToBackUp(BOOL bSetNext)
{
	strcpy(CWinConsole::m_pszBackUpString[CWinConsole::m_iBackUpLineNo], 
				CWinConsole::m_CommandBuffer);

	if ( bSetNext )
	{
		++CWinConsole::m_iBackUpLineNo;
		if ( CWinConsole::m_iBackUpLineNo >= WINCONSOLE_LINE_COUNT )
			CWinConsole::m_iBackUpLineNo = 0;
	}
	else
	{
		--CWinConsole::m_iBackUpLineNo;
		if ( CWinConsole::m_iBackUpLineNo < 0 )
			CWinConsole::m_iBackUpLineNo = WINCONSOLE_LINE_COUNT-1;
	}

	CWinConsole::m_iCurrentBackUpLineNo = CWinConsole::m_iBackUpLineNo;
}

void CWinConsole::ChangeFromBackUpToString(BOOL bGetNext)
{
	BOOL bExist = FALSE;
	if ( bGetNext )
	{
		if ( CWinConsole::m_iCurrentBackUpLineNo < WINCONSOLE_LINE_COUNT-1 )
			++CWinConsole::m_iCurrentBackUpLineNo;
		else
			CWinConsole::m_iCurrentBackUpLineNo = 0;

		for (int i = CWinConsole::m_iCurrentBackUpLineNo; i < WINCONSOLE_LINE_COUNT; ++i )
		{
			if ( strlen(CWinConsole::m_pszBackUpString[i]) )
			{
				CWinConsole::m_iCurrentBackUpLineNo = i;
				bExist = TRUE;
				break;
			}
		}

		if ( !bExist )
		{
			for (int i = 0; i < CWinConsole::m_iCurrentBackUpLineNo; ++i )
			{
				if ( strlen(CWinConsole::m_pszBackUpString[i]) )
				{
					CWinConsole::m_iCurrentBackUpLineNo = i;
					bExist = TRUE;
					break;
				}
			}
		}
	}
	else
	{
		if ( CWinConsole::m_iCurrentBackUpLineNo <= 0 )
			CWinConsole::m_iCurrentBackUpLineNo = WINCONSOLE_LINE_COUNT-1;
		else
			--CWinConsole::m_iCurrentBackUpLineNo;

		for ( int i = CWinConsole::m_iCurrentBackUpLineNo; i >= 0; --i )
		{
			if ( strlen(CWinConsole::m_pszBackUpString[i]) )
			{
				CWinConsole::m_iCurrentBackUpLineNo = i;
				bExist = TRUE;
				break;
			}
		}

		if ( !bExist )
		{
			for ( int i = WINCONSOLE_LINE_COUNT-1; i > CWinConsole::m_iCurrentBackUpLineNo; --i )
			{
				if ( strlen(CWinConsole::m_pszBackUpString[i]) )
				{
					CWinConsole::m_iCurrentBackUpLineNo = i;
					bExist = TRUE;
					break;
				}
			}
		}
	}

	if ( bExist )
	{
		ZeroMemory(CWinConsole::m_CommandBuffer, sizeof(CWinConsole::m_CommandBuffer));
		SendMessage(CWinConsole::m_hEditControl,
				WM_SETTEXT, 0,
				(LPARAM)CWinConsole::m_pszBackUpString[CWinConsole::m_iCurrentBackUpLineNo]);
	}
}


bool CWinConsole::EnableWinConsoleOption(DWORD dwWinConsoleOpt)
{
	if(g_Console && CWinConsole::m_hWnd)
	{
		if( (g_Console->GetWinConsoleOpt() & dwWinConsoleOpt) )
			return true;
	}

	return false;
}

DWORD CWinConsole::GetWinConsoleOpt(void)
{
	return m_dwWinConsoleOpt;
}

void CWinConsole::AddWinConsoleOption(DWORD dwWinConsoleOpt)
{
	m_dwWinConsoleOpt |= dwWinConsoleOpt;
}

void CWinConsole::SubtractWinConsoleOption(DWORD dwWinConsoleOpt)
{
	m_dwWinConsoleOpt &= (~dwWinConsoleOpt);
}
