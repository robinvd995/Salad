project "lua53"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/lauxlib.h",
        "include/lua.h",
        "include/lua.hpp",
        "include/luaconf.h",
        "include/lualib.h",
        "src/**.c",
        "src/**.h"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
