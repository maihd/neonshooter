local THIRDPARTY_DIR = path.getabsolute("ThirdParty")

function thirdpartyfiles()
    files {
        path.join(MOJO_DIR, "ThirdParty/glew-2.1.0/src/glew.c")
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
            path.join(THIRDPARTY_DIR, "SDL2-2.0.9/lib/Win32"),
            path.join(THIRDPARTY_DIR, "OpenAL-Soft-1.19.1/libs/Win32"),
        }

        postbuildcommands {
            --"{COPY} ".. path.join(THIRDPARTY_DIR, "SDL2-2.0.9/lib/Win32/SDL2.dll") .. " %{cfg.targetdir}",
            --"{COPY} ".. path.join(THIRDPARTY_DIR, "OpenAL-Soft-1.19.1/libs/Win32/OpenAL32.dll") .. " %{cfg.targetdir}"
        }
    end

    filter { "platforms:x64" }
    do
        libdirs {
            path.join(THIRDPARTY_DIR, "SDL2-2.0.9/lib/Win64"),
            path.join(THIRDPARTY_DIR, "OpenAL-Soft-1.19.1/libs/Win64"),
        }

        postbuildcommands {
            --"{COPY} ".. path.join(THIRDPARTY_DIR, "SDL2-2.0.9/lib/Win64/SDL2.dll") .. " %{cfg.targetdir}",
            --"{COPY} ".. path.join(THIRDPARTY_DIR, "OpenAL-Soft-1.19.1/libs/Win64/OpenAL32.dll") .. " %{cfg.targetdir}"
        }
    end

    filter {}
end

function thirdpartyincludedirs()
    includedirs {
        path.join(THIRDPARTY_DIR, "stb"),
        path.join(THIRDPARTY_DIR, "glew-2.1.0/include"),
        path.join(THIRDPARTY_DIR, "SDL2-2.0.9/include"),
        path.join(THIRDPARTY_DIR, "OpenAL-Soft-1.19.1/include"),
    }
end