// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef SKILLHITBOX_H
#define SKILLHITBOX_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define SKILL_HIT_MAX_INDEX 36
#define SKILL_HIT_MAX_SUBINDEX	441

class CSkillHitBox
{

public:

	CSkillHitBox();
	virtual ~CSkillHitBox();

	void Init();
	int Check(int dir, int x, int y);
	int HitCheck(int dir, int x, int y, int tx, int ty);
	BOOL Load(char* filename);
	

private:

	BYTE m_Table[SKILL_HIT_MAX_INDEX][SKILL_HIT_MAX_SUBINDEX];	// 4

};

extern CSkillHitBox SkillSpearHitBox;
extern CSkillHitBox SkillElectricSparkHitBox;

#endif