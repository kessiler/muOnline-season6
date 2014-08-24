//-> Decompiled by DarkSim | 02.09.2013 | 1.01.03 GS-N
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#include "..\include\prodef.h"
// -------------------------------------------------------------------------------

#define HACKTOOL_DATA_SIZE	20
// -------------------------------------------------------------------------------

struct HACKTOOL_DATA
{
	bool	bIsUsed;			// +0x0(0x1)
	WORD	wIndex;				// +0x2(0x2)
	char	chFilename[64];		// +0x4(0x40)
	BYTE	btHackCheckType;	// +0x44(0x1)
	long	lCheckDuration;		// +0x48(0x4)
	BYTE	btHackBlockType;	// +0x4c(0x1)
	DWORD	dwLastCheckTime;	// +0x50(0x4)
	WORD	wTotalUserCount;	// +0x54(0x2)
	WORD	wHacktoolUseCount;	// +0x56(0x2)
	void	HACKTOOL_DATA::Clear();
};
// -------------------------------------------------------------------------------

struct HACKTOOL_USE_INFO
{
	BYTE	btUseType;			// +0x0(0x1)
	BYTE	btCheckType;		// +0x1(0x1)
	WORD	wHacktoolIndex;		// +0x2(0x2)
	void HACKTOOL_USE_INFO::Set(BYTE UseType, BYTE CheckType, WORD HacktoolIndex);
	void HACKTOOL_USE_INFO::Clear();	//-> Not writed
};
// -------------------------------------------------------------------------------

struct PMSG_HACKTOOL_CHECK_USE_STATISTICS_REQ 
{
	PBMSG_HEAD2 head;			// +0x0(0x4)
	WORD	wHacktoolIndex;		// +0x4(0x2)
	char	chFilename[64];		// +0x6(0x40)
	BYTE	btBlockType;		// +0x46(0x1)
	long	lExpireDate;		// +0x48(0x4)
};
// -------------------------------------------------------------------------------

struct PMSG_HACKTOOL_CHECK_USE_STATISTISC_ANS
{
	PBMSG_HEAD2 head;			// +0x0(0x4)
	WORD	wHacktoolIndex;		// +0x4(0x2)
	WORD	wTotalUserCount;	// +0x6(0x2)
	WORD	wHacktoolUseCount;	// +0x8(0x2)
	BYTE	btResult;			// +0xa(0x1)
};
// -------------------------------------------------------------------------------

struct PMSG_HACKTOOL_USE_STATISTICS_REQ
{
	PBMSG_HEAD h;					// +0x0(0x3)
	char	chHackToolFilename[64];	// +0x3(0x40)
};
// -------------------------------------------------------------------------------

struct PMSG_HACKTOOL_USE_STATISTICS_ANS
{
	PBMSG_HEAD h;					// +0x0(0x3)
	WORD	wCurrentUserCount;		// +0x4(0x2)
	WORD	wHackToolUserCount;		// +0x6(0x2)
	char	chHackToolFilename[64];	// +0x8(0x40)
};
// -------------------------------------------------------------------------------

struct PMSG_HACKTOOL_USE_STATISTICS_TOTAL
{
	PBMSG_HEAD h;				// +0x0(0x3)
	char	chMessage[256];		// +0x3(0x100)
};
// -------------------------------------------------------------------------------

struct PMSG_REQ_HACKTOOL_STATISTICS
{
	PBMSG_HEAD h;				// +0x0(0x3)
	ULONG	dwKey;				// +0x4(0x4)
	char	Filename[64];		// +0x8(0x40)
};
// -------------------------------------------------------------------------------

struct PMSG_HACKTOOL_BLOCK_REQ
{
	PBMSG_HEAD2 head;			// +0x0(0x4)
	WORD	wHacktoolIndex;		// +0x4(0x2)
	char	chFilename[64];		// +0x6(0x40)
	BYTE	btBlockType;		// +0x46(0x1)
	long	lExpireDate;		// +0x48(0x4)
};
// -------------------------------------------------------------------------------

struct PMSG_HACKTOOL_BLOCK_ANS
{
	PBMSG_HEAD2 head;			// +0x0(0x4)
	BYTE	btResult;			// +0x4(0x1)
};
// -------------------------------------------------------------------------------

struct PMSG_HACKTOOL_BLOCK_REMOVE_REQ
{
	PBMSG_HEAD2 head;			// +0x0(0x4)
	WORD	wHacktoolIndex;		// +0x4(0x2)
	char	chFilename[64];		// +0x6(0x40)
};
// -------------------------------------------------------------------------------

struct PMSG_HACKTOOL_BLOCK_REMOVE_ANS
{
	PBMSG_HEAD2 head;			// +0x0(0x4)
};
// -------------------------------------------------------------------------------

extern int g_iHacktoolUsePenaltyExp;
extern int g_iHacktoolUsePenaltyZen;
extern int g_iHacktoolUsePenaltyItemDropRate;
extern int g_iHacktoolUsePenaltyDuration;
extern int g_iHacktoolCheckTerm;
// -------------------------------------------------------------------------------

class CHacktoolBlockEx
{
public:

			CHacktoolBlockEx();
	virtual	~CHacktoolBlockEx();
	// ----
	void	Initialize();
	void	LoadScript(char * pchFilename);
	// ----
	bool	AddHacktoolData(WORD wHacktoolIndex, char * pchFilename, long lDuration, BYTE btBlockType);
	void	RemoveHacktoolData(WORD wHacktoolIndex, char * pchFilename);
	void	ModifyHacktoolData();
	HACKTOOL_DATA * SearchHacktoolData(WORD wHacktoolIndex);
	// ----
	void	OnRequestHacktoolStatistics(PMSG_HACKTOOL_CHECK_USE_STATISTICS_REQ * lpMsg);
	void	OnRequestHacktoolBlock(PMSG_HACKTOOL_BLOCK_REQ * lpMsg);
	void	OnRequestHacktoolBlockRemove(PMSG_HACKTOOL_BLOCK_REMOVE_REQ * lpMsg);
	void	OnRequestUserHacktoolUseInfo(OBJECTSTRUCT * lpObj, DWORD dwHacktoolIndex, BYTE btResult);
	// ----
	void	SetHacktoolDisadvantage();
	bool	SetHacktoolUseDisadvantage(OBJECTSTRUCT * lpObj, BYTE btBlockType);
	void	SetUseForceMove2Town(OBJECTSTRUCT * lpObj);
	void	SetUseForceMove2GMMap(OBJECTSTRUCT * lpObj);
	// ----
	void	CheckHacktoolUseConnectedUser(OBJECTSTRUCT * lpObj);
	void	CheckHacktoolUse();
	// ----
private:
	HACKTOOL_DATA m_HacktoolData[HACKTOOL_DATA_SIZE];	// +0x4(0x6e0)
	int		m_iHacktoolCheckRandomTime;					// +0x6e4(0x4)
	// ----
}; extern CHacktoolBlockEx g_HacktoolBlockEx;
// -------------------------------------------------------------------------------