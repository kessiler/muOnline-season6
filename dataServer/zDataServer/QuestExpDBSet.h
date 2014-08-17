// QuestExpDBSet.h: interface for the CQuestExpDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTEXPDBSET_H__2E2B4157_4EDA_4522_8330_AE2C67318E1E__INCLUDED_)
#define AFX_QUESTEXPDBSET_H__2E2B4157_4EDA_4522_8330_AE2C67318E1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprodef.h"

#include "DBConBase.h"

typedef struct
{
	PWMSG_HEAD h;
	BYTE btQuestCnt;
	char szCharName[MAX_IDSTRING+1];
}_PMSG_QUESTEXP_INFO, *LP_PMSG_QUESTEXP_INFO;

#pragma pack(1)
typedef struct _QUESTEXP_INFO
{
	_QUESTEXP_INFO::_QUESTEXP_INFO();

	DWORD dwQuestIndexID;
	WORD wProgState;
	BYTE btAskIndex[5];
	BYTE btAskValue[5];
	BYTE btAskState[5];
	long lStartDate;
	long lEndDate;
}_QUESTEXP_INFO, *LP_QUESTEXP_INFO;
#pragma pack()

#pragma pack(1)
typedef struct
{
	PWMSG_HEAD head;
	BYTE btQuestCnt;
	int iUserIndex;
}PMSG_ANS_QUESTEXP_INFO, *LPPMSG_ANS_QUESTEXP_INFO;
#pragma pack()

class CQuestExpDBSet  : public CDBConBase
{
public:
	CQuestExpDBSet();
	virtual ~CQuestExpDBSet();

	BOOL Connect();
	BOOL DSDB_QueryQuestInfoSave(LP_PMSG_QUESTEXP_INFO lpRecv);
	BOOL DSDB_QueryQuestInfoLoad(char* szCharName, LP_QUESTEXP_INFO pQuestInfo, LPPMSG_ANS_QUESTEXP_INFO pMsg);
};

#endif // !defined(AFX_QUESTEXPDBSET_H__2E2B4157_4EDA_4522_8330_AE2C67318E1E__INCLUDED_)
