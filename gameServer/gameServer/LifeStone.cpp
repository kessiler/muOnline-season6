// LifeStone.cpp: implementation of the CLifeStone class.
//	GS-CS		1.00.90		0xXXXXXXXX  - complete
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LifeStone.h"
#include "user.h"
#include "logproc.h"
#include "GameMain.h"
#include "CastleSiegeSync.h"
#include "Protocol.h"


//#if (_GSCS==1)
CLifeStone g_CsNPC_LifeStone;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLifeStone::CLifeStone()
{

}

CLifeStone::~CLifeStone()
{

}

int CLifeStone::CreateLifeStone(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	int iMonsterIndex = -1;
	BYTE cX = lpObj->X;
	BYTE cY = lpObj->Y;

	if ( g_CastleSiegeSync.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE )
	{
		MsgOutput(iIndex, lMsg.Get(1645));
		return FALSE;
	}

	if ( lpObj->GuildStatus != G_MASTER )
	{
		return FALSE;
	}

	if ( lpObj->m_btCsJoinSide < 2 )
	{
		MsgOutput(iIndex, lMsg.Get(1641)); //HermeX Fix
		return FALSE;
	}

	if ( lpObj->lpGuild->lpLifeStone  ) //HermeX Fix
	{
		MsgOutput(iIndex, lMsg.Get(1642)); //HermeX Fix
		return FALSE;
	}
	
	if( cX > 150 && cX < 210 && cY > 175 && cY < 230 )
	{
		MsgOutput(iIndex, lMsg.Get(1644)); //HermeX Fix
		return FALSE;
	}

	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cX, cY);
	
	if ( btMapAttr & 16 != 16 ) //siege camp?
	{
		//return FALSE;
	}

	if ( gObj[iIndex].MapNumber != MAP_INDEX_CASTLESIEGE )
	{
		MsgOutput(iIndex, lMsg.Get(1627)); //HermeX Fix
		return FALSE;
	}
	
	iMonsterIndex = gObjAddMonster(lpObj->MapNumber);

	if ( iMonsterIndex >= 0 )
	{
	
		MONSTER_ATTRIBUTE * MAttr = gMAttr.GetAttr(278);
	
		if ( MAttr == NULL )
		{
			gObjDel(iMonsterIndex);
			return FALSE;
		}

		gObjSetMonster(iMonsterIndex, 278);
		gObj[iMonsterIndex].Live = TRUE;
		gObj[iMonsterIndex].Life = MAttr->m_Hp;
		gObj[iMonsterIndex].MaxLife = MAttr->m_Hp;
		gObj[iMonsterIndex].m_PosNum = -1;
		gObj[iMonsterIndex].X = cX;
		gObj[iMonsterIndex].Y = cY;
		gObj[iMonsterIndex].MTX = cX;
		gObj[iMonsterIndex].MTY = cY;
		gObj[iMonsterIndex].TX = cX;
		gObj[iMonsterIndex].TY = cY;
		gObj[iMonsterIndex].m_OldX = cX;
		gObj[iMonsterIndex].m_OldY = cY;
		gObj[iMonsterIndex].StartX = cX;
		gObj[iMonsterIndex].StartY = cY;
		gObj[iMonsterIndex].MapNumber = lpObj->MapNumber;
		gObj[iMonsterIndex].m_MoveRange = 0;
		gObj[iMonsterIndex].Level = MAttr->m_Level;
		gObj[iMonsterIndex].Type = OBJ_MONSTER;
		gObj[iMonsterIndex].MaxRegenTime = 1000;
		gObj[iMonsterIndex].Dir = 1;
		gObj[iMonsterIndex].RegenTime = GetTickCount();
		gObj[iMonsterIndex].m_Attribute = 0;
		gObj[iMonsterIndex].DieRegen = 0;
		gObj[iMonsterIndex].m_btCsNpcType = OBJ_NPC; //HermeX Fix
		gObj[iMonsterIndex].m_btCsJoinSide = lpObj->m_btCsJoinSide;
		gObj[iMonsterIndex].lpGuild = lpObj->lpGuild;
		gObj[iMonsterIndex].m_btCreationState = 0;

		lpObj->lpGuild->lpLifeStone = &gObj[iMonsterIndex]; //SirMaster Fix
		gObj[iMonsterIndex].m_iCreatedActivationTime = 0; //new

		MsgOutput(iIndex, lMsg.Get(1646));
		LogAddTD("[CastleSiege] LifeStone is created - [%s] [%s][%s] (Map:%d)(X:%d, Y:%d)",lpObj->lpGuild->Name,
		lpObj->AccountID,lpObj->Name,lpObj->MapNumber,cX,cY);
		lpObj->m_btLifeStoneCount++;
	}
	else
	{
		MsgOutput(iIndex, lMsg.Get(1647));
		return FALSE;
	}
	return TRUE;
}

int CLifeStone::DeleteLifeStone(int iIndex) //Identical
{

	if(iIndex < 0 || iIndex > OBJMAX - 1)
	{
		return FALSE;
	}

	LPOBJ lpLifeStone = &gObj[iIndex];

	if(lpLifeStone->lpGuild)
	{
		LogAddTD("[CastleSiege] LifeStone is broken - [%s]",lpLifeStone->lpGuild->Name);
		lpLifeStone->lpGuild->lpLifeStone = 0;
	}
	return TRUE;
}

int CLifeStone::SetReSpawnUserXY(int iUserIndex) //Identical
{
	
	if(!gObjIsConnected(iUserIndex))
	{
		return FALSE;
	}

	LPOBJ lpUser = &gObj[iUserIndex];

	if(lpUser->MapNumber != MAP_INDEX_CASTLESIEGE)
	{
		return FALSE;
	}

	if(!lpUser->lpGuild) //HermeX Fix
	{
		return FALSE;
	}

	BYTE btCsJoinSide = lpUser->m_btCsJoinSide;

	if(lpUser->lpGuild->lpLifeStone == 0)
	{
		return FALSE;
	}

	LPOBJ lpLifeStone = lpUser->lpGuild->lpLifeStone;

	if(lpLifeStone->m_iCreatedActivationTime < 60)//NEW
	{
		return FALSE;
	}

	lpUser->MapNumber = MAP_INDEX_CASTLESIEGE;
	lpUser->X = lpLifeStone->X;
	lpUser->Y = lpLifeStone->Y;
	return TRUE;
}

void CLifeStone::LifeStoneAct(int iIndex) //Identical
{
	if(!gObjIsConnected(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	lpObj->m_iCreatedActivationTime++;
	BYTE btCreationState = lpObj->m_btCreationState; //HermeX Fix

	if(lpObj->m_iCreatedActivationTime < 60 )
	{
		lpObj->m_btCreationState = (lpObj->m_iCreatedActivationTime / 12);
	}
	else //HermeX Fix
	{
		lpObj->m_btCreationState = 5;
	}

	if(btCreationState != lpObj->m_btCreationState) //HermeX Fix
	{
		::GCSendObjectCreationState(iIndex);
	}

	if(lpObj->m_btCreationState < 5)
	{
		return;
	}

	if(lpObj->VPCount < 1)
	{
		return;
	}

		int tObjNum = -1;

		for(int i = 0; i < MAX_VIEWPORT; i++) //HermeX Fix
		{
			BOOL iRecoverLife,iRecoverMana,iRecoverBP;

			tObjNum = lpObj->VpPlayer[i].number;
				
			if(tObjNum >= 0)
			{
				if(gObj[tObjNum].Type == OBJ_USER)
				{
					if(gObj[tObjNum].Live != FALSE)
					{
						if(gObj[tObjNum].m_btCsJoinSide == lpObj->m_btCsJoinSide)
						{
							if(abs(lpObj->Y - gObj[tObjNum].Y) <= 3)
							{
								if(abs(lpObj->X - gObj[tObjNum].X) <= 3) //fix
								{
									iRecoverLife = FALSE;
									iRecoverMana = FALSE;
									iRecoverBP = FALSE;

									if(gObj[tObjNum].Life < (gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife)) //SirMaster Fix
									{
										gObj[tObjNum].Life += ((gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife) / 100 ); //SirMaster Fix
										
										if(gObj[tObjNum].Life > (gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife)) //SirMaster Fix
										{
											gObj[tObjNum].Life = (gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife); //SirMaster Fix
										}
										iRecoverLife = TRUE;
									}

									if(gObj[tObjNum].Mana < (gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana) ) //SirMaster Fix
									{
										gObj[tObjNum].Mana += ((gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana) / 100 ); //SirMaster Fix
	
										if(gObj[tObjNum].Mana > (gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana)) //SirMaster Fix
										{
											gObj[tObjNum].Mana = (gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana); //SirMaster Fix
										}
										iRecoverMana = TRUE;
									}

									if(gObj[tObjNum].BP < (gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP))
									{
										gObj[tObjNum].BP += ((gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP) / 100);

										if(gObj[tObjNum].BP > (gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP))
										{
											gObj[tObjNum].BP = (gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP);
										}
										iRecoverBP = TRUE;
									}

									if(iRecoverLife != FALSE)
									{
										::GCReFillSend(tObjNum,gObj[tObjNum].Life,0xFF,1,gObj[tObjNum].iShield);
									}

									if( (iRecoverMana != FALSE) || (iRecoverBP != FALSE) )
									{
										::GCManaSend(tObjNum,gObj[tObjNum].Mana,0xFF,0,gObj[tObjNum].BP);
									}
								}
							}
						}
					}
				}
			}
			continue;
		}
}

//#endif