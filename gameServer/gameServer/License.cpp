#include "StdAfx.h"
#include <fstream>
#include <iostream>
#include "License.h"
#include "wininet.h"
#include "ScriptEncode.h"
#include "MD5.h"
// -------------------------------------------------------------------------------

License g_License;
bool IsLicenseChecked = true;
// -------------------------------------------------------------------------------

License::License()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	this->Init();
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

License::~License()
{

}
// -------------------------------------------------------------------------------

void License::Init()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	ZeroMemory(&this->m_Data, sizeof(this->m_Data));
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

void License::Load()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	// ----
	this->Init();
#if( __ROOT__ == 1 )
	IsLicenseChecked = true;
	this->m_Data.CustomerID = Customer::Refinery; //test
#else
	SYSTEMTIME CurrentTime;
	GetLocalTime(&CurrentTime);
	// ----
	if( !g_ScriptEncode.ReadScript(GetText("+]kIhr`ob`/udu")) )
	{
		MessageBoxA(NULL, GetText("@scjs1fnu`;15"), GetText("Ihr`ob`!tws~w"), ERROR);
		ExitProcess(0);
	}
	// ----
	g_ScriptEncode.GetToken();
	g_ScriptEncode.GetToken();
	g_ScriptEncode.GetToken();
	g_ScriptEncode.GetToken();
	this->m_Data.CustomerID = g_ScriptEncode.GetNumber();
	this->m_Data.CurrentDay = (CurrentTime.wDay + CurrentTime.wMonth * 31 + CurrentTime.wYear * 365);
	//this->GetCustomState();
	this->CheckBan();
	this->GetDrive();
	this->GetAdapter();
	this->GetHash();
	this->SendRequest();
#endif
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

void License::GetCustomState()
{
	for( int i = 0; i < LICENSE_MAX_CUSTOMID; i++ )
	{
		this->m_Data.CustomState[i] = false;
	}
	// ----
	std::string URL;
	URL.resize(_MAX_PATH);
	//sprintf(&URL[0], "http://license.z-team.pro/Test/Database/Custom, GetText(LICENSE_SERVER), this->m_Data.CustomerID);
	URL.resize(strlen(URL.data())+ 1);
	// ----
	char Agent[200];
	ZeroMemory(Agent, sizeof(Agent));
	sprintf(Agent, "%s %s", GAMESERVER_NAME, GAMESERVER_VERSION);
	HINTERNET hSession	= InternetOpenA(Agent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hHandle	= InternetOpenUrlA(hSession, URL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	// ----
	if( hSession == NULL || hHandle == NULL )
	{
		MessageBoxA(NULL, GetText("@scjs1fnu`;13"), GetText("Ihr`ob`!tws~w"), ERROR);
		ExitProcess(0);
	}
	// ----
	DWORD Number;
	char Hash[100];
	ZeroMemory(Hash, sizeof(Hash));
	// ----
	InternetReadFile(hHandle, Hash, 32, &Number);
	InternetCloseHandle(hSession);
	InternetCloseHandle(hHandle);
	// ----
	if( Number == 1 )
	{
		this->SendAlert(1, 0);
		this->Destruct();
		ExitProcess(0);
	}
}
// -------------------------------------------------------------------------------

void License::GetDrive()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	char VolumeNameBuffer[100], FileSystemNameBuffer[100];
	LPDWORD Len, SystemFlag;
	GetVolumeInformationA(NULL, VolumeNameBuffer, 100, 
		&this->m_Data.DriveID, Len, SystemFlag, FileSystemNameBuffer, 100);
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

void License::GetAdapter()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	DWORD	BufferLength	= 0;
	LPBYTE	Buffer			= 0;
	// ----
	if( ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(0, &BufferLength) )
	{
		Buffer = new BYTE[BufferLength];
	}
	// ----
	PIP_ADAPTER_INFO AdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(Buffer);
	GetAdaptersInfo(AdapterInfo, &BufferLength);
	// ----
	if( AdapterInfo )
	{
		for( int i = 0; i < MAX_ADAPTER_ADDRESS_LENGTH; i++ )
		{
			this->m_Data.Adapter[i] = AdapterInfo->Address[i];
		}
	}
	else
	{
		MessageBoxA(NULL, GetText("@scjs1fnu`;17"), GetText("Ihr`ob`!tws~w"), ERROR);
		this->SendAlert(2, 2);
		ExitProcess(0);
	}
	// ----
	delete[] Buffer;
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

void License::GetHash()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	char Hash1[200], Hash2[200];
	ZeroMemory(Hash1, sizeof(Hash1)-1);
	ZeroMemory(Hash2, sizeof(Hash2)-1);
	// ----
	sprintf(Hash1, "%d%d%d%d%d%d%d%d%d%d",
		this->m_Data.CustomerID, this->m_Data.DriveID, this->m_Data.Adapter[0],
		this->m_Data.Adapter[1], this->m_Data.Adapter[2], this->m_Data.Adapter[3],
		this->m_Data.Adapter[4], this->m_Data.Adapter[5], this->m_Data.Adapter[6],
		this->m_Data.Adapter[7]);
	sprintf(Hash2, "%d%d",
		this->m_Data.CustomerID, this->m_Data.CurrentDay);
#ifdef VM_PROTECT
	VMEND
#endif
	GetMD5(Hash1, strlen(Hash1), this->m_Data.Hash1);
	GetMD5(Hash2, strlen(Hash2), this->m_Data.Hash2);
}
// -------------------------------------------------------------------------------

void License::SendRequest()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	std::string URL;
	URL.resize(_MAX_PATH);
	sprintf(&URL[0], GetText("mueu;>*$b*.C`pd`re+qyu>xa<4a'udut8$u"), GetText(LICENSE_SERVER), 
		this->m_Data.CustomerID, this->m_Data.CurrentDay);
	URL.resize(strlen(URL.data())+ 1);
	// ----
	char Agent[200];
	ZeroMemory(Agent, sizeof(Agent));
	sprintf(Agent, "%s %s", GAMESERVER_NAME, GAMESERVER_VERSION);
	HINTERNET hSession	= InternetOpenA(Agent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hHandle	= InternetOpenUrlA(hSession, URL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	// ----
	if( hSession == NULL || hHandle == NULL )
	{
		MessageBoxA(NULL, GetText("@scjs1fnu`;16"), GetText("Ihr`ob`!tws~w"), ERROR);
		ExitProcess(0);
	}
	// ----
	InternetCloseHandle(hSession);
	InternetCloseHandle(hHandle);
	// ----
	this->CheckKey();
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

void License::SendAlert(int Type, int Code)
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	std::string URL;
	URL.resize(_MAX_PATH);
	sprintf(&URL[0], GetText("mueu;>*$b*.C`pd`re+qyu>xa<4a'udut8$u#uhud, e7fnu`<4a"), 
		GetText(LICENSE_SERVER), 
		this->m_Data.CustomerID, this->m_Data.CurrentDay,
		Type, Code);
	URL.resize(strlen(URL.data())+ 1);
	// ----
	char Agent[200];
	ZeroMemory(Agent, sizeof(Agent));
	sprintf(Agent, "%s %s", GAMESERVER_NAME, GAMESERVER_VERSION);
	HINTERNET hSession	= InternetOpenA(Agent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hHandle	= InternetOpenUrlA(hSession, URL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	// ----
	if( hSession == NULL || hHandle == NULL )
	{
		MessageBoxA(NULL, GetText("@scjs1fnu`;16"), GetText("Ihr`ob`!tws~w"), ERROR);
		ExitProcess(0);
	}
	// ----
	InternetCloseHandle(hSession);
	InternetCloseHandle(hHandle);
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

void License::CheckKey()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	std::string URL;
	URL.resize(_MAX_PATH);
	sprintf(&URL[0], GetText("mueu;>*$b*.Pfbtvr>*$b+uiq"), GetText(LICENSE_SERVER), this->m_Data.Hash2);
	URL.resize(strlen(URL.data())+ 1);
	// ----
	char Agent[200];
	ZeroMemory(Agent, sizeof(Agent));
	sprintf(Agent, "%s %s", GAMESERVER_NAME, GAMESERVER_VERSION);
	HINTERNET hSession	= InternetOpenA(Agent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hHandle	= InternetOpenUrlA(hSession, URL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	// ----
	if( hSession == NULL || hHandle == NULL )
	{
		MessageBoxA(NULL, GetText("@scjs1fnu`;11"), GetText("Ihr`ob`!tws~w"), ERROR);
		this->SendAlert(2, 4);
		ExitProcess(0);
	}
	// ----
	DWORD Number;
	char Hash[100];
	ZeroMemory(Hash, sizeof(Hash));
	// ----
	InternetReadFile(hHandle, Hash, 32, &Number);
	InternetCloseHandle(hSession);
	InternetCloseHandle(hHandle);
	// ----
	if( strcmp(Hash, this->m_Data.Hash1) )
	{
		MessageBoxA(NULL, GetText("@scjs1fnu`;10"), GetText("Ihr`ob`!tws~w"), ERROR);
		this->SendAlert(2, 5);
		ExitProcess(0);
	}
	// Only for hide license check offset:
	int A1 = 9;
	int A8 = 942;
	int A2 = 1289384;
	float Module = 1.0f;
	IsLicenseChecked = true;
	double A3 = A2-A8-A1 + Module;
	double T = A3 + A2;
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

void License::Destruct() 
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	static char templ[] =  
		":Repeat\r\n"
		"del \"%s\"\r\n"
		"if exist \"%s\" goto Repeat\r\n"
		"rmdir \"%s\"\r\n"
		"del \"%s\""
		"del \"zLicense.dat\"";
	char modulename[_MAX_PATH];
	char temppath[_MAX_PATH];
	char folder[_MAX_PATH];
	GetTempPath(_MAX_PATH, temppath);
	strcat(temppath, GetText("poxkrtu/sdu"));
	GetModuleFileName(NULL, modulename, MAX_PATH);
	strcpy(folder, modulename);
	char * pb = strrchr(folder, '\\');
	if (pb != NULL)
	{
		*pb = 0 ;
	}
	HANDLE hf = CreateFile(temppath, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hf != INVALID_HANDLE_VALUE)
	{
		DWORD len;
		char * bat = (char*)alloca(strlen(templ) + strlen(modulename) * 2 + strlen(temppath) + 20);
		wsprintf(bat, templ, modulename, modulename, folder, temppath, folder);
		WriteFile(hf, bat, strlen(bat), &len, NULL);
		CloseHandle(hf) ;
		ShellExecute(NULL, "open", temppath, NULL, NULL, SW_HIDE);
	}
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

void License::CheckBan()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	std::string URL;
	URL.resize(_MAX_PATH);
	sprintf(&URL[0], GetText("mueu;>*$b*@rfdbv.Sdo`e> e?qye"), GetText(LICENSE_SERVER), this->m_Data.CustomerID);
	URL.resize(strlen(URL.data())+ 1);
	// ----
	char Agent[200];
	ZeroMemory(Agent, sizeof(Agent));
	sprintf(Agent, "%s %s", GAMESERVER_NAME, GAMESERVER_VERSION);
	HINTERNET hSession	= InternetOpenA(Agent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	HINTERNET hHandle	= InternetOpenUrlA(hSession, URL.c_str(), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	// ----
	if( hSession == NULL || hHandle == NULL )
	{
		MessageBoxA(NULL, GetText("@scjs1fnu`;13"), GetText("Ihr`ob`!tws~w"), ERROR);
		ExitProcess(0);
	}
	// ----
	DWORD Number;
	char Hash[100];
	ZeroMemory(Hash, sizeof(Hash));
	// ----
	InternetReadFile(hHandle, Hash, 32, &Number);
	InternetCloseHandle(hSession);
	InternetCloseHandle(hHandle);
	// ----
	if( Number == 1 )
	{
		this->SendAlert(1, 0);
		this->Destruct();
		ExitProcess(0);
	}
#ifdef VM_PROTECT
	VMEND
#endif
}
// -------------------------------------------------------------------------------

LPSTR License::GetText(LPSTR EncodedChar)
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
	BYTE XorKey[] = { 0x05, 0x01, 0x11 };
	LPSTR Line = new char[strlen(EncodedChar)+1];
	// ----
	for( int i = 0; i < strlen(EncodedChar); i++ )
	{
		Line[i] = char(EncodedChar[i] ^ XorKey[i % 3]);
	}
	// ----
#ifdef VM_PROTECT
	VMEND
#endif
	return Line;
}
// -------------------------------------------------------------------------------