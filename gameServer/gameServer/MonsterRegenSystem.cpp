#include "StdAfx.h"
#include "MonsterRegenSystem.h"
#include "..\include\ReadScript.h"
#include "logproc.h"
#include "user.h"
#include "GameMain.h"
#include "TNotice.h"

// Constructor / Destructor

CMonsterRegenSystem g_MonsterRegenSystem;

CMonsterRegenSystem::CMonsterRegenSystem()
{

}

CMonsterRegenSystem::~CMonsterRegenSystem()
{

}

//006be500
void CMonsterRegenSystem::LoadScript(const char* lpFileName)	//OK
{
	SMDToken Token;
	int nType;

	this->m_bLoaded = FALSE;

	if( lpFileName == 0 )	{
		LogAdd("[MonsterGroupRegen] lpFileName is null!!");
		return;
	}

	SMDFile = fopen(lpFileName,"r");	//ok

	if( SMDFile == NULL )	{
		LogAdd(lMsg.Get(MSGGET(1,197)),lpFileName);
		return;
	}

	while( TRUE )
	{
		Token = GetToken();

		if( Token == END )
			break;

		if( Token == NUMBER )
		{
			nType = TokenNumber;

			if( nType == 0 )	{
				while( TRUE )
				{
					Token = GetToken();

					if( Token == NAME && !strcmp("end",TokenString) )
						break;

					int nGourpNumber = TokenNumber;

					if( nGourpNumber > 255 || nGourpNumber < 0 )	{
						LogAdd("[MonsterGroupRegen] Invalid monster groupNumber");
						return;
					}

					m_MonsterGroupInfo[nGourpNumber].iGroupNumber = nGourpNumber;

					Token = GetToken();
					m_MonsterGroupInfo[nGourpNumber].iMapNumber = TokenNumber;

					Token = GetToken();
					m_MonsterGroupInfo[nGourpNumber].tmpUnk = TokenNumber;

					Token = GetToken();
					m_MonsterGroupInfo[nGourpNumber].iBossMonsterClass = TokenNumber;

					Token = GetToken();
					m_MonsterGroupInfo[nGourpNumber].aType = TokenNumber;

					m_MonsterGroupInfo[nGourpNumber].dwRegenTick = m_MonsterGroupInfo[nGourpNumber].tmpUnk+GetTickCount();
				}
			}
			else if( nType == 1 )	{
				int nAreaCnt = 0;

				while( TRUE )
				{
					Token = GetToken();

					if( Token == NAME &&  !strcmp("end",TokenString) )
						break;

					int nGourpNumber = TokenNumber;

					if( nGourpNumber > 255 || nGourpNumber < 0 )	{
						LogAdd("[MonsterGroupRegen] Invalid monster groupNumber");
						return;
					}

					this->m_MonsterPosition[nGourpNumber].iGroupNumber = nGourpNumber;

					Token = GetToken();
					this->m_MonsterPosition[nGourpNumber].m_RegenArea[nAreaCnt].sx = TokenNumber;

					Token = GetToken();
					this->m_MonsterPosition[nGourpNumber].m_RegenArea[nAreaCnt].sy = TokenNumber;

					Token = GetToken();
					this->m_MonsterPosition[nGourpNumber].m_RegenArea[nAreaCnt].ex = TokenNumber;

					Token = GetToken();
					this->m_MonsterPosition[nGourpNumber].m_RegenArea[nAreaCnt].ey = TokenNumber;
					nAreaCnt++;

					this->m_MonsterPosition[nGourpNumber].iAreaCount = nAreaCnt;
				}
			}
			else if( nType == 2 )	{
				int nArrayIndex = 0;
				while( TRUE )
				{
					Token = GetToken();

					if( Token == NAME && !strcmp("end",TokenString) )
						break;

					int nGroupNumber = TokenNumber;

					if( nGroupNumber > 255 || nGroupNumber < 0 )	{
						LogAdd("[MonsterGroupRegen] Invalid monster groupNumber");
						return;
					}

					if( this->m_MonsterPosition[nGroupNumber].iGroupNumber < 0 )	{
						LogAdd("[MonsterGroupRegen] Invalid groupNumber(%d) ",nGroupNumber);
						return;
					}

					Token = GetToken();
					this->m_MonsterPosition[nGroupNumber].m_MonsterIndexInf[nArrayIndex].iIndex = TokenNumber;

					Token = GetToken();
					this->m_MonsterPosition[nGroupNumber].m_MonsterIndexInf[nArrayIndex].iCount = TokenNumber;
					nArrayIndex++;

					if( nArrayIndex >= 20 )	{
						LogAdd("[MonsterGroupRegen] Out of Max MonsterKindIndex ");
						break;
					}
				}
			}
			else if( nType == 3 )	{
				int nTimeArrayIndex = 0;
				while( TRUE )
				{
					Token = GetToken();

					if( Token == NAME && !strcmp("end",TokenString) )
						break;

					int nGroupNumber = TokenNumber;

					if( nGroupNumber > 255 || nGroupNumber < 0 )	{
						LogAdd("[MonsterGroupRegen] Invalid monster groupNumber");
						return;
					}

					if( this->m_MonsterPosition[nGroupNumber].iGroupNumber < 0 )	{
						LogAdd("[MonsterGroupRegen] Invalid groupNumber(%d) ",nGroupNumber);
						return;
					}

					Token = GetToken();
					this->m_MonsterGroupInfo[nGroupNumber].m_RegenTimeTable[nTimeArrayIndex].nBeginHour = (int)TokenNumber;

					Token = GetToken();
					this->m_MonsterGroupInfo[nGroupNumber].m_RegenTimeTable[nTimeArrayIndex].nEndHour = (int)TokenNumber;
					nTimeArrayIndex++;
					if( nTimeArrayIndex >= 24 )
					{
						LogAdd("[MonsterGroupRegen] Out of Max TimeTable ");
						break;
					}
				}
			}
		}
	}

	fclose(SMDFile);
	LogAdd(lMsg.Get(MSGGET(1,198)),lpFileName);
	this->m_bLoaded = TRUE;
}

//006bf0f0
void CMonsterRegenSystem::Run()	//~OK
{
	if( !m_bLoaded )
		return;

	DWORD dwTick = GetTickCount();
	CTime tCurrentTime = CTime::GetTickCount();

	if( this->m_CurDay != tCurrentTime.GetDay() )	{
		this->m_CurDay = tCurrentTime.GetDay();
		SetRegenTimeTable();
	}

	int i;

	for( i = 0; i < MAX_MONSTER_REGEN; i++)	{
		if( this->m_MonsterGroupInfo[i].iGroupNumber > -1 )	{
			if( this->m_MonsterGroupInfo[i].aType == 1 )
			{
				if( !IsLiveBossState(i) )
				{
					if( dwTick >= this->m_MonsterGroupInfo[i].dwRegenTick )
					{
						this->DeleteMonster(i);
						this->RegenMonster(i);

						this->m_MonsterGroupInfo[i].dwRegenTick = m_MonsterGroupInfo[i].tmpUnk+GetTickCount();
					}
				}
				else
					this->m_MonsterGroupInfo[i].dwRegenTick = m_MonsterGroupInfo[i].tmpUnk+GetTickCount();
			}
			else if( this->m_MonsterGroupInfo[i].aType == 2 )
			{
				CTime tCurrentTime = CTime::GetTickCount();
				int nCurHour = tCurrentTime.GetHour();
				int nCurMin = tCurrentTime.GetMinute();

				if( IsRegenTime(i,nCurHour,nCurMin,true) )
				{
					int nBossMonsterIndex = this->m_MonsterGroupInfo[i].iBossMonsterIndex;

					if( OBJMAX_RANGE(nBossMonsterIndex) )
					{
						if( !gObj[nBossMonsterIndex].m_ActState.Emotion )
						{
							this->DeleteMonster(i);
							this->RegenMonster(i);
						}
						else
						{
							LogAddTD("[MonsterGroupRegen]skip regen [GROUP]:%d, [INDEX]:%d",i,nBossMonsterIndex);
						}
					}
					else
					{
						this->DeleteMonster(i);
						this->RegenMonster(i);
					}
				}
			}
		}
		else
			break;
	}
}

//006bf410
void CMonsterRegenSystem::RegenMonster(int nGroupNumber)	//OK
{
	if( !MONSTER_REGEN_RANGE(nGroupNumber) )	{
		LogAddTD("");	// uhm ... webzen forgot the log ? ...
		return;
	}

	int nRegenArea = rand()%this->m_MonsterPosition[nGroupNumber].iAreaCount;

	for(int i = 0; i < MAX_MONSTER_REGEN; i++)
	{
		int nMapNumber = this->m_MonsterGroupInfo[i].iMapNumber;
		int nMonsterArrayIndex = 0;

		for(int k = 0; k < MAX_MONSTER_INDEXINFO; k++)
		{
			int nMonsterIndex = this->m_MonsterPosition[i].m_MonsterIndexInf[k].iIndex;
			int nMonsterCount = this->m_MonsterPosition[i].m_MonsterIndexInf[k].iCount;
			int nBeginX = this->m_MonsterPosition[i].m_RegenArea[nRegenArea].sx;
			int nBeginY = this->m_MonsterPosition[i].m_RegenArea[nRegenArea].sy;
			int nEndX = this->m_MonsterPosition[i].m_RegenArea[nRegenArea].ex;
			int nEndY = this->m_MonsterPosition[i].m_RegenArea[nRegenArea].ey;

			for(int n = 0; n < nMonsterCount; n++)
			{
				if( nMapNumber == -1 )
					break;

				int nResult = gObjAddMonster(nMapNumber);
				this->m_MonsterGroupInfo[i].m_MonsterAccountNumInfo[nMonsterArrayIndex].index = nResult;

				if( nResult >= 0 )
				{
					if( !SetPosMonster(nResult,nMapNumber,nBeginX,nBeginY,nEndX,nEndY) )	{
						gObjDel(nResult);
						LogAdd("error : %s %d",__FILE__,__LINE__);
						continue;
					}

					if( !gObjSetMonster(nResult,nMonsterIndex) )	{
						gObjDel(nResult);
						LogAdd("error : %s %d",__FILE__,__LINE__);
						continue;
					}
				}
				else{
					LogAddTD("[MonsterGroupRegen] AddMonster fail!! [GROUP]:%d, [INDEX]:%d",nGroupNumber,i);
					continue;
				}

				nMonsterArrayIndex++;

				if( gObj[nResult].Class == this->m_MonsterGroupInfo[nGroupNumber].iBossMonsterClass )
					this->m_MonsterGroupInfo[nGroupNumber].iBossMonsterIndex = nResult;

				LogAddTD("[MonsterGroupRegen] Group: %d, MonsterIndex: %d, MapNumber: %d, X: %d, Y: %d ",
					i,nMonsterIndex,nMapNumber,gObj[nResult].X,gObj[nResult].Y);
			}
		}
	}
}

//006bf820
int CMonsterRegenSystem::SetPosMonster(int aIndex,int nMapNumber,int nBeginX,int nBeginY,int nEndX,int nEndY)//OK
{
	if( !OBJMAX_RANGE(aIndex) )	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];
	lpObj->m_PosNum = -1;
	lpObj->MapNumber = nMapNumber;

	GetBoxPosition(nMapNumber,nBeginX,nBeginY,nEndX,nEndY,lpObj->X,lpObj->Y);

	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = rand()%8;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;
	return true;
}

//006bf9c0
int CMonsterRegenSystem::GetBoxPosition(int mapnumber,int ax,int ay,int aw,int ah,short & mx,short & my)	// strange attr check ...
{
	int count = 100 ; 
	int w;
	int h;
	int tx; 
	int ty;
	BYTE attr;

	while ( count-- != 0)
	{
		w = aw - ax;
		h = ah - ay;

		__try
		{
			tx = ax + (rand()%w);
			ty = ay + (rand()%h);
		}
		__except (w=1,h=1,1)
		{

		}

		attr = MapC[mapnumber].GetAttr(tx, ty);

		if((attr&1) != 1)
		{
			mx = tx;
			my = ty;
			return true;
		}
		//Tested
	}

	return false;
}

//006bfb00
bool CMonsterRegenSystem::IsLiveBossState(int nGroupNumber)	//OK
{
	if( !MONSTER_REGEN_RANGE(nGroupNumber) )
		return false;

	int nBossMonsterIndex = this->m_MonsterGroupInfo[nGroupNumber].iBossMonsterIndex;

	if( !OBJMAX_RANGE(nBossMonsterIndex) )
		return false;

	if( gObj[nBossMonsterIndex].Live == 1 )
		return true;

	return false;
}

//006bfbc0
void CMonsterRegenSystem::DeleteMonster(int nGroupNumber)	//OK
{
	if( !MONSTER_REGEN_RANGE(nGroupNumber) )
		return;

	for(int i = 0; i < MAX_MONSTER_REGEN; i++)	{
		if( this->m_MonsterGroupInfo[nGroupNumber].m_MonsterAccountNumInfo[i].index > -1 )	{
			gObjDel(this->m_MonsterGroupInfo[nGroupNumber].m_MonsterAccountNumInfo[i].index);
			LogAdd("[MonsterGroupRegen] Delete Monster - %d ",this->m_MonsterGroupInfo[nGroupNumber].m_MonsterAccountNumInfo[i].index);
			this->m_MonsterGroupInfo[nGroupNumber].m_MonsterAccountNumInfo[i].index = -1;
		}
		else
			break;
	}
}

//006bfd00
void CMonsterRegenSystem::SendAllUserAnyMsg(int iType,char* lpszMsg,...)	//OK
{
	if( lpszMsg == 0 )
		return;

	char szBuffer[128] = {0};
	va_list pArguments;
	PMSG_NOTICE pNotice;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	switch( iType )
	{
	case 1:
		TNotice::MakeNoticeMsg(&pNotice,0,szBuffer);
		TNotice::SetNoticeProperty(&pNotice,10,_ARGB(255,255,200,80),1,0,20);
		TNotice::SendNoticeToAllUser(&pNotice);
		break;
	case 2:
		TNotice::MakeNoticeMsg(&pNotice,0,szBuffer);
		TNotice::SendNoticeToAllUser(&pNotice);
		break;
	}
}

//006bfe50
void CMonsterRegenSystem::SetRegenTimeTable()	//OK
{
	for(int i = 0; i < MAX_MONSTER_REGEN; i++)
	{
		if( this->m_MonsterGroupInfo[i].aType == 2 )
		{
			for(int nTimeIndex = 0; nTimeIndex < 24; nTimeIndex++ )
			{
				if( this->m_MonsterGroupInfo[i].m_RegenTimeTable[nTimeIndex].nBeginHour < 0 ||
					this->m_MonsterGroupInfo[i].m_RegenTimeTable[nTimeIndex].nEndHour < 0 )
				{

				}
				else
				{
					short nBeginHour = this->m_MonsterGroupInfo[i].m_RegenTimeTable[nTimeIndex].nBeginHour;
					short nEndHour = this->m_MonsterGroupInfo[i].m_RegenTimeTable[nTimeIndex].nEndHour;

					if( nBeginHour > nEndHour )
						nEndHour += 24;

					short nRegenHour = nBeginHour + rand()%(nEndHour-nBeginHour);
					short nRegenMin = rand()%60;
					nRegenHour = nRegenHour%24;

					this->m_MonsterGroupInfo[i].m_RegenTimeTable[nTimeIndex].nRegenHour = nRegenHour;
					this->m_MonsterGroupInfo[i].m_RegenTimeTable[nTimeIndex].nRegenMin = nRegenMin;
					this->m_MonsterGroupInfo[i].m_RegenTimeTable[nTimeIndex].bIsRegen = 0;
				}
			}
		}
	}
}

//006c0080
bool CMonsterRegenSystem::IsRegenTime(int nGroupNumber,int nCurHour,int nCurMin,bool bSetRegen)	//OK
{
	for(int i = 0; i < 24; i++)
	{
		if( this->m_MonsterGroupInfo[nGroupNumber].m_RegenTimeTable[i].nRegenHour == nCurHour &&
			this->m_MonsterGroupInfo[nGroupNumber].m_RegenTimeTable[i].nRegenMin <= nCurMin &&
			this->m_MonsterGroupInfo[nGroupNumber].m_RegenTimeTable[i].bIsRegen == 0 )
		{
			if( bSetRegen )
				this->m_MonsterGroupInfo[nGroupNumber].m_RegenTimeTable[i].bIsRegen = 1;
			return true;
		}
	}
	return false;
}