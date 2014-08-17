#ifndef __SPRODEF_H__
#define __SPRODEF_H__

#include "ProDef.h"
//----------------------------------------------------------------------------
// 결과 공통
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE	Result;
} SDHP_DEFAULT_RESULT, *LPSDHP_DEFAULT_RESULT;


//----------------------------------------------------------------------------
// 결과를 보낼때 사용 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	DWORD		ItemCount;
} SDHP_RESULT, *LPSDHP_RESULT;

//----------------------------------------------------------------------------
// [0x00] 각각의 서버 정보를 보낼때 사용
//----------------------------------------------------------------------------
typedef struct 
{	
	PBMSG_HEAD	h;
		
	BYTE			Type;		// 0:Join Server, 1:MapServer 2: ManagerServer
	unsigned short	Port;		// Server Port Number
	char			ServerName[50];
	unsigned short	ServerCode;
} SDHP_SERVERINFO, * LPSDHP_SERVERINFO;

//----------------------------------------------------------------------------
// [0x20] 게임서버의 정보를 데이터서버에 보내서 저장한다.
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	DWORD		ItemCount;
} SDHP_GAMESERVERINFO, *LPSDHP_GAMESERVERINFO;

//----------------------------------------------------------------------------
// DB 서버에 캐릭터 리스트를 달라고 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	//int			UserNumber;	// 유저 절대 번호 
	char		Id[MAX_IDSTRING];
	short		Number;	// 게임 서버 유저번호	
} SDHP_GETCHARLIST, *LPSDHP_GETCHARLIST;

//----------------------------------------------------------------------------
// 캐릭터 리스트 카운트  (2바이트 크기를 사용)
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	short		Number;
	BYTE		Count;
	int			DbNumber;
	BYTE		GenerableClass;		// 마검사를 만들수 있는 계정인지..
	char		AccountId[MAX_IDSTRING+1];
	BYTE		MoveCnt;
	BYTE		ExpandedWarehouse;
} SDHP_CHARLISTCOUNT, *LPSDHP_CHARLISTCOUNT;

//----------------------------------------------------------------------------
// [0x02] 계정 인증 요청시 캐릭터 정보(카운터 만큼 반복됨)
// [0xF3][0x00]
//----------------------------------------------------------------------------
typedef struct
{
	BYTE Index;
	char Name[MAX_IDSTRING];
	WORD Level;
	BYTE Class;
	BYTE CtlCode;
	BYTE dbInventory[48];
	BYTE DbVersion;
	BYTE btGuildStatus;
} SDHP_CHARLIST, * LPSDHP_CHARLIST;


//----------------------------------------------------------------------------
// [0x03] 유저 게임 종료 메시지
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char CharName[MAX_IDSTRING];
	char GuildName[MAX_GUILDNAMESTRING];
	BYTE Type;
} SDHP_USERCLOSE, *LPSDHP_USERCLOSE;

//----------------------------------------------------------------------------
// [0x04] 캐릭터 만들기 에 사용
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	int         UserNumber;
	int			DBNumber;
	short		Number;
	char        AccountId[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	BYTE		ClassSkin;
} SDHP_CREATECHAR, *LPSDHP_CREATECHAR;

//----------------------------------------------------------------------------
// [0x04] 캐릭터 만들고 난 결과 보내는데 사용
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;	
	BYTE		Result;
	short		Number;
	char		AccountId[MAX_IDSTRING];
	char        Name[MAX_IDSTRING];
	BYTE		Pos;
	BYTE		ClassSkin;
	BYTE		Equipment[24];	// 착용하고 있는 장비
	WORD		Level;
} SDHP_CREATECHARRESULT, *LPSDHP_CREATECHARRESULT;

//----------------------------------------------------------------------------
// 0x05 게임서버가 캐릭터 삭제 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	short		Number;
	char		AccountID[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	BYTE		Guild;
	char		GuildName[MAX_GUILDNAMESTRING];
} SDHP_CHARDELETE, *LPSDHP_CHARDELETE;

//----------------------------------------------------------------------------
// 0x05 캐릭터 삭제 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
	short		Number;
	char		AccountID[MAX_IDSTRING];

} SDHP_CHARDELETERESULT, *LPSDHP_CHARDELETERESULT;

//----------------------------------------------------------------------------
// 0x06 게임서버가 인증된 유저의 캐릭터 정보를 요청
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD  h;	
	char    AccountID[MAX_IDSTRING+1];
	char	Name[MAX_IDSTRING+1];
	short	Number;
} SDHP_DBCHARINFOREQUEST, *LPSDHP_DBCHARINFOREQUEST;


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------








//----------------------------------------------------------------------------
// 0x06 캐릭터 정보 요청 결과 
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD h;
	BYTE result;
	short Number;
	char AccountID[MAX_IDSTRING];
	char Name[MAX_IDSTRING+1];
	BYTE Class;
	short Level;
	int LevelUpPoint;
	int Exp;
	int NextExp;
	int Money;
#if( FIX_MAXSTAT == 1 )
	WORD Str;	// 30
	WORD Dex;	// 32
	WORD Vit;	// 34
	WORD Energy;	// 36
#else
	short Str;	// 30
	short Dex;	// 32
	short Vit;	// 34
	short Energy;	// 36
#endif
	WORD Life;
	WORD MaxLife;
	WORD Mana;
	WORD MaxMana;
	BYTE dbInventory[MAX_DBINVENTORY];//3776
	BYTE dbMagicList[MAX_DBMAGIC];//450
	BYTE MapNumber;
	BYTE MapX;
	BYTE MapY;
	BYTE Dir;
	int PkCount;
	int PkLevel;
	int PkTime;
	BYTE CtlCode;
	BYTE DbVersion;
	BYTE AccountCtlCode;
	BYTE dbQuest[MAX_QUEST];
	WORD Leadership;
	WORD ChatLitmitTime;
	int iFruitPoint;
	int Reset;
	int GrandReset;
	int ExpandedInventory;
#ifdef __ALIEN__
	int DbOffExp;
#endif
} SDHP_DBCHAR_INFORESULT, *LPSDHP_DBCHAR_INFORESULT;

//----------------------------------------------------------------------------
// 0x07 캐릭터 정보 저장 
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD h;
	char Name[MAX_IDSTRING+1];
	short Level;
	BYTE Class;
	int LevelUpPoint;
	int Exp;
	int NextExp;
	int Money;
#if( FIX_MAXSTAT == 1 )
	WORD Str;	// 24
	WORD Dex;	// 26
	WORD Vit;	// 28
	WORD Energy;	// 2A
#else
	short Str;	// 24
	short Dex;	// 26
	short Vit;	// 28
	short Energy;	// 2A
#endif
	WORD Life;
	WORD MaxLife;
	WORD Mana;
	WORD MaxMana;
	BYTE dbInventory[MAX_DBINVENTORY];
	BYTE dbMagicList[MAX_DBMAGIC];
	BYTE MapNumber;
	BYTE MapX;
	BYTE MapY;
	BYTE Dir;
	int PkCount;
	int PkLevel;
	int PkTime;
	BYTE dbQuest[MAX_QUEST];
	BYTE CharInfoSave;
	WORD Leadership;
	WORD ChatLitmitTime;
	int iFruitPoint;
	int Reset;
	int GrandReset;
	int ExpandedInventory;
} SDHP_DBCHAR_INFOSAVE, *LPSDHP_DBCHAR_INFOSAVE;

// [0x08] 창고에 있는 아이템을 요청한다.
typedef struct
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	short		aIndex;
} SDHP_GETWAREHOUSEDB, *LPSDHP_GETWAREHOUSEDB;

// [0x08] 창고에 있는 아이템 리스트를 전송한다.
// [0x09] 창고에 아이템을 저장한다.
typedef struct
{
	PWMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	short		aIndex;
	int			Money;
	BYTE		dbItems[MAX_WAREHOUSEDBSIZE];
	BYTE		DbVersion;
	short		pw;
} SDHP_GETWAREHOUSEDB_SAVE, *LPSDHP_GETWAREHOUSEDB_SAVE;


// [0x10] 창고에 아직 아이템이 없다. 
typedef struct
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	short		aIndex;
} SDHP_GETWAREHOUSEDB_RESULT, *LPSDHP_GETWAREHOUSEDB_RESULT;


// [0x11] 캐릭터의 아이템을 저장한다.
typedef struct
{
	PWMSG_HEAD	h;
	
	char  Name[MAX_IDSTRING];	// 이름
	BYTE  dbInventory[MAX_DBINVENTORY];	// 아이템 인벤토리
} SDHP_DBCHAR_ITEMSAVE, *LPSDHP_DBCHAR_ITEMSAVE;

// [0x12] 창고에 돈을 저장한다.
typedef struct
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	short		aIndex;
	int			Money;
} SDHP_WAREHOUSEMONEY_SAVE, *LPSDHP_WAREHOUSEMONEY_SAVE;


/*
//----------------------------------------------------------------------------
// [0x10] 게임서버에서 사용자 계정 생성 요청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	char	Id[MAX_IDSTRING];
	char	Pass[MAX_IDSTRING];
	char	Name[MAX_IDSTRING];
	char	JoominN[MAX_JOOMINNUMBERSTR];
	char	Question[MAX_QUESTIONSTR];
	char	Answer[MAX_ANSWERSTR];
	char	PhoneN[MAX_PHONENUMBERSTR];
	char	Email[MAX_EMAILSTR];
	char    IpAddress[17];
} SDHP_ACCOUNTREQUEST, *LPSDHP_ACCOUNTREQUEST;

//----------------------------------------------------------------------------
// [0x10] 게임서버에서 사용자 계정 생성 결과
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		result;
	short		Number;
} SDHP_ACCOUNTRESULT, *LPSDHP_ACCOUNTRESULT;
*/

//----------------------------------------------------------------------------
// [0x20] 특정아이디의 비번질문을 얻고자 요청한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	char	Id[MAX_IDSTRING];
} SDHP_PASSQUESTION_QUERY, *LPSDHP_PASSQUESTION_QUERY;

//----------------------------------------------------------------------------
// [0x20] 특정아이디의 비번질문을 얻고자 요청한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	BYTE	Result;
	char	Question[MAX_QUESTIONSTR];
} SDHP_PASSQUESTION_QUERY_RESULT, *LPSDHP_PASSQUESTION_QUERY_RESULT;


//----------------------------------------------------------------------------
// [0x21] 비밀번호 문의
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	char	Id[MAX_IDSTRING];
	char	JoominN[MAX_JOOMINNUMBERSTR];
	char	Answer[MAX_ANSWERSTR];
	char    IpAddress[17];
} SDHP_PASSQUERY, *LPSDHP_PASSQUERY;

//----------------------------------------------------------------------------
// [0x21] 비밀번호 문의 결과
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	BYTE	Result;
	char	Password[MAX_IDSTRING];
} SDHP_PASSQUERY_RESULT, *LPSDHP_PASSQUERY_RESULT;


//----------------------------------------------------------------------------
// [0x22] 비밀번호 변경신청
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	char	Id[MAX_IDSTRING];
	char	PassOld[MAX_IDSTRING];
	char	PassNew[MAX_IDSTRING];
	char	JoominN[MAX_JOOMINNUMBERSTR];	
	char	IpAddress[17];
} SDHP_PASSCHANGE, *LPSDHP_PASSCHANGE;


//----------------------------------------------------------------------------
// [0x22] 비밀번호 변경신청 결과
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	short	Number;
	BYTE	Result;
} SDHP_PASSCHANGE_RESULT, *LPSDHP_PASSCHANGE_RESULT;

//----------------------------------------------------------------------------
// [0x30] 길드를 생성한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		Master[MAX_IDSTRING+1];
	BYTE		Mark[32];
	BYTE		NumberH;
	BYTE		NumberL;
} SDHP_GUILDCREATE, *LPSDHP_GUILDCREATE;

//----------------------------------------------------------------------------
// 길드 생성 결과
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		Result;
	BYTE		Flag;

	DWORD		GuildNumber;
	
	BYTE		NumberH;
	BYTE		NumberL;
	char		Master[MAX_IDSTRING+1];
	char		GuildName[MAX_GUILDNAMESTRING+1];
	BYTE		Mark[32];
	
} SDHP_GUILDCREATE_RESULT, *LPSDHP_GUILDCREATE_RESULT;

//----------------------------------------------------------------------------
// [0x31] 길드 삭제
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		NumberH;
	BYTE		NumberL;
	
	char		GuildName[MAX_GUILDNAMESTRING];
	char		Master[MAX_IDSTRING];
} SDHP_GUILDDESTROY, *LPSDHP_GUILDDESTROY;

//----------------------------------------------------------------------------
// [0x31] 길드 삭제 결과
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		Result;
	BYTE		Flag;

	BYTE		NumberH;
	BYTE		NumberL;

	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		Master[MAX_IDSTRING+1];
} SDHP_GUILDDESTROY_RESULT, *LPSDHP_GUILDDESTROY_RESULT;

//----------------------------------------------------------------------------
// [0x32] 길드 멤버 추가
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		MemberID[MAX_IDSTRING+1];
	BYTE		NumberH;
	BYTE		NumberL;
} SDHP_GUILDMEMBERADD, *LPSDHP_GUILDMEMBERADD;

//----------------------------------------------------------------------------
// [0x32] 길드 멤버 추가 결과
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		Result;
	BYTE		Flag;

	BYTE		NumberH;
	BYTE		NumberL;

	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		MemberID[MAX_IDSTRING+1];

} SDHP_GUILDMEMBERADD_RESULT, *LPSDHP_GUILDMEMBERADD_RESULT;

//----------------------------------------------------------------------------
// [0x33] 길드멤버 삭제 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;

	BYTE		NumberH;
	BYTE		NumberL;

	char		GuildName[MAX_GUILDNAMESTRING];
	char		MemberID[MAX_IDSTRING];
} SDHP_GUILDMEMBERDEL, *LPSDHP_GUILDMEMBERDEL;

//----------------------------------------------------------------------------
// [0x33] 길드멤버 삭제 결과
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	
	BYTE		Result;
	BYTE		Flag;

	BYTE		NumberH;
	BYTE		NumberL;

	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		MemberID[MAX_IDSTRING+1];

} SDHP_GUILDMEMBERDEL_RESULT, *LPSDHP_GUILDMEMBERDEL_RESULT;


//----------------------------------------------------------------------------
// [0x34] 길드 정보 변경 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		Master[MAX_IDSTRING+1];
	BYTE		Mark[32];
	int			Score;
	BYTE		Count;
} SDHP_GUILDUPDATE, *LPSDHP_GUILDUPDATE;

//----------------------------------------------------------------------------
// [0x35] 캐릭터가 게임에 접속시 길드 정보를 보낸다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		MemberID[MAX_IDSTRING+1];
} SDHP_GUILDMEMBER_INFO, *LPSDHP_GUILDMEMBER_INFO;


//----------------------------------------------------------------------------
// [0x36] 길드마스터가 접속했을 때 길드원 정보를 재전송한다.
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;

	char		GuildName[MAX_GUILDNAMESTRING+1];
	BYTE		Count;
} SDHP_GUILDALL_COUNT, *LPSDHP_GUILDALL_COUNT;

//----------------------------------------------------------------------------
// [0x36] 길드마스터가 접속했을 때 길드원 정보를 재전송한다.
//----------------------------------------------------------------------------
typedef struct
{
	char		MemberID[MAX_IDSTRING+1];
} SDHP_GUILDALL, *LPSDHP_GUILDALL;

//----------------------------------------------------------------------------
// [0x37] 길드 스코어 업데이트 
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	int			Score;
} SDHP_GUILDSCOREUPDATE, *LPSDHP_GUILDSCOREUPDATE;

//----------------------------------------------------------------------------
// [0x38] 길드 공지 저장
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	char GuildName[MAX_GUILDNAMESTRING+1];
	char szGuildNotice[MAX_GUILDNOTICE];
} SDHP_GUILDNOTICE, *LPSDHP_GUILDNOTICE;

//----------------------------------------------------------------------------
// [0x40] 길드를 생성한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	WORD		Number;
	char		GuildName[MAX_GUILDNAMESTRING+1];
	char		Master[MAX_IDSTRING+1];
	BYTE		Mark[32];
	int			score;
} SDHP_GUILDCREATED, *LPSDHP_GUILDCREATED;

//----------------------------------------------------------------------------
// [0x50] 이벤트 ZEN을 저장한다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	int			Total;
	int			Number;
} SDHP_ZENSAVE, *LPSDHP_ZENSAVE;

//----------------------------------------------------------------------------
// [0x50] 이벤트 ZEN을 읽어온다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	char		AccountID[MAX_IDSTRING];
	int			Total;
	int			Number;
} SDHP_ZENLOAD, *LPSDHP_ZENLOAD;

//----------------------------------------------------------------------------
// [0x52] 아이템 번호를 부여받는다.
//----------------------------------------------------------------------------
// MapNumber 가 255 일때는 카오스 박스이다.
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		x;
	BYTE		y;
	BYTE		MapNumber;
	short		Type;
	BYTE		Level;
	BYTE		Dur;
	BYTE		Op1;
	BYTE		Op2;
	BYTE		Op3;
	BYTE		NewOption;
	int			aIndex;
	short		lootindex;
	BYTE		SetOption;
	long		lDuration;
	DWORD		dwEventIndex;
} SDHP_ITEMCREATE, *LPSDHP_ITEMCREATE;

//----------------------------------------------------------------------------
// [0x52] 아이템 번호를 부여받는다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD h;
	BYTE x;
	BYTE y;
	BYTE MapNumber;
	DWORD m_Number;
	short Type;
	BYTE Level;
	BYTE Dur;
	BYTE Op1;
	BYTE Op2;
	BYTE Op3;
	BYTE NewOption;
	int aIndex;
	short lootindex;
	BYTE SetOption;
	long lDuration;
	DWORD dwEventIndex;
} SDHP_ITEMCREATERECV, *LPSDHP_ITEMCREATERECV;


//----------------------------------------------------------------------------
// [0x53] 아이템 이동 경로를 저장한다.
//----------------------------------------------------------------------------
typedef struct 
{

	PBMSG_HEAD	h;
	DWORD		Serial;
	char		ServerName[20];
	char		Account[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
	char		ItemName[21];
	BYTE		X;
	BYTE		Y;
	BYTE		ItemLevel;
	BYTE		ItemOp1;
	BYTE		ItemOp2;
	BYTE		ItemOp3;

} SDHP_ITEMMOVESAVE, *LPSDHP_ITEMMOVESAVE;

//----------------------------------------------------------------------------
// [0x60] 이벤트 시작을 알린다.
//----------------------------------------------------------------------------
typedef struct 
{
	PBMSG_HEAD	h;
	BYTE		Event;	
} SDHP_EVENTSTART, *LPSDHP_EVENTSTART;

// 0x30
typedef struct
{
	PBMSG_HEAD	h;
	char		Account[MAX_IDSTRING];
	char		Name[MAX_IDSTRING];
} SDHP_LOVEHEARTEVENT, * LPSDHP_LOVEHEARTEVENT;

// 0x30
typedef struct
{
	PBMSG_HEAD	h;

	BYTE		Result;

	char		Name[MAX_IDSTRING];
	int			Number;
	
} SDHP_LOVEHEARTEVENT_RESULT, * LPSDHP_LOVEHEARTEVENT_RESULT;

// 0x40 전체 공지
typedef struct
{
	PBMSG_HEAD	h;
	char		Notice[61];
} SDHP_NOTICE, *LPSDHP_NOTICE;

// 0x41 유저 공지
typedef struct
{
	PBMSG_HEAD	h;
	char		szId[MAX_IDSTRING];
	char		Notice[61];
} SDHP_USER_NOTICE, *LPSDHP_USER_NOTICE;


// 0x42 게임 블럭
typedef struct
{
	PBMSG_HEAD	h;
	int		ClientIndex;
	char	AccountID[MAX_IDSTRING];
	BYTE	ServerNum;
	char	CharName[MAX_IDSTRING];
	BYTE	Type;
} SDHP_GAME_BLOCK, *LPSDHP_GAME_BLOCK;

// 0x42 게임 블럭 결과
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		Result;
} SDHP_GAME_BLOCK_RESULT, *LPSDHP_GAME_BLOCK_RESULT;

#endif


