local THIRDPARTY_DIR = path.getabsolute("ThirdParty")

function thirdpartyfiles()
    files {
        path.join(MOJO_DIR, "ThirdParty/glew-2.1.0/src/glew.c")
    }
end

function thirdpartylinks()
    links {
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
            path.join(THIRDPARTY_DIR, "OpenAL-1.1/libs/Win32"),
        }
    end

    filter { "platforms:x64" }
    do
        libdirs {
            path.join(THIRDPARTY_DIR, "OpenAL-1.1/libs/Win64"),
        }
    end

    filter {}
end

function thirdpartyincludedirs()
    includedirs {
        path.join(THIRDPARTY_DIR, "stb"),
        path.join(THIRDPARTY_DIR, "glew-2.1.0/include"),
        path.join(THIRDPARTY_DIR, "OpenAL-1.1/include"),
    }
end