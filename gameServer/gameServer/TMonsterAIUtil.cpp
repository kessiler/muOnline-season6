// TMonsterAIUtil.cpp: implementation of the TMonsterAIUtil class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIUtil.h"

#include "Gamemain.h"
#include "..\common\winutil.h"
#include "BuffEffectSlot.h"
#include "logproc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIUtil::TMonsterAIUtil()
{
	return;
}

TMonsterAIUtil::~TMonsterAIUtil()
{
	return;
}


BOOL TMonsterAIUtil::FindMonViewportObj(int iObjIndex, int iTargetObjIndex)
{
	if ( OBJMON_RANGE(iObjIndex) ==  FALSE || OBJMAX_RANGE(iTargetObjIndex) == FALSE )
		return FALSE;

	for ( int i=0;i<MAX_VIEWPORT_MONSTER;i++)
	{
		if ( gObj[iObjIndex].VpPlayer[i].state != FALSE )
		{
			if ( iTargetObjIndex == gObj[iObjIndex].VpPlayer[i].number )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL TMonsterAIUtil::FindMonViewportObj2(int iObjIndex, int iTargetObjIndex)
{
	if ( OBJMON_RANGE(iObjIndex) ==  FALSE || OBJMAX_RANGE(iTargetObjIndex) == FALSE )
		return FALSE;

	for ( int i=0;i<MAX_VIEWPORT_MONSTER;i++)
	{
		if ( gObj[iObjIndex].VpPlayer2[i].state != FALSE )
		{
			if ( iTargetObjIndex == gObj[iObjIndex].VpPlayer2[i].number )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}




void TMonsterAIUtil::SendMonsterV2Msg(LPOBJ lpObj, LPBYTE lpMsg, int size)
{
	for ( int i=0;i<MAX_VIEWPORT_MONSTER;i++)
	{
		if ( lpObj->VpPlayer2[i].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[i].state != FALSE )
			{
				if ( lpObj->Connected > PLAYER_CONNECTED && lpObj->Live != FALSE)
				{
					DataSend(lpObj->VpPlayer2[i].number, lpMsg, size);
				}
				else
				{
					lpObj->VpPlayer2[i].number = -1;
					lpObj->VpPlayer2[i].state = FALSE;
					lpObj->VPCount2--;
				}
			}
		}
	}
}



BOOL TMonsterAIUtil::FindPathToMoveMonster(LPOBJ lpObj, int iTargetX, int iTargetY, int iMaxPathCount, BOOL bPreventOverMoving)
{
	if ( TMonsterAIUtil::CheckMovingCondition(lpObj)==FALSE)
		return FALSE;

	PATH_t Path;
	BOOL bPathFound = FALSE;

#ifndef DOPPEL
	if ( bPreventOverMoving )
		bPathFound = MapC[lpObj->MapNumber].PathFinding2(lpObj->X, lpObj->Y, iTargetX, iTargetY, &Path);
	else
		bPathFound = MapC[lpObj->MapNumber].PathFinding4(lpObj->X, lpObj->Y, iTargetX, iTargetY, &Path);
#else
	if ( bPreventOverMoving == 1 )
		bPathFound = MapC[lpObj->MapNumber].PathFinding2(lpObj->X, lpObj->Y, iTargetX, iTargetY, &Path);
	else if ( bPreventOverMoving == 2 )
		bPathFound = MapC[lpObj->MapNumber].PathFinding3(lpObj->X, lpObj->Y, iTargetX, iTargetY, &Path);
	else
		bPathFound = MapC[lpObj->MapNumber].PathFinding4(lpObj->X, lpObj->Y, iTargetX, iTargetY, &Path);
#endif

	if (bPathFound )
	{
		lpObj->m_LastMoveTime = GetTickCount();

		int iTargetX;
		int iTargetY;
		int iStartX;
		int iStartY;
		int iResultX;
		int iResultY;
		BYTE btTargetDir = 0;

		iStartX = lpObj->X;
		iStartY = lpObj->Y;
		iResultX = lpObj->X;
		iResultY = lpObj->Y;
		lpObj->PathCount = Path.PathNum;
		lpObj->PathCur = 1;
		lpObj->PathStartEnd = 1;

		if ( lpObj->PathCount > iMaxPathCount )
			lpObj->PathCount = iMaxPathCount;

		lpObj->PathX[0] = lpObj->X;
		lpObj->PathY[0] = lpObj->Y;
		lpObj->PathDir[0] = lpObj->Dir;

		for(int n=1;n<lpObj->PathCount;n++)
		{
			iTargetX = Path.PathX[n];
			iTargetY = Path.PathY[n];
			btTargetDir = GetPathPacketDirPos(iTargetX-iStartX, iTargetY-iStartY);
			lpObj->PathX[n] = iTargetX;
			lpObj->PathY[n] = iTargetY;
			lpObj->PathDir[n] = btTargetDir;
			iResultX += RoadPathTable[btTargetDir*2];
			iResultY += RoadPathTable[btTargetDir*2+1];
		}

		lpObj->MTX = iResultX;
		lpObj->MTY = iResultY;
		lpObj->TX = iResultX;
		lpObj->TY = iResultY;

		if ( lpObj->PathCount > 0 )
		{
			MapC[lpObj->MapNumber].ClearStandAttr(iStartX, iStartY);
			MapC[lpObj->MapNumber].SetStandAttr(iResultX, iResultY);
			lpObj->m_OldX = iStartX;
			lpObj->m_OldY = iStartY;
		}
	}

	if ( bPathFound )
	{
		this->SendMonsterMoveMsg(lpObj);
		return TRUE;
	}

	return FALSE;
}




BOOL TMonsterAIUtil::CheckMovingCondition(LPOBJ lpObj)
{
	if ( !lpObj->Live )
		return FALSE;

	if ( lpObj->RegenOk > 0 )
		return FALSE;

	if ( lpObj->Teleport )
		return FALSE;

	if ( lpObj->m_State != 2 )
		return FALSE;

	if(gObjCheckUsedBuffEffect(lpObj, AT_ICE_ARROW) == 1 || gObjCheckUsedBuffEffect(lpObj, AT_STUN) == 1)
	{
		return FALSE;
	}

	if(gObjCheckUsedBuffEffect(lpObj, AT_SLEEP) == 1)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL TMonsterAIUtil::CheckMoveRange(LPOBJ lpObj, int iTargetX, int iTargetY)
{
	return FALSE;
}



BOOL TMonsterAIUtil::GetXYToPatrol(LPOBJ lpObj)
{
	int maxmoverange = lpObj->m_MoveRange*2+1;
	int searchc=10;
	lpObj->NextActionTime = 1000;
	BYTE tpx;
	BYTE tpy;

	while ( searchc-- != 0 )
	{
		__try
		{
			tpx = (lpObj->X - lpObj->m_MoveRange) + (rand()%maxmoverange);
			tpy = (lpObj->Y - lpObj->m_MoveRange) + (rand()%maxmoverange);
		}
		__except(maxmoverange=1, 1)
		{

		}

		BYTE attr = MapC[lpObj->MapNumber].GetAttr(tpx, tpy);

		if ( (attr&1)!=1 && (attr&2)!=2 && (attr&4)!=4 && (attr&8)!= 8 )
		{
			lpObj->MTX = tpx;
			lpObj->MTY = tpy;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL TMonsterAIUtil::GetXYToEascape(LPOBJ lpObj)
{
	int tpx;	// Target Player X
	int tpy;
	int mtx;	// Monster Target X
	int mty;
	int tx;
	int ty;
	int searchp = 0;
	int sn = 0;
	int searchcount = MAX_ROAD_PATH_TABLE/2-1;
	BYTE attr;
	BOOL result = 0;
	LPOBJ lpTargetObj;

	if ( OBJMAX_RANGE(lpObj->TargetNumber) == FALSE )
	{
		return FALSE;
	}

	lpTargetObj = &gObj[lpObj->TargetNumber];
	tpx = lpTargetObj->X;
	mtx = tpx;
	tpy = lpTargetObj->Y;
	mty = tpy;
	tx = lpObj->X;
	ty = lpObj->Y;
	int dis = lpObj->m_AttackRange / sqrt(2.0);

	if ( lpObj->X < mtx )
	{
		tx -= dis;
	}

	if ( lpObj->X > mtx )
	{
		tx += dis;
	}

	if ( lpObj->Y < mty )
	{
		ty -= dis;
	}

	if ( lpObj->Y > mty )
	{
		ty += dis;
	}

	searchp = GetPathPacketDirPos( (lpObj->X - tx), (lpObj->Y - tx) ) * 2;

	if ( MapC[lpObj->MapNumber].GetStandAttr(tx, ty) == 0 )
	{
		while ( searchcount-- )
		{
			mtx = lpObj->X + RoadPathTable[searchp];
			mty = lpObj->Y + RoadPathTable[1+searchp];
			attr = MapC[lpObj->MapNumber].GetAttr(mtx, mty);
			
			if ( (attr&1) != 1 && (attr&2) != 2 && (attr&4) != 4 && (attr&8) != 8  )
			{
				lpObj->MTX = mtx;
				lpObj->MTY = mty;
				return TRUE;
			}

			searchp += 2;

			if ( searchp > MAX_ROAD_PATH_TABLE-1 )
			{
				searchp = 0;
			}
		}

		return FALSE;
	}

	attr = MapC[lpObj->MapNumber].GetAttr(tx, ty);

	if ( (attr&1) != 1 && (attr&2) != 2 && (attr&4) != 4 && (attr&8) != 8  )
	{
		lpObj->MTX = tx;
		lpObj->MTY = ty;
		return TRUE;
	}

	return FALSE;
}




BOOL TMonsterAIUtil::GetXYToChase(LPOBJ lpObj)
{
	int tpx;	// Target Player X
	int tpy;
	int mtx;	// Monster Target X
	int mty;
	int searchp = 0;
	int sn = 0;
	int searchcount = MAX_ROAD_PATH_TABLE/2-1;
	BYTE attr;
	BOOL result = 0;
	LPOBJ lpTargetObj;

	if ( OBJMAX_RANGE(lpObj->TargetNumber) == FALSE )
	{
		return FALSE;
	}

	lpTargetObj = &gObj[lpObj->TargetNumber];
	tpx = lpTargetObj->X;
	mtx = tpx;
	tpy = lpTargetObj->Y;
	mty = tpy;
	int dis = lpObj->m_AttackRange / sqrt(2.0);

	if ( lpObj->X < mtx )
	{
		tpx -= dis;
	}

	if ( lpObj->X > mtx )
	{
		tpx += dis;
	}

	if ( lpObj->Y < mty )
	{
		tpy -= dis;
	}

	if ( lpObj->Y > mty )
	{
		tpy += dis;
	}

	searchp = GetPathPacketDirPos( (lpTargetObj->X - tpx), (lpTargetObj->Y - tpy) ) * 2;

	if ( MapC[lpObj->MapNumber].GetStandAttr(tpx, tpy) == 0 )
	{
		while ( searchcount-- )
		{
			mtx = lpTargetObj->X + RoadPathTable[searchp];
			mty = lpTargetObj->Y + RoadPathTable[1+searchp];
			attr = MapC[lpObj->MapNumber].GetAttr(mtx, mty);
			
			if ( (attr&1) != 1 && (attr&2) != 2 && (attr&4) != 4 && (attr&8) != 8  )
			{
				lpObj->MTX = mtx;
				lpObj->MTY = mty;
				return TRUE;
			}

			searchp += 2;

			if ( searchp > MAX_ROAD_PATH_TABLE-1 )
			{
				searchp = 0;
			}
		}

		return FALSE;
	}

	attr = MapC[lpObj->MapNumber].GetAttr(tpx, tpy);

	if ( (attr&1) != 1 && (attr&2) != 2 && (attr&4) != 4 && (attr&8) != 8  )
	{
		lpObj->MTX = tpx;
		lpObj->MTY = tpy;
		return TRUE;
	}

	return FALSE;
}




BOOL TMonsterAIUtil::SendMonsterMoveMsg(LPOBJ lpObj)
{
	PMSG_RECVMOVE pMove;

	PHeadSetB((LPBYTE)&pMove, PROTOCOL_MOVE, sizeof(pMove));
	pMove.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMove.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMove.X = lpObj->MTX;
	pMove.Y = lpObj->MTY;
	pMove.Path = lpObj->Dir << 4;

	this->SendMonsterV2Msg(lpObj, (LPBYTE)&pMove, pMove.h.size);
	return TRUE;
}



void TMonsterAIUtil::SendChattingMsg(int iObjIndex, char* lpszMsg, ...)
{
	if ( !lpszMsg )
		return;

	if ( !OBJMAX_RANGE(iObjIndex))
		return;

	LPOBJ lpObj = &gObj[iObjIndex];
	char szBuffer[512] = "";
	va_list pArguments;

	va_start(pArguments, lpszMsg);
	vsprintf(szBuffer, lpszMsg, pArguments);
	va_end(pArguments);

	char szChat[MAX_CHAT_LEN] = {0};
	memcpy(szChat, szBuffer, sizeof(szChat)-1);

	for(int i=0;i<MAX_VIEWPORT_MONSTER;i++)
	{
		if ( lpObj->VpPlayer2[i].state )
		{
			int tObjNum = lpObj->VpPlayer2[i].number;

			if ( OBJMAX_RANGE(tObjNum) )
			{
				ChatTargetSend(lpObj, szChat, tObjNum);
			}
		}
	}
}

