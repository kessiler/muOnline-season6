//-> Revised by DarkSim | 23.04.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#include "MagicDamage.h"
// -------------------------------------------------------------------------------

#define MAX_MAGICINF	255
// -------------------------------------------------------------------------------

class CMagicInf
{
public:
	CMagicInf();
	virtual ~CMagicInf();
	// ----
	int		IsMagic();
	void	Clear();
	int		Set(BYTE aType, BYTE aIndex, BYTE aLevel);
	int		GetDamage();
	int		Set(int aSkill, BYTE aLevel);
	int		UpdateMasterSkill(int iSkill, BYTE btLevel);
	// ----
	CMagicInf & operator = (CMagicInf const & __that)
	{
		this->m_Level		= __that.m_Level;
		this->m_Skill		= __that.m_Skill;
		this->m_DamageMin	= __that.m_DamageMin;
		this->m_DamageMax	= __that.m_DamageMax;
		this->m_bPass		= __that.m_bPass;	//new
		return * this;
	};
	// ----
public:
	BYTE	m_Level;		//+4
	int		m_Skill;		//+8
	int		m_DamageMin;	//+12
	int		m_DamageMax;	//+16
	BYTE	m_bPass;		//+20
	// ----
}; extern CMagicInf DefMagicInf[];
// -------------------------------------------------------------------------------

int		GetSkillNumberInex(int type, int Index, int level);
void	MagicByteConvert(LPBYTE buf, CMagicInf Magici[], int maxmagic);
// -------------------------------------------------------------------------------