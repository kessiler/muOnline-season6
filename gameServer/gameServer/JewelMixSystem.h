#pragma once
// -------------------------------------------------------------------------------

struct JEWELMIX_INFO
{
    int		m_iJewelCount;
    int		m_iMixMoney;
    int		m_iSourceType;
    int		m_iChangeType;
};
// -------------------------------------------------------------------------------

struct CJewelMixSystem
{
public:
	CJewelMixSystem();
	virtual ~CJewelMixSystem();
	// ----
	static int	GetJewelCount(int iIndex, int iJewelType);
	static int	GetJewelCountPerLevel(int iJewelType, int iJewelLevel);
	static int	MixJewel(int iIndex, int iJewelType, int iMixType);	//-> int bool, return BYTE
	static int	UnMixJewel(int iIndex, int iJewelType, int iJewelLevel, int iInventoryPos); //-> int bool, return BYTE
};
// -------------------------------------------------------------------------------