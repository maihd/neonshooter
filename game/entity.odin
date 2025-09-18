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

// @any_entity
Entity :: union {
    Entity_Base,
    Entity_Player,
}