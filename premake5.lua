workspace "AsciiRenderer"
    architecture "x64"
    startproject "AsciiRenderer"

    configurations
    {
        -- only debug for this hobby project
        "Debug",
        "Release"
    }

-- variables
    -- cfg - configuration
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "AsciiRenderer"
    location "."
    kind "ConsoleApp"
    language "C"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "include/**.h",
        "src/**.c"
    }

    includedirs
    {
        "include",
        "vendor/include"
    }

    libdirs
    {
        "vendor/lib"
    }

    links
    {
        "glfw3.lib"
    }

    postbuildcommands
    {
        -- ("{COPY}   \"vendor/dll/x64/freeglut.dll\"   \"bin/" .. outputdir .. "/%{prj.name}/\"")
    }

    -- everything under this filter only applies to windows
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "PLATFORM_WINDOWS",
            "_USE_MATH_DEFINES"
        }

    filter { "configurations:Debug" }
        symbols "On"
    
    filter { "configurations:Release" }
        optimize "On"