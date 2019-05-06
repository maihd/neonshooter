local MOJO_DIR = path.getabsolute(".")
local MOJO_BUILD_DIR = path.join(MOJO_DIR, "Build")

-- Load third party projects
dofile (path.join(MOJO_DIR, "mojo.thirdparty.premake5.lua"))

project "Mojo"
do
    kind "StaticLib"

    links {
        --"OpenAL"
    }

    defines {
    }

    -- third party
    thirdpartyfiles()
    thirdpartylinks()
    thirdpartydefines()
    thirdpartylibdirs()
    thirdpartyincludedirs()

    includedirs {
        path.join(MOJO_DIR, "Include"),
    }

    files {
        path.join(MOJO_DIR, "Include/Mojo/*.h"),
        path.join(MOJO_DIR, "Include/Mojo/**/*.h"),

        path.join(MOJO_DIR, "Sources/*.h"),
        path.join(MOJO_DIR, "Sources/*.c"),
        path.join(MOJO_DIR, "Sources/*.cc"),
        path.join(MOJO_DIR, "Sources/*.cpp"),
        
        path.join(MOJO_DIR, "Sources/Mojo_SDL2/*.h"),
        path.join(MOJO_DIR, "Sources/Mojo_SDL2/*.c"),
        path.join(MOJO_DIR, "Sources/Mojo_SDL2/*.cc"),
        path.join(MOJO_DIR, "Sources/Mojo_SDL2/*.cpp"),

        --path.join(MOJO_DIR, "Sources/**/*.h"),
        --path.join(MOJO_DIR, "Sources/**/*.c"),
        --path.join(MOJO_DIR, "Sources/**/*.cc"),
        --path.join(MOJO_DIR, "Sources/**/*.cpp"),
    }
end