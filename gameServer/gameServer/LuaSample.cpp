// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "LuaSample.h"
#include "logproc.h"

LuaBindTest::LuaBindTest(void)
{
}

LuaBindTest::LuaBindTest(lua_State* L)
{
}

LuaBindTest::~LuaBindTest(void)
{
}

int LuaBindTest::LuaBindFunTest(lua_State* L)
{
	char szGetLua[1024];
	int iGetLua;

	iGetLua = luaL_checkinteger(L, 1);
	strcpy(szGetLua, luaL_checklstring(L, 2, 0) );

	//TRACE_LOG("\n iGetLua %d szGetLua %s", iGetLua, szGetLua);

	return 1;
}

int LuaBindTest::LuaBindFunReturnTest(lua_State *L)
{
	int iReturn = 123;

	lua_pushnumber(L, iReturn);

	return 1;
}

void LuaBindTest::Generic_Call(char *szLuaFunctionName)
{
}