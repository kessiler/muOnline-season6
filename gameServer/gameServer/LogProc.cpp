//GameServer 1.00.90
#include "stdafx.h"
#include "logproc.h"
#include "GameMain.h"
#include "GameServer.h"

#define	WRITETOFILE	1

int m_cline;	// Current Line of Log
static FILE* logfp;	// FILE Variable for LOG
static char m_szLogFileName[100];	// Log Name

char LogText[LOG_TEXT_LINE][LOG_TEXT_LENGTH];
short LogTextLength[LOG_TEXT_LINE];
BYTE LogTextViewType[LOG_TEXT_LINE];

// Functions Pointers to Log Variables
void (*LogAdd)(char* szLog,...);
void (*LogAddC)(BYTE, char*, ...);
void (*LogAddTD)(char* szLog, ...);
void (*LogAddHeadHex)(char*, char*, int);
void (*LogAddL)(char* szLog, ...);
void (*LogTextPaint)(HWND);

CRITICAL_SECTION LogCritical;	// Critical Section For LOG

// Log Set Internals
int		LogMDay;
int		LogMonth;
int		LogMYear;
HFONT	LogFont;
int		gLogOutType = 1; // THIS IS NOT THE PLACE OF TTHIS VARIABLE

void LogInit(int logprint)
{
	int n;
	InitializeCriticalSection(&LogCritical);

	// Create yes no Log
	if (logprint!=0)		
	{

		LogAdd=LogAddFunc;
		LogAddC=LogAddFuncColor;
		LogAddTD=LogAddTimeDateFunc;
		LogAddHeadHex=LogAddStrHexFunc;
		LogTextPaint=LogTextPaintProc;
		LogAddL=LogAddLocalFunc;

		for (n=0;n<LOG_TEXT_LINE;n++)
		{
			memset(&LogText[n],0, sizeof(LogText[0]));
			LogTextLength[n]=0;
			LogTextViewType[n]=0;
		}
		if (gLogOutType==0)
		{
			return;
		}
		else
		{
			LogDataSet();
		}
	}
	else
	{

		LogAdd=LogAddFuncVoid;
		LogAddHeadHex=LogAddHeadHexFuncVoid;
		LogTextPaint=LogTextPaintProcVoid;
		LogAddTD=LogAddTimeDateVoid;
		LogAddL=LogAddLocalFuncVoid;
		
	}
}


void LogDataSet()
{
	
	char szTemp[250];
	
	struct tm *today;
	time_t ltime;	
	

	time(&ltime);
	today=localtime(&ltime);
	
	
	today->tm_year=today->tm_year + 1900;
	LogMYear=today->tm_year ;
	LogMonth=today->tm_mon+1;
	LogMDay=today->tm_mday;

	wsprintf(szTemp,"log\\%02d%02d%02d_%s.log",LogMYear, LogMonth, LogMDay, &szServerName);

	if (logfp != 0)
	{
		fclose(logfp);
		logfp=0;
	}

	logfp=fopen(szTemp,"a+t");
	if (logfp==0)
	{
		LogAdd("Log file create error");
	}
	strcpy(m_szLogFileName,szTemp);
}




int LogDateChange()
{
	char szTemp[250];

	tm* today;
	time_t ltime;
	time(&ltime);
	

	today=localtime(&ltime);
	today->tm_year =today->tm_year+1900;
	today->tm_mon = today->tm_mon +1;

	if (today->tm_year <= LogMYear)
	{
		if (today->tm_mon <= LogMonth)
		{
			if (today->tm_mday <= LogMDay)
			{
				return 0;
			}
		}
	}


	LogMYear = today->tm_year ;
	LogMonth = today->tm_mon ;
	LogMDay = today->tm_mday ;

	wsprintf(szTemp, "log\\%02d%02d%02d_%s.log", LogMYear, LogMonth, LogMDay, &szServerName);

	EnterCriticalSection(&LogCritical);
	
	if (logfp!=0)
	{
		fclose(logfp);
		logfp=0;
	}
	logfp=fopen(szTemp, "a+t");
	strcpy(m_szLogFileName, szTemp);
	LeaveCriticalSection(&LogCritical);
	return 1;
}


#if (LOG_INMEDIATLY==1)
#include "TServerInfoDisplayer.h"
extern TServerInfoDisplayer g_ServerInfoDisplayer; 
void GJNotifyMaxUserCount();
#endif


void LogTextAdd(BYTE type, char* msg, int len)
{
	
	if (len>LOG_TEXT_LENGTH-1)
	{
		len=LOG_TEXT_LENGTH-1;
	}
	m_cline++;
	if (m_cline>LOG_TEXT_LINE-1)
	{
		m_cline=0;
	}
	LogText[m_cline][0]=0;
	memcpy(&LogText[m_cline], msg, len);
	LogText[m_cline][1+len]=0;
	LogText[m_cline][len]=0;
	LogTextLength[m_cline]=len;
	LogTextViewType[m_cline]=type;	// Phew, 

#if (LOG_INMEDIATLY==1)
						RECT rect;
						HDC hdc = GetDC(ghWnd);
						GetClientRect(ghWnd, &rect);
						FillRect(hdc, &rect, (HBRUSH)GetStockObject(0));
						ReleaseDC(ghWnd, hdc);

						if ( gCurPaintType == 0)
						{
							if ( LogTextPaint != NULL )
							{
								LogTextPaint(ghWnd);
							}
						}
						
						gObjViewportPaint(ghWnd, gCurPaintPlayer);
						//g_ServerInfoDisplayer.Run(ghWnd);
						//GJNotifyMaxUserCount();
#endif
}



void LogAddFuncVoid(char* szLog, ...)
{
	// Nothing to Add
}


void LogAddTimeDateVoid(char* szLog, ...)
{
	// Log Disabled
}



void LogAddFunc(char* szLog, ...)
{
	char szBuffer[512]="";
	
	va_list pArguments;
	
	va_start( pArguments,  szLog);	// review
	vsprintf(szBuffer, szLog, pArguments);
	va_end(pArguments);
	LogTextAdd(0,szBuffer, strlen(szBuffer));
	if (gLogOutType==0)
	{
		return;
	}
	else
	{
#if ( WRITETOFILE == 1 )
		EnterCriticalSection(&LogCritical);
		fprintf(logfp, "%s\n", szBuffer);
		LeaveCriticalSection(&LogCritical);
#endif
	}
}


void LogAddFuncColor(BYTE Color, char* szLog, ...)
{
	char szBuffer[1024]="";
	va_list pArguments;
	
	va_start(pArguments, szLog);
	vsprintf(szBuffer, szLog, pArguments);
	va_end(pArguments);
	LogTextAdd(Color, szBuffer, strlen(szBuffer));

	if (!gLogOutType)
	{
		return;
	}
	else
	{
#if ( WRITETOFILE == 1 )
		EnterCriticalSection(&LogCritical);
		fprintf(logfp, "%s\n", szBuffer);
		LeaveCriticalSection(&LogCritical);
#endif
	}
}

#pragma warning ( disable : 4101 )
void LogAddLocalFunc(char* szLog, ...)
{
	char szBuffer[512];
	va_list pArguments;

#if (LOCAL_LOG==1)	
	va_start(pArguments, szLog);
	vsprintf(szBuffer, szLog, pArguments);
	va_end(pArguments);

	LogTextAdd(5, szBuffer, strlen(szBuffer));


	if (!gLogOutType)
	{
		return;
	}
	else
	{
		EnterCriticalSection(&LogCritical);
		fprintf(logfp, "[LOCAL] %s\n", szBuffer);
		LeaveCriticalSection(&LogCritical);
	}

#endif
}
#pragma warning ( default : 4101 )

void LogAddLocalFuncVoid(char* szLog, ...)
{

}


void LogAddTimeDateFunc(char* szLog, ...)
{
	char szBuffer[1024]="";
	va_list pArguments;
	tm * today;
	time_t ltime;
	char tmpbuf[512];
	
	
	time(&ltime);
	today=localtime(&ltime);
#ifdef ZEONWINDOW
	wsprintf(tmpbuf, "[%.8s] ", asctime(today)+11 );
#else
	wsprintf(tmpbuf, "%.8s", asctime(today)+11 );
#endif

	va_start( pArguments,szLog );
	vsprintf(szBuffer, szLog, pArguments);
	va_end(pArguments);

	strcat(tmpbuf, szBuffer);

	LogTextAdd(0, tmpbuf, strlen(tmpbuf));

	if (gLogOutType==0)
	{
		return;
	}
	else
	{
		if (logfp==0)
		{
			LogAdd("error-L2 : file create error %s %d", __FILE__, __LINE__);
		}
		else
		{
#if ( WRITETOFILE == 1 )
			EnterCriticalSection(&LogCritical);
			fprintf(logfp, "%s\n", &tmpbuf);
			LeaveCriticalSection(&LogCritical);
#endif
		}
	}
}


void LogAddHeadHexFuncVoid(char* str,char* data,int len)
{
	// Log Disabled
}


void LogAddHeadHexFunc(int Type,char* data,int len)
{
	signed int n;
	
	if ( gLogOutType==0)
	{
		return;
	}
	else
	{
		if (logfp==0)
		{
			return;
		}
		else
		{
#if ( WRITETOFILE == 1 )
			EnterCriticalSection(&LogCritical);

			switch(Type)
			{
			case 2:
				fprintf(logfp, "Send:[%d] : (", len);
				break;
			case 3:
				fprintf(logfp, "FDWrite:[%d] : (", len);
				break;
			case 4:
				fprintf(logfp, "Recv:[%d] : (", len);
				break;
			default:
				fprintf(logfp, "%d[%d] : (", Type ,len);
				break;
			}
			
			for (n=0; n<len; n++) 
			{
				fprintf(logfp, "0x%02x ", *(BYTE *)(data+n));
			}

			fprintf(logfp, ")\n");

			LeaveCriticalSection(&LogCritical);
#endif
		}
	}
}


void LogAddStrHexFunc(char* str,char* data, int len)
{
	tm* today;
	time_t ltime;
	signed int n;

	time(&ltime);
	today=localtime(&ltime);

	if (gLogOutType==0)
	{
		return;
	}
	else
	{
		if (logfp==0)
		{
			return;
		}
		else
		{
#if ( WRITETOFILE == 1 )
			EnterCriticalSection(&LogCritical);
			fprintf(logfp, "%.8s", asctime(today)+11); //HermeX Fix
			fprintf(logfp, "[%s][%d] : (", str,len);

			for (n=0; n<len; n++ )
			{
				fprintf(logfp, "0x%02x ", *(BYTE *)(data+n));
			}
			fprintf(logfp, ")\n");

			LeaveCriticalSection(&LogCritical);
#endif
		}
	}
}

void LogTextClear()
{
	int n;
	for ( n=0; n<LOG_TEXT_LINE; n++)
	{
		memset(&LogText[n], 0, LOG_TEXT_LENGTH);
		LogTextLength[n]=0;	// WORD
		LogTextViewType[n]=0;	// BYTE
	}
}


void LogTextPaintProcVoid(HWND hWnd)
{
	// Log Disabled
}


void LogTextPaintProc(HWND hWnd)
{
	HDC			hdc;
	RECT		rect;
	int			total;
	int			n;
	// ----
	hdc			= GetDC(hWnd);
	total		= LOG_TEXT_LINE;
	n			= m_cline;

	while(total-- != 0)
	{
		switch (LogTextViewType[n] )
		{
		case 2:
			SetTextColor(hdc, RGB(255, 0, 0) );	
			break;
		case 3:
			SetTextColor(hdc, RGB(0, 100, 0));
			break;
		case 4:
			SetTextColor(hdc, RGB(0, 0, 255));
			break;
		case 5:
			SetTextColor(hdc, RGB(155, 0, 0));
			break;
		case 6:
			SetTextColor(hdc, RGB(0, 0, 100));
			break;
		case 7:
			SetTextColor(hdc, RGB(210, 30, 150));
			break;
		default:
			SetTextColor(hdc, RGB(0, 0, 0));
			break;
		}
		if (strlen(LogText[n])>1)
		{
			TextOutA(hdc, 0, total * 15 + 80, LogText[n], strlen(LogText[n])); 
		}
		n--;
		if (n<0)
		{
			n=LOG_TEXT_LINE-1;
		}
	}
	ReleaseDC(hWnd, hdc);
}

int LogGetFileName()
{
	return 0;
}

void LogClose()
{
	if (logfp!=0)
	{
		fclose(logfp);
	}
	DeleteCriticalSection(&LogCritical);
}



void MsgBox(char *szlog, ...)
{
	char szBuffer[512]="";
	va_list pArguments;
	va_start(pArguments, szlog);
	vsprintf(szBuffer, szlog, pArguments);
	va_end(pArguments);
	MessageBox(NULL, szBuffer, "error", MB_OK|MB_APPLMODAL);
}

