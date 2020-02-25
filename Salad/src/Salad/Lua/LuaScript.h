#pragma once

#include <lua.hpp>

namespace Salad {

	class LuaScript {

	public:
		LuaScript(const char* luafile);
		~LuaScript();

		void openLibs();

		int getGlobalInt(const char* variable);

		void scopeToTable(const char* scopeName);
		int getInt(const char* variable);

	private:
		lua_State* m_LuaState;
	};
}