#pragma once
// -------------------------------------------------------------------------------

#define EVENT_ACTIVE(value) ( (Flag&value) == value )
// -------------------------------------------------------------------------------

#define AUTHKEY0	41
#define AUTHKEY1	35
#define AUTHKEY2	-66
#define AUTHKEY3	0x00
#define AUTHKEY4	-31
#define AUTHKEY5	108
#define AUTHKEY6	-42
#define AUTHKEY7	-82
#define AUTHKEY8	82
#define AUTHKEY9	-112
#define AUTHKEY10	73
#define AUTHKEY11	-15
#define AUTHKEY12	-15
#define AUTHKEY13	-69
#define AUTHKEY14	-23
#define AUTHKEY15	-21
#define AUTHKEY16	-77
#define AUTHKEY17	-90
#define AUTHKEY18	-37
#define AUTHKEY19	219
// -------------------------------------------------------------------------------

enum DATA_FILE_INDEX
{
	FILE_ITEM				= 0,
	FILE_ITEMLOCAL			= 1,
	FILE_SKILL				= 2,
	FILE_SKILLLOCAL			= 3,
	FILE_QUEST				= 4,
	FILE_QUESTLOCAL			= 5,
	FILE_CHECKSUM			= 6,
	FILE_MONSTER			= 7,
	FILE_GATE				= 8,
	FILE_MONSTERSETBASE		= 9,
	FILE_NPGGAUTH			= 10,
	FILE_SHOP0				= 11,
	FILE_SHOP1				= 12,
	FILE_SHOP2				= 13,
	FILE_SHOP3				= 14,
	FILE_SHOP4				= 15,
	FILE_SHOP5				= 16,
	FILE_SHOP6				= 17,
	FILE_SHOP7				= 18,
	FILE_SHOP8				= 19,
	FILE_SHOP9				= 20,
	FILE_SHOP10				= 21,
	FILE_SHOP11				= 22,
	FILE_SHOP12				= 23,
	FILE_MOVEREQ			= 24,
	FILE_MOVEREQLOCAL		= 25,
	FILE_SETITEMOPTION		= 26,
	FILE_SETITEMOPTIONLOCAL = 27,
	FILE_SETITEMTYPE		= 28,
	FILE_SETITEMTYPELOCAL	= 29,
	FILE_SHOP13				= 30,
	FILE_SHOP14				= 31,
	FILE_SHOP15				= 32,
	FILE_SHOP16				= 33,
	FILE_SHOP17				= 34,
	FILE_SHOP18				= 35,
};
// -------------------------------------------------------------------------------

class WzAG
{
public:
	void	GetClientVersion(char * ClientVersion, char * ClientSerial);
	char *	GetDataBuffer();
	int		GetDataBufferSize();
	void	GetKey(char * Key, int StartIndex, int Len);
	void	ReleaseData();
	void	RequestData(int Type);
	bool	SetInfo(int CountryCode, BYTE Partition, short ServerCode, char * ServerVersion,  char * ServerName, int ServerType,  void(*CallBack)(int, int));
	void	SetSubInfo(int ServerMaxUser, int ObjTotalUser, int MonsterHP, int ItemDropPer, float AddExperience, int EventFlag);
	// ----
private:
	char * GetCountryNameByCode(int CountryCode);
	char * GetServerTypeByType(int ServerType);
	char * GetEventFlag(int Flag);
	// ----
}; extern WzAG gWzAG;
// -------------------------------------------------------------------------------

void GameServerAuthCallBackFunc(int Type, int Data);
// -------------------------------------------------------------------------------