#pragma once
// -------------------------------------------------------------------------------

#include "../Lua/include/lua.hpp"
#include "../common/winutil.h"
#include "Event.h"
#include "BuffEffectSlot.h"
#include "logproc.h"
#include "gObjMonster.h"
#include "DSProtocol.h"
#include "GameMain.h"
// -------------------------------------------------------------------------------

class LuaExport
{
public:
	LuaExport();
	virtual ~LuaExport();
	// ----
	void	AddTable(lua_State * Lua);
	void	AddGlobal(lua_State * Lua);
	// ----
	static BOOL		LuaChatTargetSend(lua_State * Lua)
	{
		ChatTargetSend(&gObj[lua_tointeger(Lua, 2)], (char*)lua_tostring(Lua, 1), lua_tointeger(Lua, 3));
		return true;
	};
	// ----
	static BOOL		LuaLogAddTD(lua_State * Lua)
	{
		LogAddTD("%s", (char*)lua_tostring(Lua, 1));
		return true;
	};
	// ----
	static BOOL		LuagObjTeleport(lua_State * Lua)
	{
		gObjTeleport(lua_tointeger(Lua, 1), lua_tointeger(Lua, 2), lua_tointeger(Lua, 3), lua_tointeger(Lua, 4));
		return true;
	};
	// ----
	static BOOL		LuaGCServerMsgStringSend(lua_State * Lua)
	{
		GCServerMsgStringSend((char*)lua_tostring(Lua, 1), lua_tointeger(Lua, 2), lua_tointeger(Lua, 3));
		return true;
	};
	// ----
	static BOOL		LuaFireCrackerOpen(lua_State * Lua)
	{
		PMSG_SERVERCMD ServerCmd;
		// ----
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		ServerCmd.X = gObj[lua_tointeger(Lua, 1)].X;
		ServerCmd.Y = gObj[lua_tointeger(Lua, 1)].Y;
		// ----
		MsgSendV2(&gObj[lua_tointeger(Lua, 1)], (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		DataSend(lua_tointeger(Lua, 1), (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		return true;
	};
	// ----
	static BOOL		LuagObjCalDistance(lua_State * Lua)
	{
		BOOL Result = gObjCalDistance(&gObj[lua_tointeger(Lua, 1)], &gObj[lua_tointeger(Lua, 2)]);
		lua_pushnumber(Lua, Result);
		return Result;
	};
	// ----
	static BOOL		LuaGCServerCmd(lua_State * Lua)
	{
		GCServerCmd(lua_tointeger(Lua, 1), lua_tointeger(Lua, 2), lua_tointeger(Lua, 3), lua_tointeger(Lua, 4));
		return true;
	};
	// ----
	static BOOL		LuagUserFindDevilSquareKeyEyes(lua_State * Lua)
	{
		BOOL Result = gUserFindDevilSquareKeyEyes(lua_tointeger(Lua, 1));
		lua_pushnumber(Lua, Result);
		return Result;
	};
	// ----
	static BOOL		LuaCGReqCheckSnowManNPC(lua_State * Lua)
	{
		CGReqCheckSnowManNPC(lua_tointeger(Lua, 1));
		return true;
	};
	// ----
	static BOOL		LuagObjAddBuffEffect(lua_State * Lua)
	{
		BOOL Result = gObjAddBuffEffect(&gObj[lua_tointeger(Lua, 1)], lua_tointeger(Lua, 2), lua_tointeger(Lua, 3), lua_tointeger(Lua, 4), lua_tointeger(Lua, 5), lua_tointeger(Lua, 6), lua_tointeger(Lua, 7));
		lua_pushnumber(Lua, Result);
		return Result;
	};
	// ----
	static BOOL		LuaGCReFillSend(lua_State * Lua)
	{
		GCReFillSend(lua_tointeger(Lua, 1), lua_tointeger(Lua, 2), lua_tointeger(Lua, 3), lua_tointeger(Lua, 4), lua_tointeger(Lua, 5));
		return true;
	};
	// ----
	static BOOL		LuaGCManaSend(lua_State * Lua)
	{
		GCManaSend(lua_tointeger(Lua, 1), lua_tointeger(Lua, 2), lua_tointeger(Lua, 3), lua_tointeger(Lua, 4), lua_tointeger(Lua, 5));
		return true;
	};
	// ----
	static BOOL		LuagQeustNpcTeleport_TalkRefAdd(lua_State * Lua)
	{
		gQeustNpcTeleport.TalkRefAdd();
		return true;
	};
	// ----
	static BOOL		LuagObjIsConnected(lua_State * Lua)
	{
		BOOL Result = gObjIsConnected(lua_tointeger(Lua, 1));
		lua_pushnumber(Lua, Result);
		return Result;
	};
	// ----
	static BOOL		LuaGCGuildMasterQuestionSend(lua_State * Lua)
	{
		GCGuildMasterQuestionSend(lua_tointeger(Lua, 1));
		return true;
	};
	// ----
	static BOOL		LuaGetWtf(lua_State * Lua)
	{
		lua_pushstring(Lua, lMsg.Get(MSGGET(lua_tointeger(Lua, 1), lua_tointeger(Lua, 2))));
		return true;
	};
	// ----
	static BOOL		LuagParty_GetPKPartyPenalty(lua_State * Lua)
	{
		BOOL Result = gParty.GetPKPartyPenalty(lua_tointeger(Lua, 1));
		lua_pushnumber(Lua, Result);
		return Result;
	};
	// ----
	static BOOL		LuagObjGuildMasterCapacityTest(lua_State * Lua)
	{
		BOOL Result = gObjGuildMasterCapacityTest(&gObj[lua_tointeger(Lua, 1)]);
		lua_pushnumber(Lua, Result);
		return Result;
	};
	// ----
	static BOOL		LuagObjMonsterTopHitDamageUser(lua_State * Lua)
	{
		BOOL Result = gObjMonsterTopHitDamageUser(&gObj[lua_tointeger(Lua, 1)]);
		lua_pushnumber(Lua, Result);
		return Result;
	};
	// ----
	static BOOL		LuaItemSerialCreateSend(lua_State * Lua)
	{
		ItemSerialCreateSend(
			lua_tointeger(Lua, 1), lua_tointeger(Lua, 2), 
			lua_tointeger(Lua, 3), lua_tointeger(Lua, 4),
			lua_tointeger(Lua, 5), lua_tointeger(Lua, 6),
			lua_tointeger(Lua, 7), lua_tointeger(Lua, 8),
			lua_tointeger(Lua, 9), lua_tointeger(Lua, 10),
			lua_tointeger(Lua, 11), lua_tointeger(Lua, 12),
			lua_tointeger(Lua, 13));
		return true;
	};
	// ----
	static BOOL		LuaMakeRandomSetItem(lua_State * Lua)
	{
		MakeRandomSetItem(lua_tointeger(Lua, 1));
		return true;
	};
	// ----
}; extern LuaExport gLuaExport;
// -------------------------------------------------------------------------------
