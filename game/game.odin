package neonshooter_game

import "core:math"
import "core:math/rand"
import "base:runtime"
import "core:fmt"
import rl "vendor:raylib"

GAME_DEBUGGING :: #config(GAME_DEBUGGING, true)

Game_Input :: struct {
    fire_button_down: bool,
    move_direction: Vec2,
}

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

    // Rendering
    camera: rl.Camera2D,
    particle_system: Particle_System,

    // Input
    tick_input: Game_Input,

    // Game loop and timer
    tick_count: int,
    frame_count: int,
    accumulator: f32,
    fixed_timestep: f32,

    // Debug
    debug_flags: bit_set[Debug_Flags]
}

Debug_Flags :: enum {
    Hitbox,
}

game_state: ^Game_State

@(export)
game_init :: proc() {
    if game_state == nil {
        game_state = new(Game_State)
    }

    game_state.camera = rl.Camera2D {
        target = {},
        offset = {},
        rotation = 0.0,
        zoom = 1.0
    }

    game_state.fire_rate = 0.1
    game_state.fire_timer = 0.0

    game_state.spawn_seeker_rate = 20
    game_state.spawn_seeker_timer = 0.0
    game_state.spawn_seeker_interval = 1.0

    game_state.spawn_wanderer_rate = 30
    game_state.spawn_wanderer_timer = 0.0
    game_state.spawn_wanderer_interval = 1.0

    game_state.fixed_timestep = 1.0 / 60.0
    game_state.accumulator = 0

    entity_system_init(&game_state.entity_system)
    particle_system_init(&game_state.particle_system)

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
        particle_system_deinit(&game_state.particle_system)
        entity_system_deinit(&game_state.entity_system)
    }
}

@(export)
game_update :: proc(dt: f32) {
    game_state.frame_count += 1

    // Hot keys
    if rl.IsKeyPressed(.GRAVE) {
        game_state.debug_flags ~= { .Hitbox }
    }

    // Update input

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

    game_state.tick_input.move_direction = dir
    game_state.tick_input.fire_button_down = rl.IsMouseButtonDown(.LEFT)

    // Ticking the game

    game_state.accumulator += dt
    num_ticks := int(math.floor(game_state.accumulator / game_state.fixed_timestep))
    game_state.accumulator -= f32(num_ticks) * game_state.fixed_timestep

    if num_ticks > 0 {
        game_state.tick_input.move_direction /= f32(num_ticks)

        for i in 0..<num_ticks {
            when GAME_DEBUGGING {
                can_ticks := rl.IsWindowFocused()
            } else {
                can_ticks := true
            }

            if can_ticks {
                game_state.tick_count += 1
                game_tick(game_state.fixed_timestep)
            }
        }

        game_state.tick_input.move_direction = vec2(0)
        game_state.tick_input.fire_button_down = false
    }
}

@(private = "file")
game_tick :: proc(dt: f32) {
    speed := f32(300)

    entity_system_update(&game_state.entity_system, dt)
    particle_system_update(&game_state.particle_system, dt)
    
    dir := game_state.tick_input.move_direction
    if lensqr(dir) > 0 {
        dir = norm(dir)
    }

    player := entity_system_get(&game_state.entity_system, game_state.player_handle, Entity_Player)
    player.position += speed * dir * dt
    if lensqr(dir) > 0 {
        player.rotation = angle(dir)
    }

    if player.position.x - player.radius < 0 {
        player.position.x = player.radius
    }

    if player.position.x + player.radius > f32(rl.GetScreenWidth()) {
        player.position.x = f32(rl.GetScreenWidth()) - player.radius
    }

    if player.position.y - player.radius < 0 {
        player.position.y = player.radius
    }

    if player.position.y + player.radius > f32(rl.GetScreenHeight()) {
        player.position.y = f32(rl.GetScreenHeight()) - player.radius
    }

    if game_state.tick_input.fire_button_down {
        game_state.fire_timer += dt
        if game_state.fire_timer >= game_state.fire_rate {
            game_state.fire_timer -= game_state.fire_rate

            bullet_dir := norm(rl.GetMousePosition() - player.position)
            bullet_vel := bullet_dir * 1000

            bullet_dir_angle := angle(bullet_dir)
            bullet_pos1 := player.position + vec2_from_angle(bullet_dir_angle - 0.15) * (player.radius + 10)
            bullet_pos2 := player.position + vec2_from_angle(bullet_dir_angle + 0.15) * (player.radius + 10)

            texture := load_texture(Assets_Art_Bullet_Png)

            bullet1 := Entity_Bullet {
                position = bullet_pos1,
                texture = texture,
                rotation = bullet_dir_angle,
                scale = vec2(1),
                hp = 1,
                attack = 1,
                velocity = bullet_vel,
                radius = f32(texture.width) * 0.2
            }
            entity_system_add(&game_state.entity_system, bullet1)

            bullet2 := Entity_Bullet {
                position = bullet_pos2,
                texture = load_texture(Assets_Art_Bullet_Png),
                rotation = bullet_dir_angle,
                scale = vec2(1),
                hp = 1,
                attack = 1,
                velocity = bullet_vel,
                radius = f32(texture.width) * 0.2
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
            explose_bullet(bullet)
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

                position = get_spawn_position(player, 100, 300),
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

    game_state.spawn_wanderer_timer += dt
    if game_state.spawn_wanderer_timer >= game_state.spawn_wanderer_interval {
        game_state.spawn_wanderer_timer -= game_state.spawn_wanderer_interval

        if rand.float32_range(0, 100) <= f32(game_state.spawn_wanderer_rate) {
            texture := load_texture(Assets_Art_Wanderer_Png)
            wanderer := Entity_Wanderer {
                hp = 10,
                attack = 1,
                defense = 1,

                position = get_spawn_position(player, 100, 300),
                rotation = 0,
                scale = vec2(1),
                radius = f32(texture.width) * 0.65,

                texture = texture,
                tint = rl.WHITE,

                velocity = vec2(0.0),

                spawn_timer = 1.0,
            }
            entity_system_add(&game_state.entity_system, wanderer)
        }
    }

    seekers_iter := entity_system_iter_by_type(&game_state.entity_system, Entity_Seeker)
    for seeker in seekers_iter->next() {
        target := entity_system_get(&game_state.entity_system, seeker.target, Entity_Player)
        if target != nil {
            seeker.velocity = norm(target.position - seeker.position + vec2(math.F32_EPSILON)) * 100 // @fixme for test
            seeker.rotation = angle(seeker.velocity)
            seeker.position = seeker.position + seeker.velocity * dt
        }
    }

    wanderers_iter := entity_system_iter_by_type(&game_state.entity_system, Entity_Wanderer)
    for wanderer in wanderers_iter->next() {
        dir := rand.float32_range(-0.3, 0.3)
        wanderer.velocity = lerp(wanderer.velocity, vec2_from_angle(dir) * 30, 3 * dt)
        wanderer.rotation = angle(wanderer.velocity)
        wanderer.position = wanderer.position + wanderer.velocity * dt
    }

    bullets_iter = entity_system_iter_by_type(&game_state.entity_system, Entity_Bullet)
    for bullet in bullets_iter->next() {
        collided := false

        seekers_iter = entity_system_iter_by_type(&game_state.entity_system, Entity_Seeker)
        for seeker in seekers_iter->next() {
            dist := distsqr(bullet.position, seeker.position)
            if dist <= (bullet.radius + seeker.radius) * (bullet.radius + seeker.radius) {
                collided = true

                seeker.hp -= bullet.attack
                if seeker.hp <= 0 {
                    explose_seeker(seeker)
                }

                entity_system_destroy(&game_state.entity_system, bullet)
                break
            }
        }

        if collided {
            continue
        }

        wanderers_iter = entity_system_iter_by_type(&game_state.entity_system, Entity_Wanderer)
        for wanderer in wanderers_iter->next() {
            dist := distsqr(bullet.position, wanderer.position)
            if dist <= (bullet.radius + wanderer.radius) * (bullet.radius + wanderer.radius) {
                collided = true

                wanderer.hp -= bullet.attack
                if wanderer.hp <= 0 {
                    explose_wanderer(wanderer)
                }
                entity_system_destroy(&game_state.entity_system, bullet)
                break
            }
        }
    }
}

@(private = "file")
explose_seeker :: proc(seeker: ^Entity_Seeker) {
    texture := load_texture(Assets_Art_Laser_Png)

    hue1 := rand.float32_range(0.0, 6.0)
    hue2 := math.mod(hue1 + rand.float32_range(0.0, 2.0), 6.0)

    color1 := vec4_hsv(hue1, 0.5, 1.0)
    color2 := vec4_hsv(hue2, 0.5, 1.0)

    for i in 0..<120 {
        speed := rand.float32_range(0.2, 1.0) * 640
        dir_angle := rand.float32() * math.PI * 2
        paritcle := Particle {
            texture = texture,
            position = seeker.position,
            velocity = vec2_from_angle(dir_angle, speed),
            rotation = dir_angle,
            scale = vec2(1),
            decay = 0,
            duration = 1,
            life = 1,
            tint = color_from_vec4(lerp(color1, color2, rand.float32())),
        }
        particle_system_spawn(&game_state.particle_system, paritcle)
    }

    entity_system_destroy(&game_state.entity_system, seeker)
}

@(private = "file")
explose_wanderer :: proc(wanderer: ^Entity_Wanderer) {
    texture := load_texture(Assets_Art_Laser_Png)

    hue1 := rand.float32_range(0.0, 6.0)
    hue2 := math.mod(hue1 + rand.float32_range(0.0, 2.0), 6.0)

    color1 := vec4_hsv(hue1, 0.5, 1.0)
    color2 := vec4_hsv(hue2, 0.5, 1.0)

    for i in 0..<120 {
        speed := rand.float32_range(0.2, 1.0) * 640
        dir_angle := rand.float32() * math.PI * 2
        paritcle := Particle {
            texture = texture,
            position = wanderer.position,
            velocity = vec2_from_angle(dir_angle, speed),
            rotation = dir_angle,
            scale = vec2(1),
            decay = 0,
            duration = 1,
            life = 1,
            tint = color_from_vec4(lerp(color1, color2, rand.float32())),
        }
        particle_system_spawn(&game_state.particle_system, paritcle)
    }

    entity_system_destroy(&game_state.entity_system, wanderer)
}

@(private = "file")
explose_bullet :: proc(bullet: ^Entity_Bullet) {
    texture := load_texture(Assets_Art_Laser_Png)

    hue1 := rand.float32_range(0.0, 6.0)
    hue2 := math.mod(hue1 + rand.float32_range(0.0, 2.0), 6.0)

    color1 := vec4_hsv(hue1, 0.5, 1.0)
    color2 := vec4_hsv(hue2, 0.5, 1.0)

    for i in 0..<30 {
        speed := rand.float32_range(0.2, 1.0) * 640
        dir_angle := rand.float32() * math.PI * 2
        paritcle := Particle {
            texture = texture,
            position = bullet.position,
            velocity = vec2_from_angle(dir_angle, speed),
            rotation = dir_angle,
            scale = vec2(1),
            decay = 0,
            duration = 1,
            life = 1,
            tint = color_from_vec4(lerp(color1, color2, rand.float32())),
        }
        particle_system_spawn(&game_state.particle_system, paritcle)
    }

    entity_system_destroy(&game_state.entity_system, bullet)
}

get_spawn_position :: proc(player: ^Entity_Player, min, max: f32) -> Vec2 {
    // base := transmute(^Entity_Base)player

    return player.position + vec2_from_angle(rand.float32_range(0, math.PI * 2), rand.float32_range(min, max))
}

@(export)
game_render :: proc() {
    rl.BeginMode2D(game_state.camera)
    {
        alpha := game_state.accumulator / game_state.fixed_timestep
        entity_system_render(&game_state.entity_system, alpha)

        if .Hitbox in game_state.debug_flags {
            for &curr_entity, i in game_state.entity_system.entities {
                prev_entity := &game_state.entity_system.prev_entities[i]

                curr_base := transmute(^Entity_Base)&curr_entity
                prev_base := transmute(^Entity_Base)prev_entity

                position := lerp(prev_base.position, curr_base.position, alpha)
                rl.DrawCircleLinesV(position, curr_base.radius, { 255, 255, 255, 156 })
            }
        }

        particle_system_render(&game_state.particle_system, alpha)
    }
    rl.EndMode2D()

    rl.DrawText(rl.TextFormat("Ticks: %d", i32(game_state.tick_count)), 10, 40, 16, rl.WHITE)
    rl.DrawText(rl.TextFormat("Frames: %d", i32(game_state.frame_count)), 10, 70, 16, rl.WHITE)
    rl.DrawText(rl.TextFormat("Enitities: %d", i32(len(game_state.entity_system.entities))), 10, 100, 16, rl.WHITE)
    rl.DrawText("Press ` to toggle hitbox draw debug", 10, 130, 16, rl.WHITE)
}

@(export)
game_memory :: proc() -> runtime.Raw_Any {
    fmt.printf("Return game_state to main program...\n")

    // fmt.printf("game_state: %p\n", game_state)
    return runtime.Raw_Any { data = game_state, id = type_of(game_state) }
}

@(export)
game_hot_reload :: proc(memory: runtime.Raw_Any) {
    fmt.printf("Reload game_state from main program...\n")
    // fmt.printf("raw_any.type: %v\n", memory.id)
    // fmt.printf("raw_any.data: %v\n", memory.data)

    game_state = transmute(^Game_State)memory.data
}