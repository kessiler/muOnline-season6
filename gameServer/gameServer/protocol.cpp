//GameServer 1.00.77 JPN - Completed
#include "stdafx.h"
#include "protocol.h"
#include "user.h"
#include "giocp.h"
#include "Event.h"
#include "logproc.h"
#include "GameMain.h"
#include "..\common\winutil.h"
#include "TNotice.h"
#include "QuestInfo.h"
#include "ConMember.h"
#include "GameServer.h"
#include "GMMng.h"
#include "BattleSoccerManager.h"
#include "MixSystem.h"
#include "CastleSiegeSync.h"
#include "TUnion.h"
#include "JewelMixSystem.h"
#include "Weapon.h"
#include "DBSockMng.h"
#include "KalimaGate.h"
#include "ObjUseSkill.h"
#include "ObjCalCharacter.h"
#include "NpcTalk.h"
#include "Shop.h"
#include "CashItemPeriodSystem.h"
#include "BattleSoccer.h"
#include "BattleSoccerManager.h"
#include "gObjMonster.h"
#include "ComboAttack.h"
#include "Gate.h"
#include "MultiAttackHackCheck.h"
#include "ItemAddOption.h"
#include "JewelOfHarmonySystem.h"
#include "PacketCheckSum.h"
#include "DarkSpirit.h"
#include "Crywolf.h"
#include "CrywolfAltar.h"
#include "CrywolfSync.h"
#include "DevilSquare.h"
#include "KanturuEntranceNPC.h"
#include "CashShop.h"

#include "Sprotocol.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"

#include "Mercenary.h"
#include "Guardian.h"

#ifdef WZQUEST
#include "QuestExpProgMng.h"
#include "QuestExpProtocol.h"
#endif

int ltesttime;
int logincounttest;

#include "ChaosCastle.h"
#include "IllusionTempleEvent.h"
#include "BuffEffectSlot.h"
#include "MasterLevelSkillTreeSystem.h"
#include "MoveCommand.h"

int tempindex;
int iCount;

//#if(_GSCS==1)
#include "MapServerManager.h"
#include "LifeStone.h"
#pragma warning ( disable : 4101 )
#pragma warning ( disable : 4700 )
//#endif


#ifdef NPVP
#include "NewPVP.h"
#endif

#ifdef GENS
#include "GensSystem.h"
#include "GensSystemProtocol.h"
#endif

#ifdef PERIOD
#include "PeriodItemEx.h"
#endif

#ifdef IMPERIAL
#include "ImperialGuardian.h"
#endif

#ifdef DP
#include "Doppelganger.h"
#endif

#include "MUHelper.h"
#include "LuckyItem.h"
#include "..\common\SetItemOption.h"
#include "CheatGuard.h"

#ifdef __CUSTOMS__
#include "ResetSystem.h"
#include "MixOption.h"
#include "NewsBoard.h"
#include "ChatFilter.h"
#include "JewelsEx.h"
#include "ConnectEx.h"
#endif

#ifdef __NOVUS__
#include "CraftSystem.h"
#endif

#ifdef __MAKELO__
#include "OfflineAttack.h"
#endif

#ifdef WZQUEST
#include "QuestExpProtocol.h"
#endif

#ifdef OFFEXP
#include "OffExp.h"
#endif

#include "GameShop.h"
#include "OfflineTrade.h"
#include "LogToFile.h"

CLogToFile g_GMChatLog("Global", ".\\LOG\\Chat", TRUE);
CLogToFile g_GensChatLog("Gens", ".\\LOG\\Chat", TRUE);
CLogToFile g_GuildChatLog("Guild", ".\\LOG\\Chat", TRUE);
CLogToFile g_AllianceChatLog("Alliance", ".\\LOG\\Chat", TRUE);
CLogToFile g_PrivateChatLog("Private", ".\\LOG\\Chat", TRUE);
CLogToFile g_PartyChatLog("Party", ".\\LOG\\Chat", TRUE);
CLogToFile g_ChatFilterLog("Filter", ".\\LOG\\Chat", TRUE);

#ifdef NPVP
int OnCGInviteDuel(LPPMSG_REQ_DUEL_INVITE lpMsg, int aIndex);
int OnCGAnswerDuel(LPPMSG_ANS_DUEL_ANSWER lpMsg, int aIndex);
int OnCGLeaveDuel(LPPMSG_REQ_DUEL_EXIT lpMsg, int aIndex);
int OnDuelChannelJoin(LPPMSG_REQ_DUEL_JOINCNANNEL lpMsg, int aIndex);
int OnDuelChannelLeave(LPPMSG_REQ_DUEL_LEAVECNANNEL lpMsg, int aIndex);
#endif

BOOL PacketCheckTime(LPOBJ lpObj) 
{
	if ( ( GetTickCount() - lpObj->m_PacketCheckTime ) < 300 )
	{
		return FALSE;
	}

	lpObj->m_PacketCheckTime = GetTickCount();
	return TRUE;
}

BOOL DataEncryptCheck(int aIndex, BYTE protoNum, BOOL Encrypt) 
{
	if ( Encrypt == FALSE )
	{
		LogAdd("Error-L1 : Not Encrypt %s %d", gObj[aIndex].AccountID, protoNum);
		CloseClient(aIndex);
		return FALSE;
	}

	return TRUE;
}

void ProtocolCore(BYTE protoNum, BYTE * aRecv, int aLen, int aIndex, BOOL Encrypt, int serial)
{
	if ( gStalkProtocol )
	{
		if ( gStalkProtocolId[0] == gObj[aIndex].AccountID[0] )
		{
			if ( gStalkProtocolId[1] == gObj[aIndex].AccountID[1] )
			{
				if ( !strcmp(gStalkProtocolId, gObj[aIndex].AccountID ) )
				{
					//LogAddHeadHex(gObj[aIndex].AccountID, (char*)aRecv, aLen);
				}
			}
		}
	}
#if( DEBUG_PROTOCOL == 1 )
	LogAddHeadHex(gObj[aIndex].AccountID, (char*)aRecv, aLen);
#endif
	if ( serial >= 0 )
	{
		if ( gObj[aIndex].Type == OBJ_USER )
		{
			if ( gNSerialCheck[aIndex].InCheck(serial) == FALSE )
			{
				LogAdd("Error-L1 : Socket Serial %s %d o_serial:%d serial:%d ", gObj[aIndex].AccountID, protoNum, gNSerialCheck[aIndex].GetSerial(), serial);
				CloseClient(aIndex);
			}
		}
	}
	if ( protoNum == 0xF1 || protoNum == 0xF3 )
	{
		switch ( protoNum )
		{
			case 0x0E:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGLiveClient((PMSG_CLIENTTIME *)aRecv, aIndex);
					g_CheatGuard.SendCheckRequest(aIndex);
					tempindex = aIndex;
				}
				break;

			case 0xF1:
				{
					PMSG_DEFAULT2 * lpMsg2 = (PMSG_DEFAULT2 *)aRecv;
				
#ifdef SEASON5DOT4_ENG //-> Password convert
					aRecv[1] -= 0x0A;
					aLen = aRecv[1];
					for(int i = 24; i<50; i++)
					{
						aRecv[i] = aRecv[i+10];
					}
#endif

					switch ( lpMsg2->subcode )
					{
						case 0x01:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CSPJoinIdPassRequest((PMSG_IDPASS *)aRecv, aIndex);
							}
							break;
						case 0x02:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CGClientCloseMsg((PMSG_CLIENTCLOSE *)aRecv, aIndex);
							}
							break;
						case 0x03:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CGClientMsg((PMSG_CLIENTMSG *)aRecv, aIndex);
							}
							break;
					}
				}
				break;

			case 0xF3:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							DataServerGetCharListRequest(aIndex);
							break;
						case 0x01:
							CGPCharacterCreate((PMSG_CHARCREATE *)aRecv, aIndex);
							break;
						case 0x02:
							CGPCharDel((PMSG_CHARDELETE *)aRecv, aIndex);
							break;
						case 0x03:
							CGPCharacterMapJoinRequest((PMSG_CHARMAPJOIN *)aRecv, aIndex);
							break;
						case 0x06:
							CGLevelUpPointAdd((PMSG_LVPOINTADD *)aRecv, aIndex);
							break;
						case 0x12:
							gObjMoveDataLoadingOK(aIndex);
							break;
						case 0x15: //Season 4 Add-on
							CGCheckNameRequest((PMSG_REQ_CHARACTER_CHECKNAME *)aRecv, aIndex);
							break;
						case 0x16: //Season 4 Add-on
							CGChangeNameRequest((PMSG_REQ_CHARACTER_CHANGENAME *)aRecv, aIndex);
							break;
						case 0x30:
							GCSkillKeyRecv((PMSG_SKILLKEY *)aRecv, aIndex);
							break;
						case 0x52: //1.01.00
							g_MasterSkillSystem.CGReqGetMasterLevelSkill((PMSG_REQ_MASTERLEVEL_SKILL*)aRecv, aIndex);
							break;
					}
				}
				break;
		}
	}
	else
	{
		LPOBJ lpObj = &gObj[aIndex];

		//LogAddC(2, "HEAD: %x", protoNum);	//DeBug
				
		switch ( protoNum )
		{
			case 0x00:
				tempindex = aIndex;
				PChatProc((PMSG_CHATDATA *)aRecv, aIndex);
				break;
			case 0x01:
				CGChatRecv((PMSG_CHATDATA_NUMBER *)aRecv, aIndex);
				break;
			case 0x02:
				CGChatWhisperRecv((PMSG_CHATDATA_WHISPER *)aRecv, aIndex);
				break;
			case 0x03:
				CGCheckMainRecv((PMSG_CHECK_MAINEXE_RESULT *)aRecv, aIndex);
				break;
			case 0x0E:
				CGLiveClient((PMSG_CLIENTTIME *)aRecv, aIndex);
				g_CheatGuard.SendCheckRequest(aIndex);
				break;
			case PROTOCOL_MOVE:
				PMoveProc((PMSG_MOVE *)aRecv, aIndex);
				break;
			case PROTOCOL_POSITION:
				RecvPositionSetProc((PMSG_POSISTION_SET *)aRecv, aIndex);
				break;
			case PROTOCOL_ATTACK:
				CGAttack((PMSG_ATTACK *)aRecv, aIndex);
				break;
			case 0x18:
				CGActionRecv((PMSG_ACTION *)aRecv, aIndex);
				break;
			case 0x19:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGMagicAttack((PMSG_MAGICATTACK *)aRecv, aIndex);
				}
				break;
			case 0x1B:
				CGMagicCancel((PMSG_MAGICCANCEL *)aRecv, aIndex);
				break;
			case 0x1C:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTeleportRecv((PMSG_TELEPORT *)aRecv, aIndex);
				}
				break;
			case 0xB0:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTargetTeleportRecv((PMSG_TARGET_TELEPORT *)aRecv, aIndex);
				}
				break;
			case PROTOCOL_BEATTACK:
				CGBeattackRecv(aRecv, aIndex, FALSE);
				break;
			case 0x1E:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGDurationMagicRecv((PMSG_DURATION_MAGIC_RECV *)aRecv, aIndex);
				}
				break;
			case 0x22:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGItemGetRequest((PMSG_ITEMGETREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x23:
				CGItemDropRequest((PMSG_ITEMTHROW *)aRecv, aIndex, 0);
				break;
			case 0x24:
				CGInventoryItemMove((PMSG_INVENTORYITEMMOVE *)aRecv, aIndex);
				break;
			case 0x26:
				CGUseItemRecv((PMSG_USEITEM *)aRecv, aIndex);
				break;
			case 0x30:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTalkRequestRecv((PMSG_TALKREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x31:
				CGCloseWindow(aIndex);
				break;
			case 0x32:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGBuyRequestRecv((PMSG_BUYREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x33:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGSellRequestRecv((PMSG_SELLREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x34:
				CGModifyRequestItem((PMSG_ITEMDURREPAIR *)aRecv, aIndex);
				break;
			case 0x36:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTradeRequestSend((PMSG_TRADE_REQUEST *)aRecv, aIndex);
				}
				break;
			case 0x37:
				CGTradeResponseRecv((PMSG_TRADE_RESPONSE *)aRecv, aIndex);
				break;
			case 0x3A:
				CGTradeMoneyRecv((PMSG_TRADE_GOLD *)aRecv, aIndex);
				break;
			case 0x3C:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTradeOkButtonRecv((PMSG_TRADE_OKBUTTON *)aRecv, aIndex);
				}
				break;
			case 0x3D:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTradeCancelButtonRecv(aIndex);
				}
				break;
			case 0x3F:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGPShopReqSetItemPrice((PMSG_REQ_PSHOP_SETITEMPRICE *)aRecv, aIndex);
							break;
						case 0x02:
							CGPShopReqOpen((PMSG_REQ_PSHOP_OPEN *)aRecv, aIndex);
							break;
						case 0x03:
							CGPShopReqClose(aIndex);
							break;
						case 0x05:
							CGPShopReqBuyList((PMSG_REQ_BUYLIST_FROM_PSHOP *)aRecv, aIndex);
							break;
						case 0x06:
							CGPShopReqBuyItem((PMSG_REQ_BUYITEM_FROM_PSHOP *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x40:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGPartyRequestRecv((PMSG_PARTYREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x41:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGPartyRequestResultRecv((PMSG_PARTYREQUESTRESULT *)aRecv, aIndex);
				}
				break;
			case 0x42:
				CGPartyList(aIndex);
				break;
			case 0x43:
				if ( PacketCheckTime(&gObj[aIndex]) == TRUE )
				{
					CGPartyDelUser((PMSG_PARTYDELUSER *)aRecv, aIndex);
				}
				break;
			case 0x50:
				CGGuildRequestRecv((PMSG_GUILDJOINQ *)aRecv, aIndex);
				break;
			case 0x51:
				CGGuildRequestResultRecv((PMSG_GUILDQRESULT *)aRecv, aIndex);
				break;
			case 0x52:
				CGGuildListAll(aIndex);
				break;
			case 0x53:
				CGGuildDelUser((PMSG_GUILDDELUSER *)aRecv, aIndex);
				break;
			case 0x54:
				CGGuildMasterAnswerRecv((PMSG_GUILDMASTERANSWER *)aRecv, aIndex);
				break;
			case 0x55:
				CGGuildMasterInfoSave(aIndex, (PMSG_GUILDINFOSAVE *)aRecv);
				break;
			case 0x57:
				CGGuildMasterCreateCancel(aIndex);
				break;
			case 0x61:
				GCGuildWarRequestSendRecv((PMSG_GUILDWARSEND_RESULT *)aRecv, aIndex);
				break;
			case 0x66:
				GCGuildViewportInfo((PMSG_REQ_GUILDVIEWPORT *)aRecv, aIndex);
				break;
			case 0xB1:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					
					switch ( lpDef->subcode )
					{
						case 0x01:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CGReqMapSvrAuth((PMSG_REQ_MAPSERVERAUTH *)aRecv, aIndex);
							}
							break;
					}
				}
				break;
			case 0xB2:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqCastleSiegeState((PMSG_REQ_CASTLESIEGESTATE *)aRecv, aIndex);
							break;
						case 0x01:
							CGReqRegCastleSiege((PMSG_REQ_REGCASTLESIEGE *)aRecv, aIndex);
							break;
						case 0x02:
							CGReqGiveUpCastleSiege((PMSG_REQ_GIVEUPCASTLESIEGE *)aRecv, aIndex);
							break;
						case 0x03:
							CGReqGuildRegInfo((PMSG_REQ_GUILDREGINFO *)aRecv, aIndex);
							break;
						case 0x04:
							CGReqRegGuildMark((PMSG_REQ_REGGUILDMARK *)aRecv, aIndex);
							break;
						case 0x05:
							CGReqNpcBuy((PMSG_REQ_NPCBUY *)aRecv, aIndex);
							break;
						case 0x06:
							CGReqNpcRepair((PMSG_REQ_NPCREPAIR *)aRecv, aIndex);
							break;
						case 0x07:
							CGReqNpcUpgrade((PMSG_REQ_NPCUPGRADE *)aRecv, aIndex);
							break;
						case 0x08:
							CGReqTaxMoneyInfo((PMSG_REQ_TAXMONEYINFO *)aRecv, aIndex);
							break;
						case 0x09:
							CGReqTaxRateChange((PMSG_REQ_TAXRATECHANGE *)aRecv, aIndex);
							break;
						case 0x10:
							CGReqMoneyDrawOut((PMSG_REQ_MONEYDRAWOUT *)aRecv, aIndex);
							break;
						case 0x12:
							CGReqCsGateOperate((PMSG_REQ_CSGATEOPERATE *)aRecv, aIndex);
							break;
						case 0x1B:
							CGReqCsMiniMapData((PMSG_REQ_MINIMAPDATA *)aRecv, aIndex);
							break;
						case 0x1C:
							CGReqStopCsMiniMapData((PMSG_REQ_STOPMINIMAPDATA *)aRecv, aIndex);
							break;
						case 0x1D:
							CGReqCsSendCommand((PMSG_REQ_CSCOMMAND *)aRecv, aIndex);
							break;
						case 0x1F:
							CGReqCsSetEnterHuntZone((PMSG_REQ_CSHUNTZONEENTER *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xB3:
				CGReqNpcDbList((PMSG_REQ_NPCDBLIST *)aRecv, aIndex);
				break;
			case 0xB4:
				CGReqCsRegGuildList((PMSG_REQ_CSREGGUILDLIST *)aRecv, aIndex);
				break;
			case 0xB5:
				CGReqCsAttkGuildList((PMSG_REQ_CSATTKGUILDLIST *)aRecv, aIndex);
				break;
			case 0xB7:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGReqWeaponUse((PMSG_REQ_USEWEAPON *)aRecv, aIndex);
							break;
						case 0x04:
							CGReqWeaponDamageValue((PMSG_REQ_WEAPON_DAMAGE_VALUE *)aRecv, aIndex);
							break;
					}
				}
				break;
				// ----
			case 0xB9:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x02:
							CGReqGuildMarkOfCastleOwner((PMSG_REQ_GUILDMARK_OF_CASTLEOWNER *)aRecv, aIndex);
							break;
							//#if(_GSCS==1)
						case 0x05:
							CGReqCastleHuntZoneEntrance((PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE *)aRecv, aIndex);
							break;
							//#endif
					}
				}
				break;
			case 0xBC:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqJewelMix((PMSG_REQ_JEWEL_MIX *)aRecv, aIndex);
							break;
						case 0x01:
							CGReqJewelUnMix((PMSG_REQ_JEWEL_UNMIX *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xBD:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqCrywolfInfo((PMSG_REQ_CRYWOLF_INFO *)aRecv, aIndex);
							break;
						case 0x03:
							CGReqAlatrContract((PMSG_REQ_CRYWOLF_ALTAR_CONTRACT *)aRecv, aIndex);
							break;
						case 0x09:
							CGReqPlusChaosRate((PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xE1:
				CGGuildAssignStatus((PMSG_GUILD_ASSIGN_STATUS_REQ *)aRecv, aIndex);
				break;
			case 0xE2:
				CGGuildAssignType((PMSG_GUILD_ASSIGN_TYPE_REQ *)aRecv, aIndex);
				break;
			case 0xE5:
				CGRelationShipReqJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ *)aRecv, aIndex);
				break;
			case 0xE6:
				CGRelationShipAnsJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS *)aRecv, aIndex);
				break;
			case 0xE9:
				CGUnionList((PMSG_UNIONLIST_REQ *)aRecv, aIndex);
				break;
			case 0xEB:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGRelationShipReqKickOutUnionMember((PMSG_KICKOUT_UNIONMEMBER_REQ *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x71:
				GCPingSendRecv((PMSG_PING_RESULT *)aRecv, aIndex);
				break;
			/*case 0x72:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					GCPacketCheckSumRecv((PMSG_PACKETCHECKSUM *)aRecv, aIndex);
				}
				break;
			case 0x73:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					GCNPggCheckSumRecv((PMSG_NPROTECTGGCHECKSUM *)aRecv, aIndex);
				}
				break;*/
			case 0x81:
				CGWarehouseMoneyInOut(aIndex, (PMSG_WAREHOUSEMONEYINOUT *)aRecv);
				break;
			case 0x82:
				CGWarehouseUseEnd(aIndex);
				break;
			case 0x83:
				GCWarehouseRecivePassword(aIndex, (PMSG_WAREHOUSEPASSSEND *)aRecv);
				break;
			case 0x86:
				CGChaosBoxItemMixButtonClick((PMSG_CHAOSMIX *)aRecv, aIndex);
				break;
			case 0x87:
				CGChaosBoxUseEnd(aIndex);
				break;
			case 0x8E://SEASON 5 PACKET
				CGMapMoveRecv((PMSG_MAP_MOVE *)aRecv, aIndex);
				break;
			case 0x90:
				GCReqmoveDevilSquare((PMSG_REQ_MOVEDEVILSQUARE *)aRecv, aIndex);
				break;
			case 0x91:
				GCReqDevilSquareRemainTime((PMSG_REQ_DEVILSQUARE_REMAINTIME *)aRecv, aIndex);
				break;
			case 0x95:
				GCRegEventChipRecv((PMSG_REGEVENTCHIP *)aRecv, aIndex);
				break;
			case 0x96:
				GCGetMutoNumRecv((PMSG_GETMUTONUMBER *)aRecv, aIndex);
				break;
			case 0x97:
				GCUseEndEventChipRescv(aIndex);
				break;
			case 0x98:
				GCUseRenaChangeZenRecv((PMSG_EXCHANGE_EVENTCHIP *)aRecv, aIndex);
				break;
			case 0x99:
				CGReqMoveOtherServer((PMSG_REQ_MOVE_OTHERSERVER *)aRecv, aIndex);
				break;
			case 0xA0:
				CGRequestQuestInfo(aIndex);
				break;
			case 0xA2:
				CGSetQuestState((PMSG_SETQUEST *)aRecv, aIndex);
				break;
			case 0xA7:
				CGRequestPetItemCommand((PMSG_REQUEST_PET_ITEM_COMMAND *)aRecv, aIndex);
				break;
			case 0xA9:
				CGRequestPetItemInfo((PMSG_REQUEST_PET_ITEMINFO *)aRecv, aIndex);
				break;
#ifdef NPVP
			case 0xAA://1.00.93
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					WORD wPacketType = MAKEWORD(lpDef->subcode,0xAA);
					// ----
					switch(wPacketType)
					{
					case MSG_DUEL_INVITE :
						{
							LPPMSG_REQ_DUEL_INVITE lpReq = (LPPMSG_REQ_DUEL_INVITE)aRecv;
							int nRet = OnCGInviteDuel(lpReq, aIndex);
							if( nRet != ENEWPVP::E_NO_ERROR )
							{
								PMSG_ANS_DUEL_INVITE res = {0};
								res.h.c = PMHC_BYTE;
								res.h.size = sizeof(PMSG_ANS_DUEL_INVITE);
								res.h.headcode = 0xAA;
								res.h.subcode = 0x01;
								res.nResult = nRet;
								DataSend(aIndex, (LPBYTE)&res, res.h.size);
							}
						}
						break;
					case MSG_DUEL_ANSWER :
						{
							LPPMSG_ANS_DUEL_ANSWER lpReq = (LPPMSG_ANS_DUEL_ANSWER)aRecv;
							int nRet = OnCGAnswerDuel(lpReq, aIndex);
							/*if( nRet != ENEWPVP::E_NO_ERROR )
							{
							}*/
						}
						break;
					case MSG_DUEL_LEAVE :
						{
							LPPMSG_REQ_DUEL_EXIT lpReq = (LPPMSG_REQ_DUEL_EXIT)aRecv;
							int nRet = OnCGLeaveDuel(lpReq, aIndex);
							if( nRet != ENEWPVP::E_NO_ERROR )
							{
								PMSG_ANS_DUEL_EXIT res = {0};
								res.h.c = PMHC_BYTE;
								res.h.size = sizeof(PMSG_ANS_DUEL_EXIT);
								res.h.headcode = 0xAA;
								res.h.subcode = 0x03;
								res.nResult = nRet;
								DataSend(aIndex, (LPBYTE)&res, res.h.size);
							}
						}
						break;
					case MSG_DUEL_JOINCNANNEL :
						{
							int nRet = OnDuelChannelJoin((LPPMSG_REQ_DUEL_JOINCNANNEL)aRecv, aIndex);
							if( nRet != ENEWPVP::E_NO_ERROR )
							{
								PMSG_ANS_DUEL_JOINCNANNEL res = {0};
								res.h.c = PMHC_BYTE;
								res.h.size = sizeof(PMSG_ANS_DUEL_JOINCNANNEL);
								res.h.headcode = 0xAA;
								res.h.subcode = 0x07;
								res.nResult = nRet;
								DataSend(aIndex, (LPBYTE)&res, res.h.size);
							}
						}
						break;
					case MSG_DUEL_LEAVECNANNEL :
						{
							LPPMSG_REQ_DUEL_LEAVECNANNEL lpReq = (LPPMSG_REQ_DUEL_LEAVECNANNEL)aRecv;
							int nRet = OnDuelChannelLeave(lpReq, aIndex);
							if( nRet != ENEWPVP::E_NO_ERROR )
							{
								PMSG_ANS_DUEL_LEAVECNANNEL res = {0};
								res.h.c = PMHC_BYTE;
								res.h.size = sizeof(PMSG_ANS_DUEL_LEAVECNANNEL);
								res.h.headcode = 0xAA;
								res.h.subcode = 0x09;
								res.nResult = nRet;
								DataSend(aIndex, (LPBYTE)&res, res.h.size);
							}
						}
						break;
					}
				}
				break;
#endif
			case 0x9A:
				CGRequestEnterBloodCastle((PMSG_REQ_MOVEBLOODCASTLE*)aRecv, aIndex);
				break;
			case 0x9B:
				//#error LAcking PACKET HERE
				break;
			case 0x9F:
				CGRequestEventEnterCount((PMSG_REQ_CL_ENTERCOUNT *)aRecv, aIndex);
				break;
			case 0x9D:
				CGRequestLottoRegister((PMSG_REQ_2ANV_LOTTO_EVENT *)aRecv, aIndex);
				break;
			case 0xAF:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGRequestEnterChaosCastle((PMSG_REQ_MOVECHAOSCASTLE *)aRecv, aIndex);
							break;
						case 0x02:
							CGRequestRepositionUserInChaosCastle((PMSG_REQ_REPOSUSER_IN_CC *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xC0:
				FriendListRequest(aIndex);
				break;
			case 0xC1:
				FriendAddRequest((PMSG_FRIEND_ADD_REQ *)aRecv, aIndex);
				break;
			case 0xC2:
				WaitFriendAddRequest((PMSG_FRIEND_ADD_SIN_RESULT *)aRecv, aIndex);
				break;
			case 0xC3:
				FriendDelRequest((PMSG_FRIEND_DEL_REQ *)aRecv, aIndex);
				break;
			case 0xC4:
				FriendStateClientRecv((PMSG_FRIEND_STATE_C *)aRecv, aIndex);
				break;
			case 0xC5:
				FriendMemoSend((PMSG_FRIEND_MEMO *)aRecv, aIndex);
				break;
			case 0xC7:
				FriendMemoReadReq((PMSG_FRIEND_READ_MEMO_REQ *)aRecv, aIndex);
				break;
			case 0xC8:
				FriendMemoDelReq((PMSG_FRIEND_MEMO_DEL_REQ *)aRecv, aIndex);
				break;
			case 0xC9:
				FriendMemoListReq((FHP_FRIEND_MEMO_LIST *)aRecv, aIndex);//FriendMemoListReq(aIndex);
				break;
			case 0xCA:
				FriendChatRoomCreateReq((PMSG_FRIEND_ROOMCREATE_REQ *)aRecv, aIndex);
				break;
			case 0xCB:
				FriendRoomInvitationReq((PMSG_ROOM_INVITATION *)aRecv, aIndex);
				break;
#ifdef HACKCONTROL
			case 0x76:
				{
					g_CheatGuard.GetCheckResult((CHEATGUARD_ANS_CHECK*)aRecv, aIndex);
				}
				break;
#endif
			case 0xD0:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					
					switch ( lpDef->subcode )
					{
#ifdef PCBANG
						case 0x05:
							CGReqPCBangShopBuy((PMSG_REQ_PCBANG_SHOP_BUY *)aRecv, aIndex);
							break;
						case 0x06:
							CGReqPCBangShopOpen((PMSG_REQ_PCBANG_SHOP_OPEN *)aRecv, aIndex);
							break;
#endif
						case 0x07:
							CGReqWerewolfMove((PMSG_REQ_WEREWOLF_MOVE *)aRecv, aIndex);
							break;
						case 0x08:
							CGReqGatekeeperMove((PMSG_REQ_GATEKEEPER_MOVE *)aRecv, aIndex);
							break;
						case 0x10:
							CGReqXMasSetPayItem((PMSG_REQ_XMAS_PAYITEM *)aRecv, aIndex);
							break;
						case 0x0A:
							CGReqXMasToDeviasMapMove((PMSG_REQ_XMASNPCMAP_MOVE *)aRecv, aIndex);
							break;
					}
				}
				break;

			case 0xD1:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqKanturuStateInfo((PMSG_REQ_KANTURU_STATE_INFO *)aRecv, aIndex);
							break;
						case 0x01:
							GCReqEnterKanturuBossMap((PMSG_REQ_ENTER_KANTURU_BOSS_MAP *)aRecv, aIndex);
							break;
					}
				}
				break;
				// --
#ifdef OLDCASHSHOP
			case 0xF5:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							g_CashShop.CGCashShopOpen(lpObj, (PMSG_REQ_CASHSHOPOPEN *)aRecv);
							break;
						case 0x03:
							g_CashShop.CGCashPoint(lpObj);
							break;
						case 0x05:
							g_CashShop.GCCashItemListSend(&gObj[aIndex], (PMSG_REQ_CASHITEMLIST *)aRecv);
							break;
						case 0x07:
							g_CashShop.CGCashItemBuy(&gObj[aIndex], (PMSG_REQ_CASHITEM_BUY *)aRecv);
							break;
					}
				}
				break;
				// --
#endif
#ifdef GAMESHOP
				case 0xD2:	//-> CashShop
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					// ----
#if( GAMESHOP_DEBUG == 1 )
					LogAddC(2, "[DEBUG] [0x%X::0x%X] CashShop request", protoNum, lpDef->subcode);
#endif
					// ----
					switch(lpDef->subcode)
					{
					case 0x01:
						{
							gGameShop.RequestPoint(aIndex);
							//RequestCashPoint
							//-> 006b3930
						}
						break;
						// --
					case 0x02:
						{
							gGameShop.RequestOpenShop(aIndex, (GAMESHOP_REQ_OPEN*)aRecv);
							//RequestCashShopOpen
							//-> 006b3b10
						}
						break;
						// --
					case 0x03:
						{
							gGameShop.RequestItemBuy(aIndex, (GAMESHOP_REQ_BUY*)aRecv);
							//RequestBuyItem
							//-> 006b3d60
						}
						break;
						// --
					case 0x04:
						{
							gGameShop.RequestPoint(aIndex);
							//RequestCashPoint
							//-> 006b3d60
						}
						break;
						// --
					case 0x05:
						{
							gGameShop.RequestStorage(aIndex, (GAMESHOP_REQ_STORAGE*)aRecv);
							//RequestStorageList
							//-> 006b39b0
						}
						break;
						// --
					case 0x07:
						{
							//RequestGiftCash
							//-> 006b4350
						}
						break;
						// --
					case 0x08:
						{
							//RequestItemBuyConfirm
							//-> 006b4450
						}
						break;
						// --
					case 0x09:
						{
							//RequestItemLeftCount
							//-> 006b44b0
						}
						break;
						// --
					case 0xA:
						{
							//RequestStorageItemThrow
							//-> 006b4510
						}
						break;
						// --
					case 0xB:
						{
							gGameShop.RequestStorageItemUse(aIndex, (GAMESHOP_REQ_STORAGEITEMUSE*)aRecv);
							//RequestStorageItemUse
							//-> 006b4600
						}
						break;
						// --
					case 0x13:
						{
							//RequestEventProductList
							//-> 006b4c20
						}
						break;
					}
				}
				break;
#endif
				// --
#ifdef WZQUEST
          case 0xF6:
                {
                    PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					LogAddC(2, "[DEBUG] Quest: 0x%x", lpDef->subcode);
                    switch ( lpDef->subcode )
                    {
                    case 0x30:
                        CGReqQuestExp((PMSG_REQ_NPC_QUESTEXP *)aRecv, aIndex);
                        break;
                    case 0x31:
                        CGReqAttDefPowerInc((PMSG_REQ_ATTDEF_POWER_INC *)aRecv, aIndex);
                        break;
                    case 0x21:
                        CGReqEventItemQuestList((PMSG_REQ_EVENT_ITEM_EP_LIST *)aRecv, aIndex);
                        break;
                    case 0xA:
                        CGReqQuestSwitch((PMSG_REQ_QUESTEXP *)aRecv, aIndex);
                        break;
                    case 0xB:
                        CGReqQuestProgress((PMSG_REQ_QUESTEXP *)aRecv, aIndex);
                        break;
                    case 0xD:
                        CGReqQuestComplete((PMSG_REQ_QUESTEXP_COMPLETE *)aRecv, aIndex);
                        break;
                    case 0xF:
                        CGReqQuestGiveUp((PMSG_REQ_QUESTEXP_GIVEUP *)aRecv, aIndex);
                        break;
                    case 0x1A:
                        CGReqProgressQuestList((PMSG_REQ_QUESTEXP_PROGRESS_LIST *)aRecv, aIndex);
                        break;
                    case 0x1B:
                        CGReqProgressQuestInfo((PMSG_REQ_QUESTEXP_PROGRESS_INFO *)aRecv, aIndex);
                        break;
                    case 0x10:
                        CGReqTutorialKeyComplete((PMSG_REQ_QUESTEXP_ASK_COMPLETE *)aRecv, aIndex);
                        break;
                    }
                }
                break;
#endif
				// --
#ifdef IMPERIAL
			case 0xF7:
				{
					CGReqEnterZone((PMSG_REQ_ENTERZONE *)aRecv, aIndex);
				}
				break;
#endif
				// --
#ifdef GENS
			case 0xF8:	//-> Gens System
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					// ----
					//LogAddC(2, "[DEBUG] [0x%X::0x%X] Gens System request", protoNum, lpDef->subcode);
					// ----
					switch(lpDef->subcode)
					{
					case 0x01:
						{
							CGReqRegGensMember((PMSG_REQ_REG_GENS_MEMBER*)aRecv, aIndex);
						}
						break;
						// --
					case 0x03:
						{
							CGReqSecedeGensMember((PMSG_REQ_SEGEDE_GENS_MEMBER*)aRecv, aIndex);
						}
						break;
						// --
					case 0x09:
						{
							CGReqGensReward((PMSG_GENS_REWARD_CODE*)aRecv, aIndex);
						}
						break;
						// --
					case 0x0B:
						{
							CGReqGensMemberInfo((PMSG_REQ_GENS_INFO*)aRecv, aIndex);
						}
						break;
					}
				}
				break;
#endif
#ifdef SEASON6DOT3_ENG
			case 0x4A:
				{
					GCMonkMagicAttack((PMSG_MAGICATTACK *)aRecv, aIndex);
				}
				break;
			case 0x4B:
				{
					GCMonkDarkSideTargetSelect((PMSG_MONK_DARKSIDE_RECV *)aRecv, aIndex);
				}
				break;
#endif
#ifdef MUHELPER
			case 0xAE:
				{
					g_MUHelper.SaveMacro(aIndex, (MUHELPER_MACRO_CLIENT*)aRecv);
				}
				break;
#endif
				// --
			case 0xBF:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					
					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqEnterIllusionTemple((PMSG_ANS_ILLUSIONTEMPLE_ENTER *)aRecv, aIndex);
							break;
						case 0x02:
							CGReqUseIllusionTempleKillCntSkill((PMSG_USE_ILLUSIONTEMPLE_KILLCOUNT_SKILL *)aRecv, aIndex);
							break;
						case 0x05:
							CGReqIllusionTempleDropReward((PMSG_ILLUSIONTEMPLE_DROP_REWARD *)aRecv, aIndex);
							break;
						case 0x0B:
							CGReqRegLuckyCoinItemCount((PMSG_REQ_REG_LUCKYCOIN *)aRecv, aIndex);
							break;
						case 0x0C:
							CGSearchLuckyCoinItem((PMSG_REQ_SEARCH_LUCKY_COIN *)aRecv, aIndex);
							break;
						case 0x0D:
							CGReqLuckyCoinTrade((PMSG_REQ_TRADE_LUCKYCOIN *)aRecv, aIndex);
							break;
#ifdef DP
						case 0x0E:	//1.01.00
							CGReqEnterDoppelganger((PMSG_REQ_ENTER_DOPPELGANGER*)aRecv, aIndex);
							break;
#endif
						case 0x17: //1.01.00
							CGMoveRorenMarket(aIndex); 
							break;

						case 0x20:	//1.01.03
							CGInventoryEquipment((_tagPMSG_REQ_INVENTORY_EQUIPMENT_ITEM *)aRecv, aIndex);
							break;
#ifdef MUHELPER
						case 0x51:
							g_MUHelper.Manager(aIndex, (MUHELPER_REQ_ACTION*)aRecv);
							break;
#endif
					}
				}
				break;
				// --
			case 0x8A:	//1.01.03
				CGAnsHacktoolStatistics((PMSG_ANS_HACKTOOL_STATISTICS*)aRecv, aIndex);
				break;
				// --
			/*case 0xAE:
				CGRequestMacroSave((PMSG_MACRODATA *)aRecv, aIndex);
				break;*/
				// --
			case 0xFB:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					// ----
					switch(lpDef->subcode)
					{
#ifdef __NOVUS__
					case 1:
						{
							g_CraftSystem.FinishQuestReq(&gObj[aIndex]);
						}
						break;
#endif
						// --
#ifdef __CUSTOMS__
					case 6:
						{
							g_ResetSystem.FinishResetReq(&gObj[aIndex]);
						}
						break;
						// --
					case 7:
						{
							TargetDataRequest(aIndex, (PMSG_TARGETDATA_REQ*)aRecv);
						}
						break;
#endif
#ifdef __MAKELO__
					case 11:
						{
							g_OfflineAttack.StartingAttack(aIndex, (OFFLINEATTACK_REQ*)aRecv);
						}
						break;
#endif
						// --
#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
					case 12:
						{
							g_ConnectEx.SendLogin(aIndex, (CONNECTEX_LOGIN*)aRecv);
						}
						break;
#endif
					}
				}
				break;
				// --
#ifdef __CUSTOMS__
			case 0xFC:
				{
					DisconnectEx((DISCONNECTEX*)aRecv);
				}
				break;
				// --
			case 0xFD:
				{
					g_NewsBoard.OpenMain(&gObj[aIndex]);
				}
				break;
				// --
			case 0xFE:
				{
					g_NewsBoard.OpenItem(&gObj[aIndex], (NEWS_REQ_NEWS*)aRecv);
				}
				break;
				// --
#endif
			default:
				LogAddC(2, "error-L2 : account:%s name:%s HEAD:%x (%s,%d) State:%d", gObj[aIndex].AccountID, gObj[aIndex].Name, protoNum, __FILE__, __LINE__, gObj[aIndex].Connected);
				CloseClient(aIndex);
		}	
	}
}

void TestSend() 
{
	PMSG_TEST pMsg;
	int size = sizeof(pMsg);
	pMsg.h.c = 0xC2;
	pMsg.h.headcode = 0xFF;
	pMsg.h.sizeH = SET_NUMBERH(size);
	pMsg.h.sizeL = SET_NUMBERL(size);
	DataSend( tempindex, (LPBYTE)&pMsg, size);
	DataSend( tempindex, (LPBYTE)&pMsg, size);
	LogAdd("TEST Data Send");
}

void MsgSendV2(LPOBJ lpObj, LPBYTE Msg, int size) 
{
	for ( int n=0;n<MAX_VIEWPORT;n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state != 0 )
			{
				DataSend(lpObj->VpPlayer2[n].number, Msg, size);
			}
		}
	}
}

void CGLiveClient(PMSG_CLIENTTIME * lpClientTime, short aIndex) 
{
	if ( gObj[aIndex].Connected > PLAYER_CONNECTED )
	{
		gObj[aIndex].ConnectCheckTime = GetTickCount();
	}

#if(TESTSERVER==1)
	return;
#endif

#ifdef ANTIHACKTOOL
	if ( gObj[aIndex].CheckSpeedHack != false && g_CheatGuard.m_SpeedHackCheck )
	{
		int systemtick	= GetTickCount() - gObj[aIndex].CheckTick2;
		int usertick	= lpClientTime->Time - gObj[aIndex].CheckTick;
		int checktime	= systemtick - usertick;

		if( checktime < -7000 )
		{
			g_CheatGuard.WriteLog("[%s][%s] [SpeedHack] %d", gObj[aIndex].AccountID, gObj[aIndex].Name, checktime);
			g_CheatGuard.Disconnect(aIndex);
		}

		gObj[aIndex].CheckTickCount++;

		if( gObj[aIndex].CheckTickCount > 45 )
		{
			gObj[aIndex].CheckTick		= lpClientTime->Time;
			gObj[aIndex].CheckTickCount = 0;
		}
	}
#endif

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		if(g_bAbilityDebug == 1)
		{
			char szTemp[256];
			sprintf(szTemp, "AttackSpeed: %d,%d MagicSpeed: %d,%d", gObj[aIndex].m_AttackSpeed, lpClientTime->AttackSpeed, gObj[aIndex].m_MagicSpeed, lpClientTime->MagicSpeed);
			GCServerMsgStringSend(szTemp, gObj[aIndex].m_Index, 1);
		}
#ifdef ANTIHACKTOOL
#pragma message("#### UNCOMMENT ME #####")
		/*if ( gObj[aIndex].m_AttackSpeed < lpClientTime->AttackSpeed || gObj[aIndex].m_MagicSpeed < lpClientTime->MagicSpeed )
		{
			LogAddC(2, "[%s][%s][%s] ClientHack Detected : Editing AttackSpeed [%d][%d] [%d][%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr, gObj[aIndex].m_AttackSpeed, lpClientTime->AttackSpeed, gObj[aIndex].m_MagicSpeed, lpClientTime->MagicSpeed);
			LogAddC(2, "[%s][%s][%s] ClientHack Detected CheckSpeedHack : [%s]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr,(g_bCheckSpeedHack != FALSE) ? "TRUE" : "FALSE");
	
			if(g_bCheckSpeedHack == FALSE)
			{
				return;
			}

			gObj[aIndex].m_AttackSpeedHackDetectedCount++;

			if ( gObj[aIndex].m_AttackSpeedHackDetectedCount > 3 )
			{
				CloseClient(aIndex);
			}
		}
		else
		{
			gObj[aIndex].m_AttackSpeedHackDetectedCount = 0;
		}*/
#endif

		if ( szAuthKey[10] != AUTHKEY10 )
		{
			DestroyGIocp();
		}

		if ( szAuthKey[11] != AUTHKEY11 )
		{
			DestroyGIocp();
		}
	}
}

void GCCheckMainExeKeySend(int aIndex) 
{
	PMSG_CHECK_MAINEXE pMsg;

	gObj[aIndex].CheckSumTableNum = rand() % MAX_CHECKSUM_KEY;
	gObj[aIndex].CheckSumTime = GetTickCount();
	PHeadSetB((LPBYTE)&pMsg, 0x03, sizeof(pMsg));
	pMsg.m_wKey = EncryptCheckSumKey(gObj[aIndex].CheckSumTableNum);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

WORD EncryptCheckSumKey(WORD wSource) 
{
	WORD wRandom = rand() % 64;
	WORD wAcc = ((wSource & 0x3F0) * 64) | (wRandom *16) | (wSource & 0x0F);
	return wAcc ^ 0xB479;
}


void CGCheckMainRecv(PMSG_CHECK_MAINEXE_RESULT * lpMsg, int aIndex) 
{

	if(gMainCheckSumCheck == 0)
	{
		gObj[aIndex].CheckSumTime = 0;
		gObj[aIndex].m_InWebzen = true;
		return;
	}
	else
	{
		if ( strcmp(gObj[aIndex].Ip_addr, gMainAdminIPAddRess) == 0 )
		{
			gObj[aIndex].CheckSumTime = 0;
			gObj[aIndex].m_InWebzen = true;
			return;
		}

		if ( gObj[aIndex].CheckSumTableNum < 0 || gObj[aIndex].CheckSumTableNum > MAX_CHECKSUM_KEY-1 )
		{
			LogAdd("error-L1 : [%s][%s] CheckSum-Exe error ", gObj[aIndex].AccountID, gObj[aIndex].Name);
			CloseClient(aIndex);
			return;
		}

		if ( dwgCheckSum[gObj[aIndex].CheckSumTableNum] != lpMsg->m_dwKey)
		{
			LogAddTD("error-L1 : CheckSum-Exe error %d %d %d [%s]", dwgCheckSum[gObj[aIndex].CheckSumTableNum],
				lpMsg->m_dwKey, gObj[aIndex].CheckSumTableNum, gObj[aIndex].AccountID);
			CloseClient(aIndex);
			return;
		}

		gObj[aIndex].CheckSumTime = 0;
	}
}

void PEchoProc(LPBYTE aMsg, int aLen, short aIndex) 
{
	for ( int n = 0 ; n< OBJMAX ; n++)
	{
		if ( gObj[n].Connected >= PLAYER_CONNECTED )
		{
			DataSend(n, aMsg, aLen);
		}
	}
}

void GCResultSend(int aIndex, BYTE headcode, BYTE result) 
{
	PMSG_DEFRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, headcode, sizeof(pResult));
	pResult.result = result;
	
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

void ChatSend(LPOBJ lpObj, char* szChat) 
{
	int len = strlen(szChat);

	if ( len < 1 )
		return;

	if ( len > MAX_CHAT_LEN-1 )
		return;

	PMSG_CHATDATA_NUMBER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szChat);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void vChatSend(LPOBJ lpObj, char* szChat, ...) 
{
	char szBuffer[256] = "";
	va_list pArguments;

	va_start(pArguments, szChat);
	vsprintf(szBuffer, szChat, pArguments);
	va_end(pArguments);

	int len = strlen(szBuffer);

	if ( len < 1 )
		return;

	if ( len > MAX_CHAT_LEN-1 )
		return;


	PMSG_CHATDATA_NUMBER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szBuffer);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void AllSendServerMsg( char* chatmsg) 
{
	PMSG_NOTICE pNotice;
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, 0x00,  chatmsg);

	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				DataSend(n, (LPBYTE)&pNotice , pNotice.h.size );
			}
		}
	}
}

void DataSendAll(LPBYTE lpMsg, int iMsgSize) 
{			
	for ( int n = OBJ_STARTUSERINDEX ; n < OBJMAX ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				DataSend(n, (LPBYTE)lpMsg , iMsgSize );
			}
		}
	}
}

void ChatTargetSend(LPOBJ lpObj, char * szChat, int senduser) 
{
	int len = strlen(szChat);

	if ( len < 1 || len > MAX_CHAT_LEN-1)
		return;


	PMSG_CHATDATA_NUMBER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szChat);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.chatmsg[len+1] = 0;

	DataSend(senduser, (LPBYTE)&pMsg, pMsg.h.size);
}

void PChatProc(PMSG_CHATDATA * lpChat, short aIndex) 
{
	int n;
	LPOBJ lpObj = &gObj[aIndex];
	int number;
	int slen = strlen(lpChat->chatmsg);
	// ----
	if ( slen < 1 )
		return;
	// ----
	if ( slen > MAX_CHAT_LEN-1 )
	{
		LogAddTD("[Anti-HACK][PChatProc][%s][%s] Chat Message Len : %d", lpObj->AccountID, lpObj->Name, slen);
		return;
	}
	// ----
	char szId[MAX_ACCOUNT_LEN+1];
	szId[MAX_ACCOUNT_LEN] = 0;
	CHAT_LOG_DATA pChatMsg;

	memcpy(szId, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	memcpy(lpChat->chatid, szId, MAX_ACCOUNT_LEN);
	int szTargetNameCount = 0;

#ifdef __CUSTOMS__
	if( g_ChatFilter.CheckText(lpChat->chatmsg) == 0 )
	{
		if( g_ChatFilter.UsePenalty )
		{
			lpObj->ChatLitmitTime += g_ChatFilter.BanTime;
			g_ChatFilterLog.Output("[%s] [%s] (%s) Get chat block +%d [Total: %d]", lpObj->AccountID, lpObj->Name, lpChat->chatmsg, g_ChatFilter.BanTime, lpObj->ChatLitmitTime);
			char Text[100];
			ZeroMemory(Text, 99);
			sprintf(Text, "[ChatFilter] You get +%d minute of ban time", g_ChatFilter.BanTime);
			GCServerMsgStringSend(Text, aIndex, 1);
		}
		// ----
		memcpy(lpChat->chatmsg, "*censored*", MAX_CHAT_LEN-1);
	}
#endif

	switch( lpChat->chatmsg[0] )
	{
		case '!':	// Global Announcement
			if ( slen > 2 )
			{
				if( CheckAuthorityCondition(98, lpObj) )
				{
					DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);
					AllSendServerMsg(&lpChat->chatmsg[1]);

					//LogAddTD(lMsg.Get(MSGGET(1, 215)), gObj[aIndex].AccountID, gObj[aIndex].Name, &lpChat->chatmsg[1]);

					g_GMChatLog.Output("[%s] [%s] %s", lpObj->AccountID, lpObj->Name, &lpChat->chatmsg[1]);
					return;
				}
			}
			break;
		case '/':	// Command
			if ( slen > 2 )
			{
				if(strcmp(&lpChat->chatmsg[1], "이동 칼리마") == 0) //season4 all changed
				{
					if(IsOKPCBangBenefitAll(lpObj) != FALSE) //season4 add-on :)
					{
						if(gMoveCommand.MoveFree2Kalima(lpObj) != FALSE)
						{
							LogAddTD("[%s][%s] Use [/이동 칼리마] PCBANG",lpObj->AccountID, lpObj->Name);
							return;
						}
					}
				}
				
				if( (GetTickCount() - lpObj->MySelfDefenseTime) < 60000 && g_EnableSelfDefense )
				{
					GCServerMsgStringSend(lMsg.Get(1133),lpObj->m_Index,1);
					return;
				}

				cManager.ManagementProc(lpObj, lpChat->chatmsg, aIndex);
				return;
			}
			break;
	}

	// Other Commands
	if ( lpObj->ChatLitmitTime > 0 )
	{
		MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(4, 223)), lpObj->ChatLitmitTime);
		return;
	}

	if ( (lpObj->Penalty&2)==2)
		return;

	// Party Message
	if ( lpChat->chatmsg[0] == '~' || lpChat->chatmsg[0] == ']' )
	{
		if ( lpObj->PartyNumber >= 0 )
		{
			int partycount = gParty.GetPartyCount(lpObj->PartyNumber);

			if ( partycount >= 0 )
			{
				g_PartyChatLog.Output("[%s] [%s] %s", lpObj->AccountID, lpObj->Name, &lpChat->chatmsg[1]);

				for (n=0;n<MAX_USER_IN_PARTY;n++ )
				{
					number = gParty.m_PartyS[lpObj->PartyNumber].Number[n];

					if ( number >= 0 )
					{
						DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
					}
				}
			}
		}
	}
	// Guild
	else if ( lpChat->chatmsg[0] == '@' )
	{
		if ( lpObj->GuildNumber > 0 )
		{
			// Notice
			if ( lpChat->chatmsg[1] == '>' )
			{
				if ( lpObj->Name[0] == lpObj->lpGuild->Names[0][0] )
				{
					if ( !strcmp(lpObj->Name, lpObj->lpGuild->Names[0] ) )
					{
						GDGuildNoticeSave(lpObj->lpGuild->Name, &lpChat->chatmsg[2]);
						LogAdd(lMsg.Get(MSGGET(1, 216)), lpObj->lpGuild->Name, lpChat->chatmsg);
					}
				}
			}
			// Aliances
			else if ( lpChat->chatmsg[1] == '@' && lpObj->lpGuild->iGuildUnion )
			{
				int iGuildCount = 0;
				int iGuildList[MAX_UNION_GUILD] ={0};

				if ( UnionManager.GetGuildUnionMemberList(lpObj->lpGuild->iGuildUnion, iGuildCount, iGuildList) == TRUE )
				{
					g_AllianceChatLog.Output("[%s] [%s] [%s] %s", 
						lpObj->AccountID, lpObj->Name, lpObj->GuildName,  &lpChat->chatmsg[1]);

					for ( int i=0;i<iGuildCount;i++)
					{
						_GUILD_INFO_STRUCT *lpGuildInfo = Guild.SearchGuild_Number(iGuildList[i]);

						if ( !lpGuildInfo )
							continue;

						for (n=0;n<MAX_USER_GUILD;n++)
						{
							if ( lpGuildInfo->Use[n] )
							{
								number = lpGuildInfo->Index[n];

								if ( number >= 0 )
								{
									if ( lpGuildInfo->Names[n][0] == gObj[number].Name[0] )
									{
										if ( !strcmp(lpGuildInfo->Names[n], gObj[number].Name ))
										{
											DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
										}
									}
								}
							}
						}
					}
				}

				if ( g_iServerGroupUnionChatting == TRUE )
					GDUnionServerGroupChattingSend(lpObj->lpGuild->iGuildUnion, lpChat);
			}
			// Just Guild
			else
			{
				int count = lpObj->lpGuild->Count;

				if ( count >= 0 )
				{
					for ( n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpObj->lpGuild->Use[n] )
							{
								number = lpObj->lpGuild->Index[n];

								if ( number >= 0 )
								{
									if ( lpObj->lpGuild->Names[n][0] == gObj[number].Name[0] )
									{
										if ( !strcmp(lpObj->lpGuild->Names[n], gObj[number].Name ) )
										{
											DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
										}
									}
								}
							}
						}
				}

				if ( g_iServerGroupGuildChatting == TRUE )
				{
					if ( lpObj->lpGuild->Count > 1 )
					{
						GDGuildServerGroupChattingSend(lpObj->lpGuild->Number, lpChat);
					}
				}

				g_GuildChatLog.Output("[%s] [%s] [%s] %s", lpObj->AccountID, 
					lpObj->Name, lpObj->GuildName, &lpChat->chatmsg[1]);
			}
		}
	}
#ifdef GENS
	//-> 1.01.00, but names are not compared
	else if( lpChat->chatmsg[0] == '$' )
	{
		if( lpObj->m_GensInfluence == 1 
			|| lpObj->m_GensInfluence == 2 
			&& lpObj->Money >= gGensSystem.ChatPrice )
		{
			if( gGensSystem.ChatPrice > 0 )
			{
				lpObj->Money -= gGensSystem.ChatPrice;
				GCMoneySend(lpObj->m_Index, lpObj->Money);
			}

			g_GensChatLog.Output("[%s] [%s] [%s] %s", lpObj->AccountID, lpObj->Name, 
				gGensSystem.GetGensInfluenceName(lpObj), &lpChat->chatmsg[1]);
			// ----
			for( int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++ )
			{
				LPOBJ GensTarget = &gObj[i];
				// ----
				if( !GensTarget )
				{
					break;
				}
				// ----
				if( GensTarget->Connected == 3 && GensTarget->m_GensInfluence == lpObj->m_GensInfluence )
				{
					DataSend(GensTarget->m_Index, (LPBYTE)lpChat, lpChat->h.size);
				}
			}
		}
		// ----
		//GDGensServerGroupChattingSend(lpObj->m_GensInfluence, lpChat);	//-> Work with ExDB, i think not need...
	}
#endif
	else
	{
		if(((lpObj->Authority&32)==32) && gObjCheckUsedBuffEffect(lpObj, AT_INVISIBILITY) != FALSE) //season 2.5 add-on
		{
			return;
		}

#ifdef NPVP // Need check
		if( g_NewPVP.IsDuel(*lpObj) || g_NewPVP.IsObserver(*lpObj) )
		{
			g_NewPVP.ChatMsgSend(*lpObj, &lpChat->h.c, lpChat->h.size);
		}
		else
		{
			DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);

#ifdef GENS
		if( gGensSystem.IsMapBattleZone(lpObj->MapNumber) )
		{
			gGensSystem.BattleZoneChatMsgSend(lpObj, (LPBYTE)lpChat, lpChat->h.size);
		}
		else
		{
			MsgSendV2(lpObj, (LPBYTE)lpChat, lpChat->h.size);
		}
#else
		MsgSendV2(lpObj, (LPBYTE)lpChat, lpChat->h.size);
#endif

		}
#else
		DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);

#ifdef GENS
		if( gGensSystem.IsMapBattleZone(lpObj->MapNumber) )
		{
			gGensSystem.BattleZoneChatMsgSend(lpObj, (LPBYTE)lpChat, lpChat->h.size);
		}
		else
		{
			MsgSendV2(lpObj, (LPBYTE)lpChat, lpChat->h.size);
		}
#else
		MsgSendV2(lpObj, (LPBYTE)lpChat, lpChat->h.size);
#endif

#endif
	}
}

void CGChatRecv(PMSG_CHATDATA_NUMBER * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( PacketCheckTime(lpObj) == FALSE )
	{
		return;
	}

	DataSend(aIndex, (LPBYTE)lpMsg, lpMsg->h.size);
	MsgSendV2(lpObj, (LPBYTE)lpMsg, lpMsg->h.size);
}

void GCServerMsgSend(BYTE msg, int aIndex) 
{
	PMSG_SERVERMSG pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0C, sizeof(pMsg));
	pMsg.MsgNumber = msg;
	DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}

void GCServerMsgStringSend(LPSTR  szMsg, int aIndex, BYTE type) 
{
	PMSG_NOTICE pNotice;
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);
	DataSend(aIndex, (UCHAR*)&pNotice, pNotice.h.size);
}

void GCServerMsgStringSendEx(int aIndex, BYTE type, char *szFormat, ...)
{
	char szTmp[512];

	va_list arglist;
	va_start(arglist, szFormat);
	vsprintf(szTmp, szFormat, arglist);
	va_end(arglist);

	GCServerMsgStringSend(szTmp, aIndex, type);
}

void GCServerMsgStringSendGuild(_GUILD_INFO_STRUCT* lpNode, LPSTR szMsg, BYTE type) 
{
	if ( lpNode == NULL )
	{
		return;
	}

	for ( int n = 0; n<MAX_USER_GUILD ; n++)
	{
		if (lpNode->Use[n] > 0 && lpNode->Index[n] >= 0 )
		{
			GCServerMsgStringSend((char*)szMsg, lpNode->Index[n], type);
		}
	}
}

void GCEventStateSend(int aIndex, BYTE state, BYTE event) 
{
	PMSG_EVENT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0B, sizeof(pMsg));
	pMsg.Event = event;
	pMsg.State = state;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCMapEventStateSend(int map, BYTE state, BYTE event) 
{
	PMSG_EVENT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0B, sizeof(pMsg));
	pMsg.Event = event;
	pMsg.State = state;

	for ( int n=0; n<OBJMAX ; n++ )
	{
		if ( gObj[n].Connected == PLAYER_PLAYING && gObj[n].Type == OBJ_USER && map == gObj[n].MapNumber )
		{
			DataSend(n, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CGChatWhisperRecv(PMSG_CHATDATA_WHISPER* lpMsg, int aIndex) 
{
	char tid[11];
	PMSG_CHATDATA_WHISPER pWhisper;
	int len;
	LPOBJ lpObj = &gObj[aIndex];
	int index;

	if ( PacketCheckTime(lpObj) == FALSE )
	{
		return;
	}

	if ( lpObj->ChatLitmitTime > 0 )
	{
		MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(4, 223)), lpObj->ChatLitmitTime);
		return;
	}

	if ( (lpObj->Penalty & 2) == 2 )
	{
		return;
	}

	tid[10]=0;
	memcpy(tid, lpMsg->id, sizeof(lpMsg->id));
	index = WhisperCash.AddCash(tid);

	if ( index < 0 )
	{
		GCServerMsgSend(0, aIndex);
		return;
	}

	PHeadSetB((LPBYTE)&pWhisper, 0x02, sizeof(pWhisper));
	strcpy(pWhisper.id, gObj[aIndex].Name);
	memcpy(pWhisper.chatmsg, lpMsg->chatmsg, sizeof(lpMsg->chatmsg));
	len = strlen(pWhisper.chatmsg);

	if ( len > 0 )
	{
		pWhisper.h.size -= sizeof(pWhisper.chatmsg);
		pWhisper.h.size += len + 1;
		// ----
		CHAT_LOG_DATA pChatMsg;
		int szTargetNameCount = 0;
		// ----
#ifdef GENS
		if( gGensSystem.IsMapBattleZone(lpObj->MapNumber) )
		{
			if( gGensSystem.GetGensInfluence(lpObj) == gGensSystem.GetGensInfluence(&gObj[index])
				|| (lpObj->Authority & 32) == 32
				|| (gObj[index].Authority & 32) == 32 )
			{
				DataSend(index, (LPBYTE)&pWhisper, pWhisper.h.size);
			}
		}
		else
		{
			DataSend(index, (LPBYTE)&pWhisper, pWhisper.h.size);
		}
#endif
		g_PrivateChatLog.Output("[%s] [%s] -> [%s] [%s]: %s", 
			lpObj->AccountID, lpObj->Name, 
			gObj[index].AccountID, gObj[index].Name, lpMsg->chatmsg);
	}
}

void SCPJoinResultSend(int aIndex, BYTE result) 
{
	PMSG_JOINRESULT pResult;

	memset(&pResult, 0, sizeof(pResult));

	pResult.h.size = sizeof(pResult);
	pResult.h.c = 0xC1;
	pResult.h.headcode = 0xF1;
	pResult.scode = 0x00;
	pResult.result = result;
	pResult.NumberH = SET_NUMBERH(aIndex);
	pResult.NumberL = SET_NUMBERL(aIndex);
	pResult.CliVersion[0] = szClientVersion[0];
	pResult.CliVersion[1] = szClientVersion[1];
	pResult.CliVersion[2] = szClientVersion[2];
	pResult.CliVersion[3] = szClientVersion[3];
	pResult.CliVersion[4] = szClientVersion[4];

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	gObj[aIndex].ConnectCheckTime = GetTickCount();
}

void CSPJoinIdPassRequest(PMSG_IDPASS* lpMsg, int aIndex) 
{
	char serial[17];
	char id[11];

	if ( lpMsg->CliVersion[0] != szClientVersion[0] ||
		 lpMsg->CliVersion[1] != szClientVersion[1] ||
		 lpMsg->CliVersion[2] != szClientVersion[2] ||
		 lpMsg->CliVersion[3] != szClientVersion[3] ||
		 lpMsg->CliVersion[4] != szClientVersion[4] )
	{
		GCJoinResult(JS_BAD_CLIENT_VERSION, aIndex);
		CloseClient(aIndex);
		return;
	}

	serial[16] = 0;
	memcpy(serial, lpMsg->CliSerial, sizeof(lpMsg->CliSerial));
	id[10]=0;
	memcpy(id, lpMsg->Id, sizeof(lpMsg->Id));
	BuxConvert(id, MAX_ACCOUNT_LEN);

	if ( strcmp(serial, szGameServerExeSerial) != 0 )
	{
		LogAddC(2, "error-L1: Serial error [%s] [%s]", id, serial);
		GCJoinResult(JS_BAD_CLIENT_VERSION, aIndex);
		CloseClient(aIndex);
		return;
	}
	
	if ( bCanConnectMember == TRUE )
	{
		if ( ConMember.IsMember(id) == FALSE )
		{
			GCJoinResult(JS_ONLY_VIP_MEMBERS, aIndex);
			return;
		}
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (PacketCheckTime(lpObj) == FALSE )
	{
		LogAdd(lMsg.Get(MSGGET(1, 217)), aIndex, id);
		CloseClient(aIndex);
		return;
	}
	// ----
#ifdef __CUSTOMS__
	g_OfflineTrade.Close(id);
#endif

#ifdef OFFEXP
	OffExp.Close(id);
#endif
	// ----
	if ( gObj[aIndex].Connected != PLAYER_CONNECTED )
	{
		LogAdd(lMsg.Get(MSGGET(1, 218)), aIndex, id);
		CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].LoginMsgSnd != FALSE )
	{
		if ( gObj[aIndex].LoginMsgSnd == TRUE )
		{
			LogAdd(lMsg.Get(MSGGET(1, 219)), aIndex, id, gObj[aIndex].Ip_addr );
		}
		else
		{
			LogAdd("error : %d %s %d", gObj[aIndex].LoginMsgSnd, __FILE__, __LINE__);
		}

		return;
	}

	SDHP_IDPASS spMsg;

	PHeadSetB((LPBYTE)&spMsg, 0x01, sizeof(spMsg));
	spMsg.Number = aIndex;
	memcpy(spMsg.Id, lpMsg->Id, sizeof(spMsg.Id));
#ifdef __CUSTOMS__
	ZeroMemory(gObj[aIndex].m_Password, 10);
	memcpy(gObj[aIndex].m_Password, lpMsg->Pass, sizeof(spMsg.Pass));
#endif
	memcpy(spMsg.Pass, lpMsg->Pass, sizeof(spMsg.Pass));
	strcpy(spMsg.IpAddress, gObj[aIndex].Ip_addr);
	gObj[aIndex].CheckTick = lpMsg->TickCount;
	gObj[aIndex].CheckTick2 = GetTickCount();
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].CheckSpeedHack = true;
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].LoginMsgSnd = 1;
	gObj[aIndex].LoginMsgCount++;
	gObj[aIndex].AccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(gObj[aIndex].AccountID, id, MAX_ACCOUNT_LEN);
	gObj[aIndex].m_cAccountItemBlock = 0;

	gObj[aIndex].ukn_30 = 0; //season4.5 add-on

	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
	LogAddTD("join send : (%d)%s", aIndex, gObj[aIndex].AccountID);
	lpObj->m_bMapSvrMoveReq = false;
	lpObj->m_sPrevMapSvrCode = -1; 
	lpObj->m_sDestMapNumber = -1;
	lpObj->m_btDestX = 0;
	lpObj->m_btDestY = 0;
}

void CSPJoinIdPassRequestTEST(PMSG_IDPASS * lpMsg, int aIndex) 
{
	char szId[11];
	char szPass[11];
	LPOBJ lpObj = &gObj[aIndex];
	SDHP_IDPASS spMsg;

	PHeadSetB((LPBYTE)&spMsg, 0x11, sizeof(spMsg));
	spMsg.Number = aIndex;

	wsprintf(szId, "슛돌이%d", logincounttest);
	wsprintf(szPass, "m321", rand()%9);
	LogAdd("login send : %s %s", szId, szPass);
	
	BuxConvert(szId, MAX_ACCOUNT_LEN);
	BuxConvert(szPass, MAX_ACCOUNT_LEN);
	memcpy(spMsg.Id, szId, MAX_ACCOUNT_LEN);
	memcpy(spMsg.Pass, szPass, MAX_ACCOUNT_LEN);
	logincounttest++;
	
	if ( (GetTickCount()-ltesttime) > 1000 )
	{
		ltesttime = GetTickCount();
		logincounttest=0;
	}

	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
	spMsg.h.headcode = 0x01;
	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
}

void GCJoinBillCheckSend(LPSTR AccountId, int aIndex) 
{
	SDHP_BILLSEARCH pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof(pMsg));
	memcpy(pMsg.Id, AccountId, sizeof(pMsg.Id));
	pMsg.Number = aIndex;

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void GCJoinResult(BYTE result, int aIndex) 
{
	PMSG_RESULT  pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xF1, 0x01, sizeof(pResult));
	pResult.result = result;
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

void GCJoinSocketResult(BYTE result, SOCKET Socket) 
{
	PMSG_RESULT pResult;

	PHeadSubSetB((LPBYTE)&pResult , 0xF1, 0x01, sizeof(pResult) );
	pResult.result= result;

	wsGServer.DataSocketSend(Socket, (char*)&pResult, pResult.h.size);
}

void CGClientCloseMsg(PMSG_CLIENTCLOSE * lpMsg, int aIndex) 
{
	switch ( lpMsg->Flag )
	{
		case 0:	// Close Game
			gObjCloseSet(aIndex, 0);

			if ( aIndex < 0 || aIndex > OBJMAX-1)
			{
				return;
			}

			LogAddTD("[UserSelClose] Go Exit [%s][%s] Map:[%d][%d/%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
			break;

		case 2:	// ServerList
			gObjCloseSet(aIndex, 2);

			if ( aIndex < 0 || aIndex > OBJMAX-1)
			{
				return;
			}

			LogAddTD("[UserSelClose] Go ServerList [%s][%s] Map:[%d][%d/%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
			break;

		case 1:	// Select Character
			gObjCloseSet(aIndex, 1);

			if ( aIndex < 0 || aIndex > OBJMAX-1)
			{
				return;
			}

			LogAddTD("[UserSelClose] Go CharSelect [%s][%s] Map:[%d][%d/%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
			break;
	}
}

void GCCloseMsgSend(int aIndex, BYTE result) 
{
	PMSG_RESULT pMsg;

	PHeadSubSetBE((LPBYTE)&pMsg, 0xF1, 0x02, sizeof(pMsg));
	pMsg.result = result;
	
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGClientMsg(PMSG_CLIENTMSG* lpMsg, int aIndex) 
{
	if ( lpMsg->Flag )
	{
		LogAddC(2,"error-L1 : HackCheck [%s][%s] 0x%x 0x%x",
			gObj[aIndex].AccountID, gObj[aIndex].Name,
			lpMsg->Flag, lpMsg->subFlag);
	}
	else 
	{
		if ( gObj[aIndex].m_ClientHackLogCount < 100 )
		{
			LogAddC(2, "error-L1 : HackCheck [%s][%s] 0x%x 0x%x",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				lpMsg->Flag, lpMsg->subFlag);
		}

		gObj[aIndex].m_ClientHackLogCount++;

		if ( gObj[aIndex].m_ClientHackLogCount >= 100 )
			gObj[aIndex].m_ClientHackLogCount = 100;
	}
}

void CGPCharacterCreate( PMSG_CHARCREATE * lpMsg, int aIndex) 
{
	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( gObj[aIndex].Connected < PLAYER_LOGGED )
	{
		LogAdd(lMsg.Get(MSGGET(1, 220)), aIndex, gObj[aIndex].Ip_addr);
		CloseClient(aIndex);

		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		LogAddTD("[Anti-HACK][CGPCharacterCreate] Attempted Character Create during GamePlay. [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name);
		CloseClient(aIndex);

		return;
	}


	if ( !gCreateCharacter )
	{
		GCServerMsgStringSend("서버분할 기간에는 캐릭터를 생성할수 없습니다", aIndex, 1);
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);

		return;
	}

#ifdef SEASON6DOT3_ENG
	if ( lpMsg->ClassSkin != 0
		&& lpMsg->ClassSkin != 16
		&& lpMsg->ClassSkin != 32
		&& lpMsg->ClassSkin != 48
		&& lpMsg->ClassSkin != 64
		&& lpMsg->ClassSkin != 80
		&& lpMsg->ClassSkin != 96 )
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}
#else
	if ( lpMsg->ClassSkin == 0x00 || lpMsg->ClassSkin == 0x10 || lpMsg->ClassSkin == 0x20 || lpMsg->ClassSkin == 0x30 || lpMsg->ClassSkin == 0x40 || lpMsg->ClassSkin == 0x50 )
	{

	}
	else
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}
#endif

	if ( lpMsg->ClassSkin == 0x50 ) // season4 add-on Summoner fix antihack
	{
#ifdef SEASON5DOT4_ENG
		if ( gObj[aIndex].Summoner < 1 )
#else
		if ( gObj[aIndex].Summoner != 1 )
#endif
		{
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x30 ) // MG
	{
		if ( gObj[aIndex].Magumsa == 0 )
		{
			LogAddC(2, "error-L1: Magumsa Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x40 ) // DL
	{
		if ( gObj[aIndex].Magumsa != 2 )
		{
			LogAddC(2, "error-L1: Darklord Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	SDHP_CREATECHAR sCreate;

	memset(&sCreate, 0, sizeof(sCreate));
	PHeadSetB((LPBYTE)&sCreate, 0x04, sizeof(sCreate));
	sCreate.ClassSkin = lpMsg->ClassSkin;
	sCreate.Number = aIndex;
	memcpy(sCreate.AccountId, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(sCreate.Name, lpMsg->Name, MAX_ACCOUNT_LEN);
	char szTemp[20];
	char szTemp2[20];
	szTemp[10]=0;
	szTemp2[10]=0;
	memcpy(szTemp, sCreate.Name, MAX_ACCOUNT_LEN);
	memcpy(szTemp2, sCreate.AccountId, MAX_ACCOUNT_LEN);

	LogAddTD("Character create : %s,%s", szTemp2, szTemp);

	cDBSMng.Send((char*)&sCreate, sCreate.h.size);
}

void CGPCharDel(PMSG_CHARDELETE * lpMsg,int aIndex) 
{
	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	if ( gObj[aIndex].Connected < PLAYER_LOGGED )
	{
		CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		LogAddTD("[Anti-HACK][CGPCharDel] Attempted Character Delete during GamePlay. [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
		CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].bEnableDelCharacter == FALSE )
	{
		LogAddTD("[Anti-HACK][CGPCharDel] Attempted Character Delete during Prevented Time. [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
		CloseClient(aIndex);
		return;
	}

	SDHP_CHARDELETE pCDel;
	PMSG_RESULT pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xF3, 0x02, sizeof(pResult));
	pResult.result = 0;

	if ( gObj[aIndex].m_cAccountItemBlock )
	{
		pResult.result = 3;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !gGuildDestroy )
	{
		pResult.result = 0;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	char szJoomin[11];
	char szTemp[20];
	char szTemp2[20];

	memset(szJoomin, 0, sizeof(szJoomin));
	memcpy(szJoomin, lpMsg->LastJoominNumber, 10);

	//if ( gObjJoominCheck(aIndex, szJoomin) == FALSE )
	//{
	//	pResult.result = 2;
	//	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	//	return;
	//}

	PHeadSetB((LPBYTE)&pCDel, 0x05, sizeof(pCDel));
	pCDel.Number = aIndex;
	memcpy(pCDel.AccountID, gObj[aIndex].AccountID, sizeof(pCDel.AccountID));
	memcpy(pCDel.Name, lpMsg->Name, sizeof(pCDel.Name));
	pCDel.Guild = 0;

	if ( gObj[aIndex].GuildNumber > 0 && gObj[aIndex].lpGuild != NULL)
	{
		if ( !strcmp(gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0] ))
		{
			pCDel.Guild = 1;	// Master
		}
		else
		{
			pCDel.Guild = 2;	// Member
		}

		memcpy(pCDel.GuildName, gObj[aIndex].lpGuild->Name, MAX_GUILD_LEN);
	}

	g_CashItemPeriodSystem.GDReqPeriodItemDelete(&gObj[aIndex], lpMsg->Name);
	szTemp[10]=0;
	szTemp2[10]=0;
	memcpy(szTemp, pCDel.Name, MAX_ACCOUNT_LEN);
	memcpy(szTemp2, pCDel.AccountID, MAX_ACCOUNT_LEN);

	LogAddTD("Character delete : %s,%s", szTemp2, szTemp);

	cDBSMng.Send((char*)&pCDel, pCDel.h.size);
	gObj[aIndex].Level = 0;
}

struct PMSG_BATTLE_LIST
{
	PBMSG_HEAD	h;
	// ----
	BYTE		BattleMapList[11];
	BYTE		BattleZoneList[11];
};

void CGPCharacterMapJoinRequest( PMSG_CHARMAPJOIN * lpMsg, int aIndex) 
{
	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	if ( !gObjIsAccontConnect(aIndex, gObj[aIndex].AccountID))
	{
		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		return;
	}

	gObj[aIndex].bEnableDelCharacter = FALSE;

	char _name[MAX_ACCOUNT_LEN+1];
	SDHP_DBCHARINFOREQUEST pCRequest;

	PHeadSetB((LPBYTE)&pCRequest, 0x06, sizeof(pCRequest));
	memset(_name, 0, MAX_ACCOUNT_LEN);
	memcpy(_name, lpMsg->Name, MAX_ACCOUNT_LEN);
	BuxConvert(_name, MAX_ACCOUNT_LEN);
	memcpy(pCRequest.Name, _name, MAX_ACCOUNT_LEN);
	strcpy(pCRequest.AccountID, gObj[aIndex].AccountID);
	pCRequest.Number = aIndex;

	cDBSMng.Send((char*)&pCRequest, pCRequest.h.size);
#ifdef GENS
	PMSG_BATTLE_LIST Result;
	// ----
	PHeadSetB((LPBYTE)&Result, 0xFC, sizeof(Result));
	// ----
	for(int i = 0; i < MAX_BATTLE_ZONE; i++ )
	{
		Result.BattleMapList[i]		= gGensSystem.BattleMapList[i];
	}
	// ----
	for(int i = 0; i < MAX_BATTLE_ZONE; i++ )
	{
		Result.BattleZoneList[i]	= gGensSystem.BattleZoneList[i];
	}
	// ----
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
#endif
}

void GCLevelUpMsgSend(int aIndex, int iSendEffect) 
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	short AddPoint=0;
	short MaxAddPoint=0;
	short MinusPoint=0;
	short MaxMinusPoint=0;

	gObjGetStatPointState(gObj[aIndex].m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
	
	PMSG_LEVELUP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x05, sizeof(pMsg));
	pMsg.Level = gObj[aIndex].Level;
	pMsg.LevelUpPoint = gObj[aIndex].LevelUpPoint;
	pMsg.MaxLife = ((float)gObj[aIndex].MaxLife+(float)gObj[aIndex].AddLife);
	pMsg.MaxMana = ( (float)gObj[aIndex].MaxMana+(float)gObj[aIndex].AddMana);
	pMsg.MaxBP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	pMsg.wMaxShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
	pMsg.AddPoint = AddPoint;
	pMsg.MaxAddPoint = MaxAddPoint;
	pMsg.MinusPoint = MinusPoint;
	pMsg.MaxMinusPoint = MaxMinusPoint;

	pMsg.UpPoint = gObj[aIndex].LevelUpPoint;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( iSendEffect == 1 )
	{
		GCSendEffectInfo(aIndex, 0x10);
	}
}

void CGLevelUpPointAdd(PMSG_LVPOINTADD * lpMsg, int aIndex) 
{
	if ( !gObjIsConnected(aIndex))
	{
		CloseClient(aIndex);
		return;
	}

	PMSG_LVPOINTADDRESULT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x06, sizeof(pMsg));
	pMsg.ResultType = 0;
	
	if (::gObjLevelUpPointAdd(lpMsg->Type, &gObj[aIndex]) == TRUE )
	{
		pMsg.ResultType = 0x10;
		pMsg.ResultType += lpMsg->Type;

		switch ( lpMsg->Type )
		{
			case 2:	// Vit
				pMsg.MaxLifeAndMana = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				break;

			case 3:	// Energy
				pMsg.MaxLifeAndMana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
				break;
		}

		if ( szAuthKey[9] != AUTHKEY9 )
		{
			DestroyGIocp();
		}

		pMsg.wMaxShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
		gObjSetBP(aIndex);
		pMsg.MaxBP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	}

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

}

void GCInventoryItemOneSend(int aIndex, int pos) 
{
	if ( !gObj[aIndex].pInventory[pos].IsItem())
	{
		return;
	}

	PMSG_INVENTORYITEMMODIFY pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x14, sizeof(pMsg));
	pMsg.Pos = pos;
	ItemByteConvert(pMsg.ItemInfo, gObj[aIndex].pInventory[pos]);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCPkLevelSend(int aIndex, BYTE pklevel) 
{
	PMSG_PKLEVEL pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x08, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.PkLevel = pklevel;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
}

bool CheckAuthorityCondition(int AuthorityCode, LPOBJ lpObj) //0043F3E0 identical season4.5 add-on
{
	return ((lpObj->Authority&AuthorityCode) == lpObj->Authority)?true:false;
}

//Revised to 1.01.03
void GCMagicListOneSend(int aIndex, char Pos, WORD type, BYTE level, WORD skill, BYTE btListType)
{
	PMSG_MAGICLISTCOUNT pCount;
	PMSG_MAGICLIST pList;
	BYTE sendbuf[1000];
	// ----
	int lOfs			= sizeof(pCount);
	PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);
	// ----
	pCount.Count		= 0xFE;
	pCount.btListType	= btListType;
	// ----
	pList.Pos			= Pos;
	pList.wSkillNum		= type;
	pList.btSkillLevel	= level << 3;
	pList.btSkillLevel	|= (BYTE)skill & 0x07;
	// ----
	memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
	lOfs				+= sizeof(pList);
	// ----
	pCount.h.size		= lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));
	// ----
	DataSend(aIndex, sendbuf, lOfs);
}

//Revised to 1.01.03
void GCMagicListOneDelSend(int aIndex, char Pos, WORD type, BYTE level, BYTE skill, BYTE btListType) 
{
	PMSG_MAGICLISTCOUNT pCount;
	PMSG_MAGICLIST pList;
	BYTE sendbuf[1000];
	// ----
	int lOfs			= sizeof(pCount);
	PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);
	// ----
	pCount.h.size		= 0;
	pCount.Count		= 0xFF;
	pCount.btListType	= btListType;
	// ----
	pList.Pos			= Pos;
	pList.wSkillNum		= type;
	pList.btSkillLevel	= level << 3;
	pList.btSkillLevel	|= (BYTE)skill & 0x07;
	// ----
	memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
	lOfs				+= sizeof(pList);
	// ----
	pCount.h.size		= lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));
	// ----
	DataSend(aIndex, sendbuf, lOfs);
}

//Revised to 1.01.03
void GCMagicListMultiSend(LPOBJ lpObj, BYTE btListType)
{
	WORD BrandSkill;	//normaly position?
	PMSG_MAGICLISTCOUNT pCount;
	PMSG_MAGICLIST pList;
	BYTE sendbuf[1000];
	// ----
	int lOfs			= sizeof(pCount);
	int Count			= 0;
	pCount.btListType	= btListType;
	// ----
	PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);
	// ----
	for( int n=0; n < MAX_MAGIC; n++ )
	{
		if( lpObj->Magic[n].IsMagic() == TRUE )
		{
			if( !g_MasterSkillSystem.CheckUsableWeaponSkill(lpObj, n) )
			{
				continue;
			}
			// ----
			if( g_MasterSkillSystem.IsExistMLSBasedSkill(lpObj, lpObj->Magic[n].m_Skill) )
			{
				continue;
			}
			// ----
			if( lpObj->Magic[n].m_bPass == true )
			{
				continue;
			}
			// ----
			pList.Pos			= n;
			pList.wSkillNum		= (WORD)lpObj->Magic[n].m_Skill;
			pList.btSkillLevel	= lpObj->Magic[n].m_Level << 3;
			pList.btSkillLevel	|= lpObj->Magic[n].m_Skill & 0x07;
			// ----
			memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			lOfs				+= sizeof(pList);
			// ----
			Count++;
			// ----
			if( MagicDamageC.IsItemAttachedSkill(lpObj->Magic[n].m_Skill) )	//1.01.03
			{
				BrandSkill = MagicDamageC.GetNormalBrandOfSkill(lpObj->Magic[n].m_Skill);
				// ----
				if( !lpObj->pInventory[1].IsItem() || lpObj->pInventory[1].m_Special[0] != BrandSkill )
				{
					continue;
				}
				// ----
				if( !lpObj->pInventory[0].IsItem() || lpObj->pInventory[0].m_Special[0] != BrandSkill )
				{
					continue;
				}
				// ----
				pList.Pos			= n + 1;
				pList.wSkillNum		= LOWORD(lpObj->Magic[n].m_Skill);
				pList.btSkillLevel	= lpObj->Magic[n].m_Level << 3;
				pList.btSkillLevel	|= lpObj->Magic[n].m_Skill & 0x07;
				// ----
				memcpy(&sendbuf[lOfs], &pList.Pos, 4);
				lOfs				+= 4;
				// ----
				Count++;
			}
		}
	}
	// ----
	pCount.Count	= Count;
	pCount.h.size	= lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));
	// ----
	DataSend(lpObj->m_Index, sendbuf, lOfs);
}

void GCEquipmentSend(int aIndex) 
{
	PMSG_EQUIPMENTLIST pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x13, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	gObjMakePreviewCharSet(aIndex);
	LPOBJ lpObj = &gObj[aIndex];
	memcpy(pMsg.Equipment, lpObj->CharSet, sizeof(lpObj->CharSet));

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCRecallMonLife(int aIndex, int maxlife, int life) 
{
	if ( maxlife <= 0 )
	{
		return;
	}
	
	PMSG_RECALLMONLIFE pMsg;
	BYTE per = life * 100 / maxlife;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x20, sizeof(pMsg));
	pMsg.Life = per;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCTimeViewSend(int aIndex, int second) 
{
	PMSG_TIMEVIEW pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x22, sizeof(pMsg));
	pMsg.Second = second;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCGoalSend(int aIndex, char* Name1, BYTE score1, char* Name2, BYTE score2) 
{
	PMSG_GOALSEND pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x23, sizeof(pMsg));
	memcpy(pMsg.RedTeamName, Name1, sizeof(pMsg.RedTeamName));
	pMsg.RedTeamScore = score1;
	memcpy(pMsg.BlueTeamName, Name2, sizeof(pMsg.BlueTeamName));
	pMsg.BlueTeamScore = score2;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCSkillKeyRecv(PMSG_SKILLKEY * lpMsg, int aIndex) 
{
	if ( !gObjIsGamePlaing(&gObj[aIndex]))
	{
		return;
	}

	if(gObj[aIndex].m_bSkillKeyRecv == 1) //season 2.5 add-on
	{
		return;
	}

	gObj[aIndex].m_bSkillKeyRecv = 1; //season 2.5 add-on

	BYTE RKey = 0;

	if(lpMsg->h.size == sizeof(PMSG_SKILLKEY)) // OMG :|
	{
		RKey = lpMsg->RkeyDefine;
	}

	int QWERLevel = 0;

	if(lpMsg->h.size == sizeof(PMSG_SKILLKEY)) // OMG :|
	{
		QWERLevel = ntohl(lpMsg->QWERLevel);
	}

	DGOptionDataSend(aIndex, gObj[aIndex].Name,	lpMsg->SKillKey, lpMsg->GameOption,	lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, RKey, QWERLevel);

	LogAddL("Option Recv %d %d %d %d %d %d", lpMsg->GameOption, lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, RKey, lpMsg->QWERLevel); //fix me WZ noob
}

void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk, int QWERLevel)
{
	PMSG_SKILLKEY pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x30, sizeof(pMsg));

	memcpy(pMsg.SKillKey, keybuffer, sizeof(pMsg.SKillKey));
	pMsg.GameOption = GO;
	pMsg.QkeyDefine = Qk;
	pMsg.WkeyDefine = Wk;
	pMsg.EkeyDefine = Ek;
	pMsg.ChatWindow = ChatWnd;

	pMsg.RkeyDefine = Rk;
	pMsg.QWERLevel = QWERLevel;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	LogAddL("Option Send %d %d %d %d %d %d", GO, Qk, Wk, Ek, Rk, QWERLevel);
}

void GCMoneySend(int aIndex, DWORD money) 
{
	PMSG_ITEMGETRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x22;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0xFE;

	WORD hiWord = SET_NUMBERHW(money);
	WORD loWord = SET_NUMBERLW(money);
	pMsg.Data[0] = SET_NUMBERH(hiWord);
	pMsg.Data[1] = SET_NUMBERL(hiWord);
	pMsg.Data[2] = SET_NUMBERH(loWord);
	pMsg.Data[3] = SET_NUMBERL(loWord);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCItemInventoryPutSend(int aIndex,BYTE result, BYTE iteminfo1, BYTE iteminfo2) 
{
	PMSG_ITEMGETRESULT pResult;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x22;
	pResult.h.size = sizeof(pResult);
	pResult.result = result;
	pResult.Data[0] = iteminfo1;
	pResult.Data[1] = iteminfo2;
	pResult.h.size -= 2;

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

//0044B180	-> OK (without doppelganger and etc)
void CGItemGetRequest(PMSG_ITEMGETREQUEST * lpMsg, int aIndex)
{
	int item_num, map_num;
	PMSG_ITEMGETRESULT pResult;
	int type;
	int level;
	int special;
	int NOption;
	char szItemName[50];
	CMapItem * lpItem;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x22;
	pResult.h.size = sizeof(pResult);
	pResult.result = -1;

	if ( !gObjIsConnected(aIndex))
	{
		CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	if ( gObj[aIndex].DieRegen != 0 )
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

#ifdef NPVP
	if( g_NewPVP.IsObserver(gObj[aIndex]) )
    {
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
    }
#endif

	if ( gObj[aIndex].m_IfState.use != 0 )
	{
		if ( gObj[aIndex].m_IfState.type != 3 )
		{
			::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 94)), aIndex, 1);
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == TRUE )
	{
		LogAddTD("[%s][%s] CGItemGetRequest() Failed : Transaction == 1, IF_TYPE : %d",	gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;

	}

	item_num = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( MAX_ITEM_TYPE_RANGE(item_num) == FALSE )
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	map_num = gObj[aIndex].MapNumber;

	if ( MAX_MAP_RANGE(map_num) == FALSE )
	{
		LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	lpItem = &MapC[map_num].m_cItem[item_num];

	if ( lpItem->IsItem() == TRUE && lpItem->Give == false && lpItem->live == true)
	{
		if ( lpItem->m_QuestItem != false )
		{
			BOOL bNotFoundQuestItem = g_QuestInfo.CountQuestItemInInventory(aIndex, lpItem->m_Type, lpItem->m_Level); //season 2.5 add-on
#ifdef WZQUEST
			if(bNotFoundQuestItem == FALSE && 
                !g_QuestExpProgMng.IsQuestDropItem(aIndex, lpItem->m_Type, lpItem->m_Level))
			{
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
#endif
		}

		if ( lpItem->m_Type == ITEMGET(13,20) )
		{
			switch ( lpItem->m_Level )
			{
				case 0:
				{
					int iWRCount = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type%MAX_SUBTYPE_ITEMS, lpItem->m_Level);

					if ( iWRCount > 0 )
					{
						LogAdd("[Ring Event] Too many have Magician's Ring [%s][%s] (Name:%s, Count:%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), iWRCount);

						pResult.result = -1;
						pResult.h.size -= sizeof(pResult.Data);

						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 186)), aIndex, 1);
						return;
					}
					break;
				}
				case 1:
					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return;
					break;
				
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,20) && lpItem->m_Level == 2 )
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
			return;
		}

		if ( lpItem->m_Type == ITEMGET(13,38) )
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				LogAdd("[ MoonStonePendant ] Too many have MoonStonePendant [%s][%s] ( Name:%s, Count:%d )", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);

				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 56)), aIndex, 1);
				return;
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,39) )
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				LogAdd("[ ChangeRing ] Too many have ChangeRing [%s][%s] ( Name:%s, Count:%d )", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);

				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 63)), aIndex, 1);
				return;
			}
		}

		type = lpItem->m_Type;
		level = lpItem->m_Level;
		special = lpItem->m_Special[0];
		NOption = lpItem->m_NewOption;
		strcpy(szItemName, lpItem->GetName());

		if ( lpItem->m_Type == ITEMGET(14,15) )
		{
			if ( MapC[map_num].ItemGive(aIndex, item_num, false) == TRUE )
			{
				if ( !gObjCheckMaxZen(aIndex, lpItem->m_BuyMoney))
				{
					if ( gObj[aIndex].Money < MAX_ZEN )
					{
						gObj[aIndex].Money = MAX_ZEN;
						pResult.result = -2;
						WORD hiWord = SET_NUMBERHW(gObj[aIndex].Money);
						WORD loWord = SET_NUMBERLW(gObj[aIndex].Money);
						pResult.Data[0] = SET_NUMBERH(hiWord);
						pResult.Data[1] = SET_NUMBERL(hiWord);
						pResult.Data[2] = SET_NUMBERH(loWord);
						pResult.Data[3] = SET_NUMBERL(loWord);
						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						return;
					}

					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
					return;
				}

				gObj[aIndex].Money += lpItem->m_BuyMoney;
				pResult.result = -2;
				WORD hiWord = SET_NUMBERHW(gObj[aIndex].Money);
				WORD loWord = SET_NUMBERLW(gObj[aIndex].Money);
				pResult.Data[0] = SET_NUMBERH(hiWord);
				pResult.Data[1] = SET_NUMBERL(hiWord);
				pResult.Data[2] = SET_NUMBERH(loWord);
				pResult.Data[3] = SET_NUMBERL(loWord);
				pResult.h.size -= 3;
			}

			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		}
		else
		{
			if ( lpItem->m_Type == ITEMGET(13,32) )
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 20, ITEMGET(13,32), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
					return;
				}
			}

			if ( lpItem->m_Type == ITEMGET(13,33) )
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 20, ITEMGET(13,33), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
					return;
				}
			}

			if ( lpItem->m_Type == ITEMGET(13,34) )
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 10, ITEMGET(13,34), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
					return;
				}
			}

			if (lpItem->m_Type == ITEMGET(14,70) ||	lpItem->m_Type == ITEMGET(14,71) ||	lpItem->m_Type == ITEMGET(14,94)
				||	lpItem->m_Type == ITEMGET(14, 133)	//6.2
				)
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 50, lpItem->m_Type, 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
					return;
				}
			}

			if (lpItem->m_Type == ITEMGET(14,88) ||	lpItem->m_Type == ITEMGET(14,89) ||	lpItem->m_Type == ITEMGET(14,90) || lpItem->m_Type == ITEMGET(14,85) ||	lpItem->m_Type == ITEMGET(14,86) ||	lpItem->m_Type == ITEMGET(14,87)) //season 3.5 add-on
			{
				int iMaxOverLapped = 1;

				switch(lpItem->m_Type)	//OK
				{
				case ITEMGET(14, 88):	iMaxOverLapped = g_MaxCherryBranchWhite;	break;
				case ITEMGET(14, 89):	iMaxOverLapped = g_MaxCherryBranchRed;		break;
				case ITEMGET(14, 90):	iMaxOverLapped = g_MaxCherryBranchGold;		break;	
				case ITEMGET(14,85):	iMaxOverLapped = 3;	break;
				case ITEMGET(14,86):	iMaxOverLapped = 3;	break;
				case ITEMGET(14,87):	iMaxOverLapped = 3;	break;
				}

				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, iMaxOverLapped, lpItem->m_Type, 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
					return;
				}
			}
			
			if ( lpItem->m_Type == ITEMGET(14,29) )
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = ::g_KalimaGate.CheckOverlapKundunMark(aIndex, lpItem->m_Level);

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							BYTE NewOption[MAX_EXOPTION_SIZE];

							::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

							LogAddTD(lMsg.Get(MSGGET(1, 221)), gObj[aIndex].AccountID, gObj[aIndex].Name, map_num, gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type, level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (int)lpItem->m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7, g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL));

							pResult.result = -3;

							DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

							gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

							if ( gObj[aIndex].pInventory[pos].m_Durability >= 5.0f )
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - 5.0f;
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								::GCInventoryItemDeleteSend(aIndex, pos, 1);
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 28), lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0);

								LogAddTD("[Kalima] [%s][%s] Make Lost Kalima Map (Left Kundun Mark:%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;
									continue;
								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,21) && lpItem->m_Level == 3)	//OK
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = ::g_CastleSiegeSync.CheckOverlapCsMarks(aIndex);

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						int Dur = gObj[aIndex].pInventory[pos].m_Durability + lpItem->m_Durability;

						if ( Dur <= 255 )
						{
							if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
							{
								pResult.result = -3;
								DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
								GCItemDurSend( aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability,0);
								return;
							}
						}
						else
						{
							lpItem->m_Durability = Dur - 255;
							gObj[aIndex].pInventory[pos].m_Durability = 255.0f;
							GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
						}

					}
					else
					{
						break;
					}
				}
			}

			if (lpItem->m_Type == ITEMGET(14,100)) //season4.5 add-on
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = -1;

					if(gObj[aIndex].pInventory[n].m_Type == ITEMGET(14,100))
					{
						if(gObj[aIndex].pInventory[n].m_Durability < 255.0f)
						{
							pos = n;
						}
					}

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						int Dur = gObj[aIndex].pInventory[pos].m_Durability + lpItem->m_Durability;

						if ( Dur <= 255 )
						{
							if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
							{
								pResult.result = -3;
								DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
								GCItemDurSend( aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability,0);
								return;
							}
						}
						else
						{
							lpItem->m_Durability = Dur - 255;
							gObj[aIndex].pInventory[pos].m_Durability = 255.0f;
							GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
						}

					}
				}
			}

#ifdef IMPERIAL
			if ( lpItem->m_Type == ITEMGET(14,101) )	//OK
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = g_ImperialGuardian.CheckOverlapMysteriousPaper(aIndex, lpItem->m_Level);

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							BYTE NewOption[MAX_EXOPTION_SIZE];

							::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

							//LogAddTD(lMsg.Get(MSGGET(1, 221)), gObj[aIndex].AccountID, gObj[aIndex].Name, map_num, gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type, level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (int)lpItem->m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7, g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL));

							pResult.result = -3;

							DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

							gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

							if ( gObj[aIndex].pInventory[pos].m_Durability >= 5.0f )
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - 5.0f;
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								::GCInventoryItemDeleteSend(aIndex, pos, 1);
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, 
									ItemGetNumberMake(14, 102), lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0);

								//LogAddTD("[Kalima] [%s][%s] Make Lost Kalima Map (Left Kundun Mark:%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;
									continue;
								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}
#endif

#ifdef DP
			if( lpItem->m_Type == 7278 && g_DoppleganerEvent.PickupMarkOfDimensionItem(&gObj[aIndex], lpItem, lpItem->m_Type) )//OK
			{
				// ----
			}
#endif
			if( g_LuckyItemManager.IsLuckyItemEquipment(lpItem->m_Type) || g_LuckyItemManager.IsLuckyItemTicket(lpItem->m_Type) )
			{
				char szSetItemName[48]	= { 0 };
				int tmpSetOption		= 0;
				BYTE NewOption[8]		= { 0 };
				// ----
				ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);
				// ----
				if( gSetItemOption.IsSetItem(lpItem->m_Type) )
				{
					if( lpItem->m_SetOption & 1 )
					{
						tmpSetOption = 1;
					}
					else
					{
						if( lpItem->m_SetOption & 2 )
						{
							tmpSetOption = 2;
						}
					}
					// ----
					strcpy(szSetItemName, gSetItemOption.GetSetOptionName(type, tmpSetOption));
				}
				// ----
				LogAddTD("[%s][%s][%d]%d/%d Try Get LuckyItem serial:%u [%s][%d][%d][%d][%d][%d] dur:[%d]Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name, map_num, gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szSetItemName,
					type, level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (INT)lpItem->m_Durability,
					NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
					lpItem->m_SetOption, lpItem->m_ItemOptionEx >> 7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL),
					g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL));
				// ----
				pResult.result = -1;
				// ----
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				GCServerMsgStringSend(lMsg.Get(1118), aIndex, 1);
			}
			
			pResult.result = ::gObjInventoryInsertItemTemp(&gObj[aIndex], lpItem);
			
			if ( pResult.result != 0xFF )
			{
				ItemByteConvert((LPBYTE)&pResult.Data[0], 
					lpItem->m_Type, 
					lpItem->m_Option1, 
					lpItem->m_Option2,	
					lpItem->m_Option3, 
					lpItem->m_Level, 
					lpItem->m_Durability, 
					lpItem->m_NewOption, 
					lpItem->m_SetOption, 
					lpItem->m_JewelOfHarmonyOption, 
					lpItem->m_ItemOptionEx, 
					lpItem->m_SocketOption, 
					lpItem->m_BonusSocketOption, 0);

				if ( MapC[map_num].ItemGive(aIndex, item_num, false) == TRUE )
				{
					BYTE pos = ::gObjInventoryInsertItem(aIndex, lpItem);

					if ( pos == 0xFF )
					{
						pResult.result = -1;
					}

					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					if ( pos == 0xFF )
					{
						::GCItemListSend(aIndex);
#ifdef PERIOD
						g_PeriodItemEx.OnRequestPeriodItemList(&gObj[aIndex]);
#endif
					}
					else
					{
						BYTE NewOption[MAX_EXOPTION_SIZE];

						::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);
						LogAddTD(lMsg.Get(477), gObj[aIndex].AccountID, gObj[aIndex].Name, map_num, gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type, level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (INT)lpItem->m_Durability, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7, g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL));
						::GCSendGetItemInfoForParty(aIndex, lpItem);

						if ( BC_MAP_RANGE(map_num) )
						{
							int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(map_num);

							if ( g_BloodCastle.CheckQuestItemSerial(iBridgeIndex, lpItem) )
							{
								g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_QUEST_ITEM_USER_INDEX = gObj[aIndex].m_Index;

								int iMapNumber = g_BloodCastle.GetBridgeIndexByMapNum(gObj[aIndex].MapNumber); //WZ Fix-me plz

								char szTempMsg[128];

								switch ( lpItem->m_Level )
								{
									case 0:	// Staff of Archangel
										wsprintf(szTempMsg, lMsg.Get(MSGGET(4, 150)), gObj[aIndex].Name);
										g_BloodCastle.SendNoticeMessage(iMapNumber, szTempMsg);
										break;

									case 1:	// Sword of Archangel
										wsprintf(szTempMsg, lMsg.Get(MSGGET(4, 151)), gObj[aIndex].Name);
										g_BloodCastle.SendNoticeMessage(iMapNumber, szTempMsg);
										break;

									case 2:	// Crossbow of Archangel
										wsprintf(szTempMsg, lMsg.Get(MSGGET(4, 152)), gObj[aIndex].Name);
										g_BloodCastle.SendNoticeMessage(iMapNumber, szTempMsg);
										break;

									default:
										szTempMsg[0] = 0;
								}

								LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) picked up Angel King's Weapon (%d)", iBridgeIndex+1, gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->m_Level);
							}
						}

						if ( IT_MAP_RANGE(map_num) ) //Season 2.5 add-on
						{
							if(lpItem->m_Type == ITEMGET(14,64))
							{
								g_IllusionTempleEvent.SetInventoryPos(gObj[aIndex].MapNumber,aIndex,pos); //check
								g_IllusionTempleEvent.SendRelicsUser(&gObj[aIndex]);
								LogAddTD("[Illusion Temple] (%d) (Account:%s, Name:%s) picked up Relics Item(serial:%d)", map_num-(45-1), gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->m_Number);
							}
						}
					}
				}
				else
				{
					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				}
			}
			else
			{
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
	else
	{
		pResult.h.size -= sizeof(pResult.Data);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
}

//0044e7b0	->
BOOL CGItemDropRequest(PMSG_ITEMTHROW * lpMsg, int aIndex, BOOL drop_type)
{
	BOOL CopyItem = FALSE;
	BYTE NewOption[MAX_EXOPTION_SIZE];

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return FALSE;
	}

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = true;
	pResult.Ipos = lpMsg->Ipos;

	if ( gObj[aIndex].m_IfState.use != 0 )
	{
		if ( gObj[aIndex].m_IfState.type != 3 )
		{
			pResult.Result = false;
			return FALSE;
		}
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		pResult.Result = false;
	}

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true )
	{
		LogAddTD("[ANTI-HACK][Item Duplication] - Item Drop during MapServer Move [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
		pResult.Result = false;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[%s][%s] CGItemDropRequest() Failed : Transaction == 1, IF_TYPE : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		pResult.Result = false;
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->Ipos == 0xFF )
	{
		LogAdd("error:%s %d", __FILE__, __LINE__);
		pResult.Result = false;
	}

	if ( lpMsg->Ipos < 0 || lpMsg->Ipos > MAIN_INVENTORY_SIZE-1)
	{
		pResult.Result = false;
	}

	if (!::gObjCanItemTouch(lpObj, 0))
	{
		pResult.Result = false;
	}

	if( g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[lpMsg->Ipos].m_Type) 
		|| g_LuckyItemManager.IsLuckyItemTicket(lpObj->pInventory[lpMsg->Ipos].m_Type) )
	{
		pResult.Result = false;
	}

	if ( gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		if ( lpMsg->Ipos == 8 )
		{
			if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,3) )
			{
				if( lpObj->pInventory[7].IsItem() == FALSE )
				{
					pResult.Result = false;
				}
			}
		}
		else if ( lpMsg->Ipos == 7 )
		{
			if ( lpObj->pInventory[8].m_Type != ITEMGET(13,3) )
			{
				pResult.Result = false;
			}
		}
	}

#ifdef PERIOD
	if( lpObj->pInventory[lpMsg->Ipos].IsPeriodItem() )
	{
		pResult.Result = false;
	}
#endif

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,30) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,31) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,36) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,37) )
	{
		pResult.Result = false;
	}

	if( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,42) ) //Season 2.5 add-on
	{
		pResult.Result = false;
	}

	if(IT_MAP_RANGE(lpObj->MapNumber) != FALSE) //Season 2.5 add-on
	{
		if( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,51) )
		{
			pResult.Result = false;
		}
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,64) )
	{
		pResult.Result = false;
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,36) && //Season 2.5 add-on
		 lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,40) &&
		 gPkLimitFree == 0) //season 4.5 add-on
	{
		pResult.Result = false;
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,52) || //Season 2.5 add-on
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,53) )
	{
		pResult.Result = false;
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,13) ||
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,14) ||
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,16) ||
		(lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,0)
		&& lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,6)) 
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,15) 
		|| (lpObj->pInventory[lpMsg->Ipos].m_Level >= 7 //season 2.5 changed 
		&& lpObj->pInventory[lpMsg->Ipos].m_Type < ITEMGET(12,0)) 
		||  lpObj->pInventory[lpMsg->Ipos].IsSetItem() != FALSE 
		||  lpObj->pInventory[lpMsg->Ipos].IsExtItem() != FALSE 
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,14) //season 2.5 add-on
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,19)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,15)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,34)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,31)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,41)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,42)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,43)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,44)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,41) //season 3 add-on
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,42)
		|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,43)
#ifdef NEWWINGS
		|| IS_NEWWINGS(lpObj->pInventory[lpMsg->Ipos].m_Type)
#endif
		)
	{
		if ( gPkLimitFree == FALSE )
		{
			pResult.Result = false;
		}
	}

	if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[lpMsg->Ipos]) == TRUE )
	{
		pResult.Result = false;
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 43)), lpObj->m_Index, 1);
	}

	if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal(lpObj->pInventory[lpMsg->Ipos].m_Type) ||
		 g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(lpObj->pInventory[lpMsg->Ipos].m_Type)   ||
		 g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems(lpObj->pInventory[lpMsg->Ipos].m_Type) )
	{
		pResult.Result = false;
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,39) )
	{
		pResult.Result = false;
	}

	if ( IsCashItem(lpObj->pInventory[lpMsg->Ipos].m_Type) == TRUE )
	{
		pResult.Result = false;
	}

	if ( IsPremiumItem(lpObj->pInventory[lpMsg->Ipos].m_Type) == TRUE ) //season 4.5 add-on
	{
		pResult.Result = false;
	}

	if( lpObj->pInventory[lpMsg->Ipos].m_QuestItem) //season 2.5 add-on
	{
		return 0xFF;
	}

#ifdef NPVP
	if (drop_type == 0 && lpObj->pInventory[lpMsg->Ipos].m_Type == (512 * 14) + 28 && g_NewPVP.IsDuel(*lpObj))
	{
		pResult.Result = 0x00;
	}
#endif

	if ( pResult.Result == 1 )
	{
		::ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Ipos]);

		int map = lpObj->MapNumber;
		int type = lpObj->pInventory[lpMsg->Ipos].m_Type;
		int level = lpObj->pInventory[lpMsg->Ipos].m_Level;
		float dur = lpObj->pInventory[lpMsg->Ipos].m_Durability;
		BOOL bIsItem = lpObj->pInventory[lpMsg->Ipos].IsItem();
		BYTE Option1 = lpObj->pInventory[lpMsg->Ipos].m_Option1;
		BYTE Option2 = lpObj->pInventory[lpMsg->Ipos].m_Option2;
		BYTE Option3 = lpObj->pInventory[lpMsg->Ipos].m_Option3;
		BYTE NOption = lpObj->pInventory[lpMsg->Ipos].m_NewOption;
		DWORD serial = lpObj->pInventory[lpMsg->Ipos].m_Number;
		int PetLevel = lpObj->pInventory[lpMsg->Ipos].m_PetItem_Level;
		int PetExp = lpObj->pInventory[lpMsg->Ipos].m_PetItem_Exp;
		BYTE SOption = lpObj->pInventory[lpMsg->Ipos].m_SetOption;
		BYTE btItemEffectEx = lpObj->pInventory[lpMsg->Ipos].m_ItemOptionEx;

		//Season 4 add-on
		BYTE SocketOption[5];
		BYTE SocketIndex;
		gItemSocketOption.GetSocketOption(&lpObj->pInventory[lpMsg->Ipos],SocketOption,SocketIndex);

		int serial2 = lpObj->pInventory[lpMsg->Ipos].m_Number;
		char szItemName[50] = "Item";
		BYTE JOHOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[lpMsg->Ipos]);
		BYTE JOHOptionLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pInventory[lpMsg->Ipos]);

		if ( gObjCheckSerial0ItemList(&lpObj->pInventory[lpMsg->Ipos]) != FALSE )
		{
			MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(13, 26)));
			LogAddTD("[ANTI-HACK][Serial 0 Item] [ItemDrop] (%s)(%s) Item(%s) Pos(%d)", lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->Ipos].GetName(), lpMsg->Ipos);

			CopyItem = TRUE;
			pResult.Result = false;
			bIsItem = FALSE;
		}

		if ( !::gObjInventorySearchSerialNumber(lpObj, serial2))
		{
			CopyItem = TRUE;
			pResult.Result = false;
			bIsItem = FALSE;
		}

		if ( bIsItem == TRUE )
		{
			strcpy(szItemName, lpObj->pInventory[lpMsg->Ipos].GetName());

			if ( type == ITEMGET(14,11))
			{
				::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				pResult.Result = true;

				if ( level == 1 )
				{
					::StarOfXMasOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of StarOfXMas Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 2 )
				{
					::FireCrackerOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of FireCracker Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 3 )
				{
					::HeartOfLoveOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of HeartOfLove Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 6 )
				{
					::GoldMedalOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of GoldMedal Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 5 )
				{
					::SilverMedalOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of SilverMedal Serial:%d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 7 )
				{
					::EventChipOpenEven(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of EventChip Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 8 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 50000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-8 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 9 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 100000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-9 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 10 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 150000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox-10 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 11 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 200000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox1-11 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 12 )
				{
					::EledoradoBoxOpenEven(&gObj[aIndex], level, 2, 250000);
					LogAddTD("[%s][%s][%d]%d/%d Used box of ElradoraBox1-12 Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 13 )
				{
					::DarkLordHeartItemBoxOpen(&gObj[aIndex]);
					LogAddTD("[%s][%s][%d]%d/%d Used box of DarkLordHeart Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else if ( level == 0 )
				{
					::LuckyBoxOpenEven(&gObj[aIndex]);
					LogAddTD(lMsg.Get(MSGGET(1, 222)), lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
				}
				else
				{
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}

				LogAddTD("[%s][%s] Used box of luck (level:%d)", lpObj->AccountID, lpObj->Name, level);
			}
			else if ( type == ITEMGET(12,32) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				RedRibbonBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(12,33) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GreenRibbonBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(12,34) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				BlueRibbonBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas RibbonBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,32) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				PinkChocolateBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,33)  && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				RedChocolateBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);

			}
			else if ( type == ITEMGET(14,34) && level == 0  )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				BlueChocolateBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used Valentine's Day ChocolateBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,32)  && level == 1 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				LightPurpleCandyBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,33)  && level == 1  )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				VermilionCandyBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,34)  && level == 1 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				DeepBlueCandyBoxOpen(&gObj[aIndex]);
				LogAddTD("[%s][%s][%d]%d/%d Used WhiteDay CandyBox Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,45) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				HallowinDayEventItemBoxOpen(lpObj);
				LogAddTD("[%s][%s][%d]%d/%d Used Pumpkin of Luck Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,51) ) //Season 2.5 add-on Christmas Star identical
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				ChristmasStarDrop(lpObj);
				LogAddTD("[%s][%s][%d]%d/%d Used Christmas-Star Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,63) ) //Season 2.5 add-on FireCraker (from Natasha Seller) identical
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				ChristmasStarDrop(lpObj);
				LogAddTD("[%s][%s][%d]%d/%d Used FireCraker Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14,84) ) //Season 3.0 add-on Cherry Blossom Play-Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				CherryBlossomEventItemBoxOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used CherryBox Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14,99) ) //Season 4.0 add-on XMas FireCrack
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				ChristmasFireCrackDrop(lpObj);//ChristmasFireCrackerDrop
				LogAddTD("[%s][%s][%d]%d/%d Used FireCraker Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)", lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(12,26) )
			{
				switch ( level )
				{
					case 0:
						{
							gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
							pResult.Result = true;
							int ltype = ITEMGET(12,26);
							int randor = rand()%100;
							int Level;

							if ( randor < 10 )
							{
								Level = 3;
							}
							else if ( randor < 45 )
							{
								Level = 1;
							}
							else
							{
								Level = 2;
							}

							ItemSerialCreateSend(aIndex, lpObj->MapNumber, lpObj->X, lpObj->Y, ltype, Level, 1, 0, 0, 0, lpObj->m_Index, 0, 0);
							
							LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Mysterious Bead : Drop (%d) - serial:%u", lpObj->AccountID, lpObj->Name, Level, serial);
						}
						break;
					case 1: case 2: case 3:
						{
							gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
							pResult.Result = true;

							PMSG_SERVERCMD ServerCmd;

							PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
							ServerCmd.CmdType = 0;
							ServerCmd.X = lpObj->X;
							ServerCmd.Y = lpObj->Y;

							MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
							DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
													
							LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Crystal (%d) - serial:%u", lpObj->AccountID, lpObj->Name, level, serial);
						}
						break;
					case 4:
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Hidden Treasure Box - serial:%u", lpObj->AccountID, lpObj->Name,  serial);

						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = true;
						HiddenTreasureBoxItemBoxOpen(&gObj[aIndex]);

						break;
					case 5:
						LogAddTD("[Hidden TreasureBox Event] [%s][%s] Throw Surprise Treasure Box - serial:%u", lpObj->AccountID, lpObj->Name,  serial);

						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = true;

						if ( (rand()%10000) < g_iHiddenTreasureBoxOfflineRate )
						{
							EGReqRegHTOfflineGift(lpObj->m_Index);
						}
						else
						{
							int money = 1000000;
							MapC[lpObj->MapNumber].MoneyItemDrop(money, lpObj->X, lpObj->Y);
							LogAddTD("[Hidden TreasureBox Event] [%s][%s] Event ZenDrop : %d : (%d)(%d/%d)", lpObj->AccountID, lpObj->Name, money, lpObj->MapNumber, lpObj->X, lpObj->Y);
						}
						break;
				}
			}
			else if( type == ITEMGET(13,20) && level == 1 ) // Wizard Ring A
			{
				if ( gObj[aIndex].Level >= 40 )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					::RingOfHeroBoxOpen(&gObj[aIndex]); //season2.5
				}
				else
				{
					PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsg((TNotice *)&pNotice, 10, lMsg.Get(MSGGET(4, 187)));
					TNotice::SetNoticeProperty((TNotice *)&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
					TNotice::SendNoticeToUser(aIndex, (TNotice *)&pNotice);
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}
			}
			else if ( type == ITEMGET(13,20) && level == 2 ) // Wizard Ring B
			{
				if ( gObj[aIndex].Level >= 80 )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					::FriendShipItemBoxOpen(&gObj[aIndex]);
				}
				else
				{
					PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsg((TNotice *)&pNotice, 10, lMsg.Get(MSGGET(4, 191)));
					TNotice::SetNoticeProperty((TNotice *)&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
					TNotice::SendNoticeToUser(aIndex, (TNotice *)&pNotice);
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}
			}
			else if ( type == ITEMGET(14,28) ) // Lost Map
			{
				if ( g_KalimaGate.CreateKalimaGate(aIndex, level, lpMsg->px, lpMsg->py))
				{
					LogAddTD("[Kalima] [%s][%s] Success to Make Kalima Gate (Lost Kalima Map Serial:%u)", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pInventory[lpMsg->Ipos].m_Number);
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else
				{
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}
			}
#ifdef INTERNATIONAL_JAPAN
			else if ( type == ITEMGET(14,20) && ( ((level-1<0)?FALSE:(level-1>4)?FALSE:TRUE) != FALSE ) ) // Remedy Of Love
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				Japan1StAnivBoxOpen(&gObj[aIndex], level);
			}
#endif
			else if ( type == ITEMGET(13,7) ) // Contract (Summon)
			{
				int iMonsterID = 0;

				if ( level == 0 )
				{
					iMonsterID = 286;
				}
				else if ( level == 1 )
				{
					iMonsterID = 287;
				}

				if ( g_CsNPC_Mercenary.CreateMercenary(aIndex, iMonsterID, lpMsg->px, lpMsg->py) )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else
				{
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if ( type == ITEMGET(13,11) ) // Guardian / LifeStone
			{
				BOOL bResult = FALSE;

				if ( level == 0 )
					bResult = g_CsNPC_Guardian.CreateGuardian(aIndex);
//#if(_GSCS==1)
				else if ( level == 1 )
					bResult = g_CsNPC_LifeStone.CreateLifeStone(aIndex);
//#endif

				if ( bResult == TRUE )
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				else
				{
					pResult.Result = false;
					DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return FALSE;
				}
			}
			else if ( type == ITEMGET(14,52) ) //Season 2.5 add-on GM Box identical
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GMPresentBoxItemBagOpen(lpObj); //season 3.0 changed arguments
				LogAddTD("[%s][%s][%d]%d/%d Used GM Present Box Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
#ifdef PCBANG
			else if ( type == ITEMGET(14,55) && level == 0 ) //Season 4.5 add-on PCBang Green Chaos Box identical
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				PCBangGreenChaosBoxItemBagOpen(&gObj[aIndex],lpObj->MapNumber,lpObj->X,lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of Green Chaos Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,56) && level == 0 ) //Season 4.5 add-on PCBang Red Chaos Box identical
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				PCBangRedChaosBoxItemBagOpen(&gObj[aIndex],lpObj->MapNumber,lpObj->X,lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of Red Chaos Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if ( type == ITEMGET(14,57) && level == 0 ) //Season 4.5 add-on PCBang Purple Chaos Box identical
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				PCBangPurpleChaosBoxItemBagOpen(&gObj[aIndex],lpObj->MapNumber,lpObj->X,lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of Purple Chaos Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
#endif
#ifdef SEASON6DOT3_ENG
			else if( type == ITEMGET(14, 123) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GoldenBoxItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Golden Box Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14, 124) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				SilverBoxItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Silver Box Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14, 141) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				ShineJewelleryCaseItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used ShineJewelleryCase Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14, 142) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				RefinedJewelleryCaseItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used RefinedJewelleryCase Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14, 143) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				IronJewelleryCaseItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used IronJewelleryCase Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14, 144) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				OldJewelleryCaseItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Golden Box Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14, 157) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				BoxOfGreenColorItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of GreenColor Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14, 157) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				BoxOfGreenColorItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of GreenColor Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14, 158) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				BoxOfRedColorItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of RedColor Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
			else if( type == ITEMGET(14, 159) && level == 0 )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				BoxOfPurpleColorItemBagOpen(&gObj[aIndex], lpObj->MapNumber, lpObj->X, lpObj->Y);
				LogAddTD("[%s][%s][%d]%d/%d Used Box Of PurpleColor Item Serial:%u (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}
#endif
			else
			{
				int OwnerIndex = aIndex;

				if ( (type >= ITEMGET(14,0) && type <= ITEMGET(14,10)) || (type == ITEMGET(4,7) ) || type ==ITEMGET(4,15))
				{
					OwnerIndex = -1;
				}

				if ( MapC[map].ItemDrop( type, level, dur, lpMsg->px, lpMsg->py, Option1, Option2, Option3,	NOption, SOption, serial2, OwnerIndex, PetLevel, PetExp, btItemEffectEx, SocketOption, SocketIndex) == TRUE )
				{
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					pResult.Result = true;

					if ( drop_type == FALSE )
					{
						LogAddTD(lMsg.Get(MSGGET(1, 223)), lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], SOption, btItemEffectEx>>7, JOHOption, JOHOptionLevel);
					}
					else
					{
						LogAddTD(lMsg.Get(MSGGET(1, 224)), lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], SOption, btItemEffectEx>>7, JOHOption, JOHOptionLevel); //season3.5 fixed
					}
				}
				else
				{
					pResult.Result = false;
				}
			}
		}
		else
		{
			LogAdd(lMsg.Get(MSGGET(1, 225)));
			pResult.Result = false;
		}
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	if ( CopyItem != FALSE )
	{
		::GCItemListSend(aIndex);
#ifdef PERIOD
		g_PeriodItemEx.OnRequestPeriodItemList(&gObj[aIndex]);
#endif
	}

	if ( pResult.Result == 1 )
	{
		if ( lpMsg->Ipos < 12 )
		{
			if ( lpMsg->Ipos == 10 || lpMsg->Ipos == 11 )
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			::gObjMakePreviewCharSet(aIndex);

			PMSG_USEREQUIPMENTCHANGED pChange;

			PHeadSetB((LPBYTE)&pChange, 0x25, sizeof(pChange));
			pChange.NumberH = SET_NUMBERH(aIndex);
			pChange.NumberL = SET_NUMBERL(aIndex);
			ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[lpMsg->Ipos]);
			pChange.ItemInfo[1] = lpMsg->Ipos <<  4;
			pChange.ItemInfo[1] |= LevelSmallConvert(aIndex, lpMsg->Ipos) & 0x0F;

			DataSend(aIndex, (LPBYTE)&pChange, pChange.h.size);
			MsgSendV2(lpObj, (LPBYTE)&pChange, pChange.h.size);
		}
	}

	return pResult.Result;
}

struct PMSG_INVENTORYITEMMOVE_RESULT
{
	PBMSG_HEAD h;	// C3:24
	BYTE result;	// 3
	BYTE Pos;	// 4
	BYTE ItemInfo[MAX_ITEM_INFO];	// 5
};

void GCItemMoveResultSend(int aIndex, BYTE result, BYTE pos, BYTE ItemInfo[])  //00446170
{
	PMSG_INVENTORYITEMMOVE_RESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x24;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = result;
	pMsg.Pos = pos;
	pMsg.ItemInfo[0] = ItemInfo[0];
	pMsg.ItemInfo[1] = ItemInfo[1];
	pMsg.ItemInfo[2] = ItemInfo[2];
	pMsg.ItemInfo[3] = ItemInfo[3];
	pMsg.ItemInfo[4] = ItemInfo[4];
	pMsg.ItemInfo[5] = ItemInfo[5];
	pMsg.ItemInfo[6] = ItemInfo[6];

	//season4 add-on
	pMsg.ItemInfo[7] = ItemInfo[7];
	pMsg.ItemInfo[8] = ItemInfo[8];
	pMsg.ItemInfo[9] = ItemInfo[9];
	pMsg.ItemInfo[10] = ItemInfo[10];
	pMsg.ItemInfo[11] = ItemInfo[11];

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

//00452A30	-> OK
void CGInventoryItemMove(PMSG_INVENTORYITEMMOVE * lpMsg, int aIndex)
{
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		::GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	if ( !::gObjIsConnectedGP(aIndex))
	{
		LogAddC(2,"[%s][%d] error-L3", __FILE__, __LINE__);
		::GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];	//ok
	int it_type = 0;

	if ( lpObj->m_ReqWarehouseOpen != false )
	{
		LogAddC(2,"[%s][%d] error-L3", __FILE__, __LINE__);
		GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		return;
	}

	int source = lpMsg->source_item_num;
	int target = lpMsg->target_item_num;

	if ( lpMsg->tFlag == 2 || lpMsg->sFlag == 2 )
	{
		it_type = 6;
	}

	if ( lpMsg->tFlag == 0 && lpMsg->sFlag == 0 )
	{
		it_type = 8;
	}

	if ( (lpMsg->tFlag == 0 && lpMsg->sFlag == 4) || (lpMsg->tFlag == 4 && lpMsg->sFlag == 0) || (lpMsg->tFlag == 4 && lpMsg->sFlag == 4) )
	{
		it_type = 8;
	}

	if( (lpMsg->tFlag == lpMsg->sFlag) && (source == target)) //season 3.0 add-on (New-Antihack)
	{
		GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		return;
	}

	if (::gObjCanItemTouch(lpObj, it_type) == FALSE)
	{
		if ( lpMsg->sItemInfo[0] == ITEMGET(4,7) || lpMsg->sItemInfo[0] == ITEMGET(4,15) )
		{
			if ( target >= 12 || source < 12 )
			{
				::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
				return;
			}
		}
		else
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}

	if ( lpObj->DieRegen != FALSE )
	{
		::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		return;
	}

	ItemInfo[0] = lpMsg->sItemInfo[0];
	ItemInfo[1] = lpMsg->sItemInfo[1];
	ItemInfo[2] = lpMsg->sItemInfo[2];
	ItemInfo[3] = lpMsg->sItemInfo[3];
	ItemInfo[4] = lpMsg->sItemInfo[4];
	ItemInfo[5] = lpMsg->sItemInfo[5];
	ItemInfo[6] = lpMsg->sItemInfo[6];

	//season4 add-on
	ItemInfo[7] = lpMsg->sItemInfo[7];
	ItemInfo[8] = lpMsg->sItemInfo[8];
	ItemInfo[9] = lpMsg->sItemInfo[9];
	ItemInfo[10] = lpMsg->sItemInfo[10];
	ItemInfo[11] = lpMsg->sItemInfo[11];

	if ( lpMsg->sFlag == 3 || lpMsg->tFlag == 3 )	//OK
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 7 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Chaosbox", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			LogAddTD("[%s][%s] error-L3 : CBMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		
			return;
		}
	}

	if ( lpMsg->sFlag == 5 || lpMsg->tFlag == 5 )	//OK
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 13 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not DarkTrainerBox", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			LogAddTD("[%s][%s] error-L3 : DarkTrainerBoxMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		
			return;
		}
	}

	if ( lpMsg->sFlag == 6 || lpMsg->sFlag == 7 || lpMsg->sFlag == 8 ||	//OK
		 lpMsg->tFlag == 6 || lpMsg->tFlag == 7 || lpMsg->tFlag == 8 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 7 )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			LogAddTD("[%s][%s] error-L3 : JewelOfHarmony Mixing",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

			return;
		}
	}

	if ( lpMsg->sFlag == 9 || lpMsg->tFlag == 9 )	//OK
	{
		if ( lpObj->m_IfState.use	 < 1 || lpObj->m_IfState.type !=7 )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			LogAddTD("[%s][%s] error-L3 : JewelOfHarmony Mixing",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

			return;
		}
	}

	//season4 add-on (socket mix)
	if( lpMsg->sFlag == 11 ||	//OK
		lpMsg->sFlag == 12 ||
		lpMsg->sFlag == 13 ||
		lpMsg->sFlag == 14 ||
		lpMsg->tFlag == 11 ||
		lpMsg->tFlag == 12 ||
		lpMsg->tFlag == 13 ||
		lpMsg->tFlag == 14 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type !=7 )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			LogAddTD("[%s][%s] error-L3 : SocketItem Mixing",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

			return;
		}
	}

#ifdef SEASON6DOT3_ENG
	if( lpMsg->sFlag == 15 || lpMsg->sFlag == 16 )	//OK
	{
		if( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 7 )
		{
			GCItemMoveResultSend(aIndex, 0xFF, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not ChaosBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
		// ----
		if( lpObj->ChaosLock == true )
		{
			LogAddTD("[%s][%s] error-L3 : SocketItem Mixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, 0xFF, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}
#endif

	if ( lpMsg->sFlag == 2 || lpMsg->tFlag == 2 )	//OK
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 6 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Warehouse", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )	//OK
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			LogAdd("[%s][%s] error-L1 : used not Trade", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}
	}

	if ( lpMsg->sFlag == 2 && lpMsg->tFlag == 0 )	//OK
	{
		int money = ::GetWarehouseUsedHowMuch(lpObj->Level, lpObj->m_nMasterLevel, lpObj->WarehousePW);

		if ( (lpObj->Money - money ) < 1 && (lpObj->WarehouseMoney - money) < 1)
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(MSGGET(6, 69)), money);
			::GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);

			return;
		}

		if ( ::bCanWarehouseLock == TRUE )
		{
			if ( gObj[aIndex].WarehouseLock == 1 )
			{
				::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
				::GCServerMsgStringSend(lMsg.Get(MSGGET(6, 70)), lpObj->m_Index, 1);

				return;
			}
		}
	}

	if( it_type != 8 )
	{
		int type = (ItemInfo[0] + ((ItemInfo[3] & 0x80 )*2)) + ((ItemInfo[5] & 0xF0)<<5);
		LPITEM_ATTRIBUTE lpItemAttr = GetItemAttr(type);

		if ( lpItemAttr == NULL )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}

		if ( lpItemAttr->QuestItem != false )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}

	if ( (lpMsg->sFlag == 0 && lpMsg->tFlag == 0) ||
 		 (lpMsg->sFlag == 2 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 2) ||
 		 (lpMsg->sFlag == 2 && lpMsg->tFlag == 2) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 3) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 5) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 6) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 7) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 8) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 9) ||
		 (lpMsg->sFlag == 9 && lpMsg->tFlag == 9) ||
		 (lpMsg->sFlag == 9 && lpMsg->tFlag == 0) ||

		 //season4 add-on start
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 11) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 12) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 13) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 14) ||

		 (lpMsg->sFlag == 11 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 12 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 13 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 14 && lpMsg->tFlag == 0) ||

		 //1.01.00
		 (lpMsg->sFlag == 15 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 16 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 15) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 16) ||

		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 10) ||
		 (lpMsg->sFlag == 10 && lpMsg->tFlag == 10) ||
		 (lpMsg->sFlag == 10 && lpMsg->tFlag == 0) ||

		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 4) ||
		 (lpMsg->sFlag == 4 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 4 && lpMsg->tFlag == 4) ||
		 (lpMsg->sFlag == 5 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 5 && lpMsg->tFlag == 5) ||
		 (lpMsg->sFlag == 6 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 6 && lpMsg->tFlag == 6) ||
		 (lpMsg->sFlag == 7 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 7 && lpMsg->tFlag == 7) ||
		 (lpMsg->sFlag == 8 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 8 && lpMsg->tFlag == 8) ||

		 //season4 add-on start
		 (lpMsg->sFlag == 11 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 12 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 13 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 14 && lpMsg->tFlag == 0) ||

		 (lpMsg->sFlag == 11 && lpMsg->tFlag == 11) ||
		 (lpMsg->sFlag == 12 && lpMsg->tFlag == 12) ||
		 (lpMsg->sFlag == 13 && lpMsg->tFlag == 13) ||
		 (lpMsg->sFlag == 14 && lpMsg->tFlag == 14) ||

		 //1.01.00
		 (lpMsg->sFlag == 15 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 15 && lpMsg->tFlag == 15) ||
		 (lpMsg->sFlag == 16 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 16 && lpMsg->tFlag == 16) ||

		 (lpMsg->sFlag == 3 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 3 && lpMsg->tFlag == 3) )
	{
		BOOL DurSSend;
		BOOL DurTSend;

		result = gObjInventoryMoveItem(aIndex, source, target, DurSSend, DurTSend, lpMsg->sFlag, lpMsg->tFlag, (LPBYTE)&ItemInfo);
		::GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);

		if ( DurSSend != FALSE )
			::GCItemDurSend(aIndex, source, lpObj->pInventory[source].m_Durability, FALSE);

		if ( DurTSend != FALSE )
			::GCItemDurSend(aIndex, target, lpObj->pInventory[target].m_Durability, FALSE);

		int loc77 = (lpMsg->sItemInfo[0] + ((lpMsg->sItemInfo[3] & 0x80 )*2)) + ((lpMsg->sItemInfo[5] & 0xF0)<<5); 

		if( loc77 == ITEMGET(14,29) || loc77 == ITEMGET(14, 101) || loc77 == ITEMGET(14, 110) ) //6.2 OK
		{
			if( lpObj->pInventory[target].m_Durability == 0.0f && (lpMsg->sFlag == FALSE) && (lpMsg->tFlag == FALSE))
			{
				GCInventoryItemDeleteSend(aIndex,target,0);
			}
		}

		if ( result != 0xFF && lpMsg->sFlag == 2 && lpMsg->tFlag == 0) //OK
		{
			int money = ::GetWarehouseUsedHowMuch(lpObj->Level, lpObj->m_nMasterLevel, lpObj->WarehousePW);

			if ( (lpObj->Money - money) > 0 )
			{
				int iZen = lpObj->Money;
				lpObj->Money -= money;
				GCMoneySend(lpObj->m_Index, lpObj->Money);

				LogAdd("Pay WareHouse Money(In Inventory) : %d - %d = %d", iZen, money, lpObj->Money);
			}
			else if ( (lpObj->WarehouseMoney - money) > 0 )
			{
				int iZen = lpObj->WarehouseMoney;
				lpObj->WarehouseMoney -= money;
	
				LogAdd("Pay WareHouse Money(In WareHouse) : %d - %d = %d", iZen, money, lpObj->WarehouseMoney);

				GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
			}
		}

#ifdef PERIOD
		g_PeriodItemEx.SendPeriodItemInfoOnce(&gObj[aIndex], &lpObj->pInventory[target]);
#endif
		return;
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 1 )
	{
		result = gObjTradeTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
		return;
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )
	{
		result = gObjTradeInventoryMove(lpObj, source, target);

		if ( result == 0xFF )
		{
			GCItemMoveResultSend(aIndex, 0xFF, target, (LPBYTE)&ItemInfo);
			CGTradeCancelButtonRecv(aIndex);
		}
		else
		{
			GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
		}

		if ( result == 0xFF )
		{
			lpObj->m_IfState.state = 0;
			GCItemListSend(aIndex);
#ifdef PERIOD
			g_PeriodItemEx.OnRequestPeriodItemList(&gObj[aIndex]);
#endif
		}
		
		if ( lpObj->TargetNumber >= 0 )
		{
			lpObj->TradeOk = false;
			gObj[gObj[aIndex].TargetNumber].TradeOk = false;
			GCTradeOkButtonSend(lpObj->TargetNumber, 2);
			GCTradeOkButtonSend(aIndex, 0);
		}
		return;
	}

	if ( lpMsg->sFlag == 0 && lpMsg->tFlag == 1 )
	{
		result = gObjInventoryTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
	}
}

void GCEquipmentChange(int aIndex, int pos) 
{
	PMSG_USEREQUIPMENTCHANGED pChange;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pChange, 0x25, sizeof(PMSG_USEREQUIPMENTCHANGED));
	pChange.NumberH = SET_NUMBERH(aIndex);
	pChange.NumberL = SET_NUMBERL(aIndex);
	ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[pos]);
	pChange.ItemInfo[1] = pos << 4;
	pChange.ItemInfo[1] |= LevelSmallConvert(aIndex, pos) & MAX_ITEM_LEVEL;

	MsgSendV2(lpObj, (LPBYTE)&pChange, pChange.h.size);
}

struct PMSG_SHOPITEMCOUNT
{
	PWMSG_HEAD h;
	BYTE Type;
	BYTE count;
};

void CGTalkRequestRecv(PMSG_TALKREQUEST * lpMsg, int aIndex) 
{
	LPOBJ lpObj = &gObj[aIndex];
	int DealerNumber ;
	PMSG_TALKRESULT pResult;
	PMSG_SHOPITEMCOUNT pShopItemCount;

	if ( !PacketCheckTime(lpObj))
	{
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	if ( !gObjIsConnectedGP(aIndex) )
	{
		LogAddTD("[ANTI-HACK][NPC Talk] Couldn't talk.. (%s)(%s)", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	BYTE SendByte[1024];
	int lOfs = 0;

	if ( lpObj->m_IfState.use > 0 )
	{
		return;
	}

	DealerNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	
	if ( DealerNumber < 0 || DealerNumber > OBJMAX-1)
	{
		return;
	}

	if ( lpObj->MapNumber != gObj[DealerNumber].MapNumber )
	{
		return;
	}

	if( gObj[DealerNumber].Class == 367 )
	{
		if ( (lpObj->X < (gObj[DealerNumber].X-5) )|| (lpObj->X> (gObj[DealerNumber].X+5) )||(lpObj->Y < (gObj[DealerNumber].Y-10)) ||(lpObj->Y > (gObj[DealerNumber].Y+5)))
		{
			return;
		}
	}
	else if ( (lpObj->X < (gObj[DealerNumber].X-5) )|| (lpObj->X> (gObj[DealerNumber].X+5) )||(lpObj->Y < (gObj[DealerNumber].Y-5)) ||(lpObj->Y > (gObj[DealerNumber].Y+5)))
	{
		return;
	}

	int ShopNum = gObj[DealerNumber].ShopNumber;

	if ( gObj[DealerNumber].Type == OBJ_NPC )
	{
		if( NpcTalk(&gObj[DealerNumber], lpObj) == TRUE )
		{
			return;
		}
	}

	if ( ::gObjFixInventoryPointer(aIndex) == false )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[DealerNumber].Class == 234 )
	{
		pResult.h.c = 0xC3;
		pResult.h.headcode = 0x30;
		pResult.h.size = sizeof(pResult);
		pResult.result = 0;
		pResult.result = 5;

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if( gObj[DealerNumber].Type == OBJ_NPC && ShopNum < 0 )
	{
		return;
	}

	if ( ShopNum < MAX_SHOP )
	{
		if ( ShopC[ShopNum].ItemCount < TRUE )
		{
			if ( (rand()%2) != 0 )
				ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 119)), aIndex);
			else
				ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 120)), aIndex);
			return;
		}

		if( !gPkLimitFree
#ifdef GENS
			&& !gGensSystem.IsMapBattleZone(lpObj->MapNumber)
#endif	
			)
		{
			BOOL bPlayerKiller = FALSE; //season 2.5 add-on
#ifdef PARTYKILLPKSET
			if( lpObj->PartyNumber >= 0) //season 2.5 add-on
			{
				if( (gParty.GetPKPartyPenalty(lpObj->PartyNumber)) > 4)
				{
					bPlayerKiller = TRUE;
				}
			}
			else
			{
				if( lpObj->m_PK_Level > 4 )
				{
					bPlayerKiller = TRUE;
				}
			}
#else
			if( lpObj->m_PK_Level > 4 )
			{
				bPlayerKiller = TRUE;
			}
#endif
			if ( bPlayerKiller == TRUE ) //season 2.5 changed
			{
				if ( (rand()%2) != 0 )
					ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 121)), aIndex);
				else
					ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 122)), aIndex);

				return;
			}
		}
	}

	if ( gMerryXMasNpcEvent == TRUE )
	{
		if ( (rand()%6) == 0 )
		{
			ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 123)), aIndex);
		}
	}

	if ( ::gHappyNewYearNpcEvent == TRUE )
	{
		if ( (rand()%6) == 0 )
		{
			ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 124)), aIndex);
		}
	}

	lpObj->TargetShopNumber = ShopNum;
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 3;
	lpObj->m_ShopTime = 0;
	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0;

	if ( ShopNum == 100 )
	{
		if( gObj[aIndex].m_ReqWarehouseOpen != false )
		{
			return;
		}
		
		gObj[aIndex].m_ReqWarehouseOpen = true;
		lpObj->m_IfState.type = 6;
		lpObj->m_IfState.state = 0;
	}
	else if ( ShopNum == 101 )
	{
		if ( bCanChaosBox == TRUE )
		{
			if ( lpObj->m_bPShopOpen == true )
			{
				LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);
				return;
			}

			lpObj->m_IfState.type = 7;
			lpObj->m_IfState.state = 0;
			pResult.result = 3;

			if(gObj[DealerNumber].Class == 450) //Season 3.0 add-on
			{
				pResult.result = 0x16; //season4 changed
			}

			if(gObj[DealerNumber].Class == 478) //Season 4.0 add-on
			{
				pResult.result = 0x20;
			}

			lpObj->bIsChaosMixCompleted = false;

			pResult.level1 = gDQChaosSuccessRateLevel1;
			pResult.level2 = gDQChaosSuccessRateLevel2;
			pResult.level3 = gDQChaosSuccessRateLevel3;
			pResult.level4 = gDQChaosSuccessRateLevel4;
			pResult.level5 = gDQChaosSuccessRateLevel5;
			pResult.level6 = gDQChaosSuccessRateLevel6;
			pResult.level7 = gDQChaosSuccessRateLevel7; //season 3.0 add-on

			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			gObjInventoryTrans(lpObj->m_Index);

			LogAddTD("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
			gObjItemTextSave(lpObj);
			gObjWarehouseTextSave(lpObj);
			GCAnsCsMapSvrTaxInfo( lpObj->m_Index, 1, ::g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		}
	}
	else
	{
		if(gObj[DealerNumber].Class == 492) //Season 4.6 add-on
		{
			pResult.result = 0x22;

		}

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}

	if ( ShopNum == 100 )
	{
		gObj[aIndex].WarehouseCount = 0;
		GDGetWarehouseList(aIndex, gObj[aIndex].AccountID);

		return;
	}

	if ( ShopNum == 101 )
	{
		return;
	}
	else
	{
		lOfs = 0;

		lOfs += sizeof(pShopItemCount);

		int size = lOfs + ShopC[ShopNum].SendItemDataLen;
		PHeadSetW((LPBYTE)&pShopItemCount, 0x31, size);
		pShopItemCount.Type = 0;
		pShopItemCount.count = ShopC[ShopNum].ItemCount;
		memcpy(SendByte, &pShopItemCount, sizeof(pShopItemCount));
		memcpy(&SendByte[lOfs], ShopC[ShopNum].SendItemData, ShopC[ShopNum].SendItemDataLen);

		DataSend(aIndex, SendByte, size);
		GCAnsCsMapSvrTaxInfo(lpObj->m_Index,2, g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index));
	}
}

void GCUserWarehouseSend(LPOBJ lpObj) 
{
	if ( lpObj->m_IfState.type != 6 )
	{
		return;
	}

	if ( lpObj->m_IfState.type == 6 && lpObj->m_IfState.state == 1 )
	{
		return;
	}

	PMSG_SHOPITEMCOUNT pMsg;
	BYTE SendByte[WAREHOUSE_BUFF];
	int lOfs = sizeof(pMsg);

	PHeadSetW((LPBYTE)&pMsg, 0x31, 0);
	pMsg.count = 0;
	pMsg.Type = 0;

	for ( int n=0;n<WAREHOUSE_SIZE;n++)
	{
		if ( lpObj->pWarehouse[n].IsItem() == TRUE )
		{
			SendByte[lOfs] = n;
			lOfs++;
			ItemByteConvert(&SendByte[lOfs], lpObj->pWarehouse[n] );
			lOfs += MAX_ITEM_INFO;
			pMsg.count ++;
		}
	}

	pMsg.h.sizeH = SET_NUMBERH(lOfs);
	pMsg.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(SendByte, &pMsg, sizeof(pMsg));

	DataSend(lpObj->m_Index, SendByte, lOfs);
	GCWarehouseInventoryMoneySend(lpObj->m_Index, 1, lpObj->Money, lpObj->WarehouseMoney);

	if ( lpObj->m_IfState.type == 6 )
	{
		lpObj->m_IfState.state = 1;
		lpObj->WarehouseSave = TRUE;
	}
}

struct PMSG_BUYRESULT
{
	PBMSG_HEAD h;
	BYTE Result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

void CGBuyRequestRecv(PMSG_BUYREQUEST * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_BUYRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));
	pResult.Result = -1;

	if ( !PacketCheckTime(lpObj))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].m_ShopTime == 0 )
	{
		gObj[aIndex].m_ShopTime = 1;
	}

	if ( gObj[aIndex].m_ShopTime > 	60 )
	{
		if ( lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 3 )
		{
			pResult.Result = 0xFE; //season 3.0 add-on
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.use = 0;
			lpObj->m_IfState.type = 0;
		}

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		if ( lpObj->m_IfState.type != 3 )
		{
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	int tShop = lpObj->TargetShopNumber;

	if ( tShop < 0 || tShop > MAX_SHOP-1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Pos > MAX_ITEM_IN_SHOP-1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	if ( ShopC[tShop].m_item[lpMsg->Pos].IsItem() == TRUE )
	{
		if( ShopC[tShop].m_item[lpMsg->Pos].m_Type == ITEMGET(13, 5) )	//fix
		{
			if( ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney < 1000000 )
			{
				ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney = 1000000;
			}
		}
		// ----
		int iStoreTaxMoney = ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney + (int)((__int64)ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney * (__int64)g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index) / (__int64)100);
		
		if ( iStoreTaxMoney < 0 )
		{
			iStoreTaxMoney  = 0;
		}

		int iStoreTaxMoney2 = (int)((__int64)ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney * (__int64)g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index) / (__int64)100);

		if ( iStoreTaxMoney2 < 0 )
		{
			iStoreTaxMoney2  = 0;
		}

		if ( lpObj->Money < iStoreTaxMoney )
		{
			pResult.Result = -1;
		}
		else
		{
			BOOL bNoItem = TRUE;

			if ( (ShopC[tShop].m_item[lpMsg->Pos].m_Type >= ITEMGET(14,0) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= ITEMGET(14,8)) || (ShopC[tShop].m_item[lpMsg->Pos].m_Type >= ITEMGET(14,35) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= ITEMGET(14,40)))
			{
				int dur = ShopC[tShop].m_item[lpMsg->Pos].m_Durability;

				if ( dur == 0 )
				{
					dur = 1;
				}

				if ( ::gObjSearchItem(&gObj[aIndex], ShopC[tShop].m_item[lpMsg->Pos].m_Type,dur, ShopC[tShop].m_item[lpMsg->Pos].m_Level) == TRUE )
				{
					bNoItem = FALSE;
					lpObj->Money -= iStoreTaxMoney;

					::g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);

					if ( lpObj->Money < 0 )
					{
						lpObj->Money = 0;
					}
		
					::GCMoneySend(aIndex, lpObj->Money);
				}
			}

			if ( bNoItem != FALSE )
			{
				BYTE btRareItem = -1;//Season 4.6

				switch(ShopC[tShop].m_item[lpMsg->Pos].m_Type)
				{
					case ITEMGET(13,71):
						btRareItem = 0;
						break;
					case ITEMGET(13,72):
						btRareItem = 1;
						break;
					case ITEMGET(13,73):
						btRareItem = 2;
						break;
					case ITEMGET(13,74):
						btRareItem = 3;
						break;
					case ITEMGET(13,75):
						btRareItem = 4;
						break;
				}

				if(btRareItem != -1 && lpObj->TargetShopNumber == 18)
				{
					if(g_bUseGambleSystem == FALSE)
					{
						GCServerMsgStringSend(lMsg.Get(3418),aIndex,1);
					}
					else
					{
						if(gObj[aIndex].m_bUseGambleState != false) //multi buy prevention for inventory bug
						{
							return;
						}
						
						int iItemHeight = 4;
						int iItemWidth = 2;

						if(!CheckInventoryEmptySpace(lpObj, iItemHeight, iItemWidth))
						{
							//GCServerMsgStringSend(lMsg.Get(3419), gObj[aIndex].m_Index, 1);
							DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
							return;
						}

						if(lpObj->Money >= g_iGambleSystemMoney)
						{
							gObj[aIndex].m_bUseGambleState = true;
							g_GambleSystem.TryGamble(aIndex,238,0,0,btRareItem);
						}
					}
				}
				else
				{
					//-> Fix
					if(		ShopC[tShop].m_item[lpMsg->Pos].m_Type == ITEMGET(13, 4)
						||	ShopC[tShop].m_item[lpMsg->Pos].m_Type == ITEMGET(13, 5) )
					{
						PetItemSerialCreateSend(aIndex, 236, 0, 0, ShopC[tShop].m_item[lpMsg->Pos].m_Type,
							ShopC[tShop].m_item[lpMsg->Pos].m_Level, ShopC[tShop].m_item[lpMsg->Pos].m_Durability,
							ShopC[tShop].m_item[lpMsg->Pos].m_Option1, ShopC[tShop].m_item[lpMsg->Pos].m_Option2, 
							ShopC[tShop].m_item[lpMsg->Pos].m_Option3, -1, ShopC[tShop].m_item[lpMsg->Pos].m_NewOption,
							ShopC[tShop].m_item[lpMsg->Pos].m_SetOption);
						int lc64 = ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney;
						lpObj->Money -= iStoreTaxMoney;
						::g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);
						if( lpObj->Money < 0 )
						{
							lpObj->Money = 0;
						}
						GCMoneySend(aIndex, lpObj->Money);
						int iTaxRate = g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index);
						LogAddTD("[%s][%s] (%d) Shop buy [%d][%d][%d][%s] LEV:%d, DUR:%d, OP:[%d][%d][%d]", lpObj->AccountID, lpObj->Name, tShop, lc64, iTaxRate, lc64+iStoreTaxMoney2, ShopC[tShop].m_item[lpMsg->Pos].GetName(), ShopC[tShop].m_item[lpMsg->Pos].m_Level, ShopC[tShop].m_item[lpMsg->Pos].m_Durability, ShopC[tShop].m_item[lpMsg->Pos].m_Option1, ShopC[tShop].m_item[lpMsg->Pos].m_Option2, ShopC[tShop].m_item[lpMsg->Pos].m_Option3);
						DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						return;
					}
					// ----
					pResult.Result = gObjShopBuyInventoryInsertItem(aIndex, ShopC[tShop].m_item[lpMsg->Pos]);
				
					if( pResult.Result != 0xFF )
					{
						ItemByteConvert((LPBYTE)&pResult.ItemInfo, ShopC[tShop].m_item[lpMsg->Pos]);
						int lc64 = ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney;

						lpObj->Money -= iStoreTaxMoney;
						::g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);

						if ( lpObj->Money < 0 )
						{
							lpObj->Money = 0;
						}

						GCMoneySend(aIndex, lpObj->Money);

						int iTaxRate = g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index);

						LogAddTD("[%s][%s] (%d) Shop buy [%d][%d][%d][%s] LEV:%d, DUR:%d, OP:[%d][%d][%d]", lpObj->AccountID, lpObj->Name, tShop, lc64, iTaxRate, lc64+iStoreTaxMoney2, ShopC[tShop].m_item[lpMsg->Pos].GetName(), ShopC[tShop].m_item[lpMsg->Pos].m_Level, ShopC[tShop].m_item[lpMsg->Pos].m_Durability, ShopC[tShop].m_item[lpMsg->Pos].m_Option1, ShopC[tShop].m_item[lpMsg->Pos].m_Option2, ShopC[tShop].m_item[lpMsg->Pos].m_Option3);
				
					}
				}
			}
		}
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

struct PMSG_SELLRESULT
{
	PBMSG_HEAD h;
	BYTE Result;
	DWORD Money;
};

void CGSellRequestRecv(PMSG_SELLREQUEST * lpMsg, int aIndex) 
{
	PMSG_SELLRESULT pResult;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pResult, 0x33, sizeof(pResult));
	pResult.Result = false;
	pResult.Money = 0;

	if ( gObj[aIndex].CloseType != -1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != 3 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !PacketCheckTime(lpObj))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[%s][%s] CGSellRequestRecv() Failed : Transaction == 1, IF_TYPE : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;
	}

	int tShop = lpObj->TargetShopNumber;

	if ( tShop < 0 || tShop > MAX_SHOP-1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !gObjCanItemTouch(lpObj, 3))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Pos > MAIN_INVENTORY_SIZE-1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAdd(lMsg.Get(MSGGET(1, 227)), lpMsg->Pos);
		return;
	}

	if ( gObjCheckSerial0ItemList(&lpObj->pInventory[lpMsg->Pos]) != FALSE )
	{
		MsgOutput(aIndex, lMsg.Get(MSGGET(13, 26)));
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		LogAddTD("[ANTI-HACK][Serial 0 Item] [Sell Item] (%s)(%s) Item(%s) Pos(%d)", lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->Pos].GetName(), lpMsg->Pos);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14,11) )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	//season4 removed
	/*if(lpMsg->Pos >= 0 && lpMsg->Pos < 12?TRUE:FALSE) //Season 3.5 add-on (Must unwear item first and later sell it)
	{
		pResult.Result = 0xFF;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}*/

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Pos].m_Level == 1)
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Pos].m_Level == 2)
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(12,26) &&
		(lpObj->pInventory[lpMsg->Pos].m_Level == 1 ||
		 lpObj->pInventory[lpMsg->Pos].m_Level == 2 || 
		 lpObj->pInventory[lpMsg->Pos].m_Level == 3 || 
		 lpObj->pInventory[lpMsg->Pos].m_Level == 4 || 
		 lpObj->pInventory[lpMsg->Pos].m_Level == 5 ))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if( g_LuckyItemManager.IsLuckyItemTicket(lpObj->pInventory[lpMsg->Pos].m_Type) 
		|| g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[lpMsg->Pos].m_Type)
		|| lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14, 160) || lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14, 161) )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

#ifdef PERIOD
	if( lpObj->pInventory[lpMsg->Pos].IsPeriodItem() && !lpObj->pInventory[lpMsg->Pos].IsPeriodItemExpire() )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
#endif
	if(gSellHarmonyItemShop == false)
	{
		if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[lpMsg->Pos]) == TRUE )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(13, 37)), lpObj->m_Index, 1);
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
#pragma message("##### CHANGE ME ######")
	//if ( IsCashItem(lpObj->pInventory[lpMsg->Pos].m_Type) == TRUE )
	//{
	//	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	//	return;
	//}

	//if ( IsPremiumItem(lpObj->pInventory[lpMsg->Pos].m_Type) == TRUE ) //season4 add-on
	//{
	//	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	//	return;
	//}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14,12) && lpObj->pInventory[lpMsg->Pos].m_Level == 1) // Heart+1
	{
		pResult.Result = true;
		pResult.Money = lpObj->Money;
		gObjInventoryItemSet(aIndex, lpMsg->Pos, -1);
		lpObj->pInventory[lpMsg->Pos].Clear();
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].IsItem() == FALSE )
	{
		pResult.Result = false;
		pResult.Money = lpObj->Money;
		LogAdd(lMsg.Get(MSGGET(1, 228)), __FILE__, __LINE__, lpObj->Name, lpMsg->Pos);
		LogAdd("ID:%s CharID:%s ", lpObj->AccountID, lpObj->Name);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	}
	else
	{
		BYTE NewOption[MAX_EXOPTION_SIZE];
		int iAddZen = 0;
		int type = lpObj->pInventory[lpMsg->Pos].m_Type;

		if ( type == ITEMGET(13,4) || type == ITEMGET(13,5))
		{
			lpObj->pInventory[lpMsg->Pos].PetValue();
		}
		else
		{
			lpObj->pInventory[lpMsg->Pos].Value();
		}

		iAddZen = lpObj->pInventory[lpMsg->Pos].m_SellMoney;

		if ( gObjCheckMaxZen(aIndex, iAddZen) == FALSE )
		{
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		lpObj->Money += iAddZen;

		pResult.Result =true;
		pResult.Money = lpObj->Money;

		ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Pos]);

		LogAddTD(lMsg.Get(MSGGET(1, 229)), lpObj->AccountID, lpObj->Name, tShop,
			iAddZen, lpObj->pInventory[lpMsg->Pos].GetName(), 
			lpObj->pInventory[lpMsg->Pos].m_Level, lpObj->pInventory[lpMsg->Pos].m_Option1,
			lpObj->pInventory[lpMsg->Pos].m_Option2, lpObj->pInventory[lpMsg->Pos].m_Option3,
			lpObj->pInventory[lpMsg->Pos].m_Number, (int)lpObj->pInventory[lpMsg->Pos].m_Durability,
			NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], 
			lpObj->pInventory[lpMsg->Pos].m_SetOption, lpObj->pInventory[lpMsg->Pos].m_ItemOptionEx >> 7, //season 2.5 add-on
			g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[lpMsg->Pos]),
			g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pInventory[lpMsg->Pos]));

		::gObjInventoryDeleteItem(aIndex, lpMsg->Pos);

		if ( lpMsg->Pos == 10 || lpMsg->Pos == 11 )
		{
			::gObjUseSkill.SkillChangeUse(aIndex);
		}

		::gObjMakePreviewCharSet(aIndex);
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

int GetNeedMoneyItemDurRepaire(CItem *  DurItem, BOOL RequestPos) 
{
	int iMoney = 0;
	int type = DurItem->m_Type;
	float ItemDurability = DurItem->m_Durability;

	if ( type == ITEMGET(13,10) )
	{
		return 0;
	}

	if ( ::HasItemDurability(type ) != FALSE )
	{
		float lc4;
		float lc5 = 0;
		lc4 = DurItem->m_BaseDurability;

		if ( ItemDurability == lc4 )
		{
			return 0;
		}

		float lc6 = 1.0f - ItemDurability / lc4;
		int lc7;

		if ( type == ITEMGET(13,4)|| type ==ITEMGET(13,5) )
		{
			lc7 = DurItem->m_BuyMoney;
		}
		else
		{
			lc7 = DurItem->m_BuyMoney/3;
		}

		if ( lc7 > 400000000 ) lc7 = 400000000;

		if ( lc7 >= 1000 )
		{
			lc7 = lc7/100*100;
		}
		else if ( lc7 >= 100 )
		{
			lc7 = lc7/10*10;
		}

		float lc8 = sqrtf(lc7);
		float lc9 = sqrtf(sqrtf(lc7)); 
		lc5 = 3.0f * lc8 * lc9;
		lc5 *= lc6;
		lc5 += 1.0f;

		if ( ItemDurability <= 0.0f )
		{
			if ( type == ITEMGET(13,4)|| type ==ITEMGET(13,5) )
			{
				lc5 *= 2;
			}
			else
			{
				lc5 *= ::GetAllRepairItemRate(type);
			}

		}

		if ( RequestPos == TRUE )
		{
			lc5 += lc5 * 2; //season4 changed
		}

		iMoney = lc5;

		if ( iMoney >= 1000 )
		{
			iMoney = iMoney/100*100;
		}
		else if ( iMoney > 10 )
		{
			iMoney = iMoney/10*10;
		}		
	}
	else
	{
		iMoney = -1;
	}

	return iMoney;
}

struct PMSG_ITEMDURREPAIR_RESULT
{
	PBMSG_HEAD h;
	int Money;
};

void ItemDurRepaire(LPOBJ lpObj, CItem * DurItem, int pos, int RequestPos) 
{
	PMSG_ITEMDURREPAIR_RESULT pResult;
	int result = TRUE;

	PHeadSetB((LPBYTE)&pResult, 0x34, sizeof(pResult));
	int itemtype = DurItem->m_Type;

	if( itemtype >= ITEMGET(14,0) 
		|| (itemtype >= ITEMGET(13,0) && itemtype  < ITEMGET(13,4)) 
		|| itemtype == ITEMGET(13,10) 
		|| (itemtype >= ITEMGET(12, 7) && itemtype < ITEMGET(12,36)) 
		|| ( itemtype > ITEMGET(12,43) && itemtype < ITEMGET(12, 49) )
		|| ( itemtype > ITEMGET(12, 50) && itemtype < ITEMGET(13,0) )
		|| itemtype == ITEMGET(4,7) || itemtype == ITEMGET(4,15) 
#ifdef NEWWINGS
		&& !IS_NEWWINGS(itemtype)
#endif
		)
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( itemtype == ITEMGET(13,38) )
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( itemtype == ITEMGET(13,39) )
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( itemtype == ITEMGET(13,40) ) //Second Edition
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( itemtype == ITEMGET(13,41) ) //season 2.5 add-on
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		//return;
	}

	if ( itemtype == ITEMGET(13,42) ) //season 2.5 add-on
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		//return;
	}

#ifdef PERIOD
	if( DurItem->IsPeriodItem() )
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}
#endif

	pResult.Money = GetNeedMoneyItemDurRepaire(DurItem, RequestPos);

	if ( pResult.Money <= 0 )
	{
		pResult.Money = 0;
		DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( (lpObj->Money - pResult.Money) < 1 )
	{
		pResult.Money = 0;
	}
	else
	{
		lpObj->Money -= pResult.Money;
		pResult.Money = lpObj->Money;
		DurItem->m_Durability = (float)((int)DurItem->m_BaseDurability);
		DurItem->Convert(DurItem->m_Type, DurItem->m_Option1, DurItem->m_Option2, DurItem->m_Option3, DurItem->m_NewOption, DurItem->m_SetOption,DurItem->m_ItemOptionEx, NULL, 0xFF, 0, CURRENT_DB_VERSION);
		
		if ( DurItem->m_Type == ITEMGET(13,4)  || DurItem->m_Type == ITEMGET(13,5)) //season4.5 add-on
		{
			DurItem->PetValue();
		}

		GCItemDurSend(lpObj->m_Index, pos, DurItem->m_Durability, FALSE);
	}

	DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
}

void CGModifyRequestItem(PMSG_ITEMDURREPAIR * lpMsg, int aIndex) 
{
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_ITEMDURREPAIR_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x34, sizeof(pResult));
	pResult.Money = 0;

	if ( gObj[aIndex].CloseType != -1 )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !PacketCheckTime(lpObj))
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (lpMsg->Requestpos == 1 && gObj[aIndex].Level < 50 )
	{
		pResult.Money = 0;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Position == 0xFF )
	{
		for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
		{
			if ( lpObj->pInventory[n].IsItem())
			{
				if ( IsCashItem(lpObj->pInventory[n].m_Type ) == TRUE )
					continue;

				if ( IsPremiumItem(lpObj->pInventory[n].m_Type ) == TRUE )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,20) && (lpObj->pInventory[n].m_Level == 0 ||lpObj->pInventory[n].m_Level == 1 ))
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,20) && lpObj->pInventory[n].m_Level == 2 )
					continue;

				//WZ remove it after S4
				if( lpObj->pInventory[n].m_Type == ITEMGET(13,4) 
				|| lpObj->pInventory[n].m_Type == ITEMGET(13,5) )
				{
					continue;
				}

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,37) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,38) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,39) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(12,26) &&
					(lpObj->pInventory[n].m_Level == 1 ||
					 lpObj->pInventory[n].m_Level == 2 || 
					 lpObj->pInventory[n].m_Level == 3 || 
					 lpObj->pInventory[n].m_Level == 4 || 
					 lpObj->pInventory[n].m_Level == 5 ))
					 continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,40) ) //Second Edition
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,41) ) //season 2.5 add-on
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,42) ) //season 2.5 add-on
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,51) ) //season 2.5 add-on
					continue;

				if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,70) ) //season 4.0 add-on
					return;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,66) ) //season 4.0 add-on
					return;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13, 67) || 
					lpObj->pInventory[n].m_Type == ITEMGET(13, 68) || //season 4.0 add-on
					lpObj->pInventory[n].m_Type == ITEMGET(13, 76) || //season 4.6 add-on
					lpObj->pInventory[n].m_Type == ITEMGET(13, 106) 
					|| lpObj->pInventory[n].m_Type == ITEMGET(13, 122) // Season 5 Episode 2 JPN
					|| lpObj->pInventory[n].m_Type == ITEMGET(13, 123) ) 
					return;
				
#ifdef PERIOD
				if( lpObj->pInventory[n].IsPeriodItem() )	//season 5.4 add-on
				{
					return;
				}
#endif

				if( g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[n].m_Type) )	//season 6.1 add-on
				{
					return;
				}

				ItemDurRepaire(lpObj,&lpObj->pInventory[n] , n, lpMsg->Requestpos);
			}
		}

		gObjCalCharacter(lpObj->m_Index);
		return;
	}

	if ( lpMsg->Position > MAIN_INVENTORY_SIZE-1)
	{
		pResult.Money = 0;
		return;
	}

	if ( IsCashItem(lpObj->pInventory[lpMsg->Position].m_Type ) == TRUE )
		return;

	if ( IsPremiumItem(lpObj->pInventory[lpMsg->Position].m_Type ) == TRUE ) //season4 add-on
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,20) && (lpObj->pInventory[lpMsg->Position].m_Level == 0 ||lpObj->pInventory[lpMsg->Position].m_Level == 1 ))
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Position].m_Level == 2 )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,37) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,38) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,40) ) //Second Edition
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,41) ) //season 2.5 add-on
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,42) ) //season 2.5 add-on
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,51) ) //season 2.5 add-on
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,70) ) //season 4.0 add-on
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,66)) //season 4.0 add-on
		return;

#ifdef PERIOD
	if( lpObj->pInventory[lpMsg->Position].IsPeriodItem() )	//season 5.4 add-on
	{
		return;
	}
#endif

	if( g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[lpMsg->Position].m_Type) )	//season 6.1 add-on
	{
		return;
	}

	ItemDurRepaire(lpObj, &lpObj->pInventory[lpMsg->Position], lpMsg->Position, lpMsg->Requestpos);
	gObjCalCharacter(lpObj->m_Index);
}

struct PMSG_TRADE_REQUESTSEND
{
	PBMSG_HEAD h;
	char szId[10];
};

void CGTradeRequestSend(PMSG_TRADE_REQUEST * lpMsg, int aIndex) 
{
	int number;

	if (bCanTrade == FALSE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 125)), aIndex, 1);
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 126)), aIndex, 1);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( !PacketCheckTime(lpObj))
	{
		return;
	}

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( OBJMAX_RANGE(number) == FALSE )
	{
		LogAdd("error : %s %d (%d)", 
			__FILE__, 
			__LINE__, 
			number);
		return;
	}


	if ( number == aIndex )
	{
		return;
	}


	if ( !gObjIsConnected(number))
	{
		return;
	}


	if ( gObj[number].Type == OBJ_MONSTER )
	{
		return;
	}


	if ( gObj[number].CloseCount >= 0 )
	{
		return;
	}

#ifdef NPVP
	if( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[number]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3422), aIndex, 1);
		 return;
	}
	// ----
	if( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3423), aIndex, 1);
		 return;
	}
#endif

	if( gObj[number].Type != OBJ_USER ) //season4 add-on
	{
		LogAddC(2,"[HACKTOOL] : NPC-TradeRequest npc:%d ip:%s account:%s name:%s State:%d",
			gObj[number].Class,gObj[aIndex].Ip_addr,gObj[aIndex].AccountID,
			gObj[aIndex].Name,gObj[aIndex].Connected);
		CloseClient(aIndex);
		return;
	}


	if ( DS_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(2, 199)), aIndex, 1);
		return;
	}

	if ( gObj[aIndex].m_bPShopOpen == true )
	{
		return;
	}

	if ( gObj[number].m_bPShopOpen == true )
	{
		return;
	}

	if ( CC_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 196)), aIndex, 1);
		return;
	}

	if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
	{
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(gObj[aIndex].MapNumber); //season 3.0 add-on

		if( g_BloodCastle.GetCurrentState(iBridgeIndex) != 1 ||
			g_BloodCastle.CheckCanEnter(iBridgeIndex) == false )
		{
			::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 188)), aIndex, 1);
			return;
		}	
	}

	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
	{

		::GCServerMsgStringSend(lMsg.Get(3396), aIndex, 1);
		return;
	}

	if ( (gObj[number].m_Option &1) != 1 )
	{

		::GCTradeResponseSend(0, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{

		::GCTradeResponseSend(3, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if (gObj[number].m_IfState.use > 0 )
	{

		::GCTradeResponseSend(2, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if ( ::gObjFixInventoryPointer(aIndex) == false )
	{

		LogAdd("[Fix Inv.Ptr] False Location - %s, %d",
			__FILE__,
			__LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{

		LogAddTD("[%s][%s] CGTradeRequestSend() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 1;
	lpObj->TargetNumber = number;
	gObj[number].m_IfState.use = 1;
	gObj[number].m_IfState.state = 0;
	gObj[number].m_IfState.type = 1;
	gObj[number].TargetNumber = aIndex;
	lpObj->m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();

	PMSG_TRADE_REQUESTSEND pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x36;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.szId, gObj[aIndex].Name, sizeof(pMsg.szId));

	DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);

	LogAddTD(lMsg.Get(MSGGET(1, 230)), gObj[aIndex].AccountID, 
		gObj[aIndex].Name, gObj[aIndex].Ip_addr, 
		gObj[number].AccountID, gObj[number].Name, 
		gObj[number].Ip_addr);
}




void CGTradeResponseRecv(PMSG_TRADE_RESPONSE * lpMsg, int aIndex) 
{
	BOOL Result = TRUE;
	int number = gObj[aIndex].TargetNumber;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		LogAdd("error-L1 : %d (A_ID:%s) %s %d", number, gObj[aIndex].AccountID, __FILE__, __LINE__);
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( OBJMAX_RANGE(number) == FALSE )
	{
		if ( gObj[aIndex].m_IfState.use != FALSE )
		{
			if ( gObj[aIndex].m_IfState.type == 1 )
			{
				gObj[aIndex].m_IfState.use = FALSE;
				gObj[aIndex].TargetNumber = -1;
			}
		}

		LogAdd("error-L1 : target:%d (A_ID:%s) %s %d", number, gObj[aIndex].AccountID, __FILE__, __LINE__);
		Result = FALSE;
		lpMsg->Response = false;
		return; //season4 add-on (fix)
	}

	if ( gObjIsConnected(number) == FALSE )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}
	
	if ( gObj[aIndex].CloseType != -1 )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].m_bPShopOpen == true )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[number].m_bPShopOpen == true )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].X < (gObj[number].X -2 ) || gObj[aIndex].X > (gObj[number].X +2 ) || gObj[aIndex].Y < (gObj[number].Y -2 ) || gObj[aIndex].Y > (gObj[number].Y +2 ) )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].m_IfState.use == FALSE || gObj[aIndex].m_IfState.type != 1 || gObj[aIndex].m_IfState.state != 0 )
	{
		LogAddTD("(%s)(%s) Trade Interface State Error : use:%d type:%d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.use, gObj[aIndex].m_IfState.type);
		return;
	}

	if ( gObj[number].m_IfState.use == FALSE || gObj[number].m_IfState.type != 1 || gObj[number].m_IfState.state != 0 )
	{
		LogAddTD("(%s)(%s) Trade Interface State Error : use:%d type:%d", gObj[number].AccountID, gObj[number].Name, gObj[number].m_IfState.use, gObj[number].m_IfState.type);
		return;
	}

	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[%s][%s] CGTradeResponseRecv() Failed : Transaction == 1, IF_TYPE : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;
	}

	if ( lpMsg->Response == false )
	{
		GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, 0, 0);
		Result = FALSE;
	}
	else
	{
		Result = TRUE;

		for ( int i=0;i<TRADE_BOX_SIZE;i++)
		{
			gObj[aIndex].Trade[i].Clear();
			gObj[number].Trade[i].Clear();
		}

		memset(gObj[aIndex].TradeMap, (BYTE)-1, TRADE_BOX_MAP_SIZE );
		memset(gObj[number].TradeMap, (BYTE)-1, TRADE_BOX_MAP_SIZE );
		gObj[aIndex].m_IfState.state = 1;
		gObj[number].m_IfState.state = 1;
		gObj[aIndex].TradeMoney = 0;
		gObj[number].TradeMoney = 0;

		if (::gObjInventoryTrans(aIndex) == FALSE )
			Result = 2;

		if (::gObjInventoryTrans(number) == FALSE )
			Result = 3;

		if ( Result != TRUE )
		{
			lpMsg->Response = false;
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, 0, 0);
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, 0, 0);

			if ( Result == 2 )
			{
				LogAddTD("[%s][%s] Made Trade Error (in Transaction) [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].AccountID, gObj[number].Name);
			}
			else if ( Result == 3 )
			{
				LogAddTD("[%s][%s] Made Trade Error (in Transaction) [%s][%s]",	gObj[number].AccountID, gObj[number].Name, gObj[aIndex].AccountID, gObj[aIndex].Name);
			}

			Result = FALSE;
		}
		else
		{
			lpMsg->Response = true;
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, gObj[aIndex].Level, gObj[aIndex].GuildNumber);
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, gObj[number].Level, gObj[number].GuildNumber);

			LogAddTD("[%s][%s] Trade Ready [%s][%s]", gObj[number].AccountID, gObj[number].Name, gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}

	if ( Result == FALSE )
	{
		if ( gObj[aIndex].m_IfState.use != FALSE && gObj[aIndex].m_IfState.type == 1 )
		{
			gObj[aIndex].m_IfState.use = FALSE;
			gObj[aIndex].TargetNumber = -1;
		}

		if ( gObj[number].TargetNumber == aIndex )
		{
			if ( gObj[number].m_IfState.use != FALSE && gObj[number].m_IfState.type == 1 )
			{
				gObj[number].m_IfState.use = FALSE;
				gObj[number].TargetNumber = -1;
			}
		}
	}
}

BOOL GCTradeResponseSend(BYTE response, int aIndex, LPSTR id, WORD level, int GuildNumber) 
{
	PMSG_TRADE_RESPONSE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x37, sizeof(pMsg));
	pMsg.Response = response;
	pMsg.Level = level;
	pMsg.GuildNumber = GuildNumber;
	memcpy(pMsg.Id, id, sizeof(pMsg.Id));

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_TRADE_OTHER_DEL
{
	PBMSG_HEAD h;
	BYTE TradeItemIndex;
};

int GCTradeOtherDel(int aIndex, BYTE tradeindex) 
{
	PMSG_TRADE_OTHER_DEL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x38, sizeof(pMsg ));
	pMsg.TradeItemIndex = tradeindex;

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_TRADE_OTHER_MOVE
{
	PBMSG_HEAD h;
	BYTE TradeItemIndex;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

int GCTradeOtherAdd(int aIndex, BYTE tradeindex, LPBYTE iteminfo) 
{
	PMSG_TRADE_OTHER_MOVE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x39, sizeof(pMsg));
	pMsg.TradeItemIndex = tradeindex;
	pMsg.ItemInfo[0] = iteminfo[0];
	pMsg.ItemInfo[1] = iteminfo[1];
	pMsg.ItemInfo[2] = iteminfo[2];
	pMsg.ItemInfo[3] = iteminfo[3];
	pMsg.ItemInfo[4] = iteminfo[4];
	pMsg.ItemInfo[5] = iteminfo[5];
	pMsg.ItemInfo[6] = iteminfo[6];

	//season4 add-on
	pMsg.ItemInfo[7] = iteminfo[7];
	pMsg.ItemInfo[8] = iteminfo[8];
	pMsg.ItemInfo[9] = iteminfo[9];
	pMsg.ItemInfo[10] = iteminfo[10];
	pMsg.ItemInfo[11] = iteminfo[11];
	
	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGTradeMoneyRecv(PMSG_TRADE_GOLD * lpMsg, int aIndex) 
{
	PMSG_DEFRESULT pMsg;
	int money = 0;
	int number;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	if ( !OBJMAX_RANGE(aIndex) )
	{
		LogAdd("error : %s %d (%d)", __FILE__, __LINE__, aIndex);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 231)), gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	number = gObj[aIndex].TargetNumber;

	if ( !OBJMAX_RANGE(number) )
	{
		LogAdd(lMsg.Get(MSGGET(1, 232)), gObj[aIndex].AccountID, gObj[aIndex].Name, number);
		return;
	}

	if ( gObj[number].CloseType != -1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 233)), gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].AccountID, gObj[number].Name);
		return;
	}

	if ( gObj[aIndex].m_IfState.use == FALSE && gObj[aIndex].m_IfState.type != 1 )
	{
		return;
	}

	if ( ::gObjCanItemTouch(&gObj[aIndex], 1) == FALSE )
	{
		return;
	}

	if ( lpMsg->Money > MAX_TRADE_ZEN )
	{
		return;
	}

	money = lpMsg->Money;

	if ( (gObj[aIndex].Money - money) < 0 )
	{
		return;
	}

	GCMoneySend(aIndex, gObj[aIndex].Money - money);

	PHeadSetB((LPBYTE)&pMsg, 0x3A, sizeof(pMsg));
	pMsg.result = 1;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( number >= 0)
	{
		gObj[aIndex].TradeMoney = money;
		gObj[aIndex].TradeOk  = false;
		gObj[number].TradeOk  = false;

		GCTradeOkButtonSend(number, 2);
		GCTradeOkButtonSend(aIndex, 0);
		GCTradeMoneyOther(number, gObj[aIndex].TradeMoney);
	}
}

struct PMSG_TRADE_OTHER_GOLD
{
	PBMSG_HEAD h;
	DWORD Money;
};

BOOL GCTradeMoneyOther(int aIndex, DWORD money) 
{
	PMSG_TRADE_OTHER_GOLD pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3B, sizeof(pMsg));
	pMsg.Money = money;

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);	
}

BOOL GCTradeOkButtonSend(int aIndex, BYTE flag) 
{
	PMSG_TRADE_OKBUTTON pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3C, sizeof(pMsg));
	pMsg.Flag = flag;

	return DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGTradeOkButtonRecv(PMSG_TRADE_OKBUTTON * lpMsg, int aIndex) 
{
	int number = gObj[aIndex].TargetNumber;

	if ( number < 0 || number > OBJMAX-1 )
	{
		return;
	}

	if ( !gObjIsConnected(&gObj[number]))
	{
		return;
	}

	if ( gObj[aIndex].m_IfState.use < 1 || gObj[aIndex].m_IfState.state != 1 )
	{
		return;
	}

	if ( gObj[number].m_IfState.use < 1 || gObj[number].m_IfState.state != 1 )
	{
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	if ( gObj[number].CloseType != -1 )
	{
		return;
	}
	
	if ( lpMsg->Flag == 1 )
	{
		if ( gObj[aIndex].TradeOk == false )
		{
			gObj[aIndex].TradeOk = true;
			LogAddTD("[%s][%s] Trade Accept", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCTradeOkButtonSend(number, 1);
		}
	}
	else if ( lpMsg->Flag == 0 )
	{
		if ( gObj[aIndex].TradeOk  == true )
		{
			GCTradeOkButtonSend(number, 0);
			gObj[aIndex].TradeOk = false;
		}
	}

	gObjTradeOkButton(aIndex);
}

void CGTradeCancelButtonRecv(int aIndex) 
{
	int number = gObj[aIndex].TargetNumber;
	gObjTradeCancel(aIndex);
	CGTradeResult(aIndex, 0);

	if ( number >= 0 )
	{
		gObjTradeCancel(number);
		CGTradeResult(number, 0);
	}
}

struct PMSG_TRADE_RESULT
{
	PBMSG_HEAD h;
	BYTE Result;
};

void CGTradeResult(int aIndex , BYTE result ) 
{
	PMSG_TRADE_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3D, sizeof(pMsg));
	pMsg.Result = result;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct CG_REQ_PSHOP_SET_ITEM_PRICE
{
	PHEADSUBB PHeader;
	BYTE btSourcePos;
	BYTE dwItemPrice[4];
};

void CGPShopReqSetItemPrice(PMSG_REQ_PSHOP_SETITEMPRICE * lpMsg, int aIndex) 
{
	if ( gDoPShopOpen  == FALSE )
	{
		CGPShopAnsSetItemPrice(aIndex, 0, lpMsg->btItemPos);
		return;
	}

	if ( !gObjIsConnected(aIndex))
	{
		LogAddTD("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		LogAddTD("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

	if ( gObj[aIndex].Level <= 5 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Level is Under 6 : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Level);
		CGPShopAnsSetItemPrice(aIndex, 5, lpMsg->btItemPos);
		return;
	}

	if ( INVENTORY_RANGE(lpMsg->btItemPos) == FALSE )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Item Position Out of Bound : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->btItemPos);
		CGPShopAnsSetItemPrice(aIndex, 2, lpMsg->btItemPos);
		return;
	}

	if ( gObj[aIndex].Inventory1[lpMsg->btItemPos].IsItem() == FALSE )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Item Does Not Exist : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->btItemPos);
		CGPShopAnsSetItemPrice(aIndex, 3, lpMsg->btItemPos);
		return;
	}

	CItem * sitem = &gObj[aIndex].Inventory1[lpMsg->btItemPos];
	int iItemPrice = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->sItemPrice4, lpMsg->sItemPrice3), MAKE_NUMBERW(lpMsg->sItemPrice2, lpMsg->sItemPrice1));

	if ( gObjCheckSerial0ItemList(sitem) != FALSE )
	{
		MsgOutput(aIndex, lMsg.Get(MSGGET(13, 26)));
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		LogAddTD("[ANTI-HACK][Serial 0 Item] [PShop] (%s)(%s) Item(%s) Pos(%d)",
			gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->GetName(), lpMsg->btItemPos);
		return;
	}

	if ( gObjInventorySearchSerialNumber(&gObj[aIndex], sitem->GetNumber()) == FALSE )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);
		LogAdd("error-L2 : CopyItem [%s][%s] return %s %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

	if ( iItemPrice <= 0 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Personal Shop Item Price <= 0 : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->m_iPShopValue);
		CGPShopAnsSetItemPrice(aIndex, 4, lpMsg->btItemPos);
		return;
	}

	if ( (gObj[aIndex].Penalty&4) == 4 || (gObj[aIndex].Penalty&8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Item Price Change - Item Block",
			gObj[aIndex].AccountID, gObj[aIndex].Name,sitem->m_iPShopValue);
		CGPShopAnsSetItemPrice(aIndex, 6, lpMsg->btItemPos);
		return;
	}

	sitem->m_iPShopValue = iItemPrice;
	ItemAttribute[sitem->m_Type].Name[31] = 0;

	LogAddTD("[PShop] [%s][%s] Changing Item Price : %d (Type:%d (%s), Lev:%d, Serial:%u, Dur:%d",
		gObj[aIndex].AccountID, gObj[aIndex].Name, sitem->m_iPShopValue, sitem->m_Type, ItemAttribute[sitem->m_Type].Name, sitem->m_Level, sitem->m_Number, sitem->m_Durability);
	CGPShopAnsSetItemPrice(aIndex, 1, lpMsg->btItemPos);
}

struct PMSG_ANS_PSHOP_SETITEMPRICE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btItemPos;
};

void CGPShopAnsSetItemPrice(int aIndex, BYTE btResult, BYTE btItemPos) 
{
	PMSG_ANS_PSHOP_SETITEMPRICE pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x01, sizeof(pMsg));
	pMsg.btResult = btResult;
	pMsg.btItemPos = btItemPos;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_ANS_PSHOP_TEXT_CHANGED
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE btPShopText[36];
	BYTE btName[10];
};

void CGPShopReqOpen(PMSG_REQ_PSHOP_OPEN * lpMsg, int aIndex) 
{
	if (::gDoPShopOpen == FALSE )
	{
		return;
	}

	if ( !gObjIsConnected(aIndex))
	{
		LogAddTD("[PShop] ERROR : Index is not CONNECTED : %d",aIndex);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		LogAddTD("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

#ifdef NPVP
	if( g_NewPVP.IsDuel(gObj[aIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3429), aIndex, 1);
		 return;
	}
	// ----
	if( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3430), aIndex, 1);
		 return;
	}
#endif

	if ( gObj[aIndex].Level <= 5 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Level is Under 6 : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Level);
		::CGPShopAnsOpen( aIndex, 2);
		return;
	}

	if ( (gObj[aIndex].Penalty&4) == 4 || (gObj[aIndex].Penalty&8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Cant't Open Shop - Item Block",
			gObj[aIndex].AccountID, gObj[aIndex].Name);
		::CGPShopAnsOpen( aIndex, 3);
		return;
	}	

	if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 197)), aIndex, 1);
		::CGPShopAnsOpen( aIndex, 0);
		return;
	}	

	bool bEnablePShopOpen = false;

	if ( gObj[aIndex].m_IfState.use == FALSE )
	{
		bEnablePShopOpen  = true;
	}
	else
	{
		if ( gObj[aIndex].m_IfState.use == TRUE )
		{
			switch ( gObj[aIndex].m_IfState.type )
			{
				case 8:
					bEnablePShopOpen = true; break;
			}

			if ( !bEnablePShopOpen  )
			{
				LogAddTD("[PShop] [%s][%s] ERROR : m_IfState.type is Using : %d",
					gObj[aIndex].AccountID, gObj[aIndex].Name,
					gObj[aIndex].m_IfState.type);
			}
		}
		else
		{
			LogAddTD("[PShop] [%s][%s] ERROR : Unknown m_IfState.type : %d",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].m_IfState.type);
			return;
		}
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d",
			__FILE__ ,__LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[PShop] [%s][%s] ERROR : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		bEnablePShopOpen = false;
	}

	if ( bEnablePShopOpen )
	{
		if ( gObj[aIndex].m_bPShopOpen == false )
		{
			gObj[aIndex].m_bPShopOpen = true;
			memcpy(gObj[aIndex].m_szPShopText, lpMsg->szPShopText, sizeof(lpMsg->szPShopText));
			LogAddTD("[PShop] [%s][%s] Personal Shop Opened",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		else
		{
			memcpy(gObj[aIndex].m_szPShopText, lpMsg->szPShopText, sizeof(lpMsg->szPShopText));
			
			PMSG_ANS_PSHOP_TEXT_CHANGED pMsg;

			PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x10, sizeof(pMsg));
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);
			memcpy(pMsg.btPShopText, lpMsg->szPShopText, sizeof(pMsg.btPShopText));
			memcpy(pMsg.btName, gObj[aIndex].Name, sizeof(pMsg.btName));

			MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);

			LogAddTD("[PShop] [%s][%s] Personal Shop Already Opened - Changing PShop Name",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}
	else
	{
		CGPShopAnsOpen(aIndex, 0);
		return;
	}
	
	CGPShopAnsOpen(aIndex, 1);
}

struct PMSG_ANS_PSHOP_OPEN
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

void CGPShopAnsOpen(int aIndex, BYTE btResult) 
{
	PMSG_ANS_PSHOP_OPEN pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x02, sizeof(pMsg));
	pMsg.btResult = btResult;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CGPShopReqClose(int aIndex) 
{
	if ( !gObjIsConnected(aIndex))
	{
		LogAddTD("[PShop] ERROR : Index is not CONNECTED : %d", aIndex);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		LogAddTD("[PShop] ERROR : Index is not CHARACTER : %d", aIndex);
		return;
	}

#ifdef NPVP
	if( g_NewPVP.IsDuel(gObj[aIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3429), aIndex, 1);
		 return;
	}
	// ----
	if( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3430), aIndex, 1);
		 return;
	}
#endif

	LogAddTD("[PShop] [%s][%s] Requested to Close PShop",
		gObj[aIndex].AccountID, gObj[aIndex].Name);
	gObj[aIndex].m_bPShopOpen = false;
	memset(gObj[aIndex].m_szPShopText, 0, sizeof(gObj[aIndex].m_szPShopText));
	CGPShopAnsClose(aIndex, 1);
}

struct PMSG_ANS_PSHOP_CLOSE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE NumberH;
	BYTE NumberL;
};

void CGPShopAnsClose(int aIndex, BYTE btResult) 
{
	if( gObj[aIndex].IsOffTrade )
	{
		gObjDel(aIndex);
	}
	// ----
	gObj[aIndex].IsOffTrade = false;
	// ----
	LogAddTD("[PShop] [%s][%s] Close PShop",
		gObj[aIndex].AccountID, gObj[aIndex].Name);

	PMSG_ANS_PSHOP_CLOSE pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x03, sizeof(pMsg));
	pMsg.btResult = btResult;
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));

	if ( btResult == 1 )
	{
		MsgSendV2(&gObj[aIndex], (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void CGPShopReqBuyList(PMSG_REQ_BUYLIST_FROM_PSHOP * lpMsg, int aSourceIndex) 
{
	if ( ::gDoPShopOpen == FALSE )
	{
		return;
	}

	if ( gObjIsConnected(MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)) == FALSE)
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : Seller did Not CONNECTED (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	LPOBJ lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];

	if ( lpObj->Type != OBJ_USER )
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : Seller is Not CHARACTER (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	if ( lpObj->CloseCount >= 0 )	
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Closing (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL) );
		CGPShopAnsBuyItem(aSourceIndex, -1, 0,2);
		return;
	}

	if ( gObj[aSourceIndex].CloseCount >= 0 )	
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Buyer is Closing (%d)",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name,  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

#ifdef NPVP
	if( g_NewPVP.IsDuel(gObj[aSourceIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3429), aSourceIndex, 1);
		 return;
	}
	// ----
	if( g_NewPVP.IsObserver(gObj[aSourceIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3430), aSourceIndex, 1);
		 return;
	}
#endif

	if ( lpObj->m_Index == aSourceIndex )
	{
		LogAddTD("[PShop] [%s][%s] PShop List Requested to Him(/Her)Self",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name);
		::CGPShopAnsBuyList(aSourceIndex, -1, 2, 0);
		return;
	}

	if ( lpObj->m_bPShopOpen == false )
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : [%s][%s] Did not Open PShop",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
		::CGPShopAnsBuyList(aSourceIndex, -1, 3, 0);
		return;
	}

	if ( (lpObj->Penalty &4) == 4 || (lpObj->Penalty&8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);
		::CGPShopAnsBuyList(aSourceIndex, -1, 4, 0);
		return;
	}

	if ( !::gObjFixInventoryPointer(aSourceIndex))
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d",
			__FILE__, __LINE__);
	}

	if ( gObj[aSourceIndex].pTransaction == 1 )
	{
		LogAddTD("[PShop] [%s][%s] PShop List Request Failed : Requester Transaction == 1, IF_TYPE : %d",
			gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, gObj[aSourceIndex].m_IfState.type);
		return;
	}

	LogAddTD("[PShop] [%s][%s] is Receiving PShop List From [%s][%s]",
		gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, lpObj->AccountID, lpObj->Name);

	gObj[aSourceIndex].m_bPShopWantDeal = true;
	gObj[aSourceIndex].m_iPShopDealerIndex = lpObj->m_Index;
	memcpy(gObj[aSourceIndex].m_szPShopDealerName, lpObj->Name, MAX_ACCOUNT_LEN);
	::CGPShopAnsBuyList(aSourceIndex, lpObj->m_Index, 1, false);
}

struct PMSG_BUYLIST_FROM_PSHOP
{
	BYTE Pos;
	BYTE ItemInfo[MAX_ITEM_INFO];
	int PShopItemValue;
};

struct PMSG_ANS_BUYLIST_FROM_PSHOP
{
	PWMSG_HEAD2 h;
	BYTE Result;
	BYTE NumberH;
	BYTE NumberL;
	BYTE btName[10];
	char szPShopText[36];
	BYTE btCount;
};

void CGPShopAnsBuyList(int aSourceIndex, int aTargetIndex, BYTE btResult, bool bResend) 
{
	if ( gDoPShopOpen == FALSE )
	{
		return;
	}
	
	PMSG_ANS_BUYLIST_FROM_PSHOP pMsgILC;
	PMSG_BUYLIST_FROM_PSHOP pMsgIL;
	BYTE sendBuf[1024];
	int sOfs = sizeof(pMsgILC);
	int pMsgILSize = sizeof(pMsgIL);

	if ( btResult != 1 )
	{
		LPOBJ lpObj = &gObj[aSourceIndex];
		
		pMsgILC.h.c = 0xC2;
		pMsgILC.h.headcode = 0x3F;

		if ( bResend == false )
		{
			pMsgILC.h.subcode = 0x05;
		}
		else
		{
			pMsgILC.h.subcode = 0x13;
		}

		pMsgILC.h.sizeH = SET_NUMBERH(sOfs);
		pMsgILC.h.sizeL = SET_NUMBERL(sOfs);
		pMsgILC.btCount = 0;
		pMsgILC.Result = btResult;
		pMsgILC.NumberH = SET_NUMBERH(aTargetIndex);
		pMsgILC.NumberL = SET_NUMBERL(aTargetIndex);
		memset(pMsgILC.btName, 0, sizeof(pMsgILC.btName));
		memset(pMsgILC.szPShopText, 0, sizeof(pMsgILC.szPShopText));
		memcpy(sendBuf, &pMsgILC, sizeof(pMsgILC));

		DataSend(aSourceIndex, sendBuf, sOfs);
	}
	else
	{
		LPOBJ lpObjSource = &gObj[aSourceIndex];
		LPOBJ lpObjTarget = &gObj[aTargetIndex];
		int iCount = 0;

		for ( int n=MAIN_INVENTORY_SIZE;n<INVENTORY_SIZE;n++)
		{
			if ( lpObjTarget->Inventory1[n].IsItem() == TRUE )
			{
				pMsgIL.Pos = n;
				ItemByteConvert(pMsgIL.ItemInfo, lpObjTarget->Inventory1[n]);
				pMsgIL.PShopItemValue = lpObjTarget->Inventory1[n].m_iPShopValue;
				memcpy(&sendBuf[sOfs], &pMsgIL, pMsgILSize);
				iCount++;
				sOfs += pMsgILSize;
			}
		}

		pMsgILC.h.c = 0xC2;
		pMsgILC.h.headcode = 0x3F;

		if ( bResend == false )
		{
			pMsgILC.h.subcode = 0x05;
		}
		else
		{
			pMsgILC.h.subcode = 0x13;
		}

		pMsgILC.h.sizeH = SET_NUMBERH(sOfs);
		pMsgILC.h.sizeL = SET_NUMBERL(sOfs);
		pMsgILC.btCount = iCount;
		pMsgILC.Result = btResult;
		pMsgILC.NumberH = SET_NUMBERH(aTargetIndex);
		pMsgILC.NumberL = SET_NUMBERL(aTargetIndex);
		memcpy(pMsgILC.btName, lpObjTarget->Name, sizeof(pMsgILC.btName));
		memcpy(pMsgILC.szPShopText, lpObjTarget->m_szPShopText, sizeof(pMsgILC.szPShopText));
		memcpy(sendBuf, &pMsgILC, sizeof(pMsgILC));

		DataSend(aSourceIndex, sendBuf, sOfs);
	}
}

void CGPShopReqBuyItem(PMSG_REQ_BUYITEM_FROM_PSHOP * lpMsg, int aSourceIndex) 
{
	if ( gDoPShopOpen == FALSE )
	{
		return;
	}

	if ( gObjIsConnected( MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)) == FALSE )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller did Not CONNECTED (%d)",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name, 
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	LPOBJ lpObj = &gObj[MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)];

	if ( lpObj->Type != OBJ_USER )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Not CHARACTER (%d)",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name,  
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

	if ( lpObj->CloseCount >= 0 )	
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Seller is Closing (%d)",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name,  
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL)  );
		::CGPShopAnsBuyItem(aSourceIndex, -1, 0,2);
		return;
	}

	if ( gObj[aSourceIndex].CloseCount >= 0 )	
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Buyer is Closing (%d)",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name,  
			MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL));
		::CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
		return;
	}

#ifdef NPVP
	if( g_NewPVP.IsDuel(gObj[aSourceIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3429), aSourceIndex, 1);
		 return;
	}
	// ----
	if( g_NewPVP.IsObserver(gObj[aSourceIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3430), aSourceIndex, 1);
		 return;
	}
#endif

	if ( lpObj->m_bPShopOpen == false )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did not Open PShop",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name,	
			lpObj->AccountID, 
			lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 3);
		return;
	}

	char szName[MAX_ACCOUNT_LEN+1] = {0};
	memcpy(szName, lpMsg->btName, sizeof(lpMsg->btName));
	szName[MAX_ACCOUNT_LEN] = 0;
	int iITEM_LOG_TYPE;
	int iITEM_LOG_LEVEL;
	int iITEM_LOG_DUR;
	int iITEM_LOG_SERIAL;

	if ( strcmp(szName, lpObj->Name) )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Name Mismatch [%s] - [%s]",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name, 
			szName, 
			lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 6);
		return;
	}

	if ( lpObj->CloseType != -1 )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is Closing Connection",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name, 
			lpObj->AccountID, 
			lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0,2);
		return;
	}

	if ( (lpObj->Penalty &4) == 4 || (lpObj->Penalty &8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name, 
			lpObj->AccountID, 
			lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0,9);
		return;
	}

	if ( (gObj[aSourceIndex].Penalty &4) == 4 || (gObj[aSourceIndex].Penalty &8) == 8 )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] is in Item Block",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name, 
			lpObj->AccountID, 
			lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0,9);
		return;
	}

	EnterCriticalSection(&lpObj->m_critPShopTrade);

	if ( lpObj->m_bPShopTransaction == true )
	{
		LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Already Trade With Other",
			gObj[aSourceIndex].AccountID, 
			gObj[aSourceIndex].Name, 
			lpObj->AccountID, 
			lpObj->Name);
		::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0,4);
		LeaveCriticalSection(&lpObj->m_critPShopTrade);
	}
	else
	{
		lpObj->m_bPShopTransaction = true;
		LeaveCriticalSection(&lpObj->m_critPShopTrade);

		if ( gObjCheckSerial0ItemList(&lpObj->Inventory1[lpMsg->btItemPos]) != FALSE )
		{
			MsgOutput(lpObj->m_Index, lMsg.Get(MSGGET(13, 26)));
			GCReFillSend(aSourceIndex, gObj[aSourceIndex].Life, 0xFD, 1, gObj[aSourceIndex].iShield);

			LogAddTD("[ANTI-HACK][Serial 0 Item] [PShop Buy] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID, lpObj->Name, lpObj->Inventory1[lpMsg->btItemPos].GetName(),
				lpMsg->btItemPos);
			return;
		}

		if ( ::gObjInventorySearchSerialNumber(&gObj[aSourceIndex], lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE )
		{
			::GCReFillSend(aSourceIndex, gObj[aSourceIndex].Life, -3, 1, gObj[aSourceIndex].iShield);
			LogAdd("error-L2 : CopyItem [%s][%s] return %s %d",
				gObj[aSourceIndex].AccountID, 
				gObj[aSourceIndex].Name,
				__FILE__, 
				__LINE__);
			return;
		}

		if ( ::gObjInventorySearchSerialNumber(lpObj, lpObj->Inventory1[lpMsg->btItemPos].GetNumber()) == FALSE )
		{
			GCReFillSend(lpObj->m_Index, lpObj->Life, -3, 1, lpObj->iShield);
			LogAdd("error-L2 : CopyItem [%s][%s] return %s %d",
				lpObj->AccountID, 
				lpObj->Name,
				__FILE__, 
				__LINE__);
			return;
		}

		if (gObjFixInventoryPointer(aSourceIndex) == false )
		{
			LogAdd( "[Fix Inv.Ptr] False Location - %s, %d",
				__FILE__, 
				__LINE__);
		}

		if ( gObj[aSourceIndex].pTransaction == 1 )
		{
			LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : Requester Transaction == 1, IF_TYPE : %d",
				gObj[aSourceIndex].AccountID, 
				gObj[aSourceIndex].Name, 
				gObj[aSourceIndex].m_IfState.type);
			return;
		}

		iITEM_LOG_TYPE=0;
		iITEM_LOG_LEVEL=0;
		iITEM_LOG_DUR=0;
		iITEM_LOG_SERIAL = 0;

		__try
		{
			if ( PSHOP_RANGE(lpMsg->btItemPos) == FALSE )
			{
				::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 5);
				return;
			}

			if ( lpObj->m_bMapSvrMoveQuit == true )
			{
				CGPShopAnsBuyItem(aSourceIndex, -1, 0, 2);
				return;
			}

			if ( lpObj->Inventory1[lpMsg->btItemPos].IsItem() == TRUE )
			{
				if ( lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue <= 0 )
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Did Not Choose Item Price",
						gObj[aSourceIndex].AccountID, 
						gObj[aSourceIndex].Name, 
						lpObj->AccountID, 
						lpObj->Name);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 6);
					return;
				}
				DWORD dwCost = lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue;
#ifdef __ALIEN__
				if(!g_OfflineTrade.CheckExchange(lpObj,&gObj[aSourceIndex],lpMsg->btItemPos)) return;
#else
				if ( gObj[aSourceIndex].Money < lpObj->Inventory1[lpMsg->btItemPos].m_iPShopValue )
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Lack of Zen",
						gObj[aSourceIndex].AccountID, 
						gObj[aSourceIndex].Name, 
						lpObj->AccountID, 
						lpObj->Name);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 7);
					return;
				}

				if ( gObjCheckMaxZen(lpObj->m_Index, dwCost) == FALSE )
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] Exceeding Zen of the Host",
						gObj[aSourceIndex].AccountID, 
						gObj[aSourceIndex].Name, 
						lpObj->AccountID, 
						lpObj->Name);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);
					return;
				}
#endif
				BYTE btNewItemPos = 0;
				btNewItemPos = ::gObjOnlyInventoryInsertItem(aSourceIndex, lpObj->Inventory1[lpMsg->btItemPos]);

				if ( btNewItemPos == 0xFF )
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Buy Request Failed : [%s][%s] No Room to Buy Item",
						gObj[aSourceIndex].AccountID, 
						gObj[aSourceIndex].Name, 
						lpObj->AccountID, 
						lpObj->Name);
					::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, 0, 8);
					return;
				}

				
				iITEM_LOG_TYPE = lpObj->Inventory1[lpMsg->btItemPos].m_Type;
				iITEM_LOG_LEVEL = lpObj->Inventory1[lpMsg->btItemPos].m_Level;
				iITEM_LOG_DUR = lpObj->Inventory1[lpMsg->btItemPos].m_Durability;
				iITEM_LOG_SERIAL = lpObj->Inventory1[lpMsg->btItemPos].m_Number;

				::gObjInventoryItemSet_PShop(lpObj->m_Index, lpMsg->btItemPos, -1);
				lpObj->Inventory1[lpMsg->btItemPos].Clear();
				::GCInventoryItemDeleteSend(lpObj->m_Index, lpMsg->btItemPos, TRUE);
#ifdef __ALIEN__
				g_OfflineTrade.UpdateExchange(lpObj,&gObj[aSourceIndex],dwCost);
#else
				gObj[aSourceIndex].Money -= dwCost;
				lpObj->Money += dwCost;
				::GCMoneySend(aSourceIndex, gObj[aSourceIndex].Money);
				::GCMoneySend(lpObj->m_Index, lpObj->Money);
#endif
				::CGPShopAnsBuyItem(aSourceIndex, lpObj->m_Index, btNewItemPos, 1);
				::CGPShopAnsSoldItem(lpObj->m_Index, aSourceIndex, lpMsg->btItemPos);

				LogAddTD("[PShop] [%s][%s][%s] PShop Item Buy Request Succeed : [%s][%s][%s] (Price=%d, ItemType:%d (%s), ItemLevel:%d, ItemDur:%d, Serial:%u", 
					gObj[aSourceIndex].AccountID, gObj[aSourceIndex].Name, gObj[aSourceIndex].Ip_addr, lpObj->AccountID, lpObj->Name, lpObj->Ip_addr, dwCost, 
					iITEM_LOG_TYPE, ItemAttribute[iITEM_LOG_TYPE].Name, iITEM_LOG_LEVEL, iITEM_LOG_DUR, iITEM_LOG_SERIAL);

				if (PShop_CheckInventoryEmpty(lpObj->m_Index) == true )
				{
					LogAddTD("[PShop] [%s][%s] Sold All Items - Auto Closing PShop",
						lpObj->AccountID, lpObj->Name);
					::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 195)), lpObj->m_Index, 1);
					lpObj->m_bPShopOpen = false;
					memset(lpObj->m_szPShopText, 0, sizeof(lpObj->m_szPShopText));
					::CGPShopAnsClose(lpObj->m_Index, 1);
				}
				else
				{
					lpObj->m_bPShopItemChange = true;
				}

			}
		}
		__finally
		{
			lpObj->m_bPShopTransaction = false;
		}
	}
}

struct PMSG_ANS_BUYITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;
	BYTE Result;
	BYTE NumberH;
	BYTE NumberL;
	BYTE cItemInfo[MAX_ITEM_INFO];
	BYTE btItemPos;
};

void CGPShopAnsBuyItem(int aSourceIndex, int aTargetIndex, int iItemPos, BYTE btResult) 
{
	PMSG_ANS_BUYITEM_FROM_PSHOP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x06, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aTargetIndex);
	pMsg.NumberL = SET_NUMBERL(aTargetIndex);
	pMsg.btItemPos = iItemPos;
	pMsg.Result = btResult;
	ItemByteConvert(pMsg.cItemInfo, gObj[aSourceIndex].Inventory1[iItemPos]);

	DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_ANS_SOLDITEM_FROM_PSHOP
{
	PBMSG_HEAD2 h;
	BYTE btItemPos;
	BYTE btName[10];
};

void CGPShopReqCloseDeal(PMSG_REQ_PSHOPDEAL_CLOSE * lpMsg, int aIndex) 
{
	if ( !gObjIsConnected(aIndex))
	{
		return;
	}

	gObj[aIndex].m_bPShopWantDeal = false;
	gObj[aIndex].m_iPShopDealerIndex =-1;
	memset(gObj[aIndex].m_szPShopDealerName , 0, MAX_ACCOUNT_LEN);
}

void CGPShopAnsSoldItem(int aSourceIndex, int aTargetIndex, int iItemPos) 
{
	PMSG_ANS_SOLDITEM_FROM_PSHOP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x08, sizeof(pMsg));
	pMsg.btItemPos = iItemPos;
	memcpy(pMsg.btName, gObj[aTargetIndex].Name, MAX_ACCOUNT_LEN);

	DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_REQ_DEALER_CLOSED_SHOP
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
};

void CGPShopAnsDealerClosedShop(int aSourceIndex, int aTargetIndex) 
{
	PMSG_REQ_DEALER_CLOSED_SHOP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x3F, 0x12, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aTargetIndex);
	pMsg.NumberL = SET_NUMBERL(aTargetIndex);

	DataSend(aSourceIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_PARTYREQUESTSEND
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
};

void CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex) 
{
	int number =  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	PMSG_PARTYREQUESTSEND pMsg;

	if ( number < 0 || number > OBJMAX-1)
	{
		return;
	}

	if (!PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	if ( !gObjIsConnected(&gObj[number]))
	{
		::GCResultSend(aIndex, 0x41, 0x03);
		return;
	}

	if ( gObj[aIndex].CloseCount >= 0 || gObj[number].CloseCount >= 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	} 

#ifdef NPVP
	if( g_NewPVP.IsDuel(gObj[aIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3429), aIndex, 1);
		 return;
	}
	// ----
	if( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3430), aIndex, 1);
		 return;
	}
#endif

	if( gObj[number].Type != OBJ_USER ) //season4 add-on
	{
		LogAddC(2,"[HACKTOOL] : NPC-PartyRequest npc:%d ip:%s account:%s name:%s State:%d",
			gObj[number].Class,gObj[aIndex].Ip_addr,gObj[aIndex].AccountID,
			gObj[aIndex].Name,gObj[aIndex].Connected);
		CloseClient(aIndex);
		return;
	}

	if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
		return;
	}

	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) ) //season 2.5 add-on
	{
		::GCServerMsgStringSend(lMsg.Get(3397), aIndex, 1);
		return;
	}

	if ( szAuthKey[4] != AUTHKEY4 )
	{
		DestroyGIocp();
	}

	LogAddL("Party result : %d %d %d %d", gObj[aIndex].CloseCount, gObj[number].CloseCount, gObj[aIndex].Connected, gObj[number].Connected);
	LogAddL("%s %s", gObj[aIndex].Name, gObj[number].Name);

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
		
	if ( gObj[number].m_IfState.use > 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if ( (gObj[number].m_Option &1) != TRUE )
	{
		::GCResultSend(aIndex, 0x41, 0x01);
		return;
	}

	if ( gObj[aIndex].PartyNumber >= 0 )
	{
		int iUserIndex;
		int iDbNumber;
		gParty.GetIndexUser(gObj[aIndex].PartyNumber, 0, iUserIndex, iDbNumber);
		
		if ( iUserIndex < 0 || ( iUserIndex != aIndex && gObj[aIndex].DBNumber != iDbNumber )  )
		{
			::GCResultSend(aIndex, 0x41, 0x00);
			return;

		}
	}

	if ( gObj[aIndex].PartyTargetUser >= 0 )
	{
		return;
	}

	if ( gObj[number].PartyNumber >= 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x04);
		return;
	}

	if ( gObj[number].PartyTargetUser >= 0 )
	{
		::GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

#ifdef GENS
	if( gGensSystem.CheckPartyEnter )
	{
		if( gGensSystem.GetGensInfluence(&gObj[aIndex]) != gGensSystem.GetGensInfluence(&gObj[number]) )
		{
			::GCResultSend(aIndex, 0x41, 0x06);
			return;
		}
	}
	// ----
	//-> Allow party on battle maps
	/*if( gGensSystem.IsMapBattleZone(gObj[aIndex].MapNumber) )
	{
		::GCResultSend(aIndex, 0x41, 0x07);
		return;
	}*/
#endif

	int TotalLevel = 0, PartyCount = 0;
	// ----
	if( gObj[aIndex].PartyNumber >= 0 )
	{
		for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
		{
			int PartyIndex = gParty.m_PartyS[gObj[aIndex].PartyNumber].Number[i];
			// ----
			if( PartyIndex != -1 )
			{
				TotalLevel += gObj[PartyIndex].Level;
				PartyCount++;
			}
		}
		// ----
		TotalLevel = TotalLevel / PartyCount;
	}
	else
	{
		TotalLevel = gObj[aIndex].Level;
	}
	// ----
	if( TotalLevel > gObj[number].Level && TotalLevel - gObj[number].Level >= g_MaxPartyLevelDiff )
	{	
		MsgOutput(aIndex, "You can't create party with %s (%s need %d more levels)", gObj[number].Name, gObj[number].Name, TotalLevel - g_MaxPartyLevelDiff - gObj[number].Level);
		GCResultSend(aIndex, 0x41, 0x05);
		return;
	}
	// ----
	if( TotalLevel < gObj[number].Level && gObj[number].Level - TotalLevel >= g_MaxPartyLevelDiff )
	{		
		MsgOutput(aIndex, "You can't create party with %s (need %d more levels)", gObj[number].Name, gObj[number].Level - g_MaxPartyLevelDiff - TotalLevel);
		GCResultSend(aIndex, 0x41, 0x05);
		return;
	}	
	// ----
	gObj[aIndex].m_IfState.use = TRUE;
	gObj[aIndex].m_IfState.type = 2;
	gObj[aIndex].m_IfState.state = 0;
	gObj[number].m_IfState.use = TRUE;
	gObj[number].m_IfState.type = 2;
	gObj[number].m_IfState.state = 0;
	gObj[aIndex].TargetNumber = number;
	gObj[number].TargetNumber = aIndex;
	gObj[aIndex].m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();
	gObj[aIndex].PartyTargetUser = number;
	
	PHeadSetB((LPBYTE)&pMsg, 0x40, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGPartyRequestResultRecv(PMSG_PARTYREQUESTRESULT * lpMsg, int aIndex) 
{
	int number;
	BYTE result = false;
	char szTemp[256];
	int pnumber = -1;

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < 0 || number > OBJMAX-1)
	{
		return;
	}

	if ( !gObjIsConnected(&gObj[aIndex]))
	{
		return;
	}

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x41, 0x04);
	}
	else
	{
		result = true;
	}

	if( gObj[number].Type != OBJ_USER ) //season4 add-on
	{
		LogAddC(2,"[HACKTOOL] : NPC-TradeRequest(result) npc:%d ip:%s account:%s name:%s State:%d",
			gObj[number].Class,gObj[aIndex].Ip_addr,gObj[aIndex].AccountID,
			gObj[aIndex].Name,gObj[aIndex].Connected);
		CloseClient(aIndex);
		return;
	}

	if ( gObj[number].MapNumber != gObj[aIndex].MapNumber )
	{
		result = false;
		GCResultSend(number, 0x41, 0x00);
	}	

	if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 198)), aIndex, 1);
		result = false;
		::GCResultSend(number, 0x41, 0x00);
	}

	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) ) //season 2.5 add-on
	{
		result = false;
		::GCResultSend(number, 0x41, 0x00);
	}

	if ( lpMsg->Result == 0 )
	{
		result = false;
		::GCResultSend(number, 0x41, 0x01);
	}

	if ( gObj[number].m_IfState.use == 0 || gObj[number].m_IfState.type != 2)
	{
		result = false;
		::GCResultSend(number, 0x41, 0x00);
	}	
	
	if ( gObj[aIndex].m_IfState.use == 0 || gObj[aIndex].m_IfState.type != 2)
	{
		result = false;
		::GCResultSend(number, 0x41, 0x00);
	}

	if ( result == 1 )
	{
		if ( gObj[number].PartyNumber < 0 )
		{
			gObj[number].PartyNumber = gParty.Create(number, gObj[number].DBNumber, gObj[number].Level);
		}

		if ( gObj[number].PartyNumber >= 0  )
		{
			pnumber = gObj[number].PartyNumber;
			int iPartyPos = gParty.Add(gObj[number].PartyNumber, aIndex, gObj[aIndex].DBNumber, gObj[aIndex].Level);

			if ( iPartyPos >= 0 )
			{
				gObj[aIndex].PartyNumber = gObj[number].PartyNumber;
				result = true;
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 83)), gObj[aIndex].Name);
				::GCServerMsgStringSend(szTemp, number, 1);
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 83)), gObj[number].Name);
				::GCServerMsgStringSend(szTemp, aIndex, 1);

				gParty.Paint(pnumber);
			}
			else if ( iPartyPos == -1 )
			{
				GCResultSend(number, 0x41, 2);
				GCResultSend(aIndex, 0x41, 2);
			}
			else if ( iPartyPos == -2 )
			{
				result = 2;
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 84)), gObj[aIndex].Name);
				GCServerMsgStringSend(szTemp, aIndex, 1);
			}
		}
	}

	if ( gObj[aIndex].m_IfState.use !=  0 && gObj[aIndex].m_IfState.type == 2)
	{
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].PartyTargetUser = -1;
	}

	if ( gObj[number].m_IfState.use !=  0 && gObj[number].m_IfState.type == 2)
	{
		gObj[number].m_IfState.use = 0;
		gObj[number].PartyTargetUser = -1;
	}

	if ( pnumber >= 0 )
	{
		CGPartyListAll(pnumber);
	}
}

struct PMSG_PARTYLIST
{
	char szId[10];
	BYTE Number;
	BYTE MapNumber;
	BYTE X;
	BYTE Y;
	int Life;
	int MaxLife;
};

struct PMSG_PARTYLISTCOUNT
{
	PBMSG_HEAD h;
	BYTE Result;
	BYTE Count;
};

void CGPartyList(int aIndex) 
{
	PMSG_PARTYLISTCOUNT pCount;
	PMSG_PARTYLIST pList;
	BYTE sendbuf[512];
	int lOfs = 0;

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	PHeadSetB((LPBYTE)&pCount, 0x42, sizeof(pCount));
	pCount.Count = 0;
	pCount.Result = false;

	int number;
	int pnumber = gObj[aIndex].PartyNumber ;

	if ( pnumber >= 0 )
	{
		pCount.Count = gParty.GetCount(pnumber);
	}

	lOfs += sizeof(pCount);

	if ( pCount.Count != 0 )
	{
		for ( int n=0;n<MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyS[pnumber].Number[n];

			if ( number >= 0 )
			{
				if ( gObjIsConnected(&gObj[number], gParty.m_PartyS[pnumber].DbNumber[n]) == TRUE )
				{
					memset(&pList, 0, sizeof(pList));
					memcpy(pList.szId, gObj[number].Name, MAX_ACCOUNT_LEN);
					pList.Number = n;
					pList.MapNumber = gObj[number].MapNumber;
					pList.X = gObj[number].X;
					pList.Y = gObj[number].Y;
					pList.Life = gObj[number].Life;
					pList.MaxLife = gObj[number].MaxLife + gObj[number].AddLife;

					memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
					lOfs += sizeof(pList);
				}
			}
		}

		pCount.Result = true;
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));
	
	DataSend(aIndex, (LPBYTE)&sendbuf, lOfs);
}

void CGPartyListAll(int pnumber) 
{
	PMSG_PARTYLISTCOUNT pCount;
	PMSG_PARTYLIST pList;
	BYTE sendbuf[512];
	int lOfs = 0;
	int number;

	if ( pnumber < 0 )
	{
		return;
	}

	if ( gParty.IsParty(pnumber) == FALSE )
	{
		return;
	}

	PHeadSetB((LPBYTE)&pCount, 0x42, sizeof(pCount));
	pCount.Result = false;
	pCount.Count = gParty.GetCount(pnumber);

	if ( pCount.Count == 0 )
	{
		return;
	}

	pCount.Result = true;
	lOfs += sizeof(pCount);

	for ( int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[pnumber].Number[n];

		if ( number >= 0 )
		{
			if ( gObjIsConnected(&gObj[number], gParty.m_PartyS[pnumber].DbNumber[n]) == TRUE )
			{
				memset(&pList, 0, sizeof(pList));
				memcpy(pList.szId, gObj[number].Name, MAX_ACCOUNT_LEN);
				pList.Number = n;
				pList.MapNumber = gObj[number].MapNumber;
				pList.X = gObj[number].X;
				pList.Y = gObj[number].Y;
				pList.Life = gObj[number].Life;
				pList.MaxLife =  gObj[number].MaxLife;

				memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
				lOfs += sizeof(pList);
			}
		}
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	for (int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[pnumber].Number[n];

		if ( number >= 0 )
		{
			DataSend(number, sendbuf, lOfs);
		}
	}
}

//0045fc70
void CGPartyDelUser(PMSG_PARTYDELUSER * lpMsg, int aIndex) 
{
	int usernumber = -1; //loc1
	int dbnumber = -1; //loc2

	if ( gObj[aIndex].PartyNumber < 0 )
	{
		LogAdd("No Party %s %s", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	if ( !gParty.GetIndexUser(gObj[aIndex].PartyNumber, lpMsg->Number, usernumber, dbnumber))
	{
		LogAdd("User Not found");
		return;
	}

	if ( usernumber < 0 )
	{
		LogAdd("User Not found");
		return;
	}

	int count; //loc3
	int number; //loc4
	int pnumber = gObj[aIndex].PartyNumber; //loc5

	if ( pnumber < 0 )
	{
		LogAdd("%s %d", __FILE__, __LINE__);
		return;
	}

	count = gParty.GetCount(pnumber);

	if ( count < 1 )
	{
		LogAdd("%s %d", __FILE__, __LINE__);
		return;
	}
#ifdef IMPERIAL
	if ( CImperialGuardian::IsEventMap(gObj[usernumber].MapNumber) )
		return;
#endif

	if ( lpMsg->Number == 0 )
	{
		bool bDPUser = 0;
		for (int j = 1; j < 5; ++j )
		{
			int nUserIndex = gParty.m_PartyS[pnumber].Number[j];
			if ( nUserIndex != -1 )
			{
#ifdef IMPERIAL
				if ( CImperialGuardian::IsEventMap(gObj[nUserIndex].MapNumber) )
				{
					bDPUser = 1;
					break;
				}
#endif
			}
		}
		if ( bDPUser == 1 && count > 2 )
		{
			gParty.Delete(pnumber, lpMsg->Number);
			count = gParty.GetCount(gObj[aIndex].PartyNumber);
			gObj[usernumber].PartyNumber = -1;
			gObj[usernumber].PartyTargetUser = -1;
			gParty.UpdatePKPartyPanalty(pnumber);
			gParty.ChangeLeader(pnumber);
			GCPartyDelUserSend(usernumber);
			CGPartyListAll(pnumber);
			return;
		}
	}
	if ( gObj[usernumber].lpGuild )
	{
		if ( gObj[usernumber].lpGuild->WarState == 1 && gObj[usernumber].IsInBattleGround )
		{
			GCServerMsgStringSend(lMsg.Get(3469), aIndex, 1u);
			return;
		}
		if ( usernumber == gParty.m_PartyS[pnumber].Number[0] )
		{
			for (int k = 0; k < 5; ++k )
			{
				int nUserIndex = gParty.m_PartyS[pnumber].Number[k];
				if ( nUserIndex >= 0 && gObj[nUserIndex].lpGuild
					&& gObj[nUserIndex].lpGuild->WarState == 1
					&& gObj[nUserIndex].IsInBattleGround )
				{
					GCServerMsgStringSend(lMsg.Get(3469), aIndex, 1u);
					return;
				}
			}
		}
	}


	if ( lpMsg->Number == 0 || count <= 2)
	{
		if(IT_MAP_RANGE(gObj[usernumber].MapNumber) != FALSE ) //season 2.5 add-on
		{
			if(lpMsg->Number == 0 && count > 2 )
			{
				gParty.Delete(pnumber, lpMsg->Number);
				count = gParty.GetCount(gObj[aIndex].PartyNumber);

				gObj[usernumber].PartyNumber = -1;
				gObj[usernumber].PartyTargetUser = -1;

				gParty.UpdatePKPartyPanalty(pnumber);
				gParty.ChangeLeader(pnumber);
				GCPartyDelUserSend(usernumber);
				CGPartyListAll(pnumber);

				if(count == 0)
				{
					gParty.Destroy(pnumber);
				}
				return;
			}
		}
		if(IT_MAP_RANGE(gObj[usernumber].MapNumber) != FALSE ) //season 2.5 add-on
		{
			if(count <= 2 )
			{
				gParty.Delete(pnumber, lpMsg->Number);
				count = gParty.GetCount(gObj[aIndex].PartyNumber);

				gObj[usernumber].PartyNumber = -1;
				gObj[usernumber].PartyTargetUser = -1;

				gParty.UpdatePKPartyPanalty(pnumber);

				if(lpMsg == NULL)
				{
					gParty.ChangeLeader(pnumber);
				}

				GCPartyDelUserSend(usernumber);
				CGPartyListAll(pnumber);
				return;
			}
		}

		for ( int n=0;n<MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyS[pnumber].Number[n];

			if ( number >= 0 )
			{
				gParty.Delete(pnumber, n);
				gObj[number].PartyNumber = -1;
				gObj[number].PartyTargetUser = -1;
				GCPartyDelUserSend(number);
			}
		}

		gParty.Destroy(pnumber);
	}
	else
	{
		gParty.Delete(pnumber, lpMsg->Number);
		count = gParty.GetCount(gObj[aIndex].PartyNumber);
		gObj[usernumber].PartyNumber = -1;
		gObj[usernumber].PartyTargetUser = -1;
		gParty.UpdatePKPartyPanalty(pnumber); //season 2.5 add-on
		GCPartyDelUserSend(usernumber);
		CGPartyListAll(pnumber);
	}		
}

struct PMSG_PARTYDELUSERSEND
{
	PBMSG_HEAD h;
};

void GCPartyDelUserSend(int aIndex) 
{
	PMSG_PARTYDELUSERSEND pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x43, sizeof(pMsg));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_GUILDQUESTSEND
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
};

void CGGuildRequestRecv(PMSG_GUILDJOINQ * lpMsg, int aIndex) 
{
	PMSG_GUILDQUESTSEND pMsg;
	int number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < 0 || number > OBJMAX-1 )
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

//#if(_GSCS==1)
	if(g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
	{
		MsgOutput(aIndex,(lMsg.Get(MSGGET(6, 194))));
		return;
	}
//#endif

	if( IT_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE ) //season 2.5 add-on
	{
		MsgOutput(aIndex,(lMsg.Get(3398)));
		return;
	}

#ifdef NPVP
	if( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[number]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3426), aIndex, 1);
		 return;
	}
	// ----
	if( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3427), aIndex, 1);
		 return;
	}
#endif

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}
	
	if ( gObj[number].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}

	if ( gObj[aIndex].Level < 6 )
	{
		GCResultSend(aIndex, 0x51, 0x07);
		return;
	}

	if ( gObj[aIndex].GuildNumber > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x05);
		return;
	}

	if ( (gObj[number].m_Option&1 ) != 1 )
	{
		GCResultSend(aIndex, 0x51, 0x00);
		return;
	}

	if( gObj[number].Type != OBJ_USER ) //season4 add-on
	{
		LogAddC(2,"[HACKTOOL] : NPC-GuildRequest npc:%d ip:%s account:%s name:%s State:%d",
			gObj[number].Class,gObj[aIndex].Ip_addr,gObj[aIndex].AccountID,
			gObj[aIndex].Name,gObj[aIndex].Connected);
		CloseClient(aIndex);
		return;
	}

	if ( gObj[number].GuildNumber > 0 )
	{
#ifdef __NOVUS__
		if( gObj[number].GuildStatus != G_MASTER && gObj[number].GuildStatus != G_SUB_MASTER )
		{
			GCResultSend(aIndex, 0x51, 0x04);
			return;
		}
#else
		if ( strcmp(gObj[number].lpGuild->Names[0], gObj[number].Name))
		{
			GCResultSend(aIndex, 0x51, 0x04);
			return;
		}
#endif
		if ( gObj[number].lpGuild->WarState )
		{
			GCResultSend(aIndex, 0x51, 0x06);
			return;
		}

		int MaxGuildMember;

		if ( gObj[number].Class == CLASS_DARKLORD )
		{
			MaxGuildMember = (gObj[number].Level+gObj[number].m_nMasterLevel) / 10 + gObj[number].Leadership / 10;
		}
		else
		{
			MaxGuildMember = (gObj[number].Level+gObj[number].m_nMasterLevel) / 10;
		}

		if ( MaxGuildMember > MAX_USER_GUILD )
		{
			MaxGuildMember = MAX_USER_GUILD;
		}

		if ( gObj[number].lpGuild->TotalCount >= MaxGuildMember )
		{
			GCResultSend(aIndex, 0x51, 0x02);
			return;
		}

#ifdef GENS
		if( gGensSystem.CheckGuildEnter )
		{
			if( !gGensSystem.GetGensInfluence(&gObj[aIndex]) )
			{
				LogAddTD("[GensSystem] [%s][%s] Guild Joining Failed to Not Join Gens", 
					gObj[aIndex].AccountID, gObj[aIndex].Name);
				// ----
				GCResultSend(aIndex, 0x51, 0xA3);
				return;
			}
			// ----
			if( !gGensSystem.GetGensInfluence(&gObj[number]) )
			{
				LogAddTD("[GensSystem] [%s][%s] Guild Joining Failed to Not Join Gens GuildMaster", 
					gObj[aIndex].AccountID, gObj[aIndex].Name);
				// ----
				GCResultSend(aIndex, 0x51, 0xA1);
				return;
			}
			// ----
			if( gGensSystem.GetGensInfluence(&gObj[number]) != gGensSystem.GetGensInfluence(&gObj[aIndex]) )
			{
				LogAddTD("[GensSystem] [%s][%s] [%s] Guild Joining Failed to Gens be Different : [%s][%s] [%s]", 
					gObj[aIndex].AccountID, gObj[aIndex].Name, gGensSystem.GetGensInfluenceName(&gObj[aIndex]),
					gObj[number].AccountID, gObj[number].Name, gGensSystem.GetGensInfluenceName(&gObj[number]));
				// ----
				GCResultSend(aIndex, 0x51, 0xA2);
				return;
			}
		}
#endif
	}
	else
	{
		GCResultSend(aIndex, 0x51, 0x04);
		return;
	}

	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 4;
	gObj[aIndex].m_IfState.state = 0;

	gObj[number].m_IfState.use = 1;
	gObj[number].m_IfState.type = 4;
	gObj[number].m_IfState.state = 0;

	gObj[aIndex].TargetNumber = number;
	gObj[number].TargetNumber = aIndex;
	gObj[aIndex].m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();

	PHeadSetB((LPBYTE)&pMsg, 0x50, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);

	LogAddTD("[%s][%s] Guild Request - Join : %s", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].lpGuild->Name);
}

void CGGuildRequestResultRecv(PMSG_GUILDQRESULT * lpMsg, int aIndex) 
{
	
	int number;
	BYTE result=0;
	int pnumber=-1;

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < 0 || number > OBJMAX-1)
	{
		return;
	}

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x51, 0x02);
	}
	else
	{
		result = 1;
	}

	if( gObj[number].Type != OBJ_USER ) //season4 add-on
	{
		LogAddC(2,"[HACKTOOL] : NPC-GuildRequest(result) npc:%d ip:%s account:%s name:%s State:%d",
			gObj[number].Class,gObj[aIndex].Ip_addr,gObj[aIndex].AccountID,
			gObj[aIndex].Name,gObj[aIndex].Connected);
		CloseClient(aIndex);
		return;
	}

	if ( lpMsg->Result == 0 )
	{
		result = 0;
		GCResultSend(number, 0x51, 0x00);
	}

	if ( result == 1 )
	{
		if ( gObj[aIndex].GuildNumber > 0 )
		{
			GDGuildMemberAdd(number, gObj[aIndex].lpGuild->Name, gObj[number].Name);
		}
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 4)
	{
		gObj[aIndex].m_IfState.use = 0;
	}
		
	if ( gObj[number].m_IfState.use && gObj[number].m_IfState.type == 4)
	{
		gObj[number].m_IfState.use = 0;
	}
}

struct PMSG_GUILDLIST
{
	char Name[10];
	BYTE Number;
	BYTE ConnectAServer;
	BYTE btGuildStatus;
};

struct PMSG_GUILDLISTCOUNT
{
	PWMSG_HEAD h;
	BYTE Result;
	BYTE Count;
	int TotalScore;
	BYTE Score;
	char szRivalGuild[9];
};

void CGGuildListAll(int pnumber) 
{
	if ( gObj[pnumber].lpGuild == NULL )
	{
		return;
	}

	PMSG_GUILDLISTCOUNT pCount;
	PMSG_GUILDLIST pList;
	char sendbuf[1500];
	int lOfs = 0;
	int number;
	int n;
	int guildmaster = 0;
	
	PHeadSetW((LPBYTE)&pCount, 0x52, 0);
	pCount.Result = 0;
	pCount.Count = 0;
	lOfs += sizeof(pCount);
	pCount.h.sizeH = SET_NUMBERH(lOfs);
	pCount.h.sizeL = SET_NUMBERL(lOfs);

	if ( gObj[pnumber].GuildNumber < 1 )
	{
		DataSend(pnumber, (LPBYTE)&pCount, lOfs);
		return;
	}

	pCount.Count = gObj[pnumber].lpGuild->Count;
	pCount.TotalScore = gObj[pnumber].lpGuild->TotalScore;
	pCount.Score = gObj[pnumber].lpGuild->PlayScore;
	memset(pCount.szRivalGuild, 0, sizeof(pCount.szRivalGuild));

	if ( gObj[pnumber].lpGuild->iGuildRival )
	{
		memcpy(pCount.szRivalGuild, gObj[pnumber].lpGuild->szGuildRivalName, MAX_GUILD_LEN);
	}

	pCount.Result = 1;
	guildmaster = 1;

	if ( pCount.Count == 0 )
	{
		return;
	}

	pCount.Count = 0;

	for ( n=0;n<MAX_USER_GUILD;n++)
	{
		number = gObj[pnumber].lpGuild->Use[n];

		if ( number > 0 )
		{
			memset(&pList, 0, sizeof(pList));
			memcpy(pList.Name, gObj[pnumber].lpGuild->Names[n], sizeof(pList.Name));
			pList.Number = gObj[pnumber].lpGuild->pServer[n];
			pList.ConnectAServer = (short)gObj[pnumber].lpGuild->pServer[n] & 0x7F;

			if ( gObj[pnumber].lpGuild->pServer[n] >= 0 )
			{
				pList.ConnectAServer |= 0x80;
			}

			pList.btGuildStatus = gObj[pnumber].lpGuild->GuildStatus[n];
			memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			lOfs+= sizeof(pList);
			pCount.Count++;
		}
	}

	pCount.h.sizeH = SET_NUMBERH(lOfs);
	pCount.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(sendbuf, (LPBYTE)&pCount, sizeof(pCount));

	DataSend(pnumber, (LPBYTE)sendbuf, lOfs);
}

struct PMSG_GUILDDELUSER_RESULT
{
	PBMSG_HEAD h;
	BYTE Result;
};

void CGGuildDelUser(PMSG_GUILDDELUSER * lpMsg, int aIndex) 
{
	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

//#if(_GSCS==1)
	if(g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) //Good
	{
		MsgOutput(aIndex,(lMsg.Get(MSGGET(6, 195))));
		return;
	}
//#endif

	if ( gObj[aIndex].GuildNumber < 1 )
		return;

	char joomin[14];
	char memberid[11];
	char guildname[11];
	PMSG_GUILDDELUSER_RESULT pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x53, sizeof(pMsg));
	pMsg.Result = 3;

	if ( !gGuildDestroy )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	memset(memberid, 0, sizeof(memberid));
	memset(guildname, 0, sizeof(guildname));
	memcpy(memberid, lpMsg->Name, MAX_ACCOUNT_LEN);
	strcpy(guildname, gObj[aIndex].lpGuild->Name);

	if( !strcmp(gObj[aIndex].lpGuild->Names[0], gObj[aIndex].Name ))
	{
		memset(joomin, 0, sizeof(joomin));
		memcpy(joomin, lpMsg->JoominNumber, 10);

		//if ( gObjJoominCheck(aIndex, joomin) == TRUE )
		//{
			if ( !strcmp(memberid, gObj[aIndex].Name ))
			{
//#if(_GSCS==1)
				if( g_bCastleGuildDestoyLimit != 0 )
				{
					if( !strcmp(gObj[aIndex].lpGuild->Name,g_CastleSiege.GetCastleOwnerGuild()) )
					{
						MsgOutput(aIndex,lMsg.Get(0x6BD));
						return;
					}
				}
//#endif
				gObjGuildWarMasterClose(&gObj[aIndex]);
				GDGuildDestroySend(aIndex, gObj[aIndex].lpGuild->Name, gObj[aIndex].Name);
			}
			else
			{
				GDGuildMemberDel(aIndex, guildname, memberid);
			}

			LogAddTD("[%s][%s] Guild Request - Dismiss All : %s",
				gObj[aIndex].AccountID,gObj[aIndex].Name, guildname);
		//}
		//else
		//{
		//	pMsg.Result = 0;
		//	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		//	return;
		//}
	}
	else
	{
		if ( strcmp(memberid, gObj[aIndex].Name) )
		{
			pMsg.Result = 0;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		memset(joomin, 0, sizeof(joomin));
		memcpy(joomin, lpMsg->JoominNumber, 10);

		//if ( gObjJoominCheck(aIndex, joomin) == TRUE )
		//{
			GDGuildMemberDel(aIndex, guildname, memberid);
		//}
		//else
		//{
		//	GCServerMsgStringSend(lMsg.Get(MSGGET(4, 108)), aIndex, 1);
		//}

		LogAddTD("[%s][%s] Guild Request - Leave : %s",	gObj[aIndex].AccountID, gObj[aIndex].Name, guildname);
	}
}

void GCGuildDelUserResult(int aIndex, BYTE Result) 
{
	PMSG_GUILDDELUSER_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x53, sizeof(pMsg));
	pMsg.Result = Result;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCGuildMasterQuestionSend(int aIndex) 
{
	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		return;
	}

	PMSG_GUILDMASTERQUESTION pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x54, sizeof(pMsg));
	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 5;
	gObj[aIndex].m_IfState.state = 0;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGGuildMasterAnswerRecv(PMSG_GUILDMASTERANSWER * lpMsg, int aIndex) 
{
	if ( lpMsg->Result == 1 )
	{
		if ( !gObjGuildMasterCapacityTest(&gObj[aIndex]) )
		{
			if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
			{
				gObj[aIndex].m_IfState.use = 0;
			}

			return;
		}

		GCGuildMasterManagerRun(aIndex);
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}

void GCGuildMasterManagerRun(int aIndex) 
{
	PMSG_GUILDMASTERMANAGER_RUN pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x55, sizeof(pMsg));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGGuildMasterInfoSave(int aIndex, PMSG_GUILDINFOSAVE * lpMsg) 
{
	char GuildName[MAX_GUILD_LEN+1];
	GuildName[MAX_GUILD_LEN] ='\0';
	memcpy(GuildName, lpMsg->GuildName, MAX_GUILD_LEN);
	int len = strlen(GuildName);

	if ( len <= 2 || len > MAX_GUILD_LEN)
	{
		PMSG_GUILDCREATED_RESULT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x56, sizeof(pMsg));
		pMsg.Result = 2;
		
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
		{
			gObj[aIndex].m_IfState.use = 0;
		}

		return;
	}

	GDGuildCreateSend(aIndex, GuildName, gObj[aIndex].Name, lpMsg->Mark, lpMsg->btGuildType);

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}

void CGGuildMasterCreateCancel(int aIndex) 
{
	if ( gObj[aIndex].GuildNumber > 0 )
	{
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}

void GCGuildViewportNowPaint(int aIndex, char* guildname, BYTE* mark, BOOL isGuildMaster) 
{
	_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(guildname);

	if ( lpGuild == NULL )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	BYTE _GuildInfoBuf[256] = {0};
	int _GuildInfoOfs = 5;

	if ( lpObj->lpGuild == NULL )
	{
		return;
	}

	PMSG_SIMPLE_GUILDVIEWPORT pMsg;
	pMsg.GuildNumber = lpObj->lpGuild->Number;
	pMsg.NumberH = SET_NUMBERH(aIndex)&0x7F;
	pMsg.NumberL = SET_NUMBERL(aIndex);

	if ( isGuildMaster != FALSE )
	{
		pMsg.NumberH |= 0x80;
	}


	if ( isGuildMaster != FALSE )
	{
		pMsg.btGuildStatus = 0x80;
	}
	else
	{
		pMsg.btGuildStatus = lpObj->GuildStatus;
	}

	pMsg.btGuildType = lpObj->lpGuild->btGuildType;

	pMsg.btGuildRelationShip = 0;

	lpObj->iGuildUnionTimeStamp = 0;

	if( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == TRUE || //season 4 add-on
		g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE)
	{
		pMsg.btOwnerStatus = 1;
	}
	else
	{
		pMsg.btOwnerStatus = 0;
	}

	memcpy(&_GuildInfoBuf[_GuildInfoOfs], &pMsg, sizeof(pMsg));
	_GuildInfoOfs += sizeof(pMsg);

	PMSG_SIMPLE_GUILDVIEWPORT_COUNT pGVCount;

	pGVCount.h.c = 0xC2;
	pGVCount.h.headcode = 0x65;
	pGVCount.h.sizeH = SET_NUMBERH(_GuildInfoOfs);
	pGVCount.h.sizeL = SET_NUMBERL(_GuildInfoOfs);
	pGVCount.Count = 1;
	memcpy(_GuildInfoBuf, &pGVCount, sizeof(pGVCount));

	if ( !CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		DataSend(aIndex, _GuildInfoBuf, _GuildInfoOfs);
	}

	MsgSendV2(lpObj, _GuildInfoBuf, _GuildInfoOfs);
}

struct PMSG_GUILDDEL_VIEWPORT_NOW
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
};

void GCGuildViewportDelNow(int aIndex, BOOL isGuildMaster) 
{
	PMSG_GUILDDEL_VIEWPORT_NOW pMsg;

	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pMsg, 0x5D, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex) & 0x7F;
	pMsg.NumberL = SET_NUMBERL(aIndex);

	if ( isGuildMaster != FALSE )
	{
		pMsg.NumberH |= 0x80;
	}

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCManagerGuildWarEnd(char * GuildName) 
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(GuildName);

	if ( lpNode == NULL )
	{
		return;
	}

	int n=0;
	int warmaster = -1;

	while ( true )
	{
		if ( gObj[n].Type == OBJ_USER )
		{
			if ( gObj[n].Connected > PLAYER_LOGGED )
			{
				if ( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if ( strcmp(gObj[n].Name, lpNode->Names[0]) == 0 )
					{
						warmaster = n;
						break;
					}
				}
			}
		}

		if ( n < OBJMAX-1 )
		{
			n++;
		}
		else
		{
			break;
		}
	}

	if ( warmaster >= 1 )
	{
		BYTE Result1 = 0;
		BYTE Result2 = 0;

		if ( lpNode != NULL )
		{
			if ( lpNode->lpTargetGuildNode != NULL )
			{
				if ( lpNode->PlayScore == 0 && lpNode->lpTargetGuildNode->PlayScore == 0 )
				{
					Result1 = 6;
					Result2 = 6;
				}
				else
				{
					Result1 = 0;
					Result2 = 1;
				}
			}
		}

		if ( lpNode->WarType == 1 )
		{
			gBattleGroundEnable(lpNode->BattleGroundIndex, FALSE );
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, Result1, Result2);
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);

			cManager.BattleInfoSend(::GetBattleTeamName(0, 0), -1, ::GetBattleTeamName(0, 1), -1);
		}
		else
		{
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, Result1, Result2);
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);
		}
	}
}

void GCManagerGuildWarSet(LPSTR GuildName1, LPSTR GuildName2, int type) 
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(GuildName1);

	if ( lpNode == NULL )
	{
		return;
	}

	int n=0;
	int warmaster = -1;

	while ( true )
	{
		if ( gObj[n].Type == OBJ_USER )
		{
			if ( gObj[n].Connected > PLAYER_LOGGED )
			{
				if ( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if ( strcmp(gObj[n].Name, lpNode->Names[0]) == 0 )
					{
						warmaster = n;
						break;
					}
				}
			}
		}

		if ( n < OBJMAX-1 )
		{
			n++;
		}
		else
		{
			break;
		}
	}

	if ( warmaster >= 1 )
	{
		::GCGuildWarRequestResult(GuildName2, warmaster, type);
	}
}

struct PMSG_GUILDWARREQUEST_RESULT
{
	PBMSG_HEAD h;
	BYTE Result;
};

void GCGuildWarRequestResult(LPSTR GuildName, int aIndex, int type) 
{
//#if(_GSCS == 1)
//	return;
//#endif
	PMSG_GUILDWARREQUEST_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x60, sizeof(pMsg));
	pMsg.Result = 3;

	if ( gObj[aIndex].GuildNumber < 1)
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	_GUILD_INFO_STRUCT * lpMyGuild = gObj[aIndex].lpGuild;

	if ( !lpMyGuild )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if ( lpMyGuild->WarState == 1 || lpMyGuild->WarDeclareState == 1 )
	{
		pMsg.Result = 4;
		LogAdd(lMsg.Get(MSGGET(1, 234)), lpMyGuild->WarState, lpMyGuild->WarDeclareState, lpMyGuild->Name);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if ( gPkLimitFree == FALSE && gObj[aIndex].m_PK_Level >= 6)
	{
		pMsg.Result = 4;
		LogAdd("Error on declaring war : GuildMaster Is Murderer 0x04 %s %s", lpMyGuild->Names[0], gObj[aIndex].Name);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if ( strcmp(lpMyGuild->Names[0], gObj[aIndex].Name ) )
	{
		pMsg.Result = 5;
		LogAdd(lMsg.Get(MSGGET(1, 235)), lpMyGuild->Names[0], gObj[aIndex].Name);
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	char _guildname[MAX_GUILD_LEN+1];
	memset(_guildname, 0, sizeof(_guildname));
	memcpy(_guildname, GuildName, MAX_GUILD_LEN);

	if ( !strncmp(lpMyGuild->Name, GuildName, MAX_GUILD_LEN))
	{
		return;
	}

	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(_guildname);

	if ( lpNode )
	{
		if ( lpNode->WarState == 1 || lpNode->WarDeclareState == 1 )
		{
			pMsg.Result = 4;
			LogAdd(lMsg.Get(MSGGET(1, 234)), lpNode->WarState, lpNode->WarDeclareState, lpNode->Name);
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		if ( lpMyGuild->iGuildUnion != 0 && lpMyGuild->iGuildUnion == lpNode->iGuildUnion )
		{
			LogAddTD("[U.System][Guildwar] Can't declare guildwar between Same Union (%s / %d) <-> (%s / %d)", lpMyGuild->Name, lpMyGuild->iGuildUnion, lpNode->Name, lpMyGuild->iGuildUnion);
			return;
		}

		int n=0;
		int warmaster=-1;

		while ( true )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				if ( gObj[n].Connected > PLAYER_LOGGED )
				{
					if ( gObj[n].Name[0] == lpNode->Names[0][0] )
					{
						if ( !strcmp(gObj[n].Name, lpNode->Names[0]))
						{
							if ( gPkLimitFree == FALSE && gObj[n].m_PK_Level >= 6)
							{
								pMsg.Result = 4;

								LogAdd("Error on declaring war : Target GuildMaster Is Murderer 0x04 %s %s", lpMyGuild->Names[0], gObj[aIndex].Name);
								DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
								return;
							}

							warmaster = n;
							break;
						}
					}
				}
			}

			if ( n < OBJMAX-1 )
			{
				n++;
			}
			else
			{
				break;
			}
		}
		
		if ( warmaster >= 1 )
		{
			if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) || CC_MAP_RANGE(gObj[warmaster].MapNumber) )
			{
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 199)), aIndex, 1);
				return;
			}

			if ( (gObj[warmaster].m_Option&1) != 1 )
			{
				pMsg.Result = 4;
				DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				return;
			}

			pMsg.Result = 1;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			GCGuildWarRequestSend(lpMyGuild->Name, warmaster, type);
			lpMyGuild->WarDeclareState = 1;
			lpNode->WarDeclareState = 1;
			lpMyGuild->WarType = type;
			lpNode->WarType = type;

			LogAddTD(lMsg.Get(MSGGET(1, 236)), gObj[aIndex].AccountID, gObj[aIndex].Name, lpMyGuild->Name, lpNode->Name);

			strcpy(lpMyGuild->TargetGuildName, lpNode->Name);
			strcpy(lpNode->TargetGuildName, lpMyGuild->Name);
			lpMyGuild->lpTargetGuildNode = lpNode;
			lpNode->lpTargetGuildNode = lpMyGuild;
		}
		else
		{
			pMsg.Result = 2;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			//return;
		}
	}
	else
	{
		pMsg.Result = 0;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}
}

void GCGuildWarRequestSend(LPSTR GuildName, int aIndex, int type) //check gs-cs
{
	PMSG_GUILDWARSEND pMsg;
//#if(_GSCS==0)
	PHeadSetB((LPBYTE)&pMsg, 0x61, sizeof(pMsg));
	pMsg.Type = type;
	memcpy(pMsg.GuildName, GuildName, MAX_GUILD_LEN);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	LogAddTD(lMsg.Get(MSGGET(1, 237)), GuildName);
//#endif
}

struct PMSG_GUILDWAR_DECLARE
{
	PBMSG_HEAD h;
	char GuildName[8];
	BYTE Type;
	BYTE TeamCode;
};

void GCGuildWarRequestSendRecv(PMSG_GUILDWARSEND_RESULT * lpMsg, int aIndex)
{
//#if(_GSCS == 1)
//	return;
//#endif

	PMSG_GUILDWAR_DECLARE pMsg;
	int count=0;
	int g_call=0;
	
	PHeadSetB((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
	pMsg.Type = 0;
	
	_GUILD_INFO_STRUCT * lpMyNode = gObj[aIndex].lpGuild;

	if ( !lpMyNode )
	{
		PMSG_GUILDWARREQUEST_RESULT pResult;

		pResult.h.c = 0xC1;
		pResult.h.headcode = 0x60;
		pResult.h.size = sizeof(pMsg);
		pResult.Result = 0;

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMyNode->lpTargetGuildNode && lpMsg->Result )
	{
		int iTarGetIndex = lpMyNode->lpTargetGuildNode->Index[0];

		if ( BC_MAP_RANGE(gObj[iTarGetIndex].MapNumber) || CC_MAP_RANGE(gObj[iTarGetIndex].MapNumber) || DS_MAP_RANGE(gObj[iTarGetIndex].MapNumber))
		{
			PMSG_NOTICE pNotice;

			TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 199)));

			DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);

			lpMsg->Result = 0;
		}
	}

	pMsg.Type = lpMyNode->WarType;

	if ( lpMsg->Result == 0 )
	{
		if ( lpMyNode->lpTargetGuildNode )
		{
			if ( lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
			{
				lpMyNode->lpTargetGuildNode->WarDeclareState = 0;
				lpMyNode->WarDeclareState = 0;

				PMSG_GUILDWARREQUEST_RESULT pResult;

				PHeadSetB((LPBYTE)&pResult, 0x60, sizeof(pResult));
				pResult.Result = 6;

				if ( lpMyNode->lpTargetGuildNode->Index[0] >= 0 )
				{
					DataSend(lpMyNode->lpTargetGuildNode->Index[0], (LPBYTE)&pResult, pResult.h.size);
				}
			}
		}
	}
	else
	{
		if ( lpMyNode->WarDeclareState == 1 )
		{
			if ( lpMyNode->lpTargetGuildNode )
			{
				if (lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
				{
					PMSG_TELEPORT pTeleportMsg;

					if ( lpMyNode->WarType == 1 )
					{
						lpMyNode->BattleGroundIndex = gCheckBlankBattleGround();

						switch ( lpMyNode->BattleGroundIndex )
						{
							case 0xFF:
								lpMyNode->WarDeclareState = 0;
								lpMyNode->WarState = 0;
								lpMyNode->lpTargetGuildNode->WarDeclareState = 0;
								lpMyNode->lpTargetGuildNode->WarState = 0;

								PMSG_GUILDWARREQUEST_RESULT pResult;

								PHeadSetB((LPBYTE)&pResult, 0x60, sizeof(pResult));
								pResult.Result = 4;

								DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								return;
						}
	
						pTeleportMsg.MoveNumber = 51;
						lpMyNode->lpTargetGuildNode->BattleGroundIndex = lpMyNode->BattleGroundIndex;

						if ( gBSGround[0]->m_BallIndex >= 0 )
						{
							gObjMonsterRegen(&gObj[gBSGround[0]->m_BallIndex]);
						}

						BattleSoccerGoalEnd(0);
						lpMyNode->PlayScore = 0;
						lpMyNode->lpTargetGuildNode->PlayScore = 0;
						gBattleGroundEnable(lpMyNode->BattleGroundIndex, TRUE);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 0, lpMyNode->Name, lpMyNode);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 1, lpMyNode->lpTargetGuildNode->Name, lpMyNode->lpTargetGuildNode);
					}

					lpMyNode->WarDeclareState = 2;
					lpMyNode->WarState = 1;
					lpMyNode->lpTargetGuildNode->WarDeclareState = 2;
					lpMyNode->lpTargetGuildNode->WarState = 1;
					lpMyNode->PlayScore = 0;
					lpMyNode->lpTargetGuildNode->PlayScore = 0;
					lpMyNode->BattleTeamCode = 0;
					lpMyNode->lpTargetGuildNode->BattleTeamCode = 1;
					memset(pMsg.GuildName, 0, sizeof(pMsg.GuildName));
					memcpy(pMsg.GuildName, lpMyNode->lpTargetGuildNode->Name, sizeof(pMsg.GuildName));
					pMsg.TeamCode = lpMyNode->BattleTeamCode;
					count = 0;

					for ( int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpMyNode->Use[n] )
						{
							if ( lpMyNode->Index[n] >= 0 )
							{
								g_call = 0;

								if ( n > 0 )
								{
									if ( lpMyNode->WarType == 1 )
									{
										if ( gObj[lpMyNode->Index[0]].PartyNumber  >= 0 )
										{
											if ( gObj[lpMyNode->Index[0]].PartyNumber == gObj[lpMyNode->Index[n]].PartyNumber )
											{
												g_call = 1;
											}
										}
									}
									else 
									{
										g_call = 1;
									}
								}
								else if ( n== 0 )
								{
									g_call = 1;
								}

								if ( g_call )
								{
									DataSend(lpMyNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);

									// Declared guild war against (%s)
									LogAddTD(lMsg.Get(MSGGET(1, 238)), lpMyNode->Name);
									GCGuildWarScore(lpMyNode->Index[n]);
									int x = 60;

									if ( lpMyNode->WarType == 1 )
									{
										gObj[lpMyNode->Index[n]].IsInBattleGround = true;
										pTeleportMsg.MoveNumber = 51;

										if ( gPkLimitFree || gObj[lpMyNode->Index[n]].m_PK_Level < 6 )
										{
											gObjTeleport(lpMyNode->Index[n], 6, x++, 153);
											count++;
										}
									}
								}
							}
						}
					}

					memset(pMsg.GuildName, 0, sizeof(pMsg.GuildName));
					memcpy(pMsg.GuildName, lpMyNode->Name, sizeof(pMsg.GuildName));

					pMsg.TeamCode = lpMyNode->lpTargetGuildNode->BattleTeamCode;
					count = 0;

					for (int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpMyNode->lpTargetGuildNode->Use[n] )
						{
							if ( lpMyNode->lpTargetGuildNode->Index[n] >= 0 )
							{
								g_call = 0;

								if ( n > 0 )
								{
									if ( lpMyNode->WarType == 1 )
									{
										if ( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber  >= 0 )
										{
											if ( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber == gObj[lpMyNode->lpTargetGuildNode->Index[n]].PartyNumber )
											{
												g_call = 1;
											}
										}
									}
									else 
									{
										g_call = 1;
									}
								}
								else if ( n== 0 )
								{
									g_call = 1;
								}

								if ( g_call )
								{
									DataSend(lpMyNode->lpTargetGuildNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);

									LogAddTD(lMsg.Get(MSGGET(1, 238)), lpMyNode->lpTargetGuildNode->Name);
									GCGuildWarScore(lpMyNode->lpTargetGuildNode->Index[n]);
									int x = 59;

									if ( lpMyNode->lpTargetGuildNode->WarType == 1 )
									{

										if ( gPkLimitFree != 0 || gObj[lpMyNode->lpTargetGuildNode->Index[n]].m_PK_Level < 6 )
										{
											gObj[lpMyNode->lpTargetGuildNode->Index[n]].IsInBattleGround = true;
											gObjTeleport(lpMyNode->lpTargetGuildNode->Index[n], 6, x++, 164);
											count++;
										}
									}
								}
							}
						}
					}

					cManager.BattleInfoSend(GetBattleTeamName(0,0), GetBattleTeamScore(0,0), GetBattleTeamName(0, 1), GetBattleTeamScore(0,1));

					if ( lpMyNode->WarType == 1 )
					{
						gObjAddMsgSendDelay(&gObj[aIndex], 5, aIndex, 10000, 0);
						GCServerMsgStringSendGuild(lpMyNode->lpTargetGuildNode,lMsg.Get(MSGGET(4, 130)), 1);
						GCServerMsgStringSendGuild(lpMyNode,lMsg.Get(MSGGET(4, 130)), 1);
					}
				}
			}
		}
	}
}

void GCGuildWarDeclare(int aIndex, LPSTR _guildname) //check gs-cs
{
	PMSG_GUILDWAR_DECLARE pMsg;
//#if(_GSCS==0)
	PHeadSetB((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
	memcpy(pMsg.GuildName, _guildname, sizeof(pMsg.GuildName));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
//#endif
}

struct PMSG_GUILDWAR_END
{
	PBMSG_HEAD h;
	BYTE Result;
	char GuildName[8];
};

void GCGuildWarEnd(int aIndex, BYTE result, char* _guildname) //check gs-cs
{
	PMSG_GUILDWAR_END pMsg;
//#if(_GSCS==0)
	PHeadSetB((LPBYTE)&pMsg, 0x63, sizeof(pMsg));
	pMsg.Result = result;
	memcpy(pMsg.GuildName, _guildname, sizeof(pMsg.GuildName));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
//#endif
}

struct PMSG_GUILDSCORE
{
	PBMSG_HEAD h;
	BYTE Score1;
	BYTE Score2;
	BYTE Type;
};

void GCGuildWarScore(int aIndex) //check gs-cs
{
	PMSG_GUILDSCORE pMsg;
//#if(_GSCS==0)
	if ( gObj[aIndex].GuildNumber < 1 )
	{
		return;
	}

	PHeadSetB((LPBYTE)&pMsg, 0x64, sizeof(pMsg));
	pMsg.Score1 = 0;
	pMsg.Score2 = 0;
	pMsg.Type = 0;

	if ( gObj[aIndex].lpGuild != NULL )
	{
		pMsg.Score1 = gObj[aIndex].lpGuild->PlayScore;
		LogAdd("Score %s %d", gObj[aIndex].Name, gObj[aIndex].lpGuild->PlayScore);
	}

	if ( gObj[aIndex].lpGuild->lpTargetGuildNode != NULL )
	{
		pMsg.Score2 = gObj[aIndex].lpGuild->lpTargetGuildNode->PlayScore;
		LogAdd("Target Score %s %d", gObj[aIndex].Name, gObj[aIndex].lpGuild->lpTargetGuildNode->PlayScore);
	}

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
//#endif
}	

void CGWarehouseMoneyInOut(int aIndex, PMSG_WAREHOUSEMONEYINOUT* lpMsg) 
{
	int money = 0;

	if ( !gObjIsConnected(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	money = lpMsg->Money;
	
	if ( lpObj->m_IfState.state == 0 )
	{
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		return;
	}

	if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 6 )
	{
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		LogAdd("[%s][%s] error-L1 : used not Warehouse", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( money <= 0 || money > MAX_WAREHOUSE_ZEN )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			if ( (DWORD)(lpObj->WarehouseMoney+money) > MAX_WAREHOUSE_ZEN ) //fix :D
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			if ( money > lpObj->Money )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			lpObj->Money-=money;
			lpObj->WarehouseMoney+=money;
			break;
		case 0x01:
			{
				if ( bCanWarehouseLock == TRUE)
				{
					if ( lpObj->WarehouseLock == 1)
					{
						GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
						GCServerMsgStringSend(lMsg.Get(MSGGET(6, 70)), lpObj->m_Index,1);
						return;
					}
				}

				if ( money <= 0 || money > MAX_WAREHOUSE_ZEN )
				{
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					return;
				}
				
				if ( money > lpObj->WarehouseMoney )
				{
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					return;
				}

				lpObj->WarehouseMoney -= money;
				lpObj->Money += money;
				int rZen = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->m_nMasterLevel, lpObj->WarehousePW);

				if ( (lpObj->Money-rZen) > 0 )
				{
					int oldmoney = lpObj->Money;
					lpObj->Money -= rZen;
					GCMoneySend(lpObj->m_Index, lpObj->Money);
					
					LogAdd("Get WareHouse Money(In Inventory) : %d - %d = %d", oldmoney, rZen, lpObj->Money);
				}
				else if ( (lpObj->WarehouseMoney-rZen) > 0 )
				{
					int oldmoney = lpObj->WarehouseMoney;
					lpObj->WarehouseMoney -= rZen;

					LogAdd("Get WareHouse Money(In WareHouse) : %d - %d = %d", oldmoney, rZen, lpObj->WarehouseMoney);

					GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
				}
				else
				{
					lpObj->WarehouseMoney +=money;
					lpObj->Money -= money;
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);

					char msg[255];

					wsprintf(msg, lMsg.Get(MSGGET(6, 69)), rZen);
					GCServerMsgStringSend(msg, lpObj->m_Index, 1);
					return;
				}
			}
			break;

		default:
			GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
			return;
	}

	lpObj->WarehouseCount++;
	GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
}

void GCWarehouseInventoryMoneySend(int aIndex, BYTE result, int money, int wmoney) 
{
	PMSG_MONEY pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x81, sizeof(pMsg));
	pMsg.Result = result;
	pMsg.iMoney = money;
	pMsg.wMoney = wmoney;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGWarehouseUseEnd(int aIndex) 
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_DEFAULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x82, sizeof(pMsg));
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	gObjItemTextSave(lpObj);
	GDSetWarehouseList(aIndex);
	GDUserItemSave(lpObj);

	if ( lpObj->m_IfState.use  && lpObj->m_IfState.type == 6 )
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.state = 0;
		lpObj->WarehouseSave = FALSE;
	}
}

struct PMSG_WAREHOUSESTATE
{
	PBMSG_HEAD h;
	BYTE State;
};

void GCWarehouseStateSend(int aIndex, BYTE state) 
{
	PMSG_WAREHOUSESTATE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x83, sizeof(pMsg));
	pMsg.State = state;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCWarehouseRecivePassword(int aIndex, PMSG_WAREHOUSEPASSSEND* lpMsg) 
{
	if ( bCanWarehouseLock == FALSE )
	{
		return;
	}

	int pw = lpMsg->Pass;

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( gObj[aIndex].WarehouseLock )
			{
				if ( gObj[aIndex].WarehousePW == pw )
				{
					gObj[aIndex].WarehouseLock = FALSE;
					GCWarehouseStateSend(aIndex, 0x0C);
				}
				else
				{
					GCWarehouseStateSend(aIndex, 0x0A);
				}
			}
			break;
		case 0x01:
			if ( !gObj[aIndex].WarehouseLock || gObj[aIndex].WarehouseLock == 0xFF )
			{
				char szJoomin[11];

				memset(szJoomin, 0, sizeof(szJoomin));
				memcpy(szJoomin, lpMsg->LastJoominNumber, 10);

				//if ( gObjJoominCheck(aIndex, szJoomin) == FALSE )
				//{
				//	GCWarehouseStateSend(aIndex, 0x0D);
				//	return;
				//}

				gObj[aIndex].WarehousePW = pw;
				gObj[aIndex].WarehouseLock = FALSE;
				GCWarehouseStateSend(aIndex, 0x0C);
			}
			else
			{
				GCWarehouseStateSend(aIndex, 0x0B);
			}
			break;
		case 0x02:
			{
				char szJoomin[11];

				memset(szJoomin, 0, sizeof(szJoomin));
				memcpy(szJoomin, lpMsg->LastJoominNumber, 10);

				//if ( gObjJoominCheck(aIndex, szJoomin) == FALSE )
				//{
				//	GCWarehouseStateSend(aIndex, 0x0D);
				//	return;
				//}

				gObj[aIndex].WarehouseLock = FALSE;
				gObj[aIndex].WarehousePW = 0;
				GCWarehouseStateSend(aIndex, gObj[aIndex].WarehouseLock);
			}
			break;
	}
}	

void GCUserChaosBoxSend(LPOBJ lpObj, int iChaosBoxType) 
{
	if ( iChaosBoxType == 1  )
	{
		if ( lpObj->m_IfState.type != 13 )
		{
			return;
		}

		if ( lpObj->m_IfState.type == 13 && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}
	else 
	{
		if ( lpObj->m_IfState.type != 7 )
		{
			return;
		}

		if ( lpObj->m_IfState.type == 7 && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}

	PMSG_SHOPITEMCOUNT pMsg;
	BYTE SendByte[2048];
	int lOfs = sizeof(pMsg);
	PHeadSetW((LPBYTE)&pMsg, 0x31, 0);
	pMsg.count = 0;

	if ( iChaosBoxType == 1 )
	{
		pMsg.Type = 5;
	}
	else
	{
		pMsg.Type = 3;
	}

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			SendByte[lOfs] = n;
			lOfs++;
			ItemByteConvert(&SendByte[lOfs], lpObj->pChaosBox[n] );
			lOfs += MAX_ITEM_INFO;
			pMsg.count ++;
		}
	}

	pMsg.h.sizeH = SET_NUMBERH(lOfs);
	pMsg.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(SendByte, &pMsg, sizeof(pMsg));

	DataSend(lpObj->m_Index, SendByte, lOfs);
}

//00465770	-> OK
void CGChaosBoxItemMixButtonClick(PMSG_CHAOSMIX* aRecv, int aIndex) 
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->ChaosLock == TRUE )
	{
		LogAdd("[%s][%s] Already Used Chaos", lpObj->AccountID, lpObj->Name);
		GCServerMsgStringSend(lMsg.Get(MSGGET(2, 198)), lpObj->m_Index, 1);
		return;
	}

	if ( lpObj->m_bPShopOpen == true )
	{
		LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);
		return;
	}

	//season4 add-on start
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if( aRecv->Type == 3 ||
		aRecv->Type == 4 ||
		aRecv->Type == 22 ||
		aRecv->Type == 23 )
	{
		int pos = 0;
		int validitemcount = 0;

		for ( int n=0;n<CHAOS_BOX_SIZE;n++) //loc8
		{
			if ( lpObj->pChaosBox[n].IsItem() == TRUE )
			{
				if( lpObj->pChaosBox[n].m_Level == 9)
				{
					pos = n;
					validitemcount++;
				}
				else if( lpObj->pChaosBox[n].m_Level == 10)
				{
					pos = n;
					validitemcount++;
				}
				else if( lpObj->pChaosBox[n].m_Level == 11)
				{
					pos = n;
					validitemcount++;
				}
				else if( lpObj->pChaosBox[n].m_Level == 12)
				{
					pos = n;
					validitemcount++;
				}
				else if( lpObj->pChaosBox[n].m_Level == 13) //new
				{
					pos = n;
					validitemcount++;
				}
				else if( lpObj->pChaosBox[n].m_Level == 14)
				{
					pos = n;
					validitemcount++;
				}
			}
		}

		if(validitemcount != 1)
		{
			pMsg.Result = 0;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		int iItemWidth = 0;
		int iItemHeight = 0;

		lpObj->pChaosBox[pos].GetSize(iItemWidth, iItemHeight);

		if(CheckInventoryEmptySpace(lpObj,iItemHeight,iItemWidth) != 0)//CheckInventoryEmptySpace_
		{
			
		}
		else
		{
			pMsg.Result = 0;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}
	}
	//season4 add-on end

	lpObj->bIsChaosMixCompleted = true;
	int iMixType = aRecv->Type;

#ifdef __CUSTOMS__
	g_MixOption.GetOption(lpObj, iMixType);
#endif

	switch(iMixType )
	{
		case CHAOS_TYPE_DEVILSQUARE:
			g_MixSystem.DevilSquareItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_UPGRADE_10:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_10);
			break;
		case CHAOS_TYPE_UPGRADE_11:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_11);
			break;
		case CHAOS_TYPE_UPGRADE_12:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_12);
			break;
		case CHAOS_TYPE_UPGRADE_13:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_13);
			break;
		case CHAOS_TYPE_UPGRADE_14:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_14);
			break;
		case CHAOS_TYPE_UPGRADE_15:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_15);
			break;
		case CHAOS_TYPE_DINORANT:
			g_MixSystem.PegasiaChaosMix(lpObj);
			break;
		case CHAOS_TYPE_FRUIT:
			g_MixSystem.CircleChaosMix(lpObj);
			break;
		case CHAOS_TYPE_SECOND_WING:
		case CHAOS_TYPE_CLOAK:
			g_MixSystem.WingChaosMix(lpObj);
			break;
		case CHAOS_TYPE_BLOODCATLE:
			g_MixSystem.BloodCastleItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_DEFAULT:
		case CHAOS_TYPE_FIRST_WING:
			g_MixSystem.DefaultChaosMix(lpObj);
			break;
		//case CHAOS_TYPE_SETITEM:
			//g_MixSystem.SetItemChaosMix(lpObj);
			//break;
		case CHAOS_TYPE_DARKHORSE:
			g_MixSystem.DarkHorseChaosMix(lpObj);
			break;
		case CHAOS_TYPE_DARKSPIRIT:
			g_MixSystem.DarkSpiritChaosMix(lpObj);
			break;
		case CHAOS_TYPE_BLESS_POTION:
			g_MixSystem.BlessPotionChaosMix(lpObj);
			break;
		case CHAOS_TYPE_SOUL_POTION:
			g_MixSystem.SoulPotionChaosMix(lpObj);
			break;
		case CHAOS_TYPE_LIFE_STONE:
			g_MixSystem.LifeStoneChaosMix(lpObj);
			break;
		case CHAOS_TYPE_CASTLE_ITEM:
			g_MixSystem.CastleSpecialItemMix(lpObj);
			break;
		case CHAOS_TYPE_HT_BOX:
			g_MixSystem.HiddenTreasureBoxItemMix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_01:
			g_MixSystem.Fenrir_01Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_02:
			g_MixSystem.Fenrir_02Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_03:
			g_MixSystem.Fenrir_03Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_04:
			g_MixSystem.Fenrir_04Upgrade_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNDPOTION_LV1:
			g_MixSystem.ShieldPotionLv1_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV2:
			g_MixSystem.ShieldPotionLv2_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV3:
			g_MixSystem.ShieldPotionLv3_Mix(lpObj);
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_PURITY:
			g_kJewelOfHarmonySystem.PurityJewelOfHarmony(lpObj);
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM:
			g_kJewelOfHarmonySystem.MakeSmeltingStoneItem(lpObj);
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM:
			g_kJewelOfHarmonySystem.RestoreStrengthenItem(lpObj);
			break;
		case CHAOS_TYPE_380_OPTIONITEM:
			g_kItemSystemFor380.ChaosMix380ItemOption(lpObj);
			break;
		case CHAOS_TYPE_LOTTERY_MIX:
			g_MixSystem.ChaosCardMix(lpObj);
			break;
		case CHAOS_TYPE_ILLUSION_TEMPLE_ITEM:
			g_IllusionTempleEvent.IllusionTempleChaosMixItem(lpObj);
			break;
		case CHAOS_TYPE_THIRD_WING_1:
			g_MixSystem.ThirdWingLevel1_Mix(lpObj);
			break;
		case CHAOS_TYPE_THIRD_WING_2:
			g_MixSystem.ThirdWingLevel2_Mix(lpObj);
			break;
		case CHAOS_TYPE_CHERRY_MIX:
			g_MixSystem.CherryBlossomMix(lpObj);
			break;
		case CHAOS_TYPE_SEED_EXTRACT:
			g_MixSystem.MixExtractSeed(lpObj);
			break;
		case CHAOS_TYPE_SEED_COMPOSIT:
			g_MixSystem.MixCompositeSeedSphere(lpObj);
			break;
		case CHAOS_TYPE_SEED_EQUIP:
			g_MixSystem.MixSetSeedSphere(lpObj, aRecv->SocketSlot);
			break;
		case CHAOS_TYPE_SEED_REMOVE:
			g_MixSystem.MixRemoveSeedSphere(lpObj, aRecv->SocketSlot);
			break;
		case CHAOS_TYPE_DUNGEON_KEYITEM:	//-> 1.01.00 start
			g_MixSystem.EventDungeonKeyItemMix(lpObj);
			break;
		case CHAOS_TYPE_GOLD_AND_SILBER_BOX:
			g_MixSystem.GoldenNSilverBoxMix(lpObj);
			break;
		case CHAOS_TYPE_LUCKY_ITEM_EXCHANGE:
			g_LuckyItemManager.LuckyItemTicketExchange(lpObj);
			break;
		case CHAOS_TYPE_LUCKY_ITEM_SMELTING:
			g_LuckyItemManager.LuckyItemSmelting(lpObj);
			break;
#ifdef NEWWINGS
		//case CHAOS_TYPE_NEWWINGS_PART1_MIX:
		//	g_MixSystem.NewWingsMix_Part1(lpObj);
		//	break;
#endif
			// ----
		default:
			LogAddTD("[%s][%s] Undefine chaosmix type detect %d", lpObj->AccountID, lpObj->Name, iMixType);
			break;
	}
}

void CGChaosBoxUseEnd(int aIndex) 
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_DEFAULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x87, sizeof(pMsg));
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( lpObj->m_IfState.use && lpObj->m_IfState.type == 7 )
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.state = 0;
	}

	g_MixSystem.ChaosBoxInit(lpObj);
	gObjInventoryCommit(lpObj->m_Index);
	lpObj->m_bIsCastleNPCUpgradeCompleted = false;
}

struct PMSG_ACTIONRESULT
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE Dir;
	BYTE ActionNumber;
	BYTE TargetNumberH;
	BYTE TargetNumberL;
};

void PMoveProc(PMSG_MOVE* lpMove, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		LogAdd("error-L3 : move protocol index error %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_RECVMOVE pMove;

	short n, pathtable;
	short ax, ay;
	int   sx, sy;

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->RegenOk > 0 )
	{
		return;
	}

	if ( lpObj->SkillRecallParty_Time )
	{
		lpObj->SkillRecallParty_Time = 0;
		char msg[255];
		wsprintf(msg, lMsg.Get(MSGGET(4, 230)));
		GCServerMsgStringSend(msg, lpObj->m_Index, 1);
	}

	if ( (GetTickCount()-lpObj->m_LastMoveTime) < 100 )
	{
		return;
	}

	if ( lpObj->Teleport )
	{
		return;
	}

	if(gObjCheckUsedBuffEffect(lpObj, AT_ICE_ARROW) == 1 
		|| gObjCheckUsedBuffEffect(lpObj, AT_STUN) == 1 
		|| gObjCheckUsedBuffEffect(lpObj, AT_SLEEP) == 1 )
	{
		return;
	}

	lpObj->m_LastMoveTime = GetTickCount();

	lpObj->m_Rest = 0;
	lpObj->PathCur = 0;

	lpObj->Dir = lpMove->Path[0] >> 4;
	lpObj->PathCount = lpMove->Path[0] & 0x0F;

	if ( lpObj->PathCount > 15 )
	{
		LogAdd("error-L3 : Path Count error %d id:%s %s %d", lpObj->PathCount, lpObj->AccountID, __FILE__, __LINE__);
		return;
	}

	for ( n=0;n<15;n++)
	{
		lpObj->PathX[n]=0;
		lpObj->PathY[n]=0;
		lpObj->PathOri[n]=0;
	}

	sx = lpMove->X;
	sy = lpMove->Y;

#ifdef DP
	if( g_DoppleganerEvent.IsEventMap(lpObj->MapNumber) )
	{
		if( g_DoppleganerEvent.GetDoppelgangerState() == 1 &&
			!g_DoppleganerEvent.CheckMapTile(lpObj->MapNumber, lpObj->X, lpObj->Y) )
		{
			gObjSetPosition(aIndex, lpObj->X, lpObj->Y);
            return;
		}
	}
#endif

	if ( gObjCheckXYMapTile(lpObj, 1) == TRUE )
	{
		lpObj->PathCount = 0;
		lpObj->PathCur = 0;
		lpObj->PathStartEnd = 0;
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}

	ax = lpObj->PathX[0] = lpMove->X;
	ay = lpObj->PathY[0] = lpMove->Y;

	lpObj->PathDir[0] = lpObj->Dir;

	lpObj->PathStartEnd = 1;

	if ( lpObj->PathCount > 0 )
	{
		lpObj->PathCur = 1;
		lpObj->PathCount +=1;
	}

	for (n=1;n<lpObj->PathCount;n++)
	{
		if ( (n%2) == 1 )
		{
			pathtable = lpMove->Path[(n+1) / 2] >> 4;
		}
		else
		{
			pathtable = lpMove->Path[(n+1) / 2] & 0x0F;
		}

		ax += RoadPathTable[pathtable*2];
		ay += RoadPathTable[pathtable*2+1];
		
		lpObj->PathOri[n-1] = pathtable;
		lpObj->PathDir[n]= pathtable;
		lpObj->PathX[n] = ax;
		lpObj->PathY[n] = ay;
	}

	if ( lpObj->PathCount > 0 )
	{
		int nextX;
		int nextY;
		BYTE mapnumber;
		BYTE attr;
		nextX = lpObj->PathX[1];
		nextY = lpObj->PathY[1];
		mapnumber = lpObj->MapNumber;

		if ( lpObj->PathCount > 0 ) //HermeX Add-on for fixing registers
		{
			//logadd
		}

		attr = MapC[mapnumber].GetAttr(nextX, nextY);

		if ( ((attr & (BYTE)4) == (BYTE)4) || ((attr & (BYTE)8) == (BYTE)8) )
		{
			for ( n=0 ; n<15 ; n++)
			{
				lpObj->PathX[n] = 0;
				lpObj->PathY[n] = 0;
				lpObj->PathOri[n] = 0;
			}

			lpObj->PathCount = 0;
			lpObj->PathCur = 0;
			lpObj->PathStartEnd = 0;
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			return;
		}
	}

	lpObj->TX = (BYTE)ax;
	lpObj->TY = (BYTE)ay;

#if(_GSCS==1)
	if( lpObj->Type == OBJ_USER )
	{
		int nextX = lpObj->X;
		int nextY = lpObj->Y;
		int iResult = 0; //should be BOOL
		WORD attr = 0;
		
		for(int i = 0; i < 256;i++)
		{	
			if(nextX > lpObj->TX)
			{
				nextX -= 1;
			}

			if(nextX < lpObj->TX)
			{
				nextX += 1;
			}

			if(nextY > lpObj->TY)
			{
				nextY -= 1;
			}

			if(nextY < lpObj->TY)
			{
				nextY += 1;
			}

			attr = MapC[lpObj->MapNumber].GetAttr(nextX,nextY);

			if ((attr&16) == 16 )
			{
				iResult = 1;
				break;
			}

			if( nextX == lpObj->TX)
			{
				if(nextY == lpObj->TY)
				{
					break;
				}
			}
		}

		if(iResult == 1)
		{
			lpObj->m_Rest = 1;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
			lpObj->PathStartEnd = 0;

			gObjSetPosition(lpObj->m_Index,lpObj->X,lpObj->Y);
			
			PMSG_ACTIONRESULT pActionResult;

			PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
			pActionResult.NumberH = SET_NUMBERH(aIndex);
			pActionResult.NumberL = SET_NUMBERL(aIndex);
			pActionResult.ActionNumber = 0x7A;
			pActionResult.Dir = lpObj->Dir;
			pActionResult.TargetNumberH = 0;
			pActionResult.TargetNumberL = 0;

			DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
			return;	
		}
	}
#endif

	if ( BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Type == OBJ_USER )
	{
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(lpObj->MapNumber);

		switch ( g_BloodCastle.GetCurrentState(iBridgeIndex) )
		{
			case 0x01:
				if ( lpObj->TY > 15 )
				{
					lpObj->m_Rest = 1;
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = 0;
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

					PMSG_ACTIONRESULT pActionResult;

					PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
					pActionResult.NumberH = SET_NUMBERH(aIndex);
					pActionResult.NumberL = SET_NUMBERL(aIndex);
					pActionResult.ActionNumber = 0x7A;
					pActionResult.Dir = lpObj->Dir;
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;

					DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
					return;
				}
				break;
			case 0x02:
				if ( lpObj->Y > 17 && g_BloodCastle.CheckPlayStart(iBridgeIndex) == false)
				{
					gObjMoveGate(lpObj->m_Index, iBridgeIndex+66);
					return;
				}

				if ( lpObj->TY > 15 && g_BloodCastle.CheckPlayStart(iBridgeIndex) == false )
				{
					lpObj->m_Rest = 1;
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = 0;
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

					PMSG_ACTIONRESULT pActionResult;

					PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
					pActionResult.NumberH = SET_NUMBERH(aIndex);
					pActionResult.NumberL = SET_NUMBERL(aIndex);
					pActionResult.ActionNumber = 0x7A;
					pActionResult.Dir = lpObj->Dir;
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;

					DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
					return;
				}

				if ( lpObj->TY > 76 &&  g_BloodCastle.m_BridgeData[iBridgeIndex].m_bCASTLE_DOOR_LIVE )
				{
					lpObj->m_Rest = 1;
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = 0;
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

					PMSG_ACTIONRESULT pActionResult;

					PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
					pActionResult.NumberH = SET_NUMBERH(aIndex);
					pActionResult.NumberL = SET_NUMBERL(aIndex);
					pActionResult.ActionNumber = 0x7A;
					pActionResult.Dir = lpObj->Dir;
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;

					DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
					return;
				}
				break;
		}
	}

	PHeadSetB((LPBYTE)&pMove, PROTOCOL_MOVE , sizeof(pMove));
	pMove.NumberH = SET_NUMBERH(aIndex);
	pMove.NumberL = SET_NUMBERL(aIndex);
	pMove.X				= (BYTE)ax;
	pMove.Y				= (BYTE)ay;
	pMove.Path = lpObj->Dir << 4;

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_IfState.use )
		{
			if ( lpObj->m_IfState.type == 3 )
			{
				lpObj->TargetShopNumber = -1;
				lpObj->m_IfState.type = 0;
				lpObj->m_IfState.use = 0;
			}
		}

		if ( !gObjPositionCheck(lpObj))
		{
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
			ax = lpObj->X;
			ay = lpObj->Y;
			lpObj->TX = (BYTE)ax;
			lpObj->TY = (BYTE)ay;
			pMove.X			= (BYTE)ax;
			pMove.Y	  		= (BYTE)ay;
		}

		DataSend(aIndex, (LPBYTE)&pMove, pMove.h.size);
	}

	int MVL = MAX_VIEWPORT;

	if ( lpObj->Type == OBJ_MONSTER )
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}
		
	for (n=0;n<MVL;n++)
	{
		if ( lpObj->VpPlayer2[n].state == TRUE )
		{
			int number = lpObj->VpPlayer2[n].number;
			
			if ( number >= 0 )
			{
				if ( gObj[number].Connected > PLAYER_CONNECTED && gObj[number].Live != false)
				{
					if(gObj[number].Type == OBJ_USER)
					{
						DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pMove, pMove.h.size);
					}
				}
				else
				{
					lpObj->VpPlayer2[n].number= -1;
					lpObj->VpPlayer2[n].state = 0;
					lpObj->VPCount2--;
				}
			}
		}
	}

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);

	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

	lpObj->X = sx;
	lpObj->Y = sy;

	lpObj->m_ViewState = 0;
}

void RecvPositionSetProc(PMSG_POSISTION_SET * lpMove, int aIndex) 
{
	short n;

	if (  OBJMAX_RANGE(aIndex) == FALSE)
	{
		LogAdd("error : move protocol index error %s %d", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( ::PacketCheckTime(lpObj) == FALSE )
	{
		return;
	}

	if ( lpObj->Teleport != 0 )
	{
		return;
	}

	lpObj->X = lpMove->X;
	lpObj->Y = lpMove->Y;

	if ( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		if ( (GetTickCount() - lpObj->m_iChaosCastleBlowTime ) < 1000 )
		{
			return;
		}
	}

	if ( gObjCheckXYMapTile(lpObj, 2) == TRUE)
	{
		return;
	}

	PMSG_RECV_POSISTION_SET pMove;

	PHeadSetB((LPBYTE)&pMove, PROTOCOL_POSITION, sizeof(pMove));
	pMove.NumberH = SET_NUMBERH(aIndex);
	pMove.NumberL = SET_NUMBERL(aIndex);
	pMove.X = lpMove->X;
	pMove.Y = lpMove->Y;
	lpObj->TX = lpMove->X;
	lpObj->TY = lpMove->Y;

	if ( ::gObjPositionCheck(lpObj) == FALSE )
	{
		return;
	}

	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(aIndex, (LPBYTE)&pMove, pMove.h.size);
	}

	int MVL = MAX_VIEWPORT;

	if ( lpObj->Type == OBJ_MONSTER )
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for ( n=0;n<MVL;n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state == 1 )
			{
				int Index = lpObj->VpPlayer2[n].number;

				if ( gObj[Index].Connected > PLAYER_CONNECTED && gObj[Index].Live != FALSE)
				{
					DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pMove, pMove.h.size);
				}
				else
				{
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VpPlayer2[n].state = FALSE;
					lpObj->VPCount2--;
				}
			}
		}
	}

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);

	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

}

void CGAttack(PMSG_ATTACK* lpMsg, int aIndex) 
{
	LPOBJ lpObj;
	LPOBJ lpTargetObj;
	int usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( usernumber < 0 || usernumber > OBJMAX-1)
	{
		LogAdd("[CGAttack] [UserIndex Error] :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}

	if ( !gObj[usernumber].Live )
	{
		return;
	}

	lpObj = &gObj[aIndex];
	int iTimeCalc = GetTickCount() - lpObj->m_LastAttackTime;

	if ( iTimeCalc < lpObj->m_DetectSpeedHackTime) //season4 changed (fix)
	{
		lpObj->m_DetectCount++;
		lpObj->m_SumLastAttackTime += iTimeCalc;

		if ( lpObj->m_DetectCount > gHackCheckCount )
		{
			lpObj->m_DetectedHackKickCount++;
			lpObj->m_SpeedHackPenalty = gSpeedHackPenalty;
			
			if ( gIsKickDetecHackCountLimit )
			{
				if ( lpObj->m_DetectedHackKickCount > gDetectedHackKickCount )
				{
					LogAddTD("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 108)+lpObj->Class), lpObj->m_DetectedHackKickCount);
					CloseClient(aIndex);
					return;
				}
			}

			LogAddTD("[%s][%s] %s Attack Speed Is Wrong Normal (%d)(%d) Penalty %d", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 108)+lpObj->Class), lpObj->m_DetectSpeedHackTime, lpObj->m_SumLastAttackTime/lpObj->m_DetectCount, lpObj->m_SpeedHackPenalty);
		}

		lpObj->m_LastAttackTime = GetTickCount();
	}
	else
	{
		lpObj->m_SumLastAttackTime = 0;
		lpObj->m_DetectCount = 0;
	}

	lpObj->m_LastAttackTime = GetTickCount();

	if ( bIsIgnorePacketSpeedHackDetect )
	{
		if ( lpObj->m_SpeedHackPenalty > 0 )
		{
			lpObj->m_SpeedHackPenalty--;
			
			LogAddTD("[%s][%s] %s Apply Attack Speed Penalty (%d left)", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 108)+lpObj->Class), lpObj->m_SpeedHackPenalty);
			return;
		}
	}
	
	lpTargetObj = &gObj[usernumber];
	lpObj->Dir = lpMsg->DirDis;
	GCActionSend(lpObj, lpMsg->AttackAction, aIndex, usernumber);
	gComboAttack.CheckCombo(aIndex, 0);
	gObjAttack(lpObj, lpTargetObj, NULL, FALSE, FALSE, 0, FALSE, 0, 0);
	lpObj->UseMagicNumber = 0;
}

void GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage, int iShieldDamage) 
{
	PMSG_ATTACKRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, PROTOCOL_ATTACK, sizeof(pResult));
	pResult.NumberH = SET_NUMBERH(TargetIndex);
	pResult.NumberL = SET_NUMBERL(TargetIndex);
	pResult.DamageH = SET_NUMBERH(AttackDamage);
	pResult.DamageL = SET_NUMBERL(AttackDamage);
	pResult.btShieldDamageH = SET_NUMBERH(iShieldDamage);
	pResult.btShieldDamageL = SET_NUMBERL(iShieldDamage);
	pResult.Life = gObj[TargetIndex].Life;	//A
	pResult.MaxLife= gObj[TargetIndex].MaxLife + gObj[TargetIndex].AddLife;	//E
	pResult.Class = gObj[TargetIndex].Class;

	pResult.AttackHP = AttackDamage;
	pResult.AttackSD = iShieldDamage;

	if ( MSBFlag != FALSE )
	{
		pResult.NumberH &= 0x7F;
		pResult.NumberH |= 0x80;
	}

	pResult.DamageType = MSBDamage;

	if ( gObj[TargetIndex].Type == OBJ_USER )
	{

		DataSend(TargetIndex, (LPBYTE)&pResult, pResult.h.size);
	}

	if ( cManager.WatchTargetIndex == TargetIndex || cManager.WatchTargetIndex == aIndex )
	{
		cManager.DataSend((LPBYTE)&pResult, pResult.h.size);
	}

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}

}

struct PMSG_DAMAGE
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE DamageH;
	BYTE DamageL;
	BYTE btShieldDamageH;
	BYTE btShieldDamageL;
};

void GCDamageSend(int aIndex, int damage, int iShieldDamage) 
{
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	PMSG_DAMAGE pDamage;

	PHeadSubSetB((LPBYTE)&pDamage, 0xF3, 0x07, sizeof(pDamage));
	pDamage.DamageH = SET_NUMBERH(damage);
	pDamage.DamageL = SET_NUMBERL(damage);
	pDamage.btShieldDamageH = SET_NUMBERH(iShieldDamage);
	pDamage.btShieldDamageL = SET_NUMBERL(iShieldDamage);

	DataSend(aIndex, (LPBYTE)&pDamage, pDamage.h.size);
}

struct PMSG_KILLPLAYER
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE ExpH;
	BYTE ExpL;
	BYTE DamageH;
	BYTE DamageL;
};

#ifdef EXPERIENCE_EXTERN
void GCKillPlayerExpSend(int aIndex, int TargetIndex, __int64 exp, int AttackDamage, BOOL MSBFlag)
{
	return GCSendExp_INT64(aIndex, TargetIndex, exp, AttackDamage, MSBFlag);
#else
void GCKillPlayerExpSend(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag) 
{
#endif
	PMSG_KILLPLAYER pkillMsg;

	PHeadSetBE((LPBYTE)&pkillMsg, 0x16, sizeof(pkillMsg));
	pkillMsg.NumberH = SET_NUMBERH(TargetIndex);
	pkillMsg.NumberL = SET_NUMBERL(TargetIndex);
	pkillMsg.ExpH = SET_NUMBERH(exp);
	pkillMsg.ExpL = SET_NUMBERL(exp);
	pkillMsg.DamageH = SET_NUMBERH(AttackDamage);
	pkillMsg.DamageL = SET_NUMBERL(AttackDamage);

	if ( MSBFlag != FALSE )
	{
		pkillMsg.NumberH &= 0x7F;
		pkillMsg.NumberH |= 0x80;
	}

	if (  BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
	{
		g_BloodCastle.AddExperience(aIndex, exp);
	}

	DataSend(aIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);
}

void GCSendExp_INT64(int aIndex, int TargetIndex, __int64 exp, int AttackDamage, int MSBFlag)
{
	PMSG_KILLPLAYER_EXT pMsg;

	PHeadSetBE((LPBYTE)&pMsg, 0x9C, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(TargetIndex);
	pMsg.NumberL = SET_NUMBERL(TargetIndex);
	pMsg.ExpH = SET_NUMBERHW(exp);
	pMsg.ExpL = SET_NUMBERLW(exp);
	pMsg.DamageH = SET_NUMBERH(AttackDamage);
	pMsg.DamageL = SET_NUMBERL(AttackDamage);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

#pragma pack(push, 1)
struct PMSG_DIEPLAYER
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE SkillH;
	BYTE SkillL;
	BYTE KillerNumberH;
	BYTE KillerNumberL;
};
#pragma pack(pop)

void GCDiePlayerSend(LPOBJ lpObj, int TargetIndex, BYTE skill, int KillerIndex)
{
	PMSG_DIEPLAYER pDieMsg;

	PHeadSetB((LPBYTE)&pDieMsg, 0x17, sizeof(pDieMsg));
	pDieMsg.NumberH = SET_NUMBERH(TargetIndex);
	pDieMsg.NumberL = SET_NUMBERL(TargetIndex);

	pDieMsg.SkillH = SET_NUMBERH(skill);
	pDieMsg.SkillL = SET_NUMBERL(skill);

	pDieMsg.KillerNumberH = SET_NUMBERH(KillerIndex);
	pDieMsg.KillerNumberL = SET_NUMBERL(KillerIndex);

	int iSize = pDieMsg.h.size;

	MsgSendV2(lpObj, (LPBYTE)&pDieMsg, iSize);

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pDieMsg, iSize);
	}
}

void GCActionSend(LPOBJ lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex) 
{
	PMSG_ACTIONRESULT pActionResult;

	PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
	pActionResult.NumberH = SET_NUMBERH(aIndex);
	pActionResult.NumberL = SET_NUMBERL(aIndex);
	pActionResult.ActionNumber = ActionNumber;
	pActionResult.Dir = lpObj->Dir;
	pActionResult.TargetNumberH = SET_NUMBERH(aTargetIndex);
	pActionResult.TargetNumberL = SET_NUMBERL(aTargetIndex);

	MsgSendV2(lpObj, (LPBYTE)&pActionResult, pActionResult.h.size);
}

//Revised to 1.01.03
void CGActionRecv(PMSG_ACTION * lpMsg, int aIndex) 
{
	PMSG_ACTIONRESULT pResult;
	// ----
	if( !OBJMAX_RANGE(aIndex))
	{
		LogAdd("error : move protocol index error %s %d", __FILE__, __LINE__);
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[aIndex];
	PHeadSetB((LPBYTE)&pResult, 0x18, sizeof(pResult));
	// ----
	pResult.ActionNumber	= lpMsg->ActionNumber;
	pResult.NumberH			= SET_NUMBERH(aIndex);
	pResult.NumberL			= SET_NUMBERL(aIndex);
	lpObj->m_ActionNumber	= lpMsg->ActionNumber;
	lpObj->Dir				= lpMsg->Dir;
	pResult.Dir				= lpObj->Dir;
	pResult.TargetNumberH	= lpMsg->iTargetIndexH;
	pResult.TargetNumberL	= lpMsg->iTargetIndexL;
	// ----
	if( lpObj->m_ActionNumber == 0x80 )
	{
		lpObj->m_ViewState	= 2;
		lpObj->m_Rest		= lpObj->m_ActionNumber;
	}
	else if( lpObj->m_ActionNumber == 0x81 )
	{
		lpObj->m_ViewState	= 3;
		lpObj->m_Rest		= lpObj->m_ActionNumber;
	}
	else if( lpObj->m_ActionNumber == 0x82 )
	{
		lpObj->m_ViewState	= 4;
		lpObj->m_Rest		= lpObj->m_ActionNumber;
	}
	// ----
	int MVL = MAX_VIEWPORT;
	// ----
	if( lpObj->Type == OBJ_MONSTER )
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}
	// ----
	for( int n = 0; n < MVL; n++ )
	{
		if( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if(		lpObj->VpPlayer2[n].state != 0
				&&	lpObj->VpPlayer2[n].state != 0x10
				&&	lpObj->VpPlayer2[n].state != 0x08
				&&	gObj[lpObj->VpPlayer2[n].number].Connected == PLAYER_PLAYING )	//1.01.03
			{
				DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
}

void CGMagicAttack(PMSG_MAGICATTACK* lpMsg, int aIndex)
{
	LPOBJ lpObj;
	LPOBJ lpTargetObj;

	int usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL); //loc3
	CMagicInf * lpMagic; //loc4
	WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL); //loc5

	if ( usernumber < 0 || usernumber > OBJMAX-1 )
	{
		LogAdd("error :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}

	if ( gObj[usernumber].Live == 0 || gObj[usernumber].Connected  < PLAYER_LOGGED )
	{
		return;
	}

	if ( gObj[aIndex].CloseCount >= 0 )
	{
		return;
	}

	//season 4.5 add-on start
	BYTE attr = MapC[gObj[aIndex].MapNumber].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);

	if ( (attr&1) == 1 && MagicNumber == 58) //safezone
	{
		GCMagicAttackNumberSend(&gObj[aIndex], 40, aIndex, 1);
		gObjSetPosition(aIndex,gObj[aIndex].X, gObj[aIndex].Y);
		return;
	}
	//season 4.5 add-on end

	lpObj = &gObj[aIndex];
	lpTargetObj = &gObj[usernumber];
	
	int attackret = gObjCheckAttackArea(aIndex, usernumber);

	if ( attackret )
	{
		//LogAddC(2, "[DEBUG] Point 2");
		//LogAddTD("[%s][%s] Try Attack In Not Attack Area [Protocol] (%s:%d,%d) errortype = %d",	lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 208)+lpObj->MapNumber), lpObj->X, lpObj->Y, attackret);

		if ( lpObj->Class == 275 )
		{
			lpObj->TargetNumber = -1;
		}

		if ( bIsIgnorePacketSpeedHackDetect )
		{
			return;
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( MagicNumber == 58 )
		{
			usernumber = 58;
		}

		lpMagic = gObjGetMagicSearch(lpObj, MagicNumber);

#if( FIX_MAXSTAT == 1 )
		WORD Energy = lpObj->Energy+ lpObj->AddEnergy;
		WORD Strength = lpObj->Strength + lpObj->AddStrength;
		WORD Dexterity = lpObj->Dexterity + lpObj->AddDexterity;
#else
		short Energy = lpObj->Energy+ lpObj->AddEnergy;
		short Strength = lpObj->Strength + lpObj->AddStrength;
		short Dexterity = lpObj->Dexterity + lpObj->AddDexterity;
#endif

		switch(MagicNumber) //season 3.5 add-on
		{
		case AT_SKILL_ELFHARDEN:
			{
				if(Dexterity < 646)
				{
					return;
				}
			}
			break;
		}

		if ( lpMagic == NULL )
		{
			return;
		}

		if ( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpObj->ThirdChangeUp, lpMagic->m_Skill) < 1 )
		{
			return;
		}

#ifdef NPVP
		if( lpMagic->m_Skill == 63 && (g_NewPVP.IsDuel(*lpObj) || g_NewPVP.IsDuel(*lpTargetObj)) )
		{
			return;
		}
#endif

		if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == 0 )
		{
			LogAddC(2, "[0x19] CGMagicAttack() - Invalid Status");
			return;
		}

		if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			LogAddC(2, "[0x19] CGMagicAttack() - Invalid KillCount");
			return;
		}
	}
	else
	{
		lpMagic = gObjGetMagic(lpObj, MagicNumber);

		if ( lpMagic == NULL )
		{
			return;
		}
	}

	if ( lpMagic->m_Skill != 24 && lpMagic->m_Skill != 414 && lpMagic->m_Skill != 418 && gObjUseSkill.SpeedHackCheck(aIndex) == FALSE )
	{
		return;
	}

	if ( !gCheckSkillDistance(aIndex, usernumber, lpMagic->m_Skill))
	{
		DWORD dwTick = GetTickCount() - lpObj->m_dwSkillDistanceErrorTick;
		
		if ( dwTick > (g_iSkillDiatanceKickCheckTime*1000) )
		{
			lpObj->m_iSkillDistanceErrorCount = 0;
			lpObj->m_dwSkillDistanceErrorTick = GetTickCount();
		}

		lpObj->m_iSkillDistanceErrorCount++;

		if ( lpObj->m_iSkillDistanceErrorCount > g_iSkillDistanceKickCount )
		{
			if ( g_iSkillDistanceKick )
			{
				LogAddTD("[SKILL DISTANCE CHECK] [%s][%s] Kick Invalid Skill Area User. count(%d)", lpObj->AccountID, lpObj->Name, lpObj->m_iSkillDistanceErrorCount);
				CloseClient(lpObj->m_Index);
				return;
			}

			lpObj->m_iSkillDistanceErrorCount = 0;
		}
		return;
	}
	
	gObjUseSkill.UseSkill(aIndex, usernumber, lpMagic);
}

void GCMagicAttackNumberSend(LPOBJ lpObj, WORD MagicNumber, int usernumber,  BYTE skillsuccess)
{
	PMSG_MAGICATTACK_RESULT pAttack;
	
	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
	pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
	pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);
	pAttack.SourceNumberH = SET_NUMBERH(lpObj->m_Index);
	pAttack.SourceNumberL = SET_NUMBERL(lpObj->m_Index);
	pAttack.TargetNumberH = SET_NUMBERH(usernumber);
	pAttack.TargetNumberL = SET_NUMBERL(usernumber);
	pAttack.TargetNumberH &= 0x7F;

	if ( skillsuccess )
	{
		pAttack.TargetNumberH |= 0x80;
	}

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);
	}

	if ( CC_MAP_RANGE(lpObj->MapNumber) )
	{
		if(g_MasterSkillSystem.CheckMasterLevelSkill(MagicNumber) != FALSE) //Season3 add-on
		{
			int GetSkillID = g_MasterSkillSystem.GetBaseMasterLevelSkill(MagicNumber);

			switch ( GetSkillID )
			{
				case 435:
				case 470:
				case 475:
				//Season4 add-on
				case 480:
				case 485:
					return;
			}
		}
		else
		{
			switch ( MagicNumber )
			{
				case 16:
				case 26:
				case 27:
				case 28:
				case 48:
					return;
			}
		}
	}
	
	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);
}

void GCMagicCancelSend(LPOBJ lpObj, WORD MagicNumber)
{
	PMSG_MAGICCANCEL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x1B, sizeof(pMsg));
	pMsg.MagicNumberH = SET_NUMBERH(MagicNumber);
	pMsg.MagicNumberL = SET_NUMBERL(MagicNumber);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
	
	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCUseEffectItem(LPOBJ lpObj, int iEffectUseOption, int iOptionType, int iEffectType, int iLeftTime)
{
	PMSG_USEEFFECTITEM pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2D, sizeof(pMsg));
	pMsg.byEffectOption = iEffectUseOption;
	pMsg.wOptionType = iOptionType;
	pMsg.wEffectType = iEffectType;
	pMsg.iLeftTime = iLeftTime;
	pMsg.flag = 0;

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void CGMagicCancel(PMSG_MAGICCANCEL * lpMsg, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);

	if ( MagicNumber == 77 )
	{
		if ( gObjCheckUsedBuffEffect(lpObj, AT_INFINITY_ARROW) == 1 ) //S3 Addition -> Infinity Arrow
		{
			gObjRemoveBuffEffect(lpObj, AT_INFINITY_ARROW); //S3 Addition -> Infinity Arrow
		}
	}
	else if ( MagicNumber == 233 ) //season4 add-on
	{
		if ( gObjCheckUsedBuffEffect(lpObj, AT_EXPANSION_WIZARDRY) == 1 )
		{
			gObjRemoveBuffEffect(lpObj, AT_EXPANSION_WIZARDRY);
		}
	}
}

struct PMSG_USE_MONSTERSKILL
{
	PBMSG_HEAD h;
	BYTE btMonsterSkillNumberH;
	BYTE btMonsterSkillNumberL;
	WORD wObjIndex;
	WORD wTargetObjIndex;
};

void GCUseMonsterSkillSend(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkillNumber) 
{	
	PMSG_USE_MONSTERSKILL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x69, sizeof(pMsg));
	pMsg.btMonsterSkillNumberH = SET_NUMBERH(iSkillNumber);
	pMsg.btMonsterSkillNumberL = SET_NUMBERL(iSkillNumber);
	pMsg.wObjIndex = lpObj->m_Index;
	pMsg.wTargetObjIndex = lpTargetObj->m_Index;

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_VIEWSKILLSTATE
{
	PBMSG_HEAD h;
	BYTE State;
	BYTE NumberH;
	BYTE NumberL;
	BYTE BuffEffect;
};

void GCBuffStateInfoSend(LPOBJ lpObj, BYTE state, BYTE btEffectIndex)
{
	PMSG_VIEWSKILLSTATE pMsg;

	BYTE loc3 = 0;

	PHeadSetB((LPBYTE)&pMsg, 0x07, sizeof(pMsg));

	pMsg.State		= state;

	pMsg.NumberH	= SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL	= SET_NUMBERL(lpObj->m_Index);

	pMsg.BuffEffect = btEffectIndex;

	if( lpObj->Type == OBJ_USER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	if( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		return;
	}

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGTeleportRecv(PMSG_TELEPORT* lpMsg, int aIndex) 
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if(gObjCheckUsedBuffEffect(&gObj[aIndex], AT_STUN) == 1 || gObjCheckUsedBuffEffect(&gObj[aIndex], AT_SLEEP) == 1) //Season3 update
	{
		PMSG_TELEPORT_RESULT pTeleportResult;

		pTeleportResult.h.c = 0xC3;
		pTeleportResult.h.size = sizeof(pTeleportResult);
		pTeleportResult.h.headcode = 0x1C;
		pTeleportResult.MoveNumber = 0;
		pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
		pTeleportResult.MapX = gObj[aIndex].X;
		pTeleportResult.MapY = gObj[aIndex].Y;
		pTeleportResult.Dir = gObj[aIndex].Dir;
		DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
		return;
	}

	if(IT_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE) //season 2.5 add-on
	{
		if(g_IllusionTempleEvent.GetIllusionTempleState(gObj[aIndex].MapNumber) == 2)
		{
			if(g_IllusionTempleEvent.CheckRelics(aIndex) != FALSE)
			{
				PMSG_TELEPORT_RESULT pTeleportResult;

				pTeleportResult.h.c = 0xC3;
				pTeleportResult.h.size = sizeof(pTeleportResult);
				pTeleportResult.h.headcode = 0x1C;
				pTeleportResult.MoveNumber = 0;
				pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
				pTeleportResult.MapX = gObj[aIndex].X;
				pTeleportResult.MapY = gObj[aIndex].Y;
				pTeleportResult.Dir = gObj[aIndex].Dir;
				DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
				return;
			}
			if(g_IllusionTempleEvent.CheckSkillRestraint(gObj[aIndex].m_iIllusionTempleIndex,gObj[aIndex].MapNumber) != FALSE)
			{
				PMSG_TELEPORT_RESULT pTeleportResult;

				pTeleportResult.h.c = 0xC3;
				pTeleportResult.h.size = sizeof(pTeleportResult);
				pTeleportResult.h.headcode = 0x1C;
				pTeleportResult.MoveNumber = 0;
				pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
				pTeleportResult.MapX = gObj[aIndex].X;
				pTeleportResult.MapY = gObj[aIndex].Y;
				pTeleportResult.Dir = gObj[aIndex].Dir;
				DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
				return;
			}
		}
	}

	if(IT_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE) //season 2.5 add-on
	{
		if(g_IllusionTempleEvent.GetIllusionTempleState(gObj[aIndex].MapNumber) == 1)
		{
			PMSG_TELEPORT_RESULT pTeleportResult;

			pTeleportResult.h.c = 0xC3;
			pTeleportResult.h.size = sizeof(pTeleportResult);
			pTeleportResult.h.headcode = 0x1C;
			pTeleportResult.MoveNumber = 0;
			pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
			pTeleportResult.MapX = gObj[aIndex].X;
			pTeleportResult.MapY = gObj[aIndex].Y;
			pTeleportResult.Dir = gObj[aIndex].Dir;
			DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}
	}

	if(CC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE) //season 4.0 add-on (fix)
	{
		if(g_ChaosCastle.GetCurrentState(gObj[aIndex].m_cChaosCastleIndex) == 3)
		{
			PMSG_TELEPORT_RESULT pTeleportResult;

			pTeleportResult.h.c = 0xC3;
			pTeleportResult.h.size = sizeof(pTeleportResult);
			pTeleportResult.h.headcode = 0x1C;
			pTeleportResult.MoveNumber = 0;
			pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
			pTeleportResult.MapX = gObj[aIndex].X;
			pTeleportResult.MapY = gObj[aIndex].Y;
			pTeleportResult.Dir = gObj[aIndex].Dir;
			DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}
	}
	
#ifdef DP
	if( g_DoppleganerEvent.IsEventMap(gObj[aIndex].MapNumber) )
	{
		if( g_DoppleganerEvent.GetDoppelgangerState() != 2 )
		{
			PMSG_TELEPORT_RESULT pTeleportResult;
			pTeleportResult.h.c = 0xC3;
			pTeleportResult.h.size = sizeof(pTeleportResult);
			pTeleportResult.h.headcode = 0x1C;
			pTeleportResult.MoveNumber = 0;
			pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
			pTeleportResult.MapX = gObj[aIndex].X;
			pTeleportResult.MapY = gObj[aIndex].Y;
			pTeleportResult.Dir = gObj[aIndex].Dir;
			DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}
	}
#endif

#ifdef IMPERIAL
	if( lpMsg->MoveNumber )
	{
		if ( CImperialGuardian::IsEventMap(gObj[aIndex].MapNumber) )
		{
			int nZoneIndex = g_ImperialGuardian.GetCurrentZoneIndex(aIndex);

			if ( nZoneIndex >= 0 )
				g_ImperialGuardian.CGEnterPortal(aIndex, nZoneIndex + 1);

			return;
		}
	}
#endif

	if ( lpMsg->MoveNumber == 0 )
	{
		CMagicInf * lpMagic;
		BYTE x;
		BYTE y;
		PMSG_MAGICATTACK_RESULT pAttack;

		PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
		pAttack.MagicNumberH = SET_NUMBERH(6);
		pAttack.MagicNumberL = SET_NUMBERL(6);
		pAttack.SourceNumberH = SET_NUMBERH(aIndex);
		pAttack.SourceNumberL = SET_NUMBERL(aIndex);
		pAttack.TargetNumberH = SET_NUMBERH(aIndex);
		pAttack.TargetNumberL = SET_NUMBERL(aIndex);

		lpMagic = gObjGetMagicSearch(&gObj[aIndex], 6);
		x = lpMsg->MapX;
		y = lpMsg->MapY;

		if ( gObjCheckTeleportArea(aIndex, x, y) == FALSE )
		{
			LogAddC(2, "[%s][%s] Try Teleport Not Move Area [%d,%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

			if ( szAuthKey[5] != AUTHKEY5 )
			{
				DestroyGIocp();
			}

			PMSG_TELEPORT_RESULT pTeleportResult;

			pTeleportResult.h.c = 0xC3;
			pTeleportResult.h.size = sizeof(pTeleportResult);
			pTeleportResult.h.headcode = 0x1C;
			pTeleportResult.MoveNumber = 0;
			pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
			pTeleportResult.MapX = gObj[aIndex].X;
			pTeleportResult.MapY = gObj[aIndex].Y;
			pTeleportResult.Dir = gObj[aIndex].Dir;

			DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}

//#if(_GSCS==1)
		if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE )
		{
			if( g_CastleSiege.CheckTeleportMagicAxisY(gObj[aIndex].Y,x,y) == FALSE )
			{
				y = gObj[aIndex].Y;
			}
		}
//#endif

		if ( lpMagic )
		{
			int usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);

			if ( usemana >= 0 )
			{
				int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
				
				if ( usebp >= 0 )
				{
					if ( gObj[aIndex].Type == OBJ_USER )
					{
						DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);
					}

					MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
					gObjTeleportMagicUse(aIndex, x, y);
					gObj[aIndex].Mana = usemana;
					gObj[aIndex].BP = usebp;
					GCManaSend(gObj[aIndex].m_Index, gObj[aIndex].Mana, -1, 0, gObj[aIndex].BP);
				}
			}
		}
	}
	else if ( gGateC.IsInGate(aIndex, lpMsg->MoveNumber) )
	{
		gObjMoveGate(aIndex, lpMsg->MoveNumber);
	}
	else
	{
		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex], lpMsg->MoveNumber, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);

//#if(_GSCS==1)
		if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				g_CastleSiege.NotifySelfCsJoinSide(aIndex);
				g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
			}
		}
//#endif
	}
}

void CGTargetTeleportRecv(PMSG_TARGET_TELEPORT * lpMsg, int aIndex) 
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if(gObjCheckUsedBuffEffect(&gObj[aIndex], AT_STUN) == 1)
	{
		return;
	}

	if(gObjCheckUsedBuffEffect(&gObj[aIndex], AT_SLEEP) == 1)
	{
		return;
	}

	CMagicInf * lpMagic;
	BYTE x;
	BYTE y;
	PMSG_MAGICATTACK_RESULT pAttack;

	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
	pAttack.MagicNumberH = SET_NUMBERH(AT_SKILL_TARGET_TELEPORT);
	pAttack.MagicNumberL = SET_NUMBERL(AT_SKILL_TARGET_TELEPORT);
	pAttack.SourceNumberH = SET_NUMBERH(aIndex);
	pAttack.SourceNumberL = SET_NUMBERL(aIndex);
	int iTargetIndex = MAKE_NUMBERW(lpMsg->NumberL, lpMsg->NumberH);
	pAttack.TargetNumberH = SET_NUMBERH(iTargetIndex);
	pAttack.TargetNumberL = SET_NUMBERL(iTargetIndex);
	lpMagic = gObjGetMagicSearch(&gObj[aIndex], AT_SKILL_TARGET_TELEPORT);
	x = lpMsg->MapX;
	y = lpMsg->MapY;

	if ( !gObjIsConnectedGP(iTargetIndex) )
	{
		LogAddC(2, "[%s][%s] Try Target Teleport Not Move Area [%d,%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);
		return;
	}

	if(IT_MAP_RANGE(gObj[iTargetIndex].MapNumber) != FALSE) //season 2.5 add-on
	{
		if(g_IllusionTempleEvent.CheckRelics(gObj[iTargetIndex].m_Index) != FALSE)
		{
			return;
		}
	}

	if ( gObj[aIndex].PartyNumber != gObj[iTargetIndex].PartyNumber || gObj[aIndex].PartyNumber == -1 || gObj[iTargetIndex].PartyNumber == -1)
	{
		return;
	}

	if ( !gObjCheckTeleportArea(iTargetIndex, x, y))
	{
		LogAddC(2, "[%s][%s] Try Target Teleport Not Move Area [%d,%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);
		return;
	}

	if ( lpMagic )
	{
		int usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);

		if ( usemana >= 0 )
		{
			int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
			
			if ( usebp >= 0 )
			{
				if ( gObj[aIndex].Type == OBJ_USER )
				{
					DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);
				}

				MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
				gObjTeleportMagicUse(iTargetIndex, x, y);
				gObj[aIndex].Mana = usemana;
				gObj[aIndex].BP = usebp;
				GCManaSend(gObj[aIndex].m_Index, gObj[aIndex].Mana, -1, 0, gObj[aIndex].BP);
				return;
			}
		}
	}
}

void GCTeleportSend(LPOBJ lpObj, int MoveNumber, BYTE MapNumber, BYTE MapX, BYTE MapY, BYTE Dir) 
{
	PMSG_TELEPORT_RESULT pMsg;

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if(MoveNumber > 0) //Season 2.5 add-on
	{
		MoveNumber = 1;
	}

	pMsg.h.c = 0xC3;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0x1C;
	pMsg.MoveNumber = MoveNumber;
	pMsg.MapNumber = MapNumber;
	pMsg.MapX = MapX;
	pMsg.MapY = MapY;
	pMsg.Dir = Dir;

	if ( MoveNumber == 0 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
	else
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void CGBeattackRecv(BYTE* lpRecv, int aIndex, int magic_send) 
{
	PMSG_BEATTACK_COUNT * lpCount = (PMSG_BEATTACK_COUNT *)lpRecv;

	if ( lpCount->h.headcode != PROTOCOL_BEATTACK )
	{
		LogAdd("error-L3 %s %d [0x%x]", 
			__FILE__, 
			__LINE__,
			lpCount->h.headcode);
		return;
	}

	if ( lpCount->Count < 1 )
	{
		LogAdd("error-L3 %s %d", 
			__FILE__, 
			__LINE__);
		return;
	}

	if ( lpCount->Count > 5 )
	{
		lpCount->Count = 5;
	}

	int lOfs = sizeof(PMSG_BEATTACK_COUNT);

	CMagicInf * lpMagic;

	int tNumber;

	PMSG_BEATTACK * lpMsg;

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		BYTE attr = MapC[gObj[aIndex].MapNumber].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);

		if ( (attr&1) == 1 )
		{
			gObjSetPosition(aIndex,gObj[aIndex].X, gObj[aIndex].Y);
			return;
		}
		WORD MagicNumber = MAKE_NUMBERW(lpCount->MagicNumberH, lpCount->MagicNumberL);
		lpMagic = gObjGetMagicSearch(&gObj[aIndex], MagicNumber);
	}
	else
	{
		WORD MagicNumber = MAKE_NUMBERW(lpCount->MagicNumberH, lpCount->MagicNumberL);
		lpMagic = gObjGetMagic(&gObj[aIndex], MagicNumber);
	}

	if( lpMagic == NULL )
	{
		//LogAdd("error-L3 %s %d [N: %d]", 
		//	__FILE__, 
		//	__LINE__, MAKE_NUMBERW(lpCount->MagicNumberH, lpCount->MagicNumberL));
		return;
	}

	if ( lpMagic->m_Skill == 42 || 
		lpMagic->m_Skill == 43 || 
		g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 336 || //s4
		g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 331 || //s4
		lpMagic->m_Skill == 41 || 
		lpMagic->m_Skill == 333 ||
		lpMagic->m_Skill == 332 ||
		lpMagic->m_Skill == 330 )	//1.01.00
	{
		return;
	}

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		if ( MagicDamageC.SkillGetRequireClass(
			gObj[aIndex].Class, 
			gObj[aIndex].ChangeUP, 
			gObj[aIndex].ThirdChangeUp, 
			lpMagic->m_Skill) < 1 )
		{
			LogAddC(2, lMsg.Get(MSGGET(1, 239)), 
				gObj[aIndex].AccountID, 
				gObj[aIndex].Name, 
				lpMagic->m_Skill);
			gObjUserKill(aIndex);
			return;
		}

		if ( (GetTickCount()- gObj[aIndex].UseMagicTime) > 8000 )
		{
			LogAddC(2, "Too long time passed after casting magic. [%s][%s] (%d)(%d)", 
				gObj[aIndex].AccountID, 
				gObj[aIndex].Name, 
				lpMagic->m_Skill, 
				GetTickCount() - gObj[aIndex].UseMagicTime);
			return;
		}

		if ( gObj[aIndex].UseMagicCount > 4 )
		{
			return;
		}

		gObj[aIndex].UseMagicCount++;
	}

	if( g_MasterSkillSystem.CheckMasterLevelSkill(lpMagic->m_Skill) )
	{
		int iBaseMLS = g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill);
		// ----
		if(	   iBaseMLS != 403
			&& iBaseMLS != 388
			&& iBaseMLS != 330
			&& iBaseMLS != 413
			&& iBaseMLS != 414
			&& iBaseMLS != 481
			&& iBaseMLS != 356
			&& iBaseMLS != 382
			&& iBaseMLS != 512
			&& iBaseMLS != 508
			&& iBaseMLS != 385
			&& iBaseMLS != 391
			&& iBaseMLS != 336
			&& iBaseMLS != 331
			&& iBaseMLS != 417
			&& iBaseMLS != 420
			&& iBaseMLS != 490
			&& iBaseMLS != 482
			&& iBaseMLS != 518
			&& iBaseMLS != 487
			&& iBaseMLS != 326
			&& iBaseMLS != 327
			&& iBaseMLS != 328
			&& iBaseMLS != 329
			&& iBaseMLS != 332
			&& iBaseMLS != 333
			&& iBaseMLS != 337
			&& iBaseMLS != 339
			&& iBaseMLS != 340
			&& iBaseMLS != 342
			&& iBaseMLS != 343
			&& iBaseMLS != 344
			&& iBaseMLS != 346
			&& iBaseMLS != 360
			&& iBaseMLS != 363
			&& iBaseMLS != 376
			&& iBaseMLS != 378
			&& iBaseMLS != 379
			&& iBaseMLS != 380
			&& iBaseMLS != 381
			&& iBaseMLS != 383
			&& iBaseMLS != 384
			&& iBaseMLS != 386
			&& iBaseMLS != 387
			&& iBaseMLS != 389
			&& iBaseMLS != 390
			&& iBaseMLS != 392
			&& iBaseMLS != 393
			&& iBaseMLS != 394
			&& iBaseMLS != 395
			&& iBaseMLS != 396
			&& iBaseMLS != 404
			&& iBaseMLS != 406
			&& iBaseMLS != 411
			&& iBaseMLS != 415
			&& iBaseMLS != 416
			&& iBaseMLS != 418
			&& iBaseMLS != 419
			&& iBaseMLS != 425
			&& iBaseMLS != 426
			&& iBaseMLS != 427
			&& iBaseMLS != 428
			&& iBaseMLS != 429
			&& iBaseMLS != 430
			&& iBaseMLS != 431
			&& iBaseMLS != 432
			&& iBaseMLS != 433
			&& iBaseMLS != 434
			&& iBaseMLS != 441
			&& iBaseMLS != 459
			&& iBaseMLS != 460
			&& iBaseMLS != 461
			&& iBaseMLS != 462
			&& iBaseMLS != 466
			&& iBaseMLS != 468
			&& iBaseMLS != 469
			&& iBaseMLS != 470
			&& iBaseMLS != 472
			&& iBaseMLS != 477
			&& iBaseMLS != 479
			&& iBaseMLS != 480
			&& iBaseMLS != 483
			&& iBaseMLS != 484
			&& iBaseMLS != 486
			&& iBaseMLS != 488
			&& iBaseMLS != 489
			&& iBaseMLS != 491
			&& iBaseMLS != 492
			&& iBaseMLS != 493
			&& iBaseMLS != 494
			&& iBaseMLS != 495
			&& iBaseMLS != 497
			&& iBaseMLS != 498
			&& iBaseMLS != 499
			&& iBaseMLS != 500
			&& iBaseMLS != 501
			&& iBaseMLS != 502
			&& iBaseMLS != 509
			&& iBaseMLS != 510
			&& iBaseMLS != 514
			&& iBaseMLS != 515
			&& iBaseMLS != 516
			&& iBaseMLS != 519
			&& iBaseMLS != 520
			&& iBaseMLS != 521
			&& iBaseMLS != 523
			&& iBaseMLS != 539
			&& iBaseMLS != 551
			&& iBaseMLS != 552
			&& iBaseMLS != 554
			&& iBaseMLS != 555
			&& iBaseMLS != 556
			&& iBaseMLS != 558
			&& iBaseMLS != 559
			&& iBaseMLS != 560
			&& iBaseMLS != 561
			&& iBaseMLS != 562
			&& iBaseMLS != 563
			&& iBaseMLS != 564
			&& iBaseMLS != 565
			&& iBaseMLS != 566
			&& iBaseMLS != 567
			&& iBaseMLS != 575
			&& iBaseMLS != 577 )
		{
			LogAdd("error-L3 %s %d", 
				__FILE__, 
				__LINE__);
			return;
		}
	}
	else if (	 lpMagic->m_Skill != 8
			  && lpMagic->m_Skill != 9
			  && lpMagic->m_Skill != 10
			  && lpMagic->m_Skill != 13
			  && lpMagic->m_Skill != 14
			  && lpMagic->m_Skill != 5
			  && lpMagic->m_Skill != 24
			  && lpMagic->m_Skill != 50
			  && lpMagic->m_Skill != 12
			  && lpMagic->m_Skill != 41
			  && lpMagic->m_Skill != 47
			  && lpMagic->m_Skill != 43
			  && lpMagic->m_Skill != 42
			  && lpMagic->m_Skill != 52
			  && lpMagic->m_Skill != 55
			  && lpMagic->m_Skill != 78 )
		{
			LogAdd("error-L3 %s %d", 
				__FILE__, 
				__LINE__);
			return;
		}

	if ( lpMagic->m_Skill == 24 || 
		lpMagic->m_Skill == 78 || 
		lpMagic->m_Skill == 418 || 
		g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 414 ||
		g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 518)
	{
		int NSAttackSerial = lpCount->Serial;

		if ( gObj[aIndex].OSAttackSerial >= 255 && (gObj[aIndex].OSAttackSerial - lpCount->Serial) > 50 )
		{
			gObj[aIndex].OSAttackSerial = -1;
		}
	
		if ( NSAttackSerial > gObj[aIndex].OSAttackSerial )
		{
			gObj[aIndex].OSAttackSerial = NSAttackSerial;
			LPOBJ lpObj = &gObj[aIndex];
			int iTimeCalc = GetTickCount() - lpObj->m_LastAttackTime;

			if ( iTimeCalc < lpObj->m_DetectSpeedHackTime && iTimeCalc )
			{
				lpObj->m_DetectCount++;
				lpObj->m_SumLastAttackTime += iTimeCalc;

				if ( lpObj->m_DetectCount > gHackCheckCount )
				{
					lpObj->m_DetectedHackKickCount++;

					if ( gIsKickDetecHackCountLimit )
					{
						if ( lpObj->m_DetectedHackKickCount > gDetectedHackKickCount )
						{
							LogAddTD("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)", 
								lpObj->AccountID, 
								lpObj->Name, 
								lMsg.Get(MSGGET(7, 108)+lpObj->Class), 
								lpObj->m_DetectedHackKickCount);
							CloseClient(aIndex);
							return;
						}
					}

					LogAddTD("[%s][%s] %s Attack Speed Is Wrong Magic3 (%d)(%d) Penalty %d", 
						lpObj->AccountID, 
						lpObj->Name, 
						lMsg.Get(MSGGET(7, 108)+lpObj->Class), 
						lpObj->m_DetectSpeedHackTime,
						lpObj->m_SumLastAttackTime/lpObj->m_DetectCount,
						lpObj->m_SpeedHackPenalty);
				}

				lpObj->m_LastAttackTime = GetTickCount();
			}
			else
			{
				lpObj->m_SumLastAttackTime = 0;
				lpObj->m_DetectCount = 0;
			}

			lpObj->m_LastAttackTime = GetTickCount();
		}
	}

	//season4 add-on
	if( lpMagic->m_Skill == 9 ||
		g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 385 ||	//1.01.00
		g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 487)
	{
		if(gObj[aIndex].Class != CLASS_WIZARD && gObj[aIndex].Class != CLASS_MAGUMSA)
		{
			return;
		}
	}
	if ( bIsIgnorePacketSpeedHackDetect )
	{
		LPOBJ lpObj = &gObj[aIndex];

		if ( lpObj->m_SpeedHackPenalty > 0 )
		{
			lpObj->m_SpeedHackPenalty--;
			LogAddTD("[%s][%s] %s Apply Attack Speed Penalty (%d left)", 
				lpObj->AccountID, 
				lpObj->Name, 
				lMsg.Get(MSGGET(7, 108)+lpObj->Class), 
				lpObj->m_SpeedHackPenalty);
			return;
		}
	}
	
	int lOfs2 = lOfs;
	int pTargetNumber[128];
	
	for (int i=0;i<lpCount->Count;i++)
	{
		lpMsg = (PMSG_BEATTACK *)&lpRecv[lOfs2];
		pTargetNumber[i] = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		lOfs2 += sizeof(PMSG_BEATTACK);
	}

	for (int n=0;n<lpCount->Count;n++)
	{
		lpMsg = (PMSG_BEATTACK *)&lpRecv[lOfs];
		tNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		LPOBJ lpObj = &gObj[aIndex];

		//season4 add-on
		if ( lpMsg->MagicKey == 0 && (lpMagic->m_Skill == 9 ||
		g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 385 ||
		g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 487))
		{
			return;
		}

		if ( lpMsg->MagicKey != 0 && (lpMagic->m_Skill == 9 ||
			//season4 add-on
			g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 485 ||
		g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 487))
		{
			if ( lpObj->DurMagicKeyChecker.IsValidDurationTime(lpMsg->MagicKey) == FALSE )
			{
				LogAddC(0, "★★★★ InValid DurationTime Key = %d ( Time : %d) [%d][%d]", 
					lpMsg->MagicKey, 
					lpObj->DurMagicKeyChecker.GetValidDurationTime(lpMsg->MagicKey),
				lpObj->AccountID, 
				lpObj->Name); 
				lOfs += sizeof(PMSG_BEATTACK);
				continue;
			}
			
			if ( lpObj->DurMagicKeyChecker.IsValidCount(lpMsg->MagicKey) == FALSE )
			{
				LogAddC(0, "★★★★ InValid VailidCount = %d ( Count : %d) [%d][%d]", 
					lpMsg->MagicKey, 
					lpObj->DurMagicKeyChecker.GetValidCount(lpMsg->MagicKey), 
					lpObj->AccountID, 
					lpObj->Name); 
				lOfs += sizeof(PMSG_BEATTACK);
				continue;
			}
		}
		
		 //season4 add-on start
		if(OBJMAX_RANGE(tNumber)==false)
		{
			continue;
		}

		int DistanceCheck = gObjCalDistance(lpObj, &gObj[tNumber]);

		if(DistanceCheck > 13)
		{
			lOfs += 3;
			continue;
		}
		//season4 add-on end

		if ( gWriteSkillLog )
		{
			LogAddTD("Magic Attack3 : %d, serial = %d, Tgt =  %d, cnt = %d", 
				lpMagic->m_Skill, 
				lpCount->Serial, 
				tNumber, 
				lpCount->Count);
		}

		if ( gEnableCheckPenetrationSkill )
		{
			if ( lpMagic->m_Skill == 78 )
			{
				if ( gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckFireScreamSkill(tNumber, lpMagic->m_Skill, lpCount->Serial) == FALSE )
				{
					return;
				}
			}
			else if ( gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckPenetrationSkill(tNumber, lpMagic->m_Skill, lpCount->Serial)== FALSE )
			{
				return;
			}
		}

		for (int i=0;i<lpCount->Count;i++)
		{
			if ( n != i)
			{
				if ( pTargetNumber[i] == tNumber )
				{
					LogAddTD("[%s][%s] %s Detect Hack : Multi Attack", 
						gObj[aIndex].AccountID, 
						gObj[aIndex].Name, 
						lMsg.Get(MSGGET(7, 108)+gObj[aIndex].Class));
					CloseClient(gObj[aIndex].m_Index);
					return;
				}
			}
		}

		if ( tNumber >= 0 && tNumber < OBJMAX-1 )
		{
			if ( lpMagic->m_Skill == 55 )
			{
				if(gObj[aIndex].Type == OBJ_USER) //season 3.5 add-on
				{
					if(gObj[aIndex].Strength+gObj[aIndex].AddStrength >= 596)
					{
						gObjUseSkill.MaGumSkillDefenseDown(aIndex, tNumber, lpMagic->m_Level);
						gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE, 0, 0);
					}
				}
				else
				{
					gObjUseSkill.MaGumSkillDefenseDown(aIndex, tNumber, lpMagic->m_Level);
					gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE, 0, 0);
				}
			}
			else if ( lpMagic->m_Skill == 78 )
			{
				gObjUseSkill.SkillFireScream(aIndex, tNumber, lpMagic);
			}
			else if ( g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 490 ) //season4 add-on
			{
				g_MasterSkillSystem.MLS_MaGumSkillDefenseDown(aIndex, tNumber);
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE, 0, 0);
			}
			else if ( g_MasterSkillSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 518 ) //season4 add-on
			{
				g_MasterSkillSystem.MLS_FireScream(aIndex, tNumber, lpMagic);
			}
			else if ( magic_send )
			{
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE, 0, 0);
			}
			else
			{
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, FALSE, 1, 0, FALSE, 0, 0);
			}
		}

		lOfs += sizeof(PMSG_BEATTACK);
	}
}

void CGDurationMagicRecv(PMSG_DURATION_MAGIC_RECV * lpMsg, int aIndex)
{
	CMagicInf * lpMagic;
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Teleport )
	{
		return;
	}

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		//season 4.5 add-on start
		BYTE attr = MapC[gObj[aIndex].MapNumber].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);

		if ( (attr&1) == 1 ) //safezone
		{
			gObjSetPosition(aIndex,gObj[aIndex].X, gObj[aIndex].Y);
			return;
		}
		//season 4.5 add-on end

		WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);
		lpMagic = gObjGetMagicSearch(lpObj, MagicNumber);
	}
	else
	{
		WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);
		lpMagic = gObjGetMagic( lpObj, MagicNumber);
	}

	if ( lpMagic == NULL )
	{
		return;
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == FALSE )
		{
			LogAddC(2, "[0x1E] CGDurationMagicRecv() - Invalid Status");
			return;
		}

		if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			LogAddC(2, "[0x1E] CGDurationMagicRecv() - Invalid KillCount");
			return;
		}
	}

	lpObj->UseMagicNumber = 0;

	if ( lpMagic->m_Skill != 24 && lpMagic->m_Skill != 414 && lpMagic->m_Skill != 418 )
	{
		if ( !gObjUseSkill.SpeedHackCheck(aIndex))
		{
			return;
		}
	}

	if ( !gObjUseSkill.EnableSkill(lpMagic->m_Skill, lpMagic->m_Level) )
	{
		return;
	}

	int aTargetIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	gObjUseSkill.UseSkill(aIndex, lpMagic,lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->TargetPos, aTargetIndex);
	
	if ( lpMsg->MagicKey )
	{
		lpObj->DurMagicKeyChecker.SetDurationTime(lpMsg->MagicKey, GetTickCount());
	}
}

void ObjectMapJoinPositionSend(short aIndex)
{
	PMSG_USERMAPJOIN pObject;
	
	PHeadSubSetB((LPBYTE)&pObject, 0xF3, 0x03, sizeof(pObject));
	pObject.NumberH = SET_NUMBERH(aIndex);
	pObject.NumberL = SET_NUMBERL(aIndex);
	pObject.X = gObj[aIndex].X;
	pObject.Y = gObj[aIndex].Y;
	pObject.MapNumber = gObj[aIndex].MapNumber;
	pObject.Dir = gObj[aIndex].Dir;

	DataSend(aIndex, (LPBYTE)&pObject, pObject.h.size);
}

//0046d550	->
void CGUseItemRecv(PMSG_USEITEM* lpMsg, int aIndex) //0045C690
{
	int pos;
	CItem * citem;
	int iItemUseType = lpMsg->btItemUseType;
	// ----
	if( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 3 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);
		LogAdd("[%s][%s] CGUseItemRecv()_If return %s %d",gObj[aIndex].AccountID,gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}
	// ----
	if( gObj[aIndex].CloseType != -1 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);
		LogAdd("[%s][%s] CGUseItemRecv()_CloseType return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}
	// ----
	if( gObjFixInventoryPointer(aIndex) == false )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d",	__FILE__, __LINE__);

	}
	// ----
	if( gObj[aIndex].pTransaction == 1 )
	{

		LogAddTD("[%s][%s] CGUseItemRecv() Failed : Transaction == 1, IF_TYPE : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;
	}
	// ----
#ifdef NPVP
	if( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3428), aIndex, 1);
		 return;
	}
#endif
	// ----
	pos = lpMsg->inventoryPos;
	// ----
	if( pos > MAIN_INVENTORY_SIZE - 1 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 241)), __FILE__, __LINE__);
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);
		LogAdd("[%s][%s] CGUseItemRecv()_Inventory return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}
	// ----
	if( lpMsg->inventoryPos == lpMsg->invenrotyTarget )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);
		LogAdd("error-L1 : [%s][%s] CGUseItemRecv()_Pos return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}
	// ----
	citem = &gObj[aIndex].pInventory[pos];
	// ----
#ifdef __ALIEN__
	if( citem->m_Type >= ITEMGET(14, 0) && citem->m_Type <= ITEMGET(14, 6) 
		|| citem->m_Type >= ITEMGET(14, 35) && citem->m_Type <= ITEMGET(14, 40) 
		|| citem->m_Type== ITEMGET(14, 70) || citem->m_Type == ITEMGET(14, 71) )
	{
		DWORD CurrentTick	= GetTickCount();
		DWORD Delay			= CurrentTick - gObj[aIndex].m_PotionTick;
		// ----
		if( Delay < g_PotionDelay )
		{
			char Text[100] = { 0 };
			sprintf(Text, "Delay: %d (%d)", Delay, g_PotionDelay);
			GCServerMsgStringSend(Text, aIndex, 1);
			return;
		}
	}
#endif
	if( citem->IsItem() )
	{
		if( gObjCheckSerial0ItemList(citem) )
		{
			MsgOutput(aIndex, lMsg.Get(MSGGET(13, 26)));
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);
			LogAddTD("[ANTI-HACK][Serial 0 Item] [UseItem] (%s)(%s) Item(%s) Pos(%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, citem->GetName(), pos);
			return;
		}
		// ----
		if( gObjInventorySearchSerialNumber(&gObj[aIndex], citem->GetNumber()) == FALSE )
		{
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);			
			LogAdd("error-L2 : CopyItem [%s][%s] return %s %d",	gObj[aIndex].AccountID,	gObj[aIndex].Name,	__FILE__, __LINE__);
			return;
		}
		// ----
		if( citem->m_serial && !gObjCanItemTouch(&gObj[aIndex], 1) )
		{
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			return;
		}
		// ----
		if( citem->m_Type == ITEMGET(14,0) || citem->m_Type == ITEMGET(14,1) || //ok
			citem->m_Type == ITEMGET(14,2) || citem->m_Type == ITEMGET(14,3) )
		{
			int tLife = (citem->m_Value * 10) - (gObj[aIndex].Level * 2);
			// ----
			if( tLife <  0 )
			{
				tLife = 0;
			}
			// ----
			int nAddRate = 0;
			// ----
			switch( citem->m_Type )
			{
				case ITEMGET(14,0):
					nAddRate = 10;
					break;
				case ITEMGET(14,1): 
					nAddRate = 20;
					break;
				case ITEMGET(14,2):
					nAddRate = 30;
					break;
				case ITEMGET(14,3): 
					nAddRate = 40;
					break;
			}
			// ----
			if( citem->m_Level == 1 )
			{
				nAddRate += 5;
			}
			// ----
			tLife += ((int)gObj[aIndex].MaxLife * nAddRate) / 100;
			// ----
			if( citem->m_Type == ITEMGET(14, 0) )
			{
				if( citem->m_Level < 2 )
				{
					gObj[aIndex].FillLife += tLife;
					tLife = 0;
				}
			}
			// ----
			if( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;
				// ----
				if( gObj[aIndex].Life > (gObj[aIndex].MaxLife +gObj[aIndex].AddLife) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
					gObj[aIndex].FillLife = 0;
				}
				// ----
				GCReFillSend(gObj[aIndex].m_Index, gObj[aIndex].Life, 0xFF, FALSE, gObj[aIndex].iShield);
			}
			// ----
			gObj[aIndex].FillLifeMax	= tLife;
			gObj[aIndex].FillLife		= tLife;
			// ----
			if( citem->m_Type == ITEMGET(14,0) && citem->m_Level < 2 )	//ok
			{
				gObj[aIndex].FillLifeCount = 0;
			}
			else if( citem->m_Level == 1 )
			{
				gObj[aIndex].FillLifeCount = 2;
			}
			else
			{
				gObj[aIndex].FillLifeCount = 3;
			}
			// ----
			if( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if( citem->m_Type == ITEMGET(14, 4) || citem->m_Type == ITEMGET(14, 5) || citem->m_Type == ITEMGET(14, 6) )
		{
			int tMana = (citem->m_Value*10) - (gObj[aIndex].Level);

			if ( tMana < 0 )
			{
				tMana=0;
			}

			switch ( citem->m_Type )
			{
				case ITEMGET(14,4):
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*20/100; 
					break;
				case ITEMGET(14,5):
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*30/100; 
					break;
				case ITEMGET(14,6):
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*40/100; 
					break;
			}

			gObj[aIndex].Mana += tMana;

			if ( gObj[aIndex].Mana > (gObj[aIndex].MaxMana+gObj[aIndex].AddMana-1.0f) )
			{
				gObj[aIndex].Mana = gObj[aIndex].MaxMana+gObj[aIndex].AddMana;
			}
			
			GCManaSend(aIndex, gObj[aIndex].Mana, 0xFF, 0, gObj[aIndex].BP);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,38) )	//ok
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 5/100;
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 10.0f / 100.0f );

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				}
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (  gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield )  )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			LogAddTD("[%s][%s]Use Compound Potion Lv1 - SD[%d] HP[%f] -> SD[%d] HP[%f]", 
				gObj[aIndex].AccountID, 
				gObj[aIndex].Name, 
				gObj[aIndex].iShield, 
				gObj[aIndex].Life, 
				gObj[aIndex].iShield+iShieldGage, 
				(double)(gObj[aIndex].Life+(float)iHPGage));
		}
		else if ( citem->m_Type == ITEMGET(14,39) ) //ok
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 10/100;
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 25.0f / 100.0f );

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				}
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (  gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield )  )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			LogAddTD("[%s][%s]Use Compound Potion Lv2 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield+iShieldGage, (double)(gObj[aIndex].Life+(float)iHPGage));
		}
		else if ( citem->m_Type == ITEMGET(14, 40) )	//ok
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 20/100;
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 45.0f / 100.0f );

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				}
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (  gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield )  )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			LogAddTD("[%s][%s]Use Compound Potion Lv3 - SD[%d] HP[%f] -> SD[%d] HP[%f]", 
				gObj[aIndex].AccountID, 
				gObj[aIndex].Name, 
				gObj[aIndex].iShield, 
				gObj[aIndex].Life, 
				gObj[aIndex].iShield+iShieldGage, 
				(double)(gObj[aIndex].Life+(float)iHPGage));
		}
		else if ( citem->m_Type == ITEMGET(14,35) )	//ok
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 25 / 100;

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,36) )	//ok
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 35 / 100;

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,37) )	//ok
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 45 / 100;

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,8) )	//ok
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			if(gObjCheckUsedBuffEffect(&gObj[aIndex], AT_POISON) == 1) //S3 Addition -> Poison
			{
				gObjRemoveBuffEffect(&gObj[aIndex], AT_POISON);
				gObj[aIndex].lpAttackObj = NULL;
				GCMagicCancelSend(&gObj[aIndex], 1);
			}
			else if(gObjCheckUsedBuffEffect(&gObj[aIndex], AT_ICE) == 1) //S3 Addition -> Ice
			{
				gObjRemoveBuffEffect(&gObj[aIndex], AT_ICE);
				gObj[aIndex].DelayActionTime = 0;
				gObj[aIndex].DelayLevel = 0;
				gObj[aIndex].lpAttackObj = NULL;
				GCMagicCancelSend(&gObj[aIndex], 7);
			}
		}
		else if ( citem->m_Type == ITEMGET(14, 46) )	//ok
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], 35);
		}
		else if ( citem->m_Type == ITEMGET(14,47) )	//ok
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], 36);
		}
		else if ( citem->m_Type == ITEMGET(14,48) )	//ok
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], 37);
		}
		else if ( citem->m_Type == ITEMGET(14,49) )	//ok
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], 38);
		}
		else if ( citem->m_Type == ITEMGET(14,50) )	//ok
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			gObjAddBuffEffect(&gObj[aIndex], 39);
		}
		else if ( citem->m_Type == ITEMGET(14,63) || citem->m_Type == ITEMGET(14,64) )
		{
			switch(citem->m_Type)
			{
			case ITEMGET(14,63):
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
					GCReFillSend(aIndex,gObj[aIndex].Life, -1, 0,gObj[aIndex].iShield);
				}
				break;
			case ITEMGET(14,64):
				{
					gObj[aIndex].Mana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
					GCManaSend(aIndex,gObj[aIndex].Mana, -1, 0, gObj[aIndex].BP);
				}
				break;
			}

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if( g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(citem->m_Type) == TRUE &&
#ifdef __ALIEN__
			(!g_HarmonyOptionOnSocket
			&& gItemSocketOption.IsSocketItem(&gObj[aIndex].pInventory[lpMsg->invenrotyTarget]) == FALSE) ) //season4 add-on
#else
			gItemSocketOption.IsSocketItem(&gObj[aIndex].pInventory[lpMsg->invenrotyTarget]) == FALSE ) //season4 add-on
#endif
		{
			if ( g_kJewelOfHarmonySystem.StrengthenItemByJewelOfHarmony(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems(citem->m_Type) == TRUE )
		{
			if ( g_kJewelOfHarmonySystem.SmeltItemBySmeltingStone(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,13) ) // Jewel Of Bless
		{
			if ( gObjItemLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
			//else	//1.01.00 update
			//{
			//	GCInventoryItemDeleteSend(aIndex, 0xFFu, 1);
			//}
		}
		// ----
		else if( citem->m_Type == ITEMGET(14, 160) )	//1.01.00 update
		{
			if( g_LuckyItemManager.LuckyItemRepaire(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if( citem->m_Type == ITEMGET(14, 161) 
#ifdef __ALIEN__
			&& (!g_HarmonyOptionOnSocket
			&& gItemSocketOption.IsSocketItem(&gObj[aIndex].pInventory[lpMsg->invenrotyTarget]) == FALSE) )
#else
			)	//1.01.00 update
#endif
		{
			if( g_kJewelOfHarmonySystem.StrengthenItemByJewelOfRise(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		// ----
		else if ( citem->m_Type == ITEMGET(14,14) ) // Jewel Of Soul
		{
			if ( gObjItemRandomLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,16) ) // Jewel Of Life
		{
			if ( gObjItemRandomOption3Up(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
#ifdef __CUSTOMS__
		else if( g_JewelsEx.IsJewelEx(citem->m_Type) )
		{
			g_JewelsEx.Upgrade(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget);
		}
#endif
		else if ( citem->m_Type >= ITEMGET(15,0) ||	citem->m_Type == ITEMGET(12,7) || 
			(citem->m_Type >= ITEMGET(12,8) && citem->m_Type <= ITEMGET(12,24) ) || 
			citem->m_Type == ITEMGET(12,35) ||	// Scroll of Fire Scream
			citem->m_Type == ITEMGET(12,44) ||
			citem->m_Type == ITEMGET(12,45) ||
			citem->m_Type == ITEMGET(12,46) ||
			citem->m_Type == ITEMGET(12,47) ||
			citem->m_Type == ITEMGET(12,48) ) //season4
		{
			if ( (gObj[aIndex].Strength + gObj[aIndex].AddStrength) < citem->m_RequireStrength )
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}
			
			if ( (gObj[aIndex].Dexterity + gObj[aIndex].AddDexterity) < citem->m_RequireDexterity )
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			if ( citem->m_Type == ITEMGET(15,18) )
			{
				if ( g_QuestInfo.GetQuestState(&gObj[aIndex], 2) != 2 )
				{
					GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
					return;
				}
			}

			if (citem->m_Type >= ITEMGET(12,8) && citem->m_Type <= ITEMGET(12,24) )
			{
				if ( gObj[aIndex].Level < citem->m_RequireLevel )
				{
					GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
					return;
				}
			}

			short skillnumber;

			if ( citem->IsClass(gObj[aIndex].Class, gObj[aIndex].ChangeUP, gObj[aIndex].ThirdChangeUp) == FALSE )
			{	
				GCInventoryItemDeleteSend(aIndex, -1, 1);
			}
			else
			{
				int addskill = gObjMagicAdd(&gObj[aIndex], citem->m_Type>>(9), citem->m_Type %  MAX_SUBTYPE_ITEMS, citem->m_Level, (short)skillnumber);

				if ( addskill >= 0 )
				{
					gObjInventoryItemSet(aIndex, pos, -1);
					GCMagicListOneSend(aIndex, addskill, skillnumber, citem->m_Level, 0, 0);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}
				else
				{
					GCInventoryItemDeleteSend(aIndex, -1, 1);
				}
			}
		}
		else if ( citem->m_Type == ITEMGET(14,10) )	//ok
		{
			LogAddTD("[Using Item] [Return Scroll] [%s][%s] - Current Map:[%d]", 
				gObj[aIndex].AccountID, 
				gObj[aIndex].Name, 
				gObj[aIndex].MapNumber);

			if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) )
			{
				int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(gObj[aIndex].MapNumber);

				if ( g_BloodCastle.GetCurrentState(iBridgeIndex) == 2 )
				{
					g_BloodCastle.SearchUserDropQuestItem(aIndex);
				}
				else
				{
					g_BloodCastle.SearchUserDeleteQuestItem(aIndex);
				}
			}

			if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) ) //Season2.5 add-on
			{
				g_IllusionTempleEvent.DropRelicsItem(gObj[aIndex].MapNumber, aIndex);
			}

			if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 3 )
			{
				gObj[aIndex].TargetShopNumber = -1;
				gObj[aIndex].m_IfState.type = 0;
				gObj[aIndex].m_IfState.use = 0;
			}

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);

			if ( gObj[aIndex].MapNumber == MAP_INDEX_DEVIAS )
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_NORIA )
			{
				gObjMoveGate(aIndex, 27);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_LOSTTOWER )
			{
				gObjMoveGate(aIndex, 42);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_ATHLANSE )
			{
				gObjMoveGate(aIndex, 49);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_TARKAN )
			{
				gObjMoveGate(aIndex, 57);
			}
			else if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( KALIMA_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_AIDA )
			{
				gObjMoveGate(aIndex, 27);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
			{
				gObjMoveGate(aIndex, 27);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_ELBELAND ) //season 3.0 add-on
			{
				gObjMoveGate(aIndex, 267);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS ) //season 3.0 add-on
			{
				gObjMoveGate(aIndex, 273);
			}
			else if( gObj[aIndex].MapNumber == MAP_INDEX_RAKLION_FIELD ) //season 4.0 add-on
			{
				gObjMoveGate(aIndex, 286);
			}
#ifdef NPVP
			else if( g_NewPVP.IsVulcanusMap(gObj[aIndex].MapNumber) )
			{
				gObjMoveGate(aIndex, 294);
			}
			else if( g_NewPVP.IsPKFieldMap(gObj[aIndex].MapNumber) )
			{
				g_NewPVP.Reset(gObj[aIndex]);
				gObjMoveGate(aIndex, 17);
			}
#endif

			else if ( gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE )
              {
                if ( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
                {
                  if ( gObj[aIndex].m_btCsJoinSide == 1 )
				  {
					gObjMoveGate(aIndex, 101);
				  }
                  else
				  {
					gObjMoveGate(aIndex, 100);
				  }
                }
                else
                {
                  if (g_CastleSiege.CheckCastleOwnerMember(aIndex) == TRUE || 
					  g_CastleSiege.CheckCastleOwnerUnionMember(aIndex) == TRUE )
				  {
                    gObjMoveGate(aIndex, 101);
				  }
                  else
				  {
                    gObjMoveGate(aIndex, 100);
				  }
                }
              }
//#endif
			else
			{
				gObjMoveGate(aIndex, 17);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,9) )	//ok
		{
			int level = citem->m_Level;

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gObjUseDrink(&gObj[aIndex], level);
		}
		else if ( citem->m_Type == ITEMGET(14,20) )//ok
		{
			if ( citem->m_Level == 0 )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				gObjUseDrink(&gObj[aIndex], 2);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,15) )	//ok
		{
			if ( iItemUseType == 0 )
			{
				gObjUsePlusStatFruit(aIndex, pos);
			}
			else if ( iItemUseType == 1 )
			{
				gObjUseMinusStatFruit(aIndex, pos);
			}
		}
		else if(citem->m_Type == ITEMGET(13,54) || 
				citem->m_Type == ITEMGET(13,55) || 
				citem->m_Type == ITEMGET(13,56) || 
				citem->m_Type == ITEMGET(13,57) || 
				citem->m_Type == ITEMGET(13,58) )
		{
			gObjUseCashShopStatFruit(&gObj[aIndex], pos);
		}
		else if(citem->m_Type == ITEMGET(14,78) || 
				citem->m_Type == ITEMGET(14,79) || 
				citem->m_Type == ITEMGET(14,80) || 
				citem->m_Type == ITEMGET(14,81) || 
				citem->m_Type == ITEMGET(14,82) )
		{
			gObjUseCashShopStatPotion(&gObj[aIndex], pos);
		}
		else if (citem->m_Type == ITEMGET(14,72) )
		{
			gObjUseIndulgence(&gObj[aIndex], pos);
		}
		else if(citem->m_Type == ITEMGET(14,70) || 
				citem->m_Type == ITEMGET(14,71) )
		{
			gObjUseFullRecoveryPotion(&gObj[aIndex], pos);
		}
		else if( citem->m_Type == ITEMGET(14,94) ) //season 3.5 add-on
		{
			gObjUseRecoveryPotion(&gObj[aIndex], pos, (double)0.65);
		}
		else if( citem->m_Type == ITEMGET(14, 133) )	//1.01.00
		{
			gObjUseRecoveryPotion(&gObj[aIndex], pos, (double)0.65);
		}
		else if( citem->m_Type == ITEMGET(14, 162) )	//1.01.00 update
		{
			GDUpdateExtendedInvenCount(&gObj[aIndex], 1, 0, lpMsg->inventoryPos, 0, 0);
		}
		else if( citem->m_Type == ITEMGET(14, 163) )	//1.01.00 update
		{
			GDUpdateExtendedWarehouseCount(&gObj[aIndex], 1, 0, lpMsg->inventoryPos, 0, 0);
		}
		else if(citem->m_Type == ITEMGET(14,85) || 
				citem->m_Type == ITEMGET(14,86) || 
				citem->m_Type == ITEMGET(14,87) ) //S3 Addition
		{
			int BuffEffectID = 0;

			if(citem->m_Type == ITEMGET(14,85))
			{
				BuffEffectID = 78;
			}
			else if(citem->m_Type == ITEMGET(14,86))
			{
				BuffEffectID = 79;
			}
			else if(citem->m_Type == ITEMGET(14,87))
			{
				BuffEffectID = 80;
			}

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BuffEffectID);
		}
		else if ( citem->m_Type == ITEMGET(14,7) )
		{
			int iLevel = citem->m_Level;
			int iSerial = citem->GetNumber();
			int iDur = citem->m_Durability;

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}	
			
			if ( gObj[aIndex].lpGuild )
			{
				LogAddTD("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s] - (Guild : %s)", 
					iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].lpGuild->Name);
			}
			else
			{
				LogAddTD("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s]", 
					iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name);
			}

			if ( iLevel == 0 )
			{
				gObjUseBlessAndSoulPotion(aIndex, iLevel);
			}
			else if ( iLevel == 1 )
			{
				gObjUseBlessAndSoulPotion(aIndex, iLevel);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,48) )
		{
			if ( (gObj[aIndex].MapNumber < MAP_INDEX_KALIMA1 || 
				gObj[aIndex].MapNumber > MAP_INDEX_KALIMA6 ) && 
				gObj[aIndex].MapNumber != MAP_INDEX_KALIMA7 )
			{
				int iLevel = g_KalimaGate.GetKalimaGateLevel2(aIndex);
				int iSerial = citem->GetNumber();
				int iDuration = citem->m_Durability;
				BOOL bKalimaGateCreateResult = FALSE;
				int iKalimaGateX = 0;
				int iKalimaGateY = 0;

				iLevel++;
				iKalimaGateX = gObj[aIndex].X + rand()%6 - 2;
				iKalimaGateY = gObj[aIndex].Y + rand()%6 - 2;

				bKalimaGateCreateResult = g_KalimaGate.CreateKalimaGate(aIndex, iLevel, iKalimaGateX, iKalimaGateY);

				if ( bKalimaGateCreateResult == TRUE )
				{
					if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
					{
						gObjInventoryItemSet(aIndex, pos, -1);
						gObj[aIndex].pInventory[pos].Clear();
						GCInventoryItemDeleteSend(aIndex, pos, 1);
					}

					LogAddTD("[PCS] Use Free Kalima Ticket Success");
				}
				else
				{
					GCItemDurSend(aIndex,pos,gObj[aIndex].pInventory[pos].m_Durability,1); //GS 19 Second Edition
					LogAddTD("[PCS] Use Free Kalima Ticket Failed");
				}
			}
		}
		else if ( citem->m_Type == ITEMGET(13,69) ) //season 4 add-on
		{
			BOOL bCheckEquipmentToMove = gMoveCommand.CheckEquipmentToMove(&gObj[aIndex],gObj[aIndex].m_wRaiseTalismanSaveMap);

			if( BC_MAP_RANGE(gObj[aIndex].MapNumber) == FALSE &&
				CC_MAP_RANGE(gObj[aIndex].MapNumber) == FALSE &&
				KALIMA_MAP_RANGE(gObj[aIndex].MapNumber) == FALSE &&
				DS_MAP_RANGE(gObj[aIndex].MapNumber) == FALSE &&
				IT_MAP_RANGE(gObj[aIndex].MapNumber) == FALSE &&
				gObj[aIndex].MapNumber != MAP_INDEX_CASTLESIEGE &&
				gObj[aIndex].MapNumber != MAP_INDEX_CASTLEHUNTZONE && 
				gObj[aIndex].MapNumber != MAP_INDEX_PKFIELD 
#ifdef IMPERIAL
				&& !CImperialGuardian::IsEventMap(gObj[aIndex].MapNumber)
#endif
				&& bCheckEquipmentToMove == TRUE )
			{
				gObjTeleport(aIndex, gObj[aIndex].m_wRaiseTalismanSaveMap, 
					gObj[aIndex].m_wRaiseTalismanSaveX, gObj[aIndex].m_wRaiseTalismanSaveY);
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);

				LogAddTD("[Using Item] [Charm of Raise] [%s][%s] Die Position Map:[%d] X:[%d] Y:[%d]", 
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_wRaiseTalismanSaveMap,
					gObj[aIndex].m_wRaiseTalismanSaveX, gObj[aIndex].m_wRaiseTalismanSaveY);

				gObj[aIndex].m_wRaiseTalismanSaveMap = 0;
				gObj[aIndex].m_wRaiseTalismanSaveX = 0;
				gObj[aIndex].m_wRaiseTalismanSaveY = 0;

				GCServerCmd(aIndex, 60, 0, 0);
			}
			else
			{
				char szTemp[256];
				wsprintf(szTemp, lMsg.Get(1252));
				GCServerMsgStringSend(szTemp, aIndex, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,70) ) //season 4 add-on
		{
			BOOL bCheckEquipmentToMove = gMoveCommand.CheckEquipmentToMove(&gObj[aIndex],gObj[aIndex].m_wTeleportTalismanSaveMap);

			if( gObjCheckRaiseTalismanMap(gObj[aIndex].MapNumber) == TRUE && bCheckEquipmentToMove == TRUE)
			{
				if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
				{
					gObjTeleport(aIndex, gObj[aIndex].m_wTeleportTalismanSaveMap, gObj[aIndex].m_wTeleportTalismanSaveX, gObj[aIndex].m_wTeleportTalismanSaveY);
					gObjInventoryItemSet(aIndex, pos, -1);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);

					gObj[aIndex].m_wTeleportTalismanSaveX = 0;
					gObj[aIndex].m_wTeleportTalismanSaveY = 0;
					gObj[aIndex].m_wTeleportTalismanSaveMap = 0;

					GCServerCmd(aIndex, 61, 0, 0);

					LogAddTD("[Using Item] [Charm of Move] [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
				}
				else
				{
					gObj[aIndex].m_wTeleportTalismanSaveX = gObj[aIndex].X;
					gObj[aIndex].m_wTeleportTalismanSaveY = gObj[aIndex].Y;
					gObj[aIndex].m_wTeleportTalismanSaveMap = gObj[aIndex].MapNumber;

					LogAddTD("[Using Item] [Charm of Move - SavePosition] [%s][%s] Map:[%d] X:[%d] Y:[%d]", 
						gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_wTeleportTalismanSaveMap,
						gObj[aIndex].m_wTeleportTalismanSaveX, gObj[aIndex].m_wTeleportTalismanSaveY);
				}				
			}
			else
			{
				char szTemp[256];
				wsprintf(szTemp, lMsg.Get(1252));
				GCServerMsgStringSend(szTemp, aIndex, 1);
			}			
		}
#ifdef DP
		else if( citem->m_Type == 6722 )
		{
			if( gObj[aIndex].Level < 15 )
			{
				GCServerMsgStringSend(lMsg.Get(2613), aIndex, 1);
			}
			else
			{
				if( g_DoppleganerEvent.IsEventMap(gObj[aIndex].MapNumber) )
				{
					g_DoppleganerEvent.LeaveDoppelgangerEvent(aIndex);
					if( g_DoppleganerEvent.GetDoppelgangerState() == 2 )
					{
						g_DoppleganerEvent.SendDoppelgangerResult(&gObj[aIndex], 1);
					}
				}
				gObjTeleport(aIndex, 62, 220, 20);
				if( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
                {
					gObjInventoryItemSet(aIndex, pos, -1);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}
			}
		}
#endif
		else if ( citem->m_Type == ITEMGET(13,66) ) //season 4 add-on
		{
			if(gObj[aIndex].Level < 15)
			{
				GCServerMsgStringSend(lMsg.Get(2613), aIndex, 1);
			}
			else
			{
				gObjTeleport(aIndex, MAP_INDEX_XMASEVENT, 220, 20);

				if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
				{
					gObjInventoryItemSet(aIndex, pos, -1);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}
			}	
		}
		else if ( citem->m_Type == ITEMGET(13,81) ) //season 4.5 add-on Amulet 1
		{
			g_CashItemPeriodSystem.GDReqPeriodItemInsert(&gObj[aIndex], citem->m_Type, -1);

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);	
		}
		else if ( citem->m_Type == ITEMGET(13,82) ) //season 4.5 add-on Amulet 2
		{
			g_CashItemPeriodSystem.GDReqPeriodItemInsert(&gObj[aIndex], citem->m_Type, -1);

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);	
		}
		else
		{
			LogAdd("error-L3 : %s return %s %d %d %s", 
				gObj[aIndex].Name, __FILE__,__LINE__, pos, citem->GetName());
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		}
	}
	else
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		LogAdd("error-L3 : %s return %s %d %d",	
			gObj[aIndex].Name, __FILE__,__LINE__, pos);
	}
}

void GCReFillSend(int aIndex, int Life, BYTE Ipos, BYTE flag,  int wShield) 
{
	PMSG_REFILL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x26, sizeof(pMsg));
	pMsg.IPos = Ipos;
	pMsg.LifeH = SET_NUMBERH(Life);
	pMsg.LifeL = SET_NUMBERL(Life);
	pMsg.btShieldH = SET_NUMBERH(wShield);
	pMsg.btShieldL = SET_NUMBERL(wShield);
	pMsg.Flag = 0;
	pMsg.Life = Life;
	pMsg.Shield = wShield;
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCManaSend(int aIndex, int Mana, BYTE Ipos, BYTE flag,  int BP) 
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		LogAdd("GCManaSend() return %s %d", __FILE__, __LINE__);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	int SendMana = Mana;

	if(Mana > 65535)
	{
		SendMana = 65535;
	}

	PMSG_MANASEND pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x27, sizeof(pMsg));
	pMsg.IPos = Ipos;
	pMsg.ManaH = SET_NUMBERH(SendMana);
	pMsg.ManaL = SET_NUMBERL(SendMana);
	pMsg.BPH = SET_NUMBERH(BP);
	pMsg.BPL = SET_NUMBERL(BP);

	pMsg.Mana = Mana;
	pMsg.BP = BP;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCInventoryItemDeleteSend(int aIndex, BYTE pos, BYTE flag) 
{
	PMSG_INVENTORYDELETE pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x28, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Flag = flag;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCItemUseSpecialTimeSend(int aIndex, BYTE number, int time) 
{
	if ( time > 65535 )
	{
		time = 65535;
	}

	PMSG_ITEMUSESPECIALTIME pMsg;

	PHeadSetBE((LPBYTE)&pMsg, 0x29, sizeof(pMsg));
	pMsg.Number = number;
	pMsg.Time = time;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCItemDurSend(int aIndex, BYTE pos, BYTE dur, BYTE flag) 
{
	PMSG_ITEMDUR pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2A, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Dur = dur;
	pMsg.Flag = flag;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCItemDurSend2(int aIndex, BYTE pos, BYTE dur, BYTE flag) 
{
	PMSG_ITEMDUR pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2A, sizeof(pMsg));
	pMsg.IPos	= pos;
	pMsg.Dur	= dur;
	pMsg.Flag	= flag;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGWeatherSend(int aIndex, BYTE weather) 
{
	PMSG_WEATHER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0F, sizeof(pMsg));
	pMsg.Weather = weather;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCServerCmd(int aIndex, BYTE type, BYTE Cmd1, BYTE Cmd2) 
{
	PMSG_SERVERCMD ServerCmd;

	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = type;
	ServerCmd.X = Cmd1;
	ServerCmd.Y = Cmd2;

	DataSend(aIndex, (LPBYTE)&ServerCmd, ServerCmd.h.size);
}

void GCReqmoveDevilSquare(PMSG_REQ_MOVEDEVILSQUARE* lpMsg, int aIndex) 
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE cSquareNumber = lpMsg->SquareNumber;
	BYTE cInvitationItemPos = lpMsg->InvitationItemPos - INVENTORY_BAG_START;

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 12 )
	{
		return;
	}

	PMSG_RESULT_MOVEDEVILSQUARE pResult;

	PHeadSetB((LPBYTE)&pResult, 0x90, sizeof(pResult));
	pResult.Result = 0;

	LPOBJ lpObj = &gObj[aIndex];

	LogAddTD("[DevilSquare] [%s][%s] Request Move DevilSquare [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

	BOOL bPlayerKiller = FALSE; //Season 2.5 add-on

	if(gDevilSquareAllowingPlayers == FALSE)
	{
		if(lpObj->m_PK_Level >= 4)
		{
			bPlayerKiller = TRUE;
		}
	}

	if ( bPlayerKiller == TRUE )
	{
		pResult.Result = 6;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( MAIN_INVENTORY_RANGE(cInvitationItemPos) == FALSE )
	{
		pResult.Result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	if ( DS_LEVEL_RANGE(cSquareNumber) == FALSE )
	{
		pResult.Result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	if ( g_DevilSquare.GetState() != DevilSquare_OPEN )
	{
		pResult.Result = 2;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	if ( g_DevilSquare.m_DevilSquareGround[cSquareNumber].GetObjCount() >= MAX_DEVILSQUARE_USER )
	{
		pResult.Result = 5;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	CItem * sitem = &lpObj->pInventory[cInvitationItemPos];

	if ( sitem->m_Type != ITEMGET(14,19) && ( sitem->m_Type != ITEMGET(13,46) || sitem->m_Durability <= 0.0f ) )	// Devil's Invitation
	{
		pResult.Result = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	int ItemLevel = sitem->m_Level;

	if ( sitem->m_Type == ITEMGET(13,46) )
	{
		ItemLevel = 10;
	}

	int level = lpObj->Level;

	if ( lpObj->Class == CLASS_DARKLORD 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif
		|| lpObj->Class == CLASS_MAGUMSA )
	{
		level = (level+1)/2*3;
	}

	if ( ItemLevel != 0 )
	{
		if ( ItemLevel != 10 )
		{
			if ( ItemLevel != (cSquareNumber+1) )
			{
				pResult.Result = 1;
				DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Invitation LevelError [%d][%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1, ItemLevel);
				return;
			}
		}
	}

	BOOL bEnterCheck = FALSE;
	int movegateindex = 61;

	int iENTER_LEVEL = g_DevilSquare.GetUserLevelToEnter(aIndex, movegateindex);

	if ( DS_LEVEL_RANGE(iENTER_LEVEL) == FALSE )
	{
		LogAddTD("[DevilSquare] [%s][%s] GetUserLevelToEnter() failed", lpObj->AccountID, lpObj->Name);
		return;
	}

	if ( ItemLevel == 10 )
	{
		cSquareNumber = iENTER_LEVEL;
	}

	if ( cSquareNumber != iENTER_LEVEL )
	{
		if ( cSquareNumber > iENTER_LEVEL )
		{
			bEnterCheck = 2;
		}
		else
		{
			bEnterCheck = 1;
		}
	}

	if ( bEnterCheck == 1 )
	{
		pResult.Result = 3;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);
		return;
	}

	if ( bEnterCheck == 2 )
	{
		pResult.Result = 4;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);
		return;
	}

	if ( szAuthKey[6] != AUTHKEY6 )
	{
		DestroyGIocp();
	}

	LogAddTD("[DevilSquare] [%s][%s] Move DevilSquare success [%d][%d] RemoveItem[%s][%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Level, cSquareNumber, lpObj->pInventory[cInvitationItemPos].GetName(), cInvitationItemPos, ItemLevel, lpObj->pInventory[cInvitationItemPos].m_Number);

	if ( sitem->m_Type == ITEMGET(14,19) || (sitem->m_Type == ITEMGET(13,46) && sitem->m_Durability == 1.0f) )
	{
		gObjInventoryDeleteItem(aIndex, cInvitationItemPos);
		GCInventoryItemDeleteSend(aIndex, cInvitationItemPos, 1);
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if ( sitem->m_Type == ITEMGET(13,46) && sitem->m_Durability > 1.0f )
	{
		sitem->m_Durability -= 1.0f;
		GCItemDurSend2(lpObj->m_Index, cInvitationItemPos, sitem->m_Durability, 0);
	}

	lpObj->m_nEventExp = 0;
	lpObj->m_nEventScore = 0;
	lpObj->m_nEventMoney = 0;
	lpObj->m_bDevilSquareIndex = g_DevilSquare.GetDevilSquareIndex(movegateindex);
	lpObj->m_bDevilSquareAuth = true;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 0;
	lpObj->m_IfState.use = 0;

	gObjMoveGate(lpObj->m_Index, movegateindex);
	g_DevilSquare.m_DevilSquareGround[cSquareNumber].IncObjCount();
}

void GCReqDevilSquareRemainTime(PMSG_REQ_DEVILSQUARE_REMAINTIME* lpMsg, int aIndex) 
{
	PMSG_RESULT_DEVILSQUARE_REMAINTIME pResult;

	PHeadSetB((LPBYTE)&pResult, 0x91, sizeof(pResult));
	pResult.RemainTime = 0;
	int inutil = 0;

	switch(lpMsg->hEventType)
	{
		case 1:
			if(gDevilSquareAllowingPlayers == FALSE)
			{
				if ( gObj[aIndex].m_PK_Level >= 4 )
				{
					GCServerCmd(aIndex, 0x37, 0, 0);
					LogAddTD("[PK User][DevilSquare] [%s][%s] Move Fail [PK Level:%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);
					return;
				}
			}

			pResult.hEventType = 1;

			if ( g_DevilSquare.GetState() == DevilSquare_CLOSE )
			{
				pResult.RemainTime = g_DevilSquare.GetRemainTime();
			}
			else if ( g_DevilSquare.GetState() == DevilSquare_OPEN )
			{
				pResult.RemainTime = 0;
			}
			else
			{
				pResult.RemainTime = g_DevilSquare.CheckCloseTime();
			}
			break;

		case 2:
			if(gBloodCastleAllowingPlayers == FALSE)
			{
				if ( gObj[aIndex].m_PK_Level >= 4 )
				{
					GCServerCmd(aIndex, 0x38, 0, 0);

					LogAddTD("[PK User][BloodCastle] [%s][%s] Move Fail [PK Level:%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

					return;
				}
			}

			pResult.hEventType = 2;

			if ( g_BloodCastle.GetCurrentState(lpMsg->btItemLevel) == 1 )
			{
				if ( g_BloodCastle.CheckCanEnter(lpMsg->btItemLevel) != false )
				{
					pResult.RemainTime = 0;
				}
				else
				{
					pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel);
				}
			}
			else
			{
				pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel);
			}
			break;
		case 4:
			{
				if(gChaosCastleAllowingPlayers == FALSE)
				{
					if ( gObj[aIndex].m_PK_Level >= 4 )
					{
						GCServerCmd(aIndex, 0x39, 0, 0);

						LogAddTD("[PK User][ChaosCastle] [%s][%s] Move Fail [PK Level:%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

						return;
					}
				}

				int iENTER_LEVEL =  g_ChaosCastle.GetUserLevelToEnter(aIndex);

				if ( iENTER_LEVEL == -1 )
				{
					return;
				}

				pResult.hEventType = 4;

				if ( g_ChaosCastle.GetCurrentState(iENTER_LEVEL) == 1 )
				{
					if ( g_ChaosCastle.CheckCanEnter(iENTER_LEVEL) != false )
					{
						pResult.RemainTime = 0;
						pResult.RemainTime_LOW = 0;
						pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
					}
					else
					{
						WORD wREMAIN_TIME = g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
						pResult.RemainTime = SET_NUMBERH(wREMAIN_TIME);
						pResult.RemainTime_LOW = SET_NUMBERL(wREMAIN_TIME);
						pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
					}
				}
				else
				{
					WORD wREMAIN_TIME = g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
					pResult.RemainTime = SET_NUMBERH(wREMAIN_TIME);
					pResult.RemainTime_LOW = SET_NUMBERL(wREMAIN_TIME);
					pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
				}
			}
			break;
		case 5:
			pResult.hEventType = 5;
			pResult.RemainTime = g_IllusionTempleEvent.GetRemainTime();
			pResult.EnteredUser = 0;
			pResult.RemainTime_LOW = 0;
			break;
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}

void AllSendMsg(LPBYTE Msg, int size) 
{
	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				DataSend(n, Msg, size);
			}
		}
	}
}

void AllSendSameMapMsg(LPBYTE Msg, int size, BYTE mapnumber) 
{
	for ( int n=0;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				if ( gObj[n].MapNumber == mapnumber )
				{
					DataSend(n, Msg, size);
				}
			}
		}
	}
}

void GCSendPing(int aIndex) 
{
	PMSG_PING pMsgPing;

	PHeadSetB((LPBYTE)&pMsgPing, 0x71, sizeof(pMsgPing));
	gObj[aIndex].iPingTime = GetTickCount();

	DataSend(aIndex, (LPBYTE)&pMsgPing, pMsgPing.h.size);
}

void GCPingSendRecv(PMSG_PING_RESULT* aRecv, int aIndex) 
{
	int PingTime = GetTickCount() - gObj[aIndex].iPingTime;

	LogAdd("[%s][%s] Ping = %d ms", gObj[aIndex].AccountID, gObj[aIndex].Name, PingTime);
}

void GCRegEventChipRecv(PMSG_REGEVENTCHIP* lpMsg, int aIndex) 
{
	if ( !EVENCHIP_TYPE_RANGE(lpMsg->Type) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_IfState.type == 1 )
	{
		LogAddTD("[EventChip] [%s][%s] Attempted ItemCopy using Trade Window", lpObj->AccountID, lpObj->Name);
		return;
	}

	if ( lpObj->m_IfState.type == 7 )
	{
		LogAddTD("[EventChip] [%s][%s] Attempted ItemCopy using ChaosBox Window", lpObj->AccountID, lpObj->Name);
		return;
	}

	if ( lpObj->UseEventServer != FALSE )
	{
		PMSG_REGEVENTCHIP_RESULT Result;

		PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;

		LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #1 %d", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
		return;
	}

	int Pos = lpMsg->ChipPos + INVENTORY_BAG_START;
	CItem * sitem = &lpObj->pInventory[Pos];

	if ( !sitem->IsItem() )
	{
		PMSG_REGEVENTCHIP_RESULT Result;

		PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;

		LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #2 %d", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
		return;
	}

	lpObj->UseEventServer = TRUE;

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( sitem->m_Type == ITEMGET(14,21) && sitem->m_Level == 0 )
			{
				PMSG_REQ_REGISTER_EVENTCHIP pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x38, sizeof(pMsg));
				pMsg.iINDEX = aIndex;
				pMsg.Pos = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);

				DataSendEventChip((char*)&pMsg, sizeof(pMsg));
			}
			else
			{
				PMSG_REGEVENTCHIP_RESULT Result;

				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
				Result.ChipCount = -1;
				Result.Type = 0x00;

				LogAddTD("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #3 %d", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

				DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
				lpObj->UseEventServer = FALSE;
			}
			break;
		case 0x01:
			if ( sitem->m_Type == ITEMGET(14,21) && sitem->m_Level == 1 )
			{
				PMSG_REQ_REGISTER_STONES pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof(pMsg));
				pMsg.iINDEX = aIndex;
				pMsg.iPosition = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);

				DataSendEventChip((char*)&pMsg, sizeof(pMsg));

				LogAddTD("[Stone] [%s][%s] Register Stone (Stone Pos:%d, Serial:%u)", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos, sitem->m_Number);
			}
			else
			{
				PMSG_REGEVENTCHIP_RESULT Result;

				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
				Result.ChipCount = -1;
				Result.Type = 0x01;

				LogAddTD("[Stone] [%s][%s] Not Found EventChip (Stone Pos: %d)", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

				DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
				lpObj->UseEventServer = FALSE;
			}
			break;
		default:
			lpObj->UseEventServer = FALSE;
			break;
	}
}

void GCGetMutoNumRecv(PMSG_GETMUTONUMBER* lpMsg, int aIndex) 
{
	if ( gObj[aIndex].MutoNumber != 0 )
	{
		char msg[255];
		wsprintf(msg, "이미 루가드의 숫자가 있습니다");
		GCServerMsgStringSend(msg, aIndex, 1);
		return;
	}

	if ( gObj[aIndex].UseEventServer != FALSE )
	{
		return;
	}

	gObj[aIndex].UseEventServer = TRUE;

	if ( !gObjFind10EventChip(aIndex) )
	{
		PMSG_GETMUTONUMBER_RESULT Result;

		PHeadSetB((LPBYTE)&Result, 0x96, sizeof(Result));
		Result.MutoNum[0] = -1;
		Result.MutoNum[1] = 0;
		Result.MutoNum[2] = 0;

		DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
		gObj[aIndex].UseEventServer = FALSE;

		return;
	}

	PMSG_REQ_REGISTER_MUTONUM pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x03, sizeof(pMsg));
	pMsg.iINDEX = aIndex;
	strcpy(pMsg.szUID, gObj[aIndex].AccountID);

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));

	LogAddTD("[EventChip] [%s][%s] Request MutoNumber", gObj[aIndex].AccountID, gObj[aIndex].Name);
}

void GCUseEndEventChipRescv(int aIndex) 
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 9 )
	{
		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use = 0;
	}
}

void GCUseRenaChangeZenRecv(PMSG_EXCHANGE_EVENTCHIP* lpMsg, int aIndex) 
{
	if ( gObj[aIndex].UseEventServer )
	{
		return;
	}

	gObj[aIndex].UseEventServer = TRUE;

	PMSG_REQ_RESET_EVENTCHIP pMsg;

	if ( lpMsg->btType == 1 )
	{
		PHeadSetB((LPBYTE)&pMsg, 0x09, sizeof(pMsg));
	}
	else
	{
		PHeadSetB((LPBYTE)&pMsg, 0x04, sizeof(pMsg));
	}

	pMsg.iINDEX = aIndex;
	strcpy(pMsg.szUID, gObj[aIndex].AccountID);

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));

	if ( lpMsg->btType == 0x01 )
	{
		LogAddTD("[EventChip] [%s][%s] Request Change Stones", gObj[aIndex].AccountID, gObj[aIndex].Name);
	}
	else
	{
		LogAddTD("[EventChip] [%s][%s] Request Change Rena", gObj[aIndex].AccountID, gObj[aIndex].Name);
	}
}

void CGRequestQuestInfo(int aIndex) 
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_SendQuestInfo != 0 )
	{
		return;
	}

	int questcount = g_QuestInfo.GetQeustCount();
	int foundquest = 0;

	PMSG_SEND_QEUSTINFO pMsg;
	LPQUEST_INFO lpQuestInfo;

	memcpy(pMsg.State, lpObj->m_Quest, sizeof(pMsg.State));
	int i;
	for ( i=0;i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = g_QuestInfo.GetQuestInfo(i);

		if ( lpQuestInfo )
		{
			foundquest++;

			if ( foundquest == questcount )
			{
				break;
			}
		}
	}

	int iSize = i/4+5;

	PHeadSetB((LPBYTE)&pMsg, 0xA0, iSize);
	pMsg.Count = i;
	DataSend(aIndex, (LPBYTE)&pMsg, iSize);
	lpObj->m_SendQuestInfo = true;

	if ( szAuthKey[8] != AUTHKEY8 )
	{
		DestroyGIocp();
	}
}

void GCSendQuestInfo(int aIndex, int QuestIndex) 
{
	CGRequestQuestInfo(aIndex);
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_SETQUEST pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xA1, sizeof(pMsg));
	pMsg.QuestIndex = QuestIndex;
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, QuestIndex);

	if ( pMsg.State != 0 )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}
}

void CGSetQuestState(PMSG_SETQUEST* lpMsg, int aIndex) 
{
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_SETQUEST_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xA2, sizeof(pMsg));
	pMsg.QuestIndex = lpMsg->QuestIndex;
	pMsg.Result = g_QuestInfo.SetQuestState(lpObj, lpMsg->QuestIndex, lpMsg->State);
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, lpMsg->QuestIndex);

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GCSendQuestPrize(int aIndex, BYTE Type, BYTE Count) 
{
	CGRequestQuestInfo(aIndex);
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_SETQUEST_PRIZE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xA3, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.Type = Type;
	pMsg.Count = Count;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	MsgSendV2(lpObj, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CGCloseWindow(int aIndex) 
{
	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type )
	{
		if ( gObj[aIndex].m_IfState.type == 7 )
		{
			return;
		}

		if( gObj[aIndex].m_IfState.type == 10 && gObj[aIndex].TargetShopNumber == 229 )
		{
			gQeustNpcTeleport.TalkRefDel();
			gObj[aIndex].TargetShopNumber = -1;
		}

		if( gObj[aIndex].TargetShopNumber == 568 )	//1.01.00
		{
			gQeustNpcTeleport.TalkRefDel();
			gObj[aIndex].TargetShopNumber = -1;
		}

		if ( gObj[aIndex].m_IfState.type == 3 )
		{
			gObj[aIndex].TargetShopNumber = -1;
		}

		if ( gObj[aIndex].m_IfState.type == 1 )
		{
			CGTradeCancelButtonRecv(aIndex);
		}

		if ( gObj[aIndex].m_IfState.type == 6 )
		{
			CGWarehouseUseEnd(aIndex);
		}

		if ( gObj[aIndex].m_IfState.type == 13 )
		{
			g_MixSystem.ChaosBoxInit(&gObj[aIndex]);
			gObjInventoryCommit(aIndex);
		}

		//uncoded in 1.01.00
		if ( gObj[aIndex].m_IfState.type == 18 ) //season4 add-on
		{
			gObj[aIndex].TargetShopNumber = -1;
		}

		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use = 0;

		if ( szAuthKey[7] != AUTHKEY7)
		{
			DestroyGIocp();
		}
	}
}

void CGRequestEnterBloodCastle(PMSG_REQ_MOVEBLOODCASTLE* lpMsg, int iIndex) 
{
	if ( !OBJMAX_RANGE(iIndex))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}
	
	BYTE btBridgeNumber = lpMsg->iBridgeNumber - 1;
	BYTE btInvisibleCourtItemPos = lpMsg->iItemPos;

	int iITEM_LEVEL = 0;
	DWORD dwITEM_SERIAL = 0;

	PMSG_RESULT_MOVEBLOODCASTLE pResult;

	PHeadSetB((LPBYTE)&pResult, 0x9A, sizeof(pResult));
	pResult.Result = 0;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	if ( gObj[iIndex].m_IfState.use && gObj[iIndex].m_IfState.type != 12 )
	{
		return;
	}

	BOOL bPlayerKiller = FALSE; //Season 2.5 add-on

	if(gBloodCastleAllowingPlayers == FALSE)
	{
		if(gObj[iIndex].PartyNumber >= 0) //Season 2.5 add-on
		{
			if(gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 5)
			{
				bPlayerKiller = TRUE;
			}
		}
		else if(gObj[iIndex].m_PK_Level >= 4)
		{
			bPlayerKiller = TRUE;
		}
	}

	if ( bPlayerKiller == TRUE )
	{
		pResult.Result = 7;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_BloodCastle.CheckCanEnter(btBridgeNumber) == false )
	{
		pResult.Result = 2;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].IsItem() == TRUE )
	{
		if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,18) )
		{
			iITEM_LEVEL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Level;
			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;
			
			if ( BC_CLOACK_LEVEL_RANGE(iITEM_LEVEL)==FALSE)
			{
				pResult.Result = 1;
				DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
		else if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 0.0f )
		{
			// Original, with master level from 400
			//if(g_MasterLevelSystem.IsMasterLevelUser(&gObj[iIndex]) != FALSE)
			//{
			//	iITEM_LEVEL = 8;
			//}
			if( gObj[iIndex].ThirdChangeUp == 1 )
			{
				iITEM_LEVEL = 8;
			}
			else
			{
				for ( int i=0;i<MAX_CLOACK_LEVEL;i++)
				{
					if ( g_BloodCastle.CheckEnterLevel(iIndex, i+1) == 0 )
					{
						iITEM_LEVEL = i+1;
						break;
					}
				}
			}

			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;

			if ( BC_CLOACK_LEVEL_RANGE(iITEM_LEVEL) == FALSE )
			{
				pResult.Result = 1;
				DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
		else
		{
			pResult.Result = 1;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 1;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	int iRESULT = g_BloodCastle.CheckEnterLevel(iIndex, iITEM_LEVEL);

	if ( iRESULT== 0 )
	{
		pResult.Result = 0;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else 
	{
		if ( iRESULT == -1 )
		{
			pResult.Result = 4;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( iRESULT == 1 )
		{
			pResult.Result = 3;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		return;
	}
		
	int iBC_INDEX = g_BloodCastle.EnterUserBridge(iITEM_LEVEL-1, iIndex);

	if ( iBC_INDEX == -1 )
	{
		pResult.Result = 5;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,18) || (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47)) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability == 1.0f ))
	{
		gObjInventoryDeleteItem(iIndex, btInvisibleCourtItemPos);
		GCInventoryItemDeleteSend(iIndex, btInvisibleCourtItemPos, 1);
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 1.0f )
	{
		gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability -= 1.0f;
		GCItemDurSend2(iIndex, btInvisibleCourtItemPos, gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability, 0);
	}

	gObj[iIndex].m_cBloodCastleIndex = iITEM_LEVEL-1;
	gObj[iIndex].m_cBloodCastleSubIndex = iBC_INDEX;
	gObj[iIndex].m_iBloodCastleEXP = 0;
	gObj[iIndex].m_IfState.state = 0;
	gObj[iIndex].m_IfState.type = 0;
	gObj[iIndex].m_IfState.use = 0;

	g_BloodCastle.SearchUserDeleteQuestItem(iIndex);

	char szTemp[256];
	wsprintf(szTemp, lMsg.Get(MSGGET(4, 147)), iITEM_LEVEL);

	GCServerMsgStringSend(szTemp, iIndex, 1);

	switch ( iITEM_LEVEL )
	{
		case 1:
			gObjMoveGate(iIndex, 66);
			break;
		case 2:
			gObjMoveGate(iIndex, 67);
			break;
		case 3:
			gObjMoveGate(iIndex, 68);
			break;
		case 4:
			gObjMoveGate(iIndex, 69);
			break;
		case 5:
			gObjMoveGate(iIndex, 70);
			break;
		case 6:
			gObjMoveGate(iIndex, 71);
			break;
		case 7:
			gObjMoveGate(iIndex, 80);
			break;
		case 8:
			gObjMoveGate(iIndex, 271);
			break;
	}

	LogAddTD("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%u)", iITEM_LEVEL, gObj[iIndex].AccountID, gObj[iIndex].Name, dwITEM_SERIAL);
}

void CGRequestEnterChaosCastle(PMSG_REQ_MOVECHAOSCASTLE* lpMsg, int iIndex) 
{
	if ( !OBJMAX_RANGE(iIndex))
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE btChaosCastleNumber = lpMsg->iChaosCastleNumber-1;
	BYTE btGuardMailSetItemPos = lpMsg->iItemPos;
	int iITEM_LEVEL=0;
	DWORD dwITEM_SERIAL=0;
	PMSG_RESULT_MOVECHAOSCASTLE pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xAF, 0x01, sizeof(pResult));
	pResult.Result = 0;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) || CC_MAP_RANGE(gObj[iIndex].MapNumber) || DS_MAP_RANGE(gObj[iIndex].MapNumber) )
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 200)));
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}

	if ( gObj[iIndex].m_IfState.use && gObj[iIndex].m_IfState.type != 12 )
	{
		return;
	}

	if ( gPkLimitFree == FALSE )
	{
		BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
		if(gChaosCastleAllowingPlayers == FALSE)
		{
			if(gObj[iIndex].PartyNumber >= 0) //Season 2.5 add-on
			{
				if(gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 6)
				{
					bPlayerKiller = TRUE;
				}
			}
			else if(gObj[iIndex].m_PK_Level >= 6)
			{
				bPlayerKiller = TRUE;
			}
		}

		if ( bPlayerKiller == TRUE )
		{
			PMSG_NOTICE pNotice;
			TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 201)));
			DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
			return;
		}
	}

	if ( gObjDuelCheck(&gObj[iIndex]) == TRUE )
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 202)));
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}

	if ( gObj[iIndex].lpGuild )
	{
		if ( gObj[iIndex].lpGuild->WarState == 1 )
		{
			PMSG_NOTICE pNotice;
			TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 203)));
			DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
			return;
		}
	}

	if ( gObj[iIndex].pInventory[8].m_Type == ITEMGET(13,3) ||	// Dino
		 gObj[iIndex].pInventory[8].m_Type == ITEMGET(13,2) ||	// Uniria
		 gObj[iIndex].pInventory[8].m_Type == ITEMGET(13,37) )	// Fenrir
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 204)));
		DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}

	BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
	if(gChaosCastleAllowingPlayers == FALSE)
	{
		if(gObj[iIndex].PartyNumber >= 0) //Season 2.5 add-on
		{
			if(gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 5)
			{
				bPlayerKiller = TRUE;
			}
		}
		else if(gObj[iIndex].m_PK_Level >= 4)
		{
			bPlayerKiller = TRUE;
		}
	}

	if ( bPlayerKiller == TRUE )
	{
		pResult.Result = 8;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	int iENTER_LEVEL = g_ChaosCastle.GetUserLevelToEnter(iIndex);

	if ( iENTER_LEVEL == -1 )
	{
		pResult.Result = 1;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_ChaosCastle.CheckCanEnter(iENTER_LEVEL) == false )
	{
		pResult.Result = 2;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[iIndex].pInventory[btGuardMailSetItemPos].IsItem() == TRUE )
	{
		if ( gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Type == ITEMGET(13,29) ) // Armor of Guardman
		{
			iITEM_LEVEL = gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Level;
			dwITEM_SERIAL = gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Number;
		}
		else
		{
			pResult.Result = 1;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 1;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_ChaosCastle.CheckWearingMOPH(iIndex) == TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 205)), iIndex, 1);
		return;
	}

	//season 2.5 add-on
	BYTE attr = MapC[gObj[iIndex].MapNumber].GetAttr(gObj[iIndex].X, gObj[iIndex].Y);

	if ( (attr&1) != 1 )
	{
		return;
	}

	int iBC_INDEX = -1;

	if ( g_ChaosCastle.CheckUserEnterMoney(iIndex, iENTER_LEVEL) )
	{
		iBC_INDEX = g_ChaosCastle.EnterUserChaosCastle(iENTER_LEVEL, iIndex);

		if ( iBC_INDEX == -1 )
		{
			pResult.Result = 5;
			DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 7;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	BOOL bRES = g_ChaosCastle.PayUserEnterMoney(iIndex, iENTER_LEVEL);

	if ( bRES == FALSE )
	{
		g_ChaosCastle.LeaveUserChaosCastle(iENTER_LEVEL, iIndex);
		pResult.Result = 5;
		DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	gObjInventoryDeleteItem(iIndex, btGuardMailSetItemPos);
	GCInventoryItemDeleteSend(iIndex, btGuardMailSetItemPos, 1);

	DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	gObj[iIndex].m_cChaosCastleIndex = iENTER_LEVEL;
	gObj[iIndex].m_cChaosCastleSubIndex = iBC_INDEX;
	gObj[iIndex].m_IfState.state = 0;
	gObj[iIndex].m_IfState.type = 0;
	gObj[iIndex].m_IfState.use = 0;

	if ( gObj[iIndex].m_bPShopOpen == true )
	{
		CGPShopReqClose(iIndex);
	}

	if ( gObj[iIndex].PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		int pindex = gParty.GetIndex(gObj[iIndex].PartyNumber, iIndex, gObj[iIndex].DBNumber);

		if ( pindex >= 0 )
		{
			pMsg.Number = pindex;
			CGPartyDelUser(&pMsg, iIndex);
		}
	}

	if ( gObj[iIndex].Class == CLASS_ELF && gObj[iIndex].m_RecallMon >= 0)
	{
		GCRecallMonLife(iIndex, 60, 0);
		gObjMonsterCallKill(iIndex);
	}

	char szTemp[256];
	wsprintf(szTemp, lMsg.Get(MSGGET(4, 206)), iENTER_LEVEL+1);
	GCServerMsgStringSend(szTemp, iIndex, 1);

	switch ( iENTER_LEVEL+1 )
	{
		case 1:
			gObjMoveGate(iIndex, 82);
			break;
		case 2:
			gObjMoveGate(iIndex, 83);
			break;
		case 3:
			gObjMoveGate(iIndex, 84);
			break;
		case 4:
			gObjMoveGate(iIndex, 85);
			break;
		case 5:
			gObjMoveGate(iIndex, 86);
			break;
		case 6:
			gObjMoveGate(iIndex, 87);
			break;
		case 7:
			gObjMoveGate(iIndex, 272);
			break;
	}

	LogAddTD("[Chaos Castle] (%d) (Account:%s, Name:%s) Entered Chaos Castle (Guard Mail Set Serial:%u)", iENTER_LEVEL+1, gObj[iIndex].AccountID, gObj[iIndex].Name, dwITEM_SERIAL);
}

void CGRequestRepositionUserInChaosCastle(PMSG_REQ_REPOSUSER_IN_CC * lpMsg, int aIndex) 
{
	if ( !gObjIsConnected(aIndex) )
	{
		return;
	}

	if ( !CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		return;
	}

	if ( g_ChaosCastle.ObjSetPosition(aIndex, gObj[aIndex].X, gObj[aIndex].Y) == TRUE )
	{
		gObj[aIndex].m_iChaosCastleBlowTime = GetTickCount();
	}
}

void CGRequestEventEnterCount(PMSG_REQ_CL_ENTERCOUNT* lpMsg, int aIndex) //0045A900
{
	if ( !lpMsg )
	{
		return;
	}

	switch ( lpMsg->btEventType )
	{
		case 0x02:
			EGReqBloodCastleEnterCount(aIndex);
			break;
		case 0x03: //Illusion Temple
			g_IllusionTempleEvent.EGReqIllusionTempleEnterCount(aIndex);
			break;
	}
}

void CGRequestLottoRegister(PMSG_REQ_2ANV_LOTTO_EVENT* lpMsg, int aIndex) 
{
	PMSG_REQ_2ANIV_SERIAL pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x08, sizeof(pMsg));

	if ( gObj[aIndex].UseEventServer )
	{
		return;
	}

	gObj[aIndex].UseEventServer = TRUE;
	pMsg.iINDEX = aIndex;
	pMsg.iMEMB_GUID = gObj[aIndex].DBNumber;
	memcpy(pMsg.szUID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	pMsg.szUID[MAX_ACCOUNT_LEN] = 0;
	memcpy(pMsg.SERIAL1, lpMsg->SERIAL1, 4);
	pMsg.SERIAL1[4] = 0;
	memcpy(pMsg.SERIAL2, lpMsg->SERIAL2, 4);
	pMsg.SERIAL2[4] = 0;
	memcpy(pMsg.SERIAL3, lpMsg->SERIAL3, 4);
	pMsg.SERIAL3[4] = 0;

	DataSendEventChip((char*)&pMsg, sizeof(pMsg));

	LogAddTD("[Mu_2Anv_Event] [%s][%s] Register Lotto Number (Serial: %s-%s-%s)", gObj[aIndex].AccountID, gObj[aIndex].Name, pMsg.SERIAL1, pMsg.SERIAL2, pMsg.SERIAL3);
}





void CGReqMoveOtherServer(PMSG_REQ_MOVE_OTHERSERVER * lpMsg, int aIndex) 
{
	if ( !gEnableServerDivision)
	{
		return;
	}

	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_MoveOtherServer )
	{
		return;
	}

	lpObj->m_MoveOtherServer = true;

	PMSG_DEFRESULT pResult;
	PHeadSetB((LPBYTE)&pResult, 0x99, sizeof(pResult));
	
	if ( !gObjJoominCheck(aIndex, lpMsg->LastJoominNumber))
	{
		pResult.result = 0;

		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		lpObj->m_MoveOtherServer = false;

		LogAddTD("[CharTrasfer] Fail (JoominNumber) [%s][%s]", lpObj->AccountID, lpObj->Name);
		GCServerMsgStringSend("문제 발생시 change@webzen.co.kr로 문의해 주시기바랍니다", lpObj->m_Index, 1);
		return;
	}

	SDHP_CHARACTER_TRANSFER pCharTransfer;

	GJSetCharacterInfo(lpObj, lpObj->m_Index, FALSE, 0);
	PHeadSetB((LPBYTE)&pCharTransfer, 0xCF, sizeof(pCharTransfer));
	pCharTransfer.Number = aIndex;
	memcpy(pCharTransfer.Account, lpObj->AccountID, sizeof(pCharTransfer.Account));

	cDBSMng.Send((char*)&pCharTransfer, sizeof(pCharTransfer));
	LogAddTD("[CharTrasfer] Request Character Trasfer [%s][%s]", lpObj->AccountID, lpObj->Name);
}

void GCPacketCheckSumRecv(PMSG_PACKETCHECKSUM * aRecv, int aIndex) 
{
	gPacketCheckSum.AddCheckSum(aIndex, aRecv->funcindex, aRecv->CheckSum);
}

void GCNPggSendCheckSum(int aIndex, _GG_AUTH_DATA * pggAuthData) 
{
	PMSG_NPROTECTGGCHECKSUM pMsg;

	PHeadSetBE((LPBYTE)&pMsg, 0x73, sizeof(pMsg));
	memcpy(&pMsg.m_ggadCheckSum, pggAuthData, sizeof(pMsg.m_ggadCheckSum));

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

/*void GCNPggCheckSumRecv(PMSG_NPROTECTGGCHECKSUM * lpMsg, int aIndex) 
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( !gObjIsConnected(aIndex))
	{
		return;
	}

	LogAddTD("[NPgg] Recv Checksum = %x, AuthValue = %x [%s][%s]", lpMsg->m_ggadCheckSum.dwIndex, lpObj->NPggCSAuth.m_AuthAnswer.dwIndex, lpObj->AccountID, lpObj->Name);

	memcpy(&gObj[aIndex].NPggCSAuth.m_AuthAnswer, &lpMsg->m_ggadCheckSum, sizeof(lpObj->NPggCSAuth.m_AuthAnswer));

	lpObj->m_NPggCheckSumSendTime = GetTickCount();
	lpObj->m_bSentGGAuth = false;

	LogAddTD("[NPgg] Checksum Clear %x [%s][%s]", lpMsg->m_ggadCheckSum.dwIndex, lpObj->AccountID, lpObj->Name);
}*/

void GCSendGetItemInfoForParty(int aIndex, CMapItem * lpItem) 
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->PartyNumber >= 0)
	{
		PMSG_GETITEMINFO_FOR_PARTY pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x47;
		pMsg.h.size = sizeof(pMsg);
		pMsg.NumberH = SET_NUMBERH(aIndex);
		pMsg.NumberL = SET_NUMBERL(aIndex);
		pMsg.ItemInfo = lpItem->m_Type & (ITEMGET(MAX_TYPE_ITEMS-1, MAX_SUBTYPE_ITEMS-1));

		if ( lpItem->m_Option1 )
		{
			pMsg.ItemInfo |= 0x2000;
		}

		if ( lpItem->m_Option2 )
		{
			pMsg.ItemInfo |= 0x4000;
		}

		if ( lpItem->m_Option3 )
		{
			pMsg.ItemInfo |= 0x8000;
		}

		if ( lpItem->m_NewOption )
		{
			pMsg.ItemInfo |= 0x10000;
		}

		if ( lpItem->m_SetOption )
		{
			pMsg.ItemInfo |= 0x20000;
		}

		if ( g_kItemSystemFor380.Is380OptionItem( (lpItem != NULL)?((CItem *)&lpItem->m_Number):NULL) == TRUE )
		{
			pMsg.ItemInfo |= 0x40000;
		}

		pMsg.Level = lpItem->m_Level;
		
		int partycount = gParty.GetPartyCount(lpObj->PartyNumber);

		if ( partycount >= 0 )
		{
			for ( int n=0;n<MAX_USER_IN_PARTY;n++)
			{
				int number = gParty.m_PartyS[lpObj->PartyNumber].Number[n];

				if ( number >= 0 )
				{
					DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}

void GCSendEffectInfo(int aIndex, BYTE btType) 
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_EFFECTINFO pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x48;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.btType = btType;

	if ( btType == 17 )
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGRequestPetItemCommand(PMSG_REQUEST_PET_ITEM_COMMAND * lpMsg, int aIndex) 
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->PetType )
	{
		return;
	}

	int iTargetIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	gDarkSpirit[aIndex].ChangeCommand(lpMsg->Command, iTargetIndex);
}

void CGRequestPetItemInfo(PMSG_REQUEST_PET_ITEMINFO * lpMsg, int aIndex) 
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->PetType != 0 && lpMsg->PetType != 1 )
	{
		return;
	}
	
	PMSG_SEND_PET_ITEMINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xA9, sizeof(pMsg));
	pMsg.PetType = lpMsg->PetType;
	pMsg.InvenType = lpMsg->InvenType;
	pMsg.nPos = lpMsg->nPos;

	if ( lpMsg->InvenType == 0 )	// Inventory
	{
		if ( !INVENTORY_RANGE(lpMsg->nPos))
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d", lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( lpObj->pInventory[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->pInventory[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->pInventory[lpMsg->nPos].m_PetItem_Level;
			pMsg.Life = lpObj->pInventory[lpMsg->nPos].m_Durability;
			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 1 )	// Warehouse
	{
		if ( lpObj->LoadWareHouseInfo != false )
		{
			if ( !WAREHOUSE_RANGE(lpMsg->nPos))
			{
				LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid WareHouse nPos %d",	lpObj->AccountID, lpObj->Name, lpMsg->nPos);
				return;
			}

			if ( lpObj->pWarehouse[lpMsg->nPos].IsItem() )
			{
				pMsg.Exp = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Exp;
				pMsg.Level = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Level;
				pMsg.Life = lpObj->pWarehouse[lpMsg->nPos].m_Durability;
				DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			}
		}
	}
	else if ( lpMsg->InvenType == 2 )	// Trade
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
		{
			return;
		}

		if ( !TRADE_BOX_RANGE(lpMsg->nPos))
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Trade nPos %d",	lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( lpObj->Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->Trade[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->Trade[lpMsg->nPos].m_PetItem_Level;
			pMsg.Life = lpObj->Trade[lpMsg->nPos].m_Durability;
			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 3 )	// Target Trade
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
		{
			return;
		}

		int iTargetIndex = gObj[aIndex].TargetNumber;

		if ( iTargetIndex < 0 )
		{
			return;
		}

		if ( !OBJMAX_RANGE(iTargetIndex) )
		{
			return;
		}

		if ( gObj[iTargetIndex].m_IfState.use < 1 || gObj[iTargetIndex].m_IfState.type != 1 )
		{
			return;
		}

		if ( !TRADE_BOX_RANGE(lpMsg->nPos))
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid TargetTrade nPos %d", lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( gObj[iTargetIndex].Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Level;
			pMsg.Life = gObj[iTargetIndex].Trade[lpMsg->nPos].m_Durability;
			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 4 )	// Chaos
	{
		if ( !CHAOS_BOX_RANGE(lpMsg->nPos) )
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Chaos nPos %d",	lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( lpObj->pChaosBox[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Level;
			pMsg.Life = lpObj->pChaosBox[lpMsg->nPos].m_Durability;
			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 5 )	// Personal Shop
	{
		if ( !INVENTORY_RANGE(lpMsg->nPos) )
		{
			LogAddTD("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d", lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( lpObj->m_iPShopDealerIndex != -1 )
		{
			if ( OBJMAX_RANGE(lpObj->m_iPShopDealerIndex))
			{
				if ( gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].IsItem() )
				{
					pMsg.Exp = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Exp;
					pMsg.Level = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Level;
					pMsg.Life = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_Durability;
					DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}	
}

void GCGuildViewportInfo(PMSG_REQ_GUILDVIEWPORT * aRecv, int aIndex) 
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	DWORD dwGuildNumber = aRecv->GuildNumber;
	_GUILD_INFO_STRUCT * lpGuildInfo = Guild.SearchGuild_Number(dwGuildNumber);

	if ( lpGuildInfo )
	{
		PMSG_ANS_GUILDVIEWPORT pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x66;
		pMsg.h.size = sizeof(pMsg);
		pMsg.GuildNumber = lpGuildInfo->Number;
		pMsg.btGuildType = lpGuildInfo->btGuildType;

		TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);

		if ( pUnionInfo )
		{
			memcpy(pMsg.UnionName, pUnionInfo->m_szMasterGuild, MAX_GUILD_LEN);
		}
		else
		{
			pMsg.UnionName[0] = '\0';
		}

		strcpy(pMsg.GuildName, lpGuildInfo->Name);
		memcpy(pMsg.Mark, lpGuildInfo->Mark, sizeof(pMsg.Mark));

		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}
	else
	{
		LogAddTD("★★☆ 길드 정보 찾을수 없음. 이름 : [%s] 번호 : %d", lpObj->Name, dwGuildNumber);
	}
}

void CGGuildAssignStatus(PMSG_GUILD_ASSIGN_STATUS_REQ * aRecv, int aIndex) 
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_GUILD_ASSIGN_STATUS_RESULT pMsg={0};
	
	pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(pMsg));
	pMsg.btType = aRecv->btType;
	memcpy(pMsg.szTagetName, aRecv->szTagetName, sizeof(pMsg.szTagetName));

	if ( lpObj->GuildNumber <= 0 || lpObj->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( gObj[aIndex].GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	char szTargetName[MAX_ACCOUNT_LEN+1]={0};
	memcpy(szTargetName, aRecv->szTagetName, MAX_ACCOUNT_LEN);

	if ( !strcmp(gObj[aIndex].Name, szTargetName))
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( lpObj->lpGuild == NULL )
	{
		return;
	}

	if ( aRecv->btType == 1 || aRecv->btType == 2 )
	{
		int iSubMasterCount = 0;
		int iBattleMasterCount = 0;

		for ( int n=0;n<MAX_USER_GUILD;n++)
		{
			if ( lpObj->lpGuild->Use[n] > 0 )
			{
				if ( lpObj->lpGuild->GuildStatus[n] == G_SUB_MASTER )
				{
					iSubMasterCount++;
				}
				else if ( lpObj->lpGuild->GuildStatus[n] == G_BATTLE_MASTER )
				{
					iBattleMasterCount++;
				}
			}
		}

		if ( aRecv->btGuildStatus == G_SUB_MASTER )
		{
			if ( iSubMasterCount == 0 )
			{
				GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
				return;
			}

			pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}
		else if ( aRecv->btGuildStatus == G_BATTLE_MASTER )
		{
			if ( iBattleMasterCount < (((lpObj->Level+lpObj->m_nMasterLevel) / 200 ) +1) )
			{
				GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
				return;
			}

			pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
			DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}
		else if ( aRecv->btGuildStatus == G_PERSON )
		{
			GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
			return;
		}
		else
		{
			return;
		}
	}
	
	if ( aRecv->btType == 3 )
	{
		GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
	}
}

void CGGuildAssignType(PMSG_GUILD_ASSIGN_TYPE_REQ * aRecv, int aIndex) 
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_GUILD_ASSIGN_TYPE_RESULT pMsg={0};
	
	pMsg.h.set((LPBYTE)&pMsg, 0xE2, sizeof(pMsg));
	pMsg.btGuildType = aRecv->btGuildType;
	
	if ( lpObj->GuildNumber <= 0 || lpObj->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( gObj[aIndex].GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}
	
	GDGuildReqAssignType(aIndex, aRecv->btGuildType);
}

void CGRelationShipReqJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ * aRecv, int aIndex) 
{
	if ( g_CastleSiegeSync.GetCastleState() >= 5 && g_CastleSiegeSync.GetCastleState() <= 7 )
	{
		MsgOutput(aIndex, lMsg.Get(MSGGET(6, 196)));
		return;
	}

	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE ) //season 2.5 add-on
	{
		MsgOutput(aIndex, lMsg.Get(3399));
		return;
	}

	int iTargetUserIndex = MAKE_NUMBERW(aRecv->btTargetUserIndexH, aRecv->btTargetUserIndexL);

	if ( !OBJMAX_RANGE(aIndex) || !OBJMAX_RANGE(iTargetUserIndex))
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	if ( gObjIsConnected(&gObj[iTargetUserIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if((gObj[iTargetUserIndex].m_Option&1) != 1) //season 3.0 add-on
	{
		return;
	}

#ifdef NPVP
	if( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[iTargetUserIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3431), aIndex, 1);
		 return;
	}
	// ----
	if( g_NewPVP.IsObserver(gObj[aIndex]) || g_NewPVP.IsObserver(gObj[iTargetUserIndex]) )
	{
		 GCServerMsgStringSend(lMsg.Get(3432), aIndex, 1);
		 return;
	}
#endif

	if ( !gObj[aIndex].lpGuild || !gObj[iTargetUserIndex].lpGuild )
	{
		GCResultSend(aIndex, 0x51, 5);
		return;
	}

	if ( gObj[aIndex].GuildStatus != G_MASTER && gObj[iTargetUserIndex].GuildStatus != G_MASTER)
	{
		GCResultSend(aIndex, 0x51, 4);
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 6);
		return;
	}

	if ( gObj[iTargetUserIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 6);
		return;
	}

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg={0};

	PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
	ErrMsg.btResult = 0;
	ErrMsg.btRequestType = aRecv->btRequestType;
	ErrMsg.btRelationShipType = aRecv->btRelationShipType;
	ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
	ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[iTargetUserIndex];

	if ( lpObj->GuildStatus != G_MASTER || lpTargetObj->GuildStatus != G_MASTER)
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[aIndex].lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuildInfo = gObj[iTargetUserIndex].lpGuild;
	BYTE btRelationShip = gObjGetRelationShip(lpObj, lpTargetObj);

	if ( aRecv->btRequestType == 1 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			if ( lpTargetGuildInfo->Count < gGuildAllianceMinUsers )
			{
				ErrMsg.btResult = GUILD_ANS_CANNOT_BE_UNION_MASTER;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpTargetGuildInfo->iGuildUnion != 0 && lpTargetGuildInfo->iGuildUnion == lpTargetGuildInfo->Number )
			{
				if ( UnionManager.GetGuildRelationShipCount(lpTargetGuildInfo->iGuildUnion, 1) >= MAX_UNION_MEMBER  )
				{
					ErrMsg.btResult = GUILD_ANS_EXCEED_MAX_UNION_MEMBER;
					DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
			}
		
			char szCastleOwnerGuildName[MAX_GUILD_LEN+1]={0};
			memcpy(szCastleOwnerGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILD_LEN);

			if ( !strcmp(lpGuildInfo->Name, szCastleOwnerGuildName))
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpGuildInfo->iGuildUnion )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( btRelationShip == 2 )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_RIVAL;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpTargetGuildInfo->iGuildUnion && lpTargetGuildInfo->Number != lpTargetGuildInfo->iGuildUnion )
			{
				ErrMsg.btResult = GUIDL_ANS_NOT_UNION_MASTER;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

#ifdef GENS
			if( gGensSystem.CheckAllianceEnter )
			{
				if( !gGensSystem.GetGensInfluence(lpObj) )
				{
					LogAddTD("[GensSystem] [%s][%s] Union Joining Failed to Not Join Gens GuildMaste",
						lpObj->AccountID, lpObj->Name);
					// ----
					ErrMsg.btResult = 0xA1;
					DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
				// ----
				if( !gGensSystem.GetGensInfluence(lpTargetObj) )
				{
					LogAddTD("[GensSystem] [%s][%s] Union Joining Failed to Not Join Gens GuildMaster : UnionMaster [%s][%s]",
						lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name);
					// ----
					ErrMsg.btResult = 0xA2;
					DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
				// ----
				if( gGensSystem.GetGensInfluence(lpTargetObj) != gGensSystem.GetGensInfluence(lpObj) )
				{
					LogAddTD("[GensSystem] [%s][%s] Union Joining Failed to Gens be Different : UnionMaster [%s][%s] [%s]",
						lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name, 
						gGensSystem.GetGensInfluenceName(lpTargetObj));
					// ----
					ErrMsg.btResult = 0xA3;
					DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
			}
#endif
		}
		else if ( aRecv->btRelationShipType == 2 )
		{
			if ( lpGuildInfo->iGuildRival || lpTargetGuildInfo->iGuildRival )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_RIVAL;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( btRelationShip == 1 )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}
	else if ( aRecv->btRequestType == 2 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			if ( lpGuildInfo->iGuildUnion == 0 )
			{
				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
		else if ( aRecv->btRelationShipType == 2 )
		{
			if ( !lpGuildInfo->iGuildRival || !lpTargetGuildInfo->iGuildRival )
			{
				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpGuildInfo->iGuildRival != lpTargetGuildInfo->Number || lpGuildInfo->Number != lpTargetGuildInfo->iGuildRival )
			{
				ErrMsg.btResult = GUILD_ANS_NOT_GUILD_RIVAL;
				DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}		
	
	if ( aRecv->btRequestType == 2 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			GDUnionBreakOff(aIndex, lpGuildInfo->iGuildUnion);
			return;
		}
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 14;
	lpObj->m_IfState.state = 0;
	lpObj->m_InterfaceTime = GetTickCount();

	lpTargetObj->m_IfState.use = 1;
	lpTargetObj->m_IfState.type = 14;
	lpTargetObj->m_IfState.state = 0;
	lpTargetObj->m_InterfaceTime = GetTickCount();

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ pMsg={0};

	PHeadSetB((LPBYTE)&pMsg, 0xE5, sizeof(pMsg));
	pMsg.btTargetUserIndexL = SET_NUMBERL(aIndex);
	pMsg.btTargetUserIndexH = SET_NUMBERH(aIndex);
	pMsg.btRequestType = aRecv->btRequestType;
	pMsg.btRelationShipType = aRecv->btRelationShipType;
	DataSend(iTargetUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGRelationShipAnsJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS * aRecv, int aIndex) 
{
	int iTargetUserIndex = MAKE_NUMBERW(aRecv->btTargetUserIndexH, aRecv->btTargetUserIndexL);

	if ( !OBJMAX_RANGE(aIndex) || !OBJMAX_RANGE(iTargetUserIndex))
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	if ( gObjIsConnected(&gObj[iTargetUserIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( aRecv->btResult == 1 )
	{
		if ( aRecv->btRequestType == 1 )
		{
			GDRelationShipReqJoin(iTargetUserIndex, aIndex, aRecv->btRelationShipType);
		}
		else if ( aRecv->btRequestType == 2 )
		{
			GDRelationShipReqBreakOff(aIndex, iTargetUserIndex, aRecv->btRelationShipType);
		}
	}
	else
	{
		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg ={0};

		PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
		ErrMsg.btResult = 0;
		ErrMsg.btRequestType = aRecv->btRequestType;
		ErrMsg.btRelationShipType = aRecv->btRelationShipType;
		ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
		ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;
		ErrMsg.btResult = GUILD_ANS_CANCLE_REQUEST;

		DataSend(iTargetUserIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
	}

	if ( gObj[aIndex].m_IfState.use )
	{
		if ( gObj[aIndex].m_IfState.type == 14 )
		{
			gObj[aIndex].m_IfState.use = 0;
		}
	}

	if ( gObj[iTargetUserIndex].m_IfState.use )
	{
		if ( gObj[iTargetUserIndex].m_IfState.type == 14 )
		{
			gObj[iTargetUserIndex].m_IfState.use = 0;
		}
	}
}

void CGUnionList(PMSG_UNIONLIST_REQ * aRecv, int aIndex) 
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj == NULL )
	{
		return;
	}

	if ( gObjIsConnected(&gObj[aIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( lpObj->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	GDUnionListSend(aIndex, lpObj->lpGuild->iGuildUnion);

}

void CGRelationShipReqKickOutUnionMember(PMSG_KICKOUT_UNIONMEMBER_REQ* aRecv, int aIndex) 
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if ( ::PacketCheckTime(&gObj[aIndex]) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj == NULL )
	{
		return;
	}

	if ( gObjIsConnected(&gObj[aIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		MsgOutput(aIndex, "★ Terminated User.");
		return;
	}

	if ( lpObj->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 3);
		MsgOutput(aIndex, "☆ Terminated Guild.");
		return;
	}

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg = {0};

	PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
	ErrMsg.btResult = 0;
	ErrMsg.btRequestType = 2;
	ErrMsg.btRelationShipType = 1;
	ErrMsg.btTargetUserIndexH = 0;
	ErrMsg.btTargetUserIndexL = 0;

	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	if ( lpObj->GuildStatus != GUILD_MASTER || lpObj->lpGuild->Number != lpObj->lpGuild->iGuildUnion)
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);

		return;
	}

	char szUnionMemberGuildName[MAX_GUILD_LEN+1] = {0};
	memcpy(szUnionMemberGuildName, aRecv->szTargetGuildName, MAX_GUILD_LEN);

	if ( !strcmp(lpObj->lpGuild->Name, szUnionMemberGuildName))
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	GDRelationShipReqKickOutUnionMember(aIndex, szUnionMemberGuildName);
}

void CGReqMapSvrAuth(PMSG_REQ_MAPSERVERAUTH * lpMsg, int iIndex) 
{
	if ( lpMsg->btCliVersion[0] != szClientVersion[0] ||
		 lpMsg->btCliVersion[1] != szClientVersion[1] ||
		 lpMsg->btCliVersion[2] != szClientVersion[2] ||
		 lpMsg->btCliVersion[3] != szClientVersion[3] ||
		 lpMsg->btCliVersion[4] != szClientVersion[4] )
	{
		GCAnsMapSvrAuth(iIndex, 6);
		CloseClient(iIndex);
		return;
	}

	BYTE btSerial[17];

	btSerial[16]=0;
	memcpy(btSerial, lpMsg->btCliSerial, sizeof(lpMsg->btCliSerial));
	char id[MAX_ACCOUNT_LEN+1];
	id[MAX_ACCOUNT_LEN] = 0;
	memcpy(id, lpMsg->szAccountID, MAX_ACCOUNT_LEN);
	BuxConvert(id, MAX_ACCOUNT_LEN);

	if ( strcmp((char*)btSerial, szGameServerExeSerial) )
	{
		LogAddC(2, "error-L1: Serial error [%s] [%s]", id, btSerial);
		GCAnsMapSvrAuth(iIndex, 6);
		CloseClient(iIndex);
		return;
	}

	if ( bCanConnectMember == TRUE )
	{
		if ( ConMember.IsMember(id) == FALSE )
		{
			GCAnsMapSvrAuth(iIndex, 2);	
			return;
		}
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( PacketCheckTime(lpObj) == FALSE )
	{
		LogAdd(lMsg.Get(MSGGET(1, 217)), iIndex, id);
		CloseClient(iIndex);
		return;
	}

	if ( gObj[iIndex].Connected != PLAYER_CONNECTED )
	{
		LogAdd(lMsg.Get(MSGGET(1, 218)), iIndex, id);
		CloseClient(iIndex);
		return;
	}

	if ( gObj[iIndex].LoginMsgSnd != 0 )
	{
		if ( gObj[iIndex].LoginMsgSnd == 1 )
		{
			LogAdd(lMsg.Get(MSGGET(1, 219)), iIndex, id, gObj[iIndex].Ip_addr);
		}
		else
		{
			LogAdd("error : %d %s %d", gObj[iIndex].LoginMsgSnd, __FILE__, __LINE__);
		}

		return;
	}

	gObj[iIndex].CheckTick = lpMsg->iTickCount;
	gObj[iIndex].CheckTick2 = GetTickCount();
	gObj[iIndex].ConnectCheckTime = GetTickCount(); 
	gObj[iIndex].CheckSpeedHack = true;
	gObj[iIndex].ConnectCheckTime = GetTickCount();
	gObj[iIndex].LoginMsgSnd = 1;
	gObj[iIndex].LoginMsgCount++;
	gObj[iIndex].AccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(gObj[iIndex].AccountID, id, MAX_ACCOUNT_LEN);
	memset(gObj[iIndex].Name, 0, MAX_ACCOUNT_LEN);
	gObj[iIndex].m_cAccountItemBlock = 0;
	gObj[iIndex].ukn_30 = 0; //season4.5 add-on
	GJReqMapSvrAuth(iIndex, gObj[iIndex].AccountID, lpMsg->szCharName, lpMsg->iJoinAuthCode1, lpMsg->iJoinAuthCode2, lpMsg->iJoinAuthCode3, lpMsg->iJoinAuthCode4);

	LogAddTD("[MapServerMng] Map Server Join Send : [%s][%s](%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iIndex);


}

void GCAnsMapSvrAuth(int iIndex, int iResult) 
{
	if ( !OBJMAX_RANGE(iIndex) )
	{
		LogAddC(2,"[MapServerMng] Packet Error JG [0x7B] - Index out of bound : %d",iIndex);
		return;
	}

	PMSG_ANS_MAPSERVERAUTH pMsgResult;

	PHeadSubSetB((LPBYTE)&pMsgResult, 0xB1, 0x01, sizeof(pMsgResult));
	pMsgResult.iResult = iResult;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void CGReqCastleSiegeState(PMSG_REQ_CASTLESIEGESTATE * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	GS_GDReqOwnerGuildMaster(g_MapServerManager.GetMapSvrGroup(), iIndex);
//#endif
}

void GCAnsCastleSiegeState(int iIndex, int iResult, LPSTR lpszGuildName, LPSTR lpszGuildMaster)
{
//#if (_GSCS==1)
	if( (lpszGuildName == NULL) || (lpszGuildMaster == NULL) )
	{
		return;
	}

	PMSG_ANS_CASTLESIEGESTATE pMsgResult;


	pMsgResult.h.set((LPBYTE)&pMsgResult.h, 0xB2, 0x00, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;

	memcpy(pMsgResult.cOwnerGuild, lpszGuildName, sizeof(pMsgResult.cOwnerGuild));
	memcpy(pMsgResult.cOwnerGuildMaster, lpszGuildMaster, sizeof(pMsgResult.cOwnerGuildMaster));

	SYSTEMTIME tmStateStartDate = {0};
	SYSTEMTIME tmStateEndDate = {0};
	SYSTEMTIME tmSiegeStartDate = {0};

	int iCastleState = g_CastleSiege.GetCastleState();
	int bRETVAL = g_CastleSiege.GetCastleStateTerm(&tmStateStartDate, &tmStateEndDate);
	tmSiegeStartDate = g_CastleSiege.GetCastleLeftSiegeDate();

	if( bRETVAL == FALSE )
	{
		iCastleState = -1;
	}

	int iStateLeftSec = g_CastleSiege.GetCurRemainSec();

	// Create Packet
	pMsgResult.cCastleSiegeState = iCastleState;
	// Start Date
	pMsgResult.btStartYearH = SET_NUMBERH(tmStateStartDate.wYear);
	pMsgResult.btStartYearL = SET_NUMBERL(tmStateStartDate.wYear);
	pMsgResult.btStartMonth = tmStateStartDate.wMonth;
	pMsgResult.btStartDay   = tmStateStartDate.wDay;
	pMsgResult.btStartHour  = tmStateStartDate.wHour;
	pMsgResult.btStartMinute= tmStateStartDate.wMinute;
	// End Date
	pMsgResult.btEndYearH = SET_NUMBERH(tmStateEndDate.wYear);
	pMsgResult.btEndYearL = SET_NUMBERL(tmStateEndDate.wYear);
	pMsgResult.btEndMonth = tmStateEndDate.wMonth;
	pMsgResult.btEndDay   = tmStateEndDate.wDay;
	pMsgResult.btEndHour  = tmStateEndDate.wHour;
	pMsgResult.btEndMinute= tmStateEndDate.wMinute;
	// Siege Start Date
	pMsgResult.btSiegeStartYearH = SET_NUMBERH(tmSiegeStartDate.wYear);
	pMsgResult.btSiegeStartYearL = SET_NUMBERL(tmSiegeStartDate.wYear);
	pMsgResult.btSiegeStartMonth = tmSiegeStartDate.wMonth;
	pMsgResult.btSiegeStartDay   = tmSiegeStartDate.wDay;
	pMsgResult.btSiegeStartHour  = tmSiegeStartDate.wHour;
	pMsgResult.btSiegeStartMinute= tmSiegeStartDate.wMinute;
	// Time Left
	pMsgResult.btStateLeftSec1 = SET_NUMBERH(SET_NUMBERHW(iStateLeftSec));
	pMsgResult.btStateLeftSec2 = SET_NUMBERL(SET_NUMBERHW(iStateLeftSec));
	pMsgResult.btStateLeftSec3 = SET_NUMBERH(SET_NUMBERLW(iStateLeftSec));
	pMsgResult.btStateLeftSec4 = SET_NUMBERL(SET_NUMBERLW(iStateLeftSec));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void CGReqRegCastleSiege(PMSG_REQ_REGCASTLESIEGE * lpMsg, int iIndex) 
{
//#if (_GSCS==1)
	if( g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGSIEGE )
	{
		GCAnsRegCastleSiege(iIndex, 7, "");
		return;
	}
	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		GCAnsRegCastleSiege(iIndex, 6, "");
		return;
	}
	if( g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		GCAnsRegCastleSiege(iIndex, 0, "");
	}
	else if( (g_CastleSiege.CheckCastleOwnerMember(iIndex) != FALSE) || (g_CastleSiege.CheckCastleOwnerUnionMember(iIndex) != FALSE) )
	{
		GCAnsRegCastleSiege(iIndex, 3, "");
	}
	else
	{
		GS_GDReqRegAttackGuild(g_MapServerManager.GetMapSvrGroup(), iIndex);
	}
//#endif
}

void GCAnsRegCastleSiege(int iIndex, int iResult, LPSTR lpszGuildName) 
{
//#if(_GSCS==1)
	PMSG_ANS_REGCASTLESIEGE pMsgResult;

	if( lpszGuildName == NULL )
	{
		return;
	}

	if( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x01, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	memcpy(&pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	if( ::gObjIsConnected(iIndex) )
	{
		::LogAddTD("[CastleSiege] [%s][%s] Registered Castle Siege (GUILD:%s) - Result:%d",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, iResult);
	}
//#endif
}


void CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	if( (g_CastleSiege.GetCastleState() < CASTLESIEGE_STATE_REGSIEGE) || (g_CastleSiege.GetCastleState() > CASTLESIEGE_STATE_REGMARK) ) //Fixed
	{
		GCAnsGiveUpCastleSiege(iIndex, 3, 0, 0, "");
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return;
	}
	
	if( g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		return;
	}

	GS_GDReqGuildSetGiveUp(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btGiveUp);
//#endif
}

void GCAnsGiveUpCastleSiege(int iIndex, int iResult, int bGiveUp, int iMarkCount, LPSTR lpszGuildName)
{
//#if(_GSCS==1)
	PMSG_ANS_GIVEUPCASTLESIEGE pMsgResult;

	if( lpszGuildName == NULL )
	{
		return;
	}

	if( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x02, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btIsGiveUp = bGiveUp;

	memcpy(&pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	if( (pMsgResult.btResult == 1) && (iMarkCount > 0) )
	{
		gObj[iIndex].Money += (iMarkCount*3000);

		if( gObj[iIndex].Money > MAX_ZEN ) 
		{
			gObj[iIndex].Money = MAX_ZEN;
		}
		GCMoneySend(iIndex, gObj[iIndex].Money);
	}
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	if( gObjIsConnected(iIndex) )
	{
		LogAddTD("[CastleSiege] [%s][%s] GiveUp Castle Siege (GUILD:%s) - Result:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, iResult);
	}
//#endif
}




void CGReqGuildRegInfo(PMSG_REQ_GUILDREGINFO * lpMsg, int iIndex) 
{
//#if (_GSCS==1)
	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return;
	}

	GS_GDReqGuildMarkRegInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
//#endif
}



void GCAnsGuildRegInfo(int iIndex, int iResult, CSP_ANS_GUILDREGINFO* lpMsgResult)
{
//#if (_GSCS==1)
	PMSG_ANS_GUILDREGINFO pMsgResult;

	if( lpMsgResult == NULL )
	{
		return;
	}

	if( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 3, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;
	pMsgResult.btGuildMark1 = SET_NUMBERH(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark2 = SET_NUMBERL(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark3 = SET_NUMBERH(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark4 = SET_NUMBERL(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
	pMsgResult.btRegRank = lpMsgResult->btRegRank;
	pMsgResult.btIsGiveUp = lpMsgResult->bIsGiveUp;
	memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK * lpMsg, int iIndex) 
{
//#if (_GSCS==1)
	if( lpMsg == NULL )
	{
		return;
	}
	
	if( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}
	
	if( gObj[iIndex].UseEventServer == TRUE )
	{
		return;
	}

	gObj[iIndex].UseEventServer = TRUE;

	if( g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGMARK )
	{
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}
	if( strcmp( gObj[iIndex].GuildName, "") == 0)
	{
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}
	
	if( CHECK_LIMIT(lpMsg->btItemPos, 64) == FALSE )
	{
		PMSG_ANS_REGGUILDMARK pMsgResult = {0};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;		
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		gObj[iIndex].UseEventServer = FALSE;
	}
	else
	{
		int iItemPos = (lpMsg->btItemPos + 12);

		if( gObj[iIndex].pInventory[iItemPos].IsItem() == FALSE )
		{
			PMSG_ANS_REGGUILDMARK pMsgResult = {0};
			pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
			pMsgResult.btResult = 3;		
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = FALSE;
		}
		else
		{
			if( gObj[iIndex].pInventory[iItemPos].m_Type != ITEMGET(14,21) || 
				gObj[iIndex].pInventory[iItemPos].m_Level != 3 || 
				gObj[iIndex].pInventory[iItemPos].m_Durability <= 0.0f )
			{
				PMSG_ANS_REGGUILDMARK pMsgResult = {0};
				pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
				pMsgResult.btResult = 3;		
				DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
				gObj[iIndex].UseEventServer = FALSE;
			}
			else
			{
				GS_GDReqRegGuildMark(g_MapServerManager.GetMapSvrGroup(), iIndex, iItemPos);
			}
		}
	}
//#endif
}

void GCAnsRegGuildMark(int iIndex, int iResult, CSP_ANS_GUILDREGMARK* lpMsgResult) 
{
//#if (_GSCS==1)
	if( lpMsgResult == NULL )
	{
		return;
	}

	if( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if( gObj[iIndex].pInventory[lpMsgResult->iItemPos].IsItem() == FALSE )
	{	
		PMSG_ANS_REGGUILDMARK pMsgResult ={0};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		gObj[iIndex].UseEventServer = 0;
	}
	else
	{
		if( gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Type != ITEMGET(14,21) && 
			gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Level == 3 || 
			gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability <= 0.0f ) 
		{
			PMSG_ANS_REGGUILDMARK pMsgResult = {0};
			pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
			pMsgResult.btResult = 3;
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = FALSE;
		}
		else
		{
			if( (gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability -= 1.0) > 0.0f )
			{
				::GCItemDurSend(iIndex, lpMsgResult->iItemPos, gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability, 0);
			}
			else
			{
				gObjInventoryDeleteItem(iIndex, lpMsgResult->iItemPos);
				GCInventoryItemDeleteSend(iIndex,lpMsgResult->iItemPos,1);
			}
			
			PMSG_ANS_REGGUILDMARK pMsgResult;
			pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
			
			pMsgResult.btResult = iResult;
			pMsgResult.btGuildMark1 = SET_NUMBERH(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark2 = SET_NUMBERL(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark3 = SET_NUMBERH(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark4 = SET_NUMBERL(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = 0;
			
		}
	}
//#endif
}

void CGReqNpcBuy(PMSG_REQ_NPCBUY * lpMsg, int iIndex) 
{
//#if (_GSCS==1)
	if( lpMsg == NULL )
	{
		return;
	}

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return;
	}

	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || (gObj[iIndex].GuildStatus != G_MASTER && gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		LogAddC(2, "[CastleSiege] CGReqNpcBuy() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
	}
	else
	{
		BYTE btResult = 0;

		BOOL bRETVAL = g_CastleSiege.CheckAddDbNPC(iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, btResult);

		if( bRETVAL == FALSE )
		{
			GCAnsNpcBuy(iIndex, btResult, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
			LogAddC(2, "[CastleSiege] CGReqNpcBuy() ERROR - CCastleSiege::CheckAddDbNPC() [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
		else
		{
			LogAddC(2, "[CastleSiege] CGReqNpcBuy() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
	}
//#endif
}

void GCAnsNpcBuy(int iIndex, int iResult, int iNpcNumber, int iNpcIndex) 
{
//#if(_GSCS==1)
	PMSG_ANS_NPCBUY pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x05, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void CGReqNpcRepair(PMSG_REQ_NPCREPAIR * lpMsg, int iIndex) 
{
//#if (_GSCS==1)
	if( lpMsg == NULL )
	{
		return;
	}

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return;
	}

	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) && (gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		return;
	}

	_CS_NPC_DATA pNpcData;
	BOOL bRETVAL;
	int iNpcIndex;
	int iRepairCost;

	bRETVAL = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex,pNpcData);

	if( bRETVAL == FALSE )
	{
		GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
		LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - CL Request Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		return;
	}
	else
	{
		iNpcIndex = pNpcData.m_iNPC_OBJINDEX;

		if( gObjIsConnected(iNpcIndex) == FALSE )
		{
			GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
			LogAddC(2,"[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
			return;
		}
		else if( gObj[iNpcIndex].Live == 0 )
		{
			GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
			LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
			return;
		}
		else
		{
			if( (gObj[iNpcIndex].MaxLife == gObj[iNpcIndex].Life) || (gObj[iNpcIndex].MaxLife < gObj[iNpcIndex].Life) )
			{
				gObj[iNpcIndex].Life = gObj[iNpcIndex].MaxLife;
				GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
				return;
			}
			else
			{
				iRepairCost = 0;

				switch( lpMsg->iNpcNumber )
				{
					case 277:
							iRepairCost	= float((gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life) * 0.5f) + float(gObj[iNpcIndex].m_btCsNpcDfLevel * 1000000);
						break;
					case 283:
							iRepairCost	= (gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life)* 0.3f + float((gObj[iNpcIndex].m_btCsNpcDfLevel + gObj[iNpcIndex].m_btCsNpcRgLevel) * 1000000);
						break;
					default:
						GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
						return;
				}

				if( gObj[iIndex].Money < iRepairCost )
				{
					GCAnsNpcRepair(iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
					LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Money isn't enough [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
					return;
				}
				else
				{
					LogAddC(2, "[CastleSiege] CGReqNpcRepair() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);	
					GS_GDReqCastleNpcRepair(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, iRepairCost);
					//return;
				}
			}
		}
	}	
//#endif
}


void GCAnsNpcRepair(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcHP, int iNpcMaxHP) 
{
//#if(_GSCS==1)
	PMSG_ANS_NPCREPAIR pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x06, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcHP = iNpcHP;
	pMsgResult.iNpcMaxHP = iNpcMaxHP;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}




void CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE * lpMsg, int iIndex) 
{
//#if (_GSCS==1)

	if( lpMsg == NULL )
	{
		return;
	}

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	int iNEED_GEMOFDEFEND = 0; 
	int	iNEED_MONEY = 0;
	int	iCUR_UPLVL = 0;
	int	iNXT_UPLVL = 0; 
	int	iNXT_UPVALUE = 0;
	int	bENABLE_UPGRADE = FALSE;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;

	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) )
	{
		LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		return;
	}

	switch( lpMsg->iNpcNumber )
	{
		case 277://Castle Gate
		{
			_CS_NPC_DATA m_NpcData;
			BOOL bResult = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

			if( bResult )
			{
				switch( lpMsg->iNpcUpType )
				{
					case 1:
					{
						iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
						iNXT_UPLVL = lpMsg->iNpcUpValue;

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = iNXT_UPLVL;
							iNEED_GEMOFDEFEND =  g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL-1)][1];
							iNEED_MONEY = g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL-1)][2];
						}
					}
					break;

					case 3:
					{
						iCUR_UPLVL = 0;

						for( int i = 0; i < 3; i++ )
						{
							if( m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CGATE[i] )
							{
								iCUR_UPLVL = (i+1);
							}
						}
						iNXT_UPLVL = 0;

						for(int i = 0; i < 3; i++ )
						{
							if( lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CGATE[i] )
							{
								iNXT_UPLVL = (i+1);
							}
						}

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][0];
							iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][1];
							iNEED_MONEY = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][2];
						}
					}
					break;
					default:
						{
							GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
							return;
						}
						break;
				}
			}
			else
			{
				GCAnsNpcUpgrade(iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				return;	
			}
		}
		break;
		case 283://Statue
		{
			_CS_NPC_DATA m_NpcData;

			BOOL bResult;
			
			bResult	= g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

			if( bResult )
			{
				switch( lpMsg->iNpcUpType )
				{
					case 1:
						{
							iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
							iNXT_UPLVL = lpMsg->iNpcUpValue;

							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) ) //Fixed
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = iNXT_UPLVL;
								iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL-1)][2];
							}
						}
						break;
					case 2:
						{
							iCUR_UPLVL = m_NpcData.m_iNPC_RG_LEVEL;
							iNXT_UPLVL = lpMsg->iNpcUpValue;
							
							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = iNXT_UPLVL;
								iNEED_GEMOFDEFEND = g_iNpcUpRgLevel_CSTATUE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpRgLevel_CSTATUE[(iNXT_UPLVL-1)][2];
							}
						}
						break;
					case 3:
						{
							iCUR_UPLVL = 0;

							for( int i = 0; i < 3; i++ )
							{
								if( m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CSTATUE[i] )
								{
									iCUR_UPLVL = (i+1);
								}
							}

							iNXT_UPLVL = 0;

							for( int i = 0; i < 3; i++ )
							{
								if( lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CSTATUE[i] )
								{
									iNXT_UPLVL = (i+1);
								}
							}

							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][0];
								iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][2];
							}
						}
						break;
					default:
						{
							GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
							LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - UpType doesn't Exist [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d", gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].GuildName,gObj[iIndex].GuildNumber,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
							return;
						}
						break;
				}
			}
			else
			{
				GCAnsNpcUpgrade(iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);		
				return;
			}
		}
		break;
	}

	int iGEMOFDEFEND_COUNT;
	int x;
	int bRETVAL;

	if( bENABLE_UPGRADE == FALSE )
	{
		GCAnsNpcUpgrade(iIndex, 6, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);		
	}
	else 
	{
		if( gObj[iIndex].Money < iNEED_MONEY )
		{
			GCAnsNpcUpgrade(iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
			LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Money is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Money:%d, Need Money:%d", gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].GuildName,gObj[iIndex].GuildNumber,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,iNXT_UPLVL,gObj[iIndex].Money,iNEED_MONEY);
		}
		else
		{
			for( iGEMOFDEFEND_COUNT = 0, x = 0; x < INVENTORY_SIZE; x++ )
			{
				if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
				{
					if( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,31) )
					{
						iGEMOFDEFEND_COUNT += 1;
					}
				}
			}

			if( iGEMOFDEFEND_COUNT < iNEED_GEMOFDEFEND )
			{
				GCAnsNpcUpgrade(iIndex, 4, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				LogAddTD("[CastleSiege] CGReqNpcUpgrade() ERROR - Gem is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Gem:%d, Need Gem:%d",	gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].GuildName,gObj[iIndex].GuildNumber,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,iNXT_UPLVL,iGEMOFDEFEND_COUNT,iNEED_GEMOFDEFEND);
			}
			else
			{
				bRETVAL = g_CastleSiege.PayForUpgradeDbNpc(iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,(iNXT_UPLVL-1));
				
				if( bRETVAL == FALSE )
				{
				}
				else
				{
					gObj[iIndex].m_bIsCastleNPCUpgradeCompleted = TRUE;
					LogAddTD("[CastleSiege] [0xB2][0x07] CGReqNpcUpgrade() - Pay For Npc Upgrade (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)", lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,iNXT_UPVALUE);
					GS_GDReqCastleNpcUpgrade(g_MapServerManager.GetMapSvrGroup(),iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,iNXT_UPVALUE,(iNXT_UPLVL-1));
					LogAddTD("[CastleSiege] CGReqNpcUpgrade() REQ OK - [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d", gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].GuildName,gObj[iIndex].GuildNumber,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,iNXT_UPLVL);
				}	
			}
		}

	}
//#endif
}

void GCAnsNpcUpgrade(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue) 
{
//#if(_GSCS==1)
	PMSG_ANS_NPCUPGRADE pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x07, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcUpType = iNpcUpType;
	pMsgResult.iNpcUpValue = iNpcUpValue;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void CGReqTaxMoneyInfo(PMSG_REQ_TAXMONEYINFO * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return;
	}
	
	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) )
	{
		LogAddC(2,"[CastleSiege] CGReqTaxMoneyInfo() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		GS_GDReqTaxInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
		LogAddTD("[CastleSiege] CGReqTaxMoneyInfo() REQ OK - [%s][%s], Guild:(%s)(%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
//#endif
}

void GCAnsTaxMoneyInfo(int iIndex, int iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, __int64 i64Money) 
{
//#if (_GSCS==1)
	PMSG_ANS_TAXMONEYINFO pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x08, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxRateChaos = btTaxRateChaos;
	pMsgResult.btTaxRateStore = btTaxRateStore;
	pMsgResult.btMoney1 = SET_NUMBERH(SET_NUMBERHW(HIDWORD(i64Money)));
	pMsgResult.btMoney2 = SET_NUMBERL(SET_NUMBERHW(HIDWORD(i64Money)));
	pMsgResult.btMoney3 = SET_NUMBERH(SET_NUMBERLW(HIDWORD(i64Money)));
	pMsgResult.btMoney4 = SET_NUMBERL(SET_NUMBERLW(HIDWORD(i64Money)));
	pMsgResult.btMoney5 = SET_NUMBERH( SET_NUMBERHW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney6 = SET_NUMBERL( SET_NUMBERHW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney7 = SET_NUMBERH( SET_NUMBERLW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney8 = SET_NUMBERL( SET_NUMBERLW( LODWORD( i64Money ) ) ) ;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE * lpMsg, int iIndex) 
{
//#if(_GSCS==1)

	int iMaxTaxRate;
	int iTaxRate;

	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return;
	}

	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) )
	{
		LogAddC(2,"[CastleSiege] CGReqTaxRateChange() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		iMaxTaxRate = 0;

		switch( lpMsg->btTaxType )
		{
		case 1:
			iMaxTaxRate = 3;
			break;
		case 2:
			iMaxTaxRate = 3;
			break;
		case 3:
			iMaxTaxRate = 300000;
			break;
		}

		iTaxRate = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btTaxRate1, lpMsg->btTaxRate2), MAKE_NUMBERW(lpMsg->btTaxRate3, lpMsg->btTaxRate4)); 

		if( (iTaxRate < 0) || (iTaxRate > iMaxTaxRate) )
		{
			GCAnsTaxRateChange(iIndex,0,0,0);
			LogAddTD("[CastleSiege] CGReqTaxRateChange() ERROR - Tax Rate Out of Range [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
		else
		{
			GS_GDReqTaxRateChange(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btTaxType, iTaxRate);
			LogAddTD("[CastleSiege] CGReqTaxRateChange() REQ OK - [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
	}
//#endif
}


void GCAnsTaxRateChange(int iIndex, int iResult, BYTE btTaxType, int iTaxRate) 
{
//#if (_GSCS==1)
	PMSG_ANS_TAXRATECHANGE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x09, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxType = btTaxType;
	pMsgResult.btTaxRate1 = SET_NUMBERH(SET_NUMBERHW(iTaxRate));
	pMsgResult.btTaxRate2 = SET_NUMBERL(SET_NUMBERHW(iTaxRate));
	pMsgResult.btTaxRate3 = SET_NUMBERH(SET_NUMBERLW(iTaxRate));
	pMsgResult.btTaxRate4 = SET_NUMBERL(SET_NUMBERLW(iTaxRate));
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}




void CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	int iMoneyChange;
	BOOL bRETVAL;

	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return;
	}
	
	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) )
	{
		LogAddC(2,"[CastleSiege] CGReqMoneyDrawOut() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		iMoneyChange = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btMoney4, lpMsg->btMoney3), MAKE_NUMBERW(lpMsg->btMoney2, lpMsg->btMoney1));

		if( iMoneyChange <= 0 )
		{
			GCAnsMoneyDrawOut(iIndex, 0, 0);
			LogAddTD("[CastleSiege] CGReqMoneyDrawOut() ERROR - Req Money < 0 [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
		}
		else
		{
			bRETVAL = g_CastleSiege.CheckCastleHasMoney(iMoneyChange);

			if( bRETVAL == FALSE )
			{
				GCAnsMoneyDrawOut(iIndex, 0, 0);
				LogAddTD("[CastleSiege] CGReqMoneyDrawOut() ERROR - Castle Money is Low [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
			}
			else
			{
				if( (gObj[iIndex].Money + iMoneyChange) > MAX_ZEN )
				{
					GCAnsMoneyDrawOut(iIndex, 0, 0);
				}
				else
				{
					GS_GDReqCastleMoneyChange(g_MapServerManager.GetMapSvrGroup(), iIndex, -iMoneyChange);
					LogAddTD("[CastleSiege] CGReqMoneyDrawOut() REQ OK - [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
				}
			}
		}
	}
//#endif
}

void GCAnsMoneyDrawOut(int iIndex, int iResult, __int64 i64Money) 
{
//#if(_GSCS==1)
	PMSG_ANS_MONEYDRAWOUT pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x10, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btMoney1 = SET_NUMBERH(SET_NUMBERHW(HIDWORD(i64Money)));
	pMsgResult.btMoney2 = SET_NUMBERL(SET_NUMBERHW(HIDWORD(i64Money)));
	pMsgResult.btMoney3 = SET_NUMBERH(SET_NUMBERLW(HIDWORD(i64Money)));
	pMsgResult.btMoney4 = SET_NUMBERL(SET_NUMBERLW(HIDWORD(i64Money)));
	pMsgResult.btMoney5 = SET_NUMBERH( SET_NUMBERHW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney6 = SET_NUMBERL( SET_NUMBERHW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney7 = SET_NUMBERH( SET_NUMBERLW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney8 = SET_NUMBERL( SET_NUMBERLW( LODWORD( i64Money ) ) ) ;
	
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void GCAnsCsGateState(int iIndex, int iResult, int iGateIndex) 
{
//#if(_GSCS==1)
	PMSG_ANS_CSGATESTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x11, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btIndex1 = SET_NUMBERH( (iGateIndex &0xffff) );
	pMsgResult.btIndex2 = SET_NUMBERL( (iGateIndex &0xffff) );
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	int iGateIndex;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return;
	}
	
	if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
	{
		if( gObj[iIndex].m_btCsJoinSide != 1 )
		{
			LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
			return;
		}
	}
	else
	{
		if( (g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE) && (g_CastleSiege.CheckCastleOwnerUnionMember(iIndex) == FALSE) )
		{
			LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
			return;
		}
	}
	
	iGateIndex = MAKE_NUMBERW(lpMsg->btIndex1,lpMsg->btIndex2);

	if( g_CastleSiege.CheckCsGateAlive(iGateIndex) == TRUE ) 
	{
		int iOperateGate = g_CastleSiege.OperateGate(iGateIndex, gObj[iGateIndex].m_iCsNpcExistVal, lpMsg->btOperate);

		if( iOperateGate == 0 )
		{
			GCAnsCsGateOperate(iIndex, 0, -1, 0);
			LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Operate Gate Failed [%s][%s], Guild:(%s)(%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);	
		}
		else
		{
			GCAnsCsGateOperate(iIndex, 1, iGateIndex, lpMsg->btOperate);
			g_CastleSiege.SendCsGateStateViewPort(iGateIndex, lpMsg->btOperate);
			LogAddTD("[CastleSiege] CGReqCsGateOperate() OK - [%s][%s], Guild:(%s)(%d), DOOR:(%d)(X:%d,Y:%d)(STATUE:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildNumber,iGateIndex, gObj[iGateIndex].X, gObj[iGateIndex].Y, gObj[iGateIndex].m_btCsGateOpen);
		}
	}
	else
	{
		GCAnsCsGateOperate(iIndex, 2, -1, 0);
		LogAddTD("[CastleSiege] CGReqCsGateOperate() ERROR - Gate Doesn't Exist [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);	
	}
//#endif
}

void GCAnsCsGateOperate(int iIndex, int iResult, int iGateIndex, int iGateOperate) 
{
//#if(_GSCS==1)
	PMSG_ANS_CSGATEOPERATE pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x12, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex & 0xffff); //??
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex & 0xffff); //??
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void GCAnsCsGateCurState(int iIndex, int iGateIndex, int iGateOperate) 
{
//#if(_GSCS==1)
	PMSG_ANS_CSGATECURSTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x13, sizeof(pMsgResult));
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex & 0xffff); //??
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex & 0xffff); //??
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void GCAnsCsAccessSwitchState(int iIndex, int iSwitchIndex, int iSwitchUserIndex, BYTE btSwitchState) 
{
//#if(_GSCS==1)
	PMSG_ANS_NOTIFYSWITCHPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x14, sizeof(pMsgResult));
	pMsgResult.btIndex1 = SET_NUMBERH(iSwitchIndex & 0xffff); //??
	pMsgResult.btIndex2 = SET_NUMBERL(iSwitchIndex & 0xffff); //??
	pMsgResult.btUserIndex1 = SET_NUMBERH(iSwitchUserIndex & 0xffff); //??
	pMsgResult.btUserIndex2 = SET_NUMBERL(iSwitchUserIndex & 0xffff); //??
	pMsgResult.btSwitchState = btSwitchState;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void GCAnsCsAccessCrownState(int iIndex, BYTE btCrownState) 
{
//#if(_GSCS==1)
	PMSG_ANS_NOTIFYCROWNPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x15, sizeof(pMsgResult));
	pMsgResult.btCrownState = btCrownState;

	if( btCrownState == 0 )
	{
		if( gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 || gObj[iIndex].m_iAccumulatedCrownAccessTime > 61000 ) //season4.0 changed
		{
			gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
		}

		LogAddTD("[CastleSiege] [Reg. Attempt] Accumulated Crown AccessTime : %d [%s](%s)(%s)",	gObj[iIndex].m_iAccumulatedCrownAccessTime,gObj[iIndex].GuildName,gObj[iIndex].AccountID,gObj[iIndex].Name);
	}

	if( btCrownState == 1 )
	{
		LogAddTD("[CastleSiege] [Reg. Success] Accumulated Crown AccessTime : %d [%s](%s)(%s)",	gObj[iIndex].m_iAccumulatedCrownAccessTime,gObj[iIndex].GuildName,gObj[iIndex].AccountID,gObj[iIndex].Name);
		gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
	}

	if( btCrownState == 2 )
	{
		gObj[iIndex].m_iAccumulatedCrownAccessTime += GetTickCount() - g_CastleSiege.GetCrownAccessTickCount();
		
		if( gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 || gObj[iIndex].m_iAccumulatedCrownAccessTime > 61000 ) //season4.0 changed
		{
			gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
		}

		LogAddTD("[CastleSiege] [Reg. Fail] Accumulated Crown AccessTime : %d [%s](%s)(%s)", gObj[iIndex].m_iAccumulatedCrownAccessTime,gObj[iIndex].GuildName,gObj[iIndex].AccountID,gObj[iIndex].Name);
	
		if( gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 ) //season4.0 changed
		{
			//gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
		}
	}

	pMsgResult.dwAccumulatedCrownAccessTime = gObj[iIndex].m_iAccumulatedCrownAccessTime; //??

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void GCAnsCsNotifyStart(int iIndex, BYTE btStartState) 
{
//#if(_GSCS==1)
	PMSG_ANS_NOTIFYCSSTART pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x17, sizeof(pMsgResult));
	pMsgResult.btStartState = btStartState;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
//#endif
}

void GCAnsCsNotifyProgress(int iIndex, BYTE btCastleSiegeState, LPCSTR lpszGuildName) 
{
	if ( lpszGuildName == NULL )
	{
		return;
	}

	PMSG_ANS_NOTIFYCSPROGRESS pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x18, sizeof(pMsgResult));
	pMsgResult.btCastleSiegeState = btCastleSiegeState;
	memset(pMsgResult.szGuildName, 0, sizeof(pMsgResult.szGuildName));
	memcpy(pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GCAnsCsMapSvrTaxInfo(int iIndex, BYTE btTaxType, BYTE btTaxRate) 
{
	PMSG_ANS_MAPSVRTAXINFO pMsgResult;
	
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1A, sizeof(pMsgResult));
	pMsgResult.btTaxType = btTaxType;
	pMsgResult.btTaxRate = btTaxRate;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

}

void CGReqCsMiniMapData(PMSG_REQ_MINIMAPDATA * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( (gObj[iIndex].m_btCsJoinSide <= 0) || ( g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE ) )
	{
		GCAnsCsMiniMapData(iIndex, 3);
	}
	else if( g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE )
	{
		GCAnsCsMiniMapData(iIndex, 2);
	}
	else
	{
		g_CastleSiege.AddMiniMapDataReqUser(iIndex);
		GCAnsCsMiniMapData(iIndex, 1);
	}
//#endif
}

void GCAnsCsMiniMapData(int iIndex, BYTE btResult) 
{
	PMSG_ANS_MINIMAPDATA pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1B, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}



void CGReqStopCsMiniMapData(PMSG_REQ_STOPMINIMAPDATA * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	g_CastleSiege.DelMiniMapDataReqUser(iIndex);
//#endif
}



void CGReqCsSendCommand(PMSG_REQ_CSCOMMAND * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( gObj[iIndex].m_btCsJoinSide <= 0 ||	g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		return;
	}

	if( g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE )
	{
		return;
	}

	GCAnsCsSendCommand(gObj[iIndex].m_btCsJoinSide, lpMsg->btTeam, lpMsg->btX, lpMsg->btY, lpMsg->btCommand);
//#endif
}


void GCAnsCsSendCommand(int iCsJoinSize, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand) 
{
//#if(_GSCS==1)
	PMSG_ANS_CSCOMMAND pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1D, sizeof(pMsgResult));
	
	pMsgResult.btTeam = btTeam;
	pMsgResult.btX = btX;
	pMsgResult.btY = btY;
	pMsgResult.btCommand = btCommand;
	
	for( int iIndex = OBJ_STARTUSERINDEX; iIndex < OBJMAX; iIndex++ )
	{
		if( gObjIsConnected(iIndex) == FALSE ) continue;

		if( (gObj[iIndex].m_btCsJoinSide == iCsJoinSize) && (gObj[iIndex].MapNumber == MAP_INDEX_CASTLESIEGE) )
		{
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		}
	}
//#endif
}

void GCAnsCsLeftTimeAlarm(BYTE btHour, BYTE btMinute) 
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	for (int iIndex = OBJ_STARTUSERINDEX;iIndex < OBJMAX;iIndex++)
	{
		if ( !gObjIsConnected(iIndex) )
		{
			continue;
		}

		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
	}

}

void GCAnsSelfCsLeftTimeAlarm(int iIndex, BYTE btHour, BYTE btMinute) 
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	if ( !gObjIsConnected(iIndex))
	{
		return;
	}

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}
	
	if( g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE )
	{
		GCAnsCsSetEnterHuntZone(iIndex, 2, lpMsg->btHuntZoneEnter);
	}
	else if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
	{
		GCAnsCsSetEnterHuntZone(iIndex, 0, lpMsg->btHuntZoneEnter);
	}
	else
	{
		g_CastleSiege.SetHuntZoneEnter(lpMsg->btHuntZoneEnter);
		GCAnsCsSetEnterHuntZone(iIndex, 1, lpMsg->btHuntZoneEnter);
	}

//#endif
}

void GCAnsCsSetEnterHuntZone(int iIndex, BYTE btResult, BYTE btEnterHuntZone) 
{
	PMSG_ANS_CSHUNTZONEENTER pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1F, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;
	pMsgResult.btHuntZoneEnter = btEnterHuntZone;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void CGReqNpcDbList(PMSG_REQ_NPCDBLIST * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	PMSG_ANS_NPCDBLIST pResult;

	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		return;
	}
	
	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || ( (gObj[iIndex].GuildStatus != 128) && (gObj[iIndex].GuildStatus != 64) ) )
	{
		LogAddC(2, "[CastleSiege] CGReqNpcDbList() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		pResult.h.set((LPBYTE)&pResult, 0xB3, sizeof(pResult));
		pResult.iCount = 0;
		pResult.btResult = 2;
		DataSend(iIndex, (LPBYTE)&pResult, sizeof(pResult));
	}
	else
	{
		switch( lpMsg->btMonsterCode )
		{
		case 1:
			g_CastleSiege.SendNpcStateList(iIndex, 277);
			break;
		case 2:
			g_CastleSiege.SendNpcStateList(iIndex, 283);
			break;
		}
	}
//#endif
}

void CGReqCsRegGuildList(PMSG_REQ_CSREGGUILDLIST * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	if( lpMsg == NULL ) return;

	GS_GDReqAllGuildMarkRegInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
//#endif
}

void CGReqCsAttkGuildList(PMSG_REQ_CSATTKGUILDLIST * lpMsg, int iIndex) 
{
//#if(_GSCS==1)
	char cBUFFER[1625]; //season 2.5 changed old -> 1022
	PMSG_ANS_CSATTKGUILDLIST* lpMsgSend;
	PMSG_CSATTKGUILDLIST* lpMsgSendBody;
	int iCount;

	if( lpMsg == NULL )	return;
	
	lpMsgSend = (PMSG_ANS_CSATTKGUILDLIST*)(cBUFFER);
	lpMsgSendBody = (PMSG_CSATTKGUILDLIST*)(cBUFFER+sizeof(PMSG_ANS_CSATTKGUILDLIST));

	iCount = 0;

	lpMsgSend->btResult = g_CastleSiege.GetCsAttkGuildList(lpMsgSendBody, iCount);
	lpMsgSend->iCount = iCount;

	if( lpMsgSend->iCount < 0 )	 lpMsgSend->iCount = 0;
	if( lpMsgSend->iCount > 100 )lpMsgSend->iCount = 100;

	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0xB5, ((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
	DataSend(iIndex, (LPBYTE)lpMsgSend, ((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
//#endif
}

void CGReqWeaponUse(PMSG_REQ_USEWEAPON * aRecv, int iIndex) 
{
	WORD wObjIndex = MAKE_NUMBERW(aRecv->btObjIndexH, aRecv->btObjIndexL);

	if ( !OBJMAX_RANGE(wObjIndex))
	{
		return;
	}

	if ( !OBJMAX_RANGE(iIndex))
	{
		return;
	}

	LPOBJ lpOwnerObj = &gObj[iIndex];
	LPOBJ lpWeaponObj = &gObj[wObjIndex];

	if ( lpOwnerObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpWeaponObj->Type != OBJ_NPC )
	{
		return;
	}

	PMSG_ANS_USEWEAPON pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xB7, 0x01, sizeof(pMsg));
	pMsg.btResult = 1;

	if ( lpWeaponObj->Class == 221 )	// Slingshot attack
	{
		pMsg.btWeaponType = 1;
		pMsg.btObjIndexH = SET_NUMBERH(wObjIndex);
		pMsg.btObjIndexL = SET_NUMBERL(wObjIndex);
		g_CsNPC_Weapon.GetTargetPointXY(lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY, TRUE);
	}
	else if( lpWeaponObj->Class == 222 )	// Slingshot defense
	{
		pMsg.btWeaponType = 2;
		pMsg.btObjIndexH = SET_NUMBERH(wObjIndex);
		pMsg.btObjIndexL = SET_NUMBERL(wObjIndex);
		g_CsNPC_Weapon.GetTargetPointXY(lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY, TRUE);
	}

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));

	pMsg.btResult = 2;

	for ( int n=0;n<MAX_VIEWPORT;n++)
	{
		if ( lpWeaponObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpWeaponObj->VpPlayer2[n].state )
			{
				if ( iIndex != lpWeaponObj->VpPlayer2[n].number )
				{
					DataSend(lpWeaponObj->VpPlayer2[n].number, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}

	gObjNotifyUseWeaponV1(lpOwnerObj, lpWeaponObj, pMsg.btPointX, pMsg.btPointY);
	g_CsNPC_Weapon.SetWeaponCalDamageInfo(wObjIndex, pMsg.btPointX, pMsg.btPointY, 10000);
}

void CGReqWeaponDamageValue(PMSG_REQ_WEAPON_DAMAGE_VALUE * aRecv, int iIndex) 
{
	if ( !OBJMAX_RANGE(iIndex) )
	{
		return;
	}

	BYTE btTargetX = 0;
	BYTE btTargetY = 0;
	WORD wObjIndex = MAKE_NUMBERW(aRecv->btObjIndexH, aRecv->btObjIndexL);

	if ( g_CsNPC_Weapon.GetWeaponCalDamageInfo(wObjIndex, btTargetX, btTargetY) == TRUE )
	{
		gObjNotifyUseWeaponDamage(&gObj[wObjIndex], btTargetX, btTargetY);
	}
	else
	{
		MsgOutput(iIndex, lMsg.Get(MSGGET(6, 112)));
	}
}

//#if(_GSCS == 1)
void GCSendObjectCreationState(int iObjectIndex) 
{
	if ( !OBJMAX_RANGE(iObjectIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iObjectIndex];
	PMSG_NOTIFY_OBJECT_CREATION_STATE	pMsg = {0};

	if(lpObj->Class == 278)
	{
		PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x01, sizeof(pMsg));
		pMsg.btObjIndexH = SET_NUMBERH(iObjectIndex);
		pMsg.btObjIndexL = SET_NUMBERL(iObjectIndex);
		pMsg.btCreationState = lpObj->m_btCreationState;
		MsgSendV2(lpObj, (LPBYTE)&pMsg, sizeof(pMsg)); 
	}
}
//#endif

void CGReqGuildMarkOfCastleOwner(PMSG_REQ_GUILDMARK_OF_CASTLEOWNER * aRecv, int iIndex) 
{
	if ( !OBJMAX_RANGE(iIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	char szGuildName[MAX_GUILD_LEN+1] = {0};

	memcpy(szGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILD_LEN);

	if ( szGuildName )
	{
		_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(szGuildName);

		if ( lpGuild != NULL )
		{
			PMSG_ANS_GUILDMARK_OF_CASTLEOWNER pMsg = {0};

			PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x02, sizeof(pMsg));
			memcpy(pMsg.GuildMarkOfCastleOwner, lpGuild->Mark, sizeof(pMsg.GuildMarkOfCastleOwner));

			DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
}

//#if(_GSCS==1)
void CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE * aRecv, int iIndex) 
{
	if( !OBJMAX_RANGE(iIndex) )
	{
		return;
	}
	
	LPOBJ lpObj = &gObj[iIndex];

	PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x05, sizeof(pMsg));

	pMsg.btResult = 0;

	int iEnterTaxMoney = 0;

	iEnterTaxMoney = g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index, TRUE);

	if( iEnterTaxMoney < 0)
	{
		iEnterTaxMoney = 0;
	}

	BOOL bPermission = FALSE;

	if( g_CastleSiege.GetHuntZoneEnter() )
	{
		bPermission = TRUE;
	}

	if( lpObj->lpGuild )
	{
		if( g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}

		if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}
	}
	else
	{
		if( g_CastleSiege.GetHuntZoneEnter() )
		{
			bPermission = TRUE;
		}
	}

	if( bPermission == TRUE )
	{
		if( lpObj->Money >= iEnterTaxMoney )
		{
			if( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				if( gObjMoveGate(lpObj->m_Index, 95) == TRUE )
				{
					pMsg.btResult = 1;
					int iOldMoney = lpObj->Money;

					lpObj->Money -= iEnterTaxMoney;

					g_CastleSiegeSync.AddTributeMoney(iEnterTaxMoney);
					
					if( lpObj->Money < 0)
					{
						lpObj->Money = 0;
					}
					
					GCMoneySend(lpObj->m_Index, lpObj->Money);

					LogAddTD("[Castle HuntZone] [%s][%s] - Entrance TAX : %d - %d = %d", lpObj->AccountID, lpObj->Name, iOldMoney, iEnterTaxMoney, lpObj->Money);
				}
			}
		}
	}

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}
//#endif

void CGReqJewelMix(PMSG_REQ_JEWEL_MIX * lpMsg, int iIndex) 
{
	if ( lpMsg == NULL )
	{
		return;
	}

	CJewelMixSystem::MixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelMix);
}

void GCAnsJewelMix(int iIndex, int iResult) 
{
	PMSG_ANS_JEWEL_UNMIX pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBC, 0x00, sizeof(pMsg));
	pMsg.btResult = iResult;

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CGReqJewelUnMix(PMSG_REQ_JEWEL_UNMIX * lpMsg, int iIndex) 
{
	if ( lpMsg == NULL )
	{
		return;
	}

	CJewelMixSystem::UnMixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelLevel, lpMsg->btJewelPos);
}

void GCAnsJewelUnMix(int iIndex, int iResult) 
{
	PMSG_ANS_JEWEL_UNMIX pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBC, 0x01, sizeof(pMsg));
	pMsg.btResult = iResult;

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CGReqCrywolfInfo(PMSG_REQ_CRYWOLF_INFO* lpMsg, int iIndex) 
{
	if ( !OBJMAX_RANGE(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
//#if(_GSCS==1)
	GCAnsCrywolfInfo(iIndex, g_Crywolf.GetOccupationState(), g_Crywolf.GetCrywolfState());
//#else
//	GCAnsCrywolfInfo(iIndex, g_CrywolfSync.GetOccupationState(), g_CrywolfSync.GetCrywolfState());
//#endif
}


void GCAnsCrywolfInfo(int iIndex, BYTE btOccupationState, BYTE btCrywolfState) 
{
	PMSG_ANS_CRYWOLF_INFO pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x00, sizeof(pMsg));
	pMsg.btOccupationState = btOccupationState;
	pMsg.btCrywolfState = btCrywolfState;
	
	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CGReqAlatrContract(PMSG_REQ_CRYWOLF_ALTAR_CONTRACT* lpMsg, int iIndex) 
{
	int iAltarIndex = MAKE_NUMBERW(lpMsg->btObjIndexH, lpMsg->btObjIndexL);

	if ( !OBJMAX_RANGE(iIndex))
	{
		return;
	}

	if ( !OBJMAX_RANGE(iAltarIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpAltarObj = &gObj[iAltarIndex];

	if ( !CRYWOLF_ALTAR_CLASS_RANGE(lpAltarObj->Class))
	{
		return;
	}

	if ( !gObjIsConnected(lpObj))
	{
		return;
	}

	PMSG_ANS_CRYWOLF_ALTAR_CONTRACT pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x03, sizeof(pMsg));
	pMsg.btResult = 0;
	pMsg.btAltarState = g_CrywolfNPC_Altar.GetAltarState(lpAltarObj->Class);
	pMsg.btObjIndexH = lpMsg->btObjIndexH;
	pMsg.btObjIndexL = lpMsg->btObjIndexL;

	if ( pMsg.btAltarState == 0 && lpObj->Type == OBJ_USER && lpObj->Class == CLASS_ELF && lpObj->Level >= MIN_ELF_LEVEL_ALTAR )
	{
		if ( g_CrywolfNPC_Altar.SetAltarUserIndex(iAltarIndex, lpAltarObj->Class, iIndex) != FALSE )
		{
			pMsg.btResult = 1;
		}
	}
	else
	{
		MsgOutput(iIndex, lMsg.Get(MSGGET(13, 9)));
	}

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CGReqPlusChaosRate(PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE* lpMsg, int iIndex) 
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x09, sizeof(pMsg));

	if(g_CrywolfSync.GetOccupationState() == 0 && g_iCrywolfApplyMvpBenefit != 0) //season 3.0 add-on
	{
		pMsg.btPlusChaosRate = g_CrywolfSync.GetPlusChaosRate();
	}	

	DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void CGReqKanturuStateInfo(PMSG_REQ_KANTURU_STATE_INFO* lpMsg, int iIndex) 
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

//#if(_GSCS==0)
	g_KanturuEntranceNPC.NotifyEntranceInfo(iIndex);
//#endif
}



void GCReqEnterKanturuBossMap(PMSG_REQ_ENTER_KANTURU_BOSS_MAP* lpMsg, int iIndex) 
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

//#if(_GSCS==0)
	g_KanturuEntranceNPC.NotifyResultEnterKanturuBossMap(iIndex);
//#endif
}

#ifdef PCBANG
void CGReqPCBangShopBuy(PMSG_REQ_PCBANG_SHOP_BUY* lpMsg, int iIndex) //season 4.5 add-on 004700E0
{
	if(lpMsg == NULL)
	{
		return;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if(PacketCheckTime(&gObj[iIndex]) == FALSE)
	{
		PMSG_POINT_BUY_ITEM pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xD1, 0x05, sizeof(pMsg));
		pMsg.Result = 6;
		pMsg.ItemPos = 0xFF;
		DataSend(iIndex,(LPBYTE)&pMsg,pMsg.h.size);
		return;
	}

	g_PCBangPointSystem.PCBangBuyItem(iIndex, lpMsg->btPosition);
}

void CGReqPCBangShopOpen(PMSG_REQ_PCBANG_SHOP_OPEN* lpMsg, int iIndex) //season 4.5 add-on 004701C0
{
	if(lpMsg == NULL)
	{
		return;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	g_PCBangPointSystem.PCBangShopOpen(iIndex);
}
#endif

void CGReqWerewolfMove(PMSG_REQ_WEREWOLF_MOVE* lpMsg, int iIndex) 
{
	if(lpMsg == NULL)
	{
		return;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	g_QuestInfo.GCReqWerewolfMove(iIndex);
}

void CGReqGatekeeperMove(PMSG_REQ_GATEKEEPER_MOVE* lpMsg, int iIndex) 
{
	if(lpMsg == NULL)
	{
		return;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	g_QuestInfo.GCReqGateKeeperMove(iIndex);
}

void CGReqEnterIllusionTemple(PMSG_ANS_ILLUSIONTEMPLE_ENTER* lpMsg, int iIndex) //case 0 
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	g_IllusionTempleEvent.EnterIllusionTemple(iIndex,lpMsg->btFloorIndex-1, lpMsg->TicketPos);
}

void CGReqUseIllusionTempleKillCntSkill(PMSG_USE_ILLUSIONTEMPLE_KILLCOUNT_SKILL* lpMsg, int iIndex) //case 2 
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	WORD TargetIndex = MAKE_NUMBERW( lpMsg->btTargetH, lpMsg->btTargetL );

	g_IllusionTempleEvent.IllusionTempleUseSkill(iIndex, MAKE_NUMBERW( lpMsg->btSkillIdH, lpMsg->btSkillIdL ), TargetIndex, lpMsg->btDis);
}

void CGReqIllusionTempleDropReward(PMSG_ILLUSIONTEMPLE_DROP_REWARD* lpMsg, int iIndex) //case 5 
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	g_IllusionTempleEvent.ReqEventReward(iIndex);
}

void GCSendIllusionTempleKillCount(int aIndex, BYTE KillCount) 
{
	PMSG_SEND_ILLUSIONTEMPLE_KILLCOUNT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));
	pMsg.btKillCount = KillCount;
	DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
}

//00470570  /> \55            PUSH EBP
void CGReqXMasToDeviasMapMove(PMSG_REQ_XMASNPCMAP_MOVE* lpMsg, int iIndex) 
{
	if(lpMsg == NULL)
	{
		return;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	ReqNPCXMasMapMoveDevias(iIndex); //Move to Devias! :)
}

//004705D0  /> \55            PUSH EBP
void CGReqXMasSetPayItem(PMSG_REQ_XMAS_PAYITEM* lpMsg, int iIndex) 
{
	if(lpMsg == NULL)
	{
		return;
	}

	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	EGAnsRegXMasSetPayItem(iIndex, 1);
}

//00470630  /> \55            PUSH EBP                                 ;  ChangeNameRequest
void CGCheckNameRequest(PMSG_REQ_CHARACTER_CHECKNAME* lpMsg, int iIndex) //identical
{
	if(gObj[iIndex].Connected < PLAYER_LOGGED)
	{
		LogAdd(lMsg.Get(476),iIndex,gObj[iIndex].Ip_addr);
		CloseClient(iIndex);
		return;
	}

	PMSG_ANS_CHARACTER_CHECKNAME pMsg; //loc4
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x15, sizeof(pMsg));

	if(bCanChangeCharacterName != FALSE) //serverinfo.dat
	{
		char szChangeName[10]; //loc8
		szChangeName[11] = '\0';

		memcpy(szChangeName, lpMsg->ChangeName, sizeof(szChangeName));

		int Value = 95; //underline
		char * check = strchr(szChangeName, Value);

		if(check != NULL)
		{
			pMsg.Result = 1; //:FAILURE MSG? Special Character?
		}
		else
		{
			pMsg.Result = 0;
		}
	}
	else
	{
		pMsg.Result = 0;
	}

	memcpy(pMsg.Name, lpMsg->ChangeName, sizeof(pMsg.Name));
	DataSend(iIndex,(LPBYTE)&pMsg,pMsg.h.size);
}

//00470770  /> \55            PUSH EBP
void CGChangeNameRequest(PMSG_REQ_CHARACTER_CHANGENAME* lpMsg, int iIndex) //identical
{
	if(bCanChangeCharacterName == FALSE) //serverinfo.dat
	{
		return;
	}

	if(gObj[iIndex].Connected < PLAYER_LOGGED)
	{
		LogAdd(lMsg.Get(476),iIndex,gObj[iIndex].Ip_addr);
		CloseClient(iIndex);
		return;
	}

	PMSG_ANS_CHARACTER_CHANGENAME pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x16, sizeof(pMsg));
	pMsg.iIndex = iIndex;

	memcpy(pMsg.AccountID, gObj[iIndex].AccountID, sizeof(pMsg.AccountID));
	memcpy(pMsg.Name, lpMsg->Name, sizeof(pMsg.Name));
	memcpy(pMsg.ChangeName, lpMsg->ChangeName, sizeof(pMsg.ChangeName));

	cDBSMng.Send((char *)&pMsg, pMsg.h.size);
}

//004708A0  /> \55            PUSH EBP
void CGReqRegLuckyCoinItemCount(PMSG_REQ_REG_LUCKYCOIN* lpMsg, int iIndex) 
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if(gObj[iIndex].UseEventServer != 0)
	{
		return;
	}

	gObj[iIndex].UseEventServer = 1;

	EGAnsRegLuckyCoinItem(iIndex);
}

//00470940  /> \55            PUSH EBP
void GCSendLuckyCoinCount(int iIndex, int CoinCount) //identical
{
	PMSG_ANS_LUCKYCOIN_COINT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0B, sizeof(pMsg));
	pMsg.CoinCount = CoinCount;
	DataSend(iIndex,(LPBYTE)&pMsg,pMsg.h.size);

	gObj[iIndex].UseEventServer = 0;
}

//004709C0  /> \55            PUSH EBP
void CGSearchLuckyCoinItem(PMSG_REQ_SEARCH_LUCKY_COIN *lpMsg, int iIndex) //identical
{
	int iValidItemPos = -1;

	if(gObj[iIndex].UseEventServer != 0)
	{
		return;
	}

	gObj[iIndex].bIsChaosMixCompleted = 1;
	gObj[iIndex].UseEventServer = 1;

	for(int n = INVETORY_WEAR_SIZE; n < MAIN_INVENTORY_SIZE; n++) //2
	{
		if( gObj[iIndex].pInventory[n].IsItem() == TRUE &&
			gObj[iIndex].pInventory[n].m_Type == ITEMGET(14,100))
		{
			iValidItemPos = n;
			break;
		}
	}

	if(iValidItemPos == -1)
	{
		PMSG_REGISTER_LUCKYCOIN pMsg = {0};
		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0C, sizeof(pMsg));
		pMsg.btResult = 0;
		//missing pMsg.CoinCount
		DataSend(iIndex,(LPBYTE)&pMsg,pMsg.h.size);
		gObj[iIndex].UseEventServer = 0;
	}
	else
	{
		gObj[iIndex].pInventory[iValidItemPos].m_Durability -= 1.0f;
		EGAnsRegLuckyCoinItemPosition(iIndex,iValidItemPos);
	}
}

//00470BD0  /> \55            PUSH EBP									;  LuckyCoin Register
void CGReqLuckyCoinRegister(int iIndex, BYTE Result, int Coin, BYTE Ipos) //identical
{
	PMSG_REGISTER_LUCKYCOIN pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0C, sizeof(pMsg));

	pMsg.btResult = Result;
	pMsg.CoinCount = Coin;

	if(Result == 1)
	{
		if(gObj[iIndex].pInventory[Ipos].m_Durability == 0.f)
		{
			gObjInventoryDeleteItem(iIndex, Ipos);
			GCInventoryItemDeleteSend(iIndex, Ipos, 1);
		}
		else
		{
			GCItemDurSend2(iIndex, Ipos, gObj[iIndex].pInventory[Ipos].m_Durability, 0);
		}
		LogAddTD("[LuckyCoinEvent] RegistCoin [%s][%s], Coin: %d",gObj[iIndex].AccountID, gObj[iIndex].Name, Coin);
	}
	else
	{
		gObj[iIndex].pInventory[Ipos].m_Durability += 1.0f;
	}

	DataSend(iIndex,(LPBYTE)&pMsg,pMsg.h.size);
	gObj[iIndex].UseEventServer = 0;
}

//00470E00  /> \55            PUSH EBP									;  LuckyCoin Trade
typedef void(*LuckyCoinEventItemBoxOpen)(LPOBJ lpObj, BYTE btMapNumber, BYTE cX, BYTE cY);

void CGReqLuckyCoinTrade(PMSG_REQ_TRADE_LUCKYCOIN* lpMsg, int iIndex) //identical
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex]; //1
	
	if ( !gObjIsConnected(iIndex))
	{
		return;
	}

	LuckyCoinEventItemBoxOpen	pLuckyCoinEventItemBoxOpen; //2
	int LuckyCoinNumber = lpMsg->LuckyCoinNumber; //3
	int LuckyCoinCount; //4
	int n; //5

	switch(LuckyCoinNumber)
	{
	case 10:
		pLuckyCoinEventItemBoxOpen = LuckyCoint10ItemBagOpen;
		break;
	case 20:
		pLuckyCoinEventItemBoxOpen = LuckyCoint20ItemBagOpen;
		break;
	case 30:
		pLuckyCoinEventItemBoxOpen = LuckyCoint30ItemBagOpen;
		break;
	default:
		GCSendLuckyCoinResult(iIndex, 4);
		return;
	}

	LuckyCoinCount = 0; //4
	
	for(n = INVETORY_WEAR_SIZE; n < MAIN_INVENTORY_SIZE; n++) //5
	{
		if( gObj[iIndex].pInventory[n].IsItem() == TRUE &&
			gObj[iIndex].pInventory[n].m_Type == ITEMGET(14,100))
		{
			LuckyCoinCount = LuckyCoinCount + (float)gObj[iIndex].pInventory[n].m_Durability;
		}
	}

	if(LuckyCoinCount < LuckyCoinNumber)
	{
		GCSendLuckyCoinResult(iIndex, 0);
	}
	else if( !CheckInventoryEmptySpace(lpObj, 4, 2) )
	{
		GCSendLuckyCoinResult(iIndex, 2);
	}
	else
	{
		//????????????????????????????????????????????????????????????????
		gObj[iIndex].bIsChaosMixCompleted = true;
		lpObj->ChaosLock = TRUE;
		gObjInventoryTrans(lpObj->m_Index);

		for(n = INVETORY_WEAR_SIZE; n < MAIN_INVENTORY_SIZE; n++) //5
		{
			if( gObj[iIndex].pInventory[n].IsItem() == TRUE && gObj[iIndex].pInventory[n].m_Type == ITEMGET(14,100))
			{
				if(gObj[iIndex].pInventory[n].m_Durability < (float)LuckyCoinNumber)
				{
					LuckyCoinNumber -= gObj[iIndex].pInventory[n].m_Durability;
					gObjInventoryDeleteItem(iIndex, n);
					GCInventoryItemDeleteSend(iIndex, n, 1);
				}
				else
				{
					if(gObj[iIndex].pInventory[n].m_Durability == (float)LuckyCoinNumber)
					{
						gObjInventoryDeleteItem(iIndex, n);
						GCInventoryItemDeleteSend(iIndex, n, 1);
						break; //
					}
			
					gObj[iIndex].pInventory[n].m_Durability -= LuckyCoinNumber;
					GCItemDurSend2(lpObj->m_Index, n, gObj[iIndex].pInventory[n].m_Durability, 0);
					break; //
				}
			}
		}
		LogAddTD("[LuckyCoinEvent] TradeCoin [%s][%s], TradeCoin: %d",gObj[iIndex].AccountID, gObj[iIndex].Name, LuckyCoinNumber);
		pLuckyCoinEventItemBoxOpen(lpObj, 237, lpObj->X, lpObj->Y);
	}
}

//00471310  /> \55            PUSH EBP
void GCSendLuckyCoinResult(int aIndex, BYTE result) //identical
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( !gObjIsConnected(aIndex))
	{
		return;
	}

	PMSG_LUCKYCOIN_RESULT pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0D, sizeof(pMsg));
	pMsg.btResult = result;
	DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
}


#ifndef NPVP
//Season 4.6
void CGDuelStartRequestRecv(PMSG_REQ_START_DUEL * lpMsg, int aIndex) 
{
//#if(_GSCS == 0)
	if(g_iNewPVPSystem != TRUE)
	{
		GCServerMsgStringSend(lMsg.Get(3420), aIndex, 1);//#need add to message.wtf
		return;
	}
//#else
//	GCServerMsgStringSend(lMsg.Get(3421), aIndex, 1);//#need add to message.wtf
//	return;
//#endif
	int iDuelIndex = -1;
	char szTempText[256];
	
	if ( gObj[aIndex].CloseType != -1 )
		return;
	
	if ( gNonPK )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 174)), aIndex, 1);
		return;
	}

	if ( !gPkLimitFree )
	{
		BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
		if(gObj[aIndex].PartyNumber >= 0) //Season 2.5 add-on
		{
			if(gParty.GetPKPartyPenalty(gObj[aIndex].PartyNumber) >= 6)
			{
				bPlayerKiller = TRUE;
			}
		}
		else if(gObj[aIndex].m_PK_Level >= 6)
		{
			bPlayerKiller = TRUE;
		}

		if ( bPlayerKiller == TRUE ) //? gObj[aIndex].m_PK_Level >= 6
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 175)), aIndex, 1);
			return;
		}
	}

	if ( OBJMAX_RANGE(gObj[aIndex].m_iDuelUserReserved) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 159)), aIndex, 1);
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 161)), aIndex, 1);
		return;
	}

	if ( OBJMAX_RANGE(gObj[aIndex].m_iDuelUser ) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 160)), aIndex, 1);
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 161)), aIndex, 1);
		return;
	}

	if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 207)), aIndex, 1);
		return;
	}
	
	BYTE btDuelRoom = g_DuelRoom.GetFreeRoom();

	if ( btDuelRoom == -1 )
	{
		PMSG_DUEL_START_ERROR pMsgError;

		pMsgError.h.c = 0xC1;
		pMsgError.h.headcode = 0xAA;
		pMsgError.h.subcode = 0x01;
		pMsgError.ErrorType = 0x10;
		pMsgError.h.size = sizeof(pMsgError);

		DataSend(gObj[aIndex].m_Index, (LPBYTE)&pMsgError, pMsgError.h.size);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( !PacketCheckTime(lpObj ) )
	{
		return;
	}

	iDuelIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( !OBJMAX_RANGE(iDuelIndex) )
	{
		LogAdd("error : %s %d (%d)", __FILE__, __LINE__, iDuelIndex);
		return;
	}

	if ( iDuelIndex == aIndex  )
	{
		return;
	}
	
	
	if( gObj[aIndex].Class == CLASS_MAGUMSA || gObj[aIndex].Class == CLASS_DARKLORD || 
		gObj[iDuelIndex].Class == CLASS_MAGUMSA || gObj[iDuelIndex].Class == CLASS_DARKLORD)
	{
		if ( gObj[aIndex].Level < 20 || gObj[iDuelIndex].Level < 20)
		{
			PMSG_DUEL_START_ERROR pMsgError;
			pMsgError.h.c = 0xC1;
			pMsgError.h.headcode = 0xAA;
			pMsgError.h.subcode = 0x01;
			pMsgError.ErrorType = 0x0C;
			pMsgError.h.size = sizeof(pMsgError);
			DataSend(gObj[aIndex].m_Index,(LPBYTE)&pMsgError, pMsgError.h.size);
			return;
		}
	}
	else
	{
		if ( gObj[aIndex].Level < 30 || gObj[iDuelIndex].Level < 30)
		{
			PMSG_DUEL_START_ERROR pMsgError;
			pMsgError.h.c = 0xC1;
			pMsgError.h.headcode = 0xAA;
			pMsgError.h.subcode = 0x01;
			pMsgError.ErrorType = 0x0C;
			pMsgError.h.size = sizeof(pMsgError);
			DataSend(gObj[aIndex].m_Index,(LPBYTE)&pMsgError, pMsgError.h.size);
			return;
		}
	}

	if ( gObj[aIndex].Money < 10000 || gObj[iDuelIndex].Money < 10000 )
	{
		PMSG_DUEL_START_ERROR pMsgError;
		pMsgError.h.c = 0xC1;
		pMsgError.h.headcode = 0xAA;
		pMsgError.h.subcode = 0x01;
		pMsgError.ErrorType = 0x1e;
		pMsgError.h.size = sizeof(pMsgError);
		DataSend(gObj[aIndex].m_Index, (LPBYTE)&pMsgError, pMsgError.h.size);
		return;
	}

	if ( !gPkLimitFree )
	{
		BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
		if(gObj[aIndex].PartyNumber >= 0) //Season 2.5 add-on
		{
			if(gParty.GetPKPartyPenalty(gObj[aIndex].PartyNumber) >= 6)
			{
				bPlayerKiller = TRUE;
			}
		}
		else if(gObj[aIndex].m_PK_Level >= 6)
		{
			bPlayerKiller = TRUE;
		}

		if ( bPlayerKiller == TRUE )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 176)), aIndex, 1);
			return;
		}
	}

	char szDuelName[MAX_ACCOUNT_LEN+1]={0};
	char szDuelName2[MAX_ACCOUNT_LEN+1]={0};
	memcpy(szDuelName, gObj[iDuelIndex].Name, MAX_ACCOUNT_LEN);
	szDuelName[MAX_ACCOUNT_LEN] = 0;
	memcpy(szDuelName2, lpMsg->szName, MAX_ACCOUNT_LEN);
	szDuelName2[MAX_ACCOUNT_LEN] = 0;

	if ( strcmp(szDuelName, szDuelName2) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 162)), aIndex, 1);
		return;
	}

	if ( IsDuelEnable(iDuelIndex) == FALSE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 163)), aIndex, 1);
		return;
	}

	if ( lpObj->lpGuild && lpObj->lpGuild->WarState == 1 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 164)), aIndex, 1);
		return;
	}

	if ( gObj[iDuelIndex].lpGuild && gObj[iDuelIndex].lpGuild->WarState == 1 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 165)), aIndex, 1);
		return;
	}

	if ( gObjIsConnected(iDuelIndex) == FALSE )
	{
		return;
	}

	if ( gObj[iDuelIndex].Type == OBJ_MONSTER )
	{
		return;
	}

	if ( gObj[iDuelIndex].CloseCount >= 0 )
	{
		return;
	}

	BOOL bRetVal = FALSE;

	for (int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 )
		{
			bRetVal = TRUE;
			break;
		}
	}

	if ( bRetVal )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 189)), aIndex, 1);
		return;
	}

	for (int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( gObj[iDuelIndex].SelfDefense[n] >= 0 )
		{
			bRetVal = TRUE;
			break;
		}
	}

	if ( bRetVal )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 189)), aIndex, 1);
		return;
	}

	if ( DS_MAP_RANGE(gObj[iDuelIndex].MapNumber ) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 180)), aIndex, 1);
		return;
	}

	if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 166)), aIndex, 1);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 167)), aIndex, 1);
		return;
	}

	if ( gObj[iDuelIndex].m_IfState.use > 0 )
	{
		wsprintf(szTempText, lMsg.Get(MSGGET(4, 168)), gObj[iDuelIndex].Name);
		GCServerMsgStringSend(szTempText, aIndex, 1);
		return;
	}

	if ( OBJMAX_RANGE(gObj[iDuelIndex].m_iDuelUserRequested) )
	{
		wsprintf(szTempText, lMsg.Get(MSGGET(4, 169)), gObj[iDuelIndex].Name);
		GCServerMsgStringSend(szTempText, aIndex, 1);
		return;
	}

	if ( OBJMAX_RANGE(gObj[iDuelIndex].m_iDuelUserReserved) )
	{
		wsprintf(szTempText, lMsg.Get(MSGGET(4, 170)), gObj[iDuelIndex].Name);
		GCServerMsgStringSend(szTempText, aIndex, 1);
		return;
	}

	if ( OBJMAX_RANGE(gObj[iDuelIndex].m_iDuelUser) )
	{
		wsprintf(szTempText, lMsg.Get(MSGGET(4, 171)), gObj[iDuelIndex].Name);
		GCServerMsgStringSend(szTempText, aIndex, 1);
		return;
	}

	lpObj->m_iDuelUserReserved = iDuelIndex;
	lpObj->m_iDuelUser = -1;
	gObj[iDuelIndex].m_iDuelUserRequested = aIndex;
	//lpObj->m_btDuelRoom = btDuelRoom;
	//gObj[iDuelIndex].m_btDuelRoom = btDuelRoom;

	g_DuelRoom.m_Zone[btDuelRoom].UserIndex[DUEL_USER_INDEX_1] = lpObj->m_Index;
	g_DuelRoom.m_Zone[btDuelRoom].UserIndex[DUEL_USER_INDEX_2] = gObj[iDuelIndex].m_Index;
	g_DuelRoom.m_Zone[btDuelRoom].bWaiting = TRUE;
	PMSG_REQ_DUEL_OK pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x02;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	memcpy(pMsg.szName, gObj[aIndex].Name, sizeof(pMsg.szName));

	DataSend(iDuelIndex, (LPBYTE)&pMsg, pMsg.h.size);

	wsprintf(szTempText, lMsg.Get(MSGGET(4, 172)), gObj[iDuelIndex].Name);
	GCServerMsgStringSend(szTempText, aIndex, 1);

	LogAddTD("[Duel] [%s][%s] Requested to Start Duel to [%s][%s] in Zone [%d]", lpObj->AccountID, lpObj->Name, gObj[iDuelIndex].AccountID, gObj[iDuelIndex].Name,btDuelRoom);
}

void CGDuelEndRequestRecv(PMSG_REQ_END_DUEL * lpMsg, int aIndex)
{
//#if(_GSCS==1)
//	return;
//#endif
	int iDuelIndex = -1;

	if ( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	if ( OBJMAX_RANGE(gObj[aIndex].m_iDuelUserReserved) )
	{
		gObj[aIndex].m_iDuelUserReserved = -1;
	}

	if ( OBJMAX_RANGE(gObj[aIndex].m_iDuelUser) )
	{
		iDuelIndex = gObj[aIndex].m_iDuelUser;
		gObj[aIndex].m_iDuelUser = -1;

		PMSG_ANS_END_DUEL pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0xAB;
		pMsg.h.size = sizeof(pMsg);
		pMsg.NumberH = SET_NUMBERH(iDuelIndex);
		pMsg.NumberL = SET_NUMBERL(iDuelIndex);
		memcpy(pMsg.szName, gObj[iDuelIndex].Name, sizeof(pMsg.szName));

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		if ( iDuelIndex == aIndex )
		{
			return;
		}
	}
	else
	{
		LogAdd("error : %s %d (%d)", __FILE__, __LINE__, iDuelIndex);
		return;
	}

	if ( OBJMAX_RANGE(gObj[iDuelIndex].m_iDuelUser) )
	{
		if ( gObj[iDuelIndex].m_iDuelUser == aIndex )
		{
			gObj[iDuelIndex].m_iDuelUser = -1;

			PMSG_ANS_END_DUEL pMsg;

			pMsg.h.c = 0xC1;
			pMsg.h.headcode = 0xAB;
			pMsg.h.size = sizeof(pMsg);
			pMsg.NumberH = SET_NUMBERH(aIndex);
			pMsg.NumberL = SET_NUMBERL(aIndex);
			memcpy(pMsg.szName, gObj[aIndex].Name, sizeof(pMsg.szName));

			DataSend(iDuelIndex, (LPBYTE)&pMsg, pMsg.h.size);

			LogAddTD("[Duel] [%s][%s] Duel Ended [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[iDuelIndex].AccountID, gObj[iDuelIndex].Name);
		}
	}
}

void CGDuelOkRequestRecv(PMSG_ANS_DUEL_OK * lpMsg, int aIndex)
{
//#if(_GSCS==1)
//	return;
//#endif
	PMSG_ANS_START_DUEL pMsgSend;

	int iDuelIndex = -1;
	
	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0xAA;
	pMsgSend.h.subcode = 0x01;
	pMsgSend.bDuelStart = 0;
	pMsgSend.h.size = sizeof(pMsgSend);

	BYTE btDuelRoom;// = gObj[aIndex].m_btDuelRoom;

	if(btDuelRoom < 0 || btDuelRoom > MAX_DUEL_ROOMS - 1) 
	{
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	iDuelIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( OBJMAX_RANGE(iDuelIndex) )
	{
		if ( !gObjIsConnected(iDuelIndex) )
			return;

		if ( gObj[iDuelIndex].Type == OBJ_MONSTER )
			return;

		if ( gObj[iDuelIndex].CloseCount >= 0 )
			return;

		if ( lpMsg->bDuelOK )
		{
			if ( BC_MAP_RANGE(gObj[iDuelIndex].MapNumber) || CC_MAP_RANGE(gObj[iDuelIndex].MapNumber) || DS_MAP_RANGE(gObj[iDuelIndex].MapNumber) )
			{
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 207)), aIndex, 1);
				
				g_DuelRoom.ClearRoom(btDuelRoom);
				
				memcpy(pMsgSend.szName, gObj[aIndex].Name, sizeof(pMsgSend.szName));
				DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
				return;
			}
		}

		if ( OBJMAX_RANGE(gObj[aIndex].m_iDuelUser ) )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 160)), aIndex, 1);
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 161)), aIndex, 1);
			
			g_DuelRoom.ClearRoom(btDuelRoom);

			memcpy(pMsgSend.szName, gObj[aIndex].Name, sizeof(pMsgSend.szName));
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
			return;
		}

		if ( OBJMAX_RANGE(gObj[aIndex].m_iDuelUserReserved) )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 173)), aIndex, 1);
			
			g_DuelRoom.ClearRoom(btDuelRoom);

			memcpy(pMsgSend.szName, gObj[aIndex].Name, sizeof(pMsgSend.szName));
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
			return;
		}

		if ( gObj[iDuelIndex].m_iDuelUserReserved == aIndex )
		{
			char szDuelName[MAX_ACCOUNT_LEN+1]={0};
			char szDuelName2[MAX_ACCOUNT_LEN+1]={0};
			memcpy(szDuelName, gObj[iDuelIndex].Name, MAX_ACCOUNT_LEN);
			szDuelName[MAX_ACCOUNT_LEN] = 0;
			memcpy(szDuelName2, lpMsg->szName, MAX_ACCOUNT_LEN);
			szDuelName2[MAX_ACCOUNT_LEN] = 0;

			if ( !strcmp(szDuelName, szDuelName2))
			{
				if ( lpMsg->bDuelOK == false)
				{
					g_DuelRoom.ClearRoom(btDuelRoom);
					memcpy(pMsgSend.szName, gObj[aIndex].Name, sizeof(pMsgSend.szName));
					pMsgSend.bDuelStart = 0x0F;

					DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
				}
				else if ( lpMsg->bDuelOK == true )
				{
					if ( gObj[aIndex].Money < 10000 )
					{
						GCSendEndDuel(&gObj[iDuelIndex]);
						
						g_DuelRoom.ClearRoom(btDuelRoom);

						pMsgSend.bDuelStart = 0x0e;
						pMsgSend.h.size = sizeof(pMsgSend);
						DataSend(gObj[aIndex].m_Index, (BYTE*)&pMsgSend, pMsgSend.h.size);
						return;
					}

					if(gObj[iDuelIndex].Money < 10000)
					{
						g_DuelRoom.ClearRoom(btDuelRoom);
						pMsgSend.bDuelStart = 0x0e;
						pMsgSend.h.size = sizeof(pMsgSend);
						DataSend(iDuelIndex, (BYTE*)&pMsgSend, pMsgSend.h.size);
						return;
					}
					
					if(gObjMoveGate(gObj[aIndex].m_Index,g_DuelGates[btDuelRoom].UserGate01) == false)
					{
						g_DuelRoom.ClearRoom(btDuelRoom);
						return;
					}

					if(gObjMoveGate(iDuelIndex, g_DuelGates[btDuelRoom].UserGate02) == false)
					{
						g_DuelRoom.ClearRoom(btDuelRoom);
						return;
					}

					gObj[aIndex].Money -= 10000;
					GCMoneySend(gObj[aIndex].m_Index, gObj[aIndex].Money);

					gObj[iDuelIndex].Money -= 10000;
					GCMoneySend(iDuelIndex, gObj[iDuelIndex].Money);


					gObj[iDuelIndex].m_iDuelUserReserved = -1;
					gObj[iDuelIndex].m_btDuelScore = 0;
					gObj[iDuelIndex].m_iDuelUser = aIndex;
					gObj[iDuelIndex].m_iDuelTickCount = GetTickCount();

					gObj[aIndex].m_iDuelUserRequested = -1;
					gObj[aIndex].m_iDuelUserReserved = -1;
					gObj[aIndex].m_btDuelScore = 0;
					gObj[aIndex].m_iDuelUser = iDuelIndex;
					gObj[aIndex].m_iDuelTickCount = GetTickCount();
				
					g_DuelRoom.m_Zone[btDuelRoom].btOpen = 0;
					g_DuelRoom.m_Zone[btDuelRoom].dwClearRoomTime = GetTickCount() + 900000;

					g_DuelRoom.m_Zone[btDuelRoom].bWaiting = FALSE;

					pMsgSend.bDuelStart = 0;
					pMsgSend.NumberH = SET_NUMBERH(iDuelIndex);
					pMsgSend.NumberL = SET_NUMBERL(iDuelIndex);
					memcpy(pMsgSend.szName, szDuelName, sizeof(pMsgSend.szName));

					DataSend(aIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);

					pMsgSend.NumberH = SET_NUMBERH(aIndex);
					pMsgSend.NumberL = SET_NUMBERL(aIndex);
					memcpy(pMsgSend.szName, gObj[aIndex].Name, sizeof(pMsgSend.szName));

					DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);

					GCSendDuelScore(gObj[aIndex].m_Index, gObj[iDuelIndex].m_Index,btDuelRoom);

					LogAddTD("[Duel] [%s][%s] Duel Started [%s][%s] on Room [%d]", gObj[aIndex].AccountID, gObj[aIndex].Name,gObj[iDuelIndex].AccountID, gObj[iDuelIndex].Name,btDuelRoom);
				}
			}
			else
			{
				g_DuelRoom.ClearRoom(btDuelRoom);
				DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
				return;
			}
		}
		else
		{
			g_DuelRoom.ClearRoom(btDuelRoom);
			DataSend(iDuelIndex, (LPBYTE)&pMsgSend, pMsgSend.h.size);
			return;
		}
	}
}

void GCSendDuelScore(int aIndex1, int aIndex2,BYTE btDuelRoom)
{
//#if(_GSCS==1)
//	return;
//#endif
	if(btDuelRoom < 0 || btDuelRoom > MAX_DUEL_ROOMS - 1)
	{
		return;
	}

	if ( !OBJMAX_RANGE(aIndex1) || !OBJMAX_RANGE(aIndex2))
	{
		return;
	}

	if ( !gObjIsConnected(aIndex1) || !gObjIsConnected(aIndex2) )
	{
		return;
	}

	if ( gObj[aIndex1].Type == OBJ_MONSTER || gObj[aIndex2].Type == OBJ_MONSTER )
	{
		return;
	}
	
	if ( gObj[aIndex1].CloseCount >= 0 || gObj[aIndex2].CloseCount >= 0 )
	{
		return;
	}
	
	PMSG_ANS_DUEL_SCORE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x04;
	pMsg.NumberH1 = SET_NUMBERH(aIndex1);
	pMsg.NumberL1 = SET_NUMBERL(aIndex1);
	pMsg.NumberH2 = SET_NUMBERH(aIndex2);
	pMsg.NumberL2 = SET_NUMBERL(aIndex2);
	pMsg.btDuelScore1 = gObj[aIndex1].m_btDuelScore;
	pMsg.btDuelScore2 = gObj[aIndex2].m_btDuelScore;

	DataSend(aIndex1, (LPBYTE)&pMsg, pMsg.h.size);
	DataSend(aIndex2, (LPBYTE)&pMsg, pMsg.h.size);

	for(int i = 0; i < MAX_DUEL_SPECTATORS; i++)
	{
		if(g_DuelRoom.m_Zone[btDuelRoom].SpectatorIndex != 0) 
		{
			DataSend(g_DuelRoom.m_Zone[btDuelRoom].SpectatorIndex[i], (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CGDuelSpectateAddRecv(PMSG_REQ_SPECTATE_ADD_DUEL* lpMsg,int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if ( !gObjIsConnected(aIndex))
	{
		return;
	}

	if(gObj[aIndex].m_IfState.use == 0 || gObj[aIndex].m_IfState.type != 20) 
	{
		return;
	}

	gObj[aIndex].m_IfState.use = 0;
	gObj[aIndex].m_IfState.type = 0;

	if(lpMsg->btRoomIndex < 0 && lpMsg->btRoomIndex >= MAX_DUEL_ROOMS)
	{
		LogAddC(2,"[Duel] [%s][%s] Wrong Duel Room [%d]", gObj[aIndex].AccountID, gObj[aIndex].Name,lpMsg->btRoomIndex);
		return;
	}

	if(g_DuelRoom.m_Zone[lpMsg->btRoomIndex].btOpen == TRUE) 
	{
		GCServerMsgStringSend(lMsg.Get(3422), gObj[aIndex].m_Index, 1);
		LogAddC(2,"[Duel Zone] [%s][%s] Spectate Duel error, zone [%d] is open", gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->btRoomIndex);
		return;
	}
	
	if(g_DuelRoom.GetSpectatorCount(lpMsg->btRoomIndex) >= 10)
	{
		GCServerMsgStringSend(lMsg.Get(3423), gObj[aIndex].m_Index, 1);
		LogAddC(2,"[Duel Zone] [%s][%s] Spectate Duel error, zone [%d] is full", gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->btRoomIndex);
		return;
	}

	if(g_DuelRoom.IsSpectator(aIndex,lpMsg->btRoomIndex) == 1)
	{
		LogAddC(2,"[Duel Zone] Spectate Duel error, player [%s][%s] is already watching duel in zone [%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->btRoomIndex);
		return;
	}

	for(int i = 0; i < MAX_DUEL_SPECTATORS; i++)
	{
		if(g_DuelRoom.m_Zone[lpMsg->btRoomIndex].SpectatorIndex[i] == 0) 
		{
			if(gObjMoveGate(aIndex, g_DuelGates[lpMsg->btRoomIndex].LearnerGate))
			{
				g_DuelRoom.m_Zone[lpMsg->btRoomIndex].SpectatorIndex[i] = aIndex;
			
				GCSendDuelSpectatorList(&gObj[aIndex], lpMsg->btRoomIndex);
				GCSendDuelSpectatorAdd(i, lpMsg->btRoomIndex);
				
				gObjAddBuffEffect(&gObj[aIndex], AT_INVISIBILITY, 0, 0, 0, 0, -10);
				gObjAddBuffEffect(&gObj[aIndex], AT_NEWPVPSYSTEM_WATCH_DUEL, 0, 0, 0, 0, -10);
				gObjAddBuffEffect(&gObj[aIndex], AT_CW_NPC_TRANSPARENCY, 0, 0, 0, 0, -10);
				gObjViewportListProtocolDestroy(&gObj[aIndex]);

				GCSendDuelLifebarInit(&gObj[aIndex], lpMsg->btRoomIndex);
				GCSendDuelLifebarStatus(&gObj[aIndex], lpMsg->btRoomIndex);

				GCSendDuelScore(g_DuelRoom.m_Zone[lpMsg->btRoomIndex].UserIndex[0], g_DuelRoom.m_Zone[lpMsg->btRoomIndex].UserIndex[1],lpMsg->btRoomIndex);
				return;
			}	
			
			GCServerMsgStringSend(lMsg.Get(3424), aIndex, 1);
			return;
		}
	}
}

void CGDuelSpectateRemoveRecv(PMSG_REQ_SPECTATE_REMOVE_DUEL* lpMsg,int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if ( !gObjIsConnected(aIndex))
	{
		return;
	}
	
	gObjMoveGate(aIndex, 294);
}

void GCSendEndDuel(LPOBJ lpObj)
{
	if(lpObj == NULL)
	{
		return;
	}

	if(!gObjIsConnected(lpObj->m_Index))
	{
		return;
	}

	PMSG_SEND_END_DUEL pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x03;
	pMsg.unk = 0;

	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
}

void GCSendDuelSpectatorList(LPOBJ lpObj,BYTE btRoom)
{
	if(btRoom < 0 || btRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}
	
	if( g_DuelRoom.m_Zone[btRoom].UserIndex[0] == 0 ||
		g_DuelRoom.m_Zone[btRoom].UserIndex[1] == 0 )
	{
		return;
	}

	PMSG_SEND_DUEL_SPEC_LIST pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0B;
	pMsg.btCount = 0;

	if(g_DuelRoom.m_Zone[btRoom].btOpen == FALSE)
	{
		for(int i = 0; i < MAX_DUEL_SPECTATORS; i++)
		{
			if(g_DuelRoom.m_Zone[btRoom].SpectatorIndex[i] != 0)
			{
				memcpy(&pMsg.szName[pMsg.btCount++][0], gObj[g_DuelRoom.m_Zone[btRoom].SpectatorIndex[i]].Name, 10);
			}
		}
	}

	pMsg.h.size = 5 + (pMsg.btCount * 10);

	DataSend(lpObj->m_Index,(LPBYTE)&pMsg, pMsg.h.size);
}

void GCSendDuelSpectatorAdd(int iSpecIndex, BYTE btRoom)
{
	if(btRoom < 0 || btRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if(iSpecIndex < 0 || iSpecIndex >= MAX_DUEL_SPECTATORS)
	{
		return;
	}

	if(g_DuelRoom.m_Zone[btRoom].SpectatorIndex[iSpecIndex] == 0)
	{
		return;
	}

	if( g_DuelRoom.m_Zone[btRoom].UserIndex[0] == 0 ||
		g_DuelRoom.m_Zone[btRoom].UserIndex[1] == 0 )
	{
		return;
	}

	PMSG_SEND_DUEL_SPEC_ADD pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x08;

	memcpy(pMsg.szName,gObj[g_DuelRoom.m_Zone[btRoom].SpectatorIndex[iSpecIndex]].Name, 10);

	for(int i = 0; i < MAX_DUEL_SPECTATORS; i++)
	{
		if(i == iSpecIndex)
		{
			continue;
		}

		if(g_DuelRoom.m_Zone[btRoom].SpectatorIndex[i] != 0)
		{
			DataSend(g_DuelRoom.m_Zone[btRoom].SpectatorIndex[i],(LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void GCSendDuelSpectatorRemove(int iSpecIndex, BYTE btRoom)
{
	if(btRoom < 0 || btRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	if(iSpecIndex < 0 || iSpecIndex >= MAX_DUEL_SPECTATORS)
	{
		return;
	}

	if(g_DuelRoom.m_Zone[btRoom].SpectatorIndex[iSpecIndex] == 0)
	{
		return;
	}

	if( g_DuelRoom.m_Zone[btRoom].UserIndex[0] == 0 ||
		g_DuelRoom.m_Zone[btRoom].UserIndex[1] == 0 )
	{
		return;
	}


	PMSG_SEND_DUEL_SPEC_ADD pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0A;

	memcpy(pMsg.szName,gObj[g_DuelRoom.m_Zone[btRoom].SpectatorIndex[iSpecIndex]].Name, 10);

	for(int i = 0; i < MAX_DUEL_SPECTATORS; i++)
	{
		if(i == iSpecIndex)
		{
			continue;
		}

		if(g_DuelRoom.m_Zone[btRoom].SpectatorIndex[i] != 0)
		{
			DataSend(g_DuelRoom.m_Zone[btRoom].SpectatorIndex[i],(LPBYTE)&pMsg, pMsg.h.size);
		}
	}

}

void GCSendDuelLifebarStatus(LPOBJ lpObj, BYTE btRoom)
{
	if(btRoom < 0 || btRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	LPOBJ lpFirstUser = &gObj[g_DuelRoom.m_Zone[btRoom].UserIndex[0]];
	LPOBJ lpSecordUser = &gObj[g_DuelRoom.m_Zone[btRoom].UserIndex[1]];

	if( lpFirstUser == NULL || lpSecordUser == NULL)
	{
		return;
	}

	PMSG_DUEL_LIFEBAR_REFILL pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x05;

	pMsg.btObjId01H = HIBYTE(lpFirstUser->m_Index);
	pMsg.btObjId01L = LOBYTE(lpFirstUser->m_Index);

	pMsg.btObjId02H = HIBYTE(lpSecordUser->m_Index);
	pMsg.btObjId02L = LOBYTE(lpSecordUser->m_Index);

	pMsg.btLife01 = lpFirstUser->Life / ((lpFirstUser->MaxLife + lpFirstUser->AddLife) / 100);
	pMsg.btLife02 = lpSecordUser->Life / ((lpSecordUser->MaxLife + lpSecordUser->AddLife) / 100);

	pMsg.btShield01 = lpFirstUser->iShield / ((lpFirstUser->iMaxShield + lpFirstUser->iAddShield) / 100);
	pMsg.btShield02 = lpSecordUser->iShield / ((lpSecordUser->iMaxShield + lpSecordUser->iAddShield) / 100);

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCSendDuelLifebarInit(LPOBJ lpObj, BYTE btRoom)
{
	if(btRoom < 0 || btRoom >= MAX_DUEL_ROOMS)
	{
		return;
	}

	LPOBJ lpFirstUser = &gObj[g_DuelRoom.m_Zone[btRoom].UserIndex[0]];
	LPOBJ lpSecordUser = &gObj[g_DuelRoom.m_Zone[btRoom].UserIndex[1]];

	if( lpFirstUser == NULL || lpSecordUser == NULL)
	{
		return;
	}

	PMSG_DUEL_LIFEBAR_NAME pMsg2;
	pMsg2.h.c = 0xC1;
	pMsg2.h.size = sizeof(pMsg2);
	pMsg2.h.headcode = 0xAA;
	pMsg2.h.subcode = 0x07;
	pMsg2.Type = 0x00;
	
	memcpy(pMsg2.szName1, lpFirstUser->Name, 10);
	memcpy(pMsg2.szName2, lpSecordUser->Name, 10);

	pMsg2.btObjId1H = HIBYTE(lpFirstUser->m_Index);
	pMsg2.btObjId1L = LOBYTE(lpFirstUser->m_Index);

	pMsg2.btObjId2H = HIBYTE(lpSecordUser->m_Index);
	pMsg2.btObjId2L = LOBYTE(lpSecordUser->m_Index);

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg2, pMsg2.h.size);

	PMSG_DUEL_LIFEBAR_INIT pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0D;

	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCSendDuelEndNotification(LPOBJ lpObj, char * Winner, char * Looser)
{

	PMSG_DUEL_FINISH pMsg;
	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x0C;
	memcpy(pMsg.szWinner, Winner, 10);
	memcpy(pMsg.szLooser, Looser, 10);

	DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}
#endif
void CGMapMoveRecv(PMSG_MAP_MOVE *lpMsg,int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}
	
	if( (GetTickCount() - gObj[aIndex].MySelfDefenseTime) < 60000 && g_EnableSelfDefense ) 
	{
		GCServerMsgStringSend(lMsg.Get(1133),aIndex,1);
		return;
	}

	if ( lpMsg->wMoveNum == NULL )
	{
		return;
	}
		
	if ( gObj[aIndex].Teleport != 0 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 68)), aIndex, 1);
		return;
	}

	if ( (gObj[aIndex].m_IfState.use) != 0 )
	{
		if (  gObj[aIndex].m_IfState.type  == 3 )
		{
			gObj[aIndex].TargetShopNumber = -1;
			gObj[aIndex].m_IfState.type = 0;
			gObj[aIndex].m_IfState.use = 0;
		}
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 68)), aIndex, 1);
		return;
	}

	if ( gObj[aIndex].IsInBattleGround != false )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(6, 68)), aIndex, 1);
		return;
	}

	BOOL bPlayerKiller = FALSE; 

#ifdef PARTYKILLPKSET
	if(gObj[aIndex].PartyNumber >= 0) 
	{
		if(gParty.GetPKPartyPenalty(gObj[aIndex].PartyNumber) >= 5)
		{
			bPlayerKiller = TRUE;
		}
	}
	else if(gObj[aIndex].m_PK_Level >= 5)
	{
		bPlayerKiller = TRUE;
	}
#else
	if(gObj[aIndex].m_PK_Level >= 5)
	{
		bPlayerKiller = TRUE;
	}
#endif

	if ( bPlayerKiller == TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 101)), aIndex, 1);
		return;
	}

	gMoveCommand.Move(&gObj[aIndex], lpMsg->wMoveNum);
}

//480AF0	-> 99% (Function use PMSG_MOVE_ROREN_MARKET * lpMsg)
void CGMoveRorenMarket(int aIndex) //-> Complete
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	if( !CHECK_LIMIT(aIndex, OBJMAX) )
	{
		return;
	}
	// ----
	if( lpObj->Teleport || ((lpObj->m_State >> 2) & 1) )
	{
		return;
	}
	// ----
	int iGateNumber;
	int nUserLevel;
	int nPermitLevel	= 0;
	int iMapNumber		= lpObj->MapNumber;
	int nUserClass		= lpObj->Class;
	// ----
	BYTE attr = MapC[iMapNumber].GetAttr(lpObj->X, lpObj->Y);
	// ----
	if( (attr & 1) == 1 )
	{
		if( iMapNumber == 79 )
		{
			if( nUserClass != CLASS_WIZARD && nUserClass != CLASS_KNIGHT && nUserClass != CLASS_ELF && nUserClass != CLASS_SUMMONER )
			{
				if( nUserClass == CLASS_MAGUMSA || nUserClass == CLASS_MONK || nUserClass == CLASS_DARKLORD )
				{
					nPermitLevel = 6;
				}
			}
			else
			{
				nPermitLevel = 10;
			}
			// ----
			if( lpObj->Level >= nPermitLevel )
			{
				iGateNumber = 17;
			}
			else
			{
				if( nUserClass != CLASS_WIZARD && nUserClass != CLASS_KNIGHT && nUserClass != CLASS_MAGUMSA && nUserClass != CLASS_DARKLORD )
				{
					if( nUserClass == CLASS_ELF )
					{
						iGateNumber = 27;
					}
					else
					{
						if( nUserClass == CLASS_SUMMONER )
						{
							iGateNumber = 267;
						}
						else
						{
							iGateNumber = 17;
						}
					}
				}
				else
				{
					iGateNumber = 17;
				}
			}
		}
		else
		{
			iGateNumber = 333;
		}
		// ----
		gObjMoveGate(lpObj->m_Index, iGateNumber);
	}
}
// -------------------------------------------------------------------------------

//480920	-> 99% (Function use PMSG_REQ_ATTDEF_POWER_INC * lpMsg)
void CGReqAttDefPowerInc(int aIndex)
{
	if( OBJMAX_RANGE(aIndex) && gObjIsConnected(aIndex) )
	{
		NpcShadowPhantom(aIndex);
	}
}
// -------------------------------------------------------------------------------

//004691a0	-> Maybe complete :D (must will be work)
void GCMonkMagicAttack(PMSG_MAGICATTACK * lpMsg, int aIndex)
{
	LPOBJ lpObj;
	LPOBJ lpTargetObj;
	int usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	CMagicInf* lpMagic;


	if( gObj[aIndex].CloseCount >= 0 )
		return;

	WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);

	if( MagicNumber == 263 || MagicNumber == 559 )
	{
		GCMonkMagicAttackNumberSend(&gObj[aIndex],MagicNumber,gObj[aIndex].m_wDarkSideTargetList[0],TRUE);
		return;
	}

	if( usernumber < 0 || usernumber > OBJMAX-1 )
	{
		LogAdd("error :%s %d %d",__FILE__,__LINE__,usernumber);
		return;
	}

	if( gObj[aIndex].Live == FALSE || 
		gObj[aIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	lpObj = &gObj[aIndex];
	lpTargetObj = &gObj[usernumber];

	int attackret = gObjCheckAttackArea(aIndex, usernumber);

	if( attackret != 0 )
	{
		//LogAddC(2, "[DEBUG] Point 1");
		//LogAddTD("[%s][%s] Try Attack In Not Attack Area [Protocol] (%d:%d,%d) errortype = %d",
		//	lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, attackret);

		if( lpObj->Class == 275 )
			lpObj->TargetNumber = -1;
		if( bIsIgnorePacketSpeedHackDetect != 0 )
			return;
	}

	if ( lpObj->Type == OBJ_USER )
	{
		lpMagic = gObjGetMagicSearch(lpObj, MagicNumber);

		if ( !lpMagic )
			return;

		if ( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->ChangeUP, lpObj->ThirdChangeUp , lpMagic->m_Skill) < 1 )
			return;

		if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == 0 )
		{
			LogAddC(2, "[0x4A] CGRageAttack() - Invalid Status");
			return;
		}

		if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			LogAddC(2, "[0x4A] CGRageAttack() - Invalid KillCount");
			return;
		}
	}
	else
	{
		lpMagic = gObjGetMagic(lpObj, MagicNumber);

		if ( !lpMagic )
			return;
	}

	if ( !gCheckSkillDistance(aIndex, usernumber, lpMagic->m_Skill))
	{
		DWORD dwTick = GetTickCount() - lpObj->m_dwSkillDistanceErrorTick;
		
		if ( dwTick > (g_iSkillDiatanceKickCheckTime*1000) )
		{
			lpObj->m_iSkillDistanceErrorCount = 0;
			lpObj->m_dwSkillDistanceErrorTick = GetTickCount();
		}

		lpObj->m_iSkillDistanceErrorCount++;

		if ( lpObj->m_iSkillDistanceErrorCount > g_iSkillDistanceKickCount )
		{
			if ( g_iSkillDistanceKick )
			{
				LogAddTD("[SKILL DISTANCE CHECK] [%s][%s] Kick Invalid Skill Area User. count(%d)",
					lpObj->AccountID, lpObj->Name, lpObj->m_iSkillDistanceErrorCount);

				CloseClient(lpObj->m_Index);
				return;
			}

			lpObj->m_iSkillDistanceErrorCount=0;
		}

		return;
	}
	GCMonkMagicAttackNumberSend(&gObj[aIndex],MagicNumber,usernumber,TRUE);
}
// -------------------------------------------------------------------------------

//004696b0	-> 95% (pAttack +9 is unknown byte)
void GCMonkMagicAttackNumberSend(LPOBJ lpObj, WORD MagicNumber, int usernumber, BYTE skillsuccess)
{
	/*PMSG_MONK_MAGICATTACK_SEND pAttack;
	// ----
	PHeadSetBE((LPBYTE)&pAttack, 0x4A, sizeof(pAttack));
	// ----
	pAttack.MagicNumberH	= SET_NUMBERH(MagicNumber);
	pAttack.MagicNumberL	= SET_NUMBERL(MagicNumber);
	pAttack.SourceNumberH	= SET_NUMBERH(lpObj->m_Index);
	pAttack.SourceNumberL	= SET_NUMBERL(lpObj->m_Index);
	pAttack.TargetNumberH	= SET_NUMBERH(usernumber);
	pAttack.TargetNumberL	= SET_NUMBERL(usernumber);
	pAttack.TargetNumberH	&= 0x7F;
	// ----
	if( skillsuccess )
	{
		pAttack.TargetNumberH |= 0x80;
	}
	// ----
	if( lpObj->Type == 1 )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);
	}
	// ----
	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);*/
	PMSG_MAGICATTACK_RESULT pAttack;
	PHeadSetBE((LPBYTE)&pAttack, 0x4A, sizeof(pAttack));

	pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
	pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);

	pAttack.SourceNumberH = SET_NUMBERH(lpObj->m_Index);
	pAttack.SourceNumberL = SET_NUMBERL(lpObj->m_Index);
	pAttack.TargetNumberH = SET_NUMBERH(usernumber);
	pAttack.TargetNumberL = SET_NUMBERL(usernumber);
	pAttack.TargetNumberH &= 0x7F;

	if ( skillsuccess )
		pAttack.TargetNumberH |= 0x80;

	if ( lpObj->Type == OBJ_USER )
		DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);
	
	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);
}
// -------------------------------------------------------------------------------

//004697c0	-> 100%
void GCMonkDarkSideTargetSelect(PMSG_MONK_DARKSIDE_RECV * lpMsg, int aIndex)
{
	WORD wMagicNumber	= MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);	//ebp-4
	WORD wTargetIndex	= MAKE_NUMBERW(lpMsg->TargetNumberH, lpMsg->TargetNumberL);	//ebp-8
	WORD wTargetList[5];	//ebp-14 (not declared before for???)
	// ----
	for( int i = 0; i < 5; i++ )	//ebp-18
	{
		wTargetList[i] = 10000;	//0x2710 (maybe user start or maxobj)
	}
	// ----
	if( gObj[aIndex].CloseCount >= 0 )
	{
		return;
	}
	// ----
	CMagicInf * lpMagic = 0;
	lpMagic				= gObjGetMagicSearch(&gObj[aIndex], wMagicNumber);
	// ----
	if( lpMagic == 0 )
	{
		return;
	}
	// ----
	if( !gObjUseSkill.SkillMonkDarkSideGetTargetIndex(aIndex, wTargetIndex, lpMagic, &wTargetList[0]) )
	{
		return;
	}
	// ----
	PMSG_MONK_DARKSIDE_SEND pDarkSideTargetList;
	memset(&pDarkSideTargetList, 0, sizeof(pDarkSideTargetList));
	PHeadSetB((LPBYTE)&pDarkSideTargetList, 0x4B, sizeof(pDarkSideTargetList));
	memcpy(pDarkSideTargetList.wTargetList, wTargetList, sizeof(wTargetList));
	pDarkSideTargetList.MagicNumber = wMagicNumber;
	gObj[aIndex].m_btDarkSideTargetNum = 0;
	// ----
	for( int i = 0; i < 5; i++ )
	{
		if( wTargetList[i] != 10000 )
		{
			++gObj[aIndex].m_btDarkSideTargetNum;
		}
	}
	// ----
	memcpy(gObj[aIndex].m_wDarkSideTargetList, wTargetList, sizeof(wTargetList));
	DataSend(aIndex, (LPBYTE)&pDarkSideTargetList, pDarkSideTargetList.h.size);
}
// -------------------------------------------------------------------------------

//004802e0	-> 100%
void CGReqEnterDoppelganger(PMSG_REQ_ENTER_DOPPELGANGER * lpMsg, int iIndex)
{
	if( !OBJMAX_RANGE(iIndex) )
	{
 		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}
	// ----
#ifdef DP
	g_DoppleganerEvent.EnterDoppelgangerEvent(iIndex, lpMsg->item_pos);
#endif
}
// -------------------------------------------------------------------------------

//00480370	-> 100%
void CGReqEnterZone(PMSG_REQ_ENTERZONE * lpMsg, int iIndex)
{
	if( !OBJMAX_RANGE(iIndex) )
	{
 		LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}
	// ----
	if( lpMsg->ukn04 == 1 )
	{
#ifdef IMPERIAL
		g_ImperialGuardian.CGEnterPortal(iIndex, 0);
#endif
	}
}
// -------------------------------------------------------------------------------

//004814d0	-> 100%
void CGPartyDelUserAsExitGameByForce(PMSG_PARTYDELUSER *lpMsg, int aIndex)
{
	int usernumber = -1;
	int dbnumber = -1;
	if ( gObj[aIndex].PartyNumber < 0 )
	{
		LogAdd("No Party %s %s", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	if ( !gParty.GetIndexUser(gObj[aIndex].PartyNumber, lpMsg->Number, usernumber, dbnumber) )
	{
		LogAdd("User Not found");
		return;
	}

	if ( usernumber < 0 )
	{
		LogAdd("User Not found");
		return;
	}

	int pnumber = gObj[aIndex].PartyNumber;
	if ( pnumber < 0 )
	{
		LogAdd("%s %d", __FILE__, __LINE__);
		return;
	}

	int count = gParty.GetCount(pnumber);
	if ( count < 1 )
	{
		LogAdd("%s %d", __FILE__, __LINE__);
		return;
	}

	if ( gObj[usernumber].lpGuild )
	{
		if ( gObj[usernumber].lpGuild->WarState == 1 && gObj[usernumber].IsInBattleGround )
		{
			GCServerMsgStringSend(lMsg.Get(3469), aIndex, 1);
			return;
		}

		if ( usernumber == gParty.m_PartyS[pnumber].Number[0] )
		{
			for (int i = 0; i < 5; ++i )
			{
				int n = gParty.m_PartyS[pnumber].Number[i];
				if ( n >= 0 && gObj[n].lpGuild && 
					gObj[n].lpGuild->WarState == 1 && gObj[n].IsInBattleGround )
				{
					GCServerMsgStringSend(lMsg.Get(3469), aIndex, 1);
					return;
				}
			}
		}
	}

	if ( lpMsg->Number == 0 || count <= 2 )
	{
		int nReallyConnectPartyMemberCount = gParty.GetReallyConnectPartyMemberCount(pnumber);
		if ( lpMsg->Number != 0 || nReallyConnectPartyMemberCount <= 1 )
		{
			if ( nReallyConnectPartyMemberCount <= 1 )
			{
				for (int j = 0; j < 5; ++j )
				{
					int n = gParty.m_PartyS[pnumber].Number[j];
					if ( n >= 0 )
					{
						gParty.Delete(pnumber, j);
						gObj[n].PartyNumber = -1;
						gObj[n].PartyTargetUser = -1;
						if ( gObj[n].Connected == 3 )
							GCPartyDelUserSend(n);
					}
				}
				gParty.Destroy(pnumber);
			}
		}
		else
		{
			if ( !gObj[aIndex].bTemporaryUser )
				gParty.Delete(pnumber, lpMsg->Number);

			gParty.UpdatePKPartyPanalty(pnumber);
			gParty.SwitchLeader(pnumber);
			CGPartyListAll(pnumber);
		}
	}	
}
// -------------------------------------------------------------------------------

struct PMSG_EXPAND_RESULT
{
	PBMSG_HEAD h;
	BYTE IsSuccess;
};

//00474330	-> 100%
void GCResultForExtendInvenOrWarehouse(int aIndex, BYTE byIsSuccess)
{
	PMSG_EXPAND_RESULT pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x2B, sizeof(pMsg));
	pMsg.IsSuccess = byIsSuccess;
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// -------------------------------------------------------------------------------
#ifdef NPVP
int OnCGInviteDuel(LPPMSG_REQ_DUEL_INVITE lpMsg, int aIndex)
{
#ifdef _CS
	return ENEWPVP::E_NOT_DUEL_CSSERVER;
#endif

	int nResponsor = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	
	LPOBJECTSTRUCT lpRequester = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( !PacketCheckTime(lpRequester) ) return ENEWPVP::E_LIMIT_PACKETTIME;
	if( nResponsor == aIndex )			return ENEWPVP::E_INVITE_MYSELF;
	if( gNonPK )
	{
		GCServerMsgStringSend(lMsg.Get(1198), aIndex, 1);
		return ENEWPVP::E_NON_PKSERVER;
	}
	if( gObj[aIndex].CloseType != -1 )	return ENEWPVP::E_CONNECTION_CLOSING;
	if( g_NewPVP.CheckPKPenalty(lpRequester) )
	{
		GCServerMsgStringSend(lMsg.Get(1199), aIndex, 1);
		return ENEWPVP::E_NOT_DUEL_MURDERER;
	}
	if( !gObjIsConnectedEx(nResponsor) )
	{
		LogAdd("error : %s %d (%d)", __FILE__, __LINE__, nResponsor);
	}
	LPOBJECTSTRUCT lpResponsor = (LPOBJECTSTRUCT)&gObj[nResponsor];
	if( g_NewPVP.CheckPKPenalty(lpResponsor) )
	{
		GCServerMsgStringSend(lMsg.Get(1200), aIndex, 1);
		return ENEWPVP::E_NOT_DUEL_MURDERER;
	}

	char szDuelName[MAX_IDSTRING+1] = {0};
	char szDuelName2[MAX_IDSTRING+1] = {0};

	memcpy(szDuelName, lpResponsor->Name, MAX_IDSTRING);
	szDuelName[MAX_IDSTRING] = '\0';

	memcpy(szDuelName2, lpMsg->szName, MAX_IDSTRING);
	szDuelName2[MAX_IDSTRING] = '\0';

	if( strcmp(szDuelName, szDuelName2) != 0 )
	{
		GCServerMsgStringSend(lMsg.Get(1186), aIndex, 1);
		return ENEWPVP::E_FAILED_RESULT;
	}
	if( !IsDuelEnable(lpResponsor->m_Index) )
	{
		GCServerMsgStringSend(lMsg.Get(1187), aIndex, 1);
		return ENEWPVP::E_FAILED_RESULT;
	}
	if( g_NewPVP.IsGuildWar(lpRequester) )
	{
		GCServerMsgStringSend(lMsg.Get(1188), aIndex, 1);
		return ENEWPVP::E_GUILDWAR;
	}
	if( g_NewPVP.IsGuildWar(lpResponsor) )
	{
		GCServerMsgStringSend(lMsg.Get(1189), aIndex, 1);
		return ENEWPVP::E_GUILDWAR;
	}
	if( g_NewPVP.IsSelfDefense(lpRequester) || g_NewPVP.IsSelfDefense(lpResponsor) )
	{
		GCServerMsgStringSend(lMsg.Get(1213), aIndex, 1);
		return ENEWPVP::E_SELFDEFENSE;
	}
	if( CC_MAP_RANGE(lpRequester->MapNumber) || CC_MAP_RANGE(lpResponsor->MapNumber) )
	{
		GCServerMsgStringSend(lMsg.Get(1231), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
	if( DS_MAP_RANGE(lpRequester->MapNumber) || DS_MAP_RANGE(lpResponsor->MapNumber) )
	{
		GCServerMsgStringSend(lMsg.Get(1204), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
	if( BC_MAP_RANGE(lpRequester->MapNumber) || BC_MAP_RANGE(lpResponsor->MapNumber) )
	{
		GCServerMsgStringSend(lMsg.Get(1190), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
	if( lpRequester->m_IfState.use > 0 )
	{
		GCServerMsgStringSend(lMsg.Get(1191), aIndex, 1);
		return ENEWPVP::E_FAILED_RESULT;
	}
	if( lpResponsor->m_IfState.use > 0 )
	{
		GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1192), lpResponsor->Name);
		return ENEWPVP::E_FAILED_RESULT;
	}
	int nRet = g_NewPVP.Reserve(*lpRequester, *lpResponsor);
	if( nRet != ENEWPVP::E_NO_ERROR )
	{
		switch( nRet )
		{
		case ENEWPVP::E_ALREADY_DUELRESERVED:
			GCServerMsgStringSend(lMsg.Get(1183), aIndex, 1);
			GCServerMsgStringSend(lMsg.Get(1185), aIndex, 1);
			break;
		case ENEWPVP::E_ALREADY_DUELLING:
			GCServerMsgStringSend(lMsg.Get(1184), aIndex, 1);
			GCServerMsgStringSend(lMsg.Get(1185), aIndex, 1);
			break;
		case ENEWPVP::E_ALREADY_DUELREQUESTED_1:
			GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1193), lpResponsor->Name);
			break;
		case ENEWPVP::E_ALREADY_DUELRESERVED_1:			
			GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1194), lpResponsor->Name);
			break;
		case ENEWPVP::E_ALREADY_DUELLING_1:
			GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1195), lpResponsor->Name);
			break;
		}
		return nRet;
	}
	
	GCServerMsgStringSendEx(aIndex, 1, lMsg.Get(1196), lpResponsor->Name);
	LogAddTD("[Duel] [%s][%s] Requested to Start Duel to [%s][%s]", lpRequester->AccountID, lpRequester->Name, lpResponsor->AccountID, lpResponsor->Name);
	return ENEWPVP::E_NO_ERROR;
}

int OnCGAnswerDuel(LPPMSG_ANS_DUEL_ANSWER lpMsg, int aIndex)
{
#ifdef _CS
	return ENEWPVP::E_NOT_DUEL_CSSERVER;
#endif

	LPOBJECTSTRUCT lpRequester = NULL;
	LPOBJECTSTRUCT lpResponsor = (LPOBJECTSTRUCT)&gObj[aIndex];
	int nRequester = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);

	if( !gObjIsConnectedEx(nRequester) )	return ENEWPVP::E_DISCONNECT;
	lpRequester = (LPOBJECTSTRUCT)&gObj[nRequester];

	PMSG_ANS_DUEL_INVITE res = {0};
	res.h.c = PMHC_BYTE;
	res.h.size = sizeof(PMSG_ANS_DUEL_INVITE);
	res.h.headcode = 0xAA;
	res.h.subcode = 0x01;	
	memcpy(res.szName, lpResponsor->Name, MAX_IDSTRING);
	if( lpResponsor->CloseType != -1 )
	{
		g_NewPVP.Cancel(*lpRequester, *lpResponsor, FALSE);
		res.nResult = ENEWPVP::E_CONNECTION_CLOSING;
		DataSend(aIndex, (LPBYTE)&res, res.h.size);
		return ENEWPVP::E_CONNECTION_CLOSING;
	}
	if( !lpMsg->bDuelOK )
	{
		g_NewPVP.Cancel(*lpRequester, *lpResponsor, FALSE);
		res.nResult = ENEWPVP::E_REFUSE_INVATE;
		DataSend(aIndex, (LPBYTE)&res, res.h.size);
		return ENEWPVP::E_REFUSE_INVATE;
	}
	if( BC_MAP_RANGE(lpRequester->MapNumber) ||
		CC_MAP_RANGE(lpRequester->MapNumber) ||
		DS_MAP_RANGE(lpRequester->MapNumber) )
	{
		g_NewPVP.Cancel(*lpRequester, *lpResponsor, FALSE);
		GCServerMsgStringSend(lMsg.Get(1231), lpResponsor->m_Index, 1);
		GCServerMsgStringSend(lMsg.Get(1231), lpRequester->m_Index, 1);
		res.nResult = ENEWPVP::E_INVALID_MAP;
		DataSend(lpResponsor->m_Index, (LPBYTE)&res, res.h.size);
		DataSend(lpRequester->m_Index, (LPBYTE)&res, res.h.size);
		return ENEWPVP::E_INVALID_MAP;
	}
	int nRet = g_NewPVP.Join(*lpRequester, *lpResponsor);
	if( nRet != ENEWPVP::E_NO_ERROR )
	{
		switch( nRet )
		{
		case ENEWPVP::E_ALREADY_DUELRESERVED_1:
			GCServerMsgStringSend(lMsg.Get(1197), aIndex, 1);
			break;
		case ENEWPVP::E_ALREADY_DUELLING_1:
			GCServerMsgStringSend(lMsg.Get(1184), aIndex, 1);
			GCServerMsgStringSend(lMsg.Get(1185), aIndex, 1);
			break;
		}
		res.nResult = nRet;
		DataSend(lpRequester->m_Index, (LPBYTE)&res, res.h.size);
		DataSend(lpResponsor->m_Index, (LPBYTE)&res, res.h.size);
		g_NewPVP.Cancel(*lpRequester, *lpResponsor, FALSE);
		return nRet;
	}
	return ENEWPVP::E_NO_ERROR;
}

int OnCGLeaveDuel(LPPMSG_REQ_DUEL_EXIT lpMsg, int aIndex)
{
#ifdef _CS
	return ENEWPVP::E_NOT_DUEL_CSSERVER;
#endif

	if( gObj[aIndex].CloseType != -1 )	return ENEWPVP::E_DISCONNECT;
	int nRet = g_NewPVP.Leave(gObj[aIndex]);
	if( nRet != ENEWPVP::E_NO_ERROR )	return nRet;
	return ENEWPVP::E_NO_ERROR;
}

int OnDuelChannelJoin(LPPMSG_REQ_DUEL_JOINCNANNEL lpMsg, int aIndex)
{
	if( gObj[aIndex].CloseType != -1 )	return ENEWPVP::E_DISCONNECT;
	int nRet = g_NewPVP.JoinChannel(lpMsg->nChannelId, gObj[aIndex]);
	if( nRet != ENEWPVP::E_NO_ERROR )	return nRet;
	return ENEWPVP::E_NO_ERROR;
}

int OnDuelChannelLeave(LPPMSG_REQ_DUEL_LEAVECNANNEL lpMsg, int aIndex)
{
	if( gObj[aIndex].CloseType != -1 )	return ENEWPVP::E_DISCONNECT;
	int nRet = g_NewPVP.LeaveChannel(lpMsg->nChannelId, gObj[aIndex]);
	if( nRet != ENEWPVP::E_NO_ERROR )	return nRet;
	return ENEWPVP::E_NO_ERROR;
}
#endif

#ifdef __CUSTOMS__
void TargetDataRequest(int aIndex, PMSG_TARGETDATA_REQ * Request)
{
	if( !OBJMAX_RANGE(aIndex) )
	{
		return;
	}
	// ----
	PMSG_TARGETDATA_ANS pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xFB, 7, sizeof(pMsg));
	// ----
	if( gObj[Request->aIndex].Live == 0 )
	{
		return;
	}
	// ----
	pMsg.TargetLifePercent	= (gObj[Request->aIndex].Life / (gObj[Request->aIndex].MaxLife + gObj[Request->aIndex].AddLife)) * 100.0f;
	// ----
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void UpdateCharInfo(int aIndex)
{
	if( !OBJMAX_RANGE(aIndex) )
	{
		return;
	}
	// ----
	CHAR_UPDATEINFO pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 8, sizeof(CHAR_UPDATEINFO));
	// ----
	pRequest.Strength		= gObj[aIndex].Strength;
	pRequest.Dexterity		= gObj[aIndex].Dexterity;
	pRequest.Vitality		= gObj[aIndex].Vitality;
	pRequest.Energy			= gObj[aIndex].Energy;
	pRequest.Leadership		= gObj[aIndex].Leadership;
	pRequest.LevelUpPoint	= gObj[aIndex].LevelUpPoint;
	// ----
	DataSend(aIndex, (LPBYTE)&pRequest, sizeof(pRequest));
}
#endif


//1.01.03
void GCReqHacktoolStatistics(LPOBJ lpObj, char * pchFilename, DWORD dwKey)
{
	if( !lpObj )
	{
		return;
	}
	// ----
	if( lpObj->Type == 1 && lpObj->Connected == 3 )
	{
		//g_HackToolBlock.HacktoolStatistics(lpObj, pchFilename);
	}
}

//1.01.03
void CGAnsHacktoolStatistics(PMSG_ANS_HACKTOOL_STATISTICS * lpMsg, int iIndex)
{
	OBJECTSTRUCT * lpObj = &gObj[iIndex];
	// ----
	if( !lpObj )
	{
		return;
	}
	// ----
	if( lpObj->Type == 1 && lpObj->Connected == 3 )
	{
		g_HacktoolBlockEx.OnRequestUserHacktoolUseInfo(lpObj, lpMsg->dwKey, lpMsg->btResult);
	}
}

//1.01.03
void CGInventoryEquipment(_tagPMSG_REQ_INVENTORY_EQUIPMENT_ITEM * lpMsg, int iIndex)
{
	if( !OBJMAX_RANGE(iIndex) )
	{
		return;
	}
	// ----
	LPOBJ lpObj = &gObj[iIndex];
	// ----
	if( !gObjIsConnected(iIndex) )
	{
		return;
	}
	// ----
	_tagPMSG_ANS_INVENTORY_EQUIPMENT_ITEM pMsg = { 0 };
	pMsg.btResult = 0;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x20, sizeof(_tagPMSG_ANS_INVENTORY_EQUIPMENT_ITEM));
	pMsg.btItemPos = lpMsg->btItemPos;
	// ----
	if( lpObj->pInventory[lpMsg->btItemPos].m_Durability == 0.0 )
	{
		pMsg.btResult = -1;
		DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}
	// ----
	switch( lpObj->pInventory[lpMsg->btItemPos].m_Type )
	{
	case ITEMGET(13, 128):
	case ITEMGET(13, 129):
		{
			if( !lpObj->m_btSculptPos )
			{
				if( lpMsg->btValue == 254 )
				{
					if( lpObj->pInventory[lpMsg->btItemPos].m_Durability == 255.0 )
					{
						lpObj->m_btSculptPos = lpMsg->btItemPos;
						lpObj->pInventory[lpMsg->btItemPos].m_Durability = 254.0;
						pMsg.btResult = -2;
					}
				}
				break;
			}
			// ----
			if(		lpObj->m_btSculptPos < INVETORY_WEAR_SIZE 
				||	lpObj->m_btSculptPos >= MAIN_INVENTORY_SIZE )
			{
				break;
			}
			// ----
			if(		lpMsg->btValue			== 255
				&&	lpObj->m_btSculptPos	== lpMsg->btItemPos )
			{
				if( lpObj->pInventory[lpMsg->btItemPos].m_Durability == 254.0 )
				{
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 255.0;
					pMsg.btResult = -1;
				}
				break;
			}
			// ----
			GCServerMsgStringSend(lMsg.Get(3480), lpObj->m_Index, 1);
            DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
		break;
		// --
	case ITEMGET(13, 130):
	case ITEMGET(13, 131):
	case ITEMGET(13, 132):
	case ITEMGET(13, 133):
		{
			if( !lpObj->m_btCharmPos )
			{
				if( lpMsg->btValue == 254 )
				{
					if( lpObj->pInventory[lpMsg->btItemPos].m_Durability == 255.0 )
					{
						lpObj->m_btCharmPos = lpMsg->btItemPos;
						lpObj->pInventory[lpMsg->btItemPos].m_Durability = 254.0;
						pMsg.btResult = -2;
					}
				}
				break;
			}
			// ----
			if(		lpObj->m_btCharmPos < INVETORY_WEAR_SIZE 
				||	lpObj->m_btCharmPos >= MAIN_INVENTORY_SIZE )
			{
				break;
			}
			// ----
			if(		lpMsg->btValue		== 255
				&&	lpObj->m_btCharmPos == lpMsg->btItemPos )
			{
				if( lpObj->pInventory[lpMsg->btItemPos].m_Durability == 254.0 )
				{
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 255.0;
					pMsg.btResult = -1;
				}
				break;
			}
			// ----
			GCServerMsgStringSend(lMsg.Get(3480), lpObj->m_Index, 1);
            DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
		break;
		// --
	case ITEMGET(13, 134):
		{
			if( !lpObj->m_btArtifactPos )
			{
				if( lpMsg->btValue == 254 )
				{
					if( lpObj->pInventory[lpMsg->btItemPos].m_Durability == 255.0 )
					{
						lpObj->m_btArtifactPos = lpMsg->btItemPos;
						lpObj->pInventory[lpMsg->btItemPos].m_Durability = 254.0;
						pMsg.btResult = -2;
					}
				}
				break;
			}
			// ----
			if(		lpObj->m_btArtifactPos < INVETORY_WEAR_SIZE 
				||	lpObj->m_btArtifactPos >= MAIN_INVENTORY_SIZE )
			{
				break;
			}
			// ----
			if(		lpMsg->btValue			== 255
				&&	lpObj->m_btArtifactPos	== lpMsg->btItemPos )
			{
				if( lpObj->pInventory[lpMsg->btItemPos].m_Durability == 254.0 )
				{
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 255.0;
					pMsg.btResult = -1;
				}
				break;
			}
			// ----
			GCServerMsgStringSend(lMsg.Get(3480), lpObj->m_Index, 1);
            DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
		break;
	}
	// ----
	gObjCalCharacter(iIndex);
	DataSend(iIndex,(LPBYTE)&pMsg, pMsg.h.size);
}

void DisconnectEx(DISCONNECTEX * lpData)
{
	int UserIndex = gObjGetIndexByAccount(lpData->AccountID);
	// ----
	if( UserIndex == -1 )
	{
		return;
	}
	// ----
	LPOBJ lpUser = &gObj[UserIndex];
	// ----
	if( strcmp(lpUser->m_Password, lpData->Password) )
	{
		return;
	}
	// ----
#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
	g_ConnectEx.SendClose(UserIndex);
#endif
	CloseClient(UserIndex);
}
