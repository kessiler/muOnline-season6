// TMonsterSkillElementInfo.h: interface for the TMonsterSkillElementInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMONSTERSKILLELEMENTINFO_H__8EC5FD6E_2B9F_409A_B2AA_70024491BEE5__INCLUDED_)
#define AFX_TMONSTERSKILLELEMENTINFO_H__8EC5FD6E_2B9F_409A_B2AA_70024491BEE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class TMonsterSkillElementInfo
{

public:
	void SetSkillElement(int iSkillElement);//NEW

	TMonsterSkillElementInfo();

	void  Reset();
	void  ResetDefense();
	void  ResetAttack();
	void  ResetAutoHP();
	void  ResetAutoMP();
	void  ResetAutoAG();
	void  ResetImmune();
	void  ResetResist();
	void  ResetModifyStat();
	void  ResetBerserkTime();//NEW
	int  CheckDefenseTime();
	int  CheckAttackTime();
	int  CheckAutoHPTime();
	int  CheckAutoMPTime();
	int  CheckAutoAGTime();
	int  CheckResistTime();
	int  CheckModifyStatTime();
	int	 CheckBerserkTime();//NEW
	int  DecDefenseTime();
	int  DecAttackTime();
	int  DecAutoHPTime();
	int  DecAutoMPTime();
	int  DecAutoAGTime();
	int  DecResistTime();
	int  DecModifyStatTime();
	int	 DecBerserkTime();//NEW
	static void CheckSkillElementInfoProc(struct OBJECTSTRUCT * lpObj);

public:

	int m_iSkillElementDefense;	// 0
	int m_iSkillElementDefenseTime;	// 4
	int m_iSkillElementAttack;	// 8
	int m_iSkillElementAttackTime;	// C
	int m_iSkillElementAutoHP;	// 10
	int m_iSkillElementAutoHPCycle;	// 14
	int m_iSkillElementAutoHPTime;	// 18
	int m_iSkillElementAutoMP;	// 1C
	int m_iSkillElementAutoMPCycle;	// 20
	int m_iSkillElementAutoMPTime;	// 24
	int m_iSkillElementAutoAG;	// 28
	int m_iSkillElementAutoAGCycle;	// 2C
	int m_iSkillElementAutoAGTime;	// 30
	int m_iSkillElementImmuneNumber;	// 34
	int m_iSkillElementImmuneTime;	// 38
	int m_iSkillElementResistNumber;	// 3C
	int m_iSkillElementResistTime;	// 40
	int m_iSkillElementModifyStat;	// 44
	int m_iSkillElementModifyStatType;	// 48
	int m_iSkillElementModifyStatTime;	// 4C
	int m_iSkillElementBerserkTime; //50
};



#endif // !defined(AFX_TMONSTERSKILLELEMENTINFO_H__8EC5FD6E_2B9F_409A_B2AA_70024491BEE5__INCLUDED_)
