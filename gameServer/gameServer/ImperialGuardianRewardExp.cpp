// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "ImperialGuardianRewardExp.h"
#include "..\common\ReadScript.h"
#include "gamemain.h"
#include "logproc.h"
#include "user.h"

CImperialGuardianRewardExp::CImperialGuardianRewardExp(void)
{
	Init();
}


CImperialGuardianRewardExp::~CImperialGuardianRewardExp(void)
{
}

void CImperialGuardianRewardExp::Init()
{
	m_bLoadScript = false;
}

void CImperialGuardianRewardExp::LoadScript(const char *lpFileName)
{
	_stRewardExpInfo rewardExpInfo;
	int nType;
	SMDToken Token;

	this->m_bLoadScript = false;


	if ( !lpFileName )
	{
		LogAdd("[IMPERIALGUARDIAN][REWARDEXP] lpFileName is null!!");
		return;
	}


	SMDFile = fopen(lpFileName, "r");	//ok

	if ( !SMDFile )
	{
		LogAdd(lMsg.Get(453), lpFileName);
		return;
	}

	while ( 1 )
	{
		Token = GetToken();
		if ( Token == SMDToken::END )
			break;

		if ( Token == SMDToken::NUMBER )
		{
			nType = TokenNumber;
			if ( nType == 0)
			{
				while ( 1 )
				{
					Token = GetToken();

					if ( !Token )
					{
						if ( !strcmp("end", TokenString) )
							break;
					}

					stRewardExpInfo rewardExpInfo;

					rewardExpInfo.MinUserLevel = TokenNumber;

					Token = GetToken();
					rewardExpInfo.MaxUserLevel = TokenNumber;

					Token = GetToken();
					rewardExpInfo.RewardExp = TokenNumber;

					m_vtRewardExpInfo.push_back(rewardExpInfo);
				}
			}
		}
	}


	fclose(SMDFile);

	LogAdd(lMsg.Get(454), lpFileName);

	this->m_bLoadScript = true;
}




int CImperialGuardianRewardExp::GetRewardExp(int nUserIndex)
{
	int nSize = this->m_vtRewardExpInfo.size();
	int nResult = 0;


	if( !OBJMAX_RANGE(nUserIndex) )
	{
		LogAdd("error : %s %d", __FILE__, __LINE__);
		return 0;
	}

	OBJECTSTRUCT * lpObj = &gObj[nUserIndex];

	int nUserLevel = gObj[nUserIndex].m_nMasterLevel + gObj[nUserIndex].Level;
	
	for (int i = 0; i < nSize; ++i )
	{
		stRewardExpInfo rewardExpInfo = this->m_vtRewardExpInfo.at(i);

		if( nUserLevel >= rewardExpInfo.MinUserLevel && nUserLevel <= rewardExpInfo.MaxUserLevel)
		{
			nResult = rewardExpInfo.RewardExp;
			//break;
		}
	}

	return nResult;
}