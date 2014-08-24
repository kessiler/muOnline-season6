//	GS-CS	1.00.90	JPN		-	Completed
#include "stdafx.h"
#include "EledoradoEvent.h"
#include "Gamemain.h"
#include "LogProc.h"
#include "user.h"
#include "BuffEffectSlot.h"

CEledoradoEvent gEledoradoEvent;

CEledoradoEvent::CEledoradoEvent()
{
	this->GoldGoblenEventStartTime = 0;
	this->TitanEventStartTime = 0;
	this->GoldDerconEventStartTime = 0;
	this->DevilLizardKingEventStartTime = 0;
	this->KanturEventStartTime = 0;
	//Season 4.5 addon start
	this->GoldenRabbitRegenTime = 0;
	this->GoldenDarkKnightRegenTime = 0;
	this->GoldenDevilRegenTime = 0;
	this->GoldenMonsterRegenTime = 0;
	this->GoldenCrustRegenTime = 0;
	this->GoldenSatirosRegenTime = 0;
	this->GoldenTwintailRegenTime = 0;
	this->GoldenIronKnightRegenTime = 0;
	this->GoldenNeipinRegenTime = 0;
	this->GoldenGreatDragonRegenTime = 0;
	//Season 4.5 addon end
	this->m_BossGoldDerconMapNumber[0] = -1;
	this->m_BossGoldDerconMapNumber[1] = -1;
	this->m_BossGoldDerconMapNumber[2] = -1;
}

CEledoradoEvent::~CEledoradoEvent()
{
	return;
}

void CEledoradoEvent::Init()
{
	return;
}

void CEledoradoEvent::SetEventState(int State)
{
	this->EventState = State;
}

//00484C10  /> \55            PUSH EBP
void CEledoradoEvent::Run()
{
	if ( this->m_bMenualStart != FALSE )
	{
		return;
	}

	if ( this->EventState == 0 )
	{
		return;
	}

	if ( this->GoldGoblenEventStartTime == 0 )
	{
		tm * today;
		time_t ltime;

		time(&ltime);
		today = localtime(&ltime);

		if ( today->tm_min >= 30 && today->tm_min  <= 35 )
		{
			int Time = GetTickCount();

			this->GoldGoblenEventStartTime = Time;
			this->RegenGoldGoblen();

			this->TitanEventStartTime = Time;
			this->RegenTitan();

			this->GoldDerconEventStartTime = Time;
			this->RegenGoldDercon();

			this->DevilLizardKingEventStartTime = Time;
			this->RegenDevilLizardKing();

			this->KanturEventStartTime = Time;
			this->RegenKantur();

			//Season 4.5 addon start
			this->GoldenRabbitRegenTime = Time;
			this->RegenGoldenRabbit();

			this->GoldenDarkKnightRegenTime = Time;
			this->RegenGoldenDarkKnight();

			this->GoldenDevilRegenTime = Time;
			this->RegenGoldenDevil();

			this->GoldenMonsterRegenTime = Time;
			this->RegenGoldenMonster();

			this->GoldenCrustRegenTime = Time;
			this->RegenGoldenCrust();

			this->GoldenSatirosRegenTime = Time;
			this->RegenGoldenSatiros();

			this->GoldenTwintailRegenTime = Time;
			this->RegenGoldenTwintail();

			this->GoldenIronKnightRegenTime = Time;
			this->RegenGoldenIronKnight();

			this->GoldenNeipinRegenTime = Time;
			this->RegenGoldenNeipin();

			this->GoldenGreatDragonRegenTime = Time;
			this->RegenGoldenGreatDragon();

			//Season 4.5 addon end
		}
	}
	else
	{
		DWORD Time = GetTickCount();

		if ( ( Time - this->GoldGoblenEventStartTime ) > gEledoradoGoldGoblenRegenTime * 60000 )
		{
			this->GoldGoblenEventStartTime = Time;
			this->RegenGoldGoblen();
		}

		if ( ( Time - this->TitanEventStartTime ) > gEledoradoTitanRegenTime * 60000 )
		{
			this->TitanEventStartTime = Time;
			this->RegenTitan();
		}

		if ( ( Time - this->GoldDerconEventStartTime ) > gEledoradoGoldDerconRegenTime * 60000 )
		{
			this->GoldDerconEventStartTime = Time;
			this->RegenGoldDercon();
		}

		if ( ( Time - this->DevilLizardKingEventStartTime ) > gEledoradoDevilLizardKingRegenTime * 60000 )
		{
			this->DevilLizardKingEventStartTime = Time;
			this->RegenDevilLizardKing();
		}

		if ( ( Time - this->KanturEventStartTime ) > gEledoradoDevilTantarosRegenTime * 60000 )
		{
			this->KanturEventStartTime = Time;
			this->RegenKantur();
		}
	

		//Season 4.5 addon start
		if ( ( Time - this->GoldenRabbitRegenTime ) > gEledoradoGoldenRabbitRegenTime * 60000 )
		{
			this->GoldenRabbitRegenTime = Time;
			this->RegenGoldenRabbit();
		}

		if ( ( Time - this->GoldenDarkKnightRegenTime ) > gEledoradoGoldenDarkKnightRegenTime * 60000 )
		{
			this->GoldenDarkKnightRegenTime = Time;
			this->RegenGoldenDarkKnight();
		}

		if ( ( Time - this->GoldenDevilRegenTime ) > gEledoradoGoldenDevilRegenTime * 60000 )
		{
			this->GoldenDevilRegenTime = Time;
			this->RegenGoldenDevil();
		}

		if ( ( Time - this->GoldenMonsterRegenTime ) > gEledoradoGoldenMonsterRegenTime * 60000 )
		{
			this->GoldenMonsterRegenTime = Time;
			this->RegenGoldenMonster();
		}

		if ( ( Time - this->GoldenCrustRegenTime ) > gEledoradoGoldenCrustRegenTime * 60000 )
		{
			this->GoldenCrustRegenTime = Time;
			this->RegenGoldenCrust();
		}

		if ( ( Time - this->GoldenSatirosRegenTime ) > gEledoradoGoldenSatirosRegenTime * 60000 )
		{
			this->GoldenSatirosRegenTime = Time;
			this->RegenGoldenSatiros();
		}

		if ( ( Time - this->GoldenTwintailRegenTime ) > gEledoradoGoldenTwintailRegenTime * 60000 )
		{
			this->GoldenTwintailRegenTime = Time;
			this->RegenGoldenTwintail();
		}

		if ( ( Time - this->GoldenIronKnightRegenTime ) > gEledoradoGoldenIronKnightRegenTime * 60000 )
		{
			this->GoldenIronKnightRegenTime = Time;
			this->RegenGoldenIronKnight();
		}

		if ( ( Time - this->GoldenNeipinRegenTime ) > gEledoradoGoldenNeipinRegenTime * 60000 )
		{
			this->GoldenNeipinRegenTime = Time;
			this->RegenGoldenNeipin();
		}

		if ( ( Time - this->GoldenGreatDragonRegenTime ) > gEledoradoGoldenGreatDragonRegenTime * 60000 )
		{
			this->GoldenGreatDragonRegenTime = Time;
			this->RegenGoldenGreatDragon();
		}

		//Season 4.5 addon end
	}
}

void CEledoradoEvent::RegenGoldGoblen()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 78 )
		{
			gObj[n].Live = TRUE;
			MapNumber = 3;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make GoldGoblen : %d, %d,%d", MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenTitan()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 53 )	// Golden Titan
		{
			gObj[n].Live = TRUE;
			MapNumber = 2;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			this->m_BossTitanMapNumber = gObj[n].MapNumber;
			this->m_BossTitanMapX = gObj[n].X;
			this->m_BossTitanMapY = gObj[n].Y;

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make GoldTaitan : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
		else if ( gObj[n].Class == 54 ) // Golden Soldier
		{
			gObj[n].Live = TRUE;
			MapNumber = this->m_BossTitanMapNumber;
			gObj[n].MapNumber = MapNumber;

			gMSetBase.GetBoxPosition(MapNumber, this->m_BossTitanMapX-4, this->m_BossTitanMapY-4,
				this->m_BossTitanMapX+4, this->m_BossTitanMapY+4, gObj[n].X, gObj[n].Y);

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;
		}
	}
}

void CEledoradoEvent::RegenGoldDercon()
{
	int n=0;
	int MapNumber=-1;
	int Map[3]={0, 3, 2}; // Map[2]={0,4};
	int SelMap = -1;
	int count = 0;

	this->CheckGoldDercon(MapNumber);	// #error??? MapNumber hasn't been setted

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 79 )	// Gold Dercon
		{
			gObj[n].Live = TRUE;

			if ( SelMap == -1 )	
			{
				MapNumber = Map[rand()%3]; // MapNumber = Map[rand()%2];
			}
			else
			{
				MapNumber = SelMap;
			}

			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 80, 80, 170, 170, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make GoldDercon : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);

			if ( SelMap == -1 )
			{
				char szTemp[256];
				strcpy(szTemp, " ");

				switch ( MapNumber )
				{
					case 0:
						strcat(szTemp, lMsg.Get(MSGGET(7, 208)));
						break;
					case 2:
						strcat(szTemp, lMsg.Get(MSGGET(7, 210)));
						break;
					case 3:
						strcat(szTemp, lMsg.Get(MSGGET(7, 211)));
						break;
				}

				strcat(szTemp, lMsg.Get(MSGGET(4, 134)));
			}

			this->m_BossGoldDerconMapNumber[count] = MapNumber;
			gObj[n].m_BossGoldDerconMapNumber = count;	// #error change count by MapNumber
			count++;
			SelMap = MapNumber;
		}
	}

	this->CheckGoldDercon(MapNumber);
}

void CEledoradoEvent::RegenDevilLizardKing()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		MapNumber = 7;

		if ( gObj[n].Class == 80 )	// Devil Lizard King
		{
			gObj[n].Live = TRUE;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 50, 10, 250, 200, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			this->m_BossDevilLizardKingMapNumber = gObj[n].MapNumber;
			this->m_BossDevilLizardKingMapX = gObj[n].X;
			this->m_BossDevilLizardKingMapY = gObj[n].Y;

			gObj[n].Life = gObj[n].MaxLife;

			LogAddTD("Make GoldLizarKing : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
		else if ( gObj[n].Class == 81 ) // Golden Velparie
		{
			gObj[n].Live = TRUE;
			MapNumber = this->m_BossDevilLizardKingMapNumber;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 15, 50, 250, 200, gObj[n].X, gObj[n].Y) == 0 )//NEW changed
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
		}
	}
}

void CEledoradoEvent::RegenKantur()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 82 )	// Gold Tantalos
		{
			gObj[n].Live = TRUE;
			MapNumber = 8;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 50, 50, 200, 200, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			this->m_BossKanturMapNumber = gObj[n].MapNumber;
			this->m_BossKanturMapX = gObj[n].X;
			this->m_BossKanturMapY = gObj[n].Y;

			gObj[n].Life = gObj[n].MaxLife;
			/*LogAddTD("GoldenDebug %d HP: %d", gObj[n].Class, gObj[n].Life);
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
			gObjClearBuffEffect(&gObj[n], CLEAR_TYPE_DIEREGEN);
			gObj[n].m_ViewState = 0;
			gObj[n].Teleport = 0;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;*/

			LogAddTD("Make GoldTantarus : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
		else if ( gObj[n].Class == 83 ) // Gold Iron Wheel
		{
			gObj[n].Live = TRUE;
			MapNumber = this->m_BossKanturMapNumber;
			gObj[n].MapNumber = MapNumber;

			gMSetBase.GetBoxPosition(MapNumber, this->m_BossKanturMapX-10, this->m_BossKanturMapY-10,
				this->m_BossKanturMapX+10, this->m_BossKanturMapY+10, gObj[n].X, gObj[n].Y);

			gObj[n].Life = gObj[n].MaxLife;
			/*LogAddTD("GoldenDebug %d HP: %d", gObj[n].Class, gObj[n].Life);
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;
			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;*/
		}
	}
}

void CEledoradoEvent::CheckGoldDercon(int MapNumber)
{
	if ( this->EventState == 0 )
		return;

	BOOL EventOn = FALSE;
	int EventClearMapNumber = -1;

	for ( int i=0;i<3;i++)
	{
		if ( this->m_BossGoldDerconMapNumber[i] != -1 )
		{
			if ( this->m_BossGoldDerconMapNumber[i] == MapNumber )
			{
				EventOn = TRUE;
			}
			else
			{
				EventClearMapNumber = this->m_BossGoldDerconMapNumber[i];
			}
		}
	}

	if ( EventClearMapNumber != -1 )
	{
		GCMapEventStateSend(EventClearMapNumber, 0, 3);
	}

	if ( EventOn != FALSE )
	{
		GCMapEventStateSend(MapNumber, 1, 3);
	}
	else
	{
		GCMapEventStateSend(MapNumber, 0, 3);
	}
}



void CEledoradoEvent::Start_Menual()
{
	this->SetMenualStart(TRUE);

	LogAddTD("[Event Management] [Start] EledoradoEvent!");

	DWORD TickCount = GetTickCount();

	this->GoldGoblenEventStartTime = TickCount;
	this->RegenGoldGoblen();

	this->TitanEventStartTime = TickCount;
	this->RegenTitan();

	this->GoldDerconEventStartTime = TickCount;
	this->RegenGoldDercon();

	this->DevilLizardKingEventStartTime = TickCount;
	this->RegenDevilLizardKing();

	this->KanturEventStartTime = TickCount;
	this->RegenKantur();

	//Season 4.5 addon start
	this->GoldenRabbitRegenTime = TickCount;
	this->RegenGoldenRabbit();

	this->GoldenDarkKnightRegenTime = TickCount;
	this->RegenGoldenDarkKnight();

	this->GoldenDevilRegenTime = TickCount;
	this->RegenGoldenDevil();

	this->GoldenMonsterRegenTime = TickCount;
	this->RegenGoldenMonster();

	this->GoldenCrustRegenTime = TickCount;
	this->RegenGoldenCrust();

	this->GoldenSatirosRegenTime = TickCount;
	this->RegenGoldenSatiros();

	this->GoldenTwintailRegenTime = TickCount;
	this->RegenGoldenTwintail();

	this->GoldenIronKnightRegenTime = TickCount;
	this->RegenGoldenIronKnight();

	this->GoldenNeipinRegenTime = TickCount;
	this->RegenGoldenNeipin();

	this->GoldenGreatDragonRegenTime = TickCount;
	this->RegenGoldenGreatDragon();


}
void CEledoradoEvent::End_Menual()
{
	this->SetMenualStart(FALSE);
}

//00486F80 
void CEledoradoEvent::RegenGoldenRabbit()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 502 )	// fixed
		{
			gObj[n].Live = TRUE;
			MapNumber = 51;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden rabbit : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenGoldenDarkKnight()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 493 )	// 
		{
			gObj[n].Live = TRUE;
			MapNumber = 1;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden DarkKnight : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenGoldenDevil()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 494 )	// 
		{
			gObj[n].Live = TRUE;
			MapNumber = 4;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden Devil : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenGoldenMonster()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 495 )	// 
		{
			gObj[n].Live = TRUE;
			MapNumber = 33;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden DarkKnight : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenGoldenCrust()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 496 )	// 
		{
			gObj[n].Live = TRUE;
			MapNumber = 10;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden Crust : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenGoldenSatiros()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 497 )	// 
		{
			gObj[n].Live = TRUE;
			MapNumber = 37;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden Satiros : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenGoldenTwintail()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 498 )	// 
		{
			gObj[n].Live = TRUE;
			MapNumber = 38;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden Twintail : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenGoldenIronKnight()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 499 )	// 
		{
			gObj[n].Live = TRUE;
			MapNumber = 57;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden IronKnight : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenGoldenNeipin()
{
	int n=0;
	int MapNumber;

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 500 )	// 
		{
			gObj[n].Live = TRUE;
			MapNumber = 56;
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden Neipin : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}

void CEledoradoEvent::RegenGoldenGreatDragon()
{
	int n=0;
	int MapNumber;
	int Map[2] = { 37, 57 };

	for (n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( gObj[n].Class == 501 )	// 
		{
			gObj[n].Live = TRUE;
			MapNumber = Map[rand()%2];
			gObj[n].MapNumber = MapNumber;

			while ( gMSetBase.GetBoxPosition(MapNumber, 1, 1, 255, 255, gObj[n].X, gObj[n].Y) == 0 )
			{

			}

			gObj[n].Life = gObj[n].MaxLife;
			gObj[n].TX = gObj[n].X;
			gObj[n].TY = gObj[n].Y;
			gObj[n].MTX = gObj[n].X;
			gObj[n].MTY = gObj[n].Y;
			gObj[n].StartX = gObj[n].X;
			gObj[n].StartY = gObj[n].Y;


			gObj[n].m_State = 1;
			gObj[n].PathCount = 0;

			LogAddTD("Make Golden Great Dragon : %d, %d,%d",
				MapNumber, gObj[n].X, gObj[n].Y);
		}
	}
}