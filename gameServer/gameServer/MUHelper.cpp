#include "StdAfx.h"
#include "..\common\winutil.h"
#include "..\include\prodef.h"
#include "..\include\ReadScript.h"
#include "GameMain.h"
#include "user.h"
#include "logproc.h"
#include "DBSockMng.h"
#include "MUHelper.h"
// -------------------------------------------------------------------------------

MUHelper	g_MUHelper;
// -------------------------------------------------------------------------------

MUHelper::MUHelper()
{
	this->Init();
}
// -------------------------------------------------------------------------------

MUHelper::~MUHelper()
{
	// ----
}
// -------------------------------------------------------------------------------

void MUHelper::Init()
{
	ZeroMemory(this->StagePay, sizeof(this->StagePay));
	this->StageTime	= DEF_HELPER_STAGETIME;
	this->PayTime	= DEF_HELPER_PAYTIME;
}
// -------------------------------------------------------------------------------

void MUHelper::ReadData(char * File)
{
	int Token, Type = 0;
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
		if( Token == END )
		{
			break;
		}
		// ----
		Type = TokenNumber;
		// ----
		while(true)
		{
			if( Type == 0 )	//-> Main
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->StageTime	= TokenNumber;
				Token = GetToken();
				this->PayTime	= TokenNumber;
			}
			else if( Type == 1 ) //-> Stage pay
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				this->StagePay[0] = TokenNumber;
				Token = GetToken();
				this->StagePay[1] = TokenNumber;
				Token = GetToken();
				this->StagePay[2] = TokenNumber;
				Token = GetToken();
				this->StagePay[3] = TokenNumber;
				Token = GetToken();
				this->StagePay[4] = TokenNumber;
			}
		}
	}
	// ----
	LogAddTD("[MUHelper] Data loaded %d %d", this->StageTime, this->PayTime);
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

void MUHelper::Load()
{
	this->ReadData(gDirPath.GetNewPath("MUHelper.dat"));
}
// -------------------------------------------------------------------------------

void MUHelper::Manager(int UserIndex, MUHELPER_REQ_ACTION * lpRequest)
{
	LPOBJ lpUser = &gObj[UserIndex];
	// ----
	if( !gObjIsConnected(UserIndex) || lpUser->Connected != PLAYER_PLAYING )
	{
		return;
	}
	// ----
	switch(lpRequest->Mode)
	{
	case 0:
		{
			this->Start(lpUser);
		}
		break;
		// --
	case 1:
		{
			this->Close(lpUser);
		}
		break;
	}
}
// -------------------------------------------------------------------------------

void MUHelper::Start(LPOBJ lpUser)
{
	if( !gObjIsConnected(lpUser) || lpUser->Connected != PLAYER_PLAYING || !this->CheckMoney(lpUser) )
	{
		return;
	}
	// ----
	MUHELPER_ANS_DATA pAnswer = { 0 };
	PHeadSubSetB((LPBYTE)&pAnswer, 0xBF, 0x51, sizeof(pAnswer));
	// ----
	pAnswer.Mode = 0;
	// ----
	lpUser->m_MUHelperOn		= true;
	lpUser->m_MUHelperReadyPay	= false;
	lpUser->m_MUHelperStage		= 1;
	lpUser->m_MUHelperTick		= GetTickCount();
	// ----
	DataSend(lpUser->m_Index, (LPBYTE)&pAnswer, pAnswer.h.size);
	LogAddTD("[MUHelper] [%s] [%s] has been started", lpUser->AccountID, lpUser->Name);
}
// -------------------------------------------------------------------------------

void MUHelper::Work(LPOBJ lpUser)
{
	if( !lpUser->m_MUHelperOn )
	{
		return;
	}
	// ----
	DWORD CurrentTick	= GetTickCount();
	DWORD WorkTime		= (CurrentTick - lpUser->m_MUHelperTick) / 60000;
	// ----
	if( WorkTime > this->StageTime * MAX_HELPER_STAGE || !this->CheckMoney(lpUser) )
	{
		this->Close(lpUser);
		return;
	}
	// ----
	this->SetStage(lpUser, WorkTime);
	// ----
	if( WorkTime % this->PayTime == this->PayTime - 1 )
	{
		lpUser->m_MUHelperReadyPay = true;
	}
	// ----
	if( WorkTime % this->PayTime == 0 && WorkTime != 0 && lpUser->m_MUHelperReadyPay )
	{
		this->SendMoney(lpUser, WorkTime);
	}
}
// -------------------------------------------------------------------------------

void MUHelper::Close(LPOBJ lpUser)
{
	MUHELPER_ANS_DATA pAnswer = { 0 };
	PHeadSubSetB((LPBYTE)&pAnswer, 0xBF, 0x51, sizeof(pAnswer));
	// ----
	pAnswer.Mode = 1;
	// ----
	lpUser->m_MUHelperOn		= false;
	lpUser->m_MUHelperReadyPay	= false;
	lpUser->m_MUHelperStage		= 1;
	lpUser->m_MUHelperTick		= 0;
	// ----
	DataSend(lpUser->m_Index, (LPBYTE)&pAnswer, pAnswer.h.size);
	// ----
	LogAddTD("[MUHelper] [%s] [%s] has been closed", lpUser->AccountID, lpUser->Name);
}
// -------------------------------------------------------------------------------

void MUHelper::SetStage(LPOBJ lpUser, WORD WorkTime)
{
	if( lpUser->m_MUHelperStage < MAX_HELPER_STAGE )
	{
		lpUser->m_MUHelperStage = WorkTime / this->StageTime;	//-> 1 stage = 3.2h
		// ----
		if( lpUser->m_MUHelperStage < 1 )
		{
			lpUser->m_MUHelperStage = 1;
		}
	}
	else if( lpUser->m_MUHelperStage > MAX_HELPER_STAGE )
	{
		lpUser->m_MUHelperStage = MAX_HELPER_STAGE;
	}
}
// -------------------------------------------------------------------------------

int MUHelper::GetMoney(LPOBJ lpUser)
{
	int Level = lpUser->Level + lpUser->m_nMasterLevel;
	return Level * this->StagePay[lpUser->m_MUHelperStage-1];
}
// -------------------------------------------------------------------------------

bool MUHelper::CheckMoney(LPOBJ lpUser)
{
	MUHELPER_ANS_DATA pAnswer = { 0 };
	PHeadSubSetB((LPBYTE)&pAnswer, 0xBF, 0x51, sizeof(pAnswer));
	// ----
	pAnswer.Mode = 2;
	// ----
	if( lpUser->Money < this->GetMoney(lpUser) )
	{
		DataSend(lpUser->m_Index, (LPBYTE)&pAnswer, pAnswer.h.size);
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

void MUHelper::SendMoney(LPOBJ lpUser, WORD WorkTime)
{
	MUHELPER_ANS_DATA pAnswer = { 0 };
	PHeadSubSetB((LPBYTE)&pAnswer, 0xBF, 0x51, sizeof(pAnswer));
	// ----
	pAnswer.Money	= this->GetMoney(lpUser);
	pAnswer.Mode	= 0;
	pAnswer.Time	= WorkTime;
	// ----
	lpUser->Money -= pAnswer.Money;
	GCMoneySend(lpUser->m_Index, lpUser->Money);
	// ----
	lpUser->m_MUHelperReadyPay = false;
	DataSend(lpUser->m_Index, (LPBYTE)&pAnswer, pAnswer.h.size);
}
// -------------------------------------------------------------------------------

void MUHelper::SaveMacro(int UserIndex, MUHELPER_MACRO_CLIENT * lpRequest)
{
	LPOBJ lpUser = &gObj[UserIndex];
	// ----
	MUHELPER_MACRO_DS	pRequest = { 0 };
	PHeadSetW((LPBYTE)&pRequest, 0x18, sizeof(MUHELPER_MACRO_DS));
	// ----
	memcpy(pRequest.AccountID, lpUser->AccountID, MAX_IDSTRING);
	memcpy(pRequest.Name, lpUser->Name, MAX_IDSTRING);
	pRequest.aIndex = UserIndex;
	memcpy(pRequest.btMacroData, lpRequest->MacroBind, sizeof(pRequest.btMacroData));
	// ----
	cDBSMng.Send((char*)&pRequest, sizeof(MUHELPER_MACRO_DS));
}
// -------------------------------------------------------------------------------

void MUHelper::ReqMacro(int UserIndex)
{
	LPOBJ lpUser = &gObj[UserIndex];
	// ----
	MUHELPER_MACRO_DS	pRequest = { 0 };
	PHeadSetW((LPBYTE)&pRequest, 0x17, sizeof(MUHELPER_MACRO_DS));
	// ----
	memcpy(pRequest.AccountID, lpUser->AccountID, MAX_IDSTRING);
	memcpy(pRequest.Name, lpUser->Name, MAX_IDSTRING);
	pRequest.aIndex = UserIndex;
	// ----
	cDBSMng.Send((char*)&pRequest, sizeof(MUHELPER_MACRO_DS));
}
// -------------------------------------------------------------------------------

void MUHelper::SendMacro(MUHELPER_MACRO_DS * lpRequest)
{
	if( lpRequest->btResult || !gObjIsConnected(lpRequest->aIndex) )
	{
		return;
	}
	// ----
	MUHELPER_MACRO_CLIENT pAnswer = { 0 };
	PHeadSetW((LPBYTE)&pAnswer, 0xAE, sizeof(MUHELPER_MACRO_CLIENT));
	// ----
	memcpy(pAnswer.MacroBind, lpRequest->btMacroData, sizeof(lpRequest->btMacroData));
	// ----
	DataSend(lpRequest->aIndex, (LPBYTE)&pAnswer, sizeof(MUHELPER_MACRO_CLIENT));
}
// -------------------------------------------------------------------------------