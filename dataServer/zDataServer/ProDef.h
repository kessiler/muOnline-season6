#ifndef __PROTOCOLDEF__
#define __PROTOCOLDEF__
//////////////////////////////////////////////////////////////////////////////
// 프로토콜에 사용

#define PMHC_BYTE				0xC1 // 클라이언트가 보내는 1바이트짜리 프로토콜 
#define PMHC_WORD				0xC2 // 클라이언트가 보내는 2바이트짜리 프로토콜 
#define PMHCE_BYTE				0xC3 // 클라이언트가 보내는 1바이트짜리 프로토콜 
#define PMHCE_WORD				0xC4 // 클라이언트가 보내는 2바이트짜리 프로토콜 

#define SET_NUMBERH(x) ( (BYTE)((DWORD)(x)>>(DWORD)8) )
#define SET_NUMBERL(x) ( (BYTE)((DWORD)(x) & 0xFF) )

typedef struct
{
	void set ( LPBYTE lpBuf, BYTE head, BYTE size)
	{
		lpBuf[0] = PMHC_BYTE;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};

	void setE ( LPBYTE lpBuf, BYTE head, BYTE size)
	{
		lpBuf[0] = PMHCE_BYTE;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};	

	BYTE c;			// 프로토콜 코드
	BYTE size;		// 프로토콜 크기( 헤더 크기 포함 )
	BYTE headcode;	// 프로토콜 종류
} PBMSG_HEAD, *LPPBMSG_HEAD;

typedef struct
{
	void set( LPBYTE lpBuf, BYTE head, int size)
	{
		lpBuf[0] = PMHC_WORD;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
	};

	void setE( LPBYTE lpBuf, BYTE head, int size)
	{
		lpBuf[0] = PMHCE_WORD;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
	};

	BYTE c;			// 프로토콜 코드
	BYTE sizeH;		// 프로토콜 크기( 헤더 크기 포함 ) 상위
	BYTE sizeL;
	BYTE headcode;	// 프로토콜 종류
} PWMSG_HEAD, *LPPWMSG_HEAD;


typedef struct
{
	void set( LPBYTE lpBuf, BYTE head, BYTE sub, BYTE size)
	{
		lpBuf[0] = PMHC_BYTE;
		lpBuf[1] = size;
		lpBuf[2] = head;
		lpBuf[3] = sub;
	};

	BYTE c;			// 프로토콜 코드
	BYTE size;		// 프로토콜 크기( 헤더 크기 포함 )
	BYTE headcode;	// 프로토콜 종류
	BYTE subcode;	// 서브 프로토콜 종류
} PBMSG_HEAD2, * LPPBMSG_HEAD2;

typedef struct
{
	void set(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
	{
		lpBuf[0] = PMHC_WORD;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
		lpBuf[4] = sub;
	}

	BYTE c;			// 프로토콜 코드
	BYTE sizeH;		// 프로토콜 크기 상위 바이트
	BYTE sizeL;		// 프로토콜 크기 하위 바이트
	BYTE headcode;	// 프로토콜 종류
	BYTE subcode;	// 서브 프로토콜 종류
} PWMSG_HEAD2, * LPPWMSG_HEAD2;


//----------------------------------------------------------------------------
// 카운터
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		count;
} PBMSG_COUNT, * LPPBMSG_COUNT;


//----------------------------------------------------------------------------
// 카운터
//----------------------------------------------------------------------------
typedef struct
{
	PWMSG_HEAD	h;
	BYTE		count;
} PWMSG_COUNT, * LPPWMSG_COUNT;

//----------------------------------------------------------------------------
// SUBCODE 가 있는 카운터 
//----------------------------------------------------------------------------
typedef struct
{
	PBMSG_HEAD	h;
	BYTE		subcode;
	BYTE		count;
} PBMSGS_COUNT, * LPPBMSGS_COUNT;


#endif
