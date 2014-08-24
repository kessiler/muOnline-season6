// CastleCrownSwitch.cpp: implementation of the CCastleCrownSwitch class.
// GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CastleCrownSwitch.h"

#include "CastleSiege.h"
#include "user.h"
#include "LogProc.h"


//#if (_GSCS==1)
CCastleCrownSwitch g_CsNPC_CastleCrownSwitch;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCastleCrownSwitch::CCastleCrownSwitch()
{
	return;
}

CCastleCrownSwitch::~CCastleCrownSwitch()
{
	return;
}

void CCastleCrownSwitch::CastleCrownSwitchAct(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return;

	if ( gObj[iIndex].Type != OBJ_NPC ||
		((gObj[iIndex].Class < 217 )?FALSE:(gObj[iIndex].Class > 219-1 )?FALSE:TRUE)==FALSE )
		return;

	if ( !gObjIsConnected(g_CastleSiege.GetCrownSwitchUserIndex(gObj[iIndex].Class)) )
	{
		g_CastleSiege.ResetCrownSwitchUserIndex(gObj[iIndex].Class);

		if ( g_CastleSiege.GetRegCrownAvailable() == 1 )
		{
			g_CastleSiege.SetRegCrownAvailable(FALSE);
			g_CastleSiege.NotifyCrownState(1);
		}

		return;
	}
	
	LPOBJ lpObj= &gObj[iIndex];
	LPOBJ lpUserObj = &gObj[g_CastleSiege.GetCrownSwitchUserIndex(gObj[iIndex].Class)];
	g_CastleSiege.NotifyCrownSwitchInfo(iIndex);

	if ( lpUserObj->MapNumber == MAP_INDEX_CASTLESIEGE &&
		 lpUserObj->m_btCsJoinSide >= 2 )
	{
		if ( abs(lpObj->Y - lpUserObj->Y) <= 3 &&
			 abs(lpObj->X - lpUserObj->X) <= 3 )
		{
			int iCrownIndex1 = g_CastleSiege.GetCrownSwitchUserIndex(217);
			int iCrownIndex2 = g_CastleSiege.GetCrownSwitchUserIndex(218);

			if (gObjIsConnected(iCrownIndex1) && gObjIsConnected(iCrownIndex2) )
			{
				if ( gObj[iCrownIndex1].m_btCsJoinSide == gObj[iCrownIndex2].m_btCsJoinSide )
				{
					if ( g_CastleSiege.GetRegCrownAvailable() == 0 )
					{
						g_CastleSiege.SetRegCrownAvailable(TRUE);
						g_CastleSiege.NotifyCrownState(0);
					}
				}
			}
		}
		else
		{
			GCAnsCsAccessSwitchState(lpUserObj->m_Index, lpObj->m_Index, -1, 0);
			g_CastleSiege.ResetCrownSwitchUserIndex(gObj[iIndex].Class);

			if ( g_CastleSiege.GetRegCrownAvailable() == 1 )
			{
				g_CastleSiege.SetRegCrownAvailable(FALSE);
				g_CastleSiege.NotifyCrownState(1);
			}

			LogAddTD("[CastleSiege] [%s][%s] Push Castle Crown Switch Canceled (GUILD:%s) - CS X:%d/Y:%d",
				lpUserObj->AccountID, lpUserObj->Name, lpUserObj->GuildName, lpObj->X, lpObj->Y);
		}
	}
	else
	{
		GCAnsCsAccessSwitchState(lpUserObj->m_Index, lpObj->m_Index, -1, 0);

		g_CastleSiege.ResetCrownSwitchUserIndex(gObj[iIndex].Class);

		if ( g_CastleSiege.GetRegCrownAvailable() == 1 )
		{
			g_CastleSiege.SetRegCrownAvailable(FALSE);
			g_CastleSiege.NotifyCrownState(1);
		}
	}
}

//#endif