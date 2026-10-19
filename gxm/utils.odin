package gxm


dir :: proc {
	vec2_dir,
}


vel :: proc {
	vec2_vel,
}


spread_rad :: proc {
	vec2_spread_rad,
}


@require_results
vec2_dir :: #force_inline proc "contextless" (from, to: Vec2) -> Vec2 {
	return norm(to - from)
}


@require_results
vec2_vel :: #force_inline proc "contextless" (from, to: Vec2, speed: f32) -> Vec2 {
	return vec2_dir(from, to) * speed
}


@require_results
vec2_spread_rad :: #force_inline proc "contextless" (center: Vec2, angle: f32, length := f32(1)) -> Vec2 {
	return center + vec2_from_rad(angle, length)
}