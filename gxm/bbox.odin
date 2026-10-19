package gxm

BBox2 :: struct {
	min, max: Vec2,
}


in_bbox :: proc {
	vec2_in_bbox,
}


out_of_bbox :: proc {
	vec2_out_of_bbox,
}


clamp_in_bbox :: proc {
	vec2_clamp_in_bbox,
	vec2_clamp_in_bbox_w_radius,
}


bounce_in_bbox :: proc {
	vec2_bounce_in_bbox,
	vec2_bounce_in_bbox_inplace,
}


@require_results
vec2_in_bbox :: #force_inline proc "contextless" (v: Vec2, b: BBox2) -> bool {
	return !(
		v.x < b.min.x ||
		v.x > b.max.x ||
		v.y < b.min.x ||
		v.y > b.max.x
	)
}


@require_results
vec2_out_of_bbox :: #force_inline proc "contextless" (v: Vec2, b: BBox2) -> bool {
	return !vec2_in_bbox(v, b)
}


@require_results
vec2_clamp_in_bbox :: #force_inline proc "contextless" (v: Vec2, b: BBox2) -> Vec2 {
	res := v
	
	if v.x < b.min.x {
		res.x = b.min.x
	}

	if v.x > b.max.x {
		res.x = b.max.x
	}

	if v.y < b.min.y {
		res.y = b.min.y
	}

	if v.y > b.min.y {
		res.y = b.max.y
	}

	return res
}


@require_results
vec2_clamp_in_bbox_w_radius :: #force_inline proc "contextless" (v: Vec2, radius: f32, b: BBox2) -> Vec2 {
	res := v
	
	if v.x - radius < b.min.x {
		res.x = b.min.x + radius
	}

	if v.x + radius > b.max.x {
		res.x = b.max.x - radius
	}

	if v.y - radius < b.min.y {
		res.y = b.min.y + radius
	}

	if v.y - radius > b.max.y {
		res.y = b.max.y - radius
	}

	return res
}


@require_results
vec2_bounce_in_bbox :: #force_inline proc "contextless" (
	pos, vel: Vec2, b: BBox2, epsilon := f32(0.1)
) -> (
	_new_pos, _new_vel: Vec2
) {
	_new_pos, _new_vel = pos, vel

	if pos.x < b.min.x {
		_new_pos.x = b.min.x + epsilon
		_new_vel.x = +abs(_new_vel.x)
	}

	if pos.x > b.max.x {
		_new_pos.x = b.max.x - epsilon
		_new_vel.x = -abs(_new_vel.x)
	}

	if pos.y < b.min.y {
		_new_pos.y = b.min.y + epsilon
		_new_vel.y = +abs(_new_vel.y)
	}

	if pos.y > b.max.y {
		_new_pos.y = b.max.y - epsilon
		_new_vel.y = -abs(_new_vel.y)
	}
	
	return
}


vec2_bounce_in_bbox_inplace :: #force_inline proc "contextless" (pos, vel: ^Vec2, b: BBox2) {
	pos^, vel^ = vec2_bounce_in_bbox(pos^, vel^, b)
}