#include "StdAfx.h"

#ifdef DROP_EVENT

#define DROP_EVENT_MAX	256
#define DROP_EVENT_DIR  "Custom\\DropEvent.ini"

class cDropEvent
{
public:
	int Enable;

	void LoadConfigs();
	void LoadFile();
	void StartEvent();
	void TickTime();

	struct
	{
		int Index;
		int Type;
		int Level;
		int Skill;
		int Luck;
		int Opt;
		int Exl;
		int Anc;
	}Item[DROP_EVENT_MAX];

	struct 
	{
		int Hour;
		int Min;
	}Time[DROP_EVENT_MAX];

	struct  
	{
		int Map;
		int X;
		int Y;
	}Cord[100];

	int Count[4];
};
extern cDropEvent DropEvent;

#endif