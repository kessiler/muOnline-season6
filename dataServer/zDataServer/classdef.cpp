// classdef.cpp: implementation of the classdef class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "classdef.h"
#include "public.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

classdef::classdef()
{
	m_BabubBanJiCreate = 0;
}

classdef::~classdef()
{

}

void classdef::Init()
{
	SetCharacter(CLASS_WIZARD		, 18, 18, 15, 30, 60.0f, 60.0f, 1.0f, 2.0f, 2.0f, 2.0f, 0);
	SetCharacter(CLASS_KNIGHT		, 28, 20, 25, 10, 110.0f, 20.0f, 2.0f, 0.5f, 3.0f, 1.0f, 0);
	SetCharacter(CLASS_ELF		, 22, 25, 20, 15, 80.0f, 30.0f, 1.0f, 1.5f, 2.0f, 1.5f, 0);
	SetCharacter(CLASS_MAGUMSA	, 26, 26, 26, 26, 110.0f, 60.0f, 1.0f, 1.0f, 2.0f, 2.0f, 0);
	SetCharacter(CLASS_DARKLORD	, 26, 20, 20, 15, 90.0f, 40.0f, 1.5f, 1.0f, 2.0f, 1.5f, 25);
	SetCharacter(CLASS_SUMMONER	, 21, 21, 18, 23, 70.0f, 40.0f, 1.0f, 1.5f, 2.0f, 1.5f, 0);
	SetCharacter(CLASS_MONK, 32, 27, 25, 20, 100.0f, 40.0f, 1.3f, 1.0f, 2.0f, 1.3f, 0);
}

void classdef::SetCharacter(int Class, int Str, int Dex, int Vit, int Energy, float Life, float Mana, float LevelLife, float LevelMana, float VitalityToLife, float EnergyToMana, int Leadership)
{
	DefClass[Class].Experience = 0;
	DefClass[Class].Strength = Str;
	DefClass[Class].Dexterity = Dex;
	DefClass[Class].Vitality = Vit;
	DefClass[Class].Energy = Energy;
	DefClass[Class].Life = Life;
	DefClass[Class].MaxLife = Life;
	DefClass[Class].LevelLife = LevelLife;
	DefClass[Class].Mana = Mana;
	DefClass[Class].MaxMana = Mana;
	DefClass[Class].LevelMana = LevelMana;
	DefClass[Class].VitalityToLife = VitalityToLife;
	DefClass[Class].EnergyToMana = EnergyToMana;
	DefClass[Class].Leadership = Leadership;

	SetEquipment(Class);
}

void classdef::SetEquipment(int Class)
{
	int def_type=0;
	int right_type=0;
	int n;

	for (n=0;n<MAX_EQUIPMENT+1;n++)
	{
		DefClass[Class].Equipment[n].Clear();
	}

	switch (Class)
	{
		case CLASS_KNIGHT:
			def_type = ItemGetNumberMake(1, 0);
			break;

		case CLASS_ELF:
			def_type = ItemGetNumberMake(4, 0);
			right_type = ItemGetNumberMake(4, 15);
			break;

		case CLASS_DARKLORD:
			def_type = ItemGetNumberMake(6, 0);
			right_type = ItemGetNumberMake(0, 1);
			break;

		case CLASS_MAGUMSA:
			def_type = ItemGetNumberMake(6, 0);
			right_type = ItemGetNumberMake(0, 1);
			break;

		case CLASS_SUMMONER:
			def_type = ItemGetNumberMake(1, 0);
			break;

		case CLASS_MONK:
			def_type = ItemGetNumberMake(1, 0);
			break;
	}

	if ( Class != CLASS_WIZARD )
	{
		if ( Class == CLASS_DARKLORD )
		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_LEFT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(right_type);
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_LEFT].Convert(def_type);
		}
		else if ( Class == CLASS_ELF || Class == CLASS_MAGUMSA )
		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_LEFT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(right_type);
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_LEFT].Convert(def_type);
		}
		else if( Class == CLASS_SUMMONER )
		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(def_type);
		}
		else
		{
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].m_Level = 0;
			DefClass[Class].Equipment[EQUIPMENT_WEAPON_RIGHT].Convert(def_type);
		}
	}

	if ( m_BabubBanJiCreate == TRUE )
	{
		def_type = ItemGetNumberMake(13, 20);
		DefClass[Class].Equipment[EQUIPMENT_GOLD].m_Level = 1;
		DefClass[Class].Equipment[EQUIPMENT_GOLD].Convert(def_type);
		
		def_type = ItemGetNumberMake(13, 20);
		DefClass[Class].Equipment[EQUIPMENT_GOLD+1].m_Level = 2;
		DefClass[Class].Equipment[EQUIPMENT_GOLD+1].Convert(def_type);
	}
}

int classdef::GetDefPoint(int char_class)
{
	if ( char_class < 0 || char_class > MAX_CLASSTYPE-1 )
	{
		return 0;
	}

	return DefClass[char_class].Strength + DefClass[char_class].Dexterity + DefClass[char_class].Vitality + DefClass[char_class].Energy + DefClass[char_class].Leadership;
}

void classdef::SetMabubBanjiOption(BOOL Flag)
{
	m_BabubBanJiCreate = Flag;
}