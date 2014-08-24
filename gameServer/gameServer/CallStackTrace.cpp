//GameServer 1.00.77 JPN - Completed
#include "stdafx.h"
#include "CallStackTrace.h"
#include "DBGHELP.H"

#pragma comment( lib, "dbghelp.lib" )

CCallStackTrace::CCallStackTrace(char * const szLogID,char * const szLogFileName, char * const szLogDirectoryName)
{
	if ( !strcmp(szLogID, ""))
	{
		MessageBox(NULL, "INSERT LOG-ID !!!", "ERROR", MB_OK);
		return;
	}

	strcpy(this->m_szLogID, szLogID);

	if ( !strcmp(szLogFileName, ""))
		strcpy(this->m_szLogFileName, "LOGDATA");
	else
		strcpy(this->m_szLogFileName, szLogFileName);

	if ( !strcmp(szLogDirectoryName, ""))
		strcpy(this->m_szLogDirectoryName, "Logs");
	else
		strcpy(this->m_szLogDirectoryName, szLogDirectoryName);

	CreateDirectory(szLogDirectoryName, NULL);
	InitializeCriticalSection(&this->m_critLogToFile);
}

CCallStackTrace::~CCallStackTrace()
{
	DeleteCriticalSection(&this->m_critLogToFile);
}

void CCallStackTrace::Output()
{
	if ( !strcmp(this->m_szLogID, ""))
	{
		MessageBox(NULL, "NO LOG-ID !!!", "ERROR", 0);
		return;
	}

	char szLogFileName[260];
	SYSTEMTIME strSystime;

	GetLocalTime(&strSystime);

	wsprintf(szLogFileName, "%s\\%s %04d-%02d-%02d.txt", this->m_szLogDirectoryName, this->m_szLogFileName, strSystime.wYear, strSystime.wMonth, strSystime.wDay);

	this->m_fLogFile = fopen(szLogFileName, "a+");

	if ( this->m_fLogFile == NULL )
	{
		return;
	}

	fprintf(this->m_fLogFile, "\nCallStack Trace Infomation (DATE:%04d-%02d-%02d %02d:%02d:%02d / ID:%s)\n", strSystime.wYear, strSystime.wMonth, strSystime.wDay, strSystime.wHour, strSystime.wMinute,	strSystime.wSecond, this->m_szLogID);
	fprintf(this->m_fLogFile, "\n*-----> CallStack List <-----*\n", this->m_szLogID);

	for ( int iC=0;iC<this->m_strCallStackLog.size(); iC++)
	{
		fputc(this->m_strCallStackLog[iC], this->m_fLogFile);
	}

	fclose(this->m_fLogFile);
}

DWORD CCallStackTrace::ConvertAddress(HANDLE hProcess, DWORD address, LPSTR output_buffer)
{
	char* current_pointer = output_buffer;
	IMAGEHLP_MODULE imagehlp_module;
	memset(&imagehlp_module, 0, sizeof(imagehlp_module));
	imagehlp_module.SizeOfStruct = sizeof(imagehlp_module);

	if ( SymGetModuleInfo(hProcess, address, &imagehlp_module) != FALSE )
	{
		char * image_name = strrchr(imagehlp_module.ImageName, '\\');

		if ( image_name != NULL )
			image_name += 1;
		else
			image_name = imagehlp_module.ImageName;

		current_pointer += sprintf(current_pointer, "%s : ", image_name);
	}
	else
	{
		current_pointer += sprintf(current_pointer, "<unknown module> : ");
	}

	DWORD displacement;
	char temp[0x11c];
	IMAGEHLP_SYMBOL * imagehlp_symbol = (IMAGEHLP_SYMBOL *)temp;
	memset(imagehlp_symbol, 0, sizeof(temp));
	imagehlp_symbol->SizeOfStruct = 24;
	imagehlp_symbol->Address = address;
	imagehlp_symbol->MaxNameLength = 0x104;

	if ( SymGetSymFromAddr(hProcess, address, &displacement, imagehlp_symbol) != FALSE )
	{
		current_pointer += sprintf(current_pointer, "%s() ", imagehlp_symbol->Name);

		IMAGEHLP_LINE imagehlp_line;
		memset(&imagehlp_line, 0, sizeof(imagehlp_line));
		imagehlp_line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		if ( SymGetLineFromAddr(hProcess, address, &displacement, &imagehlp_line) != FALSE )
		{
			current_pointer += sprintf(current_pointer, "// %s(%i)", imagehlp_line.FileName, imagehlp_line.LineNumber);
		}
	}
	else
	{
		current_pointer += sprintf(current_pointer, "<unknown symbol>");
	}
	
	current_pointer += sprintf(current_pointer, "\r\n");
	return current_pointer - output_buffer;
}

int CCallStackTrace::SaveCallStack(BOOL bWriteInFile)
{
	return this->TraceStack(bWriteInFile);
}

int CCallStackTrace::TraceStack(BOOL bWriteInFile)
{
	if ( strcmp(this->m_szLogID, "") == 0 )
	{
		MessageBox(NULL, "INSERT LOG-ID !!!", "ERROR", MB_OK);
		return -1;
	}

	this->m_strCallStackLog = "";

	EnterCriticalSection(&this->m_critLogToFile);

	__try
	{
		HANDLE hProcess = GetCurrentProcess();
		HANDLE hThread  = GetCurrentThread();
		DWORD option = SymGetOptions();
		option |= 0x10;
		option &= ~2;
		SymSetOptions(option|0x10);

		if (SymInitialize(hProcess, NULL, TRUE) == FALSE )
		{
			return -1;
		}

		DWORD addresses[0x2000]={0};
		UINT address_count = 0;
		CONTEXT context;
		memset(&context, 0, sizeof(context));
		context.ContextFlags = 0x10007;

		if ( GetThreadContext(GetCurrentThread(), &context) == FALSE )
		{
			return -1;
		}

		STACKFRAME stackframe;
		memset(&stackframe, 0, sizeof(stackframe));

		stackframe.AddrPC.Mode = AddrModeFlat;
		stackframe.AddrPC.Offset = context.Eip;

		stackframe.AddrStack.Offset = context.Esp;
		stackframe.AddrStack.Mode = AddrModeFlat;
		
		stackframe.AddrFrame.Offset = context.Ebp;
		stackframe.AddrFrame.Mode = AddrModeFlat;

		for ( UINT i=0; i<512; i++ )
		{
			if ( StackWalk( 332, hProcess, hThread, &stackframe, &context, NULL, SymFunctionTableAccess, (PGET_MODULE_BASE_ROUTINE)CCallStackTrace::GetModuleBase, NULL) == FALSE )
			{
				break;
			}

			if ( i > 1 )
			{
				 if ( stackframe.AddrPC.Offset != 0 )
				 {
					 addresses[address_count] = stackframe.AddrPC.Offset;
					 address_count++;
				 }
			}
		}

		for ( UINT addr_index =0 ;addr_index < address_count ; addr_index++ )
		{
			char symbol[0x504] ={0};
			DWORD symbol_size = this->ConvertAddress(hProcess, addresses[addr_index], symbol);
			this->m_strCallStackLog += "\t";
			this->m_strCallStackLog += symbol;
		}

		if ( bWriteInFile )
		{
			if ( this->m_strCallStackLog.empty() == false )
			{
				this->Output();
			}
		}
	}
	__finally
	{
		LeaveCriticalSection(&this->m_critLogToFile);
		return this->m_strCallStackLog.size();
	}
}

DWORD __stdcall CCallStackTrace::GetModuleBase(HANDLE hProcess, DWORD address)
{
	IMAGEHLP_MODULE imagehlp_module;
	MEMORY_BASIC_INFORMATION memory_basic_information;
	char module_file_name[260];
	DWORD module_file_name_length;
	HANDLE file_handle;
	char * image_name;

	imagehlp_module.SizeOfStruct = sizeof(imagehlp_module);

	if ( SymGetModuleInfo(hProcess, address, &imagehlp_module) == TRUE )
	{
		return imagehlp_module.BaseOfImage;
	}

	if ( VirtualQueryEx(hProcess, (LPCVOID)address, &memory_basic_information, sizeof(memory_basic_information)) == FALSE )
	{
		return 0;
	}

	module_file_name_length = GetModuleFileName((HMODULE)memory_basic_information.AllocationBase, module_file_name, sizeof(module_file_name));

	if ( module_file_name_length )
	{
		file_handle = CreateFile(module_file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	}
	else
	{
		file_handle = NULL;
	}

	image_name = NULL;

	if ( module_file_name_length > 0 )
	{
		image_name = module_file_name;
	}
	else
	{
		image_name = NULL;
	}

	SymLoadModule(hProcess, file_handle, image_name, 0, (DWORD)memory_basic_information.AllocationBase, 0);

	return (DWORD)memory_basic_information.AllocationBase;
}