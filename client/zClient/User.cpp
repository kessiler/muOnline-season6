#include "stdafx.h"
#include "Import.h"
#include "User.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "Interface.h"
#include "Protocol.h"
#include "TrayMode.h"
#include "ConnectEx.h"
#include "VisualFix.h"
// ----------------------------------------------------------------------------------------------

User	gObjUser;
// ----------------------------------------------------------------------------------------------

void User::Load()
{
	ZeroMemory(&this->lpPlayer, sizeof(this->lpPlayer));
	ZeroMemory(&this->lpViewPlayer, sizeof(this->lpViewPlayer));
	ZeroMemory(&this->lpViewTarget, sizeof(this->lpViewTarget));
	ZeroMemory(this->m_BattleZoneList, sizeof(this->m_BattleZoneList));
	ZeroMemory(this->m_ResetItemCheck, sizeof(this->m_ResetItemCheck));
	// ----
	this->m_TargetUpdateTick	= 0;
	this->m_TargetType			= false;
	this->m_CursorX				= -1;
	this->m_CursorY				= -1;
	this->m_MapNumber			= -1;
	this->m_MasterLevel			= 0;
	this->m_MasterPoint			= 0;
	this->m_MasterExp			= 0;
	this->m_MasterNextExp		= 0;
	this->m_Reset				= 0;
	this->m_GrandReset			= 0;
#ifdef __NOVUS__
	this->m_TargetLifePercent	= 0.0f;
	this->m_CraftLevel			= 0;
	this->m_CraftStage			= 0;
	this->m_CraftQuest			= 0;
	ZeroMemory(this->m_CraftItemCheck, sizeof(this->m_CraftItemCheck));
#endif
	// ----
	SetOp((LPVOID)oIsBattleServer_Call1, (LPVOID)this->IsBattleServer, ASM::CALL);
	SetOp((LPVOID)oIsBattleServer_Call2, (LPVOID)this->IsBattleServer, ASM::CALL);
	// ----
	SetOp((LPVOID)0x0047FE31, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x0059890D, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x0059A399, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x0059A93C, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x0059B13B, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x005A4261, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x005A4D52, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x005B6117, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x0065A6CE, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x00727837, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x007860A1, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x007D31AA, (LPVOID)this->IsBattleZone, ASM::CALL);
	SetOp((LPVOID)0x007D39BD, (LPVOID)this->IsBattleZone, ASM::CALL);
	// ----
	SetOp((LPVOID)0x0077F9A9, (LPVOID)this->IsMasterClass, ASM::CALL);
	SetOp((LPVOID)0x0064A85F, (LPVOID)this->IsMasterClass, ASM::CALL);
	SetOp((LPVOID)0x0064AC02, (LPVOID)this->IsMasterClass, ASM::CALL);
	SetOp((LPVOID)0x0063DDF2, (LPVOID)this->IsMasterClass, ASM::CALL);
	SetOp((LPVOID)0x0063DF89, (LPVOID)this->IsMasterClass, ASM::CALL);
	SetOp((LPVOID)0x0063EA4C, (LPVOID)this->IsMasterClass, ASM::CALL);
	SetOp((LPVOID)0x0064A79A, (LPVOID)this->IsMasterClass, ASM::CALL);
	SetOp((LPVOID)0x0064AB3D, (LPVOID)this->IsMasterClass, ASM::CALL);
	SetOp((LPVOID)0x008105CB, (LPVOID)this->IsMasterClass, ASM::CALL);
	SetOp((LPVOID)0x0081064B, (LPVOID)this->IsMasterClass, ASM::CALL);
	// ----
	SetOp((LPVOID)0x005B96E8, (LPVOID)this->DrawPartyHP, ASM::CALL);
	// ----
#ifdef __NOVUS__
	SetOp((LPVOID)0x00588511, (LPVOID)this->SetEventEffect, ASM::CALL);
#endif
}
// ----------------------------------------------------------------------------------------------

void User::Refresh()
{
	this->lpPlayer			= &*(ObjectCharacter*)oUserObjectStruct;
	this->lpViewPlayer		= &*(ObjectPreview*)oUserPreviewStruct;
	this->GetTarget();
	// ----
	this->m_CursorX			= pCursorX;
	this->m_CursorY			= pCursorY;
	this->m_MapNumber		= pMapNumber;
	this->m_MasterLevel		= pMasterLevel;
	this->m_MasterPoint		= pMasterPoint;
	this->m_MasterExp		= pMasterExp;
	this->m_MasterNextExp	= pMasterNextExp;
	// ----
	if( (gObjUser.lpPlayer->Class & 7) != 6 )
	{
		SetByte((PVOID)(0x00556C32 + 6), 2);
	}
	else
	{
		SetByte((PVOID)(0x00556C32 + 6), 15);
	}
	// ----
	if( gTrayMode.InTray )
	{
		if( !this->lpViewPlayer->m_Model.Unknown4 && gConnectEx.m_ConnectState == 0 )
		{
			gTrayMode.ShowMessage(NIIF_WARNING, "MU Online", "Your character has been killed");
		}
	}
}
// ----------------------------------------------------------------------------------------------

bool User::GetTarget()
{
	if( pViewNPCTargetID != -1 )
	{
		this->lpViewTarget	= &*(ObjectPreview*)pGetPreviewStruct(pPreviewThis(), pViewNPCTargetID);
		this->m_TargetType	= 1;
		return true;
	}
	else if( pViewAttackTargetID != -1 )
	{
		this->lpViewTarget	= &*(ObjectPreview*)pGetPreviewStruct(pPreviewThis(), pViewAttackTargetID);
		this->m_TargetType	= 2;
		return true;
	}
	// ----
	if( this->lpViewTarget != 0 )
	{
		ZeroMemory(&this->lpViewTarget, sizeof(this->lpViewTarget));
	}
	// ----
	this->m_TargetType = 0;
	return false;
}
// ----------------------------------------------------------------------------------------------

int User::GetActiveSkill()
{
	return this->lpPlayer->pMagicList[this->lpViewPlayer->ActiveMagic];
}
// ----------------------------------------------------------------------------------------------

void User::UpdateCharInfo(CHAR_UPDATEINFO * aRecv)
{
	this->lpPlayer->Strength		= aRecv->Strength;
	this->lpPlayer->Dexterity		= aRecv->Dexterity;
	this->lpPlayer->Vitality		= aRecv->Vitality;
	this->lpPlayer->Energy			= aRecv->Energy;
	this->lpPlayer->Leadership		= aRecv->Leadership;
	this->lpPlayer->LevelPoint		= aRecv->LevelUpPoint;
	gVisualFix.UpPoint				= aRecv->LevelUpPoint;
}
// ----------------------------------------------------------------------------------------------

void User::SetBattleMapData(PMSG_BATTLE_LIST * aRecv)
{
	SetByte((PVOID)pMaxBattleZoneCount, MAX_BATTLE_LIST);
	// ----
	for( int i = 0; i < MAX_BATTLE_LIST; i++ )
	{
		SetByte((PVOID)((oBattleMapStart + 7 * i) + 3), aRecv->BattleMapList[i]);
	}
	// ----
	for( int i = 0; i < MAX_BATTLE_LIST; i++ )
	{
		this->m_BattleZoneList[i] = aRecv->BattleZoneList[i];
	}
}
// ----------------------------------------------------------------------------------------------

bool User::IsBattleZone(int MapNumber)
{
	if( gObjUser.IsBattleServer() )
	{
		for( int i = 0; i < MAX_BATTLE_LIST; i++ )
		{
			if( gObjUser.m_BattleZoneList[i] == MapNumber )
			{
				return true;
			}
		}
	}
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

void User::SetTargetData(PMSG_TARGETDATA_ANS * aRecv)
{
	this->m_TargetLifePercent	= 0.0f;
	this->m_TargetLifePercent	= aRecv->TargetLifePercent;
	this->m_TargetUpdateTick	= GetTickCount();
}
// ----------------------------------------------------------------------------------------------

void User::SetEventEffect(int PreviewStruct)
{
	//SetByte((PVOID)(0x004EFF0D + 2), 3);	//Min event effect number
	// ----
	lpViewObj lpPreview = &*(ObjectPreview*)PreviewStruct;
	// ----
	if( lpPreview->CtlCode == 32 )
	{
		if( *(DWORD*)(PreviewStruct + 672) && lpPreview->Unknown23 != 5 )
		{
			pInitEventStatus(PreviewStruct);
		}
		// ----
		lpPreview->Unknown23 = 5;
	}
	// ----
	if( *(DWORD*)(PreviewStruct + 672) != 0 || !lpPreview->Unknown23 )
	{
		return;
	}
	// ----
	switch(lpPreview->Unknown23)
	{
	case 5:	//-> GM
		{
			if( pEventEffectThis(668) )
			{
				DWORD ModelID = 349;
				// ----
				if( !strncmp(lpPreview->Name, "[GM]", 4) )
				{
					ModelID = 406;
				}
				else if( !strncmp(lpPreview->Name, "[EM]", 4) )
				{
					ModelID = 407;
				}
				else if( !strcmp(lpPreview->Name, "Admin") )
				{
					ModelID = 408;
				}
				// ----
				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect(pEventEffectThis(668), ModelID, 20, 1, 70.0, -5.0, 0.0, 0.0, 0.0, 45.0);
			}
		}
		break;
		// --
	case 6:
		{
			if( pEventEffectThis(668) )
			{
				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 0, 20, 120.0, 0.0, 0.0);
			}
		}
		break;
		// --
	case 7:
		{
			if( pEventEffectThis(668) )
			{
				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 1, 20, 120.0, 0.0, 0.0);
			}
		}
		break;
		// --
	case 8:
		{
			if( pEventEffectThis(668) )
			{
				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 2, 20, 120.0, 0.0, 0.0);
			}
		}
		break;
		// --
	case 9:
		{
			if( pEventEffectThis(668) )
			{
				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 3, 20, 120.0, 0.0, 0.0);
			}
		}
		break;
		// --
	case 10:
		{
			if( pEventEffectThis(668) )
			{
				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 4, 20, 120.0, 0.0, 0.0);
			}
		}
		break;
		// --
	case 11:
		{
			if( pEventEffectThis(668) )
			{
				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 5, 20, 120.0, 0.0, 0.0);
			}
		}
		break;
		// --
	case 12:
		{
			if( pEventEffectThis(668) )
			{
				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 6, 20, 120.0, 0.0, 0.0);
			}
		}
		break;
		// --
	case 13:
		{
			if( pEventEffectThis(668) )
			{
				*(LPVOID*)(PreviewStruct + 672) = pAddEventEffect2(pEventEffectThis(668), 32241, 7, 20, 120.0, 0.0, 0.0);
			}
		}
		break;
		// --
	default:
		{
			lpPreview->Unknown23 = 0;
		}
		break;
	}
}
// ----------------------------------------------------------------------------------------------

bool User::IsMasterClass(BYTE Class)
{
	gObjUser.Refresh();
	// ----
	if( pIsMaster(Class) && gObjUser.lpPlayer->Level == 400 )
	{
		return true;
	}
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

void User::DrawPartyHP()
{
	if( pPartyMemberCount <= 0 )
	{
		return;
	}
	// ----
	float LifeBarWidth = 38.0f;
	char LifeDisplay[20];
	VAngle Angle;
	int PosX, PosY, LifeProgress;
	// ----
	for( int PartySlot = 0; PartySlot < pPartyMemberCount; PartySlot++ )
	{
		PartyList PartyMember	= *(PartyList*)((char*)&pPartyListStruct + sizeof(PartyList) * PartySlot);
		lpViewObj lpPartyObj	= &*(ObjectPreview*)pGetPreviewStruct(pPreviewThis(), PartyMember.ViewportID);
		// ----
		if( !lpPartyObj )
		{
			continue;
		}
		// ----
		Angle.X = lpPartyObj->m_Model.VecPosX;
		Angle.Y = lpPartyObj->m_Model.VecPosY;
		Angle.Z = lpPartyObj->m_Model.VecPosZ + lpPartyObj->m_Model.Unknown216.Z + 100.0;
		// ----
		pGetPosFromAngle(&Angle, &PosX, &PosY);
		PosX -= (int)floor(LifeBarWidth / (double)2.0);	//T_T
		// ----
		if(		pCursorX >= PosX
			&&	(float)pCursorX <= (float)PosX + LifeBarWidth
			&&	pCursorY >= PosY - 2 
			&&	pCursorY < PosY + 6 )
		{
			sprintf(LifeDisplay, "HP : %d0%%", PartyMember.LifePercent);
			pSetTextColor(pTextThis(), 0xFF, 0xE6, 0xD2, 0xFF);
			pDrawText(pTextThis(), PosX, PosY - 6, LifeDisplay, 0, 0, (LPINT)1, 0);
		}
		// ----
		pSetBlend(true);
		// ----
		glColor4f(0.0, 0.0, 0.0, 0.5);
		pDrawBarForm((float)(PosX + 1), (float)(PosY + 1), LifeBarWidth + 4.0, 5.0, 0.0, 0);
		pGLSwitchBlend();
		// ----
		glColor3f(0.2, 0.0, 0.0);
		pDrawBarForm((float)PosX, (float)PosY, LifeBarWidth + 4.0, 5.0, 0.0, 0);
		// ----
		glColor3f(0.19607843, 0.039215688, 0.0);
		pDrawBarForm((float)(PosX + 2), (float)(PosY + 2), LifeBarWidth, 1.0, 0.0, 0);
		//-> Very strange method, maybe will be cool LifePercent / 10? :D
		if( PartyMember.LifePercent > 10 )	
		{
			LifeProgress = 10;
		}
		else
		{
			LifeProgress = PartyMember.LifePercent;
		}
		// ----
		glColor3f(0.98039216, 0.039215688, 0.0);
		// ----
		for( int i = 0; i < LifeProgress; i++ )
        {
			pDrawBarForm((float)(i * 4 + PosX + 2), (float)(PosY + 2), 3.0, 2.0, 0.0, 0);
        }
		// ----
		pGLSwitch();
	}
	// ----
	pGLSwitch();
    glColor3f(1.0, 1.0, 1.0);
}
// ----------------------------------------------------------------------------------------------

void User::DrawSome()
{
	char LifeDisplay[20];
	VAngle Angle;
	int PosX, PosY, LifeProgress;
	// ----
	float LifeBarWidth	= 38.0f;
	lpViewObj lpObj		= gObjUser.lpViewTarget;
	// ----
	if( !lpObj || lpObj->m_Model.ObjectType != emMonster || !lpObj->m_Model.Unknown4 )
	{
		return;
	}
	// ----
	Angle.X = lpObj->m_Model.VecPosX;
	Angle.Y = lpObj->m_Model.VecPosY;
	Angle.Z = lpObj->m_Model.VecPosZ + lpObj->m_Model.Unknown216.Z + 100.0;
	// ----
	pGetPosFromAngle(&Angle, &PosX, &PosY);
	PosX -= (int)floor(LifeBarWidth / (double)2.0);
	// ----
	sprintf(LifeDisplay, "%s", lpObj->Name);
	pSetTextColor(pTextThis(), 0xFF, 0xE6, 0xD2, 0xFF);
	pDrawText(pTextThis(), PosX + 2, PosY - 6, LifeDisplay, 0, 0, (LPINT)1, 0);
	// ----
	pSetBlend(true);
	glColor4f(0.0, 0.0, 0.0, 0.5);
	pDrawBarForm((float)(PosX + 1), (float)(PosY + 1), LifeBarWidth + 4.0, 5.0, 0.0, 0);
	pGLSwitchBlend();
	// ----
	glColor3f(0.2, 0.0, 0.0);
	pDrawBarForm((float)PosX, (float)PosY, LifeBarWidth + 4.0, 5.0, 0.0, 0);
	// ----
	glColor3f(0.19607843, 0.039215688, 0.0);
	pDrawBarForm((float)(PosX + 2), (float)(PosY + 2), LifeBarWidth, 1.0, 0.0, 0);
	// ----
	if( (int)(gObjUser.m_TargetLifePercent / 10) > 10 )
	{
		LifeProgress = 10;
	}
	else
	{
		LifeProgress = (int)(gObjUser.m_TargetLifePercent / 10);
	}
	// ----
	glColor3f(0.98039216, 0.039215688, 0.0);
	// ----
	for( int i = 0; i < LifeProgress; i++ )
	{
		pDrawBarForm((float)(i * 4 + PosX + 2), (float)(PosY + 2), 3.0, 2.0, 0.0, 0);
	}
	// ----
	pGLSwitch();
    glColor3f(1.0, 1.0, 1.0);
}
// ----------------------------------------------------------------------------------------------