local THIRDPARTY_DIR = path.getabsolute("ThirdParty")

function thirdpartyfiles()
    files {
        path.join(MOJO_DIR, "ThirdParty/.Sources/glew-2.1.0/src/glew.c")
    }
end

function thirdpartylinks()
    links {
        "SDL2",
        "OpenAL32"
    }

    linkoptions {
        --"/wholearchive:OpenAL32"
    }
end

function thirdpartydefines()
    defines {
        "GLEW_STATIC",
        "AL_LIBTYPE_STATIC",
    }
end

function thirdpartylibdirs()
    filter { "platforms:x32" }
    do
        libdirs {
            path.join(THIRDPARTY_DIR, "libs/Win32"),
        }

        postbuildcommands {
        }
    end

    filter { "platforms:x64" }
    do
        libdirs {
            path.join(THIRDPARTY_DIR, "libs/Win64"),
        }

        postbuildcommands {
        }
    end

    filter {}
end

function thirdpartyincludedirs()
    includedirs {
        path.join(THIRDPARTY_DIR, "include"),
        path.join(THIRDPARTY_DIR, ".Sources/stb"),
        path.join(THIRDPARTY_DIR, ".Sources/glew-2.1.0/include"),
    }
end