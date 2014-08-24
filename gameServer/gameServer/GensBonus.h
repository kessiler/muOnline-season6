//for Alexander
#pragma once

struct GensBonusData
{
	BYTE	StartHour;
	BYTE	EndHour;
	BYTE	DefenseIncrease;
	BYTE	DefenseRateIncrease;
	BYTE	DamageIncrease;
	BYTE	ExcellentDamageIncrease;
	BYTE	IgnoreDefenseIncrease;
	BYTE	MaxLifeIncrease;
	BYTE	MaxManaIncrease;
	BYTE	BattleExpIncrease;
	bool	FreeKill;
	bool	FreeKillMove;
	bool	FreeEvent;
};

class GensBonus
{
public:
			GensBonus();
			~GensBonus();
	// ----
	void	Init();
	void	Load();
	void	Read(char * File);
	// ----
	bool	IsBonusTime(int UserIndex);
	void	ApplyBonus(int UserIndex);
	void	CheckBonusTime();
	// ----
private:
	std::vector<GensBonusData> m_Bonus;
	bool	m_IsVanertTime;
	bool	m_IsDuprianTime;
	// ----
}; extern GensBonus g_GensBonus;