package neonshooter_game

import "core:fmt"
import "core:math"
import "core:math/rand"

import rl "vendor:raylib"
import gxm "../gxm"

// Action as utils

// In more complex and huge world game, we putting state
// And doing events instead of calling procs directly

// @region shared actions

action_move :: proc(entity: ^Entity, dt: f32) {
    entity.rotation = gxm.angle(entity.velocity)
    gxm.integrate(&entity.position, entity.velocity, dt)
}

action_follow_target :: proc(entity: ^Entity, dt: f32) {
    target := entity_system_get(&game_state.entity_system, entity.follow_target)
    if target != nil {
        entity.velocity = gxm.vel(entity.position, target.position, 100) // @fixme(maihd): hard code here for test
        action_move(entity, dt)
    }
}

action_move_wandering :: proc(entity: ^Entity, dt: f32) {
    dir 	:= rand.float32_range(-0.3, 0.3)
    dir_v2 	:= gxm.vec2_from_turn(dir, 30)
    
    entity.velocity = gxm.lerp(entity.velocity, dir_v2, 3 * dt)
    action_move(entity, dt)
}

// @region specific actions for entity per kind

action_move_bullet :: proc(bullet: ^Entity, dt: f32) -> (_exploded: bool) {
    action_move(bullet, dt)

    out_of_world := gxm.out_of_bbox(bullet.position, game_state.world_bbox)
    if out_of_world {
        explose_bullet(bullet)
        _exploded = true
        return
    }

    warp_grid_explosive_force(&game_state.warp_grid, 0.25 * gxm.len(bullet.velocity), gxm.vec3(bullet.position), 60)

    return
}

action_move_seeker :: proc(seeker: ^Entity, dt: f32) {
    action_follow_target(seeker, dt)
}

action_move_wanderer :: proc(wanderer: ^Entity, dt: f32) {
    action_move_wandering(wanderer, dt)
}
