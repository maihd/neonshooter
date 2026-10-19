package neonshooter_game

import rl "vendor:raylib"

import gxm "../gxm"

// @region entity

Entity_Kind :: enum(u32) {
    Nil,
    Entity_Player,
    Entity_Bullet,
    Entity_Wanderer,
    Entity_Seeker
}

Entity_Flag :: enum(u32) {}
Entity_Flags :: bit_set[Entity_Flag; u32]

Entity_Layer :: enum(u32) {
    Player,
    Enemy,
}

Entity_Layer_Mask :: bit_set[Entity_Layer; u32]

Entity :: Entity_Base

Entity_Handle :: struct {
    index:      u32,
    generation: u32,
}

Sparse_Index :: struct {
    generation:     u32,    // Generation of entity, same value as valid handle
    index_or_next:  u32,    // This index point to elements of entities/handles
}

// Dont reorder fields when hot reload
Entity_Base :: struct {
    // Metadata

    id:     Entity_Handle,
    kind:   Entity_Kind,
    flags:  Entity_Flags, // Or state if you like state machine

    // Scene

    position:   gxm.Vec2,
    rotation:   f32,
    scale:      gxm.Vec2,

    // For collisions

    radius:     f32,

    layer:      Entity_Layer,
    layer_mask: Entity_Layer_Mask,

    // For rendering

    texture:    rl.Texture,
    tint:       rl.Color,

    // Movement

    velocity:                   gxm.Vec2,
    pos_correction:             gxm.Vec2,

    follow_target:              Entity_Handle,
    next_direction_timer:       f32,
    next_direction_interval:   	f32,

    // Timer

    spawn_timer: f32,

    // Stats

    hp:         i32,    // Do not use int/uint, they are not fixed size
    attack:     i32,
    defense:    i32,

    // Reserved for hot reload
    // Entity is small thing, that need store in a container
    // So we need manually padding
    // If you dont care about alignment, just add big chunk here (maybe 64)
    // Add for hot reload, you just decrement it, this is better for development
    // You dont need to calculate the struct size
    _reversed: [64]u8,
}

// @region entities

MAX_NUM_ENTITIES :: 1024

Entities :: struct {
    items:          [MAX_NUM_ENTITIES]Entity,
    handles:        [MAX_NUM_ENTITIES]Entity_Handle,
    sparse_indices: [MAX_NUM_ENTITIES]Sparse_Index,
    kinds:          [MAX_NUM_ENTITIES]Entity_Kind,
    next_index:     i32,
    num_items:      i32,
}

entities_init :: proc "contextless" (entities: ^Entities) {
    for idx in 1..<MAX_NUM_ENTITIES {
        entities.sparse_indices[idx].index_or_next = u32(idx % MAX_NUM_ENTITIES + 1)
    }

    entities.next_index = 1 // slot 0 is reversed for nil entity
    entities.num_items  = 0
}

entities_new :: proc "contextless" (entities: ^Entities, kind: Entity_Kind) -> (
    _handle: Entity_Handle,
    _entity: ^Entity,
) {

    assert_contextless(entities.next_index > 0 && entities.next_index < MAX_NUM_ENTITIES)
    assert_contextless(entities.num_items < MAX_NUM_ENTITIES)

    sparse := &entities.sparse_indices[entities.next_index]
    sparse.generation += 1

    _handle = Entity_Handle{
        index       = u32(entities.next_index),
        generation  = sparse.generation,
    }

    entities.next_index     = i32(sparse.index_or_next)
    sparse.index_or_next    = u32(entities.num_items)

    entities.handles[entities.num_items]    = _handle
    entities.kinds[entities.num_items]      = kind

    _entity         = &entities.items[entities.num_items]
    _entity.id      = _handle
    _entity.kind    = kind

    entities.num_items += 1

    return
}

entities_remove :: proc "contextless" (entities: ^Entities, handle: Entity_Handle) -> (
    _idx:   int,
    _ok:    bool,
) {
    assert_contextless(handle.index < MAX_NUM_ENTITIES)

    sparse := &entities.sparse_indices[handle.index]
    if sparse.generation != handle.generation {
        return
    }

    _idx    = int(sparse.index_or_next)
    _ok     = true

    // This is safe to remove if check
    // if _idx != int(entities.num_items - 1) {
        last_handle := entities.handles[entities.num_items - 1]
        entities.sparse_indices[last_handle.index].index_or_next = u32(_idx)

        entities.handles[_idx]  = last_handle
        entities.items[_idx]    = entities.items[entities.num_items - 1]
        entities.kinds[_idx]    = entities.kinds[entities.num_items - 1]
    // }

    sparse.index_or_next    = u32(entities.next_index)
    entities.next_index     = i32(handle.index)

    entities.num_items -= 1

    return
}

entities_get :: proc "contextless" (entities: ^Entities, handle: Entity_Handle) -> (_entity: ^Entity) {
    assert_contextless(handle.index < MAX_NUM_ENTITIES)

    sparse := entities.sparse_indices[handle.index];
    idx := int(sparse.generation != handle.generation) * int(sparse.index_or_next)
    return &entities.items[idx]
}

Entities_Iter :: struct {
    entities:   ^Entities,
    kind:       Entity_Kind,
    idx:        int,
}


@require_results
entities_iter_by_kind :: proc "contextless" (entities: ^Entities, kind: Entity_Kind) -> Entities_Iter {
    return {
        entities    = entities,
        kind        = kind,
        idx         = 1,
    }
}


@require_results
entities_it :: #force_inline proc "contextless" (iter: ^Entities_Iter) -> (
    _entity:    ^Entity,
    _handle:    Entity_Handle,
    _ok:        bool
) {
    for i in iter.idx..<int(iter.entities.num_items) {
        if iter.entities.kinds[i] == iter.kind {
            _entity     = &iter.entities.items[i]
            iter.idx    = i + 1
            _handle     = iter.entities.handles[i]
            _ok         = true
            return
        }
    }

    return
}


entities_iter_step_back :: proc "contextless" (iter: ^Entities_Iter) {
    iter.idx -= 1
}


@require_results
entities_of_kind :: #force_inline proc "contextless" (entities: ^Entities, kind: Entity_Kind, idx: ^int) -> (
    _entity:    ^Entity,
    _handle:    Entity_Handle,
    _ok:        bool
) {
    for i in idx^..<int(entities.num_items) {
        if entities.kinds[i] == kind {
            idx    	   ^= i + 1
            _entity     = &entities.items[i]
            _handle     = entities.handles[i]
            _ok         = true
            return
        }
    }

    return
}