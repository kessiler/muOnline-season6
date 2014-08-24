#ifndef MULTIATTACKHACKCHECK_H
#define MULTIATTACKHACKCHECK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#define MAX_MULTI_ATTACK_INFO 40
#define MULTI_ATTACK_SERIAL_SIZE 255

struct MULTIATTACK_HACKCHECK
{
	short number;	// 0
	BYTE Serial[MULTI_ATTACK_SERIAL_SIZE];	// 2
};


class CMultiAttackHackCheck
{

public:

	CMultiAttackHackCheck();
	virtual ~CMultiAttackHackCheck();

	void Init();
	int Insert(int aTargetIndex, BYTE skillnum, BYTE serial);
	int CheckPenetrationSkill(int aTargetIndex, BYTE skillnum, BYTE serial);
	int CheckFireScreamSkill(int iTargetIndex, BYTE btSkillnum, BYTE btSerial);


private:

	int m_iCount;	// 4
	MULTIATTACK_HACKCHECK m_Table[MAX_MULTI_ATTACK_INFO];	// 8

};

extern CMultiAttackHackCheck gMultiAttackHackCheck[OBJMAX];

#endif