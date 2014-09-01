#include "stdafx.h"
#include "GensDBSet.h"
#include "winutil.h"
// -----------------------------------------------------------------

GensDBSet::GensDBSet()
{
	// ----
}
// -----------------------------------------------------------------

GensDBSet::~GensDBSet()
{
	// ----
}
// -----------------------------------------------------------------
BOOL GensDBSet::Connect()
{
	return CDBConBase::Connect(gSettings.ConnectLogin, gSettings.ConnectPassword);
}
// -----------------------------------------------------------------

BOOL GensDBSet::GetMemberInfo(char * Character, PMSG_GENS_USERINFO_ANS * Result)
{
	CString Query;
	// ----
	Query.Format("SELECT * FROM GensMember WHERE Name = '%s'", Character);
	// ----
	if( !m_DBQuery.Exec(Query) || m_DBQuery.Fetch() == SQL_NO_DATA )
	{
		m_DBQuery.Clear();
		// ----
		gWindow.PrintLog("[%s] Failed to select values [Next: INSERT]", __FUNCTION__);
		// ----
		CString InsertQuery;
		// ----
		InsertQuery.Format("INSERT INTO GensMember (Name, Influence, Contribute, Class, Rank, RewardDate) VALUES ('%s', %d, %d, %d, %d, GetDate())", 
			Character, 0, 0, 0, 0);
		// ----
		if( !m_DBQuery.Exec(InsertQuery) )
		{
			gWindow.PrintLog("[%s] Failed to insert new values", __FUNCTION__);
			// ----
			return false;
		}
		// ----
		m_DBQuery.Clear();
	}
	else
	{
		Result->Influence		= m_DBQuery.GetInt("Influence");
		Result->ContributePoint	= m_DBQuery.GetInt("Contribute");
		Result->GensClass		= m_DBQuery.GetInt("Class");
		Result->GensRanking		= m_DBQuery.GetInt("Rank");
		// ----
		m_DBQuery.Clear();
	}
	// ----
	return true;
}
// -----------------------------------------------------------------

BOOL GensDBSet::SaveMemberInfo(char * Character, BYTE Influence, int Contribute, int Class)
{
	bool ResultCheck = true;
	// ----
	CString Query;
	// ----
	Query.Format("UPDATE GensMember SET Influence = %d, Contribute = %d, Class = %d WHERE Name = '%s'", 
		Influence, Contribute, Class, Character);
	// ----
	if( !m_DBQuery.Exec(Query) )
	{
		ResultCheck = false;
	}
	// ----
	m_DBQuery.Clear();
	// ----
	return ResultCheck;
}
// -----------------------------------------------------------------

BOOL GensDBSet::RegMemberInfo(char * Character, BYTE Influence)
{
	bool ResultCheck = true;
	// ----
	CString Query;
	// ----
	Query.Format("UPDATE GensMember SET Influence = %d, Contribute = 0, Class = 14 WHERE Name = '%s'", 
		Influence, Character);
	// ----
	if( !m_DBQuery.Exec(Query) )
	{
		ResultCheck = false;
	}
	// ----
	m_DBQuery.Clear();
	// ----
	return ResultCheck;
}
// -----------------------------------------------------------------

BOOL GensDBSet::DelMemberInfo(char * Character)
{
	bool ResultCheck = true;
	// ----
	CString Query;
	// ----
	Query.Format("UPDATE GensMember SET Influence = 0, Contribute = 0, Class = 0, Rank = 0 WHERE Name = '%s'", 
		Character);
	// ----
	if( !m_DBQuery.Exec(Query) )
	{
		ResultCheck = false;
	}
	// ----
	m_DBQuery.Clear();
	// ----
	return ResultCheck;
}
// -----------------------------------------------------------------

BOOL GensDBSet::CheckRewardDate(char * Character)
{
	CString Query;
	// ----
	Query.Format("EXEC GensRewardCheck '%s'", Character);
	// ----
	if( !m_DBQuery.Exec(Query) || m_DBQuery.Fetch() == SQL_NO_DATA )
	{
		m_DBQuery.Clear();
		return -1;
	}
	// ----
	m_DBQuery.Clear();
	return m_DBQuery.GetInt(1);
}
// -----------------------------------------------------------------

BOOL GensDBSet::SetRewardDate(char * Character)
{
	CString Query;
	// ----
	Query.Format("UPDATE GensMember SET RewardDate = GetDate() WHERE Name = '%s'", Character);
	// ----
	if( !m_DBQuery.Exec(Query) )
	{
		m_DBQuery.Clear();
		return false;
	}
	// ----
	m_DBQuery.Clear();
	return true;
}
// -----------------------------------------------------------------

BOOL GensDBSet::UpdateRanking()
{
	if( !m_DBQuery.ExecH("GensRankingUpdate 1") || !m_DBQuery.ExecH("GensRankingUpdate 2") )
	{
		return false;
	}
	// ----
	return true;
}
// -----------------------------------------------------------------

void GensDBSet::SQLExec(char * Text)
{
	m_DBQuery.Exec(Text);
	m_DBQuery.Clear();
}
// -----------------------------------------------------------------