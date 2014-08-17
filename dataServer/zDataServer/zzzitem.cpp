#include "stdafx.h"
#include <math.h>
#include "Define.h"
#include "ItemDef.h"
#include "ReadScript.h"
#include "ZzzItem.h"
#include "SetItemOption.h"
#include "ProDef.h"
#include "winutil.h"

#pragma warning ( disable : 4244 )

ITEM_ATTRIBUTE      ItemAttribute [MAX_ITEM];
CPetItemExp gPetItemExp;
int g_MaxItemIndexOfEachItemType[MAX_ITEM_INDEX];

CPetItemExp::CPetItemExp()
{
	m_DarkSpiritExpTable[0] = 0;
	m_DarkSpiritExpTable[1] = 0;

	for ( int i = 2 ; i < 52 ; i++ )
	{
		m_DarkSpiritExpTable[i] = (i+10) * i * i * i * 100;
	}

	m_DarkHorseExpTable[0] = 0;
	m_DarkHorseExpTable[1] = 0;

	for ( int i = 2 ; i < 52 ; i++ )
	{
		m_DarkHorseExpTable[i] = (i+10) * i * i * i * 100;
	}
}

//----------------------------------------------------------------------------
// 아이템 정보를 초기화 한다.
CItem::CItem()
{
	Clear();
}

void CItem::Clear()
{
	m_Type			= -1;
	m_Level			= 0;
	m_Part			= 0;
	m_Class			= 0;
	m_TwoHand		= 0;
	m_AttackSpeed	= 0;
	m_DamageMin		= 0;
	m_DamageMax		= 0;
	m_SuccessfulBlocking = 0;
	m_Defense		= 0;
	m_MagicDefense	= 0;
	m_Durability	= 0;
	m_SpecialNum	= 0;	
	m_Value			= 0;	
	m_Option1		= 0;
	m_Option2		= 0;
	m_Option3		= 0;
	m_NewOption		= 0;
	m_Number		= 0;

	m_DurabilitySmall			= 0;
	m_iPShopValue				= -1;
	m_CurrentDurabilityState	= -1;
	m_SetOption		= 0;
	m_QuestItem		= 0;

	memset(m_Special,      0, MAX_ITEM_SPECIAL);
	memset(m_SpecialValue, 0, MAX_ITEM_SPECIAL);

	memset(m_Resistance, 0, MAX_RESISTANCE);

	m_IsLoadPetItemInfo			= 0;
	m_PetItem_Level	= 1;
	m_PetItem_Exp	= 0;
	m_Leadership	= 0;
}

//----------------------------------------------------------------------------
// 아이템이 존재하는가?
BOOL CItem::IsItem()
{
	if( m_Type < 0 ) return FALSE;
	
	return TRUE;
}

//----------------------------------------------------------------------------
//
int CItem::IsSetItem()
{
	if( m_Type < 0 ) return FALSE;
	
	return m_SetOption & 0x03;
}

//----------------------------------------------------------------------------
// 기본속성을 기반으로 아이템을 만든다.
void CItem::Convert( int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE Attribute2, BYTE SetOption, BYTE DbVersion )
{
	int _type=type;

	int ItemLevel; //3
	
	if ( (DbVersion ) == 0)
	{
		_type=(type/MAX_ITEM_TYPE*MAX_ITEM_INDEX)+(type%MAX_ITEM_TYPE);

	}
	else if ( DbVersion <= 2 )
	{
		_type = (type/32*MAX_ITEM_INDEX)+(type%32);
	}
	
	if ( _type >MAX_ITEM-1)
	{
		gWindow.PrintLog("error-L1 : ItemIndex error %d", _type);
	}

	ITEM_ATTRIBUTE *p = &ItemAttribute[_type];
	
	m_serial			 = ItemAttribute[_type].Serial;
	m_Type               = type;

	if(DbVersion <= 2)
	{
		m_Type=_type;
	}

	m_NewOption = Attribute2;
	
	if(p->OptionFlag == 0)
	{
		m_NewOption = 0;
	}

	if( ( (_type >= ITEM_WING+3 ) && (_type <= ITEM_WING+6) ) || (_type == ITEM_HELPER+30) )
	{
		Attribute2=0;
	}

	if ( _type == ITEM_HELPER+37 )
	{
		Attribute2 = 0;
	}

	SetOption = SetOption & 15;
		
	int SOption = SetOption & 3;
	int SOptionStatValue = 0;

	if (SOption != 1 && SOption != 2)
	{
		SOption=0;
	}

	if ( (( (SetOption>>2) & 3) != 1) && ( ((SetOption>>2) & 3) != 2) )
	{
		SOption=0;
	}

	if ( gSetItemOption.IsSetItem(_type) != 0 && SetOption != 255 && SOption != 0)
	{
		m_SetOption = SetOption;
		if ( m_SetOption != 0 )
		{
			m_NewOption = 0;
			Attribute2 = 0;
			SOptionStatValue = (SetOption>>2)&3;
		}
	}
	else
	{
		m_SetOption = 0;
	}

	memset( m_SkillResistance , 0, sizeof(m_SkillResistance) );
	
	if( _type == ITEM_SWORD+19 || 
		_type == ITEM_BOW+18 || 
		_type == ITEM_STAFF+10 || 
		_type == ITEM_MACE+13)
	{
		Attribute2 = 0;
	}

	int iChaosItem = 0;

	if ( m_Type == ITEM_MACE+6 )
	{
		iChaosItem = 15;
	}
	else if ( m_Type == ITEM_STAFF+7 )
	{
		iChaosItem = 25;
	}
	else if ( m_Type == ITEM_BOW+6 )
	{
		iChaosItem = 30;
	}

	if ( ItemGetDurability(m_Type, 0, 0, 0) != 0 )
	{
		m_BaseDurability = (float)ItemGetDurability(m_Type, m_Level, Attribute2 & 0x7F, m_SetOption );
	}
	else
	{
		m_BaseDurability = 0;
	}

	m_DurabilityState[0] = m_BaseDurability * 0.5f;
	m_DurabilityState[1] = m_BaseDurability * 0.3f;
	m_DurabilityState[2] = m_BaseDurability * 0.2f;

	m_Value				 = p->Value;
	m_AttackSpeed        = p->AttackSpeed;	
	m_TwoHand			 = p->TwoHand;
    m_DamageMin          = p->DamageMin;
	m_DamageMax			 = p->DamageMax;
	m_SuccessfulBlocking = p->SuccessfulBlocking;
	m_Defense            = p->Defense;
	m_MagicDefense       = p->MagicDefense;
	m_WalkSpeed          = p->WalkSpeed;

	m_Magic = p->MagicPW;

	m_Durability         = p->Durability;
	m_serial			 = p->Serial;

	m_QuestItem = (bool)p->QuestItem;
	m_RequireLeaderShip = 0;

	if ( m_Durability == 0 )
	{
		m_CurrentDurabilityState = 1;
	}
	else if ( m_Durability < m_DurabilityState[2] )
	{
		m_CurrentDurabilityState = (float)0.5;
	}
	else if ( m_Durability < m_DurabilityState[1] )
	{
		m_CurrentDurabilityState = (float)0.3;
	}
	else if ( m_Durability < m_DurabilityState[0] )
	{
		m_CurrentDurabilityState = (float)0.2;
	}
	else
	{
		m_CurrentDurabilityState = 0.0;
	}

	memcpy(m_RequireClass, p->RequireClass, MAX_CLASSTYPE);

	for (int i = 0; i<MAX_RESISTANCE ; i++ ) //9
	{
		m_Resistance[i] = p->Resistance[i] * m_Level;
	}

	//memcpy(m_Resistance,   p->Resistance,   MAX_RESISTANCE);

	ItemLevel = p->Level;

	if ( ( Attribute2 & 0x3F) > 0 )
	{
		ItemLevel = p->Level + 25;
	}
	else if (m_SetOption != 0 )
	{
		ItemLevel = p->Level + 25;
	}

	if(p->RequireStrength)
		m_RequireStrength  = 20+p->RequireStrength *(ItemLevel+m_Level*3)*3/100;
	else
		m_RequireStrength  = 0;
	if(p->RequireDexterity)
		m_RequireDexterity = 20+p->RequireDexterity*(ItemLevel+m_Level*3)*3/100;
	else
		m_RequireDexterity = 0;
	if(p->RequireEnergy)
		m_RequireEnergy    = 20+p->RequireEnergy   *(ItemLevel+m_Level*3)*4/100;
	else
		m_RequireEnergy    = 0;


	if ( _type >= ITEM_SWORD && _type < ITEM_WING )
	{
		if ( p->RequireVitality != 0 )
		{
			m_RequireVitality = ((p->RequireVitality * (ItemLevel + m_Level * 3 ) ) * 3) / 100 + 20;
		}
		else
		{
			m_RequireVitality = 0;
		}

		if ( p->RequireLeadership != 0 )
		{
			m_RequireLeaderShip = ((p->RequireVitality * (ItemLevel + m_Level * 3 ) ) * 3) / 100 + 20;
		}
		else
		{
			m_RequireLeaderShip = 0;
		}
	}

	if ( _type == ITEM_HELPER+5 )
	{
		m_RequireLeaderShip = m_PetItem_Level * 15 + 185;
	}

	if(p->RequireLevel)
	{
		if ( _type == ITEM_HELPER+4 )
		{
			m_RequireLevel = m_PetItem_Level * 2 + 218;
		}
		else if ( _type >= ITEM_WING+3 && _type <= ITEM_WING+6 )
		{
			m_RequireLevel = p->RequireLevel + (m_Level * 5);
		}
		else if ( _type >= ITEM_WING+7 && _type <= ITEM_WING+24 )
		{
			m_RequireLevel = p->RequireLevel;
		}
		else if ( _type >= ITEM_SWORD && _type < ITEM_WING )
		{
			if ( p->RequireLevel )
				m_RequireLevel = p->RequireLevel;
		}
		else
			m_RequireLevel = p->RequireLevel+m_Level*4;//반지,목걸이;
	}
	else
		m_RequireLevel = 0;

	if ( m_Type == ITEM_HELPER+10 )
	{
		if ( ItemLevel <= 2 )
		{
			m_RequireLevel = 20;
		}
		else
		{
			m_RequireLevel = 50;
		}
	}

	if ( (Attribute2 & 0x3F) > 0 )
	{
		if ( m_RequireLevel > 0 )
		{
			if ( _type < ITEM_SWORD || _type >= ITEM_WING )
				m_RequireLevel += 20;
		}
	}

	if ( m_SetOption != 0 )
	{
		ItemLevel = p->Level + 30;
	}
	
	m_Leadership = 0;

	if ( m_DamageMax > 0 )
	{
		if ( m_SetOption != 0 && ItemLevel != 0 )
		{
			m_DamageMax += (m_DamageMin * 25) / p->Level + 5;
			m_DamageMax += ItemLevel / 40 + 5;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 )
				{
					m_DamageMax += iChaosItem;
				}
				else if ( p->Level != 0 )
				{
					m_DamageMax += (m_DamageMin * 25) / p->Level + 5;
				}
			}
		}

		m_DamageMax += m_Level * 3;

		if ( m_Level >= 10 )
		{
			m_DamageMax += (m_Level - 9) * (m_Level - 8) / 2;
		}
	}

	if ( m_DamageMin > 0 )	
	{
		if ( m_SetOption != 0 && ItemLevel != 0 )
		{
			m_DamageMin += (m_DamageMin * 25) / p->Level + 5;
			m_DamageMin += ItemLevel / 40 + 5;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 )
				{
					m_DamageMin += iChaosItem;
				}
				else if ( p->Level != 0 )
				{
					m_DamageMin += (m_DamageMin * 25) / p->Level + 5;
				}
			}
		}

		m_DamageMin += m_Level * 3;

		if ( m_Level >= 10 )
		{
			m_DamageMin += (m_Level - 9) * (m_Level - 8) / 2;
		}
	}

	if ( m_Magic > 0 )	
	{
		if ( m_SetOption != 0 && ItemLevel != 0 )
		{
			m_Magic += (m_Magic * 25) / p->Level + 5;
			m_Magic += ItemLevel / 60 + 2;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( iChaosItem != 0 )
				{
					m_Magic += iChaosItem;
				}
				else if ( p->Level != 0 )
				{
					m_Magic += (m_Magic * 25) / p->Level + 5;
				}
			}
		}

		m_Magic += m_Level * 3;

		if ( m_Level >= 10 )
		{
			m_Magic += (m_Level - 9) * (m_Level - 8) / 2;
		}
	}

	if ( p->SuccessfulBlocking > 0 )	
	{
		if ( m_SetOption != 0 && ItemLevel != 0 )
		{
			m_SuccessfulBlocking += (m_SuccessfulBlocking * 25) / p->Level + 5;
			m_SuccessfulBlocking += ItemLevel / 40 + 5;
		}
		else
		{
			if ( (Attribute2 & 0x3F)  > 0 )
			{
				if ( p->Level != 0 )
				{
					m_SuccessfulBlocking += (m_SuccessfulBlocking * 25) / p->Level + 5;
				}
			}
		}

		m_SuccessfulBlocking += m_Level * 3;

		if ( m_Level >= 10 )
		{
			m_SuccessfulBlocking += (m_Level - 9) * (m_Level - 8) / 2;
		}
	}

	if ( p->Defense > 0 )	
	{
		if ( m_Type >= ITEM_SHIELD && m_Type < ITEM_HELM )
		{
			m_Defense += m_Level;

			if ( m_SetOption != 0 && ItemLevel != 0 )
			{
				m_Defense += (m_Defense * 20) / ItemLevel + 2;
			}
		}
		else 
		{
			if ( m_SetOption != 0 && ItemLevel != 0 )
			{
				m_Defense += (m_Defense * 12) / p->Level + (p->Level / 5 ) + 4;
				m_Defense += (m_Defense * 3) / ItemLevel  + ( ItemLevel / 30 ) + 2;
			}
			else if ( (Attribute2 & 0x3F) > 0 )
			{
				if ( p->Level != 0 )
				{
					m_Defense += (m_Defense * 12) / p->Level + (p->Level / 5 ) + 4;
				}
			}
			
			if ( ( _type >= ITEM_WING+3 && _type <= ITEM_WING+6) || _type == ITEM_HELPER+30 || _type == ITEM_HELPER+4 )
			{
				m_Defense += m_Level * 2;
			}
			else
			{
				m_Defense += m_Level * 3;
			}

			if ( m_Level >= 10 )
			{
				m_Defense += (m_Level - 9) * (m_Level - 8) / 2;
			}
		}
	}

	if ( _type == ITEM_HELPER+30 )
	{
		m_Defense = m_Level*2 + 15;

		if ( m_Level >= 10 )
		{
			m_Defense += (m_Level - 9) * (m_Level - 8) / 2;
		}
	}

	if ( p->MagicDefense > 0 )
	{
		m_MagicDefense += m_Level * 3;

		if ( m_Level >= 10 )
		{
			m_MagicDefense += (m_Level - 9) * (m_Level - 8) / 2;
		}
	}

	m_Level &= 0xf;

	memset(m_Special,      0, MAX_ITEM_SPECIAL);
	memset(m_SpecialValue, 0, MAX_ITEM_SPECIAL);
	m_SpecialNum	= 0;
	m_Option1		= 0;
	m_Option2		= 0;
	m_Option3		= 0;
	m_SkillChange	= 0;

	//옵션1(스킬)
	if(Option1)
	{
		if ( p->SkillType != 0 )
		{
			if ( p->SkillType == AT_SKILL_LONGSPEAR )
			{
				m_SkillChange = TRUE;
				m_Special[m_SpecialNum] = 0;
				m_Option1 = 1;
			}
			else
			{
				m_Special[m_SpecialNum] = p->SkillType;
				m_Option1 = 1;
			}
		}
	}

	if ( m_Type == ITEM_HELPER+3 )
	{
		m_Option1 = TRUE;
		m_Special[m_SpecialNum] = AT_SKILL_KNIGHTDINORANT;
	}

	if ( m_Type == ITEM_HELPER+4 )
	{
		m_Option1 = TRUE;
		m_Special[m_SpecialNum] = AT_SKILL_DARKHORSE_ATTACK;
	}

	if ( m_Type == ITEM_HELPER+37 )
	{
		m_Option1 = TRUE;
		m_Special[m_SpecialNum] = AT_SKILL_FENRIR_ATTACK;
	}

	m_SpecialNum++;
	
	//옵션2(행운)
	if(Option2)
	{
		if(_type>=ITEM_SWORD && _type<ITEM_BOOT+MAX_ITEM_INDEX)
		{
			m_Special[m_SpecialNum] = AT_LUCK;			
			m_Option2 = 1;
		}
		if( _type>=ITEM_WING && _type<=ITEM_WING+6)
		{
			m_Special[m_SpecialNum] = AT_LUCK;
			m_Option2 = 1;
		}
		if ( _type == ITEM_HELPER+30 )
		{
			m_Special[m_SpecialNum] = AT_LUCK;
			m_Option2 = 1;
		}
	}
	m_SpecialNum++;

	//옵션3(추가포인트)
	if(Option3)
	{
		if(_type>=ITEM_SWORD && _type<ITEM_BOW+MAX_ITEM_INDEX)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
			m_Option3= Option3;
			m_RequireStrength += Option3*4;
		}
		if(_type>=ITEM_STAFF && _type<ITEM_STAFF+MAX_ITEM_INDEX)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
			m_Option3= Option3;
			m_RequireStrength += Option3*4;
		}	
		if(_type>=ITEM_SHIELD && _type<ITEM_SHIELD+MAX_ITEM_INDEX)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_BLOCKING;
			m_Option3= Option3;
			m_RequireStrength += Option3*4;
		}	
		if(_type>=ITEM_HELM && _type<ITEM_BOOT+MAX_ITEM_INDEX)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_DEFENSE;
			m_Option3= Option3;
			m_RequireStrength += Option3*4;
		}
		if( (_type>=ITEM_HELPER+8 && _type<ITEM_HELPER+14) || (_type>=ITEM_HELPER+20 && _type<=ITEM_HELPER+28))//반지,목걸이
		{
			if ( _type == ITEM_HELPER+24 )
			{
				m_Special[m_SpecialNum] = AT_SET_OPTION_IMPROVE_MAXMANA;
				m_Option3 = Option3;
			}
			else if ( _type == ITEM_HELPER+28 )
			{
				m_Special[m_SpecialNum] = AT_SET_OPTION_IMPROVE_MAXAG;
				m_Option3 = Option3;
			}
			else
			{
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
				m_Option3= Option3;
			}
		}
		if ( _type == ITEM_HELPER+30 )
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
			m_Option3 = Option3;
			m_RequireStrength +=  Option3 * 4;
		}
		if( _type==ITEM_WING )
		{
			m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
			m_Option3= Option3;
		}
		else if( _type==ITEM_WING+1 )
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
			m_Option3= Option3;
			m_RequireStrength += Option3*4;
		}
		else if( _type==ITEM_WING+2 )
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
			m_Option3= Option3;
			m_RequireStrength += Option3*4;
		}
		else if ( _type == ITEM_WING+3 )
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
			m_Option3 = Option3;
			m_RequireStrength +=  Option3 * 4;

			if ( (m_NewOption&0x20) != 0 )
			{
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
			}
			else
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
			}

		}
		else if ( _type == ITEM_WING+4 )
		{
			m_Option3 = Option3;
			m_RequireStrength +=  Option3 * 4;

			if ( (m_NewOption&0x20) != 0 )
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
			}
			else
			{
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
			}

		}
		else if ( _type == ITEM_WING+5 )
		{
			m_Option3 = Option3;
			m_RequireStrength +=  Option3 * 4;

			if ( (m_NewOption&0x20) != 0 )
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
			}
			else
			{
				m_Special[m_SpecialNum] = AT_LIFE_REGENERATION;
			}

		}			
		else if ( _type == ITEM_WING+6 )
		{
			m_Option3 = Option3;
			m_RequireStrength +=  Option3 * 4;

			if ( (m_NewOption&0x20) != 0 )
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
			}
			else
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC;
			}

		}
		else if ( _type == ITEM_HELPER+3 )
		{
			m_Option3 = Option3;
	
			if ( (m_Option3&0x02) != 0 )
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_AG_MAX;
				m_SpecialNum++;
			}
		
			if ( (m_Option3&0x04) != 0 )
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_ATTACK_SPEED;
				m_SpecialNum++;
				m_AttackSpeed+=5;
			}
		
			if ( (m_Option3&0x01) != 0 )
			{
				m_Special[m_SpecialNum] = AT_DAMAGE_ABSORB;
			}
		}

		if ( _type == ITEM_HELPER+30 ) // Cape ofLord
		{
			m_Option3 = Option3;
			m_RequireStrength +=  Option3 * 4;
			m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE;
		}
	}
	
	m_SpecialNum++;

	if((_type>=ITEM_SHIELD && _type<ITEM_BOOT+MAX_ITEM_INDEX) ||//방어구
	    (_type>=ITEM_HELPER+8 && _type<=ITEM_HELPER+9) ||
		(_type>=ITEM_HELPER+21 && _type<=ITEM_HELPER+24) )//반지
	{
      	//생명증가
		if( (m_NewOption>>5)&1 )
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_LIFE;m_SpecialNum++;
		}	
		//마나증가
		if((m_NewOption>>4)&1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_MANA;m_SpecialNum++;
		}	
		//데미지감소
		if((m_NewOption>>3)&1)
		{
			m_Special[m_SpecialNum] = AT_DECREASE_DAMAGE;m_SpecialNum++;
		}	
		//데미지반사
		if((m_NewOption>>2)&1)
		{
			m_Special[m_SpecialNum] = AT_REFLECTION_DAMAGE;m_SpecialNum++;
		}	
		//방어성공율
		if((m_NewOption>>1)&1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_BLOCKING_PERCENT;m_SpecialNum++;
		}	
		//획득젠증가
		if((m_NewOption)&1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_GAIN_GOLD;m_SpecialNum++;
		}	
	}
	if((_type>=ITEM_SWORD && _type<ITEM_STAFF+MAX_ITEM_INDEX) ||//무기
	    (_type>=ITEM_HELPER+12 && _type<=ITEM_HELPER+13) ||
		(_type>=ITEM_HELPER+25 && _type<=ITEM_HELPER+28))//목걸이
	{
      	//마나사용량감소
		if((m_NewOption>>5)&1)
		{
			m_Special[m_SpecialNum] = AT_EXCELLENT_DAMAGE;m_SpecialNum++;
		}	
		if((_type>=ITEM_STAFF && _type<ITEM_STAFF+MAX_ITEM_INDEX) ||//지팡이
			(_type==ITEM_HELPER+12) ||
			(_type==ITEM_HELPER+25) ||
			(_type==ITEM_HELPER+27))//목걸이
		{
			//마력증가(레벨)
			if((m_NewOption>>4)&1)
			{
     			m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC_LEVEL;m_SpecialNum++;
			}	
			//마력증가(퍼센트)
			if((m_NewOption>>3)&1)
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_MAGIC_PERCENT;m_SpecialNum++;
			}	
		}
		else
		{
			//공격력증가(레벨)
			if((m_NewOption>>4)&1)
			{
     			m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE_LEVEL;m_SpecialNum++;
			}	
			//공격력증가(퍼센트)
			if((m_NewOption>>3)&1)
			{
				m_Special[m_SpecialNum] = AT_IMPROVE_DAMAGE_PERCENT;m_SpecialNum++;
			}	
		}
		//공격속도
		if((m_NewOption>>2)&1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_ATTACK_SPEED;m_SpecialNum++;
			m_AttackSpeed += 7;
		}	
		//획득생명증가
		if((m_NewOption>>1)&1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_GAIN_LIFE;m_SpecialNum++;
		}	
		//획득마나증가
		if((m_NewOption)&1)
		{
			m_Special[m_SpecialNum] = AT_IMPROVE_GAIN_MANA;m_SpecialNum++;
		}	
	}

	if ( (_type >= ITEM_WING+3 && _type <= ITEM_WING+6) || _type == ITEM_HELPER+30 )
	{
		if((m_NewOption)&1)
		{
			m_Special[m_SpecialNum] = AT_PLUS_WING_MAXLIFE;m_SpecialNum++;
		}

		if((m_NewOption)&2)
		{
			m_Special[m_SpecialNum] = AT_PLUS_WING_MAXMANA;m_SpecialNum++;
		}

		if((m_NewOption)&4)
		{
			m_Special[m_SpecialNum] = AT_PLUS_ONE_PERCENT_DAMAGE;m_SpecialNum++;
		}

		if((m_NewOption)&8)
		{
			m_Special[m_SpecialNum] = AT_PLUS_WING_ADD_LEADERSHIP;m_SpecialNum++;
		}
	}

	if ( _type == ITEM_HELPER+37 )
	{
		if((m_NewOption)&1)
		{
			m_Special[m_SpecialNum] = AT_PLUS_FENRIR_INCREASE_LAST_DAMAGE;m_SpecialNum++;
		}
		else if((m_NewOption)&2)
		{
			m_Special[m_SpecialNum] = AT_PLUS_FENRIR_DECREASE_LAST_DAMAGE;m_SpecialNum++;
		}
	}

	if ( SOptionStatValue != 0 )
	{
		if (p->SetAttr != 0 )
		{
			m_Special[m_SpecialNum] = p->SetAttr + AT_SET_OPTION_WATER_MASTERY;
			m_SetAddStat = SOptionStatValue * 5; m_SpecialNum++;
		}
	}
	
	m_Part = p->ItemSlot;

	Value();

	if ( m_Type != ITEM_BOW+7 && m_Type != ITEM_BOW+15 )
	{
		m_DamageMinOrigin = m_DamageMin;
		m_DefenseOrigin = m_Defense;
		m_DamageMin -= (int)(m_DamageMin * m_CurrentDurabilityState);
		m_DamageMax -= (int)(m_DamageMax * m_CurrentDurabilityState);
		m_Defense -= (int)(m_Defense * m_CurrentDurabilityState);
		m_SuccessfulBlocking -= (int)(m_SuccessfulBlocking * m_CurrentDurabilityState);

		if ( m_Durability < 1.0f )
		{
			m_AttackSpeed = 0;
		}
	}
	
	if ( m_Durability == 0.0f )
	{
		memset(m_Resistance , 0, sizeof(m_Resistance));
	}
}

int CItem::GetLevel(void)
{
	return ItemAttribute[m_Type].Level;
}

DWORD CItem::GetNumber()
{
	return m_Number;
}

void CItem::Value()
{
	if(m_Type == -1) return;

	ITEM_ATTRIBUTE *p = &ItemAttribute[m_Type];

	if ( p->BuyMoney != 0 )
	{
		m_SellMoney = p->BuyMoney;
		m_BuyMoney = p->BuyMoney;
		m_SellMoney /= 3;
		
		if ( m_SellMoney >= 1000 )
		{
			m_SellMoney = m_SellMoney/100*100;
		}
		else if ( m_SellMoney >= 100 )
		{
			m_SellMoney = m_SellMoney/10*10;
		}

		if ( m_BuyMoney >= 1000 )
		{
			m_BuyMoney = m_BuyMoney/100*100;
		}
		else if ( m_BuyMoney >= 100 )
		{
			m_BuyMoney = m_BuyMoney/10*10;
		}
		return;
	}

	int Level = (int)sqrtf(m_Level);

	__int64 Gold = 0;

	int Type = m_Type/MAX_ITEM_INDEX;

	int Level2 = p->Level + m_Level*3;
	int excellent = 0;

	for (int i=0;i<m_SpecialNum;i++)
	{
		switch ( m_Special[i] )
		{
			case AT_IMPROVE_LIFE:
			case AT_IMPROVE_MANA:
			case AT_DECREASE_DAMAGE:
			case AT_REFLECTION_DAMAGE:
			case AT_IMPROVE_BLOCKING_PERCENT:
			case AT_IMPROVE_GAIN_GOLD:
			case AT_EXCELLENT_DAMAGE:
			case AT_IMPROVE_DAMAGE_LEVEL:
			case AT_IMPROVE_DAMAGE_PERCENT:
			case AT_IMPROVE_MAGIC_LEVEL:
			case AT_IMPROVE_MAGIC_PERCENT:
			case AT_IMPROVE_ATTACK_SPEED:
			case AT_IMPROVE_GAIN_LIFE:
			case AT_IMPROVE_GAIN_MANA:
				excellent = 1;
		}
	}

	if ( excellent != 0 )
	{
		Level2 += 25;
	}

	if(m_Type==ITEM_BOW+15)
	{
		int lc10 = 70;

		if ( m_Level == 1 )
		{
			lc10 = 1200;
		}
		else if ( m_Level == 2 )
		{
			lc10 = 2000;
		}

		if(m_Durability > 0)
           	Gold = (lc10 * m_Durability / p->Durability);
	}
	else if(m_Type==ITEM_BOW+7)
	{
		if(m_Durability > 0)
		{
			int lc11 = 100;

			if ( m_Level == 1 )
			{
				lc11 = 1400;
			}
			else if ( m_Level == 2 )
			{
				lc11 = 2200;
			}

           	Gold = (lc11 * m_Durability / p->Durability);
		}
	}
	else if(m_Type==ITEM_POTION+13)	// 축석
	{
        Gold = 9000000;
	}
	else if(m_Type==ITEM_POTION+14)	// 영석
	{
        Gold = 6000000;
	}
	else if(m_Type==ITEM_WING+15)	// 혼석
	{
        Gold = 810000;
	}
	else if(m_Type==ITEM_POTION+16)
	{
        Gold = 45000000;
	}
	else if(m_Type==ITEM_POTION+22)
	{
        Gold = 36000000;
	}
	else if(m_Type==ITEM_WING+30)
	{
        Gold = (m_Level+1)*9000000*10;
	}
	else if(m_Type==ITEM_WING+31)
	{
        Gold = (m_Level+1)*6000000*10;
	}
	else if(m_Type==ITEM_HELPER+15)
	{
        Gold = 33000000;
	}
	else if(m_Type==ITEM_HELPER+14)
	{
		if(m_Level == 1)
			Gold = 7500000;
		else
			Gold = 180000;
	}
	else if(m_Type==ITEM_POTION+31)
	{
        Gold = 60000000;
	}
	else if(m_Type==ITEM_POTION+7)
	{
        if(m_Level == 0)
			Gold = (int)(m_Durability) * 2100000;
		else if(m_Level == 1)
			Gold = (int)(m_Durability) * 1500000;
	}
	else if(m_Type==ITEM_HELPER+11)
	{
		if(m_Level == 1)
			Gold = 2400000;
	}
	else if(m_Type==ITEM_HELPER+7)
	{
        if(m_Level == 0)
			Gold = 1500000;
		else if(m_Level == 1)
			Gold = 1200000;
	}
	else if ( m_Type == ITEM_HELPER+32 ) 
	{
		Gold = (int)m_Durability * 150;
	}
	else if ( m_Type == ITEM_HELPER+33 ) 
	{
		Gold = (int)m_Durability * 300;
	}
	else if ( m_Type == ITEM_HELPER+34 ) 
	{
		Gold = (int)m_Durability * 3000;
	}
	else if ( m_Type == ITEM_HELPER+35 ) 
	{
		Gold = 30000;
	}
	else if ( m_Type == ITEM_HELPER+36 ) 
	{
		Gold = 90000;
	}
	else if ( m_Type == ITEM_HELPER+37 ) 
	{
		Gold = 150000;
	}
	else if ( m_Type == ITEM_HELPER+3 )
	{
		Gold = 960000;

		for ( int k=0;k<3;k++)
		{
			if ( (m_Option3 & (1<<k)) != 0 )
			{
				Gold += 300000;
			}
		}
	}
	else if ( m_Type == ITEM_POTION+17 )
	{
		switch ( m_Level )
		{
			case 1:
				Gold = 15000;
				break;
			case 2:
				Gold = 21000;
				break;
			case 3:
				Gold = 30000;
				break;
			case 4:
				Gold = 45000;
				break;
			default:
				Gold = 30000;
				break;
		}
	}
	else if ( m_Type == ITEM_POTION+18 )
	{
		switch ( m_Level )
		{
			case 1:
				Gold = 15000;
				break;
			case 2:
				Gold = 21000;
				break;
			case 3:
				Gold = 30000;
				break;
			case 4:
				Gold = 45000;
				break;
			default:
				Gold = 30000;
				break;
		}
	}
	else if ( m_Type == ITEM_POTION+19 )
	{
		switch ( m_Level )
		{
			case 1:
				Gold = 60000;
				break;
			case 2:
				Gold = 84000;
				break;
			case 3:
				Gold = 120000;
				break;
			case 4:
				Gold = 180000;
				break;
			default:
				Gold = 120000;
				break;
		}
	}
	else if ( m_Type == ITEM_POTION+20 )
	{
		Gold = 900;
	}
	else if ( m_Type == ITEM_POTION+21 )
	{
		switch ( m_Level )
		{
			case 0:
				Gold = 9000;
				m_Durability = m_BaseDurability;
			case 1:
				Gold = 9000;
				m_Durability = m_BaseDurability;
				break;
			case 3:
				Gold = (int)(m_Durability) * 3900;
				break;
			default:
				Gold = 9000;
				m_Durability = m_BaseDurability;
		}
	}
	else if ( m_Type == ITEM_POTION+9 && m_Level == 1)
	{
		Gold = 1000;
	}
	else if ( m_Type == ITEM_HELPER+18 )
	{
		switch ( m_Level )
		{
			case 1:
				Gold = 150000;
				break;
			case 2:
				Gold = 660000;
				break;
			case 3:
				Gold = 720000;
				break;
			case 4:
				Gold = 780000;
				break;
			case 5:
				Gold = 840000;
				break;
			case 6:
				Gold = 900000;
				break;
			case 7:
				Gold = 960000;
				break;
			default:
				Gold = 600000;
				break;
		}
	}
	else if ( m_Type ==ITEM_HELPER+16 || m_Type == ITEM_HELPER+17 )
	{
		switch ( m_Level )
		{
			case 1:
				Gold = 15000;
				break;
			case 2:
				Gold = 21000;
				break;
			case 3:
				Gold = 30000;
				break;
			case 4:
				Gold = 39000;
				break;
			case 5:
				Gold = 48000;
				break;
			case 6:
				Gold = 60000;
				break;
			case 7:
				Gold = 75000;
				break;
			default:
				Gold = 15000;
				break;
		}
	}
	else if ( m_Type == ITEM_HELPER+29 )
	{
		Gold = 5000;
	}
	else if ( m_Type == ITEM_HELPER+20 )
	{
		switch ( m_Level )
		{
			case 0:
				Gold =30000;
				break;

			default:
				Gold = 0;
		}
	}
	else if ( m_Type == ITEM_HELPER+31 )
	{
		switch ( m_Level )
		{
			case 0:
				Gold = 30000000;
				break;

			case 1:
				Gold = 15000000;
		}
	}
	else if ( m_Type == ITEM_POTION+28 )
	{
		Gold = 600000;
	}
	else if ( m_Type == ITEM_POTION+29 )
	{
		Gold = (DWORD)(((10000.0f) * m_Durability) * 3.0f);
	}
	else if ( m_Type == ITEM_WING+26 )
	{
		switch ( m_Level )
		{
			case 0:
				Gold = 60000;
				break;
		}
	}
	else if(p->Value > 0)
	{
		Gold += p->Value*p->Value*10/12;
		if( m_Type >= ITEM_POTION && m_Type <= ITEM_POTION+8)
		{
			if ( m_Level > 0 )
			{
				Gold *= (__int64)(pow(2.0f, Level)) ;
			}

			Gold = Gold/10*10;
			Gold *= (__int64)m_Durability;
			m_BuyMoney = Gold;
			m_SellMoney = Gold/3;
			m_SellMoney = m_SellMoney/10*10;
			return;
			//Gold *= (int)m_Durability;
		}
	}
    else if( (Type==12 && m_Type > ITEM_WING+6) || Type==13 || Type==15)//12(날개,구슬) 13(사탄,반지) 15(법서)
	{
		//int Level2 = p->Level+m_Level*3;
		Gold = 100+Level2*Level2*Level2;
        for(int i=0;i<m_SpecialNum;i++)
		{
			switch(m_Special[i])
			{
			case AT_LIFE_REGENERATION:
				Gold += Gold*m_Option3;
				break;
			}
		}
	}
    else
	{
		//int Level2 = p->Level+m_Level*3;
		switch(m_Level)     //+무기일수록 가중치 붙음
		{
		case 5:Level2 += 4;break;
		case 6:Level2 += 10;break;
		case 7:Level2 += 25;break;
		case 8:Level2 += 45;break;
		case 9:Level2 += 65;break;
		case 10:Level2 += 95;break;
		case 11:Level2 += 135;break;
		case 12:Level2 += 185;break;
		case 13:Level2 += 245;break;
		}

		if ( Type == 12 && m_Type <= ITEM_WING+6 )	// Wings
		{
			Gold = (Level2+40)*Level2*Level2*11+40000000;
		}
		else
			Gold = 100+(40+Level2)*Level2*Level2/8;
		if(Type>=0 && Type<=6) //무기일때
		{
			if(!p->TwoHand) //한손무기일때 80%
				Gold = Gold*80/100;
		}
        for(int i=0;i<m_SpecialNum;i++)
		{
			switch(m_Special[i])
			{
			case AT_SKILL_BLOCKING:
			case AT_SKILL_SWORD1:
			case AT_SKILL_SWORD2:
			case AT_SKILL_SWORD3:
			case AT_SKILL_SWORD4:
			case AT_SKILL_SWORD5:
			case AT_SKILL_CROSSBOW:
			case AT_SKILL_SWORD6:
				Gold += (__int64)(Gold * 1.5);
				break;
			case AT_IMPROVE_DAMAGE:
			case AT_IMPROVE_MAGIC:
			case AT_IMPROVE_DEFENSE:
			case AT_LIFE_REGENERATION:
				switch(m_Option3)
				{
				case 1:	Gold += (__int64)(Gold * 6.0 / 10.0);	break;
				case 2:	Gold += (__int64)(Gold * 14.0 / 10.0);	break;
				case 3:	Gold += (__int64)(Gold * 28.0 / 10.0);	break;
				case 4:	Gold += (__int64)(Gold * 56.0 / 10.0);	break;
				}
				break;
			case AT_IMPROVE_BLOCKING:
				switch(m_Option3)
				{
				case 1:	Gold += (__int64)(Gold * 6.0 / 10.0);	break;
				case 2:	Gold += (__int64)(Gold * 14.0 / 10.0);	break;
				case 3:	Gold += (__int64)(Gold * 28.0 / 10.0);	break;
				case 4:	Gold += (__int64)(Gold * 56.0 / 10.0);	break;
				}
				break;
			case AT_LUCK:
				Gold += (__int64)(Gold * 25.0 / 100.0);
				break;

			case AT_IMPROVE_LIFE:
			case AT_IMPROVE_MANA:
			case AT_DECREASE_DAMAGE:
			case AT_REFLECTION_DAMAGE:
			case AT_IMPROVE_BLOCKING_PERCENT:
			case AT_IMPROVE_GAIN_GOLD:
			case AT_EXCELLENT_DAMAGE:
			case AT_IMPROVE_DAMAGE_LEVEL:
			case AT_IMPROVE_DAMAGE_PERCENT:
			case AT_IMPROVE_MAGIC_LEVEL:
			case AT_IMPROVE_MAGIC_PERCENT:
			case AT_IMPROVE_ATTACK_SPEED:
			case AT_IMPROVE_GAIN_LIFE:
			case AT_IMPROVE_GAIN_MANA:
				Gold += Gold;
				break;

			case AT_PLUS_WING_MAXLIFE:
			case AT_PLUS_WING_MAXMANA:
			case AT_PLUS_ONE_PERCENT_DAMAGE:
			case AT_IMPROVE_AG_MAX:
			case AT_DAMAGE_ABSORB:
				Gold += (__int64)(Gold * 25.0 / 100.0);
				break;
			}
		}
	}

	if ( Gold > 3000000000 )
	{
		Gold = 3000000000;
	}

	m_SellMoney = (DWORD)Gold;
	m_BuyMoney = (DWORD)Gold;
	
	m_SellMoney = m_SellMoney/3;

	if ( (m_Type < ITEM_POTION || m_Type > ITEM_POTION+8) &&
		  m_Type != ITEM_HELPER+20 &&
		  m_Type != ITEM_POTION+28 &&
		  m_Type != ITEM_POTION+29 &&
		  m_Type != ITEM_POTION+21 &&
		  m_Type != ITEM_HELPER+15 &&
		  m_Type != ITEM_HELPER+16 &&
		  m_Type != ITEM_HELPER+17 &&
		  m_Type != ITEM_HELPER+18 &&
		  m_Type != ITEM_HELPER+11 &&
		  m_Type != ITEM_HELPER+7  &&
		  m_Type != ITEM_HELPER+32 &&
		  m_Type != ITEM_HELPER+33 &&
		  m_Type != ITEM_HELPER+34 &&
		  m_Type != ITEM_HELPER+35 &&
		  m_Type != ITEM_HELPER+36 &&
		  m_Type != ITEM_HELPER+37 )
	{
		float lc15 = 1.0f-(m_Durability / m_BaseDurability);
		int lc16 = m_SellMoney*0.6*lc15;
		m_SellMoney -= lc16;
	}

	if ( m_SellMoney >= 1000 )
	{
		m_SellMoney = m_SellMoney/100*100;
	}
	else if ( m_SellMoney >= 100 )
	{
		m_SellMoney = m_SellMoney/10*10;
	}

	if ( m_BuyMoney >= 1000 )
	{
		m_BuyMoney = m_BuyMoney/100*100;
	}
	else if ( m_BuyMoney >= 100 )
	{
		m_BuyMoney = m_BuyMoney/10*10;
	}
}

void CItem::OldValue()
{
	if ( m_Type == -1 )
	{
		return;
	}

	int Gold = 0;

	if ( m_Type == ITEM_POTION+13 )
	{
		Gold = 100000;
	}
	else if ( m_Type == ITEM_POTION+14 )
	{
		Gold = 70000;
	}
	else if ( m_Type == ITEM_WING+15 )
	{
		Gold = 40000;
	}
	else if ( m_Type == ITEM_POTION+22 )
	{
		Gold = 450000;
	}
	else if ( m_Type == ITEM_POTION+16 )
	{
		Gold = 450000;
	}
	else
	{
		m_OldBuyMoney = m_BuyMoney;
		return;
	}

	m_OldBuyMoney = Gold;

	if ( m_OldBuyMoney >= 1000 )
	{
		m_OldBuyMoney = m_OldBuyMoney /100*100;
	}
	else if ( m_OldBuyMoney >= 100 )
	{
		m_OldBuyMoney = m_OldBuyMoney / 10 * 10;
	}
}

//----------------------------------------------------------------------------
// 아이템의 가로 세로 크기를 얻는다.
//----------------------------------------------------------------------------
BOOL CItem::GetSize(int &w, int &h)
{
#ifdef _DEBUG
	if( m_Type < 0 || m_Type > MAX_ITEM-1 ) 
	{
//		LogAdd("error: %s %d (m_Type:%d)", __FILE__, __LINE__, m_Type);
		return FALSE;
	}
#endif
	w = ItemAttribute[m_Type].Width;
	h = ItemAttribute[m_Type].Height;

	return TRUE;
	//LogAdd("Item:%d %d", w, h);
}

int CItem::ItemDamageMin()
{
	if( m_Type < 0 ) return 0;
	
	int damagemin = m_DamageMinOrigin;
	PlusSpecial(&damagemin,AT_IMPROVE_DAMAGE);
	return damagemin;
}

int CItem::ItemDefense()
{
	if( m_Type < 0 ) return 0;
	if( m_IsValidItem == false ) return 0;
	
	int Defense = m_Defense;
	PlusSpecial(&Defense,AT_IMPROVE_DEFENSE);
	return Defense;
}

// 아이템을 사용할수 있는 직업인지 
BOOL CItem::IsClass(char aClass, int ChangeUP)
{
	if( aClass < 0 || aClass >= MAX_CLASSTYPE )
	{
		gWindow.PrintLog("클래스 에러 : %d (%s %d)", aClass, __FILE__, __LINE__);
		return FALSE;
	}
	int requireclass = m_RequireClass[aClass];
	if( requireclass == 0 ) return FALSE;
	if( requireclass > 1 && requireclass != (ChangeUP+1) ) return FALSE;
	return TRUE;
}

//----------------------------------------------------------------------------
// 아이템의 이름을 얻는다.
//----------------------------------------------------------------------------
char *CItem::GetName()
{
#ifdef _DEBUG
	if( m_Type < 0 || m_Type > MAX_ITEM-1 ) 
	{
		//LogAdd("error: %s %d (m_Type:%d)", __FILE__, __LINE__, m_Type);
		return NULL;
	}
#endif	
	return ItemAttribute[m_Type].Name;
	//LogAdd("Item:%d %d", w, h);
}

void CItem::PlusSpecialSetRing(BYTE* Value)
{
	if ( m_Type < 0 )	return;
	if ( IsSetItem() == FALSE ) return;
	if ( m_Durability == 0.0f) return;
	if ( m_IsValidItem == false )	return;

	ITEM_ATTRIBUTE *p = &ItemAttribute[m_Type];

	if ( p->ResistanceType == -1 )	return;

	BYTE iValue = 5;

	Value[p->ResistanceType] += iValue - (BYTE)( iValue * m_CurrentDurabilityState);
}

void CItem::PlusSpecial(int* Value,int Special)
{
	if(m_Type < 0 ) return;
	if ( m_Durability == 0.0f) return;
	if ( m_IsValidItem == false )	return;
	
	for(int i=0;i<m_SpecialNum;i++)
	{
		if( m_Special[i] == Special )
		{
			switch( Special )
			{
			case AT_IMPROVE_BLOCKING :
				{
					int lc3 = m_Option3*5;
					*Value += lc3 - (int)( lc3 * m_CurrentDurabilityState );
				}
				break;
			case AT_LUCK :
				*Value += 4;
				break;

			case AT_IMPROVE_MAGIC:
			case AT_IMPROVE_DAMAGE:
			case AT_IMPROVE_DEFENSE:
				{
					int lc4 = m_Option3*4;
					*Value += lc4 - (int)( lc4 * m_CurrentDurabilityState );
				}
				break;

			case AT_PLUS_WING_MAXLIFE:
			case AT_PLUS_WING_MAXMANA:
				*Value =  *Value + m_Level * 5 + 50;
				break;

			case AT_IMPROVE_AG_MAX:
				*Value += 50;
				break;

			case AT_PLUS_WING_ADD_LEADERSHIP:
				*Value =  *Value + m_Level * 5 + 10;
				break;

			default :	// AT_IMPROVE_DAMAGE
						// AT_IMPROVE_MAGIC 
				        // AT_IMPROVE_DEFENSE
				*Value += m_Option3*4;
				break;
			}
		}
	}
}

void CItem::PlusSpecialPercent(int* Value,int Special, WORD Percent)
{
	if(m_Type < 0 ) return;
	

	for(int i=0;i<m_SpecialNum;i++)
	{
		if( m_Special[i] == Special )
		{
			*Value += *Value*Percent/100;
		}
	}
}

void CItem::PlusSpecialPercentEx(int * Value, int SourceValue, int Special)
{
	if ( m_Type < 0 )	return;
	if ( m_Durability == 0.0f ) return;
	if ( m_IsValidItem == false  ) return;

	for ( int i =0;i<m_SpecialNum;i++)
	{
		if ( m_Special[i] == Special )
		{
			int lc3 = SourceValue * m_Option3/100;
			*Value += lc3 - (int)(lc3 * m_CurrentDurabilityState );
		}
	}
}

void CItem::SetItemPlusSpecialStat(short * Value, int Special)
{
	if ( m_Type < 0 )	return;
	if ( m_Durability == 0.0f )	return;

	for ( int i =0;i<m_SpecialNum;i++)
	{
		if ( m_Special[i] == Special )
		{
			switch ( Special )
			{
				case AT_SET_IMPROVE_STRENGTH:
				case AT_SET_IMPROVE_DEXTERITY:
				case AT_SET_IMPROVE_ENERGY:
				case AT_SET_IMPROVE_VITALITY:
					{
						int lc3 = m_SetAddStat;
						*Value += lc3 - (int)(lc3 * m_CurrentDurabilityState);
					}
					break;

				default:
					*Value += m_Option3*3;
			}
		}
	}
}

int CItem::GetAddStatType()
{
	ITEM_ATTRIBUTE *p = &ItemAttribute[m_Type];
	return p->SetAttr;
}

int CItem::GetWeaponType()
{
	if ( m_Type <= ITEM_STAFF ) return 1;
	if ( m_Type <= ITEM_SHIELD ) return 2;
	return 0;
}

void CItem::SetPetItemInfo(int petlevel, int petexp)
{
	if ( m_Type == ITEM_HELPER+4 )
	{
		m_PetItem_Level = petlevel;
		m_PetItem_Exp = petexp;

		if ( m_PetItem_Exp < gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level] || m_PetItem_Exp >= gPetItemExp.m_DarkHorseExpTable[1+m_PetItem_Level] )
		{
			m_PetItem_Exp = gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level];
		}

		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
		PetValue();
		m_IsLoadPetItemInfo = 1;
	}
	else if ( m_Type == ITEM_HELPER+5 )
	{
		m_PetItem_Level = petlevel;
		m_PetItem_Exp = petexp;

		if ( m_PetItem_Exp < gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level] || m_PetItem_Exp >= gPetItemExp.m_DarkSpiritExpTable[1+m_PetItem_Level] )
		{
			m_PetItem_Exp = gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level];
		}

		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
		PetValue();
		m_IsLoadPetItemInfo = 1;
	}
}

BOOL CItem::AddPetItemExp(int petexp)
{
	if ( m_Durability <= 0.0f )
	{
		return FALSE;
	}

	if ( m_IsLoadPetItemInfo == 0 )
	{
		return FALSE;
	}

	if ( m_PetItem_Level < 1 )
	{
		m_PetItem_Level = 1;
		m_PetItem_Exp = 0;
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
		PetValue();
		return FALSE;
	}

	if ( m_Type == ITEM_HELPER+4 )
	{
		if ( m_PetItem_Level >= 50 )
		{
			return FALSE;
		}

		BOOL ret = FALSE;
		int maxexp = gPetItemExp.m_DarkHorseExpTable[1+m_PetItem_Level];
		m_PetItem_Exp += petexp;

		while ( m_PetItem_Exp >= maxexp )
		{
			m_PetItem_Level++;
			maxexp = gPetItemExp.m_DarkHorseExpTable[1+m_PetItem_Level];
			ret = TRUE;
		}

		if ( ret != FALSE )
		{
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			PetValue();
			return TRUE;
		}
	}
	else if ( m_Type == ITEM_HELPER+5 )
	{
		if ( m_PetItem_Level >= 50 )
		{
			return FALSE;
		}

		BOOL ret = FALSE;
		int maxexp = gPetItemExp.m_DarkSpiritExpTable[1+m_PetItem_Level];
		m_PetItem_Exp += petexp;

		while ( m_PetItem_Exp >= maxexp )
		{
			m_PetItem_Level++;
			maxexp = gPetItemExp.m_DarkSpiritExpTable[1+m_PetItem_Level];
			ret = TRUE;
		}

		if ( ret != FALSE )
		{
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			PetValue();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CItem::DecPetItemExp(int percent)
{
	if ( m_IsLoadPetItemInfo == 0 )
	{
		return FALSE;
	}

	if ( m_PetItem_Level < 1 )
	{
		m_PetItem_Level = 1;
		m_PetItem_Exp = 0;
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
		PetValue();
		return FALSE;
	}

	if ( m_Type == ITEM_HELPER+4 )
	{
		int exp = gPetItemExp.m_DarkHorseExpTable[1+m_PetItem_Level] - gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level];
		exp = exp * percent/100;
		m_PetItem_Exp -= exp;

		if ( m_PetItem_Exp < 0 )
		{
			m_PetItem_Level = 1;
			m_PetItem_Exp = 0;
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			PetValue();
		}
		else if ( m_PetItem_Exp < gPetItemExp.m_DarkHorseExpTable[m_PetItem_Level] )
		{
			m_PetItem_Level--;
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			PetValue();
			return TRUE;
		}
	}
	else if ( m_Type == ITEM_HELPER+5 )
	{
		int exp = gPetItemExp.m_DarkSpiritExpTable[1+m_PetItem_Level] - gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level];
		exp = exp * percent/100;
		m_PetItem_Exp -= exp;

		if ( m_PetItem_Exp < 0 )
		{
			m_PetItem_Level = 1;
			m_PetItem_Exp = 0;
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			PetValue();
		}
		else if ( m_PetItem_Exp < gPetItemExp.m_DarkSpiritExpTable[m_PetItem_Level] )
		{
			m_PetItem_Level--;
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			PetValue();
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CItem::PetItemLevelDown(int exp)
{
	if ( m_IsLoadPetItemInfo == FALSE )
	{
		return FALSE;
	}

	if ( m_PetItem_Level < 2 )
	{
		return FALSE;
	}

	m_PetItem_Exp -= exp;
	m_PetItem_Level--;

	if ( m_Type == ITEM_HELPER+4 )
	{
		int petlevel = m_PetItem_Level;
		int petexp = gPetItemExp.m_DarkHorseExpTable[petlevel];

		while ( m_PetItem_Exp <= petexp )
		{
			if ( petlevel <= 1 )
			{
				break;
			}

			petlevel--;
			petexp = gPetItemExp.m_DarkHorseExpTable[petlevel];
		}

		m_PetItem_Level = petlevel;
	}
	else if ( m_Type == ITEM_HELPER+5 )
	{
		int petlevel = m_PetItem_Level;
		int petexp = gPetItemExp.m_DarkSpiritExpTable[petlevel];

		while ( m_PetItem_Exp <= petexp )
		{
			if ( petlevel <= 1 )
			{
				break;
			}

			petlevel--;
			petexp = gPetItemExp.m_DarkSpiritExpTable[petlevel];
		}

		m_PetItem_Level = petlevel;
	}

	Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
	PetValue();
	return TRUE;
}

void CItem::PetValue()
{
	if ( m_Type == ITEM_HELPER+4 )
	{
		m_SellMoney = m_PetItem_Level * 2000000;
	}
	else if ( m_Type == ITEM_HELPER+5 )
	{
		m_SellMoney = m_PetItem_Level * 1000000;
	}	

	m_BuyMoney = m_SellMoney;
	m_SellMoney /= 3;

	if ( m_SellMoney >= 1000 )
	{
		m_SellMoney = m_SellMoney / 100 * 100;
	}
	else if ( m_SellMoney >= 100 )
	{
		m_SellMoney = m_SellMoney / 10 * 10;
	}

	if ( m_BuyMoney >= 1000 )
	{
		m_BuyMoney = m_BuyMoney / 100 * 100;
	}
	else if ( m_BuyMoney >= 100 )
	{
		m_BuyMoney = m_BuyMoney / 10 * 10;
	}
}

BOOL CItem::IsExtItem()
{
	if ( (m_NewOption & 0x7F) != 0 )	return TRUE;
	return FALSE;
}

BOOL CItem::IsExtLifeAdd()
{
	if ( (m_NewOption & 0x20) == 32 )	return 4;
	return 0;
}

BOOL CItem::IsExtManaAdd()
{
	if ( (m_NewOption & 0x10) == 16 )	return 4;
	return 0;
}

BOOL CItem::IsExtDamageMinus()
{
	if ( (m_NewOption & 8) == 8 )	return 4;
	return 0;
}

BOOL CItem::IsExtDamageReflect()
{
	if ( (m_NewOption & 4) == 4 )	return 5;
	return 0;
}


BOOL CItem::IsExtDefenseSuccessfull()
{
	if ( (m_NewOption & 2) == 2 )	return 10;
	return 0;
}

BOOL CItem::IsExtMonsterMoney()
{
	if ( (m_NewOption & 1) == 1 )	return 40;
	return 0;
}

BOOL CItem::IsExtExcellentDamage()
{
	if ( (m_Type >= ITEM_SHIELD ) && (m_Type < ITEM_SHIELD+MAX_ITEM_INDEX ) ) return 0;
	if ( ( m_NewOption & 0x20 ) == 0x20 )	return 10;
	return 0;
}

BOOL CItem::IsExtAttackRate()
{
	if ( (m_NewOption & 0x10) == 16 )	return 1;
	return 0;
}

BOOL CItem::IsExtAttackRate2()
{
	if ( (m_NewOption & 8) == 8 )	return 2;
	return 0;
}

BOOL CItem::IsExtAttackSpeed()
{
	if ( (m_NewOption & 4) == 4 )	return 7;
	return 0;
}


BOOL CItem::IsExtMonsterDieLife()
{
	if ( (m_NewOption&2) == 2 )	return 1;
	return 0;
}


BOOL CItem::IsExtMonsterDieMana()
{
	if ( (m_Type >= ITEM_SHIELD ) && (m_Type < ITEM_SHIELD+MAX_ITEM_INDEX ) ) return 0;
	if ( (m_NewOption & 1) == 1 )	return 1;
	return 0;
}

BOOL CItem::IsWingOpGetOnePercentDamage()
{
	if ( m_Durability  < 1.0f ) return 0;
	if ( (m_NewOption & 4)!= 0 )	return 3;
	return 0;
}

BOOL CItem::IsWingOpGetManaToMoster()
{
	if ( m_Durability < 1.0f ) return 0;
	if ( (m_NewOption & 16 ) == 1 ) return 10;
	return 0;
}

BOOL CItem::IsDinorantReduceAttackDamaege()
{
	if ( m_Durability  < 1.0f ) return 0;
	if ( (m_NewOption & 1 ) == 1 ) return 5;
	return 0;
}

int CItem::IsFenrirIncLastAttackDamage()
{
	if ( m_Durability < 1.0f ) return 0;
	if ( (m_NewOption&1) == 1 ) return 10;
	return 0;
}

int CItem::IsFenrirDecLastAttackDamage()
{
	if ( m_Durability < 1.0f ) return 0;
	if ( (m_NewOption&2) == 2 ) return 10;
	return 0;
}

int CItem::SimpleDurabilityDown(int iDecValue)
{
	m_Durability -= 1.0f ;

	if ( m_Durability < 0 )
	{
		m_Durability = 0.0f;
		return 0;
	}

	if ( CheckDurabilityState() != 0 ) return 2;
	return m_Durability;
}

int CItem::DurabilityDown(int dur, int iDecValue)
{
	if ( m_Durability <= 0 ) return 0;
	m_DurabilitySmall += dur;

	if ( m_DurabilitySmall > 564 )
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1.0f;

		if ( this ->m_Durability < 0.0f ) m_Durability = 0;
		if ( CheckDurabilityState() != FALSE ) return 2;
		return 1;
	}
	
	return m_Durability;
}

int CItem::DurabilityDown2(int dur, int iDecValue)
{
	if ( m_Durability <= 0 )
	{
		return 0;
	}

	m_DurabilitySmall += dur;

	if ( m_DurabilitySmall > 1000 )
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1.0f;

		if ( m_Durability < 0.0f ) m_Durability = 0;
		if ( CheckDurabilityState() != FALSE ) return 2;
		return 1;
	}
	
	return 0;
}

int CItem::NormalWeaponDurabilityDown(int defense, int aIndex)
{
	if ( m_Durability <= 0 )
	{
		return 0;
	}

	int damagemin = m_DamageMinOrigin;
	int plusdamage = 0;
	PlusSpecial(&plusdamage, AT_IMPROVE_DAMAGE);

	if ( damagemin == 0 )
	{
		gWindow.PrintLog("damagemin이 0이다");
		return 0;
	}

	int div = damagemin + damagemin/2 + plusdamage;

	if ( div == 0 )
	{
		return 0;
	}

	int DecreaseDur = (defense*2)/div;
	m_DurabilitySmall += DecreaseDur;

	if ( m_DurabilitySmall > 564 )
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1.0f;

		if ( m_Durability < 0.0f )
		{
			m_Durability = 0;
		}

		if ( CheckDurabilityState() != 0 )
		{
			return 2;
		}
		
		return 1;
		
	}
	return 0;
}



int CItem::BowWeaponDurabilityDown(int defense, int aIndex)
{
	if ( m_Durability <= 0 )
	{
		return 0;
	}

	int damagemin = m_DamageMinOrigin;
	int plusdamage = 0;
	PlusSpecial(&plusdamage, AT_IMPROVE_DAMAGE);
	
	int div = damagemin + damagemin/2 + plusdamage;

	if ( div == 0 )
	{
		return 0;
	}

	int DecreaseDur = (defense*2)/div;
	m_DurabilitySmall += DecreaseDur;

	if ( m_DurabilitySmall > 780 )
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1.0f;

		if ( m_Durability < 0.0f )
		{
			m_Durability = 0;
		}
		
		if ( CheckDurabilityState() != 0 )
		{
			return 2;
		}
		
		return 1;
		
	}
	return 0;
}

int CItem::StaffWeaponDurabilityDown(int defence, int aIndex)
{
	if ( m_Durability <= 0 )
	{
		return 0;
	}

	int magic = m_Magic/2 + m_Level*2;
	int plusmagic = 0;
	PlusSpecial(&plusmagic, AT_IMPROVE_MAGIC);
	int div = magic + magic/3 + plusmagic;

	if ( div == 0 )
	{
		return 0;
	}

	int DecreaseDur = defence/div;
	m_DurabilitySmall += DecreaseDur;

	if ( m_DurabilitySmall > 1050 )
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1.0f;

		if ( m_Durability < 0.0f )
		{
			m_Durability = 0;
		}
		
		if ( CheckDurabilityState() != 0 )
		{
			return 2;
		}
		return 1;
		
	}
	return 0;
}




int CItem::ArmorDurabilityDown(int damagemin, int aIndex)
{
	if ( m_Durability <= 0 )
	{
		return 0;
	}

	if ( m_Type == -1 )
	{
		return 0;
	}

	int def = m_DefenseOrigin;
	int plusdef = 0;
	int DecreaseDur;

	if ( def == 0 )
	{
		gWindow.PrintLog("def이 0이다");
		return 0;
	}

	if ( m_Type >= ITEM_SHIELD && m_Type < ITEM_SHIELD+MAX_ITEM_INDEX )
	{
		PlusSpecial(&plusdef, AT_IMPROVE_BLOCKING);
		DecreaseDur = damagemin/(def * 5+ plusdef);
	}
	else if ( m_RequireClass[CLASS_WIZARD] != 0 )
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}
	else if ( m_RequireClass[CLASS_KNIGHT] != 0 )
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin/(def * 3 + plusdef);
	}
	else if ( m_RequireClass[CLASS_ELF] != 0 )
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin/(def * 2 + plusdef);
	}
	else if ( m_RequireClass[CLASS_MAGUMSA] != 0 )
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin/(def * 7 + plusdef);
	}
	else if ( m_RequireClass[CLASS_DARKLORD] != 0 )
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin/(def * 6 + plusdef);
	}
	else if ( m_RequireClass[CLASS_SUMMONER] != 0 )
	{
		PlusSpecial(&plusdef, AT_IMPROVE_DEFENSE);
		DecreaseDur = damagemin/(def * 6 + plusdef);
	}

	m_DurabilitySmall += DecreaseDur;

	if ( m_DurabilitySmall > 69 )
	{
		m_DurabilitySmall = 0;
		m_Durability -= 1.0f;

		if ( m_Durability < 0.0f )
		{
			m_Durability = 0;
		}
		
		if ( CheckDurabilityState() != 0 )
		{
			return 2;
		}
		
		return 1;
		
	}
	return 0;
}

BOOL CItem::CheckDurabilityState()
{
	if ( m_Durability == 0.0f )
	{
		if ( m_CurrentDurabilityState != 1.0f )
		{
			m_CurrentDurabilityState = 1.0f;
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			return true;
		}
	}
	else if ( m_Durability < m_DurabilityState[2] )
	{
		if ( m_CurrentDurabilityState != 0.5f )
		{
			m_CurrentDurabilityState = 0.5f;
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			return true;
		}
	}
	else if ( m_Durability < m_DurabilityState[1] )
	{
		if ( m_CurrentDurabilityState != 0.3f )
		{
			m_CurrentDurabilityState = 0.3f;
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			return true;
		}
	}
	else if ( m_Durability < m_DurabilityState[0] )
	{
		if ( m_CurrentDurabilityState != 0.2f )
		{
			m_CurrentDurabilityState = 0.2f;
			Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
			return true;
		}
	}
	else if ( m_CurrentDurabilityState != 0.0f )
	{
		m_CurrentDurabilityState = 0;
		Convert(m_Type, m_Option1, m_Option2, m_Option3, m_NewOption, m_SetOption);
		return true;
	}

	return false;
}

/*
WORD ItemMagicDefense(ITEM *Item)
{
	if(Item->Type == -1) return 0;
	WORD MagicDefense = Item->MagicDefense;
	ItemPlusSpecial(&MagicDefense,PLUS_MAGIC_DEFENSE,Item);
	return MagicDefense;
}
*/

void BufferItemtoConvert3(LPBYTE buf, int & type, BYTE & level, BYTE & op1, BYTE & op2, BYTE & op3, BYTE & dur)
{
	type	= buf[0]+ ((buf[3]&0x80)*2)+((buf[5]&0xF0)<<0x05);
	level	= (buf[1]>>3)&0xF;
	op1		= (buf[1]>>7)&0x01;
	op2		= (buf[1]>>2)&0x01;
	op3		= (buf[1]&0x03);
	dur     = buf[2];

	if ( type == ITEM_HELPER+3)
	{
		op3 |= (buf[3] & 0x40) >> 4;
	}
	else if ( (buf[3] & 0x40) == 0x40 )
	{
		op3 = 4;
	}
}

//----------------------------------------------------------------------------
// 아이템 정보를 3바이트 정보로 바꾼다.
//----------------------------------------------------------------------------
/*void ItemByteConvert3(LPBYTE buf, CItem item[], int maxitem)
{
	int n=0;

	WORD hiWord, loWord;

	// type/index, Level, Op1, Op2, Op3
	//   4 ,  4  ,   5,   1,    1,   2
	for( int index=0; index<maxitem; index++)
	{
		buf[n] = (BYTE)item[index].m_Type;		// Type       : 8bit
		n++;	

		buf[n] = 0;
		buf[n] |= item[index].m_Level<<3;		// Level	: 5bit
		buf[n] |= item[index].m_Option1<<7;		// Option 1 : 1bit
		buf[n] |= item[index].m_Option2<<2;		// Option 2 : 1bit
		buf[n] |= item[index].m_Option3;			// Option 3 : 2bit

		n++;
		buf[n] = (BYTE)item[index].m_Durability;			// Durability : 8bit
		n++;

		hiWord = HIWORD(item[index].m_Number);
		loWord = LOWORD(item[index].m_Number);
		buf[n] = SET_NUMBERH(hiWord);
		n++;
		buf[n] = SET_NUMBERL(hiWord);
		n++;
		buf[n] = SET_NUMBERH(loWord);
		n++;
		buf[n] = SET_NUMBERL(loWord);
		n++;
	}
}

*/
//----------------------------------------------------------------------------
// 아이템 정보를 7바이트 정보로 바꾼다.
//----------------------------------------------------------------------------
void ItemByteConvert7(LPBYTE buf, CItem item[], int maxitem)
{
	int n=0;
	WORD hiWord, loWord;


	// type / index, Level, Dur, Special, Number
	//   4  ,   4  ,   5,   3,    8        4Byte
	for( int index=0; index<maxitem; index++)
	{
		buf[n] = (BYTE)item[index].m_Type;				// Type       : 4bit
		n++;

		buf[n] = 0;
		buf[n] |= item[index].m_Level<<3;		// Level	: 5bit
		buf[n] |= item[index].m_Option1<<7;		// Option 1 : 1bit
		buf[n] |= item[index].m_Option2<<2;		// Option 2 : 1bit
		buf[n] |= item[index].m_Option3;			// Option 3 : 2bit

		n++;
		buf[n] = (BYTE)item[index].m_Durability;			// Durability : 8bit
		n++;

		hiWord = item[index].m_Number>>16;
		loWord = item[index].m_Number&0xFFFF;

     	buf[n] = SET_NUMBERH(hiWord);
		n++;
		buf[n] = SET_NUMBERL(hiWord);
		n++;
		buf[n] = SET_NUMBERH(loWord);
		n++;
		buf[n] = SET_NUMBERL(loWord);
		n++;
	}
}

//----------------------------------------------------------------------------
// 아이템 정보를 10 바이트 정보로 바꾼다.
//----------------------------------------------------------------------------
void ItemByteConvert10(LPBYTE buf, CItem item[], int maxitem)
{
	int n=0;
	WORD hiWord, loWord;


	// type / index, Level, Dur, Special, Number, extension
	//   4  ,   4  ,   5,   3,    8        4Byte,   3Byte
	for( int index=0; index<maxitem; index++)
	{
		if ( item[index].m_Type == ITEM_HELPER+19 )
		{
			if ( CHECK_LIMIT(item[index].m_Level, 3) != FALSE )
			{
				buf[n] = -1;
				buf[n+1] = -1;
				buf[n+2] = -1;
				buf[n+3] = -1;
				buf[n+4] = -1;
				buf[n+5] = -1;
				buf[n+6] = -1;
				buf[n+7] = -1;
				buf[n+8] = -1;
				buf[n+9] = -1;
				n += 16;
				continue;
			}
		}

		if ( item[index].m_Type <  0 )
		{
			buf[n] = -1;
			buf[n+1] = -1;
			buf[n+2] = -1;
			buf[n+3] = -1;
			buf[n+4] = -1;
			buf[n+5] = -1;
			buf[n+6] = -1;
			buf[n+7] = -1;
			buf[n+8] = -1;
			buf[n+9] = -1;
			n += 16;
		}
		else
		{
			buf[n] = (BYTE)item[index].m_Type%256;				// Type       : 4bit
			n++;

			buf[n] = 0;
			buf[n] |= item[index].m_Level<<3;		// Level	: 5bit
			buf[n] |= item[index].m_Option1<<7;		// Option 1 : 1bit
			buf[n] |= item[index].m_Option2<<2;		// Option 2 : 1bit
			buf[n] |= item[index].m_Option3&3;			// Option 3 : 2bit

			n++;
			buf[n] = item[index].m_Durability;			// Durability : 8bit
			n++;

			hiWord = item[index].m_Number >> 16;
			loWord = item[index].m_Number & 0xFFFF;

     		buf[n] = SET_NUMBERH(hiWord);
			n++;
			buf[n] = SET_NUMBERL(hiWord);
			n++;
			buf[n] = SET_NUMBERH(loWord);
			n++;
			buf[n] = SET_NUMBERL(loWord);
			n++;

			buf[n] = 0;

			if ( item[index].m_Type > 255 )
			{
				buf[n] = 0x80;
			}

			if ( item[index].m_Option3 > 3 )
			{
				buf[n] |= 0x40;
			}

			buf[n] |= item[index].m_NewOption;
			n++;
			buf[n] = item[index].m_SetOption;
			n++;
			n++;
		}
	}
}


//----------------------------------------------------------------------------
// 아이템 정보를 3바이트 정보로 바꾼다.
//----------------------------------------------------------------------------
void ItemByteConvert(LPBYTE buf, CItem item)
{
	int n=0;
	memset(buf, 0, 7);

	// type , index, Level, Dur, Special 
	//   4  ,   4  ,   5,   3,    8
	buf[n] = item.m_Type & 0xFF;				// Type       : 4bit
	n++;

	buf[n] = 0;
	buf[n] |= item.m_Level<<3;		// Level	: 5bit
	buf[n] |= item.m_Option1<<7;		// Option 1 : 1bit
	buf[n] |= item.m_Option2<<2;		// Option 2 : 1bit
	buf[n] |= item.m_Option3&3;			// Option 3 : 2bit
	
	n++;
	buf[n] = (BYTE)item.m_Durability;			// Durability : 8bit
	n++;

	buf[n] = 0;
	buf[n] |= ((item.m_Type & 0x100) >> 1);

	if ( item.m_Option3 > 3 )
	{
		buf[n] |= 0x40;
	}

	buf[n] |= item.m_NewOption;
	n++;
	buf[n] = item.m_SetOption;
	n++;
	buf[n] |= (item.m_Type & ITEM_ETC ) >> 5;
	n++;
}


//----------------------------------------------------------------------------
// 아이템 정보를 3바이트 정보로 바꾼다.
//----------------------------------------------------------------------------
//void ItemByteConvert(LPBYTE buf, BYTE type, BYTE special, BYTE level, BYTE dur)
void ItemByteConvert(LPBYTE buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption)
{
	memset(buf, 0, 7);
	int n=0;

	// type , index, Level, Dur, Special 
	//   4  ,   4  ,   5,   3,    8
	buf[n] = type&0xFF;				// Type       : 4bit
	n++;
	buf[n] = 0;
	buf[n] |= level<<3;			// Level	  : 5bit
	buf[n] |= Option1<<7;
	buf[n] |= Option2<<2;		// Option 2 : 1bit
	buf[n] |= Option3&3;		// Option 3 : 2bit
	n++;
	buf[n] = (BYTE)dur;			// Durability : 8bit
	n++;

	buf[n] = 0;
	buf[n] |= ((type& 0x100) >> 1);

	if ( Option3 > 3 )
	{
		buf[n] |= 0x40;
	}

	buf[n] |= Noption;
	n++;
	buf[n] = SetOption;
	n++;
	buf[n] |= (type & ITEM_ETC ) >> 5;
	n++;
}

void ItemByteConvert16(LPBYTE buf, CItem item[], int maxitem)
{
	int n=0;
	WORD hiWord, loWord;


	// type / index, Level, Dur, Special, Number, extension
	//   4  ,   4  ,   5,   3,    8        4Byte,   3Byte
	for( int index=0; index<maxitem; index++)
	{
		if ( item[index].m_Type == ITEM_HELPER+19 )
		{
			if ( CHECK_LIMIT(item[index].m_Level, 3) != FALSE )
			{
				buf[n] = -1;
				buf[n+1] = -1;
				buf[n+2] = -1;
				buf[n+3] = -1;
				buf[n+4] = -1;
				buf[n+5] = -1;
				buf[n+6] = -1;
				buf[n+7] = -1;
				buf[n+8] = -1;
				buf[n+9] = -1;
				buf[n+10]=-1;
				buf[n+11]=-1;
				buf[n+12]=-1;
				buf[n+13]=-1;
				buf[n+14]=-1;
				buf[n+15]=-1;
				n += 16;
				continue;
			}
		}

		if ( item[index].m_Type <  0 )
		{
			buf[n] = -1;
			buf[n+1] = -1;
			buf[n+2] = -1;
			buf[n+3] = -1;
			buf[n+4] = -1;
			buf[n+5] = -1;
			buf[n+6] = -1;
			buf[n+7] = -1;
			buf[n+8] = -1;
			buf[n+9] = -1;
			buf[n+10]=-1;
			buf[n+11]=-1;
			buf[n+12]=-1;
			buf[n+13]=-1;
			buf[n+14]=-1;
			buf[n+15]=-1;
			n += 16;
		}
		else
		{
			memset(buf+n, 0, 16);

			buf[n] = (BYTE)item[index].m_Type%256;				// Type       : 4bit
			n++;

			buf[n] = 0;
			buf[n] |= item[index].m_Level<<3;		// Level	: 5bit
			buf[n] |= item[index].m_Option1<<7;		// Option 1 : 1bit
			buf[n] |= item[index].m_Option2<<2;		// Option 2 : 1bit
			buf[n] |= item[index].m_Option3&3;			// Option 3 : 2bit

			n++;
			buf[n] = item[index].m_Durability;			// Durability : 8bit
			n++;

			hiWord = item[index].m_Number >> 16;
			loWord = item[index].m_Number & 0xFFFF;

     		buf[n] = SET_NUMBERH(hiWord);
			n++;
			buf[n] = SET_NUMBERL(hiWord);
			n++;
			buf[n] = SET_NUMBERH(loWord);
			n++;
			buf[n] = SET_NUMBERL(loWord);
			n++;

			buf[n] = 0;

			BYTE btItemType=0;
			btItemType |= (item[index].m_Type & ITEM_ETC ) >> 5;
			buf[n] |= ((item[index].m_Type & 0x100) >> 1);

			if ( item[index].m_Option3 > 3 )
			{
				buf[n] |= 0x40;
			}

			buf[n] |= item[index].m_NewOption;
			n++;
			buf[n] = item[index].m_SetOption;
			n++;
			buf[n] = 0;

			buf[n] |= btItemType;
			n++;
			n+=6;
		}
	}
}

//----------------------------------------------------------------------------
// type,과 index로 아이템 정보를 구한다.
// 최종적으로 실제 인덱스를 만들어 낸다...
// 아이템이 존재하는지도 알수있다.
int ItemGetNumberMake(int type, int index )
{
	int make;

	make = (type*MAX_ITEM_INDEX)+index;
	if( ItemAttribute[make].Width < 1 || ItemAttribute[make].Height < 1 )
	{
		return -1;
	}
	return make;
}

//----------------------------------------------------------------------------
// 인덱스를 넣어주면 아이템의 가로와 세로 크기를 얻을수있다.
void ItemGetSize(int index, int & width, int & height)
{
	width = ItemAttribute[index].Width;
	height= ItemAttribute[index].Height;
}

BOOL HasItemDurability(int index)
{
	if ( ItemAttribute[index].Durability == 0 && ItemAttribute[index].MagicDurability == 0)
	{
		return FALSE; 
	}
	
	return TRUE;
}

int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem)
{
	if ( index == ITEM_POTION+21 && itemLevel == 3 )
		itemLevel=0;
	
	if ( index == ITEM_POTION+29 )
		return 1;

	int dur=0;

	if ( itemLevel < 5)
	{
		dur= ItemAttribute[index].Durability + itemLevel;
	}
	else if ( itemLevel >= 5 )
	{
		if ( itemLevel == 10 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2-3;
		}
		else if (itemLevel == 11 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2-1;
		}
		else if (itemLevel == 12 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2+2;
		}
		else if (itemLevel == 13 )
		{
			dur=ItemAttribute[index].Durability + itemLevel*2+6;
		}
		else
		{
			dur=ItemAttribute[index].Durability + itemLevel*2-4;
		}
	}
	
	if ( (index < ITEM_WING+3 || index > ITEM_WING+6 ) &&
		  index != ITEM_SWORD+19 &&
		  index != ITEM_BOW+18 &&
		  index != ITEM_STAFF+10 &&
		  index != ITEM_MACE+13)
	{
		if ( SetItem != 0 )
			dur +=20;
		else if ( ExcellentItem != 0 )
			dur +=15;
	}
	return dur;
}

int ItemGetAttackDurability(int index)
{
	return ItemAttribute[index].AttackDur;
}

int ItemGetDefenseDurability(int index)
{
	return ItemAttribute[index].DefenceDur;
}

float GetRepairItemRate(int index)
{
	return ItemAttribute[index].RepaireMoneyRate;
}

float GetAllRepairItemRate(int index)
{
	return ItemAttribute[index].AllRepaireMoneyRate;
}

void CalRepairRate( int itemtype, int itemsubtype, ITEM_ATTRIBUTE* p)
{
	p->RepaireMoneyRate = (float)0.1;
	p->AllRepaireMoneyRate = (float)0.4;
}
//----------------------------------------------------------------------------
// 아이템 스크립트 데이터를 읽는다.
//----------------------------------------------------------------------------
BOOL OpenItemScript(char *FileName)
{
	int loop_count;

	if((SMDFile=fopen(FileName,"r")) == NULL)	
		return FALSE;

	memset ( ItemAttribute, 0, sizeof(ItemAttribute) );
	
	for (int n=0;n<MAX_ITEM;n++)
	{
		ItemAttribute[n].Level = 0xFF;
		ItemAttribute[n].RepaireMoneyRate  = 0;
		ItemAttribute[n].AllRepaireMoneyRate  = 0;
	}

	SMDToken Token;
	while(true)
	{
		Token = (*GetToken)();
		if(Token == END) break;
		if(Token == NUMBER)
		{
			int Type = (int)TokenNumber;
			loop_count = 0;
			while(true)
			{
				Token = (*GetToken)();
				int Index = (int)TokenNumber;

				if ( Type >= 0 && Type < MAX_ITEM_TYPE )
				{
					if ( g_MaxItemIndexOfEachItemType[Type] <= Index )
					{
						g_MaxItemIndexOfEachItemType[Type] = Index;
					}
				}

				if(Token==NAME && strcmp("end",TokenString)==NULL) break;


               	ITEM_ATTRIBUTE *p = &ItemAttribute[Type*MAX_ITEM_INDEX+Index];
				//memset(p,0,sizeof(ITEM_ATTRIBUTE));

				Token = (*GetToken)();p->ItemSlot  = (int)TokenNumber;
				Token = (*GetToken)();p->SkillType = (int)TokenNumber;

				Token = (*GetToken)();p->Width  = (int)TokenNumber;
				Token = (*GetToken)();p->Height = (int)TokenNumber;
				Token = (*GetToken)();p->Serial = (int)TokenNumber;
				
				Token = (*GetToken)();p->OptionFlag		= (int)TokenNumber;
				Token = (*GetToken)();p->MondownFlag	= (int)TokenNumber;
				
				Token = (*GetToken)();strcpy(p->Name,TokenString);
				if(Type>=0 && Type<=5)
				{
					Token = (*GetToken)();p->Level		  = (int)TokenNumber;

					Token = (*GetToken)();p->DamageMin		  = (int)TokenNumber;
					Token = (*GetToken)();p->DamageMax		  = (int)TokenNumber;
					Token = (*GetToken)();p->AttackSpeed      = (int)TokenNumber;
					Token = (*GetToken)();p->Durability       = (int)TokenNumber;
					
					Token = (*GetToken)();p->MagicDurability   = (int)TokenNumber;
					Token = (*GetToken)();p->MagicPW		   = (int)TokenNumber;
						
					Token = (*GetToken)();p->RequireLevel		= (int)TokenNumber;
					Token = (*GetToken)();p->RequireStrength	= (int)TokenNumber;
					Token = (*GetToken)();p->RequireDexterity	= (int)TokenNumber;
					Token = (*GetToken)();p->RequireEnergy		= (int)TokenNumber;
					Token = (*GetToken)();p->RequireVitality	= (int)TokenNumber;
					Token = (*GetToken)();p->RequireLeadership	= (int)TokenNumber;
					if(p->Width >= 2) p->TwoHand = true;
					(BYTE)p->Durability += (BYTE)p->MagicDurability;
				}
				if(Type>=6 && Type<=11)
				{
      				Token = (*GetToken)();p->Level            = (int)TokenNumber;

					if(Type==6)
					{
						Token = (*GetToken)();p->Defense				= (int)TokenNumber;
						Token = (*GetToken)();p->SuccessfulBlocking     = (int)TokenNumber;
					}

					else if(Type>=7 && Type<=9)
					{
						Token = (*GetToken)();p->Defense      = (int)TokenNumber;
						Token = (*GetToken)();p->MagicDefense = (int)TokenNumber;
					}
					else if(Type==10)
					{
						Token = (*GetToken)();p->Defense      = (int)TokenNumber;
						Token = (*GetToken)();p->AttackSpeed  = (int)TokenNumber;
					}
					else if(Type==11)
					{
						Token = (*GetToken)();p->Defense      = (int)TokenNumber;
						Token = (*GetToken)();p->WalkSpeed    = (int)TokenNumber;
					}
					Token = (*GetToken)();p->Durability		  = (int)TokenNumber;
					Token = (*GetToken)();p->RequireLevel	  = (int)TokenNumber;
					Token = (*GetToken)();p->RequireStrength  = (int)TokenNumber;
					Token = (*GetToken)();p->RequireDexterity = (int)TokenNumber;
					Token = (*GetToken)();p->RequireEnergy	  = (int)TokenNumber;
					Token = (*GetToken)();p->RequireVitality  = (int)TokenNumber;
					Token = (*GetToken)();p->RequireLeadership = (int)TokenNumber;
				}
				if(Type==13)
				{
      				Token = (*GetToken)();p->Level          = (int)TokenNumber;
					Token = (*GetToken)();p->Durability     = (int)TokenNumber;
					Token = (*GetToken)();p->Resistance[0]  = (int)TokenNumber;
					Token = (*GetToken)();p->Resistance[1]  = (int)TokenNumber;
					Token = (*GetToken)();p->Resistance[2]  = (int)TokenNumber;
					Token = (*GetToken)();p->Resistance[3]  = (int)TokenNumber;
					Token = (*GetToken)();p->Resistance[4]  = (int)TokenNumber;
					Token = (*GetToken)();p->Resistance[5]  = (int)TokenNumber;
					Token = (*GetToken)();p->Resistance[6]  = (int)TokenNumber;
					p->ResistanceType = 0xFF;

					for(int n = 0; n < MAX_RESISTANCE-1; n++)
					{
						if(p->Resistance[n] != 0) p->ResistanceType = n;
					}
					p->RequireLevel = p->Level;
				}
				if(Type==14)
				{					
					Token = (*GetToken)();p->Value = (int)TokenNumber;
					Token = (*GetToken)();p->Level = (int)TokenNumber;
					p->Durability = 1;
				}
				if(Type==12)
				{
      				Token = (*GetToken)();p->Level				= (int)TokenNumber;
					Token = (*GetToken)();p->Defense			= (int)TokenNumber;
      				Token = (*GetToken)();p->Durability			= (int)TokenNumber;
      				Token = (*GetToken)();p->RequireLevel		= (int)TokenNumber;
					Token = (*GetToken)();p->RequireEnergy		= (int)TokenNumber;
					Token = (*GetToken)();p->RequireStrength	= (int)TokenNumber;
					Token = (*GetToken)();p->RequireDexterity	= (int)TokenNumber;
					Token = (*GetToken)();p->RequireLeadership	= (int)TokenNumber;
					Token = (*GetToken)();p->BuyMoney		    = (int)TokenNumber;
				}
				if(Type==15)
				{
      				Token = (*GetToken)();p->Level            = (int)TokenNumber;
					Token = (*GetToken)();p->RequireLevel	  = (int)TokenNumber;
					Token = (*GetToken)();p->RequireEnergy    = (int)TokenNumber;
					Token = (*GetToken)();p->BuyMoney		  = (int)TokenNumber;
					p->DamageMin = p->Level;
                    p->DamageMax = p->Level+p->Level/2;

				}

				if(Type <= 11 || Type == 13)
				{
					Token = (*GetToken)();
				}

				p->SetAttr = TokenNumber;

				//사용 클래스
				if(Type<=13 || Type==15 )
				{
					Token = (*GetToken)();p->RequireClass[CLASS_WIZARD]		= (int)TokenNumber;
					Token = (*GetToken)();p->RequireClass[CLASS_KNIGHT]		= (int)TokenNumber;
					Token = (*GetToken)();p->RequireClass[CLASS_ELF]		= (int)TokenNumber;
					Token = (*GetToken)();p->RequireClass[CLASS_MAGUMSA]	= (int)TokenNumber;
					Token = (*GetToken)();p->RequireClass[CLASS_DARKLORD]	= (int)TokenNumber;
					Token = (*GetToken)();p->RequireClass[CLASS_SUMMONER]	= (int)TokenNumber;
					Token = (*GetToken)();p->RequireClass[CLASS_MONK]		= (int)TokenNumber;
				}

				CalRepairRate(Type, Index, p);

				p->HaveItemInfo = TRUE;

				loop_count++;

				if ( loop_count > MAX_ITEM_INDEX )
				{
//					MsgBox("Error : Item Data fail. (LoopCount:%d) Item Type:%d Index %d", loop_count, Type, Index);
					break;
				}
			}

			if ( loop_count > MAX_ITEM_INDEX )
			{
				break;
			}
		}
	}
	fclose(SMDFile);	
	return TRUE;
}

BOOL OpenItemNameScript(char* FileName)
{
	int Token;

	SMDFile = fopen(FileName, "r");

	if (SMDFile == 0 )
	{
		return FALSE;
	}

	while ( true )
	{
		Token = GetToken();

		if ( Token == END )	break;
		if ( Token == NUMBER )
		{
			int ItemType;

			ItemType = TokenNumber;

			while ( true )
			{
				int ItemSubType;
				ITEM_ATTRIBUTE* p;

				Token = GetToken();
				ItemSubType = TokenNumber;

				if ( Token == 0 )
				{
					if ( strcmp("end", TokenString ) == 0 )
					{
						break;
					}
				}
					
				p = &ItemAttribute[ItemType*MAX_ITEM_INDEX+ItemSubType];

				Token = GetToken();
				strcpy(p->Name, TokenString );				
			}
		}
	}
	fclose(SMDFile);
	return TRUE;
}

// 아이템의 레벨을 얻는다.
BOOL zzzItemLevel(int type, int index, int level)
{
	int item_num;
	item_num = (type*MAX_ITEM_INDEX)+index;

	if( level < 0 ) level = 0;
	if( ItemAttribute[item_num].Level == 0xFF ) return FALSE;	// 레벨이존재하지 않다면.
	if( ItemAttribute[item_num].Level == 0    ) return FALSE;
	if( ItemAttribute[item_num].Level < level ) return TRUE;
	return FALSE;
}

//
int GetLevelItem(int type, int index, int level)
{
	int item_num;
	int itemlevel;

//	level = 65;
	if( level < 0 ) level = 0;
	item_num = (type*MAX_ITEM_INDEX)+index;

	if (ItemAttribute[item_num].MondownFlag  == 0) return -1;
	if( ItemAttribute[item_num].Level == 0xFF ) return -1;	// 레벨이존재하지 않다면.
	if( ItemAttribute[item_num].Level == 0 ) return -1;	// 레벨이존재하지 않다면.

	if( type == 14 )	// 사과및 물약시리즈는 
	{
		itemlevel = ItemAttribute[item_num].Level;
		
		if( index == 15 ) return -1;	// 금이면 리턴
		if( itemlevel >= (level-8) && itemlevel <= level )
		{
			return 0;
		}		
		return -1;
	}
	// 변신 반지일때는..
	if( type == 13 && index == 10 )
	{
		if( (rand()%10) == 0 )
		{
			int callbead_level=0;
			if( level < 0 ) level = 0;
			callbead_level = level/10;
			if( callbead_level > 0 ) callbead_level -= 1;
			if( callbead_level > 5 ) callbead_level = 5;
			return callbead_level;
		}
		return -1;
	}
	if( type == 12 && index == 11 )
	{	
		if( (rand()%10) == 0 )
		{
			int callbead_level=0;
			if( level < 0 ) level = 0;
			callbead_level = level/10;
			if( callbead_level > 0 ) callbead_level -= 1;
			if( callbead_level > 6 ) callbead_level = 6;
			return callbead_level;
		}
		// 이번 버젼엔 소환 구슬 안나가게..
		return -1;
	}
	
	itemlevel = ItemAttribute[item_num].Level;
	
	if( ((itemlevel) >= (level-18)) && (itemlevel<=level) )
	{
		if( type == 15 ) return 0;	// 마법서이면 + 레벨은 없다..
		itemlevel = (level - itemlevel)/3;

		if ( type == 13 )
		{
			if( index == 8 || 
				index == 9 || 
				index == 12 || 
				index == 13 || 
				index == 20 || 
				index == 21 || 
				index == 22 || 
				index == 23 || 
				index == 24 || 
				index == 25 || 
				index == 26 || 
				index == 27 || 
				index == 28 )
			{
				if ( itemlevel > 4 )
				{
					itemlevel=4;
				}
			}
		}

		return itemlevel;
	}
	return -1;
}

int GetSerialItem(int type)
{
	int item_num;

	item_num = type;

	if( ItemAttribute[item_num].Level == 0xFF ) return -1;	// 레벨이존재하지 않다면.

	if( ItemAttribute[item_num].Serial == 0 ) return FALSE;
	return TRUE;
}

int IsItem(int item_num)
{
	if ( item_num < 0 || item_num >= MAX_ITEM )
	{
		return 0;
	}

	return ItemAttribute[item_num].HaveItemInfo;
}

ITEM_ATTRIBUTE* GetItemAttr(int item_num)
{
	if ( item_num < 0 || item_num >= MAX_ITEM )
	{
		return NULL;
	}

	return &ItemAttribute[item_num];
}