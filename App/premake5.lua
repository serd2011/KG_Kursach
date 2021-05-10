project "App"
    kind "WindowedApp"
    language "C++"
    cdialect "C11"
    cppdialect "C++17"

    characterset ("Unicode")

    targetdir (bindirPath .. "/%{prj.name}")
    objdir (objdirPath .. "/%{prj.name}")

    includedirs {
        "%{wks.location}/Renderer/src",
        "src"
    };

    files { 
        "./src/**.h",
        "./src/**.cpp",
        "app.manifest"
    }

    links{
        "comctl32.lib",
        "Renderer"
    }

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

    filter "action:vs*"
        buildoptions {"/utf-8"}
