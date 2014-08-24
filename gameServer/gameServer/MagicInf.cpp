//-> Revised by DarkSim | 18.06.2013 | 1.01.00 GS-N
// -------------------------------------------------------------------------------

#include "stdafx.h"
#include "MagicInf.h"
#include "LogProc.h"
#include "GameMain.h"
#include "MasterLevelSkillTreeSystem.h"
// -------------------------------------------------------------------------------

CMagicInf	DefMagicInf[MAX_MAGICINF];
// -------------------------------------------------------------------------------

//004c5a60	-> 100% (Identical)
CMagicInf::CMagicInf()
{
	this->Clear();
}
// -------------------------------------------------------------------------------

//004c5ae0	-> 100% (Identical)
CMagicInf::~CMagicInf()
{
	this->m_DamageMin  = 0;
	this->m_DamageMax  = 0;
}
// -------------------------------------------------------------------------------

//004c5b20	-> 100% (Identical)
void CMagicInf::Clear()
{
	this->m_Skill	= 0xFF;
	this->m_Level	= 0;
	this->m_bPass	= 0;
}
// -------------------------------------------------------------------------------

//004c5b60	-> 100% (Identical)
BOOL CMagicInf::IsMagic()
{
	if( this->m_Skill == 0xFF )
	{
		return false;
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

//004c5ba0	-> 100% (Identical)
int GetSkillNumberInex(int type, int Index, int level)
{
	int skillnumber = -1;
	// ----
	if( type == 15 )
	{
		if( Index >= 0 && Index < 16 )
		{
			skillnumber = Index + 1;
		}
		else if( Index == 16 )
		{
			skillnumber = 38;
		}
		else if( Index == 17 )
		{
			skillnumber = 39;
		}
		else if( Index == 18 )
		{
			skillnumber = 40;
		}
		else if( Index == 19 )
		{
			skillnumber = 215;
		}
		else if( Index == 20 )
		{
			skillnumber = 214;
		}
		else if( Index == 21 )
		{
			skillnumber = 230;
		}
		else if( Index == 22 )
		{
			skillnumber = 217;
		}
		else if( Index == 23 )
		{
			skillnumber = 218;
		}
		else if( Index == 24 )
		{
			skillnumber = 219;
		}
		else if( Index == 25 )
		{
			skillnumber = 220;
		}
		else if( Index == 26 )
		{
			skillnumber = 221;
		}
		else if( Index == 27 )
		{
			skillnumber = 222;
		}
		else if( Index == 28 )
		{
			skillnumber = 233;
		}
		else if( Index == 29 )
		{
			skillnumber = 237;
		}
		else if( Index == 30 )
		{
			skillnumber = 262;
		}
		else if( Index == 31 )
		{
			skillnumber = 263;
		}
		else if( Index == 32 )
		{
			skillnumber = 264;
		}
		else if( Index == 33 )
		{
			skillnumber = 265;
		}
		else if( Index == 34 )
		{
			skillnumber = 266;
		}
		else if( Index == 35 )
		{
			skillnumber = 267;
		}
		else if( Index == 36 )
		{
			skillnumber = 268;
		}
	}
	else if( type == 12 )
	{
		switch(Index)
		{
		case 7:		
			skillnumber = 41;	
			break;
		case 8:		
			skillnumber = 26;	
			break;
		case 9:		
			skillnumber = 27;	
			break;
		case 10:	
			skillnumber = 28;	
			break;
		case 12:	
			skillnumber = 42;	
			break;
		case 13:	
			skillnumber = 47;	
			break;
		case 14:	
			skillnumber = 48;	
			break;
		case 19:	
			skillnumber = 43;	
			break;
		case 17:	
			skillnumber = 52;	
			break;
		case 18:	
			skillnumber = 51;	
			break;
		case 16:	
			skillnumber = 55;	
			break;
		case 21:	
			skillnumber = 61;	
			break;
		case 22:	
			skillnumber = 63;	
			break;
		case 23:	
			skillnumber = 64;	
			break;
		case 24:	
			skillnumber = 65;	
			break;
		case 11:	
			skillnumber = level + 30;	
			break;
		case 35:	
			skillnumber = 78;	
			break;
		case 44: 
			skillnumber = 232; 
			break;
		case 45: 
			skillnumber = 235; 
			break;
		case 46: 
			skillnumber = 234; 
			break;
		case 47: 
			skillnumber = 236; 
			break;
		case 48: 
			skillnumber = 238; 
			break;
		}
	}
	// ----
	return skillnumber;
}
// -------------------------------------------------------------------------------

//004c5fb0	-> 100%	(Identical)
int CMagicInf::Set(BYTE aType, BYTE aIndex, BYTE aLevel)
{
	if( this->m_Skill 
		!= 0xFF )
	{
		LogAdd(lMsg.Get(MSGGET(1, 203)), 
			__FILE__, 
			__LINE__);	//Line: 6
		return -1;
	}
	// ----
	this->m_Level = aLevel;
	this->m_Skill = GetSkillNumberInex(aType, aIndex, aLevel);
	// ----
	if( this->m_Skill < 0 )
	{
		this->m_Skill = 1;
	}
	// ----
	int damage			= MagicDamageC.SkillGet(this->m_Skill);
	this->m_DamageMin	= damage;
	this->m_DamageMax	= damage + damage/2;
	// ----
	return this->m_Skill;
}
// -------------------------------------------------------------------------------

//004c60b0	-> 100% (Identical)
int CMagicInf::GetDamage()
{
	int damage = 0;
	// ----
	if( this->m_Skill == 0xFF )
	{
		LogAdd(lMsg.Get(MSGGET(1, 202)), 
			__FILE__, 
			__LINE__);	//Line: 7
		return 0;
	}
	// ----
	int subd = this->m_DamageMax - this->m_DamageMin;
	// ----
	__try
	{
		if( subd < 0 )
		{
			subd = 0;
		}
		// ----
		damage = this->m_DamageMin + rand() % (subd + 1);
	}
	// ----
	__except(subd = 1, 1)
	{
		LogAdd(lMsg.Get(MSGGET(0, 111)),
			__FILE__, 
			__LINE__);	//Line: 27
	}
	// ----
	return damage;
}
// -------------------------------------------------------------------------------

//004c6200	-> 100%
int CMagicInf::Set(int aSkill, BYTE aLevel)
{
	int damage = 1;
	// ----
	if( this->m_Skill != 0xFF )
	{
		return -1;
	}
	// ----
	this->m_Skill = aSkill;
	this->m_Level = aLevel;
	// ----
	if( g_MasterSkillSystem.CheckMasterLevelSkill(aSkill) )
	{
		if( MagicDamageC.GetSkillUseType(aSkill) == 4 )
		{
			int nBrandSkill = MagicDamageC.GetNormalBrandOfSkill(aSkill);
			// ----
			if( nBrandSkill > 0 && nBrandSkill != aSkill )
			{
				damage = MagicDamageC.SkillGet(nBrandSkill);
			}
			else
			{
				float fValue = g_MasterSkillSystem.GetMasterSkillValue(MagicDamageC.SkillGet(this->m_Skill), this->m_Level);
				// ----
				if( fValue == 0.0f )
				{
					damage = 1;
				}
				else
				{
					damage = (INT64)fValue;
				}
			}
		}
		else
		{
			float fValue = g_MasterSkillSystem.GetMasterSkillValue(MagicDamageC.SkillGet(this->m_Skill), this->m_Level);
			// ----
			if( fValue == 0.0f )
			{
				damage = 1;
			}
			else
			{
				damage = (INT64)fValue;
			}
		}
	}
	else
	{
		damage = MagicDamageC.SkillGet(this->m_Skill);
	}
	// ----
	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage / 2;
	// ----
	return this->m_Skill;
}
// -------------------------------------------------------------------------------

//004c63e0	-> 100%
int CMagicInf::UpdateMasterSkill(int iSkill, BYTE btLevel)
{
	int damage = 1;
	// ----
	if( this->m_Skill != 0xFF )
	{
		this->m_Skill = iSkill;
		this->m_Level = btLevel;
		// ----
		if( MagicDamageC.GetSkillUseType(this->m_Skill) == 4 )
		{
			int nBrandSkill = MagicDamageC.GetNormalBrandOfSkill(this->m_Skill);
			// ----
			if( nBrandSkill > 0 && nBrandSkill != this->m_Skill )
			{
				damage = MagicDamageC.SkillGet(nBrandSkill);
			}
			else
			{
				float fValue = g_MasterSkillSystem.GetMasterSkillValue(MagicDamageC.SkillGet(this->m_Skill), this->m_Level);
				// ----
				if( fValue == 0.0f )
				{
					damage = 1;
				}
				else
				{
					damage = (INT64)fValue;
				}
			}
		}
		else
		{
			float fValue = g_MasterSkillSystem.GetMasterSkillValue(MagicDamageC.SkillGet(this->m_Skill), this->m_Level);
			// ----
			if( fValue == 0.0f )
			{
				damage = 1;
			}
			else
			{
				damage = (INT64)fValue;
			}
		}
		// ----
		this->m_DamageMin = damage;
		this->m_DamageMax = damage + damage / 2;
		// ----
		return this->m_Skill;
	}
	// ----
	LogAdd("[MasterSkill] [ERROR!!] Fail - Update Master Skill:%d, Level:%d", iSkill, btLevel);
	return -1;
}
// -------------------------------------------------------------------------------

//004c65b0	-> 100% (Identical)
void MagicByteConvert(LPBYTE buf, CMagicInf * const Magici, int maxmagic)
{
	int n = 0;
	int iExpendSkillCount, iExpendSkillNumber, iExpendSkillExtraNumber;
	// ----
	for( int index = 0; index < maxmagic; index++ )
	{
		iExpendSkillNumber		= Magici[index].m_Skill;
		iExpendSkillCount		= 0;
		iExpendSkillExtraNumber = 0;
		// ----
		if( Magici[index].m_Skill > 765)
		{
			iExpendSkillNumber		= 0xFF;
			iExpendSkillCount		= 3;
			iExpendSkillExtraNumber = Magici[index].m_Skill - (iExpendSkillNumber * 3);
		}
		else if( Magici[index].m_Skill > 510 )
		{
			iExpendSkillNumber		= 0xFF;
			iExpendSkillCount		= 2;
			iExpendSkillExtraNumber = Magici[index].m_Skill - (iExpendSkillNumber << 1);
		}
		else if( Magici[index].m_Skill > 255 )
		{
			iExpendSkillNumber		= 0xFF;
			iExpendSkillCount		= 1;
			iExpendSkillExtraNumber = Magici[index].m_Skill - iExpendSkillNumber;
		}
		// ----
		buf[n] = iExpendSkillNumber;
		n++;
		// ----
		buf[n] = Magici[index].m_Level << 3;
		buf[n] |= iExpendSkillCount & 0x07;
		n++;
		// ----
		buf[n] = iExpendSkillExtraNumber;
		n++;
	}
}
// -------------------------------------------------------------------------------