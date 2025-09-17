package neonshooter_game

import "core:fmt"
import rl "vendor:raylib"

Game_State :: struct {
    player_pos: rl.Vector2,
    player_tex: rl.Texture
}

game_state: ^Game_State

@(export)
game_init :: proc() {
    if game_state == nil {
        game_state = new(Game_State)
    }

    if game_state.player_tex.id == 0 {
        game_state.player_tex = load_texture(Assets_Art_Player_Png)
        game_state.player_pos = { cast(f32)rl.GetScreenWidth() * 0.5, cast(f32)rl.GetScreenHeight() * 0.5}
    }
}

@(export)
game_deinit :: proc() {
    free(game_state)
}

@(export)
game_update :: proc() {
    dt := rl.GetFrameTime()

    speed := f32(100)

    if rl.IsKeyDown(.W) || rl.IsKeyDown(.UP) {
        game_state.player_pos.y -= speed * dt;
    }

    if rl.IsKeyDown(.S) || rl.IsKeyDown(.DOWN) {
        game_state.player_pos.y += speed * dt;
    }

    if rl.IsKeyDown(.A) || rl.IsKeyDown(.LEFT) {
        game_state.player_pos.x -= speed * dt;
    }

    if rl.IsKeyDown(.D) || rl.IsKeyDown(.RIGHT) {
        game_state.player_pos.x += speed * dt;
    }
}

@(export)
game_render :: proc() {
    rl.DrawTexture(game_state.player_tex, i32(game_state.player_pos.x), i32(game_state.player_pos.y), rl.WHITE)
}

@(export)
game_memory :: proc() -> rawptr {
    return game_state
}

@(export)
game_hot_reload :: proc(memory: rawptr) {
    game_state = cast(^Game_State)memory

    if game_state == nil {
        game_init()
    }

    if game_state.player_tex.id == 0 {
        game_init()
    }
}