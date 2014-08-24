// IllusionTempleEvent.cpp: implementation of the CIllusionTempleEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IllusionTempleEvent.h"
#include "Dirpath.h"
#include "GameMain.h"
#include "..\include\readscript.h"
#include "user.h"
#include "..\\Common\\Winutil.h"
#include "..\\Include\\ProDef.h"
#include "Protocol.h"
#include "DevilSquare.h"
#include "logproc.h"
#include "ItemDef.h"

extern CDirPath		gDirPath;
extern PartyClass	gParty;

CIllusionTempleEvent	g_IllusionTempleEvent;

int g_bIllusionTempleEvent = 1;
int g_iOldScrollDropRate = 0;
int g_iCovenantOfIllusionDropRate = 0;
int g_bUseMaxLevelIllusionTemple = 0;
int g_bSaveIllusionTempleRankingPoint = 0;
int g_iIllusionTempleRewardPercent = 100;
int g_btIllusionTempleMinUseSkillCnt = 3;
int g_iIllusionTempleRewardPercentMaster = 100;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIllusionTempleEvent::CIllusionTempleEvent()
{

}

CIllusionTempleEvent::~CIllusionTempleEvent()
{

}

BOOL CIllusionTempleEvent::IllusionTempleEventInit()
{
	for(int i=0; i<MAX_ILLUSIONTEMPLE_TEMPLE_COUNT; i++)
	{
		m_cIllusionTempleProc[i].IllusionTempleProcessInit(i);
	}
	return TRUE;
}

BOOL CIllusionTempleEvent::LoadIllusionTempleEventInfo()
{
	LoadIllusionTempleScript(gDirPath.GetNewPath("Event\\IllusionTemple.dat"));

	g_bIllusionTempleEvent					= GetPrivateProfileInt("GameServerInfo","IllusionTempleEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iOldScrollDropRate					= GetPrivateProfileInt("GameServerInfo","OldScrollDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iCovenantOfIllusionDropRate			= GetPrivateProfileInt("GameServerInfo","CovenantOfIllusionDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_bUseMaxLevelIllusionTemple			= GetPrivateProfileInt("GameServerInfo","UseMaxLevelIllusionTemple", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_bSaveIllusionTempleRankingPoint		= GetPrivateProfileInt("GameServerInfo","SaveIllusionTempleRankingPoint", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iIllusionTempleRewardPercent			= GetPrivateProfileInt("GameServerInfo","IllusionTempleRewardPercent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_btIllusionTempleMinUseSkillCnt		= GetPrivateProfileInt("GameServerInfo","IllusionTempleMinUseSkillCnt", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iIllusionTempleRewardPercentMaster	= GetPrivateProfileInt("GameServerInfo","IllusionTempleRewardPercentMaster", 0, gDirPath.GetNewPath("commonserver.cfg"));
	return TRUE;
}

BOOL CIllusionTempleEvent::LoadIllusionTempleScript(char *filename)
{
	if( (SMDFile = fopen(filename, "r")) == NULL )	//ok
	{
		MsgBox("[Illusion Temple] Info file Load Fail [%s]", filename);
		return FALSE;
	}

	SMDToken Token;
	int type;
	m_MinPlayerCount = 0; //-> Custom
	while ( true )
	{
		Token = GetToken();

		if( Token == END )	break;

		type = TokenNumber;

		while ( true )
		{
			if( type == 0 )
			{
				Token = GetToken();

				if( strcmp("end", TokenString) == 0 )	break;

				CIllusionTempleEvent::m_nIllusionTempleOpenTime = TokenNumber;
				Token = GetToken();	CIllusionTempleEvent::m_nIllusionTempleReadyTime = TokenNumber;
				Token = GetToken();	CIllusionTempleEvent::m_nIllusionTemplePlayTime = TokenNumber;
				Token = GetToken();	CIllusionTempleEvent::m_nIllusionTempleEndTime = TokenNumber;
			}
			else if( type == 1 )
			{
				Token = GetToken();

				if( strcmp("end", TokenString) == 0 )	break;

				__ILLUSION_TEMPLE_STARTTIME startTime;

				startTime.nHour = TokenNumber;
				Token = GetToken();	startTime.nMin = TokenNumber;

				m_lstIllusionTempleStartTime.push_back(startTime);
			}
			else if( type == 2 )	//-> Custom
			{
				Token = GetToken();
				
				if( strcmp("end", TokenString) == 0 )	break;
				
				m_MinPlayerCount = TokenNumber;
			}
			else if( type == 3 )
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )	break;
				// ----
				BYTE CastleIndex = TokenNumber;
				// ----
				Token = GetToken();
				this->m_ItemReward[CastleIndex].ItemCount = TokenNumber;
				// ----
				Token = GetToken();
				WORD ItemID	= TokenNumber;
				// ----
				Token = GetToken();
				WORD ItemIndex = TokenNumber;
				// ----
				this->m_ItemReward[CastleIndex].ItemID = ITEMGET(ItemID, ItemIndex);
				// ----
				Token = GetToken();
				this->m_ItemReward[CastleIndex].ItemLevel = TokenNumber;
			}
		}
	}

	fclose(SMDFile);
	LogAdd("%s file load!", filename);
	return TRUE;
}

void CIllusionTempleEvent::IllusionTempleRun()
{
	if(!g_bIllusionTempleEvent)	return;

	int nCurTime = GetTickCount();
	BOOL bCheckSync = FALSE;

	if(m_cIllusionTempleProc[0].GetRealRemainTimeMin() == 10 && !m_cIllusionTempleProc[0].GetAllChecksync())
	{
		bCheckSync = TRUE;
		m_cIllusionTempleProc[0].SetAllCheckSync();
	}

	for(int i=0; i<MAX_ILLUSIONTEMPLE_TEMPLE_COUNT ;i++)
	{
		if(bCheckSync == TRUE)
		{
			m_cIllusionTempleProc[i].CheckSync();
		}
		m_cIllusionTempleProc[i].ProcIllusionTemple(nCurTime);
	}
}

void CIllusionTempleEvent::SetInventoryPos(BYTE btMapNumber, int aIndex, BYTE btPos)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[(btMapNumber - MAP_INDEX_ILLUSION_TEMPLE1)].SetInventoryPos(aIndex, btPos);
}

void CIllusionTempleEvent::SendRelicsUser(LPOBJECTSTRUCT lpObj)
{
	if(!CHECK_LIMIT((lpObj->MapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[(lpObj->MapNumber - MAP_INDEX_ILLUSION_TEMPLE1)].SendRelicsUser(lpObj);
}

BOOL CIllusionTempleEvent::EnterIllusionTemple(int aIndex, BYTE btTempleIndex, BYTE btItemPos)
{
	if( !g_bIllusionTempleEvent )	
	{
		return FALSE;
	}
	// ----
	if(!CHECK_LIMIT(btTempleIndex, MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) 
	{
		return FALSE;
	}
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];

	PMSG_RESULT_ENTER_ILLUSION_TEMPLE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x00, sizeof(pResult));
	pResult.Result = 0;

	if( lpObj->Type != OBJ_USER || lpObj->Connected <= 2 )
	{
		return FALSE;
	}

	if( lpObj->m_IfState.use && lpObj->m_IfState.type != 12 )
	{
		return FALSE;
	}

	if( lpObj->m_bPShopOpen == true )
	{
		CGPShopReqClose(lpObj->m_Index);
	}

	BOOL PKFlag = FALSE;

	if(lpObj->PartyNumber >= 0)
	{
		if( (gParty.GetPKPartyPenalty(lpObj->PartyNumber)) >= 5)
		{
			PKFlag = TRUE;
		}
	}
	else if( lpObj->m_PK_Level >= 4 )
	{
		PKFlag = TRUE;
	}

	if( PKFlag == TRUE )
	{
		pResult.Result = 7;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return FALSE;
	}

	if( CheckWearingMOPH(aIndex) )
	{
		pResult.Result = 8;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return FALSE;
	}

	if( m_cIllusionTempleProc[btTempleIndex].GetIllusionTempleState() != 0 
		|| !m_cIllusionTempleProc[btTempleIndex].GetEntrance() )
	{
		pResult.Result = 2;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return FALSE;
	}

	int iITEM_LEVEL;
	DWORD dwITEM_SERIAL;

	if(gObj[aIndex].pInventory[btItemPos].IsItem() == TRUE)
	{
		if(gObj[aIndex].pInventory[btItemPos].m_Type == (ITEM_HELPER+51))
		{
			iITEM_LEVEL = gObj[aIndex].pInventory[btItemPos].m_Level;
			dwITEM_SERIAL = gObj[aIndex].pInventory[btItemPos].m_Number;

			if( !CHECK_LIMIT( iITEM_LEVEL-1, MAX_ILLUSIONTEMPLE_TEMPLE_COUNT ) && gObj[aIndex].pInventory[btItemPos].m_Durability != 1.0f)
			{
				pResult.Result = 1;
				DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
				return FALSE;
			}
		}
		else if(gObj[aIndex].pInventory[btItemPos].m_Type == (ITEM_HELPER+61))
		{
			dwITEM_SERIAL = gObj[aIndex].pInventory[btItemPos].m_Number;
			iITEM_LEVEL = CheckCanEnterLevel(aIndex);
			
			if(iITEM_LEVEL <= 0)
			{
				pResult.Result = 1;
				DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
				return FALSE;
			}
		}
		else
		{
			pResult.Result = 1;
			DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
			return FALSE;
		}
	}
	else
	{
		pResult.Result = 1;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return FALSE;
	}

#pragma message("#### Enable me after time ####")
	//if( GEReqIllusionTempleEnterCountCheck(aIndex, btTempleIndex, btItemPos, iITEM_LEVEL) )
	//{
	//	LogAddC(2, "[DEBUG] [IT] Enter R12");
	//	return TRUE;
	//}
	if( !CheckEnterLevel(aIndex, iITEM_LEVEL) )
	{
		pResult.Result = 3;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return FALSE;
	}

	m_cIllusionTempleProc[iITEM_LEVEL-1].CheckUsersOnConnect();

	BOOL bRet = m_cIllusionTempleProc[iITEM_LEVEL-1].EnterUserIllusionTemple(aIndex, btTempleIndex, btItemPos);

	if(bRet == TRUE)
	{
		if(lpObj->PartyNumber >= 0)
		{
			PMSG_PARTYDELUSER pMsg;

			int pindex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
			if(pindex >= 0)
			{
				pMsg.Number = pindex;
				CGPartyDelUser(&pMsg, lpObj->m_Index);
			}
		}

		if( lpObj->pInventory[btItemPos].m_Type == (ITEM_HELPER+51) )
		{
			lpObj->pInventory[btItemPos].m_Durability = 0;
			GCItemDurSend(lpObj->m_Index, btItemPos, lpObj->pInventory[btItemPos].m_Durability, 0);
		}
		else if(lpObj->pInventory[btItemPos].m_Type == (ITEM_HELPER+61) && lpObj->pInventory[btItemPos].m_Durability > 0.0f)
		{
			lpObj->pInventory[btItemPos].m_Durability -= 1.0f;
			GCItemDurSend(lpObj->m_Index, btItemPos, lpObj->pInventory[btItemPos].m_Durability, 0);
		}

#ifdef OPT
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u). PCRoom:%d", iITEM_LEVEL, lpObj->AccountID, lpObj->Name, lpObj->DbClass, lpObj->pInventory[btItemPos].m_Number, lpObj->m_iPcBangRoom);
#else
		LogAddTD("[Illusion Temple] (%d) EnterUser: (%s)(%s) class:%d (Serial:%u)", iITEM_LEVEL, lpObj->AccountID, lpObj->Name, lpObj->DbClass, lpObj->pInventory[btItemPos].m_Number);
#endif
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);

		gObjMoveGate(lpObj->m_Index, iITEM_LEVEL+141);
	}

	m_cIllusionTempleProc[btTempleIndex].SendIllusionTempleState(0, lpObj->m_Index);
	SendIllusionTempleUserCount();

#if(TESTSERVER==1)
	GCServerMsgStringSend("CIllusionTempleEvent::EnterIllusionTemple", lpObj->m_Index, 0);
#endif
	return FALSE;
}

BOOL CIllusionTempleEvent::GEReqIllusionTempleEnterCountCheck(int aIndex, BYTE btTempleIndex, BYTE btItemPos, int nItemLevel)
{
	if(DevilSquareEventConnect) return FALSE;
	
	PMSG_REQ_ILLUSION_TMPLE_ENTERCOUNTCHECK pMsg;	
	PHeadSetB((LPBYTE)&pMsg, 0x12, sizeof(pMsg));
	
	memcpy(pMsg.AccountID, gObj[aIndex].AccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID, gObj[aIndex].Name, MAX_IDSTRING);
	
	pMsg.ServerCode = gGameServerCode;
	pMsg.iObjIndex = aIndex;
	pMsg.iItemPos = btItemPos;
	pMsg.iItemLevel = nItemLevel;
	pMsg.iTempleNumber = btTempleIndex;
	
	DataSendRank((char *)&pMsg, pMsg.h.size);
	return TRUE;
}

void CIllusionTempleEvent::EGAnsIllusionTempleEnterCountCheck(LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK aRecv)
{
}

void CIllusionTempleEvent::GEReqIllusionTempleIncEnterCount(LPOBJECTSTRUCT lpObj)
{
	PMSG_REQ_ILLUSION_TEMPLE_INC_COUNT pMsg;	
	PHeadSetB((LPBYTE)&pMsg, 0x13, sizeof(pMsg));
	
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_IDSTRING);
	memcpy(pMsg.GameID, lpObj->Name, MAX_IDSTRING);
	pMsg.ServerCode = gGameServerCode;
	
	DataSendRank((char *)&pMsg, pMsg.h.size);
}

BOOL CIllusionTempleEvent::LeaveIllusionTemple(int aIndex, BYTE btMapNumber)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return FALSE;

	m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].LeaveUserIllusionTemple(aIndex);
	SendIllusionTempleUserCount();	
	return TRUE;
}

BOOL CIllusionTempleEvent::CheckEnterLevel(int nIndex, int nItemLevel)
{
	int nIllusionTempleNumber = -1;
	if( gObj[nIndex].Level < 220 )
	{
		nIllusionTempleNumber = -1;
	}
	else if( gObj[nIndex].Level < 271 )
	{
		nIllusionTempleNumber = 0;
	}
	else if( gObj[nIndex].Level < 321 )
	{
		nIllusionTempleNumber = 1;
	}
	else if( gObj[nIndex].Level < 351 )
	{
		nIllusionTempleNumber = 2;
	}
	else if( gObj[nIndex].Level < 381 )
	{
		nIllusionTempleNumber = 3;
	}
	else if( gObj[nIndex].Level < 400 )
	{
		nIllusionTempleNumber = 4;
	}
	else if( gObj[nIndex].Level == 400 && !gObj[nIndex].ThirdChangeUp )
	{
		nIllusionTempleNumber = 4;
	}
	else if( gObj[nIndex].Level == 400 && gObj[nIndex].ThirdChangeUp )
	{
		if( g_bUseMaxLevelIllusionTemple ){
			nIllusionTempleNumber = 5;
		}
		else nIllusionTempleNumber = 4;
	}
	else nIllusionTempleNumber = -1;	
	if(nIllusionTempleNumber+1 == nItemLevel)
	{
		return TRUE;
	}
	return FALSE;
}

void CIllusionTempleEvent::IllusionHolyRelicsStatusAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].IllusionHolyRelicsStatusAct(lpNpc, lpObj);
}

void CIllusionTempleEvent::HolyRelicsBoxAlliedAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].HolyRelicsBoxAlliedAct(lpNpc, lpObj);
}

void CIllusionTempleEvent::HolyRelicsBoxIllusionAct(BYTE btMapNumber, LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].HolyRelicsBoxIllusionAct(lpNpc, lpObj);
}

void CIllusionTempleEvent::PlatformMirageAct(LPOBJECTSTRUCT lpNpc, LPOBJECTSTRUCT lpObj)
{
	if( lpObj->m_IfState.use && lpObj->m_IfState.type != 12 )
	{
		return;
	}
	// ----
	if( lpObj->m_PK_Level >= 4 )
	{
		GCServerMsgStringSend(lMsg.Get(3400), lpObj->m_Index, 1);
		return;
	}
	// ----
	int nIllusionTempleNumber = -1;
	// ----
	if( lpObj->Level < 220 )
	{
		nIllusionTempleNumber = -1;
	}
	else if( lpObj->Level < 271 )
	{
		nIllusionTempleNumber = 0;
	}
	else if( lpObj->Level < 321 )
	{
		nIllusionTempleNumber = 1;
	}
	else if( lpObj->Level < 351 )
	{
		nIllusionTempleNumber = 2;
	}
	else if( lpObj->Level < 381 )
	{
		nIllusionTempleNumber = 3;
	}
	else if( lpObj->Level < 400 )
	{
		nIllusionTempleNumber = 4;
	}
	else if( lpObj->Level == 400 && !lpObj->ThirdChangeUp )
	{
		nIllusionTempleNumber = 4;
	}
	else if( lpObj->Level == 400 && lpObj->ThirdChangeUp )
	{
		if( g_bUseMaxLevelIllusionTemple )
		{
			nIllusionTempleNumber = 5;
		}
		else 
		{
			nIllusionTempleNumber = 4;
		}
	}
	else
	{
		nIllusionTempleNumber = -1;
	}
	// ----
	if( !CHECK_LIMIT(nIllusionTempleNumber, MAX_ILLUSIONTEMPLE_TEMPLE_COUNT) )
	{
		GCServerMsgStringSend(lMsg.Get(3401), lpObj->m_Index, 1);
		return;
	}
	// ----
	int nItempos = m_cIllusionTempleProc[nIllusionTempleNumber].SearchTicketItem(lpObj->m_Index);
	// ----
	if( nItempos == -1 )
	{
		GCServerMsgStringSend(lMsg.Get(3402), lpObj->m_Index, 1);
		return;
	}
	// ----
	if( m_cIllusionTempleProc[nIllusionTempleNumber].GetIllusionTempleState() != 0 )
	{
		GCServerMsgStringSend(lMsg.Get(3403), lpObj->m_Index, 1);
		return;
	}
	// ----
	if( m_cIllusionTempleProc[nIllusionTempleNumber].GetIllusionTempleState() != 0 
		|| m_cIllusionTempleProc[nIllusionTempleNumber].GetEntrance() == FALSE )
	{
		GCServerMsgStringSend(lMsg.Get(3404), lpObj->m_Index, 1);
		return;
	}
	// ----
	PMSG_TALKRESULT pMsg;
	// ----
	pMsg.h.c		= PMHCE_BYTE;
	pMsg.h.headcode = 0x30;
	pMsg.h.size		= sizeof(pMsg);
	pMsg.result		= 0x14;
	pMsg.level1		= this->m_cIllusionTempleProc[nIllusionTempleNumber].GetRemainTimeMin();
	pMsg.level2		= this->m_cIllusionTempleProc[nIllusionTempleNumber].GetUserCount();
	// ----
	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
}

void CIllusionTempleEvent::DeleteRelicsItem(BYTE btMapNumber, LPOBJECTSTRUCT lpObj)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].DeleteRelicsItem(lpObj);
}

void CIllusionTempleEvent::DropRelicsItem(BYTE btMapNumber, int nIndex)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].DropRelicsItem(nIndex);
}

void CIllusionTempleEvent::SetIllusionTempleState(BYTE btState, BYTE btMapNumber)
{
#if(TESTSERVER==1)
#endif
}

BYTE CIllusionTempleEvent::GetIllusionTempleState(BYTE btMapNumber)
{
	return m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].GetIllusionTempleState();
}

void CIllusionTempleEvent::AddNpcPosNum(BYTE btMapNumber, int nClass, int nPosNum)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].AddNpcPosNum(nClass, nPosNum);
}

void CIllusionTempleEvent::AddMonsterPosNum(BYTE btMapNumber, int nClass, int nPosNum)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].AddMonsterPosNum(nClass, nPosNum);
}

void CIllusionTempleEvent::ResetPosNum()
{
	for (int i=0; i<MAX_ILLUSIONTEMPLE_TEMPLE_COUNT; i++)
	{
		m_cIllusionTempleProc[i].ResetPosNum();
	}
}

BYTE CIllusionTempleEvent::GetUserTeam(BYTE btMapNumber, int nIndex)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return 0xFF;
	return m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].GetUserTeam(nIndex);
}

void CIllusionTempleEvent::SetStatusRegenTime(BYTE btMapNumber)
{
	if(!CHECK_LIMIT((btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT)) return;
	m_cIllusionTempleProc[btMapNumber-MAP_INDEX_ILLUSION_TEMPLE1].SetStatusRegenTime();
}

#include "MixSystem.h"
#include "CrywolfSync.h"
#include "CastleSiegeSync.h"
#include "DSProtocol.h"

void CIllusionTempleEvent::IllusionTempleChaosMixItem(LPOBJECTSTRUCT lpObj)
{
		lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	int aIndex;
	int ebp18;
	int iRET_VAL;

	aIndex = lpObj->m_Index;
	ebp18 = 0;
	iRET_VAL = this->CheckChaosMixItem(aIndex);
	ebp18 = this->CheckCharmCount(aIndex);

	if(ebp18 > 10)
	{
		iRET_VAL = 15;
	}

	if ( IT_MAP_RANGE((iRET_VAL+(45-1))) != FALSE )
	{
		if(this->IllusionTempleChaosMix(aIndex, iRET_VAL, ebp18) == FALSE)
		{
			lpObj->ChaosLock = FALSE;
		}
		return;
	}

	switch ( iRET_VAL )
	{
		case 9:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 10:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 11:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 12:
			pMsg.Result = CB_INVALID_ITEM_LEVEL	;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 13:
			pMsg.Result = CB_BC_NOT_ENOUGH_ZEN	;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;

		case 14:
			pMsg.Result = CB_USER_CLASS_LOW_LEVEL	;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->m_Index;
			lpObj->ChaosLock = FALSE;
			break;
		case 15:
			pMsg.Result = 0xF0;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;
		default:
			lpObj->ChaosLock = FALSE;
	}
}

static const int g_iIT_ChoasMixSuccessRate[MAX_ILLUSIONTEMPLE_TEMPLE_COUNT] = {70, 70, 70, 70, 70, 70};
static const int g_iIT_ChoasMixMoney[MAX_ILLUSIONTEMPLE_TEMPLE_COUNT] = {3000000 , 5000000 , 7000000, 9000000, 11000000, 13000000};

BYTE CIllusionTempleEvent::IllusionTempleChaosMix(int nIndex, int nLEVEL, int iCharmOfLuckCount)
{
	if( OBJMAX_RANGE(nIndex) == FALSE )
	{
		return FALSE;
	}

	if ( IT_MAP_RANGE((nLEVEL+(45-1))) == FALSE )
	{
		return FALSE;
	}

	int loc2 = 0;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	gObj[nIndex].ChaosLock = TRUE;
	LogAddTD("[Illusion Temple] 피의 두루마리 Mix Chaos Mix Start (Account:%s, Name:%s, Level:%d)", gObj[nIndex].AccountID, gObj[nIndex].Name, nLEVEL);

	char szTemp[17];
	wsprintf(szTemp, "피의 두루마리 Mix,%d", nLEVEL);

	g_MixSystem.LogChaosItem(&gObj[nIndex], szTemp);

	int iMIX_SUCCESS_RATE = g_iIT_ChoasMixSuccessRate[nLEVEL-1]; //loc11

	if ( iMIX_SUCCESS_RATE < 0 || iMIX_SUCCESS_RATE > 100 )
	{
		DataSend(nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Illusion Temple] 피의 두루마리 Mix Chaos Mix Failed - MixRate Out of Bound (Account:%s, Name:%s, Level:%d)", gObj[nIndex].AccountID, gObj[nIndex].Name, nLEVEL);
		return FALSE;
	}

	if ( g_CrywolfSync.GetOccupationState() == 0 && g_iCrywolfApplyMvpBenefit )
	{
		iMIX_SUCCESS_RATE += g_CrywolfSync.GetPlusChaosRate();
	}

	int iMIX_NEED_MONEY = g_iIT_ChoasMixMoney[nLEVEL-1]; //loc12

	int iChaosTaxMoney = (int)((__int64)(iMIX_NEED_MONEY) * (__int64)(g_CastleSiegeSync.GetTaxRateChaos(nIndex)) / (__int64)100); //loc13

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iMIX_NEED_MONEY += iChaosTaxMoney;

	if ( iMIX_NEED_MONEY <  0 )
	{
		DataSend(nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Illusion Temple] 피의 두루마리 Mix Chaos Mix Failed - MixMoney < 0 (Account:%s, Name:%s, Level:%d)",	gObj[nIndex].AccountID, gObj[nIndex].Name, nLEVEL);
		return FALSE;
	}

	if ( (gObj[nIndex].Money - iMIX_NEED_MONEY) < 0 )
	{
		pMsg.Result = CB_BC_NOT_ENOUGH_ZEN;
		DataSend(nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[Illusion Temple] 피의 두루마리 Mix Chaos Mix Failed - Not Enough Money (Account:%s, Name:%s, Level:%d)",	gObj[nIndex].AccountID, gObj[nIndex].Name, nLEVEL);
		return FALSE;
	}

	gObj[nIndex].Money -= iMIX_NEED_MONEY;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GCMoneySend(nIndex, gObj[nIndex].Money);

	iMIX_SUCCESS_RATE += iCharmOfLuckCount; //season 3.0 moved

	if ( (rand()%100) < iMIX_SUCCESS_RATE )	// Success (season 3.0 changed)
	{
		int item_num = ITEMGET(13,51);	// Blood Scroll loc14
		ItemSerialCreateSend(nIndex, -1, 0, 0, item_num, nLEVEL, 0, 0, 0, 0, -1, 0, 0);
		LogAddTD("[피의 두루마리 Mix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",	gObj[nIndex].AccountID, gObj[nIndex].Name, iMIX_SUCCESS_RATE, gObj[nIndex].Money, iMIX_NEED_MONEY, iCharmOfLuckCount);
	}
	else
	{
		g_MixSystem.ChaosBoxInit(&gObj[nIndex]);
		GCUserChaosBoxSend(&gObj[nIndex], 0);
		DataSend(nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		LogAddTD("[피의 두루마리 Mix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d", gObj[nIndex].AccountID, gObj[nIndex].Name, iMIX_SUCCESS_RATE, gObj[nIndex].Money, iMIX_NEED_MONEY, iCharmOfLuckCount);
		return FALSE;
	}

	::gObjInventoryCommit(nIndex);
	return TRUE;
}

BOOL CIllusionTempleEvent::CheckChaosMixItem(int nIndex)
{
	 int result; // eax@5
  int v3; // [sp+4Ch] [bp-40h]@37
  int v4; // [sp+50h] [bp-3Ch]@32
  int v5; // [sp+54h] [bp-38h]@2
  signed int i; // [sp+60h] [bp-2Ch]@6
  signed int iCharmOfLuckCount; // [sp+64h] [bp-28h]@6
  int iCovenantOfIllusionLevel; // [sp+68h] [bp-24h]@6
  int iOldScroolLevel; // [sp+6Ch] [bp-20h]@6
  signed int iEventItemCount; // [sp+70h] [bp-1Ch]@6
  signed int bIsOtherItemExist; // [sp+74h] [bp-18h]@6
  signed int bIsCovenantOfIllusionExist; // [sp+78h] [bp-14h]@6
  signed int bIsOldScrollExist; // [sp+7Ch] [bp-10h]@6
  signed int bIsChaosGemExist; // [sp+80h] [bp-Ch]@6

  if( OBJMAX_RANGE(nIndex) == FALSE )
	{
		return FALSE;
	}

    bIsChaosGemExist = 0;
    bIsOldScrollExist = 0;
    bIsCovenantOfIllusionExist = 0;
    bIsOtherItemExist = 0;
    iEventItemCount = 0;
    iOldScroolLevel = 0;
    iCovenantOfIllusionLevel = 0;
    iCharmOfLuckCount = 0;
    for ( i = 0; i < 32; ++i )
    {
      if ( gObj[nIndex].pChaosBox[i].IsItem() == 1 )
      {
        switch ( gObj[nIndex].pChaosBox[i].m_Type )
        {
          case 0x180F:
            bIsChaosGemExist = 1;
            break;
          case 0x1A31:
            ++iEventItemCount;
            bIsOldScrollExist = 1;
            iOldScroolLevel = gObj[nIndex].pChaosBox[i].m_Level;
            break;
          case 0x1A32:
            ++iEventItemCount;
            bIsCovenantOfIllusionExist = 1;
            iCovenantOfIllusionLevel = gObj[nIndex].pChaosBox[i].m_Level;
            break;
          case 0x1C35:
            iCharmOfLuckCount = (signed __int64)((double)iCharmOfLuckCount + gObj[nIndex].pChaosBox[i].m_Durability);
            break;
          default:
            bIsOtherItemExist = 1;
            break;
        }
      }
    }
    if ( bIsOtherItemExist )
    {
      result = 8;
    }
    else
    {
      if ( bIsOldScrollExist || bIsCovenantOfIllusionExist )
      {
        if ( bIsOldScrollExist && bIsCovenantOfIllusionExist )
        {
          if ( iEventItemCount <= 2 )
          {
            if ( iOldScroolLevel == iCovenantOfIllusionLevel )
            {
              if ( iOldScroolLevel - 1 >= 0 )
                v4 = iOldScroolLevel - 1 <= 5;
              else
                v4 = 0;
              if ( v4 )
              {
                if ( iCovenantOfIllusionLevel - 1 >= 0 )
                  v3 = iCovenantOfIllusionLevel - 1 <= 5;
                else
                  v3 = 0;
                if ( v3 )
                {
                  if ( bIsChaosGemExist )
                  {
                    if ( gObj[nIndex].Level >= 10 )
                    {
                      if ( bIsChaosGemExist && bIsOldScrollExist && bIsCovenantOfIllusionExist )
                        result = iOldScroolLevel;
                      else
                        result = 0;
                    }
                    else
                    {
                      result = 14;
                    }
                  }
                  else
                  {
                    result = 10;
                  }
                }
                else
                {
                  result = 9;
                }
              }
              else
              {
                result = 9;
              }
            }
            else
            {
              result = 9;
            }
          }
          else
          {
            result = 12;
          }
        }
        else
        {
          result = 11;
        }
      }
      else
      {
        result = 0;
      }
    }

  return result;
}

BYTE CIllusionTempleEvent::IncSkillPoint(int nIndex, BYTE btMapNumber, BYTE btObjectType)
{
if( OBJMAX_RANGE(nIndex) == FALSE )
	{
		return FALSE;
	}

	if( CHECK_LIMIT( (btMapNumber - 45), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT) == FALSE )
	{
		return FALSE;
	}

	BYTE btPoint = 0;

	if(btObjectType == 1)
	{
		this->m_cIllusionTempleProc[btMapNumber - 45].IncUserKillCount(nIndex);
		btPoint = 5;
	}
	else
	{
		this->m_cIllusionTempleProc[btMapNumber - 45].IncMonsterKillCount(nIndex);
		btPoint = 2;
	}

	return this->m_cIllusionTempleProc[btMapNumber - 45].IncSkillPoint(nIndex, btPoint);
}

BYTE CIllusionTempleEvent::DecSkillPoint(int nIndex, BYTE btMapNumber, BYTE btDecPoint)
{
	if( OBJMAX_RANGE(nIndex) == FALSE )
	{
		return FALSE;
	}

	if( CHECK_LIMIT( (btMapNumber - 45), MAX_ILLUSIONTEMPLE_TEMPLE_COUNT) == FALSE )
	{
		return FALSE;
	}

	return this->m_cIllusionTempleProc[btMapNumber - 45].DecSkillPoint(nIndex, btDecPoint);
}

void CIllusionTempleEvent::IllusionTempleUseSkill(int iIndex, WORD MagicNumber, int wTargetObjIndex, BYTE btDis)
{
	if( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	if(IT_MAP_RANGE(gObj[iIndex].MapNumber) == FALSE)
	{
		return;
	}

	this->m_cIllusionTempleProc[gObj[iIndex].MapNumber - 45].IllusionTempleUseSkill(iIndex, MagicNumber, wTargetObjIndex, btDis);
}

void CIllusionTempleEvent::EventSkillProc(LPOBJECTSTRUCT lpObj)
{
	this->m_cIllusionTempleProc[lpObj->MapNumber - 45].IllusionTempleSkillProc(lpObj);
}

void CIllusionTempleEvent::IllusionTempleUserDie(LPOBJECTSTRUCT lpObj)
{
	this->m_cIllusionTempleProc[lpObj->MapNumber - 45].IllusionTempleUserDie(lpObj);
}

void CIllusionTempleEvent::IllusionTempleUserDieRegen(LPOBJECTSTRUCT lpObj)
{
	if( OBJMAX_RANGE(lpObj->m_Index) == FALSE )
	{
		return;
	}

	if(IT_MAP_RANGE(lpObj->MapNumber) == FALSE || this->m_cIllusionTempleProc[lpObj->MapNumber - 45].GetIllusionTempleState() != 2)
	{
		return;
	}

	this->m_cIllusionTempleProc[lpObj->MapNumber - 45].UseSkillProdection(lpObj);
}

void CIllusionTempleEvent::IncUseSkillCount(int nIndex, BYTE btMapNumber)
{
	if( OBJMAX_RANGE(nIndex) == FALSE )
	{
		return;
	}

	if( IT_MAP_RANGE(btMapNumber) == FALSE || this->m_cIllusionTempleProc[btMapNumber - 45].GetIllusionTempleState() != 2)
	{
		return;
	}

	this->m_cIllusionTempleProc[btMapNumber - 45].IncUseSkillCount(nIndex);
}

void CIllusionTempleEvent::SetMirageIndex(int nIndex)
{
	this->m_nMirageIndex = nIndex;
}

struct PMSG_ILLUSION_TEMPLE_USER_COUNT // 0xa
{
    struct PBMSG_HEAD2 h; // +0x0(0x4)
    BYTE btUserCount[0x6]; // +0x4(0x6)
};

void CIllusionTempleEvent::SendIllusionTempleUserCount()
{
	PMSG_ILLUSION_TEMPLE_USER_COUNT pMsg; // [sp+4Ch] [bp-10h]@3

  if ( !this->m_cIllusionTempleProc[1].GetIllusionTempleState()
    && this->m_cIllusionTempleProc[1].GetEntrance() == 1 )
  {
    PHeadSubSetB(&pMsg.h.c, -65, 3, 10);
    pMsg.btUserCount[0] = this->m_cIllusionTempleProc[0].GetUserCount();
    pMsg.btUserCount[1] = this->m_cIllusionTempleProc[1].GetUserCount();
    pMsg.btUserCount[2] = this->m_cIllusionTempleProc[2].GetUserCount();
    pMsg.btUserCount[3] = this->m_cIllusionTempleProc[3].GetUserCount();
    pMsg.btUserCount[4] = this->m_cIllusionTempleProc[4].GetUserCount();
    if ( g_bUseMaxLevelIllusionTemple )
      pMsg.btUserCount[5] = 0;
    else
      pMsg.btUserCount[5] = this->m_cIllusionTempleProc[5].GetUserCount();

    MsgSendV2(&gObj[this->m_nMirageIndex],(LPBYTE)&pMsg,pMsg.h.size);
  }
}

BOOL CIllusionTempleEvent::CheckWearingMOPH(int nUserIndex)
{
	if (  gObjIsConnected(nUserIndex) == FALSE )
	{
		return FALSE;
	}

	if ( gObj[nUserIndex].pInventory[10].IsItem()  )
	{
		if ( gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,10) )
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,39) ) 
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,40) )
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,41) )
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,68) ) //season4 add-on
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,76) ) //season4.6 add-on
		{
			return TRUE;
		}
	}

	if ( gObj[nUserIndex].pInventory[11].IsItem() )
	{
		if ( gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,10) )
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,39) ) 
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,40) )
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,41) )
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,68) ) //season4 add-on
		{
			return TRUE;
		}

		if ( gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,76) ) //season4,6 add-on
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CIllusionTempleEvent::CheckChangeRing(int nType)
{
	int bRet = 0;

	switch(nType)
	{
		case ITEMGET(13,10):
		case ITEMGET(13,39):
		case ITEMGET(13,40):
		case ITEMGET(13,41):
		case ITEMGET(13,68): //season4 add-on
		case ITEMGET(13,76): //season4.6 add-on
			bRet = 1;
			break;
	}

	return bRet;
}

BOOL CIllusionTempleEvent::CheckRelics(int nIndex)
{
	if (  gObjIsConnected(nIndex) == FALSE )
	{
		return FALSE;
	}

	if( OBJMAX_RANGE(nIndex) == FALSE )
	{
		return FALSE;
	}

	if(IT_MAP_RANGE(gObj[nIndex].MapNumber) == FALSE || this->m_cIllusionTempleProc[gObj[nIndex].MapNumber - 45].GetIllusionTempleState() != 2)
	{
		return FALSE;
	}

	if(nIndex == this->m_cIllusionTempleProc[gObj[nIndex].MapNumber - 45].GetRelicsUserIndex())
	{
		return TRUE;
	}

	return FALSE;
}

void CIllusionTempleEvent::MonsterDieItemDrop(LPOBJECTSTRUCT lpObj)
{
	if(IT_MAP_RANGE(lpObj->MapNumber) == FALSE)
	{
		return;
	}

	this->m_cIllusionTempleProc[lpObj->MapNumber - 45].MonsterDieItemDrop(lpObj);
}

void CIllusionTempleEvent::ReqEventReward(int nIndex)
{
	if(IT_MAP_RANGE(gObj[nIndex].MapNumber) == FALSE)
	{
		return;
	}

	this->m_cIllusionTempleProc[gObj[nIndex].MapNumber - 45].DropChaosGem(nIndex);
}

BYTE CIllusionTempleEvent::GetRemainTime()
{
	BYTE nRemainTime = 0;
	if ( g_bIllusionTempleEvent )
    nRemainTime = this->m_cIllusionTempleProc[0].GetEnterRemainTimeMin();

  return nRemainTime;
}

BOOL CIllusionTempleEvent::CheckSkillProdection(int iIllusionTempleIndex, BYTE btMapNumber)
{
	if(IT_MAP_RANGE(btMapNumber) == FALSE)
	{
		return FALSE;
	}
	return this->m_cIllusionTempleProc[btMapNumber - 45].CheckSkillProdection(iIllusionTempleIndex);
}

BOOL CIllusionTempleEvent::CheckSkillRestraint(int iIllusionTempleIndex, BYTE btMapNumber)
{
	if(IT_MAP_RANGE(btMapNumber) == FALSE)
	{
		return FALSE;
	}
	return this->m_cIllusionTempleProc[btMapNumber - 45].CheckSkillRestraint(iIllusionTempleIndex);
}

BOOL CIllusionTempleEvent::CheckCanEnterLevel(int aIndex)
{
		int result = 0;

	LPOBJ lpObj = &gObj[aIndex];

	if( OBJMAX_RANGE(aIndex) == FALSE )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	if( lpObj->Level < 220 ) return 0;
	if( lpObj->Level < 271 ) return 1;
	if( lpObj->Level < 321 ) return 2;
	if( lpObj->Level < 351 ) return 3;
	if( lpObj->Level < 381 ) return 4;
	if( lpObj->Level < 400 ) return 5;
	if( lpObj->Level == 400 && lpObj->ThirdChangeUp == 0 ) return 5;

	if( lpObj->Level == 400 && lpObj->ThirdChangeUp != 0 )
	{ 
		if( g_bUseMaxLevelIllusionTemple != 0 )
		{
			return 6;
		}
		else
		{ 
			return 5;
		}
	}

	return -1;
}

BOOL CIllusionTempleEvent::CheckCharmCount(int aIndex)
{
	if( OBJMAX_RANGE(aIndex) == FALSE )
	{
		LogAdd("return %s %d", __FILE__, __LINE__);
		return -1;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++) //loc4
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
		}
	}
	return iCharmOfLuckCount;
}

struct PMSG_REQ_ILLUSIONTEMPLE_ENTERCOUNT // 0x20
{
    struct PBMSG_HEAD h; // +0x0(0x3)
    char AccountID[0xa]; // +0x3(0xa)
    char GameID[0xa]; // +0xd(0xa)
    int ServerCode; // +0x18(0x4)
    int iObjIndex; // +0x1c(0x4)
};

void CIllusionTempleEvent::EGReqIllusionTempleEnterCount(int iIndex)
{
	PMSG_REQ_ILLUSIONTEMPLE_ENTERCOUNT pMsg; // [sp+4Ch] [bp-24h]@2

  if ( gObjIsConnected(iIndex) )
  {
    pMsg.h.c = -63;
    pMsg.h.headcode = 20;
    memcpy(pMsg.AccountID, gObj[iIndex].AccountID, 0xAu);
    memcpy(pMsg.GameID, gObj[iIndex].Name, 0xAu);
    pMsg.ServerCode = gGameServerCode / 20;
    pMsg.iObjIndex = iIndex;
    DataSendRank((char*)&pMsg, pMsg.h.size);
  }
}

struct PMSG_ANS_CL_ENTERCOUNT // 0x5
{
    struct PBMSG_HEAD h; // +0x0(0x3)
    BYTE btEventType; // +0x3(0x1)
    BYTE btLeftEnterCount; // +0x4(0x1)
};

void CIllusionTempleEvent::EGAnsIllusionTempleEnterCount(LPPMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT lpMsg)
{
	if ( !lpMsg)
		return;

	if ( !gObjIsConnected(lpMsg->iObjIndex))
		return;

	char szAccountID[11] = {0};
	char szName[11] = {0};
	memcpy(szAccountID, lpMsg->AccountID, 10);
	memcpy(szName, lpMsg->GameID, 10);

	if ( strcmp(gObj[lpMsg->iObjIndex].AccountID, szAccountID) || strcmp(gObj[lpMsg->iObjIndex].Name, szName) )
		 return;

	PMSG_ANS_CL_ENTERCOUNT pMsgSend;

	pMsgSend.h.c = 0xC1;
	pMsgSend.h.headcode = 0x9F;
	pMsgSend.h.size = sizeof(pMsgSend);
	pMsgSend.btEventType = 3;
	pMsgSend.btLeftEnterCount = lpMsg->iLeftCount;

	DataSend(lpMsg->iObjIndex, (LPBYTE)&pMsgSend, sizeof(pMsgSend));
}