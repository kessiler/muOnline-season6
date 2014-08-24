#include "StdAfx.h"
#include "CheatGuard.h"
#include "user.h"
#include "logproc.h"
#include "GameMain.h"
#include "..\include\ReadScript.h"
#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
#include "ConnectEx.h"
#endif
// -------------------------------------------------------------------------------

CheatGuard g_CheatGuard;
// -------------------------------------------------------------------------------

CheatGuard::CheatGuard()
{
	this->Init();
}
// -------------------------------------------------------------------------------

CheatGuard::~CheatGuard()
{

}
// -------------------------------------------------------------------------------

void CheatGuard::Init()
{
	ZeroMemory(this->XOR, sizeof(this->XOR));
	this->m_UseDisconnect		= true;
	this->m_PacketsPerSecond	= 100;
	this->m_SpeedHackCheck		= false;
}
// -------------------------------------------------------------------------------

void CheatGuard::ReadXOR(char * File)
{
	int Token;
	this->Init();
	// ----
	SMDFile		= fopen(File, "r");
	int Count	= 0;
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
			this->XOR[Count] = TokenNumber;
			Count++;
		}
	}
	// ----
	LogAddTD("[CheatGuard] [%d] Keys loaded", Count);
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

void CheatGuard::ReadMain(char * File)
{
	this->m_UseDisconnect		= GetPrivateProfileInt("Common", "UseDisconnect", true, File);
	this->m_PacketsPerSecond	= GetPrivateProfileInt("Common", "MaxPacketPerSecond", 120, File);
	this->m_SpeedHackCheck		= GetPrivateProfileInt("Common", "SpeedHackCheck", true, File);
}
// -------------------------------------------------------------------------------

void CheatGuard::Load()
{
	this->ReadXOR(gDirPath.GetNewPath("CheatGuard\\ProtocolFilter.dat"));
	this->ReadMain(gDirPath.GetNewPath("CheatGuard\\CheatGuard.ini"));
}
// -------------------------------------------------------------------------------

void CheatGuard::Disconnect(int aIndex)
{
	if( this->m_UseDisconnect )
	{
#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
		g_ConnectEx.SendClose(aIndex);
#endif
		CloseClient(aIndex);
	}
}
// -------------------------------------------------------------------------------

void CheatGuard::GenerateFileName(char * Buff)
{
	SYSTEMTIME t;
	GetLocalTime(&t);
	// ----
	sprintf(Buff, "LOG\\CheatGuard\\%02d_%02d_%02d.log", t.wDay, t.wMonth, t.wYear);
}
// -------------------------------------------------------------------------------

void CheatGuard::WriteLog(char * Text, ...)
{
	SYSTEMTIME t;
	GetLocalTime(&t);
	// ----
	char Buff[300];
	char Message[300];
	char FileName[40];
	this->GenerateFileName(FileName);
	FILE * Log = fopen(FileName, "a+");	//ok
	// ----
	va_list pArguments;
	va_start(pArguments, Text);
	vsprintf_s(Message, Text, pArguments);
	va_end(pArguments);
	// ----
	sprintf_s(Buff, "[%02d:%02d:%02d] %s\n", t.wHour, t.wMinute, t.wSecond, Message);
	// ----
	LogAddC(2, " [CheatGuard] %s", Message);
	fprintf(Log, "%s", Buff);
	// ----
	fclose(Log);
}
// -------------------------------------------------------------------------------

void CheatGuard::GetCheckResult(CHEATGUARD_ANS_CHECK * pResult, int aIndex)
{
	if( gObj[aIndex].CloseType == 1 )
	{
		return;
	}
	// ----
	char * MemberType[] = { "Level", "LevelPoint", "Strength", "Dexterity", 
		"Vitality", "Energy", "Leadership", "AttackSpeed", "MagicSpeed", "FrameValue"};
	// ----
	LPOBJ lpUser		= &gObj[aIndex];
	bool Disconnect		= false;
	int OriginalValue	= 0;
	// ----
	pResult->Data[emLevelPoint].Status	= true;	//Simple its not need
	pResult->Data[emLevel].Status		= true;	//Simple its not need
	pResult->Data[emEnergy].Status		= true;	//Simple its not need
	pResult->Data[emVitality].Status	= true;	//Simple its not need
	pResult->Data[emStrength].Status	= true;	//Simple its not need
	pResult->Data[emLeadership].Status	= true;	//Simple its not need
	// ----
	for( int i = 0; i < MAX_CHECK_VARS; i++ )
	{
		if( !pResult->Data[i].Status )
		{
			Disconnect = true;
			// ----
			switch(i)
			{
			case emLevel:
				{
					OriginalValue = lpUser->Level;
				}
				break;
				// --
			case emLevelPoint:
				{
					OriginalValue = lpUser->LevelUpPoint;
				}
				break;
				// --
			case emStrength:
				{
					OriginalValue = lpUser->Strength;
				}
				break;
				// --
			case emDexterity:
				{
					OriginalValue = lpUser->Dexterity;
				}
				break;
				// --
			case emVitality:
				{
					OriginalValue = lpUser->Vitality;
				}
				break;
				// --
			case emEnergy:
				{
					OriginalValue = lpUser->Energy;
				}
				break;
				// --
			case emLeadership:
				{
					OriginalValue = lpUser->Leadership;
				}
				break;
				// --
			case emAttackSpeed:
				{
					OriginalValue = lpUser->m_AttackSpeed;
				}
				break;
				// --
			case emMagicSpeed:
				{
					OriginalValue = lpUser->m_MagicSpeed;
				}
				break;
			}
			// ----
			this->WriteLog("[%s][%s] [%s] %d | %d", gObj[aIndex].AccountID, gObj[aIndex].Name, MemberType[i], OriginalValue, pResult->Data[i].Value);
		}
	}
	// ----
	if( Disconnect )
	{
		this->Disconnect(aIndex);
	}
}
// -------------------------------------------------------------------------------

void CheatGuard::SendCheckRequest(int aIndex)
{
	CHEATGUARD_REQ_CHECK pRequest;
	pRequest.Head.set((LPBYTE)&pRequest, 0x76, sizeof(pRequest));
	LPOBJ lpUser = &gObj[aIndex];
	// ----
	pRequest.Level			= lpUser->Level;
	pRequest.LevelPoint		= lpUser->LevelUpPoint;
	pRequest.Strength		= lpUser->Strength;
	pRequest.Dexterity		= lpUser->Dexterity;
	pRequest.Vitality		= lpUser->Vitality;
	pRequest.Energy			= lpUser->Energy;
	pRequest.Leadership		= lpUser->Leadership;
	pRequest.AttackSpeed	= lpUser->m_AttackSpeed;
	pRequest.MagicSpeed		= lpUser->m_MagicSpeed;
	// ----
	if( gObjIsGamePlaing(lpUser) )
	{
		DataSend(aIndex, (LPBYTE)&pRequest, sizeof(pRequest));
	}
}
// -------------------------------------------------------------------------------