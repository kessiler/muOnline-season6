#include "stdafx.h"
#include "User.h"
#include "Interface.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "CheatGuard.h"
#include "Protocol.h"
#include "Monster.h"
#include "Camera.h"
#include "ResetSystem.h"
#include "CraftSystem.h"
#include "NewsBoard.h"
#include "ConnectEx.h"
// ----------------------------------------------------------------------------------------------

Interface	gInterface;
DWORD		CharacterInfoExtern_Buff;
char		CharacterInfoExtern_LevelBuff[40];
DWORD		StaffNameColor_Buff;
lpViewObj	StaffNameColor_lpView;
DWORD		AddSomeShine_Buff;
DWORD		AddSomeShine_Pointer;
// ----------------------------------------------------------------------------------------------

Interface::Interface()
{
	ZeroMemory(this->Data, sizeof(this->Data));
}
// ----------------------------------------------------------------------------------------------

Naked(CharacterInfoExtern)
{
	sprintf(CharacterInfoExtern_LevelBuff, "Level: %d | Reset: %d (%d)", 
		gObjUser.lpPlayer->Level, gObjUser.m_Reset, gObjUser.m_GrandReset);
	// ----
	_asm
	{
		lea eax, CharacterInfoExtern_LevelBuff
		push eax
		// ----
		mov CharacterInfoExtern_Buff, 0x0077FB85
		jmp CharacterInfoExtern_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(StaffNameColor)
{
	_asm
	{
		mov ecx, dword ptr ss:[ebp+0x10]
		mov StaffNameColor_Buff, ecx
	}
	// ----
	StaffNameColor_lpView = &*(ObjectPreview*)(*(DWORD*)(StaffNameColor_Buff + 668));
	// ----
	if( !strncmp(StaffNameColor_lpView->Name, "[EM]", 4) )
	{
		pSetTextColor(pTextThis(), 255, 60, 160, 255);
	}
	else if( !strncmp(StaffNameColor_lpView->Name, "Admin", 5) )
	{
		pSetTextColor(pTextThis(), 255, 175, 0, 255);
	}
	else
	{
		pSetTextColor(pTextThis(), 100, 250, 250, 255);
	}
	// ----
	_asm
	{
		mov StaffNameColor_Buff, 0x00598C9A
		jmp StaffNameColor_Buff
	}
}
// ----------------------------------------------------------------------------------------------

Naked(AddSomeShine)
{
	_asm
	{
		mov eax, dword ptr ds:[ecx + 0x30]
		mov AddSomeShine_Buff, eax
	}
	// ----
	if( AddSomeShine_Buff == 349 || AddSomeShine_Buff == 406
		|| AddSomeShine_Buff == 407 || AddSomeShine_Buff == 408 )
	{
		_asm
		{
			mov AddSomeShine_Buff, 0x005E4979
			jmp AddSomeShine_Buff
		}
	}
	else
	{
		_asm
		{
			mov AddSomeShine_Buff, 0x005E4A3C
			jmp AddSomeShine_Buff
		}
	}
}
// ----------------------------------------------------------------------------------------------

void Interface::Load()
{
	this->BindObject(eSAMPLEBUTTON, 0x7AA4, 16, 15, 175, 1);
	// ----
	this->BindObject(eRANK_MAIN, 0x7A5A, 222, 303, -1, -1);
	this->BindObject(eRANK_TITLE, 0x7A63, 230, 67, -1, -1);
	this->BindObject(eRANK_FRAME, 0x7A58, 230, 15, -1, -1);
	this->BindObject(eRANK_FOOTER, 0x7A59, 230, 50, -1, -1);
	this->BindObject(eRANK_BUTTON, 0x7A5E, 128, 29, -1, -1);
	this->BindObject(eRANK_DIV, 0x7A62, 223, 21, -1, -1);
	this->BindObject(eRANK_TAB1, 0x7AAD, 48, 22, -1, -1);
	this->BindObject(eRANK_TAB2, 0x7AAD, 48, 22, -1, -1);
	this->BindObject(eRANK_TAB3, 0x7AAD, 48, 22, -1, -1);
	this->BindObject(eRANK_TAB4, 0x7AAE, 72, 22, -1, -1);
	this->BindObject(eCAMERA_MAIN, 0x787A, 54, 18, 174, 0);
	this->BindObject(eCAMERA_BUTTON1, 0x787B, 16, 12, 175.5, 1);
	this->BindObject(eCAMERA_BUTTON2, 0x787C, 16, 12, 192.5, 1);
	this->BindObject(eCRAFT_MAIN, 0x7A5A, 222, 303, -1, -1);
	this->BindObject(eCRAFT_TITLE, 0x7A63, 230, 67, -1, -1);
	this->BindObject(eCRAFT_FRAME, 0x7A58, 230, 15, -1, -1);
	this->BindObject(eCRAFT_FOOTER, 0x7A59, 230, 50, -1, -1);
	this->BindObject(eCRAFT_DIV, 0x7A62, 223, 21, -1, -1);
	this->BindObject(eCRAFT_TAB1, 0x7AAD, 48, 22, -1, -1);
	this->BindObject(eCRAFT_TAB2, 0x7AAD, 48, 22, -1, -1);
	this->BindObject(eCRAFT_TAB3, 0x7AAD, 48, 22, -1, -1);
	this->BindObject(eCRAFT_TAB4, 0x7AAE, 72, 22, -1, -1);
	this->BindObject(eCRAFT_CLOSE, 0x7EC5, 36, 29, -1, -1);
	this->BindObject(eCRAFT_INFOBG, 0x7AA3, 170, 21, -1, -1);
	this->BindObject(eCRAFT_MONEYBG, 0x7A89, 170, 26, -1, -1);
	this->BindObject(eCRAFT_FINISH, 0x7A5E, 128, 29, -1, -1);
	this->BindObject(eRESET_MAIN, 0x7A5A, 222, 303, -1, -1);
	this->BindObject(eRESET_TITLE, 0x7A63, 230, 67, -1, -1);
	this->BindObject(eRESET_FRAME, 0x7A58, 230, 15, -1, -1);
	this->BindObject(eRESET_FOOTER, 0x7A59, 230, 50, -1, -1);
	this->BindObject(eRESET_DIV, 0x7A62, 223, 21, -1, -1);
	this->BindObject(eRESET_CLOSE, 0x7EC5, 36, 29, -1, -1);
	this->BindObject(eRESET_INFOBG, 0x7AA3, 170, 21, -1, -1);
	this->BindObject(eRESET_MONEYBG, 0x7A89, 170, 26, -1, -1);
	this->BindObject(eRESET_FINISH, 0x7A5E, 128, 29, -1, -1);
	this->BindObject(eNEWS_MAIN, 0x7A5A, 222, 303, -1, -1);
	this->BindObject(eNEWS_TITLE, 0x7A63, 230, 67, -1, -1);
	this->BindObject(eNEWS_FRAME, 0x7A58, 230, 15, -1, -1);
	this->BindObject(eNEWS_FOOTER, 0x7A59, 230, 50, -1, -1);
	this->BindObject(eNEWS_DIV, 0x7A62, 223, 21, -1, -1);
	this->BindObject(eNEWS_INFOBG, 0x787D, 170, 21, -1, -1);
	this->BindObject(eNEWS_CLOSE, 0x7EC5, 36, 29, -1, -1);
	this->BindObject(eNEWS_BACK, 0x7A5E, 128, 29, -1, -1);
#ifdef __MAKELO__
	this->BindObject(eOFFEXP_SWITCH, 32513, 57, 23, MAX_WIN_WIDTH-57, 0);
#endif
#ifdef __BEREZNUK__
	this->BindObject(eTIME, 0x787E, 131, 70, -10, 359);
	this->Data[eTIME].OnShow = true;
#endif
#ifdef __ROOT__
	this->BindObject(eQUEST_MAIN, 0x7A5A, 222, 303, -1, -1);
	this->BindObject(eQUEST_TITLE, 0x7A63, 230, 67, -1, -1);
	this->BindObject(eQUEST_FRAME, 0x7A58, 230, 15, -1, -1);
	this->BindObject(eQUEST_FOOTER, 0x7A59, 230, 50, -1, -1);
	this->BindObject(eQUEST_DIV, 0x7A62, 223, 21, -1, -1);
	this->BindObject(eQUEST_CLOSE, 0x7EC5, 36, 29, -1, -1);
#endif
	// ----
	SetOp((LPVOID)oAllowGUI_Call1, this->AllowGUI, ASM::CALL);
	SetOp((LPVOID)oAllowGUI_Call2, this->AllowGUI, ASM::CALL);
	SetOp((LPVOID)oDrawInterface_Call, this->Work, ASM::CALL);
	SetOp((LPVOID)oLoadSomeForm_Call, this->LoadImages, ASM::CALL);
	SetOp((LPVOID)0x00633FFB, this->LoadModels, ASM::CALL);
	SetRange((LPVOID)0x0077FB7E, 7, ASM::NOP);
	SetOp((LPVOID)0x0077FB7E, (LPVOID)CharacterInfoExtern, ASM::JMP);
#ifdef __NOVUS__
	SetRange((LPVOID)0x00598C7D, 29, ASM::NOP);
	SetOp((LPVOID)0x00598C7D, (LPVOID)StaffNameColor, ASM::JMP);
	SetRange((LPVOID)0x005E496C, 13, ASM::NOP);
	SetOp((LPVOID)0x005E496C, (LPVOID)AddSomeShine, ASM::JMP);
#endif
}
// ----------------------------------------------------------------------------------------------

void Interface::Work()
{
	gObjUser.Refresh();
	gCamera.Rotate();
	// ----
#if defined __BEREZNUK__ || __MIX__ || __REEDLAN__ || __MUANGEL__ || __WHITE__ || __MEGAMU__ || __VIRNET__
	gConnectEx.Run();
#endif
#ifdef __NOVUS__
	gInterface.DrawCraftWindow();
#endif
#ifdef __MAKELO__
	gInterface.DrawOffExpSwitch();
#endif
#ifdef __BEREZNUK__
	gInterface.DrawTime();
#endif
	// ----
	gInterface.DrawLifeBar();
	gInterface.DrawCameraUI();
	gInterface.DrawResetWindow();
	gInterface.DrawNewsWindow();
#ifdef __RMOS__
	gInterface.DrawQuestDialog();
#endif
	// ----
	pDrawInterface();
}
// ----------------------------------------------------------------------------------------------

void Interface::LoadImages()
{
	pLoadImage("Custom\\Interface\\CameraUI_BG.tga", 0x787A, 0x2601, 0x2900, 1, 0);
	pLoadImage("Custom\\Interface\\CameraUI_Switch.tga", 0x787B, 0x2601, 0x2900, 1, 0);
	pLoadImage("Custom\\Interface\\CameraUI_Reset.tga", 0x787C, 0x2601, 0x2900, 1, 0);
	pLoadImage("Custom\\Interface\\NewsBoard_Title.tga", 0x787D, 0x2601, 0x2900, 1, 0);
#ifdef __BEREZNUK__
	pLoadImage("Custom\\Interface\\TimeBar.tga", 0x787E, 0x2601, 0x2900, 1, 0);
#endif
#ifdef __MAKELO__
	pLoadImage("Interface\\newui_pcroom.tga", 32513, 0x2601, 0x2900, 1, 0);
#endif
	// ----
	pLoadSomeForm();
}
// ----------------------------------------------------------------------------------------------

void Interface::LoadModels()
{
#ifdef __NOVUS__
	pLoadModel(406, "Data\\Custom\\Skill\\", "sign_gm", -1);
	pLoadTexture(406, "Custom\\Skill\\", 0x2901, 0x2600, 1);
	pLoadModel(407, "Data\\Custom\\Skill\\", "sign_em", -1);
	pLoadTexture(407, "Custom\\Skill\\", 0x2901, 0x2600, 1);
	pLoadModel(408, "Data\\Custom\\Skill\\", "sign_admin", -1);
	pLoadTexture(408, "Custom\\Skill\\", 0x2901, 0x2600, 1);
#endif
	// ----
	pInitModelData2();
}
// ----------------------------------------------------------------------------------------------

void Interface::BindObject(short MonsterID, DWORD ModelID, float Width, float Height, float X, float Y)
{
	this->Data[MonsterID].EventTick	= 0;
	this->Data[MonsterID].OnClick	= false;
	this->Data[MonsterID].OnShow	= false;
	this->Data[MonsterID].ModelID	= ModelID;
	this->Data[MonsterID].Width		= Width;
	this->Data[MonsterID].Height	= Height;
	this->Data[MonsterID].X			= X;
	this->Data[MonsterID].Y			= Y;
	this->Data[MonsterID].MaxX		= X + Width;
	this->Data[MonsterID].MaxY		= Y + Height;
	this->Data[MonsterID].Attribute	= 0;
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawLifeBar()
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - gObjUser.m_TargetUpdateTick);
	// ----
#ifdef __NOVUS__
	if( gObjUser.m_TargetType != 2 )
	{
		return;
	}
#else
	if( gObjUser.m_TargetType != 2 )
	{
		return;
	}
	// ----
	if( gObjUser.lpViewTarget->m_Model.ObjectType != emMonster )
	{
		return;
	}
#endif
	// ----
	PMSG_TARGETDATA_REQ pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 7, sizeof(pRequest));
	pRequest.aIndex = gObjUser.lpViewTarget->aIndex;
	// ----
	if( Delay >= 10 )
	{
		gProtocol.DataSend((LPBYTE)&pRequest, pRequest.h.size);
	}
	// ----
	if( gObjUser.m_TargetLifePercent <= 0.0f )
	{
		return;
	}
	// ----
	float BarWidth	= (148.5f / 100.0f) * gObjUser.m_TargetLifePercent;
	// ----
	pDrawGUI(0x7B3F, (MAX_WIN_WIDTH / 2) - 75.5, 0, 151, 8);
	pDrawGUI(0x7B3F, (MAX_WIN_WIDTH / 2) - 75.5, 6.5, 151, 8);
	pDrawGUI(0x7B40, (MAX_WIN_WIDTH / 2) - 74, 8.3, BarWidth, 4);
	//pDrawColorButton(0x7B40, (MAX_WIN_WIDTH / 2) - 74, 8.3, BarWidth, 4, 0, 0, Color4f(255, 255, 0, 100 - ((BYTE)gObjUser.m_TargetLifePercent)));
	// ----
	this->DrawFormat(eOrange, (MAX_WIN_WIDTH / 2) - 74, 0.5, 148, 3, gObjUser.lpViewTarget->Name);
	this->DrawFormat(eGold, (MAX_WIN_WIDTH / 2) - 74, 6, 148, 3, "%d%% / 100%%", (int)gObjUser.m_TargetLifePercent);
#ifdef __NOVUS__
	this->DrawFormat(eGold, gObjUser.m_CursorX + 25, gObjUser.m_CursorY - 10, 100, 1, "%d / %d", (int)gObjUser.m_TargetLife, (int)gObjUser.m_TargetMaxLife);
#endif
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawSampleButton()
{
	float PosX = this->GetResizeX(eSAMPLEBUTTON);
	// ----
	if( this->CheckWindow(ObjWindow::CashShop) 
		|| this->CheckWindow(ObjWindow::FullMap) 
		|| this->CheckWindow(ObjWindow::SkillTree) 
		|| this->CheckWindow(MoveList) )
	{
		return;
	}
	// ----
	this->DrawGUI(eSAMPLEBUTTON, PosX, 1);
	// ----
	if( !IsWorkZone(eSAMPLEBUTTON) )
	{
		return;
	}
	// ----
	this->DrawToolTip(PosX - 5, 31, "Button tooltip");
	// ----
	if( this->Data[eSAMPLEBUTTON].OnClick )
	{
		this->DrawColoredGUI(eSAMPLEBUTTON, PosX, 1, pMakeColor(40, 20, 3, 130));
		return;
	}
	// ----
	this->DrawColoredGUI(eSAMPLEBUTTON, PosX, 1, pMakeColor(255, 204, 20, 130));
}
// ----------------------------------------------------------------------------------------------

void Interface::EventSampleButton(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - this->Data[eSAMPLEBUTTON].EventTick);
	// ----
	if( this->CheckWindow(CashShop) || this->CheckWindow(FullMap) 
		|| this->CheckWindow(SkillTree) || this->CheckWindow(MoveList) 
		|| !IsWorkZone(eSAMPLEBUTTON) )
	{
		return;
	}
	// ----
	if( Event == WM_LBUTTONDOWN )
	{
		this->Data[eSAMPLEBUTTON].OnClick = true;
		return;
	}
	// ----
	this->Data[eSAMPLEBUTTON].OnClick = false;
	// ----
	if( Delay < 500 )
	{
		return;
	}
	// ----
	this->Data[eSAMPLEBUTTON].EventTick = GetTickCount();
	// ----
	if( !this->Data[eRANK_MAIN].OnShow )
	{
		this->Data[eRANK_MAIN].OnShow = true;
	}
	else
	{
		this->Data[eRANK_MAIN].OnShow = false;
	}
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawCameraUI()
{
	float PosX = this->GetResizeX(eCAMERA_MAIN);
	// ----
	if( this->CheckWindow(CashShop)
		||	this->CheckWindow(SkillTree)
		||	this->CheckWindow(FullMap) 
		||	this->CheckWindow(MoveList) 
		||  (this->CheckWindow(Inventory) 
		&& this->CheckWindow(ExpandInventory) 
		&& this->CheckWindow(Store)) 
		||  (this->CheckWindow(Inventory) 
		&& this->CheckWindow(Warehouse) 
		&& this->CheckWindow(ExpandWarehouse)) )
	{
		return;
	}
	// ----
	this->DrawGUI(eCAMERA_MAIN, PosX, 0);
	this->DrawGUI(eCAMERA_BUTTON1, PosX + 0.5, 1);
	this->DrawGUI(eCAMERA_BUTTON2, PosX + 18.5, 1);
	// ----
	if( gCamera.IsActive )
	{
		this->DrawColoredGUI(eCAMERA_BUTTON1, PosX + 0.5, 1, eShinyGreen);
	}
	// ----
	if( IsWorkZone(eCAMERA_BUTTON1) )
	{
		if( gCamera.IsActive )
		{
			this->DrawToolTip(PosX + 0.5 - 5, 25, "Zoom: %02.f%%", gCamera.ZoomPercent);
		}
		else
		{
			this->DrawToolTip(PosX + 0.5 - 5, 25, "3D Camera [On|Off]");
		}
		// ----
		if( this->Data[eCAMERA_BUTTON1].OnClick )
		{
			this->DrawColoredGUI(eCAMERA_BUTTON1, PosX + 0.5, 1, pMakeColor(40, 20, 3, 130));
			return;
		}
		// ----
		this->DrawColoredGUI(eCAMERA_BUTTON1, PosX + 0.5, 1, pMakeColor(255, 204, 20, 200));
	}
	else if( IsWorkZone(eCAMERA_BUTTON2) )
	{
		this->DrawToolTip(PosX + 18.5 - 5, 25, "3D Camera [Reset]");
		// ----
		if( this->Data[eCAMERA_BUTTON2].OnClick )
		{
			this->DrawColoredGUI(eCAMERA_BUTTON2, PosX + 18.5, 1, pMakeColor(40, 20, 3, 130));
			return;
		}
		// ----
		this->DrawColoredGUI(eCAMERA_BUTTON2, PosX + 18.5, 1, pMakeColor(255, 204, 20, 200));
	}
}
// ----------------------------------------------------------------------------------------------

void Interface::EventCameraUI(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	// ----
	if( this->CheckWindow(CashShop)
		||	this->CheckWindow(SkillTree)
		||	this->CheckWindow(FullMap) 
		||	this->CheckWindow(MoveList) 
		||  (this->CheckWindow(Inventory) 
		&& this->CheckWindow(ExpandInventory) 
		&& this->CheckWindow(Store)) 
		||  (this->CheckWindow(Inventory) 
		&& this->CheckWindow(Warehouse) 
		&& this->CheckWindow(ExpandWarehouse)) )
	{
		return;
	}
	// ----
	if( IsWorkZone(eCAMERA_BUTTON1) )
	{
		DWORD Delay	= (CurrentTick - this->Data[eCAMERA_BUTTON1].EventTick);
		// ----
		if( Event == WM_LBUTTONDOWN )
		{
			this->Data[eCAMERA_BUTTON1].OnClick = true;
			return;
		}
		// ----
		this->Data[eCAMERA_BUTTON1].OnClick = false;
		// ----
		if( Delay < 500 )
		{
			return;
		}
		// ----
		this->Data[eCAMERA_BUTTON1].EventTick = GetTickCount();
		// ----
		gCamera.Switch();
	}
	else if( IsWorkZone(eCAMERA_BUTTON2) )
	{
		DWORD Delay	= (CurrentTick - this->Data[eCAMERA_BUTTON2].EventTick);
		// ----
		if( Event == WM_LBUTTONDOWN )
		{
			this->Data[eCAMERA_BUTTON2].OnClick = true;
			return;
		}
		// ----
		this->Data[eCAMERA_BUTTON2].OnClick = false;
		// ----
		if( Delay < 500 )
		{
			return;
		}
		// ----
		this->Data[eCAMERA_BUTTON2].EventTick = GetTickCount();
		// ----
		gCamera.Init();
	}
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawOffExpSwitch()
{
	if(	this->CheckWindow(CashShop)
		||	this->CheckWindow(SkillTree)
		||	this->CheckWindow(FullMap)  
		||  this->CheckWindow(Inventory) 
		||  this->CheckWindow(Character)
		||  this->CheckWindow(CommandWindow)
		||  this->CheckWindow(Party)
		||  this->CheckWindow(Guild) 
		||  this->CheckWindow(GensInfo) 
		|| gObjUser.lpViewPlayer->InSafeZone )
	{
		return;
	}
	// ----
	this->DrawGUI(eOFFEXP_SWITCH, MAX_WIN_WIDTH-57, 0);
	// ----
	if( IsWorkZone(eOFFEXP_SWITCH) )
	{
		this->DrawToolTip(MAX_WIN_WIDTH-55, 25, "Offline Mode");
		// ----
		if( this->Data[eOFFEXP_SWITCH].OnClick )
		{
			this->DrawColoredGUI(eOFFEXP_SWITCH, MAX_WIN_WIDTH-57, 0, pMakeColor(40, 20, 3, 130));
			return;
		}
		// ----
		this->DrawColoredGUI(eOFFEXP_SWITCH, MAX_WIN_WIDTH-57, 0, pMakeColor(255, 204, 20, 200));
	}
	// ----
	this->DrawFormat(eBrown, MAX_WIN_WIDTH-52, 8, 55, 1, "OffExp");
}
// ----------------------------------------------------------------------------------------------

void Interface::EventOffExpSwitch(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - this->Data[eOFFEXP_SWITCH].EventTick);
	// ----
	if(	!IsWorkZone(eOFFEXP_SWITCH)
		||	this->CheckWindow(CashShop)
		||	this->CheckWindow(SkillTree)
		||	this->CheckWindow(FullMap)  
		||  this->CheckWindow(Inventory) 
		||  this->CheckWindow(Character)
		||  this->CheckWindow(CommandWindow)
		||  this->CheckWindow(Party)
		||  this->CheckWindow(Guild)
		||  this->CheckWindow(GensInfo) 
		|| gObjUser.lpViewPlayer->InSafeZone )
		
	{
		return;
	}
	// ----
	if( Event == WM_LBUTTONDOWN )
	{
		this->Data[eOFFEXP_SWITCH].OnClick = true;
		return;
	}
	// ----
	this->Data[eOFFEXP_SWITCH].OnClick = false;
	// ----
	if( Delay < 500 )
	{
		return;
	}
	// ----
	this->Data[eOFFEXP_SWITCH].EventTick = GetTickCount();
	// ----
	OFFLINEATTACK_REQ pRequest;
	pRequest.h.set((LPBYTE)&pRequest, 0xFB, 11, sizeof(pRequest));
	pRequest.MagicNumber = gObjUser.GetActiveSkill();
	gProtocol.DataSend((LPBYTE)&pRequest, pRequest.h.size);
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawCraftWindow()
{
	if( !this->Data[eCRAFT_MAIN].OnShow )
	{
		return;
	}
	// ----
	float MainWidth			= 230.0;
	float MainHeight		= 313.0;
	float StartY			= 100.0;
	float StartX			= (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
	float MainCenter		= StartX + (MainWidth / 3);
	float ButtonX			= MainCenter - (29.0 / 2);
	DWORD TabTextColor[5]	= { eWhite180, eWhite180, eWhite180, eWhite180, eWhite180 };
	// ----
	this->DrawGUI(eCRAFT_MAIN, StartX, StartY + 2);
	this->DrawGUI(eCRAFT_TITLE, StartX, StartY);
	StartY = this->DrawRepeatGUI(eCRAFT_FRAME, StartX, StartY + 67.0, 13);
	this->DrawGUI(eCRAFT_FOOTER, StartX, StartY);
	this->DrawGUI(eCRAFT_TAB1, StartX + 5, 153);
	this->DrawGUI(eCRAFT_TAB2, StartX + 5 + this->Data[eCRAFT_TAB1].Width, 153);
	this->DrawGUI(eCRAFT_TAB3, StartX + 5 + this->Data[eCRAFT_TAB1].Width * 2, 153);
	this->DrawGUI(eCRAFT_TAB4, StartX + 5 + this->Data[eCRAFT_TAB1].Width * 3, 153);
	this->DrawGUI(eCRAFT_CLOSE, StartX + MainWidth - this->Data[eCRAFT_CLOSE].Width, this->Data[eCRAFT_TITLE].Height + this->Data[eCRAFT_CLOSE].Height + 2);
	// ----
	if( this->IsWorkZone(eCRAFT_CLOSE) )
	{
		this->DrawToolTip(this->Data[eCRAFT_CLOSE].X + 5, this->Data[eCRAFT_CLOSE].Y + 25, "Close");
	}
	// ----
	for( int i = eCRAFT_TAB1; i < eCRAFT_CLOSE + 1; i++ )
	{
		if( this->IsWorkZone(i) )
		{
			DWORD Color = eGray100;
			// ----
			if( this->Data[i].OnClick )
			{
				Color = eGray150;
			}
			// ----
			TabTextColor[i - eCRAFT_TAB1] = eWhite;
			this->DrawColoredGUI(i, this->Data[i].X, this->Data[i].Y, Color);
			break;
		}
	}
	// ----
	this->DrawFormat(eGold, StartX + 10, 110, 210, 3, "Craft Master");
	this->DrawFormat(eOrange, StartX + 10, 135, 210, 3, "Select category:");
	this->DrawFormat(TabTextColor[0], Data[eCRAFT_TAB1].X, Data[eCRAFT_TAB1].Y + 7, Data[eCRAFT_TAB1].Width, 3, "Main");
	this->DrawFormat(TabTextColor[1], Data[eCRAFT_TAB2].X, Data[eCRAFT_TAB2].Y + 7, Data[eCRAFT_TAB2].Width, 3, "Quest");
	this->DrawFormat(TabTextColor[2], Data[eCRAFT_TAB3].X, Data[eCRAFT_TAB3].Y + 7, Data[eCRAFT_TAB3].Width, 3, "Weekly");
	this->DrawFormat(TabTextColor[3], Data[eCRAFT_TAB4].X, Data[eCRAFT_TAB4].Y + 7, Data[eCRAFT_TAB4].Width, 3, "Reset");
	// ----
	if( this->Data[eCRAFT_MAIN].Attribute )
	{
		short Object = this->Data[eCRAFT_MAIN].Attribute + eCRAFT_TAB1 - 1;
		this->DrawColoredGUI(Object, this->Data[Object].X, this->Data[Object].Y, eGray150);
		// ----
		switch(this->Data[eCRAFT_MAIN].Attribute)
		{
		case 1:
			this->FillCraftTab1();
			break;
			// --
		case 2:
			this->FillCraftTab2();
			break;
			// --
		case 3:
			this->FillCraftTab3();
			break;
			// --
		case 4:
			this->FillCraftTab4();
			break;
		}
	}
	else
	{
		this->DrawFormat(eGold, StartX + 10, 195, 210, 3, "Hello dear %s!", gObjUser.lpPlayer->Name);
		this->DrawFormat(eWhite, StartX + 10, 205, 210, 3, "My name is Craft Master, i'm legendary teacher of craft.\nIf you make my first simple quest maybe i will take you to the my students.", gObjUser.lpPlayer->Name);
	}
	// ----
	this->DrawGUI(eCRAFT_DIV, StartX, 166);
}
// ----------------------------------------------------------------------------------------------

void Interface::FillCraftTab1()
{
	float MainWidth		= 230.0;
	float StartX		= (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
	// ----
	this->DrawGUI(eCRAFT_INFOBG, StartX + 30, 190);
	this->DrawFormat(eGold, StartX + 35, 195, 70, 3, "Level");
	this->DrawFormat(eWhite, StartX + 135, 195, 70, 1, "%d", gObjUser.m_CraftLevel);
	// ----
	this->DrawGUI(eCRAFT_INFOBG, StartX + 30, 210);
	this->DrawFormat(eGold, StartX + 35, 215, 70, 3, "Craft Rate");
	this->DrawFormat(eWhite, StartX + 135, 215, 70, 1, "+%d%%", gObjUser.m_CraftLevel);
	// ----
	this->DrawGUI(eCRAFT_INFOBG, StartX + 30, 230);
	this->DrawFormat(eGold, StartX + 35, 235, 70, 3, "Stage");
	this->DrawFormat(eWhite, StartX + 135, 235, 70, 1, "%d / %d", gObjUser.m_CraftStage, MAX_CRAFT_LEVEL);
	// ----
	this->DrawGUI(eCRAFT_INFOBG, StartX + 30, 250);
	this->DrawFormat(eGold, StartX + 35, 255, 70, 3, "Quest");
	this->DrawFormat(eWhite, StartX + 135, 255, 70, 1, "%d / %d", gObjUser.m_CraftQuest, gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestCount);
	// ----
	if( gCraftSystem.IsNotCrafter() )
	{
		this->DrawFormat(eOrange, StartX + 10, 280, 210, 3, "You do not come into the craft world!");
		this->DrawFormat(eWhite, StartX + 10, 290, 210, 3, "You can get first quest and start your career,\nevery craft level will give you bonus percent of success\ncreate item rate in chaos machine!");
	}
}
// ----------------------------------------------------------------------------------------------

void Interface::FillCraftTab2()
{
	float StartX		= (MAX_WIN_WIDTH / 2) - (230.0 / 2);
	DWORD ItemNameColor	= eWhite;
	float MainCenter	= StartX + (230.0 / 3);
	float ButtonX		= MainCenter - (34.0 / 2);
	// ----
	if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestCount <= 0 )
	{
		this->DrawFormat(eOrange, StartX, 185, 210, 3, "Sorry, but now i don't have weekly quest for you.");
		this->DrawFormat(eOrange, StartX, 195, 210, 3, "Try return a little later.");
		return;
	}
	// ----
	this->DrawFormat(eBlue, StartX, 183, 210, 3, "'%s'", gCraftSystem.m_StageData[gObjUser.m_CraftStage].StageName);
	this->DrawFormat(eYellow, StartX, 190, 210, 3, "Quest %d: '%s'", gObjUser.m_CraftQuest + 1, gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].QuestName);
	this->DrawGUI(eCRAFT_DIV, StartX, 195);
	this->DrawFormat(eGold, StartX + 30, 215, 210, 1, "Required Level:");
	// ----
	if( gObjUser.lpPlayer->Level < gCraftSystem.m_StageData[gObjUser.m_CraftStage].ReqLevel )
	{
		this->DrawFormat(eRed, StartX + 30, 225, 210, 1, "- %d (Your: %d)",
		gCraftSystem.m_StageData[gObjUser.m_CraftStage].ReqLevel, gObjUser.lpPlayer->Level);
	}
	else
	{
		this->DrawFormat(eWhite, StartX + 30, 225, 210, 1, "- %d",
		gCraftSystem.m_StageData[gObjUser.m_CraftStage].ReqLevel);
	}
	// ----
	this->DrawFormat(eGold, StartX + 30, 240, 210, 1, "Required Reset:");
	// ----
	if( gObjUser.m_Reset < gCraftSystem.m_StageData[gObjUser.m_CraftStage].ReqReset )
	{
		this->DrawFormat(eRed, StartX + 30, 250, 210, 1, "- %d (Your: %d)",
		gCraftSystem.m_StageData[gObjUser.m_CraftStage].ReqReset, gObjUser.m_Reset);
	}
	else
	{
		this->DrawFormat(eWhite, StartX + 30, 250, 210, 1, "- %d",
		gCraftSystem.m_StageData[gObjUser.m_CraftStage].ReqReset);
	}
	// ----
	this->DrawFormat(eGold, StartX + 30, 265, 210, 1, "Required Items:");
	// ----
	for( int i = 0; i < gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemCount; i++ )
	{
		char ItemNameBuff[150], LevelBuff[25], OptionBuff[25];
		// ----
		ZeroMemory(ItemNameBuff, sizeof(ItemNameBuff));
		// ----
		if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].IsCrafted )
		{
			strcat(ItemNameBuff, "Crafted ");
			ItemNameColor = eOrange;
		}
		// ----
		if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].IsExcellent )
		{
			strcat(ItemNameBuff, "Excellent ");
			ItemNameColor = eExcellent;
		}
		// ----
		if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].IsAncient )
		{
			strcat(ItemNameBuff, "Ancient ");
			ItemNameColor = eAncient;
		}
		// ----
		if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].IsSocket )
		{
			strcat(ItemNameBuff, "Socket ");
			ItemNameColor = eSocket;
		}
		// ----
		strcat(ItemNameBuff, (char*)(*(DWORD*)0x8128AC0 + 84 * 
			gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].ID));
		// ----
		if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MinLevel 
			== gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MaxLevel )
		{
			ZeroMemory(LevelBuff, sizeof(LevelBuff));
			sprintf(LevelBuff, " [+%d, ", gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MinLevel);
			strcat(ItemNameBuff, LevelBuff);
		}
		else
		{
			ZeroMemory(LevelBuff, sizeof(LevelBuff));
			sprintf(LevelBuff, " [+%d~%d, ", 
				gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MinLevel,
				gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MaxLevel);
			strcat(ItemNameBuff, LevelBuff);
		}
		// ----
		if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MinOption 
			== gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MaxOption )
		{
			ZeroMemory(OptionBuff, sizeof(OptionBuff));
			sprintf(OptionBuff, "+%d] ", gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MinOption * 4);
			strcat(ItemNameBuff, OptionBuff);
		}
		else
		{
			ZeroMemory(OptionBuff, sizeof(OptionBuff));
			sprintf(OptionBuff, "+%d~%d] ", 
				gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MinOption * 4,
				gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].MaxOption * 4);
			strcat(ItemNameBuff, OptionBuff);
		}
		// ----
		if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].IsLuck == 0 )
		{
			strcat(ItemNameBuff, "+ Without Luck ");
		}
		else if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].IsLuck == 1 )
		{
			strcat(ItemNameBuff, "+ Luck ");
		}
		// ----
		if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].IsSkill == 0 )
		{
			strcat(ItemNameBuff, "+ Without Skill");
		}
		else if( gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ItemData[i].IsSkill == 1 )
		{
			strcat(ItemNameBuff, "+ Skill");
		}
		// ----
		if( !gObjUser.m_CraftItemCheck[i] )
		{
			pDrawGUI(0x7BC4, StartX + 22.0, 276 + (i * 8), 7.0, 7.0);
			//this->DrawFormat(eRed, StartX + 15, 275 + (i * 8), 210, 1, "[NO]");
		}
		else
		{
			pDrawGUI(0x7BC1, StartX + 22.0, 276 + (i * 8), 7.0, 7.0);
			//this->DrawFormat(eExcellent, StartX + 15, 275 + (i * 8), 210, 1, "[OK]");
		}
		// ----
		this->DrawFormat(ItemNameColor, StartX + 30, 275 + (i * 8), 210, 1, ItemNameBuff);
	}
	// ----
	this->DrawFormat(eGold, StartX + 30, 320, 210, 1, "Required Zen:");
	this->DrawGUI(eCRAFT_MONEYBG, StartX + 30, 330);
	// ----
	char MoneyBuff[50], MoneyBuff2[50];
	ZeroMemory(MoneyBuff, sizeof(MoneyBuff));
	ZeroMemory(MoneyBuff2, sizeof(MoneyBuff2));
	// ----
	if( !gCraftSystem.CheckQuestReq() )
	{
		pGetMoneyFormat((double)gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ReqMoney, 
			MoneyBuff, 0);
		pGetMoneyFormat((double)gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ReqMoney - gObjUser.lpPlayer->MoneyInventory, 
			MoneyBuff2, 0);
		this->DrawFormat(eRed, StartX + 70, 334, 170, 1, "%s\n(Need still %s)", 
			MoneyBuff, MoneyBuff2);
	}
	else
	{
		pGetMoneyFormat((double)gCraftSystem.m_StageData[gObjUser.m_CraftStage].QuestData[gObjUser.m_CraftQuest].ReqMoney, 
			MoneyBuff, 0);
		this->DrawFormat(eGold, StartX + 70, 337, 70, 1, "%s", 
			MoneyBuff);
	}
	// ----
	this->DrawGUI(eCRAFT_FINISH, ButtonX, this->Data[eCRAFT_FOOTER].Y + 10);
	this->DrawFormat(eWhite, StartX + 8, this->Data[eCRAFT_FOOTER].Y + 20, 210, 3, "Finish Quest");
	// ----
	this->DrawGUI(eCRAFT_DIV, StartX, this->Data[eCRAFT_FOOTER].Y - 10);
	// ----
	if( !gCraftSystem.CheckAllReq() )
	{
		this->Data[eCRAFT_FINISH].Attribute = false;
		this->DrawColoredGUI(eCRAFT_FINISH, this->Data[eCRAFT_FINISH].X, this->Data[eCRAFT_FINISH].Y, eGray150);
		return;
	}
	// ----
	this->Data[eCRAFT_FINISH].Attribute = true;
	// ----
	if( IsWorkZone(eCRAFT_FINISH) )
	{
		DWORD Color = eGray100;
		// ----
		if( this->Data[eCRAFT_FINISH].OnClick )
		{
			Color = eGray150;
		}
		// ----
		this->DrawColoredGUI(eCRAFT_FINISH, this->Data[eCRAFT_FINISH].X, this->Data[eCRAFT_FINISH].Y, Color);
	}
}
// ----------------------------------------------------------------------------------------------

void Interface::FillCraftTab3()
{
	float StartX = (MAX_WIN_WIDTH / 2) - (230.0 / 2);
	// ----
	this->DrawFormat(eOrange, StartX, 185, 210, 3, "Sorry, but now i don't have weekly quest for you.");
	this->DrawFormat(eOrange, StartX, 195, 210, 3, "Try return a little later.");
}
// ----------------------------------------------------------------------------------------------

void Interface::FillCraftTab4()
{
	float StartX = (MAX_WIN_WIDTH / 2) - (230.0 / 2);
	// ----
	this->DrawFormat(eOrange, StartX, 185, 210, 3, "Sorry, but now i can't reset your career.");
	this->DrawFormat(eOrange, StartX, 195, 210, 3, "Try return a little later.");
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventCraftWindow_Main(DWORD Event)
{
	this->EventCraftWindow_TabControl(Event);
	this->EventCraftWindow_Close(Event);
	this->EventCraftWindow_Finish(Event);
	return true;
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventCraftWindow_TabControl(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - this->Data[eCRAFT_MAIN].EventTick);
	BYTE TabType		= 0;
	short Object		= 0;
	// ----
	for( int i = eCRAFT_TAB1; i < eCRAFT_TAB4 + 1; i++ )
	{
		if( IsWorkZone(i) )
		{
			TabType = i - eCRAFT_TAB1 + 1;
			Object	= i;
			break;
		}
	}
	// ----
	if( !this->Data[eCRAFT_MAIN].OnShow || !TabType )
	{
		return false;
	}
	// ----
	if( Event == WM_LBUTTONDOWN )
	{
		this->Data[Object].OnClick = true;
		return true;
	}
	// ----
	this->Data[Object].OnClick = false;
	// ----
	if( Delay < 100 )
	{
		return false;
	}
	// ----
	this->Data[eCRAFT_MAIN].EventTick = GetTickCount();
	this->Data[eCRAFT_MAIN].Attribute = TabType;
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventCraftWindow_Close(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - this->Data[eCRAFT_CLOSE].EventTick);
	// ----
	if( !this->Data[eCRAFT_MAIN].OnShow || !IsWorkZone(eCRAFT_CLOSE) )
	{
		return false;
	}
	// ----
	if( Event == WM_LBUTTONDOWN )
	{
		this->Data[eCRAFT_CLOSE].OnClick = true;
		return true;
	}
	// ----
	this->Data[eCRAFT_CLOSE].OnClick = false;
	// ----
	if( Delay < 500 )
	{
		return false;
	}
	// ----
	this->Data[eCRAFT_CLOSE].EventTick = GetTickCount();
	this->CloseCraftWindow();
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventCraftWindow_Finish(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - this->Data[eCRAFT_FINISH].EventTick);
	// ----
	if( !this->Data[eCRAFT_MAIN].OnShow || !IsWorkZone(eCRAFT_FINISH) 
		|| !this->Data[eCRAFT_FINISH].Attribute )
	{
		return false;
	}
	// ----
	if( Event == WM_LBUTTONDOWN )
	{
		this->Data[eCRAFT_FINISH].OnClick = true;
		return true;
	}
	// ----
	this->Data[eCRAFT_FINISH].OnClick = false;
	// ----
	if( Delay < 500 )
	{
		return false;
	}
	// ----
	this->Data[eCRAFT_FINISH].EventTick = GetTickCount();
	gCraftSystem.ReqQuestFinish();
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawResetWindow()
{
	if( !this->Data[eRESET_MAIN].OnShow )
	{
		return;
	}
	// ----
	DWORD ItemNameColor		= eWhite;
	float MainWidth			= 230.0;
	float MainHeight		= 313.0;
	float StartY			= 100.0;
	float StartX			= (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
	float MainCenter		= StartX + (MainWidth / 3);
	float ButtonX			= MainCenter - (29.0 / 2);
	// ----
	this->DrawGUI(eRESET_MAIN, StartX, StartY + 2);
	this->DrawGUI(eRESET_TITLE, StartX, StartY);
	StartY = this->DrawRepeatGUI(eRESET_FRAME, StartX, StartY + 67.0, 13);
	this->DrawGUI(eRESET_FOOTER, StartX, StartY);
	this->DrawGUI(eRESET_CLOSE, StartX + MainWidth - this->Data[eRESET_CLOSE].Width, this->Data[eRESET_TITLE].Height + this->Data[eRESET_CLOSE].Height + 2);
	// ----
	if( this->IsWorkZone(eRESET_CLOSE) )
	{
		DWORD Color = eGray100;
		// ----
		if( this->Data[eRESET_CLOSE].OnClick )
		{
			Color = eGray150;
		}
		// ----
		this->DrawColoredGUI(eRESET_CLOSE, this->Data[eRESET_CLOSE].X, this->Data[eRESET_CLOSE].Y, Color);
		this->DrawToolTip(this->Data[eRESET_CLOSE].X + 5, this->Data[eRESET_CLOSE].Y + 25, "Close");
	}
	// ----
	this->DrawFormat(eGold, StartX + 10, 110, 210, 3, "Reset Master");
	// ----
	this->DrawGUI(eRESET_INFOBG, StartX + 30, 140);
	this->DrawFormat(eGold, StartX + 35, 145, 70, 3, "Next Reset:");
	this->DrawFormat(eWhite, StartX + 135, 145, 70, 1, "%d", gObjUser.m_Reset + 1);
	this->DrawGUI(eRESET_INFOBG, StartX + 30, 160);
	this->DrawFormat(eGold, StartX + 35, 165, 70, 3, "Reward Points:");
	this->DrawFormat(eWhite, StartX + 135, 165, 70, 1, "%d", gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].RewardData[gObjUser.lpPlayer->Class&7].LevelPoint * (gObjUser.m_Reset + 1));
	// ----
	this->DrawGUI(eRESET_DIV, StartX, 185);
	// ----
	this->DrawFormat(eGold, StartX + 30, 215, 210, 1, "Required Level:");
	// ----
	if( gObjUser.lpPlayer->Level < gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ReqLevel )
	{
		this->DrawFormat(eRed, StartX + 30, 225, 210, 1, "- %d (Your: %d)",
		gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ReqLevel, gObjUser.lpPlayer->Level);
	}
	else
	{
		this->DrawFormat(eWhite, StartX + 30, 225, 210, 1, "- %d",
		gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ReqLevel);
	}
	// ----
	this->DrawFormat(eGold, StartX + 30, 240, 210, 1, "Required Items:");
	// ----
	for( int i = 0; i < gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemCount; i++ )
	{
		char ItemNameBuff[150], LevelBuff[25], OptionBuff[25];
		// ----
		ZeroMemory(ItemNameBuff, sizeof(ItemNameBuff));
		// ----
		if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].IsExcellent )
		{
			strcat(ItemNameBuff, "Excellent ");
			ItemNameColor = eExcellent;
		}
		// ----
		if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].IsAncient )
		{
			strcat(ItemNameBuff, "Ancient ");
			ItemNameColor = eAncient;
		}
		// ----
		if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].IsSocket )
		{
			strcat(ItemNameBuff, "Socket ");
			ItemNameColor = eSocket;
		}
		// ----
		strcat(ItemNameBuff, (char*)(*(DWORD*)0x8128AC0 + 84 * 
			gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].ID));
		// ----
		if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MinLevel 
			== gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MaxLevel )
		{
			ZeroMemory(LevelBuff, sizeof(LevelBuff));
			sprintf(LevelBuff, " [+%d, ", gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MinLevel);
			strcat(ItemNameBuff, LevelBuff);
		}
		else
		{
			ZeroMemory(LevelBuff, sizeof(LevelBuff));
			sprintf(LevelBuff, " [+%d~%d, ", 
				gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MinLevel,
				gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MaxLevel);
			strcat(ItemNameBuff, LevelBuff);
		}
		// ----
		if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MinOption 
			== gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MaxOption )
		{
			ZeroMemory(OptionBuff, sizeof(OptionBuff));
			sprintf(OptionBuff, "+%d] ", gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MinOption * 4);
			strcat(ItemNameBuff, OptionBuff);
		}
		else
		{
			ZeroMemory(OptionBuff, sizeof(OptionBuff));
			sprintf(OptionBuff, "+%d~%d] ", 
				gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MinOption * 4,
				gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].MaxOption * 4);
			strcat(ItemNameBuff, OptionBuff);
		}
		// ----
		if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].IsLuck == 0 )
		{
			strcat(ItemNameBuff, "+ Without Luck ");
		}
		else if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].IsLuck == 1 )
		{
			strcat(ItemNameBuff, "+ Luck ");
		}
		// ----
		if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].IsSkill == 0 )
		{
			strcat(ItemNameBuff, "+ Without Skill");
		}
		else if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemData[i].IsSkill == 1 )
		{
			strcat(ItemNameBuff, "+ Skill");
		}
		// ----
		if( !gObjUser.m_ResetItemCheck[i] )
		{
			pDrawGUI(0x7BC4, StartX + 22.0, 251 + (i * 8), 7.0, 7.0);
		}
		else
		{
			pDrawGUI(0x7BC1, StartX + 22.0, 251 + (i * 8), 7.0, 7.0);
		}
		// ----
		this->DrawFormat(ItemNameColor, StartX + 30, 250 + (i * 8), 210, 1, ItemNameBuff);
	}
	// ----
	if( gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ItemCount <= 0 )
	{
		this->DrawFormat(eWhite, StartX + 30, 250, 210, 1, "- None");
	}
	// ----
	this->DrawFormat(eGold, StartX + 30, 295, 210, 1, "Required Zen:");
	this->DrawGUI(eCRAFT_MONEYBG, StartX + 30, 305);
	// ----
	char MoneyBuff[50], MoneyBuff2[50];
	ZeroMemory(MoneyBuff, sizeof(MoneyBuff));
	ZeroMemory(MoneyBuff2, sizeof(MoneyBuff2));
	// ----
	if( gObjUser.lpPlayer->MoneyInventory 
		< gResetSystem.m_GroupData[gResetSystem.GetResetGroup()].ReqMoney )
	{
		pGetMoneyFormat((double)gResetSystem.GetResetMoney(), 
			MoneyBuff, 0);
		pGetMoneyFormat((double)gResetSystem.GetResetMoney() - gObjUser.lpPlayer->MoneyInventory, 
			MoneyBuff2, 0);
		this->DrawFormat(eRed, StartX + 70, 309, 170, 1, "%s\n(Need still %s)", 
			MoneyBuff, MoneyBuff2);
	}
	else
	{
		pGetMoneyFormat((double)gResetSystem.GetResetMoney(), 
			MoneyBuff, 0);
		this->DrawFormat(eGold, StartX + 70, 312, 70, 1, "%s", 
			MoneyBuff);
	}
	// ----
	this->DrawFormat(eRed, StartX + 10, 340, 210, 3, "Warning!", MoneyBuff, MoneyBuff2);
	this->DrawFormat(eBlue, StartX + 10, 348, 210, 3, "Before make reset you need remove all items from inventory", MoneyBuff, MoneyBuff2);
	// ----
	this->DrawGUI(eRESET_FINISH, ButtonX, this->Data[eRESET_FOOTER].Y + 10);
	this->DrawFormat(eWhite, StartX + 8, this->Data[eRESET_FOOTER].Y + 20, 210, 3, "Make Reset");
	this->DrawGUI(eRESET_DIV, StartX, this->Data[eRESET_FOOTER].Y - 10);
	// ----
	if( !gResetSystem.CheckAllReq() )
	{
		this->Data[eRESET_FINISH].Attribute = false;
		this->DrawColoredGUI(eRESET_FINISH, this->Data[eRESET_FINISH].X, this->Data[eRESET_FINISH].Y, eGray150);
		return;
	}
	// ----
	this->Data[eRESET_FINISH].Attribute = true;
	// ----
	if( IsWorkZone(eRESET_FINISH) )
	{
		DWORD Color = eGray100;
		// ----
		if( this->Data[eRESET_FINISH].OnClick )
		{
			Color = eGray150;
		}
		// ----
		this->DrawColoredGUI(eRESET_FINISH, this->Data[eRESET_FINISH].X, this->Data[eRESET_FINISH].Y, Color);
	}
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventResetWindow_Main(DWORD Event)
{
	this->EventResetWindow_Close(Event);
	this->EventResetWindow_Finish(Event);
	return true;
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventResetWindow_Close(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - this->Data[eRESET_CLOSE].EventTick);
	// ----
	if( !this->Data[eRESET_MAIN].OnShow || !IsWorkZone(eRESET_CLOSE) )
	{
		return false;
	}
	// ----
	if( Event == WM_LBUTTONDOWN )
	{
		this->Data[eRESET_CLOSE].OnClick = true;
		return true;
	}
	// ----
	this->Data[eRESET_CLOSE].OnClick = false;
	// ----
	if( Delay < 500 )
	{
		return false;
	}
	// ----
	this->Data[eRESET_CLOSE].EventTick = GetTickCount();
	this->CloseResetWindow();
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventResetWindow_Finish(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - this->Data[eRESET_FINISH].EventTick);
	// ----
	if( !this->Data[eRESET_MAIN].OnShow || !IsWorkZone(eRESET_FINISH) 
		|| !this->Data[eRESET_FINISH].Attribute )
	{
		return false;
	}
	// ----
	if( Event == WM_LBUTTONDOWN )
	{
		this->Data[eRESET_FINISH].OnClick = true;
		return true;
	}
	// ----
	this->Data[eRESET_FINISH].OnClick = false;;
	// ----
	if( Delay < 500 )
	{
		return false;
	}
	// ----
	this->Data[eRESET_FINISH].EventTick = GetTickCount();
	gResetSystem.ReqResetFinish();
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawNewsWindow()
{
	if( !this->Data[eNEWS_MAIN].OnShow )
	{
		return;
	}
	// ----
	float MainWidth			= 230.0;
	float MainHeight		= 313.0;
	float StartY			= 100.0;
	float StartX			= (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
	float MainCenter		= StartX + (MainWidth / 3);
	float ButtonX			= MainCenter - (29.0 / 2);
	// ----
	this->DrawGUI(eNEWS_MAIN, StartX, StartY + 2);
	this->DrawGUI(eNEWS_TITLE, StartX, StartY);
	StartY = this->DrawRepeatGUI(eNEWS_FRAME, StartX, StartY + 67.0, 13);
	this->DrawGUI(eNEWS_FOOTER, StartX, StartY);
	this->DrawGUI(eNEWS_CLOSE, StartX + MainWidth - this->Data[eNEWS_CLOSE].Width, this->Data[eNEWS_TITLE].Height + this->Data[eNEWS_CLOSE].Height + 2);
	// ----
	if( this->IsWorkZone(eNEWS_CLOSE) )
	{
		DWORD Color = eGray100;
		// ----
		if( this->Data[eNEWS_CLOSE].OnClick )
		{
			Color = eGray150;
		}
		// ----
		this->DrawColoredGUI(eNEWS_CLOSE, this->Data[eNEWS_CLOSE].X, this->Data[eNEWS_CLOSE].Y, Color);
		this->DrawToolTip(this->Data[eNEWS_CLOSE].X + 5, this->Data[eNEWS_CLOSE].Y + 25, "Close");
	}
	// ----
	this->DrawFormat(eGold, StartX + 10, 110, 210, 3, "News Board");
	// ----
	if( this->Data[eNEWS_MAIN].Attribute == 0 )
	{
		for( int i = 0; i < gNewsBoard.m_LoadedCount; i++ )
		{
			this->DrawGUI(eNEWS_INFOBG, StartX + 30, 140 + (i * 21));
			// ----
			if( IsWorkZone(StartX + 30, 140 + (i * 21), StartX + 200, 160 + (i * 21)) )
			{
				this->DrawColoredGUI(eNEWS_INFOBG, StartX + 30,  140 + (i * 21), pMakeColor(255, 204, 20, 200));
			}
			// ----
			this->DrawFormat(eGold, StartX + 18, 142 + (i * 21), 65, 3, gNewsBoard.m_Data[i].Title.Date);
			this->DrawFormat(eOrange, StartX + 18, 148 + (i * 21), 65, 3, gNewsBoard.m_Data[i].Title.Time);
			this->DrawFormat(eWhite, StartX + 80, 145 + (i * 21), 190, 1, gNewsBoard.m_Data[i].Title.Text);
		}
		// ----
		this->DrawFormat(eBlue, StartX + 8, 130, 210, 3, "Last refresh: %d min. ago", gNewsBoard.m_LastRefreshMin);
	}
	else
	{
		BYTE ID = this->Data[eNEWS_MAIN].Attribute - 10;
		// ----
		this->DrawGUI(eNEWS_INFOBG, StartX + 30, 140);
		this->DrawFormat(eGold, StartX + 18, 142, 65, 3, gNewsBoard.m_Data[ID].Title.Date);
		this->DrawFormat(eOrange, StartX + 18, 148, 65, 3, gNewsBoard.m_Data[ID].Title.Time);
		this->DrawFormat(eWhite, StartX + 80, 145, 190, 1, gNewsBoard.m_Data[ID].Title.Text);
		this->DrawFormat(eWhite, StartX + 15, 170, 200, 1, gNewsBoard.m_Data[ID].Text);
		// ----
		this->DrawGUI(eNEWS_BACK, ButtonX, this->Data[eNEWS_FOOTER].Y + 10);
		this->DrawFormat(eWhite, StartX + 8, this->Data[eNEWS_FOOTER].Y + 20, 210, 3, "Back");
		this->DrawGUI(eRESET_DIV, StartX, this->Data[eNEWS_FOOTER].Y - 10);
		// ----
		if( IsWorkZone(eNEWS_BACK) )
		{
			DWORD Color = eGray100;
			// ----
			if( this->Data[eNEWS_BACK].OnClick )
			{
				Color = eGray150;
			}
			// ----
			this->DrawColoredGUI(eNEWS_BACK, this->Data[eNEWS_BACK].X, this->Data[eNEWS_BACK].Y, Color);
		}
		// ----
		this->DrawFormat(eBlue, StartX + 8, 130, 210, 3, "Last page refresh: %d min. ago", gNewsBoard.m_Data[ID].LastRefreshMin);
	}
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventNewsWindow_Main(DWORD Event)
{
	float MainWidth			= 230.0;
	float MainHeight		= 313.0;
	float StartY			= 100.0;
	float StartX			= (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
	// ----
	for( int i = 0; i < gNewsBoard.m_LoadedCount; i++ )
	{
		if( this->IsWorkZone(StartX + 30, 140 + (i * 21), StartX + 200, 161 + (i * 21)) )
		{
			if( Event == WM_LBUTTONDOWN && this->Data[eNEWS_MAIN].Attribute == 0 )
			{
				this->Data[eNEWS_MAIN].Attribute = i + 10;
				gNewsBoard.ReqOpenItem(i);
			}
		}
	}
	// ----
	this->EventNewsWindow_Close(Event);
	this->EventNewsWindow_Back(Event);
	// ----
	return true;
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventNewsWindow_Close(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - this->Data[eNEWS_CLOSE].EventTick);
	// ----
	if( !this->Data[eNEWS_MAIN].OnShow || !IsWorkZone(eNEWS_CLOSE) )
	{
		return false;
	}
	// ----
	if( Event == WM_LBUTTONDOWN )
	{
		this->Data[eNEWS_CLOSE].OnClick = true;
		return true;
	}
	// ----
	this->Data[eNEWS_CLOSE].OnClick = false;
	// ----
	if( Delay < 500 )
	{
		return false;
	}
	// ----
	this->Data[eNEWS_CLOSE].EventTick	= GetTickCount();
	this->Data[eNEWS_MAIN].Attribute	= 0;
	this->CloseNewsWindow();
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

bool Interface::EventNewsWindow_Back(DWORD Event)
{
	DWORD CurrentTick	= GetTickCount();
	DWORD Delay			= (CurrentTick - this->Data[eNEWS_BACK].EventTick);
	// ----
	if( !this->Data[eNEWS_MAIN].OnShow || !IsWorkZone(eNEWS_BACK) )
	{
		return false;
	}
	// ----
	if( Event == WM_LBUTTONDOWN )
	{
		this->Data[eNEWS_BACK].OnClick = true;
		return true;
	}
	// ----
	this->Data[eNEWS_BACK].OnClick = false;;
	// ----
	if( Delay < 500 )
	{
		return false;
	}
	// ----
	this->Data[eNEWS_BACK].EventTick = GetTickCount();
	this->Data[eNEWS_MAIN].Attribute = 0;
	// ----
	return false;
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawQuestDialog()
{
	if( !this->Data[eQUEST_MAIN].OnShow )
	{
		return;
	}
	// ----
	float MainWidth			= 230.0;
	float MainHeight		= 313.0;
	float StartY			= 100.0;
	float StartX			= (MAX_WIN_WIDTH / 2) - (MainWidth / 2);
	float MainCenter		= StartX + (MainWidth / 3);
	float ButtonX			= MainCenter - (29.0 / 2);
	// ----
	this->DrawGUI(eQUEST_MAIN, StartX, StartY + 2);
	this->DrawGUI(eQUEST_TITLE, StartX, StartY);
	StartY = this->DrawRepeatGUI(eQUEST_FRAME, StartX, StartY + 67.0, 13);
	this->DrawGUI(eQUEST_FOOTER, StartX, StartY);
	this->DrawGUI(eQUEST_CLOSE, StartX + MainWidth - this->Data[eQUEST_CLOSE].Width, this->Data[eQUEST_TITLE].Height + this->Data[eQUEST_CLOSE].Height + 2);
	// ----
	if( this->IsWorkZone(eQUEST_CLOSE) )
	{
		DWORD Color = eGray100;
		// ----
		if( this->Data[eQUEST_CLOSE].OnClick )
		{
			Color = eGray150;
		}
		// ----
		this->DrawColoredGUI(eQUEST_CLOSE, this->Data[eQUEST_CLOSE].X, this->Data[eQUEST_CLOSE].Y, Color);
		this->DrawToolTip(this->Data[eQUEST_CLOSE].X + 5, this->Data[eQUEST_CLOSE].Y + 25, "Close");
	}
	// ----
	this->DrawFormat(eGold, StartX + 10, 110, 210, 3, "%s", "Help save new year!");
	int LineY = this->DrawFormat(eWhite, StartX + 30, 145, 210, 1, "Hello valiant hero %s!", gObjUser.lpPlayer->Name);
	LineY = this->DrawFormat(eWhite, StartX + 30, LineY, 210, 1, "I have a unique quest for you!");
	LineY = this->DrawFormat(eWhite, StartX + 30, LineY, 210, 1, "For start new mission please select episode from list,");
	LineY = this->DrawFormat(eWhite, StartX + 30, LineY, 210, 1, "each episode will be rewarded with nice prizes.");
	// ----
	this->DrawGUI(eQUEST_DIV, StartX, 195);
	this->DrawFormat(eGold, StartX + 30, 215, 210, 1, "N");
	this->DrawFormat(eGold, StartX + 50, 215, 210, 1, "Level");
	this->DrawFormat(eGold, StartX + 90, 215, 210, 1, "Reset");
	this->DrawFormat(eGold, StartX + 140, 215, 210, 1, "Episode");
	// ----
	for( int i = 0; i < 10; i++ )
	{
		this->DrawFormat(eWhite, StartX + 30, 230 + (12 * i), 210, 1, "(%d)", i + 1);
		pDrawGUI(0x7B5E, StartX + 30, 230 + (12 * i) + 9, 82.0, 2.0);
		pDrawGUI(0x7B5E, StartX + 30 + 82, 230 + (12 * i) + 9, 82.0, 2.0);
		this->DrawFormat(eWhite, StartX + 50, 230 + (12 * i), 210, 1, "[%d~%d]", 1, 80);
		this->DrawFormat(eWhite, StartX + 90, 230 + (12 * i), 210, 1, "[%d~%d]", 0, 1000);
		this->DrawFormat(eWhite, StartX + 140, 230 + (12 * i), 210, 1, "%s", "Holiday treats");
	}
	// ----
	this->DrawGUI(eQUEST_DIV, StartX, this->Data[eQUEST_FOOTER].Y - 15);
	// ----
	this->DrawFormat(eGold, StartX + 30, this->Data[eQUEST_FOOTER].Y + 7, 160, 3, "Page: %d / %d", 0, 1);
	pDrawGUI(0x7BAA, StartX + 30, this->Data[eQUEST_FOOTER].Y + 5, 17, 18);
	pDrawGUI(0x7BAB, StartX + 178, this->Data[eQUEST_FOOTER].Y + 5, 17, 18);
}
// ----------------------------------------------------------------------------------------------

void Interface::FillQuestDialog1()
{

}
// ----------------------------------------------------------------------------------------------

void Interface::DrawTime()
{
	if( !this->Data[eTIME].OnShow )
	{
		return;
	}
	// ----
	if( this->CheckWindow(ObjWindow::ChatWindow) || this->CheckWindow(ObjWindow::CashShop)
		|| this->CheckWindow(ObjWindow::FullMap) || this->CheckWindow(ObjWindow::SkillTree)
		|| this->CheckWindow(ObjWindow::MoveList) || gObjUser.m_MapNumber == 34 || gObjUser.m_MapNumber == 30 )
	{
		return;
	}
	// ----
	this->DrawGUI(eTIME, this->Data[eTIME].X, this->Data[eTIME].Y);
	// -----
	time_t TimeServer, TimeLocal;
	struct tm * ServerT, * LocalT;
	time(&TimeServer);
	time(&TimeLocal);
	// ----
	ServerT = gmtime(&TimeServer);
	// ----
	char ServerTimeName[25] = "Server:";
	char ServerTime[30];
	sprintf(ServerTime, "%2d:%02d:%02d", (ServerT->tm_hour+2)%24, ServerT->tm_min, ServerT->tm_sec);
	// -----
	LocalT = localtime(&TimeLocal); 
	// -----
	char LocalTimeName[25] = "Local:";
	char LocalTime[30];
	sprintf(LocalTime, "%2d:%02d:%02d", LocalT->tm_hour, LocalT->tm_min, LocalT->tm_sec);
	// -----
	this->DrawFormat(eGold, 5, 391, 50, 1, ServerTimeName);
	this->DrawFormat(eWhite, 55, 391, 100, 1, ServerTime);
	// ----
	this->DrawFormat(eGold, 5, 413, 50, 1, LocalTimeName);
	this->DrawFormat(eWhite, 55, 413, 100, 1, LocalTime);
}
// ----------------------------------------------------------------------------------------------

bool Interface::CheckWindow(int WindowID)
{
	return pCheckWindow(pWindowThis(), WindowID);
}
// ----------------------------------------------------------------------------------------------

int Interface::CloseWindow(int WindowID)
{
	return pCloseWindow(pWindowThis(), WindowID);
}
// ----------------------------------------------------------------------------------------------

int Interface::OpenWindow(int WindowID)
{
	return pOpenWindow(pWindowThis(), WindowID);
}
// ----------------------------------------------------------------------------------------------

bool Interface::AllowGUI()
{
	if( gInterface.CheckCraftWindow() 
		|| gInterface.CheckResetWindow()
		|| gInterface.CheckNewsWindow() )
	{
		return false;
	}
	// ----
	return pAllowGUI();
}
// ----------------------------------------------------------------------------------------------

void Interface::SetTextColor(BYTE Red, BYTE Greed, BYTE Blue, BYTE Opacity)
{
	pSetTextColor(pTextThis(), Red, Green, Blue, Opacity);
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawText(int X, int Y, LPCTSTR Text)
{
	pDrawText(pTextThis(), X, Y, Text, 0, 0, (LPINT)1, 0);
}
// ----------------------------------------------------------------------------------------------

int Interface::DrawFormat(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen	= sizeof(Buff)-1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len	= vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);
	// ----
	int LineCount = 0;
	// ----
	char * Line = strtok(Buff, "\n");
	// ----
	while( Line != NULL )
	{
		pDrawColorText(Line, PosX, PosY, Width, 0, Color, 0, Align);
		PosY += 10;
		Line = strtok(NULL, "\n");
	}
	// ----
	return PosY;
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawFormatEx(DWORD Color, int PosX, int PosY, int Width, int Align, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen	= sizeof(Buff)-1;
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len	= vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);
	// ----
	pDrawColorText(Buff, PosX, PosY, Width, 0, Color, 0, Align);
}
// ----------------------------------------------------------------------------------------------

float Interface::DrawRepeatGUI(short MonsterID, float X, float Y, int Count)
{
	float StartY = Y;
	// ----
	for( int i = 0; i < Count; i++ )
	{
		pDrawGUI(this->Data[MonsterID].ModelID, X, StartY,
			this->Data[MonsterID].Width, this->Data[MonsterID].Height);
		// ----
		StartY += this->Data[MonsterID].Height;
	}
	// ----
	return StartY;
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawGUI(short ObjectID, float PosX, float PosY)
{
	if( this->Data[ObjectID].X == -1 || this->Data[ObjectID].Y == -1 )
	{
		this->Data[ObjectID].X		= PosX;
		this->Data[ObjectID].Y		= PosY;
		this->Data[ObjectID].MaxX	= PosX + this->Data[ObjectID].Width;
		this->Data[ObjectID].MaxY	= PosY + this->Data[ObjectID].Height;
	}
	// ----
	pDrawGUI(this->Data[ObjectID].ModelID, PosX, PosY, 
		this->Data[ObjectID].Width, this->Data[ObjectID].Height);
}
// ----------------------------------------------------------------------------------------------

void Interface::DrawColoredGUI(short ObjectID, float X, float Y, DWORD Color)
{
	if( this->Data[ObjectID].X == -1 || this->Data[ObjectID].Y == -1 )
	{
		this->Data[ObjectID].X		= X;
		this->Data[ObjectID].Y		= Y;
		this->Data[ObjectID].MaxX	= X + this->Data[ObjectID].Width;
		this->Data[ObjectID].MaxY	= Y + this->Data[ObjectID].Height;
	}
	// ----
	pDrawColorButton(this->Data[ObjectID].ModelID, X, Y, 
		this->Data[ObjectID].Width, this->Data[ObjectID].Height, 0, 0, Color);
}
// ----------------------------------------------------------------------------------------------

int Interface::DrawToolTip(int X, int Y, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen	= sizeof(Buff);
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len	= vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);
	// ----
	return pDrawToolTip(X, Y, Buff);
}
// ----------------------------------------------------------------------------------------------

int Interface::DrawToolTipEx(int X, int Y, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen	= sizeof(Buff);
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len	= vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);
	// ----
	int LineCount = 0;
	// ----
	char * Line = strtok(Buff, "\n");
	// ----
	while( Line != NULL )
	{
		pDrawToolTip(X, Y, Line);
		Y += 10;
		Line = strtok(NULL, "\n");
	}
	// ----
	return Y;
}
// ----------------------------------------------------------------------------------------------

int Interface::DrawMessage(int Mode, LPCSTR Text, ...)
{
	char Buff[2048];
	int BuffLen	= sizeof(Buff);
	ZeroMemory(Buff, BuffLen);
	// ----
	va_list args;
	va_start(args, Text);
	int Len	= vsprintf_s(Buff, BuffLen, Text, args);
	va_end(args);
	// ----
	return pDrawMessage(Buff, Mode);
}
// ----------------------------------------------------------------------------------------------

bool Interface::IsWorkZone(short ObjectID)
{
	float PosX = this->Data[ObjectID].X;
	float MaxX = PosX + this->Data[ObjectID].Width;
	// ----
	if( ObjectID == eSAMPLEBUTTON 
		|| ObjectID == eCAMERA_MAIN 
		|| ObjectID == eCAMERA_BUTTON1
		|| ObjectID == eCAMERA_BUTTON2 )
	{
		PosX = this->GetResizeX(ObjectID);
		MaxX = PosX + this->Data[ObjectID].Width;
	}
	// ----
	if( (gObjUser.m_CursorX < PosX || gObjUser.m_CursorX > MaxX) 
		|| (gObjUser.m_CursorY < this->Data[ObjectID].Y || gObjUser.m_CursorY > this->Data[ObjectID].MaxY) )
	{
		return false;
	}
	// ----
	return true;
}
// ----------------------------------------------------------------------------------------------

bool Interface::IsWorkZone(float X, float Y, float MaxX, float MaxY)
{
	if( (gObjUser.m_CursorX < X || gObjUser.m_CursorX > MaxX) 
		|| (gObjUser.m_CursorY < Y || gObjUser.m_CursorY > MaxY) )
	{
		return false;
	}
	// ----
	return true;
}
// ----------------------------------------------------------------------------------------------

float Interface::GetResizeX(short ObjectID)
{
	if( pWinWidth == 800 )
	{
		return this->Data[ObjectID].X + 16.0;
	}
	else if( pWinWidth != 1024 )
	{
		return this->Data[ObjectID].X - 16.0;
	}
	// ----
	return this->Data[ObjectID].X;
}
// ----------------------------------------------------------------------------------------------