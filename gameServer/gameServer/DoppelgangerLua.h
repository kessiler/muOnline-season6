// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#pragma once

#include "..\Lua\include\lua.hpp"
#include "Luna.h"

class CDoppelgangerLua
{
public:
    CDoppelgangerLua(void);
	CDoppelgangerLua(lua_State* L){};
    virtual ~CDoppelgangerLua(void);

    int LuaGetMaxUserLevel(lua_State* L);
    int LuaGetMinUserLevel(lua_State* L);
    int LuaGetUserCount(lua_State *L);
    int LuaSetReadyTime(lua_State *L);
    int LuaSetPlayTime(lua_State *L);
    int LuaSetEndTime(lua_State *L);
    int LuaGetLocalTime(lua_State *L);
    int LuaGetStateTime(lua_State *L);
    int LuaSetHerdStartPosInfo(lua_State *L);
    int LuaSetHerdEndPosInfo(lua_State *L);
    int LuaAddMonsterHerd(lua_State *L);
    int LuaAddMonsterNormal(lua_State *L);
    int LuaIceworkerRegen(lua_State *L);
    int LuaGetMonsterHerdIndex(lua_State *L);
    int LuaIncMonsterHerdIndex(lua_State *L);
    int LuaMonsterHerdStart(lua_State *L);
    int LuaGetMapNumber(lua_State *L);
    int LuaGetRandomValue(lua_State *L);
    int LuaSetAddHerdMonsterTime(lua_State *L);
    int LuaGetAddHerdMonsterTime(lua_State *L);
    int LuaSetAddBossMonsterTime(lua_State *L);
    int LuaGetAddBossMonsterTime(lua_State *L);
    int LuaGetBossRegenOrder(lua_State *L);
    int LuaSetBossRegenOrder(lua_State *L);
    int LuaGetBossMonsterState(lua_State *L);
    int LuaSetBossMonsterState(lua_State *L);
    int LuaGetLastBossMonsterState(lua_State *L);
    int LuaSetLastBossMonsterState(lua_State *L);

    static const char className[];
    static const Luna<CDoppelgangerLua>::RegType Register[];

};

