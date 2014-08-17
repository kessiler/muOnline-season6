// CrywolfDBSet.h: interface for the CCrywolfDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFDBSET_H__DE77448E_DEA3_4B5A_83A0_9F0B04EFF3DE__INCLUDED_)
#define AFX_CRYWOLFDBSET_H__DE77448E_DEA3_4B5A_83A0_9F0B04EFF3DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprodef.h"

#include "DBConBase.h"

typedef struct
{
	PBMSG_HEAD h;
	BYTE btResult;
	int iCrywolfState;
	int iOccupationState;
}CWP_ANS_CRYWOLFINFOLOAD, *LPCWP_ANS_CRYWOLFINFOLOAD;

typedef struct
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
	int iCrywolfState;
	int iOccupationState;
}CWP_REQ_CRYWOLFINFOSAVE, *LPCWP_REQ_CRYWOLFINFOSAVE;

struct CWP_REQ_CRYWOLFSYNC
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
	int iCrywolfState;
	int iOccupationState;
};

struct CWP_ANS_CRYWOLFSYNC
{
	PBMSG_HEAD h;
	BYTE btResult;
	int iCrywolfState;
	int iOccupationState;
};

struct CWP_REQ_CRYWOLFINFOLOAD
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;
};

struct CWP_ANS_CRYWOLFINFOSAVE
{
	PBMSG_HEAD h;
	BYTE btResult;
};

class CCrywolfDBSet  : public CDBConBase
{
public:
	CCrywolfDBSet();
	virtual ~CCrywolfDBSet();

	BOOL Connect();
	BOOL DSDB_QueryCrywolfStateUpdate();
	BOOL DSDB_QueryCrywolfInfoLoad(int iMapSvrGroup, LPCWP_ANS_CRYWOLFINFOLOAD lpAnsMsg);
	BOOL DSDB_QueryCrywolfInfoSave( int iMapSvrGroup, LPCWP_REQ_CRYWOLFINFOSAVE lpReqMsg);

};

#endif // !defined(AFX_CRYWOLFDBSET_H__DE77448E_DEA3_4B5A_83A0_9F0B04EFF3DE__INCLUDED_)
