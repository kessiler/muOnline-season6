// QuestUtil.h: interface for the CQuestUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTUTIL_H__EFA29188_3CBA_4D31_B70C_77788DEB3888__INCLUDED_)
#define AFX_QUESTUTIL_H__EFA29188_3CBA_4D31_B70C_77788DEB3888__INCLUDED_


#include "user.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct PMSG_004A2100
{
	PBMSG_HEAD h;	// C1
	int iUserIndex;	//
	char chCharacterName[11];	//
};

typedef struct
{
	PBMSG_HEAD h;
	int Index;
	char szName[11];
	int QuestIndex;
	MONSTERKILLINFO KillInfo[5];
}PMSG_ANS_MONSTERKILLINFO, *LPMSG_ANS_MONSTERKILLINFO;

typedef struct
{
	PBMSG_HEAD h;
	int Index;
	char szName[11];
	int QuestIndex;
	MONSTERKILLINFO KillInfo[5];
}PMSG_SAVE_MONSTERKILLINFO, *LPMSG_SAVE_MONSTERKILLINFO;

class CQuestUtil  
{
public:
	CQuestUtil();
	virtual ~CQuestUtil();

	void Quest3rdRequestInfo(LPOBJ lpObj);
	void Quest3rdLoadInfo(LPBYTE aRecv);
	void Quest3rdSaveInfo(LPOBJ lpObj);
};

extern CQuestUtil g_QuestUtil;

#endif // !defined(AFX_QUESTUTIL_H__EFA29188_3CBA_4D31_B70C_77788DEB3888__INCLUDED_)
