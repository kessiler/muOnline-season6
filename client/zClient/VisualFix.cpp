#include "stdafx.h"
#include "VisualFix.h"
#include "TMemory.h"
#include "ProtocolDefine.h"
#include "Import.h"
#include "User.h"
#include "TDebugLog.h"

cVisualFix gVisualFix;

void cVisualFix::InitVisualFix()
{
	this->aIndex = 0;
	this->AttackHP = 0;
	this->AttackSD = 0;
	this->Life = 0;
	this->SD = 0;
	this->Mana = 0;
	this->AG = 0;
	this->MaxLife = 0;
	this->MaxSD = 0;
	this->MaxMana = 0;
	this->MaxAG = 0;
	this->UpPoint = 0;
	this->PlayerKill = TRUE;
	this->AttackRate = 0;
	this->DamageRate = 0;

	SetOp((LPVOID)0x006443F1, (LPVOID)this->DrawDamageHP, ASM::CALL);
	SetOp((LPVOID)0x00644465, (LPVOID)this->DrawDamageSD, ASM::CALL);

	SetOp((LPVOID)0x0080FDD8, (LPVOID)this->DrawHp, ASM::CALL);
	SetOp((LPVOID)0x0080FF6B, (LPVOID)this->DrawMp, ASM::CALL);
	SetOp((LPVOID)0x00810239, (LPVOID)this->DrawAg, ASM::CALL);
	SetOp((LPVOID)0x00810509, (LPVOID)this->DrawSd, ASM::CALL);

	SetOp((LPVOID)0x0080FE57, (LPVOID)this->DrawTabHP, ASM::CALL);
	SetOp((LPVOID)0x0080FFEA, (LPVOID)this->DrawTabMP, ASM::CALL);
	SetOp((LPVOID)0x0081059A, (LPVOID)this->DrawTabSD, ASM::CALL);
	SetOp((LPVOID)0x008102B8, (LPVOID)this->DrawTabAG, ASM::CALL);

	SetOp((LPVOID)0x00782C93, (LPVOID)this->InfoHp, ASM::CALL);
	SetOp((LPVOID)0x00782CC9, (LPVOID)this->InfoHp, ASM::CALL);
	SetOp((LPVOID)0x0078306C, (LPVOID)this->InfoMp, ASM::CALL);
	SetOp((LPVOID)0x007830A3, (LPVOID)this->InfoMp, ASM::CALL);

	SetOp((LPVOID)0x0077FC06, (LPVOID)this->InfoPoint, ASM::CALL);

	SetOp((LPVOID)0x0080FD06, (LPVOID)this->DrawCircle, ASM::CALL);	// - hp
	SetOp((LPVOID)0x0080FD9F, (LPVOID)this->DrawCircle, ASM::CALL);	// - hp
	SetOp((LPVOID)0x0080FF33, (LPVOID)this->DrawCircle, ASM::CALL);	// - mp
	SetOp((LPVOID)0x00810204, (LPVOID)this->DrawCircle, ASM::CALL);	// - ag
	SetOp((LPVOID)0x008104D4, (LPVOID)this->DrawCircle, ASM::CALL);	// - sd

	SetOp((LPVOID)0x00595BB0, (LPVOID)this->CalculateAttackRate, ASM::CALL);
	SetOp((LPVOID)0x00595B78, (LPVOID)this->CalculateDamageRate, ASM::CALL);

	SetRange((LPVOID)0x007801AC, 9, ASM::NOP);
	SetOp((LPVOID)0x007801AC, (LPVOID)this->GetAttackRate, ASM::CALL);
	SetByte((LPVOID)0x007801B1, 0x89);
	SetByte((LPVOID)(0x007801B1 + 1), 0xC6);

	SetRange((LPVOID)0x00780231, 10, ASM::NOP);
	SetOp((LPVOID)0x00780231, (LPVOID)this->GetAttackRate, ASM::CALL);
	SetByte((LPVOID)0x00780236, 0x89);
	SetByte((LPVOID)(0x00780236 + 1), 0xC6);

	SetRange((LPVOID)0x00780180, 10, ASM::NOP);
	SetOp((LPVOID)0x00780180, (LPVOID)this->GetDamageRate, ASM::CALL);
	SetByte((LPVOID)0x00780185, 0x89);
	SetByte((LPVOID)(0x00780185 + 1), 0xC6);

	SetRange((LPVOID)0x00780205, 10, ASM::NOP);
	SetOp((LPVOID)0x00780205, (LPVOID)this->GetDamageRate, ASM::CALL);
	SetByte((LPVOID)0x0078020A, 0x89);
	SetByte((LPVOID)(0x0078020A + 1), 0xC6);
}


void cVisualFix::RecvIndex(PMSG_JOINRESULT * Data)
{
	this->aIndex = MAKE_NUMBERW(Data->NumberH, Data->NumberL);
	this->PlayerKill = FALSE;
}

void cVisualFix::RecvDamage(PMSG_ATTACKRESULT * Data)
{
	this->AttackHP = Data->AttackHP;
	this->AttackSD = Data->AttackSD;
	short tIndex =  MAKE_NUMBERW(Data->NumberH, Data->NumberL);
	if (this->aIndex == tIndex)
	{
		this->Life	-= this->AttackHP;
		this->SD	-= this->AttackSD;

		if(this->Life < 0)
		{
			this->Life = 0;
		}
	}
}

void cVisualFix::RecvHPSD(PMSG_REFILL * Data)
{
	if(Data->IPos == 0xFF)
	{
		this->Life = Data->Life;
		this->SD = Data->Shield;
	}
	else if(Data->IPos == 0xFE)
	{
		this->MaxLife = Data->Life;
		this->MaxSD = Data->Shield;
	}
	if(this->Life > this->MaxLife)
	{
		this->Life = this->MaxLife;
	}
	if(this->PlayerKill == TRUE)
	{
		this->Life = 0;
		this->SD = 0;
	}
}

void cVisualFix::RecvMPAG(PMSG_MANASEND * Data)
{
	if(Data->IPos == 0xFF)
	{
		this->Mana = Data->Mana;
		this->AG = Data->BP;
	}
	else if(Data->IPos == 0xFE)
	{
		this->MaxMana = Data->Mana;
		this->MaxAG = Data->BP;
	}
	if(this->Mana > this->MaxMana)
	{
		this->Mana = this->MaxMana;
	}
}

void cVisualFix::RecvUpPoint(PMSG_CHARMAPJOINRESULT * Data)
{
	this->UpPoint = Data->UpPoint;
}

void cVisualFix::RecvUpLevel(PMSG_LEVELUP * Data)
{
	this->UpPoint = Data->UpPoint;
}

void cVisualFix::RecvDownPoint(PMSG_LVPOINTADDRESULT * Data)
{
	if(Data->ResultType != 0)
	{
		this->UpPoint--;
	}
}

void cVisualFix::RecvRespawn()
{
	this->PlayerKill = FALSE;
	this->Life = this->MaxLife;
	this->SD = this->MaxSD;
}

void cVisualFix::RecvKilledObject(PMSG_DIEPLAYER * Data)
{
	short tIndex =  MAKE_NUMBERW(Data->NumberH, Data->NumberL);
	if(this->aIndex == tIndex)
	{
		this->Life = 0;
		this->SD = 0;
		this->PlayerKill = TRUE;
	}
}

void cVisualFix::DrawDamageHP(int a1, int a2, int a3, float a4, char a5, char a6)
{
	pDrawDamage(a1, gVisualFix.AttackHP, a3, a4, a5, a6);
}

void cVisualFix::DrawDamageSD(int a1, int a2, int a3, float a4, char a5, char a6)
{
	pDrawDamage(a1, gVisualFix.AttackSD, a3, a4, a5, a6);
}

void cVisualFix::DrawHp(float a1, float a2, int Value, float a4)
{
	pDrawBar(a1, 450.0f, gVisualFix.Life, 1.0f);
}

void cVisualFix::DrawMp(float a1, float a2, int Value, float a4)
{
	pDrawBar(a1, 450.0f, gVisualFix.Mana, 1.0f);
}

void cVisualFix::DrawAg(float a1, float a2, int a3, float a4)
{
	pDrawBar(a1, a2, gVisualFix.AG, a4);
}

void cVisualFix::DrawSd(float a1, float a2, int a3, float a4)
{
	pDrawBar(a1, a2, gVisualFix.SD, a4);
}

void cVisualFix::DrawTabHP(signed int a1, signed int a2, LPCSTR lpMultiByteStr)
{
	sprintf(gVisualFix.LifeTab, "Life: %d/%d", gVisualFix.Life, gVisualFix.MaxLife);
	pDrawTab(a1,a2,gVisualFix.LifeTab);
}

void cVisualFix::DrawTabMP(signed int a1, signed int a2, LPCSTR lpMultiByteStr)
{
	sprintf(gVisualFix.ManaTab, "Mana: %d/%d", gVisualFix.Mana,gVisualFix.MaxMana);
	pDrawTab(a1,a2,gVisualFix.ManaTab);
}

void cVisualFix::DrawTabSD(signed int a1, signed int a2, LPCSTR lpMultiByteStr)
{
	sprintf(gVisualFix.SDTab, "SD: %d/%d", gVisualFix.SD,gVisualFix.MaxSD);
	pDrawTab(a1,a2,gVisualFix.SDTab);
}

void cVisualFix::DrawTabAG(signed int a1, signed int a2, LPCSTR lpMultiByteStr)
{
	sprintf(gVisualFix.AGTab, "AG: %d/%d", gVisualFix.AG,gVisualFix.MaxAG);
	pDrawTab(a1,a2,gVisualFix.AGTab);
}

void cVisualFix::InfoHp(int a1, const char *a2, ...)
{
	pInfo(a1,a2,gVisualFix.Life,gVisualFix.MaxLife);
}

void cVisualFix::InfoMp(int a1, const char *a2, ...)
{
	pInfo(a1,a2,gVisualFix.Mana,gVisualFix.MaxMana);
}

void cVisualFix::InfoPoint(int a1, const char * a2, ...)
{
	if( gVisualFix.UpPoint == 0 && gObjUser.lpPlayer->LevelPoint != 0 ) //Marlon add stat fix
	{
		gVisualFix.UpPoint = gObjUser.lpPlayer->LevelPoint;
	}

	pInfo(a1, a2, gVisualFix.UpPoint);
}

void cVisualFix::DrawCircle(DWORD ModelID, float Arg2, float Arg3, float Arg4, float Arg5, float Arg6, float Arg7, float Arg8, float Arg9, int Arg10, int Arg11, int Arg12)
{
    float Height    = 39.0f;
    float PosY		= 432.0f;
    float LifeFill  = (float)(gVisualFix.MaxLife - gVisualFix.Life) / (float)gVisualFix.MaxLife;
	float ManaFill  = (float)(gVisualFix.MaxMana - gVisualFix.Mana) / (float)gVisualFix.MaxMana;
	float AgFill	= (float)(gVisualFix.MaxAG - gVisualFix.AG) / (float)gVisualFix.MaxAG;
	float SdFill	= (float)(gVisualFix.MaxSD - gVisualFix.SD) / (float)gVisualFix.MaxSD;
    // ----
    if( ModelID == 0x7A41 || ModelID == 0x7A42 )
    {
        float StartY        = LifeFill * Height + PosY;
        float StartHeight   = Height - LifeFill * Height;
        float SourceY       = LifeFill * Height / 64.0;
        float SouceHeight   = (1.0 - LifeFill) * Height / 64.0;
        return pDrawCircle(ModelID, Arg2, StartY, Arg4, StartHeight, 0, SourceY, Arg8, SouceHeight, 1, 1, 0);
    }
    // ----
    if( ModelID == 0x7A40 )
    {
        float StartY        = ManaFill * Height + PosY;
        float StartHeight   = Height - ManaFill * Height;
        float SourceY       = ManaFill * Height / 64.0;
        float SouceHeight   = (1.0 - ManaFill) * Height / 64.0;
        return pDrawCircle(ModelID, Arg2, StartY, Arg4, StartHeight, 0, SourceY, Arg8, SouceHeight, 1, 1, 0);
    }
    // ----
	if( ModelID == 0x7A43 )
	{
        float StartY        = AgFill * Height + PosY;
        float StartHeight   = Height - AgFill * Height;
        float SourceY       = AgFill * Height / 64.0;
        float SouceHeight   = (1.0 - AgFill) * Height / 64.0;
        return pDrawCircle(ModelID, Arg2, StartY, Arg4, StartHeight, 0, SourceY, Arg8, SouceHeight, 1, 1, 0);
	}
	// ----
	if( ModelID == 0x7A44 )
	{
        float StartY        = SdFill * Height + PosY;
        float StartHeight   = Height - SdFill * Height;
        float SourceY       = SdFill * Height / 64.0;
        float SouceHeight   = (1.0 - SdFill) * Height / 64.0;
        return pDrawCircle(ModelID, Arg2, StartY, Arg4, StartHeight, 0, SourceY, Arg8, SouceHeight, 1, 1, 0);
	}
	// ----
    pDrawCircle(ModelID, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12);
}

void cVisualFix::CalculateAttackRate(lpCharObj lpUser, LPVOID EDX)
{
	float AttackRate	= 0.0f;
	int Dexterity		= lpUser->Dexterity + lpUser->AddDexterity;
	// ----
	switch(lpUser->Class & 7)
	{
	case DarkWizard:
	case DarkLord:
		AttackRate = (float)(lpUser->Level * 3 + Dexterity * 4);
		break;
	case DarkKnight:
		AttackRate = (float)(lpUser->Level * 3 + (Dexterity * 45) / 10);
		break;
	case Elf:
		AttackRate = (float)(lpUser->Level * 3 + (Dexterity * 6) / 10);
		break;
	case MagicGladiator:
	case Summoner:
		AttackRate = (float)(lpUser->Level * 3 + (Dexterity * 35) / 10);
		break;
	case Monk:
		AttackRate = (float)(lpUser->Level * 2.6f + (Dexterity * 36) / 10);
		break;
	}
	// ----
	gVisualFix.AttackRate	= (int)AttackRate;
	lpUser->AttackRate		= (WORD)gVisualFix.AttackRate;
}

void cVisualFix::CalculateDamageRate(lpCharObj lpUser, LPVOID EDX)
{
	WORD Strength	= lpUser->Strength + lpUser->AddStrength;
	WORD Dexterity	= lpUser->Dexterity + lpUser->AddDexterity;
	WORD Leadership	= lpUser->Leadership + lpUser->AddLeadership;
	// ----
	switch(lpUser->Class & 7)
	{
	case DarkLord:
		gVisualFix.DamageRate = (Leadership / 10) + (Strength / 6) + (5 * Dexterity / 2) + (5 * lpUser->Level);
		break;
	case Monk:
		gVisualFix.DamageRate = (Strength / 6) + (5 * Dexterity / 4) + (3 * lpUser->Level);
		break;
	default:
		gVisualFix.DamageRate = ((int)Strength >> 2) + (3 * Dexterity / 2) + (5 * lpUser->Level);
		break;
	}
	// ----
	pUpdateUserStat(pUserStat(), &gVisualFix.DamageRate, 177);
	gVisualFix.DamageRate	+= *(DWORD*)0x986C1C4;
	lpUser->DamageRate		= (WORD)gVisualFix.DamageRate;
}

int cVisualFix::GetAttackRate()
{
	return gVisualFix.AttackRate;
}

int cVisualFix::GetDamageRate()
{
	return gVisualFix.DamageRate;
}