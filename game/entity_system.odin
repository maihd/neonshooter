package neonshooter_game

import "core:mem"

Entity_System :: struct {
    entities: [dynamic]Entity,
    free_entity: ^Entity,
    allocator: mem.Allocator,
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

entity_system_iter_by_type :: proc(entity_system: ^Entity_System, $T: typeid) -> proc() -> (^Entity, bool) {
    return proc() -> (^Entity, bool) {
        return nil, false
    }
}

entity_system_update :: proc(entity_system: ^Entity_System, dt: f32) {
    // Simulation

    // Collisions
    
}

entity_system_render :: proc(entity_system: ^Entity_System) {

}