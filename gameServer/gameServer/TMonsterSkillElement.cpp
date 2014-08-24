// TMonsterSkillElement.cpp: implementation of the TMonsterSkillElement class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterSkillElement.h"
#include "ObjUseSkill.h"
#include "protocol.h"
#include "gObjMonster.h"
#include "Gamemain.h"
#include "..\include\ReadScript.h"
#include "LogProc.h"
#include "..\common\winutil.h"
#include "BuffEffectSlot.h"
#include "LuckyItem.h"	//1.01.00
#ifdef IMPERIAL
#include "ImperialGuardian.h"	//1.01.00
#endif

BOOL TMonsterSkillElement::s_bDataLoad = FALSE;

TMonsterSkillElement TMonsterSkillElement::s_MonsterSkillElementArray[MAX_MONSTER_SKILL_ELEMENT];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterSkillElement::TMonsterSkillElement()
{
	this->Reset();
}

TMonsterSkillElement::~TMonsterSkillElement()
{
	return;
}

void TMonsterSkillElement::Reset()
{
	this->m_iSuccessRate = -1;
	this->m_iContinuanceTime = -1;
	this->m_iIncAndDecType = -1;
	this->m_iIncAndDecValue = -1;
	this->m_iNullifiedSkill = -1;
	this->m_iCharacterClass = -1;
	this->m_iCharacterLevel = -1;
	this->m_iElementType = -1;
	this->m_iElementNumber = -1;
	memset(this->m_szElementName, 0, sizeof(this->m_szElementName));
}

BOOL TMonsterSkillElement::LoadData(LPSTR lpszFileName)
{	
	TMonsterSkillElement::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[Monster Element] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			MsgBox("[Monster Element] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		TMonsterSkillElement::DelAllSkillElement();
		int iType = -1;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
				break;

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					char szElementName[20] = {0};
					int iElementNumber = -1;						
					int iElementType = -1;
					int iSuccessRate = -1;
					int iContinuanceTime = -1;
					int iIncAndDecType = -1;
					int iIncAndDecValue = -1;
					int iNullifiedMagic = -1;
					int iCharacterClass = -1;
					int iCharacterLevel = -1;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iElementNumber = TokenNumber;

					Token = GetToken();
					memcpy(szElementName, TokenString, sizeof(szElementName));

					Token = GetToken();
					iElementType = TokenNumber;

					Token = GetToken();
					iSuccessRate = TokenNumber;

					Token = GetToken();
					iContinuanceTime = TokenNumber;

					Token = GetToken();
					iIncAndDecType = TokenNumber;

					Token = GetToken();
					iIncAndDecValue = TokenNumber;

					Token = GetToken();
					iNullifiedMagic = TokenNumber;

					Token = GetToken();
					iCharacterClass = TokenNumber;

					Token = GetToken();
					iCharacterLevel = TokenNumber;

					if ( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_SKILL_ELEMENT )
					{
						MsgBox("[Monster Element] - ElementNumber(%d) Error (%s) File. ",
							iElementNumber, lpszFileName);
						continue;
					}

					char szMsg[200] ={0};

					wsprintf(szMsg, "¡ÙNum:%d,Name:%s,Type:%d,Rate:%d,Time:%d,I&DType:%d,I&DValue:%d,Null:%d,Class:%d, Level:%d\n",
						iElementNumber, szElementName, iElementType, iSuccessRate, iContinuanceTime,
						iIncAndDecType, iIncAndDecValue, iNullifiedMagic, iCharacterClass, iCharacterLevel);

					OutputDebugString(szMsg);

					TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iSuccessRate = iSuccessRate;
					TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iContinuanceTime = iContinuanceTime;
					TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iIncAndDecType = iIncAndDecType;
					TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iIncAndDecValue = iIncAndDecValue;
					TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iNullifiedSkill = iNullifiedMagic;
					TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iCharacterClass = iCharacterClass;
					TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iCharacterLevel = iCharacterLevel;
					TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iElementType = iElementType;
					TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iElementNumber = iElementNumber;
					memcpy(TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_szElementName,
						szElementName, sizeof(TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_szElementName));
					
				}
			}
		}	// while ( true )

		fclose(SMDFile);

		LogAddC(2, "[Monster Element] - %s file is Loaded", lpszFileName);

		TMonsterSkillElement::s_bDataLoad = TRUE;
	}
	catch ( DWORD)
	{
		MsgBox("[Monster Element] - Loading Exception Error (%s) File. ", lpszFileName);	
	}

	return FALSE;
}

BOOL TMonsterSkillElement::DelAllSkillElement()
{
	for(int i=0;i<MAX_MONSTER_SKILL_ELEMENT;i++)
	{
		TMonsterSkillElement::s_MonsterSkillElementArray[i].Reset();
	}

	return FALSE;
}

TMonsterSkillElement * TMonsterSkillElement::FindSkillElement(int iElementNumber)
{
	if ( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_SKILL_ELEMENT )
	{
		LogAddTD("[Monster Element] FindSkillElement() Error - (ElementNumber=%d) ",
			iElementNumber);

		return NULL;
	}

	if ( TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iElementNumber == iElementNumber )
	{
		return &TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber];
	}

	LogAddTD("[Monster Element] FindSkillElement() Error - (ElementNumber=%d) ",
		iElementNumber);

	return  NULL;
}

void TMonsterSkillElement::ForceSkillElement(int iIndex, int iTargetIndex)
{
	if ( (rand()%100) > this->m_iSuccessRate )
		return;

	switch ( this->m_iElementType )
	{
		case MSE_TYPE_STUN:
			this->ApplyElementStun(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_MOVE:
			this->ApplyElementMove(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_HP:
			this->ApplyElementHP(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_MP:
			this->ApplyElementMP(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_AG:
			this->ApplyElementAG(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_DEFENSE:
			this->ApplyElementDefense(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_ATTACK:
			this->ApplyElementAttack(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_DURABILITY:
			this->ApplyElementDurability(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_SUMMON:
			this->ApplyElementSummon(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_PUSH:
			this->ApplyElementPush(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_STAT_ENERGY:
			this->ApplyElementStatEnergy(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_STAT_STRENGTH:
			this->ApplyElementStatStrength(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_STAT_DEXITERITY:
			this->ApplyElementStatDexiterity(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_STAT_VITALITY:
			this->ApplyElementStatVitality(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_REMOVE_SKILL:
			this->ApplyElementRemoveSkill(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_RESIST_SKILL:
			this->ApplyElementResistSkill(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_IMMUNE_SKILL:
			this->ApplyElementImmuneSkill(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_TELEPORT_SKILL:
			this->ApplyElementTeleportSkill(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_DOUBLE_HP:
			this->ApplyElementDoubleHP(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_POISON:
			this->ApplyElementPoison(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_NORMALATTACK:
			this->ApplyElementNormalAttack(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_BERSERK://Season 4.5 addon
			this->ApplyElementBerserk(iIndex, iTargetIndex);//Season 4.5 addon
			break;
	}
}

BOOL TMonsterSkillElement::ApplyElementStun(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	int SkillStunTime = 0;

	if(this->m_iContinuanceTime == -1)
	{
		SkillStunTime = -10;
	}
	else
	{
		SkillStunTime = this->m_iContinuanceTime;//  Season 4.5 fix
	}

	gObjAddBuffEffect(lpTargetObj, AT_STUN, 0, 0, 0, 0, SkillStunTime);

	gObjSetPosition(iTargetIndex, lpTargetObj->X, lpTargetObj->Y);

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementMove(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementHP(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTINC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_PERCENTINC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_PERCENTDEC )
		{
			iIncDecValue = (((float)(lpTargetObj->Life) * (float)(this->m_iIncAndDecValue)))/100.0f;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}

		if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTINC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_CONSTANTINC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_CONSTANTDEC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC ||
				 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}

		if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_PERCENT )
		{
			lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPTime = this->m_iContinuanceTime;
			lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP = iIncDecValue;
			lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle = MSE_INCDEC_TYPE_CYCLE_PERCENT;

			if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_CONSTANT )
				lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle = MSE_INCDEC_TYPE_CYCLE_CONSTANT;
			else
				lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle = MSE_INCDEC_TYPE_CYCLE_PERCENT;
		}
	}

	lpTargetObj->Life += iIncDecValue;

	if ( lpTargetObj->Life < 0 )
	{
		lpTargetObj->Life = 0;
		GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);

		return TRUE;
	}

	if ( (lpTargetObj->MaxLife + lpTargetObj->AddLife) < lpTargetObj->Life )
	{
		lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;
		GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);

		return TRUE;
	}

	GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementMP(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = (((float)(lpTargetObj->Mana) * (float)(this->m_iIncAndDecValue)))/100.0f;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->Mana += iIncDecValue;

	if ( lpTargetObj->Mana < 0 )
	{
		lpTargetObj->Mana = 0;
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpObj->BP);
		return TRUE;
	}

	if ( (lpTargetObj->MaxMana + lpTargetObj->AddMana) < lpTargetObj->Mana )
	{
		lpTargetObj->Mana = lpTargetObj->MaxMana + lpTargetObj->AddMana;
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

		return TRUE;
	}

	GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementAG(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = lpTargetObj->BP * this->m_iIncAndDecValue / 100;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->BP += iIncDecValue;

	if ( lpTargetObj->BP < 0 )
	{
		lpTargetObj->BP = 0;
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

		return TRUE;
	}

	if ( (lpTargetObj->MaxBP + lpTargetObj->AddBP) < lpTargetObj->Mana )
	{
		lpTargetObj->Mana = lpTargetObj->MaxBP + lpTargetObj->AddBP;
		GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

		return TRUE;
	}

	GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementDefense(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = lpTargetObj->m_Defense * this->m_iIncAndDecValue / 100;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}	

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementDefenseTime = this->m_iContinuanceTime;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementDefense = iIncDecValue;

#ifdef IMPERIAL
	if ( CImperialGuardian::IsEventMap(lpObj->MapNumber) )
	{
		if ( m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC || m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
			gObjAddBuffEffect(lpTargetObj, 58, 23, 10, 0, 0, 10);
	}
#endif

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementAttack(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = lpTargetObj->m_Defense * this->m_iIncAndDecValue / 100;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}	

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAttackTime = this->m_iContinuanceTime;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAttack = iIncDecValue;

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementDurability(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Type != OBJ_USER )
		return FALSE;

	int iEquipmentPos = rand()%6+2;	// Armors
	CItem * lpEquipment = &lpTargetObj->pInventory[iEquipmentPos];

	if ( lpEquipment == NULL || lpEquipment->IsItem() == FALSE )
		return FALSE;

	if( g_LuckyItemManager.IsLuckyItemEquipment(lpEquipment->m_Type) )
    {
		return false;
    }

	int iDurEquipment = lpEquipment->m_Durability;
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = iDurEquipment * this->m_iIncAndDecValue / 100;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}	

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	iDurEquipment += iIncDecValue;

	if ( lpEquipment )
	{
		lpEquipment->m_Durability = iDurEquipment;

		if ( lpEquipment->m_Durability < 0.0f )
			lpEquipment->m_Durability = 0;

		GCItemDurSend(lpTargetObj->m_Index, iEquipmentPos, lpEquipment->m_Durability, 0);
	}

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementSummon(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if (lpObj->Class == 459	&& lpObj->Connected == 3 && lpObj->MapNumber == 58 && 	iIndex == iTargetIndex )//Season 4.5 addon
	{
		LogAddTD("[TMonsterSkillElement][ApplyElementSummon] Selupan use summon.");
		return FALSE;
	}

	if ( lpTargetObj->Connected < PLAYER_PLAYING || lpTargetObj->Type != OBJ_MONSTER )
		 return FALSE;

	lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;
	lpTargetObj->Mana = lpTargetObj->MaxMana + lpTargetObj->AddMana;
	lpTargetObj->Live = TRUE;

	gObjRemoveBuffEffect(lpTargetObj, AT_POISON);
	gObjRemoveBuffEffect(lpTargetObj, AT_ICE);
	gObjRemoveBuffEffect(lpTargetObj, AT_ICE_ARROW);

	lpTargetObj->m_ViewState = 0;
	lpTargetObj->Teleport = 0;

	gObjClearBuffEffect(lpTargetObj, CLEAR_TYPE_LOGOUT);

	for ( int i=0;i<MAX_SELF_DEFENSE;i++)
	{
		lpTargetObj->SelfDefenseTime[i] = 0;
	}

	gObjTimeCheckSelfDefense(lpTargetObj);
	gObjViewportListProtocolDestroy(lpTargetObj);
	gObjViewportClose(lpTargetObj);
	lpTargetObj->m_ActState.Attack = 0;
	lpTargetObj->m_ActState.EmotionCount = 0;
	lpTargetObj->m_ActState.Escape = 0;
	lpTargetObj->m_ActState.Move = 0;
	lpTargetObj->m_ActState.Rest = 0;
	lpTargetObj->m_ActState.Emotion = 0;
	lpTargetObj->TargetNumber = -1;
	lpTargetObj->NextActionTime = 5000;

	if ( this->m_iIncAndDecValue < 0 )
	{
		this->m_iIncAndDecValue = 10;
	}

	BYTE cX;
	BYTE cY;
	BOOL bGetPosition = FALSE;
	int iCount = 100;


	while ( iCount-- != 0 )
	{
		cX = ( rand() % (this->m_iIncAndDecValue+1) ) * (((rand()%2==0)?-1:1)) + lpObj->X;
		cY = ( rand() % (this->m_iIncAndDecValue+1) ) * (((rand()%2==0)?-1:1)) + lpObj->Y;

		BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cX, cY);

		if ( btMapAttr == 0 )
		{
			bGetPosition = TRUE;
			break;
		}
	}

	if ( bGetPosition == FALSE )
	{
		lpObj->Live = FALSE;
		lpObj->m_State = 4;
		lpObj->RegenTime = GetTickCount();
		lpObj->DieRegen = 1;

		return FALSE;
	}

	lpTargetObj->X = cX;
	lpTargetObj->Y = cY;
	lpTargetObj->MTX = lpTargetObj->X;
	lpTargetObj->MTY = lpTargetObj->Y;
	lpTargetObj->TX = lpTargetObj->X;
	lpTargetObj->TY = lpTargetObj->Y;
	lpTargetObj->StartX = lpTargetObj->X;
	lpTargetObj->StartY = lpTargetObj->Y;

	gObjMonsterHitDamageInit(lpTargetObj);
	CreateFrustrum(lpTargetObj->X, lpTargetObj->Y, lpTargetObj->m_Index);
	lpTargetObj->DieRegen = 0;
	lpTargetObj->m_State = 1;
	lpTargetObj->m_Agro.ResetAll();
	gObjViewportListCreate(lpTargetObj->m_Index);

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementPush(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	
	if ( lpObj->Class == 459 && lpObj->Connected == 3 && lpObj->MapNumber == 58 && iIndex == iTargetIndex )//Season 4.5 addon
	{
		return FALSE;
	}

	if ( this->m_iNullifiedSkill == -1 )
		this->m_iNullifiedSkill = 0;

	if ( this->m_iIncAndDecValue < 1 )
		this->m_iIncAndDecValue = 3;

	if ( this->m_iNullifiedSkill == 0 )
	{
		gObjBackSpring2(lpTargetObj, lpObj,this->m_iIncAndDecValue);
	}
	else
	{
		gObjAddMsgSendDelay(lpObj, 57, iTargetIndex, this->m_iNullifiedSkill, this->m_iIncAndDecValue);
	}

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementStatEnergy(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementStatStrength(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementStatDexiterity(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementStatVitality(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementRemoveSkill(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	gObjUseSkill.RemoveSpecificSkillState(lpTargetObj, this->m_iNullifiedSkill);

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementResistSkill(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementResistNumber = this->m_iNullifiedSkill;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementResistTime = this->m_iContinuanceTime;

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementImmuneSkill(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneNumber = this->m_iNullifiedSkill;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneTime = this->m_iContinuanceTime;

	int SkillImmune = 0;

	if(this->m_iContinuanceTime == -1)
	{
		SkillImmune = -10;
	}
	else
	{
		SkillImmune = this->m_iContinuanceTime / 1000;
	}

	gObjAddBuffEffect(lpTargetObj, AT_IMMUNE_MAGIC, 0, 0, 0, 0, SkillImmune);
	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementTeleportSkill(int iIndex, int iTargetIndex)
{
	if ( !OBJMAX_RANGE(iIndex))
		return FALSE;

	if(gObjCheckUsedBuffEffect(&gObj[iIndex], AT_STUN) == 1)
	{
		return FALSE;
	}

	if(gObjCheckUsedBuffEffect(&gObj[iIndex], AT_SLEEP) == 1)
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[iIndex];
	BYTE x;
	BYTE y;
	int depth = rand()%4 + 3;
	PMSG_MAGICATTACK_RESULT pAttack;

	if ( (rand()%2) == 0 )
		x = lpObj->X + depth;
	else
		x = lpObj->X - depth;

	if ( (rand()%2) == 0 )
		y = lpObj->Y + depth;
	else
		y = lpObj->Y - depth;

	if ( gObjCheckTeleportArea(iIndex, x, y) == FALSE )
	{
		LogAddC(2, "[%s][%s] Try Teleport Not Move Area [%d,%d]",
			lpObj->AccountID, lpObj->Name,	x, y);
		
		return FALSE;
	}

	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
	pAttack.MagicNumberH = SET_NUMBERH(6);
	pAttack.MagicNumberL = SET_NUMBERL(6);
	pAttack.SourceNumberH = SET_NUMBERH(iIndex);
	pAttack.SourceNumberL = SET_NUMBERL(iIndex);
	pAttack.TargetNumberH = SET_NUMBERH(iIndex);
	pAttack.TargetNumberL = SET_NUMBERL(iIndex);

	if ( lpObj->Type == OBJ_USER )
		DataSend(iIndex, (LPBYTE)&pAttack, pAttack.h.size);	

	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);	

	LogAddC(2, "[%s] Warp [%d,%d] -> [%d,%d]",
		lpObj->Name, lpObj->X, lpObj->Y, x, y);

	gObjTeleportMagicUse(iIndex, x, y);
	lpObj->TargetNumber = -1;

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementDoubleHP(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
		return FALSE;

	gObjAddMsgSendDelay(lpObj, 55, iTargetIndex, 400, 0);
	gObjAddMsgSendDelay(lpObj, 55, iTargetIndex, 1000, 0);

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementPoison(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
		return FALSE;

	if(gObjCheckUsedBuffEffect(lpTargetObj, AT_POISON) == 0)
	{
		if ( this->m_iNullifiedSkill  < 1 )
		{
			if ( retResistance(lpTargetObj, 1) == 0 )
			{
				lpTargetObj->lpAttackObj = lpObj;
				gObjAddBuffEffect(lpTargetObj, AT_POISON, 19, 3, 0, 0, this->m_iIncAndDecValue);
			}
		}
		else
		{
			gObjAddMsgSendDelay(lpObj, 56, iTargetIndex, this->m_iNullifiedSkill, this->m_iIncAndDecValue);
		}
	}

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementNormalAttack(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
		return FALSE;

	if ( this->m_iNullifiedSkill == -1 )
		this->m_iNullifiedSkill = 0;

	if ( this->m_iIncAndDecType != -1 && this->m_iIncAndDecValue != -1 )
	{
		//Season 4.5 changed
		if(lpTargetObj->pInventory[9].m_Type == ITEMGET(this->m_iIncAndDecType, this->m_iIncAndDecValue) && lpTargetObj->pInventory[9].m_Durability > 0.0 
		|| lpTargetObj->pInventory[10].m_Type == ITEMGET(this->m_iIncAndDecType, this->m_iIncAndDecValue)	&& lpTargetObj->pInventory[10].m_Durability > 0.0 
		|| lpTargetObj->pInventory[11].m_Type == ITEMGET(this->m_iIncAndDecType, this->m_iIncAndDecValue)	&& lpTargetObj->pInventory[11].m_Durability > 0.0)  
		{
			
		}
		else
		{
			lpTargetObj->Life = 0;
			gObjLifeCheck(lpTargetObj, lpObj, 100, 0, 0, 0, 0 ,0);

			return FALSE;
		}
	}

	if ( this->m_iNullifiedSkill == 0 )
	{
		gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0 ,0, 0, 0);
	}
	else
	{
		gObjAddMsgSendDelay(lpObj, 55, iTargetIndex, this->m_iNullifiedSkill, 0);
	}
	

	return FALSE;
}

//005E1480  - identical
BOOL TMonsterSkillElement::ApplyElementBerserk(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementBerserkTime = this->m_iContinuanceTime;

	int DurationTime = 0;
	
	if(this->m_iContinuanceTime == -1)
	{
		DurationTime = -10;
	}
	else
	{
		DurationTime = this->m_iContinuanceTime / 1000;
	}
		
	gObjAddBuffEffect(lpTargetObj, 63, 0, 0, 0, 0, DurationTime);
	gObjAddBuffEffect(lpTargetObj, 64, 0, 0, 0, 0, DurationTime);
	
	return FALSE;
}
