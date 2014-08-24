// GS-CS	1.00.90		0xXXXXXXXX	- finished
#include "stdafx.h"
#include "MonsterAttr.h"
#include "LogProc.h"
#include "..\include\readscript.h"
#include "..\common\WzMemScript.h"
#include "user.h"
#include "GameMain.h"

//004260D0 - identical
CMonsterAttr::CMonsterAttr()
{
	return;
}

//00426150 - identical
CMonsterAttr::~CMonsterAttr()
{
	return;
}

//00426180 - identical
LPMONSTER_ATTRIBUTE CMonsterAttr::GetAttr(int aClass)
{
	for ( int n = 0; n<MAX_MONSTER_TYPE;n++)
	{
		if ( this->m_MonsterAttr[n].m_Index == aClass )
		{
			return &this->m_MonsterAttr[n];
		}
	}

	return NULL;
}

//004261F0 - identical (new func) calls - 1x (from GMMng.cpp)
LPMONSTER_ATTRIBUTE CMonsterAttr::GetAttr(char * zsName)
{
	for ( int n = 0; n<MAX_MONSTER_TYPE;n++)
	{
		if ( !strcmp(this->m_MonsterAttr[n].m_Name,zsName) )
		{
			return &this->m_MonsterAttr[n];
		}
	}

	return NULL;
}

//00426270 - identical
void CMonsterAttr::LoadAttr(char* filename) // Not in use
{
	int Token;	
	int n;
	// ----
	SMDFile = fopen(filename, "r");	//ok
	// ----
	if ( SMDFile == NULL )
	{
		MsgBox("load error %s", filename);
		return;
	}
	// ----
	n = 0;
	// ----
	while ( true )
	{
		Token = GetToken();
		// ----
		if( Token == END )
		{
			break;
		}
		// ----
		if( Token == 1 )
		{
			this->m_MonsterAttr[n].m_Index															= TokenNumber;
			// ----
			Token = GetToken();					this->m_MonsterAttr[n].m_Rate						= TokenNumber;
			Token = GetToken();					strcpy(this->m_MonsterAttr[n].m_Name, TokenString);
			Token = GetToken();					this->m_MonsterAttr[n].m_Level						= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_Hp							= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_Mp							= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_DamageMin					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_DamageMax					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_Defense					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_MagicDefense				= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_AttackRating				= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_Successfulblocking			= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_MoveRange					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_AttackType					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_AttackRange				= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_ViewRange					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_MoveSpeed					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_AttackSpeed				= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_RegenTime					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_Attribute					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_ItemRate					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_MoneyRate					= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_MaxItemLevel				= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_MonsterSkill				= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_Resistance[R_ICE]			= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_Resistance[R_POISON]		= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_Resistance[R_LIGHTNING]	= TokenNumber;
			Token = GetToken();					this->m_MonsterAttr[n].m_Resistance[R_FIRE]			= TokenNumber;
			// ----
			if( gMonsterAttackRateIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_AttackRating	+= this->m_MonsterAttr[n].m_AttackRating * gMonsterAttackRateIncrease / 100;
			}
			// --
			if( gMonsterDamageMinIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_DamageMin		+= this->m_MonsterAttr[n].m_DamageMin * gMonsterDamageMinIncrease / 100;
			}
			// --
			if( gMonsterDamageMaxIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_DamageMax		+= this->m_MonsterAttr[n].m_DamageMax * gMonsterDamageMaxIncrease / 100;
			}
			// --
			if( gMonsterDefenseIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_Defense		+= this->m_MonsterAttr[n].m_Defense * gMonsterDefenseIncrease / 100;
			}
			// --
			if( gMonsterLifeIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_Hp				+= this->m_MonsterAttr[n].m_Hp * gMonsterLifeIncrease / 100;
			}
			// Original
			/*if ( this->m_MonsterAttr[n].m_Level <= 24 && this->m_MonsterAttr[n].m_Index < 50)
			{
				if ( this->m_MonsterAttr[n].m_Level == 13 ||
					 this->m_MonsterAttr[n].m_Level == 14 ||
					 this->m_MonsterAttr[n].m_Level == 17 ||
					 this->m_MonsterAttr[n].m_Level == 18 ||
					 this->m_MonsterAttr[n].m_Level == 19 ||
					 this->m_MonsterAttr[n].m_Level == 24 )
				{
					int modiryvalue = this->m_MonsterAttr[n].m_Hp*20/100;
					modiryvalue-= modiryvalue*gMonsterHPAdjust/100;
					this->m_MonsterAttr[n].m_Hp -= modiryvalue;
				}
				else if ( this->m_MonsterAttr[n].m_Level == 20 ||
						  this->m_MonsterAttr[n].m_Level == 22 )
				{
					int modiryvalue = this->m_MonsterAttr[n].m_Hp*30/100;
					modiryvalue-= modiryvalue*gMonsterHPAdjust/100;
					this->m_MonsterAttr[n].m_Hp -= modiryvalue;
				}
				else
				{
					int modiryvalue = this->m_MonsterAttr[n].m_Hp/2;
					modiryvalue-= modiryvalue*gMonsterHPAdjust/100;
					this->m_MonsterAttr[n].m_Hp -= modiryvalue;
				}
			}

			if ( this->m_MonsterAttr[n].m_Level  <= 19 && this->m_MonsterAttr[n].m_Index  < 50 )
			{
				if ( this->m_MonsterAttr[n].m_Level == 13 ||
					 this->m_MonsterAttr[n].m_Level == 14 ||
					 this->m_MonsterAttr[n].m_Level == 17 ||
					 this->m_MonsterAttr[n].m_Level == 18 ||
					 this->m_MonsterAttr[n].m_Level == 19 )
				 {
					int modiryvaluemin = this->m_MonsterAttr[n].m_DamageMin*30/100;
					int modiryvaluemax = this->m_MonsterAttr[n].m_DamageMax*30/100;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					this->m_MonsterAttr[n].m_DamageMin -= modiryvaluemin;
					this->m_MonsterAttr[n].m_DamageMax -= modiryvaluemax;
				}
				else
				{
					int modiryvaluemin = this->m_MonsterAttr[n].m_DamageMin/2;
					int modiryvaluemax = this->m_MonsterAttr[n].m_DamageMax/2;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					this->m_MonsterAttr[n].m_DamageMin -= modiryvaluemin;
					this->m_MonsterAttr[n].m_DamageMax -= modiryvaluemax;
				}
			}
			this->m_MonsterAttr[n].m_Hp = this->m_MonsterAttr[n].m_Hp - (INT)(this->m_MonsterAttr[n].m_Hp / 100.0f * gMonsterHp);*/

			this->m_MonsterAttr[n].m_iScriptHP = this->m_MonsterAttr[n].m_Hp;

			n++;
			if ( n > MAX_MONSTER_TYPE-1 )
			{
				MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
				fclose(SMDFile);
				return;
			}
		}
	}

	fclose(SMDFile);
}

//004270D0 - identical
void CMonsterAttr::LoadAttr(char* Buffer, int iSize)
{
	CWzMemScript WzMemScript;


	WzMemScript.SetBuffer(Buffer, iSize);

	int Token;	
	int n=0;
	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == END )
			break;

		if ( Token == 1 )
		{
			this->m_MonsterAttr[n].m_Index = WzMemScript.GetNumber();
			// ----
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Rate = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			strcpy(this->m_MonsterAttr[n].m_Name, WzMemScript.GetString());
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Level = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Hp = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Mp = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_DamageMin = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_DamageMax = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Defense = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_MagicDefense = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_AttackRating = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Successfulblocking = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_MoveRange = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_AttackType = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_AttackRange = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_ViewRange = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_MoveSpeed = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_AttackSpeed = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_RegenTime = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Attribute = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_ItemRate = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_MoneyRate = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_MaxItemLevel = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_MonsterSkill = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Resistance[R_ICE]  = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Resistance[R_POISON] = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Resistance[R_LIGHTNING] = WzMemScript.GetNumber();
			Token = WzMemScript.GetToken();			this->m_MonsterAttr[n].m_Resistance[R_FIRE] = WzMemScript.GetNumber();
			// ----
			if( gMonsterAttackRateIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_AttackRating	+= this->m_MonsterAttr[n].m_AttackRating * gMonsterAttackRateIncrease / 100;
			}
			// --
			if( gMonsterDamageMinIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_DamageMin		+= this->m_MonsterAttr[n].m_DamageMin * gMonsterDamageMinIncrease / 100;
			}
			// --
			if( gMonsterDamageMaxIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_DamageMax		+= this->m_MonsterAttr[n].m_DamageMax * gMonsterDamageMaxIncrease / 100;
			}
			// --
			if( gMonsterDefenseIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_Defense		+= this->m_MonsterAttr[n].m_Defense * gMonsterDefenseIncrease / 100;
			}
			// --
			if( gMonsterLifeIncrease > 0 )
			{
				this->m_MonsterAttr[n].m_Hp				+= this->m_MonsterAttr[n].m_Hp * gMonsterLifeIncrease / 100;
			}
			// Original
			/*if ( this->m_MonsterAttr[n].m_Level <= 24 && this->m_MonsterAttr[n].m_Index < 50)
			{
				if ( this->m_MonsterAttr[n].m_Level == 13 ||
					 this->m_MonsterAttr[n].m_Level == 14 ||
					 this->m_MonsterAttr[n].m_Level == 17 ||
					 this->m_MonsterAttr[n].m_Level == 18 ||
					 this->m_MonsterAttr[n].m_Level == 19 ||
					 this->m_MonsterAttr[n].m_Level == 24 )
				{
					int modiryvalue = this->m_MonsterAttr[n].m_Hp*20/100;
					modiryvalue-= modiryvalue*gMonsterHPAdjust/100;
					this->m_MonsterAttr[n].m_Hp -= modiryvalue;
				}
				else if ( this->m_MonsterAttr[n].m_Level == 20 ||
						  this->m_MonsterAttr[n].m_Level == 22 )
				{
					int modiryvalue = this->m_MonsterAttr[n].m_Hp*30/100;
					modiryvalue-= modiryvalue*gMonsterHPAdjust/100;
					this->m_MonsterAttr[n].m_Hp -= modiryvalue;
				}
				else
				{
					int modiryvalue = this->m_MonsterAttr[n].m_Hp/2;
					modiryvalue-= modiryvalue*gMonsterHPAdjust/100;
					this->m_MonsterAttr[n].m_Hp -= modiryvalue;
				}
			}

			if ( this->m_MonsterAttr[n].m_Level  <= 19 && this->m_MonsterAttr[n].m_Index  < 50 )
			{
				if ( this->m_MonsterAttr[n].m_Level == 13 ||
					 this->m_MonsterAttr[n].m_Level == 14 ||
					 this->m_MonsterAttr[n].m_Level == 17 ||
					 this->m_MonsterAttr[n].m_Level == 18 ||
					 this->m_MonsterAttr[n].m_Level == 19 )
				 {
					int modiryvaluemin = this->m_MonsterAttr[n].m_DamageMin*30/100;
					int modiryvaluemax = this->m_MonsterAttr[n].m_DamageMax*30/100;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					this->m_MonsterAttr[n].m_DamageMin -= modiryvaluemin;
					this->m_MonsterAttr[n].m_DamageMax -= modiryvaluemax;
				}
				else
				{
					int modiryvaluemin = this->m_MonsterAttr[n].m_DamageMin/2;
					int modiryvaluemax = this->m_MonsterAttr[n].m_DamageMax/2;

					modiryvaluemin -= modiryvaluemin*gMonsterHPAdjust/100;
					modiryvaluemax -= modiryvaluemax*gMonsterHPAdjust/100;

					this->m_MonsterAttr[n].m_DamageMin -= modiryvaluemin;
					this->m_MonsterAttr[n].m_DamageMax -= modiryvaluemax;
				}
			}
			this->m_MonsterAttr[n].m_Hp = this->m_MonsterAttr[n].m_Hp - (INT)(this->m_MonsterAttr[n].m_Hp / 100.0f * gMonsterHp);*/

			this->m_MonsterAttr[n].m_iScriptHP = this->m_MonsterAttr[n].m_Hp;

			n++;

			if ( n> MAX_MONSTER_TYPE-1 )
			{
				MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
				fclose(SMDFile);

				return;
			}
		}
	}
}


