package neonshooter_common

import "core:fmt"
import "core:strings"
import rl "vendor:raylib"

textures: map[string]rl.Texture

// @export
load_texture :: proc(path: string) -> rl.Texture {
    rl.TraceLog(.DEBUG, "load_texture")

    if textures == nil {
        textures = make(type_of(textures))
    }
    
    result, found := textures[path]
    if found {
        return result
    }

    result = rl.LoadTexture(strings.clone_to_cstring(path, context.temp_allocator))
    if result.id != 0 {
        textures[path] = result
    }

    return result
}

// @export
unload_texture_w_path :: proc(path: string) {
    result, found := textures[path]
    if found {
        rl.UnloadTexture(result)
    }

    delete_key(&textures, path)
}