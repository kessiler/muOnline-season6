// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// requires CWinConsole decompilation
// ------------------------------

#include "stdafx.h"
#include "MuLua.h"

#include "GameMain.h"
#include "logproc.h"
#include "WinConsole.h"

MULua g_MuLuaQuestExp;
int MuRequire(lua_State* state);

MULua::MULua(void):
m_luaState(NULL)
{
	Create();
}


MULua::~MULua(void)
{
	Release();
}

bool MULua::DoFile(const char* szFileName)
{
	int iState = 0;

	if ( luaL_loadfile(m_luaState, szFileName) )
	{		
		MsgBox("Error!!, : %s\n", lua_tolstring(m_luaState, -1, 0) );
		return false;
	}

    iState = lua_pcall(m_luaState, 0, -1, 0);

	if ( iState )
	{
		MsgBox("Error!!, : %s : State = %d\n", lua_tolstring(m_luaState, -1, 0) , iState);
		return false;
	}

	return true;	
}

bool MULua::DoFile(lua_State* L, const char* szFileName)
{
	return true;
}

bool MULua::DoString(std::string kString)
{
	return true;
}

lua_State* MULua::GetLua()
{
	return m_luaState;
}

bool MULua::Create()
{
	m_luaState = luaL_newstate();

	luaL_openlibs(m_luaState);
	lua_pushcclosure(m_luaState, MuRequire, 0);
	lua_setfield(m_luaState, LUA_GLOBALSINDEX, "murequire");
	lua_gc(m_luaState, LUA_GCCOLLECT, 0);

	return true;
}

void MULua::Register(void* pLua)
{
	lua_State* pLuaState = (lua_State*)pLua;
}

void MULua::Release()
{
	if(m_luaState)
	{
		lua_close(m_luaState);
		m_luaState = NULL;
	}
}

// no references found
void MULua::CreateWinConsole(HINSTANCE hInstance)
{
	HWND hWnd = NULL;

	hWnd = CWinConsole::StartConsole(hInstance, m_luaState);

	if ( hWnd )
		ShowWindow(hWnd, SW_HIDE);

}

// no references found
void MULua::DestroyWinConsole()
{
	CWinConsole::StopConsole();
}


static const int sentinel_ = 0;
#define sentinel	((void *)&sentinel_)


int MuRequire(lua_State* L)
{
	//char *name = NULL;
	char name[MAX_PATH] = { 0 };
	sprintf(name, "..\\Data\\Lua\\%s", (LPCSTR)luaL_checklstring(L, 1, 0));
	//name = gDirPath.GetNewPath((LPCSTR)luaL_checklstring(L, 1, 0) );
	lua_settop(L, 1);

	lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
	lua_getfield(L, 2, name);

	if ( lua_toboolean(L, -1) )
	{
		if ( lua_touserdata(L, -1) == sentinel )
			luaL_error(L, "loop or previous error loading module '%s'", name);

		return 1;
	}


	if ( luaL_loadfile(L, name) )
	{
		luaL_error(L, "error loading module '%s' from file '%s':\n\t%s", 
			lua_tolstring(L, 1, 0), name, lua_tolstring(L, -1, 0));
	}

	lua_pushlightuserdata(L, sentinel);
	lua_setfield(L, 2, name);
	lua_pushstring(L, name);
	lua_call(L, 1, 1);

	if ( lua_type(L, -1) )
		lua_setfield(L, 2, name);

	lua_getfield(L, 2, name);

	if ( lua_touserdata(L, -1) == sentinel )
	{
		lua_pushboolean(L, 1);
		lua_pushvalue(L, -1);
		lua_setfield(L, 2, name);
	}

	return 1;
}