#include "StdAfx.h"
#include "user.h"
#include "logproc.h"
#include "LuaExport.h"
#include "LuaNPC.h"
#include "LuaScripts.h"
// -------------------------------------------------------------------------------

LuaNPC	gLuaNPC;
// -------------------------------------------------------------------------------

LuaNPC::LuaNPC()
{
	// ----
}
// -------------------------------------------------------------------------------

LuaNPC::~LuaNPC()
{
	// ----
}
// -------------------------------------------------------------------------------

bool LuaNPC::Run(LPOBJ lpObj, LPOBJ lpNpc)
{
	int Result = false;
	// ----
	InsertObjMapData(lpObj);
	InsertNpcMapData(lpNpc);
	// ----
	const struct luaL_Reg RegisterlpObj[] =	//-> Set method for lpObj
	{
		{"__index",			GetVaribleObj			},
		{"__newindex",		SetVaribleObj			},
		{NULL,				NULL					},
	};
	// ----
	const struct luaL_Reg RegisterlpNpc[] =	//-> Set method for lpNpc
	{
		{"__index",			GetVaribleNpc			},
		{"__newindex",		SetVaribleNpc			},
		{NULL,				NULL					},
	};
	// ----
	lua_State * Lua = lua_open();	//-> Set load from script mode
	luaL_openlibs(Lua);				//-> Open lua common libs
	// ----
	this->RegisterData(Lua, RegisterlpObj, "RegisterlpObj", "lpObj", sizeof(ObjMap));
	this->RegisterData(Lua, RegisterlpNpc, "RegisterlpNpc", "lpNpc", sizeof(NpcMap));
	// ----
	gLuaExport.AddTable(Lua);
	gLuaExport.AddGlobal(Lua);
	// ----
	int FileDo = luaL_dofile(Lua, LUA_NPC_SCRIPT);	//-> Open / check / start script
	if( FileDo )
	{
		LogAddC(2, "[LuaNPC] %s", lua_tostring(Lua, -1));	//-> Get error as str
		return false;
	}
	// ----
	lua_getglobal(Lua, LUA_NPC_START);				//-> Get global function (var) from script
	lua_call(Lua, 0, 1);						//-> Call to this function with 0 args and 1 return
	Result = (int)luaL_checkinteger(Lua, -1);	//-> Get result via int
	lua_pop(Lua, 1);							//-> Pop stack?
	// ----
	lua_close(Lua);								//-> Close script
	// ----
	return Result;
}
// -------------------------------------------------------------------------------

void LuaNPC::RegisterData(lua_State * Lua, const luaL_Reg * Table, char * TableName, char * DataName, size_t Size)
{
	lua_newuserdata(Lua, Size);
	luaL_newmetatable(Lua, TableName);
	luaL_register(Lua, DataName, Table);
	lua_setmetatable(Lua, -2);
	lua_setglobal(Lua, DataName);
}
// -------------------------------------------------------------------------------

BOOL LuaNPC::GetVaribleObj(lua_State * Lua)
{
	const char * Varible = luaL_checkstring(Lua, 2);
	// ----
	for( int i = 0; i < (sizeof(gLuaNPC.ObjMap) / sizeof(gLuaNPC.ObjMap[0])); i++ ) 
	{
		if( !strcmp(gLuaNPC.ObjMap[i].Name, Varible) ) 
		{
			lua_pushinteger(Lua, gLuaNPC.ObjMap[i].Varible);
			return true;
		}
	}
	// ----
	return false;
}
// -------------------------------------------------------------------------------

BOOL LuaNPC::GetVaribleNpc(lua_State * Lua)
{
	const char * Varible = luaL_checkstring(Lua, 2);
	// ----
	for( int i = 0; i < (sizeof(gLuaNPC.NpcMap) / sizeof(gLuaNPC.NpcMap[0])); i++ ) 
	{
		if( !strcmp(gLuaNPC.NpcMap[i].Name, Varible) ) 
		{
			lua_pushinteger(Lua, gLuaNPC.NpcMap[i].Varible);
			return true;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

BOOL LuaNPC::SetVaribleObj(lua_State * Lua)
{
	const char * Varible	= luaL_checkstring(Lua, 2);
	int Value				= luaL_checkint(Lua, 3);
	// ----
	for( int i = 0; i < (sizeof(gLuaNPC.ObjMap) / sizeof(gLuaNPC.ObjMap[0])); i++ ) 
	{
		if( !strcmp(gLuaNPC.ObjMap[i].Name, Varible) && gLuaNPC.ObjMap[i].Varible ) 
		{
			//lua_pushinteger(Lua, ObjMap[i].Varible);
			break;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------

BOOL LuaNPC::SetVaribleNpc(lua_State * Lua)
{
	const char * Varible	= luaL_checkstring(Lua, 2);
	int Value				= luaL_checkint(Lua, 3);
	// ----
	for( int i = 0; i < (sizeof(gLuaNPC.ObjMap) / sizeof(gLuaNPC.ObjMap[0])); i++ ) 
	{
		if( !strcmp(gLuaNPC.ObjMap[i].Name, Varible) && gLuaNPC.ObjMap[i].Varible ) 
		{
			//lua_pushinteger(Lua, ObjMap[i].Varible);
			break;
		}
	}
	// ----
	return true;
}
// -------------------------------------------------------------------------------