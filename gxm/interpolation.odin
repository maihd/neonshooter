package gxm

integrate :: proc {
	vec2_integrate,
	vec2_integrate_inplace,

	vec3_integrate,
	vec3_integrate_accel,
	
	vec3_integrate_inplace,
	vec3_integrate_accel_inplace,
}

decay :: proc {
	vec2_decay,
	vec2_decay_inplace,
	
	vec3_decay,
	vec3_decay_inplace,
}

smooth_damp :: proc {
	vec2_smooth_damp,
	vec2_smooth_damp_inplace,
	
	vec3_smooth_damp,
	vec3_smooth_damp_inplace,
}