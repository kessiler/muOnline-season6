// TMonsterSkillElement.h: interface for the TMonsterSkillElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERSKILLELEMENT_H__34D7C501_EFB0_43E4_A73D_356315331BFE__INCLUDED_)
#define AFX_TMONSTERSKILLELEMENT_H__34D7C501_EFB0_43E4_A73D_356315331BFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TSkillElement.h"

#define MAX_MONSTER_SKILL_ELEMENT	(100)


enum MONSTER_SKILL_ELEMENT_TYPE
{
	MSE_TYPE_STUN = 0x0,
	MSE_TYPE_MOVE = 0x1,
	MSE_TYPE_HP = 0x2,
	MSE_TYPE_MP = 0x3,
	MSE_TYPE_AG = 0x4,
	MSE_TYPE_DEFENSE = 0x5,
	MSE_TYPE_ATTACK = 0x6,
	MSE_TYPE_DURABILITY = 0x7,
	MSE_TYPE_SUMMON = 0x8,
	MSE_TYPE_PUSH = 0x9,
	MSE_TYPE_STAT_ENERGY = 0xa,
	MSE_TYPE_STAT_STRENGTH = 0xb,
	MSE_TYPE_STAT_DEXITERITY = 0xc,
	MSE_TYPE_STAT_VITALITY = 0xd,
	MSE_TYPE_REMOVE_SKILL = 0xe,
	MSE_TYPE_RESIST_SKILL = 0xf,
	MSE_TYPE_IMMUNE_SKILL = 0x10,
	MSE_TYPE_TELEPORT_SKILL = 0x11,
	MSE_TYPE_DOUBLE_HP = 0x12,
	MSE_TYPE_POISON = 0x13,
	MSE_TYPE_NORMALATTACK = 0x14,
	MSE_TYPE_BERSERK = 0x15,
};


enum MONSTER_SKILL_ELEMENT_INCDEC_TYPE
{
	MSE_INCDEC_TYPE_NONE = 0x0,
	MSE_INCDEC_TYPE_PERCENTINC = 0x1,
	MSE_INCDEC_TYPE_PERCENTDEC = 0x2,
	MSE_INCDEC_TYPE_CONSTANTINC = 0xb,
	MSE_INCDEC_TYPE_CONSTANTDEC = 0xc,
	MSE_INCDEC_TYPE_CYCLE_PERCENT = 0x64,
	MSE_INCDEC_TYPE_CYCLE_PERCENTINC = 0x65,
	MSE_INCDEC_TYPE_CYCLE_PERCENTDEC = 0x66,
	MSE_INCDEC_TYPE_CYCLE_CONSTANT = 0x6e,
	MSE_INCDEC_TYPE_CYCLE_CONSTANTINC = 0x6f,
	MSE_INCDEC_TYPE_CYCLE_CONSTANTDEC = 0x70,
};



class TMonsterSkillElement  : public  TSkillElement
{

public:

	virtual void ForceSkillElement(int iIndex, int iTargetIndex);	// 0
	TMonsterSkillElement();
	
	virtual ~TMonsterSkillElement();	// 4

	int ApplyElementStun(int iIndex, int iTargetIndex);
	int ApplyElementMove(int iIndex, int iTargetIndex);
	int ApplyElementHP(int iIndex, int iTargetIndex);
	int ApplyElementMP(int iIndex, int iTargetIndex);
	int ApplyElementAG(int iIndex, int iTargetIndex);
	int ApplyElementDefense(int iIndex, int iTargetIndex);
	int ApplyElementAttack(int iIndex, int iTargetIndex);
	int ApplyElementDurability(int iIndex, int iTargetIndex);
	int ApplyElementSummon(int iIndex, int iTargetIndex);
	int ApplyElementPush(int iIndex, int iTargetIndex);
	int ApplyElementStatEnergy(int iIndex, int iTargetIndex);
	int ApplyElementStatStrength(int iIndex, int iTargetIndex);
	int ApplyElementStatDexiterity(int iIndex, int iTargetIndex);
	int ApplyElementStatVitality(int iIndex, int iTargetIndex);
	int ApplyElementRemoveSkill(int iIndex, int iTargetIndex);
	int ApplyElementResistSkill(int iIndex, int iTargetIndex);
	int ApplyElementImmuneSkill(int iIndex, int iTargetIndex);
	int ApplyElementTeleportSkill(int iIndex, int iTargetIndex);
	int ApplyElementDoubleHP(int iIndex, int iTargetIndex);
	int ApplyElementPoison(int iIndex, int iTargetIndex);
	int ApplyElementNormalAttack(int iIndex, int iTargetIndex);
	int ApplyElementBerserk(int iIndex,int iTargetIndex);
	void Reset();
	char* GetElementName();

	static int LoadData(char* lpszFileName);
	static int DelAllSkillElement();
	static TMonsterSkillElement* FindSkillElement(int iElementNumber);

private:

	char m_szElementName[20];	// 4
	int m_iElementNumber;	// 18
	int m_iElementType;	// 1C
	int m_iSuccessRate;	// 20
	int m_iContinuanceTime;	// 24
	int m_iIncAndDecType;	// 28
	int m_iIncAndDecValue;	// 2C
	int m_iNullifiedSkill;	// 30
	int m_iCharacterClass;	// 34
	int m_iCharacterLevel;	// 38

public:

	static BOOL s_bDataLoad;
	static TMonsterSkillElement s_MonsterSkillElementArray[MAX_MONSTER_SKILL_ELEMENT];

};

#endif // !defined(AFX_TMONSTERSKILLELEMENT_H__34D7C501_EFB0_43E4_A73D_356315331BFE__INCLUDED_)
