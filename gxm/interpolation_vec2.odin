package gxm

import "core:math"

// @region implementation

@require_results
vec2_integrate :: #force_inline proc "contextless" (pos, vel: Vec2, d: f32) -> Vec2 {
	return pos + vel * d
}


// @require_results
// vec2_integrate_fixed :: #force_inline proc "contextless" (
// 	pos, vel, err: Vec2, 
// 	d: f32
// ) -> (
// 	_new_pos: Vec2, 
// 	_new_err: Vec2,
// )  {
// 	y := dt - 
// }


vec2_integrate_inplace :: #force_inline proc "contextless" (pos: ^Vec2, vel: Vec2, d: f32) {
	pos^ = vec2_integrate(pos^, vel, d)
}


@require_results
vec2_decay :: #force_inline proc "contextless" (v, decay: Vec2, d: f32) -> Vec2 {
	return v * (1 - decay * d)
}


vec2_decay_inplace :: #force_inline proc "contextless" (v: ^Vec2, decay: Vec2, d: f32){
	v^ = vec2_decay(v^, decay, d)
}


@require_results
vec2_move_torwards :: #force_inline proc "contextless" (from, to: Vec2, d: f32) -> Vec2 {
	if distsq(from, to) <= math.F32_EPSILON * math.F32_EPSILON {
		return to
	}

	return from + (to - from) * d
}


vec2_move_towards_inplace :: #force_inline proc "contextless" (pos: ^Vec2, target: Vec2, d: f32) {
	pos^ = vec2_move_torwards(pos^, target, d)
}


@require_results
vec2_shake :: #force_inline proc "contextless" (intensity: f32, time_seed: f32, angle_scale := Vec2{50, 43}, pow_scale := f32(10)) -> Vec2 {
	shake_pow := pow_scale * intensity * intensity
	off_x := math.sin(time_seed * angle_scale.x) * shake_pow
	off_y := math.sin(time_seed * angle_scale.y) * shake_pow
	return { off_x, off_y }
}


@require_results
vec2_smooth_damp :: #force_inline proc "contextless" (
	from, to, vel: Vec2, 
	smooth_time, max_speed, dt: f32
) -> (
	_new_pos: Vec2, 
	_new_vel: Vec2
) {
	smooth_time := max(0.0001, smooth_time)
	omega 		:= 2.0 / smooth_time

	x 	:= omega * dt 
	exp := 1.0 / (1.0 + x + (0.48 + 0.235 * x) * x * x)

	orig_to := to

	max_change 	:= max_speed * smooth_time
	change 		:= clamp(from - to, -max_change, max_change)

	new_vel 	:= (vel + omega * change) * dt
	new_pos		:= to + (change + new_vel) * exp

	if (distsq(to, from) > 0.0) == (distsq(new_pos, to) > 0.0) {
		new_pos = to
		new_vel = (new_pos - to) * dt
	}

	return new_vel, new_pos
}


vec2_smooth_damp_inplace :: #force_inline proc "contextless" (pos: ^Vec2, target: Vec2, vel: ^Vec2, smooth_time, max_speed, dt: f32) {
	pos, vel := vec2_smooth_damp(pos^, target, vel^, smooth_time, max_speed, dt)
}


@require_results
vec2_inv_lerp :: #force_inline proc "contextless" (from, to, vel: Vec2) -> Vec2 {
	return (to - from) / vel
}


@require_results
vec2_remap :: #force_inline proc "contextless" (in_min, in_max, out_min, out_max, vel: Vec2) -> Vec2 {
	return lerp(out_min, out_max, vec2_inv_lerp(in_min, in_max, vel))
}


// funcs:
//  - rotate_torwards
//  - angle_to
//  - dir_and_angle
//  - hit_stop
//  - bbox_in_bbox