package neonshooter_game

import rl "vendor:raylib"

when ODIN_OS == .Darwin do foreign import common "../common.dylib"
when ODIN_OS == .Windows do foreign import common "../common.dll"

foreign common {
    load_texture :: proc "odin" (path: string) -> rl.Texture ---
    unload_texture_w_path :: proc "odin" (path: string) ---
}