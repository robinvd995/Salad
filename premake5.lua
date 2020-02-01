workspace "Salad"
    architecture "x64"
    startproject "Sandbox"

    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Salad/vendor/GLFW/include"
IncludeDir["Glad"] = "Salad/vendor/Glad/include"
IncludeDir["ImGui"] = "Salad/vendor/imgui"
IncludeDir["glm"] = "Salad/vendor/glm"
IncludeDir["stb_image"] = "Salad/vendor/stb_image"
IncludeDir["lua53"] = "Salad/vendor/lua53/include"
IncludeDir["LuaBridge"] = "Salad/vendor/LuaBridge/Source"

group "Dependencies"
    include "Salad/vendor/GLFW"
    include "Salad/vendor/Glad"
    include "Salad/vendor/imgui"
    include "Salad/vendor/lua53"
group ""

project "Salad"
    location "Salad"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "sldpch.h"
    pchsource "Salad/src/sldpch.cpp"

    files {
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp"
    }

    defines{
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.lua53}",
        "%{IncludeDir.LuaBridge}"
    }

    links{
        "GLFW",
        "Glad",
        "ImGui",
        "lua53",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines{
            "SLD_PLATFORM_WINDOWS",
            "SLD_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "SLD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "SLD_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "SLD_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "Salad/vendor/spdlog/include",
        "Salad/src",
        "Salad/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.LuaBridge}"
    }

    links {
        "Salad"
    }

    filter "system:windows"
        systemversion "latest"

        defines{
            "SLD_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "SLD_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "SLD_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "SLD_DIST"
        runtime "Release"
        optimize "on"
