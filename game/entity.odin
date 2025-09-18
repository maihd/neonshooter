package neonshooter_game

import rl "vendor:raylib"

Entity_Base :: struct {
    position: Vec2,
    rotation: f32,
    scale: Vec2,
    radius: f32,
    texture: rl.Texture,
}

// @entity
Entity_Player :: struct {
    using base: Entity_Base,


}

Entity :: struct {
    Entity_Player,
}