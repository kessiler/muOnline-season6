// KanturuMonsterMng.cpp: implementation of the CKanturuMonsterMng class.
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KanturuMonsterMng.h"
#include "Gamemain.h"
#include "LogProc.h"
#include "..\include\ReadScript.h"


CKanturuMonsterMng g_KanturuMonsterMng;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKanturuMonsterMng::CKanturuMonsterMng()
{
	this->ResetLoadData();
	this->ResetRegenMonsterObjData();
}

CKanturuMonsterMng::~CKanturuMonsterMng()
{
	return;
}


void CKanturuMonsterMng::ResetLoadData()
{
	for ( int iCount=0;iCount<MAX_KANTURU_MONSTER;iCount++)
	{
		memset(&this->m_SetBaseInfo[iCount], -1, sizeof(this->m_SetBaseInfo[iCount]));
	}

	this->m_iMaxMonsterCount = 0;
}



void CKanturuMonsterMng::ResetRegenMonsterObjData()
{
	for ( int iCount=0;iCount<this->m_KanturuMonster.GetCount();iCount++)
	{
		if ( this->m_KanturuMonster.m_iObjIndex[iCount] != -1 )
		{
			gObjDel(this->m_KanturuMonster.m_iObjIndex[iCount]);
		}
	}

	if ( this->m_iMayaObjIndex != -1 )
	{
		gObjDel(this->m_iMayaObjIndex);
	}

	this->m_KanturuMonster.Reset();
	this->m_iMaxRegenMonsterCount = 0;
	this->m_iAliveMonsterCount = 0;
	this->m_iMayaObjIndex = -1;
}



void CKanturuMonsterMng::MonsterDie(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];

	for ( int iCount=0;iCount<this->m_KanturuMonster.GetCount();iCount++)
	{
		if ( this->m_KanturuMonster.m_iObjIndex[iCount] == iIndex )
		{
			this->m_iAliveMonsterCount--;
			return;
		}
	}
}


BOOL CKanturuMonsterMng::LoadData(LPSTR lpszFileName)
{
	this->m_bFileDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName , "") )
	{
		MsgBox("[ KANTURU ][ MonsterSetBase ] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			DWORD dwError = GetLastError();
			MsgBox("[ KANTURU ][ MonsterSetBase ] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		this->ResetLoadData();
	
		enum SMDToken Token;
		int iType = -1;
		BYTE btGroup = 0;
		WORD wType = 0;
		BYTE btMapNumber = 0;
		BYTE btDistance = 0;
		BYTE btX = 0;
		BYTE btY = 0;
		BYTE btDir = 0;

		while ( true )
		{
			Token = GetToken();
			
			if ( Token == END )
			{
				break;
			}

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					btGroup = 0;
					wType = 0;
					btMapNumber = 0;
					btDistance = 0;
					btX = 0;
					btY = 0;
					btDir = 0;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}

					btGroup = TokenNumber;

					Token = GetToken();
					wType = TokenNumber;

					Token = GetToken();
					btMapNumber = TokenNumber;

					Token = GetToken();
					btDistance = TokenNumber;

					Token = GetToken();
					btX = TokenNumber;

					Token = GetToken();
					btY = TokenNumber;

					Token = GetToken();
					btDir = TokenNumber;

					if ( this->m_iMaxMonsterCount < 0 ||
						 this->m_iMaxMonsterCount >= MAX_KANTURU_MONSTER )
					{
						MsgBox("[ KANTURU ][ MonsterSetBase ] - Exceed Max Info Count (%d)",
							this->m_iMaxMonsterCount);

						break;
					}

					this->SetMonsterSetBaseInfo(this->m_iMaxMonsterCount, btGroup, wType,
						btMapNumber, btDistance, btX, btY, btDir);
					this->m_iMaxMonsterCount++;
				}
				else
				{
					Token = GetToken();

					if ( !strcmp("end", TokenString))
					{
						break;
					}
				}
			}	// while ( true )
		}	// while ( true )

		fclose(SMDFile);
		LogAddC(2, "[ KANTURU ][ MonsterSetBase ] - %s file is Loaded",
			lpszFileName);

		this->m_bFileDataLoad = TRUE;
	}	// __try
	catch ( DWORD)
	{
		MsgBox("[ KANTURU ][ MonsterSetBase ] Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_bFileDataLoad;
}



void CKanturuMonsterMng::SetMonsterSetBaseInfo(int iIndex, BYTE btGroup, WORD wType, BYTE btMapNumber, BYTE btDistance, BYTE btX, BYTE btY, BYTE btDir)
{
	this->m_SetBaseInfo[iIndex].btGroup = btGroup;
	this->m_SetBaseInfo[iIndex].wType = wType;
	this->m_SetBaseInfo[iIndex].btMapNumber = btMapNumber;
	this->m_SetBaseInfo[iIndex].btDistance = btDistance;
	this->m_SetBaseInfo[iIndex].btX = btX;
	this->m_SetBaseInfo[iIndex].btY = btY;
	this->m_SetBaseInfo[iIndex].btDir = btDir;
}



int CKanturuMonsterMng::SetKanturuMonster(int iGroupNumber)
{
	int iMapNumber = 0;
	WORD wType = 0;
	LPOBJ lpObj;
	int iResult;
	int iRegenCount = 0;
	
	for ( int iCount=0;iCount<MAX_KANTURU_MONSTER;iCount++)
	{
		if ( this->m_SetBaseInfo[iCount].btGroup == iGroupNumber &&
			 this->m_SetBaseInfo[iCount].wType != 0 )
		{
			iResult  = gObjAddMonster(this->m_SetBaseInfo[iCount].btMapNumber);

			if ( iResult < 0 )
			{
				LogAddTD("[ KANTURU ][ SetBossMapMonster ] Fail - Type:%d Map[%d]-[%d][%d]",
					this->m_SetBaseInfo[iCount].wType,
					this->m_SetBaseInfo[iCount].btMapNumber,
					this->m_SetBaseInfo[iCount].btX,
					this->m_SetBaseInfo[iCount].btY);
			}
			else
			{
				lpObj = &gObj[iResult];

				lpObj->m_PosNum = -1;
				lpObj->X = this->m_SetBaseInfo[iCount].btX;
				lpObj->Y = this->m_SetBaseInfo[iCount].btY;
				lpObj->MapNumber = this->m_SetBaseInfo[iCount].btMapNumber;

				if ( this->GetPosition(iCount, lpObj->MapNumber, lpObj->X, lpObj->Y) == FALSE )
				{
					gObjDel(iResult);
					continue;
				}

				lpObj->TX = lpObj->X;
				lpObj->TY = lpObj->Y;
				lpObj->m_OldX = lpObj->X;
				lpObj->m_OldY = lpObj->Y;
				lpObj->StartX = lpObj->X;
				lpObj->StartY = lpObj->Y;
				lpObj->m_MoveRange = this->m_SetBaseInfo[iCount].btDistance;
				lpObj->DieRegen = FALSE;

				if ( this->m_SetBaseInfo[iCount].btDir == 0xFF )
				{
					lpObj->Dir = rand()%8;
				}
				else
				{
					lpObj->Dir = this->m_SetBaseInfo[iCount].btDir;
				}

				if ( gObjSetMonster(iResult, this->m_SetBaseInfo[iCount].wType) == FALSE )
				{
					gObjDel(iResult);
					continue;
				}

				if ( this->m_SetBaseInfo[iCount].wType == 364 )	/// Maya Hand
				{
					this->m_iMayaObjIndex = iResult;

					LogAddTD("[ KANTURU ][ SetBossMapMonster ] %s(Index:%d / ObjIndex:%d) Map:%d-[%d][%d]",
						gObj[iResult].Name, gObj[iResult].Class, iResult, gObj[iResult].MapNumber,
						gObj[iResult].X, gObj[iResult].Y);

					continue;
				}

				if ( gObj[iResult].Type == OBJ_MONSTER )
				{
					this->m_KanturuMonster.AddObj(iResult);
					this->m_iMaxRegenMonsterCount++;
					this->m_iAliveMonsterCount++;

					LogAddTD("[ KANTURU ][ SetBossMapMonster ] Count:%d %s(Index:%d / ObjIndex:%d) Map:%d-[%d][%d]",
						this->m_iAliveMonsterCount, gObj[iResult].Name, gObj[iResult].Class, iResult,
						gObj[iResult].MapNumber,gObj[iResult].X, gObj[iResult].Y);
				}

				iRegenCount++;
			}
		}
	}

	return iRegenCount;
}



BOOL CKanturuMonsterMng::GetPosition(int TableNum, short MapNumber, short & x, short & y)
{
	if ( TableNum < 0 || TableNum > MAX_KANTURU_MONSTER-1 )
	{
		LogAdd("ERROR : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	int iOrx = this->m_SetBaseInfo[TableNum].btX;
	int iOry = this->m_SetBaseInfo[TableNum].btY;
	BYTE btAttr = MapC[MapNumber].GetAttr(iOrx, iOry);

	if ( (btAttr&1)!=1 && (btAttr&4)!=4 && (btAttr&8)!=8 )
	{
		x = iOrx;
		y = iOry;

		return TRUE;
	}

	int iCount = 100;	// Number of Tries

	while ( iCount-- != 0 )
	{
		iOrx = this->m_SetBaseInfo[TableNum].btX - 3;
		iOry = this->m_SetBaseInfo[TableNum].btY - 3;
		iOrx += rand() % 7;
		iOry += rand() % 7;

		btAttr = MapC[MapNumber].GetAttr(iOrx, iOry);

		if ( (btAttr&1)!=1 && (btAttr&4)!=4 && (btAttr&8)!=8 )
		{
			x = iOrx;
			y = iOry;

			return TRUE;
		}
	}

	return FALSE;
}



BOOL CKanturuMonsterMng::IsExistAliveMonster()
{
	if ( this->m_iAliveMonsterCount <=  0 )
	{
		return FALSE;
	}

	return TRUE;
}