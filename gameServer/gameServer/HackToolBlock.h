#pragma once
// -------------------------------------------------------------------------------

class CHackToolData
{
public:
			CHackToolData();
			~CHackToolData();
	// ----
    void	Clear();
	// ----
    bool	IsUsed();
    bool	IsMatched(char * pchFilename);
	// ----
	void	SetData(char * pchFilename, DWORD dwKeyValue);
	void	SetData(char * pchFilename, DWORD dwBlockTerm, int iBlockRate);
	void	SetData(char * pchFilename, DWORD dwKeyValue, DWORD dwBlockTerm, int iBlockRate);
    void	SetData(char * pchFilename, DWORD dwKeyValue, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate);
	// ----
    bool	KeyValueVerify(DWORD dwKey);
    void	SetStatistics(LPOBJ lpObj, BYTE btResult);
    void	BeginStatistics();
    bool	CheckStatisticsFinish();
	// ----
    void	ResetData();
    void	ResetSchedule();
    bool	IsBlocked();
    bool	CheckBlockActive();
	// ----
public:
    bool	bIsUsed;				// +0x0(0x1)
    DWORD	dwActiveTime;			// +0x4(0x4)
    DWORD	dwKey;					// +0x8(0x4)
    char	chHackToolName[0x40];	// +0xc(0x40)
    int		iHacktoolUserCount;		// +0x4c(0x4)
    bool	bBlockHacktoolUser;		// +0x50(0x1)
    int		iMapmoveBlockRate;		// +0x54(0x4)
    DWORD	dwMapmoveBlockTerm;		// +0x58(0x4)
    bool	bBeginStatistics;		// +0x5c(0x1)
    DWORD	dwStatisticsBeginTick;	// +0x60(0x4)
	CTime	tBlockFinishTime;		// +0x64(0x8)
};
// -------------------------------------------------------------------------------

class CHackToolBlock
{
public:
			CHackToolBlock();
	virtual	~CHackToolBlock();
	// ----
	void	Initialize();
	void	Clear();
	bool	LoadOption(char * pchOptionFilename);
	// ----
	bool	AddHacktoolData(char * pchFilename, DWORD dwBlockTerm, int iBlockRate);
	bool	AddHacktoolData(char * pchFilename, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate);
	bool	RemoveHacktoolData(char * pchFilename);
	bool	ModifyHacktoolData(char * pchFilename, DWORD dwActiveTime, DWORD dwBlockTerm, int iBlockRate);
	// ----
	bool	KeyValueVerify(DWORD dwKey);
	CHackToolData * SearchData(char * pchFilename);
	CHackToolData * SearchData(DWORD dwKeyValue);
	DWORD	_MakeKeyValue();
	DWORD	MakeDateToDword(char * pchDateString);
	// ----
	void	CheckBlockActive();
	bool	SetGMObject(LPOBJ lpObj);
	LPOBJ	GetGMObject();
	bool	IsGMUser(LPOBJ lpObj);
	void	SendNoticeToGM(char * pchMessage, ...);
	void	SendChatMessageToGM(char * pchMessage, ...);
	bool	IsHackedUser(LPOBJ lpObj);
	// ----
	void	SetHacktoolStatistics(LPOBJ lpObj, DWORD dwKey, BYTE btResult);
	void	HacktoolStatisticsAll(char * pchFilename);
	void	HacktoolStatistics(LPOBJ lpObj, char * pchFilename);
	bool	CheckStatisticsResult();
	void	HacktoolStatisticsNewConnect(LPOBJ lpObj);
	static void	AddLog(char * pchLog, ...);	//-> __cdecl...
	// ----
}; extern CHackToolBlock g_HackToolBlock;
// -------------------------------------------------------------------------------