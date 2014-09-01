// AccountCharDBSet.cpp: implementation of the CAccountCharDBSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AccountCharDBSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccountCharDBSet::CAccountCharDBSet()
{

}

CAccountCharDBSet::~CAccountCharDBSet()
{

}

BOOL CAccountCharDBSet::Connect()
{
	if(m_DBQuery.Connect(3,gSettings.ConnectDNS,gSettings.ConnectLogin,gSettings.ConnectPassword) == FALSE)
	{
		//MsgBox("계정정보 DB 접속 실패");
		return FALSE;
	}

	return TRUE;
}

BOOL CAccountCharDBSet::CreateAccountCharacter(char* id)
{
	CString qSQL;
	qSQL.Format("INSERT AccountCharacter (Id) VALUES ('%s')", id);
	if (m_DBQuery.Exec(qSQL) == TRUE)
	{
		return TRUE;
	}
	return FALSE;
}

int CAccountCharDBSet::DeleteAccountCharacter(char* id, char* gameid)
{
	CString qSQL;
	char gid[MAX_IDSTRING+1];

	qSQL.Format("SELECT * FROM AccountCharacter where Id='%s'", id);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return -1;
	}

	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return -1;
	}

	m_DBQuery.GetStr("GameID1", gid);

	if(strcmp(gid, gameid) == 0)
	{
		m_DBQuery.Clear();
		SaveAccountCharacter(id, 0, "");
		return 0;
	}

	m_DBQuery.GetStr("GameID2", gid);

	if(strcmp(gid, gameid) == 0)
	{
		m_DBQuery.Clear();
		SaveAccountCharacter(id, 1, "");
		return 1;
	}

	m_DBQuery.GetStr("GameID3", gid);

	if(strcmp(gid, gameid) == 0)
	{
		m_DBQuery.Clear();
		SaveAccountCharacter(id, 2, "");
		return 2;
	}

	m_DBQuery.GetStr("GameID4", gid);

	if(strcmp(gid, gameid) == 0)
	{
		m_DBQuery.Clear();
		SaveAccountCharacter(id, 3, "");
		return 3;
	}

	m_DBQuery.GetStr("GameID5", gid);

	if(strcmp(gid, gameid) == 0)
	{
		m_DBQuery.Clear();
		SaveAccountCharacter(id, 4, "");
		return 4;
	}

	m_DBQuery.Clear();
	
	return -1;
}

BOOL CAccountCharDBSet::SaveAccountCharacter(char* id, char* GameID1, char* GameID2, char* GameID3, char* GameID4, char* GameID5)
{
	CString qSQL;

	qSQL.Format("UPDATE AccountCharacter SET GameID1='%s',GameID2='%s', GameID3='%s', GameID4='%s', GameID5='%s'  WHERE Id='%s'", GameID1, GameID2, GameID3, GameID4, GameID5, id);

	if (m_DBQuery.Exec(qSQL) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CAccountCharDBSet::SaveAccountCharacter(char* id, int pos, char* GameID)
{
	CString qSQL;

	switch(pos)
	{
	case 0:
		qSQL.Format("UPDATE AccountCharacter SET GameID1='%s' WHERE Id='%s'", GameID, id);
		break;
	case 1:
		qSQL.Format("UPDATE AccountCharacter SET GameID2='%s' WHERE Id='%s'", GameID, id);
		break;
	case 2:
		qSQL.Format("UPDATE AccountCharacter SET GameID3='%s' WHERE Id='%s'", GameID, id);
		break;
	case 3:
		qSQL.Format("UPDATE AccountCharacter SET GameID4='%s' WHERE Id='%s'", GameID, id);
		break;
	case 4:
		qSQL.Format("UPDATE AccountCharacter SET GameID5='%s' WHERE Id='%s'", GameID, id);
		break;
	default:
		gWindow.PrintLog("error : %s %d pos: %d ", __FILE__, __LINE__, pos);
		break;
	}

	if (m_DBQuery.Exec(qSQL) == TRUE)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CAccountCharDBSet::GetAccountId(char* Id)
{
	CString qSQL;

	qSQL.Format("SELECT Id FROM AccountCharacter WHERE Id='%s'", Id);

	char buffer[256];

	if (m_DBQuery.ExecGetStr(qSQL, buffer) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAccountCharDBSet::GetAccountInfo(char* Id, LPAccountCharacterInfo lpACInfo)
{
	CString qSQL;

	qSQL.Format("SELECT * FROM AccountCharacter WHERE Id='%s'", Id);

	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;		
	}

	m_DBQuery.GetStr("Id", lpACInfo->AccountId);
	
	if( strlen(lpACInfo->AccountId) < 1 )
	{
		m_DBQuery.Clear();
		return FALSE;		
	}

	if( strcmp(lpACInfo->AccountId,Id) != 0)
	{
		gWindow.PrintLog("error-L1:'%s' '%s' 아이디가 같지 않다.", lpACInfo->AccountId, Id);
		m_DBQuery.Clear();
		return FALSE;		
	}
	
	int ctl_code = 0;
	
	lpACInfo->DBNumber = m_DBQuery.GetInt("Number");
	
	m_DBQuery.GetStr("GameID1", lpACInfo->GameID1);
	m_DBQuery.GetStr("GameID2", lpACInfo->GameID2);
	m_DBQuery.GetStr("GameID3", lpACInfo->GameID3);
	m_DBQuery.GetStr("GameID4", lpACInfo->GameID4);
	m_DBQuery.GetStr("GameID5", lpACInfo->GameID5);
	
	int movecount = m_DBQuery.GetInt("MoveCnt");
	
	if (movecount < 0)
	{
		movecount = 0;
	}

	lpACInfo->MoveCnt = movecount;
	lpACInfo->ExpandedWarehouse = m_DBQuery.GetInt("ExpandedWarehouse");

	//GetCharacterExtendedWarehouseCount(lpACInfo->AccountId, &lpACInfo->ExpandedWarehouse);
	//gWindow.PrintLog("CharName : %s 1[%s] 2[%s] 3[%s] 4[%s] 5[%s]", Id, lpACInfo->GameID1, lpACInfo->GameID2, lpACInfo->GameID3, lpACInfo->GameID4, lpACInfo->GameID5);
	
	m_DBQuery.Clear();
	return TRUE;
}

BYTE CAccountCharDBSet::GetAccountCharCtlCode(char* Id)
{
	CString qSQL;
	
	int ctl_code = 0;
	
	qSQL.Format("SELECT CtlCode FROM AccountCharacter WHERE Id=%s",Id);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		gWindow.PrintLog("%s CtlCode가 없다. #1", Id);
		return 0;
	}
	
	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		gWindow.PrintLog("%s CtlCode가 없다. #2", Id);
		return 0;		
	}
	
	ctl_code = m_DBQuery.GetInt("CtlCode");
	
	if (ctl_code < 0)
	{
		ctl_code = 0;
	}
	
	m_DBQuery.Clear();
	
	return ctl_code;
}

int CAccountCharDBSet::GetAccountCharBlank(char* Id)
{
	CString qSQL;

	char GameID[5][MAX_IDSTRING+1];
	char szAccountId[MAX_IDSTRING+1];

	qSQL.Format("SELECT * FROM AccountCharacter WHERE Id='%s'", Id);

	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return 0;
	}
	
	m_DBQuery.Fetch();
	m_DBQuery.GetStr("Id", szAccountId);
	
	if ( strlen(szAccountId) < 1)
	{
		gWindow.PrintLog("계정이 없다. 1");
		m_DBQuery.Clear();
		return -1;	
	}
	
	if ( strcmp(szAccountId,Id) != 0)
	{
		gWindow.PrintLog("계정이 없다. 2 %s %s", szAccountId, Id);
		m_DBQuery.Clear();
		return -1;	
	}
	
	m_DBQuery.GetStr("GameID1",GameID[0]);
	
	if ( strlen(GameID[0]) < 1)
	{
		m_DBQuery.Clear();
		return 0;			
	}
	
	m_DBQuery.GetStr("GameID2",GameID[1]);
	
	if ( strlen(GameID[1]) < 1)
	{
		m_DBQuery.Clear();
		return 1;			
	}
	
	m_DBQuery.GetStr("GameID3",GameID[2]);
	
	if ( strlen(GameID[2]) < 1)
	{
		m_DBQuery.Clear();
		return 2;			
	}
	
	m_DBQuery.GetStr("GameID4",GameID[3]);
	
	if ( strlen(GameID[3]) < 1)
	{
		m_DBQuery.Clear();
		return 3;			
	}
	
	m_DBQuery.GetStr("GameID5",GameID[4]);
	
	if ( strlen(GameID[4]) < 1)
	{
		m_DBQuery.Clear();
		return 4;			
	}

	m_DBQuery.Clear();
	
	return -1;
}

BOOL CAccountCharDBSet::CurAccountCharacterSet(char* id, char* GameID)
{
	CString qSQL;
	
	qSQL.Format("UPDATE AccountCharacter SET GameIDC='%s' WHERE Id='%s'", GameID, id);
	
	m_DBQuery.Exec(qSQL);
	
	return TRUE;
}

BOOL CAccountCharDBSet::GetSummonerCardInfo(char* Id)
{
	CString qSQL;
	
	qSQL.Format("SELECT Summoner FROM AccountCharacter WHERE Id = '%s'", Id);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}
	
	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;		
	}
	
	if (m_DBQuery.GetInt(1) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;	
	}
	
	m_DBQuery.Clear();
	return TRUE;
}

BOOL CAccountCharDBSet::SetSummonerCardInfo(char* Id)
{
	CString qSQL;
	
	qSQL.Format("UPDATE AccountCharacter SET Summoner = 1 WHERE Id = '%s'", Id);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}
	
	return TRUE;
}

BOOL CAccountCharDBSet::GetRageFighterCardInfo(char* id)
{
	CString qSQL;
	
	qSQL.Format("SELECT RageFighter FROM AccountCharacter WHERE Id = '%s'", id);

	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}
	
	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;		
	}

	if (m_DBQuery.GetInt(1) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;	
	}

	m_DBQuery.Clear();
	return TRUE;
}

BOOL CAccountCharDBSet::SetRageFighterCardInfo(char* id)
{
	CString qSQL;
	
	qSQL.Format("UPDATE AccountCharacter SET RageFighter = 1 WHERE Id = '%s'", id);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}
	
	return TRUE;
}

BOOL CAccountCharDBSet::SetCharacterExtendedInvenCountAdd(char* Name, BYTE btAddExtendedInvenCount)
{
	CString qSQL;
	BYTE btSlotCount = 0;

	qSQL.Format("SELECT ExpandedInventory FROM Character WHERE Name = '%s'", Name);

	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	btSlotCount = m_DBQuery.GetInt(1);
	
	m_DBQuery.Clear();
	
	if (btSlotCount + btAddExtendedInvenCount > 2)
	{
		gWindow.PrintLog("error : %s %d btSlotCount + btAddExtendedInvenCount: %d ", __FILE__, __LINE__, btSlotCount + btAddExtendedInvenCount);
		return FALSE;
	}

	qSQL.Empty();
	
	qSQL.Format("UPDATE Character SET ExpandedInventory = %d WHERE Name = '%s'", btSlotCount + btAddExtendedInvenCount, Name);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	return TRUE;
}

BOOL CAccountCharDBSet::SetCharacterExtendedInvenCountReplace(char* Name, BYTE btReplaceExtendedInvenCount)
{
	CString qSQL;
	BYTE btSlotCount = 0;

	qSQL.Format("SELECT ExpandedInventory FROM Character WHERE Name = '%s'", Name);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;		
	}

	m_DBQuery.Clear();
	
	if (btReplaceExtendedInvenCount > 2)
	{
		gWindow.PrintLog("error : %s %d btReplaceExtendedInvenCount: %d ", __FILE__, __LINE__, btReplaceExtendedInvenCount);
		return FALSE;	
	}
	
	qSQL.Empty();
	
	qSQL.Format("UPDATE Character SET ExpandedInventory = %d WHERE Name = '%s'", btReplaceExtendedInvenCount, Name);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	return TRUE;
}

BOOL CAccountCharDBSet::GetCharacterExtendedInvenCount(char* Name, BYTE* btAddExtendedInvenCount)
{
	CString qSQL;
	
	qSQL.Format("SELECT ExpandedInventory FROM Character WHERE Name = '%s'", Name);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	*btAddExtendedInvenCount = m_DBQuery.GetInt(1);
	
	if (*btAddExtendedInvenCount > 2)
	{
		*btAddExtendedInvenCount = 2;
	}

	m_DBQuery.Clear();
	
	return TRUE;
}

BOOL CAccountCharDBSet::SetCharacterExtendedWarehouseCountAdd(char* Name, BYTE btAddExtendedWarehouseCount)
{
	CString qSQL;
	BYTE btSlotCount = 0;
	
	qSQL.Format("SELECT ExpandedWarehouse FROM AccountCharacter WHERE Id = '%s'", Name);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;		
	}

	btSlotCount = m_DBQuery.GetInt(1);
	
	m_DBQuery.Clear();
	
	if (btSlotCount + btAddExtendedWarehouseCount > 1)
	{
		gWindow.PrintLog("error : %s %d btSlotCount + btAddExtendedWarehouseCount: %d ", __FILE__, __LINE__, btSlotCount + btAddExtendedWarehouseCount);
		return FALSE;
	}

	qSQL.Empty();
	
	qSQL.Format("UPDATE AccountCharacter SET ExpandedWarehouse = %d WHERE Id = '%s'", btSlotCount + btAddExtendedWarehouseCount, Name);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	return TRUE;
}

BOOL CAccountCharDBSet::SetCharacterExtendedWarehouseCountReplace(char* Name, BYTE btReplaceExtendedWarehouseCount)
{
	CString qSQL;
	BYTE btSlotCount = 0;

	qSQL.Format("SELECT ExpandedWarehouse FROM AccountCharacter WHERE Id = '%s'",Name);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;		
	}

	m_DBQuery.Clear();
	
	if (btReplaceExtendedWarehouseCount > 1)
	{
		gWindow.PrintLog("error : %s %d btReplaceExtendedWarehouseCount: %d ", __FILE__, __LINE__, btReplaceExtendedWarehouseCount);
		return FALSE;	
	}
	
	qSQL.Empty();

	qSQL.Format("UPDATE AccountCharacter SET ExpandedWarehouse = %d WHERE Id = '%s'", btReplaceExtendedWarehouseCount, Name);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}	

	return TRUE;
}

BOOL CAccountCharDBSet::GetCharacterExtendedWarehouseCount(char* Name, BYTE* btAddExtendedWarehouseCount)
{
	CString qSQL;

	qSQL.Format("SELECT ExpandedWarehouse FROM AccountCharacter WHERE Id = '%s'", Name);
	
	if (m_DBQuery.Exec(qSQL) == FALSE)
	{
		m_DBQuery.Clear();
		return FALSE;
	}

	if (m_DBQuery.Fetch() == SQL_NO_DATA)
	{
		m_DBQuery.Clear();
		return FALSE;		
	}

	*btAddExtendedWarehouseCount = m_DBQuery.GetInt(1);
	
	if (*btAddExtendedWarehouseCount > 1)
	{
		*btAddExtendedWarehouseCount = 1;
	}

	m_DBQuery.Clear();
	
	return TRUE;
}

bool CAccountCharDBSet::SaveGameShopStorageItem(char * AccountID, STORAGE Item)
{
	CString qSql;
	qSql.Format("INSERT INTO GameShopStorage (AccountID, AuthIndex, AuthID, ItemID, IBSID, IBSCategory, IBSIndex, PackageID) VALUES ('%s', %d, %d, %d, %d, %d, %d, %d)",
		AccountID, Item.AuthIndex, Item.AuthID, Item.ItemID, Item.IBSID, Item.IBSCategory, Item.IBSIndex, Item.PackageID);
	if( !m_DBQuery.Exec(qSql) )
	{
		return false;
	}
	return true;
}

bool CAccountCharDBSet::DelGameShopStorageItem(char * AccountID, STORAGE Item)
{
	CString qSql;
	qSql.Format("DELETE FROM GameShopStorage WHERE AccountID = '%s' AND AuthIndex = %d AND AuthID = %d AND ItemID = %d AND IBSID = %d AND IBSCategory = %d AND IBSIndex = %d", 
		AccountID, Item.AuthIndex, Item.AuthID, Item.ItemID, Item.IBSID, Item.IBSCategory, Item.IBSIndex);
	if( !m_DBQuery.Exec(qSql) )
	{
		m_DBQuery.Clear();
		return false;
	}
	m_DBQuery.Clear();
	return true;
}


bool CAccountCharDBSet::LoadGameShopStorage(char * AccountID, GAMESHOP_GD_STORAGE * Storage)
{
	int iReturnCode = 0;
	int iItemCount = 0;

	CString QuerySql;

	QuerySql.Format("EXEC GameShopStorageSelect '%s'", AccountID);

	if( m_DBQuery.Exec(QuerySql) == FALSE)
	{
		m_DBQuery.Clear();
		return false;
	}

	short i = m_DBQuery.Fetch();

	while(i != SQL_NO_DATA && i != SQL_NULL_DATA)
	{
		iReturnCode = m_DBQuery.GetInt(1);

		if(iReturnCode < 0)
		{
			break;
		}

		Storage->Storage[iItemCount].AuthIndex	= m_DBQuery.GetInt("AuthIndex");
		Storage->Storage[iItemCount].AuthID	= m_DBQuery.GetInt("AuthID");
		Storage->Storage[iItemCount].ItemID	= m_DBQuery.GetInt("ItemID");
		Storage->Storage[iItemCount].IBSID	= m_DBQuery.GetInt("IBSID");
		Storage->Storage[iItemCount].IBSCategory	= m_DBQuery.GetInt("IBSCategory");
		Storage->Storage[iItemCount].IBSIndex	= m_DBQuery.GetInt("IBSIndex");
		Storage->Storage[iItemCount].PackageID	= m_DBQuery.GetInt("PackageID");
		iItemCount++;

		if(iItemCount >= 30)
		{
			gWindow.PrintLog("[GameShop][Select] Item List Count Over %d, Id : %s", 30, AccountID);
			break;
		}

		i = m_DBQuery.Fetch();
	}

	m_DBQuery.Clear();
	Storage->ItemCount = iItemCount;

	return true;
}

#if( __MUANGEL__ == 1 )
void CAccountCharDBSet::SetWarehouseStatus(LPSTR AccountID, BYTE Status)
{
	CString qSQL;
	qSQL.Format("UPDATE AccountCharacter SET WarehouseStatus = %d WHERE Id = '%s'", Status, AccountID);
	// ---
	if( m_DBQuery.Exec(qSQL) == FALSE )
	{
		m_DBQuery.Clear();
		return;
	}
}
#endif