#ifndef MUMSG_H
#define MUMSG_H

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

class CMsg
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

extern int nMsg;
int fnMuMsg(void);

#endif
