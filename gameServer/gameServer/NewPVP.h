#pragma once

#pragma warning ( disable : 4786 )

#include <map>
using namespace std;

#include "ProbabilityItemBag.h"
#include "..\\Common\\Winutil.h"

#define DUEL_CHANNEL_MAX 4
#define DC_IDLE 0
#define DC_RESERVEDEND 5
#define IS_START(nId) ( (nId) >= 2 && (nId) <= 4 )

enum _MAP_NEWPVP_PROTOCOL
{
	MSG_DUEL_INVITE = 0xaa01,
	MSG_DUEL_ANSWER = 0xaa02,
	MSG_DUEL_LEAVE = 0xaa03,
	MSG_DUEL_SCORE_BROADCAST = 0xaa04,
	MSG_DUEL_HP_BROADCAST = 0xaa05,
	MSG_DUEL_CHANNELLIST = 0xaa06,
	MSG_DUEL_JOINCNANNEL = 0xaa07,
	MSG_DUEL_JOINCNANNEL_BROADCAST = 0xaa08,
	MSG_DUEL_LEAVECNANNEL = 0xaa09,
	MSG_DUEL_LEAVECNANNEL_BROADCAST = 0xaa0a,
	MSG_DUEL_OBSERVERLIST_BROADCAST = 0xaa0b,
	MSG_DUEL_RESULT_BROADCAST = 0xaa0c,
	MSG_DUEL_ROUNDSTART_BROADCAST = 0xaa0d,
};

namespace ENEWPVP
{
	enum EERROR
	{
		E_NO_ERROR = 0x0,
		E_FAILED_RESULT = 0x1,
		E_INVALID_USER = 0x2,
		E_NON_PKSERVER = 0x3,
		E_NOT_DUEL_CSSERVER = 0x4,
		E_CONNECTION_CLOSING = 0x5,
		E_NOT_DUEL_MURDERER = 0x6,
		E_ALREADY_DUELLED = 0x7,
		E_INVALID_MAP = 0x8,
		E_LIMIT_PACKETTIME = 0x9,
		E_INVITE_MYSELF = 0xa,
		E_INVALID_INDEX = 0xb,
		E_DISCONNECT = 0xc,
		E_SELFDEFENSE = 0xd,
		E_GUILDWAR = 0xe,
		E_REFUSE_INVATE = 0xf,
		E_DUEL_MAX = 0x10,
		E_INVALID_STATUS = 0x11,
		E_ALREADY_DUELREQUESTED = 0x12,
		E_ALREADY_DUELRESERVED = 0x13,
		E_ALREADY_DUELLING = 0x14,
		E_ALREADY_DUELREQUESTED_1 = 0x15,
		E_ALREADY_DUELRESERVED_1 = 0x16,
		E_ALREADY_DUELLING_1 = 0x17,
		E_INVALID_CHANNELID = 0x18,
		E_FAILED_ENTER = 0x19,
		E_NOT_EXIST_USER = 0x1a,
		E_OBSERVER_MAX = 0x1b,
		E_LIMIT_LEVEL = 0x1c,
		E_NOT_FOUND_MOVEREQUESTDATA = 0x1d,
		E_NOT_ENOUGH_MONEY = 0x1e,
	};
};

typedef struct _tagPMSG_DUEL_OBSERVERLIST_BROADCAST
{
	PBMSG_HEAD2 h;
	BYTE nCount;
	
	struct //check union
	{
		char szName[MAX_IDSTRING];
	}
	user[10];

} PMSG_DUEL_OBSERVERLIST_BROADCAST, *LPMSG_DUEL_OBSERVERLIST_BROADCAST;

typedef struct _tagPMSG_ANS_DUEL_CHANNELLIST
{
	PBMSG_HEAD2 h;

	struct //check union
	{
		char szName1[MAX_IDSTRING];
		char szName2[MAX_IDSTRING];
		BYTE bStart;
		BYTE bWatch;
	}
	channel[DUEL_CHANNEL_MAX];
	
} PMSG_ANS_DUEL_CHANNELLIST, *LPMSG_ANS_DUEL_CHANNELLIST;

typedef struct _tagDUEL_CHANNEL
{
	int nId;
	int nIndex1;
	int nIndex2;
	char szName1[MAX_IDSTRING+1];
	char szName2[MAX_IDSTRING+1];
	BYTE nStatus;
	DWORD dwTime;
	short nNotifyCount;
} DUEL_CHANNEL, *LPDUEL_CHANNEL;

typedef struct _tagWaiting
{
	int nRequester;
	int nResponsor;
	int bExpired;
	DWORD dwTime;
} Waiting, *LPWaiting ;

typedef struct _tagObserverInfo
{
	int nId;
	int nIndex;
	char szName[MAX_IDSTRING+1];
} ObserverInfo, *LPObserverInfo ;

typedef struct _tagPMSG_REQ_DUEL_ANSWER 
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	char szName[MAX_IDSTRING];
} PMSG_REQ_DUEL_ANSWER, *LPMSG_REQ_DUEL_ANSWER;

typedef struct _tagPMSG_ANS_DUEL_INVITE 
{
	PBMSG_HEAD2 h;
	BYTE nResult;
	BYTE NumberH;
	BYTE NumberL;
	char szName[MAX_IDSTRING];
} PMSG_ANS_DUEL_INVITE, *LPMSG_ANS_DUEL_INVITE;

typedef struct _tagPMSG_ANS_DUEL_EXIT 
{
	PBMSG_HEAD2 h;
	BYTE nResult;
	BYTE NumberH;
	BYTE NumberL;
	char szName[MAX_IDSTRING];
} PMSG_ANS_DUEL_EXIT, *LPMSG_ANS_DUEL_EXIT ;

typedef struct _tagPMSG_ANS_DUEL_JOINCNANNEL 
{
	PBMSG_HEAD2 h;
	BYTE nResult;
	BYTE nChannelId;
	char szName1[MAX_IDSTRING];
	char szName2[MAX_IDSTRING];
	BYTE NumberH1;
	BYTE NumberL1;
	BYTE NumberH2;
	BYTE NumberL2;
} PMSG_ANS_DUEL_JOINCNANNEL, *LPMSG_ANS_DUEL_JOINCNANNEL ;

typedef struct _tagPMSG_ANS_DUEL_LEAVECNANNEL 
{
	PBMSG_HEAD2 h;
	BYTE nResult;
} PMSG_ANS_DUEL_LEAVECNANNEL, *LPMSG_ANS_DUEL_LEAVECNANNEL ;

typedef struct _tagPMSG_DUEL_SCORE_BROADCAST 
{
	PBMSG_HEAD2 h;
	BYTE NumberH1;
	BYTE NumberL1;
	BYTE NumberH2;
	BYTE NumberL2;
	BYTE btDuelScore1;
	BYTE btDuelScore2;

} PMSG_DUEL_SCORE_BROADCAST, *LPMSG_DUEL_SCORE_BROADCAST;

typedef struct _tagPMSG_DUEL_RESULT_BROADCAST 
{
	PBMSG_HEAD2 h;
	char szWinner[MAX_IDSTRING];
	char szLoser[MAX_IDSTRING];
} PMSG_DUEL_RESULT_BROADCAST, *LPMSG_DUEL_RESULT_BROADCAST ;

typedef struct _tagPMSG_DUEL_HP_BROADCAST 
{
	PBMSG_HEAD2 h;
	BYTE NumberH1;
	BYTE NumberL1;
	BYTE NumberH2;
	BYTE NumberL2;
	BYTE nHP1;
	BYTE nHP2;
	BYTE nShield1;
	BYTE nShield2;
} PMSG_DUEL_HP_BROADCAST, *LPMSG_DUEL_HP_BROADCAST ;

typedef struct _tagPMSG_DUEL_JOINCNANNEL_BROADCAST 
{
	PBMSG_HEAD2 h;
	char szName[MAX_IDSTRING];
} PMSG_DUEL_JOINCNANNEL_BROADCAST, *LPMSG_DUEL_JOINCNANNEL_BROADCAST ;

typedef struct _tagPMSG_DUEL_LEAVECNANNEL_BROADCAST 
{
	PBMSG_HEAD2 h;
	char szName[MAX_IDSTRING];
} PMSG_DUEL_LEAVECNANNEL_BROADCAST, *LPMSG_DUEL_LEAVECNANNEL_BROADCAST ;

typedef struct _tagPMSG_DUEL_ROUNDSTART_BROADCAST 
{
	PBMSG_HEAD2 h;
	BYTE nFlag;
} PMSG_DUEL_ROUNDSTART_BROADCAST, *LPMSG_DUEL_ROUNDSTART_BROADCAST;

typedef struct _tagPMSG_REQ_DUEL_INVITE
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	char szName[MAX_IDSTRING];
} PMSG_REQ_DUEL_INVITE, *LPPMSG_REQ_DUEL_INVITE;

typedef struct _tagPMSG_ANS_DUEL_ANSWER
{
	PBMSG_HEAD2 h;
	BYTE bDuelOK;
	BYTE NumberH;
	BYTE NumberL;
} PMSG_ANS_DUEL_ANSWER, *LPPMSG_ANS_DUEL_ANSWER;

typedef struct _tagPMSG_REQ_DUEL_EXIT
{
	PBMSG_HEAD2 h;
} PMSG_REQ_DUEL_EXIT, *LPPMSG_REQ_DUEL_EXIT;

typedef struct _tagPMSG_REQ_DUEL_JOINCNANNEL
{
	PBMSG_HEAD2 h;
	BYTE nChannelId;
} PMSG_REQ_DUEL_JOINCNANNEL, *LPPMSG_REQ_DUEL_JOINCNANNEL;

typedef struct _tagPMSG_REQ_DUEL_LEAVECNANNEL
{
	PBMSG_HEAD2 h;
	BYTE nChannelId;
} PMSG_REQ_DUEL_LEAVECNANNEL, *LPPMSG_REQ_DUEL_LEAVECNANNEL;

class CNewPVP 
{
public:
	CNewPVP();
	virtual ~CNewPVP();

	void LoadData();
	void Run();

	int Reserve(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);
	int Join(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);
	void Cancel(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor, BOOL bSendPacket);
	int Leave(OBJECTSTRUCT& obj);
	void SetScore(OBJECTSTRUCT& obj);
	void CheckScore(OBJECTSTRUCT& obj, OBJECTSTRUCT& target);
	BOOL SendChannelList(OBJECTSTRUCT& npc, OBJECTSTRUCT& obj);
	int JoinChannel(int nId, OBJECTSTRUCT& obj);
	int LeaveChannel(int nId, OBJECTSTRUCT& obj);
	void ChatMsgSend(OBJECTSTRUCT& obj, BYTE* Msg, int size);
	void SetBuff(OBJECTSTRUCT& obj, BOOL bUse);
	int GetChannelStatus(OBJECTSTRUCT& obj);
	BOOL IsSafeState(OBJECTSTRUCT& obj);

	BOOL IsDuelRequested(OBJECTSTRUCT& obj)	//OK
	{
		return CHECK_LIMIT(obj.m_iDuelUserRequested, OBJMAX);
	}
	
	BOOL IsDuelReserved(OBJECTSTRUCT &obj)	//OK
	{
		return CHECK_LIMIT(obj.m_iDuelUserReserved, OBJMAX);
	}
	
	BOOL IsDuel(OBJECTSTRUCT& obj,int nIndex)	//OK
	{
		if(CHECK_LIMIT(obj.m_iDuelUser, OBJMAX) && obj.m_iDuelUser == nIndex)
		{
			return TRUE;
		}
		return FALSE;
	}

	BOOL IsDuel(OBJECTSTRUCT& obj)	//~OK
	{
		return CHECK_LIMIT(obj.m_iDuelUser, OBJMAX);
	}

	int IsObserver(OBJECTSTRUCT& obj);
	void Reset(OBJECTSTRUCT& obj);
	BOOL CheckPKPenalty(LPOBJ lpObj);
	BOOL IsSelfDefense(LPOBJ lpObj);
	BOOL IsGuildWar(LPOBJ lpObj);

	int IsVulcanusMap(int nMapNumber)	//OK
	{
		return nMapNumber == MAP_INDEX_VULCANUS;
	}

	int IsPKFieldMap(int nMapNumber)	//OK
	{
		return nMapNumber == MAP_INDEX_PKFIELD;
	}

	int IsVulcanusMonster(WORD wClass)	//OK
	{
		return wClass == 481 || wClass == 483 || wClass == 485 || wClass == 488 || wClass == 491;
	}

	int IsEnable();

	BOOL DropItem(LPOBJ lpObj,BYTE btMapNumber,BYTE cX, BYTE cY, int nMaxHitUser);

private:
	int GetDuelChannelId(char* lpszName);
	int GetDuelChannelId(int nIndex);
	int GetDuelChannelId();

	LPDUEL_CHANNEL GetDuelChannel(char* lpszName);
	LPDUEL_CHANNEL GetDuelChannel();

	void SetStatus(int nStatus,int nId, OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);
	void SetDuelStatus(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor, int nStatus);
	
	int GetDuelStatus(OBJECTSTRUCT &obj)	//OK
	{
		if(CHECK_LIMIT(obj.m_iDuelUserRequested, OBJMAX))
		{
			return ENEWPVP::E_ALREADY_DUELREQUESTED;
		}

		if(CHECK_LIMIT(obj.m_iDuelUserReserved, OBJMAX))
		{
			return ENEWPVP::E_ALREADY_DUELRESERVED;
		}

		if(CHECK_LIMIT(obj.m_iDuelUser, OBJMAX))
		{
			return ENEWPVP::E_ALREADY_DUELLING;
		}

		return 0;
	};

	int GetDuelStatusByResponsor(OBJECTSTRUCT &obj)	//OK
	{
		if(CHECK_LIMIT(obj.m_iDuelUserRequested, OBJMAX))
		{
			return ENEWPVP::E_ALREADY_DUELREQUESTED_1;
		}

		if(CHECK_LIMIT(obj.m_iDuelUserReserved, OBJMAX))
		{
			return ENEWPVP::E_ALREADY_DUELRESERVED_1;
		}

		if(CHECK_LIMIT(obj.m_iDuelUser, OBJMAX))
		{
			return ENEWPVP::E_ALREADY_DUELLING_1;
		}

		return 0;
	};

	void Leave(OBJECTSTRUCT& requester, OBJECTSTRUCT& responsor);
	void ReFill(OBJECTSTRUCT & obj);
	BOOL MoveGate(int nIndex,int nGateNum);
	BOOL CheckLimitLevel(int nIndex, int nGateNum);
	int GetChannelIdByObserver(OBJECTSTRUCT& obj);
	int CheckUsersOnConnect(int nId);
	void GetObserverList(int nId,PMSG_DUEL_OBSERVERLIST_BROADCAST& res);
	void LeaveChannelObserver(int nId);
	void CheckChannelListInfo(int nId);

	void BroadcastScore(int nId, BYTE nFlag);
	void BroadcastResult(int nId,BYTE nFlag, OBJECTSTRUCT& obj);
	void BroadcastDuelInfo(int nId,BYTE nFlag);
	void BroadcastJoinChannel(int nId,OBJECTSTRUCT& obj);
	void BroadcastLeaveChannel(int nId,OBJECTSTRUCT& obj);
	void BroadcastToObserver(int nId,BYTE* lpData,int nSize);
	void BroadcastMessage(int nId,BYTE nFlag, BYTE nMsgType,int nNotifySec);
	void BroadcastRound(int nId, BYTE nFlag, BOOL bEnd);

public:
	BOOL m_bNewPVP;

	PMSG_ANS_DUEL_CHANNELLIST m_DuelChannelList;
	DUEL_CHANNEL m_DuelChannel[DUEL_CHANNEL_MAX];

	std::map<int,Waiting> m_Waiting;
	std::map<int,ObserverInfo> m_ObserverInfoList;

	int m_ObserverCount[DUEL_CHANNEL_MAX];

	CProbabilityItemBag* m_pVulcanuProbability;
};

extern CNewPVP g_NewPVP;