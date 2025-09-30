package neonshooter_game

import "base:intrinsics"
import "core:fmt"
import "core:mem"
import rl "vendor:raylib"

Entity_System :: struct {
    entities: [dynamic]Entity,
    free_entity: ^Entity,
    allocator: mem.Allocator,
    entities_by_type: map[typeid][dynamic]Entity_Handle
}

Entity_Iterator :: struct($T: typeid) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    entities: []Entity,
    current: int,
    next: proc(self: ^Entity_Iterator(T)) -> (^T, bool)
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

entity_system_iter_by_type :: proc(entity_system: ^Entity_System, $T: typeid) -> Entity_Iterator(T) {
    return Entity_Iterator(T) {
        entities = entity_system.entities[:],
        current = 0,
        next = proc(self: ^Entity_Iterator(T)) -> (^T, bool) {
            if self.entities != nil && self.current < len(self.entities) {
                for self.current < len(self.entities) {
                    defer self.current += 1

                    entity, ok := &self.entities[self.current].(T)
                    if ok {
                        return entity, true
                    }
                }
            }

            return nil, false
        }
    }
}

entity_system_add :: proc(entity_system: ^Entity_System, entity: Entity) -> Entity_Handle {
    entity_ptr: ^Entity
    if entity_system.free_entity != nil {
        entity_ptr = entity_system.free_entity

        next_entity := (cast(^^Entity)entity_system.free_entity)^
        entity_system.free_entity = next_entity

        entity_base := transmute(^Entity_Base)entity_ptr
        generation := entity_base.generation
        entity_ptr^ = entity
        entity_base.generation = generation
    } else {
        append(&entity_system.entities, entity)
        entity_ptr = &entity_system.entities[len(entity_system.entities) - 1]
    }
    
    index := mem.ptr_sub(entity_ptr, transmute(^Entity)raw_data(entity_system.entities))
    entity_base := transmute(^Entity_Base)entity_ptr
    entity_base.generation += 1

    return {
        index = u32(index),
        generation = u32(entity_base.generation)
    }
}

entity_system_destroy :: proc(entity_system: ^Entity_System, entity: ^Entity) {
    index := mem.ptr_sub(entity, transmute(^Entity)raw_data(entity_system.entities))
    if index < 0 || index >= len(entity_system.entities) {
        return
    }

    entity^ = nil

    next_entity := cast(^^Entity)entity
    next_entity^ = entity_system.free_entity

    entity_system.free_entity = entity
}

entity_system_get :: proc(entity_system: ^Entity_System, handle: Entity_Handle, $T: typeid) -> ^T
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    if int(handle.index) >= len(entity_system.entities) {
        return nil
    }

    entity := &entity_system.entities[handle.index]
    entity_base := transmute(^Entity_Base)entity

    return entity_base.generation == int(handle.generation) ? transmute(^T)entity_base : nil
}

entity_system_update :: proc(entity_system: ^Entity_System, dt: f32) {
    // Simulation

    // Collisions
    
}

entity_system_render :: proc(entity_system: ^Entity_System) {
    for i in 0..<len(entity_system.entities) {
        if entity_system.entities[i] == nil {
            continue
        }

        entity_base := cast(^Entity_Base)&entity_system.entities[i]
        if entity_base.hp <= 0 {
            continue
        }

        w := f32(entity_base.texture.width)
        h := f32(entity_base.texture.height)
        sx := f32(entity_base.scale.x)
        sy := f32(entity_base.scale.y)
        
        rl.DrawTexturePro(
            texture = entity_base.texture,
            source = { 0, 0, w, h },
            dest = { entity_base.position.x, entity_base.position.y, w * sx, h * sy },
            origin = vec2(w, h) * 0.5,
            rotation = degrees(entity_base.rotation),
            tint = rl.WHITE
        )
    }
}