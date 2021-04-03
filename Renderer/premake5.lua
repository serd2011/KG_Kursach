project "Renderer"
    kind "StaticLib"
    language "C++"
    cdialect "C11"
    cppdialect "C++17"

    characterset ("Unicode")

    targetdir (bindirPath .. "/%{prj.name}")
    objdir (objdirPath .. "/%{prj.name}")

    includedirs (
       path.join(_SCRIPT_DIR,"src")
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
        optimize "On"

    filter "action:vs*"
        buildoptions {"/utf-8"}

    postbuildcommands { 
        "{COPYDIR} \"./src/*.h\" \"" .. bindirPath .. "/%{prj.name}/include/\""
    }

IncludeDir["Renderer"] = bindirPath .. "/Renderer/include/"
