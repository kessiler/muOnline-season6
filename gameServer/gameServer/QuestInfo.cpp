//GameServer 1.00.77 JPN - Completed
//GameServer 1.00.90 JPN - Completed
#include "stdafx.h"
#include "QuestInfo.h"
#include "..\include\readscript.h"
#include "..\common\WzMemScript.h"
#include "..\common\winutil.h"
#include "logproc.h"
#include "GameServer.h"
#include "GameMain.h"
#include "protocol.h"
//#include "GameServerAuth.h"
#include "gObjMonster.h"
#include "DSProtocol.h"

#include "QuestUtil.h"
#include "MasterLevelSystem.h"

BYTE QuestBitMask[8];
CQuestInfo g_QuestInfo;

char sQuestString[4][50] = { "None", "Accept", "Clear", "None"};

CQuestInfo::CQuestInfo()
{
	QuestBitMask[0]=0xFC;
	QuestBitMask[1]=0xFC;
	QuestBitMask[2]=0xF3;
	QuestBitMask[3]=0xF3;
	QuestBitMask[4]=0xCF;
	QuestBitMask[5]=0xCF;
	QuestBitMask[6]=0x3F;
	QuestBitMask[7]=0x3F;

	this->Init();
}


CQuestInfo::~CQuestInfo()
{
	return;
}

void CQuestInfo::Init()
{
	this->m_QuestCount = 0;
	memset(this->QuestInfo, -1, sizeof(this->QuestInfo));

	for ( int i=0;i<MAX_QUEST_INFO;i++)
	{
		this->QuestInfo[i].QuestConditionCount=0;
		this->QuestInfo[i].QuestSubInfoCount= 0;
	}
}

BOOL CQuestInfo::LoadQuestInfo(char * filename)
{
	int Token;
	int n;

	SMDFile = fopen(filename, "r");	//ok

	if ( SMDFile == NULL )
	{
		MsgBox("load error %s", filename);
		return false;
	}

	this->Init();

	n = 0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		else
		{
			if ( Token == 1 )
			{
				int Type = TokenNumber;

				if ( Type == 0 )
				{
					Token = GetToken();
					int iQuestIndex = TokenNumber;
					this->QuestInfo[iQuestIndex].QuestIndex = iQuestIndex;

					Token = GetToken();
					this->QuestInfo[iQuestIndex].QuestStartType = TokenNumber;

					Token = GetToken();
					this->QuestInfo[iQuestIndex].QuestStartSubType = TokenNumber;

					Token = GetToken();
					strcpy( this->QuestInfo[iQuestIndex].Name, TokenString);

					this->QuestInfo[iQuestIndex].QuestSubInfoCount = 0;

					while ( true )
					{
						int iSubInfoCount = this->QuestInfo[iQuestIndex].QuestSubInfoCount;

						Token = GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", TokenString) == 0 )
							{
								break;
							}
						}
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].QuestType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedSubType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ItemLevel = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedNumber = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMinLevel = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMaxLevel = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedDropRate = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardSubType = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardCount = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].LinkConditionIndex = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextBeforeReg = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextAfterReg = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextCompleteQuest = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextQuestClear = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_WIZARD] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_KNIGHT] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_ELF] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_MAGUMSA] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_DARKLORD] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_SUMMONER] = TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_MONK] = TokenNumber;

						this->QuestInfo[iQuestIndex].QuestSubInfoCount++;

					
					}
					
					LogAdd("[Quest] (%s) Load : Index %d, SubInfoCount %d",	this->QuestInfo[iQuestIndex].Name, iQuestIndex,	this->QuestInfo[iQuestIndex].QuestSubInfoCount );
					this->m_QuestCount++;
				}
				else if ( Type == 1 )
				{
					Token = GetToken();
					int iQuestIndex = TokenNumber;

					while ( true )
					{
						auto iFailInfoCount = this->QuestInfo[iQuestIndex].QuestConditionCount;
						Token = GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", TokenString) == 0 )
							{
								break;
							}
						}
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].Index = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedQuestIndex = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MinLevel = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MaxLevel = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].ReqStr = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedZen = TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].StartContext = TokenNumber;

							this->QuestInfo[iQuestIndex].QuestConditionCount++;
						
					}
				}
				else
				{
					MsgBox("Quest Info Read Fail");
					fclose(SMDFile);
					return false;
				}
			}
		}
	}

	fclose(SMDFile);
	this->InitQuestItem();
	return true;
}


BOOL CQuestInfo::LoadQuestInfo(char* Buffer, int iSize)
{
	CWzMemScript WzMemScript;
	int Token;
	int n;

	WzMemScript.SetBuffer(Buffer, iSize);
	this->Init();

	n = 0;

	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
		{
			break;
		}
		else
		{
			if ( Token == 1 )
			{
				int Type = WzMemScript.GetNumber();

				if ( Type == 0 )
				{
					Token = WzMemScript.GetToken();
					auto iQuestIndex = WzMemScript.GetNumber();
					this->QuestInfo[iQuestIndex].QuestIndex = iQuestIndex;

					Token = WzMemScript.GetToken();
					this->QuestInfo[iQuestIndex].QuestStartType = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken();
					this->QuestInfo[iQuestIndex].QuestStartSubType = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken();
					strcpy( this->QuestInfo[iQuestIndex].Name, WzMemScript.GetString());

					this->QuestInfo[iQuestIndex].QuestSubInfoCount = 0;

					while ( true )
					{
						int iSubInfoCount = this->QuestInfo[iQuestIndex].QuestSubInfoCount;

						Token = WzMemScript.GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", WzMemScript.GetString()) == 0 )
							{
								break;
							}
						}
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].QuestType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedSubType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ItemLevel = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedNumber = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMinLevel = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMaxLevel = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedDropRate = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardSubType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardCount = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].LinkConditionIndex = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextBeforeReg = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextAfterReg = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextCompleteQuest = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextQuestClear = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_WIZARD] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_KNIGHT] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_ELF] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_MAGUMSA] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_DARKLORD] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_SUMMONER] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[CLASS_MONK] = WzMemScript.GetNumber();

						this->QuestInfo[iQuestIndex].QuestSubInfoCount++;

					
					}
					
					LogAdd("[Quest] (%s) Load : Index %d, SubInfoCount %d",	this->QuestInfo[iQuestIndex].Name, iQuestIndex,	this->QuestInfo[iQuestIndex].QuestSubInfoCount );
					this->m_QuestCount++;
				}
				else if ( Type == 1 )
				{
					Token = WzMemScript.GetToken();
					auto iQuestIndex = WzMemScript.GetNumber();

					while ( true )
					{
						auto iFailInfoCount = this->QuestInfo[iQuestIndex].QuestConditionCount;
						Token = WzMemScript.GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", WzMemScript.GetString()) == 0 )
							{
								break;
							}
						}	
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].Index = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedQuestIndex = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MinLevel = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MaxLevel = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].ReqStr = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedZen = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].StartContext = WzMemScript.GetNumber();

							this->QuestInfo[iQuestIndex].QuestConditionCount++;
						
					}
				}
				else
				{
					MsgBox("Quest Info Read Fail");
					fclose(SMDFile);
					return false;
				}
			}
		}
	}

	this->InitQuestItem();
	return true;
}


void CQuestInfo::InitQuestItem()
{
	int questcount = this->GetQeustCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;
	LPQUEST_SUB_INFO lpSubInfo;

	for (int i=0;i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		for ( int n=0;n<lpQuestInfo->QuestSubInfoCount;n++)
		{
			lpSubInfo = &lpQuestInfo->QuestSubInfo[n];

			if ( lpSubInfo != NULL )
			{
				if ( lpSubInfo->QuestType == 1 )
				{
					LPITEM_ATTRIBUTE p = &ItemAttribute[ITEMGET(lpSubInfo->NeedType, lpSubInfo->NeedSubType) ];
					p->QuestItem = TRUE;
					LogAddTD("[Quest] [SetQuestItem] %s", p->Name);
				}
			}
		}

		foundquest++;

		if ( foundquest == questcount )
		{
			return;
		}
	}
}


BOOL CQuestInfo::IsQuest(int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return false;
	}

	if ( this->QuestInfo[QuestIndex].QuestIndex == -1 )
	{
		return false;
	}

	return true;
}

				
int CQuestInfo::GetQuestState(LPOBJ lpObj, int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return false;
	}

	int index = (QuestIndex) /4;
	int shift = (QuestIndex % 4) *2;
	return (lpObj->m_Quest[index] >> shift) &3;
}

BYTE CQuestInfo::GetQuestStateBYTE(LPOBJ lpObj, int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return false;
	}
	
	int index = (QuestIndex / 4);
	return lpObj->m_Quest[index];
}

BYTE CQuestInfo::SetQuestState(LPOBJ lpObj, int QuestIndex, int State)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return -1;
	}

	int QuestState = this->GetQuestState(lpObj, QuestIndex);

	if ( QuestState == 3 || QuestState == 0)
	{
		BYTE btCond = this->QuestRunConditionCheck(lpObj, QuestIndex);

		if ( btCond != 0 )
		{
			return btCond;
		}

		this->QuestAccept(lpObj, QuestIndex);
		LogAddTD("[Quest] AcceptQuest [%s][%s] (%s)", lpObj->AccountID, lpObj->Name, this->GetQuestInfo(QuestIndex)->Name);
		State = 1;
	}
	else if ( QuestState == 1 )
	{
		BYTE btCond = this->QuestClearConditionCheck(lpObj, QuestIndex);

		if ( btCond != 0 )
		{
			return btCond;
		}

		this->QuestClear(lpObj, QuestIndex);
		State = 2;
		LogAddTD("[Quest] ClearQuest [%s][%s] (%s)", lpObj->AccountID, lpObj->Name,	this->GetQuestInfo(QuestIndex)->Name);
	}
	else
	{
		return -1;
	}

	int index = (QuestIndex / 4);
	int shift =  (QuestIndex % 4) *2;
	lpObj->m_Quest[index] &= QuestBitMask[shift];
	lpObj->m_Quest[index] |= (State&3) << shift;
	return 0;
}

BYTE CQuestInfo::ReSetQuestState(LPOBJ lpObj, int QuestIndex)
{
	int State = 3;
	int index = (QuestIndex/4);
	int shift = (QuestIndex % 4)*2;
	lpObj->m_Quest[index] &= QuestBitMask[shift];
	lpObj->m_Quest[index] |= (State&3) << shift;

	return 0;
}

LPQUEST_INFO CQuestInfo::GetQuestInfo(int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return false;
	}
	
	if ( this->QuestInfo[QuestIndex].QuestIndex == -1 )
	{
		return NULL;
	}

	return &this->QuestInfo[QuestIndex];
}

LPQUEST_SUB_INFO CQuestInfo::GetSubquestInfo(LPOBJ lpObj, LPQUEST_INFO lpQuestInfo, int subquestindex)
{
	if ( lpQuestInfo == NULL )
	{
		return NULL;
	}

	if ( subquestindex < 0 || subquestindex > MAX_SUBINFO_QUEST )
	{
		return NULL;
	}

	LPQUEST_SUB_INFO lpSubInfo = &lpQuestInfo->QuestSubInfo[subquestindex];
	int Class = lpObj->Class;
	int ChangeUP = lpObj->ChangeUP;
	int requireclass = lpSubInfo->RequireClass[Class];

	if ( requireclass == 0 )
	{
		return NULL;
	}

	if ( requireclass > 1 )
	{
		if ( requireclass != (ChangeUP + 1) )
		{
			return NULL;
		}
	}

	return lpSubInfo;
}

LPQUEST_CONDITION CQuestInfo::GetQuestCondition(LPQUEST_INFO lpQuestInfo, int conditionindex)
{
	if ( lpQuestInfo == NULL )
	{
		return NULL;
	}

	if ( conditionindex < 0 || conditionindex > MAX_CONDITION_QUEST )
	{
		return NULL;
	}

	return &lpQuestInfo->QuestCondition[conditionindex];
}

BYTE CQuestInfo::QuestClearConditionCheck(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return -1;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	BOOL bFoundSubQuest = FALSE;
	
	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			bFoundSubQuest = TRUE;

			if ( lpSubInfo->QuestType == 1 )
			{
				int NumberItemQuestFound = gObjGetItemCountInIventory(lpObj->m_Index, lpSubInfo->NeedType, lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);

				if ( NumberItemQuestFound < lpSubInfo->NeedNumber)
				{
					return 1;
				}
			}
		}
	}

	if ( bFoundSubQuest == FALSE )
	{
		return -1;
	}

	return 0;
}

BYTE CQuestInfo::QuestRunConditionCheck(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return -1;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	LPQUEST_CONDITION lpCondition;
	BOOL bFoundSubQuest = FALSE;


	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			bFoundSubQuest = TRUE;

			for ( int i=0;i<concount;i++)
			{
				lpCondition = this->GetQuestCondition(lpQuestInfo, i);

				if ( lpCondition != NULL && lpCondition->Index == -1)
				{
					if ( this->CompareCondition(lpObj, lpCondition) == FALSE )
					{
						return lpCondition->StartContext;
					}
				}
				else if ( lpCondition != NULL && lpCondition->Index == lpSubInfo->LinkConditionIndex )
				{
					if ( this->CompareCondition(lpObj, lpCondition) == 0)
					{
						return lpCondition->StartContext;
					}
				}
				else if ( lpCondition == NULL )
				{
					return -1;
				}
			}

		}
	}

	if ( bFoundSubQuest == FALSE )
	{
		return -1;
	}

	return 0;
}

BOOL CQuestInfo::CompareCondition(LPOBJ lpObj, LPQUEST_CONDITION lpCondition)
{
	if ( lpCondition->NeedQuestIndex != -1 )
	{
		if ( this->GetQuestState(lpObj, lpCondition->NeedQuestIndex) != 2 )
		{
			return FALSE;
		}
	}

	if ( lpCondition->MinLevel != 0 )
	{
		if ( lpCondition->MinLevel > lpObj->Level )
		{
			return FALSE;
		}
	}

	if ( lpCondition->MaxLevel != 0 )
	{
		if ( lpCondition->MaxLevel < lpObj->Level )
		{
			return FALSE;
		}
	}

	if ( lpCondition->NeedZen > lpObj->Money )
	{
		return FALSE;
	}

	if ( lpCondition->ReqStr > lpObj->Strength + lpObj->AddStrength)
	{
		return FALSE;
	}

	return TRUE;

}

BOOL CQuestInfo::NpcTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	int questcount = this->GetQeustCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;
	int queststate = -1;
	int questindex = -1;

	for ( int i = 0; i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		if ( lpQuestInfo->QuestStartType == 2 )
		{
			if ( lpNpc->Class == lpQuestInfo->QuestStartSubType)
			{
				if ( queststate == -1 )
				{
					questindex = lpQuestInfo->QuestIndex;
					queststate = this->GetQuestState(lpObj, lpQuestInfo->QuestIndex);
				}
				else if ( queststate == 2 )
				{
					for ( int n=0;n<lpQuestInfo->QuestSubInfoCount ;n++)
					{
						LPQUEST_SUB_INFO lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, n);

						if ( lpSubInfo != NULL )
						{
							questindex = lpQuestInfo->QuestIndex;
							queststate = this->GetQuestState(lpObj, lpQuestInfo->QuestIndex);
						}
					}
				}
			}
		}

		foundquest++;

		if ( foundquest == questcount )
		{
			break;
		}
	}

	if ( queststate != -1 )
	{
		int loc10 = this->GCSendQuestKillCountInfo(questindex, lpObj->m_Index);
		
		if(loc10 == 0)
		{
			return true;
		}
		
		GCSendQuestInfo(lpObj->m_Index, questindex);
		lpObj->m_IfState.type = 10;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;
		return true;
	}

	return false;
}

BOOL CQuestInfo::MonsterItemDrop(LPOBJ lpObj)
{
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj); //loc1

	if ( MaxHitUser == -1 )
	{
		return false;
	}

	int partycount = gParty.GetPartyCount(gObj[MaxHitUser].PartyNumber); //loc3

	if ( partycount > 0 ) //Season 2.5 changed
	{
		int MaxHitUserInParty = this->MonsterItemDropParty(lpObj,&gObj[MaxHitUser]);
		return MaxHitUserInParty;
	}

	LPOBJ lpTarget = &gObj[MaxHitUser];
	int questcount = this->GetQeustCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;
	LPQUEST_SUB_INFO lpSubInfo;
	int type;
	int level;
	int x;
	int y;
	float dur = 0;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	for ( int i=0;i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		for ( int n =0;n<lpQuestInfo->QuestSubInfoCount;n++)
		{
			lpSubInfo = this->GetSubquestInfo(lpTarget, lpQuestInfo, n);
			
			if ( lpSubInfo != NULL )
			{
				if ( lpSubInfo->QuestType == 1 )
				{
					if( (lpSubInfo->NeedTargetMinLevel > 0) && (lpObj->Level >= lpSubInfo->NeedTargetMinLevel) && (lpObj->Level <= lpSubInfo->NeedTargetMaxLevel) || (lpSubInfo->NeedTargetMinLevel == -1) && (lpObj->Class == lpSubInfo->NeedTargetMaxLevel))
					{
						if ( this->GetQuestState(lpTarget, lpQuestInfo->QuestIndex) == TRUE )
						{
							if ( (rand() % ITEM_QUEST_DROP_PROBABILITY) < lpSubInfo->NeedDropRate)
							{
									int itemcount = gObjGetItemCountInIventory(MaxHitUser, lpSubInfo->NeedType,
										lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);

									if ( itemcount >= lpSubInfo->NeedNumber)
									{
										continue;
									}

									dur = 0;
									x = lpObj->X;
									y = lpObj->Y;
									level = lpSubInfo->ItemLevel;
									type = ItemGetNumberMake(lpSubInfo->NeedType, lpSubInfo->NeedSubType);
									ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur, Option1,	Option2, Option3, MaxHitUser, 0, 0);
									LogAddTD("[Quest] Quest Item Drop [%s]: [%s][%s] (%s) (%d,%d)", lpObj->Name, lpTarget->AccountID, lpTarget->Name, lpQuestInfo->Name, lpSubInfo->NeedType, lpSubInfo->NeedSubType);
									return true;
							}
						}
					}
				}
			}
		}

		foundquest++;

		if ( foundquest == questcount )
		{
			break;
		}
	}

	return false;
}

BOOL CQuestInfo::MonsterItemDropParty(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int questcount = this->GetQeustCount(); //loc1
	int foundquest = 0; //loc2
	LPQUEST_INFO lpQuestInfo;
	LPQUEST_SUB_INFO lpSubInfo;
	int type;
	int level;
	int x;
	int y;
	float dur = 0; //loc10
	int Option1 = 0; //loc11
	int Option2 = 0; //loc12
	int Option3 = 0; //loc13
	int TargetPartyNumber = lpTargetObj->PartyNumber; //loc14
	int LocalPartyNumber; //loc15

	if(TargetPartyNumber < 0)
	{
		return false;
	}

	LPOBJ loc16; //loc16

	int n; //loc17
	int i; //loc18
	int j; //loc19

	for ( n=0;n<MAX_USER_IN_PARTY;n++)
	{
		LocalPartyNumber = gParty.m_PartyS[TargetPartyNumber].Number[n];

		if(LocalPartyNumber < 0)
		{
			continue;
		}

		if (gObj[LocalPartyNumber].MapNumber != lpTargetObj->MapNumber)
		{
			continue;
		}

		loc16 = &gObj[LocalPartyNumber];

		for ( i=0;i<MAX_QUEST_INFO;i++)
		{
			lpQuestInfo = this->GetQuestInfo(i);

			if ( lpQuestInfo == NULL )
			{
				continue;
			}

			for ( j=0;j<lpQuestInfo->QuestSubInfoCount;j++)
			{
				lpSubInfo = this->GetSubquestInfo(loc16, lpQuestInfo, j);
			
				if ( lpSubInfo == NULL )
				{
					continue;
				}
				
				if ( lpSubInfo->QuestType == 1 )
				{
					if( (lpSubInfo->NeedTargetMinLevel > 0) && (lpObj->Level >= lpSubInfo->NeedTargetMinLevel) && (lpObj->Level <= lpSubInfo->NeedTargetMaxLevel) || (lpSubInfo->NeedTargetMinLevel == -1) && (lpObj->Class == lpSubInfo->NeedTargetMaxLevel))
					{
						if ( this->GetQuestState(loc16, lpQuestInfo->QuestIndex) == TRUE )
						{
							if ( (rand() % ITEM_QUEST_DROP_PROBABILITY) < lpSubInfo->NeedDropRate)
							{
								dur = 0;
								x = lpTargetObj->X;
								y = lpTargetObj->Y;
								level = lpSubInfo->ItemLevel;
								type = ItemGetNumberMake(lpSubInfo->NeedType, lpSubInfo->NeedSubType);
								ItemSerialCreateSend(lpTargetObj->m_Index, lpTargetObj->MapNumber, x, y, type, level, dur, Option1, Option2, Option3, lpTargetObj->m_Index, 0, 0);
								LogAddTD("[Quest] Quest Item Drop(Party) [%s]: [%s][%s] (%s) (%d,%d)", lpObj->Name,	lpTargetObj->AccountID, lpTargetObj->Name, lpQuestInfo->Name, lpSubInfo->NeedType, lpSubInfo->NeedSubType);
								return true;
							}
						}
					}					
				}
			}

			foundquest++;

			if ( foundquest == questcount )
			{
				break;
			}

		}
	}

	return false;
}

BOOL CQuestInfo::QuestAccept(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return 0xFF;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	LPQUEST_CONDITION lpCondition;
	int NeedZen = 0;

	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			for ( int n = 0;n<concount;n++)
			{
				lpCondition = this->GetQuestCondition(lpQuestInfo, n);

				if(lpCondition != 0 && lpCondition->Index == -1) //Season 2.5 changed
				{
					NeedZen += lpCondition->NeedZen;
				}
				else if(lpCondition != 0 && (lpCondition->Index == lpSubInfo->LinkConditionIndex)) //season 2.5 add-on
				{
					NeedZen += lpCondition->NeedZen;
				}
			}

			if(lpSubInfo->QuestType == 2) //season 2.5 add-on
			{
				if(lpObj->m_i3rdQuestIndex == -1)
				{
					lpObj->m_i3rdQuestIndex = QuestIndex;
				}

				for ( int n = 0;n<5;n++)
				{
					if(lpObj->MonsterKillInfo[n].MonIndex == -1)
					{
						lpObj->MonsterKillInfo[n].MonIndex = lpSubInfo->NeedType;
						lpObj->MonsterKillInfo[n].KillCount = 0;
						break;
					}
				}

				LogAddTD("[Quest] Quest Accept - MonsterKillCount Set (%s) : [%s][%s]", lpQuestInfo, lpObj->AccountID, lpObj->Name);
			}

		}
	}

	lpObj->Money -= NeedZen;
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	return true;
}

BOOL CQuestInfo::QuestClear(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( szAuthKey[18] != AUTHKEY18 )
	{
		DestroyGIocp();
	}

	if ( lpQuestInfo == NULL )
	{
		return 0xFF;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;

	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			if ( lpSubInfo->RewardType == QUEST_COMPENSATION_CHANGEUP )
			{
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_STATUP, lpSubInfo->RewardCount );
				lpObj->ChangeUP = 1;
				lpObj->DbClass |= 1;
				gObjMakePreviewCharSet(lpObj->m_Index);
				BYTE btClass = (lpObj->Class<<5)&0xE0;
				btClass |= (lpObj->ChangeUP<<4)&0x10;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_CHANGEUP, btClass);
				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), ChangeUp",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint,
					lpSubInfo->RewardCount );
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_STATUP )
			{
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_STATUP, lpSubInfo->RewardCount );
				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d)",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint,
					lpSubInfo->RewardCount );
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_PLUSSSTAT )
			{
				int level = lpObj->Level - QUEST_MINLEVEL_PLUSSTAT;

				if ( level <= 0 )
				{
					level = 0;
				}

				lpObj->LevelUpPoint += level;
				lpObj->PlusStatQuestClear = true;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_PLUSSSTAT, level );
				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), PlusStat",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint, level);
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_COMBOSKILL )
			{
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_COMBOSKILL, 0 );
				lpObj->ComboSkillquestClear = true;
				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] ComboSkill",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name);
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_ThirdChangeUp ) //season 2.5 add-on
			{
				lpObj->ThirdChangeUp = 1;
				lpObj->DbClass |= 2;
				gObjMakePreviewCharSet(lpObj->m_Index);
				BYTE btClass = (lpObj->Class<<5)&0xE0;
				btClass |= (lpObj->ChangeUP<<4)&0x10;
				btClass |= (lpObj->ThirdChangeUp<<0x03)&0x08;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_ThirdChangeUp, btClass );

				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Class:%d(%d), 3rd ChangeUp",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->DbClass, btClass);
			}
			
			if ( lpObj->ChangeUP == 1 ) //HermeX Add-on for fixing registers
			{
#if(TESTSERVER==1)
				LogAddTD("[Quest] Quest Clear - ChangeUP (%d) DbClass (%d) : [%s][%s]", lpObj->ChangeUP, lpObj->DbClass, lpObj->AccountID, lpObj->Name);
#endif
			}

			if ( lpSubInfo->QuestType == 1 )
			{
				gObjDelteItemCountInInventory(lpObj->m_Index, lpSubInfo->NeedType, lpSubInfo->NeedSubType,
					lpSubInfo->NeedNumber);
			}

			if ( lpSubInfo->QuestType == 2 ) //season 2.5 add-on
			{
				lpObj->m_i3rdQuestIndex = -1;
				
				for ( int n = 0;n<5;n++)
				{
					lpObj->MonsterKillInfo[n].MonIndex = -1;
					lpObj->MonsterKillInfo[n].KillCount = -1;
				}
				LogAddTD("[Quest] Quest Clear - MonsterKillCount Reset (%s) : [%s][%s]", lpQuestInfo, lpObj->AccountID, lpObj->Name);
			}

			if( lpSubInfo->RewardSubType == QUEST_COMPENSATION_STATUP) //season 2.5 add-on
			{
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_STATUP, lpSubInfo->RewardCount );
				LogAddTD("[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), Class:%d PlusStat",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint,
					lpSubInfo->RewardCount, lpObj->DbClass );
			}
		}
	}
	return TRUE;
}


void CQuestInfo::QuestInfoSave(LPOBJ lpObj)
{
	int questcount = this->GetQeustCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;

	for ( int i = 0 ; i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		LogAddTD("[Quest] QuestSave [%s][%s] index(%d) name(%s) state(%s)",
			lpObj->AccountID, lpObj->Name, i, lpQuestInfo->Name,
			sQuestString[this->GetQuestState(lpObj, i)]);
		foundquest++;

		if ( foundquest == questcount )
		{
			break;
		}
	}
}

BOOL CQuestInfo::MonsterPlusKillCountParty(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpTargetObj->Connected < PLAYER_PLAYING)
	{
		if(lpTargetObj->Live != FALSE)
		{
			return FALSE;
		}
	}

	int PartyNumber = lpTargetObj->PartyNumber;

	if(PartyNumber < 0)
	{
		if(lpTargetObj->m_i3rdQuestIndex == -1)
		{
			return FALSE;
		}

		for ( int n = 0;n<5;n++)
		{
			if(lpObj->Class == lpTargetObj->MonsterKillInfo[n].MonIndex)
			{
				if(lpTargetObj->MonsterKillInfo[n].KillCount <= 50)
				{
					lpTargetObj->MonsterKillInfo[n].KillCount++;
					break;
				}
			}
		}
	}
	else
	{
			int TargetPartyNumber = lpTargetObj->PartyNumber; //loc4
			int LocalPartyNumber; //loc5
			LPOBJ loc6; //loc6
			int dis = 0;

			for ( int n=0;n<MAX_USER_IN_PARTY;n++) //loc8
			{
				LocalPartyNumber = gParty.m_PartyS[TargetPartyNumber].Number[n];

				if(LocalPartyNumber < 0)
				{
					continue;
				}

				loc6 = &gObj[LocalPartyNumber];

				if(loc6->Connected < 3)
				{
					if(loc6->Live != FALSE)
					{
						continue;
					}
				}
				
				if (lpTargetObj->MapNumber != loc6->MapNumber)
				{
					continue;
				}

				dis = gObjCalDistance(lpTargetObj,loc6);

				if(dis > 25)
				{
					continue;
				}

				if(loc6->m_i3rdQuestIndex == -1)
				{
					continue;
				}

				for ( int n=0;n<5;n++)
				{
					if(lpObj->Class == loc6->MonsterKillInfo[n].MonIndex)
					{
						if(loc6->MonsterKillInfo[n].KillCount <= 50)
						{
							loc6->MonsterKillInfo[n].KillCount++;
							break;
						}
					}
				}
			}
	}

	return TRUE;
}

BOOL CQuestInfo::GCSendQuestKillCountInfo(int QuestIndex, int aIndex)
{
	PMSG_SETQUEST_KILLCOUNT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x00, sizeof(pMsg));
	pMsg.Result = 0; //loc12
	pMsg.QuestIndex = QuestIndex; //loc13

	LPOBJ lpObj = &gObj[aIndex]; //loc14

	int QuestState = this->GetQuestState(lpObj, QuestIndex); //loc15

	if(QuestState != TRUE)
	{
		return TRUE;
	}

	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex); //loc16

	if(lpQuestInfo == NULL)
	{
		return TRUE;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount; //loc17
	int concount = lpQuestInfo->QuestConditionCount; //loc18
	LPQUEST_SUB_INFO lpSubInfo; //loc19

	int loc20 = 0; //loc20

	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			loc20 = 1;
			if(lpSubInfo->QuestType == 2)
			{
				if(lpObj->m_i3rdQuestState == 0)
				{
					g_QuestUtil.Quest3rdRequestInfo(lpObj);
					return FALSE;
				}
				if(lpObj->m_i3rdQuestState == 1)
				{
					if(lpObj->m_i3rdQuestIndex == -1)
					{
						lpObj->m_i3rdQuestState = 0;
						g_QuestUtil.Quest3rdRequestInfo(lpObj);
						LogAddTD("[Quest] Error - Invalid MonsterKillInfo [%s][%s] (QuestIndex:%d/DBIndex:%d)",lpObj->AccountID, lpObj->Name, QuestIndex, lpObj->m_i3rdQuestIndex);
						return FALSE;
					}
				}
				
				pMsg.Result = 1;
				pMsg.QuestIndex = lpObj->m_i3rdQuestIndex;

				int loc22 = 0; //loc22

				for ( int n=0;n<5;n++) //loc23
				{
					pMsg.dwData[loc22] = lpObj->MonsterKillInfo[n].MonIndex; loc22++;
					pMsg.dwData[loc22] = lpObj->MonsterKillInfo[n].KillCount; loc22++;
				}
				break;
			}
		}
	}
	//
	if(pMsg.Result == 1)
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	return TRUE;
}

//004A1800   /> \55                            PUSH EBP (Werewolf Quarrel)
void CQuestInfo::GCReqWerewolfMove(int aIndex) //Identical gs-cs 56
{
	LPOBJ lpObj = &gObj[aIndex]; //loc2

	if(lpObj == NULL)
	{
		return;
	}

	PMSG_WEREWOLF_MOVE pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x07, sizeof(pMsg));
	pMsg.result = 0;
	
	int ReqZen = 3000000; //loc5
	int ReqLvl = 350; //loc6

	if(lpObj->Level < 350 || lpObj->Money < ReqZen)
	{
		pMsg.result = 0;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	int QuestState = this->GetQuestState(lpObj, 5); //loc7
	
	if(QuestState == 1 || QuestState == 2)
	{
		LPOBJ loc8; //loc8
		int PartyNumber = lpObj->PartyNumber; //loc9
		int LocalPartyNumber; //loc10
		int dis; //loc11
		BOOL bMoveGateSuccess = FALSE; //loc12

		if(PartyNumber >=0)
		{
			for ( int n=0;n<MAX_USER_IN_PARTY;n++) //loc13
			{
				LocalPartyNumber = gParty.m_PartyS[PartyNumber].Number[n];

				if(LocalPartyNumber < 0)
				{
					continue;
				}

				if(aIndex == LocalPartyNumber)
				{
					continue;
				}

				loc8 = &gObj[LocalPartyNumber];

				if(loc8->Level < ReqLvl)
				{
					continue;
				}

				if(lpObj->MapNumber == loc8->MapNumber)
				{
					dis = gObjCalDistance(lpObj,loc8);

					if(dis < 10)
					{
						bMoveGateSuccess = gObjMoveGate(LocalPartyNumber,256);
						
						if(bMoveGateSuccess == FALSE)
						{
							pMsg.result = 0;
							DataSend(LocalPartyNumber, (LPBYTE)&pMsg, pMsg.h.size);
						}
					}
				}

			}
		}

		bMoveGateSuccess = gObjMoveGate(aIndex,256);
						
		if(bMoveGateSuccess != FALSE)
		{
			lpObj->Money -= ReqZen;
			GCMoneySend(lpObj->m_Index,lpObj->Money);
			return;
		}
		
		pMsg.result = 0;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

//004A1AA0   /> \55                            PUSH EBP (GateKeeper)
void CQuestInfo::GCReqGateKeeperMove(int aIndex) //Identical gs-cs 56
{
	LPOBJ lpObj = &gObj[aIndex]; //loc2

	if(lpObj == NULL)
	{
		return;
	}

	PMSG_GATEKEEPER_MOVE pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xD0, 0x08, sizeof(pMsg));
	pMsg.result = 0;
	
	int ReqLvl = 350; //loc5

	if(lpObj->Level < ReqLvl)
	{
		pMsg.result = 0;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(lpObj->m_i3rdQuestIndex != 6)
	{
		pMsg.result = 0;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	LPOBJ loc6; //loc6
	int PartyNumber = lpObj->PartyNumber; //loc7
	int LocalPartyNumber; //loc8
	int dis; //loc9
	BOOL bMoveGateSuccess = FALSE; //loc10

	if(PartyNumber >=0)
	{
		for ( int n=0;n<MAX_USER_IN_PARTY;n++) //loc11
		{
			LocalPartyNumber = gParty.m_PartyS[PartyNumber].Number[n];

			if(LocalPartyNumber < 0)
			{
				continue;
			}

			if(aIndex == LocalPartyNumber)
			{
				continue;
			}

			loc6 = &gObj[LocalPartyNumber];

			if(loc6->Level < ReqLvl)
			{
				continue;
			}

			if(lpObj->MapNumber == loc6->MapNumber)
			{
				dis = gObjCalDistance(lpObj,loc6);

				if(dis < 10)
				{
					bMoveGateSuccess = gObjMoveGate(LocalPartyNumber,257);
				
					if(bMoveGateSuccess == FALSE)
					{
						pMsg.result = 0;
						DataSend(LocalPartyNumber, (LPBYTE)&pMsg, pMsg.h.size);
						return;
					}
				}
			}		
		}
	}
	//
	bMoveGateSuccess = gObjMoveGate(aIndex,257);
						
	if(bMoveGateSuccess == FALSE)
	{
		pMsg.result = 0;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void CQuestInfo::QuestSuccessCommand(int aIndex, int QuestIndex) //004965E0
{	//Completed Hidden Function
#if(TESTSERVER == 0)
	return;
#endif
	
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->ThirdChangeUp == 1) return;

	int State = 2;	//Clear Condition
	int index = (QuestIndex/4);
	int shift = (QuestIndex % 4)*2;

	//Set Quest Clear Condition (prevent reload in case quest still active)
	lpObj->m_Quest[index] &= QuestBitMask[shift];
	lpObj->m_Quest[index] |= (State&3) << shift;

	//Send User New Class Statistics
	if(QuestIndex == 6)
	{
		lpObj->ThirdChangeUp = true;
		lpObj->DbClass |= 2;
		g_MasterLevelSystem.GDReqMasterLevelInfo(lpObj); //season 4.5 add-on
	}

	gObjMakePreviewCharSet(lpObj->m_Index);

	//Clear 3rd Quest User State
	lpObj->m_i3rdQuestState = 1;	//Loaded (prevent reload)
	lpObj->m_i3rdQuestIndex = -1;	//Set 0 (prevent reload when open Priest Devin)

	//Clear 3rd Quest Monster Kill State
	for(int i = 0; i < 5; i++)
	{
		lpObj->MonsterKillInfo[i].MonIndex = -1;	//ReSet
		lpObj->MonsterKillInfo[i].KillCount = -1;	//ReSet
	}

	GCServerMsgStringSend("Äù½ºÆ® ¼º°ø",lpObj->m_Index,1); //Quest Success
}

BOOL CQuestInfo::CountQuestItemInInventory(int aIndex, short arg2, short arg3)
{
	if(gObj[aIndex].m_i3rdQuestIndex != -1)
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex]; //loc2

	LPQUEST_INFO lpQuestInfo; //loc3
	LPQUEST_SUB_INFO lpSubInfo; //loc4

	int loc5; //loc5
	int loc6 = 3; //loc6

	for ( int i=0;i<this->m_QuestCount;i++) //loc7
	{
		loc6 = this->GetQuestState(lpObj, i);

		if(loc6 == 1)
		{
			lpQuestInfo = this->GetQuestInfo(i);

			if(lpQuestInfo == NULL)
			{
				return FALSE;
			}

			loc5 = lpQuestInfo->QuestSubInfoCount;

			for ( int j=0;j<loc5;j++)
			{
				lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, j);

				if(lpSubInfo == NULL)
				{
					continue;
				}

				if(lpSubInfo->QuestType != 1)
				{
					return FALSE;
				}

				if(arg2 == ITEMGET(lpSubInfo->NeedType, lpSubInfo->NeedSubType))
				{
					if(arg3 == lpSubInfo->ItemLevel)
					{
						int NumberItemQuestFound = gObjGetItemCountInIventory(aIndex, arg2);

						if ( NumberItemQuestFound >= lpSubInfo->NeedNumber)
						{
							LogAdd("[Quest] Too many has quest items [%s][%s] (%d/%d)",	lpObj->AccountID, lpObj->Name, arg2, NumberItemQuestFound);
							return FALSE;
						}
						return TRUE;
					}
				}
			}
			return FALSE;
		}
	}

	return FALSE;
}