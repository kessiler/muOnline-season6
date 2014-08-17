// classdef.h: interface for the classdef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSDEF_H__88F4F8A9_7939_4AAD_BBBD_DC93F77BA329__INCLUDED_)
#define AFX_CLASSDEF_H__88F4F8A9_7939_4AAD_BBBD_DC93F77BA329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zzzitem.h"

struct DEFAULTCLASSTYPE
{
	int Experience;
	WORD Strength;
	WORD Dexterity;
	WORD Vitality;
	WORD Energy;
	float LevelLife;
	float Life;
	float MaxLife;
	float LevelMana;
	float Mana;
	float MaxMana;
	float VitalityToLife;
	float EnergyToMana;
	CItem Equipment[14];
	WORD Leadership;
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
	void SetMabubBanjiOption(int flag);

public:
	DEFAULTCLASSTYPE DefClass[MAX_CLASSTYPE];
	int m_BabubBanJiCreate;
};

#endif // !defined(AFX_CLASSDEF_H__88F4F8A9_7939_4AAD_BBBD_DC93F77BA329__INCLUDED_)
