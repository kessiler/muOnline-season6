#pragma once
// -------------------------------------------------------------------------------

struct PMSG_REQ_VIEW_EC_MN
{
	PBMSG_HEAD h;
	// ----
	int		iINDEX;
	char	szUID[11];
};
// -------------------------------------------------------------------------------

struct C_PMSG_BCTALKOPEN
{
	PBMSG_HEAD2 h;
	// ----
	short	Level;
	BYTE	ThirdChangeUp;
};
// -------------------------------------------------------------------------------

struct PMSG_ANS_USE_WEAPON_INTERFACE
{
	PBMSG_HEAD2 h;
	// ----
	BYTE	btResult;
	BYTE	btWeaponType;
	BYTE	btObjIndexH;
	BYTE	btObjIndexL;
};
// -------------------------------------------------------------------------------

struct PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE 
{
	PBMSG_HEAD2 h;
	// ----
	BYTE	btResult;
	BYTE	btUsable;
	int		iCurrentPrice;
	int		iMaxPrice;
	int		iUnitOfPrice;
};
// -------------------------------------------------------------------------------

struct PMSG_ANS_DIG_NPC_CLICK // 1.01.00
{
	PBMSG_HEAD2 h;
	WORD	wNPCIndex;
	DWORD	dwContributePoint;
};
// -------------------------------------------------------------------------------

bool NpcTalk(LPOBJ lpNpc, LPOBJ lpObj);
// ----
bool NpcMainatenceMachine(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcReira(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcBattleAnnouncer(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcFasi(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcGuildMasterTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcRolensiaGuard(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcChaosGoblelin(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcChaosCardMaster(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcRusipher(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcPosionGirl(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcDeviasWizard(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcDeviasGuard(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcDeviasWareHousemen(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcNoriaRara(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcDeviasMadam(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcEventChipNPC(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcRorensiaSmith(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcNoriJangIn(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcQuestCheck(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcServerDivision(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcRoadMerchant(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcAngelMessanger(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcRolensiaMadam(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcDeviasWeapon(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcDarkSpiritTrainer(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcJewelMixDealer(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcSiegeMachine_Attack(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcSiegeMachine_Defense(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcElderCircle(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcCastleGuard(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcCastleGateLever(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcCastleCrown(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcCastleSwitch(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcCastleHuntZoneGuard(LPOBJ lpNpc, LPOBJ lpObj);	// -> _CS
bool NpcShadowPhantom(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcShadowPhantom(int iObjIndex);
bool NpcCrywolfAltar(LPOBJ lpNpc, LPOBJ lpObj);			// -> _CS
bool NpcMainatenceMachine(LPOBJ lpNpc, LPOBJ lpObj);	// -> _CS
bool NpcQuartermasterPamella(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcQuartermasterAngela(LPOBJ lpNpc, LPOBJ lpObj);
bool NPcFireCrackerStore(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcIllusionHolyRelicsStatue(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcHolyRelicsBoxAllied(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcHolyRelicsBoxIllusion(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcPlatformMirage(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcSilviaTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcRheaTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcMarceTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcSeedMaster(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcSeedResearcher(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcCherryTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcLuckyCoinDelgado(LPOBJ lpNpc, LPOBJ lpObj);
bool Npc_SantaTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool Npc_SnowManTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool Npc_LittleSantaTalk(LPOBJ lpNpc, LPOBJ lpObj, int BuffEffect);
bool Npc_EventDungeonTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool Npc_LugardTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool Npc_MiddleTreasureTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool Npc_LastTreasureTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool Npc_Dialog(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcKristenTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcRaulTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcJuliaTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcRehina(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcVolo(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcDabide(LPOBJ lpNpc, LPOBJ lpObj);

//Temp
bool NpcGateKeeperTitus(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcChristine(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcJewelerRaul(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcMarketJulia(LPOBJ lpNpc, LPOBJ lpObj);

/*int NpcBattleAnnouncer(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcFasi(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcGuildMasterTalk(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRolensiaGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcChaosGoblelin(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRusipher(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcPosionGirl(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasWizard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasWareHousemen(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcNoriaRara(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasMadam(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcEventChipNPC(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRorensiaSmith(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcNoriJangIn(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcQuestCheck(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcServerDivision(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRoadMerchant(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcAngelMessanger(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRolensiaMadam(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasWeapon(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDarkSpiritTrainer(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcJewelMixDealer(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcSiegeMachine_Attack(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcSiegeMachine_Defense(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcElderCircle(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleGateLever(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleCrown(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleSwitch(LPOBJ lpNpc, LPOBJ lpObj);

//#if(_GSCS==1)
BOOL NpcCastleHuntZoneGuard(LPOBJ lpNpc, LPOBJ lpObj);
//#endif

BOOL NpcShadowPhantom(LPOBJ lpNpc, LPOBJ lpObj);

//#if(_GSCS==1)
BOOL NpcCrywolfAltar(LPOBJ lpNpc, LPOBJ lpObj);
//#endif

//#if(_GSCS==0)
BOOL NpcMainatenceMachine(LPOBJ lpNpc, LPOBJ lpObj);
//#endif

BOOL NpcReira(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcChaosCardMaster(LPOBJ lpNpc, LPOBJ lpObj);

//NPCS New
BOOL NpcQuartermasterPamella(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcQuartermasterAngela(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NPcFireCrackerStore(LPOBJ lpNpc, LPOBJ lpObj);

//Illusion Temple
BOOL NpcIllusionHolyRelicsStatue(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcHolyRelicsBoxAllied(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcHolyRelicsBoxIllusion(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcPlatformMirage(LPOBJ lpNpc, LPOBJ lpObj);

//Season3
BOOL NpcSilviaTalk(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRheaTalk(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcMarceTalk(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCherryTalk(LPOBJ lpNpc, LPOBJ lpObj);

//Season 4
BOOL NpcSeedMaster(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcSeedResearcher(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcLuckyCoinDelgado(LPOBJ lpNpc, LPOBJ lpObj);
BOOL Npc_SantaTalk(LPOBJ lpNpc, LPOBJ lpObj);
BOOL Npc_SnowManTalk(LPOBJ lpNpc, LPOBJ lpObj);
BOOL Npc_LittleSantaTalk(LPOBJ lpNpc, LPOBJ lpObj,int BuffEffect);

//Season 4.6
BOOL NpcGateKeeperTitus(LPOBJ lpNpc, LPOBJ lpObj);

//Season 5 Episode 2 JPN
BOOL NpcMarketJulia(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcChristine(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcJewelerRaul(LPOBJ lpNpc, LPOBJ lpObj);

//Zeon Custom
BOOL NpcArenaHelper(LPOBJ lpNpc, LPOBJ lpObj);*/