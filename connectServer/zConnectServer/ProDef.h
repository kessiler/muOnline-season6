#ifndef __PROTOCOLDEF__
#define __PROTOCOLDEF__

#define PMHC_BYTE	0xC1
#define PMHC_WORD	0xC2
#define PMHCE_BYTE	0xC3
#define PMHCE_WORD	0xC4

typedef struct
{
    BYTE c;
    BYTE size;
    BYTE headcode;
} PBMSG_HEAD, *LPPBMSG_HEAD;

typedef struct
{
    BYTE c;
    BYTE sizeH;
    BYTE sizeL;
    BYTE headcode;
} PWMSG_HEAD, *LPPWMSG_HEAD;


typedef struct
{
    BYTE c;
    BYTE size;
    BYTE headcode;
    BYTE subcode;
} PBMSG_HEAD2, * LPPBMSG_HEAD2;

typedef struct
{
    BYTE c;
    BYTE sizeH;
    BYTE sizeL;
    BYTE headcode;
    BYTE subcode;
} PWMSG_HEAD2, * LPPWMSG_HEAD2;


typedef struct
{
    PBMSG_HEAD	h;
    BYTE		count;
} PBMSG_COUNT, * LPPBMSG_COUNT;


typedef struct
{
    PWMSG_HEAD	h;
    BYTE		count;
} PWMSG_COUNT, * LPPWMSG_COUNT;


typedef struct
{
    PBMSG_HEAD	h;
    BYTE		subcode;
    BYTE		count;
} PBMSGS_COUNT, * LPPBMSGS_COUNT;


#endif
