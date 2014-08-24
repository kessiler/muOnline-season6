//GameServer 1.00.77 JPN - Completed
//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include "DevilSquareGround.h"
#include "DevilSquare.h"
#include "GameMain.h"
#include "Logproc.h"
#include "..\common\winutil.h"
#include "CrywolfSync.h"
#include "BuffEffectSlot.h"
#ifdef WZQUEST
#include "QuestExpProgMng.h"
#endif
#ifdef __CUSTOMS__
#include "ShopPointEx.h"
#include "DSProtocol.h"
#endif

bool DevilSquareScoreSort(LPOBJ const & lhs,LPOBJ const & rhs) //0046E5A0
{
	if ( lhs->m_nEventScore == rhs->m_nEventScore )
	{
		if ( lhs->Level < rhs->Level)
		{
			return true;
		}

		return false;
	}

	if ( lhs->m_nEventScore > rhs->m_nEventScore)
	{
		return true;
	}

	return false;
}

CDevilSquareGround::CDevilSquareGround() 
{
	this->Init(0);
}

CDevilSquareGround::~CDevilSquareGround()
{
	//Dark Wizard until 4th Floor
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_1] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_2] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_3] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_4] = 170;
	
	//Dark Knight until 4th Floor
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_1] = 20;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_2] = 90;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_3] = 120;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_4] = 400;

	//Elf until 4th Floor
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_1] = 10;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_2] = 10;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_3] = 10;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_4] = 200;

	//Magumsa until 4th Floor
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_1] = 0;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_2] = 0;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_3] = 0;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_4] = 0;

	//Dark Wizard 5th & 6th Floor
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_5] = 170;
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_6] = 170;

	//Dark Knight 5th & 6th Floor
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_5] = 400;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_6] = 400;

	//Elf 5th & 6th Floor
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_5] = 200;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_6] = 200;

	//Magumsa 5th & 6th Floor
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_5] = 0;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_6] = 0;

	//Dark Lord All Floors
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_1] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_2] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_3] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_4] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_5] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_6] = 0;

	//Season3 add-on (All Classes 7th Floor)
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_7] = 170;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_7] = 400;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_7] = 200;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_7] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_7] = 0;
}

void CDevilSquareGround::Init(int Index)
{
	this->m_dwObjCount = 0;
	this->m_iIndex = Index;
	memset(this->m_DevilSquareMonsterInfo, 0xFF, sizeof(this->m_DevilSquareMonsterInfo));
	memset(this->m_DevilSquareBossMonsterInfo, 0xFF, sizeof(this->m_DevilSquareBossMonsterInfo));
}

void CDevilSquareGround::Clear()
{
	this->m_dwObjCount = 0;
}

void CDevilSquareGround::Set(WORD type, int starttime, int endtime)
{
	for (int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareMonsterInfo[i].m_Type == (WORD)-1  )
		{
			this->m_DevilSquareMonsterInfo[i].m_Type = type;
			this->m_DevilSquareMonsterInfo[i].m_RegenStartTime = starttime;
			this->m_DevilSquareMonsterInfo[i].m_RegenEndTime = endtime;
			this->m_DevilSquareMonsterInfo[i].m_iDevilSquareNumber = this->m_iIndex;
			
			return;
		}
	}
}

void CDevilSquareGround::SetBoss(WORD type, int starttime, int x, int y, int tx, int ty)
{
	for (int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareBossMonsterInfo[i].m_Type == (WORD)-1  )
		{
			this->m_DevilSquareBossMonsterInfo[i].m_Type = type;
			this->m_DevilSquareBossMonsterInfo[i].m_RegenStartTime = starttime;
			this->m_DevilSquareBossMonsterInfo[i].X = x;
			this->m_DevilSquareBossMonsterInfo[i].Y = y;
			this->m_DevilSquareBossMonsterInfo[i].TX = tx;
			this->m_DevilSquareBossMonsterInfo[i].TY = ty;
			this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber = this->m_iIndex;
			
			return;
		}
	}
}

#ifdef __CUSTOMS__
void CDevilSquareGround::SetBonus(int index, int zen, int exp, BYTE	ItemCount, WORD ItemID, BYTE ItemLevel)
#else
void CDevilSquareGround::SetBonus(int index, int zen, int exp)
#endif
{
	this->m_Bonus[index][0] = exp;
	this->m_Bonus[index][1] = zen;
#ifdef __CUSTOMS__
	this->m_Bonus[index][2] = ItemCount;
	this->m_Bonus[index][3] = ItemID;
	this->m_Bonus[index][4] = ItemLevel;
#endif
}

void CDevilSquareGround::IncExp(float exp) //season4.5 add-on
{
	for( int i = 0; i < 4; i++)
	{
		this->m_Bonus[i][1] *= exp; //check
	}
}

void CDevilSquareGround::IncObjCount()
{
	InterlockedIncrement((LPLONG)&this->m_dwObjCount);
}

long  CDevilSquareGround::GetObjCount()
{
	return this->m_dwObjCount;
}

void CDevilSquareGround::RegenBossMonster(int currtime)
{
	for ( int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareBossMonsterInfo[i].m_Type != (WORD)-1 )
		{
			if ( currtime == this->m_DevilSquareBossMonsterInfo[i].m_RegenStartTime )
			{
				int iMapNumber = (this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber >= DEVIL_SQUARE_GROUND_1&& this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber <= DEVIL_SQUARE_GROUND_4)?MAP_INDEX_DEVILSQUARE:(this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber>=DEVIL_SQUARE_GROUND_5  && this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber < DEVIL_SQUARE_GROUND_7+1 )?MAP_INDEX_DEVILSQUARE2:-1;

				int iMonIndex = gObjAddMonster(iMapNumber);

				if ( iMonIndex >= 0 )
				{
					gObjSetMonster(iMonIndex, this->m_DevilSquareBossMonsterInfo[i].m_Type);
					gObj[iMonIndex].MapNumber = iMapNumber;
					
					gMSetBase.GetBoxPosition(gObj[iMonIndex].MapNumber,	this->m_DevilSquareBossMonsterInfo[i].X, this->m_DevilSquareBossMonsterInfo[i].Y, this->m_DevilSquareBossMonsterInfo[i].TX, this->m_DevilSquareBossMonsterInfo[i].TY, gObj[iMonIndex].X, gObj[iMonIndex].Y);
					
					int map = gObj[iMonIndex].MapNumber;
					int x = gObj[iMonIndex].X;
					int y = gObj[iMonIndex].Y;

					LogAddTD("[DevilSquare] Boss %d %d %d create ", map, x, y);

					gObj[iMonIndex].TX = gObj[iMonIndex].X;
					gObj[iMonIndex].TY = gObj[iMonIndex].Y;
					gObj[iMonIndex].MTX = gObj[iMonIndex].X;
					gObj[iMonIndex].MTY = gObj[iMonIndex].Y;
					gObj[iMonIndex].StartX = gObj[iMonIndex].X;
					gObj[iMonIndex].StartY = gObj[iMonIndex].Y;
					gObj[iMonIndex].m_bDevilSquareIndex = this->m_iIndex;
					gObj[iMonIndex].DieRegen = FALSE;
					gObj[iMonIndex].m_PosNum = -1;
					gObj[iMonIndex].Live = TRUE;
					gObj[iMonIndex].m_State = 1;
				}
			}
		}
	}
}

BOOL CDevilSquareGround::FindMonsterType(WORD type)
{
	for ( int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareMonsterInfo[i].m_Type == type )
		{
			return TRUE;
		}
	}

	return FALSE;
}

WORD CDevilSquareGround::GetMonsterType(int currtime)
{
	WORD sTemp[MAX_ST_DEVILSQUARE_MONSTER];

	int icount = 0;

	for ( int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareMonsterInfo[i].m_Type != (WORD)-1 )
		{
			if ( currtime >= this->m_DevilSquareMonsterInfo[i].m_RegenStartTime )
			{
				if ( currtime <= this->m_DevilSquareMonsterInfo[i].m_RegenEndTime )
				{
					sTemp[icount] = this->m_DevilSquareMonsterInfo[i].m_Type;
					icount++;
				}
			}
		}
	}

	if ( icount == 0 )
	{
		return (BYTE)-1;
	}

	return sTemp[rand()%icount];
}

void CDevilSquareGround::ClearScore()
{
	this->m_DevilSquareRankList.clear();
}

void CDevilSquareGround::SortScore()
{
	std::sort( this->m_DevilSquareRankList.begin(), this->m_DevilSquareRankList.end(), DevilSquareScoreSort);
}

void CDevilSquareGround::InsertObj(LPOBJ lpObj)
{
	if ( lpObj->Authority != 2 )
	{
		if ( lpObj->Class >= 0 && lpObj->Class < MAX_TYPE_PLAYER-1)
		{
			if ( DS_LEVEL_RANGE(lpObj->m_bDevilSquareIndex) != FALSE )
			{
				lpObj->m_nEventScore +=  this->m_BonusScoreTable[lpObj->Class][lpObj->m_bDevilSquareIndex] / 100;
			}
		}

		this->m_DevilSquareRankList.push_back(lpObj);
	}
}

void CDevilSquareGround::SendScore()
{
	if ( this->m_DevilSquareRankList.size() < 1 )
	{
		return;
	}

	BYTE count = 1;
	int iUserCount = this->m_DevilSquareRankList.size();

	int iExp = 0;

	for ( std::vector<OBJECTSTRUCT *>::iterator Itor = this->m_DevilSquareRankList.begin() ; Itor != this->m_DevilSquareRankList.end() ; Itor++ )
	{
		memcpy(this->m_DevilSquareScoreInfoTOP10.Score[count].Name , (*(Itor))->Name, MAX_ACCOUNT_LEN);
		this->m_DevilSquareScoreInfoTOP10.Score[count].TotalScore = (*(Itor))->m_nEventScore;

		if ( iUserCount <= 6 )
		{
			if ( count < 4 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[count-1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[count-1][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[3][1];
			}
		}
		else
		{
			int lc5 = count*100/iUserCount;

			if ( count == 1 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[0][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[0][1];
			}
			else if ( lc5 <= 30 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[1][1];
			}
			else if ( lc5 <= 50 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[2][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[2][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[3][1];
			}
		}

		count++;

		if ( count >= 10 )
		{
			break;
		}
	}

	this->m_DevilSquareScoreInfoTOP10.Count  = count;
	int iSize = count * 24 + 5;

	PHeadSetB((LPBYTE)&this->m_DevilSquareScoreInfoTOP10, 0x93, iSize);
	count = 1;

	std::vector<OBJECTSTRUCT *>::iterator Itor = this->m_DevilSquareRankList.begin();

	LogAddTD("[DevilSquare] Rank [%d]", this->m_iIndex);

	for ( ; Itor != this->m_DevilSquareRankList.end(); Itor++ )
	{
		int Rank = 0;
		// ----
		if ( iUserCount <= 6 )
		{
			if ( count < 4 ) //webzen fixed on season 3.0
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[count-1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[count-1][1];
				Rank = count-1;
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[3][1];
				Rank = 3;
			}
		}
		else
		{
			int lc7 = count*100/iUserCount;

			if ( count ==1 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[0][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[0][1];
				Rank = 0;
			}
			else if ( lc7 <= 30 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[1][1];
				Rank = 1;
			}
			else if ( lc7 <= 50 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[2][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[2][1];
				Rank = 2;
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[3][1];
				Rank = 3;
			}
		}

#ifdef __CUSTOMS__
		for( int i = 0; i < this->m_Bonus[Rank][2]; i++ )
		{
			ItemSerialCreateSend((*(Itor))->m_Index, (*(Itor))->MapNumber, (*(Itor))->X, (*(Itor))->Y, this->m_Bonus[Rank][3], this->m_Bonus[Rank][4], 0, 0, 0, 0, (*(Itor))->m_Index, 0, 0);
		}
#endif

		if ( g_CrywolfSync.GetOccupationState() == 1 && g_iCrywolfApplyMvpPenalty != FALSE)
		{
			this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp =  (this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
		}

		__int64 exp = this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp;
		CheckItemOptForGetExpEx((*(Itor)), exp, TRUE); //Seal Exp (Season3 add-on)

		(*(Itor))->Experience += (int)exp;		
		
		bool ret = gObjLevelUp((*(Itor)), exp, 0, EVENT_TYPE_DEVILSQUARE);

		(*(Itor))->m_nEventMoney = this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen;

		if ( gObjCheckMaxZen((*(Itor))->m_Index, (*(Itor))->m_nEventMoney) == FALSE )
		{
			int Zen = MAX_ZEN - (*(Itor))->Money;
			(*(Itor))->Money += Zen;
		}
		else
		{
			(*(Itor))->Money += (*(Itor))->m_nEventMoney;
		}

#if defined __REEDLAN__ || __BEREZNUK__
		g_ShopPointEx.AddEventBonus((*(Itor))->m_Index, ShopPointExEvent::DS);
#endif

		GCMoneySend((*(Itor))->m_Index, (*(Itor))->Money);

		if ( ret == true )
		{
			GCKillPlayerExpSend((*(Itor))->m_Index, (WORD)-1, exp, 0, 0);//Season 4.5 changed
		}

		this->SendRankingInfo((*(Itor)));

		LogAddTD("Rank :[%d] : [%s][%s][%d][%d][%d]", count, (*(Itor))->AccountID, (*(Itor))->Name, (*(Itor))->m_nEventMoney, (*(Itor))->m_nEventExp, (*(Itor))->m_nEventScore);
		
		memcpy(this->m_DevilSquareScoreInfoTOP10.Score[0].Name , (*(Itor))->Name, MAX_ACCOUNT_LEN);
		this->m_DevilSquareScoreInfoTOP10.Score[0].TotalScore = (*(Itor))->m_nEventScore;
		this->m_DevilSquareScoreInfoTOP10.MyRank = count;
		count++;

		DataSend((*(Itor))->m_Index, (LPBYTE)&this->m_DevilSquareScoreInfoTOP10 , iSize);
#ifdef WZQUEST
		g_QuestExpProgMng.ChkUserQuestTypeEventMap(258, (*(Itor)), (*(Itor))->m_bDevilSquareIndex, 1);
#endif
		(*(Itor))->m_nEventScore = 0;
		(*(Itor))->m_nEventMoney = 0;
		(*(Itor))->m_nEventExp = 0;
	}
}

void CDevilSquareGround::SendRankingInfo(LPOBJ lpObj)
{
	if ( lpObj->m_nEventScore <= 0 )
	{
		return;
	}

	PMSG_ANS_EVENTUSERSCORE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x01;
	pMsg.h.size = sizeof(pMsg);
	pMsg.Score = lpObj->m_nEventScore;
	pMsg.SquareNum = lpObj->m_bDevilSquareIndex;
	pMsg.Class = lpObj->Class;
	pMsg.ServerCode = gGameServerCode;
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GameID, lpObj->Name, MAX_ACCOUNT_LEN);

	DataSendRank((char *)&pMsg, pMsg.h.size);
}

//004972b0	-> 
int CDevilSquareGround::DelUser(int sUserIndex)	//1.01.00
{
	return 1;	//temp
}