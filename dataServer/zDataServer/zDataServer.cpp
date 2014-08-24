#include "stdafx.h"
#include "zDataServer.h"
#include "resource.h"
#include "Window.h"
#include "ServerPrc.h"
#include "WZIPCheck.h"
#include "BadSyntaxChk.h"
#include "MapServerManager_DS.h"

char szKorSkillTextFileName[256];
char szKorItemTextFileName[256];
char szSkillTextFileName[256];
char szItemTextFileName[256];

int gLanguage;

unsigned __int32 gItemCount;

CwsJoinServer wsjServer;

CMuNameCheck gMuName;

COptionDbSet gOptionDbSet;
CItemExtDbSet gItemExtDbSet;
CQuery GuildMemberQry;
CWarehouseDBSet CWhDBSet;
CZenEvenDb CZenEvenDBSet;
CCharPreviewDBSet gCharPreDBSet;
CPeriodItemExDBSet PeriodItemExDBSet;
CPeriodItemDBSet PeriodItemDBSet;
LuckyItemDBSet g_LuckyItemDBSet;
CMasterLevelSystemDBSet MasterLevelSystemDBSet;
CQuestMonKillDBSet QuestMonKillDBSet;
CCrywolfDBSet CrywolfDBSet;
CCastleDBSet CastleDBSet;
CPetDBSet PetDBSet;
CCharDBSet gCharDbSet;
CAccountCharDBSet gACDbSet;
CGSDBSet gGSDbSet;
classdef DCInfo;
GameShopDBSet gGameShopDB;
GensDBSet gGensDBSet;
CQuestExpDBSet g_QuestExpDBSet;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	gWindow.InitInstance(hInstance);
	return 0;
}

void Init()
{
#ifdef VM_PROTECT
	VMBEGIN
#endif
		bool Return;
	char szText[256];
	// ----
	DCInfo.SetMabubBanjiOption(true);
	// ----
	gCharDbSet.SetSP_WZ_CreateCharacterGetVersion( GetPrivateProfileInt("DataServerInfo", "CreateCharacterSP_Ver", 1, "..\\data\\dataserver.ini") );

	gLanguage = GetPrivateProfileInt("GameServerInfo", "Language", 0, "..\\data\\commonserver.cfg");

	strcpy(szKorItemTextFileName, "..\\Data\\Item\\Item.txt");
	strcpy(szKorSkillTextFileName, "..\\Data\\Skill\\Skill.txt");
	strcpy(szItemTextFileName, "..\\Data\\Item\\Item.txt");
	strcpy(szSkillTextFileName, "..\\\Data\\Skill\\Skill.txt");
	strcat(szText, "(Kor)");

	if(OpenItemScript(szKorItemTextFileName) == FALSE)
	{
		//MsgBox("Item FIle Not Found. (파일이 존재하지 않습니다.) %s", szKorItemTextFileName);
	}

	if(gLanguage == 0)
	{
		if(gMuName.NameLoad(szItemTextFileName) == FALSE)
		{
			//("Item File Not Found.");
		}
	}
	else if(gMuName.NameLoad(szKorItemTextFileName) == FALSE)
	{
		//MsgBox("Item File Not Found.");
	}

	if(gMuName.NameLoad("..\\Data\\Monster\\Monster.txt") == FALSE)
	{
		//MsgBox("Monster File Not Found.");
	}

	if(gLanguage == 0)
	{
		if(gMuName.NameLoad(szSkillTextFileName) == FALSE)
		{
			//MsgBox("Skill File Not Found.");
		}
	}
	else if(gMuName.NameLoad(szKorSkillTextFileName) == FALSE)
	{
		//MsgBox("Skill File Not Found.");
	}

	DCInfo.Init();

	if(cBadStrChk.Load("BadSyntax.txt") == FALSE)
	{
		//MsgBox("badstring load error");
	}

	if(g_MapServerManager.LoadMapData("..\\Data\\Other\\MapServerInfo.dat") == FALSE)
	{
		//MsgBox("Map Server Data load error");
	}

	gSObjInit();

	//DbSet
	if(gGSDbSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(gACDbSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(gCharDbSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(gCharPreDBSet.Conenect() == FALSE)
	{
		Return = FALSE;
	}

	CZenEvenDBSet.Connect();

	if(CWhDBSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(gItemExtDbSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(gOptionDbSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(g_QuestExpDBSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(gGSDbSet.CheckMuDBIdentification() == FALSE)
	{
		//		MsgBox("☆ Unknow MuDB Id.");
	}

	gItemCount = gGSDbSet.GetCount();

	//DbSet New
	if(PetDBSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(CastleDBSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(CrywolfDBSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(QuestMonKillDBSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(MasterLevelSystemDBSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if(PeriodItemDBSet.Connect() == FALSE
		|| PeriodItemExDBSet.Connect() == FALSE
		|| g_LuckyItemDBSet.Connect() == FALSE)
	{
		Return = FALSE;
	}

	if( !gGensDBSet.Connect() || !gGameShopDB.Connect() )
	{
		Return = false;
	}

	g_WZIPCheck.Init();

	gWindow.PrintLine(0);
	gWindow.PrintLog("DataServer is started [%d]", gSettings.ServerPort);
	gWindow.PrintLine(0);

	CreateGIocp(gSettings.ServerPort);
	//-> Maybe for send invite to GS?
	wsjServer.MyWinsockBase::CreateSocket();
	wsjServer.CreateServer(NULL, gSettings.ServerPort+1, 2026, 0);
#ifdef VM_PROTECT
	VMEND
#endif
}