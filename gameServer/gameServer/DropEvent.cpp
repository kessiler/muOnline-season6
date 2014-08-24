#include "StdAfx.h"

#ifdef DROP_EVENT

#include "DropEvent.h"
#include "USER.h"
#include "ExUtilits.h"
#include "DSProtocol.h"
#include "GameMain.h"

cDropEvent DropEvent;

void cDropEvent::LoadConfigs()
{
	Enable = GetPrivateProfileInt("Common","Enable",1,gDirPath.GetNewPath(DROP_EVENT_DIR));

	for(int i(0);i < DROP_EVENT_MAX; i++)
	{
		Item[i].Index = 0;
		Item[i].Type  = 0;
		Item[i].Level = 0;
		Item[i].Skill = 0;
		Item[i].Luck  = 0;
		Item[i].Opt   = 0;
		Item[i].Exl	  = 0;
		Item[i].Anc   = 0;
	}

	for(int j(0);j < DROP_EVENT_MAX; j++)
	{
		Time[j].Hour  = 0;
		Time[j].Min   = 0;
	}

	for(int f=0;f<100;f++)
	{
		Cord[f].Map	= 0;
		Cord[f].X	= 0;
		Cord[f].Y	= 0;
	}
}

void cDropEvent::LoadFile()
{
	LoadConfigs();

	FILE *file = fopen(DROP_EVENT_DIR,"r");

	if(file == NULL)
	{
		MessageBoxA(0,"Data\\Custom\\DropEvent.ini Load Error","CRITICAL ERROR",0);
		ExitProcess(1);
		return;
	}

	char Buff[256];
	int Flag = 0;

	Count[1] = 0;
	Count[2] = 0;
	Count[3] = 0;

	while(!feof(file))
	{
		fgets(Buff,256,file);
		if(Ex_IsBadFileLine(Buff, Flag))
			continue;
		if(Flag == 1)
		{
			int n[10];
			sscanf(Buff, "%d %d %d %d %d %d %d %d", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5], &n[6], &n[7]);

			Item[Count[1]].Type  = n[0];
			Item[Count[1]].Index = n[1];
			Item[Count[1]].Level = n[2];
			Item[Count[1]].Skill = n[3];
			Item[Count[1]].Luck  = n[4];
			Item[Count[1]].Opt   = n[5];
			Item[Count[1]].Exl	 = n[6];
			Item[Count[1]].Anc   = n[7];
			Count[1]++;
		}

		if(Flag == 2)
		{
			int n[3];
			sscanf(Buff, "%d %d", &n[0], &n[1]);
			Time[Count[2]].Hour  = n[0];
			Time[Count[2]].Min   = n[1];
			Count[2]++;
		}

		if(Flag == 3)
		{
			int n[4];
			sscanf(Buff, "%d %d %d", &n[0], &n[1], &n[2]);
			Cord[Count[3]].Map  = n[0];
			Cord[Count[3]].X	= n[1];
			Cord[Count[3]].Y	= n[2];
			Count[3]++;
		}
	}
	fclose(file);
}
void cDropEvent::StartEvent()
{
	AllSendServerMsg("[Drop Event] Started!");
	for(int i=0;i<Count[3];i++)
	{
		int Ran = rand()%Count[1];

		int NewExl = 0;
		int NewAnc = 0;

		int DropItem = ITEMGET(Item[Ran].Type,Item[Ran].Index);

		if(Item[Ran].Exl > 0)
			NewExl = ExcOpt(Item[Ran].Exl);

		if(Item[Ran].Anc == 5 || Item[Ran].Anc == 10)
			NewAnc = Item[Ran].Anc;

		int CordMap = Cord[i].Map;
		int CordX = Cord[i].X;
		int CordY = Cord[i].Y;

		ItemSerialCreateSend(NULL,CordMap,CordX,CordY,DropItem,Item[Ran].Level,0,Item[Ran].Skill,Item[Ran].Luck,Item[Ran].Opt,NULL,NewExl,NewAnc);
	}
}
void cDropEvent::TickTime()
{
	if(!Enable)
		return;

	SYSTEMTIME t;
	GetLocalTime(&t);
	if(t.wSecond == 00)
	{
		for(int i = 0; i < Count[2]; i++)
		{
			if(Time[i].Hour == t.wHour && Time[i].Min == t.wMinute)
			{
				this->StartEvent();
				return;
			}
		}
	}
}

#endif