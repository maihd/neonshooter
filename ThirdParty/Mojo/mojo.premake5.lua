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

    local function filedirs(dirs)
        if type(dirs) == "string" then
            files {
                path.join(MOJO_DIR, dirs, "*.h"),
                path.join(MOJO_DIR, dirs, "*.c"),
                path.join(MOJO_DIR, dirs, "*.cc"),
                path.join(MOJO_DIR, dirs, "*.cpp"),
            }
        elseif type(dirs) == "table" then
            for _, dir in ipairs(dirs) do
                filedirs(dir)
            end
        end
    end


    files {
        path.join(MOJO_DIR, "Include/Mojo/*.h"),
        path.join(MOJO_DIR, "Include/Mojo/**/*.h"),
    }

    filedirs {
        "Sources",
        "Sources/Core",
        
        "Sources/Graphics",
        "Sources/Graphics_OpenGL",

        "Sources/Audio_OpenAL",

        "Sources/Native",
        "Sources/Native_SDL2",
    }
end