//-> Decompiled by DarkSim | 13.11.2012 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#include "stdafx.h"
#include "PartyClass.h"
#include "LogProc.h"
#include "Protocol.h"
#include "..\common\winutil.h"
// -------------------------------------------------------------------------

PartyClass::PartyClass()
{
	// ----
}
// -------------------------------------------------------------------------

PartyClass::~PartyClass()
{
	// ----
}
// -------------------------------------------------------------------------

//00506590	-> 100%
bool PartyClass::IsParty(int party_number)
{
	if( party_number < 0 || party_number > OBJMAX )
	{
		return false;
	}
	// ----
	if( this->m_PartyS[party_number].Count == 0 )
	{
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------

//005065f0	-> 100%
bool PartyClass::Isleader(int party_number, int usernumber, int dbnumber)
{
	if( !this->IsParty(party_number) )
	{
		return false;
	}
	// ----
	if( this->m_PartyS[party_number].Number[0] == usernumber && this->m_PartyS[party_number].DbNumber[0] == dbnumber )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------

//00506660	-> 100%
int PartyClass::GetCount(int party_number)
{
	if( this->IsParty(party_number) == false )
	{
		return 0;
	}
	// ----
	return this->m_PartyS[party_number].Count;
}
// -------------------------------------------------------------------------

//005066b0	-> 100%
BOOL PartyClass::GetIndexUser(int party_number, int index, int & usernumber, int & dbnumber)
{
	if( !this->IsParty(party_number) )
	{
		return false;
	}
	// ----
	if( index < 0 || index > MAX_USER_IN_PARTY-1 )
	{
		return false;
	}
	// ----
	usernumber	= this->m_PartyS[party_number].Number[index];
	dbnumber	= this->m_PartyS[party_number].DbNumber[index];
	// ----
	return true;
}
// -------------------------------------------------------------------------

//00506740	-> 100%
int PartyClass::GetIndex(int party_number, int usernumber, int dbnumber)
{
	if( !this->IsParty(party_number) )
	{
		return false;
	}
	// ----
	for( int n = 0; n < MAX_USER_IN_PARTY; n++ )
	{
		if( this->m_PartyS[party_number].Number[n] == usernumber || this->m_PartyS[party_number].DbNumber[n] == dbnumber )
		{
			return n;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

//005067e0	-> 
int PartyClass::Create(int usernumber, int dbnumber, int level)
{
	int n			= this->m_PartyCount;
	int loopcount	= 0;
	// ----
	while( true )
	{
		if( this->m_PartyS[n].Count == 0 )
		{
			this->m_PartyS[n].Count			= 1;
			this->m_PartyS[n].Number[0]		= usernumber;
			this->m_PartyS[n].DbNumber[0]	= dbnumber;
			// ----
			for( int i = 1; i < MAX_USER_IN_PARTY; i++ )
			{
				this->m_PartyS[n].DbNumber[i]	= -1;
				this->m_PartyS[n].Number[i]		= -1;
			}
			// ----
			this->m_PartyCount++;
			this->m_PartyS[n].m_MaxLevel = 0;
			this->m_PartyS[n].m_MinLevel = 0;
			this->SetLevel(n, level);
			// ----
			if( this->m_PartyCount > OBJMAX-1 )
			{
				this->m_PartyCount = 0;
			}
			// ----
			this->m_PartyS[n].m_UserPKLevel[0] = gObj[usernumber].m_PK_Level;
			// ----
			if( this->m_PartyS[n].m_UserPKLevel[0] >= 5 )
			{
				this->m_PartyS[n].m_PKPartyLevel	= this->m_PartyS[n].m_UserPKLevel[0];
				this->m_PartyS[n].m_nPKUserCount			= 1;
			}
			else
			{
				this->m_PartyS[n].m_PKPartyLevel	= 3;
				this->m_PartyS[n].m_nPKUserCount			= 0;
			}
			// ----
			return n;
		}
		// ----
		n++;
		// ----
		if( n > OBJMAX-1 )
		{
			n = 0;
		}
		// ----
		loopcount++;
		// ----
		if( loopcount > OBJMAX-1 )
		{
			break;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

//00506a30	-> 100%
bool PartyClass::SetLevel(int party_number, int level)
{
	if( this->m_PartyS[party_number].m_MaxLevel < level )
	{
		this->m_PartyS[party_number].m_MaxLevel = level;
	}
	// ----
	if( this->m_PartyS[party_number].m_MinLevel == 0 )
	{
		this->m_PartyS[party_number].m_MinLevel = level;
	}
	// ----
	if( this->m_PartyS[party_number].m_MinLevel > level )
	{
		this->m_PartyS[party_number].m_MinLevel = level;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------

//00506ae0	-> 100%
bool PartyClass::GetLevel(int party_number, int & maxlevel, int & minlevel)
{
	if( !this->IsParty(party_number) )
	{
		return false;
	}
	// ----
	maxlevel = this->m_PartyS[party_number].m_MaxLevel;
	minlevel = this->m_PartyS[party_number].m_MinLevel;
	// ----
	return true;
}
// -------------------------------------------------------------------------

//00506b50	-> 100%
bool PartyClass::Destroy(int party_number)
{
	if( !this->IsParty(party_number) )
	{
		return false;
	}
	// ----
	this->PartyLog(party_number, 0, 0);	//-> New
	// ----
	this->m_PartyS[party_number].Count			= 0;
	this->m_PartyS[party_number].Number[0]		= -1;
	this->m_PartyS[party_number].DbNumber[0]	= -1;
	this->m_PartyCount--;
	// ----
	return false;
}
// -------------------------------------------------------------------------

//00506bf0	-> 100%
int PartyClass::Add(int party_number, int usernumber, int dbnumber, int level)
{
	if( !this->IsParty(party_number) )
	{
		return -1;
	}
	// ----
	if( this->m_PartyS[party_number].Count >= OBJMAX )
	{
		return -1;
	}
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		if( (this->m_PartyS[party_number].Number[i] >= 0) && (this->m_PartyS[party_number].Number[i] == usernumber) && (this->m_PartyS[party_number].DbNumber[i] == dbnumber) )
		{
			return -2;
		}
	}
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		if( this->m_PartyS[party_number].Number[i] == -1 )
		{
			this->m_PartyS[party_number].Number[i]		= usernumber;
			this->m_PartyS[party_number].DbNumber[i]	= dbnumber;
			this->m_PartyS[party_number].Count++;
			this->SetLevel(party_number, level);
			this->m_PartyS[party_number].m_UserPKLevel[i]	= gObj[usernumber].m_PK_Level;
			// ----
			if( this->m_PartyS[party_number].m_UserPKLevel[i] >= 5 )
			{
				if( this->m_PartyS[party_number].m_PKPartyLevel < this->m_PartyS[party_number].m_UserPKLevel[i] )
				{
					this->m_PartyS[party_number].m_PKPartyLevel = this->m_PartyS[party_number].m_UserPKLevel[i];
				}
				// ----
				this->m_PartyS[party_number].m_nPKUserCount += 1;
			}
			// ----
			return i;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

//00506e70	-> 100%
int PartyClass::Delete(int party_number, int usernumber, int dbnumber)
{
	if( !this->IsParty(party_number) )
	{
		return -1;
	}
	// ----
	this->PartyLog(party_number, usernumber, 1);	//-> New
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		if( (this->m_PartyS[party_number].Number[i] == usernumber) && (this->m_PartyS[party_number].DbNumber[i] == dbnumber) )
		{
			this->m_PartyS[party_number].Number[i]		= -1;
			this->m_PartyS[party_number].DbNumber[i]	= -1;
			this->m_PartyS[party_number].Count--;
			this->m_PartyS[party_number].m_UserPKLevel[i]	= 3;
			// ----
			return i;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------

//00506fb0	-> 99% (Need check PartyLog send)
void PartyClass::Delete(int party_number, int index)
{
	if( !this->IsParty(party_number) )
	{
		return;
	}
	// ----
	if( index < 0 || index > MAX_USER_IN_PARTY-1 )
	{
		return;
	}
	// ----
	this->PartyLog(party_number, this->m_PartyS[party_number].Number[index], 1);	//-> New
	// ----
	if( this->m_PartyS[party_number].Number[index] >= 0 )
	{
		this->m_PartyS[party_number].Number[index]		= -1;
		this->m_PartyS[party_number].DbNumber[index]	= -1;
		this->m_PartyS[party_number].Count--;
		this->m_PartyS[party_number].m_UserPKLevel[index]	= 3;
	}
}
// -------------------------------------------------------------------------

//005070c0	-> 100%
int PartyClass::GetPartyCount(int party_number)
{
	if( !this->IsParty(party_number) )
	{
		return -1;
	}
	// ----
	return this->m_PartyS[party_number].Count;
}
// -------------------------------------------------------------------------

//00507110	-> 100%
void PartyClass::Paint(int party_number)
{
	int usern = 0;
	// ----
	if( this->IsParty(party_number) )
	{
		return;
	}
	// ----
	wsprintf(this->m_szTempPaint, "PartyMember(%d) :", this->m_PartyS[party_number].Count);
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		if( this->m_PartyS[party_number].Number[i] && this->m_PartyS[party_number].DbNumber[i] )
		{
			usern = this->m_PartyS[party_number].Number[i];
			// ----
			if( gObjIsConnected(usern) )
			{
				strcat(this->m_szTempPaint, "[");
				strcat(this->m_szTempPaint, gObj[usern].AccountID);
				strcat(this->m_szTempPaint, "][");
				strcat(this->m_szTempPaint, gObj[usern].Name);
				strcat(this->m_szTempPaint, "], ");
			}
		}
	}
	// ----
	LogAddTD(this->m_szTempPaint);
}
// -------------------------------------------------------------------------

//005072f0	-> 
void PartyClass::PartyMemberLifeSend(int party_number)
{
	if( !this->IsParty(party_number) )
	{
		return;
	}
	// ----
	PMSG_DEFAULT_COUNT	pCount;
	PMSG_PARTYLIFEALL	pList;
	// ----
	int		usern;
	char	sendbuf[512];
	int		lOfs	= 0;
	int		tlife	= 0;
	// ----
	PHeadSetB((LPBYTE)&pCount, 0x44, sizeof(pCount));
	// ----
	pCount.Count = this->m_PartyS[party_number].Count;
	lOfs		+= sizeof(pCount);
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		if( (this->m_PartyS[party_number].Number[i] >= 0) && (this->m_PartyS[party_number].DbNumber[i] >= 0) )
		{
			usern = this->m_PartyS[party_number].Number[i];
			// ----
			if( gObjIsConnected(usern) )
			{
				pList.Number	= (i << 4) & 0xF0;
				tlife			= ( (BYTE)(gObj[usern].Life / ((gObj[usern].MaxLife + gObj[usern].AddLife )/10.0f)) ) & 0x0F;
				pList.Number	|= tlife;
				// ----
				memcpy(sendbuf+lOfs, (char*)&pList, 1);
				// ----
				lOfs++;
			}
			else
			{
				this->m_PartyS[party_number].Number[i]		= -1;
				this->m_PartyS[party_number].DbNumber[i]	= -1;
				this->m_PartyS[party_number].Count--;
			}
		}
	}
	// ----
	pCount.h.size = lOfs;
	memcpy(sendbuf, (char*)&pCount, sizeof(pCount));
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		usern = this->m_PartyS[party_number].Number[i];
		// ----
		if( usern >= 0 )
		{
			DataSend(usern, (LPBYTE)sendbuf, lOfs);
		}
	}
}
// -------------------------------------------------------------------------

//00507650	-> 100%
char PartyClass::GetPKPartyPenalty(int party_number)
{
#ifdef __CUSTOMS__
	return 0;
#else
	return this->m_PartyS[party_number].m_PKPartyLevel;
#endif
}
// -------------------------------------------------------------------------

//00507680	-> 100%
void PartyClass::UpdatePKUserInfo(int party_number, int usernumber, int dbnumber, char userPKLevel)
{
	if( !this->IsParty(party_number) )
	{
		return;
	}
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		if( (this->m_PartyS[party_number].Number[i] == usernumber ) && (this->m_PartyS[party_number].DbNumber[i] == dbnumber) )
		{
			this->m_PartyS[party_number].m_UserPKLevel[i] = userPKLevel;
			return;
		}
	}
}
// -------------------------------------------------------------------------

//00507730	-> 99% (Need check not used int usern)
void PartyClass::UpdatePKPartyPanalty(int party_number)
{
	int PKUserCount = 0;
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		if( this->m_PartyS[party_number].Number[i] >= 0 && this->m_PartyS[party_number].m_UserPKLevel[i] >= 5 )
		{
			if( this->m_PartyS[party_number].m_PKPartyLevel < this->m_PartyS[party_number].m_UserPKLevel[i] )
			{
				this->m_PartyS[party_number].m_PKPartyLevel = this->m_PartyS[party_number].m_UserPKLevel[i];
			}
			// ----
			PKUserCount++;
		}
	}
	// ----
	this->m_PartyS[party_number].m_nPKUserCount = PKUserCount;
	// ----
	if( PKUserCount == 0 )
	{
		this->m_PartyS[party_number].m_PKPartyLevel = 3;
	}
}
// -------------------------------------------------------------------------

//00507860	-> 100%
void PartyClass::ChangeLeader(int party_number)
{
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		if( this->m_PartyS[party_number].Number[i] >= 0 )
		{
			this->m_PartyS[party_number].Number[0]			= this->m_PartyS[party_number].Number[i];
			this->m_PartyS[party_number].Number[i]			= -1;
			this->m_PartyS[party_number].DbNumber[0]		= this->m_PartyS[party_number].DbNumber[i];
			this->m_PartyS[party_number].DbNumber[i]		= -1;
			this->m_PartyS[party_number].m_UserPKLevel[0]	= this->m_PartyS[party_number].m_UserPKLevel[i];
			this->m_PartyS[party_number].m_UserPKLevel[i]	= 3;
			// ----
			return;
		}
	}
}
// -------------------------------------------------------------------------

//005079b0	-> 100%
void PartyClass::SwitchLeader(int party_number)
{
	if ( IsParty(party_number) )
	{
		for (int i = 1; i < MAX_USER_IN_PARTY; ++i )
		{
			int usern = this->m_PartyS[party_number].Number[i];

			if ( usern >= 0 && gObj[usern].Connected == 3 )
			{
				int tempUserNumber = this->m_PartyS[party_number].Number[0];
				this->m_PartyS[party_number].Number[0] = this->m_PartyS[party_number].Number[i];
				this->m_PartyS[party_number].Number[i] = tempUserNumber;

				int tempUserDbNumber = this->m_PartyS[party_number].DbNumber[0];
				this->m_PartyS[party_number].DbNumber[0] = this->m_PartyS[party_number].DbNumber[i];
				this->m_PartyS[party_number].DbNumber[i] = tempUserDbNumber;

				int nTempUserPKLevel = this->m_PartyS[party_number].m_UserPKLevel[0];
				this->m_PartyS[party_number].m_UserPKLevel[0] = this->m_PartyS[party_number].m_UserPKLevel[i];
				this->m_PartyS[party_number].m_UserPKLevel[i] = nTempUserPKLevel;
				return;
			}
		}
	}
}
// -------------------------------------------------------------------------

//00507b90	-> 100%
int PartyClass::GetReallyConnectPartyMemberCount(int party_number)
{
	int nVaildUserCount = 0;
	// ----
	if( this->IsParty(party_number) )
	{
		for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
		{
			int usern = this->m_PartyS[party_number].Number[i];
			// ----
			if( usern >= 0 )
			{
				if( gObj[usern].Connected == 3 )
				{
					nVaildUserCount++;
				}
			}
		}
	}
	else
	{
		return -1;
	}
	// ----
	return nVaildUserCount;
}
// -------------------------------------------------------------------------

//00507c50	-> 100%
void PartyClass::PartyLog(int iPartyNumber, int iUserNumber, int iFlag)
{
	if( !OBJMAX_RANGE(iPartyNumber) || !OBJMAX_RANGE(iUserNumber) )
	{
		return;
	}
	// -----
	char strTemp[256];
	char strPartyInfo[1020];
	// ----
	if( !iFlag )
	{
		wsprintf(strPartyInfo, "[Party System] PartyIndex(%d): PartyBreak", iPartyNumber);
		LogAddTD(strPartyInfo);
		return;
	}
	// ----
	wsprintf(strPartyInfo, "[Party System] PartyMember(%d), PartyIndex(%d) ", GetPartyCount(iPartyNumber), iPartyNumber);
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		int usern = this->m_PartyS[iPartyNumber].Number[i];
		// ----
		if( usern >= 0 && gObj[usern].Connected == 3 || gObj[usern].Connected == 4 )
		{
			wsprintf(strTemp, "[%s][%s] ", gObj[usern].AccountID, gObj[usern].Name);
			strcat(strPartyInfo, strTemp);
		}
	}
	// ----
	if( iFlag == 1 )
	{
		wsprintf(strTemp, "PartyOut : [%s][%s] ", gObj[iUserNumber].AccountID, gObj[iUserNumber].Name);
		strcat(strPartyInfo, strTemp);
		LogAddTD(strPartyInfo);
		return;
	}
	// ----
	if( iFlag == 2 )
	{
		wsprintf(strTemp, "PartyRejoin : [%s][%s] ", gObj[iUserNumber].AccountID, gObj[iUserNumber].Name);
		strcat(strPartyInfo, strTemp);
		LogAddTD(strPartyInfo);
		return;
	}	
}
// -------------------------------------------------------------------------

//-> Custom
bool PartyClass::SetLeader(int OldLeader, int NewLeader)
{
	LPOBJ lpObj			= &gObj[OldLeader];
	LPOBJ lpTargetObj	= &gObj[NewLeader];		
	// ----
	int PartyNumber		= lpObj->PartyNumber;
	// ----
	if( !this->IsParty(PartyNumber) )
	{
		GCServerMsgStringSend("Your character is not in party", lpObj->m_Index, 1);
		return false;
	}
	// ----
	if( lpObj->PartyNumber != lpTargetObj->PartyNumber )
	{
		GCServerMsgStringSend("Target is not party member", lpObj->m_Index, 1);
		return false;
	}
	// ----
	if( !this->Isleader(PartyNumber, lpObj->m_Index, lpObj->DBNumber) )
	{
		GCServerMsgStringSend("Your character is not party leader", lpObj->m_Index, 1);
		return false;
	}
	// ----
	int TempIndex = this->GetIndex(PartyNumber, lpTargetObj->m_Index, lpTargetObj->DBNumber);
	// ----
	this->m_PartyS[PartyNumber].Number[0]					= this->m_PartyS[PartyNumber].Number[TempIndex];
	this->m_PartyS[PartyNumber].DbNumber[0]					= this->m_PartyS[PartyNumber].DbNumber[TempIndex];
	this->m_PartyS[PartyNumber].m_UserPKLevel[0]			= this->m_PartyS[PartyNumber].m_UserPKLevel[TempIndex];
	// ----
	this->m_PartyS[PartyNumber].Number[TempIndex]			= lpObj->m_Index;
	this->m_PartyS[PartyNumber].DbNumber[TempIndex]			= lpObj->DBNumber;
	this->m_PartyS[PartyNumber].m_UserPKLevel[TempIndex]	= lpObj->m_PK_Level;
	// ----
	char Text[100] = { 0 };
	sprintf(Text, "New party leader: %s", lpTargetObj->Name);
	// ----
	for( int i = 0; i < MAX_USER_IN_PARTY; i++ )
	{
		int UserIndex = this->m_PartyS[PartyNumber].Number[i];
		// ----
		if( UserIndex >= 0 && gObj[UserIndex].Connected == 3 )
		{
			GCServerMsgStringSend(Text, this->m_PartyS[PartyNumber].Number[i], 1);
			CGPartyList(this->m_PartyS[PartyNumber].Number[i]);
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------