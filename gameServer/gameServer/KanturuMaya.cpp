// KanturuMaya.cpp: implementation of the CKanturuMaya class.
//	GS-CS	1.00.90	JPN	 - finished
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuMaya.h"
#include "TMonsterSkillManager.h"
#include "LogProc.h"
#include "KanturuUtil.h"

static CKanturuUtil KANTURU_UTIL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuMaya::CKanturuMaya()
{
	this->Init();
}

CKanturuMaya::~CKanturuMaya()
{
	return;
}


void CKanturuMaya::Init()
{
	this->m_iMayaObjIndex = -1;
	this->m_iMayaSkillTime = 0;
	this->m_iIceStormCount = 0;
}


void CKanturuMaya::KanturuMayaAct_IceStorm(int iSkillUsingRate)
{
	if ( (rand()%10000) > iSkillUsingRate )
		return;

	if ( this->m_iMayaObjIndex < 0 || this->m_iMayaObjIndex > OBJMAX )	// #errot change to OBJMAX-1
		return;

	for ( int iCount=OBJ_STARTUSERINDEX;iCount < OBJMAX;iCount++)
	{
		if ( gObj[iCount].Type == OBJ_USER &&
			 gObjIsConnected(iCount) &&
			 gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS )
		{
			KANTURU_UTIL.NotifyKanturuWideAreaAttack(this->m_iMayaObjIndex, iCount, 0);
			TMonsterSkillManager::UseMonsterSkill(this->m_iMayaObjIndex, iCount, 31,-1,NULL);
			this->m_iIceStormCount++;
		}
	}

	LogAddTD("[ KANTURU ][ IceStorm ] Skill Using(%d) : Index(%d) %s",
		this->m_iIceStormCount, this->m_iMayaObjIndex, gObj[this->m_iMayaObjIndex].Name);
}



//Season 4.5 identical
void CKanturuMaya::KanturuMayaAct_Hands()
{
	int TickCount = GetTickCount() - this->m_iMayaSkillTime;

	if ( TickCount < 20000 )
		return;

	this->m_iMayaSkillTime = GetTickCount();

	if ( this->m_iMayaObjIndex < 0 || this->m_iMayaObjIndex > OBJMAX )	// #errot change to OBJMAX-1
		return;

	for ( int iCount=OBJ_STARTUSERINDEX;iCount < OBJMAX;iCount++)
	{
		if ( gObj[iCount].Type == OBJ_USER &&
			 gObjIsConnected(iCount) &&
			 gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS )
		{
			KANTURU_UTIL.NotifyKanturuWideAreaAttack(this->m_iMayaObjIndex, iCount, 1);
			TMonsterSkillManager::UseMonsterSkill(this->m_iMayaObjIndex, iCount, 1,-1,NULL);

			//#if(_GSCS==0)
			if (gObj[iCount].pInventory[10].m_Type == ITEMGET(13,38) &&
				gObj[iCount].pInventory[10].m_Durability != 0.0f )
			{
				continue;
			}


			if ( gObj[iCount].pInventory[11].m_Type == ITEMGET(13,38) &&
				gObj[iCount].pInventory[11].m_Durability != 0.0f )
			{
				continue;
			}

			if ( gObj[iCount].MapNumber == MAP_INDEX_KANTURU_BOSS )
			{
				LPOBJ lpMayaHandObj = &gObj[this->m_iMayaObjIndex];
				gObj[iCount].Life = 0;

				gObjLifeCheck(&gObj[iCount], lpMayaHandObj, gObj[iCount].Life, 0, 0, 0, 0, 0);

				LogAddTD("[ KANTURU ][ BrokenShower ] [%s][%s] User Dying cause NOT wearing MoonStone Pandent",
				gObj[iCount].AccountID, gObj[iCount].Name);
			}
			//#endif

		}
	}

	LogAddTD("[ KANTURU ][ BrokenShower ] Skill Using : Index(%d) %s",
		this->m_iMayaObjIndex, gObj[this->m_iMayaObjIndex].Name);
}