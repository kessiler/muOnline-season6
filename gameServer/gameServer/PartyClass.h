#pragma once
// -------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------

#define MAX_PARTYUSER				5
#define MAX_USER_IN_PARTY			5
#define MAX_PARTY_DISTANCE_EFFECT	10
// -------------------------------------------------------------------------

struct PARTY_STRUCT
{
	int		Count;
	int		Number[MAX_USER_IN_PARTY];
	int		DbNumber[MAX_USER_IN_PARTY];
	short	m_MaxLevel;
	short	m_MinLevel;
	char	m_UserPKLevel[MAX_USER_IN_PARTY];
	char	m_PKPartyLevel;
	int		m_nPKUserCount;
};
// -------------------------------------------------------------------------

struct PMSG_DEFAULT_COUNT
{
	PBMSG_HEAD h;
	BYTE Count;
};
// -------------------------------------------------------------------------

struct PMSG_PARTYLIFEALL
{
	BYTE Number;
};
// -------------------------------------------------------------------------

class PartyClass
{
public:
	PartyClass();
	virtual ~PartyClass();
	// ----
	bool	IsParty(int party_number);
	int		Create(int usernumber, int dbnumber, int level);
	bool	Destroy(int party_number);
	int		Add(int party_number, int usernumber, int dbnumber,  int level);
	void	Delete(int party_number, int index);
	int		Delete(int party_number, int usernumber, int dbnumber);
	int		GetCount(int party_number);
	int		GetIndexUser(int party_number, int index, int& usernumber,  int& dbnumber);
	int		GetPartyCount(int party_number);
	int		GetIndex(int party_number, int usernumber, int dbnumber);
	bool	SetLevel(int party_number, int level);
	bool	GetLevel(int party_number, int& maxlevel, int& minlevel);
	bool	Isleader(int party_number, int usernumber, int dbnumber);
	void	Paint(int party_number);
	void	PartyMemberLifeSend(int party_number);
	char	GetPKPartyPenalty(int party_number);
	void	UpdatePKUserInfo(int party_number, int usernumber, int dbnumber, char pklevel);
	void	UpdatePKPartyPanalty(int party_number);
	void	ChangeLeader(int party_number);
	void	SwitchLeader(int party_number);
	int		GetReallyConnectPartyMemberCount(int party_number);
	void	PartyLog(int iPartyNumber, int iUserNumber, int iFlag);
	// ----
	bool	SetLeader(int OldLeader, int NewLeader);
	// ----
	PARTY_STRUCT m_PartyS[OBJMAX];
	char	m_szTempPaint[256];
	// ----
private:
	int m_PartyCount;
};
// -------------------------------------------------------------------------