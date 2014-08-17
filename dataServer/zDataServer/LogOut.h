#pragma once
#include "StdAfx.h"
#include "ConnectServer.h"
#include "winutil.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace std;

/*#include <boost/date_time/gregorian/gregorian.hpp>
boost::gregorian::date ;
boost::gregorian::day_clock::get_local_time();*/

class CLOG
{
public:

	bool bIsLogWrite;
	bool bIsLogWriteFile;
	std::vector<std::string> LogText;
	
	//------------------------------------------------------------

	CLOG()
	{
		this->LogInit();
	}

	//------------------------------------------------------------

	~CLOG()
	{
		this->LogClose();
	}

	//------------------------------------------------------------

	std::string Time()
	{
		SYSTEMTIME t;
		GetLocalTime(&t);
		std::ostringstream str("");
		str<< "[";
	
		t.wHour >= 10? str << t.wHour <<":" : str << "0"<<t.wHour << ":";
		t.wMinute >= 10? str << t.wMinute << ":" : str << "0"<<t.wMinute << ":";
		t.wSecond >= 10? str << t.wSecond : str << "0"<<t.wSecond;
		str<<"]";
		
		return str.str();
	}

	//------------------------------------------------------------

	void LogDataSet()
	{
		char szTemp[70];
	
		GetLocalTime(&this->LogTime);
	
		sprintf(szTemp,"%s\\%02d%02d%02d.log","LOG",LogTime.wYear, LogTime.wMonth, LogTime.wDay);

		if (this->LogFile.is_open())
		{
			this->LogFile.close();
		}

		this->LogFile.open(szTemp, std::ios::out | std::ios::app);
		if (!this->LogFile.is_open())
		{
			this->LogAdd("LogFile file create error (%s)",szTemp);
		}
		strcpy(this->LogFileName,szTemp);
	}

	//---------------------------------------------------------------------------

	bool LogDateChange()
	{
		char szTemp[70];
		SYSTEMTIME CurTime;
		GetLocalTime(&CurTime);

		if ( (CurTime.wYear != this->LogTime.wYear) || (CurTime.wMonth != this->LogTime.wMonth) || (CurTime.wDay != this->LogTime.wDay) )
			return false;

		GetLocalTime(&this->LogTime);

		sprintf(szTemp,"%s\\%02d%02d%02d.log","LOG",this->LogTime.wYear, this->LogTime.wMonth, this->LogTime.wDay);

		EnterCriticalSection(&this->LogCritical);
	
		if (this->LogFile.is_open())
		{
			this->LogFile.close();
		}

		this->LogFile.open(szTemp, std::ios::out);
		if (!this->LogFile.is_open())
		{
			this->LogAdd("LogFile file create error (%s)",szTemp);
		}
		strcpy(this->LogFileName,szTemp);
		LeaveCriticalSection(&this->LogCritical);
		return true;
	}

	//---------------------------------------------------------------------------

	void LogTextAdd(const char* msg)
	{
		EnterCriticalSection(&this->LogCritical);
		this->LogText.push_back(msg);
		LeaveCriticalSection(&this->LogCritical);
	}

	//---------------------------------------------------------------------------

	void LogAdd(char* szLog, ...)
	{
		if (bIsLogWrite==0)
			return;
			
		char szBuffer[512];
	
		va_list pArguments;
	
		va_start( pArguments,  szLog);	// review
		vsprintf(szBuffer, szLog, pArguments);
		va_end(pArguments);

		std::string str = Time() + " " + szBuffer;
		this->LogTextAdd(str.c_str());

		if (bIsLogWriteFile == 0)
			return;
		else
		{
			EnterCriticalSection(&this->LogCritical);
			if (!this->LogFile.is_open())
				this->LogDataSet();
		
			this->LogFile<<str.c_str()<<"\n";
			this->LogFile.flush();
			LeaveCriticalSection(&this->LogCritical);
		}
	}

	//---------------------------------------------------------------------------
	
	void LogAdd(const char* szLog)
	{
		if (bIsLogWrite == 0)
			return;
			
		std::string str = Time() + " " + szLog;
		this->LogTextAdd(str.c_str());

		if (bIsLogWriteFile == 0)
			return;
		else
		{
			EnterCriticalSection(&this->LogCritical);
			if (!this->LogFile.is_open())
				this->LogDataSet();
		
			this->LogFile<<str.c_str()<<"\n";
			this->LogFile.flush();
			LeaveCriticalSection(&this->LogCritical);
		}
	}
	
private:

	char LogFileName[MAX_PATH];
	SYSTEMTIME LogTime;
	CRITICAL_SECTION LogCritical;	
	boost::filesystem::ofstream LogFile;

	//---------------------------------------------------------------------------

	void LogInit()
	{
		CreateDirectory("LOG",NULL);
		InitializeCriticalSection(&this->LogCritical);

		std::string dir = CurrentDirA();

		this->bIsLogWrite = GetPrivateProfileIntA("ConnectServer","IsLogWrite",1,dir.c_str());
		this->bIsLogWriteFile = GetPrivateProfileIntA("ConnectServer","IsLogWriteFile",1,dir.c_str());

		if (bIsLogWriteFile)
			this->LogDataSet();
	}

	//---------------------------------------------------------------------------
	
	void LogClose()
	{
		if (this->LogFile.is_open())
			LogFile.close();
		
		DeleteCriticalSection(&this->LogCritical);
	}

	//---------------------------------------------------------------------------
};

extern CLOG Log;



