package neonshooter_game

import rl "vendor:raylib"

Entity_Base :: struct {
    // For rendering

    position: Vec2,
    rotation: f32,
    scale: Vec2,
    radius: f32,
    texture: rl.Texture,
    tint: rl.Color,

    // Timer

    spawn_timer: f32,

    // Stats
    
    hp: int,
    attack: int,
    defense: int,
}

// @entity
Entity_Player :: struct {
    using base: Entity_Base,


}

// @entity
Entity_Bullet :: struct {
    using base: Entity_Base,

    velocity: Vec2,
}

// @entity
Entity_Wanderer :: struct {
    using base: Entity_Base,
    
    velocity: Vec2,
    next_direction_timer: f32,
    next_direction_interval: f32,
}

// @entity
Entity_Seeker :: struct {
    using base: Entity_Base,

    velocity: Vec2,
    target: Entity_Handle,
}

// @any_entity
Entity :: union {
    Entity_Base,
    Entity_Player,
    Entity_Bullet,
    Entity_Seeker,
    Entity_Wanderer,
}