#include "StdAfx.h"
#include "GameMain.h"
#include "user.h"
#include "OfflineAttack.h"
#include "MasterLevelSkillTreeSystem.h"
#include "giocp.h"
#include "ObjUseSkill.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "..\common\winutil.h"
// -------------------------------------------------------------------------------

OfflineAttack	g_OfflineAttack;
// -------------------------------------------------------------------------------

OfflineAttack::OfflineAttack()
{
	this->Init();
}
// -------------------------------------------------------------------------------

OfflineAttack::~OfflineAttack()
{

}
// -------------------------------------------------------------------------------

void OfflineAttack::Init()
{
	ZeroMemory(this->CharInfo, sizeof(this->CharInfo));
	ZeroMemory(&this->m_Data, sizeof(this->m_Data));
	this->m_MaxCastTime = 0;
	this->m_MinLevel	= -1;
}
// -------------------------------------------------------------------------------

void OfflineAttack::ReadListData(char * File)
{
	SMDToken Token;
	this->Init();
	// ----
	SMDFile = fopen(File, "r");
	// ----
	if( !SMDFile )
	{
		MsgBox(lMsg.Get(MSGGET(0, 112)), File);
		return;
	}
	// ----
	int MapLoaded	= 0;
	int MagicLoaded = 0;
	int BuffLoaded	= 0;
	// ----
	while(true)
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		if( Token == NUMBER )
		{
			int Type = TokenNumber;
			// ----
			if( Type == 0 )	//-> Forbidden Maps
			{
				while(true)
				{
					Token = GetToken();
					// ----
					if( Token == NAME && !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					this->m_Data.ForbiddenMaps[MapLoaded] = TokenNumber;
					MapLoaded++;
				}
			}
			else if( Type == 1 ) //-> Allowed Magic
			{
				while(true)
				{
					Token = GetToken();
					// ----
					if( Token == NAME && !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					int Class = TokenNumber;
					Token = GetToken();
					this->m_Data.AllowMagic[Class][MagicLoaded] = TokenNumber;
					// ----
					MagicLoaded++;
				}
			}
			/*else if( Type == 2 ) //-> Allowed Buff Magic
			{
				while(true)
				{
					Token = GetToken();
					// ----
					if( Token == NAME && !strcmp("end", TokenString) )
					{
						break;
					}
					// ----
					BuffLoaded++;
				}
			}*/
		}
	}
	// ----
	fclose(SMDFile);
}
// -------------------------------------------------------------------------------

void OfflineAttack::ReadMainData(char * File)
{
	this->m_MaxCastTime	= GetPrivateProfileIntA("Common", "MaxCastTime", 0, File);
	this->m_MinLevel	= GetPrivateProfileIntA("Common", "MinLevel", 0, File);
}
// -------------------------------------------------------------------------------

void OfflineAttack::Load()
{
	this->ReadListData(gDirPath.GetNewPath("Custom\\OffAttackList.txt"));
	this->ReadMainData(gDirPath.GetNewPath("Custom\\OffAttackMain.ini"));
}
// -------------------------------------------------------------------------------

bool OfflineAttack::CheckMap(LPOBJ lpUser)
{
	for( int i = 0; i < MAX_OFFATTACK_MAP; i++ )
	{
		if( lpUser->MapNumber == this->m_Data.ForbiddenMaps[i] )
		{
			return false;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

bool OfflineAttack::CheckSkill(LPOBJ lpUser, WORD MagicNumber)
{
	CMagicInf * lpMagic = gObjGetMagicSearch(lpUser, MagicNumber);
	// ----
	if( !lpMagic )
	{
		return false;
	}
	// ----
	for( int i = 0; i < MAX_OFFATTACK_MAGIC; i++ )
	{
		if( MagicNumber != this->m_Data.AllowMagic[lpUser->Class][i] )
		{
			continue;
		}
		// ----
		return true;
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

DWORD OfflineAttack::GetAttackDelay(LPOBJ lpUser)
{
	int SpeedValue	= lpUser->m_MagicSpeed;
	// ----
	if( SpeedValue > 800 )
	{
		SpeedValue = 800;
	}
	// ----
	return 850 - SpeedValue;
}
// -------------------------------------------------------------------------------

void OfflineAttack::StartingAttack(int UserIndex, OFFLINEATTACK_REQ * aRecv)
{
	LPOBJ lpUser = &gObj[UserIndex];
	// ----
	if( lpUser->Level < this->m_MinLevel )
	{
		GCServerMsgStringSend("You need larger level to use Offline Attack", lpUser->m_Index, 1);
		return;
	}
	// ----
	if( !this->CheckMap(lpUser) )
	{
		GCServerMsgStringSend("Offline Attack is not allowed on this location", lpUser->m_Index, 1);
		return;
	}
	// ----
	if( !this->CheckSkill(lpUser, aRecv->MagicNumber) )
	{
		GCServerMsgStringSend("You need select other skill to use Offline Attack", lpUser->m_Index, 1);
		return;
	}
	// ----
	this->CharInfo[UserIndex].IsOffline		= true;
	this->CharInfo[UserIndex].MagicNumber	= aRecv->MagicNumber;
	// ----
	CloseClientEx(UserIndex);
	_beginthread(this->TryUseSkill, 0, lpUser);
}
// -------------------------------------------------------------------------------

void OfflineAttack::StoppingAttack(int UserIndex)
{
	this->CharInfo[UserIndex].IsOffline		= false;
	this->CharInfo[UserIndex].MagicNumber	= -1;
	ResponErrorCloseClient(UserIndex);
}
// -------------------------------------------------------------------------------

void OfflineAttack::TryUseSkill(void * lpArgList)
{
	LPOBJ lpObj				= (LPOBJ)lpArgList;
	DWORD UseBuffEachXTime	= 0;
	DWORD StartTimeUseSkill = GetTickCount();
	// ----
	while(true)
	{
		if( !g_OfflineAttack.CharInfo[lpObj->m_Index].IsOffline )
		{
			break;
		}
		// ----
		if( (GetTickCount() - StartTimeUseSkill) > (g_OfflineAttack.m_MaxCastTime * (1000 * 60)) )
		{
			g_OfflineAttack.StoppingAttack(lpObj->m_Index);
			break;
		}
		// ----
		for(int n = 0; n < MAX_VIEWPORT; n++ )
		{
			int	TargetIndex = lpObj->VpPlayer2[n].number;
			// ----
			if( TargetIndex < 0 || gObj[TargetIndex].Type != OBJ_MONSTER )
			{
				continue;
			}
			// ----		
			switch(lpObj->Class)
			{
			case CLASS_WIZARD:
				{						
					g_OfflineAttack.WizardAttack(lpObj->m_Index, TargetIndex, UseBuffEachXTime);			
				}
				break;
				// --
			case CLASS_KNIGHT:
				{						
					g_OfflineAttack.KnigthAttack(lpObj->m_Index, TargetIndex, UseBuffEachXTime);
				}
				break;
				// --
			case CLASS_ELF:
				{
					g_OfflineAttack.ElfAttack(lpObj->m_Index, TargetIndex, UseBuffEachXTime);
				}
				break;
				// --
			case CLASS_MAGUMSA:
				{
					g_OfflineAttack.MagumsaAttack(lpObj->m_Index, TargetIndex);
				}
				break;
				// --
			case CLASS_DARKLORD:
				{
					g_OfflineAttack.LordAttack(lpObj->m_Index, TargetIndex, UseBuffEachXTime);
				}
				break;
				// --
			case CLASS_SUMMONER:
				{
					g_OfflineAttack.SummonerAttack(lpObj->m_Index, TargetIndex, UseBuffEachXTime);
				}
				break;
				// --
			case CLASS_MONK:
				{
					g_OfflineAttack.MonkAttack(lpObj->m_Index, TargetIndex, UseBuffEachXTime);
				}
				break; 
			}
			// ----
			Sleep(g_OfflineAttack.GetAttackDelay(lpObj));
		}
		// ----
		Sleep(1000);
		UseBuffEachXTime += 1000;
	}
	// ----
	_endthread();
}
// -------------------------------------------------------------------------------

void OfflineAttack::WizardAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime)
{
	LPOBJ lpObj			= &gObj[UserIndex];
	LPOBJ lpTargetObj	= &gObj[TargetIndex];
	// ----
	/*if( UseBuffEachXTime > 0 && (UseBuffEachXTime % 60000 == 0) )	
	{
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, 16);
		// ----
		if( lpMagic != NULL )
		{
			switch(lpMagic->m_Skill)
			{
			case 16:
				gObjUseSkill.RunningSkill(UserIndex, TargetIndex, lpMagic, 0);
				this->SetBuffPartyUser(UserIndex, lpMagic);			
				break;
			}
		}
		else 
		{
			for( int n = 0; n < 5; n++ )
			{
				lpMagic = gObjGetMagicSearch(lpObj, i_szWizardBuffMasterSkill[n]);

				if (lpMagic != NULL)
				{
					switch(lpMagic->m_Skill)
					{
					default:
						gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
						break;
					}
				}
			}
		}
	}
	else 
	{*/
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, this->CharInfo[UserIndex].MagicNumber);
		// ----
		if( lpMagic == 0 || !gCheckSkillDistance(UserIndex, lpTargetObj->m_Index, lpMagic->m_Skill) )
		{
			return;
		}
		// ----
		gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
	//}
}
// -------------------------------------------------------------------------------

void OfflineAttack::KnigthAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime)
{
	LPOBJ lpObj			= &gObj[UserIndex];
	LPOBJ lpTargetObj	= &gObj[TargetIndex];
	// ----
	/*if (UseBuffEachXTime > 0 && (UseBuffEachXTime % 60000 == 0))	
	{
		gObjUseSkill.KnightSkillAddLife(UserIndex, 0);

		for (int n = 0; n < 5; n++)
		{
			CMagicInf *lpMagic = gObjGetMagicSearch(lpObj, i_szKnightBuffMasterSkill[n]);

			if (lpMagic != NULL)
			{
				switch (lpMagic->m_Skill)
				{
				default:
					gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
					break;
				}			
			}
		}
	}
	else 
	{*/
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, this->CharInfo[UserIndex].MagicNumber);
		// ----
		if( lpMagic == 0 || !gCheckSkillDistance(UserIndex, lpTargetObj->m_Index, lpMagic->m_Skill) )
		{
			return;
		}
		// ----
		switch(this->CharInfo[UserIndex].MagicNumber)
		{
		case 41:	//-> Twisting Slash
		case 330:	//-> Twisting Slash Strengthener
		case 332:	//-> Twisting Slash Mastery
			{
				gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
			}
			break;
		}
	//}
}
// -------------------------------------------------------------------------------

void OfflineAttack::ElfAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime)
{
	LPOBJ lpObj			= &gObj[UserIndex];
	LPOBJ lpTargetObj	= &gObj[TargetIndex];
	//CMagicInf * lpMagic	= lpObj->Magic;
	// ----
	/*if (UseBuffEachXTime > 0 && (UseBuffEachXTime % 60000 == 0))	
	{
		lpMagic = gObjGetMagicSearch(lpObj, 26);

		if (lpMagic != NULL)
		{
			switch (lpMagic->m_Skill)
			{
			case 26:
				gObjUseSkill.RunningSkill(UserIndex, UserIndex, lpMagic, 0);
				this->SetBuffPartyUser(UserIndex, lpMagic);			
				break;
			}
		}
		else 
		{
			for (int n = 0; n < 5; n++)
			{
				lpMagic = gObjGetMagicSearch(lpObj, i_szElfBuff1MasterSkill[n]);

				if (lpMagic != NULL)
				{
					switch (lpMagic->m_Skill)
					{
					default:
						gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
						break;
					}
				}
			}
		}

		lpMagic = gObjGetMagicSearch(lpObj, 27);

		if (lpMagic != NULL)
		{
			switch (lpMagic->m_Skill)
			{
			case 27:
				gObjUseSkill.RunningSkill(UserIndex, UserIndex, lpMagic, 0);
				this->SetBuffPartyUser(UserIndex, lpMagic);			
				break;
			}
		}
		else 
		{
			for (int n = 0; n < 5; n++)
			{
				lpMagic = gObjGetMagicSearch(lpObj, i_szElfBuff2MasterSkill[n]);

				if (lpMagic != NULL)
				{
					switch (lpMagic->m_Skill)
					{
					default:
						gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
						break;
					}
				}
			}
		}

		lpMagic = gObjGetMagicSearch(lpObj, 28);

		if (lpMagic != NULL)
		{
			switch (lpMagic->m_Skill)
			{
			case 28:
				gObjUseSkill.RunningSkill(UserIndex, TargetIndex, lpMagic, 0);
				this->SetBuffPartyUser(UserIndex, lpMagic);			
				break;
			}
		}
		else 
		{
			for (int n = 0; n < 5; n++)
			{
				lpMagic = gObjGetMagicSearch(lpObj, i_szElfBuff3MasterSkill[n]);

				if (lpMagic != NULL)
				{
					switch (lpMagic->m_Skill)
					{
					default:
						gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
						break;
					}
				}
			}
		}
	}
	else 
	{*/
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, this->CharInfo[UserIndex].MagicNumber);
		// ----
		if( lpMagic == 0 || !gCheckSkillDistance(UserIndex, lpTargetObj->m_Index, lpMagic->m_Skill) )
		{
			return;
		}
		// ----
		switch(this->CharInfo[UserIndex].MagicNumber)
		{
		case 24:
		case 414:
		case 418:
			{
				gObjUseSkill.RunningSkill(UserIndex, lpTargetObj->m_Index, lpMagic, false);
			}
			break;
		}
		
		//gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
	//}
}
// -------------------------------------------------------------------------------

void OfflineAttack::MagumsaAttack(int UserIndex, int TargetIndex)
{
	LPOBJ lpObj			= &gObj[UserIndex];
	LPOBJ lpTargetObj	= &gObj[TargetIndex];
	// ----
	CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, this->CharInfo[UserIndex].MagicNumber);
	// ----
	if( lpMagic == 0 || !gCheckSkillDistance(UserIndex, lpTargetObj->m_Index, lpMagic->m_Skill) )
	{
		return;
	}
	// ----
	gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
}
// -------------------------------------------------------------------------------

void OfflineAttack::LordAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime)
{
	LPOBJ lpObj = &gObj[UserIndex];
	LPOBJ lpTargetObj = &gObj[TargetIndex];

	/*if (UseBuffEachXTime > 0 && (UseBuffEachXTime % 60000 == 0))	
	{
		CMagicInf *lpMagic = gObjGetMagicSearch(lpObj, 64);

		if (lpMagic != NULL)
		{
			switch (lpMagic->m_Skill)
			{
			case 64:
				gObjUseSkill.RunningSkill(UserIndex, TargetIndex, lpMagic, 0);
				this->SetBuffPartyUser(UserIndex, lpMagic);	
				break;
			}
		}
		else 
		{
			for (int n = 0; n < 5; n++)
			{
				lpMagic = gObjGetMagicSearch(lpObj, i_szDarkLordBuffMasterSkill[n]);

				if (lpMagic != NULL)
				{
					switch (lpMagic->m_Skill)
					{
					default:
						gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
						break;
					}
				}
			}
		}
	}
	else 
	{*/
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, this->CharInfo[UserIndex].MagicNumber);
		// ----
		if( lpMagic == 0 || !gCheckSkillDistance(UserIndex, lpTargetObj->m_Index, lpMagic->m_Skill) )
		{
			return;
		}
		// ----
		gObjUseSkill.RunningSkill(lpObj->m_Index, lpTargetObj->m_Index, lpMagic, 0);
		//gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, lpTargetObj->m_Index);
	//}
}
// -------------------------------------------------------------------------------

void OfflineAttack::SummonerAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime)
{
	LPOBJ lpObj			= &gObj[UserIndex];
	LPOBJ lpTargetObj	= &gObj[TargetIndex];
	// ----
	/*if (UseBuffEachXTime > 0 && (UseBuffEachXTime % 60000 == 0))	
	{
		CMagicInf *lpMagic = gObjGetMagicSearch(lpObj, 218);

		if (lpMagic != NULL)
		{
			switch (lpMagic->m_Skill)
			{
			case 218:
				gObjUseSkill.RunningSkill(UserIndex, TargetIndex, lpMagic, 0);
				this->SetBuffPartyUser(UserIndex, lpMagic);		
				break;
			}
		}
	}
	else 
	{*/
		CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, this->CharInfo[UserIndex].MagicNumber);
		// ----
		if( lpMagic == 0 || !gCheckSkillDistance(UserIndex, lpTargetObj->m_Index, lpMagic->m_Skill) )
		{
			return;
		}
		// ----
		gObjUseSkill.UseSkill(UserIndex, lpMagic, lpObj->X, lpObj->Y, lpObj->Dir, 0, 65535);
		gObjUseSkill.RunningSkill(UserIndex, TargetIndex, lpMagic, 0);
	//}
}
// -------------------------------------------------------------------------------

void OfflineAttack::MonkAttack(int UserIndex, int TargetIndex, int UseBuffEachXTime)
{

}
// -------------------------------------------------------------------------------

bool OfflineAttack::SetBuffPartyUser(int UserIndex, CMagicInf *lpMagic)
{
	if( gParty.GetPartyCount(gObj[UserIndex].PartyNumber) < 0 ) 
	{
		return false;
	}
	// ----
	for(int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		int PartyNumIndex = gParty.m_PartyS[gObj[i].PartyNumber].Number[i];
		// ----
		if( PartyNumIndex >= 0 )
		{
			gObjUseSkill.RunningSkill(PartyNumIndex, PartyNumIndex, lpMagic, 0);		
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------