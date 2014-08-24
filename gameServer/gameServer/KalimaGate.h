// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef KALIMAGATE_H
#define KALIMAGATE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameMain.h"
#include "user.h"

#define KALIMA_FLOORS 7
#define MAX_KALIMA_ENTER 5
#define KALIMA_GATE_TIMEOUT 180000
#define KUNDUN_MARK_COUNT	5

#define KALIMA_MAP_RANGE(x) ( ((x)<MAP_INDEX_KALIMA1)?FALSE:((x)>MAP_INDEX_KALIMA6)?(((x)==MAP_INDEX_KALIMA7)?TRUE:FALSE):TRUE   )
#define KALIMA_FLOOR_RANGE(x) (  (x<0)?FALSE:(x>KALIMA_FLOORS-1)?FALSE:TRUE )
#define KUNDUN_MARK_RANGE(x) ( ((x) <0)?FALSE:((x) > KUNDUN_MARK_COUNT-1)?FALSE:TRUE)
#define KALIMA_ATTRIBUTE_RANGE(x) ( ((x)<51)?FALSE:((x)>58)?FALSE:TRUE  )

static struct _KALIMAGATE_LEVEL
{
	int iLOWER_BOUND;	// 0
	int iUPPER_BOUND;	// 4
	int iLOWER_BOUND_MAGUMSA;	// 8
	int iUPPER_BOUND_MAGUMSA;	// C

} g_sttKALIMAGATE_LEVEL[KALIMA_FLOORS] =
{
	// Kalima 1
	0x28, 0x82, // DK, DW, Elf
	0x14, 0x6E, // MG, DL

	// Kalima 2
	0x83, 0xB4, // DK, DW, Elf
	0x6F, 0xA0, // MG, DL

	// Kalima 3
	0xB5, 0xE6, // DK, DW, Elf
	0xA1, 0xD2, // MG, DL

	// Kalima 4
	0xE7, 0x118, // DK, DW, Elf
	0xD3, 0x104, // MG, DL

	// Kalima 5
	0x119, 0x14A, // DK, DW, Elf
	0x105, 0x136, // MG, DL

	// Kalima 6
	0x14B, 0x15D, // DK, DW, Elf
	0x137, 0x15D, // MG, DL

	// Kalima 7
	0x15E, MAX_CHAR_LEVEL, // DK, DW, Elf
	0x15E, MAX_CHAR_LEVEL // MG, DL
};


static int g_iKalimaGateGateIndex[KALIMA_FLOORS] =
{
	0x58, 0x59,
	0x5A, 0x5B,
	0x5C, 0x5D,
	0x74
};




class CKalimaGate
{

private:

	CRITICAL_SECTION m_critKalimaGate;	// 4

public:

	CKalimaGate();
	virtual ~CKalimaGate();

	int CreateKalimaGate(int iIndex, BYTE btLevel, BYTE cTX,  BYTE cTY);
	void KalimaGateAct(int iIndex);
	int CheckOverlapKundunMark(int iIndex, BYTE btLevel);
	int DeleteKalimaGate(int iCallOwnerIndex);
	int DeleteKalimaGate(int iKalimaGateIndex, int iCallOwnerIndex);
	int CreateKalimaGate2(int iIndex, int iMonMapNumber, BYTE cTX, BYTE cTY);
	void KalimaGateAct2(int iIndex);
	int GetKalimaGateLevel2(int iIndex);

private:

	int GetRandomLocation(int iMapNumber, BYTE& cX, BYTE& cY);
	int GetKalimaGateLevel(int iIndex);
};


extern CKalimaGate g_KalimaGate;

#endif