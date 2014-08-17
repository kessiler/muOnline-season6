// API 프로젝트에 사용될 경우 다음 두 줄을 포함시켜 주면 된다.
#include <windows.h>


// MFC 프로젝트에 사용될 경우는 stdafx.h에 CQuery.h를 포함시켜 주고 다음 한줄만
// 있으면 된다.
#include "stdafx.h"
#include "DelayHandler.h"

// 생성자:각종 초기화를 담당한다.
CQuery::CQuery()
{
	AffectCount=-1;
	ret=SQL_SUCCESS;

	// 환경 핸들을 할당하고 버전 속성을 설정한다.
	SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnv);
	SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER);
}

// 파괴자:연결 핸들을 해제한다.
CQuery::~CQuery()
{
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
}

// 연결 핸들을 할당하고 연결한 후 명령핸들까지 같이 할당한다.
// Type=1:ConStr은 MDB 파일의 경로를 가진다. 경로 생략시 현재 디렉토리에서 MDB를 찾는다.
// Type=2:ConStr은 SQL 서버의 연결 정보를 가지는 DSN 파일의 경로를 가진다. 
//        경로는 반드시 완전 경로로 지정해야 한다.
// Type=3:SQLConnect 함수로 DSN에 직접 연결한다.
// 연결 또는 명령 핸들 할당에 실패하면 FALSE를 리턴한다.
BOOL CQuery::Connect(int Type, char *ConStr, char *UID, char *PWD)
{
	SQLCHAR InCon[255];
	SQLCHAR OutCon[255];
    SQLSMALLINT cbOutCon;

	int ii=1;
	SQLRETURN Ret;
	SQLINTEGER NativeError;
	SQLCHAR SqlState[6], Msg[255];
	SQLSMALLINT MsgLen;
	char str[256];

	m_Type = Type;
	strcpy(m_szConnect, ConStr);
	strcpy(m_Id, UID);
	strcpy(m_Pass, PWD);

	// 연결 타입에 따라 MDB 또는 SQL 서버, 또는 DSN에 연결한다.
	SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc);
	switch (Type) {
	case 1:
		wsprintf((char *)InCon,"DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s;",ConStr);
		ret=SQLDriverConnect(hDbc,NULL,(SQLCHAR *)InCon,sizeof(InCon),OutCon,
			sizeof(OutCon),&cbOutCon, SQL_DRIVER_NOPROMPT);
		break;
	case 2:
		wsprintf((char *)InCon, "FileDsn=%s",ConStr);
		ret=SQLDriverConnect(hDbc,NULL,(SQLCHAR *)InCon,sizeof(InCon),OutCon,
			sizeof(OutCon),&cbOutCon, SQL_DRIVER_NOPROMPT);
		break;
	case 3:
		ret=SQLConnect(hDbc,(SQLCHAR *)ConStr,SQL_NTS,(SQLCHAR *)UID,SQL_NTS,
			(SQLCHAR *)PWD,SQL_NTS);
		break;
	}

	// 접속 에러시 진단 정보를 보여준다.
	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO)) {
		while (Ret=SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, ii, SqlState, &NativeError, 
			Msg, sizeof(Msg), &MsgLen)!=SQL_NO_DATA) {
			wsprintf(str, "(1) SQLSTATE:%s, Diagnosis:%s",(LPCTSTR)SqlState,(LPCTSTR)Msg);
			//::MessageBox(NULL,str,"진단 정보",0);
			gWindow.PrintLog(str);
			ii++;
		}
		return FALSE;
	}

	// 명령 핸들을 할당한다.
	ret=SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt);
	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO)) {
		hStmt=0;
		return FALSE;
	}
	return TRUE;
}

BOOL CQuery::ReConnect()
{
	return Connect(m_Type, m_szConnect, m_Id, m_Pass);
}

// SQL문을 실행한다. 실패시 진단 정보를 출력하고 FALSE를 리턴한다.
BOOL CQuery::Exec(LPCTSTR szSQL)
{
	int c;

	while( true )
	{		
		gWindow.PrintLog("%s", szSQL);
		// SQL문을 실행한다. SQL_NO_DATA를 리턴한 경우도 일단 성공으로 취급한다. 
		// 이 경우 Fetch에서 EOF를 리턴하도록 되어 있기 때문에 진단 정보를 출력할 필요는 없다.
		ret=SQLExecDirect(hStmt,(SQLCHAR *)szSQL,SQL_NTS);
		if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO) && (ret != SQL_NO_DATA))
		{			
			bool bReConnect = false;

			PrintDiag(bReConnect);

			Clear();

			if(bReConnect == true)
			{
				::Sleep(1);
				continue;
			}
			return FALSE;
		}

		// Update, Delete, Insert 명령시 영향받은 레코드 개수를 구해 놓는다.
		SQLRowCount(hStmt,&AffectCount);

		SQLNumResultCols(hStmt,&nCol);
		if (nCol > MAXCOL) {
			//::MessageBox(NULL,"최대 컬럼 수를 초과했습니다","CQuery 에러",MB_OK);
			gWindow.PrintLog("CQuery error :최대 컬럼 수를 초과했습니다");
			return FALSE;
		}

		// nCol이 0인 경우는 Select문 이외의 다른 명령을 실행한 경우이므로 
		// 바인딩을 할 필요가 없다.
		if (nCol == 0) {
			Clear();
			return TRUE;
		}

		// 모든 컬럼을 문자열로 바인딩해 놓는다. Col배열은 zero base, 
		// 컬럼 번호는 one base임에 유의할 것
		for (c=0;c<nCol;c++) {
			SQLBindCol(hStmt,c+1,SQL_C_CHAR,Col[c],255,&lCol[c]);
			SQLDescribeCol(hStmt,c+1,ColName[c],30,NULL,NULL,NULL,NULL,NULL);
		}
		return TRUE;
	}
}

BOOL CQuery::ExecH(LPCTSTR szSQL)
{
	int c;

	while( true )
	{		
		// SQL문을 실행한다. SQL_NO_DATA를 리턴한 경우도 일단 성공으로 취급한다. 
		// 이 경우 Fetch에서 EOF를 리턴하도록 되어 있기 때문에 진단 정보를 출력할 필요는 없다.
		ret=SQLExecDirect(hStmt,(SQLCHAR *)szSQL,SQL_NTS);
		if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO) && (ret != SQL_NO_DATA))
		{			
			bool bReConnect = false;

			PrintDiag(bReConnect);

			Clear();

			if(bReConnect == true)
			{
				::Sleep(1);
				continue;
			}
			return FALSE;
		}

		// Update, Delete, Insert 명령시 영향받은 레코드 개수를 구해 놓는다.
		SQLRowCount(hStmt,&AffectCount);

		SQLNumResultCols(hStmt,&nCol);
		if (nCol > MAXCOL) {
			//::MessageBox(NULL,"최대 컬럼 수를 초과했습니다","CQuery 에러",MB_OK);
			gWindow.PrintLog("CQuery error :최대 컬럼 수를 초과했습니다");
			return FALSE;
		}

		// nCol이 0인 경우는 Select문 이외의 다른 명령을 실행한 경우이므로 
		// 바인딩을 할 필요가 없다.
		if (nCol == 0) {
			Clear();
			return TRUE;
		}

		// 모든 컬럼을 문자열로 바인딩해 놓는다. Col배열은 zero base, 
		// 컬럼 번호는 one base임에 유의할 것
		for (c=0;c<nCol;c++) {
			SQLBindCol(hStmt,c+1,SQL_C_CHAR,Col[c],255,&lCol[c]);
			SQLDescribeCol(hStmt,c+1,ColName[c],30,NULL,NULL,NULL,NULL,NULL);
		}
		return TRUE;
	}
}

// SQL문을 실행하고 결과셋의 첫 칼럼에서 정수값을 읽어 리턴해 준다. 
// 결과를 얻은 후 정리까지 해 준다.
int CQuery::ExecGetInt(LPCTSTR szSQL)
{
	int i;

	//gWindow.PrintLog("%s", szSQL);

	if (Exec(szSQL) == FALSE) 
		return CQUERYERROR;

	// 데이터가 없는 경우 EOF를 리턴한다.
	if (Fetch()==SQL_NO_DATA) {
		Clear();
		return CQUERYEOF;
	}
	i=GetInt(1);
	Clear();
	return i;
}

int CQuery::ExecGetInt(char* szSQL, int * retvalue)
{
	gWindow.PrintLog("%s", szSQL);

	if (Exec(szSQL) == FALSE) 
	{
		return FALSE;
	}


	if (Fetch()==SQL_NO_DATA) 
	{
		Clear();

		return FALSE;
	}

	int i=GetInt(1);
	Clear();
	*retvalue = i;
	
	return TRUE;
}

// SQL문을 실행하고 결과셋의 첫 칼럼에서 문자열을 읽어 리턴해 준다.
BOOL CQuery::ExecGetStr(LPCTSTR szSQL, char *buf)
{
//	gWindow.PrintLog("%s", szSQL);

	// SQL문 실행중 에러가 발생한 경우 문자열 버퍼에 에러를 돌려 보낸다.
	if (Exec(szSQL) == FALSE) {
		//strcpy(buf, "CQUERYERROR");
		buf[0] = '\0';
		return FALSE;
	}

	// 데이터가 없는 경우 EOF를 리턴한다.
	if (Fetch()==SQL_NO_DATA) {
		Clear();
		//strcpy(buf,"EOF");
		buf[0] = '\0';
		return FALSE;
	}
	GetStr(1,buf);
	Clear();
	return TRUE;
}

// 결과셋에서 한 행을 가져온다.
SQLRETURN CQuery::Fetch()
{
	ret=SQLFetch(hStmt);
	return ret;
}

// 커서를 닫고 바인딩 정보를 해제한다.
void CQuery::Clear()
{
	SQLCloseCursor(hStmt);
	::SQLFreeStmt(hStmt, SQL_UNBIND);
}

// 컬럼 이름으로부터 컬럼 인덱스를 찾는다. 없을 경우 -1을 리턴한다.
int CQuery::FindCol(char *name)
{
	int i;
	for (i=0;i<nCol;i++) {
		if (stricmp(name,(LPCTSTR)ColName[i])==0)
			return i+1;
	}
	return -1;
}

// nCol의 컬럼값을 정수로 읽어준다. NULL일 경우 CQUERYNULL을 리턴한다.
int CQuery::GetInt(int nCol)
{
	if (nCol > nCol)
		return CQUERYNOCOL;

	if (lCol[nCol-1]==SQL_NULL_DATA) {
		return CQUERYNULL;
	} else {
		return atoi(Col[nCol-1]);
	}
}

// sCol의 컬럼값을 정수로 읽어준다.
int CQuery::GetInt(char *sCol)
{
	int n;
	n=FindCol(sCol);
	if (n==-1) {
		return CQUERYNOCOL;
	} else {
		return GetInt(n);
	}
}

__int64 CQuery::GetInt64(int nCol)
{
	if (nCol > nCol)
	{
		return CQUERYNOCOL;
	}

	if (lCol[nCol-1]==SQL_NULL_DATA) 
	{
		return CQUERYNULL;
	} 
	else 
	{
		return _atoi64(Col[nCol-1]);
	}

}

__int64 CQuery::GetInt64(char* sCol)
{
	int n;
	n = FindCol(sCol);

	if (n==-1) 
	{
		return CQUERYNOCOL;
	} 
	else 
	{
		return GetInt64(n);
	}
}

// nCol의 컬럼값을 실수로 읽어준다. NULL일 경우 CQUERYNULL을 리턴한다.
float CQuery::GetFloat(int nCol)
{
	if (nCol > nCol)
		return CQUERYNOCOL;

	if (lCol[nCol-1]==SQL_NULL_DATA) {
		return 0;//CQUERYNULL; //-> Temp
	} else {
		return (float)atof(Col[nCol-1]);
	}
}

// sCol의 컬럼값을 실수로 읽어준다.
float CQuery::GetFloat(char *sCol)
{
	int n;
	n=FindCol(sCol);
	if (n==-1) {
		return CQUERYNOCOL;
	} else {
		return GetFloat(n);
	}
}


// nCol의 컬럼값을 문자열로 읽어준다. NULL일 경우 문자열에 NULL을 채워준다. 
// buf의 길이는 최소한 256이어야 하며 길이 점검은 하지 않는다.
void CQuery::GetStr(int nCol, char *buf)
{
	if (nCol > nCol) {
		//strcpy(buf, "CQUERYNOCOL");
		buf[0] = '\0';
		return;
	}

	if (lCol[nCol-1]==SQL_NULL_DATA) 
	{
		buf[0] = '\0';
		//lstrcpy(buf,"NULL");
	} else {
		lstrcpy(buf,Col[nCol-1]);
	}
}

// sCol의 컬럼값을 문자열로 읽어준다.
void CQuery::GetStr(char *sCol, char *buf)
{
	int n;
	n=FindCol(sCol);
	if (n==-1) {
		//lstrcpy(buf,"ERROR:Colume not found");
		buf[0] = '\0';
	} else {
		GetStr(n, buf);
	}
}

// 에러 발생시 진단 정보를 출력해 준다.
void CQuery::PrintDiag(bool &bReconnect)
{
	int ii;
	SQLRETURN Ret;
	SQLINTEGER NativeError;
	SQLCHAR SqlState[6], Msg[255];
	SQLSMALLINT MsgLen;
	char str[256];

	ii=1;
	while (Ret=SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, ii, SqlState, &NativeError, 
		Msg, sizeof(Msg), &MsgLen)!=SQL_NO_DATA) {
		wsprintf(str, "(2) SQLSTATE:%s, Diagnosis:%s",(LPCTSTR)SqlState,(LPCTSTR)Msg);
		//::MessageBox(NULL,str,"진단 정보",0);
		gWindow.PrintLog(str);
		//LogAdd(str);
		ii++;
		if(ii > 3) break;
	}

	// 오류가 통신 오류라면.. 다시 접속해 본다.
	if( strcmp((LPCTSTR)SqlState, "08S01") == 0 )
	{
		if(ReConnect() == 1) g_DelayHandler.IncreaseQuerySessionId();

		bReconnect = true;
	}
	// 복구 불가능한 에러가 발생한 경우 프로그램을 종료한다. 극단적 예외 처리가 불필요한
	// 경우는 이 코드를 주석 처리하거나 적당하게 다른 루틴으로 바꿔야 한다.
/*	if (ii > 1) {
		::MessageBox(NULL,"진단 정보가 출력되었습니다. 이 정보가 출력될 경우는 네트웍 끊김, DB 서버 중지 등의\r\n"
			"복구 불가능한 에러가 발생한 경우이며 프로그램 실행을 계속할 수 없습니다.\r\n"
			"에러를 수정하신 후 프로그램을 다시 실행해 주십시오.","Critical Error",MB_OK | MB_ICONERROR);

		// 다음 둘 중 하나를 선택할 것
		PostQuitMessage(0);
		// ExitProcess(0);
	}
*/
}

// BLOB 데이터를 buf에 채워준다. 이때 buf는 충분한 크기의 메모리를 미리 할당해 
// 놓아야 한다. NULL일 경우 0을 리턴하고 에러 발생시 -1을 리턴한다. 성공시 읽은 
// 총 바이트 수를 리턴한다. szSQL은 하나의 BLOB 필드를 읽는 Select SQL문이어야 한다.
int CQuery::ReadBlob(LPCTSTR szSQL, void *buf)
{
	SQLCHAR BinaryPtr[BLOBBATCH];
	SQLINTEGER LenBin;
	char *p;
	int nGet;
	int TotalGet=0;

	//gWindow.PrintLog("%s", szSQL);

	while( true )
	{
		ret=SQLExecDirect(hStmt,(SQLCHAR *)szSQL,SQL_NTS);
		if (ret!=SQL_SUCCESS)
		{
			bool bReconnect = false;

			PrintDiag(bReconnect);

			if(bReconnect == true){
				::Sleep(1);
				continue;
			}

			return -1;
		}

		while ((ret=SQLFetch(hStmt)) != SQL_NO_DATA) {
			p=(char *)buf;
			while ((ret=SQLGetData(hStmt,1,SQL_C_BINARY,BinaryPtr,sizeof(BinaryPtr),
				&LenBin))!=SQL_NO_DATA) {
				if (LenBin==SQL_NULL_DATA) {
					Clear();
					return 0;
				}
				if (ret==SQL_SUCCESS)
					nGet=LenBin;
				else
					nGet=BLOBBATCH;
				TotalGet+=nGet;
				memcpy(p,BinaryPtr,nGet);
				p+=nGet;
			}
		}
		Clear();
		return TotalGet;
	}
}

// buf의 BLOB 데이터를 저장한다. szSQL은 하나의 BLOB 데이터를 저장하는 Update, Insert
// SQL문이어야 한다.
void CQuery::WriteBlob(LPCTSTR szSQL, void *buf, int size)
{
	SQLINTEGER cbBlob;
	char tmp[BLOBBATCH],*p;
	SQLPOINTER pToken;
	int nPut;

	//gWindow.PrintLog("%s", szSQL);

	cbBlob=SQL_LEN_DATA_AT_EXEC(size);
	SQLBindParameter(hStmt,1,SQL_PARAM_INPUT,SQL_C_BINARY,SQL_LONGVARBINARY,
		size,0,(SQLPOINTER)1,0,&cbBlob);
	SQLExecDirect(hStmt,(SQLCHAR *)szSQL,SQL_NTS);
	ret=SQLParamData(hStmt, &pToken);
	while (ret==SQL_NEED_DATA) {
		if (ret==SQL_NEED_DATA) {
			if ((int)pToken==1) {
				for (p=(char *)buf;p<(char *)buf+size;p+=BLOBBATCH) {
					nPut=min(BLOBBATCH,(char *)buf+size-p);
					memcpy(tmp,p,nPut);
					SQLPutData(hStmt,(PTR)tmp,nPut);
				}
			}
		}
		ret=SQLParamData(hStmt, &pToken);
	}
	Clear();
}

int CQuery::BindParameterBinaryOutput(int nCol, BYTE *nValue, int iSize, long *lLength)
{
	int iRet = SQLBindParameter(hStmt, nCol, 4, -2, -3, iSize, 0, nValue, iSize, lLength);
	return iRet;
}