#ifndef _VISUAL_FIX_
#define _VISUAL_FIX_

#include "stdafx.h"
#include "ProtocolDefine.h"
#include "Object.h"

//#pragma pack(push, 1)
struct PMSG_JOINRESULT
{
	PBMSG_HEAD h;	// C1:F1
	BYTE scode;	// 3
	BYTE result;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE CliVersion[5];	// 7
};
//#pragma pack(pop)

//#pragma pack(push, 1)
struct PMSG_ATTACKRESULT
{
	PBMSG_HEAD h;	// C1:DC
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE DamageH;	// 5
	BYTE DamageL;	// 6
	BYTE DamageType;	// 7
	BYTE btShieldDamageH;	// 8
	BYTE btShieldDamageL;	// 9
	int Life;	//A
	int MaxLife;	//E
	int Class;
	int AttackHP;
	int AttackSD;
};
//#pragma pack(pop)

//#pragma pack(push, 1)
struct PMSG_REFILL
{
	PBMSG_HEAD h;	// C1:26
	BYTE IPos;	// 3
	BYTE LifeH;	// 4
	BYTE LifeL;	// 5
	BYTE Flag;	// 6
	BYTE btShieldH;	// 7
	BYTE btShieldL;	// 8
	int Life;
	int Shield;
};
//#pragma pack(pop)

//#pragma pack(push, 1)
struct PMSG_MANASEND
{
	PBMSG_HEAD h;	// C1:27
	BYTE IPos;	// 3
	BYTE ManaH;	// 4
	BYTE ManaL;	// 5
	BYTE BPH;	// 6
	BYTE BPL;	// 7
	int Mana;
	int BP;
};
//#pragma pack(pop)

//#pragma pack(push, 1)
struct PMSG_CHARMAPJOINRESULT
{
	PBMSG_HEAD h;	// C3:F3:03
	BYTE subcode;	// 3
	BYTE MapX;	// 4
	BYTE MapY;	// 5
	BYTE MapNumber;	// 6
	BYTE Dir;	// 7

	BYTE ExpHHH; //
	BYTE ExpHHL; //
	BYTE ExpHLH; //
	BYTE ExpHLL; //
	BYTE ExpLHH; //
	BYTE ExpLHL; //
	BYTE ExpLLH; //
	BYTE ExpLLL; //

	BYTE NextExpHHH; //
	BYTE NextExpHHL; //
	BYTE NextExpHLH; //
	BYTE NextExpHLL; //
	BYTE NextExpLHH; //
	BYTE NextExpLHL; //
	BYTE NextExpLLH; //
	BYTE NextExpLLL; //

	WORD LevelUpPoint;	// 10
	WORD Str;	// 12
	WORD Dex;	// 14
	WORD Vit;	// 16
	WORD Energy;	// 18
	WORD Life;	// 1A
	WORD MaxLife;	// 1C
	WORD Mana;	// 1E
	WORD MaxMana;	// 20
	WORD wShield;	// 22
	WORD wMaxShield;	// 24
	WORD BP;	// 26
	WORD MaxBP;	// 28
	int Money;	// 2C
	BYTE PkLevel;	// 30
	BYTE CtlCode;	// 31
	short AddPoint;	// 32
	short MaxAddPoint;	// 34
	WORD Leadership;	// 36
	WORD wMinusPoint;	// 38
	WORD wMaxMinusPoint;	// 3A
	BYTE ExpandedInventoryLevel;	//0x44
	int UpPoint;
};
//#pragma pack(pop)

//#pragma pack(push, 1)
struct PMSG_LEVELUP
{
	PBMSG_HEAD h;	// C1:F3:05
	BYTE subcode;	// 3
	WORD Level;	// 4
	WORD LevelUpPoint;	// 6
	WORD MaxLife;	// 8
	WORD MaxMana;	// A
	WORD wMaxShield;	// C
	WORD MaxBP;	// E
	short AddPoint;	// 10
	short MaxAddPoint;	// 12
	short MinusPoint;	// 14
	short MaxMinusPoint;	// 16
	int UpPoint;
};
//#pragma pack(pop)

//#pragma pack(push, 1)
struct PMSG_LVPOINTADDRESULT
{
	PBMSG_HEAD h;	// C1:F3:06
	BYTE subcode;	// 3
	BYTE ResultType;	// 4
	WORD MaxLifeAndMana;	// 6
	WORD wMaxShield;	// 8
	WORD MaxBP;	// A
};
//#pragma pack(pop)

//#pragma pack(push, 1)
struct PMSG_DIEPLAYER
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE SkillH;
	BYTE SkillL;
	BYTE KillerNumberH;
	BYTE KillerNumberL;
};
//#pragma pack(pop)

#define pDrawDamage  ((void(__cdecl*)(int a1, int a2, int a3, float a4, char a5, char a6)) 0x770B50)
#define pDrawBar	 ((double(__cdecl*)(float a1, float a2, int a3, float a4)) 0x790EB0)
#define pDrawTab	 ((int(__cdecl*)(signed int a1, signed int a2, LPCSTR lpMultiByteStr)) 0x597220)
#define pInfo		 ((int(*)(int a1, const char *a2, ...)) 0x50D100)
#define pDrawCircle  ((void(__cdecl*)(DWORD ModelID, float Arg2, float Arg3, float Arg4, float Arg5, float Arg6, float Arg7, float Arg8, float Arg9, bool Arg10, bool Arg11, float Arg12)) 0x637C60)

class cVisualFix
{
public:
	short aIndex;
	int AttackHP;
	int AttackSD;
	int Life;
	int SD;
	int Mana;
	int AG;
	int MaxLife;
	int MaxSD;
	int MaxMana;
	int MaxAG;
	int UpPoint;
	int AttackRate;
	int DamageRate;

	bool PlayerKill;

	char LifeTab[50];
	char ManaTab[50];
	char SDTab[50];
	char AGTab[50];

	void InitVisualFix();

	void RecvIndex(PMSG_JOINRESULT * Data);
	void RecvDamage(PMSG_ATTACKRESULT * Data);
	void RecvHPSD(PMSG_REFILL * Data);
	void RecvMPAG(PMSG_MANASEND * Data);
	void RecvUpPoint(PMSG_CHARMAPJOINRESULT * Data);
	void RecvUpLevel(PMSG_LEVELUP * Data);
	void RecvDownPoint(PMSG_LVPOINTADDRESULT * Data);
	void RecvRespawn();
	void RecvKilledObject(PMSG_DIEPLAYER * Data);

	static void DrawDamageHP(int a1, int a2, int a3, float a4, char a5, char a6);
	static void DrawDamageSD(int a1, int a2, int a3, float a4, char a5, char a6);

	static void DrawHp(float a1, float a2, int a3, float a4);
	static void DrawMp(float a1, float a2, int a3, float a4);
	static void DrawAg(float a1, float a2, int a3, float a4);
	static void DrawSd(float a1, float a2, int a3, float a4);

	static void DrawTabHP(signed int a1, signed int a2, LPCSTR lpMultiByteStr);
	static void DrawTabMP(signed int a1, signed int a2, LPCSTR lpMultiByteStr);
	static void DrawTabSD(signed int a1, signed int a2, LPCSTR lpMultiByteStr);
	static void DrawTabAG(signed int a1, signed int a2, LPCSTR lpMultiByteStr);

	static void InfoHp(int a1, const char *a2, ...);
	static void InfoMp(int a1, const char *a2, ...);
	static void InfoPoint(int a1, const char *a2, ...);

	static void DrawCircle(DWORD ModelID, float Arg2, float Arg3, float Arg4, float Arg5, float Arg6, float Arg7, float Arg8, float Arg9, int Arg10, int Arg11, int Arg12);

	static void _FC CalculateAttackRate(lpCharObj lpUser, LPVOID EDX);
	static void _FC CalculateDamageRate(lpCharObj lpUser, LPVOID EDX);

	static int GetAttackRate();
	static int GetDamageRate();
};
extern cVisualFix gVisualFix;

#endif