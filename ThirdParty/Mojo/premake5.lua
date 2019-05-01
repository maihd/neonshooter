local MOJO_DIR = path.getabsolute(".")
local MOJO_BUILD_DIR = path.join(MOJO_DIR, "Build")

workspace "Mojo"
do
    language "C++"
    location (MOJO_BUILD_DIR)

    platforms { "x32", "x64" }
    configurations { "Debug", "Release" }

    configuration {}

    rtti "Off"
    exceptionhandling "Off"
    
    flags {
    }
end

-- Load mojo project
dofile (path.join(MOJO_DIR, "mojo.premake5.lua"))

project "Mojo.UnitTests"
do
    kind "ConsoleApp"

    links {
        "Mojo"
    }

    includedirs {
        path.join(MOJO_DIR, "include")
    }

    files {
        path.join(MOJO_DIR, "Tests/*.h"),
        path.join(MOJO_DIR, "Tests/*.c"),
        path.join(MOJO_DIR, "Tests/*.cc"),
        path.join(MOJO_DIR, "Tests/*.cpp"),

        path.join(MOJO_DIR, "Tests/**/*.h"),
        path.join(MOJO_DIR, "Tests/**/*.c"),
        path.join(MOJO_DIR, "Tests/**/*.cc"),
        path.join(MOJO_DIR, "Tests/**/*.cpp"),
    }
end