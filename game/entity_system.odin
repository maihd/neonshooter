package neonshooter_game

import "base:runtime"
import "core:math/bits"
import "core:math"
import "core:reflect"
import "base:intrinsics"
import "core:fmt"
import "core:mem"
import rl "vendor:raylib"

Entity_System :: struct {
    entities: [dynamic]Entity,
    handles: [dynamic]Entity_Handle,
    sparse_indices: [dynamic]Sparse_Index, // Persistant size, never remove items
    next_index: u32,    // This index point to elements of sparse_indices
    destroying_handles: [dynamic]Entity_Handle,

    allocator: mem.Allocator,
    entities_by_type: map[typeid]u32
}

Entity_Iterator :: struct($T: typeid) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    entity_system: ^Entity_System,
    entities: u32,
    next: proc(self: ^Entity_Iterator(T)) -> (^T, bool)
}

Sparse_Index :: struct {
    index_or_next: u32,     // This index point to elements of entities/handles, next point to sparse_indices
    generation: u32,
    prev_by_type: u32,
    next_by_type: u32,
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
    entity_system.destroying_handles.allocator = allocator
    entity_system.entities_by_type.allocator = allocator

    entity_system.next_index = 0
    entity_system.allocator = allocator
}

entity_system_deinit :: proc(entity_system: ^Entity_System) {
    if entity_system != nil {
        delete(entity_system.entities_by_type)
        delete(entity_system.destroying_handles)
        delete(entity_system.sparse_indices)
        delete(entity_system.handles)
        delete(entity_system.entities)
        entity_system^ = {}
    }
}

entity_system_iter_by_type :: proc(entity_system: ^Entity_System, $T: typeid) -> Entity_Iterator(T) {
    entities, ok := entity_system.entities_by_type[T]
    if !ok {
        entities = bits.U32_MAX
    }

    return Entity_Iterator(T) {
        entity_system = entity_system,
        entities = entities,
        next = proc(self: ^Entity_Iterator(T)) -> (^T, bool) {
            if self.entities >= u32(len(self.entity_system.sparse_indices)) {
                return nil, false
            }

            entry := self.entity_system.sparse_indices[self.entities]
            index := entry.index_or_next
            if index >= u32(len(self.entity_system.entities)) {
                return nil, false
            }

            handle := self.entity_system.handles[index]
            if handle.generation != entry.generation {
                return nil, false
            }

            result := &self.entity_system.entities[index]
            self.entities = entry.next_by_type
            return transmute(^T)result, false
        }
    }
}

entity_system_add :: proc(entity_system: ^Entity_System, entity: $T) -> (handle: Entity_Handle) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    entry_index: u32
    if entity_system.next_index < u32(len(entity_system.sparse_indices)) {
        entry_index = entity_system.next_index
        entry := entity_system.sparse_indices[entry_index]
        next := entry.index_or_next

        entry.generation += 1
        entry.index_or_next = u32(len(entity_system.handles)) 

        handle = {
            generation = entry.generation,
            index = entry_index,
        }

        entity_system.next_index = next
    } else {
        entry_index = u32(len(entity_system.sparse_indices))
        index := len(entity_system.handles)
        handle = Entity_Handle {
            index = entry_index,
            generation = 0
        }

        entry := Sparse_Index {
            index_or_next = u32(index), 
            generation = 0,
        }
        append(&entity_system.sparse_indices, entry)
        entity_system.next_index += 1
    }

    append(&entity_system.handles, handle)
    append(&entity_system.entities, entity)

    entry := &entity_system.sparse_indices[entry_index]
    entry.prev_by_type = bits.U32_MAX

    entry_index_by_type, ok := entity_system.entities_by_type[T]
    entry.next_by_type = ok ? entry_index_by_type : bits.U32_MAX
    entity_system.entities_by_type[T] = entry_index

    if ok {
        entry_by_type := &entity_system.sparse_indices[entry_index_by_type]
        entry_by_type.prev_by_type = entry_index
    }

    return
}

entity_system_destroy :: proc(entity_system: ^Entity_System, entity: ^$T) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    index := mem.ptr_sub(transmute(^Entity)entity, transmute(^Entity)raw_data(entity_system.entities))
    if index < 0 || index >= len(entity_system.handles) {
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
    if entry.generation != handle.generation {
        return
    }

    append(&destroying_handles, handle)
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
    // We need to remove the destroyed entities to avoid redundant draw
    for handle in entity_system.destroying_handles {
        using entity_system

        entry := sparse_indices[handle.index]
        index := entry.index_or_next

        entry.generation += 1
        entry.index_or_next = next_index
        next_index = handle.index

        unordered_remove(&handles, index)
        unordered_remove(&entities, index)

        // Now element as the given index contains new moved value from the last element
        // We need to redirecting the sparse index
        if index < u32(len(handles)) {
            sparse_indices[handles[index].index].index_or_next = index
        }

        // Unlink on the sequence of entities by type
        prev_entry := entry.prev_by_type < u32(len(sparse_indices)) ? &sparse_indices[entry.prev_by_type] : nil
        if prev_entry != nil {
            prev_entry.next_by_type = entry.next_by_type
        }

        next_entry := entry.next_by_type < u32(len(sparse_indices)) ? &sparse_indices[entry.next_by_type] : nil
        if next_entry != nil {
            next_entry.prev_by_type = entry.prev_by_type
        }

        for k, v in entities_by_type {
            if v == handle.index {
                entities_by_type[k] = bits.U32_MAX
            }
        }
    }
    clear(&entity_system.destroying_handles)

    // Actually rendering here, maybe have multiple cameras here
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