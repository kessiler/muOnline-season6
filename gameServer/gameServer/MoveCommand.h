#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#define MAX_MOVE_COMMAND 50

struct MOVE_COMMAND_DATA
{
	int Index;	// 0
	char Name[255];	// 3
	char EngName[255];	// 103
	int NeedZen;	// 204
	int NeedLevel;	// 208
	int GateNumber;	// 20C
};

struct MOVE_MOVE_LEVEL
{
	int MoveLevel;	// 0
	int MapNumber;	// 4
	int X;	// 8
	int Y;	// C
	int TX;	// 10
	int TY;	// 14
};

class CMoveCommand
{

private:

	int FindIndex(char* mapname);

public:

	CMoveCommand();
	virtual ~CMoveCommand();

	void Init();
	int Load(char* Buffer, int iSize);
	int Load(char* filename);
	int LoadMoveLevel(char* filename);
	int GetMoveLevel(int mapnumber, int x, int y, int Class);
	int CheckMainToMove(LPOBJ lpObj);
	int CheckEquipmentToMove(LPOBJ lpObj, int iTargetMapNumber);
	int CheckInterfaceToMove(LPOBJ lpObj);
	int Move(LPOBJ lpObj, char* mapname);
	int MoveFree2Kalima(LPOBJ lpObj);
	BOOL CheckMoveMapBound(int iMapIndex);
	BOOL Move(LPOBJ lpObj,int MoveNum);
	int FindIndex(int MoveNum);
	int CheckMoveNum(int MoveNum);
	MOVE_COMMAND_DATA * GetMoveCommandData(int nMapIndex);	//1.01.00
private:

	MOVE_COMMAND_DATA m_MoveCommandData[MAX_MOVE_COMMAND];
	MOVE_MOVE_LEVEL m_MoveLevel[MAX_MOVE_COMMAND];
	
};

extern CMoveCommand gMoveCommand;

#endif