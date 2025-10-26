package neonshooter_game

import "core:math"
import "core:mem"
import "core:fmt"
import rl "vendor:raylib"

Particle :: struct {
    position: Vec2,
    rotation: f32,
    scale: Vec2,
    tint: rl.Color,
    velocity: Vec2,
    decay: f32,
    duration: f32,
    life: f32,
    texture: rl.Texture,
}

Particle_System :: struct {
    particles: [dynamic]Particle,
    prev_particles: [dynamic]Particle,
    removing_particles: [dynamic]int,
}

particle_system_init :: proc(particle_system: ^Particle_System, allocator := context.allocator) {
    particle_system.particles.allocator = allocator
    particle_system.prev_particles.allocator = allocator 
    particle_system.removing_particles.allocator = allocator
}

particle_system_deinit :: proc(particle_system: ^Particle_System) {
    if particle_system == nil {
        return
    }

    delete(particle_system.particles)
    delete(particle_system.prev_particles)
    delete(particle_system.removing_particles)

    particle_system^ = {}
}

particle_system_spawn :: proc(particle_system: ^Particle_System, particle: Particle) {
    append(&particle_system.particles, particle)
    append(&particle_system.prev_particles, particle)
}

particle_system_update :: proc(particle_system: ^Particle_System, dt: f32) {
    mem.copy(raw_data(particle_system.prev_particles), raw_data(particle_system.particles), len(particle_system.particles) * size_of(Particle))
    
    for &particle, i in particle_system.particles {
        particle.life -= dt
        if particle.life <= 0 {
            append(&particle_system.removing_particles, i)
            continue
        }

        particle.position += particle.velocity * dt
        particle.velocity -= particle.velocity * particle.decay * dt
        particle.scale = particle.life / particle.duration
        particle.rotation = angle(particle.velocity)
        particle.tint.a = u8((particle.life / particle.duration) * 255)


        if particle.position.x < 0 {
            particle.position.x = 0
            particle.velocity.x = abs(particle.velocity.x)
        }

        if particle.position.x > f32(rl.GetScreenWidth()) {
            particle.position.x = f32(rl.GetScreenWidth())
            particle.velocity.x = -abs(particle.velocity.x)
        }

        if particle.position.y < 0 { 
            particle.position.y = 0
            particle.velocity.y = abs(particle.velocity.y)
        } 
        
        if particle.position.y > f32(rl.GetScreenHeight()) {
            particle.position.y = f32(rl.GetScreenHeight())
            particle.velocity.y = -abs(particle.velocity.y)
        }
    }

    #reverse for i in particle_system.removing_particles {
        unordered_remove(&particle_system.particles, i)
        unordered_remove(&particle_system.prev_particles, i)
    }
    clear(&particle_system.removing_particles)
}

particle_system_render :: proc(particle_system: ^Particle_System, alpha: f32, blend := true) {
    if blend {
        rl.BeginBlendMode(.ADDITIVE)
    }

    for curr_particle, i in particle_system.particles {
        prev_particle := particle_system.prev_particles[i]
        
        particle := curr_particle
        particle.position = lerp(prev_particle.position, curr_particle.position, alpha)
        particle.rotation = lerp(prev_particle.rotation, curr_particle.rotation, alpha)
        particle.scale = lerp(prev_particle.scale, curr_particle.scale, alpha)
        particle.tint = rl.ColorLerp(prev_particle.tint, curr_particle.tint, alpha)

        w := f32(particle.texture.width)
        h := f32(particle.texture.height)
        sx := f32(particle.scale.x)
        sy := f32(particle.scale.y)
        
        rl.DrawTexturePro(
            texture = particle.texture,
            source = { 0, 0, w, h },
            dest = { particle.position.x, particle.position.y, w * sx, h * sy },
            origin = vec2(w, h) * 0.5,
            rotation = degrees(particle.rotation),
            tint = particle.tint
        )
    }

    if blend {
        rl.EndBlendMode()
    }
}