package neonshooter_game

import rl "vendor:raylib"
import gxm "../gxm"

Entity_System :: struct {
    entities: Entities,
}

entity_system_init :: proc "contextless" (entity_system: ^Entity_System) {
    assert_contextless(entity_system != nil)

    entities_init(&entity_system.entities)
}

entity_system_deinit :: proc "contextless" (entity_system: ^Entity_System) {
}

entity_system_add :: proc "contextless" (entity_system: ^Entity_System, entity: Entity) -> Entity_Handle
{
    handle, _entity := entities_new(&entity_system.entities, entity.kind)

    _entity^   = entity
    _entity.id = handle

    return handle
}

entity_system_destroy :: proc "contextless" (entity_system: ^Entity_System, handle: Entity_Handle)
{
    entities_remove(&entity_system.entities, handle)
}

entity_system_get :: proc "contextless" (entity_system: ^Entity_System, handle: Entity_Handle) -> ^Entity
{
    return entities_get(&entity_system.entities, handle)
}

check_collision_layer :: proc "contextless" (a: ^Entity, b: ^Entity) -> bool {
    a_hits_b := a.layer in b.layer_mask
    b_hits_a := b.layer in a.layer_mask

    return a_hits_b || b_hits_a
}

check_collision :: proc "contextless" (a: ^Entity, b: ^Entity) -> bool {
    dist 	:= gxm.distsq(a.position, b.position)
    range 	:= gxm.dot(gxm.vec2(a.radius), gxm.vec2(b.radius))
    return dist <= range
}

entity_system_update :: proc(entity_system: ^Entity_System, dt: f32) {
    {
        bullet_idx := 0
        for bullet, _ in entities_of_kind(&entity_system.entities, .Entity_Bullet, &bullet_idx) {
            exploded := action_move_bullet(bullet, dt)
            if exploded {
            	bullet_idx -= 1
                // entities_iter_step_back(&iter_bullet)
            }
        }
    }

    {
        iter_seeker := entities_iter_by_kind(&entity_system.entities, .Entity_Seeker)
        for seeker, _ in entities_it(&iter_seeker) {
            action_move_seeker(seeker, dt)
        }
    }

    {
        iter_wanderer := entities_iter_by_kind(&entity_system.entities, .Entity_Wanderer)
        for wanderer, _ in entities_it(&iter_wanderer) {
            action_move_wanderer(wanderer, dt)
        }
    }


    for a_idx := i32(0); a_idx < entity_system.entities.num_items; a_idx += 1 {
        a := &entity_system.entities.items[a_idx]

        b_loop: for b_idx := a_idx + 1; b_idx < entity_system.entities.num_items; b_idx += 1 {
            b := &entity_system.entities.items[b_idx]

            if check_collision_layer(a, b) && check_collision(a, b) {
                if a.kind == .Entity_Bullet {
                    b.hp -= b.attack
                    if b.hp <= 0 {
                        #partial switch b.kind {
                        case .Entity_Seeker:
                            explose_seeker(b)
                            b_idx -= 1

                        case .Entity_Wanderer:
                            explose_wanderer(b)
                            b_idx -= 1
                        }
                    }

                    explose_bullet(a)
                    a_idx -= 1
                    break b_loop
                }

                if b.kind == .Entity_Bullet {
                    a.hp -= b.attack
                    if a.hp <= 0 {
                        #partial switch a.kind {
                        case .Entity_Seeker:
                            explose_seeker(a)
                            a_idx -= 1
                            break b_loop

                        case .Entity_Wanderer:
                            explose_wanderer(a)
                            a_idx -= 1
                            break b_loop
                        }
                    }

                    explose_bullet(b)
                    b_idx -= 1
                    continue b_loop
                }

                if a.kind == .Entity_Player {
                    a.hp -= min(1, b.attack - a.defense)
                    if a.hp <= 0 {
                        explode_player()
                        return
                    }

                    #partial switch b.kind {
                    case .Entity_Seeker:
                        explose_seeker(b)
                        b_idx -= 1
                        break b_loop

                    case .Entity_Wanderer:
                        explose_wanderer(b)
                        b_idx -= 1
                        break b_loop
                    }
                }

                if b.kind == .Entity_Player {
                    b.hp -= min(1, a.attack - b.defense)
                    if b.hp <= 0 {
                        explode_player()
                        return
                    }

                    #partial switch a.kind {
                    case .Entity_Seeker:
                        explose_seeker(a)
                        a_idx -= 1
                        break b_loop

                    case .Entity_Wanderer:
                        explose_wanderer(a)
                        a_idx -= 1
                        break b_loop
                    }
                }
            }
        }
    }
}

entity_system_render :: proc "contextless" (entity_system: ^Entity_System, alpha: f32) {
    for i in 0..<entity_system.entities.num_items {
        entity := &entity_system.entities.items[i]

        w   := f32(entity.texture.width)
        h   := f32(entity.texture.height)
        sx  := entity.scale.x
        sy  := entity.scale.y

        pos := entity.position
        dst := rl.Rectangle{ pos.x, pos.y, w * sx, h * sy }
        
        rl.DrawTexturePro(
            texture     = entity.texture,
            source      = { 0, 0, w, h },
            dest        = dst,
            origin      = [2]f32{w, h} * 0.5,
            rotation    = gxm.degrees(entity.rotation),
            tint        = rl.WHITE
        )
    }
}
