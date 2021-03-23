workspace "Kursach"

    configurations { "Debug", "Release" }
    
    platforms { "x86", "x86_64" }

    startproject "App"

    
bindirPath = "%{wks.location}/build/bin/%{cfg.buildcfg}-%{cfg.architecture}"
objdirPath = "%{wks.location}/build/obj/%{cfg.buildcfg}-%{cfg.architecture}"

IncludeDir = {}

include "Renderer/"
include "App/"
