// OptionDbSet.cpp: implementation of the COptionDbSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OptionDbSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptionDbSet::COptionDbSet()
{

}

COptionDbSet::~COptionDbSet()
{

}

BOOL COptionDbSet::Connect()
{
	if(m_DBQuery.Connect(3,gSettings.ConnectDNS,gSettings.ConnectLogin,gSettings.ConnectPassword) == FALSE)
	{
		//MsgBox("可记沥焊 DB 立加 角菩");
		return FALSE;
	}

	return TRUE;
}

BOOL COptionDbSet::Load(char* name, BYTE* KeyBuffer, BYTE& GO, BYTE& QK, BYTE& WK, BYTE& EK, BYTE& CW, BYTE& RK, int& QWERLevel)
{
	int nRet;
	CString qSql;

	qSql.Format("SELECT * FROM OptionData WHERE Name='%s'", name);

	if(m_DBQuery.Exec(qSql) == FALSE)
	{
		return FALSE;
	}

	if(m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	int GameOption = m_DBQuery.GetInt("GameOption");

	if(GameOption >= 0)
	{
		GO = GameOption;
	}
	
	int Qkey, Wkey, Ekey, Rkey, QWERL, ChatWindow;

	Qkey = m_DBQuery.GetInt("Qkey");

	if(Qkey >= 0)
	{
		QK = Qkey;
	}
	
	Wkey = m_DBQuery.GetInt("Wkey");

	if(Wkey >= 0)
	{
		WK = Wkey;
	}

	Ekey = m_DBQuery.GetInt("Ekey");

	if(Ekey >= 0)
	{
		EK = Ekey;
	}

	ChatWindow = m_DBQuery.GetInt("ChatWindow");

	if(ChatWindow >= 0)
	{
		CW = ChatWindow;
	}
	else
	{
		CW = 0xFF;
	}

	Rkey = m_DBQuery.GetInt("Rkey");

	if(Rkey >= 0)
	{
		RK = Rkey;
	}

	QWERL = m_DBQuery.GetInt("QWERLevel");

	if(QWERL >= 0)
	{
		QWERLevel = QWERL;
	}

	m_DBQuery.Clear();

	qSql.Format("SELECT SkillKey from OptionData where Name='%s'", name);
	nRet = m_DBQuery.ReadBlob(qSql, KeyBuffer);

	return nRet;
}

BOOL COptionDbSet::IsUser(char* name)
{
	CString qSQL;

	qSQL.Format("SELECT Name FROM OptionData WHERE Name='%s'", name);

	char buffer[256];

	if(m_DBQuery.ExecGetStr(qSQL, buffer) == FALSE)
	{
		return FALSE;
	}

	if( strcmp(name, buffer) != 0)
	{
		return FALSE;
	}

	return TRUE;
}

void COptionDbSet::Save(char* name, BYTE* KeyBuffer, BYTE GO, BYTE QK, BYTE WK, BYTE EK, BYTE CW, BYTE RK, int QWERLevel)
{
	CString qSql;

	if(IsUser(name) == FALSE)
	{
		qSql.Format("INSERT INTO OptionData (Name, GameOption, Qkey, Wkey, Ekey, ChatWindow, RKey, QWERLevel) VALUES ('%s', %d, %d, %d, %d, %d, %d, %d)",
			name, GO, QK, WK, EK, CW, RK, QWERLevel);

		if(m_DBQuery.Exec(qSql) == FALSE)
		{
			return;
		}
		return;
	}

	qSql.Format("UPDATE OptionData SET GameOption=%d, Qkey=%d, Wkey=%d, Ekey=%d, ChatWindow=%d, RKey=%d, QWERLevel=%d where Name='%s'",
		GO, QK, WK, EK, CW, RK, QWERLevel, name);

	if(m_DBQuery.Exec(qSql) == FALSE)
	{
		return;
	}

	qSql.Format("UPDATE OptionData SET SkillKey=? where Name='%s'", name);
	m_DBQuery.WriteBlob(qSql, KeyBuffer, 20);
}