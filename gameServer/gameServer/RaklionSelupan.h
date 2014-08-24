// RaklionSelupanMng.h: interface for the CRaklionSelupanMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONSELUPANMNG_H__10C0D6C9_4187_4482_BDF5_68C6CEB2A0D8__INCLUDED_)
#define AFX_RAKLIONSELUPANMNG_H__10C0D6C9_4187_4482_BDF5_68C6CEB2A0D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RaklionObjInfo.h"


class CRaklionSelupan 
{
public:
	CRaklionSelupan();
	virtual ~CRaklionSelupan();
	void ClearData();
	void SelupanAct_FirstSkill();
	void SelupanAct_PoisonAttack();
	void SelupanAct_IceStorm();
	void SelupanAct_IceStrike();
	void SelupanAct_SummonMonster();
	void SelupanAct_Heal();
	void SelupanAct_Freeze();
	void SelupanAct_Teleport();
	void SelupanAct_Invincibility();
	void SelupanAct_BerserkCansel();
	void SelupanAct_Berserk1();
	void SelupanAct_Berserk2();
	void SelupanAct_Berserk3();
	void SelupanAct_Berserk4();
	DWORD GetSelupanLife();
	void SetSelupanLife(int arg1);
	void InitSelupanObjIndex();
	BOOL CreateSelupan();
	void DeleteSelupan();
	int GetSelupanTargetIndex();
	void SetSelupanSkillDelay(int SkillDelay);
	int GetSelupanSkillDelay();
	int GetBerserkIndex();
	void SetBossAttackDamage(int DamageMin,int DamageMax);
	void SetSelupanObjIndex(int arg1);
	int GetSelupanObjIndex();
	void CreateSummonMonster();
	void DeleteSummonMonster();
public:
	int				m_iSelupanObjIndex;;			//this + 4
	CRaklionObjInfo m_RaklionMonster;				//this + 8
	int				m_iSelupanSkillDelay;			//this + C90
	int				m_iBerserkIndex;				//this + C94
	int				m_iBossAttackMin;				//this + C98
	int				m_iBossAttackMax;				//this + C9C
	

};

#endif // !defined(AFX_RAKLIONSELUPANMNG_H__10C0D6C9_4187_4482_BDF5_68C6CEB2A0D8__INCLUDED_)
