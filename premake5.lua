local ROOT_DIR = path.getabsolute(".")
local BUILD_DIR = path.join(PROJ_DIR, "Projects")
local THIRD_PARTY_DIR = path.join(ROOT_DIR, "ThirdParty")

local PROJECT_PREFIX = "NeonShooter"

-- Load MaiLib
local MAIDLIB_DIR = path.join(THIRD_PARTY_DIR, "MaiLib")
local MaiLib = dofile(path.join(MAIDLIB_DIR, "premake5.mailib.lua"))

workspace (PROJECT_PREFIX)
do
    language "C++"
    location (path.join(BUILD_DIR, string.upper(_ACTION)))

    platforms { "x32", "x64" }
    configurations { "Debug", "Release" }

    configuration {}

    MaiLib.cflags()
    
    startproject (PROJECT_PREFIX)

    filter {}
end

-- Game project
project (PROJECT_PREFIX)
do
    kind "ConsoleApp"

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

    filter "action:vs*"
    do
        linkoptions {
            "/SAFESEH:NO"
        }
    end

    filter {}

    MaiLib.links(MAIDLIB_DIR)
    MaiLib.files(MAIDLIB_DIR)
    MaiLib.includedirs(MAIDLIB_DIR)

    filter {}
end