#include "StdAfx.h"
#include "BuffCalc.h"
#include "logproc.h"
#include "GameMain.h"
#include "LuaFun.h"
#include "LuaExport.h"
#include "MasterLevelSkillTreeSystem.h"
// -------------------------------------------------------------------------------

BuffCalc g_BuffCalc;
// -------------------------------------------------------------------------------

void BuffCalc::SetClass(LPOBJ lpUser, MULua * Lua)
{
	lua_State * pLuaState = Lua->GetLua();
	// ----
	if( pLuaState == NULL )
	{
		LogAddC(2, "[BuffCalc] Error [%s] [%d]", __FILE__, __LINE__);
		return;
	}
	// ----
	this->m_Bind[0].Name	= "Level";
	this->m_Bind[0].Varible	= lpUser->Level;
	this->m_Bind[1].Name	= "MasterLevel";
	this->m_Bind[1].Varible	= lpUser->m_nMasterLevel;
	this->m_Bind[2].Name	= "Reset";
	this->m_Bind[2].Varible	= lpUser->Reset;
	this->m_Bind[3].Name	= "AttackSpeed";
	this->m_Bind[3].Varible	= lpUser->m_AttackSpeed;
	this->m_Bind[4].Name	= "MagicSpeed";
	this->m_Bind[4].Varible	= lpUser->m_MagicSpeed;
	this->m_Bind[5].Name	= "Strength";
	this->m_Bind[5].Varible	= lpUser->Strength + lpUser->AddStrength;
	this->m_Bind[6].Name	= "Dexterity";
	this->m_Bind[6].Varible	= lpUser->Dexterity + lpUser->AddDexterity;
	this->m_Bind[7].Name	= "Vitality";
	this->m_Bind[7].Varible	= lpUser->Vitality + lpUser->AddVitality;
	this->m_Bind[8].Name	= "Energy";
	this->m_Bind[8].Varible	= lpUser->Energy + lpUser->AddEnergy;
	this->m_Bind[9].Name	= "Command";
	this->m_Bind[9].Varible	= lpUser->Leadership + lpUser->AddLeadership;
	this->m_Bind[10].Name	= "Class";
	this->m_Bind[10].Varible= lpUser->Class;
	// ----
	const struct luaL_Reg ClassReg[] =
	{
		{ "__index",	this->GetClassVar	},
		{ NULL,			NULL				},
	};
	// ----
	lua_newuserdata(pLuaState, sizeof(this->m_Bind));
	luaL_newmetatable(pLuaState, "ClassReg");
	luaL_register(pLuaState, "lpUser", ClassReg);
	lua_setmetatable(pLuaState, -2);
	lua_setglobal(pLuaState, "lpUser");
	// ----
	Lua->DoFile(gDirPath.GetNewPath("Custom\\BuffCalc.lua"));
}
// -------------------------------------------------------------------------------

int BuffCalc::GetClassVar(lua_State * Lua)
{
	const char * Varible = luaL_checkstring(Lua, 2);
	// ----
	for( int i = 0; i < (sizeof(g_BuffCalc.m_Bind) / sizeof(g_BuffCalc.m_Bind[0])); i++ ) 
	{
		if( !strcmp(g_BuffCalc.m_Bind[i].Name, Varible) ) 
		{
			lua_pushinteger(Lua, g_BuffCalc.m_Bind[i].Varible);
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

bool BuffCalc::GetSoulBarrier(LPOBJ lpUser, int * Defense, int * ManaRate, int * Duration)
{
	MULua * Lua = new MULua;
	this->SetClass(lpUser, Lua);
	// ----
	g_Generic_Call(Lua->GetLua(), "GetSoulBarrier", ">iii", Defense, ManaRate, Duration);
	Lua->Release();
	// ----
	delete Lua;
	return true;
}
// -------------------------------------------------------------------------------

bool BuffCalc::GetSoulBarrier_Strengthen(LPOBJ lpUser, int SkillLevel, float * Defense, int * ManaRate, int * Duration)
{
	MULua * Lua = new MULua;
	this->SetClass(lpUser, Lua);
	// ----
	double Return = 0;
	g_Generic_Call(Lua->GetLua(), "GetSoulBarrier_Strengthen", "i>dii", SkillLevel, &Return, ManaRate, Duration);
	*Defense = (float)Return;
	Lua->Release();
	// ----
	delete Lua;
	return true;
}
// -------------------------------------------------------------------------------

bool BuffCalc::GetSwellLife(LPOBJ lpUser, int * LifeAdd, int * Duration)
{
	MULua * Lua = new MULua;
	this->SetClass(lpUser, Lua);
	// ----
	g_Generic_Call(Lua->GetLua(), "GetSwellLife", ">ii", LifeAdd, Duration);
	Lua->Release();
	// ----
	delete Lua;
	return true;
}
// -------------------------------------------------------------------------------

bool BuffCalc::GetSwellLife_Strengthen(LPOBJ lpUser, float * LifeAdd, int * Duration)
{
	MULua * Lua = new MULua;
	this->SetClass(lpUser, Lua);
	// ----
	double Return = 0;
	g_Generic_Call(Lua->GetLua(), "GetSwellLife_Strengthen", ">di", &Return, Duration);
	*LifeAdd = (float)Return;
	Lua->Release();
	// ----
	delete Lua;
	return true;
}
// -------------------------------------------------------------------------------

bool BuffCalc::GetSwellLife_Proficiency(LPOBJ lpUser, float * LifeAdd, int * Duration)
{
	MULua * Lua = new MULua;
	this->SetClass(lpUser, Lua);
	// ----
	double Return = 0;
	g_Generic_Call(Lua->GetLua(), "GetSwellLife_Proficiency", ">di", &Return, Duration);
	*LifeAdd = (float)Return;
	Lua->Release();
	// ----
	delete Lua;
	return true;
}
// -------------------------------------------------------------------------------

bool BuffCalc::GetElfDefense(LPOBJ lpUser, int SkillLevel, float * Defense, int * Duration)
{
	MULua * Lua = new MULua;
	this->SetClass(lpUser, Lua);
	// ----
	double Return = 0;
	g_Generic_Call(Lua->GetLua(), "GetElfDefense", "i>di", SkillLevel, &Return, Duration);
	*Defense = (float)Return;
	Lua->Release();
	// ----
	delete Lua;
	return true;
}
// -------------------------------------------------------------------------------

bool BuffCalc::GetElfDamage(LPOBJ lpUser, int SkillLevel, float * Damage, int * Duration)
{
	MULua * Lua = new MULua;
	this->SetClass(lpUser, Lua);
	// ----
	double Return = 0;
	g_Generic_Call(Lua->GetLua(), "GetElfDamage", "i>di", SkillLevel, &Return, Duration);
	*Damage = (float)Return;
	Lua->Release();
	// ----
	delete Lua;
	return true;
}
// -------------------------------------------------------------------------------