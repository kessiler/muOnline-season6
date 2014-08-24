//GameServer 1.00.77 JPN - Completed
//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include "DevilSquare.h"
#include "GameServer.h"
#include "LogProc.h"
#include "GameMain.h"
#include "TNotice.h"
#include "..\include\readscript.h"
#include "gObjMonster.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
//#include "GameServerAuth.h"
#include "..\common\winutil.h"
#include "CrywolfSync.h"
#include "IllusionTempleEvent.h"
#include "Event.h"
#include "MasterLevelSystem.h"
#include "BuffEffectSlot.h"
#ifdef WZQUEST
#include "QuestExpProgMng.h"
#endif
#ifdef __CUSTOMS__
#include "ResetSystem.h"
#endif

CDevilSquare g_DevilSquare;

unsigned int __stdcall DevilSquareThreadFunc(LPVOID p)
{
	CDevilSquare * pDevilSquare = (CDevilSquare *)p;
	return pDevilSquare->Run();
}

void DevilSquareProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{
	switch ( protoNum )
	{
#if(DEBUG_EVENT_COUNTER == 1)
		case 11: //Receive the EnterCount for BloodCastle on T Key
			EGAnsBloodCastleEnterCount((PMSG_ANS_BLOODCASTLE_ENTERCOUNT *)aRecv);
			break;
#endif
		case 18: //To enter Illusion Temple Increasing the EnterCount thru Ranking Server
			g_IllusionTempleEvent.EGAnsIllusionTempleEnterCountCheck((LPPMSG_ANS_ILLUSION_TEMPLE_ENTERCOUNTCHECK)aRecv);
			break;
		case 20: //Receive the EnterCount for IllusionTemple on T Key
			g_IllusionTempleEvent.EGAnsIllusionTempleEnterCount((PMSG_ANS_ILLUSIONTEMPLE_ENTERCOUNT *)aRecv);
			break;

	}
}

void DataSendRank(char* pMsg, int size)
{
	if ( IsDevilSquareEventConnected == FALSE && DevilSquareEventConnect == FALSE )
	{
		wsRServerCli.Close();
		wsRServerCli.CreateSocket(ghWnd);

		if ( GMRankingServerConnect(gDevilSquareEventServerIp, WM_GM_RANKING_CLIENT_MSG_PROC) == FALSE )
		{
			IsDevilSquareEventConnected = FALSE;
			LogAddTD("Can not connect Ranking Server");
			return;
		}

		IsDevilSquareEventConnected = TRUE;
	}

	if ( DevilSquareEventConnect == FALSE && IsDevilSquareEventConnected != FALSE )
	{
		wsRServerCli.DataSend(pMsg, size);
	}
}

CDevilSquare::CDevilSquare()
{
	this->m_bQuit = FALSE;
	this->m_iCloseTime = 2; //30
	this->m_iOpenTime = 2; //34
	this->m_iPlaytime = 10;

	//Dark Wizard until 4th Floor
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_1] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_2] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_3] = 0;
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_4] = 170;
	
	//Dark Knight until 4th Floor
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_1] = 20;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_2] = 90;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_3] = 120;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_4] = 400;

	//Elf until 4th Floor
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_1] = 10;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_2] = 10;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_3] = 10;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_4] = 200;

	//Magumsa until 4th Floor
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_1] = 0;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_2] = 0;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_3] = 0;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_4] = 0;

	//Dark Wizard 5th & 6th Floor
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_5] = 170;
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_6] = 170;

	//Dark Knight 5th & 6th Floor
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_5] = 400;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_6] = 400;

	//Elf 5th & 6th Floor
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_5] = 200;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_6] = 200;

	//Magumsa 5th & 6th Floor
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_5] = 0;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_6] = 0;

	//Dark Lord All Floors
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_1] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_2] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_3] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_4] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_5] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_6] = 0;

	//Season3 add-on (All Classes 7th Floor)
	this->m_BonusScoreTable[CLASS_WIZARD][DEVIL_SQUARE_GROUND_7] = 170;
	this->m_BonusScoreTable[CLASS_KNIGHT][DEVIL_SQUARE_GROUND_7] = 400;
	this->m_BonusScoreTable[CLASS_ELF][DEVIL_SQUARE_GROUND_7] = 200;
	this->m_BonusScoreTable[CLASS_MAGUMSA][DEVIL_SQUARE_GROUND_7] = 0;
	this->m_BonusScoreTable[CLASS_DARKLORD][DEVIL_SQUARE_GROUND_7] = 0;

	for(int i = 0;i < 7; i++)
	{
		(float&)this->m_BonusScoreTable[CLASS_SUMMONER][i] = 1.0f;
	}
}

CDevilSquare::~CDevilSquare()
{
	this->m_bQuit = TRUE;
}

void CDevilSquare::Init()
{
	this->m_bQuit = TRUE;
	Sleep(500);

	this->SetState(DevilSquare_CLOSE);
	this->m_bQuit = FALSE;

	DWORD dwThreadId;

	this->m_hThread = (UINT)_beginthreadex( 0, 0, DevilSquareThreadFunc, this, 0, (LPUINT)&dwThreadId);
}

void CDevilSquare::Load(char * filename)
{
	for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
	{
		this->m_DevilSquareGround[i].Init(i);
	}

	int Token;
	int number;
	int type;
	int monstertype;
	int starttime;
	int endtime;
	int x;
	int y;
	int tx;
	int ty;

	SMDFile = fopen(filename, "r");	//ok

	if ( SMDFile == NULL )
	{
		MsgBox("[DevilSquare] Info file Load Fail [%s]", filename);
		return;
	}

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		type = TokenNumber;

		while ( true )
		{
			if ( type == 0 )	// Time Sync
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				this->m_iCloseTime = TokenNumber;

				Token = GetToken();
				this->m_iOpenTime = TokenNumber;

				Token = GetToken();
				this->m_iPlaytime = TokenNumber;
			}
			else if ( type == 1 )	// Normal Monster Settings
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				number = TokenNumber;	// Devils Square Index;

				Token = GetToken();
				monstertype = TokenNumber;

				Token = GetToken();
				starttime = TokenNumber;

				Token = GetToken();
				endtime = TokenNumber;

				this->m_DevilSquareGround[number].Set(monstertype, starttime, endtime);
			}
			else if ( type == 2 )	// Boss Monster Set
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				number = TokenNumber;	// Devils Square Index;

				Token = GetToken();
				monstertype = TokenNumber;

				Token = GetToken();
				starttime = TokenNumber;

				Token = GetToken();
				x = TokenNumber;

				Token = GetToken();
				y = TokenNumber;

				Token = GetToken();
				tx = TokenNumber;

				Token = GetToken();
				ty = TokenNumber;

				this->m_DevilSquareGround[number].SetBoss(monstertype, starttime, x, y, tx, ty);
			}
			else if ( type == 3 ) // Bonus Sttings
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				int index;
				int rank;
				int zen;
				int exp;
				BYTE ItemCount;
				WORD ItemID;
				BYTE ItemLevel;

				index = TokenNumber;

				Token = GetToken();
				rank = TokenNumber;

				Token = GetToken();
				exp = TokenNumber;

				Token = GetToken();
				zen = TokenNumber;

#ifdef __CUSTOMS__
				Token = GetToken();
				ItemCount = TokenNumber;

				Token = GetToken();
				int ItemType = TokenNumber;

				Token = GetToken();
				int ItemIndex = TokenNumber;

				ItemID	= ITEMGET(ItemType,ItemIndex);

				Token = GetToken();
				ItemLevel = TokenNumber;

				this->m_DevilSquareGround[index].SetBonus(rank, exp, zen, ItemCount, ItemID, ItemLevel);
#else
				this->m_DevilSquareGround[index].SetBonus(rank, exp, zen);
#endif
			}
			//season 2.5 add-on
			else if ( type == 4 ) // Timer Settings (webzen stupid??) -> Identical
			{
				Token = GetToken();

				if ( strcmp("end", TokenString) == 0 )
				{
					break;
				}

				DEVILSQUARE_START_TIME Schedule;

				Schedule.m_iHour = TokenNumber;

				Token = GetToken();
				Schedule.m_iMinute = TokenNumber;

				this->m_listDevilSquareOpenTime.push_back(Schedule);
			}
			//Season 4.5 addon
			else if ( type == 5 ) // Exp Settings  -> Identical
			{
				Token = GetToken();
				
				if ( strcmp("end", TokenString) == 0 )
				{
					for(int i = 0;i < 7;i++)
					{
						this->m_DevilSquareGround[i].IncExp((float&)this->m_BonusScoreTable[CLASS_SUMMONER][i]);
					}

					break;
				}
				
				int level;
		
				level = TokenNumber;

				if(DS_LEVEL_RANGE(level))
				{					
					Token = GetToken();
					this->m_BonusScoreTable[CLASS_SUMMONER][level] = (int&)TokenNumber;
				}
				else
				{
					Token = GetToken();
				}
			}
		}
	}

	fclose (SMDFile );

	LogAdd("%s file load!", filename);
}

void CDevilSquare::SetState(enum eDevilSquareState eState)
{
	this->m_eState = eState;

	switch ( this->m_eState )
	{
		case DevilSquare_CLOSE:	this->SetClose();	break;
		case DevilSquare_OPEN:		this->SetOpen();	break;
		case DevilSquare_PLAYING:	this->SetPlaying();	break;
	}
}

void CDevilSquare::SetClose()
{
	if ( LogAdd != NULL )
	{
		LogAddTD("[DevilSquare] Close");
	}

	this->ClearMonstr();
	this->CalcScore();

	for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
	{
		this->m_DevilSquareGround[i].Clear();
	}

	this->m_iRemainTime = this->m_iCloseTime;
	this->CheckSync();
	this->m_bFlag = 0;

	if ( this->m_iRemainTime <= 1 )
	{
		this->m_iTime = GetTickCount();
		this->m_iremainTimeSec = 60;
	}
	else
	{
		this->m_iTime = GetTickCount() + 60000;
		this->m_iremainTimeSec = -1;
	}
	
	this->m_bSendTimeCount = FALSE;
}

void CDevilSquare::SetOpen()
{
	this->m_iRemainTime = this->m_iOpenTime;

	if ( this->m_iOpenTime <= 1 )
	{
		this->m_iTime = GetTickCount();
		this->m_iremainTimeSec = 60;
	}
	else
	{
		this->m_iTime = GetTickCount() + 60000;
		this->m_iremainTimeSec = -1;
	}

	this->m_bSendTimeCount = FALSE;
}

void CDevilSquare::SetPlaying()
{
	this->gObjScoreClear();
	this->SetMonster();

	this->m_iRemainTime = this->m_iPlaytime;

	if ( this->m_iPlaytime <= 1 )
	{
		this->m_iTime = GetTickCount();
		this->m_iremainTimeSec = 60;
	}
	else
	{
		this->m_iTime = GetTickCount() + 60000;
		this->m_iremainTimeSec = -1;
	}
	
	this->m_bSendTimeCount = FALSE;
}

int CDevilSquare::GetDevilSquareIndex(int iGateNumber)
{
	switch ( iGateNumber )
	{
		case 58:
		case 59:
		case 60:
		case 61:
			return iGateNumber - 58;	// Devil 1 - 4
			break;

		case 111:
		case 112:
			return iGateNumber - 107;	// Devil 5 - 6
			break;

		case 270:
			return ( MAX_DEVILSQUARE_GROUND - 1 ); // Devil 7
			break;

		default:
			return -1;
			break;
	}
}

int CDevilSquare::GetUserLevelToEnter(int iUserIndex, int& iMoveGate) //0046A880
{
	iMoveGate = (BYTE)-1;

	if ( gObjIsConnected(iUserIndex) == FALSE )
	{
		return -1;
	}

	int iENTER_LEVEL = -1;

	// Original, with master level from 400
	//if(g_MasterLevelSystem.IsMasterLevelUser(&gObj[iUserIndex]) != FALSE)
	//{
	//	iENTER_LEVEL = MAX_DEVILSQUARE_GROUND - 1;
	//	iMoveGate = g_sttDEVILSQUARE_LEVEL[iENTER_LEVEL].MoveGate;
	//}
	if( gObj[iUserIndex].ThirdChangeUp > 0 )
	{
		iENTER_LEVEL = MAX_DEVILSQUARE_GROUND - 1;
		iMoveGate = g_sttDEVILSQUARE_LEVEL[iENTER_LEVEL].MoveGate;
	}
	else
	{
		for ( int i=0;i<MAX_DEVILSQUARE_GROUND-1;i++)
		{
			if ( gObj[iUserIndex].Class == CLASS_DARKLORD
#ifdef MONK
				|| gObj[iUserIndex].Class == CLASS_MONK
#endif
				|| gObj[iUserIndex].Class == CLASS_MAGUMSA )
			{
				if ( gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].SpecialCharacterMinLevel  && gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].SpecialCharacterMaxLevel )
				{
					iENTER_LEVEL = i;
					iMoveGate = g_sttDEVILSQUARE_LEVEL[i].MoveGate;
					break;
				}
			}
			else
			{
				if ( gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].NormalCharacterMinLevel  && gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].NormalCharacterMaxLevel )
				{
					iENTER_LEVEL = i;
					iMoveGate = g_sttDEVILSQUARE_LEVEL[i].MoveGate;
					break;
				}
			}
		}
	}

	return iENTER_LEVEL;
}

BOOL CDevilSquare::Run()
{
	int count = 0;

	while ( this->m_bQuit == FALSE )
	{
		if ( gDevilSquareEvent != FALSE )
		{
			switch ( this->m_eState )
			{
				case DevilSquare_CLOSE:	this->ProcClose();		break;
				case DevilSquare_OPEN:		this->ProcOpen();		break;
				case DevilSquare_PLAYING:	this->ProcPlaying();	break;
			}
		}

		Sleep(100);
		count++;

		if ( count > 600 )
		{
			this->SendLiveCall();
			count = 0;

			if ( szAuthKey[12] != AUTHKEY12 )
			{
				DestroyGIocp();
			}
		}
	}

	return TRUE;
}

void CDevilSquare::CheckSync()
{
	std::list<DEVILSQUARE_START_TIME>::iterator it; //loc2 -> ebp 4
	DEVILSQUARE_START_TIME WebzenVar1; //loc 3-4 -> ebp C
	DEVILSQUARE_START_TIME WebzenVar2; //loc 5-6 -> epb 14
	int BaseTime = 0; // loc7 -> ebp 18
	int CheckTime = 0; // loc8 -> ebp 1C
	DWORD CurrentTime = 0;	// loc9 -> ebp 20

	if(this->m_listDevilSquareOpenTime.size() == 0)
	{
		return;
	}

	tm * today; //loc10 -> ebp 24
	time_t ltime; //loc11 -> ebp 28

	time(&ltime); //
	today = localtime(&ltime); //

	int loc12; //loc12 -> ebp 30
	CurrentTime = (today->tm_hour * 60) + today->tm_min;
	WebzenVar1 = *m_listDevilSquareOpenTime.begin();

	for( it = m_listDevilSquareOpenTime.begin(); it != m_listDevilSquareOpenTime.end(); ++it ) //for identical
	{
		WebzenVar2 = *it; // loc5-6
		BaseTime = (WebzenVar1.m_iHour * 60) + WebzenVar1.m_iMinute;
		CheckTime =	(WebzenVar2.m_iHour * 60) + WebzenVar2.m_iMinute;

		if( BaseTime == CheckTime )
		{
			if( CurrentTime < CheckTime )
			{
				WebzenVar2 = *it;
				break;
			}
			continue;
		}
		
		if( CurrentTime >= BaseTime && CurrentTime < CheckTime )
		{
			break;
		}
		else
		{
			WebzenVar1 = *it;
		}
	}

	for(loc12 = 2;loc12--;) //good ->func identical so far
	{
		if(it == m_listDevilSquareOpenTime.end())
		{
			it = m_listDevilSquareOpenTime.begin();

			WebzenVar2 = (*it);
		}

		CheckTime = WebzenVar2.m_iHour*60+WebzenVar2.m_iMinute;

		if(today->tm_hour <= WebzenVar2.m_iHour)
		{
			this->m_iRemainTime = CheckTime - CurrentTime;
		}
		else
		{
			this->m_iRemainTime = 1440-CurrentTime+CheckTime;
		}

		if(this->m_iRemainTime <= this->m_iOpenTime)
		{
			it++;

			if(it != this->m_listDevilSquareOpenTime.end())
			{
				WebzenVar2 = (*it);
			}
		}
		else
		{
			this->m_iRemainTime -= this->m_iOpenTime;
			break;
		}
	}

	LogAdd("[DevilSquare] Sync Open Time. [%d]min remain", m_iRemainTime);
}

int CDevilSquare::CheckCloseTime()
{
	std::list<DEVILSQUARE_START_TIME>::iterator it; //loc2 -> ebp 4
	DEVILSQUARE_START_TIME WebzenVar1; //loc 3-4 -> ebp C
	DEVILSQUARE_START_TIME WebzenVar2; //loc 5-6 -> epb 14
	int BaseTime = 0; // loc7 -> ebp 18
	int CheckTime = 0; // loc8 -> ebp 1C
	DWORD CurrentTime = 0;	// loc9 -> ebp 20
	
	tm * today; //loc10 -> ebp 24
	time_t ltime; //loc11 -> ebp 28

	int loc12 = 0; //loc12 -> ebp 30
	int loc13;

	if(this->m_listDevilSquareOpenTime.size() == 0)
	{
		return 0;
	}

	time(&ltime); //
	today = localtime(&ltime); //

	
	CurrentTime = (today->tm_hour * 60) + today->tm_min;
	WebzenVar1 = *m_listDevilSquareOpenTime.begin();

	for( it = m_listDevilSquareOpenTime.begin(); it != m_listDevilSquareOpenTime.end(); ++it ) //for identical
	{
		WebzenVar2 = *it; // loc5-6
		BaseTime = (WebzenVar1.m_iHour * 60) + WebzenVar1.m_iMinute;
		CheckTime =	(WebzenVar2.m_iHour * 60) + WebzenVar2.m_iMinute;

		if( BaseTime == CheckTime )
		{
			if( CurrentTime < CheckTime )
			{
				WebzenVar2 = *it;
				break;
			}
			continue;
		}
		
		if( CurrentTime >= BaseTime && CurrentTime < CheckTime )
		{
			break;
		}
		else
		{
			WebzenVar1 = *it;
		}
	}

	for(loc13 = 2;loc13--;) //good ->func identical so far
	{
		if(it == m_listDevilSquareOpenTime.end())
		{
			it = m_listDevilSquareOpenTime.begin();

			WebzenVar2 = (*it);
		}

		CheckTime = WebzenVar2.m_iHour*60+WebzenVar2.m_iMinute;

		if(today->tm_hour <= WebzenVar2.m_iHour)
		{
			loc12 = CheckTime - CurrentTime;
		}
		else
		{
			loc12 = 1440-CurrentTime+CheckTime;
		}

		if(loc12 <= this->m_iOpenTime)
		{
			it++;

			if(it != this->m_listDevilSquareOpenTime.end())
			{
				WebzenVar2 = (*it);
			}
		}
		else
		{
			loc12 -= this->m_iOpenTime;
			break;
		}
	}

	return loc12;
}

void CDevilSquare::ProcClose()
{
	if ( this->m_iremainTimeSec != -1 )
	{
		int lc2 = (GetTickCount() - this->m_iTime)/1000;

		if ( lc2 != 0 )
		{
			this->m_iremainTimeSec -= lc2;
			this->m_iTime += lc2 * 1000;

			if ( this->m_iremainTimeSec <= 30 && this->m_bSendTimeCount == FALSE )
			{
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = DevilSquare_CLOSE;

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
							}
						}
					}
				}

				this->m_bSendTimeCount = TRUE;
			}		

			if (this->m_iremainTimeSec < 1 )
			{
				PMSG_NOTICE pNotice;

				TNotice::MakeNoticeMsg((TNotice *)&pNotice, 0, lMsg.Get(MSGGET(2, 191)));

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
							}
						}
					}
				}

				LogAddTD(pNotice.Notice);
				this->SetState(DevilSquare_OPEN);
			}
		
		}
	}
	else if ( this->m_iTime < GetTickCount() )
	{
		this->ClearMonstr();
		this->m_iRemainTime--;

		if ( this->m_bFlag == 0 )
		{
			for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
			{
				if ( gObj[i].Connected == PLAYER_PLAYING )
				{
					if ( DS_MAP_RANGE(gObj[i].MapNumber) != FALSE )
					{
						gObjMoveGate(i, 27);
					}
				}
			}

			this->ItemClear();
		}
		else
		{
			this->CheckInvalidUser();
		}

		if ( this->m_iRemainTime <= 15 )
		{
			if ( (this->m_iRemainTime%5)== 0 )
			{
				PMSG_NOTICE pNotice;

				TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 0, lMsg.Get(MSGGET(2, 192)), this->m_iRemainTime);

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
							}
						}
					}
				}

				LogAddTD((char*)pNotice.Notice);
			}
		}

		if ( this->m_iRemainTime == 1 && this->m_iremainTimeSec == -1 )
		{
			this->m_iremainTimeSec = 60;
			this->m_iTime = GetTickCount();
		}
		else
		{
			this->m_iTime = GetTickCount() + 60000;
		}
	}
}

void CDevilSquare::ProcOpen()
{
	if ( this->m_iremainTimeSec != -1 )
	{

		int lc2 = (GetTickCount() - this->m_iTime)/1000;

		if ( lc2 != 0 )
		{
			this->m_iremainTimeSec -= lc2;
			this->m_iTime += lc2 * 1000;

			if ( this->m_iremainTimeSec <= 30 && this->m_bSendTimeCount == FALSE )
			{
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = DevilSquare_OPEN;

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
							}
						}
					}
				}

				this->m_bSendTimeCount = TRUE;
			}

			if (this->m_iremainTimeSec < 1 )
			{
				this->SendEventStartMsg();
				this->SetState(DevilSquare_PLAYING);
			}
		
		}
	}
	else
	{
		if ( this->m_iTime < GetTickCount() )
		{
			this->ClearMonstr();
			this->m_iRemainTime--;

			if ( this->m_iRemainTime > 0 )
			{
				PMSG_NOTICE pNotice;

				TNotice::MakeNoticeMsgEx(&pNotice, 0, lMsg.Get(MSGGET(2, 193)), this->m_iRemainTime);

				for ( int i=OBJ_STARTUSERINDEX;i<OBJMAX;i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
							}
						}
					}
				}

				LogAddTD((char*)pNotice.Notice);

				this->m_iTime = GetTickCount();
			}

			if (this->m_iRemainTime == 1 && this->m_iremainTimeSec == -1)
			{
				this->m_iremainTimeSec = 60;
				this->m_iTime = GetTickCount();
			}
			else
			{
				this->m_iTime = GetTickCount() + 60000;
			}
		}
	}
}

void CDevilSquare::ProcPlaying()
{
	if ( this->m_iremainTimeSec != -1 )
	{
		int lc2 = (GetTickCount() - this->m_iTime)/1000;

		if ( lc2 != 0 )
		{
			this->m_iremainTimeSec -= lc2;
			this->m_iTime += lc2 * 1000;

			if ( this->m_iremainTimeSec <= 30 && this->m_bSendTimeCount == FALSE )
			{
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = DevilSquare_PLAYING;

				for(int i = 0; i < OBJMAX; i++)
				{
					if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if(DS_MAP_RANGE(gObj[i].MapNumber) != FALSE)
						{
							DataSend(i, (LPBYTE)&pMsg, sizeof(pMsg));
						}
					}
				}
				
				this->m_bSendTimeCount = TRUE;
			}

			if ( this->m_iremainTimeSec < 1 )
			{
				this->SetState(DevilSquare_CLOSE);
			}
		}
	}
	else
	{
		if ( this->m_iTime < GetTickCount() )
		{
			this->m_iRemainTime--;

			if ( this->m_iRemainTime > 0 )
			{
				if ( (this->m_iRemainTime%5) == 0 )
				{
					PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 0, lMsg.Get(MSGGET(2, 194)), this->m_iRemainTime);
					LogAddTD((char*)pNotice.Notice);
					AllSendSameMapMsg((LPBYTE)&pNotice, pNotice.h.size, MAP_INDEX_DEVILSQUARE);
				}

				for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
				{
					this->m_DevilSquareGround[i].RegenBossMonster(this->m_iPlaytime - this->m_iRemainTime);
				}

				if ( this->m_iRemainTime == 1 && this->m_iremainTimeSec == -1 )
				{
					this->m_iremainTimeSec = 60;
					this->m_iTime = GetTickCount();
				}
				else
				{
					this->m_iTime = GetTickCount() + 60000;
				}
			}
		}
	}
}

void CDevilSquare::SetMonster()
{
	int result;

	for ( int n=0;n<gMSetBase.m_Count;n++)
	{
		if ( DS_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			WORD wMonIndex = gMSetBase.m_Mp[n].m_Type;
			BYTE btDSIndex = -1;

			if ( gMSetBase.m_Mp[n].m_X == 119 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_1;
			}
			else if ( gMSetBase.m_Mp[n].m_X == 121 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_2;
			}
			else if ( gMSetBase.m_Mp[n].m_X == 49 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_3;
			}
			else if ( gMSetBase.m_Mp[n].m_X == 53 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_4;
			}
			else if ( gMSetBase.m_Mp[n].m_X == 120 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_5;
			}
			else if ( gMSetBase.m_Mp[n].m_X == 122 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_6;
			}
			else if ( gMSetBase.m_Mp[n].m_X == 50 )
			{
				btDSIndex = DEVIL_SQUARE_GROUND_7;
			}

			if ( btDSIndex == 0xFF )
			{
				LogAddTD("[DevilSquare] [%d] Invalid MonterType", wMonIndex);
				continue;
			}

			result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

			if ( result >= 0 )
			{
				gObjSetPosMonster(result, n);
				gObjSetMonster(result, wMonIndex); 
				gObj[result].MaxRegenTime = 1000;	// Rgeneration in 1 second
				gObj[result].m_bDevilSquareIndex = btDSIndex;
			}
		}
	}
}

void CDevilSquare::ClearMonstr()
{
	for ( int n=0;n<OBJ_MAXMONSTER;n++)
	{
		if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
		{
			gObjDel(n);
		}
	}
}

void CDevilSquare::gDevilSquareMonsterRegen(LPOBJ lpObj)
{
	BYTE devilsquareindex = lpObj->m_bDevilSquareIndex;

	if ( devilsquareindex < 0 || devilsquareindex >= MAX_DEVILSQUARE_GROUND )
	{
		LogAddTD("[DevilSquare] Invalid DevilSquareIndex [%d]", devilsquareindex);
		return;
	}

	if ( lpObj->m_PosNum == -1 )
	{
		gObjDel(lpObj->m_Index);
		return;
	}

	WORD monstertype = this->m_DevilSquareGround[devilsquareindex].GetMonsterType(this->m_iPlaytime - this->m_iRemainTime );

	if ( monstertype == (BYTE)-1 )
	{
		LogAddTD("[DevilSquare] [%d] Invalid MonterType", monstertype);
		return;
	}

	for ( int n=0;n<MAX_MAGIC;n++)
	{
		lpObj->Magic[n].Clear();
	}

	gObjSetMonster(lpObj->m_Index, monstertype);
	lpObj->DieRegen = FALSE;
	gObjMonsterRegen(lpObj);
	CreateFrustrum(lpObj->X, lpObj->Y, lpObj->m_Index);
	lpObj->m_bDevilSquareIndex = devilsquareindex;
	lpObj->MaxRegenTime = 1000;

	LogAddTD("[DevilSquare] Monter Regen [%d][%d][%d,%d]", monstertype, devilsquareindex, lpObj->X, lpObj->Y);
}

void CDevilSquare::SendEventStartMsg()
{
	PMSG_NOTICE pToEventer;
	PMSG_NOTICE pWithOutEvneter;

	TNotice::MakeNoticeMsg(&pToEventer, 0, lMsg.Get(MSGGET(2, 195)));
	TNotice::MakeNoticeMsg(&pWithOutEvneter, 0, lMsg.Get(MSGGET(2, 196)));

	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				DataSend(n, (LPBYTE)&pToEventer, pToEventer.h.size);
			}
			else
			{
				DataSend(n, (LPBYTE)&pWithOutEvneter, pWithOutEvneter.h.size);
			}
		}
	}

	LogAddTD("[DevilSquare] Start Event");
}

void CDevilSquare::DieProcDevilSquare(LPOBJ lpObj)
{
	char msg[255];

	wsprintf(msg, lMsg.Get(MSGGET(2, 197)), lpObj->m_nEventScore);
	GCServerMsgStringSend(msg, lpObj->m_Index, 1);

	if ( lpObj->m_nEventScore <= 0 )
	{
		return;
	}

	PMSG_ANS_EVENTUSERSCORE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x01;
	pMsg.h.size = sizeof(pMsg);
	pMsg.SquareNum = lpObj->m_bDevilSquareIndex;
	pMsg.Class = lpObj->Class;

	if ( lpObj->Class == 1 )
	{
		if ( lpObj->Class >= 0 && lpObj->Class < MAX_TYPE_PLAYER )
		{
			if ( DS_LEVEL_RANGE(lpObj->m_bDevilSquareIndex) != FALSE )
			{
				lpObj->m_nEventScore+= this->m_BonusScoreTable[lpObj->Class][lpObj->m_bDevilSquareIndex]/100;
				
			}
		}

		pMsg.Score = lpObj->m_nEventScore;
	}
	else
	{
		pMsg.Score = lpObj->m_nEventScore;
	}

	pMsg.ServerCode = gGameServerCode;
	memcpy(pMsg.AccountID, lpObj->AccountID, sizeof(pMsg.AccountID));
	memcpy(pMsg.GameID, lpObj->Name, sizeof(pMsg.GameID));

	DataSendRank((char*)&pMsg, pMsg.h.size);

	LogAddTD("[DevilSquare] Dead [%s][%s][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->m_nEventExp, lpObj->m_nEventScore);

	lpObj->m_nEventScore = 0;
	lpObj->m_nEventMoney = 0;
	lpObj->m_nEventExp = 0;

}

int CDevilSquare::gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg)
{
	__int64 exp;
	__int64 maxexp = 0;

	int level = ((lpTargetObj->Level + 25) * lpTargetObj->Level) / 3;

	if ( (lpTargetObj->Level + 10) < lpObj->Level )
	{
		level = (level*(lpTargetObj->Level + 10))/lpObj->Level;
	}

	if ( lpTargetObj->Level >= 65 )
	{
		level += (lpTargetObj->Level-64)*(lpTargetObj->Level / 4);
	}

	if ( level > 0 )
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if ( maxexp < 1 )
	{
		exp = level;
	}
	else
	{
		exp = level + rand()%maxexp;
	}

	exp = (dmg * exp)/tot_dmg;
	DWORD mymaxexp = gLevelExperience[lpObj->Level];

	if ( exp > mymaxexp )
	{
		exp = mymaxexp;
	}

	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) == FALSE)
	{
#ifdef __CUSTOMS__
		exp = int(exp * g_ResetSystem.GetDynamicExp(lpObj));
#else
		exp = int(exp * gAddExperience);
#endif
	}
	
#ifdef PCBANG
	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) == FALSE)//Season 4.5 addon
	{
		g_PCBangPointSystem.AddExperience(lpObj,exp);
	}
#endif

	if ( g_CrywolfSync.GetOccupationState() == 1 && g_iCrywolfApplyMvpPenalty != FALSE)
	{
		exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
	}

	exp +=  (exp * g_MAP_SETTINGS[lpObj->MapNumber].exp_increase) / 100;

	if ( exp > 0 )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			CheckItemOptForGetExpEx(lpObj, exp, FALSE); //Seal Exp (Season3 add-on)
		
			lpObj->Experience += exp;
			lpObj->m_nEventExp += exp;

			if ( gObjLevelUp(lpObj, exp, lpTargetObj->Class, 0) == false )
			{
				return 0;
			}
		}
	}

	return exp;
}

void CDevilSquare::gObjExpParty(LPOBJ lpObj, LPOBJ lpTargetObj, int AttackDamage, BOOL MSBFlag)
{
	int n;

	__int64 exp;
	__int64 maxexp = 0;
	__int64 totalexp;

	int level = ((lpTargetObj->Level + 25) * lpTargetObj->Level) / 3;
	int number;
	int partynum = 0;
	int totallevel = 0;
	int partylevel;
	int partycount;
	int dis[MAX_USER_IN_PARTY];
	int viewplayer = 0;
	int viewpercent = 100;
	BOOL bApplaySetParty = FALSE;
	bool bCheckSetParty[MAX_TYPE_PLAYER];
	partynum = lpObj->PartyNumber;
	LPOBJ lpPartyObj;
	
	int toplevel = 0;

	for (n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if ( number >= 0 )
		{
			lpPartyObj = &gObj[number];

			int dis = gObjCalDistance(lpTargetObj,lpPartyObj); //Season 2.5 add-on

			if(dis < 10) //Season 2.5 add-on
			{
				if ( lpPartyObj->Level+lpPartyObj->m_nMasterLevel > toplevel )//Season 4.5 fix
				{
					toplevel = lpPartyObj->Level+lpPartyObj->m_nMasterLevel;//Season 4.5 fix
				}
			}
		}
	}

	if ( OBJMAX_RANGE(partynum) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

	partycount = gParty.m_PartyS[partynum].Count;

	for ( n =0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if ( number >= 0 )
		{
			lpPartyObj = &gObj[number];

			if ( lpTargetObj->MapNumber ==lpPartyObj->MapNumber )
			{
				dis[n] = gObjCalDistance(lpTargetObj, &gObj[number]);

				if ( dis[n] < 10 )
				{
					lpPartyObj = &gObj[number];

					if ( toplevel >= (lpPartyObj->Level + lpPartyObj->m_nMasterLevel + 200 ) ) // Season 4.5 fix
					{
						totallevel += lpPartyObj->Level + lpPartyObj->m_nMasterLevel + 200;//Season 4.5 fix
					}
					else
					{
						totallevel += lpPartyObj->Level + lpPartyObj->m_nMasterLevel; // Season 4.5 fix
					}

					viewplayer++;
					bCheckSetParty[lpPartyObj->Class] = true;
				}
			}
		}
	}

	if ( bCheckSetParty[0] != false && bCheckSetParty[1] != false && bCheckSetParty[2] != false 
		|| bCheckSetParty[3] != false && bCheckSetParty[4] != false && bCheckSetParty[5] != false )//Season 4.5 addon
	{
		bApplaySetParty = TRUE;
	}

	if ( viewplayer > 1 )
	{
		if ( bApplaySetParty != FALSE )
		{
			if ( viewplayer == 3 )
			{
				viewpercent = 230;
			}
			else if ( viewplayer == 4 )
			{
				viewpercent = 270;
			}
			else if ( viewplayer >= 5 )
			{
				viewpercent = 300;
			}
			else
			{
				viewpercent = 120;
			}
		}
		else
		{
			if ( viewplayer == 2 )
			{
				viewpercent = 160;
			}
			else if ( viewplayer == 3 )
			{
				viewpercent = 180;
			}
			else if ( viewplayer == 4 )
			{
				viewpercent = 200;
			}
			else if ( viewplayer >= 5 )
			{
				viewpercent = 220;
			}
			else
			{
				viewpercent = 120;
			}
		}

		partylevel = totallevel / viewplayer;
	}
	else
	{
		partylevel = totallevel;
	}

	if ( (lpTargetObj->Level +10) < partylevel )
	{
		level = (level * (lpTargetObj->Level+10) ) / partylevel;
	}

	if ( lpTargetObj->Level >= 65 )
	{
		if ( viewplayer == 1 )
		{
			level += ((lpTargetObj->Level+lpTargetObj->m_nMasterLevel) - 64) * (lpTargetObj->Level/ 4);
		}
		else
		{
			level += (200.0 - ((lpObj->Level + lpObj->m_nMasterLevel) * 0.2));
		}
	}

	if ( level > 0 )
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if ( maxexp < 1 )
	{
		totalexp = level;
	}
	else
	{
		totalexp = level + rand()%maxexp;
	}

	if( lpTargetObj->Type == OBJ_MONSTER )
	{
		lpTargetObj->Money = totalexp;
	}

	for ( n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if ( number >= 0 )
		{
			lpPartyObj = &gObj[number];

			if ( lpTargetObj->MapNumber == lpPartyObj->MapNumber )
			{
				if ( dis[n] < 10 )
				{
					__int64 myexp = 0;//Season 4.5 changed
					
					//Season 4.5 addon start
					if(g_MasterLevelSystem.CheckMLGetExp(lpPartyObj, lpTargetObj) == 0)
					{
						exp = 0;
					}
					else 
					{
						if(g_MasterLevelSystem.IsMasterLevelUser(lpPartyObj) != 0)
						{
							myexp = lpPartyObj->m_i64NextMasterLevelExp;
						}
						else
						{
							myexp = gLevelExperience[lpPartyObj->Level];
						}

						exp = ((totalexp * viewpercent* (lpPartyObj->Level + lpPartyObj->m_nMasterLevel) ) / totallevel ) / 100;
					}
					//Season 4.5 addon end

					if ( exp > myexp  )
					{
						exp = myexp;
					}

					if( lpPartyObj->Type == OBJ_USER )
					{
						if ( lpTargetObj->Type == OBJ_USER )
						{
							exp = 0;
						}
					}

					if(g_MasterLevelSystem.IsMasterLevelUser(lpPartyObj) == FALSE)
					{
#ifdef __CUSTOMS__
						exp = int(exp * g_ResetSystem.GetDynamicExp(lpPartyObj));
#else
						exp = int(exp * gAddExperience);
#endif
					}
					
#ifdef PCBANG
					if(g_MasterLevelSystem.IsMasterLevelUser(lpPartyObj) == FALSE)//Season 4.5 addon
					{
						g_PCBangPointSystem.AddExperience(lpPartyObj,exp);
					}
#endif

					if ( g_CrywolfSync.GetOccupationState() == 1 && g_iCrywolfApplyMvpPenalty != FALSE)
					{
						exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
					}
					
					exp +=  (exp * g_MAP_SETTINGS[lpObj->MapNumber].exp_party_increase) / 100;

					if ( exp > 0 )
					{
						if ( lpPartyObj->Type == OBJ_USER )
						{
							CheckItemOptForGetExpEx(lpPartyObj, exp, FALSE); //Seal Exp (Season3 add-on)

							lpPartyObj->Experience += exp;
							lpPartyObj->m_nEventExp += exp;

							if ( gObjLevelUp(lpPartyObj, exp, lpTargetObj->Class, EVENT_TYPE_PARTY) == false )
							{
								continue;
							}
						}
					}

					if ( lpPartyObj->Type == OBJ_USER )
					{
						GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
					}
				}
			}
		}
	}
}

void CDevilSquare::gObjMonsterScoreDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, BOOL MSBFlag) //0046F9D0 identical gs-cs 56
{
	::gObjMonsterHitDamageUserDel(lpMonObj);
	lpMonObj->Money = 0;

	LPOBJ lpTargetObj;
	int HitIndex;
	int LastHitObjNum = ::gObjMonsterLastHitDamageUser(lpMonObj, HitIndex);

	if ( LastHitObjNum != -1 )
	{
		lpTargetObj = &gObj[LastHitObjNum];
		int lc5 = lpMonObj->sHD[HitIndex].HitDamage / lpMonObj->MaxLife * lpMonObj->Level;
		lc5 *= lpTargetObj->m_bDevilSquareIndex + 1;
		lpTargetObj->m_nEventScore += lc5;
#ifdef WZQUEST
		g_QuestExpProgMng.ChkUserQuestTypeEventMap(260, lpTargetObj, lpTargetObj->m_bDevilSquareIndex, 2);
#endif
	}
}

void CDevilSquare::ItemClear()
{
	int CurTime = GetTickCount();

	for ( int i=0;i<MAX_MAPITEM;i++)
	{
		MapC[MAP_INDEX_DEVILSQUARE].m_cItem[i].m_State = 8;
	}
}

void CDevilSquare::CalcScore()
{
	for ( int n = 0; n < MAX_DEVILSQUARE_GROUND; n++ )
	{
		this->m_DevilSquareGround[n].ClearScore();
	}

	for ( int n = OBJ_STARTUSERINDEX; n < OBJMAX; n++ )
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				if ( DS_LEVEL_RANGE(gObj[n].m_bDevilSquareIndex) != FALSE )
				{
					this->m_DevilSquareGround[gObj[n].m_bDevilSquareIndex].InsertObj(&gObj[n]);
				}
			}
		}
	}

	for ( int n = 0; n < MAX_DEVILSQUARE_GROUND; n++ )
	{
		this->m_DevilSquareGround[n].SortScore();
		this->m_DevilSquareGround[n].SendScore();
	}
}

struct PMSG_ANS_EVENTLIVE
{
	PBMSG_HEAD h;
};

void CDevilSquare::SendLiveCall()
{
	PMSG_ANS_EVENTLIVE pMsg;
	
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x03;
	pMsg.h.size = sizeof(pMsg);

	DataSendRank((char *)&pMsg, pMsg.h.size);
}

void CDevilSquare::gObjScoreClear()
{
	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				gObj[n].m_nEventScore = 0;
				gObj[n].m_nEventMoney = 0;
				gObj[n].m_nEventExp = 0;
			}
		}
	}
}

void CDevilSquare::CheckInvalidUser()
{
	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				if ( this->m_eState == DevilSquare_CLOSE )
				{
					LogAddC(2, "[DevilSquare] [%s][%s] Found user in DevilSquare [State:Close]", gObj[n].AccountID, gObj[n].Name);
					gObjUserKill(gObj[n].m_Index);
				}
			}
		}
	}
}

//00494e50	-> 100%
int CDevilSquare::DelUser(int nDevilSquareIndex, int UserIndex)	//1.01.00
{
	if( !DS_LEVEL_RANGE(nDevilSquareIndex) )
	{
		return false;
	}
	// ----
	m_DevilSquareGround->DelUser(UserIndex);
}

//00494fa0	-> 100%
int CDevilSquare::LeaveDevilSquare(int nDevilSquareIndex, int UserIndex)	//1.01.00
{
	return this->DelUser(nDevilSquareIndex, UserIndex);
}