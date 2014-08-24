//GameServer 1.00.77 JPN - Completed
//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include "ItemBagEx.h"
#include "gObjMonster.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "logproc.h"

CItemBagEx::CItemBagEx()
{
	return;
}

CItemBagEx::~CItemBagEx()
{
	return;
}

void CItemBagEx::Init(char* name)
{
	this->m_bLoad = FALSE;
	this->m_sEventName[0] = 0;
	this->m_iEventItemType = -1;
	this->m_iEventItemLevel = 0;
	this->m_iDropZen = 0;
	this->m_iEventItemDropRate = 0;
	this->m_iItemDropRate = 0;
	this->m_iExItemDropRate = 0;
	this->m_iBagObjectCount = 0;

	this->LoadItem(gDirPath.GetNewPath(name));
}

void CItemBagEx::LoadItem(char* script_file)
{
	int Token;

	this->m_bLoad = FALSE;
	SMDFile = fopen(script_file, "r");	//ok

	if ( SMDFile == NULL )
	{
		LogAdd(lMsg.Get(MSGGET(1, 197)), script_file);
		return;
	}

	int n=0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			int st = TokenNumber;	// script_type

			if ( st == 0 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					int map = TokenNumber;

					if ( MAX_MAP_RANGE(map) == FALSE )
					{
						MsgBox("ExEvent ItemBag LoadFail [%s]", script_file);
						return;
					}

					Token = GetToken();
					this->DropMapInfo[map].m_bIsDrop = TokenNumber;

					Token = GetToken();
					this->DropMapInfo[map].m_MinMonsterLevel = TokenNumber;

					Token = GetToken();
					this->DropMapInfo[map].m_MaxMonsterLevel = TokenNumber;
				}
			
			}
			else if ( st == 1 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					strcpy(this->m_sEventName, TokenString);

					Token = GetToken();
					this->m_iDropZen = TokenNumber;

					Token = GetToken();
					int type = TokenNumber;

					Token = GetToken();
					int index = TokenNumber;

					this->m_iEventItemType = ITEMGET(type, index);

					Token = GetToken();
					this->m_iEventItemLevel = TokenNumber;

					Token = GetToken();
					this->m_iEventItemDropRate = TokenNumber;

					Token = GetToken();
					this->m_iItemDropRate = TokenNumber;

					Token = GetToken();
					this->m_iExItemDropRate = TokenNumber;

					char szTemp[256];

					wsprintf(szTemp, "[%s] Eventitemnum = %d,EventItemLevel = %d, EventItemDropRate = %d, ItemDropRate = %d, ExItemDropRate = %d",
						this->m_sEventName, this->m_iEventItemType, this->m_iEventItemLevel, this->m_iEventItemDropRate, this->m_iItemDropRate, this->m_iExItemDropRate);
					LogAddTD(szTemp);
				}
			}
			else if ( st == 2 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( Token == 0 )
					{
						if ( strcmp("end", TokenString) == 0 )
						{
							break;
						}
					}

					this->BagObject[n].m_type = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_index = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_minLevel = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_maxLevel = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_isskill = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_isluck = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_isoption = TokenNumber;

					Token = GetToken();
					this->BagObject[n].m_isexitem = TokenNumber;

					n++;
					this->m_iBagObjectCount++;

					if ( this->m_iBagObjectCount > MAX_ITEMBAG_ATTR-1 )
					{
						break;
					}
				}
			}
		}
	}

	fclose(SMDFile);
	LogAdd(lMsg.Get(MSGGET(1, 198)), script_file);
	this->m_bLoad = TRUE;
}


BYTE CItemBagEx::GetLevel(int n)
{
	if ( n <0 || n > MAX_ITEMBAG_ATTR-1 )
	{
		return 0;
	}

	if ( this->BagObject[n].m_minLevel == this->BagObject[n].m_maxLevel )
	{
		return this->BagObject[n].m_minLevel;
	}

	int sub = (this->BagObject[n].m_maxLevel - this->BagObject[n].m_minLevel) + 1;
	int level = this->BagObject[n].m_minLevel + (rand()%sub);

	return level;
}

BOOL CItemBagEx::IsEnableEventItemDrop(int aIndex)
{
	int iMapNumber = gObj[aIndex].MapNumber;

	if ( this->DropMapInfo[iMapNumber].m_bIsDrop == FALSE )
	{
		return FALSE;
	}

	int iLevel = gObj[aIndex].Level;

	if  ( iLevel < this->DropMapInfo[iMapNumber].m_MinMonsterLevel || iLevel > this->DropMapInfo[iMapNumber].m_MaxMonsterLevel)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CItemBagEx::DropEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	if ( this->IsEnableEventItemDrop(aIndex) == FALSE )
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( (rand()%1000) < this->m_iEventItemDropRate )
	{
		int ei;
		int eil;
		int x;
		int y;
		float dur = 0;
		dur = 255.0;

		x = lpObj->X;
		y = lpObj->Y;
		eil = this->m_iEventItemLevel;
		ei = this->m_iEventItemType;

		int thdu = gObjMonsterTopHitDamageUser(lpObj);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x ,y, ei, eil, dur, 0, 0, 0, thdu, 0, 0);
		LogAddTD("[%s][%s] [%s] MonsterEventItemDrop (%d)(%d/%d)", lpObj->AccountID, lpObj->Name, 
			this->m_sEventName, lpObj->MapNumber, x, y);
		return TRUE;
	}

	return FALSE;
}


BOOL CItemBagEx::DropItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		if ( (rand()%100) < this->m_iItemDropRate )
		{
			DropItemNum =  rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
			{
				return FALSE;
			}

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
			{
				Option1 = rand()%2;
			}

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = rand()%2;
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( rand()%5 < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand()%3;
				}
			}

			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = NewOptionRand(0);
				Option1 = 1;
			}

			if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(13,0) || type == ITEMGET(13,1) || type ==ITEMGET(13,2) ||
				 type == ITEMGET(13,8) || type == ITEMGET(13,9) || type == ITEMGET(13,12) ||
				 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
			{
				level = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);
			LogAddTD("[%s][%s] [%s] Event ItemDrop : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
				lpObj->AccountID, lpObj->Name, this->m_sEventName, lpObj->MapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
			return TRUE;
		}
	}

	x = lpObj->X;
	y = lpObj->Y;
	MapC[lpObj->MapNumber].MoneyItemDrop(this->m_iDropZen, x, y);
	return TRUE;
}


BYTE CItemBagEx::GetMinLevel(int n)
{
	if ( n<0 || n > MAX_ITEMBAG_ATTR-1 )
		return 0;

	return this->BagObject[n].m_minLevel;
}

BYTE CItemBagEx::GetMaxLevel(int n)
{
	if ( n<0 || n > MAX_ITEMBAG_ATTR-1 )
		return 0;

	return this->BagObject[n].m_maxLevel;
}


BOOL CItemBagEx::PickItem(CItem & objItem, int & nItemIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	int DropItemNum;

	if ( this->GetBagCount() > 0 )
	{
		DropItemNum = rand() % this->GetBagCount();
		nItemIndex = DropItemNum;
		objItem.m_Type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);
		objItem.m_Level = this->GetLevel(DropItemNum);

		if ( objItem.m_Type == -1 )
			return FALSE;

		if ( this->BagObject[DropItemNum].m_isskill != 0 )
			objItem.m_Option1 = 1;

		if ( this->BagObject[DropItemNum].m_isluck != 0 )
			objItem.m_Option2 = 1;

		if ( this->BagObject[DropItemNum].m_isoption != 0 )
			objItem.m_Option3 = 1;

		if ( this->BagObject[DropItemNum].m_isexitem != 0 )
		{
			objItem.m_NewOption = 1;
			objItem.m_Option1 = 0;
			objItem.m_Option2 = 0;
		}

		if ( objItem.m_Type == ITEMGET(12,15) ||
			 objItem.m_Type == ITEMGET(14,13) ||
			 objItem.m_Type == ITEMGET(14,14) )
		{
			objItem.m_Option1 = 0;
			objItem.m_Option2 = 0;
			objItem.m_Option3 = 0;
			objItem.m_Level = 0;
		}

		if ( objItem.m_Type == ITEMGET(13,0) ||
			 objItem.m_Type == ITEMGET(13,1) ||
			 objItem.m_Type == ITEMGET(13,2) ||
			 objItem.m_Type == ITEMGET(13,8) ||
			 objItem.m_Type == ITEMGET(13,9) ||
			 objItem.m_Type == ITEMGET(13,12) ||
			 objItem.m_Type == ITEMGET(13,13) )
		{
			objItem.m_Level = 0;
		}

		objItem.m_Durability = 0;

		return TRUE;
	}

	return FALSE;
}

BOOL CItemBagEx::DropRedRibbonBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemRate = rand() % 10000;

		if ( DropItemRate < g_iRedRibbonBoxDropZenRate )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iRedRibbonBoxDropZen, 
				lpObj->X, lpObj->Y);

			LogAddTD("[EVENT CHRISTMAS RIBBONBOX] RIBBONBOX Event ItemDrop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name,
				g_iRedRibbonBoxDropZen);
		}
		else
		{
			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
				return FALSE;

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
				Option1 = 1;

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			ExOption = 0;

			if ( type == ITEMGET(12,15) ||
				 type == ITEMGET(14,13) ||
				 type == ITEMGET(14,14) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(14,19) ||
				 type == ITEMGET(14,28) ||
				 type == ITEMGET(13,18) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[EVENT CHRISTMAS RIBBONBOX ] REDRIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, this->m_sEventName,
				lpObj->MapNumber, x, y, ItemAttribute[type].Name,
				type, level, Option1, Option2, Option3, ((ExOption&0x20) >> 5), ((ExOption&0x10) >> 4), ((ExOption&0x08) >> 3), ((ExOption&0x04) >> 2), ((ExOption&0x02) >> 1), (ExOption&0x01), 0);
		}

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}


BOOL CItemBagEx::DropGreenRibbonBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemRate = rand() % 10000;

		if ( DropItemRate < g_iGreenRibbonBoxDropZenRate )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iGreenRibbonBoxDropZen, 
				lpObj->X, lpObj->Y);

			LogAddTD("[EVENT CHRISTMAS RIBBONBOX] GREENRIBBONBOX Event ItemDrop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name,
				g_iGreenRibbonBoxDropZen);
		}
		else
		{
			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
				return FALSE;

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
				Option1 = 1;

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			if ( this->BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				Option2 = 0;
				Option1 = 1;
				level = 0;
			}

			if ( type == ITEMGET(12,15) ||
				 type == ITEMGET(14,13) ||
				 type == ITEMGET(14,14) ||
				 type == ITEMGET(14,16) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(14,19) ||
				 type == ITEMGET(14,28) ||
				 type == ITEMGET(13,18) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[EVENT CHRISTMAS RIBBONBOX ] GREENRIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, this->m_sEventName,
				lpObj->MapNumber, x, y, ItemAttribute[type].Name,
				type, level, Option1, Option2, Option3, ((ExOption&0x20) >> 5), ((ExOption&0x10) >> 4), ((ExOption&0x08) >> 3), ((ExOption&0x04) >> 2), ((ExOption&0x02) >> 1), (ExOption&0x01), 0);
		}

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}




BOOL CItemBagEx::DropBlueRibbonBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemRate = rand() % 10000;

		if ( DropItemRate < g_iBlueRibbonBoxDropZenRate )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iBlueRibbonBoxDropZen, 
				lpObj->X, lpObj->Y);

			LogAddTD("[EVENT CHRISTMAS RIBBONBOX] BLUERIBBONBOX Event ItemDrop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name,
				g_iBlueRibbonBoxDropZen);
		}
		else
		{
			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
				return FALSE;

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
				Option1 = 1;

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			if ( this->BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				Option2 = 0;
				Option1 = 1;
				level = 0;
			}

			if ( type == ITEMGET(12,15) ||
				 type == ITEMGET(14,13) ||
				 type == ITEMGET(14,14)  )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(13,8) ||
				 type == ITEMGET(13,9) ||
				 type == ITEMGET(13,12) ||
				 type == ITEMGET(13,21) )
			{
				level = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[EVENT CHRISTMAS RIBBONBOX ] BLUERIBBONBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, this->m_sEventName,
				lpObj->MapNumber, x, y, ItemAttribute[type].Name,
				type, level, Option1, Option2, Option3, ((ExOption&0x20) >> 5), ((ExOption&0x10) >> 4), ((ExOption&0x08) >> 3), ((ExOption&0x04) >> 2), ((ExOption&0x02) >> 1), (ExOption&0x01), 0);
		}

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}



BOOL CItemBagEx::DropPinkChocolateBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemRate = rand() % 10000;

		if ( DropItemRate < g_iPinkChocolateBoxDropZenRate )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iPinkChocolateBoxDropZen, 
				lpObj->X, lpObj->Y);

			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX] PINKCHOCOLATE BOX Event ItemDrop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name,
				g_iPinkChocolateBoxDropZen);
		}
		else
		{
			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
				return FALSE;

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
				Option1 = 1;

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			ExOption = 0;

			if ( type == ITEMGET(12,15) ||
				 type == ITEMGET(14,13) ||
				 type == ITEMGET(14,14) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(14,19) ||
				 type == ITEMGET(14,28) ||
				 type == ITEMGET(13,18) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX ] PINKCHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, this->m_sEventName,
				lpObj->MapNumber, x, y, ItemAttribute[type].Name,
				type, level, Option1, Option2, Option3, ((ExOption&0x20) >> 5), ((ExOption&0x10) >> 4), ((ExOption&0x08) >> 3), ((ExOption&0x04) >> 2), ((ExOption&0x02) >> 1), (ExOption&0x01), 0);
		}

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}


BOOL CItemBagEx::DropRedChocolateBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemRate = rand() % 10000;

		if ( DropItemRate < g_iRedChocolateBoxDropZenRate )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iRedChocolateBoxDropZen, 
				lpObj->X, lpObj->Y);

			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX] REDCHOCOLATEBOX Event ItemDrop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name,
				g_iRedChocolateBoxDropZen);
		}
		else
		{
			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
				return FALSE;

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
				Option1 = 1;

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			if ( this->BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				Option2 = 0;
				Option1 = 1;
				level = 0;
			}

			if ( type == ITEMGET(12,15) ||
				 type == ITEMGET(14,13) ||
				 type == ITEMGET(14,14) ||
				 type == ITEMGET(14,16) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(14,19) ||
				 type == ITEMGET(14,28) ||
				 type == ITEMGET(13,18) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX ] REDCHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, this->m_sEventName,
				lpObj->MapNumber, x, y, ItemAttribute[type].Name,
				type, level, Option1, Option2, Option3, ((ExOption&0x20) >> 5), ((ExOption&0x10) >> 4), ((ExOption&0x08) >> 3), ((ExOption&0x04) >> 2), ((ExOption&0x02) >> 1), (ExOption&0x01), 0);
		}

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}




BOOL CItemBagEx::DropBlueChocolateBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemRate = rand() % 10000;

		if ( DropItemRate < g_iBlueChocolateBoxDropZenRate )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iBlueChocolateBoxDropZen, 
				lpObj->X, lpObj->Y);

			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX] BLUECHOCOLATEBOX Event ItemDrop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name,
				g_iBlueChocolateBoxDropZen);
		}
		else
		{
			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
				return FALSE;

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
				Option1 = 1;

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			if ( this->BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				Option2 = 0;
				Option1 = 1;
				level = 0;
			}

			if ( type == ITEMGET(12,15) ||
				 type == ITEMGET(14,13) ||
				 type == ITEMGET(14,14)  )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(13,8) ||
				 type == ITEMGET(13,9) ||
				 type == ITEMGET(13,12) ||
				 type == ITEMGET(13,21) )
			{
				level = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[EVENT VALENTINE'S DAY CHOCOLATEBOX ] BLUECHOCOLATEBOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d  Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, this->m_sEventName,
				lpObj->MapNumber, x, y, ItemAttribute[type].Name,
				type, level, Option1, Option2, Option3, ((ExOption&0x20) >> 5), ((ExOption&0x10) >> 4), ((ExOption&0x08) >> 3), ((ExOption&0x04) >> 2), ((ExOption&0x02) >> 1), (ExOption&0x01), 0);
		}

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}



BOOL CItemBagEx::DropLightPurpleCandyBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemRate = rand() % 10000;

		if ( DropItemRate < g_iLightPurpleCandyBoxDropZenRate )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iLightPurpleCandyBoxDropZen, 
				lpObj->X, lpObj->Y);

			LogAddTD("[EVENT WHITE DAY CANDY BOX] LIGHT PURPLE BOX Event ItemDrop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name,
				g_iLightPurpleCandyBoxDropZen);
		}
		else
		{
			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
				return FALSE;

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
				Option1 = 1;

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			ExOption = 0;

			if ( type == ITEMGET(12,15) ||
				 type == ITEMGET(14,13) ||
				 type == ITEMGET(14,14) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(14,19) ||
				 type == ITEMGET(14,28) ||
				 type == ITEMGET(13,18) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[EVENT WHITE DAY CANDY BOX] LIGHT PURPLE BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, this->m_sEventName,
				lpObj->MapNumber, x, y, ItemAttribute[type].Name,
				type, level, Option1, Option2, Option3, ((ExOption&0x20) >> 5), ((ExOption&0x10) >> 4), ((ExOption&0x08) >> 3), ((ExOption&0x04) >> 2), ((ExOption&0x02) >> 1), (ExOption&0x01), 0);
		}

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}


BOOL CItemBagEx::DropVermilionCandyBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemRate = rand() % 10000;

		if ( DropItemRate < g_iVermilionCandyBoxDropZenRate )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iVermilionCandyBoxDropZen, 
				lpObj->X, lpObj->Y);

			LogAddTD("[EVENT WHITE DAY CANDY BOX] VERMILION BOX Event ItemDrop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name,
				g_iVermilionCandyBoxDropZen);
		}
		else
		{
			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
				return FALSE;

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
				Option1 = 1;

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			if ( this->BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				Option2 = 0;
				Option1 = 1;
				level = 0;
			}

			if ( type == ITEMGET(12,15) ||
				 type == ITEMGET(14,13) ||
				 type == ITEMGET(14,14) ||
				 type == ITEMGET(14,16) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(14,19) ||
				 type == ITEMGET(14,28) ||
				 type == ITEMGET(13,18) )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[EVENT WHITE DAY CANDY BOX] VERMILION BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, this->m_sEventName,
				lpObj->MapNumber, x, y, ItemAttribute[type].Name,
				type, level, Option1, Option2, Option3, ((ExOption&0x20) >> 5), ((ExOption&0x10) >> 4), ((ExOption&0x08) >> 3), ((ExOption&0x04) >> 2), ((ExOption&0x02) >> 1), (ExOption&0x01), 0);
		}

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}




BOOL CItemBagEx::DropDeepBlueCandyBoxEventItem(int aIndex)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemRate;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		DropItemRate = rand() % 10000;

		if ( DropItemRate < g_iDeepBlueCandyBoxDropZenRate )
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iDeepBlueCandyBoxDropZen, 
				lpObj->X, lpObj->Y);

			LogAddTD("[EVENT WHITE DAY CANDY BOX] DEEP BLUE BOX Event ItemDrop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name,
				g_iDeepBlueCandyBoxDropZen);
		}
		else
		{
			DropItemNum = rand() % this->GetBagCount();
			dur = 0;
			x = lpObj->X;
			y = lpObj->Y;
			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
				return FALSE;

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
				Option1 = 1;

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( (rand()%5) < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand() % 3;
				}
			}

			if ( this->BagObject[DropItemNum].m_isexitem )
			{
				ExOption = NewOptionRand(level);
				Option2 = 0;
				Option1 = 1;
				level = 0;
			}

			if ( type == ITEMGET(12,15) ||
				 type == ITEMGET(14,13) ||
				 type == ITEMGET(14,14)  )
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(13,8) ||
				 type == ITEMGET(13,9) ||
				 type == ITEMGET(13,12) ||
				 type == ITEMGET(13,21) )
			{
				level = 0;
			}
			
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[EVENT WHITE DAY CANDY BOX] DEEP BLUE BOX Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d Ex:[%d,%d,%d,%d,%d,%d,%d]",
				lpObj->AccountID, lpObj->Name, this->m_sEventName,
				lpObj->MapNumber, x, y, ItemAttribute[type].Name,
				type, level, Option1, Option2, Option3, ((ExOption&0x20) >> 5), ((ExOption&0x10) >> 4), ((ExOption&0x08) >> 3), ((ExOption&0x04) >> 2), ((ExOption&0x02) >> 1), (ExOption&0x01), 0);
		}

		PMSG_SERVERCMD ServerCmd;

		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = lpObj->X;
		ServerCmd.Y = lpObj->Y;

		MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	}

	return TRUE;
}

BOOL CItemBagEx::DropHallowinEventItem(LPOBJ lpObj)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	int iType;
	int iLevel;
	int X;
	int Y;
	int iDropItemNum = 0;
	int iDropItemRate;
	int iDuration = 0;
	int iTotalDropRate = 0;
	int iItemBagCount = this->GetBagCount();

	if ( iItemBagCount > 0 )
	{
		iTotalDropRate = g_iHallowinDayEventJOLBlessDropRate +
			             g_iHallowinDayEventJOLAngerDropRaTe +
						 g_iHallowinDayEventJOLScreamDropRate +
						 g_iHallowinDayEventJOLFoodDropRate +
						 g_iHallowinDayEventJOLDrinkDropRate +
						 g_iHallowinDayEventJOLPolymorphRingDropRate;

		iDropItemRate = rand() % iTotalDropRate;

		if ( iDropItemRate >= (iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate)  )
			iDropItemNum = 0;
		else if ( iDropItemRate >= (iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe) )
			iDropItemNum = 1;
		else if ( iDropItemRate >= (iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe - g_iHallowinDayEventJOLScreamDropRate) )
			iDropItemNum = 2;
		else if ( iDropItemRate >= (iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe - g_iHallowinDayEventJOLScreamDropRate - g_iHallowinDayEventJOLFoodDropRate) )
			iDropItemNum = 3;
		else if ( iDropItemRate >= (iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe - g_iHallowinDayEventJOLScreamDropRate - g_iHallowinDayEventJOLFoodDropRate - g_iHallowinDayEventJOLDrinkDropRate) )
			iDropItemNum = 4;
		else if ( iDropItemRate >= (iTotalDropRate - g_iHallowinDayEventJOLBlessDropRate - g_iHallowinDayEventJOLAngerDropRaTe - g_iHallowinDayEventJOLScreamDropRate - g_iHallowinDayEventJOLFoodDropRate - g_iHallowinDayEventJOLDrinkDropRate - g_iHallowinDayEventJOLPolymorphRingDropRate) )
			iDropItemNum = 5;

		X = lpObj->X;
		Y = lpObj->Y;
		iLevel = this->GetLevel(iDropItemNum);
		iType = ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( iType == ITEMGET(13,40) )
		{
			iLevel = 0;
			iDuration = 100;
		}

		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, X, Y,
			iType, iLevel, iDuration, 0, 0, 0, lpObj->m_Index, 0, 0);

		LogAddTD("[HallowinDay Event][Pumpkin of Luck] Event Item Drop [%s][%s] : (%d)(%d/%d) Item Attribute : %s(%d) Level : %d Dur : %d",
			lpObj->AccountID, lpObj->Name, lpObj->MapNumber, X, Y, ItemAttribute[iType].Name, iType, iLevel, iDuration);
	}

	return TRUE;
}

BOOL CItemBagEx::DropKundunEventItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int iItemBagCount = this->GetBagCount();

	for(int i=0;i<iItemBagCount;i++)
	{
		type = ItemGetNumberMake(this->BagObject[i].m_type, this->BagObject[i].m_index);

		LogAdd("[¡ÛKUNDUN EVENT][KUNDUN ITEM BAG LIST] Item:(%s)%d Level:%d-%d op1:%d op2:%d op3:%d expo:%d",
			ItemAttribute[type].Name, type, this->BagObject[i].m_minLevel, BagObject[i].m_maxLevel,
			BagObject[i].m_isskill, BagObject[i].m_isluck, BagObject[i].m_isoption, BagObject[i].m_isexitem);
	}

	if ( this->GetBagCount() > 0 )
	{
		if ( true )
		{
			DropItemNum =  rand() % this->GetBagCount();
			dur = 0;
			
			if ( cX == 0 && cY == 0 )
			{
				x = lpObj->X;
				y = lpObj->Y;
			}
			else
			{
				x = cX;
				y = cY;
			}

			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
			{
				return FALSE;
			}

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
			{
				Option1 =1;
			}

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( rand()%5 < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand()%3;
				}
			}

			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = NewOptionRand(0);
				Option2 = 0;
				Option1 = 1;
			}

			if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14) || type == ITEMGET(14,16))	// Chaos, Bless, Soul
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(13,0) ||
				 type == ITEMGET(13,1) ||
				 type == ITEMGET(13,2) ||
				 type == ITEMGET(13,8) ||
				 type == ITEMGET(13,9) ||
				 type == ITEMGET(13,12) ||
				 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
			{
				level = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[¡ÛKundun EVENT] ¡ÚKUNDUN Event ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
				lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);
			return TRUE;
		}
	}

	return TRUE;
}

//#if(_GSCS==1)
BOOL CItemBagEx::DropCastleHuntZoneBossReward(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY) //Identical
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float dur;
	int type;
	int level;
	int x;
	int y;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int DropItemNum;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		if ( true )
		{
			DropItemNum =  rand() % this->GetBagCount();
			dur = 0;

			if ( cX == 0 && cY == 0 )
			{
				x = lpObj->X;
				y = lpObj->Y;
			}
			else
			{
				x = cX;
				y = cY;
			}

			level = this->GetLevel(DropItemNum);
			type = ItemGetNumberMake(this->BagObject[DropItemNum].m_type, this->BagObject[DropItemNum].m_index);

			if ( type == -1 )
			{
				return FALSE;
			}

			if ( this->BagObject[DropItemNum].m_isskill != 0 )
			{
				Option1 =1;
			}

			if ( this->BagObject[DropItemNum].m_isluck != 0 )
			{
				Option2 = 0;

				if ( (rand()%2) == 0 )
				{
					Option2 = 1;
				}
			}

			if ( this->BagObject[DropItemNum].m_isoption != 0 )
			{
				if ( rand()%5 < 1 )
				{
					Option3 = 3;
				}
				else
				{
					Option3 = rand()%3;
				}
			}

			if ( this->BagObject[DropItemNum].m_isexitem != 0 )
			{
				ExOption = NewOptionRand(0);
				Option2 = 0;
				Option1 = 1;
			}

			if ( type == ITEMGET(12,15) || type == ITEMGET(14,13) || type == ITEMGET(14,14))	// Chaos, Bless, Soul
			{
				Option1 = 0;
				Option2 = 0;
				Option3 = 0;
				level = 0;
			}

			if ( type == ITEMGET(13,0) ||
				 type == ITEMGET(13,1) ||
				 type == ITEMGET(13,2) ||
				 type == ITEMGET(13,8) ||
				 type == ITEMGET(13,9) ||
				 type == ITEMGET(13,12) ||
				 type ==ITEMGET(13,13) )	// Angel, imp, unirioa, dino, r and pendant of ice, poisonm
			{
				level = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, btMapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpObj->m_Index, ExOption, 0);

			LogAddTD("[Castle HuntZone] ¡ÚCastle HuntZone Boss ItemDrop [%s][%s] [%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d",
				lpObj->AccountID, lpObj->Name, this->m_sEventName, btMapNumber, x, y, ItemAttribute[type].Name, type, level, Option1, Option2, Option3);

		}
		return TRUE;
	}
	return TRUE;
}
//#endif

BOOL CItemBagEx::DropCrywolfDarkElfItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int iDropItemNum = 0;
	int iDropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		iDropItemRate = rand() % 10000;

		if ( iDropItemRate < g_iCrywolfMonsterDarkElfDropZenRate ) 
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iCrywolfMonsterDarkElfDropZen, cX, cY);

			LogAddTD("[ Crywolf ][Reward] Dark Elf Item Drop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name, g_iCrywolfMonsterDarkElfDropZen);
		}
		else
		{
			iDropItemNum = rand()% this->GetBagCount();
			fDur = 0;

			if ( cX == 0 && cY == 0 )
			{
				X = lpObj->X;
				Y = lpObj->Y;
			}
			else
			{
				X = cX;
				Y = cY;
			}

			iLevel = this->GetLevel(iDropItemNum);
			iType = ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);

			if ( iType == -1 )
				return FALSE;

			if ( this->BagObject[iDropItemNum].m_isskill )
				iOption1 = 1;

			if ( this->BagObject[iDropItemNum].m_isluck )
			{
				iOption2 = 0;

				if ( (rand()%2) == 0 )
				{
					iOption2 = 1;
				}
			}

			if ( this->BagObject[iDropItemNum].m_isoption != 0 )
			{
				if ( rand()%5 < 1 )
				{
					iOption3 = 3;
				}
				else
				{
					iOption3 = rand()%3;
				}
			}

			if ( this->BagObject[iDropItemNum].m_isexitem != 0 )
			{
				iExOption = NewOptionRand(0);
				iOption2 = 0;
				iOption1 = 1;
				iLevel = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
				iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

			LogAddTD("[ Crywolf ][Reward] Dark Elf Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
				iType, iLevel, iOption1, iOption2, iOption3, iExOption);

			return TRUE;
		}
	}

	return TRUE;


}




BOOL CItemBagEx::DropCrywolfBossMonsterItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int iDropItemNum = 0;
	int iDropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		iDropItemRate = rand() % 10000;

		if ( iDropItemRate < g_iCrywolfBossMonsterDropZenRate ) 
		{
			MapC[lpObj->MapNumber].MoneyItemDrop(g_iCrywolfBossMonsterDropZen, cX, cY);

			LogAddTD("[ Crywolf ][Reward] Boss Monster Item Drop [%s][%s] [%d Zen]",
				lpObj->AccountID, lpObj->Name, g_iCrywolfBossMonsterDropZen);
		}
		else
		{
			iDropItemNum = rand()% this->GetBagCount();
			fDur = 0;

			if ( cX == 0 && cY == 0 )
			{
				X = lpObj->X;
				Y = lpObj->Y;
			}
			else
			{
				X = cX;
				Y = cY;
			}

			iLevel = this->GetLevel(iDropItemNum);
			iType = ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);

			if ( iType == -1 )
				return FALSE;

			if ( this->BagObject[iDropItemNum].m_isskill )
				iOption1 = 1;

			if ( this->BagObject[iDropItemNum].m_isluck )
			{
				iOption2 = 0;

				if ( (rand()%2) == 0 )
				{
					iOption2 = 1;
				}
			}

			if ( this->BagObject[iDropItemNum].m_isoption != 0 )
			{
				if ( rand()%5 < 1 )
				{
					iOption3 = 3;
				}
				else
				{
					iOption3 = rand()%3;
				}
			}

			if ( this->BagObject[iDropItemNum].m_isexitem != 0 )
			{
				iExOption = NewOptionRand(0);
				iOption2 = 0;
				iOption1 = 1;
				iLevel = 0;
			}

			ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
				iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

			LogAddTD("[ Crywolf ][Reward] Boss Monster Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
				lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
				iType, iLevel, iOption1, iOption2, iOption3, iExOption);

			return TRUE;
		}
	}

	return TRUE;
}


//#if(_GSCS==0)
BOOL CItemBagEx::DropKanturuMayaHandItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int iDropItemNum = 0;
	int iDropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		iDropItemNum = rand()% this->GetBagCount();
		fDur = 0;

		if ( cX == 0 && cY == 0 )
		{
			X = lpObj->X;
			Y = lpObj->Y;
		}
		else
		{
			X = cX;
			Y = cY;
		}

		iLevel = this->GetLevel(iDropItemNum);
		iType = ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( this->BagObject[iDropItemNum].m_isskill )
			iOption1 = 1;

		if ( this->BagObject[iDropItemNum].m_isluck )
		{
			iOption2 = 0;

			if ( (rand()%2) == 0 )
			{
				iOption2 = 1;
			}
		}

		if ( this->BagObject[iDropItemNum].m_isoption != 0 )
		{
			if ( rand()%5 < 1 )
			{
				iOption3 = 3;
			}
			else
			{
				iOption3 = rand()%3;
			}
		}

		if ( this->BagObject[iDropItemNum].m_isexitem != 0 )
		{
			iExOption = NewOptionRand(0);
			iOption2 = 0;
			iOption1 = 1;
			iLevel = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
			iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

		LogAddTD("[ KANTURU ][ Reward ] MayaHand Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
			iType, iLevel, iOption1, iOption2, iOption3, iExOption);

		return TRUE;
	}

	return TRUE;
}


BOOL CItemBagEx::DropKanturuNightmareItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY)
{
	if ( this->m_bLoad == FALSE )
		return FALSE;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int iDropItemNum = 0;
	int iDropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->GetBagCount() > 0 )
	{
		iDropItemNum = rand()% this->GetBagCount();
		fDur = 0;

		if ( cX == 0 && cY == 0 )
		{
			X = lpObj->X;
			Y = lpObj->Y;
		}
		else
		{
			X = cX;
			Y = cY;
		}

		iLevel = this->GetLevel(iDropItemNum);
		iType = ItemGetNumberMake(this->BagObject[iDropItemNum].m_type, this->BagObject[iDropItemNum].m_index);

		if ( iType == -1 )
			return FALSE;

		if ( this->BagObject[iDropItemNum].m_isskill )
			iOption1 = 1;

		if ( this->BagObject[iDropItemNum].m_isluck )
		{
			iOption2 = 0;

			if ( (rand()%2) == 0 )
			{
				iOption2 = 1;
			}
		}

		if ( this->BagObject[iDropItemNum].m_isoption != 0 )
		{
			if ( rand()%5 < 1 )
			{
				iOption3 = 3;
			}
			else
			{
				iOption3 = rand()%3;
			}
		}

		if ( this->BagObject[iDropItemNum].m_isexitem != 0 )
		{
			iExOption = NewOptionRand(0);
			iOption2 = 0;
			iOption1 = 1;
			iLevel = 0;
		}

		ItemSerialCreateSend(lpObj->m_Index, btMapNumber, X, Y, iType, iLevel, fDur,
			iOption1, iOption2, iOption3, lpObj->m_Index, iExOption, 0);

		LogAddTD("[ KANTURU ][ Reward ] Nightmare Item Drop [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			lpObj->AccountID, lpObj->Name, btMapNumber, X, Y, ItemAttribute[iType].Name,
			iType, iLevel, iOption1, iOption2, iOption3, iExOption);

		return TRUE;
	}

	return TRUE;
}
//#endif