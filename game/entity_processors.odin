
// This is generated file, DONOT EDIT!

package neonshooter_game

entity_system_process_entities :: proc(storage: ^Entity_Storage, dt: f32) {
    {
        entities_0 := storage.entities[.Entity_Bullet]
        for &entity_0 in entities_0 {
            is_break := _process_bullet_movement(transmute(^Entity_Bullet)&entity_0, dt)
            if is_break do break
        }
    }

    {
        entities_0 := storage.entities[.Entity_Seeker]
        for &entity_0 in entities_0 {
            is_break := _process_seeker_movement(transmute(^Entity_Seeker)&entity_0, dt)
            if is_break do break
        }
    }

    {
        entities_0 := storage.entities[.Entity_Wanderer]
        for &entity_0 in entities_0 {
            is_break := _process_wanderer_movement(transmute(^Entity_Wanderer)&entity_0, dt)
            if is_break do break
        }
    }

    {
        entities_0 := storage.entities[.Entity_Bullet]
        entities_1 := storage.entities[.Entity_Seeker]
        for &entity_0 in entities_0 {
            for &entity_1 in entities_1 {
                is_break := _process_bullet_seeker_collisions(transmute(^Entity_Bullet)&entity_0, transmute(^Entity_Seeker)&entity_1, dt)
                if is_break do break
            }
        }
    }

    {
        entities_0 := storage.entities[.Entity_Bullet]
        entities_1 := storage.entities[.Entity_Wanderer]
        for &entity_0 in entities_0 {
            for &entity_1 in entities_1 {
                is_break := _process_bullet_wanderer_collisions(transmute(^Entity_Bullet)&entity_0, transmute(^Entity_Wanderer)&entity_1, dt)
                if is_break do break
            }
        }
    }

    {
        entities_0 := storage.entities[.Entity_Player]
        entities_1 := storage.entities[.Entity_Seeker]
        for &entity_0 in entities_0 {
            for &entity_1 in entities_1 {
                is_break := _process_player_seeker_collisions(transmute(^Entity_Player)&entity_0, transmute(^Entity_Seeker)&entity_1, dt)
                if is_break do break
            }
        }
    }

    {
        entities_0 := storage.entities[.Entity_Player]
        entities_1 := storage.entities[.Entity_Wanderer]
        for &entity_0 in entities_0 {
            for &entity_1 in entities_1 {
                is_break := _process_player_wanderer_collisions(transmute(^Entity_Player)&entity_0, transmute(^Entity_Wanderer)&entity_1, dt)
                if is_break do break
            }
        }
    }
}
    