#+feature using-stmt

package neonshooter_game

import "core:encoding/base32"
import "core:math/bits"
import "base:intrinsics"
import "core:fmt"
import "core:mem"
import rl "vendor:raylib"

Entity_System :: struct {
    using storage: Entity_Storage
}

Entity_Iterator :: struct($T: typeid) {
    entities: []Any_Entity,
    current: int,
    next: #type proc(self: ^Entity_Iterator(T)) -> (^T, bool)
}

entity_system_init :: proc(entity_system: ^Entity_System, allocator := context.allocator) {
    assert(entity_system != nil)

    entity_storage_init(entity_system)
}

entity_system_deinit :: proc(entity_system: ^Entity_System) {
    entity_storage_deinit(entity_system)
}

entity_system_iter_by_type :: proc(entity_system: ^Entity_System, $T: typeid) -> Entity_Iterator(T) {
    return Entity_Iterator(T) {
        entities = entity_system.entities[entity_type_from_typeid(T)][:],
        current = 0,
        next = proc(self: ^Entity_Iterator(T)) -> (^T, bool) {
            if self.current < len(self.entities) {
                self.current += 1
                return transmute(^T)&self.entities[self.current - 1], true
            }

            return nil, false
        }
    }
}

entity_system_add :: proc(entity_system: ^Entity_System, entity: $T) -> (handle: Entity_Handle) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    handle = entity_storage_add(entity_system, entity)
    return
}

entity_system_destroy :: proc(entity_system: ^Entity_System, entity: ^$T) 
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    entity_storage_destroy(entity_system, entity)
}

entity_system_destroy_w_handle :: proc(using entity_system: ^Entity_System, handle: Entity_Handle) {
    entity_storage_destroy_w_handle(entity_system, handle)
}

entity_system_get :: proc(entity_system: ^Entity_System, handle: Entity_Handle, $T: typeid) -> ^T
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    return entity_storage_get(entity_system, handle, T)
}

entity_system_update :: proc(entity_system: ^Entity_System, dt: f32) {
    for i in 0..<len(Entity_Type) {
        idx := Entity_Type(i)
    
        src := raw_data(entity_system.prev_entities[idx])
        dst := raw_data(entity_system.entities[idx])
        len := len(entity_system.entities[idx]) * size_of(Any_Entity)

        mem.copy(src, dst, len)
    }
    
    entity_system_process_entities(entity_system, dt)
}

entity_system_render :: proc(entity_system: ^Entity_System, alpha: f32) {
    for type_index in 0..<len(Entity_Type) {
        type := Entity_Type(type_index)

        for i in 0..<len(entity_system.entities[type]) {
            curr_entity := transmute(^Entity_Base)&entity_system.entities[type][i]
            prev_entity := transmute(^Entity_Base)&entity_system.prev_entities[type][i]

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
}