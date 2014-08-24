#ifndef GAMEMAIN_H
#define	GAMEMAIN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wsJoinServerCli.h"
#include "wsGameServer.h"
#include "DirPath.h"
#include "MapClass.h"
#include "NSerialCheck.h"
#include "..\common\classdef.h"
#include "CLoginCount.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "WzUdp.h"
#include "WhisperCash.h"
#include "DbSave.h"
#include "..\SimpleModulus\SimpleModulus.h"
#include "MonsterAttr.h"
#include "MonsterSetBase.h"
#include "PartyClass.h"
#include "ItemBagEx.h"
#include "ProbabilityItemBag.h"

#ifdef IMPERIAL
#include "EventDungeonItemBag.h"
#endif

//#if (_GSCS==1)
#include "CastleSiege.h"
#include "CastleDeepEvent.h"
#include "Crywolf.h"
//#endif

#define FIRST_PATH "c:\\Muproject\\data\\" //Original Path
#define COMMONSERVER_MAINSECTION "GameServerInfo"
#define COMMONSERVER_FILE "commonserver.cfg"

struct PMSG_FRIEND_STATE
{
	PBMSG_HEAD h;	// C1:C4
	char Name[10];	// 3
	BYTE State;	// D
};

enum MU_EVENT_TYPE {
	MU_EVENT_ALL = 0x0,
	MU_EVENT_DEVILSQUARE = 0x1,
	MU_EVENT_BLOODCASTLE = 0x2,
	MU_EVENT_ATTACKEVENTTROOP = 0x3,
	MU_EVENT_GOLDENTROOP = 0x4,
	MU_EVENT_WHITEMAGETROOP = 0x5,
	MU_EVENT_LOVEPANGPANG = 0x6,
	MU_EVENT_FIRECRACKER = 0x7,
	MU_EVENT_MEDALION = 0x8,
	MU_EVENT_XMASSTAR = 0x9,
	MU_EVENT_HEARTOFLOVE = 0xa,
	MU_EVENT_SAY_HAPPYNEWYEAR = 0xb,
	MU_EVENT_SAY_MERRYXMAS = 0xc,
	MU_EVENT_CHAOSCASTLE = 0xd,
	MU_EVENT_CHRISTMAS_RIBBONBOX = 0xe,
	MU_EVENT_VALENTINESDAY_CHOCOLATEBOX = 0xf,
	MU_EVENT_WHITEDAY_CANDYBOX = 0x10,
};

enum MU_ETC_TYPE {
	MU_ETC_ALL = 0x0,
	MU_ETC_CREATECHARACTER = 0x1,
	MU_ETC_GUILD = 0x2,
	MU_ETC_TRADE = 0x3,
	MU_ETC_USECHAOSBOX = 0x4,
	MU_ETC_PERSONALSHOP = 0x5,
	MU_ETC_PKITEMDROP = 0x6,
	MU_ETC_ITEMDROPRATE = 0x7,
	MU_ETC_SPEEDHACK = 0x8,
	MU_ETC_GAMEGUARD = 0x9,
};

extern BOOL JoinServerConnected;
extern BOOL DataServerConnected;
extern BOOL GameServerCreated;
extern BOOL DevilSquareEventConnect;
extern BOOL IsDevilSquareEventConnected;
extern BOOL EventChipServerConnect;
extern BOOL IsEventChipServerConnected;
extern CDragonEvent * DragonEvent;
extern CAttackEvent * AttackEvent;
extern CItemBag * LuckboxItemBag;
extern CItemBag * Mon55;
extern CItemBag * Mon53;
extern CItemBagEx * StarOfXMasItemBag;
extern CItemBag * FireCrackerItemBag;
extern CItemBag * HeartOfLoveItemBag;
extern CItemBag * GoldMedalItemBag;
extern CItemBag * SilverMedalItemBag;
extern CItemBag * EventChipItemBag;
extern CProbabilityItemBag * GoldGoblenItemBag;
extern CProbabilityItemBag * TitanItemBag;
extern CProbabilityItemBag * GoldDerconItemBag;
extern CProbabilityItemBag * DevilLizardKingItemBag;
extern CProbabilityItemBag * KanturItemBag;
extern CItemBag * RingEventItemBag;
extern CItemBag * FriendShipItemBag;
extern CItemBag * DarkLordHeartItemBag;
extern CItemBagEx * KundunEventItemBag;

//#if(_GSCS==1)
extern CItemBagEx * CastleHuntZoneBossItemBag;
extern CItemBagEx * CastleItemMixItemBag;
//#endif

extern CItemBagEx * HiddenTreasureBoxItemBag;
extern CItemBagEx * RedRibbonBoxEventItemBag;
extern CItemBagEx * GreenRibbonBoxEventItemBag;
extern CItemBagEx * BlueRibbonBoxEventItemBag;
extern CItemBagEx * PinkChocolateBoxEventItemBag;
extern CItemBagEx * RedChocolateBoxEventItemBag;
extern CItemBagEx * BlueChocolateBoxEventItemBag;
extern CItemBagEx * LightPurpleCandyBoxEventItemBag;
extern CItemBagEx * VermilionCandyBoxEventItemBag;
extern CItemBagEx * DeepBlueCandyBoxEventItemBag;
extern CItemBagEx * CrywolfDarkElfItemBag;
extern CItemBagEx * CrywolfBossMonsterItemBag;

//#if(_GSCS==0)
extern CItemBagEx * KanturuMayaHandItemBag;
extern CItemBagEx * KanturuNightmareItemBag;
//#endif

extern CItemBagEx * HallowinDayEventItemBag;

extern CItemBag * RingOfHeroBoxItemBag;

extern CProbabilityItemBag * NewYearLuckyPouchItemBag; //test
extern CProbabilityItemBag * GMPresentBoxItemBag; //test
extern CProbabilityItemBag * IllusionTemple1ItemBag; //test
extern CProbabilityItemBag * IllusionTemple2ItemBag; //test
extern CProbabilityItemBag * IllusionTemple3ItemBag; //test
extern CProbabilityItemBag * MoonHarvestItemBag; //test
extern CProbabilityItemBag * MoonHarvestItemBag2; //test

extern CProbabilityItemBag * CherryBlossom1; //test
extern CProbabilityItemBag * CherryBlossom2; //test
extern CProbabilityItemBag * CherryBlossom3; //test
extern CProbabilityItemBag * CherryBlossom4; //test

extern CProbabilityItemBag * PCBangGageGreenBox; //test
extern CProbabilityItemBag * PCBangGageRedBox; //test
extern CProbabilityItemBag * PCBangGagePurpleBox; //test
extern CProbabilityItemBag * ReservedBox; //test

extern CProbabilityItemBag * RaklionSelupanItemBag; //test

extern CProbabilityItemBag * XMasEventA; //test
extern CProbabilityItemBag * XMasEventB; //test
extern CProbabilityItemBag * XMasEventC; //test

extern CProbabilityItemBag * LuckyCoin10; //test
extern CProbabilityItemBag * LuckyCoin20; //test
extern CProbabilityItemBag * LuckyCoin30; //test

#ifdef IMPERIAL
extern CEventDungeonItemBag *pEventDungeonItemBag;
extern CEventDungeonItemBag *pEventDungeonItemBagGaion;
extern CEventDungeonItemBag *pEventDungeonItemBagStone;
#endif

#ifdef SEASON6DOT3_ENG
extern CProbabilityItemBag * GoldenBoxItemBag;
extern CProbabilityItemBag * SilverBoxItemBag;
extern CProbabilityItemBag * ShineJewelleryCaseItemBag;
extern CProbabilityItemBag * RefinedJewelleryCaseItemBag;
extern CProbabilityItemBag * IronJewelleryCaseItemBag;
extern CProbabilityItemBag * OldJewelleryCaseItemBag;
extern CProbabilityItemBag * NewMonsterItemBag;
extern CProbabilityItemBag * BoxOfGreenColorItemBag;
extern CProbabilityItemBag * BoxOfRedColorItemBag;
extern CProbabilityItemBag * BoxOfPurpleColorItemBag;
#endif

extern BOOL SpeedHackPlayerBlock;
extern BOOL bCanConnectMember;
extern BOOL bCanChangeCharacterName;
extern int  gServerType;
extern int  gPartition;
extern BOOL gApplyHeroSystem;
extern int  gSpeedHackPenalty;
extern BOOL gEnableEventNPCTalk;
extern BOOL gEnableServerDivision;
extern BOOL gEvent1;
extern int  gMonsterHp;
extern BOOL gMerryXMasNpcEvent;
extern BOOL gHappyNewYearNpcEvent;
extern int  gEvent1ItemDropTodayCount;
extern int  gLanguage;
extern BOOL gChaosEvent;
extern char gChaosEventServerIp[20];
extern char gDevilSquareEventServerIp[20];
extern char gHackLogServerIp[20];
extern char gEventChipServerIp[20];
extern char gStalkProtocolId[11];
extern BOOL gNonPK;
extern BOOL gPkLimitFree;
extern BOOL gXMasEvent;
extern BOOL gFireCrackerEvent;
extern BOOL gHeartOfLoveEvent;
extern BOOL gMedalEvent;
extern BOOL gEventChipEvent;
extern BOOL gDevilSquareEvent;
extern BOOL gWriteSkillLog;
extern BOOL g_bStoneItemDrop;
extern BOOL g_bDoRingEvent;
extern BOOL g_bDoXMasAttackEvent;
extern BOOL g_bEventManagerOn;
extern int  g_iKundunMarkDropRate;
extern int  g_iMarkOfTheLord;
extern int g_iJapan1StAnivItemDropRate;
extern int  g_iDarkLordHeartDropRate;
extern int  g_iDarkLordHeartOffEventRate;
extern int g_iMysteriousBeadDropRate1;
extern int g_iMysteriousBeadDropRate2;
extern int g_iHiddenTreasureBoxOfflineRate;
extern BOOL bIsIgnorePacketSpeedHackDetect;
extern BOOL gIsKickDetecHackCountLimit;
extern BOOL gTamaJJangEvent;
extern int gAppearTamaJJang;
extern int gTamaJJangTime;
extern BOOL gIsItemDropRingOfTransform;
extern BOOL gIsEledoradoEvent;
extern BOOL gDoPShopOpen;
extern BOOL gDisconnectHackUser;
extern int g_iBlockKanturuMapEnter;
extern int g_iBlockCastleSiegeMapEnter;
extern int g_iBlockRaklionMapEnter;
extern BOOL GSInfoSendFlag;
extern int  GameServerPort;
extern int  JoinServerPort;
extern int  DataServerPort;
extern int  DataServerPort2;
extern int  ExDbPort;
extern int  GameServerUpdPort;
extern char RankingServerIP[256];
extern int RankingServerPort;
extern char EventServerIP[256];
extern int EventServerPort;

extern BOOL gNewServer;
extern BOOL gEventOff;
extern int g_ServerMinUserReset;
extern int g_ServerMaxUserReset;

extern CwsGameServer wsGServer;

extern wsJoinServerCli wsJServerCli;
extern wsJoinServerCli wsDataCli;
extern wsJoinServerCli wsExDbCli;
extern wsJoinServerCli wsRServerCli;
extern wsJoinServerCli wsEvenChipServerCli;

extern CDirPath gDirPath;

extern MapClass MapC[MAX_NUMBER_MAP];

extern CMonsterAttr gMAttr;
extern CMonsterSetBase gMSetBase;
extern classdef DCInfo;
extern CWhisperCash WhisperCash;
extern PartyClass gParty;
extern CDbSave gDbSave;

extern WzUdp gUdpSoc;
extern WzUdp gUdpSocCE;
extern WzUdp gUdpSocCER;
extern WzUdp gUdpSocCRank;
extern WzUdp gUdpSocCRankR;

extern CGuildClass Guild;
extern CMsg lMsg;
extern NSerialCheck gNSerialCheck[OBJMAX];
extern CLoginCount gLCount[3];
extern DWORD  gLevelExperience[MAX_CHAR_LEVEL+1];
extern char szGameServerExeSerial[24];
extern char szServerName[50];
extern char szKorItemTextFileName[256];
extern char szKorSkillTextFileName[256];
extern char szItemTextFileName[256];
extern char szSkillTextFileName[256];
extern char szQuestTextFileName[256];
extern char szMoveReqFileName[256];
extern char szCommonlocIniFileName[256];
extern char szAuthKey[20];
extern BOOL gIsDropDarkLordItem;
extern int  gSleeveOfLordDropRate;
extern int  gSleeveOfLordDropLevel;
extern int  gSoulOfDarkHorseDropRate;
extern int  gSoulOfDarkHorseropLevel;
extern int  gSoulOfDarkSpiritDropRate;
extern int  gSoulOfDarkSpiritDropLevel;
extern float gDarkSpiritAddExperience;
extern BOOL gIsDropGemOfDefend;
extern int  gGemOfDefendDropRate;
extern int  gGemOfDefendDropLevel;
extern int  g_iUseCharacterAutoRecuperationSystem;
extern int  g_iCharacterRecuperationMaxLevel;
extern int g_iServerGroupGuildChatting;
extern int g_iServerGroupUnionChatting;
extern BOOL g_bRibbonBoxEvent;
extern int g_iRedRibbonBoxDropLevelMin;
extern int g_iRedRibbonBoxDropLevelMax;
extern int g_iRedRibbonBoxDropRate;
extern int g_iRedRibbonBoxDropZenRate;
extern int g_iRedRibbonBoxDropZen;
extern int g_iGreenRibbonBoxDropLevelMin;
extern int g_iGreenRibbonBoxDropLevelMax;
extern int g_iGreenRibbonBoxDropRate;
extern int g_iGreenRibbonBoxDropZenRate;
extern int g_iGreenRibbonBoxDropZen;
extern int g_iBlueRibbonBoxDropLevelMin;
extern int g_iBlueRibbonBoxDropLevelMax;
extern int g_iBlueRibbonBoxDropRate;
extern int g_iBlueRibbonBoxDropZenRate;
extern int g_iBlueRibbonBoxDropZen;

extern BOOL g_bNewYearLuckyBagMonsterEventOn;

extern BOOL g_bChocolateBoxEvent;
extern int g_iPinkChocolateBoxDropLevelMin;
extern int g_iPinkChocolateBoxDropLevelMax;
extern int g_iPinkChocolateBoxDropRate;
extern int g_iPinkChocolateBoxDropZenRate;
extern int g_iPinkChocolateBoxDropZen;
extern int g_iRedChocolateBoxDropLevelMin;
extern int g_iRedChocolateBoxDropLevelMax;
extern int g_iRedChocolateBoxDropRate;
extern int g_iRedChocolateBoxDropZenRate;
extern int g_iRedChocolateBoxDropZen;
extern int g_iBlueChocolateBoxDropLevelMin;
extern int g_iBlueChocolateBoxDropLevelMax;
extern int g_iBlueChocolateBoxDropRate;
extern int g_iBlueChocolateBoxDropZenRate;
extern int g_iBlueChocolateBoxDropZen;
extern BOOL g_bCandyBoxEvent;
extern int g_iLightPurpleCandyBoxDropLevelMin;
extern int g_iLightPurpleCandyBoxDropLevelMax;
extern int g_iLightPurpleCandyBoxDropRate;
extern int g_iLightPurpleCandyBoxDropZenRate;
extern int g_iLightPurpleCandyBoxDropZen;
extern int g_iVermilionCandyBoxDropLevelMin;
extern int g_iVermilionCandyBoxDropLevelMax;
extern int g_iVermilionCandyBoxDropRate;
extern int g_iVermilionCandyBoxDropZenRate;
extern int g_iVermilionCandyBoxDropZen;
extern int g_iDeepBlueCandyBoxDropLevelMin;
extern int g_iDeepBlueCandyBoxDropLevelMax;
extern int g_iDeepBlueCandyBoxDropRate;
extern int g_iDeepBlueCandyBoxDropZenRate;
extern int g_iDeepBlueCandyBoxDropZen;
extern BOOL g_bFenrirStuffItemDrop;
extern int g_iFenrirStuff_01_DropLv_Min;
extern int g_iFenrirStuff_01_DropLv_Max;
extern int g_iFenrirStuff_01_DropMap;
extern int g_iFenrirStuff_01_DropRate;
extern int g_iFenrirStuff_02_DropLv_Min;
extern int g_iFenrirStuff_02_DropLv_Max;
extern int g_iFenrirStuff_02_DropMap;
extern int g_iFenrirStuff_02_DropRate;
extern int g_iFenrirStuff_03_DropLv_Min;
extern int g_iFenrirStuff_03_DropLv_Max;
extern int g_iFenrirStuff_03_DropMap;
extern int g_iFenrirStuff_03_DropRate;
extern int g_iFenrirRepairRate;
extern int g_iFenrirDefaultMaxDurSmall;
extern int g_iFenrirElfMaxDurSmall;
extern int g_iFenrir_01Level_MixRate;
extern int g_iFenrir_02Level_MixRate;
extern int g_iFenrir_03Level_MixRate;
extern BOOL g_bCrywolfMonsterDarkElfItemDrop;
extern int g_iCrywolfMonsterDarkElfItemDropRate;
extern int g_iCrywolfMonsterDarkElfDropZenRate;
extern int g_iCrywolfMonsterDarkElfDropZen;
extern BOOL g_bCrywolfBossMonsterItemDrop;
extern int g_iCrywolfBossMonsterItemDropRate;
extern int g_iCrywolfBossMonsterDropZenRate;
extern int g_iCrywolfBossMonsterDropZen;
extern int g_iCrywolfApplyMvpBenefit;
extern int g_iCrywolfApplyMvpPenalty;
extern int g_iSkillDistanceCheck;
extern int g_iSkillDistanceCheckTemp;
extern int g_iSkillDistanceKick;
extern int g_iSkillDistanceKickCount;
extern int g_iSkillDiatanceKickCheckTime;

//#if(_GSCS==0)
extern BOOL g_bKanturuMayaHandItemDrop;
extern int g_iKanturuMayaHandItemDropRate;
extern int g_iKanturuMayaHandDropZenRate;
extern int g_iKanturuMayaHandDropZen;
extern BOOL g_bKanturuNightmareItemDrop;
extern int g_iKanturuNightmareItemDropRate;
extern int g_iKanturuNightmareDropZenRate;
extern int g_iKanturuNightmareDropZen;
extern BOOL g_bKanturuSpecialItemDropOn;
extern int g_iKanturuMoonStoneDropRate;
extern int g_iKanturuJewelOfHarmonyDropRate;
//#endif

extern BOOL	g_bRaklionSelupanItemDrop;
extern int	g_bRaklionSelupanItemCount;
extern int	g_iRaklionSelupanItemDropRate;
extern int	g_iRaklionSelupanDropZenRate;
extern BOOL	g_bRaklionSelupanDropZen;
extern int	g_iBlockRaklionMapEnter;

extern BOOL	g_bLuckyCoinEventOn;
extern int	g_iLuckyCoinDropRate;

extern int g_nMysteriousPaperDropRate;

extern BOOL g_bHallowinDayEventOn;
extern int g_iHallowinDayEventItemDropRate;
extern int g_iHallowinDayEventJOLBlessDropRate;
extern int g_iHallowinDayEventJOLAngerDropRaTe;
extern int g_iHallowinDayEventJOLScreamDropRate;
extern int g_iHallowinDayEventJOLFoodDropRate;
extern int g_iHallowinDayEventJOLDrinkDropRate;
extern int g_iHallowinDayEventJOLPolymorphRingDropRate;

//Season2.5 add-on
extern int g_iSantaPolymorphRingDropOn;
extern int g_iSantaPolymorphRingDropRate;
extern int g_iCondorFlameDropRate;

//Season3 add-on
extern BOOL g_bCherryBlossomEventOn;
extern int g_iCherryBlossomEventItemDropRate;
extern int g_iML_OldScrollDropRate;
extern int g_iML_CovenantOfIllusionDropRate;
extern int g_iML_AngelKingsPaperDropRate;
extern int g_iML_BloodBoneDropRate;
extern int g_iML_EyesOfDevilSquareDropRate;
extern int g_iML_KeysOfDevilSquareDropRate;
extern int g_ShadowPahtomMaxLevel;

extern int g_bAbilityDebug; //For GMMNG

//Season 4.5 addon
extern int g_iXMasVisitCount;
extern int g_iXMasEvent_LuckNumber1st;
extern int g_iXMasEvent_LuckNumber2nd;

//Season 4.6 addon
extern int g_iNewPVPSystem;
extern int g_bUseGambleSystem;
extern int g_iGambleSystemMoney;

//Customs
extern int g_iPostReqLevel;
extern int g_iPostCost;
extern int g_bPostFloodProtect;
extern int g_bPostFloodProtectTime;
extern BYTE g_PostChatColor;

extern int g_iUseAddPointsReqLevel;
extern int g_iAddPointsCommandCost;
extern int g_iMaxAddPoints;

extern int g_bUseSkinCommand;
extern int g_iUseSkinReqLevel;
extern int g_iSkinCommandCost;

extern int gGuildAllianceMinUsers;

extern long	gMainExeSize;

extern int g_iMaxCharStats;

//Plus item Mix
extern int gChaos10MoneyNeed;
extern int gChaos11MoneyNeed;
extern int gChaos12MoneyNeed;
extern int gChaos13MoneyNeed;
extern int gChaos14MoneyNeed;
extern int gChaos15MoneyNeed;
extern int gChaosNormalMix10SuccessRate;
extern int gChaos380AncExcMix10SuccessRate;
extern int gChaosSocketMix10SuccessRate;
extern int gChaosNormalMix11SuccessRate;
extern int gChaos380AncExcMix11SuccessRate;
extern int gChaosSocketMix11SuccessRate;
extern int gChaosNormalMix12SuccessRate;
extern int gChaos380AncExcMix12SuccessRate;
extern int gChaosSocketMix12SuccessRate;
extern int gChaosNormalMix13SuccessRate;
extern int gChaos380AncExcMix13SuccessRate;
extern int gChaosSocketMix13SuccessRate;
extern int gChaosNormalMix14SuccessRate;
extern int gChaos380AncExcMix14SuccessRate;
extern int gChaosSocketMix14SuccessRate;
extern int gChaosNormalMix15SuccessRate;
extern int gChaos380AncExcMix15SuccessRate;
extern int gChaosSocketMix15SuccessRate;
extern int gChaosMaxSuccessRate;
extern int gChaosLuckOptRateAdd;

extern int gAllowExcellentAncient;
extern int gAllowExcellentSocket;
extern int gAllowJOHonAncient;
extern int gParty3ExpPercent;
extern int gParty4ExpPercent;
extern int gParty5ExpPercent;
extern int gPartyExpPercentOther;

extern int gParty2ExpSetPercent;
extern int gParty3ExpSetPercent;
extern int gParty4ExpSetPercent;
extern int gParty5ExpSetPercent;
extern int gParty1ExpSetPercent;

extern int gItemsDurationTime;
extern int gHackToolPacketPerSecond;
extern int gItemsDurationTime;
extern int gHackToolPacketPerSecond;
extern int gSoulSuccessRate;
extern int gLifeSuccessRate;
extern int gBlessSuccessRate;

extern int gGoldenArcherNeedRenaForPrize;
extern int gMainCheckSumCheck;
extern char gMainAdminIPAddRess[20];

extern int gKnightSkillAddLifeInceaseFormulaPercentNormal;
extern int gKnightSkillAddLifeDecreaseFormulaPercentNormal;
extern int gKnightSkillAddLifeInceaseFormulaPercentParty;
extern int gKnightSkillAddLifeDecreaseFormulaPercentParty;
extern int gKnightSkillAddLifeInceaseTimeDurationPercent;
extern int gKnightSkillAddLifeDecreaseTimeDurationPercent;

//SoulBarier
extern int gWizardMagicDefenseInceaseFormulaPercent;
extern int gWizardMagicDefenseDecreaseFormulaPercent;
extern int gWizardMagicDefenseInceaseTimeDurationPercent;
extern int gWizardMagicDefenseDecreaseTimeDurationPercent;


extern int gSkillAddCriticalDamageInceaseFormulaPercent;
extern int gSkillAddCriticalDamageDecreaseFormulaPercent;
extern int gSkillAddCriticalDamageInceaseTimeDurationPercent;
extern int gSkillAddCriticalDamageDecreaseTimeDurationPercent;


extern int gSkillDamageReflectionInceaseFormulaPercent;
extern int gSkillDamageReflectionDecreaseFormulaPercent;

extern int gSkillDamageReflectionInceaseTimeDurationPercent;
extern int gSkillDamageReflectionDecreaseTimeDurationPercent;

extern int gBerserkSkillAttackDamageIncreaseFormulaPercent;
extern int gBerserkSkillAttackDamageDecreaseFormulaPercent;

extern int	gMonsterAttackRateIncrease;
extern int	gMonsterDamageMinIncrease;
extern int	gMonsterDamageMaxIncrease;
extern int	gMonsterDefenseIncrease;
extern int	gMonsterLifeIncrease;

extern char gGuardMessage[255];
extern char gWelcomeMessage[255];
extern int gIncreaseDarkSpiritAttackDamagePercent;
extern int gIncreaseDarkSpiritDefencePercent;
extern int gDecreaseDarkSpiritExpGivePercent;
extern int gIncreaseDarkHorseAttackDamagePercent;
extern int gIncreaseDarkHorseDefencePercent;
extern int gDecreaseDarkHorseExpGivePercent;
extern DWORD dwgCheckSum[MAX_CHECKSUM_KEY];
extern char connectserverip[20];
extern int  connectserverport;
extern short gGameServerCode;
extern int  gPkTime;
extern BOOL g_bCastleGuildDestoyLimit;
extern DWORD  gItemNumberCount;
extern BOOL gStalkProtocol;
extern DWORD  gAttackEventRegenTime;
extern int  gYear;
extern BOOL gOnlyFireCrackerEffectUse;
extern int  g_iRingOrcKillGiftRate; //F8
extern int  g_iRingDropGiftRate; //F4
extern int  g_iXMasAttackEventDropRate; //F0
extern int  g_iXMasAttackEventDropZen; //EC
extern CSimpleModulus g_SimpleModulusCS;
extern CSimpleModulus g_SimpleModulusSC;
extern int  gEledoradoGoldGoblenItemDropRate;
extern int  gEledoradoTitanItemDropRate;
extern int  gEledoradoGoldDerconItemDropRate;
extern int  gEledoradoDevilLizardKingItemDropRate;
extern int  gEledoradoDevilTantarosItemDropRate;
extern int  gEledoradoGoldGoblenExItemDropRate;
extern int  gEledoradoTitanExItemDropRate;
extern int  gEledoradoGoldDerconExItemDropRate;
extern int  gEledoradoDevilLizardKingExItemDropRate;
extern int  gEledoradoDevilTantarosExItemDropRate;
extern int gEledoradoGoldenRabbitRegenTime;
extern int gEledoradoGoldenDarkKnightRegenTime;
extern int gEledoradoGoldenDevilRegenTime;
extern int gEledoradoGoldenMonsterRegenTime;
extern int gEledoradoGoldenCrustRegenTime;
extern int gEledoradoGoldenSatirosRegenTime;
extern int gEledoradoGoldenTwintailRegenTime;
extern int gEledoradoGoldenIronKnightRegenTime;
extern int gEledoradoGoldenNeipinRegenTime;
extern int gEledoradoGoldenGreatDragonRegenTime;
extern int  giKundunRefillHPSec;
extern int  giKundunRefillHP;
extern int  giKundunRefillHPTime;
extern int  giKundunHPLogSaveTime;
extern BOOL gUseNPGGChecksum;
extern int g_ShieldSystemOn;
extern int g_iDamageDevideToSDRate;
extern int g_iDamageDevideToHPRate;
extern float g_fSuccessAttackRateOption;
extern int g_iSDChargingOption;
extern int g_iConstNumberOfShieldPoint;
extern int g_ShieldAutoRefillOn;
extern int g_ShieldAutoRefillOnSafeZone;
extern int g_PKLevelIncreaseOff;
extern int g_CompoundPotionDropOn;
extern int g_iCompoundPotionLv1DropRate;
extern int g_iCompoundPotionLv2DropRate;
extern int g_iCompoundPotionLv3DropRate;
extern int g_iCompoundPotionLv1DropLevel;
extern int g_iCompoundPotionLv2DropLevel;
extern int g_iCompoundPotionLv3DropLevel;
extern BOOL g_bShieldComboMissOptionOn;
extern int g_iShieldPotionLv1MixSuccessRate;
extern int g_iShieldPotionLv1MixMoney;
extern int g_iShieldPotionLv2MixSuccessRate;
extern int g_iShieldPotionLv2MixMoney;
extern int g_iShieldPotionLv3MixSuccessRate;
extern int g_iShieldPotionLv3MixMoney;
extern int g_iShieldGageConstA;
extern int g_iShieldGageConstB;
extern BOOL g_bCheckSpeedHack;
extern char gMapName[MAX_NUMBER_MAP][255];
extern char g_szMapName[MAX_NUMBER_MAP][32];
extern char szGameServerVersion[12];
extern char szClientVersion[8];
extern BOOL bCanTrade;
extern BOOL bCanChaosBox;
extern BOOL bCanWarehouseLock;
extern int  MapMinUserLevel[MAX_NUMBER_MAP];
extern BOOL gEnableBattleSoccer;
extern int  gLootingTime;
extern int  gPkItemDrop;
extern int  gItemDropPer;
extern int gExcItemDropRate;
extern int  gEvent1ItemDropTodayMax;
extern int  gEvent1ItemDropTodayPercent;
extern char gCountryName[20];
extern int  gCharacterDeleteMinLevel;
extern BOOL gCreateCharacter;
extern short gCreateMGLevel;
extern short gCreateDLLevel;
extern short gCreateSUMLevel;
extern short gCreateMONKLevel;
extern BOOL gGuildCreate;
extern BOOL gGuildDestroy;
extern int  gGuildCreateLevel;
extern int  gGuildCreateMoney;
extern BOOL gItemSerialCheck;
extern float  gAddExperience;
extern float  gAddZen;
extern float gAddZenDiv;
extern int  g_XMasEvent_StarOfXMasDropRate;
extern int  g_XMasEvent_ItemDropRateForStarOfXMas;
extern int  gFireCrackerDropRate;
extern int  g_ItemDropRateForgFireCracker;
extern int  gHeartOfLoveDropRate;
extern int  g_ItemDropRateForgHeartOfLove;
extern int  gGoldMedalDropRate;
extern int  gSilverMedalDropRate;
extern int  g_ItemDropRateForGoldMedal;
extern int  g_ItemDropRateForSilverMedal;
extern int  gBoxOfGoldDropRate;
extern int  g_ItemDropRateForBoxOfGold;
extern int  g_EventChipDropRateForBoxOfGold;
extern int  gEyesOfDevilSquareDropRate;
extern int  gKeyOfDevilSquareDropRate;
extern int  gDQChaosSuccessRateLevel1;
extern int  gDQChaosSuccessRateLevel2;
extern int  gDQChaosSuccessRateLevel3;
extern int  gDQChaosSuccessRateLevel4;
extern int  gDQChaosSuccessRateLevel5;
extern int  gDQChaosSuccessRateLevel6;
extern int  gDQChaosSuccessRateLevel7;
extern BOOL g_bBloodCastle;
extern int  g_iBloodCastle_StartHour;
extern int  g_iStoneDropRate;
extern int	g_iBloodCastle_Prize;
extern int  g_iBloodCastle_OddEvenHour;
extern int  g_iAngelKingsPaperDropRate;
extern int  g_iBloodBoneDropRate;
extern BOOL g_bChaosCastle;
extern DWORD  gAttackSpeedTimeLimit;
extern DWORD  gHackCheckCount;
extern float gDecTimePerAttackSpeed;
extern int  gMinimumAttackSpeedTime;
extern int  gDetectedHackKickCount;
extern int gTamaJJangKeepTime;
extern int gTamaJJangDisappearTime;
extern int gTamaJJangDisappearTimeRandomRange;
extern BOOL gItemDropRingOfTransform;
extern int  gQuestNPCTeleportTime;
extern DWORD  gEledoradoGoldGoblenRegenTime;
extern DWORD  gEledoradoTitanRegenTime;
extern DWORD  gEledoradoGoldDerconRegenTime;
extern DWORD  gEledoradoDevilLizardKingRegenTime;
extern DWORD  gEledoradoDevilTantarosRegenTime;
extern int  gZenDurationTime;
extern int gMonsterHPAdjust;
extern BOOL gEnableCheckPenetrationSkill;
extern int g_MaintainUserConnectionSecond;
extern bool g_EnableSelfDefense;
extern BYTE g_MaxCherryBranchWhite;
extern BYTE g_MaxCherryBranchRed;
extern BYTE g_MaxCherryBranchGold;
extern WORD g_MaxPartyLevelDiff;
extern int g_LuckyItemDurabilityTime;
extern bool g_LuckyItemTrade;
extern int g_MaxStat;

//#if (_GSCS==1)
extern CCastleSiege g_CastleSiege;
extern CCastleDeepEvent g_CastleDeepEvent;
extern int g_iCastleItemMixLimit;
extern int gIsDropSetItemInCastleHuntZone;
extern int gSetItemInCastleHuntZoneDropRate;
extern int gSetItemInCastleHuntZoneDropLevel;
extern BOOL g_bDoCastleDeepEvent;
//#endif

#ifdef __ALIEN__
extern int g_ComboAttackPower;
extern bool g_SuperPanda;
extern DWORD g_ConnectMemberUpdate;
extern bool g_HarmonyOptionOnSocket;
extern int g_GuildCreateReset;
extern DWORD g_PotionDelay;
extern int g_PandaExpPerc;
extern int g_SkeletonExpPerc;
extern int g_SkeletonRingExpPerc;
#endif

extern bool gBloodCastleAllowingPlayers;
extern bool gDevilSquareAllowingPlayers;
extern bool gChaosCastleAllowingPlayers;

extern bool gSellHarmonyItemShop;

void gSetDate();
BOOL gJoomin15Check(char* szJN);
BOOL gJoominCheck(char* szJN, int iLimitAge);
void GameMainInit(HWND hWnd);
int GetWarehouseUsedHowMuch(int UserLevel, int MasterLevel, BOOL IsLock);
void GraphPaint(HWND hWnd);
void GameMonsterAllAdd();
void GameMonsterAllCloseAndReLoad();
void GameMainFree();
BOOL GMJoinServerConnect(char* ConnectServer, DWORD wMsg);
BOOL GMRankingServerConnect(char* RankingServer, DWORD wMsg);
BOOL GMEventChipServerConnect(char* ServerIP, DWORD wMsg);
BOOL GMDataServerConnect(char* ConnectServer, WPARAM wMsg);
BOOL ExDataServerConnect(char* ConnectServer, DWORD wMsg);
BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg);
void GMServerMsgProc( WPARAM wParam, LPARAM lParam);
void GMClientMsgProc( WPARAM wParam, LPARAM lParam);
void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMRankingClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMEventChipClientMsgProc(WPARAM wParam, LPARAM lParam);
void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void ReadServerInfo();
void ReadCommonServerInfo();
void GameServerInfoSendStop();
void GameServerInfoSendStart();
void GameServerInfoSend();
void CheckSumFileLoad(char * szCheckSum);
void LoadItemBag();
void SetMapName();
int GetEventFlag();
void ReadEventInfo(MU_EVENT_TYPE eEventType);
void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType);
void SetEventOff();
#endif