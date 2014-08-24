#ifndef WEAPON_H
#define WEAPON_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




#define MAX_WEAPON_DAMAGED_TARGET_INFO 100
#define MAX_WEAPON_CAL_DAMAGER_INFO 50

#include "user.h"


struct ST_WEAPON_DAMAGED_TARGET_INFO
{
	int m_iWeaponObjIndex;	// 0
	int m_iTargetObjIndex ;	// 4
	int m_iDamageDealingTime;	// 8
	BOOL m_IsUsed;	// C

	void RESET()
	{
		this->m_iWeaponObjIndex = -1;
		this->m_iTargetObjIndex = -1;
		this->m_iDamageDealingTime = 0;
		this->m_IsUsed = FALSE;
	}
};

struct ST_WEAPON_CAL_DAMAGER_INFO
{
	int m_iWeaponObjIndex;	// 0
	int m_iTargetX;	// 4
	int m_iTargetY;	// 8
	int m_iLimitTime;	// C
	BOOL m_IsUsed;	// 10

	void RESET()
	{
		this->m_iWeaponObjIndex = -1;
		this->m_iTargetX = -1;
		this->m_iTargetY = -1;
		this->m_iLimitTime = -1;
		this->m_IsUsed = FALSE;
	}
};



class CWeapon
{

public:

	CWeapon();
	virtual ~CWeapon();

	BOOL SetWeaponCalDamageInfo(WORD wObjIndex, BYTE btTargetX, BYTE btTargetY, int iDelayTime);
	BOOL GetWeaponCalDamageInfo(WORD wObjIndex, BYTE& btTargetX, BYTE& btTargetY);
	BOOL AddWeaponDamagedTargetInfo(int iWeaponIndex, int iTargetIndex, int iDelayTime);
	void WeaponAttackProc();
	void WeaponAct(int iIndex);
	BOOL Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic,  int iCriticalDamage, int iActionType);
	int GetAttackDamage(int iObjClass);
	BOOL GetTargetPointXY(int iObjClass, int iTargetPointIndex, BYTE& btX, BYTE& btY, BOOL bRandomPos);
	BOOL MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkill, int iSkillSuccess, BOOL& bAllMiss);


private:

	ST_WEAPON_DAMAGED_TARGET_INFO m_WeaponDamagedTargetInfo[MAX_WEAPON_DAMAGED_TARGET_INFO];	// 4
	ST_WEAPON_CAL_DAMAGER_INFO m_WeaponCalDamageInfo[MAX_WEAPON_CAL_DAMAGER_INFO];	// 644

};

extern CWeapon g_CsNPC_Weapon;

#endif