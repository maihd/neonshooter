/*
Navigation (code region):
- init
- main
- update
- render
*/

package neonshooter

// @region import

import "core:mem"
import "base:runtime"
import "core:math"
import "core:math/rand"
import "core:strings"
import "core:c/libc"
import "core:path/slashpath"
import "core:fmt"
import "core:os"
import "core:time"
import "core:dynlib"

import "core:path/filepath"
import win32 "core:sys/windows"

import rl "vendor:raylib"
import gxm "../gxm"
import "../seh"

// @region fixed time step clock

Fixed_Clock :: struct {
    tps:                i32,
    timestep_seconds:   f64,
    timestep_fixed:     i64,
    max_timestep_fixed: i64,

    last_tick:          time.Tick,
    accumulator:        i64,
}

init_clock :: proc(tps: i32, max_timestep_fixed := i64(250_000_000)) -> Fixed_Clock {
    return Fixed_Clock{
        tps                 = tps,
        timestep_seconds    = 1.0 / f64(tps),
        timestep_fixed      = i64(1_000_000_000 / i64(tps)),
        max_timestep_fixed  = max_timestep_fixed,

        last_tick           = time.tick_now(),
        accumulator         = 0.0,
    }
}

update_clock :: proc(clock: ^Fixed_Clock) {
    duration     := time.tick_lap_time(&clock.last_tick)
    frame_micros := time.duration_nanoseconds(duration)

    clock.accumulator += frame_micros
    if  clock.accumulator > clock.max_timestep_fixed {
        clock.accumulator = clock.max_timestep_fixed
    }
}

step_clock :: proc(clock: ^Fixed_Clock) -> (f32, bool) {
    if  clock.accumulator >= clock.timestep_fixed {
        clock.accumulator -= clock.timestep_fixed

        return f32(clock.timestep_seconds), true
    } else {
        return f32(0.0), false
    }
}

clock_alpha :: proc(clock: ^Fixed_Clock) -> (_seconds: f32) {
    _seconds = f32(f64(clock.accumulator) / 1_000_000_000)
    return
}

// @region main

Emulator_State :: enum {
    Normal,
    GameCrashed,
}

main :: proc() {
    track: mem.Tracking_Allocator
    mem.tracking_allocator_init(&track, context.allocator)
    context.allocator = mem.tracking_allocator(&track)

    defer {
        if len(track.allocation_map) > 0 {
            fmt.eprintf("=== %v allocations not freed: ===\n", len(track.allocation_map))
            for _, entry in track.allocation_map {
                fmt.eprintf("- %v bytes @ %v\n", entry.size, entry.location)
            }
        } else {
            fmt.printfln("NO MEMORY LEAK")
        }
        mem.tracking_allocator_destroy(&track)
    }

    rl.SetConfigFlags({ .VSYNC_HINT, .WINDOW_HIGHDPI })

    rl.InitWindow(1024, 768, "Neon Shooter - Odin + Raylib");
    defer rl.CloseWindow()

    when ODIN_OS == .Windows {
        game_api_file := "temp/game.dll"
    } else when ODIN_OS == .Darwin {
        game_api_file := "temp/game.dylib"
    } else {
        game_api_file := "temp/game.so"
    }

    game_api, game_api_ok := load_game_api(game_api_file, 0)
    if !game_api_ok {
        fmt.println("Failed to load Game API")
        return
    }
    defer {
        unload_game_api(game_api)
    }

    // @todo(maihd): doing SEH here
    memory_size := game_api.get_memory_size()

    state: rawptr       = raw_data(make([]u8, memory_size))
    prev_state: rawptr  = raw_data(make([]u8, memory_size))
    temp_state: rawptr  = raw_data(make([]u8, memory_size))

    game_api.init(state)
    mem.copy_non_overlapping(prev_state, state, memory_size)
    mem.copy_non_overlapping(temp_state, state, memory_size)

    defer {
        game_api.deinit(state)

        free(temp_state)
        free(prev_state)
        free(state)
    }

    pause := false
    emulator_state := Emulator_State.Normal

    hot_reload_timer := f32(0.0)
    hot_reload_interval := f32(1.0)

    fixed_clock := init_clock(60)

    for !rl.WindowShouldClose() {
        // Clear temp allocations
        free_all(context.temp_allocator)

        // @region hot reload

        hot_reload_timer += rl.GetFrameTime()
        if hot_reload_timer >= hot_reload_interval {
            hot_reload_timer -= hot_reload_interval

            dll_time, dll_time_ok := os.last_write_time_by_name(game_api.lib_path)
            reload := dll_time_ok == nil && game_api.dll_time != dll_time
            if reload {
                seh_ctx := seh.begin();
                defer seh.end(seh_ctx)

                if seh_ctx.code == .None {
                    new_api, ok := load_game_api(game_api.lib_path, game_api.api_version + 1)
                    if ok {
                        new_api.hot_reload(game_api.get_memory())

                        unload_game_api(game_api)
                        game_api = new_api
                    } else {
                        // skip this dll version
                        game_api.dll_time = dll_time
                    }
                } else {
                    emulator_state = .GameCrashed
                    fmt.printf("SEH Exception occurred when doing game_api.hot_reload(): %v\n", seh_ctx.code)
                }
            }

            // Game usually paused when I change to code
            // So we can safe skip the frame
            continue
        }

        // @region update

        if rl.IsKeyPressed(.SPACE) && pause && emulator_state == .Normal {
            pause = false
        }

        if !rl.IsWindowFocused() {
            pause = true
        }

        frame_time := rl.GetFrameTime()

        if !pause {
            seh_ctx := seh.begin();

            if seh_ctx.code == .None {
                update_clock(&fixed_clock)

                for dt in step_clock(&fixed_clock) {
                // for fixed_clock.accumulator >= fixed_clock.timestep_fixed {
                //     fixed_clock.accumulator -= fixed_clock.timestep_fixed

                    mem.copy_non_overlapping(prev_state, state, memory_size)

                    game_api.process_input(state, dt)

                    game_api.tick(state, dt, Game_Tick_Flags_All)
                }
            } else {
                emulator_state = .GameCrashed
                fmt.printf("SEH Exception occurred when doing game_api.update(): %v\n", seh_ctx.code)
            }

            seh.end(seh_ctx)
        }

        // @region render

        rl.BeginDrawing()
            rl.ClearBackground(rl.BLACK)

            seh_ctx := seh.begin(); {
                if seh_ctx.code == .None {
                    mem.copy_non_overlapping(temp_state, state, memory_size)

                    alpha      := clock_alpha(&fixed_clock)
                    tick_flags := Game_Tick_Flags_All - {.Spawning}

                    game_api.tick(temp_state, alpha, tick_flags)
                    game_api.render(temp_state)
                } else {
                    emulator_state = .GameCrashed
                    fmt.printf("SEH Exception occurred when doing game_api.render(): %v\n", seh_ctx.code)
                }

                seh.end(seh_ctx)
            }

            if pause {
                width, height := rl.GetScreenWidth(), rl.GetScreenHeight()
                rl.DrawRectangle(0, 0, width, height, { 0, 0, 0, 200 })

                text := cstring("GAME IS PAUSE. PRESS SPACE TO CONTINUE")
                text_width := rl.MeasureText(text, 64)

                rl.DrawText(text, (width - text_width) / 2, (height) / 2, 64, rl.RAYWHITE)
            }

            rl.DrawFPS(10, 10)

            // rl.DrawText(rl.TextFormat("Ticks: %d", i32(game_state.tick_count)), 10, 40, 16, rl.WHITE)
            // rl.DrawText(rl.TextFormat("Frames: %d", i32(game_state.frame_count)), 10, 70, 16, rl.WHITE)
        rl.EndDrawing()

        if emulator_state != .Normal {
            pause = true
        }
    }
}

// @region game api

Game_Tick_Kind  :: enum(u32) { Spawning, Movement, Particle }
Game_Tick_Flags :: bit_set[Game_Tick_Kind; u32]

Game_Tick_Flags_All :: Game_Tick_Flags{ .Spawning, .Movement, .Particle }

Game_Api :: struct {
    init:               proc(state: rawptr),
    deinit:             proc(state: rawptr),

    process_input:      proc(state: rawptr, dt: f32),
    tick:               proc(state: rawptr, dt: f32, tick_kind: Game_Tick_Flags),
    render:             proc(state: rawptr),

    hot_reload:         proc(memory: rawptr),
    get_memory:         proc() -> rawptr,
    get_memory_size:    proc() -> int,

    lib:                dynlib.Library,
    lib_path:           string,
    dll_time:           time.Time,
    api_version:        int,
}

load_game_api :: proc(path: string, api_version: int) -> (Game_Api, bool) {
    fmt.printfln("Load game api...")

    dll_time, dll_time_err := os.last_write_time_by_name(path)
    if dll_time_err != nil {
        fmt.printf("Could not fetch last write date of %s\n", path);
        return {}, false
    }

    file_name := slashpath.name(path)
    file_ext := slashpath.ext(path)

    target_dll_name := fmt.tprintf("%s_%d%s", file_name, api_version, file_ext)

    fmt.printfln("Clone dll to temp dll file: %s", target_dll_name)
    copy_err := os.copy_file(target_dll_name, path)
    if copy_err != nil {
        fmt.printfln("Failed to copy %s to %s. Error: %v", path, target_dll_name, copy_err)
        return {}, false
    }

    fmt.printfln("Load temp dll file: %s", target_dll_name)
    lib, lib_ok := dynlib.load_library(target_dll_name)
    if !lib_ok {
        fmt.println("Failed loading game DLL")
        return {}, false
    }

    api: Game_Api
    api = {
        lib             = lib,
        lib_path        = strings.clone(path),
        dll_time        = dll_time,
        api_version     = api_version,

        init            = cast(type_of(api.init))(dynlib.symbol_address(lib, "game_init") or_else nil),
        deinit          = cast(type_of(api.deinit))(dynlib.symbol_address(lib, "game_deinit") or_else nil),

        process_input   = cast(type_of(api.process_input))(dynlib.symbol_address(lib, "game_process_input") or_else nil),
        tick            = cast(type_of(api.tick))(dynlib.symbol_address(lib, "game_tick") or_else nil),
        render          = cast(type_of(api.render))(dynlib.symbol_address(lib, "game_render") or_else nil),

        hot_reload      = cast(type_of(api.hot_reload))(dynlib.symbol_address(lib, "game_hot_reload") or_else nil),
        get_memory      = cast(type_of(api.get_memory))(dynlib.symbol_address(lib, "game_memory") or_else nil),
        get_memory_size = cast(type_of(api.get_memory_size))(dynlib.symbol_address(lib, "game_memory_size") or_else nil),
    }

    if api.init == nil || api.deinit == nil \
        || api.process_input == nil || api.tick == nil || api.render == nil \
        || api.get_memory == nil \
        || api.hot_reload == nil {
        unload_game_api(api)
        rl.TraceLog(.FATAL, "Game library missing required precedures")
        return {}, false
    }

    return api, true
}

unload_game_api :: proc(api: Game_Api) {
    fmt.printfln("Unload game api...")

    if api.lib != nil {
        dynlib.unload_library(api.lib)
    }

    file_name := slashpath.name(api.lib_path)
    file_ext := slashpath.ext(api.lib_path)

    when ODIN_OS == .Windows {
        copy_cmd_fmt := "del \"\""
    } else {
        copy_cmd_fmt := "rm -f \"%s_%d%s\""
    }

    filename := fmt.tprintf("%s_%d%s", file_name, api.api_version, file_ext)
    del_err := os.remove(filename)
    if del_err != nil {
        fmt.printfln("Failed to remove temp dll file: %s", filename)
    }

    delete(api.lib_path)

    fmt.printfln("Unload game api finished")
}

// @region Crash handler
