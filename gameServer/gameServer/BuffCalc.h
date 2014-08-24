#pragma once
// -------------------------------------------------------------------------------

#include "..\common\classdef.h"
#include "user.h"
#include "MuLua.h"
#include "Luna.h"
// -------------------------------------------------------------------------------

static struct BuffCalc_Bind
{
	const char * Name;
	int	Varible;
};
// -------------------------------------------------------------------------------

class BuffCalc
{
public:
	void	SetClass(LPOBJ lpUser, MULua * Lua);
	static int GetClassVar(lua_State * Lua);
	// ----
	bool	GetSoulBarrier(LPOBJ lpUser, int * Defense, int * ManaRate, int * Duration);
	bool	GetSoulBarrier_Strengthen(LPOBJ lpUser, int SkillLevel, float * Defense, int * ManaRate, int * Duration);
	bool	GetSwellLife(LPOBJ lpUser, int * LifeAdd, int * Duration);
	bool	GetSwellLife_Strengthen(LPOBJ lpUser, float * LifeAdd, int * Duration);
	bool	GetSwellLife_Proficiency(LPOBJ lpUser, float * LifeAdd, int * Duration);
	bool	GetElfDefense(LPOBJ lpUser, int SkillLevel, float * Defense, int * Duration);
	bool	GetElfDamage(LPOBJ lpUser, int SkillLevel, float * Damage, int * Duration);
	// ----
public:
	BuffCalc_Bind	m_Bind[11];
	// ----
}; extern BuffCalc g_BuffCalc;
// -------------------------------------------------------------------------------