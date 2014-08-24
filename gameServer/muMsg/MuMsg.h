
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MUMSG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MUMSG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MUMSG_EXPORTS
#define MUMSG_API __declspec(dllexport)
#else
#define MUMSG_API __declspec(dllimport)
#endif


typedef struct MSG_STRUCT
{
	MSG_STRUCT* next;	// 0
	int number;	// 4
	LPBYTE msg;	// 8

} MSG_STRUCT, * LPMSG_STRUCT;



#define MSGGET(x, y) ( (x) * 256 + (y)  )


struct TCHeader
{
	BYTE headcode;	 // 0
	BYTE version;	// 1
	char caption[21];	// 2
	int count;	// 18
};



extern "C" MUMSG_API int fnMsg(void);

class MUMSG_API CMsg
{

public:

	CMsg();
	~CMsg();

	void  LoadWTF(LPSTR filename);
	LPSTR Get(int);

private:

	int  lMsgListInit();
	int  lMsgListAdd(int, LPBYTE);
	MSG_STRUCT*  lMsgListNew();
	void  XorBuffer(LPSTR, int);
	void  DataFileLoadVer01(struct _iobuf*);
	void  lMsgFree();
	void  lMsgListPrint();


protected:

	MSG_STRUCT* Msghead;	// 0
	MSG_STRUCT* Msgcur;	// 4
	MSG_STRUCT* MsgIndex[32768];	// 8
	TCHeader LoadHeader;	// 20008
	char szDefaultMsg[50];	// 20024

};


extern MUMSG_API int nMsg;

MUMSG_API int fnMuMsg(void);

