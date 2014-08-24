#include "StdAfx.h"
#include "MixOption.h"
#include "logproc.h"
#include "..\include\ReadScript.h"
#include "..\common\winutil.h"
#include "GameMain.h"
// -------------------------------------------------------------------------------

#ifdef __CUSTOMS__
// -------------------------------------------------------------------------------

MixOption g_MixOption;
// -------------------------------------------------------------------------------

MixOption::MixOption()
{
	this->Init();
}
// -------------------------------------------------------------------------------

MixOption::~MixOption()
{
	// ----
}
// -------------------------------------------------------------------------------

void MixOption::Init()
{
	ZeroMemory(this->m_Data, sizeof(this->m_Data));
	this->m_LoadedCount = 0;
}
// -------------------------------------------------------------------------------

void MixOption::ReadData(char * File)
{
	int Token;
	this->Init();
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
			this->m_Data[this->m_LoadedCount].MixCode		= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].Money			= TokenNumber;
			Token = GetToken();
			this->m_Data[this->m_LoadedCount].SuccessRate	= TokenNumber;
			// ----
			this->m_LoadedCount++;
		}
	}
	// ----
	LogAddTD("[MixOption] [%d] Mixes loaded from list", this->m_LoadedCount);
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

void MixOption::Load()
{
	this->ReadData(gDirPath.GetNewPath("Custom\\MixOption.txt"));
}
// -------------------------------------------------------------------------------

int MixOption::GetMoney(LPOBJ lpUser, int MixCode)
{
	for( int i = 0; i < this->m_LoadedCount; i++ )
	{
		if( this->m_Data[i].MixCode == MixCode )
		{
			LogAddTD("[MixOption] [%d] Money + [%d]", 
				MixCode, this->m_Data[i].Money);
			return this->m_Data[i].Money;
		}
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

int MixOption::GetSuccessRate(LPOBJ lpUser, int MixCode)
{
	for( int i = 0; i < this->m_LoadedCount; i++ )
	{
		if( this->m_Data[i].MixCode == MixCode )
		{
			LogAddTD("[MixOption] [%d] SuccessRate + [%d]", 
				MixCode, this->m_Data[i].SuccessRate);
			return this->m_Data[i].SuccessRate;
		}
	}
	// ----
	return 0;
}
// -------------------------------------------------------------------------------

void MixOption::GetOption(LPOBJ lpUser, int MixCode)
{
	lpUser->m_AddMixMoney		= 0;
	lpUser->m_AddMixSuccessRate = 0;
	lpUser->m_AddMixMoney		= this->GetMoney(lpUser, MixCode);
	lpUser->m_AddMixSuccessRate = this->GetSuccessRate(lpUser, MixCode);
}
// -------------------------------------------------------------------------------

#endif
// -------------------------------------------------------------------------------