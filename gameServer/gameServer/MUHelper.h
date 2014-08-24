#pragma once
// -------------------------------------------------------------------------------

#define MUHELPER_DEBUG			0
// -------------------------------------------------------------------------------

#define MAX_HELPER_STAGE		5
#define DEF_HELPER_STAGETIME	200
#define DEF_HELPER_PAYTIME		5
// -------------------------------------------------------------------------------

struct MUHELPER_ANS_DATA	//-> Complete (from 6.3 GMO)
{
	PBMSG_HEAD2 h;			//-> 0	(4)
	WORD	Time;			//-> 4	(2)
	DWORD	Money;			//-> 8	(4)
	BYTE	Mode;			//-> 12 (1)
};
// -------------------------------------------------------------------------------

struct MUHELPER_REQ_ACTION	//-> Complete
{
	PBMSG_HEAD2 h;			//-> 0	(4)
	BYTE	Mode;			//-> 4	(1)
};
// -------------------------------------------------------------------------------

struct MUHELPER_MACRO_CLIENT//-> Complete
{
	PWMSG_HEAD h;			//-> 0	(4)
	BYTE	MacroBind[257];	//-> 4	(257)
};
// -------------------------------------------------------------------------------

struct MUHELPER_MACRO_DS	//-> Complete
{
	PWMSG_HEAD h;
	int		aIndex;
	char	AccountID[MAX_IDSTRING+1];
	char	Name[MAX_IDSTRING+1];
	BYTE	btResult;
	BYTE	btMacroData[256];
};
// -------------------------------------------------------------------------------

class MUHelper
{
public:
				MUHelper();
	virtual		~MUHelper();
	// ----
	void		Init();
	void		ReadData(char * File);
	void		Load();
	// ----
	bool		AddUserData(LPOBJ lpUser);
	// ----
	void		Manager(int UserIndex, MUHELPER_REQ_ACTION * lpRequest);
	void		Start(LPOBJ lpUser);
	void		Work(LPOBJ lpUser);
	void		Close(LPOBJ lpUser);
	// ----
	void		SetStage(LPOBJ lpUser, WORD WorkTime);
	int			GetMoney(LPOBJ lpUser);
	bool		CheckMoney(LPOBJ lpUser);
	void		SendMoney(LPOBJ lpUser, WORD WorkTime);
	// ----
	void		SaveMacro(int UserIndex, MUHELPER_MACRO_CLIENT * lpRequest);
	void		ReqMacro(int UserIndex);
	void		SendMacro(MUHELPER_MACRO_DS * lpRequest);
	// ----
private:
	int					StagePay[MAX_HELPER_STAGE];
	short				StageTime;
	short				PayTime;
	// ----
}; extern MUHelper g_MUHelper;
// -------------------------------------------------------------------------------