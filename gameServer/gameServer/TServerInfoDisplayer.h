// TServerInfoDisplayer.h: interface for the TServerInfoDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSERVERINFODISPLAYER_H__DE0B5062_F5FC_4F24_8EF8_67E2D276CF7D__INCLUDED_)
#define AFX_TSERVERINFODISPLAYER_H__DE0B5062_F5FC_4F24_8EF8_67E2D276CF7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TServerInfoDisplayer
{

public:

	TServerInfoDisplayer();
	virtual ~TServerInfoDisplayer();

	void Run(HWND hWnd);
	void CheckLogFileSize();
	//void __thiscall CheckPerformance();
	void CheckJSConnection();
	void CheckFSConnection();
	void CheckDSConnection();
	void CheckEXDSConnection();
	void CheckEVDSConnection();
	void CheckRKDSConnection();
	void InitGDIObject();
	void DelGDIObject();
	void PaintAllInfo(HWND hWnd, int iTopLeftX, int iTopLeftY);

private:

	BOOL m_bValidJSConnection;	// 4
	BOOL m_bValidFSConnection;	// 8
	BOOL m_bValidDSConnection;	// C
	BOOL m_bValidEXDSConnection;	// 10
	BOOL m_bValidEVDSConnection;	// 14
	BOOL m_bValidRKDSConnection;	// 18
	HFONT m_hFont;	// 1C
	HBRUSH m_hNormalBrush;	// 20
	HBRUSH m_hAbnormalBrush;	// 24
};

extern TServerInfoDisplayer g_ServerInfoDisplayer;

#endif // !defined(AFX_TSERVERINFODISPLAYER_H__DE0B5062_F5FC_4F24_8EF8_67E2D276CF7D__INCLUDED_)