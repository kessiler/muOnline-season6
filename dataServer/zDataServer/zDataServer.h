#pragma once

#include "resource.h"

#include "wsJoinServer.h"
#include "MuNameCheck.h"
#include "GSDBSet.h"
#include "CharDBSet.h"
#include "AccountCharDBSet.h"
#include "CastleDBSet.h"
#include "CharPreviewDBSet.h"
#include "CrywolfDBSet.h"
#include "ItemExtDbSet.h"
#include "MasterLevelSystemDBSet.h"
#include "PetDBSet.h"
#include "QuestMonKillDBSet.h"
#include "WarehouseDBSet.h"
#include "ZenEvenDb.h"
#include "OptionDbSet.h"
#include "GensDBSet.h"
#include "GameShopDBSet.h"
#include "classdef.h"
#include "PeriodItemExDBSet.h"
#include "PeriodItemDBSet.h"
#include "QuestExpDBSet.h"

extern char szKorSkillTextFileName[];
extern char szKorItemTextFileName[];
extern char szSkillTextFileName[];
extern char szItemTextFileName[];

extern int gLanguage;

extern unsigned __int32 gItemCount;

extern CwsJoinServer wsjServer;

extern CMuNameCheck gMuName;

extern COptionDbSet gOptionDbSet;
extern CItemExtDbSet gItemExtDbSet;
extern CQuery GuildMemberQry;
extern CWarehouseDBSet CWhDBSet;
extern CZenEvenDb CZenEvenDBSet;
extern CCharPreviewDBSet gCharPreDBSet;
extern CPeriodItemExDBSet PeriodItemExDBSet;
extern CPeriodItemDBSet PeriodItemDBSet;
extern LuckyItemDBSet g_LuckyItemDBSet;
extern CMasterLevelSystemDBSet MasterLevelSystemDBSet;
extern CQuestMonKillDBSet QuestMonKillDBSet;
extern CCrywolfDBSet CrywolfDBSet;
extern CCastleDBSet CastleDBSet;
extern CPetDBSet PetDBSet;
extern CCharDBSet gCharDbSet;
extern CAccountCharDBSet gACDbSet;

extern CGSDBSet gGSDbSet;
extern classdef DCInfo;

extern GensDBSet gGensDBSet;
extern GameShopDBSet gGameShopDB;
extern CQuestExpDBSet g_QuestExpDBSet;

void Init();