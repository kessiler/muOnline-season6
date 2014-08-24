#include "StdAfx.h"
#include "PVPZone.h"
#include "GameMain.h"
#include "logproc.h"
#include "..\include\readscript.h"
// -------------------------------------------------------------------------------

PVPZone g_PVPZone;
// -------------------------------------------------------------------------------

PVPZone::PVPZone()
{
	this->Init();
}
// -------------------------------------------------------------------------------

PVPZone::~PVPZone()
{

}
// -------------------------------------------------------------------------------

void PVPZone::Init()
{
	this->m_LoadedCount = 0;
	ZeroMemory(this->m_Data, sizeof(this->m_Data));
}
// -------------------------------------------------------------------------------

void PVPZone::Load()
{
	this->Init();
	this->ReadData(gDirPath.GetNewPath("Custom\\PVPZone.txt"));
}
// -------------------------------------------------------------------------------

void PVPZone::ReadData(char * File)
{
	int Token;
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
			this->m_Data[this->m_LoadedCount].MapNumber	= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].X1	= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].Y1	= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].X2	= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].Y2	= TokenNumber;
			// ----
			this->m_LoadedCount++;
		}
	}
	// ----
	LogAddTD("[PVPZone] [%d] Zones loaded from list", this->m_LoadedCount);
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

bool PVPZone::IsPVPZone(LPOBJ lpUser)
{
	for( int i = 0; i < this->m_LoadedCount; i++ )
	{
		if( lpUser->MapNumber == this->m_Data[i].MapNumber )
		{
			//LogAddTD("[X]%d, [Y]%d, X1:%d, Y1:%d, X2:%d, Y2:%d,", lpUser->X,lpUser->Y,this->m_Data[i].X1,this->m_Data[i].Y1,this->m_Data[i].X2,this->m_Data[i].Y2);
			if(		(lpUser->X >= this->m_Data[i].X1 && lpUser->X <= this->m_Data[i].X2)// <= 131)	//Fix
				&&	(lpUser->Y >= this->m_Data[i].Y1 && lpUser->Y <= this->m_Data[i].Y2) )
			{
				return true;
			}
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------