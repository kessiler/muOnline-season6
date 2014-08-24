#include "StdAfx.h"
#include "DBSockMng.h"
#include "GameMain.h"
#include "GuildClass.h"
#include "logproc.h"
#include "DSProtocol.h"
#include "user.h"
#include "BuffEffectSlot.h"
#include "..\common\winutil.h"
#include "..\common\ReadScript.h"
#include "GensSystem.h"
#include "GensSystemProtocol.h"
// -------------------------------------------------------------------------

#ifdef GENS

int StartContribution					= 20;
int RequiredLevel						= 50;
int RequiredReset						= 0;
// -------------------------------------------------------------------------

char *szInfluence[] = { "NONE_INFLUENCE", "Duprian", "Vanert"};
// -------------------------------------------------------------------------

PMSG_GENS_MEMBER_VIEWPORT_INFO		pGensMsg;
PMSG_SEND_GENS_MEMBER_VIEWPORT		pGensCount;
GensSystem							gGensSystem;
// -------------------------------------------------------------------------

unsigned char						lpViewportAdd[MAX_VIEWPORT_INFO];
// -------------------------------------------------------------------------

GensSystem::GensSystem() 
{
	this->ChatPrice = 0;
}
// -------------------------------------------------------------------------

GensSystem::~GensSystem() 
{
	// ----
}
// -------------------------------------------------------------------------

//-> 00508140
//-> Completed
//-> Calls: 0043910A (JGGetCharacterInfo)
//-> Fine
int GensSystem::GDReqAbusingInfo(LPOBJ lpObj)
{
	if( !OBJMAX_RANGE(lpObj->m_Index) )
	{
		return false;
	}
	// ----
	if( gObjIsConnected(lpObj) )
	{
		PMSG_REQ_ABUSING_INFO pMsg;		
		// ----
		PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));
		// ----
		memcpy(pMsg.Name, lpObj->Name, 10);
		pMsg.bIndexH = SET_NUMBERH(lpObj->m_Index);
		pMsg.bIndexL = SET_NUMBERL(lpObj->m_Index);
		// ----
		pMsg.bResult = 9;
		// ----
		if( wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size) )
		{
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------

//-> 00508140
//-> Completed
//-> Calls: 004390FC (JGGetCharacterInfo), 
//005CC113 (ManagementProc), 005CC145 (ManagementProc),
// 1 Local call
//-> Fine
int GensSystem::ReqExDBGensInfo(LPOBJ lpObj)
{
	if( !OBJMAX_RANGE(lpObj->m_Index) ) 
	{
		return false;
	}
	// ----
	if( gObjIsConnected(lpObj) )
	{
		PMSG_REQ_ABUSING_INFO pMsg;			
		// ----
		PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x1, sizeof(pMsg));
		// ----
		pMsg.bResult = 0;
		memcpy(pMsg.Name, lpObj->Name, 10);
		pMsg.bIndexH = SET_NUMBERH(lpObj->m_Index);
		pMsg.bIndexL = SET_NUMBERL(lpObj->m_Index);
		// ----
		if( cDBSMng.Send((PCHAR)&pMsg, pMsg.h.size) )
		{
			return true;
		}
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 00508310
//-> Completed
//-> Calls: 00480627 (CGReqRegGensMember)
//-> Fine
void GensSystem::ReqRegGensMember(LPOBJ lpObj, unsigned char btInfluence)
{
	//-----
	_GUILD_INFO_STRUCT *lpGuildInfo = lpObj->lpGuild;
	//-----
	if (!OBJMAX_RANGE(lpObj->m_Index)) return;
	// ----
	if (gObjIsConnected(lpObj->m_Index))
	{
		if (btInfluence == VANERT_INFLUENCE || btInfluence == DUPRIAN_INFLUENCE) 
		{
			//-----
			PMSG_REQ_REG_GENS_MEMBER_EXDB pMsg;		
			//-----
	
			if (btInfluence == VANERT_INFLUENCE || btInfluence == DUPRIAN_INFLUENCE)
			{
				PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x03, sizeof(pMsg));
				//-----
				memcpy(pMsg.Name, lpObj->Name, sizeof(lpObj->Name));	
				//-----
			 	pMsg.bMessage = 0;
				//-----
				pMsg.bInfluence = btInfluence;
				//-----
				if (lpGuildInfo)
				{
					pMsg.wNumberH = SET_NUMBERHW(lpGuildInfo->Number);
					pMsg.wNumberL = SET_NUMBERLW(lpGuildInfo->Number);
				}
				//-----
				pMsg.bIndexH = SET_NUMBERH(lpObj->m_Index);
				pMsg.bIndexL = SET_NUMBERL(lpObj->m_Index);				//-----
				if( cDBSMng.Send((char*)&pMsg, pMsg.h.size) ) return;
			}
		}
		else
		{
			LogAddTD("[GensSystem] - Error - %s %s Line: %d", "btInfluence", __FILE__, __LINE__);       
		}
	}
}
// -------------------------------------------------------------------------

//-> 005084d0
//-> Completed
//-> Calls: -
//-> Fine
int GensSystem::AnsRegGensMember(int iObjIndex, unsigned char btResult)
{
	return 0;
}
// -------------------------------------------------------------------------

//-> 00508500
//-> Completed
//-> Calls: 0048072D (CGReqSecedeGensMember)
//-> Fine
bool GensSystem::IsInfluenceNPC(LPOBJ lpObj)
{
	//Fixed 13.10.2012, TargetNumber -> TargetShopNumber
	//TargetNumber		= +1108
	//TargetShopNumber	= +1110
	//a1 + 1110
	return (lpObj->TargetShopNumber == 543 && this->GetGensInfluence(lpObj) == VANERT_INFLUENCE 
		    || lpObj->TargetShopNumber == 544 && this->GetGensInfluence(lpObj) == DUPRIAN_INFLUENCE); 
}
// -------------------------------------------------------------------------

//-> 00508580
//-> Completed
//-> Calls: 00480790 (CGReqSecedeGensMember)
//-> Fine
int GensSystem::ReqSecedeGensMember(LPOBJ lpObj)
{
	if (gObjIsConnected(lpObj->m_Index))
	{
		PMSG_REQ_SECEDE_GENS_MEMBER_EXDB pMsg;
		// ----
		PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x4, sizeof(pMsg)); 
		// ----
		pMsg.bResult = 0; //[GensSystem] [%s][%s] Gens Request - Leave [%s]
		// ----
		pMsg.bIndexH = SET_NUMBERH(lpObj->m_Index);
		//-----
		pMsg.bIndexL = SET_NUMBERL(lpObj->m_Index);
		// ----
		memcpy(pMsg.Name, lpObj->Name, sizeof(lpObj->Name));
		// ----
		if (cDBSMng.Send((char*)&pMsg, sizeof(pMsg))) return true;
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 00508680
//-> Completed
//-> Calls: 0048A629 (DGAnsGensInfo), 0058F4B2 (gObjViewportListProtocolCreate)
//-> Fine
int GensSystem::SendGensInfo(LPOBJ lpObj)
{
	if (!OBJMAX_RANGE(lpObj->m_Index))
	 return 0;

	if (gObjIsConnected(lpObj))
	{
		PMSG_SEND_GENS_INFO pMsg;

		PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x7, sizeof(pMsg));
		
		pMsg.bInfluence				= this->GetGensInfluence(lpObj);
		pMsg.iGensRanking			= lpObj->m_GensRanking;
		pMsg.iGensClass				= lpObj->m_GensClass;
		pMsg.iContributePoint		= lpObj->m_ContributePoint;
		pMsg.iNextContributePoint	= this->GetNextContributePoint(lpObj);

		if (DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size))
			return true;
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 00508770
//-> Completed
//-> Calls: -
//-> Fine
int GensSystem::AnsSecedeGensMember(int iObjIndex, BYTE btResult)
{
	return 0;
}
// -------------------------------------------------------------------------

//-> 005087a0
//-> Completed
//-> Calls: -
//-> Fine
int GensSystem::SendPkPenaltyMapMove(LPOBJ lpObj)
{
	if (!OBJMAX_RANGE(lpObj->m_Index))
 	return 0;

	if (gObjIsConnected(lpObj))
	{
		BYTE pMsg[4] = { 0xC1, 0x04, 0xF8, 0x08 };
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg[1]);
		return true;
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 00508840
//-> Completed
//-> Calls: 00439B97 (GJSetCharacterInfo), 00489FF9 (DGAnsRegGensMember),
//005094B8 (AddContributePoint), 00509521 (SubContributePoint),
//0050A3A1 (AbusingPenalty)
//-> Fine
int GensSystem::GDReqSaveContributePoint(LPOBJ lpObj)
{
	if (!OBJMAX_RANGE(lpObj->m_Index))
		return false;

	//if (gObjIsConnected(lpObj))
	//{
		if (this->IsRegGensInfluence(lpObj))
		{
			PMSG_REQ_SAVE_CONTRIBUTE_POINT_EXDB pMsg;
			// ----
			//PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));
			//pMsg.subcode			= 0x07;
			PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x7, sizeof(pMsg));
			// ----
			memcpy(pMsg.Name, lpObj->Name, 0x0A);
			pMsg.Index				= lpObj->m_Index;
			pMsg.Influence			= this->GetGensInfluence(lpObj);
			pMsg.iContributePoint	= this->GetContributePoint(lpObj);
			pMsg.iGensClass			= this->GetGensClass(lpObj);
			// ----
			//if( wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size) )
			if( cDBSMng.Send((char*)&pMsg, pMsg.h.size) )
			{
				return true;
			}
		}
	//}
	return false;
}
// -------------------------------------------------------------------------

//-> 00508950
//-> Completed
//-> Calls: 00439BA5 (GJSetCharacterInfo)
//-> Fine
int GensSystem::DBSaveAbusingKillUserName(LPOBJ lpObj)
{
	if (!OBJMAX_RANGE(lpObj->m_Index))
		return false;

	if (gObjIsConnected(lpObj))
	{
		if (this->IsRegGensInfluence(lpObj))
		{
			EnterCriticalSection(&criti);

			PMSG_REQ_SAVE_ABUSING_KILLUSER_EXDB pMsg;

			pMsg.bResult = 0;

			for (int i = 0; i < 10; ++i)
			{
			 pMsg.KillName[i * 11] = 0;

				pMsg.KillCount[i] = lpObj->m_KillUserCnt;

				memcpy(&pMsg.KillName[i * 11], lpObj->Name, 0x0A); 
			}

			memcpy(pMsg.AccountID, lpObj->AccountID, 0x0A);
			
			memcpy(pMsg.Name, lpObj->Name, 0x0A);

			PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));

			pMsg.bResult = 8;

			LeaveCriticalSection(&criti);
			
			if (wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size))
				return true;
		}
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 00508b60
//-> Completed
//-> Calls: 004479B0 (PChatProc)
//-> Fine
void GensSystem::BattleZoneChatMsgSend(LPOBJ obj, LPBYTE Msg, int size)
{
	if( this->IsMapBattleZone(obj->MapNumber) )
	{
		int iInfluence = this->GetGensInfluence(obj);
		// -----
		for( int n = 0; n < 75; n++ )
		{
			if( obj->VpPlayer2[n].type == OBJ_USER && obj->VpPlayer2[n].state == PLAYER_CONNECTED )
			{
				if( this->GetGensInfluence(&gObj[obj->VpPlayer2[n].number]) == iInfluence )
				{
					DataSend(obj->VpPlayer2[n].number, Msg, size);
				}
			}
		}
	}
}
// -------------------------------------------------------------------------

//-> 00508d00
//-> Completed
//-> Calls: 005d2a30 (Move@CMoveCommand)
//-> Fine
signed int GensSystem::IsMoveMapBattleZone(int iMapIndex)
{
	if (gPkLimitFree)
	{
		/*switch (iMapIndex)
		{
		case 11:
		case 12:
		case 13:
		case 25:
		case 27:
		case 28:
		case 29:
		case 33:
		case 42:
		case 45:
			return 1;
		default:
			break;
		}*/
		for( int i = 0; i < MAX_BATTLE_ZONE; i++ )
		{
			if( iMapIndex == this->BattleMapList[i] )
			{
				return true;
			}
		}
	}
	// ----
	return !gPkLimitFree && iMapIndex == 42;
}
// -------------------------------------------------------------------------

//-> 00508dc0
//-> Completed
//-> Calls: 00489D04 (DGAnsSecedeGensMember), 00489FBB (DGAnsRegGensMember)
//DGAnsGensInfo (0048A55D), 0058F58B (gObjViewportListProtocolCreate)
//-> Fine
void GensSystem::GensViewportListProtocol(LPOBJ lpObj)
{
	unsigned char btInfluence;

	int iGensCount = 0;
	
	int iViewportSize = 0x06;

	memset(&lpViewportAdd, 0, MAX_VIEWPORT_INFO);

	if ( gObjCheckUsedBuffEffect(lpObj, AT_INVISIBILITY) != CLEAR_TYPE_DIEREGEN )
		//(lpObj->Authority & 32) != 32 || 
	{
		pGensMsg.btInfluence		= this->GetGensInfluence(lpObj);
		btInfluence					= pGensMsg.btInfluence;
		pGensMsg.NumberH			= HIBYTE(lpObj->m_Index);
		pGensMsg.NumberL			= LOBYTE(lpObj->m_Index);
		pGensMsg.iGensRanking		= lpObj->m_GensRanking;
		pGensMsg.iGensClass			= this->GetGensClass(lpObj);
		pGensMsg.iContributePoint	= this->GetContributePoint(lpObj);
		// ----
		memcpy((BYTE*)lpViewportAdd + iViewportSize, &pGensMsg, 16);
		// ----
		iViewportSize				+= 16; 
		++iGensCount;
		// ----
		PHeadSubSetW((LPBYTE)&pGensCount, 0xF8, 0x05, iViewportSize); 	
		pGensCount.Count = iGensCount;
		memcpy((BYTE*)lpViewportAdd, &pGensCount, 6);

		for (int n = 0; n < MAX_VIEWPORT; ++n)
		{
			if (lpObj->VpPlayer[n].type == OBJ_USER)
			{
				switch (lpObj->VpPlayer[n].state)
				{
				case 1:
				case 2:
				case 3:
					{
				 	int tObjNum = lpObj->VpPlayer[n].number;

					 if (tObjNum > 0)
					 {
			  		DataSend(tObjNum, (LPBYTE)&lpViewportAdd, iViewportSize);
				 	}				
					}
					break;
				}
			}
		}
	}
}
// -------------------------------------------------------------------------

//-> 00509040
//-> Completed
//-> Calls:
// OK	- 0041F77F -> (gObjGuardSearchEnemy)
// OK	- 00447990 -> (PChatProc)
// OK	- 004482F8 -> (CGChatWhisperRecv)
// OK	- 00454D22 -> (CGTalkRequestRecv)
// OK	- 0045E819 -> (CGPartyRequestRecv)
//		- 00471DBF -> (CGUseItemRecv)
// OK	- 0048A64C -> (DGAnsGensInfo)
// OK	- 00508B7B -> (BattleZoneChatMsgSend)
// OK	- 00509219 -> (IsPkEnable)
// OK	- 005095AB -> (PkPenaltyAddNeedZenMapMove)
// OK	- 00509629 -> (PkPenaltyDropInvenItem)
// OK	- 00509869 -> (PkPenaltyDropZen)
// OK	- 005099F6 -> (SendPKPenaltyDebuff)
// OK	- 00509A5F -> (CalcContributePoint)
// OK	- 00509A77 -> (CalcContributePoint)
// OK	- 005136B4 -> (Attack@CObjAttack)
// OK	- 00517A9A -> (Attack@CObjAttack)
// OK	- 00517AB4 -> (Attack@CObjAttack)
// OK	- 00528240 -> (SkillRecallParty)
// NONE - 00531C6D -> (SkillMonkDarkSideGetTargetIndex)
//		- 0055362C -> (MasterLevelUp)
// OK	- 0056A14D -> (gObjPlayerKiller)
// OK	- 0056A167 -> (gObjPlayerKiller)
// OK	- 0056AC34 -> (gObjUserDie)
// OK	- 0056AF58 -> (gObjUserDie)
// OK	- 0056AF73 -> (gObjUserDie)
//		- 0057385E -> (gObjMonsterExpSingle)
// OK	- 005923CB -> (gObjMoveGate)
// NONE - 005A3EB2 -> (gIsMoveCharmEnable)
//-> ~Fine
signed int GensSystem::IsMapBattleZone(int iMapIndex)
{
	if (gNonPK)
	{
		return FALSE;
	}

	if (gPkLimitFree)
	{
		/*switch (iMapIndex)
		{
		case 7:

		case 33:

		case 37:

		case 56:

		case 63:
			return TRUE;
			break;

		default:
		break;
		}*/
		for( int i = 0; i < MAX_BATTLE_ZONE; i++ )
		{
			if( iMapIndex == this->BattleZoneList[i] )
			{
				return true;
			}
		}
	}
	// ----
	return (!gPkLimitFree && iMapIndex == 63);
}
// -------------------------------------------------------------------------

//-> 00509130
//-> Completed
//-> Calls: 00489C2E (DGAnsSecedeGensMember), 00489F78 (DGAnsRegGensMember),
//0048A4D9 (DGAnsGensInfo), 0048A59A (DGAnsGensInfo)
//-> Fine
void GensSystem::SetGensInfluence(LPOBJ lpObj, int iInfluence)
{
	if (iInfluence != VANERT_INFLUENCE && iInfluence != DUPRIAN_INFLUENCE && iInfluence != LEAVED_INFLUENCE)
	{
		LogAddTD("[GensSystem] - Error - [%s][%s] Influence[%d] %s %d",	
			lpObj->AccountID, lpObj->Name, iInfluence, __FILE__, __LINE__);
	}
	else
	{
		if (iInfluence == LEAVED_INFLUENCE) iInfluence = NONE_INFLUENCE;

		lpObj->m_GensInfluence = iInfluence;
	}
}
// -------------------------------------------------------------------------

//-> 005091e0
//-> Completed
//-> Calls: 00513050 (Attack@CObjAttack)
//-> Fine
signed int GensSystem::IsPkEnable(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if( this->IsMapBattleZone(lpObj->MapNumber) )
	{
		if( lpObj->Type != 1 || lpTargetObj->Type != 1 )
		{
			return this->GetGensInfluence(lpObj) != this->GetGensInfluence(lpTargetObj);
		}
		else
		{
			return true;
		}
	}
	return true;
}
// -------------------------------------------------------------------------

//-> 00509290
//-> Completed
//-> Calls:
// OK	- 0044830F -> (CGChatWhisperRecv)
// OK	- 0044832D -> (CGChatWhisperRecv)
// OK	- 0045E7AD -> (CGPartyRequestRecv)
// OK	- 0045E7CB -> (CGPartyRequestRecv)
// OK	- 00460F8E -> (CGGuildRequestRecv)
// OK	- 00460FAD -> (CGGuildRequestRecv)
// OK	- 0047BCEA -> (CGRelationShipReqJoinBreakOff)
// OK	- 0047BCFB -> (CGRelationShipReqJoinBreakOff)
// OK	- 00480747 -> (CGReqSecedeGensMember)
// NONE	- 004DA664 -> (GetGensInfluence@QuestExpLuaBind)
// NONE	- 004E1FC6 -> (ChkUserQuestType@QuestExpProgMng)
// NONE	- 004E1FEC -> (ChkUserQuestType@QuestExpProgMng)
// OK	- 00508525 -> (IsInfluenceNPC)
// OK	- 00508548 -> (IsInfluenceNPC)
// OK	- 005086D1 -> (SendGensInfo)
// OK	- 00508B92 -> (BattleZoneChatMsgSend)
// OK	- 00508C0D -> (BattleZoneChatMsgSend)
// OK	- 00508E42 -> (GensViewportListProtocol)
// OK	- 005091F3 -> (IsPkEnable)
// OK	- 00509202 -> (IsPkEnable)
// OK	- 005093C3 -> (IsRegGensInfluence)
// OK	- 00509A8C -> (CalcContributePoint)
// OK	- 00509A9B -> (CalcContributePoint)
// OK	- 00590522 -> (gObjViewportListProtocol)
// OK	- 00590578 -> (gObjViewportListProtocol)
// OK	- 0061527C -> (NpcGuildMasterTalk)
//-> Fine
signed int GensSystem::GetGensInfluence(LPOBJ lpObj)
{
	if (lpObj->m_GensInfluence == LEAVED_INFLUENCE || lpObj->m_GensInfluence < NONE_INFLUENCE || lpObj->m_GensInfluence > DUPRIAN_INFLUENCE) 
	{
		LogAddTD("[GensSystem] Error [%s][%s] GensInfluence [%d]", lpObj->AccountID, lpObj->Name, lpObj->m_GensInfluence); return 0;
	}
	return lpObj->m_GensInfluence;
}
// -------------------------------------------------------------------------

//-> 00509320
//-> Completed
//-> Calls:
// OK	- 00461090 -> (CGGuildRequestRecv)
// OK	- 004610D5 -> (CGGuildRequestRecv)
// OK	- 0047BDB3 -> (CGRelationShipReqJoinBreakOff)
// OK	- 00489AD6 -> (DGAnsSecedeGensMember)
// OK	- 00489B2E -> (DGAnsSecedeGensMember)
// OK	- 00489BC4 -> (DGAnsSecedeGensMember)
// OK	- 0048A05E -> (DGAnsRegGensMember)
// OK	- 0048A0B9 -> (DGAnsRegGensMember)
// OK	- 0048A5D3 -> (DGAnsGensInfo)
// OK	- 00509E4B -> (CalcContributePoint)
// OK	- 00509E63 -> (CalcContributePoint)
// OK 	- 00509EFC -> (CalcContributePoint)
// OK	- 00509F14 -> (CalcContributePoint)
// OK	- 00509FC8 -> (CalcContributePoint)
// OK	- 00509FE0 -> (CalcContributePoint)
// OK	- 00617C94 -> (Npc_Dialog)
// OK	- 00617CD9 -> (Npc_Dialog)
//-> Fine
char *GensSystem::GetGensInfluenceName(LPOBJ lpObj)
{
	if (lpObj->m_GensInfluence == LEAVED_INFLUENCE || lpObj->m_GensInfluence < NONE_INFLUENCE || lpObj->m_GensInfluence > DUPRIAN_INFLUENCE) 
	{
		LogAddTD("[GensSystem] Error [%s][%s] GensInfluence [%d]", lpObj->AccountID, lpObj->Name, lpObj->m_GensInfluence);
		return NULL;
	}
	return szInfluence[lpObj->m_GensInfluence];
}
// -------------------------------------------------------------------------

//-> 005093b0
//-> Completed
//-> Calls: 00480AB2 (CGReqGensMemberInfo), 00508896 (GDReqSaveContributePoint),
//005089B5 (DBSaveAbusingKillUserName), 0050A647 (AbusingInfoReset),
//0050AA33 (GetNextContributePoint)
//-> Fine
bool GensSystem::IsRegGensInfluence(LPOBJ lpObj)
{
	if (this->GetGensInfluence(lpObj) == VANERT_INFLUENCE || this->GetGensInfluence(lpObj) == DUPRIAN_INFLUENCE)
	{
		return true;
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 00509400
//-> Completed
//-> Calls: 00489C4C (DGAnsSecedeGensMember), 00489F96 (DGAnsRegGensMember),
//0048A4B4 (DGAnsGensInfo), 0048A57C (DGAnsGensInfo)
//-> Fine
int GensSystem::SetUserBattleZoneEnable(LPOBJ lpObj, int bBattleZoneEnable)
{
	return lpObj->m_UserBattleZoneEnable = bBattleZoneEnable;
}
// -------------------------------------------------------------------------

//-> 00509430
//-> Completed
//-> Calls: 0048A66D (DGAnsGensInfo), 005923E2 (gObjMoveGate), 
//005D2AC4 (Move@CMoveCommand)
//-> Fine
int GensSystem::IsUserBattleZoneEnable(LPOBJ lpObj)
{
	return lpObj->m_UserBattleZoneEnable;
}
// -------------------------------------------------------------------------

//-> 00509460
//-> Completed
//-> Calls: 00489C6A (DGAnsSecedeGensMember), 00489FDD (DGAnsRegGensMember),
//0048A4FB (DGAnsGensInfo), 0048A5B8 (DGAnsGensInfo)
//-> Fine
int GensSystem::SetContributePoint(LPOBJ lpObj, int iContributePoint)
{
	return lpObj->m_ContributePoint = iContributePoint;
}
// -------------------------------------------------------------------------

//-> 00509490
//-> Completed
//-> Calls: 004E6D26 (SendQuestReward), 00509DD0 (CalcContributePoint),
//00509F5D (CalcContributePoint)
//-> Fine
int GensSystem::AddContributePoint(LPOBJ lpObj, int iContributePoint)
{
	lpObj->m_ContributePoint += iContributePoint;
	return this->GDReqSaveContributePoint(lpObj);
}
// -------------------------------------------------------------------------

//-> 005094e0
//-> Completed
//-> Calls: 00509DE0 (CalcContributePoint)
//-> Fine
int GensSystem::SubContributePoint(LPOBJ lpObj, int iContributePoint)
{
	lpObj->m_ContributePoint -= iContributePoint;

	if (lpObj->m_ContributePoint < 0)
	{
		lpObj->m_ContributePoint = 0;
	}

	return this->GDReqSaveContributePoint(lpObj);
}
// -------------------------------------------------------------------------

//-> 00509550
//-> Completed
//-> Calls:
// NONE	- 004E6DB8 -> (SendQuestReward)
// OK	- 005088CB -> (GDReqSaveContributePoint)
// OK	- 00509B2C -> (CalcContributePoint)
// OK	- 00509B3B -> (CalcContributePoint)
// OK	- 00509E02 -> (CalcContributePoint)
// OK	- 00509EB3 -> (CalcContributePoint)
// OK	- 00509F7F -> (CalcContributePoint)
// OK	- 00617C6B -> (Npc_Dialog)
// OK	- 00617C85 -> (Npc_Dialog)
// OK	- 00617CCA -> (Npc_Dialog)
//-> Fine
int GensSystem::GetContributePoint(LPOBJ lpObj)
{
	return lpObj->m_ContributePoint;
}
// -------------------------------------------------------------------------

//-> 00509580
//-> Completed
//-> Calls: 005D2C71 (Move@CMoveCommand)
//-> Fine
signed int GensSystem::PkPenaltyAddNeedZenMapMove(LPOBJ lpObj)
{
	if (gPkLimitFree)
	{
		if (this->IsMapBattleZone(lpObj->MapNumber))
		{
			return 1;
		}
		return 1;
	}
	return 0;
}
// -------------------------------------------------------------------------

//-> 00509600
//-> Completed
//-> Calls: 0056AF8B (gObjUserDie)
//-> Fine
void GensSystem::PkPenaltyDropInvenItem(LPOBJ lpObj)
{
	if (gPkLimitFree)
	{
		if (this->IsMapBattleZone(lpObj->MapNumber))
		{
			this->b_PkPenaltyDropInvenItem = TRUE; 		
		}
	}
	this->b_PkPenaltyDropInvenItem = FALSE;	
}
// -------------------------------------------------------------------------

//-> 00509840
//-> Completed
//-> Calls: 0056B171 (gObjUserDie)
//-> Fine
void GensSystem::PkPenaltyDropZen(LPOBJ lpObj)
{
	if (gPkLimitFree)
	{
		if (this->IsMapBattleZone(lpObj->MapNumber))
		{
			this->b_PkPenaltyDropZen = TRUE;		
		}	
	}
	this->b_PkPenaltyDropZen = FALSE;
}
// -------------------------------------------------------------------------

//-> 005099c0
//-> Completed
//-> Calls: 0056A17E (gObjPlayerKiller)
//-> Fine
int GensSystem::SendPKPenaltyDebuff(LPOBJ lpObj)
{
	if (lpObj->Type)
	{
		if (gPkLimitFree)
		{
			return this->IsMapBattleZone(lpObj->MapNumber);
		}
	}
	return 0;
}
// -------------------------------------------------------------------------

//-> 00509a20
//-> Completed
//-> Calls: 0056ACD2 (gObjUserDie)
//-> Fine
void GensSystem::CalcContributePoint(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int iInfluence;
	int iTarInfluence;
	int iContributePoint;
	int iKillerContributePoint;
	int iCalcLv;
	int iAddValue;
	int iSubValue;
	int iAddBonusValue;
	int iGensClassGap;
	
	char szName[0x0B];

	if (lpObj->Type && lpTargetObj->Type && this->IsMapBattleZone(lpObj->MapNumber) && this->IsMapBattleZone(lpTargetObj->MapNumber))
	{
		iInfluence		= this->GetGensInfluence(lpObj);
		iTarInfluence	= this->GetGensInfluence(lpTargetObj);

		if (iInfluence && iTarInfluence)
		{
			if (iInfluence != iTarInfluence)
			{
				memcpy(szName, lpTargetObj->Name, sizeof(lpTargetObj->Name));		

				if (this->ChkKillUserName(lpObj, szName) == 0)
				{
					iKillerContributePoint = this->GetContributePoint(lpTargetObj); 
					
					iContributePoint = this->GetContributePoint(lpObj);

					if (iKillerContributePoint > 0 || iContributePoint <= 0)
					{
						iAddValue = 0; iAddBonusValue = 0;

						if (lpObj->Level == lpTargetObj->Level)
						{
							iAddValue = 5; iAddBonusValue = 3;
						}
						else
						{							
							if (lpObj->Level >= lpTargetObj->Level)
							{					
								if (lpObj->Level > lpTargetObj->Level)
								{
									iCalcLv = lpObj->Level - lpTargetObj->Level;

								 if (iCalcLv < 51)
							 	{
									 if (iCalcLv < 31 || iCalcLv > 50)
								 	{
								 		if (iCalcLv < 11 || iCalcLv > 30)
									 	{
									 		if (iCalcLv > 0 && iCalcLv <= 10)
											 {
										 		iAddValue = 5; iSubValue = 3;
										 	}
									 	}
									 	else
										 {
										 	iAddValue = 3; iSubValue = 3;
										 }
								 	}
								 	else 
								 	{
										 iAddValue = 2; iSubValue = 1;
									 }
								 }
							 	else
							 	{
							 		iAddValue = 1; iSubValue = 1;
							 	}
							 }
						 }
							else
							{
								iCalcLv = lpTargetObj->Level - lpObj->Level;

								if (iCalcLv < 51)
								{
									if (iCalcLv < 31 || iCalcLv > 50)
									{
										if (iCalcLv < 11 || iCalcLv > 30)
										{
											if (iCalcLv > 0 && iCalcLv <= 10)
											{
												iAddValue = 5; iSubValue = 3;
											}
										}
										else
										{
											iAddValue = 6; iSubValue = 3;
										}
									}
									else
									{
										iAddValue = 6; iSubValue = 3;
									}
								}
								else
								{
									iAddValue = 7; iSubValue = 3;
								}
							}
						}

						iAddBonusValue = 0;

						iGensClassGap = lpObj->m_GensClass - lpTargetObj->m_GensClass;
					
						if (iGensClassGap == 1)
						{
							if (lpObj->m_GensClass >= 9)
							{
								iAddBonusValue = 1;
							}
							else
							{
								iAddBonusValue = 3;
							}
						}
						else
						{
							if (iGensClassGap == 2)
							{
								if (lpObj->m_GensClass >= 9)
								{
									iAddBonusValue = 2;
								}
								else
								{
									iAddBonusValue = 4;
								}
							}
							else
							{
								if (iGensClassGap >= 3)
								{
									if (lpObj->m_GensClass >= 9)
									{
										iAddBonusValue = 3;
									}
									else
									{
										iAddBonusValue = 5;
									}
								}
							}
					
							if( lpObj->m_GensClass > 9 && iAddBonusValue + iAddValue >= this->GetNextContributePoint(lpObj) )
							{
								//GCServerMsgStringSend(lMsg.Get(3470), lpObj->m_Index, 1);
								GCServerMsgStringSend("You have maximum gens class", lpObj->m_Index, 1); //-> Not original text
							}
							else //-> Not original
							{
								char Text[100];
								sprintf(Text, "%d contribution point(s) obtained", iAddValue);
								// ----
								GCServerMsgStringSend(Text, lpObj->m_Index, 1);
							}

							this->AddContributePoint(lpObj, iAddValue);				
										
							LogAddTD("[GensSystem] [%s][%s] Add ContributePoint[%d] - Result : Lv [%d] GensName [%s] / Lv [%d] GensName [%s]",
								lpObj->AccountID, lpObj->Name, iAddValue, lpObj->Level, this->GetGensInfluenceName(lpObj), lpTargetObj->Level, this->GetGensInfluenceName(lpTargetObj));

							this->SubContributePoint(lpTargetObj, iSubValue);
						
							if( iSubValue > 0 )
							{
								char Text[100];
								sprintf(Text, "%d contribution point(s) would be lost", iAddValue);
								// ----
								GCServerMsgStringSend(Text, lpObj->m_Index, 1);
							}

							LogAddTD("[GensSystem] [%s][%s] Sub ContributePoint[%d] - Result : Lv [%d] GensName [%s] / Lv [%d] GensName [%s]",
								lpTargetObj->AccountID, lpTargetObj->Name, iSubValue, lpTargetObj->Level, this->GetGensInfluenceName(lpTargetObj), lpObj->Level, this->GetGensInfluenceName(lpObj));
					
							if (iAddBonusValue > 0)
							{
								this->AddContributePoint(lpObj, iAddBonusValue);

								//GCServerMsgStringSend(lMsg.Get(3472), lpObj->m_Index, 1);
								// ----
								char Text[100];
								sprintf(Text, "%d bonus contribution point(s) obtained", iAddBonusValue);
								// ----
								GCServerMsgStringSend(Text, lpObj->m_Index, 1); //-> Not original text

								LogAddTD("[GensSystem] [%s][%s] Add BonusCP[%d] - Result : Lv [%d] GensName [%s] / Lv [%d] GensName [%s]",
									lpObj->AccountID, lpObj->Name, iAddBonusValue, lpObj->Level, this->GetGensInfluenceName(lpObj), lpTargetObj->Level, this->GetGensInfluenceName(lpTargetObj));
							}
						}
				 }
		 	}
	 	}
			else
			{
				LogAddTD("[GensSystem] - Error NONE_INFLUENCE - %s Line: %d", __FILE__, __LINE__);
			}
	 }
	}
}
// -------------------------------------------------------------------------

//-> 0050a1a0
//-> Completed
//-> Calls: 00509B16 (CalcContributePoint)
//-> Fine
signed int GensSystem::ChkKillUserName(LPOBJ lpObj, char *szCharName)
{
	int i = 0;

	while (true)
	{
		if (i >= 10)
		{
			EnterCriticalSection((LPCRITICAL_SECTION)&criti);
			this->InsertKillUserName(lpObj, szCharName);
			LeaveCriticalSection((LPCRITICAL_SECTION)&criti);
			return 0;
		}

		if (!strcmp(gObj[lpObj->m_Index].m_szCharName, szCharName))
	 	break;
	
		i++;
	}
	
	EnterCriticalSection((LPCRITICAL_SECTION)&criti);
	++lpObj->m_KillUserCnt;
	LeaveCriticalSection((LPCRITICAL_SECTION)&criti);

	if (this->AbusingPenalty(lpObj, i))
		return true;

	return false;
}
// -------------------------------------------------------------------------

//-> 0050a2c0
//-> Completed
//-> Calls: 00509B16 (ChkKillUserName)
//-> Fine
signed int GensSystem::AbusingPenalty(LPOBJ lpObj, int iKillUserIndex)
{
	int iKillUserCnt = lpObj->m_KillUserCnt;

	int bDBSave = 0;

	if (lpObj->m_ContributePoint > 0)
	{
		bDBSave = 1;
	}

	if (iKillUserCnt >= 3)
	{
		if (iKillUserCnt < 3 || iKillUserCnt > 5)
		{
			if (iKillUserCnt >= 6)
			{
				GCServerMsgStringSend(lMsg.Get(3450), lpObj->m_Index, 1);
			}

			EnterCriticalSection((LPCRITICAL_SECTION)&criti);
		
			if (lpObj->m_KillUserCnt < 0)
			{
				lpObj->m_KillUserCnt = 0;
			}
		
			LeaveCriticalSection((LPCRITICAL_SECTION)&criti);

			if (bDBSave)
			{
				this->GDReqSaveContributePoint(lpObj);	return true;
			}
			return true;
		}
		else
		{
			GCServerMsgStringSend(lMsg.Get(3449), lpObj->m_Index, 1);
			return false;
		}
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 0050a400
//-> Completed
//-> Calls: 0050A265 (ChkKillUserName)
//-> Fine
int GensSystem::InsertKillUserName(LPOBJ lpObj, char *szCharName)
{
	int iKillUserToCnt = lpObj->m_KillUserTotCnt;

	if (iKillUserToCnt >= 10)
	{
		lpObj->m_KillUserTotCnt = 0; iKillUserToCnt = 0; 
	}

	memset(lpObj->m_szCharName, 0, 0x0B);
	strcpy(lpObj->m_szCharName, szCharName);

	++lpObj->m_KillUserCnt;
	++lpObj->m_KillUserTotCnt;

	for (int i = 0; i < lpObj->m_KillUserTotCnt; ++i)
	{
		LogAddTD("[GensSystem] Abusing List [%s][%s] KillUserName[%s] Cnt[%d]", lpObj->AccountID, lpObj->Name, lpObj->m_szCharName, lpObj->m_KillUserCnt);
		iKillUserToCnt = i + 1;
	}
	return iKillUserToCnt;
}
// -------------------------------------------------------------------------

//-> 0050a550
//-> Completed
//-> Calls: (Move@CMoveCommand)
//-> Fine
void GensSystem::MoveInBattleZonePartySplit(LPOBJ lpObj)
{
	PMSG_PARTYDELUSER pMsg;	

	if (gPkLimitFree != TRUE && lpObj->PartyNumber >= 0)
	{
		int pindex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);

		if (pindex >= 0)
		{
			pMsg.Number = pindex;
			CGPartyDelUser(&pMsg, lpObj->m_Index); GCResultSend(lpObj->m_Index, 0x41, 0x7);
		}
	}
}
// -------------------------------------------------------------------------

//-> 0050a5f0
//-> Completed
//-> Calls: 006D4189 (TimerProcQueue)
//-> Not fine (becoz need decompile MuTimer.obj)
void GensSystem::AbusingInfoReset(LPOBJ lpObj)
{
	/*if (!OBJMAX_RANGE(lpObj->m_Index))
		return;

	if (!gObjIsConnected(lpObj->m_Index))
	{
		if (this->IsRegGensInfluence(lpObj))
		{
			if (g_GensAbusingResetTime > 0 && GetTickCount() - lpObj->m_GensAbusingTick >= 60000 * g_GensAbusingResetTime)
			{
				EnterCriticalSection((LPCRITICAL_SECTION)&criti);

				lpObj->m_GensAbusingTick = GetTickCount();

				for (int i = 0; i < 10; ++i)
				{
					lpObj->m_KillUserCnt = 0;
					memset(lpObj->m_szCharName, 0, 0x0B);
				}

				lpObj->m_KillUserTotCnt = 0;

				LeaveCriticalSection((LPCRITICAL_SECTION)&criti);
			}
		}
		else
		{
			lpObj->m_GensAbusingTick = GetTickCount();
		}
	}*/
}
// -------------------------------------------------------------------------

//-> 0050a780
//-> Completed
//-> Calls: Custom
//-> Not original
void GensSystem::CalGensClass(LPOBJ lpObj)
{
	if( lpObj->m_GensRanking > 300 || lpObj->m_GensRanking == 0 )
	{
		if( lpObj->m_ContributePoint >= 0 && lpObj->m_ContributePoint <= 499 )
		{
			lpObj->m_GensClass = 14; //-> Private
		}
		// ----
		if( lpObj->m_ContributePoint >= 500 && lpObj->m_ContributePoint <= 1499 )
		{
			lpObj->m_GensClass = 13; //-> Sergeant
		}
		// ----
		if( lpObj->m_ContributePoint >= 1500 && lpObj->m_ContributePoint <= 2999 )
		{
			lpObj->m_GensClass = 12; //-> Lieutenant
		}
		// ----
		if( lpObj->m_ContributePoint >= 3000 && lpObj->m_ContributePoint <= 5999 )
		{
			lpObj->m_GensClass = 11; //-> Officer
		}
		// ----
		if( lpObj->m_ContributePoint >= 6000 && lpObj->m_ContributePoint <= 9999 )
		{
			lpObj->m_GensClass = 10; //-> Guard Perfect
		}
		// ----
		if( lpObj->m_ContributePoint >= 10000 )
		{
			lpObj->m_GensClass = 9; //-> Knight
		}
	}
	else
	{
		if( lpObj->m_GensRanking == 1 )
		{
			lpObj->m_GensClass = 1; //-> Grand Duke
		}
		// ----
		if( lpObj->m_GensRanking >= 2 && lpObj->m_GensRanking <= 5 )
		{
			lpObj->m_GensClass = 2; //-> Duke
		}
		// ----
		if( lpObj->m_GensRanking >= 6 && lpObj->m_GensRanking <= 10 )
		{
			lpObj->m_GensClass = 3; //-> Marquis
		}
		// ----
		if( lpObj->m_GensRanking >= 11 && lpObj->m_GensRanking <= 30 )
		{
			lpObj->m_GensClass = 4; //-> Count
		}
		// ----
		if( lpObj->m_GensRanking >= 31 && lpObj->m_GensRanking <= 50 )
		{
			lpObj->m_GensClass = 5; //-> Viscount
		}
		// ----
		if( lpObj->m_GensRanking >= 51 && lpObj->m_GensRanking <= 100 )
		{
			lpObj->m_GensClass = 6; //-> Baron
		}
		// ----
		if( lpObj->m_GensRanking >= 101 && lpObj->m_GensRanking <= 200 )
		{
			lpObj->m_GensClass = 7; //-> Knight Commander
		}
		// ----
		if( lpObj->m_GensRanking >= 201 && lpObj->m_GensRanking <= 300 )
		{
			lpObj->m_GensClass = 8; //-> Super Knight
		}
	}
	/*
	if (lpObj->m_ContributePoint > 200)
	{
		if (lpObj->m_ContributePoint < 224)
		{
			if (lpObj->m_ContributePoint < 248)
			{
				if (lpObj->m_ContributePoint < 272)
				{
					if (lpObj->m_ContributePoint < 296)
					{
						if (lpObj->m_ContributePoint < 320)
						{
							return 0; // WTF? >< if rank from 297 to 319 your rank is 0???
						}
						else 
						{
							return 14;
						}
					}
					else
					{
						return 13;
					}
				}
				else 
				{
					return 12;
				}
			}
			else
			{
				return 11;
			}
		}
		else
		{
			return 10;
		}
	}
	else
	{
		if (lpObj->m_ContributePoint > 8)
		{
			if (lpObj->m_ContributePoint > 32)
			{
				if (lpObj->m_ContributePoint > 56)
				{
					if (lpObj->m_ContributePoint > 80)
					{
						if (lpObj->m_ContributePoint > 104)
						{
							if (lpObj->m_ContributePoint > 128)
							{
								if (lpObj->m_ContributePoint > 152)
								{
									if (lpObj->m_ContributePoint > 176)
									{
										return 9;
									}
									else
									{
										return 8;
									}
								}
								else
								{
									return 7;
								}
							}
							else 
							{
								return 6;
							}
						}
						else 
						{
							return 5;
						}
					}
					else
					{
						return 4;
					}
				}
				else
				{
					return 3;
				}
			}
			else 
			{
				return 2;
			}
		}
		else
		{
			return 1;
		}
	}
	return 0;*/
}
// -------------------------------------------------------------------------

//-> 0050a990
//-> Completed
//-> Calls: 00489D3F (DGAnsSecedeGensMember), 0048A51E (DGAnsGensInfo)
//-> Fine
int GensSystem::SetGensRanking(LPOBJ lpObj, int iGensRanking)
{
	return lpObj->m_GensRanking = iGensRanking;
}
// -------------------------------------------------------------------------

//-> 0050a9c0
//-> Completed
//-> Calls: 00489D21 (DGAnsSecedeGensMember), 0048A541 (DGAnsGensInfo)
//-> Fine
int GensSystem::SetGensClass(LPOBJ lpObj, int iGensClass)
{
	return lpObj->m_GensClass = iGensClass;
}
// -------------------------------------------------------------------------

//-> 0050a9f0
//-> Completed
//-> Calls: 005088EA (GDReqSaveContributePoint)
//-> Fine
int GensSystem::GetGensClass(LPOBJ lpObj)
{
	return lpObj->m_GensClass;
}
// -------------------------------------------------------------------------

//-> 0050aa20
//-> Completed
//-> Calls: 00508714 (SendGensInfo), 00509D84 (CalcContributePoint)
//-> Fine
int GensSystem::GetNextContributePoint(LPOBJ lpObj)
{
	if (this->IsRegGensInfluence(lpObj))
	{
		if (lpObj->m_NextContributePoint > 499)
		{
			if (lpObj->m_NextContributePoint > 1499)
			{
				if (lpObj->m_NextContributePoint > 2999)
				{
					if (lpObj->m_NextContributePoint > 5999)
					{
						if (lpObj->m_NextContributePoint > 9999)
						{
							return -2;
						}
						else
						{
							return 10000 - lpObj->m_NextContributePoint;
						}
					}
					else
					{
						return 6000 - lpObj->m_NextContributePoint;
					}
				}
				else
				{
					return 3000 - lpObj->m_NextContributePoint;
				}
			}
			else
			{
		 	return 1500 - lpObj->m_NextContributePoint;
			}
		}
		else
		{
			return 500 - lpObj->m_NextContributePoint;
		}
	}
	else
	{
		return -1;
	}

	return 0;
}
// -------------------------------------------------------------------------

//-> 0050ab30
//-> Completed
//-> Calls: 0048AAD7 (DGAnsGensMemberCount), 0048AAEA (CalcContributePoint)
//-> Fine
int GensSystem::SetGensMemberCount(int iInfluence, int iMemberCount)
{
	if (iInfluence == VANERT_INFLUENCE)
	{
		this->iVanertMemberCount = iMemberCount;
	}
	else 
	{
		if (iInfluence == DUPRIAN_INFLUENCE)
		 this->iDuprianMemberCount = iMemberCount;
	}

	return NONE_INFLUENCE;
}
// -------------------------------------------------------------------------

//-> 0050ab80
//-> Completed
//-> Calls: 005CC192 (ManagementProc), 005CC19F (ManagementProc)
//-> Fine
signed int GensSystem::GetGensMemberCount(int iInfluence)
{
	if (iInfluence == VANERT_INFLUENCE)
	{
		return this->iVanertMemberCount;
	}
	else 	if (iInfluence == DUPRIAN_INFLUENCE)
	{
		return this->iDuprianMemberCount;
	}
	else
	{
	 return -1;
	}
}
// -------------------------------------------------------------------------

//-> 0050abd0
//-> Completed
//-> Calls: 00480A1B (CGReqGensReward)
//-> Fine
int GensSystem::ReqExDBGensRewardCheck(LPOBJ lpObj, int iInfluence)
{
	if (!OBJMAX_RANGE(lpObj->m_Index))
		return false;

	if (gObjIsConnected(lpObj))
	{
		PMSG_REQ_GENS_REWARD_CHECK_EXDB pMsg;	

		PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));
	
		pMsg.bResult	= 12;
		strcpy(pMsg.Name, lpObj->Name);
		pMsg.wIndexH	= SET_NUMBERH(lpObj->m_Index);
		pMsg.wIndexL	= SET_NUMBERL(lpObj->m_Index);
		pMsg.iInfluence = iInfluence;
			
		if (wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size))
			return true;
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 0050acc0
//-> Completed
//-> Calls: 0048A9F2 (DGAnsGensReward)
//-> Fine
int GensSystem::ReqExDBGensRewardComplete(LPOBJ lpObj)
{
	if (!OBJMAX_RANGE(lpObj->m_Index))
		return false;

	if (gObjIsConnected(lpObj))
	{
		PMSG_REQ_GENS_REWARD_COMPLETE_EXDB pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));

		pMsg.bResult = 14;
		memcpy(pMsg.Name, lpObj->Name, 0x0A);
		pMsg.wIndexH = SET_NUMBERHW(lpObj->m_Index);
		pMsg.wIndexL = SET_NUMBERLW(lpObj->m_Index);

		if (wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size))
			return true;
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 0050ada0
//-> Completed
//-> Calls: 0048AA64 (DGAnsGensReward)
//-> Fine
int GensSystem::SendGensReward(LPOBJ lpObj, unsigned char btResult)
{
	if (!OBJMAX_RANGE(lpObj->m_Index))
		return false;

	if (gObjIsConnected(lpObj))
	{
		PMSG_SEND_GENS_REWARD pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));

		pMsg.bResult = 10;
		pMsg.bItemType = btResult;

		if (DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size))
		 return true;
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 0050ae40
//-> Completed
//-> Calls: 0048A9CE (DGAnsGensReward)
//-> Fine
signed int GensSystem::SendGensRewardItem(LPOBJ lpObj, int iGensClass)
{
	int iIndex = lpObj->m_Index;

	int iItemCount = iGensClass - 1;

	if (iItemCount - 1 >= 0)
	{
		iItemCount = this->iIndex * iItemCount + 5;
		
		if (iItemCount > 0)
		{
			int iItemNumber = ((this->iIndex * iItemCount + 4) + (this->iIndex * iItemCount + 3)) << 9; 
		
			if (this->GensRewardInventoryCheck(lpObj, iItemCount))
			{
				for (int i = 0; i < iItemCount; ++i)
				{
					ItemSerialCreateSend(iIndex, 240, 0, 0, iItemNumber, 0, 0, 0, 0, 0, 0, 0, 0);
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return false;
}
// -------------------------------------------------------------------------

//-> 0050af70
//-> Completed
//-> Calls: 0048A9CE (SendGensRewardItem)
//-> Fine
bool GensSystem::GensRewardInventoryCheck(LPOBJ lpObj, int iItemCount)
{
	unsigned char blank = NULL;
	
	int iCount = 0;

	for (int h = 0; h < 8; ++h)
	{
		for (int w = 0; w < 8; ++w)
		{
			if (*(BYTE*)(lpObj->pInventoryMap + (h) * 8 + (w)) == 255)
			{
				if (gObjOnlyInventoryRectCheck(lpObj->m_Index, w, h, 1, 1))
				{
					++iCount;
				}
			}
		}
	}
	return iItemCount <= iCount;
}
// -------------------------------------------------------------------------

//-> 0050af70
//-> Completed
//-> Calls: 0048A9CE (SendGensRewardItem)
//-> Fine
int GensSystem::ReqExDBGensMemberCount(LPOBJ lpObj)
{
	if (!OBJMAX_RANGE(lpObj->m_Index))
		return false;

	if (gObjIsConnected(lpObj))
	{
		PMSG_REQ_GENS_MEMBER_COUNT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));

		pMsg.bResult = 0xF;
		pMsg.wIndexH = SET_NUMBERHW(lpObj->m_Index);
		pMsg.wIndexL = SET_NUMBERLW(lpObj->m_Index);

		if (wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size))
			return true;
	}
	return false;
}
// -------------------------------------------------------------------------

//-> Completed
int GensSystem::ReqExDBSetGensRewardDay(void)
{
	PMSG_SET_GENS_REWARD_DAY_EXDB pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));
	pMsg.bRewardDay = 17;

	return wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
};

//-> Completed
int GensSystem::ReqExDBSetGensRanking(void)
{
	GENS_RANKING pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));
	pMsg.bRanking = 0x12;

	return wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
};

//-> Not Completed
int GensSystem::LoadData(char * lpszFileName)
{
	/*int iType = 0;
	SMDToken Token = GetToken();
	int i = 0;
	unsigned long dwError = 0;
	int iGensClass = 0;
	int iRewardItemType = 0;
	int iRewardItemCount = 0;
	unsigned char btRankingKind = NULL;
	int iRankingValue = 0;
	int iRewardItemNumber = 0;*/
	// ----
	SMDFile = fopen(lpszFileName, "r");	//ok
	// ----
	if( !SMDFile )
	{
		MsgBox("[GensSystem] Info file Load Fail [%s]", lpszFileName);
		return false;
	}
	// ----
	int Token;
	int Type;
	int ZoneCounter		= 0;
	int MapCounter		= 0;
	int RewardCounter	= 0;
	// ----
	for( int i = 0; i < MAX_BATTLE_ZONE; i++ )
	{
		this->BattleZoneList[i]	= -1;
		this->BattleMapList[i]	= -1;
	}
	// ----
	for( int i = 0; i < MAX_REWARD_TABLE+1; i++ )
	{
		this->RewardTableList[i].Rank		= -1;
		this->RewardTableList[i].ItemID		= -1;
		this->RewardTableList[i].ItemIndex	= -1;
		this->RewardTableList[i].ItemLevel	= -1;
		this->RewardTableList[i].ItemCount	= -1;
	}
	// ----
	this->AllowPK = false;
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
			if( Type == 0 )
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				// ----
				StartContribution	= (int)TokenNumber;
				// ----
				Token				= GetToken();
				RequiredLevel		= (int)TokenNumber;
				// ----
				Token				= GetToken();
				RequiredReset		= (int)TokenNumber;
				// ----
				Token						= GetToken();
				this->GuildCreateReqGens	= (bool)TokenNumber;
				// ----
				Token						= GetToken();
				this->CheckGuildEnter		= (bool)TokenNumber;
				// ----
				Token						= GetToken();
				this->CheckPartyEnter		= (bool)TokenNumber;
				// ----
				Token						= GetToken();
				this->CheckAllianceEnter	= (bool)TokenNumber;
				// ----
				Token						= GetToken();
				this->AllowPK				= (bool)TokenNumber;
				// ----
				Token						= GetToken();
				this->ChatPrice				= (int)TokenNumber;
			}
			else if( Type == 1 ) //-> Battle Zone List
			{
				Token		= GetToken();
				// ----
				if( !strcmp("end", TokenString) || ZoneCounter > MAX_BATTLE_ZONE )
				{
					break;
				}
				// ----
				this->BattleZoneList[ZoneCounter] = TokenNumber;
				ZoneCounter++;
			}
			else if( Type == 2 ) //-> Battle Map List
			{
				Token		= GetToken();
				// ----
				if( !strcmp("end", TokenString) || MapCounter > MAX_BATTLE_ZONE )
				{
					break;
				}
				// ----
				this->BattleMapList[MapCounter] = TokenNumber;
				MapCounter++;
			}
			else if( Type == 3 ) //-> Reward Table
			{
				BYTE RankID	= -1;
				Token		= GetToken();
				// ----
				if( !strcmp("end", TokenString) || RewardCounter > MAX_REWARD_TABLE )
				{
					break;
				}
				// ----
				RankID	= TokenNumber;
				this->RewardTableList[RankID].Rank		= RankID;
				// ----
				Token	= GetToken();
				this->RewardTableList[RankID].ItemID	= TokenNumber;
				// ----
				Token	= GetToken();
				this->RewardTableList[RankID].ItemIndex	= TokenNumber;
				// ----
				Token	= GetToken();
				this->RewardTableList[RankID].ItemLevel	= TokenNumber;
				// ----
				Token	= GetToken();
				this->RewardTableList[RankID].ItemCount	= TokenNumber;
				// ----
				RewardCounter++;
			}
		}
	}
	// ----
	fclose(SMDFile);
	// ----
	LogAdd("[GensSystem] %s file is loaded", lpszFileName);
	// ----
	return true;
}

//-> Completed
int GensSystem::ReqGensRewardDay(LPOBJ lpObj)
{
	PMSG_REQ_GENS_REWARD_DAY pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xF8, sizeof(pMsg));
	pMsg.bRewardDay = 19;
	pMsg.bIndexH = SET_NUMBERH(lpObj->m_Index);
	pMsg.bIndexL = SET_NUMBERL(lpObj->m_Index);

	return wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
};

//--------------------------------------------------



//--------------------------------------------------

//void ProtocolCore(BYTE protoNum, BYTE * aRecv, int aLen, int aIndex, BOOL Encrypt, int serial)
 
//-> Completed 
void CGReqRegGensMember(PMSG_REQ_REG_GENS_MEMBER *lpMsg, int iIndex)
{
	unsigned char bRet;

	LPOBJ lpObj = &gObj[iIndex];

	_GUILD_INFO_STRUCT *lpGuildInfo = gObj[iIndex].lpGuild;

	if (!OBJMAX_RANGE(iIndex)) return;

	if (gObjIsConnected(iIndex))
	{
		PMSG_ANS_REG_GENS_MEMBER_EXDB pMsg;	

		pMsg.bInfluence = lpMsg->bResult;

		//[GensSystem] [%s][%s] Gens Request - Join [%s]
		if (!gGensSystem.GetGensInfluence(lpObj))
		{
			pMsg.bResult = 0; bRet = 1;
			//LogAddTD("[GensSystem] [%s][%s] Gens Request - Join", lpObj->AccountID, lpObj->Name);
		}
		
		//GensSystem] [%s][%s] Gens Joining Failed to Gens be Same : [%s]
		if (gGensSystem.GetGensInfluence(lpObj) > 0)
		{
			pMsg.bResult = 1; bRet = 0;
			//LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Gens be Same", lpObj->AccountID, lpObj->Name);
		}
		
		//[GensSystem] [%s][%s] Gens Joining Failed to Below Gens leaving Time
		//if (g_GensAbusingResetTime < 60)
		//{
		//	pMsg.bResult = 2; bRet = 1;
		//}

		//[GensSystem] [%s][%s] Gens Joining Failed to Below 50Lv
		if (lpObj->Level < RequiredLevel )
		{
			pMsg.bResult = 3; bRet = 0;
			//LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Below 50Lv", lpObj->AccountID, lpObj->Name);
		}

		if (lpObj->Reset < RequiredReset )
		{
			pMsg.bResult = 3; bRet = 0;
			//LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Below 50Lv", lpObj->AccountID, lpObj->Name);
		}
		
		//[GensSystem] [%s][%s] Gens Joining Failed to Gens be Different Guild Leader [%s]
		if (lpObj->GuildStatus >= 0)
		{
			pMsg.bResult = 4; bRet = 0;
			//LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Gens be Different Guild Leader", lpObj->AccountID, lpObj->Name);
		}
		
		//[GensSystem] [%s][%s] Gens Joining Failed to Not Register Guild Leader GuildName [%s]
		if (lpObj->GuildStatus == GUILD_MASTER)
		{
			pMsg.bResult = 5; bRet = 0;
			//LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Gens be Different Guild Leader", lpObj->AccountID, lpObj->Name);
		}
		
		//[GensSystem] [%s][%s] Gens Joining Failed to Already Partymember
		if (lpObj->PartyNumber >= 0)
		{
			pMsg.bResult = 6; bRet = 0; 
		}
		
		//[GensSystem] [%s][%s] Gens Joining Failed to Union GuildMaster
		if (lpGuildInfo != NULL && lpGuildInfo->iGuildUnion)
		{
			pMsg.bResult = 7; bRet = 0;
		}
		
		if (bRet)
		{
			gGensSystem.ReqRegGensMember(lpObj, lpMsg->bResult);
		}
		else
		{
			pMsg.bIndexH = SET_NUMBERH(iIndex);
			pMsg.bIndexL = SET_NUMBERL(iIndex);
			DGAnsRegGensMember((PMSG_ANS_REG_GENS_MEMBER_EXDB*)&pMsg);
		}
	}
}

//-> Completed
void CGReqSecedeGensMember(PMSG_REQ_SEGEDE_GENS_MEMBER *lpMsg, int iIndex)
{
	if (!OBJMAX_RANGE(iIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];

	unsigned char bRet = 1;

	if (gObjIsConnected(iIndex))
	{
		PMSG_ANS_SECEDE_GENS_MEMBER_EXDB pMsg;	

		if( !gGensSystem.IsInfluenceNPC(lpObj) )
		{
			pMsg.bResult = 3; bRet = 0;
		}
		// ----
		if(  !gGensSystem.GetGensInfluence(lpObj) )
		{
			pMsg.bResult = 1; bRet = 0;
		}
		// ----
		if( bRet )
		{
			gGensSystem.ReqSecedeGensMember(lpObj);
		}
		else
		{
			pMsg.bIndexH = SET_NUMBERH(iIndex); 
			pMsg.bIndexL = SET_NUMBERL(iIndex);
			DGAnsSecedeGensMember((PMSG_ANS_SECEDE_GENS_MEMBER_EXDB*)&pMsg);
		}
	}
}
// -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------

//-> Completed
void CGReqGensReward(PMSG_GENS_REWARD_CODE *lpMsg, int iIndex)
{
	if (!OBJMAX_RANGE(iIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if (gObjIsConnected(lpObj))
	{
		//gGensSystem.ReqExDBGensRewardCheck(lpObj, lpMsg->bReward);
		gGensSystem.GDRewardReq(lpObj);
	}
}
// -------------------------------------------------------------------------------

void GensSystem::GDRewardReq(LPOBJ lpObj)
{
	PMSG_GENS_REWARD_REQ pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x08, sizeof(pMsg));
	// ----
	memcpy(pMsg.Name, lpObj->Name, 10);
	pMsg.aIndex = lpObj->m_Index;
	// ----
	if( !gObjIsConnected(lpObj) )
	{
		return;
	}
	// ----
	if( !this->IsRegGensInfluence(lpObj) || lpObj->m_GensClass > 8 )
	{
		gGensSystem.SendGensReward(lpObj, 2);
		return;
	}
	// ----
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
}

void GensSystem::DGRewardAns(PMSG_GENS_REWARD_ANS * aRecv)
{
	//0 - Complete
	//1 - Gens ranking rewards are given out in the first week of each month
	//2 - Not eligible to reward
	//3	- Not have space
	// ----
	LPOBJ lpObj = &gObj[aRecv->aIndex];
	// ----
	if( !gObjIsConnected(lpObj) )
	{
		return;
	}
	// ----
	if( !aRecv->Result )
	{
		gGensSystem.SendGensReward(lpObj, 1);
		return;
	}
	// ----
	if( !GensRewardInventoryCheck(lpObj, this->RewardTableList[lpObj->m_GensClass].ItemCount) )
	{
		gGensSystem.SendGensReward(lpObj, 3);
		return;
	}
	// ----
	int RewardItem = ITEMGET(this->RewardTableList[lpObj->m_GensClass].ItemID, this->RewardTableList[lpObj->m_GensClass].ItemIndex);
	// ----
	for( int i = 0; i < this->RewardTableList[lpObj->m_GensClass].ItemCount; i++ )
	{
		ItemSerialCreateSend(lpObj->m_Index, 235, 0, 0, RewardItem, this->RewardTableList[lpObj->m_GensClass].ItemLevel, 0, 0, 0, 0, 0, 0, 0);
	}
	// ----
	LogAddTD("[GensSystem] [Reward] [%s] [%s] Class: %d, Count: %d, Item: %d",
		lpObj->AccountID, lpObj->Name, lpObj->m_GensClass, this->RewardTableList[lpObj->m_GensClass].ItemCount, RewardItem);
	// ----
	PMSG_GENS_REWARD_SET pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x09, sizeof(pMsg));
	// ----
	memcpy(pMsg.Name, lpObj->Name, 10);
	// ----
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
	gGensSystem.SendGensReward(lpObj, 0);
}
// -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------
// -------------------------------------------------------------------------------
//-> Completed
void CGReqGensMemberInfo(PMSG_REQ_GENS_INFO *lpMsg, int iIndex)
{
	if (!OBJMAX_RANGE(iIndex)) return;

	LPOBJ lpObj = &gObj[iIndex];

	if (gObjIsConnected(lpObj))
	{
		if (gGensSystem.IsRegGensInfluence(lpObj))
		{
			gGensSystem.ReqExDBGensInfo(lpObj);
		}
	}
}

//--------------------------------------------------



//--------------------------------------------------

// void ExDataServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)

//-> Completed
void DGAnsGensInfo(PMSG_ANS_GENS_INFO_EXDB *lpMsg)
{
	int iObjIndex = MAKE_NUMBERW(lpMsg->wIndexH, lpMsg->wIndexL);

	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(iObjIndex))
	{
		if (!lpMsg->iGensClass)
		{
			gGensSystem.SetUserBattleZoneEnable(lpObj, BATTLE_ZONE_BLOCK);
			//gGensSystem.SetGensInfluence(lpObj, NONE_INFLUENCE);
			//-> Becoz it one more error in log only =/
			gGensSystem.SetContributePoint(lpObj, 0);
		}
		else
		{
			if (lpMsg->bInfluence != LEAVED_INFLUENCE && lpMsg->bInfluence)
			{
				gGensSystem.SetUserBattleZoneEnable(lpObj, BATTLE_ZONE_UNBLOCK);
			}

			gGensSystem.SetGensInfluence(lpObj, lpMsg->bInfluence);
			gGensSystem.SetContributePoint(lpObj, lpMsg->iContributePoint);
			gGensSystem.SetGensRanking(lpObj, lpMsg->iGensRanking);
			gGensSystem.SetGensClass(lpObj, lpMsg->iGensClass);
			gGensSystem.GensViewportListProtocol(lpObj);
			gGensSystem.CalGensClass(lpObj);
		}

		//LogAddTD("[GensSystem] Get DB GensInfo [%s][%s] GensName [%s]", lpObj->AccountID, lpObj->Name, gGensSystem.GetGensInfluenceName(lpObj));

		gGensSystem.SendGensInfo(lpObj);

		if (gGensSystem.IsMapBattleZone(lpObj->MapNumber))
		{
			if (!gGensSystem.IsUserBattleZoneEnable(lpObj))
			{
			 gObjTeleport(lpObj->m_Index, 0, 140, 125);
			 GCServerMsgStringSend(lMsg.Get(3451), lpObj->m_Index, 1);
			}
		}
	}
	else
	{
		//LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//-> Completed
void DGAnsRegGensMember(PMSG_ANS_REG_GENS_MEMBER_EXDB *lpMsg)
{
	int iObjIndex = MAKE_NUMBERW(lpMsg->bIndexH, lpMsg->bIndexL);

	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(lpObj->m_Index))
	{
		PMSG_ANS_REG_GENS_MEBMER pMsg;

		if (!lpMsg->bResult)
		{
			lpObj->m_GensClass = 14; //-> For fix error with Influence[0] becoz Class = NONE
			// ----
			gGensSystem.SetGensInfluence(lpObj, lpMsg->bInfluence);
			gGensSystem.SetUserBattleZoneEnable(lpObj, BATTLE_ZONE_UNBLOCK);
			gGensSystem.GensViewportListProtocol(lpObj);
			gGensSystem.SetContributePoint(lpObj, StartContribution);
	 		gGensSystem.GDReqSaveContributePoint(lpObj);
		}

		PHeadSubSetBE((LPBYTE)&pMsg, 0xF8, 0x02, sizeof(pMsg));
		pMsg.bResult	= lpMsg->bResult; 
		pMsg.iInfluence = lpMsg->bInfluence;
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		
		switch (lpMsg->bResult)
		{
		case 0:
			LogAddTD("[GensSystem] [%s][%s] Gens Request - Join [%s]", lpObj->AccountID, lpObj->Name, gGensSystem.GetGensInfluenceName(lpObj));
	 	break;

		case 1:
			LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Gens be Same : [%s]", lpObj->AccountID, lpObj->Name, gGensSystem.GetGensInfluenceName(lpObj));
			break;

	 case 2:
			LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Below Gens leaving Time", lpObj->AccountID, lpObj->Name);
			break;

		case 3:
		 LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Below 50Lv", lpObj->AccountID, lpObj->Name);
			break;

		case 4:
			LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Gens be Different Guild Leader [%s]", lpObj->AccountID, lpObj->Name, lpObj->lpGuild->Names);
			break;

		case 5:
			LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Not Register Guild Leader GuildName [%s]", lpObj->AccountID, lpObj->Name, lpObj->lpGuild->Name);      
			break;

		case 6:
			LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Already Partymember", lpObj->AccountID, lpObj->Name);
			break;

		case 7:
			LogAddTD("[GensSystem] [%s][%s] Gens Joining Failed to Union GuildMaster", lpObj->AccountID, lpObj->Name);
			break;

		default:
			return;
		}
	}
	else
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//-> Completed
void DGAnsSecedeGensMember(PMSG_ANS_SECEDE_GENS_MEMBER_EXDB *lpMsg)
{
	int iObjIndex = MAKE_NUMBERW(lpMsg->bIndexH, lpMsg->bIndexL);

	LPOBJ lpObj = &gObj[iObjIndex];
	
	if (gObjIsConnectedGP(lpObj->m_Index))
	{
		switch (lpMsg->bResult)
		{
		case 0:
			LogAddTD("[GensSystem] [%s][%s] Gens Request - Leave [%s]", lpObj->AccountID, lpObj->Name, gGensSystem.GetGensInfluenceName(lpObj));
			break;

		case 1:
			LogAddTD("[GensSystem] [%s][%s] Gens Leaving Failed to Not Register Gens", lpObj->AccountID, lpObj->Name);
			break;

		case 2:
			LogAddTD("[GensSystem] [%s][%s] Gens Leaving Failed to GuildMaster GensName [%s]", lpObj->AccountID, lpObj->Name, gGensSystem.GetGensInfluenceName(lpObj));
			break;

		case 3:
			LogAddTD("[GensSystem] [%s][%s] Gens Leaving Failed to Not Influence NPC : [%s]", lpObj->AccountID, lpObj->Name, gGensSystem.GetGensInfluenceName(lpObj));
			break;

		default:
			break;
		}

		if( gGensSystem.CheckGuildEnter && lpObj->GuildNumber > 0 )
		{
			lpMsg->bResult = 2;
		}
		// ----
		if (!lpMsg->bResult)
		{
			gGensSystem.SetGensInfluence(lpObj, LEAVED_INFLUENCE);
			gGensSystem.SetUserBattleZoneEnable(lpObj, BATTLE_ZONE_BLOCK);
			gGensSystem.SetContributePoint(lpObj, 0);

			memset(lpObj->m_szCharName, 0, sizeof(lpObj->m_szCharName)); 	

			lpObj->m_KillUserCnt = 0; 
			lpObj->m_KillUserTotCnt = 0;	

			gGensSystem.GensViewportListProtocol(lpObj);
			gGensSystem.SetGensClass(lpObj, 0);
			gGensSystem.SetGensRanking(lpObj, 0);
		}

		PMSG_ANS_SECEDE_GENS_MEMBER_EXDB pMsg;

		PHeadSubSetBE((LPBYTE)&pMsg, 0xF8, 0x04, sizeof(pMsg)); 
		pMsg.bResult = lpMsg->bResult;		
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, 0x05);
	}
	else
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//-> Completed
void DGAnsAbusingInfo(PMSG_ANS_ABUSING_INFO *lpMsg)
{
	//int iObjIndex = MAKE_NUMBERW(lpMsg->bIndexH, lpMsg->bIndexL);

	//LPOBJ lpObj = &gObj[iObjIndex];

	//if (gObjIsConnectedGP(iObjIndex))
	//{
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		if (strlen(lpMsg->Name) > 0)
	//		{
	//			//lpObj->m_szTargetCharName[0] = 0;
	//			//memcpy(lpObj->m_szCharName, &lpMsg->Name[0], 0x0A);
	//			//lpObj->m_KillUserCnt = lpMsg->iKillUserCount;
	//			//++lpObj->m_KillUserTotCnt;
	//		}
	//	}
	//}
	//else
	//{
	//	LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
	//}
}

//-> Completed
void DGAnsGensReward(PMSG_ANS_GENS_REWARD_EXDB *lpMsg)
{
	unsigned char btResult;

	int iObjIndex = MAKE_NUMBERW(0, 0);

	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(iObjIndex))
	{
		if (lpMsg->bResult)
		{
			btResult = lpMsg->bResult;
		}
		else
		{
			if (gGensSystem.SendGensRewardItem(lpObj, lpMsg->iGensClass))
			{
				btResult = 0;
				gGensSystem.ReqExDBGensRewardComplete(lpObj);
			}
			else
			{
				btResult = 3;
			}
		}

		LogAddTD("[GensSystem] Send Gens Ranking Reward [%s][%s] Result [%d]", lpObj->AccountID, lpObj->Name, btResult);

		gGensSystem.SendGensReward(lpObj, btResult);
	}
	else
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//-> Completed
void DGAnsGensMemberCount(PMSG_ANS_GENS_MEMBER_COUNT_EXDB *lpMsg)
{
	gGensSystem.SetGensMemberCount(1, lpMsg->iVanertMemberCount);
	gGensSystem.SetGensMemberCount(2, lpMsg->iDuprianMemberCount);
}

//-> Completed
void DGAnsGensRewardDayCheck(PMSG_ANS_GENS_REWARD_DAY_CHECK_EXDB *lpMsg)
{
	int iObjIndex = MAKE_NUMBERW(lpMsg->bIndexH, lpMsg->bIndexL);

	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(iObjIndex))
	{
		if (lpMsg->iRewardDay)
		{
			GCServerMsgStringSend(lMsg.Get(3459), iObjIndex, 1);
		}
	}
	else
	{
		LogAddTD("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//--------------------------------------------------

#endif