#include "stdafx.h"
#include "Protocol.h"
#include "CheatGuard.h"
#include "Import.h"
#include "TMemory.h"
#include "TDebugLog.h"
#include "User.h"
#include "CraftSystem.h"
#include "ResetSystem.h"
#include "NewsBoard.h"
#include "ConnectEx.h"
#include "ChatExpanded.h"
#include "VisualFix.h"
// ----------------------------------------------------------------------------------------------

Protocol	gProtocol;
// ----------------------------------------------------------------------------------------------

void Protocol::Load()
{
	SetOp((LPVOID)oDataRecv_Call, (LPVOID)this->DataRecv, ASM::CALL);
}
// ----------------------------------------------------------------------------------------------

void Protocol::DataRecv(DWORD Case, LPBYTE Data, int Len, int aIndex)
{
	BYTE ProtocolType = Data[0];
	// ----
	if( ProtocolType == 0xC1 )
	{
		switch(BYTE(Case))
		{
		case 0x11:
			{
				gVisualFix.RecvDamage((PMSG_ATTACKRESULT*) Data);
			}
			break;
		case 0x17:
			{
				gVisualFix.RecvKilledObject((PMSG_DIEPLAYER*)Data);
			}
			break;
		case 0x26:
			{
				gVisualFix.RecvHPSD((PMSG_REFILL*)Data);
			}
			break;
		case 0x27:
			{
				gVisualFix.RecvMPAG((PMSG_MANASEND*)Data);
			}
			break;
		case 0x76:
			{
				gCheatGuard.Check((CHEATGUARD_REQ_CHECK*)Data);
			}
			break;
			// --
		case 0xF1:
			{
				PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2*)Data;
				switch(lpDef->subcode)
				{
				case 0x00:
					{
						gVisualFix.RecvIndex((PMSG_JOINRESULT*)Data);
					}
					break;
				}
			}
			break;
		case 0xF3:
			{
				PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2*)Data;
				// ----
				switch(lpDef->subcode)
				{
				case 3:
					{
						gConnectEx.RestoreGame();
						gVisualFix.RecvUpPoint((PMSG_CHARMAPJOINRESULT*)Data);
					}
					break;
				case 0x04:
					{
						gVisualFix.RecvRespawn();
					}
					break;
				case 0x05:
					{
						gVisualFix.RecvUpLevel((PMSG_LEVELUP*)Data);
					}
					break;
				case 0x06:
					{
						gVisualFix.RecvDownPoint((PMSG_LVPOINTADDRESULT*)Data);
					}
					break;
				}
			}
			break;
			// --
		case 0xFB:
			{
				PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2*)Data;
				// ----
				switch(lpDef->subcode)
				{
#ifdef __NOVUS__
				case 0:
					{
						gCraftSystem.SetData((CRAFT_ANS_USERDATA*)Data);
					}
					break;
					// --
				case 1:
					{
						gCraftSystem.GetQuestResult((CRAFT_ANS_QUESTFINISH*)Data);
					}
					break;
					// --
#endif
				case 5:
					{
						gResetSystem.SetData((RESET_ANS_USERDATA*)Data);
					}
					break;
					// --
				case 6:
					{
						gResetSystem.GetResetResult();
					}
					break;
					// --
				case 7:
					{
						gObjUser.SetTargetData((PMSG_TARGETDATA_ANS*)Data);
					}
					break;
					// --
				case 8:
					{
						gObjUser.UpdateCharInfo((CHAR_UPDATEINFO*)Data);
					}
					break;
					// --
				case 0xC:
					{
						gConnectEx.m_ConnectState = ConnectExType::OnForceDisconnect;
					}
					break;
				}
			}
			break;
			// --
		case 0xFC:
			{
				gObjUser.SetBattleMapData((PMSG_BATTLE_LIST*)Data);
			}
			break;
		}
	}
	else if( ProtocolType == 0xC2 )
	{
		switch(BYTE(Case))
		{
		case 0xFD:
			{
				gNewsBoard.OpenMain((NEWS_ANS_TITLES*)Data);
			}
			break;
			// --
		case 0xFE:
			{
				gNewsBoard.OpenItem((NEWS_ANS_NEWS*)Data);
			}
			break;
		}
	}
	// ----
	pDataRecv(Case, Data, Len, aIndex);
}
// ----------------------------------------------------------------------------------------------

void Protocol::DataSend(LPBYTE Data, int Size)
{
	int StartPos = 0;
	// ----
	if( Data[0] == 0xC1 || Data[0] == 0xC3 )
	{
		StartPos = 3;
	}
	else if( Data[0] == 0xC2 || Data[0] == 0xC4 )
	{
		StartPos = 4;
	}
	// ----
	for( int i = StartPos; i < Size; i++ )
	{
		Data[i] ^= Data[i - 1] ^ gCheatGuard.XOR[i%32];
	}
	// ----
	send(pActiveSocket, (char*)Data, Size, 0);
}
// ----------------------------------------------------------------------------------------------