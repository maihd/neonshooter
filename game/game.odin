package neonshooter_game

import "core:math"
import "core:math/rand"
import "base:runtime"
import "core:fmt"
import rl "vendor:raylib"

Game_State :: struct {
    entity_system: Entity_System,

    player_handle: Entity_Handle,

    fire_timer: f32,
    fire_rate: f32,

    spawn_seeker_rate: int,
    spawn_seeker_timer: f32,
    spawn_seeker_interval: f32,

    spawn_wanderer_rate: int,
    spawn_wanderer_timer: f32,
    spawn_wanderer_interval: f32,
}

game_state: ^Game_State

@(export)
game_init :: proc() {
    if game_state == nil {
        game_state = new(Game_State)
    }

    game_state.fire_rate = 0.1
    game_state.fire_timer = 0.0

    game_state.spawn_seeker_rate = 20
    game_state.spawn_seeker_timer = 0.0
    game_state.spawn_seeker_interval = 1.0

    game_state.spawn_wanderer_rate = 30
    game_state.spawn_wanderer_timer = 0.0
    game_state.spawn_wanderer_interval = 1.0

    entity_system_init(&game_state.entity_system)

    // Preload
    load_all_textures()

    if game_state.player_handle == Entity_Handle(0) {
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
        game_state.player_handle = entity_handle
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

    player := entity_system_get(&game_state.entity_system, game_state.player_handle, Entity_Player)
    player.position += speed * dir * dt
    if lensqr(dir) > 0 {
        player.rotation = angle(dir)
    }

    entity_system_update(&game_state.entity_system, dt)

    if rl.IsMouseButtonDown(.LEFT) {
        game_state.fire_timer += dt
        if game_state.fire_timer >= game_state.fire_rate {
            game_state.fire_timer -= game_state.fire_rate

            bullet_dir := norm(rl.GetMousePosition() - player.position)
            bullet_vel := bullet_dir * 1000

            bullet_dir_angle := angle(bullet_dir)
            bullet_pos1 := player.position + vec2_from_angle(bullet_dir_angle - 0.15) * (player.radius + 10)
            bullet_pos2 := player.position + vec2_from_angle(bullet_dir_angle + 0.15) * (player.radius + 10)

            bullet1 := Entity_Bullet {
                position = bullet_pos1,
                texture = load_texture(Assets_Art_Bullet_Png),
                rotation = bullet_dir_angle,
                scale = vec2(1),
                hp = 1,
                attack = 1,
                velocity = bullet_vel
            }
            entity_system_add(&game_state.entity_system, bullet1)

            bullet2 := Entity_Bullet {
                position = bullet_pos2,
                texture = load_texture(Assets_Art_Bullet_Png),
                rotation = bullet_dir_angle,
                scale = vec2(1),
                hp = 1,
                attack = 1,
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
            entity_system_destroy(&game_state.entity_system, bullet)
        }
    }

    game_state.spawn_seeker_timer += dt
    if game_state.spawn_seeker_timer >= game_state.spawn_seeker_interval {
        game_state.spawn_seeker_timer -= game_state.spawn_seeker_interval

        if rand.float32_range(0, 100) <= f32(game_state.spawn_seeker_rate) {
            texture := load_texture(Assets_Art_Seeker_Png)
            seeker := Entity_Seeker {
                hp = 10,
                attack = 1,
                defense = 1,

                position = player.position, // @fixme for test
                rotation = 0,
                scale = vec2(1),
                radius = f32(texture.width) * 0.65,

                texture = texture,
                tint = rl.WHITE,

                velocity = vec2(0.0),
                target = game_state.player_handle,

                spawn_timer = 1.0,
            }
            entity_system_add(&game_state.entity_system, seeker)
        }
    }

    seekers_iter := entity_system_iter_by_type(&game_state.entity_system, Entity_Seeker)
    for seeker in seekers_iter->next() {
        target := entity_system_get(&game_state.entity_system, seeker.target, Entity_Player)
        if target != nil {
            seeker.velocity = norm(target.position - seeker.position + vec2(math.F32_EPSILON)) * 10 // @fixme for test
            seeker.rotation = angle(seeker.velocity)
            seeker.position = seeker.position + seeker.velocity * dt
        }
    }

    bullets_iter = entity_system_iter_by_type(&game_state.entity_system, Entity_Bullet)
    for bullet in bullets_iter->next() {
        seekers_iter = entity_system_iter_by_type(&game_state.entity_system, Entity_Seeker)
        for seeker in seekers_iter->next() {
            dist := distsqr(bullet.position, seeker.position)
            if dist <= (bullet.radius + seeker.radius) * (bullet.radius + seeker.radius) {
                seeker.hp -= bullet.attack
                if seeker.hp <= 0 {
                    entity_system_destroy(&game_state.entity_system, seeker)
                }
                entity_system_destroy(&game_state.entity_system, bullet)
                break
            }
        }
    }

    if rl.IsKeyPressed(.GRAVE) {
        fmt.printf("entity_system.entities_by_type: %v\n", game_state.entity_system.entities_by_type)
    }
}

@(export)
game_render :: proc() {
    entity_system_render(&game_state.entity_system)

    rl.DrawText(rl.TextFormat("Enitities: %d", i32(len(game_state.entity_system.entities))), 10, 40, 16, rl.WHITE)
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