#include "StdAfx.h"
#include "..\common\winutil.h"
#include "..\include\readscript.h"
#include "user.h"
#include "NewsBoard.h"
#include "GameMain.h"
#include "logproc.h"
// -------------------------------------------------------------------------------

NewsBoard g_NewsBoard;
// -------------------------------------------------------------------------------

NewsBoard::NewsBoard()
{
	this->Init();
}
// -------------------------------------------------------------------------------

NewsBoard::~NewsBoard()
{

}
// -------------------------------------------------------------------------------

void NewsBoard::Init()
{
	this->m_LoadedCount = 0;
	this->m_ShowOnLogin	= false;
	// ----
	for( int i = 0; i < MAX_NEWS_LIST; i++ )
	{
		this->m_Data[i].Title.Date[0]	= 0;
		this->m_Data[i].Title.Time[0]	= 0;
		this->m_Data[i].Title.Text[0]	= 0;
		this->m_Data[i].Text[0]			= 0;
	}
}
// -------------------------------------------------------------------------------

void NewsBoard::ReadListData(char * File)
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
		if( Token == END || !strcmp(TokenString, "end") )
		{
			break;
		}
		else
		{
			memcpy(this->m_Data[this->m_LoadedCount].Title.Date, TokenString, 11);
			Token = GetToken();
			memcpy(this->m_Data[this->m_LoadedCount].Title.Time, TokenString, 6);
			Token = GetToken();
			memcpy(this->m_Data[this->m_LoadedCount].Title.Text, TokenString, 69);
			Token = GetToken();
			memcpy(this->m_Data[this->m_LoadedCount].Text, TokenString, 699);
			// ----
			this->m_LoadedCount++;
		}
	}
	// ----
	LogAddTD("[NewsBoard] [%d] Item loaded from list", this->m_LoadedCount);
	fclose(SMDFile);	
}
// -------------------------------------------------------------------------------

void NewsBoard::ReadMainData(char * File)
{
	this->m_ShowOnLogin	= GetPrivateProfileIntA("Common", "ShowOnLogin", 0, File);
}
// -------------------------------------------------------------------------------

void NewsBoard::Load()
{
	this->Init();
	this->ReadListData(gDirPath.GetNewPath("Custom\\NewsList.txt"));
	this->ReadMainData(gDirPath.GetNewPath("Custom\\NewsMain.ini"));
}
// -------------------------------------------------------------------------------

void NewsBoard::OpenMain(LPOBJ lpUser)
{
	if( lpUser->Connected != PLAYER_PLAYING )
	{
		return;
	}
	// ----
	NEWS_ANS_TITLES pAnswer = { 0 };
	PHeadSetW((LPBYTE)&pAnswer, 0xFD, sizeof(NEWS_ANS_TITLES));
	// ----
	pAnswer.RealCount = this->m_LoadedCount;
	// ----
	for( int i = 0; i < this->m_LoadedCount; i++ )
	{

		strcpy(pAnswer.Titles[i].Date, this->m_Data[i].Title.Date);
		strcpy(pAnswer.Titles[i].Time, this->m_Data[i].Title.Time);
		strcpy(pAnswer.Titles[i].Text, this->m_Data[i].Title.Text);
	}
	// ----
	DataSend(lpUser->m_Index, (LPBYTE)&pAnswer, sizeof(NEWS_ANS_TITLES));
}
// -------------------------------------------------------------------------------

void NewsBoard::OpenItem(LPOBJ lpUser, NEWS_REQ_NEWS * Request)
{
	if( lpUser->Connected != PLAYER_PLAYING )
	{
		return;
	}
	// ----
	if( Request->ID < 0 || Request->ID >= MAX_NEWS_LIST )
	{
		return;
	}
	// ----
	NEWS_ANS_NEWS pAnswer = { 0 };
	PHeadSetW((LPBYTE)&pAnswer, 0xFE, sizeof(NEWS_ANS_TITLES));
	// ----
	pAnswer.ID = Request->ID;
	// ----
	strcpy(pAnswer.News.Title.Date, this->m_Data[Request->ID].Title.Date);
	strcpy(pAnswer.News.Title.Time, this->m_Data[Request->ID].Title.Time);
	strcpy(pAnswer.News.Title.Text, this->m_Data[Request->ID].Title.Text);
	strcpy(pAnswer.News.Text, this->m_Data[Request->ID].Text);
	// ----
	DataSend(lpUser->m_Index, (LPBYTE)&pAnswer, sizeof(NEWS_ANS_TITLES));
}
// -------------------------------------------------------------------------------