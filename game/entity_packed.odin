package neonshooter_game

import "base:intrinsics"

Entity :: union {
    Entity_Base,
    Entity_Player,
    Entity_Bullet,
    Entity_Seeker,
    Entity_Wanderer,
}

Entity_Type :: enum {
    None,
    Entity_Player,
    Entity_Bullet,
    Entity_Seeker,
    Entity_Wanderer,
}

entity_typeid_to_enum :: proc($T: typeid) -> Entity_Type
    where intrinsics.type_is_subtype_of(T, Entity_Base)
{
    when T == Entity_Bullet {
        return .Entity_Bullet
    }

    when T == Entity_Bullet {
        return .Entity_Bullet
    }

    when T == Entity_Player {
        return .Entity_Player
    }

    when T == Entity_Seeker {
        return .Entity_Seeker
    }

    when T == Entity_Wanderer {
        return .Entity_Wanderer
    }

    return .None
}