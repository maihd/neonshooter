package neonshooter_game

import "base:runtime"
import "core:fmt"
import rl "vendor:raylib"

Game_State :: struct {
    entity_system: Entity_System,
    player: ^Entity_Player,

    fire_timer: f32,
    fire_rate: f32,
}

game_state: ^Game_State

@(export)
game_init :: proc() {
    if game_state == nil {
        game_state = new(Game_State)
    }

    game_state.fire_rate = 0.1
    game_state.fire_timer = 0.0

    entity_system_init(&game_state.entity_system)

    // Preload
    load_texture(Assets_Art_Player_Png)
    load_texture(Assets_Art_Bullet_Png)
    load_texture(Assets_Art_Seeker_Png)
    load_texture(Assets_Art_Wanderer_Png)

    if game_state.player == nil {
        player_texture := load_texture(Assets_Art_Player_Png)
        player := Entity_Player {
            texture = player_texture,
            position = { cast(f32)rl.GetScreenWidth() * 0.5, cast(f32)rl.GetScreenHeight() * 0.5},
            rotation = 0,
            scale = {1, 1},
            radius = f32(player_texture.width),
            hp = 100
        }
        entity_handle := entity_system_add(&game_state.entity_system, player)
        game_state.player = entity_system_get(&game_state.entity_system, entity_handle, Entity_Player)
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
game_update :: proc(dt: f32) {
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

    if rl.IsMouseButtonDown(.LEFT) {
        game_state.fire_timer += dt
        if game_state.fire_timer >= game_state.fire_rate {
            game_state.fire_timer -= game_state.fire_rate

            bullet_dir := norm(rl.GetMousePosition() - game_state.player.position)
            bullet_vel := bullet_dir * 1000

            bullet_dir_angle := angle(bullet_dir)
            bullet_pos1 := game_state.player.position + vec2_from_angle(bullet_dir_angle - 0.15) * (game_state.player.radius + 10)
            bullet_pos2 := game_state.player.position + vec2_from_angle(bullet_dir_angle + 0.15) * (game_state.player.radius + 10)

            bullet1 := Entity_Bullet {
                position = bullet_pos1,
                texture = load_texture(Assets_Art_Bullet_Png),
                rotation = bullet_dir_angle,
                scale = vec2(1),
                hp = 1,
                velocity = bullet_vel
            }
            entity_system_add(&game_state.entity_system, bullet1)

            bullet2 := Entity_Bullet {
                position = bullet_pos2,
                texture = load_texture(Assets_Art_Bullet_Png),
                rotation = bullet_dir_angle,
                scale = vec2(1),
                hp = 1,
                velocity = bullet_vel
            }
            entity_system_add(&game_state.entity_system, bullet2)
        }
    }

    bullets_iter := entity_system_iter_by_type(&game_state.entity_system, Entity_Bullet)
    for bullet in bullets_iter->next() {
        bullet.position += bullet.velocity * dt
        bullet.rotation = angle(bullet.velocity)

        if bullet.position.x < 0 \
            || bullet.position.y < 0 \
            || bullet.position.x > f32(rl.GetScreenWidth()) \
            || bullet.position.y > f32(rl.GetScreenHeight()) 
        {
            entity_system_destroy(&game_state.entity_system, cast(^Entity)bullet)
        }
    }
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