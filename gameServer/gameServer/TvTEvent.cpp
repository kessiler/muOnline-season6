#include "StdAfx.h"

#ifdef TVTEVENT

#include "user.h"
#include "TvTEvent.h"
#include "Message.h"
#include "Functions.h"
#include "DSProtocol.h"
#include "PCPointSystem.h"

cTvTEvent TvT;

void cTvTEvent::Load()
{
	this->Enable = GetPrivateProfileInt("Common","Enable",0,TVT_DIR);
	if(!this->Enable) return;
	this->NPC = GetPrivateProfileInt("Common","NPC",0,TVT_DIR);

	this->DisplayCommands = GetPrivateProfileInt("Common","DisplayCommands",1,TVT_DIR);

	this->WaitingMap = GetPrivateProfileInt("Common","WaitingMap",0,TVT_DIR);
	this->WaitingX = GetPrivateProfileInt("Common","WaitingX",0,TVT_DIR);
	this->WaitingY = GetPrivateProfileInt("Common","WaitingY",0,TVT_DIR);

	this->BluTeamX = GetPrivateProfileInt("Common","BluTeamX",0,TVT_DIR);
	this->BluTeamY = GetPrivateProfileInt("Common","BluTeamY",0,TVT_DIR);

	this->RedTeamX = GetPrivateProfileInt("Common","RedTeamX",0,TVT_DIR);
	this->RedTeamY = GetPrivateProfileInt("Common","RedTeamY",0,TVT_DIR);

	this->WaitingTime = GetPrivateProfileInt("Common","WaitingTime",0,TVT_DIR) * 60;

	this->EventTime = GetPrivateProfileInt("Common","EventTime",0,TVT_DIR) * 60;

	this->RewardItem = GetPrivateProfileInt("Common","RewardItem",0,TVT_DIR);
	this->ItemRewardType = GetPrivateProfileInt("Common","ItemRewardType",0,TVT_DIR);
	this->ItemRewardIndex = GetPrivateProfileInt("Common","ItemRewardIndex",0,TVT_DIR);
	this->ItemRewardLevel = GetPrivateProfileInt("Common","ItemRewardLevel",0,TVT_DIR);

	this->RewardExPoint = GetPrivateProfileInt("Common","RewardExPoint",0,TVT_DIR);
	this->RewardWCoin = GetPrivateProfileInt("Common","RewardWCoin",0,TVT_DIR);

	this->Waiting = false;
	this->Start = false;
	this->StartTick = 0;
	this->WaitingTick = 0;
	this->TotalPlayer = 0;

	FILE * file = fopen(TVT_DIR,"r");
	if(file == NULL)
	{
		this->Enable = 0;
		return;
	}
	for(int i=0;i<MAXTIME;i++)
	{
		this->T[i].H = 0;
		this->T[i].M = 0;
	}
	char Buff[256];
	int Flag = 0;
	this->Count = 0;
	while(!feof(file))
	{
		fgets(Buff,256,file);	
		if(Ex_IsBadFileLine(Buff, Flag))continue;
		if(Flag == 1)
		{
			int n[2];
			sscanf(Buff,"%d %d",&n[0],&n[1]);
			this->T[this->Count].H	= n[0];
			this->T[this->Count].M	= n[1];
			this->Count++;
		}
	}
	fclose(file);
}

void cTvTEvent::RestoreUser(int aIndex)
{
	this->Player[aIndex].Available = false;
	this->Player[aIndex].Team = 0;	
}

void cTvTEvent::RestoreTEvent(int aIndex)
{
	this->Player[aIndex].tEvent = false;
}

void cTvTEvent::NPCClick(LPOBJ lpNpc, LPOBJ lpObj)
{
	if(!this->Enable)return;
	if(lpNpc->Class == this->NPC)
	{
		if(!this->Waiting)
		{
			MsgNormal(lpObj->m_Index,"[TvT Event] Closed");
			return;
		}
		this->Player[lpObj->m_Index].tEvent = true;
		this->Player[lpObj->m_Index].Available = true;
		this->TotalPlayer++;
		gObjTeleport(lpObj->m_Index,this->WaitingMap,this->WaitingX,this->WaitingY);
	}
}

void cTvTEvent::TickTime()
{
	if(!this->Enable)return;
	SYSTEMTIME t;
	GetLocalTime(&t);

	if(this->Waiting == true)
	{
		this->WaitingTick++;
		if(this->WaitingTick == this->WaitingTime)
		{
			if(this->TotalPlayer >= 2)
			{
				this->DivisionTeam();
				MessaageAllGlobal("[TvT Event] Start");
				MessaageAllGlobal("[TvT Event] Total Player: %d",this->TotalPlayer);
				this->Start = true;
			}
			else
			{
				MessaageAllGlobal("[TvT Event] End");
				for(int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
				{
					LPOBJ sObj = &gObj[n];
					if(sObj->Connected != PLAYER_PLAYING) continue;
					if(this->Player[n].tEvent == true)
					{
						this->Player[n].tEvent = false;
						gObjMoveGate(n,17);	//Lorencia
						if(this->DisplayCommands == 1)
						{
							gObj[n].m_Change = -1;
							gObjViewportListProtocolCreate(&gObj[n]);
						}
						this->Reward(n);
						this->RestoreUser(n);
					}
				}
				this->Start = false;
				this->TotalPlayer = 0;
				this->ScoreBlue = 0;
				this->ScoreRed = 0;
			}
			this->Waiting = false;
			this->WaitingTick = 0;
		}
		return;
	}

	if(this->Start == true)
	{
		this->StartTick++;
		if(this->StartTick == this->EventTime)
		{
			MessaageAllGlobal("[TvT Event] End");

			if(this->ScoreBlue > this->ScoreRed) 
			{
				MessaageAllGlobal("[TvT Event] Win Blue Team");
				MessaageAllGlobal("[TvT Event] Blue: %d Vs Red: %d",this->ScoreBlue,this->ScoreRed);
			}
			else if(this->ScoreRed > ScoreBlue) 
			{
				MessaageAllGlobal("[TvT Event] Win Red Team");
				MessaageAllGlobal("[TvT Event] Red: %d Vs Blue: %d",this->ScoreRed,this->ScoreBlue);
			}
			else 
			{
				MessaageAllGlobal("[TvT Event] Dead heat");
			}

			for(int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
			{
				LPOBJ sObj = &gObj[n];
				if(sObj->Connected != PLAYER_PLAYING) continue;
				if(this->Player[n].tEvent == true)
				{
					this->Player[n].tEvent = false;
					gObjMoveGate(n,17);	//Lorencia
					if(this->DisplayCommands == 1)
					{
						gObj[n].m_Change = -1;
						gObjViewportListProtocolCreate(&gObj[n]);
					}
					this->Reward(n);
					this->RestoreUser(n);
				}
			}
			this->Start = false;
			this->TotalPlayer = 0;
			this->ScoreBlue = 0;
			this->ScoreRed = 0;
			this->StartTick = 0;
		}
		return;
	}

	if(t.wSecond == 00)
	{
		for(int i=0;i<this->Count;i++)
		{
			if(t.wHour == this->T[i].H && t.wMinute == this->T[i].M)
			{
				MessaageAllGlobal("[TvT Event] Starts in %d minute",(this->WaitingTime/60));
				this->Waiting = true;
				return;
			}
		}
	}
}

int cTvTEvent::ReturnMaxReset()
{
	int MaxRes = 0;
	int Index = 0;
	for(int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
	{
		LPOBJ sObj = &gObj[i];
		if(sObj->Connected != PLAYER_PLAYING) continue;
		if(!this->Player[i].Available)continue;
		if(ExUser[i].Reset >= MaxRes)
		{
			MaxRes = ExUser[i].Reset;
			Index = i;
		}
	}
	this->Player[Index].Available = false;
	return Index;
}

void cTvTEvent::DivisionTeam()
{
	int iTeam = 1;
	int Index = 0;
	for(int i=0;i<this->TotalPlayer;i++)
	{
		Index = this->ReturnMaxReset();
		this->Player[Index].Team = iTeam;
		if(iTeam == 1) iTeam = 2;
		else iTeam = 1;
	}
	for(int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		LPOBJ sObj = &gObj[n];
		if(sObj->Connected != PLAYER_PLAYING) continue;
		if(this->Player[n].tEvent == false) continue;
		if(this->Player[n].Team == 1)
		{
			gObjTeleport(n,this->WaitingMap,this->BluTeamX,this->BluTeamY);
			if(this->DisplayCommands == 1)
			{
				gObj[n].m_Change = 405; //"MU Allies"	Blue
				gObjViewportListProtocolCreate(&gObj[n]);
			}
			MsgNormal(n,"[BlueTeam]: %d Vs [RedTeam]: %d",this->ScoreBlue,this->ScoreRed);
		}
		else if(this->Player[n].Team == 2)
		{
			gObjTeleport(n,this->WaitingMap,this->RedTeamX,this->RedTeamY);
			if(this->DisplayCommands == 1)
			{
				gObj[n].m_Change = 404; //"MU Allies"	Red
				gObjViewportListProtocolCreate(&gObj[n]);
			}
			MsgNormal(n,"[RedTeam]: %d Vs [BlueTeam]: %d",this->ScoreRed,this->ScoreBlue);
		}	
	}
}

bool cTvTEvent::CheckStartEvent()
{
	if(!this->Enable || !this->Start)return false;
	return true;
}

int cTvTEvent::ReturnStatus(int aIndex, int TargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	if(this->Player[aIndex].tEvent == false || this->Player[aIndex].Team == 0) return (int)lpObj->m_PK_Level;
	if(this->Player[aIndex].Team == this->Player[TargetIndex].Team) return 3;
	else return 6;
}

bool cTvTEvent::PkCheck(LPOBJ lpObj, LPOBJ lpTarget)
{
	if(!this->Enable)return true;
	if(!this->Start)return true;
	if(this->Player[lpObj->m_Index].tEvent == true && this->Player[lpTarget->m_Index].tEvent == true) return false;
	return true;
}

void cTvTEvent::ProcessKill(LPOBJ lpObj, LPOBJ lpTarget)
{
	if(!this->Enable)return;
	if(!this->Start)return;
	if(this->Player[lpObj->m_Index].Team == BLUETEAM && this->Player[lpTarget->m_Index].Team == REDTEAM)
	{
		this->ScoreBlue++;
	}
	else if(this->Player[lpObj->m_Index].Team == REDTEAM && this->Player[lpTarget->m_Index].Team == BLUETEAM) 
	{
		this->ScoreRed++;
	}
	for(int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		LPOBJ sObj = &gObj[n];
		if(sObj->Connected != PLAYER_PLAYING) continue;
		if(this->Player[n].tEvent == false) continue;
		if(this->Player[n].Team == BLUETEAM)
		{
			MsgNormal(n,"[BlueTeam]: %d Vs [RedTeam]: %d",this->ScoreBlue,this->ScoreRed);
		}
		else if(this->Player[n].Team == REDTEAM)
		{
			MsgNormal(n,"[RedTeam]: %d Vs [BlueTeam]: %d",this->ScoreRed,this->ScoreBlue);
		}
	}
}

bool cTvTEvent::AllyKillCheck(LPOBJ lpObj, LPOBJ lpTarget)
{
	if(!this->Enable)return true;
	if(!this->Start)return true;
	if(this->Player[lpObj->m_Index].tEvent == false) return true;
	if(this->Player[lpObj->m_Index].Team == BLUETEAM && this->Player[lpTarget->m_Index].Team == BLUETEAM) return false;
	if(this->Player[lpObj->m_Index].Team == REDTEAM && this->Player[lpTarget->m_Index].Team == REDTEAM) return false;
	return true;
}

void cTvTEvent::Reward(int aIndex)
{
	if(!this->Enable)return;

	int TeamReward = 0;
	if(this->ScoreBlue > this->ScoreRed)
		TeamReward = BLUETEAM;
	else if(this->ScoreRed > ScoreBlue)
		TeamReward = REDTEAM;
	
	if(TeamReward == 0) return;

	if(this->Player[aIndex].Team != TeamReward) return;

	LPOBJ lpObj = &gObj[aIndex];

	if(this->RewardExPoint > 0)
	{
		ExUser[aIndex].PCPoint += this->RewardExPoint;
		ExPCPointSystem.InitPCPointForPlayer(lpObj, ExUser[aIndex].PCPoint );
	}
	if(this->RewardWCoin > 0)
	{
		GDRequestWcoinInfoLoad (lpObj,this->RewardWCoin,1);
	}
	
	if(this->RewardItem == 0) return;

	ItemSerialCreateSend(aIndex,235,lpObj->X,lpObj->Y,ITEMGET(this->ItemRewardType,this->ItemRewardIndex),(BYTE)this->ItemRewardLevel,0,0,0,0,aIndex,0,0);
}

bool cTvTEvent::CheckGateEvent(int aIndex)
{
	if(!this->Enable)return true;
	if(this->Player[aIndex].tEvent == true) return false;
	return true;
}

void cTvTEvent::CheckMapXY(LPOBJ lpObj)
{
	if(lpObj->MapNumber == 6 && lpObj->X >= 100 && lpObj->X <= 120 && lpObj->Y >= 150 && lpObj->Y <= 186)
	{
		lpObj->MapNumber = 0;
		lpObj->X = 123;
		lpObj->Y = 123;
	}
}

void cTvTEvent::ChatData(int aIndex, LPBYTE Protocol)
{
	LPOBJ lpObj = &gObj[aIndex];
	if(lpObj->Authority == 8 || lpObj->Authority == 32)
	{
		char CommandReloadConfig[]	= "/reloadtvt";
		if(!memcmp(&Protocol[13],CommandReloadConfig,strlen(CommandReloadConfig)))
		{
			this->Load();
		}
	}
}

#endif