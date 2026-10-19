package gxm

import "core:math"

// @region implementation


@require_results
vec3_integrate :: #force_inline proc "contextless" (pos, vel: Vec3, d: f32) -> Vec3 {
	return pos + vel * d
}


@require_results
vec3_integrate_accel :: #force_inline proc "contextless" (pos, vel, accel: Vec3, d: f32) -> (new_pos, new_vel: Vec3) {
	return pos + vel * d, vel + accel * d
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


vec3_integrate_inplace :: #force_inline proc "contextless" (pos: ^Vec3, vel: Vec3, d: f32) {
	pos^ = vec3_integrate(pos^, vel, d)
}


vec3_integrate_accel_inplace :: #force_inline proc "contextless" (pos, vel: ^Vec3, accel: Vec3, d: f32) {
	pos^, vel^ = vec3_integrate_accel(pos^, vel^, accel, d)
}


@require_results
vec3_decay :: #force_inline proc "contextless" (v, decay: Vec3, d: f32) -> Vec3 {
	return v * (1 - decay * d)
}


vec3_decay_inplace :: #force_inline proc "contextless" (v: ^Vec3, decay: Vec3, d: f32){
	v^ = vec3_decay(v^, decay, d)
}


@require_results
vec3_move_torwards :: #force_inline proc "contextless" (from, to: Vec3, d: f32) -> Vec3 {
	if distsq(from, to) <= math.F32_EPSILON * math.F32_EPSILON {
		return to
	}

	return from + (to - from) * d
}


vec3_move_towards_inplace :: #force_inline proc "contextless" (pos: ^Vec3, target: Vec3, d: f32) {
	pos^ = vec3_move_torwards(pos^, target, d)
}


@require_results
vec3_shake :: #force_inline proc "contextless" (intensity: f32, time_seed: f32, angle_scale := Vec3{50, 43, 50}, pow_scale := f32(10)) -> Vec3 {
	shake_pow := pow_scale * intensity * intensity
	off_x := math.sin(time_seed * angle_scale.x) * shake_pow
	off_y := math.sin(time_seed * angle_scale.y) * shake_pow
	off_z := math.sin(time_seed * angle_scale.z) * shake_pow
	return { off_x, off_y, off_z }
}


@require_results
vec3_smooth_damp :: #force_inline proc "contextless" (
	from, to, vel: Vec3, 
	smooth_time, max_speed, dt: f32
) -> (
	_new_pos: Vec3, 
	_new_vel: Vec3,
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


vec3_smooth_damp_inplace :: #force_inline proc "contextless" (pos: ^Vec3, target: Vec3, vel: ^Vec3, smooth_time, max_speed, dt: f32) {
	pos, vel := vec3_smooth_damp(pos^, target, vel^, smooth_time, max_speed, dt)
}


@require_results
vec3_inv_lerp :: #force_inline proc "contextless" (from, to, vel: Vec3) -> Vec3 {
	return (to - from) / vel
}


@require_results
vec3_remap :: #force_inline proc "contextless" (in_min, in_max, out_min, out_max, vel: Vec2) -> Vec2 {
	return lerp(out_min, out_max, vec2_inv_lerp(in_min, in_max, vel))
}


// funcs:
//  - rotate_torwards
//  - angle_to
//  - dir_and_angle
//  - hit_stop
//  - bbox_in_bbox