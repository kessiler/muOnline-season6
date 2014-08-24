#include "StdAfx.h"

#ifdef TVTEVENT

#include "user.h"

#define TVT_DIR "..\\Custom\\TvTEvent.ini"
#define MAXTIME 10

#define BLUETEAM 1
#define REDTEAM 2

class cTvTEvent
{
private:
	int Enable;
	int NPC;

	int DisplayCommands;

	int WaitingMap;
	int WaitingX;
	int WaitingY;

	int BluTeamX;
	int BluTeamY;

	int RedTeamX;
	int RedTeamY;

	int WaitingTime;
	int EventTime;

	int RewardItem;
	int ItemRewardType;
	int ItemRewardIndex;
	int ItemRewardLevel;

	int RewardExPoint;
	int RewardWCoin;

	struct
	{
		int H;
		int M;
	}T[MAXTIME];
	int Count;
	//---
	bool Waiting;
	bool Start;
	int WaitingTick;
	int StartTick;

	struct
	{
		bool tEvent;
		bool Available;
		int Team;
	}Player[OBJMAX];
	int TotalPlayer;

	int ScoreBlue;
	int ScoreRed;

public:
	void Load();
	void RestoreUser(int aIndex);
	void NPCClick(LPOBJ lpNpc, LPOBJ lpObj);
	void TickTime();
	int ReturnMaxReset();
	void DivisionTeam();
	bool CheckStartEvent();
	int ReturnStatus(int aIndex, int TargetIndex);
	bool PkCheck(LPOBJ lpObj, LPOBJ lpTarget);
	void ProcessKill(LPOBJ lpObj, LPOBJ lpTarget);
	bool AllyKillCheck(LPOBJ lpObj, LPOBJ lpTarget);
	void Reward(int aIndex);
	bool CheckGateEvent(int aIndex);
	void CheckMapXY(LPOBJ lpObj);
	void RestoreTEvent(int aIndex);
	void ChatData(int aIndex, LPBYTE Protocol);
};
extern cTvTEvent TvT;

#endif