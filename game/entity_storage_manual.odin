package neonshooter_game

import "core:mem"
import "base:intrinsics"

Sparse_Index :: struct {
    generation: u32,        // Generation of entity, same value as valid handle
    index_or_next: u32,     // This index point to elements of entities/handles
}

entity_storage_init :: proc(entity_storage: ^Entity_Storage, allocator := context.allocator) {
    assert(entity_storage != nil)

    for i in 0..<len(Entity_Type) {
        idx := Entity_Type(i)

        entity_storage.entities[idx].allocator = allocator
        entity_storage.prev_entities[idx].allocator = allocator

        entity_storage.handles[idx].allocator = allocator
        entity_storage.sparse_indices[idx].allocator = allocator
        
        entity_storage.next_index[idx] = 0
    }

    entity_storage.allocator = allocator
}

entity_storage_deinit :: proc(entity_storage: ^Entity_Storage) {
    if entity_storage != nil {
        for i in 0..<len(Entity_Type) {
            idx := Entity_Type(i)

            delete(entity_storage.sparse_indices[idx])
            delete(entity_storage.handles[idx])
            delete(entity_storage.entities[idx])
            delete(entity_storage.prev_entities[idx])
        }

        entity_storage^ = {}
    }
}

entity_type_from_typeid :: proc($T: typeid) -> Entity_Type
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    if T == Entity_Bullet {
        return .Entity_Bullet
    }

    if T == Entity_Player {
        return .Entity_Player
    }

    if T == Entity_Wanderer {
        return .Entity_Wanderer
    }

    return .Entity_Seeker
}

entity_type_to_typeid :: proc(type: Entity_Type) -> typeid
{
    switch type {
        case .Entity_Bullet:
            return Entity_Bullet

        case .Entity_Player:
            return Entity_Player

        case .Entity_Wanderer:
            return Entity_Wanderer

        case .Entity_Seeker:
            return Entity_Seeker
    }

    return Entity_Base
}

entity_storage_add :: proc(entity_storage: ^Entity_Storage, entity: $T) -> (handle: Entity_Handle) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    type := entity_type_from_typeid(T)

    entry_index := entity_storage.next_index[type]
    if entry_index < u32(len(entity_storage.sparse_indices[type])) {
        entry := &entity_storage.sparse_indices[type][entry_index]
        entry.generation += 1

        index := len(entity_storage.handles[type])
        handle = {
            type = type,
            index = entry_index,
            generation = entry.generation
        }

        entity_storage.next_index[type] = entry.index_or_next
        entry.index_or_next = u32(index)
    } else {
        entry_index = u32(len(entity_storage.sparse_indices[type]))
        index := len(entity_storage.handles[type])
        handle = Entity_Handle {
            type = type,
            index = entry_index,
            generation = 0
        }

        entry := Sparse_Index {
            index_or_next = u32(index), 
            generation = 0,
        }
        append(&entity_storage.sparse_indices[type], entry)
        entity_storage.next_index[type] = entry_index + 1
    }

    any_entity := Any_Entity(entity)

    append(&entity_storage.handles[type], handle)
    append(&entity_storage.entities[type], any_entity)
    append(&entity_storage.prev_entities[type], any_entity)

    // entry := &entity_storage.sparse_indices[type][entry_index]
    // entry.entity_type = type

    return
}

entity_storage_destroy :: proc(entity_storage: ^Entity_Storage, entity: ^$T) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    type := entity_type_from_typeid(T)

    index := mem.ptr_sub(transmute(^Any_Entity)entity, transmute(^Any_Entity)raw_data(entity_storage.entities[type]))
    if index < 0 || index >= len(entity_storage.entities[type]) {
        return
    }

    handle := entity_storage.handles[type][index]
    if handle.type == type {
        entity_storage_destroy_w_handle(entity_storage, handle)
    }
}

entity_storage_destroy_w_handle :: proc(entity_storage: ^Entity_Storage, handle: Entity_Handle) {
    if handle.index >= u32(len(entity_storage.sparse_indices[handle.type])) {
        return
    }

    entry_index := handle.index
    entry := &entity_storage.sparse_indices[handle.type][entry_index]
    if handle.generation != entry.generation {
        return
    }

    entity_storage_destroy_w_handle_unsafe(entity_storage, handle)
}

@(private = "file")
entity_storage_destroy_w_handle_unsafe :: proc(entity_storage: ^Entity_Storage, handle: Entity_Handle) {
    entry_index := handle.index
    entry := &entity_storage.sparse_indices[handle.type][entry_index]

    entry.generation += 1
    index := entry.index_or_next

    entry.index_or_next = entity_storage.next_index[handle.type]
    entity_storage.next_index[handle.type] = entry_index

    unordered_remove(&entity_storage.handles[handle.type], index)
    unordered_remove(&entity_storage.entities[handle.type], index)
    unordered_remove(&entity_storage.prev_entities[handle.type], index)

    if index < u32(len(entity_storage.handles[handle.type])) {
        re_entry_index := entity_storage.handles[handle.type][index].index
        re_entry := &entity_storage.sparse_indices[handle.type][re_entry_index]
        re_entry.index_or_next = index

        assert(re_entry_index != entry_index)
        assert(re_entry != entry)
    }
}

entity_storage_get :: proc(entity_storage: ^Entity_Storage, handle: Entity_Handle, $T: typeid) -> ^T
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    type := entity_type_from_typeid(T)
    if handle.type != type {
        return nil
    }

    if int(handle.index) >= len(entity_storage.sparse_indices) {
        return nil
    }

    entry := entity_storage.sparse_indices[handle.type][handle.index]
    entity := &entity_storage.entities[handle.type][entry.index_or_next]
    entity_base := transmute(^Entity_Base)entity

    if entry.generation == handle.generation {
        return transmute(^T)entity_base
    }

    return nil
}