package neonshooter_game

import rl "vendor:raylib"

Entity_Base :: struct {
    generation: int,

    position: Vec2,
    rotation: f32,
    scale: Vec2,
    radius: f32,
    texture: rl.Texture,
    
    hp: int,
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

// @any_entity
Entity :: union {
    Entity_Base,
    Entity_Player,
    Entity_Bullet,
}

Entity_Handle :: bit_field u32 {
    index: u32 | 24,
    generation: u32 | 8,
}
#assert(size_of(Entity_Handle) == size_of(u32))