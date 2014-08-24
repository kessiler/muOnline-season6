#ifndef GUILDCLASS_H
#define GUILDCLASS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef unsigned char GUILDMARK[32];

#include "..\\include\\prodef.h"

#define MAX_USER_GUILD			80
#define MAX_GUILD_NOTICE_LEN	MAX_CHAT_LEN
#define MAX_WC_GUILD_NOTICE_LEN	(MAX_GUILD_NOTICE_LEN/2)
#define MIN_GUILD_COUNT_FOR_CS	1 // 20

// Member Type
// -------------------------------
#define GUILD_MASTER		0x80
#define GUILD_ASSISTANT		0x40
#define GUILD_BATTLE_MASTER	0x20
#define GUILD_NORMAL_USER	0x00

#define GUILD_RELATION_UNION	0x01
#define GUILD_RELATION_RIVAL	0x02


enum GUILD_NOTIFICATION
{
	G_NOTIFICATION_SET = 0x1,
	G_NOTIFICATION_UNION_BREAKOFF = 0x10,
};

enum GUILD_OPERATION_ERRORCODE
{
	G_ERROR_NONE = 0x0,
};

enum GUILD_RELATION_OPERATION
{
	G_RELATION_OPERATION_JOIN = 0x1,
	G_RELATION_OPERATION_BREAKOFF = 0x2,
};

enum GUILD_RELATIONSHIP {
	G_RELATIONSHIP_NONE = 0x0,
	G_RELATIONSHIP_UNION = 0x1,
	G_RELATIONSHIP_RIVAL = 0x2,
	G_RELATIONSHIP_UNIONMASTER = 0x4,
	G_RELATIONSHIP_RIVALMASTER = 0x8,
};

enum GUILD_TYPE
{
	G_TYPE_NONE = 0xff,
	G_TYPE_COMMON = 0x0,
	G_TYPE_GUARD = 0x1,
};

enum GUILD_STATUS
{
	G_NONE = 0xff,
	G_PERSON = 0x0,
	G_MASTER = 0x80,
	G_SUB_MASTER = 0x40,
	G_BATTLE_MASTER = 0x20,
};

enum GUILD_REQ_COMMON_RESULT
{
	GUILD_ANS_NOTEXIST_GUILD = 0x10,
	GUILD_ANS_UNIONFAIL_BY_CASTLE = 0x10,
	GUILD_ANS_NOTEXIST_PERMISSION = 0x11,
	GUILD_ANS_NOTEXIST_EXTRA_STATUS = 0x12,
	GUILD_ANS_NOTEXIST_EXTRA_TYPE = 0x13,
	GUILD_ANS_EXIST_RELATIONSHIP_UNION = 0x15,
	GUILD_ANS_EXIST_RELATIONSHIP_RIVAL = 0x16,
	GUILD_ANS_EXIST_UNION = 0x17,
	GUILD_ANS_EXIST_RIVAL = 0x18,
	GUILD_ANS_NOTEXIST_UNION = 0x19,
	GUILD_ANS_NOTEXIST_RIVAL = 0x1a,
	GUIDL_ANS_NOT_UNION_MASTER = 0x1b,
	GUILD_ANS_NOT_GUILD_RIVAL = 0x1c,
	GUILD_ANS_CANNOT_BE_UNION_MASTER = 0x1d,
	GUILD_ANS_EXCEED_MAX_UNION_MEMBER = 0x1e,
	GUILD_ANS_CANCLE_REQUEST = 0x20,
};


struct _GUILD_INFO_STRUCT
{
	int Number;	//0
	char Name[9];	//4
	unsigned char Mark[32];	//D
	BYTE Count;	//2D
	BYTE TotalCount;	//2E
	char Names[MAX_USER_GUILD][11];	//2F
	short Index[MAX_USER_GUILD];	//3A0
	unsigned char Use[MAX_USER_GUILD];	//440
	char pServer[MAX_USER_GUILD];	//490
	char TargetGuildName[9];	//4E0
	short TargetIndex[MAX_USER_GUILD];	//4EA
	struct _GUILD_INFO_STRUCT* lpTargetGuildNode;	//58C
	unsigned char WarDeclareState;	//590
	unsigned char WarState;	//591
	unsigned char WarType;	//592
	unsigned char BattleGroundIndex;	//593
	unsigned char BattleTeamCode;	//594
	unsigned char PlayScore;	//595
	int TotalScore;	//598
	char Notice[MAX_CHAT_LEN];	//59c
	int GuildStatus[MAX_USER_GUILD];	//5D8
	BYTE btGuildType;	//718
	int iGuildUnion;	//71C
	int iGuildRival;	//720
	int iTimeStamp;	//724
	char szGuildRivalName[9];	//728

//#if (_GSCS==1)

	struct OBJECTSTRUCT * lpLifeStone;
	int btLifeStoneCount;

//#endif

	struct _GUILD_INFO_STRUCT* back;	//734
	struct _GUILD_INFO_STRUCT* next;	//738



	// #error Deathay Fix here
	_GUILD_INFO_STRUCT(){return;};
	int  GetGuildUnion(){return this->iGuildUnion;};
	int  GetGuildRival(){return this->iGuildRival;};


	void SetGuildUnion(int iGuildNumber);	// line : 102
	/*{
		this->iGuildUnion = iGuildNumber;
		this->SetTimeStamp();
	};*/	// line : 105

	void SetGuildRival(int iGuildNumber);	// line : 108
	/*{
		this->iGuildRival = iGuildNumber;
		this->SetTimeStamp();
	};*/	// line : 111

	void SetTimeStamp();	// line : 117
	/*{
		this->iTimeStamp++;
	};*/	// line : 119

	int GetTimeStamp()	// line : 122
	{
		return this->iTimeStamp;
	};	// line : 124

	BOOL CheckTimeStamp(int iTime)	// line : 127
	{
		return (iTime == this->iTimeStamp)? TRUE : FALSE;
	}	// line : 129
};





class CGuildClass
{

public:

	CGuildClass();
	virtual ~CGuildClass();
	
	void Init();
	_GUILD_INFO_STRUCT* AddGuild(int number, LPSTR guildname, LPBYTE mark, LPSTR mastername,  int score);
	_GUILD_INFO_STRUCT* AddMember(_GUILD_INFO_STRUCT* pNode, LPSTR player_name, int aIndex, int totalcount, int pServer);
	_GUILD_INFO_STRUCT* AddMember(LPSTR guild_name, LPSTR player_name, int aIndex, int totalcount, int iGuildStatus,  int pServer);
	int GetGuildMemberStatus(LPSTR szGuildName, LPSTR szMemberName);
	int SetGuildMemberStatus(LPSTR szGuildName, LPSTR szMemberName, int iGuildStatus);
	int GetGuildType(LPSTR szGuildName);
	int SetGuildType(LPSTR szGuildName, int iGuildType);
	int BuildMemberTotal(_GUILD_INFO_STRUCT* lpNode);
	int DelMember(LPSTR guild_name, LPSTR player_name);
	void AddTail(_GUILD_INFO_STRUCT* pNewNode);
	int DeleteGuild(LPSTR guildname, LPSTR commader);
	void AllDelete();
	void Print();
	void PrintGuild(LPSTR guildname);
	int ConnectUser(_GUILD_INFO_STRUCT* lpNode, LPSTR guild_name, LPSTR player_name,  int aIndex, int pServer);
	int SetServer(LPSTR guild_name, LPSTR player_name, int pServer);
	_GUILD_INFO_STRUCT* SearchGuild(LPSTR guildname);
	_GUILD_INFO_STRUCT* SearchGuild_Number(int number);
	_GUILD_INFO_STRUCT* SearchGuild_NumberAndId(int number, LPSTR name);
	int CloseMember(LPSTR guild_name, LPSTR player_name);


private:

	_GUILD_INFO_STRUCT* head;	// 4
	_GUILD_INFO_STRUCT* tail;	// 8
	std::map<std::string,_GUILD_INFO_STRUCT *> m_GuildMap;	// C
	std::map<int,_GUILD_INFO_STRUCT *> m_GuildNumberMap;	// 1C

};

BOOL gGuildNoticeStringCheck(LPSTR notice);

#endif