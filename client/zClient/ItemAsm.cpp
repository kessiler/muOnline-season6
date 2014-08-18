#include "stdafx.h"
#include "Item.h"
#include "ItemAsm.h"
#include "TMemory.h"
#include "Import.h"
// ----------------------------------------------------------------------------------------------

DWORD FixWingsExcellentGlow_Buff;
DWORD FixWingsLevelGlow_Buff;
DWORD FixWingsLevelGlow_Buff2;
DWORD FixWingsMoveList_Buff;
DWORD FixWingsJewelsInsert_Buff;
DWORD FixWingsJewelsInsert_Buff2;
DWORD WingsFixMoveKanturu_Buff;
DWORD AllowJewelsInsert_Buff;
DWORD AllowJewelsInsert_Buff2;
DWORD AllowJewelsInsert_Buff3;
DWORD ReToJewelVisualFlag_Buff;
DWORD AddShineGlow_Buff;
DWORD DropTitleColor_Buff;
DWORD DropTitleColor_Buff2;
DWORD	AddWingsEffect_Buff;
float	Color[3];
LPVOID	F1_This;
DWORD	F1_Arg1;
DWORD	F1_Arg11;
DWORD	F1_Arg2;
DWORD	F2_Arg5;
DWORD	F3_Arg3;
// ----------------------------------------------------------------------------------------------

Naked(FixWingsExcellentGlow)
{
	_asm
	{
		mov FixWingsExcellentGlow_Buff, ecx
		mov ecx, dword ptr ds:[FixWingsExcellentGlow_Buff + 0x30]
		mov FixWingsExcellentGlow_Buff, ecx
	}
	// ----
	if(		FixWingsExcellentGlow_Buff <= ITEM2(12, 50)
		||	gItem.IsCustomWings(FixWingsExcellentGlow_Buff, true) )
	{
		_asm
		{
			mov FixWingsExcellentGlow_Buff, 0x0060F325
			jmp FixWingsExcellentGlow_Buff
		}
	}
	// ----
	_asm
	{
		mov FixWingsExcellentGlow_Buff, 0x0060EE95
		jmp FixWingsExcellentGlow_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(FixWingsLevelGlow)
{
	_asm
	{
		mov FixWingsLevelGlow_Buff, ecx
		sub ecx, 0x1C9A
		mov FixWingsLevelGlow_Buff2, ecx
		mov dword ptr ss:[ebp-0x4ec], ecx
	}
	// ----
	if( gItem.IsCustomWings(FixWingsLevelGlow_Buff, true) )
	{
		_asm
		{
			mov dword ptr ss:[ebp-4], 0
		}
	}
	// ----
	if( FixWingsLevelGlow_Buff2 > 136 )
	{
		_asm
		{
			mov FixWingsLevelGlow_Buff, 0x0060A3B0
			jmp FixWingsLevelGlow_Buff
		}
	}
	else
	{
		_asm
		{
			mov FixWingsLevelGlow_Buff, 0x0060A06B
			jmp FixWingsLevelGlow_Buff
		}
	}
}
// ----------------------------------------------------------------------------------------------

Naked(FixWingsMoveList)
{
	_asm
	{
		mov FixWingsMoveList_Buff, eax
	}
	// ----
	if( FixWingsMoveList_Buff == 0x1887 || gItem.IsCustomWings(FixWingsMoveList_Buff, false) )
	{
		_asm
		{
			mov FixWingsMoveList_Buff, 0x00831996
			jmp FixWingsMoveList_Buff
		}
	}
	// ----
	_asm
	{
		mov FixWingsMoveList_Buff, 0x008319D5
		jmp FixWingsMoveList_Buff
	}
}
// ----------------------------------------------------------------------------------------------

DWORD FixWingsJewelsInsert_Buff3;
DWORD FixWingsJewelsInsert_Buff4;

Naked(FixWingsJewelsInsert)
{
	_asm
	{
		mov FixWingsJewelsInsert_Buff2, ecx

		mov FixWingsJewelsInsert_Buff3, edx
		mov ecx, dword ptr ss:[ebp-0x20]
		movsx edx, word ptr ds:[ecx]
		mov FixWingsJewelsInsert_Buff4, edx
		mov edx, FixWingsJewelsInsert_Buff3

		mov ecx, dword ptr ss:[ebp-0x3c]
		mov FixWingsJewelsInsert_Buff, ecx
		mov ecx, FixWingsJewelsInsert_Buff2
	}
	// ----
	if(		FixWingsJewelsInsert_Buff == 0x1887 
		||	gItem.IsCustomWings(FixWingsJewelsInsert_Buff, false) 
		||	gItem.IsCustomJewels(FixWingsJewelsInsert_Buff4, false) )
	{
		_asm
		{
			mov FixWingsJewelsInsert_Buff, 0x00838365
			jmp FixWingsJewelsInsert_Buff
		}
	}
	// ----
	_asm
	{
		mov FixWingsJewelsInsert_Buff, 0x00838361
		jmp FixWingsJewelsInsert_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(AddWingsEffect)
{
	_asm
	{
		mov AddWingsEffect_Buff, edx
		mov edx, dword ptr ss:[ebp-0x68]
		mov F1_This, edx
		lea edx, [ebp-0x58]
		mov F1_Arg1, edx
		lea edx, [ebp-0x18]
		mov F1_Arg11, edx
		mov edx, dword ptr ss:[ebp-0x6c]
		mov F1_Arg2, edx
		fldz
		fstp dword ptr ss:[esp]
		mov edx, dword ptr ss:[ebp-0x70]
		mov F2_Arg5, edx
		mov edx, AddWingsEffect_Buff
	}
	// ----
	if( gItem.IsCustomWings(AddWingsEffect_Buff, true) )
	{
		if( AddWingsEffect_Buff == ITEM2(12, 180) )
		{
			Color[0] = 0.4;
			Color[1] = 0.7;
			Color[2] = 1.0;
		}
		else if( AddWingsEffect_Buff == ITEM2(12, 181) )
		{
			Color[0] = 0.9;
			Color[1] = 0.0;
			Color[2] = 0.2;
		}
		// ----
		pAllowStaticEffect(F1_This, &F1_Arg11, F1_Arg2, 1, 0);
		pPlayStaticEffect(0x7D02, &F1_Arg11, 8.0, Color, F2_Arg5, 0.0, 0);
		// ----
		_asm
		{
			mov AddWingsEffect_Buff, 0x0056CA63
			jmp AddWingsEffect_Buff
		}
	}
	// ----
	if( AddWingsEffect_Buff > 0x0A9D )
	{
		_asm
		{
			mov AddWingsEffect_Buff, 0x005667BB
			jmp AddWingsEffect_Buff
		}
	}
	// ----
	_asm
	{
		mov AddWingsEffect_Buff, 0x00566707
		jmp AddWingsEffect_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(WingsFixMoveKanturu)
{
	_asm
	{
		mov WingsFixMoveKanturu_Buff, ecx
	}
	// ----
	if(		gItem.IsCustomWings(WingsFixMoveKanturu_Buff, false)
		||	WingsFixMoveKanturu_Buff == 0x1887 )
	{
		_asm
		{
			mov WingsFixMoveKanturu_Buff, 0x00876E5E
			jmp WingsFixMoveKanturu_Buff
		}
	}
	// ----
	_asm
	{
		mov WingsFixMoveKanturu_Buff, 0x00876E50
		jmp WingsFixMoveKanturu_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(AllowJewelsInsert)
{
	_asm
	{
		mov AllowJewelsInsert_Buff, edx
	}
	// ----
	if(		AllowJewelsInsert_Buff == ITEM(14, 14)
		||	gItem.IsCustomJewels(AllowJewelsInsert_Buff, false) )
	{
		_asm
		{
			mov AllowJewelsInsert_Buff, 0x008382C1
			jmp AllowJewelsInsert_Buff
		}
	}
	// ----
	_asm
	{
		mov AllowJewelsInsert_Buff, 0x0083826B
		jmp AllowJewelsInsert_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(ReToJewelVisualFlag)
{
	_asm
	{
		mov ReToJewelVisualFlag_Buff, eax
	}
	// ----
	if(		ReToJewelVisualFlag_Buff == ITEM(14, 13)
		||	gItem.IsCustomJewels(ReToJewelVisualFlag_Buff, false) )
	{
		_asm
		{
			mov ReToJewelVisualFlag_Buff, 0x007DBD5A
			jmp ReToJewelVisualFlag_Buff
		}
	}
	// ----
	_asm
	{
		mov ReToJewelVisualFlag_Buff, 0x007DBD4C
		jmp ReToJewelVisualFlag_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(AddShineGlow)
{
	_asm
	{
		mov AddShineGlow_Buff, eax
	}
	// ----
	if( gItem.IsCustomJewels(AddShineGlow_Buff, true) )
	{
		_asm
		{
			mov dword ptr ss:[ebp-4], 9
			mov AddShineGlow_Buff, 0x0060A3B0
			jmp AddShineGlow_Buff
		}
	}
	// ----
	if( AddShineGlow_Buff > 0x1E96 )
	{
		_asm
		{
			mov AddShineGlow_Buff, 0x0060A07F
			jmp AddShineGlow_Buff
		}
	}
	// ----
	_asm
	{
		mov AddShineGlow_Buff, 0x00609FF8
		jmp AddShineGlow_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(DropTitleColor)
{
	_asm
	{
		mov DropTitleColor_Buff2, eax
		mov eax, dword ptr ds:[ecx+0x30]
		mov DropTitleColor_Buff, eax
		mov eax, DropTitleColor_Buff2
	}
	// ----
	if(		DropTitleColor_Buff == ITEM2(14, 14)
		||	gItem.IsCustomJewels(DropTitleColor_Buff, true) )
	{
		_asm
		{
			mov DropTitleColor_Buff, 0x005F439E
			jmp DropTitleColor_Buff
		}
	}
	// ----
	_asm
	{
		mov DropTitleColor_Buff, 0x005F3F9E
		jmp DropTitleColor_Buff
	}
}
// ----------------------------------------------------------------------------------------------

void ItemAsmLoad()
{
	SetRange((LPVOID)0x005F3F91, 13, ASM::NOP);
	SetOp((LPVOID)0x005F3F91, DropTitleColor, ASM::JMP);
	// ----
	SetRange((LPVOID)0x00609FF2, 6, ASM::NOP);
	SetOp((LPVOID)0x00609FF2, AddShineGlow, ASM::JMP);
	// ----
	SetRange((LPVOID)0x007DBD45, 7, ASM::NOP);
	SetOp((LPVOID)0x007DBD45, ReToJewelVisualFlag, ASM::JMP);
	// ----
	SetRange((LPVOID)0x00838263, 8, ASM::NOP);
	SetOp((LPVOID)0x00838263, AllowJewelsInsert, ASM::JMP);
	// ----
#ifdef NEWWINGS
	SetRange((LPVOID)0x00876E48, 8, ASM::NOP);
	SetOp((LPVOID)0x00876E48, WingsFixMoveKanturu, ASM::JMP);
	// ----
	//SetRange((LPVOID)0x005666F7, 10, ASM::NOP);
	//SetOp((LPVOID)0x005666F7, AddWingsEffect, ASM::JMP);
	// ----
	SetRange((LPVOID)0x00838358, 9, ASM::NOP);
	SetOp((LPVOID)0x00838358, FixWingsJewelsInsert, ASM::JMP);
	// ----
	SetRange((LPVOID)0x0083198E, 8, ASM::NOP);
	SetOp((LPVOID)0x0083198E, FixWingsMoveList, ASM::JMP);
	// ----
	SetRange((LPVOID)0x0060EE88, 7, ASM::NOP);
	SetOp((LPVOID)0x0060EE88, FixWingsExcellentGlow, ASM::JMP);
	// ----
	SetRange((LPVOID)0x0060A04F, 28, ASM::NOP);
	SetOp((LPVOID)0x0060A04F, FixWingsLevelGlow, ASM::JMP);
#endif
}
// ----------------------------------------------------------------------------------------------