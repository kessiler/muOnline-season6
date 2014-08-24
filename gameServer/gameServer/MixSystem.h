#pragma once
// -------------------------------------------------------------------------------

#include "protocol.h"
#include "..\common\zzzitem.h"
// -------------------------------------------------------------------------------

#define CHAOS_BOX_SIZE				32
#define MIN_CHAOS_ITEM_LEVEL		4
#define CHAOS_BOX_RANGE(x)			( ((x) < 0) ? FALSE : ((x) > CHAOS_BOX_SIZE - 1) ? FALSE : TRUE )
#define CB_ERROR					0
#define CB_SUCCESS					1
#define CB_NOT_ENOUGH_ZEN			2
#define CB_TOO_MANY_ITEMS			3
#define CB_LOW_LEVEL_USER			4
#define CB_LACKING_MIX_ITEMS		6
#define CB_INCORRECT_MIX_ITEMS		7
#define CB_INVALID_ITEM_LEVEL		8
#define CB_USER_CLASS_LOW_LEVEL		9
#define CB_NO_BC_CORRECT_ITEMS		10
#define CB_BC_NOT_ENOUGH_ZEN		11
// -------------------------------------------------------------------------------

enum CHAOS_TYPE	//-> Complete (All positions & names are original for 1.01.00)
{
	CHAOS_TYPE_DEFAULT							= 1,
	CHAOS_TYPE_DEVILSQUARE						= 2,
	CHAOS_TYPE_UPGRADE_10						= 3,
	CHAOS_TYPE_UPGRADE_11						= 4,
	CHAOS_TYPE_UPGRADE_12						= 22,
	CHAOS_TYPE_UPGRADE_13						= 23,
	CHAOS_TYPE_UPGRADE_14						= 49,
	CHAOS_TYPE_UPGRADE_15						= 50,
	CHAOS_TYPE_DINORANT							= 5,
	CHAOS_TYPE_FRUIT							= 6,
	CHAOS_TYPE_SECOND_WING						= 7,
	CHAOS_TYPE_BLOODCATLE						= 8,
	CHAOS_TYPE_FIRST_WING						= 11,
	CHAOS_TYPE_SETITEM							= 12,
	CHAOS_TYPE_DARKHORSE						= 13,
	CHAOS_TYPE_DARKSPIRIT						= 14,
	CHAOS_TYPE_CLOAK							= 24,
	CHAOS_TYPE_BLESS_POTION						= 15,
	CHAOS_TYPE_SOUL_POTION						= 16,
	CHAOS_TYPE_LIFE_STONE						= 17,
	CHAOS_TYPE_CASTLE_ITEM						= 18,
	CHAOS_TYPE_HT_BOX							= 20,
	CHAOS_TYPE_FENRIR_01						= 25,
	CHAOS_TYPE_FENRIR_02						= 26,
	CHAOS_TYPE_FENRIR_03						= 27,
	CHAOS_TYPE_FENRIR_04						= 28,
	CHAOS_TYPE_COMPOUNDPOTION_LV1				= 30,
	CHAOS_TYPE_COMPOUNTPOTION_LV2				= 31,
	CHAOS_TYPE_COMPOUNTPOTION_LV3				= 32,
	CHAOS_TYPE_JEWELOFHARMONY_PURITY			= 33,
	CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM	= 34,
	CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM		= 35,
	CHAOS_TYPE_380_OPTIONITEM					= 36,
	CHAOS_TYPE_ILLUSION_TEMPLE_ITEM				= 37,
	CHAOS_TYPE_THIRD_WING_1						= 38,
	CHAOS_TYPE_THIRD_WING_2						= 39,
	CHAOS_TYPE_LOTTERY_MIX						= 40,
	CHAOS_TYPE_CHERRY_MIX						= 41,
	CHAOS_TYPE_SEED_EXTRACT						= 42,
	CHAOS_TYPE_SEED_COMPOSIT					= 43,
	CHAOS_TYPE_SEED_EQUIP						= 44,
	CHAOS_TYPE_SEED_REMOVE						= 45,
	CHAOS_TYPE_DUNGEON_KEYITEM					= 46,
	CHAOS_TYPE_GOLD_AND_SILBER_BOX				= 47,
	CHAOS_TYPE_LUCKY_ITEM_EXCHANGE				= 51,
	CHAOS_TYPE_LUCKY_ITEM_SMELTING				= 52,
};
// -------------------------------------------------------------------------------

struct PMSG_CHAOSMIXRESULT	//-> Complete
{
	PBMSG_HEAD	h;
	// ----
	BYTE		Result;
	BYTE		ItemInfo[MAX_ITEM_INFO];
};
// -------------------------------------------------------------------------------

struct CB_PMSG_REQ_PRESENT	//-> Complete
{
	PBMSG_HEAD	h;
	// ----
	char		AccountId[10];
	char		GameId[10];
	int			Sequence;
};
// -------------------------------------------------------------------------------

class CMixSystem	//-> Complete
{
public:
				CMixSystem();
	virtual		~CMixSystem();
	// ----
	BOOL		ChaosBoxCheck(LPOBJ lpObj);
	BOOL		ChaosBoxInit(LPOBJ lpObj);
	BOOL		ChaosBoxItemDown(LPOBJ lpObj);
	int			ChaosBoxMix(LPOBJ lpObj, int & Result2, int & Result3);
	// ----
	void		DefaultChaosMix(LPOBJ lpObj);
	void		LogDQChaosItem(LPOBJ lpObj);
	int			CheckDevilSquareItem(LPOBJ lpObj, int & eventitemcount, int & itemlevel);
	BOOL		DevilSquareEventChaosMix(LPOBJ lpObj, BOOL bCheckType, int iItemLevel);
	void		DevilSquareItemChaosMix(LPOBJ lpObj);
	void		LogPlusItemLevelChaosItem(LPOBJ lpObj, int iPlusMixLevel);
	void		LogChaosItem(LPOBJ lpObj, LPSTR sLogType);
	BOOL		PlusItemLevelChaosMix(LPOBJ lpObj, int mixType);
	void		ChaosMixCharmItemUsed(LPOBJ lpObj);
	BOOL		IsDeleteItem(int iItemCode);
	BOOL		PegasiaChaosMix(LPOBJ lpObj);
	BOOL		CircleChaosMix(LPOBJ lpObj);
	BOOL		WingChaosMix(LPOBJ lpObj);
	void		BloodCastleItemChaosMix(LPOBJ lpObj);
	void		DarkHorseChaosMix(LPOBJ lpObj);
	void		DarkSpiritChaosMix(LPOBJ lpObj);
	void		BlessPotionChaosMix(LPOBJ lpObj);
	void		SoulPotionChaosMix(LPOBJ lpObj);
	void		LifeStoneChaosMix(LPOBJ lpObj);
	void		CastleSpecialItemMix(LPOBJ lpObj); //-> _CS
	void		HiddenTreasureBoxItemMix(LPOBJ lpObj);
	void		Fenrir_01Level_Mix(LPOBJ lpObj);
	void		Fenrir_02Level_Mix(LPOBJ lpObj);
	void		Fenrir_03Level_Mix(LPOBJ lpObj);
	void		Fenrir_04Upgrade_Mix(LPOBJ lpObj);
	void		ShieldPotionLv1_Mix(LPOBJ lpObj);
	void		ShieldPotionLv2_Mix(LPOBJ lpObj);
	void		ShieldPotionLv3_Mix(LPOBJ lpObj);
	// ----
	BOOL		IsMixPossibleItem(int iItemCode);
	BOOL		CheckWingItem(int iItemCode);
	BOOL		Check2ndWingItem(int iItemCode);
	BOOL		Check3rdWingItem(int iItemCode);
	BOOL		CheckLevelCondition(CItem * lpItem, short Level, BYTE Op1, BYTE Op2, BYTE Op3, BYTE SetOption, BYTE NewOption);
	void		ThirdWingLevel1_Mix(LPOBJ lpObj);
	void		ThirdWingLevel2_Mix(LPOBJ lpObj);
	void		ThirdWingMixFail(LPOBJ lpObj);
	void		ThirdWingMixFailItemPanalty(CItem * lpItem);
	// ----
	void		SetItemChaosMix(LPOBJ lpObj);
	void		CBUPS_ItemRequest(int aIndex, char* AccountId, char* Name);
	void		ChaosCardMix(LPOBJ lpObj);
	void		CherryBlossomMix(LPOBJ lpObj);
	// ----
	void		MixExtractSeed(LPOBJ lpObj);
	void		MixCompositeSeedSphere(LPOBJ lpObj);
	void		MixSetSeedSphere(LPOBJ lpObj, BYTE SocketSlot);
	void		MixRemoveSeedSphere(LPOBJ lpObj, BYTE SocketSlot);
	// ----
	void		EventDungeonKeyItemMix(LPOBJ lpObj);	//1.01.00
	void		GoldenNSilverBoxMix(LPOBJ lpObj);		//1.01.00
	// ----
private:
	int			CHAOS_GEM_ITEMINDEX;		// +0x4(0x4)
    int			BLESS_GEM_ITEMINDEX;		// +0x8(0x4)
    int			SOUL_GEM_ITEMINDEX;			// +0xc(0x4)
    int			CREATURE_GEM_ITEMINDEX;		// +0x10(0x4)
    int			BLESS_GEM_BUNDLE_ITEMINDEX; // +0x14(0x4)
    int			SOUL_GEM_BULDLE_ITEMINDEX;	// +0x18(0x4)
    int			CONDOR_FEATHER_ITEMINDEX;	// +0x1c(0x4)
    int			CONDOR_FLAME_ITEMINDEX;		// +0x20(0x4)
    int			WING_OF_STORM_ITEMINDEX;	// +0x24(0x4)
    int			WING_OF_ETERNAL_ITEMINDEX;	// +0x28(0x4)
    int			WING_OF_ILLUSION_ITEMINDEX; // +0x2c(0x4)
    int			WING_OF_RUIN_ITEMINDEX;		// +0x30(0x4)
    int			CAPE_OF_EMPEROR_ITEMINDEX;	// +0x34(0x4)
    int			WING_OF_DIMENSION_ITEMINDEX;// +0x38(0x4)
	int			CAPE_OF_OVERRULE_ITEMINDEX;	// +0x3c(0x4)
    int			m_i3rdWingOpt_LuckRate;		// +0x40(0x4)
	// ----
}; extern CMixSystem g_MixSystem;
// -------------------------------------------------------------------------------

void ChaosEventProtocolCore(BYTE protoNum, unsigned char* aRecv, int aLen);
// -------------------------------------------------------------------------------