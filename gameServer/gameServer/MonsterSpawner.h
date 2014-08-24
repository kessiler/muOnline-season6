#include "StdAfx.h"

#if __ALIEN__

#include "user.h"

#define MAX_MONSTER 100
#define MAX_CONFIG	3

class cMonsterSpawner
{
private:
	int TotalConfig;
	int Group[MAX_CONFIG];
	bool Enable[MAX_CONFIG];
	int Type[MAX_CONFIG];
	int Value[MAX_CONFIG];
	int Hour[MAX_CONFIG];
	int Min[MAX_CONFIG];
	int TimeEnd[MAX_CONFIG];

	bool EventStart[MAX_CONFIG];
	int MonsterIndex[MAX_MONSTER];

	struct
	{
		int Mob;
		int Map;
		int X;
		int Y;
		int Group;
	}Monster[MAX_MONSTER];

	int TotalMob;

	int Tick[MAX_CONFIG];

public:
	cMonsterSpawner();
	~cMonsterSpawner();

	void Load();
	void StartMonster(int p);
	void EndMonster(int p);
	void StatusMonster(LPOBJ lpObj);

	void StartEvent(int p);
	void EndEvent(int p);

	void TickTime();

	bool EventStatus();

};
extern cMonsterSpawner gMonsterSpawner;

#endif