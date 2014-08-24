// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#pragma once

#include "../Lua/include/lua.hpp"

void Debug_AddDebugPrintTemp(lua_State *L);
bool g_Generic_Call(lua_State *L, const char* func, const char* sig, ...);