workspace "Salad"
    architecture "x64"
    startproject "Salad-Bowl"

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

IncludeDir["entt"] = "Salad/vendor/entt/include"
IncludeDir["json"] = "Salad/vendor/nlohmann/include"

IncludeDir["lua53"] = "Salad/vendor/lua53/include"
IncludeDir["LuaBridge"] = "Salad/vendor/LuaBridge/Source"
IncludeDir["freetype"] = "Salad/vendor/freetype/include"
IncludeDir["ImGuizmo"] = "Salad/vendor/ImGuizmo"

IncludeDir["libzip"] = "Salad/vendor/libzip"

--Salad-Bowl Include
IncludeDir["assimp"] = "Salad-Bowl/vendor/assimp/include"

group "Dependencies"
    include "Salad/vendor/GLFW"
    include "Salad/vendor/Glad"
    include "Salad/vendor/imgui"
    include "Salad/vendor/lua53"
    include "Salad/vendor/freetype"
    include "Salad/vendor/libzip"
    include "Salad-Bowl/vendor/assimp"
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
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
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
        "%{IncludeDir.entt}",
        "%{IncludeDir.json}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.lua53}",
        "%{IncludeDir.LuaBridge}",
        "%{IncludeDir.freetype}",
        "%{IncludeDir.libzip}"
    }

    links{
        "GLFW",
        "Glad",
        "ImGui",
        "lua53",
        "freetype",
        "libzip",
        "opengl32.lib"
    }

    filter "files:Salad/vendor/ImGuizmo/**.cpp"
        flags {"NoPCH"}

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
        "%{IncludeDir.entt}",
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

project "Salad-Bowl"
        location "Salad-Bowl"
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
            "%{prj.name}/src",

            "Salad/vendor/spdlog/include",
            "Salad/src",
            "Salad/vendor",
            "%{IncludeDir.glm}",
            "%{IncludeDir.entt}",
            "%{IncludeDir.ImGuizmo}",
            "%{IncludeDir.json}",
            "%{IncludeDir.libzip}",

            IncludeDir["assimp"]
        }
    
        links {
            "Salad",
            "assimp"
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
