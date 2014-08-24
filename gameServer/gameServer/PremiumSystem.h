//for Alexander
#pragma once

struct PremiumSystemPrice
{
	int		Days;
	BYTE	PayType; //0 - WCoinC, 1 - WCoinP, 2 - WCoinG
	int		PayAmount;
};

class PremiumSystem
{
public:
			PremiumSystem();
			~PremiumSystem();
	// ----
	void	Init();
	void	Load();
	void	ReadPrice(char * File);
	void	ReadMain(char * File);
	// ----
	bool	IsPremiumUser(int UserIndex);
	void	ApplyBonus(int UserIndex);
	void	BuyPremium(int UserIndex, int Days);
	void	RemovePremium(int UserIndex);
	void	CheckEndTime(int UserIndex);
	// ----
private:
	std::vector<PremiumSystemPrice> m_Price;
	// ----
	BYTE	m_ChaosMachineRateIncrease;
	BYTE	m_ChaosMachineMoneyDecrease;
	BYTE	m_DieMoneyPenaltyPercent;
	short	m_ResetDelayMin;
	BYTE	m_DamagePowerIncrease;
	BYTE	m_DefenseIncrease;
	BYTE	m_CriticalDamageIncrease;
	BYTE	m_ExcellentDamageIncrease;
	BYTE	m_MaxLifeIncrease;
	BYTE	m_MaxManaIncrease;
	BYTE	m_MaxSDIncrease;
	BYTE	m_MaxAGIncrease;
	// ----
}; extern PremiumSystem g_PremiumSystem;
