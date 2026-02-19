// This is generated file, DONOT EDIT!

package neonshooter_game

import "core:mem"

Entity_Type :: enum(u32) {
    Entity_Player,
    Entity_Bullet,
    Entity_Wanderer,
    Entity_Seeker
}

Entity_Handle :: bit_field(u64) {
    type: Entity_Type | 32,
    index: u32 | 24,
    generation: u32 | 8
}

Any_Entity :: union {
    Entity_Player,
    Entity_Bullet,
    Entity_Wanderer,
    Entity_Seeker
}

Entity_Storage :: struct {
    entities: [Entity_Type][dynamic]Any_Entity,
    prev_entities: [Entity_Type][dynamic]Any_Entity,
    
    handles: [Entity_Type][dynamic]Entity_Handle,
    sparse_indices: [Entity_Type][dynamic]Sparse_Index,
    next_index: [Entity_Type]u32,
    
    allocator: mem.Allocator,
}