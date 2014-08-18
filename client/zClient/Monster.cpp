#include "stdafx.h"
#include "Monster.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "Import.h"
// ----------------------------------------------------------------------------------------------

Monster gMonster;
// ----------------------------------------------------------------------------------------------

//1. 580FC0		-> Start loading monsters
//case 244:
//sub_61E060(1050);
//LODWORD(v73) = sub_57EEC0(a4, 1050, a2, a3, 0.0);
//sub_9CF3B0(LODWORD(v73) + 56);
//break;
//2. 61E060		-> Open model by resource index?
//sub_614D10(1052, "Data\\Npc\\", "SnowWizard", 1);
//3. 

#define pGetViewStruct			((lpViewObj(__cdecl*)(int a1, signed int a2, unsigned __int8 a3, unsigned __int8 a4, float a5)) 0x57EEC0)
#define	pOpenMonsterModel		((void(__cdecl*)(int MonsterID)) 0x61FDE0)

void __cdecl ShowMonster(signed int MonsterID, int a2, int a3, int a4)
{
	pShowMonster(MonsterID, a2, a3, a4);
}
// ----------------------------------------------------------------------------------------------

DWORD CraftMasterView_Buff;
lpViewObj CraftMasterView_lpView;

Naked(CraftMasterView)
{
	_asm
	{
		mov CraftMasterView_Buff, edx
	}
	// ----
#ifdef __NOVUS__
	if( pMapNumber == 3 
		&& *(BYTE*)(CraftMasterView_Buff + 45) == 177 
		&& *(BYTE*)(CraftMasterView_Buff + 46) == 104 )
	{
		*(short*)(CraftMasterView_Buff + 268)	= ITEM2(7, 45);
		*(BYTE*)(CraftMasterView_Buff + 270)	= 15;
		*(short*)(CraftMasterView_Buff + 304)	= ITEM2(8, 45);
		*(BYTE*)(CraftMasterView_Buff + 306)	= 15;
		*(short*)(CraftMasterView_Buff + 340)	= ITEM2(9, 45);
		*(BYTE*)(CraftMasterView_Buff + 342)	= 15;
		*(short*)(CraftMasterView_Buff + 376)	= ITEM2(10, 45);
		*(BYTE*)(CraftMasterView_Buff + 378)	= 15;
		*(short*)(CraftMasterView_Buff + 412)	= ITEM2(11, 45);
		*(BYTE*)(CraftMasterView_Buff + 414)	= 15;
		*(short*)(CraftMasterView_Buff + 520)	= ITEM2(12, 36);
		*(short*)(CraftMasterView_Buff + 448)	= (short)-1;
	}
#endif
	// ----
	if( pMapNumber == 79 
		&& *(BYTE*)(CraftMasterView_Buff + 45) == 108 
		&& *(BYTE*)(CraftMasterView_Buff + 46) == 135 )
	{
		*(short*)(CraftMasterView_Buff + 268)	= ITEM2(7, 3);
		*(BYTE*)(CraftMasterView_Buff + 270)	= 15;
		*(short*)(CraftMasterView_Buff + 304)	= ITEM2(8, 3);
		*(BYTE*)(CraftMasterView_Buff + 306)	= 15;
		*(short*)(CraftMasterView_Buff + 340)	= ITEM2(9, 3);
		*(BYTE*)(CraftMasterView_Buff + 342)	= 15;
		*(short*)(CraftMasterView_Buff + 376)	= ITEM2(10, 3);
		*(BYTE*)(CraftMasterView_Buff + 378)	= 15;
		*(short*)(CraftMasterView_Buff + 412)	= ITEM2(11, 3);
		*(BYTE*)(CraftMasterView_Buff + 414)	= 15;
		*(short*)(CraftMasterView_Buff + 520)	= ITEM2(12, 37);
		*(short*)(CraftMasterView_Buff + 448)	= (short)-1;
	}
	else
	{
		*(short*)(CraftMasterView_Buff + 268)	= ITEM2(7, 30);
		*(BYTE*)(CraftMasterView_Buff + 270)	= 9;
		*(short*)(CraftMasterView_Buff + 304)	= ITEM2(8, 30);
		*(BYTE*)(CraftMasterView_Buff + 306)	= 9;
		*(short*)(CraftMasterView_Buff + 340)	= ITEM2(9, 30);
		*(BYTE*)(CraftMasterView_Buff + 342)	= 9;
		*(short*)(CraftMasterView_Buff + 376)	= ITEM2(10, 30);
		*(BYTE*)(CraftMasterView_Buff + 378)	= 9;
		*(short*)(CraftMasterView_Buff + 412)	= ITEM2(11, 30);
		*(BYTE*)(CraftMasterView_Buff + 414)	= 9;
		*(short*)(CraftMasterView_Buff + 520)	= ITEM2(12, 1);
		*(short*)(CraftMasterView_Buff + 448)	= (short)-1;
	}
	// ----
	_asm
	{
		mov CraftMasterView_Buff, 0x00585390
		jmp CraftMasterView_Buff
	}
}
// ----------------------------------------------------------------------------------------------

void Monster::Load()
{
	ZeroMemory(this->lpNpcName, sizeof(this->lpNpcName));
	// ----
	SetRange((LPVOID)0x005852EC, 0xa4, ASM::NOP);
	SetOp((LPVOID)0x005852EC, (LPVOID)CraftMasterView, ASM::JMP);
	// ----
	/*SetOp((LPVOID)0x004E199D, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x00587049, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x00590CCC, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x005BBEE6, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x0064229D, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x00642D51, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x00643229, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x0065EA86, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x00911903, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x0091191D, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x00911937, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x00911951, (LPVOID)ShowMonster, ASM::CALL);
	SetOp((LPVOID)0x0091196B, (LPVOID)ShowMonster, ASM::CALL);*/
	// ----
	SetOp((LPVOID)0x0058101A, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x0058105B, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x0058109C, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x005810DD, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x0058111E, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x0058115F, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x005811A0, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x005811E1, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x00581222, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x00581263, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x005812A8, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x005812ED, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x0058132E, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x0058136F, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x005813B4, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x005813F9, (LPVOID)this->SetMainAttribute, ASM::CALL);
	SetOp((LPVOID)0x00586859, (LPVOID)this->SetMainAttribute, ASM::CALL);
}
// ----------------------------------------------------------------------------------------------

//00580BB0	->
void Monster::SetMainAttribute(int ObjectPointer, int MonsterID)
{
	lpViewObj Object	= &*(ObjectPreview*)ObjectPointer;
	int MapClass		= pMapNumber - 9;	//ebp-4
	// ----
	if( MapClass > 0 && MapClass <= 8 && MonsterID >= 84 && MonsterID <= 143 )
	{
		Object->Unknown130				= 0;
		*(float*)(ObjectPointer + 872)	+= (double)(MapClass / 3) * 0.05;
	}
	// ----
	if( !Object )
	{
		return;
	}
	// ----
	for( int i = 0; i < MAX_MONSTER; i++ )
	{
		gMonster.lpNpcName[i] = *(NpcNameList*)(oNpcName + sizeof(NpcNameList) * i);
		// ----
		if( MonsterID == gMonster.lpNpcName[i].ObjectID )
		{
#ifdef __NOVUS__
			if( MonsterID == 375 
				&& pMapNumber == 3 
				&& Object->RespawnPosX == 177 
				&& Object->RespawnPosY == 104 )
			{
				sprintf(Object->Name, "Craft Master");
				break;
			}
#endif
			// ----
			if( MonsterID == 375 
				&& pMapNumber == 79 
				&& Object->RespawnPosX == 108 
				&& Object->RespawnPosY == 135 )
			{
				sprintf(Object->Name, "Reset Master");
				break;
			}
			// ----
			memcpy(Object->Name, gMonster.lpNpcName[i].Name, sizeof(gMonster.lpNpcName[i].Name)-1);
			break;
		}
	}
	// ----
	Object->ID = MonsterID;
	*(BYTE*)(ObjectPointer + 798) = 0;
	Object->Unknown128 = *(DWORD*)0x879343C;
	// ----
	if( MonsterID == 200 )	//-> Soccerball
	{
		*(BYTE*)(ObjectPointer + 800) = emMonster;
	}
	else
	{
		if( MonsterID < 260 )
		{
			if( MonsterID <= 200 )
			{
				if( MonsterID < 150 )
				{
					if( MonsterID <= 110 )
					{
						if( MonsterID < 100 )
						{
							*(BYTE*)(ObjectPointer + 800)	= emMonster;
						}
						else
						{
							*(BYTE*)(ObjectPointer + 800)	= emNone;
						}
					}
					else
					{
						*(BYTE*)(ObjectPointer + 800) = emMonster;
					}
				}
				else
				{
					*(BYTE*)(ObjectPointer + 800)	= emMonster;
				}
			}
			else
			{
				*(BYTE*)(ObjectPointer + 800) = emNPC;
			}
		}
		else
		{
			*(BYTE*)(ObjectPointer + 800) = emMonster;
		}
	}
	// ----
	switch(MonsterID)
	{
		//-> 칸투르 npc
	case 367:	//-> Gateway Machine
	case 368:	//-> Elpis
		//-> 조화의 보석 관련 npc
	case 369:	//-> Osbourne
	case 370:	//-> Jerridon
		//-> 화이트엔젤 이벤트 아이템 지급 NPC 추가
	case 371:	//-> Leo the Helper
		//-> 카오스카드 담당
	case 375:	//-> Chaos Card Master
		//-> 공성전 신규 물약 NPC
	case 376:	//-> Pamela the Supplier
	case 377:	//-> Angela the Supplier
		//-> 설날 복주머니 이름
	case 379:	//-> Natasha the Firecracker Merchant
		//-> 신규 이벤트맵
	case 380:	//-> Stone Statue
	case 381:	//-> MU Allies General
	case 382:	//-> Illusion Sorcerer Elder
	case 383:	//-> Alliance Sacred Item Storage
	case 384:	//-> Illusion Castle Sacred Item Storage
	case 385:	//-> Mirage
		//-> 3차 체인지업
	case 406:	//-> Apostle Devin
	case 407:	//-> Werewolf Quarel
	case 408:	//-> Gatekeeper
		//-> 추석(2007)이벤트
	case 414:	//-> Helper Ellen
		//-> 엘베란드  NPC
	case 415:	//-> Silvia
	case 416:	//-> Rhea
	case 417:	//-> Marce
		//-> 벚꽃이벤트 npc 추가	
	case 450:	//-> Cherry Blossom Spirit
		//-> 소켓 시스템 NPC
	case 452:	//-> Seed Master
	case 453:	//-> Seed Researcher
		//-> 체험서버용 NPC 추가	
	case 464:	//-> Re-Initialization Helper
		//-> 크리스마스이벤트 NPC 추가(국내 적용 추가)
	case 465:	//-> Santa Claus
	case 467:	//-> Snowman
	case 468:	//-> Dasher
	case 469:	//-> Kermit
	case 470:	//-> Little Santa
	case 471:	//-> Cupid
	case 472:	//-> Prancer
	case 473:	//-> Donner
	case 474:	//-> Vixen
	case 475:	//-> Blitzen
		//-> 상용화 7주년 이벤트 NPC 추가
	case 478:	//-> Delgado
		//-> PK필드 몬스터 NPC 추가
	case 479:	//-> Doorkeeper Titus
		//-> 겜블 NPC 추가
	case 492:	//-> Moss
		//-> 일반 몬스터 NPC
	case 522:	//-> Jerint the Assistant
		//-> 도플갱어 몬스터 NPC
	case 540:	//-> Lugard
	case 541:	//-> Interim Reward Chest
	case 542:	//-> Final Reward Chest
		//-> 겐스 NPC
	case 543:	//-> Gens Duprian Steward
	case 544:	//-> Gens Vanert Steward
		//-> 로랜시장 NPC 추가
	case 545:	//-> Christine the General Goods Merchant
	case 546:	//-> Jeweler Raul
	case 547:	//-> Market Union Member Julia
		//-> 스켈레톤변신반지용 변신스켈레톤 추가
	case 566:	//-> Mercenary Guild Manager Tercia
	case 567:	//-> Priestess Veina
	case 568:	//-> Wandering Merchant Zyro
		//-> 칼루탄 NPC 추가 2010-06-30 홍승욱
	case 577:	//-> Leina the General Goods Merchant
	case 578:	//-> Weapons Merchant Bolo
		//-> 럭키아이템 NPC 추가 2010-09-01 이기수	
	case 579:	//-> David
		{
			*(BYTE*)(ObjectPointer + 800) = emNPC;
		}
		break;
	}
	// ----
	if( MonsterID >= 480 && MonsterID <= 491 )	//-> PK필드 몬스터
	{
		*(BYTE*)(ObjectPointer + 800) = emMonster;
	}
	// ----
	if( MonsterID == 451 )	//-> Cherry Blossom Tree
	{
		*(BYTE*)(ObjectPointer + 800) = emNone;
	}
}
// ----------------------------------------------------------------------------------------------