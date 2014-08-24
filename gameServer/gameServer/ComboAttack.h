#pragma once
// -------------------------------------------------------------------------------

struct ComboSkillData
{
	void	Init();
	DWORD	dwTime;
	short	Skill[3];
	int		ProgressIndex;
};
// -------------------------------------------------------------------------------

class CComboAttack
{
public:
			CComboAttack();
	virtual ~CComboAttack();
private:
	int		GetSkillPos(int skillnum);
public:
	int		CheckCombo(int aIndex, int skillnum);
}; extern CComboAttack gComboAttack;
// -------------------------------------------------------------------------------