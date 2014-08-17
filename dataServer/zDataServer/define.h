//----------------------------------------------------------------------------
// MU 서버프로젝트 전반에 걸쳐 정의되어 사용되어지는 것들..
//----------------------------------------------------------------------------
#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "Public.h"

//----------------------------------------------------------------------------
// 컴파일시에 사용되는 것
//#define _DEBUG_PROTOCOL					// 프로토콜을 디버그 시에 사용 
#define LOG_PRINT			1				// 로그를 출력할것인가?
#define SPEEDHACK_CHECK		1				// 스피드핵을 체크할것인가?
#define TESTSERVER	 		1				// 테스트 서버인가..
#define LOCALCONNECT		0				// 사내에서만 접속 가능하게..
#define DATANOSAVE			0				// 데이터가 저장되지 않게 한다.

//----------------------------------------------------------------------------
#define CERTIFYTYPE_ACCOUNT		0			// 계정인증
#define CERTIFYTYPE_IP			1			// IP 인증

#define BILLTYPE_JUNGYANG		0			// 정량 
#define BILLTYPE_JUNGACK		1			// 정액 
#define BILLTYPE_JONGYANG		2			// 종량
#define BILLTYPE_TIME			3			// 시간제		
#define BILLTYPE_DATE			4			// 날짜제
#define BILLTYPE_NOCHARGE		5			// 무료

//----------------------------------------------------------------------------
// 인증서버에 접속하는 프로그램의 타입
#define ST_GAMESERVER				1			// 게임서버 번호

#define DEFMAPSERVERPORT			55900		// Map Server port number
#define DEFJOINSERVERPORT			55970		// Join Server port number
#define DEFDATASERVERPORT			55960		// Game Data Server Port number

//----------------------------------------------------------------------------
#define MAX_ROADPATH				15		// 사용자가 이동하는 PATH 최대 크기
#define MAX_PATHDIR					8		// 이동에 사용되는 방향 개수
#define MAX_IDSTRING				10		// 아이디 크기
#define DEFAULT_MONEY				0		// 초기 주어지는 돈 액수
#define MAX_LEVEL					400		// 레벨 최대 크기

//----------------------------------------------------------------------------
// 계정정보에 사용되는 정의들..
#define MAX_JOOMINNUMBERSTR			13		// 주민등록번호 스트링 크기 -를 뺸 크기이다.
#define MAX_QUESTIONSTR				30		// 질문 스트링 크기
#define MAX_ANSWERSTR				30		// 답변 스트링 크기
#define MAX_PHONENUMBERSTR			20		// 전화번호 스트링 크기
#define MAX_EMAILSTR				50		// e-mail 스트링 크기

#define MAXVIEWPORTOBJECT			75		// 뷰포트 최대 OBJECT 수 
#define MAX_MONVIEWPORTOBJECT		20		// 몬스터의 뮤에는 최대 10 마리 까지 표시


#define MAXUSERCHARACTER			5		// 유저당 가질수 있는 최대 캐릭터 수


#define MAX_GUILDNAMESTRING			8		// 길드 이름 스트링 길이

#define MAX_INVENTORYMAP			8*8

#define MAX_PSHOP_SIZE				8*4

#define MAX_ITEMDBBYTE				16		// 아이템이 DB에 저장되는 바이트 수

#define MAX_EQUIPMENT				12

#define MAX_DBINVENTORY				3776//((MAX_INVENTORYMAP+MAX_EQUIPMENT+MAX_PSHOP_SIZE)*MAX_ITEMDBBYTE) // (8x8x10)+120 데이터베이스 읽기&저장되는 인벤토리데이터 크기


#define MAX_ITEMDBBYTE_V00			7		// 아이템이 DB에 저장되는 바이트 수


//#define MAX_DBEQUIPMENT				84		// (12x7) 데이터베이스 읽기&저장되는 장비데이터 크기
//#define MAX_DBINVENTORY				532		// (8x8x7)+84 데이터베이스 읽기&저장되는 인벤토리데이터 크기
//#define MAX_ITEMDBBYTE				7		// 아이템이 DB에 저장되는 바이트 수

#define MAX_WAREHOUSEDBSIZE			3840//1920
#define MAX_WAREHOUSEITEMS			(8*15)

#define MAX_CHAOSBOXSIZE			8*4
#define MAX_CHAOSBOXITEMS			8*4

#define MAX_QUEST					50

#define MAX_INVENTORY				76		// (8x8)+12 인벤토리 최대 개수
#define MAX_TRADEITEM				32		// (8x4) 

#define MAX_DBMAGIC					(MAX_MAGIC*3)	// 데이터베이스에 저장되는 마법 데이터 크기

//----------------------------------------------------------------------------
// Server Type
#define ST_JOINSERVER				0		// 인증 접속 서버
#define ST_GAMESERVER				1		// 게임 서버
#define MAX_CHAT					60		// 대화 최대 크기
#define MAX_GUILDNOTICE				60		// 최대 길드 공지 길이

//----------------------------------------------------------------------------
// 캐릭터에 관련된 정의들
#define PK_MINIMUM_LEVEL			5		// 사용자끼리 죽일수 있는 최소 레벨 
#define MAX_CLASSTYPE				7		// 최대 종족 개수

#define MAX_PREVIEWCHARSET			9		// 캐릭터 세트 최대 크기( 미리보기용 버퍼 )


#define MAX_OBJECT					4800	// 게임서버의 최대 오브젝트(유저) 수
#define ALLOC_USEROBJECTSTART		2000	// 게임서버의 최대 오브젝트(유저) 수

#define OBJTYPE_CHARACTER			1		// 오브젝트 타입이 유저캐릭터이다.
#define OBJTYPE_MONSTER				2		// 오브젝트 타입이 몬스터이다.
#define OBJTYPE_NPC					3		// 오브젝트 타입이  NPC 이다.
#define OBJTYPE_ITEM				5		// 아이템이다.

//----------------------------------------------------------------------------
// Killer Type ( 죽였을
#define KT_CHAR						0		// 플레이어한테 죽었다.
#define KT_MONSTER					1		// 몬스터한테 죽엇따.
#define KT_GUILDWARCHAR				2		// 길드전에서 플레이어가 죽였다.

//----------------------------------------------------------------------------
// 게임에 사용되는 정의들
#define MAX_LEVELUPPOINT			5		// 캐릭터당 레벨업 했을때 포인트
#define MAX_SHOP					12		// 최대 상점 개수

#define PK_LEVEL_DEFAULT			3		// PK Level 의 기본 값

#define I_TRADE						1		// 교환 진행중
#define I_PARTY						2		// 파티 진행중
#define I_SHOP						3
#define I_GUILD						4		// 길드 가입 인터페이스
#define I_GUILDCREATE				5		// 길드 생성 인터페이스
#define I_WAREHOUSE					6		// 창고
#define I_CHAOSBOX					7		// 카오스 박스

//----------------------------------------------------------------------------
// 맵서버에서 사용되는 정의들..
#define MAX_MAPITEM					200
#define MAX_MAP						63

#define MAX_USEROBJECT				45000
#endif
