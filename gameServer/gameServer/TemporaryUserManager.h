#pragma once
// -------------------------------------------------------------------------

#include "user.h"
// -------------------------------------------------------------------------

#define MAX_TEMP_SLOT			OBJMAXUSER
#define IS_TEMP_SLOT(SlotIndex)	( ((SlotIndex) < 0 )?FALSE:( (SlotIndex) > MAX_TEMP_SLOT-1 )?FALSE:TRUE )
#define TEMP_NONE				0
#define TEMP_BLOODCASTLE		1
#define TEMP_ILLUSION			2
#define TEMP_IMPERIAL			3
#define TEMP_DOPPEL				4
#define TEMP_DEVILSQUARE		5
// -------------------------------------------------------------------------

struct TEMPORARY_SET_EXGAMESERVERCODE	//-> 16
{
	PBMSG_HEAD h;
	// ----
	char	szCharName[MAX_IDSTRING+1];
	short	sExGameServerCode;
};
// -------------------------------------------------------------------------

struct TEMPORARY_USER_DATA
{
	int		Tick;
	char	Name[11];
	int		UserIndex;
	int		Change;
	int		PartyNumber;
	char	MapNumber;
	int		BloodCastleIndex;
	int		cBloodCastleSubIndex;
	int		iBloodCastleEXP;
	int		iIllusionTempleIndex;
	int		ImperialGuardianIndex;
	int		EventScore;
	int		EventExp;
	int		EventMoney;
	bool	bDevilSquareIndex;
	bool	bDevilSquareAuth;
};
// -------------------------------------------------------------------------

class CTemporaryUserManager
{
public:
	CTemporaryUserManager(void)
	{ 
		InitializeTempUserData(); 
		InitializeCriticalSection(&m_cs); 
	};
	// ----
	~CTemporaryUserManager(void)
	{ 
		DeleteCriticalSection(&m_cs); 
	};
	// ----
	void InitializeTempUserData()
	{
		ZeroMemory(m_UserData, sizeof(m_UserData));
	}
	// ----
	static CTemporaryUserManager * Instance() 
	{
		if( m_pInstance == NULL )
		{
			m_pInstance = new CTemporaryUserManager;
		}
		// ----
		return m_pInstance;
	}
	// ----
	bool	AddUserData(const char * Name, int UserIndex, int PartyNumber);
	int		AddToPriviousParty(int UserIndex, int SlotIndex);
	int		ClearOverTimeUserData(int Second);
	int		TidyUpTempuserdate(int SlotIndex);
	void	DeleteUserInParty(int SlotIndex);
	int		DeleteAllPartyUserData(int PartyNumber);
	int		CheckMatchCharacter(const char *CharacterName);
	int		CheckMatchIndex(int UserIndex);
	// ----
	int		CheckInEventMap(int SlotIndex);
	void	LeaveEvent(int SlotIndex);
	int		ResumeEvent(int SlotIndex, int CurUserIndex);
	int		ResumeIllusionTemple(int SlotIndex, int CurUserIndex);
	int		ResumeImperialguardian(int SlotIndex, int CurUserIndex);
	int		ResumeBloodCastle(int SlotIndex, int CurUserIndex);
	int		ResumeDoppelganger(int SlotIndex, int CurUserIndex);
	int		ResumeDevilsquare(int SlotIndex, int CurUserIndex);
	void	SendEventMapInfo(int SlotIndex, int CurUserIndex);
	// ----
private:
	static	CTemporaryUserManager * m_pInstance;
	int		SearchEmptyIndex() const;
	void	ClearSpecificUserData(int SlotIndex);
	// ----
	TEMPORARY_USER_DATA m_UserData[MAX_TEMP_SLOT];
	CRITICAL_SECTION	m_cs;
};
// -------------------------------------------------------------------------