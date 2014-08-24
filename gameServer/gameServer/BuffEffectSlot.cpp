#include "StdAfx.h"
#include "BuffEffectSlot.h"
#include "BuffEffect.h"
#include "ObjCalCharacter.h"
#include "ItemAddOption.h"
#include "..\common\winutil.h"
#include "MasterLevelSystem.h"
#include "GameMain.h"

#ifdef NPVP
#include "NewPVP.h"
#endif

CBuffEffectSlot g_BuffEffectSlot;

// Constructor / Destructor 

CBuffEffectSlot::CBuffEffectSlot()
{

}

CBuffEffectSlot::~CBuffEffectSlot()
{

}


int CBuffEffectSlot::SetEffect(LPOBJ lpObj,int iBuffIndex,BYTE btEffectType1,BYTE btEffectType2,int iEffectValue1,int iEffectValue2,int iDuration)
{
	LPBUFF_EFFECT_DATE lpBuffData = 0;
	LPBUFF_EFFECT_DATE lpPrevBuffData = 0;

	int BuffCount = 0;
	int DebuffCount = 0;
	int iRemovedBuffIndex = 0;

	if(lpObj == NULL)
		return -1;

	if(lpObj->Connected < PLAYER_PLAYING)
		return -1;

	if(g_BuffScript.CheckVaildBuffEffect(iBuffIndex) == FALSE)
		return -1;

	lpBuffData = g_BuffScript.GetBuffData(iBuffIndex);

	if(lpBuffData == NULL)
		return -1;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if( lpObj->m_BuffEffectList[i].btBuffIndex != NULL )
		{
			lpPrevBuffData = g_BuffScript.GetBuffData(lpObj->m_BuffEffectList[i].btBuffIndex);

			if(lpPrevBuffData == NULL)
				continue;
			
			if(lpPrevBuffData->btViewPortIndex == lpBuffData->btViewPortIndex )
			{
				this->RemoveEffect(lpObj,lpObj->m_BuffEffectList[i].btBuffIndex);
				iRemovedBuffIndex = lpPrevBuffData->btIndex;
			}

			if(lpPrevBuffData->btType == 0)
			{
				BuffCount +=1;
			}
			else if(lpPrevBuffData->btType == 1)
			{
				DebuffCount += 1;	
			}

			if(i > lpObj->m_BuffEffectCount)
				break;
		}
	}

	for( int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(lpObj->m_BuffEffectList[i].btBuffIndex == NULL)
		{
			lpObj->m_BuffEffectList[i].btBuffIndex = iBuffIndex;
			lpObj->m_BuffEffectList[i].btVpIndex = lpBuffData->btViewPortIndex;
			lpObj->m_BuffEffectList[i].btEffectType1 = btEffectType1;
			lpObj->m_BuffEffectList[i].btEffectType2 = btEffectType2;
			lpObj->m_BuffEffectList[i].iValue1 = iEffectValue1;
			lpObj->m_BuffEffectList[i].iValue2 = iEffectValue2;
			lpObj->m_BuffEffectList[i].iDuration = iDuration;
			lpObj->m_BuffEffectList[i].dwBuffTickCount = GetTickCount();

			g_BuffEffect.SetBuffEffect(lpObj,btEffectType1,iEffectValue1);
			g_BuffEffect.SetBuffEffect(lpObj,btEffectType2,iEffectValue2);
			break;
		}
	}

	return iRemovedBuffIndex;
}

bool CBuffEffectSlot::RemoveEffect(LPOBJ lpObj,int iBuffIndex)
{
	BYTE bIsRemoveEffect = 0;

	if(lpObj == NULL)
		return false;
	
	if(lpObj->Connected < PLAYER_PLAYING)
		return false;

	if(g_BuffScript.CheckVaildBuffEffect(iBuffIndex) == FALSE)
		return false;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(bIsRemoveEffect == 1)
		{
			if( (lpObj->m_BuffEffectList[i].btBuffIndex != 0) && (i > 0) )
			{
				memcpy(&lpObj->m_BuffEffectList[i-1],&lpObj->m_BuffEffectList[i],sizeof(lpObj->m_BuffEffectList[i])); //Change the Buff Slot to the removed one...
				memset(&lpObj->m_BuffEffectList[i],0,sizeof(lpObj->m_BuffEffectList[i]));
			}
		}
		
		if(lpObj->m_BuffEffectList[i].btBuffIndex == iBuffIndex )
		{
			g_BuffEffect.ClearBuffEffect(lpObj,lpObj->m_BuffEffectList[i].btEffectType1,lpObj->m_BuffEffectList[i].iValue1);
			g_BuffEffect.ClearBuffEffect(lpObj,lpObj->m_BuffEffectList[i].btEffectType2,lpObj->m_BuffEffectList[i].iValue2);
			memset(&lpObj->m_BuffEffectList[i],0,sizeof(lpObj->m_BuffEffectList[i]));
			bIsRemoveEffect = 1;
		}
	}
	return bIsRemoveEffect;
}

bool CBuffEffectSlot::CheckUsedEffect(LPOBJ lpObj,int iBuffIndex)
{
	if( g_BuffScript.CheckVaildBuffEffect(iBuffIndex) == FALSE )
		return false;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(lpObj->m_BuffEffectList[i].btBuffIndex == iBuffIndex)
			return true;
	}
	return false;
}

void CBuffEffectSlot::RemoveBuffVariable(LPOBJ lpObj,int iBuffIndex)
{
	switch( iBuffIndex )
	{
	case AT_ICE:
		{
			lpObj->DelayActionTime = 0;
			lpObj->DelayLevel = 0;
			lpObj->m_IceType = 0;
		}
		break;
	}
}

int CBuffEffectSlot::ClearEffect(LPOBJ lpObj,enum eBuffClearType ClearType)
{
	LPBUFF_EFFECT_DATE lpBuffData = NULL;
	int iRemoveEffectCount = 0; //clear count
	
	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if( this->CheckUsedEffect(lpObj,lpObj->m_BuffEffectList[i].btBuffIndex) == TRUE )
		{
			lpBuffData = g_BuffScript.GetBuffData(lpObj->m_BuffEffectList[i].btBuffIndex);

			if(lpBuffData == NULL)
				continue;

			if(lpBuffData->btClearType == ClearType)
			{
				if( this->RemoveEffect(lpObj, lpObj->m_BuffEffectList[i].btBuffIndex) == TRUE)
				{
					iRemoveEffectCount += 1;
					i -= 1;
				}
			}

			if(ClearType == CLEAR_TYPE_LOGOUT)
			{
				if( this->RemoveEffect(lpObj,lpObj->m_BuffEffectList[i].btBuffIndex ) == TRUE)
				{
					iRemoveEffectCount += 1;
					i -= 1;
				}
			}

			if(ClearType == CLEAR_TYPE_DIEREGEN)
			{
				if( this->RemoveEffect(lpObj,lpObj->m_BuffEffectList[i].btBuffIndex) == TRUE)
				{
					iRemoveEffectCount += 1;
					i -= 1;
				}
			}
		}
	}
	return iRemoveEffectCount;
}

BUFF_EFFECT_DATE* CBuffEffectSlot::GetEffectData(int iBuffIndex)
{
	BUFF_EFFECT_DATE* lpBuffData = 0;

	if( !g_BuffScript.CheckVaildBuffEffect(iBuffIndex) )
		return NULL;

	lpBuffData = g_BuffScript.GetBuffData(iBuffIndex);
return lpBuffData;
}

BUFF_EFFECT_DATE* CBuffEffectSlot::GetEffectDataFromItemCode(WORD wItemCode)
{
	BUFF_EFFECT_DATE* lpBuffData = 0;

	lpBuffData = g_BuffScript.GetBuffDataFromItemCode(wItemCode);
	return lpBuffData;
}

int CBuffEffectSlot::GetBuffClearType(int iBuffIndex)
{
	BUFF_EFFECT_DATE* lpBuffData = 0;

	if( !g_BuffScript.CheckVaildBuffEffect(iBuffIndex) )
		return -1;

	lpBuffData = g_BuffScript.GetBuffData(iBuffIndex);
	return lpBuffData->btClearType;
}

void gObjCheckBuffEffectList(LPOBJ lpObj)
{
	if(lpObj == NULL)
		return;

	if(lpObj->m_Index < 0 || lpObj->m_Index > OBJMAX)
		return;

	if( (lpObj->Type == OBJ_USER && lpObj->Connected >= PLAYER_PLAYING) || lpObj->Type == OBJ_MONSTER)
	{
		for(int i = 0; i < MAX_STATE_COUNT; i++)
		{
			if(lpObj->m_BuffEffectList[i].iDuration == -10)
				continue;

			if(lpObj->m_BuffEffectList[i].btBuffIndex == NULL)
				continue;

			switch(lpObj->m_BuffEffectList[i].btBuffIndex)
			{
			case AT_POISON:
				{
					if((lpObj->m_BuffEffectList[i].iDuration % 3) == 0) //Rand type??
					{
						g_BuffEffect.SetActiveBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType1, lpObj->m_BuffEffectList[i].iValue1);
						g_BuffEffect.SetActiveBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType2, lpObj->m_BuffEffectList[i].iValue2);
					}
				}
				break;
			default:
				{
					g_BuffEffect.SetActiveBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType1, lpObj->m_BuffEffectList[i].iValue1);
					g_BuffEffect.SetActiveBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType2, lpObj->m_BuffEffectList[i].iValue2);
				}
				break;
			}
			
			lpObj->m_BuffEffectList[i].iDuration--;

			if(lpObj->m_BuffEffectList[i].iDuration <= 0)
			{
				if( lpObj->m_BuffEffectList[i].btBuffIndex == AT_RFVITALITY )
				{
					gObjRemoveBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btBuffIndex);
					gObjCalCharacter(lpObj->m_Index);
				}
				else
				{
					gObjRemoveBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btBuffIndex);
				}
			}
		}
	}
}

bool gObjAddBuffEffect(LPOBJ lpObj,int iBuffIndex)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->m_Index < 0 || lpObj->m_Index > OBJMAX)
		return false;

	int iItemCode = 0;
	LPITEMEFFECT lpItemEffectInfo = NULL;
	LPBUFF_EFFECT_DATE lpBuffData = NULL;

	lpBuffData = g_BuffEffectSlot.GetEffectData(iBuffIndex);

	if(lpBuffData == NULL)
		return false;

	iItemCode = (lpBuffData->btItemType << 9) + lpBuffData->btItemIndex;

	if(g_ItemAddOption.IsValidEffect(iItemCode) == FALSE)
		return false;

	lpItemEffectInfo = g_ItemAddOption.GetItemEffectData(iItemCode);

	int iRemoveBuffIndex = 0;

	iRemoveBuffIndex = g_BuffEffectSlot.SetEffect(lpObj, iBuffIndex, lpItemEffectInfo->wEffectType1, lpItemEffectInfo->wEffectType2, lpItemEffectInfo->iEffectValue1, lpItemEffectInfo->iEffectValue2, lpItemEffectInfo->iEffectValidTime);

	if(iRemoveBuffIndex != -1)
	{
		if(iRemoveBuffIndex > 0)
		{
			GCUseBuffEffect(lpObj, iRemoveBuffIndex, 2, lpItemEffectInfo->wOptionType, 0, 0);
		}
		else
		{
			lpObj->m_BuffEffectCount++;
		}

		GCUseBuffEffect(lpObj, iBuffIndex, 0, lpItemEffectInfo->wOptionType, lpItemEffectInfo->wEffectType1, lpItemEffectInfo->iEffectValidTime);
		return 1;
	}
	
	return 0;
}

bool gObjAddBuffEffect(LPOBJ lpObj,int iBuffIndex,BYTE EffectType1,int EffectValue1,BYTE EffectType2,int EffectValue2,int Duration)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->m_Index < 0 || lpObj->m_Index > OBJMAX)
		return false;

	LPBUFF_EFFECT_DATE lpBuffData = NULL;
	lpBuffData = g_BuffEffectSlot.GetEffectData(iBuffIndex);
	int iRemoveBuffIndex = 0;

	iRemoveBuffIndex = g_BuffEffectSlot.SetEffect(lpObj, iBuffIndex, EffectType1, EffectType2, EffectValue1, EffectValue2, Duration);

	if(iRemoveBuffIndex != -1)
	{
		if(iRemoveBuffIndex > 0)
		{
			GCUseBuffEffect(lpObj, iRemoveBuffIndex, 2, lpBuffData->btViewPortIndex, 0, 0);
		}
		else
		{
			lpObj->m_BuffEffectCount++;
		}

		GCUseBuffEffect(lpObj, iBuffIndex, 0, lpBuffData->btViewPortIndex, EffectType1, Duration);
		return true;
	}

	return false;
}

bool gObjAddBuffEffect(LPOBJ lpObj,int iBuffIndex,int Duration)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->m_Index < 0 || lpObj->m_Index > OBJMAX)
		return false;

	int iItemCode = 0;
	LPITEMEFFECT lpItemEffectInfo = NULL;
	LPBUFF_EFFECT_DATE lpBuffData = NULL;

	lpBuffData = g_BuffEffectSlot.GetEffectData(iBuffIndex);

	if(lpBuffData == NULL)
		return false;

	iItemCode = (lpBuffData->btItemType << 9) + lpBuffData->btItemIndex;

	if(g_ItemAddOption.IsValidEffect(iItemCode) == FALSE)
		return false;

	lpItemEffectInfo = g_ItemAddOption.GetItemEffectData(iItemCode);

	int iRemoveBuffIndex = 0;

	iRemoveBuffIndex = g_BuffEffectSlot.SetEffect(lpObj, iBuffIndex, lpItemEffectInfo->wEffectType1, lpItemEffectInfo->wEffectType2, lpItemEffectInfo->iEffectValue1, lpItemEffectInfo->iEffectValue2, Duration);

	if(iRemoveBuffIndex != -1)
	{
		if(iRemoveBuffIndex > 0)
		{
			GCUseBuffEffect(lpObj, iRemoveBuffIndex, 2, lpItemEffectInfo->wOptionType, 0, 0);
		}
		else
		{
			lpObj->m_BuffEffectCount++;
		}

		GCUseBuffEffect(lpObj, iBuffIndex, 0, lpItemEffectInfo->wOptionType, lpItemEffectInfo->wEffectType1, Duration);
		return true;
	}

	return false;
}

bool gObjAddBuffEffectForInGameShop(LPOBJ lpObj,WORD wItemCode,int iDuration)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->m_Index < 0 || lpObj->m_Index > OBJMAX)
		return false;

	int iItemCode = 0;
	LPITEMEFFECT lpItemEffectInfo = NULL;
	LPBUFF_EFFECT_DATE lpBuffData = NULL;

	lpBuffData = g_BuffEffectSlot.GetEffectDataFromItemCode(wItemCode);

	if(lpBuffData == NULL)
		return false;

	iItemCode = (lpBuffData->btItemType << 9) + lpBuffData->btItemIndex;

	if(g_ItemAddOption.IsValidEffect(iItemCode) == FALSE)
		return false;

	lpItemEffectInfo = g_ItemAddOption.GetItemEffectData(iItemCode);

	int iRemoveBuffIndex = 0;

	iRemoveBuffIndex = g_BuffEffectSlot.SetEffect(lpObj, lpBuffData->btIndex, lpItemEffectInfo->wEffectType1, lpItemEffectInfo->wEffectType2, lpItemEffectInfo->iEffectValue1, lpItemEffectInfo->iEffectValue2, iDuration);

	if(iRemoveBuffIndex != -1)
	{
		if(iRemoveBuffIndex > 0)
		{
			GCUseBuffEffect(lpObj, iRemoveBuffIndex, 2, lpItemEffectInfo->wOptionType, 0, 0);
		}
		else
		{
			lpObj->m_BuffEffectCount++;
			gObjCalCharacter(lpObj->m_Index);
		}

		GCUseBuffEffect(lpObj, lpBuffData->btIndex, 0, lpItemEffectInfo->wOptionType, lpItemEffectInfo->wEffectType1, iDuration);
		return true;
	}
	return false;
}

bool gObjRemoveBuffEffect(LPOBJ lpObj,int iBuffIndex)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->m_Index < 0 || lpObj->m_Index > OBJMAX)
		return false;

	LPBUFF_EFFECT_DATE lpBuffData = NULL;

	lpBuffData = g_BuffEffectSlot.GetEffectData(iBuffIndex);

	if(lpBuffData == NULL)
		return false;

	if( g_BuffEffectSlot.RemoveEffect(lpObj, iBuffIndex) == TRUE)
	{
		g_BuffEffectSlot.RemoveBuffVariable(lpObj, iBuffIndex);
		lpObj->m_BuffEffectCount--;
		GCUseBuffEffect(lpObj, iBuffIndex, 1, 0, lpBuffData->btViewPortIndex, 0);
		return 1;
	}
	return 0;
}

bool gObjClearBuffEffect(LPOBJ lpObj,enum eBuffClearType ClearType)
{
	if(lpObj == NULL)
		return 0;

	if(lpObj->m_Index < 0 || lpObj->m_Index > OBJMAX)
		return 0;

	int iRemoveBuffEffectCount = 0;
	int iBuffIndex = 0;
	
	switch(ClearType)
	{
	case CLEAR_TYPE_CASTLESIEGE:
		{
			for(iBuffIndex = 0; iBuffIndex < MAX_STATE_COUNT; iBuffIndex++)
			{
				switch(lpObj->m_BuffEffectList[iBuffIndex].btBuffIndex)
				{
				case AT_WATCHING_TOWER:
				case AT_ATTACKING_GUILD1:
				case AT_ATTACKING_GUILD2:
				case AT_ATTACKING_GUILD3:
					{
						if(gObjRemoveBuffEffect(lpObj, lpObj->m_BuffEffectList[iBuffIndex].btBuffIndex) == TRUE)
						{
							iRemoveBuffEffectCount++;
							iBuffIndex--;
						}
					}
					break;
				default:
					{
						break;
					}
					break;
				}
			}
		}
		break;
	case CLEAR_TYPE_PCBANG_SEAL_FINISH:
		{
			for(iBuffIndex = 0; iBuffIndex < MAX_STATE_COUNT; iBuffIndex++)
			{
				switch(lpObj->m_BuffEffectList[iBuffIndex].btBuffIndex)
				{
				case AT_PCBANG_SEAL_OF_ASCENSION:
				case AT_PCBANG_SEAL_OF_WEALTH:
				case AT_PCBANG_SEAL_OF_SUSTENANCE:
					{
						if(gObjRemoveBuffEffect(lpObj, lpObj->m_BuffEffectList[iBuffIndex].btBuffIndex) == TRUE)
						{
							iRemoveBuffEffectCount++;
							iBuffIndex--;
						}
					}
					break;
				default:
					{
						break;
					}
					break;
				}
			}
		}
		break;
	case CLEAR_TYPE_NON_PCS_ITEM_EFFECT:
		{
			for(iBuffIndex = 0; iBuffIndex < MAX_STATE_COUNT; iBuffIndex++)
			{
				switch(lpObj->m_BuffEffectList[iBuffIndex].btBuffIndex)
				{
				case AT_PCBANG_SEAL_OF_ASCENSION:
				case AT_PCBANG_SEAL_OF_WEALTH:
				case AT_PCBANG_SEAL_OF_SUSTENANCE:

				case AT_CSHOP_SEAL_OF_ASCENSION:
				case AT_CSHOP_SEAL_OF_WEALTH:
				case AT_CSHOP_SEAL_OF_SUSTENANCE:
				case AT_CSHOP_SEAL_OF_MOBILITY:

				case AT_CSHOP_SCROLL_OF_QUICKNESS:
				case AT_CSHOP_SCROLL_OF_DEFENSE:
				case AT_CSHOP_SCROLL_OF_WRATH:
				case AT_CSHOP_SCROLL_OF_WIZARDRY:
				case AT_CSHOP_SCROLL_OF_HEALTH:
				case AT_CSHOP_SCROLL_OF_MANA:

				case AT_CSHOP_LEAP_OF_STRENGTH:
				case AT_CSHOP_LEAP_OF_QUICKNESS:
				case AT_CSHOP_LEAP_OF_HEALTH:
				case AT_CSHOP_LEAP_OF_ENERGY:
				case AT_CSHOP_LEAP_OF_CONTROL:

				case AT_CSHOP_SEAL_OF_HEALING:
				case AT_CSHOP_SEAL_OF_DIVINITY:

				case AT_CSHOP_SCROLL_OF_BATTLE:
				case AT_CSHOP_SCROLL_OF_STRENGTHENER:

				case AT_CSHOP_GUARDIAN_AMULET:
				case AT_CSHOP_PROTECT_AMULET:
				case AT_CSHOP_MASTER_SEAL_OF_ASCENSION:
				case AT_CSHOP_MASTER_SEAL_OF_WEALTH:
				case 112:
				case 113:
				case 114:
				case 119:
				case 120:
				case 121:
				case 122:
				case 123:
				case 124:
				case 125:
				case 126:
				case 127:
				case 128:
					break;
				default:
					{
						if(gObjRemoveBuffEffect(lpObj, lpObj->m_BuffEffectList[iBuffIndex].btBuffIndex) == TRUE)
						{
							iRemoveBuffEffectCount++;
							iBuffIndex--;
						}
					}
					break;
				}
			}
		}
		break;
	case CLEAR_TYPE_LOGOUT:
		{
			for(iBuffIndex = 0; iBuffIndex < MAX_STATE_COUNT; iBuffIndex++)
			{
				if(gObjRemoveBuffEffect(lpObj, lpObj->m_BuffEffectList[iBuffIndex].btBuffIndex) == TRUE)
				{
					iRemoveBuffEffectCount++;
					iBuffIndex--;
				}
			}
			lpObj->m_BuffEffectCount = 0;
		}
		break;
	default:
		{
			iRemoveBuffEffectCount = g_BuffEffectSlot.ClearEffect(lpObj,ClearType);
		}
		break;
	}

	if(lpObj->m_BuffEffectCount < 0 || lpObj->m_BuffEffectCount > MAX_STATE_COUNT)
	{
		lpObj->m_BuffEffectCount = 0;
	}

	gObjSendBuffList(lpObj);
	return true;
}

bool gObjCheckUsedBuffEffect(LPOBJ lpObj,int iBuffIndex)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->m_Index < 0 || lpObj->m_Index > OBJMAX)
		return false;

	return g_BuffEffectSlot.CheckUsedEffect(lpObj,iBuffIndex);
}

bool gObjChangeBuffValidTime(LPOBJ lpObj,int iBuffIndex,int iTime)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->Connected < PLAYER_PLAYING)
		return false;

	DWORD iDuration = GetTickCount();

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(i > lpObj->m_BuffEffectCount)
			return false;

		if(lpObj->m_BuffEffectList[i].btBuffIndex == iBuffIndex)
		{
			lpObj->m_BuffEffectList[i].iDuration += iTime;

			if(lpObj->m_BuffEffectList[i].iDuration < 0)
			{
				gObjRemoveBuffEffect(lpObj, iBuffIndex);
			}
			return true;
		}
	}
	return false;
}

void gObjSetActiveEffectAtTick(LPOBJ lpObj)
{
	int EffectValue = 0;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(lpObj->m_BuffEffectList[i].btBuffIndex != NULL)
		{
			g_BuffEffect.SetActiveBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType1, lpObj->m_BuffEffectList[i].iValue1);
			g_BuffEffect.SetActiveBuffEffect(lpObj, lpObj->m_BuffEffectList[i].btEffectType2, lpObj->m_BuffEffectList[i].iValue2);
		}
	}
}

int gObjMakeViewportState(LPOBJ lpObj,BYTE* lpBuffer)
{
	if(lpObj == NULL)
		return -1;

	if(lpObj->Connected < PLAYER_PLAYING)
		return -1;

	int iBuffCount = 0;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(iBuffCount >= MAX_STATE_COUNT || iBuffCount >= lpObj->m_BuffEffectCount)
			break;

		if(lpObj->m_BuffEffectList[i].btBuffIndex == NULL)
			continue;

		switch(lpObj->m_BuffEffectList[i].btBuffIndex)
		{
		case 0:
			break;
		default:
			lpBuffer[iBuffCount] = lpObj->m_BuffEffectList[i].btBuffIndex;
			iBuffCount++;
			break;
		}		
	}
	return iBuffCount;
}

bool gObjCheckPowerfulEffect(LPOBJ lpObj,int iBuffIndex,int EffectValue1,int EffectValue2)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->Connected < PLAYER_PLAYING)
		return false;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(lpObj->m_BuffEffectList[i].btBuffIndex == iBuffIndex)
		{
			if(lpObj->m_BuffEffectList[i].iValue1 > EffectValue1)
			{
				return true;
			}

			if(lpObj->m_BuffEffectList[i].iValue2 > EffectValue2)
			{
				return true;
			}			
		}
	}
	return false;
}

int gObjGetTotalValueOfEffect(LPOBJ lpObj,int EffectType)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->Connected < PLAYER_PLAYING)
		return false;

	int iTotalValue = 0;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(lpObj->m_BuffEffectList[i].btBuffIndex == NULL)
			continue;

		if(lpObj->m_BuffEffectList[i].btEffectType1 == EffectType)
		{
			iTotalValue += lpObj->m_BuffEffectList[i].iValue1;
		}

		if(lpObj->m_BuffEffectList[i].btEffectType2 == EffectType)
		{
			iTotalValue += lpObj->m_BuffEffectList[i].iValue2;
		}
	}

	return iTotalValue;
}

bool gObjCheckUsedBuffEffect(LPOBJ lpObj,int iBuffIndex,int * EffectValue1,int * EffectValue2)
{
	if(lpObj == NULL)
		return false;

	if(lpObj->Connected < PLAYER_PLAYING)
		return false;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(lpObj->m_BuffEffectList[i].btBuffIndex == iBuffIndex)
		{
			*EffectValue1 = lpObj->m_BuffEffectList[i].iValue1;

			if(EffectValue2 != 0)
			{
				*EffectValue2 = lpObj->m_BuffEffectList[i].iValue2;
			}

			return true;
		}
	}

	return false;
}

void GCUseBuffEffect(LPOBJ lpObj, BYTE btEffectIndex, BYTE btEffectUseOption, WORD wOptionType, WORD wEffectType, int iLeftTime)
{
	PMSG_SEND_BUFF_ICON pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2D, sizeof(PMSG_SEND_BUFF_ICON));

	pMsg.btEffectIndex = btEffectIndex;
	pMsg.byEffectOption = btEffectUseOption;
	pMsg.wOptionType = wOptionType;
	pMsg.wEffectType = wEffectType;
	pMsg.iLeftTime = iLeftTime;

	if ( lpObj->Type == OBJ_USER )
	{
		DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	if(btEffectUseOption == 0)
	{
		GCBuffStateInfoSend(lpObj, 1, btEffectIndex);
	}
	else
	{
		GCBuffStateInfoSend(lpObj, 0, btEffectIndex);
	}
}

void gObjSendBuffList(LPOBJ lpObj)
{
	if(lpObj == NULL)
		return;

	if(lpObj->Type != OBJ_USER)
		return;

	if(lpObj->m_BuffEffectCount <= 0)
		return;

	for(int i = 0; i < MAX_STATE_COUNT; i++)
	{
		if(lpObj->m_BuffEffectList[i].btBuffIndex != NULL)
			GCBuffStateInfoSend(lpObj, 1, lpObj->m_BuffEffectList[i].btBuffIndex);
	}
}

int gObjAddBuffEffectWideArea(LPOBJ lpTargetObj,int nDistance,int & nAffectedCount,int iBuffIndex,BYTE EffectType1,int EffectValue1,BYTE EffectType2,int EffectValue2,int Duration)
{
	if(lpTargetObj == NULL)
		return FALSE;

	for(int iIndex = OBJ_STARTUSERINDEX; iIndex < OBJMAX; iIndex++)
	{
		LPOBJ lpObj = &gObj[iIndex];

		if(lpObj->Type == OBJ_USER && gObjIsConnected(iIndex) != FALSE)
		{
			if(lpObj->MapNumber == lpTargetObj->MapNumber)
			{
				if(gObjCalDistance(lpTargetObj,lpObj) < nDistance)
				{
					gObjAddBuffEffect(lpObj, iBuffIndex, EffectType1, EffectValue1, EffectType2, EffectValue2, Duration);
					nAffectedCount+= 1;
				}
			}
		}
	}
	return TRUE;
}

void CheckItemOptForGetExpEx(LPOBJ lpObj, __int64 & iExp, int bRewardExp)
{
	__int64 iRetExp = iExp;
	int iAddExpRate = 100;

	if ( gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_SEAL_OF_SUSTENANCE) || gObjCheckUsedBuffEffect(lpObj, AT_PCBANG_SEAL_OF_SUSTENANCE) )
	{
		iRetExp = 0;
	}
	else if (
		 gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_SEAL_OF_ASCENSION)
      || gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_SEAL_OF_WEALTH)
      || gObjCheckUsedBuffEffect(lpObj, AT_PCBANG_SEAL_OF_ASCENSION)
      || gObjCheckUsedBuffEffect(lpObj, AT_PCBANG_SEAL_OF_WEALTH)
      || gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_SEAL_OF_HEALING)
      || gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_MASTER_SEAL_OF_ASCENSION)
      || gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_MASTER_SEAL_OF_WEALTH) )
    {
		iAddExpRate = gObjGetTotalValueOfEffect(lpObj, ADD_OPTION_EXPERIENCE);
		
		if( g_MasterLevelSystem.IsMasterLevelUser(lpObj) )
		{
			if ( gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_MASTER_SEAL_OF_ASCENSION) || gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_MASTER_SEAL_OF_WEALTH) )
			{
				if ( bRewardExp )
					iAddExpRate = 100;
			}
			else
			{
				iAddExpRate = 100;
			}
		}
		else
		{
			if ( gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_MASTER_SEAL_OF_ASCENSION) || gObjCheckUsedBuffEffect(lpObj, AT_CSHOP_MASTER_SEAL_OF_WEALTH) )
			{
				iAddExpRate = 100;
			}
			else
			{
				if ( bRewardExp )
					iAddExpRate = 100;
			}
		}
		//iRetExp = iRetExp * iAddExpRate / 100;
    }
	else if( gObjCheckUsedBuffEffect(lpObj, AT_PARTY_EXP_BONUS) )
	{
        iAddExpRate = 190;

        if ( lpObj->PartyNumber >= 0 )
		{
			for (int i = 0; i < MAX_USER_IN_PARTY; ++i )
			{
				LPOBJ lpPartyObj = &gObj[gParty.m_PartyS[lpObj->PartyNumber].Number[i]]; // wz bug ? need to check for -1
				int iPartyDis = gObjCalDistance(lpObj, lpObj);
				
				if ( lpObj->m_Index != lpPartyObj->m_Index && lpObj->MapNumber == lpPartyObj->MapNumber && iPartyDis < 10 )
					iAddExpRate += 10;
			}
        }
       
		if( bRewardExp || g_MasterLevelSystem.IsMasterLevelUser(lpObj) )
			iAddExpRate = 100;
		
		//iRetExp = iRetExp * iAddExpRate / 100;
	}
	else if( gObjCheckUsedBuffEffect(lpObj, AT_NEWPVPSYSTEM_REWARD) )
	{
		if( g_MasterLevelSystem.IsMasterLevelUser(lpObj) )
		{
			return;
		}

        if ( g_NewPVP.IsVulcanusMap(lpObj->MapNumber) && iRetExp == iExp )
		{
			iExp = iRetExp * gObjGetTotalValueOfEffect(lpObj, 60) / 100 + iRetExp;
			return;
		}
	}

	if( lpObj->pInventory[8].IsItem() && lpObj->pInventory[8].m_Type == ITEMGET(13,80) //Panda Pet
		&& lpObj->pInventory[8].m_Durability > 0.0 && !lpObj->pInventory[8].IsPeriodItemExpire() )
	{
		if( gObjCheckUsedBuffEffect(lpObj,AT_CSHOP_SEAL_OF_SUSTENANCE) ||
			gObjCheckUsedBuffEffect(lpObj,AT_PCBANG_SEAL_OF_SUSTENANCE) )
		{
			iRetExp = 0;
		}
		else
		{
			if( bRewardExp )
			{
				iAddExpRate = 100;
			}
			else
			{
#ifdef __ALIEN__
				iAddExpRate += g_PandaExpPerc;
#else
				iAddExpRate += 50;
#endif
			}

			//iRetExp = iRetExp * iAddExpRate / 100;
		}
	}

	if( lpObj->pInventory[8].IsItem() && lpObj->pInventory[8].m_Type == ITEMGET(13, 123) //Skeleton Pet
		&& lpObj->pInventory[8].m_Durability > 0.0 && !lpObj->pInventory[8].IsPeriodItemExpire() )
	{
		if( gObjCheckUsedBuffEffect(lpObj,AT_CSHOP_SEAL_OF_SUSTENANCE) ||
			gObjCheckUsedBuffEffect(lpObj,AT_PCBANG_SEAL_OF_SUSTENANCE) )
		{
			iRetExp = 0;
		}
		else
		{
			if( bRewardExp )
			{
				iAddExpRate = 100;
			}
			else
			{
#ifdef __ALIEN__
				iAddExpRate += g_SkeletonExpPerc;
#else
				iAddExpRate += 30;
#endif
			}

			//iRetExp = iRetExp * iAddExpRate / 100;
		}
	}

	if( lpObj->pInventory[8].IsItem() && //Skeleton Pet + Ring
		lpObj->pInventory[8].m_Type == ITEMGET(13,123) &&
		lpObj->pInventory[8].m_Durability > 0.0 &&
		!lpObj->pInventory[8].IsPeriodItemExpire() && 
		( (lpObj->pInventory[10].m_Type == ITEMGET(13,122) && lpObj->pInventory[10].m_Durability > 0.0 && !lpObj->pInventory[10].IsPeriodItemExpire()) ||
		  (lpObj->pInventory[11].m_Type == ITEMGET(13,122) && lpObj->pInventory[11].m_Durability > 0.0 && !lpObj->pInventory[11].IsPeriodItemExpire()) )
		  )
	{
		if( gObjCheckUsedBuffEffect(lpObj,AT_CSHOP_SEAL_OF_SUSTENANCE) ||
			gObjCheckUsedBuffEffect(lpObj,AT_PCBANG_SEAL_OF_SUSTENANCE) )
		{
			iRetExp = 0;
		}
		else
		{
			if( bRewardExp )
			{
				iAddExpRate = 100;
			}
			else
			{
#ifdef __ALIEN__
				iAddExpRate += g_SkeletonRingExpPerc;
#else
				iAddExpRate += 30;
#endif
			}
		}
	}

	iRetExp = iRetExp * iAddExpRate / 100;
	iExp = iRetExp;
}