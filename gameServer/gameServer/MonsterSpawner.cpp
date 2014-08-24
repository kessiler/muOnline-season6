#include "StdAfx.h"

#if __ALIEN__

#include "MonsterSpawner.h"
#include "..\include\ReadScript.h"
#include "..\common\winutil.h"
#include "logproc.h"
#include "GameMain.h"
#include "user.h"

SYSTEMTIME t;

cMonsterSpawner gMonsterSpawner;

cMonsterSpawner::cMonsterSpawner()
{
	for(int i=0;i<MAX_CONFIG;i++)
	{
		this->Enable[i] = false;

		this->Type[i] = 0;
		this->Value[i] = 0;
		this->Hour[i] = 20;
		this->Min[i] = 0;
		this->TimeEnd[i] = 300;
		this->EventStart[i] = false;
		this->Tick[i] = 0;
	}

	this->TotalMob = 0;

	this->TotalConfig = 0;

	for(int i=0;i<MAX_MONSTER;i++)
	{
		this->MonsterIndex[i] = -1;

		this->Monster[i].Map = 0;
		this->Monster[i].Mob = 0;
		this->Monster[i].X = 0;
		this->Monster[i].Y = 0;
		this->Monster[i].Group = 0;
	}
}

cMonsterSpawner::~cMonsterSpawner()
{

}

void cMonsterSpawner::Load()
{
	SMDToken Token;
	SMDFile = fopen(gDirPath.GetNewPath("Custom\\MonsterSpawner.txt"), "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[MonsterSpawner] %s file not found", gDirPath.GetNewPath("Custom\\MonsterSpawner.txt"));
		return;
	}
	// ----
	int Category = -1;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		Category = TokenNumber;
		// ----
		while(true)
		{
			if( Category == 0 )	//-> Config
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				if(this->TotalConfig >= MAX_CONFIG)
				{
					continue;
				}
				// ----
				this->Group[this->TotalConfig]		= TokenNumber;
				// ----
				Token = GetToken();
				this->Enable[this->TotalConfig]		= TokenNumber;
				// ----
				Token = GetToken();
				this->Type[this->TotalConfig]		= TokenNumber;
				// ----
				Token = GetToken();
				this->Value[this->TotalConfig]		= TokenNumber;
				// ----
				Token = GetToken();
				this->Hour[this->TotalConfig]		= TokenNumber;
				// ----
				Token = GetToken();
				this->Min[this->TotalConfig]		= TokenNumber;
				// ----
				Token = GetToken();
				this->TimeEnd[this->TotalConfig]	= TokenNumber;

				this->TimeEnd[this->TotalConfig]	*= 60;	//Tested

				this->TotalConfig++;
			}
			else if( Category == 1 ) //-> Monster
			{
				Token = GetToken();
				// ----
				if( !strcmp("end", TokenString) )
				{
					break;
				}
				if(this->TotalMob >= MAX_MONSTER)
				{
					break;
				}
				// ----
				this->Monster[this->TotalMob].Mob	= TokenNumber;
				// ----
				Token = GetToken();
				this->Monster[this->TotalMob].Map	= TokenNumber;
				// ----
				Token = GetToken();
				this->Monster[this->TotalMob].X		= TokenNumber;
				// ----
				Token = GetToken();
				this->Monster[this->TotalMob].Y		= TokenNumber;
				// ----
				Token = GetToken();
				this->Monster[this->TotalMob].Group	= TokenNumber;
				// ----
				this->TotalMob++;
			}
		}
	}
	// ----
	fclose(SMDFile);
	LogAddTD("[MonsterSpawner] %s file is loaded (Mobs: %d)", gDirPath.GetNewPath("Custom\\MonsterSpawner.txt"), this->TotalMob);
}

void cMonsterSpawner::StartMonster(int p)
{
	for(int i=0;i<this->TotalMob;i++)
	{
		if(this->Monster[i].Group == this->Group[p])
		{
			int result = gObjAddMonster(this->Monster[i].Map);

			if(result >= 0 && result < OBJ_MAXMONSTER)
			{
				gObj[result].m_PosNum = (WORD)-1;
				gObj[result].MapNumber = this->Monster[i].Map;
				gObj[result].X = this->Monster[i].X;
				gObj[result].Y = this->Monster[i].Y;
				gObj[result].TX = this->Monster[i].X;
				gObj[result].TY = this->Monster[i].Y;
				gObj[result].m_OldX = this->Monster[i].X;
				gObj[result].m_OldY = this->Monster[i].Y;
				gObj[result].StartX = this->Monster[i].X;
				gObj[result].StartY = this->Monster[i].Y;
				gObj[result].MaxRegenTime = 0;
				gObj[result].Dir = rand()%8;
				gObj[result].RegenTime = GetTickCount();

				gObj[result].m_ActState.Emotion       = 1;
				gObj[result].m_ActState.EmotionCount  = 15;

				gObjSetMonster(result,this->Monster[i].Mob);

				this->MonsterIndex[i] = result;
			}
		}
	}
}

void cMonsterSpawner::EndMonster(int p)
{
	for ( int i=0;i<MAX_MONSTER;i++)
	{
		if(this->Monster[i].Group == this->Group[p])
		{
			if ( this->MonsterIndex[i] != -1 && this->MonsterIndex[i] )
			{
				gObjDel(this->MonsterIndex[i]);
				this->MonsterIndex[i] = -1;
			}
		}
	}
}

void cMonsterSpawner::StatusMonster(LPOBJ lpObj)
{
	int Code = -1;
	for ( int i=0;i<this->TotalMob;i++)
	{
		if(this->MonsterIndex[i] == lpObj->m_Index)
		{
			Code = i;
			break;
		}
	}

	if(Code == -1)
	{
		return;
	}

	lpObj->Live = TRUE;
	lpObj->X = this->Monster[Code].X;
	lpObj->Y = this->Monster[Code].Y;
	lpObj->TX = this->Monster[Code].X;
	lpObj->TY = this->Monster[Code].Y;
	lpObj->m_OldX = this->Monster[Code].X;
	lpObj->m_OldY = this->Monster[Code].Y;
	lpObj->StartX = this->Monster[Code].X;
	lpObj->StartY = this->Monster[Code].Y;

	lpObj->m_ActState.Emotion       = 1;
	lpObj->m_ActState.EmotionCount  = 15;
}

void cMonsterSpawner::StartEvent(int p)
{
	//if(!this->Enable) return;

	this->EventStart[p] = true;
	this->StartMonster(p);

	char Message[100];
	sprintf(Message,"[MonsterSpawner] Group %d Start!",this->Group[p]);

	AllSendServerMsg(Message);
}

void cMonsterSpawner::EndEvent(int p)
{
	//if(!this->Enable) return;

	this->EventStart[p] = false;
	this->EndMonster(p);

	char Message[100];
	sprintf(Message,"[MonsterSpawner] Group %d End!",this->Group[p]);

	AllSendServerMsg(Message);
}

void cMonsterSpawner::TickTime()
{
	//if(!this->Enable) return;
	for(int i=0;i<this->TotalConfig;i++)
	{
		if(!this->Enable[i]) continue;
		if(this->EventStart[i] == true)
		{
			this->Tick[i]++;
			if(this->Tick[i] >= this->TimeEnd[i])
			{
				this->EndEvent(i);
				this->Tick[i] = 0;
			}
		}
	}

	GetLocalTime(&t);

	for(int s=0;s<this->TotalConfig;s++)
	{
		if(this->Type[s] == 0)			// аждый день
		{
			if(t.wHour == this->Hour[s] && t.wMinute == this->Min[s] && t.wSecond == 00)
			{
				this->StartEvent(s);
			}
		}
		else if(this->Type[s] == 1)	//раз в неделю, value = день недели 1-7
		{
			if(t.wDayOfWeek == this->Value[s] && t.wHour == this->Hour[s] && t.wMinute == this->Min[s] && t.wSecond == 00)
			{
				this->StartEvent(s);
			}
		}
		else if(this->Type[s] == 2)	//раз в мес€ц, value = число в мес€це
		{
			if(t.wDay == this->Value[s] && t.wHour == this->Hour[s] && t.wMinute == this->Min[s] && t.wSecond == 00)
			{
				this->StartEvent(s);
			}
		}
	}
}

bool cMonsterSpawner::EventStatus()
{
	//if(!this->Enable) return false;
	for(int s=0;s<this->TotalConfig;s++)
	{
		if(this->EventStart[s] == true)
		{
			return true;
		}
	}
	return false;
}

#endif