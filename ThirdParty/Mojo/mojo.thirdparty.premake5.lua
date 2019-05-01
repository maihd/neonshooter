local THIRDPARTY_DIR = path.getabsolute("ThirdParty")

local OPENAL_DIR = path.join(THIRDPARTY_DIR, "openal-soft-1.19.1")

function thirdpartyfiles()
    files {
        path.join(MOJO_DIR, "ThirdParty/glew-2.1.0/src/glew.c")
    }
end

function thirdpartylinks()
    links {
        --"OpenAL"
    }
end

function thirdpartydefines()
    defines {
        "GLEW_STATIC",
        "AL_LIBTYPE_STATIC",
    }
end

function thirdpartylibdirs()
    libdirs {
    }
end

function thirdpartyincludedirs()
    includedirs {
        path.join(THIRDPARTY_DIR, "stb"),
        path.join(THIRDPARTY_DIR, "glew-2.1.0/include"),
        path.join(THIRDPARTY_DIR, "openal-soft-1.19.1/include"),
    }
end

--project "OpenAL"
--do
--    kind "StaticLib"
--
--    defines {
--        "AL_LIBTYPE_STATIC"
--    }
--
--    includedirs {
--        path.join(OPENAL_DIR, "Alc"),
--        path.join(OPENAL_DIR, "common"),
--        path.join(OPENAL_DIR, "include"),
--        path.join(OPENAL_DIR, "OpenAL32/Include"),
--    }
--
--    files {
--        path.join(OPENAL_DIR, "Alc/*.h"),
--        path.join(OPENAL_DIR, "Alc/*.c"),
--        path.join(OPENAL_DIR, "common/*.h"),
--        path.join(OPENAL_DIR, "common/*.c"),
--        path.join(OPENAL_DIR, "OpenAL32/*.c"),
--        path.join(OPENAL_DIR, "OpenAL32/Include/*.h"),
--    }
--end