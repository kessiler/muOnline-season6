#pragma once

struct CLASSCALC_DATA
{
	WORD LevelPoints;
	WORD AttackDamageMinRightDiv1;
	WORD AttackDamageMaxRightDiv1;
	WORD AttackDamageMinLeftDiv1;
	WORD AttackDamageMaxLeftDiv1;
	WORD AttackDamageMinRightDiv2;
	WORD AttackDamageMaxRightDiv2;
	WORD AttackDamageMinLeftDiv2;
	WORD AttackDamageMaxLeftDiv2;
	WORD AttackDamageMinRightDiv3;
	WORD AttackDamageMaxRightDiv3;
	WORD AttackDamageMinLeftDiv3;
	WORD AttackDamageMaxLeftDiv3;
	WORD MagicDamageMinDiv;
	WORD MagicDamageMaxDiv;
	WORD CurseDamageMinDiv;
	WORD CurseDamageMaxDiv;
	WORD AttackSpeedDiv;
	WORD MagicSpeedDiv;
	WORD DefenseDiv;
	WORD PvPAttackRateFact1;
	float PvPAttackRateFact1RF;
	WORD PvPAttackRateFact2;
	WORD PvPAttackRateDiv;
	WORD PvPDefenseRateFact;
	float PvPDefenseRateFactRF;
	WORD PvPDefenseRateDiv;
	WORD SuccessBlockDiv;
	WORD PetAttackDamageMinFact;
	WORD PetAttackDamageMinDiv;
	WORD PetAttackDamageMinStart;
	WORD PetAttackDamageMaxFact;
	WORD PetAttackDamageMaxDiv;
	WORD PetAttackDamageMaxStart;
	WORD PetCriticalAttackRate;
	WORD PetAttackSpeedFact;
	WORD PetAttackSpeedDiv1;
	WORD PetAttackSpeedDiv2;
	WORD PetAttackSpeedStart;
	WORD PetSuccessAttackRateDiv;
	WORD PetSuccessAttackRateStart;
};

class ClassCalc
{
public:
	ClassCalc();
	virtual ~ClassCalc();
	void	Load();
	void	ReadData(char * File);	
	CLASSCALC_DATA m_Data[7];
}; 
extern ClassCalc g_ClassCalc;
