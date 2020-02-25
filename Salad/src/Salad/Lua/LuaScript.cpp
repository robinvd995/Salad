#include "sldpch.h"
#include "LuaScript.h"

namespace Salad {

	LuaScript::LuaScript(const char* luafile) {
		m_LuaState = luaL_newstate();
		luaL_dofile(m_LuaState, luafile);
	}

	LuaScript::~LuaScript() {
		lua_close(m_LuaState);
	}

	void LuaScript::openLibs() {
		luaL_openlibs(m_LuaState);
	}

	int LuaScript::getGlobalInt(const char* variable) {
		int value = 0;
		lua_getglobal(m_LuaState, variable);
		SLD_CORE_ASSERT(lua_isnumber(m_LuaState, -1), "Variable {0} is not a number!")
		value = lua_tonumber(m_LuaState, -1);
		return value;
	}

	void LuaScript::scopeToTable(const char* scopeName) {
		lua_getglobal(m_LuaState, scopeName);
		SLD_CORE_ASSERT(lua_istable(m_LuaState, -1), "Could not scope to table {0}!", scopeName);
	}

	int LuaScript::getInt(const char* variable) {
		lua_pushstring(m_LuaState, variable);
		lua_gettable(m_LuaState, -2);
		int value = lua_tointeger(m_LuaState, -1);
		lua_pop(m_LuaState, 1);
		return value;
	}

}
