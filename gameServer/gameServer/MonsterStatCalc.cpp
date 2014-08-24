// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "MonsterStatCalc.h"
#include "..\common\ReadScript.h"
#include "logproc.h"
#include "user.h"
#include "MonsterAttr.h"
#include "GameMain.h"

CMonsterStatCalc g_MonsterStatCalc;

CMonsterStatCalc::CMonsterStatCalc(void)
{
}


CMonsterStatCalc::~CMonsterStatCalc(void)
{
}

void CMonsterStatCalc::LoadScript(const char *Filename)	//OK
{

	int nType;
	SMDToken Token;

	m_bScriptLoad = 0;
	SMDFile = fopen(Filename, "r");	//ok
	if ( !SMDFile )
	{
		LogAdd(lMsg.Get(453), Filename);
		return;
	}

	while ( 1 )
	{
		Token = GetToken();
		if ( Token == 2 )
			break;

		if ( Token == 1 )
		{
			nType = TokenNumber;
			if ( !nType )
			{
				while ( 1 )
				{
					Token = GetToken();

					if ( !Token )
					{
						if ( !strcmp("end", TokenString) )
							break;
					}

					stMonsterStatRef monsterStatRef;

					//monsterStatRef.minUserReset = TokenNumber;

					//GetToken();
					//monsterStatRef.maxUserReset = TokenNumber;

					//GetToken();
					monsterStatRef.minUserLevel = TokenNumber;

					Token = GetToken();
					monsterStatRef.maxUserLevel = TokenNumber;

					Token = GetToken();
					monsterStatRef.level_mul = TokenNumber;

					Token = GetToken();
					monsterStatRef.hp_mul = TokenNumber;

					Token = GetToken();
					monsterStatRef.mp_mul = TokenNumber;

					Token = GetToken();
					monsterStatRef.min_dmg_mul = TokenNumber;

					Token = GetToken();
					monsterStatRef.max_dmg_mul = TokenNumber;

					Token = GetToken();
					monsterStatRef.def_mul = TokenNumber;

					Token = GetToken();
					monsterStatRef.magic_def_mul = TokenNumber;

					Token = GetToken();
					monsterStatRef.attack_rate_mul = TokenNumber;

					Token = GetToken();
					monsterStatRef.def_rate_mul = TokenNumber;

					m_MonsterStatRef.push_back(monsterStatRef);
				}
			}
		}
	}

	fclose(SMDFile);

	LogAdd(lMsg.Get(454), Filename);
	m_bScriptLoad = 1;
}

int CMonsterStatCalc::ConvertMonsterStat(OBJECTSTRUCT *lpObj, int nUserMaxLevel, int nUserMaxReset)	//OK
{
	stMonsterStatRef monsterStatRef;

	if ( !m_bScriptLoad )
		return 0;

	if ( !lpObj )
		return 0;

	MONSTER_ATTRIBUTE *lpm = gMAttr.GetAttr(lpObj->Class);

	if ( !lpm )
	{
		LogAdd(lMsg.Get(509), lpObj->Class, __FILE__, __LINE__);
		return 0;
	}

    if ( lpm->m_Level == 0 )
    {
		LogAdd(lMsg.Get(510), lpObj->Class, __FILE__, __LINE__);
		return 0;        
	}

    int nSize = m_MonsterStatRef.size();
    for (int i = 0; i < nSize; ++i )
    {
						monsterStatRef = m_MonsterStatRef.at(i);

						//if( monsterStatRef.minUserReset <= nUserMaxReset && nUserMaxReset <= monsterStatRef.maxUserReset )
						//{
								if ( monsterStatRef.minUserLevel <= nUserMaxLevel && monsterStatRef.maxUserLevel >= nUserMaxLevel )
								{
									strncpy(lpObj->Name, lpm->m_Name, sizeof(MAX_ACCOUNT_LEN));
									lpObj->Level = lpm->m_Level * monsterStatRef.level_mul;
									lpObj->Life = lpm->m_Hp * monsterStatRef.hp_mul;
									lpObj->MaxLife = lpm->m_Hp * monsterStatRef.hp_mul;
									lpObj->Mana = lpm->m_Mp * monsterStatRef.mp_mul;
									lpObj->MaxMana = lpm->m_Mp * monsterStatRef.mp_mul;
									lpObj->m_AttackDamageMin = lpm->m_DamageMin * monsterStatRef.min_dmg_mul;
									lpObj->m_AttackDamageMax = lpm->m_DamageMax * monsterStatRef.max_dmg_mul;
									lpObj->m_Defense = lpm->m_Defense * monsterStatRef.def_mul;
									lpObj->m_MagicDefense = lpm->m_MagicDefense * monsterStatRef.magic_def_mul;
									lpObj->m_AttackRating = lpm->m_AttackRating * monsterStatRef.attack_rate_mul;
									lpObj->m_SuccessfulBlocking = lpm->m_Successfulblocking * monsterStatRef.def_rate_mul;
									lpObj->m_MoveRange = lpm->m_MoveRange;
									lpObj->m_AttackSpeed = lpm->m_AttackSpeed;
									lpObj->m_MoveSpeed = lpm->m_MoveSpeed;
									lpObj->MaxRegenTime = 1000 * lpm->m_RegenTime;
									lpObj->m_AttackRange = lpm->m_AttackRange;
									lpObj->m_ViewRange = lpm->m_ViewRange;
									lpObj->m_Attribute = lpm->m_Attribute;
									lpObj->m_AttackType = lpm->m_AttackType;
									lpObj->m_ItemRate = lpm->m_ItemRate;
									lpObj->m_MoneyRate = lpm->m_MoneyRate;
									lpObj->Dexterity = 0;
									lpObj->m_Resistance[0] = lpm->m_Resistance[0];
									lpObj->m_Resistance[1] = lpm->m_Resistance[1];
									lpObj->m_Resistance[2] = lpm->m_Resistance[2];
									lpObj->m_Resistance[3] = lpm->m_Resistance[3];
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
									memcpy(lpObj->m_Resistance, lpm->m_Resistance, sizeof(lpObj->m_Resistance));
									gObjGiveItemSearch(lpObj, lpm->m_MaxItemLevel);
									gObjGiveItemWarehouseSearch(lpObj, lpm->m_MaxItemLevel);

									break;
								}
						//}
    }


	return 1;
}