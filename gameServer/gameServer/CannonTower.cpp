// CannonTower.cpp: implementation of the CCannonTower class.
// GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GameServer.h"
#include "CannonTower.h"
#include "user.h"
#include "..\common\winutil.h"
#include "ObjUseSkill.h"

//#if(_GSCS==1)
CCannonTower g_CsNPC_CannonTower;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCannonTower::CCannonTower()
{

}

CCannonTower::~CCannonTower()
{

}

void CCannonTower::CannonTowerAct(int iIndex)
{	
	if( ( rand() % 2 ) != 0 )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	int tObjNum;
	int count = 0;
	PMSG_BEATTACK_COUNT pCount;
	PMSG_BEATTACK pAttack;
	BYTE AttackSendBuff[256];
	int ASBOfs;

	ASBOfs = 0;

	pCount.h.c = 0xC1;
	pCount.h.headcode = PROTOCOL_BEATTACK;
	pCount.h.size = 0x00;
	pCount.MagicNumberH = 0x00;
	pCount.MagicNumberL = 0x00;
	pCount.Count = 0x00;
	pCount.X = lpObj->X;
	pCount.Y = lpObj->Y;

	ASBOfs = sizeof(PMSG_BEATTACK_COUNT);

	while( true ) 
	{
		if( lpObj->VpPlayer2[count].state ) 
		{
			if( lpObj->VpPlayer2[count].type == OBJ_USER ) 
			{
				tObjNum = lpObj->VpPlayer2[count].number;

				if( tObjNum >= 0 ) 
				{
					if( gObj[tObjNum].m_btCsJoinSide != 1 )
					{
						if( gObjCalDistance(lpObj, &gObj[tObjNum]) < 7 ) 
						{
							pAttack.NumberH = SET_NUMBERH(tObjNum);
							pAttack.NumberL = SET_NUMBERL(tObjNum);
							memcpy( (AttackSendBuff+ASBOfs), (LPBYTE)&pAttack, sizeof(PMSG_BEATTACK));
							ASBOfs += sizeof(PMSG_BEATTACK);
							pCount.Count++;
						}
					}
				}
			}
		}
		count++;

		if( count > MAX_VIEWPORT-1 ) break;
	}
	if( pCount.Count > 0 ) 
	{
		pCount.h.size = ASBOfs;

		memcpy(AttackSendBuff, (LPBYTE)&pCount, sizeof(PMSG_BEATTACK_COUNT));
		CGBeattackRecv(AttackSendBuff, lpObj->m_Index, 1);

		PMSG_DURATION_MAGIC_SEND pSend;
		PHeadSetBE((LPBYTE)&pSend, 0x1E, sizeof(PMSG_DURATION_MAGIC_SEND));
		pSend.MagicNumberH	= 0;
		pSend.MagicNumberL	= 0x32;
		pSend.X			= lpObj->X;
		pSend.Y			= lpObj->Y;
		pSend.Dir		= 0;
		pSend.NumberH	= SET_NUMBERH(iIndex);
		pSend.NumberL	= SET_NUMBERL(iIndex);
			
		::MsgSendV2(lpObj, (LPBYTE)&pSend, pSend.h.size);

	}
}
//#endif