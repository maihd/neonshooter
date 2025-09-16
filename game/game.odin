package neonshooter_game

import "core:fmt"
import rl "vendor:raylib"

@(export)
game_init :: proc() {

}

@(export)
game_deinit :: proc() {

}

@(export)
game_reload :: proc() {

}

@(export)
game_update :: proc() {

}

@(export)
game_render :: proc() {
    rl.DrawCircleV({100, 100}, 30, rl.WHITE)
}