// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef CLASSDEF_H
#define CLASSDEF_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\common\zzzitem.h"


#define MAX_PLAYER_EQUIPMENT 12


enum ClassNumber 
{
  CLASS_WIZARD		= 0,
  CLASS_KNIGHT		= 1,
  CLASS_ELF			= 2,
  CLASS_MAGUMSA		= 3,
  CLASS_DARKLORD	= 4,
  CLASS_SUMMONER	= 5,
  CLASS_MONK		= 6,
};


struct DEFAULTCLASSTYPE
{
	int Experience;			// 0
	WORD Strength;			// 68
	WORD Dexterity;			// 6A
	WORD Vitality;			// 6c
	WORD Energy;			// 6E
	float LevelLife;		// 70
	float Life;				// 74
	float MaxLife;			// 78
	float LevelMana;		// 7C
	float Mana;				// 80
	float MaxMana;			// 84
	float VitalityToLife;	// 88
	float EnergyToMana;		// 8C
	CItem Equipment[MAX_PLAYER_EQUIPMENT+2];	// 2C
	WORD Leadership;	// 9CC
};



class classdef
{
public:

	classdef();
	virtual ~classdef();

	void Init();
	void SetCharacter(int Class, int Str, int Dex, int Vit, int Energy, float Life, float Mana, float LevelLife, float LevelMana, float VitalityToLife, float EnergyToMana, int Leadership);
	void SetEquipment(int Class);
	int GetDefPoint(int char_class);
	void SetMabubBanjiOption(BOOL Flag);

public:

	DEFAULTCLASSTYPE DefClass[MAX_TYPE_PLAYER];	// 4
	BOOL m_BabubBanJiCreate; // 3114
	
};

#endif