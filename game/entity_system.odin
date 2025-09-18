package neonshooter_game

import "core:fmt"
import "core:mem"
import rl "vendor:raylib"

Entity_System :: struct {
    entities: [dynamic]Entity,
    free_entity: ^Entity,
    allocator: mem.Allocator,
}

Entity_Iterator :: struct {
    entities: []Entity,
    current: int,
    next: proc(self: Entity_Iterator) -> (^Entity, bool)
}

entity_system_init :: proc(entity_system: ^Entity_System, allocator := context.allocator, loc := #caller_location) {
    assert(entity_system != nil)

    entity_system.entities = make(type_of(entity_system.entities), allocator = allocator, loc = loc)
    entity_system.free_entity = nil
    entity_system.allocator = allocator
}

entity_system_deinit :: proc(entity_system: ^Entity_System) {
    if entity_system != nil {
        delete(entity_system.entities)
        entity_system^ = {}
    }
}

entity_system_iter_by_type :: proc(entity_system: ^Entity_System, $T: typeid) -> Entity_Iterator {
    return Entity_Iterator {
        entities = entity_system.entities[:],
        current = 0,
        next = proc(self: Entity_Iterator) -> (^Entity, bool) {
            if self.entities != nil && self.current < len(self.entities) {
                self.current += 1
                return self.entities[self.current], true
            }

            return nil, false
        }
    }
}

entity_system_add :: proc(entity_system: ^Entity_System, entity: Entity) -> ^Entity {
    new_len, _ := append(&entity_system.entities, entity)
    return &entity_system.entities[new_len - 1]
}

entity_system_update :: proc(entity_system: ^Entity_System, dt: f32) {
    // Simulation

    // Collisions
    
}

entity_system_render :: proc(entity_system: ^Entity_System) {
    for i in 0..<len(entity_system.entities) {
        entity_base := cast(^Entity_Base)&entity_system.entities[i]
        if entity_base.hp <= 0 {
            continue
        }

        rl.DrawTextureEx(
            texture = entity_base.texture, 
            position = entity_base.position, 
            rotation = degrees(entity_base.rotation), 
            scale = lerp(entity_base.scale.x, entity_base.scale.y, 0.5), 
            tint = rl.WHITE
        )
    }
}