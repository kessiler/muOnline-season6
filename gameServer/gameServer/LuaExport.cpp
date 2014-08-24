#include "StdAfx.h"
#include "LuaExport.h"
// -------------------------------------------------------------------------------

LuaExport gLuaExport;
// -------------------------------------------------------------------------------

LuaExport::LuaExport()
{
	// ----
}
// -------------------------------------------------------------------------------

LuaExport::~LuaExport()
{
	// ----
}
// -------------------------------------------------------------------------------

void LuaExport::AddTable(lua_State * Lua)
{
	lua_register(Lua, "ChatTargetSend",					this->LuaChatTargetSend);
	lua_register(Lua, "LogAddTD",						this->LuaLogAddTD);
	lua_register(Lua, "gObjTeleport",					this->LuagObjTeleport);
	lua_register(Lua, "GCServerMsgStringSend",			this->LuaGCServerMsgStringSend);
	lua_register(Lua, "FireCrackerOpen",				this->LuaFireCrackerOpen);
	lua_register(Lua, "gObjCalDistance",				this->LuagObjCalDistance);
	lua_register(Lua, "GCServerCmd",					this->LuaGCServerCmd);
	lua_register(Lua, "gUserFindDevilSquareKeyEyes",	this->LuagUserFindDevilSquareKeyEyes);
	lua_register(Lua, "gObjAddBuffEffect",				this->LuagObjAddBuffEffect);
	lua_register(Lua, "GCReFillSend",					this->LuaGCReFillSend);
	lua_register(Lua, "GCManaSend",						this->LuaGCManaSend);
	lua_register(Lua, "gQeustNpcTeleport_TalkRefAdd",	this->LuagQeustNpcTeleport_TalkRefAdd);
	lua_register(Lua, "gObjIsConnected",				this->LuagObjIsConnected);
	lua_register(Lua, "GCGuildMasterQuestionSend",		this->LuaGCGuildMasterQuestionSend);
	lua_register(Lua, "GetWtf",							this->LuaGetWtf);
	lua_register(Lua, "gParty_GetPKPartyPenalty",		this->LuagParty_GetPKPartyPenalty);
	lua_register(Lua, "gObjGuildMasterCapacityTest",	this->LuagObjGuildMasterCapacityTest);
	lua_register(Lua, "gObjMonsterTopHitDamageUser",	this->LuagObjMonsterTopHitDamageUser);
	lua_register(Lua, "ItemSerialCreateSend",			this->LuaItemSerialCreateSend);
	lua_register(Lua, "MakeRandomSetItem",				this->LuaMakeRandomSetItem);
}
// -------------------------------------------------------------------------------

void LuaExport::AddGlobal(lua_State * Lua)
{
	lua_pushinteger(Lua, gEnableServerDivision);
	lua_setglobal(Lua, "gEnableServerDivision");
	// ----
	lua_pushinteger(Lua, gGuildCreate);
	lua_setglobal(Lua, "gGuildCreate");
	// ----
	lua_pushinteger(Lua, gLanguage);
	lua_setglobal(Lua, "gLanguage");
	// ----
	lua_pushinteger(Lua, gEnableEventNPCTalk);
	lua_setglobal(Lua, "gEnableEventNPCTalk");
	// ----
	lua_pushinteger(Lua, gDevilSquareEvent);
	lua_setglobal(Lua, "gDevilSquareEvent");
	// ----
	lua_pushinteger(Lua, gPkLimitFree);
	lua_setglobal(Lua, "gPkLimitFree");
}
// -------------------------------------------------------------------------------