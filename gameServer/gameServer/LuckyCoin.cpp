#include "StdAfx.h"
#include "LuckyCoin.h"
#include "Defines.h"
#include "Notice.H"
#include "DSProtocol.h"
#include "SendProto.H"


CLuckyCoin gLuckyCoin;

CLuckyCoin::CLuckyCoin(void)
{
	return;
}

CLuckyCoin::~CLuckyCoin(void)
{
	return;
}

void CLuckyCoin::Init()
{
	gLuckyCoin.Enabled = 1;

	if ( gLuckyCoin.Enabled == 1 )
	{
		gLuckyCoin.ReadFile();
	}
	return;
}

void CLuckyCoin::ReadFile()
{
	return;
}

void CLuckyCoin::OpenRegisterWindow(LPOBJ lpObj)
{
	BYTE Packet[6] = { 0xC1, 0x06, 0xBF, 0x0B, 0xFF, 0xFF } ;
	gSendProto.DataSend(lpObj->m_Index,Packet,Packet[1]);
	return;
}

void CLuckyCoin::RegisterCoin(LPOBJ lpObj)
{
	int Count = 0;
	// ----
	Count = gLuckyCoin.GetCoinCount(lpObj);
	SendMsg->MessageOut(lpObj->m_Index, 0x01, "Coin Count : %d",Count);
	if ( Count <= 0 )
	{
		return;
	}

	return;
}

int CLuckyCoin::GetCoinPos(LPOBJ lpObj)
{
	for (int i=12;i<MAIN_INVENTORY_SIZE;i++)
	{
		if( lpObj->pInventory[i].IsItem() == true )
		{
			if ( lpObj->pInventory[i].m_Type == ITEMGET(14,100) )
			{
				int op = (int)lpObj->pInventory[i].m_Durability;

				if ( ITEM_DUR_RANGE(op) != FALSE)
				{
					return i;
				}
			}
		}
	}
	return -1;
}

int CLuckyCoin::GetCoinCount(LPOBJ lpObj)
{
	int Count = 0;
	// ----
	for (int i=12;i<MAIN_INVENTORY_SIZE;i++)
	{
		if( lpObj->pInventory[i].IsItem() == true )
		{
			if ( lpObj->pInventory[i].m_Type == ITEMGET(14,100) )
			{
				Count+=lpObj->pInventory[i].m_Durability;
			}
		}
	}
	return Count;
}

void CLuckyCoin::DeleteCoin(LPOBJ lpObj)
{
	
}

void CLuckyCoin::ExchangeCoin(LPOBJ lpObj,int Type)
{
	int Count = 0;
	BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
	CItem item;
	// ----
	switch ( Type )
	{
	case 0:	// -- 10 Coin Change
		Count = gLuckyCoin.GetCoinCount(lpObj);
		if ( Count < 10 )
		{
			GCServerMsgStringSend("[LuckyCoin]: Need 10 coins",lpObj->m_Index,1);
			return;
		}
		else
		{
			item.m_Type = ITEMGET(14,13);
			// ----
			if ( gObjInventoryInsertItem(lpObj->m_Index,item) == -1 )
			{
				return;
			}
			for (int i=0;i<10;i++)
			{
				gLuckyCoin.DeleteCoin(lpObj);
			}

			//ItemSerialCreateSend(lpObj->m_Index,236,0,0,ITEMGET(14,14),0,0,0,0,0,lpObj->m_Index,0,0,Socket);
		}
		break;
	case 1:	// -- 20 Coin Change
		Count = gLuckyCoin.GetCoinCount(lpObj);
		if ( Count < 20 )
		{
			GCServerMsgStringSend("[LuckyCoin]: Need 20 coins",lpObj->m_Index,1);
			return;
		}
		else
		{
			item.m_Type = ITEMGET(14,14);
			// ----
			if ( gObjInventoryInsertItem(lpObj->m_Index,item) == -1 )
			{
				return;
			}
			for (int i=0;i<20;i++)
			{
				gLuckyCoin.DeleteCoin(lpObj);
			}
			//ItemSerialCreateSend(lpObj->m_Index,236,0,0,ITEMGET(14,13),0,0,0,0,0,lpObj->m_Index,0,0,Socket);
		}
		break;
	case 2:	// -- 30 Coin Change
		Count = gLuckyCoin.GetCoinCount(lpObj);
		if ( Count < 30 )
		{
			GCServerMsgStringSend("[LuckyCoin]: Need 30 coins",lpObj->m_Index,1);
			return;
		}
		else
		{
			item.m_Type = ITEMGET(14,16);
			// ----
			if ( gObjInventoryInsertItem(lpObj->m_Index,item) == -1 )
			{
				return;
			}
			for (int i=0;i<30;i++)
			{
				gLuckyCoin.DeleteCoin(lpObj);
			}
			//ItemSerialCreateSend(lpObj->m_Index,236,0,0,ITEMGET(14,16),0,0,0,0,0,lpObj->m_Index,0,0,Socket);
		}
		break;
	}
}