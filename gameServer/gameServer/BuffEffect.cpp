#include "stdafx.h"
#include "BuffEffect.h"
#include "..\include\readscript.h"
#include "ItemAddOption.h"

CBuffEffect g_BuffEffect;

CBuffEffect::CBuffEffect()
{

}

CBuffEffect::~CBuffEffect()
{

}

//0067c120
void CBuffEffect::SetBuffEffect(LPOBJ lpObj, BYTE EffectType, int EffectValue)
{
	if( lpObj == NULL || EffectType < 0 )
		return;

	if( lpObj->Connected < 3 )
		return;

	switch( EffectType )
	{
	case ADD_OPTION_SPEED:
		lpObj->m_AttackSpeed += EffectValue;
		lpObj->m_MagicSpeed += EffectValue;
		break;
	case ADD_OPTION_DEFENSE:
		lpObj->m_Defense += EffectValue;
		lpObj->m_MagicDefense += EffectValue;
		break;
	case ADD_OPTION_LIFE:
		lpObj->AddLife += EffectValue;
		GCReFillSend(lpObj->m_Index,(int)(lpObj->AddLife + lpObj->MaxLife),0xFEu,0,lpObj->iAddShield + lpObj->iMaxShield);
        GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFF, 0, lpObj->iShield);
		break;
	case ADD_OPTION_MANA:
		lpObj->AddMana += EffectValue;
		GCManaSend(lpObj->m_Index,(int)(lpObj->AddMana + lpObj->MaxMana),0xFEu,0,lpObj->AddBP + lpObj->MaxBP);
        GCManaSend(lpObj->m_Index, (int)lpObj->Mana, 0xFF, 0, lpObj->BP);
		break;
	case ADD_OPTION_STRENGTH:
		lpObj->AddStrength += EffectValue;
		break;
	case ADD_OPTION_DEXTERITY:
		lpObj->AddDexterity += EffectValue;
		break;
	case ADD_OPTION_VITALITY:
		lpObj->AddVitality += EffectValue;
		break;
	case ADD_OPTION_ENERGY:
		lpObj->AddEnergy += EffectValue;
		break;
	case ADD_OPTION_LEADERSHIP:
		lpObj->AddLeadership += EffectValue;
		break;
	case ADD_OPTION_WIZARD_DEFENSE:
        lpObj->m_nSoulBarrierDefence = EffectValue;
        break;
	case ADD_OPTION_MAGIC_DEFENSE:
		lpObj->m_MagicDefense += EffectValue;
		break;
	case ADD_OPTION_REFLECT:
		lpObj->DamageReflect += EffectValue;
		break;
	case ADD_OPTION_ATTACK_RATE:
		lpObj->m_AttackRating -= EffectValue;
		break;
	case ADD_OPTION_WIZARD_DEFENSE_DURATION:
		lpObj->m_nSoulBarrierManaRate = EffectValue;
		break;
	case ADD_OPTION_CRITICAL_DAMAGE:
		lpObj->m_CriticalDamage += EffectValue;
		break;
	case ADD_OPTION_EXCELLENT_DAMAGE:
		lpObj->m_ExcelentDamage += EffectValue;
		break;
	case ADD_OPTION_BERSERKMANA:
        lpObj->AddMana += (int)(EffectValue * lpObj->MaxMana / 100.0);
        GCManaSend(lpObj->m_Index,(int)(lpObj->AddMana + lpObj->MaxMana),0xFEu,0,lpObj->AddBP + lpObj->MaxBP);
        GCManaSend(lpObj->m_Index, (int)lpObj->Mana, 0xFFu, 0, lpObj->BP);
        break;
	case ADD_OPTION_BERSERKLIFE:
		{
			float fAddLife = 0;
			float fPer = (((float)(40.0f) - (float)(EffectValue))/100.0f);

			fPer = (fPer > 0.1f)?fPer:0.1f;
			fAddLife = fPer * lpObj->MaxLife;

			lpObj->AddLife -= (int)((float)(fAddLife));
			lpObj->Life = ( (float)(lpObj->Life) < ( (float)(lpObj->AddLife) + (float)(lpObj->MaxLife) ) )?( (float)(lpObj->Life) ):( ( (float)(lpObj->AddLife) + (float)(lpObj->MaxLife) ) );

			GCReFillSend(lpObj->m_Index,(int)(lpObj->AddLife + lpObj->MaxLife),0xFEu,0,lpObj->iAddShield + lpObj->iMaxShield);
			GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFFu, 0, lpObj->iShield);
		}
		break;
	case ADD_OPTION_MAGICDAMAGEMIN:
		lpObj->m_MagicDamageMin += EffectValue;
		break;
	case ADD_OPTION_MAGICDAMAGEMAX:
		lpObj->m_MagicDamageMax += EffectValue;
		break;
	case ADD_OPTION_XMASATTACK:
		lpObj->m_AttackDamageMaxLeft += EffectValue;
		lpObj->m_AttackDamageMinLeft += EffectValue;
		lpObj->m_AttackDamageMaxRight += EffectValue;
		lpObj->m_AttackDamageMinRight += EffectValue;
		lpObj->m_MagicDamageMin += EffectValue;
		lpObj->m_MagicDamageMax += EffectValue;
		lpObj->m_CurseDamgeMin += EffectValue;
		lpObj->m_CurseDamgeMax += EffectValue;
		break;
	case ADD_OPTION_XMASDEFENSE:
		lpObj->m_Defense += EffectValue;
		break;
	case ADD_OPTION_LEVEL_BP:
		lpObj->AddBP += EffectValue * (lpObj->m_nMasterLevel + lpObj->Level);
		GCManaSend(lpObj->m_Index,(int)(lpObj->AddMana + lpObj->MaxMana),0xFEu,0,lpObj->AddBP + lpObj->MaxBP);
        GCManaSend(lpObj->m_Index, (int)lpObj->Mana, 0xFFu, 0, lpObj->BP);
		break;
	case ADD_OPTION_LEVEL_SD:
		lpObj->iAddShield += EffectValue * (lpObj->m_nMasterLevel + lpObj->Level);
		GCReFillSend(lpObj->m_Index,(int)(lpObj->AddLife + lpObj->MaxLife),0xFEu,0,lpObj->iAddShield + lpObj->iMaxShield);
        GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFFu, 0, lpObj->iShield);
		break;
	case ADD_OPTION_BP:
		lpObj->AddBP += EffectValue;
		GCManaSend(lpObj->m_Index,(int)(lpObj->AddMana + lpObj->MaxMana),0xFEu,0,lpObj->AddBP + lpObj->MaxBP);
        GCManaSend(lpObj->m_Index, (int)lpObj->Mana, 0xFFu, 0, lpObj->BP);
		break;
	case ADD_OPTION_SD:
		lpObj->iAddShield += EffectValue;
		GCReFillSend(lpObj->m_Index,(int)(lpObj->AddLife + lpObj->MaxLife),0xFEu,0,lpObj->iAddShield + lpObj->iMaxShield);
        GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFFu, 0, lpObj->iShield);
		break;
	case ADD_OPTION_IGNOREDEFENSE:
		lpObj->SetOpIgnoreDefense += EffectValue;
		break;
	case ADD_OPTION_SUCCESSFULBLOCKING:
		lpObj->m_SuccessfulBlocking += EffectValue;
		break;
	case SUB_OPTION_SUCCESSFULBLOCKING:
        lpObj->m_SuccessfulBlocking -= EffectValue;
        if ( lpObj->m_SuccessfulBlocking < 0 )
          lpObj->m_SuccessfulBlocking = 0;
        break;
	default:
		return;
	}
}

void CBuffEffect::ClearBuffEffect(LPOBJ lpObj,BYTE EffectType,int EffectValue)
{
	if( lpObj == NULL || EffectType < 0 )
		return;

	if( lpObj->Connected < 3 )
		return;

	switch( EffectType )
	{
	case ADD_OPTION_SPEED:
		lpObj->m_AttackSpeed -= EffectValue;
		lpObj->m_MagicSpeed -= EffectValue;
		break;
	case ADD_OPTION_DEFENSE:
		lpObj->m_Defense -= EffectValue;
		lpObj->m_MagicDefense -= EffectValue;
		break;
	case ADD_OPTION_LIFE:
		lpObj->AddLife -= EffectValue;
		GCReFillSend(lpObj->m_Index,(int)(lpObj->AddLife + lpObj->MaxLife),0xFEu,0,lpObj->iAddShield + lpObj->iMaxShield);
        GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFFu, 0, lpObj->iShield);
		break;
	case ADD_OPTION_MANA:
		lpObj->AddMana -= EffectValue;
		GCManaSend(lpObj->m_Index,(int)(lpObj->AddMana + lpObj->MaxMana),0xFEu,0,lpObj->AddBP + lpObj->MaxBP);
        GCManaSend(lpObj->m_Index, (int)lpObj->Mana, 0xFFu, 0, lpObj->BP);
		break;
	case ADD_OPTION_STRENGTH:
		lpObj->AddStrength -= EffectValue;
		break;
	case ADD_OPTION_DEXTERITY:
		lpObj->AddDexterity -= EffectValue;
		break;
	case ADD_OPTION_VITALITY:
		lpObj->AddVitality -= EffectValue;
		break;
	case ADD_OPTION_ENERGY:
		lpObj->AddEnergy -= EffectValue;
		break;
	case ADD_OPTION_LEADERSHIP:
		lpObj->AddLeadership -= EffectValue;
		break;
	case ADD_OPTION_WIZARD_DEFENSE:
        lpObj->m_nSoulBarrierDefence -= EffectValue;
        break;
	case ADD_OPTION_MAGIC_DEFENSE:
		lpObj->m_MagicDefense -= EffectValue;
		break;
	case ADD_OPTION_REFLECT:
		lpObj->DamageReflect -= EffectValue;
		break;
	case ADD_OPTION_ATTACK_RATE:
		lpObj->m_AttackRating += EffectValue;
		break;
	case ADD_OPTION_WIZARD_DEFENSE_DURATION:
		lpObj->m_nSoulBarrierManaRate = 0;
		break;
	case ADD_OPTION_CRITICAL_DAMAGE:
		lpObj->m_CriticalDamage -= EffectValue;
		break;
	case ADD_OPTION_EXCELLENT_DAMAGE:
		lpObj->m_ExcelentDamage -= EffectValue;
		break;
	case ADD_OPTION_BERSERKMANA:
        lpObj->AddMana -= (int)(EffectValue * lpObj->MaxMana / 100.0);
		lpObj->Mana = ((lpObj->AddMana + lpObj->MaxMana) <= lpObj->Mana) ? lpObj->Mana : (lpObj->AddMana + lpObj->MaxMana);

        GCManaSend(lpObj->m_Index,(int)(lpObj->AddMana + lpObj->MaxMana),0xFEu,0,lpObj->AddBP + lpObj->MaxBP);
        GCManaSend(lpObj->m_Index, (int)lpObj->Mana, 0xFFu, 0, lpObj->BP);
        break;
	case ADD_OPTION_BERSERKLIFE:
		{
			float fAddLife = 0;
			float fPer = (((float)(40.0f) - (float)(EffectValue))/100.0f);

			fPer = (fPer > 0.1f)?fPer:0.1f;
			fAddLife = fPer * lpObj->MaxLife;

			lpObj->AddLife += (int)((float)(fAddLife));
			lpObj->Life = ( (float)(lpObj->Life) < ( (float)(lpObj->AddLife) + (float)(lpObj->MaxLife) ) )?( (float)(lpObj->Life) ):( ( (float)(lpObj->AddLife) + (float)(lpObj->MaxLife) ) );

			GCReFillSend(lpObj->m_Index,(int)(lpObj->AddLife + lpObj->MaxLife),0xFEu,0,lpObj->iAddShield + lpObj->iMaxShield);
			GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFFu, 0, lpObj->iShield);
		}
		break;
	case ADD_OPTION_MAGICDAMAGEMIN:
		lpObj->m_MagicDamageMin -= EffectValue;
		break;
	case ADD_OPTION_MAGICDAMAGEMAX:
		lpObj->m_MagicDamageMax -= EffectValue;
		//lpObj->m_MPSkillOpt.Ukn[43] = 0; --  need to update this when ML structs done
		break;
	case ADD_OPTION_XMASATTACK:
		lpObj->m_AttackDamageMaxLeft -= EffectValue;
		lpObj->m_AttackDamageMinLeft -= EffectValue;
		lpObj->m_AttackDamageMaxRight -= EffectValue;
		lpObj->m_AttackDamageMinRight -= EffectValue;
		lpObj->m_MagicDamageMin -= EffectValue;
		lpObj->m_MagicDamageMax -= EffectValue;
		lpObj->m_CurseDamgeMin -= EffectValue;
		lpObj->m_CurseDamgeMax -= EffectValue;
		break;
	case ADD_OPTION_XMASDEFENSE:
		lpObj->m_Defense -= EffectValue;
		break;
	case ADD_OPTION_LEVEL_BP:
		lpObj->AddBP -= EffectValue * (lpObj->m_nMasterLevel + lpObj->Level);
		GCManaSend(lpObj->m_Index,(int)(lpObj->AddMana + lpObj->MaxMana),0xFEu,0,lpObj->AddBP + lpObj->MaxBP);
        GCManaSend(lpObj->m_Index, (int)lpObj->Mana, 0xFFu, 0, lpObj->BP);
		break;
	case ADD_OPTION_LEVEL_SD:
		lpObj->iAddShield -= EffectValue * (lpObj->m_nMasterLevel + lpObj->Level);
		GCReFillSend(lpObj->m_Index,(int)(lpObj->AddLife + lpObj->MaxLife),0xFEu,0,lpObj->iAddShield + lpObj->iMaxShield);
        GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFFu, 0, lpObj->iShield);
		break;
	case ADD_OPTION_BP:
		lpObj->AddBP -= EffectValue;
		GCManaSend(lpObj->m_Index,(int)(lpObj->AddMana + lpObj->MaxMana),0xFEu,0,lpObj->AddBP + lpObj->MaxBP);
        GCManaSend(lpObj->m_Index, (int)lpObj->Mana, 0xFFu, 0, lpObj->BP);
		break;
	case ADD_OPTION_SD:
		lpObj->iAddShield -= EffectValue;
		GCReFillSend(lpObj->m_Index,(int)(lpObj->AddLife + lpObj->MaxLife),0xFEu,0,lpObj->iAddShield + lpObj->iMaxShield);
        GCReFillSend(lpObj->m_Index, (int)lpObj->Life, 0xFFu, 0, lpObj->iShield);
		break;
	case ADD_OPTION_IGNOREDEFENSE:
		lpObj->SetOpIgnoreDefense -= EffectValue;
		break;
	case ADD_OPTION_SUCCESSFULBLOCKING:
		lpObj->m_SuccessfulBlocking -= EffectValue;
		break;
	case SUB_OPTION_SUCCESSFULBLOCKING:
        lpObj->m_SuccessfulBlocking += EffectValue;
        break;
	default:
		return;
	}
}

void CBuffEffect::SetActiveBuffEffect(LPOBJ lpObj,BYTE EffectType,int EffectValue)
{
	if( EffectType <= 0 )
		return;

	switch( EffectType )
	{
	case BUFF_DAMAGE_EFFECT:
		GiveDamageEffect(lpObj,EffectValue);
		break;
	case BUFF_POISON_EFFECT:
		PoisonEffect(lpObj,EffectValue);
		break;
	}
}

void CBuffEffect::GiveDamageEffect(LPOBJ lpObj,int Damage)
{
	int DecreaseHealthPoint = 0;
	int DecreaseShiledPoint = 0;

	DecreaseHealthPoint = Damage;
	DecreaseShiledPoint = DecreaseHealthPoint;

	if( !lpObj->Live )
		return;

	if(lpObj->lpAttackObj != 0 && lpObj->Type == OBJ_USER && lpObj->lpAttackObj->Type == OBJ_USER)
	{
		DecreaseShiledPoint = 90 * DecreaseHealthPoint / 100;
		DecreaseHealthPoint -= DecreaseShiledPoint;

		if(lpObj->iShield-DecreaseShiledPoint > 0)
		{
			lpObj->iShield -= DecreaseShiledPoint;
			lpObj->Life -= DecreaseHealthPoint;
		}
		else
		{
			DecreaseHealthPoint += DecreaseShiledPoint - lpObj->iShield;
			DecreaseShiledPoint = lpObj->iShield;
			lpObj->Life -= DecreaseHealthPoint;
			lpObj->iShield = 0;
		}
	}
	else
	{
		DecreaseShiledPoint = 0;
		lpObj->Life -= DecreaseHealthPoint;
	}

	if(lpObj->Life < 0.0f)
		lpObj->Life = 0.0f;

	if(lpObj->lpAttackObj != 0)
		gObjLifeCheck(lpObj, lpObj->lpAttackObj, DecreaseHealthPoint, 3, 0, 0, 0, DecreaseShiledPoint);
}

void CBuffEffect::PoisonEffect(LPOBJ lpObj,BYTE PoisonRate)
{
	int DecreaseHealthPoint = 0;
	int DecreaseShiledPoint = 0;

	DecreaseHealthPoint = (int)(PoisonRate * lpObj->Life / 100.0);
	DecreaseShiledPoint = DecreaseHealthPoint;

	if(lpObj->lpAttackObj != 0 && lpObj->Type == OBJ_USER && lpObj->lpAttackObj->Type == OBJ_USER)
	{
		if(lpObj->iShield-DecreaseShiledPoint > 0)
		{
			lpObj->iShield -= DecreaseShiledPoint;
			DecreaseHealthPoint = 0;
		}
		else
		{
			DecreaseHealthPoint = DecreaseShiledPoint - lpObj->iShield;
			DecreaseShiledPoint = lpObj->iShield;
			lpObj->Life -= DecreaseHealthPoint;
			lpObj->iShield = 0;
		}
	}
	else
	{
		lpObj->Life -= DecreaseHealthPoint;
		DecreaseShiledPoint = 0;
	}
	
	if(lpObj->Life < 0.0f)
		lpObj->Life = 0.0f;

	if(lpObj->lpAttackObj != 0)
		gObjLifeCheck(lpObj, lpObj->lpAttackObj, DecreaseHealthPoint, 2, 0, 0, 1, DecreaseShiledPoint);
}

void CBuffEffect::SetPrevEffect(LPOBJ lpObj)
{
	if( lpObj == NULL )
		return;

	int BuffCount = 0;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if( !lpObj->m_BuffEffectList[i].btBuffIndex )
			continue;

		switch ( lpObj->m_BuffEffectList[i].btEffectType1 )
        {
		case ADD_OPTION_LIFE:
		case ADD_OPTION_MANA:
		case ADD_OPTION_STRENGTH:
		case ADD_OPTION_DEXTERITY:
		case ADD_OPTION_VITALITY:
		case ADD_OPTION_ENERGY:
		case ADD_OPTION_LEADERSHIP:
		case ADD_OPTION_REFLECT:
		case ADD_OPTION_LEVEL_BP:
		case ADD_OPTION_LEVEL_SD:
		case ADD_OPTION_BP:
		case ADD_OPTION_SD:
			BuffCount++;
			SetBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType1, lpObj->m_BuffEffectList[i].iValue1);
            break;
          default:
            break;
        }
		
		switch ( lpObj->m_BuffEffectList[i].btEffectType2 )
		{
		case ADD_OPTION_LIFE:
		case ADD_OPTION_MANA:
		case ADD_OPTION_STRENGTH:
		case ADD_OPTION_DEXTERITY:
		case ADD_OPTION_VITALITY:
		case ADD_OPTION_ENERGY:
		case ADD_OPTION_LEADERSHIP:
		case ADD_OPTION_REFLECT:
		case ADD_OPTION_LEVEL_BP:
		case ADD_OPTION_LEVEL_SD:
		case ADD_OPTION_BP:
		case ADD_OPTION_SD:
			BuffCount++;
			SetBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType2, lpObj->m_BuffEffectList[i].iValue2);
			break;
		default:
			break;
        }
	}
}

void CBuffEffect::SetNextEffect(LPOBJ lpObj)
{
	if( lpObj == NULL )
		return;

	int BuffCount = 0;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if( !lpObj->m_BuffEffectList[i].btBuffIndex )
			continue;

		switch ( lpObj->m_BuffEffectList[i].btEffectType1 )
        {
		case ADD_OPTION_LIFE:
		case ADD_OPTION_MANA:
		case ADD_OPTION_STRENGTH:
		case ADD_OPTION_DEXTERITY:
		case ADD_OPTION_VITALITY:
		case ADD_OPTION_ENERGY:
		case ADD_OPTION_LEADERSHIP:
		case ADD_OPTION_REFLECT:
		case ADD_OPTION_LEVEL_BP:
		case ADD_OPTION_LEVEL_SD:
		case ADD_OPTION_BP:
		case ADD_OPTION_SD:
			break;
		default:
			BuffCount++;
			SetBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType1, lpObj->m_BuffEffectList[i].iValue1);
            break;
        }
		
		switch ( lpObj->m_BuffEffectList[i].btEffectType2 )
		{
		case ADD_OPTION_LIFE:
		case ADD_OPTION_MANA:
		case ADD_OPTION_STRENGTH:
		case ADD_OPTION_DEXTERITY:
		case ADD_OPTION_VITALITY:
		case ADD_OPTION_ENERGY:
		case ADD_OPTION_LEADERSHIP:
		case ADD_OPTION_REFLECT:
		case ADD_OPTION_LEVEL_BP:
		case ADD_OPTION_LEVEL_SD:
		case ADD_OPTION_BP:
		case ADD_OPTION_SD:
			break;
		default:
			BuffCount++;
			SetBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType2, lpObj->m_BuffEffectList[i].iValue2);
			break;
        }
	}
}

void CBuffEffect::ClearAllBuffEffect(LPOBJ lpObj)
{
	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if( !lpObj->m_BuffEffectList[i].btBuffIndex )
			continue;

		ClearBuffEffect(lpObj,lpObj->m_BuffEffectList[i].btEffectType1, lpObj->m_BuffEffectList[i].iValue1);
		ClearBuffEffect(lpObj,lpObj->m_BuffEffectList[i].btEffectType2, lpObj->m_BuffEffectList[i].iValue2);
	}
}