package neonshooter_game

import "core:fmt"
import rl "vendor:raylib"
import "core:math"
import "core:math/rand"

@entity_proc
_process_bullet_movement :: proc(bullet: ^Entity_Bullet, dt: f32) -> bool {
    bullet.position += bullet.velocity * dt
    bullet.rotation = angle(bullet.velocity)

    assert(!math.is_nan(bullet.position.x))
    assert(!math.is_nan(bullet.position.y))
    assert(!math.is_inf(bullet.position.x))
    assert(!math.is_inf(bullet.position.y))

    if bullet.position.x < 0 \
        || bullet.position.y < 0 \
        || bullet.position.x > f32(rl.GetScreenWidth()) \
        || bullet.position.y > f32(rl.GetScreenHeight()) 
    {
        explose_bullet(bullet)
    }

    return false
}

@entity_proc
_process_seeker_movement :: proc(seeker: ^Entity_Seeker, dt: f32) -> bool {
    target := entity_system_get(&game_state.entity_system, seeker.target, Entity_Player)
    if target != nil {
        seeker.velocity = norm(target.position - seeker.position + vec2(math.F32_EPSILON)) * 100 // @fixme for test
        seeker.rotation = angle(seeker.velocity)
        seeker.position = seeker.position + seeker.velocity * dt
    }

    return false
}

@entity_proc
_process_wanderer_movement :: proc(wanderer: ^Entity_Wanderer, dt: f32) -> bool {
    dir := rand.float32_range(-0.3, 0.3)
    wanderer.velocity = lerp(wanderer.velocity, vec2_from_angle(dir) * 30, 3 * dt)
    wanderer.rotation = angle(wanderer.velocity)
    wanderer.position = wanderer.position + wanderer.velocity * dt

    return false
}

@entity_proc
_process_bullet_seeker_collisions :: proc(bullet: ^Entity_Bullet, seeker: ^Entity_Seeker, dt: f32) -> bool {
    dist := distsqr(bullet.position, seeker.position)
    if dist <= (bullet.radius + seeker.radius) * (bullet.radius + seeker.radius) {
        seeker.hp -= bullet.attack
        if seeker.hp <= 0 {
            explose_seeker(seeker)
        }

        entity_system_destroy(&game_state.entity_system, bullet)
        return true
    }

    return false
}

@entity_proc
_process_bullet_wanderer_collisions :: proc(bullet: ^Entity_Bullet, wanderer: ^Entity_Wanderer, dt: f32) -> bool {
    dist := distsqr(bullet.position, wanderer.position)
    if dist <= (bullet.radius + wanderer.radius) * (bullet.radius + wanderer.radius) {
        wanderer.hp -= bullet.attack
        if wanderer.hp <= 0 {
            explose_wanderer(wanderer)
        }

        entity_system_destroy(&game_state.entity_system, bullet)
        return true
    }

    return false
}

@entity_proc
_process_player_seeker_collisions :: proc(player: ^Entity_Player, seeker: ^Entity_Seeker, dt: f32) -> bool {
    dist := distsqr(player.position, seeker.position)
    collided := dist <= (player.radius + seeker.radius) * (player.radius + seeker.radius)
    if collided {
        player.hp -= math.min(1, seeker.attack - player.defense)
        explose_seeker(seeker)
        
        if player.hp <= 0 {
            explode_player()
            return true
        }
    }

    return false
}

@entity_proc
_process_player_wanderer_collisions :: proc(player: ^Entity_Player, wanderer: ^Entity_Wanderer, dt: f32) -> bool {
    dist := distsqr(player.position, wanderer.position)
    collided := dist <= (player.radius + wanderer.radius) * (player.radius + wanderer.radius)
    if collided {
        player.hp -= math.min(1, wanderer.attack - player.defense)
        explose_wanderer(wanderer)

        if player.hp <= 0 {
            explode_player()
            return true
        }
    }

    return false
}