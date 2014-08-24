#pragma once 

#include "MuLua.h"
#include "user.h"

#define MAX_SKILLTREE_TYPE	3
#define MAX_SKILLTREE_RANK	9
#define MAX_SKILLTREE_POS	4
#define MAX_SKILLTREE_CLASS	7
#define MAX_SKILLTREE_LEVEL	20

struct _TreeData_SkillInfo
{
	int m_iIndex;
	BYTE m_btPoint;
	BYTE m_btMaxPoint;

	int m_iParentSkill1;
	int m_iParentSkill2;
	int m_iSkillID;
};

struct _TreeData
{
	int m_iUnk0;
	_TreeData_SkillInfo m_SkillInfo[MAX_SKILLTREE_TYPE][MAX_SKILLTREE_RANK][MAX_SKILLTREE_POS];
};

typedef struct 
{
	PBMSG_HEAD		head;				// 0
	BYTE			subcode;			// 3
	int				AddSkill;	// 4

} PMSG_REQ_MASTERLEVEL_SKILL, *LPPMSG_REQ_MASTERLEVEL_SKILL;
// --------------------------------------------------------------------------------------------


class CMasterLevelSkillTreeSystem
{
public:
	CMasterLevelSkillTreeSystem();
	virtual ~CMasterLevelSkillTreeSystem();

	void Init();
	void Load(char const * lpszFileName);
	void InitPassiveSkillData(LPOBJ lpObj);
	void CalcPassiveSkillData(LPOBJ lpObj,int iMLSkill,int iSkillLevel);
	int CheckSkillCondition(LPOBJ lpObj,int iMLSkill,int iSkillLevel);
	int CheckCompleteBrandOfSkill(LPOBJ lpObj,int iMLSkill);
	int CheckMasterLevelSkill(int iMLSkill);
	int IsExistMLSBasedSkill(LPOBJ lpObj,int iSkill);
	int GetBaseMasterLevelSkill(int iMLSkill);
	int gObjMagicAdd_MLS(LPOBJ lpObj,int iMLSkill,int iSkillLevel);
	void CGReqGetMasterLevelSkillTree(int aIndex);
	void CGReqGetMasterLevelSkill(PMSG_REQ_MASTERLEVEL_SKILL * lpMsg,int aIndex);
	int RunningSkill_MLS(LPOBJ lpObj,int aTargetIndex,CMagicInf* lpMagic,int bCombo,BYTE x,BYTE y, BYTE dir, BYTE aTargetPos);
	void MLS_WizardMagicDefense(LPOBJ lpObj,int aTargetIndex,CMagicInf * lpMagic);
	void MLS_Healing(LPOBJ lpObj,int aTargetIndex,CMagicInf * lpMagic);
	void MLS_Wheel(LPOBJ lpObj,CMagicInf *lpMagic,int aTargetIndex,int isCombo);
	void MLS_SkillSword4(LPOBJ lpObj,CMagicInf * lpMagic,int aTargetIndex,int isCombo);
	void MLS_SkillSword4_Magumsa(LPOBJ lpObj,CMagicInf * lpMagic,int aTargetIndex,int isCombo);
	void MLS_SkillSword5(LPOBJ lpObj,CMagicInf * lpMagic,int aTargetIndex,int isCombo);
	void MLS_SkillSword1(LPOBJ lpObj,CMagicInf * lpMagic,int aTargetIndex,int isCombo);
	void MLS_SkillSword2(LPOBJ lpObj,CMagicInf * lpMagic,int aTargetIndex,int isCombo);
	int MLS_SkillStrengthenDesruction(LPOBJ lpObj,CMagicInf *lpMagic,BYTE x,BYTE y,int aTargetIndex,int isCombo);
	void MLS_KnightSkillAddLife(LPOBJ lpObj,CMagicInf *lpMagic);
	void MLS_SkillFulKnightSkillAddLife(LPOBJ lpObj,CMagicInf *lpMagic);
	void MLS_SkillMasteryKnightSkillAddLife(LPOBJ lpObj,CMagicInf *lpMagic);
	int MLS_SkillStrengthenMagicPower(LPOBJ lpObj,CMagicInf *lpMagic,int aTargetIndex);
	int MLS_SkillSkillfulMagicPower(LPOBJ lpObj,CMagicInf *lpMagic,int aTargetIndex);
	void MLS_SkillStrengthenFlame(LPOBJ lpObj,CMagicInf *lpMagic,int aTargetIndex);
	void MLS_SkillStrengthenThunder(LPOBJ lpObj,CMagicInf *lpMagic,int aTargetIndex);
	void MLS_SkillMeteorit(LPOBJ lpObj,int aTargetIndex,CMagicInf* lpMagic);
	void MLS_SkillStrengthenPoison(LPOBJ lpObj,CMagicInf *lpMagic,int aTargetIndex);
	void MLS_SkillStrengthenExPoison(int aIndex,CMagicInf *lpMagic,BYTE x,BYTE y,int aTargetIndex);
	void MLS_SkillStrengthenIce(LPOBJ lpObj,CMagicInf *lpMagic,int aTargetIndex);
	void MLS_SkillWizardMagicDefense(LPOBJ lpObj,CMagicInf *lpMagic,int aTargetIndex);
	void MLS_SkillWizardMagicDefenseMastery(LPOBJ lpObj,CMagicInf *lpMagic,int aTargetIndex);

	int MLS_SkillInfinityArrow(int aIndex,int aTargetIndex,CMagicInf *lpMagic);
	int MLS_SkillStrengthenBerserker(int aIndex,CMagicInf *lpMagic);
	int MLS_SkillSkillfulBerserker(int aIndex,CMagicInf *lpMagic);
	int MLS_SkillMasteryBerserker(int aIndex,CMagicInf *lpMagic);
	void MLS_SkillStrendthenAddCriticalDamage(int aIndex,int skill_level,CMagicInf *lpMagic);
	void MLS_SkillSkillFulAddCriticalDamage(int aIndex,int skill_level,CMagicInf *lpMagic);
	void MLS_SkillMasteryAddCriticalDamage(int aIndex,int skill_level,CMagicInf *lpMagic);
	int MLS_DarkHorseAttack(LPOBJ lpObj,int aTargetIndex,CMagicInf * lpMagic);
	int MLS_FireBurst(LPOBJ lpObj,int aTargetIndex,CMagicInf * lpMagic);
	int MLS_SkillStrengthenSpear(int aIndex,int aTargetIndex,CMagicInf *lpMagic);
	int MLS_SkillAreaMonsterAttack(int aIndex,CMagicInf *lpMagic,BYTE x,BYTE y,int aTargetIndex,int aDistance,int bPVP,int nSuccessRate,int nDelay);
	int MLS_SuddenIce(int aIndex,CMagicInf * lpMagic,BYTE x,BYTE y,int aTargetIndex);
	int MLS_CalDistance(int x,int y,int x1,int y1);
	int MLS_GetAngle(int x,int y,int tx,int ty);
	void MLS_SkillFrustrum(int aIndex,float fangle,float tx,float ty);
	int MLS_KnightBlow(int aIndex,int aTargetIndex,CMagicInf *lpMagic,int isCombo);
	int MLS_KnightBlowStrength(int aIndex,int aTargetIndex,CMagicInf *lpMagic,int isCombo);
	void MLS_SkillBlowOfFury(int aIndex,CMagicInf *lpMagic,BYTE x,BYTE y,int aTargetIndex,int isCombo);
	void MLS_SkillDefense(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	void MLS_SkillAttack(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	void MLS_SkillStrengthenAttack(int aIndex,int aTargetIndex,CMagicInf *lpMagic);
	void MLS_SkillStrengthenDefense(int aIndex,int aTargetIndex,CMagicInf *lpMagic);
	void MLS_MaGumSkillDefenseDown(int aIndex,int aTargetIndex);
	void MLS_PowerSlash(int aIndex,CMagicInf *lpMagic,BYTE x,BYTE y,BYTE Targetangle,int aTargetIndex);
	int MLS_FireScream(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	int MLS_WeaponSkillDel(LPOBJ lpObj,BYTE aSkill,BYTE Level);
	int MLS_WeaponSkillAdd(LPOBJ lpObj,BYTE aSkill,BYTE Level);
	int CheckUsableWeaponSkill(LPOBJ lpObj,int nCount);
	int MLS_SkillSleep(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	int MLS_SkillChainLightning(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	int MLS_SkillLightningShock(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	int MLS_SkillDrainLife(int aIndex,int aTargetIndex,CMagicInf * lpMagic);
	int MLS_SkillHellFire2(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	int MLS_SkillHellFire2Start(int aIndex, CMagicInf * lpMagic);
	void MLS_SkillEarthPrison(int aIndex,int aTargetIndex,CMagicInf* lpMagic);
	int MLS_SkillCure(int aIndex,int aTargetIndex,CMagicInf* lpMagic);
	void MLS_SkillMultiShot(int aIndex,CMagicInf* lpMagic,BYTE targetpos,int aTargetIndex);
	int MLS_SkillBless(int aIndex,int aTargetIndex,CMagicInf* lpMagic);
	void MLS_MaGumSkillDefenseDownMastery(int aIndex,int aTargetIndex,CMagicInf* lpMagic);
	void MLS_SkillGiganticStorm(int aIndex,CMagicInf* lpMagic,BYTE x,BYTE y,int aTargetIndex);
	void MLS_SkillFlameStrike(int aIndex, CMagicInf * lpMagic,BYTE targetangle, int aTargetIndex);
	int MLS_SkillWeakness(int aIndex,CMagicInf* lpMagic,BYTE x,BYTE y,int aTargetIndex);
	int MLS_SkillInnovation(int aIndex,CMagicInf* lpMagic,BYTE x,BYTE y,int aTargetIndex);
	int MLS_SkillBlind(int aIndex,int aTargetIndex,CMagicInf* lpMagic);
	void MLS_SkillFulMasteryAddCriticalDamage(int aIndex,int aTargetIndex,CMagicInf *lpMagic);
	void MLS_SkillElectricSpark(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	void MLS_SkillChaoticDesier(int aIndex, CMagicInf * lpMagic, BYTE TargetPos, int aTargetIndex);
	void MLS_SkillIronDefense(int aIndex,int aTargetIndex,CMagicInf* lpMagic);
	void MLS_SkillMonkBarrageJustOneTarget(int aIndex,CMagicInf * lpMagic,int aTargetIndex);
	void MLS_SkillMonkBarrageJustOneTargetMastery(int aIndex,CMagicInf * lpMagic,int aTargetIndex);
	int MLS_SkillMonkBuffApplyParty(int aIndex,CMagicInf * lpMagic);
	int MLS_SkillMonkBuffApplyPartyMastery(int aIndex,CMagicInf * lpMagic);
	void MLS_SkillPartyHeal(int aIndex,int aTargetIndex,CMagicInf* lpMagic);

	int GetRequireMLPoint(int iClass,int iSkill);
	int GetMaxPointOfMasterSkill(int iClass,int iSkill);
	float GetMasterSkillValue(int iType,int iPoint);
	float GetMasterSkillManaIncRate(int iPoint);
	float GetBrandOfMasterSkillValue(LPOBJ lpObj,int iSkill,int iStep);
	int CheckSkillLevel(LPOBJ lpObj,int iSkill);
	int CheckPreviousRankSkill(LPOBJ lpObj,int iSkill,int iLevel);
	int GetMasterSkillUIIndex(int iClass,int iSkill);
	int GetParentSkill1(int iClass,int iSkill);
	int GetParentSkill2(int iClass,int iSkill);
	void SetItemMLPassiveSkill(LPOBJ lpObj,int iType);
	void SetPetItemMLPassiveSkill(LPOBJ lpObj,int iType);
	int GetCurSkillLevel(LPOBJ lpObj,int iSkill);
	float RoundValue(double dbValue,int iPos);
	float GetSkillAttackDamage(LPOBJ lpObj,int iSkill);
	void MLS_SkillFrustrum3(int aIndex,BYTE aTargetPos,float f1,float f2,float f3,float f4);
	
private:

	_TreeData m_SkillTreeData[MAX_SKILLTREE_CLASS];
	float m_fValue[81][MAX_SKILLTREE_LEVEL+1];
	MULua m_Lua;
	bool IsLoaded;

}; extern CMasterLevelSkillTreeSystem g_MasterSkillSystem;