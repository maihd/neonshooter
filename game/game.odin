package neonshooter_game

import "base:runtime"
import "core:fmt"
import rl "vendor:raylib"

Game_State :: struct {
    entity_system: Entity_System,
    player: ^Entity_Player,
}

game_state: ^Game_State

@(export)
game_init :: proc() {
    if game_state == nil {
        game_state = new(Game_State)
    }

    entity_system_init(&game_state.entity_system)

    if game_state.player == nil {
        player := Entity_Player {
            texture = load_texture(Assets_Art_Player_Png),
            position = { cast(f32)rl.GetScreenWidth() * 0.5, cast(f32)rl.GetScreenHeight() * 0.5},
            rotation = 0,
            scale = {1, 1},
            hp = 100
        }
        entity := entity_system_add(&game_state.entity_system, player)
        game_state.player = cast(^Entity_Player)entity
    }
}

@(export)
game_deinit :: proc() {
    defer free(game_state)

    if game_state != nil {
        entity_system_deinit(&game_state.entity_system)
        unload_texture(Assets_Art_Player_Png)
    }
}

@(export)
game_update :: proc() {
    dt := rl.GetFrameTime()

    speed := f32(300)

    dir: Vec2;

    if rl.IsKeyDown(.W) || rl.IsKeyDown(.UP) {
        dir.y -= 1;
    }

    if rl.IsKeyDown(.S) || rl.IsKeyDown(.DOWN) {
        dir.y += 1;
    }

    if rl.IsKeyDown(.A) || rl.IsKeyDown(.LEFT) {
        dir.x -= 1;
    }

    if rl.IsKeyDown(.D) || rl.IsKeyDown(.RIGHT) {
        dir.x += 1;
    }

    game_state.player.position += speed * dir * dt
    if lensqr(dir) > 0 {
        game_state.player.rotation = angle(dir)
    }

    entity_system_update(&game_state.entity_system, dt)
}

@(export)
game_render :: proc() {
    entity_system_render(&game_state.entity_system)
}

@(export)
game_memory :: proc() -> runtime.Raw_Any {
    // fmt.printf("game_state: %p\n", game_state)
    return runtime.Raw_Any { data = game_state, id = type_of(game_state) }
}

@(export)
game_hot_reload :: proc(memory: runtime.Raw_Any) {
    // fmt.printf("raw_any.type: %v\n", memory.id)
    // fmt.printf("raw_any.data: %v\n", memory.data)

    game_state = transmute(^Game_State)memory.data

    if game_state == nil {
        game_init()
    }
}