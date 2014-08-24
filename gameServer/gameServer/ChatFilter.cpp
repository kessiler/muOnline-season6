#include "StdAfx.h"
#include "ChatFilter.h"
#include "..\include\readscript.h"
#include "logproc.h"
#include "GameMain.h"
#include "LogToFile.h"
// -------------------------------------------------------------------------

ChatFilter g_ChatFilter;
// -------------------------------------------------------------------------

void ChatFilter::Init()
{
	this->Counter	= 0;
	this->BanTime	= 0;
}
// -------------------------------------------------------------------------

void ChatFilter::Load()
{
	this->Init();
	this->ReadList(gDirPath.GetNewPath("Custom\\FilterList.txt"));
	this->ReadMain(gDirPath.GetNewPath("Custom\\FilterMain.ini"));
}
// -------------------------------------------------------------------------

void ChatFilter::ReadList(char * File)
{
	int Token;
	// ----
	SMDFile = fopen(File, "r");
	// ----
	if( SMDFile == 0 )
	{
		MsgBox(lMsg.Get(MSGGET(0, 112)), File);
		return;
	}
	// ----
	while(true) 
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		else
		{
			if( Token == NAME )
			{
				CString TextUTF(TokenString);
				int Size = this->StringToUTF8(TextUTF);
				memcpy(this->Filter[Counter].Text, TextUTF, Size);
				// ----
				this->Counter++;
			}
		}
	}
	// ----
	LogAddTD("[ChatFilter] [%d] Words loaded from list", this->Counter);
	// ----
	fclose(SMDFile);
}
// -------------------------------------------------------------------------

void ChatFilter::ReadMain(char * File)
{
	this->UsePenalty	= GetPrivateProfileInt("Common", "UsePenalty", 0, File);
	this->BanTime		= GetPrivateProfileInt("Common", "ChatBanTime", 0, File);
}
// -------------------------------------------------------------------------

int ChatFilter::StringToUTF8(CString & Text)
{
    LPCTSTR MultiByte	= Text;
    int Size			= MultiByteToWideChar(CP_UTF8, 0, MultiByte, -1, 0, 0);
    wchar_t * Output	= new wchar_t[Size];
	// ----
    MultiByteToWideChar(CP_UTF8, 0, MultiByte, -1, Output, Size);
    Text.Empty();
    Text = CString(Output);
	// ----
    delete [] Output;
	return Size;
}
// -------------------------------------------------------------------------

int ChatFilter::CheckText(char * Text)
{
	return 2;
	// ----
	if( (Text == NULL) || (strcmp(Text, "") == 0) )
	{
		return 2;
	}
	// ----
	LogAddC(2, "[DEBUG] Text: %s", Text);
	CString TextUTF(Text);
	int Size = StringToUTF8(TextUTF);
	LogAddC(2, "[DEBUG] UTF8: %s", TextUTF);
	char TextBuff[300];
	ZeroMemory(TextBuff, 299);
	memcpy(TextBuff, TextUTF, Size);
	LogAddC(2, "[DEBUG] TextBuff: %s", TextBuff);
	// ----
	int Result = 1;
	// ----
	for( int i = 0; i < this->Counter; i++ )
	{
		if( _tcsstr(CharLower(TextBuff), CharLower(this->Filter[i].Text)) != NULL )
		{
			LogAddC(2, "[DEBUG] Filter[i].Text: %s", this->Filter[i].Text);
			Result = 0;
			break;
		}
	}
	// ----
	return Result;
}
// -------------------------------------------------------------------------