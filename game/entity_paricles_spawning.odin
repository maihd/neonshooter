package neonshooter_game

import "core:fmt"
import "core:math"
import "core:math/rand"

import gxm "../gxm"
import linalg "core:math/linalg"

explode_player :: proc() {
    fmt.printf("Explode player...\n")
}

explose_seeker :: proc(seeker: ^Entity) {
    texture := load_texture(&game_state.assets, Assets_Art_Laser_Png)

    hue1 := rand.float32_range(0.0, 6.0)
    hue2 := math.mod(hue1 + rand.float32_range(0.0, 2.0), 6.0)

    color1 := vec4_hsv(hue1, 0.5, 1.0)
    color2 := vec4_hsv(hue2, 0.5, 1.0)

    for i in 0..<120 {
        speed 		:= rand.float32_range(0.2, 1.0) * 640
        dir_angle 	:= gxm.turn_to_rad(rand.float32()) // [0..1]
        paritcle := Particle {
            texture 	= texture,
            position 	= seeker.position,
            velocity 	= gxm.vec2_from_rad(dir_angle, speed),
            rotation 	= dir_angle,
            scale 		= gxm.vec2(1),
            decay 		= 0,
            duration 	= 1,
            life 		= 1,
            tint 		= color_from_vec4(gxm.lerp(color1, color2, rand.float32())),
        }
        particle_system_spawn(&game_state.particle_system, paritcle)
    }

    entity_system_destroy(&game_state.entity_system, seeker.id)
}

explose_wanderer :: proc(wanderer: ^Entity) {
    texture := load_texture(&game_state.assets, Assets_Art_Laser_Png)

    hue1 := rand.float32_range(0.0, 6.0)
    hue2 := math.mod(hue1 + rand.float32_range(0.0, 2.0), 6.0)

    color1 := vec4_hsv(hue1, 0.5, 1.0)
    color2 := vec4_hsv(hue2, 0.5, 1.0)

    for i in 0..<120 {
        speed 		:= rand.float32_range(0.2, 1.0) * 640
        dir_angle 	:= gxm.turn_to_rad(rand.float32()) // turns [0..1]
        paritcle := Particle {
            texture 	= texture,
            position 	= wanderer.position,
            velocity 	= gxm.vec2_from_rad(dir_angle, speed),
            rotation 	= dir_angle,
            scale 		= gxm.vec2(1),
            decay 		= 0,
            duration 	= 1,
            life 		= 1,
            tint 		= color_from_vec4(gxm.lerp(color1, color2, rand.float32())),
        }
        particle_system_spawn(&game_state.particle_system, paritcle)
    }

    entity_system_destroy(&game_state.entity_system, wanderer.id)
}

explose_bullet :: proc(bullet: ^Entity) {
    texture := load_texture(&game_state.assets, Assets_Art_Laser_Png)

    hue1 := rand.float32_range(0.0, 6.0)
    hue2 := math.mod(hue1 + rand.float32_range(0.0, 2.0), 6.0)

    color1 := vec4_hsv(hue1, 0.5, 1.0)
    color2 := vec4_hsv(hue2, 0.5, 1.0)

    for i in 0..<30 {
        speed 		:= rand.float32_range(0.2, 1.0) * 640
        dir_angle 	:= rand.float32() // turns [0..1]
        paritcle := Particle {
            texture 	= texture,
            position 	= bullet.position,
            velocity 	= gxm.vec2_from_turn(dir_angle, speed),
            rotation 	= dir_angle,
            scale 		= gxm.vec2(1),
            decay 		= 0,
            duration 	= 1,
            life 		= 1,
            tint 		= color_from_vec4({ 0.6, 1.0, 1.0, 1.0 }),
        }
        particle_system_spawn(&game_state.particle_system, paritcle)
    }

    entity_system_destroy(&game_state.entity_system, bullet.id)
}
