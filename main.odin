package neonshooter

import "core:strings"
import "core:c/libc"
import "core:path/slashpath"
import "core:fmt"
import "core:os"
import "core:dynlib"
import rl "vendor:raylib"

main :: proc() {
    rl.SetConfigFlags({ .VSYNC_HINT })

    rl.InitWindow(800, 600, "Neon Shooter - Odin + Raylib");
    defer rl.CloseWindow()

    build_game_dll()

    when ODIN_OS == .Windows {
        game_api_file := "game.dll"
    } else when ODIN_OS == .Darwin {
        game_api_file := "game.dylib"
    } else {
        game_api_file := "game.so"
    }

    game_api_version := 0
    game_api, game_api_ok := load_game_api(game_api_file, game_api_version)
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

            memory := game_api.get_memory()
            game_api = new_api
            game_api.hot_reload(memory)

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
    update: proc(),
    render: proc(),
    get_memory: proc() -> any,
    hot_reload: proc(memory: any),

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
        get_memory = cast(proc() -> any)(dynlib.symbol_address(lib, "game_memory") or_else nil),
        hot_reload = cast(proc(any))(dynlib.symbol_address(lib, "game_hot_reload") or_else nil),
    }

    if api.init == nil || api.deinit == nil || api.update == nil || api.render == nil \ 
        || api.get_memory == nil \
        || api.hot_reload == nil {
        dynlib.unload_library(api.lib)
        rl.TraceLog(.FATAL, "Game library missing required precedures")
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