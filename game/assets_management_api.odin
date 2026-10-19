package neonshooter_game

import "core:fmt"
import "core:strings"
import "core:hash"

import rl "vendor:raylib"

Assets :: struct {
    textures_indexor:   Hash_Indexor,
    textures:           [Hash_Indexor_Num_Items]rl.Texture,
    num_textures:       i32,
}

load_texture :: proc(assets: ^Assets, path: string) -> rl.Texture {
    rl.TraceLog(.DEBUG, "load_texture")

    path_hash := hash.murmur64a(transmute([]u8)path)
    index, found := hi_get(&assets.textures_indexor, path_hash)
    if found {
        return assets.textures[index]
    }

    result := rl.LoadTexture(strings.clone_to_cstring(path, context.temp_allocator))
    if result.id != 0 {
        idx := assets.num_textures

        hi_add(&assets.textures_indexor, path_hash, idx)

        assets.textures[idx] = result
        assets.num_textures += 1
    }

    return result
}

unload_texture_w_path :: proc(assets: ^Assets, path: string) {
    path_hash := hash.murmur64a(transmute([]u8)path)

    idx, removed := hi_remove(&assets.textures_indexor, path_hash)
    if removed {
        last_idx := assets.num_textures - 1

        assets.textures[idx] = assets.textures[last_idx]
        hi_set(&assets.textures_indexor, path_hash, last_idx)

        assets.num_textures -= 1
    }
}

unload_all_textures :: proc(assets: ^Assets) {
    for texture in assets.textures {
        rl.UnloadTexture(texture)
    }

    assets.textures_indexor = {}
    assets.textures = {}
}

// unload_texture :: proc{unload_texture_w_path, unload_all_textures}

unload_texture :: proc{unload_texture_w_path}
