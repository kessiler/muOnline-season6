// TMonsterAIMovePath.cpp: implementation of the TMonsterAIMovePath class.
//	GS-N	1.00.77	JPN	-	Completed
//	GS-CS	1.00.90	JPN	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIMovePath.h"


#include "..\include\ReadScript.h"
#include "LogProc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIMovePath::TMonsterAIMovePath()
{
	this->DelAllAIMonsterMovePath();
}



TMonsterAIMovePath::~TMonsterAIMovePath()
{
	return;
}




BOOL TMonsterAIMovePath::LoadData(LPSTR lpszFileName)
{
	this->m_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		MsgBox("[Monster AI MovePath] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");	//ok

		if ( SMDFile == NULL )
		{
			MsgBox("[Monster AI MovePath] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		this->DelAllAIMonsterMovePath();
		int iType = -1;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
				break;

			iType = TokenNumber;

			while ( true )
			{
				if ( iType == 2 )
				{
					int iSpotType = -1;
					int iMapNumber = -1;
					int iX = -1;
					int iY = -1;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iSpotType = TokenNumber;

					Token = GetToken();
					iMapNumber = TokenNumber;

					Token = GetToken();
					iX = TokenNumber;

					Token = GetToken();
					iY = TokenNumber;

					this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iType = iSpotType;
					this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iMapNumber = iMapNumber;
					this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iPathX = iX;
					this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iPathY = iY;

					this->m_iMovePathSpotCount++;

					if ( this->m_iMovePathSpotCount > MAX_MONSTER_AI_MOVE_PATH )
					{
						MsgBox("[Monster AI MovePath] Exceed Max Move Path-Spot ");
						this->DelAllAIMonsterMovePath();

						return FALSE;
					}
				}
			}
		}

		fclose(SMDFile);

		LogAddC(2, "[Monster AI MovePath ] - %s file is Loaded", lpszFileName);

		this->m_bDataLoad = TRUE;
	}
	catch(DWORD)
	{
		MsgBox("[Monster AI MovePath] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}



BOOL TMonsterAIMovePath::DelAllAIMonsterMovePath()
{
	for ( int i=0;i<MAX_MONSTER_AI_MOVE_PATH;i++)
	{
		this->m_MovePathInfo[i].Reset();
	}

	this->m_iMovePathSpotCount = 0;
	this->m_bDataLoad = FALSE;

	return FALSE;
}