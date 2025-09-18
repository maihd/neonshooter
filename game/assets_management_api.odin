package neonshooter_game

import rl "vendor:raylib"

when ODIN_OS == .Darwin do foreign import common "../common.dylib"
when ODIN_OS == .Windows do foreign import common "../common.dll"

@(default_calling_convention = "odin")
foreign common {
    load_texture :: proc(path: string) -> rl.Texture ---

    @(private = "file")
    unload_texture_w_path :: proc(path: string) ---
}

unload_texture :: proc { unload_texture_w_path }