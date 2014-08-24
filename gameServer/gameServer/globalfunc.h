// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------
#pragma once

void gGetPartyMaxLevelAndReset(int nPartyNumber, int& nMaxLevel, int& nMaxReset)
{
	nMaxReset = -1;
	nMaxLevel = -1;
	for (int i = 0; i < 5; ++i )
	{
		if ( gParty.m_PartyS[nPartyNumber].Number[i] > 0 )
		{
			int nUserIndex = gParty.m_PartyS[nPartyNumber].Number[i];
//			int nLevel = gObj[nUserIndex].Level + gObj[nUserIndex].m_nMasterLevel;

			int nReset = gObj[nUserIndex].Reset;

			if ( nMaxReset < nReset )
			{
				nMaxReset = nReset;
				nMaxLevel = gObj[nUserIndex].Level + gObj[nUserIndex].m_nMasterLevel;
			}
		}
	}
}