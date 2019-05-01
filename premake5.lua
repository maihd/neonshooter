local PROJ_DIR = path.getabsolute(".")
local PROJ_BUILD_DIR = path.join(PROJ_DIR, "Build")

workspace "NeonShooter"
do
    language "C++"
    location (PROJ_BUILD_DIR)

    platforms { "x32", "x64" }
    configurations { "Debug", "Release" }

    configuration {}

    rtti "Off"
    exceptionhandling "Off"
    
    startproject "NeonShooter"

    filter {}
end

-- Load mojo project
dofile (path.join(PROJ_DIR, "ThirdParty/Mojo/mojo.premake5.lua"))

-- Game project
project "NeonShooter"
do
    kind "ConsoleApp"

    links {
        "Mojo"
    }

    includedirs {
        path.join(PROJ_DIR, "ThirdParty/Mojo/include"),
    }

    files {
        path.join(PROJ_DIR, "Sources/*.h"),
        path.join(PROJ_DIR, "Sources/*.c"),
        path.join(PROJ_DIR, "Sources/*.cc"),
        path.join(PROJ_DIR, "Sources/*.cpp"),

        path.join(PROJ_DIR, "Sources/**/*.h"),
        path.join(PROJ_DIR, "Sources/**/*.c"),
        path.join(PROJ_DIR, "Sources/**/*.cc"),
        path.join(PROJ_DIR, "Sources/**/*.cpp"),
    }
end