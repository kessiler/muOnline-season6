// PCBangPointSystem.h: interface for the CPCBangPointSystem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __PCBANGPOINTSYSTEM_H__
#define __PCBANGPOINTSYSTEM_H__

#include "..\include\prodef.h"
#include "..\common\zzzitem.h"

#ifdef PCBANG
struct PCBANG_POINT_SYSTEM
{
	//PcBang Control
	BOOL m_bPcBangPointEnable;

	//PcBang Common Settings
	BYTE	m_bPcBangCommonRule; //0x20A8 
	short	m_sPcBangCommonRuleTime; //0x20AA

	//PcBang Point Storage
	short	m_sPcBangGainPoint; //0x20AC
	int		m_iPcBangAccumulatedPoint; //0x20B0

	//PcBang Set Info
	BOOL	m_bPcBangInfoSet; //0x20B4

	//PcBang Timer Point Tick
	DWORD	m_dwPcBangPointTick; //0x20B8

	//PcBang Reset Point Time Control (Eventserver Send)
	short	m_sPcBangResetYear; //0x20BC
	short	m_sPcBangResetMonth; //0x20BE
	short	m_sPcBangResetDay; //0x20C0
	short	m_sPcBangResetHour; //0x20C2

	void RESET();
};

class CPCBangPointTimeCondition //completed identical
{
public:
	CPCBangPointTimeCondition();
	virtual ~CPCBangPointTimeCondition();

	int GetTimeDate(int iYear, int iMonth, int iDay);
	BYTE IsLeapYear(int iYear);
	int GetNumberedDays(int iYear, int iMonth);
	void ModifyTimeDate(int iDayOfWeek, int * iYear, int * iMonth, int * iDay);
	int CheckUserTimeDate(int iUSR_Year, int iUSR_Month, int iUSR_Day, int iUSR_Hour, int iSVR_Year, int iSVR_Month, int iSVR_Day, int iSVR_Hour);
};

class CPCBangPointTimer : public CPCBangPointTimeCondition //completed identical
{
public:
	CPCBangPointTimer();
	virtual ~CPCBangPointTimer();

	typedef void (*TUserIndex)(CPCBangPointTimer*,int);
	static void SetUserTimer(CPCBangPointTimer*, int);

	void SetResetTimeData(int iYear, int iMonth, int iDay, int iHour, int iSvrDayWeek, int iSvrHour, TUserIndex iTUserIndex, int aIndex);
	void Clear();
	int CheckTime();
	void SetCheckTimer(BOOL bResult);
	void ChangeTimeDate(int * iYear, int * iMonth, int * iDay, int * iHour);

public:
	int	m_iUSR_ResetYear; //0x4
	int m_iUSR_ResetMonth; //0x8
	int m_iUSR_ResetDay; //0xC
	int m_iUSR_ResetHour; //0x10
	int m_iSVR_ResetDayOfWeek; //0x14
	int m_iSVR_ResetHour; //0x18
	TUserIndex m_iTUserIndex; //0x1C
	BOOL m_bSetTimer; //(size 0x4) //0x20
	SYSTEMTIME m_tmPcBangResetTime; //(size 0x10) //0x24
	int m_iUserIndex; //0x34
};

#define MAX_PCBANG_ITEM_COUNT 6

extern BYTE	g_btPcBangPointSystemLevel;
extern short	g_sPCBangFirstRuleTime;
extern short	g_sPCBangFirstRuleTimePoint;
extern short	g_sPCBangCommonRuleTime;
extern short	g_sPCBangCommonRuleTimePoint;
extern short	g_sPCBangMaxPoint;
extern short	g_sPCBangFeverStartHour;
extern short	g_sPCBangFeverEndHour;
extern short	g_sPCBangFeverMultipleValue;
extern short	g_sPCBangResetDayOfWeek;
extern short	g_sPCBangResetHour;
extern float	g_fPCBangAddExperience;

struct PMSG_POINT_BUY_ITEM
{
	PBMSG_HEAD2 h;
	BYTE Result;
	BYTE ItemPos;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PCBANG_LOAD_INFO
{
	int Index;
	short BuyPoint;
	short Condition;
	BYTE Ability;
	int KeepTime;
	WORD AddExpPercent;
	short GetItemPercent;
	BYTE StopExp;
};

struct PMSG_ANS_REG_PC_POINT //check size 1C
{
	PBMSG_HEAD h; //3
	short aIndex; //4
	char AccountID[11]; //
	BYTE btShopItemBuy; //11
	short sPoint; //12
	short sPcBangResetYear; //14
	short sPcBangResetMonth; //16
	short sPcBangResetDay; //18
	short sPcBangResetHour; //1A
};

struct PMSG_REQ_REG_PC_POINT
{
	PBMSG_HEAD h;
	short aIndex; //14
	BYTE btResult; // 12
	char AccountID[11]; //11
};

struct PMSG_ANS_PCBANG_POINT_INFO
{
	PBMSG_HEAD2 h;
	short m_sPoint;
	short m_sMaxPoint;
	BYTE m_btType;
};

struct PMSG_REQ_UPDATE_PCBANG_USER_POINT
{
	PBMSG_HEAD h;
	short aIndex; //14
	char AccountID[11]; //12
	BYTE btResult; //7
	short sPoint; //6
};

struct PMSG_SHOP_INFO
{
	PBMSG_HEAD2 h;
	BYTE Result;
};

struct PMSG_SHOP_ITEM_COUNT
{
	PWMSG_HEAD h;
	BYTE Type;
	BYTE count;
};

class CPCBangPointSystem
{
public:
	CPCBangPointSystem();
	virtual ~CPCBangPointSystem();

	void Init();
	int Load(char* lpszFileName);
	BOOL CheckPcBangRuleTime(int aIndex);
	int GetItemIndex(int iItemIndex);
	int ShopMapCheck(BYTE MapNumber, short X, short Y);
	void SetPointUser(int aIndex, int iRuleTime);
	BOOL AddPoint(struct OBJECTSTRUCT * lpObj);
	void EGUserConnectTest(int aIndex);
	void EGAnsPcBangPointInfo(PMSG_ANS_REG_PC_POINT* aRecv);
	void EGReqUpdatePcBangPointInfo(int aIndex);
	void EGAnsUserFirstPcBangUpdatePoint(PMSG_ANS_REG_PC_POINT* aRecv);
	void EGReqUpdatePcBangPointInfo(int aIndex, short sPCBangPoint);
	void GCSendPcBangUserPoint(int aIndex);
	void PCBangShopOpen(int aIndex);
	int PCBangBuyItem(int aIndex, BYTE ItemPosition);
	void EGUserConnectPcBang(int aIndex);
	void EGAnsUpdatePcBangResetPointInfo(PMSG_ANS_REG_PC_POINT* aRecv);
	void EGReqUpdatePcBangResetPointInfo(int aIndex);
	void EGAnsUserPcBangUpdatePoint(PMSG_ANS_REG_PC_POINT* aRecv);
	BOOL AddExperience(struct OBJECTSTRUCT * lpObj, __int64 & Experience);

private:
	int m_iPcBangIndex; //4
	BOOL m_bPointSystemOn; //8
	BOOL m_bPointRankEvent; //C
	BYTE m_btPcBangShopNumber; //10
	PCBANG_LOAD_INFO m_PcBangSystem[MAX_PCBANG_ITEM_COUNT];
};

extern CPCBangPointSystem g_PCBangPointSystem;
#endif
#endif