package neonshooter

import "core:strings"
import "core:c/libc"
import "core:path/slashpath"
import "core:fmt"
import "core:os"
import "core:dynlib"
import rl "vendor:raylib"

when ODIN_OS == .Windows {
    GAME_DLL_PATH :: "game.dll"
} else {
    GAME_DLL_PATH :: "game.so"
}

main :: proc() {
    rl.SetConfigFlags({ .VSYNC_HINT })

    rl.InitWindow(800, 600, "Neon Shooter - Odin + Raylib");
    defer rl.CloseWindow()

    build_game_dll()

    game_api_version := 0
    game_api, game_api_ok := load_game_api("game.dll", game_api_version)
    if !game_api_ok {
        fmt.println("Failed to load Game API")
        return
    }
    defer {
        unload_game_api(game_api)
    }

    game_api_version += 1

    game_api.init()
    defer game_api.deinit()

    // rl.SetTargetFPS(60)

    for !rl.WindowShouldClose() {
        dll_time := os.last_write_time_by_name(game_api.lib_path) or_continue
        reload := game_api.dll_time != dll_time
        if reload {
            new_api := load_game_api(game_api.lib_path, game_api_version) or_continue
            
            unload_game_api(game_api)

            game_api.deinit()
            game_api = new_api
            game_api.init()

            game_api_version += 1
        }

        game_api.update()
        
        rl.BeginDrawing()
        defer rl.EndDrawing()

        rl.ClearBackground(rl.BLACK)
        defer rl.DrawFPS(10, 10)

        game_api.render()
    }
}

build_game_dll :: proc() {
    libc.system("odin build game -build-mode:dll -define:RAYLIB_SHARED=true")
}

Game_Api :: struct {
    init: proc(),
    deinit: proc(),
    reload: proc(),
    update: proc(),
    render: proc(),

    lib: dynlib.Library,
    lib_path: string,
    dll_time: os.File_Time,
    api_version: int,
}

load_game_api :: proc(path: string, api_version: int) -> (Game_Api, bool) {
    dll_time, dll_time_err := os.last_write_time_by_name(path)
    if dll_time_err != nil {
        fmt.printf("Could not fetch last write date of %s\n", path);
        return {}, false
    }

    file_name := slashpath.name(path)
    file_ext := slashpath.ext(path)

    target_dll_name := fmt.tprintf("%s_%d.%s", file_name, api_version, file_ext)

    when ODIN_OS == .Windows {
        copy_cmd_fmt := "copy \"%s\" \"%s\""
    } else {
        copy_cmd_fmt := "cp \"%s\" \"%s\""
    }

    copy_cmd := fmt.ctprintf(copy_cmd_fmt, path, target_dll_name)
    if libc.system(copy_cmd) != 0 {
        fmt.println("Failed to copy %s to %s", path, target_dll_name)
    }

    lib, lib_ok := dynlib.load_library(target_dll_name)
    if !lib_ok {
        fmt.println("Failed loading game DLL")
        return {}, false
    }

    api := Game_Api {
        lib = lib,
        lib_path = strings.clone(path),
        dll_time = dll_time,
        api_version = api_version,

        init = cast(proc())(dynlib.symbol_address(lib, "game_init") or_else nil),
        deinit = cast(proc())(dynlib.symbol_address(lib, "game_deinit") or_else nil),
        update = cast(proc())(dynlib.symbol_address(lib, "game_update") or_else nil),
        render = cast(proc())(dynlib.symbol_address(lib, "game_render") or_else nil),
    }

    if api.init == nil || api.deinit == nil || api.update == nil || api.render == nil{
        dynlib.unload_library(api.lib)
        fmt.println("Game library missing required precedures")
        return {}, false
    }

    return api, true
}

unload_game_api :: proc(api: Game_Api) {
    defer delete(api.lib_path)

    if api.lib != nil {
        dynlib.unload_library(api.lib)
    }

    file_name := slashpath.name(api.lib_path)
    file_ext := slashpath.ext(api.lib_path)

    when ODIN_OS == .Windows {
        copy_cmd_fmt := "del \"%s_%d.%s\""
    } else {
        copy_cmd_fmt := "rm -f \"%s_%d.%s\""
    }

    del_cmd := fmt.ctprintf(copy_cmd_fmt, file_name, api.api_version, file_ext)
    libc.system(del_cmd)
}