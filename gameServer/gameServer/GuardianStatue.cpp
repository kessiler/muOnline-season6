// GuardianStatue.cpp: implementation of the CGuardianStatue class.
//	GS-CS		1.00.90		0xXXXXXXXX  - complete
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GuardianStatue.h"
#include "logproc.h"
#include "User.h"

//#if (_GSCS==1)

CGuardianStatue g_CsNPC_GuardianStatue;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuardianStatue::CGuardianStatue()
{
	return;
}

CGuardianStatue::~CGuardianStatue()
{
	return;
}


void CGuardianStatue::GuardianStatueAct(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->VPCount < 1 ) 
		return;

	int tObjNum = -1;

	for (int i=0;i<MAX_VIEWPORT;i++)
	{
		tObjNum = lpObj->VpPlayer[i].number;

		if ( tObjNum >= 0 )
		{
			if ( gObj[tObjNum].Type == OBJ_USER && gObj[tObjNum].Live )
			{
				if ( gObj[tObjNum].m_btCsJoinSide == 1 )
				{
					if ( abs(lpObj->Y - gObj[tObjNum].Y) <= 3 &&
						 abs(lpObj->X - gObj[tObjNum].X) <= 3 )
					{
						BOOL bLifeChange = FALSE;
						BOOL bManaChange = FALSE;
						BOOL bBpChange = FALSE;

						/*gObj[tObjNum].Life += 100.0f;
						gObj[tObjNum].Mana += 100.0f;
						gObj[tObjNum].BP += 100;*/

						if ( gObj[tObjNum].Life < (gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife))
						{
							gObj[tObjNum].Life += ( ( gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife ) * (lpObj->m_btCsNpcRgLevel+1) ) / 100.0f;

							if ( gObj[tObjNum].Life > (gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife))
								gObj[tObjNum].Life = gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife;

							bLifeChange = TRUE;
						}

						if ( gObj[tObjNum].Mana < (gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana))
						{
							gObj[tObjNum].Mana += ( ( gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana ) * (lpObj->m_btCsNpcRgLevel+1) ) / 100.0f;

							if ( gObj[tObjNum].Mana > (gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana))
								gObj[tObjNum].Mana = gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana;

							bManaChange = TRUE;
						}

						if ( gObj[tObjNum].BP < (gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP))
						{
							gObj[tObjNum].BP += ( ( gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP ) * (lpObj->m_btCsNpcRgLevel+1) ) / 100;

							if ( gObj[tObjNum].BP > (gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP))
								gObj[tObjNum].BP = gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP;

							bBpChange = TRUE;
						}

						if (bLifeChange )
							GCReFillSend(tObjNum, gObj[tObjNum].Life, 0xFF, 1, gObj[tObjNum].iShield);

						if (bManaChange ||bBpChange ) 
							GCManaSend(tObjNum, gObj[tObjNum].Mana, 0xFF, 0, gObj[tObjNum].BP);
					}
				}
			}
		}
	}
}


//#endif