#include <sql.h>
#include <sqlext.h>

class CQuery
{
public:
	// 최대 컬럼수, BLOB 입출력 단위, NULL 필드값
	enum { MAXCOL=100, BLOBBATCH=10000, CQUERYNULL=-100, CQUERYEOF=-101, 
		CQUERYNOCOL=-102, CQUERYERROR=-103 };
private:
	SQLHENV hEnv;							// 환경 핸들
	SQLHDBC hDbc;							// 연결 핸들
	SQLRETURN ret;							// 최후 실행한 SQL문의 결과값
	char Col[MAXCOL][255];					// 바인딩될 컬럼 정보
	
	int	m_Type;
	char m_szConnect[128];
	char m_Id[20];
	char m_Pass[20];

	int FindCol(char *name);				// 컬럼의 이름으로부터 번호를 찾아준다.
	BOOL ReConnect();
public:
	SQLINTEGER AffectCount;					// 영향받은 레코드 개수
	SQLHSTMT hStmt;							// 명령 핸들. 직접 사용할 수도 있으므로 public으로 정의
	SQLSMALLINT nCol;						// 컬럼 개수
	SQLCHAR ColName[MAXCOL][50];			// 컬럼의 이름들
	SQLINTEGER lCol[MAXCOL];				// 컬럼의 길이/상태 정보

	void PrintDiag(bool &bReconnect);						// 진단 정보 출력
	CQuery();								// 생성자
	~CQuery();								// 파괴자:연결 핸들을 해제한다.
	BOOL Connect(int Type, char *ConStr, char *UID=NULL, char *PWD=NULL);	// 데이터 소스에 연결한다.
	BOOL Exec(LPCTSTR szSQL);				// SQL문을 실행한다.
	BOOL ExecH(LPCTSTR szSQL);				// SQL문을 실행한다.
	int ExecGetInt(LPCTSTR szSQL);			// SQL문을 실행하고 첫번째 컬럼의 정수 읽음
	int ExecGetInt(char* szSQL, int * retvalue);
	BOOL ExecGetStr(LPCTSTR szSQL, char *buf);		// SQL문을 실행하고 첫번째 컬럼의 문자열 읽음
	SQLRETURN Fetch();						// 한 행 가져오기
	void Clear();							// 커서 해제 및 언 바인딩
	int GetInt(int nCol);					// 정수형 컬럼 읽기
	int GetInt(char *sCol);					// 정수형 컬럼 읽기
	__int64 GetInt64(char* sCol);
	__int64 GetInt64(int nCol);
	float GetFloat(int nCol);
	float GetFloat(char *sCol);
	void GetStr(int nCol, char *buf);		// 문자열형 컬럼 읽기
	void GetStr(char *sCol, char *buf);		// 문자열형 컬럼 읽기
	int ReadBlob(LPCTSTR szSQL, void *buf);
	void WriteBlob(LPCTSTR szSQL, void *buf, int size);
	int BindParameterBinaryOutput(int nCol, BYTE *nValue, int iSize, long *lLength);
};
