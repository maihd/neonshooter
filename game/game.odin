package neonshooter_game

import "core:math"
import "core:math/rand"
import "core:fmt"

import rl "vendor:raylib"
import gxm "../gxm"

GAME_DEBUGGING :: #config(GAME_DEBUGGING, true)

Game_Input :: struct {
    fire_button_down: 	bool,
    move_direction: 	gxm.Vec2,
}

Game_State :: struct {
    entity_system: Entity_System,

    player_handle: Entity_Handle,

    player_life:    i32,

    fire_timer:     f32,
    fire_rate:      f32,

    spawn_seeker_rate:      i32,
    spawn_seeker_timer:     f32,
    spawn_seeker_interval:  f32,

    spawn_wanderer_rate:        i32,
    spawn_wanderer_timer:       f32,
    spawn_wanderer_interval:    f32,

    // Rendering
    camera:             rl.Camera2D,
    render_target:  	rl.RenderTexture2D,
    particle_system:    Particle_System,
    post_processing:    Post_Processing,
    viewport_width: 	f32,
    viewport_height: 	f32,

    // Input
    tick_count:         i64,
    tick_input:         Game_Input,

    // Assets
    assets:         	Assets,

    // Debug
    debug_flags:    	bit_set[Debug_Flags],

    // World
    world_bbox:			gxm.BBox2,
    warp_grid:			Warp_Grid,
    

    // Reversed here
    // But game state does not contained in any container, so we do not need specified the fields
    // Just allocated big memory of block, bigger than this struct
}


Debug_Flags :: enum {
    Hitbox,
}


Game_Tick_Kind  :: enum(u32) { Spawning, Movement, Particle }
Game_Tick_Flags :: bit_set[Game_Tick_Kind; u32]


game_state: ^Game_State


@(export)
game_memory_size :: proc() -> int {
    memory_size := 1024 * 1024
    for memory_size <= size_of(Game_State) {
        memory_size *= 2 // Choose your ratio here, unused memory are reversed for hot reload
    }

    return memory_size
}


@(export)
game_init :: proc(state: ^Game_State) {
    fmt.printfln("Game init")

    game_state = state

    screen_width 	:= f32(rl.GetScreenWidth())
    screen_height 	:= f32(rl.GetScreenHeight())

    game_state.viewport_width   = f32(800)
    game_state.viewport_height  = 800 * (screen_height / screen_width)
    game_state.render_target    = rl.LoadRenderTexture(
    	i32(game_state.viewport_width), 
     	i32(game_state.viewport_height),
    )
    rl.SetTextureFilter(game_state.render_target.texture, .TRILINEAR)
    game_state.world_bbox = {
    	min = { game_state.viewport_width, game_state.viewport_height } * -0.5,
     	max = { game_state.viewport_width, game_state.viewport_height } * +0.5,
    }

    game_state.camera = rl.Camera2D {
        target      = {0, 0},
        offset      = {
        	game_state.viewport_width * 0.5, 
         	game_state.viewport_height * 0.5,
        },
        rotation    = 0,
        zoom        = 1.0
    }

    game_state.player_life = 3

    game_state.fire_rate 	= 0.1
    game_state.fire_timer 	= 0.0

    game_state.spawn_seeker_rate 		= 20
    game_state.spawn_seeker_timer 		= 0.0
    game_state.spawn_seeker_interval 	= 1.0

    game_state.spawn_wanderer_rate 		= 30
    game_state.spawn_wanderer_timer 	= 0.0
    game_state.spawn_wanderer_interval 	= 1.0

    entity_system_init(&game_state.entity_system)
    particle_system_init(&game_state.particle_system)
    post_processing_init(&game_state.post_processing)
    warp_grid_init(&game_state.warp_grid, game_state.world_bbox, gxm.vec2(25, 25))

    // Preload
    load_all_textures(&game_state.assets)

    if game_state.player_handle == {} {
        player_texture := load_texture(&game_state.assets, Assets_Art_Player_Png)
        player := Entity {
            kind        = .Entity_Player,

            layer       = .Player,
            layer_mask  = { .Enemy },
            radius      = f32(player_texture.width),

            texture     = player_texture,
            position    = gxm.vec2(0, 0),
            rotation    = 0,
            scale       = gxm.vec2(1, 1),

            hp          = 10,
            attack      = 1,
            defense     = 0,
        }
        entity_handle := entity_system_add(&game_state.entity_system, player)
        game_state.player_handle = entity_handle
    }
}


@(export)
game_deinit :: proc(state: ^Game_State) {
    fmt.printfln("Game deinit")

    game_state = state

    if game_state != nil {
    	warp_grid_deinit(&game_state.warp_grid)
        post_processing_deinit(&game_state.post_processing)
        particle_system_deinit(&game_state.particle_system)
        entity_system_deinit(&game_state.entity_system)
    }

    unload_all_textures(&game_state.assets)

    fmt.printfln("Game deinit finished")
}

@(export)
game_process_input :: proc(state: ^Game_State, dt: f32) {
    game_state = state

    // Hot keys
    if rl.IsKeyPressed(.GRAVE) {
        game_state.debug_flags ~= { .Hitbox }
    }

    // Update input

    dir: gxm.Vec2
    if rl.IsKeyDown(.W) || rl.IsKeyDown(.UP) {
        dir.y -= 1
    }

    if rl.IsKeyDown(.S) || rl.IsKeyDown(.DOWN) {
    	dir.y += 1
    }

    if rl.IsKeyDown(.A) || rl.IsKeyDown(.LEFT) {
    	dir.x -= 1
    }

    if rl.IsKeyDown(.D) || rl.IsKeyDown(.RIGHT) {
    	dir.x += 1
    }

    game_state.tick_input.move_direction 	= dir
    game_state.tick_input.fire_button_down 	= rl.IsMouseButtonDown(.LEFT)
}

@(export)
game_tick :: proc(state: ^Game_State, dt: f32, tick_kind: Game_Tick_Flags) {
    game_state = state

    game_state.tick_count += 1

    speed := f32(300)

    warp_grid_update(&game_state.warp_grid, dt)
    entity_system_update(&game_state.entity_system, dt)
    particle_system_update(&game_state.particle_system, dt)
    post_processing_update(&game_state.post_processing, dt)

    dir := game_state.tick_input.move_direction
    if gxm.lensq(dir) > 0 {
        dir = gxm.norm(dir)
    }

    player := entity_system_get(&game_state.entity_system, game_state.player_handle)
    player.velocity = dir * speed
    action_move(player, dt)

    player.position = gxm.clamp_in_bbox(player.position, player.radius, game_state.world_bbox)

    if !(.Spawning in tick_kind) {
        return
    }

    if  game_state.tick_input.fire_button_down {
        game_state.fire_timer += dt

        if  game_state.fire_timer >= game_state.fire_rate {
            game_state.fire_timer -= game_state.fire_rate

            mouse_screen_pos := rl.GetMousePosition()
            mouse_screen_pos.x = (mouse_screen_pos.x / f32(rl.GetScreenWidth())) * game_state.viewport_width
            mouse_screen_pos.y = (mouse_screen_pos.y / f32(rl.GetScreenHeight())) * game_state.viewport_height

            mouse_world_pos := rl.GetScreenToWorld2D(mouse_screen_pos, game_state.camera)

            bullet_dir := gxm.dir(player.position, mouse_world_pos)
            bullet_vel := bullet_dir * 1000

            bullet_dir_angle := gxm.angle(bullet_dir)
            bullet_ang1 := bullet_dir_angle - 0.15
            bullet_pos1 := gxm.spread_rad(player.position, bullet_ang1, player.radius + 10)

            bullet_ang2 := bullet_dir_angle + 0.15
            bullet_pos2 := gxm.spread_rad(player.position, bullet_ang2, player.radius + 10)

            texture := load_texture(&game_state.assets, Assets_Art_Bullet_Png)

            bullet1 := Entity {
                kind        = .Entity_Bullet,

                layer       = .Player,
                layer_mask  = { .Enemy },
                radius      = f32(texture.width) * 0.2,

                position    = bullet_pos1,
                texture     = texture,
                rotation    = bullet_dir_angle,
                scale       = gxm.vec2(1),

                hp          = 1,
                attack      = player.attack,
                velocity    = bullet_vel,
            }
            entity_system_add(&game_state.entity_system, bullet1)

            bullet2 := Entity {
                kind        = .Entity_Bullet,

                layer       = .Player,
                layer_mask  = { .Enemy },
                radius      = f32(texture.width) * 0.2,

                position    = bullet_pos2,
                texture     = load_texture(&game_state.assets, Assets_Art_Bullet_Png),
                rotation    = bullet_dir_angle,
                scale       = gxm.vec2(1),

                hp          = 1,
                attack      = player.attack,
                velocity    = bullet_vel,
            }
            entity_system_add(&game_state.entity_system, bullet2)
        }
    }

    game_state.spawn_seeker_timer += dt
    if  game_state.spawn_seeker_timer >= game_state.spawn_seeker_interval {
        game_state.spawn_seeker_timer -= game_state.spawn_seeker_interval

        if rand.float32_range(0, 100) <= f32(game_state.spawn_seeker_rate) {
            texture := load_texture(&game_state.assets, Assets_Art_Seeker_Png)
            seeker := Entity {
                kind        = .Entity_Seeker,

                hp          = 10,
                attack      = 10,
                defense     = 1,

                layer       = .Enemy,
                layer_mask  = { .Player },
                radius      = f32(texture.width) * 0.65,

                position    = get_spawn_position(player, 100, 300),
                rotation    = 0,
                scale       = gxm.vec2(1),

                texture     = texture,
                tint        = rl.WHITE,

                velocity    = gxm.vec2(0.0),
                follow_target = game_state.player_handle,

                spawn_timer = 1.0,
            }
            entity_system_add(&game_state.entity_system, seeker)
        }
    }

    game_state.spawn_wanderer_timer += dt
    if  game_state.spawn_wanderer_timer >= game_state.spawn_wanderer_interval {
    	game_state.spawn_wanderer_timer -= game_state.spawn_wanderer_interval

        if rand.float32_range(0, 100) <= f32(game_state.spawn_wanderer_rate) {
            texture := load_texture(&game_state.assets, Assets_Art_Wanderer_Png)
            wanderer := Entity {
                kind        = .Entity_Wanderer,

                hp          = 10,
                attack      = 10,
                defense     = 1,

                layer       = .Enemy,
                layer_mask  = { .Player },
                radius      = f32(texture.width) * 0.65,

                position    = get_spawn_position(player, 100, 300),
                rotation    = 0,
                scale       = gxm.vec2(1),

                texture     = texture,
                tint        = rl.WHITE,

                velocity    = gxm.vec2(0.0),

                spawn_timer = 1.0,
            }
            entity_system_add(&game_state.entity_system, wanderer)
        }
    }
}

get_spawn_position :: proc(player: ^Entity, min, max: f32) -> gxm.Vec2 {
	dir 	:= rand.float32_range(0, math.PI * 2)
	range 	:= rand.float32_range(min, max)

	// @note(maihd)
	// 	- this formular is like gxm.spread
	//  - but i think the purpose is diffs
	return player.position + gxm.vec2_from_rad(dir, range)
}

@(export)
game_render :: proc(state: ^Game_State) {
    game_state = state

    rl.BeginTextureMode(game_state.render_target)
    rl.ClearBackground(rl.BLACK)
    rl.BeginMode2D(game_state.camera)
    {
        // alpha := game_state.accumulator / game_state.fixed_timestep
        alpha := f32(0.0)

        warp_grid_render(&game_state.warp_grid, game_state.world_bbox, alpha)
        
        entity_system_render(&game_state.entity_system, alpha)

        if .Hitbox in game_state.debug_flags {
            for i in 0..<game_state.entity_system.entities.num_items {
                curr_entity := game_state.entity_system.entities.items[i]
                position 	:= curr_entity.position
                radius 	 	:= curr_entity.radius
                
                rl.DrawCircleLinesV(position, radius, { 255, 255, 255, 156 })
            }
        }

        particle_system_render(&game_state.particle_system, alpha)
    }
    rl.EndMode2D()
    rl.EndTextureMode()
    post_processing_draw(&game_state.post_processing, game_state.render_target)

    rl.DrawText(rl.TextFormat("Enitities: %d", i32(game_state.entity_system.entities.num_items)), 10, 100, 16, rl.WHITE)
    rl.DrawText("Press ` to toggle hitbox draw debug", 10, 130, 16, rl.WHITE)
}

// Handle hot reloading
when GAME_DEBUGGING {
    @(export)
    game_memory :: proc() -> rawptr {
        fmt.printf("Return game_state to main program...\n")
        fmt.printf("Game state: %p\n", game_state)

        return game_state
    }

    @(export)
    game_hot_reload :: proc(memory: rawptr) {
        fmt.printf("Reload game_state from main program...\n")
        fmt.printf("Game state: %p\n", memory)

        game_state = cast(^Game_State)memory
    }
}
