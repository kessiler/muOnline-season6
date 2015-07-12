//GameServer 1.00.77 JPN - Completed
//GameServer 1.00.90 JPN - Completed -> All hidden functions and registers fixed *-* but initial .cpp classes(objectstruct, bill etc) wrong position :(
#include "stdafx.h"
#include "user.h"
#include "logproc.h"
#include "GameServer.h"
#include "GameMain.h"
#include "MapServerManager.h"
#include "gObjMonster.h"
#include "ObjCalCharacter.h"
#include "ObjUseSkill.h"
#include "DarkSpirit.h"
#include "DSProtocol.h"
#include "DevilSquare.h"
#include "protocol.h"
#include "AcceptIp.h"
#include "BattleGround.h"
#include "MonsterAttr.h"
#include "MixSystem.h"
#include "PacketChecksum.h"
#include "QuestInfo.h"
#include "EledoradoEvent.h"
#include "TNotice.h"
#include "GMMng.h"
#include "zzzmathlib.h"
#include "Gate.h"
#include "ObjAttack.h"
#include "SProtocol.h"
#include "MultiAttackHAckCheck.h"
#include "BattleSoccer.h"
#include "BattleSoccerManager.h"
#include "TUnion.h"
#include "TMonsterAIElement.h"
#include "..\common\winutil.h"
#include "Weapon.h"
#include "CrywolfSync.h"
#include "Kanturu.h"
#include "CashShop.h"
#include "Mercenary.h"
#include "KalimaGate.h"
#include "KanturuBattleUserMng.h"
#include "Guardian.h"
#include "SProtocol.h"
//#include "GameServerAuth.h"
#include "MoveCommand.h"
#include "ItemAddOption.h"
#include "StatMng.h"
#include "..\common\SetItemOption.h"
#include "SkillAdditionInfo.h"
#include "CashItemPeriodSystem.h"
#include "TMonsterSkillManager.h"

//#if(_GSCS==1)
#include "CrywolfAltar.h"
#include "CrywolfStatue.h"
#include "LifeStone.h"
#include "GuardianStatue.h"
#include "CannonTower.h"
#include "CastleCrown.h"
#include "CastleCrownSwitch.h"
//#endif

#include "QuestUtil.h"
#include "IllusionTempleEvent.h"
#include "BuffEffectSlot.h"
#include "MasterLevelSkillTreeSystem.h"
#include "MasterLevelSystem.h"
#include "Raklion.h"
#include "RaklionUtil.h"
#include "RaklionBattleUserMng.h"
#include "PcBangPointSystem.h"
#include "CastleSiegeSync.h"
#include "TemporaryUserManager.h"

#ifdef NPVP
#include "NewPVP.h"
#endif

#ifdef GENS
#include "GensSystem.h"
#endif

#ifdef IMPERIAL
#include "ImperialGuardian.h"
#endif

#ifdef PERIOD
#include "PeriodItemEx.h"
#endif

#ifdef GAMESHOP
#include "GameShop.h"
#endif

#ifdef DP
#include "Doppelganger.h"
#endif

#ifdef WZQUEST
#include "QuestExpProgMng.h"
#endif

#ifdef __MAKELO__
#include "OfflineAttack.h"
#endif

#ifdef __CUSTOMS__
#include "ResetSystem.h"
#include "PVPZone.h"
#include "ClassCalc.h"
#include "ShopPointEx.h"
#ifdef QUESTSYSTEM
#include "QuestSystem.h"
#endif
#endif

#include "LuckyItem.h"
#include "MUHelper.h"
#include "OfflineTrade.h"
#include "LogToFile.h"

#ifdef OFFEXP
#include "OffExp.h"
#endif

int ChangeCount; 
int lOfsChange;
int GuildUserCount;
int GuildUserOfs;
int GuildInfoCount;
int GuildInfoOfs;
int skillSuccess;
int gCurConnectUser;
int gDisconnect;
int gObjTotalUser;
int gItemLoopMax;
int gItemLoop;
int gItemLoop2;
int gObjCSFlag;

#include "ChaosCastle.h"
#include "..\include\readscript.h"

CViewportGuild ViewGuildMng;

int gObjCount;
int gObjMonCount;
int gObjCallMonCount;

MessageStateMachine gSMMsg[OBJMAX][MAX_MONSTER_SEND_MSG];	
ExMessageStateMachine gSMAttackProcMsg[OBJMAX][MAX_MONSTER_SEND_ATTACK_MSG];

OBJECTSTRUCT*	ObjectStruct	=	NULL; // pointer changed to 0
OBJECTSTRUCT*	gObj;

#pragma init_seg(".CRT$XCB")
OBJECTSTRUCT_CLASS::OBJECTSTRUCT_CLASS() // (0050F940 -> classes related)
{
	ObjectStruct = new	OBJECTSTRUCT[OBJMAX+1]; // memory dynamically allocated
	gObj	=	&ObjectStruct[1]; //Move Data -> Start from 1 (It's like the 0 position doesn't exist)
}

OBJECTSTRUCT_CLASS::~OBJECTSTRUCT_CLASS() // (0050FA10 -> classes related)
{
	delete	[]	ObjectStruct; // memory freed up
	ObjectStruct	=	NULL; // pointer changed to 0
}

OBJECTSTRUCT_CLASS	g_OBJECTSTRUCT_CLASS; //extern

HANDLE hThread_gObjMove;
BYTE gObjMonsterInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int op1=0, int op2=0, int op3=0);

#ifdef PCBANG
CPCBangPointTimer g_PCBangPointTimer;
#endif

struct PWMSG_COUNT
{
	struct PWMSG_HEAD h;
	BYTE count;
};

struct PBMSG_COUNT
{
	struct PBMSG_HEAD h;
	BYTE count;
};

struct PMSG_ITEMVIEWPORTCREATE
{
	BYTE NumberH;
	BYTE NumberL;
	BYTE px;
	BYTE py;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_VIEWPORTCREATE
{
	BYTE NumberH;
	BYTE NumberL;
	BYTE X;
	BYTE Y;
	BYTE CharSet[18];
	char Id[10];
	BYTE TX;
	BYTE TY;
	BYTE DirAndPkLevel;
	BYTE btViewSkillStateCount;
	BYTE btViewSkillState[MAX_STATE_COUNT];
};

struct PMSG_VIEWPORTCREATE_CHANGE
{
	BYTE NumberH;
	BYTE NumberL;
	BYTE X;
	BYTE Y;
	BYTE SkinH;
	BYTE SkinL;
	char Id[10];
	BYTE TX;
	BYTE TY;
	BYTE DirAndPkLevel;
	BYTE CharSet[18]; //Season 2.5 add-on
	BYTE btViewSkillStateCount;
	BYTE btViewSkillState[MAX_STATE_COUNT];
};

struct PMSG_VIEWPORTDESTROY
{
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_MONSTER_VIEWPORTCREATE
{
	BYTE NumberH;
	BYTE NumberL;
	BYTE Type_HI;
	BYTE Type_LO;
	BYTE X;
	BYTE Y;
	BYTE TX;
	BYTE TY;
	BYTE Path;
	BYTE btViewSkillStateCount;
	BYTE btViewSkillState[MAX_STATE_COUNT];
};

struct PMSG_CALLMONSTER_VIEWPORTCREATE
{
	BYTE NumberH;
	BYTE NumberL;
	BYTE Type_HI;
	BYTE Type_LO;
	BYTE X;
	BYTE Y;
	BYTE TX;
	BYTE TY;
	BYTE Path;
	BYTE Id[10];
	BYTE btViewSkillStateCount;
	BYTE btViewSkillState[MAX_STATE_COUNT];
};

struct PMSG_GUILDVIEWPORT_USER
{
	BYTE NumberH;
	BYTE NumberL;
	BYTE GNumberH;
	BYTE GNumberL;
};

struct PMSG_GUILDVIEWPORT
{
	BYTE NumberH;
	BYTE NumberL;
	char GuildName[8];
	BYTE Mark[32];
};

BYTE GuildInfoBuf[10000];
BYTE GuildUserBuf[10000];
BYTE SendGBufChange[5000];

PMSG_VIEWPORTCREATE pViewportCreate;
PMSG_VIEWPORTCREATE_CHANGE pViewportCreateChange;
PMSG_VIEWPORTDESTROY pViewportDestroy;
PMSG_MONSTER_VIEWPORTCREATE pMonsterViewportCreate;
PMSG_CALLMONSTER_VIEWPORTCREATE pCallMonsterViewportCreate;

#pragma pack(1)
PWMSG_COUNT pItemCount;
PMSG_ITEMVIEWPORTCREATE pItemViewportCreate;
PMSG_VIEWPORTDESTROY pItemViewportDestroy;
#pragma pack()

short RoadPathTable[MAX_ROAD_PATH_TABLE] = { -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0 };

//#if(_GSCS==0)
int  gServerMaxUser=1000; //500
//#else
//int  gServerMaxUser=1500;
//#endif

BOOL g_EnergyCheckOff=1;

struct PMSG_EX_SKILL_COUNT
{
	struct PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE Type;
	BYTE Count;
};

#define OBJECT_POINTER(aIndex) ((aIndex * OBJECT_SIZE) + OBJECT_BASE)

void gObjSkillUseProcTime500(LPOBJ lpObj) //
{
	if ( lpObj->SkillHellFire2State != 0 )
	{
		DWORD dwCurrentTick = GetTickCount(); //2
		lpObj->SkillHellFire2Count = (dwCurrentTick - lpObj->SkillHellFire2Time) / 500;

		if ( lpObj->SkillHellFire2Count > 12 )
		{
			lpObj->SkillHellFire2Count = 12;
		}

		if(dwCurrentTick >= (lpObj->SkillHellFire2Time + 6000))
		{
			CMagicInf * lpMagic = gObjGetMagicSearch(lpObj,0x28);

			if(lpMagic != 0)
			{
				gObjUseSkill.RunningSkill(lpObj->m_Index,0,lpMagic,0);
			}
		}
		else
		{
			int mana = MagicDamageC.SkillGetMana(40); //3
			mana = mana * 20 / 100;

			mana -= mana * lpObj->m_MPSkillOpt.MpsDecreaseMana / 100; //season4 add-on

			if(mana < 1) //s4 add-on
			{
				mana = 1;
			}

			BOOL bSuccess = TRUE;

			if ( mana > lpObj->Mana )
			{
				bSuccess = FALSE;
				int count = 0;

				while ( true )
				{
					count++;

					if ( gObjIsConnected(lpObj) == FALSE )
					{
						LogAdd("??[CHECK_LOG_INFINITY] gObjIsConnected() error %s %d", __FILE__, __LINE__);
						break;
					}

					if ( count > 100 )
					{
						LogAdd("??[CHECK_LOG_INFINITY] ( _count > 100 ) error %s %d", __FILE__, __LINE__);
						break;
					}

					int pos = gObjGetManaItemPos(lpObj->m_Index);

					if ( pos == -1 )
					{
						break;
					}

					PMSG_USEITEM pMsg;
					pMsg.inventoryPos = pos;
					pMsg.invenrotyTarget = 0;
					CGUseItemRecv(&pMsg, lpObj->m_Index);

					if ( mana <= lpObj->Mana )
					{
						bSuccess = TRUE;
						break;
					}
				}
			}
			
			if ( bSuccess == FALSE )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, 40);
				
				if (lpMagic != NULL )
				{
					gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic, 0);
				}
			}
			else
			{
				lpObj->Mana -= mana;

				if ( lpObj->Mana < 0.0f )
				{
					lpObj->Mana = 0.0f;
				}

				GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
				PMSG_EX_SKILL_COUNT pMsg;
				PHeadSetB( (LPBYTE)&pMsg, 0xBA, sizeof(pMsg));
				pMsg.Type = 0x28;
				pMsg.Count = lpObj->SkillHellFire2Count;
				pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
				pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

				DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
				MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}

void MonsterAndMsgProc()
{
	LPOBJ lpObj;
	int n;
	int aIndex;

	for ( n=0;n<OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected == PLAYER_PLAYING  )
		{
			if ( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC) 
			{
				if(lpObj->m_iCurrentAI != 0)
				{
					continue;
				}
				gObjMonsterProcess(lpObj);
			}
			else
			{
				gObjSkillUseProcTime500(lpObj);
				gObjMsgProc(lpObj);
				CreateFrustrum(lpObj->X, lpObj->Y, n);
			}

			if ( lpObj->Type == OBJ_USER )
			{
				gDarkSpirit[n].Run();
			}
		}

		else if ( lpObj->Connected >= PLAYER_LOGGED )
		{
			if ( lpObj->Type == OBJ_USER )
			{
				gObjMsgProc(lpObj);

				if ( lpObj->Connected == PLAYER_PLAYING )
				{
					CreateFrustrum(lpObj->X, lpObj->Y, n);
				}
			}
		}
	}

	for ( n=0;n<OBJMAXUSER;n++)
	{
		lpObj = &gObj[n+OBJ_STARTUSERINDEX];

		if ( lpObj->Connected == PLAYER_PLAYING  )
		{
			aIndex = lpObj->m_Index;

			for ( int i=0;i<MAX_MONSTER_SEND_ATTACK_MSG;i++)
			{
				if ( gSMAttackProcMsg[n][i].MsgCode >= 0 )
				{
					if ( GetTickCount() > gSMAttackProcMsg[n][i].MsgTime )
					{
						gObjStateAttackProc(lpObj, gSMAttackProcMsg[n][i].MsgCode, gSMAttackProcMsg[n][i].SendUser,	gSMAttackProcMsg[n][i].SubCode, gSMAttackProcMsg[n][i].SubCode2);
						gSMAttackProcMsg[n][i].MsgCode = -1;
					}
				}
			}
		}
	}
	TMonsterSkillManager::MonsterSkillProc();
}

void MoveMonsterProc()
{
	DWORD MoveTime;
	int DelayTime;
	LPOBJ lpObj;

	for(int n = 0; n < OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if(lpObj->m_iCurrentAI != 0)
		{
			if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
			{
				continue;
			}
		}

		if(lpObj->Connected == PLAYER_PLAYING)
		{
			if(lpObj->Type == OBJ_MONSTER)
			{
//#if(_GSCS==0)
//				if(ATTRIBUTE_RANGE(lpObj->m_Attribute) != FALSE)
//#else
				if( ATTRIBUTE_RANGE(lpObj->m_Attribute) != FALSE ||	lpObj->Class == 277	|| lpObj->Class == 283 || lpObj->Class == 288 || lpObj->Class == 278 || lpObj->Class == 216 || lpObj->Class == 217 || lpObj->Class == 218 || lpObj->Class == 219 || (CRYWOLF_ALTAR_CLASS_RANGE(lpObj->Class) != FALSE || CRYWOLF_STATUE_CHECK(lpObj->Class) != FALSE) )
//#endif
				{
					CreateFrustrum(lpObj->X,lpObj->Y,n);
					continue;
				}
			}
			
			if(lpObj->m_State == 2)
			{
				if(lpObj->PathCount != 0 && (gObjCheckUsedBuffEffect(lpObj, AT_ICE_ARROW) == 0) && (gObjCheckUsedBuffEffect(lpObj, AT_STUN) == 0) && (gObjCheckUsedBuffEffect(lpObj, AT_SLEEP) == 0))
				{
					if(lpObj->DelayLevel != 0)
					{
						DelayTime = 300;
					}
					else
					{
						DelayTime = 0;
					}
	
					if(lpObj->Type == OBJ_MONSTER && lpObj->m_RecallMon >= 100)
					{
						lpObj->m_MoveSpeed = 200;
					}
					else
					{
						lpObj->m_MoveSpeed = 400;
					}
	
					if(lpObj->PathDir[lpObj->PathCur]%2 == 0)
					{
						MoveTime = (lpObj->m_MoveSpeed + DelayTime)*(double)1.3;
					}
					else
					{
						MoveTime = lpObj->m_MoveSpeed + DelayTime;
					}
	
					if( (GetTickCount() - lpObj->PathTime) > MoveTime && lpObj->PathCur < 14)
					{
						int nextX = lpObj->PathX[lpObj->PathCur];
						int nextY = lpObj->PathY[lpObj->PathCur];

						BYTE mapnumber = lpObj->MapNumber;

						if(mapnumber != 0xFF) //hermex add-on for fixing registers
						{
							//TEST LOG
						}

						BYTE attr = MapC[mapnumber].GetAttr(nextX,nextY);
	
						if( lpObj->Type == OBJ_USER && ( (attr & 4) == 4 || (attr & 8) == 8) )
						{
							LogAddTD("[ CHECK POSITION ] MoveMosterProc [%s][%s] Map[%d]-(%d,%d) User(%d,%d) Can not Move Position Attr[%d]", lpObj->AccountID,lpObj->Name,lpObj->MapNumber,nextX,nextY,lpObj->X,lpObj->Y, attr);

							for(n = 0; n < 15;n++)
							{
								lpObj->PathX[n] = 0;
								lpObj->PathY[n] = 0;
								lpObj->PathOri[n] = 0;
							}
	
							lpObj->PathCount = 0;
							lpObj->PathCur = 0;
							lpObj->PathTime = GetTickCount();

							if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
							{
								lpObj->PathStartEnd = 0;
							}
	
							if(lpObj->Type == OBJ_USER)
							{
								gObjSetPosition(lpObj->m_Index,lpObj->X,lpObj->Y);
							}
						}
						else
						{
							lpObj->X = lpObj->PathX[lpObj->PathCur];
							lpObj->Y = lpObj->PathY[lpObj->PathCur];
							lpObj->Dir = lpObj->PathDir[lpObj->PathCur];

							lpObj->PathTime = GetTickCount();
							lpObj->PathCur++;

							if(lpObj->PathCur >= lpObj->PathCount)
							{
								lpObj->PathCur = 0;
								lpObj->PathCount = 0;

								if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
								{
									lpObj->PathStartEnd = 0;
								}
							}
						}
					}
				}
				CreateFrustrum(lpObj->X,lpObj->Y,n);
			}
		}
	}
}

void gObjRefillProcess(int aIndex, LPOBJ lpObj)
{
	/*if( GetTickCount()-lpObj->LifeRefillTimer > 10000 )
	{
		if( (lpObj->Life+2) <= lpObj->MaxLife )
		{
			lpObj->Life += 2;
			GCReFillSend(aIndex, lpObj->Life, 0xff);
			lpObj->LifeRefillTimer = GetTickCount();
		}
	}*/
}

//CRITICAL_SECTION	m_critAddMonster; //1.00.77

void gObjInit()
{
	gObjCount=OBJ_STARTUSERINDEX;
	gObjMonCount=0;
	gObjCallMonCount= OBJ_MAXMONSTER;

	CItem * pTempInventory = new CItem[INVENTORY_SIZE];
	BYTE * pTempInventoryMap = new BYTE[INVENTORY_MAP_SIZE];

	//InitializeCriticalSection(&m_critAddMonster);

	for ( int n = 0; n< OBJMAX ; n++)
	{
		memset(&gObj[n], 0, sizeof(OBJECTSTRUCT));
		gObj[n].Type = (BYTE)OBJ_EMPTY;
		gObj[n].m_socket = INVALID_SOCKET;
		gObj[n].PathTime = GetTickCount();
		gObj[n].m_RecallMon = -1;
		gObj[n].m_lpMagicBack = new CMagicInf[MAGIC_SIZE];
		gObj[n].Magic = new CMagicInf[MAGIC_SIZE];
		
		if ( n >= OBJ_STARTUSERINDEX )
		{
			gObj[n].Inventory1 = new CItem[INVENTORY_SIZE];
			gObj[n].Inventory2 = new CItem[INVENTORY_SIZE];
			gObj[n].InventoryMap1 = new BYTE[INVENTORY_MAP_SIZE];
			gObj[n].InventoryMap2 = new BYTE[INVENTORY_MAP_SIZE];
		}
		else
		{
			gObj[n].Inventory1 = pTempInventory;
			gObj[n].Inventory2 = pTempInventory;
			gObj[n].InventoryMap1 = pTempInventoryMap;
			gObj[n].InventoryMap2 = pTempInventoryMap;
		}

		if ( n >= OBJ_STARTUSERINDEX )
		{
			gObj[n].Trade = new CItem[TRADE_BOX_SIZE];
			gObj[n].TradeMap = new BYTE[TRADE_BOX_MAP_SIZE];
			gObj[n].pWarehouse = new CItem[WAREHOUSE_SIZE];
			gObj[n].pWarehouseMap = new BYTE[WAREHOUSE_SIZE];

#ifdef GAMESHOP
			//ZeroMemory(&gObj[n].GameShop, sizeof(gObj[n].GameShop));
#endif
		}
		
		gObj[n].WarehouseCount = 0;
		gObj[n].pChaosBox = NULL;
		
		if ( n >= OBJ_STARTUSERINDEX )
		{
			gObj[n].PerSocketContext = new _PER_SOCKET_CONTEXT;
		}

		InitializeCriticalSection(&gObj[n].m_critPShopTrade);
		gObj[n].m_bGMSummon = 0; //season4 add-on
		gObj[n].bTemporaryUser = 1;
		gObj[n].m_btDarkSideTargetNum = 0;
#ifdef WZQUEST
		gObj[n].m_pUserQuestInfo = nullptr;
		// ----
		if( n >= OBJ_STARTUSERINDEX )
		{
			gObj[n].m_pUserQuestInfo = new UserQuestInfo[25];
		}
#endif
		for( int i = 0; i < 5; ++i )
		{
			gObj[n].m_wDarkSideTargetList[i] = 10000;
		}
	}


	InitFrustrum();
}

void gObjEnd()
{
	gObjAllDisconnect();
	gObjAllLogOut();

	for ( int n=0; n< OBJMAX; n++)
	{
		if ( gObj[n].m_lpMagicBack != NULL )
		{
			delete [] gObj[n].m_lpMagicBack;
		}

		if ( gObj[n].Magic != NULL )
		{
			delete [] gObj[n].Magic;
		}

		if ( n >= OBJ_STARTUSERINDEX )
		{
			if ( gObj[n].Inventory1 != NULL )
			{
				delete gObj[n].Inventory1;
			}

			if ( gObj[n].Inventory2 != NULL )
			{
				delete gObj[n].Inventory2;
			}

			if ( gObj[n].InventoryMap1 != NULL )
			{
				delete gObj[n].InventoryMap1;
			}

			if ( gObj[n].InventoryMap2 != NULL )
			{
				delete gObj[n].InventoryMap2;
			}

			if ( gObj[n].PerSocketContext != NULL )
			{
				delete gObj[n].PerSocketContext;
			}

			if ( gObj[n].Trade != NULL )
			{
				delete gObj[n].Trade;
			}

			if ( gObj[n].TradeMap != NULL )
			{
				delete gObj[n].TradeMap;
			}

			if ( gObj[n].pWarehouse != NULL )
			{
				delete gObj[n].pWarehouse;
			}

			if ( gObj[n].pWarehouseMap != NULL )
			{
				delete gObj[n].pWarehouseMap;
			}
#ifdef WZQUEST
			delete [] gObj[n].m_pUserQuestInfo;
#endif
		}

		DeleteCriticalSection( &gObj[n].m_critPShopTrade );
	}

	//DeleteCriticalSection(&m_critAddMonster);
}

void gObjClearViewportOfMine(LPOBJ lpObj)
{
	for (int i=0;i<MAX_VIEWPORT;i++)
	{
		lpObj->VpPlayer[i].state =0;
		lpObj->VpPlayer[i].number =-1;
	}

	lpObj->VPCount=0;
}

void gObjClearViewport(LPOBJ lpObj)
{
	for ( int i=0;i<MAX_VIEWPORT;i++)
	{
		lpObj->VpPlayer[i].state=0;
		lpObj->VpPlayer[i].number =-1;
		lpObj->VpPlayer2[i].state=0;
		lpObj->VpPlayer2[i].number=-1;
	}

	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
}

void gObjCloseSet(int aIndex, int Flag)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->CloseCount > 0 )
	{
		return;
	}

	if ( lpObj->Connected == PLAYER_PLAYING )
	{
		
		if ( BC_MAP_RANGE(lpObj->MapNumber) )
		{
			g_BloodCastle.SearchUserDropQuestItem(aIndex);
		}

		if ( IT_MAP_RANGE(lpObj->MapNumber) )
		{
			g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
			g_IllusionTempleEvent.LeaveIllusionTemple(lpObj->m_Index, lpObj->MapNumber);
		}
#ifdef DP
		if( g_DoppleganerEvent.IsEventMap(lpObj->MapNumber) )
		{
			g_DoppleganerEvent.LeaveDoppelgangerEvent(lpObj->m_Index);

			if( g_DoppleganerEvent.GetDoppelgangerState() == 2 )
			{
				g_DoppleganerEvent.SendDoppelgangerResult(lpObj, 1);
			}
		}
#endif

		if( (GetTickCount() - lpObj->MySelfDefenseTime ) < 60000 && g_EnableSelfDefense )
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 109)), lpObj->m_Index, 1);
			GCCloseMsgSend(aIndex,-1); //Season 2.5 add-on
			return;
		}

		if ( (lpObj->m_IfState.use != 0 ) && ( ( lpObj->m_IfState.type == 1 ) || (lpObj->m_IfState.type == 6) || (lpObj->m_IfState.type == 13) || (lpObj->m_IfState.type == 7)  ) ) 
		{
			gObjSaveChaosBoxItemList(lpObj);
			switch ( lpObj->m_IfState.type )
			{
				case 1:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 110)), lpObj->m_Index, 1);
					break;

				case 6:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 111)), lpObj->m_Index, 1);
					break;

				case 7:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 127)), lpObj->m_Index, 1);
					break;

				case 13:
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 229)), lpObj->m_Index, 1);
					break;
			}

			return;
		}
	}

	lpObj->bTemporaryUser = 0;
	lpObj->CloseCount = 6;
	lpObj->CloseType = Flag;
	lpObj->bEnableDelCharacter = 1;
	
}

void gObjCharTradeClear(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	memset(lpObj->TradeMap, 0xFF , TRADE_BOX_SIZE);

	for ( int i=0 ; i< TRADE_BOX_SIZE ; i++)
	{
		lpObj->Trade[i].Clear();
	}

	lpObj->TradeMoney = 0;
	lpObj->TradeOk = false;
}

void gObjCharZeroSet(int aIndex)
{
	int i;
	LPOBJ lpObj = &gObj[aIndex];
	int n;

	memset( lpObj->Name, 0, MAX_ACCOUNT_LEN);
	memset( lpObj->GuildName, 0, sizeof (lpObj->GuildName ));
	memset( lpObj->SubName, 0, sizeof (lpObj->SubName ));
	lpObj->m_Option = 3;
	lpObj->m_iScriptMaxLife = 0;
	lpObj->Level = 0;

	memset( lpObj->BackName,0,sizeof(lpObj->BackName));

	lpObj->m_nMasterLevel = 0;
	lpObj->m_i64MasterLevelExp = 0;
	lpObj->m_i64NextMasterLevelExp = 0;
	lpObj->m_iMasterLevelPoint = 0;
	lpObj->m_bMasterLevelDBLoad = 0;

#ifdef MUHELPER
	lpObj->m_MUHelperOn			= false;
	lpObj->m_MUHelperReadyPay	= false;
	lpObj->m_MUHelperStage		= 1;
	lpObj->m_MUHelperTick		= 0;
#endif

#ifdef GENS
	lpObj->m_GensInfluence = 0;
#endif

	lpObj->Life = 0;
	lpObj->MaxLife = 0;
	lpObj->AddLife = 0;
	lpObj->AddMana = 0;
	lpObj->Live = FALSE;
	lpObj->Dir = 0;
	lpObj->X = 0;
	lpObj->Y = 0;
	lpObj->TX = 0;
	lpObj->TY = 0;
	lpObj->MTX = 0;
	lpObj->MTY = 0;
	lpObj->MapNumber = MAP_INDEX_RORENCIA;
	lpObj->RegenTime = 0;
	lpObj->m_State = 0;
	lpObj->RegenOk = 0;
	lpObj->lpAttackObj = 0;
	lpObj->PartyTargetUser = -1;
	lpObj->m_MoveGateNumber = -1;
	lpObj->CloseCount = -1;
	lpObj->CloseType = -1;
	lpObj->m_IfState.state = -1;
	lpObj->m_IfState.type = -1;
	lpObj->m_IfState.use = 0;
	lpObj->m_InterfaceTime = 0;
	lpObj->m_ActionNumber = 0;
	lpObj->m_State = 0;
	lpObj->m_StateSub = 0;
	lpObj->m_ViewState = 0;
	lpObj->m_Rest = 0;

	//season4 add-on
	lpObj->m_wRaiseTalismanSaveX = 0;
	lpObj->m_wRaiseTalismanSaveY = 0;
	lpObj->m_wRaiseTalismanSaveMap = 0;
	lpObj->m_wTeleportTalismanSaveX = 0;
	lpObj->m_wTeleportTalismanSaveY = 0;
	lpObj->m_wTeleportTalismanSaveMap = 0;

	lpObj->iMaxShield = 0;
	lpObj->iShield = 0;
	lpObj->iAddShield = 0;
	lpObj->iFillShield = 0;
	lpObj->iFillShieldMax = 0;
	lpObj->iFillShieldCount = 0;
	lpObj->dwShieldAutoRefillTimer = 0;
	lpObj->m_wItemEffectType = 0;
	lpObj->m_iItemEffectValidTime = 0;
#ifdef OLDCASHSHOP
	lpObj->m_wCashPoint = 0;
#endif
	lpObj->m_iPeriodItemEffectIndex = -1;

	gObjClearBuffEffect(lpObj, CLEAR_TYPE_LOGOUT);

	lpObj->m_Attribute = 0;

#ifdef OPT
	lpObj->m_iMonsterStunDelay = 0;
	lpObj->m_iMonsterSleepDelay = 0;
#endif

	lpObj->m_iMonsterBattleDelay = 0;
	lpObj->m_cKalimaGateExist = FALSE;
	lpObj->m_iKalimaGateIndex = -1;
	lpObj->m_cKalimaGateEnterCount = 0;
	lpObj->TradeOk = false;
	lpObj->sHDCount = 0;
	lpObj->PathCount = 0;
	lpObj->PathCur = 0;
	lpObj->PathStartEnd = 0;
	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
	lpObj->m_ShopTime = 0;
	lpObj->m_ManaFillCount = 0;
	lpObj->FillLife = 0;
	lpObj->iObjectSecTimer = 0;
	lpObj->m_FriendServerOnline = FALSE;
	lpObj->SetOpAddMaxAttackDamage = 0;
	lpObj->SetOpAddMinAttackDamage = 0;
	lpObj->SetOpAddDamage = 0;
	lpObj->SetOpIncAGValue = 0;
	lpObj->SetOpAddCriticalDamageSuccessRate = 0;
	lpObj->SetOpAddCriticalDamage = 0;
	lpObj->SetOpAddExDamageSuccessRate = 0;
	lpObj->SetOpAddExDamage = 0;
	lpObj->SetOpAddSkillAttack = 0;
	lpObj->AddStrength = 0;
	lpObj->AddDexterity = 0;
	lpObj->AddVitality = 0;
	lpObj->AddEnergy = 0;
	lpObj->SetOpAddAttackDamage = 0;
	lpObj->SetOpAddDefence = 0;
	lpObj->SetOpAddMagicPower = 0;
	lpObj->SetOpAddDefenceRate = 0;
	lpObj->SetOpIgnoreDefense = 0;
	lpObj->SetOpDoubleDamage = 0;
	lpObj->SetOpTwoHandSwordImproveDamage = 0;
	lpObj->SetOpImproveSuccessAttackRate = 0;
	lpObj->SetOpReflectionDamage = 0;
	lpObj->SetOpImproveSheldDefence = 0;
	lpObj->SetOpDecreaseAG = 0;
	lpObj->SetOpImproveItemDropRate = 0;	

	memset( lpObj->m_AddResistance, 0, sizeof(lpObj->m_AddResistance));

	lpObj->IsFullSetItem = false;
	lpObj->DamageMinus = 0;
	lpObj->DamageReflect = 0;
	lpObj->MonsterDieGetMoney = 0;
	lpObj->MonsterDieGetLife = 0;
	lpObj->MonsterDieGetMana = 0;
	lpObj->m_CriticalDamage = 0;
	lpObj->m_ExcelentDamage = 0;
	lpObj->UseMagicNumber = 0;
	lpObj->UseMagicTime = 0;
	lpObj->UseMagicCount = 0;
	lpObj->OSAttackSerial = -1;
#ifdef OPT
	lpObj->SASCount = 0;
#endif
	lpObj->SkillAttackTime = GetTickCount();

	lpObj->m_nSoulBarrierDefence = 0; //S3 Addition
	lpObj->m_nSoulBarrierManaRate = 0; //S3 Addition

	lpObj->m_AttackSpeed = 0;
	lpObj->m_MagicSpeed = 0;
	lpObj->m_TotalAttackTime = GetTickCount();
	lpObj->m_TotalAttackCount = 0;
	lpObj->CheckSumTime = GetTickCount() + 10000;
	lpObj->CheckSumTableNum = -1;
	lpObj->m_SumLastAttackTime = 0;
	lpObj->m_DetectCount = 0;
	lpObj->m_DetectSpeedHackTime = 0;
	lpObj->m_SpeedHackPenalty = 0;
	lpObj->m_DetectedHackKickCount = 0;

	lpObj->m_Curse = 0; //S3 Addition

	lpObj->IsInBattleGround = false;
	lpObj->m_dwPKTimer = GetTickCount();
	lpObj->CheckTickCount = 0;
	lpObj->m_AttackSpeedHackDetectedCount = 0;

	for ( n =0;n<MAX_VIEWPORT ;n++)
	{
		lpObj->VpPlayer[n].state = 0;
		lpObj->VpPlayer2[n].state = 0;
		lpObj->VpPlayer[n].number = -1;
		lpObj->VpPlayer2[n].number = -1;
	}

	lpObj->GuildNumber = 0;
	lpObj->lpGuild = NULL;
	lpObj->PartyNumber = -1;
	lpObj->PartyTargetUser = -1;
	lpObj->m_Drink = 0;
	lpObj->Authority = 0;
	lpObj->Penalty = 0;
	lpObj->m_RecallMon = -1;
	lpObj->m_Change = -1;
	lpObj->GuildStatus = -1;
	lpObj->LastAttackerID = -1;
	lpObj->m_cChaosCastleIndex= -1;
	lpObj->m_cChaosCastleSubIndex = -1;
	lpObj->m_iChaosCastleBlowTime = 0;
	lpObj->m_cKillUserCount = 0;
	lpObj->m_cKillMonsterCount = 0;
	lpObj->m_InWebzen = false;

	memset( lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	for ( i =0; i<MAX_SELF_DEFENSE;i++)
	{
		lpObj->SelfDefense[i] = -1;
		lpObj->SelfDefenseTime[i] = 0;
	}

	lpObj->MySelfDefenseTime = 0;
	lpObj->m_MagicDefense = 0;

	lpObj->m_SkillAttack = 0;
	lpObj->m_SkillAttackTime = 0;

	lpObj->LoadWareHouseInfo = false;

	lpObj->m_SkyBossMonSheildLinkIndex = -1;
	lpObj->m_SkyBossMonSheild = 0;
	lpObj->m_SkyBossMonSheildTime = 0;
	lpObj->m_bAttackerKilled = false;
	lpObj->m_MoveOtherServer = 0;
	lpObj->m_BossGoldDerconMapNumber = -1;
	lpObj->TargetNumber =	0;
	lpObj->TargetShopNumber = 0;
	lpObj->WarehouseMoney = 0;
	lpObj->WarehouseSave = 0;
	lpObj->WarehouseLock = -1;
	lpObj->WarehouseCount = 0;
	lpObj->WarehousePW = 0;
	lpObj->WarehouseUnfailLock = 0;
	lpObj->m_bDevilSquareAuth = false;
	lpObj->m_TimeCount = 0;
#ifdef __CUSTOMS__
	lpObj->m_ShopPointExTime = 0;
#endif
	lpObj->EventChipCount = 0;
	lpObj->MutoNumber = 0;
	lpObj->HaveWeaponInHand = false;
	lpObj->m_ReqWarehouseOpen = 0;
	lpObj->iStoneCount = 0;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_iDuelUser = -1;
	lpObj->m_btDuelScore = 0;
	lpObj->m_iDuelTickCount = 0;
	lpObj->m_MaxLifePower = 0;
#ifdef OPT
	lpObj->m_PacketChecksumTime = 0;
#endif
	lpObj->AddBP = 0;
	lpObj->m_CheckLifeTime = 0;
	memset(lpObj->m_Quest, (BYTE)-1, sizeof(lpObj->m_Quest));
	lpObj->m_SendQuestInfo = 0;
	gPacketCheckSum.ClearCheckSum(lpObj->m_Index);
//	lpObj->NPggCSAuth.Init();
//	lpObj->m_bSentGGAuth = false;
//	lpObj->m_NPggCheckSumSendTime = 0;
	lpObj->PlusStatQuestClear = false;
	lpObj->ComboSkillquestClear = false;
	lpObj->m_LastTeleportTime = 0;
	lpObj->m_ClientHackLogCount = 0;
	lpObj->UseEventServer = FALSE;

	if ( lpObj->Type == OBJ_USER )
	{
		memset(lpObj->pWarehouseMap, (BYTE)-1, WAREHOUSE_SIZE);
		memset(lpObj->TradeMap, (BYTE)-1, TRADE_BOX_SIZE);
	}

	for ( i=0;i<MAX_MAGIC ; i++)
	{
		lpObj->Magic[i].Clear(); //Changed
	}

	lpObj->MagicCount = 0;

	for ( i=0;i<MAX_MONSTER_SEND_MSG;i++)
	{
		gSMMsg[aIndex][i].MsgCode = -1;
	}

	lpObj->m_bIsInMonsterHerd = false;
	lpObj->m_bIsMonsterAttackFirst = true;
	lpObj->m_lpMonsterHerd = NULL;

	if ( (aIndex-OBJ_STARTUSERINDEX) >= 0 )
	{
		for ( i=0;i<MAX_MONSTER_SEND_ATTACK_MSG;i++)
		{
			gSMAttackProcMsg[aIndex-OBJ_STARTUSERINDEX][i].MsgCode = -1;
		}
	}

	lpObj->SkillHellFire2State = 0;
	lpObj->SkillHellFire2Count = 0;
	lpObj->m_PoisonType = 0;
	lpObj->m_IceType = 0;
	lpObj->Leadership = 0;
	lpObj->AddLeadership = 0;
	lpObj->ChatLitmitTime = 0;
	lpObj->ChatLimitTimeSec = 0;
	lpObj->SkillRecallParty_Time = 0;

	lpObj->SkillLongSpearChange = false;
	lpObj->SkillDelay.Init();

	lpObj->bEnableDelCharacter = 1;

	if ( lpObj->Type == OBJ_USER )
	{
		for (i=0;i<WAREHOUSE_SIZE;i++)
		{
			lpObj->pWarehouse[i].Clear();
		}
	}

	::gMultiAttackHackCheck[aIndex].Init();
	lpObj->pTransaction = 0;
	::gObjMonsterHitDamageInit(lpObj);
	::gObjSetInventory1Pointer(&gObj[aIndex]);

	if ( lpObj->Type == OBJ_USER )
	{
		memset(lpObj->pInventoryMap, (BYTE)-1, INVENTORY_MAP_SIZE);
	}

	lpObj->m_bPShopOpen = false;
	lpObj->m_bPShopTransaction = false;
	lpObj->m_bPShopItemChange = false;
	lpObj->m_bPShopRedrawAbs = false;
	memset(lpObj->m_szPShopText, 0, sizeof(lpObj->m_szPShopText));
	lpObj->m_bPShopWantDeal = false;
	lpObj->m_iPShopDealerIndex = -1;
	memset(lpObj->m_szPShopDealerName, 0, sizeof(lpObj->m_szPShopDealerName));
	memset(lpObj->m_iVpPShopPlayer, 0, sizeof(lpObj->m_iVpPShopPlayer));
	lpObj->m_wVpPShopPlayerCount =0;
	lpObj->iGuildUnionTimeStamp = 0;
	::gObjCharTradeClear(lpObj);

	if ( lpObj->Type == OBJ_USER )
	{
		for ( i=0;i<WAREHOUSE_SIZE;i++)
		{
			lpObj->pWarehouse[i].Clear();
		}

		for ( i=0;i<INVENTORY_SIZE;i++)
		{
			lpObj->Inventory1[i].Clear();
		}

		for ( i=0;i<INVENTORY_SIZE;i++)
		{
			lpObj->Inventory2[i].Clear();
		}

		for ( i=0;i<TRADE_BOX_SIZE;i++)
		{
			lpObj->Trade[i].Clear();
		}
	}

	lpObj->bIsChaosMixCompleted = false;
	lpObj->m_bMapSvrMoveQuit = false;

//#if(_GSCS==0)
	lpObj->m_bIsCastleNPCUpgradeCompleted = false;
	lpObj->m_btWeaponState = 0;
//#endif

//#if(_GSCS==1)
	lpObj->m_iCsNpcExistVal = 0;
	lpObj->m_btCsNpcType = 0;
	lpObj->m_btCsGateOpen = 0;
	lpObj->m_iCsGateLeverLinkIndex = -1;
	lpObj->m_btCsNpcDfLevel = 0;
	lpObj->m_btCsNpcRgLevel = 0;
	lpObj->m_btCsJoinSide = 0;
	lpObj->m_bCsGuildInvolved = false;
	lpObj->m_bIsCastleNPCUpgradeCompleted = 0;
	lpObj->m_btWeaponState = 0;
//#endif

	lpObj->m_btKillCount = 0;

	lpObj->m_iAccumulatedDamage = 0;

//#if(_GSCS==1)
	lpObj->m_iCreatedActivationTime = 0;
	lpObj->m_btLifeStoneCount = 0;
//#endif

	lpObj->m_iAccumulatedCrownAccessTime = 0;

	lpObj->m_MonsterSkillElementInfo.Reset();
	lpObj->m_iCrywolfMVPScore = 0;
	lpObj->m_iSkillDistanceErrorCount = 0;
	lpObj->m_dwSkillDistanceErrorTick = 0;
	g_kJewelOfHarmonySystem.InitEffectValue(&lpObj->m_JewelOfHarmonyEffect);
	g_kItemSystemFor380.InitEffectValue(&lpObj->m_ItemOptionExFor380);

//#if(_GSCS==0)
	lpObj->m_bKanturuEntranceByNPC = FALSE;
//#endif

	gItemSocketOption.ClearUserData(lpObj); //season4 add-on

	//Season 2.5 add-on
	lpObj->m_iIllusionTempleIndex = -1;
	lpObj->m_i3rdQuestState = 0;
	lpObj->m_i3rdQuestIndex = -1;

	for ( i =0; i<5;i++)
	{
		lpObj->MonsterKillInfo[i].MonIndex = -1;
		lpObj->MonsterKillInfo[i].KillCount = -1;
	}

	lpObj->m_bSkillKeyRecv = 0; //season 2.5 add-on

	g_MasterSkillSystem.InitPassiveSkillData(lpObj); //Season3 add-on

	//season4 add-on
	lpObj->m_btRefillHPSocketOption = 0;
	lpObj->m_btRefillMPSocketOption = 0;
	lpObj->m_wSocketOptionMonsterDieGetHp = 0;
	lpObj->m_wSocketOptionMonsterDieGetMana = 0;
	lpObj->m_btAGReduceRate = 0;
	lpObj->m_bGMSummon = 0;

	//Season 4.6 addon
	lpObj->m_bUseGambleState = false;
	lpObj->IsOffTrade = 0;
	lpObj->bTemporaryUser = 1;
	//1.01.03
	lpObj->m_btSculptPos = 0;
	lpObj->m_btCharmPos = 0;
	lpObj->m_btArtifactPos = 0;

#ifdef OFFEXP
#ifdef __ALIEN__
	lpObj->DbOffExp = 0;
#endif
	lpObj->OffExp = false;
	lpObj->OffExpTick = 0;
	lpObj->OffExpSpeed = 0;
#endif

#ifdef WZQUEST
	lpObj->m_bUserQuestInfoSent = 0;
	// ----
	if( lpObj->Type == OBJ_USER )
	{
		for(int i = 0; i < 25; ++i )
			lpObj->m_pUserQuestInfo[i].Clear();
	}
#endif

	gGameShop.InitUser(aIndex);

	::gObjClearViewport(&gObj[aIndex]);
}

int gObjGetSocket(SOCKET socket)
{
	for (int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if (gObj[n].Connected != PLAYER_EMPTY)
		{
			if (gObj[n].m_socket == socket)
			{
				return n;
			}
		}
	}
	return -1;
}

void gObjSetTradeOption( int aIndex, int option)
{
	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( option == 0 )
		{
			gObj[aIndex].m_Option = 0;
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 97)), aIndex, 1);
		}
		else
		{
			gObj[aIndex].m_Option |= 1;
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 96)), aIndex, 1);
		}
	}
}

void gObjSetDuelOption(int aIndex, int option)
{
	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( option == 0 )
		{
			gObj[aIndex].m_Option = 0;
		}
		else
		{
			gObj[aIndex].m_Option |= 2;
		}
	}
}

bool IsDuelEnable(int aIndex)
{
	if ( OBJMAX_RANGE(aIndex ) == FALSE)
	{
		return false;
	}

	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( (gObj[aIndex].m_Option&2) == 2 )
		{
			return true;
		}
	}

	return false;
}

bool IsOnDuel(int aIndex1, int aIndex2 )
{
#ifdef NPVP
	if (OBJMAX_RANGE(aIndex1) == FALSE || OBJMAX_RANGE(aIndex2) == FALSE )
	{
		return false;
	}

	if (gObj[aIndex1].Type != OBJ_USER || gObj[aIndex2].Type != OBJ_USER )
	{
		return false;
	}

	if (gObj[aIndex1].m_iDuelUser == aIndex2 && gObj[aIndex2].m_iDuelUser == aIndex1 )
	{
		return true;
	}
	
	return false;
#else
	if ( OBJMAX_RANGE(aIndex1) == FALSE || OBJMAX_RANGE(aIndex2) == FALSE )
	{
		return false;
	}

	if ( gObj[aIndex1].Type != OBJ_USER || gObj[aIndex2].Type != OBJ_USER )
	{
		return false;
	}

	if ( gObj[aIndex1].m_iDuelUser != aIndex2 || gObj[aIndex2].m_iDuelUser != aIndex1 )
	{
		return false;
	}
	
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if(g_DuelRoom.m_Zone[i].btOpen == 0)
		{

			if(g_DuelRoom.m_Zone[i].UserIndex[0] == aIndex1 && 
				g_DuelRoom.m_Zone[i].UserIndex[1] == aIndex2) 
			{
				return true;
			}

			if(g_DuelRoom.m_Zone[i].UserIndex[0] == aIndex2 && 
				g_DuelRoom.m_Zone[i].UserIndex[1] == aIndex1) 
			{
				return true;
			}

		}

	}


	return false;
#endif
}

int GetMapMoveLevel(LPOBJ lpObj, int mapnumber, int max_over)
{
	int overLevel = 0;

	if ( mapnumber > MAX_NUMBER_MAP-1 )
	{
		mapnumber = 0;
		LogAdd("error-L3 : map number not %s %d", __FILE__, __LINE__);
		return 0;
	}

	if ( max_over != 0 )
	{
		overLevel = max_over;
	}
	else
	{
		max_over = MapMinUserLevel[mapnumber];
		overLevel = MapMinUserLevel[mapnumber];
	}
	
	if ( lpObj->Class == CLASS_DARKLORD 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif
		|| lpObj->Class == CLASS_MAGUMSA )
	{
		if ( overLevel > 0 && max_over > 0 )
		{
			overLevel = (max_over/3)*2;
		}
	}

	return overLevel;
}

//0055A740	-> OK
void DbItemSetInByte(LPOBJ lpObj, struct SDHP_DBCHAR_INFORESULT* lpMsg, int ItemDbByte, bool* bAllItemExist)
{
	int n;
	int itype;
	int _type;
	CItem item;
	BYTE OptionData;
	WORD hiWord;
	WORD loWord;
	bool bIsItemExist = true;

	for(n = 0; n < INVENTORY_SIZE;n++)
	{
		bIsItemExist = true;
		lpObj->pInventory[n].Clear();
		itype = lpMsg->dbInventory[n*ItemDbByte];

		if(ItemDbByte >= 16)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+7] & 0x80) == 0x80
				&& (lpMsg->dbInventory[n*ItemDbByte+9] & 0xF0) == 0xF0)
			{
				continue;
			}
		}
		else if(ItemDbByte > 7)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+7] & 0x80) == 0x80)
			{
				continue;
			}
		}
		else
		{
			if(lpMsg->dbInventory[n*ItemDbByte+0] == 0xFF)
			{
				continue;
			}
		}

		if(ItemDbByte == 16)
		{
			itype |= (lpMsg->dbInventory[n*ItemDbByte+9] & 0xF0) * 32;
			itype |= (lpMsg->dbInventory[n*ItemDbByte+7] & 0x80) * 2;
			_type = itype;
		}
		else if(ItemDbByte == 10)
		{
			if((lpMsg->dbInventory[n*ItemDbByte+7] & 0x80) == 0x80)
			{
				itype += 256;
			}

			_type = ((itype / 32) * 512) + itype % 32;
		}
		else
		{
			_type = ((itype / 16) * 512) + itype % 16;
		}

		if(ItemDbByte >= 16)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+7] & 0x80) == 0x80
				&& (lpMsg->dbInventory[n*ItemDbByte+9] & 0xF0) == 0xF0)
			{
				itype = -1;
			}
		}
		else if(ItemDbByte >= 10)
		{
			if(lpMsg->dbInventory[n*ItemDbByte] == 0xFF
				&& (lpMsg->dbInventory[n*ItemDbByte+7] & 0x80) == 0x80)
			{
				itype = -1;
			}
		}
		else
		{
			if(lpMsg->dbInventory[n*ItemDbByte+0] == 0xFF)
			{
				itype = -1;
			}
		}

		if(IsItem(_type) == false)
		{
			bIsItemExist = 0;

			if(bAllItemExist != 0)
			{
				*bAllItemExist = 0;
			}
		}

		if(_type == ITEMGET(14,17) || _type == ITEMGET(14,18))
		{
			if(lpMsg->dbInventory[n*ItemDbByte+2]==0)
			{
				itype = -1;
			}
		}

		if(_type == ITEMGET(14,19))
		{
			if(lpMsg->dbInventory[n*ItemDbByte+2]==(BYTE)-1)
			{
				itype = -1;
			}
		}

		if(_type == ITEMGET(14,100))
		{
			if(lpMsg->dbInventory[n*ItemDbByte+2]==0)
			{
				itype = -1;
			}
		}

		if(itype != -1)
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbInventory[n*ItemDbByte+1]);

			if(lpMsg->DbVersion < 2)
			{
				lpMsg->dbInventory[n*ItemDbByte+8] = -1;
				lpMsg->dbInventory[n*ItemDbByte+9] = -1;
			}

			if(_type != ITEMGET(14,11))
			{
				if(item.m_Level > 15) // [MaxLevel->15] Season 5 Episode 2 JPN
				{
					item.m_Level = 0;
				}
			}

			OptionData = lpMsg->dbInventory[n*ItemDbByte+1];
			item.m_Option1 = ( ((OptionData) >> 7) & 0x01); //HermeX Fix
			item.m_Option2 = DBI_GET_LUCK(OptionData);
			item.m_Option3 = DBI_GET_OPTION(OptionData);

			if(ItemDbByte >= 10)
			{
				#if(GS_LIFE_28_UP == 1)
					item.m_Option3 |= DBI_GET_OPTION16(lpMsg->dbInventory[n*ItemDbByte+DBI_NOPTION_DATA]) >> 4;
				#else
				if(_type == ITEMGET(13,3))
				{
					item.m_Option3 |= DBI_GET_OPTION16(lpMsg->dbInventory[n*ItemDbByte+7]) >> 4;
				}
				else
				{
					if((lpMsg->dbInventory[n*ItemDbByte+7] & 0x40)== 0x40)
					{
						#if(GS_LIFE_28_UP == 1)
						item.m_Option3 = 7;
						#else
						item.m_Option3 = 4;
						#endif
					}
				}
				#endif
			}

			item.m_Durability = lpMsg->dbInventory[n*ItemDbByte+2];

			if(ItemDbByte >= 16)
			{
				item.m_JewelOfHarmonyOption = lpMsg->dbInventory[n*ItemDbByte+10];
			}

			if(ItemDbByte >= 16)
			{
				item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbInventory[n*ItemDbByte+9]);

				if(item.m_ItemOptionEx)
				{
					item.m_Type = itype;

					if(g_kItemSystemFor380.Is380Item(&item)==false)
					{
						item.m_ItemOptionEx = 0;
						LogAddTD("[380Item][%s][%s] Invalid 380 Item Option in Inventory pos[%d]",
							lpObj->AccountID,lpObj->Name,n);
					}
				}
			}

			//SocketSystem Season 4.5 addon start
			BYTE SocketOption[5];
			BYTE SocketOptionIndex = 0xFF;
			memset(&SocketOption, 0xFF, 5);

			if ( gItemSocketOption.IsEnableSocketItem(_type) == 1 )
			{
				for(int i = 0; i < 5; i++)
				{
					SocketOption[i] =  lpMsg->dbInventory[((n*ItemDbByte)+11)+i];
				}

				SocketOptionIndex = lpMsg->dbInventory[n*ItemDbByte+10];
			}
			else
			{
				for(int i = 0; i < 5; i++)
				{
					SocketOption[i] = 0xFF;
				}

				SocketOptionIndex = 0;
			}
			//SocketSystem Season 4.5 addon end
#ifdef PERIOD
			if( ItemDbByte >= 16 )
			{
				item.m_PeriodItemOption = (lpMsg->dbInventory[n*ItemDbByte+DBI_OPTION380_DATA] & 6) >> 1;
			}
#endif
			if(ItemDbByte == 7)
			{
				item.Convert(itype,item.m_Option1,item.m_Option2,item.m_Option3,0,0,0,NULL, 0xFF, 0, 3); //season4 changed
			}
			else if(ItemDbByte <= 10)
			{
				item.Convert(itype,item.m_Option1,item.m_Option2,item.m_Option3,DBI_GET_NOPTION(lpMsg->dbInventory[n*ItemDbByte+7]),lpMsg->dbInventory[n*ItemDbByte+8],item.m_ItemOptionEx,SocketOption, SocketOptionIndex, item.m_PeriodItemOption, 1); //season4 changed
			}
			else
			{
				item.Convert(itype,item.m_Option1,item.m_Option2,item.m_Option3,DBI_GET_NOPTION(lpMsg->dbInventory[n*ItemDbByte+7]),lpMsg->dbInventory[n*ItemDbByte+8],item.m_ItemOptionEx,SocketOption, SocketOptionIndex, item.m_PeriodItemOption, 3); //season4 changed
			}

			if(_type == ITEMGET(14,7))
			{

			}
			else
			{
				if(_type >= ITEMGET(14,0) && _type <= ITEMGET(14,8)	|| _type >= ITEMGET(14,38) && _type <= ITEMGET(14,40))
				{
					if(item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}

					if(item.m_Durability > 255.0f)	//Changed for potion)
					{
						item.m_Durability = 255.0f;
					}
				}
			}

			if(_type == ITEMGET(14,70) || _type == ITEMGET(14,71))	//OK
			{
				if(item.m_Durability == 0.0f)
				{
					item.m_Durability = 1.0f;
				}

				if(item.m_Durability > 50.0f)
				{
					item.m_Durability = 50.0f;
				}
			}
			if(_type == ITEMGET(14,94)) //OK
			{
				if(item.m_Durability == 0.0f)
				{
					item.m_Durability = 1.0f;
				}

				if(item.m_Durability > 50.0f)
				{
					item.m_Durability = 50.0f;
				}
			}
			//new
			if(_type == ITEMGET(14, 133)) //OK
			{
				if(item.m_Durability == 0.0f)
				{
					item.m_Durability = 1.0f;
				}

				if(item.m_Durability > 50.0f)
				{
					item.m_Durability = 50.0f;
				}
			}

			if(_type == ITEMGET(14,78) || _type == ITEMGET(14,79) || _type == ITEMGET(14,80) || _type == ITEMGET(14,81) || _type == ITEMGET(14,82))
			{
				if(item.m_Durability == 0.0f)
				{
					item.m_Durability = 1.0f;
				}

				if(item.m_Durability > 3.0f)
				{
					item.m_Durability = 3.0f;
				}
			}
			else if(_type >= ITEMGET(14,46) && _type <= ITEMGET(14,50))
			{
				if(item.m_Durability == 0.0f)
				{
					item.m_Durability = 1.0f;
				}

				if(item.m_Durability > 3.0f)
				{
					item.m_Durability = 3.0f;
				}
			}
			else if(_type >= ITEMGET(14,35) && _type <= ITEMGET(14,37))
			{
				if(item.m_Durability == 0.0f)
				{
					item.m_Durability = 1.0f;
				}

				if(item.m_Durability > 255.0f)
				{
					item.m_Durability = 255.0f;
				}
			}
			else if(_type != ITEMGET(13,10)	&& _type != ITEMGET(14,29)	&& _type != ITEMGET(14,21) && item.m_Level == 3 && 
				_type != ITEMGET(14,100)) //season4.5 add-on
			{
				if(item.m_Durability > item.m_BaseDurability && bIsItemExist == 1)
				{
					item.m_Durability = item.m_BaseDurability;
				}
			}
			else if(_type == ITEMGET(13,70)) //season4 add-on
			{
				if(item.m_Durability == 1.0f)
				{
					item.m_Durability = item.m_BaseDurability;
				}
			}

			lpObj->pInventory[n].m_Option1 = item.m_Option1;
			lpObj->pInventory[n].m_Option2 = item.m_Option2;
			lpObj->pInventory[n].m_Option3 = item.m_Option3;
			lpObj->pInventory[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pInventory[n].m_ItemOptionEx = item.m_ItemOptionEx;
			lpObj->pInventory[n].m_BonusSocketOption = item.m_BonusSocketOption; //season4 add-on

			for(int i = 0; i < 5; i++) //season4 add-on
			{
				lpObj->pInventory[n].m_SocketOption[i] = item.m_SocketOption[i];
			}


			hiWord = ((lpMsg->dbInventory[n*ItemDbByte+4])&0xFF)&0xFF | (((lpMsg->dbInventory[n*ItemDbByte+3])&0xFF)&0xFF) * 256;
			loWord = ((lpMsg->dbInventory[n*ItemDbByte+6])&0xFF)&0xFF | (((lpMsg->dbInventory[n*ItemDbByte+5])&0xFF)&0xFF) * 256;

			item.m_Number = ((loWord &0xFFFF)&0xFFFF) | ((hiWord & 0xFFFF)&0xFFFF) << 16;

			gObjInventoryInsertItemPos(lpObj->m_Index,item,n,0);

			if(bIsItemExist == false)
			{
				lpObj->pInventory[n].m_bItemExist = 0;

				LogAddTD("error-L2 : Unknown Item found [%s][%s] (Type:%d, LEV:%d, DUR:%d OP1:%d, OP2:%d, OP3:%d, NEWOP:%d, SET:%d)",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[n].m_Type,lpObj->pInventory[n].m_Level,
					lpObj->pInventory[n].m_Durability,lpObj->pInventory[n].m_Option1,
					lpObj->pInventory[n].m_Option2,lpObj->pInventory[n].m_Option3,
					lpObj->pInventory[n].m_NewOption,lpObj->pInventory[n].m_SetOption);
			}
		}
	}
	gObjRequestPetItemInfo(lpObj->m_Index, 0);
}

TMonsterSkillElementInfo::TMonsterSkillElementInfo()
{
	this->Reset();
}

void gObjSetBP(int aIndex)
{
	int Strength	= gObj[aIndex].Strength + gObj[aIndex].AddStrength;
	int Dexterity	= gObj[aIndex].Dexterity + gObj[aIndex].AddDexterity;
	int Vitality	= gObj[aIndex].Vitality + gObj[aIndex].AddVitality;
	int Energy		= gObj[aIndex].Energy + gObj[aIndex].AddEnergy;
	int Leadership	= gObj[aIndex].Leadership + gObj[aIndex].AddLeadership;
	// ----
	switch(gObj[aIndex].Class)
	{
		case CLASS_WIZARD:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.4) + (Vitality * 0.3) + (Energy * 0.2);
			break;

		case CLASS_KNIGHT:
			gObj[aIndex].MaxBP = (Strength * 0.15) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 1.0);
			break;

		case CLASS_ELF:
			gObj[aIndex].MaxBP = (Strength * 0.3) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 0.2);
			break;

		case CLASS_MAGUMSA:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.25) + (Vitality * 0.3) + (Energy * 0.15);
			break;

		case CLASS_DARKLORD:
			gObj[aIndex].MaxBP = (Strength * 0.3) + (Dexterity * 0.2) + (Vitality * 0.1) + (Energy * 0.15) + (Leadership * 0.3);
			break;

		case CLASS_SUMMONER:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.25) + (Vitality * 0.3) + (Energy * 0.15);
			break;

#ifdef MONK
		case CLASS_MONK:
			{
				gObj[aIndex].MaxBP = (Strength * 0.15) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 1.0);
			}
			break;
#endif
	}
}

//0055bca0
BOOL gObjSetCharacter(LPBYTE lpdata, int aIndex)
{
	int itype;
	int n;
	char szCharName[MAX_ACCOUNT_LEN+1];
	SDHP_DBCHAR_INFORESULT * lpMsg = (SDHP_DBCHAR_INFORESULT *)lpdata;

	if( OBJMAX_RANGE(aIndex) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if( lpObj->Connected < PLAYER_LOGGED )
	{
		return FALSE;
	}

	if( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		LogAddC(2, lMsg.Get(MSGGET(1, 242)), gObj[aIndex].AccountID, gObj[aIndex].Name);
		return FALSE;
	}

#ifdef MONK
	if( lpMsg->Class != 0 
		&& lpMsg->Class != 16	
		&& lpMsg->Class != 32	
		&& lpMsg->Class != 48 
		&& lpMsg->Class != 1	
		&& lpMsg->Class != 17	
		&& lpMsg->Class != 33
		&& lpMsg->Class != 64
		&& lpMsg->Class != 3
		&& lpMsg->Class != 19
		&& lpMsg->Class != 35
		&& lpMsg->Class != 50
		&& lpMsg->Class != 66
		&& lpMsg->Class != 80
		&& lpMsg->Class != 81
		&& lpMsg->Class != 83
		&& lpMsg->Class != 96
		&& lpMsg->Class != 98 )
		return false;
#else
	if ( lpMsg->Class == 0x00 || lpMsg->Class == 0x10 || lpMsg->Class == 0x20 || lpMsg->Class == 0x30 || lpMsg->Class == 0x01 || lpMsg->Class == 0x11 || lpMsg->Class == 0x21 || lpMsg->Class == 0x40 ||
		 lpMsg->Class == 0x03 || lpMsg->Class == 0x13 || lpMsg->Class == 0x23 || lpMsg->Class == 0x32 || lpMsg->Class == 0x42 || lpMsg->Class == 0x50 || lpMsg->Class == 0x51 || lpMsg->Class == 0x53)
	{

	}
	else
	{
		return false;
	}
#endif
	gObjCharZeroSet(aIndex);

	if ( g_MixSystem.ChaosBoxInit(lpObj) == FALSE )
	{
		LogAdd("error-L1 : ChaosBox Init error %s %d", __FILE__, __LINE__);
	}

	lpObj->m_PosNum = 0;
	lpObj->Level = lpMsg->Level;
	lpObj->Reset = lpMsg->Reset;
	lpObj->GrandReset = lpMsg->GrandReset;
	lpObj->LevelUpPoint = lpMsg->LevelUpPoint;
	lpObj->DbClass = lpMsg->Class;
	lpObj->Class = lpMsg->Class >> 4;

	int ChangeUP = lpMsg->Class & 0x07;
	lpObj->ChangeUP = (ChangeUP & 1);
	lpObj->ThirdChangeUp = ((ChangeUP &2)>>1);

	szCharName[MAX_ACCOUNT_LEN] = 0;
	memcpy(szCharName, lpMsg->Name , MAX_ACCOUNT_LEN);
	strcpy(lpObj->Name, szCharName);

	if ( lpObj->Level < 1 )
	{
		LogAddC(2, "error : %s Level Zero", lpMsg->Name);
		return FALSE;
	}

	lpObj->X = lpMsg->MapX;
	lpObj->Y = lpMsg->MapY;
	lpObj->MapNumber = lpMsg->MapNumber;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;
	
	if( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		LogAdd("error : Map Number max over. %s %d", __FILE__, __LINE__);
		lpObj->MapNumber = MAP_INDEX_RORENCIA;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	if( lpObj->Level < 6 && lpObj->MapNumber != 79 )	//1.01.00 update
	{
		if( lpObj->Class == CLASS_ELF )
		{
			lpObj->MapNumber = MAP_INDEX_NORIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else if( lpObj->Class == CLASS_SUMMONER )
		{
			lpObj->MapNumber = MAP_INDEX_ELBELAND;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else if( lpObj->Class == CLASS_MONK )
		{
			lpObj->MapNumber = MAP_INDEX_RORENCIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_RORENCIA;
			MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
		}
	}
	// ----
	if( DS_MAP_RANGE(lpObj->MapNumber) != FALSE )	//1.01.00 update
	{
		int iTempUserDataSlotIndex = CTemporaryUserManager::Instance()->CheckMatchCharacter(lpObj->Name);
		// ----
		if( iTempUserDataSlotIndex != -1 )
		{
			int bResult = CTemporaryUserManager::Instance()->ResumeEvent(iTempUserDataSlotIndex, aIndex);
			// ----
			if( !bResult )
			{
				lpObj->MapNumber = MAP_INDEX_RORENCIA;
				MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
			}
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_RORENCIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
	}
	// ----
	if( lpObj->MapNumber == MAP_INDEX_XMASEVENT && lpObj->Level < 15)
	{
		if( lpObj->Class == CLASS_ELF )
		{
			lpObj->MapNumber = MAP_INDEX_NORIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else if( lpObj->Class == CLASS_SUMMONER )
		{
			lpObj->MapNumber = MAP_INDEX_ELBELAND;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_RORENCIA;
			MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
		}
	}
	// ----
	if( BC_MAP_RANGE(lpObj->MapNumber) != FALSE )	//1.01.00 update
	{
		int iTempUserDataSlotIndex = CTemporaryUserManager::Instance()->CheckMatchCharacter(lpObj->Name);
		// ----
		if( iTempUserDataSlotIndex != -1 )
		{
			int bResult = CTemporaryUserManager::Instance()->ResumeEvent(iTempUserDataSlotIndex, aIndex);
			// ----
			if( !bResult )
			{
				lpObj->MapNumber = MAP_INDEX_DEVIAS;
				MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
				lpObj->m_cBloodCastleIndex		= -1;
				lpObj->m_cBloodCastleSubIndex	= -1;
				lpObj->m_iBloodCastleEXP		= 0;
			}
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_DEVIAS;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
			lpObj->m_cBloodCastleIndex		= -1;
			lpObj->m_cBloodCastleSubIndex	= -1;
			lpObj->m_iBloodCastleEXP		= 0;
		}
	}
	else
	{
		lpObj->m_cBloodCastleIndex		= -1;
		lpObj->m_cBloodCastleSubIndex	= -1;
		lpObj->m_iBloodCastleEXP		= 0;
		
	}
	// ----
	lpObj->m_bBloodCastleComplete	= 0;
	// ----
	if( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
	// ----
	lpObj->m_cChaosCastleIndex		= -1;
	lpObj->m_cChaosCastleSubIndex	= -1;
	lpObj->m_iChaosCastleBlowTime	= 0;
	lpObj->m_cKillUserCount			= 0;
	lpObj->m_cKillMonsterCount		= 0;
	// ----
	if( (lpMsg->CtlCode&32) != 32 && lpObj->MapNumber == MAP_INDEX_GM_MAP )
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
	else if( IT_MAP_RANGE(lpObj->MapNumber) )	//1.01.00 update
	{
		int iTempUserDataSlotIndex = CTemporaryUserManager::Instance()->CheckMatchCharacter(lpObj->Name);
		// ----
		if( iTempUserDataSlotIndex != -1 )
		{
			int bResult = CTemporaryUserManager::Instance()->ResumeEvent(iTempUserDataSlotIndex, aIndex);
			// ----
			if( !bResult )
			{
				lpObj->MapNumber = MAP_INDEX_ELBELAND;
				MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
			}
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_ELBELAND;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
	}
	else if( lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS )
	{
		short sX;
		short sY;
		short sLVL;
		BYTE btMAPNUM = lpObj->MapNumber;
		BYTE btDIR;
		// ----
		int iMapNumber = gGateC.GetGate(137, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);
		// ----
		if( iMapNumber >= 0 )
		{
			lpObj->MapNumber	= btMAPNUM;
			lpObj->X			= sX;
			lpObj->Y			= sY;
		}
	}
	else if( lpObj->MapNumber == MAP_INDEX_3RD_CHANGEUP_QUEST 
		|| lpObj->MapNumber == MAP_INDEX_3RD_CHANGEUP_BOSS_QUEST )
	{
		int iGateNumber = 258;
		short sX;
		short sY;
		short sLVL;
		BYTE btMAPNUM = lpObj->MapNumber;
		BYTE btDIR;
		// ----
		int iMapNumber = gGateC.GetGate(iGateNumber, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);
		// ----
		if( iMapNumber >= 0 )
		{
			lpObj->MapNumber	= btMAPNUM;
			lpObj->X			= sX;
			lpObj->Y			= sY;
		}
	}
	// ----
	if( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
	{
		if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
		{
			short sX;
			short sY;
			short sLVL;
			BYTE btMAPNUM = lpObj->MapNumber;
			BYTE btDIR;
			// ----
			int iMapNumber = gGateC.GetGate(100, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);
			// ----
			if( iMapNumber >= 0 )
			{
				lpObj->MapNumber = btMAPNUM;
				lpObj->X = sX;
				lpObj->Y = sY;
				lpObj->m_sDestMapNumber = btMAPNUM;
				lpObj->m_btDestX = sX;
				lpObj->m_btDestY = sY;
			}
		}
	}
	// ----
	if( lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE )
	{
		short sX;
		short sY;
		short sLVL;
		BYTE btMAPNUM = lpObj->MapNumber;
		BYTE btDIR;
		
		int iMapNumber = gGateC.GetGate(106,(short &)sX,(short &)sY,(BYTE &)btMAPNUM,(BYTE &)btDIR,(short &)sLVL);
		
		if(iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;
			lpObj->m_sDestMapNumber = btMAPNUM;
			lpObj->m_btDestX = sX;
			lpObj->m_btDestY = sY;
		}
	}
	// ----
	if( lpObj->m_bMapSvrMoveReq == true )
	{
		short sSVR_CODE = ::g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->m_sDestMapNumber, lpObj->m_sPrevMapSvrCode);
		// ----
		if( sSVR_CODE == gGameServerCode )
		{
			lpObj->MapNumber = lpObj->m_sDestMapNumber;
			lpObj->X = lpObj->m_btDestX;
			lpObj->Y = lpObj->m_btDestY;
			lpObj->StartX = lpObj->X;
			lpObj->StartY = lpObj->Y;
		}
		else	
		{
			LogAddC(2, "[MapServerMng] Dest Map Server doesn't have Map [%s][%s] : %d",	lpObj->AccountID, lpObj->Name, lpObj->m_sDestMapNumber);
			gObjDel(lpObj->m_Index);
			return FALSE;
		}
	}
	// ----
#ifdef NPVP
	if( gNonPK )
	{
		if( g_NewPVP.IsVulcanusMap(lpObj->MapNumber) || g_NewPVP.IsPKFieldMap(lpObj->MapNumber) )
		{
			int iGateNumber = 17;
			short sX;
			short sY;
			short sLVL;
			BYTE btMAPNUM	= lpObj->MapNumber;
			BYTE btDIR;
			// ----
			int iMapNumber = gGateC.GetGate(iGateNumber, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);
			// ----
			if( iMapNumber >= 0 )
			{
				lpObj->MapNumber	= btMAPNUM;
				lpObj->X			= sX;
				lpObj->Y			= sY;
			}
		}
	}
	// ----
#else
	if( lpObj->MapNumber == MAP_INDEX_PKFIELD )
	{
		lpObj->MapNumber = MAP_INDEX_VULCANUS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
#endif
	// ----
#ifdef IMPERIAL
	if( CImperialGuardian::IsEventMap(lpObj->MapNumber) )	//1.01.00 update
	{
		int iTempUserDataSlotIndex = CTemporaryUserManager::Instance()->CheckMatchCharacter(lpObj->Name);
		// ----
		if( iTempUserDataSlotIndex != -1 )
		{
			int bResult = CTemporaryUserManager::Instance()->ResumeEvent(iTempUserDataSlotIndex, aIndex);
			// ----
			if( !bResult )
			{
				int iGateNumber = 22;
				short sX;
				short sY;
				short sLVL;
				BYTE btMAPNUM	= lpObj->MapNumber;
				BYTE btDIR;
				// ----
				int iMapNumber = gGateC.GetGate(iGateNumber, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);
				// ----
				if( iMapNumber >= 0 )
				{
					lpObj->MapNumber	= btMAPNUM;
					lpObj->X			= sX;
					lpObj->Y			= sY;
				}
			}
		}
		else
		{
			int iGateNumber = 22;
			short sX;
			short sY;
			short sLVL;
			BYTE btMAPNUM	= lpObj->MapNumber;
			BYTE btDIR;
			// ----
			int iMapNumber = gGateC.GetGate(iGateNumber, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);
			// ----
			if( iMapNumber >= 0 )
			{
				lpObj->MapNumber	= btMAPNUM;
				lpObj->X			= sX;
				lpObj->Y			= sY;
			}
		}
	}
#endif
	// ----
	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
	int result;
	short x;
	short y;
	BYTE mapNumber;
	BYTE dir;
	short level;
	int gt;
	int userlevel;

	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		LogAdd(lMsg.Get(MSGGET(1, 243)));
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	mapNumber = MAP_INDEX_RORENCIA;
	userlevel = gObj[aIndex].Level;
	x = lpObj->X;
	y = lpObj->Y;
	dir = lpObj->Dir;
	level = lpObj->Level;
	mapNumber = lpObj->MapNumber;
	gt = 0;

	if(g_iBlockKanturuMapEnter == 1)
	{
		if(gObj[aIndex].MapNumber >= MAP_INDEX_KANTURU1 && gObj[aIndex].MapNumber <= MAP_INDEX_KANTURU_BOSS)
		{
			LogAddTD("[ KANTURU ][ Forced Move From Kanturu to Lorencia ] (%s)(%s) (BeforeMap:%d(%d/%d))", gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y);
			gObj[aIndex].MapNumber = MAP_INDEX_RORENCIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
		}
	}

	if(g_iBlockCastleSiegeMapEnter == 1)
	{
		if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE || gObj[aIndex].MapNumber == MAP_INDEX_CASTLEHUNTZONE || gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE || gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_SECONDZONE)
		{
			LogAddTD("[ KANTURU ][ Forced Move From CastleSiegeMap to Lorencia ] (%s)(%s) (BeforeMap:%d(%d/%d))", gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y);
			gObj[aIndex].MapNumber = MAP_INDEX_RORENCIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
		}
	}

	if(lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS &&
		(g_Raklion.GetRaklionState() == RAKLION_STATE_CLOSE_DOOR ||
		g_Raklion.GetRaklionState() == RAKLION_STATE_ALL_USER_DIE ||
		g_Raklion.GetRaklionState() == RAKLION_STATE_NOTIFY_4 ||
		g_Raklion.GetRaklionState() == RAKLION_STATE_END)) //season4 add-on
	{
		gt = 286;
	}

	if(lpObj->MapNumber == MAP_INDEX_RAKLION_FIELD) //season4 add-on
	{
		g_RaklionUtil.NotifyRaklionCurrentState(lpObj->m_Index, g_Raklion.GetRaklionState(), g_Raklion.GetRaklionDetailState());
	}
	if( lpObj->MapNumber == MAP_INDEX_PKFIELD || lpObj->MapNumber == MAP_INDEX_VULCANUS )	//1.01.00 update
	{
		if( !lpObj->m_bMapSvrMoveReq )
		{
			int iGateNumber = 17;
			short sX;
			short sY;
			short sLVL;
			BYTE btMAPNUM	= lpObj->MapNumber;
			BYTE btDIR;
			// ----
			int iMapNumber = gGateC.GetGate(iGateNumber, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);
			// ----
			if( iMapNumber >= 0 )
			{
				lpObj->MapNumber	= btMAPNUM;
				lpObj->X			= sX;
				lpObj->Y			= sY;
			}
		}
	}
#ifdef DP
	if( g_DoppleganerEvent.IsEventMap(lpObj->MapNumber) )	//1.01.00 update
	{
		int iTempUserDataSlotIndex = CTemporaryUserManager::Instance()->CheckMatchCharacter(lpObj->Name);
		// ----
		if( iTempUserDataSlotIndex != -1 )
		{
			int bResult = CTemporaryUserManager::Instance()->ResumeEvent(iTempUserDataSlotIndex, aIndex);
			// ----
			if( !bResult )
			{
				lpObj->MapNumber = MAP_INDEX_ELBELAND;
				MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
			}
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_ELBELAND;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
	}
#endif
	if( lpObj->MapNumber == MAP_INDEX_DEVIAS )
	{
		gt = 22;
	}
	else if ( lpObj->MapNumber == MAP_INDEX_NORIA )
	{
		gt = 27;
	}
	else if ( lpObj->MapNumber == MAP_INDEX_LOSTTOWER )
	{
		gt = 42;
	}

	if ( gt > 0 )
	{
		result = gGateC.GetGate(gt, x, y, mapNumber, dir, level);

		if ( result >= 0 )
		{
			if(mapNumber != MAP_INDEX_SWAMP_OF_CALMNESS)
			{
#ifdef MONK
				if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_MONK )
#else
				if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA )
#endif
				{
					if ( level > 0 )
					{
						level = level / 3 * 2;
					}
				}
			}

			if ( level != 0 && lpObj->Level < level)
			{
				if ( lpObj->Class == CLASS_ELF ) // Elf
				{
					lpObj->MapNumber = MAP_INDEX_NORIA;
					MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
				}
				else
				{
					lpObj->MapNumber = MAP_INDEX_RORENCIA;
					MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
				}
			}
		}
		else
		{
			LogAdd("error : %d (%s %d)", result, __FILE__, __LINE__);
		}
	}

	lpMsg->MapX = lpObj->X;
	lpMsg->MapY = lpObj->Y;
	lpObj->TX = lpMsg->MapX;
	lpObj->TY = lpMsg->MapY;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Experience = lpMsg->Exp;
	lpObj->Strength = lpMsg->Str ;
	lpObj->Dexterity = lpMsg->Dex ;
	lpObj->Vitality = lpMsg->Vit ;
	lpObj->Energy = lpMsg->Energy ;
	lpObj->Life = lpMsg->Life;
	lpObj->Life /= 10.0f;
	lpObj->MaxLife = lpMsg->MaxLife;
	lpObj->MaxLife /= 10.0f;
	lpObj->Mana = lpMsg->Mana;
	lpObj->Mana /= 10.0f;
	lpObj->MaxMana = lpMsg->MaxMana;
	lpObj->MaxMana /= 10.0f;
	lpObj->Money = lpMsg->Money;
	lpObj->Leadership = lpMsg->Leadership;
	lpObj->ChatLitmitTime = lpMsg->ChatLitmitTime;
	lpObj->iFruitPoint = lpMsg->iFruitPoint;

	
#ifdef EXPINV
	lpObj->ExpandedInventory = lpMsg->ExpandedInventory;
#endif

#ifdef __ALIEN__
	lpObj->DbOffExp = lpMsg->DbOffExp;
#endif

	if ( lpObj->Mana < 1.0f )
	{
		lpObj->Mana = 1.0f;
	}

	if ( lpObj->Life < 1.0f )
	{
		lpObj->Life = 1.0f;
	}

	lpObj->MaxLife = DCInfo.DefClass[lpObj->Class].Life + (lpObj->Level + lpObj->m_nMasterLevel - 1) * DCInfo.DefClass[lpObj->Class].LevelLife  + ((lpObj->Vitality - DCInfo.DefClass[lpObj->Class].Vitality ) * DCInfo.DefClass[lpObj->Class].VitalityToLife);
	lpObj->MaxMana = DCInfo.DefClass[lpObj->Class].Mana + (lpObj->Level + lpObj->m_nMasterLevel - 1) * DCInfo.DefClass[lpObj->Class].LevelMana  + ((lpObj->Energy - DCInfo.DefClass[lpObj->Class].Energy ) * DCInfo.DefClass[lpObj->Class].EnergyToMana);

	lpObj->VitalityToLife = DCInfo.DefClass[lpObj->Class].VitalityToLife;
	lpObj->EnergyToMana = DCInfo.DefClass[lpObj->Class].EnergyToMana;

	::gObjSetBP(aIndex);
	lpObj->BP = lpObj->MaxBP / 2;
	::gObjCalcMaxLifePower(aIndex);
	lpObj->m_PK_Count = lpMsg->PkCount;
	lpObj->m_PK_Level = lpMsg->PkLevel;
	lpObj->m_PK_Time  = lpMsg->PkTime;

	if ( ::gApplyHeroSystem != FALSE )
	{
		if ( lpObj->m_PK_Level <= 3 )
		{
			lpObj->m_PK_Level = 2;
		}
	}

	if ( lpObj->m_PK_Level == 3 )
	{
		lpObj->m_PK_Count = 0;
		lpObj->m_PK_Time = 0;
	}

	lpObj->MaxRegenTime = 4000;
	lpObj->m_MoveSpeed = 1000;

	memcpy(lpObj->m_Quest, lpMsg->dbQuest, sizeof(lpObj->m_Quest));

	if ( lpObj->m_Quest[0] == 0 )
	{
		memset(lpObj->m_Quest, (BYTE)-1, sizeof(lpObj->m_Quest));
	}

#ifdef __CUSTOMS__
	if( g_QuestInfo.GetQuestState(lpObj, 2) == 2 )
	{
		lpObj->PlusStatQuestClear = true;
	}
	// ----
	if( g_QuestInfo.GetQuestState(lpObj, 3) == 2 )
	{
		lpObj->ComboSkillquestClear = true;
	}
#else
	if( g_QuestInfo.GetQuestState(lpObj, 2) == 2 )
	{
		if ( lpObj->Level < QUEST_MINLEVEL_PLUSSTAT )
		{
			//::g_QuestInfo.ReSetQuestState(lpObj, 2);
			LogAddTD("[%s][%s] Find Invalid QuestInfo (%d)",
				lpObj->AccountID, lpObj->Name, 2);
		}
		else
		{
			lpObj->PlusStatQuestClear = true;
		}
	}

	if( g_QuestInfo.GetQuestState(lpObj, 3) == 2 )
	{
		if ( lpObj->Level < QUEST_MINLEVEL_PLUSSTAT )
		{
			//::g_QuestInfo.ReSetQuestState(lpObj, 3);
			LogAddTD("[%s][%s] Find Invalid QuestInfo (%d)",
				lpObj->AccountID, lpObj->Name, 3);
		}
		else
		{
			lpObj->ComboSkillquestClear = true;
		}
	}
#endif
	::gObjSetInventory1Pointer(lpObj);
	lpObj->GuildNumber = 0;
	lpObj->lpGuild = NULL;

	if ( lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_ELF || lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_SUMMONER || lpObj->Class == CLASS_MAGUMSA 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif
		)
	{
		BOOL bEnergyBall = TRUE;
		BOOL bForceSkill = TRUE;
		BOOL IsGetInitinityArrowSkill = FALSE;

		gObjMagicAddEnergyCheckOnOff(0);

		for ( n=0;n<MAX_MAGIC;n++)
		{
			itype = lpMsg->dbMagicList[n*3];

			if( (lpMsg->dbMagicList[n*3+1]&0x7) > 0) //Season3 add-on
			{
				itype = itype * (lpMsg->dbMagicList[n*3+1]&0x7) + lpMsg->dbMagicList[n*3+2];
			}

			if( itype != 0xFF )
			{
				if( g_MasterSkillSystem.CheckMasterLevelSkill(itype) != FALSE )
				{
					BYTE btLevel = lpMsg->dbMagicList[n*3+1]>>3;
					// ----
					if( gObjMagicAdd(lpObj, itype, btLevel) > -1 )
					{
						//lpObj->m_bMasterLevelDBLoad += btLevel;
						g_MasterSkillSystem.CalcPassiveSkillData(lpObj, itype, btLevel);
					}
				}
				else
				{
					if ( itype == 17 )
					{
						bEnergyBall = FALSE;
					}

					if ( itype == 60 )
					{
						bForceSkill = FALSE;
					}

					if ( itype == 77 )
					{
						IsGetInitinityArrowSkill = 1;
						gObjMagicAdd(lpObj,itype,lpMsg->dbMagicList[n*3+1]&0xF);
					}

#ifdef SEASON6DOT3_ENG
					if ( itype != 18
						&& itype != 19
						&& itype != 20
						&& itype != 21
						&& itype != 22
						&& itype != 23
						&& itype != 24
						&& itype != 25
						&& itype != 49
						&& itype != 56
						&& itype != 62
						&& itype != 76
						&& itype != 223
						&& itype != 224
						&& itype != 225
						&& itype != 260
						&& itype != 261
						&& itype != 270
						&& itype != 54 )
#else
					if (itype != 18 && itype != 19 && 
						itype != 20 && itype != 21 && 
						itype != 22 && itype != 23 && 
						itype != 24 && itype != 25 && 
						itype != 49 && itype != 56 && 
						itype != 62 && itype != 76 && 
						itype != 223 && itype != 224 &&
						itype != 225 && //season4 add-on
						itype != 54 )
#endif
					{
						gObjMagicAdd(lpObj, itype, lpMsg->dbMagicList[n*3+1] & 0x0F);
					}
				}
			}
		}

		if ( lpObj->Class == CLASS_WIZARD )
		{
			if ( bEnergyBall == TRUE )
			{
				gObjMagicAdd(lpObj, 17, 0);
				bEnergyBall = FALSE;
			}
		}

		if ( lpObj->Class == CLASS_DARKLORD )
		{
			if ( bForceSkill == TRUE )
			{
				gObjMagicAdd(lpObj, 60, 0);
				bForceSkill = FALSE;
			}
		}

		if ( IsGetInitinityArrowSkill == 0 && lpObj->Class == CLASS_ELF)
		{
			if(lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel())
			{
				if(lpObj->ChangeUP == 1)
				{
					int iAddSkillPosition = gObjMagicAdd(lpObj,77,0);

					if(iAddSkillPosition > 0)
					{
						LogAddTD("[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)",
							lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->ChangeUP);
					}
				}
			}
		}

		if ( lpObj->Class == CLASS_WIZARD )
		{
			gObjMagicAdd(lpObj, 45, 0);
		}
		else if ( lpObj->Class == CLASS_KNIGHT )
		{
			gObjMagicAdd(lpObj, 44, 0);
		}
		else if ( lpObj->Class == CLASS_ELF )
		{
			gObjMagicAdd(lpObj, 46, 0);
		}
		else if ( lpObj->Class == CLASS_MAGUMSA )
		{
			gObjMagicAdd(lpObj, 57, 0);
			gObjMagicAdd(lpObj, 73, 0);
		}
		else if ( lpObj->Class == CLASS_DARKLORD )
		{
			gObjMagicAdd(lpObj, 74, 0);
			gObjMagicAdd(lpObj, 75, 0);
		}
		else if ( lpObj->Class == CLASS_SUMMONER )
		{
			gObjMagicAdd(lpObj, 45, 0);
		}
#ifdef MONK
		else if ( lpObj->Class == CLASS_MONK )
		{
			gObjMagicAdd(lpObj, 269, 0);
		}
#endif

		gObjMagicAdd(lpObj,67,0);
		gObjMagicAdd(lpObj,68,0);
		gObjMagicAdd(lpObj,69,0);
		gObjMagicAdd(lpObj,70,0);
		gObjMagicAdd(lpObj,71,0);
		gObjMagicAdd(lpObj,72,0);


		::gObjMagicAddEnergyCheckOnOff(TRUE);
	}

	bool bAllItemExist = true;

	if ( lpMsg->DbVersion == 0 ) 
	{
		::DbItemSetInByte(lpObj, lpMsg, 7, &bAllItemExist);

		for ( int i=MAIN_INVENTORY_SIZE;i<INVENTORY_SIZE;i++)
		{
			lpObj->Inventory1[i].Clear();
		}

		memset(lpObj->InventoryMap1+64, (BYTE)-1, PSHOP_MAP_SIZE);	
	}
	else if(lpMsg->DbVersion < 3)
	{
		::DbItemSetInByte(lpObj, lpMsg, 10, &bAllItemExist);
	}
	else
	{
		::DbItemSetInByte(lpObj, lpMsg, 16, &bAllItemExist);
	}

	if( lpObj->Level <= 5 
#ifdef __CUSTOMS__
		&& g_ResetSystem.m_ShopCleaning
#endif
		)
	{
		for ( int i=MAIN_INVENTORY_SIZE;i<INVENTORY_SIZE;i++)
		{
			BYTE NewOption[MAX_EXOPTION_SIZE];

			if ( lpObj->Inventory1[i].IsItem() == TRUE )
			{
				for ( int j=0;j<MAX_EXOPTION_SIZE;j++)
				{
					NewOption[j] = 0;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x20 ) != 0 )
				{
					NewOption[0] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x10 ) != 0 )
				{
					NewOption[1] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x08 ) != 0 )
				{
					NewOption[2] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x04 ) != 0 )
				{
					NewOption[3] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x02 ) != 0 )
				{
					NewOption[4] = TRUE;
				}

				if ( ( lpObj->Inventory1[i].m_NewOption & 0x01 ) != 0 )
				{
					NewOption[5] = TRUE;
				}

				LogAddTD("[PShop] PShop Item Delete LV <= 5 [%s][%s] [LV:%d] : serial:%u [%s][%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]",
					lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->Inventory1[i].m_Number, 
					ItemAttribute[lpObj->Inventory1[i].m_Type].Name, lpObj->Inventory1[i].m_Level,
					lpObj->Inventory1[i].m_Option1, lpObj->Inventory1[i].m_Option2,
					lpObj->Inventory1[i].m_Option3, NewOption[0], NewOption[1], NewOption[2], NewOption[3],
					NewOption[4], NewOption[5], NewOption[6], lpObj->Inventory1[i].m_SetOption);
			}
			gObjInventoryItemSet_PShop(lpObj->m_Index, i, 255);	//fix
			lpObj->Inventory1[i].Clear();
		}

		memset(lpObj->InventoryMap1+64, (BYTE)-1, PSHOP_MAP_SIZE);
	}

	lpObj->Live = TRUE;
	lpObj->Type = OBJ_USER;
	lpObj->TargetNumber = -1;
	lpObj->Connected = PLAYER_PLAYING;
	gObjFindInventoryEquipment(aIndex);	//1.01.03
	gObjMakePreviewCharSet(aIndex);
	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpMsg->NextExp = lpObj->NextExp;

	if ( lpObj->Life == 0.0f )
	{
		lpObj->Live = TRUE;
		lpObj->m_State = 4;
		lpObj->DieRegen = TRUE;
		lpObj->RegenTime = GetTickCount();
	}
	else
	{
		lpObj->m_State = 1;
	}


	if ( (lpMsg->CtlCode & 8) == 8 )
	{
		lpObj->Authority = 2;
		LogAddC(2, lMsg.Get(MSGGET(1, 245)), lpObj->AccountID, lpObj->Name);
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	}
	else if ( strcmp(lpObj->Name, "Webzen") == 0 )
	{
		lpObj->Authority = 2;
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	}
	else if ( strstr(lpObj->Name, "webzen") != 0 )
	{
		lpObj->Authority = 2;
		LogAddC(2, lMsg.Get(MSGGET(1, 245)), lpObj->AccountID, lpObj->Name);
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	}
	else if ( strstr(lpObj->Name, "Webzen") != 0 )
	{
		lpObj->Authority = 2;
		LogAddC(2, lMsg.Get(MSGGET(1, 245)), lpObj->AccountID, lpObj->Name);
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
	}
	else
	{
		lpObj->Authority = 1;
	}
	
	if ( (lpMsg->CtlCode & 0x20 ) == 0x20 )
	{
		lpObj->Authority = 0x20;
		LogAddC(2, "(%s)(%s) Set Event GM", lpObj->AccountID, lpObj->Name);
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index); //Season 2.5 add-on
	}
	
	if(lpObj->Authority == 32) //Season3 add-on
	{
		gObjAddBuffEffect(lpObj, AT_GAMEMASTER_LOGO, 0, 0, 0, 0, -10);
	}

	char * SubName = cManager.GetSubName(lpObj->Authority);

	if(SubName == NULL)
	{
		memcpy(lpObj->SubName, "Unknow", sizeof(lpObj->SubName));
	}
	else
	{
		memcpy(lpObj->SubName, SubName, sizeof(lpObj->SubName));;
	}

	LogAddC(2, "[%s] (%s)(%s) Connected",lpObj->SubName, lpObj->AccountID, lpObj->Name);

	lpObj->Penalty = 0;

	if ( lpObj->m_cAccountItemBlock != 0 )
	{
		lpObj->Penalty |= 8;
	}

	if ( (lpMsg->CtlCode & 2) == 2 )
	{
		lpObj->Penalty |= 0x4;
		LogAddL("Penalty : Item Don't touch  %s %s", lpObj->AccountID, lpObj->Name);
	}

	::gObjAuthorityCodeSet(lpObj);
	LogAddTD(lMsg.Get(MSGGET(1, 246)), lpObj->m_Index, lpObj->AccountID, lpObj->Name);

	BYTE weather = MapC[lpObj->MapNumber].GetWeather();

	if ( (weather >> 4) > 0 )
	{
		CGWeatherSend(aIndex, weather);
	}

	if ( DragonEvent->GetState() > 0 )
	{
		if ( DragonEvent->GetMapNumber() == lpObj->MapNumber )
		{
			::GCMapEventStateSend(lpObj->MapNumber, 1, 1);
		}
	}

//#if(_GSCS==1)
	if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
	{	
		::GCAnsCsNotifyStart(lpObj->m_Index,CASTLESIEGEREADY(g_CastleSiege.GetCastleState()));
		
		if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
		}
	}
//#endif

	::gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);
	::GCCheckMainExeKeySend(aIndex);

	if ( lpObj->Level <= g_iCharacterRecuperationMaxLevel )
	{
		lpObj->m_iAutoRecuperationTime = GetTickCount();
	}
	else
	{
		lpObj->m_iAutoRecuperationTime = 0;
	}

	lpObj->dwShieldAutoRefillTimer = GetTickCount();

	LogAddTD("[ShieldSystem][CalcSDPoint] [%s][%s] user SD Gage : %d", lpObj->AccountID,lpObj->Name,lpObj->iMaxShield+lpObj->iAddShield);

#ifdef GAMESHOP
	gGameShop.AddUser(aIndex);
#endif
	//g_CashShop.AddUser(lpObj); //0066de50
	//g_CashShop.CGCashPoint(lpObj); //0066e0e0
	//CCashShopInGame.AddUser(lpObj); 006b2900
#ifdef PERIOD
	g_PeriodItemEx.AddPeriodInfo(lpObj);
#endif

#ifdef QUESTSYSTEM
	g_QuestSystem.AddUser(lpObj);
#endif

	g_CashItemPeriodSystem.GDReqPeriodItemList(lpObj);

	if(lpObj->Class == CLASS_DARKLORD) //season4 add-on
	{
		gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::ePetItem_Mode::PetItem_Mode_Normal,-1);
	}
	
	return TRUE;
}

BOOL gObjCanItemTouch(LPOBJ lpObj, int type )
{
	if ( (lpObj->Penalty &4) == 4 )
	{
		return false;
	}

	if ( (lpObj->Penalty &8) == 8 )
	{
		if ( type == 6 || type == 8 )
		{
			return true;
		}
		return false;
	}
	return true;
}

void gObjMagicTextSave(LPOBJ lpObj)
{
	int total=0;

	for ( int n=0;n<MAGIC_SIZE;n++)
	{
		if ( lpObj->Magic[n].IsMagic() == TRUE )
		{
			LogAddTD(lMsg.Get(MSGGET(1, 247)), lpObj->AccountID, lpObj->Name, n, lpObj->Magic[n].m_Skill);
			total++;
		}
	}
}

void ItemIsBufExOption(BYTE * buf, CItem * lpItem)
{
	for ( int i=0;i<MAX_EXOPTION_SIZE;i++ )
	{
		buf[i] = 0;
	}

	if ( lpItem->IsExtLifeAdd() != FALSE )
	{
		buf[0] = TRUE;
	}

	if ( lpItem->IsExtManaAdd() != FALSE )
	{
		buf[1] = TRUE;
	}

	if ( lpItem->IsExtDamageMinus() != FALSE )
	{
		buf[2] = TRUE;
	}

	if ( lpItem->IsExtDamageReflect() != FALSE )
	{
		buf[3] = TRUE;
	}

	if ( lpItem->IsExtDefenseSuccessfull() != FALSE )
	{
		buf[4] = TRUE;
	}

	if ( lpItem->IsExtMonsterMoney() != FALSE )
	{
		buf[5] = TRUE;
	}

}

void gObjStatTextSave(LPOBJ lpObj)
{
	LogAddTD("[%s][%s] CharInfoSave : Class=%d Level=%d LVPoint=%d Exp=%u Str=%d Dex=%d Vit=%d Energy=%d Leadership:%d Map=%d Pk=%d",
		lpObj->AccountID, lpObj->Name, lpObj->DbClass,lpObj->Level, lpObj->LevelUpPoint, lpObj->Experience, 
		lpObj->Strength, lpObj->Dexterity, lpObj->Vitality, lpObj->Energy, lpObj->Leadership, lpObj->MapNumber,
		lpObj->m_PK_Level);
}

void gObjItemTextSave(LPOBJ lpObj)
{
	BYTE NewOption[MAX_EXOPTION_SIZE];
	int n;

	for ( n=0;n<INVENTORY_SIZE;n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			//if ( lpObj->pInventory[n].m_serial != 0 ) //Season 2.5 removed
			{
				ItemIsBufExOption(NewOption, &lpObj->pInventory[n]);
				LogAddTD(lMsg.Get(MSGGET(1, 248)), lpObj->AccountID, lpObj->Name, n, lpObj->pInventory[n].GetName(),
					lpObj->pInventory[n].m_Level, lpObj->pInventory[n].m_Option1,
					lpObj->pInventory[n].m_Option2, lpObj->pInventory[n].m_Option3,
					lpObj->pInventory[n].m_Number, (BYTE)lpObj->pInventory[n].m_Durability,
					NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
					lpObj->pInventory[n].m_SetOption,lpObj->pInventory[n].m_ItemOptionEx >> 7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[n]),
					//season4 add-on
					lpObj->pInventory[n].m_SocketOption[0], lpObj->pInventory[n].m_SocketOption[1],
					lpObj->pInventory[n].m_SocketOption[2], lpObj->pInventory[n].m_SocketOption[3],
					lpObj->pInventory[n].m_SocketOption[4], lpObj->pInventory[n].m_BonusSocketOption);
			}
		}
	}

	LogAddTD(lMsg.Get(MSGGET(1, 249)), lpObj->AccountID, lpObj->Name, lpObj->Money);
}

void gObjWarehouseTextSave(LPOBJ lpObj)
{
	BYTE NewOption[MAX_EXOPTION_SIZE];
	int n;

	for ( n=0;n<WAREHOUSE_SIZE;n++)
	{
		if ( lpObj->pWarehouse[n].IsItem() == TRUE )
		{
			//if ( lpObj->pWarehouse[n].m_serial != 0 ) //Season 2.5 removed
			{
				ItemIsBufExOption(NewOption, &lpObj->pWarehouse[n]);
				LogAddTD(lMsg.Get(MSGGET(1, 250)), lpObj->AccountID, lpObj->Name, n, lpObj->pWarehouse[n].GetName(),
					lpObj->pWarehouse[n].m_Level, lpObj->pWarehouse[n].m_Option1,
					lpObj->pWarehouse[n].m_Option2, lpObj->pWarehouse[n].m_Option3,
					lpObj->pWarehouse[n].m_Number, (BYTE)lpObj->pWarehouse[n].m_Durability,
					NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
					lpObj->pWarehouse[n].m_SetOption, lpObj->pWarehouse[n].m_ItemOptionEx>>7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pWarehouse[n]),
					g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pWarehouse[n]),
					//season4 add-on
					lpObj->pWarehouse[n].m_SocketOption[0], lpObj->pWarehouse[n].m_SocketOption[1],
					lpObj->pWarehouse[n].m_SocketOption[2], lpObj->pWarehouse[n].m_SocketOption[3],
					lpObj->pWarehouse[n].m_SocketOption[4], lpObj->pWarehouse[n].m_BonusSocketOption);
			}
		}
	}

	LogAddTD(lMsg.Get(MSGGET(1, 251)), lpObj->AccountID, lpObj->Name, lpObj->WarehouseMoney);
}

void gObjAuthorityCodeSet(LPOBJ lpObj)
{
	/*if ( (lpObj->Authority&1) == 1 )
	{
		return;
	}

	if ( (lpObj->Authority&2)== 2 )
	{
		lpObj->AuthorityCode = -1;
		return;
	}

	if ( (lpObj->Authority&4) == 4 )
	{
		return;
	}

	if ( (lpObj->Authority&8) == 8 )
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;

		return;
	}

	if ( (lpObj->Authority&16) == 16 )
	{
		return;
	}

	if ( (lpObj->Authority&32) == 32 )
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;

		return;
	}*/
}

//00560310
BOOL gObjSetPosMonster(int aIndex, int PosTableNum)
{
	if ( (  (aIndex<0)? FALSE: (aIndex > OBJMAX-1)? FALSE: TRUE ) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if ( (  (PosTableNum<0)? FALSE: (PosTableNum > OBJ_MAXMONSTER-1)? FALSE: TRUE ) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];
	lpObj->m_PosNum = PosTableNum;
	lpObj->X = gMSetBase.m_Mp[PosTableNum].m_X;
	lpObj->Y = gMSetBase.m_Mp[PosTableNum].m_Y;
	lpObj->MapNumber = gMSetBase.m_Mp[PosTableNum].m_MapNumber;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = gMSetBase.m_Mp[PosTableNum].m_Dir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	if ( lpObj->Class == 44 ||  lpObj->Class == 53 ||  lpObj->Class == 54 ||  lpObj->Class == 55 ||  lpObj->Class == 56)
	{

	}
	else if (  lpObj->Class >= 78 &&  lpObj->Class <= 83)
	{

	}
	else if (  lpObj->Class >= 493 &&  lpObj->Class <= 502) //season4 add-on
	{

	}
	else if ( gMSetBase.GetPosition(PosTableNum, lpObj->MapNumber, lpObj->X, lpObj->Y) == FALSE )
	{
		LogAdd(lMsg.Get(MSGGET(1, 252)), __FILE__, __LINE__);
		LogAdd("error-L2 : %d class : %d", PosTableNum, lpObj->Class);
		return FALSE;
	}

	lpObj->TX		= lpObj->X;
	lpObj->TY		= lpObj->Y;
	lpObj->m_OldX	= lpObj->X;
	lpObj->m_OldY	= lpObj->Y;
	lpObj->Dir		= gMSetBase.m_Mp[PosTableNum].m_Dir;
	lpObj->StartX	= lpObj->X;
	lpObj->StartY	= lpObj->Y;

	return true;
}

//00560740
BOOL gObjSetMonster(int aIndex, int MonsterClass)
{
	int Level;
	LPOBJ lpObj;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return false;
	}

	lpObj = &gObj[aIndex];
	lpObj->ConnectCheckTime = GetTickCount();
	lpObj->ShopNumber = -1;
	lpObj->TargetNumber = -1;
	lpObj->m_RecallMon = -1;

	switch ( MonsterClass )
	{
		case 200:
			gBSGround[0]->m_BallIndex = aIndex;
			break;

		case 251:
			lpObj->ShopNumber = 0;
			break;

		case 255:
			lpObj->ShopNumber = 1;
			break;

		case 254:
			lpObj->ShopNumber = 2;
			break;

		case 250:
			lpObj->ShopNumber = 3;
			break;

		case 248:
			lpObj->ShopNumber = 4;
			break;

		case 239:
		case 253:
			lpObj->ShopNumber = 5;
			break;

		case 244:
			lpObj->ShopNumber = 6;
			break;

		case 245:
			lpObj->ShopNumber = 7;
			break;

		case 246:
			lpObj->ShopNumber = 8;
			break;

		case 243:
			lpObj->ShopNumber = 9;
			break;

		case 242:
			lpObj->ShopNumber = 10;
			break;

		case 231:
			lpObj->ShopNumber = 12;
			break;

		case 230:
			lpObj->ShopNumber = 11;
			break;

		case 240:
			lpObj->ShopNumber = 100;
			break;

		case 238:
			lpObj->ShopNumber = 101;
			break;

		case 236:
			lpObj->ShopNumber = 102;
			break;
		case 259:
			lpObj->ShopNumber = 5;
			break;
		case 368:
		case 369:
		case 370:
			lpObj->ShopNumber = 101;
			break;
			//Season2.5 add-on
		case 376: //"Pamela the Supplier"
			lpObj->ShopNumber = 5;
			break;
		case 377: //"Angela the Supplier"
			lpObj->ShopNumber = 5;
			break;
		case 379: //"Natasha the Firecracker Merchant"
			lpObj->ShopNumber = 13;
			break;
		case 415:
			lpObj->ShopNumber = 15;
			break;
		case 416:
			lpObj->ShopNumber = 16;
			break;
		case 417:
			lpObj->ShopNumber = 17;
			break;
		case 450:
			lpObj->ShopNumber = 101;
			break;
		//season4 add-on
		case 452:
			lpObj->ShopNumber = 101;
			break;
		case 453:
			lpObj->ShopNumber = 101;
			break;
		case 478:
			lpObj->ShopNumber = 101;
			break;
		case 492:
			lpObj->ShopNumber = 18;
			break;
		//1.01.00
		case 545:
			lpObj->ShopNumber = 15;
			break;
		case 577:
			lpObj->ShopNumber = 15;
			break;
		case 578:
			lpObj->ShopNumber = 19;
			break;
	}

	lpObj->Connected = PLAYER_PLAYING;
	lpObj->Live = FALSE;
	lpObj->m_State = 0;
	
	if (MonsterClass == 44 || MonsterClass == 54 || 
		MonsterClass == 53 || lpObj->Class == 55 || 
		lpObj->Class == 56 || MonsterClass == 76 || 
		MonsterClass == 78 || MonsterClass == 79 || 
		MonsterClass == 80 || MonsterClass == 81 ||	
		MonsterClass == 82 || MonsterClass == 83 ||
		//season 4.5 add-on
		MonsterClass == 493 || MonsterClass == 494 ||
		MonsterClass == 495 || MonsterClass == 496 ||
		MonsterClass == 497 || MonsterClass == 498 ||
		MonsterClass == 499 || MonsterClass == 500 ||
		MonsterClass == 501 || MonsterClass == 502)
	{

	}
	else
	{
		lpObj->Live = TRUE;
		lpObj->m_State = 1;
	}

	if( MonsterClass == 365 ) //Season 2.5 add-on
	{
		if(g_bNewYearLuckyBagMonsterEventOn != FALSE)
		{
			lpObj->Live = TRUE;
			lpObj->m_State = 1;
		}
		else
		{
			lpObj->Live = FALSE;
			lpObj->m_State = 0;
		}
	}

	if ( MonsterClass >= 204 && MonsterClass <= 259	|| 
		 MonsterClass >= 368 && MonsterClass <= 370	|| 
//#if(_GSCS==0)
		MonsterClass == 367	||
//#endif
		MonsterClass == 375 || 
		MonsterClass == 406 || MonsterClass == 407 || MonsterClass == 408 || //Season 2.5 add-on
		//Season4 add-on
		MonsterClass == 465 || MonsterClass == 467 || (MonsterClass >= 468 && MonsterClass <= 475) || 
		MonsterClass == 492)
	{
		lpObj->Type = OBJ_NPC;
	}
	else
	{
		lpObj->Type = OBJ_MONSTER;
	}

	if( MonsterClass == 376 || MonsterClass == 377 ) //Season 2.5 add-on
	{
		lpObj->Type = OBJ_NPC;
	}

	if( MonsterClass == 452 || MonsterClass == 453 ) //Season 4.5 add-on
	{
		lpObj->Type = OBJ_NPC;
	}

	if( MonsterClass == 379 ) //Season 2.5 add-on
	{
		lpObj->Type = OBJ_NPC;
	}

	if( MonsterClass == 379 ) //Season 3.5 add-on looooooooooooool again
	{
		lpObj->Type = OBJ_NPC;
	}

	if( MonsterClass == 385 || //Season 2.5 add-on 
		MonsterClass == 380 || 
		MonsterClass == 381 || 
		MonsterClass == 382 || 
		MonsterClass == 383 || 
		MonsterClass == 384 ||  //Season 4.6 addon
		MonsterClass == 479 
		|| (MonsterClass >= 541 && MonsterClass <= 547) ||  MonsterClass == 414 // Season 5 Episode 2 JPN
		) 

	{
		lpObj->Type = OBJ_NPC;
	}

	if( MonsterClass == 385 ) //Season 2.5 add-on
	{
		g_IllusionTempleEvent.SetMirageIndex(aIndex);
	}

	if( MonsterClass == 415 || //Season 3.5 add-on 
		MonsterClass == 416 || 
		MonsterClass == 417 )
	{
		lpObj->Type = OBJ_NPC;
	}

	if( MonsterClass == 450 ) //Season 3.5 add-on 
	{
		lpObj->Type = OBJ_NPC;
	}

	if( MonsterClass == 478 ) //Season 4.5 add-on 
	{
		lpObj->Type = OBJ_NPC;
	}

	if (MonsterClass == 479 ) //Season 4.6 add-on 
	{
		lpObj->Type = OBJ_NPC;
	}

	if (MonsterClass >= 523 && MonsterClass < 528)
	{
		lpObj->Type = OBJ_MONSTER;
	}

	if (MonsterClass == 522)
	{
		lpObj->Type = OBJ_NPC;
	}

	if (MonsterClass == 540 || MonsterClass == 541 || MonsterClass == 542)
	{
		lpObj->Type = OBJ_NPC;
	}

	if (MonsterClass == 543 || MonsterClass == 544)
	{
		lpObj->Type = OBJ_NPC;
	}
	//1.01.00
	if (MonsterClass == 566 || MonsterClass == 567 || MonsterClass == 568)	
	{
		lpObj->Type = OBJ_NPC;
	}

	if (MonsterClass == 545 || MonsterClass == 546 || MonsterClass == 547)
	{
		lpObj->Type = OBJ_NPC;
	}

	if (MonsterClass == 577 || MonsterClass == 578)
	{
		lpObj->Type = OBJ_NPC;
	}

	if (MonsterClass == 579)
	{
		lpObj->Type = OBJ_NPC;
	}


	if ( MonsterClass == 77 )
	{
		int iSL = gObjAddMonster(MAP_INDEX_ICARUS);

		if ( iSL >= 0 )
		{
			if ( gObjSetMonster(iSL, 76) == FALSE )
			{
				MsgBox("?µ°??????? ???¤ ????");
				return false;
			}

			lpObj->m_SkyBossMonSheildLinkIndex = iSL;
			//LogAddTD("SkyLand Boss Monster Setting!!");
		}
		else
		{
			MsgBox("?µ°??????? ???¤ ????");
			return false;
		}
	}

	if ( MonsterClass == 76 )
	{
		//LogAddTD("SkyLand Boss Shield Setting!!");
	}

	lpObj->Class = MonsterClass;
	Level = lpObj->Level;
	LPMONSTER_ATTRIBUTE lpm = gMAttr.GetAttr( lpObj->Class);

	if ( lpm == NULL )
	{
		MsgBox(lMsg.Get(MSGGET(1, 253) ), lpObj->Class, __FILE__, __LINE__);
		return false;
	}

	strncpy(gObj[aIndex].Name, lpm->m_Name , 10 );

	if (lpm->m_Level == 0)
	{
		LogAdd(lMsg.Get( MSGGET(1, 254)), lpObj->Class, __FILE__, __LINE__);
	}

	lpObj->Level = lpm->m_Level;
	lpObj->m_AttackSpeed = lpm->m_AttackSpeed;
	lpObj->m_AttackDamageMin = lpm->m_DamageMin;
	lpObj->m_AttackDamageMax = lpm->m_DamageMax;
	lpObj->m_Defense = lpm->m_Defense;
	lpObj->m_MagicDefense = lpm->m_MagicDefense;
	lpObj->m_AttackRating = lpm->m_AttackRating;
	lpObj->m_SuccessfulBlocking = lpm->m_Successfulblocking;
	lpObj->Life = lpm->m_Hp;
	lpObj->MaxLife = lpm->m_Hp;
	lpObj->Mana = lpm->m_Mp;
	lpObj->MaxMana = lpm->m_Mp;
	lpObj->m_MoveRange = lpm->m_MoveRange;
	lpObj->m_AttackSpeed = lpm->m_AttackSpeed;
	lpObj->m_MoveSpeed = lpm->m_MoveSpeed;
	lpObj->MaxRegenTime = lpm->m_RegenTime * 1000;
	lpObj->m_AttackRange = lpm->m_AttackRange;
	lpObj->m_ViewRange = lpm->m_ViewRange;
	lpObj->m_Attribute = lpm->m_Attribute;
	lpObj->m_AttackType = lpm->m_AttackType;
	lpObj->m_ItemRate = lpm->m_ItemRate;
	lpObj->m_MoneyRate = lpm->m_MoneyRate;
	lpObj->Dexterity = 0;

	lpObj->m_iScriptMaxLife = lpm->m_iScriptHP;
	lpObj->m_iBasicAI = lpm->m_iAINumber;
	lpObj->m_iCurrentAI = lpm->m_iAINumber;
	lpObj->m_iCurrentAIState = 0;
	lpObj->m_iLastAIRunTime = 0;
	lpObj->m_iGroupNumber = 0;
	lpObj->m_iSubGroupNumber = 0;
	lpObj->m_iGroupMemberGuid = -1;
	lpObj->m_iRegenType = 0;

	lpObj->m_Agro.ResetAll();

	lpObj->m_iLastAutomataRuntime = 0;
	lpObj->m_iLastAutomataDelay = 0;

	lpObj->pInventoryCount[0] = 0;
	memcpy(lpObj->m_Resistance, lpm->m_Resistance, MAX_RESISTENCE_TYPE );
	gObjSetInventory1Pointer(lpObj);

	if ( lpObj->m_AttackType != 0 )
	{
#ifdef DP
		if( CDoppelganger::IsEventMap(lpObj->MapNumber) && lpObj->Type == OBJ_MONSTER )
		{
			gObjMonsterMagicAdd(lpObj, (BYTE)lpObj->m_AttackType, 1);
		}
		else if ( lpObj->m_AttackType >= 100 )
#else
		if ( lpObj->m_AttackType >= 100 )
#endif
		{
			gObjMonsterMagicAdd(lpObj, (BYTE)(lpObj->m_AttackType-100), 1);
		}
		else
		{
			gObjMonsterMagicAdd(lpObj, (BYTE)lpObj->m_AttackType, 1);
		}

		if ( lpObj->Class == 144 || lpObj->Class == 174 || lpObj->Class == 182 || lpObj->Class == 190 || lpObj->Class == 260 || lpObj->Class == 268 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}

		if ( lpObj->Class == 145 
			|| lpObj->Class == 175 
			|| lpObj->Class == 183 
			|| lpObj->Class == 191 
			|| lpObj->Class == 261 
			|| lpObj->Class == 269 
			|| lpObj->Class == 146 
			|| lpObj->Class == 176 
			|| lpObj->Class == 184 
			|| lpObj->Class == 192 
			|| lpObj->Class == 262 
			|| lpObj->Class == 270 
			|| lpObj->Class == 147 
			|| lpObj->Class == 177 
			|| lpObj->Class == 185 
			|| lpObj->Class == 193 
			|| lpObj->Class == 263 
			|| lpObj->Class == 271 
			|| lpObj->Class == 148
			|| lpObj->Class == 178
			|| lpObj->Class == 186
			|| lpObj->Class == 194
			|| lpObj->Class == 264
			|| lpObj->Class == 272
			|| lpObj->Class == 160
			|| lpObj->Class == 180
			|| lpObj->Class == 188
			|| lpObj->Class == 196
			|| lpObj->Class == 266
			|| lpObj->Class == 274 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}

		if ( lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 275 )
		{
			gObjMonsterMagicAdd(lpObj, 200, 1);
			gObjMonsterMagicAdd(lpObj, 17, 1);
			gObjMonsterMagicAdd(lpObj, 1, 1);
			gObjMonsterMagicAdd(lpObj, 201, 1);
			gObjMonsterMagicAdd(lpObj, 202, 1);
			gObjMonsterMagicAdd(lpObj, 55, 1);
		}

		if (  lpObj->Class == 149 || lpObj->Class == 179 || lpObj->Class == 187 || lpObj->Class == 195 || lpObj->Class == 265 || lpObj->Class == 273 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
			gObjMonsterMagicAdd(lpObj, 1, 1);
		}

		if ( lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77 || lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143 || lpObj->Class == 433 || lpObj->Class == 163 || lpObj->Class == 165 || lpObj->Class == 167 || lpObj->Class == 169 || lpObj->Class == 171 || lpObj->Class == 173 || lpObj->Class == 427)
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}
		
		if( lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143 || lpObj->Class == 433)
		{
			gObjMonsterMagicAdd(lpObj, 3, 1);
		}

		if( lpObj->Class == 561 )	//Medusa, 1.01.00
		{
			gObjMonsterMagicAdd(lpObj, 238, 1);
			gObjMonsterMagicAdd(lpObj, 237, 1);
			gObjMonsterMagicAdd(lpObj, 38, 1);
			gObjMonsterMagicAdd(lpObj, 9, 1);
		}
	}

	gObjGiveItemSearch(lpObj, lpm->m_MaxItemLevel);
	gObjGiveItemWarehouseSearch(lpObj, lpm->m_MaxItemLevel);
	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->LastAttackerID = -1;
	return true;
}

void gObjDestroy(SOCKET aSocket,int client)
{
	if ( client < 0 || client > OBJMAX-1)
	{
		return;
	}

	gObj[client].Connected = PLAYER_EMPTY;
}

short gObjAddSearch(SOCKET aSocket, char* ip)
{
	int count;
	int totalcount = 0;

	if ( gDisconnect == 1 )
	{
		return -1;
	}

	if ( gObjTotalUser > gServerMaxUser )
	{
		if (acceptIP.IsIp(ip) == 0 )
		{
			GCJoinSocketResult(4, aSocket);
			return -1;
		}
	}

	count = gObjCount;
	
	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			return count;
		}

		count++;

		if ( count >= OBJMAX )
		{
			count = OBJ_STARTUSERINDEX;
		}
		totalcount++;
		if ( totalcount >= OBJMAXUSER )
		{
			break;
		}
	}
	return -1;
}

short gObjAdd(SOCKET aSocket, char* ip, int aIndex)
{
	if ( gObj[aIndex].Connected != PLAYER_EMPTY )
	{
		return -1;
	}

	gObjCharZeroSet(aIndex);
	gNSerialCheck[aIndex].init();
	gObj[aIndex].LoginMsgSnd = FALSE;
	gObj[aIndex].LoginMsgCount = 0;
	memset(gObj[aIndex].AccountID, 0, sizeof(gObj[0].AccountID)-1);
	gObj[aIndex].m_Index = aIndex;
	gObj[aIndex].m_socket = aSocket;
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].AutoSaveTime = gObj[aIndex].ConnectCheckTime;
	gObj[aIndex].Connected = PLAYER_CONNECTED;
	gObj[aIndex].CheckSpeedHack = false;
	gObj[aIndex].LoginMsgCount = 0;
	gObj[aIndex].Magumsa = 0;
	gObj[aIndex].Type = OBJ_USER;
	gObj[aIndex].m_kRecvPacketStatistics.Init(1000);
	gObj[aIndex].SaveTimeForStatics = GetTickCount() + 3600000;
#ifdef PCBANG
	gObj[aIndex].m_iPcBangConnectionType = 0; //season4.5 add-on
	gObj[aIndex].m_PcBangPointSystem.RESET(); //season 4.5 add-on
#endif
	strcpy(gObj[aIndex].Ip_addr, ip);
	LogAddTD("connect : [%d][%s]", aIndex, ip);
	gObjCount++;
	
	if ( gObjCount >= OBJMAX )
	{
		gObjCount = OBJ_STARTUSERINDEX;
	}

	return aIndex;
}

#ifdef PCBANG
void PCBANG_POINT_SYSTEM::RESET()
{
	this->m_bPcBangPointEnable = 0;
	this->m_bPcBangCommonRule = 0;
	this->m_sPcBangCommonRuleTime = g_sPCBangFirstRuleTime;
	this->m_sPcBangGainPoint = g_sPCBangFirstRuleTimePoint;
	this->m_iPcBangAccumulatedPoint = 0;
	this->m_bPcBangInfoSet = 0;
	this->m_dwPcBangPointTick = 0;
	this->m_sPcBangResetYear = 2001;
	this->m_sPcBangResetMonth = 1;
	this->m_sPcBangResetDay = 1;
	this->m_sPcBangResetHour = 0;
}
#endif

short gObjMonsterRecall(int iMapNumber)
{
	int number = gObjAddMonster(iMapNumber);

	if ( number < 0 )
	{
		return -1;
	}
	return -1;
}

short gObjAddMonster(int iMapNumber)
{
	if (g_MapServerManager.CheckMapCanMove(iMapNumber) == FALSE )
	{
		return -1;
	}

	int count;

	//EnterCriticalSection(&m_critAddMonster);

	int totalcount=0;
	count = gObjMonCount;

	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index = count;
			gObj[count].Connected = PLAYER_CONNECTED;

			gObjMonCount++;

			if( gObjMonCount < count )
			{
				gObjMonCount = count + 1;
			}

			if ( gObjMonCount >= OBJ_MAXMONSTER )
			{
				gObjMonCount = 0;
			}

			//LeaveCriticalSection(&m_critAddMonster);
			return count;
		}

		count ++;
		
		if ( count >= OBJ_MAXMONSTER )
		{
			count = 0;
		}

		totalcount++;

		if ( totalcount >= OBJ_MAXMONSTER )
		{
			//LeaveCriticalSection(&m_critAddMonster);
			LogAdd( lMsg.Get( MSGGET ( 1, 255 )), __FILE__, __LINE__ );
			return -1;
		}
	}

	//LeaveCriticalSection(&m_critAddMonster);
	return -1;
}

short gObjAddCallMon()
{
	int count;
	int totalcount = 0;

	count = gObjCallMonCount;

	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index = count;
			gObj[count].Connected = PLAYER_CONNECTED;
			gObjCallMonCount++;

			if( gObjCallMonCount < count )
			{
				gObjCallMonCount = count + 1;
			}

			if ( gObjCallMonCount >= OBJ_STARTUSERINDEX )
			{
				gObjCallMonCount = OBJ_MAXMONSTER;

			}
			return count;
		}

		count++;

		if ( count >= OBJ_STARTUSERINDEX )
		{
			count = OBJ_MAXMONSTER;
		}

		totalcount++;

		if ( totalcount >= OBJ_CALLMONCOUNT )
		{
			LogAdd(lMsg.Get(MSGGET(1, 255)), __FILE__, __LINE__);
			return -1;
		}
	}

	return -1;

}

void gObjUserKill(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->CloseCount < 1 )
	{
		lpObj->CloseCount = 6;
		lpObj->CloseType = 0;
	}
}

void gObjAllLogOut()
{
	static BOOL bAllLogOut = FALSE;

	if ( bAllLogOut == 0 )
	{
		bAllLogOut = 1;
		gObjAllDisconnect();
		Sleep(1000);
		LogAddC(2, lMsg.Get( MSGGET(2, 0)));
	}
}

void gObjAllDisconnect()
{
	gDisconnect = TRUE;

	for ( int n=OBJ_STARTUSERINDEX; n< OBJMAX; n++)
	{
		if ( gObj[n].Connected > PLAYER_EMPTY )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				CloseClient(n);
			}
		}
	}
}

void gObjTradeSave(LPOBJ lpObj, int index)
{
	if ( lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 1 )
	{
		int tObjNum = lpObj->TargetNumber;

		if ( tObjNum > 0 )
		{
			char szTemp[256];

			CGTradeResult(tObjNum, 0);
			wsprintf(szTemp, lMsg.Get(MSGGET(4, 77)), lpObj->Name);
			GCServerMsgStringSend((char*)szTemp, tObjNum, 1);
			gObjTradeCancel(tObjNum);
			LogAdd(lMsg.Get(MSGGET(2, 1)), gObj[tObjNum].AccountID, gObj[tObjNum].Name);
		}

		gObjTradeCancel( index );
		LogAdd(lMsg.Get(MSGGET(2,2)), lpObj->AccountID, lpObj->Name);
	}
}

short gObjMemFree(int index)
{
	int i = 0;
	LPOBJ lpObj;

	if ( gDisconnect == TRUE )
	{
		return -1;
	}

	if ( index < 0 || index > OBJMAX )
	{
		LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return false;
	}

	lpObj = &gObj[index];

	if ( lpObj->Connected < PLAYER_CONNECTED )
	{
		return false;
	}

	gObjTradeSave(lpObj, index);

	if ( lpObj->Connected >= PLAYER_LOGGED )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Connected == PLAYER_PLAYING )
			{
				if (  BC_MAP_RANGE(lpObj->MapNumber) )
				{
					g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
				}

				if (  IT_MAP_RANGE(lpObj->MapNumber) )
				{
					g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
					g_IllusionTempleEvent.LeaveIllusionTemple(lpObj->m_Index, lpObj->MapNumber);
				}
#ifdef DP
				if( g_DoppleganerEvent.IsEventMap(lpObj->MapNumber) )
				{
					g_DoppleganerEvent.LeaveDoppelgangerEvent(lpObj->m_Index);

					if( g_DoppleganerEvent.GetDoppelgangerState() == 2 )
					{
						g_DoppleganerEvent.SendDoppelgangerResult(lpObj, 1);
					}
				}
#endif
				GJSetCharacterInfo(lpObj, index, 0, 0);
			}
			
			GJPUserClose(lpObj);
		}
	}

	gObjViewportClose(lpObj);
	lpObj->Connected = PLAYER_EMPTY;
	return 1;
}

BOOL gObjGameClose(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX )
	{
		LogAdd("(%s)(%d) = index over error (%d)", 
			__FILE__, 
			__LINE__, 
			aIndex);

		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->IsOffTrade=0;

	if ( lpObj->Connected != PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		LogAddTD(lMsg.Get(MSGGET(2, 3)), 
			lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

#ifdef PCBANG
	lpObj->m_PCBangPointTimer.Clear(); //season4.5 add-on
#endif

	if ( lpObj->m_RecallMon >= 0 )
	{
		gObjMonsterCallKill(lpObj->m_Index);
	}

	gObjMagicTextSave(lpObj);

	if ( (   (lpObj->m_IfState.use != 0 )
		&&  (lpObj->m_IfState.type == 7 )) 
		|| ((lpObj->m_IfState.use != 0)  
		&& (lpObj->m_IfState.type == 13)))
	{
		if ( lpObj->bIsChaosMixCompleted == 1 || 
			lpObj->m_bIsCastleNPCUpgradeCompleted == 1)
		{
			gObjInventoryCommit(aIndex);
		}
		else
		{
			gObjInventoryRollback(aIndex);
		}
	}

	gObjTradeSave(lpObj, aIndex);

	if ( (lpObj->m_IfState.use != 0 )
		&&  (lpObj->m_IfState.type == 10 ) 
		&& lpObj->TargetShopNumber == 229)
	{
		gQeustNpcTeleport.TalkRefDel();
	}

	if( (lpObj->m_IfState.use != 0 ) && lpObj->TargetShopNumber == 568 )	//1.01.00
	{
		gQeustNpcTeleport.TalkRefDel();
	}

	if ( CheckAuthorityCondition(98, lpObj) == true) //Season 4.5 changed
	{
		cManager.ManagerDel(lpObj->Name);
	}

#ifndef NPVP
	if ( OBJMAX_RANGE(lpObj->m_iDuelUserRequested) )
	{
		gObjSendDuelEnd(&gObj[lpObj->m_iDuelUserRequested]);
	}


	if ( gObjDuelCheck(lpObj) != FALSE )
	{
		gObjSendDuelEnd(&gObj[lpObj->m_iDuelUser]);
	}
#else
	g_NewPVP.Reset(*lpObj); //1.00.92
#endif
	gObjResetDuel(lpObj);

	/*if ( lpObj->PartyNumber >= 0 )
	{
		int iPartyIndex= gParty.GetIndex(lpObj->PartyNumber, 
			lpObj->m_Index, lpObj->DBNumber);

		if ( iPartyIndex >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;

			pMsg.Number = iPartyIndex;

			CGPartyDelUser(&pMsg, lpObj->m_Index);
		}
	}*/
	if( lpObj->PartyNumber >= 0 )	//1.01.00
	{
		int pindex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
		// ----
		if( pindex >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;
			// ----
			if ( lpObj->m_bMapSvrMoveQuit == 1 )
			{
				pMsg.Number = pindex;
				CGPartyDelUser(&pMsg, lpObj->m_Index);
			}
			else
			{
				if( lpObj->bTemporaryUser )
				{
					if( gParty.GetReallyConnectPartyMemberCount(lpObj->PartyNumber) <= 1 )
					{
						CTemporaryUserManager::Instance()->DeleteAllPartyUserData(lpObj->PartyNumber);
					}
					else
					{
						CTemporaryUserManager::Instance()->AddUserData(lpObj->Name, lpObj->m_Index, lpObj->PartyNumber);
					}
					// ----
					pMsg.Number = pindex;
					CGPartyDelUserAsExitGameByForce(&pMsg, lpObj->m_Index);
				}
				else
				{
					pMsg.Number = pindex;

					if (	g_DoppleganerEvent.IsEventMap(lpObj->MapNumber)
						||	g_ImperialGuardian.IsEventMap(lpObj->MapNumber)
						||	lpObj->MapNumber >= 45 && lpObj->MapNumber <= 50 )
					{
						CGPartyDelUserAsExitGameByForce(&pMsg, lpObj->m_Index);
					}
					else
					{
						CGPartyDelUser(&pMsg, lpObj->m_Index);
					}
				}
			}
		}
	}
	// ----
	if ( lpObj->GuildNumber > 0 )
	{
		if ( lpObj->lpGuild->WarState == 1 )
		{
			gObjGuildWarMasterClose(lpObj);
		}

		GDCharClose(0, lpObj->lpGuild->Name, lpObj->Name);
	}
	else
	{
		GDCharClose(1, NULL, lpObj->Name);
	}

	if ( lpObj->WarehouseSave == 1 )
	{
		GDSetWarehouseList(lpObj->m_Index);
	}

	if ( BC_MAP_RANGE(lpObj->MapNumber) )
	{
		g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
	}

	if( IT_MAP_RANGE(lpObj->MapNumber) )
	{
		g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
		g_IllusionTempleEvent.LeaveIllusionTemple(lpObj->m_Index, lpObj->MapNumber);
	}

	gObjSaveChaosBoxItemList(lpObj);

	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		LogAdd("[Fix Inv.Ptr] False Location - %s, %d", 
			__FILE__, 
			__LINE__);
	}

	g_CashItemPeriodSystem.ClearPeriodItemEffect(lpObj,-1);

	gObjClearBuffEffect(lpObj, CLEAR_TYPE_LOGOUT);

	GJSetCharacterInfo(lpObj, aIndex, 0, 0);
	gObjViewportClose(lpObj);
	LogAddTD(lMsg.Get(MSGGET(2, 4)), 
		lpObj->m_Index, 
		lpObj->AccountID, lpObj->Name);

	if(lpObj->Name)
	{
		WhisperCash.DelCash(lpObj->Name);
	}

	if(lpObj->m_iPeriodItemEffectIndex != -1)
	{
		g_CashItemPeriodSystem.ClearPeriodItemEffect(lpObj,-1);
	}

#ifdef PERIOD
	g_PeriodItemEx.RemovePeriodInfo(lpObj);
#endif

	memcpy(lpObj->BackName, lpObj->Name, sizeof(lpObj->Name)-1);
	lpObj->BackName[10] = 0;


	memset(lpObj->Name, 0, sizeof(lpObj->Name)-1);
	lpObj->Connected = PLAYER_LOGGED;

	return TRUE;
}

short gObjDel(int index)
{
	int i = 0;
	
	if ( index < 0 || index > OBJMAX )
	{
		LogAdd ("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}

	LPOBJ lpObj = &gObj[index];
	
	if ( lpObj->Connected < PLAYER_CONNECTED )
	{
		return 0;
	}

	if ( MAX_MAP_RANGE(lpObj->MapNumber) )
	{
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->X, lpObj->Y);
	}

	int UserConnectState = lpObj->Connected;

	if ( lpObj->Type == OBJ_USER )
	{
		gObjGameClose(index);

		if ( UserConnectState >= PLAYER_LOGGED )
		{
			if ( lpObj->m_bMapSvrMoveQuit == false )
			{
				GJPUserClose(lpObj);
				LogAddTD("(%d)logout-send : %s [%s]", index, lpObj->AccountID, lpObj->Ip_addr);
			}
			else
			{
				LogAddTD("[MapServerMng] User Quit without JoinServer logout-send [%s][%s]",
					lpObj->AccountID, lpObj->Name);
			}

			memset(lpObj->AccountID, 0, sizeof(lpObj->AccountID) -1);
			memset(lpObj->LastJoominNumber, 0, sizeof(lpObj->LastJoominNumber));
		}

		LogAddTD("(%d)logout : %s [%s]", index, lpObj->AccountID, lpObj->Ip_addr);
		IpCache.DelIp(lpObj->Ip_addr);
	}

	lpObj->Connected = PLAYER_EMPTY;

	lpObj->m_bSkillKeyRecv = 0; //Season 2.5 add-on
	lpObj->m_bGMSummon = 0; //Season 4.5 add-on
	return 1;
}

short gObjSometimeClose(int index)
{
	if ( gDisconnect == 1 )
	{
		return -1;
	}

	if ( index < 0 || index > OBJMAX )
	{
		LogAdd("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}

	LPOBJ lpObj = &gObj[index];
	gObjTradeSave(lpObj, index);

	if ( lpObj->Connected > PLAYER_CONNECTED )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Connected == PLAYER_PLAYING )
			{
				if ( lpObj->PartyNumber >= 0 )
				{
					PMSG_PARTYDELUSER pMsg;
					int iPartyIndex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);

					if ( iPartyIndex >= 0 )
					{
						pMsg.Number = iPartyIndex;
						CGPartyDelUser(&pMsg, lpObj->m_Index);
					}
				}
			}
		}
	}

	return 1;
}

BOOL gObjIsGamePlaing(LPOBJ lpObj)
{
	if ( lpObj->Connected < PLAYER_PLAYING )
	{
		return false;
	}

	return TRUE;
}

BOOL gObjIsConnectedGP(int aIndex)
{
	if (aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return FALSE;
	}
	
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected < PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL gObjIsConnectedGP(int aIndex, char * CharName)
{
	if (aIndex < 0 || aIndex > OBJMAX-1 )
	{
		return FALSE;
	}
	
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected < PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return FALSE;
	}

	if ( strcmp(gObj[aIndex].Name, CharName) != 0 )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL gObjIsConnected(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->Connected < PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( lpObj->CloseCount > 0 )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL gObjIsConnected(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		return FALSE;
	}
	if ( gObj[aIndex].Connected < PLAYER_PLAYING )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL gObjIsConnected(LPOBJ lpObj, int dbnumber)
{
	if (lpObj->Connected < PLAYER_PLAYING)
	{
		return 0;
	}

	if (lpObj->DBNumber != dbnumber)
	{
		return 0;
	}

	return 1;
}

BOOL gObjIsConnectedEx(int aIndex)
{
	if( aIndex < 0 || aIndex > OBJMAX-1 ) return FALSE;
	LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[aIndex];
	if( lpObj->Type != OBJ_USER)	return FALSE;
	if( lpObj->Connected < PLAYER_PLAYING ) return FALSE;
	if( lpObj->CloseCount >= 0 )	return FALSE;
	return TRUE;
}

BOOL gObjIsAccontConnect(int aIndex, char* accountid)
{
	if (aIndex <0 || aIndex > OBJMAX-1)
	{
		return 0;
	}

	if (gObj[aIndex].Connected< PLAYER_CONNECTED)
	{
		return 0;
	}

	if (strlen(&gObj[aIndex].AccountID[0]) < 1 )
	{
		return 0;
	}

	if ( strcmp(accountid, &gObj[aIndex].AccountID[0]) != 0)
	{
		return 0;
	}
	return 1;
}

BOOL gObjJoominCheck(int aIndex, char* szInJN)
{
	LPOBJ lpObj = &gObj[aIndex];
	
	switch ( gLanguage )
	{
		case 1:
		case 5:
		case 6:
		case 8:
			if ( strncmp(szInJN, lpObj->LastJoominNumber, 7) == 0 )
			{
				return TRUE;
			}
		case 2:
			if ( strncmp(szInJN, &lpObj->LastJoominNumber[5], 8) == 0 )
			{
				return TRUE;
			}
		case 3:
			if ( strcmpi(szInJN, &lpObj->LastJoominNumber[6]) == 0 )
			{
				return TRUE;
			}
			break;
		case 4:
			if ( strlen(szInJN) < 2 )
			{
				return FALSE;
			}

			if ( (szInJN[0] >= 'A' && szInJN[0] <= 'Z') || (szInJN[0] >= 'a' && szInJN[0] <= 'z') )
			{
				if ( szInJN[0] >= 'A' && szInJN[0] <= 'Z' )
				{
					szInJN[0] = szInJN[0] + 32;
				}

				if ( lpObj->LastJoominNumber[0] >= 'A' && lpObj->LastJoominNumber[0] <= 'Z' )
				{
					lpObj->LastJoominNumber[0] = lpObj->LastJoominNumber[0] + 32;
				}

				if ( strncmp(lpObj->LastJoominNumber, szInJN, 10) == 0 )
				{
					return TRUE;
				}
			}

			break;

		default:

			if ( strncmp(szInJN, &lpObj->LastJoominNumber[6], 7) == 0 )
			{
				return TRUE;
			}
	}

	return FALSE;
}

BOOL gObjTaiwanJoominCheck(int aIndex, char* szInJN)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( strlen(szInJN) < 2 )
	{
		return FALSE;
	}
	
	if ( strncmp(&lpObj->LastJoominNumber[1], szInJN, 9) == 0 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjCheckXYMapTile(LPOBJ lpObj, int iDbgName)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( (lpObj->Authority&2 ) == 2 )
	{
		return FALSE;
	}

	if ( CC_MAP_RANGE(lpObj->MapNumber) )
	{
		if ( g_ChaosCastle.GetCurrentState(g_ChaosCastle.GetChaosCastleIndex(lpObj->MapNumber)) == 2 )
		{
			return FALSE;
		}
	}

	int x = lpObj->X;
	int y = lpObj->Y;
	int mapnumber = lpObj->MapNumber;

	if ( mapnumber > MAX_NUMBER_MAP-1 )
	{
		mapnumber = MAP_INDEX_RORENCIA;
	}

	int mapminlevel = MapMinUserLevel[mapnumber];
	mapminlevel = 0;

	if ( lpObj->Class == CLASS_DARKLORD 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif
		|| lpObj->Class == CLASS_MAGUMSA )
	{
		if ( mapminlevel > 0 )
		{
			if ( MapMinUserLevel[mapnumber] > 0 )
			{
				mapminlevel = MapMinUserLevel[mapnumber] - (MapMinUserLevel[mapnumber] / 3 * 2);
			}
		}
	}

	/*if ( lpObj->Class == 2 && lpObj->Level < 10 )
	{
		if ( lpObj->MapNumber != MAP_INDEX_NORIA )
		{
			LogAddTD(lMsg.Get(MSGGET(2, 5)), lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			PMSG_TELEPORT pMsg;
			pMsg.MoveNumber = 27;
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
		}
	}
	else
	{*/
		if( mapminlevel > lpObj->Level )
		{
			LogAddTD(lMsg.Get(MSGGET(2, 5)), lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			PMSG_TELEPORT pMsg;
			if( lpObj->Class == 2 )
			{
				pMsg.MoveNumber = 27;
			}
			else
			{
				pMsg.MoveNumber = 17;
			}
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
		}
	//}

	BYTE attr = MapC[mapnumber].GetAttr(x, y);

	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		BYTE attr2 = MapC[mapnumber].GetAttr(x+2, y);
		BYTE attr3 = MapC[mapnumber].GetAttr(x, y+2);
		BYTE attr4 = MapC[mapnumber].GetAttr(x-2, y);
		BYTE attr5 = MapC[mapnumber].GetAttr(x, y-2);

		if ( attr2 > 1 && attr3 > 1 && attr4 > 1 && attr5 > 1 )
		{
			LogAddTD("[ CHECK POSITION ] DbgName[%d] [%s][%s] Map[%d]-(%d,%d) Invalid location causes to force to move", iDbgName,lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			
			if( IT_MAP_RANGE(lpObj->MapNumber) != 0 )
			{
				if( g_IllusionTempleEvent.GetIllusionTempleState(lpObj->MapNumber) == 2 )
				{
					BYTE loc14 = g_IllusionTempleEvent.GetUserTeam(lpObj->MapNumber, lpObj->m_Index);

					PMSG_TELEPORT pMsg;

					if(loc14 == (BYTE)-1)
					{
						pMsg.MoveNumber = 17;
					}
					else
					{
						if(loc14 == 0)
						{
							pMsg.MoveNumber = lpObj->MapNumber + 103;
						}
						else if(loc14 == 1)
						{
							pMsg.MoveNumber = lpObj->MapNumber + 109;
						}
						else
						{
							pMsg.MoveNumber = 17;
						}
					}
					gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
					return TRUE;
				}
			}

			PMSG_TELEPORT pMsg;
			pMsg.MoveNumber = 17;
			gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);

			return TRUE;
		}
	}

	return FALSE;

}

//0051CAB0  /> \55            PUSH EBP
BOOL gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber,int ukn_30, char* JoominNumber,BYTE PcBangRoom)
{
	if ( !OBJMAX_RANGE(aIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return 0;
	}

	if ( gObj[aIndex].Connected != PLAYER_CONNECTED )
	{
		LogAddTD(lMsg.Get(MSGGET(2, 6)), aIndex, szId, aDBNumber, __FILE__, __LINE__);
		return 0;
	}

	if ( strcmp(gObj[aIndex].AccountID, szId) != 0 )
	{
		LogAdd(lMsg.Get(MSGGET(2, 7)), szId);
		return 0;
	}

	gObj[aIndex].UserNumber = aUserNumber;
	gObj[aIndex].DBNumber = aDBNumber;
	gObj[aIndex].Connected = PLAYER_LOGGED;
	strcpy(gObj[aIndex].LastJoominNumber, JoominNumber);

	 //Season 4.5 addon start
	gObj[aIndex].ukn_30 = ukn_30;
#ifdef PCBANG
	gObj[aIndex].m_iPcBangRoom = PcBangRoom;
	g_PCBangPointSystem.SetPointUser(aIndex,PcBangRoom);
#endif
	return TRUE;

}

BOOL gObjGetUserNumber(int aIndex)
{
	int usernumber;

	if ( !OBJMAX_RANGE(aIndex))
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return 0;
	}

	usernumber=gObj[aIndex].UserNumber;
	return usernumber;
}

BOOL gObjGetNumber(int aIndex, int& UN, int& DBN)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return 0;
	}
	UN=gObj[aIndex].UserNumber;
	DBN=gObj[aIndex].DBNumber;
	return 1;
}

char* gObjGetAccountId(int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		LogAdd("error : %s %d",__FILE__, __LINE__);
		return 0;
	}

	if ( gObj[aIndex].Connected == 2)
	{
		return &gObj[aIndex].AccountID[0];
	}
	return 0;
}

int gObjGetIndex(char * szId)
{
	for( int n = OBJ_STARTUSERINDEX; n < OBJMAX; n++ )
	{
		if( gObj[n].Connected >= PLAYER_PLAYING )
		{
			if( szId != NULL && gObj[n].Name[0] == *szId )
			{
				if( strcmp(&gObj[n].Name[0] , szId) == 0 )
				{
					return n;
				}
			}
		}
	}
	// ----
	return -1;
}

int gObjGetIndexByAccount(char * szId)
{
	for( int n = OBJ_STARTUSERINDEX; n < OBJMAX; n++ )
	{
		if( gObj[n].Connected >= PLAYER_PLAYING )
		{
			if( szId != NULL && gObj[n].AccountID[0] == *szId )
			{
				if( strcmp(&gObj[n].AccountID[0] , szId) == 0 )
				{
					return n;
				}
			}
		}
	}
	// ----
	return -1;
}

BOOL gObjUserIdConnectCheck(char* szId,int index)
{
	if ( index < 0 || index > OBJMAX)
	{
		LogAdd("error : index error !! %s %d",__FILE__, __LINE__);
		return 0;
	}

	if ( gObj[index].Connected < PLAYER_PLAYING)
	{
		return 0;
	}

	if (gObj[index].Name[0] != *szId)
	{
		return 0;
	}

	if ( strcmp(gObj[index].Name , szId) == 0)
	{
		return 1;
	}
	return 0;
}

BYTE GetPathPacketDirPos(int px, int py)
{
	BYTE pos = 0;

	if (px <= -1 && py <= -1)
	{
		pos=0;
	}
	else if (px <= -1 && py == 0)
	{
		pos=7;
	}
	else if ( px <= -1 && py >= 1)
	{
		pos=6;
	}
	else if ( px == 0 && py <= -1)
	{
		pos= 1;
	}
	else if ( px == 0 && py >= 1)
	{
		pos = 5;
	}
	else if ( px >= 1 && py <=-1)
	{
		pos=2;
	}
	else if ( px >= 1 && py == 0)
	{
		pos = 3;
	}
	else if ( px >=1  && py >= 1)
	{
		pos = 4;
	}

	return pos;
}

int  gObjCalDistance(LPOBJ lpObj1, LPOBJ lpObj2)
{
	if ( lpObj1->X == lpObj2->X && lpObj1->Y == lpObj2->Y )
	{
		return 0;
	}

	float tx = lpObj1->X - lpObj2->X;
	float ty = lpObj1->Y - lpObj2->Y;

	return sqrt( (tx*tx)+(ty*ty) );
}

BOOL gObjPositionCheck(LPOBJ lpObj)
{
	int ix = lpObj->m_OldX - lpObj->TX;
	int iy = lpObj->m_OldY - lpObj->TY;

	if ( ix <= -15 ||  ix >= 15 )
	{
		return FALSE;
	}

	if ( iy <= -15 ||  iy >= 15 )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL gObjCheckTileArea(int aIndex, int x, int y, int dis)
{
	LPOBJ lpObj = & gObj[aIndex];

	if ( x < (lpObj->X - dis) || x > (lpObj->X + dis) || y < (lpObj->Y - dis) || y > (lpObj->Y + dis) )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL ExtentCheck(int x, int y, int w, int h)
{
	if ( (x >=0) && (x<w) && (y >=0) && (y<h) )
	{
		return 1;
	}
	return 0;
}

void gObjSetInventory1Pointer(LPOBJ lpObj)
{
	lpObj->pInventory = lpObj->Inventory1;
	lpObj->pInventoryMap = lpObj->InventoryMap1;
	lpObj->pInventoryCount  = &lpObj->InventoryCount1;
}

void gObjSetInventory2Pointer(LPOBJ lpObj)
{
	lpObj->pInventory = lpObj->Inventory2;
	lpObj->pInventoryMap = lpObj->InventoryMap2;
	lpObj->pInventoryCount  = &lpObj->InventoryCount2;
}

void gObjAddMsgSend(LPOBJ lpObj, int aMsgCode, int aIndex, int  SubCode)
{
	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;

			return;
		}
	}

	for (int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			
			return;
		}
	}
}

void gObjAddMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode)
{
	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount() + delay;
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;

			return;
		}
	}

	for (int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount();
			gSMMsg[lpObj->m_Index][n].SendUser= aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			
			return;
		}
	}

}

void gObjAddMsgSendDelayInSpecificQPos(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition)
{
	if ( iQPosition >= 0 &&  iQPosition < MAX_MONSTER_SEND_MSG )
	{
		gSMMsg[lpObj->m_Index][iQPosition].MsgCode = aMsgCode;
		gSMMsg[lpObj->m_Index][iQPosition].MsgTime = GetTickCount();
		gSMMsg[lpObj->m_Index][iQPosition].SendUser = aIndex;
		gSMMsg[lpObj->m_Index][iQPosition].SubCode = SubCode;
	}

}

void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2)
{
	for ( int n=0;n<MAX_MONSTER_SEND_ATTACK_MSG;n++)
	{
		int iIndex = lpObj->m_Index - OBJ_STARTUSERINDEX;

		if ( gSMAttackProcMsg[iIndex][n].MsgCode < 0 )
		{
			gSMAttackProcMsg[iIndex][n].MsgCode = aMsgCode;
			gSMAttackProcMsg[iIndex][n].MsgTime = GetTickCount() + delay;
			gSMAttackProcMsg[iIndex][n].SendUser = aIndex;
			gSMAttackProcMsg[iIndex][n].SubCode = SubCode;
			gSMAttackProcMsg[iIndex][n].SubCode2 = SubCode2;
			return;
		}
	}

}

void gObjMsgProc(LPOBJ lpObj)
{
	int aIndex = lpObj->m_Index;

	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[aIndex][n].MsgCode >= 0 )
		{
			if ( GetTickCount() > gSMMsg[aIndex][n].MsgTime )
			{
				if ( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC )
				{
					gObjMonsterStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
				}
				else
				{
					gObjStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
				}

				gSMMsg[aIndex][n].MsgCode = -1;
			}
		}
	}
}

void gObjStateProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode)
{
	if ( (  (aIndex<0)?FALSE:(aIndex > OBJMAX-1)?FALSE:TRUE ) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__ );
		return;
	}

	switch ( aMsgCode )
	{
		case  2:
			gObjBackSpring(lpObj, &gObj[aIndex]);
			break;

		case  3:
			gObjMonsterDieLifePlus(lpObj, &gObj[aIndex]);
			break;

		case 1000:
			//gObjBillRequest(lpObj);
			break;

		case  4:
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->lpTargetGuildNode != NULL )
			{
				gObjGuildWarEnd(lpObj->lpGuild, lpObj->lpGuild->lpTargetGuildNode);
			}
			break;

		case  5:
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->WarState != NULL && lpObj->lpGuild->WarType == 1 )
			{
				BattleSoccerGoalStart(0);
			}
			break;

		case  7:
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->WarState != NULL && lpObj->lpGuild->WarType == 1)
			{
				if ( lpObj->lpGuild->BattleGroundIndex >= 0 )
				{
					GCManagerGuildWarEnd(lpObj->lpGuild->Name);
				}
			}
			break;

		case 10:
			if ( lpObj->DamageReflect != 0 )
			{
				gObjAttack(lpObj, &gObj[aIndex], 0, 0, 0, SubCode, 0, 0, 1);
			}
			break;

		case 54: //FireScream
			if( lpObj->Live )
			{
				CMagicInf * lpMagic = &DefMagicInf[79];
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 0, SubCode, 0, 0, 0);
			}
			break;

	//Season 2.5 add-on for third wings
		case 12:
			{
				gObjAttack(lpObj, &gObj[aIndex], 0, 0, 0, SubCode, 0, 0, 0);
			}
			break;
		case 13:
			{
				lpObj->Life = (float)lpObj->AddLife + (float)lpObj->MaxLife; //season4.5 changed
				GCReFillSend(lpObj->m_Index,lpObj->Life, -1, 0, lpObj->iShield);
			}
			break;
		case 14:
			{
				lpObj->Mana = (float)lpObj->AddMana + (float)lpObj->MaxMana; //season 4.5 changed
				GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
			}
			break;

		case 58: //Eletric Surge
			if( lpObj->Live )
			{
				CMagicInf * lpMagic = &DefMagicInf[216]; //Eletrict Surge
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 0, SubCode, 0, 0, 0);
			}
			break;
		case 59: //Pollution Attack
			if( lpObj->Live )
			{
				BYTE cX = SET_NUMBERL(SET_NUMBERLW(SubCode));
				BYTE cY = SET_NUMBERH(SET_NUMBERLW(SubCode));
				BYTE Flag = SET_NUMBERHW(SubCode);

				CMagicInf * lpMagic = &DefMagicInf[225]; //Pollution
				gObjUseSkill.SkillPollutionStart(lpObj->m_Index, lpMagic, cX, cY, aIndex, Flag+1);
			}
			break;
	}
}

//Identical
void gObjStateAttackProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2) //004EB240
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__ );
		return;
	}
	
	switch ( aMsgCode )
	{
		case 50:
			if ( lpObj->Live != FALSE )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 0, 1, 0, SubCode2, 0, 0);
			}
			break;
		case 53:
			if( lpObj->Live )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 1, 0, 0, 0, 0);
			}
			break;
		case 51:
			gDarkSpirit[lpObj->m_Index].Attack(lpObj, &gObj[aIndex], NULL, SubCode, SubCode2);
			break;
	}

}

BOOL gObjBackSpring(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int tdir;

	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_LastTeleportTime > 0 )
		{
			return FALSE;
		}
	}

//#if(_GSCS==1)
	if( lpObj->Type == OBJ_USER)
	{
		if((lpObj->Authority &0x02) == 2)
		{
			return FALSE;
		}
	}
//#endif

	if ( lpObj->Class >= 131 && lpObj->Class <= 134 )
	{
		return FALSE;
	}

//#if(_GSCS==1)
	if( lpObj->Class == 277 ||//castle siege ??
		lpObj->Class == 283 ||
		lpObj->Class == 288 ||
		lpObj->Class == 278 ||
		lpObj->Class == 215 ||
		lpObj->Class == 216 ||
		lpObj->Class == 217 ||
		lpObj->Class == 218 ||
		lpObj->Class == 219 )
	{
		return FALSE;
	}
	
	if(g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index)
	{
		if(lpObj->pInventory[8].m_Type == ITEMGET(13,4))
		{
			return FALSE;
		}
	}
	
	if(g_Crywolf.GetCrywolfState() == 4 || g_Crywolf.GetCrywolfState() == 3)
	{
		if(lpObj->Type == OBJ_USER)
		{
			for(int i=205;i<=209;i++)//crywolf altar
			{
				int iAltarIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(i);
				
				if(iAltarIndex != -1)
				{
					if(iAltarIndex == lpObj->m_Index)
					{
						return FALSE;
					}
				}
			}
		}
	}
	
	if(CRYWOLF_ALTAR_CLASS_RANGE(lpObj->Class) || CHECK_CLASS(204,lpObj->Class) != FALSE) //HermeX Fix
	{
		return FALSE;
	}
	if(lpObj->Class == 348)//balista
	{
		return FALSE;
	}
//#endif

	if(lpObj->Class == 275) //Season 2.5 add-on (Kundun Fix)
	{
		return FALSE;
	}

	//Season 4.0 add-on
	if(lpObj->Class == 459 ||
		lpObj->Class == 460 ||
		lpObj->Class == 461 ||
		lpObj->Class == 462)
	{
		return FALSE;
	}

	int EffectID = -1;
	for(int i=0;i<lpObj->m_BuffEffectCount;i++) //season4 add-on
	{
		EffectID = lpObj->m_BuffEffectList[i].btBuffIndex;

		if( EffectID == AT_ICE_ARROW ||
			EffectID == AT_STUN ||
			EffectID == AT_ILLUSION_RESTRICTION_SPELL)
		{
			return FALSE;
		}
	}
		
	if ( (rand()%3) == 0 )
	{
		if ( lpTargetObj->Dir < 4 )
		{
			tdir = lpTargetObj->Dir + 4;
		}
		else
		{
			tdir = lpTargetObj->Dir - 4;
		}

		tdir *= 2;
	}
	else
	{
		tdir = lpTargetObj->Dir * 2;
	}

	int x;
	int y;
	BYTE attr;

	x = lpObj->X;
	y = lpObj->Y;
	x += RoadPathTable[tdir];
	y += RoadPathTable[1+tdir];
	attr = MapC[lpObj->MapNumber].GetAttr(x, y);

	if ( (attr&1)==1 ||
		(attr&2)==2 || 
		(attr&4)== 4 || 
		(attr&8)== 8 
//#if(_GSCS==1)
		||(attr&16)== 16 
//#endif
		)
	{
		return FALSE;
	}

	PMSG_POSISTION_SET pMove;
	pMove.h.c = 0xC1;
	pMove.h.headcode = 0xD6;
	pMove.h.size = sizeof(pMove);
	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	RecvPositionSetProc(&pMove, lpObj->m_Index);

	if ( lpObj->Type == OBJ_USER )
	{
		lpObj->PathCount = 0;
	}

	return TRUE;
}

BOOL BackSpringCheck(int & x, int & y, int & dir, BYTE map)
{
	BYTE attr;
	int tx = x;
	int ty = y;
	int tdir = dir/2;

	tx = tx + RoadPathTable[dir];
	ty = ty + RoadPathTable[1+dir];

	attr = MapC[map].GetAttr(tx, ty);

	if	(  (attr&1)==1 || 
		(attr&4)== 4 || 
		(attr&8)== 8
//#if(_GSCS==1)
		||(attr&16)== 16 
//#endif
		)
	{
		tdir += 4;
		
		if ( tdir > 7 )
		{
			tdir -= 8;
		}
		
		dir = tdir * 2;
		return FALSE;
	}

	x = tx;
	y = ty;

	return TRUE;
}

//00567c00
BOOL gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTargetObj, int count)
{
	int tdir;

	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

//#if(_GSCS==1) // # Need check | heeee
	if( lpObj->Type == OBJ_USER)
	{
		if((lpObj->Authority &0x02) == 2)
		{
			return FALSE;
		}
	}
//#endif

	if ( lpObj->Class == 287 || lpObj->Class == 286 )
	{
		return FALSE;
	}

//#if(_GSCS==1)
	if ( lpObj->Class == 278 )
	{
		return FALSE;
	}
//#endif

	if ( lpObj->Class >= 131 && lpObj->Class <= 134 )
	{
		return FALSE;
	}

//#if(_GSCS==1)
	if( lpObj->Class == 277 ||//castle siege ??
		lpObj->Class == 283 ||
		lpObj->Class == 288 ||
		lpObj->Class == 278 ||
		lpObj->Class == 215 ||
		lpObj->Class == 216 ||
		lpObj->Class == 217 ||
		lpObj->Class == 218 ||
		lpObj->Class == 219 )
	{
		return FALSE;
	}
	
	if(g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index)
	{
		if(lpObj->pInventory[8].m_Type == ITEMGET(13,4))
		{
			return FALSE;
		}
	}
	
	if(g_Crywolf.GetCrywolfState() == 4 || g_Crywolf.GetCrywolfState() == 3)
	{
		if(lpObj->Type == OBJ_USER)
		{
			for(int i=205;i<=209;i++)//crywolf altar
			{
				int iAltarIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(i);
				
				if(iAltarIndex != -1)
				{
					if(iAltarIndex == lpObj->m_Index)
					{
						return FALSE;
					}
				}
			}
		}
	}
	
	if(CRYWOLF_ALTAR_CLASS_RANGE(lpObj->Class) || CHECK_CLASS(204,lpObj->Class) != FALSE) //HermeX Fix
	{
		return FALSE;
	}	
	if(lpObj->Class == 348)//balista
	{
		return FALSE;
	}
//#endif
	
	if(lpObj->Class == 275) //Season 2.5 add-on (Kundun Fix)
	{
		return FALSE;
	}

	//Season 4.0 add-on
	if(lpObj->Class == 459 ||
		lpObj->Class == 460 ||
		lpObj->Class == 461 ||
		lpObj->Class == 462)
	{
		return FALSE;
	}

	int EffectID = -1;
	for(int i=0;i<lpObj->m_BuffEffectCount;i++) //season4 add-on
	{
		EffectID = lpObj->m_BuffEffectList[i].btBuffIndex;

		if( EffectID == AT_ICE_ARROW ||
			EffectID == AT_STUN ||
			EffectID == AT_ILLUSION_RESTRICTION_SPELL)
		{
			return FALSE;
		}
	}

//#if(_GSCS==1) # Need check | wtf? =/
	tdir = GetPathPacketDirPos(lpObj->X - lpTargetObj->X, lpObj->Y - lpTargetObj->Y) * 2;
//#else
//	tdir = GetPathPacketDirPos(lpObj->X - lpTargetObj->X, lpObj->Y - lpTargetObj->Y) * 2;
//#endif

	int x = lpObj->X;
	int y = lpObj->Y;

	for ( int n=0;n<count;n++)
	{
		if ( n >= 2 )
		{
			if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_KNIGHT )
			{
				BackSpringCheck(x, y, tdir, lpObj->MapNumber);
			}
		}
		else
		{
			BackSpringCheck(x, y, tdir, lpObj->MapNumber);
		}
	}

	PMSG_POSISTION_SET pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0xD6;
	pMove.h.size = sizeof(pMove);
	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	RecvPositionSetProc(&pMove, lpObj->m_Index);

	return TRUE;
}

//00568030
bool gObjLevelUp(LPOBJ lpObj, __int64 & addexp, int iMonsterType, int iEventType) //0051F140
{
	BYTE bEventMapReward = false;
	// ----
	if( iEventType == 2 )
	{
		bEventMapReward = true;
	}
	// ----
	if( g_MasterLevelSystem.MasterLevelUp(lpObj, addexp, bEventMapReward, iMonsterType) )	//-> 6.2 Updated
	{
		return false;
	}

	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %I64u %I64d MonsterIndex : %d, EventType : %d", lpObj->MapNumber,lpObj->X,lpObj->Y,lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->Experience - addexp, addexp, iMonsterType, iEventType);

	if ( lpObj->Level >= MAX_CHAR_LEVEL && lpObj->Experience >= gLevelExperience[-1+lpObj->Level] )
	{
		lpObj->Experience = gLevelExperience[-1+lpObj->Level];
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 112)), lpObj->m_Index, 1);
		return false;
	}

	if(NULL == addexp) //season4 add-on good
	{
		return false;
	}

	if(addexp == 0) //season4 add-on good
	{
		return false;
	}

	if(lpObj->Level <= 150) //season4.5 add-on good
	{
		if(IsOKPCBangBenefitAll(lpObj) != FALSE)
		{
			int RewardEXP = addexp / 10; //loc1
			RewardEXP = (RewardEXP < gLevelExperience[lpObj->Level])?(RewardEXP):(gLevelExperience[lpObj->Level]);
			lpObj->Experience += RewardEXP;
			addexp += (__int64)RewardEXP;
		}
	}

	gObjSetExpPetItem(lpObj->m_Index, addexp);

	if ( lpObj->Experience < lpObj->NextExp )
	{
		return true;
	}

	addexp = 0;

	lpObj->Experience = lpObj->NextExp;
	lpObj->Level++;

#ifdef __CUSTOMS__
	lpObj->LevelUpPoint += g_ClassCalc.m_Data[lpObj->Class].LevelPoints;
#else
	if ( lpObj->Class == CLASS_DARKLORD )
	{
		lpObj->LevelUpPoint += 7;
	}
	else if ( lpObj->Class == CLASS_MAGUMSA )
	{
		lpObj->LevelUpPoint += 7;
	}
#ifdef MONK
	else if ( lpObj->Class == CLASS_MONK )
	{
		lpObj->LevelUpPoint += 7;
	}
#endif
	else
	{
		lpObj->LevelUpPoint += 5;
	}
#endif

	if( lpObj->PlusStatQuestClear && lpObj->Level >= g_ResetSystem.m_MarlonStatMinLevel )
	{
		lpObj->LevelUpPoint += 1;
		LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d",	lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint);
	}

	gObjCalCharacter(lpObj->m_Index);

	lpObj->MaxLife += DCInfo.DefClass[ lpObj->Class ].LevelLife;
	lpObj->MaxMana += DCInfo.DefClass[ lpObj->Class ].LevelMana;
	lpObj->Life = lpObj->MaxLife;
	lpObj->Mana = lpObj->MaxMana;

	lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
	
	gObjCalcShieldPoint(lpObj);
	lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
	GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);

	gObjNextExpCal(lpObj);	
	LogAddTD(lMsg.Get(MSGGET(2, 8)), lpObj->AccountID, lpObj->Name, lpObj->Level);

	if(lpObj->Class == CLASS_ELF)
	{
		if(lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel() && lpObj->ChangeUP == 1)
		{
			int iAddSkillPosition = gObjMagicAdd(lpObj,AT_SKILL_INFINITY_ARROW,0);

			if(iAddSkillPosition >= 0)
			{
				GCMagicListOneSend(lpObj->m_Index,iAddSkillPosition,AT_SKILL_INFINITY_ARROW,220,0,0);
				LogAddTD("[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)", lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->ChangeUP);
			}
			else
			{
				LogAddTD("[%s][%s] Fail - Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)", lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->ChangeUP);
			}
		}
	}

	GJSetCharacterInfo(lpObj, lpObj->m_Index, 0, 0);
	gObjSetBP(lpObj->m_Index);
	GCLevelUpMsgSend(lpObj->m_Index, 1);
	gObjCalcMaxLifePower(lpObj->m_Index);

	if( lpObj->Level == 400 && lpObj->PartyNumber >= 0 ) //Season 2.5 add-on (Party Level 400 Display)
	{
		int iPartyNumber = lpObj->PartyNumber;
		char szMsg[256];
		sprintf(szMsg,"400 LevelUp (%s)(%s) Party ",lpObj->AccountID,lpObj->Name);
		int iPartyNumIndex;

		for( int i = 0; i<MAX_USER_IN_PARTY; i++ )
		{
			iPartyNumIndex = gParty.m_PartyS[iPartyNumber].Number[i];

			if( iPartyNumIndex >= 0  )
			{
				int iSize = strlen(szMsg);
				sprintf(&szMsg[iSize],",(%s)(%s) ",gObj[iPartyNumIndex].AccountID,gObj[iPartyNumIndex].Name);
			}
		}
		LogAddTD(szMsg);
	}
	return true;
}

bool gObjMaxUpToStatCheck(BYTE Type, LPOBJ lpObj)
{
	bool bResult	= TRUE;
	int CurStat;
	// ----
	switch (Type)
	{
		case 0:
			CurStat	= lpObj->Strength;
			break;
			// --
		case 1:
			CurStat	= lpObj->Dexterity;
			break;
			// --
		case 2:	  
			CurStat	= lpObj->Vitality;
			break;
			// --
		case 3:
			CurStat	= lpObj->Energy;
			break;
			// --
		case 4:
			CurStat	= lpObj->Leadership;		
			break;
	}
	// ----
	if( CurStat >= g_MaxStat || CurStat < 0 )
	{
		bResult = FALSE;
	}
	// ----
	return bResult;
}

BOOL gObjLevelUpPointAdd(BYTE type, LPOBJ lpObj)
{
	if ( lpObj->Type > OBJ_NPC )
	{
		LogAddC(2, lMsg.Get(MSGGET(2, 9)), lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return FALSE;
	}

	if ( lpObj->LevelUpPoint < 1 )
	{
		return FALSE;
	}

	switch ( type )
	{
		case 0:
			if( lpObj->Strength < g_MaxStat )	{
				lpObj->Strength++;
			}
			else	{
				return false;
			}
			break;

		case 1:
			if( lpObj->Dexterity < g_MaxStat )
				lpObj->Dexterity++;
			else
				return false;
			break;

		case 2:

			if( lpObj->Vitality < g_MaxStat )
				lpObj->Vitality++;
			else
				return false;

			lpObj->MaxLife += lpObj->VitalityToLife;
			gObjCalcMaxLifePower(lpObj->m_Index);
			break;

		case 3:
			if( lpObj->Energy < g_MaxStat )
				lpObj->Energy++;
			else
				return false;

			lpObj->MaxMana += lpObj->EnergyToMana;
			break;

		case 4:
			if( lpObj->Leadership < g_MaxStat )
				lpObj->Leadership++;
			else
				return false;
			break;
	}

	lpObj->LevelUpPoint--;
	gObjCalCharacter(lpObj->m_Index);

	return TRUE;
}







void gObjMonsterItemLoadScript(LPOBJ lpObj, char* script_file)
{
	int Token;
	int type;
	int index;
	int level;
	int op1;
	int op2;
	int op3;
	int incount;
	
	SMDFile=fopen(script_file, "r");	//ok

	if (SMDFile == 0)
	{
		MsgBox("file open error %s", script_file);
		return;
	}
	incount=0;

	while ( true )
	{
		Token=GetToken();
		
		if (Token == 2)
		{
			break;
		}

		if (Token == 1)
		{
			type=TokenNumber;

			Token=GetToken();
			index=TokenNumber;

			Token=GetToken();
			level=TokenNumber;

			Token=GetToken();
			op1=TokenNumber;

			Token=GetToken();
			op2=TokenNumber;

			Token=GetToken();
			op3=TokenNumber;

			if (  gObjMonsterInventoryInsertItem(lpObj, type, index, level, op1, op2, op3)  != 0xFF)
			{
				incount++;

				if (incount > 75)
				{
					break;
				}
			}
		}
		
	}


	fclose(SMDFile);
}

void gObjGiveItemSearch(LPOBJ lpObj, int maxlevel)
{
	return;
	int result;
	int count=256;
	int incount=0;
	int type;
	int index;
	
	memset(lpObj->pInventoryMap, 0xFF, 8*8);
	while(count--)
	{
		if( (rand()%40) == 0 )
		{
			type = 15;
			index = rand()%MAX_SUBTYPE_ITEMS;
		}
		else
		{		
			type = rand()%MAX_TYPE_ITEMS;			
			index = rand()%MAX_SUBTYPE_ITEMS;
		}

		if( 
			((type==13) && (index<8)) || 
			((type == 14) && ((index == 9) || (index == 10) || (index == 13) || (index == 14) || (index == 16) || (index == 17) || (index == 18)) ) ||
			((type==12) && (index==15)) 
		)
		{
			int perc = rand()%8;
			int btResult = FALSE;

			if( type == 12 && index == 15 )
			{
				perc = rand()%4;
			}
			
			/*if( type == 14 && index == 17 ) //season4.5 removed
			{
				if( gEyesOfDevilSquareDropRate <= 0 )
				{
					gEyesOfDevilSquareDropRate = 1;
				}

				perc = rand()%gEyesOfDevilSquareDropRate;	
				btResult = TRUE;
			}

			if( type == 14 && index == 18 )
			{
				perc = rand()%gKeyOfDevilSquareDropRate;	
				btResult = TRUE;
			}*/

			if( perc == 0 )
			{
				if( btResult == TRUE )
				{
					int level = 0;

					if( rand() % 5 != 0 )
					{
						if( lpObj->Level < 3 ) level = 0;
						else if ( lpObj->Level < 36 ) level = 1;
						else if ( lpObj->Level < 47 ) level = 2;
						else if ( lpObj->Level < 60 ) level = 3;
						else if ( lpObj->Level < 70 ) level = 4;
						else if ( lpObj->Level < 80 ) level = 5;
						else level = 6;

						if( level != 0 )
						{
							if( gObjMonsterInventoryInsertItem( lpObj, type, index, level ) != 0xFF )
							{
								incount++;
								if( incount > 75 ) break;
							}
						}

					}
				}
				else
				{
					if( zzzItemLevel(type, index, lpObj->Level) == TRUE )
					{
						if( gObjMonsterInventoryInsertItem(lpObj, type, index, 0) != 0xFF )
						{
							incount++;
							if( incount > 75 ) break;
						}
					}
				}
			}
		}
		else
		{
			result = GetLevelItem(type, index, lpObj->Level);
			if( result >= 0  )
			{
				if( (type == 13 && index == 10) || 
					(type == 12 && index == 11) )
				{
					if( gObjMonsterInventoryInsertItem(lpObj, type, index, result) != 0xFF )
					{
						incount++;
						if( incount > 75 ) break;
					}
				}
				else if( result <= maxlevel )
				{
					if( type == 12)
					{
						if( index != 11 ) result = 0;
					}
					if( type == 12 && index == 11 )
					{

					}
					else if( result > maxlevel ) result = maxlevel;

					if( (type == 4 && index == 7) || (type == 4 && index == 15) )	// ?®±?,?­»? ???? ????
					{
						result = 0;
					}
					
					if( gObjMonsterInventoryInsertItem(lpObj, type, index, result) != 0xFF )
					{
						incount++;
						if( incount > 75 ) break;
					}
				}
			}
		}
	}
}

BYTE gObjWarehouseInsertItem(LPOBJ lpObj, int type, int index, int level, int pos)
{
	CItem item;

	int item_type = ItemGetNumberMake(type, index);
	item.m_Level =level;
	item.Convert(item_type, 0 , 0, 0, 0, 0,0, NULL, 0xFF, 0, CURRENT_DB_VERSION);
	lpObj->pWarehouse[pos] = item;
	return pos;
}

void gObjGiveItemWarehouseSearch(LPOBJ lpObj, int maxlevel)
{
	return; //HermeX Decompilation
	int result;
	int count=256;
	int incount=0; //c
	int type; //10
	int index; //14
	int pos = 0; //18 (change here the starting position)
	int level = 0; //1C
	int StartItem; //20
	
	memset(lpObj->pInventoryMap, 0xFF, 8*8);
	while(count--)
	{
		if( (rand()%40) == 0 )
		{
			type = 15;
			index = rand()%MAX_SUBTYPE_ITEMS;
		}
		else
		{		
			type = rand()%MAX_TYPE_ITEMS;			
			index = rand()%MAX_SUBTYPE_ITEMS;
		}

		if(lpObj->Class == 275)
		{
			StartItem = (type<<9)+index;
			if(ItemAttribute[StartItem].Level == (BYTE)255)
			{
				continue;
			}

			if(ItemAttribute[StartItem].Level == (BYTE)0)
			{
				continue;
			}

			if(ItemAttribute[StartItem].Level < (BYTE)125)
			{
				continue;
			}
		}

		if( (type==13) && (index==3) )
		{
			continue;
		}

		if( ((type==13) && (index<8)) || 
			((type == 14) && ((index == 9) || (index == 10) || (index == 13) || (index == 14) || (index == 16) || (index == 17) || (index == 18)) ) ||
			((type==12) && (index==15)) 
		)
		{
			int perc = rand()%8;

			if( type == 12 && index == 15 )
			{
				perc = 1;
			}

			if( type == 14 && index == 17 )
			{
				perc = 1;
			}

			if( type == 14 && index == 18 )
			{
				perc = 1;
			}

			if( perc == 0 )
			{
				if( zzzItemLevel(type, index, level) == TRUE )
				{
					if( gObjWarehouseInsertItem(lpObj, type, index, 0, pos) != 0xFF )
					{
						pos++;
						incount++;
						if( incount > 75 ) break;
					}
				}
			}
		}
		else
		{
			result = GetLevelItem(type, index, level);
			if( result >= 0  )
			{
				if( (type == 13 && index == 10) || 
					(type == 12 && index == 11) )
				{
					if( gObjWarehouseInsertItem(lpObj, type, index, 0, pos) != 0xFF )
					{
						pos++;
						incount++;
						if( incount > 75 ) break;
					}
				}
				else if( result <= maxlevel )
				{
					if( type == 12)
					{
						if( index != 11 ) result = 0;
					}
					if( type == 12 && index == 11 )
					{

					}
					else if( result > maxlevel ) result = maxlevel;

					if( (type == 4 && index == 7) || (type == 4 && index == 15) )	// ?®±?,?­»? ???? ????
					{
						result = 0;
					}
					
					if( gObjWarehouseInsertItem(lpObj, type, index, 0, pos) != 0xFF )
					{
						pos++;
						incount++;
						if( incount > 75 ) break;
					}
				}
			}
		}
	}
	lpObj->WarehouseCount = pos;
}

BOOL gObjGuildMasterCapacityTest(LPOBJ lpObj)
{
	if( lpObj->Level < gGuildCreateLevel )
	{
		return false;
	}
#ifdef __ALIEN__
	if( lpObj->Reset < g_GuildCreateReset )
	{
		return false;
	}
#endif
	return true;
}

void gObjNextExpCal(LPOBJ lpObj)
{
	lpObj->NextExp = gLevelExperience[lpObj->Level];
}

BOOL retResistance(LPOBJ lpObj, int Resistance_Type)
{
	BYTE r = lpObj->m_Resistance[Resistance_Type];

	if( r == 0xFF)
	{
		return TRUE;
	}

	if( (lpObj->Authority&32) == 32 ) //Season 2.5 add-on (GM Ring +255 Resistance Attribute)
	{
		if(lpObj->pInventory[10].m_Type == ITEMGET(13,42) || lpObj->pInventory[11].m_Type == ITEMGET(13,42))
		{
			return TRUE;
		}
	}

	if ( r > 0 && (gObjCheckUsedBuffEffect(lpObj, AT_POTION_OF_SOUL) == 1) && (Resistance_Type == 2 || Resistance_Type == 0 ))
	{
		r += r * 50 / 100;
	}

	if ( r  == 0 )
	{
		return FALSE;
	}

	if (  (rand()%(r+1)) == 0 )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL retCalcSkillResistance(LPOBJ lpTargetObj, BOOL isDouble)
{
	if ( isDouble != FALSE )
	{
		if ( (rand()%100) < 45 )
		{
			return TRUE;
		}
	}
	else
	{
		if ( (rand()%100) < 40 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL gObjAttackQ(LPOBJ lpObj)
{
	if ( ATTRIBUTE_RANGE(lpObj->m_Attribute) )
	{
		return FALSE;
	}

	if ( lpObj->Class == 221 || lpObj->Class == 222 )
	{
		return FALSE;
	}

//#if(_GSCS==1)
	if ( lpObj->Class == 277 || lpObj->Class == 283 )
	{
		if(g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
		{
			return FALSE;
		}
	}

	if(lpObj->Class == 221 || lpObj->Class == 222) //HermeX Decompilation
	{
		return FALSE;
	}
	
	if( lpObj->Class == 277 )
	{
		if( lpObj->m_btCsGateOpen == 1)
		{
			return FALSE;
		}
	}
//#endif

	if ( BC_MAP_RANGE(lpObj->MapNumber) )
	{
		if ( lpObj->Type >= OBJ_MONSTER )
		{
			int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(lpObj->MapNumber);
			if ( lpObj->Class == 131 && g_BloodCastle.CheckMonsterKillSuccess(iBridgeIndex) == false)
			{
				return FALSE;
			}

			if ( (((lpObj->Class - 132) < 0)?FALSE:((lpObj->Class-132) > 2)?FALSE:TRUE) !=FALSE && g_BloodCastle.CheckBossKillSuccess(iBridgeIndex) == false )
			{
				return FALSE;
			}
		}
	}

	if ( lpObj->Class != 247 && lpObj->Class != 249 && lpObj->Class  >= 100 && lpObj->Class <= 110 )
	{
		return FALSE;
	}

	if ( lpObj->m_State != 2 )
	{
		return FALSE;
	}

	if( lpObj->Live == false )
	{
		return false;
	}

	if ( lpObj->Teleport == 1 )
	{
		return FALSE;
	}

	return TRUE;
}

void GCSendDuelScore(int aIndex1, int aIndex2)
{
#if (GS_CASTLE==1)
	return;
#endif
	PMSG_ANS_DUEL_SCORE pMsg;

	if (!OBJMAX_RANGE(aIndex1) || !OBJMAX_RANGE(aIndex2))
	{
		return;
	}

	if (!gObjIsConnected(aIndex1) || !gObjIsConnected(aIndex2) )
	{
		return;
	}

	if (gObj[aIndex1].Type == OBJ_MONSTER || gObj[aIndex2].Type == OBJ_MONSTER )
	{
		return;
	}

	if (gObj[aIndex1].CloseCount >= 0 || gObj[aIndex2].CloseCount >= 0 )
	{
		return;
	}

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xAD;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH1 = SET_NUMBERH(aIndex1);
	pMsg.NumberL1 = SET_NUMBERL(aIndex1);
	pMsg.NumberH2 = SET_NUMBERH(aIndex2);
	pMsg.NumberL2 = SET_NUMBERL(aIndex2);
	pMsg.btDuelScore1 = gObj[aIndex1].m_btDuelScore;
	pMsg.btDuelScore2 = gObj[aIndex2].m_btDuelScore;

	DataSend(aIndex1, (LPBYTE)&pMsg, pMsg.h.size);
	DataSend(aIndex2, (LPBYTE)&pMsg, pMsg.h.size);
}

//004ED900
void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj, int MSBDamage)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

#ifdef __CUSTOMS__
	if( lpTargetObj->Type == OBJ_USER )
	{
		if( g_PVPZone.IsPVPZone(lpObj) )
		{
			return;
		}
	}
#endif

	if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 )
	{
		if ( lpObj->lpGuild!= NULL && lpTargetObj->lpGuild != NULL )
		{
			LogAddTD("[U.System][Rival][Player Kill] ( Killer [%s][%s] / %s / U:%d / R:%d ) vs ( Victim [%s][%s] / %s / U:%d / R:%d )", lpObj->AccountID, lpObj->Name, lpObj->lpGuild->Name, lpObj->lpGuild->iGuildUnion, lpObj->lpGuild->iGuildRival, lpTargetObj->AccountID, lpTargetObj->Name, lpTargetObj->lpGuild->Name, lpTargetObj->lpGuild->iGuildUnion, lpTargetObj->lpGuild->iGuildRival);
		}
		else
		{
			LogAddTD("[U.System][Rival][Player Kill][??ERROR : Can't find GuildInfo] (  [%s][%s] ) vs ( [%s][%s] )", lpObj->AccountID,lpObj->Name,lpTargetObj->AccountID,lpTargetObj->Name);
		}
		return;
	}

	if ( CC_MAP_RANGE(lpTargetObj->MapNumber)  )
	{
		if ( g_ChaosCastle.GetCurrentState(g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber)) == 2 )
		{
			return;
		}
	}

//#if(_GSCS==1)
	if(lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE)
	{
		if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			if(lpObj->m_btCsJoinSide != 0)
			{
				return;
			}
		}
		else if(lpObj->Y > 113)
		{
			if(g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE)
			{
				return;
			}
		}
		
		if(lpTargetObj->m_PK_Level >= 6)
		{
			return;
		}
	}
//#endif

	if((lpObj->Authority & 16) || (lpTargetObj->Authority & 16))
	{
		return;
	}

	if ( IT_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpTargetObj->MapNumber) )
	{
		return;
	}

	int iPartyNumber = lpObj->PartyNumber;
	int loc2 = -1;

#ifdef NPVP
	if( g_NewPVP.IsVulcanusMap(lpObj->MapNumber) && g_NewPVP.IsVulcanusMap(lpTargetObj->MapNumber) )
	{
		return;
	}
#endif

#ifdef GENS
	if( gGensSystem.IsMapBattleZone(lpObj->MapNumber) )
	{
		//-> Offed for time, becoz Debuff its double check of map
		//gGensSystem.SendPKPenaltyDebuff(lpObj);
		// ----
		//-> Temp check, be cool to use ::IsPKEnable
		if( gGensSystem.GetGensInfluence(lpObj) != gGensSystem.GetGensInfluence(lpTargetObj) )
		{
			return;
		}
	}
#endif

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpTargetObj->SelfDefense[n] >= 0 )
		{
			if ( lpTargetObj->SelfDefense[n] == lpObj->m_Index )
			{
				return;
			}

			if(iPartyNumber >= 0)
			{
				if(gObj[lpTargetObj->SelfDefense[n]].PartyNumber == iPartyNumber)
				{
					return;
				}
			}
		}
	}

	int iOriginPkLevel = lpObj->m_PK_Level;
	int iOriginPkCount = lpObj->m_PK_Count;

	if(MSBDamage == 4) //season3 add-on
	{
		return;
	}
	
	if(g_PKLevelIncreaseOff == 1)
	{
		return;
	}

	BYTE btDefaultPartyPkLevel = 3; //Season 2.5 add-on

	if(lpTargetObj->PartyNumber >= 0) //Season 2.5 add-on
	{
		if( gParty.GetPKPartyPenalty(lpTargetObj->PartyNumber) >= 5 )
		{
			btDefaultPartyPkLevel = gParty.GetPKPartyPenalty(lpTargetObj->PartyNumber);
		}
		else
		{
			btDefaultPartyPkLevel = lpTargetObj->m_PK_Level;
		}
	}
	else
	{
		btDefaultPartyPkLevel = lpTargetObj->m_PK_Level;
	}

	if ( lpObj->m_PK_Level == 3 )
	{
		if( ((char)btDefaultPartyPkLevel) == 3 ) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else if(((char)btDefaultPartyPkLevel) < 3) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else if(((char)btDefaultPartyPkLevel) == 4) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else
		{
			if(((char)btDefaultPartyPkLevel) >= 6 && lpTargetObj->Level > 20) //Season 2.5 changed
			{
				lpObj->m_PK_Count--;

				if(lpObj->m_PK_Count < -3)
				{
					lpObj->m_PK_Count = (BYTE)-3;
					return;
				}
			}
			else
			{
				return;
			}
		}
	}
	else if( lpObj->m_PK_Level > 3)
	{
		if(((char)btDefaultPartyPkLevel) <= 4) //Season 2.5 changed
		{
			lpObj->m_PK_Count++;
			if(lpObj->m_PK_Count > 100)
			{
				lpObj->m_PK_Count = 100;
			}
		}
		else
		{
			return;
		}
	}
	else if( lpObj->m_PK_Level < 3)
	{
		if(((char)btDefaultPartyPkLevel) == 3) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else if(((char)btDefaultPartyPkLevel) < 3) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else if(((char)btDefaultPartyPkLevel) == 4) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else
		{
			if(((char)btDefaultPartyPkLevel) >= 6 && lpTargetObj->Level > 20) //Season 2.5 changed
			{
				lpObj->m_PK_Count--;

				if(lpObj->m_PK_Count < -3)
				{
					lpObj->m_PK_Count = (BYTE)-3;
				}
			}
			else
			{
				return;
			}
		}
	}

	if(lpObj->m_PK_Count == 0)
	{
		lpObj->m_PK_Level = 3;
		lpObj->m_PK_Time = 0;
	}
	else if(lpObj->m_PK_Count == 1)
	{
		lpObj->m_PK_Level = 4;
		lpObj->m_PK_Time = 0;
	}
	else if(lpObj->m_PK_Count == 2)
	{
		lpObj->m_PK_Level = 5;
		lpObj->m_PK_Time = 0;

		if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
		{
			gParty.UpdatePKUserInfo(lpObj->PartyNumber,lpObj->m_Index,lpObj->DBNumber,lpObj->m_PK_Level);
			gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
		}
	}
	else if(lpObj->m_PK_Count >= 3)
	{
		lpObj->m_PK_Level = 6;
		lpObj->m_PK_Time = 0;

		if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
		{
			gParty.UpdatePKUserInfo(lpObj->PartyNumber,lpObj->m_Index,lpObj->DBNumber,lpObj->m_PK_Level);
			gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
		}
	}
	else if(lpObj->m_PK_Count <= -3)
	{
		lpObj->m_PK_Level = 1;
		lpObj->m_PK_Time = 0;
	}
	
	GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);

	if ( lpObj->m_PK_Level == 4 )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 113)), lpObj->m_Index, 1);
	}

#ifdef __NOVUS__
	if( gMUHelper.GetUserSlot(lpTargetObj->m_Index) != (int)-1 )
	{
		FHP_FRIEND_MEMO_SEND pMsg;
		char Mem[200];
		ZeroMemory(Mem, sizeof(Mem));
		sprintf(Mem, "I killed you while you sleep!");
		pMsg.h.set((LPBYTE)&pMsg, 0x70, sizeof(pMsg));
		pMsg.WindowGuid		= 1;
		pMsg.MemoSize		= sizeof(Mem);
		pMsg.Number			= lpObj->m_Index;
		pMsg.Dir			= 1;
		pMsg.Action			= 1;
		sprintf(pMsg.Subject, "AFK Notification");
		sprintf(pMsg.Name, lpObj->Name);
		memcpy(pMsg.Photo, lpObj->CharSet, sizeof(pMsg.Photo));
		memcpy(pMsg.ToName, lpTargetObj->Name, sizeof(pMsg.ToName));
		memcpy(pMsg.Memo, Mem, sizeof(Mem));
		wsExDbCli.DataSend((PCHAR)&pMsg, sizeof(pMsg));
	}
#endif

	LogAddTD("[%s][%s] PK Info Set (Killed Player) Origin(PkLevel:%d, PkCount:%d) Changed(PkLevel:%d, PkCount:%d)",	lpObj->AccountID, lpObj->Name, iOriginPkLevel, iOriginPkCount, lpObj->m_PK_Level, lpObj->m_PK_Count);
}

void gObjUserDie(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int count = 76;
	int itemdrop = 1;
	int number = 0;
	int dropresult = 0;

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	gObjClearBuffEffect(lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT);

	gObjSetKillCount(lpObj->m_Index,0);

	gObjUseSkill.RemoveAllCharacterInvalidMagicAndSkillState(lpObj);

	if(gObjTargetGuildWarCheck(lpObj,lpTargetObj) == 1)
	{
		return;
	}

	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			LPMONSTER_ATTRIBUTE mAttr = gMAttr.GetAttr(lpTargetObj->Class);

			if(mAttr != 0)
			{
				LogAddTD("[DevilSquare] Dead In DevilSquare [%s][%s][%s]",lpObj->AccountID,lpObj->Name,mAttr->m_Name);
			}
		}

		return;
	}
	
	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		g_BloodCastle.SetUserState(lpObj->m_Index,1);

		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(lpObj->MapNumber);

		if(g_BloodCastle.GetCurrentState(iBridgeIndex) == BC_STATE_PLAYING)
		{
			LogAddTD("[Blood Castle] (%d) Try to drop Ultimate Weapon [%s][%s]",iBridgeIndex+1,lpObj->AccountID,lpObj->Name);
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		else
		{
			LogAddTD("[Blood Castle] (%d) Try to delete Ultimate Weapon [%s][%s]",iBridgeIndex+1,lpObj->AccountID,lpObj->Name);
			g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
		}

		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			LPMONSTER_ATTRIBUTE mAttr = gMAttr.GetAttr(lpTargetObj->Class);

			if(mAttr != 0)
			{
				LogAddTD("[Blood Castle] (%d) Dead In Blood Castle, Killed by Monster [%s][%s][%s]",iBridgeIndex+1,lpObj->AccountID,lpObj->Name,mAttr->m_Name);
				g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
			}
		}
		else if(lpTargetObj->Type == OBJ_USER)
		{
			LogAddTD("[Blood Castle] (%d) Dead In Blood Castle, Killed by Other User [%s][%s][%s]",iBridgeIndex+1,lpObj->AccountID,lpObj->Name,lpTargetObj->Name);
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		return;
	}
	else if(IT_MAP_RANGE(lpObj->MapNumber)) //season2.5 add-on
	{
		g_IllusionTempleEvent.IllusionTempleUserDie(lpObj);
		return;
	}
	else if(CC_MAP_RANGE(lpObj->MapNumber))
	{
		return;
	}

//#if(_GSCS==1)
	if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE && lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
	{
		return;
	}
//#endif

	if((lpObj->Authority & 0x10) || (lpTargetObj->Authority & 0x10))
	{
		return;
	}

	if(gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_GUARDIAN_AMULET) != 0) //season4.5 add-on
	{
		return;
	}

	if(lpTargetObj->Type == OBJ_MONSTER)
	{
		LogAddTD("[%s][%s] Killed [%s][%s] Pos:(%d,%d,%d)","Monster",lpTargetObj->Name,lpObj->AccountID,lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y);
	}
	else
	{
		LogAddTD("[%s][%s] (PkLevel = %d) Killed [%s][%s] (PkLevel = %d) Pos:(%d,%d,%d)",lpTargetObj->AccountID,lpTargetObj->Name,lpTargetObj->m_PK_Level,lpObj->AccountID,lpObj->Name,lpObj->m_PK_Level,lpObj->MapNumber,lpObj->X,lpObj->Y);
#ifdef GENS
		LogAddTD("Killed by User State Victim:[%s][%s], Murderer:[%s][%s], Dead State (Duel:%d, GuildWar:%d, SeldDefense:%d, GensWar:%d)",
			lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name,
			!!IsOnDuel(lpTargetObj->m_Index,lpObj->m_Index),
			!!gObjTargetGuildWarCheck(lpObj,lpTargetObj), 
			!!gObjIsSelfDefense(lpTargetObj,lpObj->m_Index),
			!!gGensSystem.IsMapBattleZone(lpObj->MapNumber));
#else
		LogAddTD("Killed by User State Victim:[%s][%s], Murderer:[%s][%s], Dead State (Duel:%d, GuildWar:%d, SeldDefense:%d",
			lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name,
			!!IsOnDuel(lpTargetObj->m_Index,lpObj->m_Index),
			!!gObjTargetGuildWarCheck(lpObj,lpTargetObj), 
			!!gObjIsSelfDefense(lpTargetObj,lpObj->m_Index));
#endif
	}

#ifdef GENS
	//-> Bad arg position???
	// Fine on play		= lpTargetObj, lpObj
	// Original			= lpObj, lpTargetObj
	gGensSystem.CalcContributePoint(lpTargetObj, lpObj);
	//-> Temp, for life-time update counter
	gGensSystem.SendGensInfo(lpObj);
	gGensSystem.SendGensInfo(lpTargetObj);
#endif

	if(gPkLimitFree == 0)
	{
		if(lpObj->m_PK_Level == 2)
		{
			itemdrop = rand()%32;
		}
		else if(lpObj->m_PK_Level == 3)
		{
			itemdrop = rand()%16;
		}
		else if(lpObj->m_PK_Level == 4)
		{
			itemdrop = rand()%8;
		}
		else if(lpObj->m_PK_Level == 5)
		{
			itemdrop = rand()%4;
		}
		else if(lpObj->m_PK_Level >= 6)
		{
			itemdrop = rand()%2;
		}
		else
		{
			itemdrop = rand()%64;
		}
	}
	else
	{
		if(lpObj->m_PK_Level == 2)
		{
			itemdrop = rand()%128;
		}
		else if(lpObj->m_PK_Level == 3)
		{
			itemdrop = rand()%64;
		}
		else if(lpObj->m_PK_Level == 4)
		{
			itemdrop = rand()%16;
		}
		else if(lpObj->m_PK_Level == 5)
		{
			itemdrop = rand()%4;
		}
		else if(lpObj->m_PK_Level >= 6)
		{
			itemdrop = rand()%2;
		}
#ifdef PARTYKILLPKSET
		else
		{
			itemdrop = rand()%256;
		}
#else
		else
		{
			itemdrop = 0;
		}
#endif
	}

	if( gPkItemDrop == 0 )
	{
		//if(lpTargetObj->m_PK_Level >= 4/* && gPkLimitFree == 0*/)
		if( lpTargetObj->Type == OBJ_USER )
		{
			itemdrop = 1;
		}
	}

	if(gLanguage == 0)
	{
		if(lpTargetObj->Type == OBJ_USER && gPkLimitFree == 0)
		{
			itemdrop = 1;
		}
	}

	if(gObjCanItemTouch(lpObj,0)==0)
	{
		itemdrop = 1;
	}

	if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if(IsOnDuel(lpObj->m_Index,lpTargetObj->m_Index))
		{
			itemdrop = 1;
		}
	}

#ifdef GENS
	if( gGensSystem.IsMapBattleZone(lpObj->MapNumber) )
	{
		itemdrop = true;
	}
	else
	{
		gGensSystem.PkPenaltyDropInvenItem(lpObj);
	}
#endif

//#if(_GSCS==1)
	if(g_Crywolf.GetCrywolfState() == 4)
	{
		if(CRYWOLF_MAP_RANGE(lpObj->MapNumber))
		{
			itemdrop = 1;
		}
	}
//#endif

#ifndef CLIENT_2

	if(itemdrop == false)
	{
		if(lpObj->m_PK_Level >= 4)
		{
			count = 24;

			while(count-- != 0)
			{
#ifndef ZEONGAMESERVER
				number = rand()%12;

				if(lpObj->pInventory[number].IsItem()==1)
				{
					if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[number])==0)
					{
						PMSG_ITEMTHROW lpMsg;
						lpMsg.Ipos = number;
						lpMsg.px = lpObj->X;
						lpMsg.py = lpObj->Y;
						if( CGItemDropRequest(&lpMsg, lpObj->m_Index, 1) == 1 )
						{
							dropresult = 1;
							break;
						}
					}
				}
#else 
				int DropItemByPos[11] = { 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12 };

				if( lpObj->m_PK_Level >= 4 && gPkItemDrop )
				{
					if(lpObj->pInventory[DropItemByPos[rand()%11]].IsItem() == 1)
					{
						if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[number]) == 0)
						{
							PMSG_ITEMTHROW lpMsg;

							lpMsg.Ipos	= DropItemByPos[rand()%11];
							lpMsg.px	= lpObj->X;
							lpMsg.py	= lpObj->Y;

							if( CGItemDropRequest(&lpMsg, lpObj->m_Index, 1) == 1 )
							{
								dropresult = 1;
								break;
							}
						}
					}
#endif
				}
			}
		}

		if(dropresult == 0)
		{
			count = INVENTORY_MAP_SIZE;

			while(count-- != 0)
			{
				number = rand()%INVENTORY_MAP_SIZE + 12;
#ifdef ZEONGAMESERVER
				if( lpObj->m_PK_Level >= 4 && gPkItemDrop )
				{
#endif
					if(lpObj->pInventory[number].IsItem()==1)
					{
						if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[number])==0)
						{
							if(lpObj->pInventory[number].m_Type >= ITEMGET(13,20) && (lpObj->pInventory[number].m_Level >= 1 && lpObj->pInventory[number].m_Level <= 2 ))
							{
								continue;
							}

							PMSG_ITEMTHROW lpMsg;
							lpMsg.Ipos = number;
							lpMsg.px = lpObj->X;
							lpMsg.py = lpObj->Y;

							if(CGItemDropRequest(&lpMsg,lpObj->m_Index,1)==1)
							{
								dropresult = 1;
								LogAddTD("[%s][%s] User Die Item Drop, ItemName:%s",lpObj->AccountID,lpObj->Name,lpObj->pInventory[number].GetName());
								break;
							}
						}
					}
				}
#ifdef ZEONGAMESERVER
			}
#endif
		}
	}

#endif

#ifdef GENS
	gGensSystem.PkPenaltyDropZen(lpObj);
#endif
	if( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER ) 
	{
		if( IsOnDuel(lpObj->m_Index, lpTargetObj->m_Index) )
		{
#ifdef NPVP
			g_NewPVP.SetScore(*lpTargetObj); // 1.00.92
#endif
		}
		return;
	}

	__int64 minexp = gLevelExperience[-1+lpObj->Level];
	__int64 maxexp = gLevelExperience[lpObj->Level];

	__int64 subexp = 0;
	__int64 decexp = 0;

	int decexprate = 0;

	BYTE btDefaultPartyPkLevel = 3; //Season 2.5 add-on

	if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
	{
		if( (gParty.GetPKPartyPenalty(lpObj->PartyNumber)) >= 5 )
		{
			btDefaultPartyPkLevel = gParty.GetPKPartyPenalty(lpObj->PartyNumber);
		}
		else
		{
			btDefaultPartyPkLevel = lpObj->m_PK_Level;
		}
	}
	else
	{
		btDefaultPartyPkLevel = lpObj->m_PK_Level;
	}

	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) != FALSE) //Season3 add-on
	{
		minexp = g_MasterLevelSystem.GetMasterLevelExpTlbInfo(lpObj->m_nMasterLevel);
		maxexp = g_MasterLevelSystem.GetMasterLevelExpTlbInfo(lpObj->m_nMasterLevel+1);

		decexprate = g_MasterLevelSystem.GetDieDecExpRate(lpObj);
	}
	else if(lpObj->Level <= 10)
	{
		decexprate = 0;
	}
	else if(lpObj->Level <= 150)
	{
		if(((char)btDefaultPartyPkLevel) == 2) //Season 2.5 changed
		{
			decexprate = 20;
		}
		else if(((char)btDefaultPartyPkLevel) == 3) //Season 2.5 changed
		{
			decexprate = 10; //Season 2.5 changed
		}
		else if(((char)btDefaultPartyPkLevel) == 4) //Season 2.5 changed
		{
			decexprate = 50;
		}
		else if(((char)btDefaultPartyPkLevel) == 5) //Season 2.5 changed
		{
			decexprate = 60;
		}
		else if(((char)btDefaultPartyPkLevel) >= 6) //Season 2.5 changed
		{
			decexprate = 70;
		}
		else
		{
			decexprate = 20;
		}
	}
	else if(lpObj->Level <= 220)
	{
		if(((char)btDefaultPartyPkLevel) == 2) //Season 2.5 changed
		{
			decexprate = 10;
		}
		else if(((char)btDefaultPartyPkLevel) == 3) //Season 2.5 changed
		{
			decexprate = 10; //Season 2.5 changed
		}
		else if(((char)btDefaultPartyPkLevel) == 4) //Season 2.5 changed
		{
			decexprate = 40;
		}
		else if(((char)btDefaultPartyPkLevel) == 5) //Season 2.5 changed
		{
			decexprate = 50;
		}
		else if(((char)btDefaultPartyPkLevel) >= 6) //Season 2.5 changed
		{
			decexprate = 60;
		}
		else
		{
			decexprate = 10;
		}
	}
	else
	{
		if(lpObj->m_PK_Level == 2)
		{
			decexprate = 10;
		}
		else if(((char)btDefaultPartyPkLevel) == 3) //Season 2.5 changed
		{
			decexprate = 10; //Season 2.5 changed
		}
		else if(((char)btDefaultPartyPkLevel) == 4) //Season 2.5 changed
		{
			decexprate = 30;
		}
		else if(((char)btDefaultPartyPkLevel) == 5) //Season 2.5 changed
		{
			decexprate = 40;
		}
		else if(((char)btDefaultPartyPkLevel) >= 6) //Season 2.5 changed
		{
			decexprate = 50;
		}
		else
		{
			decexprate = 10;
		}
	}

	decexp = 0;

	if(decexprate > 0)
	{
		decexp = ((maxexp - minexp)*decexprate)/1000;
	}	

//#if(_GSCS==1) //Check Position in gs 77 after siege patch
	if(g_Crywolf.GetCrywolfState() == 4)
	{
		if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
		{
			decexp = 0;
		}
	}
//#endif

	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) != FALSE) //Season3 add-on
	{
		subexp = lpObj->m_i64MasterLevelExp - decexp;
	}
	else
	{
		subexp = lpObj->Experience - decexp;
	}

	if(subexp < minexp)
	{
		subexp = minexp;
	}

	LogAddTD("[%s][%s] Death reduces Experience %u - %d",lpObj->AccountID,lpObj->Name,subexp,decexp);

	DWORD submoney = 0;
	int iMoneyLevel = 0;

	if(lpObj->Money > 0)
	{
		iMoneyLevel = g_MasterLevelSystem.GetDieDecZenRate(lpObj);

		if(iMoneyLevel < 0)
		{
			if(lpObj->Level > 10 && lpObj->Level <= 99)
			{
				iMoneyLevel = 1; //submoney = (DWORD)(lpObj->Money)/100;
			}
			else if(lpObj->Level >= 100 && lpObj->Level <= 199)
			{
				iMoneyLevel = 2; //submoney = (DWORD)(lpObj->Money*2)/100;
			}
			else if(lpObj->Level > 200)
			{
				iMoneyLevel = 3; //submoney = (DWORD)(lpObj->Money*3)/100;
			}
		}

		if(iMoneyLevel > 0)
		{
			submoney =  (DWORD)(lpObj->Money * iMoneyLevel) / 100;
		}

		lpObj->Money = lpObj->Money - submoney;
		
		if(lpObj->Money < 0)
		{
			lpObj->Money = 0;
		}

		LogAddTD(lMsg.Get(574),lpObj->AccountID,lpObj->Name,lpObj->Money,submoney);
	}

	if(lpObj->WarehouseMoney > 0)
	{
		submoney = 0;

		iMoneyLevel = g_MasterLevelSystem.GetDieDecZenRate(lpObj);

		if(iMoneyLevel < 0)
		{
			if(lpObj->Level > 10 && lpObj->Level <= 99)
			{
				iMoneyLevel = 1; //submoney = (DWORD)(lpObj->WarehouseMoney)/100;
			}
			else if(lpObj->Level >= 100 && lpObj->Level <= 199)
			{
				iMoneyLevel = 2; //submoney = (DWORD)(lpObj->WarehouseMoney*2)/100;
			}
			else if(lpObj->Level > 200)
			{
				iMoneyLevel = 3; //submoney = (DWORD)(lpObj->WarehouseMoney*3)/100;
			}
		}

		if(iMoneyLevel > 0)
		{
			submoney = (DWORD)(lpObj->Money * iMoneyLevel) / 100; //WZ Bug on Season3 (should be lpObj->WarehouseMoney like it was)
		}

		lpObj->WarehouseMoney = lpObj->WarehouseMoney - submoney;
		
		if(lpObj->WarehouseMoney < 0)
		{
			lpObj->WarehouseMoney = 0;
		}

		GDSetWarehouseMoney(lpObj->m_Index);

		LogAddTD(lMsg.Get(575),lpObj->AccountID,lpObj->Name,lpObj->WarehouseMoney,submoney);
	}

	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) != FALSE) //Season3 add-on
	{
		lpObj->m_i64MasterLevelExp = subexp;
	}
	else
	{
		lpObj->Experience = subexp;
	}
}

void gObjInterfaceTimeCheck(LPOBJ lpObj)
{
	int tObjNum;
	BOOL TimeCheck = FALSE;

	if ( lpObj->m_IfState.use == 0 )
	{
		return;
	}

	if ( (GetTickCount() - lpObj->m_InterfaceTime) < 5000 )
	{
		return;
	}

	if ( lpObj->m_IfState.state == 1 )
	{
		return;
	}

	tObjNum = lpObj->TargetNumber;

	if ( tObjNum < 0 )
	{
		return;
	}

	if ( lpObj->m_IfState.type == 1 )
	{
		CGTradeResult(lpObj->m_Index, 3);
		CGTradeResult(tObjNum, 3);

		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 114)), tObjNum, 1);
		TimeCheck = TRUE;
	}
	else
	{
		if ( lpObj->m_IfState.type == 2 )
		{
			lpObj->PartyTargetUser = -1;
			gObj[tObjNum].PartyTargetUser = -1;
			GCResultSend(tObjNum, 0x41, FALSE );
			TimeCheck = TRUE;
		}
	}

	if ( TimeCheck != FALSE )
	{
		lpObj->m_IfState.use = 0;
		lpObj->TargetNumber = -1;
		gObj[tObjNum].m_IfState.use = 0;
		gObj[tObjNum].TargetNumber = -1;
	}

}

void gObjPkDownTimeCheck(LPOBJ lpObj, int TargetLevel)
{

	if(lpObj->m_PK_Level == 3)
	{
		return;
	}

	DWORD dwtime = ((GetTickCount() - lpObj->m_dwPKTimer) / 1000) * gPkTime;

	if(dwtime > 5)
	{
		dwtime = 2;
		lpObj->m_dwPKTimer = GetTickCount();
	}
	else
	{
		lpObj->m_dwPKTimer = dwtime * 1000;
	}

	if(TargetLevel < 2)
	{
		lpObj->m_PK_Time += dwtime;
	}
	else
	{
		lpObj->m_PK_Time += TargetLevel/2; //Season 2.5 changed (Fix?)
	}

	if(lpObj->m_PK_Level < 3)
	{
		if(lpObj->m_PK_Time > 3600)
		{
			lpObj->m_PK_Time = 0;
			lpObj->m_PK_Count += 1;

			if(lpObj->m_PK_Count > 100)
			{
				lpObj->m_PK_Count = 100;
			}

			lpObj->m_PK_Level += 1;

			GCPkLevelSend(lpObj->m_Index,lpObj->m_PK_Level);
		}
	}
	else if(lpObj->m_PK_Level > 3)
	{
		if(lpObj->m_PK_Count > 3)
		{
			if(lpObj->m_PK_Time > 3600)
			{
				lpObj->m_PK_Time = 0;
				lpObj->m_PK_Count -= 1;

				if(lpObj->m_PK_Count <= 0)
				{
					lpObj->m_PK_Count = 0;
					lpObj->m_PK_Level = 3;
				}
				else if(lpObj->m_PK_Count == 2)
				{
					lpObj->m_PK_Level = 5;
				}
				else if(lpObj->m_PK_Count == 1)
				{
					lpObj->m_PK_Level = 4;
				}

				if(lpObj->m_PK_Level <= 5)
				{
					GCPkLevelSend(lpObj->m_Index,lpObj->m_PK_Level);
				}

				if(lpObj->m_PK_Level == 4)
				{
					GCServerMsgStringSend(lMsg.Get(1137),lpObj->m_Index,1);

					if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
					{
						gParty.UpdatePKUserInfo(lpObj->PartyNumber,lpObj->m_Index,lpObj->DBNumber,lpObj->m_PK_Level);
						gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
					}
				}
			}
		}
		else if(lpObj->m_PK_Time > 10800)
		{
			lpObj->m_PK_Time = 0;
			lpObj->m_PK_Count -= 1;

			if(lpObj->m_PK_Count <= 0)
			{
				lpObj->m_PK_Count = 0;
				lpObj->m_PK_Level = 3;
			}
			else if(lpObj->m_PK_Count == 2)
			{
				lpObj->m_PK_Level = 5;
			}
			else if(lpObj->m_PK_Count == 1)
			{
				lpObj->m_PK_Level = 4;
			}

			if(lpObj->m_PK_Level <= 5)
			{
				GCPkLevelSend(lpObj->m_Index,lpObj->m_PK_Level);
			}

			if(lpObj->m_PK_Level == 4)
			{
				GCServerMsgStringSend(lMsg.Get(1137),lpObj->m_Index,1);

				if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
				{
					gParty.UpdatePKUserInfo(lpObj->PartyNumber,lpObj->m_Index,lpObj->DBNumber,lpObj->m_PK_Level);
					gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
				}
			}
		}
	}
}

BOOL gObjAngelSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Angel = &lpObj->pInventory[8];

	if ( Angel->m_Type == ITEMGET(13,0) && Angel->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjSatanSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Satan = &lpObj->pInventory[8];

	if ( Satan->m_Type == ITEMGET(13,1) && Satan->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjDemonSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Demon = &lpObj->pInventory[8];

	if ( Demon->m_Type == ITEMGET(13,64) && Demon->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjSpiritGuardianSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * SpiritGuardian = &lpObj->pInventory[8];

	if ( SpiritGuardian->m_Type == ITEMGET(13,65) && SpiritGuardian->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjPandaSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Panda = &lpObj->pInventory[8];

	if ( Panda->m_Type == ITEMGET(13,80) && Panda->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjUnicornSprite(LPOBJ lpObj) // Season 5 Episode 2 JPN
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}
	// ----
	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}
	// ----
	CItem * Unicorn = &lpObj->pInventory[8];
	// ----
	if ( Unicorn->m_Type == ITEMGET(13, 106) && Unicorn->m_Durability > 0.0f )
	{
		return TRUE;
	}
	// ----
	return FALSE;
}

BOOL gObjSkeletonSprite(LPOBJ lpObj) // Season 5 Episode 2 JPN
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}
	// ----
	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}
	// ----
	CItem * Skeleton = &lpObj->pInventory[8];
	// ----
	if ( Skeleton->m_Type == ITEMGET(13, 123) && Skeleton->m_Durability > 0.0f )
	{
		return TRUE;
	}
	// ----
	return FALSE;
}
//1.01.03
BOOL gObjFindInventoryEquipment(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	if( gObj[aIndex].Type != 1 )
	{
		return false;
	}
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		gObjSetInventoryEquipment(lpObj, i);
	}
	// ----
	return true;
}

//1.01.03
BOOL gObjSetInventoryEquipment(LPOBJ lpObj, BYTE btPos)
{
	if( lpObj->Type != 1 )
	{
		return false;
	}
	// ----
	if( lpObj->pInventory[btPos].IsItem() )
	{
		switch(lpObj->pInventory[btPos].m_Type)
		{
		case ITEMGET(13, 128):
		case ITEMGET(13, 129):
			{
				if( lpObj->pInventory[btPos].m_Durability == 254.0
					&& (lpObj->m_btSculptPos < MAIN_INVENTORY_SIZE 
					|| lpObj->m_btSculptPos >= MAIN_INVENTORY_SIZE) )
				{
					lpObj->m_btSculptPos = btPos;
				}
			}
			break;
		case ITEMGET(13, 130):
		case ITEMGET(13, 131):
		case ITEMGET(13, 132):
		case ITEMGET(13, 133):
			{
				if( lpObj->pInventory[btPos].m_Durability == 254.0
					&& (lpObj->m_btSculptPos < MAIN_INVENTORY_SIZE 
					|| lpObj->m_btSculptPos >= MAIN_INVENTORY_SIZE) )
				{
					lpObj->m_btCharmPos = btPos;
				}
			}
			break;
		case ITEMGET(13, 134):
			{
				if( lpObj->pInventory[btPos].m_Durability == 254.0
					&& (lpObj->m_btSculptPos < MAIN_INVENTORY_SIZE 
					|| lpObj->m_btSculptPos >= MAIN_INVENTORY_SIZE) )
				{
					lpObj->m_btArtifactPos = btPos;
				}
			}
			break;
		}
	}
	// ----
	return true;
}

//1.01.03
BOOL gObjInventoryEquipment(LPOBJ lpObj)
{
	if( lpObj->Type != 1 )
	{
		return false;
	}
	// ----
	BUFF_EFFECT_DATE * lpBuffData	= 0;
	ITEMEFFECT * lpItemEffect		= 0;	//???
	// ----
	if(		lpObj->m_btSculptPos >= INVETORY_WEAR_SIZE 
		&&	lpObj->m_btSculptPos < MAIN_INVENTORY_SIZE )
	{
		lpBuffData = g_BuffEffectSlot.GetEffectDataFromItemCode(lpObj->pInventory[lpObj->m_btSculptPos].m_Type);
		// ----
		if( !lpBuffData )
		{
			return false;
		}
		// ----
		if( lpObj->pInventory[lpObj->m_btSculptPos].m_Durability == 254.0 )
		{
			gObjAddBuffEffect(lpObj, lpBuffData->btIndex);
		}
		else
		{
			lpObj->m_btSculptPos = 0;
			gObjRemoveBuffEffect(lpObj, lpBuffData->btIndex);
		}
	}
	// ----
	if(		lpObj->m_btCharmPos >= INVETORY_WEAR_SIZE 
		&&	lpObj->m_btCharmPos < MAIN_INVENTORY_SIZE )
	{
		lpBuffData = g_BuffEffectSlot.GetEffectDataFromItemCode(lpObj->pInventory[lpObj->m_btCharmPos].m_Type);
		// ----
		if( !lpBuffData )
		{
			return false;
		}
		// ----
		if( lpObj->pInventory[lpObj->m_btCharmPos].m_Durability == 254.0 )
		{
			gObjAddBuffEffect(lpObj, lpBuffData->btIndex);
		}
		else
		{
			lpObj->m_btCharmPos = 0;
			gObjRemoveBuffEffect(lpObj, lpBuffData->btIndex);
		}
	}
	// ----
	if(		lpObj->m_btArtifactPos >= INVETORY_WEAR_SIZE 
		&&	lpObj->m_btArtifactPos < MAIN_INVENTORY_SIZE )
	{
		lpBuffData = g_BuffEffectSlot.GetEffectDataFromItemCode(lpObj->pInventory[lpObj->m_btArtifactPos].m_Type);
		// ----
		if( !lpBuffData )
		{
			return false;
		}
		// ----
		if( lpObj->pInventory[lpObj->m_btArtifactPos].m_Durability == 254.0 )
		{
			gObjAddBuffEffect(lpObj, lpBuffData->btIndex);
		}
		else
		{
			lpObj->m_btArtifactPos = 0;
			gObjRemoveBuffEffect(lpObj, lpBuffData->btIndex);
		}
	}
	// ----
	return true;
}

BOOL gObjWingSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[7].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Wing = &lpObj->pInventory[7];

	if ( (Wing->m_Type >= ITEMGET(12,0) && Wing->m_Type <= ITEMGET(12,6)) || Wing->m_Type <= ITEMGET(13,30) || 
		 (Wing->m_Type >= ITEMGET(12,36) && Wing->m_Type <= ITEMGET(12,40)) || //Season 2.5 Third Wing add-on
		 Wing->m_Type == ITEMGET(12,41) || Wing->m_Type == ITEMGET(12,42) || Wing->m_Type == ITEMGET(12,43)
		 || Wing->m_Type == ITEMGET(12, 49)
		 || Wing->m_Type == ITEMGET(12, 50)
#ifdef NEWWINGS
		 || IS_NEWWINGS(Wing->m_Type)
#endif
		 )
	{
		if ( Wing->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL gObjUniriaSprite(LPOBJ lpObj)
{
	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Uniria = &lpObj->pInventory[8];

	if ( Uniria->m_Type == ITEMGET(13,2) && Uniria->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

typedef struct tagITEM_BYTE_DB
{
	BYTE btType;
	BYTE btOption;
	BYTE btDurability;
	BYTE btExcellentOption;
	BYTE btAncientOption;

} ITEM_BYTE_DB, * LPITEM_BYTE_DB;

//0056ccb0
void gObjSpriteDamage(LPOBJ lpObj, int damage)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

#ifdef PERIOD
	//-> Fucking WZ illusion xD
	if( lpObj->pInventory[8].IsPeriodItem() )
	{
		return;
	}
#endif

	int send_dur = 0;

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return;
	}

	float fN = 10.0f;
	CItem * sprite = &lpObj->pInventory[8];
	float fdamage = damage;

	
	if ( sprite->m_Type == ITEMGET(13, 0)  || // angel 
		//sprite->m_Type == ITEMGET(13, 67)  || //season4 add-on
		sprite->m_Type == ITEMGET(13, 106) 
		|| sprite->m_Type == ITEMGET(13, 123) 
		) // Season 5 Episode 2 JPN
	{
		fdamage = (damage*3)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}
	else if ( sprite->m_Type == ITEMGET(13,1) ) //satan
	{
		fdamage = (damage*2)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}
	
	if ( sprite->m_Type == ITEMGET(13,64) || //season4 add-on 
		sprite->m_Type == ITEMGET(13,65) ||
		sprite->m_Type == ITEMGET(13,80))//4.6 panda
	{
		fN += 10.0f;
	}

	if( sprite->m_Type == ITEMGET(13, 67)		// Ruloph
		|| sprite->m_Type == ITEMGET(13, 64) )	// Demon
	{
		fdamage					= (damage*2)/10.0f;
		fdamage					/= fN;
		sprite->m_Durability	-= fdamage;
		send_dur				= 1;
	}
	else if ( sprite->m_Type == ITEMGET(13,65) ) //Spirit Guardian (Season 3.5 add on)
	{
		fdamage = (damage*2)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}
	else if ( sprite->m_Type == ITEMGET(13,80) ) //4.6 panda
	{
		fdamage = (damage*2)/10.0f;
		fdamage /= fN;
		// ----
#ifdef __ALIEN__
		if( !g_SuperPanda )
		{
			sprite->m_Durability -= fdamage;
			send_dur = 1;
		}
#else
		sprite->m_Durability -= fdamage;
		send_dur = 1;
#endif
	}
	else if ( sprite->m_Type == ITEMGET(13,2) )
	{
		fdamage = (damage)/10.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}
	else if ( sprite->m_Type == ITEMGET(13,3) )
	{
		fdamage = (damage)/20.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}		
	else if ( sprite->m_Type == ITEMGET(13,4) )
	{
		if ( sprite->m_Durability < 1.0f )
		{
			return;
		}

		int decdmg = damage * gIncreaseDarkHorseAttackDamagePercent / 100;
		damage += decdmg;
		fdamage = (damage*2)/10.0f;
		fdamage /= 10.0f;
		fdamage += 1.0f;
		sprite->m_DurabilitySmall += (int)fdamage - decdmg;

		int inc_def = 1500 + (1500 * gIncreaseDarkHorseDefencePercent /100);

		if ( sprite->m_DurabilitySmall > lpObj->m_MPSkillOpt.MpsPetDurDownSpeed + inc_def )
		{
			sprite->m_Durability -= 1.0f;
			sprite->m_DurabilitySmall = 0;
			send_dur = 1;

			if ( sprite->m_Durability < 1.0f )
			{
				sprite->m_Durability = 0;

				int iPetExperience = lpObj->pInventory[1].m_PetItem_Exp;

				if ( sprite->DecPetItemExp(10) != FALSE )
				{
					LogAddTD("[%s][%s][PetItemExpDown] [%s] Level:[%d]Exp:[%d]DecExp[%d]", lpObj->AccountID, lpObj->Name, lpObj->pInventory[8].GetName(), lpObj->pInventory[8].m_PetItem_Level, lpObj->pInventory[8].m_PetItem_Exp, iPetExperience - lpObj->pInventory[1].m_PetItem_Exp);
					CDarkSpirit::SendLevelmsg(lpObj->m_Index, 8, 1, (BYTE)-1);
				}

				LogAddTD("[%s][%s] ???©??????»? Item is Broken because durability is exhausted [%u]", lpObj->AccountID,lpObj->Name,lpObj->pInventory[8].m_Number);
			}

			GCItemDurSend(lpObj->m_Index, 8, sprite->m_Durability, 0);
		}
	
		return;
	}
	else if(sprite->m_Type == ITEMGET(13,37))
	{
		if ( sprite->m_Durability < 1.0f )
		{
			return;
		}

		int iMaxDurSmall = g_iFenrirDefaultMaxDurSmall;
		int iMultipleDmg = 2;

		if(lpObj->Class == CLASS_WIZARD)
		{

		}
		else if(lpObj->Class == CLASS_KNIGHT)
		{

		}
		else if(lpObj->Class == CLASS_ELF)
		{
			iMaxDurSmall = g_iFenrirElfMaxDurSmall;
			iMultipleDmg = 2;
		}
		else if(lpObj->Class == CLASS_MAGUMSA)
		{
		}
		else if(lpObj->Class == CLASS_DARKLORD)
		{
		}
		else if(lpObj->Class == CLASS_MONK)
		{
		}

		fdamage = damage * iMultipleDmg / 10.0f;
		fdamage /= 10.0f;
		fdamage += 1.0f;

		sprite->m_DurabilitySmall += (int)fdamage;

		if(sprite->m_DurabilitySmall > iMaxDurSmall)
		{
			sprite->m_Durability -= 1.0f;
			sprite->m_DurabilitySmall = 0;

			if(sprite->m_Durability < 1.0f)
			{
				sprite->m_Durability = 0;
			}

			send_dur = 1;
		}
	}

	if ( send_dur != FALSE )
	{
		GCItemDurSend(lpObj->m_Index, 8, sprite->m_Durability, 0);

		if ( sprite->m_Durability < 1.0f )
		{
			int itemnum = sprite->m_Type;

			LogAddTD(lMsg.Get(MSGGET(2, 10)), lpObj->AccountID, lpObj->Name, lpObj->pInventory[8].GetName(), lpObj->pInventory[8].m_Level);
			gObjInventoryDeleteItem(lpObj->m_Index, 8);
			GCInventoryItemDeleteSend(lpObj->m_Index, 8, 0);
			gObjMakePreviewCharSet(lpObj->m_Index);
			GCEquipmentChange(lpObj->m_Index, 8);

			if ( BC_MAP_RANGE(lpObj->MapNumber) || CC_MAP_RANGE(lpObj->MapNumber) )
			{
				if ( itemnum == ITEMGET(13,0) || itemnum == ITEMGET(13,1) || 
					itemnum == ITEMGET(13,64) || itemnum == ITEMGET(13,65)|| itemnum == ITEMGET(13,80)  //Season 4.6 add-on
					|| itemnum == ITEMGET(13, 106) || itemnum == ITEMGET(13, 123) // Season 5 Episode 2 JPN
					) 
				{
					int pos = -1;

					for ( int i=MAX_PLAYER_EQUIPMENT;i<MAIN_INVENTORY_SIZE;i++)
					{
						if ( lpObj->pInventory[i].IsItem() == TRUE )
						{
							if ( lpObj->pInventory[i].m_Type == itemnum )
							{
								pos = i;
								break;
							}
						}
					}

					if ( pos != -1 )
					{
						BYTE ItemInfo[MAX_ITEM_INFO];
						BOOL DurSend;
						BOOL DurTargetSend;

						ItemByteConvert(ItemInfo, lpObj->pInventory[pos]);
						gObjInventoryMoveItem(lpObj->m_Index, pos, 8, DurSend, DurTargetSend, 0, 0, ItemInfo);
						GCItemMoveResultSend(lpObj->m_Index, 0, 8, ItemInfo);
						GCInventoryItemDeleteSend(lpObj->m_Index, pos, 0);
						GCServerMsgStringSend(lMsg.Get(MSGGET(4, 158)), lpObj->m_Index, 1);
					}
				}
			}
				
			if ( lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,3) || lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,37))
			{
				int pos = -1;

				for ( int i=MAX_PLAYER_EQUIPMENT;i<MAIN_INVENTORY_SIZE;i++)
				{
					if ( lpObj->pInventory[i].IsItem() == TRUE )
					{
						if ( lpObj->pInventory[i].m_Type == ITEMGET(13,3) )
						{
							pos = i;
							break;
						}
					}
				}

				if ( pos == -1 )
				{
					if ( lpObj->pInventory[7].IsItem() == FALSE )
					{
						gObjMoveGate(lpObj->m_Index, 22); //Devias or Lorencia??
					}
				}
				else
				{
					BYTE ItemInfo[MAX_ITEM_INFO];
					BOOL DurSend;
					BOOL DurTargetSend;

					ItemByteConvert(ItemInfo, lpObj->pInventory[pos]);
					gObjInventoryMoveItem(lpObj->m_Index, pos, 8, DurSend, DurTargetSend, 0, 0, ItemInfo);
					GCItemMoveResultSend(lpObj->m_Index, 0, 8, ItemInfo);
					GCInventoryItemDeleteSend(lpObj->m_Index, pos, 0);
					GCServerMsgStringSend(lMsg.Get(MSGGET(4, 158)), lpObj->m_Index, 1);
				}
			}
		}

	}
}

//0056da20	->
void gObjSecondDurDown(LPOBJ lpObj)
{
	if( lpObj->IsOffTrade )	//fix
	{
		return;
	}
	// ----
	lpObj->m_TimeCount	+= 1;
	// ----
#if defined __REEDLAN__ || __BEREZNUK__
	lpObj->m_ShopPointExTime++;
	g_ShopPointEx.AddTimeBonus(lpObj->m_Index);
#endif
	// ----
	int ret				= 0;
	int reCalCharacter	= 0;
	int	item_num[5];
	item_num[0]			= 2;
	item_num[1]			= 3;
	item_num[2]			= 4;
	item_num[3]			= 5;
	item_num[4]			= 6;
	// ----
	if( lpObj->m_Change >= 0 )
	{
		gObjChangeDurProc2(lpObj);
	}
	// ----
	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X,lpObj->Y);
	// ----
	for( int i = 0; i < 5; i++ )
	{
		CItem * DurItem	= &lpObj->pInventory[item_num[i]];
		ret				= 0;
		// ----
		if( g_LuckyItemManager.IsLuckyItemEquipment(DurItem->m_Type) )
		{
			ret = DurItem->LuckyItemArmorDurabilityDown(1, lpObj->m_Index);
			// ----
			if( ret != 0 )
			{
				GCItemDurSend2(lpObj->m_Index, (BYTE)item_num[i], (int)DurItem->m_Durability, 0);
			}
			// ----
			if( ret == 2 )
			{
				reCalCharacter = 1;
			}
		}
	}
	// ----
	if( lpObj->m_TimeCount % 10 != 0 )
	{
		return;
	}
	// ----
	if( lpObj->pInventory[7].IsItem() == 1 )
	{
		ret = lpObj->pInventory[7].DurabilityDown(1,lpObj->m_Index);
		// ----
		if( ret != 0 )
		{
			GCItemDurSend2(lpObj->m_Index,7,lpObj->pInventory[7].m_Durability,0);
		}
		// ----
		if( ret == 2 )
		{
			reCalCharacter = 1;
		}
	}
	// ----
	if( (attr&1) == 0 )
	{
		if( lpObj->pInventory[10].IsItem() == 1 )
		{
			if( lpObj->pInventory[10].m_Type == ITEMGET(13, 107) )
			{
				if( IsOKPCBangBenefitAll(lpObj) != FALSE )
				{
					ret = lpObj->pInventory[10].DurabilityDown(35, lpObj->m_Index);
				}
				else
				{
					ret = lpObj->pInventory[10].DurabilityDown(70, lpObj->m_Index);
				}
				// ----
				if( ret != 0 )
				{
					GCItemDurSend2(lpObj->m_Index, 10, lpObj->pInventory[10].m_Durability, 0);
				}
				// ----
				if( ret == 2 )
				{
					reCalCharacter = 1;
				}
				// ----
				if( lpObj->pInventory[10].m_Durability <= 0.0 )
				{
					gObjInventoryDeleteItem(lpObj->m_Index, 10);
					GCInventoryItemDeleteSend(lpObj->m_Index, 10, 0);
				}
			}
			else if( lpObj->pInventory[10].m_Type == ITEMGET(13, 20) && lpObj->pInventory[10].m_Level == 0 )
			{
				if(IsOKPCBangBenefitAll(lpObj) != FALSE)
				{
					ret = lpObj->pInventory[10].DurabilityDown(35,lpObj->m_Index);
				}
				else
				{
					ret = lpObj->pInventory[10].DurabilityDown(70,lpObj->m_Index);
				}

				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
			else if(lpObj->pInventory[10].m_Type == ITEMGET(13,38))
			{
				ret = lpObj->pInventory[10].DurabilityDown(63,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}
			}
			else if(lpObj->pInventory[10].m_Type != ITEMGET(13,10) &&
				//season4.5 add-on
				lpObj->pInventory[10].m_Type != ITEMGET(13,39) &&
				lpObj->pInventory[10].m_Type != ITEMGET(13,40) &&
				lpObj->pInventory[10].m_Type != ITEMGET(13,41) &&
				lpObj->pInventory[10].m_Type != ITEMGET(13,42))
			{
				ret = lpObj->pInventory[10].DurabilityDown(1, lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index, 10, lpObj->pInventory[10].m_Durability, 0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}

		if( lpObj->pInventory[11].IsItem() == 1 )
		{
			if( lpObj->pInventory[11].m_Type == ITEMGET(13, 107) )
			{
				if( IsOKPCBangBenefitAll(lpObj) != FALSE )
				{
					ret = lpObj->pInventory[11].DurabilityDown(35, lpObj->m_Index);
				}
				else
				{
					ret = lpObj->pInventory[11].DurabilityDown(70, lpObj->m_Index);
				}
				// ----
				if( ret != 0 )
				{
					GCItemDurSend2(lpObj->m_Index, 11, lpObj->pInventory[11].m_Durability, 0);
				}
				// ----
				if( ret == 2 )
				{
					reCalCharacter = 1;
				}
				// ----
				if( lpObj->pInventory[11].m_Durability <= 0.0 )
				{
					gObjInventoryDeleteItem(lpObj->m_Index, 11);
					GCInventoryItemDeleteSend(lpObj->m_Index, 11, 0);
				}
			}
			// ----
			if(lpObj->pInventory[11].m_Type == ITEMGET(13,20) && lpObj->pInventory[11].m_Level == 0)
			{
				if(IsOKPCBangBenefitAll(lpObj) != FALSE) //season4.5 add-on
				{
					ret = lpObj->pInventory[11].DurabilityDown(35,lpObj->m_Index);
				}
				else
				{
					ret = lpObj->pInventory[11].DurabilityDown(70,lpObj->m_Index);
				}
				
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,38))
			{
				ret = lpObj->pInventory[11].DurabilityDown(63,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}
			}
			else if(lpObj->pInventory[11].m_Type != ITEMGET(13,10) &&
				//season4.5 add-on
				lpObj->pInventory[11].m_Type != ITEMGET(13,39) &&
				lpObj->pInventory[11].m_Type != ITEMGET(13,40) &&
				lpObj->pInventory[11].m_Type != ITEMGET(13,41) &&
				lpObj->pInventory[11].m_Type != ITEMGET(13,42))
			{
				ret = lpObj->pInventory[11].DurabilityDown(1,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}

		if(lpObj->pInventory[9].IsItem() == 1)
		{
			if(lpObj->pInventory[9].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[9].DurabilityDown(1,lpObj->m_Index);

				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,9,lpObj->pInventory[9].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}
	}

	if(reCalCharacter != 0)
	{
		gObjCalCharacter(lpObj->m_Index);
	}
}

void gObjChangeDurProc(LPOBJ lpObj) //#unused in gs 90 now
{
	if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,10))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,10))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,39))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,39))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	//Second Edition
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,40))
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,40))
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}

	//Season 2.5 add-on
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,41)) //Season 2.5 Santa Girl Ring
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_Type == ITEMGET(13,41)) //Season 2.5 Santa Girl Ring
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,42)) //Season 2.5 GameMaster Ring
	{
		//wtf??
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,42)) //Season 4.5 add-on (fix) GameMaster Ring
	{
		//wtf??
	}
	else if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,68)) //Season 4.0 XMas Ring
	{
		int m_Durability = lpObj->pInventory[10].m_Durability;

		lpObj->pInventory[10].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
		{
			if(lpObj->pInventory[10].m_Durability < 1)
			{
				lpObj->pInventory[10].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[10].m_Durability;

			GCItemDurSend(lpObj->m_Index,10,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
				lpObj->pInventory[10].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
			}
		}
	}
	else if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[10].m_Type == ITEMGET(13,68)) //Season 4.0 add-on XMas Ring
	{
		int m_Durability = lpObj->pInventory[11].m_Durability;

		lpObj->pInventory[11].m_Durability -= (float)0.02;

		if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
		{
			if(lpObj->pInventory[11].m_Durability < 1)
			{
				lpObj->pInventory[11].m_Durability = 0;
			}

			BYTE dur = lpObj->pInventory[11].m_Durability;

			GCItemDurSend(lpObj->m_Index,11,dur,0);

			if(dur == 0)
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
				lpObj->pInventory[11].Clear();
				GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
			}
		}
	}
}

//00525F70
void gObjChangeDurProc2(LPOBJ lpObj) //season 4.5 add-on
{
	if(lpObj->pInventory[10].IsItem() == 1
#ifdef PERIOD
		&& !lpObj->pInventory[10].IsPeriodItem()
#endif
		)
	{
		if(lpObj->pInventory[10].m_Type == ITEMGET(13,10))
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,39))
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		//Second Edition
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,40))
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		//Season 2.5 add-on
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,41)) //Season 2.5 Santa Girl Ring
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,68)) //Season 4.0 XMas Ring
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,76)) //Season 4.6 Panda Ring
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
			else if(lpObj->pInventory[10].m_Type == ITEMGET(13, 122) ) // Season 5 Episode 2 JPN Skeleton Ring
			{
				int m_Durability = lpObj->pInventory[10].m_Durability;
				// ----
				lpObj->pInventory[10].m_Durability -= (float)0.02;
				// ----
				if( m_Durability > (int)lpObj->pInventory[10].m_Durability)
				{
					if(lpObj->pInventory[10].m_Durability < 1)
					{
						lpObj->pInventory[10].m_Durability = 0;
					}
					// ----
					BYTE dur = lpObj->pInventory[10].m_Durability;
					// ----
					GCItemDurSend(lpObj->m_Index, 10, dur, 0);
					// ----
					if(dur == 0)
					{
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
						lpObj->pInventory[10].Clear();
						GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
					}
				}
			}
		}
		if(lpObj->pInventory[11].IsItem() == 1	
#ifdef PERIOD
		&& !lpObj->pInventory[11].IsPeriodItem()
#endif
			)
		{
			if(lpObj->pInventory[11].m_Type == ITEMGET(13,10))
			{
				int m_Durability = lpObj->pInventory[11].m_Durability;

				lpObj->pInventory[11].m_Durability -= (float)0.02;

				if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
				{
					if(lpObj->pInventory[11].m_Durability < 1)
					{
						lpObj->pInventory[11].m_Durability = 0;
					}

					BYTE dur = lpObj->pInventory[11].m_Durability;

					GCItemDurSend(lpObj->m_Index,11,dur,0);

					if(dur == 0)
					{
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur); //wz bug
						lpObj->pInventory[11].Clear();
						GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
					}
				}
			}
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,39))
			{
				int m_Durability = lpObj->pInventory[11].m_Durability;

				lpObj->pInventory[11].m_Durability -= (float)0.02;

				if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
				{
					if(lpObj->pInventory[11].m_Durability < 1)
					{
						lpObj->pInventory[11].m_Durability = 0;
					}

					BYTE dur = lpObj->pInventory[11].m_Durability;

					GCItemDurSend(lpObj->m_Index,11,dur,0);

					if(dur == 0)
					{
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
						lpObj->pInventory[11].Clear();
						GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
					}
				}
			}
			//Second Edition
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,40))
			{
				int m_Durability = lpObj->pInventory[11].m_Durability;

				lpObj->pInventory[11].m_Durability -= (float)0.02;

				if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
				{
					if(lpObj->pInventory[11].m_Durability < 1)
					{
						lpObj->pInventory[11].m_Durability = 0;
					}

					BYTE dur = lpObj->pInventory[11].m_Durability;

					GCItemDurSend(lpObj->m_Index,11,dur,0);

					if(dur == 0)
					{
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
						lpObj->pInventory[11].Clear();
						GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
					}
				}
			}
			//Season 2.5 add-on
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,41)) //Season 2.5 Santa Girl Ring
			{
				int m_Durability = lpObj->pInventory[11].m_Durability;

				lpObj->pInventory[11].m_Durability -= (float)0.02;

				if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
				{
					if(lpObj->pInventory[11].m_Durability < 1)
					{
						lpObj->pInventory[11].m_Durability = 0;
					}

					BYTE dur = lpObj->pInventory[11].m_Durability;

					GCItemDurSend(lpObj->m_Index,11,dur,0);

					if(dur == 0)
					{
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
						lpObj->pInventory[11].Clear();
						GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
					}
				}
			}
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,68)) //Season 4.0 XMas Ring
			{
				int m_Durability = lpObj->pInventory[11].m_Durability;

				lpObj->pInventory[11].m_Durability -= (float)0.02;

				if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
				{
					if(lpObj->pInventory[11].m_Durability < 1)
					{
						lpObj->pInventory[11].m_Durability = 0;
					}

					BYTE dur = lpObj->pInventory[11].m_Durability;

					GCItemDurSend(lpObj->m_Index,11,dur,0);

					if(dur == 0)
					{
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
						lpObj->pInventory[11].Clear();
						GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
					}
				}
			}
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,76)) //Season 4.6 Panda Ring
			{
				int m_Durability = lpObj->pInventory[11].m_Durability;

				lpObj->pInventory[11].m_Durability -= (float)0.02;

				if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
				{
					if(lpObj->pInventory[11].m_Durability < 1)
					{
						lpObj->pInventory[11].m_Durability = 0;
					}

					BYTE dur = lpObj->pInventory[11].m_Durability;

					GCItemDurSend(lpObj->m_Index,11,dur,0);

					if(dur == 0)
					{
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
						lpObj->pInventory[11].Clear();
						GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
					}
				}
			}
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13, 122)) // Season 5 Episode 2 JPN Skeleton Ring
			{
				int m_Durability = lpObj->pInventory[11].m_Durability;
				// ----
				lpObj->pInventory[11].m_Durability -= (float)0.02;
				// ----
				if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
				{
					if(lpObj->pInventory[11].m_Durability < 1)
					{
						lpObj->pInventory[11].m_Durability = 0;
					}
					// ----
					BYTE dur = lpObj->pInventory[11].m_Durability;
					// ----
					GCItemDurSend(lpObj->m_Index, 11, dur, 0);
					// ----
					if(dur == 0)
					{
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						LogAddTD(lMsg.Get(544),lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
						lpObj->pInventory[11].Clear();
						GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
					}
				}
			}
	}
}
}

void gObjWingDurProc(LPOBJ lpObj) 
{
	BYTE send_dur=0;

	// ???? ???????­?? ?»±?·? ?? ??°?..
	return;

	if( lpObj->pInventory[7].IsItem() == TRUE )
	{	
		int dur = (int)lpObj->pInventory[7].m_Durability;
			lpObj->pInventory[7].m_Durability -= (float)0.0002;
			
			if( dur > (int)lpObj->pInventory[7].m_Durability ) 
			{
				if( lpObj->pInventory[7].m_Durability < 1 ) 
					lpObj->pInventory[7].m_Durability = 0;

				send_dur = (BYTE)lpObj->pInventory[7].m_Durability;
				GCItemDurSend(lpObj->m_Index, 7, send_dur, 0);
				if( send_dur == 0 ) 
				{
					LogAddTD(lMsg.Get(544), lpObj->AccountID, lpObj->Name, lpObj->pInventory[7].GetName(), lpObj->pInventory[7].m_Level, send_dur);
					lpObj->pInventory[7].Clear();
					GCInventoryItemDeleteSend(lpObj->m_Index, 7, 0);
				}
			}
			//LogAdd("??°? ?»±?·? %f",lpObj->pInventory[7].m_Durability);
			return;
	}
}

void gObjPenaltyDurDown(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int ret = 0;
	int reCalCharacter = 0;

	int irand = rand()%2;

	CItem * Weapon = &lpObj->pInventory[irand];
	int decdur = 1;

	switch(lpObj->Class)
	{
	case CLASS_WIZARD:
		decdur = lpObj->Level - (lpTargetObj->Level * 38) / 20;
		break;
	case CLASS_KNIGHT:
		decdur = lpObj->Level - (lpTargetObj->Level * 37) / 20;
		break;
	case CLASS_ELF:
		decdur = lpObj->Level - (lpTargetObj->Level * 36) / 20;
		break;
	case CLASS_MAGUMSA:
	case CLASS_DARKLORD:
#ifdef MONK
	case CLASS_MONK:
#endif
		decdur = lpObj->Level - (lpTargetObj->Level * 37) / 20;
		break;
	case CLASS_SUMMONER: //Season 3 add-on
		decdur = lpObj->Level - (lpTargetObj->Level * 38) / 20;
		break;
	default : break;
	}

	decdur /= 10;

	if(Weapon->m_Type != ITEMGET(4,7) && Weapon->m_Type != ITEMGET(4,15))
	{
		if(Weapon->m_Type < ITEMGET(6,0) && Weapon->m_Type >= ITEMGET(7,0) )
		{
			if ( Weapon->IsItem() )
			{
				ret = Weapon->DurabilityDown2(decdur,lpObj->m_Index);
			}
		}
		else
		{
			CItem * Weapon = &lpObj->pInventory[irand];

			if(Weapon->IsItem())
			{
				ret = Weapon->DurabilityDown2(decdur,lpObj->m_Index);
			}
		}
	}

	if(ret != 0)
	{
		GCItemDurSend2(lpObj->m_Index,irand,lpObj->pInventory[irand].m_Durability,0);
	}

	if(ret == 2)
	{
		reCalCharacter = 1;
	}

	int item_num[7] = {0,1,2,3,4,5,6};
	int item_pos = rand()%7;

	CItem * DurItem = &lpObj->pInventory[item_num[item_pos]];

	if(DurItem->m_Type != ITEMGET(4,7) && DurItem->m_Type != ITEMGET(4,15))
	{
		if(item_pos < 2)
		{
			if(DurItem->m_Type >= ITEMGET(6,0) && DurItem->m_Type < ITEMGET(7,0))
			{
				ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);
	
				if(ret != 0)
				{
					GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
	
					if(ret == 2)
					{
						reCalCharacter = 1;
					}
				}
			}
		}
		else
		{
			ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}
	}

	CItem * Wing = &lpObj->pInventory[7];

	if(Wing->IsItem())
	{
		ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);

		if(ret != 0)
		{
			GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);

			if(ret == 2)
			{
				reCalCharacter = 1;
			}
		}
	}

	if(reCalCharacter != 0)
	{
		gObjCalCharacter(lpObj->m_Index);
	}
}

void gObjWeaponDurDown(LPOBJ lpObj, LPOBJ lpTargetObj, int type)
{
	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		return;
	}

	int itargetdefence = 0;
	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];
	int ret = 0;

	if(gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_PROTECT_AMULET) != 0) //season4.5 add-on
	{
		return;
	}

	if(type == 0)
	{
		itargetdefence = lpTargetObj->m_Defense;

		if((lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD
#ifdef MONK
			|| lpObj->Class == CLASS_MONK) 
#else
			)
#endif
			&& (Right->m_Type >= 0 && Right->m_Type < ITEMGET(4,0) && Left->m_Type >= 0 && Left->m_Type < ITEMGET(4,0)))
		{
			if(rand()%2==0)
			{
				ret = Left->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
				
				if(ret != 0)
				{
					GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);
					if(ret == 2)
					{
						gObjCalCharacter(lpObj->m_Index);
					}
				}
			}
			else
			{
				ret = Right->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
				if(ret != 0)
				{
					GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
					if(ret==2)
					{
						gObjCalCharacter(lpObj->m_Index);
					}
				}
			}
			return;
		}
		else if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7) || Left->m_Type == ITEMGET(4,20) || Left->m_Type == ITEMGET(4,21) ||
			Left->m_Type == ITEMGET(4,22) || Left->m_Type == ITEMGET(4,23) || Left->m_Type == ITEMGET(4,24)) //season4.6 add-on
		{
			ret = Left->BowWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);
				if(ret==2)
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		else if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) || Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0))
		{
			ret = Right->BowWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				if(ret==2)
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		else if(Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0))
		{
			ret = Right->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				if(ret==2)
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		else if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
		{
			ret = Right->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				if(ret==2)
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
		return;
	}

	if(lpTargetObj->Type == OBJ_MONSTER || lpTargetObj->Type == OBJ_NPC)
	{
		itargetdefence = lpTargetObj->m_MagicDefense;
	}
	else
	{
		itargetdefence = lpTargetObj->m_Defense;
	}

	if(lpObj->Class == CLASS_SUMMONER) //Season3 add-on
	{
		if(Right->m_Type >= 0 && Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0) && Left->m_Type >= 0 && Left->m_Type >= ITEMGET(5,0) && Left->m_Type < ITEMGET(6,0))
		{
			if(rand()%2==0)
			{
				ret = Right->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
				
				if(ret != 0)
				{
					GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);

					if(ret == 2)
					{
						gObjCalCharacter(lpObj->m_Index);
					}
				}
			}
			else
			{
				ret = Left->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

				if(ret != 0)
				{
					GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);

					if(ret==2)
					{
						gObjCalCharacter(lpObj->m_Index);
					}
				}
			}
			return;
		}
		
		if(Right->m_Type >= 0 && Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
		{
			GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);

			if(ret==2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
			return;
		}
		
		if(Left->m_Type >= 0 && Left->m_Type >= ITEMGET(5,0) && Left->m_Type < ITEMGET(6,0))
		{
			ret = Left->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);

				if(ret==2)
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
			return;
		}
	}
	
	if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
	{
		ret = Right->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

		if(ret != 0)
		{
			GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);

			if(ret==2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
	}
}

void gObjArmorRandomDurDown(LPOBJ lpObj, LPOBJ lpAttackObj)
{
	int item_num[7] = {0,1,2,3,4,5,6};
	int item_pos = rand()%7;

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	CItem * DurItem = &lpObj->pInventory[item_num[item_pos]];
	int damagemin = lpAttackObj->m_AttackDamageMin;
	int ret;
	short ItemType = DurItem->m_Type;

	if(gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_PROTECT_AMULET) != 0) //season4.5 add-on
	{
		if(ItemType >= ITEMGET(0,0) && ItemType < ITEMGET(12,0))
		{
			return;
		}

		if( (ItemType >= ITEMGET(12,0) && ItemType < ITEMGET(13,0)) && 
			(((ItemType%512 >= 0) && (ItemType%512 <= 16)) ||
			((ItemType%512 >= 36) && (ItemType%512 <= 43))))
		{
			return;
		}
	}

	if(item_pos < 2)
	{
		if(DurItem->m_Type >= ITEMGET(6,0) && DurItem->m_Type < ITEMGET(7,0))
		{
			ret = DurItem->ArmorDurabilityDown(damagemin,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
				if(ret == 2)
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
			return;
		}
	}
	else
	{
		if(DurItem->m_Type != ITEMGET(4,7) && DurItem->m_Type != ITEMGET(4,15))
		{
			ret = DurItem->ArmorDurabilityDown(damagemin,lpObj->m_Index);

			if(ret != 0)
			{
				GCItemDurSend(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
				if(ret == 2)
				{
					gObjCalCharacter(lpObj->m_Index);
				}
			}
		}
	}
}

bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	if ( !OBJMAX_RANGE(aTargetIndex) )
	{
		return false;
	}

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 )
		{
			if ( lpObj->SelfDefense[n] == aTargetIndex )
			{
				return true;
			}
		}
	}

	return false;
}

void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	int n;

	if(!OBJMAX_RANGE(aTargetIndex))
	{
		return;
	}

	if(gObj[aTargetIndex].m_PK_Level > 4)
	{
		return;
	}

#ifdef NPVP
	if( g_NewPVP.IsDuel(*lpObj, aTargetIndex) )
	{
		return;
	}
#endif

	int iPartyNumber = lpObj->PartyNumber;
	int loc3 = -1;

	for(n = 0; n < MAX_SELF_DEFENSE;n++)
	{
		if(gObj[aTargetIndex].SelfDefense[n] >= 0)
		{
			if(gObj[aTargetIndex].SelfDefense[n]==lpObj->m_Index)
			{
				return;
			}

			if(iPartyNumber >= 0)
			{
				if(gObj[gObj[aTargetIndex].SelfDefense[n]].PartyNumber == iPartyNumber)
				{
					return;
				}
			}
		}
	}

	int blank = -1;

	for(n = 0; n < MAX_SELF_DEFENSE;n++)
	{
		if(lpObj->SelfDefense[n] >= 0)
		{
			if(lpObj->SelfDefense[n] == aTargetIndex)
			{
				lpObj->SelfDefenseTime[n] = GetTickCount()+60000; //Season 3.5 changed
				return;
			}
		}
		else
		{
			blank = n;
		}
	}

	if(blank < 0)
	{
		LogAdd(lMsg.Get(523));
		return;
	}

	lpObj->MySelfDefenseTime = GetTickCount();
	lpObj->SelfDefense[blank] = aTargetIndex;
	lpObj->SelfDefenseTime[blank] = GetTickCount() + 60000; //Season 3.5 changed

	char szTemp[64];

	wsprintf(szTemp,lMsg.Get(1114),lpObj->Name,gObj[aTargetIndex].Name);

	iPartyNumber = gObj[aTargetIndex].PartyNumber;

	if(iPartyNumber >= 0)
	{
		int iPartyNumIndex = -1;

		for(n = 0; n < MAX_USER_IN_PARTY; n++)
		{
			iPartyNumIndex = gParty.m_PartyS[iPartyNumber].Number[n];

			if( iPartyNumIndex >= 0  )
			{
				GCServerMsgStringSend(szTemp,iPartyNumIndex,1);
			}
		}
	}
	else
	{
		GCServerMsgStringSend(szTemp,aTargetIndex,1);
	}

	GCServerMsgStringSend(szTemp,lpObj->m_Index,1);

	LogAddTD("[%s][%s] Set SelfDefence [%s][%s]",lpObj->AccountID,lpObj->Name,gObj[aTargetIndex].AccountID,gObj[aTargetIndex].Name);
}

void gObjTimeCheckSelfDefense(LPOBJ lpObj)
{
	char szTemp[64];

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 )
		{
			if ( GetTickCount() > lpObj->SelfDefenseTime[n] )
			{
				wsprintf(szTemp, lMsg.Get(MSGGET(4, 91)), lpObj->Name);
				GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				GCServerMsgStringSend(szTemp, lpObj->SelfDefense[n], 1);
				LogAddTD("[%s][%s] ReSet SelfDefence [%s][%s]", lpObj->AccountID, lpObj->Name, gObj[lpObj->SelfDefense[n]].AccountID, gObj[lpObj->SelfDefense[n]].Name);
				lpObj->SelfDefense[n] = -1; //season4.5 position changed
			}
		}
	}
}

BOOL gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, BOOL magicsend, BYTE MSBFlag, int AttackDamage, BOOL bCombo, BYTE byBarrageCount, BYTE byReflect)
{
	return gclassObjAttack.Attack(lpObj, lpTargetObj, lpMagic, magicsend, MSBFlag, AttackDamage, bCombo, byBarrageCount, byReflect);
}

void gObjMonsterExpDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag) //005287E0
{
	gObjMonsterHitDamageUserDel(lpMonObj);

	lpMonObj->Money = 0;

	int tObjNum;

	__int64 exp;

	LPOBJ lpTargetObj;

	for(int n = 0; n < MAX_ST_HIT_DAMAGE;n++)
	{
		if(lpMonObj->sHD[n].number >= 0)
		{
			tObjNum = lpMonObj->sHD[n].number;
			lpTargetObj = &gObj[tObjNum];

			if(lpMonObj->sHD[n].HitDamage > 0)
			{
				bool bSendExp = 1;

				exp = gObjMonsterExpSingle(lpTargetObj,lpMonObj,lpMonObj->sHD[n].HitDamage,lpMonObj->MaxLife,bSendExp);

				if(bSendExp == 1)
				{
					if(lpTargetObj->m_Index == lpObj->m_Index)
					{
						GCKillPlayerExpSend(lpTargetObj->m_Index,lpMonObj->m_Index,exp,AttackDamage,MSBFlag);
						
						if(lpObj->Level <= 150)
						{
							if(IsOKPCBangBenefitAll(lpObj) != FALSE)
							{
								lpMonObj->Money += exp;
							}
						}
					}
					else
					{
						GCKillPlayerExpSend(lpTargetObj->m_Index,lpMonObj->m_Index,exp,0,MSBFlag);
					}
				}
				lpMonObj->Money += exp;
			}
		}
	}	
}

int gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, bool& bSendExp) //005289E0
{
	if(g_MasterLevelSystem.CheckMLGetExp(lpObj, lpTargetObj) == 0) //Season3 add-on
	{
		bSendExp = 0;
		return 0;
	}

	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		return g_DevilSquare.gObjMonsterExpSingle(lpObj,lpTargetObj,dmg,tot_dmg);
	}

	__int64 exp;
	__int64 maxexp = 0;

	int level = (lpTargetObj->Level+25)*lpTargetObj->Level/3;

	if((lpTargetObj->Level+10) < lpObj->Level+lpObj->m_nMasterLevel)
	{
		level = level*(lpTargetObj->Level+10)/(lpObj->Level+lpObj->m_nMasterLevel);
	}

	if(lpTargetObj->Level >= 65)
	{
		level = level + (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
	}

	if(level > 0)
	{
		maxexp = level/2;
	}
	else
	{
		level = 0;
	}

	if(maxexp < 1)
	{
		exp = level;
	}
	else
	{
		exp = level + rand()%maxexp;
	}

	exp = dmg * exp / tot_dmg;

	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		exp = exp * 50 / 100;
	}

	DWORD mymaxexp = 0; //loc6

	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) != FALSE) //Season3 add-on
	{
		mymaxexp = lpObj->m_i64NextMasterLevelExp;
	}
	else
	{
		mymaxexp = gLevelExperience[lpObj->Level];
	}

	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) == FALSE)
	{
#ifdef __CUSTOMS__
		exp = int(exp * g_ResetSystem.GetDynamicExp(lpObj));
#else
		exp = int(exp * gAddExperience);
#endif
	}

#ifdef PCBANG
	if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) == FALSE)//Season 4.5 addon
	{
		g_PCBangPointSystem.AddExperience(lpObj,exp);
	}
#endif

	if ( g_CrywolfSync.GetOccupationState() == 1 && g_iCrywolfApplyMvpPenalty != FALSE)
	{
		exp = (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
	}

	exp +=  (exp * g_MAP_SETTINGS[lpObj->MapNumber].exp_increase) / 100;

	if(exp > 0)
	{
		if(lpObj->Type == OBJ_USER)
		{
			CheckItemOptForGetExpEx(lpObj, exp, FALSE); //Seal Exp (Season3 add-on)

			lpObj->Experience += exp;

			if(gObjLevelUp(lpObj,exp,lpTargetObj->Class,0) == 0)
			{
				bSendExp = 0;
			}
		}
	}

	return exp;
}

void gObjExpParty(LPOBJ lpObj , LPOBJ lpTargetObj, int AttackDamage, int MSBFlag )
{
	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		g_DevilSquare.gObjExpParty(lpObj,lpTargetObj,AttackDamage,MSBFlag);
		return;
	}

	int n;
	__int64 exp;
	__int64 maxexp = 0;
	__int64 totalexp;
	int level = (lpTargetObj->Level+25)*lpTargetObj->Level / 3;
	int number;
	int partynum = 0;
	int totallevel = 0;
	int partylevel;
	int partycount;
	int dis[5];
	int viewplayer = 0;
	int viewpercent = 100;
	int bApplaySetParty = 0;
	BYTE bCheckSetParty[MAX_TYPE_PLAYER];
	LPOBJ lpPartyObj;

	partynum = lpObj->PartyNumber;

	int toplevel = 0;

	for(n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];
			
			int dis = gObjCalDistance(lpTargetObj,lpPartyObj); //Season 2.5 add-on

			if(dis < 10) //Season 2.5 add-on
			{
				if(lpPartyObj->Level > toplevel)
				{
					toplevel = lpPartyObj->Level;
				}
			}

			if(dis < 10) //Season 3.0 add-on
			{
				if(lpPartyObj->Level+lpPartyObj->m_nMasterLevel > toplevel)
				{
					toplevel = lpPartyObj->Level+lpPartyObj->m_nMasterLevel;
				}
			}
		}
	}

	if(!OBJMAX_RANGE(partynum))
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return;
	}

	memset(bCheckSetParty,0,sizeof(bCheckSetParty));

	partycount = gParty.m_PartyS[partynum].Count;

	for(n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpTargetObj->MapNumber == lpPartyObj->MapNumber)
			{
				dis[n] = gObjCalDistance(lpTargetObj,&gObj[number]);

				if(dis[n] < 10)
				{
					lpPartyObj = &gObj[number];

					if(toplevel >= (lpPartyObj->Level+lpPartyObj->m_nMasterLevel+200))
					{
						totallevel += lpPartyObj->Level+lpPartyObj->m_nMasterLevel+200;
					}
					else
					{
						totallevel += lpPartyObj->Level+lpPartyObj->m_nMasterLevel;
					}
					viewplayer += 1;
					bCheckSetParty[lpPartyObj->Class] = 1;
				}
			}
		}
	}

	if(bCheckSetParty[0] != false && bCheckSetParty[1] != false && bCheckSetParty[2] != false
		|| bCheckSetParty[3] != false && bCheckSetParty[4] != false && bCheckSetParty[5] != false )//Season 4.5 addon)
	{
		bApplaySetParty = 1;
	}

	if(viewplayer > 1)
	{
		if(bApplaySetParty != 0)
		{
			if(viewplayer == 3)
			{
				viewpercent = gParty3ExpPercent;
			}
			else if(viewplayer == 4)
			{
				viewpercent = gParty4ExpPercent;
			}
			else if(viewplayer >= 5)
			{
				viewpercent = gParty5ExpPercent;
			}
			else
			{
				viewpercent = gPartyExpPercentOther;
			}
		}
		else
		{
			if(viewplayer == 2)
			{
				viewpercent = gParty2ExpSetPercent;
			}
			else if(viewplayer == 3)
			{
				viewpercent = gParty3ExpSetPercent;
			}
			else if(viewplayer == 4)
			{
				viewpercent = gParty4ExpSetPercent;
			}
			else if(viewplayer >= 5)
			{
				viewpercent = gParty5ExpSetPercent;
			}
			else
			{
				viewpercent = gParty1ExpSetPercent;
			}
		}

		partylevel = totallevel / viewplayer;
	}
	else
	{
		partylevel = totallevel;
	}

	if((lpTargetObj->Level+10) < partylevel)
	{
		level = level * (lpTargetObj->Level+10) / partylevel;
	}

	if(lpTargetObj->Level >= 65)
	{
		if(viewplayer == 1)
		{
			level = level + (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
		}
		else
		{
			level += (200.0 - ((lpObj->Level + lpObj->m_nMasterLevel) * 0.2));
		}
	}

	if(level > 0)
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if(maxexp < 1)
	{
		totalexp = level;
	}
	else
	{
		totalexp = level + rand()%maxexp;
	}

	if(lpTargetObj->Type == OBJ_MONSTER)
	{
#ifdef __CUSTOMS__
		lpTargetObj->Money = totalexp * g_ResetSystem.GetDynamicExp(lpObj);
#else
		lpTargetObj->Money = totalexp;
#endif
	}

	for(n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if(number >= 0)
		{
			lpPartyObj = &gObj[number];

			if(lpTargetObj->MapNumber == lpPartyObj->MapNumber && dis[n] < 10)
			{
				__int64 mymaxexp = 0;

				if(g_MasterLevelSystem.CheckMLGetExp(lpPartyObj, lpTargetObj) == 0) //Season3 add-on
				{
					exp = 0;
				}
				else
				{
					if(g_MasterLevelSystem.IsMasterLevelUser(lpPartyObj) != FALSE)
					{
						mymaxexp = lpPartyObj->m_i64NextMasterLevelExp;
					}
					else
					{
						mymaxexp = gLevelExperience[lpPartyObj->Level];
					}

					exp = (totalexp * viewpercent * (lpPartyObj->Level+lpPartyObj->m_nMasterLevel))/totallevel / 100;
				}				

				if(BC_MAP_RANGE(lpPartyObj->MapNumber))
				{
					exp = exp * 50 / 100;
				}

				if(exp > mymaxexp)
				{
					exp = mymaxexp;
				}

				if(lpPartyObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
				{
					exp = 0;
				}

				if(g_MasterLevelSystem.IsMasterLevelUser(lpPartyObj) == FALSE)
				{
#ifdef __CUSTOMS__
					exp = int(exp * g_ResetSystem.GetDynamicExp(lpPartyObj));
#else
					exp = int(exp * gAddExperience);
#endif
				}

#ifdef PCBANG
				if(g_MasterLevelSystem.IsMasterLevelUser(lpPartyObj) == FALSE)//Season 4.5 addon
				{
					g_PCBangPointSystem.AddExperience(lpPartyObj,exp);
				}
#endif
				if ( g_CrywolfSync.GetOccupationState() == 1 && g_iCrywolfApplyMvpPenalty != FALSE)
				{
					exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
				}
				
				exp +=  (exp * g_MAP_SETTINGS[lpObj->MapNumber].exp_party_increase) / 100;

				if(exp > 0)
				{
					if(lpPartyObj->Type == OBJ_USER)
					{
						CheckItemOptForGetExpEx(lpPartyObj, exp, FALSE); //Seal Exp (Season3 add-on)

						lpPartyObj->Experience += exp;

						if(gObjLevelUp(lpPartyObj,exp,lpTargetObj->Class,1) == 0)
						{
							continue;
						}
					}
				}

				if(lpPartyObj->Type == OBJ_USER && g_MasterLevelSystem.IsMasterLevelUser(lpPartyObj) == FALSE)
				{
					GCKillPlayerExpSend(lpPartyObj->m_Index,lpTargetObj->m_Index,exp,AttackDamage,MSBFlag);
				}
			}
		}
	}
}

void gObjMonsterDieLifePlus(LPOBJ lpObj, LPOBJ lpTartObj)
{
	if(lpTartObj->Level < 0 || lpTartObj->Level > 255)
	{
		return;
	}

	lpObj->Life += lpTartObj->Level;

	if(lpObj->MonsterDieGetLife != 0)
	{
		lpObj->Life = lpObj->Life + ((lpObj->MaxLife + lpObj->AddLife)/8.0f) * lpObj->MonsterDieGetLife;
	}

	if(lpObj->MonsterDieGetMana != 0)
	{
		lpObj->Mana = lpObj->Mana + ((lpObj->MaxMana + lpObj->AddMana)/8.0f) * lpObj->MonsterDieGetMana;
	}

	if(lpObj->m_wSocketOptionMonsterDieGetHp > 0) //season4 add-on
	{
		lpObj->Life = lpObj->Life + lpObj->m_wSocketOptionMonsterDieGetHp;
	}

	if(lpObj->m_wSocketOptionMonsterDieGetMana > 0) //season4 add-on
	{
		lpObj->Mana = lpObj->Mana + lpObj->m_wSocketOptionMonsterDieGetMana;
	}

	if(lpObj->m_MPSkillOpt.MpsMonsterDieGetMana > 0)
	{
		lpObj->Mana = lpObj->Mana + (lpObj->MaxMana + lpObj->AddMana)/lpObj->m_MPSkillOpt.MpsMonsterDieGetMana;
	}

	if(lpObj->m_MPSkillOpt.MpsMonsterDieGetLife > 0)
	{
		lpObj->Life = lpObj->Life + (lpObj->MaxLife + lpObj->AddLife)/lpObj->m_MPSkillOpt.MpsMonsterDieGetLife;
	}

	if(lpObj->m_MPSkillOpt.MpsMonsterDieGetSD > 0)
	{
		lpObj->iShield = (float)lpObj->iShield + (lpObj->iMaxShield + lpObj->iAddShield)/lpObj->m_MPSkillOpt.MpsMonsterDieGetSD;
	}

	if( lpObj->m_MPSkillOpt.MpsMonsterDieGetSD > 0 )
	{
		if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
		{
			lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
		}
	}

	if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
	{
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	}

	GCReFillSend(lpObj->m_Index,lpObj->Life,-1,0,lpObj->iShield);

	if(lpObj->MonsterDieGetMana != 0
		|| lpObj->m_MPSkillOpt.MpsMonsterDieGetMana > 0
		|| lpObj->m_wSocketOptionMonsterDieGetMana > 0)
	{
		if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
		{
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		}

		GCManaSend(lpObj->m_Index,lpObj->Mana,-1,0,lpObj->BP);
	}
}

//00529D30
void gObjLifeCheck(LPOBJ lpTargetObj, LPOBJ lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage)
{
	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	if(ATTRIBUTE_RANGE(lpObj->m_Attribute) || ATTRIBUTE_RANGE(lpTargetObj->m_Attribute))
	{
		return;
	}

	LPOBJ lpCallObj = lpObj;

	if(lpObj->Type == OBJ_MONSTER && lpObj->m_RecallMon >= 0)
	{
		lpCallObj = &gObj[lpObj->m_RecallMon];
	}

	if(AttackDamage != 0 && lpTargetObj->Type == OBJ_MONSTER)
	{
		if(lpTargetObj->m_RecallMon >= 0)
		{
			GCRecallMonLife(lpTargetObj->m_RecallMon,lpTargetObj->MaxLife,lpTargetObj->Life);
		}

		gObjMonsterSetHitDamage(lpTargetObj,lpCallObj->m_Index,AttackDamage);
	}

	if(lpTargetObj->Life <= 0 && lpTargetObj->Live != 0)
	{
		if(lpObj->Type == OBJ_MONSTER)
		{
			gObjAddMsgSend(lpObj,3,lpTargetObj->m_Index,0);
			CreateFrustrum(lpObj->X,lpObj->Y,lpObj->m_Index);
			lpTargetObj->KillerType = 1;
		}

		if(lpObj->Type == OBJ_USER)
		{
			gDarkSpirit[lpObj->m_Index].ReSetTarget(lpTargetObj->m_Index);
		}

		if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
		{
			gObjSetKillCount(lpObj->m_Index,1);

			if(IT_MAP_RANGE(lpObj->MapNumber))
			{
				if(g_IllusionTempleEvent.GetIllusionTempleState(lpObj->MapNumber) == 2)
				{
					BYTE loc2 = g_IllusionTempleEvent.IncSkillPoint(lpObj->m_Index, lpObj->MapNumber, 1);
					GCSendIllusionTempleKillCount(lpObj->m_Index, loc2);
					LogAddTD("[Illusion Temple] (%d) (%s)(%s) Character Killed, (point: %d)", lpObj->MapNumber - 44, lpObj->AccountID, lpObj->Name, loc2);
				}
			}
		}

		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			if(IT_MAP_RANGE(lpObj->MapNumber))
			{
				if(g_IllusionTempleEvent.GetIllusionTempleState(lpObj->MapNumber) == 2)
				{
					BYTE loc3 = g_IllusionTempleEvent.IncSkillPoint(lpObj->m_Index, lpObj->MapNumber, 2);
					GCSendIllusionTempleKillCount(lpObj->m_Index, loc3);
					lpTargetObj->m_PosNum = -1;
					LogAddTD("[Illusion Temple] (%d) (%s)(%s) Monster Killed, (point: %d)", lpObj->MapNumber - 44, lpObj->AccountID, lpObj->Name, loc3);
				}
			}
//#if (_GSCS==1)
			if(lpTargetObj->Class == 275 || lpTargetObj->Class == 295 || lpTargetObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
			{
				gObjAddMsgSendDelayInSpecificQPos(lpTargetObj,1,lpObj->m_Index,800,0,0);
			}
			else
			{
				gObjAddMsgSendDelay(lpTargetObj,1,lpObj->m_Index,800,0);
			}
			
			g_Crywolf.CrywolfMonsterDieProc(lpTargetObj->m_Index,lpObj->m_Index);

#ifdef __NOVUS__
			if( lpTargetObj->Class == 295 ) // -> Erohim message for die
			{
				char Text[256];
				ZeroMemory(Text, sizeof(Text));
				sprintf(Text, "Erohim has been killed by %s", lpObj->Name);
				AllSendServerMsg(Text);
			}
#endif

#ifdef __CUSTOMS__
			if( lpTargetObj->Class == 561 ) // -> Medusa
			{
				char Text[256];
				ZeroMemory(Text, sizeof(Text));
				sprintf(Text, "Medusa has been killed by %s", lpObj->Name);
				AllSendServerMsg(Text);
			}
#endif

			if(lpTargetObj->m_Attribute == 62) // -> Monster die message
			{
				
				char szMsg[256];
				
				switch(lpTargetObj->Class)
				{
					case 300:// Hero Mobs
					case 301:
					case 302:
					case 303:
					{
						PMSG_NOTICE pNotice;
						
						ItemSerialCreateSend(lpObj->m_Index,lpObj->MapNumber,lpObj->X,lpObj->Y,ItemGetNumberMake(14,13),0,0,0,0,0,lpObj->m_Index,0,0);
						
						if(gObjIsConnected(lpObj->m_Index))
						{
							wsprintf(szMsg,lMsg.Get(MSGGET(12, 155)),lpTargetObj->Name,lpObj->Name);
							LogAddTD("[CastleDeep Event] [%s][%s] Boss Monster Killed : %s",lpObj->AccountID,lpObj->Name,lpTargetObj->Name);
						}
						else
						{
							wsprintf(szMsg,lMsg.Get(MSGGET(12, 156)),lpTargetObj->Name);
							LogAddTD("[CastleDeep Event] Boss Monster Killed : %s",lpTargetObj->Name);
						}
						
						TNotice::MakeNoticeMsg(&pNotice,0,szMsg);
						
						for(int i = OBJ_STARTUSERINDEX; i < OBJMAX;i++)
						{
							if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
							{
								DataSend(i,(LPBYTE)&pNotice,pNotice.h.size);
							}
						}
					}
					break;
					case 295: // Erohim
					{
						PMSG_NOTICE pNotice;
						
						if(gObjIsConnected(lpObj->m_Index))
						{
							wsprintf(szMsg,lMsg.Get(MSGGET(12, 157)),lpObj->Name);
							LogAddTD("[CastleDeep Event] [%s][%s] Boss Monster Killed : %s",lpObj->AccountID,lpObj->Name,lpTargetObj->Name);
						}
						else
						{
							wsprintf(szMsg,lMsg.Get(MSGGET(12, 158)));
							LogAddTD("[CastleDeep Event] Boss Monster Killed : %s",lpTargetObj->Name);
						}
						
						TNotice::MakeNoticeMsg(&pNotice,0,szMsg);
						
						for(int i = OBJ_STARTUSERINDEX; i < OBJMAX;i++)
						{
							if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
							{
								DataSend(i,(LPBYTE)&pNotice,pNotice.h.size);
							}
						}
					}
					break;
					}
			}
//#else
			g_Kanturu.KanturuMonsterDieProc(lpTargetObj->m_Index,lpObj->m_Index);
//#endif

			if(BC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				int iTargetBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(lpTargetObj->MapNumber);

				if(lpTargetObj->Class == 131)
				{
					int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

					if(TopHitUser != -1)
					{
						char szMsg[256];

						wsprintf(szMsg,lMsg.Get(1178),gObj[TopHitUser].Name);

						g_BloodCastle.SendNoticeMessage(iTargetBridgeIndex,szMsg);

						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Door_Party = gObj[TopHitUser].PartyNumber;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Door_Index = TopHitUser;

						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName,gObj[TopHitUser].Name,10);
						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID,gObj[TopHitUser].AccountID,10);

						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_CharName[10] = 0;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Door_AccountID[10] = 0;

						LogAddTD("[Blood Castle] (%d) Door Terminated -> %d [%s][%s]",iTargetBridgeIndex+1,g_BloodCastle.m_BridgeData[iTargetBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT,gObj[TopHitUser].AccountID,gObj[TopHitUser].Name);
					}
					else
					{
						g_BloodCastle.SendNoticeMessage(iTargetBridgeIndex,lMsg.Get(1169));
						LogAddTD("[Blood Castle] (%d) Door Terminated -> %d)",iTargetBridgeIndex+1,g_BloodCastle.m_BridgeData[iTargetBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
					}
				}


				if(BC_STATUE_RANGE(lpTargetObj->Class - 132))
				{
					int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(lpObj->MapNumber);

					int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

					if(TopHitUser != -1)
					{
						char szMsg[256];

						wsprintf(szMsg,lMsg.Get(1179),gObj[TopHitUser].Name);

						g_BloodCastle.SendNoticeMessage(iBridgeIndex,szMsg);


						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Statue_Party = gObj[TopHitUser].PartyNumber;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Statue_Index = gObj[TopHitUser].m_Index;

						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName,gObj[TopHitUser].Name,10);
						memcpy(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_AccountID,gObj[TopHitUser].AccountID,10);

						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_CharName[10] = 0;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_szKill_Status_AccountID[10] = 0;

						LogAddTD("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d [%s][%s]",iBridgeIndex+1,g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT,gObj[TopHitUser].AccountID,gObj[TopHitUser].Name);
					}
					else
					{
						g_BloodCastle.SendNoticeMessage(iBridgeIndex,lMsg.Get(1170));
						LogAddTD("[Blood Castle] (%d) a stone statue Of Saint Terminated -> %d",iBridgeIndex+1,g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT);
					}
				}

				for(int n = 0; n < MAX_BLOOD_CASTLE_SUB_BRIDGE; n++)
				{
					if(g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_iIndex != -1)
					{
						if(gObj[g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_iIndex].Connected > PLAYER_LOGGED)
						{
							g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_bLiveWhenDoorBreak = 1;
						}
#ifdef WZQUEST
						if( (g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_iExtraEXP_Kill_Door_Party ) 
								== gObj[g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_iIndex].PartyNumber )
						{
							OBJECTSTRUCT* tmp_user = &gObj[g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_UserData[n].m_iIndex];

							int bridge_index = g_BloodCastle.GetBridgeIndexByMapNum(tmp_user->MapNumber);
							g_QuestExpProgMng.ChkUserQuestTypeEventMap(66, tmp_user, bridge_index, 1);
						}
#endif
					}
				}
			}
			
			if(CC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				if(rand()%100 < 50)
				{
					g_ChaosCastle.SearchNBlowObjs(lpTargetObj->MapNumber,lpTargetObj->X,lpTargetObj->Y);
				}

				g_ChaosCastle.DelMonsterList(g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber),lpTargetObj->m_Index);

				int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

				if(TopHitUser != -1)
				{
					if(CC_MAP_RANGE(gObj[TopHitUser].MapNumber))
					{
						if(gObjIsConnected(TopHitUser))
						{
							gObj[TopHitUser].m_cKillMonsterCount += 1;

#ifdef WZQUEST
                            int kill_monster_count = gObj[TopHitUser].m_cKillMonsterCount;
							int cc_index = g_ChaosCastle.GetChaosCastleIndex(gObj[TopHitUser].MapNumber);
                            g_QuestExpProgMng.ChkUserQuestTypeEventMap(65, lpObj, cc_index, kill_monster_count);
#endif
						}
					}
				}
			}
#ifdef DP
			if( g_DoppleganerEvent.IsEventMap(lpTargetObj->MapNumber) )
			{
				if( lpTargetObj->Class == 531 )
				{
					LogAddTD("[Doppelganger] Iceworker Die");
					g_DoppleganerEvent.DelIceWorkerIndex(lpTargetObj->m_Index);
				}
				
				if( lpTargetObj->Class == 530 )
				{
					LogAddTD("[Doppelganger] Killer Monster Die MiddleTreasureBox Create");
					g_DoppleganerEvent.SetKillerState(4);
					g_DoppleganerEvent.SetMiddleBossKill(530, lpTargetObj->m_Index);
					g_DoppleganerEvent.AddMiddleTreasureBoxAll(lpTargetObj->X, lpTargetObj->Y);
				}

				if( lpTargetObj->Class == 529 )
				{
					LogAddTD("[Doppelganger] AngerKiller Monster Die MiddleTreasureBox Create");
					g_DoppleganerEvent.SetKillerState(4);
					g_DoppleganerEvent.SetMiddleBossKill(529, lpTargetObj->m_Index);
					g_DoppleganerEvent.AddMiddleTreasureBoxAll(lpTargetObj->X, lpTargetObj->Y);
				}
			}
#endif
			gObjPkDownTimeCheck(lpObj,lpTargetObj->Level);

			if(lpObj->Type == OBJ_USER)
			{
				gObjAddMsgSendDelay(lpObj,3,lpTargetObj->m_Index,2000,0);
			}
		}
		else if(lpTargetObj->Type == OBJ_USER)
		{
			if(BC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_BloodCastle.SetUserState(lpTargetObj->m_Index,1);
				g_BloodCastle.SearchUserDropQuestItem(lpTargetObj->m_Index);
			}

			if(IT_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_IllusionTempleEvent.DropRelicsItem(lpTargetObj->MapNumber, lpTargetObj->m_Index);
			}

			if(CC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_ChaosCastle.SetUserState(lpTargetObj->m_Index,1);

				if(CC_MAP_RANGE(lpObj->MapNumber))
				{
					if(gObjIsConnected(lpObj->m_Index))
					{
						lpObj->m_cKillUserCount += 1;

#ifdef WZQUEST
                        int kill_monster_count = lpObj->m_cKillUserCount;
                        int cc_index = g_ChaosCastle.GetChaosCastleIndex(lpObj->MapNumber);
                        g_QuestExpProgMng.ChkUserQuestTypeEventMap(65, lpObj, cc_index, kill_monster_count);
#endif
					}
				}

				g_ChaosCastle.SendFailMessage(g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber),lpTargetObj->m_Index);

				LogAddTD("[Chaos Castle] (%d) [%s][%s] User Dead In Chaos Castle : Killed by [%s][%s]",g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber)+1,lpTargetObj->AccountID,lpTargetObj->Name,lpObj->AccountID,lpObj->Name);
			}
			
			lpTargetObj->KillerType = 0;

			gObjUserDie(lpTargetObj,lpCallObj);

			int iGuildWar = gObjGuildWarCheck(lpCallObj,lpTargetObj);
			int iDuel = gObjDuelCheck(lpCallObj,lpTargetObj);

			if(iGuildWar == 0 && iDuel == 0)
			{
				if(lpTargetObj->m_bAttackerKilled == 0)
				{
					gObjPlayerKiller(lpCallObj,lpTargetObj, MSBDamage);
				}
			}

			lpTargetObj->m_bAttackerKilled = 0;

			if(lpTargetObj->m_RecallMon >= 0)
			{
				gObjMonsterCallKill(lpTargetObj->m_Index);
			}
#ifdef WZQUEST
			g_QuestExpProgMng.ChkUserQuestTypeEventMap(262, lpObj, lpObj->MapNumber, 0);
#endif
		}

		lpTargetObj->Live = 0;
		lpTargetObj->m_State = 4;
		lpTargetObj->RegenTime = GetTickCount();
		lpTargetObj->DieRegen = 1;
		lpTargetObj->PathCount = 0;
		if(gObjTargetGuildWarCheck(lpTargetObj,lpCallObj)==1)
		{
			lpTargetObj->KillerType = 2;
		}

		if(gObjDuelCheck(lpCallObj,lpTargetObj))
		{
			lpTargetObj->KillerType = 3;
#ifdef NPVP
			g_NewPVP.CheckScore(*lpCallObj, *lpTargetObj); // 1.00.92
#else
			BYTE btDuelRoom = g_DuelRoom.GetUserDuelRoom(lpObj);

			if(lpObj->m_btDuelScore >= 10)
			{
				GCSendEndDuel(lpObj);
				GCSendEndDuel(lpTargetObj);
				GCSendDuelEndNotification(lpTargetObj, lpObj->Name, lpTargetObj->Name);
				GCSendDuelEndNotification(lpObj, lpObj->Name, lpTargetObj->Name);

				gDarkSpirit[lpObj->m_Index].ReSetTarget(lpTargetObj->m_Index);
				gDarkSpirit[lpTargetObj->m_Index].ReSetTarget(lpObj->m_Index);

				char szMsg[256];

				wsprintf(szMsg,lMsg.Get(1216),lpTargetObj->Name);

				GCServerMsgStringSend(szMsg,lpObj->m_Index,1);

				wsprintf(szMsg,lMsg.Get(1217),lpObj->Name);

				GCServerMsgStringSend(szMsg,lpTargetObj->m_Index,1);

				PMSG_SERVERCMD ServerCmd;

				PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
				ServerCmd.CmdType = 0;
				ServerCmd.X = lpObj->X;
				ServerCmd.Y = lpObj->Y;

				MsgSendV2(lpObj,(LPBYTE)&ServerCmd,sizeof(ServerCmd));
				DataSend(lpObj->m_Index,(LPBYTE)&ServerCmd,sizeof(ServerCmd));
				
				gObjAddBuffEffect(lpObj, AT_NEWPVPSYSTEM_REWARD, 6, 20, 0, 0, 3600);
				
				g_DuelRoom.m_Zone[btDuelRoom].dwClearRoomTime = GetTickCount() + 10000;

				LogAdd("[Duel] [%s][%s] Win Duel, Loser [%s][%s]",lpObj->AccountID,lpObj->Name,lpTargetObj->AccountID,lpTargetObj->Name);
			}
#endif
		}

		if(lpTargetObj->Teleport == 1)
		{
			lpTargetObj->Teleport = 0;
		}

		GCDiePlayerSend(lpTargetObj,lpTargetObj->m_Index,Skill,lpObj->m_Index);


#ifdef IMPERIAL
		if ( lpTargetObj->Class >= 524 && lpTargetObj->Class <= 528 && lpTargetObj->Class != 526)
		{
			g_ImperialGuardian.DestroyGate(lpTargetObj->m_ImperialZoneID, lpTargetObj->m_Index, lpObj->m_Index);
		}

		if ( lpObj->Class == 523 )
		{
			LogAddTD("[IMPERIALGUARDIAN] Player Killed by Trap [AccountID]:%s, [NAME]:%s, [ZONE]:%d",
				lpTargetObj->AccountID, lpTargetObj->Name, lpObj->m_ImperialZoneID + 1);
		}

		if ( lpObj->Class >= 504 && lpObj->Class <= 521 )
		{
			LogAddTD("[IMPERIALGUARDIAN] Player Dead [Zone]:%d [AccountID]:%s, [NAME]:%s by Monster => %s(%d)",
				lpObj->m_ImperialZoneID + 1, lpTargetObj->AccountID, lpTargetObj->Name, lpObj->Name, lpObj->m_Index);
		}

		if ( lpTargetObj->Class >= 504 && lpTargetObj->Class <= 521 )
		{
			LogAddTD("[IMPERIALGUARDIAN] Monster Dead [TYPE]: %d,[NAME]: %s, [ZONE]:%d",
				lpTargetObj->Class, lpTargetObj->Name, lpTargetObj->m_ImperialZoneID + 1);
		}

		if ( (lpTargetObj->Class >= 504 && lpTargetObj->Class <= 511) || lpTargetObj->Class == 526 )
		{
			g_ImperialGuardian.DropItem(lpObj->m_Index, lpTargetObj->m_Index);
			return;
		}

		if(lpTargetObj->Class >= 524 && lpTargetObj->Class <= 528 && lpTargetObj->Class != 526)
		{
			return;
		}
#endif

		if(lpTargetObj->MapNumber == MAP_INDEX_RAKLION_BOSS) //season4 add-on
		{
			if(lpTargetObj->Type >= OBJ_MONSTER)
			{
				if( lpTargetObj->Class == 460 ||
					lpTargetObj->Class == 461 ||
					lpTargetObj->Class == 462 )
				{
					g_Raklion.BossEggDie();
					g_RaklionUtil.NotifyRaklionUserMonsterCount(g_Raklion.GetBossEggCount(), 0);
				}
			}
		}

		if(lpTargetObj->Class == 79)
		{
			gEledoradoEvent.m_BossGoldDerconMapNumber[lpTargetObj->m_BossGoldDerconMapNumber] = -1;
			gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);
		}

		if(lpTargetObj->m_RecallMon >= 0)
		{
			return;
		}

		if(lpTargetObj->Class == 340 || lpTargetObj->Class == 348 || lpTargetObj->Class == 349)
		{
			return;
		}

		if(lpTargetObj->Class == 287 || lpTargetObj->Class == 286)
		{
			return;
		}

		if(lpTargetObj->Class == 275)
		{
			return;
		}

//#if(_GSCS == 1 )
		if(lpTargetObj->Class == 288)
		{
			return;
		}
//#endif

		if(lpTargetObj->Class == 295)
		{
			return;
		}

//#if(_GSCS == 0 )
		if(lpTargetObj->Class == 362 || lpTargetObj->Class == 363 || lpTargetObj->Class == 361)
		{
			return;
		}
//#endif
		if(lpTargetObj->Class == 412) //Season 2.5 Dark Elf Hero
		{
			return;
		}

		if(lpCallObj->Type == OBJ_USER)
		{
			if(lpCallObj->PartyNumber >= 0)
			{
				gObjExpParty(lpCallObj,lpTargetObj,AttackDamage,MSBFlag);
			}
			else if(lpTargetObj->Type == OBJ_MONSTER)
			{
				gObjMonsterExpDivision(lpTargetObj,lpCallObj,AttackDamage,MSBFlag);
			}

			if(lpTargetObj->Type == OBJ_MONSTER && DS_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_DevilSquare.gObjMonsterScoreDivision(lpTargetObj,lpCallObj,AttackDamage,MSBFlag);
			}
		}
		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			if(lpTargetObj->m_bIsInMonsterHerd != 0 && lpTargetObj->m_lpMonsterHerd != 0)
			{
				lpTargetObj->m_lpMonsterHerd->MonsterDieAction(lpTargetObj);
			}
		}
	}
	else
	{
		if(DamageSendType == 0)
		{
			GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,MSBDamage,iShieldDamage);
		}
		else if(DamageSendType == 1)
		{
			GCDamageSend(lpTargetObj->m_Index,AttackDamage,iShieldDamage);
		}
		else if(DamageSendType == 2)
		{
			GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,5,iShieldDamage);
		}
		else if(DamageSendType == 3)
		{
			GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,6,iShieldDamage);
		}
	}
}

//00576b10	-> Checked
BOOL gObjInventoryTrans(int aIndex)
{
	if ( ((aIndex< 0)?FALSE:(aIndex > OBJMAX-1)?FALSE:TRUE) == FALSE )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status error %s %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name,
			gObj[aIndex].pTransaction, __FILE__, __LINE__);
		return FALSE;
	}

	for ( int n=0;n<MAX_MAGIC;n++)
	{
		gObj[aIndex].m_lpMagicBack[n] = gObj[aIndex].Magic[n]; 
	}

	for (int n=0;n<MAIN_INVENTORY_SIZE;n++)	//204
	{
		gObj[aIndex].Inventory2[n] = gObj[aIndex].Inventory1[n];
	}

	memcpy(gObj[aIndex].InventoryMap2, gObj[aIndex].InventoryMap1, MAX_INVENTORY_LEN);

	gObj[aIndex].InventoryCount2 = gObj[aIndex].InventoryCount1;
	gObjSetInventory2Pointer(&gObj[aIndex]);
	gObj[aIndex].pTransaction = 1;
	
	LogAddL("Trade Transaction (%s)", gObj[aIndex].Name);

	return TRUE;
}

//00576e80	-> Checked
int gObjInventoryCommit(int aIndex)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		LogAddTD("error : gObjInventoryCommit() - aIndex out of bound %s %d",__FILE__,__LINE__);
		return false;
	}

	if(gObj[aIndex].pTransaction != 1)
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status2",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].pTransaction);
		return false;
	}

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)	//204
	{
		gObj[aIndex].Inventory1[n] = gObj[aIndex].Inventory2[n];
	}

	memcpy(gObj[aIndex].InventoryMap1,gObj[aIndex].InventoryMap2,MAX_INVENTORY_LEN);

	gObj[aIndex].InventoryCount1 = gObj[aIndex].InventoryCount2;

	gObjSetInventory1Pointer(&gObj[aIndex]);

	gObj[aIndex].pTransaction = 2;

	LogAddL("Trade Commit (%s)",gObj[aIndex].Name);

	return true;
}

//005770f0	-> 
int gObjInventoryRollback(int aIndex)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return false;
	}

	if(gObj[aIndex].pTransaction != 1)
	{
		LogAddTD("[%s][%s] error-L3 : pTransaction(%d) status2",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].pTransaction);
		return false;
	}

	for(int n = 0; n < MAX_MAGIC; n++)
	{
		gObj[aIndex].Magic[n] = gObj[aIndex].m_lpMagicBack[n];
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)	//236
	{
		gObj[aIndex].Inventory2[n].Clear();
	}

	gObjSetInventory1Pointer(&gObj[aIndex]);

	gObj[aIndex].pTransaction = 3;

	LogAddL("Trade Rollback (%s)",gObj[aIndex].Name);

	return true;
}

//00577330	-> Checked
void gObjInventoryItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < INVETORY_WEAR_SIZE)
	{
		return;
	}
	else if(itempos > (INVENTORY_SIZE-1))	//OK
	{
		return;
	}

	if(gObj[aIndex].pInventory[itempos].GetSize((int&)width,(int &)height)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjInventoryItemBoxSet(aIndex,itempos,width,height,set_byte);

#ifdef QUESTSYSTEM
	g_QuestSystem.RunItem(&gObj[aIndex], NULL, itempos);
#endif
}

//00566240	-> 100% (1.01.00) [NEW]
BOOL InventoryExtentCheck(int x, int y, int w, int h)
{
	int nEndOfRange, nStartOfRange;
	// ----
	if( y < 0 )
	{
		return false;
	}
	// ----
	if( y >= 8 )	//-> Need rewrite too)
	{
		if( y >= 12 )
		{
			if( y >= 16 )
			{
				if( y >= 20 )
				{
					if( y >= 24 )
					{
						if( y >= 28 )
						{
							return false;
						}
						// ----
						nStartOfRange	= 24;
						nEndOfRange		= 28;
					}
					else
					{
						nStartOfRange	= 20;
						nEndOfRange		= 24;
					}
				}
				else
				{
					nStartOfRange	= 16;
					nEndOfRange		= 20;
				}
			}
			else
			{
				nStartOfRange	= 12;
				nEndOfRange		= 16;
			}
		}
		else
		{
			nStartOfRange	= 8;
			nEndOfRange		= 12;
		}
	}
	else
	{
		nStartOfRange	= 0;
		nEndOfRange		= 8;
	}
	// ----
	if( nEndOfRange > h )
	{
		nEndOfRange = h;
	}
	// ----
	return x >= 0 && x < w && y >= nStartOfRange && y < nEndOfRange;
}


//00566370	-> 100% (1.01.00) [NEW]
BOOL CheckOutOfInventory(int aIndex, int sy, int height)
{
	int nEndofItemY = sy + height - 1;
	// ----
	if( sy < 0 )
	{
		return false;
	}
	// ----
	if( sy >= 8 ) //-> Need rewrite xD
	{
		if( sy >= 12 )
		{
			if( sy >= 16 )
			{
				if( sy >= 20 )
				{
					if( sy >= 24 )
					{
						if( sy >= 28 )
						{
							return false;
						}
						// ----
						if( nEndofItemY >= 28 )
						{
							return false;
						}
					}
					else
					{
						if( nEndofItemY >= 24 )
						{
							return false;
						}
					}
				}
				else
				{
					if( nEndofItemY >= 20 )
					{
						return false;
					}
				}
			}
			else
			{
				if( nEndofItemY >= 16 )
				{
					return false;
				}
			}
		}
		else
		{
			if( nEndofItemY >= 12 )
			{
				return false;
			}
		}
	}
	else
	{
		if( nEndofItemY >= 8 )
		{
			return false;
		}
	}
	// ----
	return true;
}

//005664f0	-> 100% (1.01.00) [NEW]
BOOL CheckOutOfWarehouse(int aIndex, int sy, int height)
{
	if( sy >= 15 * gObj[aIndex].ExpandedWarehouse + 15 )
	{
		return false;
	}
	// ----
	int nEndofItemY = sy + height - 1;
	// ----
	if( sy < 0 )
	{
		return false;
	}
	// ----
	if( sy >= 15 )
	{
		if( sy >= 30 )
		{
			return false;
		}
		// ----
		if( nEndofItemY >= 30 )
		{
			return false;
		}
	}
	else
	{
		if( nEndofItemY >= 15 )
		{
			return false;
		}
	}
	// ----
	return true;
}

//00566440	-> 100% (1.01.00) [NEW]
BOOL WarehouseExtentCheck(int x, int y, int w, int h)
{
	int nEndOfRange, nStartOfRange;
	// ----
	if( y < 0 )
	{
		return false;
	}
	// ----
	if( y >= 15 )
	{
		if( y >= 30 )
		{
			return false;
		}
		// ----
		nStartOfRange	= 15;
		nEndOfRange		= 30;
	}
	else
	{
		nStartOfRange	= 0;
		nEndOfRange		= 15;
	}
	// ----
	if( nEndOfRange > h )
	{
		nEndOfRange = h;
	}
	// ----
	return x >= 0 && x < w && y >= nStartOfRange && y < nEndOfRange;
}

//00577400	-> Checked
void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE)%8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE)/8;

	int xx,yy;

#ifdef EXPINV
	if( !CheckOutOfInventory(aIndex, itemposy, yl) )	//OK
	{
		return;
	}
#endif

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

#ifdef EXPINV
			if( InventoryExtentCheck(xx,yy,8,MAX_INVENTORY_H2) )
#else
			if(ExtentCheck(xx,yy,8,MAX_INVENTORY_H2)==1)
#endif
			{
				*(BYTE*)(gObj[aIndex].pInventoryMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				return;
			}
		}
	}
}

bool gObjFixInventoryPointer(int aIndex)
{

	if(gObjIsConnected(aIndex)==0)
	{
		LogAdd("[Fix Inv.Ptr] [%s][%s] - disconnected",gObj[aIndex].AccountID,gObj[aIndex].Name);
		return false;
	}


	if(gObj[aIndex].pInventory == gObj[aIndex].Inventory1)
	{
		return true;
	}

	if(gObj[aIndex].pInventory == gObj[aIndex].Inventory2)
	{
		if(gObj[aIndex].pTransaction == 1)
		{
			LogAdd("[Fix Inv.Ptr] [%s][%s] - Transaction == 1, Do not change Pointer",gObj[aIndex].AccountID,gObj[aIndex].Name);
			return false;
		}
		else
		{
			LogAdd("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was 2",gObj[aIndex].AccountID,gObj[aIndex].Name);

			for(int n = 0; n < INVENTORY_SIZE; n++)
			{
				gObj[aIndex].Inventory2[n].Clear();
			}
		}
	}
	else
	{
		LogAdd("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was Wrong",gObj[aIndex].AccountID,gObj[aIndex].Name);
	}

	gObjSetInventory1Pointer(&gObj[aIndex]);

	return false;
}

//005777e0	-> Checked
void gObjInventoryItemSet_PShop(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;

	if(itempos < INVETORY_WEAR_SIZE)
	{
		return;
	}
	else if(itempos > (INVENTORY_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].Inventory1[itempos].GetSize((int&)width,(int &)height)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjInventoryItemBoxSet_PShop(aIndex,itempos,width,height,set_byte);
}

//005778b0	-> Checked
void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE)%8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE)/8;

	int xx,yy;

#ifdef EXPINV
	if( !CheckOutOfInventory(aIndex, itemposy, yl) )
	{
		return;
	}
#endif

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

#ifdef EXPINV
			if( InventoryExtentCheck(xx, yy, 8, MAX_INVENTORY_H2) )
#else
			if(ExtentCheck(xx,yy,8,MAX_INVENTORY_H2)==1)
#endif
			{
				*(BYTE*)(gObj[aIndex].InventoryMap1 + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return;
			}
		}
	}
}

//00577a10 -> Checked
BYTE gObjInventoryDeleteItem(int aIndex ,int itempos)
{
	if(itempos < INVETORY_WEAR_SIZE)
	{
		int MagicDel = gObjMagicDel(&gObj[aIndex],gObj[aIndex].pInventory[itempos].m_Special[0],gObj[aIndex].pInventory[itempos].m_Level);

		if(MagicDel >= 0)
		{
			GCMagicListOneDelSend(aIndex,MagicDel,
				gObj[aIndex].pInventory[itempos].m_Special[0],
				gObj[aIndex].pInventory[itempos].m_Level,0,0);
		}

		gObj[aIndex].pInventory[itempos].Clear();
	}
	else
	{
		gObjInventoryItemSet(aIndex,itempos,255);
		gObj[aIndex].pInventory[itempos].Clear();
	}

	return true;
}

BYTE gObjWarehouseDeleteItem(int aIndex, int itempos)
{
	gObjWarehouseItemSet(aIndex, itempos, -1);
	gObj[aIndex].pWarehouse[itempos].Clear();

	return true;
}

BYTE gObjChaosBoxDeleteItem(int aIndex, int itempos)
{
	gObjChaosItemSet(aIndex, itempos, -1);
	gObj[aIndex].pChaosBox[itempos].Clear();

	return true;
}

//00577c70	-> Checked
BYTE gObjInventoryInsertItem(int aIndex, int item_num)
{
	int w,h,map_num,iwidth,iheight;
	BYTE blank;

	blank = 0;
	map_num = gObj[aIndex].MapNumber;

	if(MapC[map_num].m_cItem[item_num].live == 0)
	{
		return -1;
	}

	if(MapC[map_num].m_cItem[item_num].GetSize((int &)iwidth,(int &)iheight) == 0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					gObj[aIndex].pInventory[blank] = MapC[map_num].m_cItem[item_num];
					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;

}

DWORD gGetItemNumber()	
{
	return gItemNumberCount;
}

void gPlusItemNumber() 
{
	return;
}

BYTE gObjInventoryInsertItem(int aIndex, CItem item)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize((int&)iwidth,(int&)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					if(gObjCheckSerial0ItemList(&item)!=0)
					{
						MsgOutput(aIndex,lMsg.Get(3354));
						return -1;
					}

					if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
					{
						return -1;
					}

					gObj[aIndex].pInventory[blank] = item;

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

//005781e0	-> Checked
BYTE gObjOnlyInventoryInsertItem(int aIndex, CItem item)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize((int&)iwidth,(int&)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)	//OK
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					if(gObjCheckSerial0ItemList(&item)!=0)
					{
						MsgOutput(aIndex,lMsg.Get(3354));
						return -1;
					}

					if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
					{
						return -1;
					}

					gObj[aIndex].pInventory[blank] = item;

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

//00578450	-> Checked
BYTE gObjInventoryInsertItem(int aIndex, CMapItem * item)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	CItem copyitem;


	if(item->GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);
				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
	
				if(blank != 255)
				{
					copyitem.m_Level		= item->m_Level;
					copyitem.m_Durability	= item->m_Durability;

					copyitem.Convert(item->m_Type,item->m_Option1,item->m_Option2,item->m_Option3,item->m_NewOption,item->m_SetOption,item->m_ItemOptionEx,item->m_SocketOption, item->m_BonusSocketOption, 0, CURRENT_DB_VERSION);
					copyitem.SetPetItemInfo(item->m_PetItem_Level,item->m_PetItem_Exp);
	
					copyitem.m_Number = item->m_Number;
	
					gObj[aIndex].pInventory[blank] = copyitem;

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

//00578730	-> Checked
BOOL gObjSearchItem(LPOBJ lpObj, int item_type, int add_dur, int nLevel)
{
	for(int n = INVETORY_WEAR_SIZE; n < MAIN_INVENTORY_SIZE; n++)	//204
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == item_type)
			{
				if(lpObj->pInventory[n].m_Level == nLevel)
				{
					if((int)lpObj->pInventory[n].m_Durability + add_dur <= 3)
					{
						lpObj->pInventory[n].m_Durability += add_dur;
						GCItemDurSend(lpObj->m_Index,n,lpObj->pInventory[n].m_Durability,0);
						return true;
					}
				}
			}
		}
	}
	return false;
}

//New CashShop 
BOOL gObjCashShopSearchItem(LPOBJ lpObj, int item_type, int nLevel, int add_dur, int arg5)
{
	for(int n = INVETORY_WEAR_SIZE; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == item_type)
			{
				if(lpObj->pInventory[n].m_Level == nLevel) //Arg.3
				{
					if( ((float)lpObj->pInventory[n].m_Durability + add_dur) <= (float)arg5)
					{
						lpObj->pInventory[n].m_Durability += add_dur;
						GCItemDurSend(lpObj->m_Index,n,lpObj->pInventory[n].m_Durability,0);
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}


BOOL gObjSearchItemMinus(LPOBJ lpObj, int pos, int m_dur)
{
	int n = pos;
	if(n < 0 || n > (MAIN_INVENTORY_SIZE-1))
	{
		LogAdd("error-L4 : %s %d",__FILE__,__LINE__);
		return 1;
	}

	if(lpObj->pInventory[n].IsItem() == 1)
	{
		if((int)lpObj->pInventory[n].m_Durability - m_dur > 0)
		{
#ifdef __ALIEN__
			if( lpObj->pInventory[n].m_Type >= ITEMGET(14, 0) && lpObj->pInventory[n].m_Type <= ITEMGET(14, 6) 
				|| lpObj->pInventory[n].m_Type >= ITEMGET(14, 35) && lpObj->pInventory[n].m_Type <= ITEMGET(14, 40) 
				|| lpObj->pInventory[n].m_Type== ITEMGET(14, 70) || lpObj->pInventory[n].m_Type == ITEMGET(14, 71) )
			{
				lpObj->m_PotionTick = GetTickCount();
			}
#endif
			lpObj->pInventory[n].m_Durability -= m_dur;
			GCItemDurSend(lpObj->m_Index,n,lpObj->pInventory[n].m_Durability,1);
			return 1;
		}
	}
	return 0;
}

//00578b50	-> Checked
BYTE gObjShopBuyInventoryInsertItem(int aIndex, CItem item)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize(iwidth,iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)	//24
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					gObj[aIndex].pInventory[blank] = item;

					if(item.m_serial == 0)
					{
						gObj[aIndex].pInventory[blank].m_Number = 0;
					}
					else
					{
						gObj[aIndex].pInventory[blank].m_Number = gGetItemNumber();
						gPlusItemNumber();
					}

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

//00578dc0	-> Checked
BYTE gObjShopBuyInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;
	CItem item;

	int item_type = ItemGetNumberMake(type,index);
	item.m_Level = level;
	item.m_Durability = iDur;

	item.Convert(item_type,0,0,0,0,0,0,NULL, 0xFF, 0, CURRENT_DB_VERSION);

	if(item.GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)	//24
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;

					if ( !item.m_serial )
					{
						lpObj->pInventory[blank].m_Number = 0;
					}
					else
					{
						lpObj->pInventory[blank].m_Number = gGetItemNumber();
						gPlusItemNumber();
					}
					
					gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

//00579070	-> Checked
BYTE gObjInventoryInsertItemTemp(LPOBJ lpObj, CMapItem * Item)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(Item->GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)	//24
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

//00579220	-> Checked
BYTE gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	int item_type = ItemGetNumberMake(type,index);

	item.m_Level = level;

	item.Convert(item_type,0,0,0,0,0,0,NULL, 0xFF, 0, CURRENT_DB_VERSION);

	if(item.GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)	//24
	{
		for( w = 0; w < 8; w++)
		{

			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
#ifdef EXPINV
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);
#else
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);
#endif
				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
	
				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					lpObj->pInventory[blank].m_Number = gGetItemNumber();
					gPlusItemNumber();

					gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

//005794f0	-> Checked
BYTE gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	int item_type = ItemGetNumberMake(type,index);
	item.m_Level = level;
	item.m_Durability = iDur;

	item.Convert(item_type,0,0,0,0,0,0,NULL, 0xFF, 0, CURRENT_DB_VERSION);

	if(item.GetSize((int &)iwidth,(int &)iheight)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)	//24
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					lpObj->pInventory[blank].m_Number = iSerial;
					gPlusItemNumber();

					gObjInventoryItemSet(lpObj->m_Index,blank,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

//005797c0	-> Checked
BYTE gObjMonsterInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int op1, int op2, int op3)
{
	CItem item;
	int w,h,iwidth,iheight;
	BYTE blank = 0;
	int item_type;
	int a = 0;

	item_type = ItemGetNumberMake(type,index);
	item.m_Level = level;

	if(type == 13 && index == 10)
	{
		//item.m_Durability = rand()%100+100.0f;
		item.m_Durability = (float)100+(rand()%100); //HermeX Fix from GS 00.78T
	}

	item.Convert(item_type,op1,op2,op3,0,0,0,NULL, 0xFF, 0, CURRENT_DB_VERSION);

	lpObj->pInventory[blank] = item;
	iwidth = 1;
	iheight = 1;

	for(h = 0; h < MAX_INVENTORY_H; h++)	//24
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					lpObj->pInventory[blank] = item;
					*lpObj->pInventoryCount += 1;

					gObjInventoryItemBoxSet(lpObj->m_Index,blank,iwidth,iwidth,lpObj->pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

//00579ad0	-> Checked
BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x,y;
	int blank = 0;

	if(sx + width > 8)
	{
		return -1;
	}

	if(sy + height > MAX_INVENTORY_H2)	//28
	{
		return -2;
	}
	// ----
	bool bResult = CheckOutOfInventory(aIndex, sy, height);
	// ----
	if( !bResult )
	{
		if( bResult == -1 )
		{
			return -2;
		}
		// ----
		return -1;
	}
	// ----
	int xx,yy;

	for(y = 0; y < height; y ++)
	{

		yy = sy+y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

#ifdef EXPINV
			if( InventoryExtentCheck(xx,yy,8,MAX_INVENTORY_H2) )
#else
			if(ExtentCheck(xx,yy,8,MAX_INVENTORY_H2)==1)	//28
#endif
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+(sy+y)*8+(sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sx+sy*8+12;	//strange
	}
	return  -1;
}

//00579c80	-> Checked
BYTE gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x,y;
	int blank = 0;

	if(sx + width > 8)
	{
		return -1;
	}

#ifdef EXPINV
	if( sy + height > 4 * gObj[aIndex].ExpandedInventory + 8 )
	{
		return -2;
	}
#else
	if(sy + height > 8)
	{
		return -2;
	}
#endif
	int nInventoyHeightofCurUser = CheckOutOfInventory(aIndex, sy, height);
	if ( !nInventoyHeightofCurUser )
	{
		if ( nInventoyHeightofCurUser == -1 )
		{
			return -2;
		}
		return -1;
	}

	int xx,yy;

	for(y = 0; y < height; y ++)
	{

		yy = sy+y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

#ifdef EXPINV
			if( InventoryExtentCheck(xx, yy, 8, MAX_INVENTORY_H) )	//24
#else
			if(ExtentCheck(xx,yy,8,8)==1)
#endif
			{
				if(*(BYTE*)(gObj[aIndex].pInventoryMap+(sy+y)*8+(sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sx+sy*8+12;	//strange again
	}
	return  -1;
}


BOOL CheckLotteryInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth) //season4 add-on
{
	int h=0;
	int w=0;
	BYTE blank = 0;
	
	for(h=0;h<MAX_INVENTORY_H;h++)
	{
		for(w=0;w<8;w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+(h)*8+(w)) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index, w, h, iItemWidth, iItemHeight);

				if ( blank == 0xFE )
					return FALSE;

				if ( blank != 0xFF )
					return TRUE;
			}
		}
	}

	return FALSE;
}

//00579e60	-> Checked
BOOL CheckInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth)
{
	int h=0;
	int w=0;
	BYTE blank = 0;
	
	for(h=0;h<MAX_INVENTORY_H;h++)	//24
	{
		for(w=0;w<8;w++)
		{
			if(*(BYTE*)(lpObj->pInventoryMap+(h)*8+(w)) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index, w, h, iItemWidth, iItemHeight);

				if ( blank == 0xFE )
					return FALSE;

				if ( blank != 0xFF )
					return TRUE;
			}
		}
	}

	return FALSE;
}

//00579f60	-> 1.01.00 (identical with CheckInventoryEmptySpace)
int CheckInventoryEmptySpace_(LPOBJ lpObj, int iItemHeight, int iItemWidth)
{
	int h  = 0;
	int w  = 0;
	BYTE blank = 0;
	// ----
	for (h=0; h<MAX_INVENTORY_H; h++)
	{
		for (w=0; w<8; w++)
		{
			if (*(BYTE*)(lpObj->pInventoryMap+(h)*8+(w)) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index, w, h, iItemWidth, iItemHeight);
				// ----
				if (blank == 0xFE)
				{
					return FALSE;
				}
				// ----
				if (blank != 0xFF)
				{
					return TRUE;
				}
			}
		}
	}
	// ----
	return FALSE;
}

BOOL gObjIsItemPut(LPOBJ lpObj, CItem * lpItem, int pos ) //0052EF50
{
	if(lpItem->m_TwoHand == 1)
	{
		if(pos == 0 || pos == 1)
		{
			if(lpObj->pInventory[0].IsItem() == 1)
			{
				if(lpObj->pInventory[0].m_Type == ITEMGET(4,7) || lpObj->pInventory[0].m_Type == ITEMGET(4,15))
				{
					
				}
				else
				{
					return false;
				}
			}

			if(lpObj->pInventory[1].IsItem() == 1)
			{
				if(lpObj->pInventory[1].m_Type == ITEMGET(4,7) || lpObj->pInventory[1].m_Type == ITEMGET(4,15))
				{
					
				}
				else
				{
					return false;
				}
			}
		}
	}

	if(pos == 1)
	{
		if(lpObj->pInventory[0].IsItem() == 1)
		{
			if(lpItem->m_Type == ITEMGET(4,7) || lpItem->m_Type == ITEMGET(4,15))
			{
					
			}
			else
			{
				if(lpObj->pInventory[0].m_TwoHand)
				{
					return false;
				}
			}
		}
	}

	if(lpObj->Class == CLASS_DARKLORD)
	{
		if((lpItem->m_Type>>9) == 2)
		{
			if(lpItem->m_Part != pos)
			{
				return false;
			}
		}
	}

	if(lpObj->Class == CLASS_WIZARD)
	{
		if((lpItem->m_Type>>9) == 5)
		{
			if(lpItem->m_Part != pos)
			{
				return false;
			}
		}
	}

	if(lpObj->Class == CLASS_SUMMONER)
	{
		if((lpItem->m_Type>>9) == 5)
		{
			if(lpItem->m_Part != pos)
			{
				return false;
			}
		}
	}

	if(lpItem->m_Part != pos)
	{
		if(pos == 1 || pos == 0)
		{
			if(lpItem->m_Part == 1 || lpItem->m_Part == 0)
			{

			}
			else
			{
				return false;
			}
		}
		else if(pos == 11)
		{
			if(lpItem->m_Part == 10)
			{

			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if((lpObj->Strength + lpObj->AddStrength) < (lpItem->m_RequireStrength - lpItem->m_HJOpStrength))
	{
		return false;
	}

	if((lpObj->Dexterity + lpObj->AddDexterity) < (lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity))
	{
		return false;
	}

	if(lpObj->Level < lpItem->m_RequireLevel)
	{
		return false;
	}

	if(lpItem->m_Type >= 0 && lpItem->m_Type < ITEMGET(12,0))
	{
		if((lpObj->Vitality + lpObj->AddVitality) < lpItem->m_RequireVitality)
		{
			return false;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < lpItem->m_RequireEnergy)
		{
			return false;
		}
	}

	if((lpObj->Leadership + lpObj->AddLeadership) < lpItem->m_RequireLeaderShip)
	{
		return false;
	}

	if(lpItem->m_Type >= ITEMGET(12,0) && lpItem->m_Type <= ITEMGET(12,6))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
	
	if(lpItem->m_Type == ITEMGET(12,41))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
#ifdef NEWWINGS
	if( IS_NEWWINGS(lpItem->m_Type) )
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
#endif
	if(lpItem->m_Type == ITEMGET(12,42) || lpItem->m_Type == ITEMGET(12,43))
	{
		if(lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}
	else if(lpItem->m_Type >= ITEMGET(13,0) && lpItem->m_Type <= ITEMGET(13,15))
	{
		if(lpItem->m_Type == ITEMGET(13,10))
		{
			if(lpItem->m_Level <= 2)
			{
				if(lpObj->Level < 20)
				{
					return false;
				}
			}
			else
			{
				if(lpObj->Level < 50)
				{
					return false;
				}
			}
		}
		else if(lpItem->m_Type == ITEMGET(13,2))
		{
			if(lpObj->MapNumber == 7 || lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
		else if(lpItem->m_Type == ITEMGET(13,3))
		{
			if(lpObj->MapNumber == 7 || lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
		else
		{
			if(lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
	}
	
	
	if( IT_MAP_RANGE(lpObj->MapNumber) )
	{
		if(g_IllusionTempleEvent.CheckChangeRing(lpItem->m_Type) != FALSE)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,20) && lpItem->m_Level == 0)
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,20,0);

		if(count != 0)
		{
			return false;
		}
	}
	
	if( lpItem->m_Type == ITEMGET(13, 107) && lpItem->m_Level == 0 ) //1.01.03
	{
			int count = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 107, 0);
			// ----
			if( count != 0 )
			{
				return false;
			}
	}

	if(lpItem->m_Type == ITEMGET(13,40)) //Second Edition
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,40,0);

		if(count > 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,41)) //Season 2.5 Santa Girl Ring Addition
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,41,0);

		if(count > 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,42)) //Season 2.5 Game Master Ring Addition
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}

		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,42,0);

		if(count > 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,10))
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,37))
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,38))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,38,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,39))
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}

		int count = 0;

		for(int n = 0; n < 7; n++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,10))
	{
		int count = 0;

		for(int n = 0; n < 7; n ++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}
	}
	if(lpItem->m_Type == ITEMGET(13,40)) //Second Edition
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}

		int count = 0;

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,40,0);

		if(count > 0)
		{
			return false;
		}
	}
	if(lpItem->m_Type == ITEMGET(13,41)) //Season 2.5 Santa Girl Ring Addition
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}

		int count = 0;

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,41,0);

		if(count > 0)
		{
			return false;
		}
	}
	if(lpItem->m_Type == ITEMGET(13,68)) //Season 4.0 XMas Ring Addition
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}

		int count = 0;

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,68,0);

		if(count > 0)
		{
			return false;
		}
	}
	if(lpItem->m_Type == ITEMGET(13,76)) //Season 4.6 Panda Ring Addition
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}

		int count = 0;

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,76,0);

		if(count > 0)
		{
			return false;
		}
	}
	if(lpItem->m_Type == ITEMGET(13, 122)) // Season 5 Episode 2 JPN Skeleton Ring
	{
		if(CC_MAP_RANGE(lpObj->MapNumber))
		{
			return false;
		}
		// ----
		int count = 0;
		// ----
		count = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 122, 0);
		// ----
		if(count > 0)
		{
			return false;
		}
	}
	else if(lpItem->m_Type >= ITEMGET(14,00))
	{
		return false;
	}

	return true;
}

//0057afd0	-> Checked
BYTE gObjWerehouseRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x,y;
	int blank = 0;

	if(sx + width > 8)
	{
		return -1;
	}

	if(sx + width < 0)
	{
		return -1;
	}

	/*if(sy + height > 15)
	{
		return -1;
	}

	if(sy + height < 0)
	{
		return -1;
	}*/

#ifdef EXPINV
	if( !CheckOutOfWarehouse(aIndex, sy, height) )
	{
		return -1;
	}
#endif

	int xx,yy;

	LPOBJ lpObj = &gObj[aIndex];

	for(y = 0; y < height; y ++)
	{

		yy = sy+y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

#ifdef EXPINV
			if( WarehouseExtentCheck(xx, yy, 8, MAX_WAREHOUSE_H) )
#else
			if(ExtentCheck(xx,yy,8,MAX_WAREHOUSE_H)==1)
#endif
			{
				if(*(BYTE*)(gObj[aIndex].pWarehouseMap+(sy+y)*8+(sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sx+sy*8;
	}
	return  -1;
}

BYTE gObjMapRectCheck(BYTE * lpMapBuf, int sx, int sy, int ex, int ey, int width, int height)
{
	int x,y;
	int blank = 0;

	if(sx + width > ex || sx + width < 0)
	{
		return -1;
	}

	if(sy + height > ey || sy + height < 0)
	{
		return -1;
	}

	int xx,yy;

	for(y = 0; y < height; y++)
	{
		yy = sy + y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			if(ExtentCheck(xx,yy,ex,ey)==1)
			{
				if(*(BYTE*)(lpMapBuf + (sy + y)*ex + (sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sy*ex+sx;
	}
	return  -1;

	return -1;
}

//0057b2f0	-> Checked
void gObjWarehouseItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int xx,yy;

#ifdef EXPINV
	if( !CheckOutOfWarehouse(aIndex, itemposy, yl) )
	{
		return;
	}
#endif

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

#ifdef EXPINV
			if( WarehouseExtentCheck(xx, yy, 8, MAX_WAREHOUSE_H) )
#else
			if(ExtentCheck(xx,yy,8,MAX_WAREHOUSE_H)==1)
#endif
			{
				*(BYTE*)(gObj[aIndex].pWarehouseMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				LogAdd("error : %d/%d  %s %d",xx,yy,__FILE__,__LINE__);
				return;
			}
		}
	}
}

void gObjWarehouseItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < 0)
	{
		return;
	}
	else if(itempos > (WAREHOUSE_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].pWarehouse[itempos].GetSize((int&)width,(int &)height)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjWarehouseItemBoxSet(aIndex,itempos,width,height,set_byte);
}

void gObjChaosItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,4)==1)
			{
				*(BYTE*)(gObj[aIndex].pChaosBoxMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				LogAdd("error : %d/%d  %s %d",xx,yy,__FILE__,__LINE__);
				return;
			}
		}
	}
}

void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < 0)
	{
		return;
	}
	else if(itempos > (CHAOS_BOX_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].pChaosBox[itempos].GetSize((int&)width,(int &)height)==0)
	{
		LogAdd(lMsg.Get(527),__FILE__,__LINE__);
		return;
	}

	gObjChaosItemBoxSet(aIndex,itempos,width,height,set_byte);
}

BYTE gObjChaosBoxInsertItemPos(int aIndex, CItem item, int pos, int source)
{

	LPOBJ lpObj;
	BYTE TempMap[CHAOS_BOX_MAP_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass;

	if(pos < 0 || pos > (CHAOS_BOX_SIZE-1))
	{
		return -1;
	}

	useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pChaosBox[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	w = pos%8;
	h = pos/8;

	if(ExtentCheck(w,h,8,4) == 0)
	{
		return -1;
	}

	item.GetSize((int &)iwidth,(int &)iheight);

	if(source >= 0)
	{
		memcpy(TempMap,lpObj->pChaosBoxMap,CHAOS_BOX_MAP_SIZE);
		gObjChaosItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	}

	if(*(BYTE*)(lpObj->pChaosBoxMap + h * 8 + w) != 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pChaosBoxMap,TempMap,CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	blank = gObjMapRectCheck(lpObj->pChaosBoxMap,w,h,8,4,iwidth,iheight);

	if(blank == 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pChaosBoxMap,TempMap,CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	lpObj->pChaosBox[pos] = item;
	return pos;
}

//0057b980	-> Checked
BYTE gObjWarehouseInsertItemPos(int aIndex, CItem item, int pos, int source)
{
	LPOBJ lpObj;
	BYTE TempMap[WAREHOUSE_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass;

	if(pos < 0 || pos > (WAREHOUSE_SIZE-1))	//OK
	{
		return -1;
	}

	useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pWarehouse[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	w = pos%8;
	h = pos/8;

#ifdef EXPINV
	if( !WarehouseExtentCheck(w, h, 8, MAX_WAREHOUSE_H) )
#else
	if(ExtentCheck(w,h,8,MAX_WAREHOUSE_H) == 0)
#endif
	{
		return -1;
	}

	item.GetSize((int &)iwidth,(int &)iheight);

	if(source >= 0)
	{
		memcpy(TempMap,lpObj->pWarehouseMap,WAREHOUSE_SIZE);
		gObjWarehouseItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	}

	if(*(BYTE*)(lpObj->pWarehouseMap + h * 8 + w) != 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pWarehouseMap,TempMap,WAREHOUSE_SIZE);
		}
		return -1;
	}

	blank = gObjWerehouseRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

	if(blank == 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pWarehouseMap,TempMap,WAREHOUSE_SIZE);
		}
		return -1;
	}

	lpObj->pWarehouse[pos] = item;
	return pos;
}

//0057bc30	-> Checked maybe =D
BYTE gObjInventoryInsertItemPos(int aIndex, CItem item, int pos, BOOL RequestCheck)
{
	if(pos < 0 || pos > INVENTORY_SIZE)	//236 from 6.2
	{
		return -1;
	}

	LPOBJ lpObj;
	BYTE TempInventoryMap[INVENTORY_MAP_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pInventory[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	if(pos < 12)
	{
		useClass = item.IsClass(lpObj->Class,lpObj->ChangeUP, lpObj->ThirdChangeUp);

		if(useClass == 0)
		{
			LogAdd(lMsg.Get(528));
			return -1;
		}

		if(RequestCheck != 0)
		{
			if(gObjIsItemPut(lpObj,&item,pos) == 0)
			{
				return -1;
			}
		}
	}
	else
	{
		w = (pos - 12)%8;
		h = (pos - 12)/8;
		
		item.GetSize((int &)iwidth,(int &)iheight);

#ifdef EXPINV
		bool bResult = CheckOutOfInventory(aIndex, h, iheight);
		// ----
		if( !bResult )
		{
			if( bResult == -1 )
			{
				return -2;
			}
			// ----
			return -1;
		}
		// ----
		if( !InventoryExtentCheck(w, h, 8, MAX_INVENTORY_H2) )	//OK
		{
			return -1;
		}
#else
		if(ExtentCheck(w,h,8,12)==0)
		{
			return -1;
		}
#endif
		memcpy(TempInventoryMap,lpObj->pInventoryMap,INVENTORY_MAP_SIZE);	//OK
		

		if(*(BYTE*)(lpObj->pInventoryMap + h * 8 + w) != 255 )
		{
			memcpy(lpObj->pInventoryMap,TempInventoryMap,INVENTORY_MAP_SIZE);	//OK
			return -1;
		}

		blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank >= 254)
		{
			memcpy(lpObj->pInventoryMap,TempInventoryMap,INVENTORY_MAP_SIZE);	//OK
			return false;
		}
	}

	lpObj->pInventory[pos] = item;

	if(pos > 11)
	{
		gObjInventoryItemSet(aIndex,pos,1);
	}

	if(pos < 12)
	{
		CItem * lpItem = &lpObj->pInventory[pos];

		if(lpItem->m_Option1)
		{
			int result = gObjWeaponMagicAdd(lpObj,lpItem->m_Special[0],lpItem->m_Level);

			if(result >= 0)
			{
				GCMagicListOneSend(lpObj->m_Index,result,lpItem->m_Special[0],lpItem->m_Level,0,0);
			}
		}
	}
	return pos;
}

BOOL gObjInventorySearchSerialNumber(LPOBJ lpObj, DWORD serial)
{
	int s_num;
	int count = 0;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if(serial == 0)
	{
		return true;
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

		if(s_num != 0 && s_num == serial)
		{
			count++;
		}
	}

	if(count <= 1)
	{
		return true;
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

		if(s_num != 0 && s_num == serial)
		{
			LogAddTD(lMsg.Get(529),lpObj->AccountID,lpObj->Name,lpObj->pInventory[n].GetName(),n,s_num);
			GCServerMsgStringSend(lMsg.Get(1102),lpObj->m_Index,1);
			gObjUserKill(lpObj->m_Index);
		}
	}

	return false;
}

BOOL gObjWarehouseSearchSerialNumber(LPOBJ lpObj, DWORD sirial)
{
	int s_num;
	int count = 0;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if(sirial == 0)
	{
		return true;
	}

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

		if(s_num != 0 && s_num == sirial)
		{
			count++;
		}
	}

	if(count <= 1)
	{
		return true;
	}

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

		if(s_num != 0 && s_num == sirial)
		{
			LogAddTD(lMsg.Get(530),lpObj->AccountID,lpObj->Name,lpObj->pWarehouse[n].GetName(),n,s_num);
			GCServerMsgStringSend(lMsg.Get(1102),lpObj->m_Index,1);
			gObjUserKill(lpObj->m_Index);
		}
	}

	return false;
}

//0057c470	-> OK
BYTE gObjInventoryMoveItem(int aIndex, BYTE source, BYTE target, int& durSsend, int& durTsend, BYTE sFlag, BYTE tFlag, LPBYTE siteminfo)
{
	LPOBJ lpObj;
	BYTE TempInventoryMap[MAX_INVENTORY_LEN];
	int w,h;
	int iwidth,iheight;
	int blank;
	int s_num;

	CItem * sitem;
	CItem * titem;

	int bPersonalShopTrans = 0;
	int bSourceIsPShop = 0;

	durSsend = 0;
	durTsend = 0;

	lpObj = &gObj[aIndex];

	int useClass = 0;
	__try
	{
		if(sFlag == 2 || tFlag == 2)	//OK
		{
			if(lpObj->m_IfState.type != 6)
			{
				LogAdd(lMsg.Get(531),lpObj->AccountID,lpObj->Name);
				return -1;
			}

			if(lpObj->m_IfState.state == 0)
			{
				LogAdd(lMsg.Get(531),lpObj->AccountID,lpObj->Name);
				return -1;
			}
		}

		//sw1
		switch(sFlag)
		{
		case 0:
			if(source < 0 || source > (MAIN_INVENTORY_SIZE-1))	//OK
			{
				LogAdd("error-L1 : %s %d",
					__FILE__,
					__LINE__);
				return -1;
			}

			if(lpObj->pInventory[source].IsItem() == 0)
			{
				LogAdd(lMsg.Get(532),lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pInventory[source];

			if(lpObj->MapNumber == MAP_INDEX_ICARUS)
			{
				if(source == 8)
				{
					if(sitem->m_Type == ITEMGET(13,3))	//OK
					{
						if(lpObj->pInventory[7].IsItem() == 0)
						{
							return -1;
						}
					}
					else if(sitem->m_Type == ITEMGET(13,37)) //OK
					{
						if(lpObj->pInventory[7].IsItem() == 0)
						{
							return -1;
						}
					}
					else if(sitem->m_Type == ITEMGET(13,4)) //season4 add-on
					{
						if(lpObj->pInventory[7].IsItem() == 0)
						{
							return -1;
						}
					}
				}
				else if(source == 7) //OK
				{
					if( lpObj->pInventory[8].m_Type != ITEMGET(13,3) &&
						//season4 add-on
						lpObj->pInventory[8].m_Type != ITEMGET(13,37) &&
						lpObj->pInventory[8].m_Type != ITEMGET(13,4))
					{
						return -1;
					}
				}
			}

			break;
		case 2:
			if(source < 0 || source > (WAREHOUSE_SIZE-1))	//OK
			{
				LogAdd("error-L1 : %s %d",
					__FILE__,
					__LINE__);
				return -1;
			}

			if(lpObj->pWarehouse[source].IsItem() == 0)
			{
				LogAdd(lMsg.Get(532),
					lpObj->AccountID,
					lpObj->Name,
					__LINE__);
				return -1;
			}

			sitem = &lpObj->pWarehouse[source];
			break;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10: //season3 add-on (Cherry Blossom Spirit)
		case 11: //season4 add-on
		case 12: //season4 add-on
		case 13: //season4 add-on
		case 14: //season4 add-on
		case 15: //6.2
		case 16: //6.2
			if(source < 0 || source > (CHAOS_BOX_SIZE -1))
			{
				LogAdd("error-L1 : %s %d",
					__FILE__,
					__LINE__);
				return -1;
			}

			if(lpObj->pChaosBox[source].IsItem() == 0)
			{
				LogAdd(lMsg.Get(532),lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pChaosBox[source];
			break;
		case 4:
			if(gObj[aIndex].m_bPShopOpen == 1)
			{
				return -1;
			}

			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);

			if(gObj[aIndex].m_bPShopTransaction == 1)
			{
				LogAddTD("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",
					gObj[aIndex].AccountID,
					gObj[aIndex].Name);
				LeaveCriticalSection(&lpObj->m_critPShopTrade);
				return -1;
			}

			lpObj->m_bPShopTransaction = 1;
			bPersonalShopTrans = 1;
			bSourceIsPShop = 1;

			LeaveCriticalSection(&lpObj->m_critPShopTrade);

			if(source < MAIN_INVENTORY_SIZE || source > (INVENTORY_SIZE - 1))	//OK
			{
				LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->pInventory[source].IsItem() == 0)
			{
				LogAdd(lMsg.Get(532),
					lpObj->AccountID,
					lpObj->Name,
					__LINE__);
				return -1;
			}

			sitem = &lpObj->pInventory[source];

			break;
		default: return -1;
		}
		//end sw1

		BYTE level;
		BYTE op1;
		BYTE op2;
		BYTE op3;
		BYTE dur;
		int type;

		BufferItemtoConvert3(siteminfo,type,level,op1,op2,op3,dur);

		if(sitem->m_Type != type || 
			sitem->m_Level != level || 
			sitem->m_Option1 != op1 || 
			sitem->m_Option2 != op2 || 
			sitem->m_Option3 != op3)
		{
			LogAddC(2,lMsg.Get(533),
				type,
				level,
				op1,
				op2,
				op3,
				sitem->m_Type,
				sitem->m_Level,
				sitem->m_Option1,
				sitem->m_Option2,
				sitem->m_Option3);
			return -1;
		}

		//sw2
		switch(tFlag)
		{
		case 0:
			if(target < 0 || target > (MAIN_INVENTORY_SIZE - 1))
			{
				LogAdd("error-L1 : %s %d",
					__FILE__,
					__LINE__);
				return -1;
			}
			titem = &lpObj->pInventory[target];
			break;
		case 2:
			if(target < 0 || target > (WAREHOUSE_SIZE - 1))
			{
				LogAdd("error-L1 : %s %d",
					__FILE__,
					__LINE__);
				return -1;
			}
			titem = &lpObj->pWarehouse[target];
			break;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10: //season3 add-on (Cherry Blossom Spirit)
		case 11: //season4 add-on
		case 12: //season4 add-on
		case 13: //season4 add-on
		case 14: //season4 add-on
		case 15: //6.2
		case 16: //6.2
			if(target < 0 || target > (CHAOS_BOX_SIZE -1))
			{
				LogAdd("error-L1 : %s %d",
					__FILE__,
					__LINE__);
				return -1;
			}

			titem = &lpObj->pChaosBox[target];

			if(sFlag == 0)
			{
				if(tFlag == 5)
				{
					// ----
					if(sitem->m_Type == ITEMGET(12,15)	||
						sitem->m_Type == ITEMGET(14,13) ||
						sitem->m_Type == ITEMGET(14,14) ||
						sitem->m_Type == ITEMGET(14,22) ||
						sitem->m_Type == ITEMGET(13,31) ||
						sitem->m_Type == ITEMGET(14,53) ||
						//Season4.5 add-on
						sitem->m_Type == ITEMGET(13,83) || 
						sitem->m_Type == ITEMGET(13,84) ||
						sitem->m_Type == ITEMGET(13,85) ||
						sitem->m_Type == ITEMGET(13,86) ||
						sitem->m_Type == ITEMGET(13,87) ||
						sitem->m_Type == ITEMGET(13,88) ||
						sitem->m_Type == ITEMGET(13,89) ||
						sitem->m_Type == ITEMGET(13,90) ||
						sitem->m_Type == ITEMGET(13,91) ||
						sitem->m_Type == ITEMGET(13,92) ||
						//6.2
						sitem->m_Type == ITEMGET(14, 103) || 
						sitem->m_Type == ITEMGET(14, 104) ||
						sitem->m_Type == ITEMGET(14, 105) ||
						sitem->m_Type == ITEMGET(14, 106) ||
						sitem->m_Type == ITEMGET(14, 107) ||
						sitem->m_Type == ITEMGET(14, 108) ||
						sitem->m_Type == ITEMGET(14, 121) ||
						sitem->m_Type == ITEMGET(14, 122) ||
						sitem->m_Type == ITEMGET(14, 112) ||
						sitem->m_Type == ITEMGET(14, 113) )
					{
					}
					else
					{
						return -1;
					}
				}
				else if(tFlag == 6)
				{
					if(g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal(sitem->m_Type) == 0)
					{
						return -1;
					}
				}
				else if(tFlag == 7)
				{
					if(g_kJewelOfHarmonySystem.IsEnableToMakeSmeltingStoneItem(sitem) == 0)
					{
						GCServerMsgStringSend(lMsg.Get(3377),lpObj->m_Index,1);
						return -1;
					}
				}
				else if(tFlag == 8)
				{
					if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(sitem) == 0)
					{
						return -1;
					}
				}
				//season4 add-on start
				else if(tFlag == 11)
				{
				}
				else if(tFlag == 12)
				{
				}
				else if(tFlag == 13)
				{
				}
				else if(tFlag == 14)
				{
				}
				else if(tFlag == 15)//6.2
				{
				}
				else if(tFlag == 16)
				{
				}
				//season4 add-on end
				else if(tFlag == 9) //Chaos Card Master
				{
					if(sitem->m_Type == ITEMGET(14,54) || sitem->m_Type == ITEMGET(14,92) ||
						sitem->m_Type == ITEMGET(14,93) || sitem->m_Type == ITEMGET(14,95))
					{
						//LogAddTD();
					}
					else
					{
						return -1;
					}
				}
				else if(tFlag == 10) //Cherry Blossom Spirit
				{
					if(sitem->m_Type != ITEMGET(14,88) && sitem->m_Type != ITEMGET(14,89) && sitem->m_Type != ITEMGET(14,90))
					{
						return -1;
					}
				}
				else
				{
					if(sitem->m_Level < 4 && sitem->m_Option3*4 < 4)
					{
						if( sitem->m_Type == ITEMGET(12,15) ||//OK 
							sitem->m_Type == ITEMGET(14,13) || 
							sitem->m_Type == ITEMGET(14,14) || 
							sitem->m_Type == ITEMGET(14,22) || 
							sitem->m_Type == ITEMGET(13,14) )
						{
							
						}
						else if(sitem->m_Type >= ITEMGET(12,0) && sitem->m_Type <= ITEMGET(12,2)) //OK
						{

						}
						else if(sitem->m_Type == ITEMGET(12,41)) //OK
						{

						}
						else if(sitem->m_Type == ITEMGET(14,17) || sitem->m_Type == ITEMGET(14,18))//OK
						{

						}
						else if(sitem->m_Type == ITEMGET(13,2) && sitem->m_Durability == 255.0f)//OK
						{

						}
						else if(sitem->m_Type == ITEMGET(13,16) || sitem->m_Type == ITEMGET(13,17) )//OK
						{

						}
						else if(sitem->m_Type == ITEMGET(12,30) || sitem->m_Type == ITEMGET(12,31))//OK
						{

						}
						else if(sitem->m_Type == ITEMGET(14,31))//OK
						{

						}
						else if(sitem->m_Type == ITEMGET(12,26))//OK
						{

						}
						else if(
							sitem->m_Type == ITEMGET(13,32) ||//OK
							sitem->m_Type == ITEMGET(13,33) ||
							sitem->m_Type == ITEMGET(13,34) ||
							sitem->m_Type == ITEMGET(13,35) ||
							sitem->m_Type == ITEMGET(13,36) ||
							sitem->m_Type == ITEMGET(13,37) ||
							sitem->m_Type == ITEMGET(14,16))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,3) || sitem->m_Type == ITEMGET(14,38) || sitem->m_Type == ITEMGET(14,39))//OK
						{

						}
						else if(g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(sitem->m_Type) == 1)//OK
						{

						}
						else if(sitem->m_Type == ITEMGET(14,31))//OK
						{

						}
						else if(sitem->m_Type == ITEMGET(14,53))//OK
						{

						}
						else if(sitem->m_Type == ITEMGET(14,96))//OK
						{

						}
						else if(sitem->m_Type == ITEMGET(13,49) || sitem->m_Type == ITEMGET(13,50))//OK
						{
							
						}
						else if(g_MixSystem.IsMixPossibleItem(sitem->m_Type) == 1)//OK
						{

						}
						else if(
							sitem->m_Type == ITEMGET(13,83) || //OK
							sitem->m_Type == ITEMGET(13,84) ||
							sitem->m_Type == ITEMGET(13,85) ||
							sitem->m_Type == ITEMGET(13,86) ||
							sitem->m_Type == ITEMGET(13,87) ||
							sitem->m_Type == ITEMGET(13,88) ||
							sitem->m_Type == ITEMGET(13,89) ||
							sitem->m_Type == ITEMGET(13,90) ||
							sitem->m_Type == ITEMGET(13,91) ||
							sitem->m_Type == ITEMGET(13,92) )
						{

						}
						else if(
							sitem->m_Type == ITEMGET(14, 103) || //new
							sitem->m_Type == ITEMGET(14, 104) ||
							sitem->m_Type == ITEMGET(14, 105) ||
							sitem->m_Type == ITEMGET(14, 106) ||
							sitem->m_Type == ITEMGET(14, 107) ||
							sitem->m_Type == ITEMGET(14, 108) ||
							sitem->m_Type == ITEMGET(14, 121) ||
							sitem->m_Type == ITEMGET(14, 122) ||
							sitem->m_Type == ITEMGET(14, 112) ||
							sitem->m_Type == ITEMGET(14, 113) )
						{

						}
						else
						{
							return -1;
						}
					}
					else if(sitem->m_Type == ITEMGET(14,11))
					{
						return -1;
					}
				}
			}
			break;
		case 4:
			if(gDoPShopOpen == 0)
			{
				return -1;
			}

			if(gObj[aIndex].m_bPShopOpen == 1)
			{
				return -1;
			}
			
			if(sitem->m_Type == ITEMGET(14,11) && sitem->m_Level == 13 ) 
			{
				return -1;
			}

			if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(sitem) == 1)
			{
				GCServerMsgStringSend(lMsg.Get(3367),lpObj->m_Index,1);
				return -1;
			}

			if(IsCashItem(sitem->m_Type) == 1)
			{
				switch(sitem->m_Type) //season4.5 add-on
				{
				case ITEMGET(13,83):
				case ITEMGET(13,84):
				case ITEMGET(13,85):
				case ITEMGET(13,86):
				case ITEMGET(13,87):
				case ITEMGET(13,88):
				case ITEMGET(13,89):
				case ITEMGET(13,90):
				case ITEMGET(13,91):
				case ITEMGET(13,92):
					break;
				default:
					return -1;
				}
			}



			if(IsPremiumItem(sitem->m_Type) == 1) //season4.5 add-on
			{
				return -1;
			}

			if( g_LuckyItemTrade == false ) //Custom
			{
				if( g_LuckyItemManager.IsLuckyItemTicket(sitem->m_Type) || g_LuckyItemManager.IsLuckyItemEquipment(sitem->m_Type)
					|| sitem->m_Type == ITEMGET(14, 160) || sitem->m_Type == ITEMGET(14, 161) )
				{
					return -1;
				}
			}

			if(sitem->m_Type == ITEMGET(13,38))//OK
			{
				GCServerMsgStringSend(lMsg.Get(3390),lpObj->m_Index,1);
				return -1;
			}

			//if(sitem->m_Type == ITEMGET(13,39))//OK
			//{
			//	return -1;
			//}

#ifdef PCBANG
			if(g_PCBangPointSystem.GetItemIndex(sitem->m_Type) != FALSE) //season4.5 add-on
			{
				return -1;
			}
#endif

			if(sitem->m_Type == ITEMGET(13,20)) //OK
			{
				if(sitem->m_Level == 1 || sitem->m_Level == 2)
				{
					return -1;
				}
			}

			if(sitem->m_QuestItem) //OK
			{
				return -1;
			}

			if(lpObj->pInventory[source].m_Type == ITEMGET(13,51) ) //OK
			{
				if(lpObj->pInventory[source].m_Durability == 0.0f)
				{
					return -1;
				}
			}

			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);

			if(gObj[aIndex].m_bPShopTransaction == 1)//OK
			{
				if(bSourceIsPShop == 1)
				{
					lpObj->m_bPShopTransaction = 1;
					bPersonalShopTrans = 1;
				}
				else
				{
					LogAddTD("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",gObj[aIndex].AccountID,gObj[aIndex].Name);
					LeaveCriticalSection(&lpObj->m_critPShopTrade);
					return -1;
				}
			}
			else
			{
				lpObj->m_bPShopTransaction = 1;
				bPersonalShopTrans = 1;
			}

			LeaveCriticalSection(&lpObj->m_critPShopTrade);

			if(target < MAIN_INVENTORY_SIZE || target > (INVENTORY_SIZE - 1))
			{
				LogAdd("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			titem = &lpObj->pInventory[target];

			break;
		default: return -1;
		}
		//end sw2

		s_num = sitem->GetNumber();

		if(gObjCheckSerial0ItemList(sitem))
		{
			MsgOutput(lpObj->m_Index,lMsg.Get(3354));
			LogAddTD("[ANTI-HACK][Serial 0 Item] [MoveItem] (%s)(%s) Item(%s) Pos(%d)",lpObj->AccountID,lpObj->Name,sitem->GetName(),source);
			return -1;
		}

		if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
		{
			return -1;
		}

		if(gObjWarehouseSearchSerialNumber(lpObj,s_num) == 0)
		{
			return -1;
		}

		if(titem->IsItem() == 1)
		{
			int max_count = 0;

			if(sitem->m_Type == ITEMGET(13,32) && titem->m_Durability < 20.0f)//OK
			{
				max_count = 20;
			}

			if(sitem->m_Type == ITEMGET(13,33) && titem->m_Durability < 20.0f)//OK
			{
				max_count = 20;
			}

			if(sitem->m_Type == ITEMGET(13,34) && titem->m_Durability < 10.0f)//OK
			{
				max_count = 10;
			}

			if(sitem->m_Type >= ITEMGET(14,35) || sitem->m_Type >= ITEMGET(14,36) || sitem->m_Type >= ITEMGET(14,37))//OK?
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}
			if(sitem->m_Type == ITEMGET(14,63) || sitem->m_Type == ITEMGET(14,64) ) //OK
			{
				if(titem->m_Durability < 100.0f)
				{
					max_count = 100;
				}
			}
			if(sitem->m_Type >= ITEMGET(14,46) && sitem->m_Type <= ITEMGET(14,50))//OK
			{
				if(titem->m_Durability < 3.0f)
				{
					max_count = 3;
				}
			}

			if(sitem->m_Type == ITEMGET(14,29))//OK
			{
				if(sitem->m_Level == titem->m_Level)
				{
					max_count = 5;
				}
			}

			if(sitem->m_Type == ITEMGET(14,100)) //OK
			{
				if(sitem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}

			if(sitem->m_Type == ITEMGET(14,101)) //new
			{
				if(sitem->m_Durability < 5)
				{
					max_count = 5;
				}
			}

			if(sitem->m_Type == ITEMGET(14,7))//OK
			{
				if(titem->m_Durability < 250.0f)
				{
					max_count = 250;
				}
			}
			else if(sitem->m_Type >= ITEMGET(14,0) && sitem->m_Type <= ITEMGET(14,8) || sitem->m_Type >= ITEMGET(14,38) && sitem->m_Type <= ITEMGET(14,40))//OK
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}
			else if(sitem->m_Type == ITEMGET(4,15) || sitem->m_Type == ITEMGET(4,7))//OK
			{
				if(titem->m_Durability < 255.0f)
				{
					max_count = 255;
				}
			}
			else if((sitem->m_Type == ITEMGET(14,70) //OK
				|| sitem->m_Type == ITEMGET(14,71) 
				|| sitem->m_Type == ITEMGET(14,94) 
				|| sitem->m_Type == ITEMGET(14, 133)) //new
				&& titem->m_Durability < 50.0f)
			{
				max_count = 50;
			}
			else if( (sitem->m_Type == ITEMGET(14,88)) && titem->m_Durability < (float)g_MaxCherryBranchWhite ) //OK
			{
				max_count = g_MaxCherryBranchWhite;
			}
			else if( (sitem->m_Type == ITEMGET(14,89)) && titem->m_Durability < (float)g_MaxCherryBranchRed ) //OK
			{
				max_count = g_MaxCherryBranchRed;
			}
			else if( (sitem->m_Type == ITEMGET(14,90)) && titem->m_Durability < (float)g_MaxCherryBranchGold ) //OK
			{
				max_count = g_MaxCherryBranchGold;
			}
			else if((sitem->m_Type == ITEMGET(14,85)) && titem->m_Durability < 3.0f) //OK
			{
				max_count = 3;
			}
			else if((sitem->m_Type == ITEMGET(14,86)) && titem->m_Durability < 3.0f) //OK
			{
				max_count = 3;
			}
			else if((sitem->m_Type == ITEMGET(14,87)) && titem->m_Durability < 3.0f) //OK
			{
				max_count = 3;
			}
			else if((sitem->m_Type == ITEMGET(14, 110)) && titem->m_Level == sitem->m_Level ) //new
			{
				max_count = 5;
			}
			if(max_count != 0)
			{
				if(sFlag != 0)
				{
					return -1;
				}

				if(titem->m_Type == sitem->m_Type && titem->m_Level == sitem->m_Level)
				{
					int t_dur = titem->m_Durability;

					if(t_dur < max_count)
					{
						int dif_dur = max_count - int(titem->m_Durability);

						if(dif_dur > sitem->m_Durability)
						{
							dif_dur = sitem->m_Durability;
						}

						titem->m_Durability += dif_dur;
						sitem->m_Durability -= dif_dur;

						if(titem->m_Type == ITEMGET(14,29) && max_count <= titem->m_Durability)
						{
							titem->m_Durability -= max_count;

							if(titem->m_Durability == 0)
							{
								gObjInventoryItemSet(aIndex,target,255);
								gObj[aIndex].pInventory[target].Clear();
								GCInventoryItemDeleteSend(aIndex,target,1);
								durTsend = 0;
							}
							else
							{
								durTsend = 1;
							}

							ItemSerialCreateSend(aIndex,235,gObj[aIndex].X,gObj[aIndex].Y,ItemGetNumberMake(14,28),sitem->m_Level,0,0,0,0,aIndex,0,0);
						}

						if( titem->m_Type == ITEMGET(14, 110) && max_count <= titem->m_Durability ) //new
						{
							titem->m_Durability -= max_count;
							// ----
							if( titem->m_Durability == 0.0 )
							{
								gObjInventoryItemSet(aIndex,target,255);
								gObj[aIndex].pInventory[target].Clear();
								GCInventoryItemDeleteSend(aIndex,target,1);
								durTsend = 0;
							}
							else
							{
								durTsend = 1;
							}
							ItemSerialCreateSend(aIndex,235,gObj[aIndex].X,gObj[aIndex].Y, ITEMGET(14, 111), sitem->m_Level,0,0,0,0,aIndex,0,0);
						}

						if( titem->m_Type == ITEMGET(14, 101) && max_count <= titem->m_Durability ) //new
						{
							titem->m_Durability -= max_count;
							// ----
							if( titem->m_Durability == 0.0 )
							{
								gObjInventoryItemSet(aIndex,target,255);
								gObj[aIndex].pInventory[target].Clear();
								GCInventoryItemDeleteSend(aIndex,target,1);
								durTsend = 0;
							}
							else
							{
								durTsend = 1;
							}
							ItemSerialCreateSend(aIndex,235,gObj[aIndex].X,gObj[aIndex].Y, ITEMGET(14, 102), sitem->m_Level,0,0,0,0,aIndex,0,0);
						}

						if(sitem->m_Durability > 0)
						{
							durSsend = 1;

							if(sitem->m_Type != ITEMGET(14,29)) //Season 2.5 add-on (fix for symbol of kundun)
							{
								durTsend = 1;
							}
						}
						else
						{
							switch(sFlag)
							{
							case 0:
								gObjInventoryItemSet(lpObj->m_Index,source,255);
								sitem->Clear();
								GCInventoryItemDeleteSend(lpObj->m_Index,source,0);
								durTsend = 1;
							default:	return 0;
							}
						}
					}
				}
			}
			return -1;
		}
		//Season 2.5 add-on
		switch(tFlag)
		{
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				{
					if( lpObj->pInventory[source].m_Type == ITEMGET(13,42) )
					{
						return -1;
					}
					else if( lpObj->pInventory[source].m_Type == ITEMGET(14,64) )
					{
						return -1;
					}
				}
				break;
			case 0:
				{
					if( IT_MAP_RANGE( lpObj->MapNumber ) )
					{
						if( lpObj->pInventory[source].m_Type == ITEMGET(14,64) )
						{
							g_IllusionTempleEvent.SetInventoryPos(lpObj->MapNumber, aIndex, target);
						}
					}
				}
				break;
		}
		//

		switch(tFlag)
		{
		case 0:
		case 4:
			if(target < INVETORY_WEAR_SIZE)
			{
				if(lpObj->MapNumber == MAP_INDEX_ICARUS)
				{
					if(target == 10)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,10))
						{
							return -1;
						}
					}
					else if(target == 11)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,10))
						{
							return -1;
						}
					}
				}

				if(target < INVETORY_WEAR_SIZE)
				{
					if(lpObj->MapNumber == MAP_INDEX_ICARUS || lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
					{
						if(target == 10)
						{
							if(lpObj->pInventory[source].m_Type == ITEMGET(13,39) || lpObj->pInventory[source].m_Type == ITEMGET(13,40) || lpObj->pInventory[source].m_Type == ITEMGET(13,41) || //Second Edition + Season2.5 Santa Girl Addition
								lpObj->pInventory[source].m_Type == ITEMGET(13,10))
							{
								return -1;
							}
						}
						else if(target == 11)
						{
							if(lpObj->pInventory[source].m_Type == ITEMGET(13,39) || lpObj->pInventory[source].m_Type == ITEMGET(13,40) || lpObj->pInventory[source].m_Type == ITEMGET(13,41) || //Second Edition + Season2.5 Santa Girl Addition
								lpObj->pInventory[source].m_Type == ITEMGET(13,10))
							{
								return -1;
							}
						}
					}
					//Season 2.5 add-on (GameMaster Ring)
					if(target == 10)
					{
						if((lpObj->Authority&32) != 32)
						{
							if(lpObj->pInventory[source].m_Type == ITEMGET(13,42))
							{
								return -1;
							}
						}
					}
					else if(target == 11)
					{
						if((lpObj->Authority&32) != 32)
						{
							if(lpObj->pInventory[source].m_Type == ITEMGET(13,42))
							{
								return -1;
							}
						}
					}
					//
				}

				useClass = sitem->IsClass(lpObj->Class,lpObj->ChangeUP,lpObj->ThirdChangeUp);

				if(useClass == 0)
				{
					return -1;
				}

				if(gObjIsItemPut(lpObj,sitem,target) == 0)
				{
					return -1;
				}
			}
			else 
			{
				if(target >= MAIN_INVENTORY_SIZE)	//OK
				{
					if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
					{
						if(lpObj->pInventory[source].m_Level == 1 ||
							lpObj->pInventory[source].m_Level == 2 ||
							lpObj->pInventory[source].m_Level == 3 ||
							lpObj->pInventory[source].m_Level == 4 ||
							lpObj->pInventory[source].m_Level == 5)
						{
							return -1;
						}
					}	
				}

				w = (target - INVETORY_WEAR_SIZE)%8;
				h = (target - INVETORY_WEAR_SIZE)/8;
#ifdef EXPINV
				if( !InventoryExtentCheck(w, h, 8, MAX_INVENTORY_H2) )
#else
				if(ExtentCheck(w,h,8,MAX_INVENTORY_H2) == 0)
#endif
				{
					return -1;
				}

				sitem->GetSize((int &)iwidth,(int &)iheight);

				memcpy(TempInventoryMap,lpObj->pInventoryMap,MAX_INVENTORY_LEN);

				switch(sFlag)
				{
				case 0:
				case 4:
					if(source > (INVETORY_WEAR_SIZE - 1))
					{
						gObjInventoryItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
					}
					break;
				default: break;
				}
			
				if(*(BYTE*)(gObj[aIndex].pInventoryMap + h * 8 + w) != 255)
				{
					memcpy(lpObj->pInventoryMap,TempInventoryMap,MAX_INVENTORY_LEN);
					return -1;
				}

				blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

				if(blank >= 254)
				{
					memcpy(lpObj->pInventoryMap,TempInventoryMap,MAX_INVENTORY_LEN);
					return -1;
				}
			}

			s_num = sitem->GetNumber();
			titem = sitem;

			switch(sFlag)
			{
			case 0:
			case 4:
				lpObj->pInventory[target] = lpObj->pInventory[source];
				gObjInventoryDeleteItem(aIndex, source);
				// ----
				gObjSetInventoryEquipment(lpObj, target);	//1.01.00 update
				// ----
				if( target > 12 && g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[target].m_Type) )
				{
					CItem * lpItem = &lpObj->pInventory[target];
					g_LuckyItemManager.GDReqLuckyItemInsert(lpItem, lpObj->m_Index);
				}
				break;
			case 2:
				lpObj->pInventory[target] = lpObj->pWarehouse[source];
				gObjWarehouseDeleteItem(aIndex,source);
				break;
			case 3:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10: //Season3.5 Cherry Blossom
			case 11: //season4 add-on
			case 12: //season4 add-on
			case 13: //season4 add-on
			case 14: //season4 add-on
			case 15: //season4 add-on
			case 16: //season4 add-on
				lpObj->pInventory[target] = lpObj->pChaosBox[source];
				gObjChaosBoxDeleteItem(aIndex,source);
				break;
			default : break;
			}

			if(target > (INVETORY_WEAR_SIZE - 1))
			{
				gObjInventoryItemSet(aIndex,target,1);
			}

			if(target < INVETORY_WEAR_SIZE)
			{
				CItem * lpItem = &lpObj->pInventory[target];
				if(lpItem->m_Option1)
				{
					int s_pos = gObjWeaponMagicAdd(&gObj[aIndex],lpItem->m_Special[0],lpItem->m_Level);
					if(s_pos >= 0)
					{
						GCMagicListOneSend(aIndex,s_pos,lpItem->m_Special[0],lpItem->m_Level,0,0);
					}
				}
			}
		break;
		case 2:
			BYTE res_1;

				switch(sFlag)
				{
				case 0:
					if(lpObj->pInventory[source].m_Type == ITEMGET(13,20))
					{
						if(lpObj->pInventory[source].m_Level == 0 ||
							lpObj->pInventory[source].m_Level == 1 || lpObj->pInventory[source].m_Level == 2) //Season 2.5 changed
						{
							return -1;
						}
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(14,11))
					{
						if(lpObj->pInventory[source].m_Level 
							== 15 // [MaxLevel->15] Season 5 Episode 2 JPN
							) 
						{
							return -1;
						}
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
					{
						if(lpObj->pInventory[source].m_Level == 1 ||
							lpObj->pInventory[source].m_Level == 2 ||
							lpObj->pInventory[source].m_Level == 3 ||
							lpObj->pInventory[source].m_Level == 4 ||
							lpObj->pInventory[source].m_Level == 5)
						{
							return -1;
						}
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
					{
						return -1;
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(13,42)) //Season 2.5 add-on
					{
						return -1;
					}

#ifdef PCBANG
					if(g_PCBangPointSystem.GetItemIndex(lpObj->pInventory[source].m_Type) != FALSE) //season 4.5 add-on
					{
						return -1;
					}
#endif

					if(lpObj->pInventory[source].m_QuestItem) //Season 2.5 add-on
					{
						return -1;
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(13,70)) //Season 4 add-on
					{
						if(lpObj->pInventory[source].m_Durability == 1.0f) //??
						{
							return -1;
						}
					}

#ifdef PERIOD
					if( lpObj->pInventory[source].IsPeriodItem() )
					{
						return -1;
					}
#endif

					if( g_LuckyItemTrade == false )
					{
						if( g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[source].m_Type) )
						{
							return -1;
						}
					}

					if( lpObj->pInventory[source].m_Type == ITEMGET(14, 163) )
					{
						return -1;
					}

					res_1 = gObjWarehouseInsertItemPos(aIndex,lpObj->pInventory[source],target,-1);
					break;
				case 2:
					res_1 = gObjWarehouseInsertItemPos(aIndex,lpObj->pWarehouse[source],target,source);
					break;
				default : break;
				}

				if(res_1 == 255)
				{
					return -1;
				}

				switch(sFlag)
				{
				case 0:
					lpObj->pWarehouse[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex,source);
					break;
				case 2:
					lpObj->pWarehouse[target] = lpObj->pWarehouse[source];
					gObjWarehouseDeleteItem(aIndex,source);
					break;
				default : break;
				}

				gObjWarehouseItemSet(aIndex,target,1);

				if(sFlag == 0)
				{
					if(source < INVETORY_WEAR_SIZE)
					{
						if(source == 10 || source == 11)
						{
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,10))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,40)) //Second Edition
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,41)) //Season 2.5 SantaGirl add-on
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,68)) //Season 4.0 XMas add-on
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,76)) //Season 4.6 Panda add-on
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13, 122)) // Season 5 Episode 2 JPN Skeleton Ring
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
						}

						gObjMakePreviewCharSet(aIndex);
						GCEquipmentChange(aIndex,source);
					}
				}
			return 2;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10: //Season3.5 Cherry Blossom
		case 11: //season4 add-on
		case 12: //season4 add-on
		case 13: //season4 add-on
		case 14: //season4 add-on
		case 15: //season4 add-on
		case 16: //season4 add-on
			BYTE res_2;
				switch(sFlag)
				{
				case 0:
					res_2 = gObjChaosBoxInsertItemPos(aIndex,lpObj->pInventory[source],target,-1);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10: //Season3.5 Cherry Blossom
				case 11: //season4 add-on
				case 12: //season4 add-on
				case 13: //season4 add-on
				case 14: //season4 add-on
				case 15: //season4 add-on
				case 16: //season4 add-on
					res_2 = gObjChaosBoxInsertItemPos(aIndex,lpObj->pChaosBox[source],target,source);
					break;
				default : break;
				}

				if(res_2 == 255)
				{
					return -1;
				}

				switch(sFlag)
				{
				case 0:
					lpObj->pChaosBox[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex,source);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10: //Season3.5 Cherry Blossom
				case 11: //season4 add-on
				case 12: //season4 add-on
				case 13: //season4 add-on
				case 14: //season4 add-on
				case 15: //season4 add-on
				case 16: //season4 add-on
					lpObj->pChaosBox[target] = lpObj->pChaosBox[source];
					gObjChaosBoxDeleteItem(aIndex,source);
					break;
				default : break;
				}

				gObjChaosItemSet(aIndex,target,1);

				if(sFlag == 0)
				{
					if(source < INVETORY_WEAR_SIZE)
					{
						if(source == 10 || source == 11)
						{
							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,10))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,39))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,40)) //Second Edition
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,41)) //Season 2.5 add-on
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
						}

						gObjMakePreviewCharSet(aIndex);
						GCEquipmentChange(aIndex,source);
					}
				}
			return tFlag;
		default: break;
		}

		if(sFlag == 0 && source < INVETORY_WEAR_SIZE)
		{
			if(lpObj->pInventory[source].IsItem() == 1)
			{
				if(lpObj->pInventory[source].m_Type == ITEMGET(13,10))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,40)) //Second Edition
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,41)) //Season 2.5 add-on
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}
			}
			else
			{
				if(source == 10 || source == 11)
				{
					if(lpObj->pInventory[target].m_Type == ITEMGET(13,10))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,39))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,40)) //Second Edition
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,41)) //Season 2.5 add-on
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,42)) //Season 2.5 add-on
					{
						gObjUseSkill.SkillChangeUse(aIndex);
						LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,68)) //Season 4 add-on
					{
						gObjUseSkill.SkillChangeUse(aIndex);
						LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,76)) //Season 4.6 add-on
					{
						gObjUseSkill.SkillChangeUse(aIndex);
						LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
					}
					if(lpObj->pInventory[target].m_Type == ITEMGET(13, 122)) // Season 5 Episode 2 JPN Skeleton Ring
					{
						gObjUseSkill.SkillChangeUse(aIndex);
						LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
					}
				}
			}

			gObjMakePreviewCharSet(aIndex);
			GCEquipmentChange(aIndex,source);
		}

		if(tFlag == 0 && target < INVETORY_WEAR_SIZE)
		{
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,10))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,39))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,40)) //Second Edition
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,41)) //Season 2.5 add-on
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,42)) //Season 2.5 add-on
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,68)) //Season 4 add-on
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}

			if(lpObj->pInventory[target].m_Type == ITEMGET(13,76)) //Season 4 add-on
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13, 122)) // Season 5 Episode 2 JPN Skeleton Ring
			{
				gObjUseSkill.SkillChangeUse(aIndex);
				LogAdd(lMsg.Get(534),gObj[aIndex].Name,lpObj->pInventory[target].m_Level);
			}
			gObjMakePreviewCharSet(aIndex);
			GCEquipmentChange(aIndex,target);
		}
	}
	__finally
	{
		if(bPersonalShopTrans == 1)
		{
			gObj[aIndex].m_bPShopTransaction = 0;
		}
	}
	return false;
}


//00581c70	-> Checked
BYTE gObjTradeRectCheck(int aIndex, int sx, int sy, int width, int height )
{
	int x;
	int y;
	int blank = 0;
	int xx;
	int yy;

	if ( ( sx + width) > 8 )
	{
		return -1;
	}

	if ( (sy + height) > 4 )
	{
		return -1;
	}

	for ( y=0;y<height;y++)
	{
		yy = sy + y;
		
		for ( x=0;x<width;x++)
		{
			xx = sx + x;

			if ( ::ExtentCheck(xx, yy, 8, 4) == TRUE )
			{
				if ( *(BYTE*)(gObj[aIndex].TradeMap + yy * 8 + xx) != (BYTE)-1)
				{
					blank++;
					return -1;
				}
			}
			else
			{
				LogAdd("error : %s %d", __FILE__ ,__LINE__);
				return -1;
			}
		}
	}

	if ( blank == 0 )
	{
		return (sy * 8 + sx);
	}

	return -1;
}

//00581de0	-> Checked
BOOL gObjTradeItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,4)==1)
			{
				*(BYTE*)(gObj[aIndex].TradeMap + yy*8+xx) = set_byte;
			}
			else
			{
				LogAdd("error : %s %d",__FILE__,__LINE__);
				return false;
			}
		}
	}
	return true;
}

//00581f10	-> Checked
BYTE gObjTradeInventoryMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int iwidth,iheight,s_num;
	BYTE itembuf[MAX_ITEM_INFO];

	if(source > TRADE_BOX_SIZE)
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(target > MAIN_INVENTORY_SIZE)	//204
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if(lpObj->Trade[source].IsItem() == 0)
	{
		return -1;
	}

	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

#ifdef __BEREZNUK__
	int ExcellentCount = 0;
	// ----
	for( int i = 0; i < 6; i++ )
	{
		if( (lpObj->pInventory[source].m_NewOption >> i) & 1 )
		{
			ExcellentCount++;
		}
	}
	// ----
	if( ExcellentCount >= 5 )
	{
		return -1;
	}
#endif

	if(gObjInventoryInsertItemPos(lpObj->m_Index,lpObj->Trade[source],target,1) == 255)
	{
		return -1;
	}

	s_num = 0;
	s_num = lpObj->Trade[source].m_Number;

	lpObj->Trade[source].GetSize((int &)iwidth,(int &)iheight);

	gObjTradeItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	lpObj->Trade[source].Clear();

	ItemByteConvert(itembuf,lpObj->Trade[source]);

	GCTradeOtherDel(lpObj->TargetNumber,source);

	if(gObjCheckSerial0ItemList(&lpObj->Trade[source]) != 0)
	{
		MsgOutput(lpObj->m_Index,lMsg.Get(3354));
		LogAddTD("[ANTI-HACK][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)",lpObj->AccountID,lpObj->Name,lpObj->Trade[source].GetName(),source);
		return -1;
	}

	if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
	{
		return -1;
	}

	if(target < INVETORY_WEAR_SIZE)
	{
		if(lpObj->pInventory[target].IsItem() == 1)
		{
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,10))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,39))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,40)) //Second Edition
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,41)) //Season 2.5 add-on
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				LogAdd(lMsg.Get(534),lpObj->Name,lpObj->pInventory[target].m_Level);
			}
		}

		gObjMakePreviewCharSet(lpObj->m_Index);
		GCEquipmentChange(lpObj->m_Index,target);
	}

	return false;
}

//00582710	-> Checked
BYTE gObjInventoryTradeMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int h,w,iwidth,iheight,s_num,blank;

	if(source > MAIN_INVENTORY_SIZE)	//204
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return -1;
	}

	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

#ifdef __BEREZNUK__
	int ExcellentCount = 0;
	// ----
	for( int i = 0; i < 6; i++ )
	{
		if( (lpObj->pInventory[source].m_NewOption >> i) & 1 )
		{
			ExcellentCount++;
		}
	}
	// ----
	if( ExcellentCount >= 5 )
	{
		return -1;
	}
#endif
	
	if( g_LuckyItemTrade == false )
	{
		if( g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[source].m_Type) )
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,20)) //Season 2.5 add-on
	{
		if(lpObj->pInventory[source].m_Level == 1)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,20))
	{
		if(lpObj->pInventory[source].m_Level == 2)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(14,11))
	{
		if(lpObj->pInventory[source].m_Level == 13) 
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,42)) //Season 2.5 add-on
	{
		return -1;
	}

	if((lpObj->Authority&32) != 32) //Season 2.5 add-on
	{
		if(lpObj->pInventory[source].m_Type == ITEMGET(14,52))
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,51)) //Season 2.5 add-on (Illusion Temple Ticket)
	{
		if(lpObj->pInventory[source].m_Durability == 0.0f)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(14,64)) //Season 2.5 add-on (Illusion Temple Ball)
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
	{
		if(lpObj->pInventory[source].m_Level == 1 ||
			lpObj->pInventory[source].m_Level == 2 ||
			lpObj->pInventory[source].m_Level == 3 ||
			lpObj->pInventory[source].m_Level == 4 ||
			lpObj->pInventory[source].m_Level == 5)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,38))
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
	{
		return -1;
	}

#ifdef PERIOD
	if( lpObj->pInventory[source].IsPeriodItem() )
	{
		return -1;
	}
#endif

	if(IsCashItem(lpObj->pInventory[source].m_Type) == 1)
	{
		switch(lpObj->pInventory[source].m_Type) //season 4.5 add-on
		{
		case ITEMGET(13,83):
			break;
		case ITEMGET(13,84):
			break;
		case ITEMGET(13,85):
			break;
		case ITEMGET(13,86):
			break;
		case ITEMGET(13,87):
			break;
		case ITEMGET(13,88):
			break;
		case ITEMGET(13,89):
			break;
		case ITEMGET(13,90):
			break;
		case ITEMGET(13,91):
			break;
		case ITEMGET(13,92):
			break;
		default:
			return -1;
		}
	}

	if(IsPremiumItem(lpObj->pInventory[source].m_Type) == 1) //season4.5 add-on
	{
		return -1;
	}

#ifdef PCBANG
	if(g_PCBangPointSystem.GetItemIndex(lpObj->pInventory[source].m_Type) != FALSE) //season4.5 add-on
	{
		return -1;
	}
#endif

	if(lpObj->pInventory[source].m_QuestItem) //Season 2.5 add-on
	{
		return -1;
	}

	lpObj->pInventory[source].GetSize((int &)iwidth,(int &)iheight);
	s_num = lpObj->pInventory[source].GetNumber();

	if(gObjCheckSerial0ItemList(&lpObj->pInventory[source]) != 0)
	{
		MsgOutput(lpObj->m_Index,lMsg.Get(3354));
		LogAddTD("[ANTI-HACK][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].GetName(),source);
		return -1;
	}

	if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
	{
		return -1;
	}

	w = target % 8;
#ifdef EXPINV
	h = target >> 3;	
#else
	h = target / 8;
#endif

	if(ExtentCheck(w,h,8,4) == 0)
	{
		return -1;
	}

	if(*(BYTE*)(lpObj->TradeMap + h * 8 + w) == 255)
	{
		BYTE itembuf[MAX_ITEM_INFO];
		blank = gObjTradeRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank == 255)
		{
			return -1;
		}

		lpObj->Trade[blank] = lpObj->pInventory[source];
		gObjInventoryDeleteItem(lpObj->m_Index,source);
		gObjTradeItemBoxSet(lpObj->m_Index,blank,iwidth,iheight,lpObj->Trade[blank].m_Type);
		ItemByteConvert(itembuf,lpObj->Trade[blank]);
		GCTradeOtherAdd(lpObj->TargetNumber,blank,itembuf);

		if(source < INVETORY_WEAR_SIZE)
		{
			if(source == 10 || source == 11)
			{
				if( lpObj->Trade[blank].m_Type == ITEMGET(13,10) ||
					lpObj->Trade[blank].m_Type == ITEMGET(13,39) ||
					lpObj->Trade[blank].m_Type == ITEMGET(13,40) ||
					lpObj->Trade[blank].m_Type == ITEMGET(13,41))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
			}
			gObjMakePreviewCharSet(lpObj->m_Index);
			GCEquipmentChange(lpObj->m_Index,source);
		}
		return true;
	}
	return -1;
}

// ?®·???µ????­ ?®·???µ?·? ??µ?
BYTE gObjTradeTradeMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int h,w;
	int iwidth,iheight;
	BYTE TempTradeMap[TRADE_BOX_MAP_SIZE];				// ±????? ???©?? ????
	int blank;

	if(source > TRADE_BOX_SIZE)
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}
	if(source == target)
	{
		LogAdd("error : %s %d",__FILE__,__LINE__);
		return -1;
	}
	if(lpObj->Trade[source].IsItem() == 0)
	{
		return -1;
	}

	// ??±?·??? ???????? ????????°??
	if(lpObj->Trade[target].IsItem() == 1)
	{
		return -1;
	}

	// ??±?·??? °??? ???????? ????????°??
	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}

	// °?·?°? ????µ?°??? °?·????? ????¶??? ???????» ??±??? ????.
	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	lpObj->Trade[source].GetSize((int &)iwidth,(int &)iheight);

	memcpy(TempTradeMap,lpObj->TradeMap,TRADE_BOX_MAP_SIZE);
	// ?????? ??????.

	gObjTradeItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);

	// ?????????? ?®·???µ??? ??????????.
	// ?®·???µ??? ?§??°? ?????????? ???©????.

	w = target % 8;
	h = target / 8;

	if(ExtentCheck(w,h,8,4) == 0)
	{
		memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_MAP_SIZE);
		return -1;
	}

	// ?????????? ???©
	if(*(BYTE*)(lpObj->TradeMap + h * 8 + w) == 255)
	{
		blank = gObjTradeRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank == 255)
		{
			memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_MAP_SIZE);
			return -1;
		}
		else	// ???????®?? ???®°? ??????
		{
			lpObj->Trade[blank] = lpObj->Trade[source];

			// ???????®?? ?¤???? ??????.
			lpObj->Trade[source].Clear();
			GCTradeOtherDel(lpObj->TargetNumber,source);
			gObjTradeItemBoxSet(lpObj->m_Index,blank,iwidth,iheight,lpObj->Trade[blank].m_Type);
			BYTE itembuf[MAX_ITEM_INFO];
			ItemByteConvert(itembuf,lpObj->Trade[blank]);
			GCTradeOtherAdd(lpObj->TargetNumber,blank,itembuf);
			return 0x01;
		}
	}

	memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_MAP_SIZE);
	return -1;
}

void gObjTradeCancel(int aIndex)
{
	if(OBJMAX_RANGE(aIndex) == 0)
	{
		LogAdd("error : index error %s %d",__FILE__,__LINE__);
		return;
	}

	if(gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected != PLAYER_PLAYING)
	{
		LogAdd(lMsg.Get(535),gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}

	if(gObj[aIndex].m_IfState.use != 1)
	{
		LogAdd(lMsg.Get(536),gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}

	if(gObj[aIndex].m_IfState.type != 1)
	{
		LogAdd(lMsg.Get(537),gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}

	gObjInventoryRollback(aIndex);
	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObj[aIndex].m_IfState.state = 0; //season4.5 add-on
	gObjCharTradeClear(&gObj[aIndex]);
	GCMoneySend(aIndex,gObj[aIndex].Money);
	GCItemListSend(aIndex);
	GCEquipmentSend(aIndex);
#ifdef PERIOD
	g_PeriodItemEx.OnRequestPeriodItemList(&gObj[aIndex]);
#endif
	GCMagicListMultiSend(&gObj[aIndex],0);
	gObjUseSkill.SkillChangeUse(aIndex);

	if(gObj[aIndex].GuildNumber > 0)
	{
		GCGuildViewportNowPaint(aIndex,gObj[aIndex].GuildName,0,0);
		gObjNotifyUpdateUnionV1(&gObj[aIndex]);
		gObjNotifyUpdateUnionV2(&gObj[aIndex]);
	}

	LogAddTD(lMsg.Get(514),gObj[aIndex].AccountID,gObj[aIndex].Name);
}

//00583ae0	-> Checked
void gObjTempInventoryItemBoxSet(BYTE * TempMap, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE) % 8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE) / 8;

	for(int y = 0 ; y < yl ; y++)
	{
		for(int x = 0; x < xl; x++)
		{
			*(BYTE*)(TempMap + (itemposy +y) * 8 + (itemposx + x)) = set_byte;
		}
	}
}

//00583b90	-> Checked
BYTE gObjTempInventoryRectCheck(int aIndex, BYTE * TempMap, int sx, int sy, int width, int height)
{
	int x,y,blank = 0;

	if(sx + width > 8)
	{
		return -1;
	}
	if(sy + height > MAX_INVENTORY_H)	//24
	{
		return -2;
	}
	// ----
	bool bResult = CheckOutOfInventory(aIndex, sy, height);
	// ----
	if( !bResult )
	{
		if( bResult == -1 )
		{
			return -2;
		}
		// ----
		return -1;
	}
	// ----
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
#ifdef EXPINV
			if( InventoryExtentCheck(sx+x, sy+y, 8, MAX_INVENTORY_H) && *(BYTE*)(TempMap + (sy+y)*8 + (sx+x))!= 255 )
#else
			if(*(BYTE*)(TempMap + (sy+y)*8 + (sx+x))!= 255)
#endif
			{
				blank += 1;
				return -1;
			}
		}
	}
	if(blank == 0)
	{
		return sx+sy*8+INVETORY_WEAR_SIZE;
	}
	return -1;
}

//00583ce0	-> Checked
BYTE gObjTempInventoryInsertItem(LPOBJ lpObj, CItem item, BYTE * TempMap)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize(iwidth,iheight) == 0)
	{
		LogAdd(lMsg.Get(532),__FILE__,__LINE__,item.m_Type);
		return -1;
	}

#ifdef EXPINV
	for(h = 0; h < 4 * lpObj->ExpandedInventory + 8; h++ )
#else
	for(h = 0; h < 8; h++)
#endif
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(TempMap + h * 8 + w) == 255)
			{
				blank = gObjTempInventoryRectCheck(lpObj->m_Index, TempMap,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}
				if(blank != 255)
				{
					gObjTempInventoryItemBoxSet(TempMap,blank,iwidth,iheight,item.m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}


BOOL TradeItemInventoryPutTest(int aIndex)
{
	BYTE TempInventoryMap[MAX_INVENTORY_LEN];
	int number,result = 1;
	number = gObj[aIndex].TargetNumber;

	if(gObjIsConnected(number) == 0)
	{
		return false;
	}
	memcpy(TempInventoryMap,gObj[aIndex].pInventoryMap,MAX_INVENTORY_LEN);

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			if(gObjTempInventoryInsertItem(&gObj[aIndex],gObj[number].Trade[n],TempInventoryMap) == 255)
			{
				return false;
			}
			result = 1;
		}
	}
	if(result == 1)
	{
		return true;
	}
	return false;
}

BOOL TradeitemInventoryPut(int aIndex)
{
	int number;
	BYTE insert;
	char szItemName[50] = "Item";
	BYTE ExOption[8];

	number = gObj[aIndex].TargetNumber;

	if(gObjIsConnected(number) == 0)
	{
		return false;
	}

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			insert = gObjInventoryInsertItem(aIndex,gObj[number].Trade[n]);
			if(insert != 255)
			{
				strcpy(szItemName,gObj[number].Trade[n].GetName());
				ItemIsBufExOption(ExOption,&gObj[number].Trade[n]);
				LogAddTD(lMsg.Get(538),
					gObj[number].AccountID,gObj[number].Name,gObj[number].MapNumber,gObj[number].X,gObj[number].Y,
					gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
					szItemName,gObj[number].Trade[n].m_Number,gObj[number].Trade[n].m_Level,gObj[number].Trade[n].m_Option1,gObj[number].Trade[n].m_Option2,gObj[number].Trade[n].m_Option3,
					ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],gObj[number].Trade[n].m_SetOption,
					//season4 add-on
					gObj[number].Trade[n].m_SocketOption[0],
					gObj[number].Trade[n].m_SocketOption[1],
					gObj[number].Trade[n].m_SocketOption[2],
					gObj[number].Trade[n].m_SocketOption[3],
					gObj[number].Trade[n].m_SocketOption[4]);

			}
			else
			{
				LogAdd("error : ??°? ?«????????!!");
				return false;
			}
		}
	}
	return true;
}

void gObjTradeOkButton(int aIndex)
{
	BYTE result = 1;
	int number = gObj[aIndex].TargetNumber;

	if(number < 0)
	{
		return;
	}

	if(gObjIsConnected(number) == 0)
	{
		return;
	}

	if(gObj[number].TargetNumber != aIndex)
	{
		LogAdd(lMsg.Get(539),gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].TargetNumber,gObj[number].TargetNumber);
		return;
	}

	if(gObj[aIndex].TradeOk != 1 || gObj[number].TradeOk != 1)
	{
		return;
	}

	if(TradeItemInventoryPutTest(aIndex) == 1)
	{
		if(TradeItemInventoryPutTest(number) == 1)
		{
			result = 1;
		}
		else
		{
			result = 2;
		}
	}
	else
	{
		result = 2;
	}

	if(gObjCheckMaxZen(aIndex,gObj[number].TradeMoney) == 0)
	{
		result = 2;
	}

	if(gObjCheckMaxZen(number,gObj[aIndex].TradeMoney) == 0)
	{
		result = 2;
	}

	if(g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[number]) == 0 || g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[aIndex]) == 0)
	{
		result = 4;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		CGTradeResult(aIndex,result);
		CGTradeResult(number,result);
		return;
	}

	if(result == 1)
	{
		TradeitemInventoryPut(aIndex);
		TradeitemInventoryPut(number);
		gObjInventoryCommit(aIndex);
		gObjInventoryCommit(number);
		gObjMakePreviewCharSet(aIndex);
		gObjMakePreviewCharSet(number);

		gObj[aIndex].Money -= gObj[aIndex].TradeMoney;
		if(gObj[aIndex].Money < 0)
		{
			gObj[aIndex].Money = 0;
		}
		gObj[number].Money -= gObj[number].TradeMoney;
		if(gObj[number].Money < 0)
		{
			gObj[number].Money = 0;
		}

		gObj[aIndex].Money += gObj[number].TradeMoney;
		gObj[number].Money += gObj[aIndex].TradeMoney;

		LogAddTD(lMsg.Get(540),gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].AccountID,gObj[number].Name,gObj[aIndex].TradeMoney,gObj[number].TradeMoney);
		GJSetCharacterInfo(&gObj[aIndex],aIndex,0, 0);
		GJSetCharacterInfo(&gObj[number],number,0, 0);
	}
	else
	{
		result = 2;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		CGTradeResult(aIndex,result);
		CGTradeResult(number,result);
		return;
	}

	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObj[number].TargetNumber = -1;
	gObj[number].m_IfState.use = 0;
	gObj[aIndex].TradeOk = 0;
	gObj[number].TradeOk = 0;
	gObj[aIndex].TradeMoney = 0;
	gObj[number].TradeMoney = 0;
	//season4 add-on
	gObj[aIndex].m_IfState.state = 0;
	gObj[number].m_IfState.state = 0;

	GCMoneySend(aIndex,gObj[aIndex].Money);
	GCMoneySend(number,gObj[number].Money);
	CGTradeResult(aIndex,result);
	CGTradeResult(number,result);

	GCItemListSend(aIndex);
	GCItemListSend(number);

#ifdef PERIOD
	g_PeriodItemEx.OnRequestPeriodItemList(&gObj[aIndex]);
	g_PeriodItemEx.OnRequestPeriodItemList(&gObj[number]);
#endif

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		gObj[aIndex].Trade[n].Clear();
		gObj[number].Trade[n].Clear();
	}

	gObjNotifyUpdateUnionV1(&gObj[aIndex]);
	gObjNotifyUpdateUnionV2(&gObj[aIndex]);
}

BYTE LevelSmallConvert(int aIndex, int inventoryindex) // [MaxLevel->15] Season 5 Episode 2 JPN
{
	int level = gObj[aIndex].pInventory[inventoryindex].m_Level;
	if(level >= 15)
	{
		return 7;
	}

	if(level == 13  || level == 14)
	{
		return 6;
	}
	if(level >= 11 && level <=12)
	{
		return 5;
	}

	if(level >= 9 && level <= 10)
	{
		return 4;
	}

	if(level >= 7 && level <= 8)
	{
		return 3;
	}

	if(level >= 5 && level <= 6)
	{
		return 2;
	}

	if(level >= 3 && level <= 4)
	{
		return 1;
	}
	
	return 0;
}

BYTE LevelSmallConvert(int level) // [MaxLevel->15] Season 5 Episode 2 JPN
{
	if(level >= 15)
	{
		return 7;
	}

	if(level == 13  || level == 14)
	{
		return 6;
	}
	if(level >= 11 && level <=12)
	{
		return 5;
	}

	if(level >= 9 && level <= 10)
	{
		return 4;
	}

	if(level >= 7 && level <= 8)
	{
		return 3;
	}

	if(level >= 5 && level <= 6)
	{
		return 2;
	}

	if(level >= 3 && level <= 4)
	{
		return 1;
	}
	
	return 0;
}

void gObjAttackDamageCalc(int aIndex) 
{

}

/*void gObjMakePreviewCharSet(int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex ))
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE index;
	LPOBJ lpObj = &gObj[aIndex];
	memset(lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	lpObj->CharSet[CS_CLASS] = CS_SET_CLASS(lpObj->Class);
	lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP(lpObj->ChangeUP);
	lpObj->CharSet[CS_CLASS] |= CS_SET_3RD_CHANGEUP(lpObj->ThirdChangeUp); //Season 2.5 Third Type of Character

	// Set Class
	if ( lpObj->m_ActionNumber == 0x80 )
	{
		lpObj->CharSet[CS_CLASS] |= 2;
	}
	else if ( lpObj->m_ActionNumber == 0x81 )
	{
		lpObj->CharSet[CS_CLASS] |= 3;
	}
	else
	{
		lpObj->CharSet[CS_CLASS] = lpObj->CharSet[CS_CLASS];
	}

	// Set Righth HAnd Item
	if ( lpObj->pInventory[0].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON1_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= DBI_GET_TYPE(lpObj->pInventory[0].m_Type);
		lpObj->CharSet[CS_WEAPON1_TYPE] = DBI_GET_INDEX(lpObj->pInventory[0].m_Type);
	}

	// Set Left Hand
	if ( lpObj->pInventory[1].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON2_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= DBI_GET_TYPE(lpObj->pInventory[1].m_Type);
		lpObj->CharSet[CS_WEAPON2_TYPE] = DBI_GET_INDEX(lpObj->pInventory[1].m_Type);
	}

	// Set Helmet
	if ( lpObj->pInventory[2].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(-1);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(-1);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(-1);
	}
	else
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(lpObj->pInventory[2].m_Type);
	}

	// Set Armor
	if ( lpObj->pInventory[3].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(-1);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(-1);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(-1);
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(lpObj->pInventory[3].m_Type);
	}

	// Set Pants
	if ( lpObj->pInventory[4].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(-1);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(-1);
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(lpObj->pInventory[4].m_Type);
	}

	// Set Gloves
	if ( lpObj->pInventory[5].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(-1);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(-1);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(-1);
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(lpObj->pInventory[5].m_Type);
	}

	// Set Boots
	if ( lpObj->pInventory[6].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(-1);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(-1);
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(lpObj->pInventory[6].m_Type);
	}

	index = 0;

	// Set Part onf Wings
	if ( lpObj->pInventory[7].m_Type < ITEMGET(0,0) )
	{
		index |= CS_SET_WING1(-1);
	}
	else
	{
		index |= CS_SET_WING1(lpObj->pInventory[7].m_Type);
	}

	if ( lpObj->pInventory[8].m_Type != ITEMGET(13,4) )
	{
		if ( lpObj->pInventory[8].m_Type < ITEMGET(0,0) )
		{
			index |= CS_SET_HELPER(-1);
		}
		else if(lpObj->pInventory[8].m_Type != ITEMGET(13, 67) 
			&& lpObj->pInventory[8].m_Type != ITEMGET(13, 80) //Season 4.5 Addon
			&& lpObj->pInventory[8].m_Type != ITEMGET(13, 106) // Season 5 Episode 2 JPN
			&& lpObj->pInventory[8].m_Type != ITEMGET(13, 123) ) 
		{
			index |= CS_SET_HELPER(lpObj->pInventory[8].m_Type);
		}
	}
	else
	{
		index |= CS_SET_HELPER(-1);
	}

	lpObj->CharSet[5] |= index;

	int levelindex = CS_SET_SMALLLEVEL_RH(LevelSmallConvert(aIndex, 0));
	levelindex |= CS_SET_SMALLLEVEL_LH(LevelSmallConvert(aIndex, 1));
	levelindex |= CS_SET_SMALLLEVEL_HELMET(LevelSmallConvert(aIndex, 2));
	levelindex |= CS_SET_SMALLLEVEL_ARMOR(LevelSmallConvert(aIndex, 3));
	levelindex |= CS_SET_SMALLLEVEL_PANTS(LevelSmallConvert(aIndex, 4));
	levelindex |= CS_SET_SMALLLEVEL_GLOVES(LevelSmallConvert(aIndex, 5));
	levelindex |= CS_SET_SMALLLEVEL_BOOTS(LevelSmallConvert(aIndex, 6));

	lpObj->CharSet[6] = CS_SET_SMALLLEVEL1(levelindex);
	lpObj->CharSet[7] = CS_SET_SMALLLEVEL2(levelindex);
	lpObj->CharSet[8] = CS_SET_SMALLLEVEL3(levelindex);

	if ( (lpObj->pInventory[7].m_Type >= ITEMGET(12,3) && lpObj->pInventory[7].m_Type <= ITEMGET(12,6)) ||
		  lpObj->pInventory[7].m_Type == ITEMGET(13,30) ||
		  lpObj->pInventory[7].m_Type == ITEMGET(12,41) || //Season3 add-on
		  lpObj->pInventory[7].m_Type == ITEMGET(12,42) )
	{
		lpObj->CharSet[5] |= CS_SET_WING1(-1);
		
		if ( lpObj->pInventory[7].m_Type == ITEMGET(13,30) )
		{
			lpObj->CharSet[9] |= 5;
		}
		else if ( lpObj->pInventory[7].m_Type == ITEMGET(12,41) ) //Season3 add-on
		{
			lpObj->CharSet[9] |= 6;
		}
		else if ( lpObj->pInventory[7].m_Type == ITEMGET(12,42) ) //Season3 add-on
		{
			lpObj->CharSet[9] |= 7;
		}
		else
		{
			lpObj->CharSet[9] |= (lpObj->pInventory[7].m_Type - 2) & 0x07;
		}
	}

	// Set Excellent Items
	lpObj->CharSet[10] = 0;

	if ( lpObj->pInventory[2].IsExtItem() != FALSE )
		lpObj->CharSet[10] = (char)0x80;

	if ( lpObj->pInventory[3].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x40;

	if ( lpObj->pInventory[4].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x20;

	if ( lpObj->pInventory[5].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x10;

	if ( lpObj->pInventory[6].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x8;

	if ( lpObj->pInventory[0].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x04;

	if ( lpObj->pInventory[1].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x02;

	// Set Set Items
	lpObj->CharSet[11] = 0;

	if ( lpObj->pInventory[2].IsSetItem() != FALSE )
		lpObj->CharSet[11] = (char)0x80;

	if ( lpObj->pInventory[3].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x40;

	if ( lpObj->pInventory[4].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x20;

	if ( lpObj->pInventory[5].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x10;

	if ( lpObj->pInventory[6].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x8;

	if ( lpObj->pInventory[0].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x04;

	if ( lpObj->pInventory[1].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x02;

	gObjCalCharacter(aIndex);

	if ( lpObj->IsFullSetItem != false )
		lpObj->CharSet[11] |= 0x01;

	if ( (lpObj->pInventory[8].m_Type & 0x03) != 0 )
	{
		if ( lpObj->pInventory[8].m_Type > 0 )
		{
			lpObj->CharSet[10] |= 0x01;
		}
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,4) )	// Dark Horse
	{
		lpObj->CharSet[12] |= 0x01;
	}

	if ( lpObj->pInventory[8].m_Type == ITEMGET(13,37) )	// Fenrir
	{
		lpObj->CharSet[10] &= 0xFE;
		lpObj->CharSet[12] &= 0xFE;
		lpObj->CharSet[12] |= 0x04;
		lpObj->CharSet[16] = 0;
		lpObj->CharSet[17] = 0;

		if ( lpObj->pInventory[8].IsFenrirIncLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x01;
		}

		if ( lpObj->pInventory[8].IsFenrirDecLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x02;
		}

		if ( lpObj->pInventory[8].IsFenrirSpecial() != FALSE ) // Season 2.5 Golden Fenrir Display
		{
			lpObj->CharSet[17] |= 0x01;
		}
	}

	if ( (lpObj->pInventory[7].m_Type >= ITEMGET(12,36) && lpObj->pInventory[7].m_Type <= ITEMGET(12,40)) || // Season 2.5 Third Wings Display
		lpObj->pInventory[7].m_Type == ITEMGET(12,43) ) //Season3 add-on 
	{
		lpObj->CharSet[5] |= 0x0C;
		lpObj->CharSet[16] |= (((lpObj->pInventory[7].m_Type - 35) & 0x07) << 0x02);

		if(lpObj->pInventory[7].m_Type == ITEMGET(12,43))
		{
			lpObj->CharSet[16] |= 0x18;
		}
	}

#ifdef SEASON5DOT4_ENG
	if( lpObj->pInventory[7].m_Type >= ITEMGET(12, 130) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 134) )
	{
		switch(lpObj->pInventory[7].m_Type)
		{
			case ITEMGET(12,130):
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 32;
			break;
			// --
		case ITEMGET(12,131):
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 64;
			break;
			// --
		case ITEMGET(12,132):
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 96;
			break;
			// --
		case ITEMGET(12,133):
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 128;
			break;
			// --
		case ITEMGET(12,134):
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 160;
			break;
		}
	}
#endif

#ifdef NEWWINGS
	if( lpObj->pInventory[7].m_Type >= ITEMGET(12, 180) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 185) )
	{
		lpObj->CharSet[5]	|= CS_SET_WING1(3);
		lpObj->CharSet[17]	|= (lpObj->pInventory[7].m_Type - ITEMGET(12, 180) + 1) << 2;
	}
#endif

	switch(lpObj->pInventory[8].m_Type) //Season 3.5 CashShop Pet Display
	{
	case ITEMGET(13, 64): // Deamon
		lpObj->CharSet[16] |= 32;
		break;
	case ITEMGET(13, 65): // Guardian Angel
		lpObj->CharSet[16] |= 64;
		break;
	case ITEMGET(13, 67): // Rudolf Season 4.5 addon
		lpObj->CharSet[16] |= 128;
		break;
	case ITEMGET(13, 80): // Panda Season 4.6 addon
		lpObj->CharSet[16] |= 224;
		break;
	case ITEMGET(13, 123): // Skeleton
		lpObj->CharSet[16] |= 96;
		break;
	case ITEMGET(13, 106): // Unicorn Season 5 Episode 2 JPN
		lpObj->CharSet[16] |= 160;
		break;
	}
}*/

void gObjMakePreviewCharSet(int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex ))
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE index;
	LPOBJ lpObj = &gObj[aIndex];
	memset(lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	lpObj->CharSet[CS_CLASS] = CS_SET_CLASS(lpObj->Class);
	lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP(lpObj->ChangeUP);
	lpObj->CharSet[CS_CLASS] |= CS_SET_3RD_CHANGEUP(lpObj->ThirdChangeUp);


	// Set Class
	if ( lpObj->m_ActionNumber == 0x80 )
	{
		lpObj->CharSet[CS_CLASS] |= 2;
	}
	else if ( lpObj->m_ActionNumber == 0x81 )
	{
		lpObj->CharSet[CS_CLASS] |= 3;
	}
	else
	{
		lpObj->CharSet[CS_CLASS] = lpObj->CharSet[CS_CLASS];
	}

	// Set Right Hand Item
	if ( lpObj->pInventory[0].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON1_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= DBI_GET_TYPE(lpObj->pInventory[0].m_Type);
		lpObj->CharSet[CS_WEAPON1_TYPE] = DBI_GET_INDEX(lpObj->pInventory[0].m_Type);
	}

	// Set Left Hand Item
	if ( lpObj->pInventory[1].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON2_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= DBI_GET_TYPE(lpObj->pInventory[1].m_Type);
		lpObj->CharSet[CS_WEAPON2_TYPE] = DBI_GET_INDEX(lpObj->pInventory[1].m_Type);
	}

	// Set Helmet
	if ( lpObj->pInventory[2].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(-1);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(-1);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(-1);
	}
	else
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(lpObj->pInventory[2].m_Type);
	}

	// Set Armor
	if ( lpObj->pInventory[3].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(-1);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(-1);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(-1);
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(lpObj->pInventory[3].m_Type);
	}

	// Set Pants
	if ( lpObj->pInventory[4].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(-1);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(-1);
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(lpObj->pInventory[4].m_Type);
	}

	// Set Gloves
	if ( lpObj->pInventory[5].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(-1);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(-1);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(-1);
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(lpObj->pInventory[5].m_Type);
	}

	// Set Boots
	if ( lpObj->pInventory[6].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(-1);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(-1);
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(lpObj->pInventory[6].m_Type);
	}

	index = 0;

#define SLOTNEGATIVE -1

	// Set Wings
	switch (lpObj->pInventory[7].m_Type)
	{
		case ITEMGET(12,0): // Wings of Elf
			lpObj->CharSet[5] |= CS_SET_WING1(1);
			lpObj->CharSet[9] |= 1;
			break;
		case ITEMGET(12,1): // Wings of Heaven
			lpObj->CharSet[5] |= CS_SET_WING1(1);
			lpObj->CharSet[9] |= 2;
			break;
		case ITEMGET(12,2): // Wings of Satan
			lpObj->CharSet[5] |= CS_SET_WING1(1);
			lpObj->CharSet[9] |= 3;
			break;
		case ITEMGET(12,41): // Wings of Misery
			lpObj->CharSet[5] |= CS_SET_WING1(1);
			lpObj->CharSet[9] |= 4;
			break;
		case ITEMGET(12,3): // Muse Elf Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 1;
			break;
		case ITEMGET(12,4): // Soul Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 2;
			break;
		case ITEMGET(12,5): // Blade Knight Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 3;
			break;
		case ITEMGET(12,6): // Magic Gladiator Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 4;
			break;
		case ITEMGET(13,30): // Cape of Lord
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 5;
			break;
		case ITEMGET(12,42): // Bloody Summoner Wings
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 6;
			break;
		case ITEMGET(12,49): // Cloak of Warrior
			lpObj->CharSet[5] |= CS_SET_WING1(2);
			lpObj->CharSet[9] |= 7;
			break;
		case ITEMGET(12,36): // Blade Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 1;
			break;
		case ITEMGET(12,37): // Grand Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 2;
			break;
		case ITEMGET(12,38): // High Elf Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 3;
			break;
		case ITEMGET(12,39): // Duel Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 4;
			break;
		case ITEMGET(12,40): // Lord Emperor Cape
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 5;
			break;
		case ITEMGET(12,43): // Dimension Master Wings
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 6;
			break;
		case ITEMGET(12,50): // Fist Master Cloak
			lpObj->CharSet[5] |= CS_SET_WING1(-1);
			lpObj->CharSet[9] |= 7;
			break;
		case ITEMGET(12,130): // Small Cape of Lord
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 32;
			break;
		case ITEMGET(12,131): // Small Wings of Misery
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 64;
			break;
		case ITEMGET(12,132): // Small Wings of Elf
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 96;
			break;
		case ITEMGET(12,133): // Small Wings of Heaven
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 128;
			break;
		case ITEMGET(12,134): // Small Wings of Satan
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 160;
			break;
		case ITEMGET(12,135): // Small Cloak of Warrior
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			lpObj->CharSet[17] |= 192;
			break;
		default:
			{
#ifdef NEWWINGS
				if( IS_NEWWINGS(lpObj->pInventory[7].m_Type) )
				{
					lpObj->CharSet[5]	|= CS_SET_WING1(3);
					lpObj->CharSet[17]	|= (lpObj->pInventory[7].m_Type - ITEMGET(12, 180) + 1) << 2;
				}
				else
				{
					index |= 0;
				}
#else
				index |= 0;
#endif
			}
			break;
	}

	int levelindex = CS_SET_SMALLLEVEL_RH(LevelSmallConvert(aIndex, 0));
	levelindex |= CS_SET_SMALLLEVEL_LH(LevelSmallConvert(aIndex, 1));
	levelindex |= CS_SET_SMALLLEVEL_HELMET(LevelSmallConvert(aIndex, 2));
	levelindex |= CS_SET_SMALLLEVEL_ARMOR(LevelSmallConvert(aIndex, 3));
	levelindex |= CS_SET_SMALLLEVEL_PANTS(LevelSmallConvert(aIndex, 4));
	levelindex |= CS_SET_SMALLLEVEL_GLOVES(LevelSmallConvert(aIndex, 5));
	levelindex |= CS_SET_SMALLLEVEL_BOOTS(LevelSmallConvert(aIndex, 6));

	lpObj->CharSet[6] = CS_SET_SMALLLEVEL1(levelindex);
	lpObj->CharSet[7] = CS_SET_SMALLLEVEL2(levelindex);
	lpObj->CharSet[8] = CS_SET_SMALLLEVEL3(levelindex);


	// Set Excellent Items
	lpObj->CharSet[10] = 0;

	if ( lpObj->pInventory[2].IsExtItem() != FALSE )
		lpObj->CharSet[10] = (char)0x80;

	if ( lpObj->pInventory[3].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x40;

	if ( lpObj->pInventory[4].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x20;

	if ( lpObj->pInventory[5].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x10;

	if ( lpObj->pInventory[6].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x8;

	if ( lpObj->pInventory[0].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x04;

	if ( lpObj->pInventory[1].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x02;

	// Set Set Items
	lpObj->CharSet[11] = 0;

	if ( lpObj->pInventory[2].IsSetItem() != FALSE )
		lpObj->CharSet[11] = (char)0x80;

	if ( lpObj->pInventory[3].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x40;

	if ( lpObj->pInventory[4].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x20;

	if ( lpObj->pInventory[5].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x10;

	if ( lpObj->pInventory[6].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x8;

	if ( lpObj->pInventory[0].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x04;

	if ( lpObj->pInventory[1].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x02;

	gObjCalCharacter(aIndex);

	if ( lpObj->IsFullSetItem != false )
	{
		lpObj->CharSet[11] |= 0x01;
	}

	gObjCalcMLSkillItemOption(lpObj);

	// Pets Set
	switch ( lpObj->pInventory[8].m_Type )
	{
	case ITEMGET(13,0):
	case ITEMGET(13,1):
	case ITEMGET(13,2):
		index |= CS_SET_HELPER(lpObj->pInventory[8].m_Type);
		break;
	case ITEMGET(13,3): // Dinorant
		index |= CS_SET_HELPER(lpObj->pInventory[8].m_Type);
		lpObj->CharSet[10] |= 0x01;
		break;
	case ITEMGET(13,4): // Dark Horse
		index |= CS_SET_HELPER(-1);
		lpObj->CharSet[12] |= 0x01;
		break;
	case ITEMGET(13,80): // Panda Pet
		lpObj->CharSet[16] |= 224;
		break;
	case ITEMGET(13,106): // Unicorn Pet
		lpObj->CharSet[16] |= 0xA0;
		break;
	case ITEMGET(13,123): // Skeleton Pet
		//lpObj->CharSet[5] -= 3;
		lpObj->CharSet[16] |= 96;
		break;
	case ITEMGET(13,67): // Rudolf Pet
		//lpObj->CharSet[5] -= 3;
		lpObj->CharSet[16] |= 0x80;
		break;
	case ITEMGET(13,65): // Spirit of Guardian
		lpObj->CharSet[16] |= 0x40;
		break;
	case ITEMGET(13,64): // Demon Pet
		lpObj->CharSet[16] |= 0x20;
		break;
	case ITEMGET(13,37): // Fenrir
		index |= 3;
		lpObj->CharSet[10] &= 0xFE;
		lpObj->CharSet[12] &= 0xFE;
		lpObj->CharSet[12] |= 0x04;
		lpObj->CharSet[16] = 0;

		if ( lpObj->pInventory[8].IsFenrirIncLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x01; // Red Fenrir
		}

		if ( lpObj->pInventory[8].IsFenrirDecLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x02; // Blue Fenrir
		}

		if ( lpObj->pInventory[8].IsFenrirSpecial() != FALSE )
		{
			lpObj->CharSet[17] |= 0x01; // Illusion Fenrir
		}
		break;
	default:
		index |= CS_SET_HELPER(-1);
		break;
	}
	lpObj->CharSet[5] |= index;
}

void gObjViewportPaint(HWND hWnd, short aIndex)
{
	int n;
	HDC hdc;
	char szTemp[256];
	int count = 0;
	int count2 = 0;
	int count3 = 0;
	int playerc = 0;
	int totalplayer = 0;
#ifdef ZEONWINDOW_STAT
	int offtraders = 0;
#endif
#ifdef OFFEXP
	int offexps = 0;
#endif

	if ( !OBJMAX_RANGE(aIndex))
		return;

	hdc = GetDC(hWnd);

	for ( n=0;n<OBJMAX;n++)
	{
		if ( gObj[n].Live != FALSE )
		{
			if ( gObj[n].Connected != PLAYER_EMPTY )
			{
				if ( gCurPaintMapNumber == gObj[n].MapNumber )
				{
					if ( gObj[n].Type == OBJ_USER )
					{
						playerc++;
					}
				}
			}
		}

		if ( gObj[n].Type == OBJ_USER && gObj[n].Connected != PLAYER_EMPTY )
		{
			totalplayer++;
#ifdef ZEONWINDOW_STAT
			if ( gObj[n].IsOffTrade == 1 )
			{
				offtraders++;
			}
#endif
#ifdef OFFEXP
			if ( gObj[n].OffExp == true )
			{
				offexps++;
			}
#endif
		}
		else if ( gObj[n].Connected != PLAYER_EMPTY )
		{
			count++;
		}
	}				

	if ( gCurPaintType == 1 )
	{
		RECT rect;
		int iStartX = 100;
		int iStartY = 50;
		int iWidth	= 1;
		int iHeight	= 1;
		int iMagnifying = 3;
		int x;
		int y;

		hdc = GetDC(hWnd);

		HBRUSH hCharacterBrush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH hMonsterBrush = CreateSolidBrush(RGB(128, 128, 128));
		HBRUSH hNpcBrush = CreateSolidBrush(RGB(0, 255, 255));
		HBRUSH hItemBrush = CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH hCrywolfMovePath = CreateSolidBrush(RGB(0, 255, 0));
		int iOldBkMode = SetBkMode(hdc,TRANSPARENT);
		
		// Set a Gray backgraound on non-walkable areas
		if ( MapC[gCurPaintMapNumber].m_attrbuf != NULL )
		{
			for ( y=0;y<255;y++)
			{
				for (x=0;x<255;x++)
				{
					if ( (MapC[gCurPaintMapNumber].m_attrbuf[y*256+x] & 4) ==4 || (MapC[gCurPaintMapNumber].m_attrbuf[y*256+x]&8) == 8)
					{
						rect.left = iStartX+y*iWidth*iMagnifying;
						rect.right = (iStartX+y*iWidth*iMagnifying) + (iWidth*iMagnifying);
						rect.top = iStartY+x*iHeight*iMagnifying;
						rect.bottom = (iStartY+x*iHeight*iMagnifying) + (iHeight*iMagnifying);

						FillRect(hdc, &rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
					}
				}
			}
		}

		if ( TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].m_bDataLoad != FALSE )
		{
			TMonsterAIMovePath & MovePath = TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE];

			for ( int i=0;i<MovePath.m_iMovePathSpotCount;i++)
			{
				rect.left = iStartX+MovePath.m_MovePathInfo[i].m_iPathY*iWidth*iMagnifying;
				rect.right = (iStartX+MovePath.m_MovePathInfo[i].m_iPathY*iWidth*iMagnifying) + (iWidth*iMagnifying);
				rect.top = iStartY+MovePath.m_MovePathInfo[i].m_iPathX*iHeight*iMagnifying;
				rect.bottom = (iStartY+MovePath.m_MovePathInfo[i].m_iPathX*iHeight*iMagnifying) + (iHeight*iMagnifying);

				FillRect(hdc, &rect, hCrywolfMovePath);

				CString szDesc;
				szDesc.Format("(%d,%d)", MovePath.m_MovePathInfo[i].m_iPathX, MovePath.m_MovePathInfo[i].m_iPathY);
			}
		}

		for ( n=0;n<OBJMAX;n++)
		{
			if ( gObj[n].Live != FALSE )
			{
				if ( gObj[n].Connected != PLAYER_EMPTY )
				{
					if ( gCurPaintMapNumber == gObj[n].MapNumber )
					{
						if ( gObj[n].Type == OBJ_USER )
						{
							rect.left = iStartX+gObj[n].Y*iWidth*iMagnifying;
							rect.right = (iStartX+gObj[n].Y*iWidth*iMagnifying) + ((iWidth+2)*iMagnifying);
							rect.top = iStartY+gObj[n].X*iHeight*iMagnifying;
							rect.bottom = (iStartY+gObj[n].X*iHeight*iMagnifying) + ((iHeight+2)*iMagnifying);

							FillRect(hdc, &rect, hCharacterBrush);

							CString szName;
							szName.Format("%s (%d,%d)", gObj[n].Name, gObj[n].X, gObj[n].Y);
							TextOut(hdc, rect.left, rect.bottom, szName, szName.GetLength());
						}
						else if ( gObj[n].Type == OBJ_MONSTER )
						{
							rect.left = iStartX+gObj[n].Y*iWidth*iMagnifying;
							rect.right = (iStartX+gObj[n].Y*iWidth*iMagnifying) + ((iWidth)*iMagnifying);
							rect.top = iStartY+gObj[n].X*iHeight*iMagnifying;
							rect.bottom = (iStartY+gObj[n].X*iHeight*iMagnifying) + ((iHeight)*iMagnifying);

							FillRect(hdc, &rect, hMonsterBrush);

							if ( gObj[n].m_iCurrentAI != 0 )
							{
								if ( gObj[n].m_iGroupMemberGuid == 0 )
								{
									FillRect(hdc, &rect, hCharacterBrush);

									CString szDesc;
									szDesc.Format("G(%d,%d)", gObj[n].m_iGroupNumber, gObj[n].m_iGroupMemberGuid );
									TextOut(hdc, rect.left, rect.bottom, szDesc, szDesc.GetLength());
								}
							}
						}
						else if ( gObj[n].Type == OBJ_NPC )
						{
							rect.left = iStartX+gObj[n].Y*iWidth*iMagnifying;
							rect.right = (iStartX+gObj[n].Y*iWidth*iMagnifying) + ((iWidth)*iMagnifying);
							rect.top = iStartY+gObj[n].X*iHeight*iMagnifying;
							rect.bottom = (iStartY+gObj[n].X*iHeight*iMagnifying) + ((iHeight)*iMagnifying);

							FillRect(hdc, &rect, hNpcBrush);
						}
					}
				}
			}
		}

		for ( int n=0;n<MAX_MAPITEM;n++)
		{
			if ( MapC[gCurPaintMapNumber].m_cItem[n].IsItem() )
			{
				rect.left = iStartX+MapC[gCurPaintMapNumber].m_cItem[n].py*iWidth*iMagnifying;
				rect.right = (iStartX+MapC[gCurPaintMapNumber].m_cItem[n].py*iWidth*iMagnifying) + (iWidth*iMagnifying);
				rect.top = iStartY+MapC[gCurPaintMapNumber].m_cItem[n].px*iHeight*iMagnifying;
				rect.bottom = (iStartY+MapC[gCurPaintMapNumber].m_cItem[n].px*iHeight*iMagnifying) + (iHeight*iMagnifying);

				FillRect(hdc, &rect, hItemBrush);

				CString szDesc;
				szDesc.Format("%s (%d,%d)", MapC[gCurPaintMapNumber].m_cItem[n].GetName(),
					MapC[gCurPaintMapNumber].m_cItem[n].px, MapC[gCurPaintMapNumber].m_cItem[n].py);
			}
		}

		DeleteObject((HGDIOBJ)hCharacterBrush);
		DeleteObject((HGDIOBJ)hMonsterBrush);
		DeleteObject((HGDIOBJ)hNpcBrush);
		DeleteObject((HGDIOBJ)hItemBrush);
		DeleteObject((HGDIOBJ)hCrywolfMovePath);
		SetBkMode(hdc, iOldBkMode);
	}

	gObjTotalUser = totalplayer;

#ifndef ZEONWINDOW_STAT
	wsprintf(szTemp, "COUNT:%d  TotalPlayer : %d  Player(%d):%d VpCount:%d(%d/%d) : item count:%d max:%d",
		count, totalplayer, aIndex, playerc, gObj[aIndex].VPCount, count3, count2, gItemLoop, gServerMaxUser);
#else

#ifdef OFFEXP
	wsprintf(szTemp, "Monsters: [%d] :: Online: [%d/%d] :: OffTraders: [%d] :: OffExps: [%d] :: Items: [%d]",
		count, totalplayer, gServerMaxUser, offtraders, offexps, gItemLoop);
#else
	wsprintf(szTemp, "Monsters: [%d] :: Online: [%d/%d] :: OffTraders: [%d] :: Items: [%d]",
		count, totalplayer, gServerMaxUser, offtraders, gItemLoop);
#endif


#endif

#ifndef ZEONWINDOW_STAT
	if ( gXMasEvent )
		strcat(szTemp, ":StarOfXMas");

	if ( gFireCrackerEvent )
		strcat(szTemp, ":FireCracker");

	if ( gHeartOfLoveEvent )
		strcat(szTemp, ":HeartOfLove");

	if ( gMedalEvent )
		strcat(szTemp, ":MedalEvent");
#endif

#ifdef OFFEXP
	TextOut(hdc, GAMESERVER_WIDTH / 2 - 250, 0, szTemp, strlen(szTemp));
#else
	TextOut(hdc, GAMESERVER_WIDTH / 2 - 200, 0, szTemp, strlen(szTemp));
#endif
	
	ReleaseDC(hWnd, hdc);
}

static int FrustrumX[4];
static int FrustrumY[4];

//----------------------------------------------------------------------------
// x,y ??????·? Frustrum?» »???????.
void InitFrustrum()
{
	
	float CameraViewFar    = 2400.f;	// ?§?? ??
	float CameraViewNear   = CameraViewFar*0.19f;// ??·? ??
	float CameraViewTarget = CameraViewFar*0.53f;// ???????? ???? ?§??
	float WidthFar  = 1190.f;// ?§???? °?·? ??
	float WidthNear = 550.f;// ??·????? °?·? ??
		
	vec3_t p[4];
	Vector(-WidthFar ,CameraViewFar -CameraViewTarget,0.f,p[0]);
	Vector( WidthFar ,CameraViewFar -CameraViewTarget,0.f,p[1]);
	Vector( WidthNear,CameraViewNear-CameraViewTarget,0.f,p[2]);
	Vector(-WidthNear,CameraViewNear-CameraViewTarget,0.f,p[3]);
	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f,0.f,45.f,Angle);
	AngleMatrix(Angle,Matrix);
    vec3_t Frustrum[4];
	for(int i=0;i<4;i++)
	{
		VectorRotate(p[i],Matrix,Frustrum[i]);
		FrustrumX[i] = (int)(Frustrum[i][0]*0.01f);
		FrustrumY[i] = (int)(Frustrum[i][1]*0.01f);
	}
}

void CreateFrustrum(int x, int y, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for ( int i=0;i<4;i++)
	{
		lpObj->FrustrumX[i] = FrustrumX[i] + x;
		lpObj->FrustrumY[i] = FrustrumY[i] + y;
	}
}

bool TestFrustrum2(int x, int y, int aIndex)
{
	int j = 3;
	LPOBJ lpObj = &gObj[aIndex];

	for(int i = 0; i < 4;j = i,i++)
	{
		if((lpObj->FrustrumX[i]- x) * (lpObj->FrustrumY[j]-y) - (lpObj->FrustrumX[j]-x) * (lpObj->FrustrumY[i]-y) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

BOOL gObjCheckViewport(int aIndex, int x, int y)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(x < lpObj->X - 15  || x > lpObj->X + 15 || y < lpObj->Y - 15 || y > lpObj->Y + 15)
	{
		return false;
	}

	int j = 3;

	for(int i = 0; i < 4; j = i, i++)
	{
		int frustrum = (lpObj->FrustrumX[i]- x) * (lpObj->FrustrumY[j]-y) - (lpObj->FrustrumX[j]-x) * (lpObj->FrustrumY[i]-y);
		if(frustrum < 0)
		{
			return false;
		}
	}
	return true;
}

void gObjViewportClose(LPOBJ lpObj)
{
	int tObjNum,i;
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	if(lpObj->Connected == PLAYER_PLAYING)
	{
		for(int n = 0; n < MVL; n++)
		{
			if(lpObj->VpPlayer2[n].state != 0)
			{
				if(lpObj->VpPlayer2[n].type != 5)
				{
					tObjNum = lpObj->VpPlayer2[n].number;

					i = 0;

					while( true )
					{
						if(gObj[tObjNum].VpPlayer[i].number == lpObj->m_Index)
						{
							if(gObj[tObjNum].VpPlayer[i].type == lpObj->Type)
							{
								gObj[tObjNum].VpPlayer[i].state = 3;
								break;
							}
						}

						if(i < MVL-1)
						{
							i++;
						}
						else
						{
							break;
						}
					}

					i = 0;

					while( true )
					{
						if(gObj[tObjNum].VpPlayer2[i].number == lpObj->m_Index)
						{
							if(gObj[tObjNum].VpPlayer2[i].type == lpObj->Type)
							{
								gObj[tObjNum].VpPlayer2[i].state = 0;
								break;
							}
						}

						if(i < MVL-1)
						{
							i++;
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}
	gObjClearViewport(lpObj);
}

void gObjViewportListCreate(short aIndex)
{
	int result,n;
	LPOBJ lpObj;
	int mapnum;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	lpObj = &gObj[aIndex];

	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	if(lpObj->RegenOk > 0)
	{
		return;
	}

	mapnum = lpObj->MapNumber;
	gItemLoop2 = 0;

	if(lpObj->Type == OBJ_USER)
	{
		MapClass * lpMap = &MapC[mapnum];

		for(n = 0; n < MAX_MAPITEM; n++)
		{
			if(lpMap->m_cItem[n].live)
			{
				gItemLoop2++;

				if(lpMap->m_cItem[n].m_State == 1 || lpMap->m_cItem[n].m_State == 2)
				{
					if(gObjCheckViewport(aIndex,lpMap->m_cItem[n].px,lpMap->m_cItem[n].py) == 1)
					{
						result = ViewportAdd(aIndex,n,5);
					}
				}
			}
		}
	}

	if(aIndex == 0)
	{
		gItemLoopMax = gItemLoop2;
		gItemLoop = gItemLoop2;
		gCurPaintPlayer = aIndex;
	}

	int a = 1;
	LPOBJ lpTempObj;

	if(lpObj->Type == OBJ_USER)
	{
		for(n = 0; n < OBJMAX; n++)
		{
			lpTempObj = &gObj[n];

			if(lpTempObj->Connected == PLAYER_PLAYING && aIndex != n)
			{
				if(lpTempObj->m_State == 1 || lpTempObj->m_State == 2)
				{
					if(mapnum == lpTempObj->MapNumber)
					{
						if(gObjCheckViewport(aIndex,lpTempObj->X,lpTempObj->Y) == 1)
						{
							result = ViewportAdd(aIndex,n,lpTempObj->Type);
							result = ViewportAdd2(n,aIndex,gObj[aIndex].Type);
						}
					}
				}
			}
		}
	}
	else if( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC )
	{
		for(n = OBJ_MAXMONSTER; n < OBJMAX; n++)
		{
			lpTempObj = &gObj[n];

			if(lpTempObj->Connected == PLAYER_PLAYING && aIndex != n)
			{
				if(lpTempObj->m_State == 1 || lpTempObj->m_State == 2)
				{
					if(mapnum == lpTempObj->MapNumber)
					{
						if(gObjCheckViewport(aIndex,lpTempObj->X,lpTempObj->Y) == 1)
						{
							result = ViewportAdd(aIndex,n,gObj[n].Type);
							result = ViewportAdd2(n,aIndex,gObj[aIndex].Type);
						}
					}
				}
			}
		}
	}
}

int ViewportAdd(int aIndex, int aAddIndex, int aType)
{
	int blank = -1;
	int count = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state != 0)
		{
			if(lpObj->VpPlayer[n].number == aAddIndex)
			{
				if(lpObj->VpPlayer[n].type == aType)
				{
					return -1;
				}
			}
		}
		else
		{
			if(blank < 0)
			{
				blank = n;
			}
		}
	}

	if(blank < 0)
	{
		return -1;
	}


	lpObj->VpPlayer[blank].state = 1;
	lpObj->VpPlayer[blank].number = aAddIndex;
	lpObj->VpPlayer[blank].type = aType;
	lpObj->VPCount ++;

	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		if(aType == OBJ_USER)
		{
			if(lpObj->m_iCurrentAI != 0)
			{
				lpObj->m_Agro.SetAgro(aAddIndex, 1000);
			}
		}
	}
	return blank;
}

int ViewportAdd2(int aIndex, int aAddIndex, int aType)
{
	int blank = -1;
	int count = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer2[n].state != 0)
		{
			if(lpObj->VpPlayer2[n].number == aAddIndex)
			{
				if(lpObj->VpPlayer2[n].type == aType)
				{
					return -1;
				}
			}
		}
		else
		{
			if(blank < 0)
			{
				blank = n;
			}
		}
	}

	if(blank < 0)
	{
		return -1;
	}


	lpObj->VpPlayer2[blank].state = 1;
	lpObj->VpPlayer2[blank].number = aAddIndex;
	lpObj->VpPlayer2[blank].type = aType;
	lpObj->VPCount2 ++;

	return blank;
}

void ViewportDel(short aIndex, int aDelIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(gObjIsConnected(aDelIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			if(lpObj->VpPlayer[n].number == aDelIndex)
			{
				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount--;
			}
		}
	}
}

void Viewport2Del(short aIndex, int aDelIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(gObjIsConnected(aDelIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			if(lpObj->VpPlayer2[n].number == aDelIndex)
			{
				lpObj->VpPlayer2[n].state = 0;
				lpObj->VpPlayer2[n].number = -1;
				lpObj->VPCount2 --;
			}
		}
	}
}

void gObjViewportAllDel(short aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			Viewport2Del(lpObj->VpPlayer[n].number,aIndex);
		}

		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			ViewportDel(lpObj->VpPlayer2[n].number,aIndex);
		}
	}
}

void gObjViewportListDestroy(short aIndex)
{
	short tObjNum;
	int n;
	LPOBJ lpObj;
	int MVL;

	if(OBJMAX_RANGE(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].Connected < PLAYER_PLAYING)
	{
		return;
	}

	lpObj = &gObj[aIndex];
	MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(n = 0; n < MVL; n++)
	{
		int mapnum;
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			tObjNum = lpObj->VpPlayer[n].number;

			switch(lpObj->VpPlayer[n].type)
			{
			case 5:
				mapnum = lpObj->MapNumber;

				if(MapC[mapnum].m_cItem[tObjNum].IsItem() == 1)
				{
					if(MapC[mapnum].m_cItem[tObjNum].m_State == 8)
					{
						lpObj->VpPlayer[n].state = 3;
					}
					else
					{
						if(gObjCheckViewport(aIndex,MapC[mapnum].m_cItem[tObjNum].px,MapC[mapnum].m_cItem[tObjNum].py) == 0)
						{
							lpObj->VpPlayer[n].state = 3;
						}
					}
				}
				else
				{
					if(MapC[mapnum].m_cItem[tObjNum].live == 0 || MapC[mapnum].m_cItem[tObjNum].Give == 0)
					{
						lpObj->VpPlayer[n].state = 3;
					}
					else
					{
						lpObj->VpPlayer[n].state = 3;
					}
				}
				break;
			default : 
				if(gObj[tObjNum].m_State == 1)
				{
					if(gObj[tObjNum].Live == 0)
					{
						lpObj->VpPlayer[n].state = 3;
					}
				}

				if(gObj[tObjNum].Connected  == PLAYER_EMPTY ||
					gObj[tObjNum].m_State == 8 ||
					gObj[tObjNum].Teleport != 0 ||
					gObj[tObjNum].m_State == 32)
				{
					lpObj->VpPlayer[n].state = 3;
				}
				else
				{
					if(gObjCheckViewport(aIndex,gObj[tObjNum].X,gObj[tObjNum].Y) == 0)
					{
						lpObj->VpPlayer[n].state = 3;

						if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
						{
							if(gObj[tObjNum].Type == OBJ_USER)
							{
								if(lpObj->m_iCurrentAI != 0)
								{
									lpObj->m_Agro.DelAgro(tObjNum);
								}
							}
						}
					}
				}
				break;
			}
		}

		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			tObjNum = lpObj->VpPlayer2[n].number;

			if(gObj[tObjNum].Connected < PLAYER_PLAYING)
			{
				lpObj->VpPlayer2[n].state = 0;
				lpObj->VpPlayer2[n].number = -1;
				lpObj->VPCount2 --;
			}
			else
			{
				if(gObjCheckViewport(tObjNum,lpObj->X,lpObj->Y) == 0)
				{
					lpObj->VpPlayer2[n].state = 0;
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VPCount2 --;
				}
			}
		}
	}
}

struct PMSG_ANS_PSHOP_VIEWPORT_NOTIFY {
	struct PWMSG_HEAD2 h;
	BYTE btCount;
};

struct PMSG_PSHOP_VIEWPORT_NOTIFY {
  BYTE btNumberH;
  BYTE btNumberL;
  char szPShopText[36];
};

void PShop_ViewportListRegenarate(short aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return ;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->RegenOk > 0)
	{
		return;
	}

	if(lpObj->CloseCount > -1)
	{
		return;
	}

	BYTE cBUFFER[6000];
	int iVpOpenCount = 0;
	int iVpAddCount = 0;

	PMSG_ANS_PSHOP_VIEWPORT_NOTIFY * lpMsg = (PMSG_ANS_PSHOP_VIEWPORT_NOTIFY *)&cBUFFER;
	PMSG_PSHOP_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_PSHOP_VIEWPORT_NOTIFY *)&cBUFFER[6];

	int iPShopOpenIndex[MAX_VIEWPORT];

	if(lpObj->m_bPShopRedrawAbs != 0)
	{
		memset(lpObj->m_iVpPShopPlayer,0x00,sizeof(lpObj->m_iVpPShopPlayer));
		lpObj->m_wVpPShopPlayerCount = 0;
		lpObj->m_bPShopRedrawAbs = 0;
	}

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer[n].state == 2 && lpObj->VpPlayer[n].type == OBJ_USER && gObj[lpObj->VpPlayer[n].number].m_bPShopOpen == 1)
		{
			iPShopOpenIndex[iVpOpenCount] = lpObj->VpPlayer[n].number;
			iVpOpenCount++;

			if(lpObj->m_wVpPShopPlayerCount > 0 && lpObj->m_wVpPShopPlayerCount <= MAX_VIEWPORT)
			{
				int found = 0;

				for(int i = 0; i < lpObj->m_wVpPShopPlayerCount; i++)
				{
					if(lpObj->m_iVpPShopPlayer[i] == lpObj->VpPlayer[n].number)
					{
						found = 1;
						break;
					}
				}

				if(found == 0)
				{
					lpMsgBody[iVpAddCount].btNumberL = SET_NUMBERL(WORD(lpObj->VpPlayer[n].number));
					lpMsgBody[iVpAddCount].btNumberH = SET_NUMBERH(WORD(lpObj->VpPlayer[n].number));
					memcpy(lpMsgBody[iVpAddCount].szPShopText,gObj[lpObj->VpPlayer[n].number].m_szPShopText,sizeof(gObj[lpObj->VpPlayer[n].number].m_szPShopText));
					iVpAddCount++;
				}
			}
			else if(lpObj->m_wVpPShopPlayerCount == 0)
			{
				lpMsgBody[iVpAddCount].btNumberL = SET_NUMBERL(WORD(lpObj->VpPlayer[n].number));
				lpMsgBody[iVpAddCount].btNumberH = SET_NUMBERH(WORD(lpObj->VpPlayer[n].number));
				memcpy(lpMsgBody[iVpAddCount].szPShopText,gObj[lpObj->VpPlayer[n].number].m_szPShopText,sizeof(gObj[lpObj->VpPlayer[n].number].m_szPShopText));
				iVpAddCount++;
			}
			else
			{
				LogAddTD("[PShop] ERROR : lpObj->m_wVpPShopPlayerCount is OUT of BOUND : %d",lpObj->m_wVpPShopPlayerCount);
			}
		}
	}

	memset(lpObj->m_iVpPShopPlayer,0x00,sizeof(lpObj->m_iVpPShopPlayer));
	lpObj->m_wVpPShopPlayerCount = 0;

	if(iVpOpenCount > 0 && iVpOpenCount <= MAX_VIEWPORT)
	{
		for(int n = 0; n < iVpOpenCount; n++)
		{
			lpObj->m_iVpPShopPlayer[n] = iPShopOpenIndex[n];
		}
		lpObj->m_wVpPShopPlayerCount = iVpOpenCount;
	}
	else if(iVpOpenCount != 0)
	{
		LogAddTD("[PShop] ERROR : iVpOpenCount is OUT of BOUND: %d",iVpOpenCount);
	}

	if(iVpAddCount > 0 && iVpAddCount <= MAX_VIEWPORT)
	{
		lpMsg->btCount = iVpAddCount;
		PHeadSubSetW((LPBYTE)lpMsg,0x3F,0,sizeof(PMSG_ANS_PSHOP_VIEWPORT_NOTIFY)+sizeof(PMSG_PSHOP_VIEWPORT_NOTIFY)*iVpAddCount);
		DataSend(aIndex,(LPBYTE)lpMsg,((lpMsg->h.sizeL & 0xFF)& 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
	else if(iVpAddCount != 0)
	{
		LogAddTD("[PShop] ERROR : iVpAddCount is OUT of BOUND: %d",iVpAddCount);
	}

	if(gObj[aIndex].m_bPShopWantDeal == 1)
	{
		if(gObjIsConnected(gObj[aIndex].m_iPShopDealerIndex) != 0)
		{
			if(gObj[gObj[aIndex].m_iPShopDealerIndex].Type == OBJ_USER)
			{
				if(gObj[gObj[aIndex].m_iPShopDealerIndex].m_bPShopOpen == 0)
				{
					gObj[aIndex].m_bPShopWantDeal = 0;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

					CGPShopAnsDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
				}
				else if(gObj[gObj[aIndex].m_iPShopDealerIndex].CloseCount >= 0 || gObj[aIndex].CloseCount >= 0)
				{
					gObj[aIndex].m_bPShopWantDeal = 0;
					gObj[aIndex].m_iPShopDealerIndex = -1;
					memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

					CGPShopAnsDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
				}
				else if(gObj[gObj[aIndex].m_iPShopDealerIndex].m_bPShopItemChange == 1)
				{
					CGPShopAnsBuyList(aIndex,gObj[aIndex].m_iPShopDealerIndex,1,1);
				}
			}
			else
			{
				gObj[aIndex].m_bPShopWantDeal = 0;
				gObj[aIndex].m_iPShopDealerIndex = -1;
				memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

				CGPShopAnsDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
			}
		}
		else
		{
			gObj[aIndex].m_bPShopWantDeal = 0;
			gObj[aIndex].m_iPShopDealerIndex = -1;
			memset(gObj[aIndex].m_szPShopDealerName,0,sizeof(gObj[aIndex].m_szPShopDealerName));

			CGPShopAnsDealerClosedShop(aIndex,gObj[aIndex].m_iPShopDealerIndex);
		}
	}
}

bool PShop_CheckInventoryEmpty(short aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for(int i = MAIN_INVENTORY_SIZE; i < INVENTORY_SIZE; i++)
	{
		if(lpObj->Inventory1[i].IsItem() == 1)
		{
			return false;
		}
	}
	return true;
}

void gObjStateSetCreate(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	DWORD dwNowTick = GetTickCount();
	
	if(lpObj->DieRegen == 1)
	{
		if(dwNowTick - lpObj->RegenTime > lpObj->MaxRegenTime + 1000)
		{
			gObjClearBuffEffect(lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT);
			lpObj->DieRegen = 2;
			lpObj->m_State = 8;
		}
	}

	if(lpObj->Type == OBJ_USER)
	{
		if(lpObj->m_State == 8)
		{
			gObjReady4Relife(lpObj);
		}
		else
		{
			gObjCheckTimeOutValue(lpObj,(DWORD &)dwNowTick);
		}
	}

	if(lpObj->Teleport == 1)
	{
		if(lpObj->DieRegen != 0)
		{
			lpObj->Teleport = 0;
		}
		else if(GetTickCount() - lpObj->TeleportTime > 100)
		{
			lpObj->Teleport = 2;
		}
	}
	else if(lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;

		if(lpObj->Type == OBJ_USER)
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);
			GCTeleportSend(&gObj[aIndex],0,lpObj->MapNumber,lpObj->X,lpObj->Y,lpObj->Dir);
			gObjViewportListProtocolCreate(lpObj);

//#if(_GSCS==1)
			if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
			{
				if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					g_CastleSiege.NotifySelfCsJoinSide(aIndex);
					g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
				}
			}
//#endif

			lpObj->Teleport = 0;
		}
		else if(lpObj->Type == OBJ_NPC && lpObj->Class == 229)
		{
			int x = lpObj->X;
			int y = lpObj->Y;
			
			lpObj->X = x;
			lpObj->Y = y;

			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

			lpObj->m_State = 1;
			lpObj->PathStartEnd = 0;
		}
		else if( lpObj->Type == OBJ_NPC && lpObj->Class == 568 )	//1.01.00
		{
			int x		= lpObj->X;
			int y		= lpObj->Y;
			lpObj->X	= x;
			lpObj->Y	= y;
			lpObj->TX	= lpObj->X;
			lpObj->TY	= lpObj->Y;
			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);
			lpObj->m_State		= 1;
			lpObj->PathStartEnd = 0;
		}
		else if(lpObj->Type == OBJ_MONSTER)
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

			gObjViewportListCreate(lpObj->m_Index);
			gObjViewportListProtocolCreate(lpObj);

			lpObj->Teleport = 0;
			lpObj->PathStartEnd = 0;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
		}
		else 
		{
			int x,y;

			if(lpObj->m_RecallMon >= 0)
			{
				x = lpObj->X;
				y = lpObj->Y;
			}
			else
			{
				x = lpObj->X + rand()%6 - 3;
				y = lpObj->Y + rand()%6 - 3;
			}

			lpObj->X = x;
			lpObj->Y = y;

			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			if(lpObj->m_RecallMon >= 0)
			{
				gObjViewportListProtocolCreate(lpObj);
				lpObj->PathStartEnd = 0;
			}
		}
	}

	if(lpObj->RegenOk == 2 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_State = 1;
		lpObj->RegenOk = 3;
		lpObj->X = lpObj->RegenMapX;
		lpObj->Y = lpObj->RegenMapY;
		lpObj->MapNumber = lpObj->RegenMapNumber;

		lpObj->TX = lpObj->X;
		lpObj->TY = lpObj->Y;

		CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

		if(DragonEvent->GetState() > 0)
		{
			if(DragonEvent->GetMapNumber() == lpObj->MapNumber)
			{
				GCMapEventStateSend(lpObj->MapNumber,1,1);
			}
		}

//#if(_GSCS==1)
		if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			::GCAnsCsNotifyStart(lpObj->m_Index,CASTLESIEGECHECKSTATE(g_CastleSiege.GetCastleState()) );
			
			if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
				g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
			}
		}
//#endif

		gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);

		if(gTamaJJangEvent != 0 && gAppearTamaJJang != 0)
		{
			if(lpObj->MapNumber == 0)
			{
				GCMapEventStateSend(0,gAppearTamaJJang,2);
			}
		}
	}

	//season4.5 add-on
	LPOBJ lpObj2 = &gObj[aIndex];

	if(IsOKPCBangBenefitAll(lpObj2) != FALSE)
	{
		gObjCalCharacter(lpObj2->m_Index);
		GCReFillSend(lpObj2->m_Index, lpObj2->AddLife + lpObj2->MaxLife, 0xFE, 0, lpObj2->iMaxShield + lpObj2->iAddShield);
		GCManaSend(lpObj2->m_Index, lpObj2->AddMana + lpObj2->MaxMana, 0xFE, 0, lpObj2->MaxBP + lpObj2->AddBP);
	}
}

struct PMSG_CHARREGEN
{
	struct PBMSG_HEAD h;
	BYTE subcode;
	BYTE MapX;
	BYTE MapY;
	BYTE MapNumber;
	BYTE Dir;
	WORD Life;
	WORD Mana;
	WORD wShield;
	WORD BP;

	BYTE ExpHHH; //
	BYTE ExpHHL; //
	BYTE ExpHLH; //
	BYTE ExpHLL; //
	BYTE ExpLHH; //
	BYTE ExpLHL; //
	BYTE ExpLLH; //
	BYTE ExpLLL; //

	DWORD Money;
};

void gObjSetState()
{
	int n;
	LPOBJ lpObj;

	for(n = 0; n < OBJMAX; n++)
	{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->m_State == 1)
			{
				lpObj->m_State = 2;
				if(lpObj->RegenOk == 3)
				{
					lpObj->RegenOk = 0;
				}
			}
	
			if(lpObj->Teleport == 3)
			{
				lpObj->Teleport = 0;
			}
	
			if(lpObj->DieRegen == 2)
			{
				if(lpObj->m_bGMSummon == 1) //season4.5 add-on
				{
					gObjDel(lpObj->m_Index);
					continue;
				}

				int mapnum = lpObj->MapNumber;
				int x = lpObj->X;
				int y = lpObj->Y;

				//season4 add-on for Raise Talisman
				lpObj->m_wRaiseTalismanSaveX = lpObj->X;
				lpObj->m_wRaiseTalismanSaveY = lpObj->Y;
				lpObj->m_wRaiseTalismanSaveMap = lpObj->MapNumber;
	
				lpObj->m_bAttackerKilled = 0;
	
				if(DS_MAP_RANGE(lpObj->MapNumber))
				{
					if(lpObj->Type >= OBJ_MONSTER)
					{
						g_DevilSquare.gDevilSquareMonsterRegen(lpObj);
						continue;
					}
				}
	
//#if(_GSCS==0)
				if(lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS && lpObj->Type == OBJ_MONSTER)
				{
					continue;
				}
//#endif
	
				if(lpObj->m_iCurrentAI != 0 && lpObj->m_iRegenType != 0)
				{
					continue;
				}

#ifdef IMPERIAL
				if( lpObj->Class >= 504 && lpObj->Class <= 521 ||
						lpObj->Class >= 523 && lpObj->Class <= 528)
				{
						continue;
				}
#endif
	
//#if(_GSCS==1)
				if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
				{
					if(g_CrywolfSync.GetCrywolfState() == 3)
					{
						if(lpObj->Type >= OBJ_MONSTER)
						{
							continue;
						}
					}
				}
//#endif
				if(lpObj->m_bIsInMonsterHerd != 0)
				{
					if(lpObj->m_lpMonsterHerd !=0)
					{
						lpObj->m_lpMonsterHerd->MonsterRegenAction(lpObj);
					}
					continue;
				}	
				if(BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Type >= OBJ_MONSTER)
				{
					int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(lpObj->MapNumber);

					if(lpObj->Class == 131)
					{
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bCASTLE_DOOR_LIVE = 0;
						g_BloodCastle.m_BridgeData[lpObj->m_cBloodCastleIndex].m_bBC_DOOR_TERMINATE_COMPLETE = 1;
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = g_BloodCastle.GetCurrentLiveUserCount(iBridgeIndex) * 2;
	
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_KILL_COUNT = 0;
	
						if(g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT > 10)
						{
							g_BloodCastle.m_BridgeData[iBridgeIndex].m_iBC_BOSS_MONSTER_MAX_COUNT = 10;
						}
	
						g_BloodCastle.SendCastleDoorBlockInfo(lpObj->m_cBloodCastleIndex,0);
						g_BloodCastle.ReleaseCastleDoor(lpObj->m_cBloodCastleIndex);
						g_BloodCastle.SetCastleBlockInfo(lpObj->m_cBloodCastleIndex, BC_BLOCK_PLAYEND);
	
						if(g_BloodCastle.GetCurrentState(lpObj->m_cBloodCastleIndex) == 2)
						{
							g_BloodCastle.SetBossMonster(lpObj->m_cBloodCastleIndex);
						}
					}
			
					if(BC_STATUE_RANGE(lpObj->Class - 132) && BC_MAP_RANGE(lpObj->MapNumber))
					{
						int TopHitUser = gObjMonsterTopHitDamageUser(lpObj);
						int level = lpObj->Class - 132;
	
						int type = ItemGetNumberMake(13,19);

						int iMapNumber;

						switch(lpObj->MapNumber) //Webzen Stupid :D
						{
						case MAP_INDEX_BLOODCASTLE1:
							iMapNumber = 246;
							break;
						case MAP_INDEX_BLOODCASTLE2:
							iMapNumber = 247;
							break;
						case MAP_INDEX_BLOODCASTLE3:
							iMapNumber = 248;
							break;
						case MAP_INDEX_BLOODCASTLE4:
							iMapNumber = 249;
							break;
						case MAP_INDEX_BLOODCASTLE5:
							iMapNumber = 250;
							break;
						case MAP_INDEX_BLOODCASTLE6:
							iMapNumber = 251;
							break;
						case MAP_INDEX_BLOODCASTLE7:
							iMapNumber = 252;
							break;
						case MAP_INDEX_BLOODCASTLE8:
							iMapNumber = 253;
							break;
						}
	
						ItemSerialCreateSend(lpObj->m_Index,iMapNumber,lpObj->X,lpObj->Y,type,level,0,0,0,0,TopHitUser,0,0);
	
						g_BloodCastle.m_BridgeData[iBridgeIndex].m_btBC_QUEST_ITEM_NUMBER = level;
	
						LogAddTD("[Blood Castle] (%d) Ultimate Weapon Dropped -> (%s)",iBridgeIndex+1, (level !=0)?((level==1)?"Sword":"Crossbow"):"Staff");
					}

					if(lpObj->m_PosNum == -1)
					{
						gObjDel(lpObj->m_Index);
						continue;
					}

					if(lpObj->Type == OBJ_MONSTER)
					{
						if(lpObj->Class != 131 && BC_STATUE_RANGE(lpObj->Class - 132)== 0)
						{
							g_BloodCastle.ChangeMonsterState(iBridgeIndex,lpObj->m_Index);
						}
					}
				}

				if(IT_MAP_RANGE(lpObj->MapNumber) ) //Season2.5 add-on
				{
					if(lpObj->Class == 380 || lpObj->Class >= 386 && lpObj->Class <= 403 )
					{
						if(lpObj->m_PosNum == -1)
						{
							continue;
						}
					}
				}

#ifdef DP
				if( lpObj->Class == 541 || lpObj->Class == 542 )
				{
					g_DoppleganerEvent.DelTreasureBox(lpObj->m_Index, lpObj->Class);
					continue;
				}
#endif

				if(lpObj->m_Attribute == 60)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}

//#if(_GSCS==1)
				if(lpObj->m_Attribute == 61)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}
				
				if(lpObj->m_Attribute == 62) //HermeX Fix
				{
					switch(lpObj->Class)
					{
						case 300:// Hero Mobs
						case 301:
						case 302:
						case 303:
						{
							gObjDel(lpObj->m_Index);
							continue;
						}
					}
					if((lpObj->m_dwLastCheckTick + 600000) < GetTickCount()) //Dig Applied
					{
						gObjDel(lpObj->m_Index);
						continue;
					}
				}
				
				
//#endif

				if(lpObj->Type >= OBJ_MONSTER)
				{
					if(lpObj->Class == 287 || lpObj->Class == 286)
					{
						g_CsNPC_Mercenary.DeleteMercenary(lpObj->m_Index);
						gObjDel(lpObj->m_Index);
						continue;
					}
				}

//#if(_GSCS==1)
				if(lpObj->Type >= OBJ_MONSTER)
				{
					if(lpObj->Class == 278)
					{
						g_CsNPC_LifeStone.DeleteLifeStone(lpObj->m_Index);
						gObjDel(lpObj->m_Index);
						continue;
					}					
				}
//#endif

				int State = 1;

				if(lpObj->Type >= OBJ_MONSTER)
				{
					if(lpObj->Class == 44 && DragonEvent->GetState() == 0)
					{
						State = 0;
					}

					if(lpObj->Class == 53	||
						lpObj->Class == 54	||
						lpObj->Class == 55	||
						lpObj->Class == 56)
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}
	
					if(lpObj->Class >= 78 && lpObj->Class <= 83)
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}

					if(lpObj->Class >= 493 && lpObj->Class <= 502) //season 4.5 add-on
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}

#ifdef IMPERIAL
					if( lpObj->Class >= 504 && lpObj->Class <= 521 || lpObj->Class >= 523 && lpObj->Class <= 528 )
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}
#endif

					if(lpObj->Class == 77)
					{
						int i_shield = lpObj->m_SkyBossMonSheildLinkIndex;
	
						if(i_shield != -1)
						{	
							gObj[i_shield].Life = gObj[i_shield].MaxLife;
						}	
					}

					if( lpObj->Class == 560 || lpObj->Class == 561 )	//Medusa, 1.01.00
					{
						State = 0;
						lpObj->DieRegen = 0;
					}
				}

				PMSG_CHARREGEN pMsg;

				if(State != 0)
				{
					lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
					lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
					lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

					lpObj->Live = 1;

					gObjRemoveBuffEffect(lpObj, AT_POISON);
					gObjRemoveBuffEffect(lpObj, AT_ICE);
					gObjRemoveBuffEffect(lpObj, AT_ICE_ARROW);
					
					lpObj->m_ViewState = 0;
					lpObj->Teleport = 0;

					for(int i = 0; i < MAX_SELF_DEFENSE; i++)
					{
						lpObj->SelfDefenseTime[i] = 0;
					}

					gObjTimeCheckSelfDefense(lpObj);

					lpObj->MySelfDefenseTime = 0; //Season 2.5 add-on

					gObjClearBuffEffect(lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT);
					gObjClearViewport(lpObj);

					if(lpObj->Type == OBJ_USER)
					{
						if(g_iUseCharacterAutoRecuperationSystem != 0 && lpObj->Level <= g_iCharacterRecuperationMaxLevel)
						{
							lpObj->m_iAutoRecuperationTime = GetTickCount();
						}
						else
						{
							lpObj->m_iAutoRecuperationTime = 0;
						}

						lpObj->dwShieldAutoRefillTimer = GetTickCount();

						gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::ePetItem_Mode::PetItem_Mode_Normal,-1);

						if(lpObj->KillerType == 3)
						{
#ifdef NPVP
							if( !g_NewPVP.IsDuel(*lpObj) )
							{
								MapC[lpObj->MapNumber].GetRandomLengthPos((short &)lpObj->X,(short &)lpObj->Y,18);
							}
#else
							MapC[lpObj->MapNumber].GetRandomLengthPos((short &)lpObj->X,(short &)lpObj->Y,18);
#endif
						}
						else if(lpObj->KillerType == 2)
						{
							int w_Type = 0;

							if(lpObj->lpGuild != 0)
							{
								if(lpObj->lpGuild->WarType == 1)
								{
									w_Type = 1;
								}
							}

							if(w_Type == 0)
							{
								MapC[lpObj->MapNumber].GetRandomLengthPos((short &)lpObj->X,(short &)lpObj->Y,18);
							}
							else if(lpObj->lpGuild->WarState)
							{
								int BattleGroundIndex = lpObj->lpGuild->BattleGroundIndex;
								gBattleGetTeamPosition(BattleGroundIndex,lpObj->lpGuild->BattleTeamCode,(short &)lpObj->X,(short &)lpObj->Y);
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_RORENCIA || lpObj->MapNumber == MAP_INDEX_DUNGEON)
						{
							lpObj->MapNumber = MAP_INDEX_RORENCIA;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(DS_MAP_RANGE(lpObj->MapNumber))
						{
							g_DevilSquare.DieProcDevilSquare(lpObj);
							lpObj->MapNumber = MAP_INDEX_NORIA;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_ICARUS)
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(BC_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(CC_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(KALIMA_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
//#if(_GSCS==1) 
						else if(lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE)
						{
							int mgt = 106;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
							
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
						{
							int mgt;
							short x,y,level;
							
							if(!g_CsNPC_LifeStone.SetReSpawnUserXY(lpObj->m_Index))
							{
								mgt = 100;

								if(lpObj->m_btCsJoinSide == 1)
								{
									mgt = 105;
								}
								else
								{
									mgt = 100;
								}

								BYTE map = lpObj->MapNumber,dir;
								int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								
								if(result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
							}
						}
//#endif
						else if(lpObj->MapNumber == MAP_INDEX_AIDA)
						{
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
						{
//#if(_GSCS==0) # Need check
//							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
//#else
							if(g_Crywolf.GetCrywolfState() == 4)
							{
								int mgt = 118;
								short x,y,level;
								BYTE map = lpObj->MapNumber,dir;
								
								int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								
								if(result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
								else //1st JMP
								{
									MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);

								}
							}
							else // 2nd JMP
							{
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);

							}
//#endif
						}
//#if(_GSCS==0)
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
						{
							int mgt = 137;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
							
							g_KanturuBattleUserMng.DeleteUserData(lpObj->m_Index);
						}
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU1)
						{
							int mgt = 138;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
							
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU2)
						{
							int mgt = 139;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
							
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
//#endif
						//Season 2.5 add-on
						else if(lpObj->MapNumber == MAP_INDEX_3RD_CHANGEUP_QUEST || lpObj->MapNumber == MAP_INDEX_3RD_CHANGEUP_BOSS_QUEST)
						{
							int mgt = 258;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
								
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(IT_MAP_RANGE(lpObj->MapNumber))
						{
							BYTE loc48 = g_IllusionTempleEvent.GetUserTeam(lpObj->MapNumber, lpObj->m_Index);

							if(loc48 == (BYTE)-1)
							{
								lpObj->MapNumber = MAP_INDEX_DEVIAS;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
							}
							else if(loc48 == 0)
							{
								int mgt = lpObj->MapNumber + 103;
								short x,y,level;
								BYTE map = lpObj->MapNumber,dir;

								int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

								if(result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}

								g_IllusionTempleEvent.IllusionTempleUserDieRegen(lpObj); //Set Shield after Death					
							}
							else if(loc48 == 1)
							{
								int mgt = lpObj->MapNumber + 109;
								short x,y,level;
								BYTE map = lpObj->MapNumber,dir;

								int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

								if(result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
								g_IllusionTempleEvent.IllusionTempleUserDieRegen(lpObj); //Set Shield after Death
							}
							else
							{
								lpObj->MapNumber = MAP_INDEX_DEVIAS;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS)
						{
							int mgt = 273;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
								
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_RAKLION_FIELD || lpObj->MapNumber == MAP_INDEX_RAKLION_BOSS) //season4 add-on
						{
							int mgt = 286;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
								
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
#ifdef DP
						else if( g_DoppleganerEvent.IsEventMap(lpObj->MapNumber) )
						{
							LogAddTD("[Doppelganger] UserDie MoveMap [%s][%s]", lpObj->AccountID, lpObj->Name);
							g_DoppleganerEvent.LeaveDoppelgangerEvent(lpObj->m_Index);

							int mgt = 267;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
								
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}

							if( g_DoppleganerEvent.GetDoppelgangerState() == 2 )
							{
								g_DoppleganerEvent.SendDoppelgangerResult(lpObj, 1);
							}
						}
#endif
#ifdef IMPERIAL
						else if( g_ImperialGuardian.IsEventMap(lpObj->MapNumber) )
						{
							int mgt = 22;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
								
							int result = gGateC.GetGate(mgt, (short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}

							g_ImperialGuardian.GCMissionFailUseDie(lpObj->m_Index);
						}
#endif
#ifdef SEASON6DOT3_ENG
						else if( lpObj->MapNumber == 80 || lpObj->MapNumber == 81 )
						{
							int mgt = 335;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
								
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
								
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
#endif
						else // 3rd JMP??
						{
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}

						short m_Result = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,lpObj->MapNumber,lpObj->m_sPrevMapSvrCode);

						if(m_Result != gGameServerCode )
						{
							if(m_Result == -1)
							{
								LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);
								continue;
							}

							int _map = lpObj->MapNumber;
							int _x = lpObj->X;
							int _y = lpObj->Y;

							lpObj->MapNumber = mapnum;
							lpObj->X = x;
							lpObj->Y = y;

							GJReqMapSvrMove(lpObj->m_Index,m_Result,_map,_x,_y);
							LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",m_Result,lpObj->AccountID,lpObj->Name,lpObj->m_Index);
							continue;
						}

						lpObj->TX = lpObj->X;
						lpObj->TY = lpObj->Y;

						CreateFrustrum(lpObj->X,lpObj->Y,n);

						pMsg.h.c = 0xC3;
						pMsg.h.headcode = 0xF3;
						pMsg.h.size = sizeof(pMsg);
						pMsg.subcode = 4;
						pMsg.Dir = lpObj->Dir;
						pMsg.MapX = lpObj->X;
						pMsg.MapY = lpObj->Y;
						pMsg.Life = lpObj->Life;
						pMsg.Mana = lpObj->Mana;

						__int64 Experience = lpObj->Experience; //0AC

						if(g_MasterLevelSystem.IsMasterLevelUser(lpObj) != FALSE) //Set ML Exp Info
						{
							Experience = lpObj->m_i64MasterLevelExp;
						}
						else
						{
							Experience = lpObj->Experience;
						}

						pMsg.ExpHHH = SET_NUMBERH(SET_NUMBERHW(HIDWORD(Experience)));
						pMsg.ExpHHL = SET_NUMBERL(SET_NUMBERHW(HIDWORD(Experience)));
						pMsg.ExpHLH = SET_NUMBERH(SET_NUMBERLW(HIDWORD(Experience)));
						pMsg.ExpHLL = SET_NUMBERL(SET_NUMBERLW(HIDWORD(Experience)));
						pMsg.ExpLHH = SET_NUMBERH(SET_NUMBERHW(LODWORD(Experience)));
						pMsg.ExpLHL = SET_NUMBERL(SET_NUMBERHW(LODWORD(Experience)));
						pMsg.ExpLLH = SET_NUMBERH(SET_NUMBERLW(LODWORD(Experience)));
						pMsg.ExpLLL = SET_NUMBERL(SET_NUMBERLW(LODWORD(Experience)));

						pMsg.MapNumber = lpObj->MapNumber;
						pMsg.Money = lpObj->Money;
						pMsg.wShield = lpObj->iShield;
						pMsg.BP = lpObj->BP;

						lpObj->PathCount = 0;

						DataSend(n,(LPBYTE)&pMsg,pMsg.h.size);

						gObjViewportListProtocolCreate(lpObj);

						if(DragonEvent->GetState())
						{
							if(DragonEvent->GetMapNumber() == lpObj->MapNumber)
							{
								GCMapEventStateSend(lpObj->MapNumber,1,1);
							}
						}

						gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);

						if(lpObj->Authority == 32) //season4 add-on
						{
							gObjAddBuffEffect(lpObj, AT_GAMEMASTER_LOGO, 0, 0, 0, 0, -10);
						}

//#if(_GSCS==1)
						if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
						{	
							::GCAnsCsNotifyStart(lpObj->m_Index,CASTLESIEGEREADY(g_CastleSiege.GetCastleState()));
							
							if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
							{
								g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
								g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
							}
							
						}
//#endif

						if(gTamaJJangEvent!=0 && gAppearTamaJJang!=0)
						{
							if(lpObj->MapNumber == MAP_INDEX_RORENCIA)
							{
								GCMapEventStateSend(0,gAppearTamaJJang,2);
							}
						}
					}
					else
					{
						if(gObjMonsterRegen(lpObj)==0)
						{
							continue;
						}
						CreateFrustrum(lpObj->X,lpObj->Y,n);

						if(IT_MAP_RANGE(lpObj->MapNumber)) //season 2.5 add-on
						{
							if(lpObj->Class == 380)
							{
								LogAddTD("[Illusion Temple] (%d) Status Regen OK (%d: %d/%d)", lpObj->MapNumber - 44, lpObj->MapNumber, lpObj->X, lpObj->Y);
							}
						}
					}

					lpObj->DieRegen = 0;
					lpObj->m_State = 1;					
				}
			}
		}
	}

	for(n = 0; n < MAX_NUMBER_MAP; n++)
	{
		MapC[n].StateSetDestroy();
	}
}

void gObjSecondProc()
{
	if(gDisconnect == 1)
	{
		return;
	}

	int n;
	LPOBJ lpObj;

	for(n = 0; n < OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->m_bMapSvrMoveQuit == 1)
			{
				if(GetTickCount() - lpObj->m_dwMapSvrQuitTick > 30000)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}
			}

			gObjSkillUseProc(lpObj);
			gObjCheckBuffEffectList(lpObj);

			if(lpObj->Type == OBJ_NPC && lpObj->Class == 229)
			{
				gQeustNpcTeleport.Run(n);
			}

			if( lpObj->Type == OBJ_NPC && lpObj->Class == 568 )	//1.01.00
			{
				gQeustNpcTeleport.Run(n);
			}

			if(lpObj->Type == OBJ_MONSTER)
			{
				if(lpObj->Class == 275)
				{
					gObjRefillMonsterHP(lpObj,giKundunRefillHPSec);
				}

//#if(_GSCS==1)
				if(lpObj->Class == 283)
				{
					g_CsNPC_GuardianStatue.GuardianStatueAct(lpObj->m_Index);
					continue;
				}
				
				if(lpObj->Class == 278)
				{
					g_CsNPC_LifeStone.LifeStoneAct(lpObj->m_Index);
					continue;
				}
//#endif

				if(lpObj->Class == 285)
				{
					g_CsNPC_Guardian.GuardianAct(lpObj->m_Index);
					continue;
				}

//#if(_GSCS==1)
				if(lpObj->Class == 288)
				{
					g_CsNPC_CannonTower.CannonTowerAct(lpObj->m_Index);
					continue;
				}
				g_Crywolf.CrywolfMonsterAct(lpObj->m_Index);
//#endif
			}
			if(lpObj->Type == OBJ_NPC)
			{
//#if(_GSCS==1)
				if( (lpObj->Class < 204)?FALSE:(lpObj->Class > 209)?FALSE:TRUE)
				{
					g_Crywolf.CrywolfNpcAct(lpObj->m_Index);
				}
				
				if(lpObj->Class == 216)
				{
					g_CsNPC_CastleCrown.CastleCrownAct(lpObj->m_Index);
					continue;
				}
				
				if(CASTLE_CROWNSWITCH_CLASS_RANGE(lpObj->Class))
				{
					g_CsNPC_CastleCrownSwitch.CastleCrownSwitchAct(lpObj->m_Index);
					continue;
				}
//#endif
				if(lpObj->Class == 221 || lpObj->Class == 222)
				{
					g_CsNPC_Weapon.WeaponAct(lpObj->m_Index);
				}
			}

			if(lpObj->Type == OBJ_USER)
			{
#ifdef MUHELPER
				g_MUHelper.Work(lpObj);
#endif

#ifdef OLDCASHSHOP
				g_CashShop.CGCashPoint(lpObj);
#endif
				gObjManaPotionFill(lpObj);
				gObjRestPotionFill(lpObj);
				gObjCheckXYMapTile(lpObj,3);

				if(lpObj->ChatLitmitTime > 0)
				{
					lpObj->ChatLimitTimeSec++;

					if(lpObj->ChatLimitTimeSec > 60)
					{
						lpObj->ChatLimitTimeSec = 0;
						lpObj->ChatLitmitTime--;

						if(lpObj->ChatLitmitTime < 1)
						{
							lpObj->ChatLitmitTime = 0;
							MsgOutput(lpObj->m_Index,lMsg.Get(1141));
						}
					}
				}

				if(g_iUseCharacterAutoRecuperationSystem != 0)
				{
					if(lpObj->Level <= g_iCharacterRecuperationMaxLevel)
					{
						gProcessAutoRecuperation(lpObj);
					}
				}

				if(lpObj->Type == OBJ_USER)
				{
					gObjShieldAutoRefill(lpObj);
				}
	
				if(lpObj->Type == OBJ_USER && lpObj->m_LastTeleportTime > 0)
				{
					lpObj->m_LastTeleportTime--;
				}
	
				gObjDelayLifeCheck(n);
				gObjSecondDurDown(lpObj);
	
				if(lpObj->PartyNumber >= 0)
				{
					gParty.PartyMemberLifeSend(lpObj->PartyNumber);
				}
	
				if(lpObj->m_ShopTime >= 1 && lpObj->m_ShopTime < 255)
				{
					lpObj->m_ShopTime++;
				}
	
				int BattleGround = 1;
				int m_BattleTimer = gCheckBattleGroundTimer();
	
				if((lpObj->Authority & 2) == 2)
				{
					BattleGround = 0;
					GCTimeViewSend(lpObj->m_Index,m_BattleTimer);
				}
	
				if(lpObj->MapNumber == MAP_INDEX_BATTLESOCCER && BattleGround == 1)
				{
					BattleGround = 1;
	
					if(lpObj->lpGuild != 0)
					{
						if(lpObj->lpGuild->WarState != 0)
						{
							int CheckBattleGround = gCheckBattleGround(lpObj);
	
							if(CheckBattleGround != lpObj->lpGuild->BattleGroundIndex)
							{
								if(lpObj->lpGuild->WarType == 1)
								{
									BattleGround = 0;
								}
								else
								{
									BattleGround = 0;
								}
							}
							else
							{
								BattleGround = 0;
							}
						}
					}
					else
					{
						BattleGround = 1;
					}
	
					if(BattleGround != 0)
					{
						int CheckBattleGround = gCheckBattleGround(lpObj);
		
						if(CheckBattleGround >= 0)
						{
							gObjMoveGate(lpObj->m_Index,17);
						}	
					}
				}
			}
		}

		if(lpObj->Connected >= PLAYER_LOGGED && lpObj->Type == OBJ_USER &&	lpObj->CloseCount > 0)
		{
			if(lpObj->CloseCount == 1)
			{
				if(lpObj->CloseType == 1)
				{
					if(gObjGameClose(lpObj->m_Index) == 1)
					{
						GCCloseMsgSend(lpObj->m_Index,1);
						GJUpdateMatchDBUserCharacters(lpObj);
					}
				}
				else if(lpObj->CloseType == 0)
				{
					GCCloseMsgSend(lpObj->m_Index,0);
					CloseClient(lpObj->m_Index);
				}
				else if(lpObj->CloseType == 2)
				{
					GCCloseMsgSend(lpObj->m_Index,2);
				}
			}
			else
			{
				char Msg[128];
				wsprintf(Msg,lMsg.Get(1116),lpObj->CloseCount-1);
				GCServerMsgStringSend(Msg,lpObj->m_Index,1);
			}
			lpObj->CloseCount--;
		}
		if(lpObj->Connected > PLAYER_LOGGED &&	lpObj->Type == OBJ_USER)
		{
#ifdef OFFEXP
			OffExp.TickTimes(n);
#endif
			if(GetTickCount() - lpObj->AutoSaveTime > 600000)
			{
				GJSetCharacterInfo(lpObj,n,0, 0);
				lpObj->AutoSaveTime = GetTickCount();
			}

			/*if(lpObj->CheckSumTime > 0 && GetTickCount() - lpObj->CheckSumTime > 5000)
			{
				LogAddTD("[%s][%s] CheckSumTime Error",lpObj->AccountID, lpObj->Name);
				CloseClient(n);
			}
			gObjNProtectGGCheckSum(n);*/
			gObjPkDownTimeCheck(lpObj,1);
			gObjInterfaceTimeCheck(lpObj);
			gObjTimeCheckSelfDefense(lpObj);
			
		}

#ifdef PCBANG
		g_PCBangPointSystem.AddPoint(lpObj); //season4.5 add-on
#endif

		if(lpObj->Connected == PLAYER_CONNECTED ||	lpObj->Connected == PLAYER_LOGGED || lpObj->Connected == PLAYER_PLAYING)
		{
			if(lpObj->Type == OBJ_USER)
			{
				if(lpObj->Connected >= PLAYER_LOGGED)
				{
#ifdef __CUSTOMS__
					g_OfflineTrade.Update(lpObj->m_Index, 1);
#endif
#ifdef OFFEXP
					OffExp.Update(lpObj->m_Index, 1 );
#endif

#ifdef __MAKELO__
					if( g_OfflineAttack.CharInfo[lpObj->m_Index].IsOffline )
					{
						lpObj->ConnectCheckTime = GetTickCount();
					}
#endif
					if(GetTickCount() - lpObj->ConnectCheckTime > 60000)
					{
						ResponErrorCloseClient(n);
						LogAddTD(lMsg.Get(542),lpObj->m_Index,lpObj->AccountID,lpObj->Name,lpObj->Ip_addr);
					}
				}
				else
				{
					if(GetTickCount() - lpObj->ConnectCheckTime > 30000)
					{
						ResponErrorCloseClient(n);
						LogAddTD(lMsg.Get(543),lpObj->m_Index,lpObj->AccountID,lpObj->Name,lpObj->Ip_addr);
					}
				}
			}
		}
	}

	
	if( gTamaJJangEvent != 0)
	{
		gTamaJJangTime--;

		if(gTamaJJangTime <= 0)
		{
			if(gAppearTamaJJang==0)
			{
				gAppearTamaJJang = 1;
				gTamaJJangTime = gTamaJJangKeepTime;
				LogAddTD("Appear TamaJJang : Keep Time (%d)",gTamaJJangTime);
			}
			else
			{
				gAppearTamaJJang = 0;

				gTamaJJangTime = gTamaJJangDisappearTime +rand()% gTamaJJangDisappearTimeRandomRange;
				LogAddTD("DisAppear TamaJJang : Keep Time (%d)",gTamaJJangTime);
			}

			GCMapEventStateSend(0,gAppearTamaJJang,2);
		}
	}
	
	gWzAG.SetSubInfo(gServerMaxUser, gObjTotalUser, gMonsterHp, gItemDropPer, gAddExperience, GetEventFlag());
}

void gObjManaPotionFill(LPOBJ lpObj)
{
	lpObj->m_ManaFillCount++;

	float tmana;
	int ChangeMana = 0;
	float loc3;

	int loc4;

	if(lpObj->m_ManaFillCount > 2)
	{
		lpObj->m_ManaFillCount = 0;

		int BP,AddBP;
		BYTE MapAttr;
		int rBP,CurBP;

		if(lpObj->MaxMana + lpObj->AddMana > lpObj->Mana)
		{
			loc3 = 0.037f;

			if(!(BC_MAP_RANGE(lpObj->MapNumber) || 
				CC_MAP_RANGE(lpObj->MapNumber) || 
				KALIMA_MAP_RANGE(lpObj->MapNumber) || 
				DS_MAP_RANGE(lpObj->MapNumber) || 
				IT_MAP_RANGE(lpObj->MapNumber)) &&				
				!(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE || 
				lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE))
			{
				loc4 = gObjGetTotalValueOfEffect(lpObj, ADD_OPTION_MANA_AUTO_RECOVERY); //season3 add-on

				if(loc4 > 0)
				{
					loc3 += loc4 / 100.0f + 0.037f;	//1.01.00 fix
				}				
			}

			//1.01.00
			if( loc3 > 0.0f && lpObj->m_MPSkillOpt.MpsAutoRecoverMana > 0.0f )
			{
				loc3 = lpObj->m_MPSkillOpt.MpsAutoRecoverMana / 100.0f + loc3;
			}

			tmana = lpObj->Mana;
			tmana += (lpObj->MaxMana + lpObj->AddMana)*loc3;

			tmana += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP;

			if(lpObj->m_btRefillMPSocketOption > 0) //season4 add-on
			{
				tmana += lpObj->m_btRefillMPSocketOption;
			}

			if(lpObj->MaxMana + lpObj->AddMana < tmana)
			{
				tmana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->Mana = tmana;
			ChangeMana = 1;
		}

		BP = lpObj->MaxBP + lpObj->AddBP;

		if(lpObj->BP < BP)
		{
			MapAttr = MapC[lpObj->MapNumber].GetAttr((short &)lpObj->X,(short &)lpObj->Y);
			rBP = 2;

			if((MapAttr & 1) != 0)
			{
				rBP = 5;
			}

			if(gObjCheckUsedBuffEffect(lpObj, AT_XMASS_AG_RECOVERY) == 1) //season4 add-on
			{
				rBP = 5;

				if((MapAttr & 1) != 0)
				{
					rBP = 10;
				}
			}

			if(lpObj->Class == CLASS_KNIGHT)
			{
				AddBP = rBP +(BP * 5 / 100);
			}
			else
			{
				AddBP = rBP +(BP * 3 / 100);
			}
			//1.01.00
			if( lpObj->m_MPSkillOpt.MpsAutoRecoverAG > 0.0f )
			{
				AddBP += BP * lpObj->m_MPSkillOpt.MpsAutoRecoverAG / 100.0f;
			}

			if(gObjCheckUsedBuffEffect(lpObj, AT_POTION_OF_SOUL) == 1)
			{
				AddBP += 8;
			}

			AddBP += lpObj->SetOpIncAGValue;

			if(AddBP < 1)
			{
				AddBP = 1;
			}

			CurBP = lpObj->BP;
			CurBP += AddBP;

			if(CurBP > BP)
			{
				CurBP = BP;
			}

			lpObj->BP = CurBP;

			ChangeMana = 1;
		}

		if(ChangeMana != 0)
		{
			GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
		}
	}
}

void gObjRestPotionFill(LPOBJ lpObj)
{
	float tlife;
	float tmana;
	int percent;
	int loc4;
	int loc5;
	int loc6;
	int percent1; //loc7
	int tlife1; //loc8
	float tlife2; //loc9
	int iFillSDPoint; //loc10
	float cur; //loc11

	if(lpObj->m_Rest != 0)
	{
		percent = 3;

		lpObj->m_LifeFillCount++;

		if(lpObj->m_LifeFillCount > 4)
		{
			lpObj->m_LifeFillCount = 0;

			tlife = lpObj->Life;

			if(lpObj->pInventory[10].IsItem() == 1)
			{
				if(lpObj->pInventory[10].m_Option3 != 0 && lpObj->pInventory[10].m_Type != ITEMGET(13,24))
				{
					percent += lpObj->pInventory[10].m_Option3;
				}
			}
			else if(lpObj->pInventory[11].IsItem() == 1)
			{
				if(lpObj->pInventory[11].m_Option3 != 0 && lpObj->pInventory[11].m_Type != ITEMGET(13,24))
				{
					percent += lpObj->pInventory[11].m_Option3;
				}
			}
			else if(lpObj->pInventory[9].IsItem() == 1)
			{
				if(lpObj->pInventory[9].m_Option3 != 0 && lpObj->pInventory[9].m_Type != ITEMGET(13,28))
				{
					percent += lpObj->pInventory[9].m_Option3;
				}
			}

			if(percent > 0)
			{
				if(lpObj->m_MPSkillOpt.MpsAutoRecoverLife/*m_MLPassiveSkill.m_iML_HPAutoIncrement*/ > 0)
				{
					percent += lpObj->m_MPSkillOpt.MpsAutoRecoverLife;/*m_MLPassiveSkill.m_iML_HPAutoIncrement;*/
				}
			}

			loc4 = gObjGetTotalValueOfEffect(lpObj, ADD_OPTION_LIFE_AUTO_RECOVERY);

			if(loc4 > 0)
			{
				percent += loc4;
			}

			tlife += (lpObj->MaxLife + lpObj->AddLife)*percent / 100.0f;
			tlife += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP;

			if(lpObj->m_btRefillHPSocketOption > 0) //season4 add-on
			{
				tlife += lpObj->m_btRefillHPSocketOption;
			}
			
			if((lpObj->MaxLife + lpObj->AddLife) < tlife)
			{
				tlife = lpObj->MaxLife + lpObj->AddLife;
			}

			lpObj->Life = tlife;

			GCReFillSend(lpObj->m_Index,lpObj->Life,-1,0,lpObj->iShield);

			loc5 = 3;

			loc6 = gObjGetTotalValueOfEffect(lpObj, ADD_OPTION_MANA_AUTO_RECOVERY);

			if(loc6 > 0)
			{
				loc5 += loc6;
			}

			tmana = lpObj->Mana + lpObj->AddMana;
			tmana += (lpObj->MaxMana + lpObj->AddMana) * loc5 / 100.0f;
			tmana += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillMP;

			if((lpObj->MaxMana + lpObj->AddMana) < tmana)
			{
				tmana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->Mana = tmana;
			GCManaSend(lpObj->m_Index,lpObj->Mana,-1,0,lpObj->BP);
		}
	}
	else
	{
		percent1 = 0;

		if(lpObj->pInventory[10].IsItem() == 1)
		{
			if(lpObj->pInventory[10].m_Option3 != 0 && lpObj->pInventory[10].m_Durability != 0.0f && lpObj->pInventory[10].m_Type != ITEMGET(13,24))
			{
				percent1 += lpObj->pInventory[10].m_Option3;
			}
		}

		if(lpObj->pInventory[11].IsItem() == 1)
		{
			if(lpObj->pInventory[11].m_Option3 != 0 && lpObj->pInventory[11].m_Durability != 0.0f && lpObj->pInventory[11].m_Type != ITEMGET(13,24))
			{
				percent1 += lpObj->pInventory[11].m_Option3;
			}
		}

		if(lpObj->pInventory[9].IsItem() == 1)
		{
			if(lpObj->pInventory[9].m_Option3 != 0 && lpObj->pInventory[9].m_Durability != 0.0f && lpObj->pInventory[9].m_Type != ITEMGET(13,28))
			{
				percent1 += lpObj->pInventory[9].m_Option3;
			}
		}

		if(lpObj->pInventory[7].IsItem() == 1)
		{
			if(lpObj->pInventory[7].m_Option3 != 0 && lpObj->pInventory[7].m_Special[2] == 85 && lpObj->pInventory[7].m_Durability != 0.0f)
			{
				percent1 += lpObj->pInventory[7].m_Option3;
			}
		}

		if(!(BC_MAP_RANGE(lpObj->MapNumber) || 
			CC_MAP_RANGE(lpObj->MapNumber) || 
			KALIMA_MAP_RANGE(lpObj->MapNumber) || 
			DS_MAP_RANGE(lpObj->MapNumber) || 
			IT_MAP_RANGE(lpObj->MapNumber)) &&				
			!(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE || 
			lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE))
		{
			tlife1 = gObjGetTotalValueOfEffect(lpObj, ADD_OPTION_LIFE_AUTO_RECOVERY);

			if(tlife1 > 0)
			{
				percent1 += tlife1;
			}
		}

		if(percent1 != 0)
		{
			if(lpObj->m_MPSkillOpt.MpsAutoRecoverLife/*m_MLPassiveSkill.m_iML_HPAutoIncrement*/ > 0)
			{
				percent1 += lpObj->m_MPSkillOpt.MpsAutoRecoverLife;/*m_MLPassiveSkill.m_iML_HPAutoIncrement;*/
			}

			lpObj->m_LifeFillCount++;

			if(lpObj->m_LifeFillCount > 6)
			{
				lpObj->m_LifeFillCount = 0;

				tlife2 = lpObj->Life;

				tlife2 += (lpObj->MaxLife +lpObj->AddLife) * percent1 / 100.f;
				tlife2 += lpObj->m_JewelOfHarmonyEffect.HJOpAddRefillHP;

				if(lpObj->m_btRefillHPSocketOption > 0) //season4 add-on
				{
					tlife2 += lpObj->m_btRefillHPSocketOption;
				}

				if((lpObj->MaxLife + lpObj->AddLife) < tlife2)
				{
					tlife2 = lpObj->MaxLife + lpObj->AddLife;
				}

				lpObj->Life = tlife2;

				GCReFillSend(lpObj->m_Index,lpObj->Life,-1,0,lpObj->iShield);
			}
		}
	}

	if(lpObj->iFillShield > 0)
	{
		iFillSDPoint = 0;

		if(lpObj->iFillShieldCount <= 0)
		{
			lpObj->iFillShieldCount = 1;
		}

		iFillSDPoint = lpObj->iFillShieldMax / lpObj->iFillShieldCount;

		lpObj->iShield += iFillSDPoint;

		lpObj->iFillShield -= iFillSDPoint;

		if(lpObj->iFillShield < iFillSDPoint)
		{
			iFillSDPoint = lpObj->iFillShield;
			lpObj->iFillShield = 0;

			if(iFillSDPoint < 0)
			{
				iFillSDPoint = 0;
			}
		}

		if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
		{
			lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

			lpObj->iFillShield = 0;
			lpObj->iFillShieldMax = 0;
		}

		GCReFillSend(lpObj->m_Index,lpObj->Life,-1,0,lpObj->iShield);
	}

	if(lpObj->FillLife > 0)
	{
		cur = lpObj->FillLifeMax * 34.0f / 100.0f;

		if(lpObj->FillLifeCount == 2)
		{
			cur = lpObj->FillLifeMax * 54.0f / 100.0f;
		}

		if(lpObj->FillLifeCount == 4)
		{
			cur = lpObj->FillLifeMax * 25.0f / 100.0f;
		}

		if(lpObj->FillLife < cur)
		{
			cur = lpObj->FillLife;
			lpObj->FillLife = 0;

			if(cur < 0)
			{
				cur = 0;
			}
		}
		else
		{
			lpObj->FillLife -= cur;
		}

		lpObj->Life += cur;

		if((lpObj->MaxLife + lpObj->AddLife) < lpObj->Life)
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			lpObj->FillLife = 0;
		}

		GCReFillSend(lpObj->m_Index,lpObj->Life,-1,0,lpObj->iShield);
	}
}

void gObjUseDrink(LPOBJ lpObj, int level)
{
	int time;

	if ( level == 2 )
	{
		time = 90;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 1, time);
		lpObj->m_SkillAttack = 1;
		lpObj->m_SkillAttackTime = 90;
	}
	else if ( level == 1 )
	{
		time = 180;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
	else if ( level == 0x4D )
	{
		time = 60;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 2, time);
	}
	else
	{
		time = 80;
		GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
}

int  gObjCurMoveMake(BYTE * const path , LPOBJ lpObj) //Original Source Obtained from GS 0.78.02T WEBZEN
{
	int cur      = lpObj->PathCur;
	int total    = lpObj->PathCount;
	int sendbyte = 1;
	
//	memset(path, 0, 8);

	path[0] = (lpObj->Dir<<4);			// ????
	path[0] |= (BYTE)((total-cur)&0x0f);		// ?????? ?§?? ???? °???

	if( (total-cur) < 0 ) {
		MsgBox("error : %s %d",__FILE__, __LINE__);
		path[0] &= 0xF0;
		return sendbyte;
	}
	
	if( total == 0 ) {
		path[0] &= 0xF0;
		return sendbyte;	// ??µ??» ????????
	}
	if( total == cur ) {
		path[0] &= 0xF0;
		return sendbyte;	// ??µ??? ???µ????
	}
	
	for( int n=cur, i=1; n<total; n++, i++)
	{
		if( i%2 == 1 ) {
			path[(i+1)/2] = (lpObj->PathOri[n]<<4);
		}
		else { 
			path[(i+1)/2] |= (lpObj->PathOri[n]&0x0f);
		}
	}

	sendbyte += ((path[0]&0x0f)+1)/2;

//	LogAddHeadHex(1,(char*)path, 8);
//	LogAdd("path[0] : %x sendbyte:%d ",path[0], sendbyte);	
	return sendbyte;
}

void gObjViewportListProtocolDestroy(LPOBJ lpObj)
{
	BYTE sendBuf[5000];
	int lOfs,count;
	count = 0;
	lOfs = 4;

	PBMSG_COUNT pCount;

	pViewportDestroy.NumberH = SET_NUMBERH(lpObj->m_Index);
	pViewportDestroy.NumberL = SET_NUMBERL(lpObj->m_Index);

	memcpy(&sendBuf[lOfs],&pViewportDestroy,2);

	lOfs += 2;
	count += 1;

	pCount.h.c = 0xC1;
	pCount.h.headcode = 0x14;
	pCount.h.size = lOfs;
	pCount.count = count;

	memcpy(sendBuf,&pCount,sizeof(pCount));

	if(count < 1)	{
		return;
	}

	MsgSendV2(lpObj,sendBuf,lOfs);

}

void gObjViewportListProtocolCreate(LPOBJ lpObj)
{
	int n,tObjNum;
	BYTE sendBuf[5000];
	int lOfs,count,moncount;
	PWMSG_COUNT pwCount;

	count = 0;
	moncount = 0;
	lOfs = sizeof(pwCount);

	int iViewSkillCount = 0;

	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	if(lpObj->Type == OBJ_USER)
	{
		if(lpObj->m_Change >= 0)
		{
			pViewportCreateChange.NumberH = SET_NUMBERH(lpObj->m_Index);
			pViewportCreateChange.NumberL = SET_NUMBERL(lpObj->m_Index);

			pViewportCreateChange.NumberH |= 0x80;

			pViewportCreateChange.X = lpObj->X;
			pViewportCreateChange.Y = lpObj->Y;

			pViewportCreateChange.TX = lpObj->TX;
			pViewportCreateChange.TY = lpObj->TY;

			pViewportCreateChange.SkinH = SET_NUMBERH((lpObj->m_Change & 0xFFFF) & 0xFFFF);
			pViewportCreateChange.SkinL = SET_NUMBERL((lpObj->m_Change & 0xFFFF) & 0xFFFF);

			//Season 2.5 add-on
			if(gObjCheckUsedBuffEffect(lpObj, AT_GAMEMASTER_LOGO) == 1) //Season3 update
			{
				gObjAddBuffEffect(lpObj, AT_GAMEMASTER_LOGO, 0, 0, 0, 0, -10); //Season3 update
			}

			pViewportCreateChange.DirAndPkLevel = lpObj->Dir << 4;
			pViewportCreateChange.DirAndPkLevel |= lpObj->m_PK_Level & 0x0F;

			//Season 2.5 add-on
			lpObj->CharSet[0] &= 0xF8;
			lpObj->CharSet[0] |= lpObj->m_ViewState & 0x07;

			memcpy(pViewportCreateChange.CharSet,lpObj->CharSet,sizeof(pViewportCreateChange.CharSet)); //Season 2.5 Add-on

			memcpy(pViewportCreateChange.Id,lpObj->Name,sizeof(pViewportCreateChange.Id));
			
			iViewSkillCount = gObjMakeViewportState(lpObj, pViewportCreateChange.btViewSkillState); //Season3
			pViewportCreateChange.btViewSkillStateCount = iViewSkillCount; //Season3
			memcpy(&sendBuf[lOfs],&pViewportCreateChange,(sizeof(pViewportCreateChange)) - (MAX_STATE_COUNT - iViewSkillCount)); //Season3 changed
			lOfs += (sizeof(pViewportCreateChange)) - (MAX_STATE_COUNT - iViewSkillCount); //Season3 changed

			count += 1;

			if(count < 1)
			{
				return;
			}

			pwCount.h.headcode = 0x45;
		}
		else
		{
			pViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
			pViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);

			lpObj->CharSet[0] &= 0xF8; //Season 2.5 changed

			if(lpObj->m_State == 1 && lpObj->Teleport==0)
			{
				pViewportCreate.NumberH |= 0x80;
			}

			lpObj->CharSet[0] |= lpObj->m_ViewState & 0x07; //Season 2.5 changed

			pViewportCreate.X = lpObj->X;
			pViewportCreate.Y = lpObj->Y;
			pViewportCreate.TX = lpObj->TX;
			pViewportCreate.TY = lpObj->TY;

			pViewportCreate.DirAndPkLevel = lpObj->Dir << 4;
			pViewportCreate.DirAndPkLevel |= lpObj->m_PK_Level & 0x0F;

			memcpy(pViewportCreate.CharSet,lpObj->CharSet,sizeof(pViewportCreate.CharSet));
			memcpy(pViewportCreate.Id,lpObj->Name,sizeof(pViewportCreate.Id));

			iViewSkillCount = gObjMakeViewportState(lpObj, pViewportCreate.btViewSkillState); //Season3
			pViewportCreate.btViewSkillStateCount = iViewSkillCount; //Season3
			memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate)); //Season3 changed

			lOfs += (sizeof(pViewportCreate)) - (MAX_STATE_COUNT - iViewSkillCount); //Season3 changed

			count += 1;

			if(count < 1)
			{
				return;
			}

			pwCount.h.headcode = 0x12;
		}

		pwCount.h.c = 0xC2;
		pwCount.h.sizeH = SET_NUMBERH(lOfs);
		pwCount.h.sizeL = SET_NUMBERL(lOfs);
		pwCount.count = count;

		memcpy(sendBuf,&pwCount,sizeof(pwCount));

		DataSend(lpObj->m_Index,sendBuf,lOfs);

		if(lpObj->GuildNumber != 0)
		{
			GuildInfoOfs = 0;
			GuildInfoOfs = 5;

			PMSG_SIMPLE_GUILDVIEWPORT pGuild;

			pGuild.GuildNumber = lpObj->lpGuild->Number;
			pGuild.NumberH = SET_NUMBERH(lpObj->m_Index) & 0x7F;
			pGuild.NumberL = SET_NUMBERL(lpObj->m_Index);

			if(strcmp(lpObj->lpGuild->Names[0],lpObj->Name) == 0)
			{
				pGuild.NumberH |= 0x80;
			}

			pGuild.btGuildType = lpObj->lpGuild->btGuildType;
			pGuild.btGuildStatus = lpObj->GuildStatus;

			if( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == TRUE || //season 4 add-on
				g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE)
			{
				pGuild.btOwnerStatus = 1;
			}
			else
			{
				pGuild.btOwnerStatus = 0;
			}

			memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuild,sizeof(pGuild));

			GuildInfoOfs += sizeof(pGuild);

			PMSG_SIMPLE_GUILDVIEWPORT_COUNT pwCount;

			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x65;
			pwCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
			pwCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
			pwCount.Count = 1;

			memcpy(GuildInfoBuf,&pwCount,sizeof(pwCount));

			DataSend(lpObj->m_Index,GuildInfoBuf,GuildInfoOfs);
		}

#ifdef GENS
			gGensSystem.SendGensInfo(lpObj);
#endif

		for(n = 0; n < MVL; n++)
		{
			if(lpObj->VpPlayer2[n].state != 0)
			{
				tObjNum = lpObj->VpPlayer2[n].number;

				if(tObjNum >= 0)
				{
					if(lpObj->VpPlayer2[n].type == OBJ_USER)
					{
						DataSend(tObjNum,sendBuf,lOfs);

						if(GuildInfoCount != 0)
						{
							DataSend(tObjNum,GuildInfoBuf,GuildInfoOfs);
						}

						if(GuildUserCount != 0)
						{
							DataSend(tObjNum,GuildUserBuf,GuildUserOfs);
						}
					}
				}
			}
		}

#ifdef GENS
		gGensSystem.GensViewportListProtocol(lpObj);
#endif

	}
	else
	{
		count = 0;
	
		if(lpObj->m_RecallMon >= 0)
		{
			pCallMonsterViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
			pCallMonsterViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);
	
			if(lpObj->m_State == 1)
			{
				pCallMonsterViewportCreate.NumberH |= 0x80;
			}
	
			pCallMonsterViewportCreate.Type_HI = SET_NUMBERH(lpObj->Class);
			pCallMonsterViewportCreate.Type_LO = SET_NUMBERL(lpObj->Class);
	
			pCallMonsterViewportCreate.X = lpObj->X;
			pCallMonsterViewportCreate.Y = lpObj->Y;
	
			pCallMonsterViewportCreate.TX = lpObj->TX;
			pCallMonsterViewportCreate.TY = lpObj->TY;
	
			pCallMonsterViewportCreate.Path = lpObj->Dir << 4;
	
			if(lpObj->m_RecallMon >= 0 && lpObj->m_RecallMon < OBJMAX-1)
			{
				memcpy(pCallMonsterViewportCreate.Id,gObj[lpObj->m_RecallMon].Name,sizeof(pCallMonsterViewportCreate.Id));
			}
			else
			{
				memset(pCallMonsterViewportCreate.Id,0x00,sizeof(pCallMonsterViewportCreate.Id));
			}

			iViewSkillCount = gObjMakeViewportState(lpObj, pCallMonsterViewportCreate.btViewSkillState); //Season3
			pCallMonsterViewportCreate.btViewSkillStateCount = iViewSkillCount; //Season3
			memcpy(&sendBuf[lOfs],&pCallMonsterViewportCreate,(sizeof(pCallMonsterViewportCreate))); //Season3 changed
			lOfs += (sizeof(pCallMonsterViewportCreate)) - (MAX_STATE_COUNT - iViewSkillCount); //Season3 changed

			count += 1;
		}
		else
		{
			pMonsterViewportCreate.NumberH = SET_NUMBERH(lpObj->m_Index);
			pMonsterViewportCreate.NumberL = SET_NUMBERL(lpObj->m_Index);
	
			if(lpObj->m_State == 1 && lpObj->Teleport == 0)
			{
				pMonsterViewportCreate.NumberH |= 0x80;
				pMonsterViewportCreate.NumberH |= 0x40;
			}
	
			pMonsterViewportCreate.Type_HI = SET_NUMBERH(lpObj->Class);
			pMonsterViewportCreate.Type_LO = SET_NUMBERL(lpObj->Class);
	
			pMonsterViewportCreate.X = lpObj->X;
			pMonsterViewportCreate.Y = lpObj->Y;
			pMonsterViewportCreate.TX = lpObj->TX;
			pMonsterViewportCreate.TY = lpObj->TY;
	
			pMonsterViewportCreate.Path = lpObj->Dir << 4;

			iViewSkillCount = gObjMakeViewportState(lpObj, pMonsterViewportCreate.btViewSkillState); //Season3
			pMonsterViewportCreate.btViewSkillStateCount = iViewSkillCount; //Season3
			memcpy(&sendBuf[lOfs],&pMonsterViewportCreate,(sizeof(pMonsterViewportCreate))); //Season3 changed
			lOfs += (sizeof(pMonsterViewportCreate)) - (MAX_STATE_COUNT - iViewSkillCount); //Season3 changed

			moncount += 1;
		}
	
		if(count > 0)
		{
			PWMSG_COUNT pwCount;
	
			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x1F;
			pwCount.h.sizeH = SET_NUMBERH(lOfs);
			pwCount.h.sizeL = SET_NUMBERL(lOfs);
			pwCount.count = count;
	
			memcpy(sendBuf,&pwCount,sizeof(pwCount));
			MsgSendV2(lpObj,sendBuf,lOfs);
		}
	
		if(moncount > 0)
		{
			PWMSG_COUNT pwCount;
	
			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x13;
			pwCount.h.sizeH = SET_NUMBERH(lOfs);
			pwCount.h.sizeL = SET_NUMBERL(lOfs);
			pwCount.count = moncount;
	
			memcpy(sendBuf,&pwCount,sizeof(pwCount));
			MsgSendV2(lpObj,sendBuf,lOfs);
		}
	}
}

void gObjViewportListProtocol(short aIndex)
{
	if(gObj[aIndex].Connected < PLAYER_PLAYING)
	{
		return;
	}

	int n;
	PBMSG_COUNT pCount;
	LPOBJ lpTargetObj,lpObj = &gObj[aIndex];
	int lOfs_Item,count_Item,ItemStructSize = sizeof(PMSG_ITEMVIEWPORTCREATE),lOfs,MonlOfs,callMonlOfs;
	int pMsgSize;
	short tObjNum;
	BYTE count,monstercount,callmonstercount;

	BYTE sendBuf[5000];
	BYTE MonstersendBuf[5000];
	BYTE callMonstersendBuf[5000];
	BYTE ItemBuf[5000];

	GuildInfoOfs = 0;
	GuildInfoCount = 0;
	GuildUserOfs = 0;
	GuildUserCount = 0;
	ChangeCount = 0;
	GuildInfoOfs = 5;

	int iViewSkillCount = 0;

	GuildUserOfs = 5;
	lOfsChange = 5;

	lOfs = 4;
	lOfs_Item = 5;
	count = 0;
	count_Item = 0;

	int iGensCount = 0; int iViewportSize = 6; 

	if (lpObj->Type == OBJ_USER)
	{
		for (n = 0; n < MAX_VIEWPORT; n++)
		{
			if(lpObj->VpPlayer[n].state == PLAYER_PLAYING)
			{
				tObjNum = lpObj->VpPlayer[n].number;

				if(tObjNum >= 0)
				{
					switch(lpObj->VpPlayer[n].type)
					{
					case PLAYER_CONNECTED:
					case PLAYER_LOGGED:
					case PLAYER_PLAYING:
							pViewportDestroy.NumberH = SET_NUMBERH(tObjNum);
							pViewportDestroy.NumberL = SET_NUMBERL(tObjNum);
							memcpy(&sendBuf[lOfs],&pViewportDestroy,sizeof(pViewportDestroy));
							lOfs += sizeof(pViewportDestroy);
							count += 1;
						break;
					case 5:
						pItemViewportDestroy.NumberH = SET_NUMBERH(tObjNum);
						pItemViewportDestroy.NumberL = SET_NUMBERL(tObjNum);

						memcpy(&ItemBuf[lOfs_Item],&pItemViewportDestroy,sizeof(pItemViewportDestroy));

						lOfs_Item += sizeof(pItemViewportDestroy);
						count_Item += 1;
						break;
					}
				}

				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount -= 1;
			}
		}
	}
	else if (lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		for(n = 0; n < MAX_VIEWPORT_MONSTER;n++)
		{
			if(lpObj->VpPlayer[n].state == 3)
			{
				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount -= 1;
			}
		}
	}

	if (lpObj->Type == OBJ_USER)
	{
		if (count > 0)
		{
			pCount.h.c = 0xC1;
			pCount.h.headcode = 0x14;
			pCount.h.size = lOfs;
			pCount.count = count;

			memcpy(sendBuf,&pCount,sizeof(pCount));
			DataSend(aIndex,sendBuf,lOfs);
		}

		if (count_Item > 0)
		{
			pItemCount.h.c = 0xC2;
			pItemCount.h.headcode = 0x21;
			pItemCount.h.sizeH = SET_NUMBERH(lOfs_Item);
			pItemCount.h.sizeL = SET_NUMBERL(lOfs_Item);
			pItemCount.count = count_Item;

			memcpy(ItemBuf,&pItemCount,sizeof(pItemCount));
			DataSend(aIndex,ItemBuf,lOfs_Item);
		}
	}

	if (lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		for (n = 0; n < MAX_VIEWPORT_MONSTER; n++)
		{
			if (lpObj->VpPlayer[n].state == PLAYER_CONNECTED)
			{
				lpObj->VpPlayer[n].state = PLAYER_LOGGED;
			}
		}
	}
	else
	{
		count = 0;
		monstercount = 0;
		count_Item = 0;
		callmonstercount = 0;
		lOfs = 5;
		MonlOfs = lOfs;
		callMonlOfs = MonlOfs;
		lOfs_Item = 5;
		pMsgSize = 52;

#ifdef GENS
		unsigned char lpViewportAdd[MAX_VIEWPORT_INFO];
#endif

		if (lpObj->Type == OBJ_USER)
		{
			for (n = 0; n < MAX_VIEWPORT; n++)
			{
				if (lpObj->VpPlayer[n].state == PLAYER_CONNECTED)
				{
					tObjNum = lpObj->VpPlayer[n].number;
		
					if ((gObj[tObjNum].Authority & 32) == 32) 
					{
						if (gObjCheckUsedBuffEffect(&gObj[tObjNum], AT_INVISIBILITY) == 1)
						{
							continue;
						}
					}

#ifdef NPVP
					if( g_NewPVP.IsObserver(gObj[tObjNum]) )
					{
						continue;
					}
#endif

					if (tObjNum >= 0)
					{
						tObjNum = lpObj->VpPlayer[n].number;
						
						BYTE ViewportType = lpObj->VpPlayer[n].type;
						--ViewportType;
				
						switch (ViewportType)
						{
						case 0:
							lpTargetObj = &gObj[tObjNum];
		
							if (lpTargetObj->m_Change >= 0)
							{
								pViewportCreateChange.NumberH = SET_NUMBERH(tObjNum);
								pViewportCreateChange.NumberL = SET_NUMBERL(tObjNum);
		
								lpTargetObj->CharSet[0] &= 0xF8;
	
								if(lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
								{
									pViewportCreateChange.NumberH |= 0x80;
								}

								pViewportCreateChange.X = lpTargetObj->X;
								pViewportCreateChange.Y = lpTargetObj->Y;
								pViewportCreateChange.TX = lpTargetObj->TX;
								pViewportCreateChange.TY = lpTargetObj->TY;
								pViewportCreateChange.SkinH = SET_NUMBERH((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
								pViewportCreateChange.SkinL = SET_NUMBERL((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
								
								if ((lpTargetObj->Authority & 32) == 32) 
								{
									gObjAddBuffEffect(lpTargetObj, AT_GAMEMASTER_LOGO, 0, 0, 0, 0, -10); //Season3 update
								}
								
								pViewportCreateChange.DirAndPkLevel = lpTargetObj->Dir << 4;
								pViewportCreateChange.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;

								lpObj->CharSet[0] &= 0xF8; //Season 2.5 add-on
								lpObj->CharSet[0] |= lpObj->m_ViewState & 0x07; //Season 2.5 add-on

								memcpy(pViewportCreateChange.CharSet,lpTargetObj->CharSet,sizeof(pViewportCreateChange.CharSet)); //Season 2.5 add-on
								memcpy(pViewportCreateChange.Id,lpTargetObj->Name,sizeof(pViewportCreateChange.Id));

								iViewSkillCount = gObjMakeViewportState(lpTargetObj, pViewportCreateChange.btViewSkillState); //Season3
								pViewportCreateChange.btViewSkillStateCount = iViewSkillCount; //Season3
								memcpy(&SendGBufChange[lOfsChange],&pViewportCreateChange,sizeof(pViewportCreateChange)); //Season3 changed
								lOfsChange += (sizeof(pViewportCreateChange)) - (16 - iViewSkillCount); //Season3 changed

								ChangeCount +=1;
							}
							else
							{												
								pViewportCreate.NumberH = SET_NUMBERH(tObjNum);
								pViewportCreate.NumberL = SET_NUMBERL(tObjNum);
		
								lpTargetObj->CharSet[0] &= 0xF8;
		
								if (lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
								{
									pViewportCreate.NumberH |= 0x80;
								}
		
								lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState & 0x0F;
			
								pViewportCreate.X = lpTargetObj->X;
								pViewportCreate.Y = lpTargetObj->Y;
								pViewportCreate.TX = lpTargetObj->TX;
								pViewportCreate.TY = lpTargetObj->TY;
		
								pViewportCreate.DirAndPkLevel = lpTargetObj->Dir << 4;
								pViewportCreate.DirAndPkLevel |= lpTargetObj->m_PK_Level & 0x0F;

								memcpy(pViewportCreate.CharSet,lpTargetObj->CharSet,sizeof(pViewportCreate.CharSet));
								memcpy(pViewportCreate.Id,lpTargetObj->Name,sizeof(pViewportCreate.Id));

								iViewSkillCount = gObjMakeViewportState(lpTargetObj, pViewportCreate.btViewSkillState); //Season3
								pViewportCreate.btViewSkillStateCount = iViewSkillCount; //Season3
								memcpy(&sendBuf[lOfs],&pViewportCreate,sizeof(pViewportCreate)); //Season3 changed
								lOfs += (sizeof(pViewportCreate)) - (16 - iViewSkillCount); //Season3 changed

								count += 1;
							}


							if( lpObj->Type == OBJ_USER ) // wtf :D
							{
#ifdef GENS
								if( gGensSystem.GetGensInfluence(lpTargetObj) )
								{
									if (/*(lpTargetObj->Authority & 32) == 32 &&*/ gObjCheckUsedBuffEffect(lpTargetObj, AT_INVISIBILITY) ) 
									{
										continue;
									}
									// ----
									PHeadSubSetW((LPBYTE)&pGensCount, 0xF8, 5, 6);
									// ----	
									pGensMsg.btInfluence		= gGensSystem.GetGensInfluence(lpTargetObj);
									pGensMsg.NumberH			= SET_NUMBERH(lpTargetObj->m_Index);
									pGensMsg.NumberL			= SET_NUMBERL(lpTargetObj->m_Index);
									pGensMsg.iGensRanking		= lpTargetObj->m_GensRanking;
									pGensMsg.iGensClass			= gGensSystem.GetGensClass(lpTargetObj);
									pGensMsg.iContributePoint	= gGensSystem.GetContributePoint(lpTargetObj);
									// ----
									memcpy((BYTE*)lpViewportAdd + iViewportSize, &pGensMsg, 16);
									// ----						 
									iViewportSize += 16;	
									iGensCount++;
								}
#endif

								if (lpTargetObj->lpGuild != 0)
								{
									PMSG_SIMPLE_GUILDVIEWPORT pGuild;
		
									pGuild.GuildNumber = lpTargetObj->lpGuild->Number;
									pGuild.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
									pGuild.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
		
									pGuild.btGuildStatus = lpTargetObj->GuildStatus;
									pGuild.btGuildType = lpTargetObj->lpGuild->btGuildType;
		
									if (lpObj->lpGuild != 0)
									{
										pGuild.btGuildRelationShip = gObjGetRelationShip(lpObj,lpTargetObj);
									}
									else
									{
										pGuild.btGuildRelationShip = 0;
									}
		
									if(strcmp(lpTargetObj->lpGuild->Names[0],lpTargetObj->Name)==0)
									{
										pGuild.NumberH |= 0x80;
									}

									if( g_CastleSiegeSync.CheckCastleOwnerMember(lpTargetObj->m_Index) == TRUE || //season 4 add-on
										g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpTargetObj->m_Index) == TRUE)
									{
										pGuild.btOwnerStatus = 1;
									}
									else
									{
										pGuild.btOwnerStatus = 0;
									}
		
									memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuild,sizeof(pGuild));
									GuildInfoOfs += sizeof(pGuild);
									GuildInfoCount += 1;
								}
		
								if (false) //wtf?
								{
									PMSG_GUILDVIEWPORT_USER pGuildUserViewport;

									if(ViewGuildMng.Add(lpTargetObj->GuildNumber,lpTargetObj->m_Index)==1 && lpTargetObj->lpGuild != 0)
									{
										PMSG_GUILDVIEWPORT pGuildViewport;
		
										pGuildViewport.NumberH = SET_NUMBERH(lpTargetObj->lpGuild->Number);
										pGuildViewport.NumberL = SET_NUMBERL(lpTargetObj->lpGuild->Number);
		
										strcpy(pGuildViewport.GuildName,lpTargetObj->lpGuild->Name);
										memcpy(pGuildViewport.Mark,lpTargetObj->lpGuild->Mark,sizeof(pGuildViewport.Mark));
										memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuildViewport,sizeof(pGuildViewport));
		
										GuildInfoOfs += sizeof(pGuildViewport);
										GuildInfoCount += 1;
									}
		
									pGuildUserViewport.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
									pGuildUserViewport.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
		
									if(lpTargetObj->lpGuild != 0 && strcmp(lpTargetObj->lpGuild->Names[0],lpTargetObj->Name)==0)
									{
										pGuildUserViewport.NumberH |= 0x80;
									}
		
									if(lpTargetObj->lpGuild != 0)
									{
										pGuildUserViewport.GNumberH = SET_NUMBERH(lpTargetObj->lpGuild->Number);
										pGuildUserViewport.GNumberL = SET_NUMBERL(lpTargetObj->lpGuild->Number);
		
										memcpy(&GuildUserBuf[GuildUserOfs],&pGuildUserViewport,sizeof(pGuildUserViewport));
		
										GuildUserOfs += sizeof(pGuildUserViewport);
										GuildUserCount += 1;
									}
								}
							}
							break;


						case 1:
						case 2:
							if(lpObj->Type == OBJ_USER)
							{
								lpTargetObj = &gObj[tObjNum];
		
								if(lpTargetObj->m_RecallMon >= 0)
								{
									pCallMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pCallMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);
		
									if(lpTargetObj->m_State == 1)
									{
										pCallMonsterViewportCreate.NumberH |= 0x80;
									}
		
									pCallMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
									pCallMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);
		
									pCallMonsterViewportCreate.X = lpTargetObj->X;
									pCallMonsterViewportCreate.Y = lpTargetObj->Y;
									pCallMonsterViewportCreate.TX = lpTargetObj->TX;
									pCallMonsterViewportCreate.TY = lpTargetObj->TY;
									pCallMonsterViewportCreate.Path = lpTargetObj->Dir << 4;
		
									if(lpTargetObj->m_RecallMon >= 0 && lpTargetObj->m_RecallMon < OBJMAX-1)
									{
										memcpy(pCallMonsterViewportCreate.Id,gObj[lpTargetObj->m_RecallMon].Name,sizeof(pCallMonsterViewportCreate.Id));
									}
									else
									{
										memset(pCallMonsterViewportCreate.Id,0x00,sizeof(pCallMonsterViewportCreate.Id));
									}

									iViewSkillCount = gObjMakeViewportState(lpTargetObj, pCallMonsterViewportCreate.btViewSkillState); //Season3
									pCallMonsterViewportCreate.btViewSkillStateCount = iViewSkillCount; //Season3
									memcpy(&callMonstersendBuf[callMonlOfs],&pCallMonsterViewportCreate,sizeof(pCallMonsterViewportCreate)); //Season3 changed
									callMonlOfs += (sizeof(pCallMonsterViewportCreate)) - (16 - iViewSkillCount); //Season3 changed

									callmonstercount += 1;
								}
								else
								{
									pMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);
		
									if(lpTargetObj->m_State == 1)
									{
										pMonsterViewportCreate.NumberH |= 0x80;
		
										if(lpTargetObj->Teleport != 0)
										{
											pMonsterViewportCreate.NumberH |= 0x40;
										}
									}
		
									pMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
									pMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);

//#if(GS_CASTLE==1)
									if( lpTargetObj->Class == 278 )
									{
										if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
										{
											if( lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide )
											{
												pMonsterViewportCreate.Type_HI |= 0x80;
											}
										}
										if( lpObj->lpGuild && lpTargetObj->lpGuild )
										{
											if( lpObj->lpGuild == lpTargetObj->lpGuild )
											{
												pMonsterViewportCreate.Type_HI |= 0x80;
											}
										}
										pMonsterViewportCreate.Type_HI |= (lpTargetObj->m_btCreationState<<4 & 0x70);
									}
//#endif
									pMonsterViewportCreate.X = lpTargetObj->X;
									pMonsterViewportCreate.Y = lpTargetObj->Y;
									pMonsterViewportCreate.TX = lpTargetObj->TX;
									pMonsterViewportCreate.TY = lpTargetObj->TY;
									pMonsterViewportCreate.Path = lpTargetObj->Dir << 4;
		
//#if(GS_CASTLE==1)
									if( lpTargetObj->Class == 277 )
									{
										if( lpTargetObj->m_btCsGateOpen == 0 )
										{
											gObjRemoveBuffEffect(lpTargetObj, AT_CASTLE_GATE_STATUS);
										}
										else
										{
											gObjAddBuffEffect(lpTargetObj, AT_CASTLE_GATE_STATUS);
										}
									}
									if( lpTargetObj->Class == 216 )
									{
										if( g_CastleSiege.GetRegCrownAvailable() == FALSE )
										{
											gObjRemoveBuffEffect(lpTargetObj, AT_CASTLE_CROWN_STATUS);
										}
										else
										{
											gObjAddBuffEffect(lpTargetObj, AT_CASTLE_CROWN_STATUS);
										}
									}
//#endif

									iViewSkillCount = gObjMakeViewportState(lpTargetObj, pMonsterViewportCreate.btViewSkillState); //Season3
									pMonsterViewportCreate.btViewSkillStateCount = iViewSkillCount; //Season3
									memcpy(&MonstersendBuf[MonlOfs],&pMonsterViewportCreate,sizeof(pMonsterViewportCreate)); //Season3 changed
									MonlOfs += (sizeof(pMonsterViewportCreate)) - (16 - iViewSkillCount); //Season3 changed

									monstercount += 1;
								}
							}
							break;
						case 4:
							if(lpObj->Type == OBJ_USER)
							{
								pItemViewportCreate.NumberH = SET_NUMBERH(tObjNum);
								pItemViewportCreate.NumberL = SET_NUMBERL(tObjNum);
		
								if(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_State == 1)
								{
									pItemViewportCreate.NumberH |= 0x80;
								}
		
								pItemViewportCreate.px = MapC[lpObj->MapNumber].m_cItem[tObjNum].px;
								pItemViewportCreate.py = MapC[lpObj->MapNumber].m_cItem[tObjNum].py;
		
								if(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type == ITEMGET(14,15))
								{
									WORD MoneyHW = SET_NUMBERHW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
									WORD MoneyLW = SET_NUMBERLW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
		
									pItemViewportCreate.ItemInfo[0] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type)%255;
									pItemViewportCreate.ItemInfo[1] = SET_NUMBERL(MoneyHW);
									pItemViewportCreate.ItemInfo[2] = SET_NUMBERH(MoneyLW);
									pItemViewportCreate.ItemInfo[4] = SET_NUMBERL(MoneyLW);
									pItemViewportCreate.ItemInfo[3] = 0;
									pItemViewportCreate.ItemInfo[5] = (MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type & 0x1E00) >> 5;
									pItemViewportCreate.ItemInfo[6] = 0;
		
									memcpy(&ItemBuf[lOfs_Item],&pItemViewportCreate,sizeof(pItemViewportCreate));
									lOfs_Item += ItemStructSize;
								}
								else
								{
									ItemByteConvert(pItemViewportCreate.ItemInfo, (MapC[lpObj->MapNumber].m_cItem[tObjNum]));
									memcpy(&ItemBuf[lOfs_Item],&pItemViewportCreate,sizeof(pItemViewportCreate));
									lOfs_Item += ItemStructSize;
								}

								count_Item += 1;
							}
							break;
						}
					}
					lpObj->VpPlayer[n].state = 2;
				}
			}
		}

		if (lpObj->Type == OBJ_USER) // :D
		{
			if (count > 0)
			{
				PWMSG_COUNT pCount;
	
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x12;
				pCount.h.sizeH = SET_NUMBERH(lOfs);
				pCount.h.sizeL = SET_NUMBERL(lOfs);
				pCount.count = count;
	
				memcpy(sendBuf,&pCount,sizeof(pCount));
				DataSend(aIndex,(LPBYTE)&sendBuf,lOfs);
			}
	
			if (ChangeCount > 0)
			{
				PWMSG_COUNT pCount;
	
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x45;
				pCount.h.sizeH = SET_NUMBERH(lOfsChange);
				pCount.h.sizeL = SET_NUMBERL(lOfsChange);
				pCount.count = ChangeCount;
	
				memcpy(SendGBufChange,&pCount,sizeof(pCount));
				DataSend(aIndex,(LPBYTE)&SendGBufChange,lOfsChange);
			}
	
			if (monstercount > 0)
			{
				PWMSG_COUNT pCount;
	
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x13;
				pCount.h.sizeH = SET_NUMBERH(MonlOfs);
				pCount.h.sizeL = SET_NUMBERL(MonlOfs);
				pCount.count = monstercount;
	
				memcpy(MonstersendBuf,&pCount,sizeof(pCount));
				DataSend(aIndex,(LPBYTE)&MonstersendBuf,MonlOfs);
			}	
	
			if (callmonstercount > 0)
			{
				PWMSG_COUNT pCount;
	
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x1F;
				pCount.h.sizeH = SET_NUMBERH(callMonlOfs);
				pCount.h.sizeL = SET_NUMBERL(callMonlOfs);
				pCount.count = callmonstercount;
				
				memcpy(callMonstersendBuf,&pCount,sizeof(pCount));
				DataSend(aIndex,(LPBYTE)&callMonstersendBuf,callMonlOfs);
			}
	
			if(count_Item > 0)
			{
				PWMSG_COUNT pCount;
	
				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x20;
				pCount.count = count_Item;
				pCount.h.sizeH = SET_NUMBERH(lOfs_Item);
				pCount.h.sizeL = SET_NUMBERL(lOfs_Item);
				
				memcpy(ItemBuf,&pCount,sizeof(pCount));
				DataSend(aIndex,(LPBYTE)&ItemBuf,lOfs_Item);
			}
	
			if(GuildInfoCount != 0)
			{
				PWMSG_COUNT pCount;
	
				pCount.h.headcode = 0x65;
				pCount.h.c = 0xC2;
				pCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
				pCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
				pCount.count = GuildInfoCount;
				
				memcpy(GuildInfoBuf,&pCount,sizeof(pCount));
				DataSend(aIndex,(LPBYTE)&GuildInfoBuf,GuildInfoOfs);
			}

#ifdef GENS

			if( iGensCount > 0 )
			{
				PHeadSubSetW((LPBYTE)&pGensCount, 0xF8, 5, iViewportSize);				
				pGensCount.Count = iGensCount;				
				memcpy((BYTE*)lpViewportAdd, &pGensCount, 6);
				DataSend(aIndex, (LPBYTE)&lpViewportAdd, iViewportSize);
			}
#endif

		}
	}
}

void gObjSkillUseProc(LPOBJ lpObj)
{
	if(lpObj->m_SkillAttackTime > 0)
	{
		lpObj->m_SkillAttackTime--;

		if(lpObj->m_SkillAttackTime <= 0)
		{
			lpObj->m_SkillAttack = 0;
			lpObj->m_SkillAttackTime = 0;
		}
	}

	if(lpObj->SkillRecallParty_Time != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->SkillRecallParty_Time--;

		if(lpObj->SkillRecallParty_Time <= 0)
		{
			lpObj->SkillRecallParty_Time = 0;

			int m_check1 = gMoveCommand.CheckMainToMove(lpObj);
			int m_check2 = gMoveCommand.CheckEquipmentToMove(lpObj,lpObj->SkillRecallParty_MapNumber);
			int m_check3 = gMoveCommand.CheckInterfaceToMove(lpObj);

			if(m_check1 == false || m_check2 == false || m_check3 == false)
			{
				char szMsg[256];
				wsprintf(szMsg,lMsg.Get(1254));

				GCServerMsgStringSend(szMsg,lpObj->m_Index,1);
			}
			else
			{
				gObjRecall(lpObj->m_Index,lpObj->SkillRecallParty_MapNumber,lpObj->SkillRecallParty_X,lpObj->SkillRecallParty_Y);
			}
			//season4 add-on (fix)
			lpObj->SkillRecallParty_MapNumber = 0;
			lpObj->SkillRecallParty_Time = 0;
			lpObj->SkillRecallParty_X = 0;
			lpObj->SkillRecallParty_Y = 0;
		}
		else if(lpObj->SkillRecallParty_Time <= 5)
		{
			char szMsg[256];
			wsprintf(szMsg,lMsg.Get(1243),lpObj->SkillRecallParty_Time);

			GCServerMsgStringSend(szMsg,lpObj->m_Index,1);
		}
	}

	TMonsterSkillElementInfo::CheckSkillElementInfoProc(lpObj);

	if(lpObj->Type == OBJ_USER && lpObj->m_iItemEffectValidTime > 0)
	{
		lpObj->m_iItemEffectValidTime--;

		if(lpObj->m_iItemEffectValidTime <= 0)
		{
			lpObj->m_iItemEffectValidTime = 0;

			g_ItemAddOption.ClearItemEffect(lpObj,1);

			GCReFillSend(lpObj->m_Index,lpObj->MaxLife+lpObj->AddLife,0xFE,0,lpObj->iMaxShield+lpObj->iAddShield);
			GCManaSend(lpObj->m_Index,lpObj->MaxMana+lpObj->AddMana,0xFE,0,lpObj->MaxBP);
		}
	}

	if(IT_MAP_RANGE(lpObj->MapNumber)) //season 2.5 add-on
	{
		if(lpObj->m_iIllusionTempleIndex != -1)
		{
			g_IllusionTempleEvent.EventSkillProc(lpObj);
		}
	}

	if(lpObj->m_SkillNumber < 1)
	{
		return;
	}

	if(lpObj->m_SkillNumber == 18)
	{
		if(GetTickCount() > lpObj->m_SkillTime)
		{
			lpObj->m_SkillNumber = 0;
		}
	}
}

void gObjSkillBeAttackProc(LPOBJ lpObj)
{
}

void gObjTeleportMagicUse(int aIndex, BYTE x, BYTE y)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type == OBJ_USER) //season4 add-on
	{
		if( lpObj->Class == CLASS_WIZARD &&
			lpObj->SkillHellFire2State != 0)
		{
			lpObj->SkillHellFire2State = 0;
			lpObj->SkillHellFire2Time = 0;
		}
	}

	if(lpObj->Teleport != 0)
	{
		return;
	}

	lpObj->TeleportTime = GetTickCount();
	lpObj->PathCount = 0;
	lpObj->Teleport = 1;
	lpObj->m_StateSub = 1;
	lpObj->m_ViewState = 1;

	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = x;
	lpObj->TY = y;

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX,lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX,lpObj->TY);

	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

	gObjViewportListProtocolDestroy(lpObj);
}

BOOL gObjMoveGate(int aIndex, int gt)
{
	LPOBJ lpObj = &gObj[aIndex];
	int result;
	short x,y;
	BYTE mapNumber = 0,dir;
	short level;
	int userlevel = gObj[aIndex].Level;
	int movefail = 0;

	if(gObj[aIndex].SkillRecallParty_Time != 0)
	{
		char szMsg[255];
		gObj[aIndex].SkillRecallParty_Time = 0;
		wsprintf(szMsg,lMsg.Get(1254));
		GCServerMsgStringSend(szMsg,aIndex,1);
	}

	if(lpObj->Type == OBJ_USER)
	{
		gDarkSpirit[lpObj->m_Index].SetMode(CDarkSpirit::ePetItem_Mode::PetItem_Mode_Normal,-1);
	}

	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		int iBridgeIndex = g_BloodCastle.GetBridgeIndexByMapNum(lpObj->MapNumber);
		
		if(g_BloodCastle.GetCurrentState(iBridgeIndex) == 2){
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		else g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
	}

	if(gObj[aIndex].RegenOk != 0)
	{
		LogAdd("Regen OK %s %d",__FILE__,__LINE__);
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;

		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = x;
		lpObj->RegenMapY = y;

		gObjClearViewport(&gObj[aIndex]);

		GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenOk = 1;

		return false;
	}


	if(gGateC.IsGate(gt) == false)
	{

		LogAdd(lMsg.Get(545),gt,__FILE__,__LINE__);

		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;

		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = x;
		lpObj->RegenMapY = y;

		gObjClearViewport(&gObj[aIndex]);

		GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenOk = 1;

		return false;
	}

	if(gt >= 58 && gt <= 61 || gt == 111 || gt == 112 || gt == 270)
	{
		if(gObj[aIndex].m_bDevilSquareAuth == 0)
		{
			LogAddTD("[DevilSquare] [%s][%s] Not Used DevilSquare Invitation In DevilSquare",gObj[aIndex].AccountID,gObj[aIndex].Name);
			return false;
		}
		else
		{
			gObj[aIndex].m_bDevilSquareAuth = 0;
		}
	}

	lpObj->m_MoveGateNumber = gt;

	result = gGateC.GetGate(gt,(short &)x,(short &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level);

	if(result < 0)
	{
		LogAdd("error : %d (%s %d)",result,__FILE__,__LINE__);

		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;

		movefail = 1;
	}

#ifdef GENS
	if( gGensSystem.IsMapBattleZone(mapNumber) == true && !gGensSystem.IsUserBattleZoneEnable(lpObj) )
	{
		GCServerMsgStringSend(lMsg.Get(1604), lpObj->m_Index, 1);
		// ----
		x						= lpObj->X;
		y						= lpObj->Y;
		mapNumber				= lpObj->MapNumber;
		dir						= lpObj->Dir;
		movefail				= 1;
		// ----
		lpObj->RegenMapNumber	= lpObj->MapNumber;
		lpObj->RegenMapX		= x;
		lpObj->RegenMapY		= y;
		// ----
		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);
		// ----
		if( lpObj->m_Change >= 0 )
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
		// ----
		gObj[aIndex].RegenOk = 1;
		// ----
		return false;
	}
#endif

	if(lpObj->m_bPShopOpen == 1)
	{
		int bMove = 1;

		if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && mapNumber == MAP_INDEX_RORENCIA)
		{
			bMove = 0;
		}

		if(lpObj->MapNumber == MAP_INDEX_RORENCIA && mapNumber == MAP_INDEX_CASTLESIEGE)
		{
			bMove = 0;
		}

		if(bMove == 0)
		{
			LogAddTD("[PSHOP ITEM CLONNING] Move Failed : %d (%s %s)",result,lpObj->AccountID,lpObj->Name);

			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;

			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;

			lpObj->RegenMapNumber = mapNumber;
			lpObj->RegenMapNumber = lpObj->MapNumber;

			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
	
			gObjClearViewport(&gObj[aIndex]);
	
			GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
	
			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
	
			gObj[aIndex].RegenOk = 1;
		return false;
		}
	}

	if(BC_MAP_RANGE(mapNumber) || CC_MAP_RANGE(mapNumber) || DS_MAP_RANGE(mapNumber)
		|| CHECK_ILLUSIONTEMPLE(mapNumber)) //season 4 add-on (fix)
	{
		if(gObjCheckUsedBuffEffect(lpObj, AT_NPC_HELP) == 1)
		{
			gObjRemoveBuffEffect(lpObj, AT_NPC_HELP);
		}
	}

	if(CHECK_ILLUSIONTEMPLE(lpObj->MapNumber))
	{
		if(mapNumber != lpObj->MapNumber)
		{
			g_IllusionTempleEvent.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
			g_IllusionTempleEvent.LeaveIllusionTemple(lpObj->m_Index, lpObj->MapNumber);
		}
	}

#ifdef DP
		if( g_DoppleganerEvent.IsEventMap(lpObj->MapNumber) )
		{
			if(mapNumber != lpObj->MapNumber)
			{
				g_DoppleganerEvent.LeaveDoppelgangerEvent(lpObj->m_Index);

				if( g_DoppleganerEvent.GetDoppelgangerState() == 2 )
				{
					g_DoppleganerEvent.SendDoppelgangerResult(lpObj, 1);
				}
			}
		}
#endif

//#if(_GSCS==1)
	if(result == 97)
	{
		if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			if(lpObj->m_btCsJoinSide != 1)
			{
				if(g_CastleSiege.GetCastleTowerAccessable() == FALSE)
				{
					::GCServerMsgStringSend(lMsg.Get(1624),lpObj->m_Index,1);

					x = lpObj->X;
					y = lpObj->Y;
					mapNumber = lpObj->MapNumber;
					dir = lpObj->Dir;
					movefail = 1;
					lpObj->RegenMapNumber = lpObj->MapNumber;
					lpObj->RegenMapX = x;
					lpObj->RegenMapY = y;
					::gObjClearViewport(&gObj[aIndex]);
					::GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

					if(lpObj->m_Change >=0)
					{
						gObjViewportListProtocolCreate(&gObj[aIndex]);
					}

					gObj[aIndex].RegenOk = 1;
					return false;
				}
			}
			
			if(lpObj->m_btCsJoinSide == 0)
			{
				::GCServerMsgStringSend(lMsg.Get(1625),lpObj->m_Index,1);

				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				::gObjClearViewport(&gObj[aIndex]);
				::GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
		else if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE )
			{
				::GCServerMsgStringSend(lMsg.Get(1626),lpObj->m_Index,1);

				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;
				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				::gObjClearViewport(&gObj[aIndex]);
				::GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
						
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk = 1;
				return false;
			}
	}
//#endif

	if(mapNumber == MAP_INDEX_ATHLANSE)
	{
		if(lpObj->pInventory[8].IsItem())
		{
			if(lpObj->pInventory[8].m_Type == ITEMGET(13,2))
			{
				GCServerMsgStringSend(lMsg.Get(702),lpObj->m_Index,1);

				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk = 1;
				return false;
			}

			if(lpObj->pInventory[8].m_Type == ITEMGET(13,3))
			{
				GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);

				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				gObjClearViewport(&gObj[aIndex]);
				GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
	}

	if(mapNumber == MAP_INDEX_ICARUS)
	{
		if(!(lpObj->pInventory[8].m_Type == ITEMGET(13,3)
			|| lpObj->pInventory[7].m_Type == ITEMGET(13,30)
			|| lpObj->pInventory[8].m_Type == ITEMGET(13,37)
			|| lpObj->pInventory[8].m_Type == ITEMGET(13,4) //season4 add-on (fix)
			|| lpObj->pInventory[7].IsItem() != FALSE)
			|| lpObj->pInventory[8].m_Type == ITEMGET(13,2)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,40) //Second Edition
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,41) //Season 2.5 add-on
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,41))
		{
			GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);

			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;

			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			
			GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
			gObjClearViewportOfMine(lpObj);

			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			gObj[aIndex].RegenOk = 1;
			return false;
		}

		if(lpObj->m_RecallMon >= 0)
		{
			GCRecallMonLife(aIndex,60,0);
			gObjMonsterCallKill(aIndex);
		}
	}

	if(!KALIMA_MAP_RANGE(mapNumber) && KALIMA_MAP_RANGE(lpObj->MapNumber))
	{
		int iKalimaGateIndex = lpObj->m_iKalimaGateIndex;

		if(g_KalimaGate.DeleteKalimaGate(lpObj->m_Index))
		{
			LogAddTD("[Kalima] [%s][%s] Kalima Gate Vanished - User MoveGate (SummonIndex:%d, EnterCount:%d)",lpObj->AccountID,lpObj->Name,iKalimaGateIndex,gObj[iKalimaGateIndex].m_cKalimaGateEnterCount);
		}
	}

//#if(_GSCS==0)
	if(mapNumber == MAP_INDEX_KANTURU_BOSS)
	{
		if(!(lpObj->pInventory[8].m_Type == ITEMGET(13,3)
			|| lpObj->pInventory[8].m_Type == ITEMGET(13,37)
			|| lpObj->pInventory[7].IsItem()) //No need for Wing Macro *-*
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,40) //Second Edition
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,41) //Season 3 add-on (its a fix)
			|| lpObj->pInventory[12].m_Type == ITEMGET(13,41))
		{
			GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);

			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;

			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			
			GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
			gObjClearViewportOfMine(lpObj);

			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			gObj[aIndex].RegenOk = 1;
			return false;
		}
	}
//#endif

	if(g_iBlockKanturuMapEnter == 1)
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		level = lpObj->Level;

		if(gGateC.GetGate(gt,(short &)x,(short &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) != -1)
		{
			if(mapNumber >= MAP_INDEX_KANTURU1 && mapNumber <= MAP_INDEX_KANTURU_BOSS)
			{
				GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				
				GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				gObjClearViewportOfMine(lpObj);

				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
	}

	if(g_iBlockCastleSiegeMapEnter == 1)
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		level = lpObj->Level;

		if(gGateC.GetGate(gt,(short &)x,(short &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) != -1)
		{
			if(mapNumber == MAP_INDEX_CASTLESIEGE
				|| mapNumber == MAP_INDEX_CASTLEHUNTZONE
				|| mapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE
				|| mapNumber == MAP_INDEX_CRYWOLF_SECONDZONE)
			{
				GCServerMsgStringSend(lMsg.Get(1604),lpObj->m_Index,1);
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				
				GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				gObjClearViewportOfMine(lpObj);

				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
	}

	if(mapNumber == MAP_INDEX_RAKLION_BOSS) //season4 add-on
	{
		if(g_Raklion.GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR)
		{
			GCServerMsgStringSend(lMsg.Get(3415),lpObj->m_Index,1);
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;

			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
				
			GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
			gObjClearViewportOfMine(lpObj);

			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			gObj[aIndex].RegenOk = 1;
			return false;
		}

		if(gt == 290)
		{
			g_RaklionBattleUserMng.AddUserData(aIndex);
		}
	}

	if(mapNumber == MAP_INDEX_RAKLION_FIELD) //season4.0 add-on
	{
		if(gt == 292)
		{
			if(g_Raklion.GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR)
			{
				GCServerMsgStringSend(lMsg.Get(3416),lpObj->m_Index,1);
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
					
				GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				gObjClearViewportOfMine(lpObj);

				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
			g_RaklionBattleUserMng.DeleteUserData(aIndex);
		}
		else
		{
			g_RaklionUtil.NotifyRaklionCurrentState(aIndex, g_Raklion.GetRaklionState(), g_Raklion.GetRaklionDetailState());
		}
	}

	if(mapNumber != MAP_INDEX_SWAMP_OF_CALMNESS)
	{
		if(lpObj->Class == CLASS_DARKLORD 
#ifdef MONK
			|| lpObj->Class == CLASS_MONK
#endif
			|| lpObj->Class == CLASS_MAGUMSA )
		{
			if(level > 0)
			{
				level = (level / 3)*2;
			}
		}
	}

	if(level != 0)
	{
		if(userlevel < level)
		{
			char szTemp[256];
			wsprintf(szTemp,lMsg.Get(1117),level);
			GCServerMsgStringSend(szTemp,aIndex,1);

			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;
		}
	}

	if(gObj[aIndex].m_IfState.use != 0)
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;
	}

	if(gObj[aIndex].DieRegen != 0)
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;
	}

	lpObj->m_State = 32;

	if(movefail == 0)
	{
		if(gGateC.GetGate(gt,(short &)x,(short &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) != -1)
		{
			if(gObj[aIndex].MapNumber != mapNumber)
			{
				if(MapNumberCheck(mapNumber)== 0)
				{
					LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
					return false;
				}

				short sSvrCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,mapNumber,lpObj->m_sPrevMapSvrCode);

				if(sSvrCode != gGameServerCode)
				{
					if(sSvrCode == -1)
					{
						LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);
						return false;
					}

					if(gGateC.CheckGateLevel(aIndex,gt) == 0)
					{
						x = lpObj->X;
						y = lpObj->Y;
						mapNumber = lpObj->MapNumber;
						dir = lpObj->Dir;
						movefail = 1;

						lpObj->RegenMapNumber = lpObj->MapNumber;
		
						lpObj->RegenMapX = x;
						lpObj->RegenMapY = y;
				
						gObjClearViewport(&gObj[aIndex]);
				
						GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
		
						if(lpObj->m_Change >= 0)
						{
							gObjViewportListProtocolCreate(&gObj[aIndex]);
						}
	
						gObj[aIndex].RegenOk = 1;

//#if(_GSCS==1)
						if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
						{
							if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
							{
								g_CastleSiege.NotifySelfCsJoinSide(aIndex);
								g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
							}
						}
//#endif
						return false;
					}


					GJReqMapSvrMove(lpObj->m_Index,sSvrCode,mapNumber,x,y);
					LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",sSvrCode,lpObj->AccountID,lpObj->Name,lpObj->m_Index);
					return false;
				}
			}
		}

		LogAddTD("[MoveMap][%s][%s] : (Before:Map:%d X:%d Y:%d) -> (New Map:%d X:%d Y:%d)",lpObj->AccountID,lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y,mapNumber,x,y);

		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;
		gObj[aIndex].TX = x;
		gObj[aIndex].TY = y; //season4 changed (fix)
		gObj[aIndex].MapNumber = mapNumber;
		gObj[aIndex].Dir = dir;
		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport = 0;

		gObjViewportListProtocolDestroy(&gObj[aIndex]); //season4 add-on

		GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
		
		gObjViewportListProtocolCreate(&gObj[aIndex]); //season4 add-on

		gObjClearViewport(&gObj[aIndex]);
		

		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenMapNumber = mapNumber;
		gObj[aIndex].RegenMapX = x;
		gObj[aIndex].RegenMapY = y;
		gObj[aIndex].RegenOk = 1;
	}
	else
	{
		GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
		gObj[aIndex].RegenOk = 1;
		
//#if(_GSCS==1)
		if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				g_CastleSiege.NotifySelfCsJoinSide(aIndex);
				g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
			}
		}
//#endif

	}

	if(movefail != 0)
	{
		return false;
	}

	if(gObj[aIndex].Type == OBJ_USER)
	{
		gObj[aIndex].m_LastTeleportTime = 10;
	}

#ifdef NPVP
	g_NewPVP.Reset(*lpObj); // 1.00.92
#endif

	return true;
}

void gObjTeleport(int aIndex, int map, int x, int y)
{

	if(aIndex < 0 || aIndex > OBJMAX - 1)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	if(MapNumberCheck(map)==0)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(gObj[aIndex].MapNumber != map)
	{
		short sSvrCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,map,lpObj->m_sPrevMapSvrCode);

		if(sSvrCode != gGameServerCode)
		{
			if(sSvrCode == -1)
			{
				LogAddC(2,"[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);
				return;
			}

			GJReqMapSvrMove(lpObj->m_Index,sSvrCode,map,x,y);
			LogAddTD("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",sSvrCode,lpObj->AccountID,lpObj->Name,lpObj->m_Index);
			return;
		}
	}

	if(lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 1)
	{
		CGTradeCancelButtonRecv(lpObj->m_Index);
	}

	if(lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 6)
	{
		CGWarehouseUseEnd(lpObj->m_Index);
	}

	lpObj->m_State = 32;
	gObj[aIndex].X = x;
	gObj[aIndex].Y = y;

	gObj[aIndex].TX = x;
	gObj[aIndex].TX = y;

	gObj[aIndex].MapNumber = map;
	gObj[aIndex].PathCount = 0;
	gObj[aIndex].Teleport = 0;

	gObjClearViewport(&gObj[aIndex]);

	GCTeleportSend(&gObj[aIndex],1,map,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

	if(lpObj->m_Change >= 0)
	{
		gObjViewportListProtocolCreate(&gObj[aIndex]);
	}

	gObj[aIndex].RegenMapNumber = map;
	gObj[aIndex].RegenMapX = x;
	gObj[aIndex].RegenMapY = y;
	gObj[aIndex].RegenOk = 1;
}

void gObjMoveDataLoadingOK(int aIndex)
{
	if(gObj[aIndex].RegenOk == 1)
	{
		gObj[aIndex].RegenOk = 2;
	}
}

class CMagicInf * gObjGetMagic(LPOBJ lpObj, int mIndex)
{
	if ( mIndex < 0 || mIndex > MAX_MAGIC-1 )
	{
		LogAdd(lMsg.Get(MSGGET(2, 34)), __FILE__, __LINE__);
		return NULL;
	}

	if ( lpObj->Magic[mIndex].IsMagic() == FALSE )
	{
		return NULL;
	}

	return &lpObj->Magic[mIndex];
}

CMagicInf * gObjGetMagicSearch(LPOBJ lpObj, int skillnumber)
{
	if(skillnumber == 58)
	{
		skillnumber = 40;
	}

	for(int n = 0; n < MAGIC_SIZE;n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == skillnumber)
			{
				if(gObjMagicEnergyCheck(lpObj,lpObj->Magic[n].m_Skill) == 0)
				{
					GCServerMsgStringSend(lMsg.Get(1255),lpObj->m_Index,1);
					return false;
				}
				else
				{
					return &lpObj->Magic[n];
				}
			}
		}
	}
	return false;
}

int	gObjMagicManaUse(LPOBJ lpObj, CMagicInf * lpMagic)
{
	float mana = lpObj->Mana;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpMagic == 0)
	{
		return false;
	}

	float loc2 = 0; //season4 add-on

	loc2 = MagicDamageC.SkillGetMana(lpMagic->m_Skill);
	loc2 -= lpObj->m_MPSkillOpt.MpsDecreaseMana * loc2 / 100.0f;

	if(loc2 < 1.0f)
	{
		loc2 = 1.0f;
	}

	mana -= loc2;

	if(mana < 0)
	{
		return -1;
	}

	return mana;
}






int  gObjMagicBPUse(LPOBJ lpObj, CMagicInf * lpMagic)
{
	int bp = lpObj->BP;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpMagic == 0)
	{
		return false;
	}

	int loc2 = MagicDamageC.SkillGetBP(lpMagic->m_Skill);

	if(lpObj->m_btAGReduceRate > 0)
	{
		loc2 -= loc2 * lpObj->m_btAGReduceRate / 100;
	}

	bp -= loc2;

	if(bp < 0)
	{
		return -1;
	}

	return bp;
}



struct PMSG_POSMAGIC_RESULT
{
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_POSMAGIC_COUNT
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE MagicNumber;
	BYTE MapX;
	BYTE MapY;
	BYTE Count;
};

int gObjPosMagicAttack(LPOBJ lpObj, CMagicInf * lpMagic, BYTE x, BYTE y)
{
	int tObjNum;
	int dis;
	int tx;
	int ty;
	int MagicDistance;
	BYTE sbuf[256];
	int lOfs;
	BYTE MagicNumber;
	PMSG_POSMAGIC_COUNT pCount;
	PMSG_POSMAGIC_RESULT pResult;
	int MVL;
	int n;

	MagicDistance = 3;
	lOfs = 0;
	MagicNumber = lpMagic->m_Skill;

	switch(MagicNumber)
	{
	case 9: MagicDistance = 3;
		break;
	default: MagicDistance = 3;
		break;
	}

	lOfs = sizeof(pCount);
	pCount.h.c = 0xC1;
	pCount.h.headcode = 0x1A;
	pCount.h.size = 0;

	pCount.MapX = x;
	pCount.MapY = y;
	pCount.MagicNumber = MagicNumber;
	pCount.NumberH = SET_NUMBERH(lpObj->m_Index);
	pCount.NumberL = SET_NUMBERL(lpObj->m_Index);
	pCount.Count = 0;

	MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state)
		{
			tObjNum = lpObj->VpPlayer[n].number;

			if(tObjNum >= 0)
			{
				if(gObj[tObjNum].Live)
				{
					tx = lpObj->X - gObj[tObjNum].X;
					ty = lpObj->Y - gObj[tObjNum].Y;

					dis = sqrtf(tx*tx + ty*ty);

					if(dis <= MagicDistance)
					{
						
						if(gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,0,0,0, 0, 0) == 1)
						{
							pResult.NumberH = SET_NUMBERH(tObjNum);
							pResult.NumberL = SET_NUMBERL(tObjNum);

							memcpy(&sbuf[lOfs],&pResult,sizeof(pResult));
							pCount.Count++;
							lOfs+=sizeof(pResult);

							if(lOfs > 250)
							{
								LogAdd(lMsg.Get(547),__FILE__,__LINE__);
								return false;
							}
						}
					}
				}
			}
		}
	}

	pCount.h.size = lOfs;

	memcpy(sbuf,&pCount,sizeof(pCount));

	if(lpObj->Type == OBJ_USER)
	{
		DataSend(lpObj->m_Index,sbuf,lOfs);
	}

	MsgSendV2(lpObj,sbuf,lOfs);
	return true;
}

void gObjMagicAddEnergyCheckOnOff(int flag)
{
	g_EnergyCheckOff=flag;
}

int gObjMagicAdd(LPOBJ lpObj, BYTE Type, BYTE Index, BYTE Level, short & SkillNumber)
{
	int skill = -1,n,reqeng,reqleadership;

	skill = GetSkillNumberInex(Type,Index,Level);

	if(skill < 0)
	{
		LogAdd(lMsg.Get(548));
		return -1;
	}

	if(g_EnergyCheckOff == 1)
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy(skill);

		if(reqeng < 0)
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}

		if(lpObj->Class == CLASS_KNIGHT) //season 4 add-on
		{
			reqeng -= 10;
		}

		if(lpObj->Class == CLASS_ELF) //season 4 add-on
		{
			reqeng -= 5;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}
	}

	reqleadership = MagicDamageC.GetskillRequireLeadership(skill);

	if(reqleadership < 0)
	{
		LogAdd("error-L2: Skill Leadership not enough");
		return -1;
	}

	if((lpObj->Leadership + lpObj->AddLeadership) < reqleadership)
	{
		LogAdd("error-L2: Skill Leadership not enough");
		return -1;
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,lpObj->ThirdChangeUp,skill) < 1)
	{
		LogAdd(lMsg.Get(550));
		return -1;
	}

	if( g_MasterSkillSystem.IsExistMLSBasedSkill(lpObj, skill) == 1 )
	{
		return -1;
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			skill = GetSkillNumberInex(Type,Index,Level);

			if(skill < 0)
			{
				LogAdd(lMsg.Get(551),__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->Magic[n].m_Skill == skill)
			{
				LogAdd(lMsg.Get(552));
				return -1;
			}
		}
	}

	skill = -1;

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(Type,Index,Level);
			if(skill < 0)
			{
				LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				SkillNumber = skill;
				return n;
			}
		}
	}


	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}

int gObjMagicDel(LPOBJ lpObj, DWORD aSkill, BYTE Level) //
{
	if(g_MasterSkillSystem.MLS_WeaponSkillDel(lpObj, (BYTE)aSkill, Level) != FALSE) //season4 add-on
	{
		return -1;
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				lpObj->Magic[n].Clear();
				return n;
			}
		}
	}
	return -1;
}

//00596490
int gObjMagicAdd(LPOBJ lpObj, int aSkill, BYTE Level)
{
	int skill = -1,n,reqeng;

	skill = -1;	// nice trick :DD

	if( g_EnergyCheckOff == 1 )
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

		if(reqeng < 0)
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}

		if(lpObj->Class == CLASS_KNIGHT) //season 4 add-on
		{
			reqeng -= 10;
		}

		if(lpObj->Class == CLASS_ELF) //season 4 add-on
		{
			reqeng -= 5;
		}

		if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
		{
			LogAdd(lMsg.Get(549));
			return -1;
		}
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,lpObj->ThirdChangeUp,aSkill) < 1)
	{
		LogAdd(lMsg.Get(550));
		return -1;
	}

	if(g_MasterSkillSystem.IsExistMLSBasedSkill(lpObj, skill) == 1)
	{
		return -1;
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				//LogAdd(lMsg.Get(555),lpObj->Magic[n].m_Skill,aSkill);
				return -1;
			}
		}
	}

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill,Level);
			if(skill < 0)
			{
				LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				return n;
			}
		}
	}

	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}

//00596860
int gObjWeaponMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill = -1,n;
	skill = -1;

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->ChangeUP,lpObj->ThirdChangeUp,(WORD)aSkill) < 1)
	{
		return -1;
	}

	if(g_MasterSkillSystem.MLS_WeaponSkillAdd(lpObj,(BYTE)aSkill, Level) != FALSE)
	{
		return -1;
	}

	/*for( n = 0; n < MAGIC_SIZE; n++ )	//fix for add two identical skills
	{
		if( lpObj->Magic[n].IsMagic() && lpObj->Magic[n].m_Skill == aSkill )
		{
			return -1;
		}
	}

	for( int n = 0; n < MAX_MAGIC; n++ )	//fix for add brand
	{
		if( lpObj->Magic[n].IsMagic() && MagicDamageC.GetSkillUseType(lpObj->Magic[n].m_Skill) == 4 )
		{
			WORD Brand = MagicDamageC.GetBrandOfSkill(lpObj->Magic[n].m_Skill);
			// ----
			if( Brand > 0 )
			{
				for( int i = 0; i < MAX_MAGIC; i++ )
				{
					if( lpObj->Magic[i].IsMagic() && lpObj->Magic[i].m_Skill == Brand )
					{
						return -1;
					}
				}
			}
		}
	}*/

	for(n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill,Level);

			if(skill < 0)
			{
				LogAdd(lMsg.Get(553),__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				return n;
			}
		}
	}
	LogAdd(lMsg.Get(554),__FILE__,__LINE__);
	return -1;
}

int gObjMonsterMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill = -1;

	for (int n=0; n<MAGIC_SIZE ; n++)
	{
		if ( lpObj->Magic[n].IsMagic() == FALSE )
		{
			skill = lpObj->Magic[n].Set(aSkill, Level);

			if ( skill < 0 )
			{
				LogAdd(lMsg.Get( MSGGET(2, 41)),__FILE__, __LINE__);
				return -1;
			}

			lpObj->MagicCount++;
			return n;
		}
	}

	LogAdd(lMsg.Get( MSGGET(2, 42)), __FILE__, __LINE__);
	return -1;
}

BOOL gObjMagicEnergyCheck(LPOBJ lpObj , WORD aSkill)
{
	int reqeng,reqlevel;

	reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

	if(reqeng < 0)
	{
		return false;
	}

	if(lpObj->Class == CLASS_KNIGHT)
	{
		reqeng -= 10;
	}

	if(lpObj->Class == CLASS_ELF) //season4 add-on
	{
		reqeng -= 5;
	}

	if((lpObj->Energy + lpObj->AddEnergy) < reqeng)
	{
		return false;
	}

	reqlevel = MagicDamageC.SkillGetRequireLevel(aSkill);

	if(lpObj->Level < reqlevel)
	{
		return false;
	}

	return true;
}

BOOL gObjItemLevelUp(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(target < 0 || target > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].m_Type == ITEMGET(13,37))
	{
		CItem * ItemFenrir = &lpObj->pInventory[target];

		if(ItemFenrir->m_Durability >= 255)
		{
			return false;
		}

		if(rand()% 10000 < g_iFenrirRepairRate)
		{
			int iAddDur = rand()%150 + 50;

			if((ItemFenrir->m_Durability + iAddDur) > 255)
			{
				ItemFenrir->m_Durability = 255.0f;
			}
			else
			{
				ItemFenrir->m_Durability += iAddDur;
			}

			MsgOutput(lpObj->m_Index,lMsg.Get(3342),int(ItemFenrir->m_Durability));

			LogAddTD("[FENRIR REPAIR][SUCCESS] [%s][%s] - %d/255 (+%d)",lpObj->AccountID,lpObj->Name,ItemFenrir->m_Durability,iAddDur);
		}
		else
		{
			MsgOutput(lpObj->m_Index,lMsg.Get(3343));

			LogAddTD("[FENRIR REPAIR][FAILED] [%s][%s] - %d/255 (+%d)",lpObj->AccountID,lpObj->Name,ItemFenrir->m_Durability);
		}

		return true;
	}
	
	//season 3.5 changed (added summoner wings)
	if(!( (lpObj->pInventory[target].m_Type < ITEMGET(12,7) || lpObj->pInventory[target].m_Type >=  ITEMGET(12,36)) &&
		  (lpObj->pInventory[target].m_Type <= ITEMGET(12,43) || lpObj->pInventory[target].m_Type == ITEMGET(13,30)
		  || lpObj->pInventory[target].m_Type == ITEMGET(12, 49) || lpObj->pInventory[target].m_Type == ITEMGET(12, 50)
#ifdef NEWWINGS
		  || IS_NEWWINGS(lpObj->pInventory[target].m_Type)
#endif
		  ) &&
		  (lpObj->pInventory[target].m_Type != ITEMGET(4,7) && lpObj->pInventory[target].m_Type != ITEMGET(4,15)) ))
	{
		return false;
	}

	if( g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[target].m_Type) )	//1.01.00
	{
		return false;
	}

	LogAddTD(lMsg.Get(556),lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	int _r = rand()%100;

	if(lpObj->pInventory[target].m_Option2 != 0)
	{
		_r -= 20;	//Luck
	}

	if( _r > gBlessSuccessRate )
	{
		lpObj->pInventory[target].m_Level--;

		if( lpObj->pInventory[target].m_Level < 0 )
		{
			lpObj->pInventory[target].m_Level = 0;
		}
	}
	else
	{
		lpObj->pInventory[target].m_Level++;

		if(lpObj->pInventory[target].m_Level > 6)
		{
			lpObj->pInventory[target].m_Level = 6;
		}
	}

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_Option1,
			lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			NULL, 0xFF, 0, CURRENT_DB_VERSION);

	LogAddTD(lMsg.Get(557),lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number, lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);
	return true;
}

BOOL gObjItemRandomLevelUp(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(target < 0 || target > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	//season 3.5 changed (added summoner wings)
	if(!( (lpObj->pInventory[target].m_Type < ITEMGET(12,7) || lpObj->pInventory[target].m_Type >=  ITEMGET(12,36)) &&
		  (lpObj->pInventory[target].m_Type <= ITEMGET(12,43) || lpObj->pInventory[target].m_Type == ITEMGET(13,30)
		  || lpObj->pInventory[target].m_Type == ITEMGET(12, 49)
		  || lpObj->pInventory[target].m_Type == ITEMGET(12, 50)
#ifdef NEWWINGS
		  || IS_NEWWINGS(lpObj->pInventory[target].m_Type)
#endif
		  ) &&
		  (lpObj->pInventory[target].m_Type != ITEMGET(4,7) && lpObj->pInventory[target].m_Type != ITEMGET(4,15)) ))
	{
		return false;
	}

	if( g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[target].m_Type) )	//1.01.00
	{
		return false;
	}

	if(lpObj->pInventory[target].m_Level >= 9)
	{
		return false;
	}

	LogAddTD(lMsg.Get(558), lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(), lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	int _r = rand()%100;

	//season4 add-on start
	bool bSuccess = false;

	if(lpObj->pInventory[target].m_Option2 != 0)
	{
		_r -= 20;	//Luck
	}

	if(_r < gSoulSuccessRate)
	{
		bSuccess = true;
	}

	if(bSuccess == 1)
	{
		lpObj->pInventory[target].m_Level++;
	}
	else
	{
		if(lpObj->pInventory[target].m_Level >= 7)
		{
			lpObj->pInventory[target].m_Level = 0;
		}
		else
		{
			lpObj->pInventory[target].m_Level--;

			if(lpObj->pInventory[target].m_Level < 0)
			{
				lpObj->pInventory[target].m_Level = 0;
			}
		}
	}
	//season 4 add-on end

	if(lpObj->pInventory[target].m_Level > 9)
	{
		lpObj->pInventory[target].m_Level = 9;
	}
	else
	{
		gObjMakePreviewCharSet(lpObj->m_Index);
	}

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_Option1,
			lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			NULL, 0xFF, 0, CURRENT_DB_VERSION);

	LogAddTD(lMsg.Get(559),lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number, lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[target])== 1)
	{
		if(g_kJewelOfHarmonySystem.IsActive(&lpObj->pInventory[target]) == 0)
		{
			GCServerMsgStringSend(lMsg.Get(3370),lpObj->m_Index,1);
		}
	}

	return true;
}

BOOL gObjItemRandomOption3Up(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(target < 0 || target > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	//season 3.5 changed (added summoner wings)
	if(!( (lpObj->pInventory[target].m_Type < ITEMGET(12,7) || lpObj->pInventory[target].m_Type >=  ITEMGET(12,36)) &&
		  (lpObj->pInventory[target].m_Type <= ITEMGET(12,43) || lpObj->pInventory[target].m_Type == ITEMGET(13,30)
		  || lpObj->pInventory[target].m_Type == ITEMGET(12, 49)
		  || lpObj->pInventory[target].m_Type == ITEMGET(12, 50)
#ifdef NEWWINGS
		  || IS_NEWWINGS(lpObj->pInventory[target].m_Type)
#endif
		  ) &&
		  (lpObj->pInventory[target].m_Type != ITEMGET(4,7) && lpObj->pInventory[target].m_Type != ITEMGET(4,15)) ))
	{
		return false;
	}

	LogAddTD(lMsg.Get(576), lpObj->AccountID,lpObj->Name, lpObj->pInventory[target].GetName(), lpObj->pInventory[target].m_Number, lpObj->pInventory[target].m_Option3);

	int _r = rand()%100;

	if(lpObj->pInventory[target].m_Option2 != 0)
	{
		_r -= 20;	//Luck
	}

	int loc2;
	int loc3;

	if(lpObj->pInventory[target].m_Option3 == 0)
	{
		if(lpObj->pInventory[target].m_Type >= ITEMGET(12,3) && lpObj->pInventory[target].m_Type <= ITEMGET(12,6) 
			|| lpObj->pInventory[target].m_Type == ITEMGET(12, 42)
			|| lpObj->pInventory[target].m_Type == ITEMGET(12, 49))
		{
			lpObj->pInventory[target].m_NewOption &= 0xDF;

			if(rand()%2)
			{
				lpObj->pInventory[target].m_NewOption |= 0x20;
			}
		}
	}
#if(GS_LIFE_28_UP == 1)
	if(lpObj->pInventory[target].m_Option3 < 7)
#else
	if(lpObj->pInventory[target].m_Option3 < 4)
#endif
	{
		if( _r > gLifeSuccessRate ) // gLifeAdd, T_T ïèçäåö ïðîñòî à íå ñèñòåìà
		{
			lpObj->pInventory[target].m_Option3 = 0;
		}
		else
		{
			if( (lpObj->pInventory[target].m_Option3) == FALSE)
			{
				if(lpObj->pInventory[target].m_Type >= ITEMGET(12,36) && lpObj->pInventory[target].m_Type <= ITEMGET(12,40) 
					|| lpObj->pInventory[target].m_Type == ITEMGET(12, 43)
					|| lpObj->pInventory[target].m_Type == ITEMGET(12, 50)
#ifdef NEWWINGS
					|| IS_NEWWINGS(lpObj->pInventory[target].m_Type)
#endif
					)
				{
					lpObj->pInventory[target].m_NewOption &= 0xEF;
					lpObj->pInventory[target].m_NewOption &= 0xDF;
					
					loc2 = rand()%2;

					switch(loc2)
					{
						case 0:
						{
							loc3 = rand()%1000;

							if(loc3 < 400)
							{
								lpObj->pInventory[target].m_NewOption |= 0x10;
							}
						}
						break;
						
						case 1:
						{
							loc3 = rand()%1000;

							if(loc2 < 300)
							{
								lpObj->pInventory[target].m_NewOption |= 0x20;
							}
						}
						break;
					}
				}
			}
			lpObj->pInventory[target].m_Option3++;
		}
	}
	else
	{
		return false;
	}

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_Option1,
			lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			NULL, 0xFF, 0, CURRENT_DB_VERSION);

	LogAddTD(lMsg.Get(577),lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number, lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Option3);

	BYTE NewOption[8];

	ItemIsBufExOption(NewOption,&lpObj->pInventory[target]);

	LogAddTD("[%s][%s] JewelofLife Result [%d,%s,%d,%d,%d,%d]serial:[%u] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d]",
			lpObj->AccountID,lpObj->Name,target,lpObj->pInventory[target].GetName(),
			lpObj->pInventory[target].m_Level,lpObj->pInventory[target].m_Option1,lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,lpObj->pInventory[target].m_Number,
			BYTE(lpObj->pInventory[target].m_Durability),NewOption[0],NewOption[1],NewOption[2],NewOption[3],
			NewOption[4],NewOption[5],NewOption[6]);

	return true;
}

int	GetRandomExeOption(int MaxOption)
{
	unsigned char ExeOptions[6] = {1,2,4,8,16,32};
	// ----
	int Random = 0;
	int RandomEx = 0;
	int ReturnValue = 0;

	Random = rand()%(MaxOption+1);

	for (int i=0;i<Random;i++)
	{
		while ( true )
		{
			RandomEx = ExeOptions[rand()%6];

			if ( (ReturnValue&RandomEx) == 0 )
			{
				ReturnValue+=RandomEx;
				break;
			}
		}
	}
	return ReturnValue;
}

int GenExcOpt(int amount)
{
	// User input errors
	if (amount > 6) amount = 6;
	if (amount < 1) amount = 1;

	int opt_db[6]  = {1, 2, 4, 8, 16, 32};
	int exc = 0;

	std::random_shuffle(opt_db, opt_db + 6);

	for(int n=0; n < amount; n++)
		exc += opt_db[n];

	return exc;
}

void gObjAbilityReSet(LPOBJ lpObj)
{
	lpObj->LevelUpPoint = (lpObj->Level - 1) * 5;

	lpObj->Strength = DCInfo.DefClass[lpObj->Class].Strength;
	lpObj->Dexterity = DCInfo.DefClass[lpObj->Class].Dexterity;
	lpObj->Vitality = DCInfo.DefClass[lpObj->Class].Vitality;
	lpObj->Energy = DCInfo.DefClass[lpObj->Class].Energy;

	lpObj->MaxLife = DCInfo.DefClass[lpObj->Class].MaxLife;
	lpObj->MaxMana = DCInfo.DefClass[lpObj->Class].MaxMana;

	if(lpObj->Class == CLASS_WIZARD)
	{
		lpObj->MaxLife += (lpObj->Level -1)*0.5f;
		lpObj->MaxMana += 2.0f * (lpObj->Level -1);

		lpObj->Life = lpObj->MaxLife;
		lpObj->Mana = lpObj->MaxMana;
	}
	else
	{
		lpObj->MaxLife += 2.0f * (lpObj->Level - 1);
		lpObj->MaxMana += (lpObj->Level - 1)*0.5f;

		lpObj->Life = lpObj->MaxLife;
		lpObj->Mana = lpObj->MaxMana;
	}

	CloseClient(lpObj->m_Index);
}

BOOL gObjTargetGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpTargetObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->GuildNumber < 1 || lpTargetObj->GuildNumber < 1)
	{
		return false;
	}

	if(lpObj->lpGuild->WarState != 1 || lpTargetObj->lpGuild->WarState != 1)
	{
		return false;
	}

	if(strcmp(lpObj->lpGuild->TargetGuildName,lpTargetObj->lpGuild->Name))
	{
		return false;
	}
	return true;
}

void gObjGuildWarEndSend(LPOBJ lpObj, BYTE Result1, BYTE Result2)
{
	if(lpObj == 0)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->lpGuild == 0)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->lpGuild->WarState != 1)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->lpGuild->lpTargetGuildNode == 0)
	{
		LogAdd("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(Result1 == 1)
	{
		gObjGuildWarItemGive(lpObj->lpGuild,lpObj->lpGuild->lpTargetGuildNode);
	}
	else if(Result1 == 3)
	{
		gObjGuildWarItemGive(lpObj->lpGuild->lpTargetGuildNode,lpObj->lpGuild);
	}
	else if(Result1 == 1)
	{
		gObjGuildWarItemGive(lpObj->lpGuild->lpTargetGuildNode,lpObj->lpGuild);
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpObj->lpGuild->Use[n] && lpObj->lpGuild->Index[n] >= 0)
		{
			gObj[lpObj->lpGuild->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpObj->lpGuild->Index[n],Result1,lpObj->lpGuild->lpTargetGuildNode->Name);
		}
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpObj->lpGuild->lpTargetGuildNode->Use[n] && lpObj->lpGuild->lpTargetGuildNode->Index[n] >= 0)
		{
			gObj[lpObj->lpGuild->lpTargetGuildNode->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpObj->lpGuild->lpTargetGuildNode->Index[n],Result2,lpObj->lpGuild->Name);
		}
	}
}

void gObjGuildWarEndSend(_GUILD_INFO_STRUCT * lpGuild1, _GUILD_INFO_STRUCT * lpGuild2, BYTE Result1, BYTE Result2)
{
	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return;
	}

	if(lpGuild1->WarState != 1 || lpGuild2->WarState != 1)
	{
		return;
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpGuild1->Use[n] && lpGuild1->Index[n] >= 0)
		{
			gObj[lpGuild1->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpGuild1->Index[n],Result1,lpGuild2->Name);
		}
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpGuild2->Use[n] && lpGuild2->Index[n] >= 0)
		{
			gObj[lpGuild2->Index[n]].IsInBattleGround = 0;
			GCGuildWarEnd(lpGuild2->Index[n],Result2,lpGuild1->Name);
		}
	}
}

void gObjGuildWarEnd(_GUILD_INFO_STRUCT * lpGuild, _GUILD_INFO_STRUCT * lpTargetGuild)
{
	if(lpGuild == 0 || lpTargetGuild == 0)
	{
		return;
	}

	lpGuild->WarDeclareState = 0;
	lpGuild->WarState = 0;

	if(lpTargetGuild != 0)
	{
		lpTargetGuild->WarDeclareState = 0;
		lpTargetGuild->WarState = 0;

		if(lpGuild->lpTargetGuildNode != 0)
		{
			_GUILD_INFO_STRUCT * lpTguild = lpGuild->lpTargetGuildNode;

			lpGuild->lpTargetGuildNode = 0;
			lpTguild->lpTargetGuildNode = 0;
		}
	}
	else
	{
		lpGuild->lpTargetGuildNode = 0;
	}
}

BOOL gObjGuildWarProc(_GUILD_INFO_STRUCT * lpGuild1, _GUILD_INFO_STRUCT * lpGuild2, int score)
{
	int totalscore;
	int n;
	int maxscore;
	char szTemp[0x100];

	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return false;
	}

	totalscore = 1;
	maxscore = 20;

	lpGuild1->PlayScore += score;

	wsprintf(szTemp,"%s ( %d ) VS %s ( %d )",lpGuild1->Name,lpGuild1->PlayScore,lpGuild2->Name,lpGuild2->PlayScore);
	LogAddTD(szTemp);

	if(lpGuild1->WarType == 1)
	{
		SetBattleTeamScore(lpGuild1->BattleGroundIndex,lpGuild1->BattleTeamCode,lpGuild1->PlayScore);
		maxscore = 100;
	}
	else
	{
		maxscore = 20;
	}

	cManager.BattleInfoSend(GetBattleTeamName(0,0),GetBattleTeamScore(0,0),GetBattleTeamName(0,1),GetBattleTeamScore(0,1));

	if(lpGuild1->PlayScore >= maxscore)
	{
		if(lpGuild1->PlayScore > maxscore && lpGuild2->PlayScore == 0)
		{
			totalscore = 3;
		}
		else if(lpGuild1->PlayScore > maxscore && lpGuild2->PlayScore <= 10)
		{
			totalscore = 2;
		}

		lpGuild1->TotalScore += totalscore;
		DGGuildScoreUpdate(lpGuild1->Name,lpGuild1->TotalScore);
		DGGuildScoreUpdate(lpGuild2->Name,lpGuild2->TotalScore);

		return true;
	}

	for(n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpGuild1->Use[n] && lpGuild1->Index[n] >= 0)
		{
			GCGuildWarScore(lpGuild1->Index[n]);
		}
	}

	for(n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpGuild2->Use[n] && lpGuild2->Index[n] >= 0)
		{
			GCGuildWarScore(lpGuild2->Index[n]);
		}
	}
	return false;
}

BOOL gObjGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int score = 1;

	if(gObjTargetGuildWarCheck(lpObj,lpTargetObj)==0)
	{
		return false;
	}

	if(strcmp(lpTargetObj->Name,lpTargetObj->lpGuild->Names[0])==0)
	{
		score = 2;
	}

	_GUILD_INFO_STRUCT * lpGuild = lpObj->lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuild = lpTargetObj->lpGuild;

	if(gObjGuildWarProc(lpGuild,lpTargetGuild,score)==1)
	{
		if(lpGuild->WarType == 1)
		{
			gObjGuildWarEndSend(lpGuild,lpTargetGuild,1,0);
			gBattleGroundEnable(lpGuild->BattleGroundIndex,0);
			gObjGuildWarEnd(lpGuild,lpTargetGuild);
			cManager.BattleInfoSend(GetBattleTeamName(0,0),255,GetBattleTeamName(0,1),255);
		}
		else
		{
			gObjGuildWarEndSend(lpObj,1,0);
			gObjAddMsgSendDelay(lpObj,4,lpObj->m_Index,2000,0);
		}
	}
	return true;
}

BOOL gObjGuildWarMasterClose(LPOBJ lpObj)
{
	if(lpObj->GuildNumber < 1)
	{
		return false;
	}

	if(strcmp(lpObj->lpGuild->Names[0],lpObj->Name))
	{
		return false;
	}

	gObjGuildWarEndSend(lpObj,3,2);

	if(lpObj->lpGuild->WarType == 1 && lpObj->lpGuild->BattleGroundIndex < 1)
	{
		gBattleGroundEnable(lpObj->lpGuild->BattleGroundIndex,0);
		cManager.BattleInfoSend(GetBattleTeamName(0,0),255,GetBattleTeamName(0,1),255);
	}
	gObjGuildWarEnd(lpObj->lpGuild,lpObj->lpGuild->lpTargetGuildNode);
	return true;
}

int gObjGuildWarItemGive(_GUILD_INFO_STRUCT * lpWinGuild, _GUILD_INFO_STRUCT * lpLoseGuild)
{
	return true;
	int n;
	int r_userindex[MAX_USER_GUILD];
	int r_usercount;

	for(n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpLoseGuild->Use[n] > 0)
		{
			r_userindex[r_usercount] = lpLoseGuild->Index[n];
			r_usercount++;
		}
	}

	if(r_usercount < 1)
	{
		LogAdd(lMsg.Get(560),__FILE__,__LINE__);
		return false;
	}

	int lose_user = r_userindex[rand()%r_usercount];

	if(lose_user < 0)
	{
		LogAdd(lMsg.Get(560),__FILE__,__LINE__);
		return false;
	}

	if(gObj[lose_user].Connected < PLAYER_PLAYING)
	{
		LogAdd(lMsg.Get(560),__FILE__,__LINE__);
		return false;
	}

	r_usercount = 0;

	for(n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpWinGuild->Use[n] > 0)
		{
			r_userindex[r_usercount] = lpWinGuild->Index[n];
			r_usercount++;
		}
	}

	if(r_usercount < 1)
	{
		LogAdd(lMsg.Get(561));
		return false;
	}

	int win_user = r_userindex[rand()%r_usercount];

	if(win_user < 0)
	{
		LogAdd(lMsg.Get(561));
		return false;
	}

	if(gObj[win_user].Connected < PLAYER_PLAYING)
	{
		LogAdd(lMsg.Get(561));
		return false;
	}

	LPOBJ lpObj = &gObj[lose_user];

	int count = 24;

	int number;

	while(count--)
	{
		number = rand()%12;

		if(lpObj->pInventory[number].IsItem()==1)
		{
			if(gObjInventoryInsertItem(win_user,lpObj->pInventory[number]) != 0xFF)
			{
				return true;
			}
			break;
		}
	}

	count = 64;

	while(count--)
	{
		number = rand()%64+12;

		if(lpObj->pInventory[number].IsItem()==1)
		{
			if(gObjInventoryInsertItem(win_user,lpObj->pInventory[number]) != 0xFF)
			{
				return true;
			}
			break;
		}
	}
	return true;
}

int gObjGetPkTime(LPOBJ lpObj, int& hour, int& min)
{
	int pktime = 0;

	if(lpObj->m_PK_Time > 0)
	{
		pktime = lpObj->m_PK_Time / 60;
	}

	if(lpObj->m_PK_Level == 4)
	{
		hour = (180 - pktime) / 60;
		min = (180 - pktime) % 60;
		return true;
	}

	if(lpObj->m_PK_Level == 5)
	{
		hour = (360 - pktime) / 60;
		min = (360 - pktime) % 60;
		return true;
	}

	if(lpObj->m_PK_Level >= 6)
	{
		hour = (((lpObj->m_PK_Count - 3) * 60 + 540) - pktime) / 60;
		min = (((lpObj->m_PK_Count - 3) * 60 + 540) - pktime) % 60;
		return true;
	}

	return false;
}

BOOL gObjMonsterCall(int aIndex, int MonsterType, int x, int y)
{

	if(gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		return false;
	}

	if(gObj[aIndex].m_RecallMon >= 0)
	{

		GCRecallMonLife(aIndex,60,0);
		gObjMonsterCallKill(aIndex);
		return false;
	}

	int result = gObjAddCallMon();

	if(result >= 0)
	{
		gObj[result].X = x;
		gObj[result].Y = y;

		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;

		gObj[result].MapNumber = gObj[aIndex].MapNumber;

		gObjSetMonster(result,MonsterType);

		gObj[result].m_RecallMon = aIndex;
		gObj[result].m_Attribute = 100;

		gObj[result].TargetNumber = -1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;

		gObj[result].PathCount = 0;

		gObj[aIndex].m_RecallMon = result;
		gObj[result].m_MoveRange = 15;

		GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);
		LogAddTD("[Summon Monster] [%s][%s] Try to Summon Monster - Succeed (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,result);
		return true;
	}
	return false;
}

void gObjMonsterCallKill(int aIndex)
{

	LogAddTD("[Summon Monster] [%s][%s] Try to Kill Summoned Monster",gObj[aIndex].AccountID,gObj[aIndex].Name);

	if(gObj[aIndex].m_RecallMon < 0)
	{
		return;
	}

	int callmon = gObj[aIndex].m_RecallMon;

	if(callmon < 0 || callmon > OBJMAX -1 )
	{
		return;
	}

	if(gObj[callmon].Type != OBJ_MONSTER)
	{
		return;
	}

	if(gObj[callmon].m_RecallMon != aIndex)
	{
		return;
	}

	if(gObj[aIndex].m_RecallMon >= 0)
	{
		if(gObjIsConnected(gObj[aIndex].m_RecallMon)==1)
		{
			LogAddTD("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Found Summoned Monster (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_RecallMon);
			gObj[gObj[aIndex].m_RecallMon].m_RecallMon = -1;
		}
	}

	gObj[aIndex].m_RecallMon = -1;
	gObjDel(callmon);

	LogAddTD("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Finished to Kill Summoned Monster (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,callmon);
}

BOOL gObjCheckTeleportArea(int aIndex, BYTE x, BYTE y)
{

	if(x < (gObj[aIndex].X - 8)
		|| x > (gObj[aIndex].X + 8)
		|| y < (gObj[aIndex].Y - 8)
		|| y > (gObj[aIndex].Y + 8))
	{
		return false;
	}

	int mapnumber = gObj[aIndex].MapNumber;

	if(mapnumber < 0 || mapnumber >= MAX_NUMBER_MAP)
	{
		return false;
	}

	BYTE attr = MapC[mapnumber].GetAttr(x,y);

	if(attr)
	{
		return false;
	}

	attr = MapC[mapnumber].GetAttr(gObj[aIndex].X,gObj[aIndex].Y);

	if(attr & 1)
	{
		return false;
	}

	return true;
}

BOOL gObjCheckAttackAreaUsedPath(int aIndex, int TarObjIndex)
{
	int X,Y;
	if(gObj[aIndex].PathCount > 1)
	{
		 X = gObj[aIndex].PathX[gObj[aIndex].PathCount-1];
		 Y = gObj[aIndex].PathY[gObj[aIndex].PathCount-1];
	}
	else
	{
		return false;
	}

	if(gObj[TarObjIndex].X < (X - 10)
		|| gObj[TarObjIndex].X > (X + 10)
		|| gObj[TarObjIndex].Y < (Y - 10)
		|| gObj[TarObjIndex].Y > (Y + 10))
	{
		return false;
	}
	return true;
}

BOOL gObjCheckattackAreaUsedViewPort(int aIndex, int TarObjIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[TarObjIndex];

	if(lpTargetObj->m_Index == lpObj->m_Index)
	{
		return true;
	}

	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MAX_VIEWPORT_MONSTER;
	}

	for(int n = 0; n < MVL;n++)
	{
		if(lpObj->VpPlayer[n].state)
		{
			if(lpObj->VpPlayer[n].number == lpTargetObj->m_Index)
			{
				return true;
			}
		}

		if(lpObj->VpPlayer2[n].state)
		{
			if(lpObj->VpPlayer2[n].number == lpTargetObj->m_Index)
			{
				return true;
			}
		}
	}
	return false;
}

int gObjCheckAttackArea(int aIndex, int TarObjIndex)
{
	if(gObjCheckattackAreaUsedViewPort(aIndex,TarObjIndex)==0)
	{
		return 1;
	}

	int mapnumber = gObj[aIndex].MapNumber;

	if(mapnumber < 0 || mapnumber >= MAX_NUMBER_MAP)
	{
		return 2;
	}

	int tarmapnumber = gObj[TarObjIndex].MapNumber;

	if(tarmapnumber < 0 || tarmapnumber >= MAX_NUMBER_MAP)
	{
		return 3;
	}

	if(tarmapnumber != mapnumber)
	{
		return 4;
	}

	unsigned char attr;

//#if (GS_CASTLE == 1)
	attr = 0;
	/*attr = MapC[mapnumber].GetAttr(gObj[TarObjIndex].X,gObj[TarObjIndex].Y);

	if(attr && !(attr&2))
	{
		return 5;
	}*/

	attr = MapC[mapnumber].GetAttr(gObj[aIndex].X,gObj[aIndex].Y);

	if(attr && !(attr&2))
	{
		return 6;
	}

	return false;
}

BOOL gUserFindDevilSquareInvitation(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,19))
			{
				return true;
			}

			if(lpObj->pInventory[n].m_Type == ITEMGET(13,46))
			{
				return true;
			}
		}
	}
	return false;
}

BOOL gUserFindDevilSquareKeyEyes(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int bFindKey = 0;
	int bFindEyes = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,17))
			{
				bFindEyes = 1;
			}

			if(lpObj->pInventory[n].m_Type == ITEMGET(14,18))
			{
				bFindKey = 1;
			}
		}
	}

	if(bFindKey != 0 && bFindEyes != 0)
	{
		return true;
	}
	return false;
}

void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel)
{
	if(gDisconnect == 1)
	{
		return;
	}

	if(endLevel < startLevel)
	{
		endLevel = MAX_CHAR_LEVEL;
	}

	int n;
	LPOBJ lpObj;
	int sClassCount[MAX_TYPE_PLAYER];

	sClassCount[CLASS_WIZARD] = 0;
	sClassCount[CLASS_KNIGHT] = 0;
	sClassCount[CLASS_ELF] = 0;
	sClassCount[CLASS_MAGUMSA] = 0;
	sClassCount[CLASS_DARKLORD] = 0;
	sClassCount[CLASS_SUMMONER] = 0; //season4 add-on

	int totalcount = 0; //season4 add-on

	for(n = OBJ_STARTUSERINDEX; n < OBJMAX; n++) //fixed macro
	{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->Class >= MAX_TYPE_PLAYER && lpObj->Class < 0) //season4 add-on
			{
				continue;
			}

			if(lpObj->Level >= startLevel && lpObj->Level <= endLevel)
			{
				sClassCount[lpObj->Class]++;
			}

			totalcount++;
		}
	}

	MsgOutput(gObj[aIndex].m_Index,"[%d]-[%d] Total %d %s %d %s %d %s %d %s %d %s %d %s %d",startLevel,endLevel,
		totalcount,
		lMsg.Get(1900),sClassCount[CLASS_WIZARD],
		lMsg.Get(1901),sClassCount[CLASS_KNIGHT],
		lMsg.Get(1902),sClassCount[CLASS_ELF],
		lMsg.Get(1903),sClassCount[CLASS_MAGUMSA],
		lMsg.Get(1904),sClassCount[CLASS_DARKLORD],
		lMsg.Get(1905),sClassCount[CLASS_SUMMONER]); //season4 add-on
}

LPOBJ gObjFind(char * targetcharname)
{
	if ( gDisconnect == TRUE )
	{
		return NULL;
	}

	int n;
	LPOBJ lpObj;

	for ( n = OBJ_STARTUSERINDEX ; n<OBJMAX;n++)
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected > PLAYER_LOGGED )
		{
			if ( lpObj->Name[0] == targetcharname[0] )
			{
				if ( strcmp(lpObj->Name, targetcharname) == 0 )
				{
					return lpObj;
				}
			}
		}
	}

	return NULL;

}

BOOL gObjFind10EventChip(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE;n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,21))
			{
				count++;

				if(count >= 10)
				{
					return true;
				}
			}
		}
	}
	return false;
}

BOOL gObjDelete10EventChip(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE;n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,21))
			{
				gObjInventoryDeleteItem(aIndex,n);
				GCInventoryItemDeleteSend(aIndex,n,1);
				LogAddTD("[EventChip] [%s][%s] Delete Rena (%d)",lpObj->AccountID,lpObj->Name,n);
				count++;

				if(count >= 10)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void gObjSetPosition(int aIndex, int x, int y)
{

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_POSISTION_SET pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0xD6;
	pMove.h.size = sizeof(pMove);

	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;


	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX,lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x,y);

	lpObj->m_OldX = x;
	lpObj->m_OldY = y;

	RecvPositionSetProc(&pMove,lpObj->m_Index);
}

int  gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n =0;n<MAX_PLAYER_EQUIPMENT ; n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ((itemtype * MAX_SUBTYPE_ITEMS) + itemindex) )
			{
				if ( itemlevel != -1 )
				{
					if (  lpObj->pInventory[n].m_Level != itemlevel )
					{
						continue;
					}
				}
				
				count++;
			}
		}
	}

	return count;
}

int gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n =0;n<MAIN_INVENTORY_SIZE ; n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ((itemtype * MAX_SUBTYPE_ITEMS) + itemindex) )
			{
				if ( itemlevel != -1 )
				{
					if (  lpObj->pInventory[n].m_Level != itemlevel )
					{
						continue;
					}
				}
				
				count++;
			}
		}
	}

	return count;
}

int gObjGetItemCountInIventory(int aIndex, int itemnum)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == itemnum )
			{
				count++;
			}
		}
	}

	return count;

}

int gObjGetManaItemPos(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;
	
	for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ITEMGET(14,4) || lpObj->pInventory[n].m_Type == ITEMGET(14,5) || lpObj->pInventory[n].m_Type == ITEMGET(14,6) )
			{
				return n;
			}
		}
	}

	return -1;
}

void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count)
{
	LPOBJ lpObj = &gObj[aIndex];
	int delcount = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == itemtype * 512 + itemindex)
			{
				gObjInventoryDeleteItem(aIndex,n);
				GCInventoryItemDeleteSend(aIndex,n,1);
				LogAddTD("[DeleteItem] [%s][%s] Delete (%d,%d)(%d)",lpObj->AccountID,lpObj->Name,itemtype,itemindex,n);
				delcount++;

				if(delcount >= count)
				{
					return;
				}
			}
		}
	}
}

//0059b940
void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint)
{
	if(gObj[aIndex].Level <= 5)
	{
		AddPoint = 0;
		MaxAddPoint = 0;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->Leadership + lpObj->LevelUpPoint;
	int ori_point;

	if(lpObj->Class == CLASS_DARKLORD 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif
		|| lpObj->Class == CLASS_MAGUMSA )
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 7;
	}
	else
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 5;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		ori_point+= lpObj->Level - 220;
	}

	if(g_QuestInfo.GetQuestState(lpObj,4) == 2) //season 2.5 add-on
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,5) == 2) //season 2.5 add-on
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,6) == 2) //season 2.5 add-on
	{
		ori_point+= 30;
	}

	int addpoint = ori_point;

	addpoint += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	AddPoint = total_point - ori_point;
	MaxAddPoint = gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);
	MinusPoint = lpObj->iFruitPoint;
	MaxMinusPoint = gStatMng.GetMaxMinusStat(lpObj->Level,lpObj->Class);
}

//0059bc70
BOOL gObjCheckStatPointUp(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->Leadership + lpObj->LevelUpPoint;
	int ori_point;

	if(lpObj->Class == CLASS_DARKLORD 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif		
		|| lpObj->Class == CLASS_MAGUMSA)
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 7;
	}
	else
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 5;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		ori_point+= lpObj->Level - 220;
	}

	if(g_QuestInfo.GetQuestState(lpObj,4) == 2) //season 2.5 add-on
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,5) == 2) //season 2.5 add-on
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,6) == 2) //season 2.5 add-on
	{
		ori_point+= 30;
	}

	ori_point += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	if(total_point < ori_point)
	{
		return true;
	}
	return false;
}

//0059bef0
BOOL gObjCheckStatPointDown(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->LevelUpPoint;
	int ori_point;

	if(lpObj->Class == CLASS_DARKLORD 
#ifdef MONK
		|| lpObj->Class == CLASS_MONK
#endif				
		|| lpObj->Class == CLASS_MAGUMSA)
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 7;
	}
	else
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 5;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,4) == 2) //season 2.5 add-on
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,5) == 2) //season 2.5 add-on
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,6) == 2) //season 2.5 add-on
	{
		ori_point+= 30;
	}

	ori_point += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	if(total_point > ori_point)
	{
		return true;
	}
	return false;
}

void gObjUseCircle(int aIndex, int pos)
{
	int level = gObj[aIndex].pInventory[pos].m_Level;
	int iSerial = gObj[aIndex].pInventory[pos].m_Number;

	PMSG_DEFRESULT pResult;

	PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 0;

	if(gObj[aIndex].Level <= 10 || level > 3)
	{
		pResult.result |= 0xC0;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return;
	}

	if(gObjCheckStatPointUp(aIndex)==0)
	{
		pResult.result |= 0xC0;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return;
	}

	LogAddTD("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%u",gObj[aIndex].AccountID,gObj[aIndex].Name,level,pos,iSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex,pos,1);

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)(MaxAddPoint),(short &)MinusPoint,(short &)MaxMinusPoint);

	int iSuccessRate = 0;

	if(AddPoint <= 10)
	{
		iSuccessRate = 100;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.1)
	{
		iSuccessRate = 90;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.3)
	{
		iSuccessRate = 80;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.5)
	{
		iSuccessRate = 70;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.8)
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}

	int incvalue = 0;

	if(rand()%100 < iSuccessRate)
	{
		iSuccessRate = rand()%100;

		int AddStat = 0;

		if(iSuccessRate < 70)
		{
			AddStat = 1;
		}
		else if(iSuccessRate < 95)
		{
			AddStat = 2;
		}
		else
		{
			AddStat = 3;
		}

		if((AddStat + AddPoint) > MaxAddPoint)
		{
			AddStat = 1;
		}

		pResult.result |= level << 4;
		pResult.result |= AddStat;

		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);

		incvalue = AddStat;

		LogAddTD("[StatUp] [%s][%s] Success [%d][%d] %u",gObj[aIndex].AccountID,gObj[aIndex].Name,level,incvalue,iSerial);
	}
	else
	{
		pResult.result |= 0x40;
		pResult.result |= level << 4;
		pResult.result = pResult.result;

		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		LogAddTD("[StatUp] [%s][%s] Fail [%d] %u",gObj[aIndex].AccountID,gObj[aIndex].Name,level,iSerial);
		return;
	}

	switch(level)
	{
	case 3:
		gObj[aIndex].Strength += incvalue;
		break;
	case 2:
		gObj[aIndex].Dexterity += incvalue;
		break;
	case 1:
		gObj[aIndex].Vitality += incvalue;
		gObj[aIndex].MaxLife += gObj[aIndex].VitalityToLife * incvalue;
		GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].MaxLife + gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield);
		break;
	case 0:
		gObj[aIndex].Energy += incvalue;
		gObj[aIndex].MaxMana += gObj[aIndex].EnergyToMana * incvalue;
		gObjSetBP(aIndex);
		GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP);
		break;
	default: break;
	}

	gObjCalCharacter(aIndex);
}

void gObjUsePlusStatFruit(int aIndex,int pos)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Class >= MAX_TYPE_PLAYER || gObj[aIndex].Class < 0)
	{
		return;
	}

	int iItemLevel = gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial = gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;

	PMSG_USE_STAT_FRUIT pResult;

	PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = STAT_PLUS_PREVENTED;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = 0;

	if(gObj[aIndex].Level <= 9 || iItemLevel > FRUIT_LEADERSHIP)
	{
		pResult.result = STAT_PLUS_PREVENTED;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return;
	}

	if(iItemLevel == FRUIT_LEADERSHIP && gObj[aIndex].Class != CLASS_DARKLORD)
	{
		pResult.result = STAT_PLUS_PREVENTED;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return;
	}

	for(int iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE; iItemIndex++)
	{
		if(gObj[aIndex].pInventory[iItemIndex].IsItem())
		{
			pResult.result = STAT_FRUIT_USING_PREVENTED_BY_ITEMEQUIPMENT;
			DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
			return;
		}
	}

	if(gObjCheckStatPointUp(aIndex) == false)
	{
		pResult.result = STAT_PLUS_PREVENTED_BY_MAX;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return;
	}

	LogAddTD("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%u", gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,pos,iItemSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex,pos,1);

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)MaxAddPoint,(short &)MinusPoint,(short &)MaxMinusPoint);

	int iSuccessRate = 0;

	if(AddPoint <= 10)
	{
		iSuccessRate = 100;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.1))
	{
		iSuccessRate = 90;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.3))
	{
		iSuccessRate = 80;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.5))
	{
		iSuccessRate = 70;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.8))
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}

	int iIncStat = 0;
	int bSuccess = 0;

	if(rand()%100 < iSuccessRate)
	{
		bSuccess = 1;
	}

	if(bSuccess != 0)
	{
		iSuccessRate = rand()%100;

		int AddStat = 0;

		if(iSuccessRate < 70)
		{
			AddStat = 1;
		}
		else if(iSuccessRate < 95)
		{
			AddStat = 2;
		}
		else
		{
			AddStat = 3;
		}

		if((AddStat + AddPoint) > MaxAddPoint)
		{
			AddStat = 1;
		}

		pResult.result = STAT_PLUS_SUCCESS;
		pResult.btStatValue = AddStat;
		pResult.btFruitType = iItemLevel;

		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		iIncStat = AddStat;

		LogAddTD("[StatUp] [%s][%s] Success [%d][%d] %u", gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iIncStat,iItemSerial);
	}
	else
	{
		pResult.result = STAT_PLUS_FAILED;
		pResult.btStatValue = 0;
		pResult.btFruitType = iItemLevel;

		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);

		LogAddTD("[StatUp] [%s][%s] Fail [%d] %u", gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iItemSerial);
		return;
	}

	switch(iItemLevel)
	{
	case FRUIT_LEADERSHIP: gObj[aIndex].Leadership += iIncStat;	break;
	case FRUIT_STRENGTH: gObj[aIndex].Strength += iIncStat;	break;
	case FRUIT_DEXTERITY: gObj[aIndex].Dexterity += iIncStat;	break;
	case FRUIT_VITALITY:
		gObj[aIndex].Vitality += iIncStat;
		gObj[aIndex].MaxLife += gObj[aIndex].VitalityToLife * iIncStat;
		break;
	case FRUIT_ENERGY:
		gObj[aIndex].Energy += iIncStat;
		gObj[aIndex].MaxMana += gObj[aIndex].EnergyToMana * iIncStat;
		gObjSetBP(aIndex);
		break;
	default : break;
	}

	gObjCalCharacter(aIndex);
	GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].MaxLife + gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield);
	gObjSetBP(aIndex);
	GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP + gObj[aIndex].AddBP);
}



void gObjUseMinusStatFruit(int aIndex, int pos)
{
	if(!OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Class >= MAX_TYPE_PLAYER || gObj[aIndex].Class < 0)
	{
		return;
	}

	int iItemLevel = gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial = gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;

	PMSG_USE_STAT_FRUIT pResult;

	PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = STAT_MINUS_PREVENTED;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = 0;

	if(gObj[aIndex].Level <= 9 || iItemLevel > FRUIT_LEADERSHIP)
	{
		pResult.result = STAT_MINUS_PREVENTED;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return;
	}

	if(iItemLevel == FRUIT_LEADERSHIP && gObj[aIndex].Class != CLASS_DARKLORD)
	{
		pResult.result = STAT_MINUS_PREVENTED;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return;
	}

	for(int iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE; iItemIndex++)
	{
		if(gObj[aIndex].pInventory[iItemIndex].IsItem())
		{
			pResult.result = STAT_FRUIT_USING_PREVENTED_BY_ITEMEQUIPMENT;
			DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
			return;
		}
	}

	int iDefaultStat = 0;
	int iPresentStat = 0;
	BOOL bEnableUseFruit = TRUE;

	if(iItemLevel == FRUIT_ENERGY)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Energy;
		iPresentStat = gObj[aIndex].Energy;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = FALSE;
		}
	}
	else if(iItemLevel == FRUIT_VITALITY)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Vitality;
		iPresentStat = gObj[aIndex].Vitality;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = FALSE;
		}
	}
	else if(iItemLevel == FRUIT_DEXTERITY)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Dexterity;
		iPresentStat = gObj[aIndex].Dexterity;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = FALSE;
		}
	}
	else if(iItemLevel == FRUIT_STRENGTH)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Strength;
		iPresentStat = gObj[aIndex].Strength;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = FALSE;
		}
	}
	else if(iItemLevel == FRUIT_LEADERSHIP)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Leadership;
		iPresentStat = gObj[aIndex].Leadership;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = FALSE;
		}
	}

	if(bEnableUseFruit == FALSE)
	{
		pResult.result = STAT_MINUS_PREVENTED_BY_DEFAULT;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return;
	}

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint = 0;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)MaxAddPoint,(short &)MinusPoint,(short &)MaxMinusPoint);

	if(MinusPoint >= MaxMinusPoint || MinusPoint < 0)
	{
		pResult.result = STAT_MINUS_PREVENTED_BY_MAX;
		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		return;
	}

	LogAddTD("[StatDown] [%s][%s] Use StatDownItem Level:%d Pos:%d serial:%u", gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,pos,iItemSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GCInventoryItemDeleteSend(aIndex,pos,1);

	int iDecStat = 0;
	BOOL bSuccess = FALSE;
	int iSuccessRate = 0;

	if(MinusPoint <= 10)
	{
		iSuccessRate = 100;
	}
	else if(MinusPoint - 10 < MaxMinusPoint * 0.1)
	{
		iSuccessRate = 70;
	}
	else if(MinusPoint - 10 < MaxMinusPoint * 0.3)
	{
		iSuccessRate = 60;
	}
	else if(MinusPoint - 10 < MaxMinusPoint * 0.5)
	{
		iSuccessRate = 50;
	}
	else if(MinusPoint - 10 < MaxMinusPoint * 0.8)
	{
		iSuccessRate = 40;
	}
	else
	{
		iSuccessRate = 30;
	}

	if(rand()%100 < iSuccessRate)
	{
		bSuccess = TRUE;
	}

	if(bSuccess != FALSE)
	{
		iSuccessRate = rand()%100;

		if(iSuccessRate < 50)
		{
			iDecStat = 1;
		}
		else if(iSuccessRate < 75)
		{
			iDecStat = 3;
		}
		else if(iSuccessRate < 91)
		{
			iDecStat = 5;
		}
		else if(iSuccessRate < 98)
		{
			iDecStat = 7;
		}
		else
		{
			iDecStat = 9;
		}

		if((MinusPoint + iDecStat) >= MaxMinusPoint)
		{
			iDecStat = MaxMinusPoint - MinusPoint;
		}

		if((iPresentStat - iDecStat) < iDefaultStat)
		{
			iDecStat = iPresentStat - iDefaultStat;
		}

		pResult.result = STAT_MINUS_SUCCESS;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = iDecStat;

		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		LogAddTD("[StatDown] [%s][%s] Success [%d][%d] %u", gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iDecStat,iItemSerial);
	}
	else
	{
		pResult.result = STAT_MINUS_FAILED;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = 0;

		DataSend(aIndex,(LPBYTE)&pResult,pResult.h.size);
		LogAddTD("[StatDown] [%s][%s] Fail [%d] %u", gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iItemSerial);
		return;
	}

	switch(iItemLevel)
	{
	case FRUIT_LEADERSHIP:	gObj[aIndex].Leadership -= iDecStat;	break;
	case FRUIT_STRENGTH: gObj[aIndex].Strength -= iDecStat;	break;
	case FRUIT_DEXTERITY: gObj[aIndex].Dexterity -= iDecStat;	break;
	case FRUIT_VITALITY:
		gObj[aIndex].Vitality -= iDecStat;
		gObj[aIndex].MaxLife -= gObj[aIndex].VitalityToLife *iDecStat;
		break;
	case FRUIT_ENERGY:
		gObj[aIndex].Energy -= iDecStat;
		gObj[aIndex].MaxMana -= gObj[aIndex].EnergyToMana * iDecStat;
		gObjSetBP(aIndex);
		break;
	default:
		break;
	}

	int iOldLevelUpPoint = gObj[aIndex].LevelUpPoint;
	gObj[aIndex].LevelUpPoint += iDecStat;

	int iOldFruitPoint = gObj[aIndex].LevelUpPoint;
	gObj[aIndex].iFruitPoint += iDecStat;

	LogAddTD("[StatDown] [%s][%s] MinusStat[%d] -> LevelUpPoint Old(%d)/New(%d)  FruitPoint Old(%d)/New(%d)", gObj[aIndex].AccountID,gObj[aIndex].Name,iDecStat,iOldLevelUpPoint,gObj[aIndex].LevelUpPoint,iOldFruitPoint,gObj[aIndex].iFruitPoint);

	gObjCalCharacter(aIndex);
	GCReFillSend(gObj[aIndex].m_Index,gObj[aIndex].MaxLife + gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield);
	gObjSetBP(aIndex);
	GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP);
}

//New CashShop Function
void gObjUseCashShopStatFruit(LPOBJ lpObj, int pos)
{
	if(lpObj->Class >= MAX_TYPE_PLAYER || lpObj->Class < 0)
	{
		return;
	}

	int iStatType		= 0;
	int iItemSerial		= lpObj->pInventory[pos].m_Number;
	int iItemDurability = lpObj->pInventory[pos].m_Durability * 10.0f;
	int iCharacterClass = lpObj->Class;

	switch( lpObj->pInventory[pos].m_Type )
	{
		case ITEMGET(13,54): iStatType = FRUIT_STRENGTH; break;
		case ITEMGET(13,55): iStatType = FRUIT_DEXTERITY; break;
		case ITEMGET(13,56): iStatType = FRUIT_VITALITY; break;
		case ITEMGET(13,57): iStatType = FRUIT_ENERGY; break;
		case ITEMGET(13,58): iStatType = FRUIT_LEADERSHIP; break;
	}

	PMSG_USE_STAT_FRUIT pMsg;

	::PHeadSetB((LPBYTE)&pMsg,0x2C,sizeof(PMSG_USE_STAT_FRUIT));
	pMsg.result = STAT_MINUS_PREVENTED;
	pMsg.btFruitType = iStatType;
	pMsg.btStatValue = 0;

	for( int i = 0; i < INVETORY_WEAR_SIZE; i++ )
	{
		if( lpObj->pInventory[i].IsItem() != FALSE )
		{
			pMsg.result = STAT_FRUIT_USING_PREVENTED_BY_ITEMEQUIPMENT;
			DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
			return;
		}	
	}

	int iMaxStat = 0;
	int iStat = 0;
	BOOL bResult = TRUE;

	switch( iStatType ) //season 2.5 add-on
	{
		case FRUIT_ENERGY:
			{
				if( lpObj->Strength == DCInfo.DefClass[iCharacterClass].Strength ){	bResult = FALSE;}
			}
			break;
		case FRUIT_VITALITY:
			{
				if( lpObj->Dexterity == DCInfo.DefClass[iCharacterClass].Dexterity ){ bResult = FALSE;}
			}
			break;
		case FRUIT_DEXTERITY:
			{
				if( lpObj->Vitality == DCInfo.DefClass[iCharacterClass].Vitality ){	bResult = FALSE;}
			}
			break;
		case FRUIT_STRENGTH:
			{
				if( lpObj->Energy == DCInfo.DefClass[iCharacterClass].Energy ){	bResult = FALSE;}
			}
			break;
		case FRUIT_LEADERSHIP:
			{
				if( lpObj->Leadership == DCInfo.DefClass[iCharacterClass].Leadership ){	bResult = FALSE;}
			}
			break;
	}
	
	if( iStatType == FRUIT_ENERGY )
	{
		iMaxStat = DCInfo.DefClass[iCharacterClass].Energy;
		iStat = lpObj->Energy;
		if( (iStat - iItemDurability) < iMaxStat )
			iItemDurability = iStat - iMaxStat; //Season 2.5 changed
	}
	else if( iStatType == FRUIT_VITALITY )
	{
		iMaxStat = DCInfo.DefClass[iCharacterClass].Vitality;
		iStat = lpObj->Vitality;
		if( (iStat - iItemDurability) < iMaxStat )
			iItemDurability = iStat - iMaxStat; //Season 2.5 changed
	}
	else if( iStatType == FRUIT_DEXTERITY )
	{
		iMaxStat = DCInfo.DefClass[iCharacterClass].Dexterity;
		iStat = lpObj->Dexterity;
		if( (iStat - iItemDurability) < iMaxStat )
			iItemDurability = iStat - iMaxStat; //Season 2.5 changed
	}
	else if( iStatType == FRUIT_STRENGTH )
	{
		iMaxStat = DCInfo.DefClass[iCharacterClass].Strength;
		iStat = lpObj->Strength;
		if( (iStat - iItemDurability) < iMaxStat )
			iItemDurability = iStat - iMaxStat; //Season 2.5 changed
	}
	else if( iStatType == FRUIT_LEADERSHIP )
	{
		iMaxStat = DCInfo.DefClass[iCharacterClass].Leadership;
		iStat = lpObj->Leadership;
		if( (iStat - iItemDurability) < iMaxStat )
			iItemDurability = iStat - iMaxStat; //Season 2.5 changed
	}

	if( bResult == FALSE )
	{
		pMsg.result = STAT_MINUS_PREVENTED_BY_DEFAULT;
		DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
		return;
	}

	LogAdd("[CashShop][ExMinusStatFruit] [%s][%s] Use StatDownItem Level:%d,Pos:%d,Dur:%d,Serial:%u", lpObj->AccountID,lpObj->Name,iStatType,pos,lpObj->pInventory[pos].m_Durability,iItemSerial);
	
	gObjInventoryItemSet(lpObj->m_Index, pos, -1);
	lpObj->pInventory[pos].Clear();
	GCInventoryItemDeleteSend(lpObj->m_Index, pos, 1);

	pMsg.result = STAT_EX_MINUS_SUCCESS; //season 2.5 changed
	pMsg.btFruitType = iStatType;
	pMsg.btStatValue = iItemDurability;
	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);

	switch( iStatType )
	{
		case FRUIT_LEADERSHIP:
			lpObj->Leadership -= iItemDurability;
			break;
		case FRUIT_STRENGTH:
			lpObj->Strength -= iItemDurability;
			break;
		case FRUIT_DEXTERITY:
			lpObj->Dexterity -= iItemDurability;
			break;
		case FRUIT_VITALITY:
			lpObj->Vitality -= iItemDurability;
			lpObj->MaxLife -= (iItemDurability*lpObj->VitalityToLife);
			break;
		case FRUIT_ENERGY:
			lpObj->Energy -= iItemDurability;
			lpObj->MaxMana -= (iItemDurability*lpObj->EnergyToMana);
			gObjSetBP(lpObj->m_Index);
			break;
	}
	
	int iOldLevelUpPoint = lpObj->LevelUpPoint;
	lpObj->LevelUpPoint += iItemDurability;

	LogAdd("[CashShop][ExMinusStatFruit] [%s][%s] MinusStat[%d] -> LevelUpPoint Old(%d)/New(%d)",
		lpObj->AccountID,lpObj->Name,iItemDurability,iOldLevelUpPoint,lpObj->LevelUpPoint);

	gObjCalCharacter(lpObj->m_Index);
	GCReFillSend(lpObj->m_Index,(lpObj->AddLife + lpObj->MaxLife), 0xFE, 0, (lpObj->iMaxShield + lpObj->iAddShield) );
	gObjSetBP(lpObj->m_Index);
	GCManaSend(lpObj->m_Index, (lpObj->AddMana + lpObj->MaxMana), 0xFE, 0, lpObj->MaxBP);
}

//New CashShop Function
void gObjUseCashShopStatPotion(LPOBJ lpObj, int pos)
{
	if(lpObj == NULL)
	{
		return;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	int iItemType = 0;	// EBP-4
	iItemType = lpObj->pInventory[pos].m_Type;


	if(iItemType == ITEMGET(14,82) && lpObj->Class != CLASS_DARKLORD)
	{
		GCItemDurSend(lpObj->m_Index, pos, lpObj->pInventory[pos].m_Durability,1);
		return;
	}
	
	g_CashItemPeriodSystem.GDReqPeriodItemInsert(lpObj, iItemType, -1);
	lpObj->pInventory[pos].m_Durability -= 1.0f;

	if(lpObj->pInventory[pos].m_Durability > 0.0f)
	{
		GCItemDurSend(lpObj->m_Index, pos, lpObj->pInventory[pos].m_Durability,1);
	}
	else
	{
		gObjInventoryItemSet(lpObj->m_Index, pos, -1);
		lpObj->pInventory[pos].Clear();
		GCInventoryItemDeleteSend(lpObj->m_Index, pos, 1);
	}
	return;
}

//New CashShop Function
void gObjUseFullRecoveryPotion(LPOBJ lpObj, int pos)
{
	if(lpObj == NULL)
	{
		return;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->pInventory[pos].m_Type == ITEMGET(14,70))
	{
		lpObj->Life = lpObj->AddLife + lpObj->MaxLife;
		GCReFillSend(lpObj->m_Index,lpObj->Life, -1, 1, lpObj->iShield);
	}
	else
	{
		lpObj->Mana = lpObj->AddMana + lpObj->MaxMana;
		GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
	}

	lpObj->pInventory[pos].m_Durability -= 1.0f;

	if(lpObj->pInventory[pos].m_Durability > 0.0f)
	{
		GCItemDurSend(lpObj->m_Index, pos, lpObj->pInventory[pos].m_Durability,1);
	}
	else
	{
		gObjInventoryItemSet(lpObj->m_Index, pos, -1);
		lpObj->pInventory[pos].Clear();
		GCInventoryItemDeleteSend(lpObj->m_Index, pos, 1);
	}
	return;
}

//
void gObjUseRecoveryPotion(LPOBJ lpObj, int pos, double value) //00518400

{
	if(lpObj == NULL)
	{
		return;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->pInventory[pos].m_Type == ITEMGET(14,94))
	{
		float loc1 = (float(lpObj->AddLife) + lpObj->MaxLife);

		lpObj->Life =  lpObj->Life + (value * loc1);

		if(lpObj->Life > loc1)
		{
			lpObj->Life = loc1;
		}

		GCReFillSend(lpObj->m_Index,lpObj->Life, -1, 1, lpObj->iShield);
	}
	else if( lpObj->pInventory[pos].m_Type == ITEMGET(14, 133) )	//1.01.00
	{
		float Add = lpObj->iAddShield + lpObj->iMaxShield;
        lpObj->iShield = lpObj->iShield + Add * value;
        if( lpObj->iShield > Add )
		{
          lpObj->iShield = Add;
		}
        GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFFu, 1, lpObj->iShield);
	}
	else
	{
		float loc2 = (float(lpObj->AddMana) + lpObj->MaxMana);

		lpObj->Mana = lpObj->Mana + (value * loc2);

		if(lpObj->Mana > loc2)
		{
			lpObj->Mana = loc2;
		}

		GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
	}

	lpObj->pInventory[pos].m_Durability -= 1.0f;

	if(lpObj->pInventory[pos].m_Durability > 0.0f)
	{
		GCItemDurSend(lpObj->m_Index, pos, lpObj->pInventory[pos].m_Durability,1);
	}
	else
	{
		gObjInventoryItemSet(lpObj->m_Index, pos, -1);
		lpObj->pInventory[pos].Clear();
		GCInventoryItemDeleteSend(lpObj->m_Index, pos, 1);
	}
	return;
}

//New CashShop Function
void gObjUseIndulgence(LPOBJ lpObj, int pos)
{
	if(lpObj->pInventory[pos].m_Type != ITEMGET(14,72))
	{
		return;
	}

	if(lpObj->pInventory[pos].m_Durability <= 0.0f)
	{
		return;
	}

	int iItemDurability = 0;
	iItemDurability = (int)lpObj->pInventory[pos].m_Durability * 60;

	if(lpObj->m_PK_Level <= 3)
	{
		return;
	}

	gObjPkDownTimeCheck(lpObj, iItemDurability);
	gObjInventoryItemSet(lpObj->m_Index, pos, -1);
	lpObj->pInventory[pos].Clear();
	GCInventoryItemDeleteSend(lpObj->m_Index, pos, 1);
	return;
}

void gObjCalcMaxLifePower(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int iLevelBase = lpObj->Level + lpObj->m_nMasterLevel;

	switch(lpObj->Class)
	{
	case CLASS_WIZARD:
			lpObj->m_MaxLifePower = iLevelBase + lpObj->Vitality + 110;
		break;
	case CLASS_KNIGHT:
			lpObj->m_MaxLifePower = iLevelBase*2 + lpObj->Vitality + 60;
		break;
	case CLASS_ELF:
			lpObj->m_MaxLifePower = iLevelBase + lpObj->Vitality + 80;
		break;
	case CLASS_MAGUMSA:
	case CLASS_DARKLORD:
			lpObj->m_MaxLifePower = iLevelBase + lpObj->Vitality + 110;
		break;
	default :
		lpObj->m_MaxLifePower = 0;
		break;
	}
}

void gObjDelayLifeCheck(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_CheckLifeTime > 0)
	{
		lpObj->m_CheckLifeTime--;

		if(lpObj->m_CheckLifeTime <= 0)
		{
			lpObj->m_CheckLifeTime = 0;

			if(lpObj->Life < 0)
			{
				lpObj->Life = 0;
			}

			if(lpObj->lpAttackObj != 0)
			{
				gObjLifeCheck(lpObj,lpObj->lpAttackObj,0,1,0,0,0,0);
			}
		}
	}
}

BOOL gObjDuelCheck(LPOBJ lpObj)
{
	BOOL bRetVal = FALSE;

	if ( lpObj->Type == OBJ_USER )
	{
		int iDuelIndex = lpObj->m_iDuelUser;
		if (  OBJMAX_RANGE(iDuelIndex) )
		{
			bRetVal = TRUE;
		}
	}

	return bRetVal;
}

BOOL gObjDuelCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	BOOL bRetVal = FALSE;

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if ( lpObj->m_iDuelUser == lpTargetObj->m_Index && lpTargetObj->m_iDuelUser == lpObj->m_Index)
		{
			bRetVal = TRUE;
		}
	}

	return bRetVal;

}

void gObjResetDuel(LPOBJ lpObj)
{
	if(lpObj == NULL)
	{
		return;
	}

	lpObj->m_iDuelUser = -1;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_btDuelScore = 0;
	lpObj->m_iDuelTickCount = 0;
	gDarkSpirit[lpObj->m_Index].ReSetTarget(lpObj->m_Index);
}

void gObjSendDuelEnd(LPOBJ lpObj)
{
	PMSG_ANS_END_DUEL pMsg;
	if(gObjIsConnected(lpObj->m_Index)==0)
	{
		return;
	}

	if(lpObj->Type == OBJ_MONSTER)
	{
		return;
	}

	if(lpObj->CloseCount >= 0)
	{
		return;
	}

	lpObj->m_iDuelUser = -1;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_btDuelScore = 0;
	lpObj->m_iDuelTickCount = 0;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xAB;
	pMsg.h.size = sizeof(pMsg);

	pMsg.NumberH = SET_NUMBERH(lpObj->m_iDuelUser);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_iDuelUser);

	memcpy(pMsg.szName,gObj[lpObj->m_iDuelUser].Name,10);
	DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
}

void gObjCheckAllUserDuelStop()
{
	for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
	{
		if(gObj[i].Connected == PLAYER_PLAYING)
		{
			if(gObj[i].Type == OBJ_USER)
			{
				if(OBJMAX_RANGE(gObj[i].m_iDuelUser))
				{
					if(GetTickCount() - gObj[i].m_iDuelTickCount > 60000)
					{
#ifdef NPVP
						gObjSendDuelEnd(&gObj[gObj[i].m_iDuelUser]);
						gObjSendDuelEnd(&gObj[i]);
#else
						GCSendEndDuel(&gObj[gObj[i].m_iDuelUser]);
						GCSendEndDuel(&gObj[i]);
#endif
						gObjResetDuel(&gObj[i]);
					}
				}
			}
		}
	}
}

/*void gObjNProtectGGCheckSum(int aIndex)
{
	if(gUseNPGGChecksum==0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_InWebzen)
	{
		return;
	}

	if(lpObj->m_bSentGGAuth == 0)
	{
		if((GetTickCount() - lpObj->m_NPggCheckSumSendTime) > 300000)
		{
			lpObj->m_NPggCheckSumSendTime = GetTickCount();

			DWORD dwGGErrCode = lpObj->NPggCSAuth.GetAuthQuery();

			if(dwGGErrCode != 0)
			{
				LogAddTD("[NPgg] Failed Send Checksum %x [%s][%s] errorcode: %d",dwGGErrCode,lpObj->AccountID,lpObj->Name,dwGGErrCode);
				lpObj->m_bSentGGAuth = 0;
				gObjCloseSet(aIndex,0);
			}
			else
			{
				LogAddTD("[NPgg] Send Checksum %x [%s][%s]",dwGGErrCode,lpObj->AccountID,lpObj->Name);
				GCNPggSendCheckSum(aIndex,&lpObj->NPggCSAuth.m_AuthQuery);
				lpObj->m_bSentGGAuth = 1;
			}
		}
	}
	else
	{
		if((GetTickCount() - lpObj->m_NPggCheckSumSendTime) > 30000)
		{
			LogAddTD("[NPgg] Checksum Time Out [%s][%s]",lpObj->AccountID,lpObj->Name);

			PMSG_NOTICE pNotice;

			TNotice::SetNoticeProperty(&pNotice,10,_ARGB(0xFF,0xFE,0x51,0x51),1,0,20);

			TNotice::MakeNoticeMsg(&pNotice,10,lMsg.Get(1201));
			TNotice::SendNoticeToUser(aIndex,&pNotice);

			TNotice::MakeNoticeMsg(&pNotice,10,lMsg.Get(1202));
			TNotice::SendNoticeToUser(aIndex,&pNotice);

			TNotice::MakeNoticeMsg(&pNotice,10,lMsg.Get(1203));
			TNotice::SendNoticeToUser(aIndex,&pNotice);

			gObjCloseSet(aIndex,0);
		}
	}
}*/

void SkillFrustrum(BYTE bangle, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	vec3_t p[4];
	Vector(-6, 6, 0, p[0]);
	Vector(6, 6, 0, p[1]);
	Vector(1, 0, 0, p[2]);
	Vector(-1, 0, 0, p[3]);

	vec3_t Angle;

	Vector(0.f, 0.f, (bangle * 360 / 255), Angle);

	float Matrix[3][4];

	AngleMatrix(Angle,Matrix);

	vec3_t vFrustrum[4];

	for(int i = 0; i < 4; i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}

BOOL SkillTestFrustrum(int x, int y,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int j = 3;

	for(int i = 0; i < 4; j = i,i++)
	{
		if((lpObj->fSkillFrustrumX[i]- x) * (lpObj->fSkillFrustrumY[j]-y) - (lpObj->fSkillFrustrumX[j]-x) * (lpObj->fSkillFrustrumY[i]-y) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

BOOL gObjCheckMaxZen(int aIndex, int nAddZen) 
{
	if ( !OBJMAX_RANGE(aIndex) )
		return FALSE;

	LPOBJ lpObj = &gObj[aIndex];
	
	__int64 _Money = 0;
	_Money = (__int64)lpObj->Money + (__int64)nAddZen;

	if ( _Money > (__int64)MAX_ZEN )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 226)), aIndex, 1);
		return FALSE;
	}

	return TRUE;
}

void MakeRandomSetItem(int aIndex)
{
	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	ItemSerialCreateSend(aIndex,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,itemnum,0,0,Option1,Option2,Option3,aIndex,0,SetOption);

	int loc10 = 0;

	if((SetOption&0x01) != 0)
	{
		loc10 = 1;
	}
	else if((SetOption&0x02) != 0)
	{
		loc10 = 2;
	}

	LogAddTD("[Chaos Castle] [%s][%s] Winner Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",gObj[aIndex].AccountID,gObj[aIndex].Name,
		itemnum,Option1,Option2,Option3,SetOption, gSetItemOption.GetSetOptionName(itemnum, loc10));
}

void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, int iMapnumber)
{
	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	if(cDropX == 0 && cDropY == 0)
	{
		cDropX = gObj[aIndex].X;
		cDropY = gObj[aIndex].Y;
	}

	ItemSerialCreateSend(aIndex,iMapnumber,cDropX,cDropY,itemnum,0,0,Option1,Option2,Option3,aIndex,0,SetOption);

	int loc10 = 0;

	if((SetOption&0x01) != 0)
	{
		loc10 = 1;
	}
	else if((SetOption&0x02) != 0)
	{
		loc10 = 2;
	}

	if(iRewardType == 1)
	{
		LogAddTD("[????Reward][KUNDUN] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption, gSetItemOption.GetSetOptionName(itemnum, loc10));
	}
	else
	{
		LogAddTD("[Reward][Etc] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption, gSetItemOption.GetSetOptionName(itemnum, loc10));
	}		
}

void gObjRecall(int aIndex, int mapnumber, int x, int y)
{
	if(gObj[aIndex].MapNumber != mapnumber)
	{
		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;

		gObj[aIndex].TX = x;
		gObj[aIndex].TX = y;

		gObj[aIndex].MapNumber = mapnumber;

		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport = 0;

		gObjClearViewport(&gObj[aIndex]);
		GCTeleportSend(&gObj[aIndex],-1,mapnumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

		if(gObj[aIndex].m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenMapNumber = mapnumber;
		gObj[aIndex].RegenMapX = x;
		gObj[aIndex].RegenMapY = y;
		gObj[aIndex].RegenOk = 1;
	}
	else
	{
		PMSG_MAGICATTACK_RESULT pAttack;

		PHeadSetBE((LPBYTE)&pAttack,0x19,sizeof(pAttack));

		pAttack.MagicNumberH = SET_NUMBERH(6);
		pAttack.MagicNumberL = SET_NUMBERL(6);
		pAttack.SourceNumberH = SET_NUMBERH(aIndex);
		pAttack.SourceNumberL = SET_NUMBERL(aIndex);
		pAttack.TargetNumberH = SET_NUMBERH(aIndex);
		pAttack.TargetNumberL = SET_NUMBERL(aIndex);

		DataSend(aIndex,(LPBYTE)&pAttack,pAttack.h.size);
		MsgSendV2(&gObj[aIndex],(LPBYTE)&pAttack,pAttack.h.size);
		gObjTeleportMagicUse(aIndex,x,y);
	}
}

void gObjSetExpPetItem(int aIndex, int exp)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Class != 4)	
	{
		return;
	}

	exp *= gDarkSpiritAddExperience;

	if(lpObj->pInventory[8].m_Type == ITEMGET(13,4) && lpObj->pInventory[1].m_Type == ITEMGET(13,5))
	{
		int addexp = exp * 10 / 100;
		
		if(lpObj->pInventory[1].AddPetItemExp(addexp-(addexp *gDecreaseDarkSpiritExpGivePercent/ 100 )))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[1],1) == 0)
			{
				lpObj->pInventory[1].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1245));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[1].GetName(),lpObj->pInventory[1].m_PetItem_Level,lpObj->pInventory[1].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,1,0,0xFE);
			}
		}

		if(lpObj->pInventory[8].AddPetItemExp(addexp-(addexp *gDecreaseDarkHorseExpGivePercent/ 100 )))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[8],8) == 0)
			{
				lpObj->pInventory[8].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1246));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[8].GetName(),lpObj->pInventory[8].m_PetItem_Level,lpObj->pInventory[8].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,8,1,0xFE);
			}
		}
	}
	else if(lpObj->pInventory[8].m_Type == ITEMGET(13,4))
	{
		int addexp = exp * 20 / 100;

		if(lpObj->pInventory[8].AddPetItemExp(addexp-(addexp *gDecreaseDarkHorseExpGivePercent/ 100 )))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[8],8) == 0)
			{
				lpObj->pInventory[8].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1246));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[8].GetName(),lpObj->pInventory[8].m_PetItem_Level,lpObj->pInventory[8].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,8,1,0xFE);
			}
		}
	}
	else if(lpObj->pInventory[1].m_Type == ITEMGET(13,5))
	{
		int addexp = exp * 20 / 100;

		if(lpObj->pInventory[1].AddPetItemExp(addexp-(addexp *gDecreaseDarkSpiritExpGivePercent/ 100 ) ))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[1],1) == 0)
			{
				lpObj->pInventory[1].PetItemLevelDown(addexp);
				MsgOutput(aIndex,lMsg.Get(1245));
			}
			else
			{
				LogAddTD("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%d]AddExp:[%d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[1].GetName(),lpObj->pInventory[1].m_PetItem_Level,lpObj->pInventory[1].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,1,0,0xFE);
			}
		}
	}
}

BOOL gObjGetRandomItemDropLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount)
{
	int iUX = cX;
	int iUY = cY;

	if ( iRangeX <= 0 )
	{
		iRangeX = 1;
	}

	if ( iRangeY <= 0 )
	{
		iRangeY = 1;
	}

	if ( iLoopCount <= 0 )
	{
		iLoopCount = 1;
	}

	while ( iLoopCount-- > 0 )
	{
		cX = ( rand() % (iRangeX+1) ) * (((rand()%2==0)?-1:1)) + iUX;
		cY = ( rand() % (iRangeY+1) ) * (((rand()%2==0)?-1:1)) + iUY;

		BYTE attr = MapC[iMapNumber].GetAttr(cX, cY);

		if ( (attr&4) != 4 && (attr&8) != 8)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL gObjGetRandomFreeLocation(int iMapNumber, BYTE &cX, BYTE & cY, int iRangeX, int iRangeY, int iLoopCount)
{
	int iUX = cX;
	int iUY = cY;

	if ( iRangeX <= 0 )
	{
		iRangeX = 1;
	}

	if ( iRangeY <= 0 )
	{
		iRangeY = 1;
	}

	if ( iLoopCount <= 0 )
	{
		iLoopCount = 1;
	}

	while ( iLoopCount-- > 0 )
	{
		cX = ( rand() % (iRangeX+1) ) * (((rand()%2==0)?-1:1)) + iUX;
		cY = ( rand() % (iRangeY+1) ) * (((rand()%2==0)?-1:1)) + iUY;

		BYTE attr = MapC[iMapNumber].GetAttr(cX, cY);

		if ( attr == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

int gObjCheckAttackTypeMagic(int iClass, int iSkill)
{
	if ( iSkill == 0 )
	{
		return 0;
	}

	return MagicDamageC.GetSkillType(iSkill);
}

//#if (_GSCS==1)
BOOL gObjGetRandomFreeArea(int iMapNumber, BYTE &cX, BYTE &cY, int iSX, int iSY, int iDX, int iDY, int iLoopCount)
{
	if( iSX <= 0 ) iSX = 1;
	if( iSY <= 0 ) iSY = 1;
	if( iDX <= 0 ) iDX = 1;
	if( iDY <= 0 ) iDY = 1;
	
	if( iSX >= 256 ) iSX = 255;
	if( iSY >= 256 ) iSY = 255;
	if( iDX >= 256 ) iDX = 255;
	if( iDY >= 256 ) iDY = 255;

	if( iLoopCount <= 0 ) iLoopCount = 1;

	while( iLoopCount-- > 0 )
	{
		cX = (rand() % (iDX-iSX)) + iSX;
		cY = (rand() % (iDY-iSY)) + iSY;

		BYTE btMapAttr = MapC[iMapNumber].GetAttr(cX, cY);

		if( btMapAttr == 0 )
			return TRUE;
	}
	return FALSE;
}
//#endif

int gObjGetGuildUnionNumber(LPOBJ lpObj)
{
	int iUnion=0;

	if ( lpObj->lpGuild != NULL )
	{
		iUnion = (lpObj->lpGuild->iGuildUnion == 0)? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;
	}

	return iUnion;

}

void gObjGetGuildUnionName(LPOBJ lpObj, char* szUnionName, int iUnionNameLen)
{
	szUnionName[0] = 0;
	
	if ( lpObj->lpGuild == NULL )
	{
		return;
	}

	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		return;
	}

	TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpObj->lpGuild->iGuildUnion);

	if ( pUnionInfo != NULL )
	{
		memcpy(szUnionName, pUnionInfo->m_szMasterGuild, iUnionNameLen);
	}
}

BOOL gObjCheckRival(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpTargetObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpTargetObj->lpGuild == 0 || lpObj->lpGuild == 0)
	{
		return false;
	}

	if(lpTargetObj->lpGuild->iGuildRival == 0 || lpObj->lpGuild->iGuildRival == 0)
	{
		return false;
	}

	int iUnion = !lpObj->lpGuild->iGuildUnion ? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;

	if(UnionManager.GetGuildRelationShip(iUnion,lpTargetObj->lpGuild->Number) == 2)
	{
		return true;
	}
	return false;
}

int gObjGetRelationShip(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( lpObj == NULL || lpTargetObj == NULL )
	{
		return false;
	}

	if ( lpObj->lpGuild == NULL || lpTargetObj->lpGuild == NULL )
	{
		return 0;
	}

	int iUnion = (!lpObj->lpGuild->iGuildUnion) ? lpObj->lpGuild->Number : lpObj->lpGuild->iGuildUnion;

	return UnionManager.GetGuildRelationShip(iUnion, lpTargetObj->lpGuild->Number);
}

struct PMSG_UNION_VIEWPORT_NOTIFY_COUNT
{
	struct PWMSG_HEAD h;
	BYTE btCount;
};

struct PMSG_UNION_VIEWPORT_NOTIFY
{
	BYTE btNumberH;
	BYTE btNumberL;
	int iGuildNumber;
	BYTE btGuildRelationShip;
	char szUnionName[8];
};

void gObjNotifyUpdateUnionV1(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	char cBUFFER_V1[6000];
	int iVp1Count = 0;

	memset(cBUFFER_V1,0x00,sizeof(cBUFFER_V1));

	PMSG_UNION_VIEWPORT_NOTIFY_COUNT * lpMsg = (PMSG_UNION_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V1;
	PMSG_UNION_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_UNION_VIEWPORT_NOTIFY * )&cBUFFER_V1[sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT)];


	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(!OBJMAX_RANGE(lpObj->VpPlayer[n].number))
		{
			continue;
		}

		if(lpObj->VpPlayer[n].state == 2 && lpObj->VpPlayer[n].type == OBJ_USER)
		{
			LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer[n].number];

			if(lpTargetObj == 0)
			{
				continue;
			}

			if(lpTargetObj->lpGuild == 0)
			{
				continue;
			}

			gObjGetGuildUnionName(lpTargetObj,lpMsgBody[iVp1Count].szUnionName,sizeof(lpMsgBody[iVp1Count].szUnionName));

			lpMsgBody[iVp1Count].btGuildRelationShip = gObjGetRelationShip(lpObj,lpTargetObj);
			lpMsgBody[iVp1Count].btNumberL = SET_NUMBERL(WORD(lpTargetObj->m_Index));
			lpMsgBody[iVp1Count].btNumberH = SET_NUMBERH(WORD(lpTargetObj->m_Index));
			lpMsgBody[iVp1Count].iGuildNumber = lpTargetObj->lpGuild->Number;
			iVp1Count++;
		}
	}

	if(iVp1Count > 0 && iVp1Count <= MAX_VIEWPORT)
	{
		lpMsg->btCount = iVp1Count;

		PHeadSetW((LPBYTE)lpMsg,0x67,iVp1Count * sizeof(PMSG_UNION_VIEWPORT_NOTIFY) + sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT));

		DataSend(lpObj->m_Index,(LPBYTE)lpMsg,((lpMsg->h.sizeL & 0xFF) & 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
	else
	{
		if(iVp1Count != 0)
		{
			LogAddTD("[Union ViewPort] ERROR : iVp1Count is OUT of BOUND: %d",iVp1Count);
		}
	}
}

void gObjNotifyUpdateUnionV2(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	if(lpObj->lpGuild == 0)
	{
		return;
	}

	char cBUFFER_V2[100] = {0};

	PMSG_UNION_VIEWPORT_NOTIFY_COUNT * lpMsg2 = (PMSG_UNION_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V2;
	PMSG_UNION_VIEWPORT_NOTIFY * lpMsgBody2 = (PMSG_UNION_VIEWPORT_NOTIFY *)&cBUFFER_V2[sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT)];

	gObjGetGuildUnionName(lpObj,lpMsgBody2->szUnionName,sizeof(lpMsgBody2->szUnionName));
	lpMsgBody2->btNumberL = SET_NUMBERL(WORD(lpObj->m_Index));
	lpMsgBody2->btNumberH = SET_NUMBERH(WORD(lpObj->m_Index));

	lpMsgBody2->iGuildNumber = lpObj->lpGuild->Number;
	lpMsgBody2->btGuildRelationShip = 0;
	lpMsg2->btCount = 1;

	PHeadSetW((LPBYTE)lpMsg2,0x67,sizeof(lpMsg2) + sizeof(lpMsgBody2[0])+1);

	DataSend(lpObj->m_Index,(LPBYTE)lpMsg2,((lpMsg2->h.sizeL & 0xFF) & 0xFF | ((lpMsg2->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_USER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer2[n].number];

			if(lpTargetObj->lpGuild != 0)
			{
				lpMsgBody2->btGuildRelationShip = gObjGetRelationShip(lpTargetObj,lpObj);
			}

			if(lpMsgBody2->btGuildRelationShip != 1)
			{
				DataSend(lpObj->VpPlayer2[n].number,(LPBYTE)lpMsg2,((lpMsg2->h.sizeL & 0xFF) & 0xFF | ((lpMsg2->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
			}
		}
	}
}

void gObjUnionUpdateProc(int iIndex)
{

	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->RegenOk > 0 )
	{
		return;
	}

	if ( lpObj->CloseCount > -1 )
		return;

	if ( lpObj->lpGuild == NULL )
	{
		return;
	}

	if ( lpObj->lpGuild->CheckTimeStamp(lpObj->iGuildUnionTimeStamp) != FALSE )
	{
		return;
	}

	lpObj->iGuildUnionTimeStamp = lpObj->lpGuild->GetTimeStamp();
	gObjNotifyUpdateUnionV1(lpObj);
	gObjNotifyUpdateUnionV2(lpObj);

}

void gObjSetKillCount(int aIndex, int iOption)
{
	if ( gObjIsConnected(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( iOption== KILLCOUNT_INC )
	{
		if ( lpObj->m_btKillCount <= 254 )
		{
			lpObj->m_btKillCount++;
		}
	}
	else if ( iOption == KILLCOUNT_DEC )
	{
		if ( lpObj->m_btKillCount > 0 )
		{
			lpObj->m_btKillCount--;
		}
	}
	else if ( iOption == KILLCOUNT_RESET )
	{
		lpObj->m_btKillCount = 0;
	}

	PMSG_KILLCOUNT pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xB8,0x01, sizeof(pMsg));
	pMsg.btKillCount = lpObj->m_btKillCount;
	
	DataSend( aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

struct PMSG_NOTIFY_REGION_OF_WEAPON
{
	struct PBMSG_HEAD2 h;
	BYTE btWeaponType;
	BYTE btPointX;
	BYTE btPointY;
};

struct PMSG_NOTIFY_TARGET_OF_WEAPON
{
	struct PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE btWeaponType;
};

struct PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT
{
	struct PWMSG_HEAD h;
	BYTE btCount;
};

struct PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY
{
	BYTE btObjType;
	BYTE btObjClassH;
	BYTE btObjClassL;
	BYTE btObjIndexH;
	BYTE btObjIndexL;
	BYTE btX;
	BYTE btY;
	BYTE CharSet[17]; //lol
	BYTE btViewSkillStateCount;
	BYTE btViewSkillState[MAX_STATE_COUNT];
};

void gObjNotifyUseWeaponV1(LPOBJ lpOwnerObj, LPOBJ lpWeaponObj, int iTargetX, int iTargetY)
{
	if(lpOwnerObj == 0)
	{
		return;
	}

	if(lpWeaponObj == 0)
	{
		return;
	}

	PMSG_NOTIFY_REGION_OF_WEAPON pNotifyRegionMsg = {0};

	PHeadSubSetB((LPBYTE)&pNotifyRegionMsg,0xB7,2,sizeof(pNotifyRegionMsg));

	pNotifyRegionMsg.btPointX = iTargetX;
	pNotifyRegionMsg.btPointY = iTargetY;

	PMSG_NOTIFY_TARGET_OF_WEAPON pNotifyTargetMsg = {0};

	PHeadSubSetB((LPBYTE)&pNotifyTargetMsg,0xB7,3,sizeof(pNotifyTargetMsg));

	if(lpWeaponObj->Class == 221)
	{
		pNotifyRegionMsg.btWeaponType = 1;
		pNotifyTargetMsg.btWeaponType = 1;
	}
	else if(lpWeaponObj->Class == 222)
	{
		pNotifyRegionMsg.btWeaponType = 2;
		pNotifyTargetMsg.btWeaponType = 2;
	}

	char cBUFFER_V1[6000];
	int iVp1Count = 0;

	memset(cBUFFER_V1,0x00,sizeof(cBUFFER_V1));

	PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT * lpMsg = (PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V1;
	PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY *)&cBUFFER_V1[sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT)];

	for(int n = 0; n < OBJMAX;n++)
	{
		LPOBJ lpTargetObj = &gObj[n];

		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		if(lpOwnerObj->MapNumber != lpTargetObj->MapNumber)
		{
			continue;
		}

		if(abs(lpTargetObj->X - iTargetX) > 6)
		{
			continue;
		}

		if(abs(lpTargetObj->Y - iTargetY) > 6)
		{
			continue;
		}

		lpMsgBody[iVp1Count].btObjClassH = SET_NUMBERH(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjClassL = SET_NUMBERL(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjIndexH = SET_NUMBERH(lpTargetObj->m_Index);
		lpMsgBody[iVp1Count].btObjIndexH = SET_NUMBERL(lpTargetObj->m_Index);
		lpMsgBody[iVp1Count].btX = lpTargetObj->X;
		lpMsgBody[iVp1Count].btY = lpTargetObj->Y;

		lpMsgBody[iVp1Count].btViewSkillStateCount = gObjMakeViewportState(lpTargetObj, lpMsgBody[iVp1Count].btViewSkillState); //Season3 changed

		if(lpTargetObj->Type == OBJ_USER)
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_USER;
			lpMsgBody[iVp1Count].btObjClassH = CS_SET_CLASS(lpTargetObj->Class);
			lpMsgBody[iVp1Count].btObjClassL = 0;

			memcpy(lpMsgBody[iVp1Count].CharSet,&lpTargetObj->CharSet[1],sizeof(lpMsgBody[iVp1Count].CharSet));
		}
		else if(lpTargetObj->Type == OBJ_MONSTER)
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_MONSTER;
			memset(lpMsgBody[iVp1Count].CharSet,0x00,sizeof(lpMsgBody[iVp1Count].CharSet));
		}
		else
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_NPC;
			memset(lpMsgBody[iVp1Count].CharSet,0x00,sizeof(lpMsgBody[iVp1Count].CharSet));
		}

		iVp1Count++;

		if(lpTargetObj->Type == OBJ_USER)
		{
			DataSend(lpTargetObj->m_Index,(LPBYTE)&pNotifyRegionMsg,sizeof(pNotifyRegionMsg));

			if(abs(lpTargetObj->X - iTargetX) > 3)
			{
				continue;
			}

			if(abs(lpTargetObj->Y - iTargetY) > 3)
			{
				continue;
			}

			pNotifyTargetMsg.NumberH = SET_NUMBERH(lpTargetObj->m_Index);
			pNotifyTargetMsg.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
		}
	}

	if(iVp1Count > 0 && iVp1Count <= 100)
	{
		lpMsg->btCount = iVp1Count;
		PHeadSetW((LPBYTE)lpMsg,0x68,iVp1Count * sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY) + sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT));

		DataSend(lpOwnerObj->m_Index,(LPBYTE)lpMsg,((lpMsg->h.sizeL & 0xFF) & 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
}

void gObjNotifyUseWeaponDamage(LPOBJ lpWeaponObj, int iTargetX, int iTargetY)
{
	if ( lpWeaponObj->Class == 221 || lpWeaponObj->Class == 222)
	{
		for ( int i=0;i<OBJMAX;i++)
		{
			LPOBJ lpObj = &gObj[i];

			if ( lpWeaponObj->MapNumber != lpObj->MapNumber )
			{
				continue;
			}

			if ( abs(lpObj->X - iTargetX) > 3 )
			{
				continue;
			}

			if ( abs(lpObj->Y - iTargetY) > 3 )
			{
				continue;
			}

			g_CsNPC_Weapon.AddWeaponDamagedTargetInfo(lpWeaponObj->m_Index, i, 100);
		}
	}

}

void gObjUseBlessAndSoulPotion(int aIndex, int iItemLevel)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( iItemLevel == 0 )	// Bless Potion
	{
		gObjAddBuffEffect(lpObj, AT_POTION_OF_BLESS, 0, 0, 0, 0, 120);
	}
	else if ( iItemLevel == 1 ) // Soul Potion
	{
		gObjAddBuffEffect(lpObj, AT_POTION_OF_SOUL, 0, 0, 0, 0, 60);
		gObjUseDrink(lpObj, 77);
	}
}

//005a24c0
void gObjWeaponDurDownInCastle(LPOBJ lpObj, LPOBJ lpTargetObj, int iDecValue)
{
	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		return;
	}

	int itargetdefence = 0;

	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];

	int bIsRightDurDown = 0;
	int bIsLeftDurDown = 0;


	if(lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_DARKLORD)
	{
		if(Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0)
			&& Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(4,0))
		{
			bIsRightDurDown = 1;
			bIsLeftDurDown = 1;
		}
	}

	if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7)
		|| Left->m_Type == ITEMGET(4,20)
		|| Left->m_Type == ITEMGET(4,21)
		//season4 add-on
		|| Left->m_Type == ITEMGET(4,22)
		|| Left->m_Type == ITEMGET(4,23)
		|| Left->m_Type == ITEMGET(4,24))
	{
		bIsLeftDurDown = 1;
	}
	else if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) || Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0))
	{
		bIsRightDurDown = 1;
	}
	else if(Right->m_Type >= 0 && Right->m_Type < ITEMGET(4,0))
	{
		bIsRightDurDown = 1;
	}
	else if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
	{
		bIsRightDurDown = 1;
	}

	if(bIsRightDurDown != 0)
	{
		int iRet = Right->SimpleDurabilityDown(iDecValue);

		if(iRet != 0)
		{
			GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);

			if(iRet == 2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
	}

	if(bIsLeftDurDown != 0)
	{
		int iRet = Right->SimpleDurabilityDown(iDecValue);

		if(iRet != 0)
		{
			GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);

			if(iRet == 2)
			{
				gObjCalCharacter(lpObj->m_Index);
			}
		}
	}
}

void gObjReady4Relife(LPOBJ lpObj)
{
	gObjCalCharacter(lpObj->m_Index);
	GCReFillSend(lpObj->m_Index,lpObj->MaxLife + lpObj->AddLife,0xFE,0,lpObj->iMaxShield + lpObj->iAddShield);
}

//pcbang connection
#if(TESTSERVER == 0)
BOOL g_bPCBangServer = FALSE;
#else
BOOL g_bPCBangServer = TRUE;
#endif

//00554B60
int gObjCheckUserPCBang(LPOBJ lpObj) //season4.5 add-on
{
#ifdef OPT
	if(g_bPCBangServer == FALSE)
		return 0;

	if(lpObj->m_iPcBangRoom == 0)
		return 0;

	if(BC_MAP_RANGE(lpObj->MapNumber) != FALSE)
	{
		return 0;
	}
	
	if(CC_MAP_RANGE(lpObj->MapNumber) != FALSE)
	{
		return 0;
	}
	
	if(DS_MAP_RANGE(lpObj->MapNumber) != FALSE)
	{
		return 0;
	}
	
	return lpObj->m_iPcBangRoom;
#else
	return 0;
#endif
}

//00554CE0
BOOL IsOKPCBangBenefitAll(LPOBJ lpObj) //season4.5 add-on
{
#ifdef OPT
	if(g_bPCBangServer == FALSE)
		return FALSE;

	if(lpObj->m_iPcBangRoom == 0)
		return FALSE;

	return TRUE;
#else
	return false;
#endif
}

void gObjCheckTimeOutValue(LPOBJ lpObj, DWORD& rNowTick)
{
	long lTick;
	long lSecond;

	lTick = rNowTick - lpObj->m_dwLastCheckTick;
	lSecond = lTick / 1000;

	if(lSecond == 0)
	{
		return;
	}

	lpObj->m_dwLastCheckTick = rNowTick;
}

void MsgOutput(int aIndex, char* msg, ...)
{
	char szBuffer[512]="";
	va_list pArguments;

	va_start(pArguments, msg );
	vsprintf(&szBuffer[0], msg, pArguments);
	va_end(pArguments);
	
	GCServerMsgStringSend(&szBuffer[0], aIndex, 1);
}

void gProcessAutoRecuperation(LPOBJ lpObj)
{
	if(lpObj->Life == (lpObj->MaxLife + lpObj->AddLife)
		&& lpObj->Mana == (lpObj->MaxMana + lpObj->AddMana)
		&& lpObj->BP == (lpObj->MaxBP + lpObj->AddBP) )
	{
		lpObj->m_iAutoRecuperationTime = GetTickCount();
		return;
	}

	if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= CART_10_SEC && ( GetTickCount() - lpObj->m_iAutoRecuperationTime ) < CART_15_SEC )
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 1.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 1.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += VPT_HP_MP_AG_1_PER_1_SEC;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
	else if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= CART_15_SEC && ( GetTickCount() - lpObj->m_iAutoRecuperationTime ) < CART_25_SEC )
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 5.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 5.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += VPT_HP_MP_AG_5_PER_1_SEC;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
	else if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= CART_25_SEC )
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 10.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 10.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += VPT_HP_MP_AG_10_PER_1_SEC;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
}

void gObjShieldAutoRefill(LPOBJ lpObj)
{
	int iRefillPoint = 0;
	int iShieldRefillOption = 0;

	if(g_ShieldSystemOn == 0)
	{
		return;
	}

	if(g_ShieldAutoRefillOn == 0)
	{
		return;
	}

	if(g_ShieldAutoRefillOnSafeZone == 1)
	{
		BYTE btMapAttribute = MapC[lpObj->MapNumber].GetAttr(lpObj->X,lpObj->Y);

		if((btMapAttribute & 1) != 1 && lpObj->m_ItemOptionExFor380.OpRefillOn == 0)
		{
			lpObj->dwShieldAutoRefillTimer = GetTickCount();
			return;
		}
	}

	if(lpObj->iShield >= (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->dwShieldAutoRefillTimer = GetTickCount();
		return;
	}

	iShieldRefillOption = lpObj->m_ItemOptionExFor380.OpAddRefillSD;

	int iRefillExpression = 0;

	float fRefillExpressionA = (lpObj->iMaxShield + lpObj->iAddShield) / 30;
	float fRefillExpressionB = iShieldRefillOption + 100;

	iRefillExpression = ((fRefillExpressionA * fRefillExpressionB) / 100.0f) / 25.0f;

	DWORD dwTick = GetTickCount() - lpObj->dwShieldAutoRefillTimer;

	if(dwTick >= 25000)
	{
		iRefillPoint = iRefillExpression * 3; 
	}
	else if(dwTick >= 15000)
	{
		iRefillPoint = (iRefillExpression * 25) / 10;
	}
	else if(dwTick >= 10000)
	{
		iRefillPoint = iRefillExpression * 2;
	}
	else
	{
		return;
	}

	iRefillPoint++; //season4 changed (fix)

	if(iRefillPoint == 0)
	{
		return;
	}

	iRefillPoint += iRefillPoint * lpObj->m_MPSkillOpt.MpsSDSpeed / 100;

	lpObj->iShield += iRefillPoint;

	if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
	}

	GCReFillSend(lpObj->m_Index,lpObj->Life,0xFF,0,lpObj->iShield);
}

int gObjCheckOverlapItemUsingDur(int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel)
{
	for(int x = INVETORY_WEAR_SIZE; x < MAIN_INVENTORY_SIZE; x++)
	{
		if(gObj[iUserIndex].pInventory[x].IsItem() == 1
			&& gObj[iUserIndex].pInventory[x].m_Type == (short)iItemType
			&& gObj[iUserIndex].pInventory[x].m_Level == (short)iItemLevel)
		{
			int iITEM_DUR = gObj[iUserIndex].pInventory[x].m_Durability;

			if((((iITEM_DUR)<0)?FALSE:((iITEM_DUR)>iMaxOverlapped-1)?FALSE:TRUE ))
			{
				return x;
			}
		}
	}

	return -1;
}

int gObjOverlapItemUsingDur(class CItem* lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel)
{
	for(int iLoop = 0; iLoop < MAIN_INVENTORY_SIZE; iLoop ++)
	{
		int iInventoryIndex = gObjCheckOverlapItemUsingDur(iUserIndex,iMaxOverlapped,iItemType,iItemLevel);
		if(MAIN_INVENTORY_RANGE(iInventoryIndex))
		{
			int iItemDur = gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability + lpItem->m_Durability;

			if(iItemDur <= iMaxOverlapped)
			{
				if(MapC[iMapNumber].ItemGive(iUserIndex,iItemNumber,1) == 1)
				{
					return iInventoryIndex;
				}
			}
			else
			{
				lpItem->m_Durability = iItemDur - iMaxOverlapped;
				gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability = iMaxOverlapped;

				GCItemDurSend(iUserIndex,iInventoryIndex,gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability,0);
			}
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

int gObjCheckSerial0ItemList(class CItem* lpItem)
{
	if(gItemSerialCheck == 0)
	{
		return false;
	}

	if(lpItem->m_Type == ITEMGET(14,13)
		|| lpItem->m_Type == ITEMGET(14,14)
		|| lpItem->m_Type == ITEMGET(14,16)
		|| lpItem->m_Type == ITEMGET(14,22)
		|| lpItem->m_Type == ITEMGET(12,15)
		|| lpItem->m_Type == ITEMGET(12,30)
		|| lpItem->m_Type == ITEMGET(12,31)
		|| lpItem->m_Type == ITEMGET(14,31))
	{
		if(lpItem->GetNumber() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

int gObjCheckInventorySerial0Item(LPOBJ lpObj)
{
	int iItemSerial;
	int iCount;
	int i;

	if(gItemSerialCheck == 0)
	{
		return false;
	}

	iCount = 0;

	for(i = 0; i < INVENTORY_SIZE; i++)
	{
		if(lpObj->pInventory[i].IsItem() == 0)
		{
			continue;
		}

		if(gObjCheckSerial0ItemList(&lpObj->pInventory[i])==0)
		{
			continue;
		}

		iItemSerial = lpObj->pInventory[i].GetNumber();

		if(iItemSerial == 0)
		{
			LogAddTD("[ANTI-HACK][Serial 0 Item] (%s)(%s) Item(%s) Pos(%d)", lpObj->AccountID,lpObj->Name,lpObj->pInventory[i].GetName(),i);
			iCount++;
		}
	}

	if(iCount > 0)
	{
		return true;
	}
	
	return false;
}

BOOL gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum)
{
	if ( !g_iSkillDistanceCheck  )
		return TRUE;

	if ( iSkillNum == 40 )
		return TRUE;

	int iSkillDistance = MagicDamageC.GetSkillDistance(iSkillNum);

	if ( iSkillDistance == -1 )
		return FALSE;

	iSkillDistance += g_iSkillDistanceCheckTemp;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( abs(lpObj->X - lpTargetObj->X) > iSkillDistance || abs(lpObj->Y - lpTargetObj->Y) > iSkillDistance )
	{
		LogAddTD("[SKILL DISTANCE CHECK] [%s][%s] Invalid Skill attacker(%d, %d), defender(%d, %d), skillnum:%d skilldistance:%d",
			lpObj->AccountID, lpObj->Name,
			lpObj->X, lpObj->Y, 
			lpTargetObj->X, lpTargetObj->Y,
			iSkillNum, iSkillDistance);

		return FALSE;
	}

	return TRUE;
}

void gObjSaveChaosBoxItemList(LPOBJ lpObj)
{
	BYTE ExOption[0x8];
	for(int n = 0; n < CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->pChaosBox[n].IsItem() == 1)
		{
			ItemIsBufExOption(ExOption,&lpObj->pChaosBox[n]);

			LogAddTD("[ChaosBoxItemList][Lost ItemList] [%s][%s] [%d,%s,%d,%d,%d,%d] Serial:[%u][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
				lpObj->AccountID,lpObj->Name,n,lpObj->pChaosBox[n].GetName(),lpObj->pChaosBox[n].m_Level,lpObj->pChaosBox[n].m_Option1,lpObj->pChaosBox[n].m_Option2,lpObj->pChaosBox[n].m_Option3,lpObj->pChaosBox[n].m_Number,
				(int)lpObj->pChaosBox[n].m_Durability,ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}

//identical
BOOL gObjCheckRaiseTalismanMap(int MapNumber) //00556200
{
	switch(MapNumber)
	{
	case MAP_INDEX_RORENCIA:
	case MAP_INDEX_DUNGEON:
	case MAP_INDEX_DEVIAS:
	case MAP_INDEX_NORIA:
	case MAP_INDEX_LOSTTOWER:
	case MAP_INDEX_BATTLESOCCER:
	case MAP_INDEX_ATHLANSE:
	case MAP_INDEX_TARKAN:
	case MAP_INDEX_ICARUS:
	case MAP_INDEX_AIDA:
	case MAP_INDEX_KANTURU1:
	case MAP_INDEX_KANTURU2:
	case MAP_INDEX_ELBELAND:
	case MAP_INDEX_SWAMP_OF_CALMNESS:
	case MAP_INDEX_RAKLION_FIELD:
	case MAP_INDEX_VULCANUS: //season4.6
		return TRUE;
	}

	return FALSE;
}

#ifndef NPVP
//Season 4.6
CDuelRoom g_DuelRoom;

CDuelRoom::CDuelRoom()
{
	return;
}

CDuelRoom::~CDuelRoom()
{
	return;
}

void CDuelRoom::Init()
{
	if(g_iNewPVPSystem == 0)
	{
		LogAddTD("[OPTION] NEW PVP System is Disabled");
		return;
	}

	for(int i =0;i< MAX_DUEL_ROOMS;i++)
	{
		this->ClearRoom(i);
	}

	m_dwUpdateTickCount = 0;
}

void CDuelRoom::ClearRoom(BYTE btRoom)
{
	if(btRoom < 0 || btRoom >= MAX_DUEL_ROOMS) 
	{
		return;
	}

	LPOBJ lpUserObj = NULL;
	LPOBJ lpSpecObj = NULL;

	for(int i=0;i< MAX_DUEL_USERS;i++)
	{
		lpUserObj = &gObj[this->m_Zone[btRoom].UserIndex[i]];

		if(lpUserObj->m_Index != NULL)
		{
			if(gObjIsConnected(lpUserObj->m_Index))
			{
				if(lpUserObj->MapNumber == MAP_INDEX_PKFIELD)
				{
					gObjMoveGate(lpUserObj->m_Index, 294);
				}
				
				GCSendEndDuel(lpUserObj);
			}
			
			gObjResetDuel(lpUserObj);
		}

		this->m_Zone[btRoom].UserIndex[i] = 0;
	}

	for(int i=0;i< MAX_DUEL_SPECTATORS;i++)
	{
		lpSpecObj = &gObj[this->m_Zone[btRoom].SpectatorIndex[i]];

		if(lpSpecObj->m_Index != NULL)
		{
			GCSendEndDuel(lpSpecObj);

			gObjRemoveBuffEffect(lpSpecObj, AT_INVISIBILITY);
			gObjRemoveBuffEffect(lpSpecObj,	AT_NEWPVPSYSTEM_WATCH_DUEL);
			gObjRemoveBuffEffect(lpSpecObj, AT_CW_NPC_TRANSPARENCY);
			gObjViewportListProtocolCreate(lpSpecObj);
			
			gObjMoveGate(lpSpecObj->m_Index, 294);
		}

		this->m_Zone[btRoom].SpectatorIndex[i] = 0;
	}
				
	this->m_Zone[btRoom].btOpen = 1;
	this->m_Zone[btRoom].dwClearRoomTime = 0;
	this->m_Zone[btRoom].m_dwUpdateLifebarTime = 0;
	this->m_Zone[btRoom].bWaiting = FALSE;

	LogAddTD("[Duel Room] Zone [%d] was cleaned", btRoom+1);
}

void CDuelRoom::DuelZoneProcess()
{
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if(this->m_Zone[i].btOpen == 0 && GetTickCount() - gObj[this->m_Zone[i].UserIndex[0]].m_iDuelTickCount >= 5000) 
		{
			for(int n = 0; n < MAX_DUEL_SPECTATORS; n++)
			{
				if(this->m_Zone[i].SpectatorIndex[n] == NULL) continue;

				if(gObjIsConnected(this->m_Zone[i].SpectatorIndex[n]))
				{
					LPOBJ lpSpecObj = &gObj[this->m_Zone[i].SpectatorIndex[n]];

					if(lpSpecObj->MapNumber != MAP_INDEX_PKFIELD)
					{
						GCSendEndDuel(lpSpecObj);
						
						gObjRemoveBuffEffect(lpSpecObj, AT_INVISIBILITY);
						gObjRemoveBuffEffect(lpSpecObj,	AT_NEWPVPSYSTEM_WATCH_DUEL);
						gObjRemoveBuffEffect(lpSpecObj, AT_CW_NPC_TRANSPARENCY);
						GCSendDuelSpectatorRemove(n, i);

						LogAddTD("[Duel Room] Spectator [%s] has left the room %d", lpSpecObj->Name, i + 1);
						
						this->m_Zone[i].SpectatorIndex[n] = 0;
					}
				}
				else
				{
					this->m_Zone[i].SpectatorIndex[n] = 0;
					
					for(int k = 0; k < MAX_DUEL_SPECTATORS; k++)
					{
						if(this->m_Zone[i].SpectatorIndex[k] != NULL)
						{
							GCSendDuelSpectatorList(&gObj[this->m_Zone[i].SpectatorIndex[k]], i);
						}
					}
				}
			}

			for(int n = 0; n < MAX_DUEL_USERS; n++)
			{
				
				if(this->m_Zone[i].UserIndex[n] == 0 )
				{
					this->ClearRoom(i);
					continue;
				}
					
				LPOBJ lpUserObj = &gObj[this->m_Zone[i].UserIndex[n]];

				if(gObjIsConnected(lpUserObj->m_Index) == FALSE ||
					lpUserObj->MapNumber != MAP_INDEX_PKFIELD ||
					lpUserObj->Connected < PLAYER_PLAYING)
				{
					this->ClearRoom(i);
					continue;
				}
			}

			if(this->m_Zone[i].m_dwUpdateLifebarTime < GetTickCount())
			{
				for(int k = 0; k < MAX_DUEL_SPECTATORS; k++)
				{
					if(this->m_Zone[i].SpectatorIndex[k] != 0)
					{
						GCSendDuelLifebarStatus(&gObj[this->m_Zone[i].SpectatorIndex[k]], i);
					}
				}
				this->m_Zone[i].m_dwUpdateLifebarTime = GetTickCount() + 2000;
			}

			if(this->m_Zone[i].dwClearRoomTime != 0)
			{
				if(this->m_Zone[i].dwClearRoomTime < GetTickCount())
				{
					this->ClearRoom(i);
				}
			}
		}


	}
	
	if(this->m_dwUpdateTickCount < GetTickCount())
	{
		for(int i = OBJ_STARTUSERINDEX; i < OBJMAX; i++)
		{
			if((gObj[i].m_IfState.use) && gObj[i].m_IfState.type == 20)
			{
				this->NotifyDuelRoomsInfo(&gObj[i]);
			}
		}
		this->m_dwUpdateTickCount = GetTickCount() + 6000;
	}

//	this->CheckInvalidUserOnDuelRoom();
}

BYTE CDuelRoom::GetFreeRoom()
{
	BYTE btRoom = -1;

	for(int i = 0; i < MAX_DUEL_ROOMS;i++)
	{
		if(this->m_Zone[i].btOpen == 1)
		{
			if(this->m_Zone[i].bWaiting == FALSE)
			{
				btRoom = i;
				break;
			}
		}
	}

	return btRoom;
}

BYTE CDuelRoom::GetSpectatorCount(BYTE btRoom)
{
	int iCount = 0;

	for(int i = 0; i < MAX_DUEL_SPECTATORS;i++)
	{
		if(this->m_Zone[btRoom].SpectatorIndex[i] >= OBJ_STARTUSERINDEX && this->m_Zone[btRoom].SpectatorIndex[i] <= OBJMAX)
		{
			iCount++;
		}
	}

	return iCount;
}

BYTE CDuelRoom::IsSpectator(int aIndex,BYTE btRoom)
{
	for(int i = 0;i < MAX_DUEL_SPECTATORS;i++)
	{
		LogAdd("specindex %d aIndex %d Room %d",this->m_Zone[btRoom].SpectatorIndex[i],aIndex,btRoom);
		if(this->m_Zone[btRoom].SpectatorIndex[i] == aIndex)
		{
			return 1;
		}
	}
	
	return 0;
}

BYTE CDuelRoom::IsUser(int aIndex,BYTE btRoom)
{
	if(this->m_Zone[btRoom].UserIndex[0] == aIndex || this->m_Zone[btRoom].UserIndex[1] == aIndex)
	{

			return 1;
	}
	
	return 0;
}

BYTE CDuelRoom::GetUserDuelRoom(LPOBJ lpObj)
{
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		for(int n = 0;n < MAX_DUEL_USERS;n++)
		{
			if(this->m_Zone[i].UserIndex[n] == lpObj->m_Index)
			{
				return i;
			}
		}
	}

	return -1;
}

BYTE CDuelRoom::GetSpectatorDuelRoom(LPOBJ lpObj)
{
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		for(int n = 0;n < MAX_DUEL_SPECTATORS;n++)
		{
			if(this->m_Zone[i].SpectatorIndex[n] == lpObj->m_Index)
			{
				return i;
			}
		}
	}

	return -1;
}

void CDuelRoom::SetDoorKeeperNpcIndex(int aIndex)
{
	this->m_iDoorKeeperNpcIndex = aIndex;
}

void CDuelRoom::NotifyDuelRoomsInfo(LPOBJ lpObj)
{
	PMSG_REQ_DUEL_STATUS pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xAA;
	pMsg.h.subcode = 0x06;

	ZeroMemory(&pMsg.pRoomStatus, sizeof(pMsg.pRoomStatus));
	
	for(int i = 0; i < MAX_DUEL_ROOMS; i++)
	{
		if(g_DuelRoom.m_Zone[i].btOpen == FALSE)
		{
			if( g_DuelRoom.m_Zone[i].UserIndex[0] == 0 ||
				g_DuelRoom.m_Zone[i].UserIndex[1] == 0 )
			{
				continue;
			}

			pMsg.pRoomStatus[i].btDuelRunning = TRUE;

			int iSpecCount = g_DuelRoom.GetSpectatorCount(i);

			if(iSpecCount < 0 || iSpecCount >= 10)
			{
				pMsg.pRoomStatus[i].btDuelOpen = FALSE;
			}
			else
			{
				pMsg.pRoomStatus[i].btDuelOpen = TRUE;
			}

			memcpy(&pMsg.pRoomStatus[i].szName1[0], &gObj[g_DuelRoom.m_Zone[i].UserIndex[0]].Name[0], 10);
			memcpy(&pMsg.pRoomStatus[i].szName2[0], &gObj[g_DuelRoom.m_Zone[i].UserIndex[1]].Name[0], 10);
		}
	}

	DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
}
#endif

int gObjDeleteItemDur(int aIndex, OBJECTSTRUCT * lpObj, int ItemCode, int DurCount)
{
	int ItemCount = 0;
	// ----
	for(int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++)
	{
		if(lpObj->pInventory[i].m_Type == ItemCode)
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], i, 1) )
			{
				gObjInventoryItemSet(aIndex, i, -1);
				gObj[aIndex].pInventory[i].Clear();
				GCInventoryItemDeleteSend(aIndex, i, 1);
				// ----
				ItemCount++;
			}
			if(ItemCount == DurCount) { break; };
		}
	}
	// ----
	return ItemCount;
}

int gObjGetItemDur(int aIndex, OBJECTSTRUCT * lpObj, int ItemCode)
{
	int ItemCount = 0;
	// ----
	for(int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++)
	{
		if(lpObj->pInventory[i].m_Type == ItemCode)
		{
			ItemCount++;
		}
	}
	// ----
	return ItemCount;
}