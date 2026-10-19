package neonshooter_game

import rl "vendor:raylib"

Post_Processing :: struct {
	crt_shader: 			rl.Shader,
    bloom_shader: 			rl.Shader,

    aberration_amount_loc:  i32,
    time_loc:               i32,
    bloom_intensity_loc:    i32,
    
    current_aberration:     f32, // The current frame value
    target_aberration:      f32, // The baseline value (typically 0.001)
    bloom_intensity:        f32,
}

post_processing_init :: proc(post_processing: ^Post_Processing) {
	post_processing.crt_shader = rl.LoadShader(nil, Assets_Shaders_CRT_Frag)
    post_processing.bloom_shader = rl.LoadShader(nil, Assets_Shaders_Bloom_Frag)

    post_processing.aberration_amount_loc = rl.GetShaderLocation(post_processing.crt_shader, "uAmount")
    post_processing.time_loc = rl.GetShaderLocation(post_processing.crt_shader, "uTime")
    post_processing.bloom_intensity_loc = rl.GetShaderLocation(post_processing.crt_shader, "uBloomIntensity")

    
    post_processing.target_aberration = 0.001 
    post_processing.current_aberration = post_processing.target_aberration
    post_processing.bloom_intensity = 1.2
}

post_processing_deinit :: proc(post_processing: ^Post_Processing) {
	rl.UnloadShader(post_processing.bloom_shader)
    rl.UnloadShader(post_processing.crt_shader)

    post_processing^ = {}
}

post_processing_update :: proc(post_processing: ^Post_Processing, time: f32) {
	// dt := fxm.to_f32(fxm.mul(fxm.fixed(10), time))
	dt := time
	post_processing.current_aberration = rl.Lerp(post_processing.current_aberration, post_processing.target_aberration, dt)
}

post_processing_draw :: proc(post_processing: ^Post_Processing, render_target: rl.RenderTexture2D) {
	// Bloom
	{
		rl.BeginTextureMode(render_target)
	    rl.BeginShaderMode(post_processing.bloom_shader)
	
	    src := rl.Rectangle{ +0.0, +0.0, f32(render_target.texture.width), -f32(render_target.texture.height) }
	    dst := rl.Rectangle{ +0.0, +0.0, f32(render_target.texture.width), -f32(render_target.texture.height) }
	    rl.DrawTexturePro(render_target.texture, src, dst, {0, 0}, 0.0, rl.WHITE)
	
	
	    rl.EndShaderMode()
	    rl.EndTextureMode()
	}

    // CRT
    {
	    rl.BeginShaderMode(post_processing.crt_shader)
	
	    current_time := f32(rl.GetTime())
		rl.SetShaderValue(post_processing.crt_shader, post_processing.aberration_amount_loc, &post_processing.current_aberration, .FLOAT)
		rl.SetShaderValue(post_processing.crt_shader, post_processing.time_loc, &current_time, .FLOAT)
		rl.SetShaderValue(post_processing.crt_shader, post_processing.bloom_intensity_loc, &post_processing.bloom_intensity, .FLOAT)
	    
	    src := rl.Rectangle{ +0.0, +0.0, f32(render_target.texture.width), -f32(render_target.texture.height) }
	    dst := rl.Rectangle{ -0.0, -0.0, f32(rl.GetScreenWidth()), f32(rl.GetScreenHeight()) }
	    rl.DrawTexturePro(render_target.texture, src, dst, {0, 0}, 0.0, rl.WHITE)
	
	
	    rl.EndShaderMode()
    }
}
