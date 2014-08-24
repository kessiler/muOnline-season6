// CrywolfStatue.cpp: implementation of the CCrywolfStatue class.
//	GS-N	1.00.77	JPN	0xXXXXXXXX - Completed
//	GS-CS	1.00.90	JPN	0xXXXXXXXX - Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrywolfStatue.h"
#include "CrywolfAltar.h"
#include "CrywolfUtil.h"
#include "Gamemain.h"
#include "user.h"
#include "BuffEffectSlot.h"

static CCrywolfUtil UTIL;
CCrywolfStatue g_CrywolfNPC_Statue;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolfStatue::CCrywolfStatue()
{

}

CCrywolfStatue::~CCrywolfStatue()
{

}

int CCrywolfStatue::GetStatueViewState(int iPriestNumber)
{
	int iViewState = 0;

	switch ( iPriestNumber )
	{
		case 0:
			iViewState = 0;
			break;
		case 1:
			iViewState = 1;
			break;
		case 2:
			iViewState = 2;
			break;
		case 3:
			iViewState = 4;
			break;
		case 4:
			iViewState = 8;
			break;
		case 5:
			iViewState = 256;
			break;
	}

	return iViewState;
}

void CCrywolfStatue::SetStatueViewState(LPOBJ lpObj, int iAltarNumber)
{
	switch ( iAltarNumber )
	{
		case 1:
			gObjAddBuffEffect(lpObj, AT_CRYWOLF_PROTECTION1, 0, 0, 0, 0, -10);
			break;
		case 2:
			gObjAddBuffEffect(lpObj, AT_CRYWOLF_PROTECTION2, 0, 0, 0, 0, -10);
			break;
		case 3:
			gObjAddBuffEffect(lpObj, AT_CRYWOLF_PROTECTION3, 0, 0, 0, 0, -10);
			break;
		case 4:
			gObjAddBuffEffect(lpObj, AT_CRYWOLF_PROTECTION4, 0, 0, 0, 0, -10);
			break;
		case 5:
			gObjAddBuffEffect(lpObj, AT_CRYWOLF_PROTECTION5, 0, 0, 0, 0, -10);
			break;
	}
}

void CCrywolfStatue::CrywolfStatueAct(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return;

	if ( gObj[iIndex].Type != OBJ_NPC || CHECK_CLASS(204, gObj[iIndex].Class) == FALSE )
		return;

	LPOBJ lpObj = &gObj[iIndex];
	int iContractedAlterCount = g_CrywolfNPC_Altar.GetContractedAltarCount();
	int iPriestHPSum = g_CrywolfNPC_Altar.GetPriestHPSum();
	int iPriestMaxHPSum = g_CrywolfNPC_Altar.GetPriestMaxHPSum();

	if ( iContractedAlterCount == 0 || this->m_Shield.m_iShieldHP == 0 )
	{
		if ( this->m_Shield.m_iShieldState == 1 )
		{
			UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(13, 5)));
			this->m_Shield.m_iShieldState = 0;
			this->m_Shield.m_iShieldHP = 0;
			this->m_Shield.m_iShieldMaxHP = 0;
			this->m_Shield.m_iPriestNumber = iContractedAlterCount;
			this->SetStatueViewState(lpObj, iContractedAlterCount); //season 3.0 add-on
		}
		else if ( this->m_Shield.m_iPriestNumber != iContractedAlterCount )
		{
			this->m_Shield.m_iShieldState = 1;
			this->m_Shield.m_iShieldHP = iPriestHPSum;
			this->m_Shield.m_iShieldMaxHP = iPriestMaxHPSum;
			this->m_Shield.m_iPriestNumber = iContractedAlterCount;
			UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(13, 6)), iContractedAlterCount, iPriestHPSum);
			this->SetStatueViewState(lpObj, iContractedAlterCount); //season 3.0 add-on
		}
		return;
	}
	else if ( this->m_Shield.m_iShieldState == 0 )
	{
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(13, 7)));
		this->SetStatueViewState(lpObj, iContractedAlterCount); //season 3.0 add-on
	}
	else if ( this->m_Shield.m_iPriestNumber != iContractedAlterCount )
	{
		UTIL.SendCrywolfUserAnyMsg(2, lMsg.Get(MSGGET(13, 6)), iContractedAlterCount, iPriestHPSum);
		this->SetStatueViewState(lpObj, iContractedAlterCount); //season 3.0 add-on
	}

	this->m_Shield.m_iShieldState = 1;
	this->m_Shield.m_iShieldHP = iPriestHPSum;
	this->m_Shield.m_iShieldMaxHP = iPriestMaxHPSum;
	this->m_Shield.m_iPriestNumber = iContractedAlterCount;
}