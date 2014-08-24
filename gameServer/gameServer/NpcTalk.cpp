#include "stdafx.h"
#include "user.h"
#include "NpcTalk.h"
#include "gObjMonster.h"
#include "..\common\winutil.h"
#include "protocol.h"
#include "GameMain.h"
#include "BloodCastle.h"
#include "gObjMonster.h"
#include "DevilSquare.h"
#include "QuestInfo.h"
#include "CastleSiegeSync.h"
#include "KanturuEntranceNPC.h"
#include "JewelOfHarmonySystem.h"
#include "Gamemain.h"
#include "Event.h"
#include "IllusionTempleEvent.h"
#include "BuffEffectSlot.h"
#include "CashShop.h"
#include "LuaNPC.h"

#ifdef NPVP
#include "NewPVP.h"
#endif

#ifdef GENS
#include "GensSystem.h"
#endif

#ifdef DP
#include "Doppelganger.h"
#endif

#ifdef WZQUEST
#include "QuestExpProgMng.h"
#endif

#ifdef __CUSTOMS__
#include "ResetSystem.h"
#include "PKClear.h"
#ifdef QUESTSYSTEM
#include "QuestSystem.h"
#endif
#endif

#ifdef __NOVUS__
#include "CraftSystem.h"
#endif
// -------------------------------------------------------------------------------

//1.00.92	-> 5AA0A0 -> 100%
//1.01.00	-> 613DB0 -> 100%
bool NpcTalk(LPOBJ lpNpc, LPOBJ lpObj) // -> Complete
{
	int npcnum = lpNpc->Class;
	// ----
	if( npcnum < 0 )
	{
		return false;
	}
	// ----
	if( lpObj->m_IfState.use > 0 )
	{
		return true;
	}
	// ----
#ifdef QUESTSYSTEM
	if( g_QuestSystem.Dialog(lpObj, lpNpc) )
	{
		return true;
	}
#endif
	// ----
#ifdef __CUSTOMS__
	if( g_ResetSystem.Dialog(lpObj, lpNpc) )
	{
		return true;
	}
	// ----
	if( g_PKClear.Dialog(lpObj, lpNpc) )
	{
		return true;
	}
#endif
	// ----
#ifdef __NOVUS__
	if( g_CraftSystem.Dialog(lpObj, lpNpc) )
	{
		return true;
	}
#endif
	// ----
	if( npcnum == 229 ) //-> Marlon
	{
		gQeustNpcTeleport.TalkRefAdd();
		lpObj->TargetShopNumber = npcnum;
	}
	// ----
	g_QuestExpProgMng.ChkUserQuestType(263, lpObj, lpNpc, 0);
	// ----
	if( npcnum == 568 ) //-> Wandering Merchant Zyro
	{
		gQeustNpcTeleport.TalkRefAdd();
		lpObj->TargetShopNumber = npcnum;
		// ----
		if( Npc_Dialog(lpNpc, lpObj) )
		{
			return true;
		}
	}
	// ----
	if( NpcQuestCheck(lpNpc, lpObj) != FALSE )
	{
		return true;
	}
	// ----
	if( npcnum > 367 )
	{
		switch( npcnum )
		{
		case 566: //-> Mercenary Guild Manager Tercia
		case 567: //-> Priestess Veina
			{
				if( Npc_Dialog(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 368: //-> Elpis
		case 369: //-> Osbourne
		case 370: //-> Jerridon
			{
				if( g_kJewelOfHarmonySystem.NpcJewelOfHarmony(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 375: //-> Chaos Card Master
			{
				if( NpcChaosCardMaster(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 376: //-> Pamela the Supplier
			{
				if( NpcQuartermasterPamella(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 377: //-> Angela the Supplier
			{
				if( NpcQuartermasterAngela(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 379: //-> Natasha the Firecracker Merchant
			{
				if( NPcFireCrackerStore(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 385: //-> Mirage
			{
				if( NpcPlatformMirage(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 380: //-> Stone Statue
			{
				if( NpcIllusionHolyRelicsStatue(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 383: //-> Alliance Sacred Item Storage
			{
				if( NpcHolyRelicsBoxAllied(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 384: //-> Illusion Castle Sacred Item Storage
			{
				if( NpcHolyRelicsBoxIllusion(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 415: //-> Silvia
			{
				if( NpcSilviaTalk(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 416: //-> Rhea
			{
				if( NpcRheaTalk(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 417: //-> Marce
			{
				if( NpcMarceTalk(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 522: //-> Jerint the Assistant
			{
				if( Npc_EventDungeonTalk(lpNpc, lpObj) ) //-> 617AA0
				{
					return true;
				}
			}
			break;
			// --
		case 450: //-> Cherry Blossom Spirit
			{
				if( NpcCherryTalk(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 452: //-> Seed Master
			{
				if( NpcSeedMaster(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 453: //-> Seed Researcher
			{
				if( NpcSeedResearcher(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 478: //-> Delgado
			{
				if( NpcLuckyCoinDelgado(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 465: //-> Santa Claus
			{
				if( Npc_SantaTalk(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 467: //-> Snowman
			{
				if( Npc_SnowManTalk(lpNpc, lpObj) )
				{
					return true;
				}
			}
			break;
			// --
		case 468: //-> Dasher
			{
				if( Npc_LittleSantaTalk(lpNpc, lpObj, 94) )
				{
					return true;
				}
			}
			break;
			// --
		case 469: //-> Kermit
			{
				if( Npc_LittleSantaTalk(lpNpc, lpObj, 95) )
				{
					return true;
				}
			}
			break;
			// --
		case 470: //-> Little Santa
			{
				if( Npc_LittleSantaTalk(lpNpc, lpObj, 0) )
				{
					return true;
				}
			}
			break;
			// --
		case 471: //-> Cupid
			{
				if( Npc_LittleSantaTalk(lpNpc, lpObj, 1) )
				{
					return true;
				}
			}
			break;
			// --
		case 472: //-> Prancer
			{
				if( Npc_LittleSantaTalk(lpNpc, lpObj, 92) )
				{
					return true;
				}
			}
			break;
			// --
		case 473: //-> Donner
			{
				if( Npc_LittleSantaTalk(lpNpc, lpObj, 93) )
				{
					return true;
				}
			}
			break;
			// --
		case 474: //-> Vixen
			{
				if( Npc_LittleSantaTalk(lpNpc, lpObj, 96) )
				{
					return true;
				}
			}
			break;
			// --
		case 475: //-> Blitzen
			{
				if( Npc_LittleSantaTalk(lpNpc, lpObj, 97) )
				{
					return true;
				}
			}
			break;
			// --
		case 479: //-> Doorkeeper Titus
			{
				/*if ( lpObj->m_IfState._bf0 & 3 )
				{
				return true
				}
				else*/
				PMSG_TALKRESULT pResult;
				// ----
#ifdef NPVP
				g_NewPVP.SendChannelList(*lpNpc, *lpObj);
#endif
				// ----
				pResult.h.c				= 0xC3;
				pResult.h.headcode		= 0x30;
				pResult.h.size			= sizeof(pResult);
				pResult.result			= 33;
				// ----
				DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
				// ----
				lpObj->m_IfState.type	= 20;
				lpObj->m_IfState.use	= 1;
				// ----
				return true;
			}
			break;
			// --
		case 540: //-> Lugard
			{
				if( Npc_LugardTalk(lpNpc, lpObj) ) //-> 617B10
				{
					return true;
				}
			}
			break;
			// --
		case 541: //-> Interim Reward Chest
			{
				if( Npc_MiddleTreasureTalk(lpNpc, lpObj) ) //-> 617B80
				{
					return true;
				}
			}
			break;
			// --
		case 542: //-> Final Reward Chest
			{
				if( Npc_LastTreasureTalk(lpNpc, lpObj) ) //-> 617BD0
				{
					return true;
				}
			}
			break;
			// --
		case 543: //-> Gens Duprian Steward
		case 544: //-> Gens Vanert Steward
			{
				if( Npc_Dialog(lpNpc, lpObj) ) //-> 617C20
				{
					return true;
				}
			}
			break;
			// --
		case 545: //-> Christine the General Goods Merchant
			{
				if( NpcKristenTalk(lpNpc, lpObj) ) //-> 617E30
				{
					return true;
				}
			}
			break;
			// --
		case 546: //-> Jeweler Raul
			{
				if( NpcRaulTalk(lpNpc, lpObj) ) //-> 617E70
				{
					return true;
				}
			}
			break;
			// --
		case 547: //-> Market Union Member Julia
			{
				if( NpcJuliaTalk(lpNpc, lpObj) ) //-> 617EB0
				{
					return true;
				}
			}
			break;
			// --
		case 577: //-> Leina the General Goods Merchant
			{
				if( NpcRehina(lpNpc, lpObj) ) //-> 617F30
				{
					return true;
				}
			}
			break;
			// --
		case 578: //-> Weapons Merchant Bolo
			if( NpcVolo(lpNpc, lpObj) ) //-> 617F70
			{
				return true;
			}
			break;
			// --
		case 579: //-> David
			{
				if( NpcDabide(lpNpc, lpObj) ) //-> 617FB0
				{
					return true;
				}
			}
			break;
			// --
		default:
			{
				return false;
			}
		}
	}
	else
	{
		if( npcnum == 367 ) //-> Gateway Machine
		{
			if( NpcMainatenceMachine(lpNpc, lpObj) )
			{
				return true;
			}
		}
		else
		{
			switch( npcnum )
			{
			case 254: //-> Pasi the Mage
				{
					if( NpcFasi(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 241: //-> Guild Master
				{
					if( NpcGuildMasterTalk(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 239: //-> Arena Guard
				{
					if( NpcBattleAnnouncer(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 249: //-> Guard
				{
					if( NpcRolensiaGuard(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 238: //-> Chaos Goblin
				{
					if( NpcChaosGoblelin(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 237: //-> Charon
				{
					if ( NpcRusipher(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 253: //-> Potion Girl Amy
				{
					if( NpcPosionGirl(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 245: //-> Wizard Izabel
				{
					if( NpcDeviasWizard(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 247: //-> Guard
				{
					if( NpcDeviasGuard(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 240: //-> Safety Guardian (Vault Keeper)
				{
					if( NpcWarehouse(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 226: //-> Trainer
				{
					if( NpcDarkSpiritTrainer(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 242: //-> Elf Lala
				{
					if( NpcNoriaRara(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 244: //-> Caren the Barmaid
				{
					if( NpcDeviasMadam(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 236: //-> Golden Archer
				{
					if( NpcEventChipNPC(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 234: //->
				{
					if( NpcServerDivision(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 232: //-> Archangel
				{
					if( NpcAngelKing(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 233: //-> Messenger of Archangel
				{
					if( NpcAngelMessanger(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 255: //-> Lumen the Barmaid
				{
					if( NpcRolensiaMadam(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 250: //-> Wandering Merchant
				{
					if( NpcRoadMerchant(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 246: //-> Weapons Merchant Zienna
				{
					if( NpcDeviasWeapon(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 251: //-> Hanzo the Blacksmith
				{
					if( NpcRorensiaSmith(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 243: //-> Eo the Craftsman
				{
					if( NpcNoriJangIn(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 256: //-> Lahap
				{
					if( NpcJewelMixDealer(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 221: //-> Slingshot attack
				{
					if( NpcSiegeMachine_Attack(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 222: //-> Slingshot defense
				{
					if( NpcSiegeMachine_Defense(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 223: //-> Senior
				{
					if( NpcElderCircle(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 224: //-> Guardsman
				{
					if( NpcCastleGuard(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 219: //-> Castle Gate Switch
				{
					if( NpcCastleGateLever(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 216: //-> Crown
				{
					if( NpcCastleCrown(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 217: //-> Crown Switch 2
			case 218: //-> Crown Switch 1
				{
					if( NpcCastleSwitch(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 220:
				{
					if( NpcCastleHuntZoneGuard(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			case 257: //-> Shadow Phantom Soldier
				{
					if( Npc_Dialog(lpNpc, lpObj) ) //-> 617C20
					{
						return true;
					}
				}
				break;
				// --
			case 205:
				if ( NpcCrywolfAltar( lpNpc, lpObj ) == TRUE ) //ok
				{
					return TRUE;
				}
				break;
				// --
			case 206:
				if ( NpcCrywolfAltar( lpNpc, lpObj ) == TRUE ) //ok
				{
					return TRUE;
				}
				break;
				// --
			case 207:
				if ( NpcCrywolfAltar( lpNpc, lpObj ) == TRUE ) //ok
				{
					return TRUE;
				}
				break;
				// --
			case 208:
				if ( NpcCrywolfAltar( lpNpc, lpObj ) == TRUE ) //ok
				{
					return TRUE;
				}
				break;
				// --
			case 209:
				if ( NpcCrywolfAltar( lpNpc, lpObj ) == TRUE ) //ok
				{
					return TRUE;
				}
				break;
			// --
			case 259: //-> Oracle Layla
				{
					if( NpcReira(lpNpc, lpObj) )
					{
						return true;
					}
				}
				break;
				// --
			default:
				{
					return false;
				}
				break;
			}
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AAF50 -> 100%
//1.01.00	-> 614F60 -> 100%
bool NpcMainatenceMachine(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	g_KanturuEntranceNPC.NotifyEntranceInfo(lpObj->m_Index);
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AAF80 -> 100%
//1.01.00	-> 614F90 -> 100%
bool NpcReira(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AAFA0 -> 100%
//1.01.00	-> 614FB0 -> 100%
bool NpcBattleAnnouncer(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AAFC0 -> 99% (Need change 1 name)
//1.01.00	-> 614FD0 -> 99% (Need change 1 name)
bool NpcFasi(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gLanguage && gLanguage != 8 && gDevilSquareEvent )
	{
		GCServerCmd(lpObj->m_Index, 1, 6, 0);
	}
	// ----
	int PKFlag = false;
	// ----
#ifdef PARTYKILLPKSET
	if( lpObj->PartyNumber >= 0 )
	{
		if( gParty.GetPKPartyPenalty(lpObj->PartyNumber) > 4 ) //PartyClass::GetPKPartyPenalty
		{
			PKFlag = true;
		}
	}
	else if( lpObj->m_PK_Level > 4 )
	{
		PKFlag = true;
	}
#else
	if( lpObj->m_PK_Level > 4 )
	{
		PKFlag = true;
	}
#endif
	// ----
	if( PKFlag )
	{
		int hour	= 0;
		int min		= 0;
		// ----
		if( lpNpc->TargetNumber != lpObj->m_Index )
		{
			if( gObjGetPkTime(lpObj, hour, min) == true )
			{
				char TextBuff[100];
				wsprintf(TextBuff, lMsg.Get(MSGGET(4, 79)), hour, min);
				// ----
				ChatTargetSend(lpNpc, TextBuff, lpObj->m_Index);
				// ----
				lpNpc->TargetNumber = lpObj->m_Index;
				// ----
				if( !gPkLimitFree )
				{
					return true;
				}
			}
		}
		else if( !gPkLimitFree )
		{
			int TextID = rand() % 3;
			// ----
			switch( TextID )
			{
				case 0:
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 98)), lpObj->m_Index);
					break;
					// --
				case 1:
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 99)), lpObj->m_Index);
					break;
					// --
				case 2:
					ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 100)), lpObj->m_Index);
					break;
			}
			// ---
			lpNpc->TargetNumber = 0;
			// ----
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB220 -> 100%
//1.01.00	-> 615240 -> 99% (Need define text to CommonServer.cfg)
bool NpcGuildMasterTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gGuildCreate )
	{
		ChatTargetSend(lpNpc, "서버분할 기간에는 길드를 생성할수가 없습니다", lpObj->m_Index);
		return true;
	}
	// ----
	if( lpObj->GuildNumber > 0 )
	{
		ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 116)), lpObj->m_Index);
		return true;
	}
	// ----
#ifdef GENS
	if( gGensSystem.GuildCreateReqGens )
	{
		if( !gGensSystem.GetGensInfluence(lpObj) )
		{
			GCServerMsgStringSend("You need Gens family to create guild", lpObj->m_Index, 1);
			return true;
		}
	}
#endif
	int nRet = gObjGuildMasterCapacityTest(lpObj);
	// ----
	if( !nRet )
	{
		ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 117)), lpObj->m_Index);
	}
	else if( nRet == 2 )
	{
		ChatTargetSend(lpNpc, lMsg.Get(MSGGET(4, 118)), lpObj->m_Index);
	}
	else
	{
		GCGuildMasterQuestionSend(lpObj->m_Index);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB350 -> 100%
//1.01.00	-> 6153B0 -> 100%
bool NpcRolensiaGuard(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gLanguage && gEnableEventNPCTalk )
	{
		GCServerCmd(lpObj->m_Index, 4, 0, 0);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB3A0 -> 100%
//1.01.00	-> 615400 -> 100%
bool NpcChaosGoblelin(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	gUserFindDevilSquareKeyEyes(lpObj->m_Index);
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB3D0 -> 100%
//1.01.00	-> 615430 -> 100%
bool NpcChaosCardMaster(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	//if( !g_bUseLotteryEvent )
	//{
	//	return false;
	//}
	// ----
	if( lpObj->m_bPShopOpen )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4,194)), lpObj->m_Index, 1);
		// ----
		return false;
	}
	// ----
	if( bCanChaosBox )
	{
		if( lpObj->m_IfState.use > 0 )
		{
			return false;
		}
		// ----
		if( lpObj->m_bPShopOpen )
		{
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(MSGGET(4,194)), lpObj->m_Index, 1);
			// ----
			return true;
		}
		// ----
		PMSG_TALKRESULT pResult;
		// ----
		pResult.h.c				= 0xC3;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof(pResult);
		pResult.result			= 0x15;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		lpObj->m_IfState.use	= 1;
		lpObj->m_IfState.type	= 7;
		// ----
		LogAddTD("[ChaosCardlMix] [%s][%s] ChaosCardMaster Start", lpObj->AccountID, lpObj->Name);
		// ----
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB5A0 -> 100%
//1.01.00	-> 615600 -> 100%
bool NpcRusipher(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gDevilSquareEvent )
	{
		return true;
	}
	// ----
	int aIndex = lpObj->m_Index;
	// ----
	if( lpObj->m_IfState.use > 0 )
	{
		return true;
	}
	// ----
	if( !gUserFindDevilSquareInvitation(aIndex) )
	{
		GCServerCmd(lpObj->m_Index, 1, 2 ,0);
	}
	else if( g_DevilSquare.GetState() )
	{
		PMSG_TALKRESULT pResult;
		// ----
		pResult.h.c				= 0xC3;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof(pResult);
		pResult.result			= 4;
		// ----
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		// ----
		lpObj->m_IfState.use	= 1;
		lpObj->m_IfState.type	= 12;
	}
	else
	{
		GCServerCmd(lpObj->m_Index, 1, 3, 0);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB6E0 -> 100%
//1.01.00	-> 615740 -> 100%
bool NpcPosionGirl(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB700 -> 100%
//1.01.00	-> 615760 -> 100%
bool NpcDeviasWizard(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB720 -> 100%
//1.01.00	-> 615780 -> 100%
bool NpcDeviasGuard(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB740 -> 100%
//1.01.00	-> 6157A0 -> 100%
bool NpcDeviasWareHousemen(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB760 -> 100%
//1.01.00	-> 6157C0 -> 100%
bool NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return gObjCalDistance(lpNpc, lpObj) > 10;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB780 -> 100%
//1.01.00	-> 615800 -> 100%
bool NpcNoriaRara(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gLanguage && gEnableEventNPCTalk )
	{
		GCServerCmd(lpObj->m_Index, 4, 2, 0);
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB7D0 -> 100%
//1.01.00	-> 615850 -> 100%
bool NpcDeviasMadam(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gLanguage && gEnableEventNPCTalk )
	{
		GCServerCmd(lpObj->m_Index, 4, 1, 0);
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB820 -> 99% (Need add msg send for THA proto)
//1.01.00	-> 6158A0 -> 99% (Need add msg send for THA proto)
bool NpcEventChipNPC(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( lpObj->m_IfState.use > 0 )
	{
		return true;
	}
	// ----
	if( lpNpc->MapNumber == 0 )
	{
		PMSG_REQ_VIEW_EC_MN pMsg;
		// ----
		PHeadSetB((LPBYTE)&pMsg, 0x37, sizeof(pMsg));
		pMsg.iINDEX				= lpObj->m_Index;
		strcpy(pMsg.szUID, lpObj->AccountID);
		pEventObj				= lpNpc;
		// ----
		DataSendEventChip((char*)&pMsg, sizeof(pMsg));
		// ----
		lpObj->m_IfState.type	= 9;
		lpObj->m_IfState.state	= 0;
		lpObj->m_IfState.use	= 1;
		// ----
		return true;
	}
	// ----
	if( lpNpc->MapNumber == 2 )
	{
		PMSG_EVENTCHIPINFO Result;
		// ----
		PHeadSetB((LPBYTE)&Result, 0x94, sizeof(Result));
		Result.Type				= 2;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&Result, Result.h.size);
		// ----
		lpObj->m_IfState.type	= 9;
		lpObj->m_IfState.state	= 0;
		lpObj->m_IfState.use	= 1;
		// ----
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AB820 -> 100%
//1.01.00	-> 615A70 -> 100%
bool NpcRorensiaSmith(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ABA10 -> 100%
//1.01.00	-> 615A90 -> 100%
bool NpcNoriJangIn(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ABA30 -> 100%
//1.01.00	-> 615AB0 -> 100%
bool NpcQuestCheck(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( g_QuestInfo.NpcTalk(lpNpc, lpObj) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ABA70 -> 100%
//1.01.00	-> 615AF0-> 100%
bool NpcServerDivision(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gEnableServerDivision )
	{
		return true;
	}
	// ----
	if( lpObj->lpGuild )
	{
		GCServerCmd(lpObj->m_Index, 6, 0, 0);
		// ----
		return true;
	}
	// ----
	lpObj->m_IfState.type	= 11;
	lpObj->m_IfState.state	= 0;
	lpObj->m_IfState.use	= 1;
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ABB30 -> 100%
//1.01.00	-> 615BB0 -> 100%
bool NpcRoadMerchant(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> ? -> 0%
//1.01.00	-> 615BD0 -> 0%
bool NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj)
{
	bool iITEM_LEVEL = 0; //loc1

	if( CHECK_LIMIT(lpObj->m_cBloodCastleIndex, MAX_BLOOD_CASTLE_LEVEL) == FALSE )
	{
		GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
		return FALSE;
	}

	LogAddTD("[Blood Castle] (%d) [%s][%s] Talk to Angel King",	lpObj->m_cBloodCastleIndex+1, lpObj->AccountID, lpObj->Name);

	if( g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bBC_REWARDED != false )
	{
		GCServerCmd(lpObj->m_Index, 1, 0x2E, 0);
		return FALSE;
	}

	if( g_BloodCastle.GetCurrentState(lpObj->m_cBloodCastleIndex) != 2 || g_BloodCastle.CheckPlayStart(lpObj->m_cBloodCastleIndex) == false)
	{
		GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
		return FALSE;
	}

	if( lpObj->m_bBloodCastleComplete == true)
	{
		GCServerCmd(lpObj->m_Index, 1, 0x2E, 0);
		return FALSE;
	}

	if( (iITEM_LEVEL = g_BloodCastle.CheckQuestItem(lpObj->m_Index)<0?0:iITEM_LEVEL = g_BloodCastle.CheckQuestItem(lpObj->m_Index)>2?0:1) ) //perfect
	{
		if( lpNpc->m_cBloodCastleIndex != lpObj->m_cBloodCastleIndex )
		{
			GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
			return FALSE;
		}

		if( g_BloodCastle.CheckUserBridgeMember(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == false )
		{
			GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
			return FALSE;
		}

		g_BloodCastle.SetUserState(lpObj->m_Index, 2);
		
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Win_Quest_Party = lpObj->PartyNumber;
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Win_Quest_Index = lpObj->m_Index;
		
		memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName, lpObj->Name, 10);
		memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID, lpObj->AccountID, 10);
		
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName[10] = 0;
		g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID[10] = 0;
		
		LogAddTD("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Destroy Castle Door [%s][%s]", lpObj->m_cBloodCastleIndex+1, 
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID, 
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName);

		LogAddTD("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Destroy Saint Status [%s][%s]", lpObj->m_cBloodCastleIndex+1, 
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID, 
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName);

		LogAddTD("[Blood Castle] (%d) Blood Castle Quest Succeed Result -> Win Quest [%s][%s]", lpObj->m_cBloodCastleIndex+1, 
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_AccountID, 
			g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szWin_Quest_CharName);

		if( g_BloodCastle.SetBridgeWinner(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == TRUE )
		{
			lpObj->m_bBloodCastleComplete = true;

			if( g_BloodCastle.CheckPartyExist(lpObj->m_Index) == FALSE )
			{
				GCServerCmd(lpObj->m_Index, 1, 0x17, 0);
				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest", 
					g_BloodCastle.GetBridgeIndexByMapNum(lpNpc->MapNumber)+1, lpObj->AccountID, lpObj->Name);
			}
			else
			{
				GCServerCmd(lpObj->m_Index, 1, 0x17, 0);
				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register", 
					g_BloodCastle.GetBridgeIndexByMapNum(lpNpc->MapNumber)+1, lpObj->AccountID, lpObj->Name);
			}
		}
		else
		{
			LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Winner Already Exist !!", 
				g_BloodCastle.GetBridgeIndexByMapNum(lpNpc->MapNumber)+1, lpObj->AccountID, lpObj->Name);
			return FALSE;
		}

		g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
	}
	else
	{
		if( g_BloodCastle.CheckUserWinnerParty(lpObj->m_cBloodCastleIndex, lpObj->m_Index) == TRUE )
		{
			lpObj->m_bBloodCastleComplete = true;

			if ( g_BloodCastle.CheckPartyExist(lpObj->m_Index) == FALSE )
			{
				GCServerCmd(lpObj->m_Index, 1, 0x17, 0);
				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest", 
					g_BloodCastle.GetBridgeIndexByMapNum(lpNpc->MapNumber)+1, lpObj->AccountID, lpObj->Name);
			}
			else
			{
				GCServerCmd(lpObj->m_Index, 1, 0x17, 0);
				LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Succeed Blood Castle Quest - More Party to register", 
					g_BloodCastle.GetBridgeIndexByMapNum(lpNpc->MapNumber)+1, lpObj->AccountID, lpObj->Name);
			}

			return FALSE;
		}
		GCServerCmd(lpObj->m_Index, 1, 0x18, 0);
	}
	return FALSE;
}
// -------------------------------------------------------------------------------

//1.00.92	-> ? -> 0%
//1.01.00	-> 616340 -> 0%
bool NpcAngelMessanger(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	int iIndex = lpObj->m_Index;

	if(gBloodCastleAllowingPlayers == FALSE)
	{
		if ( gObj[iIndex].m_PK_Level >= 4 )
		{
			GCServerCmd(iIndex, 0x38, 0, 0);
			return TRUE;
		}
	}
	
	C_PMSG_BCTALKOPEN customPmsg;

	PHeadSubSetB((LPBYTE)&customPmsg, 0xD0,0x15, sizeof(customPmsg));
	customPmsg.ThirdChangeUp = lpObj->ThirdChangeUp;
	customPmsg.Level = lpObj->Level;
	DataSend(lpObj->m_Index, (LPBYTE)&customPmsg, customPmsg.h.size);


	int iITEM_LEVEL = 0;

	iITEM_LEVEL = g_BloodCastle.CheckEnterItem(lpObj->m_Index);

	if ( iITEM_LEVEL != 0 )
	{
		if ( iITEM_LEVEL != 10 && g_BloodCastle.CheckEnterLevel(lpObj->m_Index, iITEM_LEVEL-1) )
		{
			if ( g_BloodCastle.CheckEnterFreeTicket(lpObj->m_Index) == true )
			{
				iITEM_LEVEL = 10;
			}
		}

		if ( iITEM_LEVEL == 10 )
		{
			for (int i=0;i<MAX_CLOACK_LEVEL;i++)
			{
				if ( g_BloodCastle.CheckEnterLevel(lpObj->m_Index, i) == FALSE )
				{
					iITEM_LEVEL = i;
				}
			}
		}

		if ( g_BloodCastle.GetCurrentState(iITEM_LEVEL-1) == BC_STATE_CLOSED && g_BloodCastle.CheckCanEnter(iITEM_LEVEL-1) != false )
		{
			PMSG_TALKRESULT pMsg;

			pMsg.h.c = 0xC3;
			pMsg.h.headcode = 0x30;
			pMsg.h.size = sizeof(pMsg);
			pMsg.result = 0x06;

			DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 12;
		}
		else
		{
			GCServerCmd(lpObj->m_Index, 1, 0x14, 0);
		}
	}
	else
	{
		GCServerCmd(lpObj->m_Index, 1, 0x15, 0);
	}

	return FALSE;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AC4E0 -> 100%
//1.01.00	-> 616580 -> 100%
bool NpcRolensiaMadam(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AC500 -> 100%
//1.01.00	-> 6165A0 -> 100%
bool NpcDeviasWeapon(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AC520 -> 100%
//1.01.00	-> 6165C0 -> 100%
bool NpcDarkSpiritTrainer(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( bCanChaosBox )
	{
		PMSG_TALKRESULT pMsg;
		// ----
		pMsg.h.c		= 0xC3;
		pMsg.h.headcode	= 0x30;
		pMsg.h.size		= sizeof(pMsg);
		// ----
		if( lpObj->m_bPShopOpen )
		{
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);
			// ----
			return true;
		}
		// ----
		lpObj->m_IfState.type		= 13;
		lpObj->m_IfState.state		= 0;
		lpObj->m_IfState.use		= 1;
		// ----
		pMsg.result					= 7;
		// ----
		lpObj->bIsChaosMixCompleted = false;
		// ----
		pMsg.level1					= gDQChaosSuccessRateLevel1;
		pMsg.level2					= gDQChaosSuccessRateLevel2;
		pMsg.level3					= gDQChaosSuccessRateLevel3;
		pMsg.level4					= gDQChaosSuccessRateLevel4;
		pMsg.level5					= gDQChaosSuccessRateLevel5;
		pMsg.level6					= gDQChaosSuccessRateLevel6;
		pMsg.level7					= gDQChaosSuccessRateLevel7;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		// ----
		GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		gObjInventoryTrans(lpObj->m_Index);
		// ----
		LogAddTD("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
		// ----
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AC720 -> 100%
//1.01.00	-> 6167D0 -> 100%
bool NpcJewelMixDealer(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( bCanChaosBox )
	{
		if( lpObj->m_IfState.use > 0 )
		{
			return true;
		}
		// ----
		PMSG_TALKRESULT pResult;
		// ----
		pResult.h.c				= 0xC3;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof(pResult);
		pResult.result			= 0x09;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		lpObj->m_IfState.use	= 1;
		lpObj->m_IfState.type	= 20;
		// ----
		LogAddTD("[JewelMix] [%s][%s] Jewel Mix Dealer Start", lpObj->AccountID, lpObj->Name);
		// ----
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AC830 -> 100%
//1.01.00	-> 6168E0 -> 100%
bool NpcSiegeMachine_Attack(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( lpObj->m_IfState.use > 0 )
	{
		return true;
	}
	// ----
	PMSG_ANS_USE_WEAPON_INTERFACE pResult = { 0 };
	// ----
	PHeadSubSetB((LPBYTE)&pResult, 0xB7, 0x00, sizeof(pResult));
	// ----
	pResult.btResult		= 0;
	pResult.btWeaponType	= 1;
	pResult.btObjIndexH		= SET_NUMBERH(lpNpc->m_Index);
	pResult.btObjIndexL		= SET_NUMBERL(lpNpc->m_Index);
	// ----
	if( !gObjIsConnectedGP(lpNpc->m_iWeaponUser) )
	{
		lpNpc->m_iWeaponUser	= 0;
		lpNpc->m_btWeaponState	= 1;
	}
	// ----
	lpNpc->m_btWeaponState = 1;
	// ----
	if( lpNpc->m_btWeaponState == 1 ) // >< Stupid WZ
	{
		pResult.btResult		= 1;
		lpNpc->m_btWeaponState	= 2;
		lpNpc->m_iWeaponUser	= lpObj->m_Index;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		lpObj->m_IfState.use	= 1;
		lpObj->m_IfState.type	= 15;
	}
	else
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AC9D0 -> 100%
//1.01.00	-> 616A80 -> 100%
bool NpcSiegeMachine_Defense(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( lpObj->m_IfState.use > 0 )
	{
		return true;
	}
	// ----
	PMSG_ANS_USE_WEAPON_INTERFACE pResult = { 0 };
	// ----
	PHeadSubSetB((LPBYTE)&pResult, 0xB7, 0x00, sizeof(pResult));
	// ----
	pResult.btResult		= 0;
	pResult.btWeaponType	= 2;
	pResult.btObjIndexH		= SET_NUMBERH(lpNpc->m_Index);
	pResult.btObjIndexL		= SET_NUMBERL(lpNpc->m_Index);
	// ----
	if( !gObjIsConnectedGP(lpNpc->m_iWeaponUser) )
	{
		lpNpc->m_iWeaponUser	= 0;
		lpNpc->m_btWeaponState	= 1;
	}
	// ----
	lpNpc->m_btWeaponState = 1;
	// ----
	if( lpNpc->m_btWeaponState == 1 ) // >< Stupid WZ
	{
		pResult.btResult		= 1;
		lpNpc->m_btWeaponState	= 2;
		lpNpc->m_iWeaponUser	= lpObj->m_Index;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		lpObj->m_IfState.use	= 1;
		lpObj->m_IfState.type	= 16;
	}
	else
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.93	-> ? -> 0%
//1.01.01	-> ? -> 0%
bool NpcElderCircle(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

//#if(_GSCS==1)
	if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		ChatTargetSend(lpNpc,(lMsg.Get(MSGGET(6, 85))), lpObj->m_Index);
		return TRUE;
	}

	if(strcmp(lpObj->GuildName, "") == 0 )
	{
		ChatTargetSend(lpNpc,(lMsg.Get(MSGGET(6, 86))), lpObj->m_Index);
		return TRUE;
	}

	if(!g_CastleSiege.CheckGuildOwnCastle(lpObj->GuildName) || (lpObj->GuildStatus != G_MASTER && lpObj->GuildStatus != G_SUB_MASTER))
	{
		ChatTargetSend(lpNpc,(lMsg.Get(MSGGET(6, 86))), lpObj->m_Index);
		return TRUE;
	}
//#endif

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x0C;

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;

//#if(_GSCS==1)
	pMsg.level1 = 0;
	pMsg.level2 = 0;
	pMsg.level3 = 0;
	pMsg.level4 = 0;
	pMsg.level5 = 0;
	pMsg.level6 = 0;
	pMsg.level7 = 0;//Season 4.5 fix

	if( bCanChaosBox == TRUE)
	{
		lpObj->m_IfState.type = 7;

		lpObj->bIsChaosMixCompleted = false;
		lpObj->m_bIsCastleNPCUpgradeCompleted = false; 

		pMsg.level1 = 1;

		GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		gObjInventoryTrans(lpObj->m_Index);

		LogAddTD("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
//#endif

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	return TRUE;
}
// -------------------------------------------------------------------------------

//1.00.93	-> ? -> 0%
//1.01.01	-> ? -> 0%
bool NpcCastleGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

//#if(_GSCS==1)
	if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		ChatTargetSend(lpNpc,(lMsg.Get(MSGGET(6, 87))), lpObj->m_Index);
		return TRUE;
	}
//#endif

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x0D;

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;


	return TRUE;
}
// -------------------------------------------------------------------------------

//1.00.93	-> ? -> 0%
//1.01.01	-> ? -> 0%
bool NpcCastleGateLever(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}

//#if(_GSCS==1)
	bool bControlEnable = FALSE;
	BYTE btResult = 0;
	int iGateIndex = -1;

	if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		if(lpObj->m_btCsJoinSide != TRUE)
		{
			btResult = 4;
			bControlEnable = FALSE;
		}
		else
		{
			bControlEnable = TRUE;
		}
	}
	else
	{
		if(g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) == FALSE && g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE) //Fixed
		{
			btResult = 4;
			bControlEnable = FALSE;
		}
		else
		{
			bControlEnable = TRUE;
		}
	}

	if(bControlEnable != FALSE)
	{
		if(g_CastleSiege.CheckLeverAlive(lpNpc->m_Index) == TRUE)
		{
			if(g_CastleSiege.CheckCsGateAlive(lpNpc->m_iCsGateLeverLinkIndex) == TRUE)
			{
				btResult = 1;
				iGateIndex = lpNpc->m_iCsGateLeverLinkIndex;
			}
			else
			{
				btResult = 2;
			}
		}
		else
		{
			btResult = 3;
		}

	}

	GCAnsCsGateState(lpObj->m_Index,btResult,iGateIndex);

	if(btResult != 1)
	{
		return TRUE;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;
//#endif

	return TRUE;
}
// -------------------------------------------------------------------------------

//1.00.93	-> ? -> 0%
//1.01.01	-> ? -> 0%
bool NpcCastleCrown(LPOBJ lpNpc, LPOBJ lpObj)
{
	//#if(_GSCS==1)
	if ( gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		return TRUE;
	}
	
	if(lpObj->m_btCsJoinSide < 2 || lpObj->m_bCsGuildInvolved == FALSE )
	{
		return TRUE;
	}

	if(g_CastleSiege.CheckUnionGuildMaster(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	if( (abs(lpObj->Y - lpNpc->Y)) > 3 || (abs(lpObj->X - lpNpc->X)) > 3)
	{
		return TRUE;
	}

	if(g_CastleSiege.GetRegCrownAvailable() == FALSE )
	{
		return TRUE;
	}

	int iUserIndex = g_CastleSiege.GetCrownUserIndex();

	if(gObjIsConnected(iUserIndex) == FALSE )
	{
		int iSwitchIndex1 = g_CastleSiege.GetCrownSwitchUserIndex(217);
		int iSwitchIndex2 = g_CastleSiege.GetCrownSwitchUserIndex(218);

		if(gObjIsConnected(iSwitchIndex1) == FALSE || gObjIsConnected(iSwitchIndex2) == FALSE )
		{
			GCAnsCsAccessCrownState(lpObj->m_Index,4);
			LogAddTD("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
			return TRUE;
		}

		if(lpObj->m_btCsJoinSide != gObj[iSwitchIndex1].m_btCsJoinSide || lpObj->m_btCsJoinSide != gObj[iSwitchIndex2].m_btCsJoinSide)
		{
			GCAnsCsAccessCrownState(lpObj->m_Index,4);
			LogAddTD("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s) (S1:%s/%s)(S2:%s/%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName,gObj[iSwitchIndex1].Name,gObj[iSwitchIndex1].GuildName,gObj[iSwitchIndex2].Name,gObj[iSwitchIndex2].GuildName);
			return TRUE;
		}
		else
		{
			GCAnsCsAccessCrownState(lpObj->m_Index,0);
			g_CastleSiege.SetCrownUserIndex(lpObj->m_Index);
			g_CastleSiege.SetCrownAccessUserX(lpObj->X);
			g_CastleSiege.SetCrownAccessUserY(lpObj->Y);
			g_CastleSiege.SetCrownAccessTickCount();
			g_CastleSiege.NotifyAllUserCsProgState(0,lpObj->GuildName);
			LogAddTD("[CastleSiege] [%s][%s] Start to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->GuildName);
		}
	}
	else if(lpObj->m_Index != iUserIndex)
	{
		GCAnsCsAccessCrownState(lpObj->m_Index,3);
	}

	return TRUE;
//#else
//	return TRUE;
//#endif
}
// -------------------------------------------------------------------------------

//1.00.93	-> ? -> 0%
//1.01.01	-> ? -> 0%
bool NpcCastleSwitch(LPOBJ lpNpc, LPOBJ lpObj)
{
	//#if(_GSCS==1)
	if(gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
	{
		return TRUE;
	}
	
	if(lpObj->m_btCsJoinSide < 2)
	{
		return TRUE;
	}

	if( (abs(lpObj->Y - lpNpc->Y)) > 3 || (abs(lpObj->X - lpNpc->X)) > 3)
	{
		return TRUE;
	}

	if(g_CastleSiege.CheckGuardianStatueExist() == TRUE)
	{
		MsgOutput(lpObj->m_Index,lMsg.Get(MSGGET(6, 200)));
		return TRUE;
	}

	int iUserIndex = g_CastleSiege.GetCrownSwitchUserIndex(lpNpc->Class);

	if(!gObjIsConnected(iUserIndex))
	{
		GCAnsCsAccessSwitchState(lpObj->m_Index,lpNpc->m_Index,-1,1);
		g_CastleSiege.SetCrownSwitchUserIndex(lpNpc->Class,lpObj->m_Index);
		LogAddTD("[CastleSiege] [%s][%s] Start to Push Castle Crown Switch (GUILD:%s) - CS X:%d/Y:%d",lpObj->AccountID,lpObj->Name,lpObj->GuildName,lpNpc->X,lpNpc->Y);
	}
	else if(lpObj->m_Index != iUserIndex)
	{
		GCAnsCsAccessSwitchState(lpObj->m_Index,lpNpc->m_Index,iUserIndex,2);
	}

	return TRUE;
//#else
//	return TRUE;
//#endif
}
// -------------------------------------------------------------------------------

//1.00.93	-> ? -> 0%
//1.01.01	-> ? -> 0%
bool NpcCastleHuntZoneGuard(LPOBJ lpNpc, LPOBJ lpObj) // -> Only _CS
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	if( (lpObj->m_IfState.use ) > 0 )
	{
		return true;
	}
	// ----
	PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE pResult = { 0 };
	// ----
	PHeadSubSetB((LPBYTE)&pResult, 0xB9, 0x03, sizeof(PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE));
	// ----
	pResult.btResult		= TRUE;
	pResult.iMaxPrice		= 300000;
	pResult.iUnitOfPrice	= 10000;
	pResult.btUsable		= 0;
	pResult.iCurrentPrice	= 0;
	pResult.iCurrentPrice	= g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index, false);
	// ----
	if( g_CastleSiege.GetHuntZoneEnter() )
	{
		pResult.btUsable = 1;
	}
	// ----
	if( lpObj->lpGuild != 0 )
	{
		if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) )
		{
			pResult.btResult = 2;
		}
		// ----
		if( g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) )
		{
			if( lpObj->GuildStatus == G_MASTER )
			{
				pResult.btResult = 3;
			}
		}
	}
	else
	{
		if( g_CastleSiege.GetHuntZoneEnter() )
		{
			pResult.btResult = 1;
		}
	}
	// ----
	DataSend(lpObj->m_Index,(LPBYTE)&pResult,pResult.h.size);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ACD50 -> 100%
//1.01.00	-> 616F60 -> Not in use
bool NpcShadowPhantom(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	if( lpObj->Level > g_ShadowPahtomMaxLevel || lpObj->ThirdChangeUp )
	{
		GCServerCmd(lpObj->m_Index, 13, 0, 0);
		// ----
		return true;
	}
	// ----
	int iIncleaseAttackPower	= 0;
	int iIncleaseDefensePower	= 0;
	// ----
	if ( lpObj->Level <= 180 )
	{
		iIncleaseAttackPower	= lpObj->Level / 3 + 45;
		iIncleaseDefensePower	= lpObj->Level / 5 + 50;
	}
	else
	{
		iIncleaseAttackPower	= 105;
		iIncleaseDefensePower	= 86;
	}
	// ----
	gObjAddBuffEffect(lpObj, AT_NPC_HELP, 2, iIncleaseAttackPower, 3, 
		iIncleaseDefensePower, (lpObj->Level / 6 + 30) * 60);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 616F60 -> 100%
bool NpcShadowPhantom(int iObjIndex) //-> Complete
{
	LPOBJ lpObj = &gObj[iObjIndex];
	// ----
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	if( lpObj->Level > g_ShadowPahtomMaxLevel || lpObj->ThirdChangeUp )
	{
		GCServerCmd(lpObj->m_Index, 13, 0, 0);
		// ----
		return true;
	}
	// ----
	int iIncleaseAttackPower	= 0;
	int iIncleaseDefensePower	= 0;
	// ----
	if ( lpObj->Level <= 180 )
	{
		iIncleaseAttackPower	= lpObj->Level / 3 + 45;
		iIncleaseDefensePower	= lpObj->Level / 5 + 50;
	}
	else
	{
		iIncleaseAttackPower	= 105;
		iIncleaseDefensePower	= 86;
	}
	// ----
	gObjAddBuffEffect(lpObj, AT_NPC_HELP, 2, iIncleaseAttackPower, 3, 
		iIncleaseDefensePower, (lpObj->Level / 6 + 30) * 60);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.93	-> ? -> 0%
//1.01.01	-> ? -> 0%
bool NpcCrywolfAltar(LPOBJ lpNpc, LPOBJ lpObj) //-> Only on _CS
{
	if ( gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}
	return TRUE;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ACEB0 -> 100%
//1.01.00	-> 6170D0 -> 100%
bool NpcQuartermasterPamella(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ACEF0 -> 100%
//1.01.00	-> 617110 -> 100%
bool NpcQuartermasterAngela(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ACF30 -> 100%
//1.01.00	-> 617150 -> 100%
bool NPcFireCrackerStore(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ACF70 -> 100%
//1.01.00	-> 617190 -> 100%
bool NpcIllusionHolyRelicsStatue(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
		g_IllusionTempleEvent.IllusionHolyRelicsStatusAct(lpObj->MapNumber, lpNpc, lpObj);
		return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5ACFD0 -> 100%
//1.01.00	-> 6171F0 -> 100%
bool NpcHolyRelicsBoxAllied(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
		g_IllusionTempleEvent.HolyRelicsBoxAlliedAct(lpObj->MapNumber, lpNpc, lpObj);
		return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD030 -> 100%
//1.01.00	-> 617250 -> 100%
bool NpcHolyRelicsBoxIllusion(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
		g_IllusionTempleEvent.HolyRelicsBoxIllusionAct(lpObj->MapNumber, lpNpc, lpObj);
		return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD090 -> 100%
//1.01.00	-> 6172B0 -> 100%
bool NpcPlatformMirage(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
		g_IllusionTempleEvent.PlatformMirageAct(lpNpc, lpObj);
		return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD0E0 -> 100%
//1.01.00	-> 617300 -> 100%
bool NpcSilviaTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD120 -> 100%
//1.01.00	-> 617340 -> 100%
bool NpcRheaTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD160 -> 100%
//1.01.00	-> 617380 -> 100%
bool NpcMarceTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD1A0 -> 100%
//1.01.00	-> 6173C0 -> 100%
bool NpcSeedMaster(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( bCanChaosBox )
	{
		if( lpObj->m_IfState.use > 0 )
		{
			return true;
		}
		// ----
		if( lpObj->m_bPShopOpen )
		{
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(MSGGET(4,194)), lpObj->m_Index, 1);
			// ----
			return true;
		}
		// ----
		PMSG_TALKRESULT pResult;
		// ----
		pResult.h.c				= 0xC3;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof(pResult);
		pResult.result			= 0x17;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		lpObj->m_IfState.use	= 1;
		lpObj->m_IfState.type	= 7;
		// ----
		LogAddTD("[JewelMix] [%s][%s] Socket Item Mix Start", lpObj->AccountID, lpObj->Name);
		// ----
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD320 -> 100%
//1.01.00	-> 617540 -> 100%
bool NpcSeedResearcher(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( bCanChaosBox )
	{
		if( lpObj->m_IfState.use > 0 )
		{
			return true;
		}
		// ----
		if( lpObj->m_bPShopOpen )
		{
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(MSGGET(4,194)), lpObj->m_Index, 1);
			// ----
			return true;
		}
		// ----
		PMSG_TALKRESULT pResult;
		// ----
		pResult.h.c				= 0xC3;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof(pResult);
		pResult.result			= 0x18;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		lpObj->m_IfState.use	= 1;
		lpObj->m_IfState.type	= 7;
		// ----
		LogAddTD("[JewelMix] [%s][%s] Socket Item Mix Start", lpObj->AccountID, lpObj->Name);
		// ----
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD4A0 -> 100%
//1.01.00	-> 6176C0 -> 100%
bool NpcCherryTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD4E0 -> 100%
//1.01.00	-> 617700 -> 100%
bool NpcLuckyCoinDelgado(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD520 -> 99% (Need change 1 item)
//1.01.00	-> 617740 -> 99% (Need change 1 item)
bool Npc_SantaTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
		//GEReqCheckEventNPCTarget
		EGAnsRegXMasGetPayItem(lpObj->m_Index, 1);
		return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD570 -> 100%
//1.01.00	-> 617790 -> 100%
bool Npc_SnowManTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
		CGReqCheckSnowManNPC(lpObj->m_Index);
		return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.00.92	-> 5AD5C0 -> 100%
//1.01.00	-> 6177E0 -> 100%
bool Npc_LittleSantaTalk(LPOBJ lpNpc, LPOBJ lpObj, int BuffEffect) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	switch( BuffEffect )
	{
	case AT_NULL_BUFF_EFFECT:
		lpObj->Life = lpObj->AddLife + lpObj->MaxLife;
		GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
		break;
		// --
	case AT_INCREASE_ATTACK:
		lpObj->Mana = lpObj->AddMana + lpObj->MaxMana;
		GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
		break;
		// --
	case AT_XMASS_ATTACK:
		gObjAddBuffEffect(lpObj, AT_XMASS_ATTACK, 57, 30, 0, 0, 1800);
		break;
		// --
	case AT_XMASS_DEFENSE:
		gObjAddBuffEffect(lpObj, AT_XMASS_DEFENSE, 58, 100, 0, 0, 1800);
		break;
		// --
	case AT_XMASS_HEALTH:
		gObjAddBuffEffect(lpObj, AT_XMASS_HEALTH, 4, 500, 0, 0, 1800);
		break;
		// --
	case AT_XMASS_MANA:
		gObjAddBuffEffect(lpObj, AT_XMASS_MANA, 5, 500, 0, 0, 1800);
		break;
		// --
	case AT_XMASS_DEXTERITY:
		gObjAddBuffEffect(lpObj, AT_XMASS_DEXTERITY, 1, 15, 0, 0, 1800);
		break;
		// --
	case AT_XMASS_AG_RECOVERY:
		gObjAddBuffEffect(lpObj, AT_XMASS_AG_RECOVERY, 59, 10, 0, 0, 1800);
		break;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617AA0 -> 100%
bool Npc_EventDungeonTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
		PMSG_TALKRESULT pResult;
		// ----
		pResult.h.c				= 0xC3;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof(pResult);
		pResult.result			= 0x24;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617B10 -> 90% (Need add global bool & write act)
bool Npc_LugardTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
		//if( bDoppelgangerEvent )
		//{
#ifdef DP
			g_DoppleganerEvent.PlatformLugardAct(lpNpc, lpObj); //-> 69e000
			return true;
#endif
		//}
		// ----
		//return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617B80 -> 95% (Need write act)
bool Npc_MiddleTreasureTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
#ifdef DP
		g_DoppleganerEvent.MiddleTreasureAct(lpNpc, lpObj); //-> 69e2d0
		return true;
#endif
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617BD0 -> 95% (Need write act)
bool Npc_LastTreasureTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
#ifdef DP
		g_DoppleganerEvent.LastTreasureAct(lpNpc, lpObj); //-> 69e820
		return true;
#endif
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617C20 -> 99% (Need fix state)
bool Npc_Dialog(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	PMSG_ANS_DIG_NPC_CLICK pResult;
	// ----
	char * GensName;
	// ----
	if( gObjIsConnected(lpObj->m_Index) )
	{
		lpObj->TargetShopNumber = lpNpc->Class;
		// ----
		switch(lpObj->TargetShopNumber) // Not original, but easier
		{
#ifdef GENS
		case 543:
			{
				pResult.dwContributePoint	= gGensSystem.GetContributePoint(lpObj);
				GensName					= gGensSystem.GetGensInfluenceName(lpObj);
				// ----
				LogAddTD("[GensSystem] Gens_Duprian_NPC [%s][%s] GensName [%s] Total ContributePoint [%d]",
					lpObj->AccountID, lpObj->Name, GensName, pResult.dwContributePoint);
			}
			break;
			// --
		case 544:
			{
				pResult.dwContributePoint	= gGensSystem.GetContributePoint(lpObj);
				GensName					= gGensSystem.GetGensInfluenceName(lpObj);
				// ----
				LogAddTD("[GensSystem] Gens_Vanert_NPC [%s][%s] GensName [%s] Total ContributePoint [%d]",
					lpObj->AccountID, lpObj->Name, GensName, pResult.dwContributePoint);
			}
			break;
#endif
			// --
		case 257: case 566:
		case 567: case 568:
			{
				pResult.dwContributePoint = 0;
			}
			break;
		}
		// ----
		if( lpObj->TargetShopNumber == 568 )
		{
			//lpObj->m_IfState._bf0 = lpObj->m_IfState._bf0 & 0xFFFF003F | 0x280;
			//lpObj->m_IfState._bf0 &= 0xFFFFFFC3u;
			//lpObj->m_IfState._bf0 = lpObj->m_IfState._bf0 & 0xFFFFFFFC | 1;
		}
		// ----
		pResult.h.c					= 0xC3;
		pResult.h.headcode			= 0xF9;
		pResult.h.subcode			= 1;
		pResult.h.size				= sizeof(pResult);
		pResult.wNPCIndex			= lpObj->TargetShopNumber;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617E30 -> 100%
bool NpcKristenTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617E70 -> 100%
bool NpcRaulTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	NpcJewelMixDealer(lpNpc, lpObj);
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617EB0 -> 100%
bool NpcJuliaTalk(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( gObjIsConnected(lpObj->m_Index) )
	{
		PMSG_TALKRESULT pResult;
		// ----
		pResult.h.c				= 0xC3;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof(pResult);
		pResult.result			= 0x25;
		pResult.level1			= 0;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		return true;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617F30 -> 100%
bool NpcRehina(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617F70 -> 100%
bool NpcVolo(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( !gObjIsConnected(lpObj->m_Index) )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//1.01.00	-> 617FB0 -> 100%
bool NpcDabide(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	if( bCanChaosBox )
	{
		if( lpObj->m_IfState.use > 0 )
		{
			return true;
		}
		// ----
		if( lpObj->m_bPShopOpen )
		{
			LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			GCServerMsgStringSend(lMsg.Get(MSGGET(4,194)), lpObj->m_Index, 1);
			// ----
			return true;
		}
		// ----
		lpObj->bIsChaosMixCompleted = false;
		// ----
		PMSG_TALKRESULT pResult;
		// ----
		pResult.h.c				= 0xC3;
		pResult.h.headcode		= 0x30;
		pResult.h.size			= sizeof(pResult);
		pResult.result			= 0x26;
		// ----
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		// ----
		lpObj->m_IfState.use	= 1;
		lpObj->m_IfState.type	= 7;
		// ----
		gObjInventoryTrans(lpObj->m_Index);
		// ----
		LogAddTD("[Lucky Item] [%s][%s] Lucky Item Mix Start", lpObj->AccountID, lpObj->Name);
		// ----
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------