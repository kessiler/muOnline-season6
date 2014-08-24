// TMonsterAIGroup.cpp: implementation of the TMonsterAIGroup class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIGroup.h"

#include "user.h"
#include "Gamemain.h"

#include "..\include\ReadScript.h"
#include "LogProc.h"



BOOL TMonsterAIGroup::s_bDataLoad = FALSE;
TMonsterAIGroupMember TMonsterAIGroup::s_MonsterAIGroupMemberArray[MAX_MONSTER_AI_GROUP][MAX_MONSTER_AI_GROUP_MEMBER];
int TMonsterAIGroup::s_iMonsterAIGroupMemberCount[MAX_MONSTER_AI_GROUP];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIGroup::TMonsterAIGroup()
{
	this->DelAllGroupInfo();
}

TMonsterAIGroup::~TMonsterAIGroup()
{
	return;
}



BOOL TMonsterAIGroup::LoadData(LPSTR lpszFileName)
{
	TMonsterAIGroup::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[Monster AI Group] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			MsgBox("[Monster AI Group] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		int iType = -1;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
				break;

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					int iGroupNumber = -1;
					int iGuid = -1;
					int iClass = -1;
					int iRank = -1;
					int iStartAI = 0;
					int iAI01 = 0;
					int iAI02 = 0;
					int iAI03 = 0;
					int iCreateType = -1;
					int iMapNumber = -1;
					int iStartX = -1;
					int iStartY = -1;
					int iStartDir = -1;
					int iRegenType = -1;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iGroupNumber = TokenNumber;

					Token = GetToken();
					iGuid = TokenNumber;

					Token = GetToken();
					iClass = TokenNumber;

					Token = GetToken();
					iRank = TokenNumber;

					Token = GetToken();
					iStartAI = TokenNumber;

					Token = GetToken();
					iAI01 = TokenNumber;

					Token = GetToken();
					iAI02 = TokenNumber;

					Token = GetToken();
					iAI03 = TokenNumber;

					Token = GetToken();
					iCreateType = TokenNumber;

					Token = GetToken();
					iMapNumber = TokenNumber;

					Token = GetToken();
					iStartX = TokenNumber;

					Token = GetToken();
					iStartY = TokenNumber;

					Token = GetToken();
					iStartDir = TokenNumber;

					Token = GetToken();
					iRegenType = TokenNumber;

					if ( iGroupNumber > MAX_MONSTER_AI_GROUP )
					{
						MsgBox("[Monster AI Group] Exceed Max GroupNumber %d",
							iGroupNumber);
						continue;
					}

					if ( iGuid > MAX_MONSTER_AI_GROUP_MEMBER)
					{
						MsgBox("[Monster AI Group] Exceed Max GroupMember %d, %d ",
							iGroupNumber, iGuid);
						continue;
					}

					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iGroupNumber = iGroupNumber;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iGuid = iGuid;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iClass = iClass;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iRank = iRank;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iStartAI = iStartAI;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iAI01 = iAI01;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iAI02 = iAI02;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iAI03 = iAI03;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iCreateType = iCreateType;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iMapNumber = iMapNumber;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iStartX = iStartX;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iStartY = iStartY;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iStartDir = iStartDir;
					TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iRegenType = iRegenType;
					
					TMonsterAIGroup::s_iMonsterAIGroupMemberCount[iGroupNumber]++;
				}
			}
		}	// while ( true )

		fclose(SMDFile);

		LogAddC(2, "[Monster AI Group ] - %s file is Loaded", lpszFileName);

		TMonsterAIGroup::s_bDataLoad = TRUE;
	}
	catch ( DWORD )
	{
		MsgBox("[Monster AI Group] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}




void TMonsterAIGroup::ChangeAIOrder(int iGroupNumber, int iAIOrder)
{
	if ( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] ChangeAIOrder() Error - (GroupNumber=%d)", iGroupNumber);
		return;
	}

	TMonsterAIGroupMember * lpMemb = NULL;

	for ( int j=0;j<TMonsterAIGroup::s_iMonsterAIGroupMemberCount[iGroupNumber];j++)
	{
		lpMemb = &TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][j];

		if ( lpMemb->m_iGroupNumber == iGroupNumber )
		{
			if ( iAIOrder == 0 )
			{
				gObj[lpMemb->m_iObjIndex].m_iBasicAI = lpMemb->m_iStartAI;
				gObj[lpMemb->m_iObjIndex].m_iCurrentAI = lpMemb->m_iStartAI;
			}
			else if (iAIOrder == 1 )
			{
				gObj[lpMemb->m_iObjIndex].m_iBasicAI = lpMemb->m_iAI01;
				gObj[lpMemb->m_iObjIndex].m_iCurrentAI = lpMemb->m_iAI01;
			}
			else if (iAIOrder == 2 )
			{
				gObj[lpMemb->m_iObjIndex].m_iBasicAI = lpMemb->m_iAI02;
				gObj[lpMemb->m_iObjIndex].m_iCurrentAI = lpMemb->m_iAI02;
			}
			else if (iAIOrder == 3 )
			{
				gObj[lpMemb->m_iObjIndex].m_iBasicAI = lpMemb->m_iAI03;
				gObj[lpMemb->m_iObjIndex].m_iCurrentAI = lpMemb->m_iAI03;
			}
		}		
	}
}




BOOL TMonsterAIGroup::DelGroupInstance(int iGroupNumber)
{
	if ( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] DelGroup() Error - (GroupNumber=%d)", iGroupNumber);
		return FALSE;
	}

	TMonsterAIGroupMember * lpMemb = NULL;

	for ( int j=0;j<TMonsterAIGroup::s_iMonsterAIGroupMemberCount[iGroupNumber];j++)
	{
		lpMemb = &TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][j];

		if ( lpMemb->m_iGroupNumber == iGroupNumber )
		{
			if ( lpMemb->m_iObjIndex != -1 )
			{
				gObjDel(lpMemb->m_iObjIndex);
			}
		}
	}

	return FALSE;
}





BOOL TMonsterAIGroup::DelAllGroupInfo()
{
	for ( int i=0;i<MAX_MONSTER_AI_GROUP;i++)
	{
		for ( int j=0;j<MAX_MONSTER_AI_GROUP_MEMBER;j++)
		{
			TMonsterAIGroup::s_MonsterAIGroupMemberArray[i][j].Reset();
		}

		TMonsterAIGroup::s_iMonsterAIGroupMemberCount[i] = 0;
	}

	TMonsterAIGroup::s_bDataLoad = FALSE;
	return FALSE;
}


int TMonsterAIGroup::FindGroupLeader(int iGroupNumber)
{
	if ( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] FindGroupLeader() Error - (GroupNumber=%d)", iGroupNumber);
		return 0;
	}

	for ( int i=0;i<MAX_MONSTER_AI_GROUP_MEMBER;i++)
	{
		if ( TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iRank == 0 )
		{
			return TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iObjIndex;
		}
	}

	return -1;
}




int TMonsterAIGroup::FindGroupMemberObjectIndex(int iGroupNumber, int iGuid)
{
	if ( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] FindGroupMemberObjectIndex() Error - (GroupNumber=%d Guid=%d)", iGroupNumber, iGuid);
		return 0;
	}

	for ( int i=0;i<MAX_MONSTER_AI_GROUP_MEMBER;i++)
	{
		if ( TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iGuid == iGuid )
		{
			return TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iObjIndex;
		}
	}

	return -1;
}




TMonsterAIGroupMember * TMonsterAIGroup::FindGroupMember(int iGroupNumber, int iGuid)
{
	if ( iGroupNumber < 0 || iGroupNumber >= MAX_MONSTER_AI_GROUP )
	{
		LogAddTD("[Monster AI Group] FindGroupMember() Error - (GroupNumber=%d)(Guid=%d) ", iGroupNumber, iGuid);
		return NULL;
	}

	if ( TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iGroupNumber == iGroupNumber &&
		 TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid].m_iGuid == iGuid )
	{
		return &TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iGuid];
	}

	LogAddTD("[Monster AI Group] FindGroupMember() Error - (GroupNumber=%d)(Guid=%d) ", iGroupNumber, iGuid);

	return NULL;
}



TMonsterAIGroupMember * TMonsterAIGroup::FindGroupMemberToHeal(int iObjIndex, int iGroupNumber, int iGuid, int iDistance)
{
	if ( TMonsterAIGroup::s_iMonsterAIGroupMemberCount[iGroupNumber] == 0 )
		return NULL;

	int iMinHPGuid = -1;
	int iMinHP = 100000000;
	int iCount = TMonsterAIGroup::s_iMonsterAIGroupMemberCount[iGroupNumber];

	for ( int i=0;i<iCount;i++)
	{
		if ( TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iGroupNumber == iGroupNumber )
		{
			LPOBJ lpTargetObj = &gObj[TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iObjIndex];

			if ( lpTargetObj == NULL )
				continue;

			if ( gObjCalDistance(&gObj[iObjIndex], lpTargetObj) > iDistance )
				continue;

			int iMaxLife = lpTargetObj->MaxLife + 1.0f;
			int iPercentHP = lpTargetObj->Life * 100.0f / iMaxLife;

			if ( iPercentHP < 60 )
			{
				if ( lpTargetObj->m_Index == iObjIndex )
					iPercentHP = 1;

				if ( iPercentHP < iMinHP )
				{
					iMinHP = iPercentHP;
					iMinHPGuid = lpTargetObj->m_iGroupMemberGuid;
				}
			}
		}
	}

	if ( iMinHPGuid != -1 )
		return &TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][iMinHPGuid];
	
	return NULL;
}



TMonsterAIGroupMember * TMonsterAIGroup::FindGroupMemberToSommon(int iObjIndex, int iGroupNumber, int iGuid)
{
	if ( TMonsterAIGroup::s_iMonsterAIGroupMemberCount[iGroupNumber] == 0 )
		return NULL;

	int iCount = TMonsterAIGroup::s_iMonsterAIGroupMemberCount[iGroupNumber];

	for ( int i=0;i<iCount;i++)
	{
		if ( TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iGroupNumber == iGroupNumber )
		{
			LPOBJ lpTargetObj = &gObj[TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][i].m_iObjIndex];

			if ( lpTargetObj->m_Index == iObjIndex )
				continue;

			if ( lpTargetObj == NULL )
				continue;

			if ( lpTargetObj->m_iRegenType == -1 )
				continue;

			if ( lpTargetObj->Live == FALSE )
			{
				return &TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][i];
			}
		}
	}

	return NULL;
}



void TMonsterAIGroup::InitAll()
{
	for ( int i=0;i<MAX_MONSTER_AI_GROUP;i++)
	{
		if ( TMonsterAIGroup::s_iMonsterAIGroupMemberCount[i] == 0 )
			continue;

		TMonsterAIGroup::Init(i);
	}
}



void TMonsterAIGroup::Init(int iGroupNumber)
{
	if ( TMonsterAIGroup::s_iMonsterAIGroupMemberCount[iGroupNumber] == 0 )
		return;

	TMonsterAIGroup::DelGroupInstance(iGroupNumber);

	for ( int j=0;j<MAX_MONSTER_AI_GROUP_MEMBER;j++)
	{
		TMonsterAIGroupMember & Memb = TMonsterAIGroup::s_MonsterAIGroupMemberArray[iGroupNumber][j];

		if ( Memb.m_iGuid == -1 )
			continue;

		int iResult = gObjAddMonster(Memb.m_iMapNumber);

		if ( iResult >= 0 )
		{
			gObj[iResult].m_PosNum = -1;
			gObj[iResult].MapNumber = Memb.m_iMapNumber;
			gObj[iResult].Live = TRUE;

			gObjViewportListProtocolDestroy(&gObj[iResult]);
			gObjViewportClose(&gObj[iResult]);

			BYTE cX;
			BYTE cY;

			if ( Memb.m_iCreateType == 1 )
			{
				int iRadius = 10;
				BOOL bGetPosition = FALSE;
				int iCount = 100;

				while ( iCount-- != 0 )
				{
					cX = ( rand() % (iRadius+1) ) * (((rand()%2==0)?-1:1)) + Memb.m_iStartX;
					cY = ( rand() % (iRadius+1) ) * (((rand()%2==0)?-1:1)) + Memb.m_iStartX;

					BYTE btMapAttr = MapC[Memb.m_iMapNumber].GetAttr(cX, cY);

					if ( btMapAttr == 0 )
					{
						bGetPosition = TRUE;
						break;
					}
				}

				if ( bGetPosition == FALSE )
				{
					gObj[iResult].Live = FALSE;
					gObj[iResult].m_State = 4;
					gObj[iResult].RegenTime = GetTickCount();
					gObj[iResult].DieRegen = 1;

					return;
				}
			}
			else if ( Memb.m_iCreateType == 0 )
			{
				cX = Memb.m_iStartX;
				cY = Memb.m_iStartY;
			}

			gObj[iResult].X = cX;
			gObj[iResult].Y = cY;
			gObj[iResult].MTX = gObj[iResult].X;
			gObj[iResult].MTY = gObj[iResult].Y;
			gObj[iResult].TX = gObj[iResult].X;
			gObj[iResult].TY = gObj[iResult].Y;
			gObj[iResult].StartX = gObj[iResult].X;
			gObj[iResult].StartY = gObj[iResult].Y;


			gObjSetMonster(iResult, Memb.m_iClass);

			gObj[iResult].m_iGroupNumber = Memb.m_iGroupNumber;
			gObj[iResult].m_iGroupMemberGuid = Memb.m_iGuid;
			gObj[iResult].m_iCurrentAI = Memb.m_iStartAI;
			gObj[iResult].m_iBasicAI = Memb.m_iStartAI;
			gObj[iResult].m_iRegenType = Memb.m_iRegenType;
			gObj[iResult].Dir = Memb.m_iStartDir;
			gObj[iResult].m_State = 1;
			gObj[iResult].DieRegen = 0;
			Memb.m_iObjIndex = iResult;

			if ( Memb.m_iCreateType == -1 )
			{
				gObj[iResult].Live = FALSE;
				gObj[iResult].m_State = 4;
				gObj[iResult].RegenTime = GetTickCount();
				gObj[iResult].DieRegen = 1;

				continue;
			}
			
//#if(_GSCS==0)
			LogAddTD("[ KANTURU ][ SetAIMonster ] %s(Index:%d ObjIndex:%d) Map:%d-[%d][%d]",
				gObj[iResult].Name, gObj[iResult].Class, iResult, gObj[iResult].MapNumber,
				gObj[iResult].X, gObj[iResult].Y);
//#endif
		}
	}
}
