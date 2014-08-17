// CastleDBSet.cpp: implementation of the CCastleDBSet class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CastleDBSet.h"
#include "winutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCastleDBSet::CCastleDBSet()
{

}

CCastleDBSet::~CCastleDBSet()
{

}

BOOL CCastleDBSet::Connect()
{
	return CDBConBase::Connect(gSettings.ConnectLogin, gSettings.ConnectPassword);
}

BOOL CCastleDBSet::DSDB_QueryCastleTotalInfo(int iMapSvrGroup, int iCastleEventCycle, CASTLE_DATA* lpCastleData)
{
	if(lpCastleData == NULL)
	{
		return TRUE;
	}

	CString qSql;

	qSql.Format("EXEC WZ_CS_GetCastleTotalInfo %d, %d", iMapSvrGroup, iCastleEventCycle);

	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();

	if(sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	lpCastleData->wStartYear = m_DBQuery.GetInt("SYEAR");
	lpCastleData->btStartMonth = m_DBQuery.GetInt("SMONTH");
	lpCastleData->btStartDay = m_DBQuery.GetInt("SDAY");
	lpCastleData->wEndYear = m_DBQuery.GetInt("EYEAR");
	lpCastleData->btEndMonth = m_DBQuery.GetInt("EMONTH");
	lpCastleData->btEndDay = m_DBQuery.GetInt("EDAY");
	lpCastleData->btIsSiegeGuildList = m_DBQuery.GetInt("SIEGE_GUILDLIST_SETTED");
	lpCastleData->btIsSiegeEnded = m_DBQuery.GetInt("SIEGE_ENDED");
	lpCastleData->btIsCastleOccupied = m_DBQuery.GetInt("CASTLE_OCCUPY");
	lpCastleData->i64CastleMoney = m_DBQuery.GetInt64("MONEY");
	lpCastleData->iTaxRateChaos = m_DBQuery.GetInt("TAX_RATE_CHAOS");
	lpCastleData->iTaxRateStore = m_DBQuery.GetInt("TAX_RATE_STORE");
	lpCastleData->iTaxHuntZone = m_DBQuery.GetInt("TAX_HUNT_ZONE");
	lpCastleData->iFirstCreate = m_DBQuery.GetInt("FIRST_CREATE");
	m_DBQuery.GetStr("OWNER_GUILD", lpCastleData->szCastleOwnGuild);
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryOwnerGuildMaster(int iMapSvrGroup, CSP_ANS_OWNERGUILDMASTER* lpOwnerGuildMaster)
{
	if (lpOwnerGuildMaster == NULL)
	{
		return TRUE;
	}

	CString qSql;
	
	qSql.Format("EXEC WZ_CS_GetOwnerGuildMaster %d", iMapSvrGroup);

	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return NULL;		
	}
	
	char szOwnerGuild[MAX_GUILDNAMESTRING+MAX_GUILDNAMESTRING] = {'\0'};
	char szOwnerGuildMaster[MAX_GUILDNAMESTRING+MAX_GUILDNAMESTRING] = {'\0'};
	
	lpOwnerGuildMaster->iResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.GetStr("OwnerGuild", szOwnerGuild);
	m_DBQuery.GetStr("OwnerGuildMaster", szOwnerGuildMaster);
	
	memcpy(lpOwnerGuildMaster->szCastleOwnGuild, szOwnerGuild, MAX_GUILDNAMESTRING);
	memcpy(lpOwnerGuildMaster->szCastleOwnGuildMaster, szOwnerGuildMaster, MAX_IDSTRING);
	
	m_DBQuery.Clear();
	return FALSE;	
}

int CCastleDBSet::DSDB_QueryCastleNpcBuy(int iMapSvrGroup, CSP_REQ_NPCBUY* lpNpcBuy, int* lpiResult)
{
	if (lpiResult == NULL || lpNpcBuy == NULL)
	{
		return TRUE;
	}
	
	CString qSql;
	
	qSql.Format("EXEC WZ_CS_ReqNpcBuy %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
		iMapSvrGroup, lpNpcBuy->iNpcNumber, lpNpcBuy->iNpcIndex, lpNpcBuy->iNpcDfLevel,
		lpNpcBuy->iNpcRgLevel, lpNpcBuy->iNpcMaxHp, lpNpcBuy->iNpcHp, lpNpcBuy->btNpcX, lpNpcBuy->btNpcY, lpNpcBuy->btNpcDIR);

	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}
	
	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCastleNpcRepair(int iMapSvrGroup, CSP_REQ_NPCREPAIR* lpNpcRepair, CSP_ANS_NPCREPAIR* lpNpcRepairResult, int* lpiResult)
{
	if (lpNpcRepair == NULL || lpNpcRepairResult == NULL)
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_ReqNpcRepair %d, %d, %d", 
		iMapSvrGroup, lpNpcRepair->iNpcNumber, lpNpcRepair->iNpcIndex);

	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}

	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	lpNpcRepairResult->iNpcHp = m_DBQuery.GetInt("NPC_HP");
	lpNpcRepairResult->iNpcMaxHp = m_DBQuery.GetInt("NPC_MAXHP");
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCastleNpcUpgrade(int iMapSvrGroup, CSP_REQ_NPCUPGRADE* lpNpcUpgrade)
{
	if (lpNpcUpgrade == NULL)
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_ReqNpcUpgrade %d, %d, %d, %d, %d", iMapSvrGroup, lpNpcUpgrade->iNpcNumber, lpNpcUpgrade->iNpcIndex, lpNpcUpgrade->iNpcUpType, lpNpcUpgrade->iNpcUpValue);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA)
	{
		m_DBQuery.Clear();
		return TRUE;		
	}

	int iResult = m_DBQuery.GetInt("QueryResult");

	if (iResult == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;		
	}

	m_DBQuery.Clear();
	
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryTaxInfo(int iMapSvrGroup, CSP_ANS_TAXINFO* lpTaxInfo)
{
	if (lpTaxInfo == NULL)
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_GetCastleTaxInfo %d", iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	lpTaxInfo->i64CastleMoney = m_DBQuery.GetInt64("MONEY");
	lpTaxInfo->iTaxRateChaos = m_DBQuery.GetInt("TAX_RATE_CHAOS");
	lpTaxInfo->iTaxRateStore = m_DBQuery.GetInt("TAX_RATE_STORE");
	lpTaxInfo->iTaxHuntZone = m_DBQuery.GetInt("TAX_HUNT_ZONE");
	m_DBQuery.Clear();
	
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryTaxRateChange(int iMapSvrGroup, int iTaxType, int iTaxRate, CSP_ANS_TAXRATECHANGE* lpTaxRateChange, int* lpiResult)
{
	if (lpiResult == NULL || lpTaxRateChange == NULL)
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_ModifyTaxRate %d, %d, %d", iMapSvrGroup, iTaxType, iTaxRate);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	lpTaxRateChange->iTaxKind = m_DBQuery.GetInt("TaxKind");
	lpTaxRateChange->iTaxRate = m_DBQuery.GetInt("TaxRate");

	m_DBQuery.Clear();
	
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCastleMoneyChange(int iMapSvrGroup, int iMoneyChange, __int64* i64ResultMoney, int* lpiResult)
{
	if (lpiResult == NULL || i64ResultMoney == NULL)
	{
		return TRUE;
	}
	
	CString qSql;
	
	qSql.Format("EXEC WZ_CS_ModifyMoney %d, %d", iMapSvrGroup, iMoneyChange);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}
	
	*lpiResult = m_DBQuery.GetInt("QueryResult");
	*i64ResultMoney = m_DBQuery.GetInt64("MONEY");
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QuerySiegeDateChange(int iMapSvrGroup, CSP_REQ_SDEDCHANGE* lpSdEdChange, int* lpiResult)
{
	if (lpSdEdChange == NULL)
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_ModifyCastleSchedule %d, '%d-%d-%d 00:00:00', '%d-%d-%d 00:00:00'",
		iMapSvrGroup, lpSdEdChange->wStartYear, lpSdEdChange->btStartMonth, lpSdEdChange->btStartDay,
		lpSdEdChange->wEndYear, lpSdEdChange->btEndMonth, lpSdEdChange->btEndDay);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA)
	{
		m_DBQuery.Clear();
		return TRUE;		
	}

	*lpiResult = m_DBQuery.GetInt("QueryResult");
	m_DBQuery.Clear();
	
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryGuildMarkRegInfo(int iMapSvrGroup, char* lpszGuildName, CSP_ANS_GUILDREGINFO* lpGuildRegInfo, int* lpiResult)
{
	if (lpszGuildName  == NULL || lpGuildRegInfo == NULL)
	{
		return TRUE;
	}

	if (SQLSyntexCheck(lpszGuildName) == FALSE)
	{
		return TRUE;
	}

	CString qSql;
	
	qSql.Format("EXEC WZ_CS_GetGuildMarkRegInfo %d, '%s'", iMapSvrGroup, lpszGuildName);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	memcpy(lpGuildRegInfo->szGuildName, lpszGuildName, MAX_GUILDNAMESTRING);
	
	if (sqlRet == SQL_NO_DATA )
	{
		lpGuildRegInfo->iRegMarkCount = 0;
		*lpiResult = 2;
	}
	else
	{
		*lpiResult = 1;
		lpGuildRegInfo->iRegMarkCount = m_DBQuery.GetInt("REG_MARKS");
		lpGuildRegInfo->btRegRank = m_DBQuery.GetInt("SEQ_NUM");
		lpGuildRegInfo->bIsGiveUp = (m_DBQuery.GetInt("IS_GIVEUP") > 0)?true:false;	
	}

	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QuerySiegeEndedChange(int iMapSvrGroup, int bIsCastleSiegeEnded, int* lpiResult)
{
	if (lpiResult == NULL)
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_ModifySiegeEnd %d, %d", iMapSvrGroup, bIsCastleSiegeEnded);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}

	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCastleOwnerChange(int iMapSvrGroup, CSP_REQ_CASTLEOWNERCHANGE* lpCastleOwnerInfo, CSP_ANS_CASTLEOWNERCHANGE* lpCastleOwnerInfoResult, int* lpiResult)
{
	if (lpCastleOwnerInfo == NULL || lpCastleOwnerInfoResult == NULL || lpiResult == NULL)
	{
		return TRUE;
	}

	char szGuildName[MAX_GUILDNAMESTRING+1] = {'\0'};
	memcpy(szGuildName, lpCastleOwnerInfo->szOwnerGuildName, MAX_GUILDNAMESTRING);
	
	if (SQLSyntexCheck(szGuildName) == FALSE)
	{
		return TRUE;
	}

	lpCastleOwnerInfoResult->bIsCastleOccupied = lpCastleOwnerInfo->bIsCastleOccupied;
	memcpy(lpCastleOwnerInfoResult->szOwnerGuildName, lpCastleOwnerInfo->szOwnerGuildName, MAX_GUILDNAMESTRING);
	
	CString qSql;
	qSql.Format("EXEC WZ_CS_ModifyCastleOwnerInfo %d, %d, '%s'", iMapSvrGroup, lpCastleOwnerInfo->bIsCastleOccupied,szGuildName);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}

	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryRegAttackGuild(int iMapSvrGroup, CSP_REQ_REGATTACKGUILD* lpRegAttackGuild, CSP_ANS_REGATTACKGUILD* lpRegAttackGuildResult, int* lpiResult)
{
	if (lpRegAttackGuild == NULL || lpRegAttackGuildResult == NULL || lpiResult == NULL)
	{
		return TRUE;
	}

	char szGuildName[MAX_GUILDNAMESTRING+1] = {'\0'};
	memcpy(szGuildName, lpRegAttackGuild->szEnemyGuildName, MAX_GUILDNAMESTRING);
	
	if (SQLSyntexCheck(szGuildName) == FALSE)
	{
		return TRUE;
	}

	memcpy(lpRegAttackGuild->szEnemyGuildName, lpRegAttackGuildResult->szEnemyGuildName, MAX_GUILDNAMESTRING);
	
	CString qSql;
	qSql.Format("EXEC WZ_CS_ReqRegAttackGuild %d, '%s'", iMapSvrGroup, szGuildName);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}

	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryRestartCastleState(int iMapSvrGroup, CSP_REQ_CASTLESIEGEEND* lpCastleSiegeEnd, int* lpiResult)
{
	if (lpCastleSiegeEnd == NULL)
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_ResetCastleSiege %d", iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryGuildMarkRegMark(int iMapSvrGroup, char* lpszGuildName, CSP_ANS_GUILDREGMARK* lpGuildRegMark, int* lpiResult)
{
	if (lpszGuildName == NULL || lpGuildRegMark == NULL)
	{
		return TRUE;
	}

	if ( !SQLSyntexCheck(lpszGuildName) )
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_ReqRegGuildMark %d, '%s'",iMapSvrGroup,lpszGuildName);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}
	
	memcpy(lpGuildRegMark->szGuildName, lpszGuildName, MAX_GUILDNAMESTRING);
	
	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	lpGuildRegMark->iRegMarkCount = m_DBQuery.GetInt("REG_MARKS");
	
	m_DBQuery.Clear();
	
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryGuildMarkReset(int iMapSvrGroup, char* lpszGuildName, CSP_ANS_GUILDRESETMARK* lpGuildResetMark)
{
	if (lpszGuildName == NULL || lpGuildResetMark == NULL)
	{
		return TRUE;
	}

	if (SQLSyntexCheck(lpszGuildName) == FALSE)
	{
		return TRUE;
	}
	
	CString qSql;
	qSql.Format("EXEC WZ_CS_ModifyGuildMarkReset %d, '%s'",iMapSvrGroup,lpszGuildName);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	short sqlRet = m_DBQuery.Fetch();

	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}
	
	memcpy(lpGuildResetMark->szGuildName, lpszGuildName, MAX_GUILDNAMESTRING);
	
	lpGuildResetMark->iResult = m_DBQuery.GetInt("QueryResult");
	lpGuildResetMark->iRegMarkCount = m_DBQuery.GetInt("DEL_MARKS");
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryGuildSetGiveUp(int iMapSvrGroup, char* lpszGuildName, int bIsGiveUp, CSP_ANS_GUILDSETGIVEUP* lpGuildSetGiveUp)
{
	if (lpszGuildName == NULL || lpGuildSetGiveUp == NULL)
	{
		return TRUE;
	}
	
	if (SQLSyntexCheck(lpszGuildName) == FALSE)
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_ModifyGuildGiveUp %d, '%s', %d",iMapSvrGroup,lpszGuildName,bIsGiveUp);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	short sqlRet = m_DBQuery.Fetch();

	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}
	
	memcpy(lpGuildSetGiveUp->szGuildName, lpszGuildName, MAX_GUILDNAMESTRING);
	
	lpGuildSetGiveUp->bIsGiveUp = bIsGiveUp;
	lpGuildSetGiveUp->iResult = m_DBQuery.GetInt("QueryResult");
	lpGuildSetGiveUp->iRegMarkCount = m_DBQuery.GetInt("DEL_MARKS");
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCastleNpcRemove(int iMapSvrGroup, CSP_REQ_NPCREMOVE* lpNpcRemove, int* lpiResult)
{
	if (lpNpcRemove == NULL)
	{
		return TRUE;
	}
	
	CString qSql;
	qSql.Format("EXEC WZ_CS_ReqNpcRemove %d, %d, %d",iMapSvrGroup,lpNpcRemove->iNpcNumber,lpNpcRemove->iNpcIndex);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();

	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}

	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	return FALSE;
}

int CCastleDBSet::DSDB_QueryResetCastleTaxInfo(int iMapSvrGroup, int* lpiResult)
{
	CString qSql;
	qSql.Format("EXEC WZ_CS_ResetCastleTaxInfo %d",iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}
	
	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	return FALSE;
}

int CCastleDBSet::DSDB_QueryResetSiegeGuildInfo(int iMapSvrGroup, int* lpiResult)
{
	CString qSql;
	qSql.Format("EXEC WZ_CS_ResetSiegeGuildInfo %d",iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	return FALSE;
}

int CCastleDBSet::DSDB_QueryResetRegSiegeInfo(int iMapSvrGroup, int* lpiResult)
{
	CString qSql;
	qSql.Format("EXEC WZ_CS_ResetRegSiegeInfo %d",iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	short sqlRet = m_DBQuery.Fetch();
	
	if (sqlRet == SQL_NO_DATA || sqlRet == SQL_NULL_DATA )
	{
		m_DBQuery.Clear();
		return TRUE;		
	}

	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	return FALSE;
}

int CCastleDBSet::DSDB_QueryCastleNpcInfo(int iMapSvrGroup, CSP_NPCDATA* lpNpcData, int* lpiCount)
{
	if (lpNpcData == NULL || lpiCount == NULL)
	{
		return TRUE;
	}
	
	int iMAX_COUNT = *lpiCount;
	*lpiCount=0;

	CString qSql;
	qSql.Format("EXEC WZ_CS_GetCastleNpcInfo %d",iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	while(sqlRet != SQL_NO_DATA && sqlRet != SQL_NULL_DATA)
	{
		if (*lpiCount > iMAX_COUNT)
		{
			break;
		}

		lpNpcData[*lpiCount].iNpcNumber = m_DBQuery.GetInt("NPC_NUMBER");
		lpNpcData[*lpiCount].iNpcIndex = m_DBQuery.GetInt("NPC_INDEX");
		lpNpcData[*lpiCount].iNpcDfLevel = m_DBQuery.GetInt("NPC_DF_LEVEL");
		lpNpcData[*lpiCount].iNpcRgLevel = m_DBQuery.GetInt("NPC_RG_LEVEL");
		lpNpcData[*lpiCount].iNpcMaxHp = m_DBQuery.GetInt("NPC_MAXHP");
		lpNpcData[*lpiCount].iNpcHp = m_DBQuery.GetInt("NPC_HP");
		lpNpcData[*lpiCount].btNpcX = m_DBQuery.GetInt("NPC_X");
		lpNpcData[*lpiCount].btNpcY = m_DBQuery.GetInt("NPC_Y");
		lpNpcData[*lpiCount].btNpcDIR = m_DBQuery.GetInt("NPC_DIR");
		
		*lpiCount = *lpiCount + 1;
		
		sqlRet = m_DBQuery.Fetch();
	}
	
	if (sqlRet == SQL_NULL_DATA)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryAllGuildMarkRegInfo(int iMapSvrGroup, CSP_GUILDREGINFO* lpGuildRegInfo, int* lpiCount)
{
	if (lpGuildRegInfo == NULL || lpiCount == NULL)
	{
		return TRUE;
	}

	int iMAX_COUNT = *lpiCount;
	
	if ( iMAX_COUNT > 100 )
	{
		iMAX_COUNT = 100;
	}
	
	*lpiCount=0;
	
	CString qSql;
	qSql.Format("EXEC WZ_CS_GetAllGuildMarkRegInfo %d",iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();

	while(sqlRet != SQL_NO_DATA && sqlRet != SQL_NULL_DATA)
	{
		if (*lpiCount > iMAX_COUNT)
		{
			break;
		}

		m_DBQuery.GetStr("REG_SIEGE_GUILD", lpGuildRegInfo[*lpiCount].szGuildName);
		lpGuildRegInfo[*lpiCount].iRegMarkCount = m_DBQuery.GetInt("REG_MARKS");
		lpGuildRegInfo[*lpiCount].bIsGiveUp = (m_DBQuery.GetInt("IS_GIVEUP") > 0)?true:false;
		lpGuildRegInfo[*lpiCount].btRegRank = m_DBQuery.GetInt("SEQ_NUM");
		
		*lpiCount = *lpiCount + 1;
		
		sqlRet = m_DBQuery.Fetch();
	}
	
	if (sqlRet == SQL_NULL_DATA)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryFirstCreateNPC(int iMapSvrGroup, CSP_REQ_NPCSAVEDATA* lpNpcSaveData)
{
	if (lpNpcSaveData == NULL)
	{
		return TRUE;
	}
	
	CSP_NPCSAVEDATA* lpMsgBody = (CSP_NPCSAVEDATA*)&lpNpcSaveData[1];
	
	CString qSql;
	qSql.Format("DELETE MuCastle_NPC WHERE MAP_SVR_GROUP = %d",iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	m_DBQuery.Clear();

	for (int iNPC_COUNT = 0; iNPC_COUNT < lpNpcSaveData->iCount; iNPC_COUNT++)
	{
		qSql.Format("EXEC WZ_CS_ReqNpcBuy %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			iMapSvrGroup,lpMsgBody[iNPC_COUNT].iNpcNumber,lpMsgBody[iNPC_COUNT].iNpcIndex,
			lpMsgBody[iNPC_COUNT].iNpcDfLevel,lpMsgBody[iNPC_COUNT].iNpcRgLevel,
			lpMsgBody[iNPC_COUNT].iNpcMaxHp,lpMsgBody[iNPC_COUNT].iNpcHp,
			lpMsgBody[iNPC_COUNT].btNpcX,lpMsgBody[iNPC_COUNT].btNpcY,
			lpMsgBody[iNPC_COUNT].btNpcDIR);
		
		if (m_DBQuery.Exec(qSql) == FALSE)
		{
			m_DBQuery.Clear();
			return TRUE;
		}
		
		m_DBQuery.Clear();
	}

	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCalcRegGuildList(int iMapSvrGroup, CSP_CALCREGGUILDLIST* lpCalcRegGuildList, int* lpiCount)
{
	if (lpCalcRegGuildList == NULL || lpiCount == NULL)
	{
		return TRUE;
	}
	
	int iMAX_COUNT = *lpiCount;
	
	if ( iMAX_COUNT > 100 )
	{
		iMAX_COUNT = 100;
	}
	
	*lpiCount=0;
	
	CString qSql;
	qSql.Format("EXEC WZ_CS_GetCalcRegGuildList %d",iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();
	
	while(sqlRet != SQL_NO_DATA && sqlRet != SQL_NULL_DATA)
	{
		if (*lpiCount > iMAX_COUNT)
		{
			break;
		}

		char szGuildName[MAX_GUILDNAMESTRING+MAX_GUILDNAMESTRING] = {'\0'};
		
		m_DBQuery.GetStr("REG_SIEGE_GUILD", szGuildName);
		
		memcpy(&lpCalcRegGuildList[*lpiCount], szGuildName, MAX_GUILDNAMESTRING);
		
		lpCalcRegGuildList[*lpiCount].iRegMarkCount = m_DBQuery.GetInt("REG_MARKS");
		lpCalcRegGuildList[*lpiCount].iGuildMemberCount = m_DBQuery.GetInt("GUILD_MEMBER");
		lpCalcRegGuildList[*lpiCount].iGuildMasterLevel = m_DBQuery.GetInt("GM_LEVEL");
		lpCalcRegGuildList[*lpiCount].iSeqNum = m_DBQuery.GetInt("SEQ_NUM");
		
		*lpiCount = *lpiCount + 1;
		
		sqlRet = m_DBQuery.Fetch();
	}
	
	if (sqlRet == SQL_NULL_DATA)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCsGuildUnionInfo(int iMapSvrGroup, char* lpszGuildName, int iCsGuildID, CSP_CSGUILDUNIONINFO* lpCsGuildUnionInfo, int* lpiCount)
{
	if (lpszGuildName == NULL || lpCsGuildUnionInfo == NULL || lpiCount == NULL)
	{
		return TRUE;
	}

	int iRET_COUNT = *lpiCount;
	
	if ( iRET_COUNT < 0 )
	{
		return TRUE;
	}
	
	CString qSql;
	qSql.Format("EXEC WZ_CS_GetCsGuildUnionInfo '%s'",lpszGuildName);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	short sqlRet = m_DBQuery.Fetch();

	while(sqlRet != SQL_NO_DATA && sqlRet != SQL_NULL_DATA)
	{
		if (iRET_COUNT > 100)
		{
			break;
		}

		char szGuildName[MAX_GUILDNAMESTRING+MAX_GUILDNAMESTRING] = {'\0'};
		
		m_DBQuery.GetStr("GUILD_NAME", szGuildName);
		
		memcpy(&lpCsGuildUnionInfo[iRET_COUNT], szGuildName, MAX_GUILDNAMESTRING);
		lpCsGuildUnionInfo[iRET_COUNT].iCsGuildID = iCsGuildID;
		
		iRET_COUNT++;
		
		sqlRet = m_DBQuery.Fetch();
	}
	
	if (sqlRet == SQL_NULL_DATA)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	m_DBQuery.Clear();
	
	*lpiCount = iRET_COUNT;
	
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCsClearTotalGuildInfo(int iMapSvrGroup)
{
	CString qSql;
	qSql.Format("DELETE MuCastle_SIEGE_GUILDLIST WHERE MAP_SVR_GROUP = %d",iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCsSaveTotalGuildInfo(int iMapSvrGroup, char* lpszGuildName, int iCsGuildID, int iCsGuildInvolved, int iCsGuildScore)
{
	if (lpszGuildName == NULL)
	{
		return TRUE;
	}

	CString qSql;
	qSql.Format("EXEC WZ_CS_SetSiegeGuildInfo %d, '%s', %d, %d, %d",
		iMapSvrGroup, lpszGuildName, iCsGuildID, iCsGuildInvolved, iCsGuildScore);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCsSaveTotalGuildOK(int iMapSvrGroup, int* lpiResult)
{
	if (lpiResult == NULL)
	{
		return TRUE;
	}
	
	CString qSql;
	qSql.Format("EXEC WZ_CS_SetSiegeGuildOK %d",iMapSvrGroup);
	
	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	*lpiResult = m_DBQuery.GetInt("QueryResult");
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCsLoadTotalGuildInfo(int iMapSvrGroup, CSP_CSLOADTOTALGUILDINFO* lpLoadTotalGuildInfo, int* lpiCount)
{
	if (lpLoadTotalGuildInfo == NULL || lpiCount == NULL)
	{
		return TRUE;
	}
	
	int iMAX_COUNT = *lpiCount;
	
	if ( iMAX_COUNT > 100 )
	{
		iMAX_COUNT = 100;
	}
	
	*lpiCount=0;
	
	CString qSql;
	qSql.Format("EXEC WZ_CS_GetSiegeGuildInfo %d",iMapSvrGroup);

	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	short sqlRet = m_DBQuery.Fetch();

	while(sqlRet != SQL_NO_DATA && sqlRet != SQL_NULL_DATA)
	{
		if (*lpiCount > iMAX_COUNT)
		{
			break;
		}
		
		char szGuildName[MAX_GUILDNAMESTRING+MAX_GUILDNAMESTRING] = {'\0'};
		
		m_DBQuery.GetStr("GUILD_NAME", szGuildName);
		
		memcpy(&lpLoadTotalGuildInfo[*lpiCount], szGuildName, MAX_GUILDNAMESTRING);
		
		lpLoadTotalGuildInfo[*lpiCount].iCsGuildID = m_DBQuery.GetInt("GUILD_ID");
		lpLoadTotalGuildInfo[*lpiCount].iGuildInvolved = m_DBQuery.GetInt("GUILD_INVOLVED");
		lpLoadTotalGuildInfo[*lpiCount].iGuildScore = m_DBQuery.GetInt("GUILD_SCORE");
		
		*lpiCount = *lpiCount + 1;
		
		sqlRet = m_DBQuery.Fetch();
	}

	if (sqlRet == SQL_NULL_DATA)
	{
		m_DBQuery.Clear();
		return TRUE;
	}
	
	m_DBQuery.Clear();
	return FALSE;
}

BOOL CCastleDBSet::DSDB_QueryCastleNpcUpdate(int iMapSvrGroup, CSP_REQ_NPCUPDATEDATA* lpNpcSaveData)
{
	if (lpNpcSaveData == NULL)
	{
		return TRUE;
	}

	CSP_NPCSAVEDATA* lpMsgBody = (CSP_NPCSAVEDATA*)&lpNpcSaveData[1];

	CString qSql;
	qSql.Format("DELETE MuCastle_NPC WHERE MAP_SVR_GROUP = %d",iMapSvrGroup);

	if (m_DBQuery.Exec(qSql) == FALSE)
	{
		m_DBQuery.Clear();
		return TRUE;
	}

	m_DBQuery.Clear();
	
	for (int iNPC_COUNT = 0; iNPC_COUNT < lpNpcSaveData->iCount; iNPC_COUNT++)
	{
		qSql.Format("EXEC WZ_CS_ReqNpcUpdate %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			iMapSvrGroup,lpMsgBody[iNPC_COUNT].iNpcNumber,lpMsgBody[iNPC_COUNT].iNpcIndex,
			lpMsgBody[iNPC_COUNT].iNpcDfLevel,lpMsgBody[iNPC_COUNT].iNpcRgLevel,
			lpMsgBody[iNPC_COUNT].iNpcMaxHp,lpMsgBody[iNPC_COUNT].iNpcHp,
			lpMsgBody[iNPC_COUNT].btNpcX,lpMsgBody[iNPC_COUNT].btNpcY,
			lpMsgBody[iNPC_COUNT].btNpcDIR);
		
		if (m_DBQuery.Exec(qSql) == FALSE)
		{
			m_DBQuery.Clear();
			return TRUE;
		}
		
		m_DBQuery.Clear();
	}

	return FALSE;
}