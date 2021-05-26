project "Renderer"
    kind "StaticLib"
    language "C++"
    cdialect "C11"
    cppdialect "C++17"

    characterset ("Unicode")

    targetdir (bindirPath .. "/%{prj.name}")
    objdir (objdirPath .. "/%{prj.name}")

    includedirs (
       "src"
    );

    files { 
        "./src/**.h",
        "./src/**.cpp"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "Speed"

    filter "action:vs*"
        buildoptions {"/utf-8"}
