package neonshooter_game

import "base:intrinsics"
import "core:fmt"
import "core:mem"
import rl "vendor:raylib"

Entity_System :: struct {
    entities: [dynamic]Entity,
    handles: [dynamic]Entity_Handle,
    sparse_indices: [dynamic]Sparse_Index, // Persistant size, never remove items
    next_index: u32,    // This index point to elements of sparse_indices

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

Sparse_Index :: struct {
    generation: u32,
    index_or_next: u32,     // This index point to elements of entities/handles
}

Entity_Handle :: bit_field u32 {
    index: u32 | 24,        // This index point to elements of sparse_indices
    generation: u32 | 8,
}

entity_system_init :: proc(entity_system: ^Entity_System, allocator := context.allocator) {
    assert(entity_system != nil)

    entity_system.entities.allocator = allocator
    entity_system.handles.allocator = allocator
    entity_system.sparse_indices.allocator = allocator

    entity_system.next_index = 0
    entity_system.allocator = allocator
}

entity_system_deinit :: proc(entity_system: ^Entity_System) {
    if entity_system != nil {
        delete(entity_system.entities_by_type)
        delete(entity_system.sparse_indices)
        delete(entity_system.handles)
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

entity_system_add :: proc(entity_system: ^Entity_System, entity: Entity) -> (handle: Entity_Handle) {
    if entity_system.next_index < u32(len(entity_system.sparse_indices)) {
        entry := entity_system.sparse_indices[entity_system.next_index]
        _ = entry
    } else {
        index := len(entity_system.handles)
        handle = Entity_Handle {
            index = entity_system.next_index,
            generation = 0
        }
        append(&entity_system.handles, handle)
        append(&entity_system.entities, entity)

        entry := Sparse_Index {
            index_or_next = u32(index), 
            generation = 0,
        }
        append(&entity_system.sparse_indices, entry)
        entity_system.next_index += 1
    }

    return
}

entity_system_destroy :: proc(entity_system: ^Entity_System, entity: ^Entity) {
    index := mem.ptr_sub(entity, transmute(^Entity)raw_data(entity_system.entities))
    if index < 0 || index >= len(entity_system.entities) {
        return
    }

    handle := entity_system.handles[index]
    entity_system_destroy_w_handle(entity_system, handle)
}

entity_system_destroy_w_handle :: proc(using entity_system: ^Entity_System, handle: Entity_Handle) {
    if handle.index >= u32(len(sparse_indices)) {
        return
    }

    entry := &sparse_indices[handle.index]
    index := entry.index_or_next

    entry.index_or_next = next_index
    next_index = handle.index

    unordered_remove(&handles, index)
    unordered_remove(&entities, index)
}

entity_system_get :: proc(entity_system: ^Entity_System, handle: Entity_Handle, $T: typeid) -> ^T
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    if int(handle.index) >= len(entity_system.sparse_indices) {
        return nil
    }

    entry := entity_system.sparse_indices[handle.index]
    entity := &entity_system.entities[entry.index_or_next]
    entity_base := transmute(^Entity_Base)entity

    return entity_base.generation == int(handle.generation) ? transmute(^T)entity_base : nil
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