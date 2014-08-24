// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------


#pragma once

#include "../Lua/include/lua.hpp"

class MULua
{
public:
	MULua(void);
	~MULua(void);

private:
	bool Create();

public:
	void Release();

	bool DoFile(const char* szFileName);
	bool DoFile(lua_State* L, const char* szFileName);

	bool DoString(std::string kString);

	lua_State* GetLua();

	void Register(void* pLua);


	void CreateWinConsole(HINSTANCE hInstance);
	void DestroyWinConsole();

private:
	lua_State*	m_luaState;
};

extern MULua g_MuLuaQuestExp;