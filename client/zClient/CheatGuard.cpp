#include "stdafx.h"
#include "CheatGuard.h"
#include "Import.h"
#include "TMemory.h"
#include "User.h"
#include "Protocol.h"
#include "TrayMode.h"
// ----------------------------------------------------------------------------------------------

CheatGuard gCheatGuard;
// ----------------------------------------------------------------------------------------------

void CheatGuard::Load()
{
	ZeroMemory(this->XOR, sizeof(XOR));
	this->ChangeXORFilter();
}
// ----------------------------------------------------------------------------------------------

void CheatGuard::ChangeXORFilter()
{
	this->XOR[0]	= 0xf2;
	this->XOR[1]	= 0xa5;
	this->XOR[2]	= 0xb4;
	this->XOR[3]	= 0x85;
	this->XOR[4]	= 0xf5;
	this->XOR[5]	= 0xa7;
	this->XOR[6]	= 0xd9;
	this->XOR[7]	= 0x38;
	this->XOR[8]	= 0x92;
	this->XOR[9]	= 0x01;
	this->XOR[10]	= 0xee;
	this->XOR[11]	= 0x11;
	this->XOR[12]	= 0xd1;
	this->XOR[13]	= 0x96;
	this->XOR[14]	= 0xfe;
	this->XOR[15]	= 0xfa;
	this->XOR[16]	= 0xa5;
	this->XOR[17]	= 0xda;
	this->XOR[18]	= 0x2f;
	this->XOR[19]	= 0xd1;
	this->XOR[20]	= 0x44;
	this->XOR[21]	= 0xf6;
	this->XOR[22]	= 0x4c;
	this->XOR[23]	= 0x20;
	this->XOR[24]	= 0x91;
	this->XOR[25]	= 0x74;
	this->XOR[26]	= 0xdc;
	this->XOR[27]	= 0x1d;
	this->XOR[28]	= 0x37;
	this->XOR[29]	= 0xbe;
	this->XOR[30]	= 0xaf;
	this->XOR[31]	= 0x6b;
	// ----
	for( int i = 0; i < 32; i++ )
	{
		SetByte((PVOID)((oXORFilterStart + 4 * i) + 3), this->XOR[i]);
	}
}
// ----------------------------------------------------------------------------------------------

bool CheatGuard::IsCorrectFrame()
{
	if(		pFrameValue1 > MAX_FRAME_VALUE
		||	pFrameValue2 > MAX_FRAME_VALUE
		||	pFrameValue3 > MAX_FRAME_VALUE
		||	pFrameValue4 > MAX_FRAME_VALUE )
	{
		return false;
	}
	// ----
	return true;
}
// ----------------------------------------------------------------------------------------------

int CheatGuard::GetLargerFrame()
{
	if( pFrameValue1 > MAX_FRAME_VALUE )
	{
		return pFrameValue1;
	}
	else if( pFrameValue2 > MAX_FRAME_VALUE )
	{
		return pFrameValue2;
	}
	else if( pFrameValue3 > MAX_FRAME_VALUE )
	{
		return pFrameValue3;
	}
	else if( pFrameValue4 > MAX_FRAME_VALUE )
	{
		return pFrameValue4;
	}
	// ----
	return 0;
}
// ----------------------------------------------------------------------------------------------

void CheatGuard::Check(CHEATGUARD_REQ_CHECK * pRequest)
{
	gObjUser.Refresh();
	// ----
	CHEATGUARD_ANS_CHECK pResult;
	pResult.Head.set((LPBYTE)&pResult, 0x76, sizeof(pResult));
	ZeroMemory(pResult.Data, sizeof(pResult.Data));
	// ----
	for( int i = 0; i < MAX_CHECK_VARS; i++ )
	{
		pResult.Data[i].Status = true;
	}
	// ----
	if( gObjUser.lpPlayer->Level != pRequest->Level )
	{
		pResult.Data[emLevel].Status		= false;
		pResult.Data[emLevel].Value			= gObjUser.lpPlayer->Level;
	}
	// ----
	if( gObjUser.lpPlayer->LevelPoint != pRequest->LevelPoint )
	{
		pResult.Data[emLevelPoint].Status	= false;
		pResult.Data[emLevelPoint].Value	= gObjUser.lpPlayer->LevelPoint;
	}
	// ----
	if( gObjUser.lpPlayer->Strength > pRequest->Strength + MAX_STAT_LIMIT 
		|| pRequest->Strength > gObjUser.lpPlayer->Strength + MAX_STAT_LIMIT )
	{
		pResult.Data[emStrength].Status		= false;
		pResult.Data[emStrength].Value		= gObjUser.lpPlayer->Strength;
	}
	// ----
	if( gObjUser.lpPlayer->Dexterity > pRequest->Dexterity + MAX_STAT_LIMIT
		|| pRequest->Dexterity > gObjUser.lpPlayer->Dexterity + MAX_STAT_LIMIT )
	{
		pResult.Data[emDexterity].Status	= false;
		pResult.Data[emDexterity].Value		= gObjUser.lpPlayer->Dexterity;
	}
	// ----
	if( gObjUser.lpPlayer->Vitality > pRequest->Vitality + MAX_STAT_LIMIT
		|| pRequest->Vitality > gObjUser.lpPlayer->Vitality + MAX_STAT_LIMIT )
	{
		pResult.Data[emVitality].Status		= false;
		pResult.Data[emVitality].Value		= gObjUser.lpPlayer->Vitality;
	}
	// ----
	if( gObjUser.lpPlayer->Energy > pRequest->Energy + MAX_STAT_LIMIT
		|| pRequest->Energy > gObjUser.lpPlayer->Energy + MAX_STAT_LIMIT )
	{
		pResult.Data[emEnergy].Status		= false;
		pResult.Data[emEnergy].Value		= gObjUser.lpPlayer->Energy;
	}
	// ----
	if( gObjUser.lpPlayer->Leadership > pRequest->Leadership + MAX_STAT_LIMIT
		|| pRequest->Leadership > gObjUser.lpPlayer->Leadership + MAX_STAT_LIMIT )
	{
		pResult.Data[emLeadership].Status	= false;
		pResult.Data[emLeadership].Value	= gObjUser.lpPlayer->Leadership;
	}
	// ----
	if( (gObjUser.lpPlayer->AttackSpeed > pRequest->AttackSpeed + MAX_SPEED_LIMIT 
		|| pRequest->AttackSpeed > gObjUser.lpPlayer->AttackSpeed + MAX_SPEED_LIMIT) 
		&& (gObjUser.lpPlayer->AttackSpeed > pRequest->MagicSpeed + MAX_SPEED_LIMIT
		|| pRequest->MagicSpeed > gObjUser.lpPlayer->AttackSpeed + MAX_SPEED_LIMIT) )
	{
		pResult.Data[emAttackSpeed].Status	= false;
		pResult.Data[emAttackSpeed].Value	= gObjUser.lpPlayer->AttackSpeed;
		pResult.Data[emMagicSpeed].Value	= gObjUser.lpPlayer->MagicSpeed;
	}
	// ----
	if( !IsCorrectFrame() && !gTrayMode.InTray )
	{
		pResult.Data[emFrameValue].Status	= false;
		pResult.Data[emFrameValue].Value	= this->GetLargerFrame();
	}
	// ----
	for( int i = 0; i < MAX_CHECK_VARS; i++ )
	{
		if( !pResult.Data[i].Status )
		{
			gProtocol.DataSend((LPBYTE)&pResult, pResult.Head.size);
			return;
		}
	}
}
// ----------------------------------------------------------------------------------------------