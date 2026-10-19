local core      = require("core")
local command   = require("core.command")
local keymap    = require("core.keymap")
local console   = require("plugins.console")

core.add_ignore {
    "%.dll$",
    "%.exe$",
    "%.lib$",
    "%.exp$",
    "%.obj$",
    "%.pdb$",
    "%.rdi$",
}

local emulator_exe = "NeonShooterDebug.exe"

local function build_emulator()
    system.exec("odin build emulator -define:RAYLIB_SHARED=true -debug -out:" .. emulator_exe)
end

command.add(nil, {
    ["project:build-project"] = function ()
        console.clear()

        local prefix = (PLATFORM == "Windows" and "" or "./")
        local ext = (PLATFORM == "Windows" and ".bat" or ".sh")
        core.log "Building..."
        console.run {
            command = prefix .. "build" .. ext,
            error_pattern = "Error:",
            warning_pattern = "Warning:",
            on_complete = function()
                core.log "Build finished, please check output!"
                console.log "Build finished..."
            end,
        }
    end,

    ["project:run-hot-reload"] = function ()
        -- console.clear()

        -- exec_spawn will not freezing the lite editor
        -- so we can doing hot reload game dll)
        build_emulator()
        system.exec_spawn(emulator_exe)
        -- core.log "Run game with hot reload"
        -- console.run {
        --     command = "odin run . -define:RAYLIB_SHARED=true",
        --     error_pattern = "[Error]",
        --     warning_pattern = "[Warning]",
        --     on_complete = function ()

        --     end
        -- }
    end,

    ["project:launch-debugger"] = function ()
        build_emulator()
        system.exec_spawn("raddbg " .. emulator_exe)
    end
})

keymap.add { ["ctrl+shift+b"] = "project:build-project" }
keymap.add { ["ctrl+shift+r"] = "project:run-hot-reload" }

