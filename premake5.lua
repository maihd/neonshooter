local ROOT_DIR = path.getabsolute(".")
local BUILD_DIR = path.join(ROOT_DIR, "Build")
local THIRDPARTY_DIR = path.join(ROOT_DIR, "ThirdParty")

workspace "NeonShooter"
do
    language "C++"
    location (BUILD_DIR)

    platforms { "x32", "x64" }
    configurations { "Debug", "Release" }

    --cdialect "c99"
    --compileas "C++"
    --staticruntime "On"

    flags {
        "NoPCH"
    }

    filter { "configurations:Debug"}
    do
        defines {
            "DEBUG"
        }
    end

    filter { "configurations:Release"}
    do
        defines {
            "RELEASE"
        }
    end

    filter {} 
end

project "MaiLibC"
do
    kind "StaticLib"

    links {
    }

    includedirs { 
        path.join(ROOT_DIR, "MaiLibC/Include"),
        path.join(ROOT_DIR, "ThirdParty/Include"),
    }

    files {
        path.join(ROOT_DIR, "MaiLibC/Include/*.h"),
        path.join(ROOT_DIR, "MaiLibC/Include/**/*.h"),
        path.join(ROOT_DIR, "MaiLibC/Sources/*.c"),
        path.join(ROOT_DIR, "MaiLibC/Sources/**/*.c"),
        path.join(ROOT_DIR, "MaiLibC/Sources/*.cpp"),
        path.join(ROOT_DIR, "MaiLibC/Sources/**/*.cpp"),

        path.join(THIRDPARTY_DIR, "Include/raylib.h"),
        path.join(THIRDPARTY_DIR, "Include/raymath.h"),
    }

    filter {}
end

project "NeonShooter"
do
    local shouldHideConsole = true
    if not shouldHideConsole then
        kind "ConsoleApp"
    else 
        filter { "configurations:Debug" }
        do
            kind "ConsoleApp"
        end

        filter { "configurations:Release" }
        do
            kind "WindowedApp"
        end
        
        filter {}
    end

    links {
        "MaiLibC",
        "raylib_static",
        "winmm",
    }

    includedirs {
        path.join(ROOT_DIR, "MaiLibC/Include"),
        path.join(ROOT_DIR, "ThirdParty/Include"),
    }

    files {
        path.join(ROOT_DIR, "Sources", "*.h"),
        path.join(ROOT_DIR, "Sources", "**/*.h"),
        path.join(ROOT_DIR, "Sources", "*.c"),
        path.join(ROOT_DIR, "Sources", "**/*.c"),
        path.join(ROOT_DIR, "Sources", "*.cpp"),
        path.join(ROOT_DIR, "Sources", "**/*.cpp"),
    }

    defines {
        "GRAPHICS_API_OPENGL_33",
        "PLATFORM_DESKTOP"
    }

    filter { "platforms:x32" }
    do
        libdirs { path.join(ROOT_DIR, "ThirdParty/Library/Win32") }
    end

    filter { "platforms:x64" }
    do
        libdirs { path.join(ROOT_DIR, "ThirdParty/Library/Win64") }
    end

    filter {}
end