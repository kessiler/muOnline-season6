//	GS-CS	1.00.90	JPN		-	Completed
#include "stdafx.h"
#include "CastleSiegeSync.h"
#include "TUnion.h"
#include "protocol.h"
#include "DSProtocol.h"
#include "MapServerManager.h"

CCastleSiegeSync::CCastleSiegeSync()
{
	this->Clear();
}

CCastleSiegeSync::~CCastleSiegeSync()
{
	return;
}

void CCastleSiegeSync::Clear()
{
	this->m_iCurCastleState = -1;
	this->m_iCurTaxRateChaos = 0;
	this->m_iCurTaxRateStore  = 0;
	this->m_lCastleTributeMoney = 0;
	this->m_iCsTributeMoneyTimer = 0;
	memset(this->m_szCastleOwnerGuild, 0, sizeof(this->m_szCastleOwnerGuild));
}

void CCastleSiegeSync::SetCastleOwnerGuild(char * lpszGuildName)
{
	if ( lpszGuildName == NULL )
	{
		return;
	}

	memset(this->m_szCastleOwnerGuild, 0, sizeof(this->m_szCastleOwnerGuild));
	memcpy(this->m_szCastleOwnerGuild, lpszGuildName, sizeof(this->m_szCastleOwnerGuild)/2);
}

int CCastleSiegeSync::GetTaxRateChaos(int iIndex)
{
	int iCurTaxRateChaos = this->m_iCurTaxRateChaos;

	if ( this->CheckCastleOwnerMember(iIndex) == TRUE || this->CheckCastleOwnerUnionMember(iIndex) == TRUE )
	{
		iCurTaxRateChaos = 0;
	}

	return iCurTaxRateChaos;
}

int CCastleSiegeSync::GetTaxRateStore(int iIndex)
{
	int iCurTaxRateStore = this->m_iCurTaxRateStore;

	if ( this->CheckCastleOwnerMember(iIndex) == TRUE || this->CheckCastleOwnerUnionMember(iIndex) == TRUE )
	{
		iCurTaxRateStore = 0;
	}

	return iCurTaxRateStore;
}

int CCastleSiegeSync::GetTaxHuntZone(int iIndex, BOOL bCheckOwnerGuild)
{
	int iCurTaxHuntZone = this->m_iCurTaxHuntZone;

	if ( bCheckOwnerGuild == TRUE )
	{
		if ( this->CheckCastleOwnerMember(iIndex) == TRUE || this->CheckCastleOwnerUnionMember(iIndex) == TRUE )
		{
			iCurTaxHuntZone = 0;
		}
	}

	return iCurTaxHuntZone;
}

void CCastleSiegeSync::AddTributeMoney(int iMoney)
{
	if ( this->m_lCastleTributeMoney < 0 )
	{
		InterlockedExchange((LPLONG)&this->m_lCastleTributeMoney, 0);
	}

	if ( iMoney <= 0 )
	{
		return;
	}

	if ( (this->m_lCastleTributeMoney + iMoney) > MAX_TRIBUTE_MONEY )
	{
		return;
	}

	InterlockedExchangeAdd((LPLONG)&this->m_lCastleTributeMoney, iMoney);
}

void CCastleSiegeSync::ResetTributeMoney()
{
	InterlockedExchange((LPLONG)&this->m_lCastleTributeMoney, 0);
}

void CCastleSiegeSync::AdjustTributeMoney()
{
	if ( this->m_lCastleTributeMoney < 0 )
	{
		InterlockedExchange((LPLONG)&this->m_lCastleTributeMoney, 0);
	}

	if ( this->m_lCastleTributeMoney == 0 )
	{
		return;
	}

	this->m_iCsTributeMoneyTimer++;

	this->m_iCsTributeMoneyTimer %= 180;

	if ( this->m_iCsTributeMoneyTimer != 0 )
	{
		return;
	}

	GS_GDReqCastleTributeMoney(g_MapServerManager.GetMapSvrGroup(), this->m_lCastleTributeMoney);
}

BOOL CCastleSiegeSync::CheckCastleOwnerMember(int iIndex)
{
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return FALSE;
	}

	if ( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
	{
		return FALSE;
	}

	if ( strcmp(gObj[iIndex].GuildName, this->m_szCastleOwnerGuild) != 0 )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCastleSiegeSync::CheckCastleOwnerUnionMember(int iIndex)
{
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return FALSE;
	}

	if ( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
	{
		return FALSE;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[iIndex].lpGuild;
	
	if ( lpGuildInfo == NULL )
	{
		return FALSE;
	}
	
	TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);

	if ( pUnionInfo == NULL )
	{
		return FALSE;
	}

	if ( strcmp( pUnionInfo->m_szMasterGuild, this->m_szCastleOwnerGuild) == 0 )
	{
		return TRUE;
	}

	return FALSE;
}

int CCastleSiegeSync::CheckOverlapCsMarks(int iIndex)
{
	for ( int x=INVENTORY_BAG_START;x<MAIN_INVENTORY_SIZE;x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,21) )
			{
				if ( gObj[iIndex].pInventory[x].m_Level == 3 )
				{
					int op = gObj[iIndex].pInventory[x].m_Durability;

					if ( ITEM_DUR_RANGE(op) != FALSE)
					{
						return x;
					}
				}
			}
		}
	}

	return -1;
}


CCastleSiegeSync g_CastleSiegeSync;