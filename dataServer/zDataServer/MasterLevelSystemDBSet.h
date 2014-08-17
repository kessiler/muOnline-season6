// MasterLevelSystemDBSet.h: interface for the CMasterLevelSystemDBSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MASTERLEVELSYSTEMDBSET_H__C2AB5642_4ACC_4340_969D_8AA74198B503__INCLUDED_)
#define AFX_MASTERLEVELSYSTEMDBSET_H__C2AB5642_4ACC_4340_969D_8AA74198B503__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Sprodef.h"

#include "DBConBase.h"

typedef struct
{
	PBMSG_HEAD h;
	BYTE btResult;
	int iUserIndex;
	char szCharName[MAX_IDSTRING+1];
	short nMLevel;
	__int64 i64MLExp;
	__int64 i64NextMLExp;
	short nMLPoint;
}MLP_ANS_MASTERLEVEL_INFO, *LPMLP_ANS_MASTERLEVEL_INFO;

typedef struct
{
	PBMSG_HEAD h;
	char szCharName[MAX_IDSTRING+1];
	short nMLevel;
	__int64 i64MLExp;
	__int64 i64NextMLExp;
	short nMLPoint;
}MLP_REQ_MASTERLEVEL_INFOSAVE, *LPMLP_REQ_MASTERLEVEL_INFOSAVE;

class CMasterLevelSystemDBSet  : public CDBConBase
{
public:
	CMasterLevelSystemDBSet();
	virtual ~CMasterLevelSystemDBSet();

	BOOL Connect();
	BOOL DSDB_QueryMasterLevelSystemLoad(char* szCharName, LPMLP_ANS_MASTERLEVEL_INFO lpReqInfo);
	BOOL DSDB_QueryMasterLevelSystemSave(LPMLP_REQ_MASTERLEVEL_INFOSAVE lpRecv);

};

#endif // !defined(AFX_MASTERLEVELSYSTEMDBSET_H__C2AB5642_4ACC_4340_969D_8AA74198B503__INCLUDED_)
