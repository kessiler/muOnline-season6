#pragma once
// ----------------------------------------------------------------------------------------------

#include "Object.h"
// ----------------------------------------------------------------------------------------------

class Item
{
public:
	void			Load();
	static void		LoadModels();
	static void		LoadTextures();
	// ----
	static bool		IsCustomWings(WORD ItemID, bool Preview);
	static bool		IsCustomJewels(WORD ItemID, bool Preview);
	// ----
	static __int64	GetPrice(ObjectItem * lpItem, int Mode);
	static bool		IsExpensiveItem(ObjectItem * lpItem);
	static int	_FC IsWings(LPVOID This, LPVOID EDX, DWORD ItemID);
	static bool	_FC	GetUpgradeFlag(LPVOID This, LPVOID EDX, ObjectItem * lpSource, ObjectItem * lpTarget);
	static void		PreviewCharSet(int ObjectIndex, BYTE * CharSet, lpViewObj Object, int Mode);
	static bool		AllowRepairWings(ObjectItem * lpItem);
	static void		SetOption(ObjectItem * lpItem, BYTE Option, BYTE Special, BYTE Value);
	static void		SetOptionText(int ItemID, int Line, WORD OptionID, bool Arg4, int Arg5);
	static void		SetItemEffect(int Arg1, int ModelID, VAngle Angle, int Arg4, int ItemLevel, char Arg6, int Arg7, int Arg8, int Arg9);
	static short	GetMaxDurability(ObjectItem * lpItem, int ScriptStruct, int Level);
	static void		GetItemColor(int ModelID, float Arg2, float Arg3, VAngle * Color, bool BalrogMode);
	static void		GetItemColor2(int ModelID, float Arg2, float Arg3, VAngle * Color, bool BalrogMode);
	// ----
}; extern Item gItem;
// ----------------------------------------------------------------------------------------------