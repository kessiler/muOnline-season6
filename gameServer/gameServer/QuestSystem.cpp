#include "StdAfx.h"
#include "QuestSystem.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
#include "GameMain.h"
#include "LogToFile.h"
#include "GMMng.h"
#include "ShopPointEx.h"
#include "DSProtocol.h"
// -------------------------------------------------------------------------------

#ifdef QUESTSYSTEM

CLogToFile	QuestLog("Quest", ".\\LOG\\Quest", TRUE);
CLogToFile	QuestJobLog("QuestJob", ".\\LOG\\Quest", TRUE);
// -------------------------------------------------------------------------------

QuestSystem g_QuestSystem;
// -------------------------------------------------------------------------------

QuestSystem::QuestSystem()
{
	this->Init();
}
// -------------------------------------------------------------------------------

QuestSystem::~QuestSystem()
{

}
// -------------------------------------------------------------------------------

void QuestSystem::Init()
{
	this->m_Scene.clear();
}
// -------------------------------------------------------------------------------

void QuestSystem::Load()
{
	this->Init();
	this->ReadManagerList(gDirPath.GetNewPath("Custom\\QuestManager.dat"));
	// ----
	for( int i = 0; i < this->m_Scene.size(); i++ )
	{
		this->ReadSceneList(&this->m_Scene[i]);
	}
	// ----
	for( int i = 0; i < this->m_Scene.size(); i++ )
	{
		for( int j = 0; j < this->m_Scene[i].EpisodeCount; j++ )
		{
			char EpisodeFile[MAX_PATH] = { 0 };
			sprintf(EpisodeFile, "%s\\%s\\Episode\\%d.dat", gDirPath.GetNewPath("Custom\\Scenes"),
				this->m_Scene[i].Folder, j+1);
			QuestEpisode NewQuest;
			NewQuest.Index = j;
			this->m_Scene[i].Episode.push_back(NewQuest);
			this->ReadEpisode(&this->m_Scene[i].Episode[j], EpisodeFile);
		}
	}
}
// -------------------------------------------------------------------------------

void QuestSystem::ReadManagerList(LPSTR File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox("[QuestManager] %s file not found", File);
		QuestJobLog.Output("[QuestManager] %s file not found", File);
		return;
	}
	// ----
	while(true)
	{
		SMDToken Token = GetToken();
		// ----
		if( Token == END || strcmp("end", TokenString) == 0 )
		{
			break;
		}
		// ----
		QuestManager pData;
		// ----
		pData.Index		= (BYTE)TokenNumber;
		// ----
		Token = GetToken();
		pData.ClassID	= (WORD)TokenNumber;
		// ----
		Token = GetToken();
		pData.MapNumber	= (BYTE)TokenNumber;
		// ----
		Token = GetToken();
		pData.X			= (BYTE)TokenNumber;
		// ----
		Token = GetToken();
		pData.Y			= (BYTE)TokenNumber;
		// ----
		Token = GetToken();
		memcpy(pData.Folder, TokenString, sizeof(pData.Folder));
		sprintf(pData.File, "Custom\\Scenes\\%s\\Scene.dat", TokenString);
		// ----
		this->m_Scene.push_back(pData);
		QuestJobLog.Output("[QuestManager] [%d] Manager added [%d] [%d] [%d] [%d] [%s]",
			pData.Index, pData.ClassID, pData.MapNumber, pData.X, pData.Y, pData.File);
	}
	// ----
	fclose(SMDFile);
	QuestJobLog.Output("[QuestManager] [%s] File is loaded, total: [%d]", File, this->m_Scene.size());
}
// -------------------------------------------------------------------------------

void QuestSystem::ReadSceneList(QuestManager * lpScene)
{
	SMDToken Token;
	SMDFile = fopen(gDirPath.GetNewPath(lpScene->File), "r");
	// ----
	if( !SMDFile )
	{
		QuestJobLog.Output("[QuestScene] %s file not found", gDirPath.GetNewPath(lpScene->File));
		return;
	}
	// ----
	while(true)
	{
		SMDToken Token = GetToken();
		// ----
		if( Token == END || strcmp("end", TokenString) == 0 )
		{
			break;
		}
		// ----
		lpScene->Repeat			= (bool)TokenNumber;
		// ----
		Token = GetToken();
		lpScene->TimeOut		= (DWORD)TokenNumber;
		// ----
		Token = GetToken();
		lpScene->EpisodeCount	= (short)TokenNumber;
		// ----
		Token = GetToken();
		lpScene->Class			= (char)TokenNumber;
		// ----
		Token = GetToken();
		lpScene->MinLevel		= (WORD)TokenNumber;
		// ----
		Token = GetToken();
		lpScene->MaxLevel		= (WORD)TokenNumber;
		// ----
		Token = GetToken();
		lpScene->MinReset		= (WORD)TokenNumber;
		// ----
		Token = GetToken();
		lpScene->MaxReset		= (WORD)TokenNumber;
		// ----
		Token = GetToken();
		memcpy(lpScene->Title, TokenString, sizeof(lpScene->Title));
		// ----
		QuestJobLog.Output("[QuestScene] [%d] Scene added [%d] [%d] [%d] [%d]",
			lpScene->Index, lpScene->Repeat, lpScene->TimeOut, 
			lpScene->EpisodeCount, lpScene->Class);
	}
	// ----
	fclose(SMDFile);
	QuestJobLog.Output("[QuestScene] [%s] File is loaded", gDirPath.GetNewPath(lpScene->File));
}
// -------------------------------------------------------------------------------

void QuestSystem::ReadEpisode(QuestEpisode * lpEpisode, LPSTR File)
{
	SMDToken Token;
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		QuestJobLog.Output("[QuestEpisode] %s file not found", File);
		return;
	}
	// ----
	while(true)
	{
		SMDToken Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		int Type = (int)TokenNumber;
		// ----
		if( Type == 0 )
		{
			while(true)
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				lpEpisode->Type = (BYTE)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->PreviousEpisode = (int)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->Class = (char)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->MinLevel = (WORD)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->MaxLevel = (WORD)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->MinReset = (WORD)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->MaxReset = (WORD)TokenNumber;
				// ----
				Token = GetToken();
				memcpy(lpEpisode->Title, TokenString, sizeof(lpEpisode->Title));
			}
		}
		else if( Type == 1 )
		{
			if( lpEpisode->Type == QUEST_TYPE_HUNT )
			{
				while(true)
				{
					Token = GetToken();
					// ----
					if( strcmp("end", TokenString) == 0 )
					{
						break;
					}
					// ----
					QuestType_Hunt pHunt;
					// ----
					pHunt.Count = (BYTE)TokenNumber;
					// ----
					Token = GetToken();
					pHunt.MonsterClass = (WORD)TokenNumber;
					// ----
					Token = GetToken();
					pHunt.MonsterMap = (short)TokenNumber;
					// ----
					lpEpisode->Hunt.push_back(pHunt);
				}
			}
			else if( lpEpisode->Type == QUEST_TYPE_SEARCH )
			{
				while(true)
				{
					Token = GetToken();
					// ----
					if( strcmp("end", TokenString) == 0 )
					{
						break;
					}
					// ----
					QuestType_Search pSearch;
					// ----
					pSearch.Count = (BYTE)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.ItemType = (WORD)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.ItemIndex = (WORD)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.MinLevel = (BYTE)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.MaxLevel = (BYTE)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.MinOption = (BYTE)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.MaxOption = (BYTE)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.IsLuck = (BYTE)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.IsSkill = (BYTE)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.IsExcellent = (bool)TokenNumber;
					// ----
					Token = GetToken();
					pSearch.IsAncient = (bool)TokenNumber;
					// ----
					lpEpisode->Search.push_back(pSearch);
				}
			}
			else if( lpEpisode->Type == QUEST_TYPE_TALK )
			{
				while(true)
				{
					Token = GetToken();
					// ----
					if( strcmp("end", TokenString) == 0 )
					{
						break;
					}
					// ----
					QuestType_Talk pTalk;
					// ----
					pTalk.Index = (short)TokenNumber;
					// ----
					Token = GetToken();
					pTalk.NPC = (short)TokenNumber;
					// ----
					Token = GetToken();
					pTalk.MapNumber = (short)TokenNumber;
					// ----
					Token = GetToken();
					pTalk.X = (short)TokenNumber;
					// ----
					Token = GetToken();
					pTalk.Y = (short)TokenNumber;
					// ----
					Token = GetToken();
					memcpy(pTalk.Answer, TokenString, sizeof(pTalk.Answer));
					// ----
					lpEpisode->Talk.push_back(pTalk);
				}
			}
		}
		else if( Type == 2 )
		{
			while(true)
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				lpEpisode->Reward.Money = (__int64)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->Reward.Experience = (BYTE)TokenNumber;
			}
		}
		else if( Type == 3 )
		{
			while(true)
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				QuestReward_Item pItem;
				// ----
				pItem.Count = (BYTE)TokenNumber;
				// ----
				Token = GetToken();
				pItem.ItemType = (WORD)TokenNumber;
				// ----
				Token = GetToken();
				pItem.ItemIndex = (WORD)TokenNumber;
				// ----
				Token = GetToken();
				pItem.Level = (BYTE)TokenNumber;
				// ----
				Token = GetToken();
				pItem.Option = (BYTE)TokenNumber;
				// ----
				Token = GetToken();
				pItem.Skill = (BYTE)TokenNumber;
				// ----
				Token = GetToken();
				pItem.Luck = (BYTE)TokenNumber;
				// ----
				Token = GetToken();
				pItem.Excellent = (BYTE)TokenNumber;
				// ----
				Token = GetToken();
				pItem.Ancient = (BYTE)TokenNumber;
				// ----
				lpEpisode->Reward.Item.push_back(pItem);
			}
		}
		else if( Type == 4 )
		{
			while(true)
			{
				Token = GetToken();
				// ----
				if( strcmp("end", TokenString) == 0 )
				{
					break;
				}
				// ----
				lpEpisode->Reward.LevelPoint = (int)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->Reward.WCoinC = (int)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->Reward.WCoinP = (int)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->Reward.WCoinG = (int)TokenNumber;
				// ----
				Token = GetToken();
				lpEpisode->Reward.Contribute = (int)TokenNumber;
			}
		}
	}
	// ----
	fclose(SMDFile);
	QuestJobLog.Output("[QuestEpisode] [%s] File is loaded", File);
}
// -------------------------------------------------------------------------------

short QuestSystem::GetSceneIndex(LPOBJ lpNpc)
{
	for( int i = 0; i < this->m_Scene.size(); i++ )
	{
		QuestManager & pData = this->m_Scene[i];
		// ----
		if(		lpNpc->Class == pData.ClassID 
			&&	lpNpc->MapNumber == pData.MapNumber
			&&	lpNpc->X == pData.X
			&&	lpNpc->Y == pData.Y )
		{
			return i;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

short QuestSystem::GetQuestIndex(LPOBJ lpUser, short SceneIndex, short EpisodeIndex)
{
	for( int i = 0; i < lpUser->m_QuestCount; i++ )
	{
		// ----
		if(		lpUser->m_QuestData[i].SceneIndex == SceneIndex
			&&	lpUser->m_QuestData[i].EpisodeIndex == EpisodeIndex )
		{
			return i;
		}
	}
	// ----
	return -1;
}
// -------------------------------------------------------------------------------

bool QuestSystem::IsManager(LPOBJ lpNpc)
{
	for( int i = 0; i < this->m_Scene.size(); i++ )
	{
		QuestManager & pData = this->m_Scene[i];
		// ----
		if(		lpNpc->Class == pData.ClassID 
			&&	lpNpc->MapNumber == pData.MapNumber
			&&	lpNpc->X == pData.X
			&&	lpNpc->Y == pData.Y )
		{
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool QuestSystem::IsValidScene(LPOBJ lpUser, short SceneIndex)
{
	//this->DB_ReqSceneStatus(lpUser, SceneIndex);
	QuestManager & pData = this->m_Scene[SceneIndex];
	// ----
	if( pData.Class != -1 && lpUser->Class != pData.Class )
	{
		return false;
	}
	// ----
	if( lpUser->Level < pData.MinLevel || lpUser->Level > pData.MaxLevel )
	{
		return false;
	}
	// ----
	if( lpUser->Reset < pData.MinReset || lpUser->Reset > pData.MaxReset )
	{
		return false;
	}
	// ----
	if( pData.Repeat != false )
	{
		if( lpUser->m_QuestSceneStatus == true )
		{
			lpUser->m_QuestSceneStatus = -1;
			return false;
		}
	}
	// ----
	// Check timeout
	// ----
	return true;
}
// -------------------------------------------------------------------------------

bool QuestSystem::IsValidEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex)
{
	//this->DB_ReqEpisodeStatus(lpUser, SceneIndex, EpisodeIndex);
	QuestEpisode & pData = this->m_Scene[SceneIndex].Episode[EpisodeIndex];
	// ----
	if( pData.Class != -1 && lpUser->Class != pData.Class )
	{
		return false;
	}
	// ----
	if( lpUser->Level < pData.MinLevel || lpUser->Level > pData.MaxLevel )
	{
		return false;
	}
	// ----
	if( lpUser->Reset < pData.MinReset || lpUser->Reset > pData.MaxReset )
	{
		return false;
	}
	// ----
	if( this->GetQuestIndex(lpUser, SceneIndex, EpisodeIndex) != -1 )
	{
			return false;
	}
	// ----
	if( pData.PreviousEpisode != -1 )
	{
		if( lpUser->m_QuestPrevEpisode != true )
		{
			lpUser->m_QuestPrevEpisode = -1;
			return false;
		}
	}
	// ---
	return true;
}
// -------------------------------------------------------------------------------

bool QuestSystem::IsFinishedEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex)
{
	BYTE QuestIndex = this->GetQuestIndex(lpUser, SceneIndex, EpisodeIndex);
	// ----
	if( QuestIndex == -1 )
	{
		return false;
	}
	// ----
	return lpUser->m_QuestData[QuestIndex].Reward;
}
// -------------------------------------------------------------------------------

bool QuestSystem::IsSplitItem(WORD ItemID)
{
	switch(ItemID)
	{
	case ITEMGET(4, 7):
	case ITEMGET(4, 15):
	case ITEMGET(13, 32):
	case ITEMGET(13, 33):
	case ITEMGET(13, 34):
	case ITEMGET(14, 0):
	case ITEMGET(14, 1):
	case ITEMGET(14, 2):
	case ITEMGET(14, 3):
	case ITEMGET(14, 4):
	case ITEMGET(14, 5):
	case ITEMGET(14, 6):
	case ITEMGET(14, 7):
	case ITEMGET(14, 8):
	case ITEMGET(14, 10):
	case ITEMGET(14, 15):
	case ITEMGET(14, 21):
	case ITEMGET(14, 29):
	case ITEMGET(14, 35):
	case ITEMGET(14, 36):
	case ITEMGET(14, 37):
	case ITEMGET(14, 38):
	case ITEMGET(14, 39):
	case ITEMGET(14, 40):
	case ITEMGET(14, 46):
	case ITEMGET(14, 47):
	case ITEMGET(14, 48):
	case ITEMGET(14, 49):
	case ITEMGET(14, 50):
	case ITEMGET(14, 70):
	case ITEMGET(14, 71):
	case ITEMGET(14, 85):
	case ITEMGET(14, 86):
	case ITEMGET(14, 87):
	case ITEMGET(14, 88):
	case ITEMGET(14, 89):
	case ITEMGET(14, 90):
	case ITEMGET(14, 94):
		{
			return true;
		}
		break;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

void QuestSystem::AddUser(LPOBJ lpUser)
{
	for( int i = 0; i < 40; i++ )
	{
		lpUser->m_QuestData[i].EpisodeIndex	= -1;
		lpUser->m_QuestData[i].SceneIndex	= -1;
		lpUser->m_QuestData[i].TalkIndex	= -1;
		lpUser->m_QuestData[i].Reward		= false;
		// ----
		for( int j = 0; j < QUEST_MAX_HUNT; j++ )
		{
			lpUser->m_QuestData[i].HuntData[j] = -1;
		}
	}
	// ----
	//DB request
}
// -------------------------------------------------------------------------------

bool QuestSystem::Dialog(LPOBJ lpUser, LPOBJ lpNpc)
{
	this->RunItem(lpUser, lpNpc, (BYTE)-1);
	this->RunTalk(lpUser, lpNpc);
	// ----
	if( !this->IsManager(lpNpc) )
	{
		return false;
	}
	// ----
	short SceneIndex = this->GetSceneIndex(lpNpc);
	// ----
	if( SceneIndex == -1 )
	{
		return false;
	}
	// ----
	QuestManager & lpScene = this->m_Scene[SceneIndex];
	ChatSend(lpNpc, lpScene.Title);
	// ----
	for( int EpisodeIndex = 0; EpisodeIndex < lpScene.Episode.size(); EpisodeIndex++ )
	{
		QuestEpisode & lpEpisode = lpScene.Episode[EpisodeIndex];
		// ----
		if( this->IsFinishedEpisode(lpUser, SceneIndex, EpisodeIndex) )
		{
			ChatSend(lpNpc, "Nice work!");
			this->MakeReward(lpUser, SceneIndex, EpisodeIndex);
			this->DelEpisode(lpUser, SceneIndex, EpisodeIndex);
		}
		else if( this->GetQuestIndex(lpUser, SceneIndex, EpisodeIndex) != -1 )
		{
			this->MakeNotifyQuest(lpUser, SceneIndex, EpisodeIndex);
		}
		else if( this->IsValidScene(lpUser, SceneIndex) && this->IsValidEpisode(lpUser, SceneIndex, EpisodeIndex) )
		{
			this->AddEpisode(lpUser, SceneIndex, EpisodeIndex);
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

void QuestSystem::AddEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex)
{
	lpUser->m_QuestData[lpUser->m_QuestCount].SceneIndex	= SceneIndex;
	lpUser->m_QuestData[lpUser->m_QuestCount].EpisodeIndex	= EpisodeIndex;
	lpUser->m_QuestData[lpUser->m_QuestCount].Reward		= false;
	lpUser->m_QuestData[lpUser->m_QuestCount].TalkIndex		= 0;
	// ---
	for( int i = 0; i < QUEST_MAX_HUNT; i++ )
	{
		lpUser->m_QuestData[lpUser->m_QuestCount].HuntData[i] = 0;
	}
	// ----
	lpUser->m_QuestCount++;
	// ----
	this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Obtained");
	this->MakeNotifyQuest(lpUser, SceneIndex, EpisodeIndex);
	QuestLog.Output("[%s] [%s] (Episode) Added - Scene: %d, Episode: %d",
		lpUser->AccountID, lpUser->Name, SceneIndex, EpisodeIndex);
}
// -------------------------------------------------------------------------------

void QuestSystem::DelEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex)
{
	BYTE QuestIndex = this->GetQuestIndex(lpUser, SceneIndex, EpisodeIndex);
	// ----
	if( QuestIndex == -1 )
	{
		return;
	}
	// ----
	this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Deleted");
	// ----
	lpUser->m_QuestData[QuestIndex].SceneIndex		= -1;
	lpUser->m_QuestData[QuestIndex].EpisodeIndex	= -1;
	lpUser->m_QuestData[QuestIndex].Reward			= false;
	lpUser->m_QuestData[QuestIndex].TalkIndex		= -1;
	// ---
	for( int i = 0; i < QUEST_MAX_HUNT; i++ )
	{
		lpUser->m_QuestData[QuestIndex].HuntData[i] = -1;
	}
	// ----
	lpUser->m_QuestCount--;
}
// -------------------------------------------------------------------------------

void QuestSystem::EndEpisode(LPOBJ lpUser, short SceneIndex, short EpisodeIndex)
{
	BYTE QuestIndex = this->GetQuestIndex(lpUser, SceneIndex, EpisodeIndex);
	// ----
	if( QuestIndex == -1 )
	{
		return;
	}
	// ----
	lpUser->m_QuestData[QuestIndex].Reward = true;
	// ----
	//DB save
	// ----
}
// -------------------------------------------------------------------------------

void QuestSystem::MakeReward(LPOBJ lpUser, short SceneIndex, short EpisodeIndex)
{
	QuestLog.Output("[%s] [%s] (Reward) - Scene: %d, Episode: %d",
		lpUser->AccountID, lpUser->Name, SceneIndex, EpisodeIndex);
	QuestEpisode & lpEpisode = this->m_Scene[SceneIndex].Episode[EpisodeIndex];
	// ----
	if( lpEpisode.Reward.Money > 0 )
	{
		lpUser->Money += lpEpisode.Reward.Money;
		// ----
		if( lpUser->Money >= MAX_ZEN )
		{
			lpUser->Money = MAX_ZEN;
		}
		// ----
		GCMoneySend(lpUser->m_Index, lpUser->Money);
		QuestLog.Output("Money: %d", lpEpisode.Reward.Money);
	}
	// ----
	if( lpEpisode.Reward.Experience > 0 )
	{
		__int64 Experience	= lpUser->NextExp - lpUser->Experience;
		__int64 RewardExp	= Experience / 100 * lpEpisode.Reward.Experience;
		lpUser->Experience	+= RewardExp;
		// ----
		if( gObjLevelUp(lpUser, RewardExp, 0, 0) == true )
		{
			GCKillPlayerExpSend(lpUser->m_Index, (WORD)-1, RewardExp, 0, 0);
		}
		// ----
		QuestLog.Output("Experience: %I64u", RewardExp);
	}
	// ----
	if( lpEpisode.Reward.LevelPoint > 0 )
	{
		lpUser->LevelUpPoint += lpEpisode.Reward.LevelPoint;
		UpdateCharInfo(lpUser->m_Index);
		QuestLog.Output("LevelPoint: %I64u", lpEpisode.Reward.LevelPoint);
	}
	// ----
	lpUser->GameShop.WCoinC			+= lpEpisode.Reward.WCoinC;
	lpUser->GameShop.WCoinP			+= lpEpisode.Reward.WCoinP;
	lpUser->GameShop.GoblinPoint	+= lpEpisode.Reward.WCoinG;
	lpUser->m_ContributePoint		+= lpEpisode.Reward.Contribute;
	// ----
	QuestLog.Output("WCoinC: %d", lpEpisode.Reward.WCoinC);
	QuestLog.Output("WCoinP: %d", lpEpisode.Reward.WCoinP);
	QuestLog.Output("WCoinG: %d", lpEpisode.Reward.WCoinG);
	QuestLog.Output("Contribute: %d", lpEpisode.Reward.Contribute);
	// ----
	g_ShopPointEx.SendNotify(lpUser->m_Index,
		lpEpisode.Reward.WCoinC,
		lpEpisode.Reward.WCoinP,
		lpEpisode.Reward.WCoinG);
	// ----
	for( int i = 0; i < lpEpisode.Reward.Item.size(); i++ )
	{
		QuestReward_Item & lpItem = lpEpisode.Reward.Item[i];
		// ----
		for( int j = 0; j < lpItem.Count; j++ )
		{
			ItemSerialCreateSend(lpUser->m_Index, lpUser->MapNumber, lpUser->X, lpUser->Y, 
				ITEMGET(lpItem.ItemType, lpItem.ItemIndex), lpItem.Level, 0, lpItem.Option, 
				lpItem.Luck, lpItem.Skill, lpUser->m_Index, lpItem.Excellent, lpItem.Ancient);
			QuestLog.Output("%d. Item: %s (%d/%d) +%d (Option: +%d) (Luck: %d) (Skill: %d) (Excellent: %d) (Ancient: %d)", 
				j + 1, ItemAttribute[ITEMGET(lpItem.ItemType, lpItem.ItemIndex)].Name, lpItem.ItemType, lpItem.ItemIndex, 
				lpItem.Level, lpItem.Option * 4, lpItem.Luck, lpItem.Skill, lpItem.Excellent, lpItem.Ancient);
		}
	}
	// ----
	PMSG_SERVERCMD ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = 0;
	ServerCmd.X = lpUser->X;
	ServerCmd.Y = lpUser->Y;
	MsgSendV2(lpUser, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	DataSend(lpUser->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	// ----
	this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Rewarded");
	// ----
	//DB save
	// ----
}
// -------------------------------------------------------------------------------

void QuestSystem::MakeNotify(LPOBJ lpUser, short SceneIndex, short EpisodeIndex, char * Text, ...)
{
	char Temp[512] = { 0 };
	char Notify[1024] = { 0 };
	// ----
	va_list ArgList;
	va_start(ArgList, Text);
	vsprintf(Temp, Text, ArgList);
	va_end(ArgList);
	// ----
	sprintf(Notify, "[%s] (%d) %s - %s", 
		this->m_Scene[SceneIndex].Title, EpisodeIndex + 1, 
		this->m_Scene[SceneIndex].Episode[EpisodeIndex].Title, Temp);
	// ----
	CHAT_WHISPER_EX pMessage;
	memcpy(pMessage.Name, "Quest", 10);
	memcpy(pMessage.Message, Notify, MAX_CHAT_LEN);
	pMessage.Type = 8;
	pMessage.Head.set((LPBYTE)&pMessage, 2, sizeof(CHAT_WHISPER_EX));
	DataSend(lpUser->m_Index, (LPBYTE)&pMessage, pMessage.Head.size);
}
// -------------------------------------------------------------------------------

void QuestSystem::MakeNotifyQuest(LPOBJ lpUser, short SceneIndex, short EpisodeIndex)
{
	QuestManager & lpScene		= this->m_Scene[SceneIndex];
	QuestEpisode & lpEpisode	= lpScene.Episode[EpisodeIndex];
	short QuestIndex			= this->GetQuestIndex(lpUser, SceneIndex, EpisodeIndex);
	// ----
	if( lpEpisode.Type == QUEST_TYPE_HUNT )
	{
		this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "");
		// ----
		for( int i = 0; i < lpEpisode.Hunt.size(); i++ )
		{
			LPMONSTER_ATTRIBUTE lpMonster = gMAttr.GetAttr(lpEpisode.Hunt[i].MonsterClass);
			// ----
			if( lpMonster == NULL )
			{
				continue;
			}
			if( lpEpisode.Hunt[i].MonsterMap != -1 && g_szMapName[lpEpisode.Hunt[i].MonsterMap] )
			{
				char HuntIndex[10] = { 0 };
				sprintf_s(HuntIndex, "[%d/%d]", i + 1, lpEpisode.Hunt.size());
				this->MakeNotice(lpUser, HuntIndex, "Kill %s's (%s) %d/%d",
					lpMonster->m_Name, g_szMapName[lpEpisode.Hunt[i].MonsterMap], 
					lpUser->m_QuestData[QuestIndex].HuntData[i], lpEpisode.Hunt[i].Count);
			}
			else
			{
				char HuntIndex[10] = { 0 };
				sprintf_s(HuntIndex, "[%d/%d]", i + 1, lpEpisode.Hunt.size());
				this->MakeNotice(lpUser, HuntIndex, "Kill %s's %d/%d",
					lpMonster->m_Name, lpUser->m_QuestData[QuestIndex].HuntData[i], 
					lpEpisode.Hunt[i].Count);
			}
		}
	}
	else if( lpEpisode.Type == QUEST_TYPE_SEARCH )
	{
		this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "");
		// ----
		for( int ItemIndex = 0; ItemIndex < lpEpisode.Search.size(); ItemIndex++ )
		{
			this->MakeNoticeItem(lpUser, lpEpisode, ItemIndex);
		}
	}
	else if( lpEpisode.Type == QUEST_TYPE_TALK )
	{
		BYTE QuestIndex = this->GetQuestIndex(lpUser, SceneIndex, EpisodeIndex);
		short TalkIndex	= lpUser->m_QuestData[QuestIndex].TalkIndex;
		LPMONSTER_ATTRIBUTE lpMonster = gMAttr.GetAttr(lpEpisode.Talk[TalkIndex].NPC);
		// ----
		if( lpMonster == NULL )
		{
			return;
		}
		// ----
		if( lpEpisode.Talk[TalkIndex].MapNumber != -1 && g_szMapName[lpEpisode.Talk[TalkIndex].MapNumber] )
		{
			this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Talk with %s (%s)", 
				lpMonster->m_Name, g_szMapName[lpEpisode.Talk[TalkIndex].MapNumber]);
		}
		else
		{
			this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Talk with %s", lpMonster->m_Name);
		}
	}
}
// -------------------------------------------------------------------------------

void QuestSystem::MakeNotice(LPOBJ lpUser, char * Sender, char * Message, ...)
{
	char szTemp[1024];
	va_list pArguments;
	va_start(pArguments, Message);
	vsprintf(szTemp, Message, pArguments);
	va_end(pArguments);
	CHAT_WHISPER_EX pMessage;
	memcpy(pMessage.Name, Sender, 10);
	memcpy(pMessage.Message, szTemp, MAX_CHAT_LEN);
	pMessage.Type = 8;
	pMessage.Head.set((LPBYTE)&pMessage, 2, sizeof(CHAT_WHISPER_EX));
	DataSend(lpUser->m_Index, (LPBYTE)&pMessage, pMessage.Head.size);
}
// -------------------------------------------------------------------------------

void QuestSystem::MakeNoticeItem(LPOBJ lpUser, QuestEpisode lpEpisode, short ItemIndex)
{
	char Prefix[10] = { 0 };
	char ItemLog[MAX_CHAT_LEN] = { 0 };
	char ExtLog[30] = { 0 };
	QuestType_Search & lpItem = lpEpisode.Search[ItemIndex];
	// ----
	sprintf(Prefix, "[%d/%d]", ItemIndex + 1,  lpEpisode.Search.size());
	sprintf(ItemLog, "(%d/%d) ", this->GetItemCount(lpUser, lpEpisode, ItemIndex), lpItem.Count);
	// ----
	if( lpItem.IsExcellent )
	{
		strcat(ItemLog, "Excellent ");
	}
	// ----
	if( lpItem.IsAncient )
	{
		strcat(ItemLog, "Ancient ");
	}
	// ----
	strcat(ItemLog, ItemAttribute[ITEMGET(lpItem.ItemType, lpItem.ItemIndex)].Name);
	// ----
	if( lpItem.MinLevel == lpItem.MaxLevel )
	{
		sprintf(ExtLog, " +%d", lpItem.MinLevel);
		strcat(ItemLog, ExtLog);
	}
	else if( lpItem.MinLevel != 0 && lpItem.MaxLevel != 15 )
	{
		sprintf(ExtLog, " +%d~%d", lpItem.MinLevel, lpItem.MaxLevel);
		strcat(ItemLog, ExtLog);
	}
	// ----
	if( lpItem.MinOption == lpItem.MaxOption )
	{
		sprintf(ExtLog, " [+%d]", lpItem.MinOption * 4);
		strcat(ItemLog, ExtLog);
	}
	else if( lpItem.MinOption != 0 && lpItem.MaxOption != 7 )
	{
		sprintf(ExtLog, " [+%d~%d]", lpItem.MinOption * 4, lpItem.MaxOption * 4);
		strcat(ItemLog, ExtLog);
	}
	// ----
	if( lpItem.IsLuck == 1 )
	{
		strcat(ItemLog, " (Luck)");
	}
	else if( lpItem.IsLuck == 0 )
	{
		strcat(ItemLog, " (NoLuck)");
	}
	// ----
	if( lpItem.IsSkill == 1 )
	{
		strcat(ItemLog, " (Skill)");
	}
	else if( lpItem.IsSkill == 0 )
	{
		strcat(ItemLog, " (NoSkill)");
	}
	// ----
	this->MakeNotice(lpUser, Prefix, ItemLog); 
}
// -------------------------------------------------------------------------------

void QuestSystem::MakeNoticeItem(LPOBJ lpUser, BYTE ItemPos)
{
	for( int i = 0; i < lpUser->m_QuestCount; i++ )
	{
		QuestManager & lpScene		= this->m_Scene[lpUser->m_QuestData[i].SceneIndex];
		QuestEpisode & lpEpisode	= lpScene.Episode[lpUser->m_QuestData[i].EpisodeIndex];
		// ----
		if( lpEpisode.Type != QUEST_TYPE_SEARCH )
		{
			continue;
		}
		// ----
		for( int ItemIndex = 0; ItemIndex < lpEpisode.Search.size(); ItemIndex++ )
		{
			QuestType_Search & lpItem = lpEpisode.Search[ItemIndex];
			// ----
			if( !lpUser->pInventory[i].IsItem() )
			{
				continue;
			}
			// ----
			if( lpUser->pInventory[ItemPos].m_Type != ITEMGET(lpItem.ItemType, lpItem.ItemIndex) )
			{
				continue;
			}
			// ----
			if(		lpUser->pInventory[ItemPos].m_Level < lpItem.MinLevel 
				||	lpUser->pInventory[ItemPos].m_Level > lpItem.MaxLevel )
			{
				continue;
			}
			// ----
			if(		lpUser->pInventory[ItemPos].m_Option3 < lpItem.MinOption
				||	lpUser->pInventory[i].m_Option3 > lpItem.MaxOption )
			{
				continue;
			}
			// ----
			if( lpItem.IsLuck != 2 )
			{
				if( lpUser->pInventory[ItemPos].m_Option2 != lpItem.IsLuck )
				{
					continue;
				}
			}
			// ----
			if( lpItem.IsSkill != 2 )
			{
				if( lpUser->pInventory[ItemPos].m_Option1 != lpItem.IsSkill )
				{
					continue;
				}
			}
			// ----
			if( lpItem.IsExcellent )
			{
				if( !lpUser->pInventory[ItemPos].m_NewOption )
				{
					continue;
				}
			}
			// ----
			if( lpItem.IsAncient )
			{
				if( !lpUser->pInventory[ItemPos].m_SetOption )
				{
					continue;
				}
			}
			// ----
			this->MakeNotifyQuest(lpUser, lpUser->m_QuestData[i].SceneIndex, lpUser->m_QuestData[i].EpisodeIndex);
		}
	}
}
// -------------------------------------------------------------------------------

short QuestSystem::GetItemCount(LPOBJ lpUser, QuestEpisode lpEpisode, short ItemIndex)
{
	QuestType_Search & lpItem	= lpEpisode.Search[ItemIndex];
	short ItemCount				= 0;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		if( !lpUser->pInventory[i].IsItem() )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Type != ITEMGET(lpItem.ItemType, lpItem.ItemIndex) )
		{
			continue;
		}
		// ----
		if(		lpUser->pInventory[i].m_Level < lpItem.MinLevel 
			||	lpUser->pInventory[i].m_Level > lpItem.MaxLevel )
		{
			continue;
		}
		// ----
		if(		lpUser->pInventory[i].m_Option3 < lpItem.MinOption
			||	lpUser->pInventory[i].m_Option3 > lpItem.MaxOption )
		{
			continue;
		}
		// ----
		if( lpItem.IsLuck != 2 )
		{
			if( lpUser->pInventory[i].m_Option2 != lpItem.IsLuck )
			{
				continue;
			}
		}
		// ----
		if( lpItem.IsSkill != 2 )
		{
			if( lpUser->pInventory[i].m_Option1 != lpItem.IsSkill )
			{
				continue;
			}
		}
		// ----
		if( lpItem.IsExcellent )
		{
			if( !lpUser->pInventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( lpItem.IsAncient )
		{
			if( !lpUser->pInventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->IsSplitItem(ITEMGET(lpItem.ItemType, lpItem.ItemIndex)) )
		{
			ItemCount += (short)lpUser->pInventory[i].m_Durability;
		}
		else
		{
			ItemCount++;
		}
	}
	// ----
	return ItemCount;
}
// -------------------------------------------------------------------------------

bool QuestSystem::SearchItem(LPOBJ lpUser, short SceneIndex, short EpisodeIndex, short ItemIndex)
{
	QuestManager & lpScene		= this->m_Scene[SceneIndex];
	QuestEpisode & lpEpisode	= lpScene.Episode[EpisodeIndex];
	QuestType_Search & lpItem	= lpEpisode.Search[ItemIndex];
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		if( !lpUser->pInventory[i].IsItem() )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Type != ITEMGET(lpItem.ItemType, lpItem.ItemIndex) )
		{
			continue;
		}
		// ----
		if(		lpUser->pInventory[i].m_Level < lpItem.MinLevel 
			||	lpUser->pInventory[i].m_Level > lpItem.MaxLevel )
		{
			continue;
		}
		// ----
		if(		lpUser->pInventory[i].m_Option3 < lpItem.MinOption
			||	lpUser->pInventory[i].m_Option3 > lpItem.MaxOption )
		{
			continue;
		}
		// ----
		if( lpItem.IsLuck != 2 )
		{
			if( lpUser->pInventory[i].m_Option2 != lpItem.IsLuck )
			{
				continue;
			}
		}
		// ----
		if( lpItem.IsSkill != 2 )
		{
			if( lpUser->pInventory[i].m_Option1 != lpItem.IsSkill )
			{
				continue;
			}
		}
		// ----
		if( lpItem.IsExcellent )
		{
			if( !lpUser->pInventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( lpItem.IsAncient )
		{
			if( !lpUser->pInventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( lpItem.Count > 1 )
		{
			if( lpItem.Count > this->GetItemCount(lpUser, lpEpisode, ItemIndex) )
			{
				continue;
			}
		}
		// ----
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool QuestSystem::DeleteItem(LPOBJ lpUser, short SceneIndex, short EpisodeIndex, short ItemIndex)
{
	QuestManager & lpScene		= this->m_Scene[SceneIndex];
	QuestEpisode & lpEpisode	= lpScene.Episode[EpisodeIndex];
	QuestType_Search & lpItem	= lpEpisode.Search[ItemIndex];
	int Deleted					= 0;
	int Left					= 0;
	// ----
	for( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		Left = lpItem.Count - Deleted;
		// ----
		if( Deleted >= lpItem.Count )
		{
			break;
		}
		// ----
		if( !lpUser->pInventory[i].IsItem() )
		{
			continue;
		}
		// ----
		if( lpUser->pInventory[i].m_Type != ITEMGET(lpItem.ItemType, lpItem.ItemIndex) )
		{
			continue;
		}
		// ----
		if(		lpUser->pInventory[i].m_Level < lpItem.MinLevel 
			||	lpUser->pInventory[i].m_Level > lpItem.MaxLevel )
		{
			continue;
		}
		// ----
		if(		lpUser->pInventory[i].m_Option3 < lpItem.MinOption
			||	lpUser->pInventory[i].m_Option3 > lpItem.MaxOption )
		{
			continue;
		}
		// ----
		if( lpItem.IsLuck != 2 )
		{
			if( lpUser->pInventory[i].m_Option2 != lpItem.IsLuck )
			{
				continue;
			}
		}
		// ----
		if( lpItem.IsSkill != 2 )
		{
			if( lpUser->pInventory[i].m_Option1 != lpItem.IsSkill )
			{
				continue;
			}
		}
		// ----
		if( lpItem.IsExcellent )
		{
			if( !lpUser->pInventory[i].m_NewOption )
			{
				continue;
			}
		}
		// ----
		if( lpItem.IsAncient )
		{
			if( !lpUser->pInventory[i].m_SetOption )
			{
				continue;
			}
		}
		// ----
		if( this->IsSplitItem(lpUser->pInventory[i].m_Type) )
		{
			if( Left >= lpUser->pInventory[i].m_Durability )
			{
				Deleted += lpUser->pInventory[i].m_Durability;
				gObjInventoryDeleteItem(lpUser->m_Index, i);
				GCInventoryItemDeleteSend(lpUser->m_Index, i, 1);
			}
			else
			{
				for( int j = 0; j < Left; j++ )
				{
					lpUser->pInventory[i].m_Durability--;
					Deleted++;
				}
				// ----
				GCItemDurSend(lpUser->m_Index, i, lpUser->pInventory[i].m_Durability, 1);
			}
		}
		else
		{
			Deleted++;
			gObjInventoryDeleteItem(lpUser->m_Index, i);
			GCInventoryItemDeleteSend(lpUser->m_Index, i, 1);
		}
	}
	// ----
	if( Deleted >= lpItem.Count )
	{
		return true;
	}
	// -----
	return false;
}
// -------------------------------------------------------------------------------

void QuestSystem::RunHunt(LPOBJ lpUser, LPOBJ lpMonster)
{
	for( int i = 0; i < lpUser->m_QuestCount; i++ )
	{
		short SceneIndex		= lpUser->m_QuestData[i].SceneIndex;
		short EpisodeIndex		= lpUser->m_QuestData[i].EpisodeIndex;
		short Completed			= 0;
		QuestManager & pScene	= this->m_Scene[SceneIndex];
		QuestEpisode & pEpisode	= this->m_Scene[SceneIndex].Episode[EpisodeIndex];
		// ----
		if( pEpisode.Type != QUEST_TYPE_HUNT )
		{
			continue;
		}
		// ----
		for( int j = 0; j < pEpisode.Hunt.size(); j++ )
		{
			int KillCount = lpUser->m_QuestData[i].HuntData[j];
			// ----
			if( KillCount >= pEpisode.Hunt[j].Count )
			{
				Completed++;
			}
			// ----
			if( pEpisode.Hunt[j].MonsterClass != lpMonster->Class )
			{
				continue;
			}
			// ----
			if( pEpisode.Hunt[j].MonsterMap != -1 )
			{
				if( pEpisode.Hunt[j].MonsterMap != lpMonster->MapNumber )
				{
					continue;
				}
			}
			// ----
			if( KillCount >= pEpisode.Hunt[j].Count )
			{
				continue;
			}
			// ----
			lpUser->m_QuestData[i].HuntData[j]++;
			KillCount++;
			// ----
			if( KillCount >= pEpisode.Hunt[j].Count )
			{
				Completed++;
				this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "");
				char HuntIndex[10] = { 0 };
				sprintf_s(HuntIndex, "[%d/%d]", j + 1, pEpisode.Hunt.size());
				this->MakeNotice(lpUser, HuntIndex, "%s's %d/%d - Complete",
					lpMonster->Name, lpUser->m_QuestData[i].HuntData[j], pEpisode.Hunt[i].Count);
				QuestLog.Output("[%s] [%s] (Hunt) Complete %d/%d - Scene: %d, Episode: %d, HuntIndex: %d",
					lpUser->AccountID, lpUser->Name, SceneIndex, EpisodeIndex, j);
			}
			else
			{
				this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "");
				char HuntIndex[10] = { 0 };
				sprintf_s(HuntIndex, "[%d/%d]", j + 1, pEpisode.Hunt.size());
				this->MakeNotice(lpUser, HuntIndex, "%s's %d/%d - Update",
					lpMonster->Name, lpUser->m_QuestData[i].HuntData[j], pEpisode.Hunt[i].Count);
				QuestLog.Output("[%s] [%s] (Hunt) Update %d/%d - Scene: %d, Episode: %d, HuntIndex: %d",
					lpUser->AccountID, lpUser->Name, SceneIndex, EpisodeIndex, j);
			}
		}
		// ----
		if( Completed >= pEpisode.Hunt.size() )
		{
			this->EndEpisode(lpUser, SceneIndex, EpisodeIndex);
			this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Finished");
			// ---
			LPMONSTER_ATTRIBUTE lpMonster = gMAttr.GetAttr(pScene.ClassID);
			// ----
			if( lpMonster == NULL )
			{
				return;
			}
			// ----
			this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Come back to %s", lpMonster->m_Name);
			QuestLog.Output("[%s] [%s] (Hunt) Finished - Scene: %d, Episode: %d",
				lpUser->AccountID, lpUser->Name, SceneIndex, EpisodeIndex);
		}
	}
}
// -------------------------------------------------------------------------------

void QuestSystem::RunItem(LPOBJ lpUser, LPOBJ lpNpc, BYTE ItemPos)
{
	for( int i = 0; i < lpUser->m_QuestCount; i++ )
	{
		short SceneIndex		= lpUser->m_QuestData[i].SceneIndex;
		short EpisodeIndex		= lpUser->m_QuestData[i].EpisodeIndex;
		QuestManager & pScene	= this->m_Scene[SceneIndex];
		QuestEpisode & pEpisode	= this->m_Scene[SceneIndex].Episode[EpisodeIndex];
		short Completed			= 0;
		// ----
		if( pEpisode.Type != QUEST_TYPE_SEARCH )
		{
			continue;
		}
		// ----
		if( lpNpc != NULL )
		{
			if( lpNpc->Class != pScene.ClassID )
			{
				continue;
			}
		}
		// ----
		for( int ItemIndex = 0; ItemIndex < pEpisode.Search.size(); ItemIndex++ )
		{
			if( this->SearchItem(lpUser, SceneIndex, EpisodeIndex, ItemIndex) == true )
			{
				Completed++;
			}
		}
		// ----
		if( Completed >= pEpisode.Search.size() )
		{
			if( lpNpc != NULL )
			{
				for( int ItemIndex = 0; ItemIndex < pEpisode.Search.size(); ItemIndex++ )
				{
					if( !this->DeleteItem(lpUser, SceneIndex, EpisodeIndex, ItemIndex) )
					{
						QuestLog.Output("[%d] [%s] [ERROR] (Search) Item delete error - Scene: %d, Episode: %d, ItemIndex: %d",
							lpUser->AccountID, lpUser->Name, SceneIndex, EpisodeIndex, ItemIndex);
						return;
					}
				}
				// ----
				this->EndEpisode(lpUser, SceneIndex, EpisodeIndex);
				this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Finished");
				QuestLog.Output("[%s] [%s] (Search) Finished - Scene: %d, Episode: %d",
					lpUser->AccountID, lpUser->Name, SceneIndex, EpisodeIndex);
			}
			else
			{
				LPMONSTER_ATTRIBUTE lpMonster = gMAttr.GetAttr(pScene.ClassID);
				// ----
				if( lpMonster == NULL )
				{
					return;
				}
				// ----
				this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Come back to %s", lpMonster->m_Name);
			}
		}
		else if( ItemPos != (BYTE)-1 )
		{
			this->MakeNoticeItem(lpUser, ItemPos);			
		}
	}
}
// -------------------------------------------------------------------------------

void QuestSystem::RunTalk(LPOBJ lpUser, LPOBJ lpNpc)
{
	for( int i = 0; i < lpUser->m_QuestCount; i++ )
	{
		short SceneIndex		= lpUser->m_QuestData[i].SceneIndex;
		short EpisodeIndex		= lpUser->m_QuestData[i].EpisodeIndex;
		short TalkIndex			= lpUser->m_QuestData[i].TalkIndex;
		QuestManager & pScene	= this->m_Scene[SceneIndex];
		QuestEpisode & pEpisode	= this->m_Scene[SceneIndex].Episode[EpisodeIndex];
		// ----
		if( pEpisode.Type != QUEST_TYPE_TALK )
		{
			continue;
		}
		// ----
		if( TalkIndex >= pEpisode.Talk.size() )
		{
			continue;
		}
		// ----
		if( pEpisode.Talk[TalkIndex].NPC != -1 )
		{
			if( pEpisode.Talk[TalkIndex].NPC != lpNpc->Class )
			{
				continue;
			}
		}
		// ----
		if( pEpisode.Talk[TalkIndex].MapNumber != -1 )
		{
			if( pEpisode.Talk[TalkIndex].MapNumber != lpNpc->MapNumber )
			{
				continue;
			}
		}
		// ----
		if( pEpisode.Talk[TalkIndex].X != -1 )
		{
			if( pEpisode.Talk[TalkIndex].X != lpNpc->X )
			{
				continue;
			}
		}
		// ----
		if( pEpisode.Talk[TalkIndex].Y != -1 )
		{
			if( pEpisode.Talk[TalkIndex].Y != lpNpc->Y )
			{
				continue;
			}
		}
		// ----
		lpUser->m_QuestData[i].TalkIndex++;
		// ----
		if( lpUser->m_QuestData[i].TalkIndex >= pEpisode.Talk.size() )
		{
			this->EndEpisode(lpUser, SceneIndex, EpisodeIndex);
			ChatSend(lpNpc, pEpisode.Talk[TalkIndex].Answer);
			this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Finished");
			QuestLog.Output("[%s] [%s] (Talk) Finished - Scene: %d, Episode: %d",
				lpUser->AccountID, lpUser->Name, SceneIndex, EpisodeIndex);			
			// ----
			LPMONSTER_ATTRIBUTE lpMonster = gMAttr.GetAttr(pScene.ClassID);
			// ----
			if( lpMonster == NULL )
			{
				return;
			}
			// ----
			this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "Come back to %s", lpMonster->m_Name);
		}
		else
		{
			BYTE QuestIndex = this->GetQuestIndex(lpUser, SceneIndex, EpisodeIndex);
			short TalkIndex	= lpUser->m_QuestData[QuestIndex].TalkIndex;
			ChatSend(lpNpc, pEpisode.Talk[TalkIndex].Answer);
			this->MakeNotify(lpUser, SceneIndex, EpisodeIndex, "%s (%d/%d) Complete", 
				lpNpc->Name, lpUser->m_QuestData[i].TalkIndex, pEpisode.Talk.size());
			this->MakeNotifyQuest(lpUser, SceneIndex, EpisodeIndex);
		}
	}
}
// -------------------------------------------------------------------------------

#endif