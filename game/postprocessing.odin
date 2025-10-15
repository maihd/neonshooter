package neonshooter_game

import rl "vendor:raylib"

Post_Processing :: struct {
    render_texture: rl.RenderTexture,
    bloom_shader: rl.Shader,
}

post_processing_init :: proc(post_processing: ^Post_Processing) {
    post_processing.bloom_shader = rl.LoadShader(nil, Assets_Shaders_Bloom_Frag)
    post_processing.render_texture = rl.LoadRenderTexture(rl.GetScreenWidth(), rl.GetScreenHeight())
}

post_processing_deinit :: proc(post_processing: ^Post_Processing) {
    rl.UnloadRenderTexture(post_processing.render_texture)
    rl.UnloadShader(post_processing.bloom_shader)

    post_processing^ = {}
}

post_processing_begin :: proc(post_processing: ^Post_Processing) {
    rl.BeginTextureMode(post_processing.render_texture)
    rl.ClearBackground(rl.BLACK)
}

post_processing_end :: proc(post_processing: ^Post_Processing) {
    rl.EndTextureMode()

    rl.BeginShaderMode(post_processing.bloom_shader)
    rl.DrawTextureRec(post_processing.render_texture.texture, { 0, 0, f32(rl.GetScreenWidth()), f32(-rl.GetScreenHeight()) }, vec2(0, 0), rl.WHITE)
    rl.EndShaderMode()
}