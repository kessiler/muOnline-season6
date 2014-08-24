// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------
#pragma once

#include "../Lua/include/lua.hpp"


class LuaBindTest
{
public:
	LuaBindTest(void);
	LuaBindTest(lua_State* L);

	~LuaBindTest(void);

	int LuaBindFunTest(lua_State* L);
	int LuaBindFunReturnTest(lua_State *L);
	void Generic_Call(char *szLuaFunctionName);
};

