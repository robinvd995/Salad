#pragma once

#include <lua.hpp>

namespace Salad {

	class LuaScript {

	public:
		LuaScript(const std::string& filepath);
		~LuaScript();

		void printError(const std::string& variable, const std::string& reason);

		template<typename T>
		T get(const std::string& variableName);

		bool lua_gettostack(const std::string& variableName);

		template<typename T>
		T lua_get(const std::string& variableName) { return 0; }

		template<typename T>
		T lua_getDefault(const std::string& variableName);

	private:
		lua_State* m_LuaState;
	};
}