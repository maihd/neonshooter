package main_release

import "../game"
import rl "vendor:raylib"

main :: proc() {
    rl.InitWindow(800, 600, "Neon Shooter - Odin + Raylib");
    defer rl.CloseWindow()

    game.game_init()
    defer game.game_deinit()

    for !rl.WindowShouldClose() {
        game.game_update()

        rl.BeginDrawing()
        defer rl.EndDrawing()

        rl.ClearBackground(rl.BLACK)
        defer rl.DrawFPS(10, 10)

        game.game_render()
    }
}