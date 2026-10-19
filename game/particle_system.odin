package neonshooter_game


import rl "vendor:raylib"
import gxm "../gxm"


Particle :: struct #align(64) {
    position:   gxm.Vec2,       // offset 0
    scale:      gxm.Vec2,       // offset 8
    velocity:   gxm.Vec2,       // offset 16
    rotation:   f32,      		// offset 20
    decay:      f32,      		// offset 32
    duration:   f32,      		// offset 36
    life:       f32,      		// offset 40
    tint:       rl.Color,       // offset 44
    texture:    rl.Texture,     // offset 48
}
#assert(size_of(Particle) == 64, "Size of particle must be 60.")

MAX_NUM_PARTICLES :: 10 * 1024

Particle_System :: struct {
    particles:      #soa[MAX_NUM_PARTICLES]Particle,
    num_particles:  i32,
}

particle_system_init :: proc(particle_system: ^Particle_System) {
}

particle_system_deinit :: proc(particle_system: ^Particle_System) {
    if particle_system == nil {
        return
    }

    particle_system^ = {}
}

particle_system_spawn :: proc(particle_system: ^Particle_System, particle: Particle) {
    particle_system.particles[particle_system.num_particles] = particle
    particle_system.num_particles += 1
}

particle_system_update :: proc(particle_system: ^Particle_System, dt: f32) {
    len := particle_system.num_particles
    for idx := i32(0); idx < len; idx += 1 {
        particle := &particle_system.particles[idx]

        particle.life -= dt
        if particle.life <= 0 {
            particle_system.num_particles -= 1
            particle_system.particles[idx] = particle_system.particles[particle_system.num_particles]

            len -= 1
            idx -= 1

            continue
        }

        gxm.integrate(&particle.position, particle.velocity, dt)
        gxm.decay(&particle.velocity, particle.decay, dt)
        // gxm.smooth_damp(&particle.position, )
        
        particle.scale      = gxm.vec2(particle.life / particle.duration)
        particle.rotation   = gxm.angle(particle.velocity)
        particle.tint.a     = u8((particle.life / particle.duration) * 255)

        gxm.bounce_in_bbox(&particle.position, &particle.velocity, game_state.world_bbox)
    }
}

particle_system_render :: proc(particle_system: ^Particle_System, alpha: f32, blend := true) {
    if blend {
        rl.BeginBlendMode(.ADDITIVE)
    }

    for i in 0..<particle_system.num_particles {
        particle := particle_system.particles[i]

        w   := f32(particle.texture.width)
        h   := f32(particle.texture.height)
        sx  := f32(particle.scale.x)
        sy  := f32(particle.scale.y)

        pos := particle.position
        dst := rl.Rectangle{ pos.x, pos.y, w * sx, h * sy }
        
        rl.DrawTexturePro(
            texture     = particle.texture,
            source      = { 0, 0, w, h },
            dest        = dst,
            origin      = [2]f32{w, h} * 0.5,
            rotation    = gxm.degrees(particle.rotation),
            tint        = particle.tint
        )
    }

    if blend {
        rl.EndBlendMode()
    }
}
