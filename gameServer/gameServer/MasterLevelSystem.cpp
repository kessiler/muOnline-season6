//-> Decompiled by DarkSim | 18.02.2013 | 1.01.00 GS-N (All names are original)
// -------------------------------------------------------------------------------

#include "StdAfx.h"
#include "MasterLevelSystem.h"
#include "MasterLevelSkillTreeSystem.h"
#include "logproc.h"
#include "GameMain.h"
#include "..\common\winutil.h"
#include "DBSockMng.h"
#include "ObjCalCharacter.h"
#include "DSProtocol.h"
#include "GensSystem.h"
#include "..\include\ReadScript.h"
// -------------------------------------------------------------------------------

#pragma message("##### REMOVE ME #####")
int g_bIsSpeedServer;		//-> extern from GameMain.cpp (0b0415c8)
int g_MLBattleZoneAddExp;	//-> extern from GameMain.cpp (0b0415dc)
// -------------------------------------------------------------------------------

CMasterLevelSystem g_MasterLevelSystem;
// -------------------------------------------------------------------------------

//00553160	-> 100%
CMasterLevelSystem::CMasterLevelSystem()
{
	// ----
}
// -------------------------------------------------------------------------------

//005531e0	-> 100%
CMasterLevelSystem::~CMasterLevelSystem()
{
	// ----
}
// -------------------------------------------------------------------------------

//00553210	-> 100%
void CMasterLevelSystem::LoadData()	//OK
{
	this->m_UseMonsterList = GetPrivateProfileInt("Common", "UseMonsterList", true, gDirPath.GetNewPath("MasterSystem.cfg"));
	// ----
	if( this->m_UseMonsterList )
	{
		this->ReadMonsterList(gDirPath.GetNewPath("MasterSystemMonster.txt"));
	}
	// ----
	this->m_iMinMonsterKillLevel = GetPrivateProfileInt("Common", "MonsterMinLevel", 110, gDirPath.GetNewPath("MasterSystem.cfg"));
	char szTemp[10] = { 0 };
	GetPrivateProfileString("Common", "AddExperience", "1", szTemp, 5, gDirPath.GetNewPath("MasterSystem.cfg"));
	this->m_fAddExpRate = atof(szTemp);
	// ----
/*	if( g_bIsSpeedServer )	//-> New
	{
		char defaultExp[10] = "";
		_gcvt(3.0, 4, defaultExp);
		GetPrivateProfileString("GameServerInfo", "SpeedS_ML_AddExperience", defaultExp, szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
		this->m_fAddExpRate  = atof(szTemp);
	}*/
	// ----
	LogAddTD("MasterLevel Info Set MinMonsterKillLevel:%d, AddExpRate:%d", this->m_iMinMonsterKillLevel,this->m_fAddExpRate);
}
// -------------------------------------------------------------------------------

//Custom
void CMasterLevelSystem::ReadMonsterList(char * File)
{
	ZeroMemory(this->m_MonsterList, sizeof(this->m_MonsterList));
	int Token;
	int	LoadedCount = 0;
	// ----
	SMDFile = fopen(File, "r");
	// ----
	if( SMDFile == 0 )
	{
		MsgBox(lMsg.Get(MSGGET(0, 112)), File);
		return;
	}
	// ----
	while(true) 
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		else
		{
			this->m_MonsterList[LoadedCount] = TokenNumber;
			LoadedCount++;
		}
	}
	// ----
	LogAddTD("[MasterSystem] [%d] Monsters loaded from list", LoadedCount);
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

//00553380	-> 100%
void CMasterLevelSystem::SetMasterLevelExpTlb()	//OK
{
	this->m_i64MasterLevelExpTlb[0] = 0;
	__int64 nTotalLevel				= 0;
	__int64 nTotalLevel_Over		= 0;
	__int64 i64Exp					= 0;
	// ----
	for( int iLevel = 1; iLevel < MASTER_MAX_LEVEL + 1; iLevel++ )
	{
		nTotalLevel			= iLevel + MAX_CHAR_LEVEL;
		i64Exp				= (nTotalLevel + 9) * nTotalLevel * nTotalLevel * 10;
		nTotalLevel_Over	= nTotalLevel - 255;
		i64Exp				+= (nTotalLevel_Over + 9) * nTotalLevel_Over * nTotalLevel_Over * 1000;
		i64Exp				= (i64Exp - 3892250000) / 2;
		// ----
		this->m_i64MasterLevelExpTlb[iLevel] = i64Exp;
	}
	// ----
	LogAddTD("Master level system exp table setting is complete");
}
// -------------------------------------------------------------------------------

//00553540	-> 100%
void CMasterLevelSystem::gObjNextMLExpCal(LPOBJ lpObj)	//OK
{
	int nNextML						= lpObj->m_nMasterLevel + 1;
	lpObj->m_i64NextMasterLevelExp	= this->m_i64MasterLevelExpTlb[nNextML];
}
// -------------------------------------------------------------------------------

//005535a0	-> 100%
int	CMasterLevelSystem::MasterLevelUp(LPOBJ lpObj, __int64 iAddExp, bool bEventMapReward, int iMonsterType)	//OK
{
	if( !this->IsMasterLevelUser(lpObj) )
	{
		return false;
	}
	// ----
	if( lpObj->m_nMasterLevel >= MASTER_MAX_LEVEL )
	{
		GCServerMsgStringSend(lMsg.Get(1136), lpObj->m_Index, 1);
		return false;
	}
	// ----
	if( bEventMapReward )	//-> New
	{
		iAddExp = iAddExp;	//???
	}
	else
	{
#ifdef GENS
		if( gGensSystem.IsMapBattleZone(lpObj->MapNumber) )	//-> Original g_GensSystem maybe
		{
			iAddExp = iAddExp * (this->m_fAddExpRate + g_MLBattleZoneAddExp);
		}
		else
		{
			iAddExp = iAddExp * this->m_fAddExpRate;
		}
#else
		iAddExp = iAddExp * this->m_fAddExpRate;
#endif
	}
	// ----
	if( lpObj->m_MPSkillOpt.MpsPlusExp > 0 )
	{
		iAddExp += iAddExp * lpObj->m_MPSkillOpt.MpsPlusExp / 100;
	}
	// ----
	if( iAddExp > 0 )
	{
		gObjSetExpPetItem(lpObj->m_Index, iAddExp);
		// ----
		LogAddTD("ML Experience : Map[%d]-(%d,%d) [%s][%s](%d) %I64d %I64d MonsterIndex : %d",	//-> Updated
          lpObj->MapNumber, lpObj->X, lpObj->Y, lpObj->AccountID,lpObj->Name,
          lpObj->m_nMasterLevel, lpObj->m_i64MasterLevelExp, iAddExp, iMonsterType);
		// ----
		if( iAddExp + lpObj->m_i64MasterLevelExp >= lpObj->m_i64NextMasterLevelExp )
		{
			iAddExp = 0;
			// -----
			lpObj->m_i64MasterLevelExp	= lpObj->m_i64NextMasterLevelExp;
			lpObj->m_nMasterLevel++;
			lpObj->m_iMasterLevelPoint++;	//-> In future can use it for change ml point per level)
			// -----
			gObjCalCharacter(lpObj->m_Index);
			// -----
			lpObj->MaxLife				+= DCInfo.DefClass[ lpObj->Class ].LevelLife;
			lpObj->MaxMana				+= DCInfo.DefClass[ lpObj->Class ].LevelMana;
			lpObj->Life					= lpObj->AddLife + lpObj->MaxLife;
			lpObj->Mana					= lpObj->AddMana + lpObj->MaxMana;
			// -----
			gObjCalcShieldPoint(lpObj);
			lpObj->iShield				= lpObj->iAddShield + lpObj->iMaxShield;
			// -----
			this->gObjNextMLExpCal(lpObj);
			// ----
			gObjCalcMaxLifePower(lpObj->m_Index);
			gObjSetBP(lpObj->m_Index);
			GJSetCharacterInfo(lpObj, lpObj->m_Index, 0, 0);
			this->GCMasterLevelUpInfo(lpObj);
			GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
			GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);	//-> Original name ->_BP
		}
		else
		{
			lpObj->m_i64MasterLevelExp += iAddExp;
		}
		// ----
		GCSendExp_INT64(lpObj->m_Index, 65535, iAddExp, 0, 0);		
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//00553a90	-> 100%
int CMasterLevelSystem::IsMasterLevelUser(LPOBJ lpObj)	//
{
	if( lpObj == NULL )
	{
		return false;
	}
	// ----
	if( !lpObj->m_bMasterLevelDBLoad )
	{
		return false;
	}
	// ----
	if( lpObj->Level >= MAX_CHAR_LEVEL && lpObj->ThirdChangeUp == 1 )
	{
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//00553b00	-> 100%
int CMasterLevelSystem::CheckMLGetExp(LPOBJ lpObj, LPOBJ lpTargetObj)	//OK
{
	if( !this->IsMasterLevelUser(lpObj) )
	{
		return true;
	}
	// ----
	if( this->m_UseMonsterList )
	{
		for( int i = 0; i < MASTER_MAX_LIST; i++ )
		{
			if( this->m_MonsterList[i] == lpTargetObj->Class )
			{
				return true;
			}
		}
	}
	else
	{
		if( lpTargetObj->Level >= this->m_iMinMonsterKillLevel )
		{
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

//00553b60	-> 100%
__int64 CMasterLevelSystem::GetMasterLevelExpTlbInfo(int iMasterLevel)	//OK
{
	return m_i64MasterLevelExpTlb[iMasterLevel];
}
// -------------------------------------------------------------------------------

//00553b90	-> 100%
int CMasterLevelSystem::GetDieDecExpRate(LPOBJ lpObj)	//OK
{
	if( !this->IsMasterLevelUser(lpObj) )
	{
		return -1;
	}
	// ----
	int iRetValue = 0;
	// ----
	if( lpObj->m_PK_Level <= 3 )
	{
		iRetValue = 7;
	}
	else if( lpObj->m_PK_Level == 4 )
	{
		iRetValue = 20;
	}
	else if( lpObj->m_PK_Level == 5 )
	{
		iRetValue = 30;
	}
	else if( lpObj->m_PK_Level >= 6 )
	{
		iRetValue = 40;
	}
	// ----
	return iRetValue;
}
// -------------------------------------------------------------------------------

//00553c50	-> 100%
int CMasterLevelSystem::GetDieDecZenRate(LPOBJ lpObj)	//OK
{
	if( !this->IsMasterLevelUser(lpObj) )
	{
		return -1;
	}
	// ----
	return MASTER_ZEN_DEC;
}
// -------------------------------------------------------------------------------

//00553c90	-> 100%
void CMasterLevelSystem::GDReqMasterLevelInfo(LPOBJ lpObj)	//OK
{
	if( lpObj->m_bMasterLevelDBLoad )
	{
		return;
	}
	// ----
	MLP_REQ_MASTERLEVEL_INFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x30, sizeof(MLP_REQ_MASTERLEVEL_INFO));
	// ----
	pMsg.iUserIndex = lpObj->m_Index;
    memcpy(pMsg.szCharName, lpObj->Name, MAX_IDSTRING);
    pMsg.szCharName[MAX_IDSTRING] = 0;
	// ----
    cDBSMng.Send((char*)&pMsg, pMsg.h.size);
	// ----
    LogAddTD("[%s][%s] Request master level info to DB", lpObj->AccountID, lpObj->Name);
}
// -------------------------------------------------------------------------------

//00553d40	-> 100%
void CMasterLevelSystem::DGAnsMasterLevelInfo(BYTE * aRecv)	//OK
{
	if( !aRecv )
	{
		return;
	}
	// ----
	MLP_ANS_MASTERLEVEL_INFO * lpRecvMsg = (MLP_ANS_MASTERLEVEL_INFO*)aRecv;
	// ----
	if( !gObjIsConnectedGP(lpRecvMsg->iUserIndex) )
	{
		return;
	}
	// ----
	int iIndex	= lpRecvMsg->iUserIndex;
	LPOBJ lpObj	= &gObj[lpRecvMsg->iUserIndex];
	// ---
	if( lpObj->m_bMasterLevelDBLoad )
	{
		return;
	}
	// ----
	if( !lpRecvMsg->btResult )
	{
		LogAddTD("[%s][%s] MasterLevel Info Load Fail", lpObj->AccountID, lpObj->Name);	//-> New
		return;
	}
	// ----
	if( lpRecvMsg->btResult == 1 )
	{
		if( lpObj->m_iMasterLevelPoint + lpRecvMsg->nMLPoint != lpRecvMsg->nMLevel )	//-> New (be good use MASTER_MAX_POINT for check)
		{
			LogAddTD("[%s][%s] MasterLevel Info Mismatch!! - Point(%d), Use Point(%d), Level(%d)",
				lpObj->AccountID, lpObj->Name,
				lpRecvMsg->nMLPoint, lpObj->m_bMasterLevelDBLoad, lpRecvMsg->nMLevel);
		}
		// ----
		lpObj->m_bMasterLevelDBLoad = 1;
		// ----
		if( lpRecvMsg->nMLevel == 0 && lpRecvMsg->i64NextMLExp == 0 )
		{
			lpObj->m_nMasterLevel			= lpRecvMsg->nMLevel;
			lpObj->m_i64MasterLevelExp		= lpRecvMsg->i64MLExp;
			lpObj->m_i64NextMasterLevelExp	= m_i64MasterLevelExpTlb[1];
			lpObj->m_iMasterLevelPoint		= lpRecvMsg->nMLPoint;
			// ----
			LogAddTD("[%s][%s] MasterLevel Info First Set [MLevel:%d][MLExp:%I64d][m_i64NextMasterLevelExp:%I64d][MLPoint:%d]",
				lpObj->AccountID, lpObj->Name, lpObj->m_nMasterLevel, 
				lpObj->m_i64MasterLevelExp, lpObj->m_i64NextMasterLevelExp, lpObj->m_iMasterLevelPoint);
			// ----
			this->GDReqMasterLevelInfoSave(lpObj);
		}
		else
		{
			lpObj->m_nMasterLevel			= lpRecvMsg->nMLevel;
			lpObj->m_i64MasterLevelExp		= lpRecvMsg->i64MLExp;
			lpObj->m_i64NextMasterLevelExp	= lpRecvMsg->i64NextMLExp;
			lpObj->m_iMasterLevelPoint		= lpRecvMsg->nMLPoint;
			// ----
			LogAddTD("[%s][%s] Recv MasterLevel Info [MLevel:%d][MLExp:%I64d][m_i64NextMasterLevelExp:%I64d][MLPoint:%d]", 
				lpObj->AccountID, lpObj->Name, lpObj->m_nMasterLevel, 
				lpObj->m_i64MasterLevelExp, lpObj->m_i64NextMasterLevelExp, lpObj->m_iMasterLevelPoint);
			// ----
			this->GDReqMasterLevelInfoSave(lpObj);
		}
		// ----
		lpObj->MaxLife = DCInfo.DefClass[lpObj->Class].Life + (lpObj->Level + lpObj->m_nMasterLevel - 1) * DCInfo.DefClass[lpObj->Class].LevelLife  + ((lpObj->Vitality - DCInfo.DefClass[lpObj->Class].Vitality ) * DCInfo.DefClass[lpObj->Class].VitalityToLife);
		// ----
		if( lpObj->Life > lpObj->MaxLife + lpObj->AddLife )
		{
			lpObj->Life = lpObj->MaxLife;
		}
		// ----
		lpObj->MaxMana = DCInfo.DefClass[lpObj->Class].Mana + (lpObj->Level + lpObj->m_nMasterLevel - 1) * DCInfo.DefClass[lpObj->Class].LevelMana  + ((lpObj->Energy - DCInfo.DefClass[lpObj->Class].Energy ) * DCInfo.DefClass[lpObj->Class].EnergyToMana);
		// ----
		if( lpObj->Mana > lpObj->MaxMana + lpObj->AddMana )
		{
			lpObj->Mana = lpObj->MaxMana;
		}
		// ----
		gObjCalcMaxLifePower(lpObj->m_Index);
		gObjSetBP(lpObj->m_Index);
		gObjCalcShieldPoint(lpObj);
		// ----
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
		// ----
		LogAddTD("[%s][%s] Reset Max Value For MasterLevel [MaxLife:%d][MaxMana:%d][MaxSD:%d]", lpObj->AccountID, lpObj->Name, lpObj->MaxLife, lpObj->MaxMana, lpObj->iShield);
		// ----
		this->GCMasterLevelInfo(lpObj);
		// ----
		GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
		GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
	}
	// ----
	gObjCalCharacter(lpObj->m_Index);
	// -----
	gObjCalcMLSkillItemOption(lpObj);
	g_MasterSkillSystem.CGReqGetMasterLevelSkillTree(lpObj->m_Index);
}
// -------------------------------------------------------------------------------

//005543f0	-> 100%
void CMasterLevelSystem::GDReqMasterLevelInfoSave(LPOBJ lpObj)	//OK
{
	if( !lpObj->m_bMasterLevelDBLoad )
	{
		return;
	}
	// ----
	MLP_REQ_MASTERLEVEL_INFOSAVE pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0x31, sizeof(MLP_REQ_MASTERLEVEL_INFOSAVE));
	// ----
	memcpy(pMsg.szCharName, lpObj->Name, MAX_IDSTRING);
	pMsg.szCharName[MAX_IDSTRING] = 0;
	pMsg.nMLevel		= lpObj->m_nMasterLevel;
	pMsg.i64MLExp		= lpObj->m_i64MasterLevelExp;
	pMsg.i64NextMLExp	= lpObj->m_i64NextMasterLevelExp;
	pMsg.nMLPoint		= LOWORD(lpObj->m_iMasterLevelPoint);
	// ----
	cDBSMng.Send((char*)&pMsg, pMsg.h.size);
	// ----
	LogAddTD("[%s][%s] MasterLevel Info Save [MLevel:%d][MLExp:%I64d][MLNextExp:%I64d][MLPoint:%d]",
		lpObj->AccountID, lpObj->Name, lpObj->m_nMasterLevel, 
		LODWORD(lpObj->m_i64MasterLevelExp), HIDWORD(lpObj->m_i64MasterLevelExp), 
		LODWORD(lpObj->m_i64NextMasterLevelExp), HIDWORD(lpObj->m_i64NextMasterLevelExp),
		lpObj->m_iMasterLevelPoint);
}
// -------------------------------------------------------------------------------

//00554540	-> 100%
void CMasterLevelSystem::GCMasterLevelInfo(LPOBJ lpObj)	//OK
{
	if( !lpObj->m_bMasterLevelDBLoad )
	{
		this->GDReqMasterLevelInfo(lpObj);
		return;
	}
	// ----
	PMSG_MASTERLEVEL_INFO pMsg;
	PHeadSubSetB(&pMsg.h.c, 0xF3, 0x50, sizeof(PMSG_MASTERLEVEL_INFO));
	// ----
	pMsg.nMLevel		= lpObj->m_nMasterLevel;
	// ----
	pMsg.btMLExp1		= SET_NUMBERH(SET_NUMBERHW(HIDWORD(lpObj->m_i64MasterLevelExp)));
	pMsg.btMLExp2		= SET_NUMBERL(SET_NUMBERHW(HIDWORD(lpObj->m_i64MasterLevelExp)));
	pMsg.btMLExp3		= SET_NUMBERH(SET_NUMBERLW(HIDWORD(lpObj->m_i64MasterLevelExp)));
	pMsg.btMLExp4		= SET_NUMBERL(SET_NUMBERLW(HIDWORD(lpObj->m_i64MasterLevelExp)));
	pMsg.btMLExp5		= SET_NUMBERH(SET_NUMBERHW(LODWORD(lpObj->m_i64MasterLevelExp)));
	pMsg.btMLExp6		= SET_NUMBERL(SET_NUMBERHW(LODWORD(lpObj->m_i64MasterLevelExp)));
	pMsg.btMLExp7		= SET_NUMBERH(SET_NUMBERLW(LODWORD(lpObj->m_i64MasterLevelExp)));
	pMsg.btMLExp8		= SET_NUMBERL(SET_NUMBERLW(LODWORD(lpObj->m_i64MasterLevelExp)));
	// ----
	pMsg.btMLNextExp1	= SET_NUMBERH(SET_NUMBERHW(HIDWORD(lpObj->m_i64NextMasterLevelExp)));
	pMsg.btMLNextExp2	= SET_NUMBERL(SET_NUMBERHW(HIDWORD(lpObj->m_i64NextMasterLevelExp)));
	pMsg.btMLNextExp3	= SET_NUMBERH(SET_NUMBERLW(HIDWORD(lpObj->m_i64NextMasterLevelExp)));
	pMsg.btMLNextExp4	= SET_NUMBERL(SET_NUMBERLW(HIDWORD(lpObj->m_i64NextMasterLevelExp)));
	pMsg.btMLNextExp5	= SET_NUMBERH(SET_NUMBERHW(LODWORD(lpObj->m_i64NextMasterLevelExp)));
	pMsg.btMLNextExp6	= SET_NUMBERL(SET_NUMBERHW(LODWORD(lpObj->m_i64NextMasterLevelExp)));
	pMsg.btMLNextExp7	= SET_NUMBERH(SET_NUMBERLW(LODWORD(lpObj->m_i64NextMasterLevelExp)));
	pMsg.btMLNextExp8	= SET_NUMBERL(SET_NUMBERLW(LODWORD(lpObj->m_i64NextMasterLevelExp)));
	// ----
	pMsg.nMLPoint		= LOWORD(lpObj->m_iMasterLevelPoint);
	pMsg.wMaxLife		= (double)lpObj->AddLife + lpObj->MaxLife;
	pMsg.wMaxMana		= (double)lpObj->AddMana + lpObj->MaxMana;
	pMsg.wMaxShield		= LOWORD(lpObj->iAddShield) + LOWORD(lpObj->iMaxShield);
	pMsg.wMaxBP			= LOWORD(lpObj->AddBP) + LOWORD(lpObj->MaxBP);
	// ----
	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}
// -------------------------------------------------------------------------------

//00554960	-> 100%
void CMasterLevelSystem::GCMasterLevelUpInfo(LPOBJ lpObj)	//OK
{
	PMSG_MASTERLEVEL_UP pMsg;
	PHeadSubSetB(&pMsg.h.c, 0xF3, 0x51, 0x20);
	// ----
	pMsg.nMLevel		= lpObj->m_nMasterLevel;
	pMsg.nAddMLPoint	= 1;
	pMsg.nMLPoint		= lpObj->m_iMasterLevelPoint;
	pMsg.nMaxMLPoint	= MASTER_MAX_POINT;
	pMsg.wMaxLife		= (double)lpObj->AddLife + lpObj->MaxLife;
	pMsg.wMaxMana		= (double)lpObj->AddMana + lpObj->MaxMana;
	pMsg.wMaxShield		= LOWORD(lpObj->iAddShield) + LOWORD(lpObj->iMaxShield);
	pMsg.wMaxBP			= LOWORD(lpObj->AddBP) + LOWORD(lpObj->MaxBP);
	// ----
	DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	GCSendEffectInfo(lpObj->m_Index, 16);
	// ----
	LogAddTD("[%s][%s] Master Level Up :%d, MLPoint:%d/%d", lpObj->AccountID, lpObj->Name, lpObj->m_nMasterLevel, lpObj->m_iMasterLevelPoint, MASTER_MAX_POINT);
}
// -------------------------------------------------------------------------------