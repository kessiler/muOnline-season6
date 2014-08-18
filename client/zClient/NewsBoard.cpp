#include "stdafx.h"
#include "NewsBoard.h"
#include "Interface.h"
#include "Protocol.h"
#include "TDebugLog.h"
// -------------------------------------------------------------------------------

NewsBoard gNewsBoard;
// -------------------------------------------------------------------------------

NewsBoard::NewsBoard()
{
	this->m_LoadedCount		= 0;
	this->m_LastRefreshTick = 0;
	this->m_LastRefreshMin	= 0;
	// ----
	for( int i = 0; i < MAX_NEWS_LIST; i++ )
	{
		this->m_Data[i].Title.Date[0]	= 0;
		this->m_Data[i].Title.Time[0]	= 0;
		this->m_Data[i].Title.Text[0]	= 0;
		this->m_Data[i].Text[0]			= 0;
		this->m_Data[i].LastRefreshTick	= 0;
		this->m_Data[i].LastRefreshMin	= 0;
	}
}
// -------------------------------------------------------------------------------

NewsBoard::~NewsBoard()
{

}
// -------------------------------------------------------------------------------

void NewsBoard::Init(BYTE ID)
{
	ZeroMemory(this->m_Data[ID].Title.Date, sizeof(this->m_Data[ID].Title.Date)-1);
	ZeroMemory(this->m_Data[ID].Title.Time, sizeof(this->m_Data[ID].Title.Time)-1);
	ZeroMemory(this->m_Data[ID].Title.Text, sizeof(this->m_Data[ID].Title.Text)-1);
	ZeroMemory(this->m_Data[ID].Text, sizeof(this->m_Data[ID].Text)-1);
}
// -------------------------------------------------------------------------------

void NewsBoard::ReqOpenMain()
{
	DWORD CurrentTick		= GetTickCount();
	this->m_LastRefreshMin	= (CurrentTick - this->m_LastRefreshTick) / 60000;
	// ----
	if( this->m_LastRefreshMin <= MIN_NEWS_REFRESH )
	{
		gInterface.OpenNewsWindow();
		return;
	}
	// ----
	NEWS_REQ_NEWS pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFD, sizeof(pRequest));
	gProtocol.DataSend((LPBYTE)&pRequest, pRequest.h.size);
}
// -------------------------------------------------------------------------------

void NewsBoard::OpenMain(NEWS_ANS_TITLES * Answer)
{
	if( Answer->RealCount < 0 || Answer->RealCount > MAX_NEWS_LIST )
	{
		return;
	}
	// ----
	this->m_LastRefreshTick = GetTickCount();
	this->m_LastRefreshMin	= 0;
	this->m_LoadedCount		= Answer->RealCount;
	// ----
	for( int i = 0; i < Answer->RealCount; i++ )
	{
		ZeroMemory(this->m_Data[i].Title.Date, sizeof(this->m_Data[i].Title.Date)-1);
		ZeroMemory(this->m_Data[i].Title.Time, sizeof(this->m_Data[i].Title.Time)-1);
		ZeroMemory(this->m_Data[i].Title.Text, sizeof(this->m_Data[i].Title.Text)-1);
		// ----
		strcpy(this->m_Data[i].Title.Date, Answer->Titles[i].Date);
		strcpy(this->m_Data[i].Title.Time, Answer->Titles[i].Time);
		strcpy(this->m_Data[i].Title.Text, Answer->Titles[i].Text);
	}
	// ----
	gInterface.OpenNewsWindow();
}
// -------------------------------------------------------------------------------

void NewsBoard::ReqOpenItem(BYTE ID)
{
	DWORD CurrentTick				= GetTickCount();
	this->m_Data[ID].LastRefreshMin	= (CurrentTick - this->m_Data[ID].LastRefreshTick) / 60000;
	// ----
	if( this->m_Data[ID].LastRefreshMin <= MIN_NEWS_REFRESH_PAGE )
	{
		return;
	}
	NEWS_REQ_NEWS pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFE, sizeof(pRequest));
	pRequest.ID = ID;
	gProtocol.DataSend((LPBYTE)&pRequest, pRequest.h.size);
}
// -------------------------------------------------------------------------------

void NewsBoard::OpenItem(NEWS_ANS_NEWS * Answer)
{
	if( Answer->ID < 0 || Answer->ID > MAX_NEWS_LIST )
	{
		return;
	}
	// ----
	this->Init(Answer->ID);
	// ----
	strcpy(this->m_Data[Answer->ID].Title.Date, Answer->News.Title.Date);
	strcpy(this->m_Data[Answer->ID].Title.Time, Answer->News.Title.Time);
	strcpy(this->m_Data[Answer->ID].Title.Text, Answer->News.Title.Text);
	strcpy(this->m_Data[Answer->ID].Text, Answer->News.Text);
	this->m_Data[Answer->ID].LastRefreshTick	= GetTickCount();
	this->m_Data[Answer->ID].LastRefreshMin		= 0;
}
// -------------------------------------------------------------------------------