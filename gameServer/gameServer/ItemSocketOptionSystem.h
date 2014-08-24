//-> Decompiled by DarkSim | 24.01.2013 | 1.01.00 GS-N (All names are original)
// -------------------------------------------------------------------------------

#pragma once
// -------------------------------------------------------------------------------

#include "ItemSeedSphere.h"
#include "..\common\ItemSocketOptionDefine.h"
// -------------------------------------------------------------------------------

extern int g_SocketOptionSystem;
extern int g_RequireMoneyForSeedExtract;
extern int g_RequireMoneyForSeedSphereComposite;
extern int g_RequireMoneyForSeedSphereSet;
extern int g_RequireMoneyForSeedSphereRemove;
extern int g_ShopBuySocketItemSlotCount;
// -------------------------------------------------------------------------------

#define MAX_SOCKET_ITEM		150
// -------------------------------------------------------------------------------

#ifdef __CUSTOMS__
struct SOCKET_ITEM_LIST
{
	short	ItemID;
	short	ItemIndex;
};
#endif
// -------------------------------------------------------------------------------

class CItemSocketOptionSystem //-> Complete
{
public:
	CItemSocketOptionSystem();
	virtual ~CItemSocketOptionSystem();
	// ----
	void	Initialize();
	bool	LoadOptionScript(char * pchFileName);
	void	LoadCommonServerScript();
	// ----
	bool	IsEnableSocketItem(CItem * lpItemData);
	bool	IsEnableSocketItem(int ItemCode);
	// ----
	BYTE	GetEmptySlotCount(CItem * lpItemData);
	BYTE	GetMaxSlotCount(int iItemCode);
	bool	GetSeedSphereEffect(BYTE btSeedSphereIndex, BYTE & pbSeedType, BYTE & pbEffectValueType, BYTE & pbEffectValue);
	void	GetSocketOption(CItem * lpItemData, LPBYTE pbSocketOption, BYTE & pbSocketBonusOption);
	// ----
	bool	IsSocketItem(CItem * lpItemData);
	bool	IsSeedItem(int ItemCode);
	bool	IsSphereItem(int ItemCode);
	bool	IsSeedSphereItem(int ItemCode);
	// ----
	BYTE	GetSeedIndex(int ItemCode, BYTE ItemLevel);
	BYTE	GetSphereLevel(int ItemCode);
	// ----
	bool	CheckItemForSameSeedType(CItem * lpItemData, WORD SeedSphereItemCode, BYTE ItemLevel);
	// ----
	BYTE	GetSeedSphereIndexForItemCode(WORD ItemCode, BYTE ItemLevel);
	long	GetSeedSphereValue(BYTE btSeedSphereIndex);
	// ----
	bool	CheckSetSeedSphereItemType(CItem * lpItemData, BYTE btSeedSphereIndex);
	bool	IsEnableDropSocketItemMap(int iMapIndex);
	int		GetSphereDropInfo(int iMonsterLevel);
	BYTE	GetSeedSphereIndex(BYTE btSeedIndex, BYTE btSphereLevel);
	// ----
	bool	MakeSeedSphere(CSeedSphereItem * lpSeedSphere, BYTE btSeedIndex, BYTE btSphereLevel);
	// ----
	CSeedItem	* GetSeedData(BYTE btSeedIndex);
	CSphereItem * GetSphereData(BYTE btSphereLevel);
	// ----
	BYTE	GetExtractSeed();
	BYTE	GetSocketType(BYTE btSeedSphereIndex);
	// ----
	BONUS_SOCKET_OPTION * GetBonusSocketOption(BYTE btBonusSocketOptionIndex);
	// ----
	int		ApplySeedSphereEffect(OBJECTSTRUCT * lpObj);
	void	ApplyBonusOptionEffect(OBJECTSTRUCT * lpObj);
	void	ApplySetOptionEffect(OBJECTSTRUCT * lpObj);
	bool	SetApplySocketEffect(OBJECTSTRUCT * lpObj);
	void	SetClearSocketEffect(OBJECTSTRUCT * lpObj);
	void	SetEffect(OBJECTSTRUCT * lpObj, BYTE btOptionType, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void	ClearEffect(OBJECTSTRUCT * lpObj, SOCKET_OPTION_LIST * lpOptionData);
	// ----
	void	_SetFireEffect(OBJECTSTRUCT * lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void	_SetWaterEffect(OBJECTSTRUCT * lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void	_SetIceEffect(OBJECTSTRUCT * lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void	_SetWindEffect(OBJECTSTRUCT * lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void	_SetThunderEffect(OBJECTSTRUCT * lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void	_SetEarthEffect(OBJECTSTRUCT * lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void	_SetUniqueEffect(OBJECTSTRUCT * lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	void	_SetSetEffect(OBJECTSTRUCT * lpObj, BYTE btEffectType, BYTE btEffectValueType, WORD wEffectValue);
	// ----
	int		CalcEffectValue(int iInputValue, WORD wEffectValue, BYTE btEffectValueType);
	void	SetRequireStatOption(CItem * lpItemdata);
	void	MakeSocketSlot(CItem * lpItemData, BYTE btSocketSlotCount);
	BYTE	GetMakeSocketSlotCount(int iItemCode);
	void	ClearUserData(OBJECTSTRUCT * lpObj);
	bool	SetSocketSlot(CItem * lpTargetItem, CItem * lpSeedSphere, BYTE btSocketSlotIndex);
	bool	SetSocketSlot(CItem * lpTargetItem, BYTE btSeedIndex, BYTE btSphereLevel, BYTE btSocketSlotIndex);
	bool	SetSocketSlot(CItem * lpTargetItem, BYTE btSeedSphereIndex, BYTE btSocketSlotIndex);
	bool	SetBonusSocketOption(CItem * lpItemData);
	bool	ClearSocketSlot(CItem * lpTargetItem, BYTE btSocketSlotIndex);
	// ----
private:	//-> 1.00.92
	CSeedItem			m_SeedData[MAX_SEED_COUNT];
	CSphereItem			m_SphereData[MAX_SOCKET_SLOT];
	BONUS_SOCKET_OPTION m_BonusSocketOptionData[MAX_SEED_TYPE];
	SET_SOCKET_OPTION	m_SetSocketOptionData[2];
	TRandomPoolMgr		m_SeedExtractPool;
	TRandomPoolMgr		m_MakeSocketSlotCountPool;
	TRandomPoolMgr		m_TwoHandWeaponMakeSocketSlotCountPool;
	int					m_MakeSocketSlotRate[MAX_SOCKET_SLOT];
	int					m_TwoHandWeaponMakeSocketSlotRate[MAX_SOCKET_SLOT];
	int					m_SphereDropOn;
	int					m_SphereDropRate[MAX_SOCKET_SLOT];
	int					m_SphereDropLevel[MAX_SOCKET_SLOT];
	// ----
#ifdef __CUSTOMS__
	SOCKET_ITEM_LIST	m_SocketItemList[MAX_SOCKET_ITEM];
	int					m_SocketItemCount;
#endif
	// ----
}; extern CItemSocketOptionSystem gItemSocketOption;
// -------------------------------------------------------------------------------