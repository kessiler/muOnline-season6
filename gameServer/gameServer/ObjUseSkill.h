#ifndef OBJUSESKILL_H
#define OBJUSESKILL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MagicInf.h"
#include "user.h"

struct PMSG_DURATION_MAGIC_SEND
{
	struct PBMSG_HEAD h;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	BYTE NumberH;
	BYTE NumberL;
	BYTE X;
	BYTE Y;
	BYTE Dir;
};

class CObjUseSkill
{

public:

	CObjUseSkill();
	virtual ~CObjUseSkill();

	int SpeedHackCheck(int aIndex);
	int EnableSkill(DWORD Skill, int Level);
	int GetUseMana(int aIndex, CMagicInf* lpMagic);
	int GetAddUseMana(LPOBJ lpObj, CMagicInf* lpMagic);
	int GetUseBP(int aIndex, CMagicInf* lpMagic);
	void UseSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	void UseSkill(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int RunningSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL bCombo);
	void MaGumSkillDefenseDown(int aIndex, int aTargetIndex, int skill_level);
	void KnightSkillAddLife(int aIndex, int skill_level);
	void WizardMagicDefense(int aIndex, int aTargetIndex, int skill_level);
	void SkillDefense(int aIndex, int aTargetIndex, int skill_level);
	void SkillAttack(int aIndex, int aTargetIndex, int skill_level);
	int SkillChangeUse(int aIndex);
	void SkillHealing(int aIndex, int aTargetIndex, int skill_level);
	int SkillMonsterCall(int aIndex, int MonsterType, int x, int y);
	int SkillDeathPoison(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillSuddenIce(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillHellFire2Start(int aIndex, CMagicInf* lpMagic);
	int SkillHellFire2(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	void SkillBlowOfFury(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo);
	void SkillWheel(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL isCombo);
	int SkillKnightBlow(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL isCombo);
	void SkillPowerSlash(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle, int aTargetIndex);
	void SkillFrustrum(int aIndex, float fangle, float tx, float ty);
	int GetTargetLinePath(int sx, int sy, int tx, int ty,  int* PathX,int* PathY, int distance);
	int GetAngle(int x, int y, int tx,  int ty);
	int CalDistance(int x, int y, int x1, int y1);
	void SkillFrustrum2(int aIndex, float fangle, float tx, float ty);
	int SkillAreaCheck(int angel, int x, int y,  int tx, int ty,  int dis,  int arc);
	int SkillSpear(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillFireBurst(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillDarkHorseAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int RecallPartyCheck(int aIndex, int skill_level);
	void SkillRecallParty(int aIndex, int skill_level);
	void SkillAddCriticalDamage(int aIndex, int skill_level);
	void SkillElectricSpark(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillSummon(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillImmuneToMagic(int aIndex, CMagicInf* lpMagic);
	int SkillImmuneToHarm(int aIndex, CMagicInf* lpMagic);
	int SkillKnightRush(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillWizardJavalin(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillElfRush(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillMagumOneFlash(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillMagumDeathCannon(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillDarklordSpaceSplit(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillBrandOfSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveStun(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillAddMana(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillCloaking(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int RemoveCloakingEffect(int aIndex);
	int SkillRemoveCloaking(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveMagic(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillFenrirAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillInfinityArrow(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillStun(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BYTE btX, BYTE btY, BYTE btDir, BYTE btTargetPos);
	int RemoveSpecificSkillState(LPOBJ lpObj, int iSkillNumber);
	int RemoveAllCharacterInvalidMagicAndSkillState(LPOBJ lpObj);
	int SkillFireScream(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int FireScreamExplosionAttack(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage);
	void SpecificSkillAdditionTreat(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic, int iAttackDamage);

	//Season3 New
	int SkillEletrictSurge(int aIndex, int aTargetIndex, CMagicInf* lpMagic); //Good
	int SkillEletrictSurgeProc(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkillID, int iAttackDamage, int flag, int Message); //Good
	int SkillDrainLife(int aIndex, int aTargetIndex, CMagicInf* lpMagic); //Good
	int SkillChainLightning(int aIndex, int aTargetIndex, CMagicInf* lpMagic); //Good
	int SkillSummonerBook(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex); //Good
	int SkillDamageReflection(int aIndex,int aTargetIndex,CMagicInf* lpMagic); //Good
	int SkillSleep(int aIndex, int aTargetIndex, CMagicInf* lpMagic); //Good
	int SkillBlind(int aIndex, int aTargetIndex, CMagicInf* lpMagic); //Good
	int SkillWeakness(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex); //Good
	int SkillInnovation(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex); //Good

	//Season4 New
	BOOL SkillLightningShock(int aIndex, int aTargetIndex, CMagicInf* lpMagic); //Good
	BOOL SkillBerserker(int aIndex, CMagicInf* lpMagic); //Good
	int SkillPollutionStart(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex, int Flag); //Good
	int SkillAreaMonsterAttack(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex, int Dis, int Flag, int Delay); //Good
	int SkillStrikeOfDestructionStart(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex); //Good
	void SkillFrustrum3(int aIndex, BYTE fangle, float tx, float ty, float arg5, float arg6); //Good
	void SkillFlameStrike(int aIndex, CMagicInf* lpMagic, BYTE TargetPos, int aTargetIndex); //Good
	int SkillGiganticStormStart(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex); //Good
	void SkillMultiShot(int aIndex, CMagicInf* lpMagic, BYTE TargetPos, int aTargetIndex); //Good
	void SkillSDRecovery(int aIndex, int aTargetIndex, CMagicInf* lpMagic); //Good
	BOOL SkillExpansionWizardry(int aIndex, int aTargetIndex, CMagicInf* lpMagic); //Good
	void SkillChaoticDiseier(int aIndex, CMagicInf* lpMagic, BYTE TargetPos, int aTargetIndex); //Good

	//1.01.00
	int		SkillEvilMonster(int aIndex, int aTargetIndex, CMagicInf * lpMagic);							//005308f0
	int		SkillDeathPoisonForMeDusa(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex);	//00530b60
	int		SkillMonkBuff(int aIndex, CMagicInf * lpMagic);													//00530eb0
	int		SkillMonkBuffApplyParty(int aIndex, CMagicInf * lpMagic);										//00531010
	void	SkillMonkBarrageJustOneTarget(int aIndex, CMagicInf * lpMagic, int aTargetIndex);				//005314c0
	int		SkillMonkDarkSideGetTargetIndex(int aIndex, int aTargetIndex, CMagicInf * lpMagic, WORD * wTargetList);	//00531a30
	int		SkillCharge(int aIndex, CMagicInf * lpMagic, int aTargetIndex);									//00531f10
	void    SkillDarkSide(int aIndex, int iTargetIndex, CMagicInf *lpMagic);

};


int  DecPartyMemberHPandMP(LPOBJ lpObj);

extern CObjUseSkill gObjUseSkill;

#endif