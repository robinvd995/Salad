group "Tests"
    include "test/third_party/googletest/googletest"

    project "Salad-Test"
        kind "ConsoleApp"
        location "Salad-Test"
        language "C++"
        cppdialect "C++17"
        staticruntime "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/test/**.c",
            "%{prj.name}/test/**.h",
            "%{prj.name}/test/**.hpp",
            "%{prj.name}/test/**.cpp"
        }

        includedirs {
            "third_party/googletest/googletest/include",

            "%{wks.location}/Salad/src",
            "%{wks.location}/Salad/vendor/spdlog/include",

            "%{wks.location}/%{IncludeDir.GLFW}",
            "%{wks.location}/%{IncludeDir.Glad}",
            "%{wks.location}/%{IncludeDir.glm}",
            "%{wks.location}/%{IncludeDir.stb_image}",
            "%{wks.location}/%{IncludeDir.entt}",
            "%{wks.location}/%{IncludeDir.json}",
            "%{wks.location}/%{IncludeDir.ImGuizmo}",
            "%{wks.location}/%{IncludeDir.lua53}",
            "%{wks.location}/%{IncludeDir.LuaBridge}",
            "%{wks.location}/%{IncludeDir.freetype}",
            "%{wks.location}/%{IncludeDir.libzip}",
            "%{VulkanInclude}"
        }

        links {
            "googletest",
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

    project "Salad-Bowl-Test"
        kind "ConsoleApp"
        location "Salad-Bowl-Test"
        language "C++"
        cppdialect "C++17"
        staticruntime "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/test/**.c",
            "%{prj.name}/test/**.h",
            "%{prj.name}/test/**.hpp",
            "%{prj.name}/test/**.cpp"
        }

        includedirs {
            "third_party/googletest/googletest/include",

            "%{wks.location}/Salad/src",
            "%{wks.location}/Salad-Bowl/src",
            "%{wks.location}/Salad/vendor/spdlog/include",

            "%{wks.location}/%{IncludeDir.glm}",
            "%{wks.location}/%{IncludeDir.entt}",
            "%{wks.location}/%{IncludeDir.ImGuizmo}",
            "%{wks.location}/%{IncludeDir.json}",
            "%{wks.location}/%{IncludeDir.libzip}",
            "%{wks.location}/%{IncludeDir.rapidxml}",
            "%{wks.location}/%{IncludeDir.assimp}",
            "%{VulkanInclude}"
        }

        links {
            "googletest",
            "Salad",
            "Salad-Bowl"
        }

        filter "system:windows"
            systemversion "latest"
            defines {
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

    project "Sandbox-Test"
        kind "ConsoleApp"
        location "Sandbox-Test"
        language "C++"
        cppdialect "C++17"
        staticruntime "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/test/**.c",
            "%{prj.name}/test/**.h",
            "%{prj.name}/test/**.hpp",
            "%{prj.name}/test/**.cpp"
        }

        includedirs {
            "third_party/googletest/googletest/include",

            "%{wks.location}/Salad/src",
            "%{wks.location}/Salad/vendor/spdlog/include",

            "%{wks.location}/Salad/vendor",
            "%{wks.location}/%{IncludeDir.glm}",
            "%{wks.location}/%{IncludeDir.entt}",
            "%{wks.location}/%{IncludeDir.LuaBridge}",
            "%{wks.location}/%{IncludeDir.Glad}",
            "%{VulkanInclude}"
        }

        links {
            "googletest",
            "Salad",
            "Sandbox"
        }

        filter "system:windows"
            systemversion "latest"
            defines {
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

group ""
