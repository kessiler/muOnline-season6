// TServerInfoDisplayer.cpp: implementation of the TServerInfoDisplayer class.
//	GS-N	1.00.90	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TServerInfoDisplayer.h"
#include "Gamemain.h"
#include "DBSockMng.h"
#include "LogToFile.h"

TServerInfoDisplayer g_ServerInfoDisplayer;
CLogToFile SERVER_CONN_STATE_LOG("ConnectState", ".\\LOG\\ConnectState", 1);

//#if(_GSCS==1)
static char * ServerTypeText[1] =
{
	"GAMESERVER" 
};
//#else
//static char * ServerTypeText[1] =
//{
//	"GAMESERVER-N"
//};
//#endif

static char * ErrorMessge[8] = 
{
	"0) JoinServer - Disconnected",
	"1) FsGateServer - Disconnected",
	"2) DataServer - Disconnected",
	"3) ExDBServer - Disconnected",
	"4) EventServer - Disconnected",
	"5) RankingServer - Disconnected"
};




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TServerInfoDisplayer::TServerInfoDisplayer()
{
	this->m_bValidJSConnection = FALSE;
	this->m_bValidFSConnection = FALSE;
	this->m_bValidDSConnection = FALSE;
	this->m_bValidEXDSConnection = FALSE;
	this->m_bValidEVDSConnection = FALSE;
	this->m_bValidRKDSConnection = FALSE;

	this->InitGDIObject();
}

TServerInfoDisplayer::~TServerInfoDisplayer()
{
	this->DelGDIObject();
}



void TServerInfoDisplayer::InitGDIObject()
{
#ifdef ZEONWINDOW
	this->m_hFont = CreateFont(60, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, 
		1, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, "Times");
#else
	this->m_hFont = CreateFont(80, 0, 0, 0, FW_THIN, FALSE, TRUE, FALSE, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, "Times");
#endif
	this->m_hNormalBrush	= CreateSolidBrush(RGB(110, 240, 120));
	this->m_hAbnormalBrush	= CreateSolidBrush(RGB(250, 110, 110));
}



void TServerInfoDisplayer::DelGDIObject()
{
	DeleteObject(this->m_hFont);
	DeleteObject(this->m_hNormalBrush);
	DeleteObject(this->m_hAbnormalBrush);
}



void TServerInfoDisplayer::Run(HWND hWnd)
{
	this->CheckDSConnection();
	this->CheckJSConnection();
	this->CheckFSConnection();
	this->CheckEXDSConnection();
	this->CheckEVDSConnection();
	this->CheckRKDSConnection();

	this->CheckLogFileSize();

	this->PaintAllInfo(hWnd, 0, 20);
}


void TServerInfoDisplayer::CheckLogFileSize()
{
	return;
}



void TServerInfoDisplayer::CheckJSConnection()
{
	if ( wsJServerCli.GetSocket() == INVALID_SOCKET )
		this->m_bValidJSConnection = FALSE;
	else
		this->m_bValidJSConnection = TRUE;
}



void TServerInfoDisplayer::CheckDSConnection()
{
	if ( cDBSMng.wsCDBS[0].GetSocket() == INVALID_SOCKET )
		this->m_bValidDSConnection = FALSE;
	else
		this->m_bValidDSConnection = TRUE;
}



void TServerInfoDisplayer::CheckFSConnection()
{
	this->m_bValidFSConnection = TRUE;
}


void TServerInfoDisplayer::CheckEXDSConnection()
{
	if ( wsExDbCli.GetSocket() == INVALID_SOCKET )
		this->m_bValidEXDSConnection = FALSE;
	else
		this->m_bValidEXDSConnection = TRUE;
}


void TServerInfoDisplayer::CheckEVDSConnection()
{
	if ( IsEventChipServerConnected == FALSE )
		this->m_bValidEVDSConnection = FALSE;
	else
		this->m_bValidEVDSConnection = TRUE;
}



void TServerInfoDisplayer::CheckRKDSConnection()
{
	if ( IsDevilSquareEventConnected == FALSE )
		this->m_bValidRKDSConnection = FALSE;
	else
		this->m_bValidRKDSConnection = TRUE;
}



void TServerInfoDisplayer::PaintAllInfo(HWND hWnd, int iTopLeftX, int iTopLeftY)
{
	HDC hDC = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.top = 20;
	rect.bottom = 80;
	int iLineCount = 0;

	int iOldBkMode = SetBkMode(hDC, TRANSPARENT);

	if ( this->m_bValidJSConnection != FALSE && this->m_bValidDSConnection != FALSE && this->m_bValidFSConnection != FALSE )
		FillRect(hDC, &rect, this->m_hNormalBrush);
	else
		FillRect(hDC, &rect, this->m_hAbnormalBrush);

	if ( this->m_bValidJSConnection == FALSE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX, iTopLeftY+iLineCount*15, ErrorMessge[0], strlen(ErrorMessge[0]));
		iLineCount++;
	}

	if ( this->m_bValidFSConnection == FALSE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX,  iTopLeftY+iLineCount*15, ErrorMessge[1], strlen(ErrorMessge[1]));
		iLineCount++;
	}

	if ( this->m_bValidDSConnection == FALSE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX, iTopLeftY+iLineCount*15, ErrorMessge[2], strlen(ErrorMessge[2]));
		iLineCount++;
	}

	if ( this->m_bValidEXDSConnection == FALSE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX, iTopLeftY+iLineCount*15, ErrorMessge[3], strlen(ErrorMessge[3]));
		iLineCount++;
	}

	if ( this->m_bValidEVDSConnection == FALSE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX,  iTopLeftY+iLineCount*15, ErrorMessge[4], strlen(ErrorMessge[4]));
		iLineCount++;
	}

	if ( this->m_bValidRKDSConnection == FALSE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX, iTopLeftY+iLineCount*15, ErrorMessge[5], strlen(ErrorMessge[5]));
		iLineCount++;
	}

	if ( this->m_bValidJSConnection == FALSE || this->m_bValidDSConnection == FALSE || this->m_bValidFSConnection == FALSE || this->m_bValidEXDSConnection == FALSE || this->m_bValidEVDSConnection == FALSE || this->m_bValidRKDSConnection == FALSE )
	{
		SERVER_CONN_STATE_LOG.Output("Server Interconnection Check - [FS/%d] [DS/%d] [EX/%d] [ES/%d] [RS/%d]",
			this->m_bValidFSConnection, this->m_bValidDSConnection, 
			this->m_bValidEXDSConnection, this->m_bValidEVDSConnection,
			this->m_bValidRKDSConnection);	// #error Deathway Tip ( Add also for JoinServer )
	}

	HFONT pOldFont = (HFONT)SelectObject(hDC, this->m_hFont);
	SetTextColor(hDC, RGB(250, 250, 250));

	TextOutA(hDC, GAMESERVER_WIDTH / 2 - 200, 18, ServerTypeText[0], strlen(ServerTypeText[0]));
	SelectObject(hDC, pOldFont);
	SetBkMode(hDC, iOldBkMode);
	ReleaseDC(hWnd, hDC);
}