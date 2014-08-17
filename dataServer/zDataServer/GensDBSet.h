#pragma once
// -----------------------------------------------------------------

#include "Sprodef.h"
#include "DBConBase.h"
// -----------------------------------------------------------------

struct PMSG_GENS_USERINFO_REQ
{
	PBMSG_HEAD2 h;
	// ----
	char		Name[10];
	BYTE		Result;
	BYTE		IndexH;
	BYTE		IndexL;
};
// -----------------------------------------------------------------

struct PMSG_GENS_USERINFO_ANS
{
	PBMSG_HEAD2 h;
	// ----
	BYTE		Influence;
	WORD		IndexH;
	WORD		IndexL;
	int			ContributePoint;
	int			GensClass;
	int			GensRanking;
};
// -----------------------------------------------------------------

struct PMSG_GENS_SAVEINFO
{
	PBMSG_HEAD2 h;
	// ----
	int			Index;
	char		Name[10];
	BYTE		Influence;
	int			ContributePoint;
	int			GensClass;
};
// -----------------------------------------------------------------

struct PMSG_GENS_REGUSER_REQ
{
	PBMSG_HEAD2 h;
	// ----
	BYTE		Result;
	char		Name[10];
	BYTE		Message;
	BYTE		Influence;
	WORD		NumberH;
	WORD		NumberL;
	BYTE		IndexH;
	BYTE		IndexL;
};
// -----------------------------------------------------------------

struct PMSG_GENS_REGUSER_ANS
{
	PBMSG_HEAD2 h;
	// ----
	BYTE		Result;
	BYTE		Influence;
	BYTE		IndexH;
	BYTE		IndexL;
};
// -----------------------------------------------------------------

struct PMSG_GENS_DELUSER_REQ
{
	PBMSG_HEAD2 h;
	// ----
	char		Name[10];
	BYTE		Result;
	WORD		NumberH;
	WORD		NumberL;
	BYTE		IndexH;
	BYTE		IndexL;
};
// -----------------------------------------------------------------

struct PMSG_GENS_DELUSER_ANS 
{
	PBMSG_HEAD2 h;
	// ----
	BYTE		Result;
	BYTE		IndexH;
	BYTE		IndexL;
};
// -----------------------------------------------------------------

struct PMSG_GENS_REWARD_REQ
{
	PBMSG_HEAD2 h;
	// ----
	char		Name[11];
	int			aIndex;
};
// -----------------------------------------------------------------

struct PMSG_GENS_REWARD_ANS 
{
	PBMSG_HEAD2 h;
	// ----
	BYTE		Result;
	int			aIndex;
};
// -----------------------------------------------------------------

struct PMSG_GENS_REWARD_SET
{
	PBMSG_HEAD2 h;
	// ----
	char		Name[11];
};
// -----------------------------------------------------------------

class GensDBSet: public CDBConBase
{
public:
	GensDBSet();
	virtual ~GensDBSet();
	// ----
	bool Connect();
	// ----
	bool GetMemberInfo(char * Character, PMSG_GENS_USERINFO_ANS * Result);
	bool SaveMemberInfo(char * Character, BYTE Influence, int Contribute, int Class);
	bool RegMemberInfo(char * Character, BYTE Influence);
	bool DelMemberInfo(char * Character);
	bool CheckRewardDate(char * Character);
	bool SetRewardDate(char * Character);
	bool UpdateRanking();
	// ----
	void SQLExec(char * Text);
};
// -----------------------------------------------------------------