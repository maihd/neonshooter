package neonshooter_game

import "core:encoding/base32"
import "core:math/bits"
import "base:intrinsics"
import "core:fmt"
import "core:mem"
import rl "vendor:raylib"

Entity_System :: struct {
    entities: [dynamic]Entity,
    prev_entities: [dynamic]Entity, // This is for rendering

    handles: [dynamic]Entity_Handle,
    sparse_indices: [dynamic]Sparse_Index, // Persistant size, never remove items
    next_index: u32,    // This index point to elements of sparse_indices

    allocator: mem.Allocator,
    entities_by_type: [Entity_Type]Entity_Iterator_Entry,
}

Entity_Iterator :: struct($T: typeid) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    entity_system: ^Entity_System,
    entities: []Entity,
    current: u32,
    next: proc(self: ^Entity_Iterator(T)) -> (^T, bool)
}

Entity_Iterator_Entry :: struct {
    head: u32,
    tail: u32,
}

Sparse_Index :: struct {
    generation: u32,
    index_or_next: u32,     // This index point to elements of entities/handles

    entity_type: Entity_Type,    // Type of entity
    prev_by_type: u32,      // Previous entry index (same entity type)
    next_by_type: u32,      // Next entry index (same entity type)
}

Entity_Handle :: bit_field u32 {
    index: u32 | 24,        // This index point to elements of sparse_indices
    generation: u32 | 8,
}

entity_system_init :: proc(entity_system: ^Entity_System, allocator := context.allocator) {
    assert(entity_system != nil)

    entity_system.entities.allocator = allocator
    entity_system.prev_entities.allocator = allocator

    entity_system.handles.allocator = allocator
    entity_system.sparse_indices.allocator = allocator

    entity_system.next_index = 0
    entity_system.allocator = allocator

    for &iter in entity_system.entities_by_type {
        iter.head = bits.U32_MAX
        iter.tail = bits.U32_MAX
    }
}

entity_system_deinit :: proc(entity_system: ^Entity_System) {
    if entity_system != nil {
        delete(entity_system.sparse_indices)
        delete(entity_system.handles)
        delete(entity_system.entities)
        delete(entity_system.prev_entities)

        entity_system^ = {}
    }
}

entity_system_iter_by_type :: proc(entity_system: ^Entity_System, $T: typeid) -> Entity_Iterator(T) {
    iter_entry := entity_system.entities_by_type[entity_typeid_to_enum(T)]

    return Entity_Iterator(T) {
        entity_system = entity_system,
        entities = entity_system.entities[:],
        current = iter_entry.head,
        next = proc(self: ^Entity_Iterator(T)) -> (^T, bool) {
            for self.current < u32(len(self.entity_system.sparse_indices)) {
                entry := self.entity_system.sparse_indices[self.current]
                self.current = entry.next_by_type

                handle := self.entity_system.handles[entry.index_or_next]
                if handle.generation != entry.generation {
                    continue
                }

                entity := transmute(^T)&self.entity_system.entities[entry.index_or_next]
                return entity, true
            }

            return nil, false
        }
    }
}

entity_system_add :: proc(entity_system: ^Entity_System, entity: $T) -> (handle: Entity_Handle) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{

    entry_index := entity_system.next_index
    if entry_index < u32(len(entity_system.sparse_indices)) {
        entry := &entity_system.sparse_indices[entry_index]
        entry.generation += 1

        index := len(entity_system.handles)
        handle = {
            index = entry_index,
            generation = entry.generation
        }

        entity_system.next_index = entry.index_or_next
        entry.index_or_next = u32(index)
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
        entity_system.next_index = entry_index + 1
    }

    append(&entity_system.handles, handle)
    append(&entity_system.entities, entity)
    append(&entity_system.prev_entities, entity)

    entry := &entity_system.sparse_indices[entry_index]
    entry.entity_type = entity_typeid_to_enum(T)

    iter_entry := &entity_system.entities_by_type[entry.entity_type]

    if iter_entry.tail < u32(len(entity_system.sparse_indices)) {
        last_entry := &entity_system.sparse_indices[iter_entry.tail]
        last_entry.next_by_type = entry_index
        
        entry.prev_by_type = iter_entry.tail
        entry.next_by_type = bits.U32_MAX

        iter_entry.tail = entry_index
    } else {
        entry.prev_by_type = bits.U32_MAX
        entry.next_by_type = bits.U32_MAX

        iter_entry.head = entry_index
        iter_entry.tail = entry_index
    }

    return
}

entity_system_destroy :: proc(entity_system: ^Entity_System, entity: ^$T) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    index := mem.ptr_sub(transmute(^Entity)entity, transmute(^Entity)raw_data(entity_system.entities))
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

    entry_index := handle.index
    entry := &sparse_indices[entry_index]
    if handle.generation != entry.generation {
        return
    }

    entity_system_destroy_w_handle_unsafe(entity_system, handle)
}

@(private = "file")
entity_system_destroy_w_handle_unsafe :: proc(using entity_system: ^Entity_System, handle: Entity_Handle) {
    entry_index := handle.index
    entry := &sparse_indices[entry_index]

    entry.generation += 1
    index := entry.index_or_next

    entry.index_or_next = next_index
    next_index = entry_index

    unordered_remove(&handles, index)
    unordered_remove(&entities, index)
    unordered_remove(&prev_entities, index)

    if index < u32(len(handles)) {
        re_entry_index := handles[index].index
        re_entry := &sparse_indices[re_entry_index]
        re_entry.index_or_next = index

        assert(re_entry_index != entry_index)
        assert(re_entry != entry)
    }
    
    prev := entry.prev_by_type < u32(len(sparse_indices)) ? &sparse_indices[entry.prev_by_type] : nil
    next := entry.next_by_type < u32(len(sparse_indices)) ? &sparse_indices[entry.next_by_type] : nil

    if prev != nil {
        prev.next_by_type = entry.next_by_type
    }

    if next != nil {
        next.prev_by_type = entry.prev_by_type
    }

    iter := &entities_by_type[entry.entity_type]
    if iter.head == entry_index {
        iter.head = entry.next_by_type
    } 
    if iter.tail == entry_index {
        iter.tail = entry.prev_by_type
    }

    entry.prev_by_type = bits.U32_MAX
    entry.next_by_type = bits.U32_MAX
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

    return entry.generation == handle.generation ? transmute(^T)entity_base : nil
}

entity_system_update :: proc(entity_system: ^Entity_System, dt: f32) {
    mem.copy(raw_data(entity_system.prev_entities), raw_data(entity_system.entities), len(entity_system.entities) * size_of(Entity))
}

entity_system_render :: proc(entity_system: ^Entity_System, alpha: f32) {
    for i in 0..<len(entity_system.entities) {
        curr_entity := transmute(^Entity_Base)&entity_system.entities[i]
        prev_entity := transmute(^Entity_Base)&entity_system.prev_entities[i]

        entity := curr_entity^
        entity.position = lerp(prev_entity.position, entity.position, alpha)
        entity.rotation = lerp(prev_entity.rotation, entity.rotation, alpha)
        entity.scale = lerp(prev_entity.scale, entity.scale, alpha)

        w := f32(entity.texture.width)
        h := f32(entity.texture.height)
        sx := f32(entity.scale.x)
        sy := f32(entity.scale.y)
        
        rl.DrawTexturePro(
            texture = entity.texture,
            source = { 0, 0, w, h },
            dest = { entity.position.x, entity.position.y, w * sx, h * sy },
            origin = vec2(w, h) * 0.5,
            rotation = degrees(entity.rotation),
            tint = rl.WHITE
        )
    }
}