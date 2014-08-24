#include "StdAfx.h"
#include "user.h"
#include "logproc.h"
#include "LuaExport.h"
#include "LuaMonsterDie.h"
#include "LuaScripts.h"
// -------------------------------------------------------------------------------

LuaMonsterDie	gLuaMonsterDie;
// -------------------------------------------------------------------------------

LuaMonsterDie::LuaMonsterDie()
{
	// ----
}
// -------------------------------------------------------------------------------

LuaMonsterDie::~LuaMonsterDie()
{
	// ----
}
// -------------------------------------------------------------------------------

bool LuaMonsterDie::Run(LPOBJ lpObj, LPOBJ lpMonster)
{
	int Result = false;
	// ----
	InsertObjMapData(lpObj);
	InsertMonMapData(lpMonster);
	// ----
	const struct luaL_Reg RegisterlpObj[] =	//-> Set method for lpObj
	{
		{"__index",			GetVaribleObj			},
		{"__newindex",		SetVaribleObj			},
		{NULL,				NULL					},
	};
	// ----
	const struct luaL_Reg RegisterlpMon[] =	//-> Set method for lpNpc
	{
		{"__index",			GetVaribleMon			},
		{"__newindex",		SetVaribleMon			},
		{NULL,				NULL					},
	};
	// ----
	lua_State * Lua = lua_open();	//-> Set load from script mode
	luaL_openlibs(Lua);				//-> Open lua common libs
	// ----
	this->RegisterData(Lua, RegisterlpObj, "RegisterlpObj", "lpObj", sizeof(ObjMap));
	this->RegisterData(Lua, RegisterlpMon, "RegisterlpMon", "lpMonster", sizeof(MonMap));
	// ----
	gLuaExport.AddTable(Lua);
	gLuaExport.AddGlobal(Lua);
	// ----
	int FileDo = luaL_dofile(Lua, LUA_MONDIE_SCRIPT);	//-> Open / check / start script
	if( FileDo )
	{
		LogAddC(2, "[LuaMonsterDie] %s", lua_tostring(Lua, -1));	//-> Get error as str
		return false;
	}
	// ----
	lua_getglobal(Lua, LUA_MONDIE_START);		//-> Get global function (var) from script
	lua_call(Lua, 0, 1);						//-> Call to this function with 0 args and 1 return
	Result = (int)luaL_checkinteger(Lua, -1);	//-> Get result via int
	lua_pop(Lua, 1);							//-> Pop stack?
	// ----
	lua_close(Lua);								//-> Close script
	// ----
	return Result;
}
// -------------------------------------------------------------------------------

void LuaMonsterDie::RegisterData(lua_State * Lua, const luaL_Reg * Table, char * TableName, char * DataName, size_t Size)
{
	lua_newuserdata(Lua, Size);
	luaL_newmetatable(Lua, TableName);
	luaL_register(Lua, DataName, Table);
	lua_setmetatable(Lua, -2);
	lua_setglobal(Lua, DataName);
}
// -------------------------------------------------------------------------------

BOOL LuaMonsterDie::GetVaribleObj(lua_State * Lua)
{
	const char * Varible = luaL_checkstring(Lua, 2);
	// ----
	for( int i = 0; i < (sizeof(gLuaMonsterDie.ObjMap) / sizeof(gLuaMonsterDie.ObjMap[0])); i++ ) 
	{
		if( !strcmp(gLuaMonsterDie.ObjMap[i].Name, Varible) ) 
		{
			lua_pushinteger(Lua, gLuaMonsterDie.ObjMap[i].Varible);
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

BOOL LuaMonsterDie::GetVaribleMon(lua_State * Lua)
{
	const char * Varible = luaL_checkstring(Lua, 2);
	// ----
	for( int i = 0; i < (sizeof(gLuaMonsterDie.MonMap) / sizeof(gLuaMonsterDie.MonMap[0])); i++ ) 
	{
		if( !strcmp(gLuaMonsterDie.MonMap[i].Name, Varible) ) 
		{
			lua_pushinteger(Lua, gLuaMonsterDie.MonMap[i].Varible);
			return true;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

BOOL LuaMonsterDie::SetVaribleObj(lua_State * Lua)
{
	const char * Varible	= luaL_checkstring(Lua, 2);
	int Value				= luaL_checkint(Lua, 3);
	// ----
	for( int i = 0; i < (sizeof(gLuaMonsterDie.ObjMap) / sizeof(gLuaMonsterDie.ObjMap[0])); i++ ) 
	{
		if( !strcmp(gLuaMonsterDie.ObjMap[i].Name, Varible) && gLuaMonsterDie.ObjMap[i].Varible ) 
		{
			//lua_pushinteger(Lua, ObjMap[i].Varible);
			break;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

BOOL LuaMonsterDie::SetVaribleMon(lua_State * Lua)
{
	const char * Varible	= luaL_checkstring(Lua, 2);
	int Value				= luaL_checkint(Lua, 3);
	// ----
	for( int i = 0; i < (sizeof(gLuaMonsterDie.ObjMap) / sizeof(gLuaMonsterDie.ObjMap[0])); i++ ) 
	{
		if( !strcmp(gLuaMonsterDie.MonMap[i].Name, Varible) && gLuaMonsterDie.MonMap[i].Varible ) 
		{
			//lua_pushinteger(Lua, MonMap[i].Varible);
			break;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------