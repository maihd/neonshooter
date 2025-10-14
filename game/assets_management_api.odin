package neonshooter_game

import rl "vendor:raylib"

when ODIN_OS == .Darwin do foreign import common "../common.dylib"
when ODIN_OS == .Windows do foreign import common "../common.lib"

@(default_calling_convention = "odin")
foreign common {
    load_texture :: proc(path: string) -> rl.Texture ---

    @(private = "file")
    unload_texture_w_path :: proc(path: string) ---

    unload_all_textures :: proc() ---
}

unload_texture :: proc { unload_texture_w_path }

// import "../common"

// load_texture :: common.load_texture
// unload_texture :: proc { common.unload_texture_w_path}