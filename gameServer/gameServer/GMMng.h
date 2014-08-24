#ifndef GMMNG_H
#define GMMNG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_GM_COMMAND 450
#define MAX_GMCOMMAND_LEN 25
#define MAX_GAME_MASTER 50
#define MAX_SUB_NAME	5


#include "user.h"

enum COMMAND_INDEX
{
	COMMAND_MAKEBLESS			=	0,
	COMMAND_MAKESOUL			=	1,
	COMMAND_RANDOMSETITEM		=	2,
	COMMAND_SETITEM				=	3,
	COMMAND_RESETPK				=	4,
	COMMAND_VIEWCLASSHP			=	5,
	COMMAND_NOTICEALL			=	6,
	COMMAND_SETPKLEVEL			=	7,
	COMMAND_MAKEITEMBYNAME		=	8,
	COMMAND_FINDITEMBYNAME		=	9,
	COMMAND_SETSTATS			=	10,
	COMMAND_SETSTATSNULLEXP		=	11,
	COMMAND_UPDBCLASS			=	12,
	COMMAND_UPCLASS				=	13,
	COMMAND_RESETCLASS			=	14,
	COMMAND_MAKEITEMS			=	15,
	COMMAND_SETMONEY			=	16,
	COMMAND_MODIFYFENDUR		=	17,
	COMMAND_MODIFYELFFENDUR		=	18,
	COMMAND_MODIFYSD			=	19,
	COMMAND_SHOWHPSD			=	20,
	COMMAND_SETOPTIONEFFECT		=	21,
	COMMAND_MAKEPRIZE			=	22,			
	COMMAND_MAKEITEM			=	23,
	COMMAND_MAKECHAOS			=	24,
	COMMAND_MAKELIFE			=	25,
	COMMAND_MAKECREATION		=	26,
	COMMAND_MONEYZEROSET		=	27,
	COMMAND_MASTERLEVEL			=	28,
	COMMAND_MLPOINT				=	29,
	COMMAND_MS_RESET			=	30,
	COMMAND_MAKESOCKETOPTION	=	31,
	COMMAND_REMAKESOCKETOPTION	=	32,
	COMMAND_CLEARSOCKETOPTION	=	33,
	COMMAND_FIRECRACK			=	34,
	COMMAND_SETWATCHINDEX		=	35,
	COMMAND_TRACK				=	36,
	COMMAND_USERSTATS			=	37,
	COMMAND_DISCONNECT			=	38,
	COMMAND_GDISCONNECT			=	39,
	COMMAND_MOVE				=	40,
	COMMAND_GMOVE				=	41,
	COMMAND_WARSTART			=	42,
	COMMAND_WARSTOP				=	43,
	COMMAND_WAREND				=	44,
	COMMAND_DISABLECHAT			=	45,
	COMMAND_ENABLECHAT			=	46,
	COMMAND_GUILDWAREND			=	47,
	COMMAND_GUILDWARSTART		=	48,
	COMMAND_GUILDWARSTOP		=	49,
	COMMAND_GUILDWAR			=	50,
	COMMAND_BATTLESOCCER		=	51,
	COMMAND_RE					=	52,
	COMMAND_REQUEST				=	53,
	COMMAND_SETDEFSTATS			=	54,
	COMMAND_SETDSMODE			=	55,
	COMMAND_GETINFARROWMP		=	56,
	COMMAND_CONTROLINFARRORMP0	=	57,
	COMMAND_CONTROLINFARRORMP1	=	58,
	COMMAND_CONTROLINFARRORMP2	=	59,
	COMMAND_CONTROLINFARRORMP3	=	60,
	COMMAND_SETINFARROWTIME		=	61,
	COMMAND_ADDEXP				=	62,
	COMMAND_MAKECHAOSCARD		=	63,
	COMMAND_ADDPCPOINT			=	64,
	COMMAND_SETPCPOINT			=	65,
	COMMAND_HIDEON				=	66,
	COMMAND_HIDEOFF				=	67,
	COMMAND_ABILITY				=	68,
	COMMAND_PARTY				=	69,
	COMMAND_SUMMONMONSTER		=	70,
	COMMAND_CLEARMONSTER		=	71,
	COMMAND_CLEARITEM			=	72,
	COMMAND_CLEARINVEN			=	73,
	COMMAND_SUMMONCHAR			=	74,
	COMMAND_SKIN				=	75,
	COMMAND_ADDSTR				=	76,
	COMMAND_ADDAGI				=	77,
	COMMAND_ADDVIT				=	78,
	COMMAND_ADDENE				=	79,
	COMMAND_ADDCOM				=	80,
	COMMAND_POST				=	81,
	COMMAND_WARP				=	82,
	COMMAND_ADDBUFF				=	83,
	COMMAND_CLEARBUFF			=	84,
	COMMAND_GIFTBUFF			=	85,
	COMMAND_TRANS				=	86,
	COMMAND_CHATBLOCK			=	87,
	COMMAND_CHATUNBLOCK			=	88,
	COMMAND_OFFTRADE			=	89,
	COMMAND_SETPARTYLEADER		=	90,	
#ifdef OFFEXP
	COMMAND_OFFEXP				=	91,	
#endif
	COMMAND_LOREN_MARKET		=	92,
};
class CGMCommand
{
public:
	CGMCommand()
	{
		this->Init();
	}

	void Init()
	{
		for ( int n = 0; n<  MAX_GM_COMMAND; n++ )
		{
			this->nCmdCode[0] = -1;
		}

		this->count=0;
	}

	void Add(LPSTR command_str, int command_code,int gamemaster_code,int command_active) 
	{
		strcpy(this->szCmd[this->count], command_str);
		this->nCmdCode[this->count]=command_code;
		this->nGMCode[this->count]=gamemaster_code;
		this->nCmdActive[this->count]=command_active;

		if ( this->count < MAX_GM_COMMAND-1 )
		{
			this->count++;
		}
	}


public:
	
	char szCmd[MAX_GM_COMMAND][MAX_GMCOMMAND_LEN];
	int nCmdCode[MAX_GM_COMMAND];
	int nGMCode[MAX_GM_COMMAND];
	int nCmdActive[MAX_GM_COMMAND];
	int count;
};

struct MANAGER_SUB_NAME
{
	int iAuthCode;
	char szSubName[30];
};

#pragma pack(push, 1)
struct CHAT_WHISPER_EX
{
	PBMSG_HEAD	Head;
	char		Name[10];
	char		Message[90];
	BYTE		Type;
};
#pragma pack(pop)


class CGMMng
{

public:

	CGMMng();
	virtual ~CGMMng();

	void Init();
	void LoadCommands(LPSTR szFileName);
	char * GetSubName(int auth_code);
	int GetCmd(LPSTR szCmd);
	BYTE GetData(char *szCmd,int &command_code,int &gamemaster_code);
	int ManagementProc(LPOBJ lpObj, LPSTR szCmd, int aIndex);
	LPSTR GetTokenString();
	int GetTokenNumber();
	BOOL CommandActive(int command_code);
	void GetInfinityArrowMPConsumption(LPOBJ lpObj);
	void ControlInfinityArrowMPConsumption0(LPOBJ lpObj, int iValue);
	void ControlInfinityArrowMPConsumption1(LPOBJ lpObj, int iValue);
	void ControlInfinityArrowMPConsumption2(LPOBJ lpObj, int iValue);
	void ControlInfinityArrowMPConsumption3(LPOBJ lpObj, int iValue);
	void SetInfinityArrowTime(LPOBJ lpObj, int iValue);
	void ControlFireScreamDoubleAttackDistance(LPOBJ lpObj, int iValue);
	void ManagerInit();
	int  ManagerAdd(LPSTR name, int aIndex);
	void ManagerDel(LPSTR name);
	void ManagerSendData(LPSTR szMsg, int size);
	void BattleInfoSend(LPSTR Name1, BYTE score1, LPSTR Name2, BYTE score2);
	void DataSend(LPBYTE szMsg, int size);
	void MessageAll(int Type, int Type2, char *Sender, char *Msg,...);
	void MsgYellow(int aIndex,char* szMsg,...);
	void CmdAbility(LPOBJ lpObj,char *szName);
	void CmdParty(LPOBJ lpObj,char *szName);
	void CmdSummonMonster(LPOBJ lpObj,char *szMonsterName,int MonsterCount);
	void CmdClearMonster(LPOBJ lpObj,int Dis);
	void CmdClearItem(LPOBJ lpObj,int Dis);
	void CmdClearInven(LPOBJ lpObj);
	void CmdSummonChar(LPOBJ lpObj,char * szName);
	int GetType(WORD wClass);
	void AddLevelUpPoints(int aIndex,unsigned long ulValue, short sType);
private:

	CGMCommand cCommand;
	char szManagerName[MAX_GAME_MASTER][11];
	char szSubName[MAX_SUB_NAME][30];
	int ManagerIndex[MAX_GAME_MASTER];

public:

	int WatchTargetIndex;
};

extern CGMMng cManager;

#endif