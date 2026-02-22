
// This is generated file, DONOT EDIT!

package neonshooter_game

entity_type_from_typeid :: proc($T: typeid) -> Entity_Type {
    if T == Entity_Player do return .Entity_Player
    if T == Entity_Bullet do return .Entity_Bullet
    if T == Entity_Wanderer do return .Entity_Wanderer
    if T == Entity_Seeker do return .Entity_Seeker

    return .Entity_Player
}

entity_type_to_typeid :: proc(type: Entity_Type) -> typeid {
    switch type {
        case .Entity_Player: return Entity_Player
        case .Entity_Bullet: return Entity_Bullet
        case .Entity_Wanderer: return Entity_Wanderer
        case .Entity_Seeker: return Entity_Seeker
    }

    return Entity_Player
}
    